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
 * @file    IServer.hpp
 */

#pragma once

#include "Macros.hpp"
#include "ISocket.hpp"
#include "SockAddr.hpp"
#include "Exception.hpp"
#include "Timeval.hpp"

ELS_BEGIN_NAMESPACE_2(els, sock)

class IServer
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(ServerError, except::IOError);

    ELS_EXPORT_SYMBOL IServer(void);
    ELS_EXPORT_SYMBOL virtual ~IServer(void);

    ELS_EXPORT_SYMBOL virtual void listen(const SockAddr& addr) = 0;
    ELS_EXPORT_SYMBOL virtual bool hasPendingConnections(
            const misc::Timeval& tv = misc::Timeval(0, 0)) const;
    ELS_EXPORT_SYMBOL virtual ISocket* accept(void) const = 0;
    ELS_EXPORT_SYMBOL virtual void accept(ISocket& sock) const = 0;
    ELS_EXPORT_SYMBOL virtual void close(void);

private:

    int _M_sock;

    ELS_CLASS_UNCOPYABLE(IServer);
};

ELS_END_NAMESPACE_2



