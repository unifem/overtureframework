#include "GenericMappedGridOperators.h"
#include "SparseRep.h"
#include "GridFunctionParameters.h"
#include "display.h"
#include "ParallelUtility.h"
#include "AssignInterpNeighbours.h"

// extern realMappedGridFunction Overture::nullDoubleMappedGridFunction();
// extern realMappedGridFunction Overture::nullFloatMappedGridFunction();
// #ifdef OV_USE_DOUBLE
// define NULLRealMappedGridFunction Overture::nullDoubleMappedGridFunction()
// else
// define NULLRealMappedGridFunction Overture::nullFloatMappedGridFunction()
// endif

int GenericMappedGridOperators::defaultMaximumWidthToExtrapolationInterpolationNeighbours=4;
real GenericMappedGridOperators::timeForDirichlet=0.;
real GenericMappedGridOperators::timeForNeumann=0.;
real GenericMappedGridOperators::timeForExtrapolate=0.;
real GenericMappedGridOperators::timeForNormalComponent=0.;
real GenericMappedGridOperators::timeForGeneralMixedDerivative=0.;
real GenericMappedGridOperators::timeForNormalDerivative=0.;
real GenericMappedGridOperators::timeForNormalDerivativeOfADotU=0.;
real GenericMappedGridOperators::timeForADotU=0.;
real GenericMappedGridOperators::timeForADotGradU=0.;
real GenericMappedGridOperators::timeForNormalDotScalarGrad=0.;
real GenericMappedGridOperators::timeForSymmetry=0.;
real GenericMappedGridOperators::timeForGeneralizedDivergence=0.;
real GenericMappedGridOperators::timeForExtrapolateInterpolationNeighbours=0.;
real GenericMappedGridOperators::timeForExtrapolateNormalComponent=0.;
real GenericMappedGridOperators::timeForExtrapolateRefinementBoundaries=0.;
real GenericMappedGridOperators::timeForPeriodicUpdate=0.;
real GenericMappedGridOperators::timeForFixBoundaryCorners=0.;

real GenericMappedGridOperators::timeToSetupBoundaryConditions=0.;
real GenericMappedGridOperators::timeForAllBoundaryConditions=0.;


void GenericMappedGridOperators::
printBoundaryConditionStatistics(FILE *file /* =stdout */)
// =======================================================================
// =======================================================================
{
  real sum=(timeForDirichlet+
	    timeForNeumann+
	    timeForExtrapolate+
	    timeForNormalComponent+
	    timeForGeneralMixedDerivative+
	    timeForNormalDerivative+
	    timeForNormalDerivativeOfADotU+
	    timeForADotU+
	    timeForADotGradU+
	    timeForNormalDotScalarGrad+
	    timeForSymmetry+
	    timeForGeneralizedDivergence+
	    timeForExtrapolateInterpolationNeighbours+
	    timeForExtrapolateNormalComponent+
	    timeForExtrapolateRefinementBoundaries+
	    timeForPeriodicUpdate+
	    timeForFixBoundaryCorners+
	    timeToSetupBoundaryConditions);

  fPrintF(file,
	  " Boundary Condition Timings  \n"
	  " --------------------------  \n"
	  " dirichlet.........................%8.2e \n"
	  " neumann...........................%8.2e \n"
	  " extrapolate.......................%8.2e \n"
	  " normal/tangential component.......%8.2e \n"
	  " general mixed.....................%8.2e \n"
	  " normal derivative.................%8.2e \n"
	  " aDotU.............................%8.2e \n"
	  " aDotGradU.........................%8.2e \n"
	  " normalDotScalarGrad...............%8.2e \n"
	  " symmetry..........................%8.2e \n"
	  " generalized divergence............%8.2e \n"
	  " extrap interpolation neighbours...%8.2e \n"
	  " extrap normal/tangential .........%8.2e \n"
	  " extrap refinement boundaries......%8.2e \n"
          " periodic update...................%8.2e \n"
          " fix boundary corners..............%8.2e \n"
	  " \n"
	  " time for setup....................%8.2e \n"
	  " sum of the above..................%8.2e \n"
	  " total.............................%8.2e \n",
	  timeForDirichlet,
	  timeForNeumann,
	  timeForExtrapolate,
	  timeForNormalComponent,
	  timeForGeneralMixedDerivative,
	  timeForNormalDerivative,
//	  timeForNormalDerivativeOfADotU,
	  timeForADotU,
	  timeForADotGradU,
	  timeForNormalDotScalarGrad,
	  timeForSymmetry,
	  timeForGeneralizedDivergence,
	  timeForExtrapolateInterpolationNeighbours,
	  timeForExtrapolateNormalComponent,
	  timeForExtrapolateRefinementBoundaries,
	  timeForPeriodicUpdate,
	  timeForFixBoundaryCorners,
	  timeToSetupBoundaryConditions,
          sum,
	  timeForAllBoundaryConditions);
}



//===========================================================================================
// This class is the base class for MappedGridOperators
//
//  o To define the derivatives in a different way you should derive from this class
//    and redefine any functions that you want to. If you provide the virtualConstructor
//    member function then your derived class can be used by the GridCollectionOperators
//    and CompositeGridOperators classes which define derivatives for GridCollectionFunction's
//    and CompositeGridFunction's.
//
//  Who to blame: Bill Henshaw, CIC-3, henshaw@lanl.gov
//  Date of last revision: 95/04/05
//===========================================================================================


static void
throwErrorMessage( const aString & routineName )
{
  cout << "GenericMappedGridFunction::ERROR:base class function `" << routineName << "' called! \n" ; 
  cout << "This function is apparently not implemented\n";
  Overture::abort("GenericMappedGridFunction::ERROR:base class function called! " ); 
}

  // ********************************************************************
  // **************** Miscellaneous Functions **************************
  // ********************************************************************

  // default constructor
GenericMappedGridOperators::
GenericMappedGridOperators()
{
  orderOfAccuracy=2;
  stencilSize=10;                            
  numberOfComponentsForCoefficients=1;      
  twilightZoneFlow=FALSE;
  twilightZoneFlowFunction=NULL;
  conservative=FALSE;
  averagingType=arithmeticAverage;

  // maximum width of extrapolation formula
  maximumWidthToExtrapolationInterpolationNeighbours=
                     defaultMaximumWidthToExtrapolationInterpolationNeighbours; 
  // *wdh* 091123 : OLD way: --------------------------------------
  extrapolateInterpolationNeighbourPoints=NULL;
  extrapolateInterpolationNeighboursDirection=NULL;
  extrapolateInterpolationNeighboursVariableWidth=NULL;
  //  *wdh* 091123 : new way:  -----------------------------------
  assignInterpNeighbours=NULL;

  interpolationPoint=NULL;
  errorStatus=noErrors;
  
}

// contructor taking a MappedGrid
GenericMappedGridOperators::
GenericMappedGridOperators( MappedGrid & mg )
{
  orderOfAccuracy=2;
  stencilSize=10;                            
  numberOfComponentsForCoefficients=1;      
  twilightZoneFlow=FALSE;
  twilightZoneFlowFunction=NULL;
  conservative=FALSE;
  averagingType=arithmeticAverage;

  maximumWidthToExtrapolationInterpolationNeighbours=4;

  // *wdh* 091123 : OLD way: --------------------------------------
  extrapolateInterpolationNeighbourPoints=NULL;
  extrapolateInterpolationNeighboursDirection=NULL;
  extrapolateInterpolationNeighboursVariableWidth=NULL;
  //  *wdh* 091123 : new way:  -----------------------------------
  assignInterpNeighbours=NULL;
  
  interpolationPoint=NULL;

  mappedGrid.reference(mg);
}

  // copy constructor
GenericMappedGridOperators::
GenericMappedGridOperators( const GenericMappedGridOperators & mgo )
{
  *this=mgo;
}
// create a new object of this class
GenericMappedGridOperators* GenericMappedGridOperators::
virtualConstructor() const
{
  throwErrorMessage("virtualConstructor, this should not be called!");
  GenericMappedGridOperators *gmgop = new GenericMappedGridOperators;
  return gmgop;
}

GenericMappedGridOperators::
~GenericMappedGridOperators()
{
  // *wdh* 091123 old way: 
  delete extrapolateInterpolationNeighbourPoints;
  delete extrapolateInterpolationNeighboursDirection;
  delete extrapolateInterpolationNeighboursVariableWidth;
  // *wdh* new ay
  delete assignInterpNeighbours;
}

