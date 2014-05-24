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
 * @file    Exception.hpp
 *
 * Base exception classes for this project and a few macros regarding
 * exceptions in general.
 */

#pragma once

#include "Macros.hpp"

#include <exception>
#include <string>
#include <cstdarg>
#include <stdexcept>

ELS_BEGIN_NAMESPACE_2(els, except)

/**
 * @class   Exception
 *
 * Basic exception class for all other exceptions used inside ELS.
 */
class ELS_EXPORT_SYMBOL Exception : public std::exception
{
public:

    Exception(void) throw();
    explicit Exception(const std::string& what) throw();
    Exception(const char* format, ...) throw() ELS_PRINTF_FUNC(2, 3);
    Exception(const Exception& other) throw();
    Exception& operator =(const Exception& other) throw();
    virtual ~Exception(void) throw();

    virtual const char* what(void) const throw();

protected:

    void _M_setWhat(const char* format, ::va_list va) throw();

private:

    std::string _M_what;
};

/**
 * @class   LibcError
 *
 * Exception thrown when a function from the C standard library
 * fails. Besides error description in a form of a string, it also
 * contains the errno error number.
 */
class ELS_EXPORT_SYMBOL LibcError : public Exception
{
public:

    LibcError(void) throw();
    LibcError(int err, const std::string& what) throw();
    LibcError(int err, const char* format, ...) throw() ELS_PRINTF_FUNC(3, 4);
    LibcError(const LibcError& other) throw();
    LibcError& operator =(const LibcError& other) throw();
    virtual ~LibcError(void) throw();

    virtual int errnum(void) const throw();

private:

    int _M_errno;
};

/**
 * @brief   Call _M_setWhat after converting (...) to va_list in a function
 *          with variadic arguments.
 */
#define ELS_EXC_VA_SET_WHAT(FORMAT)                                         \
    do                                                                      \
    {                                                                       \
        ::va_list va;                                                       \
        ::va_start(va, FORMAT);                                             \
        this->_M_setWhat(FORMAT, va);                                       \
        ::va_end(va);                                                       \
    }                                                                       \
    while (false)

ELS_EXPORT_SYMBOL int getErrno(void) throw();
ELS_EXPORT_SYMBOL std::string getErrnoStr(int errnum) throw();

typedef std::logic_error LogicError;
ELS_EXPORT_SYMBOL void throwLogicError(const std::string& what);
ELS_EXPORT_SYMBOL void throwLogicError(const char* format, ...)
    ELS_PRINTF_FUNC(1, 2);

#define ELS_DECLARE_EXCEPTION(NAME, BASE)                                   \
    class ELS_EXPORT_SYMBOL NAME : public BASE                              \
    {                                                                       \
    public:                                                                 \
        NAME(void) throw();                                                 \
        explicit NAME(const std::string& what) throw();                     \
        NAME(const char* format, ...) throw() ELS_PRINTF_FUNC(2, 3);        \
        NAME(const NAME& other) throw();                                    \
        NAME& operator =(const NAME& other) throw();                        \
        virtual ~NAME(void) throw();                                        \
    }

#define ELS_DECLARE_NESTED_EXCEPTION(NAME, BASE)                            \
    ELS_DECLARE_EXCEPTION(NAME, BASE)

#define ELS_DEFINE_EXCEPTION(NAME, BASE)                                    \
    NAME::NAME(void) throw() : BASE() {}                                    \
    NAME::NAME(const std::string& what) throw() : BASE(what) {}             \
    NAME::NAME(const char* format, ...) throw() : BASE()                    \
        { ELS_EXC_VA_SET_WHAT(format); }                                    \
    NAME::NAME(const NAME& other) throw() : BASE(other) {}                  \
    NAME& NAME::operator =(const NAME& other) throw()                       \
        { BASE::operator =(other); return *this; }                          \
    NAME::~NAME(void) throw() {}

#define ELS_DEFINE_NESTED_EXCEPTION(NAME, OWNER, BASE)                      \
    OWNER::NAME::NAME(void) throw() : BASE() {}                             \
    OWNER::NAME::NAME(const std::string& what) throw() : BASE(what) {}      \
    OWNER::NAME::NAME(const char* format, ...) throw() : BASE()             \
        { ELS_EXC_VA_SET_WHAT(format); }                                    \
    OWNER::NAME::NAME(const NAME& other) throw() : BASE(other) {}           \
    OWNER::NAME& OWNER::NAME::operator =(const NAME& other) throw()         \
        { BASE::operator =(other); return *this; }                          \
    OWNER::NAME::~NAME(void) throw() {}

ELS_DECLARE_EXCEPTION(InvalidArgument, Exception);
ELS_DECLARE_EXCEPTION(OutOfRange, Exception);
ELS_DECLARE_EXCEPTION(IOError, Exception);
ELS_DECLARE_EXCEPTION(KeyNotFound, Exception);

ELS_END_NAMESPACE_2

