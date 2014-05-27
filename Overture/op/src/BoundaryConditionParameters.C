#include "Overture.h"
#include "BoundaryConditionParameters.h"
#include "GridCollectionFunction.h"
#include "MappedGridFunction.h"
#include "GridCollection.h"

//\begin{>BoundaryConditionParametersInclude.tex}{\subsubsection{constructor}}  
BoundaryConditionParameters:: 
BoundaryConditionParameters()
// ===================================================================================
// /Description:
//   This class is used to pass optional parameters to the boundary condition routines.
// 
// /Optional parameters: The following parameters are public members of this class:
// \begin{description}
//   \item[int lineToAssign:] apply Dirichlet BC on this line.
//   \item[int orderOfExtrapolation:] order of extrapolation for various BC's. A value < 0 means
//      use orderOfExtrapolation=3 for 2nd-order accuracy and orderOfExtrapolation=5 for fourth order
//   \item[int orderOfInterpolation:] not used yet(?)
//   \item[int ghostLineToAssign:] assign this ghost line (various bc's)
//   \item[extraInTangentialDirections:] extend the set of pointts assigned by this many points
//                 in the tangential directions
//   \item[numberOfCornerGhostLinesToAssign:] assign at most this many lines at edges and corners, by default 
//       do all.
//       For a second order method that only uses one ghost line one could set this value to 1 to avoid
//       assigning any unused ghost points. NOTE: Some BC's may still assign all ghost points, this is
//       only used as a recommendation.
//   \item[cornerExtrapolationOption:]  by default (=0) corner points are extrapolated along diagonals.
//         Setting this parameter to 1,2 or 3 means corner points are not extrapolated along direction 1,2, or 3.
//         This option was introduced to keep some symmetries in 3d computations.
//   \item[IntegerArray components:]  holds components to assign for various BC's
//   \item[IntegerArray uComponents,fComponents:] holds components to assign for various BC's
//   \item[RealArray a,b0,b1,b2,b3:] hold parameters for various BC's
//   \item[int useMask] : if TRUE use the mask (below) to determine where boundary conditions should be applied.
//   \item[IntegerArray mask] : supply a mask array to indicate where the BC's should be applied. This
//      array is only used if useMask=TRUE. 
// \end{description}
//
// /Example:
//   This example shows how to extrapolate to order 4:
//   \begin{verbatim}
//       BoundaryConditionParameters bcParams;
//       bcParams.orderOfExtrapolation=4;
//       ...
//       int wall=3;       
//       real value=0., time=0.;
//       u.applyBoundaryCondition(0,BCTypes::extrapolate,wall,value,time,bcParams);
//       ....
//   \end{verbatim} 
//     
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  orderOfExtrapolation=-1;   // This means use 3 for 2nd-order accuracy and 5 for fourth order
  orderOfInterpolation=2;
  lineToAssign=0;
  ghostLineToAssign=1;
  extraInTangentialDirections=0;  // e.g. set to 1 if you want a dirichlet BC to be applied to the boundary pts extended by 1

  numberOfCornerGhostLinesToAssign=INT_MAX; // assign at most this many lines at edges and corners, by default do all

  // for extrapolating corners along given directions instead of the diagonal
  //    1= do not extrapolate in axis1 direction
  //    2= do not extrapolate in axis2 direction
  //    3= do not extrapolate in axis3 direction
  cornerExtrapolationOption=0; 

  refinementLevelToSolveFor=-1;
  
  for( int s3=0; s3<=2; s3++ )
    for( int s2=0; s2<=2; s2++ )
      for( int s1=0; s1<=2; s1++ )
	cornerBC[s1][s2][s3]=extrapolateCorner;
  
  vectorSymmetryCornerComponent=0;  // indicates where the "vector" starts for the vector symmetry corner BC
  
  variableCoefficientsArray=0;
  
  variableCoefficients=0;
  variableCoefficientsGC=0;
  useMask=0;
  maskPointer=0;

  useMixedBoundaryMask=true;  // by default we use the mask on mixed-boundaries 
  
  interpolateRefinementBoundaries=true;  // if true, interpolate all refinement boundaries
  interpolateHidden=true; 

  boundaryConditionForcingOption=unSpecifiedForcing; 
  variableCoefficientOption=spatiallyConstantCoefficients;  // coeff's do not vary by default

  extrapolationOption=polynomialExtrapolation;
  extrapolateWithLimiterParameters[0]=1.;  // coefficient for extrapolation limiter
  extrapolateWithLimiterParameters[1]=0.;  // coefficient for extrapolation limiter
  
}


