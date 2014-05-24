/*
 * Copyright (C) 2013 Bartosz Golaszewski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @file    AddrInfo.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "Exception.hpp"

#include <string>
#include <list>

ELS_BEGIN_NAMESPACE_2(els, sock)

class AddrInfo
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(AddrInfoError, except::Exception);

    typedef ElsUint32 HostAddr;

private:

    typedef std::list<HostAddr> _T_AddrList;

public:

    typedef _T_AddrList::const_iterator const_iterator;
    typedef _T_AddrList::const_reverse_iterator const_reverse_iterator;

    ELS_EXPORT_SYMBOL explicit AddrInfo(const std::string& hostname);
    ELS_EXPORT_SYMBOL AddrInfo(const AddrInfo& other);
    ELS_EXPORT_SYMBOL AddrInfo& operator =(const AddrInfo& other);
    ELS_EXPORT_SYMBOL ~AddrInfo(void);

    ELS_EXPORT_SYMBOL HostAddr first(void) const;

    ELS_EXPORT_SYMBOL const_iterator begin(void) const;
    ELS_EXPORT_SYMBOL const_iterator end(void) const;
    ELS_EXPORT_SYMBOL const_reverse_iterator rbegin(void) const;
    ELS_EXPORT_SYMBOL const_reverse_iterator rend(void) const;

private:

    _T_AddrList _M_addrList;
};

ELS_END_NAMESPACE_2


