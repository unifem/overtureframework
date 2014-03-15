#include "CompositeGridFiniteVolumeOperators.h"
#include "GridCollectionFiniteVolumeOperators.h"

CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators()
//===========================================================================================
{
  setup();
  mappedGridOperatorsPointer = new MappedGridFiniteVolumeOperators();
  mappedGridOperatorWasNewed=TRUE;
}

//\begin{>>CompositeGridFiniteVolumeOperators.tex}{\subsection{CompositeGridFiniteVolumeOperators Constructor from a CompositeGrid}}
CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators( CompositeGrid & cg0 )
//
// /Description:
//   Construct a CompositeGridFiniteVolumeOperators object
// /cg0 (input): Associate this grid with the operators.
// /Author: DLB
//\end{CompositeGridFiniteVolumeOperators.tex}
//=======================================================================================
{
  setup ();
  mappedGridOperatorsPointer = new MappedGridFiniteVolumeOperators();
  mappedGridOperatorWasNewed=TRUE;
  updateToMatchGrid( cg0 );
}

//\begin{>CompositeGridFiniteVolumeOperators.tex}{}
CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators( MappedGridFiniteVolumeOperators & op )
: GenericCompositeGridOperators (op)
//
//
// /Description:
//   Construct a CompositeGridFiniteVolumeOperators using a MappedGridOperators
// /op (input): Associate this grid with these operators.
// /Author: WDH
//\end{CompositeGridFiniteVolumeOperators.tex}
//=======================================================================================
{
  mappedGridOperatorWasNewed = FALSE;
}

//===========================================================================================
// This constructor takes a grid collection and a MappedGridOperators as input
//===========================================================================================
CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators(CompositeGrid & cg0, MappedGridFiniteVolumeOperators & op)
//: GridCollectionFiniteVolumeOperators(cg0,op)
: GenericCompositeGridOperators (cg0, op)
{
  mappedGridOperatorWasNewed = FALSE;
}


//===========================================================================================
//  Copy constructor 
//   deep copy
//===========================================================================================
CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators( const CompositeGridFiniteVolumeOperators & cg0 ) 
{
 // GridCollectionFiniteVolumeOperators::operator=(cg0);  
  *this = cg0;
}

/* Gone with .v3
CompositeGridFiniteVolumeOperators::
CompositeGridFiniteVolumeOperators( const GridCollectionFiniteVolumeOperators & cg0 ) 
{
  GridCollectionFiniteVolumeOperators::operator=(cg0);  
}
*/

//=======================================================================================
// destructor
//=======================================================================================
CompositeGridFiniteVolumeOperators::~CompositeGridFiniteVolumeOperators()
{
  cout << "CompositeGridFiniteVolumeOperators destructor (:(:" << endl;
  if (mappedGridOperatorWasNewed) delete mappedGridOperatorsPointer;
}

//=======================================================================================
//   virtualConstructor
//
//  This routine should create a new object of this class and return as a pointer
//  to the base classGridCollectionFiniteVolumeOperators.
//
//  Notes:
//   o This routine is needed if this class has been derived from the base class CompositeGridFiniteVolumeOperators
//   o This routine is used by the classe MultigridCompositeGridOperators
//     in order to construct lists of this class. These classes only know about the base class
//     and so they are unable to create a "new" version of this class
//=======================================================================================
//GenericGridCollectionOperators* CompositeGridFiniteVolumeOperators::
GenericCompositeGridOperators* CompositeGridFiniteVolumeOperators::
virtualConstructor()
{
  return new CompositeGridFiniteVolumeOperators();
}


//===========================================================================================
// operator = is a deep copy
//===========================================================================================
CompositeGridFiniteVolumeOperators & CompositeGridFiniteVolumeOperators::
operator= ( const CompositeGridFiniteVolumeOperators & cgo )
{
//  GridCollectionFiniteVolumeOperators::operator=(cgo);  
  GenericCompositeGridOperators::operator=(cgo); 
  return *this;
}
GenericCompositeGridOperators & CompositeGridFiniteVolumeOperators::
operator= ( const GenericCompositeGridOperators & gco )
{
  if( this )
  {
    cout << "CompositeGridFiniteVolumeOperators::operator= :ERROR: operator= ( const GenericCompositeGridOperators & gco )"
            "  called\n";
    throw "error";
  }
  return *this;
}

