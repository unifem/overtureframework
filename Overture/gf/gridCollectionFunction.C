#include "GridCollectionFunction.h"
#include "intGridCollectionFunction.h"
#include "GridCollection.h"
#include "Interpolant.h"
#include "GridCollectionOperators.h"
#include "SparseRep.h"
#include "GridFunctionParameters.h"
#include "display.h"
#include "DisplayParameters.h"

#include "ListOfGridCollectionFunction.h"

#undef FLOAT_COLLECTION_FUNCTION
#undef DOUBLE_COLLECTION_FUNCTION
#undef INT_COLLECTION_FUNCTION


  // numberOfComponentGrids : number of grids including refinements but excluding multigrid levels
int GridCollectionFunction::
numberOfComponentGrids() const 
{ 
  return gridCollectionData!=0 ? gridCollectionData->numberOfComponentGrids : 0; 
}

  // numberOfGrids : total number of grids including refinements and multigrid levels
int GridCollectionFunction::
numberOfGrids() const 
{ 
  return gridCollectionData!=0 ? gridCollectionData->numberOfGrids : 0; 
}


// Note that FABS is converted to abs in the int case

GridCollectionFunction::RCData::
RCData()
{
//  positionOfComponent.redim(maximumNumberOfIndicies);
//  positionOfCoordinate.redim(maximumNumberOfIndicies);
  positionOfFaceCentering=-1;
  faceCentering=GridFunctionParameters::none;
  interpolant            =NULL;
  numberOfNames          =0;
  name                   =NULL;
}
GridCollectionFunction::RCData::
~RCData()
{
  delete [] name;
}
//=================================================================================================
// Perform a deep copy, set reference count
//=================================================================================================
GridCollectionFunction::RCData & GridCollectionFunction::RCData::
operator=( const GridCollectionFunction::RCData & rcData )  // deep copy
{
  if( this == &rcData )
    return *this;
  int i;
  for( i=0; i<maximumNumberOfIndicies+1; i++ ) 
  {
    positionOfComponent[i]    =rcData.positionOfComponent[i];
    positionOfCoordinate[i]   =rcData.positionOfCoordinate[i];
  }
  positionOfFaceCentering=rcData.positionOfFaceCentering;
  faceCentering          =rcData.faceCentering;
  interpolant            =rcData.interpolant;
  // refinementLevel        =rcData.refinementLevel; // do not do this here since we may have a list of 
  // multigridLevel         =rcData.multigridLevel;  // compositeGridFunction's
  numberOfNames          =rcData.numberOfNames;  
  delete [] name;
  if( numberOfNames>0 )
  {
    name = new aString[numberOfNames];
    for( int i=0; i<numberOfNames; i++ )
      name[i]=rcData.name[i];
  }
  else
    name=NULL;
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
    R[i]=rcData.R[i];
  
  return *this;
}




//\begin{>GridCollectionFunctionInclude.tex}{\subsubsection{Public data members}} 
//\no function header:
// 
// Here are the public data members:
//
// /intR numberOfComponentGrids: (CompositeGridFunction only) equals value found in the CompositeGrid, 
//       this is here for convenience
// /GridCollection     *gridCollection:      pointer to the GridCollection
// /GenericGridCollectionOperators *operators:      pointer to operators used for derivatives etc.
//
//\end{GridCollectionFunctionInclude.tex} 


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// The following enumerators are equivalent to the ones appearing in the {\ff MappedGridFunction}.
// See the MappedGridFunction documentation for futher details.
// /updateReturnValue:
//    The value returned from the {\ff updateToMatchGrid} and {\ff update\-To\-Match\-Grid\-Function} is
//    a mask formed by a bitwise {\ff or} of the following values:  
//\end{GridCollectionFunctionInclude.tex}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{Arithmetic operators, max,min,abs}} 
//\no function header:
// 
//  The arithmetic operators $+$, $-$, $*$, $/$, $+=$, $-=$, $*=$ and $/=$ are defined for
//  two  {\ff Grid\-Collection\-Function}'s of the same type  or for a 
//  {\ff GridCollectionFunction} and a float, double, or int.
//  The operators {\ff max}, {\ff min} and {\ff fabs} ({\ff abs} for the int case) are
//  defined.  
//\end{GridCollectionFunctionInclude.tex} 




//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{Constructors}} 
GridCollectionFunction::
GridCollectionFunction ()
//=============================================================================================
// /Description:
//   Default constructor
//\end{GridCollectionFunctionInclude.tex} 
//=============================================================================================
{
  rcData=NULL;
  initialize();
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::
GridCollectionFunction(GridCollection & gc)
//=============================================================================================
// /Description:
//   Create a grid function and associate with a GridCollection.
//   The grid function will be a "scalar" as in the declaration:
//   \begin{verbatim}  
//     Range all;
//     GridCollection gc(...);
//     GridCollectionFunction u(gc,all,all,all);
//   \end{verbatim}  
// /gc (input): grid to associate this grid function with.
// /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//=============================================================================================
{
  rcData=NULL;
  initialize();
  gridCollection=&gc;  // save so we can construct an Interpolant

  // convert to new format:
  Range Ru[maximumNumberOfIndicies];
  for( int i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;
  constructor( &gc,gc.rcData,Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );
}

GridCollectionFunction::
GridCollectionFunction(GridCollectionData & gcData)
{
  rcData=NULL;
  initialize();
  // convert to new format:
  Range Ru[maximumNumberOfIndicies];
  for( int i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;
  constructor( NULL,&gcData,Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::
GridCollectionFunction(GridCollection & gc, 
		       const Range & R0, /* =nullRange */
		       const Range & R1, /* =nullRange */
		       const Range & R2, /* =nullRange */
		       const Range & R3, /* =nullRange */
		       const Range & R4, /* =nullRange */
		       const Range & R5, /* =nullRange */
		       const Range & R6, /* =nullRange */
		       const Range & R7  /* =nullRange */ )
//=============================================================================================
// /Description:
//   This constructor takes ranges, the first 3 "nullRange" values are taken to be the
//   coordinate directions in the grid function.
//   Each grid function is dimensioned according to the dimensions found
//   with the {\ff MappedGrid} found in the {\ff GridCollection}.
//   Grid functions can have up to 8 dimensions, the index positions
//   not used by the coordinate dimensions can be used to store
//   different components. For example, a {\em vector} grid
//   functions would use 1 index position for components while a {\em matrix} grid functions would
//   use two index positions for components. 
//
// /grid0 (input): GridCollection to associate this grid function with.
// /R0, R1, R2, ... (input): Ranges to determine the shape and size of the grid function.
//       An int can also be used instead of a Range.
// 
// /Examples:
//   Here are some examples
//   {\footnotesize\begin{verbatim}
//    
//        //  R1 = range of first dimension of the grid array
//        //  R2 = range of second dimension of the grid array
//        //  R3 = range of third dimension of the grid array 
//
//        GridCollection gc(...);
//
//        Range all;    // null Range is used to specify where the coordinates are
//
//        GridCollectionFunction u(gc);                           //  --> u[grid](R1,R2,R3);
//
//        GridCollectionFunction u(gc,all,all,all,1);             //  --> u[grid](R1,R2,R3,0:1);
//        GridCollectionFunction u(gc,all,all,Range(1,1));        //  --> u[grid](R1,R2,1:1,R3);
//
//        GridCollectionFunction u(gc,2,all);                     //  --> u[grid](0:2,R1,R2,R3);
//        GridCollectionFunction u(gc,Range(0,2),all,all,all);    //  --> u[grid](0:2,R1,R2,R3);
//        GridCollectionFunction u(gc,all,Range(3,3),all,all);    //  --> u[grid](R1,3:3,R2,R3);
//    
//    \end{verbatim}
//    }   
//    
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//=============================================================================================
{
  rcData=NULL;
  initialize();
  gridCollection=&gc;  
  constructor( &gc,gc.rcData,R0,R1,R2,R3,R4,R5,R6,R7 );
}


GridCollectionFunction::
GridCollectionFunction(GridCollectionData & gcData, 
		       const Range & R0, 
		       const Range & R1,
		       const Range & R2,
		       const Range & R3,
		       const Range & R4,
		       const Range & R5,
		       const Range & R6,
		       const Range & R7 )
{
  rcData=NULL;
  initialize();
  constructor( NULL,&gcData,R0,R1,R2,R3,R4,R5,R6,R7 );
}
GridCollectionFunction::
GridCollectionFunction(GridCollection & gc, 
		       const int   & i0, 
		       const Range & R1,
		       const Range & R2,
		       const Range & R3,
		       const Range & R4,
		       const Range & R5,
		       const Range & R6,
		       const Range & R7 )
{
  rcData=NULL;
  initialize();
  gridCollection=&gc;  
  constructor( &gc,gc.rcData,Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
}


GridCollectionFunction::
GridCollectionFunction(GridCollectionData & gcData, 
		       const int   & i0, 
		       const Range & R1,
		       const Range & R2,
		       const Range & R3,
		       const Range & R4,
		       const Range & R5,
		       const Range & R6,
		       const Range & R7 )
{
  rcData=NULL;
  initialize();
  constructor( NULL,&gcData,Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
}


//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::
GridCollectionFunction(GridCollection & gc, 
		       const GridFunctionParameters::GridFunctionType & type, 
		       const Range & component0, /* =nullRange */
		       const Range & component1, /* =nullRange */
		       const Range & component2, /* =nullRange */
		       const Range & component3, /* =nullRange */
		       const Range & component4  /* =nullRange */ )
//=============================================================================================
// /Description:
//   This constructor is used to create a grid function of some standard type.
//   The standard types are defined in the GridFunctionParameters::GridFunctionType enum,
//   \begin{itemize}
//     \item vertexCentered  : grid function is vertex centred
//     \item cellCentered    : grid function is cell centred
//     \item faceCenteredAll : grid function components are face centred in all directions
//     \item faceCenteredAxis1 : grid function is face centred along axis1
//     \item faceCenteredAxis2 : grid function is face centred along axis2
//     \item faceCenteredAxis3 : grid function is face centred along axis3
//     \item general : means same as vertexCentered when used in this constructor
//   \end{itemize}  
// /grid0 (input): Use this GridCollection.
// /type (input): Make this type of grid function.
// /component0, component1,... (input): supply a Range for each component. 
// /Examples:
//   Here are some examples:
//  {\footnotesize  
//  \begin{verbatim} 
//    GridCollection gc(...);
//    realGridCollectionFunction u(gc,GridFunctionParameters::vertexCentered,2);   // u(gc,all,all,all,2);
//    realGridCollectionFunction u(gc,GridFunctionParameters::cellCentered,2,3);   // u(gc,all,all,all,2,3);
//    realGridCollectionFunction u(gc,GridFunctionParameters::faceCenteredAll,2);  // u(gc,all,all,all,2,faceRange);
//    realGridCollectionFunction u(gc,GridFunctionParameters::faceCenteredAll,3,2);// u(gc,all,all,all,3,2,faceRange);
//  \end{verbatim}  
//  }  
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//=============================================================================================
{
  rcData=NULL;
  initialize();
  gridCollection=&gc;  
  gridCollectionData=gc.rcData;

  // make a component grid function and put into the list (reference)
  int grid;
  for( grid=0; grid< numberOfGrids(); grid++ )
  {
    if( gridCollection ) // use the gridCollection if it is there
    {
      MappedGridFunction mgf((*gridCollection)[grid],type,
              component0,component1,component2,component3,component4);
      mappedGridFunctionList.addElement( mgf );
    }
    else
    {
      MappedGridFunction mgf((*gridCollectionData)[grid],type,
           component0,component1,component2,component3,component4);
      mappedGridFunctionList.addElement( mgf );
    }
  }
  // save local copies of R, positionOfComponent, and positionOfCoordinate
  Range *R = rcData->R;
  if( numberOfGrids() > 0 )
  {
    grid=0;  // get a copy from this grid, they should all  be the same
    mappedGridFunctionList[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );
    for( int i=0; i<maximumNumberOfIndicies; i++ )
    {
      rcData->positionOfComponent[i]    =mappedGridFunctionList[grid].positionOfComponent(i);
      rcData->positionOfCoordinate[i]  =mappedGridFunctionList[grid].positionOfCoordinate(i);
    }
    rcData->positionOfFaceCentering=mappedGridFunctionList[grid].positionOfFaceCentering();
    rcData->faceCentering          =mappedGridFunctionList[grid].faceCentering();
  }

  // update refinementLevel, multigridLevel etc.
  updateCollections();
}



//----------------------------------------------------------------------------------------------
//  This routine performs operations that are common to the constructors
//----------------------------------------------------------------------------------------------
void GridCollectionFunction::
constructor(GridCollection *gridCollection0,
	    GridCollectionData *gridCollectionData0,
	    const Range & R0, 
	    const Range & R1,
	    const Range & R2,
	    const Range & R3,
	    const Range & R4,
	    const Range & R5,
	    const Range & R6,
	    const Range & R7,
            const bool createMappedGridFunctionList /* = TRUE */ )
{
  gridCollectionData=gridCollectionData0;

  if( createMappedGridFunctionList )
  {
    // make a component grid function and put into the list (reference)
    for( int grid=0; grid< numberOfGrids(); grid++ )
    {
      if( gridCollection ) // use the gridCollection if it is there
      {
	MappedGridFunction mgf((*gridCollection)[grid],R0,R1,R2,R3,R4,R5,R6,R7);
	mappedGridFunctionList.addElement( mgf );
      }
      else
      {
	MappedGridFunction mgf((*gridCollectionData)[grid],R0,R1,R2,R3,R4,R5,R6,R7);
	mappedGridFunctionList.addElement( mgf );
      }
    }
  }
  // save local copies of R, positionOfComponent, and positionOfCoordinate
  Range *R = rcData->R;
  if( numberOfGrids() > 0 )
  {
    int grid=0;  // get a copy from this grid, they should all  be the same
    mappedGridFunctionList[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );
    // positionOfComponent    =mappedGridFunctionList[grid].positionOfComponent;
    // positionOfCoordinate   =mappedGridFunctionList[grid].positionOfCoordinate;
    for( int i=0; i<maximumNumberOfIndicies; i++ )
    {
      rcData->positionOfComponent[i] =mappedGridFunctionList[grid].positionOfComponent(i);
      rcData->positionOfCoordinate[i]=mappedGridFunctionList[grid].positionOfCoordinate(i);
    }
    rcData->positionOfFaceCentering=mappedGridFunctionList[grid].positionOfFaceCentering();
    rcData->faceCentering          =mappedGridFunctionList[grid].faceCentering();
  }

  // update refinementLevel, multigridLevel etc.
  updateCollections();
}


//----------------------------------------------------------------------------------------------
// Copy constructor, deep copy by default
//----------------------------------------------------------------------------------------------
GridCollectionFunction::
GridCollectionFunction( const GridCollectionFunction & cgf, const CopyType copyType  )
  : GenericGridCollectionFunction(cgf,copyType)
{
  rcData=NULL;
  switch( copyType )
  {
  case DEEP:
    initialize();
    (*this)=cgf;
    break;
  case SHALLOW:
    initialize();  
    reference( cgf ); 
    break;
  case NOCOPY:
    initialize();
    break;
  }
}


//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
GridCollectionFunction::
~GridCollectionFunction ()
{
  if( rcData->decrementReferenceCount() == 0 )
    delete rcData;
}

//----------------------------------------------------------------------------------------------
// Initialize variables in a gridCollectionFunction
//----------------------------------------------------------------------------------------------
void GridCollectionFunction::
initialize()
{
  className="GridCollectionFunction";
  if( rcData!=NULL )
  {
    printf("GridCollectionFunction::ERROR:initialize: rcData!=NULL, something is wrong here\n");
    Overture::abort("error");
  }
  rcData = new RCData;
  rcData->incrementReferenceCount();  // is initialized to 0
  gridCollectionData = NULL;
  gridCollection=NULL;
  operators=NULL;
  temporary=FALSE;                    // TRUE means this is an internal temporary used by + - * = op's
  dataAllocationOption=0;
  
//  positionOfComponent.reference(rcData->positionOfComponent);
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
    rcData->positionOfComponent[i]=maximumNumberOfIndicies;                   // default value

  // positionOfCoordinate.reference(rcData->positionOfCoordinate);
  for( i=0; i<maximumNumberOfIndicies; i++ )
    rcData->positionOfCoordinate[i]=maximumNumberOfIndicies;                   // default values

  multigridLevel.reference(rcData->multigridLevel);  // ** is this correct?
  refinementLevel.reference(rcData->refinementLevel);

  for( int axis=0; axis<3; axis++ )
  {
    rcData->positionOfCoordinate[axis]=axis;
    rcData->R[axis]=nullRange;                 // range values are null at coordinate positions
  }
  for( i=3; i<maximumNumberOfIndicies+1; i++ )
    rcData->R[i]=Range(0,0);
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{breakReference}}
void GridCollectionFunction::
breakReference()
//-----------------------------------------------------------------------------------------
// /Description:
//   This member function will cause the grid function
//   to no longer be referenced. The grid function acquires its own copy
//   of the data.
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  // if there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    GridCollectionFunction cgf = *this;  // makes a deep copy
    reference(cgf);                           // make a reference to this new copy
  }
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{operator()(Range,...)}}
GridCollectionFunction GridCollectionFunction::
operator()(const Range & component0,
	   const Range & component1 /* =nullRange */,
	   const Range & component2 /* =nullRange */,
	   const Range & component3 /* =nullRange */,
	   const Range & component4 /* =nullRange */ )
//==================================================================================
// /Description:
//   Return a new GridCollectionFunction that is linked (using the {\tt link} function) to some specfied components
//   of the current GridCollectionFunction. This is a convenient but 
//  {\bf inefficient} way to easily access certain components of a multi-component GridCollectionFunction as in
//  the example:
// \begin{verbatim} 
//    GridCollection gc(...);
//    floatGridCollectionFunction u(gc,all,all,all,2);
//    u(0)=1.;   // set component 0 of u to be 1.
//    u(1)=2.;   // set component 1 of u to be 2.
//    u(0)=u(0)*2.+u(1)*u(0);
// \end{verbatim}
// The above code is inefficient since a new gridCollectionFunction is built every time an expression like {\tt u(0)}
// appears.
//
// This has the same effect as the following (more efficient but not as cute) code:
// \begin{verbatim} 
//    GridCollection gc(...);
//    floatGridCollectionFunction u(gc,all,all,all,2), u0,u1;
//    u0.link(u,Range(0,0));
//    u1.link(u,Range(1,1)); 
//    u0=1.;
//    u1=2.;
//    u0=u0*2.+u1*u0;  
// \end{verbatim}
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  GridCollectionFunction result;
  result.link(*this,component0,component1,component2,component3,component4);
  return GridCollectionFunction(result,SHALLOW);   // return a shallow copy so that we don't break the link
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{consistencyCheck}}
void GridCollectionFunction::
consistencyCheck() const
//==================================================================================
// /Description:
//   Perform a consistency check on the grid function.
// /Return values: Return 0 if the grid function appears to be ok. 
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  ReferenceCounting::consistencyCheck();
  if( gridCollection!=0 )
  {
    assert( gridCollectionData!=0 );
    assert( gridCollection->numberOfGrids()==mappedGridFunctionList.getLength() );

    // ***** finish this bill ****
  }

}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{dataCopy}}
int GridCollectionFunction::
dataCopy( const GridCollectionFunction & gcf )
//==================================================================================
// /Description:
//   copy the array data only
//
// /gcf (input): set the array data equal to the data in this grid function.
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid< numberOfGrids(); grid++ )
    (*this)[grid].dataCopy(gcf[grid]);

// *wdh 961212
  if( gcf.temporary )
  { // Delete gcf if it is a temporary
    GridCollectionFunction *temp = (GridCollectionFunction*) &gcf; 
    delete temp;                                                  
  }
  return 0;
}



