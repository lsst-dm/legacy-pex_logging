/**
 * @file FileDestination.cc
 * @author Ray Plante
 */
#include "lsst/pex/logging/FileDestination.h"

namespace lsst {
namespace pex {
namespace logging {

/*
 * delete this destination
 */
FileDestination::~FileDestination() { 
    std::ofstream *ofstrm = dynamic_cast<std::ofstream*>(_strm);
    if (ofstrm != NULL && ofstrm->is_open()) {
        try {
            ofstrm->flush();
            ofstrm->close();
        }
        catch (...) { }
    }
    delete _strm;
}




}}}
