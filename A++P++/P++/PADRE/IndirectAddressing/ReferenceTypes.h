#ifndef _ReferenceTypes
#define _ReferenceTypes

//
// Who to blame:  Geoff Chesshire
//
// Usage:  ReferenceType(TypeR,Type)
//
// This declares class <TypeR> which acts like a reference to <Type>,
// except that we can change the reference at any time to point to
// a different <Type> object.
//
// Warning:  If the default constructor was used or breakReference()
//   is called, then reference() must be called before anything else.
//
// Warning:  Try to avoid using the copy constructor, as it does not
//   conform to the rule that the copy constructor should do a deep copy.
//
// Public member functions:
//   Default constructor       Sets the pointer to NULL.
//   Copy constructor          Does a shallow copy.  This is non-conforming!
//   Destructor                (virtual)
//   Conversion operator       Converts const <TypeR>& to const <Type>&.
//   Conversion operator       Converts <TypeR>& to <Type>&.
//   Address    operator       Returns <Type>* address.
//   Assignment operator       Takes a const <Type>&.
//   Assignment operator       Takes a const <TypeR>&.  Does a deep copy.
//   Reference function        Takes a const <Type>&.   Sets the pointer.
//   Reference function        Takes a const <TypeR>&.  Does a shallow copy.
//   breakReference function   Sets the pointer to NULL.
//   consistency check         Checks the consistency of the data structure.
//
// For native types, all the arithmetic operators and
// ostream&operator<<(ostream&,const<TypeR>&) are defined also.
//

#include "ReferenceCounting.h"

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceType(TypeR,Type,Foobar)                                       \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&() const                 { return *data;             }  \
  inline operator Type () const                 { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator+  (const Type& i) const  { return *data +   i;       }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Type operator-  (const Type& i) const  { return *data -   i;       }  \
  inline Type operator*  (const Type& i) const  { return *data *   i;       }  \
  inline Type operator/  (const Type& i) const  { return *data /   i;       }  \
  inline Type operator%  (const Type& i) const  { return *data %   i;       }  \
  inline Type operator^  (const Type& i) const  { return *data ^   i;       }  \
  inline Type operator&  (const Type& i) const  { return *data &   i;       }  \
  inline Type operator|  (const Type& i) const  { return *data |   i;       }  \
  inline Type operator~  (             ) const  { return ~ *data;           }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator<  (const Type& i) const  { return *data <   i;       }  \
  inline Type operator>  (const Type& i) const  { return *data >   i;       }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Type operator%= (const Type& i)        { return *data %=  i;       }  \
  inline Type operator^= (const Type& i)        { return *data ^=  i;       }  \
  inline Type operator&= (const Type& i)        { return *data &=  i;       }  \
  inline Type operator|= (const Type& i)        { return *data |=  i;       }  \
  inline Type operator<< (const Type& i) const  { return *data <<  i;       }  \
  inline Type operator>> (const Type& i) const  { return *data >>  i;       }  \
  inline Type operator<<=(const Type& i)        { return *data <<= i;       }  \
  inline Type operator>>=(const Type& i)        { return *data >>= i;       }  \
  inline Logical operator==(const Type&i) const { return *data ==  i;       }  \
  inline Logical operator!=(const Type&i) const { return *data !=  i;       }  \
  inline Logical operator<=(const Type&i) const { return *data <=  i;       }  \
  inline Logical operator>=(const Type&i) const { return *data >=  i;       }  \
  inline Logical operator&&(const Type&i) const { return *data &&  i;       }  \
  inline Logical operator||(const Type&i) const { return *data ||  i;       }  \
  inline Logical operator&&(const Foobar& i) const                             \
                                                { return *data &&  i;       }  \
  inline Logical operator||(const Foobar& i) const                             \
                                                { return *data ||  i;       }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +   (Type)i; }  \
  inline Type operator-  (const TypeR& i) const { return *this -   (Type)i; }  \
  inline Type operator*  (const TypeR& i) const { return *this *   (Type)i; }  \
  inline Type operator/  (const TypeR& i) const { return *this /   (Type)i; }  \
  inline Type operator%  (const TypeR& i) const { return *this %   (Type)i; }  \
  inline Type operator^  (const TypeR& i) const { return *this ^   (Type)i; }  \
  inline Type operator&  (const TypeR& i) const { return *this &   (Type)i; }  \
  inline Type operator|  (const TypeR& i) const { return *this |   (Type)i; }  \
  inline Type operator<  (const TypeR& i) const { return *this <   (Type)i; }  \
  inline Type operator>  (const TypeR& i) const { return *this >   (Type)i; }  \
  inline Type operator+= (const TypeR& i)       { return *this +=  (Type)i; }  \
  inline Type operator-= (const TypeR& i)       { return *this -=  (Type)i; }  \
  inline Type operator*= (const TypeR& i)       { return *this *=  (Type)i; }  \
  inline Type operator/= (const TypeR& i)       { return *this /=  (Type)i; }  \
  inline Type operator%= (const TypeR& i)       { return *this %=  (Type)i; }  \
  inline Type operator^= (const TypeR& i)       { return *this ^=  (Type)i; }  \
  inline Type operator&= (const TypeR& i)       { return *this &=  (Type)i; }  \
  inline Type operator|= (const TypeR& i)       { return *this |=  (Type)i; }  \
  inline Type operator<< (const TypeR& i) const { return *this <<  (Type)i; }  \
  inline Type operator>> (const TypeR& i) const { return *this >>  (Type)i; }  \
  inline Type operator<<=(const TypeR& i)       { return *this <<= (Type)i; }  \
  inline Type operator>>=(const TypeR& i)       { return *this >>= (Type)i; }  \
  inline Logical operator==(const TypeR&i)const { return *this ==  (Type)i; }  \
  inline Logical operator!=(const TypeR&i)const { return *this !=  (Type)i; }  \
  inline Logical operator<=(const TypeR&i)const { return *this <=  (Type)i; }  \
  inline Logical operator>=(const TypeR&i)const { return *this >=  (Type)i; }  \
  inline Logical operator&&(const TypeR&i)const                                \
                                                { return *data &&  (Type)i; }  \
  inline Logical operator||(const TypeR&i)const                                \
                                                { return *data ||  (Type)i; }  \
  inline double operator+(const double&i) const { return (double)*data + i; }  \
  inline double operator-(const double&i) const { return (double)*data - i; }  \
  inline double operator*(const double&i) const { return (double)*data * i; }  \
  inline double operator/(const double&i) const { return (double)*data / i; }  \
  inline float  operator+(const float& i) const { return  (float)*data + i; }  \
  inline float  operator-(const float& i) const { return  (float)*data - i; }  \
  inline float  operator*(const float& i) const { return  (float)*data * i; }  \
  inline float  operator/(const float& i) const { return  (float)*data / i; }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Type operator+  (const Type& i,const TypeR& j) { return i  +  (Type)j; }\
