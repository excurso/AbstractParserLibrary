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


#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT __attribute__((visibility("default")))
#  define DECL_IMPORT __attribute__((visibility("default")))
#endif

#ifdef ABSTRACTPARSER_LIBRARY
#  define ABSTRACTPARSER_EXPORT DECL_EXPORT
#else
#  define ABSTRACTPARSER_EXPORT DECL_IMPORT
#endif

#include "../tokenizer/AbstractTokenizer.h"
#include <stack>

namespace Abstract {
namespace Parsing {
using namespace std;
using namespace Abstract::Tokenization;
using namespace Abstract::Tokenization::Tokens;

class ABSTRACTPARSER_EXPORT AbstractParser
{
public:
    AbstractParser(AbstractParser &) = delete;
    AbstractParser(const AbstractParser &) = delete;
    AbstractParser(AbstractParser &&) = delete;
    AbstractParser(const AbstractParser &&) = delete;

    AbstractParser &operator=(AbstractParser &) = delete;
    AbstractParser &operator=(const AbstractParser &) = delete;
    AbstractParser &operator=(AbstractParser &&) = delete;
    AbstractParser &operator=(const AbstractParser &&) = delete;

    explicit
    AbstractParser(AbstractTokenStreamPtr token_stream);
    virtual ~AbstractParser() = default;

protected:
    inline const AbstractTokenStreamPtr
    tokenStream         () const;

    inline bool
    advance(const int64_t count = 1) const,

    parseError          () const;

    inline const AbstractTokenPtr
    prevToken           () const,
    currentToken        (const int64_t count = 0) const,
    nextToken           () const;

    inline const AbstractTokenStream::iterator
    getIterator         () const;

    inline void
    setIterator         (const AbstractTokenStream::iterator iterator),
    rememberPosition    (),
    resetPosition       (),
    popPosition         (),

    setParseError       ();

	void
    setErrorMessage     (const string &message);

    const string &
    errorMessage        () const;

    virtual void
    throwParseError     (const string &message) = 0;

private:
    const AbstractTokenStreamPtr m_token_stream;
    stack<AbstractTokenStream::iterator> m_position_stack;
    mutable AbstractTokenStream::iterator m_iterator;

    bool m_parse_error {false};
    string m_error_message;
};

inline const AbstractTokenStreamPtr
AbstractParser::
tokenStream() const
{
    return m_token_stream;
}

inline bool
AbstractParser::
advance(const int64_t count) const
{
    m_iterator += count;
    return true;
}

inline const AbstractTokenPtr
AbstractParser::
prevToken() const
{
    return *(m_iterator-1);
}

inline const AbstractTokenPtr
AbstractParser::
currentToken(int64_t count) const
{
    return *(m_iterator+count);
}

inline const AbstractTokenPtr
AbstractParser::
nextToken() const
{
    return *(m_iterator+1);
}

inline void
AbstractParser::
setIterator(const AbstractTokenStream::iterator iterator)
{
    m_iterator = iterator;
}

inline const AbstractTokenStream::iterator
AbstractParser::
getIterator() const
{
    return m_iterator;
}

inline void
AbstractParser::
rememberPosition()
{
    m_position_stack.push(m_iterator);
}

inline void
AbstractParser::
resetPosition()
{
    m_iterator = m_position_stack.top();
    m_position_stack.pop();
}

inline void
AbstractParser::
popPosition()
{
    m_position_stack.pop();
}

inline void
AbstractParser::
setParseError()
{
    m_parse_error = true;
}

inline bool
AbstractParser::
parseError() const
{
    return m_parse_error;
}

inline const string &
AbstractParser::
errorMessage() const
{
    return m_error_message;
}

} // namespace Parsing
} // namespace Abstract

#endif // ABSTRACTPARSER_H
