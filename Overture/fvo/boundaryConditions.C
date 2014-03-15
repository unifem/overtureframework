#include "MappedGridFiniteVolumeOperators.h"
#include <SparseRep.h>
#include "billsMergeMacro.h"

// extern realMappedGridFunction Overture::nullRealMappedGridFunction();

  // bcOption's
static const int scalarForcing=0,
                 arrayForcing=1,
                 gridFunctionForcing=2; 


//\begin{>>boundaryConditions.tex}{\subsubsection{applyBoundaryCondition}}  
void MappedGridFiniteVolumeOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,  /* = BCTypes::dirichlet */
		       const int & bc,                   /* = allBoundaries */
		       const real & forcing,             /* =0. */
		       const real & time,                /* =0. */
		       const BoundaryConditionParameters & 
                              bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		       const int & grid)
//=======================================================================================
// /Description:
//  Apply a boundary condition to a grid function.
//  This routine implements every boundary condition known to man (ha!)
//
// /u (input/output): apply boundary conditions to this grid function.
// /Components (input): apply to these components
// /bcType  (input): the name of the boundary condition to apply (dirichlet, neumann,...)
// /bc (input): apply the boundary condition on all sides of the grid where the
//     boundaryCondition array (in the MappedGrid) is equal to this value. By default
//     apply to all boundaries (with a positive value for boundaryCondition).
// /forcing (input): This value is used as a forcing for the boundary condition, if needed. 
// /time (input): apply boundary conditions at this time (used by twilightZoneFlow)
// /bcParameters (input): optional parameters are passed using this object.
//
//\end{boundaryConditions.tex}
//=======================================================================================
{
  applyBoundaryCondition(u,
			 Components,
			 bcType,
			 bc,
			 forcing,
			 Overture::nullRealDistributedArray(),
			 Overture::nullRealMappedGridFunction(),
			 time,
                         bcParameters,
			 scalarForcing);
}


void MappedGridFiniteVolumeOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType, 
		       const int & bc,                  
		       const realArray & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid)
{
  real forcing1;
  applyBoundaryCondition(u,
			 Components,
			 bcType,
			 bc,
			 forcing1,
			 forcing,
			 Overture::nullRealMappedGridFunction(),
			 time,
                         bcParameters,
			 arrayForcing);
}

void MappedGridFiniteVolumeOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & Components,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const realMappedGridFunction & forcing,
		       const real & time,
		       const BoundaryConditionParameters & bcParameters,
		       const int & grid)
{
  real forcing1;
  applyBoundaryCondition(u,
			 Components,
			 bcType,
			 bc,
			 forcing1,
			 Overture::nullRealDistributedArray(),
			 forcing,
			 time,
                         bcParameters,
			 gridFunctionForcing);
}





// Private BC routine
void MappedGridFiniteVolumeOperators::
applyBoundaryCondition(realMappedGridFunction & u, 
		       const Index & C,
		       const BCTypes::BCNames & bcType,
		       const int & bc,
		       const real & scalarData,
		       const realArray & arrayData,
		       const realMappedGridFunction & gfData,
		       const real & t,
		       const BoundaryConditionParameters & bcParameters,
		       const int bcOption)

