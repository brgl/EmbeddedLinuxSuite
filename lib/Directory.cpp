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
 * @file    Directory.cpp
 */

#include <els/Directory.hpp>
#include <els/Exception.hpp>

#include <dirent.h>

ELS_BEGIN_NAMESPACE_2(els, fs)

Directory::DirEntry::DirEntry(const std::string& name, FileType type)
    : _M_name(name),
      _M_type(type)
{

}

Directory::DirEntry::DirEntry(const DirEntry& other)
    : _M_name(other._M_name),
      _M_type(other._M_type)
{

}

Directory::DirEntry&
Directory::DirEntry::operator =(const Directory::DirEntry& other)
{
    this->_M_name = other._M_name;
    this->_M_type = other._M_type;
    return *this;
}

Directory::DirEntry::~DirEntry(void)
{

}

void Directory::DirEntry::setName(const std::string& name)
{
    this->_M_name = name;
}

void Directory::DirEntry::setType(FileType type)
{
    this->_M_type = type;
}

const std::string& Directory::DirEntry::getName(void) const
{
    return this->_M_name;
}

Directory::DirEntry::FileType Directory::DirEntry::getType(void) const
{
    return this->_M_type;
}

/**
 * @brief   Default constructor.
 */
Directory::Directory(void)
    : _M_dirents(),
      _M_parsed(false)
{

}

/**
 * @brief   Constructor. Parses given directory.
 * @param   path        Path to the directory.
 * @throw   IOError     Unable to open the directory.
 */
Directory::Directory(const std::string& path)
    : _M_dirents(),
      _M_parsed(false)
{
    this->parseDir(path);
}

/**
 * @brief   Copy constructor.
 * @param   other   Other instance of Directory.
 */
Directory::Directory(const Directory& other)
    : _M_dirents(other._M_dirents),
      _M_parsed(other._M_parsed)
{

}

/**
 * @brief   Assignment operator.
 * @param   other   Other instance of Directory.
 * @return  Reference to this object.
 */
Directory& Directory::operator =(const Directory& other)
{
    this->_M_dirents = other._M_dirents;
    this->_M_parsed = other._M_parsed;
    return *this;
}

/**
 * @brief   Destructor.
 */
Directory::~Directory(void)
{

}

void Directory::parseDir(const std::string& path)
{
    ::dirent* de = 0;
    ::DIR* dir = 0;

    this->clear();
    dir = ::opendir(path.c_str());
    if (dir == 0)
    {
        throw except::IOError(
                "Error opening directory '%s': %s",
                path.c_str(), except::getErrnoStr(
                        except::getErrno()).c_str());
    }

    while ((de = ::readdir(dir)) != 0)
    {
        this->_M_dirents.push_back(DirEntry(de->d_name,
                _S_getFileType(de->d_type)));
    }

    ::closedir(dir);
    this->_M_parsed = true;
}

/**
 * @brief   Resets the state of this object. Deletes all stored entries.
 */
void Directory::clear(void)
{
    this->_M_dirents.clear();
    this->_M_parsed = false;
}

/**
 * @brief   Returns a reference to a single element in the directory.
 * @param   index   Index of the element.
 * @return  Reference to the DirEntry object.
 * @throw   LogicError  No directory has been parsed yet.
 */
const Directory::DirEntry& Directory::getEnt(unsigned index) const
{
    this->_M_throwIfNotParsed();
    return this->_M_dirents.at(index);
}

/**
 * @brief   Returns the number of files in this directory.
 * @return  Number of files in the directory.
 */
unsigned Directory::numDirEnts(void) const
{
    return this->_M_dirents.size();
}

Directory::DirEntry::FileType Directory::_S_getFileType(unsigned char type)
{
    DirEntry::FileType ret = DirEntry::TYPE_UNKNOWN;

    switch (type)
    {
    case DT_UNKNOWN: ret = DirEntry::TYPE_UNKNOWN; break;
    case DT_FIFO: ret = DirEntry::TYPE_FIFO; break;
    case DT_CHR: ret = DirEntry::TYPE_CHAR; break;
    case DT_DIR: ret = DirEntry::TYPE_DIR; break;
    case DT_BLK: ret = DirEntry::TYPE_BLOCK; break;
    case DT_REG: ret = DirEntry::TYPE_REGULAR; break;
    case DT_LNK: ret = DirEntry::TYPE_LINK; break;
    case DT_SOCK: ret = DirEntry::TYPE_SOCK; break;
    case DT_WHT: ret = DirEntry::TYPE_WHT; break;
    default: except::throwLogicError("Invalid file type"); break;
    }

    return ret;
}

inline void Directory::_M_throwIfNotParsed(void) const
{
    if (!this->_M_parsed)
        throw except::LogicError("Not parsed yet");
}

ELS_END_NAMESPACE_2


