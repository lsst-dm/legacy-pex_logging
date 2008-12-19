// -*- lsst-c++ -*-
#ifndef LSST_PEX_LOG_H
#define LSST_PEX_LOG_H

#include "lsst/daf/base/PropertySet.h"
#include "lsst/pex/logging/LogRecord.h"
#include "lsst/pex/logging/LogDestination.h"
#include "lsst/pex/logging/Component.h"

#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>

namespace lsst {
namespace pex {
namespace logging {

using std::string;
using std::list;
using std::ostream;
using boost::shared_ptr;
using lsst::daf::base::PropertySet;

/**
 * @brief a place to record messages and descriptions of the state of 
 * processing. 
 *
 * This class is the centerpiece of the logging framework.  It allows modules
 * to record plain text statements about what is happening (including errors,
 * warnings, status information, and debugging messages) as well as typed
 * data delivered as named properties.  It allows users to conveniently
 * tag their messages with a hierarchical label that can indicate where in 
 * the application the message originates.  Messages sent to a Log can be 
 * routed to multiple destinations, such as to the terminal screen, to 
 * a file and to the Event system, all simultaneously.  Messages are also 
 * tagged with a verbosity or "loudness" level, and Logs are set with verbosity 
 * thresholds.  If a message's verbosity level is less than the Log's 
 * threshold, it will not be recorded.  This allows applications and modules to 
 * selectively turn on and off, say, debugging messages of which there may be 
 * a great number.  Thresholds are implemented to minimize the impact of 
 * unrecorded messages on overall performance, so debugging messages can 
 * remain permanently compiled into the code.  Finally, the different streams
 * that receive log messages can have their own thresholds set as well; this
 * allows one to, for example, send more messages to a file than to the screen.
 * 
 * Logs used by an application are organized into a hierarchy.  Applications 
 * normally get the root Log via the static method Log::getDefaultLog().  This
 * log by default is configured to print messages to the screen (although
 * a production application will typically replace the default Log with one 
 * that sends messages elsewhere).  Alternatively, an application can configure 
 * its own root Log via the Log constructors, or with the convenience 
 * subclasses, ScreenLog and DualLog (used to send messages to both the screen
 * and a file).  It is possible to have multiple, independently-configured 
 * Log hierarchies, but it is intended that one is sufficient for an entire 
 * application.  
 * 
 * A root Log has an empty string name associated with it.  A module will 
 * usually create a "child" or "descendent" Log object to send its messages 
 * to, giving it a period-delimited name and a verbosity threshold:
 * 
 *     Log mylog(Log::getDefaultLog(), "myapp.mymod", Log::INFO);
 *
 * or 
 *
 *     scoped_ptr<Log> 
 *           mylog(getDefaultLog().createChildLog("myapp.mymod", Log::INFO));
 *
 * The new log will have the name "myapp.mymod" and allows messages at least 
 * as "loud" as Log::INFO.  The defined levels, FATAL, WARN, INFO, DEBUG (set 
 * to 20, 10, 0, -10, respectively) are conventional notions to encourage 
 * uniformity of levels across modules.  The sub-names between the periods
 * indicate the levels of the Logging hierarchy.  Note that once a threshold
 * is set for a log at a particular level, it is remembered even after that
 * log is deleted; when anther log is created with that name, its previous
 * threshold is preserved.  If no threshold is provided, it will default to 
 * that of its parents log.  Furthermore, setting the threshold for a parent
 * automatically updates the threshold for all of its descendents that have 
 * not otherwise set the threshold.  For example,
 *
 *   Log root = Log::getDefaultLog();   // the root log; name is an empty string
 *   Log applog(root, "myapp");         // myapp: a child log
 *   Log *modlog = applog.createChildLog("mymod", Log::WARN);
 *                                      // myapp.mymod: a quieter grandchild log
 *   delete modlog;
 *   modlog = applog.createChildLog("mymod")  // still set to Log::WARN
 *   
 *   root.setThreshold(log::DEBUG);     // root and applog now set to Log::DEBUG
 *   modlog.resetThreshold();           // modlog now also set to Log::DEBUG
 * 
 * As you can see in the example, names are set relative to their parent.  
 * That is, when we use a log called "myapp" to create a child called 
 * "mymod", the full name is "myapp.mymod".  This allows a Log to be made 
 * a member of a class and given a name without knowing the context in which 
 * the class is being used.  Note that it is not necessary to create
 * logs at intervening levels; the first example above, shows us directly 
 * creating a "grandchild" from the root Log.  
 *
 * Simple text log messages can be recorded with the log() functions:  
 * 
 *     mylog.log(Log::WARN, "Skipping initialization");
 *     mylog.log(Log::WARN, boost::format("Found only %d objects") % nobj);
 *
 * For more complex messages that might include named properties, it is usually
 * more convenient to use the streaming method:
 * 
 *     using lsst::pex::logging::Rec;
 *     using lsst::pex::logging::Prop;
 *     Rec(mylog, Log::DEBUG) << "Completed deconvolution"
 *                            << Prop("iterations", 541)
 *                            << Prop("rms", 0.0032)
 *                            << Rec::endr;   
 *
 *     Rec(mylog, Log::INFO) << boost::format("applying %s kernel") % kern
 *                           << Prop("width", width);
 *                           << Rec::endr;
 * 
 * The manipulator Rec::endr triggers the recording of the message to the log. 
 * Note that Rec is a shorthand typedef for the LogRec class.  
 */
class Log {
public:

