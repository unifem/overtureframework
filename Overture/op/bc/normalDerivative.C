#include "MappedGridOperators.h"
#include "SparseRep.h"

#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"
#include "display.h"

// This next include file defines the prototype for assignBoundaryConditions
#include "assignBoundaryConditions.h"

// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))


void MappedGridOperators::
applyBCnormalDerivative(realMappedGridFunction & u, 
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
  // const bool rectangularSave=rectangular;
  
  // *** rectangular=false;  // ***************** for testing 050414


  if( orderOfAccuracy!=2 )
  {
    printf("MappedGridOperators:: Sorry, the normal derivative boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }
  bool normalsAreNeeded = !rectangular  && 
            ( bcType==BCTypes::normalDerivativeOfNormalComponent ||
              bcType==BCTypes::normalDerivativeOfTangentialComponent0 || 
              bcType==BCTypes::normalDerivativeOfTangentialComponent1 );


  MappedGrid & c = mappedGrid;
  if( !boundaryNormalsUsed && normalsAreNeeded )
  {
    boundaryNormalsUsed=TRUE;
//    #ifndef USE_PPP  // *wdh* 061010
    mappedGrid.update(MappedGrid::THEvertexBoundaryNormal);
//    #endif
  }
  
  bool tangentsAreNeeded = !rectangular  && 
    ( bcType==BCTypes::normalDerivativeOfTangentialComponent0 || 
      bcType==BCTypes::normalDerivativeOfTangentialComponent1);

  if( !boundaryTangentsUsed && tangentsAreNeeded )
  {
    if( numberOfDimensions<2 || (numberOfDimensions<3 && bcType==BCTypes::normalDerivativeOfTangentialComponent1) )
      return;
    
    boundaryTangentsUsed=TRUE;
//    #ifndef USE_PPP // *wdh* 061010
    mappedGrid.update(MappedGrid::THEcenterBoundaryTangent);
//    #endif
  }
  RealDistributedArray & uA = u;
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  #else
    const realSerialArray & uLocal = u;
  #endif

  #ifdef USE_PPP
//     realSerialArray *pNormal = mappedGrid.rcData->pVertexBoundaryNormal[axis][side];
//     assert( !normalsAreNeeded || pNormal!=NULL );
//     realSerialArray & normal  = normalsAreNeeded ? *pNormal : uLocal; 

//     realSerialArray *pTangent = mappedGrid.rcData->pCenterBoundaryTangent[axis][side];
//     assert( !tangentsAreNeeded || pTangent!=NULL );
//     realSerialArray & tangent = tangentsAreNeeded ? *pTangent : uLocal; 

    realSerialArray & normal  = normalsAreNeeded  ? mappedGrid.vertexBoundaryNormalArray(side,axis) : uLocal;  
    realSerialArray & tangent = tangentsAreNeeded ? mappedGrid.centerBoundaryTangentArray(side,axis) : uLocal;

  #else
    realSerialArray & normal  = normalsAreNeeded  ? mappedGrid.vertexBoundaryNormal(side,axis) : uA;  
    realSerialArray & tangent = tangentsAreNeeded ? mappedGrid.centerBoundaryTangent(side,axis) : uA;
  #endif

  int n1,n2,n3,m1,m2,m3,nt;
  // int is1,is2,is3;
  int v0,v1,v2;

  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

  getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"normalDerivativeOf[Normal/Tangential]Component",uC,fC  );

  if( bcType==BCTypes::normalDerivativeOfNormalComponent )
  {
    v0=0; v1=1; v2=2;
  }
  else
  {
    nt= bcType==BCTypes::normalDerivativeOfTangentialComponent0 ? 0 : 1;
    v0=nt*numberOfDimensions; v1=v0+1; v2=v1+1;
    if( numberOfDimensions<2+nt )
    {
      cout << "applyBoundaryCondition::ERROR: cannot apply extrapolateTangentialComponent" << nt 
	   << " BC in " << numberOfDimensions << "D\n";
      throw "error";
    }
  }

  if( !( rectangular || numberOfDimensions==1 ) && !nCoeffIsSet[axis][side] )
  {
    // *** note we use the same coeff matrix as for the neumann BC

    if( !nCoeffIsSet[axis][side] )
      createBoundaryMatrix(side,axis,BCTypes::neumann);

    realSerialArray & nmCoeff = neumannCoeff[axis][side];

    if( !nCoeffIsSet[axis][side] )
    { // generate coefficients if they have not already been set
      Index M(0,int(pow(3,numberOfDimensions)+.5));
      nCoeffIsSet[axis][side]=TRUE; 
      //  buildNormalDerivativeCoefficientMatrix(*this,nmCoeff,M,I1,I2,I3,normal,numberOfDimensions);
      const real b0=0., b1=1.;
      buildNeumannCoefficientMatrix(*this,nmCoeff,M,I1,I2,I3,normal,b0,b1,numberOfDimensions,side,axis,
                                    bcParameters);
    }
  }


  int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 
  const int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b, includeGhost);
  if( ok ) // In parallel, the boundary may not be on this processor
  {

    real twoDeltaX = 2.*dx[axis]; // 1./h21(axis);
    if( !rectangular )
      twoDeltaX=1.;
    
    real dr[3];

    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
      const realSerialArray & arrayDataDLocal=arrayDataD;
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      // const intSerialArray & cmaskLocal = c.mask().getLocalArrayWithGhostBoundaries();
    #else
      const realSerialArray & uLocal = u;
      const realSerialArray & gfDataLocal = gfData;
      const realSerialArray & arrayDataDLocal = arrayDataD;

      const intSerialArray & maskLocal = mask;
      // const intSerialArray & maskLocal = c.mask();
    #endif
    
    realSerialArray & vect = bcType==BCTypes::normalDerivativeOfNormalComponent ? normal : tangent;

    //  ::display(vect," normalDerivative: vect");
    
    realSerialArray *gfDatap=(realSerialArray*)(&gfDataLocal);
    realSerialArray *rhsp=NULL;

    // option from : parameter( scalarForcing=0,gfForcing=1,arrayForcing=2, vectorForcing=3 )
    int option=-1;
    if( twilightZoneFlow )
    {
      // Fill in the rhs array with TZ forcing
      option=1;
      
      Range M(m1,m3); // m3==m2 in 2d
      rhsp = new realSerialArray(I1,I2,I3,M);
      realSerialArray & rhs = *rhsp;
      
      gfDatap=&rhs;
      
      realSerialArray ux(I1,I2,I3,M),uy,uz;
      if( numberOfDimensions>1 )
        uy.redim(I1,I2,I3,M);
      if( numberOfDimensions>2 )
	uz.redim(I1,I2,I3,M);


      c.update(MappedGrid::THEcenter);
      realArray & x= c.center();
      #ifdef USE_PPP
        realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
      #else
        const realSerialArray & xLocal = x;
      #endif  
      
      bool isRectangular=false; // do this for now
      if( normalsAreNeeded )
      {
	if( numberOfDimensions==1 )
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,m1,t);
	  rhs(I1,I2,I3,m1)=normal(I1,I2,I3,0)*ux(I1,I2,I3,m1);
	}
	else if( numberOfDimensions==2 )
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,M,t);
	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,M,t);
	  rhs(I1,I2,I3,m1)=(normal(I1,I2,I3,0)*ux(I1,I2,I3,m1)+
			    normal(I1,I2,I3,1)*uy(I1,I2,I3,m1));
	  rhs(I1,I2,I3,m2)=(normal(I1,I2,I3,0)*ux(I1,I2,I3,m2)+
			    normal(I1,I2,I3,1)*uy(I1,I2,I3,m2));
	}
	else 
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,M,t);
	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,M,t);
	  (*e).gd( uz,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,1,I1,I2,I3,M,t);
	  rhs(I1,I2,I3,m1)=(normal(I1,I2,I3,0)*ux(I1,I2,I3,m1)+
			    normal(I1,I2,I3,1)*uy(I1,I2,I3,m1)+
			    normal(I1,I2,I3,2)*uz(I1,I2,I3,m1));
	  rhs(I1,I2,I3,m2)=(normal(I1,I2,I3,0)*ux(I1,I2,I3,m2)+
			    normal(I1,I2,I3,1)*uy(I1,I2,I3,m2)+
			    normal(I1,I2,I3,2)*uz(I1,I2,I3,m2));
	  rhs(I1,I2,I3,m3)=(normal(I1,I2,I3,0)*ux(I1,I2,I3,m3)+
			    normal(I1,I2,I3,1)*uy(I1,I2,I3,m3)+
			    normal(I1,I2,I3,2)*uz(I1,I2,I3,m3));
	}
      }
      else
      {
	real an[3]={0.,0.,0.};  //
	an[axis]=2*side-1;   // set the outward normal
	 
	if( numberOfDimensions==1 )
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,m1,t);
	  rhs(I1,I2,I3,m1)=an[0]*ux(I1,I2,I3,m1);
	}
	else if( numberOfDimensions==2 )
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,M,t);
	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,M,t);
	  rhs(I1,I2,I3,m1)=(an[0]*ux(I1,I2,I3,m1)+
			    an[1]*uy(I1,I2,I3,m1));
	  rhs(I1,I2,I3,m2)=(an[0]*ux(I1,I2,I3,m2)+
			    an[1]*uy(I1,I2,I3,m2));
	}
	else 
	{
	  (*e).gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,M,t);
	  (*e).gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,M,t);
	  (*e).gd( uz,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,1,I1,I2,I3,M,t);
	  rhs(I1,I2,I3,m1)=(an[0]*ux(I1,I2,I3,m1)+
			    an[1]*uy(I1,I2,I3,m1)+
			    an[2]*uz(I1,I2,I3,m1));
	  rhs(I1,I2,I3,m2)=(an[0]*ux(I1,I2,I3,m2)+
			    an[1]*uy(I1,I2,I3,m2)+
			    an[2]*uz(I1,I2,I3,m2));
	  rhs(I1,I2,I3,m3)=(an[0]*ux(I1,I2,I3,m3)+
			    an[1]*uy(I1,I2,I3,m3)+
			    an[2]*uz(I1,I2,I3,m3));
	}
      }
    }
    else if( bcOption==scalarForcing )
    {
      option=0;
    }
    else if( bcOption==arrayForcing )
    {
      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
	option=1;
	gfDatap=(realSerialArray *)(&arrayDataD); // use arrayDataD(I1,I2,I3,fC(n))
      }
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
    {throw "Invalid value for bcOption! (neumann)";}
    
    assert( option>=0 );
    
    real par[]={twoDeltaX};
    int ipar[]={n1,n2,n3,m1,m2,m3,v0,v1,v2};
    

    const int gridType = rectangular ? 0 : 1;
    const int ca = uC.getBase(0);  // not used
    const int cb = uC.getBound(0); // not used

    const int useWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
      bcParameters.getUseMask() ;

    assert( gfDatap!=NULL );
    realSerialArray & gfd=*gfDatap;
    
    real *up = getDataPointer(uLocal);
    real *adp=getDataPointer(arrayData);
    const real *arrayDatap=adp!=NULL ? adp : up;
    
    int *mp=getDataPointer(maskLocal);
    const int *maskp = mp!=NULL ? mp : &uC(uC.getBase(0));

    realSerialArray *nmCoeffp;
    if( nCoeffIsSet[axis][side] )
      nmCoeffp= &neumannCoeff[axis][side];
    else
      nmCoeffp=&gfd;  // not used in this case

    realSerialArray & nmCoeff = *nmCoeffp;
    const real *rxp = up;  // Jacobian not needed here

    //  ::display(nmCoeff,"normalDerivative: nmCoeff");
    

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
	  printf("normalDerivative:ERROR:mask bounds are not valid!\n"
		 " n1a,n1b,n2a,n2b,n3a,n3b = %i,%i,%i,%i,%i,%i \n"
		 " mask bounds = [%i,%i][%i,%i][%i,%i]\n",n1a,n1b,n2a,n2b,n3a,n3b,
		 ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2]);
	  Overture::abort("ERROR:normalDerivative");
	}
      }
    }

    assignBoundaryConditions( c.numberOfDimensions(), 
			      n1a,n1b,n2a,n2b,n3a,n3b,
			      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			      uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			      vect.getBase(0),vect.getBound(0),vect.getBase(1),vect.getBound(1),
			      vect.getBase(2),vect.getBound(2),vect.getBase(3),vect.getBound(3),
			      nmCoeff.getBase(0),nmCoeff.getBound(0),nmCoeff.getBase(1),nmCoeff.getBound(1),
			      nmCoeff.getBase(2),nmCoeff.getBound(2),nmCoeff.getBase(3),nmCoeff.getBound(3),
			      gfd.getBase(0),gfd.getBound(0),gfd.getBase(1),gfd.getBound(1),
			      gfd.getBase(2),gfd.getBound(2),gfd.getBase(3),gfd.getBound(3),
			      arrayData.getBase(0),arrayData.getBound(0),arrayData.getBase(1),arrayData.getBound(1),
			      arrayData.getBase(2),arrayData.getBound(2),arrayData.getBase(3),arrayData.getBound(3),
			      arrayData.getBase(0),arrayData.getBound(0),
			      ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2],  // dimensions for mask
			      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                              uLocal.getBase(2),uLocal.getBound(2),
			      *rxp,
			      *up,*getDataPointer(vect),*getDataPointer(nmCoeff), *maskp,
			      scalarData,*getDataPointer(gfd),*arrayDatap,*arrayDatap,
			      dx[0],dr[0],ipar[0], par[0], ca,cb,
                              uC.getBase(0),uC(uC.getBase(0)), fC.getBase(0),fC(fC.getBase(0)),
			      side,axis,grid, (int)bcType, option,gridType,orderOfAccuracy,useWhereMask,
                              bcParameters.lineToAssign );


    delete rhsp;
  }
    
  // rectangular=rectangularSave;  // restore. 

  timeForNormalDerivative+=getCPU()-time;
  return;