GenericMappedGridOperators & GenericMappedGridOperators::
operator= ( const GenericMappedGridOperators & mgo )
{
  mappedGrid=mgo.mappedGrid;
  orderOfAccuracy=mgo.orderOfAccuracy;
  stencilSize=mgo.stencilSize;
  numberOfComponentsForCoefficients=mgo.numberOfComponentsForCoefficients;
  twilightZoneFlow=mgo.twilightZoneFlow;
  twilightZoneFlowFunction=mgo.twilightZoneFlowFunction;
  conservative=mgo.conservative;
  averagingType=mgo.averagingType;
  
  interpolationPoint=mgo.interpolationPoint;

  // *wdh* 091123 old way:  --------------------------------------------
  delete extrapolateInterpolationNeighbourPoints;
  delete extrapolateInterpolationNeighboursDirection;
  
  extrapolateInterpolationNeighboursIsInitialized=mgo.extrapolateInterpolationNeighboursIsInitialized;
  if( mgo.extrapolateInterpolationNeighbourPoints!=NULL )
  {
    extrapolateInterpolationNeighbourPoints = new IntegerArray(*mgo.extrapolateInterpolationNeighbourPoints);
    extrapolateInterpolationNeighboursDirection=new IntegerArray(*mgo.extrapolateInterpolationNeighboursDirection);
  }
  else
  {
    extrapolateInterpolationNeighbourPoints=NULL;
    extrapolateInterpolationNeighboursDirection=NULL;
  }
  if( mgo.extrapolateInterpolationNeighboursVariableWidth!=NULL )
  {
    extrapolateInterpolationNeighboursVariableWidth=new IntegerArray(*mgo.extrapolateInterpolationNeighboursVariableWidth);
  }
  else
  {
    extrapolateInterpolationNeighboursVariableWidth=NULL;
  }
  // *wdh* 091123 new way
  if( mgo.assignInterpNeighbours!=NULL )
  {
    if( assignInterpNeighbours==NULL )
      assignInterpNeighbours = new AssignInterpNeighbours;
    *assignInterpNeighbours = *mgo.assignInterpNeighbours;
  }
  
  return *this;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{sizeOf}} 
real GenericMappedGridOperators::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object  
//\end{MappedGridOperatorsInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);
  if( extrapolateInterpolationNeighbourPoints!=NULL )
  {
    size+=(extrapolateInterpolationNeighbourPoints->elementCount()+
           extrapolateInterpolationNeighboursDirection->elementCount())*sizeof(int);
  }
  if( extrapolateInterpolationNeighboursVariableWidth!=NULL )
  {
    size+=(extrapolateInterpolationNeighboursVariableWidth->elementCount())*sizeof(int);
  }
  if( assignInterpNeighbours!=NULL )
    size+=assignInterpNeighbours->sizeOf();
  
  return size;
}


// supply a new grid to use
void GenericMappedGridOperators::
updateToMatchGrid( MappedGrid & mg )
{
  mappedGrid.reference(mg);
}
  
int GenericMappedGridOperators::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"GenericMappedGridOperators");

  subDir.get( orderOfAccuracy,"orderOfAccuracy" ); 
  subDir.get( stencilSize,"stencilSize" ); 
  subDir.get( numberOfComponentsForCoefficients,"numberOfComponentsForCoefficients" ); 
  subDir.get( conservative,"conservative" ); 
  // *************** need get/put for twilight zone functions *********
  delete &subDir;
  return 0;
}


int GenericMappedGridOperators::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   output onto a database file
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( orderOfAccuracy,"orderOfAccuracy" ); 
  subDir.put( stencilSize,"stencilSize" ); 
  subDir.put( numberOfComponentsForCoefficients,"numberOfComponentsForCoefficients" ); 
  subDir.put( conservative,"conservative" ); 
  delete &subDir;
  return 0;
}

// set the order of accuracy
void GenericMappedGridOperators::
setOrderOfAccuracy( const int & orderOfAccuracy0 )
{
  orderOfAccuracy=orderOfAccuracy0;
}
  

int GenericMappedGridOperators::
setMaximumWidthForExtrapolateInterpolationNeighbours(const int width /* =4 */)
// Set the maximal allowable width to extrapolation interpolation neighbours
{
  maximumWidthToExtrapolationInterpolationNeighbours=width;
  return 0;
}

int GenericMappedGridOperators::
getMaximumWidthForExtrapolateInterpolationNeighbours() const
{
  return maximumWidthToExtrapolationInterpolationNeighbours;
}

int GenericMappedGridOperators::
setDefaultMaximumWidthForExtrapolateInterpolationNeighbours(const int width /* =4 */)
// Set the default maximal allowable width to extrapolation interpolation neighbours
// This value will be used as the initial value for all subsequent MappedGridOperators that are built
{
  defaultMaximumWidthToExtrapolationInterpolationNeighbours=width;
  return 0;
}

// Indicate the number of components (system size) for functions returning coefficients
void GenericMappedGridOperators::
setNumberOfComponentsForCoefficients(const int number)
{
  numberOfComponentsForCoefficients=number;
}


// This function is used to evaluate a whole set of derivatives at a time (for efficiency)
void GenericMappedGridOperators::
getDerivatives(const realMappedGridFunction & u, 
	       const Index & I1,
	       const Index & I2,
	       const Index & I3,
	       const Index & I4,
	       const Index & Evalute )   // evaluate these
{
  throwErrorMessage("getDerivatives");
}

// Indicate the stencil size for functions returning coefficients
void GenericMappedGridOperators::
setStencilSize(const int stencilSize0)
{
  stencilSize=stencilSize0;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setTwilightZoneFlow}}  
void GenericMappedGridOperators::
setTwilightZoneFlow( const int & twilightZoneFlow_ )
//=======================================================================================
// /Description: Indicate if twilight-zone forcing should be added to boundary conditions
// /twilightZoneFlow\_ (input): if 1 then add the twilight-zone forcing to all boundary
// conditions except for extrapolation. If 2 then also add to extrapolation. 
//   (see also setTwilightZoneFlowFunction and the section on boundary conditions)
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  twilightZoneFlow= twilightZoneFlow_;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setTwilightZoneFlowFunction}}  
void GenericMappedGridOperators::
setTwilightZoneFlowFunction( OGFunction & twilightZoneFlowFunction0 )
//=======================================================================================
// /Description:  Supply a twilight-zone forcing to use for boundary conditions
// /twilightZoneFlowFunction0 (input): use this class for twilight-zone forcing 
//   (see also setTwilightZoneFlow and the section on boundary conditions)
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  twilightZoneFlowFunction=&twilightZoneFlowFunction0;
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{useConservativeApproximations}}  
void GenericMappedGridOperators::
useConservativeApproximations(bool trueOrFalse /* = TRUE */ )
//=======================================================================================
// /Description: 
//    Indicate whether to use the {\sl conservative} approximations to the operators
//  {\tt div}, {\tt laplacian}, {\tt divScalarGrad} and {\tt scalarGrad} and correspoding boundary 
// conditions 
//  /trueOrFalse (input): TRUE means use conservative approximations.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  conservative=trueOrFalse;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{usingConservativeApproximations}}  
bool GenericMappedGridOperators::
usingConservativeApproximations() const
//=======================================================================================
// /Description: 
//    Return TRUE if we are using conservative approximations.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  return conservative;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setAveragingType}}  
void GenericMappedGridOperators::
setAveragingType(const AveragingType & type )
//=======================================================================================
// /Description: 
//    Set the averaging type for certain operators such as {\tt divScalarGrad}. The default
// is {\tt arithmeticAverage}. The {\tt harmonicAverage} is often used for problems
// with discontinuos coefficients. Recall that 
// \begin{align*}
//    \mbox{arithmetic average} ~= {a+b \over 2} \\
//    \mbox{harmonic  average} ~= {2 \over {1\over a} + {1\over b}} ~= {2 a b \over a+ b }
// \end{align*}
// /type (input) : one of {\tt arithmeticAverage} or {\tt harmonicAverage}.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  averagingType=type;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{getAveragingType}}  
GenericMappedGridOperators::AveragingType GenericMappedGridOperators::
getAveragingType() const
//=======================================================================================
// /Description: 
//    Return the current averaging type.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  return averagingType;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{isRectangular}}  
bool GenericMappedGridOperators::
isRectangular()
//=======================================================================================
// /Description: 
//  Return true if the grid is rectangular
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  return FALSE;
}



// ************************************************
// ***** DIFFERENTIATION CLASS FUNCTIONS **********
// ************************************************


// Macro to define a typical function 
#define FUNCTION(type) \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(const realMappedGridFunction & u,  \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}                                                              \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                 \
}

// Macro to define a typical function 
#define FUNCTION_COEFFICIENTS(type) \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(                                   \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}   \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(                                   \
			    const GridFunctionParameters & gfType,   \
                            const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}



