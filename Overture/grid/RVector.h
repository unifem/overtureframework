#ifndef RVector_
#define RVector_
//
//******************************************************************************
//
// This file defines a reference-counted vector of <T> objects.
//
//     class RVector<T>
//
// This class may be used instead of the STL class vector<T>.
//
// Differences of class RVector<T> from the class vector<T>:
//     The vector itself is reference-counted, though its elements need not be.
//     The <T> objects may be very large, so this class contains a vector of
//     pointers to these objects, allocating only those objects which are used.
//     The copy constructor has an extra optional argument of type CopyType.
//     Additional member function push_back() uses the default <T> constructor.
//     Additional member function find(const T&), not in STL class vector<T>.
//     Additional member functions from the base class ReferenceCounting.
//     Additional obsolete member functions for compatibility with the Overture
//     ListOfXXX classes.  These member functions are expected to disappear.
//
//******************************************************************************
//
#include "ReferenceCounting.h"
#include "PVector.h"
//
// Letter class for reference-counted STL vector of <T> objects.
//
template<class T>
class RVectorData:
  public ReferenceCounting {
  public:
//
//  Type definitions.
//
    typedef vector<T*>::size_type             size_type;
    typedef PVector_iterator<T>               iterator;
    typedef PVector_const_iterator<T>         const_iterator;
    typedef PVector_reverse_iterator<T>       reverse_iterator;
    typedef PVector_const_reverse_iterator<T> const_reverse_iterator;
//
//  STL vector member functions.
//
    RVectorData()                             { className = "RVectorData";     }
    RVectorData(size_type n_)
        { className = "RVectorData"; for (Integer i=0; i<n_; i++) push_back(); } 
    RVectorData(size_type n_, const T& value_)
                     { className = "RVectorData"; insert(begin(), n_, value_); }
    RVectorData(const T* first_, const T* last_)
                  { className = "RVectorData"; insert(begin(), first_, last_); }
    RVectorData(const RVectorData& vector_, const CopyType ct = DEEP)
               { className = "RVectorData"; if (ct != NOCOPY) *this = vector_; }
    virtual ~RVectorData() {
       for (vector<T*>::iterator it=vector_T.begin(); it!=vector_T.end(); it++)
         delete *it;
    }
    RVectorData& operator=(const RVectorData& vector_) {
        ReferenceCounting::operator=(vector_);
        if (size() > vector_.size()) erase(begin() + vector_.size(), end());
        int i;
        for (i=0; i<size(); i++) (*this)[i] = vector_[i];
        for (; i<vector_.size(); i++) { push_back(); back() = vector_[i]; }
        return *this;
    }
          T& operator[](int index_)          { return *vector_T[index_];       }
    const T& operator[](int index_) const    { return *vector_T[index_];       }
          T& back()                          { return *vector_T.back();        }
    const T& back() const                    { return *vector_T.back();        }
          iterator begin()                   { return vector_T.begin();        }
    const_iterator begin() const             { return vector_T.begin();        }
    size_type capacity() const               { return vector_T.capacity();     }
    bool empty() const                       { return vector_T.empty();        }
          iterator end()                     { return vector_T.end();          }
    const_iterator end() const               { return vector_T.end();          }
    void erase(iterator pos_) {
        delete *(vector<T*>::iterator)pos_;
        vector_T.erase(pos_);
    }
    void erase(iterator first_, iterator last_) {
        for (vector<T*>::iterator it=(vector<T*>::iterator)first_;
          it!=(vector<T*>::iterator)last_; it++) delete *it;
        vector_T.erase(first_, last_);
    }
          T& front()                         { return *vector_T.front();       }
    const T& front() const                   { return *vector_T.front();       }
    iterator insert(iterator pos_, const T& value_)
                                { return vector_T.insert(pos_, new T(value_)); }
    void insert(iterator pos_, size_type n_, const T& value_) {
        int pos_i = pos_ - begin();
        vector_T.insert((vector<T*>::iterator)pos_, n_, (T*)0);
        vector<T*>::iterator it = begin() + pos_i;
        for (int i=0; i<n_; i++, it++) *it = new T(value_);
    }
    void insert(iterator pos_, const T* first_, const T* last_) {
        int pos_i = pos_ - begin();
        vector_T.insert((vector<T*>::iterator)pos_, last_ - first_, (T*)0);
        vector<T*>::iterator it = begin() + pos_i;
        for (const T* t=first_; t!=last_; t++, it++) *it = new T(*t);
    }
    size_type max_size() const            { return vector_T.max_size();        }
    void push_back(const T& value_)       { vector_T.push_back(new T(value_)); }
    void pop_back()          { delete vector_T.back(); vector_T.pop_back(); }
          reverse_iterator rbegin()       { return vector_T.rbegin();          }
    const_reverse_iterator rbegin() const { return vector_T.rbegin();          }
          reverse_iterator rend()         { return vector_T.rend();            }
    const_reverse_iterator rend() const   { return vector_T.rend();            }
    void reserve(size_type n_)            { vector_T.reserve(n_);              }
    size_type size() const                { return vector_T.size();            }
    void swap(RVectorData<T>& vector_)    { vector_T.swap(vector_.vector_T);   }
//
//  Additional STL member functions not normally associated with vector<T>.
//
    void push_back()                      { vector_T.push_back(new T);         }
    iterator find(const T& value_) const { 
        for (const_iterator it=begin(); it!=end() && *it!=value_; it++);
        return it;
    }
//
// Private member data.
//
    vector<T*>        vector_T;
    static vector<T*> freeVector;
//
//  Virtual member functions used only through class ReferenceCounting:
//
  private:
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((RVectorData<T>&)x); }
    inline virtual void reference(const ReferenceCounting& x)
      { reference((RVectorData<T>&)x); }
    inline virtual ReferenceCounting* virtualConstructor(
      const CopyType ct = DEEP) const
      { return new RVectorData<T>(*this, ct); }
    aString className;
  public:
    inline virtual aString getClassName() const { return className; }
};
template<class T>
inline bool operator==(const RVectorData<T>& x, const RVectorData<T>& y)
  { return x.size() == y.size() && equal(x.begin(), x.end(), y.begin()); }
