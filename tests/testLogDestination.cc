#include "lsst/pex/logging/LogDestination.h"
#include "lsst/pex/logging/LogRecord.h"
#include <iostream>
#include <boost/shared_ptr.hpp>

using lsst::pex::logging::LogRecord;
using lsst::pex::logging::LogFormatter;
using lsst::pex::logging::LogDestination;
using lsst::pex::logging::BriefFormatter;
using lsst::daf::base::PropertySet;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main() {

    PropertySet preamble;
    preamble.add("HOST", string("localhost.localdomain"));
    preamble.add("IP", string("111.111.111.111"));
    preamble.add("LOG", string("tester"));

    LogRecord lr1(1, 5, preamble);
    LogRecord lr2(5, 1, preamble);

    lr1.addComment("This is a test");
    lr2.addComment("This is a test");

    boost::shared_ptr<LogFormatter> brief(new BriefFormatter());

    LogDestination shy(&cerr, brief, 10);
    cerr << "Shy: " << endl;
    shy.write(lr1);
    cerr << "-------------" << endl;
    shy.write(lr2);
    cerr << "=============" << endl;

    LogDestination chatty(&cerr, brief);
    cerr << "Chatty: " << endl;
    chatty.write(lr1);
    cerr << "-------------" << endl;
    chatty.write(lr2);
    cerr << "=============" << endl;

}
