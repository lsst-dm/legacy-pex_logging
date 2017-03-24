from __future__ import absolute_import

__all__ = ['Prop', 'endr', 'Rec']

import lsst.daf.base
from lsst.utils import continueClass

from ._loggingLib import LogRec

class Prop:
    """package a property to send it to a LogRecord"""
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def addToRec(self, rec):
        rec.addProperty(self.name, self.value)

@continueClass
class LogRec:
    def __lshift__(self, *args):
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
        
# finally put an instance of endr at the module level for convenience
endr = LogRec.endr

# duplicate the Rec typedef
Rec = LogRec
