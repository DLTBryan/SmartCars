#ifndef OSMIUM_MEMORY_CALLBACK_BUFFER_HPP
#define OSMIUM_MEMORY_CALLBACK_BUFFER_HPP

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

#include <osmium/memory/buffer.hpp>

#include <cstddef>
#include <functional>
#include <utility>

namespace osmium {

    namespace memory {

        /**
         * This is basically a wrapper around osmium::memory::Buffer with an
         * additional callback function that is called whenever the buffer is
         * full.
         *
         * The internal buffer is created with the `initial_buffer_size` set
         * in the constructor. When it grows beyond the `max_buffer_size` set
         * in the constructor, the callback function is called with the buffer
         * and a new, empty buffer is created internally.
         *
         * Note that the buffer can grow beyond the initial buffer size if
         * needed. This can happen if a new object doesn't fit into the rest
         * of the buffer available or if no callback function is set (yet).
         *
         * Example:
         * @code
         *     CallbackBuffer cb;
         *     cb.set_callback([&](osmium::memory::Buffer&& buffer) {
         *         ...handle buffer...
         *     }
         *     osmium::builder::add_node(cb.buffer(), _id(9), ...);
         *     osmium::builder::add_way(cb.buffer(), _id(27), ...);
         * @endcode
         */
        class CallbackBuffer {

        public:

            /// The type for the callback function
            using callback_func_type = std::function<void(osmium::memory::Buffer&&)>;

        private:

            enum {
                default_initial_buffer_size = 1024UL * 1024UL
            };

            enum {
                default_max_buffer_size =  800UL * 1024UL
            };

            osmium::memory::Buffer m_buffer;
            std::size_t m_initial_buffer_size;
            std::size_t m_max_buffer_size;
            callback_func_type m_callback;

        public:

            /**
             * Construct a CallbackBuffer without a callback function. You
             * can later call set the callback with set_callback().
             *
             * @param initial_buffer_size The initial size of newly created
             *                            internal buffers.
             * @param max_buffer_size If the buffer grows beyond this size the
             *                        callback will be called.
             */
            explicit CallbackBuffer(std::size_t initial_buffer_size = default_initial_buffer_size, std::size_t max_buffer_size = default_max_buffer_size) :
                m_buffer(initial_buffer_size, osmium::memory::Buffer::auto_grow::yes),
                m_initial_buffer_size(initial_buffer_size),
                m_max_buffer_size(max_buffer_size),
                m_callback(nullptr) {
            }

            /**
             * Construct a CallbackBuffer with a callback function.
             *
             * @param callback The callback function. Must be of type
             *                 @code void(osmium::memory::Buffer&&) @endcode
             * @param initial_buffer_size The initial size of newly created
             *                            internal buffers.
             * @param max_buffer_size If the buffer grows beyond this size the
             *                        callback will be called.
             */
            explicit CallbackBuffer(callback_func_type callback, std::size_t initial_buffer_size = default_initial_buffer_size, std::size_t max_buffer_size = default_max_buffer_size) :
                m_buffer(initial_buffer_size, osmium::memory::Buffer::auto_grow::yes),
                m_initial_buffer_size(initial_buffer_size),
                m_max_buffer_size(max_buffer_size),
                m_callback(std::move(callback)) {
            }

            /**
             * Access the internal buffer. This is used to fill the buffer,
             * the CallbackBuffer still owns the buffer.
             *
             * Use read() or the callback if you need to own the buffer.
             */
            osmium::memory::Buffer& buffer() noexcept {
                return m_buffer;
            }

            /**
             * Set the callback. The function must take a rvalue reference to
             * a buffer and return void.
             *
             * @param callback The callback function. Must be of type
             *                 @code void(osmium::memory::Buffer&&) @endcode
             */
            void set_callback(const callback_func_type& callback = nullptr) noexcept {
                m_callback = callback;
            }

            /**
             * Flush the internal buffer regardless of how full it is. Calls
             * the callback with the buffer and creates an new empty internal
             * one.
             *
             * This will do nothing if no callback is set or if the buffer
             * is empty.
             */
            void flush() {
                if (m_callback && m_buffer.committed() > 0) {
                    m_callback(read());
                }
            }

            /**
             * Flush the internal buffer if and only if it contains more than
             * the max_buffer_size set in the constructor. Calls the callback
             * with the buffer and creates an new empty internal one.
             *
             * This will do nothing if no callback is set or if the buffer
             * is empty.
             */
            void possibly_flush() {
                if (m_buffer.committed() > m_max_buffer_size) {
                    flush();
                }
            }

            /**
             * Return the internal buffer and create a new empty internal one.
             * You can use this as an alternative access instead of using the
             * callback.
             */
            osmium::memory::Buffer read() {
                osmium::memory::Buffer new_buffer{m_initial_buffer_size, osmium::memory::Buffer::auto_grow::yes};
                using std::swap;
                swap(new_buffer, m_buffer);
                return new_buffer;
            }

        }; // class CallbackBuffer

    } // namespace memory

} // namespace osmium

#endif // OSMIUM_MEMORY_CALLBACK_BUFFER_HPP
