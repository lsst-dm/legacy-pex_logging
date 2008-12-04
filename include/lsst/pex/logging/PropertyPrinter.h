// -*- lsst-c++ -*-
#ifndef LSST_PEX_PROPERTYPRINTER_H
#define LSST_PEX_PROPERTYPRINTER_H

#include <string>
#include <ostream>
#include <typeinfo>

#include "lsst/daf/base/PropertySet.h"
#include "lsst/pex/logging/TypeInfoLess.h"
#include "boost/any.hpp"

namespace lsst {
namespace pex {
namespace logging {

/**
 * @brief an abstract iterator class used to print out property values
 *
 * The main purpose of implementations of this class is to provide (via 
 * its write(std::ostream*) function) the means for printing the values.
 * The other functions support iteration.  
 */
class PrinterIter {
public:

    /**
     * write the value pointed to by this iterator to the given stream
     */
    virtual std::ostream write(std::ostream *strm) const=0;

    /**
     * move the iterator forward one position
     */
    virtual PrinterIter& operator++()=0;

    /**
     * move the iterator backward one position
     */
    virtual PrinterIter& operator--()=0;

    /**
     * return true if this iterator is pointing to the same value as 
     * another pointer.  This method is not recommend for testing whether
     * an iterator is past the beginning or end of the list; notAtEnd() 
     * and notAtRend() are more efficient.
     */
    virtual bool operator==(const PrinterIter& that) const=0;

    /**
     * return true if this iterator is not pointing to the same value as 
     * another pointer.  This method is not recommend for testing whether
     * an iterator is past the beginning or end of the list; notAtEnd() 
     * and notAtRend() are more efficient.
     */
    virtual bool operator!=(const PrinterIter& that) const=0;

    /**
     * return true if this iterator is not pointing past the last value
     * in the list.  Thus, *this should return a legal string.
     */
    virtual bool notAtEnd() const=0;

    /**
     * return true if this iterator is not pointing past the last value
     * in the list.  Thus, *this should return a legal string.
     */
    virtual bool notAtRend() const=0;

    /**
     * return a string version of the property value.  This simply uses
     * write(std::ostream*) to write to a string; consequently using 
     * write(std::ostream*) is more efficient for most purposes.
     */
    const string& operator*() const {
        std::ostringstream out;
        write(&out);
        return out.str();
    }
};

template <class T>
class TmplPrinterIter : public PrinterIter {
public:
    TmplPrinterIter(std::vector<T>::const_iterator listiter,
                    std::vector<T>::const_iterator enditer,
                    std::vector<T>::const_iterator renditer) 
        : _it(listiter), _end(enditer), _rend(renditer) { }
    virtual ~TmplPrinterIter();
    virtual std::ostream write(std::ostream *strm);
    virtual PrintIter& operator++();
    virtual PrintIter& operator--();
    virtual bool operator==(const PrinterIter& that) const;
    virtual bool operator!=(const PrinterIter& that) const;
    virtual bool notAtEnd() const;
    virtual bool notAtRend() const;
    bool operator==(const TmplPrinterIter& that) const { 
        return (this->_it == that._it); 
    }
    bool operator!=(const TmplPrinterIter& that) const { 
        return (this->_it != that._it); 
    }
protected:
    std::vector<T>::const_iterator _it, _end, _rend;
};

template <class T>
std::ostream TmplPrinterIter::write(std::ostream *strm) const { 
    (*strm) << *_it; 
}
template <class T>
TmplPrinterIter::~TmplPrinterIter() { }
template <class T>
PrinterIter& TmplPrinterIter::operator++() { ++i; return *this; }
template <class T>
PrinterIter& TmplPrinterIter::operator--() { --i; return *this; }
template <class T>
bool TmplPrinterIter::operator==(const PrinterIter& that) const {
    TmplPrinterIter<T> *thit = dynamic_cast<PrinterIter>(&that);
    return (thit == 0) ? false : (*this == *thit);
}
template <class T>
bool TmplPrinterIter::operator!=(const PrinterIter& that) const {
    TmplPrinterIter<T> *thit = dynamic_cast<PrinterIter>(&that);
    return (thit == 0) ? true : (*this != *thit);
}
template <class T>
bool TmplPrinterIter::notAtEnd() const { return (_it != _end); }
template <class T>
bool TmplPrinterIter::notAtRend() const { return (_it != _rend); } 

class DateTimePrinterIter : public PrinterIter<DateTime> {
public:
    DateTimePrinterIter(std::vector<DateTime>::const_iterator listiter,
                        std::vector<DateTime>::const_iterator enditer,
                        std::vector<DateTime>::const_iterator renditer)
        : PrinterIter<DateTime>(listiter, enditer, renditer) { }
    virtual ~DateTimePrinterIter();
    virtual std::ostream write(std::ostream *strm) const;
};

/**
 * @brief  a wrapper PrinterIter class that hides the polymorphic (and 
 * possibly templatized) nature of an underlying PrinterIter subclass.  
 *
 * This is the iterator type available from the PropertyPrinter class.
 */
class WrappedPrinterIter : public PrinterIter {
public:
    WrappedPrinterIter(boost::shared_ptr<PrinterIter> iter) : _it(iter) { }
    virtual ~WrappedPrinterIter();
    virtual std::ostream write(std::ostream *strm);
    virtual PrintIter& operator++();
    virtual PrintIter& operator--();
    virtual bool operator==(const PrinterIter& that);
    virtual bool operator!=(const PrinterIter& that);
    virtual bool notAtEnd();
    virtual bool notAtRend();
private:
    boost::shared_ptr<PrinterIter> _it;
};

/**
 * @brief an abstract class that encapsulates a list of property values
 * to be printed out.
 * 
 * The purpose of this class is wrap a list of property values (all of the 
 * same type) and provide an iterator for printing out those values.  This 
 * class is not used directly by applications but internally via the 
 * PropertyPrinter.  Since PropertyPrinter does not support all types by 
 * default, an application may need to subclass this class in order to 
 * support printing of a new type.  
 *
 * To support a new type in PropertyPrinter (via PrinterFactory) one must 
 * subclass this class and the PrinterIter class (the latter actually 
 * controls how the type gets printed to an output stream).  
 *
 * For types that already support the output operator (<<), an explicit
 * subclass need not be defined.  Instead, one can instantiate the 
 * makePrinter<T> factory function; the required subclasses will be 
 * generated automatically.  
 *
 * The PropertyPrinter.h file also provide support for printing DateTime
 * properties via the explicitly defined DateTimePrinterList and 
 * DateTimePrinterIter because DateTime does not support the output (<<)
 * operator.  See the implementation of these classes for a good example 
 * supporting other types of this sort.  
 */
class PrinterList {
public:
    typedef WrappedPrinterIter iterator;

