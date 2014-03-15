//========================================================================================================
//    This file defines the functions for the mappedGridFunction Class
//
//  The perl script gf.p is used to convert this file into the files
//      <type>floatMappedGridFunction.C  where <type> is one of float, float or int
//========================================================================================================

#include "floatMappedGridFunction.h"
#include "MappedGrid.h"
#include "MappedGridOperators.h"  
#include "SparseRep.h"
#include "GenericDataBase.h"
#include "GridFunctionParameters.h"
#include "UnstructuredMapping.h"
#include "ParallelUtility.h"
#include "App.h"

// ----- Define member functions for the reference-counted data class ------
floatMappedGridFunctionRCData::
floatMappedGridFunctionRCData()
{
//  positionOfComponent.redim(maximumNumberOfIndicies);
//  positionOfCoordinate.redim(maximumNumberOfIndicies);
  numberOfComponents=0;
  positionOfFaceCentering=-1;
  faceCentering=GridFunctionParameters::none;
  numberOfDimensions     =0;
  isACoefficientMatrix   =FALSE;
  stencilType            =floatMappedGridFunction::generalStencil;
//  stencilOffset.redim(1);
  stencilWidth           =3;
  updateToMatchGridOption=floatMappedGridFunction::updateSize | floatMappedGridFunction::updateCoefficientMatrix;
  numberOfNames          =0;
  name                   =NULL;
}
floatMappedGridFunctionRCData::
~floatMappedGridFunctionRCData()
{
  delete [] name;
}
floatMappedGridFunctionRCData & floatMappedGridFunctionRCData::
operator=( const floatMappedGridFunctionRCData & rcData )  // deep copy
{
  if( this == &rcData )
    return *this;
  
//  positionOfComponent    =rcData.positionOfComponent;  // Make sure this breaks any references ???
//  positionOfCoordinate   =rcData.positionOfCoordinate;
  numberOfDimensions     =rcData.numberOfDimensions;
  numberOfComponents     =rcData.numberOfComponents;
  positionOfFaceCentering=rcData.positionOfFaceCentering;
  faceCentering          =rcData.faceCentering;
  if( !isACoefficientMatrix )
    isACoefficientMatrix   =rcData.isACoefficientMatrix;
  stencilType            =rcData.stencilType;
  stencilWidth           =rcData.stencilWidth;
  // stencilOffset          =rcData.stencilOffset;
  updateToMatchGridOption=rcData.updateToMatchGridOption;
  
  int i;
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
    R[i]=rcData.R[i];
  for( i=0; i<numberOfIndicies; i++ )
    Ra[i]=rcData.Ra[i];
  for( i=0; i<3; i++ )
    Rc[i]=rcData.Rc[i];
  for( i=0; i<maximumNumberOfIndicies; i++ )  
  {
    positionOfComponent[i]  =rcData.positionOfComponent[i];  
    positionOfCoordinate[i] =rcData.positionOfCoordinate[i];
  }
  
  numberOfNames          =rcData.numberOfNames;
  delete [] name;
  if( numberOfNames>0 )
  {
    name = ::new aString[numberOfNames];
    for( i=0; i<numberOfNames; i++ )
      name[i]=rcData.name[i];
  }
  else  
    name=NULL;
  return *this;
}


// ================ member functions for floatMappedGridFunction ========

//\begin{>MappedGridFunctionInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
//  
// /edgeGridFunctionValues: 
//    Use these values to create special Range objects to define grid functions on boundaries.
//  {\footnotesize
//  \begin{verbatim} 
//    enum edgeGridFunctionValues     // these enums are used to declare grid functions defined on faces or edges
//    {
//      startingGridIndex   =-(INT_MAX/2),              // choose a big negative number assuming that
//      biggerNegativeNumber=startingGridIndex/2,       // no grid will ever have dimensions in this range
//      endingGridIndex     =biggerNegativeNumber/2,
//      bigNegativeNumber   =endingGridIndex/2
//    };
//  \end{verbatim} 
//  }
//  
// /stencilTypes:
//    Here are somes standard stencil types for coefficient matrices.
//  {\footnotesize
//  \begin{verbatim} 
//    enum stencilTypes               // if the grid function holds a coefficient matrix
//    {                               // these are the types of stencil that it may contain
//      standardStencil,              // 3x3 int 2D or 3x3x3 in 3D (if 2nd order accuracy)
//      starStencil,                  // 5 point star in 2D or 7pt star in 3D (if 2nd order accuracy)
//      generalStencil
//    };
//  \end{verbatim} 
//  }
//  
// /updateReturnValue:
//    The value returned from the {\ff updateToMatchGrid} and {\ff updateToMatchGridFunction} is
//    a mask formed by a bitwise {\ff or} of the following values:  
//  {\footnotesize
//  \begin{verbatim} 
//    enum updateReturnValue  // the return value from updateToMatchGrid is a mask of the following values
//    {
//      updateNoChange          = 0, // no changes made
//      updateReshaped          = 1, // grid function was reshaped
//      updateResized           = 2, // grid function was resized
//      updateComponentsChanged = 4  // component dimensions may have changed (but grid was not resized or reshaped)
//    };
//  \end{verbatim} 
//  }
//  
//\end{MappedGridFunctionInclude.tex}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Constructors}} 
floatMappedGridFunction::
floatMappedGridFunction ()
//-----------------------------------------------------------------------------------------
// /Description:
//   Default constructor
// /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  initialize();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Constructors}} 
floatMappedGridFunction::
floatMappedGridFunction(MappedGrid & grid0)
//-----------------------------------------------------------------------------------------
// /Description:
//   Create a grid function and associate with a MappedGrid.
//   The grid function will be a "scalar" as in the declaration:
//   \begin{verbatim}  
//     Range all;
//     MappedGrid mg(...);
//     floatMappedGridFunction u(mg,all,all,all);
//   \end{verbatim}  
// /grid0 (input): grid to associate this grid function with.
// /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  initialize();
  mappedGrid = &grid0;
  grid=grid0.rcData;

  Range Ru[maximumNumberOfIndicies];  // ****
  for( int i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;

  updateRanges( Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );
  updateToMatchGrid();
}

  
//---------------------------------------------------------------------------------------------
// This constructor is here for use by the MappedGrid Class
//---------------------------------------------------------------------------------------------
floatMappedGridFunction::
floatMappedGridFunction(MappedGridData & grid0)
{
  initialize();
  grid=&grid0;

  Range Ru[maximumNumberOfIndicies];   // ****
  for( int i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;
  if( positionOfFaceCentering()>=0 )
    Ru[positionOfFaceCentering()]=faceRange;

  updateRanges( Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );

  updateToMatchGrid();
}

//------------------------------------------------------------------------------
//  Count the number of Ranges with length > 0
//------------------------------------------------------------------------------
static int 
countNumberOfNonNullRanges(const Range & R0=nullRange,   
			   const Range & R1=nullRange,  
			   const Range & R2=nullRange,  
			   const Range & R3=nullRange,  
			   const Range & R4=nullRange,  
			   const Range & R5=nullRange,  
			   const Range & R6=nullRange,  
			   const Range & R7=nullRange ) 
{
  int count=0;
  Range Ru[] = { R0,R1,R2,R3,R4,R5,R6,R7 };  
  for( int i=0; i<floatMappedGridFunction::maximumNumberOfIndicies; i++ )
    if( Ru[i].length() > 0 )
      count++;
  return count;
}


//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::
floatMappedGridFunction(MappedGrid & grid0, 
		   const Range & R0,   
		   const Range & R1,   /* =nullRange */
		   const Range & R2,   /* =nullRange */
		   const Range & R3,   /* =nullRange */
		   const Range & R4,   /* =nullRange */
		   const Range & R5,   /* =nullRange */
		   const Range & R6,   /* =nullRange */
		   const Range & R7    /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description:
//   This constructor takes ranges, the first 3 "nullRange" values are taken to be the
//   coordinate directions in the grid function.
//   Each grid function is dimensioned according to the dimensions found
//   with the {\ff MappedGrid}, using the {\ff dimension} values.
//   Grid functions can have up to 8 dimensions, the index positions
//   not used by the coordinate dimensions can be used to store
//   different components. For example, a {\em vector} grid
//   functions would use 1 index position for components while a {\em matrix} grid functions would
//   use two index positions for components. 
//
// /grid0 (input): MappedGrid to associate this grid function with.
// /R0, R1, R2, ... (input): Ranges to determine the shape and size of the grid function.
//       An int can also be used instead of a Range.
// 
// /Examples:
//   Here are some examples
//   {\footnotesize
//     \begin{verbatim}
//    
//        //  R1 = range of first dimension of the grid array
//        //  R2 = range of second dimension of the grid array
//        //  R3 = range of third dimension of the grid array 
//
//        MappedGrid mg(...);
//
//        Range R1(mg.dimension()(Start,axis1),mg.dimension()(End,axis1));
//        Range R2(mg.dimension()(Start,axis2),mg.dimension()(End,axis2));
//        Range R3(mg.dimension()(Start,axis3),mg.dimension()(End,axis3));
//
//        Range all;    // null Range is used to specify where the coordinates are
//
//        floatMappedGridFunction u(mg);                           //  --> u(R1,R2,R3);
//
//        floatMappedGridFunction u(mg,all,all,all,1);             //  --> u(R1,R2,R3,0:1);
//        floatMappedGridFunction u(mg,all,all,Range(1,1));        //  --> u(R1,R2,1:1,R3);
//
//        floatMappedGridFunction u(mg,2,all);                     //  --> u(0:2,R1,R2,R3);
//        floatMappedGridFunction u(mg,Range(0,2),all,all,all);    //  --> u(0:2,R1,R2,R3);
//        floatMappedGridFunction u(mg,all,Range(3,3),all,all);    //  --> u(R1,3:3,R2,R3);
//    
//    \end{verbatim}
//    }   
//    
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  initialize();
  mappedGrid = &grid0;
  grid=grid0.rcData;
  updateRanges( R0,R1,R2,R3,R4,R5,R6,R7 );
  privateUpdateToMatchGrid();
} 

floatMappedGridFunction::
floatMappedGridFunction(MappedGrid & grid0, 
		   const int   & i0,   
		   const Range & R1,   /* =nullRange */
		   const Range & R2,   /* =nullRange */
		   const Range & R3,   /* =nullRange */
		   const Range & R4,   /* =nullRange */
		   const Range & R5,   /* =nullRange */
		   const Range & R6,   /* =nullRange */
		   const Range & R7    /* =nullRange */ )
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
{
  initialize();
  mappedGrid = &grid0;
  grid=grid0.rcData;
  updateRanges( Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
  privateUpdateToMatchGrid();
} 

//-----------------------------------------------------------------------------------------
// This constructor is needed by the MappedGrid Class
//-----------------------------------------------------------------------------------------
floatMappedGridFunction::
floatMappedGridFunction(MappedGridData & gridData, 
		   const Range & R0, 
		   const Range & R1,
		   const Range & R2,
		   const Range & R3,
		   const Range & R4,
		   const Range & R5,
		   const Range & R6,
		   const Range & R7)
{
  initialize();
  grid=&gridData;
  updateRanges( R0,R1,R2,R3,R4,R5,R6,R7 );
  privateUpdateToMatchGrid();
} 
floatMappedGridFunction::
floatMappedGridFunction(MappedGridData & gridData, 
		   const int   & i0, 
		   const Range & R1,
		   const Range & R2,
		   const Range & R3,
		   const Range & R4,
		   const Range & R5,
		   const Range & R6,
		   const Range & R7)
{
  initialize();
  grid=&gridData;
  updateRanges( Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
  privateUpdateToMatchGrid();
} 


//\begin{>>MappedGridFunctionInclude.tex}{} 
floatMappedGridFunction::
floatMappedGridFunction(MappedGrid & grid0, 
		   const GridFunctionParameters::GridFunctionType & type, 
		   const Range & component0,  /* =nullRange */
		   const Range & component1,  /* =nullRange */
		   const Range & component2,  /* =nullRange */
		   const Range & component3,  /* =nullRange */
		   const Range & component4   /* =nullRange */ )
//-----------------------------------------------------------------------------------------
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
// /grid0 (input): Use this MappedGrid
// /type (input): Make this type of grid function.
// /component0, component1,... (input): supply a Range for each component. 
// /Examples:
//   Here are some examples:
//  {\footnotesize  
//  \begin{verbatim} 
//    MappedGrid mg(...);
//    realMappedGridFunction u(mg,GridFunctionParameters::vertexCentered,2);   // u(mg,all,all,all,2);
//    realMappedGridFunction u(mg,GridFunctionParameters::cellCentered,2,3);   // u(mg,all,all,all,2,3);
//    realMappedGridFunction u(mg,GridFunctionParameters::faceCenteredAll,2);  // u(mg,all,all,all,2,faceRange);
//    realMappedGridFunction u(mg,GridFunctionParameters::faceCenteredAll,3,2);// u(mg,all,all,all,3,2,faceRange);
//  \end{verbatim}  
//  }  
// /Remarks:
//  A face centered grid function along axis=axis0 is vertex centered along axis0 and cell centered along the
//  other axes.
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  initialize();
  mappedGrid = &grid0;
  grid=grid0.rcData;
  const Range &fr =  grid->gridType==MappedGrid::structuredGrid ? faceRange : nullRange; // kkc 040325
  switch (type)
  {
  case GridFunctionParameters::defaultCentering:
  case GridFunctionParameters::general:
  case GridFunctionParameters::vertexCentered:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::cellCentered:
  case GridFunctionParameters::faceCentered: // unstructured grid
  case GridFunctionParameters::edgeCentered: // unstructured grid
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4,type);
    break;
  case GridFunctionParameters::faceCenteredAll:
    switch (countNumberOfNonNullRanges(component0,component1,component2,component3,component4))
    {
    case 0:
      updateRanges( nullRange,nullRange,nullRange,fr,nullRange,nullRange,nullRange,nullRange,type); break;
    case 1:
      updateRanges( nullRange,nullRange,nullRange,component0,fr,nullRange,nullRange,nullRange,type); break;
    case 2:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,fr,nullRange,nullRange,type); break;
    case 3:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,fr,nullRange,type); break;
    case 4:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,fr,type); break;
    default:
      cout << "floatMappedGridFunction::constructor:ERROR too many components, only 4 allowed \n";
      Overture::abort("floatMappedGridFunction::constructor:ERROR too many components, only 4 allowed" );
    }
    break;
  case GridFunctionParameters::faceCenteredAxis1:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::faceCenteredAxis2:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::faceCenteredAxis3:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  default:
    cout << "floatMappedGridFunction::(constructor):ERROR unknown GridFunctionType!! This error should not occur\n";
    Overture::abort("error");
  }
  privateUpdateToMatchGrid();
  // now set the cell/face centering because the isCellCentered Array is now defined
  switch (type)
  {
  case GridFunctionParameters::defaultCentering:
    // defaultCentering == cellCentered if the grid is cell centered
    if( grid0.isAllCellCentered() )
      setIsCellCentered(TRUE);
    break;
  case GridFunctionParameters::cellCentered:
    setIsCellCentered(TRUE);
    break;
  case GridFunctionParameters::vertexCentered:
    setIsCellCentered(FALSE);
    break;
  case GridFunctionParameters::faceCenteredAll:
    // do nothing here. already done in privateUpdateToMatchGrid
    break;
  case GridFunctionParameters::faceCenteredAxis1:
    setFaceCentering(axis1);
    break;
  case GridFunctionParameters::faceCenteredAxis2:
    setFaceCentering(axis2);
    break;
  case GridFunctionParameters::faceCenteredAxis3:
    setFaceCentering(axis3);
    break;
  case GridFunctionParameters::faceCentered:
    rcData->faceCentering=GridFunctionParameters::faceCenteredUnstructured;
    break;
  case GridFunctionParameters::edgeCentered:
    rcData->faceCentering=GridFunctionParameters::edgeCenteredUnstructured;
    break;
  default:
    cout << "floatMappedGridFunction::(constructor):ERROR unknown GridFunctionType!! This error should not occur\n";
    break;
  }
}


//\begin{>>MappedGridFunctionInclude.tex}{} 
floatMappedGridFunction::
floatMappedGridFunction(const floatMappedGridFunction & cgf, 
                   const CopyType copyType /* =DEEP */  ) 
   : floatDistributedArray(cgf, copyType==DEEP ? DEEPCOPY : SHALLOWCOPY)
//-----------------------------------------------------------------------------------------
// /Description:
//   Copy constructor, deep copy by default
// /Notes: 
//     This routine was changes 011103 to call the underlying A++ copy constructor. This
// was necessary for functions that return a realMGF by value. On some compilers, like the Sun CC 4.2
// These return by value temporaries would not be deleted immediately but rather stay around until
// the end of scope -- this could result in a low of extra storage being required. See the 
// test code otherStuff/memoryUsage.C for examples.
// 
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
//   printf("Inside floatMappedGridFunction copy constructor. cgf.name=%s\n",(const char*)cgf.getName());
//   printf("   -->isTemporary()=%i\n",isTemporary());

  initialize();

  switch( copyType )
  {
  case DEEP:
    updateToMatchGridFunction(cgf);
    // initialize();
    //    (*this)=cgf;    // semi-deep copy

    // need to do this as well for a deep copy
    rcData->isACoefficientMatrix=cgf.rcData->isACoefficientMatrix;  
    if( isACoefficientMatrix() )
    { // make a copy of the sparse matrix representation
      setIsACoefficientMatrix(TRUE);
      *sparse= *(cgf.sparse);
    }
    break;
  case SHALLOW:
//    initialize();
    reference( cgf ); 
    break;
  case NOCOPY:
//    initialize();
    break;
  }
}

/* --------------------------------------------------
floatMappedGridFunction::
floatMappedGridFunction(const floatMappedGridFunction & cgf, 
                   const CopyType copyType ) 
   : floatDistributedArray(cgf, copyType==DEEP ? DEEPCOPY : SHALLOWCOPY)
//-----------------------------------------------------------------------------------------
// /Description:
//   Copy constructor, deep copy by default
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  printf("Inside floatMappedGridFunction copy constructor. cgf.name=%s\n",(const char*)cgf.getName());
  printf("   -->isTemporary()=%i\n",isTemporary());
  switch( copyType )
  {
  case DEEP:
    initialize();
    (*this)=cgf;    // semi-deep copy
    // need to do this as well for a deep copy
    rcData->isACoefficientMatrix=cgf.rcData->isACoefficientMatrix;  
    if( isACoefficientMatrix() )
    { // make a copy of the sparse matrix representation
      setIsACoefficientMatrix(TRUE);
      *sparse= *(cgf.sparse);
    }
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
------------------------- */

//-----------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------
floatMappedGridFunction::
~floatMappedGridFunction ()
{
  if( rcData->decrementReferenceCount() == 0 )
    ::delete rcData;     // don't call A++ delete!

  if( sparse && sparse->decrementReferenceCount()==0 )
    delete sparse;
}

//-----------------------------------------------------------------------------------------
// Assign initial values to variables for a null grid function
//   This function should only be called once for a grid function (by constructors)
//   otherwise there will me a memory leak.
//-----------------------------------------------------------------------------------------
void floatMappedGridFunction::
initialize()  
{
  rcData = ::new floatMappedGridFunctionRCData;  //    new : don't call A++ new
  rcData->incrementReferenceCount();

//  positionOfComponent.reference(rcData->positionOfComponent);
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
  {
    rcData->positionOfComponent[i]=maximumNumberOfIndicies;                   // default value
    rcData->positionOfCoordinate[i]=maximumNumberOfIndicies;                   // default values
  }
  
//  positionOfCoordinate.reference(rcData->positionOfCoordinate);
//  positionOfCoordinate=maximumNumberOfIndicies;                   // default values
  for( int axis=0; axis<3; axis++ )
    rcData->positionOfCoordinate[axis]=axis;

  // stencilOffset.reference(rcData->stencilOffset);

  rcData->R[0]=nullRange;
  rcData->R[1]=nullRange;
  rcData->R[2]=nullRange;
  for( i=3; i<maximumNumberOfIndicies+1; i++ )
    rcData->R[i]=Range(0,0);
  for( i=0; i<numberOfIndicies; i++ )
    rcData->Ra[i]=Range(0,0);

  rcData->Rc[0]=nullRange;  // by default the coordinate Ranges span the entire grid
  rcData->Rc[1]=nullRange;  // these get changed if we make a grid function that lives
  rcData->Rc[2]=nullRange;  // on a boundary

  grid = NULL;
  mappedGrid=NULL;
  operators=NULL;
  sparse=NULL;
}




                                 
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{applyBoundaryConditions}}
void floatMappedGridFunction::
applyBoundaryConditions( const real & time /* = 0. */ )
//==================================================================================
// /Description:
//   Apply the boundary conditions to this grid function. This routine just calls the 
//   function of the same name in the MappedGridOperators.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryConditions( *this, time );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryConditions( *this, time );
  #endif
  
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{assignBoundaryConditionCoefficients}}
void floatMappedGridFunction::
assignBoundaryConditionCoefficients( const real & time /* = 0. */ )
//==================================================================================
// /Description:
//   Fill in the coefficients of the boundary conditions into this grid function. 
//   This routine just calls the function of the same name in the MappedGridOperators.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->assignBoundaryConditionCoefficients( *this, time );
  #else
    //  boundaryConditionError(); 
     operators->assignBoundaryConditionCoefficients( *this, time );
  #endif
  
}



  // new BC interface:
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{assignBoundaryCondition}}
void floatMappedGridFunction::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,  /* = BCTypes::dirichlet */
		       const int & bc,                   /* = BCTypes::allBoundaries */
		       const real & forcing,             /* =0. */
		       const real & time,                /* =0. */
		       const BoundaryConditionParameters & 
		       bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid_ /* =0  */ )
