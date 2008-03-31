#include "lsst/pex/logging/LogDestination.h"
#include "lsst/pex/logging/LogRecord.h"
#include <iostream>
#include <boost/shared_ptr.hpp>

using lsst::pex::logging::LogRecord;
using lsst::pex::logging::LogFormatter;
using lsst::pex::logging::LogDestination;
using lsst::pex::logging::BriefFormatter;
using lsst::daf::base::DataProperty;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main() {

    typedef boost::shared_ptr<DataProperty> sharedPtrT;
    sharedPtrT dp1(new DataProperty("HOST", string("localhost.localdomain")));
    sharedPtrT dp2(new DataProperty("IP", string("111.111.111.111")));
    sharedPtrT dp3(new DataProperty("LOG", string("tester")));

    LogRecord::DataListT preamble;
    preamble.push_back(dp1);
    preamble.push_back(dp2);
    preamble.push_back(dp3);

    LogRecord lr1(1, 5, preamble);
    LogRecord lr2(5, 1, preamble);

    lr1.addComment("This is a test");
    lr2.addComment("This is a test");

    boost::shared_ptr<LogFormatter> brief(new BriefFormatter());

    LogDestination shy(&cout, brief, 10);
    cout << "Shy: " << endl;
    shy.write(lr1);
    cout << "-------------" << endl;
    shy.write(lr2);
    cout << "=============" << endl;

    LogDestination chatty(&cout, brief);
    cout << "Chatty: " << endl;
    chatty.write(lr1);
    cout << "-------------" << endl;
    chatty.write(lr2);
    cout << "=============" << endl;

}
