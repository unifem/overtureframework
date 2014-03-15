// created by Bobby Philip 05092001
#ifndef _PARENTCHILDSIBLINGINFO_H_
#define _PARENTCHILDSIBLINGINFO_H_

#include <limits.h>
#include "GenericDataBase.h"
#include "ReferenceCounting.h"
#include "GenericDataBase.h"
#include "GridCollection.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <list>
OV_USINGNAMESPACE(std);
#else
#include <list.h>
#endif

#include "ListOfParentChildSiblingInfo.h"
#include <BoxArray.H>

// forward declarations
class ChildInfo;
class ParentInfo;
class SiblingInfo;

class ParentChildSiblingInfoData: public ReferenceCounting
{
 private:

  //! list of parents  ( coarser grids that lie underneath grid that corresponds to this object )
  list<ParentInfo*>  ParentList;  

  //! list of children ( finer grids that lie over grid that corresponds to this object)
  list<ChildInfo*>   ChildList;   

  //! list of siblings ( grids at same refinement level that share a point, edge or surface)
  list<SiblingInfo*> SiblingList; 

  //! stores the name of this class, i.e. "ParentChildSiblingInfoData"
  aString className;

  //! virtual functions used only through class ReferenceCounting
  inline virtual ReferenceCounting& operator=( const ReferenceCounting &x )
    { return operator=((ParentChildSiblingInfoData &) x);}

  inline virtual void reference( const ReferenceCounting &x )
    { reference((ParentChildSiblingInfoData &) x); }

  inline virtual ReferenceCounting *virtualConstructor( const CopyType ct = DEEP ) const
    { return new ParentChildSiblingInfoData(*this, ct); }
  
 public:

  //! default constructor
  ParentChildSiblingInfoData();

  //! copy constructor
  ParentChildSiblingInfoData( const ParentChildSiblingInfoData &x,
			      const CopyType ct = DEEP );

  //! destructor
  ~ParentChildSiblingInfoData();

  //! assignment operator
  ParentChildSiblingInfoData &operator=( const ParentChildSiblingInfoData &x );

  //! returns the name of this class
  inline virtual aString getClassName( void ) const { return className; }

  //! creates a reference to another ParentChildSiblingInfoData object
  //! should not be called in general
  void reference( const ParentChildSiblingInfoData &x );

  //! break the reference to an object
  virtual void breakReference(void);

  //! performs internal error and consistency checks
  virtual void consistencyCheck( void ) const;

  //! read from a database
  virtual Integer get( const GenericDataBase & db, const aString &name );

  //! write to a database
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;

  //! returns a reference to the ParentList
  inline const list<ParentInfo* > &getParents (void) const { return ParentList; }

  //! returns a reference to the ChildList
  inline const list<ChildInfo*  > &getChildren(void) const { return ChildList;  }

  //! returns a reference to the SiblingList
  inline const list<SiblingInfo*> &getSiblings(void) const { return SiblingList;}

  //! add a ParentInfo object to ParentList
  void addParent ( ParentInfo * );

  //! add a ChildInfo object to ChildList
  void addChild  ( ChildInfo  * );

  //! add a SiblingInfo object to SiblingList
  void addSibling( SiblingInfo *);

  //! write a description of pcsInfoData to the output stream s
  friend ostream& operator<<(ostream& s, const ParentChildSiblingInfoData &pcsInfoData );

};

class ParentChildSiblingInfo: public ReferenceCounting
{
 private:

  //! stores the name of this class, i.e. "ParentChildSiblingInfo"
  aString className;

  //! virtual functions used only through class ReferenceCounting
  inline virtual ReferenceCounting& operator=( const ReferenceCounting &x )
    { return operator=((ParentChildSiblingInfo &) x);}

  inline virtual void reference( const ReferenceCounting &x )
    { reference((ParentChildSiblingInfo &) x); }

  inline virtual ReferenceCounting *virtualConstructor( const CopyType ct = DEEP ) const
    { return new ParentChildSiblingInfo(*this, ct); }

  //! check if two MappedGrids overlap
  static bool gridsOverlap( const MappedGrid &, const MappedGrid &, const IntegerArray &refinementRatio, Box & );

  //! check whether siblings
  static bool isSibling( const MappedGrid &, const MappedGrid &, Box & );

