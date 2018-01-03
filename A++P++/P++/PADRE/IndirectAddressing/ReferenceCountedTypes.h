#ifndef _ReferenceCountedTypes
#define _ReferenceCountedTypes

//
// Who to blame:  Geoff Chesshire
//
// Usage:  ReferenceCountedType(TypeRC,Type)
//
// This declares class <TypeRC> to be a reference-counted <Type>.
// This class makes a reference-counted <Type> act like a <Type>.
//
// Public member functions:
//   Default constructor
//   Initialize constructor    Takes a const <Type>&.
//   Copy constructor          Does a deep copy by default.
//   Destructor                (virtual)
//   Conversion operator       Converts const <TypeRC>& to const <Type>&.
//   Conversion operator       Converts <TypeRC>& to <Type>&.
//   Address    operator       Returns <Type>* address.
//   Assignment operator       Takes a const <Type>&.
//   Assignment operator       Takes a const <TypeRC>&.  Does a deep copy.
//   Reference function        Does a shallow copy.
//   breakReference function   Replaces with a deep copy.
//   consistency check         Checks the consistency of the data structure.
//
// For native types, all the arithmetic operators and
// ostream&operator<<(ostream&,const<TypeRC>&) are defined also.
//

#include "ReferenceCounting.h"

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceCountedType(TypeRC,Type,Foobar)                               \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&()   const { return  rcData->data; }                  \
    inline operator Type ()   const { return  rcData->data; }                  \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator+  (const Type& i) const { return rcData->data +   i; }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Type operator-  (const Type& i) const { return rcData->data -   i; }\
    inline Type operator*  (const Type& i) const { return rcData->data *   i; }\
    inline Type operator/  (const Type& i) const { return rcData->data /   i; }\
    inline Type operator%  (const Type& i) const { return rcData->data %   i; }\
    inline Type operator^  (const Type& i) const { return rcData->data ^   i; }\
    inline Type operator&  (const Type& i) const { return rcData->data &   i; }\
    inline Type operator|  (const Type& i) const { return rcData->data |   i; }\
    inline Type operator~  (             ) const { return ~ rcData->data;     }\
    inline Logical operator!(            ) const { return ! rcData->data;     }\
    inline Type operator<  (const Type& i) const { return rcData->data <   i; }\
    inline Type operator>  (const Type& i) const { return rcData->data >   i; }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Type operator%= (const Type& i)       { return rcData->data %=  i; }\
    inline Type operator^= (const Type& i)       { return rcData->data ^=  i; }\
    inline Type operator&= (const Type& i)       { return rcData->data &=  i; }\
    inline Type operator|= (const Type& i)       { return rcData->data |=  i; }\
    inline Type operator<< (const Type& i) const { return rcData->data <<  i; }\
    inline Type operator>> (const Type& i) const { return rcData->data >>  i; }\
    inline Type operator<<=(const Type& i)       { return rcData->data <<= i; }\
    inline Type operator>>=(const Type& i)       { return rcData->data >>= i; }\
    inline Logical operator==(const Type&i)const { return rcData->data ==  i; }\
    inline Logical operator!=(const Type&i)const { return rcData->data !=  i; }\
    inline Logical operator<=(const Type&i)const { return rcData->data <=  i; }\
    inline Logical operator>=(const Type&i)const { return rcData->data >=  i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Foobar& i) const                           \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Foobar& i) const                           \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
    inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; }\
    inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; }\
    inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; }\
    inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; }\
    inline Type operator%  (const TypeRC& i) const { return *this %  (Type)i; }\
    inline Type operator^  (const TypeRC& i) const { return *this ^  (Type)i; }\
    inline Type operator&  (const TypeRC& i) const { return *this &  (Type)i; }\
    inline Type operator|  (const TypeRC& i) const { return *this |  (Type)i; }\
    inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; }\
    inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; }\
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Type operator%= (const TypeRC& i)       { return *this %= (Type)i; }\
    inline Type operator^= (const TypeRC& i)       { return *this ^= (Type)i; }\
    inline Type operator&= (const TypeRC& i)       { return *this &= (Type)i; }\
    inline Type operator|= (const TypeRC& i)       { return *this |= (Type)i; }\
    inline Type operator<< (const TypeRC& i) const { return *this << (Type)i; }\
    inline Type operator>> (const TypeRC& i) const { return *this >> (Type)i; }\
    inline Type operator<<=(const TypeRC& i)       { return *this <<=(Type)i; }\
    inline Type operator>>=(const TypeRC& i)       { return *this >>=(Type)i; }\
    inline Logical operator==(const TypeRC&i)const { return *this == (Type)i; }\
    inline Logical operator!=(const TypeRC&i)const { return *this != (Type)i; }\
    inline Logical operator<=(const TypeRC&i)const { return *this <= (Type)i; }\
    inline Logical operator>=(const TypeRC&i)const { return *this >= (Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
    inline double operator+(const double&i) const                              \
                                           { return (double)rcData->data + i; }\
    inline double operator-(const double&i) const                              \
                                           { return (double)rcData->data - i; }\
    inline double operator*(const double&i) const                              \
                                           { return (double)rcData->data * i; }\
    inline double operator/(const double&i) const                              \
                                           { return (double)rcData->data / i; }\
    inline float  operator+(const float& i) const                              \
                                           { return  (float)rcData->data + i; }\
    inline float  operator-(const float& i) const                              \
                                           { return  (float)rcData->data - i; }\
    inline float  operator*(const float& i) const                              \
                                           { return  (float)rcData->data * i; }\
    inline float  operator/(const float& i) const                              \
                                           { return  (float)rcData->data / i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Type operator+  (const Type& i, const TypeRC& j) {return i  + (Type)j; }\
inline Type operator-  (const Type& i, const TypeRC& j) {return i  - (Type)j; }\
inline Type operator*  (const Type& i, const TypeRC& j) {return i  * (Type)j; }\
inline Type operator/  (const Type& i, const TypeRC& j) {return i  / (Type)j; }\
inline Type operator%  (const Type& i, const TypeRC& j) {return i  % (Type)j; }\
inline Type operator^  (const Type& i, const TypeRC& j) {return i  ^ (Type)j; }\
inline Type operator&  (const Type& i, const TypeRC& j) {return i  & (Type)j; }\
inline Type operator|  (const Type& i, const TypeRC& j) {return i  | (Type)j; }\
inline Type operator<  (const Type& i, const TypeRC& j) {return i  < (Type)j; }\
inline Type operator>  (const Type& i, const TypeRC& j) {return i  > (Type)j; }\
inline Type operator+= (      Type& i, const TypeRC& j) {return i += (Type)j; }\
inline Type operator-= (      Type& i, const TypeRC& j) {return i -= (Type)j; }\
inline Type operator*= (      Type& i, const TypeRC& j) {return i *= (Type)j; }\
inline Type operator/= (      Type& i, const TypeRC& j) {return i /= (Type)j; }\
inline Type operator%= (      Type& i, const TypeRC& j) {return i %= (Type)j; }\
inline Type operator^= (      Type& i, const TypeRC& j) {return i ^= (Type)j; }\
inline Type operator&= (      Type& i, const TypeRC& j) {return i &= (Type)j; }\
inline Type operator|= (      Type& i, const TypeRC& j) {return i |= (Type)j; }\
inline Type operator<< (const Type& i, const TypeRC& j) {return i << (Type)j; }\
inline Type operator>> (const Type& i, const TypeRC& j) {return i >> (Type)j; }\
inline Type operator<<=(      Type& i, const TypeRC& j) {return i <<=(Type)j; }\
inline Type operator>>=(      Type& i, const TypeRC& j) {return i >>=(Type)j; }\
inline Logical operator==(const Type&i, const TypeRC&j) {return i == (Type)j; }\
inline Logical operator!=(const Type&i, const TypeRC&j) {return i != (Type)j; }\
inline Logical operator<=(const Type&i, const TypeRC&j) {return i <= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeRC&j) {return i >= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeRC&j) {return i && (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) {return i || (Type)j; }\
inline Logical operator&&(const Foobar&i,const TypeRC&j){return i && (Type)j; }\
inline Logical operator||(const Foobar&i,const TypeRC&j){return i || (Type)j; }\
inline double operator+(const double& i,const TypeRC& j)                       \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeRC& j)                       \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeRC& j)                       \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeRC& j)                       \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeRC& j)                        \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeRC& j)                        \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeRC& j)                        \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeRC& j)                        \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }

