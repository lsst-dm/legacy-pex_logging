from __future__ import absolute_import

import lsst.daf.base
import lsst.utils

from past.builtins import long
from ._common import _DefaultLogIsScreenLog, _getDefaultAsScreenLog
from ._trace import *
from ._threshold import *
from ._logRecord import *
from ._log import *
from ._debug import *
from ._blockTimingLog import *
from ._screenLog import *

def getDefaultLog():
    if _DefaultLogIsScreenLog():
        return _getDefaultAsScreenLog();
    else:
        return Log.getDefaultLog();

class Prop:
    """package a property to send it to a LogRecord"""
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def addToRec(self, rec):
        rec.addProperty(self.name, self.value)

def _LogRecord_addProperty(self, name, val):
    """add a property with a default type.

    Users can explicitly set the C++ type of a property that gets added by 
    using the proper type-specific addProperty method (addPropertyBool(), 
    addPropertyLongLong(), etc.); the methods that are supported this way
    are int, long, long long, float, double, bool, string, and PropertySet.  
    This method will choose a type for the property based on the value.  

    If the value is a Python integer, then the value will be set based on 
    its value:  if the value is out of range of a 32-bit integer, 
    [-2147483648, 2147483648), it will be stored as a C++ int; out of that 
    range, the type will be long long.  All floating point numbers are stored
    as C++ doubles.  Booleans and strings are stored as C++ bools and 
    std::strings, respectively.  Lists are stored as arrays with the same
    mappings for the elements (but note that all values in the list must be
    of the same type).  Dictionaries are stored as PropertySets with similar
    mappings for their underlying types.

    @param name    the name of the property
    @param val     that value to set for the property
    @exception pex.exceptions.TypeError   if the value is of an unsupported type.
    """
    if isinstance(val, (int, long)):
        if val > 2147483648 or val <= -2147483648:
            return self.addPropertyLongLong(name, val)
        else:
            return self.addPropertyInt(name, val)
    elif isinstance(val, float):
        return self.addPropertyDouble(name, val)
    elif isinstance(val, bool):
        return self.addPropertyBool(name, val)
    elif isinstance(val, str):
        return self.addPropertyString(name, val)
    elif isinstance(val, lsst.daf.base.PropertySet):
#         return self.addPropertyPropertySetPtr(name, val)
        raise lsst.pex.exceptions.TypeError("PropertySet type temporarily unsupported")
    elif isinstance(val, list):
        for v in val:
            self.addProperty(name, v)
    elif isinstance(val, dict):
        for k in val.keys():
            self.addProperty("%s.%s" % (name, k), val[k])
    else:
        raise lsst.pex.exceptions.TypeError("unsupported property type for logging: %s(%s)" % (name, type(val)))

def _LogRecord_setProperty(self, name, val):
    """add a property with a default type.

    Users can explicitly set the C++ type of a property that gets added by 
    using the proper type-specific addProperty method (addPropertyBool(), 
    addPropertyLongLong(), etc.); the methods that are supported this way
    are int, long, long long, float, double, bool, string, and PropertySet.  
    This method will choose a type for the property based on the value.
    See addProperty() for an explanation of the default mappings. 

    @param name    the name of the property
    @param val     that value to set for the property
    @exception pex.exceptions.TypeError   if the value is of an unsupported type.
    """
    if isinstance(val, (int, long)):
        if val > 2147483648 or val <= -2147483648:
            return self.setPropertyLongLong(name, val)
        else:
            return self.setPropertyInt(name, val)
    elif isinstance(val, float):
        return self.setPropertyDouble(name, val)
    elif isinstance(val, bool):
        return self.setPropertyBool(name, val)
    elif isinstance(val, string):
        return self.setPropertyBool(name, val)
    elif isinstance(val, lsst.daf.base.PropertySet):
#        return self.setPropertyPropertySet(name, val)
        raise lsst.pex.exceptions.TypeError("PropertySet type temporarily unsupported")
    elif isinstance(val, list):
        v = val.pop(0)
        self.setProperty(name, v)
        for v in val:
            self.addProperty(name, v)
    elif isinstance(val, dict):
        self.data().remove(name)
        for k in val.keys():
            self.addProperty("%s.%s" % (name, k), val[k])
    else:
        raise lsst.pex.exceptions.TypeError("unsupported property type for logging: %s(%s)" % (name, type(val)))

