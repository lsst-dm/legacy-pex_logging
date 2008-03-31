// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOGFORMATTER_H
#define LSST_PEX_LOGFORMATTER_H

#include <string>
#include <ostream>

#include "lsst/daf/base/DataProperty.h"
#include "boost/any.hpp"

namespace lsst {
namespace pex {
namespace logging {

// forward declaration
class LogRecord;
class lsst::daf::base::DataProperty;

using lsst::daf::base::DataProperty;
using std::string;
using std::ostream;

/**
 * @brief  an abstract class for formatting log records into a text stream.
 * 
 * Implementations of this class actually write log messages to a stream 
 * in a particular format.  The messages come in as LogRecord objects, which 
 * stores its data as a list of DataProperty objects.  The formatter can
 * expect certain properties to be given specific names.  The following are 
 * standard names:
 *
 *   Name      type     meaning 
 *    LOG      string   the name of the Log producing the message
 *    COMMENT  string   a simple text message
 *    DATE     string   the timestamp when the message was recorded in ISO 
 *                         format
 *    HOST     string   the hostname of the machine
 *    IP       string   the IP address of the host
 *    PID      int      the process id of the application 
 *    NODE     int      a logical node id in a multi-process application
 * 
 * Only LOG is guaranteed to appear.  There may be multiple COMMENT properties,
 * each with a complete thought.  
 */
class LogFormatter {
public:

    /**
     * create a base LogFormatter
     */
    LogFormatter() { }

    /**
     * create a copy of a LogFormatter
     */ 
    LogFormatter(const LogFormatter& that) { }

    /**
     * delete the formatter
     */
    virtual ~LogFormatter();  

    /**
     * copy another formatter into this one
     */
    LogFormatter& operator=(const LogFormatter& that) { return *this; }

    /**
     * write out a log record to a stream
     * @param strm   pointer to the output stream to write the record to.  If 
     *                 the pointer is null, nothing is written.  
     * @param rec    the record to write
     */
    virtual void write(ostream *strm, const LogRecord &rec) = 0;

    /**
     * a helper function for writing DataProperty values to a stream.  
     * Currently this will only handle primitive values of types int, long,
     * float, double, bool, and string.  
     * @param strm   the stream to write to 
     * @param value  the property value as a boost::any pointer.  
     * @return bool  false if the DataProperty value was of an unrecognized 
     *                 type.  
     */
    static bool writeDPValue(ostream& strm, const boost::any &value);

};

/**
 * @brief a formatter that renders records in a brief format for screen 
 * display.
 *
 * This formatter has a normal mode and a verbose mode.  In normal mode,
 * only the log name (LOG) and text messages (COMMENT) are printed.  In 
 * verbose mode, all other properties are printed as well.
 */
class BriefFormatter : public LogFormatter {
public:

    /**
     * create the formatter. 
     * @param verbose    all data property values will be printed.
     */
    explicit BriefFormatter(bool verbose=false) 
        : LogFormatter(), _doAll(verbose) 
    { }

    /**
     * create a copy of a LogFormatter
     */ 
    BriefFormatter(const BriefFormatter& that) 
        : LogFormatter(that), _doAll(that._doAll) 
    { }

    /**
     * delete the formatter
     */
    virtual ~BriefFormatter();

    /**
     * copy another formatter into this one
     */
    BriefFormatter& operator=(const BriefFormatter& that) { 
        _doAll = that._doAll;
        return *this; 
    }

    /**
     * return true if all data properties will be printed or false if 
     * just the Log name ("LOG") and the text comment ("COMMENT") will 
     * be printed.
     */
    bool isVerbose() { return _doAll; }

    /**
     * set whether all data properties will be printed or
     * just the Log name ("LOG") and the text comment ("COMMENT").
     * @param printAll   true if all properties should be printed.
     */
    void setVerbose(bool printAll) { _doAll = printAll; }

    /**
     * write out a log record to a stream
     * @param strm   the output stream to write the record to
     * @param rec    the record to write
     */
    virtual void write(ostream *strm, const LogRecord& rec);

    /**
     * write out a data property to a stream
     * @param strm   a pointer to the output stream.  The caller is 
     *                 consider the owner of the stream.
     * @param prop   a pointer to the data property to write.  The caller is 
     *                 consider the owner of the property object.
     * @param namePrefix  the name of the parent prefix to prepend to the name
     *               of this data property.
     */
    void write(ostream *strm, const DataProperty *prop, 
               const string& namePrefix=0);


private:
    virtual void _write(ostream *strm, const LogRecord& rec);

    bool _doAll;
};

/**
 * \brief a formatter that renders records in a netlogger-like format.  
 * 
 * This is the format intended for use with the Event system.  
 */
class NetLoggerFormatter : public LogFormatter {
public: 
    
    /**
     * create a formatter for NetLogger-like ouput
     * @param nameSep     the string to use as the delimiter between 
     *                      hierarchical components of a name.  The default is 
     *                      ".".
     * @param valueDelim  the string to use as the delimiter between 
     *                      the name and the value.  The default is ":".
     */
    explicit NetLoggerFormatter(const string& nameSep = defaultSep,
                                const string& valueDelim = defaultValDelim);

    /**
     * create a copy
     */
    NetLoggerFormatter(const NetLoggerFormatter& that) 
        : LogFormatter(that), _sep(that._sep), _midfix(that._midfix)
    { }

    /**
     * delete the formatter
     */
    virtual ~NetLoggerFormatter();

    /**
     * copy another formatter into this one
     */
    NetLoggerFormatter& operator=(const NetLoggerFormatter& that);

    /**
     * return the string used to separate a property name and its value 
     * in the output stream.
     */
    const string& getValueDelimiter() const { return _midfix; }

    /**
     * return the string used to separate fields in a hierarchical 
     * property name on the output stream.
     */
    const string& getNameSeparator() const { return _sep; }

    /**
     * write out a log record to a stream
     * @param strm   the output stream to write the record to
     * @param rec    the record to write
     */
    virtual void write(ostream *strm, const LogRecord& rec);

    /**
     * write out a data property to a stream
     * @param strm   a pointer to the output stream.  The caller is 
     *                 consider the owner of the stream.
     * @param prop   a pointer to the data property to write.  The caller is 
     *                 consider the owner of the property object.
     * @param namePrefix  the name of the parent prefix to prepend to the name
     *               of this data property.
     */
    void write(ostream *strm, const DataProperty *prop, 
               const string& namePrefix=0);

    static const string defaultSep;
    static const string defaultValDelim;

private:
    string _sep;
    string _midfix;
};

}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_LOGFORMATTER_H
