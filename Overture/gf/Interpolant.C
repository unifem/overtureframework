#include "Interpolant.h"
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "interpPoints.h"
#include "SparseRep.h"
#include "InterpolateRefinements.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "ParallelUtility.h"

real Interpolant::timeForExplicitInterpolation=0.;
real Interpolant::timeForImplicitInterpolation=0.;
real Interpolant::timeForIterativeImplicitInterpolation=0.;
real Interpolant::timeForAMRInterpolation=0.;
real Interpolant::timeForInitializeInterpolation=0.;

real Interpolant::timeForAMRCoarseFromFine=0.;
real Interpolant::timeForAMRExtrapolateRefinementBoundaries=0.;
real Interpolant::timeForAMRExtrapolateAll=0.;
real Interpolant::timeForAMRExtrapInterpolationNeighbours=0.;
real Interpolant::timeForAMRRefinementBoundaries=0.;

static int numberOfImplicitInterpolations=0;
static int numberOfImplicitIterations=0;


// Notes:
//                old(-O)  new (indirect add)
//    cic 44,32 : .012       .0135  >>>>Maximum error in interpolating = 4.093647e-04 <<<<<<
//    cic2 108,65 .031       .018      --> overhead about .009?
//    cilc:80,84  .034       .018      
//
//    sib2:       4.84              >>>>Maximum error in interpolating = 1.169443e-03 <<<<<<
//    sib.mg      4.5               >>>>Maximum relative error in interpolating = 3.388104e-04 <<<<<<


//\begin{>InterpolateInclude.tex}{\subsubsection{Constructors}}  
Interpolant::
Interpolant()
//========================================================================
// /Decsription: Default constructor.
//\end{InterpolateInclude.tex}  
//========================================================================
{
  initialize();
}

//\begin{>>InterpolateInclude.tex}{}
Interpolant::
Interpolant(CompositeGrid & cg0 )
//========================================================================
// /Decsription: 
//    Create an interpolant and associate with a Composite grid.
// /cg0 (input): assoicate the interpolant with this CompositeGrid.
//\end{InterpolateInclude.tex}  
//========================================================================
{
  initialize();
  updateToMatchGrid( cg0 );
}

Interpolant::
Interpolant(GridCollection & )
//========================================================================
// /Decsription: 
//    Create an interpolant and associate with a GridCollection.
// /gc (input): assoicate the interpolant with this GridCollection.
//========================================================================
{
 cout << "Interpolant:ERROR: Interpolant not implemented for a GridCollection \n";
}

Interpolant::RCData::
RCData()
{
  implicitInterpolant=NULL;
  parallelInterpolator=NULL;
}

Interpolant::RCData::
~RCData()
{
  delete implicitInterpolant;
  delete parallelInterpolator;
}


// deep copy of reference counted data 
Interpolant::RCData & Interpolant::RCData::
operator=( const Interpolant::RCData & rcdata )
{
  if( rcdata.implicitInterpolant !=NULL )
  {
    delete implicitInterpolant;
    // **** fix this ***
    // *implicitInterpolant = *(rcdata.implicitInterpolant);      // deep copy, does this work?
  }

  parallelInterpolator=rcdata.parallelInterpolator;  // is this right?

  return *this;
}


// Copy constructor, deep copy by default
Interpolant::
Interpolant(const Interpolant & interpolant, const CopyType copyType )
{
  switch (copyType)
  {
    case DEEP:
    initialize();
    (*this)=interpolant;
    break;
  case SHALLOW:
    rcData=interpolant.rcData;
    rcData->incrementReferenceCount();
    reference( (Interpolant &) interpolant ); 
    break;
  case NOCOPY:
    initialize();
    break;
  }
}


void Interpolant::
initialize()
// ===========================================================================================
// /Description:
//   Initialization routine.
// ===========================================================================================
{
  interpolationIsInitialized=false;
  debug=0;
  rcData = new RCData;  
  rcData->incrementReferenceCount();
  interpolationMethod=optimized; // optimizedC; // standard;
  implicitInterpolationMethod=directSolve; // iterateToInterpolate; // directSolve;
  explicitInterpolation=false;
  explicitInterpolationStorageOption=precomputeNoCoefficients; // precomputeAllCoefficients;
  useVariableWidthInterpolation=NULL;  // this will be set when the interpolation is initialized
  initializeParallelInterpolator=true;

  tolerance=REAL_EPSILON*50.;

  interpRefinements=NULL;
  interpolateRefinementBoundaries=true;  // if true, interpolate all refinement boundaries
  interpolateHidden=true;                // interpolate hidden coarse grid points from higher level refinemnts
  interpolateOverlappingRefinementBoundaries=true;
  updateForAdaptiveGrid=0;
  interpRefinementsWasNewed=false;
  maximumRefinementLevelToInterpolate=INT_MAX/2;

  maximumNumberOfIterations=25;
}




Interpolant::
~Interpolant()
{
  if( false )
  {
    printF("Interpolant::destructor called: envelope reference count=%i\n",getReferenceCount());
    printF("Interpolant::destructor called: rcData->getReferenceCount()=%i\n",rcData->getReferenceCount());
  }
  
  if( interpRefinementsWasNewed )
    delete interpRefinements;

  delete [] useVariableWidthInterpolation;
  
  if( rcData->decrementReferenceCount() == 0 )
    delete rcData; 

  // At the end, the reference counts should normally be 1 here since the Interpolant::cg will still have
  // a reference that will be decremented when the destructor is called at the end of this function. 
//  printF("Interpolant::destructor: envelope reference count=%i, rcData ref count=%i\n",getReferenceCount(),
//           getReferenceCount(),rcData->getReferenceCount() );

//   if( getReferenceCount()!=0 )
//   {
//     printF("Interpolant::destructor: ERROR: reference count=%i of the envelope class is non-zero!!\n",
//            getReferenceCount());
//     while( getReferenceCount()>0 )
//       decrementReferenceCount();
//   }
//   printF("Interpolant::destructor: (A) envelope reference count=%i\n",getReferenceCount());

//   if( rcData->getReferenceCount()!=0 )
//   {
//     printF("Interpolant::destructor:WARNING: rcData->getReferenceCount()!=0, value=%i\n"
//            "      Reference counting for the interpolant is currently broken. Fix me Bill!\n",
// 	   rcData->getReferenceCount());
//     while( rcData->getReferenceCount()>0 )
//       rcData->decrementReferenceCount();
//   }
//   printF("Interpolant::destructor: (B) envelope reference count=%i\n",getReferenceCount());
  
  
}

int Interpolant::
getComponentRanges(const Range & C0, const Range & C1, const Range & C2, Range C[4],
		   realCompositeGridFunction & u )
// ============================================================================================
// /Access: protected
// /Description:
//     Compute the appropriate ranges of components to interpolate.
// /C0,C1,C2,u (input) : 
// /C (output) :
//
// ===========================================================================================
{
  if( explicitInterpolation || implicitInterpolationMethod==iterateToInterpolate )
  {
    // Fill in C[i] to match indexing as u(I1,C[0],C[1],C[2],C[3])
    // where C[i] is either the component or coordinate dimenions
    if( u.positionOfComponent(0)<4 )
      C[u.positionOfComponent(0)]=C0;
    if( u.positionOfComponent(1)<4 )
      C[u.positionOfComponent(1)]=C1;
    if( u.positionOfComponent(2)<4 )
      C[u.positionOfComponent(2)]=C2;
    for( int i=0; i<4; i++ )
      if( C[i].length()<=0 )
	C[i]=Range(u[0].getBase(i),u[0].getBound(i));
  }
  else
  {
    // Fill in C[i] to match the component dimensions
    if( C0.length() > 0 )
      C[0]=C0;
     else
      C[0]=Range(u[0].getComponentBase(0),u[0].getComponentBound(0));
    if( C1.length() > 0 )
      C[1]=C1;
     else
      C[1]=Range(u[0].getComponentBase(1),u[0].getComponentBound(1));
    if( C2.length() > 0 )
      C[2]=C2;
     else
      C[2]=Range(u[0].getComponentBase(2),u[0].getComponentBound(2));
  }
  return 0;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate a CompositeGridFunction}}  
int Interpolant::
interpolate( realCompositeGridFunction & u,
		   const Range & C0 /* = nullRange */,     
		   const Range & C1 /* = nullRange */, 
		   const Range & C2 /* = nullRange */ )
//==============================================================================
// /Description:
//    Interpolate the interpolation boundary of a CompositeGridFunction
// /u (input/output): fill in the values on the interpolation boundary using
//    other values on the grid function.
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  Range C[4];
  getComponentRanges( C0,C1,C2,C,u);
  
  return internalInterpolate(u,C);
  
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate a single grid from a CompositeGridFunction}}  
int Interpolant::
interpolate( int gridToInterpolate,             // only interpolate this grid.
	     realCompositeGridFunction & u, 
	     const Range & C0 /* = nullRange */,      // optionally specify components to interpolate
	     const Range & C1 /* = nullRange */,  
	     const Range & C2 /* = nullRange */ )
//==============================================================================
// /Description:
//    Interpolate the interpolation boundary of a CompositeGridFunction
// /gridToInterpolate (input) : interpolate this grid (-1 ==> do all grids.)
// /u (input/output): fill in the values on the interpolation boundary using
//    other values on the grid function.
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  Range C[4];
  getComponentRanges( C0,C1,C2,C,u);
  if( gridToInterpolate>0 )
  {
    IntegerArray gridsToInterpolate(cg.numberOfComponentGrids());
    gridsToInterpolate=false;
    assert( gridToInterpolate>=0 && gridToInterpolate<cg.numberOfComponentGrids());
    gridsToInterpolate(gridToInterpolate)=true;
    return internalInterpolate(u,C,gridsToInterpolate);
  }
  else
  {
    return internalInterpolate(u,C);  // interpolate all
  }
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate specified grids from a CompositeGridFunction}}  
int Interpolant::
interpolate( realCompositeGridFunction & u, 
             const IntegerArray & gridsToInterpolate,        // only interpolate these grids.
	     const Range & C0 /* = nullRange */,      // optionally specify components to interpolate
	     const Range & C1 /* = nullRange */,  
	     const Range & C2 /* = nullRange */ )
//==============================================================================
// /Description:
//    Interpolate the interpolation boundary of a CompositeGridFunction
//
// /Note:  No AMR style interpolation will be applied when only some grids are interpolated.
//
// /u (input/output): fill in the values on the interpolation boundary using
//    other values on the grid function.
// /gridsToInterpolate (input) : an array of length cg.numberOfComponentGrids(), which 
//       specifies to interpolate grid g if  gridsToInterpolate(g)!=0 
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  Range C[4];
  getComponentRanges( C0,C1,C2,C,u);
  if( gridsToInterpolate.getLength(0)<cg.numberOfComponentGrids() )
  {
    printF("Interpolant::interpolate:ERROR: gridsToInterpolate.getLength(0)<cg.numberOfComponentGrids()\n");
    Overture::abort("error");
  }
  if( !interpolationIsExplicit() && implicitInterpolationMethod!=iterateToInterpolate )
  {
    printF("Interpolant::interpolate:ERROR: can only interpolate selected grid when the interpolation "
           "is explicit or we iterate to solve the implicit equations. \n"
           "Either remake the grid with explicit interpolation or use setImplicitInterpolationMethod. \n");
    Overture::abort("error");
  }
  
  return internalInterpolate(u,C,gridsToInterpolate);
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate specified grids from specified grids}}
int Interpolant::
interpolate( realCompositeGridFunction & u, 
             const IntegerArray & gridsToInterpolate,      // specify which grids to interpolate
	     const IntegerArray & gridsToInterpolateFrom,  // specify which grids to interpolate from
	     const Range & C0 /* = nullRange */,      // optionally specify components to interpolate
	     const Range & C1 /* = nullRange */,  
	     const Range & C2 /* = nullRange */)
//==============================================================================
// /Description:
//    Interpolate the interpolation boundary of a CompositeGridFunction
// /u (input/output): fill in the values on the interpolation boundary using
//    other values on the grid function.
// /gridsToInterpolate (input) : an array of length cg.numberOfComponentGrids(), which 
//       specifies to interpolate grid g if  gridsToInterpolate(g)!=0 
// /gridsToInterpolateFrom (input) : an array of length cg.numberOfComponentGrids(), which 
//       specifies to interpolate from grid g if gridsToInterpolateFrom(g)!=0 
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  Range C[4];
  getComponentRanges( C0,C1,C2,C,u);
  if( gridsToInterpolate.getLength(0)<cg.numberOfComponentGrids() )
  {
    printF("Interpolant::interpolate:ERROR: gridsToInterpolate.getLength(0)<cg.numberOfComponentGrids()\n");
    Overture::abort("error");
  }
  if( gridsToInterpolateFrom.getLength(0)<cg.numberOfComponentGrids() )
  {
    printF("Interpolant::interpolate:ERROR: gridsToInterpolateFrom.getLength(0)<cg.numberOfComponentGrids()\n");
    Overture::abort("error");
  }
  if( !interpolationIsExplicit() && implicitInterpolationMethod!=iterateToInterpolate )
  {
    printF("Interpolant::interpolate:ERROR: can only interpolate selected grid when the interpolation "
           "is explicit or we iterate to solve the implicit equations. \n"
           "Either remake the grid with explicit interpolation or use setImplicitInterpolationMethod. \n");
    Overture::abort("error");
  }
  
  return internalInterpolate(u,C,gridsToInterpolate,gridsToInterpolateFrom);
}




//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate grid A from grid B}}  
int Interpolant::
interpolate( realArray & ug,                   
	     int gridToInterpolate,            
	     int interpoleeGrid,               
             realCompositeGridFunction & u, 
	     const Range & C0 /* = nullRange */, 
	     const Range & C1 /* = nullRange */,  
	     const Range & C2 /* = nullRange */)
