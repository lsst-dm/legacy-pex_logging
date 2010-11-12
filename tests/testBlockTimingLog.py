#!/usr/bin/env python
"""
Tests of the BlockTestingLog
"""
from __future__ import with_statement

import pdb                              # we may want to say pdb.set_trace()
import os
import sys
import unittest
import time
import copy
import random

from lsst.pex.logging import Log, BlockTimingLog

class BlockTimingLogTestCase(unittest.TestCase):

    def setUp(self):
        self.log = BlockTimingLog(Log.getDefaultLog(), "test")
    def tearDown(self):
        pass

    def testINSTRUMValue(self):
        self.assertEquals(BlockTimingLog.INSTRUM, -3)

    def testCreate(self):
        btl = self.log.createForBlock("api")
        self.assertEquals(btl.getInstrumentationLevel(), BlockTimingLog.INSTRUM)
        self.assertEquals(btl.getFunctionName(), "api")

        btl = self.log.createForBlock("api", BlockTimingLog.INSTRUM+1)
        self.assertEquals(btl.getInstrumentationLevel(), -2)

    def testBlock(self):
        log = BlockTimingLog(self.log, "test")
        log.setThreshold(BlockTimingLog.INSTRUM)
        btl = log.createForBlock("api")
        btl.log(Log.INFO, "message")
        btl.done()
        del btl


__all__ = "BlockTimingLogTestCase".split()

if __name__ == "__main__":
    unittest.main()

        