inline Type operator-  (const Type& i,const TypeR& j) { return i  -  (Type)j; }\
inline Type operator*  (const Type& i,const TypeR& j) { return i  *  (Type)j; }\
inline Type operator/  (const Type& i,const TypeR& j) { return i  /  (Type)j; }\
inline Type operator%  (const Type& i,const TypeR& j) { return i  %  (Type)j; }\
inline Type operator^  (const Type& i,const TypeR& j) { return i  ^  (Type)j; }\
inline Type operator&  (const Type& i,const TypeR& j) { return i  &  (Type)j; }\
inline Type operator|  (const Type& i,const TypeR& j) { return i  |  (Type)j; }\
inline Type operator<  (const Type& i,const TypeR& j) { return i  <  (Type)j; }\
inline Type operator>  (const Type& i,const TypeR& j) { return i  >  (Type)j; }\
inline Type operator+= (      Type& i,const TypeR& j) { return i +=  (Type)j; }\
inline Type operator-= (      Type& i,const TypeR& j) { return i -=  (Type)j; }\
inline Type operator*= (      Type& i,const TypeR& j) { return i *=  (Type)j; }\
inline Type operator/= (      Type& i,const TypeR& j) { return i /=  (Type)j; }\
inline Type operator%= (      Type& i,const TypeR& j) { return i %=  (Type)j; }\
inline Type operator^= (      Type& i,const TypeR& j) { return i ^=  (Type)j; }\
inline Type operator&= (      Type& i,const TypeR& j) { return i &=  (Type)j; }\
inline Type operator|= (      Type& i,const TypeR& j) { return i |=  (Type)j; }\
inline Type operator<< (const Type& i,const TypeR& j) { return i <<  (Type)j; }\
inline Type operator>> (const Type& i,const TypeR& j) { return i >>  (Type)j; }\
inline Type operator<<=(      Type& i,const TypeR& j) { return i <<= (Type)j; }\
inline Type operator>>=(      Type& i,const TypeR& j) { return i >>= (Type)j; }\
inline Logical operator==(const Type&i,const TypeR&j) { return i ==  (Type)j; }\
inline Logical operator!=(const Type&i,const TypeR&j) { return i !=  (Type)j; }\
inline Logical operator<=(const Type&i,const TypeR&j) { return i <=  (Type)j; }\
inline Logical operator>=(const Type&i,const TypeR&j) { return i >=  (Type)j; }\
inline Logical operator&&(const Type&i,const TypeR&j) { return i &&  (Type)j; }\
inline Logical operator||(const Type&i,const TypeR&j) { return i ||  (Type)j; }\
inline Logical operator&&(const Foobar& i,const TypeR& j)                      \
                                                      { return i &&  (Type)j; }\
inline Logical operator||(const Foobar& i,const TypeR& j)                      \
                                                      { return i ||  (Type)j; }\
inline double operator+(const double& i,const TypeR& j)                        \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeR& j)                        \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeR& j)                        \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeR& j)                        \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeR& j)                         \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeR& j)                         \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeR& j)                         \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeR& j)                         \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s,const TypeR& i)                          \
  { s << (Type)i; return s; }

