//kkc 081124 #include <iostream.h>
#include <iostream>
#include "Fraction.h"
#include "GenericDataBase.h"

int Fraction::
get( const GenericDataBase & dir, const aString & name)
// get a Fraction from the database
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Fraction");

  subDir->get( numerator,"numerator" );
  subDir->get( denominator,"denominator" );

  delete subDir;
  return 0; 
}

int Fraction::
put( GenericDataBase & dir, const aString & name) const
// put a Fraction to a database
{
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Fraction");                      // create a sub-directory 
  subDir->put( numerator,"numerator" );
  subDir->put( denominator,"denominator" );

  delete subDir;
  return 0;
}


//================================================================
//   Define the arithmetic and relational operators for Class Fraction
//===============================================================

Fraction Fraction::operator - ( ) const
{ // define negative operator
  return Fraction( -numerator,denominator );
}

Fraction Fraction::operator + ( const Fraction & f1 ) const 
{ // should remove the GCD from the numerator and denominator
  if( denominator == f1.denominator )
    return Fraction( numerator+f1.numerator,denominator );
  else if( denominator !=0 && f1.denominator != 0 )
    return Fraction( numerator*f1.denominator+denominator*f1.numerator, 
                     denominator*f1.denominator );
  else if( denominator == 0 )
    return *this ;
  else
    return f1;
}    
Fraction Fraction::operator - ( const Fraction & f1 ) const 
{ // should remove the GCD from the numerator and denominator
  if( denominator == f1.denominator )
    return Fraction( numerator-f1.numerator,denominator );
  else if( denominator !=0 && f1.denominator != 0 )
    return Fraction( numerator*f1.denominator-denominator*f1.numerator, 
                     denominator*f1.denominator );
  else if( denominator == 0 )
    return *this ;
  else
    return -f1;
}    
Fraction Fraction::operator * ( const Fraction & f1 ) const 
{ // should remove the GCD from the numerator and denominator
  return Fraction( numerator*f1.numerator, denominator*f1.denominator );
}    
Fraction Fraction::operator / ( const Fraction & f1 ) const 
{ // should remove the GCD from the numerator and denominator
  if( denominator == f1.denominator )
    return Fraction( numerator,f1.numerator );
  else
  return Fraction( numerator*f1.denominator, denominator*f1.numerator );
}    

int Fraction::operator<= ( const Fraction & f1 ) const
{ if( denominator == f1.denominator )
    return numerator <= f1.numerator;
  else
   return numerator*f1.denominator <= denominator*f1.numerator;
}    
int Fraction::operator< ( const Fraction & f1 ) const
{ if( denominator == f1.denominator )
    return numerator < f1.numerator;
  else
   return numerator*f1.denominator < denominator*f1.numerator;
}    
int Fraction::operator>= ( const Fraction & f1 ) const
{ if( denominator == f1.denominator )
    return numerator >= f1.numerator;
  else
   return numerator*f1.denominator >= denominator*f1.numerator;
}    
int Fraction::operator > ( const Fraction & f1 ) const
{ if( denominator == f1.denominator )
    return numerator > f1.numerator;
  else
   return numerator*f1.denominator > denominator*f1.numerator;
}    

int Fraction::operator==( const Fraction & f1 ) const
{ if( denominator == f1.denominator )
    return numerator == f1.numerator;
  else
   return numerator*f1.denominator == denominator*f1.numerator;
}    

int Fraction::operator<= ( const real x ) const
{ return numerator <= denominator*x;
}    

int Fraction::operator <  ( const real x ) const
{ return numerator < denominator*x  ;
}    

int Fraction::operator >= ( const real x ) const
{ return numerator >= denominator*x ;
}    

int Fraction::operator >  ( const real x ) const
{ return numerator > denominator*x ;
}    

int Fraction::operator == ( const real x ) const
{ return numerator == denominator*x;
}    


// ---Define the friend operators for Class fraction----

int operator<=( const real x0, const Fraction f0 )
{
  return x0*f0.denominator <= f0.numerator;
}
int operator < ( const real x0, const Fraction f0 )
{
  return x0*f0.denominator  <  f0.numerator;
}
int operator >=( const real x0, const Fraction f0 )
{
  return x0*f0.denominator  >= f0.numerator;
}
int operator > ( const real x0, const Fraction f0 )
{
  return x0*f0.denominator  >  f0.numerator;
}
int operator ==( const real x0, const Fraction f0 )
{
  return x0*f0.denominator  == f0.numerator;
}

real operator + ( const real x0, const Fraction f0 )
{
  if( f0.denominator !=0 )
    return (x0*f0.denominator+f0.numerator)/f0.denominator;
  else
  { cerr << "Fraction::Error real+fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator + ( const Fraction f0, const real x0 )
{
  if( f0.denominator !=0 )
    return (x0*f0.denominator+f0.numerator)/f0.denominator;
  else
  { cerr << "Fraction::Error real+fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator - ( const real x0, const Fraction f0 )
{
  if( f0.denominator !=0 )
    return (x0*f0.denominator-f0.numerator)/f0.denominator;
  else
  { cerr << "Fraction::Error real-fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator - ( const Fraction f0, const real x0 )
{
  if( f0.denominator !=0 )
    return (f0.numerator-x0*f0.denominator)/f0.denominator;
  else
  { cerr << "Fraction::Error real-fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator * ( const real x0, const Fraction f0 )
{
  if( f0.denominator !=0 )
    return x0*f0.numerator/f0.denominator;
  else
  { cerr << "Fraction::Error real*fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator * ( const Fraction f0, const real x0 )
{
  if( f0.denominator !=0 )
    return x0*f0.numerator/f0.denominator;
  else
  { cerr << "Fraction::Error real*fraction, but denominator=0! " << endl;
    return 0.;
  }  
}
real operator / ( const real x0, const Fraction f0 )
{
  if( f0.numerator !=0 )
    return x0*f0.denominator/f0.numerator;
  else
  { cerr << "Fraction::Error real/fraction, but numerator=0! " << endl;
    return 0.;
  }  
}
real operator / ( const Fraction f0, const real x0 )
{
  if( f0.denominator !=0 && x0 != 0. )
    return f0.numerator/(f0.denominator*x0);
  else
  { cerr << "Fraction::Error fraction/real, but divide by 0. " << endl;
    return 0.;
  }  
}


/* this was a bad idea
Fraction::operator real ( )
{ // conversion routine from fraction to real
  if( denominator==0 )then
  {
    cerr << "Fraction::Error: conversion to real, denominator=0" << endl;
    return 0.;
  }    
  else
    return real(numerator)/denominator; }
}
*/

ostream& 
operator<< ( ostream& os, const Fraction& f)
{
  os << "(" << f.numerator << "," << f.denominator << ")" ;
  return os;
}
