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
 * @file    Logger.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Mutex.hpp"
#include "LogLevel.hpp"
#include "ILogHandler.hpp"
#include "ByteArray.hpp"

#include <list>
#include <cstdarg>
#include <utility>

ELS_BEGIN_NAMESPACE_2(els, log)

class Logger
{
public:

    ELS_EXPORT_SYMBOL Logger(void) throw();
    ELS_EXPORT_SYMBOL ~Logger(void) throw();

    ELS_EXPORT_SYMBOL void log(LogLevel level, const char* format, ...)
        throw() ELS_PRINTF_FUNC(3, 4);

    ELS_EXPORT_SYMBOL void emerg(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void alert(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void crit(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void error(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void warn(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void notice(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void info(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);
    ELS_EXPORT_SYMBOL void debug(const char* format, ...)
        throw() ELS_PRINTF_FUNC(2, 3);

    ELS_EXPORT_SYMBOL void addLogHandler(ILogHandler* handler,
            bool autoDelete = false) throw();

    ELS_EXPORT_SYMBOL void setLogLevel(LogLevel level) throw();

private:

    static const unsigned _S_DEFAULT_BUFSIZE;
    static const LogLevel _S_DEFAULT_LOGLEVEL;

    void _M_log(LogLevel level, const char* format,
            ::va_list va) throw();

    typedef std::list<std::pair<ILogHandler*,
            bool /* delete */ > > _T_LogHandlerList;

    LogLevel _M_logLevel;
    _T_LogHandlerList _M_logHandlers;
    misc::ByteArray _M_buffer;
    thread::Mutex _M_mutex;

    ELS_CLASS_UNCOPYABLE(Logger);
};

ELS_END_NAMESPACE_2
