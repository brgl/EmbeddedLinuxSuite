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
 * @file    FilePath.cpp
 */

#include <els/FilePath.hpp>
#include <els/Exception.hpp>

ELS_BEGIN_NAMESPACE_2(els, fs)

const size_t FilePath::_S_MAX_PATH_LENGTH = 255;

/**
 * @brief   Constructor setting the path.
 * @param   path    Unix filepath.
 * @throw   InvalidArgument     Invalid path passed as argument.
 */
FilePath::FilePath(const std::string& path)
    : _M_path(_S_validate(path))
{

}

/**
 * @brief   Copy constructor.
 * @param   other   FilePath to be copied.
 */
FilePath::FilePath(const FilePath& other)
    : _M_path(other._M_path)
{

}

/**
 * @brief   Assignment operator.
 * @param   other   FilePath to be copied.
 * @return  Reference to this object.
 */
FilePath& FilePath::operator =(const FilePath& other)
{
    this->_M_path = other._M_path;
    return *this;
}

/**
 * @brief   Destructor.
 */
FilePath::~FilePath(void)
{

}

/**
 * @brief   Getter function.
 * @return  Constant reference to stored path.
 */
const std::string& FilePath::get(void) const
{
    return this->_M_path;
}

/**
 * @brief   Returns the filename component of the stored path.
 * @return  New string object containing the filename.
 */
std::string FilePath::basename(void) const
{
    std::string bn;

    if ((this->_M_path == ".")
            || (this->_M_path == "..")
            || (this->_M_path == "/"))
    {
        bn = this->_M_path;
    }
    else
    {
        std::string path = this->_M_path;

        if (*(path.rbegin()) == '/')
            path.erase(path.rbegin().base()-1);
        size_t pos = path.rfind('/');
        if (pos == std::string::npos)
            bn = path;
        else
            bn = path.substr(pos+1);
    }

    return bn;
}

/**
 * @brief   Returns the directory component of the stored path.
 * @return  New string object containing the directory path.
 */
std::string FilePath::dirname(void) const
{
    std::string dn;

    if ((this->_M_path == ".") || (this->_M_path == "/"))
    {
        dn = this->_M_path;
    }
    else if (this->_M_path == "..")
    {
        dn = ".";
    }
    else
    {
        std::string path = this->_M_path;

        if (*(path.rbegin()) == '/')
            path.erase(path.rbegin().base()-1);
        size_t pos = path.rfind('/');
        if (pos == std::string::npos)
            dn = ".";
        else
            dn = path.substr(0, pos);

        if (dn.empty())
            dn = "/";
    }

    return dn;
}

inline const std::string& FilePath::_S_validate(const std::string& path)
{
    if (path.size() > _S_MAX_PATH_LENGTH)
        throw except::InvalidArgument("Not a valid file path");
    return path;
}

ELS_END_NAMESPACE_2


