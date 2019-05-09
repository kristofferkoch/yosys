/*******************************************************************************************[Vec.h]
Copyright (c) 2003-2007, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

#ifndef Minisat_Vec_h
#define Minisat_Vec_h

#include <cassert>
#include <limits>
#include <new>
#include <vector>

#include "IntTypes.h"
#include "XAlloc.h"

namespace Minisat {

//=================================================================================================
// Automatically resizable arrays
//
// NOTE! Don't use this vector on datatypes that cannot be re-located in memory (with realloc)

template<class T, class _Size = int>
class vec {
public:
    typedef _Size Size;
private:
    std::vector<T> backing;
public:
    // Constructors:
    vec()                        : backing() { }
    explicit vec(Size size)      : backing() { growTo(size); }
    vec(Size size, const T& pad) : backing() { growTo(size, pad); }

    // Pointer to first element:
    operator T*       (void)           { return backing.data(); }

    // Size operations:
    Size     size     (void) const   { return static_cast<Size>(backing.size()); }
    void     shrink   (Size nelems)  { assert(nelems <= size()); backing.resize(nelems); }
    Size     capacity (void) const   { return static_cast<Size>(backing.capacity()); }
    void     capacity (Size min_cap) { if (min_cap > capacity()) { backing.reserve(min_cap); } }
    void     growTo   (Size size);
    void     growTo   (Size size, const T& pad);
    void     clear    ()             { backing.clear(); };
    void     clear    (bool)         { clear(); };

    // Stack interface:
    void     push  (void)              { backing.emplace_back(); }
    //void     push  (const T& elem)     { if (sz == cap) capacity(sz+1); data[sz++] = elem; }
    void     push  (const T& elem)     { backing.push_back(elem); }
    void     pop   (void)              { backing.pop_back(); }
    // NOTE: it seems possible that overflow can happen in the 'sz+1' expression of 'push()', but
    // in fact it can not since it requires that 'cap' is equal to INT_MAX. This in turn can not
    // happen given the way capacities are calculated (below). Essentially, all capacities are
    // even, but INT_MAX is odd.

    const T& last  (void) const        { return backing[backing.size()-1]; }
    T&       last  (void)              { return backing[backing.size()-1]; }

    // Vector interface:
    const T& operator [] (Size index) const { return backing[index]; }
    T&       operator [] (Size index)       { return backing[index]; }

    // Duplicatation (preferred instead):
    void copyTo(vec<T>& copy) const { copy.backing = backing; }
    void moveTo(vec<T>& dest) { dest.backing = std::move(backing); }
};


template<class T, class _Size>
void vec<T,_Size>::growTo(Size size, const T& pad) {
    if (this->size() >= size) return;
    backing.resize(size, pad);
}


template<class T, class _Size>
void vec<T,_Size>::growTo(Size size) {
    if (this->size() >= size) return;
    backing.resize(size);
}

//=================================================================================================
}

#endif
