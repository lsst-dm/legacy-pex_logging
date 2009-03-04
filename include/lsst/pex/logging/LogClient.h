// -*- lsst-c++ -*-
/**
 * @file LogClient.h
 * @brief definition of the LogClient and LogClientHelper classes
 * @author Ray Plante
 */
#ifndef LSST_PEX_LOGCLIENT_H
#define LSST_PEX_LOGCLIENT_H

#include "lsst/pex/logging/Log.h"

namespace lsst {
namespace pex {
namespace logging {

/**
 * @brief an abstract interface identifying a class that writes messages to 
 * a Log.  Other classes inherit from this interface to add standard methods
 * for setting and retrieving the log that the classs will send its log 
 * messages to.  
 *
 * Typically, a class will inherit directly from LogClientHelper, which 
 * provides the internal Log data member as a convenience.  This abstract
 * parent allows the subclass to control how/where the Log is stored.  
 */
class LogClient {

    /**
     * initialize the LogClient interface.  This constructor does nothing
     */
    LogClient() { }

    //@{
    /**
     * return the Log being written to by this class.
     */
    virtual const Log& getLog() const = 0;
    virtual Log& getLog() = 0;
    //@}

    /**
     * set the Log that should be used by this class
     */
    virtual void setLog(const Log& log) = 0;

};

/**
 * 
 */
class LogClientHelper : public LogClient {
public:

    /**
     * Create a client to use the default log
     */
    LogClientHelper() : _log(Log::getDefaultLog()) { }

    /**
     * Create a client to use a child of the default Log
     */
    LogClientHelper(const string& childName) 
        : _log(Log::getDefaultLog(), childName) 
    { }

    /**
     * Create a client to use a particular Log
     */
    LogClientHelper(const Log& log) : _log(log) { }

    /**
     * Create a client to use a child of the given log
     */
    LogClientHelper(const Log& log, const string& childName) 
        : _log(log, childName) 
    { }

protected:
    Log _log;
};


}}} // end lsst::pex::logging
