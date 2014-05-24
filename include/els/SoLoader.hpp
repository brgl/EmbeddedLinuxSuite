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
 * @file    SoLoader.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Exception.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Implements an interface to the dynamic loader.
 */
class SoLoader
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(SoLoaderError, except::Exception);

    ELS_EXPORT_SYMBOL SoLoader(void) throw();
    ELS_EXPORT_SYMBOL explicit SoLoader(const std::string& path);
    ELS_EXPORT_SYMBOL ~SoLoader(void) throw();

    ELS_EXPORT_SYMBOL void open(const std::string& path);
    ELS_EXPORT_SYMBOL void* getSymbol(const std::string& sym);
    ELS_EXPORT_SYMBOL void close(void) throw();

private:

    static void* _S_getHandle(const std::string& path);

    void* _M_handle;

    ELS_CLASS_UNCOPYABLE(SoLoader);
};

ELS_END_NAMESPACE_2

