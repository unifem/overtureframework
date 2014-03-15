#include "MappedGridOperators.h"
#include "SparseRep.h"

#include "MappedGridOperatorsInclude.h"
#include "display.h"

// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))

void MappedGridOperators::
applyBCnormalDotScalarGrad(realMappedGridFunction & u, 
		    const int side,
		    const int axis,
		    const Index & Components,
		    const BCTypes::BCNames & bcType,
		    const int & bc,
		    const real & scalarData,
		    const RealArray & arrayData,
	            const RealArray & arrayDataD,
		    const realMappedGridFunction & gfData,
		    const real & t,
                    const IntegerArray & uC, const IntegerArray & fC, IntegerDistributedArray & mask,
		    const BoundaryConditionParameters & bcParameters,
		    const BoundaryConditionOption bcOption,
		    const int & grid  )
// 
// Apply the normalDotScalarGrad BC
//
{
  real time=getCPU();
  
  if( orderOfAccuracy!=2 )
  {
    printf("MappedGridOperators:: Sorry, the normalDotScalarGrad boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }

  MappedGrid & c = mappedGrid;
  RealDistributedArray & uA = u;
  RealDistributedArray & normal  = mappedGrid.vertexBoundaryNormal(side,axis);   // make centerBoundaryNormal ***
  int n;

  Index I1p,I2p,I3p;
  getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); // first line in
  Index I1m,I2m,I3m;
  getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line
  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

  RealDistributedArray uDotN(I1,I2,I3);
  RealDistributedArray & rhs = uDotN;

  if(bcParameters.getVariableCoefficients(grid)==0 )
  {
    printf("MappedGridOperators::applyBoundaryCondition:normalDotScalarGrad:ERROR: The BoundaryConditionParameters \n"
	   " do not have a variableCoefficient grid function defined in them. \n"
	   "The normalDotScalarGrad boundary condition requires a grid function to use for the scalar.\n");
    {throw "error";}
  }
  RealMappedGridFunction & scalar = *bcParameters.getVariableCoefficients(grid);
  RealDistributedArray & s = scalar;

  if( rectangular || numberOfDimensions==1 )
  {
    // rectangular grid : NOTE: we do not have to worry about +/- twoDeltaX*g  because
    // the normal changes direction on either end introducing another +/-
    real twoDeltaX;
    if( rectangular )
      twoDeltaX = 2.*dx[0]; // 1./h21(axis);
    else // 1D, non-rectangular:   u.n = (+/-) (1/x.r) D0r u
      twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),axis1,axis1)
	*c.gridSpacing()(axis1);
	  
    const real factor = !usingConservativeApproximations() ? 1. : getAveragingType()==arithmeticAverage ? .5 : 2.;
    twoDeltaX/=factor;
    
    realArray a11m;
    if( usingConservativeApproximations() )  // ---- this only seems to be first order --- should fix ----
    {
      a11m.redim(I1,I2,I3);
      realArray a11(I1,I2,I3);
      if( getAveragingType()==arithmeticAverage )
      {
	a11m=(s(I1m,I2m,I3m)+s(I1 ,I2 ,I3 ));
	a11 =(s(I1 ,I2 ,I3 )+s(I1p,I2p,I3p));
      }
      else
      {
	a11m=s(I1m,I2m,I3m)*s(I1 ,I2 ,I3 )/(s(I1m,I2m,I3m)+s(I1 ,I2 ,I3 ));
	a11 =s(I1 ,I2 ,I3 )*s(I1p,I2p,I3p)/(s(I1 ,I2 ,I3 )+s(I1p,I2p,I3p));
      }
    
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=
		    ( a11*uA(I1p,I2p,I3p,uC(n)) + (a11m-a11)*uA(I1,I2,I3,uC(n)) )/a11m; );
	
      }
    }
    else
    {
      a11m.reference(s(I1,I2,I3));
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=uA(I1p,I2p,I3p,uC(n)); );
      }
    }
    if( twilightZoneFlow )
    {
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      {
	if( numberOfDimensions==2 )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=s(I1,I2,I3)*(e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)
				  +e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2))*(twoDeltaX)/a11m; )
	else if( numberOfDimensions==3 )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=s(I1,I2,I3)*(e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)
				  +e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2)
				  +e->z(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis3))*(twoDeltaX)/a11m; )
	else
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=s(I1,I2,I3)*e->x(c,I1,I2,I3,fC(n),t)*((2*side-1)*twoDeltaX)/a11m; )
      }
    }
    else if( bcOption==scalarForcing )
    {
      if( scalarData != 0. )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(scalarData*twoDeltaX)/a11m; )
    }
    else if( bcOption==arrayForcing )
    {
      #ifdef USE_PPP
	Overture::abort("MappedGridOperators::applyBCnormalDotScalarGrad:ERROR finish me Bill!");
      #else 
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(arrayDataD(I1,I2,I3,fC(n))*twoDeltaX)/a11m; );
      }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(arrayData(fC(n),side,axis,grid)*twoDeltaX)/a11m; )
      else
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(arrayData(fC(n))*twoDeltaX)/a11m; )
      #endif
    }
    else if( bcOption==gridFunctionForcing )
    {
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=gfData(I1,I2,I3,fC(n))*twoDeltaX/a11m; )
    }
    else
      {throw "Invalid value for bcOption! (neumann)";}
  }
  else 
  {
    // cout << "Boundary conditions: apply real normalDotScalarGrad BC\n";
    // generate coeff's for n.grad
    // Solve for the ghost point from: (n.grad)u=
    const int size=stencilSize;
    Index M(0,size);
    int is1 = (axis==axis1) ? 1-2*side : 0;   
    int is2 = (axis==axis2) ? 1-2*side : 0;           
    int is3 = (axis==axis3) ? 1-2*side : 0;           
    int mGhost = numberOfDimensions==2 ? M2(-is1,-is2) : M3(-is1,-is2,-is3);    // coefficient index for ghost value

          
    if( !normalDotScalarGradCoeffIsSet[axis][side] )
      createBoundaryMatrix(side,axis,bcType);

    #ifdef USE_PPP
      RealDistributedArray & nmCoeff = Overture::nullRealDistributedArray(); 
      Overture::abort("ERROR: fix this Bill!");
    #else
      RealDistributedArray & nmCoeff   = normalDotScalarGradCoeff[axis][side];
    #endif

    if( !normalDotScalarGradCoeffIsSet[axis][side] )
    { // generate coefficients if they have not already been set
      normalDotScalarGradCoeffIsSet[axis][side]=TRUE; 
      nmCoeff.redim(Range(M.getBase(),M.getBound()),  // dimension (to get base correct)
		    Range(I1.getBase(),I1.getBound()),
		    Range(I2.getBase(),I2.getBound()),
		    Range(I3.getBase(),I3.getBound()));

      Index MN(0,size*numberOfDimensions);
      RealDistributedArray opX,opY,opZ;
      opX.redim(MN,
		Range(I1.getBase(),I1.getBound()),
		Range(I2.getBase(),I2.getBound()),
		Range(I3.getBase(),I3.getBound()));

      opX=scalarGradCoefficients(scalar,I1,I2,I3,0,0)(MN,I1,I2,I3);
      normal.reshape(1,normal.dimension(0),normal.dimension(1),normal.dimension(2),normal.dimension(3));
      for( int m=M.getBase(); m<=M.getBound(); m++ )
      {
	opX(m,I1,I2,I3)*=normal(0,I1,I2,I3,axis1);
	opX(m+size,I1,I2,I3)*=normal(0,I1,I2,I3,axis2);
	if( numberOfDimensions==3 )
	  opX(m+2*size,I1,I2,I3)*=normal(0,I1,I2,I3,axis3);
      }
      normal.reshape(normal.dimension(1),normal.dimension(2),normal.dimension(3),normal.dimension(4));

      nmCoeff(M,I1,I2,I3)=opX(M,I1,I2,I3)+opX(M+size,I1,I2,I3);
      if( numberOfDimensions==3 )
	nmCoeff(M,I1,I2,I3)+=opX(M+2*size,I1,I2,I3);
      
    }
	  
    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
      WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=0.; )// zero this out so we can use it in the rhs of the expressions below

    uA.reshape(1,uA.dimension(0),uA.dimension(1),uA.dimension(2),uA.dimension(3));
    mask.reshape(1,mask.dimension(0),mask.dimension(1),mask.dimension(2));
    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
    {
      if( twilightZoneFlow )
      { 
	rhs(I1,I2,I3)=(e->x(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis1)+
		       e->y(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis2));
	if( numberOfDimensions==3 )
	  rhs(I1,I2,I3)+=e->z(c,I1,I2,I3,fC(n),t)*normal(I1,I2,I3,axis3);
        rhs(I1,I2,I3)*=s(I1,I2,I3);
      }
      else if( bcOption==scalarForcing )
	rhs(I1,I2,I3)=scalarData;
      else if( bcOption==arrayForcing )
      {
	if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
	  rhs(I1,I2,I3)=arrayData(fC(n),side,axis,grid);
	else
	  rhs(I1,I2,I3)=arrayData(fC(n));
      }
      else if( bcOption==gridFunctionForcing )
      {
	rhs(I1,I2,I3)=gfData(I1,I2,I3,fC(n));
      }
      else
	throw "Invalid value for bcOption! (normalScalarGrad)";

      rhs.reshape(1,rhs.dimension(0),rhs.dimension(1),rhs.dimension(2),rhs.dimension(3));
      if( numberOfDimensions==2 )
      {
	WHERE_MASK0( uA(0,I1m,I2m,I3m,uC(n))=( 
	  rhs(0,I1,I2,I3) - (
	    nmCoeff(M2( 0,-1),I1,I2,I3)*uA(0,I1  ,I2-1,I3,uC(n))+
	    nmCoeff(M2(-1, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3,uC(n))+
	    nmCoeff(M2( 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3,uC(n))+
	    nmCoeff(M2(+1, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3,uC(n))+
	    nmCoeff(M2( 0,+1),I1,I2,I3)*uA(0,I1  ,I2+1,I3,uC(n))
	    ))/nmCoeff(mGhost,I1,I2,I3); )
      }
      else
      {
	WHERE_MASK0( uA(0,I1m,I2m,I3m,uC(n))=( 
	  rhs(0,I1,I2,I3) - (
	     nmCoeff(M3( 0, 0,-1),I1,I2,I3)*uA(0,I1  ,I2  ,I3-1,uC(n))+
	     nmCoeff(M3( 0,-1, 0),I1,I2,I3)*uA(0,I1  ,I2-1,I3  ,uC(n))+
	     nmCoeff(M3(-1, 0, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3  ,uC(n))+
	     nmCoeff(M3( 0, 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3  ,uC(n))+
	     nmCoeff(M3(+1, 0, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3  ,uC(n))+
	     nmCoeff(M3( 0,+1, 0),I1,I2,I3)*uA(0,I1  ,I2+1,I3  ,uC(n))+
	     nmCoeff(M3( 0, 0,+1),I1,I2,I3)*uA(0,I1  ,I2  ,I3+1,uC(n))
	    ))/nmCoeff(mGhost,I1,I2,I3); )
      }
      rhs.reshape(rhs.dimension(1),rhs.dimension(2),rhs.dimension(3),rhs.dimension(4));
    }
    uA.reshape(uA.dimension(1),uA.dimension(2),uA.dimension(3),uA.dimension(4));
    mask.reshape(mask.dimension(1),mask.dimension(2),mask.dimension(3));
  }

  timeForNormalDotScalarGrad+=getCPU()-time;
}
// These are for indexing into a coefficient matrix
#undef M2
#undef M3
