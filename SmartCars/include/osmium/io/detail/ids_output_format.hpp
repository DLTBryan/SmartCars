#ifndef OSMIUM_IO_DETAIL_IDS_OUTPUT_FORMAT_HPP
#define OSMIUM_IO_DETAIL_IDS_OUTPUT_FORMAT_HPP

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

#include <osmium/io/detail/output_format.hpp>
#include <osmium/io/detail/queue_util.hpp>
#include <osmium/io/detail/string_util.hpp>
#include <osmium/io/file.hpp>
#include <osmium/io/file_format.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/memory/collection.hpp>
#include <osmium/memory/item_iterator.hpp>
#include <osmium/osm/box.hpp>
#include <osmium/osm/changeset.hpp>
#include <osmium/osm/item_type.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/osm/metadata_options.hpp>
#include <osmium/osm/node.hpp>
#include <osmium/osm/node_ref.hpp>
#include <osmium/osm/object.hpp>
#include <osmium/osm/relation.hpp>
#include <osmium/osm/tag.hpp>
#include <osmium/osm/timestamp.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/thread/pool.hpp>
#include <osmium/visitor.hpp>

#include <cstdint>
#include <iterator>
#include <memory>
#include <string>
#include <utility>

namespace osmium {

    namespace io {

        namespace detail {

            struct ids_output_options {

                bool with_type = true;

            }; // struct ids_output_options

            /**
             * Writes out one buffer with OSM data in IDS format.
             */
            class IDSOutputBlock : public OutputBlock {

                ids_output_options m_options;

                void write_id(char c, int64_t value) {
                    if (m_options.with_type) {
                        *m_out += c;
                    }
                    output_int(value);
                    *m_out += '\n';
                }

            public:

                IDSOutputBlock(osmium::memory::Buffer&& buffer, const ids_output_options& options) :
                    OutputBlock(std::move(buffer)),
                    m_options(options) {
                }

                std::string operator()() {
                    osmium::apply(m_input_buffer->cbegin(), m_input_buffer->cend(), *this);

                    std::string out;
                    using std::swap;
                    swap(out, *m_out);

                    return out;
                }

                void node(const osmium::Node& node) {
                    write_id('n', node.id());
                }

                void way(const osmium::Way& way) {
                    write_id('w', way.id());
                }

                void relation(const osmium::Relation& relation) {
                    write_id('r', relation.id());
                }

                void changeset(const osmium::Changeset& changeset) {
                    write_id('c', changeset.id());
                }

            }; // class IDSOutputBlock

            class IDSOutputFormat : public osmium::io::detail::OutputFormat {

                ids_output_options m_options;

            public:

                IDSOutputFormat(osmium::thread::Pool& pool, const osmium::io::File& file, future_string_queue_type& output_queue) :
                    OutputFormat(pool, output_queue) {
                    if (file.is_false("ids_with_type")) {
                        m_options.with_type = false;
                    }
                }

                void write_buffer(osmium::memory::Buffer&& buffer) final {
                    m_output_queue.push(m_pool.submit(IDSOutputBlock{std::move(buffer), m_options}));
                }

            }; // class IDSOutputFormat

            // we want the register_output_format() function to run, setting
            // the variable is only a side-effect, it will never be used
            const bool registered_ids_output = osmium::io::detail::OutputFormatFactory::instance().register_output_format(osmium::io::file_format::ids,
                [](osmium::thread::Pool& pool, const osmium::io::File& file, future_string_queue_type& output_queue) {
                    return new osmium::io::detail::IDSOutputFormat(pool, file, output_queue);
            });

            // dummy function to silence the unused variable warning from above
            inline bool get_registered_ids_output() noexcept {
                return registered_ids_output;
            }

        } // namespace detail

    } // namespace io

} // namespace osmium

#endif // OSMIUM_IO_DETAIL_IDS_OUTPUT_FORMAT_HPP
