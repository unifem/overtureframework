//kkc 081124 #include <iostream.h>
#include <iostream>
#include "Fraction.h"
#include "Bound.h"
#include "GenericDataBase.h"

#define TRUE 1
#define FALSE 0

int Bound::
get( const GenericDataBase & dir, const aString & name)
// get a Bound from the database
{
  GenericDataBase *subDir = dir.virtualConstructor();
  dir.find(*subDir,name,"Bound");

  int btint;
  subDir->get( btint,"bt" );   bt=boundType(btint);   // convert integer to enum type
  subDir->get( x,"x" );
  f.get( *subDir,"f" );

  delete subDir;
  return 0; 
}

int Bound::
put( GenericDataBase & dir, const aString & name) const
// put a Bound to a database
{
  GenericDataBase *subDir = dir.virtualConstructor();      // create a derived data-base object
  dir.create(*subDir,name,"Bound");                      // create a sub-directory 
  
  subDir->put( (int)bt,"bt" );
  subDir->put( x,"x" );
  f.put( *subDir,"f" );

  delete subDir;
  return 0;
}


void Bound::
get( boundType & bndType, real & rvalue, Fraction & fvalue ) const
{ // return the value of the bound, either a real number or fraction
  bndType=bt;
  switch( bt ){
    case( realNumber ):
      rvalue=x; break;
    case( fraction ):
      fvalue=f; break;
  }
}


bool Bound:: 
isFinite()
{
  if( bt == null || (bt==fraction && f.getDenominator()== 0) ) //*ap*
    return FALSE;
  else
    return TRUE;
}

Bound::
operator real()
// conversion to a real number.
// return REAL_MAX for an infinite number
{
  switch( bt ){
  case( realNumber ):
    return x; 
  case( fraction ):
    if( f.getDenominator()== 0 )
      return REAL_MAX;
    else
      return f.getNumerator()/real(f.getDenominator());
  case null:
    cout << "Bound::operator real(): ERROR: This bound is null! \n";
    return 0.;
  default:
    cout << "Error:Bound: invalid value for bt= " << bt << "\n";
    return 0.;
  }
}



Bound operator + ( const Bound b1, const Bound b2 ) 
{  // Friend operator +
  switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.x + b2.x ); 
        case( fraction ):
          return Bound( b1.x + b2.f );   // real+fraction=real
        case( null ):
          return Bound( );  // return a null
        default:
          cout << "Error in bound + bound " << endl;
          return Bound();
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.f + b2.x );
        case( fraction ):
          return Bound( b1.f + b2.f ); 
        case( null ):
          return Bound(); 
        default:
          cout << "Error in bound + bound " << endl;
          return Bound();
      }
    case( null ):
      return Bound();
    default:
      cout << "Error in bound + bound " << endl;
      return Bound();
  }
}

Bound operator - ( const Bound b1, const Bound b2 ) 
{  // Friend operator -
  switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.x - b2.x ); 
        case( fraction ):
          return Bound( b1.x - b2.f );   // real-fraction=real
        case( null ):
          return Bound( );  // return a null
        default:
          cout << "Error in bound - bound " << endl;
          return Bound();
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.f - b2.x );
        case( fraction ):
          return Bound( b1.f - b2.f ); 
        case( null ):
          return Bound(); 
        default:
          cout << "Error in bound - bound " << endl;
          return Bound();
      }
    case( null ):
      return Bound();
    default:
      cout << "Error in bound - bound " << endl;
      return Bound();
  }
}


Bound operator * ( const Bound b1, const Bound b2 ) 
{  // Friend operator *
  switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.x * b2.x ); 
        case( fraction ):
          return Bound( b1.x * b2.f ); 
        case( null ):
          return Bound( );  // return a null
        default:
          cout << "Error in bound * bound " << endl;
          return Bound();
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.f * b2.x );
        case( fraction ):
          return Bound( b1.f * b2.f ); 
        case( null ):
          return Bound(); 
        default:
          cout << "Error in bound * bound " << endl;
          return Bound();
      }
    case( null ):
      return Bound();
    default:
      cout << "Error in bound * bound " << endl;
      return Bound();
  }
}