/* -----  Here are some comments -----
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{Derivatives: x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
GridCollectionFunction 
derivative(const Index & component0,  // =nullIndex 
	   const Index & component1,  // =nullIndex 
	   const Index & component2,  // =nullIndex 
	   const Index & component3,  // =nullIndex 
	   const Index & component4   // =nullIndex 
         )
//==================================================================================
// /Description:
//   derivative equals one of x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div.
//   Return the derivative of this grid function. This routine just calls the 
//   function of the same name in the GenericGridCollectionOperators.
// /component0,component1,... (input) : optional arguments to specify which components should be
//    computed. The other components will be returned as zero.
// /component0,component1,... (input) : optional arguments to specify which components should be
//    computed. The other components will be returned as zero.
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by component0,...
//  (if component0 etc are not specified then the result will have the same number of components 
// of the grid function being differentiated). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction 
derivative(const GridFunctionParameters::GridFunctionType & gfType,
           const Index & component0,  // =nullIndex 
	   const Index & component1,  // =nullIndex 
	   const Index & component2,  // =nullIndex 
	   const Index & component3,  // =nullIndex 
	   const Index & component4   // =nullIndex 
         )
//==================================================================================
// /Description:
//   derivative equals one of x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div.
//   Return the derivative of this grid function. 
//   The argument gfType determines the
//   type of the grid function that is returned. 
//   This routine just calls the 
//   function of the same name in the GenericGridCollectionOperators (see also setOperators).
// /gfType (input): The type of the grid function to be returned.
// /component0,component1,... (input) : optional arguments to specify which components should be
//    computed. The other components will be returned as zero.
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by component0,...
//  (if component0 etc are not specified then the result will have the same number of components 
// of the grid function being differentiated). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
 ----------------- */

/* ------- Here are some comments -----
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{Derivative Coefficients: xCoefficient,yCoefficient,...}}
GridCollectionFunction
Derivative(const Index & component0,  // =nullIndex 
	   const Index & component1,  // =nullIndex 
	   const Index & component2,  // =nullIndex 
	   const Index & component3,  // =nullIndex 
	   const Index & component4   // =nullIndex 
         )
//==================================================================================
// /Description:
//   Derivative equals one of xCoefficient,yCoefficient,zCoefficient,xxCoefficient,
//    xy\-Coefficient,xz\-Coefficient,yy\-Coefficient,yz\-Coefficient,zz\-Coefficient,
//    laplacianCoefficient,gradCoefficient,divCoefficient.
//   Return the coefficients of the derivative. This routine just calls the 
//   function of the same name in the GenericGridCollectionOperators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction
Derivative(const GridFunctionParameters::GridFunctionType & gfType,
           const Index & component0,  // =nullIndex 
	   const Index & component1,  // =nullIndex 
	   const Index & component2,  // =nullIndex 
	   const Index & component3,  // =nullIndex 
	   const Index & component4   // =nullIndex 
         )
//==================================================================================
// /Description:
//   Derivative equals one of xCoefficient,yCoefficient,zCoefficient,xxCoefficient,
//    xy\-Coefficient,xz\-Coefficient,yy\-Coefficient,yz\-Coefficient,zz\-Coefficient,
//    laplacianCoefficient,gradCoefficient,divCoefficient.
//   Return the coefficients of the derivative. 
//   The argument gfType determines the
//   type of the grid function that is returned. 
//   This routine just calls the 
//   function of the same name in the GenericGridCollectionOperators (see also setOperators).
// /gfType (input): The type of the grid function to be returned.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
 ----------------- */




//---------------------------------------------------------------------------------
// Clean up a grid function, release the memory
//  -- set equal to a null grid function
//---------------------------------------------------------------------------------
// extern GridCollectionFunction nullGridCollectionFunction;

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{destroy}}
int GridCollectionFunction::
destroy()
//==================================================================================
// /Description:
//   destroy this grid function. (Release all memory)
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  //  *this=nullGridCollectionFunction;
  // **** no need to break reference and copy if only references are from the lists here ****

  if( FALSE )
  {
    mappedGridFunctionList.breakReference();      // *wdh* 990822 : so referenced GCF's are still correct.
    while( mappedGridFunctionList.getLength()>0 )
      mappedGridFunctionList.deleteElement();
    baseGrid.breakReference();
    while( baseGrid.getLength()>0 )
      baseGrid.deleteElement();
    multigridLevel.breakReference();
    while( multigridLevel.getLength()>0 )
      multigridLevel.deleteElement();
    refinementLevel.breakReference();
    while( refinementLevel.getLength()>0 )
      refinementLevel.deleteElement();
  }
  else
  {
    // *wdh* 000415
    mappedGridFunctionList.destroy();
    baseGrid.destroy();
    multigridLevel.destroy();
    refinementLevel.destroy();
  }
  
  if( rcData->decrementReferenceCount() == 0 )
  {
    delete rcData;
  }
  rcData=NULL;
  initialize();
  return 0;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{display}}
void GridCollectionFunction::
display(const aString & label /* =nullString */, 
	const aString & format /* =nullString */ ) const
//==================================================================================
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  display(label,NULL,format);
}


//\begin{>>GridCollectionFunctionInclude.tex}{}
void GridCollectionFunction::
display(const aString & label /* =nullString */, 
	FILE *file /* = NULL */, 
	const aString & format /* =nullString */ ) const
//==================================================================================
// /Description:
//   Display the grid function, print the values of in all the components.
//
// /label (input): optional label to print.
// /file (input): print to this file
// /format (input): use this format for printf
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  const int myid=Communication_Manager::My_Process_Number;
  if( myid<=0 )
  {
    if( file!=NULL )
      fprintf(file,"%s\n",(const char *)label);
    else
      cout << label << endl;
  }
  
  static char line[80];
  for( int grid=0; grid< numberOfGrids(); grid++ )
  {
    sPrintF(line,"------ grid %i ------",grid);
    ::display( (*this)[grid], line, file, (format==nullString ? NULL : (const char*)format) );
  }
// *wdh 961212
  if( this->temporary )
  { // Delete gcf if it is a temporary
    GridCollectionFunction *temp = (GridCollectionFunction*) this;  // cast away const
    delete temp;                                                  
  }
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
void GridCollectionFunction::
display(const aString & label, const DisplayParameters & displayParameters) const
//==================================================================================
// /Description:
//   Display the grid function, print the values of in all the components.
//
// /label (input): optional label to print.
// /displayParameters (input): specify parameters
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  static char line[80];
  for( int grid=0; grid< numberOfGrids(); grid++ )
  {
    // sPrintF(line,"------ grid %i ------",grid);
    ::display( (*this)[grid], line, displayParameters );
  }
