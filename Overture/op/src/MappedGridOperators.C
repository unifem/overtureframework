#include "MappedGridOperators.h"
#include "defineFDerivatives.h"
#include "GridFunctionParameters.h"
#include "FourierOperators.h"
#include "conversion.h"
#include "AssignInterpNeighbours.h"

//
//          *** here are some comments ****
//
//\begin{>MappedGridOperatorsInclude.tex}{\subsubsection{Public enumerators}} 
//\no function header:
// 
// Here are the public enumerators:
//
// /derivativeTypes:
// This enumerator contains a list of all the derivatives that we know how to evaluate
// {\footnotesize
// \begin{verbatim}     
//   enum derivativeTypes
//   {
//     xDerivative,
//     yDerivative,
//     zDerivative,
//     xxDerivative,
//     xyDerivative,
//     xzDerivative,
//     yxDerivative,
//     yyDerivative,
//     yzDerivative,
//     zxDerivative,
//     zyDerivative,
//     zzDerivative,
//     laplacianOperator,
//     r1Derivative,
//     r2Derivative,
//     r3Derivative,
//     r1r1Derivative,
//     r1r2Derivative,
//     r1r3Derivative,
//     r2r2Derivative,
//     r2r3Derivative,
//     r3r3Derivative,
//     gradient,
//     divergence,
//     divergenceScalarGradient,
//     scalarGradient,
//     identityOperator,
//     vorticityOperator,
//     xDerivativeScalarXDerivative,
//     xDerivativeScalarYDerivative,
//     yDerivativeScalarYDerivative,
//     yDerivativeScalarZDerivative,
//     zDerivativeScalarZDerivative,
//     divVectorScalarDerivative,
//     numberOfDifferentDerivatives   // counts number of entries in this list
//   };
// \end{verbatim}     
// }
//  /BCNames: This enum (which for technical reasons is in the BCTypes Class, 
//    NOT the MappedGridOperators) defines the different types of elementary boundary
//   conditions that have been implemented:
// {\footnotesize
// \begin{verbatim}     
//      enum BCNames
//      {
//        dirichlet,
//        neumann,
//        extrapolate,
//        normalComponent,
//        mixed,
//        generalMixedDerivative,
//        normalDerivativeOfNormalComponent,
//        normalDerivativeOfADotU,
//        aDotU,
//        aDotGradU,
//        evenSymmetry,
//        vectorSymmetry,
//        TangentialComponent0,
//        TangentialComponent1,
//        normalDerivativeOfTangentialComponent0,
//        normalDerivativeOfTangentialComponent1,
//        numberOfDifferentBoundaryConditionTypes   // counts number of entries in this list
//      };
// \end{verbatim}     
// }
//
//\end{MappedGridOperatorsInclude.tex}



//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{Constructors}}  
MappedGridOperators::
MappedGridOperators()
//=======================================================================================
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  setup();
}

//\begin{>>MappedGridOperatorsInclude.tex}{}
MappedGridOperators::
MappedGridOperators( MappedGrid & mg )
//=======================================================================================
// /Description:
//   Construct a MappedGridOperators
// /mg (input): Associate this grid with the operators.
// /Author: WDH
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  setup();
  updateToMatchGrid( mg );
}

//=======================================================================================
//   Copy constructor
//=======================================================================================
MappedGridOperators::
MappedGridOperators( const MappedGridOperators & mgfd )
{
  // deep copy
  setup(); // is this needed? ***
  orderOfAccuracy=mgfd.orderOfAccuracy;   // **** finish this ***
}

//=======================================================================================
//   virtualConstructor
//
//  This routine should create a new object of this class and return as a pointer
//  to the base class MappedGridOperators.
//
//  Notes:
//   o This routine is needed if this class has been derived from the base class MappedGridOperators
//   o This routine is used by the classes GridCollectionOperators and CompositeGridOperators
//     in order to construct lists of this class. These classes only know about the base class
//     and so they are unable to create a "new" version of this class
//=======================================================================================
GenericMappedGridOperators* MappedGridOperators::
virtualConstructor() const
{
  return new MappedGridOperators();
}

//=======================================================================================
//  desctructor
//=======================================================================================
MappedGridOperators::
~MappedGridOperators()
{
  for( int axis=0; axis<3; axis++ )
  {
    delete [] neumannCoeff[axis]; 
    delete [] mixedDerivativeCoeff[axis];
    delete [] aDotGradUCoeff[axis];
    delete [] generalMixedDerivativeCoeff[axis];
    delete [] generalizedDivergenceCoeff[axis];
    delete [] normalDotScalarGradCoeff[axis];
  }
  delete fourierOperators;
}


