/*
** This software is copyright (C) by the Lawrence Berkeley National
** Laboratory.  Permission is granted to reproduce this software for
** non-commercial purposes provided that this notice is left intact.
**  
** It is acknowledged that the U.S. Government has rights to this
** software under Contract DE-AC03-765F00098 between the U.S.  Department
** of Energy and the University of California.
**  
** This software is provided as a professional and academic contribution
** for joint exchange. Thus it is experimental, is provided ``as is'',
** with no warranties of any kind whatsoever, no support, no promise of
** updates, or printed documentation. By using this software, you
** acknowledge that the Lawrence Berkeley National Laboratory and Regents
** of the University of California shall have no liability with respect
** to the infringement of other copyrights by any part of this software.
**  
** For further information about this software, contact:
** 
**         Dr. John Bell
**         Bldg. 50D, Rm. 129,
**         Lawrence Berkeley National Laboratory
**         Berkeley, CA, 94720
**         jbbell@lbl.gov
*/

// kkc 040415 
// kkc This class has been heavily  modified so that it now inherits from
//     std::string in order to reduce Overture's dependence on boxlib.
//     Most of the ManDoc discussion about the behavior, particularly
//     the reference counted memory management, no longer applies.

#ifndef _ASTRING_H_
#define _ASTRING_H_

//
// $Id: aString.H,v 1.17 2006/07/07 22:42:56 chand Exp $
//

#include "OvertureDefine.h"

#ifndef OV_USE_OLD_STL_HEADERS

#include <iostream>
#include <cstring> 
#include <string>
//OV_USINGNAMESPACE(std);
#else
#include <iostream.h>
#include <string>
#endif

// *wdh* These next two functions are added to allow the conversion from aString to std::string

std::string
substring(const std::string & s, const int startPosition, const int endPosition );

int 
matches(const std::string & s,  const char *name );

//@Man:
//@Memo: A String Class
/*@Doc:

  The class aString is used to store and manipulate character strings.  It
  has an efficient underlying storage mechanism and some useful string
  manipulation operations.

  The aString class is implemented using a character array and reference
  count. Two aStrings may reference the same underlying character array with
  a reference count of two.  When an aString copy constructor or copy
  operator is applied the reference count on the underlying character array
  is incremented but the actual string is not copied.  That is, copying an
  aString is an inexpensive operation.  When an aString is destructed, the
  reference count is decremented.  The underlying character array is deleted
  only when the reference count goes to zero.  Any operator that modifies an
  aString will make its own copy of the character array before the
  modification, unless it's the sole owner of the character array in the
  aString.
*/

class aString : public std::string
{
public:
    //
    //@ManDoc: Constructs an empty string.
    //
    aString ();

    /*@ManDoc: Constructs an aString containing the single character c.
               If c is the null character then this will be the empty string.
    */
      aString (char c);

    /*@ManDoc: Constructs an empty aString but allocates enough space to hold
               a character string of length len.  This may be useful when
               reading in very long lines with the `getline' function; i.e.
               it may be slightly more efficient to allocate a very large
               aString once and then use `getline' than to use `getline'
               on an empty string.  In general though, you won't notice any
               difference and you really shouldn't use this constructor.
    */
      aString (int len);

    /*@ManDoc: Constructs an aString initialized to the character string s.
               It is an error is `s' is the null string.
    */
      aString (const char* s);
   //
    //@ManDoc: The copy constructor.
    //
      aString (const aString& rhs);

    //
    //@ManDoc: Returns true if this is the null string.
    //
    bool isNull () const;

    /*@ManDoc: Returns a reference to the kth character in the string.  An
               error occurs if k < 0 or k >= length().
    */
    char& operator [] (int k);

    /*@ManDoc: Returns kth character in the string.  An error occurs
               if k < 0 or k >= length().
    */
    char operator[] (int k) const;

#if 0
    //
    //@ManDoc: The assignment operator
    //
    aString& operator= (const aString& rhs);

    /*@ManDoc: Catenate aString right onto end of this aString.  Return a
               reference to self to allow for operator chaining.
    */
    aString& operator+= (const aString& right);