//==============================================================================
// /Description:
//    Interpolate points on grid "gridToInterpolate" that interpolate from 
// grid "interpoleeGrid".
//
// /Note:  No AMR style interpolation will be applied when only some grids are interpolated.
//
// /ug (output): fill in the interpolated values into this array. This array will be dimensioned
//   to hold the proper number of interpolation point values.
// /gridToInterpolate (input) : interpolate points on this grid.
// /interpoleeGrid (input) : only compute points that interpolate from this grid.
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  real time=getCPU();
  
  #ifdef USE_PPP
    OV_ABORT("Interpolant::interpolate( realArray ...) : finish me for parallel");  // added 100417 
  #endif

  assert( interpolationIsInitialized ); // added 100417 

  assert( gridToInterpolate>=0 && gridToInterpolate<cg.numberOfComponentGrids() );
  assert( interpoleeGrid>=0 && interpoleeGrid<cg.numberOfComponentGrids() );

  const int grid=gridToInterpolate;
  const int gridi=interpoleeGrid;
  
  
  if( cg.interpolationStartEndIndex(0,grid,gridi) >= 0 )
  {
    Range C[4];
    getComponentRanges( C0,C1,C2,C,u);

    Range R(cg.interpolationStartEndIndex(0,grid,gridi),cg.interpolationStartEndIndex(1,grid,gridi));
    const realArray & ui = u[interpoleeGrid];
    RealDistributedArray & coeffg = coeff[grid];

    const intArray & il = cg.interpoleeLocation[grid];

    if( false )
    {
//      ** ug is a different shape here ***   
//       interpOpt( cg.numberOfDimensions(),
// 		 ui.getBase(0),ui.getBound(0),ui.getBase(1),ui.getBound(1),
// 		 ui.getBase(2),ui.getBound(2),ui.getBase(3),ui.getBound(3),
// 		 ug.getBase(0),ug.getBound(0),ug.getBase(1),ug.getBound(1),
// 		 ug.getBase(2),ug.getBound(2),ug.getBase(3),ug.getBound(3),
// 		 il.getLength(0),ip.getLength(0),
// 		 coeffg.getLength(0),coeffg.getLength(1),coeffg.getLength(2),
// 		 R.getBase(),R.getBound(),
// 		 C[2].getBase(),C[2].getBound(),C[3].getBase(),C[3].getBound(),
// 		 // *ui.getDataPointer(),
// 		 ui(ui.getBase(0),ui.getBase(1),ui.getBase(2),ui.getBase(3),ui.getBase(4)),
// 		 // *ug.getDataPointer(),
// 		 ug(ug.getBase(0),ug.getBase(1),ug.getBase(2),ug.getBase(3),ug.getBase(4)),
// 		 *coeffg.getDataPointer(),
// 		 *il.getDataPointer(),
// 		 *ip.getDataPointer(),
// 		 width(0,grid) );
    }
    else
    {
      if( cg.numberOfDimensions()==2 )
      {
	ug.redim(R,C[3]);
	assert( C[2].getLength()==1 );
      
	if( width(axis1,grid)==3 && width(axis2,grid)==3 )
	{

	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	      ug(R,c3)=          
		coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
		+coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
		+coeffg(R,2,0,0)*ui(il(R,axis1)+2,il(R,axis2)  ,c2,c3)
		+coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
		+coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3) 
		+coeffg(R,2,1,0)*ui(il(R,axis1)+2,il(R,axis2)+1,c2,c3) 
		+coeffg(R,0,2,0)*ui(il(R,axis1)  ,il(R,axis2)+2,c2,c3) 
		+coeffg(R,1,2,0)*ui(il(R,axis1)+1,il(R,axis2)+2,c2,c3) 
		+coeffg(R,2,2,0)*ui(il(R,axis1)+2,il(R,axis2)+2,c2,c3);

	}
	else if( width(axis1,grid)==2 && width(axis2,grid)==2 )
	{
	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	      ug(R,c3)=                   
		coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
		+coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
		+coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
		+coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3);
	}
	else
	{
	  // Here is the general case
	  const intArray & il00= il(R,axis1);       // make these references for efficiency
	  const intArray & il01= il(R,axis2);

	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	    {
	      ug(R,c3)= 0;
	      for( int w2=0; w2<width(axis2,grid); w2++ )            
		for( int w1=0; w1<width(axis1,grid); w1++ )
		  ug(R,c3)+=coeffg(R,w1,w2)*ui(il00+w1,il01+w2,c2,c3);
	    }
	      
	}
      }
      else
      { // *** 3D ****
	Overture::abort("error");
      }
    }
  }
  else
  {
    ug.redim(0);
  }

  timeForExplicitInterpolation+=getCPU()-time;
  return 0;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolate a refinement level}}  
int Interpolant::
interpolateRefinementLevel( const int refinementLevel,
			    realCompositeGridFunction & u, 
			    const Range & C0 /* = nullRange */,    
			    const Range & C1 /* = nullRange */,  
			    const Range & C2 /* = nullRange */ )
//==============================================================================
// /Description:
//    Interpolate points on the boundary of a refinement level -- only interpolate
// overlapping grid points from other grids on the same refinement level.
//
// /refinementLevel (input) : interpolate this refinement level.
// /u (intput/output): fill in the interpolated values into this grid function.
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( refinementLevel<0 || refinementLevel>=cg.numberOfRefinementLevels() )
  {
    printF("Interpolant::interpolateRefinementLevel:ERROR: invalid refinementLevel=%i\n"
           " The number of refinement levels is %i\n",refinementLevel,cg.numberOfRefinementLevels());
    Overture::abort("error");
  }
  if( !interpolationIsExplicit() && implicitInterpolationMethod!=iterateToInterpolate )
  {
    printF("Interpolant::interpolateRefinementLevel:ERROR: can only interpolate when the interpolation "
           "is explicit or we iterate to solve the implicit equations. \n"
           "Either remake the grid with explicit interpolation or use setImplicitInterpolationMethod. \n");
    Overture::abort("error");
  }
  
  
  GridCollection & rl = cg.refinementLevel[refinementLevel];
  IntegerArray gridsToInterpolate(cg.numberOfComponentGrids());
  gridsToInterpolate=false;
  IntegerArray gridsToInterpolateFrom(cg.numberOfComponentGrids());
  gridsToInterpolateFrom=false;
  for( int g=0; g<rl.numberOfComponentGrids(); g++ )
  {
    int grid=rl.componentGridNumber(g);
    printF("interpolateRefinementLevel: level=%i, grid=%i\n",refinementLevel,grid);
    
    gridsToInterpolate(grid)    =true;
    gridsToInterpolateFrom(grid)=true;
  }
    
  Range C[4];
  getComponentRanges( C0,C1,C2,C,u);

  InterpolationMethodEnum oldOption=interpolationMethod;
  interpolationMethod=optimized; // we must use this option.
  int returnValue=internalInterpolate(u,C,gridsToInterpolate,gridsToInterpolateFrom);
  interpolationMethod=oldOption;
  
  return returnValue;
  
}



int Interpolant::
internalInterpolate(realCompositeGridFunction & u, 
		    const Range C[],
		    const IntegerArray & gridsToInterpolate /* = Overture::nullIntArray() */,
                    const IntegerArray & gridsToInterpolateFrom /* = Overture::nullIntArray() */ )  
//==============================================================================
// /Access: protected.
//
//    Interpolate the interpolation boundary of a CompositeGridFunction
// /gridsToInterpolate (input) : an array of length cg.numberOfComponentGrids(), which 
//       specifies to interpolate grid g if  gridsToInterpolate(g)!=0 
// /gridsToInterpolateFrom (input) : an array of length cg.numberOfComponentGrids(), which 
//       specifies to interpolate from grid g if gridsToInterpolateFrom(g)!=0 
// /u (input/output): fill in the values on the interpolation boundary using
//    other values on the grid function.
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  interpolant.interpolate(u,Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0 = success, positive value is an error.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( cg.numberOfComponentGrids()==0 )
    return 0;
   
  if( !interpolationIsInitialized )
    initializeInterpolation();
   
  const Range & C0= C[u.positionOfComponent(0)];
  if( C0.getLength()>0 )
  {
    if( C0.getBase()<u.getComponentBase(0) || C0.getBound()>u.getComponentBound(0) )
    {
      printF("Interpolant:interpolate:ERROR: Trying to interpolate components that are not there\n"
             "  Trying to interpolate components in the range  C=[%i,%i] but\n"
             "  The grid function only has components in the range [%i,%i]\n",
             C0.getBase(),C0.getBound(),u.getComponentBase(0),u.getComponentBound(0));
      Overture::abort("error");
    }
  }
  
  if( false )
  {
    int ei = ParallelUtility::getMaxValue((int)explicitInterpolation);
    if( ei!=explicitInterpolation )
    {
      const int myid=max(0,Communication_Manager::My_Process_Number);
      printf("Interpolant::internalInterpolate:ERROR: myid=%i, explicitInterpolation is not the same on"
             "all processors!\n",myid);
      OV_ABORT("error");
    }
  }

  if( explicitInterpolation ) 
  {
    return explicitInterpolate( u,C,gridsToInterpolate,gridsToInterpolateFrom );
  }
  else if( implicitInterpolationMethod==iterateToInterpolate )
  {
    return implicitInterpolateByIteration( u,C,gridsToInterpolate,gridsToInterpolateFrom );
  }
  else
  {
    real time=getCPU();
    // Interpolate each component separately
    if( u.positionOfComponent(0) < u.positionOfCoordinate(cg.numberOfDimensions()-1) )
    {
      cout << "Interpolant:interpolate:ERROR unable to interpolate grid function " << u.getName() << endl;
      cout << "The component appears before the last coordinate direction\n";
      Overture::abort("Interpolant::interpolant: fatal error");
    }
    realCompositeGridFunction v;
    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
    for( int c1=C[1].getBase(); c1<=C[1].getBound(); c1++ )
    for( int c0=C[0].getBase(); c0<=C[0].getBound(); c0++ )
    {
      v.link(u,Range(c0,c0),Range(c1,c1),Range(c2,c2));         // link to a component
      rcData->implicitInterpolant->solve( v,v );      // solve the equations
    }
    timeForImplicitInterpolation+=getCPU()-time;
    return 0;  
  }
}



int Interpolant::
interpolate( realGridCollectionFunction & ,
	     const Range & C0 /* = nullRange */,     
	     const Range & C1 /* = nullRange */, 
	     const Range & C2 /* = nullRange */ )
{
  cout << "Interpolant::interpolate: sorry, don't know how to interpolate"
    " a GridCollectionFunction! \n";
  return 1;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolationIsExplicit}}
bool Interpolant::
interpolationIsExplicit() const
//==============================================================================
// /Return Values:
//    true if the interpolation is explicit, false if implicit. 
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  return explicitInterpolation;
}
//\begin{>>InterpolateInclude.tex}{\subsubsection{interpolationIsImplicit}}
bool Interpolant::
interpolationIsImplicit() const
//==============================================================================
// /Return Values:
//    true if the interpolation is implicit, false if explicit.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  return !explicitInterpolation;
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{setExplicitInterpolationStorageOption}}
int Interpolant::
setExplicitInterpolationStorageOption( ExplicitInterpolationStorageOptionEnum option)
//==============================================================================
// /Description:
//    Define the storage option to use for explicit interpolation (or implicit iterative interpolation)
// There is a tradeoff between storage and the number of operations required to determine
// the interpolated values. For wider interpolation stencils one may want to use less storage.
// For quadratic interpolation (w=3) in 3D (d=3) the storage is not bad, 27 values per interpolation
// point. Interpolation on an eight-order grid with w=9 however requires 9*9*9=729 values per interpolation
// point. In this case the options requiring less storage may be better to use.
// 
// /option (input) : one of:
//    precomputeAllCoefficients   :  requires $w^d$ coefficients per interp pt (w=width of interp stencil)
//    precomputeSomeCoefficients  :  requires w*d coefficients per interp pt (d=dimension, 1,2, or 3)
//    precomputeNoCoefficients    :  requires d coefficinets per interp point
//
//
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  explicitInterpolationStorageOption=option;
  return 0;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{setImplicitInterpolationTolerance}}
int Interpolant:: 
setImplicitInterpolationTolerance(real tol)
//==============================================================================
// /Description:
//    Set the convergence tolerance for implicit interpolation when the implicit
// equations are solved by iteration.
// /tolerance (input) : tolerance on the residual of the interpolation equations.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  tolerance=tol;
  return 0;
}

int Interpolant::
setInterpolationMethod(InterpolationMethodEnum method)
// ===================================================================
// Choose between different methods -- internal use only for now
//====================================================================
{
  interpolationMethod=method;
  return 0;
 
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{setImplicitInterpolationMethod}}
int Interpolant::
setImplicitInterpolationMethod(ImplicitInterpolationMethodEnum method)
// ===================================================================
// /Description:
//   Choose between different methods 
//\end{InterpolateInclude.tex}  
//====================================================================
{
  if( implicitInterpolationMethod!=method )
  {
    interpolationIsInitialized=FALSE;
    implicitInterpolationMethod=method;
  }
  return 0;
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{setMaximumNumberOfIterations}}
int Interpolant::
setMaximumNumberOfIterations(int maximumNumberOfIterations_ ) // for iterative interpolation
// ===================================================================
// /Description:
//   Set the maximum number of iterations for iterative interpolation.
// /maximumNumberOfIterations\_ (input) : maximum number of iterations for iterative interpolation.
//\end{InterpolateInclude.tex}  
//====================================================================
{
  maximumNumberOfIterations=maximumNumberOfIterations_;
  return 0;
}



//\begin{>>InterpolateInclude.tex}{\subsubsection{getImplicitInterpolationMethod}}
Interpolant::ImplicitInterpolationMethodEnum Interpolant::
getImplicitInterpolationMethod() const
// ===================================================================
// /Description:
//   return the current method for implicit interpolation.
//\end{InterpolateInclude.tex}  
//====================================================================
{
  return implicitInterpolationMethod;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{setInterpolationOption}}
int Interpolant::
setInterpolationOption(InterpolationOptionEnum option, bool trueOrFalse )
//==============================================================================
// /Description:
//    Set an interpolation option. Set options to determine which points should be interpolated
//   (for CompositeGrid's that have refinement grids).
// /option (input) :
//   \begin{description}
//      \item[interpolateOverlappingRefinementPoints] : assign points on refinement grids that
//    interpolate from (refinement) grids belong to different base grids. These are points that are
//    usually determined by the Ogen function updateRefinement.
//      \item[interpolateAllRefinementBoundaries] : assign points on refinement boundaries (i.e. ghost lines)
//         that interpolate from other grids on the same base grid.
//      \item[interpolateHiddenRefinementPoints] : assign points on coarser levels that interpolate
//         from finer grid patches (from the same base grid).
//   \end{description}
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( option==interpolateAllRefinementBoundaries )
  {
    interpolateRefinementBoundaries=trueOrFalse;
  }
  else if( option==interpolateHiddenRefinementPoints )
  {
    interpolateHidden=trueOrFalse;
  }
  else if( option==interpolateOverlappingRefinementPoints )
  {
    interpolateOverlappingRefinementBoundaries=trueOrFalse;
  }
  else
  {
    printF("Interpolant::setInterpolationOption:ERROR: unknown option=%i\n",option);
  }
  
  return 0;
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{getInterpolationOption}}
int Interpolant::
getInterpolationOption(InterpolationOptionEnum option )
//==============================================================================
// /Description:
//    Get the value of  an interpolation option. 
// /option (input) :
//   \begin{description}
//      \item[interpolateOverlappingRefinementPoints] : assign points on refinement grids that
//    interpolate from (refinement) grids belong to different base grids. These are points that are
//    usually determined by the Ogen function updateRefinement.
//      \item[interpolateAllRefinementBoundaries] : assign points on refinement boundaries (i.e. ghost lines)
//         that interpolate from other grids on the same base grid.
//      \item[interpolateHiddenRefinementPoints] : assign points on coarser levels that interpolate
//         from finer grid patches (from the same base grid).
//   \end{description}
// /return value: value of the option.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( option==interpolateAllRefinementBoundaries )
  {
    return interpolateRefinementBoundaries;
  }
  else if( option==interpolateHiddenRefinementPoints )
  {
    return interpolateHidden;
  }
  else if( option==interpolateOverlappingRefinementPoints )
  {
    return interpolateOverlappingRefinementBoundaries;
  }
  else
  {
    printF("Interpolant::getInterpolationOption:ERROR: unknown option=%i\n",option);
  }
  
  return 0;
}


//\begin{>>InterpolateInclude.tex}{\subsubsection{setInterpolateRefinements}}
int Interpolant::
setInterpolateRefinements( InterpolateRefinements & interp )
//==============================================================================
// /Description:
//    Supply an AMR interpolation object:
//  /interp (input) : use this to interpolate refinement boundaries on adaptive grids.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( interpRefinementsWasNewed )
    delete interpRefinements;

  interpRefinementsWasNewed=false;
  interpRefinements=&interp;
  return 0;
}

//! Only interpolate grids on refinement levels that are less than or equal to a given level.
/*! This option is used by the error estimator.
 */
int Interpolant::
setMaximumRefinementLevelToInterpolate(int maxLevelToInterpolate )
{
  maximumRefinementLevelToInterpolate=maxLevelToInterpolate;
  return 0;
}

//! return the maximum refinement level to interpolate
int Interpolant::
getMaximumRefinementLevelToInterpolate() const
{
  return maximumRefinementLevelToInterpolate;
}