//   Index I1p,I2p,I3p;
//   getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); // first line in
//   Index I1m,I2m,I3m;
//   getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line

//   RealDistributedArray uDotN(I1,I2,I3);
//   switch (bcType)
//   {
    
//   case normalDerivativeOfNormalComponent:
//   case normalDerivativeOfTangentialComponent0:
//   case normalDerivativeOfTangentialComponent1:
//     // *********************************************************
//     // give normal derivative of the normal or tangential component
//     // or actually : 
//     //        tv\cdot (n.grad \uv ) 
//     // *********************************************************
//     getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"normalDerivativeOf[Normal/Tangential]Component",uC,fC  );

//     if( bcType==BCTypes::normalDerivativeOfNormalComponent )
//     {
//       v0=0; v1=1; v2=2;
//     }
//     else
//     {
//       nt= bcType==BCTypes::normalDerivativeOfTangentialComponent0 ? 0 : 1;
//       v0=nt*numberOfDimensions; v1=v0+1; v2=v1+1;
//       if( numberOfDimensions<2+nt )
//       {
// 	cout << "applyBoundaryCondition::ERROR: cannot apply extrapolateTangentialComponent" << nt 
// 	     << " BC in " << numberOfDimensions << "D\n";
// 	throw "error";
//       }
//     }
//     realArray & vect = bcType==BCTypes::normalDerivativeOfNormalComponent ? normal : tangent;

