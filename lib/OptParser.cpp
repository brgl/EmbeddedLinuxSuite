/*
 * Copyright (C) 2013 Bartosz Golaszewski
 *
 * This program is free software you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @file    OptParser.cpp
 */

#include <els/OptParser.hpp>
#include <els/String.hpp>

#include <cctype>
#include <memory>

ELS_BEGIN_NAMESPACE_2(els, misc)

const int OptParser::OPT_UNIQUE = 0x0001;
const int OptParser::OPT_THROW = 0x0002;

const char OptParser::OPT_NOOPT = '-';

const Regex OptParser::_S_REG_OPT("^\\-[a-zA-Z]$");
const Regex OptParser::_S_REG_LONGOPT("\\-{2}[a-zA-Z][a-zA-Z\\_\\-]*");
const Regex OptParser::_S_REG_LONGOPT_ARG(
        "^\\-{2}[a-zA-Z][a-zA-Z\\_\\-]*\\=.+$");
const Regex OptParser::_S_REG_OPTS_OR_OPTARG("^\\-[a-zA-Z].+$");
const Regex OptParser::_S_REG_NONOPT("^([^\\-].*)|\\-$");

const Regex OptParser::_S_REG_VALID_LONGOPT("^[a-zA-Z][a-zA-Z0-9\\_\\-]*$");

OptParser::Option::Option(void)
    : _M_set(false),
      _M_args()
{

}

OptParser::Option::Option(const Option& other)
    : _M_set(other._M_set),
      _M_args(other._M_args)
{

}

OptParser::Option& OptParser::Option::operator =(const Option& other)
{
    this->_M_set = other._M_set;
    this->_M_args = other._M_args;
    return *this;
}

OptParser::Option::~Option(void)
{

}

bool OptParser::Option::set(void) const
{
    return this->_M_set;
}

bool OptParser::Option::hasArgs(void) const
{
    return !(this->_M_args.empty());
}

const std::string& OptParser::Option::getArg(void) const
{
    if (this->_M_args.empty())
        except::throwLogicError("Argument list empty");
    return this->_M_args.front();
}

const StringList& OptParser::Option::getArgs(void) const
{
    return this->_M_args;
}

/**
 * @brief   Default constructor.
 */
OptParser::OptParser(void)
    : _M_optList(),
      _M_optMap(),
      _M_longoptMap(),
      _M_nonopts(),
      _M_parsed(false)
{

}

/**
 * @brief   Copy constructor. Copies the entire parser state.
 * @param   other   Other instance of OptParser.
 */
OptParser::OptParser(const OptParser& other)
    : _M_optList(other._M_optList),
      _M_optMap(other._M_optMap),
      _M_longoptMap(other._M_longoptMap),
      _M_nonopts(other._M_nonopts),
      _M_parsed(other._M_parsed)
{

}

/**
 * @brief   Assignment operator. Copies the entire parser state.
 * @param   other   Other instance of OptParser.
 */
OptParser& OptParser::operator =(const OptParser& other)
{
    this->_M_optList = other._M_optList;
    this->_M_optMap = other._M_optMap;
    this->_M_longoptMap = other._M_longoptMap;
    this->_M_nonopts = other._M_nonopts;
    this->_M_parsed = other._M_parsed;
    return *this;
}

/**
 * @brief   Destructor. Frees the option list upon destruction.
 */
OptParser::~OptParser(void) throw()
{

}

/**
 * @brief   Adds new option to the parser.
 * @param   opt                 Short option name.
 * @param   longopt             Long option name.
 * @param   mode                Argument mode.
 * @param   flags               Option flags.
 * @throw   InvalidArgument     Invalid short option passed.
 */
void OptParser::addOpt(char opt, const std::string& longopt,
        ArgMode mode, int flags)
{
    if ((opt == OPT_NOOPT) && longopt.empty())
        throw except::InvalidArgument("Unable to add an empty option");

    std::auto_ptr<_T_OptHandle> newOpt(new _T_OptHandle);

    newOpt->mode = mode;
    newOpt->flags = flags;
    if (opt != OPT_NOOPT)
        this->_M_optMap.insert(std::make_pair(_S_validateOpt(opt),
                newOpt.get()));
    if (!longopt.empty())
        this->_M_longoptMap.insert(std::make_pair(
                _S_validateLongopt(longopt), newOpt.get()));
    this->_M_optList.push_back(_T_OptHandlePtr(newOpt.release()));
}

