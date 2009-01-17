// -*- lsst-c++ -*-
/** \file    Trace.h
  *
  * \ingroup pex
  *
  * \brief  This class provides limited backward compatibility to the 
  * DC2 run-time trace facilities 
  *
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
#  define LSST_NO_TRACE 0    //!< True => turn off all tracing
#endif

#if !defined(LSST_MAX_TRACE)
#  define LSST_MAX_TRACE -1  //!< Maximum level to trace (only works for TTrace)
#endif

/**
 * \brief  limited backward compatibility to the 
 * DC2 run-time trace facilities 
 *
 * This class replaces the original Trace facility and is provided for 
 * backward-compatibility.  This fully in-lined implementation sends Trace
 * calls to the logging framework.
 *
 * Developers should prefer the use of DEBUG macros in available via 
 * Debug.h in the future which sends debugging messages through the 
 * logging framework.
 *
 * From the original Trace.h (by Robert Lupton):
 * 
 * \verbatim
 *      Tracing is controlled on a per "component" basis, where a "component" 
 *      is a name of the form aaa.bbb.ccc where aaa is the Most significant 
 *      part; for example, the utilities library might be called "utils", 
 *      the doubly-linked list "utils.dlist", and the code to destroy a 
 *      list "utils.dlist.del".
 *
 *      All tracing may be disabled by recompiling with LSST_NO_TRACE defined
 *      to be non-zero
 * \endverbatim
 *
 * Old Trace capabilities not supported were chose based on current use
 * at the time this class was developed.  These include
 * \verbatim
 *   o  stream printing to Trace objects via << operator
 *   o  TTrace templated functions
 * \endverbatim
 */
class Trace {
public:
#if !LSST_NO_TRACE

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

    /// set where logging is sent; ignored in new implementation
    static void setDestination(std::ostream &) {
    }

    static void setVerbosity(const std::string &name) {
        if (name.length() == 0 || name == ".") 
            Log::getDefaultLog().setThreshold(Log::INHERIT_THRESHOLD);
        else
            Log::getDefaultLog().setThresholdFor(name, Log::INHERIT_THRESHOLD);
    }
    static void setVerbosity(const std::string &name, const int verbosity) {
        if (name.length() == 0 || name == ".") 
            Log::getDefaultLog().setThreshold(-1*verbosity);
        else
            Log::getDefaultLog().setThresholdFor(name, -1*verbosity);
    }
    static int getVerbosity(const std::string &name) {
        if (name.length() == 0 || name == ".") 
            return Log::getDefaultLog().getThreshold();
        else 
            return Log::getDefaultLog().getThresholdFor(name);
    }
    static void printVerbosity(std::ostream& out) {
        Log::getDefaultLog().printThresholds(out);
    }
    static void reset() {
        Log::getDefaultLog().reset();
    }
};

/************************************************************************************************************/

#include <boost/preprocessor/iteration/local.hpp>

/*
 * Define a macro to generate definitions of TTRace, or needed partial specializations of TTrace if we want
 * the general case to do nothing --- we do this if only want small values of N to be compiled into the code
 */
#define LSST_TTRACE0(STYPE, TEMPLATE, SPECIALIZATION, N) \
template<TEMPLATE> \
inline void TTrace SPECIALIZATION(STYPE name, \
                                  STYPE fmt,  \
                                  ... \
                                 ) {  \
    va_list ap; \
    va_start(ap, fmt); \
    Trace(name, N, fmt, ap); \
    va_end(ap); \
}

#define LSST_TTRACE(TEMPLATE, SPECIALIZATION, N) \
    LSST_TTRACE0(char const*, TEMPLATE, SPECIALIZATION, N) \
    LSST_TTRACE0(std::string const&, TEMPLATE, SPECIALIZATION, N)

#if !defined(LSST_MAX_TRACE) || LSST_MAX_TRACE < 0
LSST_TTRACE(int N, , N)                 // allow all N to be instantiated
#else
/*
 * General case does nothing (but partial specializations do)
 */
/**
 * \brief Print a trace message if the verbosity is >= N
 *
 * \note Traces at level > LSST_MAX_TRACE are compiled out of the code (if it's non-negative)
 */
template<int N>
inline void TTrace(const char *name,    //!< Name of component
                   const char *fmt,     //!< Message to write as a printf format     
                   ...                  //!< arguments for fmt
                  ) {
    ;                                   // the general TTrace doesn't so anything (specializations do)
}

template<int N>
inline void TTrace(std::string const& name, //!< Name of component
                   std::string const& fmt,  //!< Message to write as a printf format      
                   ...                      //!< arguments for fmt
           ) {
    ;                                   // the general TTrace doesn't so anything (specializations do)
}

/*
 * Use boost's C-preprocessor metaprogramming to generate the needed partial specializations
 *
 * We could do this via recursive template specialisations of TRACE if it weren't for the
 * varargs, which cause g++ to refuse to inline the function call needed to convert ...
 * to va_list before the recursion can begin.
 */
#   define BOOST_PP_LOCAL_MACRO(n)   LSST_TTRACE( , <n>, n) // expand this macro
#   define BOOST_PP_LOCAL_LIMITS     (0, LSST_MAX_TRACE) // for n=0...LSST_MAX_TRACE
#   include BOOST_PP_LOCAL_ITERATE()                     // do the work
#endif

#undef LSST_TTRACE0
#undef LSST_TTRACE

} // namespace logging
} // namespace pex
} // namespace lsst
#endif    // end LSST_PEX_UTILS_TRACE_H