//     if( rectangular )
//     {
//       // rectangular grid : NOTE: we do not have to worry about +/- twoDeltaX*g  because
//       // the normal changes direction on either end introducing another +/-
//       real twoDeltaX = 2.*dx[axis]; // *wdh* 020713 2.*dx[0] // 1./h21(axis);
//       if( numberOfDimensions==1 )
//       {
// 	uDotN(I1,I2,I3)=(uA(I1p,I2p,I3p,n1)-uA(I1m,I2m,I3m,n1))*vect(I1,I2,I3,v0);
//       }
//       else if( numberOfDimensions==2 )
//       {
// 	uDotN(I1,I2,I3)=(uA(I1p,I2p,I3p,n1)-uA(I1m,I2m,I3m,n1))*vect(I1,I2,I3,v0)
// 	  +(uA(I1p,I2p,I3p,n2)-uA(I1m,I2m,I3m,n2))*vect(I1,I2,I3,v1);
//       }
//       else
//       {
// 	uDotN(I1,I2,I3)=(uA(I1p,I2p,I3p,n1)-uA(I1m,I2m,I3m,n1))*vect(I1,I2,I3,v0)
// 	  +(uA(I1p,I2p,I3p,n2)-uA(I1m,I2m,I3m,n2))*vect(I1,I2,I3,v1) 
// 	  +(uA(I1p,I2p,I3p,n3)-uA(I1m,I2m,I3m,n3))*vect(I1,I2,I3,v2); 
//       }