//==================================================================================
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryCondition( *this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #endif
  
}

//\begin{>>MappedGridFunctionInclude.tex}{}
void floatMappedGridFunction::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const RealArray & forcing,
		       const real & time,  /* =0. */
		       const BoundaryConditionParameters & 
                                bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid_ /* =0  */  )
//==================================================================================
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #endif
}

//\begin{>>MappedGridFunctionInclude.tex}{}
void floatMappedGridFunction::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const RealArray & forcing,
		       RealArray *forcinga[2][3],
		       const real & time,  /* =0. */
		       const BoundaryConditionParameters & 
                                bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid_ /* =0  */  )
//==================================================================================
//  If forcinga[side][axis] !=NULL then use this array, otherwise use forcing.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,forcinga,time,bcParameters,grid_ );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,forcinga,time,bcParameters,grid_ );
  #endif
}


//\begin{>>MappedGridFunctionInclude.tex}{}
void floatMappedGridFunction::
applyBoundaryCondition(const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const floatMappedGridFunction & forcing,
		       const real & time, /* =0. */
		       const BoundaryConditionParameters & 
                                bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid_ /* =0  */ )
//==================================================================================
// /Description:
//   Apply a boundary condition to the grid function. This function just calls the
//  corresponding function in MappedGridOperators. See the operator documentation
//  for further details.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
  #endif
}

#ifdef USE_PPP
// void floatMappedGridFunction::
// applyBoundaryCondition(const Index & Components,
// 		       const BCTypes::BCNames & bcType,
// 		       const int & bc,
// 		       const RealDistributedArray & forcing,
// 		       const real & time,  /* =0. */
// 		       const BoundaryConditionParameters & 
//                                 bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
// 		       const int & grid_ /* =0  */  )
// //==================================================================================
// //==================================================================================
// {
//   if( operators==NULL )
//   {
//     cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator\n";
//     Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryCondition without defining an operator");
//   }
//   // boundaryConditionError(); 
//   #ifdef OV_USE_DOUBLE
//       boundaryConditionError(); 
//     // operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
//   #else
//     //  boundaryConditionError(); 
//      operators->applyBoundaryCondition(*this, Components,bcType,bc,forcing,time,bcParameters,grid_ );
//   #endif
// }
#endif

// fix corners and periodic update:
void floatMappedGridFunction::
finishBoundaryConditions(const BoundaryConditionParameters & bcParameters,
     const Range & C0 /* =nullRange */)
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to finishBoundaryConditions without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to finishBoundaryConditions without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->finishBoundaryConditions(*this,bcParameters,C0);
  #else
    //  boundaryConditionError(); 
     operators->finishBoundaryConditions(*this,bcParameters,C0);
  #endif
}


void floatMappedGridFunction::
applyBoundaryConditionCoefficients(const Index & Equation,
                                   const Index & Components,
				   const BCTypes::BCNames & bcType,
				   const int & bc,
				   const BoundaryConditionParameters & bcParameters,
		                   const int & grid_ /* =0  */  )
{
  if( operators==NULL )
  {
    cout << "floatMappedGridFunction: ERROR: trying to applyBoundaryConditionCoefficients without defining an operator\n";
    Overture::abort("floatMappedGridFunction: ERROR: trying to applyBoundaryConditionCoefficients without defining an operator");
  }
  // boundaryConditionError(); 
  #ifdef OV_USE_DOUBLE
      boundaryConditionError(); 
    // operators->applyBoundaryConditionCoefficients(*this, Equation,Components,bcType,bc,bcParameters,grid_ );
  #else
    //  boundaryConditionError(); 
     operators->applyBoundaryConditionCoefficients(*this, Equation,Components,bcType,bc,bcParameters,grid_ );
  #endif
}

// Use this function when you have too many arguments to a grid function:
//   to achieve this: u(i0,i1,i2,i3,i4,i5,i6,i7), do this -> u(i0,i1,i2,u.arg3(i3,i4,i5,i6,i7))
int floatMappedGridFunction::
arg3(int i3, int i4, int i5, int i6, int i7) const
{
  int j5 = i5==defaultValue ? rcData->R[5].getBase() : i5;
  int j6 = i6==defaultValue ? rcData->R[6].getBase() : i6;
  int j7 = i7==defaultValue ? rcData->R[7].getBase() : i7;
  return i3+rcData->R[3].length()*(i4-rcData->R[4].getBase()
           +rcData->R[4].length()*(j5-rcData->R[5].getBase()
           +rcData->R[5].length()*(j6-rcData->R[6].getBase()
	   +rcData->R[6].length()*(j7))));
}
 



//====================================================================================
// supply an erro message for the BC routines
//====================================================================================
void floatMappedGridFunction::
boundaryConditionError() const
{
  cout << "floatMappedGridFunction: ERROR: do not know how to apply boundary conditions to this grid function\n";
}



//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{breakReference}}
void floatMappedGridFunction::
breakReference()
//-----------------------------------------------------------------------------------------
// /Description:
//   This member function will cause the grid function
//   to no longer be referenced. The grid function acquires its own copy
//   of the data.
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  // If there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    floatMappedGridFunction cgf = *this;  // makes a deep copy
    reference(cgf);   // make a reference to this new copy
  }
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{dataCopy}}
int floatMappedGridFunction::
dataCopy( const floatMappedGridFunction & mgf )
//==================================================================================
// /Description:
//   copy the array data only
//
// /mgf (input): set the array data equal to the data in this grid function.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( this == &mgf )
    return 0;
  #ifndef USE_PPP
    floatDistributedArray::operator= (mgf);
  #else
    floatSerialArray u,v;
    ::getLocalArrayWithGhostBoundaries(*this,u);
    ::getLocalArrayWithGhostBoundaries(mgf,v);
    // u=v;
    // In parallel setting u=v may not work if u or v is a link since u or v may be the full dimensions
    // *wdh* 100327
    Range R[6]={u.dimension(0),u.dimension(1),u.dimension(2),u.dimension(3),u.dimension(4),u.dimension(5)};
    u(R[0],R[1],R[2],R[3],R[4],R[5])=v(R[0],R[1],R[2],R[3],R[4],R[5]);
  #endif
  return 0;
}



//=====================================================================
// Allocate space for the name array
//====================================================================
void floatMappedGridFunction::
dimensionName()
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  // first count the total number of components
  int component,numberOfNames=1;
  for( int i=0; i<maximumNumberOfIndicies; i++ )
  {
    component=positionOfComponent(i);
    if( component < maximumNumberOfIndicies )
      numberOfNames*=R[component].length();
  }
  if( rcData->numberOfNames < numberOfNames+1 )
  {
    aString *newName = ::new aString[numberOfNames+1];
    int i;
    for( i=0; i<rcData->numberOfNames; i++ )
      newName[i]=rcData->name[i];
    for( i=rcData->numberOfNames; i<numberOfNames+1; i++ )
      newName[i]=" ";
    delete [] rcData->name;
    rcData->name=newName;
    rcData->numberOfNames=numberOfNames+1;
  }
}

//static floatMappedGridFunction nullFloatMappedGridFunction;
// extern floatMappedGridFunction nullFloatMappedGridFunction;

/* -----  Here are some comments -----
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Derivatives: x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div,r1,r2,r3,r1r1,r1r2,...}}
floatMappedGridFunction 
derivative(const Index & I1,  // =nullIndex 
	   const Index & I2,  // =nullIndex 
	   const Index & I3,  // =nullIndex 
	   const Index & I4,  // =nullIndex 
	   const Index & I5,  // =nullIndex 
	   const Index & I6,  // =nullIndex 
	   const Index & I7,  // =nullIndex 
	   const Index & I8   // =nullIndex 
         )
//==================================================================================
// /Description:
//   Derivative equals one of x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div,r1,r2,r3,r1r1,r1r2,r1r3,r2r2,r2r3,r3r3.
//   Return the derivative of this grid function. This routine just calls the 
//   function of the same name in the GenericMappedGridOperators (see also setOperators).
// /I1,I2,I3 (input) : optional arguments to specify where the derivatives are evaluated. 
//    In this case the returned grid function will only have values of the
//  derivative computed at this subset of points, other values in the grid function will be
//  zero.
// /I4 (input) : evaluate the derivative for these components, by default all components. 
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by I4 (if I4
// not specified then the result will have the same number of components s {\tt u}). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction 
derivative(const GridFunctionParameters & gfType,
           const Index & I1,  // =nullIndex 
	   const Index & I2,  // =nullIndex 
	   const Index & I3,  // =nullIndex 
	   const Index & I4,  // =nullIndex 
	   const Index & I5,  // =nullIndex 
	   const Index & I6,  // =nullIndex 
	   const Index & I7,  // =nullIndex 
	   const Index & I8   // =nullIndex 
         )
//==================================================================================
// /Description:
//   derivative equals one of x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div.
//   Return the derivative of this grid function. 
//   The argument gfType determines the
//   type of the grid function that is returned. 
//   This routine just calls the 
//   function of the same name in the GenericMappedGridOperators (see also setOperators).
// /gfType (input): The type of the grid function to be returned.
// /I1,I2,I3 (input) : optional arguments to specify where the derivatives are evaluated. 
//    In this case the returned grid function will only have values of the
//  derivative computed at this subset of points, other values in the grid function will be
//  zero.
// /I4 (input) : evaluate the derivative for these components, by default all components. 
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by I4 (if I4
// not specified then the result will have the same number of components s {\tt u}). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
 ----------------- */

/* ------- Here are some comments -----
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Derivative Coefficients: xCoefficient,yCoefficient,...}}
floatMappedGridFunction
Derivative(const Index & I1,  // =nullIndex
	   const Index & I2,  // =nullIndex
	   const Index & I3,  // =nullIndex
	   const Index & I4,  // =nullIndex
	   const Index & I5,  // =nullIndex
	   const Index & I6,  // =nullIndex
	   const Index & I7,  // =nullIndex
	   const Index & I8   // =nullIndex
          )
//==================================================================================
// /Description:
//   Derivative equals one of xCoefficient,yCoefficient,zCoefficient,xxCoefficient,
//    xy\-Coefficient,xz\-Coefficient,yy\-Coefficient,yz\-Coefficient,zz\-Coefficient,
//    laplacianCoefficient,gradCoefficient,divCoefficient.
//   Return the coefficients of the derivative. This routine just calls the 
//   function of the same name in the MappedGridOperators (see also setOperators).
// /I1,I2,I3,... (input) : optional arguments to specify where the derivatives are evaluated. 
//    In this case the returned grid function will only have values of the
//  derivative computed at this subset of points, other values in the grid function will be
//  zero.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction
Derivative(const GridFunctionParameters & gfType,
           const Index & I1,  // =nullIndex
	   const Index & I2,  // =nullIndex
	   const Index & I3,  // =nullIndex
	   const Index & I4,  // =nullIndex
	   const Index & I5,  // =nullIndex
	   const Index & I6,  // =nullIndex
	   const Index & I7,  // =nullIndex
	   const Index & I8   // =nullIndex
          )
//==================================================================================
// /Description:
//   Derivative equals one of xCoefficient,yCoefficient,zCoefficient,xx\-Coefficient,
//    xy\-Coefficient,xz\-Coefficient,yy\-Coefficient,yz\-Coefficient,zzCoefficient,
//    laplacianCoefficient,gradCoefficient,divCoefficient.
//   Return the coefficients of the derivative. This routine just calls the 
//   function of the same name in the MappedGridOperators (see also setOperators).
// /gfType (input): The type of the grid function to be returned.
// /I1,I2,I3,... (input) : optional arguments to specify where the derivatives are evaluated. 
//    In this case the returned grid function will only have values of the
//  derivative computed at this subset of points, other values in the grid function will be
//  zero.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
 ----------------- */



//====================================================================================
// This function is used to print an error message for the derivative routines
//====================================================================================
void floatMappedGridFunction::
derivativeError() const
{
  cout << "floatMappedGridFunction: ERROR: do not know how to differentiate this grid function\n"
       << "Either you are trying to differentiate an intMappedGridFunction or \n"
       << "you are trying to differentiate a float/float when real=float/float \n";
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{destroy}}
int floatMappedGridFunction::
destroy()
//==================================================================================
// /Description:
//   Destroy this grid function. Release all memory, and reset the grid function 
//   properties to the default.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
//  *this=nullFloatMappedGridFunction;

  redim(0);
  if( rcData->decrementReferenceCount() == 0 )
    ::delete rcData;     // don't call A++ delete!

  if( sparse && sparse->decrementReferenceCount()==0 )
    delete sparse;
  initialize();
  
  return 0;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{get}}
int floatMappedGridFunction::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get from a database file. Example:
// \begin{verbatim}
//   HDF_DataBase db;
//   db.mount("myFile.hdf","R");
//   MappedGrid g;
//   realMappedGridFunction u;  
//   initializeMappingList();
//   g.get(db,"my grid");
//   u.updateToMatchGrid(g);   // **NOTE**
//   u.get(db,"u");
// \end{verbatim}
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
// /NOTE: This get function will not set the pointer to the MappedGrid associated
//    with this grid function. You should call updateToMatchGrid(...) to set
//    the grid BEFORE using this function. 
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"floatMappedGridFunction");

  subDir.setMode(GenericDataBase::streamInputMode);

  char buff[40];
  int i;
  subDir.get( rcData->numberOfComponents,"numberOfComponents" );
  subDir.get( rcData->positionOfCoordinate,"positionOfCoordinate",maximumNumberOfIndicies );  
  subDir.get( rcData->positionOfComponent,"positionOfComponent",maximumNumberOfIndicies ); 
  subDir.get( rcData->positionOfFaceCentering,"positionOfFaceCentering" );
  subDir.get( rcData->faceCentering,"faceCentering" ); 
  subDir.get( rcData->numberOfDimensions,"numberOfDimensions" );

  subDir.get( rcData->isACoefficientMatrix,"isACoefficientMatrix" ); 
  subDir.get( rcData->stencilType,"stencilType" ); 
  // subDir.get( rcData->stencilOffset,"stencilOffset" ); 
  subDir.get( rcData->stencilWidth,"stencilWidth" ); 
  int base,bound;
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
  {
    subDir.get( base, sPrintF(buff,"R[%i].base",i) );
    subDir.get( bound,sPrintF(buff,"R[%i].bound",i) );
    rcData->R[i]=Range(base,bound);
  }
  for( i=0; i<numberOfIndicies; i++ )
  {
    subDir.get( base, sPrintF(buff,"Ra[%i].base",i) );
    subDir.get( bound,sPrintF(buff,"Ra[%i].bound",i) );
    rcData->Ra[i]=Range(base,bound);
  }
  for( i=0; i<3; i++ )
  {
    subDir.get( base, sPrintF(buff,"Rc[%i].base",i) );
    subDir.get( bound,sPrintF(buff,"Rc[%i].bound",i) );
    rcData->Rc[i]=Range(base,bound);
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

  subDir.get( isCellCentered,"isCellCentered" ); 
  subDir.getDistributed( *this,"arrayData" );  // get the A++ array

  delete &subDir;
  return TRUE; 
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getClassName}}
aString floatMappedGridFunction::
getClassName() const 
//==================================================================================
// /Description:
//    Return the class name.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{ 
  return className; 
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getSerialArray}}
floatSerialArray & floatMappedGridFunction::
getSerialArray()
//==================================================================================
// /Description:
//    Return the grid function as a serial array. In parallel return the local array
//   with ghost boundaries.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
#ifdef USE_PPP
   return *getLocalArrayWithGhostBoundariesPointer(); 
#else
  return *this;
#endif
}

const floatSerialArray & floatMappedGridFunction::
getSerialArray() const
{
#ifdef USE_PPP
   return *getLocalArrayWithGhostBoundariesPointer(); 
#else
  return *this;
#endif
}



//----------------------------------------------------------------------------------------
//  Here are functions that return the base, bound and dimension of the components
//  There are always "maximumNumberOfComponents" defined (5 currently)
//  Note that components that were not explicitly defined default to base=0, bound=0, dimension=1
//----------------------------------------------------------------------------------------


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getComponentBase}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "floatMappedGridFunction::getComponentBase:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("floatMappedGridFunction::getComponentBase:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBase();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getComponentBound}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "floatMappedGridFunction::getComponentBound:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("floatMappedGridFunction::getComponentBound:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBound();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getComponentDimension}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( component<0 || component>=maximumNumberOfComponents )
  {
    cout << "floatMappedGridFunction::getComponentDimension:ERROR invalid argument, component = " 
         << component << endl;
    Overture::abort("floatMappedGridFunction::getComponentDimension:ERROR invalid argument");
  }
  return rcData->R[positionOfComponent(component)].getBound()
        -rcData->R[positionOfComponent(component)].getBase()+1;
}

//----------------------------------------------------------------------------------------
//  Here are functions that return the base, bound and dimension of the coordinates
//  There are always 3 coordinates defined
//  Note that coordinates that were not explicitly defined default to base=0, bound=0, dimension=1
//----------------------------------------------------------------------------------------

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getCoordinateBase}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "floatMappedGridFunction::getCoordinateBase:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("floatMappedGridFunction::getCoordinateBase:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBase();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getCoordinateBound}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "floatMappedGridFunction::getCoordinateBound:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("floatMappedGridFunction::getCoordinateBound:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBound();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getCoordinateDimension}}
int floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( coordinate<0 || coordinate>=3 )
  {
    cout << "floatMappedGridFunction::getCoordinateDimension:ERROR invalid argument, coordinate = " 
         << coordinate << endl;
    Overture::abort("floatMappedGridFunction::getCoordinateDimension:ERROR invalid argument");
  }
  return rcData->R[positionOfCoordinate(coordinate)].getBound()
        -rcData->R[positionOfCoordinate(coordinate)].getBase()+1;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getDerivatives}}
void floatMappedGridFunction::
getDerivatives(const Index & I1,  /* =nullIndex */
	       const Index & I2,  /* =nullIndex */ 
	       const Index & I3,  /* =nullIndex */ 
	       const Index & I4,  /* =nullIndex */ 
	       const Index & I5,  /* =nullIndex */
               const Index & I6,  /* =nullIndex */ 
	       const Index & I7,  /* =nullIndex */ 
	       const Index & I8   /* =nullIndex */ ) const
//==================================================================================
// /Description:
//   Get derivatives for this grid function. This routine just calls the 
//   function of the same name in the MappedGridOperators.
//   See the documentation for operators for further details.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators!=NULL )
  {
    //  derivativeError(); 
    #ifdef OV_USE_DOUBLE
        derivativeError(); 
      // operators->getDerivatives(*this,I1,I2,I3,I4,I5);
    #else
      // derivativeError(); 
        operators->getDerivatives(*this,I1,I2,I3,I4,I5);
    #endif
  }
  else
  {
    cout << "floatMappedGridFunction: ERROR: trying to take a derivative without defining operators\n";
  }
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getFaceCentering}}
GridFunctionParameters::faceCenteringType floatMappedGridFunction::
getFaceCentering() const
//---------------------------------------------------------------------------------------------
// /Description:
//     Get the type of face centering.
//      For further explanation see {\ff setFaceCentering} and section\ref{sec:cellFace}.
// /Errors:  none.
// /Return Values: faceCenteringType.
// 
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  return GridFunctionParameters::faceCenteringType(int(rcData->faceCentering));
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getGridFunctionType}} 
GridFunctionParameters::GridFunctionType floatMappedGridFunction::
getGridFunctionType(const Index & component0,  /* =nullIndex */
		    const Index & component1,  /* =nullIndex */
		    const Index & component2,  /* =nullIndex */
		    const Index & component3,  /* =nullIndex */
		    const Index & component4   /* =nullIndex */  ) const
