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
 * @file    SoLoader.cpp
 */

#include <els/SoLoader.hpp>

#include <dlfcn.h>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Default constructor.
 */
SoLoader::SoLoader(void) throw()
    : _M_handle(0)
{

}

/**
 * @brief   Constructor. Opens the object file at constructor.
 * @param   path    Path to the object file.
 * @throw   SoLoaderError   An error occured, when opening the object file.
 */
SoLoader::SoLoader(const std::string& path)
    : _M_handle(_S_getHandle(path))
{

}

/**
 * @brief   Destructor. Closes the file at destruction.
 */
SoLoader::~SoLoader(void) throw()
{
    this->close();
}

/**
 * @brief   Opens the object file.
 * @param   path    Path to the object file.
 * @throw   SoLoaderError   An error occured, when opening the object file.
 */
void SoLoader::open(const std::string& path)
{
    if (this->_M_handle != 0)
        except::throwLogicError("Already opened");
    this->_M_handle = _S_getHandle(path);
}

/**
 * @brief   Returns a pointer to the symbol if it can be resolved.
 * @param   sym     Name of the symbol.
 * @return  Void pointer to the symbol location in memory.
 * @throw   SoLoaderError   Error occured, when resolving the symbol.
 */
void* SoLoader::getSymbol(const std::string& sym)
{
    void* symHandle = 0;

    if (this->_M_handle == 0)
        except::throwLogicError("No shared object opened");

    ::dlerror();
    symHandle = ::dlsym(this->_M_handle, sym.c_str());
    if (symHandle == 0)
    {
        throw SoLoaderError(
                "Error getting symbol '%s' from shared object: %s",
                sym.c_str(), ::dlerror());
    }

    return symHandle;
}

/**
 * @brief   Closes the object file. Does nothing if no file has been opened.
 */
void SoLoader::close(void) throw()
{
    if (this->_M_handle != 0)
        ::dlclose(this->_M_handle);
    this->_M_handle = 0;
}

void* SoLoader::_S_getHandle(const std::string& path)
{
    void* handle = 0;

    ::dlerror();
    handle = ::dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    if (handle == 0)
    {
        throw SoLoaderError(
                "Error loading shared object '%s': %s",
                path.c_str(), ::dlerror());
    }

    return handle;
}

ELS_DEFINE_NESTED_EXCEPTION(SoLoaderError, SoLoader, except::Exception);

ELS_END_NAMESPACE_2


