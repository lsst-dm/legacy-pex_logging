// -*- lsst-c++ -*-
/**
  * \file logExample.cc
  *
  * \brief logExample.c demonstrates simple use of the Log facility.
  */

// Set a compile-time verbosity limit for this code.  Messages with a 
// verbosity level greater than this will not be printed.
#define LSST_MAX_DEBUG 3

#include "lsst/pex/logging/Debug.h"
#include "boost/format.hpp"
#include <fstream>

using namespace std;
using lsst::pex::logging::Log;
using lsst::pex::logging::Rec;
using lsst::pex::logging::Prop;
using lsst::pex::logging::Debug;
using lsst::pex::logging::debug;

/** \brief Demonstrate the recording debugging messages
  *
  * Application developers want a low impact way to include debugging 
  * messages in their code that doesn't hurt performance, particularly 
  * in "production mode" when the messages are not being displayed. 
  * The logging module provides such message that captures debugging 
  * messages into the general logging streams.  
  *
  * See the documentation for the Log class for more information and example
  * usage.  
  * 
  * See also note at the top of this file for turning off debug messages
  */
int main(int argc, char *argv[]) {

    // any function that wishes to have debug messages should
    // declare a log with a given name.
    Debug dblog("myapp");

    // Simple messages can use DEBUG(); these will have a verbosity
    // level of 1 and, thus, will be printed.
    // 
    dblog.debug<1>("I'm starting this routine");

    // formatted messages are supported as well
    //
    dblog.debug<3>("Starting iteration #%i", 100);

    // this message will not get printed as it exceeds LSST_MAX_DEBUG
    //
    dblog.debug<5>("This is too verbose to be seen");

    // Of course you can call any Log or Log-related functions explicitly.
    // The filtering, however, is done a run-time, and thus is less 
    // efficient.  Note that this will not get printed.
    Rec(dblog, Log::DEBUG) << "starting iterations" 
                           << Prop<double>("rms", 3.2) << Rec::endr;

    // If you want to attach properties but with different verbosity levels,
    // multiply the verbosity level by -1:
    Rec(dblog, -2) << "starting iterations" 
                   << Prop<double>("rms", 3.2) << Rec::endr;

    // if your routine will only have print one debug message with a given 
    // log name, you can use this technique, which is slightly more 
    // efficient (since a log object is only created if the compile-time 
    // limit is not exceeded).  If you expect to print at least two debug 
    // messages that will be satisfy the limit, the above technique is more 
    // efficient.
    debug<2>("myapp.func", "calling the function: %s", "func");

}
