//////////////////////////////////////////////////////////////////////////////
// DualLog.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/DualLog.h"
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
//  DualLog
///////////////////////////////////////////////////////////

/**
 * create a Log that will write messages to a given file
 * @param filename      the name of the file to send messages to.  It will 
 *                        be opened in append mode.
 * @param node          the logical node number for this process.  This is 
 *                        used to identify the current parallel process 
 *                        within a parallel, multi-process application.  
 *                        The default is 0.  
 * @param filethresh    the verbosity threshold to set for the log file
 * @param screenthresh  the verbosity threshold to set for messages going
 *                        to the screen.
 * @param screenVerbose if true, all message data properties will be printed
 *                        to the screen.  If false, only the Log name 
 *                        ("LOG") and the text comment ("COMMENT") will be
 *                        printed.
 */
DualLog::DualLog(const string& filename, int filethresh, int screenthresh,
                 bool screenVerbose, 
                 const vector<shared_ptr<DataProperty> > *preamble) 
    : ScreenLog(screenthresh, screenVerbose, preamble)
{
    // the DualLog destructor will close & destroy this.
    fstrm = new ofstream(filename.c_str(), ios_base::app);
    
    shared_ptr<LogFormatter> fmtr(new NetLoggerFormatter());

    // note that the shared_ptr held by LogDestination list will 
    // handle the deletion of this pointer.    
    _file = new LogDestination(fstrm, fmtr, INHERIT_THRESHOLD);
    shared_ptr<LogDestination> dest(_file);
    _destinations.push_back( dest );
}

DualLog::~DualLog() { 
    fstrm->close();
    delete fstrm;
}

void DualLog::createDefaultLog(const string& filename, int filethresh, 
                               int screenthresh, bool screenVerbose,
                               const vector<shared_ptr<DataProperty> > *pream)
{
    Log::setDefaultLog(new DualLog(filename, filethresh, 
                                   screenthresh, screenVerbose,pream));
}


}}} // end lsst::pex::logging

