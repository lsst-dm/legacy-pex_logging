/* 
 * LSST Data Management System
 * Copyright 2008-2016 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
//////////////////////////////////////////////////////////////////////////////
// LogFormatter.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/LogFormatter.h"
#include "lsst/pex/logging/LogRecord.h"
#include "lsst/pex/logging/Log.h"
#include "lsst/pex/logging/PropertyPrinter.h"
#include "lsst/pex/exceptions.h"
#include "lsst/daf/base/PropertySet.h"

#include <memory>
#include <boost/any.hpp>
#include <string>
#include <sstream>

using std::string;

namespace lsst {
namespace pex {
namespace logging {

namespace dafBase = lsst::daf::base;
namespace pexExcept = lsst::pex::exceptions;

///////////////////////////////////////////////////////////
//  LogFormatter
///////////////////////////////////////////////////////////

LogFormatter::~LogFormatter() {}

///////////////////////////////////////////////////////////
//  BriefFormatter
///////////////////////////////////////////////////////////

BriefFormatter::~BriefFormatter() {}

/*
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void BriefFormatter::write(std::ostream *strm, LogRecord const& rec) {
    string log;
    int level=0;
    string levstr(": ");
    std::vector<std::string> comments;

    try {
        level = rec.data().get<int>(LSST_LP_LEVEL);
        if (level >= Log::FATAL) levstr = " FATAL: ";
        else if (level >= Log::WARN) levstr = " WARNING: ";
        else if (level < Log::INFO) levstr = " DEBUG: ";
    } catch (pexExcept::TypeError const & ex) {
    } catch (pexExcept::NotFoundError const & ex) {}

    try { 
        log = rec.data().get<string>(LSST_LP_LOG);
    } catch (pexExcept::TypeError const & ex) {
        log = "mis-specified_log_name";
    } catch (pexExcept::NotFoundError const & ex) {}

    try {
        comments = rec.data().getArray<string>(LSST_LP_COMMENT);
    } catch (pexExcept::TypeError const & ex) {
        comments.push_back("(mis-specified_comment)");
    } catch (pexExcept::NotFoundError const & ex) {}

    for (auto const& vi : comments) {
        (*strm) << log << levstr << vi << std::endl;
    }

    if (isVerbose() || rec.willShowAll()) {
        std::vector<std::string> names = rec.data().paramNames(false);
        for (auto const& vi : names) {
            if (vi == LSST_LP_COMMENT || vi == LSST_LP_LOG) continue;

            PropertyPrinter pp(rec.data(), vi);
            for (PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
                (*strm) << "  " << vi << ": ";
                pi.write(strm) << std::endl;
            }
        }
        (*strm)  << std::endl;
    }
}

///////////////////////////////////////////////////////////
//  IndentedFormatter
///////////////////////////////////////////////////////////

IndentedFormatter::~IndentedFormatter() {}

/*
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void IndentedFormatter::write(std::ostream *strm, LogRecord const& rec) {
    string log;
    int level=0;
    string levstr(": ");
    std::vector<std::string> comments;

    try {
        level = rec.data().get<int>(LSST_LP_LEVEL);
        if (level >= Log::FATAL) levstr = " FATAL: ";
        else if (level >= Log::WARN) levstr = " WARNING: ";
        else if (level < Log::INFO) levstr = " DEBUG: ";
    } catch (pexExcept::TypeError const & ex) {
    } catch (pexExcept::NotFoundError const & ex) {}

    try { 
        log = rec.data().get<string>(LSST_LP_LOG);
    } catch (pexExcept::TypeError const & ex) {
        log = "mis-specified_log_name";
    } catch (pexExcept::NotFoundError const & ex) {}

    try {
        comments = rec.data().getArray<string>(LSST_LP_COMMENT);
    } catch (pexExcept::TypeError const & ex) {
        comments.push_back("(mis-specified_comment)");
    } catch (pexExcept::NotFoundError const & ex) {}

    std::ostringstream indentstr;
    if (level < 0) {
        // indent the message
        for (int i=level; i < 0; ++i) {
            indentstr << ' ';
        }
    }
    string indent(indentstr.str());

    for (auto const& vi : comments) {
        (*strm) << indent << log << levstr << vi << std::endl;
    }

    if (isVerbose() || rec.willShowAll()) {
        std::vector<std::string> names = rec.data().paramNames(false);
        for (auto const& vi : names) {
            if (vi == LSST_LP_COMMENT || vi == LSST_LP_LOG) continue;

            PropertyPrinter pp(rec.data(), vi);
            for (PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
                (*strm) << indent << "  " << vi << ": ";
                pi.write(strm) << std::endl;
            }
        }
        (*strm)  << std::endl;
    }
}

///////////////////////////////////////////////////////////
//  NetLoggerFormatter
///////////////////////////////////////////////////////////
//@cond

const string NetLoggerFormatter::defaultValDelim(": ");

NetLoggerFormatter::NetLoggerFormatter(string const& valueDelim)
    : LogFormatter(), _tplookup(), _midfix(valueDelim)
{ 
    loadTypeLookup();
} 

NetLoggerFormatter::~NetLoggerFormatter() {}

#define LSST_TL_ADD(T, C) _tplookup[typeid(T).name()] = C

void NetLoggerFormatter::loadTypeLookup() {
    LSST_TL_ADD(int, 'i');
    LSST_TL_ADD(long, 'l');
    LSST_TL_ADD(long long, 'L');
    LSST_TL_ADD(char, 'c');
    LSST_TL_ADD(std::string, 's');
    LSST_TL_ADD(dafBase::DateTime, 'L');
    LSST_TL_ADD(float, 'f');
    LSST_TL_ADD(double, 'd');
    LSST_TL_ADD(bool, 'b');
}

/**
 * copy another formatter into this one
 */