//-----------------------------------------------------------------------------------------
// /Description:
//   Return the type of the grid function.
// /component0,component1,... (input): get type of the grid function corresponding to these
//    components. 
// /Return Values:
//    The grid function type, one of the enums in GridFunctionType.
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  GridFunctionParameters::GridFunctionType type = GridFunctionParameters::general;
  Range Axes(0,numberOfDimensions()-1);

  if( grid->gridType==MappedGrid::unstructuredGrid )
  {
    // unstructured grid
    const GridFunctionParameters::faceCenteringType faceType = getFaceCentering();
    if( faceType==GridFunctionParameters::faceCenteredUnstructured ||
	( faceType==GridFunctionParameters::direction0 && numberOfDimensions()==3 ) )// should be domainDimension==3
    {
      type=GridFunctionParameters::faceCenteredAll;
    }
    else if( faceType==GridFunctionParameters::edgeCenteredUnstructured ||
	( faceType==GridFunctionParameters::direction0 && numberOfDimensions()==2 ) )// should be domainDimension==3
    {
      type=GridFunctionParameters::edgeCentered;
    }
    else if( max(abs(isCellCentered(Axes,component0,component1,component2)))==0 )
      type=GridFunctionParameters::vertexCentered;
    else if( getIsCellCentered(Axes,component0,component1,component2,component3,component4) )
      type=GridFunctionParameters::cellCentered;
    else
    {
    
      printf("mappedGridFunction::getGridFunctionType:ERROR: unknown type of an unstructured grid!\n");
      type=GridFunctionParameters::vertexCentered;
    }
    return type;
  }

  if( max(abs(isCellCentered(Axes,component0,component1,component2)))==0 )
    type=GridFunctionParameters::vertexCentered;
  else if( getIsCellCentered(Axes,component0,component1,component2,component3,component4) )
    type=GridFunctionParameters::cellCentered;
  else 
  {
    const GridFunctionParameters::faceCenteringType faceType = getFaceCentering();
    // we can quickly tell the type if the whole grid function has faceCentering=direction<i>
    // or if we are looking at the entire grid function
    if( faceType==GridFunctionParameters::direction0 || 
        faceType==GridFunctionParameters::direction1 || 
        faceType==GridFunctionParameters::direction2 || 
        (component0.length()==0 && component1.length()==0 && component2.length()==0 &&
         component3.length()==0 && component4.length()==0)
      )
    { 
      switch (faceType) 
      {
      case GridFunctionParameters::none:
	type=GridFunctionParameters::general;  break;
      case GridFunctionParameters::all:
	type=GridFunctionParameters::faceCenteredAll; break;
      case GridFunctionParameters::direction0:
	type=GridFunctionParameters::faceCenteredAxis1; break;
      case GridFunctionParameters::direction1:
	type=GridFunctionParameters::faceCenteredAxis2; break;
      case GridFunctionParameters::direction2:
	type=GridFunctionParameters::faceCenteredAxis3; break;
      default:
	type=GridFunctionParameters::general;  break;
      }
    }
    else 
    { // now we have to check the general case
      //   we need to see if the components are
      //         faceCenteredAxis<i> or faceCenteredAll

      Range *R = rcData->R;  // make a reference to the array in rcData
      Range Rc[5]= {component0,component1,component2,component3,component4};
      for( int i=0; i<5; i++ )
      {
	if( Rc[i].length()==0 )
	  Rc[i]=R[positionOfComponent(i)];
      }
      if( Rc[3].length()!=1 || Rc[4].length()!=1 )
	cout << "floatMappedGridFunction::getGridFunctionType:ERROR: cannot handle more than 3 components\n";

      if( faceType==GridFunctionParameters::all && R[positionOfFaceCentering()]==Rc[numberOfComponents()] )
      { // we have a faceCenteredAll grid function and the full range of the "faceRange" Index
	type=GridFunctionParameters::faceCenteredAll;

      }
      else
      {
        IntegerArray faceCenteredness(Rc[0],Rc[1],Rc[2],Rc[3]); // ****** ,Rc[4]);  ***** fix 
	for( int c4=Rc[4].getBase(); c4<=Rc[4].getBound(); c4++ )
	{
	  for( int c3=Rc[3].getBase(); c3<=Rc[3].getBound(); c3++ )
	  {
	    for( int c2=Rc[2].getBase(); c2<=Rc[2].getBound(); c2++ )
	    {
	      for( int c1=Rc[1].getBase(); c1<=Rc[1].getBound(); c1++ )
	      {
		for( int c0=Rc[0].getBase(); c0<=Rc[0].getBound(); c0++ )
		{
		  int faceCount=0; // "counts" number of sides that are vertexCentered, scaled by 10's
		  for( int axis=axis1; axis<numberOfDimensions(); axis++ )
		    faceCount+= int( pow(10,axis) * (isCellCentered(axis,c0,c1,c2)? 0 : 1) ); 
		  if( faceCount==1 )
		    faceCenteredness(c0,c1,c2,c3)=GridFunctionParameters::faceCenteredAxis1;
		  else if( faceCount==10 )
		    faceCenteredness(c0,c1,c2,c3)=GridFunctionParameters::faceCenteredAxis2;
		  else if( faceCount==100 )
		    faceCenteredness(c0,c1,c2,c3)=GridFunctionParameters::faceCenteredAxis3;
		  else
		    faceCenteredness(c0,c1,c2,c3)=GridFunctionParameters::general;
		}
	      }
	    }
	  }
	}
	if( max(abs(faceCenteredness(Rc[0],Rc[1],Rc[2],Rc[3])-GridFunctionParameters::faceCenteredAxis1))==0 )
	  type=GridFunctionParameters::faceCenteredAxis1;
	else if( max(abs(faceCenteredness(Rc[0],Rc[1],Rc[2],Rc[3])-GridFunctionParameters::faceCenteredAxis2))==0 )
	  type=GridFunctionParameters::faceCenteredAxis2;
	else if( max(abs(faceCenteredness(Rc[0],Rc[1],Rc[2],Rc[3])-GridFunctionParameters::faceCenteredAxis3))==0 )
	  type=GridFunctionParameters::faceCenteredAxis3;
	else
	  type=GridFunctionParameters::general;
      }
    }
  }
  return type;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getGridFunctionTypeWithComponents}}
GridFunctionParameters::GridFunctionTypeWithComponents floatMappedGridFunction::
getGridFunctionTypeWithComponents(const Index & component0,  /* =nullIndex */
				  const Index & component1,  /* =nullIndex */
				  const Index & component2,  /* =nullIndex */
				  const Index & component3,  /* =nullIndex */
				  const Index & component4   /* =nullIndex */  ) const
//-----------------------------------------------------------------------------------------
// /Description:
//   Return the type of the grid function with the number of components.
// /component0,component1,... (input): get type of the grid function corresponding to these
//    components. By default (if no arguments are given) the number of components will be 
//      equal to the number of components that the grid function was made with. Otherwise the
//      number of components will equal the number of arguments that have been passed to
//      this routine (actually the number of arguments that are not a nullIndex)
// /Return Values:
//    The grid function type with number of components, one of the enums in GridFunctionParameters::GridFunctionTypeWithComponents.
// /Note:
//   In a faceCenteredAll grid function, the position taken by the faceRange does not count
//   as a component for the value returned by this routine.
//   \begin{verbatim}
//     MappedGrid mg(...); 
//     Range all;
//     floatMappedGridFunction u(mg,floatMappedGridFunction::faceCenterAll,2);  
//     u.getGridFunctionTypeWithComponents(); // == faceCenterAllWith1Component
//     u.getNumberOfComponents();             // == 1
//   \end{verbatim}   
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  assert( numberOfComponents() >= 0 && numberOfComponents() <= 5 );

  int numberOfLocalComponents=0;
  Range Rc[5]= {component0,component1,component2,component3,component4};
  for( int i=0; i<5; i++ )
  {
    if( Rc[i].length()>0 )
      numberOfLocalComponents=i+1;
  }
  if( numberOfLocalComponents==0 )
    numberOfLocalComponents=numberOfComponents();

  assert( numberOfLocalComponents<6 );
  
  switch (getGridFunctionType(component0,component1,component2,component3,component4))
  {
  case GridFunctionParameters::general:
    switch (numberOfLocalComponents) 
    {
    case 0:
      return GridFunctionParameters::generalWith0Components;
    case 1:
      return GridFunctionParameters::generalWith1Component;
    case 2:
      return GridFunctionParameters::generalWith2Components;
    case 3:
      return GridFunctionParameters::generalWith3Components;
    case 4:
      return GridFunctionParameters::generalWith4Components;
    case 5:
      return GridFunctionParameters::generalWith5Components;
    }
  case GridFunctionParameters::vertexCentered:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::vertexCenteredWith0Components;
    case 1:
      return GridFunctionParameters::vertexCenteredWith1Component;
    case 2:
      return GridFunctionParameters::vertexCenteredWith2Components;
    case 3:
      return GridFunctionParameters::vertexCenteredWith3Components;
    case 4:
      return GridFunctionParameters::vertexCenteredWith4Components;
    case 5:
      return GridFunctionParameters::vertexCenteredWith5Components;
    }
  case GridFunctionParameters::cellCentered:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::cellCenteredWith0Components;
    case 1:
      return GridFunctionParameters::cellCenteredWith1Component;
    case 2:
      return GridFunctionParameters::cellCenteredWith2Components;
    case 3:
      return GridFunctionParameters::cellCenteredWith3Components;
    case 4:
      return GridFunctionParameters::cellCenteredWith4Components;
    case 5:
      return GridFunctionParameters::cellCenteredWith5Components;
    }
  case GridFunctionParameters::faceCenteredAll:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::faceCenteredAllWith0Components; 
    case 1:
      return GridFunctionParameters::faceCenteredAllWith1Component;
    case 2:
      return GridFunctionParameters::faceCenteredAllWith2Components;
    case 3:
      return GridFunctionParameters::faceCenteredAllWith3Components;
    case 4:
      return GridFunctionParameters::faceCenteredAllWith4Components;
    case 5:
      return GridFunctionParameters::faceCenteredAllWith5Components;
    }
  case GridFunctionParameters::faceCenteredAxis1:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::faceCenteredAxis1With0Components;
    case 1:
      return GridFunctionParameters::faceCenteredAxis1With1Component;
    case 2:
      return GridFunctionParameters::faceCenteredAxis1With2Components;
    case 3:
      return GridFunctionParameters::faceCenteredAxis1With3Components;
    case 4:
      return GridFunctionParameters::faceCenteredAxis1With4Components;
    case 5:
      return GridFunctionParameters::faceCenteredAxis1With5Components;
    }
  case GridFunctionParameters::faceCenteredAxis2:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::faceCenteredAxis2With0Components;
    case 1:
      return GridFunctionParameters::faceCenteredAxis2With1Component;
    case 2:
      return GridFunctionParameters::faceCenteredAxis2With2Components;
    case 3:
      return GridFunctionParameters::faceCenteredAxis2With3Components;
    case 4:
      return GridFunctionParameters::faceCenteredAxis2With4Components;
    case 5:
      return GridFunctionParameters::faceCenteredAxis2With5Components;
    }
  case GridFunctionParameters::faceCenteredAxis3:
    switch (numberOfLocalComponents)
    {
    case 0:
      return GridFunctionParameters::faceCenteredAxis3With0Components;
    case 1:
      return GridFunctionParameters::faceCenteredAxis3With1Component;
    case 2:
      return GridFunctionParameters::faceCenteredAxis3With2Components;
    case 3:
      return GridFunctionParameters::faceCenteredAxis3With3Components;
    case 4:
      return GridFunctionParameters::faceCenteredAxis3With4Components;
    case 5:
      return GridFunctionParameters::faceCenteredAxis3With5Components;
    }
  default:
    cerr << "floatMappedGridFunction::getGridFunctionTypeWithComponents:ERROR unknown grid function type!! \n";
    cerr << "This error should not occur \n";
    exit(1);
  }
  
  if( this )
  {
    cerr << "floatMappedGridFunction::getGridFunctionTypeWithComponents:ERROR: numberOfLocalComponents=="
	 << numberOfLocalComponents << endl;
    exit (1);
  }
  
  return GridFunctionParameters::generalWith0Components;
}


bool floatMappedGridFunction:: 
getIsACoefficientMatrix() const
{
  return rcData->isACoefficientMatrix;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getIsCellCentered}}
bool floatMappedGridFunction::
getIsCellCentered(const Index & axis0,       /* =nullIndex */
                  const Index & component0,  /* =nullIndex */
                  const Index & component1,  /* =nullIndex */
                  const Index & component2,  /* =nullIndex */
                  const Index & component3,  /* =nullIndex */
                  const Index & component4   /* =nullIndex */  ) const
// ====================================================================================================
//  /Description:
//     Determine the cell centeredness of a grid function.
//  /axis0 (input): if axis0=nullIndex (default) then all axes are checked
//  /component0 (input): if component0=nullIndex (default) then all components are checked
//  /component1 (input): if component1=nullIndex (default) then all components are checked
//  /component2 (input): if component2=nullIndex (default) then all components are checked
//  /component3 (input): if component3=nullIndex (default) then all components are checked
//  /component4 (input): if component4=nullIndex (default) then all components are checked
//
//  /Return Values: TRUE or FALSE
//
//  /Detailed Description:
//     A {\ff floatMappedGridFunction} can be used for finite
//   difference and finite volume codes. Finite volume
//   codes often require that the grid function be cell-centered.
//   By default a {\ff floatMappedGridFunction} will be cell-centered
//   if the {\ff MappedGrid} is cell-centered or vertex-centered
//   if the {\ff MappedGrid} is vertex-centered.
//   
//     Finite-volume codes often require grid functions that
//   are face-centered. In order to support all the
//   various possibilities one can, in general, specify that
//   a grid function be cell-centered (or not) in some or all
//   of the coordinate directions. Use the member function
//   {\ff setIsCellCentered} to set the ``centeredness''
//   of each component of the grid function. Use 
//   {\ff getIsCellCentered} function to inquire the
//   centeredness of each component of a grid function.
//   
//   Since face-centered grid functions are common, the
//   function {\ff setIsFaceCentered(axis,component)} can be used
//   to create a face-centered grid function in the 
//   coordinate direction ``axis'' for a given component.
//   (A face-centered grid function is vertex centered in
//   the ``axis-direction'' and cell-centered in the other
//   directions).
//   The function {\ff getIsFaceCentered} can be used to
//   determine if a grid function is face centered in a
//   given direction.
//   
//   For example
//   {\footnotesize\begin{verbatim}
//     ...
//     realMappedGridFunction u(mg,3);   // a grid function with 3 components
//     int axis=0, component=0;
//     u.setIsCellCentered(TRUE,axis,component); // make u cell centred along axis 0 for component 0
//   
//     axis=1; component=1;
//     u.setIsCellCentered(FALSE,axis,component); // make u vertex centred along axis 0 for component 1
//   
//     // inquire the cell-centredness
//     cout << "u.getIsCellCentered(axis,component) = " << u.getIsCellCentered(axis,0) << endl;
//   
//     u.setIsFaceCentered( axis,component );   // make u face-centered along axis for a component
//   
//   \end{verbatim}
//     For further explanation see section\ref{sec:cellFace}.
//   }
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
// ====================================================================================================
{
  bool returnValue=TRUE;
  Range *R = rcData->R;  // make a reference to the array in rcData

  Range Rc[5]= {component0,component1,component2,component3,component4};
  for( int i=0; i<5; i++ )
  {
    if( Rc[i].length()==0 )
      Rc[i]=R[positionOfComponent(i)];
  }
  if( Rc[3].length()!=1 || Rc[4].length()!=1 )
    cout << "floatMappedGridFunction::getIsCellCentered:ERROR: cannot handle more than 3 components\n";
  Index Axes;
  Axes = (axis0.length()==0) ? Index(0,numberOfDimensions()) : axis0;

  for( int c4=Rc[4].getBase(); c4<=Rc[4].getBound(); c4++ )
    for( int c3=Rc[3].getBase(); c3<=Rc[3].getBound(); c3++ )
      for( int c2=Rc[2].getBase(); c2<=Rc[2].getBound(); c2++ )
	for( int c1=Rc[1].getBase(); c1<=Rc[1].getBound(); c1++ )
	  for( int c0=Rc[0].getBase(); c0<=Rc[0].getBound(); c0++ )
            for( int axis=Axes.getBase(); axis<=Axes.getBound(); axis++ )
  	      returnValue = returnValue && isCellCentered(axis,c0,c1,c2);    // ******************  c3

  return returnValue;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getIsFaceCentered}}
bool floatMappedGridFunction::
getIsFaceCentered(const int   & axis0,      /* =forAll */
                  const Index & component0, /* =nullIndex */
                  const Index & component1, /* =nullIndex */
                  const Index & component2, /* =nullIndex */
                  const Index & component3, /* =nullIndex */
                  const Index & component4  /* =nullIndex */ ) const
//==================================================================================
// /Description:
//   Determine if a given component of this grid function is face-centred along a given axis.
//   By default check all axes and all components.
// /axis0: check if the components are face centred along this axis. By default check if
//   the components are face centred in ANY direction.
// /component0, component1,... (input): check the value for these components, by default
//   check all components.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  bool returnValue=FALSE;
  Range *R = rcData->R;  // make a reference to the array in rcData

  Range Rc[5]= {component0,component1,component2,component3,component4};
  for( int i=0; i<5; i++ )
  {
    if( Rc[i].length()==0 )
      Rc[i]=R[positionOfComponent(i)];
  }
  if( Rc[3].length()!=1 || Rc[4].length()!=1 )
    cout << "floatMappedGridFunction::getIsFaceCentered:ERROR: cannot handle more than 3 components\n";

  for( int c4=Rc[4].getBase(); c4<=Rc[4].getBound(); c4++ )
    for( int c3=Rc[3].getBase(); c3<=Rc[3].getBound(); c3++ )
      for( int c2=Rc[2].getBase(); c2<=Rc[2].getBound(); c2++ )
	for( int c1=Rc[1].getBase(); c1<=Rc[1].getBound(); c1++ )
	  for( int c0=Rc[0].getBase(); c0<=Rc[0].getBound(); c0++ )
            if( axis0==forAll )
	    {
	      int numberOfVertexCenteredSides=0;  // face centred = number of vertex sides = 1
	      for( int axis=axis1; axis<numberOfDimensions(); axis++ )
		numberOfVertexCenteredSides+= isCellCentered(axis,c0,c1,c2)? 0 : 1 ; 
	      returnValue=returnValue || numberOfVertexCenteredSides==1;
	    }
            else
	    {
	      bool faceCentered=TRUE; 
	      for( int axis=axis1; axis<numberOfDimensions(); axis++ )
		faceCentered=faceCentered && ((isCellCentered(axis,c0,c1,c2)==TRUE)==(axis!=axis0));
	      returnValue=returnValue || faceCentered;
	    }

  return returnValue;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getMappedGrid}}
MappedGrid* floatMappedGridFunction::
getMappedGrid(const bool abortIfNull /* =TRUE */ ) const
//==================================================================================
// /Description:
//   Return a pointer to the MappedGrid that this grid function is asscoaiated with
//   By default this function will abort if the pointer is NULL.
// /Return values:
//   A pointer to a MappedGrid or NULL
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( abortIfNull && mappedGrid==NULL )
  {
    cout << "floatMappedGridFunction:getMappedGrid:ERROR: The pointer to the MappedGrid is NULL \n";
    Overture::abort("error");
  }
  return mappedGrid;
}


// This macro makes the name[] array look like a multidimensional array
#define GFNAME(c0,c1,c2,c3,c4) name[1+(c0-R[positionOfComponent(0)].getBase())+R[positionOfComponent(0)].length()*(  \
                                      (c1-R[positionOfComponent(1)].getBase())+R[positionOfComponent(1)].length()*(  \
                                      (c2-R[positionOfComponent(2)].getBase())+R[positionOfComponent(2)].length()*(  \
                                      (c3-R[positionOfComponent(3)].getBase())+R[positionOfComponent(3)].length()*(  \
                                      (c4-R[positionOfComponent(4)].getBase())))))]
                            

  
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getName}}
aString floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( rcData->name && rcData->numberOfNames > 0 )
  {
    Range *R = rcData->R;  // make a reference to the array in rcData
    // dimensionName();
    int c[maximumNumberOfComponents] = { component0, component1, component2, component3, component4};
					 
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

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getNumberOfComponents}}  
int floatMappedGridFunction::
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
//     MappedGrid mg(...); 
//     Range all;
//     floatMappedGridFunction u(mg);                           // 0 components
//     floatMappedGridFunction u(mg,all,all,all);               // 0 components
//     floatMappedGridFunction u(mg,all,all,all,1);             // 1 component
//     floatMappedGridFunction u(mg,all,all,all,2,2);           // 2 components
//     floatMappedGridFunction u(mg,all,all,all,faceRange);     // 0 components
//     floatMappedGridFunction u(mg,all,all,all,3,faceRange);   // 1 component
//   \end{verbatim}   
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  return rcData->numberOfComponents;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{getOperators}}
MappedGridOperators*  floatMappedGridFunction::
getOperators() const
//==================================================================================
// /Description:
//    get the operators used with this grid function. Return NULL if there are none.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( operators ) 
    return (MappedGridOperators*)operators;
  else
    return NULL;
}

//-----------------------------------------------------------------------------------------
// return the current values for the Ranges
//-----------------------------------------------------------------------------------------
void  floatMappedGridFunction::
getRanges(Range & R0,   
	  Range & R1,
	  Range & R2,
	  Range & R3,
	  Range & R4,
	  Range & R5,
	  Range & R6,
	  Range & R7 )
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

floatMappedGridFunction::stencilTypes floatMappedGridFunction:: 
getStencilType() const
{
  return stencilTypes(int(rcData->stencilType));
}


int floatMappedGridFunction::
getStencilWidth() const
{
  return rcData->stencilWidth;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{isNull}}
bool floatMappedGridFunction::
isNull()
//==================================================================================
// /Description:
//   Return TRUE if this grid function is null (has no grid associated with it).
//    
// /Return value:
//   Return TRUE if this grid function is null, otherwise return FALSE.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  return mappedGrid==NULL;
}

