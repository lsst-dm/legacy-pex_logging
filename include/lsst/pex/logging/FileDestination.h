// -*- lsst-c++ -*-
/**
 * @file FileDestination.h
 * @brief definition of the FileDestination class
 * @author Ray Plante
 */
#ifndef LSST_PEX_FILEDESTINATION_H
#define LSST_PEX_FILEDESTINATION_H

#include "lsst/pex/logging/LogDestination.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>

namespace fs = boost::filesystem;

namespace lsst {
namespace pex {
namespace logging {

/**
 * @brief  a LogDestination represented by a file.  
 *
 * This class makes it easier to attach files to Logs via Python via 
 * Log.addDestination().
 */
class FileDestination : public LogDestination {
public:

    //@{
    /**
     * create a file destination.  If the file does not exist, it will 
     * be created; otherwise, messages will be appended.
     * @param filepath    the path to the log file to write messages to.
     * @param formatter   the LogFormatter to use to format the messages 
     * @param threshold   the minimum volume level required to pass a message
     *                       to the stream.  If not provided, it would be set
     *                       to 0.  
     * @param truncate    if True, overwrite the previous contents; otherwise,
     *                       new messages will be appended to the file.
     */
    FileDestination(const std::string& filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=threshold::PASS_ALL,
                    bool truncate=false) 
        : LogDestination(new std::ofstream(filepath.c_str(),
                                           truncate ? std::ios::out 
                                                    : std::ios::app), 
                         formatter, threshold),
          _path(filepath) 
    { }
    FileDestination(const char *filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=threshold::PASS_ALL,
                    bool truncate=false)
        : LogDestination(new std::ofstream(filepath,
                                           truncate ? std::ios::out 
                                                    : std::ios::app), 
                         formatter, threshold),
          _path(filepath) 
    { }
    FileDestination(const fs::path& filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=threshold::PASS_ALL,
                    bool truncate=false)
        : LogDestination(new std::ofstream(filepath.string().c_str(),
                                           truncate ? std::ios::out 
                                                    : std::ios::app), 
                         formatter, threshold),
          _path(filepath) 
    { }
    //@}

    //@{
    /**
     * create a file destination.  If the file does not exist, it will 
     * be created; otherwise, messages will be appended.  The IndentedFormatter
     * will be used to format the messages
     * @param filepath    the path to the log file to write messages to.
     * @param verbose     if True, make sure all properties are printed 
     *                       out to the destination
     * @param threshold   the minimum volume level required to pass a message
     *                       to the stream.  If not provided, it would be set
     *                       to 0.  
     * @param truncate    if True, overwrite the previous contents; otherwise,
     *                       new messages will be appended to the file.
     */
    FileDestination(const fs::path& filepath, bool verbose=false, 
                    int threshold=threshold::PASS_ALL, bool truncate=false);
    FileDestination(const std::string& filepath, bool verbose=false, 
                    int threshold=threshold::PASS_ALL, bool truncate=false);
    FileDestination(const char *filepath, bool verbose=false, 
                    int threshold=threshold::PASS_ALL, bool truncate=false);
    //@}

    virtual ~FileDestination();

    const fs::path& getPath() const { return _path; }

protected:
    fs::path _path;
};

}}}     // end lsst::pex::logging

#endif  // LSST_PEX_LOGDESTINATION_H
