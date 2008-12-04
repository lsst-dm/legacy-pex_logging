//////////////////////////////////////////////////////////////////////////////
// PropertyPrinter.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/PropertyPrinter.h"
#include "lsst/daf/base/DateTime.h"
#include <boost/any.hpp>

using std::string;

namespace lsst {
namespace pex {
namespace logging {

using std::vector;
using std::ostream;
using lsst::daf::base::PropertySet;

DateTimePrinterIter::~DateTimePrinterIter() { }

std::ostream DateTimePrinterIter::write(std::ostream *strm) const {
    (*strm) << _it->nsecs();
}

DateTimePrinterList::~DateTimePrinterList() { }

DateTimePrinterList::iterator DateTimePrinterList::begin() const { 
    PrinterIter *it = new DataTimePrinterIter(_list.begin(), 
                                              _list.end(), _list.rend());
    return iterator(boost::shared_ptr<PrinterIter>(it));
}
DateTimePrinterList::iterator DateTimePrinterList::rbegin() const { 
    PrinterIter *it = new DataTimePrinterIter(_list.rbegin(), 
                                              _list.end(), _list.rend());
    return iterator(boost::shared_ptr<PrinterIter>(it));
}

WrappedPrinterIter::~WrappedPrinterIter() { }

std::ostream WrappedPrinterIter::write(std::ostream *strm) {
    _it.get()->write(strm);
}

PrintIter& WrappedPrinterIter::operator++() { ++(_it.get()); }
PrintIter& WrappedPrinterIter::operator--() { --(_it.get()); }
bool WrappedPrinterIter::operator==(const PrinterIter& that) { 
    return (*_it == that);
}
bool WrappedPrinterIter::operator!=(const PrinterIter& that) { 
    return (*_it == that);
}
bool WrappedPrinterIter::notAtEnd() {
    return _it.get()->notAtEnd();
}
bool WrappedPrinterIter::notAtRend() {
    return _it.get()->notAtRend();
}

#define PF_ADD(T)  add(typeid(T), makePrinter<T>)

void PrinterFactory::_loadDefaults() {
    PF_ADD(short);
    PF_ADD(int);
    PF_ADD(long);
    PF_ADD(long long);
    PF_ADD(float);
    PF_ADD(double);
    PF_ADD(bool);
    PF_ADD(char);
    PF_ADD(signed char);
    PF_ADD(unsigned char);
    PF_ADD(string);
    PF_ADD(DateTime);
}

PrinterFactory PropertyPrinter::defaultPrinterFactory(true);

PropertyPrinter::PropertyPrinter(const PropertySet& prop, const string& name, 
                                 const PrinterFactory& fact) 
    : _list(fact.makePrinter(prop, name)) 
{
    if (_list.get() == 0) {
        _list = new std::vector<string>();
        _list.get()->push_back("<unprintable>");
    }
}
 
PropertyPrinter::iterator PropertyPrinter::begin() {
    return _list.get()->begin();
}

PropertyPrinter::iterator PropertyPrinter::rbegin() {
    return _list.get()->rbegin();
}




PropertyPrinter::PropertyPrinter(const PropertySet& prop, const string& name) 
    : _list(), _tp(prop.typeOf(name));
{

    if (tp == typeid(int)) {
        _list = PrintList<int>(prop, name);
    }
    else if (tp == typeid(long long)) {
        _list = PrintList<long long>(name);
    }
    else if (tp == typeid(long)) {
        _list = PrintList<long>(name);
    }
    else if (tp == typeid(float)) {
        _list = PrintList<float>(name);
    }
    else if (tp == typeid(double)) {
        _list = PrintList<double>(name);
    }
    else if (tp == typeid(bool)) {
        _list = PrintList<bool>(name);
    }
    else if (tp == typeid(string)) {
        _list = PrintList<string>(name);
    }
    else if (tp == typeid(short)) {
        _list = PrintList<short>(name);
    }
    else if (tp == typeid(long)) {
        _list = PrintList<unsigned long>(name);
    }
    else if (tp == typeid(char)) {
        _list = PrintList<char>(name);
    }
    else if (tp == typeid(signed char)) {
        _list = PrintList<signed char>(name);
    }
    else if (tp == typeid(unsigned char)) {
        _list = PrintList<unsigned char>(name);
    }
    else {
        vector<string> standin(1);
        standin.push_back("<unprintable>");
        _list = standin;
    }
}



}}} // end lsst::pex::logging

