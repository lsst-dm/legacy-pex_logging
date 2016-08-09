import unittest
import lsst.utils.tests


class UtilsBinaryTester(lsst.utils.tests.ExecutablesTestCase):
    pass

# Do not run the executables that have their output compared in python
EXECUTABLES = ("testBlockTimingLog",
               "testDefLog",
               "testFileDest",
               "testLog",
               "testLogFormatter",
               "testLogRecord",
               "testNoTrace",
               "testPropertyPrinter",
               "testThresholdMemory",
               "testTrace",
               "timeSyscalls")
UtilsBinaryTester.create_executable_tests(__file__, EXECUTABLES)

if __name__ == "__main__":
    unittest.main()
