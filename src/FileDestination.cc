/**
 * @file FileDestination.cc
 * @author Ray Plante
 */
#include "lsst/pex/logging/FileDestination.h"

namespace lsst {
namespace pex {
namespace logging {

/**
 * create a file destination.  If the file does not exist, it will 
 * be created; otherwise, messages will be appended.  The IndentedFormatter
 * will be used to format the messages
 * @param filepath    the path to the log file to write messages to.
 * @param threshold   the minimum volume level required to pass a message
 *                       to the stream.  If not provided, it would be set
 *                       to 0.  
 * @param truncate    if True, overwrite the previous contents; otherwise,
 *                       new messages will be appended to the file.
 */
FileDestination::FileDestination(const fs::path& filepath, bool verbose, 
                                 int threshold, bool truncate)
    : LogDestination(new std::ofstream(filepath.string().c_str(), 
                                       truncate ? std::ios::out : std::ios::app), 
                     boost::shared_ptr<LogFormatter>(new IndentedFormatter(verbose)), 
                     threshold),
      _path(filepath) 
{ }
FileDestination::FileDestination(const std::string& filepath, bool verbose, 
                                 int threshold, bool truncate)
    : LogDestination(new std::ofstream(filepath.c_str(), 
                                       truncate ? std::ios::out : std::ios::app), 
                     boost::shared_ptr<LogFormatter>(new IndentedFormatter(verbose)), 
                     threshold),
      _path(filepath) 
{ }
FileDestination::FileDestination(const char *filepath, bool verbose, 
                                 int threshold, bool truncate)
    : LogDestination(new std::ofstream(filepath, 
                                       truncate ? std::ios::out : std::ios::app), 
                     boost::shared_ptr<LogFormatter>(new IndentedFormatter(verbose)), 
                     threshold),
      _path(filepath) 
{ }

/*
 * delete this destination
 */
FileDestination::~FileDestination() { 
    std::ofstream *ofstrm = dynamic_cast<std::ofstream*>(_strm);
    if (ofstrm != NULL && ofstrm->is_open()) {
        try {
            ofstrm->flush();
            ofstrm->close();
        }
        catch (...) { }
    }
    delete _strm;
}




}}}