//-----------------------------------------------------------------------------
// link this grid function to a component of another grid function
//  
//  mgf : Grid function to link to
//  R0    : link to these components of the first set of components of mgf
//  R1    : link to these components of the second set of components of mgf
//          If this Range is specified then the components linked to must
//          be contiguous:
//            (a) R0 and R1 can both refer to 1 element
//            (b) R0 can be all elements and R1 a subset of the elements
//  etc.
//
// Examples:
//  (1) Linking to a vector grid function:
//      MappedGrid mg(...);
//      Range R0(0,3);
//      floatMappedGridFunction u(mg,all,all,all,R0);  // u is a vector grid function
//      floatMappedGridFunction v;
//      v.link(u,Range(0,0));   // link to component 0 of u         -> v(all,all,all,0:0)
//      v.link(u,Range(0,1));   // link to components 0 and 1 of u  -> v(all,all,all,0:1)
//      v.link(u,Range(2,2));   // link to component 2 u            -> v(all,all,all,0:0)
//
//  (2) Linking to a matrix grid function:
//      MappedGrid mg(...);
//      Range R0(0,3), R1(0,2);
//      floatMappedGridFunction u(mg,all,all,all,R0,R1);  // u is a 2D matrix grid function
//      v.link(u,Range(1,1));   // link to matrix element (1,0)  -> v(all,all,all,0:0,0:0)
//      v.link(u,Range(1,1),Range(2,2));   // link to component (1,2) -> v(all,all,all,0:0,0:0)
//      v.link(u,R0,Range(2,2));   // link to components (R0,2) -> v(all,all,all,0:3,0:0)
//
//      v.link(u,Range(1,1),Range(0,2));   // **ERROR** these values are not contiguous
//
//-----------------------------------------------------------------------------
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{link}}
void floatMappedGridFunction::
link(const floatMappedGridFunction & mgf, 
     const Range & R0,                 /* =nullRange */
     const Range & R1,                 /* =nullRange */
     const Range & R2,                 /* =nullRange */
     const Range & R3,                 /* =nullRange */
     const Range & R4                  /* =nullRange */ )
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
//  A link is sort of like a reference since the array data is shared. NOTE that a link can
//  only be made to a grid function whose components appear at the end of the 
//  array (position 3 for 3D grid functions or positions 2 or 3 for 2D grid functions).
//  Links can also be made to more than one components, provided the components are
//  contiguous. 
//
//  (1) Linking to a vector grid function:
//  {\footnotesize\begin{verbatim}
//      MappedGrid mg(...);
//      Range R0(0,3);
//      floatMappedGridFunction u(mg,all,all,all,R0);  // u is a vector grid function
//      floatMappedGridFunction v;
//      v.link(u,Range(0,0));   // link to component 0 of u         -> v(all,all,all,0:0)
//      v.link(u,Range(0,1));   // link to components 0 and 1 of u  -> v(all,all,all,0:1)
//      v.link(u,Range(2,2));   // link to component 2 u            -> v(all,all,all,0:0)
//  \end{verbatim}
//  }
//
//  (2) Linking to a matrix grid function:
//  {\footnotesize\begin{verbatim}
//      MappedGrid mg(...);
//      Range R0(0,3), R1(0,2);
//      floatMappedGridFunction u(mg,all,all,all,R0,R1);  // u is a 2D matrix grid function
//      v.link(u,Range(1,1));   // link to matrix element (1,0)  -> v(all,all,all,0:0,0:0)
//      v.link(u,Range(1,1),Range(2,2));   // link to component (1,2) -> v(all,all,all,0:0,0:0)
//      v.link(u,R0,Range(2,2));   // link to components (R0,2) -> v(all,all,all,0:3,0:0)
//
//      v.link(u,Range(1,1),Range(0,2));   // **ERROR** these values are not contiguous
//  \end{verbatim}
//  }
//
//  /Errors: Attempt to link to invalid components.
//  /Return Values: none.
// 
//  /Notes:
//     The linkee function will acquire the same operators as the function being linked to.
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//======================================================================================================
{
  grid = mgf.grid;   
  mappedGrid = mgf.mappedGrid;
  operators=mgf.operators;
  if( grid==NULL )
  {
    cerr << "floatMappedGridFunction:ERROR in link, trying to link to a floatMappedGridFunction \n"
      " that has a NULL grid! This is not allowed \n";
    if( getName()!=" " ) cout << "GridFunctionName = " << getName() << endl;
    exit(1);
  }
  // *****   we should reference some of these *****
  rcData->numberOfDimensions     =mgf.rcData->numberOfDimensions;
//  positionOfComponent    =mgf.positionOfComponent;
//  positionOfCoordinate   =mgf.positionOfCoordinate;
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
  {
    rcData->positionOfComponent[i]  =mgf.positionOfComponent(i);
    rcData->positionOfCoordinate[i] =mgf.positionOfCoordinate(i);
  }
  

  rcData->positionOfFaceCentering=mgf.rcData->positionOfFaceCentering;    
  rcData->faceCentering          =mgf.rcData->faceCentering;              // **** is this correct? ******
  rcData->isACoefficientMatrix   =mgf.rcData->isACoefficientMatrix;
  rcData->stencilType            =mgf.rcData->stencilType;
  rcData->stencilWidth           =mgf.rcData->stencilWidth;
// stencilOffset          =mgf.stencilOffset;
  
  Range *R = rcData->R;  // make a reference to the array in rcData
  Range *Ra= rcData->Ra; // make a reference to the array in rcData
  Range *Rc= rcData->Rc; // make a reference to the array in rcData
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
    R[i]=mgf.rcData->R[i];
  for( i=0; i<numberOfIndicies; i++ )
    Ra[i]=mgf.rcData->Ra[i];
  for( i=0; i<3; i++ )
    Rc[i]=mgf.rcData->Rc[i];


  Range Rl[maximumNumberOfComponents+1] = { R0,R1,R2,R3,R4,nullRange };   

  // Find the component that we will link to, it is the last non-null Range
  // For extra components that are not specified, choose the base value
  int link=maximumNumberOfComponents;                  // link is the component number that we will link to
  for( i=maximumNumberOfComponents; i>=0; i-- )      // it is the last non-null Range
    if( Rl[i].length()==0 )  //    if( Rl[i]==nullRange )
    {
      link=i-1;
      Rl[i]=Range(R[positionOfComponent(i)].getBase(),R[positionOfComponent(i)].getBase());
    }
  if( link<0 )
  {
    cout << "floatMappedGridFunction::link:ERROR nothing to link to!\n";
    cout << "The name of the grid function is " << mgf.getName() << endl;
    Overture::abort("mappedGridFunction::link:ERROR");
  }
    
  // We can only link to components that are contiguous:
  if( Rl[link].length()>1 )
  { // if linking to more than one component then Ranges before the link must be full size
    for( i=0; i<link; i++ )
    {
      if( ! ( Rl[i].getBase() ==R[positionOfComponent(i)].getBase() &&
	     Rl[i].getBound()==R[positionOfComponent(i)].getBound() ) )
      {
	cout << "floatMappedGridFunction::link:ERROR trying to link to invalid components\n";
        cout << "If you link to multiple components, the components you link to must be contiguous \n";
        cout << "Thus the linked components before the last must be full size\n";
	Overture::abort("mappedGridFunction::link:ERROR");
      }
    }
  }
  else
  { // we can link to sets of components of the form  (all,...,all,partial,1,...1)
    bool allThePreviousComponentsUsed=TRUE;
    for( i=0; i<link; i++ )
    {
      if( allThePreviousComponentsUsed &&
         Rl[i].getBase() ==R[positionOfComponent(i)].getBase() &&
	 Rl[i].getBound()==R[positionOfComponent(i)].getBound() )
      {
	if( !allThePreviousComponentsUsed && Rl[i].length()>1 )
	{
          cout << "floatMappedGridFunction::link:ERROR trying to link to an invalid component\n";
          cout << "The name of the grid function is " << mgf.getName() << endl;
          cout << "You must link to a contiguous part of the grid function\n";    
          Overture::abort("mappedGridFunction::link:ERROR");
	}
      }
      else if( allThePreviousComponentsUsed &&
         R[positionOfComponent(i)].getBase()  <= Rl[i].getBase() &&   // link must be a subset
	 R[positionOfComponent(i)].getBound() >= Rl[i].getBound() )
      { // first non-all range can be a partial range 
	allThePreviousComponentsUsed=FALSE;
      }
      else if( Rl[i].length()==1 )
      { // all remaining Ranges must have length 1
	allThePreviousComponentsUsed=FALSE;
      }
      else // linking to only part of a component, which is not the last
      {
        cout << "floatMappedGridFunction::link:ERROR trying to link to an invalid component\n";
        cout << "The name of the grid function is " << mgf.getName() << endl;
        cout << "You must link to a contiguous part of the grid function\n";    
        cout << "Contiguous links must be of the form (all,...,all,partial,1,...,1) \n";
        Overture::abort("mappedGridFunction::link:ERROR");
      }
    }
  }
  
  if( !( R[positionOfComponent(link)].getBase()  <= Rl[link].getBase() &&   // link must be a subset
	 R[positionOfComponent(link)].getBound() >= Rl[link].getBound() ) )
  {
    cout << "floatMappedGridFunction::link:ERROR trying to link to an invalid component\n";
    cout << "The name of the grid function is " << mgf.getName() << endl;
    cout << "You are trying to link to position " << positionOfComponent(link) << endl;
    cout << " Here is the Range that you specified to link to : (" 
         << Rl[link].getBase() << "," 
         << Rl[link].getBound() << ")" << endl;
    cout << " Here is the Range that you are allowed to link to ("
         << R[positionOfComponent(link)].getBase() << "," 
         << R[positionOfComponent(link)].getBound() << ")" << endl;
    Overture::abort("mappedGridFunction::link:ERROR");
  }
  
 //link must be greater than coordinate's
  if( positionOfComponent(link) < positionOfCoordinate(numberOfDimensions()-1) )
  {
    cout << "floatMappedGridFunction::link:ERROR trying to link to an invalid component\n";
    cout << "The name of the grid function is " << mgf.getName() << endl;
    cout << "You are trying to link to position " << positionOfComponent(link) << endl;
    cout << "...but this component lies before the last Coordinate at position " 
         << positionOfCoordinate(numberOfDimensions()-1) << endl;
    Overture::abort("mappedGridFunction::link:ERROR");
  }

  // *********************************************************************
  // The new Range for the linked grid function always starts at 0
  // The user can use the updateToMatchgGrid function to change this
  // *********************************************************************
  for( i=0; i<=link; i++ )  
    R[positionOfComponent(i)]=Range(0,Rl[i].getBound()-Rl[i].getBase());

  for( i=positionOfComponent(link)+1; i<maximumNumberOfIndicies; i++ )
    R[i]=Range(mgf.rcData->R[i].getBase(),mgf.rcData->R[i].getBase());   // these ranges are length 1

  // ===== assign the actual Ranges =====
  for( i=0; i<numberOfIndicies; i++ )
    Ra[i]=R[i];                            // actual Ranges for the under-lying A++ array

  // for now: merge extra components together   **** remove this when A++ is fixed *******
  if( R[4].length() > 1  )
    Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[4].length()-1);
  if( R[5].length() > 1  )
    Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[5].length()-1);
  if( R[6].length() > 1  )
    Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[6].length()-1);
  if( R[7].length() > 1  )
    Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[7].length()-1);

  // The array linkBase is used to adopt to the grid function mgf
  // determine which element to link to as a starting position
  IntegerArray linkBase(maximumNumberOfIndicies+1);
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
    linkBase(i)=mgf.rcData->R[i].getBase();
  for( i=0; i<=link; i++ )
    linkBase(positionOfComponent(i))=Rl[i].getBase(); 
  
  // adopt this grid function to the correct portion of mgf, the new dimensions of this are in Ra[i]
// * wdh 960729
// *  adopt(&mgf(linkBase(0),linkBase(1),linkBase(2),
// *             mgf.arg3(linkBase(3),linkBase(4),linkBase(5),linkBase(6))),  
// *        Ra[0],Ra[1],Ra[2],Ra[3]);

// Reference this grid function to the appropriate view of the linkee
  floatDistributedArray::reference(mgf(Index(linkBase(0),Ra[0].length()),
			     Index(linkBase(1),Ra[1].length()),
			     Index(linkBase(2),Ra[2].length()),
			     Index(mgf.arg3(linkBase(3),linkBase(4),linkBase(5),linkBase(6)),Ra[3].length())));
  for( int axis=0; axis<4; axis++ )
    setBase(Ra[axis].getBase(),axis);

//  adopt(&mgf(linkBase(0),linkBase(1),linkBase(2),linkBase(3),linkBase(4),linkBase(5),linkBase(6)),  
//        Ra[0],Ra[1],Ra[2],Ra[3]);

  // update the isCellCentered array
  // copy over the centredness from the source grid function
  isCellCentered.redim(3,R[positionOfComponent(0)],    // ****** fix this for 4 components ********
                         R[positionOfComponent(1)],
	                 R[positionOfComponent(2)]);
  
  isCellCentered=FALSE;   // by default extra dimensions are vertex centred
  Range Axes(0,numberOfDimensions()-1);
  isCellCentered(Axes,R[positionOfComponent(0)],
                      R[positionOfComponent(1)],
	              R[positionOfComponent(2)])=mgf.isCellCentered(Axes,Rl[0],Rl[1],Rl[2]);

  // The linkee retains the positionOfFaceCentering only if the Range corresponding
  // to the positionOfFaceCentering is the same size
  if( positionOfFaceCentering() >= 0 
      && ( R[positionOfFaceCentering()].getBase()!=mgf.rcData->R[positionOfFaceCentering()].getBase()
      ||   R[positionOfFaceCentering()].getBound()!=mgf.rcData->R[positionOfFaceCentering()].getBound() ))
  {
    // change the faceCentering to match the linkee
    if( R[positionOfFaceCentering()].getBase()==0 && R[positionOfFaceCentering()].getBound()==0 )
      rcData->faceCentering=GridFunctionParameters::direction0;    
    else if( R[positionOfFaceCentering()].getBase()==1 && R[positionOfFaceCentering()].getBound()==1 )
      rcData->faceCentering=GridFunctionParameters::direction1;    
    else if( R[positionOfFaceCentering()].getBase()==2 && R[positionOfFaceCentering()].getBound()==2 )
      rcData->faceCentering=GridFunctionParameters::direction2;    
    else
      rcData->faceCentering=GridFunctionParameters::none;
    rcData->positionOfFaceCentering=-1;
  } 


  // 95/10/30 set the number of components for the linkee.
  setNumberOfComponents(positionOfFaceCentering()<0 ? link+1 : link);

  // give names from the linkee  ** is this what we want to do ? ***
  setName( mgf.getName() );   // set name for grid function
  assert( link<maximumNumberOfComponents );     
  for( int c4=R[positionOfComponent(4)].getBase(); c4<R[positionOfComponent(4)].getBound(); c4++ )
    for( int c3=R[positionOfComponent(3)].getBase(); c3<R[positionOfComponent(3)].getBound(); c3++ )
      for( int c2=R[positionOfComponent(2)].getBase(); c2<R[positionOfComponent(2)].getBound(); c2++ )
	for( int c1=R[positionOfComponent(1)].getBase(); c1<R[positionOfComponent(1)].getBound(); c1++ )
	  for( int c0=R[positionOfComponent(0)].getBase(); c0<R[positionOfComponent(0)].getBound(); c0++ )
	    setName( mgf.getName(c0,c1,c2,c3),c0,c1,c2,c3 );

}

//-----------------------------------------------------------------------------
// link this grid function to a component of another grid function
//  
//  mgf : Grid function to link to
//  componentToLinkTo : link to this component (must equal 3 for a 3D grid function, 
//        or equal 2,3 for a 2D grid function)
//  numberOfComponents0 : number of compnents in this grid function
//-----------------------------------------------------------------------------
void floatMappedGridFunction::
link(const floatMappedGridFunction & mgf, const int componentToLinkTo, const int numberOfComponents0 )
{
  Range R(componentToLinkTo,componentToLinkTo+numberOfComponents0-1);
  link( mgf,R );
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{multiply(a,coeff)}}
floatMappedGridFunction &
multiply( const floatMappedGridFunction & a_, const floatMappedGridFunction & coeff_ )
//==================================================================================
// /Description:
//    Multiply a grid function times a coefficient matrix. Use this function
//  to multiply a scalar grid function "a" times a coefficient matrix "coeff".
//  The result is saved in coeff and returned by reference.    
//  \begin{verbatim}
//        coeff(M,I1,I2,I3) <- a(I1,I2,I3)*coeff(M,I1,I2,I3)
//  \end{verbatim}
//
// /a\_ (input) : a scalar grid function.
// /coeff\_ (input/output) : a grid function in the shape a coefficient matrix (1 component in position 0)
//   This argument is NOT const but it was made to to prevent some compiler warnings.
//
// /Return value: a reference to coeff
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  floatMappedGridFunction & a = (floatMappedGridFunction&)a_;  // cast away const
  floatMappedGridFunction & coeff = (floatMappedGridFunction &) coeff_;
  
  
  Index I1,I2,I3;
  if( a.getLength(3)!=1 )
  {
    cout << "floatMappedGridFunction: multiply(a,coeff):ERROR: `a' should be a scalar grid function\n";
    Overture::abort("multiply(a,coeff):ERROR");
  }
  a.reshape(1,Range(a.getBase(0),a.getBound(0)),
		  Range(a.getBase(1),a.getBound(1)),
		  Range(a.getBase(2),a.getBound(2)));
    
  MappedGrid & mg = *coeff.getMappedGrid();
  getIndex(mg.dimension(),I1,I2,I3);
  for( int m=coeff.getBase(0); m<=coeff.getBound(0); m++ )
     coeff(m,I1,I2,I3)*=a(0,I1,I2,I3);

  a.reshape(Range(a.getBase(1),a.getBound(1)),
		  Range(a.getBase(2),a.getBound(2)),
		  Range(a.getBase(3),a.getBound(3)));
  return coeff;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{multiply(a,coeff)}}
floatMappedGridFunction &
multiply( const floatDistributedArray & a_, const floatMappedGridFunction & coeff_ )
//==================================================================================
// /Description:
//    Multiply an array times a coefficient matrix. Use this function
//  to multiply a "scalar" array "a" times a coefficient matrix "coeff".
//  The result is saved in coeff and returned by reference.    
//  \begin{verbatim}
//        coeff(M,I1,I2,I3) <- a(I1,I2,I3)*coeff(M,I1,I2,I3)
//  \end{verbatim}
//
// /a\_ (input) : an array with the same dimensions as a grid function.
// /coeff\_ (input/output) : a grid function in the shape a coefficient matrix (1 component in position 0)
//   This argument is NOT const but it was made to to prevent some compiler warnings.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  floatDistributedArray & a = (floatDistributedArray&) a_; // cast away const
  floatMappedGridFunction & coeff = (floatMappedGridFunction &) coeff_;  // cast away const

  Index I1,I2,I3;
  if( a.getLength(3)!=1 )
  {
    cout << "floatMappedGridFunction: multiply(a,coeff):ERROR: `a' should not be a scalar grid function\n";
    Overture::abort("multiply(a,coeff):ERROR");
  }
  a.reshape(1,Range(a.getBase(0),a.getBound(0)),
		  Range(a.getBase(1),a.getBound(1)),
		  Range(a.getBase(2),a.getBound(2)));
    
  MappedGrid & mg = *coeff.getMappedGrid();
  getIndex(mg.dimension(),I1,I2,I3);
  for( int m=coeff.getBase(0); m<=coeff.getBound(0); m++ )
     coeff(m,I1,I2,I3)*=a(0,I1,I2,I3);

  a.reshape(Range(a.getBase(1),a.getBound(1)),
		  Range(a.getBase(2),a.getBound(2)),
		  Range(a.getBase(3),a.getBound(3)));
  return coeff;
}

//==================================================================================
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{numberOfComponents}}
//\no function header:
// \noindent{\bf const int\&}\\
// \noindent{\bf numberOfComponents() const }
//
// /Return value: the number of components (0=scalar, 1=vector, ...)
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================

//==================================================================================
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{numberOfDimensions}}
//\no function header:
// \noindent{\bf const int\&} \\
// \noindent{\bf numberOfDimensions() const }
//
// /Return value: the numberOfDimensions of the grid function (equal to the domain dimension of the grid)
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================


//---------------------------------------------------------------------------------------------------
//  Since A++ only supports 4 arguments we support more than 4 arguments by converting
//  into 4 indirect addressing Arrays **** this is slow, but should work ****
//---------------------------------------------------------------------------------------------------
floatDistributedArray  floatMappedGridFunction::  
operator()(const IndexArg & I0, 
	   const IndexArg & I1,
	   const IndexArg & I2,
	   const IndexArg & I3,
	   const IndexArg & I4) const
{
  cout << "floatMappedGridFunction::ERROR: The operator (Index,Index,Index,Index,Index) is not implemented!\n";
  if( &I0 ) // if( TRUE ) to fool kcc compiler
    Overture::abort("floatMappedGridFunction::ERROR");
  return floatDistributedArray::operator()(I0);
}

floatDistributedArray  floatMappedGridFunction::
operator()(const IndexArg & I0, 
	   const IndexArg & I1,
	   const IndexArg & I2,
	   const IndexArg & I3,
	   const IndexArg & I4,
	   const IndexArg & I5) const
{
  cout << "floatMappedGridFunction::ERROR: The operator (Index,Index,Index,Index,Index,Index) is not implemented!\n";
  if( &I0 )
    Overture::abort("floatMappedGridFunction::ERROR");
  return floatDistributedArray::operator()(I0);
}

