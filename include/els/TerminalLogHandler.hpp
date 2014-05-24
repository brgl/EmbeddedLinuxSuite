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
 * @file    TerminalLogHandler.hpp
 */

#pragma once

#include "Macros.hpp"
#include "ILogHandler.hpp"
#include "LogLevel.hpp"

ELS_BEGIN_NAMESPACE_2(els, log)

class TerminalLogHandler : public ILogHandler
{
public:

    ELS_EXPORT_SYMBOL TerminalLogHandler(void);
    ELS_EXPORT_SYMBOL virtual ~TerminalLogHandler(void) throw();

    ELS_EXPORT_SYMBOL virtual void log(LogLevel level, const char* msg);

    ELS_CLASS_UNCOPYABLE(TerminalLogHandler);
};

ELS_END_NAMESPACE_2


