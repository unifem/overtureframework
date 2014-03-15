#include "GridCollectionOperators.h"

//
//          *** here are some comments ****
//
//\begin{>GridCollectionOperatorsInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
//
//
//\end{GridCollectionOperatorsInclude.tex}



//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{Constructors}}  
GridCollectionOperators::
GridCollectionOperators()
//===========================================================================================
//\end{GridCollectionOperatorsInclude.tex}
//===========================================================================================
{
  setup();
  mappedGridOperatorsPointer=new MappedGridOperators();   // remember to delete this!
  mappedGridOperatorWasNewed=TRUE;
}

//\begin{>>GridCollectionOperatorsInclude.tex}{}
GridCollectionOperators::
GridCollectionOperators( GridCollection & gridCollection0 )
//=======================================================================================
// /Description:
//   Construct a GridCollectionOperators
// /gridCollection0 (input): Associate this grid with the operators.
// /Author: WDH
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  mappedGridOperatorsPointer=new MappedGridOperators();   // remember to delete this!
  mappedGridOperatorWasNewed=TRUE;
  updateToMatchGrid( gridCollection0 );
  setup();
}

//\begin{>>GridCollectionOperatorsInclude.tex}{}
GridCollectionOperators::
GridCollectionOperators( MappedGridOperators & op )
//=======================================================================================
// /Description:
//   Construct a GridCollectionOperators using a MappedGridOperators
// /op (input): Associate this grid with these operators.
// /Author: WDH
//\end{GridCollectionOperatorsInclude.tex}
//=======================================================================================
{
  setup();
  mappedGridOperatorsPointer=&op;
}

//===========================================================================================
// This constructor takes a grid collection and a MappedGridOperators as input
//===========================================================================================
GridCollectionOperators::
GridCollectionOperators( GridCollection & gridCollection0, MappedGridOperators & op)
{
  mappedGridOperatorsPointer=&op;
  updateToMatchGrid( gridCollection0 );
  setup();
}


//===========================================================================================
//  Copy constructor 
//   deep copy
//===========================================================================================
GridCollectionOperators::
GridCollectionOperators( const GridCollectionOperators & gco ) 
{
  *this=gco;   // this uses the = operator which is a deep copy
}

GridCollectionOperators::
~GridCollectionOperators()
{
  if( mappedGridOperatorWasNewed )
    delete mappedGridOperatorsPointer;  // ** only delete if it was newed ***

}

// **** this is not realyy needed here - could use base class version *****
void GridCollectionOperators::
updateToMatchGrid( GridCollection & gc )              
{
  GenericGridCollectionOperators::updateToMatchGrid(gc); 
}


//================================================================================
// return the MappedGridOperators object for  MappedGrid "grid"
//================================================================================
MappedGridOperators & GridCollectionOperators::
operator[]( const int grid )  const
{
  if( grid<0 || grid>=mappedGridOperators.getLength() )
  {
    cout << "GridCollectionOperators:ERROR in operator[]  argument grid is invalid" << endl;
    cout << "grid = " << grid << endl;
    cout << "mappedGridOperators.getLength() = " << mappedGridOperators.getLength() << endl;
    cout << "Perhaps you forgot to associate a GridCollection with the GridCollectionOperators object\n";
    cout << "Do this in the constructor of GridCollectionOperators or use updateToMatchGrid\n";
    Overture::abort("This is a fatal error");
  }    
  return (MappedGridOperators&) mappedGridOperators[grid];
}

//=======================================================================================
//   virtualConstructor
//
//  This routine should create a new object of this class and return as a pointer
//  to the base classGridCollectionOperators.
//
//  Notes:
//   o This routine is needed if this class has been derived from the base class GridCollectionOperators
//   o This routine is used by the classe MultigridCompositeGridOperators
//     in order to construct lists of this class. These classes only know about the base class
//     and so they are unable to create a "new" version of this class
//=======================================================================================
GenericGridCollectionOperators* GridCollectionOperators::
virtualConstructor()
{
  return new GridCollectionOperators();
}

/* -----  Here are some comments -----
//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
GridCollectionFunction 
"derivative"(const realGridCollectionFunction & u,
           const Index & N  // =nullIndex 
         )
//==================================================================================
// /Description:
//   "derivative" equals one of x, y, z, xx, xy, xz, yy, yz, zz, laplacian, grad, div.
// /u (input): Take the derivative of this grid function.
// /N (input): evaluate the derivatives for these components.
// /return Value:
//   The derivative. 
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by N
//  (if N is not specified then the result will have the same number of components 
// of the grid function being differentiated). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================

//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{Derivative coefficients}}
GridCollectionFunction 
"derivativeCoefficients"(const Index & N // =nullIndex )
//==================================================================================
// /Description:
//   "derivativeCoefficients" equals one of xCoefficients, yCoefficients, zCoefficients, 
//   xxCoefficients, xyCoefficients, xzCoefficients, yyCoefficients, yzCoefficients, zzCoefficients,
//    laplacianCoefficients, gradCoefficients, divCoefficients.
//   Compute the coefficients of the specified derivative.
// /N (input): evaluate the coefficients for these components.
// /return Value:
//   The derivative coefficients.
//\end{GridCollectionFunctionInclude.tex} 
//==================================================================================
 ----------------- */