#elif defined __DECCXX // for the brain-damaged DEC cxx compiler.
#define ReferenceType(TypeR,Type,Foobar)                                       \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&()                       { return *data;             }  \
  inline operator const Type&() const           { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Type operator~  (             ) const  { return ~ *data;           }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Type operator%= (const Type& i)        { return *data %=  i;       }  \
  inline Type operator^= (const Type& i)        { return *data ^=  i;       }  \
  inline Type operator&= (const Type& i)        { return *data &=  i;       }  \
  inline Type operator|= (const Type& i)        { return *data |=  i;       }  \
  inline Type operator<<=(const Type& i)        { return *data <<= i;       }  \
  inline Type operator>>=(const Type& i)        { return *data >>= i;       }  \
  inline Logical operator&&(const Type&i)const  { return *data &&  i;       }  \
  inline Logical operator||(const Type&i)const  { return *data ||  i;       }  \
  inline Logical operator&&(const Foobar& i) const                             \
                                                { return *data &&  i;       }  \
  inline Logical operator||(const Foobar& i) const                             \
                                                { return *data ||  i;       }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +   (Type)i; }  \
  inline Type operator-  (const TypeR& i) const { return *this -   (Type)i; }  \
  inline Type operator*  (const TypeR& i) const { return *this *   (Type)i; }  \
  inline Type operator/  (const TypeR& i) const { return *this /   (Type)i; }  \
  inline Type operator%  (const TypeR& i) const { return *this %   (Type)i; }  \
  inline Type operator^  (const TypeR& i) const { return *this ^   (Type)i; }  \
  inline Type operator&  (const TypeR& i) const { return *this &   (Type)i; }  \
  inline Type operator|  (const TypeR& i) const { return *this |   (Type)i; }  \
  inline Type operator<  (const TypeR& i) const { return *this <   (Type)i; }  \
  inline Type operator>  (const TypeR& i) const { return *this >   (Type)i; }  \
  inline Type operator+= (const TypeR& i)       { return *this +=  (Type)i; }  \
  inline Type operator-= (const TypeR& i)       { return *this -=  (Type)i; }  \
  inline Type operator*= (const TypeR& i)       { return *this *=  (Type)i; }  \
  inline Type operator/= (const TypeR& i)       { return *this /=  (Type)i; }  \
  inline Type operator%= (const TypeR& i)       { return *this %=  (Type)i; }  \
  inline Type operator^= (const TypeR& i)       { return *this ^=  (Type)i; }  \
  inline Type operator&= (const TypeR& i)       { return *this &=  (Type)i; }  \
  inline Type operator|= (const TypeR& i)       { return *this |=  (Type)i; }  \
  inline Type operator<< (const TypeR& i) const { return *this <<  (Type)i; }  \
  inline Type operator>> (const TypeR& i) const { return *this >>  (Type)i; }  \
  inline Type operator<<=(const TypeR& i)       { return *this <<= (Type)i; }  \
  inline Type operator>>=(const TypeR& i)       { return *this >>= (Type)i; }  \
  inline Logical operator==(const TypeR&i)const { return *this ==  (Type)i; }  \
  inline Logical operator!=(const TypeR&i)const { return *this !=  (Type)i; }  \
  inline Logical operator<=(const TypeR&i)const { return *this <=  (Type)i; }  \
  inline Logical operator>=(const TypeR&i)const { return *this >=  (Type)i; }  \
  inline Logical operator&&(const TypeR&i)const { return *data &&  (Type)i; }  \
  inline Logical operator||(const TypeR&i)const { return *data ||  (Type)i; }  \
  inline double operator+(const double&i) const { return (double)*data + i; }  \
  inline double operator-(const double&i) const { return (double)*data - i; }  \
  inline double operator*(const double&i) const { return (double)*data * i; }  \
  inline double operator/(const double&i) const { return (double)*data / i; }  \
  inline float  operator+(const float& i) const { return  (float)*data + i; }  \
  inline float  operator-(const float& i) const { return  (float)*data - i; }  \
  inline float  operator*(const float& i) const { return  (float)*data * i; }  \
  inline float  operator/(const float& i) const { return  (float)*data / i; }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Logical operator&&(const Type&i, const TypeR&j) { return i && (Type)j; }\
inline Logical operator||(const Type&i, const TypeR&j) { return i || (Type)j; }\
inline Logical operator&&(const Foobar& i, const TypeR& j)                     \
                                                       { return i && (Type)j; }\
inline Logical operator||(const Foobar& i, const TypeR& j)                     \
                                                       { return i || (Type)j; }\
inline double operator+(const double& i,const TypeR& j)                        \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeR& j)                        \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeR& j)                        \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeR& j)                        \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeR& j)                         \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeR& j)                         \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeR& j)                         \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeR& j)                         \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeR& i)                         \
  { s << (Type)i; return s; }

#else
#define ReferenceType(TypeR,Type,Foobar)                                       \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&()                       { return *data;             }  \
  inline operator const Type&() const           { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator+  (const Type& i) const  { return *data +   i;       }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Type operator-  (const Type& i) const  { return *data -   i;       }  \
  inline Type operator*  (const Type& i) const  { return *data *   i;       }  \
  inline Type operator/  (const Type& i) const  { return *data /   i;       }  \
  inline Type operator%  (const Type& i) const  { return *data %   i;       }  \
  inline Type operator^  (const Type& i) const  { return *data ^   i;       }  \
  inline Type operator&  (const Type& i) const  { return *data &   i;       }  \
  inline Type operator|  (const Type& i) const  { return *data |   i;       }  \
  inline Type operator~  (             ) const  { return ~ *data;           }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator<  (const Type& i) const  { return *data <   i;       }  \
  inline Type operator>  (const Type& i) const  { return *data >   i;       }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Type operator%= (const Type& i)        { return *data %=  i;       }  \
  inline Type operator^= (const Type& i)        { return *data ^=  i;       }  \
  inline Type operator&= (const Type& i)        { return *data &=  i;       }  \
  inline Type operator|= (const Type& i)        { return *data |=  i;       }  \
  inline Type operator<< (const Type& i) const  { return *data <<  i;       }  \
  inline Type operator>> (const Type& i) const  { return *data >>  i;       }  \
  inline Type operator<<=(const Type& i)        { return *data <<= i;       }  \
  inline Type operator>>=(const Type& i)        { return *data >>= i;       }  \
  inline Logical operator==(const Type&i)const  { return *data ==  i;       }  \
  inline Logical operator!=(const Type&i)const  { return *data !=  i;       }  \
  inline Logical operator<=(const Type&i)const  { return *data <=  i;       }  \
  inline Logical operator>=(const Type&i)const  { return *data >=  i;       }  \
  inline Logical operator&&(const Type&i)const  { return *data &&  i;       }  \
  inline Logical operator||(const Type&i)const  { return *data ||  i;       }  \
  inline Logical operator&&(const Foobar& i) const                             \
                                                { return *data &&  i;       }  \
  inline Logical operator||(const Foobar& i) const                             \
                                                { return *data ||  i;       }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +   (Type)i; }  \
  inline Type operator-  (const TypeR& i) const { return *this -   (Type)i; }  \
  inline Type operator*  (const TypeR& i) const { return *this *   (Type)i; }  \
  inline Type operator/  (const TypeR& i) const { return *this /   (Type)i; }  \
  inline Type operator%  (const TypeR& i) const { return *this %   (Type)i; }  \
  inline Type operator^  (const TypeR& i) const { return *this ^   (Type)i; }  \
  inline Type operator&  (const TypeR& i) const { return *this &   (Type)i; }  \
  inline Type operator|  (const TypeR& i) const { return *this |   (Type)i; }  \
  inline Type operator<  (const TypeR& i) const { return *this <   (Type)i; }  \
  inline Type operator>  (const TypeR& i) const { return *this >   (Type)i; }  \
  inline Type operator+= (const TypeR& i)       { return *this +=  (Type)i; }  \
  inline Type operator-= (const TypeR& i)       { return *this -=  (Type)i; }  \
  inline Type operator*= (const TypeR& i)       { return *this *=  (Type)i; }  \
  inline Type operator/= (const TypeR& i)       { return *this /=  (Type)i; }  \
  inline Type operator%= (const TypeR& i)       { return *this %=  (Type)i; }  \
  inline Type operator^= (const TypeR& i)       { return *this ^=  (Type)i; }  \
  inline Type operator&= (const TypeR& i)       { return *this &=  (Type)i; }  \
  inline Type operator|= (const TypeR& i)       { return *this |=  (Type)i; }  \
  inline Type operator<< (const TypeR& i) const { return *this <<  (Type)i; }  \
  inline Type operator>> (const TypeR& i) const { return *this >>  (Type)i; }  \
  inline Type operator<<=(const TypeR& i)       { return *this <<= (Type)i; }  \
  inline Type operator>>=(const TypeR& i)       { return *this >>= (Type)i; }  \
  inline Logical operator==(const TypeR&i)const { return *this ==  (Type)i; }  \
  inline Logical operator!=(const TypeR&i)const { return *this !=  (Type)i; }  \
  inline Logical operator<=(const TypeR&i)const { return *this <=  (Type)i; }  \
  inline Logical operator>=(const TypeR&i)const { return *this >=  (Type)i; }  \
  inline Logical operator&&(const TypeR&i)const { return *data &&  (Type)i; }  \
  inline Logical operator||(const TypeR&i)const { return *data ||  (Type)i; }  \
  inline double operator+(const double&i) const { return (double)*data + i; }  \
  inline double operator-(const double&i) const { return (double)*data - i; }  \
  inline double operator*(const double&i) const { return (double)*data * i; }  \
  inline double operator/(const double&i) const { return (double)*data / i; }  \
  inline float  operator+(const float& i) const { return  (float)*data + i; }  \
  inline float  operator-(const float& i) const { return  (float)*data - i; }  \
  inline float  operator*(const float& i) const { return  (float)*data * i; }  \
  inline float  operator/(const float& i) const { return  (float)*data / i; }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Type operator+  (const Type& i, const TypeR& j) { return i  + (Type)j; }\
