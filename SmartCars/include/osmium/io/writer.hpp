#ifndef OSMIUM_IO_WRITER_HPP
#define OSMIUM_IO_WRITER_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2021 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <osmium/io/compression.hpp>
#include <osmium/io/detail/output_format.hpp>
#include <osmium/io/detail/queue_util.hpp>
#include <osmium/io/detail/read_write.hpp>
#include <osmium/io/detail/write_thread.hpp>
#include <osmium/io/error.hpp>
#include <osmium/io/file.hpp>
#include <osmium/io/header.hpp>
#include <osmium/io/writer_options.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/thread/pool.hpp>
#include <osmium/thread/util.hpp>
#include <osmium/util/config.hpp>
#include <osmium/version.hpp>

#include <cassert>
#include <cstddef>
#include <exception>
#include <functional>
#include <future>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>

namespace osmium {

    namespace memory {
        class Item;
    } //namespace memory

    namespace io {

        namespace detail {

            inline std::size_t get_output_queue_size() noexcept {
                return osmium::config::get_max_queue_size("OUTPUT", 20);
            }

        } // namespace detail

        /**
         * This is the user-facing interface for writing OSM files. Instantiate
         * an object of this class with a file name or osmium::io::File object
         * and optionally the data for the header and then call operator() on
         * it to write Buffers or Items.
         *
         * The writer uses multithreading internally to do the actual encoding
         * of the data into the intended format, possible compress the data and
         * then write it out. But this is intentionally hidden from the user
         * of this class who can use it without knowing those details.
         *
         * If you are done call the close() method to finish up. Only if you
         * don't get an exception from the close() method, you can be sure
         * the data is written correctly (modulo operating system buffering).
         * The destructor of this class will also do the right thing if you
         * forget to call close(), but because the destructor can't throw you
         * will not get informed about any problems.
         *
         * The writer is usually used to write complete blocks of data stored
         * in osmium::memory::Buffers. But you can also write single
         * osmium::memory::Items. In this case the Writer uses an internal
         * Buffer.
         */
        class Writer {

            enum {
                default_buffer_size = 10UL * 1024UL * 1024UL
            };

            osmium::io::File m_file;

            detail::future_string_queue_type m_output_queue{detail::get_output_queue_size(), "raw_output"};

            std::unique_ptr<osmium::io::detail::OutputFormat> m_output{nullptr};

            osmium::memory::Buffer m_buffer{};

            osmium::io::Header m_header;

            size_t m_buffer_size = default_buffer_size;

            std::future<std::size_t> m_write_future{};

            osmium::thread::thread_handler m_thread{};

            // Checking the m_write_future is much more expensive then checking
            // one atomic bool, so we set this bool in the write_thread when
            // the writer should check the future...
            std::atomic_bool m_notification{false};

            enum class status {
                okay   = 0, // normal writing
                error  = 1, // some error occurred while writing
                closed = 2  // close() called successfully
            } m_status = status::okay;

            // Has the header already bin written to the file?
            bool m_header_written = false;

            // This function will run in a separate thread.
            static void write_thread(detail::future_string_queue_type& output_queue,
                                     std::unique_ptr<osmium::io::Compressor>&& compressor,
                                     std::promise<std::size_t>&& write_promise,
                                     std::atomic_bool* notification) {
                detail::WriteThread write_thread{output_queue,
                                                 std::move(compressor),
                                                 std::move(write_promise),
                                                 notification};
                write_thread();
            }

            void write_header() {
                if (m_header.get("generator").empty()) {
                    m_header.set("generator", "libosmium/" LIBOSMIUM_VERSION_STRING);
                }

                m_output->write_header(m_header);

                m_header_written = true;
            }

            void do_write(osmium::memory::Buffer&& buffer) {
                if (!m_header_written) {
                    write_header();
                }
                if (buffer && buffer.committed() > 0) {
                    m_output->write_buffer(std::move(buffer));
                }
            }

            void do_flush() {
                if (!m_header_written) {
                    write_header();
                }
                if (m_notification) {
                    osmium::thread::check_for_exception(m_write_future);
                }
                if (m_buffer && m_buffer.committed() > 0) {
                    osmium::memory::Buffer buffer{m_buffer_size,
                                                  osmium::memory::Buffer::auto_grow::no};
                    using std::swap;
                    swap(m_buffer, buffer);

                    m_output->write_buffer(std::move(buffer));
                }
            }

            template <typename TFunction, typename... TArgs>
            void ensure_cleanup(TFunction func, TArgs&&... args) {
                if (m_status != status::okay) {
                    throw io_error("Can not write to writer when in status 'closed' or 'error'");
                }

                try {
                    func(std::forward<TArgs>(args)...);
                } catch (...) {
                    m_status = status::error;
                    detail::add_to_queue(m_output_queue, std::current_exception());
                    detail::add_end_of_data_to_queue(m_output_queue);
                    throw;
                }
            }

            struct options_type {
                osmium::io::Header header;
                overwrite allow_overwrite = overwrite::no;
                fsync sync = fsync::no;
                osmium::thread::Pool* pool = nullptr;
            };

            static void set_option(options_type& options, osmium::thread::Pool& pool) {
                options.pool = &pool;
            }

            static void set_option(options_type& options, const osmium::io::Header& header) {
                options.header = header;
            }

            static void set_option(options_type& options, overwrite value) {
                options.allow_overwrite = value;
            }

            static void set_option(options_type& options, fsync value) {
                options.sync = value;
            }

            void do_close() {
                if (m_status == status::okay) {
                    ensure_cleanup([&](){
                        do_write(std::move(m_buffer));
                        m_output->write_end();
                        m_status = status::closed;
                        detail::add_end_of_data_to_queue(m_output_queue);
                    });
                }
            }