/**
 * @brief   Overloaded version of parse() method.
 * @param   argc            Number of arguments passed.
 * @param   argv            Array of argument strings.
 * @throw   ParsingError    Propagated from the base parse method.
 */
void OptParser::parse(int argc, char** argv)
{
    this->parse(argc, const_cast<const char**>(argv));
}

/**
 * @brief   Overloaded version of parse() method.
 * @param   argc            Number of arguments passed.
 * @param   argv            Array of argument strings.
 * @throw   ParsingError    Propagated from the base parse method.
 *
 * Accepts argv as a const array of C strings.
 */
void OptParser::parse(int argc, const char** argv)
{
    StringList args(argc);

    for (int i = 0; i < argc; ++i)
        args.at(i) = argv[i];
    this->parse(args);
}

/**
 * @brief   Parses given argument list.
 * @param   args            Arguments in a form of a string list.
 * @throw   ParsingError    Error parsing the arguments.
 *
 * Sets options according to previously set specification, and stores
 * all non-option arguments.
 */
void OptParser::parse(const StringList& args) try
{
    _T_TokStream toks;

    if (args.empty())
        return;

    _S_makeTokStream(args, toks);
    toks.start();
    while (!toks.atEnd())
    {
        std::string val = toks.get().val;
        switch (toks.get().type)
        {
        case _T_OptToken::OPT:
            {
                _T_OptHandle* handle = this->_M_getOptHandle(val.at(0));
                this->_M_handleOption(handle, toks);
            }
            break;
        case _T_OptToken::LONGOPT:
            {
                _T_OptHandle* handle = this->_M_getOptHandle(val);
                this->_M_handleOption(handle, toks);
            }
            break;
        case _T_OptToken::LONGOPT_ARG:
            {
                StringList sl = str::split(val, "=");
                _T_OptHandle* handle = this->_M_getOptHandle(sl.at(0));
                _S_handleFlags(handle, toks);
                handle->opt._M_set = true;
                handle->opt._M_args.push_back(
                        str::join(StringList(sl.begin()+1, sl.end())));
            }
            break;
        case _T_OptToken::OPT_OR_OPTARG:
            {
                _T_OptHandle* handle = this->_M_getOptHandle(val.at(0));
                if (handle->mode == OPT_NOARG)
                {
                    _S_handleFlags(handle, toks);
                    handle->opt._M_set = true;
                    for (std::string::const_iterator it = val.begin()+1;
                            it != val.end(); ++it)
                    {
                        handle = this->_M_getOptHandle(*it);
                        _S_handleFlags(handle, toks);
                        if (handle->mode != OPT_NOARG)
                        {
                            throw ParsingError(
                                    "Option '%c' requires an argument", *it);
                        }
                        handle->opt._M_set = true;
                    }
                }
                else
                {
                    _S_handleFlags(handle, toks);
                    handle->opt._M_set = true;
                    handle->opt._M_args.push_back(
                            std::string(val.begin()+1, val.end()));
                }
            }
            break;
        case _T_OptToken::NONOPT:
            this->_M_nonopts.push_back(val);
            break;
        default:
            except::throwLogicError(
                    "Invalid token type - this should not happen");
            break;
        }
        toks.next();
    }

    this->_M_parsed = true;
}
catch (const NoSuchOption& e)
{
    throw ParsingError(e.what());
}

void OptParser::_M_handleOption(_T_OptHandle* handle, _T_TokStream& toks)
{
    std::string arg = toks.get().val;

    _S_handleFlags(handle, toks);
    handle->opt._M_set = true;
    switch (handle->mode)
    {
    case OPT_NOARG:
        break;
    case OPT_ARGREQ:
        if (!toks.hasNext())
            throw ParsingError("Option '%s' needs an argument", arg.c_str());
        toks.next();
        handle->opt._M_args.push_back(toks.get().val);
        break;
    case OPT_ARGOPT:
        if (toks.hasNext())
        {
            toks.next();
            handle->opt._M_args.push_back(toks.get().val);
        }
        break;
    default:
        except::throwLogicError("Invalid argMode - this should not happen.");
        break;
    }
}

void OptParser::_S_handleFlags(const _T_OptHandle* handle,
        const _T_TokStream& toks)
{
    if ((handle->flags & OPT_UNIQUE) && (handle->opt._M_set))
        throw ParsingError("Second encounter of a unique option: '%s'",
                toks.get().val.c_str());

    if (handle->flags & OPT_THROW)
        throw OptThrowable(toks.get().val);
}

