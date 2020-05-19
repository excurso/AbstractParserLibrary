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


#ifndef ABSTRACTTOKEN_H
#define ABSTRACTTOKEN_H
#include <memory>
#include <string>

#ifdef EOF
#undef EOF
#endif

namespace Abstract {
namespace Tokenization {
namespace Tokens {

using namespace std;

class AbstractToken
{
public:
    AbstractToken(AbstractToken &) = delete;
    AbstractToken(const AbstractToken &) = delete;
    AbstractToken(AbstractToken &&) = delete;
    AbstractToken(const AbstractToken &&) = delete;

    AbstractToken &operator=(AbstractToken &) = delete;
    AbstractToken &operator=(const AbstractToken &) = delete;
    AbstractToken &operator=(AbstractToken &&) = delete;
    AbstractToken &operator=(const AbstractToken &&) = delete;

    explicit inline
    AbstractToken() = default;

	explicit inline
    AbstractToken(const char c),
    AbstractToken(const string &content);

    virtual
    ~AbstractToken() = default;

    inline void
	setContent(const string &content),
    setRow(const uint64_t row),
    setColumn(const uint64_t column);

    inline const string &
    content() const;

	inline const shared_ptr<string>
	contentPtr() const;

	inline bool
    hasContent(const char ch) const,
    hasContent(const initializer_list<const char> ch_list) const,
	hasContent(const string &content) const,
    hasContent(const initializer_list<string> content_list) const;

    inline uint64_t
    row() const, column() const;

private:
    uint64_t m_row {1}, m_column {1};
	shared_ptr<string> m_content;
};

inline
AbstractToken::AbstractToken(const char c) :
    m_content(make_shared<string>(string(1, c))) {}

inline
AbstractToken::AbstractToken(const string &content) :
    m_content(make_shared<string>(move(content))) {}

inline void
AbstractToken::
setContent(const string &content)
{
    !m_content && (m_content = make_shared<string>());
	m_content->reserve(content.length());
	*m_content = content;
}

inline const string &
AbstractToken::
content() const
{
	return *m_content;
}

inline const shared_ptr<string>
AbstractToken::
contentPtr() const
{
	return m_content;
}

inline bool
AbstractToken::
hasContent(const char ch) const
{
	return m_content->length() == 1 && m_content->front() == ch;
}

inline bool
AbstractToken::
hasContent(const initializer_list<const char> ch_list) const
{
	if (m_content->length() == 1)
        for (const auto ch : ch_list)
			if (m_content->front() == ch)
				return true;

    return false;
}

inline bool
AbstractToken::
hasContent(const string &content) const
{
	return *m_content == content;
}

inline bool
AbstractToken::
hasContent(const initializer_list<string> content_list) const
{
	for (const auto &content : content_list)
		if (*m_content == content) return true;

	return false;
}

inline void
AbstractToken::
setRow(const uint64_t row)
{
    m_row = row;
}

inline uint64_t
AbstractToken::
row() const
{
    return m_row;
}

inline void
AbstractToken::
setColumn(const uint64_t column)
{
    m_column = column;
}

inline uint64_t
AbstractToken::
column() const
{
    return m_column;
}

using AbstractTokenPtr = shared_ptr<AbstractToken>;

} // namespace Tokens
} // namespace Tokenization
} // namespace Abstract

#endif // ABSTRACTTOKEN_H
