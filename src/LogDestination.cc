//////////////////////////////////////////////////////////////////////////////
// LogDestination.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/mwi/logging/LogDestination.h"
#include "lsst/mwi/logging/LogRecord.h"
#include "lsst/mwi/utils/Trace.h"

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

using namespace std;

// #define EXEC_TRACE  20
// static void execTrace( string s, int level = EXEC_TRACE ){
//     lsst::mwi::utils::Trace( "mwi.logging.LogDestination", level, s );
// }


namespace lsst {
namespace mwi {
namespace logging {

using boost::shared_ptr;

/**
 * @brief create a destination with a threshold.  
 * @param strm       the output stream to send messages to.  If the pointer
 *                       id null, this LogDestination will act as a null-op
 *                       destination.
 * @param formatter  the LogFormatter to use to format the messages 
 * @param threshold  the minimum volume level required to pass a message
 *                       to the stream.  If not provided, it would be set
 *                       to 0.  
 */
LogDestination::LogDestination(ostream *strm, 
                               const shared_ptr<LogFormatter>& formatter,
                               int threshold) 
    : _threshold(threshold), _strm(strm), _frmtr(formatter)
{ }

/**
 * create a copy
 */
LogDestination::LogDestination(const LogDestination& that)
    : _threshold(that._threshold), _strm(that._strm), _frmtr(that._frmtr)
{ }

/**
 * delete this destination
 */
LogDestination::~LogDestination() { }

/**
 * copy a destination into this one
 */
LogDestination& LogDestination::operator=(const LogDestination& that) {
    _threshold = that._threshold;
    _strm = that._strm; 
    _frmtr = that._frmtr;
    return *this;
}

/**
 * record a given log record to this destinations output stream. The 
 * record will be sent to the stream attached to this class if (a)
 * there is actually an attached stream, (b) there is an attached
 * formatter, and (c) the verbosity level associated with the
 * record is equal to or greater than the threshold associated
 * with this destination. 
 * @return  true if the record was actually passed to the
 *          associated stream. 
 */
bool LogDestination::write(const LogRecord& rec) {
    if (_strm != 0 && _frmtr.get() != 0 && 
        rec.getVerbosity() >= _threshold)
    {
        _frmtr->write(_strm, rec);
        return true;
    }
    return false;
}

}}} // end lsst::mwi::logging