// *wdh 961212
  if( this->temporary )
  { // Delete gcf if it is a temporary
    GridCollectionFunction *temp = (GridCollectionFunction*) this;  // cast away const
    delete temp;                                                  
  }
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{evaluate}}
GridCollectionFunction
evaluate( GridCollectionFunction & cgf ) 
//==================================================================================
// /Description:
//   Due to the way that temporaries are handled it is necessary to use this function
//   on expressions involving grid collection functions that are passed as arguments to function.
//   Example:
//   \begin{verbatim}
//     realGridCollectionFunction u(gc),v(cg);
//     ...
//     myFunction(evaluate(u+v));
//     ...
//   \end{verbatim}
//   If the {\tt evaluate} function were not used there could be a possible memory leak.
// /cgf (input): If this grid function is a temporary,
// /Return value: A grid collection function equal to cgf which is not a temporary
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  GridCollectionFunction cgfn;
  cgfn.reference(cgf);
  cgfn.temporary=FALSE;
  if( cgf.temporary )
  { // Delete cgf if it is a temporary
    GridCollectionFunction *temp = (GridCollectionFunction*) &cgf; 
    delete temp;                                                  
  }

  return cgfn;
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{get}}
int GridCollectionFunction::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get from a database file. Example:
// \begin{verbatim}
//   HDF_DataBase db;
//   db.mount("myFile.hdf","R");
//   GridCollection gc;
//   realGridCollectionFunction u;
//   initializeMappingList();
//   gc.get(db,"my grid");
//   u.updateToMatchGrid(gc);   // **NOTE**
//   u.get(db,"u");
// \end{verbatim}
//    
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
// /NOTE: This get function will not set the pointer to the Grid associated
//    with this grid function. You should call updateToMatchGrid(...) to set
//    the grid BEFORE using this function. 
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"GridCollectionFunction");

  subDir.setMode(GenericDataBase::streamInputMode);
 
  int i;
  subDir.get( rcData->positionOfComponent,"positionOfComponent",maximumNumberOfIndicies );
  subDir.get( rcData->positionOfCoordinate,"positionOfCoordinate",maximumNumberOfIndicies ); 
  subDir.get( rcData->positionOfFaceCentering,"positionOfFaceCentering" ); 
  subDir.get( rcData->faceCentering,"faceCentering" );  
  char buff[40];
  int base,bound;
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
  {
    subDir.get( base, sPrintF(buff,"R[%i].base",i) );
    subDir.get( bound,sPrintF(buff,"R[%i].bound",i) );
    rcData->R[i]=Range(base,bound);
  }
  subDir.get( rcData->numberOfNames,"numberOfNames" );
  delete [] rcData->name;
  if( rcData->numberOfNames > 0 )
  {
    rcData->name = ::new aString[rcData->numberOfNames];
    for( i=0; i<rcData->numberOfNames; i++ )
      subDir.get( rcData->name[i],sPrintF(buff,"name[%i]",i) );
  }
  else
    rcData->name=NULL;
  
  for( i=0; i<mappedGridFunctionList.getLength(); i++ )
    mappedGridFunctionList[i].get( subDir,sPrintF(buff,"mappedGridFunctionList[%i]",i) );

  delete &subDir;
  return 0; 
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getClassName}}
aString GridCollectionFunction::
getClassName() const 
//==================================================================================
// /Description:
//    Return the class name.
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{ 
  return className; 
}


//----------------------------------------------------------------------------------------
//  Here are functions that return the base, bound and dimension of the components
//  There are always "maximumNumberOfComponents" defined (5 currently)
//  Note that components that were not explicitly defined default to base=0, bound=0, dimension=1
//----------------------------------------------------------------------------------------

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getComponentBase}}
int GridCollectionFunction::
getComponentBase( int component ) const
//==================================================================================
// /Description:
//  Get the base for the given component.
//
// /component (input): component number, 0,1,...
//
// /Return Values:
//    The base for the component. Unused components have base=0 and bound=0
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "GridCollectionFunction::getComponentBase:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("GridCollectionFunction::getComponentBase:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBase();
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getComponentBound}}
int GridCollectionFunction::
getComponentBound( int component ) const
//==================================================================================
// /Description:
//  Get the bound for the given component.
//
// /component (input): component number, 0,1,...
//
// /Return Values:
//    The bound for the component. Unused components have base=0 and bound=0
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "GridCollectionFunction::getComponentBound:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("GridCollectionFunction::getComponentBound:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBound();
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getComponentDimension}}
int GridCollectionFunction::
getComponentDimension( int component ) const
//==================================================================================
// /Description:
//  Get the dimension for the given component, dimension=bound-base+1
//
// /component (input): component number, 0,1,...
//
// /Return Values:
//    The dimension for the component. Unused components have dimension=1
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "GridCollectionFunction::getComponentDimension:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("GridCollectionFunction::getComponentDimension:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBound()
        -rcData->R[positionOfComponent(component)].getBase()+1;
}

//----------------------------------------------------------------------------------------
//  Here are functions that return the base, bound and dimension of the coordinates
//  There are always 3 coordinates defined
//  Note that coordinates that were not explicitly defined default to base=0, bound=0, dimension=1
//----------------------------------------------------------------------------------------
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getCoordinateBase}}
int GridCollectionFunction::
getCoordinateBase( int coordinate ) const
//==================================================================================
// /Description:
//  Get the base for the given coordinate.
//
// /coordinate (input): component number, 0,1, or 2.
//
// /Return Values:
//    The base for the coordinate. Unused coordinates have base=0 and bound=0
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "GridCollectionFunction::getCoordinateBase:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("GridCollectionFunction::getCoordinateBase:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBase();
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getCoordinateBound}}
int GridCollectionFunction::
getCoordinateBound( int coordinate ) const
//==================================================================================
// /Description:
//  Get the bound for the given coordinate.
//
// /coordinate (input): component number, 0,1, or 2.
//
// /Return Values:
//    The bound for the coordinate. Unused coordinates have base=0 and bound=0
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "GridCollectionFunction::getCoordinateBound:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("GridCollectionFunction::getCoordinateBound:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBound();
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getCoordinateDimension}}
int GridCollectionFunction::
getCoordinateDimension( int coordinate ) const
//==================================================================================
// /Description:
//  Get the dimension for the given coordinate, dimension = bound-base+1
//
// /coordinate (input): component number, 0,1, or 2.
//
// /Return Values:
//    The dimension for the coordinate. Unused coordinates have dimension=1
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "GridCollectionFunction::getCoordinateDimension:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("GridCollectionFunction::getCoordinateDimension:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBound()
        -rcData->R[positionOfCoordinate(coordinate)].getBase()+1;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getFaceCentering}}
GridFunctionParameters::faceCenteringType GridCollectionFunction:: 
getFaceCentering() const 
//---------------------------------------------------------------------------------------------
// /Description:
//     Get the type of face centering.
//      For further explanation see {\ff setFaceCentering} and section\ref{sec:cellFace}.
// /Errors:  none.
// /Return Values: faceCenteringType.
// 
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  GridFunctionParameters::faceCenteringType fct=GridFunctionParameters::faceCenteringType(int(faceCentering()));
  // check that all the component grids are consistent!
  for( int grid=0; grid<numberOfGrids(); grid++ )
  {
    if( fct!=mappedGridFunctionList[grid].getFaceCentering() )
    {
      cout << "GridCollectionFunction::getFaceCentering:ERROR: the faceCenteringType for a gridCollection\n";
      cout << " is not equal to the faceCentering type for all the components!\n";
      Overture::abort("GridCollectionFunction::getFaceCentering:ERROR:");
    }
    if( positionOfFaceCentering()!=mappedGridFunctionList[grid].positionOfFaceCentering() )
    {
      cout << "GridCollectionFunction::getFaceCentering:ERROR: the positionOfFaceCentering for a gridCollection\n";
      cout << " is not equal to the positionOfFaceCentering for all the components!\n";
      Overture::abort("GridCollectionFunction::getFaceCentering:ERROR:");
    }
  }
  return GridFunctionParameters::faceCenteringType(int(faceCentering()));
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getGridCollection}}
GridCollection* GridCollectionFunction::
getGridCollection(const bool abortIfNull /* =TRUE */ ) const
//==================================================================================
// /Description:
//   Return a pointer to the GridCollection that this grid function is asscoaiated with
//   By default this function will abort if the pointer is NULL.
// /Return values:
//   A pointer to a GridCollection or NULL
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( abortIfNull && gridCollection==NULL )
  {
    cout << "GridCollectionFunction:getGridCollection:ERROR: The pointer to the gridCollection is NULL \n";
    Overture::abort("error");
  }
  return gridCollection;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getGridFunctionType}} 
GridFunctionParameters::GridFunctionType GridCollectionFunction::
getGridFunctionType(const Index & component0,  /* =nullIndex */
		    const Index & component1,  /* =nullIndex */
		    const Index & component2,  /* =nullIndex */
		    const Index & component3,  /* =nullIndex */
		    const Index & component4   /* =nullIndex */  ) const
//-----------------------------------------------------------------------------------------
// /Description:
//   return the type of the grid function
// /component0,component1,... (input): get type of the grid function corresponding to these
//    components. 
// /Return Values:
//    The grid function type, one of the enums in GridFunctionParameters::GridFunctionType.
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  GridFunctionParameters::GridFunctionType returnValue=GridFunctionParameters::general;
  if( numberOfGrids()>0 )
  {
    returnValue=mappedGridFunctionList[0].getGridFunctionType();
    for( int grid=1; grid< numberOfGrids(); grid++ )
    {
      if(  returnValue != mappedGridFunctionList[grid].getGridFunctionType() )
      {
	cout << "GridCollectionFunction::getGridFunctionType:ERROR: gridFunctionType's are not\n";
	cout << "the same on all the component grids! \n";
        break;
      }
    }
  }
  return returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getGridFunctionTypeWithComponents}}
GridFunctionParameters::GridFunctionTypeWithComponents GridCollectionFunction:: 
getGridFunctionTypeWithComponents(const Index & c0,  /* =nullIndex */
				  const Index & c1,  /* =nullIndex */
				  const Index & c2,  /* =nullIndex */
				  const Index & c3,  /* =nullIndex */
				  const Index & c4   /* =nullIndex */  ) const
//-----------------------------------------------------------------------------------------
// /Description:
//   return the type of the grid function with the number of components
// /c0,c1,... (input): get type of the grid function corresponding to these
//    components. 
// /Return Values:
//    The grid function type with number of components, one of the enums in GridFunctionParameters::GridFunctionTypeWithComponents.
// /Note:
//   In a faceCenteredAll grid function, the position taken by the faceRange does not count
//   as a component.
//   \begin{verbatim}
//     GridCollection gc(...); 
//     Range all;
//     floatGridCollectionFunction u(mg,floatGridCollectionFunction::faceCenterAll,2);  
//     u.getGridFunctionTypeWithComponents(); // == faceCenterAllWith1Component
//     u.getNumberOfComponents();             // == 1
//   \end{verbatim}   
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  GridFunctionParameters::GridFunctionTypeWithComponents returnValue=GridFunctionParameters::generalWith0Components;
  if( numberOfGrids()>0 )
  {
    returnValue=mappedGridFunctionList[0].getGridFunctionTypeWithComponents();
    for( int grid=1; grid< numberOfGrids(); grid++ )
    {
      if(  returnValue != mappedGridFunctionList[grid].getGridFunctionTypeWithComponents() )
      {
	cout << "GridCollectionFunction::getGridFunctionTypeWithComponents:ERROR: gridFunctionType's are not\n";
	cout << "the same on all the component grids! \n";
        break;
      }
    }
  }
  return returnValue;
}

bool GridCollectionFunction::
getIsACoefficientMatrix() const
{
  int num=0;
  for( int grid=0; grid< numberOfGrids(); grid++ )
  {
    if( mappedGridFunctionList[grid].getIsACoefficientMatrix() ||
        mappedGridFunctionList[grid].elementCount()==0 ) // *wdh* added 030822 (some grids may not be allocated)
    {
      // printf(" grid=%i getIsACoefficientMatrix()=%i element count = %i\n",grid,
      //    mappedGridFunctionList[grid].getIsACoefficientMatrix(),mappedGridFunctionList[grid].elementCount());
      
      num++;
    }
  }
  if( num>0 && num!=numberOfGrids() )
  {
    printf("GridCollectionFunction::getIsACoefficientMatrix(): WARNING: Not all mappedGridFunctions in this\n"
           "collection are coefficient matrices! Number that are = %i, total number=%i\n",num,
	   numberOfGrids());
  }
  return num>0;
}


  // inquire cell centredness:
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getIsCellCentered}}
bool GridCollectionFunction::
getIsCellCentered(const Index & axis0,       /* =nullIndex */
		  const Index & component0,  /* =nullIndex */
		  const Index & component1,  /* =nullIndex */ 
		  const Index & component2,  /* =nullIndex */ 
		  const Index & component3,  /* =nullIndex */
		  const Index & component4,  /* =nullIndex */
		  const Index & grid0        /* =nullIndex */  ) const
// ====================================================================================================
//  /Description:
//     Determine the cell centeredness of a grid function.
//     See the detail description with te MappedGridFunction version of this function.
//  /axis0 (input): if axis0=nullIndex (default) then all axes are checked
//  /component0 (input): if component0=nullIndex (default) then all components are checked
//  /component1 (input): if component1=nullIndex (default) then all components are checked
//  /component2 (input): if component2=nullIndex (default) then all components are checked
//  /component3 (input): if component3=nullIndex (default) then all components are checked
//  /component4 (input): if component4=nullIndex (default) then all components are checked
//
//  /Return Values: TRUE or FALSE
//
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
// ====================================================================================================
{
  bool returnValue=TRUE;
  Index G = (grid0.length()==0) ? Index(0,numberOfGrids()) : grid0;

  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )      
    returnValue=returnValue && 
      mappedGridFunctionList[grid].getIsCellCentered(axis0,component0,component1,component2,component3,component4);
  return returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getIsFaceCentered}}
bool GridCollectionFunction:: 
getIsFaceCentered(const int   & axis0,      /* =forAll    */
                  const Index & component0, /* =nullIndex */
		  const Index & component1, /* =nullIndex */
		  const Index & component2, /* =nullIndex */
		  const Index & component3, /* =nullIndex */
		  const Index & component4, /* =nullIndex */
		  const Index & grid0       /* =nullIndex */) const 
