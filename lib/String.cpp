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
 * @file    String.cpp
 */

#include <els/String.hpp>

#include <cstring>
#include <cstdlib>
#include <cstdio>

ELS_BEGIN_NAMESPACE_3(els, misc, str)

/**
 * @brief   Builds a string similarly to snprintf function, but returns
 *          a C++ style string.
 * @param   format  Format of the string. See snprintf for reference
 *                  on printf style formats.
 * @return  Newly built string.
 *
 * Internally this function uses vsnprintf to try and build the string
 * inside the initial buffer. If it failes to fit the string into this buffer,
 * it will start to iterate (for a maximum of 8 times), allocating a new buffer
 * twice the size of the previous one and try and fit the string into it.
 * Even if the last itaration failed, it will still return the last version
 * of the string.
 */
std::string buildString(const char* format, ...)
{
    static const ssize_t INITIAL_BUFSIZE = 512;
    static const int MAX_ITERATIONS = 8;

    char buf[INITIAL_BUFSIZE];
    ::va_list va;
    int retval = 0;
    std::string ret;

    ::memset(buf, 0, INITIAL_BUFSIZE);
    ::va_start(va, format);
    retval = ::vsnprintf(buf, INITIAL_BUFSIZE, format, va);
    ::va_end(va);

    if (retval >= INITIAL_BUFSIZE)
    {
        char* ptr = 0;
        ssize_t newBufsize = INITIAL_BUFSIZE;

        for (int i = 0; i < MAX_ITERATIONS; ++i)
        {
            newBufsize *= 2;
            ptr = new char[newBufsize];
            ::va_start(va, format);
            retval = ::vsnprintf(ptr, newBufsize, format, va);
            ::va_end(va);
            if (retval < newBufsize)
            {
                ret = ptr;
                delete[] ptr;
                break;
            }
            delete[] ptr;
        }
    }
    else
    {
        ret = buf;
    }

    return ret;
}

/**
 * @brief   Extracts tokens from string.
 * @param   str     String to be tokenized.
 * @param   delim   Set of characters, that delimit the tokens
 *                  in the parsed string.
 * @return  List of tokens extracted from 'str'.
 */
StringList split(const std::string& str, const std::string& delim)
{
    StringList sl;
    char* cpy = 0;

    if (ELS_UNLIKELY(str.empty()))
        return sl;

    if (ELS_UNLIKELY(delim.empty()))
    {
        sl.push_back(str);
        return sl;
    }

    cpy = new char[str.size()+1];
    ::memset(cpy, 0, str.size()+1);
    ::strncpy(cpy, str.data(), str.size());

    for (unsigned i = 0; i < str.size(); ++i)
    {
        if (delim.find(cpy[i]) != std::string::npos)
            cpy[i] = '\0';
    }

    for (unsigned i = 0; i < str.size()+1; ++i)
    {
        if (cpy[i] != '\0')
        {
            sl.push_back(std::string(cpy+i));
            i += sl.back().size();
        }
    }

    delete[] cpy;
    return sl;
}

/**
 * @brief   Return a string which is the concatenation of the strings
 *          in the 'list' parameter. The separator between elements can
 *          be specified too.
 * @param   list    List of strings, that will be concatenated.
 * @param   sep     Separator to be put between the elements - an empty string
 *                  by default.
 * @return  Concatenation of the strings from 'list'.
 */
std::string join(const StringList& list, const std::string& sep)
{
    std::string s;

    if (list.empty())
        return s;

    s = list.front();
    for (unsigned i = 1; i < list.size(); ++i)
    {
        if (!sep.empty())
            s.append(sep);
        s.append(list.at(i));
    }

    return s;
}

/**
 * @brief   Checks whether given string contains only whitespaces.
 * @param   str     String to be checked.
 * @return  True if str is blank, false otherwise.
 */
bool isBlank(const std::string& str)
{
    for (std::string::const_iterator it = str.begin();
            it != str.end(); ++it)
    {
        if (!::isspace(static_cast<int>(*it)))
            return false;
    }

    return true;
}

/**
 * @brief   Removes preceding whitespaces from str.
 * @param   str     String to be cropped.
 * @return  Reference to str.
 */
std::string crop(const std::string& str)
{
    std::string ret(str);
    std::string::iterator it;

    if (!ret.empty())
    {
        for (it = ret.begin(); it != ret.end(); ++it)
        {
            if (!::isspace(static_cast<int>(*it)))
                break;
        }

        if (it == ret.end())
            ret.clear();
        else
            ret.erase(ret.begin(), it);
    }

    return ret;
}

/**
 * @brief   Removes trailing whitespaces from str.
 * @param   str     String to be trimmed.
 * @return  Reference to str.
 */
std::string trim(const std::string& str)
{
    std::string ret(str);
    std::string::reverse_iterator it;

    if (!ret.empty())
    {
        for (it = ret.rbegin(); it != ret.rend(); ++it)
        {
            if (!::isspace(static_cast<int>(*it)))
                break;
        }

        if (it == ret.rend())
            ret.clear();
        else
            ret.erase(it.base(), ret.end());
    }

    return ret;
}

/**
 * @brief   Removes both preceding and trailing whitespaces from str.
 * @param   str     String to be stripped.
 * @return  Reference to str.
 */
std::string strip(const std::string& str)
{
    return crop(trim(str));
}

ELS_END_NAMESPACE_3