// parametric derivatives in the r1,r2,r3 directions
FUNCTION(r1)
FUNCTION_COEFFICIENTS(r1Coefficients)
FUNCTION(r2)
FUNCTION_COEFFICIENTS(r2Coefficients)
FUNCTION(r3)
FUNCTION_COEFFICIENTS(r3Coefficients)
FUNCTION(r1r1)
FUNCTION_COEFFICIENTS(r1r1Coefficients)
FUNCTION(r1r2)
FUNCTION_COEFFICIENTS(r1r2Coefficients)
FUNCTION(r1r3)
FUNCTION_COEFFICIENTS(r1r3Coefficients)
FUNCTION(r2r2)
FUNCTION_COEFFICIENTS(r2r2Coefficients)
FUNCTION(r2r3)
FUNCTION_COEFFICIENTS(r2r3Coefficients)
FUNCTION(r3r3)
FUNCTION_COEFFICIENTS(r3r3Coefficients)

// FUNCTIONs in the x,y,z directions
FUNCTION(x)
FUNCTION_COEFFICIENTS(xCoefficients)
FUNCTION(y)
FUNCTION_COEFFICIENTS(yCoefficients)
FUNCTION(z)
FUNCTION_COEFFICIENTS(zCoefficients)
FUNCTION(xx)
FUNCTION_COEFFICIENTS(xxCoefficients)
FUNCTION(xy)
FUNCTION_COEFFICIENTS(xyCoefficients)
FUNCTION(xz)
FUNCTION_COEFFICIENTS(xzCoefficients)
FUNCTION(yy)
FUNCTION_COEFFICIENTS(yyCoefficients)
FUNCTION(yz)
FUNCTION_COEFFICIENTS(yzCoefficients)
FUNCTION(zz)
FUNCTION_COEFFICIENTS(zzCoefficients)

// other forms of derivatives

// compute face-centered variable from cell-centered variable 
FUNCTION(cellsToFaces)

//compute (u.grad)u (convective derivative)
FUNCTION(convectiveDerivative)

// compute contravariant velocity from either cell-centered or face-centered input velocity
FUNCTION(contravariantVelocity)

FUNCTION(div)
FUNCTION_COEFFICIENTS(divCoefficients)

//returns cell-centered divergence given normal velocities
FUNCTION(divNormal)

// compute faceArea-weighted normal velocity from either cell-centered or 
// face-centered input velocity (this is just an alias for contravariantVelocity)
FUNCTION(normalVelocity)

FUNCTION(grad)
FUNCTION_COEFFICIENTS(gradCoefficients)

FUNCTION(identity)
FUNCTION_COEFFICIENTS(identityCoefficients)

FUNCTION(laplacian)
FUNCTION_COEFFICIENTS(laplacianCoefficients)

FUNCTION(vorticity)

// ******* derivatives in non-standard  form  ***********

//compute (u.grad)w (convective derivative of passive variable(s))
realMappedGridFunction GenericMappedGridOperators::
convectiveDerivative (
		      const realMappedGridFunction &u, 
		      const realMappedGridFunction &w,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3
		      )
{
  throwErrorMessage("convectiveDerivative"); 
  return Overture::nullRealMappedGridFunction();     
}
realMappedGridFunction GenericMappedGridOperators::
convectiveDerivative (
		      const realMappedGridFunction &u, 
		      const GridFunctionParameters & gfType,
		      const realMappedGridFunction &w,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3
		      )
{
  throwErrorMessage("convectiveDerivative"); 
  return Overture::nullRealMappedGridFunction();     
}


// Macro to define a typical function 
#define SCALAR_FUNCTION(type) \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(const realMappedGridFunction & u,  \
	                    const realMappedGridFunction & s, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}                                                              \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(const realMappedGridFunction & u,  \
                            const GridFunctionParameters & gfType,   \
	                    const realMappedGridFunction & s, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                 \
} 

#define SCALAR_FUNCTION_COEFFICIENTS(type) \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(                                   \
	                    const realMappedGridFunction & s, \
			    const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}   \
realMappedGridFunction GenericMappedGridOperators::            \
                       type(                                   \
			    const GridFunctionParameters & gfType,   \
	                    const realMappedGridFunction & s, \
                            const Index & I1,      \
			    const Index & I2,      \
			    const Index & I3,      \
			    const Index & I4,      \
			    const Index & I5,      \
			    const Index & I6,      \
			    const Index & I7,      \
			    const Index & I8 )    \
{                                                              \
  throwErrorMessage(#type);                                   \
  return Overture::nullRealMappedGridFunction();                                        \
}

// div(s grad(u)), s=scalar field
SCALAR_FUNCTION(divScalarGrad)
SCALAR_FUNCTION_COEFFICIENTS(divScalarGradCoefficients)
SCALAR_FUNCTION(divInverseScalarGrad)
SCALAR_FUNCTION_COEFFICIENTS(divInverseScalarGradCoefficients)
SCALAR_FUNCTION(scalarGrad)
SCALAR_FUNCTION_COEFFICIENTS(scalarGradCoefficients)
SCALAR_FUNCTION(divVectorScalar)
SCALAR_FUNCTION_COEFFICIENTS(divVectorScalarCoefficients)

#undef SCALAR_FUNCTION
#undef SCALAR_FUNCTION_COEFFICIENTS

//returns face-centered gradients
realMappedGridFunction GenericMappedGridOperators::
FCgrad (const realMappedGridFunction & phi,		
	const int c0,
	const int c1,
	const int c2,
	const int c3,
	const int c4,
	const Index & I1,				
	const Index & I2,
	const Index & I3,
	const Index & I4,
	const Index & I5,
	const Index & I6,
	const Index & I7,
	const Index & I8 
	)
{
  throwErrorMessage("FCgrad"); 
  return Overture::nullRealMappedGridFunction();      
}
realMappedGridFunction GenericMappedGridOperators::
FCgrad (const realMappedGridFunction & phi,		
	const GridFunctionParameters & gfType,
	const int c0,
	const int c1,
	const int c2,
	const int c3,
	const int c4,
	const Index & I1,				
	const Index & I2,
	const Index & I3,
	const Index & I4,
	const Index & I5,
	const Index & I6,
	const Index & I7,
	const Index & I8 
	)
{
  throwErrorMessage("FCgrad"); 
  return Overture::nullRealMappedGridFunction();      
}


// scalar times identityCoefficients
realMappedGridFunction GenericMappedGridOperators::
scalarCoefficients(				
		   const realMappedGridFunction & s,
		   const Index & I1,
		   const Index & I2,
		   const Index & I3,
		   const Index & I4,
		   const Index & I5,
		   const Index & I6,
		   const Index & I7,
		   const Index & I8 )
{
  throwErrorMessage("scalarCoefficients"); 
  return Overture::nullRealMappedGridFunction();                                  
}


realMappedGridFunction GenericMappedGridOperators::
derivativeScalarDerivative(const realMappedGridFunction & u,  
			   const realMappedGridFunction & s,  
			   const int & direction1,
			   const int & direction2,
			   const Index & I1,      
			   const Index & I2,      
			   const Index & I3,      
			   const Index & I4,      
			   const Index & I5,      
			   const Index & I6,      
			   const Index & I7,      
			   const Index & I8 )
{
  throwErrorMessage("derivativeScalarDerivative"); 
  return Overture::nullRealMappedGridFunction();                                  
}
                                                                         
realMappedGridFunction GenericMappedGridOperators::
derivativeScalarDerivative(const realMappedGridFunction & u,  
			   const GridFunctionParameters & gfType,   
			   const realMappedGridFunction & s, 
			   const int & direction1,
			   const int & direction2,
			   const Index & I1,      
			   const Index & I2,      
			   const Index & I3,      
			   const Index & I4,      
			   const Index & I5,      
			   const Index & I6,      
			   const Index & I7,      
			   const Index & I8 )
{
  throwErrorMessage("derivativeScalarDerivative"); 
  return Overture::nullRealMappedGridFunction();                                  
}

realMappedGridFunction GenericMappedGridOperators::
derivativeScalarDerivativeCoefficients(const realMappedGridFunction & s,  
				       const int & direction1,
				       const int & direction2,
				       const Index & I1,      
				       const Index & I2,      
				       const Index & I3,      
				       const Index & I4,      
				       const Index & I5,      
				       const Index & I6,      
				       const Index & I7,      
				       const Index & I8 )
{
  throwErrorMessage("derivativeScalarDerivativeCoefficients"); 
  return Overture::nullRealMappedGridFunction();                                  
}

                                                                         
realMappedGridFunction GenericMappedGridOperators::
derivativeScalarDerivativeCoefficients(const GridFunctionParameters & gfType,   
				       const realMappedGridFunction & s, 
				       const int & direction1,
				       const int & direction2,
				       const Index & I1,      
				       const Index & I2,      
				       const Index & I3,      
				       const Index & I4,      
				       const Index & I5,      
				       const Index & I6,      
				       const Index & I7,      
				       const Index & I8 )
{
  throwErrorMessage("derivativeScalarDerivativeCoefficients"); 
  return Overture::nullRealMappedGridFunction();                                  
}



// ********************************************************************
// ------------- Here we define the Boundary Conditions ---------------
// ********************************************************************

void GenericMappedGridOperators::
applyBoundaryConditions(realMappedGridFunction & u, const real & time)
{
  throwErrorMessage("applyBoundaryConditions"); 
}

// fill in coefficients for the boundary conditions
void GenericMappedGridOperators::
assignBoundaryConditionCoefficients(realMappedGridFunction & coeff, const real & time)
{
  throwErrorMessage("assignBoundaryConditionCoefficients"); 
}



void GenericMappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & boundaryConditionType,
		       const int & bc,
		       const real & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid  )
{
  throwErrorMessage("applyBoundaryCondition"); 
}

void GenericMappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const RealArray & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid  )
{
  throwErrorMessage("applyBoundaryCondition"); 
}

void GenericMappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const RealArray & forcing,
		       RealArray *forcinga[2][3],
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid  )
{
  throwErrorMessage("applyBoundaryCondition"); 
}

#ifdef USE_PPP
//   // this version takes a distributed array "forcing"
// void GenericMappedGridOperators::
// applyBoundaryCondition(realMappedGridFunction & u, 
// 		       const Index & Components,
// 		       const BCTypes::BCNames & boundaryConditionType,
// 		       const int & boundaryCondition,
// 		       const RealDistributedArray & forcing,
// 		       const real & time,
// 		       const BoundaryConditionParameters & bcParameters, 
// 		       const int & grid )
// {
//   throwErrorMessage("applyBoundaryCondition"); 
// }
#endif


void GenericMappedGridOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const realMappedGridFunction & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid  )
{
  throwErrorMessage("applyBoundaryCondition"); 
}


void GenericMappedGridOperators::
applyBoundaryConditionCoefficients(realMappedGridFunction & coeff, 
			           const Index & Equations,
				   const Index & Components,
				   const BCTypes::BCNames & bcType,
				   const int & bc,
				   const BoundaryConditionParameters & bcParameters,
				   const int & grid  )
{
  throwErrorMessage("applyBoundaryConditionCoefficients"); 
}

#undef FUNCTION  
#undef FUNCTION_COEFFICIENTS



//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{finishBoundaryConditions}}  
void GenericMappedGridOperators::
finishBoundaryConditionsOld(realMappedGridFunction & u,
                         const BoundaryConditionParameters & bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
			 const Range & C0 /* =nullRange */ )
//=======================================================================================
// /Description: Call this routine when all boundary conditions have been applied.
//  This function will update periodic edges and fix up the solution values in the ghost points
//  outside corners which are not assigned by {\tt applyBoundaryCondition} (i.e. the ghost
//  points that lie outside the corners in 2D or the ghost points that lie outside the edges and the vertices
//  in 3D).  This routine wil also fill in extrapolation equations at ghost points that correspond to
//  interpolation points on physical boundaries.
// 
// More precisely,
//  \begin{enumerate}
//    \item First call {\tt u.periodicUpdate()} to assign values to {\tt side=1} boundary lines
//          \[ 
//             {\tt i_{\tt axis}={\tt mg.gridIndexRange()(1,axis)}}~~{\tt axis}=0,1,..,{\tt mg.numberOfDimensions}
//          \]
//           ({\tt mg} is the {\tt MappedGrid} associated with the grid function {\tt u})
//          as well as all ghost lines on all sides that have periodic boundary conditions.
//    \item Extrapolate corner ghost points which are not assigned by step 1 
//           using extrapolation to order bcParameters.orderOfExtrapolation (orderOfAccuray+1)
//        \begin{itemize} 
//            \item  In 2D extrapolate the corner ghost points along the diagonal.
//               For example, if
//               \[
//                    {\tt bcParameters.orderOfExtrapolation=3 ~~(default for 2nd order accuracy) }
//               \]
//               then the value at the lower left
//               corner ghost point 
//               \[
//                    (i_1,i_2)=({\tt mg.indexRange()(Start,axis1)}-1,{\tt mg.indexRange()(Start,axis2)}-1)
//               \]
//               will be given by 
//            \[
//               u(i_1,i_2)=3 u(i_1+1,i_2+1) - 3 u(i_1+2,i_2+2) + u(i_1+3,i_2+3)
//            \]
//            If there are two ghost lines then also assign points $(i_1-1,i_2)$,$(i_1,i_2-1)$,$(i_1-1,i_2-1)$.
//            And so on, if there are more than 2 ghost lines.
//            \item  In 3D extrapolate the ghost points next to edges and the ghost points next to vertices.
//                   Obtain values by extrapolating into the interior as much as possible.
//        \end{itemize}
//     \item extrapolate ghost points that lies outside of interpolation points on the physical boundary,
//         mg.boundaryCondition(side,aixs)>0.
//  \end{enumerate}
//   For even more details you can look at the code in {\tt Overture/GridFunction/GenericMappedGridOperators.C}
//
//   {\bf Note:} When applied to a coefficient matrix the above operations will generate new equations in the
//     coefficient matrix rather than be applied directly to the grid function.
//   
// /u (input/output): Grid function to which boundary conditions were applied.
// /bcParameters (input): Supply parameters such as bcParameters.orderOfExtrapolation which indicates
//   the order of extrapolation to use.
// /C0 (input) : apply to these components 
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
// ****  if( u.getIsACoefficientMatrix() )

  if( u.sparse!=NULL )
  {
    // fix up the classify array for the mask array and periodicity 
    u.sparse->fixUpClassify(u);

    // extrapolate corners as needed
    
    IntegerDistributedArray & classify = u.sparse->classify;     

    MappedGrid & mg = *u.getMappedGrid();

    //     ---when two (or more) adjacent faces have boundary conditions
    //        we set the values on the fictitous line (or vertex)
    //        that is outside both faces ( points marked + below)
    //
    //                  +                +
    //                    --------------
    //                    |            |
    //                    |            |
    //
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    int side1,side2,side3,i1,i2,i3;
    Index I1,I2,I3;
    getIndex(mg.indexRange(),I1,I2,I3);
    // Index I1=Range(mg.indexRange()(Start,axis1),mg.indexRange()(End,axis1));
    // Index I2=Range(mg.indexRange()(Start,axis2),mg.indexRange()(End,axis2));
    // Index I3=Range(mg.indexRange()(Start,axis3),mg.indexRange()(End,axis3));

    //         ---extrapolate corners ---
    if( !mg.isPeriodic(axis1) && !mg.isPeriodic(axis2) )
    {
      //       ...Do the four edges parallel to i3
      side3=-1;
      for( side1=Start; side1<=End; side1++ )
      {
	is1=1-2*side1;
        // loop over all ghost points along i1:
        for( i1=mg.indexRange()(side1,axis1); i1!=mg.dimension()(side1,axis1); i1-=is1 )
	for( side2=Start; side2<=End; side2++ )
	{
	  is2=1-2*side2;
          // loop over all ghost points along i2:
          for( i2=mg.indexRange()(side2,axis2); i2!=mg.dimension()(side2,axis2); i2-=is2 )
          for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
          for( int e=0; e<numberOfComponentsForCoefficients; e++ )
            if( classify(i1-is1,i2-is2,i3,e)==SparseRepForMGF::extrapolation )
              setCornerCoefficients(u,e,i1-is1,i2-is2,i3,side1,side2,side3,bcParameters);
	  //  setExtrapolationCoefficients(u,e,i1-is1,i2-is2,i3,orderOfExtrapolation);
	}
      }
    }
    if( mg.numberOfDimensions()==3 )
    {
      if( !mg.isPeriodic(axis1) && !mg.isPeriodic(axis3) )
      {
	//       ...Do the four edges parallel to i2
        side2=-1;
	for( side1=Start; side1<=End; side1++ )
	{
	  is1=1-2*side1;
	  for( i1=mg.indexRange()(side1,axis1); i1!=mg.dimension()(side1,axis1); i1-=is1 )
	    for( side3=Start; side3<=End; side3++ )
	    {
	      is3=1-2*side3;
	      for( i3=mg.indexRange()(side3,axis3); i3!=mg.dimension()(side3,axis3); i3-=is3 )
              for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
              for( int e=0; e<numberOfComponentsForCoefficients; e++ )
                if( classify(i1-is1,i2,i3-is3,e)==SparseRepForMGF::extrapolation )
                  setCornerCoefficients(u,e,i1-is1,i2,i3-is3,side1,side2,side3,bcParameters);
	      //     setExtrapolationCoefficients(u,e,i1-is1,i2,i3-is3,orderOfExtrapolation);
	    }
	}
      }
      if( !mg.isPeriodic(axis2) && !mg.isPeriodic(axis3) )
      {
	//       ...Do the four edges parallel to i1
        side1=-1;
	for( side2=Start; side2<=End; side2++ )
	{
	  is2=1-2*side2;
	  for( i2=mg.indexRange()(side2,axis2); i2!=mg.dimension()(side2,axis2); i2-=is2 )
	    for( side3=Start; side3<=End; side3++ )
	    {
	      is3=1-2*side3;
	      for( i3=mg.indexRange()(side3,axis3); i3!=mg.dimension()(side3,axis3); i3-=is3 )
              for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
              for( int e=0; e<numberOfComponentsForCoefficients; e++ )
                if( classify(i1,i2-is2,i3-is3,e)==SparseRepForMGF::extrapolation )
                  setCornerCoefficients(u,e,i1,i2-is2,i3-is3,side1,side2,side3,bcParameters);
	      //   setExtrapolationCoefficients(u,e,i1,i2-is2,i3-is3,orderOfExtrapolation);
	    }
	}
      }

      if( !mg.isPeriodic()(axis1) && !mg.isPeriodic()(axis2) )
      {
	//       ...Do the 8 corners
	for( side1=Start; side1<=End; side1++ )
	{
	  is1=1-2*side1;
	  for( i1=mg.indexRange()(side1,axis1); i1!=mg.dimension()(side1,axis1); i1-=is1 )
	    for( side2=Start; side2<=End; side2++ )
	    {
	      is2=1-2*side2;
	      for( i2=mg.indexRange()(side2,axis2); i2!=mg.dimension()(side2,axis2); i2-=is2 )
		for( side3=Start; side3<=End; side3++ )
		{
		  is3=1-2*side3;
		  for( i3=mg.indexRange()(side3,axis3); i3!=mg.dimension()(side3,axis3); i3-=is3 )
                  for( int e=0; e<numberOfComponentsForCoefficients; e++ )
                    if( classify(i1-is1,i2-is2,i3-is3,e)==SparseRepForMGF::extrapolation )
                      setCornerCoefficients(u,e,i1-is1,i2-is2,i3-is3,side1,side2,side3,bcParameters);
		  //  setExtrapolationCoefficients(u,e,i1-is1,i2-is2,i3-is3,orderOfExtrapolation);
		}
	    }
	}
      }

    }

    //
    // now fill in extrapolation points that lie outside interpolation points on the boundary
    //         
    const int orderOfExtrapolation = bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1
                                                                         : bcParameters.orderOfExtrapolation;
    const int numberOfGhostLines = u.sparse->numberOfGhostLines;
    const intArray & mask = mg.mask();
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
      is1=is2=is3=0;
      for( int side=Start; side<=End; side++ )
      {
        isv[axis]=1-2*side;
        if( mg.boundaryCondition(side,axis)>0 )
	{
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	  {
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
                if( mask(i1,i2,i3) & CompositeGrid::ISinterpolationPoint )
		{
                  // printf("*** finishBC: fill in extrap point that is outside a boundary-interp point\n");
		  
                  // *wdh* 000925 for( int g=0; g<=numberOfGhostLines; g++ )
                  for( int g=1; g<=numberOfGhostLines; g++ )
		    for( int e=0; e<numberOfComponentsForCoefficients; e++ )
		      setExtrapolationCoefficients(u,e,i1-is1*g,i2-is2*g,i3-is3*g,orderOfExtrapolation);
		}
	      }
	    }
	  }
	}
      }
    }
    




    // fill in equations for the periodic boundary conditions
    setPeriodicCoefficients( u );

  }
  else
  {
    fixBoundaryCorners( u,bcParameters,C0 ); 
  }
}

