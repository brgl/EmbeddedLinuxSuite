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
 * @file    TerminalLogHandler.cpp
 */

#include <els/TerminalLogHandler.hpp>

#include <cstdio>

ELS_BEGIN_NAMESPACE_2(els, log)

TerminalLogHandler::TerminalLogHandler(void)
    : ILogHandler()
{

}

TerminalLogHandler::~TerminalLogHandler(void) throw()
{

}

void TerminalLogHandler::log(LogLevel level, const char* msg)
{
    if (level > ELS_LOG_WARN)
        ::fprintf(stdout, "%s\n", msg);
    else
        ::fprintf(stderr, "%s\n", msg);
}

ELS_END_NAMESPACE_2


