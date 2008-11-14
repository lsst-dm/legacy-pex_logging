//////////////////////////////////////////////////////////////////////////////
// ScreenLog.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/ScreenLog.h"
#include "lsst/pex/logging/Trace.h"

#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;

// #define EXEC_TRACE  20
// static void execTrace( string s, int level = EXEC_TRACE ){
//     lsst::pex::logging::Trace( "pex.logging.ScreenLog", level, s );
// }

namespace lsst {
namespace pex {
namespace logging {

using boost::shared_ptr;

///////////////////////////////////////////////////////////
//  ScreenLog
///////////////////////////////////////////////////////////

/**
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
ScreenLog::ScreenLog(bool verbose, int threshold, 
                     const vector<shared_ptr<DataProperty> > *preamble) 
    : Log(threshold), _screen(0), _screenFrmtr(0)
{
    configure(verbose);
    if (preamble != 0) 
        _preamble.insert(_preamble.end(), 
                         preamble->begin(), preamble->end());
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

/** 
 *  copy another ScreenLog into this one
 */
ScreenLog& ScreenLog::operator=(const ScreenLog& that) {
    Log::operator=(that);
    _screen = that._screen;
    _screenFrmtr = that._screenFrmtr;
    return *this;
}

/**
 * create a new log and set it as the default Log
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
void ScreenLog::createDefaultLog(bool verbose, int threshold, 
                                 const vector<shared_ptr<DataProperty> > *preamble)
{
    Log::setDefaultLog(new ScreenLog(verbose, threshold, preamble));
}


}}} // end lsst::pex::logging

