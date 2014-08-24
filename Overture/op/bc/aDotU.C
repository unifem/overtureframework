#include "MappedGridOperators.h"
#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"

void MappedGridOperators::
applyBCaDotU(realMappedGridFunction & u, 
	     const int side,
	     const int axis,
	     const Index & Components,
	     const BCTypes::BCNames & boundaryConditionType,
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
	//
	// to set the component along a to g:
	//       u <- u + (g-(a.u)) a/<a,a>
	//
{
  MappedGrid & c = mappedGrid;
  real a1,a2,a3,aNorm;
  int n1,n2,n3,m1,m2,m3;
  
  #ifdef USE_PPP
    realSerialArray uA; getLocalArrayWithGhostBoundaries(u,uA);
    realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
  #else
    RealDistributedArray & uA = u;
    const realSerialArray & gfDataLocal = gfData;
  #endif

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

  int includeGhost=1;
  bool ok=ParallelUtility::getLocalArrayBounds(u,uA,I1,I2,I3,includeGhost);
  if( !ok ) return;

  RealArray uDotN(I1,I2,I3);

  //
  // to set the component along a to g:
  //       u <- u + (g-(a.u)) a/<a,a>
  //
  if( bcParameters.a.getLength(0)<c.numberOfDimensions() )
  {
    printf("MappedGridOperators::applyBoundaryConditions:ERROR applying the aDotU BC\n");
    printf(" The coefficients for `a' must be set in the BoundaryConditionParameters\n");
    exit(1);
  }
  a1=bcParameters.a(0);
  a2= c.numberOfDimensions()>1 ? bcParameters.a(1) : 0.;
  a3= c.numberOfDimensions()>2 ? bcParameters.a(2) : 0.;
  aNorm=a1*a1+a2*a2+a3*a3;
  if( aNorm==0. )
  {
    printf("MappedGridOperators::applyBoundaryConditions:ERROR applying a aDotU BC\n");
    printf(" The coefficients for `a' are all zero! side=%i, axis=%i\n",side,axis);
    exit(1);
  }
  aNorm=1./SQRT(aNorm); 
  a1*=aNorm;  a2*=aNorm;  a3*=aNorm;  // Normalize "a"
  getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"aDotU",uC,fC  );

  if( numberOfDimensions==2 )
    uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*a1+uA(I1,I2,I3,n2)*a2;
  else if( numberOfDimensions==3 )
    uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*a1+uA(I1,I2,I3,n2)*a2+uA(I1,I2,I3,n3)*a3;
  else 
    uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*a1;



  if( twilightZoneFlow ) 
  { // In this case we want to specify the value for a.u

    // *new* 070423
    realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(c.center(),xLocal);
    realSerialArray v(I1,I2,I3);
    bool isRectangular=false; // do this for now
    (*e).gd( v,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,m1,t);

    uDotN(I1,I2,I3)-=v*a1;
    if( numberOfDimensions==2 )
    {
      (*e).gd( v,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,m2,t);
      uDotN(I1,I2,I3)-=v*a2;
      
    }
    else if( numberOfDimensions==3 )
    {
      (*e).gd( v,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,m3,t);
      uDotN(I1,I2,I3)-=v*a3;
    }
    
//     if( numberOfDimensions==2 )      
//       uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*a1+(*e)(c,I1,I2,I3,m2,t)*a2;
//     else if( numberOfDimensions==3 )
//       uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*a1
// 	+(*e)(c,I1,I2,I3,m2,t)*a2
// 	+(*e)(c,I1,I2,I3,m3,t)*a3;
//     else 
//       uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*a1;

  }
  else if( bcOption==scalarForcing )
  {
    if( scalarData != 0. )
      uDotN(I1,I2,I3)-=scalarData*aNorm;  
  }
  else if( bcOption==arrayForcing )
  {
    if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
        arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
        arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
        arrayDataD.getBase(3)<=min(m1,m2,m3) && arrayDataD.getBound(3)>=max(m1,m2,m3) )
    {
      if( numberOfDimensions==2 ) 
	uDotN(I1,I2,I3)-=arrayDataD(I1,I2,I3,m1)*a1+arrayDataD(I1,I2,I3,m2)*a2;
      else if( numberOfDimensions==3 )
	uDotN(I1,I2,I3)-=arrayDataD(I1,I2,I3,m1)*a1+arrayDataD(I1,I2,I3,m2)*a2+arrayDataD(I1,I2,I3,m3)*a3;
      else
	uDotN(I1,I2,I3)-=arrayDataD(I1,I2,I3,m1)*a1;
    }
    else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
    {
      if( numberOfDimensions==2 ) 
	uDotN(I1,I2,I3)-=arrayData(m1,side,axis,grid)*a1
	  +arrayData(m2,side,axis,grid)*a2;
      else if( numberOfDimensions==3 )
	uDotN(I1,I2,I3)-=arrayData(m1,side,axis,grid)*a1
	  +arrayData(m2,side,axis,grid)*a2
	  +arrayData(m3,side,axis,grid)*a3;
      else
	uDotN(I1,I2,I3)-=arrayData(m1,side,axis,grid)*a1;
    }
    else
    {
      if( numberOfDimensions==2 ) 
	uDotN(I1,I2,I3)-=arrayData(m1)*a1
	  +arrayData(m2)*a2;
      else if( numberOfDimensions==3 )
	uDotN(I1,I2,I3)-=arrayData(m1)*a1
	  +arrayData(m2)*a2
	  +arrayData(m3)*a3;
      else
	uDotN(I1,I2,I3)-=arrayData(m1)*a1;
    }
  }
  else if( bcOption==gridFunctionForcing )
  {  // use user supplied variable values
    if( gfData.getComponentDimension(0) < numberOfDimensions )   
      uDotN(I1,I2,I3)-=gfDataLocal(I1,I2,I3,m1)*aNorm;
    else if( numberOfDimensions==2 ) 
      uDotN(I1,I2,I3)-=(gfDataLocal(I1,I2,I3,m1)*a1+
			gfDataLocal(I1,I2,I3,m2)*a2);
    else if( numberOfDimensions==3 )
      uDotN(I1,I2,I3)-=(gfDataLocal(I1,I2,I3,m1)*a1+
			gfDataLocal(I1,I2,I3,m2)*a2+
			gfDataLocal(I1,I2,I3,m3)*a3);
    else
      uDotN(I1,I2,I3)-=gfDataLocal(I1,I2,I3,0)*a1;
  }
  else
    {throw "Invalid value for bcOption in aDotU!";}

  WHERE_MASK( uA(I1,I2,I3,n1)-=uDotN(I1,I2,I3)*a1; );
  
  if( numberOfDimensions>1 )
  {
    WHERE_MASK( uA(I1,I2,I3,n2)-=uDotN(I1,I2,I3)*a2;  );
  }
  if( numberOfDimensions==3 )
  {
    WHERE_MASK( uA(I1,I2,I3,n3)-=uDotN(I1,I2,I3)*a3; );
  }
  
}