int Interpolant::
printStatistics( FILE *file /* = stdout */ )
// =======================================================================================
// /Description:
//     Print statistics.
// =======================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);

  if( myid==0 )
  {
    real totalTime=max(REAL_MIN*100.,timeForExplicitInterpolation+timeForImplicitInterpolation+
		       timeForIterativeImplicitInterpolation+timeForInitializeInterpolation);


    fprintf(file,"\n"
	    "==================== Interpolant Class Statistics ========================\n"
	    " time for explicit interpolation..........................%8.2e %4.1f%%\n"
	    " time for implicit interpolation..........................%8.2e %4.1f%%\n"
	    " time for iterative implicit interpolation................%8.2e %4.1f%%\n"
	    "    number of implicit interpolations = %i\n"
	    "    average number of iterations per interpolation = %8.2e\n"
	    "   time for AMR interpolation.(counted above).............%8.2e %4.1f%%\n"
	    "     time for AMR extrap refine bndrys.(counted above)....%8.2e %4.1f%%\n"
	    "     time for AMR extrap all.(counted above)..............%8.2e %4.1f%%\n"
	    "     time for AMR extrap interp neigh.(counted above).....%8.2e %4.1f%%\n"
	    "     time for AMR refinement bndrys.(counted above),,.....%8.2e %4.1f%%\n"
	    " time for setup interpoilation............................%8.2e %4.1f%%\n"
	    " total....................................................%8.2e %4.1f%%\n",
	    timeForExplicitInterpolation,timeForExplicitInterpolation*100./totalTime,
	    timeForImplicitInterpolation,timeForImplicitInterpolation*100./totalTime,
	    timeForIterativeImplicitInterpolation,timeForIterativeImplicitInterpolation*100./totalTime,
	    numberOfImplicitInterpolations,numberOfImplicitIterations/real(max(1,numberOfImplicitInterpolations)),
	    timeForAMRInterpolation,timeForAMRInterpolation*100./totalTime,
	    timeForAMRExtrapolateRefinementBoundaries,timeForAMRExtrapolateRefinementBoundaries*100./totalTime,
	    timeForAMRExtrapolateAll,timeForAMRExtrapolateAll*100./totalTime,
	    timeForAMRExtrapInterpolationNeighbours,timeForAMRExtrapInterpolationNeighbours*100./totalTime,
	    timeForAMRRefinementBoundaries,timeForAMRRefinementBoundaries*100./totalTime,
	    timeForInitializeInterpolation,timeForInitializeInterpolation*100./totalTime,
	    totalTime,totalTime*100./totalTime );
    
  }
  
  return 0;
}

int Interpolant::
printMyStatistics( FILE *file /* = stdout */ )
// =======================================================================================
// /Description:
//     Print statistics for this Interpolant as well as the statistics for all Interpolants
// =======================================================================================
{
  printStatistics( file );
  if( rcData!=NULL && rcData->implicitInterpolant!=NULL )
  {
    fprintf(file," ==== Here is the implicit solver used by the Interpolant:\n");
    rcData->implicitInterpolant->printStatistics(file);
  }
  
  return 0;
}


real Interpolant::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object (bytes) 
//\end{GenericGridCollectionOperatorsInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);

  for( int grid=0; grid<coeff.getLength(); grid++ )
    size+=coeff[grid].elementCount()*sizeof(real);      // coeff's for explicit interpolation

  size+=(width.elementCount())*sizeof(int);
  
  if( rcData->implicitInterpolant!=NULL )
  {
    size+=rcData->implicitInterpolant->sizeOf();
  }
  return size;
}


#define interpOptRes EXTERN_C_NAME(interpoptres)
#define interpOpt EXTERN_C_NAME(interpopt)

extern "C"
{

  void interpOptRes( const int&nd,
		  const int& ndui1a,const int& ndui1b,const int& ndui2a,const int& ndui2b,
		  const int& ndui3a,const int& ndui3b,const int& ndui4a,const int& ndui4b,
		  const int& ndug1a,const int& ndug1b,const int& ndug2a,const int& ndug2b,
		  const int& ndug3a,const int& ndug3b,const int& ndug4a,const int& ndug4b,
		  const int& ndil,const int&ndip,const int&ndc1,const int&ndc2,const int&ndc3,
		  const int& ipar,
		  const real& ui,real& ug,const real& c,real& r,const int& il,const int& ip,
		  const int& varWidth, const int& width, real& resMax );

  void interpOpt( const int&nd,
		  const int& ndui1a,const int& ndui1b,const int& ndui2a,const int& ndui2b,
		  const int& ndui3a,const int& ndui3b,const int& ndui4a,const int& ndui4b,
		  const int& ndug1a,const int& ndug1b,const int& ndug2a,const int& ndug2b,
		  const int& ndug3a,const int& ndug3b,const int& ndug4a,const int& ndug4b,
		  const int& ndil,const int&ndip,const int&ndc1,const int&ndc2,const int&ndc3,
		  const int& ipar,
		  const real& ui,real& ug,const real& c,const int& il,const int& ip,
		  const int& varWidth, const int& width );
}

#define INTERPOLATE_THIS_GRID(grid) ((!onlyInterpolateSomeGrids || gridsToInterpolate(grid)) && \
                cg.refinementLevelNumber(grid)<=maximumRefinementLevelToInterpolate )



#define UINDEX(g,i1,i2,i3,c1) ((i1)+d0[g]*( (i2)+d1[g]*( (i3)+d2[g]*( c1))))
#define UG(i1,i2,i3,c1)  ug_[base[grid]+UINDEX(grid,i1,i2,i3,c1)]
#define IP(i,axis) ip_[(i)+numberOfInterpolationPoints*(axis)]
#define IL(i,axis) il_[(i)+numberOfInterpolationPoints*(axis)]
#define IG(i) ig_[(i)]
#define COEFFG2(i,w1,w2) coeffg_[(i)+numberOfInterpolationPoints*((w1)+width0*((w2)))]
#define COEFFG3(i,w1,w2,w3) coeffg_[(i)+numberOfInterpolationPoints*((w1)+width0*((w2)+width1*(w3)))]
// we assume base of all grid functions are the same:
#define UU(grid,i1,i2,i3,c1)  u_[grid][base[grid]+UINDEX(grid,i1,i2,i3,c1)]
#define UU2(w1,w2) UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,c2,c3)
#define UU3(w1,w2,w3) UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,IL(i,axis3)+w3,c3)

int Interpolant::
explicitInterpolate(realCompositeGridFunction & u,
		    const Range C[],
                    const IntegerArray & gridsToInterpolate /* = Overture::nullIntArray() */,
                    const IntegerArray & gridsToInterpolateFrom /* = Overture::nullIntArray() */ ) const
//===================================================================================
// /Description:
//    Explicit Interpolation
// /gridToInterpolate (input): optionally supply a list of grids to interpolate.
// /allowInterpolationFromThisGrid (input) : optionally supply this array that specifies
//    allowable grids to interpolate from (grid g can be used as an interpolee grid if
//      allowInterpolationFromThisGrid(g)==true )
//
//===================================================================================
{
  real time=getCPU();
  
  assert( interpolationIsInitialized );
  
  const bool onlyInterpolateSomeGrids=gridsToInterpolate.getLength(0)>0;
  // ***** only update appropriate components:
  // u.periodicUpdate(C[3]);   // do this since we don't wrap the interpolation stencil for periodic boundaries.
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( INTERPOLATE_THIS_GRID(grid) ) 
    {
      u[grid].periodicUpdate(C[3]); 
      u[grid].updateGhostBoundaries();  // *wdh* 060302 this updates all components, does this matter?
    }
  }
  
