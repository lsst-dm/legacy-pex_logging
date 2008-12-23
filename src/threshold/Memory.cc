//////////////////////////////////////////////////////////////////////////////
// ThresholdMemory.cc
//
// Contact: Ray Plante
// 
//////////////////////////////////////////////////////////////////////////////

#include "lsst/pex/logging/threshold/Memory.h"
#include <boost/tokenizer.hpp>
#include <ostream>

using namespace std;

namespace lsst {
namespace pex {
namespace logging {
namespace threshold {

/*
 * create a hierarchical container for threshold data
 */
Family::Family(int defaultThreshold) 
    : _thresh(defaultThreshold), _children() 
{ }

Family* Family::ensureDescendant(tokenizer::iterator toptoken, 
                                 const tokenizer::iterator& end) 
{
    if (_children == 0) 
        // we have no children yet, so just make the descendents
        return makeDescendants(toptoken, end);

    // look for a child matching the top field in the name
    ChildMap::iterator out = _children->find(*toptoken);

    // if name is not found create all descendants
    if (out == _children->end()) 
        return makeDescendants(toptoken, end);

    // if this was the last token return the found child
    if (++toptoken == end)
        return out->second;

    // look for the the remainder of the name hierarchy
    return out->second->ensureDescendant(toptoken, end);
}

const Family* Family::findDescendant(tokenizer::iterator toptoken,
                                     const tokenizer::iterator& end,
                                     bool orNearest) const
{
    if (_children == 0) 
        return ((orNearest && _thresh != INHERIT) ? this : 0);

    // look for a child matching the top field in the name
    ChildMap::const_iterator found = _children->find(*toptoken);

    // if name is not found, return null
    if (found == _children->end()) 
        return ((orNearest && _thresh != INHERIT) ? this : 0);

    const Family *out = found->second;
    if (++toptoken != end) out = out->findDescendant(toptoken, end, orNearest);
    if (out != 0 && out->_thresh == INHERIT) out = 0;

    if (out == 0 && orNearest && _thresh != INHERIT) out = this;
    return out;
}

Family* Family::makeDescendants(tokenizer::iterator toptoken,
                                const tokenizer::iterator& end) 
{
    if (_children == 0) _children = new ChildMap();

    Family *next = new Family();
    _children->insert(ChildMap::value_type(*toptoken, next));

    if (++toptoken == end) return next;

    return next->makeDescendants(toptoken, end);
}

/**
 * set the threshold associated with a descendent with a given name
 */
void Family::setThresholdFor(tokenizer::iterator toptoken, 
                             const tokenizer::iterator& end,
                             int threshold) 
{
    Family *nfd = ensureDescendant(toptoken, end);
    nfd->_thresh = threshold;
}

/**
 * reset the threshold associated with a descendent with a given name
 * to inherit from its parent.
 */
void Family::resetThresholdFor(tokenizer::iterator toptoken, 
                               const tokenizer::iterator& end) 
{
    Family *nfd = const_cast<Family*>(findDescendant(toptoken, end));
    if (nfd != 0) nfd->_thresh = INHERIT;
}

void Family::deleteDescendants() {

    if (_children == 0) return;

    for(ChildMap::iterator it = _children->begin(); 
        it != _children->end(); 
        ++it)
    {
        if (it->second != 0) delete it->second;
    }

    delete _children;
    _children = 0;
}

Family::~Family() {
    deleteDescendants();
}

void Family::printDescThresholds(std::ostream& out, 
                                 const std::string& prefix) const 
{
    if (_children == 0) return;

    int i;
    ChildMap::const_iterator it;
    for(it = _children->begin(); it != _children->end(); ++it) {
        out << prefix << it->first;
        if (it->second != 0 && it->second->_thresh != INHERIT) {
            for(i = prefix.length()+it->first.length(); i < 20; ++i)
                out << ' ';
            if (it->second->_thresh >= 0 && it->second->_thresh < 10) 
                out << ' ';
            out << it->second->_thresh;
        }
        out << endl;

        if (it->second != 0) 
            it->second->printDescThresholds(out, prefix+' ');
    }
}

/* ******************************************************************* */

Memory::Memory(const std::string& delims) 
    : _tree(), _sep(delims.c_str())
{ }

/**
 * print the thresholds stored in this Memory that are not set to INHERIT.
 */
void Memory::printThresholds(std::ostream& out) {
    out << "(root)              ";
    int top = _tree.getThreshold();
    if (top < 10 && top >= 0) out << ' ';
    out << top << endl;

    _tree.printDescThresholds(out, " ");
}



}}}}  // end lsst::pex::logging::threshold
