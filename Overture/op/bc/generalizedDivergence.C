#include "MappedGridOperators.h"
#include "SparseRep.h"

#include "MappedGridOperatorsInclude.h"
#include "ParallelUtility.h"


// This next include file defines the prototype for assignBoundaryConditions
#include "assignBoundaryConditions.h"


// These are for indexing into a coefficient matrix
#undef M2
#define M2(m1,m2) ((m1)+1+3*((m2)+1))
#undef M3
#define M3(m1,m2,m3) ((m1)+1+3*((m2)+1+3*((m3)+1)))

//! Build the coefficient matrix used to apply the generalized divergence
/*!
  Return values: 0=success, 1=Fatal error, all coefficients are zero at one or more points.
 */
static int
buildGenDivCoefficientMatrix(MappedGridOperators & op,
			     realSerialArray & gdCoeff, 
                             Index &I1, Index &I2,Index &I3,
                             int is1, int is2, int is3,
			     int numberOfDimensions,
                             MappedGrid &c, const int side, const int axis )
{
      // save the coefficient on the ghost line for the operators x, y and z

//    gdCoeff.redim(1,
//  		Range(I1.getBase(),I1.getBound()),
//  		Range(I2.getBase(),I2.getBound()),
//  		Range(I3.getBase(),I3.getBound()),
//  		numberOfDimensions+1);

  gdCoeff.redim(1,I1,I2,I3,numberOfDimensions+1);
  Index M=Index(0,int(pow(3,numberOfDimensions)+.5));
  realSerialArray opX(M,I1,I2,I3);
  
  int mGhost;
  if( numberOfDimensions==2 )
  {
    mGhost=M2(-is1,-is2);   // coefficient index for ghost value
//      gdCoeff(0,I1,I2,I3,0)= op.xCoefficients(I1,I2,I3,0,0)(mGhost,I1,I2,I3);
//      gdCoeff(0,I1,I2,I3,1)= op.yCoefficients(I1,I2,I3,0,0)(mGhost,I1,I2,I3);

    op.assignCoefficients(MappedGridOperators::xDerivative,opX,I1,I2,I3,0,0);
    gdCoeff(0,I1,I2,I3,0)=opX(mGhost,I1,I2,I3);
    op.assignCoefficients(MappedGridOperators::yDerivative,opX,I1,I2,I3,0,0);
    gdCoeff(0,I1,I2,I3,1)=opX(mGhost,I1,I2,I3);

  }
  else
  {
    mGhost=M3(-is1,-is2,-is3);   // coefficient index for ghost value
//      gdCoeff(0,I1,I2,I3,0)= op.xCoefficients(I1,I2,I3,0,0)(mGhost,I1,I2,I3);
//      gdCoeff(0,I1,I2,I3,1)= op.yCoefficients(I1,I2,I3,0,0)(mGhost,I1,I2,I3);
//      gdCoeff(0,I1,I2,I3,2)= op.zCoefficients(I1,I2,I3,0,0)(mGhost,I1,I2,I3);

    op.assignCoefficients(MappedGridOperators::xDerivative,opX,I1,I2,I3,0,0);
    gdCoeff(0,I1,I2,I3,0)=opX(mGhost,I1,I2,I3);
    op.assignCoefficients(MappedGridOperators::yDerivative,opX,I1,I2,I3,0,0);
    gdCoeff(0,I1,I2,I3,1)=opX(mGhost,I1,I2,I3);
    op.assignCoefficients(MappedGridOperators::zDerivative,opX,I1,I2,I3,0,0);
    gdCoeff(0,I1,I2,I3,2)=opX(mGhost,I1,I2,I3);

  }
  // save the norm of the coefficients in the last position in the array
  gdCoeff.reshape(gdCoeff.dimension(1),gdCoeff.dimension(2),gdCoeff.dimension(3),gdCoeff.dimension(4));
  if( numberOfDimensions==2 )
    gdCoeff(I1,I2,I3,numberOfDimensions)=SQR(gdCoeff(I1,I2,I3,0))+SQR(gdCoeff(I1,I2,I3,1));
  else
    gdCoeff(I1,I2,I3,numberOfDimensions)=SQR(gdCoeff(I1,I2,I3,0))+SQR(gdCoeff(I1,I2,I3,1))+SQR(gdCoeff(I1,I2,I3,2));

  if( min(gdCoeff(I1,I2,I3,numberOfDimensions))==0. )
  {
    // ** on a singular side that is not used we can get zero coefficients -- check the mask for used points

    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    J1=I1, J2=I2, J3=I3;
    Jv[axis]=c.gridIndexRange(side,axis);          // boundary 
    #ifdef USE_PPP
      intSerialArray mask;  getLocalArrayWithGhostBoundaries(c.mask(),mask);
    #else
      const intSerialArray & mask = c.mask();
    #endif 

    where( mask(J1,J2,J3)==0 )   // mask on boundary==0 => BC not needed
    {
      gdCoeff(I1,I2,I3,numberOfDimensions)=1.;  // these points are not used anyway
    }
    // ** now check again **
    if( min(gdCoeff(I1,I2,I3,numberOfDimensions))==0. )
      return 1;
  }
  // invert here for efficiency
  gdCoeff(I1,I2,I3,numberOfDimensions)=1./gdCoeff(I1,I2,I3,numberOfDimensions);

  return 0;
}