//   if( true )
//   {
//     MPI_Barrier(MPI_COMM_WORLD);
//   }

  Overture::checkMemoryUsage("Interpolant::explicitInterpolate:after periodic and updateGhost");  

  if( cg.numberOfComponentGrids()==1 && cg.numberOfInterpolationPoints(0) <= 0) 
    return 0;

  if( cg.numberOfDimensions()==1 )
  {
    if( u.getNumberOfComponents()>3 )
    {
      cout << "explicitInterpolate:ERROR: cannot interpolate a grid function with more than 3 component indices\n";
      cout << "(i.e. I can do scalar, vector and matrix and 3-tensor grid functions) \n";
      Overture::abort("error");
    }
  }
  else if( cg.numberOfDimensions()==2 )
  {
    if( u.getNumberOfComponents()>2 )
    {
      cout << "explicitInterpolate:ERROR: cannot interpolate a grid function with more than 2 component indices\n";
      cout << "(i.e. I can do scalar, vector and matrix grid functions) \n";
      Overture::abort("error");
    }
  }
  else if( cg.numberOfDimensions()==3 )
  {
    if( u.getNumberOfComponents()>1 )
    {
      cout << "explicitInterpolate:ERROR: cannot interpolate a grid function with more than 1 component indices\n";
      cout << "(i.e. I can do scalar and vector grid functions) \n";
      Overture::abort("error");
    }
  }


  real **u_=NULL;
  int *base=NULL, *d0=NULL, *d1=NULL, *d2=NULL;
  if( interpolationMethod==optimizedC )
  {
    #ifdef USE_PPP
      Overture::abort("Interpolant::ERROR: this option not valid in parallel");
    #endif

    u_ = new real * [cg.numberOfComponentGrids()]; 
    base= new int [cg.numberOfComponentGrids()];
    d0  = new int [cg.numberOfComponentGrids()];
    d1=   new int [cg.numberOfComponentGrids()];
    d2=   new int [cg.numberOfComponentGrids()];

    for( int g=0; g<cg.numberOfComponentGrids(); g++ )
    {
      u_[g]= &u[g](u[g].getBase(0),u[g].getBase(1),u[g].getBase(2),u[g].getBase(3)); // u[g].getDataPointer();
      d0[g]= u[g].getLength(0);
      d1[g]= u[g].getLength(1);
      d2[g]= u[g].getLength(2);
      base[g]=-UINDEX(g,u[g].getBase(0),u[g].getBase(1),u[g].getBase(2),u[g].getBase(3));
    }
  }
  
  const bool onlyInterpolateFromSomeGrids=gridsToInterpolateFrom.getLength(0)>0;
  const bool restrictedInterpolation=onlyInterpolateSomeGrids || onlyInterpolateFromSomeGrids;
  if( restrictedInterpolation )
  {
    if( debug & 1 ) printF("**Interpolant: restricted interpolation for some grids ***\n");
    
    assert( interpolationMethod==optimized );
  }

  // *** note ** we do not apply amrInterpolation if we are only interpolating some grids
  const bool amrInterpolation=!restrictedInterpolation && cg.numberOfRefinementLevels()>1 && 
                              (interpolateRefinementBoundaries || interpolateHidden);

  if( amrInterpolation && interpRefinements==NULL )
  {
    Interpolant & interp = (Interpolant&)(*this); // cast away const
    interp.interpRefinements = new InterpolateRefinements(cg.numberOfDimensions());
    interp.interpRefinements->setOrderOfInterpolation(2);     // ****
    interp.interpRefinementsWasNewed=true;
  }
  if( amrInterpolation && u.getOperators()==NULL )
  {
    printF("Interpolate::ERROR: you must supply operators to u before you can interpolate\n"
           "                    on an adaptive grid. \n");
    Overture::abort("error");
  }


  if( amrInterpolation )
  {
    if( interpolateHidden )
    {
      real time0=getCPU();
      // interpRefinements->interpolateCoarseFromFine( u,InterpolateRefinements::allLevels,C[3] ); 
      if( cg.numberOfRefinementLevels()<= maximumRefinementLevelToInterpolate+1 )
        interpRefinements->interpolateCoarseFromFine( u,InterpolateRefinements::allLevels,C[3] ); 
      else
      {
        // for( int level=0; level<maximumRefinementLevelToInterpolate; level++ )
        for( int level=maximumRefinementLevelToInterpolate-1; level>=0; level-- ) // *wdh* 020928
          interpRefinements->interpolateCoarseFromFine( u,level,C[3] ); 
      }

      Overture::checkMemoryUsage("Interpolant::explicitInterpolate:after interpolateCoarseFromFine");  

      // **** note: we could avoid this next call if we copied parallel ghost points in
      // Interpolate.bC -- interpolateCoarseFromFineMacro  --> to-do
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        u[grid].updateGhostBoundaries(); // ********** 060423
      }
      

      time0=getCPU()-time0;
      timeForAMRCoarseFromFine+=time0;
      timeForAMRInterpolation+=time0;

    }

    if( interpolateRefinementBoundaries )
    {
      real time0=getCPU();
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        if( INTERPOLATE_THIS_GRID(grid) )
	{
	  // printF("Interpolant: grid=%i, bcParams.ghostLineToAssign=%i\n",grid,bcParams.ghostLineToAssign);
	  // bcParams.ghostLineToAssign=1;
          u[grid].applyBoundaryCondition(C[3],BCTypes::extrapolateRefinementBoundaries,
					 BCTypes::allBoundaries,0.,0.,bcParams);
	}
      }
      time0=getCPU()-time0;
      timeForAMRExtrapolateRefinementBoundaries+=time0;
      timeForAMRInterpolation+=time0;
    }
  }


  if( false )
  {
    // ****** Here we check the consistency of the sorted interpolation points ****
    for(int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      intArray & ig = cg.interpoleeGrid[grid];
      int num=0;
      for( int gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      {
	if( cg.interpolationStartEndIndex(0,grid,gridi)>=0 )
	  num+=cg.interpolationStartEndIndex(1,grid,gridi)-cg.interpolationStartEndIndex(0,grid,gridi)+1;
	else
	{
	  if( cg.interpolationStartEndIndex(0,grid,gridi)!= -1 )
	  {
	    printF("***ERROR in interpolationStartEndIndex : negative value not equal to -1!\n");
            cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
	    Overture::abort("error");
	  }
	}
      }
      if( num!=cg.numberOfInterpolationPoints(grid) )
      {
	printF("***ERROR grid=%i num=%i numberOfInterpolationPoints(grid)=%i\n",grid,num,
              cg.numberOfInterpolationPoints(grid));
        cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");
        cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
        ig.display("ig");
        Overture::abort("error");
      }
      
      for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      {
	int gridi=ig(i);
	if( i<cg.interpolationStartEndIndex(0,grid,gridi) || i>cg.interpolationStartEndIndex(1,grid,gridi) )
	{
	  printF("***ERROR in interpolationStartEndIndex: grid=%i gridi=%i i=%i start=%i end=%i\n",
		 grid,gridi,i,cg.interpolationStartEndIndex(0,grid,gridi),
		 cg.interpolationStartEndIndex(1,grid,gridi));
          cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
	  
          Overture::abort("error");
	}
      }
    }
  }

 #ifdef USE_PPP

  // *************************************************
  // *** here is where we interpolate in parallel ****
  // *************************************************

  // assert( !onlyInterpolateSomeGrids ); // this option not implemented yet
  Overture::checkMemoryUsage("Interpolant::explicitInterpolate:before parallel interpolate");  

  // NOTE: If you change this next section then you should also change the section in implicitInterpolateByIteration
  if( initializeParallelInterpolator || rcData->parallelInterpolator==NULL )
  {
    if( rcData->parallelInterpolator==NULL )
    {
      rcData->parallelInterpolator = new ParallelOverlappingGridInterpolator();
    }

    rcData->parallelInterpolator->updateToMatchGrid(u);  // this will call setup

    bool & initParallelInterpolator = (bool&)initializeParallelInterpolator; // cast away const
    initParallelInterpolator=false;
  }
  rcData->parallelInterpolator->setMaximumRefinementLevelToInterpolate( maximumRefinementLevelToInterpolate );
  rcData->parallelInterpolator->interpolate(u,gridsToInterpolate,gridsToInterpolateFrom,C[0],C[1],C[2]);

  Overture::checkMemoryUsage("Interpolant::explicitInterpolate:after parallel interpolate");  

 #else

  // *************************************
  // *** serial explicit interpolation ***
  // *************************************

  for(grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {

//     printF("Explicit interp: grid=%i ni=%i interpThisGrid=%i\n",grid,cg.numberOfInterpolationPoints(grid),
//                       INTERPOLATE_THIS_GRID(grid));
    
    if( cg.numberOfInterpolationPoints(grid) <= 0 || !INTERPOLATE_THIS_GRID(grid) ) // (onlyInterpolateSomeGrids && !gridsToInterpolate(grid)) )
      continue;
    
    intArray & ip = cg.interpolationPoint[grid];    // use define?
    intArray & il = cg.interpoleeLocation[grid];
    intArray & ig = cg.interpoleeGrid[grid];
    intArray & varWidth = cg.variableInterpolationWidth[grid];
    
    if( Oges::debug & 8 )
    {
      cg.interpolationWidth.display("Here is the interpolationWidth");
      ip.display("explicitInterpolate: Here is the ip array");
      il.display("explicitInterpolate: Here is the il array");
      ig.display("explicitInterpolate: Here is the ig array");
      cg.interpolationCoordinates[grid].display("Here are the interpolationCoordinates");
      coeff[grid].display("explicitInterpolate: Here is the coeff array");
    }    
    RealDistributedArray & ug = u[grid];
    RealDistributedArray & coeffg = coeff[grid];
    Index I(0,cg.numberOfInterpolationPoints(grid));

    // ********* fix this when there are more Index's *******************************
    if( cg.numberOfDimensions()==1 )
    {  // ****** 1D **************
      if( u.positionOfComponent(0)==1 || u.positionOfComponent(0)==2 || u.positionOfComponent(0)==3 )
      {
	int c1,c2,c3;
	for( c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	  for( c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	    for( c1=C[1].getBase(); c1<=C[1].getBound(); c1++ )
	    {
	      ug(ip(I,axis1),c1,c2,c3)=0.;   
	      for( int w1=0; w1<width(axis1,grid); w1++ )
		for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		  ug(ip(i,axis1),c1,c2,c3)+=coeffg(i,w1)*u[ig(i)](il(i,axis1)+w1,c1,c2,c3);
	    }
      }
      else
      {
	cout << "explicitInterpolate:ERROR: cannot interpolate in 1D when position of component 0 is = " << 
	  "u.positionOfComponent(0) \n";
	cout << "complain to Bill! \n";
	Overture::abort("error");
      }
      continue;
    }
  
    if( !( (cg.numberOfDimensions()==2 && u.positionOfComponent(0)==2) || u.positionOfComponent(0)==3) )
    {
      cout << "explicitInterpolate:ERROR: cannot interpolate when position of component 0 is = " << 
	u.positionOfComponent(0) << "\n";
      cout << "complain to Bill! \n";
      Overture::abort("error");
    }
    
    if( interpolationMethod==optimizedC )
    {
      const int numberOfInterpolationPoints = cg.numberOfInterpolationPoints(grid);

      real *ug_ = u_[grid];
      const real *coeffg_ = coeffg.getDataPointer();

      const int *ip_ = ip.getDataPointer();
      const int *il_ = il.getDataPointer();
      const int *ig_ = ig.getDataPointer();
      const int width0=coeffg.getLength(1);
      const int width1=coeffg.getLength(2);
	
	
      assert( ip.getLength(0)==numberOfInterpolationPoints );
      assert( il.getLength(0)==numberOfInterpolationPoints );
      assert( coeffg.getLength(0)==numberOfInterpolationPoints );

      if( cg.numberOfDimensions()==2 )
      {
	// ****** 2D **************
	
	for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	{
	    
	  for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	  {

	    int i;
	    if( width(axis1,grid)==3 && width(axis2,grid)==3 )
	    {
	      for( i=0; i<numberOfInterpolationPoints; i++ )
	      {
		UG(IP(i,axis1),IP(i,axis2),c2,c3)=
		  COEFFG2(i,0,0)*UU2(0,0)+COEFFG2(i,1,0)*UU2(1,0)+COEFFG2(i,2,0)*UU2(2,0)+
		  COEFFG2(i,0,1)*UU2(0,1)+COEFFG2(i,1,1)*UU2(1,1)+COEFFG2(i,2,1)*UU2(2,1)+
		  COEFFG2(i,0,2)*UU2(0,2)+COEFFG2(i,1,2)*UU2(1,2)+COEFFG2(i,2,2)*UU2(2,2);
	      }
	    }
	    else
	    {
	      for( i=0; i<numberOfInterpolationPoints; i++ )
		UG(IP(i,axis1),IP(i,axis2),c2,c3)=0.;   

	      for( int w2=0; w2<width(axis2,grid); w2++ )              // ***** optimize these *******
	      {
		for( int w1=0; w1<width(axis1,grid); w1++ )
		{
		  for( i=0; i<numberOfInterpolationPoints; i++ )
		  {
		    UG(IP(i,axis1),IP(i,axis2),c2,c3)+=
		      COEFFG2(i,w1,w2)*UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,c2,c3);
		  }
		    
		}
	      }
	    }
	  }
	}
      }
      else // 3D
      {
	if( u.positionOfComponent(0)==3 )
	{
	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	  {

	    int i;
	    if( width(axis1,grid)==3 && width(axis2,grid)==3 && width(axis3,grid)==3 )
	    {
	      for( i=0; i<numberOfInterpolationPoints; i++ )
	      {
		UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)=
		  COEFFG3(i,0,0,0)*UU3(0,0,0)+COEFFG3(i,1,0,0)*UU3(1,0,0)+COEFFG3(i,2,0,0)*UU3(2,0,0)+
		  COEFFG3(i,0,1,0)*UU3(0,1,0)+COEFFG3(i,1,1,0)*UU3(1,1,0)+COEFFG3(i,2,1,0)*UU3(2,1,0)+
		  COEFFG3(i,0,2,0)*UU3(0,2,0)+COEFFG3(i,1,2,0)*UU3(1,2,0)+COEFFG3(i,2,2,0)*UU3(2,2,0)+

		  COEFFG3(i,0,0,1)*UU3(0,0,1)+COEFFG3(i,1,0,1)*UU3(1,0,1)+COEFFG3(i,2,0,1)*UU3(2,0,1)+
		  COEFFG3(i,0,1,1)*UU3(0,1,1)+COEFFG3(i,1,1,1)*UU3(1,1,1)+COEFFG3(i,2,1,1)*UU3(2,1,1)+
		  COEFFG3(i,0,2,1)*UU3(0,2,1)+COEFFG3(i,1,2,1)*UU3(1,2,1)+COEFFG3(i,2,2,1)*UU3(2,2,1)+

		  COEFFG3(i,0,0,2)*UU3(0,0,2)+COEFFG3(i,1,0,2)*UU3(1,0,2)+COEFFG3(i,2,0,2)*UU3(2,0,2)+
		  COEFFG3(i,0,1,2)*UU3(0,1,2)+COEFFG3(i,1,1,2)*UU3(1,1,2)+COEFFG3(i,2,1,2)*UU3(2,1,2)+
		  COEFFG3(i,0,2,2)*UU3(0,2,2)+COEFFG3(i,1,2,2)*UU3(1,2,2)+COEFFG3(i,2,2,2)*UU3(2,2,2);
	      }
	    }
	    else
	    {
	      for( i=0; i<numberOfInterpolationPoints; i++ )
		UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)=0.;   

	      for( int w3=0; w3<width(axis3,grid); w3++ )
		for( int w2=0; w2<width(axis2,grid); w2++ )              // ***** optimize these *******
		  for( int w1=0; w1<width(axis1,grid); w1++ )
		    for( i=0; i<numberOfInterpolationPoints; i++ )
		    {
		      UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)+=
			COEFFG3(i,w1,w2,w3)*UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,IL(i,axis3)+w3,c3);
		    }
	      
	    }
	    
	  }
	}
      }

    }
    else if( interpolationMethod==optimized ) 
    {
      // use optimized explicit interpolation, this assumes that the points have been
      // ordered in the interpolation arrays to be in increasing order of the interpolee grid.
      // printF("Interpolant: use optimized explicit interpolation\n");
	  
      assert( useVariableWidthInterpolation!=NULL );

      Range R;
      int numInterpolated=0; // keep track of the number of points interpolated for a consistency check
      for( int gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      {
	if( cg.interpolationStartEndIndex(0,grid,gridi) >= 0 &&
              (!onlyInterpolateFromSomeGrids || gridsToInterpolateFrom(gridi) ) )
	{
         if( debug & 1 ) 
             printF("explicitInterp:interpOpt: interpolate grid %i from grid %i points=[%i,%i] width(0,grid)=%i,%i\n",
             grid,gridi,cg.interpolationStartEndIndex(0,grid,gridi),cg.interpolationStartEndIndex(1,grid,gridi),
             width(0,grid),width(1,grid));
 
	  R=Range(cg.interpolationStartEndIndex(0,grid,gridi),cg.interpolationStartEndIndex(1,grid,gridi));
          numInterpolated+=R.getLength();
	  
	  const realArray & ui = u[ig(cg.interpolationStartEndIndex(0,grid,gridi))];


	  if( true )
	  {
            int ipar[]={R.getBase(),
                        R.getBound(),
			C[2].getBase(),
			C[2].getBound(),
			C[3].getBase(),
			C[3].getBound(), 
                        (int)explicitInterpolationStorageOption,
                        useVariableWidthInterpolation[grid]}; //
	    interpOpt( cg.numberOfDimensions(),
		       ui.getBase(0),ui.getBound(0),ui.getBase(1),ui.getBound(1),
		       ui.getBase(2),ui.getBound(2),ui.getBase(3),ui.getBound(3),
		       ug.getBase(0),ug.getBound(0),ug.getBase(1),ug.getBound(1),
		       ug.getBase(2),ug.getBound(2),ug.getBase(3),ug.getBound(3),
		       il.getLength(0),ip.getLength(0),
		       coeffg.getLength(0),coeffg.getLength(1),coeffg.getLength(2),
		       ipar[0],
		       // *ui.getDataPointer(),
		       ui(ui.getBase(0),ui.getBase(1),ui.getBase(2),ui.getBase(3),ui.getBase(4)),
		       // *ug.getDataPointer(),
		       ug(ug.getBase(0),ug.getBase(1),ug.getBase(2),ug.getBase(3),ug.getBase(4)),
		       *coeffg.getDataPointer(),
		       *il.getDataPointer(),*ip.getDataPointer(),*varWidth.getDataPointer(),
		       width(0,grid) );
	  }
          else
	  {
	    // old opt version

	    if( debug & 1 )
	    {
	      if( max(abs(ig(R)-gridi))!=0 )
	      {
		printF("explicitInterpolate:ERROR: ig(R)!=gridi=%i\n",gridi);
		display(ig(R),"ig(R)");
	      }
	    }
	  

	    if( cg.numberOfDimensions()==2 )
	    {
	      if( width(axis1,grid)==3 && width(axis2,grid)==3 )
	      {

// @PD realArray4[ug,ui,coeffg] Range[R] intArray2[ip,il]

		// printF("opt:PA\n");
		  
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		    ug(ip(R,axis1),ip(R,axis2),c2,c3)=                      // @PA
		      coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
		      +coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
		      +coeffg(R,2,0,0)*ui(il(R,axis1)+2,il(R,axis2)  ,c2,c3)
		      +coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
		      +coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3) 
		      +coeffg(R,2,1,0)*ui(il(R,axis1)+2,il(R,axis2)+1,c2,c3) 
		      +coeffg(R,0,2,0)*ui(il(R,axis1)  ,il(R,axis2)+2,c2,c3) 
		      +coeffg(R,1,2,0)*ui(il(R,axis1)+1,il(R,axis2)+2,c2,c3) 
		      +coeffg(R,2,2,0)*ui(il(R,axis1)+2,il(R,axis2)+2,c2,c3);

	      }
	      else if( width(axis1,grid)==2 && width(axis2,grid)==2 )
	      {
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		    ug(ip(R,axis1),ip(R,axis2),c2,c3)=                      // @PA
		      coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
		      +coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
		      +coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
		      +coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3);
	      }
	      else
	      {
		// Here is the general case
		const intArray & il00= il(R,axis1);       // make these references for efficiency
		const intArray & il01= il(R,axis2);

		const intArray & ip0 = ip(R,axis1);
		const intArray & ip1 = ip(R,axis2);
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		  {
		    ug(ip0,ip1,c2,c3)= 0;
		    for( int w2=0; w2<width(axis2,grid); w2++ )            
		      for( int w1=0; w1<width(axis1,grid); w1++ )
			ug(ip(R,axis1),ip(R,axis2),c2,c3)+=coeffg(R,w1,w2)*ui(il00+w1,il01+w2,c2,c3);
		  }
	      
	      }
	    }
	    else
	    { // *** 3D ****
	      if( width(axis1,grid)==3 && width(axis2,grid)==3 && width(axis3,grid)==3 )
	      {
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)=                      // @PA
		    coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)  ,c3)
		    +coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)  ,c3)
		    +coeffg(R,2,0,0)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)  ,c3)
		    +coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)  ,c3)
		    +coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)  ,c3) 
		    +coeffg(R,2,1,0)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)  ,c3) 
		    +coeffg(R,0,2,0)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)  ,c3) 
		    +coeffg(R,1,2,0)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)  ,c3) 
		    +coeffg(R,2,2,0)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)  ,c3)
		    +coeffg(R,0,0,1)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)+1,c3)
		    +coeffg(R,1,0,1)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)+1,c3)
		    +coeffg(R,2,0,1)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)+1,c3)
		    +coeffg(R,0,1,1)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)+1,c3)
		    +coeffg(R,1,1,1)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)+1,c3) 
		    +coeffg(R,2,1,1)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)+1,c3) 
		    +coeffg(R,0,2,1)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)+1,c3) 
		    +coeffg(R,1,2,1)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)+1,c3) 
		    +coeffg(R,2,2,1)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)+1,c3)
		    +coeffg(R,0,0,2)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)+2,c3)
		    +coeffg(R,1,0,2)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)+2,c3)
		    +coeffg(R,2,0,2)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)+2,c3)
		    +coeffg(R,0,1,2)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)+2,c3)
		    +coeffg(R,1,1,2)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)+2,c3) 
		    +coeffg(R,2,1,2)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)+2,c3) 
		    +coeffg(R,0,2,2)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)+2,c3) 
		    +coeffg(R,1,2,2)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)+2,c3) 
		    +coeffg(R,2,2,2)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)+2,c3);

	      }
	      else if( width(axis1,grid)==2 && width(axis2,grid)==2 && width(axis3,grid)==2 )
	      {
		const intArray & il00=il(R,axis1);    
		const intArray & il10=evaluate(il00+1);
		const intArray & il01=il(R,axis2);
		const intArray & il11=evaluate(il01+1);
		const intArray & il02=il(R,axis3); 
		const intArray & il12=evaluate(il02+1);

		const intArray & ip0 = ip(R,axis1);
		const intArray & ip1 = ip(R,axis2);
		const intArray & ip2 = ip(R,axis3);
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		{
		  ug(ip0,ip1,ip2,c3)= 
		    coeffg(R,0,0,0)*ui(il00,il01,il02,c3)
		    +coeffg(R,1,0,0)*ui(il10,il01,il02,c3)
		    +coeffg(R,0,1,0)*ui(il00,il11,il02,c3)
		    +coeffg(R,1,1,0)*ui(il10,il11,il02,c3)
		    +coeffg(R,0,0,1)*ui(il00,il01,il12,c3)
		    +coeffg(R,1,0,1)*ui(il10,il01,il12,c3)
		    +coeffg(R,0,1,1)*ui(il00,il11,il12,c3)
		    +coeffg(R,1,1,1)*ui(il10,il11,il12,c3);
		}
	      }
	      else
	      {
		const intArray & il00=il(R,axis1);    
		const intArray & il01=il(R,axis2);
		const intArray & il02=il(R,axis3); 

		const intArray & ip0 = ip(R,axis1);
		const intArray & ip1 = ip(R,axis2);
		const intArray & ip2 = ip(R,axis3);
		for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		{
		  ug(ip0,ip1,ip2,c3)=0.;
		  for( int w3=0; w3<width(axis3,grid); w3++ )            
		    for( int w2=0; w2<width(axis2,grid); w2++ )            
		      for( int w1=0; w1<width(axis1,grid); w1++ )
			ug(ip0,ip1,ip2,c3)+=coeffg(R,w1,w2,w3)*ui(il00+w1,il01+w2,il02+w3,c3);
		}
	      }
	    }
	  }
	}
      } // end for grid i
      // consistency check: *wdh* 090429
      if( !onlyInterpolateFromSomeGrids && numInterpolated!=cg.numberOfInterpolationPoints(grid) )
      {
	printF("Interpolant:explicitInterpolate:ERROR: grid=%i numInterpolated=%i but numberOfInterpolationPoints=%i\n"
               " There is probably an error in the interpolationStartEndIndex array!\n",
                           grid,numInterpolated,cg.numberOfInterpolationPoints(grid) );
	display(cg.interpolationStartEndIndex,"cg.interpolationStartEndIndex(0:1,grid,gridi)");
	display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
	
	OV_ABORT("error");
      }
    }
    else
    {
      //  ========================  old way  ==================================
      if( cg.numberOfDimensions()==2 )
      {// ****** 2D **************
	for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	  for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	  {
	    ug(ip(I,axis1),ip(I,axis2),c2,c3)=0.;   
	    for( int w2=0; w2<width(axis2,grid); w2++ )              // ***** optimize these *******
	      for( int w1=0; w1<width(axis1,grid); w1++ )
		for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		  ug(ip(i,axis1),ip(i,axis2),c2,c3)+=coeffg(i,w1,w2)*
		    u[ig(i)](il(i,axis1)+w1,il(i,axis2)+w2,c2,c3);
	  }
      }
      else // 3D
      {
	if( u.positionOfComponent(0)==3 )
	{
	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	  {
	    int i;
	    for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	      u[grid](ip(i,axis1),ip(i,axis2),ip(i,axis3),c3)=0.;
	    for( int w3=0; w3<width(axis3,grid); w3++ )
	      for( int w2=0; w2<width(axis2,grid); w2++ )
		for( int w1=0; w1<width(axis1,grid); w1++ )
		  for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
		    u[grid](ip(i,axis1),ip(i,axis2),ip(i,axis3),c3)+=coeff[grid](i,w1,w2,w3)*
		      u[ig(i)](il(i,axis1)+w1,il(i,axis2)+w2,il(i,axis3)+w3,c3);
	  }
	}
      }
    }
  }
 #endif

  if( interpolationMethod==optimizedC )
  {
    delete [] u_;
    delete [] base;
    delete [] d0;
    delete [] d1;
    delete [] d2;
  }

  if( amrInterpolation && interpolateRefinementBoundaries )
  {
    real time0=getCPU();
    
//    u.applyBoundaryCondition(C[3],BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams);
//    u.applyBoundaryCondition(C[3],BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,0.,bcParams);

//    u.finishBoundaryConditions(bcParams,C[3]);
//    interpRefinements->interpolateRefinementBoundaries( u,InterpolateRefinements::allLevels,C[3] );

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( INTERPOLATE_THIS_GRID(grid) )
      {
	// if( true ) printF(" ---- Interpolant:explicitInterpolate: extrapolateInterpolationNeighbours -----\n");
	

        real time1=getCPU();
	u[grid].applyBoundaryCondition(C[3],BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams);
        real time2=getCPU();
	timeForAMRExtrapolateAll+=time2-time1;
	u[grid].applyBoundaryCondition(C[3],BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,0.,
				       bcParams);
	timeForAMRExtrapInterpolationNeighbours+=getCPU()-time2;
	u[grid].finishBoundaryConditions(bcParams,C[3]);

      }
    }
      
    // printF("implicitInterpolate it=%2i call interpolateRefinementBoundaries\n",it);
    real time1=getCPU();
    if( cg.numberOfRefinementLevels()<= maximumRefinementLevelToInterpolate+1 )
      interpRefinements->interpolateRefinementBoundaries( u,InterpolateRefinements::allLevels,C[3] ); 
    else
    {
      for( int level=1; level<=maximumRefinementLevelToInterpolate; level++ )
	interpRefinements->interpolateRefinementBoundaries( u,level,C[3] );
    }
    timeForAMRRefinementBoundaries+=getCPU()-time1;

    timeForAMRInterpolation+=getCPU()-time0;

  }
  
  if( !(amrInterpolation && interpolateRefinementBoundaries) )
  {
    u.periodicUpdate(C[3]); // ** not necessary for amr 
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( INTERPOLATE_THIS_GRID(grid) )
	u[grid].updateGhostBoundaries();  // *wdh* 060302 this updates all components, does this matter?
    }
    
  }
      

  timeForExplicitInterpolation+=getCPU()-time;
  return 0;
}


