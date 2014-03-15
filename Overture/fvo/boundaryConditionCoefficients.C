#include "MappedGridFiniteVolumeOperators.h"
#include "billsMergeMacro.h"
#include <xDC.h>
#include <SparseRep.h>
#define inside(axis,side,i) inc(axis,i)*(1-2*side)
// ... macro to loop over all components and equations
#define ForAllComponentsAndEquations(c,e) \
  for(c=C.getBase(); c<=C.getBound(); c++) \
    for (e=E.getBase(); e<=E.getBound(); e++)
#define OPX(m1,m2,m3,n,I1,I2,I3) MERGE0(opX,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPY(m1,m2,m3,n,I1,I2,I3) MERGE0(opY,M123N(m1,m2,m3,n),I1,I2,I3)
#define OPZ(m1,m2,m3,n,I1,I2,I3) MERGE0(opZ,M123N(m1,m2,m3,n),I1,I2,I3)


//\begin{>>MappedGridOperatorsInclude.tex}{\subsubsection{ApplyBoundaryConditions}}  
void MappedGridFiniteVolumeOperators::
applyBoundaryConditionCoefficients(realMappedGridFunction & coeff, 
				   const Index & E,
				   const Index & C,
				   const BCTypes::BCNames & bcType,  /* = BCTypes::dirichlet */
				   const int & bc,                   /* = allBoundaries */
				   const BoundaryConditionParameters & 
				   bcParams /* = Overture::defaultBoundaryConditionParameters() */,
				   const int & grid)