BoundaryConditionParameters:: 
~BoundaryConditionParameters()
{
  delete variableCoefficients;
  delete variableCoefficientsGC;  // *wdh* added 2011/09/03
  delete maskPointer;

  if( variableCoefficientsArray!=NULL )
  {
    variableCoefficientsArray->decrementReferenceCount();
    if( variableCoefficientsArray->getReferenceCount()==0 )
      delete variableCoefficientsArray;
  }
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setCornerBoundaryCondition}} 
int  
BoundaryConditionParameters::
setCornerBoundaryCondition( CornerBoundaryConditionEnum bc )
// ==================================================================================================
// /Description:
//    Specify the boundary conditions for the corners and edges.
// /bc (input) : use this boundary condition on all corners and edges.
// 
// /Notes:
//   For a vectorSymmetryCorner, use setVectorSymmetryCornerComponent( component  )
//  to indicate which components form the "vector" for the vector symmetry corner BC 
//  (e.g. where the velocity components start in the list of components)
//  In 3D for example the vector symmetry will be applied to the
//  set of components: [component,component+1,component+2] with all other components
//  set by even symmetry
// 
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  for( int s3=0; s3<=2; s3++ )
    for( int s2=0; s2<=2; s2++ )
      for( int s1=0; s1<=2; s1++ )
	cornerBC[s1][s2][s3]=bc;
  return 0;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setCornerBoundaryCondition}} 
int  
BoundaryConditionParameters::
setCornerBoundaryCondition( CornerBoundaryConditionEnum bc, int side1, int side2, int side3 /* = -1 */ )
// ==================================================================================================
// /Description:
//    Specify the boundary conditions for the corners and edges.
// /bc (input) : use this boundary condition on the specified corner or edge.
// /side1,side2,side3 (input): To indicate a corner, each of side1,side2, and side3 should be either 0 or 1;
//    the corner will then be  $(r_1=side1,r_2=side2,r_3=side3)$. To indicate an edge set one of
//   side1,side2,side3 to be $-1$ and the others to be 0 or 1. If side1==-1 then the edge will be
//   parallel to axis1 : $(r_1=[0,1],r_2=side2,r_3=side3)$. if side2==-1 then the edge will be 
//   parallel to axis2 : $(r_1=side,r_2=[0,1],r_3=side3)$ etc.
// 
// /Notes:
//   For a vectorSymmetryCorner, use setVectorSymmetryCornerComponent( component  )
//  to indicate which components form the "vector" for the vector symmetry corner BC 
//  (e.g. where the velocity components start in the list of components)
//  In 3D for example the vector symmetry will be applied to the
//  set of components: [component,component+1,component+2] with all other components
//  set by even symmetry
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  const int s1 = (side1==0 || side1==1) ? side1 : 2;
  const int s2 = (side2==0 || side2==1) ? side2 : 2;
  const int s3 = (side3==0 || side3==1) ? side3 : 2;
  
  if( (s1==2) + (s2==2) + (s3==2) >1 )
  {
    printf("BoundaryConditionParameters::setCornerBoundaryCondition:ERROR: invalid (side1,side2,side3)=(%i,%i,%i)\n"
           "  At most one of these should be -1, all others should be 0 or 1\n",
           side1,side2,side3);
    Overture::abort("error");
  }
  
  cornerBC[s1][s2][s3]=bc;
  return 0;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{cornerBoundaryCondition}} 
BoundaryConditionParameters::CornerBoundaryConditionEnum BoundaryConditionParameters::
getCornerBoundaryCondition( int side1, int side2, int side3 /* = -1 */ ) const
// ==================================================================================================
// /Description:
//   Return the boundary condition that applies to a corner or edge.  
// 
//  /side1,side2,side3 : Values of (0,0,0) would be a corner, (0,0,-1) would be an edge
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  const int s1 = (side1==0 || side1==1) ? side1 : 2;
  const int s2 = (side2==0 || side2==1) ? side2 : 2;
  const int s3 = (side3==0 || side3==1) ? side3 : 2;
  
  if( (s1==2) + (s2==2) + (s3==2) >1 )
  {
    printf("BoundaryConditionParameters::getCornerBoundaryCondition:ERROR: invalid (side1,side2,side3)=(%i,%i,%i)\n"
           "  At most one of these should be -1, all others should be 0 or 1\n",
           side1,side2,side3);
    Overture::abort("error");
  }

  return cornerBC[s1][s2][s3];
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setVectorSymmetryCornerComponent}}    
int BoundaryConditionParameters::
setVectorSymmetryCornerComponent( int component  )
// ===================================================================================
// /Description:
// Indicate which components form the "vector" for the vector symmetry corner BC 
// (e.g. where the velocity components start in the list of components)
// In 3D for example the vector symmetry will be applied to the
// set of components: [component,component+1,component+2] with all other components
// set by even symmetry
//
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  vectorSymmetryCornerComponent=component;
  return 0;
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getVectorSymmetryCornerComponent}} 
int BoundaryConditionParameters::
getVectorSymmetryCornerComponent() const
// ===================================================================================
// /Description:
//   Return the component that indicates the first component of the "vector" 
//   for the vector symmetry corner BC 
//
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  return vectorSymmetryCornerComponent;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setUseMask}}    
int BoundaryConditionParameters::
setUseMask(int trueOrFalse /* =TRUE */ )
// ===================================================================================
// /Description:
//   Turn on (or off) the use of the mask array for selectively applying boundary conditions
//  at certain points.
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  useMask=trueOrFalse;
  return 0;
}

