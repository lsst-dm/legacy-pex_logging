// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOGRECORD_H
#define LSST_PEX_LOGRECORD_H

#include "lsst/daf/base/PropertySet.h"

#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <string>
#include <sys/time.h>

#define LSST_LP_COMMENT     "COMMENT"
#define LSST_LP_TIMESTAMP   "TIMESTAMP"
#define LSST_LP_DATE        "DATE"
#define LSST_LP_LOG         "LOG"
#define LSST_LP_LEVEL       "LEVEL"

namespace lsst {
namespace pex {
namespace logging {

using std::string;
using lsst::daf::base::PropertySet;

/**
 * @brief a container for a named data property for a LogRecord
 *
 * This light-weight container is meant to facilitate adding an arbitrary 
 * named data item to a LogRecord.  The value will be stored as a reference 
 * to original item passed in; thus, this should be used only in the same
 * scope as the arguments.
 *
 * This class is usually accessed by applications via RecData, a typedef
 * defined in Log.h.  
 */
template <class T>
class RecordProperty {
public:
    typedef T ValueType;

    /**
     * wrap a name and value.  The value will be stored as a reference to
     * original item passed in; thus, this should be used only in the same
     * scope as the arguments
     */
    RecordProperty(const string& pname, const T& pvalue) 
        : name(pname), value(pvalue) { }

    /**
     * add the name-value pair to a PropertySet
     */
    void addTo(PropertySet& set) { set.add(this->name, this->value); }

    const string name;
    const T& value;
};

/*
 * a shorthand version of the RecordProperty class
 */
template <class T>
class Prop : public RecordProperty<T> { 
public:
    Prop(const string& pname, const T& value) 
        : RecordProperty<T>(pname, value) { }
};

/**
 * @brief a container for constructing a single Log record
 *
 * The purpose of this class is to collect data for inclusion in a message
 * to a Log.  
 */
class LogRecord {
public:

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
    LogRecord(int threshold, int verbosity, const PropertySet& preamble);

    /**
     * create a copy of a record
     */
    LogRecord(const LogRecord& that) 
        : _send(that._send), _vol(that._vol), _data()
    { 
        _data = that._data->deepCopy();
    }   

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
     * add a string comment to this record.  The comment will get stored 
     * in the data property under the key name, "COMMENT".  The comment will 
     * only get added when willRecord() is returns true which is set when
     * the record is constructed (usually by a Log object).  
     */
    void addComment(const string& comment) {
        if (_send) _data->add(LSST_LP_COMMENT, comment);
    }

    /**
     * add a string comment to this record.  This version is provided 
     * as a convenience and is equivalent to addComment(comment.str()).  
     */
    void addComment(const boost::format& comment) {
        if (_send) addComment(comment.str());
    }

    /**
     * attach a named item of data to this record.
     */
    template <class T>
    void addProperty(const RecordProperty<T>& property);

    /**
     * attach a named item of data to this record.
     */
    template <class T>
    void addProperty(const string& name, const T& val);

    /**
     * add all of the properties found in the given PropertySet.  
     * This will make sure not to overwrite critical properties, 
     * LEVEL, LOG, TIMESTAMP, and DATE.  
     */
    void addProperties(const PropertySet& props);

    /**
     * add all of the properties found in the given PropertySet.  
     * This will make sure not to overwrite critical properties, 
     * LEVEL, LOG, TIMESTAMP, and DATE.  
     */
    void addProperties(const PropertySet::Ptr& props) {
        addProperties(*props);
    }

    /**
     * return the read-only data properties that make up this log message.
     * This is a synonym for data().
     */
    const PropertySet& getProperties() const { return data(); }

    /**
     * return the data properties that make up this log message.  
     * This is a synonym for data().
     */
    PropertySet& getProperties() { return data(); }

    /**
     * return the data properties that make up this log message.  
     * This is a synonym for getProperties().
     */
    const PropertySet& data() const { return *_data; }

    /**
     * return the data properties that make up this log message.  
     * This is a synonym for getProperties().
     */
    PropertySet& data() { return *_data; }

    /**
     * return the number available property parameter names (i.e. ones 
     * that return non-PropertySet values). 
     */
    size_t countParamNames() {
        std::vector<string> names = data().paramNames(false);
        return names.size();
    }

    /**
     * return the total number of data property values currently contained 
     * in this log record.  This function will sum the number values 
     * associated with a name, summed over all available names (including 
     * subproperties).  This total does not include PropertySet values.  
     */
    size_t countParamValues() const;

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

    /**
     * set the TIMESTAMP property to the current time.  The value is stored as 
     * a lsst::daf::base::DateTime instance.  
     * 
     * This is called automatically in the constructor, but it can be reset 
     * at any time.  If there is also a DATE property, it will be updated 
     * as well (see setDate()). 
     */  
    void setTimestamp();

    /**
     * set the DATE property to the current value of the TIMESTAMP property.
     * The value is a string representation of the TIMESTAMP property, 
     * formatted for read-able display.
     *
     * Unlike setTimestamp(), this function is not called automatically within 
     * the LogRecord constructor; thus, a user of this class must call it 
     * explicitly to have this property.  
     *
     * This function is intended for use by a Log class that sets the value
     * just before sending this record to the LogDestinations.
     */
    virtual void setDate();

protected: 
    LogRecord() : _send(false), _vol(10), _data(new PropertySet()) { }

    /**
     * initialize this record with the DATE and LEVEL properties
     */
    void _init() {
        if (_send) {
            _data->set("LEVEL", _vol);
            setDate();
        }
    }

    bool _send;  // true if this record should be sent to the log
    int _vol;    // the verbosity volume of this message
    PropertySet::Ptr _data;
};

template <class T>
void LogRecord::addProperty(const RecordProperty<T>& property) {
    if (_send) property.addTo(_data);
}

template <class T>
void LogRecord::addProperty(const string& name, const T& val) {
    if (_send) data().add(name, val);
}



}}} // end lsst::pex::logging

#endif // end LSST_PEX_LOGRECORD_H
