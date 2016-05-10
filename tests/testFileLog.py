#! /usr/bin/env python
#
# LSST Data Management System
# Copyright 2008-2016 LSST Corporation.
#
# This product includes software developed by the
# LSST Project (http://www.lsst.org/).
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the LSST License Statement and
# the GNU General Public License along with this program.  If not,
# see <http://www.lsstcorp.org/LegalNotices/>.

import os
import unittest

import lsst.utils.tests as utilsTests
from lsst.pex.logging import Log


class TestCase(unittest.TestCase):

    def setUp(self):
        self.file = "tests/testFileLog-out.txt"
        self.root = Log()
        self.logger = Log(self.root, "test")

    def tearDown(self):
        del self.root
        del self.logger
        if os.path.exists(self.file):
            os.remove(self.file)

    def testAddFile(self):
        self.logger.addDestination(self.file)
        self.logger.log(Log.INFO, "in file")

        self.root.log(Log.INFO, "in bit bucket")

        fd = open(self.file)
        try:
            lines = fd.readlines()
            self.assertEquals(len(filter(lambda l: l.find("in file") >= 0, lines)), 1)
            self.assertEquals(len(filter(lambda l: l.find("bucket") >= 0, lines)), 0)
        finally:
            fd.close()

    def testPassAll(self):
        self.logger.addDestination(self.file)
        self.logger.log(Log.DEBUG, "in file")
        self.logger.setThreshold(Log.DEBUG)
        self.logger.log(Log.DEBUG, "debugging")

        fd = open(self.file)
        try:
            lines = fd.readlines()
            self.assertEquals(len(filter(lambda l: l.find("in file") >= 0, lines)), 0)
            self.assertEquals(len(filter(lambda l: l.find("debugging") >= 0, lines)), 1)
        finally:
            fd.close()

    def testVerbose(self):
        self.logger.addDestination(self.file, True)
        self.logger.log(Log.DEBUG, "in file")
        self.logger.setThreshold(Log.DEBUG)
        self.logger.log(Log.DEBUG, "debugging")

        fd = open(self.file)
        try:
            lines = fd.readlines()
            self.assertEquals(len(filter(lambda l: l.find("in file") >= 0, lines)), 0)
            self.assertEquals(len(filter(lambda l: l.find("debugging") >= 0, lines)), 1)
            self.assertEquals(len(filter(lambda l: l.find("LEVEL") >= 0, lines)), 1)
        finally:
            fd.close()


def suite():
    utilsTests.init()

    suites = []
    suites += unittest.makeSuite(TestCase)
    suites += unittest.makeSuite(utilsTests.MemoryTestCase)
    return unittest.TestSuite(suites)


def run(shouldExit=False):
    utilsTests.run(suite(), shouldExit)


if __name__ == "__main__":
    run(True)
