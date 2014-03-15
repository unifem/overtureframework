#include "GridCollectionFiniteVolumeOperators.h"


//
//          *** here are some comments ****
//
//\begin{>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
//
//
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}



//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{Constructors}}  
GridCollectionFiniteVolumeOperators::
GridCollectionFiniteVolumeOperators()
//===========================================================================================
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}
//===========================================================================================
{
  setup();
  mappedGridOperatorsPointer=new MappedGridFiniteVolumeOperators();   // remember to delete this!
  mappedGridOperatorWasNewed=TRUE;
}

//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{}
GridCollectionFiniteVolumeOperators::
GridCollectionFiniteVolumeOperators( GridCollection & gridCollection0 )
//=======================================================================================
// /Description:
//   Construct a GridCollectionFiniteVolumeOperators
// /gridCollection0 (input): Associate this grid with the operators.
// /Author: WDH
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}
//=======================================================================================
{
  setup();
  mappedGridOperatorsPointer=new MappedGridFiniteVolumeOperators();   // remember to delete this!
  mappedGridOperatorWasNewed=TRUE;
  updateToMatchGrid( gridCollection0 );
}

//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{}
GridCollectionFiniteVolumeOperators::
GridCollectionFiniteVolumeOperators( MappedGridFiniteVolumeOperators & op )
//=======================================================================================
// /Description:
//   Construct a GridCollectionFiniteVolumeOperators using a MappedGridFiniteVolumeOperators
// /op (input): Associate this grid with these operators.
// /Author: WDH
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}
//=======================================================================================
{
  setup();
  mappedGridOperatorsPointer=&op;
}

//===========================================================================================
// This constructor takes a grid collection and a MappedGridFiniteVolumeOperators as input
//===========================================================================================
GridCollectionFiniteVolumeOperators::
GridCollectionFiniteVolumeOperators( GridCollection & gridCollection0, MappedGridFiniteVolumeOperators & op)
{
  setup();
  mappedGridOperatorsPointer=&op;
  updateToMatchGrid( gridCollection0 );
}


//===========================================================================================
//  Copy constructor 
//   deep copy
//===========================================================================================
GridCollectionFiniteVolumeOperators::
GridCollectionFiniteVolumeOperators( const GridCollectionFiniteVolumeOperators & gco ) 
{
  *this=gco;   // this uses the = operator which is a deep copy
}

GridCollectionFiniteVolumeOperators::
~GridCollectionFiniteVolumeOperators()
{
  if( mappedGridOperatorWasNewed )
    delete mappedGridOperatorsPointer;  // ** only delete if it was newed ***

}

// **** this is not realyy needed here - could use base class version *****
void GridCollectionFiniteVolumeOperators::
updateToMatchGrid( GridCollection & gc )              
{
  GenericGridCollectionOperators::updateToMatchGrid(gc); 

  numberOfDimensions = gc.numberOfDimensions();
 //  numberOfComponentGrids = gc.numberOfComponentGrids();
  numberOfComponentGrids = gc.numberOfGrids();
  numberOfGrids = gc.numberOfGrids();
    
  gridCollection.update (
			   GridCollection::THEcenter
			 | GridCollection::THEfaceNormal
			 | GridCollection::THEfaceArea
			 | GridCollection::THEcellVolume
			 | GridCollection::THEcenterNormal
			 ,
  			   GridCollection::COMPUTEgeometryAsNeeded
			 | GridCollection::USEdifferenceApproximation
			 );
    
}


//================================================================================
// return the MappedGridFiniteVolumeOperators object for  MappedGrid "grid"
//================================================================================
MappedGridFiniteVolumeOperators & GridCollectionFiniteVolumeOperators::
operator[]( const int grid ) 
{
  if( grid>=0 && grid<mappedGridOperators.getLength() )
  {
    return (MappedGridFiniteVolumeOperators&) mappedGridOperators[grid];
  }
  else
  {
    cout << "GridCollectionFiniteVolumeOperators:ERROR in operator[]  argument grid is invalid" << endl;
    cout << "grid = " << grid << endl;
    cout << "Perhaps you forgot to associate a GridCollection with the GridCollectionFiniteVolumeOperators object\n";
    cout << "Do this in the constructor of GridCollectionFiniteVolumeOperators or use updateToMatchGrid\n";
    throw "This is a fatal error";
  }    
}

