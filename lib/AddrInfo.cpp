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
 * @file    AddrInfo.cpp
 */

#include <els/AddrInfo.hpp>
#include <els/Utils.hpp>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>

ELS_BEGIN_NAMESPACE_2(els, sock)

AddrInfo::AddrInfo(const std::string& hostname)
    : _M_addrList()
{
    ::addrinfo* ainfo = 0;
    ::addrinfo hints;
    int retval = 0;
    int i = 0;

    ::memset(&hints, 0, sizeof(::addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    retval = ::getaddrinfo(hostname.c_str(), 0, &hints, &ainfo);
    if (retval != 0)
    {
        throw AddrInfoError(
                "Error resolving name '%s': %s",
                hostname.c_str(),
                ::gai_strerror(retval));
    }

    try
    {
        for (::addrinfo* ai = ainfo; ai != 0; ai = ai->ai_next, ++i)
        {
            this->_M_addrList.push_back(misc::utils::netToHostLong(
                    reinterpret_cast< ::sockaddr_in*>(
                            ai->ai_addr)->sin_addr.s_addr));
        }
    }
    catch (...)
    {
        ::freeaddrinfo(ainfo);
        throw;
    }

    ::freeaddrinfo(ainfo);
}

AddrInfo::AddrInfo(const AddrInfo& other)
    : _M_addrList(other._M_addrList)
{

}

AddrInfo& AddrInfo::operator =(const AddrInfo& other)
{
    this->_M_addrList = other._M_addrList;
    return *this;
}

AddrInfo::~AddrInfo(void)
{

}

AddrInfo::HostAddr AddrInfo::first(void) const
{
    if (this->_M_addrList.empty())
        except::throwLogicError("Empty address list");
    return this->_M_addrList.front();
}

AddrInfo::const_iterator AddrInfo::begin(void) const
{
    return this->_M_addrList.begin();
}

AddrInfo::const_iterator AddrInfo::end(void) const
{
    return this->_M_addrList.end();
}

AddrInfo::const_reverse_iterator AddrInfo::rbegin(void) const
{
    return this->_M_addrList.rbegin();
}

AddrInfo::const_reverse_iterator AddrInfo::rend(void) const
{
    return this->_M_addrList.rend();
}

ELS_DEFINE_NESTED_EXCEPTION(AddrInfoError, AddrInfo, except::Exception);

ELS_END_NAMESPACE_2


