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

#include <array>
#include <map>
#include <roxas/util.h>
#include <string>

namespace roxas {

template<typename T>
class Symbol_Table
{
    /**
     * @brief Symbol table template class
     *
     * Constructs a symbol table from a template data structure
     *
     * An example table may be a map to `std::array<std::string, 5>':
     *
     * Name
     *     \
     *     |
     *   ------------------------------------------------------
     *   | Type | Size | Line Declare | Line Usage |  Address |
     *   ------------------------------------------------------
     * ...
     * ...
     */
  public:
    Symbol_Table& operator=(Symbol_Table const&) = delete;
    Symbol_Table(Symbol_Table const&) = delete;

    /**
     * @brief Construct a new Symbol_Table object
     *
     */
    Symbol_Table() = default;

  public:
    using Table_Entry = std::string_view;
    /**
     * @brief Get a symbol by name in the symbol table
     *
     * @param name
     * @return symbol_data
     */
    T get_symbol_by_name(std::string_view name);
    /**
     * @brief Get a symbol by name in the symbol table
     *
     * @param name
     * @return symbol_data
     */
    T set_symbol_by_name(std::string_view name, T entry);

  private:
    std::map<Table_Entry, T> table_;
};

} // namespace roxas
