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

#pragma once
#include <filesystem>
#include <functional>
#include <simdjson.h>
#include <stack>
#include <string_view>
#include <variant>

namespace roxas {

namespace util {

// The overload pattern
template<class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

namespace fs = std::filesystem;

namespace json = simdjson;

namespace json_ondemand = simdjson::ondemand;

using json_value = json_ondemand::value;

using Leaf_Node =
    std::variant<json_ondemand::array, json_ondemand::object, std::monostate>;
using Node_Type = std::variant<json_ondemand::array, std::string_view>;

/**
 * @brief Recursively walk abd call `callback` on a simdjson::ondemand json
 * element
 *
 * @param element
 */
void recursive_walk_json(
    json_value element,
    std::function<void(Leaf_Node, std::stack<Node_Type>)> callback,
    std::stack<Node_Type> call_stack);

/**
 * @brief Recursively print a simdjson::ondemand json element
 *
 * @param element
 */
void recursive_print_json(json_value element);

/**
 * @brief read a file from a fs::path
 *
 * @param path path to file
 * @return std::string
 */
std::string read_file_from_path(fs::path path);

} // namespace util

} // namespace roxas