void GenericMappedGridOperators::
setCornerCoefficients(realMappedGridFunction & coeff,
		      const int n, 
		      const Index & I1, 
		      const Index & I2, 
		      const Index & I3,
		      int side1,
		      int side2,
		      int side3,
		      const BoundaryConditionParameters & bcParameters )
{
  if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::extrapolateCorner )
  {
    // extrapolate corners as needed
    const int orderOfExtrapolation = bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1 :
      bcParameters.orderOfExtrapolation ;
    setExtrapolationCoefficients(coeff,n,I1,I2,I3,orderOfExtrapolation);

  }
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::symmetryCorner  ) 
  {
    setSymmetryCoefficients(coeff,n,I1,I2,I3);
  }
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::taylor2ndOrder  ) 
  {
    printf("****WARNING***set corner BC's in coefficient matrix: corner BC taylor2ndOrder not implemented yet, "
           "***** using symmetry BC for now \n");
    setSymmetryCoefficients(coeff,n,I1,I2,I3);
  }
  else
  {
    printf("setCornerCoefficients:ERROR:Unknown value for bcParameters.getCornerBoundaryCondition\n");
    Overture::abort("error");
  }
}






static real extrapCoeff[10][10] = 
                   {
                       {1.,-1.,0.,0.,0.,0.,0.,0.,0.,0.},     // order 1
                       {1.,-2.,1.,0.,0.,0.,0.,0.,0.,0.},     // order 2		       
                       {1.,-3.,3.,-1.,0.,0.,0.,0.,0.,0.},    // order 3		       
                       {1.,-4.,6.,-4.,1.,0.,0.,0.,0.,0.},		       
                       {1.,-5.,10.,-10.,5.,-1.,0.,0.,0.,0.},		       
                       {1.,-6.,15.,-20.,15.,-6.,1.,0.,0.,0.},		       
                       {1.,-7.,21.,-35.,35.,-21.,7.,-1.,0.,0.},		       
                       {1.,-8.,28.,-56.,70.,-56.,28.,-8.,1.,0.},		       
		       {1.,-9.,36.,-84.,126.,-126.,84.,-36.,9.,-1.}
		     };

// Use this for indexing into coefficient matrices representing systems of equations
#undef CE
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))

void GenericMappedGridOperators::
setExtrapolationCoefficients(realMappedGridFunction & coeff,
			     const int n, 
			     const Index & I1_, 
			     const Index & I2_, 
			     const Index & I3_,
			     const int order)
//=============================================================================
//  Set coefficients for extrapolation
//
//  Input 
//   iv(0:3) - i1,i2,i3
//   grid
//   order   - order of extrapolation, 1,2,3,...,9
//
//============================================================================
{
  

  if( order<1 || order>10 )
  {
    cout << "setExtrapolationCoefficients:: - invalid value for order = " << order << endl;
    Overture::abort("setExtrapolationCoefficients:: - invalid value for order = ");
  }
  
  const MappedGrid & mg = *coeff.mappedGrid;
  
  // set coefficients
  if( order >= stencilSize*numberOfComponentsForCoefficients )
  {
    cout << "setExtrapolationCoefficients:: - order >= stencilSize*numberOfComponentsForCoefficients \n";
    Overture::abort("error");
  }
    
  #ifdef USE_PPP
    const realSerialArray & coeffLocal =coeff.getLocalArray();
    const intSerialArray & mask = mg.mask().getLocalArray();
  #else
    const realSerialArray & coeffLocal = coeff;
    const intSerialArray & mask = mg.mask();
  #endif

  #ifdef USE_PPP
    const int includeGhost=1;
    Index I1=I1_, I2=I2_, I3=I3_;
    bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
    if( !ok ) return;
  #else
    const Index &I1=I1_, &I2=I2_, &I3=I3_;
  #endif

  // First zero out all coefficients for this equation
  Range Z(CE(0,n),CE(0,n+1)-1);
  coeffLocal(Z,I1,I2,I3)=0.;                                  // **** could do better ***

  // NOTE: store at the start of the equation (for Oges) --- see below as well
  for( int i=0; i<=order; i++ )
    coeffLocal(i+CE(0,n),I1,I2,I3)=extrapCoeff[order-1][i];    


  // now set equation numbers
  int dir[3], iv[3];
  int & i1=iv[axis1];
  int & i2=iv[axis2];
  int & i3=iv[axis3];

  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
  {
    
    for( int axis=0; axis<=axis3; axis++ )
    {
      // extrapolate in direction dir[axis]
      dir[axis]= iv[axis] < mg.indexRange(Start,axis) ?  1 : ( iv[axis] > mg.indexRange(End  ,axis) ? -1 : 0 );
    }
    
    for( int i=0; i<=order; i++ )
      coeff.sparse->setCoefficientIndex(i+CE(0,n), n,i1,i2,i3, 
					n,iv[axis1]+dir[axis1]*i,iv[axis2]+dir[axis2]*i,iv[axis3]+dir[axis3]*i );  
  }
  coeff.sparse->setClassify(SparseRepForMGF::extrapolation,I1,I2,I3,n);

}

