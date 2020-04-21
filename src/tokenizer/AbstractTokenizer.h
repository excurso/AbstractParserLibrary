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


#ifndef GENERALTOKENIZER_H
#define GENERALTOKENIZER_H
#include "../../../StringLibrary/src/String.h"
#include "elements/AbstractToken.h"
#include <memory>

namespace Abstract {
namespace Tokenization {
using namespace Abstract::Tokenization::Tokens;
using AbstractTokenStream    = DataContainer<GeneralTokenPtr>;
using AbstractTokenStreamPtr = shared_ptr<AbstractTokenStream>;

class AbstractTokenizer
{
public:
    AbstractTokenizer(AbstractTokenizer &) = delete;
    AbstractTokenizer(const AbstractTokenizer &) = delete;
    AbstractTokenizer(AbstractTokenizer &&) = delete;
    AbstractTokenizer(const AbstractTokenizer &&) = delete;

    explicit
    AbstractTokenizer(shared_ptr<string> content),
    AbstractTokenizer(const string &content, const uint64_t begin_row = 1, const uint64_t begin_column = 1);

    ~AbstractTokenizer() = default;

    AbstractTokenizer &operator=(AbstractTokenizer &) = delete;
    AbstractTokenizer &operator=(const AbstractTokenizer &) = delete;
    AbstractTokenizer &operator=(AbstractTokenizer &&) = delete;
    AbstractTokenizer &operator=(const AbstractTokenizer &&) = delete;

protected:
    enum Encoding : uint8_t { UNSUPPORTED, UTF8, ISO8859, WINDOWS125X };

    inline void
    setEncoding             (Encoding encoding);

    inline Encoding
    encoding                ();

    inline bool
    isOneOfChars            (const string &allowed) const,
    isSpaceChar             () const noexcept,

    encoding                (const initializer_list<Encoding> candidates) const;

    void
    skipSpace               () const noexcept;

    inline bool
    isLineTerminator        () const,
    isTab                   () const,
    isUtf8MultibyteChar     () const,

    syntaxError             () const;

    bool
    advance                 (int64_t count = 1) const,

    isComment               (const string &comment_start_identifier,
                             const string &comment_end_identifier,
                             string &comment) const,
    isString                (string &str) const,
    isTerm                  (string *str = nullptr) const,

    isCharOfRange           (char from_char, char to_char) const,

    posStartsWith           (const string &s,
                             const bool case_insensitive = false) const,
    posStartsWith           (const DataContainer<string> &string_list,
                             const bool case_insensitive = false) const,
    isOneOfChars            (const string::iterator iter, const string &allowed) const;

    string
    readCharSequence        (const string &not_allowed_chars) const;

    inline void
    appendToken             (const GeneralTokenPtr &token),
    appendToken             (const GeneralTokenPtr &token, const uint64_t row, const uint64_t column),
    setIterator             (const string::iterator iterator) const,

    setSyntaxError          ();

    inline char
    currentChar             () const,
    nextChar                () const,
    prevChar                () const;

	inline const string::iterator
    getIterator             (int64_t count = 0) const;

    inline bool
    currentChar             (const char c) const,
    currentChar             (const initializer_list<char> chars) const,
    nextChar                (const char c) const,
    nextChar                (const initializer_list<char> chars) const,
    prevChar                (const char c) const,
    isEof                   (const uint64_t count = 0) const;

    inline uint64_t
    currentRow              () const,
    currentColumn           () const;

    inline const shared_ptr<string>
    byteStream              () const;

    inline const AbstractTokenStreamPtr
    tokenStream             () const;

    inline void
    setTabWidth             (const uint8_t tab_width);

    inline uint8_t
    getTabWidth             ();

    void
    throwSyntaxError        (const string &message = "");

    const string &
    errorMessage            ();

private:
    AbstractTokenStreamPtr      m_token_stream;
	shared_ptr<string>			m_content;

    mutable uint64_t			m_row, m_column;
    mutable string::iterator	m_iterator, m_row_begin;

    Encoding m_encoding { UTF8 };
    uint8_t m_tab_width = 4;

