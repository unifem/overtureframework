#include "MappedGridOperators.h"

#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"

void MappedGridOperators::
applyBCsymmetry(realMappedGridFunction & u, 
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
                const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		const BoundaryConditionParameters & bcParameters,
		const BoundaryConditionOption bcOption,
		const int & grid  )
{
  real time=getCPU();
  

  if( !boundaryNormalsUsed && bcType==BCTypes::vectorSymmetry && !rectangular )
  {
    boundaryNormalsUsed=true;
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
  }

  MappedGrid & c = mappedGrid;

  #ifdef USE_PPP
    const realSerialArray & uA = u.getLocalArray();
    const RealArray & normal  = boundaryNormalsUsed ? mappedGrid.vertexBoundaryNormalArray(side,axis) : uA;   
  #else
    RealDistributedArray & uA = u;
    RealDistributedArray & normal  = boundaryNormalsUsed ? mappedGrid.vertexBoundaryNormal(side,axis) : uA; 
  #endif

  int line,n,n1,n2,n3,m1,m2,m3;
  
  Index I1,I2,I3;
  Index I1p,I2p,I3p;
  Index I1m,I2m,I3m;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

//   getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); 
//   getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); 
  
  int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uA,I1,I2,I3,includeGhost); 
  if( !ok ) return;

  switch (bcType)
  {
  case evenSymmetry:
  case oddSymmetry:
  {
    //
    // Apply an even or odd symmetry condition to a scalar, u(-) = u(+) or u(-) = - u(+)
    //
    if( uC(uC.getBase(0))<u.getComponentBase(0) || uC(uC.getBound(0)) > u.getComponentBound(0) )
    {
      cout << "MappedGridOperators::applyBoundaryConditions:ERROR applying an evenSymmetry BC\n";
      printf("The values for the Components arg is invalid for u");
      return;
    }
    line = bcParameters.ghostLineToAssign;
    if( line > 2 || line < 0 )
      cout << "applyBoundaryConditions::ERROR? scalarSymmetry: ghost line to use=" << line << endl; 

    getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+line,bcParameters.extraInTangentialDirections); // ghost line to set
    getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-line,bcParameters.extraInTangentialDirections); // line in


    ok = ParallelUtility::getLocalArrayBounds(u,uA,I1m,I2m,I3m,includeGhost); 
    if( !ok ) return;
    ok = ParallelUtility::getLocalArrayBounds(u,uA,I1p,I2p,I3p,includeGhost); 
    if( !ok )
    {
      printf("applyBCsymmetry:even/oddSymmetry:ERROR:the ghost line is on this processor but not the first line in!\n"
             "    This boundary condition will not work in this case\n");
      Overture::abort("applyBCsymmetry:ERROR");
    }
    

    for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
    {
      if( bcType==BCTypes::BCNames(evenSymmetry) )
      {
        WHERE_MASK( uA(I1m,I2m,I3m,uC(n))= uA(I1p,I2p,I3p,uC(n)); );
      }
      else
      {
        WHERE_MASK( uA(I1m,I2m,I3m,uC(n))=-uA(I1p,I2p,I3p,uC(n)); );
      }
    }

    if( twilightZoneFlow ) 
    {  
      #ifdef USE_PPP
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);

        realSerialArray um(I1m,I2m,I3m), up(I1p,I2p,I3p);
	bool isRectangular=false; // do this for now
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	{
  	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,fC(n),t);
  	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,fC(n),t);
	  if( bcType==BCTypes::BCNames(evenSymmetry) )
	  {
	    WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=um-up; );
	  }
	  else
	  {
	    WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=um+up; );
	  }
	}
      #else
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	{
	  if( bcType==BCTypes::BCNames(evenSymmetry) )
	  {
	    WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(*e)(c,I1m,I2m,I3m,fC(n),t)-(*e)(c,I1p,I2p,I3p,fC(n),t); );
	  }
	  else
	  {
	    WHERE_MASK( uA(I1m,I2m,I3m,uC(n))+=(*e)(c,I1m,I2m,I3m,fC(n),t)+(*e)(c,I1p,I2p,I3p,fC(n),t); );
	  }
	}
      #endif
    }
    break;
  }
  case vectorSymmetry:
  {
    
    //
    // Apply a symmetry condition to a vector u=(u1,u2,u3)
    //    n.u is odd
    //    t.u is even
    getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"vectorSymmetry",uC,fC  );

    line = bcParameters.ghostLineToAssign;
    if( line > 3 || line < 0 )
      cout << "applyBoundaryConditions::ERROR? vectorSymmetry: ghost line to use=" << line << endl; 

    getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+line,bcParameters.extraInTangentialDirections); // ghost line to set
    getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-line,bcParameters.extraInTangentialDirections); // line in

    ok = ParallelUtility::getLocalArrayBounds(u,uA,I1m,I2m,I3m,includeGhost); 
    if( !ok ) return;
    ok = ParallelUtility::getLocalArrayBounds(u,uA,I1p,I2p,I3p,includeGhost); 
    if( !ok )
    {
      printf("applyBCsymmetry:vectorSymmetry:ERROR:the ghost line is on this processor but not the first line in!\n"
             "    This boundary condition will not work in this case\n");
      Overture::abort("applyBCsymmetry:ERROR");
    }

    if( rectangular )
    {
      // ******************** rectangular case **********************

      // nc = normal component
      // tc1 = tangential component 1
      int nc,tc1,tc2,m1c,m2c,m3c;
      if( axis==0 )
      {
	nc=n1, tc1=n2, tc2=n3, m1c=m1, m2c=m2, m3c=m3;
      }
      else if( axis==1 )
      {
	nc=n2, tc1=n1, tc2=n3, m1c=m2, m2c=m1, m3c=m3;
      }
      else
      {
	nc=n3, tc1=n1, tc2=n2, m1c=m3, m2c=m1, m3c=m2;
      }
      

      WHERE_MASK( uA(I1m,I2m,I3m,nc)=2.*uA(I1,I2,I3,nc)-uA(I1p,I2p,I3p,nc); )
      if( numberOfDimensions>=2 )
        WHERE_MASK( uA(I1m,I2m,I3m,tc1)=uA(I1p,I2p,I3p,tc1); )
      if( numberOfDimensions>=3 )
        WHERE_MASK( uA(I1m,I2m,I3m,tc2)=uA(I1p,I2p,I3p,tc2); )

      if( twilightZoneFlow ) 
      {  
       #ifdef USE_PPP
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);

        realSerialArray um(I1m,I2m,I3m), u0(I1,I2,I3), up(I1p,I2p,I3p);
	bool isRectangular=false; // do this for now

	(*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m1c,t);
  	(*e).gd( u0,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1 ,I2 ,I3 ,m1c,t);
  	(*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m1c,t);
	WHERE_MASK( uA(I1m,I2m,I3m,nc)+=um-2.*u0+up; ); 
	if( numberOfDimensions>=2 )
	{
	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m2c,t);
	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m2c,t);
          WHERE_MASK( uA(I1m,I2m,I3m,tc1)+=um-up; )
	}
	if( numberOfDimensions>=3 )
	{
	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m3c,t);
	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m3c,t);
          WHERE_MASK( uA(I1m,I2m,I3m,tc2)+=um-up; )
	}

       #else
	WHERE_MASK( uA(I1m,I2m,I3m,nc)+=(*e)(c,I1m,I2m,I3m,m1c,t)-2.*(*e)(c,I1,I2,I3,m1c,t)+(*e)(c,I1p,I2p,I3p,m1c,t); )
	if( numberOfDimensions>=2 )
	  WHERE_MASK( uA(I1m,I2m,I3m,tc1)+=(*e)(c,I1m,I2m,I3m,m2c,t)-(*e)(c,I1p,I2p,I3p,m2c,t); )
	if( numberOfDimensions>=3 )
	  WHERE_MASK( uA(I1m,I2m,I3m,tc2)+=(*e)(c,I1m,I2m,I3m,m3c,t)-(*e)(c,I1p,I2p,I3p,m3c,t); )
       #endif
      }
    }
    else
    {
      // ******************** curvilinear case **********************

      // first make all ALL components even
      WHERE_MASK( uA(I1m,I2m,I3m,n1)=uA(I1p,I2p,I3p,n1); )
      if( numberOfDimensions>1 )
      {
        WHERE_MASK( uA(I1m,I2m,I3m,n2)=uA(I1p,I2p,I3p,n2); )
        if( numberOfDimensions==3 )
            WHERE_MASK( uA(I1m,I2m,I3m,n3)=uA(I1p,I2p,I3p,n3); )
      }

      if( twilightZoneFlow ) 
      {  
       #ifdef USE_PPP
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);

        realSerialArray um(I1m,I2m,I3m), up(I1p,I2p,I3p);
	bool isRectangular=false; // do this for now

	(*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m1,t);
  	(*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m1,t);
	WHERE_MASK( uA(I1m,I2m,I3m,n1)+=um-up; ); 
	if( numberOfDimensions>=2 )
	{
	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m2,t);
	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m2,t);
          WHERE_MASK( uA(I1m,I2m,I3m,n2)+=um-up; )
	}
	if( numberOfDimensions>=3 )
	{
	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m3,t);
	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m3,t);
          WHERE_MASK( uA(I1m,I2m,I3m,n3)+=um-up; )
	}

       #else

	WHERE_MASK( uA(I1m,I2m,I3m,n1)+=(*e)(c,I1m,I2m,I3m,m1,t)-(*e)(c,I1p,I2p,I3p,m1,t); )
	if( numberOfDimensions>1 )
	{
	  WHERE_MASK( uA(I1m,I2m,I3m,n2)+=(*e)(c,I1m,I2m,I3m,m2,t)-(*e)(c,I1p,I2p,I3p,m2,t); )
	  if( numberOfDimensions==3 )
	    WHERE_MASK( uA(I1m,I2m,I3m,n3)+=(*e)(c,I1m,I2m,I3m,m3,t)-(*e)(c,I1p,I2p,I3p,m3,t); )
	}
       #endif
      }

      // now fix normal component
    
      realSerialArray uDotN(I1,I2,I3);	

      if( numberOfDimensions==2 )
      {
	uDotN(I1,I2,I3)=(-uA(I1p,I2p,I3p,n1)+2.*uA(I1,I2,I3,n1)-uA(I1m,I2m,I3m,n1))*normal(I1,I2,I3,0)
	  +(-uA(I1p,I2p,I3p,n2)+2.*uA(I1,I2,I3,n2)-uA(I1m,I2m,I3m,n2))*normal(I1,I2,I3,1);
      }
      else if( numberOfDimensions==3 )
      {
	uDotN(I1,I2,I3)=(-uA(I1p,I2p,I3p,n1)+2.*uA(I1,I2,I3,n1)-uA(I1m,I2m,I3m,n1))*normal(I1,I2,I3,0)
	  +(-uA(I1p,I2p,I3p,n2)+2.*uA(I1,I2,I3,n2)-uA(I1m,I2m,I3m,n2))*normal(I1,I2,I3,1) 
	  +(-uA(I1p,I2p,I3p,n3)+2.*uA(I1,I2,I3,n3)-uA(I1m,I2m,I3m,n3))*normal(I1,I2,I3,2);
      }
      else  // outward normal in 1D is 2*side-1
	uDotN(I1,I2,I3)=(-uA(I1p,I2p,I3p,n1)+2.*uA(I1,I2,I3,n1)-uA(I1m,I2m,I3m,n1))*(2*side-1); 

      if( twilightZoneFlow ) 
      {  
       #ifdef USE_PPP
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);

        realSerialArray um(I1m,I2m,I3m), u0(I1,I2,I3), up(I1p,I2p,I3p);
	bool isRectangular=false; // do this for now

	(*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m1,t);
  	(*e).gd( u0,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1 ,I2 ,I3 ,m1,t);
  	(*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m1,t);

        if( numberOfDimensions==1 )
	{
          uDotN(I1,I2,I3)+=( um-2.*u0+up)*(2*side-1);
	}
	else
	{
          uDotN(I1,I2,I3)+=( um-2.*u0+up )*normal(I1,I2,I3,0);

	  (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m2,t);
	  (*e).gd( u0,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1 ,I2 ,I3 ,m2,t);
	  (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m2,t);
          uDotN(I1,I2,I3)+=( um-2.*u0+up )*normal(I1,I2,I3,1);
          
	  if( numberOfDimensions==3 )
	  {
	    (*e).gd( um,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1m,I2m,I3m,m3,t);
	    (*e).gd( u0,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1 ,I2 ,I3 ,m3,t);
	    (*e).gd( up,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1p,I2p,I3p,m3,t);

	    uDotN(I1,I2,I3)+=( um-2.*u0+up )*normal(I1,I2,I3,2);
	  }
	  
	}
       #else
	if( numberOfDimensions==2 )
	  uDotN(I1,I2,I3)+=((*e)(c,I1m,I2m,I3m,m1,t)
			    -2.*(*e)(c,I1 ,I2 ,I3 ,m1,t)+(*e)(c,I1p,I2p,I3p,m1,t))*normal(I1,I2,I3,0)
	    +((*e)(c,I1m,I2m,I3m,m2,t)
	      -2.*(*e)(c,I1 ,I2 ,I3 ,m2,t)+(*e)(c,I1p,I2p,I3p,m2,t))*normal(I1,I2,I3,1);
	else if( numberOfDimensions==3  )
	  uDotN(I1,I2,I3)+=((*e)(c,I1m,I2m,I3m,m1,t)
			    -2.*(*e)(c,I1 ,I2 ,I3 ,m1,t)+(*e)(c,I1p,I2p,I3p,m1,t))*normal(I1,I2,I3,0)
	    +((*e)(c,I1m,I2m,I3m,m2,t)
	      -2.*(*e)(c,I1 ,I2 ,I3 ,m2,t)+(*e)(c,I1p,I2p,I3p,m2,t))*normal(I1,I2,I3,1)
	    +((*e)(c,I1m,I2m,I3m,m3,t)
	      -2.*(*e)(c,I1 ,I2 ,I3 ,m3,t)+(*e)(c,I1p,I2p,I3p,m3,t))*normal(I1,I2,I3,2);
	else
	  uDotN(I1,I2,I3)+=((*e)(c,I1m,I2m,I3m,m1,t)
			    -2.*(*e)(c,I1 ,I2 ,I3 ,m1,t)+(*e)(c,I1p,I2p,I3p,m1,t))*(2*side-1);
       #endif
      }

      // now fix up the normal component
      //     n.u(Im) = n.u(Ip) + g
      if( numberOfDimensions>1 )
      {
	WHERE_MASK( uA(I1m,I2m,I3m,n1)+=uDotN(I1,I2,I3)*normal(I1,I2,I3,0); );
	WHERE_MASK( uA(I1m,I2m,I3m,n2)+=uDotN(I1,I2,I3)*normal(I1,I2,I3,1); );
	if( numberOfDimensions==3 ) 
	  WHERE_MASK( uA(I1m,I2m,I3m,n3)+=uDotN(I1,I2,I3)*normal(I1,I2,I3,2); );
      }
      else
      {
	WHERE_MASK( uA(I1m,I2m,I3m,n1)+=uDotN(I1,I2,I3)*(2*side-1); );
      }
      
    }
    break;
  }
  default:
    Overture::abort("BC: symmetry:ERROR: unknown case.");
    break;
    
  } // end switch
  

  timeForSymmetry+=getCPU()-time;
}


