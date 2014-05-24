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
 * @file    System.cpp
 */

#include <els/System.hpp>
#include <els/Exception.hpp>

#include <syscall.h>
#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

extern char** environ;

ELS_BEGIN_NAMESPACE_2(els, sys)

/**
 * @brief   Wrapper for the getpid() system call.
 * @return  Returns the process ID of the calling process.
 */
pid_t getPid(void) throw()
{
    return ::getpid();
}

/**
 * @brief   Wrapper for the gettid() system call.
 * @return  Returns the thread ID of the calling thread.
 */
pid_t getTid(void) throw()
{
    return ::syscall(SYS_gettid);
}

/**
 * @brief   Returns a pointer to the array of pointers to strings, which
 *          form the environment of the process.
 * @return  Pointer to the process environment.
 */
char** getEnvp(void) throw()
{
    return environ;
}

/**
 * @brief   Wrapper for the mkdir() system call.
 * @param   path    Path to the directory to be created.
 * @param   mode    Permissions of the new directory.
 * @throw   LibcError   If an error in the mkdir syscall occures.
 */
void makeDir(const std::string& path, ElsMode mode)
{
    int ret = ::mkdir(path.c_str(), static_cast<mode_t>(mode));
    if (ret != 0)
    {
        throw except::LibcError(errno, "mkdir: %s",
                except::getErrnoStr(errno).c_str());
    }
}

/**
 * @brief   Wrapper for the standard libc malloc function.
 * @param   size    Number of bytes to be allocated.
 * @return  Pointer to the allocated memory.
 * @throw   InvalidArgument     If size equals 0.
 * @throw   LibcError           If internal malloc function failed.
 */
void* libcMalloc(ElsSize size)
{
    void* ret = 0;

    if (size == 0)
    {
        throw except::InvalidArgument(
                "Size must be greater than 0");
    }

    ret = ::malloc(size);
    if (ret == 0)
    {
        throw except::LibcError(errno, "malloc: %s",
                except::getErrnoStr(errno).c_str());
    }

    ::memset(ret, 0, size);
    return ret;
}

/**
 * @brief   Wrapper for the standard libc free function.
 * @param   ptr     Pointer to memory previously allocated with libcMalloc.
 *
 * This function will ignore a null pointer passed.
 */
void libcFree(void* ptr)
{
    if (ptr == 0)
        return;
    ::free(ptr);
}

ELS_END_NAMESPACE_2