void MappedGridOperators:: 
applyBCGenDiv(realMappedGridFunction & u, 
		 const int side,
		 const int axis,
		 const real & scalarData,
		 const RealArray & arrayData,
		 const RealArray & arrayDataD,
		 const realMappedGridFunction & gfData,
		 const real & t,
                 const IntegerArray & uC, const IntegerArray & fC, const IntegerDistributedArray & mask,
		 const BoundaryConditionParameters & bcParameters,
		 const BoundaryConditionOption bcOption,
		 const int & grid  )
// ==================================================================================================
// This boundary condition imposes a ``generalized divergence'' condition on a vector grid function
// $\uv=(u(0),u(1),u(2)$
// \[
//   \av\cdot\uv = a(0) u(0)_x + a(1) u(1)_y + a(2) u(2)_z = g
// \]
// ==================================================================================================
{
  real time=getCPU();
  
  if( orderOfAccuracy!=2 )
  {
    printf("MappedGridOperators::Sorry, the generalized divergence boundary condition is only implemented for\n"
           " orderOfAccuracy=2, requested orderOfAccuracy=%i. Continuing with 2nd-order\n",orderOfAccuracy);
  }

  MappedGrid & c = mappedGrid;
  e = twilightZoneFlowFunction;   // "e"xact solution  
  if( twilightZoneFlow )
    assert( twilightZoneFlowFunction != NULL );

  RealDistributedArray & uA = u;
  
  int n1,n2,n3,m1,m2,m3;
  getVelocityComponents( n1,n2,n3,m1,m2,m3, u,bcParameters,"generalizedDivergence",uC,fC );

  real b1,b2,b3;
  if( bcParameters.a.getLength(0) >= numberOfDimensions )
  {
    b1=bcParameters.a(0);
    b2= c.numberOfDimensions()>1 ? bcParameters.a(1) : 0.;
    b3= c.numberOfDimensions()>2 ? bcParameters.a(2) : 0.;
  }
  else
  {
    b1=b2=b3=1.;  // default values
  }
  if( fabs(b1)==0. && (numberOfDimensions<2 || fabs(b2)==0.) && (numberOfDimensions<3 || fabs(b3)==0.) )
  {
    printf("MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC\n");
    printf(" The elements of a in generalizedDivergence are all zero!\n");
    {throw "MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC";}
  }

  // Sometimes we use the mask that is passed in:
  const bool useSpecialWhereMask = (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                                    bcParameters.getUseMask() ;
 
  // check this next section
  #ifdef USE_PPP
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray gfDataLocal; getLocalArrayWithGhostBoundaries(gfData,gfDataLocal);
    const realSerialArray & arrayDataDLocal = arrayDataD;
    intSerialArray maskLocal; 
    if( useSpecialWhereMask )
      getLocalArrayWithGhostBoundaries(mask,maskLocal);
    else
      getLocalArrayWithGhostBoundaries(c.mask(),maskLocal);

  #else
    const realSerialArray & uLocal = u;
    const realSerialArray & gfDataLocal = gfData;
    const realSerialArray & arrayDataDLocal = arrayDataD;

    const intSerialArray & maskLocal = useSpecialWhereMask ? mask : c.mask();
  #endif


  Index I1,I2,I3;
  getGhostIndex( c.indexRange(),side,axis,I1,I2,I3,bcParameters.lineToAssign,bcParameters.extraInTangentialDirections);

  int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,n1a,n1b,n2a,n2b,n3a,n3b); 


  Range N;
  if( numberOfDimensions==2 )
    N=Range(min(n1,n2),max(n1,n2));
  else if( numberOfDimensions==3 )
    N=Range(min(n1,n2,n3),max(n1,n2,n3));
  else
    N=Range(n1,n1);

  const int isRectangular = rectangular;
  
  const int is1 = (axis==axis1) ? 1-2*side : 0;   
  const int is2 = (axis==axis2) ? 1-2*side : 0;           
  const int is3 = (axis==axis3) ? 1-2*side : 0;           

  const bool useOpt=true;
  if( useOpt )
  {
    // **********************
    //    use optimised BC 
    // **********************

    if( !ok ) return;  // return if there are no boundary pts on this processor 
    

    if( !( rectangular || numberOfDimensions==1 ) && !gdCoeffIsSet[axis][side] )
    {
      // *** In the curvilinear case we build a coeff matrix on the boundary 
     // save the coefficient on the ghost line for the operators x, y and z

      createBoundaryMatrix(side,axis,BCTypes::generalizedDivergence);

      realSerialArray & gdCoeff = generalizedDivergenceCoeff[axis][side]; 

      gdCoeffIsSet[axis][side]=true;
      int returnValue=buildGenDivCoefficientMatrix(*this,gdCoeff,I1,I2,I3,is1,is2,is3,numberOfDimensions,c,side,axis);

      if( returnValue==1 )
      {
	printf("MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC\n");
	printf(" The coefficients at some ghost points are all zero! \n"
               "   grid=%i (%s) side=%i, axis=%i. If this is an unused face on a polar singularity then\n"
               "   you should set the boundary condition to be zero for an interpolation boundary.\n",
               grid,(const char*)c.getName(),side,axis);
	printf(" The coefficients at some ghost points are all zero! grid=%i side=%i, axis=%i\n",grid,side,axis);
	printf(" n1=%i, n2=%i, n3=%i, b1=%e, b2=%e, b3=%e \n",n1,n2,n3,b1,b2,b3);
	gdCoeff(I1,I2,I3,numberOfDimensions).display("Here is norm");
	throw "error";
      }
      
    }
    
    real twoDeltaX=1., twoDeltaY=1., twoDeltaZ=1.;
    if( rectangular )
    {
      twoDeltaX = 2.*dx[0]; 
      twoDeltaY = 2.*dx[1]; 
      twoDeltaZ = 2.*dx[2]; 
    }
    else if ( c.numberOfDimensions()==1 )
    {
      twoDeltaX=2.*c.vertexDerivative()(n1a,n2a,n3a,0,0)*c.gridSpacing(0);
    }

//       RealDistributedArray *gfDatap=(RealDistributedArray *)(&gfData);
//       RealDistributedArray *rhsp=NULL;
    realSerialArray *gfDatap=(realSerialArray*)(&gfDataLocal);
    realSerialArray *rhsp=NULL;
    
    // option from : parameter( scalarForcing=0,gfForcing=1,arrayForcing=2, vectorForcing=3 )
    int option=-1;
    if( twilightZoneFlow )
    {

      // Fill in the rhs array with TZ forcing
      option=1;
// 	rhsp = new RealDistributedArray;
// 	RealDistributedArray & rhs = *rhsp;
      
// 	rhs.redim(I1,I2,I3,numberOfDimensions);
// 	gfDatap=&rhs;

      rhsp = new realSerialArray(I1,I2,I3,numberOfDimensions);
      realSerialArray & rhs = *rhsp;
      gfDatap=&rhs;
     
      c.update(MappedGrid::THEcenter);
      realArray & x= c.center();
#ifdef USE_PPP
      realSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
#else
      const realSerialArray & xLocal = x;
#endif  

      realSerialArray ux(I1,I2,I3);
      (*e).gd( ux,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,m1,t);

      rhs(I1,I2,I3,0)=ux; 
      if( numberOfDimensions>=2 )
      {
	(*e).gd( ux,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,m2,t);
	rhs(I1,I2,I3,1)=ux; // this is uy
      }
      if( numberOfDimensions>=3 )
      {
	(*e).gd( ux,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,m3,t);
	rhs(I1,I2,I3,2)=ux; // this is uz
      }
	
      m1=0, m2=1, m3=2;  // the rhs data is now found in these components.
    }
    else if( bcOption==scalarForcing )
    {
      option=0;
    }
    else if( bcOption==arrayForcing )
    {
      // *** check this ****

      if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
	  arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
	  arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
	  arrayDataD.getBase(3)<=min(fC(uC.dimension(0))) && arrayDataD.getBound(3)>=max(fC(uC.dimension(0))) )
      {
	option=1;
	gfDatap=(realSerialArray *)(&arrayDataDLocal);  // use arrayDataD(I1,I2,I3,fC(n))
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
    real par[]={b1,b2,b3,twoDeltaX,twoDeltaY,twoDeltaZ};
    int ipar[]={n1,n2,n3,m1,m2,m3};  
    real dr[3]={1.,1.,1.}; // not used
    int ca=0,cb=0;   // not used
    
    const int gridType = rectangular ? 0 : 1;

    // We should always use the mask since the generalized divergence will be bad near interpolation pts 061015
    const int useWhereMask = true ||
                             (useWhereMaskOnBoundary[axis][side] && bcParameters.useMixedBoundaryMask) ||
                             bcParameters.getUseMask() ;

    assert( gfDatap!=NULL );
    const realSerialArray & gfd = *gfDatap;
    
    real *up = getDataPointer(uLocal);
    real *adp= getDataPointer(arrayData);
    const real *arrayDatap=adp!=NULL ? adp : up;
    const int *mp=getDataPointer(maskLocal);
    const int *maskp = mp!=NULL ? mp : &uC(uC.getBase(0));

    realSerialArray *gdCoeffp;
    if( gdCoeffIsSet[axis][side] )
      gdCoeffp=& generalizedDivergenceCoeff[axis][side];
    else
      gdCoeffp=&((realSerialArray&)uLocal);  // gdCoeff not used in this case, just pass u

    realSerialArray & gdCoeff = *gdCoeffp;

    realSerialArray *uxp=NULL;
    if( !( rectangular || numberOfDimensions==1 ) )
    {
      // for the curvilinear case we need to pass in ux(n1), uy(n2) and uz(n3)
      uxp=new realSerialArray(I1,I2,I3,N);
      derivative(MappedGridOperators::xDerivative,uLocal,*uxp,I1,I2,I3,n1);
      derivative(MappedGridOperators::yDerivative,uLocal,*uxp,I1,I2,I3,n2);
      if( numberOfDimensions>2 )
	derivative(MappedGridOperators::zDerivative,uLocal,*uxp,I1,I2,I3,n3);
    }
    realSerialArray & ux = uxp!=NULL ? *uxp : (realSerialArray &)uLocal;
    
//      printf(" gfd.getBase(0)=%i gfd.getBound(0)=%i \n",gfd.getBase(0),gfd.getBound(0));
//      printf(" gfd.getBase(1)=%i gfd.getBound(1)=%i \n",gfd.getBase(1),gfd.getBound(1));
//      printf(" gfd.getBase(2)=%i gfd.getBound(2)=%i \n",gfd.getBase(2),gfd.getBound(2));

    const real *rxp = up;  // Jacobian not needed here


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
	  printf("generalizedDivergence:ERROR:mask bounds are not valid!\n"
		 " n1a,n1b,n2a,n2b,n3a,n3b = %i,%i,%i,%i,%i,%i \n"
		 " mask bounds = [%i,%i][%i,%i][%i,%i]\n",n1a,n1b,n2a,n2b,n3a,n3b,
		 ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2]);
	  Overture::abort("ERROR:generalizedDivergence");
	}
      }
    }

    // printf(" **** generalizedDivergence: useWhereMask=%i\n",useWhereMask);

    int bcType=BCTypes::generalizedDivergence;
    assignBoundaryConditions( c.numberOfDimensions(), 
			      n1a,n1b,n2a,n2b,n3a,n3b,
			      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
			      uLocal.getBase(2),uLocal.getBound(2),uLocal.getBase(3),uLocal.getBound(3),
			      ux.getBase(0),ux.getBound(0),ux.getBase(1),ux.getBound(1),
			      ux.getBase(2),ux.getBound(2),ux.getBase(3),ux.getBound(3),
			      gdCoeff.getBase(0),gdCoeff.getBound(0),gdCoeff.getBase(1),gdCoeff.getBound(1),
			      gdCoeff.getBase(2),gdCoeff.getBound(2),gdCoeff.getBase(3),gdCoeff.getBound(3),
			      gfd.getBase(0),gfd.getBound(0),gfd.getBase(1),gfd.getBound(1),
			      gfd.getBase(2),gfd.getBound(2),gfd.getBase(3),gfd.getBound(3),
			      arrayData.getBase(0),arrayData.getBound(0),arrayData.getBase(1),arrayData.getBound(1),
			      arrayData.getBase(2),arrayData.getBound(2),arrayData.getBase(3),arrayData.getBound(3),
			      arrayData.getBase(0),arrayData.getBound(0),
			      ndm[0][0],ndm[1][0],ndm[0][1],ndm[1][1],ndm[0][2],ndm[1][2],  // dimensions for mask
			      uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                              uLocal.getBase(2),uLocal.getBound(2),
			      *rxp,
			      *up,*getDataPointer(ux), *getDataPointer(gdCoeff), *maskp,
			      scalarData,*getDataPointer(gfd),*arrayDatap,*arrayDatap,
			      dx[0],dr[0],ipar[0], par[0], ca,cb, 
                              uC.getBase(0),uC(uC.getBase(0)), fC.getBase(0),fC(fC.getBase(0)),
			      side,axis,grid, bcType, option,gridType,orderOfAccuracy,useWhereMask,
                              bcParameters.lineToAssign );
  
  
    delete rhsp;
    delete uxp;
    

