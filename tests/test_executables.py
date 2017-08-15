import unittest
import lsst.utils.tests


class UtilsBinaryTester(lsst.utils.tests.ExecutablesTestCase):
    pass

# Do not run the executables that have their output compared in python
EXECUTABLES = ("test_blockTimingLog",
               "test_defLog",
               "test_fileDest",
               "test_log",
               "test_logFormatter",
               "test_logRecord",
               "test_noTrace",
               "test_propertyPrinter",
               "test_thresholdMemory",
               "test_trace",
               "test_timeSyscalls")
UtilsBinaryTester.create_executable_tests(__file__, EXECUTABLES)

if __name__ == "__main__":
    unittest.main()