floatDistributedArray  floatMappedGridFunction::
operator()(const IndexArg & I0, 
	   const IndexArg & I1,
	   const IndexArg & I2,
	   const IndexArg & I3,
	   const IndexArg & I4,
	   const IndexArg & I5,
	   const IndexArg & I6) const
{
  cout << "floatMappedGridFunction::ERROR: The operator (Index,Index,Index,Index,Index,Index,Index) is not implemented!\n";
  if( &I0 )
    Overture::abort("floatMappedGridFunction::ERROR");
  return floatDistributedArray::operator()(I0);
}

floatDistributedArray  floatMappedGridFunction::
operator()(const IndexArg & I0, 
	   const IndexArg & I1,
	   const IndexArg & I2,
	   const IndexArg & I3,
	   const IndexArg & I4,
	   const IndexArg & I5,
	   const IndexArg & I6,
	   const IndexArg & I7) const
{
  cout << "floatMappedGridFunction::ERROR: The operator (Index,Index,Index,Index,Index,Index,Index,Index) is not implemented!\n";
  if( &I0 )
    Overture::abort("floatMappedGridFunction::ERROR");
  return floatDistributedArray::operator()(I0);
}

  
//-----------------------------------------------------------------------------------------
// Assignment with = is a deep copy
// only copy operators if they have not been assigned yet
//-----------------------------------------------------------------------------------------
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{operator = floatMappedGridFunction}}  
floatMappedGridFunction & floatMappedGridFunction::
operator= ( const floatMappedGridFunction & cgf )
//==================================================================================
// /Description:
//   Set one grid function equal to another. This is a shallow copy where only the
//    array data is copied. An error occurs if the two grid functions are not conformable.
//    This operation has the same affect as the {\tt dataCopy} memeber function.
//    An exception to this rule is when the grid function to the left of the equals operator
//    is a `null' grid function (one that has no grid associated with it such as a grid function
//    built by the default constructor). In this case  a deep copy is performed.
// /Examples:
//    Here are some examples
// \begin{verbatim}
//   MappedGrid mg(...);
//   realMappedGridFunction u(mg),v(mg),w;
//   Index I;
//   ...
//   u=1.;
//   v=u;                 // only the data is copied
//   w=u;                 // this is a deep copy since w is a `null' grid function.
//   u=v+w;               // does NOT call this = operator, uses grid-function=A++ array
//   u=v(I)+w(I);         // does NOT call this = operator, uses grid-function=A++ array 
//   u=3;                 // does NOT call this = operator, uses grid-function=scalar
//   u(I)=v(I)+v(I);      // does NOT call this = operator, uses A++ =
//   u.dataCopy(v+w);     // only copies array data (same as u=v+w; in this case)
//   u.updateToMatchGridFunction(v);  // this is a real deep copy.
//   realMappedGridFunction a = u;  // does NOT call this = operator, calls copy constructor
// \end{verbatim}  
// % /Notes: ~~
// %
// %   \begin{itemize}
// %    \item This is a deep copy, except as noted below. This means that the left operand
// %      will get the same number of components, cell-centeredness , etc. or the right operand.
// %      Use the {\tt dataCopy} function if you only want to copy the array data, or  
// %      else use an Index operation as in {\ff u(I)=v(I)} instead of {\ff u=v}. The former
// %      operation will only copy the indicated data.  Use the  {\ff updateToMatchGridFunction}
// %      function if you really want a deep copy.  
// %    \item If the operators have already been assigned (with setOperators) for left operand
// %       (*this) then the operators are NOT set equal to those of the right operand.
// %    \item If the left operand is a Coefficient matrix with a sparse-matrix representation
// %      (set with isACoefficientMatrix(TRUE)) then the result remains a coefficient-matrix
// %      and the sparse representation is NOT set
// %      equal to the right operand's sparse representation.
// %   \end{itemize}
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( this == &cgf )
    return *this;
  
  if( grid!=0 && cgf.grid!=0 )
  {
    // SHALLOW COPY
    if( !isConformable(cgf) )
      printf("floatMappedGridFunction:operator= ERROR: Try to assign two grid functions that are not conformable\n");
    floatDistributedArray::operator= (cgf);
  }
  else if( cgf.grid!=0 )
  {
    // DEEP COPY
    assert(cgf.rcData!=NULL);
    *rcData                =*cgf.rcData;  // deep copy
    grid                   =cgf.grid;        // note: this only copies the pointer to the grid ****
    mappedGrid             =cgf.mappedGrid; 
    if( operators==NULL )
      operators            =cgf.operators;

    if( grid==NULL )
    { // only copy sparse pointer if this is a null grid Function
      if( sparse && sparse->decrementReferenceCount()==0 )
	delete sparse;
      sparse = cgf.sparse;
      if( sparse )
	sparse->incrementReferenceCount();
    }  

    // reference objects in the envelope to the letter, no need to reference scalars because the
    // deep copy of rcData above will not break the reference
    // positionOfComponent.reference(rcData->positionOfComponent);
    // positionOfCoordinate.reference(rcData->positionOfCoordinate);
    // stencilOffset.reference(rcData->stencilOffset);

    isCellCentered.redim(cgf.isCellCentered);
    isCellCentered=cgf.isCellCentered;

    if( grid==NULL )
    {
      if( cgf.elementCount() != 0 )
      {
	cerr << "floatMappedGridFunction:ERROR in operator=, grid is NULL and array is not null!" << endl;
	if( getName()!=" " ) cout << "GridFunctionName = " << getName() << endl;
	exit(1);
      }
      else
	redim(0);   // make this array empty if the grid is NULL
    }
    else
    {
      // do NOT update the sparse rep for a coefficient matrix:
      UpdateToMatchGridOption oldOption=UpdateToMatchGridOption(rcData->updateToMatchGridOption);
      rcData->updateToMatchGridOption=updateSize;
      updateToMatchGrid();  // make sure array is correct dimensions before assigning values
      rcData->updateToMatchGridOption=oldOption;

      // floatDistributedArray::operator= ( floatDistributedArray(cgf) );
      floatDistributedArray::operator= ( (floatDistributedArray&)cgf );
    }
  }
  else
  { // rhs=NULL but LHS!=NULL
    destroy();
  }
  
  return *this;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{operator = float}}
floatMappedGridFunction & floatMappedGridFunction::
operator= ( const float x )
//==================================================================================
// /Description:
//   Set the values of a grid function equal to a scalar.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( grid==NULL )
  {
    printf("floatMappedGridFunction::operator=:ERROR: trying to assign a mappedGridFunction=value but the grid is NULL!\n");
    Overture::abort("error");
  }
  #ifndef USE_PPP
    floatDistributedArray::operator= (x);
  #else
    // do this so we assign ghost boundaries too.
    floatSerialArray uLocal;  ::getLocalArrayWithGhostBoundaries(*this,uLocal);
    uLocal=x;
  #endif
  return *this;
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{operator = floatDistributedArray}}
floatMappedGridFunction & floatMappedGridFunction::
operator= ( const floatDistributedArray & X )
//==================================================================================
// /Description:
//   Set the values of a grid function equal to an A++ array. The operation must
//   be conformable or else an A++ error will be generated.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  if( grid==NULL )
  {
    printf("floatMappedGridFunction::operator=:ERROR: trying to assign a mappedGridFunction=array but the grid is NULL!\n");
    Overture::abort("error");
  }
  if( isNull() )
  {
    floatDistributedArray::operator= (X);
  }
  else
  {
    const int nd=4;
    Index Iv[4];  // null Index --> copy all
    ParallelUtility::copy(*this,Iv,X,Iv,nd); // *wdh* 060505
  }
  
  return *this;
}

#define periodicUpdateOpt EXTERN_C_NAME(periodicupdateopt)

extern "C"
{
void periodicUpdateOpt(const int&nd, 
		  const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,
		  const int&ndu3a,const int&ndu3b,const int&ndu4a,const int&ndu4b,
		  float & u, const int&ca,const int&cb, const int&indexRange, 
		  const int&gridIndexRange, const int&dimension, 
		  const int&isPeriodic );
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{periodicUpdate}}
void floatMappedGridFunction::
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
//  periodic but that only it's derivative is -- like the grid function for the vetrex array on
//  a periodic square.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  real time=getCPU();

  if( grid->gridType==MappedGrid::unstructuredGrid ) 
    {
      GridFunctionParameters::GridFunctionType gftype = getGridFunctionType();
      UnstructuredMapping::EntityTypeEnum ftype;
      switch ( gftype ) {
      case GridFunctionParameters::faceCenteredAll:
      case GridFunctionParameters::faceCentered:
	ftype = UnstructuredMapping::Face;
	break;
      case GridFunctionParameters::edgeCentered:
	ftype = UnstructuredMapping::Edge;
	break;
      case GridFunctionParameters::vertexCentered:
	ftype = UnstructuredMapping::Vertex;
	break;
      case GridFunctionParameters::cellCentered:
	ftype = grid->numberOfDimensions==2 ? UnstructuredMapping::Face : UnstructuredMapping::Region;
	break;
      default:
	cout<<"UNKNOWN GRIDFUNCTION TYPE in periodicUpdate of unstructured grid"<<endl;
	ftype = UnstructuredMapping::Vertex;
      }

      const IntegerArray &perBC = *grid->getUnstructuredPeriodicBC( ftype );

      if ( !perBC.getLength(0) )
	return; 

      int nPer = perBC.getLength(0);

      assert(positionOfComponent(0)==3); // XXX 040322 fix this KYLE

      int *pbcPTR = perBC.Array_Descriptor.Array_View_Pointer2;

      // int *ptr = getDataPointer();
       float *ptr = getDataPointer();
      // float *ptr = getDataPointer();

      int cs = C0!=nullRange ? C0.getBase()   : getBase(3);
      int ce = C0!=nullRange ? C0.getBound()  : getBound(3);
      int cl = C0!=nullRange ? C0.getLength() : getLength(3);
      
      int nl = getLength(0);

      for ( int i=0; i<nPer; i++ )
	{
	  for ( int c=cs; c<=ce; c++ )
	    {
	      ptr[  pbcPTR[i] + c*nl ] = ptr[ pbcPTR[i + nPer] +c*nl ];
	    }
	}
      GenericMappedGridOperators::timeForPeriodicUpdate+=getCPU()-time;
      return;
    }

  
// only use opt version in serial -- need to write an opt parallel periodic update
#ifndef USE_PPP
//  if( positionOfComponent(0)==3 && numberOfComponents()<=1 && !derivativePeriodic )
//  {
//    int ca,cb;
//    if( C0!=nullRange )
//    {
//      ca=C0.getBase(); cb=C0.getBound();
//    }
//    else
//    {
//      ca=getBase(3);  cb=getBound(3);
//    }
//    periodicUpdateOpt(numberOfDimensions(), 
//	        getBase(0),getBound(0),getBase(1),getBound(1),getBase(2),getBound(2),getBase(3),getBound(3),
//		      *(getDataPointer()),ca,cb, grid->indexRange(0,0), grid->gridIndexRange(0,0), 
//		      grid->dimension(0,0), grid->isPeriodic(0) );
//    GenericMappedGridOperators::timeForPeriodicUpdate+=getCPU()-time;
//    return;
//  }
#endif

  // First see if the grid is periodic in any directions, if not, return
  int gridIsPeriodic=FALSE;
  int axis;
  for( axis=axis1; axis<numberOfDimensions(); axis++ )
    gridIsPeriodic=gridIsPeriodic || grid->isPeriodic(axis);
  if( !gridIsPeriodic ) return;

  Range C[5] = { C0,C1,C2,C3,C4 };
  
  const int nd=4;
  Index I[numberOfIndicies], Ip[numberOfIndicies];
  int i,diff[numberOfIndicies];

  Range *Ra = rcData->Ra;  // make a reference to the array in rcData
  
  for( i=0; i<numberOfIndicies; i++ )
  {
    I[i]=Ra[i];
    diff[i]=0;
  }
  for( i=0; i<maximumNumberOfComponents; i++ )
  {
    if( C[i]!=nullRange )
      I[positionOfComponent(i)]=C[i];
  }
  
  // For each axis, assign values on the "left" (Start) and on the "right" (End)
  // Values at the corners will eventually become correct
  //   
  floatMappedGridFunction & u = *this;
  const IntegerArray & indexRange     = grid->indexRange;
  const IntegerArray & gridIndexRange = grid->gridIndexRange;
  const IntegerArray & dimension      = grid->dimension;
  
  int is[4]={0,0,0,0};  //
  
  for( int dir=0; dir<nd; dir++ )
    Ip[dir]=I[dir]; 
  for( axis=0; axis<numberOfDimensions(); axis++)
  {
    i=positionOfCoordinate(axis);   // here is the index position of a coordinate axis
    if( grid->isPeriodic(axis) && 
	Ra[i].getBase() <=grid->dimension(Start,axis) &&   // check these too -- grid function may live on boundary
	Ra[i].getBound()>=grid->dimension(End  ,axis) )
    {
      // length of the period:
      diff[i]=gridIndexRange(End,axis)-gridIndexRange(Start,axis);
      if( !derivativePeriodic )
      {
	// assign all ghost points on "left"
	I[i]=Range(dimension(Start,axis),indexRange(Start,axis)-1);
        // u(I[0],I[1],I[2],I[3])=u(I[0]+diff[0],I[1]+diff[1],I[2]+diff[2],I[3]+diff[3]);
        Ip[i]=I[i]+diff[i];
        ParallelUtility::copy(u,I, u,Ip,nd );  // *wdh* 060503

        // assign all ghost points on "right"
	I[i]=Range(indexRange(End,axis)+1,dimension(End,axis));
	// u(I[0],I[1],I[2],I[3])=u(I[0]-diff[0],I[1]-diff[1],I[2]-diff[2],I[3]-diff[3]);
        Ip[i]=I[i]-diff[i]; 
        ParallelUtility::copy(u,I, u,Ip,nd );  // *wdh* 060503
      }
      else
      {
	// derivative periodic case: (i.e. like a square with periodic BC's )
        //  u(-m) = u(0) + u(N-m)-u(N)  : left side, m=1,2,..,number of ghost lines
	I[i]=indexRange(Start,axis);
        int n;
        for( n=1; n<=indexRange(Start,axis)-dimension(Start,axis); n++ )
	{
	  is[i]=n;
          u(I[0]-is[0],I[1]-is[1],I[2]-is[2],I[3]-is[3])=u(I[0],I[1],I[2],I[3])+
	    u(I[0]+(diff[0]-is[0]),I[1]+(diff[1]-is[1]),I[2]+(diff[2]-is[2]),I[3]+(diff[3]-is[3]))-
	    u(I[0]+ diff[0]       ,I[1]+ diff[1],       I[2]+ diff[2],       I[3]+ diff[3]);
	}
	I[i]=indexRange(End,axis);
        for( n=1; n<=dimension(End,axis)-gridIndexRange(End,axis); n++ )
	{
	  is[i]=n;
          u(I[0]+is[0],I[1]+is[1],I[2]+is[2],I[3]+is[3])=u(I[0],I[1],I[2],I[3])+
	    u(I[0]-(diff[0]-is[0]),I[1]-(diff[1]-is[1]),I[2]-(diff[2]-is[2]),I[3]-(diff[3]-is[3]))-
	    u(I[0]- diff[0]       ,I[1]- diff[1]       ,I[2]- diff[2]       ,I[3]- diff[3]);
	}
	

      }
      
      diff[i]=0; is[i]=0;    // reset values to default
      I[i]=Ra[i];
      Ip[i]=I[i];
    }
  }
  GenericMappedGridOperators::timeForPeriodicUpdate+=getCPU()-time;
}

int floatMappedGridFunction:: 
positionOfCoefficient(const int m1, const int m2, const int m3, const int component) const
{
  return 0;   // **** to do ****
}



//-----------------------------------------------------------------------------------------
// Update the gridFunction and change the dimensions
//
//  Here is the update function that does most of the work
//
// return values are in the form of a mask: 
//   updateNoChange          = 0 : no changes made
//   updateReshaped          = 1 : grid function was reshaped
//   updateResized           = 2 : grid function was resized
//   updateComponentsChanged = 4 : component dimensions may have changed (but grid was not resized or reshaped)
//-----------------------------------------------------------------------------------------
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
privateUpdateToMatchGrid()
{
  int returnValue=0;
  // Redimension the array to match the current grid
  if( grid!=NULL )
  {
    rcData->numberOfDimensions=grid->numberOfDimensions;
    Range *R = rcData->R;  // make a reference to the array in rcData
    Range *Ra= rcData->Ra; // make a reference to the array in rcData

    // first make sure the coordinate Ranges are correct:
// **    for( axis=0; axis<numberOfDimensions(); axis++ )
// **      Ra[positionOfCoordinate(axis)]=Range(grid->dimension(Start,axis),grid->dimension(End,axis));

    // resize the arrays unless the dimensions are the same
    int sameDimensions=TRUE;
    int newSize=1,oldSize=1;
    int i;
    for( i=0; i<numberOfIndicies; i++ )
    {
      newSize*=Ra[i].getBound()-Ra[i].getBase()+1;
      oldSize*=getBound(i)-getBase(i)+1;
      sameDimensions= sameDimensions && 
                      Ra[i].getBase()==getBase(i) && Ra[i].getBound()==getBound(i);
    }

    // *wdh* 060828: In parallel we also check that we have the same parallel distribution
    #ifdef USE_PPP
      if( oldSize>0 && newSize>0 )
      {
        // This next check can fail sometimes if the partition in "this" has changed 
        // but the array has not been updated:
	// bool samePartition= hasSameDistribution(grid->partition,getPartition()); 

        // This next function will compare the distributions AND the processors 
	bool samePartition= hasSameDistribution(*this,grid->partition);

	if( !samePartition )
	{ // *wdh* 061022 -- always redim(0) if the partition has changed
	  oldSize=0;  // force an update below
	  redim(0);
          if( !(rcData->updateToMatchGridOption & updateSize) )
	  {
	    printf("gridFunction::privateUpdateToMatchGrid:ERROR: partition has changed but\n"
                   " not requesting an update of the size! updateToMatchGridOption & updateSize=false\n");
	    Overture::abort("error");
	  }
	}
	sameDimensions = sameDimensions && samePartition;
      }
      
    #endif

    if( !sameDimensions && (rcData->updateToMatchGridOption & updateSize) )
    {
      if( newSize==0 )
      { // empty grid function -- could be for a rectangular grid, sometimes we don't allocate space --
        returnValue=2;
        redim(0);
      }
      else if( newSize!=oldSize )
      {
        returnValue=2;

        // We should not use the grid partition if the grid function lives on a boundary because
        // we should not partition across the direction normal to the boundary ***

        const IntegerArray & dimension = grid->dimension;
        bool fullSize=true;
        for( int axis=0; axis<numberOfDimensions(); axis++ )
	{
          const Range & C = Ra[positionOfCoordinate(axis)];
	  if( C.getBase()!=dimension(Start,axis) || C.getBound()!=dimension(End,axis) )
	  {
            fullSize=false;
	    break;
	  }
	}
        if( fullSize )
	{
	  grid->initializePartition();
          if( positionOfCoordinate(0)==0 )
	  {
	    partition(grid->partition);       // set the partition *** should I do this for the resize too??? *****
	  }
	  else
	  {
            // printf("***** MGF use the matrix partition since first index is not distributed ************* \n");
	    
            partition(grid->matrixPartition);   // *wdh* 050329 
	  }
	  
	}
	else
	{
          // Use default partition *** but what about number of processors???
          if( false )
	  {
	    printf("MGF:privateUpdateToMatchGrid:INFO using default partition for boundary(?) GF with "
		   "dimensions=[%i,%i][%i,%i][%i,%i]\n",
		   Ra[positionOfCoordinate(0)].getBase(), Ra[positionOfCoordinate(0)].getBound(),
		   Ra[positionOfCoordinate(1)].getBase(), Ra[positionOfCoordinate(1)].getBound(),
		   Ra[positionOfCoordinate(2)].getBase(), Ra[positionOfCoordinate(2)].getBound());
	  }
	  
	}

        redim(Ra[0],Ra[1],Ra[2],Ra[3]);   // this breaks references
      }
      else // newSize==oldSize
      {
        returnValue=1;
        #ifndef USE_PPP
          reshape(Ra[0],Ra[1],Ra[2],Ra[3]);  // this does not break references
        #else
          redim(Ra[0],Ra[1],Ra[2],Ra[3]);  // we cannot reshape for P++, do this instead, will break ref's
        #endif
      }
    }
    
    int sameComponentDimensions=isCellCentered.elementCount()>0;   // FALSE if isCellCentered has not 
                                                                   // been dimensioned yet
    for( i=0; i<3; i++ )  // *********************** fix 3->4
    {
      sameComponentDimensions= sameComponentDimensions && 
             R[positionOfComponent(i)].getBase() ==isCellCentered.getBase(i+1) 
          && R[positionOfComponent(i)].getBound()==isCellCentered.getBound(i+1);
    }
    if( !sameComponentDimensions )
    { // NOTE: If the component dimensions change then we set isCellCentered back to default values
      // **** returnValue|=4;
      // ** returnValue=0;  // ****** fix later ****
      isCellCentered.resize(3,R[positionOfComponent(0)],   // ***************************** fix
                              R[positionOfComponent(1)],
    	                      R[positionOfComponent(2)]);
      // now assign default values, take centredness from the grid
      isCellCentered=FALSE;   // by default extra dimensions are vertex centred
      for( int c3=R[positionOfComponent(3)].getBase(); c3<=R[positionOfComponent(3)].getBound(); c3++ )
      for( int c2=R[positionOfComponent(2)].getBase(); c2<=R[positionOfComponent(2)].getBound(); c2++ )
      for( int c1=R[positionOfComponent(1)].getBase(); c1<=R[positionOfComponent(1)].getBound(); c1++ )
      for( int c0=R[positionOfComponent(0)].getBase(); c0<=R[positionOfComponent(0)].getBound(); c0++ )
        for( int axis=0; axis<numberOfDimensions(); axis++ )
          isCellCentered(axis,c0,c1,c2)= grid->isCellCentered(axis)? TRUE : FALSE; 
    }      
    if( positionOfFaceCentering()>-1 ) // a face centered grid function has been defined
    {
      rcData->faceCentering=GridFunctionParameters::all;
      setFaceCentering();                       // each component will be face centred in all directions
    }

    // update sparse rep *** NO ***  only do this with a call to setIsACoefficientMatrix
    // if( (rcData->updateToMatchGridOption & updateCoefficientMatrix) && isACoefficientMatrix() && sparse!=NULL && mappedGrid!=NULL )
    //   sparse->updateToMatchGrid(*mappedGrid);


  }
  else if( elementCount()!=0 )
  {
    cout << "floatMappedGridFunction:updateToMatchGrid:ERROR: grid is NULL but array is not! " << endl;
  }
  return (updateReturnValue &) returnValue;  
}

