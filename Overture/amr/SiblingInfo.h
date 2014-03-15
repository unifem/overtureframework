// created by Bobby Philip 05092001
#ifndef _SIBLINGINFO_H_
#define _SIBLINGINFO_H_

#include "ParentChildSiblingBase.h"
#include "Box.H"

// in future these classes could be reference counted but
// at this point we are not going to worry about this issue

class SiblingInfo: public ParentChildSiblingBase
{
 private:
  // we need to keep both boxes to handle periodic grids
  //! box that specifies ghost region covered by sibling
  Box        ghostBox;
  //! box on sibling MappedGrid that covers ghost region
  Box        siblingBox;
  //! stores the name of this class, i.e. "SiblingInfo"
  aString    className;
 public:
  //! destructor
  ~SiblingInfo();
  //! default constructor
  SiblingInfo();
  //! copy constructor
  SiblingInfo( const SiblingInfo &X );
  //! main constructor for this class
  SiblingInfo( const int siblingGridIndex,
	       const Box &ghostBox,
	       const Box &siblingBox ); 
  //! assignment operator
  SiblingInfo &operator=( const SiblingInfo &X );
  //! return the name of this class, namely "SiblingInfo"
  virtual inline aString getClassName( void ) { return className; }
  //! reads from a database
  virtual Integer get( const GenericDataBase & db, const aString &name );
  //! writes to a database
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;
  //! error and consistency checking
  virtual void consistencyCheck( void ) const;
  //! returns the ghost box on the grid owning this object
  const inline Box &getGhostBox( void ) const { return ghostBox;}
  //! returns the box on the sibling that lies atop the ghost box on the grid owning this object
  const inline Box &getSiblingBox( void ) const { return siblingBox;}
  //! set the ghost box
  inline void setGhostBox( const Box &box ) { assert( box.ok() ); ghostBox = box; }
  //! set the sibling box
  inline void setSiblingBox( const Box &box ) { assert( box.ok() ); siblingBox = box; }
  //! writes a description of siblingInfo to the output stream s
  friend ostream& operator<<(ostream& s, const SiblingInfo &siblingInfo );
};
#endif //_SIBLINGINFO_H_