Bound operator / ( const Bound b1, const Bound b2 ) 
{  // Friend operator /
  switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          if( b2.x != 0. )
            return Bound( b1.x / b2.x ); 
          else
            return Bound( b1.x / Fraction(1,0) );
        case( fraction ):
          return Bound( b1.x / b2.f ); 
        case( null ):
          return Bound( );  // return a null
        default:
          cout << "Error in bound / bound " << endl;
          return Bound();
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return Bound( b1.f / b2.x );
        case( fraction ):
          return Bound( b1.f / b2.f ); 
        case( null ):
          return Bound(); 
        default:
          cout << "Error in bound / bound " << endl;
          return Bound();
      }
    case( null ):
      return Bound();
    default:
      cout << "Error in bound / bound " << endl;
      return Bound();
  }
}

//-----------------------------------------------------------------
// Define the friend operator <= for two elements of type bound
//   If both bounds are null this routine returns TRUE,
//   if one bound is null but not the other we return FALSE,
//   (This is a friend operator so it handles the case real<=bound)
//-----------------------------------------------------------------
int operator <=( const Bound b1, const Bound b2 ){
  switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          return b1.x <= b2.x; 
        case( fraction ):
          return b1.x <= b2.f; 
        case( null ):
          return FALSE; 
        default:
          cout << "Error in bound <= bound " << endl;
          return FALSE;
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return b1.f <= b2.x;
        case( fraction ):
          return b1.f <= b2.f; 
        case( null ):
          return FALSE; 
        default:
          cout << "Error in bound <= bound " << endl;
          return FALSE;
      }
    case( null ):
      switch( b2.bt ){
        case( realNumber ):
          return FALSE; 
        case( fraction ):
          return FALSE; 
        case( null ):
          return TRUE; 
        default:
          cout << "Error in bound <= bound " << endl;
          return FALSE;
      }
    default:
      cout << "Error in bound <= bound " << endl;
      return FALSE;
  }
}

//-----------------------------------------------------------------
// Define the operator < for two elements of type bound
//   If both bounds are null this routine returns TRUE,
//   if one bound is null but not the other we return FALSE,
//-----------------------------------------------------------------
int operator <( const Bound b1, const Bound b2 ) 
{ switch( b1.bt){
    case( realNumber ):
      switch( b2.bt ){
        case( realNumber ):
          return b1.x < b2.x; 
        case( fraction ):
          return b1.x < b2.f; 
        case( null ):
          return FALSE; 
        default:
          cout << "Error in bound < bound " << endl;
          return FALSE;
      }
    case( fraction ):
      switch( b2.bt ){
        case( realNumber ):
          return b1.f < b2.x; 
        case( fraction ):
          return b1.f < b2.f; 
        case( null ):
          return FALSE; 
        default:
          cout << "Error in bound < bound " << endl;
          return FALSE;
      }
    case( null ):
      switch( b2.bt ){
        case( realNumber ):
          return FALSE; 
        case( fraction ):
          return FALSE; 
        case( null ):
          return TRUE; 
        default:
          cout << "Error in bound < bound " << endl;
          return FALSE;
      }
    default:
      cout << "Error in bound < bound " << endl;
      return FALSE;
  }
}

//-----------------------------------------------------------------
// Define the operator > for two elements of type bound
//   If both bounds are null this routine returns TRUE,
//   if one bound is null but not the other we return FALSE,
//-----------------------------------------------------------------
int operator >( const Bound b1, const Bound b2 ) 
{ return !(b1<=b2);
}

//-----------------------------------------------------------------
// Define the operator >= for two elements of type bound
//   If both bounds are null this routine returns TRUE,
//   if one bound is null but not the other we return FALSE,
//-----------------------------------------------------------------
int operator >=( const Bound b1, const Bound b2 ) 
{ return !(b1<b2);
}

//-----------------------------------------------------------------
// Define the operator == for two elements of type bound
//   If both bounds are null this routine returns TRUE,
//   if one bound is null but not the other we return FALSE,
//-----------------------------------------------------------------
int operator ==( const Bound b1, const Bound b2 ) 
{ switch( b1.bt){
    case( realNumber ):
      return ((b2.bt==realNumber)&&(b1.x==b2.x));
    case( fraction ):
      return ((b2.bt==fraction)&&(b1.f==b2.f));
    case( null ):
      return FALSE;
    default:
      cout << "Error in bound == bound " << endl;
      return FALSE;
  }
}

ostream& 
operator<< ( ostream& os, const Bound& b)
{ switch( b.bt )
  { case( realNumber ):
      os << b.x ; break;
    case( fraction ):
      os << "(" << b.f.getNumerator() << "," << b.f.getDenominator() << ")" ; break;
    case( null ):
      os << "NULL" ; break;
    default:
      cout << "Error in bound << " << endl;
  }
  return os;
}