//=======================================================================================
//   virtualConstructor
//
//  This routine should create a new object of this class and return as a pointer
//  to the base classGridCollectionFiniteVolumeOperators.
//
//  Notes:
//   o This routine is needed if this class has been derived from the base class GridCollectionFiniteVolumeOperators
//   o This routine is used by the classe MultigridCompositeGridOperators
//     in order to construct lists of this class. These classes only know about the base class
//     and so they are unable to create a "new" version of this class
//=======================================================================================
GenericGridCollectionOperators* GridCollectionFiniteVolumeOperators::
virtualConstructor()
{
  return new GridCollectionFiniteVolumeOperators();
}

/* -----  Here are some comments -----
//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
GridCollectionFunction 
derivative(const realGridCollectionFunction & u,
           const Index & N  // =nullIndex 
         )
//==================================================================================
// /Description:
//   derivative equals one of x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div.
// /u (input): Take the derivative of this grid function.
// /N (input): evaluate the derivatives for these components.
// /return Value:
//   The derivative. 
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================

//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
GridCollectionFunction 
Derivative(const Index & N // =nullIndex )
//==================================================================================
// /Description:
//   derivative equals one of X,Y,Z,XX,XY,XZ,YY,YZ,ZZ,Laplacian,Div,I.
//   Compute the coefficients of the specified derivative.
// /N (input): evaluate the coefficients for these components.
// /return Value:
//   The derivative coefficients.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
 ----------------- */


/* WE DONT NEED THIS AT ALL

//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{get}}
void GridCollectionFiniteVolumeOperators::
get( const Dir & dir, const aString & name)
//-------------------------------------------------------------------
// /Description:
//   Get from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}{}
//-------------------------------------------------------------------
{
  // Use directory unless name="."
  Dir subDir = name=="." ? dir : dir.findDir(name);

  
//  // ********* rcData put ??
//  char buff[40];
//  int i;
//  subDir.get( i,"numberOfComponents" );   numberOfComponents=i;
//  subDir.get( i,"positionOfComponent" );  positionOfComponent=i;
//  subDir.get( rcData->numberOfNames,"numberOfNames" );
//  delete [] rcData->name;
//  rcData->name = ::new aString[rcData->numberOfNames];
//  for( i=0; i<rcData->numberOfNames; i++ )
//    subDir.get( rcData->name[i],sprintf(buff,"name[%i]",i) );
//  subDir.get( *this,"arrayData" );  // get the A++ array
  
}

*/




//===========================================================================================
// operator = is a deep copy
//===========================================================================================
GridCollectionFiniteVolumeOperators & GridCollectionFiniteVolumeOperators::
operator= ( const GridCollectionFiniteVolumeOperators & gco )
{
  mappedGridOperators       =gco.mappedGridOperators; // this is shallow, fix! ********
  mappedGridOperatorsPointer=gco.mappedGridOperatorsPointer;
  
  mappedGridOperatorWasNewed=gco.mappedGridOperatorWasNewed;
  numberOfComponentGrids    =gco.numberOfComponentGrids;
  numberOfGrids             =gco.numberOfGrids;
  numberOfDimensions        =gco.numberOfDimensions;
  isVolumeScaled            =gco.isVolumeScaled;
  
  return *this;
}

GenericGridCollectionOperators & GridCollectionFiniteVolumeOperators::
operator= ( const GenericGridCollectionOperators & gco )
{
  if( this )
  {
    cout << "GridCollectionFiniteVolumeOperators::operator= :ERROR: operator= ( const GenericGridCollectionOperators & gco )"
            "  called\n";
    throw "error";
  }
  return *this;
}