//=======================================================================================
// /Description:
//  Fill in the coefficients of the boundary conditions.
//
// /coeff (input/output): grid function to hold the coefficients of the BC.
// /t (input): apply boundary conditions at this time.
// /Limitations:
//  too many to write down.
//\end{MappedGridOperatorsInclude.tex}
//=======================================================================================
{
//  real time=getCPU();  // keep track of the cpu time spent in this routine

  if( numberOfDimensions==0 )
  {
    cout << "MappedGridOperators::ERROR: you must assign a MappedGrid before "
            " applyBoundaryConditionCoefficients! \n";
    return;
  }

/*
  if (C.getBound() != C.getBase())
  {
    cout << "applyBoundaryConditionCoefficients: WARNING: vector case not yet implemented" << endl;
  }

  if (E.getBound() != E.getBase())
  {
    cout << "applyBoundaryConditionCoefficients: WARNING: multiple equation case not yet implemented" << endl;
  }
*/
	      
  int side,axis;
  int n; // i,n,n1,n2,n3;		
  int arg1,arg2,coefficient;
  Index I1,  I2,  I3;  // ... boundary Index
  Index I1b, I2b, I3b; // ... boundary edge Index
  Index I1m, I2m, I3m; // ... ghost Cell Index
  // REAL HALF = 0.5, TWO = 2.0, ZERO = 0.0, ONE = 1.0;
  REAL HALF = 0.5, ZERO = 0.0;
//  int ghostCoefficientLocation;
//  int boundaryCoefficientLocation;
  int m1,m2,m3;
//  int sideSign;


//... take care of default values for some parameters
//  const int & ghostLineToAssign = bcParams.ghostLineToAssign;
  const int orderOfExtrapolation=bcParams.orderOfExtrapolation<0 ? orderOfAccuracy+1 : bcParams.orderOfExtrapolation;


//... these must be defined for various macros
  int dum;
  Range aR0, aR1, aR2, aR3;
//  int stencilLength0 = stencilSize;
  
  MappedGrid &mg = mappedGrid;

  GridFunctionParameters gfParams;

//
// e0: base of E
// c0: base of C
// Index M: one block of  stencil; zero offset
// Index M0: one block of stencil; offset to e0,c0
// Index ME: entire stencil
// CE(e0,c0): macro to compute offset to local stencil
//
  int c,e;
  int e0 = E.getBase();
  int c0 = C.getBase();

// ... define some Index'es to use with coefficient arrays

  Index M(0,stencilSize);  //Index defining the range of a single component, starting at 0
  Index M0(CE(c0,e0),stencilSize);  //single component stencil starting at beginning of E,C

  int stencilDim = stencilSize*numberOfComponentsForCoefficients;

  Index ME(E.getBase()*stencilDim, E.length()*stencilDim); //entire stencil for all equations and components

// *wdh realArray opX, opY, opZ;   // these will be used to compose complex BCs like neumann and mixed
  RealDistributedArray opX, opY, opZ;   // these will be used to compose complex BCs like neumann and mixed

  ForBoundary(side,axis)
  {
    // sideSign = side==0 ? -1 : 1;
    
    if (mg.boundaryCondition()(side,axis) == bc 
	|| ( bc==allBoundaries && mg.boundaryCondition()(side,axis) > 0) )
    {
      

      getBoundaryIndex (mg.indexRange(), side, axis, I1, I2, I3);
      I1b = I1 + inc(axis,0)*side;
      I2b = I2 + inc(axis,1)*side;
      I3b = I3 + inc(axis,2)*side;

      getGhostIndex    (mg.indexRange(), side, axis, I1m, I2m, I3m, +1);  //first ghost line

      switch (bcType)
      {

      //====================
      case dirichlet:  // ... average boundary and ghost cell
      //====================

	//... zero out all the BC coeffs
	coeff(ME,I1m,I2m,I3m) = ZERO; 
      
	//... fill in first component and equation
	arg1 = 1+inside(axis,side,rAxis);
	arg2 = 1+inside(axis,side,sAxis);
	coefficient    = CE(c0,e0)+n3n3(arg1,arg2);

	coeff(CE(c0,e0)+n3n3(1,1),I1m,I2m,I3m) = HALF;
	coeff(coefficient,I1m,I2m,I3m) = HALF;

	//... now copy to the remaining components and equations

	ForAllComponentsAndEquations (c,e)
	  if ( c!=c0 || e!=e0 )
	    coeff(M+CE(c,e),I1,I2,I3) = coeff(M0,I1,I2,I3);
      
	break;

	// ====================
      case neumann:		
      case mixed:
	// ====================

	if (TRUE)  // **** COMMENT THIS OUT FOR KAREN 960828
	{
	  
	// ... only implementing BCs for cellCentered implicit problems
	gfParams.inputType  = GridFunctionParameters::cellCentered;

	// ... boundary conditions are centered on the face appropriate for this boundary axis
	switch (axis)
	{
	case axis1:
	  gfParams.outputType = GridFunctionParameters::faceCenteredAxis1; 
	  break;
	case axis2:
	  gfParams.outputType = GridFunctionParameters::faceCenteredAxis2;
	  break;
	case axis3:
	  gfParams.outputType = GridFunctionParameters::faceCenteredAxis3;
	  break;
	}
	 
	// ... dimension temp storage for x,y,z differential op coeffs, then set to the basic x,y,z operators
	opX.redim(Range(M.getBase() , M.getBound()),
		  Range(I1.getBase(), I1.getBound()),
		  Range(I2.getBase(), I2.getBound()),
		  Range(I3.getBase(), I3.getBound()));

	opX = xCoefficients (gfParams,I1,I2,I3,0,0)(M,I1,I2,I3);  //only compute one comp and eq of xCoeff; then take view to get right size

	if (numberOfDimensions>1)
	{
	  opY.redim(opX);
	  opY = yCoefficients (gfParams,I1,I2,I3,0,0)(M,I1,I2,I3);
	}

	if (numberOfDimensions>2)
	{
	  opZ.redim(opX);
	  opZ = zCoefficients (gfParams,I1,I2,I3,0,0)(M,I1,I2,I3);
	}

	// ... now multiply derivative coefficients by normal vector; sideSign accounts for the fact that
	// ... faceNormal's always point towards positive values of the coordinate, but we need an actual bdy normal

	n = 0; //...only do this for first eqn and comp
	//... 960911: changed I1 to I1b, etc. for faceNormal's and faceArea's
/*
	ForStencil(m1,m2,m3)
	{
	  if (TRUE)
	    OPX(m1,m2,m3,n,I1,I2,I3) *= sideSign*faceNormal(I1b,I2b,I3b,xAxis,axis)/faceArea(I1b,I2b,I3b,axis);
	  if (numberOfDimensions > 1)
	    OPY(m1,m2,m3,n,I1,I2,I3) *= sideSign*faceNormal(I1b,I2b,I3b,yAxis,axis)/faceArea(I1b,I2b,I3b,axis);
	  if (numberOfDimensions > 2)
	    OPZ(m1,m2,m3,n,I1,I2,I3) *= sideSign*faceNormal(I1b,I2b,I3b,zAxis,axis)/faceArea(I1b,I2b,I3b,axis);
	}
	*/
	//...990107: use centerNormal instead since its available in O.v15
	realArray & normal = mg.centerBoundaryNormal(side,axis);
	
	ForStencil(m1,m2,m3)
	{
	  if (TRUE)
	    OPX(m1,m2,m3,n,I1,I2,I3) *= normal(I1b,I2b,I3b,xAxis);
	  if (numberOfDimensions > 1)
	    OPY(m1,m2,m3,n,I1,I2,I3) *= normal(I1b,I2b,I3b,yAxis);
	  if (numberOfDimensions > 2)
	    OPZ(m1,m2,m3,n,I1,I2,I3) *= normal(I1b,I2b,I3b,zAxis);
	}

	// ... zero out the boundary equations

	coeff(ME,I1m,I2m,I3m) = 0.;

	// ... now construct the actual BC coefficients for (c,e) = (0,0) (neumann)

	if ( (int)bcType == (int)neumann)
	{
	  if (numberOfDimensions == 1)

	    coeff(M0,I1m,I2m,I3m) = opX(M,I1,I2,I3);
	  else if (numberOfDimensions == 2) 

	    coeff(M0,I1m,I2m,I3m) = opX(M,I1,I2,I3) + opY(M,I1,I2,I3);
	  else 
	    {
	      coeff(M0,I1m,I2m,I3m) = opX(M,I1,I2,I3) + opY(M,I1,I2,I3) + opZ(M,I1,I2,I3);
	    }
	  
	    
	}
	else // if (bcType == mixed): alpha*u + beta*u.n
	{
		
          real alpha, beta;
          if( bcParams.a.getLength(0)>=2 )
	  {
	    alpha = bcParams.a(0);
	    beta  = bcParams.a(1);
	  }
	  else
	  {
	    printf("MappedGridOperators::applyBoundaryConditionCoefficients ERROR applying mixed BC\n");
	    printf(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
	    exit(1);
	  }  
	  
	  if (numberOfDimensions == 1) 

	    coeff(M0,I1m,I2m,I3m) = beta*opX(M,I1,I2,I3) + alpha*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
	  else if (numberOfDimensions == 2)

      	    coeff(M0,I1m,I2m,I3m) = beta*(opX(M,I1,I2,I3)+opY(M,I1,I2,I3)) + alpha*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
	  else

	    coeff(M0,I1m,I2m,I3m) = beta*(opX(M,I1,I2,I3)+opY(M,I1,I2,I3)+opZ(M,I1,I2,I3)) 
	      + alpha*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
	}

        // fix up equation numbers -- stencil is centered around the boundary cells for side 0, but should be centered around the ghost cells
        assert( coeff.sparse!=0 );

	for( e=E.getBase(); e<=E.getBound(); e++ )                        
	{
	  for( c=C.getBase(); c<=C.getBound(); c++ )                        
	  {
	    if (side == 0)
	    {
	      ForStencil(m1,m2,m3)  
		coeff.sparse->setCoefficientIndex(M123CE(m1,m2,m3,c,e), e,I1m,I2m,I3m, c,(I1+m1),(I2+m2),(I3+m3) );  
	    }
	    
	  }
	  coeff.sparse->setClassify(SparseRepForMGF::ghost1,I1m,I2m,I3m,e);
	}
	// coeff.sparse->classify.display("Here is classify after applyBoundaryConditionCoefficients");

	
	break;	
      }
	

/* -----
	if (FALSE) //Fake Neumann logic
	{
	  //...ghost cell always -1, boundary cell always 1: this simplifies logic
	  arg1 = 1+inside(axis,side,rAxis);
	  arg2 = 1+inside(axis,side,sAxis);
	  ghostCoefficientLocation    = n3n3(1,1);
	  boundaryCoefficientLocation = n3n3(arg1,arg2);
	  
	  coeff(M,        I1m,I2m,I3m) = ZERO;
	  coeff(ghostCoefficientLocation    ,I1m,I2m,I3m) = -ONE; // (2*side-1)
	  coeff (boundaryCoefficientLocation,I1m,I2m,I3m) =  ONE; // -(2*side-1)
	}
	break;
----- */	
	
	// ====================
      case extrapolate:		
	// ====================

      
	for (e=E.getBase(); e<=E.getBound(); e++) // ****** fix this for c and e *****
          setExtrapolationCoefficients(coeff,e,I1m,I2m,I3m,orderOfExtrapolation);

	//...old code
	if (FALSE) 
	{
	  coeff(M,        I1m,I2m,I3m) = ZERO;
	  coeff(n3n3(1,1),I1m,I2m,I3m) = (2*side-1);
	  arg1 = 1+inside(axis,side,rAxis);
	  arg2 = 1+inside(axis,side,sAxis);
	  coefficient    = n3n3(arg1,arg2);
	  //  coeff(n3n3(1+inside(axis,side,rAxis),1+inside(axis,side,sAxis)),I1m,I2m,I3m) = -(2*side-1);
	  coeff(coefficient,I1m,I2m,I3m) =  -(2*side-1);
	}
	
	break;

	// ====================
      default:
	// ====================

	cout << "applyBoundaryConditionCoefficients: boundary condition unknown or un-implemented" << endl;
	throw "MappedGridFiniteVolumeOperators::applyBoundaryConditionCoefficients: fatal error! \n";


      }
    }
  }
}


/* *** This shouldn't be here
//==============================================================================
//\begin{>boundaryConditions.tex}{\subsection{setTwilightZoneFlow}} 
void MappedGridFiniteVolumeOperators::
setTwilightZoneFlow (const bool & TwilightZoneFlow0)
//
//
// /Purpose:
//   Indicate whether or not twilightzone flow forcing should be added to the BC's
//   951107: (DLB) Twilight zone forcing is not fully implemented for this class.
//
// /TwilightZoneFlow0: = TrueOrFalse;
//\end{boundaryConditions.tex} 
//========================================
{
  twilightZoneFlow = TwilightZoneFlow0;
}

//==============================================================================
//\begin{>>boundaryConditions.tex}{\subsection{setTwilightZoneFlowFunction}} 
void MappedGridFiniteVolumeOperators::
setTwilightZoneFlowFunction (OGFunction & TwilightZoneFlowFunction0)
//
// /Purpose:
//   Indicate which twilightzone flow function should be used
//   951107 (DLB): Twilight zone forcing is not fully implemented for this class.
//
// /TwilightZoneFunction0:	the OGFunction to be used for BC
//						forcing
//
//\end{boundaryConditions.tex}  
//========================================
{
  twilightZoneFlowFunction = &TwilightZoneFlowFunction0;
}

*** */
#include "oldBoundaryConditions.C"

