// -*- lsst-c++ -*-
#ifndef LSST_MWI_DUALLOG_H
#define LSST_MWI_DUALLOG_H

#include "lsst/mwi/logging/ScreenLog.h"

#include <fstream>

namespace lsst {
namespace mwi {
namespace logging {

using std::vector;
using boost::shared_ptr;
using lsst::mwi::data::DataProperty;
using std::ofstream;

/**
 * @brief a Log that sends message to both the screen and a file. 
 *
 * This class uses the BriefFormatter for printing to the screen (via cout) 
 * and the NetLoggerFormatter for the file.  This class provides an interface 
 * for controlling the amount printed to the screen (setPrintAllProps()) as 
 * well as the thresholds on the individual destinations.  
 */
class DualLog : public ScreenLog {
public:

    /**
     * create a Log that will write messages to a given file
     * @param filename      the name of the file to send messages to.  It will 
     *                        be opened in append mode.
     * @param filethresh    the verbosity threshold to set for the log file
     * @param screenthresh  the verbosity threshold to set for messages going
     *                        to the screen.
     * @param screenVerbose if true, all message data properties will be printed
     *                        to the screen.  If false, only the Log name 
     *                        ("LOG") and the text comment ("COMMENT") will be
     *                        printed.
     * @param preamble       a list of data properties that should be included 
     *                         with every recorded message to the Log.  This
     *                         constructor will automatically add a property 
     *                         ("LOG") giving the Log name.  
     */
    DualLog(const string& filename, int filethresh=0, int screenthresh=0,
            bool screenVerbose=false,
            const vector<shared_ptr<DataProperty> > *preamble=0);

    /**
     * create a copy
     */
    DualLog(const DualLog& that) 
        : ScreenLog(that), _file(that._file) 
    { }

    /**
     * delete this log
     */
    virtual ~DualLog();

    /**
     * copy another one into this
     */
    DualLog& operator=(const DualLog& that) {
        ScreenLog::operator=(that);
        _file = that._file; 
        return *this;
    }

    /**
     * return the verbosity threshold current set for the log file. 
     * This applies only to the file stream set at construction time.
     */
    int getFileThreshold() { return _file->getThreshold(); }

    /**
     * set the verbosity threshold current set for the log file.
     * This only affects the file stream set at construction time; other 
     * subsequently added streams are unaffected.
     */
    void setFileThreshold(int thresh) { _file->setThreshold(thresh); }

    /**
     * create a new log and set it as the default Log
     * @param filename      the name of the file to send messages to.  It will 
     *                        be opened in append mode.
     * @param filethresh    the verbosity threshold to set for the log file
     * @param screenthresh  the verbosity threshold to set for messages going
     *                        to the screen.
     * @param screenVerbose if true, all message data properties will be 
     *                        printed to the screen.  If false, only the Log 
     *                        name ("LOG") and the text comment ("COMMENT") 
     *                        will be printed.
     * @param preamble      a list of data properties that should be included 
     *                        with every recorded message to the Log.  This
     *                        constructor will automatically add a property 
     *                        ("LOG") giving the Log name.  
     */
    static void createDefaultLog(
        const string& filename, int filethresh=0, 
        int screenthresh=0, bool screenVerbose=false,
        const vector<shared_ptr<DataProperty> > *preamble=0);


private:
    void setPreamble(int node);

    LogDestination *_file;
    ofstream *fstrm;
};

}}}     // end lsst::mwi::logging

#endif  // end LSST_MWI_DUALLOG_H
