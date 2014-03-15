#ifndef PVector_
#define PVector_
//
//*******************************************************************************
//
// This file defines an STL vector of <T> objects:
//
//     class PVector<T>
//
// This class may be used instead of the STL class vector<T>.
//
// Differences of class PVector<T> from the STL class vector<T>:
//     The <T> objects may be very large, so this class contains a vector of
//     pointers to these objects, allocating only those objects which are used.
//     Additional member function push_back() that uses the default constructor.
//     Additional member function find(const T&), not in STL class vector<T>.
//
//*******************************************************************************
//
#include "vector.h"
//
// Define iterators such that *it and it[i] de-reference one extra time.
//
template<class T>
class PVector_iterator {
  public:
    PVector_iterator()                            {                            }
    PVector_iterator(const PVector_iterator& it_) { *this = it_;               }
    PVector_iterator(const vector<T*>::iterator it_) { it = it_;               }
    ~PVector_iterator()                           {                            }
    PVector_iterator& operator=(const PVector_iterator& it_)
                                                  { it = it_.it; return *this; }
    PVector_iterator& operator=(const vector<T*>::iterator it_)
                                                  { it = it_; return *this;    }
    operator       vector<T*>::iterator()         { return it;                 }
    operator const vector<T*>::iterator() const   { return it;                 }
    PVector_iterator operator++()                 { return ++it;               }
    PVector_iterator operator++(int)              { return it++;               }
    PVector_iterator operator--()                 { return --it;               }
    PVector_iterator operator--(int)              { return it--;               }
    bool operator==(const PVector_iterator& it_) const { return it == it_.it;  }
    bool operator!=(const PVector_iterator& it_) const { return it != it_.it;  }
    PVector_iterator operator+(int i) const       { return it + i;             }
    PVector_iterator operator-(int i) const       { return it - i;             }
    PVector_iterator operator+=(const int i)      { return it += i;            }
    PVector_iterator operator-=(const int i)      { return it -= i;            }
    T& operator*()                                { return **it;               }
    const T& operator*() const                    { return **it;               }
    T& operator[](int i)                          { return *it[i];             }
    const T& operator[](int i) const              { return *it[i];             }
  private:
    vector<T*>::iterator it;
};
template<class T>
class PVector_const_iterator {
  public:
    PVector_const_iterator()                      {                            }
    PVector_const_iterator(const PVector_const_iterator& it_)
                                                  { *this = it_;               }
    PVector_const_iterator(const vector<T*>::const_iterator it_)
                                                  { it = it_;                  }
    ~PVector_const_iterator()                     {                            }
    PVector_const_iterator& operator=(
      const PVector_const_iterator& it_)          { it = it_.it; return *this; }
    PVector_const_iterator& operator=(const vector<T*>::const_iterator it_)
                                                  { it = it_; return *this;    }
    operator const vector<T*>::const_iterator()       { return it;             }
    operator       vector<T*>::const_iterator() const { return it;             }
    PVector_const_iterator operator++()           { return ++it;               }
    PVector_const_iterator operator++(int)        { return it++;               }
    PVector_const_iterator operator--()           { return --it;               }
    PVector_const_iterator operator--(int)        { return it--;               }
    bool operator==(const PVector_const_iterator& it_) const
                                                  { return it == it_.it;       }
    bool operator!=(const PVector_const_iterator& it_) const
                                                  { return it != it_.it;       }
    PVector_const_iterator operator+(int i) const { return it + i;             }
    PVector_const_iterator operator-(int i) const { return it - i;             }
    PVector_const_iterator operator+=(const int i) { return it += i;           }
    PVector_const_iterator operator-=(const int i) { return it -= i;           }
    const T& operator*() const                     { return **it;              }
    const T& operator[](int i) const               { return *it[i];            }
  private:
    vector<T*>::const_iterator it;
};
template<class T>
class PVector_reverse_iterator {
  public:
    PVector_reverse_iterator()                    {                            }
    PVector_reverse_iterator(const PVector_reverse_iterator& it_)
                                                  { *this = it_;               }
    PVector_reverse_iterator(const vector<T*>::reverse_iterator it_)
                                                  { it = it_;                  }
    ~PVector_reverse_iterator()                   {                            }
    PVector_reverse_iterator& operator=(
      const PVector_reverse_iterator& it_)        { it = it_.it; return *this; }
    PVector_reverse_iterator& operator=(
      const vector<T*>::reverse_iterator it_)     { it = it_; return *this;    }
    operator       vector<T*>::reverse_iterator()       { return it;           }
    operator const vector<T*>::reverse_iterator() const { return it;           }
    PVector_reverse_iterator operator++()         { return ++it;               }
    PVector_reverse_iterator operator++(int)      { return it++;               }
    PVector_reverse_iterator operator--()         { return --it;               }
    PVector_reverse_iterator operator--(int)      { return it--;               }
    bool operator==(const PVector_reverse_iterator& it_) const
                                                  { return it == it_.it;       }
    bool operator!=(const PVector_reverse_iterator& it_) const
                                                  { return it != it_.it;       }
    PVector_reverse_iterator operator+(int i) const  { return it + i;          }
    PVector_reverse_iterator operator-(int i) const  { return it - i;          }
    PVector_reverse_iterator operator+=(const int i) { return it += i;         }
    PVector_reverse_iterator operator-=(const int i) { return it -= i;         }
    T& operator*()                                   { return **it;            }
    const T& operator*() const                       { return **it;            }
    T& operator[](int i)                             { return *it[i];          }
    const T& operator[](int i) const                 { return *it[i];          }
  private:
    vector<T*>::reverse_iterator it;
};
template<class T>
class PVector_const_reverse_iterator {
  public:
    PVector_const_reverse_iterator()              {                            }
    PVector_const_reverse_iterator(
      const PVector_const_reverse_iterator& it_)  { *this = it_;               }
    PVector_const_reverse_iterator(
      const vector<T*>::const_reverse_iterator it_) { it = it_;                }
    ~PVector_const_reverse_iterator()             {                            }
    PVector_const_reverse_iterator& operator=(
      const PVector_const_reverse_iterator& it_)  { it = it_.it; return *this; }
    PVector_const_reverse_iterator& operator=(
      const vector<T*>::const_reverse_iterator it_) { it = it_; return *this;  }
    operator       vector<T*>::const_reverse_iterator()       { return it;     }
    operator const vector<T*>::const_reverse_iterator() const { return it;     }
    PVector_const_reverse_iterator operator++()            { return ++it;      }
    PVector_const_reverse_iterator operator++(int)         { return it++;      }
    PVector_const_reverse_iterator operator--()            { return --it;      }
    PVector_const_reverse_iterator operator--(int)         { return it--;      }
    bool operator==(const PVector_const_reverse_iterator& it_) const
                                                        { return it == it_.it; }
    bool operator!=(const PVector_const_reverse_iterator& it_) const
                                                        { return it != it_.it; }
    PVector_const_reverse_iterator operator+(int i)const   { return it + i;    }
    PVector_const_reverse_iterator operator-(int i)const   { return it - i;    }
    PVector_const_reverse_iterator operator+=(const int i) { return it += i;   }
    PVector_const_reverse_iterator operator-=(const int i) { return it -= i;   }
    const T& operator*() const                             { return **it;      }
    const T& operator[](int i) const                       { return *it[i];    }
  private:
    vector<T*>::const_reverse_iterator it;
};
//
// Class for STL vector of <T> objects.
//
template<class T>
class PVector {
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
    PVector()                                {                                 }
    PVector(size_type n_)                    { insert(begin(), n_, T());       }
    PVector(size_type n_, const T& value_)   { insert(begin(), n_, value_);    }
    PVector(const T* first_, const T* last_) { insert(begin(), first_, last_); }
    PVector(const PVector& vector_)          { *this = vector_;                }
    virtual ~PVector() {
       for (vector<T*>::iterator it=vector_T.begin(); it!=vector_T.end(); it++)
         delete *it;
    }
    PVector& operator=(const PVector& vector_) {
        if (size() > vector_.size()) erase(begin() + vector_.size(), end());
        for (int i=0;  i<size(); i++) (*this)[i] = vector_[i];
        for (; i<vector_.size(); i++) vector_T.push_back(new T(vector_[i]));
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
    void erase(iterator pos_)
                   { delete *(vector<T*>::iterator)pos_; vector_T.erase(pos_); }
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
    void push_back()                      { vector_T.push_back(new T);         }
    void push_back(const T& value_)       { vector_T.push_back(new T(value_)); }
    void pop_back()             { delete vector_T.back(); vector_T.pop_back(); }
          reverse_iterator rbegin()       { return vector_T.rbegin();          }
    const_reverse_iterator rbegin() const { return vector_T.rbegin();          }
          reverse_iterator rend()         { return vector_T.rend();            }
    const_reverse_iterator rend() const   { return vector_T.rend();            }
    void reserve(size_type n_)            { vector_T.reserve(n_);              }
    size_type size() const                { return vector_T.size();            }
    void swap(PVector<T>& vector_)        { vector_T.swap(vector_.vector_T);   }
//
//  Additional STL member functions not normally associated with vector<T>.
//
    iterator find(const T& value_) const { 
        for (const_iterator it=begin(); it!=end() && *it!=value_; it++);
        return it;
    }
//
// Private member data.
//
  private:
    vector<T*> vector_T;
};
template <class T>
inline bool operator==(const PVector<T>& x, const PVector<T>& y)
  { return x.size() == y.size() && equal(x.begin(), x.end(), y.begin()); }
template <class T>
inline bool operator<(const PVector<T>& x, const PVector<T>& y)
  { return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

#endif // PVector_