LogRecord.addProperty = _LogRecord_addProperty
LogRecord.setProperty = _LogRecord_setProperty

# This doesn't appear to be needed anymore (at least not for the unit tests)
#Log._cpplog_str = Log.log
#
#def _Log_log(self, verb, *args):
#    """send any number of strings, PropertySets, or other properties 
#    in a message to the Log.  
#    """
#    rec = LogRec(self, verb)
#    for prop in args:
#        if isinstance(prop,str):
#            rec << prop
#        if isinstance(prop, lsst.daf.base.PropertySet):
#            rec.addProperties(prop)
#
#    rec << endr    # sends result
#    return self
#
## add registration methods
#def _Log_registerLogRec(self, logrec):
#    if not hasattr(self.__dict__, '_logrecs'):
#        self.__dict__['_logrecs'] = {}
#    self.__dict__['_logrecs'][repr(logrec)] = logrec
#
#def _Log_unregisterLogRec(self, logrec):
#    if not hasattr(self.__dict__, '_logrecs'):
#        self.__dict__['_logrecs'] = {}
#    rep = repr(logrec)
#    if self.__dict__['_logrecs'].has_key(rep):
#        del (self.__dict__['_logrecs'])[rep]
#
#Log._registerLogRec = _Log_registerLogRec
#Log._unregisterLogRec = _Log_unregisterLogRec
#
## we need to add some functionality to the LogRec class which involves adding
## an attribute to an instance's self object.  Thus, we need to extend the
## __init__ function.  We do this by first saving the SWIG-generated function.
##
## now use the same technique to extend the LogRec's constructor
#LogRec.__cppinit__ = LogRec.__init__
#
#def _LogRec_extended__init__(self, *args):
#    """an extension to the cpp-generated constructor that will register
#    this instance with the parent logger.  It will be unregistered the
#    first time the endr manipulator is <<-ed on.  This will prevent this 
#    instance from being prematurely deleted by python's garbage collector. 
#    """
#
#    # call the original SWIG-generated constructor first
#    LogRec.__cppinit__(self, *args)
#
#    # find the attached log 
#    if len(args) > 0:
#        if isinstance(args[0], Log):
#            self.__dict__['_theLog'] = args[0]
#        elif (isinstance(args[0], LogRec) and
#              hasattr(args[0].__dict__,'_theLog')):
#            self.__dict__['_theLog'] = args[0].__dict__['_theLog']
#
#    # register this instance
#    if hasattr(self, '_theLog'):
#        self._theLog._registerLogRec(self)
#
## now replace cpp's constructor
#LogRec.__init__ = _LogRec_extended__init__

# now extend LogRec's << operator

def _LogRec_extended__lshift__(self, *args):
    """an extension to the << operator to support C++-like interface.

    This implementation does two things:  (1) supports adding Prop objects,
    PropertySets, and dictionaries for adding properties, and (2) unregisters
    this LogRec from its Log instance when an endr object is added.
    """
    out = self
    if isinstance(args[0], Prop):
        self.addProperty(args[0].name, args[0].value)
    elif isinstance(args[0], lsst.daf.base.PropertySet):
        self.addProperties(args[0])
    elif isinstance(args[0], dict):
        for k in args[0].keys():
            self.addProperty(k, args[0][k])
    else:
        # all other types, handle with the default behavior
        out = self.__cpplshift__(*args)

    # if we just shifted on the endr manipulator, assume that it is safe
    # to garbage collect this object:  unregister it from it's Log.
    if (hasattr(self,'_theLog') and len(args) > 0 and
        isinstance(args[0], int) and args[0] == LogRec.endr):
        self._theLog._unregisterLogRec(self)

    return out
        
LogRec.__lshift__ = _LogRec_extended__lshift__

# finally put an instance of endr at the module level for convenience
endr = LogRec.endr

# duplicate the Rec typedef
Rec = LogRec

# tweak Debug
Debug.default_max_debug = None
_Debug_wrapped_ctr = Debug.__init__

def _Debug__init__(s, name, maxverb=None):
    if maxverb is None:  maxverb = Debug.default_max_debug
    if maxverb is None:  maxverb = -1 * Log.INHERIT_THRESHOLD
    _Debug_wrapped_ctr(s, name, maxverb)

Debug.__init__ = _Debug__init__
