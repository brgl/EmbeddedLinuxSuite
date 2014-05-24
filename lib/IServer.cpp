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
 * @file    IServer.cpp
 */

#include <els/IServer.hpp>
#include "SockHelpers.hpp"

#include <unistd.h>

ELS_BEGIN_NAMESPACE_2(els, sock)

IServer::IServer(void)
    : _M_sock(-1)
{

}

IServer::~IServer(void)
{

}

bool IServer::hasPendingConnections(const misc::Timeval& tv) const
{
    return sockCanRead(this->_M_sock, tv);
}

void IServer::close(void)
{
    int retval = 0;

    retval = ::close(this->_M_sock);
    if (retval < 0)
    {
        throw ServerError("Error on disconnect: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }
    this->_M_sock = -1;
}

ELS_DEFINE_NESTED_EXCEPTION(ServerError, IServer, except::IOError);

ELS_END_NAMESPACE_2