  //! check for siblings
  static bool isSibling( const Box &, const Box &, const IntegerArray &, const int, Box & );

  //! create and add SiblingInfo object to the SiblingList if two MappedGrids are "periodic siblings"
  bool addIfPeriodicSibling( const MappedGrid &,
			     const MappedGrid &, 
			     const MappedGrid &,
			     const IntegerArray &,
			     const int );
  //! create and add a ParentInfo object to the ParentList of rcData
  void addParent(  const int, 
		   const Box & );

  //! create and add a ChildInfo object to the ChildList of rcData
  void addChild( const int, 
		 const Box & );
  //! create and add a SiblingInfo object to the SiblingList of rcData
  void addSibling( const int siblingGridIndex,
		   const Box &ghostBox,
		   const Box &siblingBox );


 public:
  // public member data
  typedef ParentChildSiblingInfoData RCData;   // following conventions in Overture
  RCData *rcData;

  //! default constructor
  ParentChildSiblingInfo();
  //!  copy constructor
  ParentChildSiblingInfo( const ParentChildSiblingInfo &x,
			  const CopyType ct = DEEP );
  //! destructor
  ~ParentChildSiblingInfo();

  //! assignment operator
  ParentChildSiblingInfo &operator=( const ParentChildSiblingInfo &x );

  //! returns the className
  inline virtual aString getClassName( void ) const { return className; }

  //! make a reference or a shallow copy
  void reference( const ParentChildSiblingInfo &x );

  //! reference a ParentChildSiblingInfoData object
  void reference( ParentChildSiblingInfoData &x );

  //! break a reference and replace with a deep copy
  virtual void breakReference( void );

  //! performs error and consistency checks
  virtual void consistencyCheck( void ) const;

  //! read from a database
  virtual Integer get( const GenericDataBase & db, const aString &name );

  //! write to a database
  virtual Integer put( const GenericDataBase & db, const aString &name ) const;

  //! return list of parents from letter (rcData)
  inline const list<ParentInfo*>  &getParents (void) const { assert( rcData != NULL ); return rcData->getParents(); }

  //! return list of children from letter (rcData)
  inline const list<ChildInfo*>   &getChildren(void) const { assert( rcData != NULL ); return rcData->getChildren();}

  //! return list of siblings from letter (rcData)
  inline const list<SiblingInfo*> &getSiblings(void) const { assert( rcData != NULL ); return rcData->getSiblings();}

  //! builds ParentChildSiblingInfo objects for all MappedGrids in a GridCollection object
  static void buildParentChildSiblingInfoObjects( const GridCollection &gc, 
						  ListOfParentChildSiblingInfo &listOfPCSInfo );

  //! returns true if the box is valid for the given IndexType
  static bool isValidBoxForType( const Box &box, const IndexType &iType );

  //! returns a list of ghost boxes representing the index space of the ghost points around a MappedGrid
  static void getGhostRegionBoxes( const MappedGrid &, const Range &, const IndexType &, BoxList & ,
                                   bool excludePhysicalBoundaries = false );

  //! returns a list of parent boxes and the indices of the grids on which these boxes exist
  int getParentBoxes( intSerialArray &, BoxList &, const IndexType &, const int );

  //! returns a list of child boxes and the indices of the grids on which these boxes exist
  int getChildBoxes ( intSerialArray &, BoxList &, const IndexType &, const int );

  //! returns a list of sibling boxes and the indices of the grids on which these boxes exist
  int getSiblingBoxes ( intSerialArray &, BoxList &, BoxList &, const IndexType &, const int );

  //! returns a list of parent ghost boxes and the indices of the grids on which these boxes exist
  int getParentGhostBoxes ( intSerialArray &, BoxList &, const Range &, const GridCollection &, const int, const IndexType &, const bool = FALSE );

  //! returns a list of sibling ghost boxes and the indices of the grids on which these boxes exist
  int getSiblingGhostBoxes( intSerialArray &, BoxList &, BoxList &, const Range &, const GridCollection &, const int, const IndexType &  );

  // writes a description of pcsInfo to output stream s
  friend ostream& operator<<(ostream& s, const ParentChildSiblingInfo &pcsInfo );
};

#endif  //_PARENTCHILDSIBLINGINFO_H_
