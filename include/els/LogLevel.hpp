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
 * @file    LogLevel.hpp
 */

#pragma once

#include "Macros.hpp"

ELS_BEGIN_NAMESPACE_2(els, log)

enum LogLevel
{
    ELS_LOG_EMERG = 0,
    ELS_LOG_ALERT,
    ELS_LOG_CRIT,
    ELS_LOG_ERROR,
    ELS_LOG_WARN,
    ELS_LOG_NOTICE,
    ELS_LOG_INFO,
    ELS_LOG_DEBUG
};

ELS_END_NAMESPACE_2


