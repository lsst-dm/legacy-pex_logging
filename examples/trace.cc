/*
 * NOTE:  Use of Trace is now deprecated.  This file demonstrates use of 
 * remaining Trace capabilities provided for backward-compatibility.  
 * 
 */

//#define LSST_NO_TRACE 1                 // define to compile out tracing

#define LSST_MAX_TRACE 5                // the maximum trace level that'll be compiled in

#include "lsst/pex/logging/Trace.h"

using namespace lsst::pex::logging;

namespace {
void work() {
    std::cout << "\nThreshold levels:\n";
    Trace::printVerbosity(std::cout);
    std::cout << "traces:\n";

    Trace("foo", 1, "foo 1");
    Trace("foo.bar.goo", 4, "foo.bar.goo 4");
    Trace("foo.bar.goo", 4, boost::format("foo.bar.goo %d") % 4);
    Trace("foo.bar.goo.hoo", 3, "foo.bar.goo.hoo %d", 3);

}
}

int main() {
    Trace::setVerbosity(".", 100);
    work();

    Trace::setVerbosity(".", 0);
    Trace::setVerbosity("foo.bar", 3);
    Trace::setVerbosity("foo.bar.goo", 10);
    Trace::setVerbosity("foo.tar", 5);
    work();

    Trace::setVerbosity("foo.tar");
    Trace::setVerbosity("foo.bar");
    work();
    
    std::cout << "\nReset.";
    Trace::reset();
    work();

    Trace::setVerbosity("", 1);
    Trace::setVerbosity("foo.bar.goo.hoo", 10);
    work();

    Trace::setVerbosity("", 2);
    work();

    Trace::setVerbosity("");
    Trace::setVerbosity("foo.bar.goo.hoo");
    Trace::setVerbosity("foo.bar.goo.hoo.joo", 10);
    Trace::setVerbosity("foo.bar.goo", 3);
    work();
    
    return 0;
}
