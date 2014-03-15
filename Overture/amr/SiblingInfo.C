// created by Bobby Philip 05092001
#include "SiblingInfo.h"

SiblingInfo::~SiblingInfo()
{
}

SiblingInfo::SiblingInfo(): className("SiblingInfo")
{
}

// copy constructor
SiblingInfo::SiblingInfo( const SiblingInfo &x )
{
  className = "SiblingInfo";
  *this=x; // call the assignment operator
}

SiblingInfo::SiblingInfo( const int  siblingGridIndex, 
			  const Box  &gBox,
			  const Box  &sBox ):
  ParentChildSiblingBase( siblingGridIndex ), ghostBox(gBox), siblingBox( sBox ), className("SiblingInfo")
{
}

SiblingInfo &SiblingInfo::operator=( const SiblingInfo &x )
{
  ParentChildSiblingBase::operator=(x);
  ghostBox   = x.getGhostBox();
  assert(ghostBox.ok());
  siblingBox = x.getSiblingBox();
  assert( siblingBox.ok() );
  return *this;
}

Integer SiblingInfo::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

Integer SiblingInfo::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;
  cerr << className << "::put( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

void SiblingInfo::consistencyCheck( void ) const
{
  ParentChildSiblingBase::consistencyCheck();
  // do consistency check for member variables here
}

ostream& operator<<(ostream& s, const SiblingInfo &siblingInfo )
{
  return s
    << "className = " << siblingInfo.className << endl
    << (ParentChildSiblingBase &) siblingInfo << endl
    << "ghostBox " << siblingInfo.ghostBox << endl
    << "siblingBox " << siblingInfo.siblingBox << endl;
}