#elif defined __DECCXX // for the brain-damaged DEC cxx compiler.
#define ReferenceCountedType(TypeRC,Type,Foobar)                               \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() { return rcData->data; }                           \
    inline operator const Type&() const { return rcData->data; }               \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Type operator~  (             ) const { return ~ rcData->data;     }\
    inline Logical operator!(            ) const { return ! rcData->data; ;   }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Type operator%= (const Type& i)       { return rcData->data %=  i; }\
    inline Type operator^= (const Type& i)       { return rcData->data ^=  i; }\
    inline Type operator&= (const Type& i)       { return rcData->data &=  i; }\
    inline Type operator|= (const Type& i)       { return rcData->data |=  i; }\
    inline Type operator<<=(const Type& i)       { return rcData->data <<= i; }\
    inline Type operator>>=(const Type& i)       { return rcData->data >>= i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Foobar& i) const                           \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Foobar& i) const                           \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
/********************************************************************************/ \
/*  inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; } */ \
/*  inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; } */ \
/*  inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; } */ \
/*  inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; } */ \
/*  inline Type operator%  (const TypeRC& i) const { return *this %  (Type)i; } */ \
/*  inline Type operator^  (const TypeRC& i) const { return *this ^  (Type)i; } */ \
/*  inline Type operator&  (const TypeRC& i) const { return *this &  (Type)i; } */ \
/*  inline Type operator|  (const TypeRC& i) const { return *this |  (Type)i; } */ \
/*  inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; } */ \
/*  inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; } */ \
/********************************************************************************/ \
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Type operator%= (const TypeRC& i)       { return *this %= (Type)i; }\
    inline Type operator^= (const TypeRC& i)       { return *this ^= (Type)i; }\
    inline Type operator&= (const TypeRC& i)       { return *this &= (Type)i; }\
    inline Type operator|= (const TypeRC& i)       { return *this |= (Type)i; }\
