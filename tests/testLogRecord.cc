#include "lsst/pex/logging/LogRecord.h"
#include <iostream>

using lsst::pex::logging::LogRecord;
using lsst::daf::base::DataProperty;
using namespace std;

void assure(bool mustBeTrue, const string& failureMsg) {
    if (! mustBeTrue)
        throw runtime_error(failureMsg);
}

int main() {

    LogRecord lr1(-1, 10);
    cout << "first record is " << ((lr1.willRecord()) ? "loud" : "quiet")
         << " and has " << lr1.getDataCount() << " records." << endl;
    assure(lr1.willRecord(), "verbose record not recording.");
    assure(lr1.getDataCount()==1, string("wrong initial data count (loud)."));

    LogRecord lr2(10, 5);
    cout << "second record is " << ((lr2.willRecord()) ? "loud" : "quiet")
         << " and has " << lr2.getDataCount() << " records." << endl;
    assure(! lr2.willRecord(), "quiet record is recording anyway.");
    assure(lr2.getDataCount()==0, string("wrong initial data count (quiet)."));
        
    const char * simple = "a simple comment";
    lr1.addComment(simple);
    lr2.addComment(simple);
    assure(lr1.getDataCount()==2, 
           string("wrong 1st updated data count (loud)."));
    assure(lr2.getDataCount()==0, 
           string("wrong 1st updated data count (quiet)."));
    
    const LogRecord::DataListT& lis = lr1.getData();
    LogRecord::DataListT::const_iterator li = lis.begin();
    assure(li != lis.end(), "no properties in LogRecord");
    assure(li->get()->getName() == "DATE", "wrong label for DATE");
    assure((++li)->get()->getName() == "COMMENT", "wrong label for COMMENT");

    li = lis.begin();
    string val1(li->get()->getName()); 
    string val2(boost::any_cast<string>((++li)->get()->getValue()));
    cout << (--li)->get()->getName() << ": " + val1 << endl;
    cout << (++li)->get()->getName() << ": " + val2 << endl;
    assure(val2 == simple, "Wrong comment value");

    typedef boost::shared_ptr<DataProperty> sharedPtrT;
    sharedPtrT dp1(new DataProperty("dpint", 2));
    sharedPtrT dp2(new DataProperty("dpfloat", 2.5));
    sharedPtrT dp3(new DataProperty("dplong", 5l));

    lr1.addProperty(dp1);
    lr2.addProperty(dp1);
    lr1.addProperty(dp2);
    lr2.addProperty(dp2);
    lr1.addProperty(dp3);
    lr2.addProperty(dp3);

    cout << "The first record now has " << lr1.getDataCount() 
         << " properties" << endl;
    cout << "The second record now has " << lr2.getDataCount() 
         << " properties" << endl;
    assure(lr1.getDataCount()==5, 
           string("wrong 1st updated data count (loud)."));
    assure(lr2.getDataCount()==0, 
           string("wrong 1st updated data count (quiet)."));

    LogRecord::DataListT preamble;
    preamble.push_back(dp1);
    preamble.push_back(dp2);
    preamble.push_back(dp3);

    LogRecord lr3(1, 5, preamble);
    cout << "The third record starts with " << lr3.getDataCount() 
         << " records."  << endl;
    assure(lr3.getDataCount()==5, "wrong initial count via preamble");
    const LogRecord::DataListT& list = lr3.getData();
    cout << "Properties: ";
    for(LogRecord::DataIteratorT i=list.begin(); i != list.end(); i++) 
        cout << (*i)->getName() << " ";
    cout << endl;
}