/* -----  Here are some comments -----
//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{Derivatives x,y,z,xx,xy,xz,yy,yz,zz,laplacian,grad,div}}
MappedGridFunction 
"derivative"(const realMappedGridFunction & u,
           const Index & I0,  // =nullIndex 
	   const Index & I1,  // =nullIndex 
	   const Index & I2,  // =nullIndex 
	   const Index & I3,  // =nullIndex 
	   const Index & I4,  // =nullIndex 
	   const Index & I5,  // =nullIndex 
	   const Index & I6,  // =nullIndex 
	   const Index & I7   // =nullIndex 
         )
//==================================================================================
// /Description:
//   "derivative" equals one of x, y, z, xx, xy, xz, yy, yz, zz, laplacian, grad, div.
// /u (input): Take the derivative of this grid function.
// /I0,I1,I3 (input): evaluate the derivatives at these points.
// /I4 (input) : evaluate the derivative for these components, by default all components. 
// /Return value:
//   The derivative is returned as a new grid function. For all derivatives but {\tt grad} and {\tt div}
// the number of components in the result is equal to the number of components specified by I4 (if I4
// not specified then the result will have the same number of components as {\tt u}). The {\tt grad} operator
// will have number of components equal to the number of space dimensions while the {\tt div}
// operator will have only one component.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{Derivative Coefficients}} 
MappedGridFunction 
"derivativeCoefficients"(const Index & I0,  // =nullIndex 
	   const Index & I1,  // =nullIndex 
	   const Index & I2,  // =nullIndex 
	   const Index & I3,  // =nullIndex 
	   const Index & I4,  // =nullIndex 
	   const Index & I5,  // =nullIndex 
	   const Index & I6,  // =nullIndex 
	   const Index & I7   // =nullIndex 
         )
//==================================================================================
// /Description:
//   "derivativeCoefficients" equals one of xCoefficients, yCoefficients, zCoefficients, 
//   xxCoefficients, xyCoefficients, xzCoefficients, yyCoefficients, yzCoefficients, zzCoefficients,
//    laplacianCoefficients, gradCoefficients, divCoefficients, identityCoefficients.
//   Compute the coefficients of the specified derivative.
// /I0,I1,... (input): determine the coefficients at these points.
// /return Value:
//   The derivative coefficients.
//\end{MappedGridFunctionInclude.tex} 
//==================================================================================
 ----------------- */


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{get}}
int MappedGridOperators::
get( const GenericDataBase & dir, const aString & name)
//-------------------------------------------------------------------
// /Description:
//   Get from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{MappedGridOperatorsInclude.tex}{}
//-------------------------------------------------------------------
{
  cout << " MappedGridOperators::get - not implemented yet!\n"; 
  return 1;
}


//=======================================================================================
// operator = is a deep copy
//=======================================================================================
MappedGridOperators & MappedGridOperators::
operator= ( const MappedGridOperators & dmgf )
{
  if( &dmgf )
  {
    cout << "MappedGridOperators::operator= :ERROR: not implemented yet. You can maybe use the "
            "updateToMatchGrid function.\n";
    Overture::abort("error");
  }
  orderOfAccuracy=dmgf.orderOfAccuracy;    
  GenericMappedGridOperators::operator=(dmgf);

  return *this;
}

GenericMappedGridOperators & MappedGridOperators::
operator= ( const GenericMappedGridOperators & mgo )
{
  if( this )
  {
    cout << "MappedGridOperators::operator= :ERROR: operator= ( const GenericMappedGridOperators & mgo ) called\n";
    Overture::abort("error");
  }
  return *this;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{getFourierOperators}}
FourierOperators* MappedGridOperators::
getFourierOperators(const bool abortIfNull /* =true */) const 
//==================================================================================
// /Description:
//   Return a pointer to the Fourier operators used by this class to perform pseudo-spectral derivatives.
//   {\bf NOTE:} This pointer will not be assigned until the first derivative operation is applied.
//
// /abortIfNull (input) : by default this routine will abort if the pointer is null
//\end{MappedGridOperatorsInclude.tex} 
//==================================================================================
{
  if( fourierOperators==NULL )
  {
    cout << "MappedGridOperators::getFourierOperators:ERROR: the fourierOperators pointer is NULL \n";
    Overture::abort("error");
  }
  return fourierOperators;
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{put}}
int MappedGridOperators::
put( GenericDataBase & dir, const aString & name) const
//==================================================================================
// /Description:
//   output onto a database file
// /dir (input): put onto this directory of the database.
// /name (input): the name of the grid function on the database.
//\end{MappedGridOperatorsInclude.tex} 
//==================================================================================
{
  cout << " MappedGridOperators::put - not implemented yet!\n"; 
  return 0;
}



//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setOrderOfAccuracy}}
void MappedGridOperators::
setOrderOfAccuracy( const int & orderOfAccuracy0 )
//==================================================================================
// /Description:
//   set the order of accuracy
// /orderOfAccuracy0 (input): valid values are 2 or 4 or MappedGridOperators::spectral.
//  Choosing spectral means that derivatives are computed with the pseudo-spectral method.
//  This is only valid for rectangular periodic grids.
//\end{MappedGridOperatorsInclude.tex} 
//==================================================================================
{
  if( orderOfAccuracy0!=2 && 
      orderOfAccuracy0!=4 && 
      orderOfAccuracy0!=6 &&  // some operators we know to 6th and 8th order
      orderOfAccuracy0!=8 &&
      orderOfAccuracy0!=spectral )
  {
    cout << "MappedGridOperators::ERROR: setOrderOfAccuracy : invalid orderOfAccuracy ="
         << orderOfAccuracy0 << endl;
    return;
  } 
  orderOfAccuracy=orderOfAccuracy0;   // *** more things to add ***

  if( orderOfAccuracy0!=spectral )
  {
    width=orderOfAccuracy+1;
    halfWidth1=width/2;
    halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
    halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

//     delta.redim(width);      delta.setBase(-halfWidth1);
//     delta=0.;
//     delta(0)=1.;
  }
  
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setStencilSize}}
void MappedGridOperators::
setStencilSize(const int stencilSize0)
//==================================================================================
// /Description:
//   Indicate the stencil size for functions returning coefficients
//\end{MappedGridOperatorsInclude.tex} 
//==================================================================================
{
  stencilSize=stencilSize0;
}


