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

/**
 * @brief Recursively walk and run a callback on elements on the AST JSON
 *
 * The ast class (from python):
 *
 * class AST_Node(TypedDict):
 *      """The AST data structure"""
 *      node: Node_Type
 *      root: Node_Root
 *      left: NotRequired[Node]
 *      right: NotRequired[Node]
 *      _meta: NotRequired[_Meta]
 *
 * @param element simdjson::ondemand::element The AST root
 * @param callback the function to call on leafs
 */
void recursive_walk_json(
    json_value element,
    std::function<void(Leaf_Node, std::stack<Node_Type>)> callback,
    std::stack<Node_Type> call_stack)
{

    Node_Type root_data{};
    Node_Type node_data{};
    Leaf_Node left_node{};
    Leaf_Node right_node{};

    auto root = element["root"];
    auto node = element["node"];
    auto left = element["left"];
    auto right = element["right"];

    // auto meta = element["_meta"];

    switch (node.type()) {
        case json_ondemand::json_type::array:
            node_data = root.get_array();
            break;
        case json_ondemand::json_type::string:
            node_data = root.get_string();
            break;
        default:
            throw std::runtime_error("node element invalid type");
    }

    switch (root.type()) {
        case json_ondemand::json_type::array:
            root_data = root.get_array();
            break;
        case json_ondemand::json_type::string:
            root_data = root.get_string();
            break;
        default:
            throw std::runtime_error("root element invalid type");
    }

    call_stack.push(node_data);

    switch (left.type()) {
        case json_ondemand::json_type::array:
            for (auto child : left.get_array()) {
                recursive_walk_json(child.value(), callback, call_stack);
            }
            break;
        case json_ondemand::json_type::object:
            left_node = root.get_object();
            callback(left_node, call_stack);
            break;
        case json_ondemand::json_type::null:
            break;
        default:
            throw std::runtime_error("left element invalid type");
    }

    switch (right.type()) {
        case json_ondemand::json_type::array:
            for (auto child : right.get_array()) {
                recursive_walk_json(child.value(), callback, call_stack);
            }
        case json_ondemand::json_type::object:
            right_node = right.get_object();
            callback(right_node, call_stack);
            break;
        case json_ondemand::json_type::null:
            break;
        default:
            throw std::runtime_error("right element invalid type");
    }

    call_stack.pop();
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