/********************************************************************************/ \
/*  inline Type operator<< (const TypeRC& i) const { return *this << (Type)i; } */ \
/*  inline Type operator>> (const TypeRC& i) const { return *this >> (Type)i; } */ \
/********************************************************************************/ \
    inline Type operator<<=(const TypeRC& i)       { return *this <<=(Type)i; }\
    inline Type operator>>=(const TypeRC& i)       { return *this >>=(Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
    inline double operator+(const double&i) const                              \
                                           { return (double)rcData->data + i; }\
    inline double operator-(const double&i) const                              \
                                           { return (double)rcData->data - i; }\
    inline double operator*(const double&i) const                              \
                                           { return (double)rcData->data * i; }\
    inline double operator/(const double&i) const                              \
                                           { return (double)rcData->data / i; }\
    inline float  operator+(const float& i) const                              \
                                           { return  (float)rcData->data + i; }\
    inline float  operator-(const float& i) const                              \
                                           { return  (float)rcData->data - i; }\
    inline float  operator*(const float& i) const                              \
                                           { return  (float)rcData->data * i; }\
    inline float  operator/(const float& i) const                              \
                                           { return  (float)rcData->data / i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Logical operator&&(const Type&i, const TypeRC&j) { return i&& (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) { return i|| (Type)j; }\
inline Logical operator&&(const Foobar& i, const TypeRC& j)                    \
                                                        { return i&& (Type)j; }\
inline Logical operator||(const Foobar& i, const TypeRC& j)                    \
                                                        { return i|| (Type)j; }\
inline double operator+(const double& i,const TypeRC& j)                       \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeRC& j)                       \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeRC& j)                       \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeRC& j)                       \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeRC& j)                        \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeRC& j)                        \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeRC& j)                        \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeRC& j)                        \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }

#else
#define ReferenceCountedType(TypeRC,Type,Foobar)                               \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() { return rcData->data; }                           \
    inline operator const Type&() const { return rcData->data; }               \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator+  (const Type& i) const { return rcData->data +   i; }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Type operator-  (const Type& i) const { return rcData->data -   i; }\
    inline Type operator*  (const Type& i) const { return rcData->data *   i; }\
    inline Type operator/  (const Type& i) const { return rcData->data /   i; }\
    inline Type operator%  (const Type& i) const { return rcData->data %   i; }\
    inline Type operator^  (const Type& i) const { return rcData->data ^   i; }\
    inline Type operator&  (const Type& i) const { return rcData->data &   i; }\
    inline Type operator|  (const Type& i) const { return rcData->data |   i; }\
    inline Type operator~  (             ) const { return ~ rcData->data;     }\
    inline Logical operator!(            ) const { return ! rcData->data; ;   }\
    inline Type operator<  (const Type& i) const { return rcData->data <   i; }\
    inline Type operator>  (const Type& i) const { return rcData->data >   i; }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Type operator%= (const Type& i)       { return rcData->data %=  i; }\
    inline Type operator^= (const Type& i)       { return rcData->data ^=  i; }\
    inline Type operator&= (const Type& i)       { return rcData->data &=  i; }\
    inline Type operator|= (const Type& i)       { return rcData->data |=  i; }\
    inline Type operator<< (const Type& i) const { return rcData->data <<  i; }\
    inline Type operator>> (const Type& i) const { return rcData->data >>  i; }\
    inline Type operator<<=(const Type& i)       { return rcData->data <<= i; }\
    inline Type operator>>=(const Type& i)       { return rcData->data >>= i; }\
    inline Logical operator==(const Type&i)const { return rcData->data ==  i; }\
    inline Logical operator!=(const Type&i)const { return rcData->data !=  i; }\
    inline Logical operator<=(const Type&i)const { return rcData->data <=  i; }\
    inline Logical operator>=(const Type&i)const { return rcData->data >=  i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Foobar& i) const                           \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Foobar& i) const                           \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
    inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; }\
    inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; }\
    inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; }\
    inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; }\
    inline Type operator%  (const TypeRC& i) const { return *this %  (Type)i; }\
    inline Type operator^  (const TypeRC& i) const { return *this ^  (Type)i; }\
    inline Type operator&  (const TypeRC& i) const { return *this &  (Type)i; }\
    inline Type operator|  (const TypeRC& i) const { return *this |  (Type)i; }\
    inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; }\
    inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; }\
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Type operator%= (const TypeRC& i)       { return *this %= (Type)i; }\
    inline Type operator^= (const TypeRC& i)       { return *this ^= (Type)i; }\
    inline Type operator&= (const TypeRC& i)       { return *this &= (Type)i; }\
    inline Type operator|= (const TypeRC& i)       { return *this |= (Type)i; }\
    inline Type operator<< (const TypeRC& i) const { return *this << (Type)i; }\
    inline Type operator>> (const TypeRC& i) const { return *this >> (Type)i; }\
    inline Type operator<<=(const TypeRC& i)       { return *this <<=(Type)i; }\
    inline Type operator>>=(const TypeRC& i)       { return *this >>=(Type)i; }\
    inline Logical operator==(const TypeRC&i)const { return *this == (Type)i; }\
    inline Logical operator!=(const TypeRC&i)const { return *this != (Type)i; }\
    inline Logical operator<=(const TypeRC&i)const { return *this <= (Type)i; }\
    inline Logical operator>=(const TypeRC&i)const { return *this >= (Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
    inline double operator+(const double&i) const                              \
                                           { return (double)rcData->data + i; }\
    inline double operator-(const double&i) const                              \
                                           { return (double)rcData->data - i; }\
    inline double operator*(const double&i) const                              \
                                           { return (double)rcData->data * i; }\
    inline double operator/(const double&i) const                              \
                                           { return (double)rcData->data / i; }\
    inline float  operator+(const float& i) const                              \
                                           { return  (float)rcData->data + i; }\
    inline float  operator-(const float& i) const                              \
                                           { return  (float)rcData->data - i; }\
    inline float  operator*(const float& i) const                              \
                                           { return  (float)rcData->data * i; }\
    inline float  operator/(const float& i) const                              \
                                           { return  (float)rcData->data / i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Type operator+  (const Type& i, const TypeRC& j) { return i + (Type)j; }\
inline Type operator-  (const Type& i, const TypeRC& j) { return i - (Type)j; }\
inline Type operator*  (const Type& i, const TypeRC& j) { return i * (Type)j; }\
inline Type operator/  (const Type& i, const TypeRC& j) { return i / (Type)j; }\
inline Type operator%  (const Type& i, const TypeRC& j) { return i % (Type)j; }\
inline Type operator^  (const Type& i, const TypeRC& j) { return i ^ (Type)j; }\
inline Type operator&  (const Type& i, const TypeRC& j) { return i & (Type)j; }\
inline Type operator|  (const Type& i, const TypeRC& j) { return i | (Type)j; }\
inline Type operator<  (const Type& i, const TypeRC& j) { return i < (Type)j; }\
inline Type operator>  (const Type& i, const TypeRC& j) { return i > (Type)j; }\
inline Type operator+= (      Type& i, const TypeRC& j) { return i+= (Type)j; }\
inline Type operator-= (      Type& i, const TypeRC& j) { return i-= (Type)j; }\
inline Type operator*= (      Type& i, const TypeRC& j) { return i*= (Type)j; }\
inline Type operator/= (      Type& i, const TypeRC& j) { return i/= (Type)j; }\
inline Type operator%= (      Type& i, const TypeRC& j) { return i%= (Type)j; }\
inline Type operator^= (      Type& i, const TypeRC& j) { return i^= (Type)j; }\
inline Type operator&= (      Type& i, const TypeRC& j) { return i&= (Type)j; }\
inline Type operator|= (      Type& i, const TypeRC& j) { return i|= (Type)j; }\
inline Type operator<< (const Type& i, const TypeRC& j) { return i<< (Type)j; }\
inline Type operator>> (const Type& i, const TypeRC& j) { return i>> (Type)j; }\
inline Type operator<<=(      Type& i, const TypeRC& j) { return i<<=(Type)j; }\
inline Type operator>>=(      Type& i, const TypeRC& j) { return i>>=(Type)j; }\
inline Logical operator==(const Type&i, const TypeRC&j) { return i== (Type)j; }\
inline Logical operator!=(const Type&i, const TypeRC&j) { return i!= (Type)j; }\
inline Logical operator<=(const Type&i, const TypeRC&j) { return i<= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeRC&j) { return i>= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeRC&j) { return i&& (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) { return i|| (Type)j; }\
inline Logical operator&&(const Foobar& i, const TypeRC& j)                    \
                                                        { return i&& (Type)j; }\
inline Logical operator||(const Foobar& i, const TypeRC& j)                    \
                                                        { return i|| (Type)j; }\
inline double operator+(const double& i,const TypeRC& j)                       \
                                                { return i + (double)(Type)j; }\
inline double operator-(const double& i,const TypeRC& j)                       \
                                                { return i - (double)(Type)j; }\
inline double operator*(const double& i,const TypeRC& j)                       \
                                                { return i * (double)(Type)j; }\
inline double operator/(const double& i,const TypeRC& j)                       \
                                                { return i / (double)(Type)j; }\
inline float operator+(const float& i, const TypeRC& j)                        \
                                                { return i + (float) (Type)j; }\
inline float operator-(const float& i, const TypeRC& j)                        \
                                                { return i - (float) (Type)j; }\
inline float operator*(const float& i, const TypeRC& j)                        \
                                                { return i * (float) (Type)j; }\
inline float operator/(const float& i, const TypeRC& j)                        \
                                                { return i / (float) (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }
#endif

ReferenceCountedType(charRC,            char,              Logical) // charRC
ReferenceCountedType(shortIntRC,        short int,         Logical) // shortIntRC
ReferenceCountedType(longIntRC,         long int,          Logical) // longIntRC
ReferenceCountedType(intRC,             int,               void*)   // intRC
ReferenceCountedType(unsignedCharRC,    unsigned char,     Logical) // etc.
ReferenceCountedType(unsignedShortIntRC,unsigned short int,Logical)
ReferenceCountedType(unsignedLongIntRC, unsigned long int, Logical)
ReferenceCountedType(unsignedIntRC,     unsigned int,      Logical)

#ifdef LONGINT
typedef longIntRC IntegerRC;
#else
typedef intRC     IntegerRC;
#endif // LONGINT

#if defined GNU || defined __PHOTON || defined __DECCXX
class boolRC:
  public ReferenceCounting {
  public:
    class RCData:
      public ReferenceCounting {
      public:
        inline RCData(): ReferenceCounting() { }
        inline RCData(const RCData& x): ReferenceCounting(x) { }
        inline virtual ~RCData() { }
        bool data;
    };
    inline boolRC& operator=(const boolRC& x)
      { rcData->data = x.rcData->data; return *this; }
    inline virtual ~boolRC()
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }
    inline boolRC() { initialize(); }
    inline boolRC& operator=(const bool& x)
      { rcData->data = x; return *this; }
    inline boolRC(const boolRC& x, const CopyType ct = DEEP) {
        if (ct == DEEP) {
	    initialize();
	    *this = x;
        } else if (ct == SHALLOW) {
            rcData = x.rcData; rcData->incrementReferenceCount();
            reference(x);
        } else /* if (ct == NOCOPY) */ {
	    initialize();
        }
    }
    inline boolRC(const bool& x) { initialize(); *this = x; }
#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
    inline operator bool&() const { return rcData->data; }
    inline operator bool () const { return rcData->data; }
#else
    inline operator bool&() { return rcData->data; }
    inline operator const bool&() const { return rcData->data; }
#endif // defined __sun)&& ! defined __SVR4
    inline bool* operator&() { return &rcData->data; }
    inline void reference(const boolRC& x) {
        if (rcData->decrementReferenceCount() == 0) delete rcData;
        rcData = x.rcData; rcData->incrementReferenceCount();
    }
    inline void reference(TypeRC::RCData& x) {
        if (rcData->decrementReferenceCount() == 0) delete rcData;
        rcData = &x;
        if (!x.uncountedReferencesMayExist())
          rcData->incrementReferenceCount();
    }
    inline virtual void breakReference() {
        if (rcData->getReferenceCount() != 1) {
            boolRC x = *this;
            reference(x);
        }
    };
    inline virtual void consistencyCheck() const {
        if (rcData == NULL) {
            cerr << "ReferenceCountedType::consistencyCheck():  "
                 << "rcData == NULL for "
                 << getClassName() << " " << getGlobalID() << "." << endl;
            exit(1);
        }
        rcData->consistencyCheck();
    }
    inline bool operator+  (             ) const { return + rcData->data;     }
    inline bool operator-  (             ) const { return - rcData->data;     }
    inline bool operator~  (             ) const { return ~ rcData->data;     }
    inline Logical operator!(            ) const { return ! rcData->data;     }
#ifndef __DECCXX
    inline bool operator+  (const bool& i) const { return rcData->data +   i; }
    inline bool operator-  (const bool& i) const { return rcData->data -   i; }
    inline bool operator*  (const bool& i) const { return rcData->data *   i; }
    inline bool operator/  (const bool& i) const { return rcData->data /   i; }
    inline bool operator%  (const bool& i) const { return rcData->data %   i; }
    inline bool operator^  (const bool& i) const { return rcData->data ^   i; }
    inline bool operator&  (const bool& i) const { return rcData->data &   i; }
    inline bool operator|  (const bool& i) const { return rcData->data |   i; }
    inline bool operator<  (const bool& i) const { return rcData->data <   i; }
    inline bool operator>  (const bool& i) const { return rcData->data >   i; }
    inline bool operator<< (const bool& i) const { return rcData->data <<  i; }
    inline bool operator>> (const bool& i) const { return rcData->data >>  i; }
    inline Logical operator==(const bool&i)const { return rcData->data ==  i; }
    inline Logical operator!=(const bool&i)const { return rcData->data !=  i; }
    inline Logical operator<=(const bool&i)const { return rcData->data <=  i; }
    inline Logical operator>=(const bool&i)const { return rcData->data >=  i; }
#endif // __DECCXX
    inline Logical operator&&(const bool&i)const { return rcData->data &&  i; }
    inline Logical operator||(const bool&i)const { return rcData->data ||  i; }
//*****************************************************************************
//  inline bool operator+  (const boolRC& i) const { return *this +  (bool)i; }
//  inline bool operator-  (const boolRC& i) const { return *this -  (bool)i; }
//  inline bool operator*  (const boolRC& i) const { return *this *  (bool)i; }
//  inline bool operator/  (const boolRC& i) const { return *this /  (bool)i; }
//  inline bool operator%  (const boolRC& i) const { return *this %  (bool)i; }
//  inline bool operator^  (const boolRC& i) const { return *this ^  (bool)i; }
//  inline bool operator&  (const boolRC& i) const { return *this &  (bool)i; }
//  inline bool operator|  (const boolRC& i) const { return *this |  (bool)i; }
//  inline bool operator<  (const boolRC& i) const { return *this <  (bool)i; }
//  inline bool operator>  (const boolRC& i) const { return *this >  (bool)i; }
//  inline bool operator<< (const boolRC& i) const { return *this << (bool)i; }
//  inline bool operator>> (const boolRC& i) const { return *this >> (bool)i; }
//  inline Logical operator==(const boolRC&i)const { return *this == (bool)i; }
//  inline Logical operator!=(const boolRC&i)const { return *this != (bool)i; }
//  inline Logical operator<=(const boolRC&i)const { return *this <= (bool)i; }
//  inline Logical operator>=(const boolRC&i)const { return *this >= (bool)i; }
//*****************************************************************************
    inline Logical operator&&(const boolRC&i)const { return *this && (bool)i; }
    inline Logical operator||(const boolRC&i)const { return *this || (bool)i; }
    inline double operator+(const double&i) const
                                           { return (double)rcData->data + i; }
    inline double operator-(const double&i) const
                                           { return (double)rcData->data - i; }
    inline double operator*(const double&i) const
                                           { return (double)rcData->data * i; }
    inline double operator/(const double&i) const
                                           { return (double)rcData->data / i; }
    inline float  operator+(const float& i) const
                                           { return  (float)rcData->data + i; }
    inline float  operator-(const float& i) const
                                           { return  (float)rcData->data - i; }
    inline float  operator*(const float& i) const
                                           { return  (float)rcData->data * i; }
    inline float  operator/(const float& i) const
                                           { return  (float)rcData->data / i; }
  private:
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((boolRC&)x); }
    inline virtual void reference(const ReferenceCounting& x)
      { reference((boolRC&)x); }
    inline virtual ReferenceCounting* virtualConstructor
      (const CopyType ct = DEEP) const
      { return new boolRC(*this, ct); }
    RCData* rcData;
    inline void initialize()
      { rcData = new RCData; rcData->incrementReferenceCount(); }
};
#ifndef __DECCXX
inline bool operator+  (const bool& i, const boolRC& j) { return i + (bool)j; }
inline bool operator-  (const bool& i, const boolRC& j) { return i - (bool)j; }
inline bool operator*  (const bool& i, const boolRC& j) { return i * (bool)j; }
inline bool operator/  (const bool& i, const boolRC& j) { return i / (bool)j; }
inline bool operator%  (const bool& i, const boolRC& j) { return i % (bool)j; }
inline bool operator^  (const bool& i, const boolRC& j) { return i ^ (bool)j; }
inline bool operator&  (const bool& i, const boolRC& j) { return i & (bool)j; }
inline bool operator|  (const bool& i, const boolRC& j) { return i | (bool)j; }
inline bool operator<  (const bool& i, const boolRC& j) { return i < (bool)j; }
inline bool operator>  (const bool& i, const boolRC& j) { return i > (bool)j; }
inline bool operator<< (const bool& i, const boolRC& j) { return i<< (bool)j; }
inline bool operator>> (const bool& i, const boolRC& j) { return i>> (bool)j; }
inline Logical operator==(const bool&i, const boolRC&j) { return i== (bool)j; }
inline Logical operator!=(const bool&i, const boolRC&j) { return i!= (bool)j; }
inline Logical operator<=(const bool&i, const boolRC&j) { return i<= (bool)j; }
inline Logical operator>=(const bool&i, const boolRC&j) { return i>= (bool)j; }
#endif // __DECCXX
inline Logical operator&&(const bool&i, const boolRC&j) { return i&& (bool)j; }
inline Logical operator||(const bool&i, const boolRC&j) { return i|| (bool)j; }
inline double operator+(const double& i,const boolRC& j)
                                                { return i + (double)(bool)j; }
inline double operator-(const double& i,const boolRC& j)
                                                { return i - (double)(bool)j; }
inline double operator*(const double& i,const boolRC& j)
                                                { return i * (double)(bool)j; }
inline double operator/(const double& i,const boolRC& j)
                                                { return i / (double)(bool)j; }
inline float operator+(const float& i, const boolRC& j)
                                                { return i + (float) (bool)j; }
inline float operator-(const float& i, const boolRC& j)
                                                { return i - (float) (bool)j; }
inline float operator*(const float& i, const boolRC& j)
                                                { return i * (float) (bool)j; }
inline float operator/(const float& i, const boolRC& j)
                                                { return i / (float) (bool)j; }
inline ostream& operator<<(ostream& s, const boolRC& i)
  { s << (bool)i; return s; }
typedef boolRC LogicalRC;
#else
typedef IntegerRC LogicalRC;
#endif // defined GNU || defined __PHOTON || defined __DECCXX

#undef ReferenceCountedType

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceCountedType(TypeRC,Type)                                      \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() const { return  rcData->data; }                    \
    inline operator Type () const { return  rcData->data; }                    \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator+  (const Type& i) const { return rcData->data +   i; }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Type operator-  (const Type& i) const { return rcData->data -   i; }\
    inline Type operator*  (const Type& i) const { return rcData->data *   i; }\
    inline Type operator/  (const Type& i) const { return rcData->data /   i; }\
    inline Logical operator!(            ) const { return ! rcData->data;     }\
    inline Type operator<  (const Type& i) const { return rcData->data <   i; }\
    inline Type operator>  (const Type& i) const { return rcData->data >   i; }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Logical operator==(const Type&i)const { return rcData->data ==  i; }\
    inline Logical operator!=(const Type&i)const { return rcData->data !=  i; }\
    inline Logical operator<=(const Type&i)const { return rcData->data <=  i; }\
    inline Logical operator>=(const Type&i)const { return rcData->data >=  i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Logical& i) const                          \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Logical& i) const                          \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
    inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; }\
    inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; }\
    inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; }\
    inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; }\
    inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; }\
    inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; }\
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Logical operator==(const TypeRC&i)const { return *this == (Type)i; }\
    inline Logical operator!=(const TypeRC&i)const { return *this != (Type)i; }\
    inline Logical operator<=(const TypeRC&i)const { return *this <= (Type)i; }\
    inline Logical operator>=(const TypeRC&i)const { return *this >= (Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Type operator+  (const Type& i, const TypeRC& j) { return i + (Type)j; }\
inline Type operator-  (const Type& i, const TypeRC& j) { return i - (Type)j; }\
inline Type operator*  (const Type& i, const TypeRC& j) { return i * (Type)j; }\
inline Type operator/  (const Type& i, const TypeRC& j) { return i / (Type)j; }\
inline Type operator<  (const Type& i, const TypeRC& j) { return i < (Type)j; }\
inline Type operator>  (const Type& i, const TypeRC& j) { return i > (Type)j; }\
inline Type operator+= (      Type& i, const TypeRC& j) { return i+= (Type)j; }\
inline Type operator-= (      Type& i, const TypeRC& j) { return i-= (Type)j; }\
inline Type operator*= (      Type& i, const TypeRC& j) { return i*= (Type)j; }\
inline Type operator/= (      Type& i, const TypeRC& j) { return i/= (Type)j; }\
inline Logical operator==(const Type&i, const TypeRC&j) { return i== (Type)j; }\
inline Logical operator!=(const Type&i, const TypeRC&j) { return i!= (Type)j; }\
inline Logical operator<=(const Type&i, const TypeRC&j) { return i<= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeRC&j) { return i>= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeRC&j) { return i&& (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) { return i|| (Type)j; }\
inline Logical operator&&(const Logical& i, const TypeRC& j)                   \
                                                        { return i&& (Type)j; }\
inline Logical operator||(const Logical& i, const TypeRC& j)                   \
                                                        { return i|| (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }

#elif defined __DECCXX // for the brain-damaged DEC cxx compiler.
#define ReferenceCountedType(TypeRC,Type)                                      \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() { return rcData->data; }                           \
    inline operator const Type&() const { return rcData->data; }               \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Logical operator!(            ) const { return ! rcData->data; ;   }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Logical& i) const                          \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Logical& i) const                          \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
/********************************************************************************/ \
/*  inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; } */ \
/*  inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; } */ \
/*  inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; } */ \
/*  inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; } */ \
/*  inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; } */ \
/*  inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; } */ \
/********************************************************************************/ \
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Logical operator&&(const Type&i, const TypeRC&j) { return i&& (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) { return i|| (Type)j; }\
inline Logical operator&&(const Logical& i, const TypeRC& j)                   \
                                                        { return i&& (Type)j; }\
inline Logical operator||(const Logical& i, const TypeRC& j)                   \
                                                        { return i|| (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }

#else
#define ReferenceCountedType(TypeRC,Type)                                      \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() { return rcData->data; }                           \
    inline operator const Type&() const { return rcData->data; }               \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
    inline Type operator+  (             ) const { return + rcData->data;     }\
    inline Type operator+  (const Type& i) const { return rcData->data +   i; }\
    inline Type operator-  (             ) const { return - rcData->data;     }\
    inline Type operator-  (const Type& i) const { return rcData->data -   i; }\
    inline Type operator*  (const Type& i) const { return rcData->data *   i; }\
    inline Type operator/  (const Type& i) const { return rcData->data /   i; }\
    inline Logical operator!(            ) const { return ! rcData->data;     }\
    inline Type operator<  (const Type& i) const { return rcData->data <   i; }\
    inline Type operator>  (const Type& i) const { return rcData->data >   i; }\
    inline Type operator+= (const Type& i)       { return rcData->data +=  i; }\
    inline Type operator-= (const Type& i)       { return rcData->data -=  i; }\
    inline Type operator*= (const Type& i)       { return rcData->data *=  i; }\
    inline Type operator/= (const Type& i)       { return rcData->data /=  i; }\
    inline Logical operator==(const Type&i)const { return rcData->data ==  i; }\
    inline Logical operator!=(const Type&i)const { return rcData->data !=  i; }\
    inline Logical operator<=(const Type&i)const { return rcData->data <=  i; }\
    inline Logical operator>=(const Type&i)const { return rcData->data >=  i; }\
    inline Logical operator&&(const Type&i)const { return rcData->data &&  i; }\
    inline Logical operator||(const Type&i)const { return rcData->data ||  i; }\
    inline Logical operator&&(const Logical& i) const                          \
                                                 { return rcData->data &&  i; }\
    inline Logical operator||(const Logical& i) const                          \
                                                 { return rcData->data ||  i; }\
    inline Type operator++ (             )       { return ++ rcData->data;    }\
    inline Type operator++ (int          )       { return rcData->data ++;    }\
    inline Type operator-- (             )       { return -- rcData->data;    }\
    inline Type operator-- (int          )       { return rcData->data --;    }\
    inline Type operator+  (const TypeRC& i) const { return *this +  (Type)i; }\
    inline Type operator-  (const TypeRC& i) const { return *this -  (Type)i; }\
    inline Type operator*  (const TypeRC& i) const { return *this *  (Type)i; }\
    inline Type operator/  (const TypeRC& i) const { return *this /  (Type)i; }\
    inline Type operator<  (const TypeRC& i) const { return *this <  (Type)i; }\
    inline Type operator>  (const TypeRC& i) const { return *this >  (Type)i; }\
    inline Type operator+= (const TypeRC& i)       { return *this += (Type)i; }\
    inline Type operator-= (const TypeRC& i)       { return *this -= (Type)i; }\
    inline Type operator*= (const TypeRC& i)       { return *this *= (Type)i; }\
    inline Type operator/= (const TypeRC& i)       { return *this /= (Type)i; }\
    inline Logical operator==(const TypeRC&i)const { return *this == (Type)i; }\
    inline Logical operator!=(const TypeRC&i)const { return *this != (Type)i; }\
    inline Logical operator<=(const TypeRC&i)const { return *this <= (Type)i; }\
    inline Logical operator>=(const TypeRC&i)const { return *this >= (Type)i; }\
    inline Logical operator&&(const TypeRC&i)const { return *this && (Type)i; }\
    inline Logical operator||(const TypeRC&i)const { return *this || (Type)i; }\
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};                                                                             \
inline Type operator+  (const Type& i, const TypeRC& j) { return i + (Type)j; }\
inline Type operator-  (const Type& i, const TypeRC& j) { return i - (Type)j; }\
inline Type operator*  (const Type& i, const TypeRC& j) { return i * (Type)j; }\
inline Type operator/  (const Type& i, const TypeRC& j) { return i / (Type)j; }\
inline Type operator<  (const Type& i, const TypeRC& j) { return i < (Type)j; }\
inline Type operator>  (const Type& i, const TypeRC& j) { return i > (Type)j; }\
inline Type operator+= (      Type& i, const TypeRC& j) { return i+= (Type)j; }\
inline Type operator-= (      Type& i, const TypeRC& j) { return i-= (Type)j; }\
inline Type operator*= (      Type& i, const TypeRC& j) { return i*= (Type)j; }\
inline Type operator/= (      Type& i, const TypeRC& j) { return i/= (Type)j; }\
inline Logical operator==(const Type&i, const TypeRC&j) { return i== (Type)j; }\
inline Logical operator!=(const Type&i, const TypeRC&j) { return i!= (Type)j; }\
inline Logical operator<=(const Type&i, const TypeRC&j) { return i<= (Type)j; }\
inline Logical operator>=(const Type&i, const TypeRC&j) { return i>= (Type)j; }\
inline Logical operator&&(const Type&i, const TypeRC&j) { return i&& (Type)j; }\
inline Logical operator||(const Type&i, const TypeRC&j) { return i|| (Type)j; }\
inline Logical operator&&(const Logical& i, const TypeRC& j)                   \
                                                        { return i&& (Type)j; }\
inline Logical operator||(const Logical& i, const TypeRC& j)                   \
                                                        { return i|| (Type)j; }\
inline ostream& operator<<(ostream& s, const TypeRC& i)                        \
  { s << (Type)i; return s; }
#endif

ReferenceCountedType(floatRC,      float)       // Declare class floatRC
ReferenceCountedType(doubleRC,     double)      // Declare class doubleRC
ReferenceCountedType(longDoubleRC, long double) // Declare class longDoubleRC

#ifdef DOUBLE
typedef doubleRC     RealRC;
#if defined __DECCXX
typedef longDoubleRC DoubleRealRC;
#endif // defined __DECCXX
#else // ifndef DOUBLE
typedef floatRC      RealRC;
#if defined __DECCXX
typedef doubleRC     DoubleRealRC;
#endif // defined __DECCXX
#endif // DOUBLE

#undef ReferenceCountedType

#if defined __sun && ! defined __SVR4 // for the brain-damaged SunOS compiler.
#define ReferenceCountedType(TypeRC,Type)                                      \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() const { return  rcData->data; }                    \
    inline operator Type () const { return  rcData->data; }                    \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};

#else
#define ReferenceCountedType(TypeRC,Type)                                      \
class TypeRC:                                                                  \
  public ReferenceCounting {                                                   \
  public:                                                                      \
    class RCData:                                                              \
      public ReferenceCounting {                                               \
      public:                                                                  \
        inline RCData(): ReferenceCounting() { }                               \
        inline RCData(const RCData& x): ReferenceCounting(x) { }               \
        inline virtual ~RCData() { }                                           \
        Type data;                                                             \
    };                                                                         \
    inline TypeRC& operator=(const TypeRC& x)                                  \
      { rcData->data = x.rcData->data; return *this; }                         \
    inline virtual ~TypeRC()                                                   \
      { if (rcData->decrementReferenceCount() == 0) delete rcData; }           \
    inline TypeRC() { initialize(); }                                          \
    inline TypeRC& operator=(const Type& x)                                    \
      { rcData->data = x; return *this; }                                      \
    inline TypeRC(const TypeRC& x, const CopyType ct = DEEP) {                 \
        if (ct == DEEP) {                                                      \
	    initialize();                                                      \
	    *this = x;                                                         \
        } else if (ct == SHALLOW) {                                            \
            rcData = x.rcData; rcData->incrementReferenceCount();              \
            reference(x);                                                      \
        } else /* if (ct == NOCOPY) */ {                                       \
	    initialize();                                                      \
        }                                                                      \
    }                                                                          \
    inline TypeRC(const Type& x) { initialize(); *this = x; }                  \
    inline operator Type&() { return rcData->data; }                           \
    inline operator const Type&() const { return rcData->data; }               \
    inline Type* operator&() { return &rcData->data; }                         \
    inline void reference(const TypeRC& x) {                                   \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = x.rcData; rcData->incrementReferenceCount();                  \
    }                                                                          \
    inline void reference(TypeRC::RCData& x) {                                 \
        if (rcData->decrementReferenceCount() == 0) delete rcData;             \
        rcData = &x;                                                           \
        if (!x.uncountedReferencesMayExist())                                  \
          rcData->incrementReferenceCount();                                   \
    }                                                                          \
    inline virtual void breakReference() {                                     \
        if (rcData->getReferenceCount() != 1) {                                \
            TypeRC x = *this;                                                  \
            reference(x);                                                      \
        }                                                                      \
    };                                                                         \
    inline virtual void consistencyCheck() const {                             \
        if (rcData == NULL) {                                                  \
            cerr << "ReferenceCountedType::consistencyCheck():  "              \
                 << "rcData == NULL for "                                      \
                 << getClassName() << " " << getGlobalID() << "." << endl;     \
            exit(1);                                                           \
        }                                                                      \
        rcData->consistencyCheck();                                            \
    }                                                                          \
  private:                                                                     \
    inline virtual ReferenceCounting& operator=(const ReferenceCounting& x)    \
      { return operator=((TypeRC&)x); }                                        \
    inline virtual void reference(const ReferenceCounting& x)                  \
      { reference((TypeRC&)x); }                                               \
    inline virtual ReferenceCounting* virtualConstructor                       \
      (const CopyType ct = DEEP) const                                         \
      { return new TypeRC(*this, ct); }                                        \
    RCData* rcData;                                                            \
    inline void initialize()                                                   \
      { rcData = new RCData; rcData->incrementReferenceCount(); }              \
};
#endif

#endif // _ReferenceCountedTypes