void GenericMappedGridOperators::
setSymmetryCoefficients(realMappedGridFunction & coeff,
			     const int n, 
			     const Index & I1_, 
			     const Index & I2_, 
			     const Index & I3_,
			     const int option /* = 0 */)
//=============================================================================
//
//============================================================================
{
  
  const MappedGrid & mg = *coeff.mappedGrid;
  
  #ifdef USE_PPP
    const realSerialArray & coeffLocal =coeff.getLocalArray();
    const intSerialArray & mask = mg.mask().getLocalArray();
  #else
    realSerialArray & coeffLocal = coeff;
    const intSerialArray & mask = mg.mask();
  #endif
  #ifdef USE_PPP
    const int includeGhost=1;
    Index I1=I1_, I2=I2_, I3=I3_;
    bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
    if( !ok ) return;
  #else
    const Index &I1=I1_, &I2=I2_, &I3=I3_;
  #endif

  // First zero out all coefficients for this equation
  Range Z(CE(0,n),CE(0,n+1)-1);
  coeffLocal(Z,I1,I2,I3)=0.;                                  // **** could do better ***

  // NOTE: store at the start of the equation (for Oges) --- see below as well
  const int stencilLength=3;
  real symmetryCoeff[stencilLength]= {1.,0,-1.}; //  symmetry condition is u(-1)=u(+1)
    
  for( int i=0; i<stencilLength; i++ )
    coeffLocal(i+CE(0,n),I1,I2,I3)=symmetryCoeff[i];    


  // now set equation numbers
  int dir[3], iv[3];
  int & i1=iv[axis1];
  int & i2=iv[axis2];
  int & i3=iv[axis3];

  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
  {
    
    for( int axis=0; axis<=axis3; axis++ )
    {
      // go in direction dir(axis)
      dir[axis]= iv[axis] < mg.indexRange(Start,axis) ?  1 : ( iv[axis] > mg.indexRange(End,axis) ? -1 : 0 );
    }
    
    for( int i=0; i<stencilLength; i++ )
      coeff.sparse->setCoefficientIndex(i+CE(0,n), n,i1,i2,i3, 
					n,iv[axis1]+dir[axis1]*i,iv[axis2]+dir[axis2]*i,iv[axis3]+dir[axis3]*i );  
  }
  coeff.sparse->setClassify(SparseRepForMGF::extrapolation,I1,I2,I3,n);

}

void GenericMappedGridOperators::
setPeriodicCoefficients(realMappedGridFunction & coeff )
//=============================================================================
//  Fill in the coefficients for periodicity
//
//
//============================================================================
{
  assert( coeff.sparse!=NULL );
  const MappedGrid & mg = *coeff.mappedGrid;
  
  if( stencilSize != coeff.sparse->stencilSize )
  {
    printf("GenericMappedGridOperators::setPeriodicCoefficients:ERROR: stencilSize in operators (%i) \n"
           " is not equal to the stencil size in the coefficient array (%i)! \n",
	   stencilSize,coeff.sparse->stencilSize);
    Overture::abort("error");
  }
  if( numberOfComponentsForCoefficients != coeff.sparse->numberOfComponents )
  {
    cout << "GenericMappedGridOperators::setPeriodicCoefficients:ERROR: numberOfComponentsForCoefficients \n"
      << " in operators is not equal to the numberOfComponents in the coefficient array! \n";
    Overture::abort("error");
  }
    
  #ifdef USE_PPP
    const realSerialArray & coeffLocal =coeff.getLocalArray();
    const intSerialArray & mask = mg.mask().getLocalArray();
    const intSerialArray & classify = coeff.sparse->classify.getLocalArray();
  #else
    realSerialArray & coeffLocal = coeff;
    const intSerialArray & mask = mg.mask();
    const intSerialArray & classify = coeff.sparse->classify;
  #endif

  Index I1,I2,I3;
  int ivP[3], iv[3];
  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  
  getIndex(mg.gridIndexRange(),I1,I2,I3,coeff.sparse->numberOfGhostLines);
  #ifdef USE_PPP
    const int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),mask,I1,I2,I3,includeGhost);
    if( !ok ) return;
  #else
  #endif


  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
  for( int n=0; n<numberOfComponentsForCoefficients; n++ )
  {
    if( classify(i1,i2,i3,n)==SparseRepForMGF::periodic )
    {
     //    periodic point
      // First zero out all coefficients for this equation
      Range Z(CE(0,n),CE(0,n+1)-1);
      coeffLocal(Z,i1,i2,i3)=0.;                                // **** could do better ***
      ivP[0]=iv[0];  ivP[1]=iv[1]; ivP[2]=iv[2];                // ivP is the periodic image inside the domain
      for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
      {
        if( mg.isPeriodic(axis) )
        {
  	  int shift = mg.gridIndexRange(End,axis)-mg.gridIndexRange(Start,axis);
	  ivP[axis]= ( ( iv[axis]+shift-mg.gridIndexRange(Start,axis) ) % shift  )
	               + mg.gridIndexRange(Start,axis);
        }
      }
      // sanity check:
      if( iv[0]==ivP[0] && iv[1]==ivP[1] && iv[2]==ivP[2] )
      {
	cout << "GenericMappedGridOperators::setPeriodicCoefficients:ERROR: periodic position = position!\n";
        printf("Point iv=(%i,%i,%i), periodic image is the same!!\n",iv[0],iv[1],iv[2]);
	mg.isPeriodic().display("Here is mg.isPeriodic()");
	mg.gridIndexRange().display("Here is mg.gridIndexRange()");
	mg.boundaryCondition().display("Here is boundaryCondition");
        Overture::abort("error");
      }

      // fill in coefficients
      coeffLocal(0+CE(0,n),i1,i2,i3)=+1.;
      coeffLocal(1+CE(0,n),i1,i2,i3)=-1.;
      // assign equation numbers
      coeff.sparse->setCoefficientIndex(0+CE(0,n), n,i1,i2,i3, n,i1,i2,i3 );
      coeff.sparse->setCoefficientIndex(1+CE(0,n), n,i1,i2,i3, n,ivP[0],ivP[1],ivP[2] );  
    }
  }
}

// *wdh* 091123 -- old way:  -----------------------------------------------------------

#define findInterNeighboursOptInit EXTERN_C_NAME(findinterneighboursoptinit)
#define findInterNeighboursOpt EXTERN_C_NAME(findinterneighboursopt)
extern "C"
{
  void findInterNeighboursOptInit( const int&nd, 
     const int&ndm1a,const int&ndm1b,const int&ndm2a,const int&ndm2b,const int&ndm3a,const int&ndm3b,
				   const int &ndip,const int &indexRange, 
                                   int &ni, int &ip, const int &mask );

   void findInterNeighboursOpt( const int &nd, 
      const int &ndm1a,const int &ndm1b,const int &ndm2a,const int &ndm2b,const int &ndm3a,const int &ndm3b,
				const int &ndi,const int &ndin, const int&indexRange, const int &dimension,
              const int &ni, int &nin, 
              const int &mask, int &m, const int &ip,int &id, int &ia, 
              int &vew, int &ipar, int & ierr );
}


