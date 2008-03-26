//////////////////////////////////////////////////////////////////////////////
// LogRecord.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/mwi/logging/LogRecord.h"
#include "lsst/mwi/data/DataProperty.h"
#include "lsst/mwi/utils/Trace.h"

#include <boost/shared_ptr.hpp>
#include <stdexcept>

using std::string;

// #define EXEC_TRACE  20
// static void execTrace( string s, int level = EXEC_TRACE ){
//     lsst::mwi::utils::Trace( "mwi.logging.LogFormatter", level, s );
// }

namespace lsst {
namespace mwi {
namespace logging {

using std::vector;
using lsst::mwi::data::DataProperty;
using boost::shared_ptr;

/**
 * Create a log record to be sent to a given log.  
 * @param verbosity  the loudness of the record.  If this value is 
 *                     greater than or equal to the Log's verbosity 
 *                     threshold, the message will be recorded.
 */
LogRecord::LogRecord(int threshold, int verbosity)
    : _send(threshold <= verbosity), _vol(verbosity), _data()
{ 
    if (_send) setDate();
}

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
LogRecord::LogRecord(int threshold, int verbosity, const DataListT& preamble) 
    : _send(threshold <= verbosity), _vol(verbosity), _data()
{
    if (_send) {
        for(DataIteratorT i = preamble.begin(); i != preamble.end(); i++) {
            addProperty(*i);
        }
        setDate();
        addProperty(shared_ptr<DataProperty>(new DataProperty("LEVEL", _vol)));
    }
}

/**
 * delete this log record
 */
LogRecord::~LogRecord() { }

void LogRecord::setDate() {
    struct timeval tv;      
    struct timezone tz;     
    gettimeofday(&tv,&tz);
    // _tv.tv_sec = seconds since the epoch
    // _tv.tv_usec = microseconds since tv.tv_sec

    time_t rawtime;
    struct tm timeinfo;

    char datestr[40];

    time(&rawtime);
    gmtime_r(&rawtime,&timeinfo);

    if ( 0 == strftime(datestr,39,"%Y-%m-%dT%H:%M:%S.",&timeinfo)) {
        // need system exception
        throw std::runtime_error("Failed to format time successfully");
    }
    
    string fulldate(str(format("%s%d") % string(datestr) % tv.tv_usec));

    addProperty(shared_ptr<DataProperty>(new DataProperty("DATE", fulldate)));
        
}

}}} // end lsst::mwi::logging