//=======================================================================================
//  initialize variables and provide default values
//=======================================================================================
void MappedGridOperators::
setup()
{
  useNewOperators=true;

  orderOfAccuracy=2;     // default value
  numberOfDimensions=0;  // local copy; use this to see if a MappedGrid has been assigned
  stencilSize=0;        // use default stencil size for coefficients
  rectangular=false;     // is the grid rectangular
  numberOfComponentsForCoefficients=1;
  extrapolateInterpolationNeighboursIsInitialized=false;

  // The following variables are used by the boundary conditions
  twilightZoneFlow=false;
  twilightZoneFlowFunction=NULL;

//  numberOfComponents=10;                   // *************** fix this *******
//  maximumNumberOfBoundaryConditions=10;    // *************** fix this *******
//   numberOfBoundaryConditions.redim(2,3); numberOfBoundaryConditions=0;
//   boundaryCondition.redim(2,3,maximumNumberOfBoundaryConditions); boundaryCondition=-1;
//   componentForBoundaryCondition.redim(2,3,maximumNumberOfBoundaryConditions,numberOfComponents);

//   boundaryConditionValueGiven.redim(2,3,maximumNumberOfBoundaryConditions);  
//   boundaryConditionValueGiven=false;
//   boundaryConditionValue.redim(2,3,maximumNumberOfBoundaryConditions);  
//   boundaryConditionValue=0.;
//   constantCoefficient.resize(3,2,3,maximumNumberOfBoundaryConditions);
//   boundaryData=false;
//   orderOfExtrapolation.redim(2,3,maximumNumberOfBoundaryConditions);
//   orderOfExtrapolation=2;
//   ghostLineToExtrapolate.redim(2,3,maximumNumberOfBoundaryConditions);
//   ghostLineToExtrapolate=1;

  for( int i=0; i<numberOfDifferentDerivatives; i++ )
  {
    derivativeArray[i]=NULL;
    derivativeFunction[i]=NULL;
    derivCoefficientsFunction[i]=NULL;
  }

  updateDerivativeFunctions(); // assign default functions for computing derivatives

  // nCoeffIsSet(side,axis) == true if nCoeff is used on the boundary
//   nCoeffIsSet.redim(2,3); 
//   nCoeffIsSet=false;
//   mCoeffIsSet.redim(2,3); 
//   mCoeffIsSet=false;
//   aCoeffIsSet.redim(2,3); 
//   aCoeffIsSet=false;
//   gCoeffIsSet.redim(2,3); 
//   gCoeffIsSet=false;
//   gdCoeffIsSet.redim(2,3); 
//   gdCoeffIsSet=false;
//   normalDotScalarGradCoeffIsSet.redim(2,3);
//   normalDotScalarGradCoeffIsSet=false;
  
  for( int axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      nCoeffIsSet[axis][side]=false;
      mCoeffIsSet[axis][side]=false;
      aCoeffIsSet[axis][side]=false;
      gCoeffIsSet[axis][side]=false;
      gdCoeffIsSet[axis][side]=false;
      normalDotScalarGradCoeffIsSet[axis][side]=false;
    }
    neumannCoeff[axis]=NULL;
    mixedDerivativeCoeff[axis]=NULL;
    aDotGradUCoeff[axis]=NULL;
    generalMixedDerivativeCoeff[axis]=NULL;
    generalizedDivergenceCoeff[axis]=NULL;
    normalDotScalarGradCoeff[axis]=NULL;
  }

  for( int m=0; m<12; m++ )
    mCoeffValues[m]=0.;
  for( int m=0; m<18; m++ )
    aCoeffValues[m]=0.;
  for( int m=0; m<24; m++ )
    gCoeffValues[m]=0.;
  
  fourierOperators=NULL;
  boundaryNormalsUsed=false;
  boundaryTangentsUsed=false;
}


bool MappedGridOperators::
createBoundaryMatrix(const int & side, 
		     const int & axis,
		     const BCTypes::BCNames & boundaryConditionType)
// ======================================================================================
// /Description:
//    Some boundary conditions save a matrix of values on the boundary in order to
//   be more efficient. This routine will create this matrix if it has not already
//   been created.
// /Return Value: true means that a new matrix was created. false means that no new
//    matrix ws created.
{
  bool returnValue=false;
  switch (boundaryConditionType)
  {
  case normalDotScalarGrad:
    if( normalDotScalarGradCoeff[axis]==NULL )
    {
      normalDotScalarGradCoeff[axis] = new realSerialArray [2];
      returnValue=true;
    }
    break;
  case neumann:
    if( neumannCoeff[axis]==NULL )
    {
      neumannCoeff[axis] = new realSerialArray [2];
      returnValue=true;
    }
    break;
  case mixed:
    if( mixedDerivativeCoeff[axis]==NULL )
    {
      mixedDerivativeCoeff[axis] = new realSerialArray [2];
      returnValue=true;
    }
    break;
  case aDotGradU:
    if( aDotGradUCoeff[axis]==NULL )
    {
      aDotGradUCoeff[axis] = new realSerialArray [2];
      returnValue=true;
    }
    break;
  case generalMixedDerivative:
    if( generalMixedDerivativeCoeff[axis]==NULL )
    {
      generalMixedDerivativeCoeff[axis] = new realSerialArray [2];
      returnValue=true;
    }
    break;
  case generalizedDivergence:
    if( generalizedDivergenceCoeff[axis]==NULL )
    {
      generalizedDivergenceCoeff[axis] = new realSerialArray [2];
      returnValue= true;
    }
    break;
  default:
    printF("MappedGridOperators::createBoundaryMatrix:ERROR: unknown boundaryConditionType=%i\n",(int)boundaryConditionType);
    Overture::abort("error");
  }
  return returnValue;
}