//==================================================================================
// /Description:
//   Determine if a given component of this grid function is face-centred along a given axis.
//   By default check all axes and all components.
// /axis0: check if the components are face centred along this axis. By default check if
//   the components are face centred in ANY direction.
// /component0, component1,... (input): check the value for these components, by default
//   check all components.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  bool returnValue=TRUE;
  Index G = (grid0.length()==0) ? Index(0,numberOfGrids()) : grid0;

  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )      
    returnValue=returnValue && 
                  mappedGridFunctionList[grid].getIsFaceCentered(axis0,component0,component1,component2,
                                                                       component3,component4);
  return returnValue;
}  



// This macro makes the name[] array look like a multidimensional array
#define GFNAME(c0,c1,c2,c3,c4) name[1+(c0-R[positionOfComponent(0)].getBase())+R[positionOfComponent(0)].length()*(  \
                                      (c1-R[positionOfComponent(1)].getBase())+R[positionOfComponent(1)].length()*(  \
                                      (c2-R[positionOfComponent(2)].getBase())+R[positionOfComponent(2)].length()*(  \
                                      (c3-R[positionOfComponent(3)].getBase())+R[positionOfComponent(3)].length()*(  \
                                      (c4-R[positionOfComponent(4)].getBase())))))]

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getName}}
aString GridCollectionFunction::
getName(const int & component0, /* =defaultValue */
        const int & component1, /* =defaultValue */
        const int & component2, /* =defaultValue */
        const int & component3, /* =defaultValue */
        const int & component4  /* =defaultValue */ ) const 
//==================================================================================
// /Description:
//   Get the name of the grid function or a component as in
//   \begin{verbatim} 
//     aString nameOfGridFunction = u.getName();
//     aString nameOfComponent0   = u.getName(0);
//     aString nameOfComponent1   = u.getName(1);
//   \end{verbatim} 
// /name: the name of the grid function or component.
// /component0, component1, (input): get the name for this component. 
//    if all of component0,component1,component2 ==defaultValue then the name
//    of the grid function is returned. Otherwise the default value becomes
//    the base value for that component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( rcData->name && rcData->numberOfNames )
  {
    Range *R = rcData->R;  // make a reference to the array in rcData
    // dimensionName();
    int c[maximumNumberOfComponents] = { component0, component1, component2, component3, component4 };
    
    if( c[0]==defaultValue && c[1]==defaultValue && c[2]==defaultValue &&
        c[3]==defaultValue && c[4]==defaultValue )
      return rcData->name[0];   // return the name for the grid function
    else
    {
      for( int i=0; i<maximumNumberOfComponents; i++ )
      {
	if( c[i]==defaultValue )
	  c[i]=R[positionOfComponent(i)].getBase();
	else
	  assert(c[i] >= R[positionOfComponent(i)].getBase() &&
		 c[i] <= R[positionOfComponent(i)].getBound() );
      }
      return rcData->GFNAME(c[0],c[1],c[2],c[3],c[4]);
    }
  }
  else
  {
    return blankString;   // no names have been set yet
  }
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getNumberOfComponents}}  
int GridCollectionFunction::
getNumberOfComponents() const
//-----------------------------------------------------------------------------------------
// /Description:
//   return the number of components (0=scalar, 1=vector, 2=matrix, ...).
// /Return Values:
//    Valid values are 0,...,5 
// /Examples:
//   Here are some examples. Note the special case for grid functions created
//   with a {\ff faceRange}, the {\ff faceRange} position does NOT count as
//   a component.
//   \begin{verbatim}
//     GridCollection gc(...); 
//     Range all;
//     floatGridCollectionFunction u(gc);                           // 0 components
//     floatGridCollectionFunction u(gc,all,all,all);               // 0 components
//     floatGridCollectionFunction u(gc,all,all,all,1);             // 1 component
//     floatGridCollectionFunction u(gc,all,all,all,2,2);           // 2 components
//     floatGridCollectionFunction u(gc,all,all,all,faceRange);     // 0 components
//     floatGridCollectionFunction u(gc,all,all,all,3,faceRange);   // 1 component
//   \end{verbatim}   
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  int returnValue=0;
  if( mappedGridFunctionList.getLength()>0 )
  {
    returnValue=mappedGridFunctionList[0].getNumberOfComponents();
    const int ng=min(numberOfGrids(),mappedGridFunctionList.getLength());
    for( int grid=1; grid<ng; grid++ ) // *wdh* 020806
    {
      // If we are in the midst of updating a gridCollectionFunction the numberOfComponents may be zero
      // on some grids
      const int nc=mappedGridFunctionList[grid].getNumberOfComponents();
      if(  returnValue != nc && nc!=0 )
      {
	cout << "GridCollectionFunction::getNumberOfComponents:ERROR: number of components are not\n";
	cout << "the same on all the component grids! \n";
        printf(" (grid,numberOfComponents)=");
        for( int g=0; g<ng; g++ )
          printf(" (%i,%i)",g,mappedGridFunctionList[g].getNumberOfComponents());
	printf("\n");
	Overture::abort();
	
        break;
      }
    }
  }
  return returnValue;
}



//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{getOperators}}
GridCollectionOperators* GridCollectionFunction::
getOperators() const
//==================================================================================
// /Description:
//    get the operators used with this grid function. Return NULL if there are none.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  // the case below should safe as the base class cannot be made
  if( operators ) 
    return (GridCollectionOperators*)operators;
  else
    return NULL;
}

//-----------------------------------------------------------------------------------------
// return the current values for the Ranges
//-----------------------------------------------------------------------------------------
void GridCollectionFunction::
getRanges(Range & R0,   
	  Range & R1,
	  Range & R2,
	  Range & R3,
	  Range & R4,
	  Range & R5,
	  Range & R6,
	  Range & R7 ) const  
{
  R0=rcData->R[0];  
  R1=rcData->R[1];  
  R2=rcData->R[2];  
  R3=rcData->R[3];  
  R4=rcData->R[4];  
  R5=rcData->R[5];  
  R6=rcData->R[6];  
  R7=rcData->R[7];  
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{interpolate}}  
int GridCollectionFunction::
interpolate(const Range & C0 /* = nullRange */,
	    const Range & C1 /* = nullRange */,
	    const Range & C2 /* = nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description:
//  Interpolate using default Interpolant or one found in the grid collection
// /C0, C1, C2 (input): optionally specify components to interpolate. For example
//    {\tt  u.interpolate(Range(1,2))} to interpolate components 1 and 2.
// /Return Values:
//    0=success, $>0$ indicates an error.
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( numberOfGrids()==0 )
    return 0;

  if( rcData->interpolant!=NULL )
    return interpolate( *(rcData->interpolant),C0,C1,C2 );  // interpolate
  else if( gridCollectionData->interpolant !=NULL  )
    return interpolate( *(gridCollectionData->interpolant),C0,C1,C2 );  // use interpolant from gridCollection
  else
  {
    cout << "GridCollectionFunction::interpolate:ERROR: Sorry but I cannot interpolate\n";
    cout << "...since I cannot find an interpolant to use\n";
    cout << "...you might want to make an Interpolant\n";
    return 1;
  }
}

//------------------------------------------------------------------------------
//  Interpolate using an Interpolant
//
//  Notes:
//   o The lines if_float, if_double and if_int are treated by the gf.p perl script
//------------------------------------------------------------------------------
#ifndef OV_USE_DOUBLE
if_float  int GridCollectionFunction::interpolate(Interpolant & interpolant,
if_float                                          const Range & C0,
if_float	                                  const Range & C1,
if_float                                          const Range & C2)
  if_double int GridCollectionFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &)
  if_int    int GridCollectionFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &)
{
  if_float return interpolant.interpolate( *this,C0,C1,C2 );
  if_double cout << "GridCollectionFunction::interpolate: sorry unable to interpolate\n"; return 1;
  if_int    cout << "GridCollectionFunction::interpolate: sorry unable to interpolate\n"; return 1;
}
#else
if_double int GridCollectionFunction::interpolate(Interpolant & interpolant,
if_double                                         const Range & C0,
if_double	                                  const Range & C1,
if_double                                         const Range & C2)
if_float  int GridCollectionFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &)
if_int    int GridCollectionFunction::interpolate(Interpolant &, const Range &, const Range &, const Range &)
{
  if_double return interpolant.interpolate( *this,C0,C1,C2 );
  if_float  cout << "GridCollectionFunction::interpolate: sorry unable to interpolate\n"; return 1;
  if_int    cout << "GridCollectionFunction::interpolate: sorry unable to interpolate\n"; return 1;
}
#endif
  

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{isNull}}
bool GridCollectionFunction::
isNull()
//==================================================================================
// /Description:
//   Return TRUE if this grid function is null (has no grid associated with it).
//    
// /Return value:
//   Return TRUE if this grid function is null, otherwise return FALSE.
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  return  gridCollection==NULL;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{link}}
void GridCollectionFunction::
link(const GridCollectionFunction & gcf,
     const Range & R0, /* = nullRange */
     const Range & R1, /* = nullRange */
     const Range & R2, /* = nullRange */
     const Range & R3, /* = nullRange */
     const Range & R4  /* = nullRange */ )
//======================================================================================================
//
// /Description:
//   The {\ff link} member function can be used to link a grid function
//   to a specific component of another grid function. 
// /mgf (input): link to this
// /R0, R1, ..., R4 (input): indicate which components to link to. Note that the
//    Ranges for the linked grid function always start at 0. Use updateToMatchGridFunction
//    to change this.
//
// /Examples:
//    See the examples in the documentation for the MappedGridFunction version of link.
//
//  /Errors: Attempt to link to invalid components.
//  /Return Values: none.
// 
//  /Notes:
//     The linkee function will acquire the same operators as the function being linked to.
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//======================================================================================================
{
  gridCollectionData = gcf.gridCollectionData;
  gridCollection = gcf.gridCollection;
  operators=gcf.operators;

  *rcData=*gcf.rcData;  // deep copy of data

  // make the list the correct length
  while( mappedGridFunctionList.getLength() < numberOfGrids() )
    mappedGridFunctionList.addElement( );
  while( mappedGridFunctionList.getLength() > numberOfGrids() )
    mappedGridFunctionList.deleteElement( );
  
  // link the component grids
  int grid;
  for( grid=0; grid< numberOfGrids(); grid++ )
    (*this)[grid].link(gcf[grid],R0,R1,R2,R3,R4);

  // ---assign the component info from one of the mapped grids----
  //    ++++they should all be the same++++
  assert( numberOfGrids()>0 );
  grid=0;
  for( int i=0; i<maximumNumberOfIndicies; i++ )
  {
    rcData->positionOfComponent[i]    =(*this)[grid].positionOfComponent(i);
    rcData->positionOfCoordinate[i]   =(*this)[grid].positionOfCoordinate(i);
  }
  
  rcData->positionOfFaceCentering=(*this)[grid].positionOfFaceCentering(); 
  rcData->faceCentering          =(*this)[grid].faceCentering();
  Range *R = rcData->R;
  (*this)[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );  // do this so component Ranges are correct
  for( int axis=0; axis<3; axis++ )                    
    R[positionOfCoordinate(axis)]=nullRange;     // keep coordinate ranges null


  // give names from the linkee  ** is this what we want to do ? ***
  setName( gcf.getName() );   // set name for grid function
  for( int c4=R[positionOfComponent(4)].getBase(); c4<R[positionOfComponent(4)].getBound(); c4++ )
    for( int c3=R[positionOfComponent(3)].getBase(); c3<R[positionOfComponent(3)].getBound(); c3++ )
      for( int c2=R[positionOfComponent(2)].getBase(); c2<R[positionOfComponent(2)].getBound(); c2++ )
	for( int c1=R[positionOfComponent(1)].getBase(); c1<R[positionOfComponent(1)].getBound(); c1++ )
	  for( int c0=R[positionOfComponent(0)].getBase(); c0<R[positionOfComponent(0)].getBound(); c0++ )
	    setName( gcf.getName(c0,c1,c2,c3,c4),c0,c1,c2,c3,c4);

  // update refinementLevel, multigridLevel etc.
  updateCollections();  // *wdh* 000503

}


//-----------------------------------------------------------------------------------------------
// Old style link:
//-----------------------------------------------------------------------------------------------
void GridCollectionFunction::
link(const GridCollectionFunction & gcf, const int componentToLinkTo, const int numberOfComponents )
{
  Range R(componentToLinkTo,componentToLinkTo+numberOfComponents-1);
  link( gcf,R );
}



