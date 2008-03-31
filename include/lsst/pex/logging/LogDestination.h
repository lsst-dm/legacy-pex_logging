// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOGDESTINATION_H
#define LSST_PEX_LOGDESTINATION_H

#include "lsst/pex/logging/LogFormatter.h"

#include <string>
#include <ostream>
#include <boost/shared_ptr.hpp>

namespace lsst {
namespace pex {
namespace logging {

using std::ostream;
using std::string;
using boost::shared_ptr;

// forward declaration of LogRecord
class LogRecord;

/**
 * @brief an encapsulation of a logging stream that will filter messages
 * based on their volume (verbosity) level.  
 *
 * Multiple destinations can be added to a Log either at its
 * contruction time or later using one of its addDestination()
 * methods.  A LogDestination can be added to multiple Logs; however,
 * actual writing to streams is not synchronized, so the output will
 * get garbled if multiple threads or processes write to the same
 * stream simultaneously.
 * 
 * A LogDestination has its own verbosity threshold associated with it that 
 * is in addition to a Log's threshold.  If the threshold of a destination
 * is higher than that associated with its Log, then the destination threshold
 * will override the Log's in preventing message from being recorded.  This 
 * allows some destinations to be more verbose than others.  
 */
class LogDestination {
public:

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
    LogDestination(ostream *strm, 
                   const shared_ptr<LogFormatter>& formatter, 
                   int threshold=0);

    /**
     * create a copy
     */
    LogDestination(const LogDestination& that);

    /**
     * delete this destination
     */
    virtual ~LogDestination();

    /**
     * copy a destination into this one
     */
    LogDestination& operator=(const LogDestination& that);

    /**
     * return the verbosity threshold associated with this stream.  
     */
    int getThreshold() const { return _threshold; }

    /**
     * set the verbosity threshold associated with this stream.  If this 
     * threshold is higher than the threshold associated with the 
     * LogDestination's Log, it will override it.
     */
    void setThreshold(int threshold) { _threshold = threshold; }

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
    bool write(const LogRecord& rec);

protected:
    int _threshold;   // the stream's threshold
    ostream *_strm;   // the output stream
    boost::shared_ptr<LogFormatter> _frmtr;    // the formatter to use
};

}}}     // end lsst::pex::logging

#endif  // LSST_PEX_LOGDESTINATION_H