    /**
     * the conventional verbosity level for messages that aid in debugging.  
     * This value is set to -10 with the intention that messages with 
     * negative verbosity levels (or more precisely, >= -10) will be printed 
     * when the threshold is set to this value.  
     */
    static const int DEBUG;

    /**
     * the conventional verbosity level for messages that are informational
     * and which report on normal behavior.  The value is set to 0 with the 
     * intention that this is the default threshold for logs and their 
     * destinations.  
     */
    static const int INFO;

    /**
     * the conventional verbosity level for messages that warn about 
     * abnormal but non-fatal behavior.  The value is set to 10.
     */
    static const int WARN;

    /**
     * a magic threshold value that indicates that a threshold of a Log
     * should be set to its nearest ancestor
     */
    static const int INHERIT_THRESHOLD;

    /**
     * the conventional verbosity level for messages that report on fatal
     * behavior.  The value is set to 20.  Note that the logging module 
     * makes no attempt to shutdown execution or other wise affect control 
     * flow when a message having a verbosity exceeding this level.  
     */
    static const int FATAL;

    /**
     * create a null log.  This constructor should 
     * not normally be employed to obtain a Log; the static getDefaultLog() 
     * method should be used instead.  This is provided primarily for 
     * subclasses and containers that require a no-arg constructor.  
     * @param threshold   the initial verbosity threshold for this log
     * @param name        the initial name for this log.  An empty string 
     *                    (the default) denotes a root log.
     */
    Log(const int threshold=INFO, const string& name="");

    /**
     * create a fully configured Log.  This constructor is 
     * not normally employed to obtain a Log; the static getDefaultLog() 
     * method or the createChildLog() method of should be used instead.  
     * @param destinations   the list of LogDestinations to attach to this 
     *                         Log.
     * @param preamble       a data properties that should be included 
     *                         with every recorded message to the Log.  This
     *                         constructor will automatically add the properties
     *                         "LOG", giving the Log name and "DATE", giving
     *                         the timestamp the message was recorded.
     * @param name           the name to give this log.  By default, the 
     *                         name will be an empty string, signifying a 
     *                         root log.  
     * @param threshold      the verbosity threshold to assign to this Log.  
     *                         Messages sent to this log must have a verbosity
     *                         level equal to or greater than this value to 
     *                         be recorded.  (Note that thresholds associated
     *                         with destinations have their own thresholds that
     *                         will override this one.)
     */
    Log(const list<shared_ptr<LogDestination> >& destinations, 
        const PropertySet& preamble,
        const string& name="", const int threshold=INFO);

