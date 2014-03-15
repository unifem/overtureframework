/*  -*-Mode: c++; -*-  */
#ifndef COMPOSITE_GRID_FUNCTION
#define COMPOSITE_GRID_FUNCTION 

#include "GridCollectionFunction.h" 
#include "ListOfCompositeGridFunction.h"

class CompositeGrid;            // forward declaration
class CompositeGridData;        // forward declaration
class Interpolant;              // forward declaration
class GridCollectionOperators;  // forward declaration
class CompositeGridOperators;  
class intCompositeGridFunction;


//===================================================================
//  CompositeGridFunction
//
//  Define a grid function to be used with a composite grid.
//  This class contains a list of MappedGridFunction's
//  and a CompositeGrid
//
//  This is a reference counted class so that there is no need
//  to keep a pointer to a grid function. Use the reference
//  member function to make one grid function reference another.
//
//  Usage:
//     CompositeGrid cg(...);  // here is a composite grid
//     doubleCompositeGridFunction u(cg),v;
//     u[0]=5.;                // mapped grid function for grid 0
//     Index I(0,10);
//     u[1](I,I)=3.;           // mapped grid function for grid 1
//     v.reference(u);         // v is referenced to u
//     v[1]=7.;                // changes u as well
//     v.breakReference();     // v is no longer referenced to u
//     ...
//
//==================================================================

class CompositeGridFunction : public GridCollectionFunction
{
 public:

