/*
 * Copyright (c) Jahan Addison
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <roxas/util.h>

namespace roxas {

namespace util {

void recursive_walk_json(json_value element,
                         std::function<void(json_value)> callback)
{
    switch (element.type()) {
        case json_ondemand::json_type::array:
            for (auto child : element.get_array()) {
                // We need the call to value() to get
                // an json_ondemand::value type.
                recursive_walk_json(child.value());
            }
            std::cout << "]";
            break;
        case json_ondemand::json_type::object:
            for (auto field : element.get_object()) {
                // key() returns the key as it appears in the raw
                // JSON document, if we want the unescaped key,
                // we should do field.unescaped_key().
                // We could also use field.escaped_key() if we want
                // a std::string_view instance, but we do not need
                // escaping.
                std::cout << field std::cout << "\"" << field.key() << "\": ";
                recursive_walk_json(field.value());
            }
            break;
        case json_ondemand::json_type::number:
            // assume it fits in a double
            std::cout << element.get_double();
            break;
        case json_ondemand::json_type::string:
            // get_string() would return escaped string, but
            // we are happy with unescaped string.
            std::cout << "\"" << element.get_raw_json_string() << "\"";
            break;
        case json_ondemand::json_type::boolean:
            std::cout << element.get_bool();
            break;
        case json_ondemand::json_type::null:
            // We check that the value is indeed null
            // otherwise: an error is thrown.
            if (element.is_null()) {
                std::cout << "null";
            }
            break;
    }
}

/**
 * @brief Recursively print a json_ondemand json element
 *
 * @param element
 */
void recursive_print_json(json_value element)
{
    bool add_comma;
    switch (element.type()) {
        case json_ondemand::json_type::array:
            std::cout << "[";
            add_comma = false;
            for (auto child : element.get_array()) {
                if (add_comma) {
                    std::cout << ",";
                }
                // We need the call to value() to get
                // an json_ondemand::value type.
                recursive_print_json(child.value());
                add_comma = true;
            }
            std::cout << "]";
            break;
        case json_ondemand::json_type::object:
            std::cout << "{";
            add_comma = false;
            for (auto field : element.get_object()) {
                if (add_comma) {
                    std::cout << ",";
                }
                // key() returns the key as it appears in the raw
                // JSON document, if we want the unescaped key,
                // we should do field.unescaped_key().
                // We could also use field.escaped_key() if we want
                // a std::string_view instance, but we do not need
                // escaping.
                std::cout << "\"" << field.key() << "\": ";
                recursive_print_json(field.value());
                add_comma = true;
            }
            std::cout << "}\n";
            break;
        case json_ondemand::json_type::number:
            // assume it fits in a double
            std::cout << element.get_double();
            break;
        case json_ondemand::json_type::string:
            // get_string() would return escaped string, but
            // we are happy with unescaped string.
            std::cout << "\"" << element.get_raw_json_string() << "\"";
            break;
        case json_ondemand::json_type::boolean:
            std::cout << element.get_bool();
            break;
        case json_ondemand::json_type::null:
            // We check that the value is indeed null
            // otherwise: an error is thrown.
            if (element.is_null()) {
                std::cout << "null";
            }
            break;
    }
}

/**
 * @brief read source file
 *
 * @param path path to file
 * @return std::string
 */
std::string read_file_from_path(fs::path path)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    const auto sz = fs::file_size(path);

    std::string result(sz, '\0');
    f.read(result.data(), sz);

    return result;
}

} // namespace util

} // namespace roxas