    /**
     * create a child of a given Log.  The child log will be attached 
     * to the same streams as the provided parent Log.  The full child 
     * name will be constructed from the given name, prepended by the 
     * parent's name and a ".".  That is, if the parent is named "coo.coo"
     * and the name given here is "ca-choo", then the full name will be 
     * "coo.coo.ca.choo".  If the parent Log is a root Log, then the "." 
     * is not prepended.
     *
     * @param parent        the Log that will serve as its parent.  
     * @param childName     the name of the child log, relative to the given 
     *                          parent.  
     * @param threshold     the threshold for the new child Log.  If not 
     *                          provided, the value will be set to the value
     *                          it had the last time it was created, or if it
     *                          is being created for the first time, it is
     *                          set to track the threshold of the parent. 
     */
    Log(const Log& parent, const string& childName, 
        int threshold=INHERIT_THRESHOLD);

    /**
     * create a copy
     */
    Log(const Log& that);

    /** 
     * delete this Log
     */
    virtual ~Log();

    /**
     * copy another log to this one.  
     */
    Log& operator=(const Log& that);

    /**
     * return the name associated with this Log.  The root logger 
     * will return an empty stream as its name.
     */
    const std::string& getName() const { return _name; }

    /**
     * return the verbosity threshold for this log.  A message sent to this 
     * Log will not be recorded if the message verbosity is less than the 
     * threshold. 
     */
    int getThreshold() const { 
        return ((_threshold > INHERIT_THRESHOLD) 
                       ? _threshold
                       : _thresholds->getVerbosity(_name, _sep) );
    }

    /**
     * set the verbosity threshold for this log.  A message sent to this 
     * Log will not be recorded if the message verbosity is less than the 
     * threshold. 
     * @param threshold    the new threshold value
     */
    void setThreshold(int threshold) { 
        _threshold = threshold;
        _thresholds->add(_name, threshold, _sep);
    } 

    /**
     * return true if the threshold is low enough to pass messages of a 
     * given loudness or verbosity.
     */
    bool sends(int verbosity) const { return (verbosity >= getThreshold()); }

    /**
     * reset the verbosity threshold of this log to that of its parent 
     * threshold.  If this is a root Log, the threshold will be set to INFO.
     */
    void resetThreshold() { setThreshold(INHERIT_THRESHOLD); }

    /**
     * set the verbosity threshold for a child Log.  When a child Log of the
     * same name is created, it will be assigned this threshold.  Any existing
     * Log object with name that has already explicitly set its verbosity
     * threshold will not be affected; however, those that are set to inherit
     * the threshold will be. 
     * @param name       the relative name of the child log
     * @param threshold  the verbosity threshold to set Logs with this name to.
     */
    void setThresholdFor(const string& name, int threshold);

    /**
     * get the verbosity threshold for a child Log.  When a child Log of the
     * same name is created, this is the threshold it will have.  
     * @param name       the relative name of the child log
     * @param threshold  the verbosity threshold to set Logs with this name to.
     */
    int getThresholdFor(const string& name) const;

    /**
     * add a property to the preamble
     */
    template <class T>
    void addPreambleProperty(const string& name, const T& val);

    /**
     * set a property to the preamble, overwriting any value with 
     * the same name.
     */
    template <class T>
    void setPreambleProperty(const string& name, const T& val);

    /**
     * create a child of a given Log.  The child log will be attached 
     * to the same streams as the provided parent Log.  The full child 
     * name will be constructed from the given name, prepended by the 
     * parent's name and a ".".  That is, if the parent is named "coo.coo"
     * and the name given here is "ca-choo", then the full name will be 
     * "coo.coo.ca.choo".  If the parent Log is a root Log, then the "." 
     * is not prepended.
     *
     * @param childName     the name of the child log, relative to the given 
     *                          parent.  
     * @param threshold     the threshold for the new child Log.  If not 
     *                          provided, the value will be set to the value
     *                          it had the last time it was created, or if it
     *                          is being created for the first time, it is
     *                          set to track the threshold of the parent. 
     */
    Log *createChildLog(const string& childName, 
                        int threshold=INHERIT_THRESHOLD) const;

