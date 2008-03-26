/**
 * @brief  This will test some LogRecord basics and LogFormatter.
 */

#include "lsst/mwi/logging/LogFormatter.h"
#include "lsst/mwi/logging/LogRecord.h"
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>

using lsst::mwi::logging::LogRecord;
using lsst::mwi::logging::LogFormatter;
using lsst::mwi::logging::BriefFormatter;
using lsst::mwi::logging::NetLoggerFormatter;
using lsst::mwi::data::DataProperty;
using namespace std;

#define Assert(b, m) tattle(b, m, __LINE__)

void tattle(bool mustBeTrue, const string& failureMsg, int line) {
    if (! mustBeTrue) {
        ostringstream msg;
        msg << __FILE__ << ':' << line << ":\n" << failureMsg << ends;
        throw runtime_error(msg.str());
    }
}

using boost::regex;
using boost::regex_search;

int main() {

    auto_ptr<ostringstream> cap;
    string msg;
    regex datestr("DATE: \\d{4}\\-\\d{2}\\-\\d{2}T\\d{2}:\\d{2}:\\d{2}\\.\\d+");

    typedef boost::shared_ptr<DataProperty> sharedPtrT;
    sharedPtrT dp1(new DataProperty("HOST", string("localhost.localdomain")));
    sharedPtrT dp2(new DataProperty("IP", string("111.111.111.111")));
    sharedPtrT dp3(new DataProperty("LOG", string("tester")));

    LogRecord::DataListT preamble;
    preamble.push_back(dp1);
    preamble.push_back(dp2);
    preamble.push_back(dp3);

    LogRecord lr1(1, 5, preamble);
    LogRecord lr2(5, 1, preamble);

    lr1.addComment("This is a test");
    lr2.addComment("This is a test");

    Assert(lr1.getDataCount() == 5, "Missing comment or preamble property");
    Assert(lr2.getDataCount() == 0, "Comment or preamble property not ignored");

    LogFormatter *brief = new BriefFormatter();
    LogFormatter *notsobrief = new BriefFormatter(true);
    LogFormatter *nl = new NetLoggerFormatter();

    cap.reset(new ostringstream());
    brief->write(cap.get(), lr1);
    msg = cap->str();
    cout << msg;
    Assert(msg == "tester: This is a test\n", "Brief formatting failed");
    cout << "-------------" << endl;

    cap.reset(new ostringstream());
    notsobrief->write(cap.get(), lr1);
    msg = cap->str();
    cout << msg;
    Assert(msg.find("tester: This is a test\n") == 0, 
           "Not-so-brief formatting miswrote comment");
    Assert(msg.find("  HOST: localhost.localdomain\n") != string::npos,
           "Not-so-brief formatting miswrote HOST");
    Assert(msg.find("  IP: 111.111.111.111\n") != string::npos,
           "Not-so-brief formatting miswrote IP");
    Assert(regex_search(msg, datestr), "Not-so-brief formatting miswrote Date");
    cout << "-------------" << endl;

    cap.reset(new ostringstream());
    nl->write(cap.get(), lr1);
    msg = cap->str();
    cout << msg;
    Assert(msg.find("s HOST: localhost.localdomain\n") != string::npos,
           "Netlogger formatting miswrote HOST");
    Assert(msg.find("s IP: 111.111.111.111\n") != string::npos,
           "Netlogger formatting miswrote IP");
    Assert(msg.find("s LOG: tester\n") != string::npos,
           "Netlogger formatting miswrote LOG");
    Assert(msg.find("s COMMENT: This is a test\n") != string::npos,
           "Netlogger formatting miswrote COMMENT");
    Assert(msg.find("t DATE: ") != string::npos,
           "Netlogger formatting miswrote DATE");
    Assert(regex_search(msg, datestr), "Netlogger formatting miswrote Date");
    cout << "=============" << endl;

    cout << "The next logging calls should just produce lines:" << endl;
    cap.reset(new ostringstream());
    brief->write(&cout, lr2);
    msg = cap->str();
    cout << msg;
    Assert(msg.size() == 0, "Brief silent operation failed");
    cout << "-------------" << endl;

    cap.reset(new ostringstream());
    notsobrief->write(&cout, lr2);
    msg = cap->str();
    cout << msg;
    Assert(msg.size() == 0, "Brief silent operation failed");
    cout << "-------------" << endl;

    cap.reset(new ostringstream());
    nl->write(&cout, lr2);
    msg = cap->str();
    cout << msg;
    Assert(msg.size() == 0, "Brief silent operation failed");
    cout << "=============" << endl;

    LogRecord lr3(1, 5);
    lr3.addComment("another test");
    brief->write(&cout, lr3); 

    LogRecord lr4(1, 5);
    lr4.addProperty(DataProperty("LOG", string("tester")));
    brief->write(&cout, lr4); 

    delete notsobrief;
    delete brief;
    delete nl;

}
