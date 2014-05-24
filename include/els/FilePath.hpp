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
 * @file    FilePath.hpp
 */

#pragma once

#include "Macros.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, fs)

/**
 * @brief   Stores a valid Unix path.
 */
class FilePath
{
public:

    ELS_EXPORT_SYMBOL FilePath(const std::string& path);
    ELS_EXPORT_SYMBOL FilePath(const FilePath& other);
    ELS_EXPORT_SYMBOL FilePath& operator =(const FilePath& other);
    ELS_EXPORT_SYMBOL ~FilePath(void);

    ELS_EXPORT_SYMBOL const std::string& get(void) const;
    ELS_EXPORT_SYMBOL std::string basename(void) const;
    ELS_EXPORT_SYMBOL std::string dirname(void) const;

private:

    static inline const std::string& _S_validate(const std::string& path);

    static const size_t _S_MAX_PATH_LENGTH;

    std::string _M_path;
};

ELS_END_NAMESPACE_2


