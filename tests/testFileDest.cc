#include "lsst/pex/logging/Log.h"
#include "lsst/pex/logging/FileDestination.h"
#include <iostream>
#include <boost/shared_ptr.hpp>

using lsst::pex::logging::Log;
using lsst::pex::logging::FileDestination;
using lsst::pex::logging::LogDestination;
using lsst::pex::logging::LogFormatter;
using lsst::pex::logging::IndentedFormatter;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main() {

    std::string filepath("tests/testFileDestination-out.txt");
    int threshold = 0;
    lsst::pex::logging::LogDestination *ld = new lsst::pex::logging::FileDestination(filepath, threshold);
    delete ld;

    boost::shared_ptr<LogFormatter> brief(new IndentedFormatter());

    boost::shared_ptr<LogDestination> 
        shy(new FileDestination("tests/testFileDestination-out.txt"));
    Log log;
    log.addDestination(shy);
    log.log(Log::INFO, "hello");
}
