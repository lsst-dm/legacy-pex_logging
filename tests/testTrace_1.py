#! /usr/bin/env python
"""test use of Trace from python"""

import lsst.utils.tests as tests
import lsst.pex.logging as pexLog

def main():
    pexLog.Trace_setVerbosity("lsst.afw", 3)
    pexLog.Trace("lsst.afw", 2, "Hello")
    pexLog.Trace("lsst.afw", 5, "world")

if __name__ == "__main__":
    main()
    