//---------------------------------------------------------------------------------------
//  Assign the array of pointers to derivative functions.
//
//    In the future we will allow people to change these derivatives
//
// To call one of these functions use:
//   (this->*derivativeFunction[xDerivative])( ... );  
//---------------------------------------------------------------------------------------
void MappedGridOperators::
updateDerivativeFunctions()
{
  derivativeFunction[r1Derivative]  = rDerivative;
  derivativeFunction[r2Derivative]  = sDerivative;
  derivativeFunction[r3Derivative]  = tDerivative;
  derivativeFunction[r1r1Derivative]= rrDerivative;
  derivativeFunction[r1r2Derivative]= rsDerivative;
  derivativeFunction[r1r3Derivative]= rtDerivative;
  derivativeFunction[r2r2Derivative]= ssDerivative;
  derivativeFunction[r2r3Derivative]= stDerivative;
  derivativeFunction[r3r3Derivative]= ttDerivative;

  derivativeFunction[ xDerivative]=  xFDerivative;
  derivativeFunction[ yDerivative]=  yFDerivative;
  derivativeFunction[ zDerivative]=  zFDerivative;
  derivativeFunction[xxDerivative]= xxFDerivative;
  derivativeFunction[xyDerivative]= xyFDerivative;
  derivativeFunction[xzDerivative]= xzFDerivative;
  derivativeFunction[yyDerivative]= yyFDerivative;
  derivativeFunction[yzDerivative]= yzFDerivative;
  derivativeFunction[zzDerivative]= zzFDerivative;

  derivativeFunction[laplacianOperator]= laplaceFDerivative;
  derivativeFunction[gradient]         = gradFDerivative;
  derivativeFunction[divergence]       = divFDerivative;
  derivativeFunction[identityOperator] = identityFDerivative;
  derivativeFunction[vorticityOperator]= vorticityFDerivative;

  // These functions define the coefficients
  derivCoefficientsFunction[r1Derivative]  = rDerivCoefficients;
  derivCoefficientsFunction[r2Derivative]  = sDerivCoefficients;
  derivCoefficientsFunction[r3Derivative]  = tDerivCoefficients;
  derivCoefficientsFunction[r1r1Derivative]= rrDerivCoefficients;
  derivCoefficientsFunction[r1r2Derivative]= rsDerivCoefficients;
  derivCoefficientsFunction[r1r3Derivative]= rtDerivCoefficients;
  derivCoefficientsFunction[r2r2Derivative]= ssDerivCoefficients;
  derivCoefficientsFunction[r2r3Derivative]= stDerivCoefficients;
  derivCoefficientsFunction[r3r3Derivative]= ttDerivCoefficients;

  derivCoefficientsFunction[ xDerivative]=  xFDerivCoefficients;
  derivCoefficientsFunction[ yDerivative]=  yFDerivCoefficients;
  derivCoefficientsFunction[ zDerivative]=  zFDerivCoefficients;
  derivCoefficientsFunction[xxDerivative]= xxFDerivCoefficients;
  derivCoefficientsFunction[xyDerivative]= xyFDerivCoefficients;
  derivCoefficientsFunction[xzDerivative]= xzFDerivCoefficients;
  derivCoefficientsFunction[yyDerivative]= yyFDerivCoefficients;
  derivCoefficientsFunction[yzDerivative]= yzFDerivCoefficients;
  derivCoefficientsFunction[zzDerivative]= zzFDerivCoefficients;

  derivCoefficientsFunction[laplacianOperator]= laplaceFDerivCoefficients;
  derivCoefficientsFunction[gradient]         = gradFDerivCoefficients;
  derivCoefficientsFunction[divergence]       = divFDerivCoefficients;
  derivCoefficientsFunction[identityOperator] = identityFDerivCoefficients;

}	

void MappedGridOperators::
useConservativeApproximations(bool trueOrFalse /* = true */ )
// this overloads base class so we can update the grid
{
  if( !usingConservativeApproximations() && trueOrFalse && mappedGrid.numberOfDimensions()>0 
      &&  !mappedGrid.isRectangular()  )
    mappedGrid.update(MappedGrid::THEcenterJacobian); 
    
  GenericMappedGridOperators::useConservativeApproximations(trueOrFalse);
}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{setTwilightZoneFlow}}  
void MappedGridOperators::
setTwilightZoneFlow( const int & twilightZoneFlow0 )
//=======================================================================================
// /Description: Indicate if twilight-zone forcing should be added to boundary conditions
// /twilightZoneFlow0 (input): if true then add the twilight-zone forcing 
//   (see also setTwilightZoneFlowFunction and the section on boundary conditions)
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  twilightZoneFlow= twilightZoneFlow0;
  if( twilightZoneFlow )
    mappedGrid.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
}

