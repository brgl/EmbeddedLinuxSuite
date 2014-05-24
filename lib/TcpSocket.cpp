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
 * @file    TcpSocket.cpp
 */

#include <els/TcpSocket.hpp>
#include <els/Exception.hpp>
#include <els/Utils.hpp>
#include "SockHelpers.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, sock)

TcpSocket::TcpSocket(void)
    : ISocket()
{

}

TcpSocket::~TcpSocket(void)
{

}

void TcpSocket::connect(const SockAddr& addr)
{
    int sock = -1;
    int retval = 0;
    ::sockaddr_in hostaddr;
    ::socklen_t socklen = sizeof(::sockaddr_in);

    ::memset(&hostaddr, 0, sizeof(::sockaddr_in));
    hostaddr.sin_family = AF_INET;
    hostaddr.sin_port = misc::utils::hostToNetShort(addr.port());
    if (addr.type() == SockAddr::TYPE_INET)
    {
        hostaddr.sin_addr.s_addr = misc::utils::hostToNetLong(addr.addr());
    }
    else if (addr.type() == SockAddr::TYPE_INET_NAME)
    {
        hostaddr.sin_addr.s_addr = misc::utils::hostToNetLong(
                ipFromStr(addr.name()));
    }
    else
    {
        throw except::InvalidArgument("Wrong address type");
    }

    sock = ::socket(AF_INET, SOCK_STREAM, 0);
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

const char* TcpSocket::getProtocol(void)
{
    return "TCP";
}

ELS_END_NAMESPACE_2

