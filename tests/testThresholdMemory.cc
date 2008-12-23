/**
 * @brief  This will test some LogRecord basics and LogFormatter.
 */

#include "lsst/pex/logging/threshold/Memory.h"
#include "lsst/pex/logging/LogRecord.h"
#include <iostream>
#include <sstream>

using lsst::pex::logging::threshold::Memory;
using namespace std;

#define Assert(b, m) tattle(b, m, __LINE__)

void tattle(bool mustBeTrue, const string& failureMsg, int line) {
    if (! mustBeTrue) {
        ostringstream msg;
        msg << __FILE__ << ':' << line << ":\n" << failureMsg << ends;
//        throw runtime_error(msg.str());
        cout << msg.str() << endl;
    }
}

using lsst::pex::logging::LogRecord;
namespace Threshold = lsst::pex::logging::threshold;

int main() {

    Threshold::Memory mem;

    mem.setRootThreshold(5);
    Assert(mem.getRootThreshold() == 5, "wrong root threshold");

    mem.setThresholdFor("valley.of.the.dolls", -2);
    Assert(mem.getThresholdFor("valley.of.the.dolls") == -2, 
           "wrong explicit threshold");
    Assert(mem.getThresholdFor("valley.of.the") == 5, 
           "wrong inherited threshold");

    long long t0=0L, t1=0L;
    t0 = LogRecord::utcnow();
    Assert(mem.getThresholdFor("valley.of.the.dolls.movie") == -2, 
           "wrong not-yet-set threshold");
    t1 = LogRecord::utcnow();
    cout << "got threshold in " << (t1-t0)/1000L << " usecs" << endl;

    mem.setThresholdFor("valley.of", -11);
    Assert(mem.getThresholdFor("valley.of.the") == -11, 
           "wrong new inherited threshold");

    mem.printThresholds(cout);
}