    bool m_syntax_error {false};
    string m_error_message;
};

inline void
AbstractTokenizer::
setEncoding(Encoding encoding)
{
    m_encoding = encoding;
}

inline auto
AbstractTokenizer::
encoding() -> Encoding
{
    return m_encoding;
}

inline bool
AbstractTokenizer::
encoding(const initializer_list<Encoding> candidates) const
{
    for (const auto &candidate : candidates)
        if (candidate == m_encoding)
            return true;

    return false;
}


inline void
AbstractTokenizer::
setTabWidth(const uint8_t tab_width)
{
    m_tab_width = tab_width;
}

inline uint8_t
AbstractTokenizer::
getTabWidth()
{
    return m_tab_width;
}

inline void
AbstractTokenizer::
appendToken(const GeneralTokenPtr &token)
{
    token->setRow(m_row);
    token->setColumn(m_column - (token->contentPtr() ? token->content().length() + 1 : 0));
    tokenStream()->emplace_back(token);
}

inline void
AbstractTokenizer::
appendToken(const GeneralTokenPtr &token, const uint64_t row, const uint64_t column)
{
    token->setRow(row);
    token->setColumn(column);
    tokenStream()->emplace_back(token);
}

inline bool
AbstractTokenizer::
isUtf8MultibyteChar() const
{
    uint8_t char_count = 0;

    if ((currentChar() & 0x80) == 0x80) {
        // 4 byte char
        if ((currentChar() & 0xf0) == 0xf0) {
            char_count = 4;
        }
        // 3 byte char
        else if ((currentChar() & 0xe0) == 0xe0 && (currentChar() & 0x10) != 0x10) {
            char_count = 3;
        }
        // 2 byte char
        else if ((currentChar() & 0xc0) == 0xc0 && (currentChar() & 0x20) != 0x20) {
            char_count = 2;
        }
    }

    if (bool(char_count)) {
        const auto begin = getIterator();
        const auto end = begin + char_count;

        while (!isEof() && getIterator() != end) {
            if (currentChar() < 0)
                ++m_iterator;
            else {
                m_iterator = begin;
                return false;
            }
        }

        ++m_column;
        return true;
    }

    return false;
}

inline char
AbstractTokenizer::
currentChar() const
{
    return *getIterator();
}

inline char
AbstractTokenizer::
nextChar() const
{
    return *getIterator(+1);
}

inline char
AbstractTokenizer::
prevChar() const
{
    return *getIterator(-1);
}

inline bool
AbstractTokenizer::
currentChar(const char c) const
{
    return *getIterator() == c;
}

inline bool
AbstractTokenizer::
currentChar(const initializer_list<char> chars) const
{
    for (const auto &c : chars)
        if (currentChar(c)) return true;

    return false;
}

inline bool
AbstractTokenizer::
nextChar(const char c) const
{
    return *getIterator(+1) == c;
}

inline bool
AbstractTokenizer::
nextChar(const initializer_list<char> chars) const
{
    for (const auto &c : chars)
        if (nextChar(c)) return true;

    return false;
}

inline bool
AbstractTokenizer::
prevChar(const char c) const
{
    return *getIterator(-1) == c;
}

inline void
AbstractTokenizer::
setIterator(const string::iterator iterator) const
{
    m_iterator = iterator;
}

inline const string::iterator
AbstractTokenizer::
getIterator(int64_t count) const
{
    return m_iterator+count;
}

inline bool
AbstractTokenizer::
isOneOfChars(const string &allowed) const
{
    return std::find(allowed.begin(), allowed.end(), currentChar()) != allowed.end();
}

inline bool
AbstractTokenizer::
isSpaceChar() const noexcept
{
    return bool(isspace(currentChar()));
}

inline bool
AbstractTokenizer::
isLineTerminator() const
{
    if (currentChar('\n')) {
        ++m_row; m_column = 1;
        return true;
    }
    return false;
}

inline bool
AbstractTokenizer::
isTab() const
{
    return currentChar('\t') && bool(m_column += m_tab_width);
}

inline bool
AbstractTokenizer::
isEof(const uint64_t count) const
{
    return getIterator(int64_t(count)) == byteStream()->end();
}

inline uint64_t
AbstractTokenizer::
currentRow() const
{
    return m_row;
}

inline uint64_t
AbstractTokenizer::
currentColumn() const
{
    return m_column;
}

inline const shared_ptr<string>
AbstractTokenizer::
byteStream() const
{
    return m_content;
}

inline const AbstractTokenStreamPtr
AbstractTokenizer::
tokenStream() const
{
    return m_token_stream;
}

inline void
AbstractTokenizer::
setSyntaxError()
{
    m_syntax_error = true;
}

inline bool
AbstractTokenizer::
syntaxError() const
{
    return m_syntax_error;
}

inline const string &
AbstractTokenizer::
errorMessage()
{
    return m_error_message;
}

} // namespace Tokenization
} // namespace Abstract

#endif // GENERALTOKENIZER_H