/* ---- this is inlined
//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getUseMask}}    
int BoundaryConditionParameters::
getUseMask() const
// ===================================================================================
// /Description:
//   Return the current value of the useMask flag.
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  return useMask;
}
--- */

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{mask()}}    
intArray & BoundaryConditionParameters::
mask() 
// ===================================================================================
// /Description:
//   Return a reference to the boundary condition mask array. It is up to the user to dimension this array
// to be the correct size. 
// 
// If setUseMask(true) has been called then any boundary 
// condition will only be applied where the mask array has non-zero values.
//
// The applyBoundaryCondition routine will evaluate the mask on a given side according
// to the value of bcParameters.lineToAssign, by default this will be the boundary itself.
// \begin{verbatim}
//     getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign);
//     where( mask(I1,I2,I3) )
//         apply the boundary condition
// \end{verbatim}
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( maskPointer==0 )
    maskPointer=new intArray;

  return *maskPointer;
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{useMixedBoundaryMask}}  
int BoundaryConditionParameters::
assignAllPointsOnMixedBoundaries( bool trueOrFalse /* =true */ )
// ===================================================================================
// /Description:
// Boundary conditions on mixed boundaries are normally NOT assigned at interior boundary points,
//  unless you call this function with "true"
// /trueOrFalse : set to true if you want all points to be assigned on mixed boundaries (boundaries
//     that are partially boundary points and partially interpolation points). The default is false. 
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  useMixedBoundaryMask= !trueOrFalse;
  return 0;
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getVariableCoefficientsArray}}  
realSerialArray* BoundaryConditionParameters::
getVariableCoefficientsArray() const
// ===================================================================================
// /Description:
//    Return a pointer to the array that was previously supplied through a call
// to {\tt setVariableCoefficientsArray}. If this point is not NULL then use it for
// as the variable coefficents for a given BC.
// 
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  return variableCoefficientsArray;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getVariableCoefficients}}  
RealMappedGridFunction* BoundaryConditionParameters::
getVariableCoefficients() const
// ===================================================================================
// /Description:
//    Return a pointer to the grid function that was previously supplied through a call
// to {\tt setVariableCoefficients(  RealMappedGridFunction \& var )}. Do not use this
// version if you initially passed a grid collection function.
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( variableCoefficients==0 )
  {
    printf("BoundaryConditionParameters::getVariableCoefficients:ERROR: variableCoefficients has not been set\n");
    if( variableCoefficientsGC!=0 )
    {
      printf("   A variable coefficient GridCollectionFunction has been assigned. Maybe you meant to call\n"
             "   getVariableCoefficients(const int & grid) with an appropriate grid number\n");
      Overture::abort("error");
    }
  }
  return variableCoefficients;
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getVariableCoefficients}}  
RealMappedGridFunction* BoundaryConditionParameters::
getVariableCoefficients(const int & grid) const
// ===================================================================================
// /Description:
//    Return a pointer to the grid function that was previously supplied through a call
// to {\tt setVariableCoefficients(  RealGridCollectionFunction \& var )}. 
// /grid (input) : return the mappedGridFunction for this component grid.
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( variableCoefficientsGC!=0 )
  {
    if( grid<0 )
    {
      printf("BoundaryConditionParameters::getVariableCoefficients(const int & grid):ERROR grid<0 \n");
      Overture::abort("error");
    }
    else if( grid > (*variableCoefficientsGC).getGridCollection()->numberOfGrids() )
    {
      printf("BoundaryConditionParameters::getVariableCoefficients(const int & grid):ERROR the value for grid=%i\n"
             " is large than the number of grids in the variableCoefficient grid function = %i \n",
             grid,(*variableCoefficientsGC).getGridCollection()->numberOfGrids());
      Overture::abort("error");
    }
    else
      return &( (*variableCoefficientsGC)[grid] );
  }
  else if( grid==0 )
    return variableCoefficients;
  return 0;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setVariableCoefficientsArray}}  