{
  if( numberOfDimensions==0 )
  {
    cout << "MappedGridFiniteVolumeOperators::ERROR: you must assign a MappedGrid before applyBoundaryConditions! \n";
    return;
  }
  assert( numberOfDimensions==1 || numberOfDimensions==2 || numberOfDimensions==3 );
  
  int side,axis;
  int is1,is2,is3,line,m,n,n1,n2,n3;		// Bill says its a bad idea to declare stuff within a switch
  // real a1,a2,a3,aNorm;
  // real b0,b1,b2,b3;
  realArray coeff, opX, opY, opZ, norm;

//... take care of default values for some parameters
  const int & ghostLineToAssign = bcParameters.ghostLineToAssign;
  const int orderOfExtrapolation= bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1 : bcParameters.orderOfExtrapolation;

  
  Range Cgf = C-C.getBase()+gfData.getComponentBase(0); // Range for gfData

  Index I1,  I2,  I3;           // first cell inside boundary
  Index I1m, I2m, I3m;          // first cell outside boundary
  Index I1g, I2g, I3g;          // chosen line of ghost cells outside boundary
  Index I1i, I2i, I3i;          // line of ghost cells inside boundary corresponding to I*g
  Index I1b, I2b, I3b;          // index for faceCentered objects on the boundary itself
  Index I1e, I2e, I3e;          // index of line to extrapolate
    
  bool uDotNUpdated = FALSE;
  REAL HALF = 0.5, TWO = 2.0;

  MappedGrid &c = mappedGrid;

  OGFunction *e = twilightZoneFlowFunction;
  if (twilightZoneFlow) assert (twilightZoneFlowFunction != NULL);

  // 960805: uComponents/cComponents replaces "components" in Ov.v5:
  // uC and fC will hold the components that we operate on; whether or not explicitly specified by user
// *wdh  intArray uC, fC;
  IntegerArray uC, fC;
  // bool componentsSpecified=FALSE;
  if( bcParameters.uComponents.getLength(0) > 0 )
  {
    // componentsSpecified=TRUE;
    uC=bcParameters.uComponents;
    fC=bcParameters.fComponents;
    if( uC.getLength(0)!=fC.getLength(0) )
    {
      cout << "applyBoundaryConditions:ERROR: intArray's specifying components are not the same length\n"
           << " bcParameters.uComponents.getLength(0) = " <<  bcParameters.uComponents.getLength(0) << endl
	   << " bcParameters.fComponents.getLength(0) = " <<  bcParameters.fComponents.getLength(0) << endl;
    }
  }
  else
  {
    uC.redim(Range(C));
    for( n=C.getBase(); n<=C.getBound(); n+=C.getStride() )
      uC(n)=n;
    if( bcOption==gridFunctionForcing )
    {
      fC.redim(Range(Cgf));
      for( n=Cgf.getBase(); n<=Cgf.getBound(); n+=Cgf.getStride() )
        fC(n)=n;
    }
    else
    {
      fC.redim(Range(C));
      for( n=C.getBase(); n<=C.getBound(); n+=C.getStride() )
        fC(n)=n;
    }
    
  }

// *wdh  realArray & uA = u; // use this reference to simplify some statements

  RealDistributedArray & uA = u; // use this reference to simplify some statements

// ***** this should be obsolete in v5, and therefore removed
// *wdh  const intArray & components = bcParameters.components;
  const IntegerArray & components = bcParameters.components;

  // ========================================
  // extrapolateInterpolationNeigbours is a
  // special case that loops over its own 
  // list of points; this code segment is 
  // copied from MappedGridOperators::applyBoundaryCondition
  // and will probably be replaced with a call to a 
  // GenericMappedGridOperators function sometime soon
  // ========================================

  

  // ============================================================
  if( bcType==BCTypes::extrapolateInterpolationNeighbours )
  // ============================================================
  {
    Overture::abort("This no longer works");
/* --- *wdh* 020801     
    // Extrapolate the unused points that lie next to interpolation points
    // Note: the "corners" next to interpolation points are not assigned, only
    // the neighbours that lie along one of the coordinate directions. So the
    // points marked "e" below are assigned
    //               e e e
    //             e I I I       e=extrapolate
    //           e I I X X       I= interpolation pt
    //           e I X X X       X= discretaization pt
    //           e I X X X
    if( !extrapolateInterpolationNeighboursIsInitialized )
      findInterpolationNeighbours();

    for( m=0; m<numberOfInterpolationNeighbours[0]; m++ )
      uA(ipn[0](m,0),ipn[0](m,1),ipn[0](m,2),C)=
	3.*uA(ipn[0](m,0)+  ipd[0](m),ipn[0](m,1),ipn[0](m,2),C)
	  -3.*uA(ipn[0](m,0)+2*ipd[0](m),ipn[0](m,1),ipn[0](m,2),C)
	    +uA(ipn[0](m,0)+3*ipd[0](m),ipn[0](m,1),ipn[0](m,2),C);
    
    if( c.numberOfDimensions()>1 )
    {
      for( m=0; m<numberOfInterpolationNeighbours[1]; m++ )
	uA(ipn[1](m,0),ipn[1](m,1),ipn[1](m,2),C)=
	  3.*uA(ipn[1](m,0),ipn[1](m,1)+  ipd[1](m),ipn[1](m,2),C)
	    -3.*uA(ipn[1](m,0),ipn[1](m,1)+2*ipd[1](m),ipn[1](m,2),C)
	      +uA(ipn[1](m,0),ipn[1](m,1)+3*ipd[1](m),ipn[1](m,2),C);
    }
    if( c.numberOfDimensions()>2 )
    {
      for( m=0; m<numberOfInterpolationNeighbours[2]; m++ )
	uA(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2),C)=
	  3.*uA(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+  ipd[2](m),C)
	    -3.*uA(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+2*ipd[2](m),C)
	      +uA(ipn[2](m,0),ipn[2](m,1),ipn[2](m,2)+3*ipd[2](m),C);
    }
    return;   // ******
    ----- */
  }

 // ========================================
 // Loop over Boundaries; assign values
 // ========================================

 ForBoundary (side, axis)
 {
   //...if this is the indicated boundary
    if (c.boundaryCondition()(side,axis) == bc ||
	(bc==allBoundaries && c.boundaryCondition()(side,axis) > 0))
    {
//      line = bcParameters.ghostLineToExtrapolate;
      line = ghostLineToAssign;                           // *** make sure this is really correct

      getBoundaryIndex (c.indexRange(), side, axis, I1, I2, I3);
      getGhostIndex    (c.indexRange(), side, axis, I1m, I2m, I3m, +1);  //first ghost line or surface
      getGhostIndex    (c.indexRange(), side, axis, I1b, I2b, I3b, side); //index of boundary face
      getGhostIndex    (c.indexRange(), side, axis, I1g, I2g, I3g, line);
      getGhostIndex    (c.indexRange(), side, axis, I1i, I2i, I3i, -line+1);
      
      switch ( bcType ) 
      {


	// ====================
      case BCTypes::dirichlet:
	// ====================


	if (twilightZoneFlow)
	{
//	  u(I1m,I2m,I3m,C) = -u(I1,I2,I3,C) + TWO*(e->u(c,I1m,I2m,I3m,C,t) + e->u(c,I1,I2,I3,C,t));
//	  u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + TWO*(e->u(c,I1g,I2g,I3g,C,t) + e->u(c,I1,I2,I3,C,t)); // fix? are the Index's on e->u correct?
	  u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + (*e)(c,I1g,I2g,I3g,C,t) + (*e)(c,I1i,I2i,I3i,C,t); // 961016: fixed?
	  if (debug) boundaryConditionDisplay.display (u, "array after Dirichlet TWF BC applied");
	} 
	else
	{
	  switch (bcOption)
	  {
	    //====================
	  case scalarForcing:
	    //====================
	    
//	    u(I1m,I2m,I3m,C) = -u(I1,I2,I3,C) + TWO*scalarData;
	    u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + TWO*scalarData;
	    break;
	    
	    
	    // ====================
	  case arrayForcing:
	    // ====================
//	    u(I1m,I2m,I3m,C) = -u(I1,I2,I3,C) + TWO*arrayData(C);
// *wdh     u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + TWO*arrayData(C);
  	    for( n=C.getBase(); n<=C.getBound(); n++ )
	      u(I1g,I2g,I3g,n) = -u(I1i,I2i,I3i,n) + TWO*arrayData(n);
	    break;
	    

	    // ====================
	  case gridFunctionForcing:
	    // ====================
	    if (components.getLength(0)==0)
//	      u(I1m,I2m,I3m,C) = -u(I1,I2,I3,C) + TWO*gfData(I1m,I2m,I3m, C-C.getBase()+gfData.getComponentBase(0));
	      u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + TWO*gfData(I1g,I2g,I3g, C-C.getBase()+gfData.getComponentBase(0));
	    else
	    {
	      for (n=C.getBase(), m=0; n<=C.getBound(); n++,m++)
//		u(I1m,I2m,I3m,C) = -u(I1,I2,I3,C) + TWO*gfData(I1m,I2m,I3m,components(m));
		u(I1g,I2g,I3g,C) = -u(I1i,I2i,I3i,C) + TWO*gfData(I1g,I2g,I3g,components(m));
	    }
	    
	    break;
	    
	  default:
	    
	    throw "Invalid value for bcOption!";

	  }
	}
	
	
	break;

	// ====================
      case BCTypes::neumann:  //(note, still not yet implemented for nonorthogonal grids)
	// ====================

/* ***  960522: What we need to do here is roughly the same as for the implicit boundary condition.
   ***  However, unlike the vertex centered case, the neumann BC results in an implicit 
   ***  system that must be solved on the boundary if nothing special is done since its a
   ***  9-point formula. Instead, we will first extrapolate all ghost cells, and then
   ***  solve for the central ghost cell using the formula for the normal derivative.
   ***  Since A++ creates temporaries, this will work (i.e. we get Jacobi, not Gauss-Seidel)
   *** */

	// ====================
	if ( bcOption == scalarForcing )
	  // ====================
//	  u(I1m,I2m,I3m,C) = u(I1,I2,I3,C) - scalarData;  // *** fix this ***
	  u(I1g,I2g,I3g,C) = u(I1i,I2i,I3i,C) - scalarData;  // *** fix this ***
	
	// ====================
	else if ( bcOption == arrayForcing)
	  // ====================
//	  u(I1m,I2m,I3m,C) = u(I1,I2,I3,C) - arrayData(C); // *** fix this ***
// *wdh	  u(I1g,I2g,I3g,C) = u(I1i,I2i,I3i,C) - arrayData(C); // *** fix this ***
  	  for( n=C.getBase(); n<=C.getBound(); n++ )
	    u(I1g,I2g,I3g,n) = u(I1i,I2i,I3i,n) - arrayData(n); // *** fix this ***

	// ====================
	else if ( bcOption == gridFunctionForcing)
	  // ====================
	  
	  if (components.getLength(0)==0)
//	    u(I1m,I2m,I3m,C) = u(I1,I2,I3,C) - gfData(I1m,I2m,I3m, C-C.getBase()+gfData.getBase());
	    u(I1g,I2g,I3g,C) = u(I1i,I2i,I3i,C) - gfData(I1g,I2g,I3g, C-C.getBase()+gfData.getBase());
	  else
	  {
	    for (n=C.getBase(), m=0; n<=C.getBound(); n++,m++)
//	      u(I1m,I2m,I3m,C) = u(I1,I2,I3,C) - gfData(I1m,I2m,I3m,components(m));
	      u(I1g,I2g,I3g,C) = u(I1i,I2i,I3i,C) - gfData(I1i,I2i,I3i,components(m));
	  }
	else 
	  throw "invalid value for bcOption!";

	if (twilightZoneFlow)
//	  u(I1m,I2m,I3m,n) += (*e)(c,I1m,I2m,I3m,n,t) - (*e)(c,I1,I2,I3,n,t);
	  u(I1g,I2g,I3g,n) += (*e)(c,I1i,I2i,I3i,n,t) - (*e)(c,I1,I2,I3,n,t);  // *** fix? are the Index's on (*e) correct?

	break;

	// ====================
      case BCTypes::extrapolate:
	// ====================

//	line = bcParameters.ghostLineToExtrapolate;
	line = ghostLineToAssign;                                 // *** make sure this is really correct
	if( line > 2 || line < 0 )
	  cout << "applyBoundaryConditions::ERROR? extrapolating ghost line " << line << endl; 
	getGhostIndex( c.indexRange(),side,axis,I1e,I2e,I3e,line); // line to extrapolate
	is1 = (axis==axis1) ? 1-2*side : 0;   
	is2 = (axis==axis2) ? 1-2*side : 0;           
	is3 = (axis==axis3) ? 1-2*side : 0;           
	if( orderOfExtrapolation < 0 || orderOfExtrapolation>20 )
	{
	  cout << "applyBoundaryConditions::ERROR? orderOfExtrapolation = " << 
	    orderOfExtrapolation << endl;
	  exit(1);
	}
	switch( orderOfExtrapolation )
	{  // extrapolate to the given order
	case 1:
	  u(I1e,I2e,I3e,C)=u(I1e+is1,I2e+is2,I3e+is3,C);
	  if( twilightZoneFlow )
	    u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t)-(*e)(c,I1e+is1,I2e+is2,I3e+is3,C,t);
	  break;
	case 2:
	  u(I1e,I2e,I3e,C)=2.*u(I1e+  is1,I2e+  is2,I3e+  is3,C)
	    -u(I1e+2*is1,I2e+2*is2,I3e+2*is3,C);
	  if( twilightZoneFlow )
	    u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t)-2.*(*e)(c,I1e+  is1,I2e+  is2,I3e+  is3,C,t)
	      +(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,C,t);
	  break;
	case 3:
	  u(I1e,I2e,I3e,C)=3.*u(I1e+  is1,I2e+  is2,I3e+  is3,C)
	    -3.*u(I1e+2*is1,I2e+2*is2,I3e+2*is3,C)
	      +   u(I1e+3*is1,I2e+3*is2,I3e+3*is3,C);
	  
	  if( twilightZoneFlow )
	    u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t)-3.*(*e)(c,I1e+  is1,I2e+  is2,I3e+  is3,C,t)
	      +3.*(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,C,t)
		-   (*e)(c,I1e+3*is1,I2e+3*is2,I3e+3*is3,C,t);
	  break;
	case 4:
	  u(I1e,I2e,I3e,C)=4.*u(I1e+  is1,I2e+  is2,I3e+  is3,C)
	    -6.*u(I1e+2*is1,I2e+2*is2,I3e+2*is3,C)
	      +4.*u(I1e+3*is1,I2e+3*is2,I3e+3*is3,C)
		-   u(I1e+4*is1,I2e+4*is2,I3e+4*is3,C); 
	  if( twilightZoneFlow )
	    u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t)-4.*(*e)(c,I1e+  is1,I2e+  is2,I3e+  is3,C,t)
	      +6.*(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,C,t)
		-4.*(*e)(c,I1e+3*is1,I2e+3*is2,I3e+3*is3,C,t) 
		  +   (*e)(c,I1e+3*is1,I2e+3*is2,I3e+3*is3,C,t);
	  break;
	default:  //use 1st order for default
	  u(I1e,I2e,I3e,C)=u(I1e+is1,I2e+is2,I3e+is3,C);
	  if( twilightZoneFlow )
	    u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t)-(*e)(c,I1e+is1,I2e+is2,I3e+is3,C,t);
	  break;

	  /* ***don't know about the orderOfExtrapolation array
	     // general case:
	     binomial=orderOfExtrapolation(side,axis,i);
	     for( m=1; m<=orderOfExtrapolation(side,axis,i); m++ )
	     {
	     u(I1e,I2e,I3e,C)+=binomial*u(I1e+m*is1,I2e+m*is2,I3e+m*is3,C);
	     if( twilightZoneFlow )
	     u(I1e,I2e,I3e,C)-=binomial*(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,C,t);
	     binomial*=(m-orderOfExtrapolation(side,axis,i))/real(m+1);
	     }
	     
	     u(I1m,I2m,I3m,n) = u(I1,I2,I3,n);
	     
	     if( twilightZoneFlow )
	     u(I1e,I2e,I3e,C)+=(*e)(c,I1e,I2e,I3e,C,t);
	     */
	}
	
	break;

	// ====================
      case BCTypes::normalComponent:
	// for this boundary condition to work, you need to set the ghost value of u first, say,
	// by extrapolating. 
	// ====================

	//... we need an array to store the initial normal component
	if (!uDotNUpdated)
	{
	  uDotN.updateToMatchGrid (mappedGrid);
	  uDotNUpdated = TRUE;
	}

	//...if no, or not enough components have been specified, the default is the first "3"

	if( bcParameters.components.getLength(0)<c.numberOfDimensions() )
	{
	  n1=C.getBase();
	  n2=c.numberOfDimensions()>1 ? n1+1 : n1;
	  n3=c.numberOfDimensions()>2 ? n2+1 : n2;
	}
	//...otherwise get them from bcParameters 
	else
	{
	  n1=bcParameters.components(0);
	  n2=bcParameters.components(1);
	  n3=bcParameters.components(2);
	}

	if( min(min(n1,n2),n3)<u.getComponentBase(0) || max(max(n1,n2),n3) > u.getComponentBound(0) )
	{
	  cout << "MappedGridFiniteVolumeOperators::applyBoundaryConditions:ERROR applying a normalComponent BC\n";
	  printf("There is an invalid component, component0=%i, component1=%i",n1,n2);
	  if( c.numberOfDimensions()==3 )
	    printf(", component2=%i ",n3);
	  printf("\nu.getComponentBase(0) = %i, u.getComponentBound(0)=%i \n",	    
		 u.getComponentBase(0),u.getComponentBound(0));
	  return;
	}

	//...first compute the unscaled normal component at the cell faces

	if ( numberOfDimensions == 2 )
	  uDotN(I1,I2,I3) = HALF*(
				  (u(I1,I2,I3,n1) + u(I1m,I2m,I3m,n1))*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + (u(I1,I2,I3,n2) + u(I1m,I2m,I3m,n2))*faceNormal(I1b,I2b,I3b,yAxis,axis));
	
	else // if ( numberOfDimensions == 3)
	  uDotN(I1,I2,I3) = HALF*(
				  (u(I1,I2,I3,n1) + u(I1m,I2m,I3m,n1))*faceNormal(I1b,I2b,I3b,xAxis,axis)
				  + (u(I1,I2,I3,n2) + u(I1m,I2m,I3m,n2))*faceNormal(I1b,I2b,I3b,yAxis,axis)
				  + (u(I1,I2,I3,n3) + u(I1m,I2m,I3m,n3))*faceNormal(I1b,I2b,I3b,zAxis,axis));

	//...now add in the desired forcing for the normal component to the old normal component
	if ( twilightZoneFlow )
	{
	  if ( numberOfDimensions == 2 )
	    uDotN(I1,I2,I3) -= HALF*(
				     ((*e)(c,I1,I2,I3,n1,t) + (*e)(c,I1m,I2m,I3m,n1,t))*faceNormal(I1b,I2b,I3b,xAxis,axis)
				     + ((*e)(c,I1,I2,I3,n2,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,yAxis,axis));
	  else
	    uDotN(I1,I2,I3) -= HALF*(
				     ((*e)(c,I1,I2,I3,n1,t) + (*e)(c,I1m,I2m,I3m,n1,t))*faceNormal(I1b,I2b,I3b,xAxis,axis)
				     + ((*e)(c,I1,I2,I3,n2,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,yAxis,axis)
				     + ((*e)(c,I1,I2,I3,n3,t) + (*e)(c,I1m,I2m,I3m,n2,t))*faceNormal(I1b,I2b,I3b,zAxis,axis));
	}
	else if (scalarForcing)
	  uDotN(I1,I2,I3) -= scalarData;
	else if (gridFunctionForcing) //in this case, compute the normal component of the forcing grid function
	{
	  if (numberOfDimensions == 2)
	    uDotN(I1,I2,I3) -= 
	      gfData(I1m,I2m,I3m,xAxis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
		+ gfData(I1m,I2m,I3m,yAxis)*faceNormal(I1b,I2b,I3b,yAxis,axis);
	  else if (numberOfDimensions == 3)
	    uDotN(I1,I2,I3) -= 
	      gfData(I1m,I2m,I3m,xAxis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
		+ gfData(I1m,I2m,I3m,yAxis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
		  + gfData(I1m,I2m,I3m,zAxis)*faceNormal(I1b,I2b,I3b,zAxis,axis);
	}
	else
	  throw "Invalid value for bcOption!";
	
	//...now subtract out the normal component  so that u will have normal component = 0. scaling done at this point
	
	if (numberOfDimensions == 2)
	{
	  u(I1m,I2m,I3m,n1) +=  -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,xAxis,axis)/
	    (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
	     + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis));
	  u(I1m,I2m,I3m,n2) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,yAxis,axis)/
	    (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
	     + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis));
	}

	if (numberOfDimensions == 3)
	{
	  u(I1m,I2m,I3m,n1) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,xAxis,axis)/
	    (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
	     + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
	     + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	  u(I1m,I2m,I3m,n2) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,yAxis,axis)/
	    (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
	     + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
	     + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	  u(I1m,I2m,I3m,n3) += -TWO*uDotN(I1,I2,I3)*faceNormal(I1b,I2b,I3b,zAxis,axis)/
	    (  faceNormal(I1b,I2b,I3b,xAxis,axis)*faceNormal(I1b,I2b,I3b,xAxis,axis)
	     + faceNormal(I1b,I2b,I3b,yAxis,axis)*faceNormal(I1b,I2b,I3b,yAxis,axis)
	     + faceNormal(I1b,I2b,I3b,zAxis,axis)*faceNormal(I1b,I2b,I3b,zAxis,axis));
	}
	
	break;
	
      default:
	cout << "applyBoundaryCondition: unknown or un-implemented boundary condition = "
	  << bcType << endl;
	throw "MappedGridFiniteVolumeOperators::applyBoundaryCondition: fatal error! \n";
	


      }
    }
  }
}



/*  *** this is in the base class now (Overture.v1)

// new BC interface:
// fix corners and periodic update:
void MappedGridFiniteVolumeOperators::
finishBoundaryConditions(realMappedGridFunction & u )
{
  fixBoundaryCorners( u ); 
}
**** */
