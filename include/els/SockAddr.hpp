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
 * @file    SockAddr.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "Exception.hpp"

#include <string>

#define __ELS_UNIX_PATH_MAX 108
#define __ELS_INET_HOSTNAME_MAX 255

ELS_BEGIN_NAMESPACE_2(els, sock)

/**
 * @brief   Class wrapping a socket address. It can store both INET
 *          and UNIX domain socket addresses.
 *
 * Currently SockAddr does not support INET6 family.
 */
class SockAddr
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(TypeError, except::Exception);

    enum SockAddrType
    {
        TYPE_NONE = 0,
        TYPE_INET,
        TYPE_INET_NAME,
        TYPE_UNIX
    };

    static const ElsSize UNIX_PATH_MAX;
    static const ElsSize INET_HOSTNAME_MAX;

    ELS_EXPORT_SYMBOL SockAddr(void);
    ELS_EXPORT_SYMBOL explicit SockAddr(const std::string& path);
    ELS_EXPORT_SYMBOL SockAddr(const std::string& hostname, ElsUint16 port);
    ELS_EXPORT_SYMBOL SockAddr(ElsUint32 host, ElsUint16 port);
    ELS_EXPORT_SYMBOL SockAddr(const SockAddr& other);
    ELS_EXPORT_SYMBOL SockAddr& operator =(const SockAddr& other);
    ELS_EXPORT_SYMBOL ~SockAddr(void);

    ELS_EXPORT_SYMBOL SockAddrType type(void) const;
    ELS_EXPORT_SYMBOL ElsUint16 port(void) const;
    ELS_EXPORT_SYMBOL ElsUint32 addr(void) const;
    ELS_EXPORT_SYMBOL std::string name(void) const;

private:

    struct _T_SockAddr
    {
        SockAddrType type;
        char zero[8-sizeof(SockAddrType)];
    };

    struct _T_SockAddrUn
    {
        SockAddrType type;
        char unAddr[__ELS_UNIX_PATH_MAX];
    };

    struct _T_SockAddrIn
    {
        SockAddrType type;
        ElsUint32 addr;
        ElsUint16 port;
    };

    struct _T_SockAddrInName
    {
        SockAddrType type;
        char inAddr[__ELS_INET_HOSTNAME_MAX];
        ElsUint16 port;
    };

    static _T_SockAddr* _S_makeUnixAddr(const std::string& path);
    static _T_SockAddr* _S_makeInetHostname(const std::string& hostname,
            ElsUint16 port);
    static _T_SockAddr* _S_makeInetAddr(ElsUint32 host, ElsUint16 port);
    static _T_SockAddr* _S_copy(const _T_SockAddr* other);
    static void _S_throwInvalidType(void);

    inline void _M_throwIfNull(void) const;

    _T_SockAddr* _M_addr;
};

ELS_END_NAMESPACE_2