/* *** Eliminate this operator= for v3
//===========================================================================================
// operator = is a deep copy
//===========================================================================================
CompositeGridFiniteVolumeOperators & CompositeGridFiniteVolumeOperators::
operator= ( const GridCollectionFiniteVolumeOperators & gco )
{
  GridCollectionFiniteVolumeOperators::operator=(gco);  
  return *this;
}
*/

/* *** Gone with Overture.v3
void CompositeGridFiniteVolumeOperators::
finishBoundaryConditions(realGridCollectionFunction & u )
//===========================================================================================
//  For coefficient matrices we need to add the interpolation equations
//===========================================================================================
{
  GridCollectionFiniteVolumeOperators::finishBoundaryConditions(u); 

  // cout << "****finishBoundaryConditions: u.getIsACoefficientMatrix() = " << u.getIsACoefficientMatrix() << endl;
  if( u.getIsACoefficientMatrix() )
  {
    getInterpolationCoefficients( (realCompositeGridFunction &)u );
  }
}

*/

//==============================================================================
// 960722: adding this; do we actually need it? ******
//
void CompositeGridFiniteVolumeOperators::
updateToMatchGrid( CompositeGrid & gc )              
{
  GenericCompositeGridOperators::updateToMatchGrid(gc); 

  numberOfDimensions = gc.numberOfDimensions();
  numberOfComponentGrids = gc.numberOfGrids();
  numberOfGrids = gc.numberOfGrids();
    
  gridCollection.update (
    GridCollection::THEcenter
    | GridCollection::THEfaceNormal
    | GridCollection::THEfaceArea
    | GridCollection::THEcellVolume
    | GridCollection::THEcenterNormal
    | GridCollection::THEcenterBoundaryNormal
    ,
    GridCollection::COMPUTEgeometryAsNeeded
    | GridCollection::USEdifferenceApproximation
    );
    
}

//==============================================================================
//
void CompositeGridFiniteVolumeOperators::
setIsVolumeScaled (const bool trueOrFalse)
//
// /Purpose: Tell the class that all operator functions are to return
//           results that are scaled by the {\bf cellVolume}'s . 
//           It is sometimes useful to call this function for efficiency
//           reasons when the result is not going to be interpolated,
//           since it avoids dividing by cell volumes at the end of 
//           each function.
//           {\it{\bf Warning:} This is not a good
//           idea on an overlapping grid, since typically, volume-scaled
//           functions cannot be interpolated in a way that makes sense.} 
//
// /trueOrFalse: 
//   \begin{itemize}
//     \item TRUE: operators will be scaled by cell volume
//     \item FALSE: operators will not be scaled by cell volume 
//   \end{itemize}
//
// /Author: D. L. Brown
// /Documentation last modified: 951019  
//
//==============================================================================
  { 
    isVolumeScaled = trueOrFalse;
    int grid;
    ForAllGrids (grid){
      ((MappedGridFiniteVolumeOperators &)mappedGridOperators[grid]).setIsVolumeScaled(trueOrFalse);
    }
  }
//==============================================================================
//
bool CompositeGridFiniteVolumeOperators::
getIsVolumeScaled () 
//
// /Purpose: determine whether CompositeGridFiniteVolumeOperators function will
//           be computed volume scaled or not. Using volumeScaled operators
//           at the composite grid level is not a good idea for values that
//           will be interpolated since volumeScaled functions will not
//           be interpolated in a reasonable way.
//
// /getIsVolumeScaled (output):
//   
//    \begin{itemize}
//      \item TRUE:   operators return values scaled by cellVolume
//      \item FALSE:  operators return values unscaled
//    \end{itemize}
// 
//
//==============================================================================
  {
    return (isVolumeScaled);
  }


// --------------------Boundary Condition Routines --------------------------------

// --------------------Boundary Condition Routines --------------------------------





