// -*- lsst-c++ -*-
/**
 * @file enum.h
 * @brief definitions of reusable enumerations
 * @author Ray Plante
 */
#ifndef LSST_PEX_LOGGING_THRESHOLD_ENUM_H
#define LSST_PEX_LOGGING_THRESHOLD_ENUM_H

namespace lsst {
namespace pex {
namespace logging {
namespace threshold {

/**
 * A threshold value that means use the value associated with a parent 
 * Log. 
 */
enum { INHERIT = -9999 };

/**
 * A lowest, most permissive possible threshold value, allowing all 
 * messages to pass through.  This is equivalent to the INHERIT value.
 */
enum { PASS_ALL = INHERIT };

}}}} // end lsst::pex::logging::threshold

#endif // end LSST_PEX_LOG_THRESHOLD_ENUM_H
