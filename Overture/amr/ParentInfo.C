// created by Bobby Philip 05092001
#include "ParentInfo.h"

ParentInfo::~ParentInfo()
{
}

ParentInfo::ParentInfo(): className("ParentInfo")
{
}

ParentInfo::ParentInfo( const ParentInfo &x )
{
  className = "ParentInfo";  
  *this=x;
}

ParentInfo::ParentInfo( const int parentGridIndex, 
			const Box &pBox ): ParentChildSiblingBase( parentGridIndex ), 
                                           parentBox(pBox), className("ParentInfo")
{
}

ParentInfo &ParentInfo::operator=( const ParentInfo &x )
{
  ParentChildSiblingBase::operator=(x);
  parentBox = x.getParentBox();
  assert( parentBox.ok() );
  return *this;
}

Integer ParentInfo::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

Integer ParentInfo::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;
  cerr << className << "::put( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

void ParentInfo::consistencyCheck() const
{
  ParentChildSiblingBase::consistencyCheck();
  // do consistency check for member variables here
}

ostream& operator<<(ostream& s, const ParentInfo &parentInfo )
{
  return s
    << "className = " << parentInfo.className << endl
    << (ParentChildSiblingBase &) parentInfo << endl
    << "parentBox " << parentInfo.parentBox << endl;
}
