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

import unittest

import lsst.utils.tests

import lsst.pex.logging as log

"""test simple use of the Log facility."""
"""
testLog

Run with:
   python testLog
"""


class TestSimpleLogger(unittest.TestCase):

    def testLogging(self):

        # test a simple message to the default log
        dlog = log.getDefaultLog()
        dlog.log(log.Log.WARN, "this is a warning")

        # now let's create our own root log
        logger = log.ScreenLog(True)

        # test creation of child log
        tlog = log.Log(logger, "test")
        tlog.log(log.Log.INFO, "I like your hat")

        # test that "format", "infof", etc are ignored by swig.
        self.assertFalse(hasattr(tlog, 'format'))
        self.assertFalse(hasattr(tlog, 'debugf'))
        self.assertFalse(hasattr(tlog, 'infof'))
        self.assertFalse(hasattr(tlog, 'warnf'))
        self.assertFalse(hasattr(tlog, 'fatalf'))

        # test shortcut methods
        tlog.logdebug("Shortcut debug")
        tlog.info("Shortcut info")
        tlog.warn("Shortcut warn")
        tlog.fatal("Shortcut fatal")

        # test threshold filtering
        tlog.setThreshold(log.Log.WARN)
        tlog.log(log.Log.INFO, "I like your gloves")  #  // shouldn't see this

        # test the persistance of threshold levels
        tlog = log.Log(logger, "test")
        tlog.log(log.Log.INFO, "I like your shoes")  #   // shouldn't see this
        tlog.setThreshold(log.Log.DEBUG)
        tlog.log(log.Log.INFO, "I said, I like your shoes")

        # test descendent log and ancestor's control of threshold
        tgclog = log.Log(tlog, "grand.child")  #   // name is now "test.grand.child"
        tgclog.log(log.Log.INFO, "Let's play")
        tlog.setThreshold(log.Log.FATAL)
        tgclog.log(log.Log.INFO, "You go first")

        # test streaming
        log.LogRec(tgclog, log.Log.FATAL) << "help: I've fallen" << log.Prop("NODE", 5) << "& I can't get up" << log.endr;
        tmp = log.Prop("NODE", 5)
        log.LogRec(tgclog, log.Log.FATAL) << "help: I've fallen" << tmp << "& I can't get up" << log.endr;

        # test flushing on delete
        log.Rec(tgclog, log.Log.FATAL) << "never mind"
        log.Rec(tgclog, log.Log.DEBUG) << "testing" << log.endr;


def setup_module(module):
    lsst.utils.tests.init()

if __name__ == "__main__":
    lsst.utils.tests.init()
    unittest.main()