int Interpolant::
implicitInterpolateByIteration(realCompositeGridFunction & u,
			       const Range C[],
			       const IntegerArray & gridsToInterpolate /* = Overture::nullIntArray() */,
			       const IntegerArray & gridsToInterpolateFrom /* = Overture::nullIntArray() */ ) const
// ===================================================================================================
// /Description:
//   Iterate to solve the implicit interpolation equations.
// ===================================================================================================
{
  real time=getCPU();
  
  assert( interpolationIsInitialized );

  // printF("Interpolant:implicitInterpolateByIteration\n");
    

  const bool onlyInterpolateSomeGrids=gridsToInterpolate.getLength(0)>0;
  const bool onlyInterpolateFromSomeGrids=gridsToInterpolateFrom.getLength(0)>0;
  const bool restrictedInterpolation=onlyInterpolateSomeGrids || onlyInterpolateFromSomeGrids;
  if( restrictedInterpolation )
  {
    // *************** fix this ******************
    if( false && interpolationMethod!=optimized )
    {
      printF("Interpolant: Setting interpolationMethod==optimized since were are only interpolating to/from "
             " some sub-set of the grids\n");
      InterpolationMethodEnum & im = (InterpolationMethodEnum&)interpolationMethod; // cast away const
      im=optimized;
    }
  }

  // u.periodicUpdate(C[3]);   // do this since we don't wrap the interpolation stencil for periodic boundaries.
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    if( INTERPOLATE_THIS_GRID(grid) ) // !onlyInterpolateSomeGrids || gridsToInterpolate(grid) )
    {
      u[grid].periodicUpdate(C[3]); 
      u[grid].updateGhostBoundaries();  // *wdh* 060302 this updates all components, does this matter?
    }
  
  if( cg.numberOfComponentGrids()==1 && cg.numberOfInterpolationPoints(0) <= 0 ) 
    return 0;

  if( cg.numberOfDimensions()==1 )
  {
    if( u.getNumberOfComponents()>3 )
    {
      cout << "implicitInterpolate:ERROR: cannot interpolate a grid function with more than 3 component indices\n";
      cout << "(i.e. I can do scalar, vector and matrix and 3-tensor grid functions) \n";
      Overture::abort("error");
    }
    if( !(u.positionOfComponent(0)==1 || u.positionOfComponent(0)==2 || u.positionOfComponent(0)==3) )
    {
      cout << "implicitInterpolate:ERROR: cannot interpolate in 1D when position of component 0 is = " << 
	"u.positionOfComponent(0) \n";
      cout << "complain to Bill! \n";
      Overture::abort("error");
    }
    
  }
  else if( cg.numberOfDimensions()==2 )
  {
    if( u.getNumberOfComponents()>2 )
    {
      cout << "implicitInterpolate:ERROR: cannot interpolate a grid function with more than 2 component indices\n";
      cout << "(i.e. I can do scalar, vector and matrix grid functions) \n";
      Overture::abort("error");
    }
  }
  else if( cg.numberOfDimensions()==3 )
  {
    if( u.getNumberOfComponents()>1 )
    {
      cout << "implicitInterpolate:ERROR: cannot interpolate a grid function with more than 1 component indices\n";
      cout << "(i.e. I can do scalar and vector grid functions) \n";
      Overture::abort("error");
    }
  }
  if( !( (cg.numberOfDimensions()==2 && u.positionOfComponent(0)==2) || u.positionOfComponent(0)==3) )
  {
    cout << "implicitInterpolate:ERROR: cannot interpolate when position of component 0 is = " << 
      u.positionOfComponent(0) << "\n";
    cout << "complain to Bill! \n";
    Overture::abort("error");
  }


  // *** note ** we do not apply amrInterpolation if we are only interpolating some grids
  const bool amrInterpolation=!restrictedInterpolation && cg.numberOfRefinementLevels()>1 && 
                              (interpolateRefinementBoundaries || interpolateHidden);

  if( amrInterpolation && interpRefinements==NULL )
  {
    Interpolant & interp = (Interpolant&)(*this); // cast away const
    interp.interpRefinements = new InterpolateRefinements(cg.numberOfDimensions());
    interp.interpRefinements->setOrderOfInterpolation(2);     // ****

    interp.interpRefinementsWasNewed=true;
  }
  if( amrInterpolation && u.getOperators()==NULL )
  {
    printF("Interpolant::ERROR: you must supply operators to u before you can interpolate\n"
           "                    on an adaptive grid. \n");
    Overture::abort("error");
  }

  

  real **u_ = NULL;
  int *base= NULL;
  int *d0  = NULL;
  int *d1=   NULL;
  int *d2=   NULL;
  if( interpolationMethod==optimizedC || interpolationMethod==standard)
  {
    #ifdef USE_PPP
      Overture::abort("Interpolant::ERROR: this option not valid in parallel");
    #endif

    u_ = new real * [cg.numberOfComponentGrids()]; 
    base= new int [cg.numberOfComponentGrids()];
    d0  = new int [cg.numberOfComponentGrids()];
    d1=   new int [cg.numberOfComponentGrids()];
    d2=   new int [cg.numberOfComponentGrids()];

    for( int g=0; g<cg.numberOfComponentGrids(); g++ )
    {
      u_[g]= &u[g](u[g].getBase(0),u[g].getBase(1),u[g].getBase(2),u[g].getBase(3)); // u[g].getDataPointer();
      d0[g]= u[g].getLength(0);
      d1[g]= u[g].getLength(1);
      d2[g]= u[g].getLength(2);
      base[g]=-UINDEX(g,u[g].getBase(0),u[g].getBase(1),u[g].getBase(2),u[g].getBase(3));
    }
  }
  
  if( false ) // ************ TESTING 2012/07/05
  {
    // ****** Here we check the consistency of the sorted interpolation points ****
    for(int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      intArray & ig = cg.interpoleeGrid[grid];
      int num=0;
      for( int gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
      {
	if( cg.interpolationStartEndIndex(0,grid,gridi)>=0 )
	  num+=cg.interpolationStartEndIndex(1,grid,gridi)-cg.interpolationStartEndIndex(0,grid,gridi)+1;
	else
	{
	  if( cg.interpolationStartEndIndex(0,grid,gridi)!= -1 )
	  {
	    printF("***ERROR in interpolationStartEndIndex : negative value not equal to -1!\n");
            cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
	    Overture::abort("error");
	  }
	}
      }
      if( num!=cg.numberOfInterpolationPoints(grid) )
      {
	printF("***ERROR grid=%i num=%i numberOfInterpolationPoints(grid)=%i\n",grid,num,
              cg.numberOfInterpolationPoints(grid));
        cg.numberOfInterpolationPoints.display("cg.numberOfInterpolationPoints");
        cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
        ig.display("ig");
        Overture::abort("error");
      }
      
      for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
      {
	int gridi=ig(i);
	if( i<cg.interpolationStartEndIndex(0,grid,gridi) || i>cg.interpolationStartEndIndex(1,grid,gridi) )
	{
	  printF("***ERROR in interpolationStartEndIndex: grid=%i gridi=%i i=%i start=%i end=%i\n",
		 grid,gridi,i,cg.interpolationStartEndIndex(0,grid,gridi),
		 cg.interpolationStartEndIndex(1,grid,gridi));
          cg.interpolationStartEndIndex.display("cg.interpolationStartEndIndex");
	  
          Overture::abort("error");
	}
      }
    }
  }



  // worst convergence rate is about .25, .25^(20) = 10^{-12}
  //  const int maximumNumberOfIterations=25; 
  real resMax, resMaxOld=1.;

//   printF(" Interpolant: interpolationMethod=%s\n",(interpolationMethod==optimizedC ?
// 		  "opt C" : interpolationMethod==standard? "standard" : "opt"));
  int it;
  for(it=0; it<maximumNumberOfIterations; it++ )
  {
    if( it!=0 )
    {
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        if( INTERPOLATE_THIS_GRID(grid) ) // !onlyInterpolateSomeGrids || gridsToInterpolate(grid) )
	{
  	  u[grid].periodicUpdate(C[3]); 
          u[grid].updateGhostBoundaries();  // this updates all components, does this matter?
 	}
    }
    
    
    if( amrInterpolation && interpolateHidden )
    {
      real time0=getCPU();
      if( cg.numberOfRefinementLevels()<= maximumRefinementLevelToInterpolate+1 )
        interpRefinements->interpolateCoarseFromFine( u,InterpolateRefinements::allLevels,C[3] ); 
      else
      {
        // for( int level=0; level<maximumRefinementLevelToInterpolate; level++ )
        for( int level=maximumRefinementLevelToInterpolate-1; level>=0; level-- ) // *wdh* 020928
          interpRefinements->interpolateCoarseFromFine( u,level,C[3] ); 
      }

      // **** note: we could avoid this next call if we copied parallel ghost points in
      // Interpolate.bC -- interpolateCoarseFromFineMacro  --> to-do
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        u[grid].updateGhostBoundaries(); // ********** 060423
      }

      time0=getCPU()-time0;
      timeForAMRCoarseFromFine+=time0;
      timeForAMRInterpolation+=time0;
    }

    resMax=0.;
    // for implicit interpolation we need to compute a residual

   #ifdef USE_PPP
  
    // *************************************************
    // *** here is where we interpolate in parallel ****
    // *************************************************

    // assert( !restrictedInterpolation ); // this option not implemented yet
    
    // *wdh* 100417 -- fix made so POGI is re-intialized if the grid changes
    // NOTE: If you change this next section then you should also change the section in explicitInterpolate
    if( initializeParallelInterpolator || rcData->parallelInterpolator==NULL )
    {
      if( debug & 2 )
      {
	printF("Interpolant:INFO: using iteration to solve the interpolation equations in parallel, maxit=%i.\n",
	       maximumNumberOfIterations);
      }
      
      if( rcData->parallelInterpolator==NULL )
      {
	rcData->parallelInterpolator = new ParallelOverlappingGridInterpolator();
      }
      
      // *wdh* 100417 rcData->parallelInterpolator->setup(u);
      rcData->parallelInterpolator->updateToMatchGrid(u);  // this will call setup
      rcData->parallelInterpolator->turnOnResidualComputation(true);

      bool & initParallelInterpolator = (bool&)initializeParallelInterpolator; // cast away const
      initParallelInterpolator=false;
    }
    

    rcData->parallelInterpolator->setMaximumRefinementLevelToInterpolate( maximumRefinementLevelToInterpolate );
    rcData->parallelInterpolator->interpolate(u,gridsToInterpolate,gridsToInterpolateFrom,C[0],C[1],C[2]);
  
    // do this for now -- always do 5 iterations  **** fix this ****
    // resMax = it<5 ? 1. : 0.;
    resMax = rcData->parallelInterpolator->getMaximumResidual();

    
   #else

    // *************************************
    // *** serial explicit interpolation ***
    // *************************************
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( cg.numberOfInterpolationPoints(grid) <= 0 || !INTERPOLATE_THIS_GRID(grid)) // (onlyInterpolateSomeGrids && !gridsToInterpolate(grid)) )
	continue;
    
      const int numberOfInterpolationPoints = cg.numberOfInterpolationPoints(grid);
      realArray r(numberOfInterpolationPoints);

      intArray & ip = cg.interpolationPoint[grid];    // use define?
      intArray & il = cg.interpoleeLocation[grid];
      intArray & ig = cg.interpoleeGrid[grid];
      intArray & varWidth = cg.variableInterpolationWidth[grid];
      RealDistributedArray & coeffg = coeff[grid];
    
      assert( ip.getLength(0)==numberOfInterpolationPoints );
      assert( il.getLength(0)==numberOfInterpolationPoints );
      assert( coeffg.getLength(0)==numberOfInterpolationPoints );

      RealDistributedArray & ug = u[grid];
      Index I(0,cg.numberOfInterpolationPoints(grid));


      // ********* fix this when there are more Index's *******************************
      if( true && (interpolationMethod==optimizedC || interpolationMethod==standard) )
      {

	real *ug_ = u_[grid];
	const real *coeffg_ = coeffg.getDataPointer();

	const int *ip_ = ip.getDataPointer();
	const int *il_ = il.getDataPointer();
	const int *ig_ = ig.getDataPointer();
	const int width0=coeffg.getLength(1);
	const int width1=coeffg.getLength(2);
	
	if( cg.numberOfDimensions()==1 )
	{  // ****** 1D **************
	  int c1,c2,c3;
	  for( c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	    for( c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	      for( c1=C[1].getBase(); c1<=C[1].getBound(); c1++ )
	      {
		ug(ip(I,axis1),c1,c2,c3)=0.;   
		for( int w1=0; w1<width(axis1,grid); w1++ )
		  for( int i=0; i<numberOfInterpolationPoints; i++ )
		    ug(ip(i,axis1),c1,c2,c3)+=coeffg(i,w1)*u[ig(i)](il(i,axis1)+w1,c1,c2,c3);
	      }
	}
	else if( cg.numberOfDimensions()==2 )
	{
	  // ****** 2D **************
	
	  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	  {
	    
	    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
	    {

	      int i;
	      if( width(axis1,grid)==3 && width(axis2,grid)==3 )
	      {
		for( i=0; i<numberOfInterpolationPoints; i++ )
		{
		  r(i)=
		    COEFFG2(i,0,0)*UU2(0,0)+COEFFG2(i,1,0)*UU2(1,0)+COEFFG2(i,2,0)*UU2(2,0)+
		    COEFFG2(i,0,1)*UU2(0,1)+COEFFG2(i,1,1)*UU2(1,1)+COEFFG2(i,2,1)*UU2(2,1)+
		    COEFFG2(i,0,2)*UU2(0,2)+COEFFG2(i,1,2)*UU2(1,2)+COEFFG2(i,2,2)*UU2(2,2);
		  resMax=max(resMax,fabs(UG(IP(i,axis1),IP(i,axis2),c2,c3)-r(i)));
		  UG(IP(i,axis1),IP(i,axis2),c2,c3)=r(i);
		}
	      }
	      else
	      {
		r=0.;
		for( int w2=0; w2<width(axis2,grid); w2++ )              // ***** optimize these *******
		{
		  for( int w1=0; w1<width(axis1,grid); w1++ )
		  {
		    for( i=0; i<numberOfInterpolationPoints; i++ )
		    {
		      r(i)+=COEFFG2(i,w1,w2)*UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,c2,c3);
		    }
		    
		  }
		}
		for( i=0; i<numberOfInterpolationPoints; i++ )
		{
		  resMax=max(resMax,fabs(UG(IP(i,axis1),IP(i,axis2),c2,c3)-r(i)));
		  UG(IP(i,axis1),IP(i,axis2),c2,c3)=r(i);
		}
	      }
	    }
	  }
	}
	else // 3D
	{
	  if( u.positionOfComponent(0)==3 )
	  {
	    for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
	    {
	      int i;
	      if( width(axis1,grid)==3 && width(axis2,grid)==3 && width(axis3,grid)==3 )
	      {
		// printF("Interpolant: optimized 3d..\n");
		for( i=0; i<numberOfInterpolationPoints; i++ )
		{
		  r(i)=
		    COEFFG3(i,0,0,0)*UU3(0,0,0)+COEFFG3(i,1,0,0)*UU3(1,0,0)+COEFFG3(i,2,0,0)*UU3(2,0,0)+
		    COEFFG3(i,0,1,0)*UU3(0,1,0)+COEFFG3(i,1,1,0)*UU3(1,1,0)+COEFFG3(i,2,1,0)*UU3(2,1,0)+
		    COEFFG3(i,0,2,0)*UU3(0,2,0)+COEFFG3(i,1,2,0)*UU3(1,2,0)+COEFFG3(i,2,2,0)*UU3(2,2,0)+

		    COEFFG3(i,0,0,1)*UU3(0,0,1)+COEFFG3(i,1,0,1)*UU3(1,0,1)+COEFFG3(i,2,0,1)*UU3(2,0,1)+
		    COEFFG3(i,0,1,1)*UU3(0,1,1)+COEFFG3(i,1,1,1)*UU3(1,1,1)+COEFFG3(i,2,1,1)*UU3(2,1,1)+
		    COEFFG3(i,0,2,1)*UU3(0,2,1)+COEFFG3(i,1,2,1)*UU3(1,2,1)+COEFFG3(i,2,2,1)*UU3(2,2,1)+

		    COEFFG3(i,0,0,2)*UU3(0,0,2)+COEFFG3(i,1,0,2)*UU3(1,0,2)+COEFFG3(i,2,0,2)*UU3(2,0,2)+
		    COEFFG3(i,0,1,2)*UU3(0,1,2)+COEFFG3(i,1,1,2)*UU3(1,1,2)+COEFFG3(i,2,1,2)*UU3(2,1,2)+
		    COEFFG3(i,0,2,2)*UU3(0,2,2)+COEFFG3(i,1,2,2)*UU3(1,2,2)+COEFFG3(i,2,2,2)*UU3(2,2,2);
		  resMax=max(resMax,fabs(UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)-r(i)));
		  UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)=r(i);
		}
	      }
	      else
	      {
		r=0.;
		for( int w3=0; w3<width(axis3,grid); w3++ )
		{
		  for( int w2=0; w2<width(axis2,grid); w2++ )              // ***** optimize these *******
		  {
		    for( int w1=0; w1<width(axis1,grid); w1++ )
		    {
		      for( i=0; i<numberOfInterpolationPoints; i++ )
		      {
			r(i)+=COEFFG3(i,w1,w2,w3)*UU(IG(i), IL(i,axis1)+w1,IL(i,axis2)+w2,IL(i,axis3)+w3,c3);
		      }
		    
		    }
		  }
		}
		for( i=0; i<numberOfInterpolationPoints; i++ )
		{
		  resMax=max(resMax,fabs(UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)-r(i)));
		  UG(IP(i,axis1),IP(i,axis2),IP(i,axis3),c3)=r(i);
		}
	      }
	    
	    }
	  }
	}
      }
      else
      {
	// optimized version
        // printF(" Interpolant: opt version\n");

        assert( useVariableWidthInterpolation!=NULL );
	
	Range R;
        const int endIndex= it==0 ? 1 : 2;  // after first time thru only do implicit points.

	int numInterpolated=0; // for a consistency check -- compare numInterpolated to numberOfInterpolationPoints
	for( int gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
	{
          // note: a cgrid can interpolate from itself
          if( false ) 
            printF("opt: it=%i grid=%i gridi=%i startIndex=%i end=%i (end for implicit=%i)\n",it,grid,gridi,
            cg.interpolationStartEndIndex(0,grid,gridi), cg.interpolationStartEndIndex(1,grid,gridi), 
            cg.interpolationStartEndIndex(2,grid,gridi));
	  
          if( cg.interpolationStartEndIndex(endIndex,grid,gridi) >= 0 )
	  { // for a consistency check -- compare numInterpolated to numberOfInterpolationPoints
	    numInterpolated+=cg.interpolationStartEndIndex(endIndex,grid,gridi)-
	      cg.interpolationStartEndIndex(0,grid,gridi)+1;
	  }
	  

	  if( cg.interpolationStartEndIndex(endIndex,grid,gridi) >= 0 && 
                (!onlyInterpolateFromSomeGrids || gridsToInterpolateFrom(gridi) ) )
	  {
	    R=Range(cg.interpolationStartEndIndex(0,grid,gridi),cg.interpolationStartEndIndex(endIndex,grid,gridi));
            // printF("opt: it=%i, grid=%i gridi=%i n=%i\n",it,grid,gridi,R.getLength());
	    
	    const realArray & ui = u[ig(cg.interpolationStartEndIndex(0,grid,gridi))];

//             realArray r2;
//             real resMax2=resMax;
// 	    r2=r;
//             r2=-1.;
            if( true ) 
	    {
	      int ipar[]={R.getBase(),
			  R.getBound(),
			  C[2].getBase(),
			  C[2].getBound(),
			  C[3].getBase(),
			  C[3].getBound(), 
			  (int) explicitInterpolationStorageOption,
                          useVariableWidthInterpolation[grid]}; //

	      interpOptRes( cg.numberOfDimensions(),
			 ui.getBase(0),ui.getBound(0),ui.getBase(1),ui.getBound(1),
			 ui.getBase(2),ui.getBound(2),ui.getBase(3),ui.getBound(3),
			 ug.getBase(0),ug.getBound(0),ug.getBase(1),ug.getBound(1),
			 ug.getBase(2),ug.getBound(2),ug.getBase(3),ug.getBound(3),
			 il.getLength(0),ip.getLength(0),
			 coeffg.getLength(0),coeffg.getLength(1),coeffg.getLength(2),
			    ipar[0],
			 // *ui.getDataPointer(),
                         ui(ui.getBase(0),ui.getBase(1),ui.getBase(2),ui.getBase(3),ui.getBase(4)),
			 // *ug.getDataPointer(),
                         ug(ug.getBase(0),ug.getBase(1),ug.getBase(2),ug.getBase(3),ug.getBase(4)),
			 *coeffg.getDataPointer(),
			 *r.getDataPointer(),
			 *il.getDataPointer(),*ip.getDataPointer(),*varWidth.getDataPointer(),
			 width(0,grid),resMax );

	    }
            else 
            {

// @PD realArray4[ug,ui,coeffg,r] Range[R] intArray2[ip,il]
	      if( cg.numberOfDimensions()==2 )
	      {
		if( width(axis1,grid)==3 && width(axis2,grid)==3 )
		{
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		    {
		      r(R,0,0,0)=                                                        // @PA
			coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
			+coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
			+coeffg(R,2,0,0)*ui(il(R,axis1)+2,il(R,axis2)  ,c2,c3)
			+coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
			+coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3) 
			+coeffg(R,2,1,0)*ui(il(R,axis1)+2,il(R,axis2)+1,c2,c3) 
			+coeffg(R,0,2,0)*ui(il(R,axis1)  ,il(R,axis2)+2,c2,c3) 
			+coeffg(R,1,2,0)*ui(il(R,axis1)+1,il(R,axis2)+2,c2,c3) 
			+coeffg(R,2,2,0)*ui(il(R,axis1)+2,il(R,axis2)+2,c2,c3);

		      resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),c2,c3)-r(R))));

		      ug(ip(R,axis1),ip(R,axis2),c2,c3)= r(R,0,0,0);                     // @PA
		    }
		}
		else if( width(axis1,grid)==2 && width(axis2,grid)==2 )
		{
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		    {
		      r(R,0,0,0)=                                                        // @PA
			coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,c2,c3)
			+coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,c2,c3)
			+coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,c2,c3)
			+coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,c2,c3);

		      resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),c2,c3)-r(R))));

		      ug(ip(R,axis1),ip(R,axis2),c2,c3)= r(R,0,0,0);                     // @PA
		    }
		}
		else
		{
		  // general case in 2D
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		    for( int c2=C[2].getBase(); c2<=C[2].getBound(); c2++ )
		    {
		      r=0.;
		      for( int w2=0; w2<width(axis2,grid); w2++ )              
		      {
			for( int w1=0; w1<width(axis1,grid); w1++ )
			{
			  r(R,0,0,0)+=coeffg(R,w1,w2,0)*ui(il(R,axis1)+w1,il(R,axis2)+w2,c2,c3); 
			}
		      }
		      resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),c2,c3)-r(R))));

		      ug(ip(R,axis1),ip(R,axis2),c2,c3)= r(R,0,0,0);                   
		    }
		}
	      }
	      else
	      { // *** 3D ****
		if( width(axis1,grid)==3 && width(axis2,grid)==3 && width(axis3,grid)==3 )
		{
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  {
		    r(R,0,0,0)=                       // @PA
		      coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)  ,c3)
		      +coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)  ,c3)
		      +coeffg(R,2,0,0)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)  ,c3)
		      +coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)  ,c3)
		      +coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)  ,c3) 
		      +coeffg(R,2,1,0)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)  ,c3) 
		      +coeffg(R,0,2,0)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)  ,c3) 
		      +coeffg(R,1,2,0)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)  ,c3) 
		      +coeffg(R,2,2,0)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)  ,c3)
		      +coeffg(R,0,0,1)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)+1,c3)
		      +coeffg(R,1,0,1)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)+1,c3)
		      +coeffg(R,2,0,1)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)+1,c3)
		      +coeffg(R,0,1,1)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)+1,c3)
		      +coeffg(R,1,1,1)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)+1,c3) 
		      +coeffg(R,2,1,1)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)+1,c3) 
		      +coeffg(R,0,2,1)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)+1,c3) 
		      +coeffg(R,1,2,1)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)+1,c3) 
		      +coeffg(R,2,2,1)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)+1,c3)
		      +coeffg(R,0,0,2)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)+2,c3)
		      +coeffg(R,1,0,2)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)+2,c3)
		      +coeffg(R,2,0,2)*ui(il(R,axis1)+2,il(R,axis2)  ,il(R,axis3)+2,c3)
		      +coeffg(R,0,1,2)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)+2,c3)
		      +coeffg(R,1,1,2)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)+2,c3) 
		      +coeffg(R,2,1,2)*ui(il(R,axis1)+2,il(R,axis2)+1,il(R,axis3)+2,c3) 
		      +coeffg(R,0,2,2)*ui(il(R,axis1)  ,il(R,axis2)+2,il(R,axis3)+2,c3) 
		      +coeffg(R,1,2,2)*ui(il(R,axis1)+1,il(R,axis2)+2,il(R,axis3)+2,c3) 
		      +coeffg(R,2,2,2)*ui(il(R,axis1)+2,il(R,axis2)+2,il(R,axis3)+2,c3);

		    resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)-r(R))));
		
		    ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)=r(R,0,0,0);  // @PA
		  }
		}
		else if( width(axis1,grid)==2 && width(axis2,grid)==2 && width(axis3,grid)==2 )
		{
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  {
		    r(R,0,0,0)=                       // @PA
		      coeffg(R,0,0,0)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)  ,c3)
		      +coeffg(R,1,0,0)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)  ,c3)
		      +coeffg(R,0,1,0)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)  ,c3)
		      +coeffg(R,1,1,0)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)  ,c3) 
		      +coeffg(R,0,0,1)*ui(il(R,axis1)  ,il(R,axis2)  ,il(R,axis3)+1,c3)
		      +coeffg(R,1,0,1)*ui(il(R,axis1)+1,il(R,axis2)  ,il(R,axis3)+1,c3)
		      +coeffg(R,0,1,1)*ui(il(R,axis1)  ,il(R,axis2)+1,il(R,axis3)+1,c3)
		      +coeffg(R,1,1,1)*ui(il(R,axis1)+1,il(R,axis2)+1,il(R,axis3)+1,c3);

		    resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)-r(R))));
		
		    ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)=r(R,0,0,0);  // @PA
		  }
		}
		else
		{
		  // general case in 3D
		  for( int c3=C[3].getBase(); c3<=C[3].getBound(); c3++ )
		  {
		    r=0.;
		    for( int w3=0; w3<width(axis3,grid); w3++ )              
		    {
		      for( int w2=0; w2<width(axis2,grid); w2++ )              
		      {
			for( int w1=0; w1<width(axis1,grid); w1++ )
			{
			  r(R,0,0,0)+=coeffg(R,w1,w2,w3)*ui(il(R,axis1)+w1,il(R,axis2)+w2,il(R,axis3)+w3,c3); 
			}
		      }
		    }
		    resMax=max(resMax,max(fabs(ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)-r(R))));

		    ug(ip(R,axis1),ip(R,axis2),ip(R,axis3),c3)= r(R,0,0,0);                   
		  }

		}
	      }
	    }
