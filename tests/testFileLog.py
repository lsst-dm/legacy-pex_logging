#! /usr/bin/env python
#
import sys, os
import unittest

from lsst.pex.logging import Log

class TestCase(unittest.TestCase):

    def setUp(self):
        self.file = "tests/testFileLog-out.txt"
        self.root = Log()
        self.logger = Log(self.root, "test")

    def tearDown(self):
        if os.path.exists(self.file):
            os.remove(self.file)

    def testAddFile(self):
        self.logger.addDestination(self.file)
        self.logger.log(Log.INFO, "in file")

        self.root.log(Log.INFO, "in bit bucket")

        fd = open(self.file)
        try:
            lines = fd.readlines()
            self.assertEquals(len(filter(lambda l: l.find("in file")>=0,lines)), 1)
            self.assertEquals(len(filter(lambda l: l.find("bucket")>=0,lines)), 0)
        finally:
            fd.close()
        
if __name__ == "__main__":
    unittest.main()
    