real MappedGridOperators::
sizeOf(FILE *file /* = NULL */ ) const
// =======================================================================================
// /Description:
//   Return size of this object  
//\end{MappedGridOperatorsInclude.tex}  
// =======================================================================================
{
  real size=sizeof(*this);
  size+=GenericMappedGridOperators::sizeOf()-sizeof(GenericMappedGridOperators);

//  real tempArrays1=(ur.elementCount()+us.elementCount()+ut.elementCount())*sizeof(real);
//  real tempArrays2=uDotN.sizeOf();

//   real tempArrays3=(numberOfBoundaryConditions.elementCount()+
// 	 boundaryCondition.elementCount()+
// 	 componentForBoundaryCondition.elementCount()+
// 	 boundaryConditionValueGiven.elementCount()+
// 	 orderOfExtrapolation.elementCount()+
// 	 ghostLineToExtrapolate.elementCount())*sizeof(int);
  
//   size+=tempArrays1+tempArrays2+tempArrays3;

//  size+=(boundaryConditionValue.elementCount()+constantCoefficient.elementCount())*sizeof(real);

  int i;
  real boundaryCoeff=0.;
  for( i=0; i<3; i++ )
  {
    if( neumannCoeff[i]!=NULL )
      boundaryCoeff+=neumannCoeff[i]->elementCount()*sizeof(real);
    if( mixedDerivativeCoeff[i]!=NULL )
      boundaryCoeff+=mixedDerivativeCoeff[i]->elementCount()*sizeof(real);
    if( aDotGradUCoeff[i]!=NULL )
      boundaryCoeff+=aDotGradUCoeff[i]->elementCount()*sizeof(real);
    if( generalMixedDerivativeCoeff[i]!=NULL )
      boundaryCoeff+=generalMixedDerivativeCoeff[i]->elementCount()*sizeof(real);
    if( generalizedDivergenceCoeff[i]!=NULL )
      boundaryCoeff+=generalizedDivergenceCoeff[i]->elementCount()*sizeof(real);
    if( normalDotScalarGradCoeff[i]!=NULL )
      boundaryCoeff+=normalDotScalarGradCoeff[i]->elementCount()*sizeof(real);
  }
  size+=boundaryCoeff;
  
//  size+=mask.elementCount()*sizeof(int); 

  if( file!=NULL )
  {
    fPrintF(file,"MappedGridOperators memory usage for grid %s\n",
            (const char*)mappedGrid.getName());
//     fPrintF(file,"  temp arrays (1)...........%9.2f Kbytes  %5.1f %% \n",
// 	    tempArrays1/1.e3,100.*tempArrays1/size);
//     fPrintF(file,"  temp arrays (2)...........%9.2f Kbytes  %5.1f %% \n",
// 	    tempArrays2/1.e3,100.*tempArrays2/size);
//     fPrintF(file,"  temp arrays (3)...........%9.2f Kbytes  %5.1f %% \n",
// 	    tempArrays3/1.e3,100.*tempArrays3/size);
    fPrintF(file,"  boundary coeff............%9.2f Kbytes  %5.1f %% \n",
	    boundaryCoeff/1.e3,100.*boundaryCoeff/size);
//     fPrintF(file,"  coeff, opX,...............%9.2f Kbytes  %5.1f %% \n",
// 	    opCoeff/1.e3,100.*opCoeff/size);
    fPrintF(file,"  total.....................%9.2f Kbytes  %5.1f %% \n",
            size/1.e3,100.);
  }
  

  return size;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{isRectangular}}
bool MappedGridOperators::
isRectangular()
//=======================================================================================
// /Description: 
//  Return true if the grid is rectangular
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
  return rectangular;
}

