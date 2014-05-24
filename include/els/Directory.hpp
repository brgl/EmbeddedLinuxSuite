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
 * @file    Directory.hpp
 */

#pragma once

#include "Macros.hpp"

#include <string>
#include <deque>

ELS_BEGIN_NAMESPACE_2(els, fs)

/**
 * @brief   Interface for accessing and browsing filesystem directories.
 */
class Directory
{
public:

    /**
     * @brief   Represents a single entry in a directory.
     *
     * Contains the name and type of a single file/subdirectory.
     */
    class DirEntry
    {
    public:

        enum FileType
        {
            TYPE_UNKNOWN = 0,
            TYPE_FIFO,
            TYPE_CHAR,
            TYPE_DIR,
            TYPE_BLOCK,
            TYPE_REGULAR,
            TYPE_LINK,
            TYPE_SOCK,
            TYPE_WHT
        };

        ELS_EXPORT_SYMBOL DirEntry(const std::string& name, FileType type);
        ELS_EXPORT_SYMBOL DirEntry(const DirEntry& other);
        ELS_EXPORT_SYMBOL DirEntry& operator=(const DirEntry& other);
        ELS_EXPORT_SYMBOL ~DirEntry(void);

        ELS_EXPORT_SYMBOL void setName(const std::string& name);
        ELS_EXPORT_SYMBOL void setType(FileType type);
        ELS_EXPORT_SYMBOL const std::string& getName(void) const;
        ELS_EXPORT_SYMBOL FileType getType(void) const;

    private:

        std::string _M_name;
        FileType _M_type;
    };

    typedef std::deque<DirEntry>::iterator iterator;
    typedef std::deque<DirEntry>::const_iterator const_iterator;
    typedef std::deque<DirEntry>::reverse_iterator reverse_iterator;
    typedef std::deque<DirEntry>::const_reverse_iterator
            const_reverse_iterator;

    ELS_EXPORT_SYMBOL Directory(void);
    ELS_EXPORT_SYMBOL explicit Directory(const std::string& path);
    ELS_EXPORT_SYMBOL Directory(const Directory& other);
    ELS_EXPORT_SYMBOL Directory& operator =(const Directory& other);
    ELS_EXPORT_SYMBOL ~Directory(void);

    ELS_EXPORT_SYMBOL void parseDir(const std::string& path);
    ELS_EXPORT_SYMBOL void clear(void);
    ELS_EXPORT_SYMBOL const Directory::DirEntry& getEnt(unsigned index) const;
    ELS_EXPORT_SYMBOL unsigned numDirEnts(void) const;

    ELS_EXPORT_SYMBOL Directory::iterator begin(void)
        { return this->_M_dirents.begin(); }
    ELS_EXPORT_SYMBOL Directory::iterator end(void)
        { return this->_M_dirents.end(); }
    ELS_EXPORT_SYMBOL Directory::reverse_iterator rbegin(void)
        { return this->_M_dirents.rbegin(); }
    ELS_EXPORT_SYMBOL Directory::reverse_iterator rend(void)
        { return this->_M_dirents.rend(); }
    ELS_EXPORT_SYMBOL Directory::const_iterator begin(void) const
        { return this->_M_dirents.begin(); }
    ELS_EXPORT_SYMBOL Directory::const_iterator end(void) const
        { return this->_M_dirents.end(); }
    ELS_EXPORT_SYMBOL Directory::const_reverse_iterator rbegin(void) const
        { return this->_M_dirents.rbegin(); }
    ELS_EXPORT_SYMBOL Directory::const_reverse_iterator rend(void) const
        { return this->_M_dirents.rend(); }

private:

    typedef std::deque<DirEntry> _T_DirEntList;

    static DirEntry::FileType _S_getFileType(unsigned char type);
    inline void _M_throwIfNotParsed(void) const;

    _T_DirEntList _M_dirents;
    bool _M_parsed;
};

ELS_END_NAMESPACE_2

