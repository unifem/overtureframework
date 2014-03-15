// created by Bobby Philip 05092001
#ifndef _CHILDINFO_H_
#define _CHILDINFO_H_

#include "ParentChildSiblingBase.h"
#include "Box.H"

// in future these classes could be reference counted but
// at this point we are not going to worry about this issue
class ChildInfo: public ParentChildSiblingBase
{
 private:
  //! fine box on child MappedGrid  that lies over the parent MappedGrid
  Box        childBox;    
  //! this variable is set to ChildInfo
  aString    className;   
 public:
  //! destructor
  ~ChildInfo(); 
  //! default constructor
  ChildInfo();           
  //! copy constructor                         
  ChildInfo( const ChildInfo &X );                

  ChildInfo( const int childGridIndex,
	     const Box &childBox  ); 
  //! assignment operator
  ChildInfo &operator=( const ChildInfo &X );     

  virtual inline aString getClassName( void ) { return className; }

  virtual Integer get( const GenericDataBase & db, const aString &name );
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;

  virtual void consistencyCheck( void ) const;

  //! get the box representing the intersection in the child's index space
  const inline Box &getChildBox ( void ) const { return childBox; }
  //! set the box representing the intersection in the child's index space
  inline void setChildBox ( const Box &box ) { assert( box.ok() ); childBox  = box; }
  friend ostream& operator<<(ostream& s, const ChildInfo &childInfo );
};

#endif // _CHILDINFO_H_