inline Type operator-  (const Type& i, const TypeR& j) { return i  - (Type)j; }\
inline Type operator*  (const Type& i, const TypeR& j) { return i  * (Type)j; }\
inline Type operator/  (const Type& i, const TypeR& j) { return i  / (Type)j; }\
inline Type operator%  (const Type& i, const TypeR& j) { return i  % (Type)j; }\
inline Type operator^  (const Type& i, const TypeR& j) { return i  ^ (Type)j; }\
inline Type operator&  (const Type& i, const TypeR& j) { return i  & (Type)j; }\
inline Type operator|  (const Type& i, const TypeR& j) { return i  | (Type)j; }\
inline Type operator<  (const Type& i, const TypeR& j) { return i  < (Type)j; }\
inline Type operator>  (const Type& i, const TypeR& j) { return i  > (Type)j; }\
inline Type operator+= (      Type& i, const TypeR& j) { return i += (Type)j; }\
inline Type operator-= (      Type& i, const TypeR& j) { return i -= (Type)j; }\
inline Type operator*= (      Type& i, const TypeR& j) { return i *= (Type)j; }\
inline Type operator/= (      Type& i, const TypeR& j) { return i /= (Type)j; }\
inline Type operator%= (      Type& i, const TypeR& j) { return i %= (Type)j; }\
inline Type operator^= (      Type& i, const TypeR& j) { return i ^= (Type)j; }\
inline Type operator&= (      Type& i, const TypeR& j) { return i &= (Type)j; }\
inline Type operator|= (      Type& i, const TypeR& j) { return i |= (Type)j; }\
inline Type operator<< (const Type& i, const TypeR& j) { return i << (Type)j; }\
inline Type operator>> (const Type& i, const TypeR& j) { return i >> (Type)j; }\
inline Type operator<<=(      Type& i, const TypeR& j) { return i <<=(Type)j; }\
inline Type operator>>=(      Type& i, const TypeR& j) { return i >>=(Type)j; }\
inline Logical operator==(const Type&i, const TypeR&j) { return i == (Type)j; }\
inline Logical operator!=(const Type&i, const TypeR&j) { return i != (Type)j; }\
inline Logical operator<=(const Type&i, const TypeR&j) { return i <= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeR&j) { return i >= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeR&j) { return i && (Type)j; }\
inline Logical operator||(const Type&i, const TypeR&j) { return i || (Type)j; }\
inline Logical operator&&(const Foobar& i, const TypeR& j)                     \
                                                       { return i && (Type)j; }\
inline Logical operator||(const Foobar& i, const TypeR& j)                     \
                                                       { return i || (Type)j; }\
inline double operator+(const double& i,const TypeR& j)                        \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeR& j)                        \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeR& j)                        \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeR& j)                        \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeR& j)                         \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeR& j)                         \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeR& j)                         \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeR& j)                         \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeR& i)                         \
  { s << (Type)i; return s; }
#endif

ReferenceType(charR,            char,              Logical) // class charR
ReferenceType(shortIntR,        short int,         Logical) // class shortIntR
ReferenceType(longIntR,         long int,          Logical) // class longIntR
ReferenceType(intR,             int,               void*)   // class intR
ReferenceType(unsignedCharR,    unsigned char,     Logical) // unsignedCharR
ReferenceType(unsignedShortIntR,unsigned short int,Logical) // unsignedShortIntR
ReferenceType(unsignedLongIntR, unsigned long int, Logical) // unsignedLongIntR
ReferenceType(unsignedIntR,     unsigned int,      Logical) // unsignedIntR