//       if( twilightZoneFlow )
//       {
// 	if( numberOfDimensions==1 )
// 	  uDotN(I1,I2,I3)+=(
// 	    (normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m1,t))*vect(I1,I2,I3,v0) 
// 	    )*twoDeltaX;  
// 	else if( numberOfDimensions==2 )
// 	  uDotN(I1,I2,I3)+=(
// 	    (normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,1)*e->y(c,I1,I2,I3,m1,t))*vect(I1,I2,I3,v0)
// 	    +(normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m2,t)
// 	      +normal(I1,I2,I3,1)*e->y(c,I1,I2,I3,m2,t))*vect(I1,I2,I3,v1)
// 	    )*twoDeltaX;  
// 	else 
// 	  uDotN(I1,I2,I3)+=(
// 	    (normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,1)*e->y(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,2)*e->z(c,I1,I2,I3,m1,t))*vect(I1,I2,I3,v0)
// 	    +(normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m2,t)
// 	      +normal(I1,I2,I3,1)*e->y(c,I1,I2,I3,m2,t)
// 	      +normal(I1,I2,I3,2)*e->z(c,I1,I2,I3,m2,t))*vect(I1,I2,I3,v1)
// 	    +(normal(I1,I2,I3,0)*e->x(c,I1,I2,I3,m3,t)
// 	      +normal(I1,I2,I3,1)*e->y(c,I1,I2,I3,m3,t)
// 	      +normal(I1,I2,I3,2)*e->z(c,I1,I2,I3,m3,t))*vect(I1,I2,I3,v2)
// 	    )*twoDeltaX;  
//       }
//       else if( bcOption==scalarForcing )
//       {
// 	if( scalarData != 0. )
// 	  uDotN(I1,I2,I3)+=scalarData*twoDeltaX;   // use user supplied constant value
//       }
//       else if( bcOption==arrayForcing )
//       {
// 	if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
// 	    arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
// 	    arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
// 	    arrayDataD.getBase(3)<=min(m1,m2,m3) && arrayDataD.getBound(3)>=max(m1,m2,m3) )
// 	{
// 	  if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)+=(arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0)+
// 			      arrayDataD(I1,I2,I3,m2)*vect(I1,I2,I3,v1))*twoDeltaX;
// 	  else if( numberOfDimensions==3 )
// 	    uDotN(I1,I2,I3)+=(arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0)+
// 			      arrayDataD(I1,I2,I3,m2)*vect(I1,I2,I3,v1)+
// 			      arrayDataD(I1,I2,I3,m3)*vect(I1,I2,I3,v2))*twoDeltaX;
// 	  else
// 	    uDotN(I1,I2,I3)+=arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0)*twoDeltaX;
// 	}
// 	else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
// 	{
// 	  if( numberOfDimensions==1 ) 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0))*twoDeltaX;
// 	  else if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0)
// 			      +arrayData(m2,side,axis,grid)*vect(I1,I2,I3,v1))*twoDeltaX;
// 	  else 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0)
// 			      +arrayData(m2,side,axis,grid)*vect(I1,I2,I3,v1) 
// 			      +arrayData(m3,side,axis,grid)*vect(I1,I2,I3,v2))*twoDeltaX;
// 	}
// 	else
// 	{
// 	  if( numberOfDimensions==1 ) 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1)*vect(I1,I2,I3,v0))*twoDeltaX;
// 	  else if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1)*vect(I1,I2,I3,v0)
// 			      +arrayData(m2)*vect(I1,I2,I3,v1))*twoDeltaX;
// 	  else 
// 	    uDotN(I1,I2,I3)+=(arrayData(m1)*vect(I1,I2,I3,v0)
// 			      +arrayData(m2)*vect(I1,I2,I3,v1) 
// 			      +arrayData(m3)*vect(I1,I2,I3,v2))*twoDeltaX;
// 	}
//       }
//       else if( bcOption==gridFunctionForcing )
//       {  // use user supplied variable values
// 	if( gfData.getComponentDimension(0) < numberOfDimensions )   
// 	  uDotN(I1,I2,I3)+=gfData(I1,I2,I3,m1);
// 	else if( numberOfDimensions==1 ) 
// 	  uDotN(I1,I2,I3)+=(gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0))*twoDeltaX;
// 	else if( numberOfDimensions==2 ) 
// 	  uDotN(I1,I2,I3)+=(gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0)
// 			    +gfData(I1,I2,I3,m2)*vect(I1,I2,I3,v1))*twoDeltaX;
// 	else 
// 	  uDotN(I1,I2,I3)+=(gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0)
// 			    +gfData(I1,I2,I3,m2)*vect(I1,I2,I3,v1) 
// 			    +gfData(I1,I2,I3,m3)*vect(I1,I2,I3,v2))*twoDeltaX;
//       }
//       else
//       {
// 	throw "Invalid value for bcOption! (normalDerivativeOfTangentialComponent)";
//       }
      
