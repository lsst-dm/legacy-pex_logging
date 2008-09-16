// -*- lsst-c++ -*-
%define logging_DOCSTRING
"
Access to the logging classes from the pex library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.pex.logging", docstring=logging_DOCSTRING) loggingLib

%{
#   include <fstream>
#   include <exception>
#   include <map>
#   include <boost/shared_ptr.hpp>
#   include <boost/any.hpp>
#   include "lsst/utils/Demangle.h"
#   include "lsst/pex/logging/Trace.h"
#   include "lsst/utils/Utils.h"
#include "lsst/pex/logging/Component.h"
#include "lsst/pex/logging/LogRecord.h"
#include "lsst/pex/logging/ScreenLog.h"
#include "lsst/pex/logging/DualLog.h"
%}

%inline %{
namespace lsst { namespace utils { } } 
namespace lsst { namespace daf { namespace base { } } }
namespace lsst { namespace pex { namespace logging { } } }
namespace lsst { namespace pex { namespace logging { namespace Component { } } } }
    
using namespace lsst;
using namespace lsst::utils;
using namespace lsst::daf::base;
using namespace lsst::pex::logging;
using namespace lsst::pex::logging::Component;
%}

%init %{
%}

#define NO_SWIG_LSST_EXCEPTIONS
%include "lsst/p_lsstSwig.i"
SWIG_SHARED_PTR(Persistable, lsst::daf::base::Persistable)
SWIG_SHARED_PTR_DERIVED(DataProperty, lsst::daf::base::Persistable, lsst::daf::base::DataProperty)
%import  "lsst/daf/base/Citizen.h"
%import  "lsst/daf/base/Persistable.h"
%import  "lsst/daf/base/DataProperty.i"
%import  "lsst/utils/Utils.h"

%include "lsst/pex/logging/Log.h"
%include "lsst/pex/logging/ScreenLog.h"
%include "lsst/pex/logging/DualLog.h"

%pythoncode %{
from lsst.daf.base import DataProperty

Log._swiglog_str = Log.log

def _Log_log(self, verb, *args):
    """send any number of strings and DataProperties in a message to the
    Log.  
    """
    rec = LogRec(self, verb)
    for prop in args:
        if isinstance(prop,str) or isinstance(prop, DataProperty):
            rec << prop

    rec << endr    # sends result
    return self


# add registration methods
def _Log_registerLogRec(self, logrec):
    if not hasattr(self.__dict__, '_logrecs'):
        self.__dict__['_logrecs'] = {}
    self.__dict__['_logrecs'][repr(logrec)] = logrec

def _Log_unregisterLogRec(self, logrec):
    if not hasattr(self.__dict__, '_logrecs'):
        self.__dict__['_logrecs'] = {}
    rep = repr(logrec)
    if self.__dict__['_logrecs'].has_key(rep):
        del (self.__dict__['_logrecs'])[rep]

Log._registerLogRec = _Log_registerLogRec
Log._unregisterLogRec = _Log_unregisterLogRec

# we need to add some functionality to the LogRec class which involves adding
# an attribute to an instance's self object.  Thus, we need to extend the
# __init__ function.  We do this by first saving the SWIG-generated function.
#
# now use the same technique to extend the LogRec's constructor
LogRec.__swiginit__ = LogRec.__init__

def _LogRec_extended__init__(self, *args):
    """an extension to the swig-generated constructor that will register
    this instance with the parent logger.  It will be unregistered the
    first time the endr manipulator is <<-ed on.  This will prevent this 
    instance from being prematurely deleted by python's garbage collector. 
    """

    # call the original SWIG-generated constructor first
    LogRec.__swiginit__(self, *args)

    # find the attached log 
    if len(args) > 0:
        if isinstance(args[0], Log):
            self.__dict__['_theLog'] = args[0]
        elif (isinstance(args[0], LogRec) and
              hasattr(args[0].__dict__,'_theLog')):
            self.__dict__['_theLog'] = args[0].__dict__['_theLog']

    # register this instance
    if hasattr(self, '_theLog'):
        self._theLog._registerLogRec(self)

# now replace swig's constructor
LogRec.__init__ = _LogRec_extended__init__

# now extend LogRec's << operator
LogRec.__swiglshift__ = LogRec.__lshift__

def _LogRec_extended__lshift__(self, *args):
    """extend the << operator to unregister this LogRec from its Log instance.
    """
    # first handle the default behavior
    out = self.__swiglshift__(*args)

    # if we just shifted on the endr manipulator, assume that it is safe
    # to garbage collect this object:  unregister it from it's Log.
    if (hasattr(self,'_theLog') and len(args) > 0 and
        isinstance(args[0], int) and args[0] == LogRec.endr):
        self._theLog._unregisterLogRec(self)

    return out
        
LogRec.__lshift__ = _LogRec_extended__lshift__

# finally put an instance of endr at the module level for convenience
endr = LogRec.endr

def version(HeadURL = r"$HeadURL$"):
    """Return a version given a HeadURL string.  If a different version's setup, return that too"""

    version_svn = guessSvnVersion(HeadURL)

    try:
        import eups
    except ImportError:
        return version_svn
    else:
        try:
            version_eups = eups.setup("fw")
        except AttributeError:
            return version_svn

    if version_eups == version_svn:
        return version_svn
    else:
        return "%s (setup: %s)" % (version_svn, version_eups)



%}

/******************************************************************************/// Trace
%ignore Trace(const std::string&, const int, const std::string&, va_list ap);
%include "lsst/pex/logging/Trace.h"

/******************************************************************************/
/******************************************************************************/
// Local Variables: ***
// eval: (setq indent-tabs-mode nil) ***
// End: ***
