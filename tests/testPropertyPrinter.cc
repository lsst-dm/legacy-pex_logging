#include "lsst/pex/logging/PropertyPrinter.h"
#include <iostream>
#include <vector>
#include <stdexcept>

using lsst::pex::logging::PropertyPrinter;
using lsst::daf::base::PropertySet;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main() {

    PropertySet ps;
    ps.set("count", 4);
    ps.set("done", true);
    ps.set("name", "Ray");

    std::vector<std::string>::iterator ni;
    std::vector<std::string> names(ps.paramNames(true));

    for(ni = names.begin(); ni != names.end(); ++ni) {
        PropertyPrinter pp(ps, *ni);
        for(PropertyPrinter::iterator i=pp.begin(); i.notAtEnd(); ++i) {
            cout << *ni << ": ";
            i.write(&cout) << endl;
        }
    }

    return 0;
}
    
