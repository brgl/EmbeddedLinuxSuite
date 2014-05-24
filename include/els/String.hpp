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
 * @file    String.hpp
 * @brief   Various string manipulation functions.
 */

#pragma once

#include "Macros.hpp"
#include "StringList.hpp"

#include <string>
#include <cstdarg>

ELS_BEGIN_NAMESPACE_3(els, misc, str)

ELS_EXPORT_SYMBOL std::string buildString(const char* format, ...)
        ELS_PRINTF_FUNC(1, 2);
ELS_EXPORT_SYMBOL StringList split(const std::string& str,
        const std::string& delim = std::string(" \t\n"));
ELS_EXPORT_SYMBOL std::string join(const StringList& list,
        const std::string& sep = std::string());
ELS_EXPORT_SYMBOL bool isBlank(const std::string& str);
ELS_EXPORT_SYMBOL std::string crop(const std::string& str);
ELS_EXPORT_SYMBOL std::string trim(const std::string& str);
ELS_EXPORT_SYMBOL std::string strip(const std::string& str);

ELS_END_NAMESPACE_3

