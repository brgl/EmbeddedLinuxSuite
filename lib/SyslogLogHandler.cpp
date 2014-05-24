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
 * @file    SyslogLogHandler.cpp
 */

#include <els/SyslogLogHandler.hpp>
#include <els/Exception.hpp>
#include <els/Regex.hpp>

#include <syslog.h>

ELS_BEGIN_NAMESPACE_2(els, log)

SyslogLogHandler::SyslogLogHandler(const std::string& ident,
        SyslogLogHandler::Facility facility, bool showPid)
    : ILogHandler(),
      _M_ident(ident),
      _M_facility(_S_facilToSyslogFacil(facility)),
      _M_flags(showPid ? LOG_PID : 0)
{

}

SyslogLogHandler::~SyslogLogHandler(void) throw()
{

}

void SyslogLogHandler::log(LogLevel level, const char* msg)
{
    ::openlog(this->_M_ident.c_str(), this->_M_flags, this->_M_facility);
    ::syslog(_S_logLevelToSyslogLevel(level), msg);
    ::closelog();
}

inline int SyslogLogHandler::_S_logLevelToSyslogLevel(LogLevel level)
{
    int prio = 0;

    switch (level)
    {
    case ELS_LOG_EMERG: prio = LOG_EMERG; break;
    case ELS_LOG_ALERT: prio = LOG_ALERT; break;
    case ELS_LOG_CRIT: prio = LOG_CRIT; break;
    case ELS_LOG_ERROR: prio = LOG_ERR; break;
    case ELS_LOG_WARN: prio = LOG_WARNING; break;
    case ELS_LOG_NOTICE: prio = LOG_NOTICE; break;
    case ELS_LOG_INFO: prio = LOG_INFO; break;
    case ELS_LOG_DEBUG: prio = LOG_DEBUG; break;
    default:
        except::throwLogicError("No such loglevel - this shouldn't happen");
        break;
    }

    return prio;
}

inline int SyslogLogHandler::_S_facilToSyslogFacil(
        SyslogLogHandler::Facility facil)
{
    int facility = 0;

    switch (facil)
    {
    case ELS_LOG_KERN: facility = LOG_KERN; break;
    case ELS_LOG_USER: facility = LOG_USER; break;
    case ELS_LOG_MAIL: facility = LOG_MAIL; break;
    case ELS_LOG_DAEMON: facility = LOG_DAEMON; break;
    case ELS_LOG_AUTH: facility = LOG_AUTH; break;
    case ELS_LOG_SYSLOG: facility = LOG_SYSLOG; break;
    case ELS_LOG_LPR: facility = LOG_LPR; break;
    case ELS_LOG_NEWS: facility = LOG_NEWS; break;
    case ELS_LOG_UUCP: facility = LOG_UUCP; break;
    case ELS_LOG_CRON: facility = LOG_CRON; break;
    case ELS_LOG_AUTHPRIV: facility = LOG_AUTHPRIV; break;
    case ELS_LOG_FTP: facility = LOG_FTP; break;
    case ELS_LOG_LOCAL0: facility = LOG_LOCAL0; break;
    case ELS_LOG_LOCAL1: facility = LOG_LOCAL1; break;
    case ELS_LOG_LOCAL2: facility = LOG_LOCAL2; break;
    case ELS_LOG_LOCAL3: facility = LOG_LOCAL3; break;
    case ELS_LOG_LOCAL4: facility = LOG_LOCAL4; break;
    case ELS_LOG_LOCAL5: facility = LOG_LOCAL5; break;
    case ELS_LOG_LOCAL6: facility = LOG_LOCAL6; break;
    case ELS_LOG_LOCAL7: facility = LOG_LOCAL7; break;
    default:
        except::throwLogicError("No such facility - this shouldn't happen");
        break;
    }

    return facility;
}

inline const std::string& SyslogLogHandler::_S_validateIdent(
        const std::string& ident)
{
    static const misc::Regex validIdent(
            "[a-zA-Z0-9\\-\\_]*[a-zA-Z][a-zA-Z0-9\\-\\_]*");

    if (ident.empty())
        throw except::InvalidArgument(
                "Identifier must not be an empty string");

    if (!validIdent.match(ident))
        throw except::InvalidArgument("Invalid Identifier");

    return ident;
}

ELS_END_NAMESPACE_2

