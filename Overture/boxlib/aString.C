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

//
// $Id: aString.C,v 1.9 2005/10/29 17:20:06 henshaw Exp $
//

#include <ctype.h>

#ifdef NO_APP
#ifndef boxAssert
#define boxAssert assert
#endif
#else
#include <BL_Assert.H>
#endif

#include <aString.H>



std::string
substring(const std::string & s, const int startPosition, const int endPosition )
// ====================================================================================
//   Return a substring -- replacement for aString(i1,i2)
// ====================================================================================
{
  return s.substr(startPosition,endPosition-startPosition+1);
}    


int
matches(const std::string & s,  const char *name ) 
// ================================================================================
// If all the characters of name match the characters of this string then
// return the number of characters of name, otherwise return zero.
// ==================================================================================
{
  int lenName=strlen(name);
  if( s.substr(0,lenName)==name )
    return lenName;
  else
    return 0;
}





#if 0
void
StringRep::resize (int n)
{
    if (n > bufferlength)
    {
        char* ns = new char[n];
        if (ns == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        ::memcpy(ns,s,bufferlength);
        bufferlength = n;
        if (s!=NULL) delete [] s;
        s = ns;
    }
}

aString&
aString::operator= (const aString& rhs)
{
    p   = rhs.p;
    len = rhs.len;
    return *this;
}

aString&
aString::operator+= (const aString& val)
{
    copyModify();
    int clen = length() + val.length();
    p->resize(clen+1);
    ::memcpy(&(p->s[len]),val.p->s, val.length()+1);
    len = clen;
    return *this;
}

aString&
aString::operator+= (const char* s)
{
    boxAssert(s != 0);
    copyModify();
    int slen = ::strlen(s);
    int clen = length() + slen;
    p->resize(clen+1);
    ::memcpy(&(p->s[len]),s, slen+1);
    len = clen;
    return *this;
}

aString&
aString::operator+= (char c)
{
    if (!(c == '\0'))
    {
        copyModify();
        p->resize(len+2);
        p->s[len++] = c;
        p->s[len]   = 0;
    }
    return *this;
}

char&
aString::operator[] (int index)
{
    boxAssert(index >= 0 && index < len);
    copyModify();
    return p->s[index];
}

void
aString::copyModify ()
{
    if (!p.unique())
    {
        StringRep* np = new StringRep(len+1);
        if (np == 0)
            BoxLib::OutOfMemory(__FILE__, __LINE__);
        ::memcpy(np->s,p->s,len+1);
        p = np;
    }
}

aString&
aString::toUpper ()
{
    copyModify();
    for (char *pp = p->s; pp != 0; pp++)
        *pp = toupper(*pp);
    return *this;
}

aString&
aString::toLower ()
{
    copyModify();
    for (char *pp = p->s; pp != 0; pp++)
        *pp = tolower(*pp);
    return *this;
}

istream&
operator>> (istream& is,
            aString& str)
{
    const int BufferSize = 128;
    char buf[BufferSize + 1];
    int index = 0;
    //
    // Nullify str.
    //
    str = "";
    //
    // Eat leading whitespace.
    //
    char c;
    do { is.get(c); } while (is.good() && isspace(c));
    buf[index++] = c;
    //
    // Read until next whitespace.
    //
    while (is.get(c) && !isspace(c))
    {
        buf[index++] = c;
        if (index == BufferSize)
        {
            buf[BufferSize] = 0;
            str += buf;
            index = 0;
        }
    }
    is.putback(c);
    buf[index] = 0;
    str += buf;
    if (is.fail())
        BoxLib::Abort("operator>>(istream&,aString&) failed");
    return is;
}

ostream&
operator<< (ostream&       out,
            const aString& str)
{
    out.write(str.c_str(), str.len);
    if (out.fail())
        BoxLib::Error("operator<<(ostream&,aString&) failed");
    return out;
}

istream&
aString::getline (istream& is)
{
    char      c;
    const int BufferSize = 256;
    char      buf[BufferSize + 1];
    int       index = 0;

    *this = "";

    //
    // Get those characters.
    //
    while (is.get(c) && c != '\n')
    {
        buf[index++] = c;
        if (index == BufferSize)
        {
            buf[BufferSize] = 0;
            *this += buf;
            index = 0;
        }
    }
    is.putback(c);
    buf[index] = 0;
    *this += buf;

    if (is.fail())
        BoxLib::Abort("aString::getline(istream&) failed");

    return is;
}
#endif

aString::aString() : std::string() { }
aString::aString (char c) : std::string()
{
  char s[2];
  s[0] = c;
  s[1] = '\0';
  *this = s;
}

aString::aString (int size)
{
  // XXX equivalent in std::string ?
  char *buf = new char[size];
  for ( int i=0; i<size; i++ )
    buf[i] = ' ';
  *this = buf;
  delete [] buf;
}

aString::aString (const char* initialtext) : std::string(initialtext)
{
}

aString::aString (const aString& initialstring)
  : std::string( (const std::string &)initialstring)
{
}

char&
aString::operator[] (int index)
{
    return (*((std::string *)this))[index];
}


//-----------------------------------------------------------------------
// *wdh* added
//  return a substring
// substring from i1...i2
//----------------------------------------------------------------------
aString aString::
operator()( const int i1, const int i2 ) const
{
    
  return substr(i1,i2-i1+1);
}    



int aString::
matches( const char * name ) const
// *wdh* added
// If all the characters of name match the characters of this string then
// return the number of characters of name, otherwise return zero.
{
  int lenName = strlen(name);
  if( (*this)(0,lenName-1)==name )
    return lenName;
  else
    return 0;
}
