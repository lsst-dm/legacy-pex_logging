// -*- lsst-c++ -*-

/* 
 * LSST Data Management System
 * Copyright 2008, 2009, 2010 LSST Corporation.
 * 
 * This product includes software developed by the
 * LSST Project (http://www.lsst.org/).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the LSST License Statement and 
 * the GNU General Public License along with this program.  If not, 
 * see <http://www.lsstcorp.org/LegalNotices/>.
 */
 
%define logging_DOCSTRING
"
Access to the logging classes from the pex library
"
%enddef

%feature("autodoc", "1");
%module(package="lsst.pex.logging", docstring=logging_DOCSTRING) loggingLib

%include "lsst/p_lsstSwig.i"

%newobject lsst::pex::logging::BlockTimingLog::createForBlock;
%newobject lsst::pex::logging::BlockTimingLog::timeBlock;
%newobject lsst::pex::logging::Log::createChildLog;

%{
#include "lsst/pex/logging/Trace.h"
#include "lsst/pex/logging/BlockTimingLog.h"
#include "lsst/pex/logging/ScreenLog.h"
#include "lsst/pex/logging/DualLog.h"
#include "lsst/pex/logging/FileDestination.h"
#include "lsst/pex/logging/Debug.h"
#include "lsst/pex/exceptions.h"
%}

%inline %{
using std::list;
using std::vector;
using boost::shared_ptr;
%}


%import  "lsst/daf/base/baseLib.i"                // for PropertySet
%import  "lsst/pex/exceptions/exceptionsLib.i"    // for Exceptions

// SWIG_SHARED_PTR macro invocations must precede the corresponding type declarations
SWIG_SHARED_PTR(LogFormatter, lsst::pex::logging::LogFormatter)
SWIG_SHARED_PTR_DERIVED(BriefFormatter, lsst::pex::logging::LogFormatter, lsst::pex::logging::BriefFormatter)
SWIG_SHARED_PTR_DERIVED(IndentedFormatter, lsst::pex::logging::BriefFormatter, lsst::pex::logging::IndentedFormatter)
SWIG_SHARED_PTR_DERIVED(NetLoggerFormatter, lsst::pex::logging::LogFormatter, lsst::pex::logging::NetLoggerFormatter)
SWIG_SHARED_PTR(LogDestination, lsst::pex::logging::LogDestination)
SWIG_SHARED_PTR_DERIVED(FileDestination, lsst::pex::logging::LogDestination, lsst::pex::logging::FileDestination)

%ignore lsst::pex::logging::Log::format(int verbosity, const char *fmt, va_list ap);
%ignore lsst::pex::logging::Log::format(int verbosity, const char *fmt, ...);
%ignore lsst::pex::logging::Debug::debug(int verbosity, const char *fmt, va_list ap);
%ignore lsst::pex::logging::Debug::debug(int verbosity, const char *fmt, ...);
%ignore lsst::pex::logging::Trace::Trace(const std::string& name, const int verbosity, const std::string& fmt, va_list ap);

%inline %{
namespace boost { namespace filesystem {}}
%}

%ignore lsst::pex::logging::FileDestination::FileDestination(const fs::path& filepath, const boost::shared_ptr<LogFormatter>& formatter, int threshold, bool truncate);
%ignore lsst::pex::logging::FileDestination::FileDestination(const char *filepath, const boost::shared_ptr<LogFormatter>& formatter, int threshold, bool truncate);
%ignore lsst::pex::logging::FileDestination::FileDestination(const fs::path& filepath, bool verbose, int threshold, bool truncate);
%ignore lsst::pex::logging::FileDestination::FileDestination(const char *& filepath, bool verbose, int threshold, bool truncate);
          

%include "lsst/pex/logging/LogRecord.h"
%include "lsst/pex/logging/LogFormatter.h"
%include "lsst/pex/logging/LogDestination.h"
%include "lsst/pex/logging/FileDestination.h"
%include "lsst/pex/logging/Log.h"
%include "lsst/pex/logging/BlockTimingLog.h"
%include "lsst/pex/logging/Debug.h"
%include "lsst/pex/logging/Trace.h"
%include "lsst/pex/logging/ScreenLog.h"
%include "lsst/pex/logging/DualLog.h"

