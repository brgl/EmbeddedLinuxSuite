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
 * @file    SockHelpers.cpp
 */

#include "SockHelpers.hpp"
#include <els/Exception.hpp>

#include <arpa/inet.h>
#include <cstring>
#include <ctime>
#include <sys/select.h>
#include <cerrno>

ELS_BEGIN_NAMESPACE_2(els, sock)

ElsUint32 ipFromStr(const std::string& addr)
{
    int retval = 0;
    ::in_addr dst;

    ::memset(&dst, 0, sizeof(::in_addr));
    retval = ::inet_pton(AF_INET, addr.c_str(), &dst);
    if (retval == 0)
        throw except::InvalidArgument(
                "Address does not represent a valid IP");
    else if (retval == -1)
        except::throwLogicError("Invalid family - this can't happen");

    return static_cast<ElsUint32>(dst.s_addr);
}

bool sockCanRead(int sock, const misc::Timeval& tv)
{
    int retval = -1;
    ::timeval tval;
    fd_set readset;

    FD_ZERO(&readset);
    FD_SET(sock, &readset);
    ::memset(&tval, 0, sizeof(::timeval));
    tv.toTimeval(tval);

    do
    {
        retval = select(sock+1, &readset, 0, 0, &tval);
        if (retval < 0)
        {
            if (errno == EINTR)
                continue;

            throw except::IOError("Select error: %s",
                    except::getErrnoStr(errno).c_str());
        }
    } while ((tval.tv_sec != 0) || (tval.tv_usec != 0));

    return retval == 0 ? false : true;
}

bool sockCanWrite(int sock, const misc::Timeval& tv)
{
    int retval = -1;
    ::timeval tval;
    fd_set writeset;

    FD_ZERO(&writeset);
    FD_SET(sock, &writeset);
    ::memset(&tval, 0, sizeof(::timeval));
    tv.toTimeval(tval);

    do
    {
        retval = select(sock+1, 0, &writeset, 0, &tval);
        if (retval < 0)
        {
            if (errno == EINTR)
                continue;

            throw except::IOError("Select error: %s",
                    except::getErrnoStr(errno).c_str());
        }
    } while ((tval.tv_sec != 0) || (tval.tv_usec != 0));

    return retval == 0 ? false : true;
}

ELS_END_NAMESPACE_2


