// -*- lsst-c++ -*-
/**
  * \file logExample.cc
  *
  * \brief logExample.c demonstrates simple use of the Log facility.
  */

#include "lsst/pex/logging/Log.h"
#include "lsst/pex/logging/DualLog.h"
#include <fstream>

using namespace std;
using namespace lsst::pex::logging;
using namespace lsst;
using lsst::pex::logging::Log;
using lsst::pex::logging::Rec;

/** \brief Demonstrate the Log class.   
  *
  * See the documentation for the Log class for more information and example
  * usage.  
  */
int main(int argc, char *argv[]) {

    // to get a root logger, you usually grab the default Log object:
    Log root = Log::getDefaultLog();

    // for simple messages just use the log function.  If the versbosity 
    // threshold is not low enough, this message will be ignored.
    root.log(Log::INFO, "I'm writing a message.");

    // you can use the boost::format object for more complex messages; 
    // however, be careful: you still pay the cost of the format call
    root.log(Log::INFO, 
             boost::format("Verbosity threshold: %d") % root.getThreshold());

    // You can create your own log, if you wish, for testing purposes.
    // This log prints both to the screen and a log file, each with different
    // verbosities
    int fileThreshold = Log::DEBUG;
    int screenThreshold = Log::INFO;
    DualLog myOwnLog("example.log", fileThreshold, screenThreshold);
    myOwnLog.log(Log::DEBUG, "This will only go to the file");
    myOwnLog.log(Log::WARN, "You will see this on the screen");

    // you can create a named child Log from this one.  This one will be 
    // quieter, printing only warnings and failures.  
    Log child(myOwnLog, "myapp", Log::WARN);
    child.log(Log::INFO, "This one won't be printed in either destination.");

    // Logs have hierarchical relationships, inheriting destinations and 
    // verbosity thresholds.
    Log grandchild(child, "mysubcomponent");
    grandchild.log(Log::INFO, "We still won't see this message.");
    grandchild.setThreshold(Log::INFO);
    grandchild.log(Log::INFO, "But now we will see this in the file.");

    // Note that any future child log with the same name will remember its 
    // threshold, even if the original is destroyed.  Note that this one is 
    // 2 levels down from its root.
    Log anothergc(myOwnLog, "myapp.mysubcomponent");
    anothergc.log(Log::INFO, "Again, a file-only message");

    // You can also send DataProperties to the log.  For this, use the LogRec
    // interface:
    Rec(child, Log::WARN) << "No convergence reached"
                          << Prop<int>("iterations", 541)
                          << Prop<float>("rms", 0.0032)
                          << Rec::endr;

    return 0;
}