//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{multiply}}
GridCollectionFunction &
multiply( const GridCollectionFunction & a, const GridCollectionFunction & coeff_ )
//==================================================================================
// /Description:
//    Multiply a grid function times a coefficient matrix. Use this function
//  to multiply a scalar grid function "a" times a coefficient matrix "coeff".
//  The result is saved in coeff and returned by reference.    
//  \begin{verbatim}
//        coeff[grid](M,I1,I2,I3) <- a[grid](I1,I2,I3)*coeff[grid](M,I1,I2,I3)
//  \end{verbatim}
//  This is a non-member function and is called with
//  \begin{verbatim}
//    multiply(u,coeff)
//  \end{verbatim}
//   
//
// /a (input) : a scalar grid function.
// /coeff (input) : a grid function in the shape a coefficient matrix (1 component in position 0)
// /Return value: coeff is returned by reference
// /Notes:
//   If "a" is an expression ({\tt multiply(u+v,coeff)}) then this function will properly delete "a". Note that one
//   should call "evaluate" on an expression that is being passed to a function that is
//    not a member function of this class.
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  GridCollectionFunction & coeff = (GridCollectionFunction &) coeff_; // cast away const (avoids compiler warnings)
  
  Index I1,I2,I3;
  GridCollection & cg = *coeff.getGridCollection();
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    if( a[grid].getLength(3)!=1 )
    {
      cout << "GridCollectionFunction:multiply(a,coeff):ERROR: `a' should be a scalar grid function\n";
      Overture::abort("multiply(a,coeff):ERROR");
    }
    a[grid].reshape(1,Range(a[grid].getBase(0),a[grid].getBound(0)),
                      Range(a[grid].getBase(1),a[grid].getBound(1)),
		      Range(a[grid].getBase(2),a[grid].getBound(2)));
    
    getIndex(cg[grid].dimension(),I1,I2,I3);
    for( int m=coeff[grid].getBase(0); m<=coeff[grid].getBound(0); m++ )
      coeff[grid](m,I1,I2,I3)*=a[grid](0,I1,I2,I3);

    a[grid].reshape(Range(a[grid].getBase(1),a[grid].getBound(1)),
                    Range(a[grid].getBase(2),a[grid].getBound(2)),
		    Range(a[grid].getBase(3),a[grid].getBound(3)));
  }
  if( a.temporary )
  { // Delete a if it is a temporary -- this would happen if "a" were an expression,
    // the user could have called evaluate to prevent this case
    GridCollectionFunction *temp = (GridCollectionFunction*) &a; 
    delete temp;                                                  
  }
  return coeff;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{numberOfMultigridLevels}}
int GridCollectionFunction::
numberOfMultigridLevels() const
//======================================================================================================
// /Description:
//    Return the number of multigrid levels contained in this grid function. See the grid
//  documentation for further details. 
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//======================================================================================================
{
  if( gridCollection!=NULL )
    return gridCollection->numberOfMultigridLevels();
  else
    return 0;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{numberOfRefinementLevels}}
int GridCollectionFunction::
numberOfRefinementLevels() const
//======================================================================================================
// /Description:
//    Return the number of refinement levels contained in this grid function. See the grid
//  documentation for further details. 
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//======================================================================================================
{
  if( gridCollection!=NULL )
    return gridCollection->numberOfRefinementLevels();
  else
    return 0;
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{operator = GridCollectionFunction}}
GridCollectionFunction & GridCollectionFunction::
operator= ( const GridCollectionFunction & cgf )
//======================================================================================================
// /Description:
//    Set one grid-function equal to another. This is a shallow copy where only the
//    array data is copied. An error occurs if the two grid functions do not have the same number of
//    grids or the array data in each mappedGridFunction are not conformable.
//    This operation has the same affect as the {\tt dataCopy} member function.
//    An exception to this rule is when the grid function to the left of the equals operator
//    is a `null' grid function (one that has no grid associated with it such as a grid function
//    built by the default constructor). In this case  a deep copy is performed.
// /Examples:
//    Here are some examples for a realCompositeGridFunction. The examples are the same for
//   GridCollectionFunction's.
// \begin{verbatim}
//    CompositeGrid cg(...);
//    realCompositeGridFunction u(cg),v(cg),w.
//    u=1.;
//    v=u;                 // only the array data is copied.
//    w=u;                 // this is a DEEP copy since w is null.
//    u=v+w;               // u will steal the data from the temporary `v+w'
//    u=3;                 // does NOT call this = operator, uses grid-function=scalar
//    u.dataCopy(v+w);     // only copies array data
//    u.updateToMatchGridFunction(v);  // this is a real deep copy.
//    realCompositeGridFunction a = u;  // does NOT call this = operator, calls copy constructor
// \end{verbatim}  
// % /Notes: ~~
// %   \begin{itemize}
// %     \item This is a deep copy (except as noted below) which means that the left-operand
// %       will acquire the same number of components, and cell-centredness etc. as the right
// %       operand. Use the {\ff dataCopy} member function if you only want to copy the array
// %       data.  Use the {\ff updateToMatchGridFunction} if you want to do a real deep copy.   
// %     \item See the notes with the mappedGridFunction operator=.
// %    \item If the operators have already been assigned (with setOperators) for left operand
// %       (*this) then the operators are NOT set equal to those of the right operand.
// %    \item If the left operand is a Coefficient matrix with a sparse-matrix representation
// %      (set with isACoefficientMatrix(TRUE)) then the sparse representation is NOT set
// %      equal to the right operand's sparse representation.
// %   \end{itemize}      
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//======================================================================================================
{
  if( this == &cgf )
    return *this;
  
  if( gridCollection!=0 && cgf.gridCollection!=0 )
  {
    // SHALLOW COPY
    if( numberOfGrids()!=cgf.numberOfGrids() )
    {
      printf("GridCollectionFunction:operator = : ERROR: trying to assign two grid functions that do not\n"
             "  have the same number of grids!\n");
      Overture::abort("GridCollectionFunction:operator = : ERROR");
    }
    for( int grid=0; grid<numberOfGrids(); grid++ )
    {
      // **** this is wrong for links **** we need to know if we a linker or linkee to do this !!! ***
      if( FALSE && cgf.temporary ) // if the rhs is a temporary we can just steal the data rather than copying it!
        ((doubleDistributedArray&)mappedGridFunctionList[grid]).reference((doubleDistributedArray &)cgf[grid]);   
      else
        mappedGridFunctionList[grid]=cgf[grid];   
    }
  }
  else if( cgf.gridCollection!=0 )
  {
    // DEEP COPY
    *rcData=*cgf.rcData; // deep copy
    gridCollectionData    =cgf.gridCollectionData;
    gridCollection        =cgf.gridCollection;
    if( operators==NULL )
      operators             =cgf.operators;
    mappedGridFunctionList=cgf.mappedGridFunctionList; // this IS a deep copy
    // *wdh* 000411if( className=="GridCollectionFunction" )
    if( getClassName()=="GridCollectionFunction" )
    {
      // only copy these lists if we are a GridCollectionFunction (i.e. not a CompositeGridFunction)
/* ----
    multigridLevel         =cgf.multigridLevel; 
    refinementLevel        =cgf.refinementLevel; 
    rcData->multigridLevel .reference(multigridLevel);
    rcData->refinementLevel.reference(refinementLevel);
---- */
    // update refinementLevel etc.
      updateCollections();  // *wdh* 980619 .. do NOT copy the above lists!
    }
  }
  else
  { // cgf is NULL but this is not.
    destroy();
  }
  
  // *wdh* 000411  if( cgf.className=="GridCollectionFunction" )
  if( cgf.getClassName()=="GridCollectionFunction" )
  {
    if( cgf.temporary )
    { // Delete cgf if it is a temporary
      GridCollectionFunction *temp = (GridCollectionFunction*) &cgf; 
      delete temp;                                                  
    }
  }
  
  return *this;
}


/* ---- this is inlined ---
//-----------------------------------------------------------------------------------------
// The [] operator returns a MappedGridFunction
//-----------------------------------------------------------------------------------------
MappedGridFunction & GridCollectionFunction::
operator[]( const int grid ) const
{
  return mappedGridFunctionList[grid];
}
---- */




//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{periodicUpdate}}
void GridCollectionFunction::
periodicUpdate(const Range & C0 /* =nullRange */ ,
	       const Range & C1 /* =nullRange */ ,
	       const Range & C2 /* =nullRange */ ,
	       const Range & C3 /* =nullRange */ ,
	       const Range & C4 /* =nullRange */ ,
	       const bool & derivativePeriodic /* =FALSE */)
//==================================================================================
// /Description:
//   Swap periodic edges of the grid function.
//   Assign values to {\tt side=1} boundary lines
//          \[ 
//             {\tt i_{\tt axis}={\tt mg.gridIndexRange()(1,axis)}}~~{\tt axis}=0,1,..,{\tt mg.numberOfDimensions}
//          \]
//      ({\tt mg} is the {\tt MappedGrid} associated with this grid function)
//          as well as all ghost lines on all sides that have periodic boundary conditions.
//
// /C0,C1,...C4 (input) : specify which components to update. By default update all components.
// /derivativePeriodic (input): if TRUE we assume that the grid function is not actually
//  periodic but that only it's derivative is. *** This is not implemented yet ***
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid< numberOfGrids(); grid++ )
  {
    mappedGridFunctionList[grid].periodicUpdate(C0,C1,C2,C3,C4,derivativePeriodic);  // update component grids 
  }
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{put}}
int GridCollectionFunction::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   Output the grid function onto a database file.
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
// /Notes: ~~
//
//   \noindent First some definitions
//   \begin{itemize}
//     \item $N$={\tt maximumNumberOfIndicies} The maximum number of dimensions in a grid function
//        (current value =8).
//     \item $N_A$={\tt numberOfIndicies} The maximum number of A++ dimensions (current value =4).
//   \end{itemize}   
//
//   \noindent Here are the items that are saved in a data base.
//   \begin{description}
//     \item[numberOfComponentsGrids] (int) The number of component grids. This is equal to the
//         number of {\tt mappedGridFunctions} that are in the grid collection. 
//     \item[positionOfComponent] (IntegerArray(N)) The positions (base 0) of the component positions are
//       saved in {\tt positionOfComponent(i)}, $i=0,1,...,N$. 
//     \item[positionOfCoordinate] (IntegerArray(N)) The positions (base 0) of the 3 coordinate positions are
//       saved in {\tt position\-Of\-Coordinate(i)}, $i=0,1,2$.
//       The default value and the value for unused entries is $N$={\tt maximumNumberOfIndicies}.
//     \item[positionOfFaceCentering] (int) For a face centred grid function of standard type
//        this is the position of the face centering. For all other types of grid functions this
//        has a value of $-1$.
//     \item[faceCentering] (enum faceCenteringType) The face centering type for the grid function.
//          Default value is {\tt none}$=-1$.
//     \item[numberOfDimensions] The number of space dimensions, $0,1$, or $2$. 
//     \item[isACoefficientMatrix] (bool) If TRUE (=1) then this is a coefficient matrix, 
//         default is FALSE (=$0$).
//     \item[stencilType] (enum StencilTypes) The type of stencil for a coefficient matrix, 
//       default is {\tt standardStencil} (=$0$).
//     \item[stencilOffset] (int) The stencil offset for a coefficient matrix, default value = $0$.
//     \item[stencilWidth] (int) The stencil width for a coefficient matrix, default value = $0$.
//     \item[{R[i].base}] (int) ({\tt i=0,1,...,N}) The base of the Range object {\tt R[i]} which
//       holds the base and bound for position $i$. For unused positions the default is 0.
//       There is one extra Range, {\tt R[N]=Range(0,0)}  
//       which exists just for convenience.
//     \item[{R[i].bound}] (int) ({\tt i=0,1,...,N}) The bound of the Range objects {\tt R[i]}.
//        For unused positions the default is 0.
//     \item[numberOfNames] (int) The number of names that are saved. (see next item).
//     \item[{name[i]}] (aString) ({\tt i=0,1,...,{\tt numberOfNames-1}}) The names for the
//        grid function and its components.
//     \item[{mappedGridFunctionList[i]}] ({\tt i=0,1,...,{\tt numberOfGrids-1}}) The
//         {\tt mappedGridFunctions} that are found in this grid collection function.
//       See the documentation on the {\tt put} member function for a {\tt mappedGridFunction}   
//    \end{description}  
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"GridCollectionFunction");                      // create a sub-directory 
  
  subDir.setMode(GenericDataBase::streamOutputMode);

  int i;
  subDir.put( rcData->positionOfComponent,"positionOfComponent",maximumNumberOfIndicies );     
  subDir.put( rcData->positionOfCoordinate,"positionOfCoordinate",maximumNumberOfIndicies );   
  subDir.put( rcData->positionOfFaceCentering,"positionOfFaceCentering" );       
  subDir.put( rcData->faceCentering,"faceCentering" ); 
  char buff[40];
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
  {
    subDir.put( rcData->R[i].getBase(), sPrintF(buff,"R[%i].base",i) );
    subDir.put( rcData->R[i].getBound(),sPrintF(buff,"R[%i].bound",i) );
  }
  subDir.put( rcData->numberOfNames,"numberOfNames" );
  for( i=0; i<rcData->numberOfNames; i++ )
    subDir.put( rcData->name[i],sPrintF(buff,"name[%i]",i) );

  for( i=0; i<mappedGridFunctionList.getLength(); i++ )
    mappedGridFunctionList[i].put( subDir,sPrintF(buff,"mappedGridFunctionList[%i]",i) );

  delete &subDir;
  return 0;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{reference}}
void GridCollectionFunction::
reference( const GridCollectionFunction & cgf )
//-----------------------------------------------------------------------------------------
// /Description:
//   Use this function to reference one GridCollectionFunction to another.
//   When two (or more) grid functions have been
//   referenced they share the same array data so that changes to one grid function
//   will change all the other referenced grid functions. 
//   Only the array data is referenced. Other properties of the grid function such
//   as cell-centredness can be changed in the referenced grid function. The "shape"
//   of the referenced grid function can also be changed without changing 
//   the referencee:{\ff cgf}.  
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{

  *rcData=*cgf.rcData; // deep copy
  gridCollectionData    =cgf.gridCollectionData;
  gridCollection        =cgf.gridCollection;
  operators             =cgf.operators;
  mappedGridFunctionList.reference(cgf.mappedGridFunctionList); 

  multigridLevel .reference(cgf.multigridLevel); 
  refinementLevel.reference(cgf.refinementLevel); 
  rcData->multigridLevel.reference(multigridLevel);
  rcData->refinementLevel.reference(refinementLevel);
}



