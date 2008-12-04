// -*- lsst-c++ -*-
#ifndef LSST_PEX_TYPEINFOLESS_H
#define LSST_PEX_TYPEINFOLESS_H

#include <typeinfo>

namespace lsst {
namespace pex {
namespace logging {

/*
 * @brief  a Compare function class for use with std:map containers that use
 * type_info as a key.
 */
class TypeInfoLess {
public:

    /**
     * return true if a is "less then" b.  This implementation 
     * uses the type_info.before() function to determine the result.
     */
    bool operator()(const type_info& a, const type_info& b) {
        return a.before(b);
    }
};

}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_TYPEINFOLESS_H
