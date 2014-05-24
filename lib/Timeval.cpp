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
 * @file    Timeval.cpp
 */

#include <els/Timeval.hpp>
#include <els/Exception.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Default constructor.
 */
Timeval::Timeval(void) throw()
    : _M_sec(0),
      _M_nsec(0)
{

}

/**
 * @brief   Sets object's state to given values.
 * @param   sec     Number of seconds.
 * @param   nsec    Number of nanoseconds.
 * @throw   InvalidArgument     If nsec is not lesser than 1 000 000 000.
 */
Timeval::Timeval(ElsInt32 sec, ElsInt32 nsec)
    : _M_sec(sec),
      _M_nsec(_S_checkNsec(nsec))
{

}

/**
 * @brief   Copy constructor.
 * @param   other   Other instance of Timeval.
 */
Timeval::Timeval(const Timeval& other) throw()
    : _M_sec(other._M_sec),
      _M_nsec(other._M_nsec)
{

}

/**
 * @brief   Assignment operator.
 * @param   other   Other instance of Timeval.
 * @return  Reference to this object.
 */
Timeval& Timeval::operator =(const Timeval& other) throw()
{
    this->_M_sec = other._M_sec;
    this->_M_nsec = other._M_nsec;
    return *this;
}

/**
 * @brief   Destructor.
 */
Timeval::~Timeval(void) throw()
{

}

/**
 * @brief   Setter function for seconds.
 * @param   sec     New number of seconds.
 */
void Timeval::setSec(ElsInt32 sec) throw()
{
    this->_M_sec = sec;
}

/**
 * @brief   Setter function for nanoseconds.
 * @param   nsec    New number of nanoseconds.
 * @throw   InvalidArgument     If nsec is not lesser than 1 000 000 000.
 */
void Timeval::setNsec(ElsInt32 nsec)
{
    this->_M_nsec = _S_checkNsec(nsec);
}

/**
 * @brief   Getter function for seconds.
 * @return  Currently stored number of seconds.
 */
ElsInt32 Timeval::getSec(void) const throw()
{
    return this->_M_sec;
}

/**
 * @brief   Getter function for nanoseconds.
 * @return  Currently stored number of nanoseconds.
 */
ElsInt32 Timeval::getNsec(void) const throw()
{
    return this->_M_nsec;
}

/**
 * @brief   Stores current values in a timeval structure.
 * @param   tv      Reference to a timeval struct.
 *
 * Nanoseconds are rounded up to the nearest Million.
 */
void Timeval::toTimeval(::timeval& tv) const throw()
{
    ::memset(&tv, 0, sizeof(::timeval));
    tv.tv_sec = this->_M_sec;
    tv.tv_usec = this->_M_nsec / 1000;
}

/**
 * @brief   Stores current values in a timespec structure.
 * @param   ts      Reference to a timespec struct.
 */
void Timeval::toTimespec(::timespec& ts) const throw()
{
    ::memset(&ts, 0, sizeof(::timespec));
    ts.tv_sec = this->_M_sec;
    ts.tv_nsec = this->_M_nsec;
}

ElsInt32& Timeval::_S_checkNsec(ElsInt32& nsec)
{
    if (nsec >= 1000000000)
        throw except::InvalidArgument(
                "Nsec must be lesser than one billion, %d given", nsec);
    return nsec;
}

ELS_END_NAMESPACE_2