/* --------
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setBoundaryConditionValue}}
void GridCollectionFunction:: 
setBoundaryConditionValue(const real & value,
			  const int & component,  // =forAll
			  const int & index,      // =forAll
			  const int & side,       // =forAll
			  const int & axis,       // =forAll
			  const int grid0         // =forAll
                         )
//==================================================================================
// /Description:
//   Set some values for boundary conditions. This routine just calls the 
//   function of the same name in the MappedGridOperators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( getOperators()!=NULL )                                                                                       
  {                                                                                                           
    getOperators()->setBoundaryConditionValue(value,component,index,side,axis,grid0); // *****
  }                                                                                                           
  else                                                                                                        
  {                                                                                                           
    cout << "GridCollectionFunction:ERROR:trying to setBoundaryConditionValue without defining an BC routine\n";
  }                                                                                                           
}

------ */

//! Set the data allocation option.
/*
  \param option (input):  bit 1 : do not allocate data for rectangular grids.
                       :  bit 2 : do not allocate data for rectangular grids except on the coarsest
                                  multigrid level (i.e. only allocate data for rectangular grids on the
                                  coarsest MG level)
 */
void GridCollectionFunction::
setDataAllocationOption(int option )
{
  dataAllocationOption=option;
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setFaceCentering}}
void GridCollectionFunction::
setFaceCentering(const int & axis /* =defaultValue */ )
//---------------------------------------------------------------------------------------------
// /Description:
//   Set the type of face centering, the behaviour of this function depends on whether the
//    argument "axis" has been specified or else if the current value for getFaceCentering().
// /axis (input):
//    \begin{enumerate}
//      \item if "axis" is given then make all components face centred in direction=axis
//      \item if getFaceCentering()==all : make components face centered in all directions, the
//        grid function should have been contructed or updated using the faceRange to specify
//        which Index is to be used for the "directions"
//    \end{enumerate}
//    For further explanation see section\ref{sec:cellFace}.
// 
//  /Author: WDH
//\end{GridCollectionFunctionInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  for( int grid=0; grid<numberOfGrids(); grid++ )
    mappedGridFunctionList[grid].setFaceCentering(axis);
  if( numberOfGrids()>0 )
    rcData->faceCentering=mappedGridFunctionList[0].faceCentering();
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setInterpolant}}
void GridCollectionFunction::
setInterpolant(Interpolant *interpolant ) 
//-----------------------------------------------------------------------------------------
// /Description:
//    Set a pointer to an interpolant to use. This will NOT change the interpolant
//  associated with the GridCollection.
//\end{GridCollectionFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  rcData->interpolant=interpolant;
}


void GridCollectionFunction::
setIsACoefficientMatrix(const bool trueOrFalse, 
			const int stencilSize,
			const int numberOfGhostLines,
			const int numberOfComponentsForCoefficients,
                        const int offset0 )
// **** also change the version in compositeGridFunction ******
{
  if( numberOfGrids()==0 )
  {
    cout << "GridCollectionFunction::setIsACoefficientMatrix:Warning: there are no grids in this collection\n";
    cout << "This call will have no effect, you should call setIsACoefficientMatrix after the providing\n";
    cout << "the GridCollectionFunction with a GridCollection \n";
  }
  
  GridCollection & gc = *gridCollection;
  int l,grid;
  for( l=0; l<gc.numberOfMultigridLevels(); l++ )
  {
    GridCollection & m =  gc.numberOfMultigridLevels()==1 ? gc : gc.multigridLevel[l];
    int offset=offset0;
    for( int g=0; g< m.numberOfComponentGrids(); g++ )
    {
      const int grid = m.gridNumber(g);
      if( !( m[g].isRectangular() && ( (dataAllocationOption & 1) || 
				    ((dataAllocationOption & 2) && l <gc.numberOfMultigridLevels()-1) )) )
      {
	mappedGridFunctionList[grid].setIsACoefficientMatrix(trueOrFalse,stencilSize,numberOfGhostLines,
							     numberOfComponentsForCoefficients,offset); 
      }
      // printf("GridCollectionFunction::setIsACoefficientMatrix grid=%i, offset=%i\n",grid,offset);
      offset+=numberOfComponentsForCoefficients
	*(gc[grid].dimension()(End,axis1)-gc[grid].dimension()(Start,axis1)+1)
	*(gc[grid].dimension()(End,axis2)-gc[grid].dimension()(Start,axis2)+1)
	*(gc[grid].dimension()(End,axis3)-gc[grid].dimension()(Start,axis3)+1);
    }
  }

  // *** we now need to update the information in the refinement and multigrid lists.
  if( gc.computedGeometry() & GridCollection::THErefinementLevel )
  {
    if( refinementLevel.getLength() != gc.numberOfRefinementLevels() )
    {
      printf("GridCollectionFunction::setIsACoefficientMatrix: consistency error. The number of refinement levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setIsACoefficientMatrix\n",
	     gc.numberOfRefinementLevels(),refinementLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfRefinementLevels(); l++ )
    {
      GridCollection & rl =gc.refinementLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	refinementLevel[l][grid].setIsACoefficientMatrix(mappedGridFunctionList[rl.gridNumber(grid)].sparse);
    }
  }
  if(  gc.computedGeometry() & GridCollection::THEmultigridLevel )
  {
    if( multigridLevel.getLength() != gc.numberOfMultigridLevels() )
    {
      printf("GridCollectionFunction::setIsACoefficientMatrix: consistency error. The number of multigrid levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setIsACoefficientMatrix\n",
	     gc.numberOfMultigridLevels(),multigridLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfMultigridLevels(); l++ )
    {
      GridCollection & rl =gc.multigridLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	multigridLevel[l][grid].setIsACoefficientMatrix(mappedGridFunctionList[rl.gridNumber(grid)].sparse);
    }
  }


}




//====================================================================================
// change cell centredness:
//
//  By default change all axes, components and grids
//====================================================================================
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setIsCellCentered}}
void GridCollectionFunction::
setIsCellCentered(const bool trueOrFalse, 
                  const Index & axis0,       /* =nullIndex */
		  const Index & component0,  /* =nullIndex */ 
		  const Index & component1,  /* =nullIndex */ 
		  const Index & component2,  /* =nullIndex */ 
		  const Index & component3,  /* =nullIndex */ 
		  const Index & component4,  /* =nullIndex */
		  const Index & grid0        /* =nullIndex */ )
//==================================================================================
// /Description:
//   Change the cell centered-ness of the grid function. By default set
//   all components and all grids.
// /trueOfFalse (input): make cell-centred or not.
// /axis0: set the value for this axis, by default set all axes.
// /component0, component1, (input): set the value for these components, by default
//   set all components.
// /grid0 (input): set this s component grid.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Index G = (grid0.length()==0) ? Index(0,numberOfGrids()) : grid0;
  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )      
    mappedGridFunctionList[grid].setIsCellCentered(trueOrFalse,axis0,component0,component1,component2,
                                                                       component3,component4);
  // reset face centering, if any  
  rcData->positionOfFaceCentering=-1;
  rcData->faceCentering=GridFunctionParameters::none;
}

//===============================================================================
// set a component to be face-centred along a given axis
//===============================================================================
void GridCollectionFunction::
//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setIsFaceCentered}}
setIsFaceCentered(const int   & axis0,       /* =forAll    */ 
		  const Index & component0,  /* =nullIndex */ 
		  const Index & component1,  /* =nullIndex */ 
		  const Index & component2,  /* =nullIndex */ 
		  const Index & component3,  /* =nullIndex */ 
		  const Index & component4,  /* =nullIndex */
		  const Index & grid0        /* =nullIndex */ )
//==================================================================================
// /Description:
//   Make a component of a grid function face centred along the given axis
// /axis0: set the value for this axis, by default set all axes.
// /component0, component1, (input): set the value for these components, by default
//   set all components.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Index G = (grid0.length()==0) ? Index(0,numberOfGrids()) : grid0;
  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )      
    mappedGridFunctionList[grid].setIsFaceCentered(axis0,component0,component1,component2,
                                                           component3,component4);
}




//=====================================================================
// Allocate space for the name array
//====================================================================
void GridCollectionFunction::
dimensionName()
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  // first count the total number of components
  int component,numberOfComponents=1;
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
  {
    component=positionOfComponent(i);
    if( component < maximumNumberOfIndicies )
      numberOfComponents*=R[component].length();
  }
  if( rcData->numberOfNames < numberOfComponents+1 )
  {
    aString *newName = ::new aString[numberOfComponents+1];
    int i;
    for( i=0; i<rcData->numberOfNames; i++ )
      newName[i]=rcData->name[i];
    for( i=rcData->numberOfNames; i<numberOfComponents+1; i++ )
      newName[i]=" ";
    delete [] rcData->name;
    rcData->name=newName;
    rcData->numberOfNames=numberOfComponents+1;
  }
}


//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setName}}
void GridCollectionFunction::
setName(const aString & name, 
        const int & component0,  /* =defaultValue */
        const int & component1,  /* =defaultValue */
        const int & component2,  /* =defaultValue */
        const int & component3,  /* =defaultValue */
        const int & component4   /* =defaultValue */  )
//==================================================================================
// /Description:
//   Set the name of the grid function or a component as in
//   \begin{verbatim} 
//     u.setName("nameOfGridFunction");  
//     u.setName("nameOfComponent0",0);
//     u.setName("nameOfComponent1",1);
//   \end{verbatim} 
// /name: the name of the grid function or component.
// /component0, component1,... (input): give the name for this component. 
//    if all of component0,component1,component2 ==defaultValue then the name
//    of the grid function is set. Otherwise the default value becomes
//    the base value for that component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  dimensionName();
  int c[maximumNumberOfComponents] = { component0, component1, component2, component3, component4 };
  
  if( c[0]==defaultValue && c[1]==defaultValue && c[2]==defaultValue &&
      c[3]==defaultValue && c[4]==defaultValue )
    rcData->name[0]=name;   // assign the name for the grid function
  else
  {
    int i;
    for( i=0; i<maximumNumberOfComponents; i++ )
    {
      if( c[i]==defaultValue )
        c[i]=R[positionOfComponent(i)].getBase();
      else if(c[i] < R[positionOfComponent(i)].getBase() ||
 	      c[i] > R[positionOfComponent(i)].getBound() )
      {
	printf("GridCollectionFunction::setName:ERROR component%i=%i is invalid ! \n",i,c[i]);
	printf(" It should be in the range (%i,%i) \n",R[positionOfComponent(i)].getBase(),
	       R[positionOfComponent(i)].getBound());
	return;
      }
    }
    rcData->GFNAME(c[0],c[1],c[2],c[3],c[4])=name;
  }
}

void GridCollectionFunction::
setPositionOfFaceCentering(const int number)
{ 
  rcData->positionOfFaceCentering=number; 
} 