//==================================================================================
//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{positionOfFaceCentering}}
//\no function header:
// \noindent{\bf const int\& positionOfFaceCentering() const }
//
// /Return value: the index position, (0,1,2,..) of the face centering.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{put}}
int floatMappedGridFunction::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   Output a grid function onto a database file
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
//     \item[numberOfComponents] (int) The number of component indices in the grid function: $0$ for a scalar
//         $1$ for a vector, $2$ for a matrix etc. Currently there can be at most $5$ components.     
//       The default value and the value for unused entries is $N$={\tt maximum\-Number\-Of\-Indicies}.
//     \item[positionOfCoordinate] (IntegerArray(N)) {\tt position\-Of\-Coordinate(i)} holds the index in the
//       array (numbered starting from 0) of the 3 coordinate positions, $i=0,1,2$.
//       The default value and the value for unused entries is $N$={\tt maximum\-Number\-Of\-Indicies}.
//     \item[positionOfComponent] (IntegerArray(N)) {\tt position\-Of\-Component(i)} holds the index in the
//       array (numbered starting from 0) of the component positions, $i=0,1,...,N-1$.
//       The default value and the value for unused entries is $N$={\tt maximum\-Number\-Of\-Indicies}.
//
//      \noindent Examples:
//      \begin{verbatim}
//   MappedGrid mg(...); 
//   Range all;
//   realMappedGridFunction u(mg,all,all,all); 
//   --> numberOfComponents=0
//   --> positionOfCoordinate(0)=0, positionOfCoordinate(1)=1, positionOfCoordinate(2)=2
//   realMappedGridFunction u(mg,2,all,all,all); 
//   --> numberOfComponents=1
//   --> positionOfCoordinate(0)=1, positionOfCoordinate(1)=2, positionOfCoordinate(2)=3
//   --> positionOfComponent(0)=0
//   realMappedGridFunction u(mg,all,2,all,3,all,4);
//   --> numberOfComponents=3
//   --> positionOfCoordinate(0)=0, positionOfCoordinate(1)=2, positionOfCoordinate(2)=4
//   --> positionOfComponent(0)=1,  positionOfComponent(2)=3,  positionOfComponent(3)=5
//      \end{verbatim}
//     \item[positionOfFaceCentering] (int) For a face centred grid function of standard type
//        this is the index position of the face centering. For all other types of grid functions this
//        has a value of $-1$.
//     \item[faceCentering] (enum faceCenteringType) The face centering type for the grid function.
//          Default value is {\tt none}$=-1$.
//     \item[numberOfDimensions] The number of space dimensions, $1,2$, or $3$. 
//     \item[isACoefficientMatrix] (bool) If TRUE (=1) then this is a coefficient matrix, 
//         default is FALSE (=$0$).
//     \item[stencilType] (enum StencilTypes) The type of stencil for a coefficient matrix, 
//       default is {\tt standardStencil} (=$0$).
//  %   \item[stencilOffset] (int) The stencil offset for a coefficient matrix, default value = $0$.
//     \item[stencilWidth] (int) The stencil width for a coefficient matrix, default value = $0$.
//     \item[{R[i].base}] (int) ({\tt i=0,1,...,N}) The base of the Range object {\tt R[i]} which
//       holds the base and bound for index position $i$. For unused positions the default is 0.
//       There is one extra Range, {\tt R[N]=Range(0,0)}  
//       which exists just for convenience.
//     \item[{R[i].bound}] (int) ({\tt i=0,1,...,N}) The bound of the Range objects {\tt R[i]}.
//        For unused index positions the default is 0.
//     \item[{Ra[i].base}] (int) ({\tt i=0,1,...,$N_A$-1}) The base of the Range objects {\tt Ra[i]} which
//       holds the actual base and bound for index position $i$ of the A++ array (from which the grid function
//       is derived). Currently A++ arrays have only 4 dimensions so we compress the final 5 dimensions
//       of a grid function to be stored in the last A++ dimension. For unused positions the default is 0.
//     \item[{Ra[i].bound}] (int) ({\tt i=0,1,...,$N_A$-1}) The bound of the Range object {\tt Ra[i]}.
//        For unused positions the default is 0.
//     \item[{Rc[i].base}] (int) ({\tt i=0,1,2}) The base of the Range objects {\tt Rc[i]} which
//       hold special information about the base and bound for the coordinate directions. 
//       These are required for grid functions that only live on boundaries.
//       The default value is $0$.
//     \item[{Rc[i].bound}] (int) ({\tt i=0,1,2}) The bound of the Range objects {\tt Rc[i]}.
//        The default value is $-1$.
//     \item[numberOfNames] (int) The number of names that are saved. (see next item).
//     \item[{name[i]}] (aString) ({\tt i=0,1,...,{\tt numberOfNames-1}}) The names for the
//        grid function and its components.
//     \item[isCellCentered] (IntegerArray(3,$C_0$,$C_1$,$C_2$)) The cell centeredness (0/1) in
//       each coordinate direction for each component. Currently we only save the info
//       for 3 components (when A++ is fixed for 8 dimensions we will save the info for 5 components).
//       For a vertex centered grid the default values are all $0$.
//       In terms of the other variables described here the isCellCentered array has dimensions:
//       \begin{verbatim} 
//           isCellCentered.redim(3,R[positionOfComponent(0)],  
//                                  R[positionOfComponent(1)],
//                                  R[positionOfComponent(2)]);
//       \end{verbatim}
//       and thus {\tt $C_i$=R[positionOfComponent(i)]}.
//     \item[arrayData] (A++ array) This is the A++ array that holds the actual array-data 
//      for this grid function. 
//   \end{description} 
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"floatMappedGridFunction");                      // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  int i;
  subDir.put( rcData->numberOfComponents,"numberOfComponents" );
  subDir.put( rcData->positionOfCoordinate,"positionOfCoordinate",maximumNumberOfIndicies );  
  subDir.put( rcData->positionOfComponent,"positionOfComponent",maximumNumberOfIndicies ); 
  subDir.put( rcData->positionOfFaceCentering,"positionOfFaceCentering" ); 
  subDir.put( rcData->faceCentering,"faceCentering" );   
  subDir.put( rcData->numberOfDimensions,"numberOfDimensions" );   

  subDir.put( rcData->isACoefficientMatrix,"isACoefficientMatrix" ); 
  subDir.put( rcData->stencilType,"stencilType" ); 
  // subDir.put( rcData->stencilOffset,"stencilOffset" ); 
  subDir.put( rcData->stencilWidth,"stencilWidth" ); 
  char buff[40];
  for( i=0; i<maximumNumberOfIndicies+1; i++ )
  {
    subDir.put( rcData->R[i].getBase(), sPrintF(buff,"R[%i].base",i) );
    subDir.put( rcData->R[i].getBound(),sPrintF(buff,"R[%i].bound",i) );
  }
  for( i=0; i<numberOfIndicies; i++ )
  {
    subDir.put( rcData->Ra[i].getBase(), sPrintF(buff,"Ra[%i].base",i) );
    subDir.put( rcData->Ra[i].getBound(),sPrintF(buff,"Ra[%i].bound",i) );
  }
  for( i=0; i<3; i++ )
  {
    subDir.put( rcData->Rc[i].getBase(), sPrintF(buff,"Rc[%i].base",i) );
    subDir.put( rcData->Rc[i].getBound(),sPrintF(buff,"Rc[%i].bound",i) );
  }
  subDir.put( rcData->numberOfNames,"numberOfNames" );
  for( i=0; i<rcData->numberOfNames; i++ )
    subDir.put( rcData->name[i],sPrintF(buff,"name[%i]",i) );

  subDir.put( isCellCentered,"isCellCentered" ); 
  subDir.putDistributed( *this,"arrayData" );  // put the A++ array

  delete &subDir;
  return 0;
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{reference}}
void floatMappedGridFunction::
reference(const floatMappedGridFunction & cgf)
//-----------------------------------------------------------------------------------------
// /Description:
//   Use this function to reference one floatMappedGridFunction to another.
//   When two (or more) grid functions have been
//   referenced they share the same array data so that changes to one grid function
//   will change all the other referenced grid functions. 
//   Only the array data is referenced. Other properties of the grid function such
//   as cell-centredness can be changed in the referenced grid function. The "shape"
//   of the referenced grid function can also be changed without changing 
//   the referencee:{\ff cgf}.  
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( this==&cgf ) // no need to do anything if
    return;

  assert(cgf.rcData!=NULL);
  if( rcData!=cgf.rcData )
    *rcData              =*cgf.rcData;  // deep copy
  grid                   =cgf.grid;        // note: this only copies the pointer to the grid ****
  mappedGrid             =cgf.mappedGrid; 
  operators              =cgf.operators;

  // reference array objects in the envelope to the letter
  // positionOfComponent.reference(rcData->positionOfComponent);
  // positionOfCoordinate.reference(rcData->positionOfCoordinate);
  // stencilOffset.reference(rcData->stencilOffset);
  
  // *** why is this not in rcData ??? *****
  isCellCentered.redim(cgf.isCellCentered);            // get correct dimensions, base/bound
  isCellCentered=cgf.isCellCentered;
  
  // Reference sparse 
  if( sparse && sparse->decrementReferenceCount()==0 )
    delete sparse;
  sparse = cgf.sparse;
  if( sparse )
    sparse->incrementReferenceCount();

  // *** Only the array data is referenced ****
  floatDistributedArray::reference( cgf );
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{Standard argument function, sa}}
float & floatMappedGridFunction::
sa(const int & i0, 
   const int & i1, 
   const int & i2, 
   const int & c0, /* =0 */
   const int & c1, /* =0 */
   const int & c2, /* =0 */ 
   const int & c3, /* =0 */
   const int & c4  /* =0 */ ) const
//==================================================================================
// /Description:
// The sa, "standard argument" function permutes the arguments to that you can always
// refer to a function as u(coordinate(0),coordinate(1),corrdinate(2),component(0),component(1),...)
//
// /i0, i1, i2 (input): index values for the three coordinates
// /c0, c1,... (input): index values for the components
//
// /Return Values:
//    The value of the grid function.
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  int i[8];
  i[positionOfCoordinate(0)]=i0;
  i[positionOfCoordinate(1)]=i1;
  i[positionOfCoordinate(2)]=i2;
  i[positionOfComponent(0) ]=c0;
  i[positionOfComponent(1) ]=c1;
  i[positionOfComponent(2) ]=c2;
  i[positionOfComponent(3) ]=c3;
  i[positionOfComponent(4) ]=c4;
  return (*this)(i[0],i[1],i[2],i[3],i[4],i[5],i[6],i[7]);
}

 
// =================== Boundary condition member functions ========================

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setFaceCentering}}
void floatMappedGridFunction:: 
setFaceCentering( const int & axis /* =defaultValue */ )
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
//\end{MappedGridFunctionInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  if( axis!=defaultValue )
    rcData->faceCentering=axis;
    
  IntegerArray c(maximumNumberOfComponents);
  if( getFaceCentering()==GridFunctionParameters::none )
  {
    cout << "floatMappedGridFunction::setFaceCentering:ERROR cannot face center this grid function\n"
         << " since getFaceCentering()==none \n";
    Overture::abort("floatMappedGridFunction::setFaceCentering:ERROR");
  }
  else if( getFaceCentering()==GridFunctionParameters::all )
  {
    // Make components face centred in all directions, 
    int faceCentre=-1;   // which component is the face centred one
    for( int component=0; component<maximumNumberOfComponents; component++ )
      if( positionOfFaceCentering()==positionOfComponent(component) )
	faceCentre=component;
    if( faceCentre== -1 )
    {
      cout << "floatMappedGridFunction::setFaceCentering::ERROR: unable to find the component for the `face'\n";
      cout << "Did you remember to first construct or update the grid function using `faceRange'? \n";
      Overture::abort("floatMappedGridFunction::setFaceCentering::ERROR");
    }
    Range *R = rcData->R;
    for( int c4=R[positionOfComponent(4)].getBase(); c4<=R[positionOfComponent(4)].getBound(); c4++ )
    for( int c3=R[positionOfComponent(3)].getBase(); c3<=R[positionOfComponent(3)].getBound(); c3++ )
    for( int c2=R[positionOfComponent(2)].getBase(); c2<=R[positionOfComponent(2)].getBound(); c2++ )
    for( int c1=R[positionOfComponent(1)].getBase(); c1<=R[positionOfComponent(1)].getBound(); c1++ )
    for( int c0=R[positionOfComponent(0)].getBase(); c0<=R[positionOfComponent(0)].getBound(); c0++ )
    for( int dir=0; dir<grid->numberOfDimensions; dir++ )
    {
      c(0)=c0; c(1)=c1; c(2)=c2; c(3)=c3; c(4)=c4;
      c(faceCentre)=dir;
      setIsFaceCentered(dir,  Range(c(0),c(0)),Range(c(1),c(1)),Range(c(2),c(2)),
                              Range(c(3),c(3)),Range(c(4),c(4)) );  // one of c(i) will always equal dir
    }
  }
  else
  { // make all components face centred along the given axis
    setIsFaceCentered( axis );
  }
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setIsACoefficientMatrix}}
void floatMappedGridFunction:: 
setIsACoefficientMatrix(const bool trueOrFalse, /* =TRUE */
			const int stencilSize0, /* = defaultValue */
			const int numberOfGhostLines, /* =1 */
			const int numberOfComponentsForCoefficients, /* =1 */
                        const int offset /* =0 */ )
//---------------------------------------------------------------------------------------------
// /Description:
//   Indicate whether a grid function holds a coefficient matrix. ALso use this routine
//   to update the sparse matrix representation when the grid has changed. (Call this routine
//   AFTER calling updateToMatchGrid)
// /trueOrFalse (input): TRUE means this grid function is a coefficient matrix
// /stencilSize0 (input): This is the stencil size for the coefficient matrix. By default
//    the stencil size is $3$ in 1D, $9$ in 2D and $27$ in 3D.
// /numberOfGhostLines (input): indicates the number of ghost-lines on which there will
//    equations defined in the coefficient matrix.
// /numberOfComponentsForCoefficients (input): This is the dimension of the system of equations that
//   is represented in the matrix. 
// /offset (input): This is an offset to use when numbering the equations. This value would be
//   used when the floatMappedGridFunction is really part of a CompositeGRidFunction. 
// 
//  /Author: WDH
//\end{MappedGridFunctionInclude.tex} 
//---------------------------------------------------------------------------------------------
{
  rcData->isACoefficientMatrix=(int)trueOrFalse;
  if( isACoefficientMatrix() )
  {
    // cout << ">>>>>>setIsACoeffMatrix: create a sparseRep \n";
    if( !sparse )
    {
      sparse = new SparseRepForMGF();
      assert(sparse!=NULL);
      sparse->incrementReferenceCount();
    }
    int stencilSize = stencilSize0==defaultValue ? int( pow(3,numberOfDimensions()) ) : stencilSize0 ;
    if( mappedGrid!=NULL )
      sparse->updateToMatchGrid(*mappedGrid,stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients,
                              offset); 
    else
      sparse->setParameters(stencilSize,numberOfGhostLines,numberOfComponentsForCoefficients,offset); 
  }
  else
  {
    if( sparse && sparse->decrementReferenceCount()==0 )
      delete sparse;
    sparse = NULL;
  }
}

  // use for setting equation numbers for coefficient grid functions:
int floatMappedGridFunction::
setCoefficientIndex(const int  & m, 
		    const int & na, const Index & I1a, const Index & I2a, const Index & I3a,
		    const int & nb, const Index & I1b, const Index & I2b, const Index & I3b)
{
  int returnValue;
  if( isACoefficientMatrix() )
  {
    assert(sparse!=NULL);
    returnValue =sparse->setCoefficientIndex(m, na,I1a,I2a,I3a,  nb,I1b,I2b,I3b );
  }
  return returnValue;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setIsACoefficientMatrix}}
void floatMappedGridFunction:: 
setIsACoefficientMatrix(SparseRepForMGF *sparseRep)
// ========================================================================================================
// Set the current sparse Representation. This is normally only used internally and by the gridCollectionFunction
// so that it can reference the multigrid and refinement level lists properly.
//\end{MappedGridFunctionInclude.tex} 
// ========================================================================================================
{
  rcData->isACoefficientMatrix=sparseRep!=0;

  if( sparse && sparse->decrementReferenceCount()==0 )
    delete sparse;
  sparse = 0;

  if( isACoefficientMatrix() )
  {
    sparse = sparseRep;
    sparse->incrementReferenceCount();
  }
}



//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setIsCellCentered}}
void floatMappedGridFunction::
setIsCellCentered(const bool trueOrFalse, 
                  const Index & axis0,       /* =nullIndex */
                  const Index & component0,  /* =nullIndex */    
                  const Index & component1,  /* =nullIndex */
                  const Index & component2,  /* =nullIndex */
                  const Index & component3,  /* =nullIndex */
                  const Index & component4   /* =nullIndex */ ) 
//==================================================================================
// /Description:
//   Change the cell centered-ness of the grid function. By default set
//   all components.
// /trueOfFalse (input): make cell-centred or not.
// /axis0: set the value for this axis, by default set all axes.
// /component0, component1, (input): set the value for these components, by default
//   set all components.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;  // make a reference to the array in rcData

  Range Rc[5]= {component0,component1,component2,component3,component4};
  for( int i=0; i<5; i++ )
  {
    if( Rc[i].length()==0 )
      Rc[i]=R[positionOfComponent(i)];
  }
  if( Rc[3].length()!=1 || Rc[4].length()!=1 )
    cout << "floatMappedGridFunction::setIsCellCentered:ERROR: cannot handle more than 3 components\n";
  Index Axes = (axis0.length()==0) ? Index(0,numberOfDimensions()) : axis0;
  isCellCentered(Axes,Rc[0],Rc[1],Rc[2])=trueOrFalse;

  // reset face centering, if any  
  rcData->positionOfFaceCentering=-1;
  rcData->faceCentering=GridFunctionParameters::none;
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setIsFaceCentered}}
void floatMappedGridFunction::
setIsFaceCentered(const int   & axis0,       /* =forAll */
                  const Index & component0,  /* =nullIndex */
                  const Index & component1,  /* =nullIndex */
                  const Index & component2,  /* =nullIndex */
                  const Index & component3,  /* =nullIndex */
                  const Index & component4   /* =nullIndex */ ) 