//       WHERE_MASK( uA(I1m,I2m,I3m,n1)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v0); )
//       if( numberOfDimensions > 1 ) 
//         WHERE_MASK( uA(I1m,I2m,I3m,n2)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v1); )
//       if( numberOfDimensions > 2 )
// 	WHERE_MASK( uA(I1m,I2m,I3m,n3)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v2); )

//     }
//     else
//     {

//       // cout << "Boundary conditions: apply real normalDerivativeOfTangentialComponent BC\n";
//       // generate coeff's for n.grad
//       // Solve for the ghost point from: (n.grad)u=
//       Index M(0,int(pow(3,numberOfDimensions)+.5));
          
//       is1 = (axis==axis1) ? 1-2*side : 0;   
//       is2 = (axis==axis2) ? 1-2*side : 0;           
//       is3 = (axis==axis3) ? 1-2*side : 0;           

//       int mGhost = numberOfDimensions==2 ? M2(-is1,-is2) : M3(-is1,-is2,-is3);    // coefficient index for ghost value

//       // we can use the same matrix as used by the neumann BC
//       if( !nCoeffIsSet[axis][side] )
// 	createBoundaryMatrix(side,axis,BCTypes::neumann);

//       #ifdef USE_PPP
//         RealDistributedArray & nmCoeff = Overture::nullRealDistributedArray(); 
// 	Overture::abort("ERROR: fix this Bill!");
//       #else
//         RealDistributedArray & nmCoeff = neumannCoeff[axis][side];
//       #endif

