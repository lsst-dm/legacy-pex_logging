// -*- lsst-c++ -*-
/**
 * @file Debug.h
 * @brief definition of the Debug class and macros
 * @author Ray Plante
 */
#ifndef LSST_PEX_LOGGING_DEBUG_H
#define LSST_PEX_LOGGING_DEBUG_H

#include "lsst/pex/logging/Log.h"
#include <cstdarg>

namespace lsst {
namespace pex {
namespace logging {

#ifndef LSST_DEBUGGING_ON
#define LSST_DEBUGGING_ON
#endif

#ifdef LSST_NO_DEBUG
#undef LSST_DEBUGGING_ON
#endif

#ifndef LSST_MAX_DEBUG
#define LSST_MAX_DEBUG 0
#endif

#ifndef LSST_DEBUGGING_ON
#define LSST_MAX_DEBUG 0
#endif

/**
 * a special logger used for recording debugging messages.
 */
class Debug : public Log {
public:

    Debug(const std::string& name, int verbosity=LSST_MAX_DEBUG) 
        : Log(Log::getDefaultLog(), name, -1*verbosity) { }

    Debug(const Log& parent, const std::string& name, 
          int verbosity=LSST_MAX_DEBUG) 
        : Log(parent, name , -1*verbosity) { }
    Debug(const Debug& that) : Log(that) { }

    Debug& operator=(const Debug& that) {
        if (this != &that) dynamic_cast<Log*>(this)->operator=(that);
        return *this;
    }

    /**
     * print a debug message with a particular verbosity.  The decision 
     * whether to print this message is done at runtime (just like a normal
     * log message); thus, a message sent to this function that ultimately 
     * will not be printed carries more overhead than one sent to debug<n>().
     * While debug<n>() is thus normally preferred, this function can be 
     * useful for temporarily overriding the compile-time verbosity limit.
     * @param verbosity    the verbosity level of this message.  It will be
     *                        printed if the value is equal to or lower than
     *                        the verbosity limit set when the object was 
     *                        constructed.
     * @param message      the message to print.
     */
    void debug(int verbosity, const std::string& message) {
        log(-1*verbosity, message);
    }

    /**
     * print a debug message with a particular verbosity.  The decision 
     * whether to print this message is done at runtime (just like a normal
     * log message); thus, a message sent to this function that ultimately 
     * will not be printed carries more overhead than one sent to debug<n>().
     * Use of this method typically carries the extra overhead of formatting
     * the message.  
     * While debug<n>() is thus normally preferred, this function can be 
     * useful for temporarily overriding the compile-time verbosity limit.
     */
    void debug(int verbosity, const boost::format& message) {
        log(-1*verbosity, message);
    }

    /**
     * print a formatted message with a particular verbosity.  This 
     * version is mainly in support of the debug<n>() routines.
     */
    void debug(int verbosity, const char *fmt, va_list ap) {
        const int len = strlen(fmt) + 100;  // guess the length
        char msg[len];
        vsnprintf(msg, len, fmt, ap);
        log(-1*verbosity, msg);
    }

    /**
     * conditionally print the given debug message if VERBOSITY is less 
     * than or equal to LSST_MAX_DEBUG.  This condition is evaluated at 
     * compile-time; thus, it is more efficient than the debug() functions.
     */
    template<int VERBOSITY>
    void debug(const std::string& message) {
        if (LSST_MAX_DEBUG <= 0 || VERBOSITY <= LSST_MAX_DEBUG) {
            log(-1*VERBOSITY, message);
        }
    }

    /**
     * conditionally print a debug message via a format string if VERBOSITY 
     * is less than or equal to LSST_MAX_DEBUG.  This condition is evaluated 
     * at compile-time; thus, it is more efficient than the debug() functions.
     * @param fmt      the C-style (as in printf) format string.  
     * @param ...      the values to plug into the string.  Any number may 
     *                   be provided, though they must match the format string.
     */
    template<int VERBOSITY>
    void debug(const char *fmt, ...) {
        if (LSST_MAX_DEBUG <= 0 || VERBOSITY <= LSST_MAX_DEBUG) {
            va_list ap;
            va_start(ap, fmt);
            debug(VERBOSITY, fmt, ap);
            va_end(ap);
        }
    }

};

/**
 * send a debug message to a named log.  This message will not be printed
 * if VERBOSITY > LSST_MAX_DEBUG.  This method saves creating a log if the
 * VERBOSITY level is too high to be printed, at the expense of re-creating
 * everytime VERBOSITY is low.  
 */
template <int VERBOSITY>
void debug(const std::string& name, const std::string& message) {
    if (LSST_MAX_DEBUG <= 0 || VERBOSITY <= LSST_MAX_DEBUG) {
        Debug(name).debug<VERBOSITY>(message);
    }
}

/**
 * send a formatted debug message to a named log.  This message will not be 
 * printed if VERBOSITY > LSST_MAX_DEBUG.  This method saves creating a log 
 * if the VERBOSITY level is too high to be printed, at the expense of 
 * re-creating everytime VERBOSITY is low.  
 */
template <int VERBOSITY>
void debug(const std::string& name, const char *fmt, ...) {
    if (LSST_MAX_DEBUG <= 0 || VERBOSITY <= LSST_MAX_DEBUG) {
        va_list ap;
        va_start(ap, fmt);
        Debug(name).debug(VERBOSITY, fmt, ap);
        va_end(ap);
    }
}


}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_LOGGING_DEBUG_H
