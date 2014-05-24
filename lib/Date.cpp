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
 * @file    Date.cpp
 */

#include <els/Date.hpp>
#include <els/Exception.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Default constructor.
 *
 * Sets internal time to current system time.
 */
Date::Date(void)
    : _M_time(_S_now())
{

}

/**
 * @brief   Constructor setting the internal time.
 * @param   tm      Time with which the object will be initialized.
 */
Date::Date(ElsTime tm)
    : _M_time(tm)
{

}

/**
 * @brief   Constructor converting the human readable 'date' parameter
 *          to seconds since epoch according to the specified format.
 * @param   date        Human readable
 * @param   fmt         Date string format. See strptime for reference.
 * @throw   LibcError   If internal strptime function fails to convert 'date'.
 */
Date::Date(const std::string& date, const std::string& fmt)
    : _M_time(_S_fromStr(date, fmt))
{

}

/**
 * @brief   Copy constructor.
 * @param   other   Other instance of Date.
 */
Date::Date(const Date& other)
    : _M_time(other._M_time)
{

}

/**
 * @brief   Assignment operator.
 * @param   other   Other instance of Date.
 * @return  Reference to this object.
 */
Date& Date::operator =(const Date& other)
{
    this->_M_time = other._M_time;
    return *this;
}

/**
 * @brief   Destructor.
 */
Date::~Date(void) throw()
{

}

/**
 * @brief   Setter function.
 * @param   tm      New number of seconds since Epoch.
 */
void Date::set(ElsTime tm)
{
    this->_M_time = tm;
}

/**
 * @brief   Getter function.
 * @return  Currently stored number of seconds since the Epoch.
 */
ElsTime Date::get(void) const
{
    return this->_M_time;
}

/**
 * @brief   Converts human readable date to number of seconds since the Epoch
 *          and stores it.
 * @param   date        Human readable
 * @param   fmt         Date string format. See strptime for reference.
 * @throw   LibcError   If internal strptime function fails to convert 'date'.
 */
void Date::fromStr(const std::string& date, const std::string& fmt)
{
    this->_M_time = _S_fromStr(date, fmt);
}

/**
 * @brief   Converts stored date to a human readable string according
 *          to the specified format.
 * @param   fmt     Date string format.
 * @return  Human readable date as a C++ string.
 */
std::string Date::toStr(const std::string& fmt) const
{
    ::tm t;
    char buf[128];

    ::memset(&t, 0, sizeof(::tm));
    ::memset(buf, 0, sizeof(buf));
    ::localtime_r(&this->_M_time, &t);
    ::strftime(buf, sizeof(buf), fmt.c_str(), &t);

    return std::string(buf);
}

/**
 * @brief   Sets internal date to current system time.
 */
void Date::setNow(void)
{
    this->_M_time = _S_now();
}

ElsTime Date::_S_now(void)
{
    return ::time(0);
}

ElsTime Date::_S_fromStr(const std::string& date, const std::string& fmt)
{
    ::tm t;

    ::memset(&t, 0, sizeof(::tm));
    if (::strptime(date.c_str(), fmt.c_str(), &t) == 0)
    {
        throw except::LibcError(except::getErrno(),
                except::getErrnoStr(except::getErrno()).c_str());
    }
    t.tm_isdst = -1;

    return ::mktime(&t);
}

ELS_END_NAMESPACE_2


