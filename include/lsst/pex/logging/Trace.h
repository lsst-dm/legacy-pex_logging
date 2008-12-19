// -*- lsst-c++ -*-
/** \file    Trace.h
  *
  * \ingroup pex
  *
  * \brief  Class providing basic run-time trace facilities.
  *
  * \author Robert Lupton, Princeton University
  */

#if !defined(LSST_PEX_UTILS_TRACE_H)        //!< multiple inclusion guard macro
#define LSST_PEX_UTILS_TRACE_H 1

#include <iostream>
#include <string>
#include <sstream>
#include <cstdarg>
#include <boost/format.hpp>

#include "lsst/pex/logging/Debug.h"

namespace lsst {
namespace pex {
namespace logging {

#ifndef LSST_DEBUGGING_ON
#define LSST_NO_TRACE 1
#endif

#if !defined(LSST_NO_TRACE)
#  define LSST_NO_TRACE 0               //!< True => turn off all tracing
#endif

#if !defined(LSST_MAX_TRACE)
#  define LSST_MAX_TRACE -1             //!< Maximum level to trace (only works for TTrace)
#endif

/**
 * \brief  Class providing basic run-time trace facilities.
 *
 *      Tracing is controlled on a per "component" basis, where a "component" 
 *      is a name of the form aaa.bbb.ccc where aaa is the Most significant 
 *      part; for example, the utilities library might be called "utils", 
 *      the doubly-linked list "utils.dlist", and the code to destroy a 
 *      list "utils.dlist.del".
 *
 *      All tracing may be disabled by recompiling with LSST_NO_TRACE defined
 *      to be non-zero
 *
 * \see templated function TTrace, which can control the maximum acceptable
 * verbosity via the CPP symbol LSST_TRACE_MAX
 *
 * \see Component class for details on the verbosity tree which
 *      determines when a trace record will be emitted.
 */
class Trace {
public:
#if !LSST_NO_TRACE
    /**
     * Return a Trace object (which will later print if verbosity is high enough
     * for name) to which a message can be attached with <<
    Trace(const std::string& name,      //!< Name of component
          const int verbosity           //!< Desired verbosity
          ) 
    {
        Log::getDefaultLog().setThreshold(-1*verbosity);
    }
     */

    /**
     * Print fmt if verbosity is high enough for name
     *
     * fmt is actually a printf format, so you can specify %d (etc) and provide
     * the required arguments.  This format will not actually be evaluated if
     * the trace is not active, so it's more efficient than the boost::format variant
     */
    Trace(const std::string& name,      //!< Name of component
          const int verbosity,          //!< Desired verbosity
          const std::string& fmt,       //!< Message to write as a printf format
          ...
          ) 
    {
        if (-1*verbosity >= Log::getDefaultLog().getThresholdFor(name)) {
            va_list ap;
            va_start(ap, fmt);
            const int len = vsnprintf(NULL, 0, fmt.c_str(), ap) + 1; // "+ 1" for the '\0'
            va_end(ap);

            char msg[len];
            va_start(ap, fmt);
            (void)vsnprintf(msg, len, fmt.c_str(), ap);
            va_end(ap);
            
            Debug out(Log::getDefaultLog(), name);
            out.debug(verbosity, msg);
        }
    }

    /**
     * Print fmt if verbosity is high enough for name
     *
     * fmt is actually a printf format, so you can specify %d (etc) and provide
     * the required arguments.  This format will not actually be evaluated if
     * the trace is not active, so it's more efficient than the boost::format variant
     *
     * As this routine indirectly calls va_arg the value of ap is undefined upon
     * exit;  the caller should however call va_end
     */
    Trace(const std::string& name,      //!< Name of component
          const int verbosity,          //!< Desired verbosity
          const std::string& fmt,       //!< Message to write as a printf format
          va_list ap                    //!< variable arguments
          ) 
    {
        if (-1*verbosity >= Log::getDefaultLog().getThresholdFor(name)) {
            const int len = fmt.size() + 100; // guess; we can't call vsnprintf twice to get length
            char msg[len];

            (void)vsnprintf(msg, len, fmt.c_str(), ap);
            
            Debug out(Log::getDefaultLog(), name);
            out.debug(verbosity, msg);
        }
    }

    /**
     * Print msg if verbosity is high enough for name
     */
    Trace(const std::string& name,      //!< Name of component
          const int verbosity,          //!< Desired verbosity
          const boost::format& msg      //!< Message to write
          )
    {
        if (-1*verbosity >= Log::getDefaultLog().getThresholdFor(name)) {
            Debug out(Log::getDefaultLog(), name);
            out.debug(verbosity, msg.str());
        }
    }

#else
/*
    Trace(const std::string& name, const int verbosity) {}
*/
    Trace(const std::string& name, const int verbosity,
          const std::string& msg, ...) {}
    Trace(const std::string& name, const int verbosity,
          const boost::format& msg) {}

#endif

    static void setVerbosity(const std::string &name) {
        Log::getDefaultLog().setThresholdFor(name, Log::INHERIT_THRESHOLD);
    }
    static void setVerbosity(const std::string &name, const int verbosity) {
        Log::getDefaultLog().setThresholdFor(name, -1*verbosity);
    }
    static int  getVerbosity(const std::string &name) {
        return Log::getDefaultLog().getThresholdFor(name);
    }
};



} // namespace logging
} // namespace pex
} // namespace lsst
#endif    // end LSST_PEX_UTILS_TRACE_H