//             real err=max(fabs(r-r2));
// 	    printF("******Error in interpOpt: %8.2e, resMax=%e, resmax2=%e \n",err,resMax,resMax2);
// 	    r.display("r");
// 	    r2.display("r2");
	    
	  }
	}  // end for gridi


        // ==================================================================
        // === Only check for it==0 that we have interpolated all points. ===
        // === For it>0 only implicit pts are interpolated.               ===
        // ==================================================================
	if( it==0 && numInterpolated!=numberOfInterpolationPoints )
	{
	  printF("Interpolate:implicitInterpolateByIteration: Consistency ERROR: \n"
                 "   numInterpolated=%i is NOT equal to numberOfInterpolationPoints=%i for grid=%i\n"
                 "   There is likely a mistake in cg.interpolationStartEndIndex\n",
                  numInterpolated,numberOfInterpolationPoints,grid);
	  for( int gridi=0; gridi<cg.numberOfComponentGrids(); gridi++ )
	  {
	    if( grid!=gridi )
	      printF("opt: it=%i grid=%i gridi=%i startIndex=%i end=%i (end=%i for implicit pts)\n",it,grid,gridi,
		     cg.interpolationStartEndIndex(0,grid,gridi), cg.interpolationStartEndIndex(1,grid,gridi),
                     cg.interpolationStartEndIndex(2,grid,gridi));
	  }
          ::display(cg.interpolationStartEndIndex,"cg.interpolationStartEndIndex","%6i");
	  OV_ABORT("error");
	}
      }
      
      
    }
   #endif
    
    if( amrInterpolation && interpolateRefinementBoundaries )
    {
      // interpolate refinement boundaries may use un-used points next to interpolation points!
      real time0=getCPU();
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        if( INTERPOLATE_THIS_GRID(grid) )
	{
          real time1=getCPU();
	  u[grid].applyBoundaryCondition(C[3],BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,bcParams);
	  real time2=getCPU();
	  timeForAMRExtrapolateAll+=time2-time1;
	  u[grid].applyBoundaryCondition(C[3],BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,0.,
					 bcParams);
	  timeForAMRExtrapInterpolationNeighbours+=getCPU()-time2;
	  u[grid].finishBoundaryConditions(bcParams,C[3]);
	}
      }
      
      // printF("implicitInterpolate it=%2i call interpolateRefinementBoundaries\n",it);
      real time1=getCPU();
      if( cg.numberOfRefinementLevels()<= maximumRefinementLevelToInterpolate+1 )
        interpRefinements->interpolateRefinementBoundaries( u,InterpolateRefinements::allLevels,C[3] ); 
      else
      {
        for( int level=0; level<=maximumRefinementLevelToInterpolate; level++ )
          interpRefinements->interpolateRefinementBoundaries( u,level,C[3] );
      }
      timeForAMRRefinementBoundaries+=getCPU()-time1;

      timeForAMRInterpolation+=getCPU()-time0;
    }

    if( Mapping::debug & 2 )
      printF("implicitInterpolate: it=%2i resMax=%8.2e ratio=%6.3f\n",it,resMax,resMax/max(REAL_MIN,resMaxOld));
    resMaxOld=resMax;
    if( resMax<tolerance )
      break;
  } // end for( it )

  numberOfImplicitInterpolations++;
  numberOfImplicitIterations+=it+1;

  if( resMax > tolerance && maximumNumberOfIterations==25 )
  {
    printF("Interpolant:WARNING: no convergence in implicit interpolation iteration, %i its, max residual=%e\n",
           maximumNumberOfIterations,resMax);
  }
  if(Mapping::debug & 2 )
    printF("implicitInterpolate: it=%2i resMax=%8.2e \n",it,resMax);
  
  delete [] u_;
  delete [] base;
  delete [] d0;
  delete [] d1;
  delete [] d2;
	

  if( !(amrInterpolation && interpolateRefinementBoundaries) )
  {
    // *wdh* 060309 u.periodicUpdate(C[3]);
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( INTERPOLATE_THIS_GRID(grid) ) // !onlyInterpolateSomeGrids || gridsToInterpolate(grid) )
      {
	u[grid].periodicUpdate(C[3]); 
	u[grid].updateGhostBoundaries();  // *wdh* 060302 this updates all components, does this matter?
      }
    }
  }
  
  timeForIterativeImplicitInterpolation+=getCPU()-time;

  return 0;
}