//======================================================================================================
// /Description:
//   Make a component of a grid function face centred along the given axis. 
//  A face centered grid function along axis0 is vertex centered along axis0 and cell centered along the
//  other axes.
//   
// /axis0: set the value for this axis, by default set all axes. 
// /component0, component1, (input): set the value for these components, by default
//   set all components.
//\end{MappedGridFunctionInclude.tex} 
//======================================================================================================
{
  Range *R = rcData->R;  // make a reference to the array in rcData
  Range Rc[5]= {component0,component1,component2,component3,component4};
  for( int i=0; i<5; i++ )
  {
    if( Rc[i].length()==0 )
      Rc[i]=R[positionOfComponent(i)];
  }
  if( Rc[3].length()!=1 || Rc[4].length()!=1 )
    cout << "floatMappedGridFunction::setIsFaceCentered:ERROR: cannot handle more than 3 components\n";

  for( int c4=Rc[4].getBase(); c4<=Rc[4].getBound(); c4++ )
    for( int c3=Rc[3].getBase(); c3<=Rc[3].getBound(); c3++ )
      for( int c2=Rc[2].getBase(); c2<=Rc[2].getBound(); c2++ )
	for( int c1=Rc[1].getBase(); c1<=Rc[1].getBound(); c1++ )
	  for( int c0=Rc[0].getBase(); c0<=Rc[0].getBound(); c0++ )
	    for( int axis=axis1; axis<numberOfDimensions(); axis++ )
	      if( axis0==forAll )
		isCellCentered(axis,c0,c1,c2)= axis==c0 ? FALSE : TRUE;     // ******************** c3
	      else
		isCellCentered(axis,c0,c1,c2)= axis==axis0 ? FALSE : TRUE;    // ******************** c3

}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setName}}
void floatMappedGridFunction::
setName(const aString & name, 
        const int & component0,  /* =defaultValue */
        const int & component1,  /* =defaultValue */
        const int & component2,  /* =defaultValue */
        const int & component3,  /* =defaultValue */
        const int & component4   /* =defaultValue */ )
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
//\end{MappedGridFunctionInclude.tex} 
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
    for( int i=0; i<maximumNumberOfComponents; i++ )
    {
      if( c[i]==defaultValue )
        c[i]=R[positionOfComponent(i)].getBase();
      else if(c[i] < R[positionOfComponent(i)].getBase() ||
 	      c[i] > R[positionOfComponent(i)].getBound() )
      {
	printf("floatMappedGridFunction::setName:ERROR component%i=%i is invalid ! \n",i,c[i]);
	printf(" It should be in the range (%i,%i) \n",R[positionOfComponent(i)].getBase(),
	       R[positionOfComponent(i)].getBound());
	return;
      }
      
    }
    rcData->GFNAME(c[0],c[1],c[2],c[3],c[4])=name;
  }
}

#undef GFNAME


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setOperators}}
void floatMappedGridFunction::
setOperators(GenericMappedGridOperators & operators0 )
//==================================================================================
// /Description:
//   Supply a derivative object to use for computing derivatives
//   on all component grids. This operator is used for the member functions
//   .x .y .z .xx .xy etc.
// /operators0: use these operators.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  operators=&operators0;
}

void floatMappedGridFunction::
setNumberOfDimensions(const int & number) 
{ 
  rcData->numberOfDimensions=number; 
} 
void floatMappedGridFunction::
setNumberOfComponents(const int & number)  
{ 
  rcData->numberOfComponents=number;
} 

void floatMappedGridFunction::
setPositionOfFaceCentering( const int & position ) 
{ 
  rcData->positionOfFaceCentering=position;
} 

void floatMappedGridFunction:: 
setStencilType(const stencilTypes stencilType0, const int stencilWidth0)
{
/* ---
  rcData->stencilType = int(stencilType0);
  if( stencilWidth0!=defaultValue )
    rcData->stencilWidth=stencilWidth0;
  
  if( stencilType0==standardStencil )
  {
    stencilOffset.redim(3,pow(stencilWidth(),numberOfDimensions()));
    int width = stencilWidth()/2;   // half width for stencil
    int width3 = numberOfDimensions()==2 ? 0 : width;
    int m=0;    
    for( int w3=-width3; w3<=width3; w3++)
    for( int w2=-width; w2<=width; w2++)
    for( int w1=-width; w1<=width; w1++)
    {
      stencilOffset(0,m)= w1;
      stencilOffset(1,m)= w2;
      stencilOffset(2,m)= w3;
      m++;
    }
  }
  else if( stencilType0==starStencil )
  {
    stencilOffset.redim(3,(stencilWidth()-1)*numberOfDimensions()+1);
    int width = stencilWidth()/2;   // half width for stencil
    int m=0;    
    for( int axis=0; axis<numberOfDimensions(); axis++ )
    {
      for( int w=-width; w<=width; w++)
      {
        stencilOffset(0,m)= axis==0 ? w : 0; 
        stencilOffset(1,m)= axis==1 ? w : 0;
        stencilOffset(2,m)= axis==2 ? w : 0;
        m++;
      }
    }
  }
--- */
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{setUpdateToMatchGridOption}}
void floatMappedGridFunction::
setUpdateToMatchGridOption( const UpdateToMatchGridOption & updateToMatchGridOption )
//==================================================================================
// /Description:
// Specify what should be updated when calls are made to updateToMatchGrid
// /updateToMatchGridOption (input): A combination (using the | operation) 
//  of the following options:
// \begin{verbatim}
//  enum UpdateToMatchGridOption
//  {
//    updateSize=1,
//    updateCoefficientMatrix=2
//  };
// \end{verbatim}
//  The default is {\tt updateToMatchGridOption= updateSize | updateCoefficientMatrix}.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  assert( rcData!=NULL );
  rcData->updateToMatchGridOption=updateToMatchGridOption;
}


//-----------------------------------------------------------------------------------------
// Assign the array R of Ranges
//
// Notes:
//   o the first three nullRange's become the positions of the coordinates
//-----------------------------------------------------------------------------------------
void floatMappedGridFunction::
updateRanges(const Range & R0,             // update the R[] array
	     const Range & R1,
	     const Range & R2,
	     const Range & R3,
	     const Range & R4,
	     const Range & R5,
	     const Range & R6,
	     const Range & R7,
             const GridFunctionParameters::GridFunctionType & type /* =defaultCentering */ )
{
  if( rcData==NULL || grid==NULL )
  {
    cout << "floatMappedGridFunction:ERROR: there is no grid associated with this grid function!\n";
    Overture::abort("error");
  }
  Range *R = rcData->R;  // make a reference to the array in rcData
  Range *Ra= rcData->Ra; // make a reference to the array in rcData
  R[0]=R0; 
  R[1]=R1; 
  R[2]=R2; 
  R[3]=R3; 
  R[4]=R4; 
  R[5]=R5; 
  R[6]=R6; 
  R[7]=R7; 
  R[maximumNumberOfIndicies]=Range(0,0); // here is an extra one that we sometimes use


  //  Assign the Ranges:
  //   o the first three nullRange's become the positions of the coordinates
  //   o any Range equal to faceRange becomes the positionOfFaceCentering for a 
  //     face centred grid function
  //   o if a Range has a base which is a large negative number, this indicates that
  //     the grid function is to be defined on one edge of the grid

  rcData->positionOfFaceCentering=-1;
  int numComponents=0;        // count the number of components specified explicitly
  int i,axis,component;
  // 050610 kkc NOTE TO KYLE : "grid" is actually a MappedGridData, not a MappedGrid
  assert( grid!=NULL );
  // const int numberOfCoordinateDirections=grid->gridType==MappedGrid::structuredGrid ? 3 : 1;
  const int numberOfCoordinateDirections=3; // *wdh* 020515 : go back to keep 3 coordinates for unstructured grids
  
  const bool isStructured = grid->gridType==MappedGrid::structuredGrid;
  
  for( i=0, axis=0, component=0; i<maximumNumberOfIndicies; i++ )
  {
    if( R[i].getBase()==faceRange.getBase() && R[i].getBound()==faceRange.getBound() )
    {
      R[i]=Range(0,grid->numberOfDimensions-1);
      rcData->positionOfFaceCentering=i;  // faceCentering is set in privateUpdateToMatchGrid
      rcData->positionOfComponent[component]=i;   // note that this does not add to numberOfComponents
      component++;
    }
    else if( R[i].length()==0 || R[i].getBase() < bigNegativeNumber )
    {
      if( axis<numberOfCoordinateDirections )
      {
        rcData->Rc[axis]=R[i];  
        if( R[i].getBase() > bigNegativeNumber )
	{
          if( isStructured )//kkc|| axis>0 )
	  {
	    R[i]=Range(grid->dimension(Start,axis),grid->dimension(End,axis));
	  }
	  else if ( axis>0 )
	  { // do this since dimension does not seem to work right now for uns grids (?)
	    R[i]=1;
	  }
	  else
	  {
            // for an unstructured grid the number of entries depends on the centering
            const Mapping & map = grid->mapping.getMapping();
            assert( map.getClassName()=="UnstructuredMapping" );
            const UnstructuredMapping & uns = (const UnstructuredMapping&)map;
	    
	    //            if( (type==GridFunctionParameters::defaultCentering || type==GridFunctionParameters::vertexCentered) && grid->isAllVertexCentered )
	    if( type==GridFunctionParameters::vertexCentered || ( type==GridFunctionParameters::defaultCentering && grid->isAllVertexCentered ))
	    {
	      //              assert( grid->dimension(Start,axis)==0 && grid->dimension(End,axis)==uns.getNumberOfNodes()-1 );
	      // where does dimension get set exactly???  assert( grid->dimension(Start,axis)==0 && grid->dimension(End,axis)==uns.size(UnstructuredMapping::Vertex)-1 );
	      R[i]=uns.size(UnstructuredMapping::Vertex);
	    }
	    //	    else if( (type==GridFunctionParameters::defaultCentering || type==GridFunctionParameters::cellCentered ) && grid->isAllCellCentered)
	    else if( type==GridFunctionParameters::cellCentered || ( type==GridFunctionParameters::defaultCentering&& grid->isAllCellCentered) )
	    {
              //R[i]=uns.getNumberOfElements();
 	      if ( grid->numberOfDimensions==3 )
 		R[i]=uns.size(UnstructuredMapping::Region);
 	      else
 		R[i]=uns.size(UnstructuredMapping::Face);

	    }
	    else if( type==GridFunctionParameters::faceCentered || type==GridFunctionParameters::faceCenteredAll )
	    {
	      //              R[i]=uns.getNumberOfFaces();
	      if ( grid->numberOfDimensions==3 )
 		R[i]=uns.size(UnstructuredMapping::Face);
 	      else
 		R[i]=uns.size(UnstructuredMapping::Edge);

	    }
	    else if( type==GridFunctionParameters::edgeCentered )
	    {
	      //              R[i]=uns.getNumberOfEdges();
	      R[i]=uns.size(UnstructuredMapping::Edge);
	    }
	    else
	    {
	      printf("floatMappedGridFunction::updateRanges::ERROR: unknown grid function type for an unstructured grid\n");
	      Overture::abort("error");
	    }
	  }
	}
        else if( R[i].getBase() > biggerNegativeNumber )
          R[i]=Range(grid->gridIndexRange(End,axis)+R[i].getBase() -endingGridIndex,
                     grid->gridIndexRange(End,axis)+R[i].getBound()-endingGridIndex);
        else
          R[i]=Range(grid->gridIndexRange(Start,axis)+R[i].getBase() -startingGridIndex,
                     grid->gridIndexRange(Start,axis)+R[i].getBound()-startingGridIndex);

        rcData->positionOfCoordinate[axis]=i;
	axis++;
      }
      else
      {
	rcData->positionOfComponent[component]=i;
        component++;
        R[i]=Range(0,0);                   // unused Ranges default to (base,bound)=(0,0)
      }
    }
    else
    {
      numComponents++;
      // positionOfComponent(component++)=i;
      rcData->positionOfComponent[component]=i;
      component++;
    }
  }
  setNumberOfComponents(numComponents);
  
  if( axis!=numberOfCoordinateDirections )      // there must be room for 3 coordinates!
  {
    cout << "floatMappedGridFunction::ERROR:When you declare a grid function there must be room for " <<
      numberOfCoordinateDirections << "coordinates!\n";
    cout << " Perhaps you have tried to declare too many components\n";
    Overture::abort("floatMappedGridFunction::ERROR");
  }
  // assert(axis>=grid->numberOfDimensions());  // there must be room for all coordinates!

  // assign default positions for unused components
  for( i=component; i<maximumNumberOfIndicies; i++ )
    rcData->positionOfComponent[i]=maximumNumberOfIndicies;


// assign default positions for unused coordinates
  for( i=axis; i<maximumNumberOfIndicies; i++ )
    rcData->positionOfCoordinate[i]=maximumNumberOfIndicies;
  
  // ===== assign the actual Ranges =====
  for( i=0; i<numberOfIndicies; i++ )
    Ra[i]=R[i];                            // actual Ranges for the under-lying A++ array

  // for now: merge extra components together   **** remove this when A++ is fixed *******
  if( Ra[3].length() > 0 && positionOfCoordinate(axis-1)<numberOfCoordinateDirections )
  { 
    if( R[4].length() > 1  )
      Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[4].length()-1);
    if( R[5].length() > 1  )
      Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[5].length()-1);
    if( R[6].length() > 1  )
      Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[6].length()-1);
    if( R[7].length() > 1  )
      Ra[3]=Range(Ra[3].getBase(),Ra[3].getBase()+Ra[3].length()*R[7].length()-1);
  }
  else 
  {
    if( R[4].length() > 1 || R[5].length() > 1 || R[6].length() > 1 || R[7].length() > 1 )
    {
      cout << "floatMappedGridFunction::updateRanges:ERROR invalid component Ranges!\n";
      cout << "I am unable to merge these components\n";
      Overture::abort("floatMappedGridFunction::updateRanges:ERROR");
    }
  }
  
}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{updateToMatchGrid}}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid()
//==================================================================================
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  Range Ru[maximumNumberOfIndicies]; 
  int i;
  for( i=0; i<maximumNumberOfIndicies; i++ )
    Ru[i]=nullRange;

   // set coordinate ranges from Rc : either nullRange or special boundary range.
  const int numberOfCoordinateDirections=grid->gridType==MappedGrid::structuredGrid ? 3 : 1;
  for( i=0; i<numberOfCoordinateDirections; i++ ) 
    Ru[positionOfCoordinate(i)]=rcData->Rc[i];  

  // only set component ranges for the number of components, these keeps the
  // number of components the same.
  int number = numberOfComponents() + (positionOfFaceCentering()>=0 ? 1 : 0);  // add one if faceCenterAll
  for( i=0; i<number; i++ ) 
    Ru[positionOfComponent(i)]=rcData->R[positionOfComponent(i)];   
  
  if( positionOfFaceCentering()>=0 )
    Ru[positionOfFaceCentering()]=faceRange;
  
  return updateToMatchGrid( Ru[0],Ru[1],Ru[2],Ru[3],Ru[4],Ru[5],Ru[6],Ru[7] );
}


//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//=====================================================================================================
{
  updateRanges( R0,R1,R2,R3,R4,R5,R6,R7 );
  return privateUpdateToMatchGrid();
}


//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGrid & grid0,
		  const Range & R0, 
		  const Range & R1,  /* = nullRange */
		  const Range & R2,  /* = nullRange */
		  const Range & R3,  /* = nullRange */
		  const Range & R4,  /* = nullRange */
		  const Range & R5,  /* = nullRange */
		  const Range & R6,  /* = nullRange */
		  const Range & R7   /* = nullRange */ )
//=====================================================================================================
// /Description:
//     Update a grid function. Optionally specify a new grid and new Ranges.
// /grid0 (input): update to match this grid.
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
//\end{MappedGridFunctionInclude.tex} 
//======================================================================================================
{
  mappedGrid=&grid0;
  return updateToMatchGrid( *(grid0.rcData),R0,R1,R2,R3,R4,R5,R6,R7 );
}

floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGrid & grid0,
		  const int & i0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
//=====================================================================================================
// same as above function but takes an int as 2nd argument, this is to prevent overloading conflicts
//=====================================================================================================
{
  mappedGrid=&grid0;
  return updateToMatchGrid( *(grid0.rcData),Range(0,i0-1),R1,R2,R3,R4,R5,R6,R7 );
}


//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGrid & grid0, 
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  // initialize();   *** this causes a leak, 971130 *** is this needed?
  mappedGrid = &grid0;
  grid=grid0.rcData;
  return updateToMatchGrid(type,component0,component1,component2,component3,component4);
}

//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGridData & grid0, 
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  // initialize();   *** this causes a leak, 971130 *** is this needed?
  grid=&grid0;
  return updateToMatchGrid(type,component0,component1,component2,component3,component4);
}


//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  updateReturnValue returnValue;
  const Range &fr =  grid->gridType==MappedGrid::structuredGrid ? faceRange : nullRange; // kkc 040325

  switch (type)
  {
  case GridFunctionParameters::defaultCentering:
  case GridFunctionParameters::general:
  case GridFunctionParameters::vertexCentered:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::cellCentered:
  case GridFunctionParameters::faceCentered: // unstructured grid
  case GridFunctionParameters::edgeCentered: // unstructured grid
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4,type);
    break;
  case GridFunctionParameters::faceCenteredAll:
    switch (countNumberOfNonNullRanges(component0,component1,component2,component3,component4))
      { // kkc 031229 added type to arguments of updateRanges to get unstructured grids to work (type was only used for uns)
    case 0:
      updateRanges( nullRange,nullRange,nullRange,fr,nullRange,nullRange,nullRange,nullRange,type); break;
    case 1:
      updateRanges( nullRange,nullRange,nullRange,component0,fr,nullRange,nullRange,nullRange,type); break;
    case 2:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,fr,nullRange,nullRange,type); break;
    case 3:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,fr,nullRange,type); break;
    case 4:
      updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,fr,type); break;
    default:
      cout << "floatMappedGridFunction::constructor:ERROR too many components, only 4 allowed \n";
      Overture::abort("floatMappedGridFunction::constructor:ERROR too many components, only 4 allowed" );
    }
    break;
  case GridFunctionParameters::faceCenteredAxis1:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::faceCenteredAxis2:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  case GridFunctionParameters::faceCenteredAxis3:
    updateRanges( nullRange,nullRange,nullRange,component0,component1,component2,component3,component4);
    break;
  default:
    cout << "floatMappedGridFunction::(updateToMatchGrid):ERROR unknown GridFunctionType!! This error should not occur\n";
    Overture::abort("error");
  }
  returnValue =privateUpdateToMatchGrid();
  // now set the cell/face centering beacuse the isCellCenteredArray is now defined
  switch (type)
  {
  case GridFunctionParameters::defaultCentering:
    // defaultCentering == cellCentered if the grid is cell centered
    if( (bool&)grid->isAllCellCentered )
      setIsCellCentered(TRUE);
    break;
  case GridFunctionParameters::cellCentered:
    setIsCellCentered(TRUE);
    break;
  case GridFunctionParameters::faceCenteredAxis1:
    setFaceCentering(axis1);
    break;
  case GridFunctionParameters::faceCenteredAxis2:
    setFaceCentering(axis2);
    break;
  case GridFunctionParameters::faceCenteredAxis3:
    setFaceCentering(axis3);
    break;
  case GridFunctionParameters::faceCentered:
    rcData->faceCentering=GridFunctionParameters::faceCenteredUnstructured;
    break;
  case GridFunctionParameters::faceCenteredAll:
    // kkc 031229
    if ( grid->gridType==MappedGrid::unstructuredGrid )
      rcData->faceCentering=GridFunctionParameters::faceCenteredUnstructured; // and what about str. grids?
    break;
  case GridFunctionParameters::edgeCentered:
    rcData->faceCentering=GridFunctionParameters::edgeCenteredUnstructured;
    break;
  default:
    break;
  }
  return returnValue;
}

//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGrid & grid0, 
		  const GridFunctionParameters::GridFunctionType & type)
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type, the components
//   are left unchanged.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;
  Range Rc[maximumNumberOfComponents];
  int i;
  for( i=0; i<maximumNumberOfComponents; i++ )
    Rc[i]=nullRange;
  for( i=0; i<numberOfComponents(); i++ )

    if( positionOfComponent(i)<maximumNumberOfIndicies )
      Rc[i]=R[positionOfComponent(i)];

  return updateToMatchGrid(grid0,type,Rc[0],Rc[1],Rc[2],Rc[3],Rc[4]);
}

//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(const GridFunctionParameters::GridFunctionType & type)
//==================================================================================
// /Description:
//   Use this update function to create a grid function of a given type, the components
//   are left unchanged.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  Range *R = rcData->R;
  Range Rc[maximumNumberOfComponents];
  int i;
  for( i=0; i<maximumNumberOfComponents; i++ )
    Rc[i]=nullRange;
  for( i=0; i<numberOfComponents(); i++ )

    if( positionOfComponent(i)<maximumNumberOfIndicies )
      Rc[i]=R[positionOfComponent(i)];

  return updateToMatchGrid(type,Rc[0],Rc[1],Rc[2],Rc[3],Rc[4]);
}

floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGridData & grid0, 
		  const Range & R0, 
		  const Range & R1,
		  const Range & R2,
		  const Range & R3,
		  const Range & R4,
		  const Range & R5,
		  const Range & R6,
		  const Range & R7 )
{
  grid=&grid0;
  return updateToMatchGrid( R0,R1,R2,R3,R4,R5,R6,R7 );
}


floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGrid & grid0)
{
  mappedGrid=&grid0;
  return updateToMatchGrid( *(grid0.rcData));
}

