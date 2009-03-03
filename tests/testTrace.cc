#define LSST_MAX_TRACE 3

#include "lsst/pex/logging/Trace.h"
#include "lsst/pex/logging/DualLog.h"
#include <iostream>
#include <time.h>

using lsst::pex::logging::LogRecord;
using lsst::pex::logging::Log;
using lsst::pex::logging::DualLog;
using lsst::pex::logging::Trace;
namespace lg = lsst::pex::logging;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main(int argc, char* argv[]) {
    long long t0, t1;
    int verb = 3;

    cout << "overhead of time call: " 
         << (LogRecord::utcnow() - LogRecord::utcnow())/-1000 << " usecs."
         << endl;

    if (argc > 1) {
        DualLog::createDefaultLog(string(argv[1]), -2);
    }
    if (argc > 2) verb = atoi(argv[2]);

    Trace::setVerbosity("myapp", verb);

    t0 = LogRecord::utcnow();
    Trace("myapp", 1, "I'm starting this routine");
    t1 = LogRecord::utcnow();
    cout << "message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    Trace("myapp.foo.bar", 2, "Testing arbitrary verbosity.");
    t1 = LogRecord::utcnow();
    cout << "Trace message printed in " << (t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    Trace("myapp.foo.bar", 5, "Testing arbitrary verbosity.");
    t1 = LogRecord::utcnow();
    cout << "Trace message not printed in " <<(t1-t0)/1000 << " usesc" << endl;

    t0 = LogRecord::utcnow();
    lg::TTrace<2>("myapp", "Testing arbitrary verbosity again.");
    t1 = LogRecord::utcnow();
    cout << "TTrace message printed in " << (t1-t0)/1000 << " usesc" << endl;
    
    t0 = LogRecord::utcnow();
    lg::TTrace<2>("myapp", "Testing arbitrary verbosity again.");
    t1 = LogRecord::utcnow();
    cout << "TTrace message printed in " << (t1-t0)/1000 << " usesc" << endl;
    

    if (argc > 1) {
        t0 = LogRecord::utcnow();
        lg::TTrace<2>("myapp", "Testing file writing at verbosity 2.");
        t1 = LogRecord::utcnow();
        cout << "Cost of printing to file: " << (t1-t0)/1000 
             << " usesc" << endl;
    }

    Trace("myapp.foo.bar", 4, "Testing verbosity past limit.");

    t0 = LogRecord::utcnow();
    for (int k=0; k < 10; k++)
        lg::TTrace<4>("myapp", "Testing verbosity past limit again.");
    t1 = LogRecord::utcnow();
    cout << "TTrace message not printed in " << (t1-t0)/10000 << " usesc" <<endl;
    

}