#ifdef LONGINT
typedef longIntR IntegerR;
#else
typedef intR     IntegerR;
#endif // LONGINT

#if defined GNU || defined __PHOTON || defined __DECCXX
class boolR:
 public ReferenceCounting {
 public:
  inline boolR()                                { data =  NULL;             }
  inline boolR(const boolR& i)                  { data =  i.data;           }
  inline virtual ~boolR()                       {                           }
#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
  inline operator bool&() const                 { return *data;             }
  inline operator bool () const                 { return *data;             }
#else
  inline operator bool&()                       { return *data;             }
  inline operator const bool&() const           { return *data;             }
#endif // defined __sun && ! defined __SVR4
  inline bool* operator&()                      { return  data;             }
  inline boolR& operator=(const bool&  i)       { *data =  i; return *this; }
  inline boolR& operator=(const boolR& i)  { *data = *i.data; return *this; }
  inline void reference(bool&  i)               { data = (bool*)&i;         }
  inline void reference(const boolR& i)         { data =  i.data;           }
  inline virtual void breakReference()          { data =  NULL;             }
  inline virtual void consistencyCheck() const  {
      if (data == NULL) {
          cerr << "ReferenceType::consistencyCheck():  "
               << "data == NULL for "
               << getClassName() << " " << getGlobalID() << "." << endl;
          exit(1);
      }
  }
  inline bool operator+  (             ) const  { return *data;             }
  inline bool operator-  (             ) const  { return - *data;           }
  inline bool operator~  (             ) const  { return ~ *data;           }
  inline Logical operator!(            ) const  { return ! *data;           }
#ifndef __DECCXX
  inline bool operator+  (const bool& i) const  { return *data +   i;       }
  inline bool operator-  (const bool& i) const  { return *data -   i;       }
  inline bool operator*  (const bool& i) const  { return *data *   i;       }
  inline bool operator/  (const bool& i) const  { return *data /   i;       }
  inline bool operator%  (const bool& i) const  { return *data %   i;       }
  inline bool operator^  (const bool& i) const  { return *data ^   i;       }
  inline bool operator&  (const bool& i) const  { return *data &   i;       }
  inline bool operator|  (const bool& i) const  { return *data |   i;       }
  inline bool operator<  (const bool& i) const  { return *data <   i;       }
  inline bool operator>  (const bool& i) const  { return *data >   i;       }
  inline bool operator<< (const bool& i) const  { return *data <<  i;       }
  inline bool operator>> (const bool& i) const  { return *data >>  i;       }
  inline Logical operator==(const bool&i)const  { return *data ==  i;       }
  inline Logical operator!=(const bool&i)const  { return *data !=  i;       }
  inline Logical operator<=(const bool&i)const  { return *data <=  i;       }
  inline Logical operator>=(const bool&i)const  { return *data >=  i;       }
#endif // __DECCXX
  inline Logical operator&&(const bool&i) const { return *data &&  i;       }
  inline Logical operator||(const bool&i) const { return *data ||  i;       }
  inline bool operator+  (const boolR& i) const { return *this +   (bool)i; }
  inline bool operator-  (const boolR& i) const { return *this -   (bool)i; }
  inline bool operator*  (const boolR& i) const { return *this *   (bool)i; }
  inline bool operator/  (const boolR& i) const { return *this /   (bool)i; }
  inline bool operator%  (const boolR& i) const { return *this %   (bool)i; }
  inline bool operator^  (const boolR& i) const { return *this ^   (bool)i; }
  inline bool operator&  (const boolR& i) const { return *this &   (bool)i; }
  inline bool operator|  (const boolR& i) const { return *this |   (bool)i; }
  inline bool operator<  (const boolR& i) const { return *this <   (bool)i; }
  inline bool operator>  (const boolR& i) const { return *this >   (bool)i; }
  inline bool operator<< (const boolR& i) const { return *this <<  (bool)i; }
  inline bool operator>> (const boolR& i) const { return *this >>  (bool)i; }
  inline Logical operator==(const boolR&i)const { return *this ==  (bool)i; }
  inline Logical operator!=(const boolR&i)const { return *this !=  (bool)i; }
  inline Logical operator<=(const boolR&i)const { return *this <=  (bool)i; }
  inline Logical operator>=(const boolR&i)const { return *this >=  (bool)i; }
  inline Logical operator&&(const boolR&i)const { return *this &&  (bool)i; }
  inline Logical operator||(const boolR&i)const { return *this ||  (bool)i; }
  inline double operator+(const double&i) const { return (double)*data + i; }
  inline double operator-(const double&i) const { return (double)*data - i; }
  inline double operator*(const double&i) const { return (double)*data * i; }
  inline double operator/(const double&i) const { return (double)*data / i; }
  inline float  operator+(const float& i) const { return  (float)*data + i; }
  inline float  operator-(const float& i) const { return  (float)*data - i; }
  inline float  operator*(const float& i) const { return  (float)*data * i; }
  inline float  operator/(const float& i) const { return  (float)*data / i; }
 private:
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=((boolR&)x); }
  inline virtual void reference(const ReferenceCounting& x)
    { reference((boolR&)x); }
  inline virtual ReferenceCounting* virtualConstructor
    (const CopyType ct = SHALLOW) const
    { if (&ct); return new boolR(*this); }
  bool* data;
};
#ifndef __DECCXX
inline bool operator+  (const bool& i, const boolR& j) { return i  + (bool)j; }
inline bool operator-  (const bool& i, const boolR& j) { return i  - (bool)j; }
inline bool operator*  (const bool& i, const boolR& j) { return i  * (bool)j; }
inline bool operator/  (const bool& i, const boolR& j) { return i  / (bool)j; }
inline bool operator%  (const bool& i, const boolR& j) { return i  % (bool)j; }
inline bool operator^  (const bool& i, const boolR& j) { return i  ^ (bool)j; }
inline bool operator&  (const bool& i, const boolR& j) { return i  & (bool)j; }
inline bool operator|  (const bool& i, const boolR& j) { return i  | (bool)j; }
inline bool operator<  (const bool& i, const boolR& j) { return i  < (bool)j; }
inline bool operator>  (const bool& i, const boolR& j) { return i  > (bool)j; }
inline bool operator<< (const bool& i, const boolR& j) { return i << (bool)j; }
inline bool operator>> (const bool& i, const boolR& j) { return i >> (bool)j; }
inline Logical operator<=(const bool&i, const boolR&j) { return i <= (bool)j; }
inline Logical operator>=(const bool&i, const boolR&j) { return i >= (bool)j; }
#endif // __DECCXX
inline Logical operator&&(const bool&i, const boolR&j) { return i && (bool)j; }
inline Logical operator||(const bool&i, const boolR&j) { return i || (bool)j; }
inline double operator+(const double& i,const boolR& j)
                                                { return i + (double)(bool)j; }