//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{get}}
int GridCollectionOperators::
get( const GenericDataBase & dir, const aString & name)
//-------------------------------------------------------------------
// /Description:
//   Get from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GridCollectionOperatorsInclude.tex}{}
//-------------------------------------------------------------------
{
  cout << "GridCollectionOperators::get - not implemented yet!\n"; 
  return 1;
}




//===========================================================================================
// operator = is a deep copy
//===========================================================================================
GridCollectionOperators & GridCollectionOperators::
operator= ( const GridCollectionOperators & gco )
{
  if( this  )
  {
    cout << "GridCollectionOperators::operator= :ERROR: not implemented yet. You can maybe use the "
            "updateToMatchGrid function.\n";
    Overture::abort("error");
  }
  mappedGridOperators       =gco.mappedGridOperators; // this is shallow, fix! ********
  mappedGridOperatorsPointer=gco.mappedGridOperatorsPointer;
  return *this;
}

GenericGridCollectionOperators & GridCollectionOperators::
operator= ( const GenericGridCollectionOperators & gco )
{
  if( this )
  {
    cout << "GridCollectionOperators::operator= :ERROR: operator= ( const GenericGridCollectionOperators & gco )"
            "  called\n";
    Overture::abort("error");
  }
  return *this;
}


//\begin{>>GridCollectionOperatorsInclude.tex}{\subsubsection{put}}
int GridCollectionOperators::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   output onto a database file
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{GridCollectionOperatorsInclude.tex} 
//==================================================================================
{
  cout << " GridCollectionOperators::put - not implemented yet!\n"; 
  return 0;
}


// --------------------Boundary Condition Routines --------------------------------


// void GridCollectionOperators::
// setNumberOfBoundaryConditions(const int & number, 
// 			      const int & side,  // =forAll
// 			      const int & axis,  // =forAll
// 			      const int & grid0  // =forAll
//                              )
// //=======================================================================================
// // /Description:
// //  Indicate how many boundary conditions are to be applied on a given side
// // /number (input): specify this number of boundary conditions.
// // /side, axis (input): defines the side of the grid, by default do all sides and all axes.
// //
// //=======================================================================================
// {
//   if( grid0==forAll )
//   {
//     for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
//       ((MappedGridOperators&)mappedGridOperators[grid]).setNumberOfBoundaryConditions(number,side,axis);
//   }
//   else
//   {
//     ((MappedGridOperators&)mappedGridOperators[grid0]).setNumberOfBoundaryConditions(number,side,axis);
//   }
// }


// void GridCollectionOperators:: 
// setBoundaryCondition(const MappedGridOperators::boundaryConditionTypes & boundaryConditionType )
// //=======================================================================================
// // /Description:
// //   Define what a given boundary condition should be.
// //   This version assigns the given boundaryConditionType to all boundaries and all components
// // /boundaryConditionType: the type of boundary condition
// //
// //=======================================================================================
// {
//   for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
//     ((MappedGridOperators&)mappedGridOperators[grid]).setBoundaryCondition(boundaryConditionType);
// }

// void GridCollectionOperators:: 
// setBoundaryConditionValue(const real & value,  
// 			  const int & component,   // ***** get rid of this argument *****
// 			  const int & index, 
// 			  const int & side, 
// 			  const int & axis, 
// 			  const int & grid0)
// {
//   if( grid0==forAll )
//   {
//     for( int grid=0; grid<gridCollection.numberOfGrids(); grid++ )
//       ((MappedGridOperators&)mappedGridOperators[grid]).setBoundaryConditionValue(value,index,side,axis);
//   }
//   else
//   {
//     ((MappedGridOperators&)mappedGridOperators[grid0]).setBoundaryConditionValue(value,index,side,axis);
//   }
// }

real GridCollectionOperators::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object  
//\end{GenericGridCollectionOperatorsInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);
  size+=GenericGridCollectionOperators::sizeOf()-sizeof(GenericGridCollectionOperators);
  int grid;
  for( grid=0; grid<gridCollection.numberOfGrids(); grid++ )
    size+=(*this)[grid].sizeOf(file);

    
  return size;
}
