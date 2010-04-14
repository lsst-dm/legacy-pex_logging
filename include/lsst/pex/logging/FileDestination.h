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
     */
    FileDestination(const std::string& filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=0) 
        : LogDestination(new std::ofstream(filepath.c_str()), formatter, 0),
          _path(filepath) 
    { }
    FileDestination(const char *filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=0)
        : LogDestination(new std::ofstream(filepath), formatter, 0),
          _path(filepath) 
    { }
    FileDestination(const fs::path& filepath,
                    const boost::shared_ptr<LogFormatter>& formatter, 
                    int threshold=0)
        : LogDestination(new std::ofstream(filepath.string().c_str()), 
                         formatter, 0),
          _path(filepath) 
    { }
    //@}

    //@{
    /**
     * create a file destination.  If the file does not exist, it will 
     * be created; otherwise, messages will be appended.  The IndentedFormatter
     * will be used to format the messages
     * @param filepath    the path to the log file to write messages to.
     * @param threshold   the minimum volume level required to pass a message
     *                       to the stream.  If not provided, it would be set
     *                       to 0.  
     */
    FileDestination(const fs::path& filepath, int threshold=0)
        : LogDestination(new std::ofstream(filepath.string().c_str()), 
                         boost::shared_ptr<LogFormatter>(new IndentedFormatter()), 0),
          _path(filepath) 
    { }
    FileDestination(const std::string& filepath, int threshold=0)
        : LogDestination(new std::ofstream(filepath.c_str()), 
                         boost::shared_ptr<LogFormatter>(new IndentedFormatter()), 0),
          _path(filepath) 
    { }
    FileDestination(const char *filepath, int threshold=0)
        : LogDestination(new std::ofstream(filepath), 
                         boost::shared_ptr<LogFormatter>(new IndentedFormatter()), 0),
          _path(filepath) 
    { }
    //@}

    virtual ~FileDestination();

    const fs::path& getPath() const { return _path; }

protected:
    fs::path _path;
};

}}}     // end lsst::pex::logging

#endif  // LSST_PEX_LOGDESTINATION_H
