#include <string.h>
#include <ctype.h>

#include <Assert.H>
#include "aString.H"

aString::StringRep::StringRep(int _len)
{
    bufferlength = _len;
    s = new char [bufferlength];
    assert(s!=0);
}

aString::StringRep::~StringRep()
{
    delete [] s;
    s = 0;
}

void
aString::StringRep::resize(int n)
{
    if(n > bufferlength) {
	char *ns = new char [n];
	::memcpy(ns,s,bufferlength);
	bufferlength = n;
	delete [] s;
	s = ns;
    }
}

aString::aString()
    : p(new StringRep), len(0)
{
    assert(p != 0);
    p->increment();
// wdh    p->s = 0;   // this line causes in a memory leak
}

aString::aString(char c)
    : p(new StringRep(2)), len(1)
{
    assert(p != 0);
    p->increment();
    p->s[0] = c;
    p->s[1] = 0;
}

aString::aString(int size)
    : p(new StringRep(size)), len(0)
{
    assert(p != 0);
    p->increment();
    ::memset(p->s,'\0',p->bufferlength);
}

aString::aString(const char *initialtext)
{
    len = ::strlen(initialtext);
    p = new StringRep(len + 1);
    assert(p != 0);
    p->increment();
    ::strcpy(p->s,initialtext);
}

aString::aString(const aString & initialstring)
    : p(initialstring.p), len(initialstring.len)
{
    p->increment();
}


aString::~aString()
{
    p->decrement();
    if(p->useCount() == 0) {
	delete p;
    }
    p = 0;
    len = 0;
}

aString &
aString::operator = (const char *s)
{
    if( p==0 ) // wdh
    {
      p = new StringRep;
      assert( p!=0 );
      len=0;
    } 
    len = ::strlen(s);
    if(p->useCount() > 1) {
	p->decrement();
	p = new StringRep(len + 1);
	p->increment();
    } else {
	p->resize(len + 1);
    }
    ::strcpy(p->s,s);
    return *this;
}

aString &
aString::operator = (const aString & right)
{
    if( p==0 ) // wdh
    {
      p = new StringRep;
      assert( p!=0 );
      len=0;
    } 
    right.p->increment();
    p->decrement();
    if(p->useCount() == 0) {
	delete p;
    }
    p = right.p;
    len = right.len;
    return *this;
}

aString &
aString::operator += (const aString & val)
{
    int clen = length() + val.length();
    StringRep *np = new StringRep(clen+1);
    np->increment();
    ::strcpy(np->s,p->s);
    ::strcat(np->s,val.p->s);
    p->decrement();
    if(p->useCount() == 0) {
	delete p;
    }
    p = np;
    len = clen;
    return *this;
}

aString &
aString::operator += (const char *s)
{
    int clen = length() + ::strlen(s);

    if(p->useCount() > 1) {
	p->decrement();
	StringRep *np = new StringRep(clen+1);
	np->increment();
	::strcpy(np->s, p->s);
	p = np;
    } else {
	p->resize(clen + 1);
    }
    ::strcat(p->s,s);
    len = clen;
    return *this;
}

istream &
aString::getline(istream & in)
{
    in.getline(p->s, p->bufferlength, '\n');
    return in;
}

const char &
aString::operator [](int index) const
{
    assert(index >=0 && index < len);
    return p->s[index];
}

char &
aString::operator [] (int index)
{
    assert(index >=0 && index < len);
    if(p->useCount() > 1) {
	StringRep *np = new StringRep(::strlen(p->s)+1);
	::strcpy(np->s,p->s);
	p->decrement();
	p = np;
	p -> increment();
    }
    return p->s[index];
}

int
aString::compare(const aString & val) const
{
    return ::strcmp(p->s,val.p->s);
}

aString::operator const char *() const
{
    return p->s;
}

/* --- wdh : this just generates leaks
aString::operator char *() const
{
    char *pp = new char[1+length()];
    ::strcpy(pp,p->s);
    return pp;
}
--- */

int
operator < (const aString & left, const aString & right)
{
    return left.compare(right) < 0;
}

int
operator <= (const aString & left, const aString & right)
{
    return left.compare(right) <= 0;
}

int
operator != (const aString & left, const aString & right)
{
    return left.compare(right) != 0;
}

int
operator == (const aString & left, const aString & right)
{
    return left.compare(right) == 0;
}

int
operator >= (const aString & left, const aString & right)
{
    return left.compare(right) >= 0;
}

int
operator >  (const aString & left, const aString & right)
{
    return left.compare(right) > 0;
}

aString
operator + (const aString & left, const aString & right)
{
    aString result(left);
    result += right;
    return result;
}

aString&
aString::toUpper()
{
    for (char *pp = p->s; pp != 0; pp++)
	*pp = ::toupper(*pp);
    return *this;
}

aString&
aString::toLower()
{
    for (char *pp = p->s; pp != 0; pp++)
	*pp = ::tolower(*pp);
    return *this;
}

istream &
operator >> (istream & in, aString & str)
{
    char inbuffer[1000];

    if (in >> inbuffer)
	str = inbuffer;
    else
	str = "";

    return in;
}

ostream &
operator << (ostream & out, const aString & str)
{
    out.write(&str[0], str.len);
    return out;
}

// wdh:
int
operator < (const aString & left, const char * right)
{
    return left.compare(right) < 0;
}

int
operator <= (const aString & left, const char * right)
{
    return left.compare(right) <= 0;
}

int
operator != (const aString & left, const char * right)
{
    return left.compare(right) != 0;
}

int
operator == (const aString & left, const char * right)
{
    return left.compare(right) == 0;
}

int
operator >= (const aString & left, const char * right)
{
    return left.compare(right) >= 0;
}

int
operator >  (const aString & left, const char * right)
{
    return left.compare(right) > 0;
}

int
operator < (const char * left, const aString & right)
{
    return right.compare(left) > 0;
}

int
operator <= (const char * left, const aString & right)
{
    return right.compare(left) >= 0;
}

int
operator != (const char * left, const aString & right)
{
    return right.compare(left) != 0;
}

int
operator == (const char * left, const aString & right)
{
    return right.compare(left) == 0;
}

int
operator >= (const char * left, const aString & right)
{
    return right.compare(left) <= 0;
}

int
operator >  (const char * left, const aString & right)
{
    return right.compare(left) < 0;
}

//-----------------------------------------------------------------------
//  return a substring
// substring from i1...i2
//----------------------------------------------------------------------
aString aString::operator()( const int i1, const int i2 )
{
    
  int ia= i1<0 ? 0 : i1;
  int ib= i2>length() ? length() : i2;
     
  char *c = new char[ib-ia+2];
  for( int i=ia; i<=ib; i++ )
    c[i-ia]=p->s[i];
  c[ib-ia+1]='\0';
  aString sub=c;
  delete c;
  return sub;
}    