//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{updateToMatchGrid}}
void MappedGridOperators::
updateToMatchGrid( MappedGrid & mg )
//========================================================================
// /Description:
//   associate a new MappedGrid with this object
// /mg (input): use this MappedGrid.
// /Notes:
//  perform computations here that only depend on the grid
//\end{MappedGridOperatorsInclude.tex}  
//========================================================================
{
  if ( mg.getGridType()==GenericGrid::unstructuredGrid )
    {
      updateToMatchUnstructuredGrid( mg );
      return;
    }

  if( min(mg.numberOfGhostPoints()(Range(0,1),Range(0,mg.numberOfDimensions()-1)))<1 )
  {
    cout << "MappedGridOperators::updateToMatchGrid:ERROR: not enough ghost points\n";
    cout << "The operators require at least one ghost point, you will have to remake the grid\n";
    mg.numberOfGhostPoints()(Range(0,1),Range(0,mg.numberOfDimensions()-1)).display("ghost point array");
    Overture::abort("error");
  }

  rectangular=mg.isRectangular();

  // Make sure some grid functions exist in the grid:
  int stuffToUpdate = MappedGrid::THEmask;
  if( twilightZoneFlow )
    stuffToUpdate |= MappedGrid::THEvertex | MappedGrid::THEcenter;
  if( boundaryNormalsUsed )
    stuffToUpdate |= MappedGrid::THEvertexBoundaryNormal;
  if( boundaryTangentsUsed )
    stuffToUpdate |= MappedGrid::THEcenterBoundaryTangent;                      
  if( !rectangular )
    stuffToUpdate |= MappedGrid::THEinverseVertexDerivative;
  if( !rectangular && usingConservativeApproximations() )
    stuffToUpdate |= MappedGrid::THEcenterJacobian; 
  
  mg.update(stuffToUpdate);

  mappedGrid.reference(mg);
  numberOfDimensions=mappedGrid.numberOfDimensions();    // local copy for convenience


//   nCoeffIsSet=false;   // recompute neumann BC's
//   mCoeffIsSet=false;   // recompute mixed BC's
//   aCoeffIsSet=false;   // recompute aDotGrad BC's
//   gCoeffIsSet=false;   // recompute general mixed BC's
//   gdCoeffIsSet=false;   // recompute generalized divergence
  extrapolateInterpolationNeighboursIsInitialized=false; // recompute interpolation neighbours
  // *wdh* 091123 new way: 
  if( assignInterpNeighbours!=NULL )
    assignInterpNeighbours->gridHasChanged();

  int axis;
  for( axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      nCoeffIsSet[axis][side]=false;
      mCoeffIsSet[axis][side]=false;
      aCoeffIsSet[axis][side]=false;
      gCoeffIsSet[axis][side]=false;
      gdCoeffIsSet[axis][side]=false;
      normalDotScalarGradCoeffIsSet[axis][side]=false;
    }
    delete [] neumannCoeff[axis];                 neumannCoeff[axis]=NULL;
    delete [] mixedDerivativeCoeff[axis];         mixedDerivativeCoeff[axis]=NULL;
    delete [] aDotGradUCoeff[axis];               aDotGradUCoeff[axis]=NULL;
    delete [] generalMixedDerivativeCoeff[axis];  generalMixedDerivativeCoeff[axis]=NULL;
    delete [] generalizedDivergenceCoeff[axis];   generalizedDivergenceCoeff[axis]=NULL;
  }


  // useWhereMask[3][2] = true if we should use a where mask on a given boundary
  //                    : true for a physical boundary that has interpolation points on it

  const IntegerDistributedArray & mgMask = mg.mask();
  Index I1,I2,I3;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      useWhereMaskOnBoundary[axis][side]=false;
      if( mg.boundaryCondition(side,axis)>0 )
      {
	// getBoundaryIndex(mg.indexRange(),side,axis,I1,I2,I3);
	getGhostIndex(mg.indexRange(),side,axis,I1,I2,I3);  // always look for mask from the first ghost line.
        for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	{
	  // check one line at a time since we are likely to find an unused point faster this way (?)
          // printf(" (side,axis)=(%i,%i) min(abs(mgMask(I1,I2,i3)))=%i \n",side,axis,min(abs(mgMask(I1,I2,i3))));
	  
	  // *wdh* 990915 replace stuff below with bit operations
	  // int numberOfInteriorBoundaryPoints=
	  //   sum(mgMask(I1,I2,i3)<0 && 
	  // mgMask(I1,I2,i3) >= (MappedGrid::ISinterpolationPoint | MappedGrid::ISinteriorBoundaryPoint) );
	  // if( numberOfInteriorBoundaryPoints>0 )
	  // {
	  //   printf("	numberOfInteriorBoundaryPoints=%i \n",numberOfInteriorBoundaryPoints);
	  //   printf(" useWhereMaskOnBoundary[axis=%i][side=%i] = true \n",axis,side);
	  //   useWhereMaskOnBoundary[axis][side]=true;
	  //   break;
	  // }

          // use bit operators:
          useWhereMaskOnBoundary[axis][side]= max(mgMask(I1,I2,i3) & MappedGrid::ISinteriorBoundaryPoint) > 0;
	  if( useWhereMaskOnBoundary[axis][side] )
	  {
  	    //  printf(" useWhereMaskOnBoundary[axis=%i][side=%i] = true \n",axis,side);
	    break;
	  }
	  

	}
      }
    }
  }

  // ****** but what if this is user set already ???? *******************
  if( stencilSize==0 && orderOfAccuracy!=spectral )
    stencilSize=(int)max(stencilSize,pow(orderOfAccuracy+1,numberOfDimensions)+1); 
  else if( orderOfAccuracy==spectral )
    stencilSize=0;

  // The arrays d12, d22, d14 and d24 are used to define the DerivCoefficientss
  //   d12 = 1/(2*dr)   d22=1/dr**2    d14=1/(12*dr)   d24=1/(12*dr**2)
//   d12=1./(2.*mappedGrid.gridSpacing());  
//   d22=1./SQR(mappedGrid.gridSpacing());

//   d14=1./(12.*mappedGrid.gridSpacing());
//   d24=1./(12.*SQR(mappedGrid.gridSpacing()));

  // The arrays h21, h22, h41 and h42 are used to define the derivatives for rectangular grids
//   RealArray maximumGridSpacing(3),minimumGridSpacing(3);
//   maximumGridSpacing=1.; 
//   minimumGridSpacing=1.;

//   rectangular=mappedGrid.isRectangular();

  if( rectangular )
  {
//    real dx[3];
    mappedGrid.getDeltaX(dx);
//     for( axis=0; axis<3; axis++ )
//     {
//       minimumGridSpacing(axis)=dx[axis];
//       maximumGridSpacing(axis)=dx[axis];
//     }
  }
  else
  {
    for( axis=0; axis<3; axis++ )
      dx[axis]=1.;
  }
  
  
  if( Mapping::debug & 2 )
  {
    if( rectangular )
      printF("MappedGridOperators: rectangular mapping found \n");
    else
      printF("MappedGridOperators: non-rectangular mapping found \n");
  }  

  for( axis=0; axis<3; axis++ )
  {
    if( dx[axis]<= 0. ) dx[axis]=1.;
    
//     if ( maximumGridSpacing(axis) <= 0. )
//       maximumGridSpacing(axis) = 1.;
//     if ( maximumGridSpacing(axis) <= 0. )
//       maximumGridSpacing(axis) = 1.;
  }
  
//   h21=1./(2.*maximumGridSpacing);  // used for 2nd-order 1-st derivatives, rectangular grid
//   h22=1./SQR(maximumGridSpacing);  // used for 2nd-order 2-nd derivatives

