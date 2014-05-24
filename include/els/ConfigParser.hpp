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
 * @file    ConfigParser.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "String.hpp"
#include "Exception.hpp"

#include <string>
#include <map>
#include <utility>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Interface for parsing of generic ELS config files.
 */
class ConfigParser
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(SyntaxError, except::Exception);

    /**
     * @brief   Represents a set of entries in a form of key-value pairs.
     */
    class EntryMap
    {
    public:
        ELS_EXPORT_SYMBOL EntryMap(void);
        ELS_EXPORT_SYMBOL EntryMap(const EntryMap& other);
        ELS_EXPORT_SYMBOL EntryMap& operator =(const EntryMap& other);
        ELS_EXPORT_SYMBOL ~EntryMap(void);
        ELS_EXPORT_SYMBOL const std::string& entry(
                const std::string& key) const;
        ELS_EXPORT_SYMBOL bool hasEntry(const std::string& key) const;
    private:
        typedef std::map<std::string, std::string> _T_EntryMap;
        _T_EntryMap _M_entries;
        friend class ConfigParser;
    };

    /**
     * @brief   Represents a group of sections of the same type.
     */
    class SectionMap
    {
    private:
        typedef std::map<std::string, EntryMap> _T_SectionMap;
    public:
        typedef _T_SectionMap::const_iterator const_iterator;
        typedef _T_SectionMap::const_reverse_iterator const_reverse_iterator;
        ELS_EXPORT_SYMBOL SectionMap(void);
        ELS_EXPORT_SYMBOL SectionMap(const SectionMap& other);
        ELS_EXPORT_SYMBOL SectionMap& operator =(const SectionMap& other);
        ELS_EXPORT_SYMBOL ~SectionMap(void);
        ELS_EXPORT_SYMBOL const EntryMap& sect(const std::string& key) const;
        ELS_EXPORT_SYMBOL bool hasSect(const std::string& key) const;
        ELS_EXPORT_SYMBOL ElsSize size(void) const;
        ELS_EXPORT_SYMBOL const_iterator begin(void) const;
        ELS_EXPORT_SYMBOL const_iterator end(void) const;
        ELS_EXPORT_SYMBOL const_reverse_iterator rbegin(void) const;
        ELS_EXPORT_SYMBOL const_reverse_iterator rend(void) const;
    private:
        _T_SectionMap _M_sections;
        friend class ConfigParser;
    };

    ELS_EXPORT_SYMBOL ConfigParser(void);
    ELS_EXPORT_SYMBOL ConfigParser(const ConfigParser& other);
    ELS_EXPORT_SYMBOL ConfigParser& operator =(const ConfigParser& other);
    ELS_EXPORT_SYMBOL ~ConfigParser(void) throw();

    ELS_EXPORT_SYMBOL void parseFile(const std::string& path);
    ELS_EXPORT_SYMBOL void parseStr(const std::string& str);

    ELS_EXPORT_SYMBOL const EntryMap& globals(void) const;
    ELS_EXPORT_SYMBOL const SectionMap& type(const std::string& type) const;
    ELS_EXPORT_SYMBOL bool hasType(const std::string& type) const;

    ELS_EXPORT_SYMBOL std::string toStr(void) const;

private:

    typedef std::map<std::string, SectionMap> _T_TypeMap;
    typedef std::pair<std::string, std::string> _T_Entry;

    void _M_parse(const StringList& lines);
    static _T_Entry _S_splitEntry(const std::string& line);
    static void _S_stripComments(std::string& line);
    static void _S_endSection(_T_TypeMap& types, EntryMap& entr,
            std::string& curType, std::string& curSect, unsigned lineno);
    static std::string _S_handleParens(const std::string& str);
    static void _S_throwSyntaxError(unsigned lineno, const char* fmt, ...)
        ELS_PRINTF_FUNC(2, 3);

    EntryMap _M_globals;
    _T_TypeMap _M_types;
};

ELS_END_NAMESPACE_2