int BoundaryConditionParameters::
setVariableCoefficientsArray( realSerialArray *var /* =NULL */ )
// ===================================================================================
// /Description:
//   Supply a pointer to an array to use for variable coefficient boundary conditions. 
//   Set to NULL to turn off the variable coefficients.
//   A reference to the array var will be kept.
// /var (input) : a pointer to an array of coefficient values for a boundary condition that requires variable
//    coefficients (e.g. the mixed BC). This array normally is dimensioned for a 
//    single boundary (i.e. one face of a grid). Pass var=NULL to turn off the variable coefficients.     
//        
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( variableCoefficientsArray!=NULL )
  {
    variableCoefficientsArray->decrementReferenceCount();
    if( variableCoefficientsArray->getReferenceCount()==0 )
      delete variableCoefficientsArray;
  }

  variableCoefficientsArray=var;
  if( variableCoefficientsArray!=NULL )
    variableCoefficientsArray->incrementReferenceCount();

  return 0;
}



//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setVariableCoefficients}}  
void BoundaryConditionParameters::
setVariableCoefficients(  RealMappedGridFunction & var )
// ===================================================================================
// /Description:
//   Supply a grid function for variable coefficients. The meaning of the grid function
//  depends on the boundary condition to which it is applied. A reference to `var'
//  will be kept.
// /var (input) : coefficient values for a boundary condition that requires variable
//    coefficients. This grid function could only live on a single boundary if there
//      is only one boundary where the values are needed.
//        
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( variableCoefficients==0 )
    variableCoefficients = new RealMappedGridFunction;
  variableCoefficients->reference(var);
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setVariableCoefficients}}  
void BoundaryConditionParameters::
setVariableCoefficients(  RealGridCollectionFunction & var )
// ===================================================================================
// /Description:
//   Supply a grid function for variable coefficients. The meaning of the grid function
//  depends on the boundary condition to which it is applied. A reference to `var'
//  will be kept. {\bf NOTE:} This grid function will take precedence over any variable coefficients
//  specified through the {\tt setVariableCoefficients( RealMappedGridFunction \& var )}, i.e. 
//  A {\tt GridCollectionFunction} will be used before a {\tt MappedGridFunction}.
// /var (input) : coefficient values for a boundary condition that requires variable
//    coefficients. This grid function could only live on a single boundary if there
//      is only one boundary where the values are needed.
//        
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  if( variableCoefficientsGC==0 )
    variableCoefficientsGC = new RealGridCollectionFunction;
  variableCoefficientsGC->reference(var);
}

//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{setRefinementLevelToSolveFor}} 
void BoundaryConditionParameters::
setRefinementLevelToSolveFor( int level )
// ===================================================================================
// /Description:
// /level (input) : indicate that a particular refinement level is being solved for.
//
//\end{BoundaryConditionParametersInclude.tex}
// ===================================================================================
{
  refinementLevelToSolveFor=level;
}


int BoundaryConditionParameters::
setBoundaryConditionForcingOption( BoundaryConditionForcingOption option )
// ===================================================================================
/// \brief Set the boundary condition forcing option.
/// \param option (input) : specify the form of the right-hand-sde for the boundary condition.
///
// ===================================================================================
{
  boundaryConditionForcingOption=option;
  return 0;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getBoundaryConditionForcingOption}} 
BoundaryConditionParameters::BoundaryConditionForcingOption BoundaryConditionParameters::
getBoundaryConditionForcingOption() const
// ===================================================================================
/// \brief: Return the boundary condition forcing option.
/// \return : the form of the right-hand-sde for the boundary condition.
//
// ===================================================================================
{
  return boundaryConditionForcingOption;
}


int BoundaryConditionParameters::
setVariableCoefficientOption( VariableCoefficientOptionEnum option )
// ===================================================================================
/// \brief Set the variable coefficient option for boundary condition.
/// \param option (input) : coefficient option to use.
///
// ===================================================================================
{
  variableCoefficientOption=option;
  return 0;
}


//\begin{>>BoundaryConditionParametersInclude.tex}{\subsubsection{getBoundaryConditionForcingOption}} 
BoundaryConditionParameters::VariableCoefficientOptionEnum BoundaryConditionParameters::
getVariableCoefficientOption() const
// ===================================================================================
/// \brief: Return the variable coefficient option for boundary condition.
/// \return : variable coefficient option for boundary condition.
//
// ===================================================================================
{
  return variableCoefficientOption;
}

