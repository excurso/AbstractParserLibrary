###############################################################################
# AbstractParserLibrary - A C++ parser library which can be used as base
#                         for specific parsers
#
# Copyright (C) 2019-2020 Waldemar Zimpel <hspp@utilizer.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
###############################################################################


cmake_minimum_required(VERSION 3.5)

project(AbstractParser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if (NOT TARGET String)
	add_subdirectory(../StringLibrary/ StringLibrary/)
endif()

add_library(AbstractParser
	src/visitor/AbstractVisitorInterface.h
	src/tokenizer/elements/AbstractToken.h
	src/tokenizer/elements/AbstractToken.cpp
	src/tokenizer/AbstractTokenizer.h
	src/tokenizer/AbstractTokenizer.cpp
	src/parser/AbstractParser.cpp
	src/parser/AbstractParser.h
)

target_link_libraries(AbstractParser LINK_PUBLIC String)
target_compile_definitions(AbstractParser PRIVATE ABSTRACTPARSER_LIBRARY)
