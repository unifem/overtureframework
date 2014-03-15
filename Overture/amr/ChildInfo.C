// created by Bobby Philip 05092001

#include "ChildInfo.h"

//! default constructor, sets className only
ChildInfo::ChildInfo(): className("ChildInfo")
{
}

//! destructor, nothing is done
ChildInfo::~ChildInfo()
{
}

//! copy constructor
ChildInfo::ChildInfo( const ChildInfo &x )
{
  className = "ChildInfo";  
  *this=x;   // call operator=()
}

//! main constructor that takes two arguments
/*! 
   \param childGridIndex: index of the child grid in the adaptive grid collection
   \param cBox : box representing index space on child grid lying over 
                 the MappedGrid to which this ChildInfo object belongs
 */
ChildInfo::ChildInfo( const int childGridIndex, 
		      const Box &cBox ): ParentChildSiblingBase( childGridIndex ),
                                         childBox(cBox), className("ChildInfo")
{
}

//! the assignment operator
ChildInfo &ChildInfo::operator=( const ChildInfo &x )
{
  ParentChildSiblingBase::operator=(x);
  childBox  = x.getChildBox();
  assert( childBox.ok() );
  return *this;
}

//! get() function to read from a database .. not currently implemented
Integer ChildInfo::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

//! put() function to write to a database .. not currently implemented
Integer ChildInfo::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;
  cerr << className << "::put( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

//! this function performs some error checking on the ChildInfo object
void ChildInfo::consistencyCheck() const
{
  ParentChildSiblingBase::consistencyCheck();
  // do consistency check for member variables here
}

//! friend operator<< to write to the output stream
/*! 
 \param s : output stream to write to
 \param childInfo : ChildInfo object whoe details are to be written
 */
ostream& operator<<(ostream& s, const ChildInfo &childInfo )
{
  return s
    << "className = " << childInfo.className << endl
    << (ParentChildSiblingBase &) childInfo << endl
    << "childBox " << childInfo.childBox << endl;
}