inline double operator-(const double& i,const boolR& j)
                                                { return i - (double)(bool)j; }
inline double operator*(const double& i,const boolR& j)
                                                { return i * (double)(bool)j; }
inline double operator/(const double& i,const boolR& j)
                                                { return i / (double)(bool)j; }
inline float operator+(const float& i, const boolR& j)
                                                { return i + (float) (bool)j; }
inline float operator-(const float& i, const boolR& j)
                                                { return i - (float) (bool)j; }
inline float operator*(const float& i, const boolR& j)
                                                { return i * (float) (bool)j; }
inline float operator/(const float& i, const boolR& j)
                                                { return i / (float) (bool)j; }
inline ostream& operator<<(ostream& s, const boolR& i)
  { s << (bool)i; return s; }
typedef boolR LogicalR;
#else
typedef IntegerR LogicalR;
#endif // defined GNU || defined __PHOTON || defined __DECCXX

#undef ReferenceType

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceType(TypeR,Type)                                              \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&() const                 { return *data;             }  \
  inline operator Type () const                 { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator+  (const Type& i) const  { return *data +   i;       }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Type operator-  (const Type& i) const  { return *data -   i;       }  \
  inline Type operator*  (const Type& i) const  { return *data *   i;       }  \
  inline Type operator/  (const Type& i) const  { return *data /   i;       }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator<  (const Type& i) const  { return *data <   i;       }  \
  inline Type operator>  (const Type& i) const  { return *data >   i;       }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Logical operator==(const Type&i) const { return *data ==  i;       }  \
  inline Logical operator!=(const Type&i) const { return *data !=  i;       }  \
  inline Logical operator<=(const Type&i) const { return *data <=  i;       }  \
  inline Logical operator>=(const Type&i) const { return *data >=  i;       }  \
  inline Logical operator&&(const Type&i) const { return *data &&  i;       }  \
  inline Logical operator||(const Type&i) const { return *data ||  i;       }  \
  inline Logical operator||(const Logical& i) const                            \
                                                { return *data  ||  i;      }  \
  inline Logical operator&&(const Logical& i) const                            \
                                                { return *data  &&  i;      }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +   (Type)i; }  \
  inline Type operator-  (const TypeR& i) const { return *this -   (Type)i; }  \
  inline Type operator*  (const TypeR& i) const { return *this *   (Type)i; }  \
  inline Type operator/  (const TypeR& i) const { return *this /   (Type)i; }  \
  inline Type operator<  (const TypeR& i) const { return *this <   (Type)i; }  \
  inline Type operator>  (const TypeR& i) const { return *this >   (Type)i; }  \
  inline Type operator+= (const TypeR& i)       { return *this +=  (Type)i; }  \
  inline Type operator-= (const TypeR& i)       { return *this -=  (Type)i; }  \
  inline Type operator*= (const TypeR& i)       { return *this *=  (Type)i; }  \
  inline Type operator/= (const TypeR& i)       { return *this /=  (Type)i; }  \
  inline Logical operator==(const TypeR&i)const { return *this ==  (Type)i; }  \
  inline Logical operator!=(const TypeR&i)const { return *this !=  (Type)i; }  \
  inline Logical operator<=(const TypeR&i)const { return *this <=  (Type)i; }  \
  inline Logical operator>=(const TypeR&i)const { return *this >=  (Type)i; }  \
  inline Logical operator&&(const TypeR&i)const { return *this &&  (Type)i; }  \
  inline Logical operator||(const TypeR&i)const { return *this ||  (Type)i; }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Type operator+  (const Type& i, const TypeR& j) { return i + (Type)j; } \
inline Type operator-  (const Type& i, const TypeR& j) { return i - (Type)j; } \
inline Type operator*  (const Type& i, const TypeR& j) { return i * (Type)j; } \
inline Type operator/  (const Type& i, const TypeR& j) { return i / (Type)j; } \
inline Type operator<  (const Type& i, const TypeR& j) { return i < (Type)j; } \
inline Type operator>  (const Type& i, const TypeR& j) { return i > (Type)j; } \
inline Type operator+= (      Type& i, const TypeR& j) { return i+= (Type)j; } \
inline Type operator-= (      Type& i, const TypeR& j) { return i-= (Type)j; } \
inline Type operator*= (      Type& i, const TypeR& j) { return i*= (Type)j; } \
inline Type operator/= (      Type& i, const TypeR& j) { return i/= (Type)j; } \
inline Logical operator==(const Type&i, const TypeR&j) { return i== (Type)j; } \
inline Logical operator!=(const Type&i, const TypeR&j) { return i!= (Type)j; } \
inline Logical operator<=(const Type&i, const TypeR&j) { return i<= (Type)j; } \
inline Logical operator>=(const Type&i, const TypeR&j) { return i>= (Type)j; } \
inline Logical operator&&(const Type&i, const TypeR&j) { return i&& (Type)j; } \
inline Logical operator||(const Type&i, const TypeR&j) { return i|| (Type)j; } \
inline Logical operator&&(const Logical& i, const TypeR& j)                    \
                                                       { return i&& (Type)j; } \