//       if( !nCoeffIsSet[axis][side] )
//       { // generate coefficients if they have not already been set
// 	nCoeffIsSet[axis][side]=TRUE; 
// 	buildNormalDerivativeCoefficientMatrix(*this,nmCoeff,M,I1,I2,I3,normal,numberOfDimensions);

//       }

//       // first zero out the tangential component on the ghost line (so we can use uA in the 
//       //  long expressions below)
//       if( numberOfDimensions==1 )
//         uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*uA(I1m,I2m,I3m,n1);   // compute the current vectorial component
//       else if( numberOfDimensions==2 )
//         uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*uA(I1m,I2m,I3m,n1)   
// 	  +vect(I1,I2,I3,v1)*uA(I1m,I2m,I3m,n2);
//       else if( numberOfDimensions==3 )
// 	uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*uA(I1m,I2m,I3m,n1)   
// 	  +vect(I1,I2,I3,v1)*uA(I1m,I2m,I3m,n2)
//           +vect(I1,I2,I3,v2)*uA(I1m,I2m,I3m,n3);

//       WHERE_MASK( uA(I1m,I2m,I3m,n1)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v0); ) // now zero it out
//       if( numberOfDimensions > 1 )
//         WHERE_MASK( uA(I1m,I2m,I3m,n2)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v1); )
//       if( numberOfDimensions > 2 )
// 	WHERE_MASK( uA(I1m,I2m,I3m,n3)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v2); )