#define initExplicitInterp EXTERN_C_NAME(initexplicitinterp)
extern "C"
{
  void initExplicitInterp(const int&ndc1,const int&ndc2,const int&ndc3,const int&ndci,
          const int&ipar,real&coeff,const real&ci,real&pr,real&ps,real&pt,
          const real&gridSpacing,const int&indexStart,
	  const int&variableInterpolationWidth,const int&interpoleeLocation,const int&interpoleeGrid);
}

int Interpolant::
initializeExplicitInterpolation()
//===================================================================
// /Description:
//   Pre-compute Interpolation coefficients for explicit interpolation
//===================================================================
{
  
#define Q11(x) (1.-(x))
#define Q21(x) (x)

#define Q12(x) .5*((x)-1.)*((x)-2.)
#define Q22(x) (x)*(2.-(x))
#define Q32(x) .5*(x)*((x)-1.)

  if( cg.numberOfBaseGrids() ==0 || max(cg.numberOfInterpolationPoints) <= 0 )
    return 0;

  #ifdef USE_PPP
   initializeParallelInterpolator=true;
   return 0;  // parallel case is handled elsewhere
  #endif

  if( Mapping::debug & 1 )
  {
    if( interpolationIsExplicit() )
      cout << "Interpolant: initialize explicit interpolation...\n";
    else
      cout << "Interpolant: initialize iterative implicit interpolation...\n";
  }
  

//   if( TRUE || interpolationMethod==optimized )
//     reorderInterpolationPoints(cg,interpolationStartIndex,interpolationEndIndex);
  real time0=getCPU();

  const int numberOfDimensions=cg.numberOfDimensions();

  // for now we use only one width per grid
  int axis,grid;
  width.redim(3,cg.numberOfComponentGrids()); width=1;
  Range NG(0,cg.numberOfComponentGrids()-1);
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  for( axis=axis1; axis<numberOfDimensions; axis++ ) 
    width(axis,grid)=max(width(axis,grid),max(cg.interpolationWidth(axis,grid,NG)));

  const int maxWidth=max(width);

  bool useOpt=true;
  if( useOpt )
  {

    while( coeff.getLength() > cg.numberOfComponentGrids() )   // remove excess items from the list
      coeff.deleteElement();  

    IntegerArray indexStart(3,cg.numberOfComponentGrids());
    RealArray gridSpacing(3,cg.numberOfComponentGrids());
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      for( axis=0; axis<3; axis++ )
      {
	indexStart(axis,grid)=mg.gridIndexRange(0,axis);
	gridSpacing(axis,grid)=mg.gridSpacing(axis);
      }
      
    }
    
    delete [] useVariableWidthInterpolation;
    useVariableWidthInterpolation = new int [cg.numberOfComponentGrids()];

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {

      if( grid >= coeff.getLength() ||
	  cg.refinementLevelNumber(grid) >= min(1,updateForAdaptiveGrid) ) // **** fix for updateForAdaptiveGrid>1
      {
	MappedGrid & mg = cg[grid];

        int ni=cg.numberOfInterpolationPoints(grid);
	if( ni==0 ) continue;
	
        Range R=ni;
	while( coeff.getLength()<=grid )
	{
	  // RealDistributedArray cl(R,width(axis1,grid),width(axis2,grid),width(axis3,grid));
	  RealDistributedArray cl;
	  coeff.addElement(cl);
	}

        // allocate space according to the storage option.
        if( explicitInterpolationStorageOption==precomputeAllCoefficients )
	{
	  coeff[grid].redim(R,width(axis1,grid),width(axis2,grid),width(axis3,grid));
	}
	else if( explicitInterpolationStorageOption==precomputeSomeCoefficients )
	{
	  coeff[grid].redim(R,width(axis1,grid),numberOfDimensions,1);
	}
	else 
	{
	  coeff[grid].redim(R,numberOfDimensions,1,1);
	}
	

        int ipar[7]={numberOfDimensions,
                    grid,
                    ni,
                    mg.isCellCentered(0),
                    (int)explicitInterpolationStorageOption,
                    maxWidth,
                    0}; //  This last position is saved for a return value of useVariableWidthInterpolation

        realArray & cc = coeff[grid];
        // cc=0.; // ********
	
        realArray & ci = cg.interpolationCoordinates[grid];
        RealArray pr(R),ps(R),pt(R);
	initExplicitInterp(cc.getLength(0),cc.getLength(1),cc.getLength(2),ci.getLength(0),
			   ipar[0], 
                           *cc.getDataPointer(),
                           *ci.getDataPointer(),pr(0),ps(0),pt(0),gridSpacing(0,0),indexStart(0,0),
			   *(cg.variableInterpolationWidth[grid].getDataPointer()),
                           *(cg.interpoleeLocation[grid].getDataPointer()),
                           *(cg.interpoleeGrid[grid].getDataPointer()));
	
         useVariableWidthInterpolation[grid]=ipar[6]; 

	 // useVariableWidthInterpolation[grid]=1;
	 
        // cc.display("coeff after initExplicitInterp");
      }
    }
    real time=getCPU()-time0;
    time0=getCPU();
    if( Mapping::debug & 1 ) printF("*** time for new init explicit = %8.2e\n",time);

    if( Mapping::debug & 1 )
    {
      printF(" **** initializeExplicitInterpolation: useVariableWidthInterpolation=");
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	printF(" %i, ",useVariableWidthInterpolation[grid]);
      printF(" ******\n");
    }
    
    return 0;

  }
  




  int i;
  Index I;
  Range R;
  RealDistributedArray q,px,qq;   // ******* may not want distributed ****

  // Allocate space for grid with largest number of interpolation points
  R=Range(0,max(cg.numberOfInterpolationPoints(Range(0,cg.numberOfComponentGrids()-1)))-1);
  
  px.redim(R);
  q.redim(R,3,maxWidth);    // q holds the interpolation weigths
  // *wdh* q=1.;

  while( coeff.getLength() > cg.numberOfComponentGrids() )   // remove excess items from the list
    coeff.deleteElement();  

  int m1,m2,m3;
  
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {

    if( grid >= coeff.getLength() ||
        cg.refinementLevelNumber(grid) >= min(1,updateForAdaptiveGrid) ) // **** fix for updateForAdaptiveGrid>1
    {
      MappedGrid & cgrid = cg[grid];


      I=R=Range(0,cg.numberOfInterpolationPoints(grid)-1);

      q(I,nullRange,nullRange)=0.;  // set to zero for variable width interpolation.

      const intArray & variableInterpolationWidth = cg.variableInterpolationWidth[grid];
      const bool widthIsConstant=min(variableInterpolationWidth)==maxWidth;

    //.........First form 1D interpolation coefficients
      int indexPosition,gridi;
      real relativeOffset;
      for( axis=axis1; axis<numberOfDimensions; axis++ ) 
      {
	for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	{
	  gridi = cg.interpoleeGrid[grid](i);   // **** could vectorize this loop since list is sorted by interpolee
	  MappedGrid & cgridi = cg[gridi];
	  indexPosition=cg.interpoleeLocation[grid](i,axis);
	  relativeOffset=cg.interpolationCoordinates[grid](i,axis)/cgridi.gridSpacing(axis)
	    +cgridi.indexRange(Start,axis);
	  px(i)= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5 
	    : relativeOffset-indexPosition;
//	if( width(axis,grid) < interpWidth(axis,grid,gridi) )
//	{
//	  //......interpolation width less than maximum allowed
//	  if( px(i) > width(axis,grid)/2. )
//	  {
//	    int ipx=min(int(px(i)-(width(axis,grid)-2)/2.),interpWidth(axis,grid,gridi)-width(axis,grid));
//	    px(i)-=ipx;
//	  }
//	}
	}
	if( widthIsConstant )
	{
	  switch (width(axis,grid))
	  {
	  case 3:
	    //........quadratic interpolation
	    q(I,axis,0)=Q12(px(I));
	    q(I,axis,1)=Q22(px(I));
	    q(I,axis,2)=Q32(px(I));
	    break;
	  case 2:
	    //.......linear interpolation
	    q(I,axis,0)=Q11(px(I));
	    q(I,axis,1)=Q21(px(I));
	    break;
	  default:
	    // .....order >3 - compute lagrange interpolation
	    for(m1=0; m1<width(axis,grid); m1++ ) 
	    {
	      q(I,axis,m1)=1.;
	      for( m2=0; m2<width(axis,grid); m2++ )
		if( m1 != m2  )
		  q(I,axis,m1)*=(px(I)-m2)/(m1-m2);
	    }
	  }
	}
	else
	{
	  // printF(" Interpolant: **** variableInterpolationWidth **** \n");
	  for( i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
	  {
	    switch( variableInterpolationWidth(i) )
	    {
	    case 3:
	      //........quadratic interpolation
	      q(i,axis,0)=Q12(px(i));
	      q(i,axis,1)=Q22(px(i));
	      q(i,axis,2)=Q32(px(i));
	      break;
	    case 2:
	      //.......linear interpolation
	      q(i,axis,0)=Q11(px(i));
	      q(i,axis,1)=Q21(px(i));
	      break;
	    default:
	      // .....order >3 - compute lagrange interpolation
	      for(m1=0; m1<width(axis,grid); m1++ ) 
	      {
		q(i,axis,m1)=1.;
		for( m2=0; m2<width(axis,grid); m2++ )
		  if( m1 != m2  )
		    q(i,axis,m1)*=(px(i)-m2)/(m1-m2);
	      }
	    }


	  }
	}
      
      }
      //.......Now form the interpolation coefficients
  
    // * coeff.addElement( *(new RealArray(R,width(axis1,grid),width(axis2,grid),width(axis3,grid))) );
      if( coeff.getLength()<=grid )
      {
        RealDistributedArray cl(R,width(axis1,grid),width(axis2,grid),width(axis3,grid));
        coeff.addElement(cl);
      }
      else
      {
        coeff[grid].redim(R,width(axis1,grid),width(axis2,grid),width(axis3,grid));
      }
      if( numberOfDimensions==2 )
      {
	for( m3=0; m3< width(axis3,grid); m3++ ) 
	  for( m2=0; m2< width(axis2,grid); m2++ ) 
	    for( m1=0; m1< width(axis1,grid); m1++ ) 
	      coeff[grid](I,m1,m2,m3)=q(I,axis1,m1)*q(I,axis2,m2);
      }
      else if( numberOfDimensions==3 )
      {
	for( m3=0; m3< width(axis3,grid); m3++ ) 
	  for( m2=0; m2< width(axis2,grid); m2++ ) 
	    for( m1=0; m1< width(axis1,grid); m1++ ) 
	      coeff[grid](I,m1,m2,m3)=q(I,axis1,m1)*q(I,axis2,m2)*q(I,axis3,m3);
      }
      else
      {
	for( m3=0; m3< width(axis3,grid); m3++ ) 
	  for( m2=0; m2< width(axis2,grid); m2++ ) 
	    for( m1=0; m1< width(axis1,grid); m1++ ) 
	      coeff[grid](I,m1,m2,m3)=q(I,axis1,m1);
      }
    

      // coeff[grid].display("initializeExplicitInterpolation: Here is the coeff array");
    }
    else
    {
      // printF("Interpolant: do not update interpolation coefficients for grid=%i (AMR grid)\n",grid);
    }
    // coeff[grid].display("coeff[grid] from old way");

  }
  
  real time=getCPU()-time0;
  printF("Time to initialize explicit method=%8.2e\n",time);
  
  return 0;
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{breakReference}}  
void Interpolant::
breakReference()
//==============================================================================
// /Description:
//    Break any references.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  // If there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    Interpolant interpolant = *this;  // makes a deep copy
    reference(interpolant);   // make a reference to this new copy
  }
}


// Assignment with = is a deep copy
Interpolant & Interpolant::
operator= ( const Interpolant & interpolant )
{
  *rcData=*interpolant.rcData;  // deep copy
  cg=interpolant.cg;
  coeff=interpolant.coeff;
  width=interpolant.width;

  interpolationMethod=interpolant.interpolationMethod;
  implicitInterpolationMethod=interpolant.implicitInterpolationMethod;
  tolerance=interpolant.tolerance;  
  explicitInterpolation=interpolant.explicitInterpolation;
  coeff=interpolant.coeff;   
  
  return *this;
}