//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGrid(MappedGridData & grid0)
{
  grid=&grid0;
  return updateToMatchGrid();
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{updateToMatchGridFunction}}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGridFunction(const floatMappedGridFunction & cgf)
//==================================================================================
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  *rcData=*cgf.rcData;  // deep copy, except for:
  rcData->isACoefficientMatrix=cgf.rcData->isACoefficientMatrix;  // need to do this as well
  if( isACoefficientMatrix() )
  { // make a copy of the sparse matrix representation
    setIsACoefficientMatrix(TRUE);
    *sparse= *(cgf.sparse);
  }
  grid=cgf.grid;        // note: this only copies the pointer to the grid ****
  mappedGrid=cgf.mappedGrid; 
  operators=cgf.operators;
  isCellCentered.redim(0);
  isCellCentered=cgf.isCellCentered;
  // kkc added the following argument to make sure that the function is
  //     built with the correct centering
  // return updateToMatchGrid(cgf.getGridFunctionType()); // this will dimensions arrays properly

  // *wdh* 041231 -- cannot use the above call since it may change the dimensions when the position of the components
  // is not default such as for a coefficient array
  GridFunctionParameters::GridFunctionType gft = cgf.getGridFunctionType();
  if( gft==GridFunctionParameters::vertexCentered   || gft==GridFunctionParameters::cellCentered || 
      gft==GridFunctionParameters::defaultCentering || gft==GridFunctionParameters::general )
  {
    return updateToMatchGrid(); // this call will use the dimensions in rcData->R[]
  }
  else
  {
    // do this for faceCentered grid functions
    return updateToMatchGrid(cgf.getGridFunctionType()); // this will dimensions arrays properly
  }
  
}

//\begin{>>MappedGridFunctionInclude.tex}{}
floatMappedGridFunction::updateReturnValue floatMappedGridFunction::
updateToMatchGridFunction(const floatMappedGridFunction & cgf,
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
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  *rcData=*cgf.rcData;  // deep copy
  grid=cgf.grid;        // note: this only copies the pointer to the grid ****
  mappedGrid=cgf.mappedGrid; 
  operators=cgf.operators;
  isCellCentered.redim(0);
  isCellCentered=cgf.isCellCentered;  // this may be changed again in the line below
  return updateToMatchGrid(R0,R1,R2,R3,R4,R5,R6,R7);    // this will dimensions arrays properly
}

//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{sizeOf}}
real floatMappedGridFunction::
sizeOf(FILE *file /* = NULL */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this object; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{MappedGridFunctionInclude.tex}
//==========================================================================
{
  real size=sizeof(*this);
  size+=elementCount()*sizeof(float);
  
  if( sparse!=NULL )
    size+=sparse->sizeOf();

  return size;
}

#include "mathutil.h"

#define fixupOpt EXTERN_C_NAME(fixupopt)
extern "C"
{
  void fixupOpt( const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
	       const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b, 
               float&u,const float&val, const int&mask, const int&bc, const int&nMin,const int&nMax, const int&nGhost );

}


//\begin{>>MappedGridFunctionInclude.tex}{\subsubsection{fixupUnusedPoints}}
int floatMappedGridFunction::
fixupUnusedPoints(const RealArray & value /* =Overture::nullRealArray() */, 
                  int numberOfGhostlines /* =1 */ )
//==================================================================================
// /Description:
//    Assign values to points on a grid function that correspond to unused points (mask==0).
// By default all unused points are set to zero. Use the value array to set unused points to 
// particular values.
// 
// /values (input) : if supplied, assign value(n) to unused points of component n and do not change
//    any components not found in value. If not supplied set all unused points to zero.
// /numberOfGhostLines (input) : Indicate how many ghost lines are used in the computation. Other ghost line
//    values will all be set to zero. 
//
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  assert( mappedGrid!=NULL );
  
  RealArray val;
  if( value.getLength(0)==0 )
  {
    val.redim(Range(getComponentBase(0),getComponentBound(0)));
    val=0.;
  }
  else
  {
    val.reference(value);
  }
  const int nMin=max(val.getBase(0), getComponentBase(0));
  const int nMax=min(val.getBound(0),getComponentBound(0));

  MappedGrid & mg = *mappedGrid;
  #ifdef USE_PPP
    // *** this P++ version added 040405

    bool useOpt=true;  
    intSerialArray mask; ::getLocalArrayWithGhostBoundaries(mg.mask(),mask);

    const floatDistributedArray & ud = *this;
    floatSerialArray uu; ::getLocalArrayWithGhostBoundaries(ud,uu);

    // we need to adjust the gridIndexRange and boundaryCondition arrays:
    const IntegerArray & dimension = mg.dimension();
    IntegerArray gid(2,3), bc(2,3);
    gid = mg.gridIndexRange();
    bc  = mg.boundaryCondition();
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      const int na=mask.getBase(axis) +ud.getGhostBoundaryWidth(axis);
      const int nb=mask.getBound(axis)-ud.getGhostBoundaryWidth(axis);
      if( na>dimension(0,axis) )
      { // This is an internal partition boundary
        gid(0,axis)=na;
	bc(0,axis)=0;   // pretend this internal boundary is an interpolation boundary
      }
      if( nb<dimension(1,axis) )
      {// This is an internal partition boundary
        gid(1,axis)=nb;
	bc(1,axis)=0;   // pretend this internal boundary is an interpolation boundary
      }
    }
  #else
    bool useOpt=true;
    intArray & mask = mg.mask();
    floatDistributedArray & uu = *this;
    const IntegerArray & gid = mg.gridIndexRange();
    const IntegerArray & bc = mg.boundaryCondition();
  #endif

//  if( useOpt )
//  {
//    // Here is the optimised version
//
//    fixupOpt( mg.numberOfDimensions(),
//              uu.getBase(0),uu.getBound(0),
//              uu.getBase(1),uu.getBound(1),
//              uu.getBase(2),uu.getBound(2),
//              uu.getBase(3),uu.getBound(3),
//              gid(0,0),gid(1,0),
//              gid(0,1),gid(1,1),
//              gid(0,2),gid(1,2),
//	      *(::getDataPointer(uu)),*val.getDataPointer(),*mask.getDataPointer(),*bc.getDataPointer(),nMin,nMax,numberOfGhostlines);
//  }
//  else
  {
    Index I1,I2,I3;
    getIndex(mg.dimension(),I1,I2,I3);
    where( mask(I1,I2,I3) == 0 )  
    {
      for( int n=nMin; n<=nMax; n++ )
        uu(I1,I2,I3,n)=(float)val(n);
    }
      
    // we only use 'numberOfGhostlines' ghost lines, fixup any others.
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( mg.boundaryCondition(side,axis)>0 )
	{
	  const int startLine=side==0 ? mg.dimension(side,axis) : mg.gridIndexRange(side,axis)+numberOfGhostlines+1;
	  const int endLine  =side==0 ? mg.gridIndexRange(side,axis)-numberOfGhostlines-1 : mg.dimension(side,axis);
	  for( int line=startLine; line<=endLine; line++ )
	  {
	    int ghost=abs(line-mg.gridIndexRange(side,axis));
	    getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,ghost,ghost);
	    // we may need to restrict some directions if there are not an equal number of
	    // ghost lines in every side and direction
	    I1=Range(max(I1.getBase(),mg.dimension(0,0)),min(I1.getBound(),mg.dimension(1,0)));
	    I2=Range(max(I2.getBase(),mg.dimension(0,1)),min(I2.getBound(),mg.dimension(1,1)));
	    I3=Range(max(I3.getBase(),mg.dimension(0,2)),min(I3.getBound(),mg.dimension(1,2)));
	  
	    for( int n=nMin; n<=nMax; n++ )
	      uu(I1,I2,I3,n)=(float)val(n);
	  }
	}
      }
    }
  }
  
  return 0;
}



// int floatMappedGridFunction::
// zeroUnusedPoints(floatMappedGridFunction & coeff, 
//                  float value /* = 0 */,
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
// // /Notes: If the classify array has different values for different components (for a system
// // of equations) then we do the following
// // \begin{enumerate}
// //  \item if the length of component0 equals the number of components in coeff then we use 
// //      the different components of the classify array for the different components of this grid function.
// //  \item if the length of component0 is not equal to the number of components in coeff then we use
// //      the first component of the classify array for all components in this grid function.
// // \end{enumerate}   
// //
// // /coeff (input) : a coefficient matrix that defines a sparse matrix.
// // /value (input) : assign unused points this value.
// // /component0,... (input): zero out these components of the grid function, by default apply to all components.
// //\end{MappedGridFunctionInclude.tex} 
// //==================================================================================
// {
//   assert( coeff.sparse!=NULL );
  
//   floatMappedGridFunction & u = *this;
//   MappedGrid & mg = *u.getMappedGrid();
//   IntegerArray & classify = coeff.sparse->classify;
  
//   Index I1=rcData->R[0],
//         I2=rcData->R[1],
//         I3=rcData->R[2];
//   // getIndex(mg.dimension,I1,I2,I3);
//   Range C0,C1;
//   C0 = component0.getLength()>0 ? Range(component0) : Range(u.getComponentBase(0),u.getComponentBound(0));
//   C1 = component1.getLength()>0 ? Range(component1) : Range(u.getComponentBase(1),u.getComponentBound(1));
//   // these cases not implemented :
//   assert( component2.getLength()==0 );
//   assert( component3.getLength()==0 );
//   assert( component4.getLength()==0 );
  

//   if( coeff.sparse->numberOfComponents==1 )
//   {
//     where( classify(I1,I2,I3) == SparseRepForMGF::unused )
//     {
//       for( int c1=C1.getBase(); c1<=C1.getBound(); c1++ )
// 	for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )
// 	{
// 	  u(I1,I2,I3,c0,c1)=value; 
// 	}
//     }
//   }
//   else
//   {
//     // The calssify array has different values for different components. This is
//     // not well defined what we should do this this case.
//     if( C0.getLength()==coeff.sparse->numberOfComponents )
//     {
//       for( int c1=C1.getBase(); c1<=C1.getBound(); c1++ )
//       {
//         int n=0;
// 	for( int c0=C0.getBase(); c0<=C0.getBound(); c0++,n++ )
// 	{
// 	  where( classify(I1,I2,I3,n) == SparseRepForMGF::unused )  
// 	  {
// 	    u(I1,I2,I3,c0,c1)=value; 
// 	  }
// 	}
//       }
//     }
//     else
//     {
//       where( classify(I1,I2,I3,0) == SparseRepForMGF::unused )  
//       {
// 	for( int c1=C1.getBase(); c1<=C1.getBound(); c1++ )
// 	  for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )
// 	  {
// 	    u(I1,I2,I3,c0,c1)=value; 
// 	  }
//       }
//     }
//   }
  
//   return 0;
// }



//---------------------------------------------------------------------------------------
//   These routines return particular derivatives
//      These functions are only defined for float (if compiled in single precison)
//      or for float (if compiled in float precision)
//---------------------------------------------------------------------------------------

//==========================================================================================================
// MACRO DERIVATIVE
//  Define a macro to return the derivative "x"
// Notes:
//  o since we can only take a derivative of a realMappedGridFunction we need to have a different
//    version of this macro depending on whether we compile the code with OV_USE_DOUBLE
//==========================================================================================================



// These next macro definitions are use to handle non-standard arguments to the DERIVATIVE macro
#undef  ARGS1
#undef  ARGS2
#define ARGS1 const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 

#define ARGS2 I1,I2,I3,I4,I5,I6,I7,I8

#ifdef OV_USE_DOUBLE
#define DERIVATIVE(x)                                                                                        \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
x( ARGS1 )  const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
      derivativeError(); return *this;                                                               \
       \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to take a derivative without defining a derivative routine\n";\
    if( this )  \
      Overture::abort("floatMappedGridFunction: ERROR");                                                                       \
    return *this;                                                                                            \
  }                                                                                                          \
} \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
x( const GridFunctionParameters & gfType, ARGS1 )  const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
      derivativeError(); return *this;                                                               \
       \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to take a derivative without defining a derivative routine\n";\
    if( this )  \
      Overture::abort("floatMappedGridFunction: ERROR");                                                                       \
    return *this;                                                                                            \
  }                                                                                                          \
}
#else
#define DERIVATIVE(x)                                                                                        \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
x( ARGS1 )  const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
       \
      return operators->x(*this,ARGS2);                                                              \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to take a derivative without defining a derivative routine\n";\
    if( this )  \
      Overture::abort("floatMappedGridFunction: ERROR");                                                                       \
    return *this;                                                                                            \
  }                                                                                                          \
} \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
x(  const GridFunctionParameters & gfType, ARGS1 )  const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
       \
      return operators->x(*this,gfType,ARGS2);                                                              \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to take a derivative without defining a derivative routine\n";\
    if( this )  \
      Overture::abort("floatMappedGridFunction: ERROR");                                                                       \
    return *this;                                                                                            \
  }                                                                                                          \
}
#endif

// Now define all the instances of this macro

DERIVATIVE(x)
DERIVATIVE(y)
DERIVATIVE(z)
DERIVATIVE(xx)
DERIVATIVE(xy)
DERIVATIVE(xz)
DERIVATIVE(yy)
DERIVATIVE(yz)
DERIVATIVE(zz)

DERIVATIVE(laplacian)
DERIVATIVE(grad)
DERIVATIVE(div)

DERIVATIVE(r1)
DERIVATIVE(r2)
DERIVATIVE(r3)
DERIVATIVE(r1r1)
DERIVATIVE(r1r2)
DERIVATIVE(r1r3)
DERIVATIVE(r2r2)
DERIVATIVE(r2r3)
DERIVATIVE(r3r3)

DERIVATIVE(cellsToFaces)
DERIVATIVE(convectiveDerivative)
DERIVATIVE(contravariantVelocity)
DERIVATIVE(divNormal)
DERIVATIVE(normalVelocity)
DERIVATIVE(identity)
DERIVATIVE(vorticity)

#undef  ARGS1
#undef  ARGS2
#define ARGS1 const floatMappedGridFunction &w, \
              const Index & I1, \
              const Index & I2, \
              const Index & I3

#define ARGS2 w,I1,I2,I3

DERIVATIVE(convectiveDerivative)


#undef  ARGS1
#undef  ARGS2
#define ARGS1 const floatMappedGridFunction &w, \
              const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 

#define ARGS2 w,I1,I2,I3,I4,I5,I6,I7,I8
DERIVATIVE(divScalarGrad)
DERIVATIVE(divInverseScalarGrad)
DERIVATIVE(divVectorScalar)

#undef  ARGS1
#undef  ARGS2
#define ARGS1 const floatMappedGridFunction &w, \
              const int & direction1, \
              const int & direction2, \
              const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 

#define ARGS2 w,direction1,direction2,I1,I2,I3,I4,I5,I6,I7,I8
DERIVATIVE(derivativeScalarDerivative)


#undef  ARGS1
#define ARGS1 const int c0,  \
	      const int c1,  \
	      const int c2,  \
	      const int c3,  \
	      const int c4,   \
              const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 

#undef  ARGS2
#define ARGS2 c0,c1,c2,c3,c4,I1,I2,I3,I4,I5,I6,I7,I8
DERIVATIVE(FCgrad)


#undef DERIVATIVE





//==========================================================================================================
//  Define a macro to return the COEFFICIENTS of the derivative "x"
// Notes:
//  o since we can only take a derivative of a realMappedGridFunction we need to have a different
//    version of this macro depending on whether we compile the code with OV_USE_DOUBLE
//==========================================================================================================

#undef  ARGS1
#undef  ARGS2
#define ARGS1 const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 

#define ARGS2 I1,I2,I3,I4,I5,I6,I7,I8

#ifdef OV_USE_DOUBLE
#define DERIVATIVE_COEFFICIENTS(X)                                                                           \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
X( ARGS1 ) const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
      derivativeError(); return *this;                                                               \
       \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                            \
  }                                                                                                          \
}        \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
X( const GridFunctionParameters & gfType, ARGS1 ) const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
      derivativeError(); return *this;                                                               \
       \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                            \
  }                                                                                                          \
}
#else
#define DERIVATIVE_COEFFICIENTS(X)                                                                           \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
X(ARGS1) const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
       \
      return operators->X(ARGS2);                                                                    \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                            \
  }                                                                                                          \
}   \
floatMappedGridFunction floatMappedGridFunction::                                                                      \
X( const GridFunctionParameters & gfType, ARGS1) const \
{                                                                                                            \
  if( operators!=NULL )                                                                                      \
  {                                                                                                          \
       \
       \
      return operators->X(gfType, ARGS2);                                                                    \
  }                                                                                                          \
  else                                                                                                       \
  {                                                                                                          \
    cout << "floatMappedGridFunction: ERROR: trying to get coefficients without defining a derivative routine\n"; \
    return *this;                                                                                            \
  }                                                                                                          \
}
#endif

// Now define all the instances of this macro

DERIVATIVE_COEFFICIENTS(xCoefficients)
DERIVATIVE_COEFFICIENTS(yCoefficients)
DERIVATIVE_COEFFICIENTS(zCoefficients)
DERIVATIVE_COEFFICIENTS(xxCoefficients)
DERIVATIVE_COEFFICIENTS(xyCoefficients)
DERIVATIVE_COEFFICIENTS(xzCoefficients)
DERIVATIVE_COEFFICIENTS(yyCoefficients)
DERIVATIVE_COEFFICIENTS(yzCoefficients)
DERIVATIVE_COEFFICIENTS(zzCoefficients)

DERIVATIVE_COEFFICIENTS(laplacianCoefficients)
DERIVATIVE_COEFFICIENTS(divCoefficients)
DERIVATIVE_COEFFICIENTS(identityCoefficients)

DERIVATIVE_COEFFICIENTS(r1Coefficients)
DERIVATIVE_COEFFICIENTS(r2Coefficients)
DERIVATIVE_COEFFICIENTS(r3Coefficients)
DERIVATIVE_COEFFICIENTS(r1r1Coefficients)
DERIVATIVE_COEFFICIENTS(r1r2Coefficients)
DERIVATIVE_COEFFICIENTS(r1r3Coefficients)
DERIVATIVE_COEFFICIENTS(r2r2Coefficients)
DERIVATIVE_COEFFICIENTS(r2r3Coefficients)
DERIVATIVE_COEFFICIENTS(r3r3Coefficients)

DERIVATIVE_COEFFICIENTS(gradCoefficients)


#undef  ARGS1
#define ARGS1 const floatMappedGridFunction &s, \
              const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 
#undef  ARGS2
#define ARGS2 s,I1,I2,I3,I4,I5,I6,I7,I8

DERIVATIVE_COEFFICIENTS(divScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divInverseScalarGradCoefficients)
DERIVATIVE_COEFFICIENTS(divVectorScalarCoefficients)

#undef  ARGS1
#define ARGS1 const floatMappedGridFunction &s, \
              const int & direction1, \
              const int & direction2, \
              const Index & I1, \
              const Index & I2, \
              const Index & I3, \
              const Index & I4, \
              const Index & I5, \
              const Index & I6, \
              const Index & I7, \
              const Index & I8 
#undef  ARGS2
#define ARGS2 s,direction1,direction2,I1,I2,I3,I4,I5,I6,I7,I8

DERIVATIVE_COEFFICIENTS(derivativeScalarDerivativeCoefficients)


#undef DERIVATIVE_COEFFICIENTS
#undef ARGS1
#undef ARGS2

// These next declarations are needed to be compatible with STL
intDistributedArray operator!=(const floatMappedGridFunction& x, const floatMappedGridFunction& y)
  { return (floatDistributedArray&)x != (floatDistributedArray&)y; }
intDistributedArray operator> (const floatMappedGridFunction& x, const floatMappedGridFunction& y)
  { return (floatDistributedArray&)x >  (floatDistributedArray&)y; }
intDistributedArray operator<=(const floatMappedGridFunction& x, const floatMappedGridFunction& y)
  { return (floatDistributedArray&)x <= (floatDistributedArray&)y; }
intDistributedArray operator>=(const floatMappedGridFunction& x, const floatMappedGridFunction& y)
  { return (floatDistributedArray&)x >= (floatDistributedArray&)y; }
intDistributedArray operator!=(const floatMappedGridFunction& x, const float& y)
  { return (floatDistributedArray&)x != y ; }
intDistributedArray operator> (const floatMappedGridFunction& x, const float& y)
  { return (floatDistributedArray&)x >  y ; }
intDistributedArray operator<=(const floatMappedGridFunction& x, const float& y)
  { return (floatDistributedArray&)x <= y ; }
intDistributedArray operator>=(const floatMappedGridFunction& x, const float& y)
  { return (floatDistributedArray&)x >= y ; }
intDistributedArray operator!=(const float& x, const floatMappedGridFunction& y)
  { return x != (floatDistributedArray&)y ; }
intDistributedArray operator> (const float& x, const floatMappedGridFunction& y)
  { return x >  (floatDistributedArray&)y ; }
intDistributedArray operator<=(const float& x, const floatMappedGridFunction& y)
  { return x <= (floatDistributedArray&)y ; }
intDistributedArray operator>=(const float& x, const floatMappedGridFunction& y)
  { return x >= (floatDistributedArray&)y ; }

// add these to overcome STL's definition of min and max

floatDistributedArray min(const floatMappedGridFunction & u, const floatMappedGridFunction & v )
{ return min((floatDistributedArray&)u, (floatDistributedArray&)v ); }
floatDistributedArray min(const floatMappedGridFunction & u, const float& x )
{ return min((floatDistributedArray&)u, x ); }
floatDistributedArray min(const float& x, const floatMappedGridFunction & v )
{ return min(x, (floatDistributedArray&)v ); }

floatDistributedArray max(const floatMappedGridFunction & u, const floatMappedGridFunction & v )
{ return max((floatDistributedArray&)u, (floatDistributedArray&)v ); }
floatDistributedArray max(const floatMappedGridFunction & u, const float& x )
{ return max((floatDistributedArray&)u, x ); }
floatDistributedArray max(const float& x, const floatMappedGridFunction & v )
{ return max(x, (floatDistributedArray&)v ); }

