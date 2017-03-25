from __future__ import absolute_import, division, print_function

__all__ = ['getDefaultLog']

from .common import _DefaultLogIsScreenLog, _getDefaultAsScreenLog

def getDefaultLog():
    if _DefaultLogIsScreenLog():
        return _getDefaultAsScreenLog();
    else:
        return Log.getDefaultLog();