/* WE DON'T NEED THIS AT ALL

//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{put}}
void GridCollectionFiniteVolumeOperators::
put( const Dir & dir, const aString & name)
//==================================================================================
// /Description:
//   output onto a database file
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex} 
//==================================================================================
{
  // destory the directory if it exists
  if( !dir.locateDir(name).isNull() )
    dir.destroy(name, " R");
  Dir subDir = name=="." ? dir : dir.createDir(name);

//  char buff[40];
//  subDir.put( numberOfComponents,"numberOfComponents" );
//  subDir.put( positionOfComponent,"positionOfComponent" );
//  subDir.put( rcData->numberOfNames,"numberOfNames" );
//  for( int i=0; i<rcData->numberOfNames; i++ )
//    subDir.put( rcData->name[i],sprintf(buff,"name[%i]",i) );
//  subDir.put( *this,"arrayData" );  // save the A++ array
}

*/


//==============================================================================
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{setIsVolumeScaled}} 
void GridCollectionFiniteVolumeOperators::
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
//==============================================================================
  { 
    isVolumeScaled = trueOrFalse;
    int grid;
    ForAllGrids (grid){
      ((MappedGridFiniteVolumeOperators &)mappedGridOperators[grid]).setIsVolumeScaled(trueOrFalse);
    }
  }
//==============================================================================
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{getIsVolumeScaled}} 
bool GridCollectionFiniteVolumeOperators::
getIsVolumeScaled () 
//
// /Purpose: determine whether GridCollectionFiniteVolumeOperators function will
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
//==============================================================================
  {
    return (isVolumeScaled);
  }


// --------------------Boundary Condition Routines --------------------------------





void GridCollectionFiniteVolumeOperators::
//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{setNumberOfBoundaryConditions}}  
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
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}
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


//\begin{>>GridCollectionFiniteVolumeOperatorsInclude.tex}{\subsubsection{setBoundaryCondition (using defaults)}}  
void GridCollectionFiniteVolumeOperators:: 
setBoundaryCondition(const MappedGridFiniteVolumeOperators::boundaryConditionTypes & boundaryConditionType )
//=======================================================================================
// /Description:
//   Define what a given boundary condition should be.
//   This version assigns the given boundaryConditionType to all boundaries and all components
// /boundaryConditionType: the type of boundary condition
//
//\end{GridCollectionFiniteVolumeOperatorsInclude.tex}
//=======================================================================================
{
//  int numberOfGrids = gridCollection.numberOfComponentGrids();
  
  for( int grid=0; grid<numberOfGrids; grid++ )
    ((MappedGridFiniteVolumeOperators&)mappedGridOperators[grid]).setBoundaryCondition(boundaryConditionType);
}

void GridCollectionFiniteVolumeOperators:: 
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
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{setBoundaryConditionRightHandSide}} 
void GridCollectionFiniteVolumeOperators::
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
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
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{applyBoundaryConditions}} 
void GridCollectionFiniteVolumeOperators::
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
//========================================
{
  int grid;
  ForAllGrids (grid) {
    mappedGridOperators[grid].applyBoundaryConditions(u[grid],time);
    CGDisplay.display (u[grid], "GridCollectionFiniteVolumeOperators::applyBoundaryConditions: u[grid]");
  }
}
*/
 
void GridCollectionFiniteVolumeOperators::
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{applyBoundaryConditionsToCoefficients}} 
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
//==============================================================================
{
  int grid;
  ForAllGrids (grid) ((MappedGridFiniteVolumeOperators&) mappedGridOperators[grid]).applyBoundaryConditionsToCoefficients(coeff[grid],time);
}
//==============================================================================
//\begin{>>GridCollectionFiniteVolumeOperators.tex}{\subsection{applyRightHandSideBoundaryConditions}} 
void GridCollectionFiniteVolumeOperators::
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
//\end{GridCollectionFiniteVolumeOperators.tex} 
//==============================================================================			
{
  int grid;
  ForAllGrids (grid) ((MappedGridFiniteVolumeOperators&) mappedGridOperators[grid]).applyRightHandSideBoundaryConditions(rightHandSide[grid]);
}



