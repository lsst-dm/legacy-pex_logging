// -*- lsst-c++ -*-
%define logging_DOCSTRING
"
Access to the logging classes from the mwi library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.mwi", docstring=logging_DOCSTRING) logging

%{
#   include <fstream>
#   include <exception>
#   include <map>
#   include <boost/shared_ptr.hpp>
#   include <boost/any.hpp>
#   include "lsst/mwi/utils/Demangle.h"
#   include "lsst/mwi/utils/Trace.h"
#   include "lsst/mwi/utils/Utils.h"
#   include "lsst/mwi/utils/Component.h"
#   include "lsst/mwi/logging/LogRecord.h"
#   include "lsst/mwi/logging/ScreenLog.h"
#   include "lsst/mwi/logging/DualLog.h"
%}

%inline %{
namespace lsst { namespace mwi { namespace data { } } }
namespace lsst { namespace mwi { namespace logging { } } }
namespace lsst { namespace mwi { namespace utils { } } }
namespace lsst { namespace mwi { namespace utils { namespace Component { } } } }
    
using namespace lsst;
using namespace lsst::mwi::data;
using namespace lsst::mwi::logging;
using namespace lsst::mwi::utils;
using namespace lsst::mwi::utils::Component;
%}

%init %{
%}

%include "p_lsstSwig.i"
%import  "data.i"

%include "lsst/mwi/utils/Utils.h"  // needed for deprecated LSST_START_NAMESPACE
%include "lsst/mwi/logging/Log.h"
%include "lsst/mwi/logging/ScreenLog.h"
%include "lsst/mwi/logging/DualLog.h"

%pythoncode %{
from lsst.mwi.data import DataProperty

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

%}

/******************************************************************************/
// Local Variables: ***
// eval: (setq indent-tabs-mode nil) ***
// End: ***