NetLoggerFormatter& NetLoggerFormatter::operator=(NetLoggerFormatter const& that)
{
    if (this == &that) return *this;

    dynamic_cast<NetLoggerFormatter*>(this)->operator=(that);
    _midfix = that._midfix;
    return *this;
}

/**
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void NetLoggerFormatter::write(std::ostream *strm, LogRecord const& rec) {
    string newl("\n");
    bool wrote = false;
    std::vector<std::string> comments;

    std::vector<std::string> names = rec.data().paramNames(false);
    for (auto const& vi : names) {
        char tp = _tplookup[rec.data().typeOf(vi).name()];
        if (vi == "DATE")
            tp = 't';
        else if (tp == 0) 
            tp = '?';
        

        PropertyPrinter pp(rec.data(), vi);
        for (PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
            (*strm) << tp << " " << vi << _midfix;
            pi.write(strm) << newl;
            if (!wrote) wrote = true;
        }
    }

    if (wrote) (*strm) << std::endl;
}

///////////////////////////////////////////////////////////
//  PrependedFormatter
///////////////////////////////////////////////////////////

PrependedFormatter::~PrependedFormatter() {}

/*
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void PrependedFormatter::write(std::ostream *strm, LogRecord const& rec) {
    string date;
    try {
        date = rec.data().get<string>(LSST_LP_DATE) + ": ";
    } catch (...) {
        date = "(failed to get timestamp): ";
    }

    int level = 0;
    string levstr(": ");
    try {
        level = rec.data().get<int>(LSST_LP_LEVEL);
        if (level >= Log::FATAL) levstr = " FATAL: ";
        else if (level >= Log::WARN) levstr = " WARNING: ";
        else if (level < Log::INFO) levstr = " DEBUG: ";
    } catch (pexExcept::TypeError const & ex) {
    } catch (pexExcept::NotFoundError const & ex) {}

    string log;
    try {
        log = rec.data().get<string>(LSST_LP_LOG);
    } catch (pexExcept::TypeError const & ex) {
        log = "mis-specified_log_name";
    } catch (pexExcept::NotFoundError const & ex) {}

    std::vector<std::string> comments;
    try {
        comments = rec.data().getArray<string>(LSST_LP_COMMENT);
    } catch (pexExcept::TypeError const & ex) {
        comments.push_back("(mis-specified_comment)");
    } catch (pexExcept::NotFoundError const & ex) {}

    string label;
    try {
        label = rec.data().get<string>(LSST_LP_LABEL);
    } catch (pexExcept::TypeError const & ex) {
        label = "mis-specified_label";
    } catch (pexExcept::NotFoundError const & ex) {}

    for (auto const& vi : comments) {
        (*strm) << date << label << ": " << log << levstr << vi << std::endl;
    }

    if (isVerbose() || rec.willShowAll()) {
        std::vector<std::string> names = rec.data().paramNames(false);
        for (auto const& vi : names) {
            if (vi == LSST_LP_COMMENT || vi == LSST_LP_LOG || vi == LSST_LP_LABEL) continue;

            PropertyPrinter pp(rec.data(), vi);
            for (PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
                (*strm) << "  " << vi << ": ";
                pi.write(strm) << std::endl;
            }
        }
        (*strm) << std::endl;
    }
}

//@endcond
}}} // end lsst::pex::logging