//\begin{>>InterpolateInclude.tex}{\subsubsection{reference}}  
void Interpolant::
reference( const Interpolant & interpolant )
//==============================================================================
// /Description:
//    Reference this Interpolant to another.
// /interpolant (input): reference to this Interpolant.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( this==&interpolant ) // no need to do anything if
    return;
  if( rcData->decrementReferenceCount() == 0 )
    delete rcData;   
  rcData=interpolant.rcData;
  rcData->incrementReferenceCount();
  cg.reference(interpolant.cg);
  coeff.reference(interpolant.coeff);
  width.reference(interpolant.width);

  interpolationMethod=interpolant.interpolationMethod;
  implicitInterpolationMethod=interpolant.implicitInterpolationMethod;
  tolerance=interpolant.tolerance;  
  explicitInterpolation=interpolant.explicitInterpolation;

}

//\begin{>>InterpolateInclude.tex}{\subsubsection{updateToMatchGrid}}  
void Interpolant::
updateToMatchGrid(CompositeGrid & cg0, int refinementLevel /* =0  */ )
//==============================================================================
// /Description:
//    Associate this Interpolant with a CompositeGrid and compute the interpolation
//    coefficients.
// /cg0 (input): associate the interpolant with this CompositeGrid.
// /refinementLevel : only grids on this refinement level and above have been changed.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  if( cg0.numberOfMultigridLevels()>1 )
  {
    printF("Interpolant::updateToMatchGrid:WARNING: The CompositeGrid being supplied has multigrid levels\n"
           "   Only the grids on level 0 will be interpolated. \n");
  }
  
  interpolationIsInitialized=false;
  updateForAdaptiveGrid=refinementLevel;

  // do this later initializeParallelInterpolator=true;

  cg.reference(cg0);
  // Make the pointer in the CG point to this Interpolant
  if( cg0.rcData->interpolant!=this )
  {

    if( cg0.rcData->interpolant!=NULL && !cg0.rcData->uncountedReferencesMayExist() && 
        cg0.rcData->interpolant->decrementReferenceCount()==0 )
    {
      delete cg0.rcData->interpolant;
      cg0.rcData->interpolant=NULL;
    }
    cg0.rcData->interpolant=this;   

    // This next fix is not quite right:
    cg0.rcData->interpolant->incrementReferenceCount();  

    // This is not correct either: 
    // cg0.rcData->interpolant->rcData->incrementReferenceCount();  // *wdh* 100311
  }
  
  if( cg.numberOfDimensions()==3 )
    implicitInterpolationMethod=iterateToInterpolate; // could also be: =directSolve;

  
  // cg.interpolationIsImplicit.display("Interpolant::updateToMatchGrid: cg.interpolationIsImplicit");
  
  bool implicitInterpolation=false;
  for( int toGrid=0; toGrid<cg.numberOfComponentGrids() && !implicitInterpolation; toGrid++ )
  {
    for( int fromGrid=0; fromGrid<cg.numberOfComponentGrids(); fromGrid++ )
    {
      if( toGrid!=fromGrid )
      {
        implicitInterpolation=cg.interpolationIsImplicit(toGrid,fromGrid);
        if( implicitInterpolation )
	  break;
      }
    }
  }
  explicitInterpolation=!implicitInterpolation;
  
  if( false )
  {
    const int myid=max(0,Communication_Manager::My_Process_Number);
    printf("Interpolant::updateToMatchGrid: myid=%i explicitInterpolation=%i\n",myid,(int)explicitInterpolation);
    int ei = ParallelUtility::getMaxValue((int)explicitInterpolation);
    if( ei!=explicitInterpolation )
    {
      printf("Interpolant::updateToMatchGrid:ERROR: myid=%i, explicitInterpolation is not the same on"
	     "all processors!\n",myid);
      OV_ABORT("error");
    }
  }
  
  // printF(" ****Interpolant::updateToMatchGrid explicitInterpolation=%i\n",explicitInterpolation);
  
}  

/* -----
//\begin{>>InterpolateInclude.tex}{\subsubsection{updateToMatchAdaptiveGrid}}  
void Interpolant::
updateToMatchAdaptiveGrid(CompositeGrid & cg0 )
//==============================================================================
// /Description:
//   Use this update when the grid has changed through the addition of removal of
// refinement grids (but the base grids have not changed).
//
// On an adaptive grid we always interpolate refinement grids using explicit interpolation
// or iterative implicit interpolation. This means that we do not need to reform a matrix
// for the implicit interpolation. The matrix can be used on the base-grids 
//   
// /cg0 (input): associate the interpolant with this CompositeGrid.
//\end{InterpolateInclude.tex}  
//==============================================================================
{
  // this needs to be written !

  // On an adaptive grid there is no need to recompute coefficients for the base grids
  //  since these should not have changed.

  updateToMatchGrid(cg0);
}
---- */


int Interpolant::
initializeInterpolation()
// =============================================================================================
// /Description:
//    Compute the interpolation coefficients, or initialize the sparse solver for solving
//  the implicit interpolation equations.
// =============================================================================================
{
  real time=getCPU();
  
  interpolationIsInitialized=true;

  if( false ) 
    cg.interpolationStartEndIndex.display("Interpolant:: interpolationStartEndIndex");

  bool gridHasNoInterpolationPoints=max(cg.numberOfInterpolationPoints)<=0; // is this ok in parallel?
  if( cg.numberOfBaseGrids()<=1 || gridHasNoInterpolationPoints )
  {
    //   ::display(cg.numberOfInterpolationPoints,"initializeInterpolation: cg.numberOfInterpolationPoints");
    explicitInterpolation=true;
  }
  
  if( interpolationIsImplicit() && implicitInterpolationMethod!=iterateToInterpolate )
  {
    if( Mapping::debug & 1 )
      printF("Interpolant: initialize implicit interpolation...\n");
    if( rcData->implicitInterpolant == NULL )
      rcData->implicitInterpolant= new Oges( cg );  // Equation solver
    else
      rcData->implicitInterpolant->updateToMatchGrid( cg ); 

    // printF(" *************** Interpolant: cg.numberOfRefinementLevels()=%i \n",cg.numberOfRefinementLevels());
	
    Range all;
    const int stencilWidth=max(3,max(cg.interpolationWidth(all,all,all))); // fix this
    const int diagonal=int(pow(stencilWidth,cg.numberOfDimensions())+.5)/2;  // center point
    int stencilSize=int( pow(stencilWidth,cg.numberOfDimensions())+1 );  // add 1 for interpolation equations
	
    realCompositeGridFunction interpCoeff(cg,stencilSize,all,all,all); 

    // interpCoeff.setIsACoefficientMatrix(TRUE,stencilSize);   // *wdh* 030120
    const int numberOfGhostLines=stencilWidth/2;
    interpCoeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines);  

    interpCoeff=0.;
    CompositeGridOperators op(cg);
    if( stencilWidth==5 || stencilWidth==7 || stencilWidth==9 )
    {
      op.setStencilSize(stencilSize);
      // op.setOrderOfAccuracy(4);
      op.setOrderOfAccuracy(stencilWidth-1);
    }
    else if( stencilWidth!=3 )
    {
      printF("Interpolant::initializeInterpolation:ERROR: stencilWidth=%i not expected!\n",stencilWidth);
      cg.interpolationWidth.display("Here is cg.interpolationWidth");
      Overture::abort("Interpolant::initializeInterpolation:ERROR");
    }
    
    
    interpCoeff.setOperators(op);

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      realArray & c = interpCoeff[grid];
      getIndex(mg.dimension(),I1,I2,I3);
      c(diagonal,I1,I2,I3)=1.; // diagonal entry=1
          
      // set classify for ghost line values. We just want to copy these values when we solve.
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  getBoundaryIndex(mg.dimension(),side,axis,I1,I2,I3);
	  Iv[axis]= side==Start ? Range(mg.dimension(side,axis),mg.indexRange(side,axis)-1)
	    : Range(mg.indexRange(side,axis)+1,mg.dimension(side,axis));
	  interpCoeff[grid].sparse->setClassify(SparseRepForMGF::ghost1,I1,I2,I3);
	}
      }
    }
	
    bcParams.interpolateRefinementBoundaries=interpolateRefinementBoundaries;
    bcParams.interpolateHidden=interpolateHidden;
    
    interpCoeff.finishBoundaryConditions(bcParams);  // this will fill in interpolation equations
    // interpCoeff.display("Interpolant: Here is interpCoeff after finishBoundaryConditions");
//	if( cg.numberOfRefinementLevels()>1 )
//	  interpolateRefinements(interpCoeff);     // ***** this may not be correct, should form the full matrix??

    rcData->implicitInterpolant->setCoefficientArray( interpCoeff );   // supply coefficients
    rcData->implicitInterpolant->set(OgesParameters::THEbestDirectSolver );
	
    if( cg.numberOfDimensions()==3 )
    {
      rcData->implicitInterpolant->set(OgesParameters::THEbestIterativeSolver);
      // rcData->implicitInterpolant->set(OgesParameters::THEsolverType,OgesParameters::PETSc);
      rcData->implicitInterpolant->set(OgesParameters::THEtolerance,tolerance);  // max(1.e-8,REAL_EPSILON*10.));
    }
	
    // rcData->implicitInterpolant->initialize( ); 
  }
  else
  {
    if( rcData->implicitInterpolant!=NULL ) // remove any implicit interpolant.
    {
      delete rcData->implicitInterpolant;
      rcData->implicitInterpolant=NULL;
    }

    initializeExplicitInterpolation();
  }

  timeForInitializeInterpolation+=getCPU()-time;
  return 0;
}



#include "OGPolyFunction.h"
#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int Interpolant::
testInterpolation( CompositeGrid & cg, int problemType )
// ===================================================================================
// Test the interpolate 
// /problemType: 1 : test interpolate
//               2 : test interpolateRefinements and interpolateRefinementBoundaries
// ===================================================================================
{
  int debug=0;
  
  int grid;

  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // needed for TZ functions
  
  Range all;
  realCompositeGridFunction u(cg,all,all,all);

  CompositeGridOperators op(cg);  // create some differential operators
  u.setOperators(op);             // needed for AMR interpolation

  // save any pointer to an Interpolant:
  Interpolant *interpPointer=cg->interpolant;
  
  Interpolant interpolant(cg);
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);


  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3;

  for( int degree=0; degree<=2; degree++ )
  {
    
    // create a twilight-zone function for checking the errors
    int degreeOfSpacePolynomial = degree; // problemType<2 ? 2 : 1;
    int degreeOfTimePolynomial = 0;
    int numberOfComponents = cg.numberOfDimensions();
    OGPolyFunction poly(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);
    OGFunction & exact= poly;
    OGFunction *exactPointer=&exact;
    
    // ========== Test the Interpolant. ================
    if( problemType & 2 ) 
    {
      u=0.;

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();
	getIndex(mg.dimension(),I1,I2,I3);  
	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

/* ----
   where( mask<=0 || (mask & MappedGrid::IShiddenByRefinement) )
   {
   u[grid]=999.;
   }
   if( grid!=cg.baseGridNumber(grid) )
   { // for refinement grids, set bogus values on the interpolation points.
   ForBoundary(side,axis)
   {
   if( mg.boundaryCondition(side,axis) == 0 )
   {
   getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);
   u[grid](Ib1,Ib2,Ib3)=999.;
   }
   }
   }
   ---- */
      }

      // u.display("u before interpolation");
      // u.interpolate();

      interpolateRefinements(u);  // **** do this to set MappedGrid::IShiddenByRefinement
  
      // u.display("u after interpolation","%6.2e ");

      real error=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	  error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
	if( Oges::debug & 8 )
	{
	  cg[grid].mask().display("Here is the mask");
	  realArray err(I1,I2,I3);
	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  where( cg[grid].mask()(I1,I2,I3)==0 )
	    err(I1,I2,I3)=0.;
	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
	  // display(err,"abs(error on indexRange +1)","%5.1e ");
	  display(err,"abs(error on indexRange +1)","%3.1f ");
	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	}
      }
      printF("Maximum error in interpolateRefinements(u)= %e\n",error);  



      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();
	getIndex(mg.dimension(),I1,I2,I3);  
	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

	if( grid!=cg.baseGridNumber(grid) )
	{ // for refinement grids, set bogus values on the interpolation points.
	  ForBoundary(side,axis)
	  {
	    if( mg.boundaryCondition(side,axis) == 0 )
	    {
	      getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      u[grid](Ib1,Ib2,Ib3)=999.;
	    }
	  }
	}
      }



      InterpolateRefinements interp(cg.numberOfDimensions());


      int refinementRatio=max(cg.refinementLevel[1].refinementFactor);
      printF(" >>>>>>>>>> test interpolateRefinementBoundaries: refinementRatio=%i\n",refinementRatio);
    

//     IntegerArray ratio(3);
//     ratio=refinementRatio;
//     interp.setRefinementRatio( ratio );

      interp.interpolateRefinementBoundaries(u); 
      u.interpolate();
    
      // display(u[3],"u[3] after interpolation","%6.2e ");

      error=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	  error=max(error,max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))));    
	if( Oges::debug & 8 )
	{
	  cg[grid].mask().display("Here is the mask");
	  realArray err(I1,I2,I3);
	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  where( cg[grid].mask()(I1,I2,I3)==0 )
	    err(I1,I2,I3)=0.;
	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
	  // display(err,"abs(error on indexRange +1)","%5.1e ");
	  display(err,"abs(error on indexRange +1)","%3.1f ");
	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	}
      }
      printF("Maximum error in interp.interpolateRefinementBoundaries(u)= %e (degree=%i) \n",error,
	     degreeOfSpacePolynomial);  

      continue;
    }

    // ========== Test the Interpolant. ================
    printF("testInterpolation: problemType=%i , problemType & 1=%i\n",problemType,problemType & 2);
    
    if( problemType & 1 )
    {
      u=0.;

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	const intArray & mask = mg.mask();
	getIndex(mg.dimension(),I1,I2,I3);  
	u[grid](I1,I2,I3)=exact(mg,I1,I2,I3);

	where( cg[grid].mask()(I1,I2,I3)<=0 )
	  u[grid](I1,I2,I3)=-99.;
      }

      if( debug & 4 )
      {
	display(u[3],"u[3] before interpolation","%6.2e ");
	displayMask(cg[3].mask(),"mask");
      }
    
      u.interpolate();
    
//        if( debug & 4 )
//        {
//  	display(u[3],"u[3] after interpolation","%6.2e ");
//        }
    
      Index J1,J2,J3;
      real error=0., err=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].indexRange(),I1,I2,I3,1);   // note indexRange+1
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	{
	  err=max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)));    
	  error=max(error,err);
	}
	getIndex(cg[grid].gridIndexRange(),J1,J2,J3);
	real uMax=0., uMin=0.;
	where( cg[grid].mask()(J1,J2,J3)!=0 )
	{
	  uMin=min(u[grid](J1,J2,J3));
	  uMax=max(u[grid](J1,J2,J3));
	}
	printF("testInterp: degree=%i grid=%i uMin=%8.2e uMax=%8.2e err=%8.2e \n",degree,grid,uMin,uMax,err);  
      
	if( Oges::debug & 8 )
	{
	  cg[grid].mask().display("Here is the mask");
	  realArray err(I1,I2,I3);
	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  where( cg[grid].mask()(I1,I2,I3)==0 )
	    err(I1,I2,I3)=0.;
	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
	  // display(err,"abs(error on indexRange +1)","%5.1e ");
	  display(err,"abs(error on indexRange +1)","%3.1f ");
	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	}
      }
      printF("Maximum error in interpolate= %e (degree=%i)\n",error,degreeOfSpacePolynomial);  

    }
  } // end for degree

  cg->interpolant=interpPointer; // reset

  return 0;
}

  