        public:

            /**
             * The constructor of the Writer object opens a file and writes the
             * header to it.
             *
             * @param file File (contains name and format info) to open.
             * @param args All further arguments are optional and can appear
             *             in any order:
             *
             * * osmium::io::Header: Optional header data. If this is
             *       not given, a default constructed osmium::io::Header
             *       object will be used.
             *
             * * osmium::io::overwrite: Allow overwriting of existing file?
             *       Can be osmium::io::overwrite::allow or
             *       osmium::io::overwrite::no (default).
             *
             * * osmium::io::fsync: Should fsync be called on the file
             *       before closing it? Can be osmium::io::fsync::yes or
             *       osmium::io::fsync::no (default).
             *
             * * osmium::thread::Pool&: Reference to a thread pool that should
             *      be used for writing instead of the default pool. Usually
             *      it is okay to use the statically initialized shared
             *      default pool, but sometimes you want or need your own.
             *      For instance when your program will fork, using the
             *      statically initialized pool will not work.
             *
             * @throws osmium::io_error If there was an error.
             * @throws std::system_error If the file could not be opened.
             */
            template <typename... TArgs>
            explicit Writer(const osmium::io::File& file, TArgs&&... args) :
                m_file(file.check()) {
                assert(!m_file.buffer()); // XXX can't handle pseudo-files

                options_type options;
                (void)std::initializer_list<int>{
                    (set_option(options, args), 0)...
                };

                if (!options.pool) {
                    options.pool = &thread::Pool::default_instance();
                }

                m_header = options.header;

                m_output = osmium::io::detail::OutputFormatFactory::instance().create_output(*options.pool, m_file, m_output_queue);

                std::unique_ptr<osmium::io::Compressor> compressor =
                    CompressionFactory::instance().create_compressor(file.compression(),
                                                                     osmium::io::detail::open_for_writing(m_file.filename(), options.allow_overwrite),
                                                                     options.sync);

                std::promise<std::size_t> write_promise;
                m_write_future = write_promise.get_future();
                m_thread = osmium::thread::thread_handler{write_thread, std::ref(m_output_queue), std::move(compressor), std::move(write_promise), &m_notification};
            }

            template <typename... TArgs>
            explicit Writer(const std::string& filename, TArgs&&... args) :
                Writer(osmium::io::File{filename}, std::forward<TArgs>(args)...) {
            }

            template <typename... TArgs>
            explicit Writer(const char* filename, TArgs&&... args) :
                Writer(osmium::io::File{filename}, std::forward<TArgs>(args)...) {
            }

            Writer(const Writer&) = delete;
            Writer& operator=(const Writer&) = delete;

            Writer(Writer&&) = delete;
            Writer& operator=(Writer&&) = delete;

            ~Writer() noexcept {
                try {
                    do_close();
                } catch (...) {
                    // Ignore any exceptions because destructor must not throw.
                }
            }

            /**
             * Get the currently configured size of the internal buffer.
             */
            size_t buffer_size() const noexcept {
                return m_buffer_size;
            }

            /**
             * Set the size of the internal buffer. This will only take effect
             * if you have not yet written anything or after the next flush().
             */
            void set_buffer_size(size_t size) noexcept {
                m_buffer_size = size;
            }

            /**
             * Set header. This will overwrite a header set in the constructor.
             *
             * Has to be called before writing anything to the file, otherwise
             * this will not do anything.
             */
            void set_header(const osmium::io::Header& header) {
                m_header = header;
            }

            /**
             * Flush the internal buffer if it contains any data. This is
             * usually not needed as the buffer gets flushed on close()
             * automatically.
             *
             * @throws Some form of osmium::io_error when there is a problem.
             */
            void flush() {
                ensure_cleanup([&](){
                    do_flush();
                });
            }

            /**
             * Write contents of a buffer to the output file. The buffer is
             * moved into this function and will be in an undefined moved-from
             * state afterwards.
             *
             * @param buffer Buffer that is being written out.
             * @throws Some form of osmium::io_error when there is a problem.
             */
            void operator()(osmium::memory::Buffer&& buffer) {
                ensure_cleanup([&](){
                    do_flush();
                    do_write(std::move(buffer));
                });
            }

            /**
             * Add item to the internal buffer for eventual writing to the
             * output file.
             *
             * @param item Item to write (usually an OSM object).
             * @throws Some form of osmium::io_error when there is a problem.
             */
            void operator()(const osmium::memory::Item& item) {
                ensure_cleanup([&](){
                    if (!m_buffer) {
                        m_buffer = osmium::memory::Buffer{m_buffer_size,
                                                          osmium::memory::Buffer::auto_grow::no};
                    }
                    try {
                        m_buffer.push_back(item);
                    } catch (const osmium::buffer_is_full&) {
                        do_flush();
                        m_buffer.push_back(item);
                    }
                });
            }

            /**
             * Flushes internal buffer and closes output file. If you do not
             * call this, the destructor of Writer will also do the same
             * thing. But because this call might throw an exception, which
             * the destructor will ignore, it is better to call close()
             * explicitly.
             *
             * @returns Number of bytes written to the file (or 0 if it can
             *          not be determined).
             * @throws Some form of osmium::io_error when there is a problem.
             */
            std::size_t close() {
                do_close();

                if (m_write_future.valid()) {
                    return m_write_future.get();
                }

                return 0;
            }

        }; // class Writer

    } // namespace io

} // namespace osmium

#endif // OSMIUM_IO_WRITER_HPP
