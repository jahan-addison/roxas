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

#include <cstdlib>
#include <iostream>
#include <roxas/parse_tree.h>

int main(int argc, char* argv[])
{
    if (argc < 4) {
        if (argc < 2) {
            std::cout << "Roxas :: Axel... What's this?" << std::endl;
            return EXIT_SUCCESS;
        }
        std::cerr << "Invalid arguments :: please provide the frontend python "
                     "module name, path, and B source program to compile"
                  << std::endl;
        return EXIT_FAILURE;
    }
    try {
        auto parse_tree = roxas::ParseTreeLoader(argv[2], argv[3]);
        parse_tree.get_parse_tree_as_string_from_module(argv[1]);
    } catch (std::runtime_error& e) {
        std::cerr << "Runtime Exception :: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
