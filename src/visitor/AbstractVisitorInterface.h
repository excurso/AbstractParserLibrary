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


#ifndef ABSTRACTVISITORINTERFACE_H
#define ABSTRACTVISITORINTERFACE_H

namespace Abstract {
namespace Visitor {

template<class ...VisitableTypes>
class AbstractVisitorInterface;

template<class VisitableType>
class AbstractVisitorInterface<VisitableType> {
public:
    virtual void visit(const VisitableType &visitable) = 0;
};

template<class VisitableType, class ...VisitableTypes>
class AbstractVisitorInterface<VisitableType, VisitableTypes...> : public AbstractVisitorInterface<VisitableTypes...>
{
public:
    AbstractVisitorInterface() = default;
    AbstractVisitorInterface(AbstractVisitorInterface &) = delete;
    AbstractVisitorInterface(const AbstractVisitorInterface &) = delete;
    AbstractVisitorInterface(AbstractVisitorInterface &&) noexcept = default;
    virtual ~AbstractVisitorInterface() = default;

    AbstractVisitorInterface &operator=(AbstractVisitorInterface &) = delete;
    AbstractVisitorInterface &operator=(const AbstractVisitorInterface &) = delete;
    AbstractVisitorInterface &operator=(AbstractVisitorInterface &&) noexcept = default;

    using AbstractVisitorInterface<VisitableTypes...>::visit;
    virtual void visit(const VisitableType &visitable) = 0;
};

} // namespace Visitor
} // namespace Abstract

#endif // ABSTRACTVISITORINTERFACE_H
