#
# LSST Data Management System
# Copyright 2008, 2009, 2010 LSST Corporation.
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
#

#
# A wrapper test script around testLogDestination
#
# The wrapped test program writes output to the screen.  This script tests
# the output by redirecting it to a file and comparing with canonical
# results.  Note that when the underlying test program is changed, the file
# containing the canonical results usually must change as well.

import sys
import os
import unittest

import lsst.utils.tests

testdir = os.path.dirname(__file__)
origpath = list(sys.path)
sys.path.insert(0, testdir)
import verifyOutput  # noqa E402 module level import not at top of file
sys.path = origpath


class TestLogDestination(lsst.utils.tests.TestCase):

    def testLogDestination(self):
        self.assertLogs("testLogDestination", workdir=os.path.dirname(__file__))


class TestMemory(lsst.utils.tests.MemoryTestCase):
    pass


def setup_module(module):
    lsst.utils.tests.init()

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
