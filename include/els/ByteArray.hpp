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
 * @file    ByteArray.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

#include <string>
#include <vector>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Automatic byte array.
 */
class ByteArray
{
public:

    ELS_EXPORT_SYMBOL ByteArray(void) throw();
    ELS_EXPORT_SYMBOL explicit ByteArray(ElsSize size);
    ELS_EXPORT_SYMBOL ByteArray(const void* src, ElsSize size);
    ELS_EXPORT_SYMBOL explicit ByteArray(const std::string& str);
    ELS_EXPORT_SYMBOL ByteArray(const ByteArray& other);
    ELS_EXPORT_SYMBOL ByteArray& operator =(const ByteArray& other);
    ELS_EXPORT_SYMBOL ~ByteArray(void) throw();

    ELS_EXPORT_SYMBOL void* get(void) throw();
    ELS_EXPORT_SYMBOL const void* get(void) const throw();
    ELS_EXPORT_SYMBOL void set(const void* src, ElsSize size);
    ELS_EXPORT_SYMBOL std::string toStr(void) const;
    ELS_EXPORT_SYMBOL void resize(ElsSize size);
    ELS_EXPORT_SYMBOL void clear(void) throw();
    ELS_EXPORT_SYMBOL void zero(void) throw();
    ELS_EXPORT_SYMBOL ElsSize size(void) const throw();
    ELS_EXPORT_SYMBOL bool empty(void) const throw();

    ELS_EXPORT_SYMBOL ElsByte operator [](unsigned index) const throw();

private:

    inline void _M_throwIfSrcNull(const void* src) const;
    inline void _M_throwIfEmpty(void) const;
    inline void _M_throwIfSizeWrong(ElsSize size) const;
    static inline ElsSize _S_validateSize(ElsSize size);
    inline ElsByte* _M_bufStart(void);
    inline const ElsByte* _M_bufStart(void) const;

    std::vector<ElsByte> _M_buf;
};

ELS_END_NAMESPACE_2

