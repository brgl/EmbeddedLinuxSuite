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
 * @file    File.cpp
 */

#include <els/File.hpp>
#include <els/Exception.hpp>
#include <els/System.hpp>

ELS_BEGIN_NAMESPACE_2(els, fs)

const int File::FILE_BINARY = 0x0001;

File::File(void) throw()
    : _M_path(),
      _M_handle(0)
{

}

File::File(const FilePath& path, Mode mode, int flags)
    : _M_path(path.get()),
      _M_handle(_S_open(path, mode, flags))
{

}

File::~File(void) throw()
{
    try { this->close(); } catch (...) {}
}

void File::open(const FilePath& path, Mode mode, int flags)
{
    this->_M_path = path.get();
    this->_M_handle = _S_open(path, mode, flags);
}

void File::close(void)
{
    int ret = 0;

    this->_M_throwIfNotOpen();
    ret = ::fclose(this->_M_handle);
    if (ret == EOF)
    {
        throw except::IOError(
                "Error closing file '%s': %s",
                this->_M_path.c_str(),
                except::getErrnoStr(except::getErrno()).c_str());
    }
}

void File::flush(void)
{
    int ret = 0;

    this->_M_throwIfNotOpen();
    ret = ::fflush(this->_M_handle);
    if (ret == EOF)
    {
        throw except::IOError(
                "Error flushing file '%s': %s",
                this->_M_path.c_str(),
                except::getErrnoStr(except::getErrno()).c_str());
    }
}

ElsSize File::read(misc::ByteArray& buf)
{
    if (buf.empty())
        throw except::InvalidArgument("Buffer must not be empty");
    return this->read(buf.get(), buf.size());
}

ElsSize File::read(void* buf, ElsSize size)
{
    size_t ret = 0;

    this->_M_throwIfNotOpen();
    ret = ::fread(buf, 1, size, this->_M_handle);
    if (ret != size)
    {
        if (this->_M_error())
        {
            this->_M_clearerr();
            throw except::IOError("Error reading from file '%s': %s",
                    this->_M_path.c_str(),
                    except::getErrnoStr(except::getErrno()).c_str());
        }
    }
    return ret;
}

void File::readline(std::string& str)
{
    char* lineptr = 0;
    size_t size = 0;
    ssize_t ret = 0;

    this->_M_throwIfNotOpen();
    ret = ::getline(&lineptr, &size, this->_M_handle);
    if (ret < 0)
    {
        if (this->_M_error())
        {
            sys::libcFree(lineptr);
            this->_M_clearerr();
            throw except::IOError("Error reading line from file '%s': %s",
                    this->_M_path.c_str(),
                    except::getErrnoStr(except::getErrno()).c_str());
        }
    }

    if (ret < 0)
        str.clear();
    else
        str = std::string(lineptr, ret-1);
    sys::libcFree(lineptr);
}

std::string File::readline(void)
{
    std::string ret;

    this->readline(ret);
    return ret;
}

ElsSize File::write(const misc::ByteArray& buf)
{
    if (buf.empty())
        return 0;
    return this->write(buf.get(), buf.size());
}

ElsSize File::write(const std::string& str)
{
    if (str.empty())
        return 0;
    return this->write(str.data(), str.size());
}

ElsSize File::write(const void* buf, ElsSize size)
{
    size_t ret = 0;

    this->_M_throwIfNotOpen();
    ret = ::fwrite(buf, 1, size, this->_M_handle);
    if (ret != size)
    {
        if (this->_M_error())
        {
            this->_M_clearerr();
            throw except::IOError("Error writing to file '%s': %s",
                    this->_M_path.c_str(),
                    except::getErrnoStr(except::getErrno()).c_str());
        }
    }
    return ret;
}

void File::writeall(const misc::ByteArray& buf)
{
    if (buf.empty())
        return;
    return this->writeall(buf.get(), buf.size());
}

void File::writeall(const std::string& str)
{
    if (str.empty())
        return;
    return this->writeall(str.data(), str.size());
}

void File::writeall(const void* buf, ElsSize size)
{
    size_t ret = 0;

    ret = this->write(buf, size);
    if (ret != size)
    {
        throw except::IOError("Failed to write everything to file '%s': %s",
                this->_M_path.c_str(),
                except::getErrnoStr(except::getErrno()).c_str());
    }
}

bool File::eof(void) const
{
    this->_M_throwIfNotOpen();
    return static_cast<bool>(::feof(this->_M_handle));
}

const std::string& File::path(void) const throw()
{
    return this->_M_path;
}

int File::fd(void) const
{
    this->_M_throwIfNotOpen();
    return ::fileno(this->_M_handle);
}

::FILE* File::_S_open(const FilePath& path, Mode mode, int flags)
{
    ::FILE* fd = 0;

    fd = ::fopen(path.get().c_str(), _S_getMode(mode, flags).c_str());
    if (fd == 0)
    {
        throw except::IOError(
                "Error openning file '%s': %s",
                path.get().c_str(),
                except::getErrnoStr(except::getErrno()).c_str());
    }

    return fd;
}

inline void File::_M_throwIfNotOpen(void) const
{
    if (ELS_UNLIKELY(this->_M_handle == 0))
        except::throwLogicError("File not open");
}

std::string File::_S_getMode(Mode mode, int flags)
{
    std::string s;

    switch (mode)
    {
    case MODE_READ: s.append("r"); break;
    case MODE_WRITE: s.append("w"); break;
    case MODE_APPEND: s.append("a"); break;
    default: throw except::InvalidArgument("Invalid mode"); break;
    }

    if (flags & FILE_BINARY)
        s.append("b");

    return s;
}

bool File::_M_error(void) const
{
    this->_M_throwIfNotOpen();
    return static_cast<bool>(::ferror(this->_M_handle));
}

void File::_M_clearerr(void)
{
    ::clearerr(this->_M_handle);
}

ELS_END_NAMESPACE_2


