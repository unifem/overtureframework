// created by Bobby Philip 05092001
#ifndef _PARENTCHILDSIBLINGBASE_H_
#define _PARENTCHILDSIBLINGBASE_H_

#include "GenericDataBase.h"
#include "MappedGrid.h"

// this header file contains the declaration of the base class for the
// ParentInfo, ChildInfo, and SiblingInfo objects
class ParentChildSiblingBase
{
 protected:
  //! index into the adaptive grid collection
  int gridIndex;  
  //! the name of this class, i.e. "ParentChildSiblingBase" is stored in this variable
  aString className;
 public:
  //! default constructor
  ParentChildSiblingBase();
  //! copy constructor 
  ParentChildSiblingBase( const ParentChildSiblingBase &X, const CopyType ct = DEEP );
  //! main constructor for this class
  ParentChildSiblingBase( const int );
  //! assignment operator
  ParentChildSiblingBase &operator=( const ParentChildSiblingBase &X );
  //! virtual destructor
  virtual ~ParentChildSiblingBase();
  //! return the className
  virtual inline aString getClassName( void ) { return className; }
  //! read from a database
  virtual Integer get( const GenericDataBase & db, const aString &name );
  //! write to a database
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;
  //! performs error and consistency checks
  virtual void consistencyCheck( void ) const;

  //! access functions for the grid index
  inline const int getGridIndex( void ) const { return gridIndex; }
  inline void setGridIndex( const int index ) { gridIndex = index; }
  //! writes a descriptio of pcsBase to the output stream s
  friend ostream& operator<<(ostream& s, const ParentChildSiblingBase &pcsBase );
};

#endif // _PARENTCHILDSIBLINGBASE_H_