inline Logical operator||(const Logical& i, const TypeR& j)                    \
                                                       { return i|| (Type)j; } \
inline ostream& operator<<(ostream& s, const TypeR& i)                         \
  { s << (Type)i; return s; }

#elif defined __DECCXX // for the brain-damaged DEC cxx compiler.
#define ReferenceType(TypeR,Type)                                              \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&()                       { return *data;             }  \
  inline operator const Type&() const           { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Logical operator&&(const Type&i)const  { return *data &&  i;       }  \
  inline Logical operator||(const Type&i)const  { return *data ||  i;       }  \
  inline Logical operator&&(const Logical& i) const                            \
                                                { return *data &&  i;       }  \
  inline Logical operator||(const Logical& i) const                            \
                                                { return *data ||  i;       }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +   (Type)i; }  \
  inline Type operator-  (const TypeR& i) const { return *this -   (Type)i; }  \
  inline Type operator*  (const TypeR& i) const { return *this *   (Type)i; }  \
  inline Type operator/  (const TypeR& i) const { return *this /   (Type)i; }  \
  inline Type operator<  (const TypeR& i) const { return *this <   (Type)i; }  \
  inline Type operator>  (const TypeR& i) const { return *this >   (Type)i; }  \
  inline Type operator+= (const TypeR& i)       { return *this +=  (Type)i; }  \
  inline Type operator-= (const TypeR& i)       { return *this -=  (Type)i; }  \
  inline Type operator*= (const TypeR& i)       { return *this *=  (Type)i; }  \
  inline Type operator/= (const TypeR& i)       { return *this /=  (Type)i; }  \
  inline Logical operator==(const TypeR&i)const { return *this ==  (Type)i; }  \
  inline Logical operator!=(const TypeR&i)const { return *this !=  (Type)i; }  \
  inline Logical operator<=(const TypeR&i)const { return *this <=  (Type)i; }  \
  inline Logical operator>=(const TypeR&i)const { return *this >=  (Type)i; }  \
  inline Logical operator&&(const TypeR&i)const { return *data &&  (Type)i; }  \
  inline Logical operator||(const TypeR&i)const { return *data ||  (Type)i; }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Logical operator&&(const Type&i, const TypeR&j) { return i && (Type)j; }\
inline Logical operator||(const Type&i, const TypeR&j) { return i || (Type)j; }\
inline Logical operator&&(const Logical& i, const TypeR& j)                    \
                                                       { return i && (Type)j; }\
