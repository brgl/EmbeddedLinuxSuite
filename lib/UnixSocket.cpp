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
 * @file    UnixSocket.cpp
 */

#include <els/UnixSocket.hpp>

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, sock)

UnixSocket::UnixSocket(void)
    : ISocket()
{

}

UnixSocket::~UnixSocket(void)
{

}

void UnixSocket::connect(const SockAddr& addr)
{
    int sock = -1;
    int retval = 0;
    ::sockaddr_un hostaddr;
    ::socklen_t socklen = sizeof(::sockaddr_un);

    if (addr.type() != SockAddr::TYPE_UNIX)
    {
        throw except::InvalidArgument(
                "Invalid address type - expected UNIX");
    }

    memset(&hostaddr, 0, sizeof(::sockaddr_un));
    hostaddr.sun_family = AF_UNIX;
    ::strncpy(hostaddr.sun_path,
            addr.name().c_str(), SockAddr::UNIX_PATH_MAX);

    sock = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {
        throw SocketError(
                "Error creating socket: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }

    retval = ::connect(sock, reinterpret_cast<sockaddr*>(&hostaddr), socklen);
    if (retval < 0)
    {
        throw SocketError(
                "Error connecting to host: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }

    this->setfd(sock);
}

const char* UnixSocket::getProtocol(void)
{
    return "UNIX";
}

ELS_END_NAMESPACE_2