void GenericMappedGridOperators:: 
findInterpolationNeighbours()
// ===============================================================================================
// Description:
//   Initialization routine: extrapolate the unused points that lie next to interpolation points
//  This is a new routine -- we now extrapolate corners too (for AMR interpolation)
// ================================================================================================
{
  printF("\n *************GenericMappedGridOperators:: WARNING OLD findInterpolationNeighbours called ************\n");

  // this routine should only be called if there are interpolation points, but this may not be true
  numberOfInterpolationNeighboursNew=0;
  extrapolateInterpolationNeighboursIsInitialized=TRUE;

  MappedGrid & mg = mappedGrid; 
  const int numberOfDimensions = mg.numberOfDimensions();
  const int myid=max(0,Communication_Manager::My_Process_Number);

  bool useOpt=true;
  if( useOpt && interpolationPoint!=NULL )
  {
    // printf("**** findInterpolationNeighbours: use opt version -- interpolationPoint found! ****\n");

     #ifdef USE_PPP

       // In the parallel case we just find the interpolation points on this processor
       //  No communication is required.

       const intArray & maskd = mg.mask();
       // const IntegerArray & mask = maskd.getLocalArrayWithGhostBoundaries();
       intSerialArray mask; getLocalArrayWithGhostBoundaries(maskd,mask); 
       // const IntegerArray & maskLocal = maskd.getLocalArray();
       IntegerArray ip;
       Index I1,I2,I3;
       getIndex(mg.extendedIndexRange(),I1,I2,I3);

       // The local mask array includes ghost boundaries on ALL sides
       int n1a=max(I1.getBase() ,mask.getBase(0) +maskd.getGhostBoundaryWidth(0));
       int n1b=min(I1.getBound(),mask.getBound(0)-maskd.getGhostBoundaryWidth(0));

       int n2a=max(I2.getBase() ,mask.getBase(1) +maskd.getGhostBoundaryWidth(1));
       int n2b=min(I2.getBound(),mask.getBound(1)-maskd.getGhostBoundaryWidth(1));

       int n3a=max(I3.getBase() ,mask.getBase(2) +maskd.getGhostBoundaryWidth(2));
       int n3b=min(I3.getBound(),mask.getBound(2)-maskd.getGhostBoundaryWidth(2));
       
       IntegerArray extendedIndexRange(2,3);  // local version for this processor
       IntegerArray dimension(2,3);  // local version for this processor
       // IntegerArray dimension(2,3);           // local version for this processor -- do NOT include parallel ghost
       if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
       {
//          for( int dir=0; dir<3; dir++ )
// 	 {
// 	   dimension(0,dir)=max(mg.dimension(0,dir),mask.getBase(dir) +maskd.getGhostBoundaryWidth(dir));
// 	   dimension(1,dir)=min(mg.dimension(1,dir),mask.getBound(dir)-maskd.getGhostBoundaryWidth(dir));
// 	 }
	 

         extendedIndexRange(0,0)=n1a; extendedIndexRange(1,0)=n1b;
         extendedIndexRange(0,1)=n2a; extendedIndexRange(1,1)=n2b;
         extendedIndexRange(0,2)=n3a; extendedIndexRange(1,2)=n3b;
	 
         // determine interpolation points on this processor -- 
         // *wdh* 060523: include interp. pts on the first parallel ghost line if 
         //               there are more than 1 parallel ghost line.
         if( maskd.getGhostBoundaryWidth(0)>1 )
  	   I1 = Range(n1a-1,n1b+1);  
         else
  	   I1 = Range(n1a,n1b);
	 if( maskd.getGhostBoundaryWidth(1)>1 )
           I2 = Range(n2a-1,n2b+1);
         else
           I2 = Range(n2a,n2b);
         if( maskd.getGhostBoundaryWidth(2)>1 )
  	   I3 = Range(n3a-1,n3b+1);
         else
	   I3 = Range(n3a,n3b);

	 ip = (mask(I1,I2,I3)<0).indexMap(); // interpolation points on this processor;
       }
  
       const int ni=ip.getLength(0);

       for( int axis=0; axis<3; axis++ )
       {
	 dimension(0,axis)=mask.getBase(axis) +maskd.getGhostBoundaryWidth(axis);
	 dimension(1,axis)=mask.getBound(axis)-maskd.getGhostBoundaryWidth(axis);
       }
       
  
     #else

       const IntegerArray & extendedIndexRange = mg.extendedIndexRange();
       const IntegerArray & dimension = mg.dimension();

       const intArray & mask = mg.mask();
       intArray & ip = *interpolationPoint;
       const int ni=ip.getLength(0);
     #endif

     if( ni==0 ) return;

     // we could use this to compute ip if interpolationPoint==NULL
//       findInterNeighboursOptInit( numberOfDimensions, 
//  				 mask.getBase(1),mask.getBound(1),
//  				 mask.getBase(2),mask.getBound(2),
//  				 ndip,mg.indexRange(0,0), ni,*getDataPointer(ip), *getDataPointer(mask) );


     // estimate the max number of interpolation point neighbours
     int ndin = ni*numberOfDimensions*numberOfDimensions+100;
     if( numberOfDimensions==1 )
       ndin=ni*2+100;
     else if( numberOfDimensions==2 )
       ndin=ni*2+100;
     else
       ndin=ni*3+1000;
     
     if( extrapolateInterpolationNeighbourPoints==NULL )
     {
       extrapolateInterpolationNeighbourPoints=new IntegerArray;
       extrapolateInterpolationNeighboursDirection=new IntegerArray;
     }
  
     IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;
     ia.redim(ndin,numberOfDimensions);
     IntegerArray & id = *extrapolateInterpolationNeighboursDirection;
     id.redim(ndin,numberOfDimensions);

     IntegerArray m(mask.dimension(0),mask.dimension(1),mask.dimension(2));

     int *pia = getDataPointer(ia);
     
     int ipar[5];
     ipar[0]=maximumWidthToExtrapolationInterpolationNeighbours;
     bool useVariableExtrapolation=false;
     ipar[1]=(int)useVariableExtrapolation;
     int numberOfGhostPointsAvailable=min(mg.numberOfGhostPoints()(Range(0,1),Range(0,numberOfDimensions-1)));
     ipar[2]=numberOfGhostPointsAvailable; 
     #ifdef USE_PPP
       ipar[2]=min(ipar[2],maskd.getGhostBoundaryWidth(0)); // assume all ghost boundary widths are the same
     #endif
     int *pvew = extrapolateInterpolationNeighboursVariableWidth!=NULL ? 
       getDataPointer(*extrapolateInterpolationNeighboursVariableWidth) : pia;
     
     ipar[3]=myid;
     int ierr=0;
     findInterNeighboursOpt( numberOfDimensions,
			     mask.getBase(0),mask.getBound(0),
			     mask.getBase(1),mask.getBound(1),
			     mask.getBase(2),mask.getBound(2),
			     ni,ndin,extendedIndexRange(0,0),dimension(0,0),
                             ni,numberOfInterpolationNeighboursNew,
                             *getDataPointer(mask),
			     *getDataPointer(m),*getDataPointer(ip), *getDataPointer(id), 
			     *pia, *pvew, ipar[0], ierr );

     if( ierr!=0 )
     {
       if( maximumWidthToExtrapolationInterpolationNeighbours==4 )
       {
	 maximumWidthToExtrapolationInterpolationNeighbours--; 

	 printf("GMGOP::failure in findInterpolationNeighbours, myid=%i, name=%s, mg.gid=[%i,%i][%i,%i][%i,%i]\n",
                myid,(const char*)mg.getName(),
                mg.gridIndexRange(0,0),mg.gridIndexRange(1,0),
                mg.gridIndexRange(0,1),mg.gridIndexRange(1,1),
                mg.gridIndexRange(0,2),mg.gridIndexRange(1,2));

	 if( false )
  	   displayMask(mask,"Here is the local mask");

	 printf("GMGOP::failure in findInterpolationNeighbours, now try maxExtrapWidth=%i...\n",
                maximumWidthToExtrapolationInterpolationNeighbours);
	 
	 
	 ipar[0]=maximumWidthToExtrapolationInterpolationNeighbours;
	 ierr=0;
	 findInterNeighboursOpt( numberOfDimensions,
				 mask.getBase(0),mask.getBound(0),
				 mask.getBase(1),mask.getBound(1),
				 mask.getBase(2),mask.getBound(2),
				 ni,ndin,extendedIndexRange(0,0),dimension(0,0),
                                 ni,numberOfInterpolationNeighboursNew,
				 *getDataPointer(mask),
				 *getDataPointer(m),*getDataPointer(ip), *getDataPointer(id), 
				 *pia, *pvew, ipar[0], ierr );
	 
       }
       if( ierr!=0 )
       {
         errorStatus=errorInFindInterpolationNeighbours;
	 printf("GenericMappedGridOperators::findInterpolationNeighbours:ERROR: error return from "
		"findInterNeighboursOpt.\n"
                " Unable to find points to extrapolation interp. neighbours, extrap width=%i\n",
                 maximumWidthToExtrapolationInterpolationNeighbours );
	 return;
       }
     }

     if( ipar[0]<maximumWidthToExtrapolationInterpolationNeighbours )
     {
       maximumWidthToExtrapolationInterpolationNeighbours=ipar[0];
       printf("GenericMappedGridOperators::findInterpolationNeighbours:max extrapolation width reduced to %i\n",
	      maximumWidthToExtrapolationInterpolationNeighbours);
     }
     

     assert( numberOfInterpolationNeighboursNew < ndin );
     
     if( numberOfInterpolationNeighboursNew>0 )
     {
       ia.resize(numberOfInterpolationNeighboursNew,numberOfDimensions);
       id.resize(numberOfInterpolationNeighboursNew,numberOfDimensions);
       // ia.display("ia *new*");
       // id.display("id *new* ");
     }
     else
     {
       ia.redim(0);
       id.redim(0);
     }
     
     if( false )
     {
       printf(" findInterpolationNeighbours:opt: ni=%i, numberOfInterpolationNeighboursNew=%i\n",ni,
	      numberOfInterpolationNeighboursNew);
       
       display(ip,"ip - interpolation points","%3i ");
       display(ia,"ia - interpolation neighb","%3i ");
       display(id,"id - interpolation direct","%3i ");
       
     }
     

     return;
  }

  if( true )
  {
    return;  // we assume that we are in the case of a single grid
  }

  printf("**** findInterpolationNeighbours: use old version -- interpolationPoint NOT found! ****\n");

#ifndef USE_PPP

  Index I1,I2,I3;
  const IntegerArray & indexRange = mg.extendedIndexRange();

  getIndex(indexRange,I1,I2,I3);
  const intArray & mask = mg.mask();
  
  intArray ip;
  ip = (mask(I1,I2,I3)<0).indexMap(); // interpolation points, could pass in
  // ip = (mask<0).indexMap(); // interpolation points, could pass in

  // displayMask(mask,"mask");
  // display(ip,"ip");
  
  if( ip.getLength(0)==0 )  
    return;  // there are no interpolation points. ----------------------------------

  Range R=ip.dimension(0);

  int extra=1;
  getIndex(indexRange,I1,I2,I3,extra);

  intArray m(I1,I2,I3);  // m>=0 will indicate a point that needs to be extrapolated.
  m=-1;
  
  const int rBase=R.getBase();
  const int rBound=R.getBound();
  const bool useApp=false;
  
  int i2=indexRange(Start,axis2), i3=indexRange(Start,axis3);
  int n0,n1,n2;
  if( numberOfDimensions==1 )
  {
    for( n0=-1; n0<=1; n0+=2 )
    {
      where( mask(ip(R,0)+n0,i2,i3)==0 )
      {
	m(ip(R,0)+n0,i2,i3)=-n0;
      }
    }
  }
  else if( numberOfDimensions==2 )
  {
    // mark corners first, they may be over-written below
    for( n1=-1; n1<=1; n1+=2 )
    {
      for( n0=-1; n0<=1; n0+=2 )
      {
        if( useApp )
	{  // UMR's caused here
	  where( mask(ip(R,0)+n0,ip(R,1)+n1,i3)==0 )
	  {
	    m(ip(R,0)+n0,ip(R,1)+n1,i3)=-n0+1+10*(-n1+1); // encode the extrap direction
	  }
	}
	else
	{
          for( int i=rBase; i<=rBound; i++ )
	  {
	    if( mask(ip(i,0)+n0,ip(i,1)+n1,i3)==0 )
	    {
	      m(ip(i,0)+n0,ip(i,1)+n1,i3)=-n0+1+10*(-n1+1); // encode the extrap direction
	    }
	  }
	  
	}
	
      }
    }
    for( n1=-1; n1<=1; n1+=2 )
    {
      if( useApp )
      {  // UMR's caused here
	where( mask(ip(R,0),ip(R,1)+n1,i3)==0 )
	{
	  m(ip(R,0),ip(R,1)+n1,i3)=1+10*(-n1+1);
	}
      }
      else
      {
	for( int i=rBase; i<=rBound; i++ )
	{
	  if( mask(ip(i,0),ip(i,1)+n1,i3)==0 )
	  {
	    m(ip(i,0),ip(i,1)+n1,i3)=1+10*(-n1+1);
	  }
	}
      }
    }
    for( n0=-1; n0<=1; n0+=2 )
    {
      if( useApp )
      {  // UMR's caused here
	where( mask(ip(R,0)+n0,ip(R,1),i3)==0 )
	{
	  m(ip(R,0)+n0,ip(R,1),i3)=-n0+1+10;
	}
      }
      else
      {
	for( int i=rBase; i<=rBound; i++ )
	{
	  if( mask(ip(i,0)+n0,ip(i,1),i3)==0 )
	  {
	    m(ip(i,0)+n0,ip(i,1),i3)=-n0+1+10;
	  }
	}
      }
    }
  }
  else  // 3D
  {
    // first the 8 vertices
    for( n2=-1; n2<=1; n2+=2 )
    {
      for( n1=-1; n1<=1; n1+=2 )
      {
	for( n0=-1; n0<=1; n0+=2 )
	{
	  where( mask(ip(R,0)+n0,ip(R,1)+n1,ip(R,2)+n2)==0 )
	  {
	    m(ip(R,0)+n0,ip(R,1)+n1,ip(R,2)+n2)=-n0+1+10*(-n1+1)+100*(-n2+1);
	  }
	}
      }
    }

    // now the points on the mid-points of the edges
    for( n2=-1; n2<=1; n2+=2 )
    {
      for( n1=-1; n1<=1; n1+=2 )
      {
	where( mask(ip(R,0),ip(R,1)+n1,ip(R,2)+n2)==0 )
	{
	  m(ip(R,0),ip(R,1)+n1,ip(R,2)+n2)=1+10*(-n1+1)+100*(-n2+1);
	}
      }
    }
    for( n2=-1; n2<=1; n2+=2 )
    {
      for( n0=-1; n0<=1; n0+=2 )
      {
	where( mask(ip(R,0)+n0,ip(R,1),ip(R,2)+n2)==0 )
	{
	  m(ip(R,0)+n0,ip(R,1),ip(R,2)+n2)=-n0+1+10+100*(-n2+1);
	}
      }
    }
    for( n1=-1; n1<=1; n1+=2 )
    {
      for( n0=-1; n0<=1; n0+=2 )
      {
	where( mask(ip(R,0)+n0,ip(R,1)+n1,ip(R,2))==0 )
	{
	  m(ip(R,0)+n0,ip(R,1)+n1,ip(R,2))=-n0+1+10*(-n1+1)+100;
	}
      }
    }
    
    // finally the face centres
    for( n2=-1; n2<=1; n2+=2 )
    {
      where( mask(ip(R,0),ip(R,1),ip(R,2)+n2)==0 )
      {
	m(ip(R,0),ip(R,1),ip(R,2)+n2)=1+10+100*(-n2+1);
      }
    }
    for( n1=-1; n1<=1; n1+=2 )
    {
      where( mask(ip(R,0),ip(R,1)+n1,ip(R,2))==0 )
      {
	m(ip(R,0),ip(R,1)+n1,ip(R,2))=1+10*(-n1+1)+100;
      }
    }
    for( n0=-1; n0<=1; n0+=2 )
    {
      where( mask(ip(R,0)+n0,ip(R,1),ip(R,2))==0 )
      {
	m(ip(R,0)+n0,ip(R,1),ip(R,2))=-n0+1+10+100;
      }
      
    }
  }
  
  if( extrapolateInterpolationNeighbourPoints==NULL )
  {
    extrapolateInterpolationNeighbourPoints=new intArray;
    extrapolateInterpolationNeighboursDirection=new intArray;
  }
  
  IntegerArray & ia = *extrapolateInterpolationNeighbourPoints;

  intArray im;
  ia.redim(0);
  ia= (m>=0).indexMap();
  numberOfInterpolationNeighboursNew=ia.getLength(0);
  
  R=numberOfInterpolationNeighboursNew;
  IntegerArray & id = *extrapolateInterpolationNeighboursDirection;
  id.redim(R,numberOfDimensions);

  if( numberOfDimensions==1 )
  {
    id=m(ia(R,0),i2,i3);
  }
  else if( numberOfDimensions==2 )   
  {
    im=m(ia(R,0),ia(R,1),i3);
    id(R,1)=im/10;
    id(R,0)=im-id(R,1)*10;
    id-=1;
  }
  else
  {
    im=m(ia(R,0),ia(R,1),ia(R,2));
    id(R,2)=im/100;
    im-=id(R,2)*100;
    id(R,1)=im/10;
    id(R,0)=im-id(R,1)*10;
    id-=1;
  }
  
//    ia.display("ia");
//    id.display("id");
  
#endif  /* end ifndef USE_PPP */

}   



#undef CE

realArray GenericMappedGridOperators::
harmonic(const realArray & a, const realArray & b )
// =================================================================
//   Return one half the harmonic average of a and b
//  The true harmonic average is 2.*harmonic(a,b)
// ================================================================
{
  realArray result;
  result=a+b;
  where( result!=0. )
  {
    result=(a*b)/result;
  }
  return result;
}


void GenericMappedGridOperators::
setInterpolationPoint( intArray & interpolationPoint_ )
// ============================================================
// Used by GenericCompositeGridOperators. set a pointer to the
// interpolation point array.
// ============================================================
{
  interpolationPoint=&interpolationPoint_;
}
