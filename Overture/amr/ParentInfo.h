// created by Bobby Philip 05092001
#ifndef _PARENTINFO_H_
#define _PARENTINFO_H_

#include "ParentChildSiblingBase.h"
#include "Box.H"
// in future these classes could be reference counted but
// at this point we are not going to worry about this issue
class ParentInfo: public ParentChildSiblingBase
{
 private:
  //! coarse box on parent MappedGrid that lies under the child MappedGrid
  Box        parentBox;
  //! stores the name of this class
  aString    className;
 public:
  //! destructor
  ~ParentInfo();
  //! default constructor
  ParentInfo();          
  //! copy constructor
  ParentInfo( const ParentInfo &X );
  //! main constructor for this class
  ParentInfo( const int parentGridIndex, 
	      const Box &parentBox ); 
  //! assignment operator
  ParentInfo &operator=( const ParentInfo &X );
  //! returns the name of this class, namely "ParentInfo"
  virtual inline aString getClassName( void ) { return className; }
  //! read from a database
  virtual Integer get( const GenericDataBase & db, const aString &name );
  //! write to a database
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;
  //! error and consistency checking
  virtual void consistencyCheck( void ) const; 
  //! get the box representing the index space on the parent MappedGrid that overlaps
  //! with the MappedGrid corresponding to this object
  const inline Box &getParentBox( void ) const { return parentBox;}
  //! set the parent box
  inline void setParentBox( const Box &box ) { assert( box.ok() ); parentBox = box; }
  //! write a description of parentInfo to s
  friend ostream& operator<<(ostream& s, const ParentInfo &parentInfo );
};

#endif // _PARENTINFO_H_
