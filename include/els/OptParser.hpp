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
 * @file    OptParser.hpp
 */

#pragma once

#include "Macros.hpp"
#include "StringList.hpp"
#include "Exception.hpp"
#include "Regex.hpp"
#include "SharedPtr.hpp"

#include <string>
#include <map>
#include <deque>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Getopt-like option parser for ELS.
 */
class OptParser
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(ParsingError, except::Exception);
    ELS_DECLARE_NESTED_EXCEPTION(NoSuchOption, except::InvalidArgument);

    /**
     * @brief   Thrown when an option specified with the OPT_THROW
     *          flag has been encountered.
     */
    class OptThrowable
    {
    public:
        ELS_EXPORT_SYMBOL explicit OptThrowable(const std::string& opt)
                throw();
        ELS_EXPORT_SYMBOL OptThrowable(const OptThrowable& other) throw();
        ELS_EXPORT_SYMBOL OptThrowable& operator =(
                const OptThrowable& other) throw();
        ELS_EXPORT_SYMBOL ~OptThrowable(void) throw();

        ELS_EXPORT_SYMBOL const std::string& getOpt(void) const throw();
    private:
        std::string _M_opt;
    };

    /**
     * @brief   Option argument mode.
     *
     * OPT_NOARG - no argument allowed.
     * OPT_ARGREQ - argument required.
     * OPT_ARGOPT - argument optional.
     */
    enum ArgMode
    {
        _OPT_ARG_UNDEFINED = 0,
        OPT_NOARG,
        OPT_ARGREQ,
        OPT_ARGOPT,
    };

    /**
     * @brief   Flag marking the option as unique, ie. it's not possible
     *          for it to be specified more than once.
     */
    ELS_EXPORT_SYMBOL static const int OPT_UNIQUE;
    ELS_EXPORT_SYMBOL static const int OPT_THROW;

    ELS_EXPORT_SYMBOL static const char OPT_NOOPT;

    /**
     * @brief   Represents a single valid option.
     */
    class Option
    {
    public:
        ELS_EXPORT_SYMBOL Option(void);
        ELS_EXPORT_SYMBOL Option(const Option& other);
        ELS_EXPORT_SYMBOL Option& operator =(const Option& other);
        ELS_EXPORT_SYMBOL ~Option(void);
        ELS_EXPORT_SYMBOL bool set(void) const;
        ELS_EXPORT_SYMBOL bool hasArgs(void) const;
        ELS_EXPORT_SYMBOL const std::string& getArg(void) const;
        ELS_EXPORT_SYMBOL const StringList& getArgs(void) const;
    private:
        bool _M_set;
        StringList _M_args;
        friend class OptParser;
    };

    ELS_EXPORT_SYMBOL OptParser(void);
    ELS_EXPORT_SYMBOL OptParser(const OptParser& other);
    ELS_EXPORT_SYMBOL OptParser& operator =(const OptParser& other);
    ELS_EXPORT_SYMBOL ~OptParser(void) throw();

    ELS_EXPORT_SYMBOL void addOpt(char opt, const std::string& longopt,
            ArgMode mode, int flags = 0);

    /**
     * @brief   Overloaded version of addOpt. Does not set the long name
     *          for this option.
     * @param   opt                 Short option name.
     * @param   mode                Argument mode.
     * @param   flags               Option flags.
     * @throw   InvalidArgument     Invalid short option passed.
     */
    ELS_EXPORT_SYMBOL inline void addOpt(char opt, ArgMode mode, int flags = 0)
    {
        this->addOpt(opt, "", mode, flags);
    }

    /**
     * @brief   Overloaded version of addOpt. Does not set the short
     *          name for this option.
     * @param   longopt             Long option name.
     * @param   mode                Argument mode.
     * @param   flags               Option flags.
     * @throw   InvalidArgument     Invalid short option passed.
     */
    ELS_EXPORT_SYMBOL inline void addOpt(const std::string& longopt,
            ArgMode mode, int flags = 0)
    {
        this->addOpt(OPT_NOOPT, longopt, mode, flags);
    }

    ELS_EXPORT_SYMBOL void parse(int argc, char** argv);
    ELS_EXPORT_SYMBOL void parse(int argc, const char** argv);
    ELS_EXPORT_SYMBOL void parse(const StringList& args);

    ELS_EXPORT_SYMBOL const Option& getOpt(char opt) const;
    ELS_EXPORT_SYMBOL const Option& getOpt(const std::string& longopt) const;

    ELS_EXPORT_SYMBOL const StringList& getNonopts(void) const;

private:

    static const Regex _S_REG_OPT;
    static const Regex _S_REG_LONGOPT;
    static const Regex _S_REG_LONGOPT_ARG;
    static const Regex _S_REG_OPTS_OR_OPTARG;
    static const Regex _S_REG_NONOPT;

    static const Regex _S_REG_VALID_LONGOPT;

    struct _T_OptHandle
    {
        _T_OptHandle(void) : opt(), mode(_OPT_ARG_UNDEFINED), flags(0) {}

        Option opt;
        ArgMode mode;
        int flags;
    };

    struct _T_OptToken
    {
        enum TokType
        {
            OPT = 0,
            LONGOPT,
            LONGOPT_ARG,
            OPT_OR_OPTARG,
            NONOPT
        };

        _T_OptToken(const std::string& v, TokType t) : val(v), type(t) {}

        std::string val;
        TokType type;
    };

    struct _T_TokStream
    {
        _T_TokStream(void) : toks() {}
        void start(void) { current = toks.begin(); }
        const _T_OptToken& get(void) const { return *current; }
        bool hasNext(void) const { return ((current+1) != toks.end()); }
        bool atEnd(void) const { return (current == toks.end()); }
        void next(void) { current++; }
        void add(const _T_OptToken& tok) { toks.push_back(tok); }

        typedef std::deque<_T_OptToken> TokList;

        TokList toks;
        TokList::const_iterator current;
    };

    typedef SharedPtr<_T_OptHandle> _T_OptHandlePtr;
    typedef std::deque<_T_OptHandlePtr> _T_OptList;
    typedef std::map<char, _T_OptHandle*> _T_OptMap;
    typedef std::map<std::string, _T_OptHandle*> _T_LongoptMap;

    _T_OptList _M_optList;
    _T_OptMap _M_optMap;
    _T_LongoptMap _M_longoptMap;

    StringList _M_nonopts;
    bool _M_parsed;

    static char _S_validateOpt(char opt);
    static const std::string _S_validateLongopt(const std::string& longopt);
    static void _S_makeTokStream(const StringList& args, _T_TokStream& toks);
    static void _S_handleFlags(const _T_OptHandle* handle,
            const _T_TokStream& toks);

    _T_OptHandle* _M_getOptHandle(char opt) const;
    _T_OptHandle* _M_getOptHandle(const std::string& longopt) const;
    void _M_checkIfParsed(void) const;
    void _M_handleOption(_T_OptHandle* handle, _T_TokStream& toks);
};

ELS_END_NAMESPACE_2

