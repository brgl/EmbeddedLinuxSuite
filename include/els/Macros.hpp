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
 * @file    Macros.hpp
 *
 * Various macro definitions used throughout the project.
 */

#pragma once

/**
 * @brief   When specified inside class declaration - indicates,
 *          that an instance of this class can't be copied.
 */
#define ELS_CLASS_UNCOPYABLE(CLASSNAME)                                 \
    private:                                                            \
        CLASSNAME(const CLASSNAME& other);                              \
        CLASSNAME& operator =(const CLASSNAME& other)

#define ELS_CLASS_NOT_INSTANTIABLE(CLASSNAME)                           \
    private:                                                            \
        CLASSNAME(void);                                                \
        ELS_CLASS_UNCOPYABLE(CLASSNAME)

#define ELS_EXPORT_SYMBOL __attribute__((visibility("default")))

#define ELS_BEGIN_NAMESPACE_1(N1)                                       \
    namespace N1 {
#define ELS_END_NAMESPACE_1                                             \
    }

#define ELS_BEGIN_NAMESPACE_2(N1, N2)                                   \
    namespace N1 {                                                      \
    namespace N2 {
#define ELS_END_NAMESPACE_2                                             \
    }                                                                   \
    }

#define ELS_BEGIN_NAMESPACE_3(N1, N2, N3)                               \
    namespace N1 {                                                      \
    namespace N2 {                                                      \
    namespace N3 {
#define ELS_END_NAMESPACE_3                                             \
    }                                                                   \
    }                                                                   \
    }

#define ELS_PRINTF_FUNC(FORMAT, PARAMS)                                 \
    __attribute__((format(printf, FORMAT, PARAMS)))

#define ELS_INIT_FIRST __attribute__((init_priority(1000)))
#define ELS_INIT_SECOND __attribute__((init_priority(2000)))
#define ELS_INIT_THIRD __attribute__((init_priority(3000)))
#define ELS_INIT_LAST __attribute__((init_priority(10000)))

#define ELS_LIKELY(EXPR) __builtin_expect((EXPR), 1)
#define ELS_UNLIKELY(EXPR) __builtin_expect((EXPR), 0)