//      Index I1m,I2m,I3m;
//      getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1); // first ghost line
//      uNew=u(I1m,I2m,I3m,u.dimension(3));
    
    timeForGeneralizedDivergence+=getCPU()-time;
    return;  // *********************
    
  }

// * 
// *   Index I1m,I2m,I3m;
// *   getGhostIndex( c.indexRange(),side,axis,I1m,I2m,I3m,+1,bcParameters.extraInTangentialDirections); // first ghost line
// * 
// *   Index I1p,I2p,I3p;
// *   getGhostIndex( c.indexRange(),side,axis,I1p,I2p,I3p,-1,bcParameters.extraInTangentialDirections); // first line in
// * 
// *   RealDistributedArray uDotN(I1,I2,I3);
// *   RealDistributedArray & rhs = uDotN;
// * 
// * 
// *   // now fill in the rhs
// *   if( twilightZoneFlow ) 
// *   { 
// *     if( numberOfDimensions==1 )
// *       rhs(I1,I2,I3)=(e->x(c,I1,I2,I3,m1,t)*b1);
// *     else if( numberOfDimensions==2 )
// *       rhs(I1,I2,I3)=(e->x(c,I1,I2,I3,m1,t)*b1+
// * 		     e->y(c,I1,I2,I3,m2,t)*b2);
// *     else
// *       rhs(I1,I2,I3)=(e->x(c,I1,I2,I3,m1,t)*b1+
// * 		     e->y(c,I1,I2,I3,m2,t)*b2+
// * 		     e->z(c,I1,I2,I3,m3,t)*b3);
// *   }
// *   else if( bcOption==scalarForcing )
// *     rhs(I1,I2,I3)=scalarData;  
// *   else if( bcOption==arrayForcing )
// *   {
// *     if( arrayDataD.getBase(0)<=I1.getBase() && arrayDataD.getBound(0)>=I1.getBound() &&
// *         arrayDataD.getBase(1)<=I2.getBase() && arrayDataD.getBound(1)>=I2.getBound() &&
// *         arrayDataD.getBase(2)<=I3.getBase() && arrayDataD.getBound(2)>=I3.getBound() &&
// * 	arrayDataD.getBase(3)<=min(m1,m2,m3) && arrayDataD.getBound(3)>=max(m1,m2,m3) )
// *     {
// *       rhs(I1,I2,I3)=b1*arrayDataD(I1,I2,I3,m1)+b2*arrayDataD(I1,I2,I3,m2)+b3*arrayDataD(I1,I2,I3,m3);  
// *     }
// *     else if( side<=arrayData.getBound(1) && axis<=arrayData.getBound(2) && grid<=arrayData.getBound(3) )
// *       rhs(I1,I2,I3)=b1*arrayData(m1,side,axis,grid)+b2*arrayData(m2,side,axis,grid)+b3*arrayData(m3,side,axis,grid);  
// *     else
// *       rhs(I1,I2,I3)=b1*arrayData(m1)+b2*arrayData(m2)+b3*arrayData(m3);  
// *   }
// *   else if( bcOption==gridFunctionForcing )
// *   {
// *     if( gfData.getComponentDimension(0) < numberOfDimensions )   
// *       rhs(I1,I2,I3)=gfData(I1,I2,I3);   
// *     else
// *     {
// *       if( numberOfDimensions==1 )
// *         rhs(I1,I2,I3)=b1*gfData(I1,I2,I3,m1);
// *       else if( numberOfDimensions==2 )
// *         rhs(I1,I2,I3)=b1*gfData(I1,I2,I3,m1)+b2*gfData(I1,I2,I3,m2);
// *       else
// *         rhs(I1,I2,I3)=b1*gfData(I1,I2,I3,m1)+b2*gfData(I1,I2,I3,m2)+b3*gfData(I1,I2,I3,m3);  
// *     }
// *   }
// *   else
// *   {
// *     printf("Invalid bcOption for BC generalizedDivergence = %i\n",bcOption);
// *     {throw "Invalid bcOption for BC generalizedDivergence";}
// *   }
// * 
// *   // to set the component along a to g:
// *   //       u <- u + (g-(a.u)) a/<a,a>
// *   //   g-(a.u) = b - ( discrete div of u )
// * 
// *   if( rectangular || numberOfDimensions==1 )
// *   {
// *     // ****************
// *     // ***rectangular**
// *     // ****************
// * 
// *     //  a(0) u(0)_x + a(1) u(1)_y + a(2) u(2)_z = g
// *     real twoDeltaX = 2.*dx[axis1]; // 1./h21(axis1);
// *     if( !rectangular )
// *     {
// *        // 1D, non-rectangular:   u.n = (+/-) (1/x.r) D0r u
// *       twoDeltaX=2.*c.vertexDerivative()(I1.getBase(),I2.getBase(),I3.getBase(),axis1,axis1)*c.gridSpacing()(axis1);
// *     }
// *     real twoDeltaY = 2.*dx[axis2]; // 1./h21(axis2);
// *     
// *     if( numberOfDimensions==1 )
// *     {
// * 	uA(I1m,I2m,I3m,n1)=uA(I1p,I2p,I3p,n1) +((2*side-1)*twoDeltaX/b1)*( rhs(I1,I2,I3) );
// *     }
// *     else if( numberOfDimensions==2 )
// *     {
// *       if( axis==axis1 )
// *       {
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n1)=uA(I1p,I2p,I3p,n1)
// * 	  +((2*side-1)*twoDeltaX/b1)*
// *             ( rhs(I1,I2,I3) - (uA(I1,I2+1,I3,n2)-uA(I1,I2-1,I3,n2))*(b2/twoDeltaY) ); )
// *       }
// *       else
// *       {
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n2)=uA(I1p,I2p,I3p,n2)
// * 	  +((2*side-1)*twoDeltaY/b2)*
// *            ( rhs(I1,I2,I3)-(uA(I1+1,I2,I3,n1)-uA(I1-1,I2,I3,n1))*(b1/twoDeltaX) ); )
// *       }
// *     }
// *     else
// *     {
// *       real twoDeltaZ = 2.*dx[axis3]; // 1./h21(axis3);
// *       if( axis==axis1 )
// *       {
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n1)=uA(I1p,I2p,I3p,n1)
// * 	  +((2*side-1)*twoDeltaX/b1)*
// *             ( rhs(I1,I2,I3) - (uA(I1  ,I2+1,I3  ,n2)-uA(I1  ,I2-1,I3  ,n2))*(b2/twoDeltaY)
// *                             - (uA(I1  ,I2  ,I3+1,n3)-uA(I1  ,I2  ,I3-1,n3))*(b3/twoDeltaZ) ); )
// *       }
// *       else if( axis==axis2 )
// *       {
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n2)=uA(I1p,I2p,I3p,n2)
// * 	  +((2*side-1)*twoDeltaY/b2)*
// *             ( rhs(I1,I2,I3) - (uA(I1+1,I2  ,I3  ,n1)-uA(I1-1,I2  ,I3  ,n1))*(b1/twoDeltaX)
// *                             - (uA(I1  ,I2  ,I3+1,n3)-uA(I1  ,I2  ,I3-1,n3))*(b3/twoDeltaZ) ); )
// *       }
// *       else
// *       {
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n3)=uA(I1p,I2p,I3p,n3)
// * 	  +((2*side-1)*twoDeltaZ/b3)*
// *             ( rhs(I1,I2,I3) - (uA(I1+1,I2  ,I3  ,n1)-uA(I1-1,I2  ,I3  ,n1))*(b1/twoDeltaX)
// *                             - (uA(I1  ,I2+1,I3  ,n2)-uA(I1  ,I2-1,I3  ,n2))*(b2/twoDeltaY) ); )
// *       }
// *     }
// *   }
// *   else
// *   {
// *     // ******************
// *     // ***general case***
// *     // ******************
// * 
// * 
// *     if( !gdCoeffIsSet[axis][side] )
// *       createBoundaryMatrix(side,axis,BCTypes::generalizedDivergence);
// *     RealDistributedArray & gdCoeff = generalizedDivergenceCoeff[axis][side]; 
// * 
// *     if( !gdCoeffIsSet[axis][side] )
// *     {
// *       // save the coefficient on the ghost line for the operators x, y and z
// *       gdCoeffIsSet[axis][side]=TRUE;
// *       int returnValue=buildGenDivCoefficientMatrix(*this,gdCoeff,I1,I2,I3,is1,is2,is3,numberOfDimensions,c,side,axis);
// * 
// *       if( returnValue==1 )
// *       {
// * 	printf("MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC\n");
// * 	printf(" The coefficients at some ghost points are all zero! side=%i, axis=%i\n",side,axis);
// * 	printf(" n1=%i, n2=%i, n3=%i, b1=%e, b2=%e, b3=%e \n",n1,n2,n3,b1,b2,b3);
// * 	gdCoeff(I1,I2,I3,numberOfDimensions).display("Here is norm");
// * 	throw "error";
// *       }
// * 
// *     }
// * 
// * 
// *     RealDistributedArray ux(I1,I2,I3,N);
// *     if( b1==b2 && b2==b3 )
// *     {
// *       // optimize this special (common) case
// * /* ----
// *       if( numberOfDimensions==2 )
// *       {
// *         if( (n1+1)==n2 )
// *   	  rhs(I1,I2,I3)-=div(u,I1,I2,I3,Range(n1,n2))(I1,I2,I3);  
// *         else
// * 	  rhs(I1,I2,I3)-=x(u,I1,I2,I3,n1)(I1,I2,I3,n1)+y(u,I1,I2,I3,n2)(I1,I2,I3,n2);
// *         
// *       }
// *       else
// *       {
// *         if( (n1+1)==n2 && (n2+1)==n3 )
// *   	  rhs(I1,I2,I3)-=div(u,I1,I2,I3,Range(n1,n3))(I1,I2,I3);  
// *         else
// * 	  rhs(I1,I2,I3)-=x(u,I1,I2,I3,n1)(I1,I2,I3,n1)+y(u,I1,I2,I3,n2)(I1,I2,I3,n2)+z(u,I1,I2,I3,n3)(I1,I2,I3,n3);
// *       }
// *   ---- */
// *       derivative(MappedGridOperators::xDerivative,u,ux,I1,I2,I3,n1);
// *       rhs(I1,I2,I3)-=ux(I1,I2,I3,n1);
// *       derivative(MappedGridOperators::yDerivative,u,ux,I1,I2,I3,n2);
// *       rhs(I1,I2,I3)-=ux(I1,I2,I3,n2);
// *       if( numberOfDimensions>2 )
// *       {
// * 	derivative(MappedGridOperators::zDerivative,u,ux,I1,I2,I3,n3);
// * 	rhs(I1,I2,I3)-=ux(I1,I2,I3,n3);
// *       }
// * 
// *       rhs(I1,I2,I3)*=gdCoeff(I1,I2,I3,numberOfDimensions);
// *       WHERE_MASK( uA(I1m,I2m,I3m,n1)+=rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,0); )
// *       WHERE_MASK( uA(I1m,I2m,I3m,n2)+=rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,1); )
// *       if( numberOfDimensions==3 )
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n3)+=rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,2); )
// *     }
// *     else
// *     {
// *       derivative(MappedGridOperators::xDerivative,u,ux,I1,I2,I3,n1);
// *       rhs(I1,I2,I3)-=b1*ux(I1,I2,I3,n1);
// *       derivative(MappedGridOperators::yDerivative,u,ux,I1,I2,I3,n2);
// *       rhs(I1,I2,I3)-=b2*ux(I1,I2,I3,n2);
// *       if( numberOfDimensions>2 )
// *       {
// * 	derivative(MappedGridOperators::zDerivative,u,ux,I1,I2,I3,n3);
// * 	rhs(I1,I2,I3)-=b3*ux(I1,I2,I3,n3);
// *       }
// * 
// *       if( numberOfDimensions==2 )
// *       {
// *         // **** this may loose accuracy since u may not be smooth --> u.x ot u.y large ****
// * //	rhs(I1,I2,I3)-=b1*x(u,I1,I2,I3,n1)(I1,I2,I3,n1)+b2*y(u,I1,I2,I3,n2)(I1,I2,I3,n2);
// *         rhs(I1,I2,I3)/= (b1*b1)*SQR(gdCoeff(I1,I2,I3,0))+(b2*b2)*SQR(gdCoeff(I1,I2,I3,1));
// *       }
// *       else
// *       {
// * //	rhs(I1,I2,I3)-=b1*x(u,I1,I2,I3,n1)(I1,I2,I3,n1)+b2*y(u,I1,I2,I3,n2)(I1,I2,I3,n2)+b3*z(u,I1,I2,I3,n3)(I1,I2,I3,n3);
// * 	rhs(I1,I2,I3)/= (b1*b1)*SQR(gdCoeff(I1,I2,I3,0))+(b2*b2)*SQR(gdCoeff(I1,I2,I3,1))+(b3*b3)*SQR(gdCoeff(I1,I2,I3,2));
// *       }
// *       
// * 
// *       WHERE_MASK( uA(I1m,I2m,I3m,n1)+=b1*rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,0); )
// *       WHERE_MASK( uA(I1m,I2m,I3m,n2)+=b2*rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,1); )
// *       if( numberOfDimensions==3 ) 
// * 	WHERE_MASK( uA(I1m,I2m,I3m,n3)+=b3*rhs(I1,I2,I3)*gdCoeff(I1,I2,I3,2); )
// *       
// *     }
// *   }
// *   
    