inline Logical operator||(const Logical& i, const TypeR& j)                    \
                                                       { return i || (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeR& i)                         \
  { s << (Type)i; return s; }

#else
#define ReferenceType(TypeR,Type)                                              \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                                { data =  NULL;             }  \
  inline TypeR(const TypeR& i)                  { data =  i.data;           }  \
  inline virtual ~TypeR()                       {                           }  \
  inline operator Type&()                       { return *data;             }  \
  inline operator const Type&() const           { return *data;             }  \
  inline Type* operator&()                      { return  data;             }  \
  inline TypeR& operator=(const Type&  i)       { *data =  i; return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)                { data = (Type*)&i;         }  \
  inline void reference(const TypeR& i)         { data =  i.data;           }  \
  inline virtual void breakReference()          { data =  NULL;             }  \
  inline virtual void consistencyCheck() const  {                              \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
  inline Type operator+  (             ) const  { return *data;             }  \
  inline Type operator+  (const Type& i) const  { return *data +   i;       }  \
  inline Type operator-  (             ) const  { return - *data;           }  \
  inline Type operator-  (const Type& i) const  { return *data -   i;       }  \
  inline Type operator*  (const Type& i) const  { return *data *   i;       }  \
  inline Type operator/  (const Type& i) const  { return *data /   i;       }  \
  inline Logical operator!(            ) const  { return ! *data;           }  \
  inline Type operator<  (const Type& i) const  { return *data <   i;       }  \
  inline Type operator>  (const Type& i) const  { return *data >   i;       }  \
  inline Type operator+= (const Type& i)        { return *data +=  i;       }  \
  inline Type operator-= (const Type& i)        { return *data -=  i;       }  \
  inline Type operator*= (const Type& i)        { return *data *=  i;       }  \
  inline Type operator/= (const Type& i)        { return *data /=  i;       }  \
  inline Logical operator==(const Type&i) const { return *data ==  i;       }  \
  inline Logical operator!=(const Type&i) const { return *data !=  i;       }  \
  inline Logical operator<=(const Type&i) const { return *data <=  i;       }  \
  inline Logical operator>=(const Type&i) const { return *data >=  i;       }  \
  inline Logical operator&&(const Type&i) const { return *data &&  i;       }  \
  inline Logical operator||(const Type&i) const { return *data ||  i;       }  \
  inline Logical operator&&(const Logical& i) const                            \
                                                { return *data &&  i;       }  \
  inline Logical operator||(const Logical& i) const                            \
                                                { return *data ||  i;       }  \
  inline Type operator++ (             )        { return ++ *data;          }  \
  inline Type operator++ (int          )        { return (*data)++;         }  \
  inline Type operator-- (             )        { return -- *data;          }  \
  inline Type operator-- (int          )        { return (*data)--;         }  \
  inline Type operator+  (const TypeR& i) const { return *this +  (Type)i;  }  \
  inline Type operator-  (const TypeR& i) const { return *this -  (Type)i;  }  \
  inline Type operator*  (const TypeR& i) const { return *this *  (Type)i;  }  \
  inline Type operator/  (const TypeR& i) const { return *this /  (Type)i;  }  \
  inline Type operator<  (const TypeR& i) const { return *this <  (Type)i;  }  \
  inline Type operator>  (const TypeR& i) const { return *this >  (Type)i;  }  \
  inline Type operator+= (const TypeR& i)       { return *this += (Type)i;  }  \
  inline Type operator-= (const TypeR& i)       { return *this -= (Type)i;  }  \
  inline Type operator*= (const TypeR& i)       { return *this *= (Type)i;  }  \
  inline Type operator/= (const TypeR& i)       { return *this /= (Type)i;  }  \
  inline Logical operator==(const TypeR&i)const { return *this == (Type)i;  }  \
  inline Logical operator!=(const TypeR&i)const { return *this != (Type)i;  }  \
  inline Logical operator<=(const TypeR&i)const { return *this <= (Type)i;  }  \
  inline Logical operator>=(const TypeR&i)const { return *this >= (Type)i;  }  \
  inline Logical operator&&(const TypeR&i)const { return *this && (Type)i;  }  \
  inline Logical operator||(const TypeR&i)const { return *this || (Type)i;  }  \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};                                                                             \
inline Type operator+  (const Type& i, const TypeR& j) { return i  + (Type)j; }\
inline Type operator-  (const Type& i, const TypeR& j) { return i  - (Type)j; }\
inline Type operator*  (const Type& i, const TypeR& j) { return i  * (Type)j; }\
inline Type operator/  (const Type& i, const TypeR& j) { return i  / (Type)j; }\
inline Type operator<  (const Type& i, const TypeR& j) { return i  < (Type)j; }\
inline Type operator>  (const Type& i, const TypeR& j) { return i  > (Type)j; }\
inline Type operator+= (      Type& i, const TypeR& j) { return i += (Type)j; }\
inline Type operator-= (      Type& i, const TypeR& j) { return i -= (Type)j; }\
inline Type operator*= (      Type& i, const TypeR& j) { return i *= (Type)j; }\
inline Type operator/= (      Type& i, const TypeR& j) { return i /= (Type)j; }\
inline Logical operator==(const Type&i, const TypeR&j) { return i == (Type)j; }\
inline Logical operator!=(const Type&i, const TypeR&j) { return i != (Type)j; }\
inline Logical operator<=(const Type&i, const TypeR&j) { return i <= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeR&j) { return i >= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeR&j) { return i && (Type)j; }\
inline Logical operator||(const Type&i, const TypeR&j) { return i || (Type)j; }\
inline Logical operator&&(const Logical& i, const TypeR& j)                    \
                                                       { return i && (Type)j; }\
inline Logical operator||(const Logical& i, const TypeR& j)                    \
                                                       { return i || (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeR& i)                         \
  { s << (Type)i; return s; }
#endif

ReferenceType(floatR,      float)       // Declare class floatR
ReferenceType(doubleR,     double)      // Declare class doubleR
#if defined __DECCXX
ReferenceType(longDoubleR, long double) // Declare class longDoubleR
#endif // defined __DECCXX

#ifdef DOUBLE
typedef doubleR     RealR;
#if defined __DECCXX
typedef longDoubleR DoubleRealR;
#endif // defined __DECCXX
#else // ifndef DOUBLE
typedef floatR      RealR;
#if defined __DECCXX
typedef doubleR     DoubleRealR;
#endif // defined __DECCXX
#endif // DOUBLE

#undef ReferenceType

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceType(TypeR,Type)                                              \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                           { data =  NULL;                  }  \
  inline TypeR(const TypeR& i)             { data =  i.data;                }  \
  inline virtual ~TypeR()                  {                                }  \
  inline operator Type&() const            { return *data;                  }  \
  inline operator Type () const            { return *data;                  }  \
  inline Type* operator&()                 { return  data;                  }  \
  inline TypeR& operator=(const Type&  i)  { *data =  i;      return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)           { data = (Type*)&i;              }  \
  inline void reference(const TypeR& i)    { data =  i.data;                }  \
  inline virtual void breakReference()     { data =  NULL;                  }  \
  inline virtual void consistencyCheck() const {                               \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};

#else
#define ReferenceType(TypeR,Type)                                              \
class TypeR:                                                                   \
 public ReferenceCounting {                                                    \
 public:                                                                       \
  inline TypeR()                           { data =  NULL;                  }  \
  inline TypeR(const TypeR& i)             { data =  i.data;                }  \
  inline virtual ~TypeR()                  {                                }  \
  inline operator Type&()                  { return *data;                  }  \
  inline operator const Type&() const      { return *data;                  }  \
  inline Type* operator&()                 { return  data;                  }  \
  inline TypeR& operator=(const Type&  i)  { *data =  i;      return *this; }  \
  inline TypeR& operator=(const TypeR& i)  { *data = *i.data; return *this; }  \
  inline void reference(Type& i)           { data = (Type*)&i;              }  \
  inline void reference(const TypeR& i)    { data =  i.data;                }  \
  inline virtual void breakReference()     { data =  NULL;                  }  \
  inline virtual void consistencyCheck() const {                               \
      if (data == NULL) {                                                      \
          cerr << "ReferenceType::consistencyCheck():  "                       \
               << "data == NULL for "                                          \
               << getClassName() << " " << getGlobalID() << "." << endl;       \
          exit(1);                                                             \
      }                                                                        \
  }                                                                            \
 private:                                                                      \
  inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)      \
    { return operator=((TypeR&)x); }                                           \
  inline virtual void reference(const ReferenceCounting& x)                    \
    { reference((TypeR&)x); }                                                  \
  inline virtual ReferenceCounting* virtualConstructor                         \
    (const CopyType ct = SHALLOW) const                                        \
    { if (&ct); return new TypeR(*this); }                                     \
  Type* data;                                                                  \
};
#endif

#endif // _ReferenceTypes
