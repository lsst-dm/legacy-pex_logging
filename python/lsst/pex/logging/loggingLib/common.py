from __future__ import absolute_import

__all__ = ['getDefaultLog']

from ._loggingLib import _DefaultLogIsScreenLog, _getDefaultAsScreenLog

def getDefaultLog():
    if _DefaultLogIsScreenLog():
        return _getDefaultAsScreenLog();
    else:
        return Log.getDefaultLog();
