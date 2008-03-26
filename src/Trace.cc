// -*- lsst-c++ -*-
/** \file 
  *
  * \brief Class providing basic run-time trace facilities.
  *
  * Tracing is controlled on a per "component" basis, where a "component" is a
  * name of the form aaa.bbb.ccc where aaa is the Most significant part; for
  * example, the utilities library might be called "utils", the doubly-linked
  * list "utils.dlist", and the code to destroy a list "utils.dlist.del"
  *
  * \see Component class for details on the verbosity tree which 
  *      determines when a trace record will be emitted.
  *
  * \author Robert Lupton, Princeton University
  */

#include <map>
#include <boost/tokenizer.hpp>

#include "lsst/tr1/unordered_map.h"

#include "lsst/mwi/utils/Trace.h"
#include "lsst/mwi/utils/Component.h"

using namespace lsst::mwi::utils;

namespace {
    //
    // I want a class that's like an int, but with a default value that
    // isn't 0, as 0 is a valid verbosity, and I want to distinguish the
    // cases "verbosity 0 from the cache" and "not cached"
    //
    class Verbosity {
    public:
        enum { UNDEFINED = -999 };
        Verbosity(int value = UNDEFINED) : _value(value) {}
        void operator=(const int value) { _value = value; }
        operator int() const { return _value; }
    private:
        int _value;
    };
}

/*****************************************************************************/
/**
  * \brief TraceImpl is a singleton class to which all Trace methods delegate their actual functionality.
  *
  *      By using a singleton class to orchestrate all Trace methods, a simple
  *      uniform interface across all classes is provided for the developers.
  */
class TraceImpl {
public:
    TraceImpl();

    friend class lsst::mwi::utils::Trace;

    static void reset();
    static void setVerbosity(const std::string &name);
    static void setVerbosity(const std::string &name, const int verbosity);

    static int getVerbosity(const std::string &name);

    static void printVerbosity(std::ostream &fp = std::cout);

    static void setDestination(std::ostream &fp);
private:

    virtual ~TraceImpl() {}             //!< no-one should delete the singleton

    static Component *_root;            //!< the root of the Component tree

    static std::string _separator;      //!< path separation character
    static std::ostream *_traceStream;  //!< output location for traces

    //! Properties cached for efficiency
    static int _HighestVerbosity;         //!< highest verbosity requested
    static std::tr1::unordered_map<std::string, Verbosity> cache;
};

/*****************************************************************************/
/** Create the one true trace tree
  */
TraceImpl::TraceImpl() {
    if (_root == 0) {
        _root = new Component(".", 0);
        _traceStream = &std::cerr;
        _separator = ".";
    }
}

/*****************************************************************************/
/*  Order is important below
 */
Component*    TraceImpl::_root      = 0;
std::string   TraceImpl::_separator;
std::ostream* TraceImpl::_traceStream;

static TraceImpl::TraceImpl* _singleton  = new TraceImpl(); // the singleton

/* The trace verbosity cache
 */
int         TraceImpl::_HighestVerbosity = 0;
std::tr1::unordered_map<std::string, Verbosity> TraceImpl::cache;

/******************************************************************************/
/** Reset the entire trace system
  */
void TraceImpl::reset() {
    delete _root;
    _root = new Component;
    _HighestVerbosity = 0;
    cache.clear();
}

/** Set a component's verbosity.
  *
  * If no verbosity is specified, inherit from parent
  */
void TraceImpl::setVerbosity(const std::string &name, //!< component of interest
                             const int verbosity      //!< desired trace verbosity
                        ) {    
    _root->add(name, verbosity, _separator);

    if (verbosity > _HighestVerbosity) {
        _HighestVerbosity = verbosity;
    } else {
        _HighestVerbosity = _root->highestVerbosity();
    }
}


/** \overload TraceImpl::setVerbosity(const std::string &name)
  */
void TraceImpl::setVerbosity(const std::string &name //!< component of interest
                        ) {
    int verbosity = Component::INHERIT_VERBOSITY;
    if (name == "" || name == ".") {
        verbosity = 0;
    }
    setVerbosity(name, verbosity);
}


/**  Return a component's verbosity
  */
int TraceImpl::getVerbosity(const std::string &name   //!< component of interest
                           ) {
    int verbosity = cache[name];        // look it up in the cache

    if (verbosity == Verbosity::UNDEFINED) { // not in cache
        verbosity = _root->getVerbosity(name, _separator);
        cache[name] = verbosity;
    }

    return verbosity;
}


/** Print all the trace verbosities
  */
void TraceImpl::printVerbosity(std::ostream &fp     //!< output location
                               ) {
    _root->printVerbosity(fp);
}


/** Change location where traces output
  *
  * Close previous file descriptor if it isn't stdout/stderr
  */
void TraceImpl::setDestination(std::ostream &fp     //!< new output location
                               ) {
    if (*_traceStream != std::cout && *_traceStream != std::cerr) {
        delete _traceStream;
    }
    
    _traceStream = &fp;
}

/******************************************************************************/
namespace lsst {
namespace mwi {
namespace utils {

/** Generate the trace message.
  */
void Trace::trace(const std::string &msg     //!< Trace message
                 ) {
    *TraceImpl::_traceStream << msg;
}


/** Generate the trace message.
  */
void Trace::trace(const std::string &msg,    //!< Trace message
                  const bool add_newline     //!< Should newline be added?
                 ) {
    *TraceImpl::_traceStream << msg;

    if (msg.size() == 0 || msg.substr(msg.size() - 1) != "\n") {
        *TraceImpl::_traceStream << "\n";
    }
}


/** Check that component should be output
  */
bool Trace::check_level(const std::string& name,  //!< Component of interest
                        const int verbosity       //!< Trace request verbosity
                       ) {
    bool print = (verbosity <= TraceImpl::_HighestVerbosity &&
                  TraceImpl::getVerbosity(name) >= verbosity) ? true : false;

    if (print) {
        for (int i = 0; i < verbosity; i++) {
            *TraceImpl::_traceStream << ' ';
        }
    }

    return print;
}


/** Reset the entire Trace system to default values.
  */
void Trace::reset() {
    TraceImpl::reset();
}


/** Set component's verbosity level.
  *
  * If no verbosity is specified, inherit from parent.
  */
void Trace::setVerbosity(const std::string &name,   //!< Component of interest
                         const int verbosity        //!< Component's verbosity
                        ) {
    TraceImpl::setVerbosity(name, verbosity);
}


/** \overload Trace::setVerbosity(const std::string &name)
  */
void Trace::setVerbosity(const std::string &name    //!< Component of interest
                        ) {
    TraceImpl::setVerbosity(name);
}


/** Fetch component's verbosity.
  */
int Trace::getVerbosity(const std::string &name     //!< Component of interest
                       ) {
    return TraceImpl::getVerbosity(name);
}


/** Print entire verbosity tree
  */
void Trace::printVerbosity(std::ostream &fp         //!< Output location
                          ) {
    TraceImpl::printVerbosity(fp);
}


/** Set output location 
  */
void Trace::setDestination(std::ostream &fp         //!< Output location
                          ) {
    TraceImpl::setDestination(fp);
}

} // namespace utils
} // namespace mwi
} // namespace lsst