void OptParser::_S_makeTokStream(const StringList& args, _T_TokStream& toks)
{
    if (args.empty())
        except::throwLogicError("Empty arg list");

    for (StringList::const_iterator it = args.begin();
            it != args.end(); ++it)
    {
        std::string arg(*it);

        if (_S_REG_OPT.match(arg))
            toks.add(_T_OptToken(arg.substr(1), _T_OptToken::OPT));
        else if (_S_REG_LONGOPT_ARG.match(arg))
            toks.add(_T_OptToken(arg.substr(2),
                    _T_OptToken::LONGOPT_ARG));
        else if (_S_REG_LONGOPT.match(arg))
            toks.add(_T_OptToken(arg.substr(2), _T_OptToken::LONGOPT));
        else if (_S_REG_OPTS_OR_OPTARG.match(arg))
            toks.add(_T_OptToken(arg.substr(1),
                    _T_OptToken::OPT_OR_OPTARG));
        else if (_S_REG_NONOPT.match(arg))
            toks.add(_T_OptToken(arg, _T_OptToken::NONOPT));
        else
            throw ParsingError("Invalid token: '%s'", arg.c_str());
    }
}

/**
 * @brief   Getter function for option list.
 * @param   opt     Short option name.
 * @return  Reference to the option object.
 * @throw   NoSuchOption    No option found for name opt.
 * @throw   LogicError      No arguments parsed yet.
 */
const OptParser::Option& OptParser::getOpt(char opt) const
{
    this->_M_checkIfParsed();
    return this->_M_getOptHandle(opt)->opt;
}

/**
 * @brief   Getter function for option list.
 * @param   longopt     Long option name.
 * @return  Reference to the option object.
 * @throw   NoSuchOption    No option found for name opt.
 * @throw   LogicError      No arguments parsed yet.
 */
const OptParser::Option& OptParser::getOpt(const std::string& longopt) const
{
    this->_M_checkIfParsed();
    return this->_M_getOptHandle(longopt)->opt;
}

/**
 * @brief   Getter for non-option arguments.
 * @return  Reference to the internal StringList containing all
 *          of the non-option arguments stored after parsing.
 */
const StringList& OptParser::getNonopts(void) const
{
    return this->_M_nonopts;
}

char OptParser::_S_validateOpt(char opt)
{
    if (!isalpha(static_cast<int>(opt)))
        throw except::InvalidArgument("Not a valid option: '%c'", opt);
    return opt;
}

const std::string OptParser::_S_validateLongopt(const std::string& longopt)
{
    if (!_S_REG_VALID_LONGOPT.match(longopt))
        throw except::InvalidArgument(
                "Not a valid longopt: '%s'", longopt.c_str());
    return longopt;
}

OptParser::_T_OptHandle* OptParser::_M_getOptHandle(char opt) const
{
    _T_OptMap::const_iterator it = this->_M_optMap.find(opt);
    if (it == this->_M_optMap.end())
        throw NoSuchOption("No such option: '%c'", opt);
    return it->second;
}

OptParser::_T_OptHandle*
OptParser::_M_getOptHandle(const std::string& longopt) const
{
    _T_LongoptMap::const_iterator it = this->_M_longoptMap.find(longopt);
    if (it == this->_M_longoptMap.end())
        throw NoSuchOption("No such long option: '%s'", longopt.c_str());
    return it->second;
}

void OptParser::_M_checkIfParsed(void) const
{
    if (!this->_M_parsed)
        except::throwLogicError("Not parsed yet");
}

OptParser::OptThrowable::OptThrowable(const std::string& opt)
    throw()
    : _M_opt(opt)
{

}

OptParser::OptThrowable::OptThrowable(const OptThrowable& other)
    throw()
    : _M_opt(other._M_opt)
{

}

OptParser::OptThrowable&
OptParser::OptThrowable::operator =(const OptThrowable& other)
    throw()
{
    this->_M_opt = other._M_opt;
    return *this;
}

OptParser::OptThrowable::~OptThrowable(void) throw()
{

}

const std::string& OptParser::OptThrowable::getOpt(void) const throw()
{
    return this->_M_opt;
}

ELS_DEFINE_NESTED_EXCEPTION(ParsingError, OptParser, except::Exception);
ELS_DEFINE_NESTED_EXCEPTION(NoSuchOption, OptParser, except::InvalidArgument);

ELS_END_NAMESPACE_2


