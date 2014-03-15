#include "MappedGridOperators.h"

#include "MappedGridOperatorsInclude.h"

#include "ParallelUtility.h"

#define extrapolateOpt EXTERN_C_NAME(extrapolateopt)
extern "C"
{
  void extrapolateOpt(const int&nd, 
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,
      const int&ndu3a,const int&ndu3b,const int&ndu4a,const int&ndu4b,
      const int&ndv1a,const int&ndv1b,const int&ndv2a,const int&ndv2b,
      const int&ndv3a,const int&ndv3b,const int&ndv4a,const int&ndv4b,
      const int&ndm1a,const int&ndm1b,const int&ndm2a,const int&ndm2b,const int&ndm3a,const int&ndm3b,
      real &u,const int&mask, const real&v, const int&ipar, const real&rpar, const int & uC );
}




void MappedGridOperators::
applyBCextrapolate(realMappedGridFunction & u, 
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
  const int myid=Communication_Manager::My_Process_Number;
  
  if( !boundaryNormalsUsed && bcType==BCTypes::extrapolateNormalComponent)
  {
    boundaryNormalsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
  }
  if( !boundaryTangentsUsed && 
     ( bcType==BCTypes::extrapolateTangentialComponent0 || bcType==BCTypes::extrapolateTangentialComponent1) )
  {
    boundaryTangentsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEcenterBoundaryTangent);
  }


  MappedGrid & c1 = mappedGrid;
  MappedGrid & c = *u.getMappedGrid();  // *wdh* 100813
  if( max(abs(c.dimension()-c1.dimension()))!=0 )
  {
    printF("MappedGridOperators::applyBCextrapolate:ERROR: The mappedGrid in the operators does NOT match the one in the grid function!\n"
           " Operators:    mg.dimension()=[%i,%i][%i,%i][%i,%i]\n"
           " GridFunction: mg.dimension()=[%i,%i][%i,%i][%i,%i]\n",
	   c1.dimension(0,0),c1.dimension(1,0),c1.dimension(0,1),c1.dimension(1,2),c1.dimension(0,2),c1.dimension(1,2),
	   c.dimension(0,0),c.dimension(1,0),c.dimension(0,1),c.dimension(1,2),c.dimension(0,2),c.dimension(1,2)
      );
    
    // OV_ABORT("ERROR");
  }
  

  RealDistributedArray & uA = u;
  int m,n,line,is1,is2,is3,nt,n1,n2,n3,m1,m2,m3;
  real binomial;
  int v0,v1,v2;

  line = bcParameters.ghostLineToAssign;
  if( line > 6 || line < 0 )
  {
    printF("MGOP:applyBCextrapolate::ERROR? extrapolating ghost line %i.\n",line);
  }

  int orderOfExtrap = bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1
    : bcParameters.orderOfExtrapolation;
 
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(uA,uLocal);
  #else
    const realArray & uLocal = uA;
  #endif

  #ifdef USE_PPP
    // here is how many lines we have for extrapolating in parallel:
    const int nx=min(c.gridIndexRange(1,axis),uLocal.getBound(axis))-
                 max(c.gridIndexRange(0,axis),uLocal.getBase(axis))  +1; 
  #else
    const int nx=c.gridIndexRange(End,axis)-c.gridIndexRange(Start,axis)+1;
  #endif

  if( orderOfExtrap >= nx+line && nx>0 )
  {
    // reduce order of extrap if we only have a few grid points
    // The extrap formula uses "orderOfExtrap+1" grid points -- we should not be coupled to the ghost
    //   points on the opposite boundary
    orderOfExtrap = nx+line-1;
    #ifdef USE_PPP
      printf("MappedGridOperators::applyBCextrapolate: WARNING: p=%i reducing order of extrapolation to %i "
	     "since number of grid points =%i, grid=%i local bounds=[%i,%i] (for axis=%i)\n",
             myid,orderOfExtrap,nx,grid,
	     max(c.gridIndexRange(0,axis),uLocal.getBase(axis)), 
             min(c.gridIndexRange(1,axis),uLocal.getBound(axis)),axis);
    #else
      printf("MappedGridOperators::applyBCextrapolate: WARNING: reducing order of extrapolation to %i "
            "since number of grid points =%i, grid=%i gid=[%i,%i][%i,%i][%i,%i]\n",orderOfExtrap,nx,grid,
            c.gridIndexRange(0,0),c.gridIndexRange(1,0),
            c.gridIndexRange(0,1),c.gridIndexRange(1,1),
            c.gridIndexRange(0,2),c.gridIndexRange(1,2) );
    #endif
  }
  

  // Index I1e,I2e,I3e;
  Index Iev[3], &I1e=Iev[0], &I2e=Iev[1], &I3e=Iev[2];

  const int useWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                           bcParameters.getUseMask() ;

  #ifdef USE_PPP
    bool useOpt=bcType==BCTypes::extrapolate && bcOption==scalarForcing && scalarData==0;
  #else
    bool useOpt=bcType==BCTypes::extrapolate && bcOption==scalarForcing && scalarData==0. && twilightZoneFlow==0;
  #endif
  if( useOpt ) 
  {

    getGhostIndex( c.extendedIndexRange(),side,axis,I1e,I2e,I3e,line,bcParameters.extraInTangentialDirections); // line to extrapolate
    // *wdh* 100813 -- do the following so that we get the correct ghost line for bc==0 boundaries. This is probably only
    // an issue for AMR: also changed BoundaryOperators.C lines 619 and 668 to extrap ghost line 2,1 (instead of 1,0) for AMR interp boundaries
    Iev[axis]=c.gridIndexRange(side,axis) - line*(1-2*side);  
    
    if( bcParameters.extraInTangentialDirections>0 )
    {
      // Make sure we have not exceeded the dimension bounds *wdh* 091120 
      const IntegerArray & dim = c.dimension();
      I1e = Range( max(I1e.getBase(),dim(0,0)), min(I1e.getBound(),dim(1,0)) );
      I2e = Range( max(I2e.getBase(),dim(0,1)), min(I2e.getBound(),dim(1,1)) );
      I3e = Range( max(I3e.getBase(),dim(0,2)), min(I3e.getBound(),dim(1,2)) );
      
    }
    
    is1 = (axis==axis1) ? 1-2*side : 0;   
    is2 = (axis==axis2) ? 1-2*side : 0;           
    is3 = (axis==axis3) ? 1-2*side : 0;           

    int ca=uC.getBase(0), cb=uC.getBound(0);
    int ipar[] ={(int)bcType,useWhereMask,orderOfExtrap,ca,cb,is1,is2,is3,
                 (int)bcParameters.extrapolationOption };
    const real uEps=1000.*REAL_MIN; // for limited extrapolation
    real rpar[]={bcParameters.extrapolateWithLimiterParameters[0],
                 bcParameters.extrapolateWithLimiterParameters[1],
                 uEps}; //


    #ifdef USE_PPP
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      const IntegerArray & dimension = c.dimension();
      
      int n1a=max(I1e.getBase(),uLocal.getBase(0)), n1b=min(I1e.getBound(),uLocal.getBound(0));
      int n2a=max(I2e.getBase(),uLocal.getBase(1)), n2b=min(I2e.getBound(),uLocal.getBound(1));
      int n3a=max(I3e.getBase(),uLocal.getBase(2)), n3b=min(I3e.getBound(),uLocal.getBound(2));
      
      if( n1a>n1b || n2a>n2b || n3a>n3b || 
          (side==0 && uLocal.getBase(axis) >dimension(0,axis) ) ||
          (side==1 && uLocal.getBound(axis)<dimension(1,axis) ) )
      {
        // The local array does not include the boundary 
        return;
      }

      I1e = Range(n1a,n1b); 
      I2e = Range(n2a,n2b); 
      I3e = Range(n3a,n3b); 
    #else
      const intArray & maskLocal = mask;
    #endif

    const realSerialArray & v = uLocal; // this will be normal or tangent in future

//     printf(" extrapolateOpt: extrap points [%i,%i][%i,%i][%i,%i] bounds=[%i,%i][%i,%i][%i,%i] p=%i useWhereMask=%i\n",
//            I1e.getBase(),I1e.getBound(),I2e.getBase(),I2e.getBound(), I3e.getBase(),I3e.getBound(),
//            uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
//             uLocal.getBase(2),uLocal.getBound(2),
// 	   myid,useWhereMask);
    
    extrapolateOpt(c.numberOfDimensions(), 
		   I1e.getBase(),I1e.getBound(),
		   I2e.getBase(),I2e.getBound(),
		   I3e.getBase(),I3e.getBound(),
		   uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
		   uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
		   v.getBase(0),v.getBound(0),v.getBase(1),v.getBound(1),
		   v.getBase(2),v.getBound(2),v.getBase(3),v.getBound(3),
		   maskLocal.getBase(0),maskLocal.getBound(0),maskLocal.getBase(1),maskLocal.getBound(1),
		   maskLocal.getBase(2),maskLocal.getBound(2),
		   *getDataPointer(uLocal),*getDataPointer(maskLocal), *getDataPointer(v), ipar[0], rpar[0], uC(0) );
      

    timeForExtrapolate+=getCPU()-time;
    return;
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);
  // *wdh* 091201: make sure that the Index's are not too large in the tangential directions
  if( bcParameters.extraInTangentialDirections>0 )
    for( int dir=0; dir<c.numberOfDimensions(); dir++ )
      if( dir!=axis )
	Iv[dir]=Range(max(c.dimension(0,dir),Iv[dir].getBase()),min(c.dimension(1,dir),Iv[dir].getBound()));

  RealDistributedArray uDotN(I1,I2,I3);

  // **** fix this for P++

  switch (bcType)
  {
  case extrapolate:
    line = bcParameters.ghostLineToAssign;
    if( line > 6 || line < 0 )
    {
      printF("MGOP:applyBCextrapolate::ERROR? extrapolating ghost line %i.\n",line);
    }

    // *wdh* 000310 getGhostIndex( c.indexRange(),side,axis,I1e,I2e,I3e,line); // line to extrapolate
    getGhostIndex( c.extendedIndexRange(),side,axis,I1e,I2e,I3e,line,bcParameters.extraInTangentialDirections); // line to extrapolate
    // *wdh* 100813 -- do the following so that we get the correct ghost line for bc==0 boundaries. This is probably only
    // an issue for AMR: also changed BoundaryOperators.C lines 619 and 668 to extrap ghost line 2,1 (instead of 1,0) for AMR interp boundaries
    Iev[axis]=c.gridIndexRange(side,axis) - line*(1-2*side);  


    // *wdh* 091201: make sure that the Index's are not too large in the tangential directions
    if( bcParameters.extraInTangentialDirections>0 )
      for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	if( dir!=axis )
	  Iev[dir]=Range(max(c.dimension(0,dir),Iev[dir].getBase()),min(c.dimension(1,dir),Iev[dir].getBound()));

    //if( I2e.getBase()<uA.getBase(1) )
    //{
    //  printF("extrapolate:ERROR: i2e=[%i,%i] dim=[%i,%i] extendedIndexRange=[%i,%i] uA=[%i,%i]\n",I2e.getBase(),I2e.getBound(),
    //	     c.dimension(0,1),c.dimension(1,1),c.extendedIndexRange(0,1),c.extendedIndexRange(1,1),
    //	     uA.getBase(1),uA.getBound(1));
    //}
    

    is1 = (axis==axis1) ? 1-2*side : 0;   
    is2 = (axis==axis2) ? 1-2*side : 0;           
    is3 = (axis==axis3) ? 1-2*side : 0;           
    if( orderOfExtrap < 0 || orderOfExtrap>20 )
    {
      printF("MGOP:applyBCextrapolate::ERROR? orderOfExtrapolation = %i not implemented.\n",orderOfExtrap);
      OV_ABORT("ERROR");
    }
    switch( orderOfExtrap )
    {  // extrapolate to the given order
    case 1:
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=uA(I1e+is1,I2e+is2,I3e+is3,uC(n)); )
      if( twilightZoneFlow==2 )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))+=(*e)(c,I1e,I2e,I3e,fC(n),t)-(*e)(c,I1e+is1,I2e+is2,I3e+is3,fC(n),t); )
      break;
    case 2:
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=2.*uA(I1e+  is1,I2e+  is2,I3e+  is3,uC(n))
	  -uA(I1e+2*is1,I2e+2*is2,I3e+2*is3,uC(n)); )
      if( twilightZoneFlow==2 )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))+=(*e)(c,I1e,I2e,I3e,fC(n),t)-2.*(*e)(c,I1e+is1,I2e+is2,I3e+is3,fC(n),t)
	    +(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,fC(n),t); )
      break;
    case 3:
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=3.*uA(I1e+  is1,I2e+  is2,I3e+  is3,uC(n))
	  -3.*uA(I1e+2*is1,I2e+2*is2,I3e+2*is3,uC(n))
	  +   uA(I1e+3*is1,I2e+3*is2,I3e+3*is3,uC(n)); )
	  
      if( twilightZoneFlow==2 )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))+=(*e)(c,I1e,I2e,I3e,fC(n),t)-3.*(*e)(c,I1e+is1,I2e+is2,I3e+is3,fC(n),t)
	    +3.*(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,fC(n),t)
	    -   (*e)(c,I1e+3*is1,I2e+3*is2,I3e+3*is3,fC(n),t); )
      break;
    case 4:
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=4.*uA(I1e+  is1,I2e+  is2,I3e+  is3,uC(n))
	  -6.*uA(I1e+2*is1,I2e+2*is2,I3e+2*is3,uC(n))
	  +4.*uA(I1e+3*is1,I2e+3*is2,I3e+3*is3,uC(n))
	  -   uA(I1e+4*is1,I2e+4*is2,I3e+4*is3,uC(n)); )
      if( twilightZoneFlow==2 )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))+=(*e)(c,I1e,I2e,I3e,fC(n),t)-4.*(*e)(c,I1e+is1,I2e+is2,I3e+is3,fC(n),t)
	    +6.*(*e)(c,I1e+2*is1,I2e+2*is2,I3e+2*is3,fC(n),t)
	    -4.*(*e)(c,I1e+3*is1,I2e+3*is2,I3e+3*is3,fC(n),t) 
	    +   (*e)(c,I1e+4*is1,I2e+4*is2,I3e+4*is3,fC(n),t); )
      break;
    default:  // general case:
      binomial= orderOfExtrap;
      if( twilightZoneFlow==2 )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=(*e)(c,I1e,I2e,I3e,uC(n),t); )
      else
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))=0.; )
      for( m=1; m<=orderOfExtrap; m++ )
      {
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1e,I2e,I3e,uC(n))+=binomial*uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,uC(n)); )
	if( twilightZoneFlow==2 )
	  for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	    WHERE_MASK( uA(I1e,I2e,I3e,uC(n))-=binomial*(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,fC(n),t); )
	binomial*=(m- orderOfExtrap)/real(m+1);
      }
	  
    }
    if( twilightZoneFlow==2 )
    { // already done this case
    }	    
    else if( bcOption==scalarForcing )
    {
      if( scalarData !=0. )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1,I2,I3,uC(n))+=scalarData; )
    }
    else if( bcOption==arrayForcing )
    {
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
        #ifdef USE_PPP
	  Overture::abort("MappedGridOperators::applyBCextrapolate:ERROR finish me Bill!");
        #else
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1,I2,I3,uC(n))+=arrayDataD(I1,I2,I3,fC(n)); );
        #endif
      }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1,I2,I3,uC(n))+=arrayData(fC(n),side,axis,grid); )
      else
	for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	  WHERE_MASK( uA(I1,I2,I3,uC(n))+=arrayData(fC(n)); )
    }
    else if( bcOption==gridFunctionForcing )
    {  
      for( n=uC.getBase(0); n<=uC.getBound(0); n++ )
	WHERE_MASK( uA(I1,I2,I3,uC(n))+=gfData(I1,I2,I3,fC(n)); )
    }
    else
    {
      cout << "applyBoundaryCondition: (extrapolate): ERROR: Invalid value for bcOption = " << bcOption << endl;      
      {throw "Invalid value for bcOption!";}
    }

    timeForExtrapolate+=getCPU()-time;
    break;

  case extrapolateNormalComponent:
  case extrapolateTangentialComponent0:
  case extrapolateTangentialComponent1:
  {
    // *************************************************
    // extrapolate the normal or tangential Component
    //       u <- u + (g-n.u) n
    //   g = extrapolated value
    // ************************************************

    #ifdef USE_PPP
      Overture::abort("ERROR: finish this Bill!");
    #endif

    RealDistributedArray & normal  = mappedGrid.vertexBoundaryNormal(side,axis);   // make centerBoundaryNormal ***
    RealDistributedArray & tangent = mappedGrid.centerBoundaryTangent(side,axis);

    RealDistributedArray vector;
    if( bcType==BCTypes::extrapolateNormalComponent )
    {
      v0=0; v1=1; v2=2;
      vector.reference(normal);
    }
    else
    {
      nt = bcType==BCTypes::extrapolateTangentialComponent0 ? 0 : 1;
      v0=nt*numberOfDimensions; v1=v0+1; v2=v1+1;
      if( numberOfDimensions<2+nt )
      {
	cout << "applyBoundaryCondition::ERROR: cannot apply extrapolateTangentialComponent" << nt 
	     << " BC in " << numberOfDimensions << "D\n";
	throw "error";
      }
      vector.reference(tangent(I1,I2,I3,Range(0,numberOfDimensions-1)+numberOfDimensions*nt));
    }
	  
    line = bcParameters.ghostLineToAssign;
    if( line > 6 || line < 0 )
    {
      printF("MGOP:applyBCextrapolate::ERROR? extrapolating ghost line %i.\n",line);
    }

    getGhostIndex( c.indexRange(),side,axis,I1e,I2e,I3e,line,bcParameters.extraInTangentialDirections); // line to extrapolate
    // *wdh* 091201: make sure that the Index's are not too large in the tangential directions
    if( bcParameters.extraInTangentialDirections>0 )
      for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	if( dir!=axis )
	  Iev[dir]=Range(max(c.dimension(0,dir),Iev[dir].getBase()),min(c.dimension(1,dir),Iev[dir].getBound()));

    is1 = (axis==axis1) ? 1-2*side : 0;   
    is2 = (axis==axis2) ? 1-2*side : 0;           
    is3 = (axis==axis3) ? 1-2*side : 0;           
    if( orderOfExtrap < 0 || orderOfExtrap>20 )
    {
      printF("MGOP:applyBCextrapolate::extrapolate[Normal/Tangential]Component:ERROR? orderOfExtrapolation = %i\n",
	     orderOfExtrap);
      OV_ABORT("ERROR");
    }

    getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"extrapolate[Normal/Tangential]Component",uC,fC  );

	// just do general case of any order of extrapolation (<20):
    if( numberOfDimensions==2 )
    {
      uDotN(I1,I2,I3)=-(uA(I1e,I2e,I3e,n1)*vector(I1,I2,I3,v0)
			+uA(I1e,I2e,I3e,n2)*vector(I1,I2,I3,v1));
    }
    else if( numberOfDimensions==3 )
    {
      uDotN(I1,I2,I3)=-(uA(I1e,I2e,I3e,n1)*vector(I1,I2,I3,v0)
			+uA(I1e,I2e,I3e,n2)*vector(I1,I2,I3,v1) 
			+uA(I1e,I2e,I3e,n3)*vector(I1,I2,I3,v2));
    }
    else
      uDotN(I1,I2,I3)=-uA(I1e,I2e,I3e,n1)*vector(I1,I2,I3,v0);

    if( twilightZoneFlow==2 )
    {
      if( numberOfDimensions==2 )
      {
	uDotN(I1,I2,I3)+=((*e)(c,I1e,I2e,I3e,m1,t)*vector(I1,I2,I3,v0)
			  +(*e)(c,I1e,I2e,I3e,m2,t)*vector(I1,I2,I3,v1));
      }
      else if( numberOfDimensions==3 )
      {
	uDotN(I1,I2,I3)+=((*e)(c,I1e,I2e,I3e,m1,t)*vector(I1,I2,I3,v0)
			  +(*e)(c,I1e,I2e,I3e,m2,t)*vector(I1,I2,I3,v1) 
			  +(*e)(c,I1e,I2e,I3e,m3,t)*vector(I1,I2,I3,v2));
      }
      else
	uDotN(I1,I2,I3)+=(*e)(c,I1e,I2e,I3e,m1,t)*vector(I1,I2,I3,v0);
    }

    binomial= orderOfExtrap;
    for( m=1; m<=orderOfExtrap; m++ )
    {
      if( numberOfDimensions==2 )
      {
	uDotN(I1,I2,I3)+=binomial*(uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n1)*vector(I1,I2,I3,v0)
				   +uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n2)*vector(I1,I2,I3,v1));
      }
      else if( numberOfDimensions==3 )
      {
	uDotN(I1,I2,I3)+=binomial*(uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n1)*vector(I1,I2,I3,v0)
				   +uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n2)*vector(I1,I2,I3,v1) 
				   +uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n3)*vector(I1,I2,I3,v2));
      }
      else
	uDotN(I1,I2,I3)+=binomial*uA(I1e+m*is1,I2e+m*is2,I3e+m*is3,n1)*vector(I1,I2,I3,v0); 

      if( twilightZoneFlow==2 )
      {
	if( numberOfDimensions==2 )
	  uDotN(I1,I2,I3)-=binomial*((*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m1,t)*vector(I1,I2,I3,v0)
				     +(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m2,t)*vector(I1,I2,I3,v1));
	else if( numberOfDimensions==3 ) 
	  uDotN(I1,I2,I3)-=binomial*((*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m1,t)*vector(I1,I2,I3,v0)
				     +(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m2,t)*vector(I1,I2,I3,v1) 
				     +(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m3,t)*vector(I1,I2,I3,v2));
	else
	  uDotN(I1,I2,I3)-=binomial*(*e)(c,I1e+m*is1,I2e+m*is2,I3e+m*is3,m1,t)*vector(I1,I2,I3,v0);
      }
      binomial*=(m- orderOfExtrap)/real(m+1);
    }
    // now "project" the ghost line value
    if( numberOfDimensions>1 )
    {
      WHERE_MASK( uA(I1e,I2e,I3e,n1)+=uDotN(I1,I2,I3)*vector(I1,I2,I3,v0); )
      WHERE_MASK( uA(I1e,I2e,I3e,n2)+=uDotN(I1,I2,I3)*vector(I1,I2,I3,v1); )
      if( numberOfDimensions==3 )
	WHERE_MASK( uA(I1e,I2e,I3e,n3)+=uDotN(I1,I2,I3)*vector(I1,I2,I3,v2); )
    }
    else
      WHERE_MASK( uA(I1e,I2e,I3e,n1)+=uDotN(I1,I2,I3)*vector(I1,I2,I3,v0); )


    timeForExtrapolateNormalComponent+=getCPU()-time;
    break;
  }
  }

}