void CompositeGridFiniteVolumeOperators::
//
setNumberOfBoundaryConditions(const int & number, 
			      const int & side,  // =forAll
			      const int & axis,  // =forAll
			      const int & grid0  // =forAll
                             )
//=======================================================================================
// /Description:
//  Indicate how many boundary conditions are to be applied on a given side
// /number (input): specify this number of boundary conditions.
// /side, axis (input): defines the side of the grid, by default do all sides and all axes.
//
//
//=======================================================================================
{
//  int numberOfGrids = gridCollection.numberOfComponentGrids();
  
  if( grid0==forAll )
  {
    for( int grid=0; grid<numberOfGrids; grid++ )
      ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid]).setNumberOfBoundaryConditions(number,side,axis);
  }
  else
  {
    ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid0]).setNumberOfBoundaryConditions(number,side,axis);
  }
}


//
void CompositeGridFiniteVolumeOperators:: 
setBoundaryCondition(const MappedGridFiniteVolumeOperators::boundaryConditionTypes & boundaryConditionType )
//=======================================================================================
// /Description:
//   Define what a given boundary condition should be.
//   This version assigns the given boundaryConditionType to all boundaries and all components
// /boundaryConditionType: the type of boundary condition
//
//
//=======================================================================================
{
//  int numberOfGrids = gridCollection.numberOfComponentGrids();
  
  for( int grid=0; grid<numberOfGrids; grid++ )
    ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid]).setBoundaryCondition(boundaryConditionType);
}

void CompositeGridFiniteVolumeOperators:: 
setBoundaryConditionValue(const real & value,  
			  const int & component,   // ***** get rid of this argument *****
			  const int & index,       // =forAll
			  const int & side,        // =forAll
			  const int & axis,        // =forAll
			  const int & grid0)       // =forAll
{
//  int numberOfGrids = gridCollection.numberOfComponentGrids();
  
  if( grid0==forAll )
  {
    for( int grid=0; grid<numberOfGrids; grid++ )
      ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid]).setBoundaryConditionValue(value,component,index,side,axis);
  }
  else
  {
    ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid0]).setBoundaryConditionValue(value,component,index,side,axis);
  }
}

//==============================================================================
//
void CompositeGridFiniteVolumeOperators::
setBoundaryConditionRightHandSide( const REALGridCollectionFunction & boundaryConditionRightHandSide) 
//
// /Purpose:
//   Assign a gridFunction to be used as rightHandSide of boundary conditions
//
//   /boundaryConditionRightHandSide (input): this contains the forcing for the BCs.
//                         The forcing should be in the ``boundary'' rows of the 
//                         array. Currently this is a full CompositeGridFunction rather
//                         than a boundaryGridFunction.
//
//
//==============================================================================
{
  int grid;
  ForAllGrids(grid)
  {
    ((MappedGridFiniteVolumeOperators&) mappedGridOperators[grid]).setBoundaryConditionRightHandSide(boundaryConditionRightHandSide[grid]);
  }
}

/* THE GENERIC VERSION IS SUFFICIENT
//==============================================================================
//
void CompositeGridFiniteVolumeOperators::
applyBoundaryConditions(realGridCollectionFunction & u,
                        const real & time,
			const int & grid0) 
//
// /Purpose:
//	Apply boundary conditions on all grids to a realGridCollectionFunction 
//
// /u(input/output):	function to which boundary conditions are applied
// /time:                  time at which boundary condition is applied
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	950714
// 
//
//========================================
{
  int grid;
  ForAllGrids (grid) {
    mappedGridOperators[grid].applyBoundaryConditions(u[grid],time);
    CGDisplay.display (u[grid], "CompositeGridFiniteVolumeOperators::applyBoundaryConditions: u[grid]");
  }
}
*/
 
void CompositeGridFiniteVolumeOperators::
//
applyBoundaryConditionsToCoefficients(
                        realGridCollectionFunction & coeff,      
                        const real & time
                        ) 
