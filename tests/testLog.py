"""demonstrate simple use of the Log facility."""
"""
testLog

Run with:
   python testLog
"""

import lsst.utils.tests as tests
import lsst.pex.logging as log
from lsst.daf.base import DataProperty

if __name__ == "__main__":

    # test a simple message to the default log
    dlog = log.Log_getDefaultLog()
    dlog.log(log.Log.WARN, "this is a warning")

    # now let's create our own root log
    logger = log.ScreenLog(1)

    # test creation of child log
    tlog = log.Log(logger, "test")
    tlog.log(log.Log.INFO, "I like your hat")

    # test threshold filtering
    tlog.setThreshold(log.Log.WARN)
    tlog.log(log.Log.INFO, "I like your gloves") #  // shouldn't see this 

    # test the persistance of threshold levels
    tlog = log.Log(logger, "test")
    tlog.log(log.Log.INFO, "I like your shoes") #   // shouldn't see this 
    tlog.setThreshold(log.Log.DEBUG)
    tlog.log(log.Log.INFO, "I said, I like your shoes")

    # test descendent log and ancestor's control of threshold
    tgclog = log.Log(tlog, "grand.child")   #   // name is now "test.grand.child"
    tgclog.log(log.Log.INFO, "Let's play")
    tlog.setThreshold(log.Log.FATAL)
    tgclog.log(log.Log.INFO, "You go first")

    # test streaming
    log.LogRec(tgclog, log.Log.FATAL) << "help: I've fallen" << DataProperty("NODE", 5) << "& I can't get up" << log.endr;
    tmp = DataProperty("NODE",5)
    log.LogRec(tgclog, log.Log.FATAL) << "help: I've fallen" << tmp << "& I can't get up" << log.endr;

    # test flushing on delete
    log.LogRec(tgclog, log.Log.FATAL) << "never mind"
    log.LogRec(tgclog, log.Log.DEBUG) << "testing" << log.endr;

