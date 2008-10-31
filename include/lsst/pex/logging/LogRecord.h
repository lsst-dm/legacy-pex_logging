// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOGRECORD_H
#define LSST_PEX_LOGRECORD_H

#include "lsst/daf/base/DataProperty.h"

#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <string>
#include <sys/time.h>


namespace lsst {
namespace pex {
namespace logging {

using std::list;
using std::string;
using boost::shared_ptr;
using lsst::daf::base::DataProperty;

/**
 * @brief a container for constructing a single Log record
 *
 * The purpose of this class is to collect data for inclusion in a message
 * to a Log.  
 */
class LogRecord {
public:

    typedef list<shared_ptr<DataProperty> > DataListT;
    typedef DataListT::const_iterator DataIteratorT;

    /**
     * Create a log record to be sent to a given log.  The current time is 
     * recorded and set as the DATE property.
     * @param threshold  the verbosity threshold that determines if a message
     *                     is printed.
     * @param verbosity  the loudness of the record.  If this value is 
     *                     greater than or equal to the given verbosity 
     *                     threshold, the message will be recorded.
     */
    LogRecord(int threshold, int verbosity);

    /**
     * Create a log record to be sent to a given log.  The current time is 
     * recorded and set as the DATE property.
     * @param threshold  the verbosity threshold that determines if a message
     *                     is printed.
     * @param verbosity  the loudness of the record.  If this value is 
     *                     greater than or equal to the given verbosity 
     *                     threshold, the message will be recorded.
     * @param preamble   an ordered set of properties that constitute the 
     *                     preamble of this message.  This should not include
     *                     the current time.  
     */
    LogRecord(int threshold, int verbosity, const DataListT& preamble);

    /**
     * create a copy of a record
     */
    LogRecord(const LogRecord& that) 
        : _send(that._send), _vol(that._vol), _data(that._data)
    { }   

    /**
     * delete this log record
     */
    virtual ~LogRecord();

    /**
     * copy another record into this one.
     */
    LogRecord& operator=(const LogRecord& that) {
        _send = that._send; 
        _vol = that._vol; 
        _data = that._data;
        return *this;
    }

    /**
     * add a string comment to this message.  This will only happen if
     * 
     */
    void addComment(const string& comment) {
        if (_send) 
            _data.push_back(
                shared_ptr<DataProperty>(new DataProperty("COMMENT", comment)));
    }

    /**
     * add a string comment to this message
     */
    void addComment(const boost::format& comment) {
        if (_send) addComment(comment.str());
    }

    /**
     * add a data property to this message
     */
    void addProperty(const DataProperty& prop) {
        if (_send) 
            _data.push_back(shared_ptr<DataProperty>(new DataProperty(prop)));
    }

    /**
     * add a data property to this message
     */
    void addProperty(const shared_ptr<DataProperty> prop) {
        if (_send) _data.push_back(prop);
    }

    /**
     * return the ordered list of data properties that make up this 
     * log message.
     */
    const DataListT& getData() const { return _data; }

    /**
     * return the number of data properties currently contained in this 
     * log record.
     */
    int getDataCount() const { return _data.size(); } 

    /**
     * return the verbosity level--a measure of "loudness"--associated with 
     * this record.  
     */
    int getVerbosity() const { return _vol; }

    /**
     * return true if the verbosity level is set loud enough to cause 
     * this record to be actually recorded.  This value is set at
     * construction time only.  
     */
    bool willRecord() const { return _send; }

protected: 
    LogRecord() : _send(false), _vol(10), _data() { }

    /**
     * set the current timestamp as the DATE property.
     */  
    void setDate();

    bool _send;  // true if this record should be sent to the log
    int _vol;    // the verbosity volume of this message
    DataListT _data;
};

}}} // end lsst::pex::logging

#endif // end LSST_PEX_LOGRECORD_H