    /**
     * return a PrinterIter set at the first property value 
     */
    virtual iterator begin() const=0;

    /**
     * return a PrinterIter set at the last property value 
     */
    virtual iterator rbegin() const=0;

    /**
     * return the number values available in the internal property value list
     */
    virtual size_t valueCount() const=0;
};

template <class T>
class TmplPrinterList : public PrinterList {
public:
    TmplPrinterList(const PropertySet& prop, const string& name)
        : _list(prop.getArray<T>(name)) { }

    virtual ~TmplPrinterList();
    virtual iterator begin() const;
    virtual iterator rbegin() const;
    virtual size_t valueCount() const;

private:
    typedef TmplPrinterIter<T> delegateIter;
protected:
    std::vector<T> _list;
};

template <class T>
TmplPrinterList::~TmplPrinterList() { }
template <class T>
TmplPrinterList::iterator TmplPrinterList::begin() const { 
    PrinterIter *it = new delegateIter(_list.begin(), 
                                       _list.end(), _list.rend());
    return iterator(boost::shared_ptr<PrinterIter>(it));
}
template <class T>
TmplPrinterList::iterator TmplPrinterList::rbegin() const { 
    PrinterIter *it = new delegateIter(_list.rbegin(), 
                                       _list.end(), _list.rend());
    return iterator(boost::shared_ptr<PrinterIter>(it));
}
template <class T>
size_t TmplPrinterList::valueCount() const { return _list.size(); }

/**
 * @brief the template factory function for supporting printing of printable 
 * types
 *
 * By instantiating this template, one automatically generates not only this 
 * function by also the PrintList and PrinterIter classes it creates.  This 
 * will work for types that can be printed to a stream via the output (<<) 
 * operator.  
 */
template <class T> 
PrinterList* makePrinter(const PropertySet& prop, const string& name) {
    return new TmplPrinterList<T>(prop, name);
}

class DateTimePrinterList : public PrinterList<DateTime> {
public:
    DateTimePrinterList(const PropertySet& prop, const string& name) 
        : PrinterList<DateTime>(prop, name) { }