//
//  /Purpose:
//     Apply boundary conditions to coefficient array for implicit operators.
//     This sets coefficients in the coefficient array to implement the left
//     hand side of various boundary conditions.
//
//  /coeff (input):
//     The input coefficient array
// /time (input):
//     Some boundary conditions are time-dependent. In that case, this parameter
//     passes in the ``time'' variable.
//
//
//==============================================================================
{
  int grid;
  ForAllGrids (grid) ((MappedGridFiniteVolumeOperators&) mappedGridOperators[grid]).applyBoundaryConditionsToCoefficients(coeff[grid],time);
}
//==============================================================================
//
void CompositeGridFiniteVolumeOperators::
applyRightHandSideBoundaryConditions(
				     REALCompositeGridFunction & rightHandSide,
				     const real & time                 // = 0.
				     ) 
//
//  /Purpose:
//    Set right hand side of boundary conditions for implicit operators
//
//    /rightHandSide (input):
//       contiains the right hand side of the boundary conditions for implicit operators
//       in the rows along the boundary.
//
//
//==============================================================================			
{
  int grid;
  ForAllGrids (grid) ((MappedGridFiniteVolumeOperators&) mappedGridOperators[grid]).applyRightHandSideBoundaryConditions(rightHandSide[grid]);
}

/* -----  Here are some comments -----
//\begin{>>CompositeGridFiniteVolumeOperators.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
GridCollectionFunction 
"derivative"(const realCompositeGridFunction & u,
	   const Index & C // = nullIndex
         )
//==================================================================================
// /Description:
//   "derivative" equals one of x, y, z, xx, xy, xz, yy, yz, zz, laplacian, grad, div.
// /u (input): Take the derivative of this grid function.
// /C (input): evaluate the derivatives for these components.
// /return Value:
//   The derivative. 
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================

//\begin{>>CompositeGridFiniteVolumeOperators.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div with alternate centering}}
GridCollectionFunction 
"derivative"(const realCompositeGridFunction & u,
           GridFunctionParameters & gfType,
	   const Index & C // = nullIndex
         )
//==================================================================================
// /Description:
//   "derivative" equals one of x, y, z, xx, xy, xz, yy, yz, zz, laplacian, grad, div.
// /u (input): Take the derivative of this grid function.
// /gfType (input): the centering of the output gridFunction is determined by gfType.outputType
// /C (input): evaluate the derivatives for these components.
// /return Value:
//   The derivative. 
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================

//\begin{>>CompositeGridFiniteVolumeOperatorsInclude.tex}{\subsubsection{Derivative coefficients}}
GridCollectionFunction 
"derivativeCoefficients"(const Index & E // =nullIndex,
                         const Index & C // =nullIndex)
//==================================================================================
// /Description:
//   "derivativeCoefficients" equals one of xCoefficients, yCoefficients, zCoefficients, 
//   xxCoefficients, xyCoefficients, xzCoefficients, yyCoefficients, yzCoefficients, zzCoefficients,
//    laplacianCoefficients, gradCoefficients, divCoefficients.
//   Compute the coefficients of the specified derivative.
// /E (input): evaluate the coefficients for these equations.
// /C (input): evaluate the coefficients for these components.
// /return Value:
//   The derivative coefficients.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================

//\begin{>>CompositeGridFiniteVolumeOperatorsInclude.tex}{\subsubsection{Derivative coefficients with alternate centerings}}
GridCollectionFunction 
"derivativeCoefficients"(GridFunctionParameters & gfParams,
                         const Index & E // =nullIndex,
                         const Index & C // =nullIndex)
//==================================================================================
// /Description:
//   "derivativeCoefficients" equals one of xCoefficients, yCoefficients, zCoefficients, 
//   xxCoefficients, xyCoefficients, xzCoefficients, yyCoefficients, yzCoefficients, zzCoefficients,
//    laplacianCoefficients, gradCoefficients, divCoefficients.
//   Compute the coefficients of the specified derivative.
// /gfParams (input): The coefficients are determined for an equation that solves for
//        a solution with centering specified by gfParams.inputType given a right-hand side
//        with centering specified by gfParams.outputType.
// /E (input): evaluate the coefficients for these equations.
// /C (input): evaluate the coefficients for these components.
// /return Value:
//   The derivative coefficients.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
 ----------------- */