%extend lsst::pex::logging::Log {
    %template(addPreamblePropertyInt) addPreambleProperty<int>;
    %template(addPreamblePropertyLong) addPreambleProperty<long>;
    %template(addPreamblePropertyLongLong) addPreambleProperty<long long>;
    %template(addPreamblePropertyFloat) addPreambleProperty<float>;
    %template(addPreamblePropertyDouble) addPreambleProperty<double>;
    %template(addPreamblePropertyBool) addPreambleProperty<bool>;
    %template(addPreamblePropertyString) addPreambleProperty<std::string>;
//    %template(addPreamblePropertyPropertySetPtr) addPreambleProperty<lsst::daf::base::PropertySet::Ptr>;

    %template(setPreamblePropertyInt) setPreambleProperty<int>;
    %template(setPreamblePropertyLong) setPreambleProperty<long>;
    %template(setPreamblePropertyLongLong) setPreambleProperty<long long>;
    %template(setPreamblePropertyFloat) setPreambleProperty<float>;
    %template(setPreamblePropertyDouble) setPreambleProperty<double>;
    %template(setPreamblePropertyBool) setPreambleProperty<bool>;
    %template(setPreamblePropertyString) setPreambleProperty<std::string>;
//    %template(setPreamblePropertyPropertySetPtr) setPreambleProperty<lsst::daf::base::PropertySet::Ptr>;

    %template(logPropertyInt) log<int>;
    %template(logPropertyLong) log<long>;
    %template(logPropertyLongLong) log<long long>;
    %template(logPropertyFloat) log<float>;
    %template(logPropertyDouble) log<double>;
    %template(logPropertyBool) log<bool>;
    %template(logPropertyString) log<std::string>;
//    %template(logPropertyPropertySet) log<lsst::daf::base::PropertySet>;

    void addDestination(const std::string& filepath, bool verbose=false, 
                        int threshold=lsst::pex::logging::threshold::PASS_ALL) 
    {
        boost::shared_ptr<lsst::pex::logging::LogDestination> 
            fdest(new lsst::pex::logging::FileDestination(filepath, verbose, 
                                                          threshold));
        $self->addDestination(fdest);
    }
}

%extend lsst::pex::logging::LogRecord {
    void addPropertyBool(const std::string& name, const bool val) { $self->addProperty<bool>(name, val); }
    void addPropertyInt(const std::string& name, const int val) { $self->addProperty<int>(name, val); }
    void addPropertyLong(const std::string& name, const long val) { $self->addProperty<long>(name, val); }
    void addPropertyLongLong(const std::string& name, const long long val) { $self->addProperty<long long>(name, val); }
    void addPropertyFloat(const std::string& name, const float val) { $self->addProperty<float>(name, val); }
    void addPropertyDouble(const std::string& name, const double val) { $self->addProperty<double>(name, val); }
    void addPropertyString(const std::string& name, const std::string& val) { $self->addProperty<std::string>(name, val); }
}

%inline %{

namespace lsst {
namespace pex {
namespace logging {

const ScreenLog _getDefaultAsScreenLog() {
    return ScreenLog(dynamic_cast<const ScreenLog&>(Log::getDefaultLog()));
}
bool _DefaultLogIsScreenLog() {
    return (dynamic_cast<const ScreenLog*>(&(Log::getDefaultLog())) != 0);
}

}}}

%}

%pythoncode %{
import lsst.utils

def getDefaultLog():
    if _DefaultLogIsScreenLog():
        return _getDefaultAsScreenLog();
    else:
        return Log_getDefaultLog();


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
    @exception LsstException   if the value is of an unsupported type.
    """
    if isinstance(val, int):
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
#        return self.addPropertyPropertySetPtr(name, val)
        raise lsst.pex.exceptions.LsstException("PropertySet type temporarily unsupported")
    elif isinstance(val, list):
        for v in val:
            self.addProperty(name, v)
    elif isinstance(val, dict):
        for k in val.keys():
            self.addProperty("%s.%s" % (name, k), val[k])
    else:
        raise lsst.pex.exceptions.LsstException("unsupported property type for logging: %s(%s)" % (name, type(val)))

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
    @exception LsstException   if the value is of an unsupported type.
    """
    if isinstance(val, int):
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
        raise lsst.pex.exceptions.LsstException("PropertySet type temporarily unsupported")
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
        raise lsst.pex.exceptions.LsstException("unsupported property type for logging: %s(%s)" % (name, type(val)))

LogRecord.addProperty = _LogRecord_addProperty
LogRecord.setProperty = _LogRecord_setProperty

Log._swiglog_str = Log.log

def _Log_log(self, verb, *args):
    """send any number of strings, PropertySets, or other properties 
    in a message to the Log.  
    """
    rec = LogRec(self, verb)
    for prop in args:
        if isinstance(prop,str):
            rec << prop
        if isinstance(prop, lsst.daf.base.PropertySet):
            rec.addProperties(prop)

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

def version():
    """
    Return the version of this product (extracted from an internal HeadURL string).
    If a different version is setup according to eups, include it in the return string.
    """
    HeadURL = r"$HeadURL$"
    return lsst.utils.version(HeadURL)
%}


