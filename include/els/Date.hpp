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
 * @file    Date.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Wraps time_t type in a class.
 *
 * It represents the number of seconds passed since 01/01/1970.
 */
class Date
{
public:

    ELS_EXPORT_SYMBOL Date(void);
    ELS_EXPORT_SYMBOL Date(ElsTime tm);
    ELS_EXPORT_SYMBOL Date(const std::string& date, const std::string& fmt);
    ELS_EXPORT_SYMBOL Date(const Date& other);
    ELS_EXPORT_SYMBOL Date& operator =(const Date& other);
    ELS_EXPORT_SYMBOL ~Date(void) throw();

    ELS_EXPORT_SYMBOL void set(ElsTime tm);
    ELS_EXPORT_SYMBOL ElsTime get(void) const;
    ELS_EXPORT_SYMBOL void fromStr(const std::string& date,
            const std::string& fmt);
    ELS_EXPORT_SYMBOL std::string toStr(const std::string& fmt) const;
    ELS_EXPORT_SYMBOL void setNow(void);

private:

    static ElsTime _S_now(void);
    static ElsTime _S_fromStr(const std::string& date,
            const std::string& fmt);

    ElsTime _M_time;
};

ELS_END_NAMESPACE_2

