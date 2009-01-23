#include "lsst/pex/logging/Log.h"
#include "lsst/pex/logging/ScreenLog.h"
#include <iostream>
#include <exception>
#include <boost/shared_ptr.hpp>

using lsst::pex::logging::Log;
using lsst::daf::base::Citizen;

int main() {
    
    if (true) {
        Log dlog = Log::getDefaultLog();
    }

    if (Citizen::census(0) != 0) {
        Citizen::census(std::cout);
        throw std::runtime_error("Logging left active Citizen");
    }

}
