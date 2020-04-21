/******************************************************************************
AbstractParserLibrary - A C++ parser library which can be used as base
                        for specific parsers

Copyright (C) 2019-2020 Waldemar Zimpel <hspp@utilizer.de>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/


#include "AbstractParser.h"

using namespace Abstract::Parsing;

AbstractParser::AbstractParser(AbstractTokenStreamPtr token_stream) :
    m_token_stream(move(token_stream)),
    m_iterator(m_token_stream->begin()) {}

void
AbstractParser::
setErrorMessage(const string &message)
{
    m_error_message = message;
}
