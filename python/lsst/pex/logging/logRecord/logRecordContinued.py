from __future__ import absolute_import, division, print_function

from past.builtins import long

import lsst.pex.exceptions
from lsst.utils import continueClass

from .logRecord import LogRecord

__all__ = []

@continueClass
class LogRecord:
    def addProperty(self, name, val):
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
            raise lsst.pex.exceptions.TypeError("PropertySet type temporarily unsupported")
        elif isinstance(val, list):
            for v in val:
                self.addProperty(name, v)
        elif isinstance(val, dict):
            for k in val.keys():
                self.addProperty("%s.%s" % (name, k), val[k])
        else:
            raise lsst.pex.exceptions.TypeError("unsupported property type for logging: %s(%s)" % (name, type(val)))
    
    def setProperty(self, name, val):
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
    
