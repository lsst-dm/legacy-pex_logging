#!/usr/bin/env python
"""
Tests of the BlockTestingLog
"""
from __future__ import absolute_import, division, print_function

import unittest
import lsst.utils.tests

from lsst.pex.logging import Log, BlockTimingLog, LogRecord


class BlockTimingLogTestCase(unittest.TestCase):

    def setUp(self):
        self.log = BlockTimingLog(Log.getDefaultLog(), "test")

    def tearDown(self):
        del self.log

    def testINSTRUMValue(self):
        self.assertEqual(BlockTimingLog.INSTRUM, -3)

    def testCreate(self):
        btl = self.log.createForBlock("api")
        self.assertEqual(btl.getInstrumentationLevel(), BlockTimingLog.INSTRUM)
        self.assertEqual(btl.getFunctionName(), "api")

        btl = self.log.createForBlock("api", BlockTimingLog.INSTRUM+1)
        self.assertEqual(btl.getInstrumentationLevel(), -2)

    def testBlock(self):
        log = BlockTimingLog(self.log, "test")
        log.setThreshold(BlockTimingLog.INSTRUM)
        btl = log.createForBlock("api")
        btl.log(Log.INFO, "message")
        btl.done()
        del btl

    def testStart(self):
        log = BlockTimingLog(self.log, "test")
        log.setThreshold(BlockTimingLog.INSTRUM)
        log.start("Test run")
        log.done()

    def testSysData(self):
        lr = LogRecord(0, 0, True)
        self.log.setUsageFlags(self.log.SUTIME)
        self.log.addUsageProps(lr)
        ps = lr.getProperties()
        self.assertTrue(ps.exists("usertime"))
        self.assertTrue(ps.exists("systemtime"))
        self.assertFalse(ps.exists("nswap"))

        lr = LogRecord(0, 0, True)
        self.log.setUsageFlags(self.log.ALLUDATA)
        self.log.addUsageProps(lr)
        ps = lr.getProperties()
        self.assertTrue(ps.exists("usertime"))
        self.assertTrue(ps.exists("systemtime"))
        self.assertTrue(ps.exists("maxrss"))
        self.assertTrue(ps.exists("nswap"))
        self.assertTrue(ps.exists("blocksin"))
        self.assertTrue(ps.exists("blocksout"))

__all__ = "BlockTimingLogTestCase".split()


class TestMemory(lsst.utils.tests.MemoryTestCase):
    pass


def setup_module(module):
    lsst.utils.tests.init()

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
