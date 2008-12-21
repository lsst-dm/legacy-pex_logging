#! /usr/bin/env python
#
# A wrapper test script around testLog
#
# The wrapped test program writes output to the screen.  This script tests
# the output by redirecting it to a file and comparing with canonical
# results.  Note that when the underlying test program is changed, the file
# containing the canonical results usually must change as well.  

import sys, os

testdir = os.path.dirname(sys.argv[0])
if len(testdir) > 0:
    sys.path.insert(0, testdir)
import verifyOutput

okay = verifyOutput.test("testLog")
excode = 1
if okay: excode = 0
sys.exit(excode)

