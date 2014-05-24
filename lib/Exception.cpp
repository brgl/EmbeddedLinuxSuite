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
 * @file    Exception.cpp
 */

#include <els/Exception.hpp>
#include <els/Types.hpp>

#include <cstring>
#include <cstdio>
#include <cerrno>

ELS_BEGIN_NAMESPACE_2(els, except)

Exception::Exception(void) throw()
    : std::exception(),
      _M_what("Exception")
{

}

Exception::Exception(const std::string& what) throw()
    : std::exception(),
      _M_what(what.empty() ? "Exception" : what)
{

}

Exception::Exception(const char* format, ...) throw()
    : std::exception(),
      _M_what()
{
    ELS_EXC_VA_SET_WHAT(format);
}

Exception::Exception(const Exception& other) throw()
    : std::exception(other),
      _M_what(other._M_what)
{

}

Exception& Exception::operator =(const Exception& other) throw()
{
    std::exception::operator =(other);
    this->_M_what = other._M_what;
    return *this;
}

Exception::~Exception(void) throw()
{

}

const char* Exception::what(void) const throw()
{
    return this->_M_what.c_str();
}

void Exception::_M_setWhat(const char* format, ::va_list va) throw()
{
    static const size_t BUFSIZE = 512;

    char buf[BUFSIZE];

    ::memset(buf, 0, BUFSIZE);
    ::vsnprintf(buf, BUFSIZE, format, va);
    this->_M_what = std::string(buf);
}

int getErrno(void) throw()
{
    return errno;
}

LibcError::LibcError(void) throw()
    : Exception(),
      _M_errno(-1)
{

}

LibcError::LibcError(int err, const std::string& what) throw()
    : Exception(what),
      _M_errno(err)
{

}

LibcError::LibcError(int err, const char* format, ...) throw()
    : Exception(),
      _M_errno(err)
{
    ELS_EXC_VA_SET_WHAT(format);
}

LibcError::LibcError(const LibcError& other) throw()
    : Exception(other),
      _M_errno(other._M_errno)
{

}

LibcError& LibcError::operator =(const LibcError& other) throw()
{
    Exception::operator =(other);
    this->_M_errno = other._M_errno;
    return *this;
}

LibcError::~LibcError(void) throw()
{

}

int LibcError::errnum(void) const throw()
{
    return this->_M_errno;
}

std::string getErrnoStr(int errnum) throw()
{
    static const ElsSize BUFSIZE = 512;

    char buf[BUFSIZE];

    ::memset(buf, 0x0, BUFSIZE);
    return std::string(::strerror_r(errnum, buf, BUFSIZE));
}

void throwLogicError(const std::string& what)
{
    throw LogicError(what);
}

void throwLogicError(const char* format, ...)
{
    static const ElsSize BUFSIZE = 512;

    char buf[BUFSIZE];
    ::va_list va;

    ::memset(buf, 0, BUFSIZE);
    ::va_start(va, format);
    ::snprintf(buf, BUFSIZE, format, va);
    ::va_end(va);

    throw LogicError(buf);
}

ELS_DEFINE_EXCEPTION(InvalidArgument, Exception);
ELS_DEFINE_EXCEPTION(OutOfRange, Exception);
ELS_DEFINE_EXCEPTION(IOError, Exception);
ELS_DEFINE_EXCEPTION(KeyNotFound, Exception);

ELS_END_NAMESPACE_2