    /*@ManDoc: Catenate character string right onto end of this aString.
               Returns a reference to self to allow for operator chaining.
               It is an error is `right' is the null string.
    */
    aString& operator+= (const char* right);

    /*@ManDoc: Catenate character c onto end of this aString.
               Returns a reference to self to allow for operator chaining.
               This does nothing if c is the null character.
    */
    aString& operator+= (char c);

    /*@ManDoc: Converts all characters in this aString to upper case.
               Returns a reference to self to allow for operator chaining.
    */
  //    aString& toUpper ();

    /*@ManDoc: Converts all characters in this aString to lower case.
               Returns a reference to self to allow for operator chaining.
    */
  //    aString& toLower ();

    /*@ManDoc: Read the next line from the input stream strm and store it
               as the value of the string.  The delimiter for a line of text
               is the newline character.  The newline is extracted from the
               istream, but it is NOT stored in the aString.  There is no
               limit to the length of string that can be extracted.
    */
    istream& getline (istream& strm);

    /*@ManDoc: Returns the number of characters stored in this aString.
               This does not include the terminating null character.
    */
    int length () const;


    /*@ManDoc: Returns a reference to the kth character in the string.  An
               error occurs if k < 0 or k >= length().
    */
    char& operator [] (int k);

    /*@ManDoc: Returns kth character in the string.  An error occurs
               if k < 0 or k >= length().
    */
    char operator[] (int k) const;

    /*@ManDoc: Convert an aString to a const char *.  This allows an aString
               to be used in any context where a const char* type is needed.
    */
    const char* c_str () const;
    //
    //@ManDoc: Write to an ostream in ASCII format.
    //
    friend ostream& operator<< (ostream&       os,
                                const aString& str);

    /*@ManDoc: Read a whitespace delimited string from an istream.
               This function discards leading whitespace and then reads
               in non-whitespace character until the next whitespace
               character or end-of-file.  Note that there is no limit,
               on the length of the character that can be read in, except
               that dictated by the resources of the machine.
               Note  also that operator>> and operator<< are not completely
               symmetrical in the case where operator<< writes out a
               string that contains whitespace.  If you're trying to
               read in a string that contains whitespace, you might
               want to use getline() instead.
    */
    friend istream& operator>> (istream& is,
                                aString& str);

#endif

// *wdh
 operator const char *() const {return this->c_str(); }
 bool operator==( const char c ) { return *this==aString(c); }
 aString operator()( const int i1, const int i2 ) const;  // substring from i1...i2
 int matches( const char * name ) const;   // return strlen(name) if name agress with the first chars of this.
    // kkc 
  aString (const std::string s) { *this = s.c_str(); }

// *wdh

protected:
    void copyModify ();

private:
    //
    // None of the following functions need to be friends.  I've made
    // them friends solely so they'll show up nicely in the HTML documentaion
    // spewed out by doc++.
    //
#if 0
    //
    // Is left lexically less than right?
    //
    friend inline bool operator<  (const aString& left,
                                   const aString& right);
    //
    // Is left lexically less than or equal to right?
    //
    friend inline bool operator<= (const aString& left,
                                   const aString& right);
    //
    // Is left not equal to right?
    //
    friend inline bool operator!= (const aString& left,
                                   const aString& right);
    //
    // Is left equal to right?
    //
    friend inline bool operator== (const aString& left,
                                   const aString& right);
    //
    // Is left lexically greater than or equal to right?
    //
    friend inline bool operator>= (const aString& left,
                                   const aString& right);
    //
    // Is left lexically greater than right?
    //
    friend inline bool operator>  (const aString& left,
                                   const aString& right);
    //
    // Is left lexically less than right?
    //
    friend inline bool operator<  (const aString& left,
                                   const char*    right);
    //
    // Is left lexically less than or equal to right?
    //
    friend inline bool operator<= (const aString& left,
                                   const char*    right);
    //
    // Is left not equal to right?
    //
    friend inline bool operator!= (const aString& left,
                                   const char*    right);
    //
    // Is left equal to right?
    //
    friend inline bool operator== (const aString& left,
                                   const char*    right);
    //
    // Is left lexically greater than or equal to right?
    //
    friend inline bool operator>= (const aString& left,
                                   const char*    right);
    //
    // Is left lexically greater than right?
    //
    friend inline bool operator>  (const aString& left,
                                   const char*    right);
    //
    // Is left lexically less than right?
    //
    friend inline bool operator<  (const char*    left,
                                   const aString& right);
    //
    // Is left lexically less than or equal to right?
    //
    friend inline bool operator<= (const char*    left,
                                   const aString& right);
    //
    // Is left not equal to right?
    //
    friend inline bool operator!= (const char*    left,
                                   const aString& right);
    //
    // Is left equal to right?
    //
    friend inline bool operator== (const char*    left,
                                   const aString& right);
    //
    // Is left lexically greater than or equal to right?
    //
    friend inline bool operator>= (const char*    left,
                                   const aString& right);
    //
    // Is left lexically greater than right?
    //
    friend inline bool operator>  (const char*    left,
                                   const aString& right);
#endif
};
//
// aString inlines.
//

