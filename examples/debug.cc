// -*- lsst-c++ -*-
/**
  * \file logExample.cc
  *
  * \brief logExample.c demonstrates simple use of the Log facility.
  */

// If you want to turn off debug messages, uncomment this define before
// you include any LSST includes.
//
// #define LSST_NO_DEBUG

#include "lsst/pex/logging/Debug.h"
#include "boost/format.hpp"
#include <fstream>

using namespace std;
using lsst::pex::logging::Log;
using lsst::pex::logging::Rec;
using lsst::pex::logging::Prop;

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
    // declare the log name
    DEBUGLOG("myapp", 1);

    // Simple messages can use DEBUG(); these will have a verbosity
    // level of 1.
    // 
    DEBUG("I'm starting this routine");

    // You can use boost:;format without concern for performance
    DEBUG(boost::format("This is execution number %i") % 5);

    // Use of DEBUGN() to set an arbitrary verbosity level
    DEBUGN(3, "This is a nitpic");

    // Same as above; just a convenience
    DEBUG3("The same level of nitpicing");

    // The previous DEBUGLOG call defined a default variable __debugLog__
    // For the Debug log.  If you want to write to more than one log topic
    // name, use MDEBUGLOG that let's you name the variables yourself. 
    // 
    MDEBUGLOG(dbl, "myapp.subroutine", 2);
    MDEBUG(dbl, "okay");

    // Of course you can call any Log or Log-related functions explicitly,
    // just be sure to wrap it in a preprocessor check for LSST_DEBUGGING_ON.
#ifdef LSST_DEBUGGING_ON
    Rec(dbl, Log::DEBUG) << Prop<double>("rms", 3.2) << Rec::endr;
#endif
}