//   h41=1./(12.*maximumGridSpacing);
//   h42=1./(12.*SQR(maximumGridSpacing));


  // Define Difference operators for returning the coeffcients
  //  of differential operators
  //
  //  delta : is the delta function
  //  Dr, Ds, Dt  : first derivative operators (in r space), 2nd order
  //  Drr, Drs,...: second derivative operators (in r space), 2nd order

  setOrderOfAccuracy( orderOfAccuracy );
  // --- Define 2nd order discretization:---

//   Dr.redim(5);        Dr.setBase(-2);    
//   Ds.redim(5);        Ds.setBase(-2);
//   Dt.redim(5);        Dt.setBase(-2);
//   Drr.redim(5);       Drr.setBase(-2);
//   Dss.redim(5);       Dss.setBase(-2);
//   Dtt.redim(5);       Dtt.setBase(-2);
//   Drs.redim(5,5);     Drs.setBase(-2);
//   Drt.redim(5,5);     Drt.setBase(-2);
//   Dst.redim(5,5);     Dst.setBase(-2);
  
//   Dr(+1)=d12(axis1); 
//   Dr( 0)=0.;
//   Dr(-1)=-Dr(+1);

//   Ds(+1)=d12(axis2);
//   Ds( 0)=0.;
//   Ds(-1)=-Ds(+1);

//   Dt(+1)=d12(axis3);
//   Dt( 0)=0.;
//   Dt(-1)=-Dt(+1);

//   Drr(+1)=d22(axis1);
//   Drr( 0)=-2.*Drr(+1);
//   Drr(-1)=Drr(+1);

//   Dss(+1)=d22(axis2);
//   Dss( 0)=-2.*Dss(+1);
//   Dss(-1)=Dss(+1);

//   Dtt(+1)=d22(axis3);
//   Dtt( 0)=-2.*Dtt(+1);
//   Dtt(-1)=Dtt(+1);

//   Drs=0.;
//   Drs(+1,+1)=d12(axis1)*d12(axis2);
//   Drs(+1,-1)=-Drs(+1,+1);
//   Drs(-1,+1)=-Drs(+1,+1);
//   Drs(-1,-1)=+Drs(+1,+1);

//   Drt=0.;
//   Drt(+1,+1)=d12(axis1)*d12(axis3);
//   Drt(+1,-1)=-Drt(+1,+1);
//   Drt(-1,+1)=-Drt(+1,+1);
//   Drt(-1,-1)=+Drt(+1,+1);

//   Dst=0.;
//   Dst(+1,+1)=d12(axis2)*d12(axis3);
//   Dst(+1,-1)=-Dst(+1,+1);
//   Dst(-1,+1)=-Dst(+1,+1);
//   Dst(-1,-1)=+Dst(+1,+1);

//   // -----Define 4th order discretization-----

//   Dr4.redim(5);        Dr4.setBase(-2);
//   Ds4.redim(5);        Ds4.setBase(-2);
//   Dt4.redim(5);        Dt4.setBase(-2);
//   Drr4.redim(5);       Drr4.setBase(-2);
//   Dss4.redim(5);       Dss4.setBase(-2);
//   Dtt4.redim(5);       Dtt4.setBase(-2);
//   Drs4.redim(5,5); Drs4.setBase(-2);
//   Drt4.redim(5,5); Drt4.setBase(-2);
//   Dst4.redim(5,5); Dst4.setBase(-2);
  
//   Dr4(+2)=  -d14(axis1); 
//   Dr4(+1)=8.*d14(axis1); 
//   Dr4( 0)=0.;
//   Dr4(-1)=-Dr4(+1);
//   Dr4(-2)=-Dr4(+2);

//   Ds4(+2)=  -d14(axis2); 
//   Ds4(+1)=8.*d14(axis2); 
//   Ds4( 0)=0.;
//   Ds4(-1)=-Ds4(+1);
//   Ds4(-2)=-Ds4(+2);

//   Dt4(+2)=  -d14(axis3); 
//   Dt4(+1)=8.*d14(axis3); 
//   Dt4( 0)=0.;
//   Dt4(-1)=-Dt4(+1);
//   Dt4(-2)=-Dt4(+2);

//   Drr4(+2)=    -d24(axis1);
//   Drr4(+1)= 16.*d24(axis1);
//   Drr4( 0)=-30.*d24(axis1);
//   Drr4(-1)=Drr4(+1);
//   Drr4(-2)=Drr4(+2);

//   Dss4(+2)=    -d24(axis2);
//   Dss4(+1)= 16.*d24(axis2);
//   Dss4( 0)=-30.*d24(axis2);
//   Dss4(-1)=Dss4(+1);
//   Dss4(-2)=Dss4(+2);

//   Dtt4(+2)=    -d24(axis3);
//   Dtt4(+1)= 16.*d24(axis3);
//   Dtt4( 0)=-30.*d24(axis3);
//   Dtt4(-1)=Dtt4(+1);
//   Dtt4(-2)=Dtt4(+2);