inline
bool
aString::isNull () const
{
    return ((std::string *)this)->length() == 0;
}

inline
aString
operator+ (const aString& left,
           const aString& right)
{
    aString result(left);
    return result += right;
}

inline
aString
operator+ (const char *left,
           const aString& right)
{
    aString result(left);
    return result += right;
}

inline
aString
operator+ (const aString& left,
           const char * right)
{
    aString result(left);
    return result += right;
}

inline
aString
operator+ (const std::string& left,
           const aString& right)
{
    aString result(left);
    return result += right;
}

#if 0
inline
int
aString::length () const
{
    return ((std::string *)this)->length();
}

inline
const char*
aString::c_str () const
{
    return std::string::c_str();
}
#endif

inline
char
aString::operator[] (int index) const
{
    return ((std::string &)(*this))[index];
}

#if 0
inline
char
aString::operator[] (int index) const
{
    return >s[index];
}


inline
bool
operator< (const aString& left,
           const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) < 0;
}

inline
bool
operator<= (const aString& left,
            const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) <= 0;
}

inline
bool
operator!= (const aString& left,
            const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) != 0;
}

inline
bool
operator== (const aString& left,
            const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) == 0;
}

inline
bool
operator>= (const aString& left,
            const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) >= 0;
}

inline
bool
operator>  (const aString& left,
            const aString& right)
{
    return ::strcmp(left.c_str(), right.c_str()) > 0;
}

inline
bool
operator< (const aString& left,
           const char*    right)
{
    return ::strcmp(left.c_str(), right) < 0;
}

inline
bool
operator<= (const aString& left,
            const char*    right)
{
    return ::strcmp(left.c_str(), right) <= 0;
}

inline
bool
operator!= (const aString& left,
            const char*    right)
{
    return ::strcmp(left.c_str(), right) != 0;
}

inline
bool
operator== (const aString& left,
            const char*    right)
{
    return ::strcmp(left.c_str(), right) == 0;
}

inline
bool
operator>= (const aString& left,
            const char*    right)
{
    return ::strcmp(left.c_str(), right) >= 0;
}

inline
bool
operator>  (const aString& left,
            const char*    right)
{
    return ::strcmp(left.c_str(), right) > 0;
}

inline
bool
operator< (const char*    left,
           const aString& right)
{
    return ::strcmp(left, right.c_str()) < 0;
}

inline
bool
operator<= (const char*    left,
            const aString& right)
{
    return ::strcmp(left, right.c_str()) <= 0;
}

inline
bool
operator!= (const char*    left,
            const aString& right)
{
    return ::strcmp(left, right.c_str()) != 0;
}

inline
bool
operator== (const char*    left,
            const aString& right)
{
    return ::strcmp(left, right.c_str()) == 0;
}

inline
bool
operator>= (const char*    left,
            const aString& right)
{
    return ::strcmp(left, right.c_str()) >= 0;
}

inline
bool
operator>  (const char*    left,
            const aString& right)
{
    return ::strcmp(left, right.c_str()) > 0;
}
#endif

#endif /*_ASTRING_H_*/
