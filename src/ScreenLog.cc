//////////////////////////////////////////////////////////////////////////////
// ScreenLog.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/ScreenLog.h"

#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;

namespace lsst {
namespace pex {
namespace logging {

using boost::shared_ptr;

///////////////////////////////////////////////////////////
//  ScreenLog
///////////////////////////////////////////////////////////

/*
 * create a Log that will write messages to a given file
 * @param threshold     the verbosity threshold to set for messages going
 *                        to the screen.
 * @param verbose     if true, all message data properties will be printed
 *                        to the screen.  If false, only the Log name 
 *                        ("LOG") and the text comment ("COMMENT") will be
 *                        printed.
 * @param preamble       a list of data properties that should be included 
 *                         with every recorded message to the Log.  This
 *                         constructor will automatically add a property 
 *                         ("LOG") giving the Log name.  
 */
ScreenLog::ScreenLog(const PropertySet& preamble, bool verbose, int threshold) 
    : Log(threshold), _screen(0), _screenFrmtr(0)
{
    configure(verbose);
    _preamble->combine(preamble.deepCopy());
}

/*
 * create a Log that will write messages to a given file
 * @param threshold     the verbosity threshold to set for messages going
 *                        to the screen.
 * @param verbose     if true, all message data properties will be printed
 *                        to the screen.  If false, only the Log name 
 *                        ("LOG") and the text comment ("COMMENT") will be
 *                        printed.
 * @param preamble       a list of data properties that should be included 
 *                         with every recorded message to the Log.  This
 *                         constructor will automatically add a property 
 *                         ("LOG") giving the Log name.  
 */
ScreenLog::ScreenLog(bool verbose, int threshold) 
    : Log(threshold), _screen(0), _screenFrmtr(0)
{
    configure(verbose);
}

void ScreenLog::configure(bool verbose) {
    // note that the shared_ptr held by the screen LogDestination will 
    // handle the deletion of this pointer.
    _screenFrmtr = new BriefFormatter(verbose);
    shared_ptr<LogFormatter> fmtr(_screenFrmtr);

    // note that the shared_ptr held by LogDestination list will 
    // handle the deletion of this pointer.    
    _screen = new LogDestination(&clog, fmtr, INHERIT_THRESHOLD);
    shared_ptr<LogDestination> dest(_screen);
    _destinations.push_back( dest );
}

ScreenLog::~ScreenLog() { }

/*
 *  copy another ScreenLog into this one
 */
ScreenLog& ScreenLog::operator=(const ScreenLog& that) {
    if (this == &that) return *this;

    dynamic_cast<Log*>(this)->operator=(that);
    _screen = that._screen;
    _screenFrmtr = that._screenFrmtr;
    return *this;
}

/**
 * create a new log and set it as the default Log
 * @param preamble       a list of data properties that should be included 
 *                         with every recorded message to the Log.  This
 *                         constructor will automatically add a property 
 *                         ("LOG") giving the Log name.  
 * @param threshold     the verbosity threshold to set for messages going
 *                        to the screen.
 * @param verbose     if true, all message data properties will be printed
 *                        to the screen.  If false, only the Log name 
 *                        ("LOG") and the text comment ("COMMENT") will be
 *                        printed.
 */
void ScreenLog::createDefaultLog(const PropertySet& preamble, 
                                 bool verbose, int threshold)
{
    Log::setDefaultLog(new ScreenLog(preamble, verbose, threshold));
}

/*
 * create a new log and set it as the default Log
 * @param preamble       a list of data properties that should be included 
 *                         with every recorded message to the Log.  This
 *                         constructor will automatically add a property 
 *                         ("LOG") giving the Log name.  
 * @param threshold     the verbosity threshold to set for messages going
 *                        to the screen.
 * @param verbose     if true, all message data properties will be printed
 *                        to the screen.  If false, only the Log name 
 *                        ("LOG") and the text comment ("COMMENT") will be
 *                        printed.
 */
void ScreenLog::createDefaultLog(bool verbose, int threshold) {
    Log::setDefaultLog(new ScreenLog(verbose, threshold));
}


}}} // end lsst::pex::logging