//       if( twilightZoneFlow )
//       { 
// 	if( numberOfDimensions==1 )
// 	{
// 	  uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m1,t));
// 	}
// 	else if( numberOfDimensions==2 )
// 	{
// 	  uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,axis2)*e->y(c,I1,I2,I3,m1,t))
// 	    +vect(I1,I2,I3,v1)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m2,t)
// 	     +normal(I1,I2,I3,axis2)*e->y(c,I1,I2,I3,m2,t));
// 	}
// 	else
// 	{
// 	  uDotN(I1,I2,I3)=vect(I1,I2,I3,v0)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,axis2)*e->y(c,I1,I2,I3,m1,t)
// 	     +normal(I1,I2,I3,axis3)*e->z(c,I1,I2,I3,m1,t))
// 	    +vect(I1,I2,I3,v1)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m2,t)
// 	     +normal(I1,I2,I3,axis2)*e->y(c,I1,I2,I3,m2,t)
// 	     +normal(I1,I2,I3,axis3)*e->z(c,I1,I2,I3,m2,t));
// 	  +vect(I1,I2,I3,v2)*
// 	    (normal(I1,I2,I3,axis1)*e->x(c,I1,I2,I3,m3,t)
// 	     +normal(I1,I2,I3,axis2)*e->y(c,I1,I2,I3,m3,t)
// 	     +normal(I1,I2,I3,axis3)*e->z(c,I1,I2,I3,m3,t));
// 	}
//       }
//       else if( bcOption==scalarForcing )
// 	uDotN(I1,I2,I3)=scalarData;
//       else if( bcOption==arrayForcing )
//       {
// 	if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
// 	    arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
// 	    arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
// 	    arrayDataD.getBase(3)<=min(m1,m2,m3) && arrayDataD.getBound(3)>=max(m1,m2,m3) )
// 	{
// 	  if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)=(arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0)+
// 			     arrayDataD(I1,I2,I3,m2)*vect(I1,I2,I3,v1));
// 	  else if( numberOfDimensions==3 )
// 	    uDotN(I1,I2,I3)=(arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0)+
// 			     arrayDataD(I1,I2,I3,m2)*vect(I1,I2,I3,v1)+
// 			     arrayDataD(I1,I2,I3,m3)*vect(I1,I2,I3,v2));
// 	  else
// 	    uDotN(I1,I2,I3)=arrayDataD(I1,I2,I3,m1)*vect(I1,I2,I3,v0);
// 	}
// 	else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
// 	{
// 	  if( numberOfDimensions==1 ) 
// 	    uDotN(I1,I2,I3)=arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0);
// 	  else if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)=(arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0)+
// 			     arrayData(m2,side,axis,grid)*vect(I1,I2,I3,v1));
// 	  else 
// 	    uDotN(I1,I2,I3)=(arrayData(m1,side,axis,grid)*vect(I1,I2,I3,v0)+
// 			     arrayData(m2,side,axis,grid)*vect(I1,I2,I3,v1)+
// 			     arrayData(m3,side,axis,grid)*vect(I1,I2,I3,v2));
// 	}
// 	else
// 	{
// 	  if( numberOfDimensions==1 ) 
// 	    uDotN(I1,I2,I3)=arrayData(m1)*vect(I1,I2,I3,v0);
// 	  else if( numberOfDimensions==2 ) 
// 	    uDotN(I1,I2,I3)=arrayData(m1)*vect(I1,I2,I3,v0)
// 	      +arrayData(m2)*vect(I1,I2,I3,v1);
// 	  else 
// 	    uDotN(I1,I2,I3)=arrayData(m1)*vect(I1,I2,I3,v0)
// 	      +arrayData(m2)*vect(I1,I2,I3,v1)
// 	      +arrayData(m3)*vect(I1,I2,I3,v2);
// 	}
//       }
//       else if( bcOption==gridFunctionForcing )
//       {  // use user supplied variable values
// 	if( gfData.getComponentDimension(0) < numberOfDimensions )   
// 	  uDotN(I1,I2,I3)=gfData(I1,I2,I3,m1);
// 	else if( numberOfDimensions==1 ) 
// 	  uDotN(I1,I2,I3)=gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0);
// 	else if( numberOfDimensions==2 ) 
// 	  uDotN(I1,I2,I3)=gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0)
// 	    +gfData(I1,I2,I3,m2)*vect(I1,I2,I3,v1);
// 	else 
// 	  uDotN(I1,I2,I3)=gfData(I1,I2,I3,m1)*vect(I1,I2,I3,v0)
// 	    +gfData(I1,I2,I3,m2)*vect(I1,I2,I3,v1)
// 	    +gfData(I1,I2,I3,m3)*vect(I1,I2,I3,v2);
//       }
//       else
//       {
// 	throw "Invalid value for bcOption! (normalDerivativeOf[Normal/Tangential]Component)";
//       }
      
//       uA.reshape(1,uA.dimension(0),uA.dimension(1),uA.dimension(2),uA.dimension(3));

