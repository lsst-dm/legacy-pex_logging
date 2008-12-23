// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOGGING_DEBUG_H
#define LSST_PEX_LOGGING_DEBUG_H

#include "lsst/pex/logging/Log.h"

namespace lsst {
namespace pex {
namespace logging {

#ifndef LSST_DEBUGGING_ON
#define LSST_DEBUGGING_ON
#endif

#ifdef LSST_NO_DEBUG
#undef LSST_DEBUGGING_ON
#endif


#ifdef LSST_DEBUGGING_ON

/**
 * a special logger used for recording debugging messages.
 */
class Debug : public Log {
public:

    Debug(const string& name, int verbosity=-1*Log::INHERIT_THRESHOLD) 
        : Log(Log::getDefaultLog(), name, -1*verbosity) { }

    Debug(const Log& parent, const string& name, 
          int verbosity=-1*Log::INHERIT_THRESHOLD) 
        : Log(parent, name , -1*verbosity) { }
    Debug(const Debug& that) : Log(that) { }

    Debug& operator=(const Debug& that) {
        if (this != &that) dynamic_cast<Log*>(this)->operator=(that);
        return *this;
    }

    void debug(int verbosity, const string& message) {
        log(-1*verbosity, message);
    }
    void debug(int verbosity, const boost::format& message) {
        log(-1*verbosity, message);
    }

    void debug(const string& message) { debug(1, message); }
    void debug2(const string& message) { debug(2, message); }
    void debug3(const string& message) { debug(3, message); }
    void debug4(const string& message) { debug(4, message); }
    void debug5(const string& message) { debug(5, message); }

    void debug(const boost::format& message) { debug(1, message); }
    void debug2(const boost::format& message) { debug(2, message); }
    void debug3(const boost::format& message) { debug(2, message); }
    void debug4(const boost::format& message) { debug(2, message); }
    void debug5(const boost::format& message) { debug(2, message); }
};

#define DEBUGLOG(NAME, VERBOSITY) MDEBUGLOG(__debugLog__, NAME, VERBOSITY)
#define MDEBUGLOG(LOGVAR, NAME, VERBOSITY) lsst::pex::logging::Debug LOGVAR(lsst::pex::logging::Log::getDefaultLog(), NAME, VERBOSITY)

#define DEBUGN(VERBOSITY, MSG) MDEBUGN(__debugLog__, VERBOSITY, MSG)
#define DEBUG(MSG) MDEBUG(__debugLog__, MSG)
#define DEBUG2(MSG) MDEBUG2(__debugLog__, MSG)
#define DEBUG3(MSG) MDEBUG3(__debugLog__, MSG)
#define DEBUG4(MSG) MDEBUG4(__debugLog__, MSG)
#define DEBUG5(MSG) MDEBUG5(__debugLog__, MSG)

#define MDEBUGN(LOGVAR, VERBOSITY, MSG)  LOGVAR.debug(VERBOSITY, MSG)
#define MDEBUG(LOGVAR, MSG)  LOGVAR.debug(MSG)
#define MDEBUG2(LOGVAR, MSG) LOGVAR.debug2(MSG)
#define MDEBUG3(LOGVAR, MSG) LOGVAR.debug3(MSG)
#define MDEBUG4(LOGVAR, MSG) LOGVAR.debug4(MSG)
#define MDEBUG5(LOGVAR, MSG) LOGVAR.debug5(MSG)

#else 

#define DEBUGLOG(NAME, VERBOSITY)   // 
#define MDEBUGLOG(LOGVAR, NAME, VERBOSITY)  // 

#define DEBUGN(VERBOSITY, MSG)      //
#define DEBUG(MSG)                  //
#define DEBUG2(MSG)                 //
#define DEBUG3(MSG)                 //
#define DEBUG4(MSG)                 //
#define DEBUG5(MSG)                 //

#define MDEBUGN(LOGVAR, VERBOSITY, MSG)       
#define MDEBUG(LOGVAR, MSG)                  
#define MDEBUG2(LOGVAR, MSG)                 
#define MDEBUG3(LOGVAR, MSG)                 
#define MDEBUG4(LOGVAR, MSG)                 
#define MDEBUG5(LOGVAR, MSG)                 

#endif


}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_LOGGING_DEBUG_H
