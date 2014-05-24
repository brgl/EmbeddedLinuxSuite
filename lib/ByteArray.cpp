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
 * @file    ByteArray.cpp
 */

#include <els/ByteArray.hpp>
#include <els/Exception.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Default constructor. Does not allocate any buffer.
 */
ByteArray::ByteArray(void) throw()
    : _M_buf()
{

}

/**
 * @brief   Constructor. Allocates an array and sets all bytes to zero.
 * @param   size    Initial size of the array.
 * @throw   InvalidArgument     Given size equals 0.
 */
ByteArray::ByteArray(ElsSize size)
    : _M_buf(_S_validateSize(size), 0)
{
    this->zero();
}

/**
 * @brief   Constructor. Allocates an array, then copies the contents of
 *          given buffer into it.
 * @param   src     Source buffer.
 * @param   size    Number of bytes to be copied.
 * @throw   InvalidArgument     Given size equals 0.
 */
ByteArray::ByteArray(const void* src, ElsSize size)
    : _M_buf(_S_validateSize(size), 0)
{
    this->_M_throwIfSrcNull(src);
    this->set(src, size);
}

/**
 * @brief   Constructor. Allocates an array, then copies the contents of
 *          given string into it.
 * @param   str     String, the contents of which will be copied.
 * @throw   InvalidArgument     Empty string passed.
 */
ByteArray::ByteArray(const std::string& str)
    : _M_buf(_S_validateSize(str.size()), 0)
{
    this->set(reinterpret_cast<void*>(
            const_cast<char*>(str.data())), str.size());
}

/**
 * @brief   Copy constructor.
 * @param   other   ByteArray to be copied.
 */
ByteArray::ByteArray(const ByteArray& other)
    : _M_buf(other._M_buf)
{

}

/**
 * @brief   Assignment operator.
 * @param   other   Byte array to be copied.
 * @return  Reference to this object.
 */
ByteArray& ByteArray::operator =(const ByteArray& other)
{
    this->_M_buf = other._M_buf;
    return *this;
}

/**
 * @brief   Destructor. Frees the internal buffer.
 */
ByteArray::~ByteArray(void) throw()
{

}

/**
 * @brief   Getter function for the internal buffer.
 * @return  Pointer pointing to the beginning of the internal array.
 */
void* ByteArray::get(void) throw()
{
    return this->_M_bufStart();
}

/**
 * @brief   Getter function for the internal buffer.
 * @return  Constant pointer to the beginning of the internal buffer.
 *
 * This is a const version of the get() function.
 */
const void* ByteArray::get(void) const throw()
{
    return this->_M_bufStart();
}

/**
 * @brief   Setter function. Fills the internal buffer with given content.
 * @param   src     Source of bytes to be copied into buffer.
 * @param   size    Number of bytes to be copied.
 * @throw   InvalidArgument     Given size is greater than the buffer size.
 * @throw   LogicError          No buffer allocated yet.
 */
void ByteArray::set(const void* src, ElsSize size)
{
    this->_M_throwIfSrcNull(src);
    this->_M_throwIfEmpty();
    this->_M_throwIfSizeWrong(size);

    ::memcpy(this->_M_bufStart(), src, size);
}

/**
 * @brief   Tries to convert the contents of the internal buffer
 *          to a C++ buffer.
 * @return  Converted string.
 * @throw   InvalidArgument     Internal buffer contains non-printable chars.
 * @throw   LogicError          No buffer allocated yet.
 */
std::string ByteArray::toStr(void) const
{
    this->_M_throwIfEmpty();

    return std::string(const_cast<char*>(
            reinterpret_cast<const char*>(
                    this->_M_bufStart())),
            this->_M_buf.size());
}

/**
 * @brief   Tries to resize the internal buffer to new size - possibly
 *          saving existing content.
 * @param   size    New size.
 */
void ByteArray::resize(ElsSize size)
{
    this->_M_buf.resize(size);
}

/**
 * @brief   Deallocates the internal buffer and resets the object state.
 */
void ByteArray::clear(void) throw()
{
    this->_M_buf.clear();
}

/**
 * @brief   Sets all bytes in the internal array to 0. Does nothing
 *          if no buffer is allocated.
 */
void ByteArray::zero(void) throw()
{
    if (!this->empty())
        ::memset(this->_M_bufStart(), 0, this->_M_buf.size());
}

/**
 * @brief   Getter function for buffer size.
 * @return  Current size of the buffer.
 */
ElsSize ByteArray::size(void) const throw()
{
    return this->_M_buf.size();
}

/**
 * @brief   Indicates whether any buffer space has been allocated.
 * @return  True if the buffer is allocated, false otherwise.
 */
bool ByteArray::empty(void) const throw()
{
    return this->_M_buf.empty();
}

/**
 * @brief   Returns the value of the byte at the specific position. Does not
 *          perform any error checking.
 * @param   index   Position of byte to be returned.
 * @return  Value of the byte at given position.
 */
ElsByte ByteArray::operator [](unsigned index) const throw()
{
    return this->_M_buf[index];
}

inline void ByteArray::_M_throwIfSrcNull(const void* src) const
{
    if (src == 0)
        throw except::InvalidArgument("Src must not be a null pointer");
}

inline void ByteArray::_M_throwIfEmpty(void) const
{
    if (this->empty())
        except::throwLogicError("Buffer not allocated");
}

inline void ByteArray::_M_throwIfSizeWrong(ElsSize size) const
{
    if ((size == 0) || (size > this->_M_buf.size()))
        throw except::InvalidArgument("Invalid size");
}

inline ElsSize ByteArray::_S_validateSize(ElsSize size)
{
    if (size == 0)
        throw except::InvalidArgument("Size must be greater than 0");
    return size;
}

inline ElsByte* ByteArray::_M_bufStart(void)
{
    return &(this->_M_buf[0]);
}

inline const ElsByte* ByteArray::_M_bufStart(void) const
{
    return &(this->_M_buf[0]);
}

ELS_END_NAMESPACE_2