/* ----  
    if( numberOfDimensions==2 )
    {
      mGhost=M2(-is1,-is2);   // coefficient index for ghost value
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    norm(i1,i2,i3)=SQR(opX(mGhost,i1,i2,i3))+SQR(opY(mGhost,i1,i2,i3));
      if( min(norm(I1,I2,I3))==0. )
      {
	printf("MappedGridOperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC\n");
	printf(" The coefficients at some ghost points are all zero! side=%i, axis=%i\n",side,axis);
	printf(" n1=%i, n2=%i, b1=%e, b2=%e \n",n1,n2,b1,b2);
	norm(I1,I2,I3).display("Here is norm(I1,I2,I3)");
	throw "error";
      }
      // Here is b1*u_x+b2*v_y 
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    rhs(i1,i2,i3)-=( 
	      opX(M2( 0,-1),i1,i2,i3)*u(i1  ,i2-1,i3,n1)
	      +opX(M2(-1, 0),i1,i2,i3)*u(i1-1,i2  ,i3,n1)
	      +opX(M2( 0, 0),i1,i2,i3)*u(i1  ,i2  ,i3,n1)
	      +opX(M2( 1, 0),i1,i2,i3)*u(i1+1,i2  ,i3,n1)
	      +opX(M2( 0, 1),i1,i2,i3)*u(i1  ,i2+1,i3,n1)
	      +opY(M2( 0,-1),i1,i2,i3)*u(i1  ,i2-1,i3,n2)
	      +opY(M2(-1, 0),i1,i2,i3)*u(i1-1,i2  ,i3,n2)
	      +opY(M2( 0, 0),i1,i2,i3)*u(i1  ,i2  ,i3,n2)
	      +opY(M2( 1, 0),i1,i2,i3)*u(i1+1,i2  ,i3,n2)
	      +opY(M2( 0, 1),i1,i2,i3)*u(i1  ,i2+1,i3,n2)
	      );
    }
    else
    {
      opZ.redim(opX);
      opZ= b3*zCoefficients(I1,I2,I3,0,0)(M,I1,I2,I3);
      mGhost=M3(-is1,-is2,-is3);   // coefficient index for ghost value
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    norm(i1,i2,i3)=SQR(opX(mGhost,i1,i2,i3))+SQR(opY(mGhost,i1,i2,i3))+SQR(opZ(mGhost,i1,i2,i3));
	  }
      if( min(norm(I1,I2,I3))==0. )
      {
	printf("MappedGridoperators::applyBoundaryConditions:ERROR applying generalizedDivergence BC\n");
	printf(" The coefficients are all zero! side=%i, axis=%i\n",side,axis);
	printf(" n1=%i, n2=%i, n3=%i, b1=%e, b2=%e, b3=%e \n",n1,n2,n3,b1,b2,b3);
	printf(" mGhost=%i, (is1,is2,is3)=(%i,%i,%i), side=%i, axis=%i \n",mGhost,is1,is2,is3,side,axis);
	opX.display("Here is opX");
	opY.display("Here is opY");
	opZ.display("Here is opZ");
	norm.display("Here is norm");
	throw "error";
      }
      // Here is b1*u_x+b2*v_y+b3*u_z
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    rhs(i1,i2,i3)-=( 
	      opX(M3(0,-1,0),i1,i2,i3)*u(i1  ,i2-1,i3  ,n1)
	      +opX(M3(-1,0,0),i1,i2,i3)*u(i1-1,i2  ,i3  ,n1)
	      +opX(M3(0, 0,0),i1,i2,i3)*u(i1  ,i2  ,i3  ,n1)
	      +opX(M3(1, 0,0),i1,i2,i3)*u(i1+1,i2  ,i3  ,n1)
	      +opX(M3(0, 1,0),i1,i2,i3)*u(i1  ,i2+1,i3  ,n1)
	      +opX(M3(0,0,-1),i1,i2,i3)*u(i1  ,i2  ,i3-1,n1)
	      +opX(M3(0,0,+1),i1,i2,i3)*u(i1  ,i2  ,i3+1,n1)

	      +opY(M3(0,-1,0),i1,i2,i3)*u(i1  ,i2-1,i3  ,n2)
	      +opY(M3(-1,0,0),i1,i2,i3)*u(i1-1,i2  ,i3  ,n2)
	      +opY(M3(0, 0,0),i1,i2,i3)*u(i1  ,i2  ,i3  ,n2)
	      +opY(M3(1, 0,0),i1,i2,i3)*u(i1+1,i2  ,i3  ,n2)
	      +opY(M3(0, 1,0),i1,i2,i3)*u(i1  ,i2+1,i3  ,n2)
	      +opY(M3(0,0,-1),i1,i2,i3)*u(i1  ,i2  ,i3-1,n2)
	      +opY(M3(0,0,+1),i1,i2,i3)*u(i1  ,i2  ,i3+1,n2)

	      +opZ(M3(0,-1,0),i1,i2,i3)*u(i1  ,i2-1,i3  ,n3)
	      +opZ(M3(-1,0,0),i1,i2,i3)*u(i1-1,i2  ,i3  ,n3)
	      +opZ(M3(0, 0,0),i1,i2,i3)*u(i1  ,i2  ,i3  ,n3)
	      +opZ(M3(1, 0,0),i1,i2,i3)*u(i1+1,i2  ,i3  ,n3)
	      +opZ(M3(0, 1,0),i1,i2,i3)*u(i1  ,i2+1,i3  ,n3)
	      +opZ(M3(0,0,-1),i1,i2,i3)*u(i1  ,i2  ,i3-1,n3)
	      +opZ(M3(0,0,+1),i1,i2,i3)*u(i1  ,i2  ,i3+1,n3)
	      );
    }
  
    rhs(I1,I2,I3)/=norm(I1,I2,I3);

    if( numberOfDimensions==2 )
    {
      for( i3=I3.getBase(), i3m=I3m.getBase(); i3<=I3.getBound(); i3++,i3m++ )
	for( i2=I2.getBase(), i2m=I2m.getBase(); i2<=I2.getBound(); i2++,i2m++ )
	  for( i1=I1.getBase(), i1m=I1m.getBase(); i1<=I1.getBound(); i1++,i1m++ )
	  {
	    u(i1m,i2m,i3m,n1)+=rhs(i1,i2,i3)*opX(mGhost,i1,i2,i3); 
	    u(i1m,i2m,i3m,n2)+=rhs(i1,i2,i3)*opY(mGhost,i1,i2,i3); 
	  }
    }
    else
    {
      for( i3=I3.getBase(), i3m=I3m.getBase(); i3<=I3.getBound(); i3++,i3m++ )
	for( i2=I2.getBase(), i2m=I2m.getBase(); i2<=I2.getBound(); i2++,i2m++ )
	  for( i1=I1.getBase(), i1m=I1m.getBase(); i1<=I1.getBound(); i1++,i1m++ )
	  {
	    u(i1m,i2m,i3m,n1)+=rhs(i1,i2,i3)*opX(mGhost,i1,i2,i3); 
	    u(i1m,i2m,i3m,n2)+=rhs(i1,i2,i3)*opY(mGhost,i1,i2,i3); 
	    u(i1m,i2m,i3m,n3)+=rhs(i1,i2,i3)*opZ(mGhost,i1,i2,i3);
	  }
    }
    
  }
--- */

  timeForGeneralizedDivergence+=getCPU()-time;

}

#undef M2
#undef M3