//       vect.reshape(1,vect.dimension(0),vect.dimension(1),vect.dimension(2),vect.dimension(3));
//       uDotN.reshape(1,uDotN.dimension(0),uDotN.dimension(1),uDotN.dimension(2),uDotN.dimension(3));
      
//       if( numberOfDimensions==1 )
//       {
// 	uDotN(0,I1,I2,I3)=( 
// 	  uDotN(0,I1,I2,I3) - (
// 	    +nmCoeff(M2(-1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1-1,I2  ,I3,n1))
// 	    +nmCoeff(M2( 0, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2  ,I3,n1))
// 	    +nmCoeff(M2(+1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1+1,I2  ,I3,n1))
//  	                    )       
//                                 )/nmCoeff(mGhost,I1,I2,I3);
//       }
//       else if( numberOfDimensions==2 )
//       {
// 	uDotN(0,I1,I2,I3)=( 
// 	  uDotN(0,I1,I2,I3) - (
// 	     nmCoeff(M2( 0,-1),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2-1,I3,n1)
//                                          +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2-1,I3,n2))
// 	    +nmCoeff(M2(-1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1-1,I2  ,I3,n1)
//                                          +vect(0,I1,I2,I3,v1)*uA(0,I1-1,I2  ,I3,n2))
// 	    +nmCoeff(M2( 0, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2  ,I3,n1)
//                                          +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2  ,I3,n2))
// 	    +nmCoeff(M2(+1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1+1,I2  ,I3,n1)
//                                          +vect(0,I1,I2,I3,v1)*uA(0,I1+1,I2  ,I3,n2))
// 	    +nmCoeff(M2( 0,+1),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2+1,I3,n1)
//                                          +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2+1,I3,n2))
//  	                    )       
//                                 )/nmCoeff(mGhost,I1,I2,I3);
//       }
//       else
//       {
// 	uDotN(0,I1,I2,I3)=( 
// 	  uDotN(0,I1,I2,I3) - (
// 	     nmCoeff(M3( 0, 0,-1),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2  ,I3-1,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2  ,I3-1,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1  ,I2  ,I3-1,n3))
// 	    +nmCoeff(M3( 0,-1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2-1,I3  ,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2-1,I3  ,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1  ,I2-1,I3  ,n3))
// 	    +nmCoeff(M3(-1, 0, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1-1,I2  ,I3  ,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1-1,I2  ,I3  ,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1-1,I2  ,I3  ,n3))
// 	    +nmCoeff(M3( 0, 0, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2  ,I3  ,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2  ,I3  ,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1  ,I2  ,I3  ,n3))
// 	    +nmCoeff(M3(+1, 0, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1+1,I2  ,I3  ,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1+1,I2  ,I3  ,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1+1,I2  ,I3  ,n3))
// 	    +nmCoeff(M3( 0,+1, 0),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2+1,I3  ,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2+1,I3  ,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1  ,I2+1,I3  ,n3))
// 	    +nmCoeff(M3( 0, 0,+1),I1,I2,I3)*(vect(0,I1,I2,I3,v0)*uA(0,I1  ,I2  ,I3+1,n1)
// 					    +vect(0,I1,I2,I3,v1)*uA(0,I1  ,I2  ,I3+1,n2)
// 					    +vect(0,I1,I2,I3,v2)*uA(0,I1  ,I2  ,I3+1,n3))
// 	    )
// 	  )/nmCoeff(mGhost,I1,I2,I3);
//       }
//       uA.reshape(uA.dimension(1),uA.dimension(2),uA.dimension(3),uA.dimension(4));
//       vect.reshape(vect.dimension(1),vect.dimension(2),vect.dimension(3),vect.dimension(4));
//       uDotN.reshape(uDotN.dimension(1),uDotN.dimension(2),uDotN.dimension(3),uDotN.dimension(4));

//       // Now set the vectorial component:
//       WHERE_MASK( uA(I1m,I2m,I3m,n1)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v0); )
//       if( numberOfDimensions > 1 )
//         WHERE_MASK( uA(I1m,I2m,I3m,n2)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v1); )
//       if( numberOfDimensions > 2 )
// 	WHERE_MASK( uA(I1m,I2m,I3m,n3)+=uDotN(I1,I2,I3)*vect(I1,I2,I3,v2); )

//     }
//     break;
//   }
//   timeForNormalDerivative+=getCPU()-time;

}
#undef M2
#undef M3

