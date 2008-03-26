"""demonstrate simple use of the Log facility."""
"""
testLog

Run with:
   python testLog
"""

import lsst.mwi.tests as tests
import lsst.mwi.utils as mwiu
import lsst.mwi.logging as log
from lsst.mwi.data import DataProperty

if __name__ == "__main__":

#    // now let's create our own root log
    logger = log.ScreenLog(1)

    print "before\n"
    log.LogRec(logger, log.Log.FATAL) << "not working" << log.LogRec.endr;
    print "after\n"
#    // test flushing on delete
    log.LogRec(logger, log.Log.FATAL) << "never mind"
