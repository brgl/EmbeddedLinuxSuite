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
 * @file    INamedThread.hpp
 */

#pragma once

#include "Macros.hpp"
#include "IThread.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, thread)

class INamedThread : public IThread
{
public:

    ELS_EXPORT_SYMBOL explicit INamedThread(const std::string& name);
    ELS_EXPORT_SYMBOL virtual ~INamedThread(void);

    ELS_EXPORT_SYMBOL const std::string& name(void) const;

private:

    std::string _M_name;

    ELS_CLASS_UNCOPYABLE(INamedThread);
};

ELS_END_NAMESPACE_2