//   Drs4=0.;
//   Drs4(+2,+2)=     d14(axis1)*d14(axis2);
//   Drs4(+1,+2)= -8.*d14(axis1)*d14(axis2);
//   Drs4(-1,+2)=-Drs4(+1,+2);
//   Drs4(-2,+2)=-Drs4(+2,+2);
//   Drs4(+2,+1)= Drs4(+1,+2);
//   Drs4(+1,+1)= 64.*d14(axis1)*d14(axis2);
//   Drs4(-1,+1)=-Drs4(+1,+1);
//   Drs4(-2,+1)=-Drs4(+2,+1);
//   Drs4(+2,-1)=-Drs4(+2,+1);
//   Drs4(+1,-1)=-Drs4(+1,+1);
//   Drs4(-1,-1)=+Drs4(+1,+1);
//   Drs4(-2,-1)=+Drs4(+2,+1);
//   Drs4(+2,-2)=-Drs4(+2,+2);
//   Drs4(+1,-2)=-Drs4(+1,+2);
//   Drs4(-1,-2)=+Drs4(+1,+2);
//   Drs4(-2,-2)=+Drs4(+2,+2);

//   Drt4=0.;
//   Drt4(+2,+2)=     d14(axis1)*d14(axis3);
//   Drt4(+1,+2)= -8.*d14(axis1)*d14(axis3);
//   Drt4(-1,+2)=-Drt4(+1,+2);
//   Drt4(-2,+2)=-Drt4(+2,+2);
//   Drt4(+2,+1)= Drt4(+1,+2);
//   Drt4(+1,+1)= 64.*d14(axis1)*d14(axis3);
//   Drt4(-1,+1)=-Drt4(+1,+1);
//   Drt4(-2,+1)=-Drt4(+2,+1);
//   Drt4(+2,-1)=-Drt4(+2,+1);
//   Drt4(+1,-1)=-Drt4(+1,+1);
//   Drt4(-1,-1)=+Drt4(+1,+1);
//   Drt4(-2,-1)=+Drt4(+2,+1);
//   Drt4(+2,-2)=-Drt4(+2,+2);
//   Drt4(+1,-2)=-Drt4(+1,+2);
//   Drt4(-1,-2)=+Drt4(+1,+2);
//   Drt4(-2,-2)=+Drt4(+2,+2);

//   Dst4=0.;
//   Dst4(+2,+2)=     d14(axis2)*d14(axis3);
//   Dst4(+1,+2)= -8.*d14(axis2)*d14(axis3);
//   Dst4(-1,+2)=-Dst4(+1,+2);
//   Dst4(-2,+2)=-Dst4(+2,+2);
//   Dst4(+2,+1)= Dst4(+1,+2);
//   Dst4(+1,+1)= 64.*d14(axis2)*d14(axis3);
//   Dst4(-1,+1)=-Dst4(+1,+1);
//   Dst4(-2,+1)=-Dst4(+2,+1);
//   Dst4(+2,-1)=-Dst4(+2,+1);
//   Dst4(+1,-1)=-Dst4(+1,+1);
//   Dst4(-1,-1)=+Dst4(+1,+1);
//   Dst4(-2,-1)=+Dst4(+2,+1);
//   Dst4(+2,-2)=-Dst4(+2,+2);
//   Dst4(+1,-2)=-Dst4(+1,+2);
//   Dst4(-1,-2)=+Dst4(+1,+2);
//   Dst4(-2,-2)=+Dst4(+2,+2);

}


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{updateToMatchGrid}}
void MappedGridOperators::
updateToMatchUnstructuredGrid( MappedGrid & mg )
//========================================================================
// /Description:
//   associate a new unstructured MappedGrid with this object
// /mg (input): use this MappedGrid.
// /Notes:
//  perform computations here that only depend on the grid
//\end{MappedGridOperatorsInclude.tex}  
//========================================================================
{
  // Make sure some grid functions exist in the grid:
  int stuffToUpdate = MappedGrid::THEmask;
  if( twilightZoneFlow )
    stuffToUpdate |= MappedGrid::THEvertex | MappedGrid::THEcenter;
  if( boundaryNormalsUsed )
    stuffToUpdate |= MappedGrid::THEvertexBoundaryNormal;
  if( boundaryTangentsUsed )
    stuffToUpdate |= MappedGrid::THEcenterBoundaryTangent;                      

  stuffToUpdate |= MappedGrid::THEcellVolume | MappedGrid::THEfaceNormal | MappedGrid::THEfaceArea;

  mg.update(stuffToUpdate);

  mappedGrid.reference(mg);
  numberOfDimensions=mappedGrid.numberOfDimensions();    // local copy for convenience

  int axis;
  for( axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      nCoeffIsSet[axis][side]=false;
      mCoeffIsSet[axis][side]=false;
      aCoeffIsSet[axis][side]=false;
      gCoeffIsSet[axis][side]=false;
      gdCoeffIsSet[axis][side]=false;
      normalDotScalarGradCoeffIsSet[axis][side]=false;
    }
    delete [] neumannCoeff[axis];                 neumannCoeff[axis]=NULL;
    delete [] mixedDerivativeCoeff[axis];         mixedDerivativeCoeff[axis]=NULL;
    delete [] aDotGradUCoeff[axis];               aDotGradUCoeff[axis]=NULL;
    delete [] generalMixedDerivativeCoeff[axis];  generalMixedDerivativeCoeff[axis]=NULL;
    delete [] generalizedDivergenceCoeff[axis];   generalizedDivergenceCoeff[axis]=NULL;
  }

  if( Mapping::debug & 2 )
  {
    cout << "MappedGridOperators: unstructured mapping found \n";
  }  
  
  for( axis=0; axis<3; axis++ )
    dx[axis]=1.;
  
  setOrderOfAccuracy( orderOfAccuracy );

  return;
}
