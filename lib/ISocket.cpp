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
 * @file    ISocket.cpp
 */

#include <els/ISocket.hpp>
#include "SockHelpers.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

ELS_BEGIN_NAMESPACE_2(els, sock)

ISocket::ISocket(void)
    : _M_sock(-1)
{

}

ISocket::~ISocket(void)
{
    this->disconnect();
}

void ISocket::disconnect(void)
{
    int retval = 0;

    retval = ::close(this->_M_sock);
    if (retval < 0)
    {
        throw SocketError("Error on disconnect: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }
    this->_M_sock = -1;
}

bool ISocket::canSend(const misc::Timeval& tv)
{
    return sockCanWrite(this->_M_sock, tv);
}

bool ISocket::canRecv(const misc::Timeval& tv)
{
    return sockCanRead(this->_M_sock, tv);
}

ElsSize ISocket::send(const void* buf, ElsSize size)
{
    ssize_t retval = 0;

    retval = ::send(this->_M_sock, buf, size, 0);
    if (retval < 0)
    {
        throw SocketError("Error on send: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }

    return static_cast<ElsSize>(retval);
}

ElsSize ISocket::send(const misc::ByteArray& buf)
{
    return this->send(buf.get(), buf.size());
}

ElsSize ISocket::recv(void* buf, ElsSize size)
{
    ssize_t retval = 0;

    retval = ::recv(this->_M_sock, buf, size, 0);
    if (retval < 0)
    {
        throw SocketError("Error on recv: %s",
                except::getErrnoStr(
                        except::getErrno()).c_str());
    }

    return static_cast<ElsSize>(retval);
}

ElsSize ISocket::recv(misc::ByteArray& buf)
{
    return this->recv(buf.get(), buf.size());
}

int ISocket::getfd(void) const
{
    return this->_M_sock;
}

void ISocket::setfd(int fd)
{
    if (this->_M_sock != -1)
        except::throwLogicError("Socket already set");
    this->_M_sock = fd;
}

ELS_DEFINE_NESTED_EXCEPTION(SocketError, ISocket, except::IOError);

ELS_END_NAMESPACE_2


