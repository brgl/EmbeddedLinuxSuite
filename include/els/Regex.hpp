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
 * @file    Regex.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Exception.hpp"
#include "StringList.hpp"

#include <regex.h>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   This class wraps the glibc regular expressions library functions.
 *
 * It allows compilation of and matching against the POSIX regular
 * expression patterns.
 */
class Regex
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(RegexError, except::Exception);

    ELS_EXPORT_SYMBOL Regex(void);
    ELS_EXPORT_SYMBOL Regex(const std::string& pattern);
    ELS_EXPORT_SYMBOL Regex(const Regex& other);
    ELS_EXPORT_SYMBOL Regex& operator =(const Regex& other);
    ELS_EXPORT_SYMBOL ~Regex(void) throw();

    ELS_EXPORT_SYMBOL void compile(void);
    ELS_EXPORT_SYMBOL void compile(const std::string& pattern);
    ELS_EXPORT_SYMBOL bool match(const std::string& str) const;
    ELS_EXPORT_SYMBOL StringList findMatches(const std::string& str,
            size_t maxMatches = _S_DEF_NUM_MATCHES) const;
    ELS_EXPORT_SYMBOL void reset(void) throw();
    ELS_EXPORT_SYMBOL void setPattern(const std::string& pattern);
    ELS_EXPORT_SYMBOL const std::string& getPattern(void) const throw();

private:

    void _M_checkIfCompiled(void) const;
    void _M_extractError(int retval, char* buf, size_t bufsize) const;

    static const size_t _S_ERRBUF_SIZE = 1024;
    static const size_t _S_DEF_NUM_MATCHES = 32;

    std::string _M_pattern;
    ::regex_t _M_regex;
    bool _M_compiled;
};

ELS_END_NAMESPACE_2

