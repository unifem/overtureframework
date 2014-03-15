#include "MappedGridOperators.h"

#include "MappedGridOperatorsInclude.h"

// This next include file defines the prototype for assignBoundaryConditions
#include "assignBoundaryConditions.h"
#include "ParallelUtility.h"

void MappedGridOperators::
applyBCtangentialComponent(realMappedGridFunction & u, 
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
  // real time=getCPU();

  // Need to fix check.p for this:
  const bool useOpt= bcType==BCTypes::tangentialComponent
                     && !twilightZoneFlow ; // *** finish me *******
  if( useOpt )
  {
    // **********************
    //    use optimised BC 
    // **********************

    MappedGrid & c = mappedGrid;
  

    int n1,n2,n3,m1,m2,m3;
  
    Index I1,I2,I3;
    getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);



    //
    // tangentialComponent : set the tangential component to g:
    //       u <- (n.u) n + g
    //
    // tangentialComponent0, tangentialComponent1: set a tangential component
    //       u <- (n.u) n + g*tn
    // 
    if( bcType==BCTypes::tangentialComponent )
    {
      getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"tangentialComponent",uC,fC  );
    }
    else if( bcType==BCTypes::tangentialComponent0 ||
             bcType==BCTypes::tangentialComponent1 )
    {
      getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"tangentialComponent[0,1]",uC,fC  );
    }
    else
    {
      OV_ABORT("error");
    }
    
    OV_GET_SERIAL_ARRAY(real,u,uLocal);
    OV_GET_SERIAL_ARRAY_CONST(real,gfData,gfDataLocal);
    OV_GET_SERIAL_ARRAY_CONST(int,mask,amaskLocal);
    OV_GET_SERIAL_ARRAY_CONST(int,c.mask(),cmaskLocal);
    const realSerialArray & arrayDataDLocal =arrayDataD;
    

// #ifdef USE_PPP
//     realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
//     realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
//     intSerialArray amaskLocal; getLocalArrayWithGhostBoundaries(mask,amaskLocal);
//     intSerialArray cmaskLocal; getLocalArrayWithGhostBoundaries(c.mask(),cmaskLocal);

// #else
//     const realSerialArray & uLocal = u;
//     const realSerialArray & gfDataLocal = gfData;
//     const intSerialArray & amaskLocal = mask;
//     const intSerialArray & cmaskLocal = c.mask();

