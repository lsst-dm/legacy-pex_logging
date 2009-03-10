/**
 * @file testDebug.cc
 * @brief tests the Debug class
 */

// Set a compile-time verbosity limit for this code.  Messages with a 
// verbosity level greater than this will not be printed.
#define LSST_MAX_DEBUG 3

#include "lsst/pex/logging/Debug.h"
#include "lsst/pex/logging/DualLog.h"
#include <iostream>
#include <time.h>

using lsst::pex::logging::LogRecord;
using lsst::pex::logging::Log;
using lsst::pex::logging::DualLog;
using lsst::pex::logging::Debug;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main(int argc, char* argv[]) {
    long long t0, t1;
    int verb = LSST_MAX_DEBUG;

    cout << "overhead of time call: " 
         << (LogRecord::utcnow() - LogRecord::utcnow())/-1000 << " usecs."
         << endl;

    if (argc > 1) {
        DualLog::createDefaultLog(string(argv[1]), -2);
    }
    if (argc > 2) verb = atoi(argv[2]);

    Debug log("myapp", verb);

    t0 = LogRecord::utcnow();
    log.debug(1, "I'm starting this routine");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    log.debug(1, "I'm starting this routine");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    log.debug(2, "Testing arbitrary verbosity.");

    t0 = LogRecord::utcnow();
    log.debug<2>("Testing verbosity 2.");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    log.debug<2>("Testing verbosity 2.");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    log.debug(1, "I'm starting this routine");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    log.debug(5, "I'm starting this routine");
    t1 = LogRecord::utcnow();
    cout << "message not printed in " << (t1-t0)/1000 << " usesc" << endl;

    if (argc > 1) {
        t0 = LogRecord::utcnow();
        log.debug<2>("Testing file writing at verbosity 2.");
        t1 = LogRecord::utcnow();
        cout << "Cost of printing just to file: " << (t1-t0)/1000 
             << " usesc" << endl;
    }

    log.debug<3>("Testing verbosity 3.");
    t0 = LogRecord::utcnow();
    for (int k=0; k < 10; k++)
        log.debug<4>("Testing verbosity 4.");
    t1 = LogRecord::utcnow();
    cout << "Cost of not printing message: " << (t1-t0)/10000 << " usesc" << endl;
    log.debug<5>("Testing verbosity 5.");

}