    virtual ~DateTimePrinterList();
    virtual iterator begin() const;
    virtual iterator rbegin() const;
};

/**
 * @brief a factory used to create PrintList instances to be used by 
 * a PropertyPrinter instance.   
 *
 * PrintList classes are used to print out property values of a particular 
 * type.  Typically, a default PrinterFactory is filled with support for 
 * the common primitive types and type classes that are important for 
 * logging.  Other types can be supported by providing an implementation
 * of PrintList and corresponding iterator that support the type as well 
 * as an implementation of a factory function that creates a pointer to 
 * the specialized PrintList.  A pointer to the factory function is what 
 * one registers with this general PrinterFactory.  
 * 
 * PrinterFactory::factoryFuncPtr defines the form of the factory function:
 * @code
 *   PrinterList* (*makePrintList)(const PropertySet&, const string&)
 * @endcode
 * 
 * See PrintList for more details about supporting a new type.
 */
class PrinterFactory {
public: 
    typedef PrinterList* (*)(const PropertySet&, const string&) factoryFuncPtr;
        
    PrinterFactory(bool loadDefaults=false) : _factFuncs() { 
        if (loadDefaults) _loadDefaults();
    }

    void add(const std::type_info& tp, factoryFuncPtr func) { 
        _factFuncs[tp] = func; 
    }

    PrinterList* makePrinter(const PropertySet& prop, const string& name) {
        Lookup::const_iterator fi = _factFuncs(prop.typeOf(name));
        return (fi == _factFuncs.end()) ? 0 : (*fi)(prop, name);
    }

private:
    void _loadDefaults();

    typedef std::map<std::type_info, factorFuncPtr, TypeInfoLess> Lookup;
    Lookup _factFuncs;
};

/**
 * @brief  an class for printing the values associated with a name in a 
 * PropertySet.
 * 
 * This class is vital if one needs to print out all the values in a 
 * PropertySet without knowing the data types associated with the names.  
 * In particular, it gives one the ability to write each value associated 
 * with a name individually and it any order.  
 * A typical use looks like this:
 * @code
 *   PropertySet ps;
 *   std::vector<std::string>::iterator ni;
 *   std::vector<std::string> names(ps.paramNames(true));
 * 
 *   for(ni = names.begin(); ni != names.end(); ++ni) {
 *     PropertyPrinter pp(ps, *ni);
 * 
 *     for(PropertyPrinter::iterator i=pp.begin(); i.notAtEnd(); ++i) {
 *       strm << *ni << ": ";
 *       i.write(&strm) << endl;
 *     }
 *   }
 * @endcode
 *
 * A PropertyPrinter is constructed for a particular name found in the 
 * PropertySet.  (If a property with that name doesn't exist, a
 * pex::exceptions::NotFoundException is thrown.)  The PropertyPrinter is 
 * wrapped around the array of values associated with the name.  One can 
 * get an Iterator for the values and send each one to a stream.
 *
 * A PropertyPrinter can only print out property types that it has been 
 * configured for via the PrinterFactory instance that is passed in during 
 * construction.  If one is not provided, a default is set which supports 
 * the following types:
 * @verbatim
 *   short          char              float
 *   int            signed char       double
 *   long           unsigned char     bool
 *   long long      string            DateTime
 * @endverbatim
 * Other types can be supported by passing the name of factory function 
 * that can create a printer instance to a PrinterFactory that will eventually 
 * be used with a PropertyPrinter (see PrinterFactory for details.  
 */
class PropertyPrinter {
public:

    /**
     * the iterator type used for iterating through values.
     */
    typedef PrinterList::iterator iterator;

    /**
     * the default look-up table for creating iterators for different types.
     * Users are allowsd add new printers.  See PropertyFactory for more 
     * details.
     */
    static PrinterFactory defaultPrinterFactory;

    /**
     * create the PropertyPrinter for a given name
     */
    PropertyPrinter(const PropertySet& prop, const string& name, 
                    const PrinterFactory& fact=defaultPrinterFactory);

    /**
     * return an iterator set at the start of the list of values
     */
    iterator begin();

    /**
     * return an iterator set at the last of the list of values
     */
    iterator rbegin();

    /**
     * return the number of values associated with the property
     */
    size_t valueCount() { _list.get()->valueCount(); }


private:
    boost::share_ptr<PrinterList> _list;
};

}}}     // end lsst::pex::logging

#endif  // end LSST_PEX_PROPERTYPRINTER_H

