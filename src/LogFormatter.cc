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

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <string>

using std::string;

namespace lsst {
namespace pex {
namespace logging {

using std::vector;
using std::ostream;
using std::endl;
using lsst::daf::base::PropertySet;
using boost::shared_ptr;

///////////////////////////////////////////////////////////
//  LogFormatter
///////////////////////////////////////////////////////////

/**
 * delete a base LogFormatter
 */
LogFormatter::~LogFormatter() { }

///////////////////////////////////////////////////////////
//  BriefFormatter
///////////////////////////////////////////////////////////

/*
 * delete the formatter.  
 */
BriefFormatter::~BriefFormatter() { }

/*
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void BriefFormatter::write(ostream *strm, const LogRecord& rec) {
    _write(strm, rec);
    (*strm)  << endl;
}

void BriefFormatter::_write(ostream *strm, const LogRecord& rec) {
    string log;
    int level=0;
    string levstr(": ");
    std::vector<std::string> comments;
    std::vector<std::string>::iterator vi;

    try {
        level = rec.data().get<int>(LSST_LP_LEVEL);
        if (level >= Log::FATAL) levstr = " FATAL: ";
        else if (level >= Log::WARN) levstr = " WARNING: ";
        else if (level < Log::INFO) levstr = " DEBUG: ";
    } catch (boost::bad_any_cast ex) { 
    } catch (pex::exceptions::NotFoundException ex) { }

    try { 
        log = rec.data().get<string>(LSST_LP_LOG);
    } catch (boost::bad_any_cast ex) {
        log = "mis-specified_log_name";
    } catch (pex::exceptions::NotFoundException ex) { }

    try {
        comments = rec.data().getArray<string>(LSST_LP_COMMENT);
    } catch (boost::bad_any_cast ex) { 
        comments.push_back("(mis-specified_comment)");
    } catch (pex::exceptions::NotFoundException ex) { } 

    for(vi = comments.begin(); vi != comments.end(); ++vi) {
        (*strm) << log << levstr << *vi << endl;
    }

    if (isVerbose()) {
        std::vector<std::string> names = rec.data().paramNames(false);
        for(vi = names.begin(); vi != names.end(); ++vi) {
            if (*vi == LSST_LP_COMMENT || *vi == LSST_LP_LOG)
                continue;

            PropertyPrinter pp(rec.data(), *vi);
            for(PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
                (*strm) << "  " << *vi << ": ";
                pi.write(strm) << endl;
            }
        }
    }
}

///////////////////////////////////////////////////////////
//  NetLoggerFormatter
///////////////////////////////////////////////////////////

const string NetLoggerFormatter::defaultValDelim(": ");

NetLoggerFormatter::NetLoggerFormatter(const string& valueDelim) 
    : LogFormatter(), _tplookup(), _midfix(valueDelim)
{ 
    loadTypeLookup();
} 

NetLoggerFormatter::~NetLoggerFormatter() { } 

#define LSST_TL_ADD(T, C) _tplookup[typeid(T).name()] = C

void NetLoggerFormatter::loadTypeLookup() {
    LSST_TL_ADD(int, 'i');
    LSST_TL_ADD(long, 'l');
    LSST_TL_ADD(long long, 'L');
    LSST_TL_ADD(char, 'c');
    LSST_TL_ADD(std::string, 's');
    LSST_TL_ADD(DateTime, 'L');
    LSST_TL_ADD(float, 'f');
    LSST_TL_ADD(double, 'd');
    LSST_TL_ADD(bool, 'b');
}

/**
 * copy another formatter into this one
 */
NetLoggerFormatter& NetLoggerFormatter::operator=(const NetLoggerFormatter& that)
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
void NetLoggerFormatter::write(ostream *strm, const LogRecord& rec) {
    string newl("\n");
    bool wrote = false;
    std::vector<std::string> comments;
    std::vector<std::string>::iterator vi;

    std::vector<std::string> names = rec.data().paramNames(false);
    for(vi = names.begin(); vi != names.end(); ++vi) {
        char tp = _tplookup[rec.data().typeOf(*vi).name()];
        if (*vi == "DATE") 
            tp = 't';
        else if (tp == 0) 
            tp = '?';
        

        PropertyPrinter pp(rec.data(), *vi);
        for(PropertyPrinter::iterator pi=pp.begin(); pi.notAtEnd(); ++pi) {
            (*strm) << tp << " " << *vi << _midfix;
            pi.write(strm) << newl;
            if (!wrote) wrote = true;
        }
    }

    if (wrote) (*strm) << endl;
}

}}} // end lsst::pex::logging

