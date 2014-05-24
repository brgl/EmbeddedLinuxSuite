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
 * @file    ISocket.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Exception.hpp"
#include "SockAddr.hpp"
#include "Types.hpp"
#include "ByteArray.hpp"
#include "Timeval.hpp"

ELS_BEGIN_NAMESPACE_2(els, sock)

class ISocket
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(SocketError, except::IOError);

    ELS_EXPORT_SYMBOL ISocket(void);
    ELS_EXPORT_SYMBOL virtual ~ISocket(void);

    ELS_EXPORT_SYMBOL virtual void connect(const SockAddr& addr) = 0;
    ELS_EXPORT_SYMBOL virtual void disconnect(void);
    ELS_EXPORT_SYMBOL virtual bool canSend(
            const misc::Timeval& tv = misc::Timeval(0, 0));
    ELS_EXPORT_SYMBOL virtual bool canRecv(
            const misc::Timeval& tv = misc::Timeval(0, 0));
    ELS_EXPORT_SYMBOL virtual ElsSize send(const void* buf, ElsSize size);
    ELS_EXPORT_SYMBOL virtual ElsSize send(const misc::ByteArray& buf);
    ELS_EXPORT_SYMBOL virtual ElsSize recv(void* buf, ElsSize size);
    ELS_EXPORT_SYMBOL virtual ElsSize recv(misc::ByteArray& buf);

    ELS_EXPORT_SYMBOL virtual const char* getProtocol(void) = 0;
    ELS_EXPORT_SYMBOL virtual int getfd(void) const;
    ELS_EXPORT_SYMBOL virtual void setfd(int fd);

private:

    int _M_sock;

    ELS_CLASS_UNCOPYABLE(ISocket);
};

ELS_END_NAMESPACE_2


