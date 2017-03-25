#! /usr/bin/env python

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
# routines for testing logging output
#
# The wrapped test program writes output to the screen.  This module tests
# the output by redirecting it to a file and comparing with canonical
# results.  Note that when the underlying test program is changed, the file
# containing the canonical results usually must change as well.

from __future__ import absolute_import, division, print_function
import os
import re
import subprocess

import lsst.utils.tests


def _canonicalize(loglist):
    """Given a list of lines from log output, canonicalize the order to account
    for the fact that the message field attributes can be written in arbitrary
    orders.
    """
    pat = re.compile(r'^  ([A-Za-z\._]*):(.*)')

    canon = []
    fields = {}
    for line in loglist:
        match = pat.match(line)
        if match:
            fields[match.group(1)] = match.group(2)
        else:
            # Not in field attributes, flush the fields in sorted form and then
            # store the line. Clear the fields.
            for f in sorted(fields):
                canon.append("  {}:{}".format(f, fields[f]))
            fields = {}
            canon.append(line)
    return canon


@lsst.utils.tests.inTestCase
def assertLogs(self, prog, workdir, canon_suffix="_correct.txt", test_suffix="_results.txt"):
    prog = os.path.join(workdir, prog)
    canon = prog + canon_suffix
    test = prog + test_suffix

    # Run the program. The output goes to standard error.
    with open(test, "w+") as f:
        messages = subprocess.check_output(prog, bufsize=1, stderr=f).decode()

        # Rewind the file and read it back into a list
        f.seek(0)
        output = f.readlines()

    # Messages should go to standard out to be caught by test infrastructure
    print(messages, end="")

    # Filter out any dates as we can not include those in the comparison
    filtered = [l for l in output if "DATE:" not in l and "TIMESTAMP:" not in l]

    # To allow offline comparison of the output we write the filtered version back
    # to the file
    with open(test, "w") as f:
        print("".join(filtered), file=f, end="")

    # Read reference correct values from file
    with open(canon, "r") as f:
        correct = list(f.readlines())

    # Strip newlines to simplify output if there are differences
    filtered = [l.rstrip("\n") for l in filtered]
    correct = [l.rstrip("\n") for l in correct]

    # The order of fields can be dependent on operating system so we have to force an
    # order or do dict comparison.
    filtered = _canonicalize(filtered)
    correct = _canonicalize(correct)

    # Compare the two versions
    self.maxDiff = None
    self.assertListEqual(correct, filtered)

    return
