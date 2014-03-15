// created by Bobby Philip 05092001
#include "ParentChildSiblingBase.h"

ParentChildSiblingBase::ParentChildSiblingBase()
{
  className = "ParentChildSiblingBase";
  gridIndex = -1;
}

ParentChildSiblingBase::ParentChildSiblingBase( const int index ): gridIndex(index), className("ParentChildSiblingBase")
{
}

ParentChildSiblingBase::ParentChildSiblingBase( const ParentChildSiblingBase &x,
						const CopyType ct)
{
  className = "ParentChildSiblingBase";
  if( ct != NOCOPY ) *this=x;
}

ParentChildSiblingBase &ParentChildSiblingBase::operator=( const ParentChildSiblingBase &x )
{
  gridIndex = x.getGridIndex();
  return *this;
}

ParentChildSiblingBase::~ParentChildSiblingBase()
{
  // do nothing
}

Integer ParentChildSiblingBase::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;  
}

Integer ParentChildSiblingBase::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;  
}

void ParentChildSiblingBase::consistencyCheck( void ) const
{
}

ostream& operator<<(ostream& s, const ParentChildSiblingBase &pcsBase )
{
  return s
    << "className = " << pcsBase.className << endl
    << "gridIndex = " << pcsBase.getGridIndex() << endl;
}