// #endif

    // realArray *gfDatap=(realArray*)(&gfData);

    real par[]={0.};  // not used
    int ipar[]={n1,n2,n3,m1,m2,m3};  
    real dr[3]={1.,1.,1.}; // not used
    int ca=0,cb=0;   // not used

    realSerialArray *gfDatap=(realSerialArray*)(&gfDataLocal);
    realSerialArray *rhsp=NULL;
    
    // option from : parameter( scalarForcing=0,gfForcing=1,arrayForcing=2, vectorForcing=3 )
    int option=-1;
    if( twilightZoneFlow )
    {
      // Fill in the rhs array with TZ forcing
      option=1;
      rhsp = new realSerialArray;
      realSerialArray & rhs = *rhsp;
      Range M(min(fC),max(fC));
      rhs.redim(I1,I2,I3,M);
      gfDatap=&rhs;
      
      c.update(MappedGrid::THEcenter);
      realArray & x= c.center();
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
      #else
        const realSerialArray & xLocal = x;
      #endif 

      const bool isRectangular=false;
      (*e).gd( rhs,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,M,t);
//       if( numberOfDimensions==2 )
//       {
// 	rhs(I1,I2,I3,m1)=u0(I1,I2,I3,m1);
// 	rhs(I1,I2,I3,m2)=u0(I1,I2,I3,m2);
//       }
//       else if( numberOfDimensions==3 ) 
//       {
// 	rhs(I1,I2,I3,m1)=(*e)(c,I1,I2,I3,m1,t);
// 	rhs(I1,I2,I3,m2)=(*e)(c,I1,I2,I3,m2,t);
// 	rhs(I1,I2,I3,m3)=(*e)(c,I1,I2,I3,m3,t);
//       }
//       else
//       {
// 	rhs(I1,I2,I3,m1)=(*e)(c,I1,I2,I3,m1,t);
//       }
    }
    else if( bcOption==scalarForcing )
    {
      option=0;
    }
    else if( bcOption==arrayForcing )
    {
      // **** 070423 -- finish me for parallel --
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
        option=1;
        gfDatap=(realSerialArray *)(&arrayDataDLocal);       }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
      {
        option=2; // use arrayData(fC(n),side,axis,grid)
      }
      else
        option=3;  // use arrayData(fC(n))
    }
    else if( bcOption==gridFunctionForcing )
    {
      option=1;
      gfDatap=(realSerialArray *)(&gfDataLocal);
    }
    else
    {
      throw "Invalid value for bcOption! (normalComponent)";
    }
    
    assert( option>=0 );
    assert( gfDatap!=NULL );
    const realSerialArray & gfd = *gfDatap;


    const int gridType = rectangular ? 0 : 1;

    const int useWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                              bcParameters.getUseMask() ;

    real *up = getDataPointer(uLocal);
    real *adp=getDataPointer(arrayData);
    const real *arrayDatap=adp!=NULL ? adp : up;
    

    const realSerialArray & nmCoeff = uLocal; // not used

    
    if( !rectangular )
      c.update(MappedGrid::THEinverseVertexDerivative );


    const realArray & rx = !rectangular ? c.inverseVertexDerivative() : u;
    #ifdef USE_PPP
      // const realSerialArray & rxLocal= rx.getLocalArrayWithGhostBoundaries();
      realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
    #else
      const realSerialArray & rxLocal= rx;
    #endif  

    const real *rxp = rxLocal.getDataPointer();

    int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 
    const int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b, includeGhost);

    if( ok ) // In parallel, the boundary may not be on this processor
    {
      const intSerialArray & maskLocal = useWhereMask ? amaskLocal : cmaskLocal;
      const int *maskp = maskLocal.getDataPointer();
      assert( maskp != NULL );

      // *wdh* 040930 -- check the mask bounds -- the mask values may be on the ghost line ---
      int ndm[2][3];  // base/bound for maskLocal
      for( int dir=0; dir<3; dir++ )
      {
        ndm[0][dir]=maskLocal.getBase(dir);
        ndm[1][dir]=maskLocal.getBound(dir);
        // The mask may come from the ghost -- shift the base/bound of the mask so we can index the mask in the
        // same way as the solution
        if( useWhereMask && axis==dir )
	{
	  if( ndm[0][dir]==ndm[1][dir] && nv[0][dir]==nv[1][dir] && ndm[0][dir]!=nv[0][dir] )
	  { // mask base/bound do not match boundary bounds, shift the mask bounds
	    ndm[0][dir]=nv[0][dir];
	    ndm[1][dir]=nv[1][dir];
	  }
	}
      }
      if( useWhereMask )
      {
        for( int dir=0; dir<3; dir++ )
	{
	  if( nv[0][dir]<ndm[0][dir] || nv[1][dir]>ndm[1][dir] )
	  {
	    printf("tangentialComponent:ERROR:mask bounds are not valid!\n"
                   " n1a,n1b,n2a,n2b,n3a,n3b = %i,%i,%i,%i,%i,%i \n"
                   " mask bounds = [%i,%i][%i,%i][%i,%i]\n",n1a,n1b,n2a,n2b,n3a,n3b,
                   ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2]);
            OV_ABORT("ERROR:tangentialComponent");
	  }
	}
      }
      
      assignBoundaryConditions( c.numberOfDimensions(), 
        n1a,n1b,n2a,n2b,n3a,n3b,
        uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
        uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
        uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
        uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
        nmCoeff.getBase(0),nmCoeff.getBound(0),nmCoeff.getBase(1),nmCoeff.getBound(1),
        nmCoeff.getBase(2),nmCoeff.getBound(2),nmCoeff.getBase(3),nmCoeff.getBound(3),
        gfd.getBase(0),gfd.getBound(0),gfd.getBase(1),gfd.getBound(1),
        gfd.getBase(2),gfd.getBound(2),gfd.getBase(3),gfd.getBound(3),
        arrayData.getBase(0),arrayData.getBound(0),arrayData.getBase(1),arrayData.getBound(1),
        arrayData.getBase(2),arrayData.getBound(2),arrayData.getBase(3),arrayData.getBound(3),
        arrayData.getBase(0),arrayData.getBound(0),
	ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2],  // dimensions for mask
        rxLocal.getBase(0),rxLocal.getBound(0),rxLocal.getBase(1),rxLocal.getBound(1),
        rxLocal.getBase(2),rxLocal.getBound(2),
        *rxp,
        *up,*up,*getDataPointer(nmCoeff), *maskp,
        scalarData,*getDataPointer(gfd),*arrayDatap,*arrayDatap,
        dx[0],dr[0],ipar[0], par[0], ca,cb,
        uC.getBase(0),uC(uC.getBase(0)), fC.getBase(0),fC(fC.getBase(0)),
        side,axis,grid, (int)bcType, option,gridType,orderOfAccuracy,useWhereMask,bcParameters.lineToAssign );
    }

    delete rhsp;


    // timeForTangetialComponent+=getCPU()-time;  // fix me 
    return;  // *********************
    
  }
  

  // **** old way ****



  if( !boundaryNormalsUsed && bcType==BCTypes::tangentialComponent )
  {
    boundaryNormalsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
  }
  if( !boundaryTangentsUsed && 
     ( bcType==BCTypes::tangentialComponent0 || bcType==BCTypes::tangentialComponent1) )
  {
    boundaryTangentsUsed=TRUE;
    mappedGrid.update(MappedGrid::THEcenterBoundaryTangent);
  }
  MappedGrid & c = mappedGrid;
  RealDistributedArray & uA = u;
  RealDistributedArray & normal  = mappedGrid.vertexBoundaryNormal(side,axis);   // make centerBoundaryNormal ***
  RealDistributedArray & tangent = mappedGrid.centerBoundaryTangent(side,axis);
  int n1,n2,n3,m1,m2,m3,nt,ndt;

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

  // The tangent and normal are indexed on the boundary:
  Index Ib1,Ib2,Ib3;
  getGhostIndex( c.indexRange(),side,axis,Ib1,Ib2,Ib3,0,bcParameters.extraInTangentialDirections); // boundary Index

  RealDistributedArray uDotN(I1,I2,I3);

  switch (bcType)
  {
  case tangentialComponent0:
  case tangentialComponent1:


    // tangent vectors are stored as : tangent(I1,I2,I3,0:nd-1,0:nd-2)  (last index = tangent 0 or 1)
    nt= bcType==BCTypes::tangentialComponent0 ? 0 : 1;
    ndt=nt*numberOfDimensions;  // for indexing into the tangent array (last two components are merged)
    if( numberOfDimensions==1 )
    {
      cout << "applyBoundaryCondition::ERROR: cannot apply tangentialComponent" << nt << " BC in 1D\n";
      {throw "error";}
    }
    //
    // to set the tangential component to g:
    //       u <- u + (g-(n.u)) n
    //
    getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"tangentialComponent[0,1]",uC,fC  );
	
    // NOTE: uDotN is really uDotTangent
    if( numberOfDimensions==2 )
    {
      uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*tangent(Ib1,Ib2,Ib3,0+ndt)+uA(I1,I2,I3,n2)*tangent(Ib1,Ib2,Ib3,1+ndt);
    }
    else
    {
      uDotN(I1,I2,I3)=  uA(I1,I2,I3,n1)*tangent(Ib1,Ib2,Ib3,0+ndt)
	+uA(I1,I2,I3,n2)*tangent(Ib1,Ib2,Ib3,1+ndt) 
	+uA(I1,I2,I3,n3)*tangent(Ib1,Ib2,Ib3,2+ndt); 
    }

    if( twilightZoneFlow ) 
    { // In this case we want to specify the value for n.u
      if( numberOfDimensions==2 )
	uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*tangent(Ib1,Ib2,Ib3,0+ndt)
	  +(*e)(c,I1,I2,I3,m2,t)*tangent(Ib1,Ib2,Ib3,1+ndt);
      else 
	uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*tangent(Ib1,Ib2,Ib3,0+ndt)
	  +(*e)(c,I1,I2,I3,m2,t)*tangent(Ib1,Ib2,Ib3,1+ndt) 
	  +(*e)(c,I1,I2,I3,m3,t)*tangent(Ib1,Ib2,Ib3,2+ndt);
    }
    else if( bcOption==scalarForcing )
    {
      if( scalarData != 0. )
	uDotN(I1,I2,I3)-=scalarData;   // use user supplied constant value
    }
    else if( bcOption==arrayForcing )
    {
      #ifdef USE_PPP
	Overture::abort("MappedGridOperators::applyBCtangential:ERROR finish me Bill!");
      #else 
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(m1,m2,m3) && arrayDataD.getBound(3)>=max(m1,m2,m3) )
      {
	if( numberOfDimensions==2 ) 
	  uDotN(I1,I2,I3)-=(arrayDataD(I1,I2,I3,m1)*tangent(Ib1,Ib2,Ib3,0+ndt)+
			    arrayDataD(I1,I2,I3,m2)*tangent(Ib1,Ib2,Ib3,1+ndt));
	else if( numberOfDimensions==3 )
	  uDotN(I1,I2,I3)-=(arrayDataD(I1,I2,I3,m1)*tangent(Ib1,Ib2,Ib3,0+ndt)+
			    arrayDataD(I1,I2,I3,m2)*tangent(Ib1,Ib2,Ib3,1+ndt)+
			    arrayDataD(I1,I2,I3,m3)*tangent(Ib1,Ib2,Ib3,2+ndt));
      }
      else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
      {
	if( numberOfDimensions==2 ) 
	  uDotN(I1,I2,I3)-=arrayData(m1,side,axis,grid)*tangent(Ib1,Ib2,Ib3,0+ndt)
	    +arrayData(m2,side,axis,grid)*tangent(Ib1,Ib2,Ib3,1+ndt);
	else 
	  uDotN(I1,I2,I3)-=arrayData(m1,side,axis,grid)*tangent(Ib1,Ib2,Ib3,0+ndt)
	    +arrayData(m2,side,axis,grid)*tangent(Ib1,Ib2,Ib3,1+ndt) 
	    +arrayData(m3,side,axis,grid)*tangent(Ib1,Ib2,Ib3,2+ndt);
      }
      else
      {
	if( numberOfDimensions==2 ) 
	  uDotN(I1,I2,I3)-=arrayData(m1)*tangent(Ib1,Ib2,Ib3,0+ndt)
	    +arrayData(m2)*tangent(Ib1,Ib2,Ib3,1+ndt);
	else 
	  uDotN(I1,I2,I3)-=arrayData(m1)*tangent(Ib1,Ib2,Ib3,0+ndt)
	    +arrayData(m2)*tangent(Ib1,Ib2,Ib3,1+ndt) 
	    +arrayData(m3)*tangent(Ib1,Ib2,Ib3,2+ndt);
      }
      #endif
    }
    else if( bcOption==gridFunctionForcing )
    {  // use user supplied variable values
      if( gfData.getComponentDimension(0) < numberOfDimensions )   
	uDotN(I1,I2,I3)-=gfData(I1,I2,I3,m1);
      else if( numberOfDimensions==2 ) 
	uDotN(I1,I2,I3)-=gfData(I1,I2,I3,m1)*tangent(Ib1,Ib2,Ib3,0+ndt)
	  +gfData(I1,I2,I3,m2)*tangent(Ib1,Ib2,Ib3,1+ndt);
      else 
	uDotN(I1,I2,I3)-=gfData(I1,I2,I3,m1)*tangent(Ib1,Ib2,Ib3,0+ndt)
	  +gfData(I1,I2,I3,m2)*tangent(Ib1,Ib2,Ib3,1+ndt) 
	  +gfData(I1,I2,I3,m3)*tangent(Ib1,Ib2,Ib3,2+ndt);
    }
    else
      {throw "Invalid value for bcOption! (tangentialComponent)";}

    WHERE_MASK( uA(I1,I2,I3,n1)-=uDotN(I1,I2,I3)*tangent(Ib1,Ib2,Ib3,0+ndt); )
    WHERE_MASK( uA(I1,I2,I3,n2)-=uDotN(I1,I2,I3)*tangent(Ib1,Ib2,Ib3,1+ndt); )
    if( numberOfDimensions==3 )
      WHERE_MASK( uA(I1,I2,I3,n3)-=uDotN(I1,I2,I3)*tangent(Ib1,Ib2,Ib3,2+ndt); )

    break;
	
  case tangentialComponent:
    //
    // to set the tangential component to g:
    //       u <- (n.u)n + g
    //
    // for TZ flow g = ue - (n.ue)n
    getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"tangentialComponent",uC,fC  );

    if( numberOfDimensions==2 )
    {
      uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*normal(Ib1,Ib2,Ib3,0)+uA(I1,I2,I3,n2)*normal(Ib1,Ib2,Ib3,1);
    }
    else if( numberOfDimensions==3 )
    {
      uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*normal(Ib1,Ib2,Ib3,0)
	+uA(I1,I2,I3,n2)*normal(Ib1,Ib2,Ib3,1) 
	+uA(I1,I2,I3,n3)*normal(Ib1,Ib2,Ib3,2);
    }
    else
      uDotN(I1,I2,I3)=uA(I1,I2,I3,n1)*(2*side-1);  // outward normal in 1D
    if( twilightZoneFlow ) 
    { 
      if( numberOfDimensions==2 )
	uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*normal(Ib1,Ib2,Ib3,0)
	  +(*e)(c,I1,I2,I3,m2,t)*normal(Ib1,Ib2,Ib3,1);
      else if( numberOfDimensions==3 ) 
	uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*normal(Ib1,Ib2,Ib3,0)
	  +(*e)(c,I1,I2,I3,m2,t)*normal(Ib1,Ib2,Ib3,1) 
	  +(*e)(c,I1,I2,I3,m3,t)*normal(Ib1,Ib2,Ib3,2);
      else
	uDotN(I1,I2,I3)-=(*e)(c,I1,I2,I3,m1,t)*(2*side-1);  // outward normal in 1D

      if( numberOfDimensions>1 )
      {
	WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0)+(*e)(c,I1,I2,I3,m1,t); )
	WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1)+(*e)(c,I1,I2,I3,m2,t); )
	if( numberOfDimensions==3 )
	  WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2)+(*e)(c,I1,I2,I3,m3,t); )
      }
      else
	WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1)+(*e)(c,I1,I2,I3,m1,t); )
    }
    else if( bcOption==scalarForcing )
    {
      if( scalarData == 0. )
      {
	if( numberOfDimensions>1 )
	{
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0); )
	  WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1); )
	  if( numberOfDimensions==3 )
	    WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2); )
	}
	else
	  uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1);
      }
      else
      {
	if( numberOfDimensions>1 )
	{
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0)+scalarData; )
	  WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1)+scalarData; )
	  if( numberOfDimensions==3 )
	    WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2)+scalarData; )
	}
	else
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1)+scalarData; )
      }
    }
    else if( bcOption==arrayForcing )
    {
      if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
      {
	if( numberOfDimensions>1 )
	{
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0)+arrayData(0,side,axis,grid); )
	  WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1)+arrayData(1,side,axis,grid); )
	  if( numberOfDimensions==3 )
	    WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2)+arrayData(2,side,axis,grid); )
	}
	else
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1)+arrayData(0,side,axis,grid); )
      }
      else
      {
	if( numberOfDimensions>1 )
	{
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0)+arrayData(0); )
	  WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1)+arrayData(1); )
	  if( numberOfDimensions==3 )
	    WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2)+arrayData(2); )
	}
	else
	  WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1)+arrayData(0); )
      }
    }
    else if( bcOption==gridFunctionForcing )
    {  // use user supplied variable values
      if( numberOfDimensions>1 )
      {
	WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,0)+gfData(I1,I2,I3,m1); )
	WHERE_MASK( uA(I1,I2,I3,n2)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,1)+gfData(I1,I2,I3,m2); )
	if( numberOfDimensions==3 )
	  WHERE_MASK( uA(I1,I2,I3,n3)=uDotN(I1,I2,I3)*normal(Ib1,Ib2,Ib3,2)+gfData(I1,I2,I3,m3); )
      }
      else
	WHERE_MASK( uA(I1,I2,I3,n1)=uDotN(I1,I2,I3)*(2*side-1)+gfData(I1,I2,I3,m1); )

    }
    else
      {throw "Invalid value for bcOption! (tangentialComponent)";}

    break;
    
  }
}