//------------------------------------------------------------------------------
//   Derivatives:
//------------------------------------------------------------------------------

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{setOperators}}
void GridCollectionFunction::
setOperators(GenericCollectionOperators & operators0 )
//==================================================================================
// /Description:
//   Supply a derivative object to use for computing derivatives
//   on all component grids. This operator is used for the member functions
//   .x .y .z .xx .xy etc.
// /operators0: use these operators.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( gridCollection==0 )
  {
    printf("GridCollectionFunction::setOperators:WARNING: setting operators for a null GridCollectionFunction\n"
           "                                              This operation is being ignored. \n");
    return;
  }
  
  operators=&operators0; 
    
  int grid;
  for( grid=0; grid< numberOfGrids(); grid++ )
    mappedGridFunctionList[grid].setOperators((*getOperators())[grid]);  // set operators on component grids 

  GridCollection & gc = *gridCollection;
  int l;
  if( gc.computedGeometry() & GridCollection::THErefinementLevel )
  {
    if( refinementLevel.getLength() != gc.numberOfRefinementLevels() )
    {
      printf("GridCollectionFunction::setOperators:ERROR: consistency error. The number of refinement levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setOperators\n",
	     gc.numberOfRefinementLevels(),refinementLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfRefinementLevels(); l++ )
    {
      GridCollection & rl =gc.refinementLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	refinementLevel[l][grid].setOperators((*getOperators())[rl.gridNumber(grid)]);  
    }
  }
  if(  gc.computedGeometry() & GridCollection::THEmultigridLevel )
  {
    if( multigridLevel.getLength() != gc.numberOfMultigridLevels() )
    {
      printf("GridCollectionFunction::setOperators:ERROR: consistency error. The number of multigrid levels\n"
	     " in the grid =%i, but the number of levels in the grid function =%i. \n"
	     " Perhaps you need to call updateToMatchGrid for the grid function before calling setOperators\n",
	     gc.numberOfMultigridLevels(),multigridLevel.getLength());
      Overture::abort("error");
    }
    for( l=0; l<gc.numberOfMultigridLevels(); l++ )
    {
      GridCollection & rl =gc.multigridLevel[l];
      for( grid=0; grid< rl.numberOfGrids(); grid++ )
	multigridLevel[l][grid].setOperators((*getOperators())[rl.gridNumber(grid)]);  
    }
  }
  
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{updateCollections}}
int GridCollectionFunction::
updateCollections()
//==================================================================================
// /Description:
//    Update the refinementLevel (and eventually other collections)
//  This is a protected member.
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  if( gridCollection!=NULL )
  {
    GridCollection & gc = *gridCollection;
    int level;
    if( gc.computedGeometry() & GridCollection::THErefinementLevel )
    {  // update the refinement levels
      // cout << "GridCollectionFunction:updateCollections...\n";

      // keep track of which levels are changed
      IntegerArray wasChanged(max(1,gc.numberOfRefinementLevels()));
      wasChanged=FALSE;

      // refinementLevel[level] is a gridCollectionFunction representing the refinement level
      // make the list the correct length
      while( refinementLevel.getLength() < gc.numberOfRefinementLevels() )
      {
	level=refinementLevel.getLength();
	wasChanged(level)=TRUE;
	refinementLevel.addElement();         // add an element to the end of the list
	// *wdh* 001120 refinementLevel[level].initialize();  // initialize the gridCollectionFunction **already done**
      }
      while( refinementLevel.getLength() > gc.numberOfRefinementLevels() )
        refinementLevel.deleteElement();        // delete the last element

      // Each grid in the grid collection belongs to a refinementLevel
      // We need to go through each grid in order, find which level it sits on and then add it to the correct level
      IntegerArray gridNumber(gc.numberOfRefinementLevels());
      gridNumber=0;
      for( int grid=0; grid< gc.numberOfGrids(); grid++ )
      {
        level = gc.refinementLevelNumber(grid);
        assert( level>=0 && level < gc.numberOfRefinementLevels() );

        GridCollectionFunction & gcf = refinementLevel[level];
        if( gcf.mappedGridFunctionList.getLength()<gridNumber(level)+1 )
	{
          wasChanged(level)=TRUE;
  	  gcf.mappedGridFunctionList.addElement(mappedGridFunctionList[grid],gridNumber(level));
	}
	else
	{ // reference mappedGridFunction if it is not already there (no need to update grid function)
          // ** check data pointer to see if we have the same MappedGrid : I think this is ok **
          // if( gcf[gridNumber(level)].getDataPointer() != mappedGridFunctionList[grid].getDataPointer() ) 
          gcf[gridNumber(level)].reference(mappedGridFunctionList[grid]);
	}
        gridNumber(level)++;
      } 	
      for( level=0; level< gc.numberOfRefinementLevels(); level++ )
      {  // remove entries from refinementLevel[level] if it is too long
        // 98/9/10 while( refinementLevel[level].numberOfGrids() > gc.refinementLevel[level].numberOfGrids() )
        while( refinementLevel[level].mappedGridFunctionList.getLength() > gc.refinementLevel[level].numberOfGrids() )
        {
          wasChanged(level)=TRUE;
	  refinementLevel[level].mappedGridFunctionList.deleteElement();
	}
      }

      Range *R = rcData->R;
      for( level=0; level< gc.numberOfRefinementLevels(); level++ )
      { 
        GridCollectionFunction & gcf = refinementLevel[level];
	gcf.gridCollection = & gc.refinementLevel[level];
	gcf.gridCollectionData = gc.refinementLevel[level].rcData;
        // finish updating the grid collection if it was changed
	if( wasChanged(level) )
	{
          // cout << "GridCollectionFunction:updateCollections...refinement was changed\n";
	  gcf.constructor( gcf.gridCollection,gcf.gridCollection->rcData,
			   R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],FALSE );
	}
      }
    }
    if( gc.computedGeometry() & GridCollection::THEmultigridLevel )
    {  
      // update the multigrid levels
      // cout << "GridCollectionFunction:update multigrid levels...\n";
      // keep track of which levels are changed
      IntegerArray wasChanged(max(1,gc.numberOfMultigridLevels()));
      wasChanged=FALSE;

      // multigridLevel[level] is a gridCollectionFunction representing the multigridLevel level
      // make the list the correct length
      while( multigridLevel.getLength() < gc.numberOfMultigridLevels() )
      {
	level=multigridLevel.getLength();
	wasChanged(level)=TRUE;
	multigridLevel.addElement();         // add an element to the end of the list
	// *wdh* 001120 multigridLevel[level].initialize();  // initialize the gridCollectionFunction
      }
      while( multigridLevel.getLength() > gc.numberOfMultigridLevels() )
        multigridLevel.deleteElement();        // delete the last element

      // Each grid in the grid collection belongs to a multigridLevel
      // We need to go through each grid in order, find which level it sits on and then add it to the correct level
      IntegerArray gridNumber(gc.numberOfMultigridLevels());
      gridNumber=0;
      for( int grid=0; grid< gc.numberOfGrids(); grid++ )
      {
        level = gc.multigridLevelNumber(grid);
        assert( level>=0 && level < gc.numberOfMultigridLevels() );

        GridCollectionFunction & gcf = multigridLevel[level];
        if( gcf.mappedGridFunctionList.getLength()<gridNumber(level)+1 )
	{
          wasChanged(level)=TRUE;
  	  gcf.mappedGridFunctionList.addElement(mappedGridFunctionList[grid],gridNumber(level));
	}
	else
	{ // reference mappedGridFunction if it is not already there (no need to update grid function)
          // ** check data pointer to see if we have the same MappedGrid : I think this is ok **
          // if( gcf[gridNumber(level)].getDataPointer() != mappedGridFunctionList[grid].getDataPointer() ) 
          gcf[gridNumber(level)].reference(mappedGridFunctionList[grid]);
	}
        gridNumber(level)++;
      } 	

      int level;
      for( level=0; level< gc.numberOfMultigridLevels(); level++ )
      {  // remove entries from multigridLevel[level] if it is too long
        while( multigridLevel[level].mappedGridFunctionList.getLength() > gc.multigridLevel[level].numberOfGrids() )
        {
          wasChanged(level)=TRUE;
	  multigridLevel[level].mappedGridFunctionList.deleteElement();
	}
      }

      Range *R = rcData->R;
      for( level=0; level< gc.numberOfMultigridLevels(); level++ )
      { 
	GridCollectionFunction & gcf = multigridLevel[level];
	gcf.gridCollection = & gc.multigridLevel[level];
	gcf.gridCollectionData = gc.multigridLevel[level].rcData;
        // finish updating the grid collection if it was changed
	if( wasChanged(level) )
	{
          // cout << "GridCollectionFunction:updateCollections...multigridLevel was changed\n";
	  gcf.constructor( gcf.gridCollection,gcf.gridCollection->rcData,
			   R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],FALSE );
	}
      }
    }
    
  }
  return 0;
}

  

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{updateToMatchGrid}}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid()
//==================================================================================
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;
  Range Ru[maximumNumberOfIndicies];

  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;
  int numberOfComponents=getNumberOfComponents();
  for( i=0; i<numberOfComponents; i++ )
    if( positionOfComponent(i)<maximumNumberOfIndicies )
      Ru[positionOfComponent(i)]=R[positionOfComponent(i)];
  
  if( positionOfFaceCentering()>=0 )
    Ru[positionOfFaceCentering()]=faceRange;
  return updateToMatchGrid( Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollection & gridCollection0)
//==================================================================================
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  gridCollection=&gridCollection0;
  gridCollectionData=gridCollection0.rcData;
  return updateToMatchGrid();
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(const Range & R0,  /* = nullRange */ 
		  const Range & R1,  /* = nullRange */
		  const Range & R2,  /* = nullRange */
		  const Range & R3,  /* = nullRange */
		  const Range & R4,  /* = nullRange */
		  const Range & R5,  /* = nullRange */
		  const Range & R6,  /* = nullRange */
		  const Range & R7   /* = nullRange */ )
//=====================================================================================================
//
//\end{GridCollectionFunctionInclude.tex} 
//=====================================================================================================
{
  int returnValue=0;
  if( !gridCollectionData )
  {
    cout << "GridCollectionFunction::updateToMatchGrid:ERROR: you must first associate a gridCollection "
      "with this grid before calling update!\n";
    Overture::abort("GridCollectionFunction::updateToMatchGrid:ERROR");
  }

  if( numberOfGrids() <= 0 )
    return (updateReturnValue &)returnValue;
  
  int grid;  
  int length=mappedGridFunctionList.getLength();

  // first update the grid functions that will remain in the list
  for( grid=0; grid<min(length,numberOfGrids()); grid++ )
  {
    MappedGrid & mg = (*gridCollectionData)[grid];
    if( mg.isRectangular() && ( (dataAllocationOption & 1) || 
           ((dataAllocationOption & 2) && gridCollectionData->multigridLevelNumber(grid)<gridCollectionData->numberOfMultigridLevels-1) ) )
    {
      // In this case do not allocate space for a rectangular grid
      // returnValue &=mappedGridFunctionList[grid].updateToMatchGrid(mg,0);  // this didn't work
      MappedGridFunction & mgf = mappedGridFunctionList[grid];
      mgf.mappedGrid = &mg;
      mgf.grid=mgf.mappedGrid->rcData;
    }
    else
    {
      returnValue &=mappedGridFunctionList[grid].updateToMatchGrid(mg,R0,R1,R2,R3,R4,R5,R6,R7 );
    }
  }
  if( length < numberOfGrids() )
  {
    for( int grid=length; grid< numberOfGrids(); grid++ )
    {
      returnValue&=2;
       MappedGrid & mg = (*gridCollectionData)[grid];
       if( mg.isRectangular() && ( (dataAllocationOption & 1) || 
           ((dataAllocationOption & 2) && gridCollectionData->multigridLevelNumber(grid)<gridCollectionData->numberOfMultigridLevels-1) ) )
      {
	// In this case do not allocate space for a rectangular grid
        //	MappedGridFunction mgf( mg,0 );
        //        mgf.redim(0);  // ***

	MappedGridFunction mgf; 
        // I should add a function in MGF to do this:
        mgf.mappedGrid = &mg;
        mgf.grid=mgf.mappedGrid->rcData;
        #ifdef USE_PPP
  	  // do this to avoid a P++ bug -- cannot reference a null array
  	  mgf.redim(1);
	#endif

	mappedGridFunctionList.addElement( mgf );
      }
      else
      {
	MappedGridFunction mgf(mg,R0,R1,R2,R3,R4,R5,R6,R7);
	mappedGridFunctionList.addElement( mgf );
      }
    }
  }
  else if( length > numberOfGrids() )
  {
    for( int grid=length-1; grid >= numberOfGrids(); grid-- )
      mappedGridFunctionList.deleteElement(grid);
  }

  // assign the component info from one of the mapped grids
  // they should all be the same
  if( numberOfGrids()>=0 )
  {
    grid=0;
    for( int i=0; i<maximumNumberOfIndicies; i++ )
    {
      rcData->positionOfComponent[i]=(*this)[grid].positionOfComponent(i);
      rcData->positionOfCoordinate[i]=(*this)[grid].positionOfCoordinate(i);
    }
    
    rcData->positionOfFaceCentering=(*this)[grid].positionOfFaceCentering();
    rcData->faceCentering=(*this)[grid].faceCentering();
    Range *R = rcData->R;
    (*this)[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );  // do this so component Ranges are correct
    for( int axis=0; axis<3; axis++ )                    
      R[positionOfCoordinate(axis)]=nullRange;     // keep coordinate ranges null
  }

  // update refinementLevel etc.
  updateCollections();
  
  return (updateReturnValue &)returnValue;
}






//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollection & gc, 
		  const Range & R0, 
		  const Range & R1, /* =nullRange */
		  const Range & R2, /* =nullRange */
		  const Range & R3, /* =nullRange */
		  const Range & R4, /* =nullRange */
		  const Range & R5, /* =nullRange */
		  const Range & R6, /* =nullRange */
		  const Range & R7  /* =nullRange */ )
//==================================================================================
// /Description:
//     Update a grid function. Optionally specify a new grid and new Ranges.
// /gc (input): update to match this grid.
// /R0, R1, ... (input): Use these Range objects to determine the grid function dimensions.
// /Return Values:
//   Return a value from the enumerator {\ff updateReturnValue}:
//   \begin{verbatim}
//     enum updateReturnValue  // the return value from updateToMatchGrid is a mask of the following values
//     {
//       updateNoChange          = 0, // no changes made
//       updateReshaped          = 1, // grid function was reshaped
//       updateResized           = 2, // grid function was resized
//       updateComponentsChanged = 4  // component dimensions may have changed (but grid was not resized or reshaped)
//     };
//   \end{verbatim}
//  /Author: WDH
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  gridCollection=&gc;
  gridCollectionData=gc.rcData;
  return updateToMatchGrid( R0,R1,R2,R3,R4,R5,R6,R7 );
}

GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollection & gc, 
		  const int & i0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  gridCollection=&gc;
  gridCollectionData=gc.rcData;
  return updateToMatchGrid( Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollectionData & gcData, 
		  const Range & R0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  gridCollectionData=&gcData;
  return updateToMatchGrid( R0,R1,R2,R3,R4,R5,R6,R7 );
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollectionData & gridCollectionData0)
{
  gridCollectionData=&gridCollectionData0;
  return updateToMatchGrid();
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollection & gridCollection0, 
		  const GridFunctionParameters::GridFunctionType & type, 
		  const Range & component0, 
		  const Range & component1, /* =nullRange */
		  const Range & component2, /* =nullRange */
		  const Range & component3, /* =nullRange */
		  const Range & component4  /* =nullRange */ )
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type.
//   See the comments in the corresponding constructor.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  gridCollection=&gridCollection0;
  gridCollectionData=gridCollection0.rcData;
  int returnValue=0;

  if( numberOfGrids() <= 0 )
    return (updateReturnValue &)returnValue;
  
  int grid;  
  int length=mappedGridFunctionList.getLength();

  // first update the grid functions that will remain in the list
  for( grid=0; grid<min(length,numberOfGrids()); grid++ )
    returnValue &=mappedGridFunctionList[grid].updateToMatchGrid(
         gridCollection0[grid],type,component0,component1,component2,component3,component4 );

  if( length < numberOfGrids() )
  {
    for( int grid=length; grid< numberOfGrids(); grid++ )
    {
      returnValue&=2;
      MappedGridFunction mgf(gridCollection0[grid],type,component0,component1,component2,component3,component4 );
      mappedGridFunctionList.addElement( mgf );
    }
  }
  else if( length > numberOfGrids() )
  {
    for( int grid=length-1; grid > numberOfGrids(); grid-- )
      mappedGridFunctionList.deleteElement(grid);
  }

  // assign the component info from one of the mapped grids
  // they should all be the same
  if( numberOfGrids()>=0 )
  {
    grid=0;
    for( int i=0; i<maximumNumberOfIndicies; i++ )
    {
      rcData->positionOfComponent[i]=(*this)[grid].positionOfComponent(i);
      rcData->positionOfCoordinate[i]=(*this)[grid].positionOfCoordinate(i);
    }
    rcData->positionOfFaceCentering=(*this)[grid].positionOfFaceCentering();
    rcData->faceCentering=(*this)[grid].faceCentering();
    Range *R = rcData->R;
    (*this)[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );  // do this so component Ranges are correct
    for( int axis=0; axis<3; axis++ )                    
      R[positionOfCoordinate(axis)]=nullRange;     // keep coordinate ranges null
  }

  // update refinementLevel etc.
  updateCollections();

  return (updateReturnValue &)returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type, 
		  const Range & component0, 
		  const Range & component1, /* =nullRange */
		  const Range & component2, /* =nullRange */
		  const Range & component3, /* =nullRange */
		  const Range & component4  /* =nullRange */ )
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type.
//   See the comments in the corresponding constructor.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  return updateToMatchGrid(*(this->gridCollection),type,component0,component1,component2,component3,component4);
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(GridCollection & gridCollection0, 
		  const GridFunctionParameters::GridFunctionType & type)
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type, the components
//   are left unchanged.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;
  Range Rc[maximumNumberOfComponents];
  int i;
  for( i=0; i<maximumNumberOfComponents; i++ )
    Rc[i]=nullRange;
  int numberOfComponents=getNumberOfComponents();
  for( i=0; i<numberOfComponents; i++ )

    if( positionOfComponent(i)<maximumNumberOfIndicies )
      Rc[i]=R[positionOfComponent(i)];

  return updateToMatchGrid(gridCollection0,type,Rc[0],Rc[1],Rc[2],Rc[3],Rc[4]);
	   
}
//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type)
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type, the components
//   are left unchanged.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;
  Range Rc[maximumNumberOfComponents];
  int i;
  for( i=0; i<maximumNumberOfComponents; i++ )
    Rc[i]=nullRange;
  int numberOfComponents=getNumberOfComponents();
  for( i=0; i<numberOfComponents; i++ )

    if( positionOfComponent(i)<maximumNumberOfIndicies )
      Rc[i]=R[positionOfComponent(i)];

  return updateToMatchGrid(type,Rc[0],Rc[1],Rc[2],Rc[3],Rc[4]);
}




//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGridFunction(const GridCollectionFunction & cgf)
//==================================================================================
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  int returnValue=0;
  *rcData           =*cgf.rcData;                                 // deep copy
  gridCollectionData=cgf.gridCollectionData;
  gridCollection    =cgf.gridCollection;
  operators         =cgf.operators;

  // ** wdh 960104
  Range *R = rcData->R;
  Range Ru[maximumNumberOfIndicies];
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;
  int numberOfComponents=cgf.getNumberOfComponents();
  for( i=0; i<numberOfComponents; i++ )
    if( cgf.positionOfComponent(i)<maximumNumberOfIndicies )
      Ru[cgf.positionOfComponent(i)]=R[cgf.positionOfComponent(i)];

  if( positionOfFaceCentering()>=0 )
    Ru[positionOfFaceCentering()]=faceRange;

  returnValue&=updateToMatchGrid(Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );

  // now make sure that the MappedGridFunctions are the same
  for( int grid=0; grid<gridCollectionData->numberOfGrids; grid++ )
    returnValue&=mappedGridFunctionList[grid].updateToMatchGridFunction(cgf[grid]);

  return (updateReturnValue &)returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchGridFunction(const GridCollectionFunction & cgf, 
			  const Range & R0, 
			  const Range & R1, /* =nullRange */
			  const Range & R2, /* =nullRange */
			  const Range & R3, /* =nullRange */
			  const Range & R4, /* =nullRange */
			  const Range & R5, /* =nullRange */
			  const Range & R6, /* =nullRange */
			  const Range & R7  /* =nullRange */ )
//==================================================================================
// /Description:
// Update this grid function to match another grid function
//   (this is like using the = operator but it avoids copying the array data)
//
// /cgf (input): match to this grid function.
// /R0, R1, ... (input): optional ranges to change the dimensions.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  int returnValue=0;
  *rcData           =*cgf.rcData;                                 // deep copy
  gridCollectionData=cgf.gridCollectionData;
  gridCollection    =cgf.gridCollection;
  operators         =cgf.operators;

  returnValue&=updateToMatchGrid( R0,R1,R2,R3,R4,R5,R6,R7 );  // this will dimensions arrays properly

  // now make sure that the MappedGridFunctions are the same
  for( int grid=0; grid<gridCollectionData->numberOfGrids; grid++ )
    returnValue&=mappedGridFunctionList[grid].updateToMatchGridFunction(cgf[grid],R0,R1,R2,R3,R4,R5,R6,R7);

  return (updateReturnValue &)returnValue;
}



//\begin{>>GridCollectionFunctionInclude.tex}{updateToMatchnumberOfGrids}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchNumberOfGrids(GridCollection & gc )
//=====================================================================================================
// /Purpose: Update the GridCollectionFunction so that it has the correct
//    number of components. The components are not dimensioned correctly. 
//
//\end{GridCollectionFunctionInclude.tex} 
//=====================================================================================================
{
  gridCollection=&gc;
  gridCollectionData=gc.rcData;

  int returnValue=0;
  if( numberOfGrids() <= 0 )
    return (updateReturnValue &)returnValue;
  
  int length=mappedGridFunctionList.getLength();
  if( length < numberOfGrids() )
  {
    for( int grid=length; grid< numberOfGrids(); grid++ )
    {
      returnValue&=2;
      MappedGridFunction mgf;  // add an empty gridFunction
      mappedGridFunctionList.addElement( mgf );
    }
  }
  else if( length > numberOfGrids() )
  {
    for( int grid=length-1; grid > numberOfGrids(); grid-- )
      mappedGridFunctionList.deleteElement(grid);
  }
  return (updateReturnValue &)returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{updateToMatchComponentGrids}
GridCollectionFunction::updateReturnValue GridCollectionFunction::
updateToMatchComponentGrids()
//=====================================================================================================
// /Purpose: Update the grid collection to match the component grids
//
//\end{GridCollectionFunctionInclude.tex} 
//=====================================================================================================
{
  int returnValue=0;
  // assign the component info from one of the mapped grids
  // they should all be the same
  if( numberOfGrids()>=0 )
  {
    int grid=0;
    for( int i=0; i<maximumNumberOfIndicies; i++ )
    {
      rcData->positionOfComponent[i]=(*this)[grid].positionOfComponent(i);
      rcData->positionOfCoordinate[i]=(*this)[grid].positionOfCoordinate(i);
    }

    rcData->positionOfFaceCentering=(*this)[grid].positionOfFaceCentering();
    rcData->faceCentering=(*this)[grid].faceCentering();
    Range *R = rcData->R;
    (*this)[grid].getRanges( R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7] );  // do this so component Ranges are correct
    for( int axis=0; axis<3; axis++ )                    
      R[positionOfCoordinate(axis)]=nullRange;     // keep coordinate ranges null
  }
  return (updateReturnValue &)returnValue;
}




//=======================================================================================
// Make sure the the faceCentering of the grid collection is consistent with the
// face centering of the MappedGridFunctions
//=======================================================================================
int GridCollectionFunction::
updateFaceCentering()
{
  // first check that all mappedGridFunctions have the same faceCentering as
  // the grid collection...
  int returnValue=0;
  bool consistent=TRUE;
  int grid;
  for( grid=0; grid<numberOfGrids(); grid++ )
  {
    if( (int)mappedGridFunctionList[grid].getFaceCentering()!= faceCentering()  ||
        mappedGridFunctionList[grid].positionOfFaceCentering() != positionOfFaceCentering() )
    {
      consistent=FALSE;
      break;
    }
  }
  // ... if not, check to see if all mappedGridFunctions have the same faceCentering
  // in which case we set the gridCollectionFunction to equal this value
  if( !consistent )
  {
    consistent=TRUE;
    grid=0;
    const int fc = mappedGridFunctionList[grid].getFaceCentering();
    int pfc = mappedGridFunctionList[grid].positionOfFaceCentering();
    for( int grid=1; grid<numberOfGrids(); grid++ )
    {
      if( mappedGridFunctionList[grid].getFaceCentering()!=fc  ||
	 mappedGridFunctionList[grid].positionOfFaceCentering() != pfc )
	consistent=FALSE;
    }
    if( consistent )
    {
      setFaceCentering(fc);
      setPositionOfFaceCentering(pfc);
    }
    else  // mappedGridFunctions are not consistent, set gridCollectionFunction values:
    {
      rcData->faceCentering=GridFunctionParameters::none;
      rcData->positionOfFaceCentering=-1;
      returnValue=0;
    }
  }
  return returnValue;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{sizeOf}}
real GridCollectionFunction::
sizeOf(FILE *file /* = NULL */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this object; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{GridCollectionFunctionInclude.tex}
//==========================================================================
{
  real size=sizeof(*this);
  if( numberOfGrids()>0 )
  {
    for( int grid=0; grid<mappedGridFunctionList.getLength(); grid++ )
    {
      size+=(*this)[grid].sizeOf()-sizeof((*this)[grid]);
    }
  }
  return size;
}

//\begin{>>GridCollectionFunctionInclude.tex}{\subsubsection{fixupUnusedPoints}}
int GridCollectionFunction::
fixupUnusedPoints(const RealArray & value /* =Overture::nullRealArray() */, 
                  int numberOfGhostlines /* =1 */ )
//==================================================================================
// /Description:
//    Assign values to points on a grid function that correspond to unused points (mask==0).
// By default all unused points are set to zero. Use the value array to set unused points to 
// particular values.
// 
// /value (input) : if supplied, assign value(n) to unused points of component n and do not change
//    any components not found in value. If not supplied set all unused points to zero.
// /numberOfGhostLines (input) : Indicate how many ghost lines are used in the computation. Other ghost line
//    values will all be set to zero. 
//
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
{
  for( int grid=0; grid<numberOfGrids(); grid++ )
    (*this)[grid].fixupUnusedPoints(value,numberOfGhostlines);

  return 0;
}

// int GridCollectionFunction::
// zeroUnusedPoints(GridCollectionFunction & coeff, 
// 		 double value /* = 0 */,
// 		 const Index & component0 /* =nullIndex */,
// 		 const Index & component1 /* =nullIndex */,
// 		 const Index & component2 /* =nullIndex */,
// 		 const Index & component3 /* =nullIndex */,
// 		 const Index & component4 /* =nullIndex */ )
// //==================================================================================
// // /Description:
// //   Zero out points on a grid function that correspond to unused points as defined
// // by a 'coefficient' matrix grid function.
// // 
// // /coeff (input) : a coefficient matrix that defines a sparse matrix.
// // /value (input) : assign unused points this value.
// // /component0,... (input): zero out these components of the grid function.
// //\end{GridCollectionFunctionInclude.tex} 
// //==================================================================================
// {
//   assert( coeff.getIsACoefficientMatrix() );
//   GridCollectionFunction & u = *this;
//   for( int grid=0; grid<u.numberOfComponentGrids(); grid++ )
//   {
//     u[grid].zeroUnusedPoints(coeff[grid],value,component0,component1,component2,component3,component4 );
//   }
//   return 0;
// }





GridCollectionFunction & 
FABS( const GridCollectionFunction & cgf ) 
{
  GridCollectionFunction *cgfn;
  if( cgf.temporary )
    cgfn=(GridCollectionFunction*)&cgf;  // cgfn points to the temporary (cast away const)
  else
  {
    cgfn= new GridCollectionFunction();
    (*cgfn).temporary=TRUE;
    (*cgfn).updateToMatchGridFunction(cgf);
  }
  for( int grid=0; grid< cgf.numberOfGrids(); grid++ )
    (*cgfn)[grid]=FABS(cgf[grid]);

  return *cgfn;
}

#define TYPE_COLLECTION_FUNCTION 
#define COLLECTION_FUNCTION GridCollectionFunction
#undef INT_COLLECTION_FUNCTION
#define INT_COLLECTION_FUNCTION intGridCollectionFunction
#define COLLECTION GridCollection
#define QUOTES_COLLECTION_FUNCTION "GridCollectionFunction"
#define INTEGRAL_TYPE double
#include "derivativeDefinitions.C"
#undef COLLECTION_FUNCTION
#undef INT_COLLECTION_FUNCTION
#undef COLLECTION
#undef QUOTES_COLLECTION_FUNCTION
#undef INTEGRAL_TYPE 

/* ----
GridCollectionFunction & GridCollectionFunction::
operator ()( const Index & N )
{
  GridCollectionFunction result;  // ***
  result=*this;                                 // ****** fix this ******
  GridCollection & cg = *(this->gridCollection);
  
  for( int grid=0; grid<numberOfGrids(); grid++ )
  {
    #ifndef MGCG_OR_CG
      getIndex(cg[grid].indexRange(),I1,I2,I3);
      result[grid](I1,I2,I3,N)=mappedGridFunctionList[grid].x(I1,I2,I3,N);
    #else
      result[grid](I1,I2,I3,N)=mappedGridFunctionList[grid].x(N);
    #endif
  }
  return result;
}
----------*/



/* ----- re-think this for grid collections: 
void GridCollectionFunction::
getDerivatives( const Index & N )
{
  for( int grid=0; grid<numberOfGrids(); grid++ )
    mappedGridFunctionList[grid].getDerivatives( I1,I2,I3,N );
}
------ */


