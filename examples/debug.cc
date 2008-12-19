// -*- lsst-c++ -*-
/**
  * \file logExample.cc
  *
  * \brief logExample.c demonstrates simple use of the Log facility.
  */

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
  */
int main(int argc, char *argv[]) {

    DEBUGLOG("myapp", 1);
    DEBUG("I'm starting this routine");
    DEBUG(boost::format("This is execution number %i") % 5);
    DEBUGN(3, "This is a nitpic");
    DEBUG3("The same level of nitpicing");

    MDEBUGLOG(dbl, "myapp.subroutine", 2);
    MDEBUG(dbl, "okay");

#ifdef LSST_DEBUGGING_ON
    Rec(dbl, Log::DEBUG) << Prop<double>("rms", 3.2) << Rec::endr;
#endif
}