    /**
     * send a message to the log
     * @param verbosity    how loud the message should be
     * @param message      a simple bit of text to send in the message
     * @param properties   a list of properties to include in the message.
     */
    void log(int verbosity, const string& message, 
             const PropertySet& properties);

    /**
     * send a message to the log
     * @param verbosity    how loud the message should be
     * @param message      a simple bit of text to send in the message
     * @param prop         a property to include in the message.
     */
    template <class T>
    void log(int verbosity, const string& message, 
             const string& name, const T& val);

    /**
     * send a message to the log
     * @param verbosity    how loud the message should be
     * @param message      a simple bit of text to send in the message
     * @param prop         a property to include in the message.
     */
    template <class T>
    void log(int verbosity, const string& message, 
             const RecordProperty<T>& prop);

    /**
     * send a simple message to the log
     * @param verbosity    how loud the message should be
     * @param message      a simple bit of text to send in the message
     */
    void log(int verbosity, const string& message);

    /**
     * send a simple message to the log
     * @param verbosity    how loud the message should be
     * @param message      a simple message in the form of a boost::format
     *                       instance
     */
    void log(int verbosity, const boost::format& message) {
        log(verbosity, message.str());
    }

    /**
     * send a fully formed LogRecord to the log destinations
     */
    void send(const LogRecord& record);

    /**
     * add a destination to this log.  The destination stream will included
     * in all child Logs created from this log after a call to this function.
     * All previously created logs, including ancestor logs, will be 
     * unaffected.  The NetLogger format will be used with this new 
     * destination.  
     * @param destination   the stream to send messages to
     * @param threshold     the verbosity threshold to use to filter messages
     *                         sent to the stream.
     */
    void addDestination(ostream& destination, int threshold);

    /**
     * add a destination to this log.  The destination stream will included
     * in all child Logs created from this log after a call to this function.
     * All previously created logs, including ancestor logs, will be 
     * unaffected.  
     * @param destination   a pointer to the stream to send messages to.  The
     *                         caller is responsible for ensuring that the 
     *                         stream is neither closed nor its memory freed
     *                         for the life of this Log.  
     * @param threshold     the verbosity threshold to use to filter messages
     *                         sent to the stream.
     * @param formatter     the log formatter to use.
     */
    void addDestination(ostream &destination, int threshold, 
                        const shared_ptr<LogFormatter> &formatter);

    /**
     * add a destination to this log.  The destination stream will included
     * in all child Logs created from this log after a call to this function.
     * All previously created logs, including ancestor logs, will be 
     * unaffected.  
     */
    void addDestination(const shared_ptr<LogDestination> &destination) {
        _destinations.push_back(destination);
    }

    /** 
     * return the current set of preamble properties
     */
    const PropertySet& getPreamble() { return *_preamble; }

    /**
     * obtain the default root Log instance.
     */
    static Log& getDefaultLog();

    /**
     * create a new log and set it as the default Log
     * @param destinations   the list of LogDestinations to attach to this 
     *                         Log.
     * @param preamble       a list of data properties that should be included 
     *                         with every recorded message to the Log.  This
     *                         constructor will automatically add the properties
     *                         "LOG", giving the Log name and "DATE", giving
     *                         the timestamp the message was recorded.
     * @param name           the name to give this log.  By default, the 
     *                         name will be an empty string, signifying a 
     *                         root log.  
     * @param threshold      the verbosity threshold to assign to this Log.  
     *                         Messages sent to this log must have a verbosity
     *                         level equal to or greater than this value to 
     *                         be recorded.  (Note that thresholds associated
     *                         with destinations have their own thresholds that
     *                         will override this one.)
     */
    static void createDefaultLog(
        const list<shared_ptr<LogDestination> >& destinations, 
        const PropertySet& preamble,
        const string& name="", const int threshold=INFO);

    /**
     * shutdown and destroy the default log
     */
    static void closeDefaultLog();

protected:
    /**
     * the default Log
     */
    static Log *defaultLog;

