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
 * @file    SyslogLogHandler.hpp
 */

#pragma once

#include "Macros.hpp"
#include "ILogHandler.hpp"
#include "LogLevel.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, log)

class SyslogLogHandler : public ILogHandler
{
public:

    enum Facility
    {
        ELS_LOG_KERN = 0,
        ELS_LOG_USER,
        ELS_LOG_MAIL,
        ELS_LOG_DAEMON,
        ELS_LOG_AUTH,
        ELS_LOG_SYSLOG,
        ELS_LOG_LPR,
        ELS_LOG_NEWS,
        ELS_LOG_UUCP,
        ELS_LOG_CRON,
        ELS_LOG_AUTHPRIV,
        ELS_LOG_FTP,
        ELS_LOG_LOCAL0,
        ELS_LOG_LOCAL1,
        ELS_LOG_LOCAL2,
        ELS_LOG_LOCAL3,
        ELS_LOG_LOCAL4,
        ELS_LOG_LOCAL5,
        ELS_LOG_LOCAL6,
        ELS_LOG_LOCAL7
    };

    ELS_EXPORT_SYMBOL SyslogLogHandler(const std::string& ident,
            Facility facility, bool showPid);
    ELS_EXPORT_SYMBOL virtual ~SyslogLogHandler(void) throw();

    ELS_EXPORT_SYMBOL virtual void log(LogLevel level, const char* msg);

private:

    static inline int _S_logLevelToSyslogLevel(LogLevel level);
    static inline int _S_facilToSyslogFacil(Facility facil);
    static inline const std::string& _S_validateIdent(
            const std::string& ident);

    std::string _M_ident;
    int _M_facility;
    int _M_flags;

    ELS_CLASS_UNCOPYABLE(SyslogLogHandler);
};

ELS_END_NAMESPACE_2


