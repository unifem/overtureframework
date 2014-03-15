/*  -*-Mode: c++; -*-  */
#ifndef MULTIGRID_COMPOSITE_GRID_H
#define MULTIGRID_COMPOSITE_GRID_H "MultigridCompositeGrid.h"

//=======================================================================================
/// \brief  MultigridCompositeGrid : holds the CompositeGrid and state for Multigrid so
///   that multiple solvers can share the same multigrid hierarchy.
///
/// \details Until the CompositeGrid is allocated, this is a very lightweight class.
//======================================================================================

#include "Overture.h"

class MultigridCompositeGrid : public ReferenceCounting    // derive the class from ReferenceCounting
{

public:

// default constructor
MultigridCompositeGrid( );                                                 

// destructor
~MultigridCompositeGrid();                                                 

 // copy constructor
MultigridCompositeGrid(const MultigridCompositeGrid & rcc,                
		       const CopyType copyType = DEEP );

// assignment operator
MultigridCompositeGrid& operator=( const MultigridCompositeGrid & rcc );   

// Allocate space for the CompositeGrid
int allocate();

/// \brief Return the CompositeGrid
const CompositeGrid & compositeGrid() const{ assert(rcData->pcg!=NULL); return *rcData->pcg; }  

CompositeGrid & operator()(){  assert(rcData->pcg!=NULL); return *rcData->pcg; }  // 
const CompositeGrid & operator()() const {  assert(rcData->pcg!=NULL); return *rcData->pcg; }  // 

/// \brief Return true if the multigrid hierarchy is up to date. 
bool isGridUpToDate(){ return rcData->isUpToDate; }  

// Return true if the CompositeGrid has not be allocated yet:
bool isNull() const { return rcData->pcg==NULL; }  // 

// Specify that the multigrid hierarchy is or is not update to date.
void setGridIsUpToDate( bool trueOrFalse =true );

// Update the grid to match a new CompositeGrid.
void updateToMatchGrid( CompositeGrid & cg );

// reference this object to another
void reference( const MultigridCompositeGrid & rcc );                      

// break a reference
void breakReference();                                                      

private:

// used by constructors
void initialize();                                                          

// These are used by list's of ReferenceCounting objects
virtual void reference( const ReferenceCounting & rcc )
    { MultigridCompositeGrid::reference( (MultigridCompositeGrid&) rcc ); }
virtual ReferenceCounting & operator=( const ReferenceCounting & rcc )
    { return MultigridCompositeGrid::operator=( (MultigridCompositeGrid&) rcc ); }
virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP )
    { return ::new MultigridCompositeGrid(*this,ct); }  

// protected:
public:  // *wdh* 110518 do this for now to fix a problem with gcc 4.5.?

// this class hold the reference counted data
class MultigridCompositeGridData : public ReferenceCounting  
{
public:

CompositeGrid *pcg;   // holds the multigrid hierarchy 
bool isUpToDate;    // set to true when the multigrid hierarchy is up to date.

MultigridCompositeGridData(); 

~MultigridCompositeGridData();

MultigridCompositeGridData& operator=(const MultigridCompositeGridData & rcc );

private:
// These are used by list's of ReferenceCounting objects
virtual void reference( const ReferenceCounting & rcc )
    { MultigridCompositeGridData::reference( (MultigridCompositeGridData&) rcc ); }
virtual ReferenceCounting & operator=( const ReferenceCounting & rcc )
    { return MultigridCompositeGridData::operator=( (MultigridCompositeGridData&) rcc ); }
virtual ReferenceCounting* virtualConstructor( const CopyType )
    { return ::new MultigridCompositeGridData(); }  
};


protected:
MultigridCompositeGridData *rcData;


};  

#endif 