    /**
     * set the default Log.  The pointer sent to this method should be 
     * a freshly minted pointer, "unowned" by anyone.  That is, the caller 
     * relinguishes responsibility for deleting the log by calling this 
     * function.
     */
    static void setDefaultLog(Log* deflog);

    static const string _sep;

private:
    void completePreamble();

    int _threshold;
    std::string _name;

protected: 
    /**
     * the memory of child verbosity thresholds.
     */
    shared_ptr<Component> _thresholds;

    /**
     * the list of destinations to send messages to
     */
    list<shared_ptr<LogDestination> > _destinations;

    /**
     * the list preamble data properties that are included with every 
     * log record.
     */
    PropertySet::Ptr _preamble;
};

template <class T>
void Log::addPreambleProperty(const string& name, const T& val) {
    _preamble->add<T>(name, val);
}

template <class T>
void Log::setPreambleProperty(const string& name, const T& val) {
    _preamble->set<T>(name, val);
}
        
template <class T>
void Log::log(int verbosity, const string& message, 
              const string& name, const T& val) {

    int threshold = getThreshold();
    if (verbosity < threshold)
        return;
    LogRecord rec(threshold, verbosity, *_preamble);
    rec.addComment(message);
    rec.addProperty(name, val);
    send(rec);
}

template <class T>
void Log::log(int verbosity, const string& message, 
         const RecordProperty<T>& prop) 
{
    log(verbosity, message, prop.name, prop.value);
}


/**
 * @brief  A LogRecord attached to a particular Log that suppports stream 
 * stream semantics.  
 * 
 */
class LogRec : public LogRecord {
public:

    /**
     * Create a log record to be sent to a given log.  
     * @param log        the log this record should be written to
     * @param verbosity  the loudness of the record.  If this value is 
     *                     greater than or equal to the Log's verbosity 
     *                     threshold, the message will be recorded.
     */
    LogRec(Log& log, int verbosity) 
        : LogRecord(log.getThreshold(), verbosity, log.getPreamble()), 
          _sent(false), _log(&log)
    { }

    /**
     * create a copy
     */
    LogRec(const LogRec& that) 
        : LogRecord(that), _sent(false), _log(that._log) 
    { }

    /**
     * delete this record
     */
    virtual ~LogRec();

    /**
     * copy another record into this one
     */
    LogRec& operator=(const LogRec& that) {
        LogRecord::operator=(that);
        _log = that._log;
        return *this;
    }

    /**
     * Record manipulators.  These values may be sent to this LogRec via 
     * the output operator to request a particular manipulation of the message.
     */
    enum Manip { 
        /**
         * a manipulator indicating that the message is complete and should 
         * be sent to the Log.
         */
        endr 
    };

    /**
     * record a string comment into this message
     */
    LogRec& operator<<(const string& comment); /* {
        addComment(comment);
        return *this;
    }  */

    /**
     * record a string comment into this message
     */
    LogRec& operator<<(const boost::format& comment) {
        addComment(comment);
        return *this;
    }

    /**
     * record a data property into this message
     */
    template <class T>
    LogRec& operator<<(const RecordProperty<T>& prop) {
        addProperty(prop.name, prop.value);
        return *this;
    }

    /**
     * record a data property into this message
     */
    LogRec& operator<<(const PropertySet& props) {
        addProperties(props);
        return *this;
    }

    /**
     * receive a message manipulator signal.  The most common use of this 
     * version is to indicate that the Record is now complete and should 
     * be flushed to the Log.
     * @param signal    the manipulator signal value (e.g. LogRec::endr).  
     */
    LogRec& operator<<(Manip signal) {
        if (_send) {
            switch (signal) {
            case endr:
                _log->send(*this);
                _sent = true;
                break;
            default:
                ;
            }
        }
        return *this;
    }

private:
    bool _sent;
    Log *_log;
};

/**
 * a shorthand version of the LogRec class
 */
typedef LogRec Rec;

}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_LOG_H
