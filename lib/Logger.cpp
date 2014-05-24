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
 * @file    Logger.cpp
 */

#include <els/Logger.hpp>
#include <els/AutoMutex.hpp>

#include <cstdio>

ELS_BEGIN_NAMESPACE_2(els, log)

#define ELS_VA_LOG(LEVEL, FORMAT)                                           \
    do                                                                      \
    {                                                                       \
        ::va_list va;                                                       \
        ::va_start(va, format);                                             \
        this->_M_log(LEVEL, FORMAT, va);                                    \
        ::va_end(va);                                                       \
    }                                                                       \
    while (false)

const unsigned Logger::_S_DEFAULT_BUFSIZE = 4096;
const LogLevel Logger::_S_DEFAULT_LOGLEVEL = ELS_LOG_WARN;

Logger::Logger(void) throw()
    : _M_logLevel(_S_DEFAULT_LOGLEVEL),
      _M_logHandlers(),
      _M_buffer(_S_DEFAULT_BUFSIZE),
      _M_mutex()
{
    this->_M_buffer.zero();
}

Logger::~Logger(void) throw()
{
    for (_T_LogHandlerList::iterator it = this->_M_logHandlers.begin();
            it != this->_M_logHandlers.end(); ++it)
    {
        if (it->second)
            delete it->first;
    }

    this->_M_logHandlers.clear();
}

void Logger::log(LogLevel level, const char* format, ...) throw()
{
    ELS_VA_LOG(level, format);
}

void Logger::emerg(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_EMERG, format);
}

void Logger::alert(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_ALERT, format);
}

void Logger::crit(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_CRIT, format);
}

void Logger::error(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_ERROR, format);
}

void Logger::warn(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_WARN, format);
}

void Logger::notice(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_NOTICE, format);
}

void Logger::info(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_INFO, format);
}

void Logger::debug(const char* format, ...) throw()
{
    ELS_VA_LOG(ELS_LOG_DEBUG, format);
}

void Logger::addLogHandler(ILogHandler* handler, bool autoDelete) throw()
{
    this->_M_mutex.lock();
    this->_M_logHandlers.push_back(std::make_pair(handler, autoDelete));
    this->_M_mutex.unlock();
}

void Logger::setLogLevel(LogLevel level) throw()
{
    this->_M_mutex.lock();
    this->_M_logLevel = level;
    this->_M_mutex.unlock();
}

void Logger::_M_log(LogLevel level, const char* format, ::va_list va) throw()
{
    thread::AutoMutex am(this->_M_mutex);

    if (level > this->_M_logLevel)
        return;

    this->_M_buffer.zero();
    ::snprintf(static_cast<char*>(this->_M_buffer.get()),
            this->_M_buffer.size(), format, va);

    for (_T_LogHandlerList::const_iterator it = this->_M_logHandlers.begin();
            it != this->_M_logHandlers.end(); ++it)
    {
        it->first->log(level, static_cast<char*>(this->_M_buffer.get()));
    }
}

ELS_END_NAMESPACE_2


