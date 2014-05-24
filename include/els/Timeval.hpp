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
 * @file    Timeval.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

#include <ctime>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   This class provides a fine time resolution by wrapping a time
 *          value specified in seconds and nanoseconds (not microseconds
 *          like struct timeval).
 */
class Timeval
{
public:

    ELS_EXPORT_SYMBOL Timeval(void) throw();
    ELS_EXPORT_SYMBOL Timeval(ElsInt32 sec, ElsInt32 nsec = 0);
    ELS_EXPORT_SYMBOL Timeval(const Timeval& other) throw();
    ELS_EXPORT_SYMBOL Timeval& operator =(const Timeval& other) throw();
    ELS_EXPORT_SYMBOL ~Timeval(void) throw();

    ELS_EXPORT_SYMBOL void setSec(ElsInt32 sec) throw();
    ELS_EXPORT_SYMBOL void setNsec(ElsInt32 nsec);
    ELS_EXPORT_SYMBOL ElsInt32 getSec(void) const throw();
    ELS_EXPORT_SYMBOL ElsInt32 getNsec(void) const throw();

    ELS_EXPORT_SYMBOL void toTimeval(::timeval& tv) const throw();
    ELS_EXPORT_SYMBOL void toTimespec(::timespec& ts) const throw();

private:

    static ElsInt32& _S_checkNsec(ElsInt32& nsec);

    ElsInt32 _M_sec;
    ElsInt32 _M_nsec;
};

ELS_END_NAMESPACE_2

