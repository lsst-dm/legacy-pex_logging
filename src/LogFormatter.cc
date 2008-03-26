//////////////////////////////////////////////////////////////////////////////
// LogFormatter.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/mwi/logging/LogFormatter.h"
#include "lsst/mwi/logging/LogRecord.h"
#include "lsst/mwi/data/DataProperty.h"
#include "lsst/mwi/utils/Trace.h"

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <string>

using std::string;

// #define EXEC_TRACE  20
// static void execTrace( string s, int level = EXEC_TRACE ){
//     lsst::mwi::utils::Trace( "mwi.logging.LogFormatter", level, s );
// }


namespace lsst {
namespace mwi {
namespace logging {

using std::vector;
using std::ostream;
using std::endl;
using lsst::mwi::data::DataProperty;
using boost::shared_ptr;

///////////////////////////////////////////////////////////
//  LogFormatter
///////////////////////////////////////////////////////////

/**
 * delete a base LogFormatter
 */
LogFormatter::~LogFormatter() { }

/**
 * a helper function for writing DataProperty values to a stream
 * Currently this will only handle primitive values of types int, long,
 * float, double, bool, and string.  
 * @param strm   the stream to write to 
 * @param value  the property value as a boost::any pointer.  
 * @return bool  false if the DataProperty value was of an unrecognized 
 *                 type.  
 */
bool LogFormatter::writeDPValue(ostream& strm, const boost::any& value) {

    if (value.type() == typeid(int)) {
        strm << boost::any_cast<int>(value);
    }
    else if (value.type() == typeid(long)) {
        strm << boost::any_cast<long>(value);
    }
    else if (value.type() == typeid(float)) {
        strm << boost::any_cast<float>(value);
    }
    else if (value.type() == typeid(double)) {
        strm << boost::any_cast<double>(value);
    }
    else if (value.type() == typeid(bool)) {
        strm << boost::any_cast<bool>(value);
    }
    else if (value.type() == typeid(string)) {
        strm << boost::any_cast<string>(value);
    }
    else {
        strm << "<unprintable>"; 
        return false; 
    }
    return true; 
}



///////////////////////////////////////////////////////////
//  BriefFormatter
///////////////////////////////////////////////////////////

/**
 * delete the formatter.  
 */
BriefFormatter::~BriefFormatter() { }

/**
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */

void BriefFormatter::write(ostream *strm, const LogRecord& rec) {
    _write(strm, rec);
    (*strm)  << endl;
}

void BriefFormatter::_write(ostream *strm, const LogRecord& rec) {
    LogRecord::DataListT otherData;
    LogRecord::DataListT comments;
    DataProperty::PtrType logname;

    const LogRecord::DataListT& data = rec.getData();
    string empty();
    string LOG("LOG"), COMMENT("COMMENT");
    for(LogRecord::DataIteratorT i=data.begin(); i != data.end(); i++) {
        string name = (*i)->getName();
        if (name == LOG) {
            logname = *i;
        }
        else if (name == COMMENT) {
            comments.push_back(*i);
        }
        else {
            otherData.push_back(*i);
        }
    }

    // print out the COMMENTS
    string context;
    if (logname.get() != 0) {
        try {
            context = boost::any_cast<string>(logname->getValue());
        }
        catch (boost::bad_any_cast ex) {
            context = "mis-specified_log_name";
        }
        if (comments.size() > 0 && context.length() > 0) 
            context += ": ";
    }

    if (comments.size() > 0) {
        string val;
        LogRecord::DataIteratorT i;
        for(i = comments.begin(); i != comments.end(); i++) {
            try {
                val = boost::any_cast<string>((*i)->getValue());
                (*strm) << context << val << endl; 
            }
            catch (boost::bad_any_cast ex) {
                (*strm) << context << "(mis-specified_comment)" << endl;
            }
        }
    }

    // if we were told to be verbose, print out the rest of the properties
    if (_doAll) {
        LogRecord::DataIteratorT i;
        for(i=otherData.begin(); i != otherData.end(); i++) {
            write(strm, i->get(), string());
        }
    }
}

/**
 * write out a data property to a stream
 * @param strm   a pointer to the output stream.  The caller is 
 *                 consider the owner of the stream.
 * @param prop   a pointer to the data property to write.  The caller is 
 *                 consider the owner of the property object.
 * @param namePrefix  the name of the parent prefix to prepend to the name
 *               of this data property.
 */
void BriefFormatter::write(ostream *strm, const DataProperty *prop, 
                           const string& namePrefix) 
{
    boost::any value = prop->getValue();
    if (value.type() == typeid(DataProperty::ContainerType)) {
        typedef DataProperty::ContainerType childrenType;
        typedef DataProperty::ContainerType::const_iterator 
            childIter;

        childrenType *children = boost::any_cast<childrenType>(&value);
        
        for(childIter i=children->begin(); i!=children->end(); i++) {
            write(strm, i->get(), namePrefix + prop->getName() + ".");
        }
    }
    else {
        (*strm) << "  " << namePrefix << prop->getName()
                << ": ";
        writeDPValue(*strm, prop->getValue());
        (*strm) << endl;
    }
}

///////////////////////////////////////////////////////////
//  NetLoggerFormatter
///////////////////////////////////////////////////////////

const string NetLoggerFormatter::defaultSep(".");
const string NetLoggerFormatter::defaultValDelim(": ");

NetLoggerFormatter::NetLoggerFormatter(const string& nameSep,
                                       const string& valueDelim) 
    : LogFormatter(), _sep(nameSep), _midfix(valueDelim)
{ } 

NetLoggerFormatter::~NetLoggerFormatter() { } 

/**
 * copy another formatter into this one
 */
NetLoggerFormatter& NetLoggerFormatter::operator=(const NetLoggerFormatter& that)
{
    LogFormatter::operator=(that);
    _sep = that._sep;
    _midfix = that._midfix;
    return *this;
}

/**
 * write out a log record to a stream
 * @param strm   the output stream to write the record to
 * @param rec    the record to write
 */
void NetLoggerFormatter::write(ostream *strm, const LogRecord& rec) {
    const LogRecord::DataListT& data = rec.getData();
    string empty;
    for(LogRecord::DataIteratorT i=data.begin(); i != data.end(); i++) {
        write(strm, i->get(), empty);
    }
    if (data.begin() != data.end()) (*strm) << endl;
}

/**
 * write out a data property to a stream
 * @param strm   a pointer to the output stream.  The caller is 
 *                 consider the owner of the stream.
 * @param prop   a pointer to the data property to write.  The caller is 
 *                 consider the owner of the property object.
 * @param namePrefix  the name of the parent prefix to prepend to the name
 *               of this data property.
 */
void NetLoggerFormatter::write(ostream *strm, const DataProperty *prop, 
                               const string& namePrefix) 
{
    string newl("\n");

    boost::any value = prop->getValue();
    if (value.type() == typeid(DataProperty::ContainerType)) {
        typedef DataProperty::ContainerType childrenType;
        typedef DataProperty::ContainerType::const_iterator 
            childIter;

        childrenType *children = boost::any_cast<childrenType>(&value);
        
        for(childIter i=children->begin(); i!=children->end(); i++) {
            write(strm, i->get(), namePrefix + prop->getName() + _sep);
        }
    }
    else if (value.type() == typeid(int)) {
        (*strm) << "i " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<int>(value) << newl;
    }
    else if (value.type() == typeid(long)) {
        (*strm) << "l " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<long>(value) << newl;
    }
    else if (value.type() == typeid(float)) {
        (*strm) << "f " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<float>(value) << newl;
    }
    else if (value.type() == typeid(double)) {
        (*strm) << "d " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<double>(value) << newl;
    }
    else if (value.type() == typeid(bool)) {
        (*strm) << "b " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<bool>(value) << newl;
    }
    else if (prop->getName() == "DATE") {
        (*strm) << "t " << namePrefix << prop->getName()
                << _midfix << boost::any_cast<string>(value) << newl;
    }
    else if (value.type() == typeid(string)) {
        (*strm) << "s " << namePrefix << prop->getName() 
                << _midfix << boost::any_cast<string>(value) << newl;
    }
}

}}} // end lsst::mwi::logging

