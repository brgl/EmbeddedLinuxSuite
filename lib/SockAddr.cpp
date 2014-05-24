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
 * @file    SockAddr.cpp
 */

#include <els/SockAddr.hpp>
#include <els/System.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, sock);

const ElsSize SockAddr::UNIX_PATH_MAX = 108;
const ElsSize SockAddr::INET_HOSTNAME_MAX = 254;

SockAddr::SockAddr(void)
    : _M_addr(0)
{

}

SockAddr::SockAddr(const std::string& path)
    : _M_addr(_S_makeUnixAddr(path))
{

}

SockAddr::SockAddr(const std::string& hostname, ElsUint16 port)
    : _M_addr(_S_makeInetHostname(hostname, port))
{

}

SockAddr::SockAddr(ElsUint32 host, ElsUint16 port)
    : _M_addr(_S_makeInetAddr(host, port))
{

}

SockAddr::SockAddr(const SockAddr& other)
    : _M_addr(_S_copy(other._M_addr))
{

}

SockAddr& SockAddr::operator =(const SockAddr& other)
{
    sys::libcFree(this->_M_addr);
    this->_M_addr = _S_copy(other._M_addr);
    return *this;
}

SockAddr::~SockAddr(void)
{
    sys::libcFree(this->_M_addr);
}

SockAddr::SockAddrType SockAddr::type(void) const
{
    if (this->_M_addr == 0)
        return TYPE_NONE;
    return this->_M_addr->type;
}

ElsUint16 SockAddr::port(void) const
{
    ElsUint16 port = 0;

    this->_M_throwIfNull();
    if (this->_M_addr->type == TYPE_INET)
        port = reinterpret_cast<_T_SockAddrIn*>(this->_M_addr)->port;
    else if (this->_M_addr->type == TYPE_INET_NAME)
        port = reinterpret_cast<_T_SockAddrInName*>(this->_M_addr)->port;
    else
        _S_throwInvalidType();

    return port;
}

ElsUint32 SockAddr::addr(void) const
{
    ElsUint32 addr = 0;

    this->_M_throwIfNull();
    if (this->_M_addr->type == TYPE_INET)
        addr = reinterpret_cast<_T_SockAddrIn*>(this->_M_addr)->addr;
    else
        _S_throwInvalidType();

    return addr;
}

std::string SockAddr::name(void) const
{
    std::string name;

    this->_M_throwIfNull();
    if (this->_M_addr->type == TYPE_INET_NAME)
        name = std::string(reinterpret_cast<_T_SockAddrInName*>(
                this->_M_addr)->inAddr);
    else if (this->_M_addr->type == TYPE_UNIX)
        name = std::string(reinterpret_cast<_T_SockAddrUn*>(
                this->_M_addr)->unAddr);
    else
        _S_throwInvalidType();

    return name;
}

SockAddr::_T_SockAddr* SockAddr::_S_makeUnixAddr(const std::string& path)
{
    _T_SockAddrUn* addr = 0;

    if ((path.empty()) || (path.size() > UNIX_PATH_MAX))
    {
        throw except::InvalidArgument(
                "Invalid unix domain path: '%s'",
                path.c_str());
    }

    addr = reinterpret_cast<_T_SockAddrUn*>(
            sys::libcMalloc(sizeof(_T_SockAddrUn)));
    ::memset(addr, 0, sizeof(_T_SockAddrUn));
    addr->type = TYPE_UNIX;
    ::strncpy(addr->unAddr, path.c_str(), path.size());

    return reinterpret_cast<_T_SockAddr*>(addr);
}

SockAddr::_T_SockAddr*
SockAddr::_S_makeInetHostname(const std::string& hostname, ElsUint16 port)
{
    _T_SockAddrInName* addr = 0;

    if ((hostname.empty()) || (hostname.size() > INET_HOSTNAME_MAX))
    {
        throw except::InvalidArgument(
                "Invalid inet domain hostname: '%s'",
                hostname.c_str());
    }

    addr = reinterpret_cast<_T_SockAddrInName*>(
            sys::libcMalloc(sizeof(_T_SockAddrInName)));
    ::memset(addr, 0, sizeof(_T_SockAddrInName));
    addr->type = TYPE_INET_NAME;
    ::strncpy(addr->inAddr, hostname.c_str(), hostname.size());
    addr->port = port;

    return reinterpret_cast<_T_SockAddr*>(addr);
}

SockAddr::_T_SockAddr*
SockAddr::_S_makeInetAddr(ElsUint32 host, ElsUint16 port)
{
    _T_SockAddrIn* addr = 0;

    addr = reinterpret_cast<_T_SockAddrIn*>(
            sys::libcMalloc(sizeof(_T_SockAddrIn)));
    ::memset(addr, 0, sizeof(_T_SockAddrIn));
    addr->type = TYPE_INET;
    addr->addr = host;
    addr->port = port;

    return reinterpret_cast<_T_SockAddr*>(addr);
}

SockAddr::_T_SockAddr* SockAddr::_S_copy(const _T_SockAddr* other)
{
    ElsSize size = 0;
    _T_SockAddr* addr = 0;

    switch (other->type)
    {
    case TYPE_INET: size = sizeof(_T_SockAddrIn); break;
    case TYPE_INET_NAME: size = sizeof(_T_SockAddrInName); break;
    case TYPE_UNIX: size = sizeof(_T_SockAddrUn); break;
    default: return 0; break;
    }

    addr = reinterpret_cast<_T_SockAddr*>(sys::libcMalloc(size));
    ::memcpy(addr, other, size);

    return addr;
}

void SockAddr::_S_throwInvalidType(void)
{
    throw TypeError("SockAddr of invalid type");
}

void SockAddr::_M_throwIfNull(void) const
{
    if (this->_M_addr == 0)
        throw TypeError("SockAddr is NULL");
}

ELS_DEFINE_NESTED_EXCEPTION(TypeError, SockAddr, except::Exception);

ELS_END_NAMESPACE_2


