"""
@brief Demonstrate the use of the logging framework from Python
"""

import lsst.pex.logging as log
from lsst.pex.logging import Rec, Prop, endr

if __name__ == "__main__":

    # in any script or function where you want to log message, the first
    # thing you should do is get create a Log from the DefaultLog
    mylog = log.Log(log.getDefaultLog(), "myapp.myfunc")

    # simple message can be sent with a function.  The first argument is
    # the level of message; choices are: DEBUG, INFO, WARN, FATAL.  
    mylog.log(mylog.INFO, "this is a simple message");

    # If you want to send multiple messages and/or properties all in the
    # same message, you can use the shift operator.  Be sure to end the
    # message with "endr"
    Rec(mylog, mylog.WARN) << "ouch!" << Prop("NODE", 5) \
                           << "something bad happened!" << endr

    # Normally properties are not printed to the screen.  To see these, we'll
    # turn them on now.
    #
    # Outside the pipeline framework, the default logger is a ScreenLog.
    # If you want to see properties on the screen, you put this code anywhere
    # in your python script.  Note that the effect is global.  
    dlog = log.getDefaultLog();
    if isinstance(dlog, log.ScreenLog):
        dlog.setScreenVerbose(True)

    # now try the complex message again:
    Rec(mylog, mylog.FATAL) << "ouch!" << Prop("rms", 5.0) \
                            << "something really bad happened!" << endr

    # you can also send a whole dictionary along.  The values must be 
    # of types int, long, float, boolean, or string.  They can also be lists or
    # dictionaries of those types.
    Rec(mylog, mylog.INFO) << "working with properties" \
               << { "iterations": 458, "rms": 0.00245, "converged": False } \
               << endr