  CompositeGridFunction( );
  // This constructor takes ranges, the first 3 "nullRange" values are taken to be the
  // coordinate directions in the grid function.
  CompositeGridFunction(CompositeGrid & grid, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  CompositeGridFunction(CompositeGridData & gcData, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  CompositeGridFunction(CompositeGrid & grid, 
			const int   & i0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  CompositeGridFunction(CompositeGridData & gcData, 
			const int   & i0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  //
  // This constructor takes a GridFunctionType
  // 
  CompositeGridFunction(CompositeGrid & grid, 
			const GridFunctionParameters::GridFunctionType & type, 
			const Range & Component0=nullRange,       // defaults to Range(0,0)
			const Range & Component1=nullRange,
			const Range & Component2=nullRange,
			const Range & Component3=nullRange,
			const Range & Component4=nullRange );

  CompositeGridFunction(CompositeGrid & CompositeGrid);
  CompositeGridFunction(CompositeGridData & CompositeGrid);

  CompositeGridFunction( const CompositeGridFunction & cgf, const CopyType copyType=DEEP );
  CompositeGridFunction( const GridCollectionFunction & cgf, const CopyType copyType=DEEP );

  virtual ~CompositeGridFunction();

  // A derived class must define the = operators
  CompositeGridFunction & operator= ( const CompositeGridFunction & cgf );
  GridCollectionFunction & operator= ( const GridCollectionFunction & gcf ); 

  // Return a link to some specfied components 
  CompositeGridFunction operator()(const Range & component0,
				   const Range & component1=nullRange,
				   const Range & component2=nullRange,
				   const Range & component3=nullRange,
				   const Range & component4=nullRange );


  CompositeGridOperators* getOperators() const;    // return a pointer to the operators
  void setOperators(GenericCollectionOperators & operators );

  Interpolant* getInterpolant(const bool abortIfNull =TRUE ) const;     // return a pointer to the Interpolant

  virtual aString getClassName() const;

  CompositeGrid* getCompositeGrid(const bool abortIfNull=TRUE) const;   // return a pointer to the CompositeGrid

  void link(const CompositeGridFunction & gcf,
	    const Range & R0,
	    const Range & R1=nullRange,
	    const Range & R2=nullRange,
	    const Range & R3=nullRange,
	    const Range & R4=nullRange );

  void link(const GridCollectionFunction & gcf, // *New*
	    const Range & R0,
	    const Range & R1=nullRange,
	    const Range & R2=nullRange,
	    const Range & R3=nullRange,
	    const Range & R4=nullRange );

  virtual void link(const CompositeGridFunction & gcf, 
            const int componentToLinkTo=0,
            const int numberOfComponents=1 );

  virtual void link(const GridCollectionFunction & gcf, 
            const int componentToLinkTo=0,
            const int numberOfComponents=1 );

  void reference( const CompositeGridFunction & cgf );
  void reference( const GridCollectionFunction & cgf ); // *new*

  virtual void setIsACoefficientMatrix(const bool trueOrFalse=TRUE, 
				       const int stencilSize=defaultValue, 
				       const int numberOfGhostLines=1,
				       const int numberOfComponentsForCoefficients=1,
				       const int offset=0 );

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

  // define all the update functions
  updateReturnValue updateToMatchGrid();
  updateReturnValue updateToMatchGrid(CompositeGridData & gridData, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );
  
  updateReturnValue updateToMatchGrid(CompositeGrid & grid, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  // define this version to avoid overloading ambiguities
  updateReturnValue updateToMatchGrid(CompositeGrid & grid, 
			const int  & i0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  updateReturnValue updateToMatchGrid(const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );
  
  updateReturnValue updateToMatchGrid(CompositeGrid & grid, 
				      const GridFunctionParameters::GridFunctionType & type, 
				      const Range & component0,
				      const Range & component1=nullRange,
				      const Range & component2=nullRange,
				      const Range & component3=nullRange,
				      const Range & component4=nullRange );

  updateReturnValue updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type, 
				      const Range & component0,
				      const Range & component1=nullRange,
				      const Range & component2=nullRange,
				      const Range & component3=nullRange,
				      const Range & component4=nullRange );
  updateReturnValue updateToMatchGrid(CompositeGrid & grid, 
				      const GridFunctionParameters::GridFunctionType & type);
  updateReturnValue updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type);


  updateReturnValue updateToMatchGrid(CompositeGrid & grid);
  updateReturnValue updateToMatchGrid(CompositeGridData & grid);


  // update this grid function to match another grid function
  updateReturnValue updateToMatchGridFunction(const CompositeGridFunction & cgf, 
				const Range & R0, 
				const Range & R1=nullRange,
				const Range & R2=nullRange,
				const Range & R3=nullRange,
				const Range & R4=nullRange,
				const Range & R5=nullRange,
				const Range & R6=nullRange,
				const Range & R7=nullRange );

  updateReturnValue updateToMatchGridFunction(const CompositeGridFunction & gcf);

  // make sure the number of mappedGridFunction's is correct
  updateReturnValue updateToMatchNumberOfGrids(CompositeGrid& gc);


  //  Collections of grid functions having the same base grid.
  ListOfCompositeGridFunction baseGrid;       // overloads one base class

  //  Collections of grid functions having the same multigrid level.
  ListOfCompositeGridFunction multigridLevel; // overloads one base class

  //  Collections of grid functions having the same refinement level.
  ListOfCompositeGridFunction refinementLevel; // overloads one base class


  int interpolate(Interpolant & interpolant, 
		  const Range & C0 = nullRange,
		  const Range & C1 = nullRange,
		  const Range & C2 = nullRange );
  int interpolate(const Range & C0 = nullRange,
		  const Range & C1 = nullRange,
		  const Range & C2 = nullRange );

  friend CompositeGridFunction & FABS( const CompositeGridFunction & cgf );

  // use this function to  "evaulate and expression" when calling a function with an expression
  // involving grid collection functions.
  friend CompositeGridFunction evaluate( CompositeGridFunction & u );

#define COLLECTION_FUNCTION CompositeGridFunction
#define INT_COLLECTION_FUNCTION intCompositeGridFunction
#define COLLECTION CompositeGrid
#define INTEGRAL_TYPE double
#include "derivativeDefinitions.h"
#undef COLLECTION_FUNCTION
#undef INT_COLLECTION_FUNCTION
#undef COLLECTION 
#undef INTEGRAL_TYPE

  // These are needed to avoid compiler warnings
  updateReturnValue updateToMatchGrid(GridCollectionData & gridData, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );
  
  updateReturnValue updateToMatchGrid(GridCollection & grid, 
			const Range & R0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  updateReturnValue updateToMatchGrid(GridCollection & grid, 
			const int  & i0, 
			const Range & R1=nullRange,
			const Range & R2=nullRange,
			const Range & R3=nullRange,
			const Range & R4=nullRange,
			const Range & R5=nullRange,
			const Range & R6=nullRange,
			const Range & R7=nullRange );

  
  updateReturnValue updateToMatchGrid(GridCollection & grid, 
				      const GridFunctionParameters::GridFunctionType & type, 
				      const Range & component0,
				      const Range & component1=nullRange,
				      const Range & component2=nullRange,
				      const Range & component3=nullRange,
				      const Range & component4=nullRange );

  updateReturnValue updateToMatchGrid(GridCollection & grid, 
				      const GridFunctionParameters::GridFunctionType & type);

  updateReturnValue updateToMatchGrid(GridCollection & grid);
  updateReturnValue updateToMatchGrid(GridCollectionData & grid);


  // update this grid function to match another grid function
  updateReturnValue updateToMatchGridFunction(const GridCollectionFunction & cgf, 
				const Range & R0, 
				const Range & R1=nullRange,
				const Range & R2=nullRange,
				const Range & R3=nullRange,
				const Range & R4=nullRange,
				const Range & R5=nullRange,
				const Range & R6=nullRange,
				const Range & R7=nullRange );


  updateReturnValue updateToMatchGridFunction(const GridCollectionFunction & gcf);

  // make sure the number of mappedGridFunction's is correct
  updateReturnValue updateToMatchNumberOfGrids(GridCollection& gc);


 protected:
  void updateReferences();
  int updateCollections(); // update refinementLevel etc.

 private:
  
  // These are used by list's of ReferenceCounting objects
  virtual void reference( const ReferenceCounting & mgf )
  { CompositeGridFunction::reference( (CompositeGridFunction&) mgf ); }
  virtual ReferenceCounting & operator=( const ReferenceCounting & mgf )
  { return CompositeGridFunction::operator=( (CompositeGridFunction&) mgf ); }
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
  { return ::new CompositeGridFunction(*this,ct); }  

  aString className;

};  



#endif 
