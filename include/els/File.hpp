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
 * @file    File.hpp
 */

#pragma once

#include "Macros.hpp"
#include "ByteArray.hpp"
#include "Types.hpp"
#include "FilePath.hpp"

#include <cstdio>
#include <string>

ELS_BEGIN_NAMESPACE_2(els, fs)

/**
 * @brief   Simple interface for file manipulation.
 */
class File
{
public:

    /**
     * @brief   Enum describing the requested I/O mode for this file.
     */
    enum Mode
    {
        MODE_READ = 0,
        MODE_WRITE,
        MODE_APPEND
    };

    /**
     * @brief   Indicates the file is opened in binary mode.
     */
    static const int FILE_BINARY;

    ELS_EXPORT_SYMBOL File(void) throw();
    ELS_EXPORT_SYMBOL File(const FilePath& path, Mode mode, int flags = 0);
    ELS_EXPORT_SYMBOL ~File(void) throw();

    ELS_EXPORT_SYMBOL void open(const FilePath& path,
            Mode mode, int flags = 0);
    ELS_EXPORT_SYMBOL void close(void);
    ELS_EXPORT_SYMBOL void flush(void);
    ELS_EXPORT_SYMBOL ElsSize read(misc::ByteArray& buf);
    ELS_EXPORT_SYMBOL ElsSize read(void* buf, ElsSize size);
    ELS_EXPORT_SYMBOL void readline(std::string& str);
    ELS_EXPORT_SYMBOL std::string readline(void);
    ELS_EXPORT_SYMBOL ElsSize write(const misc::ByteArray& buf);
    ELS_EXPORT_SYMBOL ElsSize write(const std::string& str);
    ELS_EXPORT_SYMBOL ElsSize write(const void* buf, ElsSize size);
    ELS_EXPORT_SYMBOL void writeall(const misc::ByteArray& buf);
    ELS_EXPORT_SYMBOL void writeall(const std::string& str);
    ELS_EXPORT_SYMBOL void writeall(const void* buf, ElsSize size);
    ELS_EXPORT_SYMBOL bool eof(void) const;

    ELS_EXPORT_SYMBOL const std::string& path(void) const throw();
    ELS_EXPORT_SYMBOL int fd(void) const;

private:

    static ::FILE* _S_open(const FilePath& path, Mode mode, int flags);
    inline void _M_throwIfNotOpen(void) const;
    static std::string _S_getMode(Mode mode, int flags);
    bool _M_error(void) const;
    void _M_clearerr(void);

    std::string _M_path;
    ::FILE* _M_handle;

    ELS_CLASS_UNCOPYABLE(File);
};

ELS_END_NAMESPACE_2