template<class T>
inline bool operator<(const RVectorData<T>& x, const RVectorData<T>& y)
  { return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }
//
// Envelope class for reference-counted STL vector of <T> objects.
//
template<class T>
class RVector:
  public ReferenceCounting {
  public:
//
//  Typedefs.
//
    typedef RVectorData<T>                         RCData;
    typedef RVectorData<T>::size_type              size_type;
    typedef RVectorData<T>::iterator               iterator;
    typedef RVectorData<T>::const_iterator         const_iterator;
    typedef RVectorData<T>::reverse_iterator       reverse_iterator;
    typedef RVectorData<T>::const_reverse_iterator const_reverse_iterator;
//
//  ReferenceCounting member functions.
//
//  Make a reference.  (Does a shallow copy.)
//
    void reference(const RVector<T>& x) {
        ReferenceCounting::reference(x);
        if (rcData != x.rcData) {
            if (isCounted && rcData->decrementReferenceCount() == 0)
              delete rcData;
            rcData = x.rcData;
            isCounted = x.isCounted;
            if (isCounted) rcData->incrementReferenceCount();
        } // end if
    }
//
//  Break a reference.  (Replaces with a deep copy.)
//
    void breakReference() {
//      ReferenceCounting::breakReference();
        if (!isCounted || rcData->getReferenceCount() != 1) {
            RVector x = *this; // Uses the (deep) copy constructor.
            reference(x);
        } // end if
    }
//
//  STL vector member functions.
//
    RVector():
      ReferenceCounting() {
        className = "RVector";
        rcData    = new RVectorData<T>;
        isCounted = TRUE;
        rcData->incrementReferenceCount();
    }
    RVector(size_type n_):
      ReferenceCounting() {
        className = "RVector";
        rcData    = new RVectorData<T>(n_);
        isCounted = TRUE;
        rcData->incrementReferenceCount();
    }
    RVector(
      size_type n_,
      const T&  value_):
      ReferenceCounting() {
        className = "RVector";
        rcData    = new RVectorData<T>(n_, value_);
        isCounted = TRUE;
        rcData->incrementReferenceCount();
    }
    RVector(
      const T* first_,
      const T* last_):
      ReferenceCounting() {
        className = "RVector";
        rcData    = new RVectorData<T>(first_, last_);
        isCounted = TRUE;
        rcData->incrementReferenceCount();
    }
    RVector(
      const RVector<T>& value_,
      const CopyType     ct = DEEP):
      ReferenceCounting(value_, ct) {
        className = "RVector";
        switch (ct) {
          case DEEP:
          case NOCOPY:
            rcData = (RVectorData<T>*)
              ((ReferenceCounting*)value_.rcData)->virtualConstructor(ct);
            isCounted = LogicalTrue;
            rcData->incrementReferenceCount();
          break;
          case SHALLOW:
            rcData = value_.rcData;
            isCounted = value_.isCounted;
            if (isCounted) rcData->incrementReferenceCount();
          break;
        } // end switch
    }
    virtual ~RVector()
       { if (isCounted && rcData->decrementReferenceCount()==0) delete rcData; }
    RVector<T>& operator=(const RVector<T>& x)
                                          { *rcData = *x.rcData; return *this; }
    bool operator==(const RVector<T>& vector_) const {
        return *(const RVectorData<T>*)rcData ==
               *(const RVectorData<T>*)vector_.rcData;
    }
    bool operator<(const RVector<T>& vector_) const {
        return *(const RVectorData<T>*)rcData <
               *(const RVectorData<T>*)vector_.rcData;
    }
          T& operator[](int index_)       { return (*rcData)[index_];          }
    const T& operator[](int index_) const
                            { return (*(const RVectorData<T>*)rcData)[index_]; }
          T& back()                       { return rcData->back();             }
    const T& back() const    { return ((const RVectorData<T>*)rcData)->back(); }
          iterator begin()                { return rcData->begin();            }
    const_iterator begin() const
                            { return ((const RVectorData<T>*)rcData)->begin(); }
    size_type capacity() const
                         { return ((const RVectorData<T>*)rcData)->capacity(); }
    bool empty() const      { return ((const RVectorData<T>*)rcData)->empty(); }
          iterator end()                  { return rcData->end();              }
    const_iterator end() const
                              { return ((const RVectorData<T>*)rcData)->end(); }
    void erase(iterator pos_)             { rcData->erase(pos_);               }
    void erase(iterator first_, iterator last_)
                                          { rcData->erase(first_, last_);      }
          T& front()                      { return rcData->front();            }
    const T& front() const  { return ((const RVectorData<T>*)rcData)->front(); }
    iterator insert(iterator pos_, const T& value_)
                                        { return rcData->insert(pos_, value_); }
    void insert(iterator pos_, size_type n_, const T& value_)
                                          { rcData->insert(pos_, n_, value_);  }
    void insert(iterator pos_, const T* first_, const T* last_)
                                        { rcData->insert(pos_, first_, last_); }
    size_type max_size() const
                         { return ((const RVectorData<T>*)rcData)->max_size(); }
    void push_back(const T& value_)       { rcData->push_back(value_);         }
    void pop_back()                       { rcData->pop_back();                }
          reverse_iterator rbegin()       { return rcData->rbegin();           }
    const_reverse_iterator rbegin() const
                           { return ((const RVectorData<T>*)rcData)->rbegin(); }
          reverse_iterator rend()         { return rcData->rend();             }
    const_reverse_iterator rend() const
                             { return ((const RVectorData<T>*)rcData)->rend(); }
    void reserve(size_type n_)            { rcData->reserve(n_);               }
    size_type size() const   { return ((const RVectorData<T>*)rcData)->size(); }
    void swap(RVector<T>& vector_)        { rcData->swap(*vector_.rcData);     }
//
//  Additional STL member functions not normally associated with vector<T>.
//
    void push_back()                      { rcData->push_back();               }
    iterator find(const T& value_) const  { return rcData->find(value_);       }
//
//****************************************************************************//
//                                                                            //
//  Obsolete interface for compatibility with Overture "List" classes.        //
//                                                                            //
    void addElement()                            { push_back();             } //
    void addElement(const T& value_)             { push_back(value_);       } //
//  ***** This conflicts with the previous member function. *****             //
//  void addElement(const int index_)            {addElement(index_, T());  } //
    void addElement(const T& value_, const int& index_)                       //
                                        { insert(begin() + index_, value_); } //
    size_type getLength()  const                 { return size();           } //
    size_type listLength() const                 { return size();           } //
    void deleteElement(const T& value_) {                                     //
        for (iterator it=begin(); it!=end(); it++)                            //
          if (*it == value_) { erase(it); break; }                            //
    }                                                                         //
//  ***** This conflicts with the previous member function. *****             //
//  void deleteElement(const int index_)         { erase(begin() + index_); } //
    void deleteElement()                         { erase(end() - 1);        } //
//  ***** This should work differently depending on whether                   //
//        or not class <T> is derived from ReferenceCounting.                 //
    void swapElements(const int i, const int j)  {                            //
        const vector<T*>::iterator it_i = begin() + i, it_j = begin() + j;    //
        T* t = *it_i; *it_i = *it_j; *it_j = t;                               //
    }                                                                         //
    int getIndex(const T& x) const                                            //
           { iterator it = find(x); return it == end() ? -1 : it - begin(); } //
//                                                                            //
//****************************************************************************//
//
//  Private member data.
//
  private:
    RVectorData<T> *rcData; bool isCounted;
//
//  Virtual member functions used only through class ReferenceCounting:
//
  private:
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((RVector<T>&)x); }
    inline virtual void reference(const ReferenceCounting& x)
      { reference((RVector<T>&)x); }
    inline virtual ReferenceCounting* virtualConstructor(
      const CopyType ct = DEEP) const
      { return new RVector<T>(*this, ct); }
    aString className;
  public:
    inline virtual aString getClassName() const { return className; }
};

#endif // RVector_
