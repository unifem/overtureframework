#include "MappedGridOperators.h"
#include "SparseRep.h"

#include "MappedGridOperatorsInclude.h"

// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))

void MappedGridOperators::
applyBCgeneralMixedDerivative(realMappedGridFunction & u, 
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
{
  real time=getCPU();
  
  if( orderOfAccuracy!=2 )
  {
    printF("MappedGridOperators:: Sorry, the general mixed boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }
  MappedGrid & c = mappedGrid;
  RealDistributedArray & uA = u;
  int n;

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);
  Index I1m,I2m,I3m;
  getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line
  Index I1p,I2p,I3p;
  getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); // first line in

  RealDistributedArray uDotN(I1,I2,I3);

  n=uC.getBase(0); // ********* fix this ******
  Index M(0,int(pow(3,numberOfDimensions)+.5));
  if( bcParameters.a.getLength(0)<c.numberOfDimensions()+1 )
  {
    printF("MappedGridOperators::applyBoundaryConditions:ERROR applying the "
	   "generalMixedDerivative BC\n");
    printF(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
    Overture::abort("error");
  }
  real b0=bcParameters.a(0);
  real b1=bcParameters.a(1);
  real b2= c.numberOfDimensions()>1 ? bcParameters.a(2) : 0.;
  real b3= c.numberOfDimensions()>2 ? bcParameters.a(3) : 0.;

  int is1 = (axis==axis1) ? 1-2*side : 0;   
  int is2 = (axis==axis2) ? 1-2*side : 0;           
  int is3 = (axis==axis3) ? 1-2*side : 0;           
  RealDistributedArray & rhs = uDotN;


  for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
  {
    int nu=uC(n);
    // now fill in the rhs
    if( twilightZoneFlow ) 
    { 
      if( numberOfDimensions==2 )
	rhs(I1,I2,I3)= (*e)(c,I1,I2,I3,fC(n),t)*b0+e->x(c,I1,I2,I3,fC(n),t)*b1+e->y(c,I1,I2,I3,fC(n),t)*b2;
      else if( numberOfDimensions==3 )
	rhs(I1,I2,I3)= (*e)(c,I1,I2,I3,fC(n),t)*b0+e->x(c,I1,I2,I3,fC(n),t)*b1+e->y(c,I1,I2,I3,fC(n),t)*b2+
	  e->z(c,I1,I2,I3,fC(n),t)*b3;
      else 
	rhs(I1,I2,I3)= (*e)(c,I1,I2,I3,fC(n),t)*b0+e->x(c,I1,I2,I3,fC(n),t)*b1;
    }
    else if( bcOption==scalarForcing )
      rhs(I1,I2,I3)=scalarData;
    else if( bcOption==arrayForcing )
    {
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
        #ifdef USE_PPP
	  Overture::abort("MappedGridOperators::applyBCgeneralMixedDerivative:ERROR finish me Bill!");
        #else        
	  rhs(I1,I2,I3)=arrayDataD(I1,I2,I3,fC(n));
        #endif
      }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
	rhs(I1,I2,I3)=arrayData(fC(n),side,axis,grid);
      else
	rhs(I1,I2,I3)=arrayData(fC(n));
    }
    else if( bcOption==gridFunctionForcing )
    {
      rhs(I1,I2,I3)=gfData(I1,I2,I3,fC(n));
    }
    else 
    {
      printf("Invalid bcOption for BC generalMixedDerivative = %i\n",bcOption);
      {throw "Invalid bcOption for BC generalMixedDerivative";}
    }
	    
    if( rectangular || numberOfDimensions==1 )
    {
      // ****************
      // ***rectangular**
      // ****************

      //  b0*u + b1 u_x + b2 u_y + b3 u_z = g
      real twoDeltaX = 2.*dx[axis1]; // 1./h21(axis1);
      if( !rectangular )
      {
	// 1D, non-rectangular:   u.n = (+/-) (1/x.r) D0r u
	twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),axis1,axis1)*c.gridSpacing()(axis1);
      }
      real twoDeltaY = 2.*dx[axis2]; //1./h21(axis2);
      if( numberOfDimensions==1 )
      {
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu) +((2*side-1)*twoDeltaX/b1)*
            ( rhs(I1,I2,I3) -b0*uA(I1,I2,I3,nu) ); )
      }
      else if( numberOfDimensions==2 )
      {
	if( axis==axis1 )
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu)
	    +((2*side-1)*twoDeltaX/b1)*
            ( rhs(I1,I2,I3) -b0*uA(I1,I2,I3,nu) - (uA(I1,I2+1,I3,nu)-uA(I1,I2-1,I3,nu))*(b2/twoDeltaY) ); )
	}
	else
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu)
	    +((2*side-1)*twoDeltaY/b2)*
	    ( rhs(I1,I2,I3) -b0*uA(I1,I2,I3,nu) - (uA(I1+1,I2,I3,nu)-uA(I1-1,I2,I3,nu))*(b1/twoDeltaX) ); )
	}
      }
      else
      {
	real twoDeltaZ = 2.*dx[axis3]; //1./h21(axis3);
	if( axis==axis1 )
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu)
	    +((2*side-1)*twoDeltaX/b1)*
            ( rhs(I1,I2,I3)  -b0*uA(I1,I2,I3,nu) - (uA(I1  ,I2+1,I3  ,nu)-uA(I1  ,I2-1,I3  ,nu))*(b2/twoDeltaY)
	      - (uA(I1  ,I2  ,I3+1,nu)-uA(I1  ,I2  ,I3-1,nu))*(b3/twoDeltaZ) ); )
	}
	else if( axis==axis2 )
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu)
	    +((2*side-1)*twoDeltaY/b2)*
            ( rhs(I1,I2,I3)  -b0*uA(I1,I2,I3,nu) - (uA(I1+1,I2  ,I3  ,nu)-uA(I1-1,I2  ,I3  ,nu))*(b1/twoDeltaX)
	      - (uA(I1  ,I2  ,I3+1,nu)-uA(I1  ,I2  ,I3-1,nu))*(b3/twoDeltaZ) ); )
	}
	else
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,nu)=uA(I1p,I2p,I3p,nu)
	    +((2*side-1)*twoDeltaZ/b3)*
            ( rhs(I1,I2,I3)  -b0*uA(I1,I2,I3,nu) - (uA(I1+1,I2  ,I3  ,nu)-uA(I1-1,I2  ,I3  ,nu))*(b1/twoDeltaX)
	      - (uA(I1  ,I2+1,I3  ,nu)-uA(I1  ,I2-1,I3  ,nu))*(b2/twoDeltaY) ); )
	}
      }
    }
    else
    {
      int mGhost = numberOfDimensions==2 ? M2(-is1,-is2) : M3(-is1,-is2,-is3);  // coefficient index for ghost value

      if( !gCoeffIsSet[axis][side] )
	createBoundaryMatrix(side,axis,bcType);

      #ifdef USE_PPP
        RealDistributedArray & gmCoeff = Overture::nullRealDistributedArray(); 
	Overture::abort("ERROR: fix this Bill!");
      #else
        RealDistributedArray & gmCoeff = generalMixedDerivativeCoeff[axis][side]; 
      #endif

      //  solve for the ghost value of u from the equation coeff*u = g

      #define coeffSaved(side,axis,m) (gCoeffValues[(side)+2*((axis)+3*(m))])
      bool buildMatrix = !gCoeffIsSet[axis][side];
      if( !buildMatrix ) // *wdh* 080724 we need to check if the coeff's have changed!
      {
	buildMatrix = (b0 !=coeffSaved(side,axis,0)) || (b1 !=coeffSaved(side,axis,1)) || 
                      (b2 !=coeffSaved(side,axis,2)) || (b3 !=coeffSaved(side,axis,3));
	// if( buildMatrix )
	//  printF("\n +++MappedGridOperators::genMixed BC:INFO: coefficients have changed! Rebuild BC matrix +++ \n\n");
      }

      if( buildMatrix )
      { // generate coefficients if they have not already been set
	gCoeffIsSet[axis][side]=true; 
	gmCoeff.resize(Range(M.getBase(),M.getBound()),  // dimension (to get base correct)
		       Range(I1.getBase(),I1.getBound()),
		       Range(I2.getBase(),I2.getBound()),
		       Range(I3.getBase(),I3.getBound()));

	if( numberOfDimensions==2 )
	{
	  gmCoeff=b0*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3) + b1*xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)+
	    b2*yCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
	}
	else
	{
	  gmCoeff=b0*identityCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3) + b1*xCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)+
	    b2*yCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3)+ b3*zCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
	}
        if( min(abs(gmCoeff(mGhost,I1,I2,I3)))==0. )
	{
	  printF("applyBoundaryCondition::applyBCgeneralMixedDerivative:ERROR: the coefficient we need to\n"
                  " divide by is zero at some points! \n"
	    " I am going to set these values to 1. (arbitrary) answers will be wrong! \n");
          where( gmCoeff(mGhost,I1,I2,I3)==0. )
            gmCoeff(mGhost,I1,I2,I3)=1.;
	}
        gmCoeff(mGhost,I1,I2,I3)=1./gmCoeff(mGhost,I1,I2,I3); // divide here for efficiency

	coeffSaved(side,axis,0)=b0;  // save the coefficients so we can check if they are changed 
	coeffSaved(side,axis,1)=b1;
	coeffSaved(side,axis,2)=b2;
	coeffSaved(side,axis,3)=b3;
      } 

      // set the ghost values to zero, this allows us to just multiply out all values
      // in the long equation that appears on the following line.
      uA(I1m,I2m,I3m,nu)=0.;   

      uA.reshape(1,uA.dimension(0),uA.dimension(1),uA.dimension(2),uA.dimension(3));
      rhs.reshape(1,rhs.dimension(0),rhs.dimension(1),rhs.dimension(2),rhs.dimension(3));
      mask.reshape(1,mask.dimension(0),mask.dimension(1),mask.dimension(2));
      
      if( numberOfDimensions==2 )
      {
	WHERE_MASK0( uA(0,I1m,I2m,I3m,nu)=( 
	  rhs(0,I1,I2,I3) - (
	     gmCoeff(M2( 0,-1),I1,I2,I3)*uA(0,I1  ,I2-1,I3,nu)
	    +gmCoeff(M2(-1, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3,nu)
	    +gmCoeff(M2( 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3,nu)
	    +gmCoeff(M2(+1, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3,nu)
	    +gmCoeff(M2( 0,+1),I1,I2,I3)*uA(0,I1  ,I2+1,I3,nu)
	    ))*gmCoeff(mGhost,I1,I2,I3); )  // note that we multiply here since we have inverted this already
      }
      else
      {
	WHERE_MASK0( uA(0,I1m,I2m,I3m,nu)=( 
	  rhs(0,I1,I2,I3) - (
	     gmCoeff(M3( 0, 0,-1),I1,I2,I3)*uA(0,I1  ,I2  ,I3-1,nu)
	    +gmCoeff(M3( 0,-1, 0),I1,I2,I3)*uA(0,I1  ,I2-1,I3  ,nu)
	    +gmCoeff(M3(-1, 0, 0),I1,I2,I3)*uA(0,I1-1,I2  ,I3  ,nu)
	    +gmCoeff(M3( 0, 0, 0),I1,I2,I3)*uA(0,I1  ,I2  ,I3  ,nu)
	    +gmCoeff(M3(+1, 0, 0),I1,I2,I3)*uA(0,I1+1,I2  ,I3  ,nu)
	    +gmCoeff(M3( 0,+1, 0),I1,I2,I3)*uA(0,I1  ,I2+1,I3  ,nu)
	    +gmCoeff(M3( 0, 0,+1),I1,I2,I3)*uA(0,I1  ,I2  ,I3+1,nu)
	    ))*gmCoeff(mGhost,I1,I2,I3); )  // note that we multiply here since we have inverted this already
      }

      uA.reshape(uA.dimension(1),uA.dimension(2),uA.dimension(3),uA.dimension(4));
      rhs.reshape(rhs.dimension(1),rhs.dimension(2),rhs.dimension(3),rhs.dimension(4));
      mask.reshape(mask.dimension(1),mask.dimension(2),mask.dimension(3));

    }

  }  // end for n
  
  timeForGeneralMixedDerivative+=getCPU()-time;
}
#undef M2
#undef M3
