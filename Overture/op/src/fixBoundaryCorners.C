#include "GenericMappedGridOperators.h"
#include "SparseRep.h"
#include "GridFunctionParameters.h"
#include "ParallelUtility.h"

#define fixBoundaryCornersOpt EXTERN_C_NAME(fixboundarycornersopt)

extern "C"
{
  void fixBoundaryCornersOpt( const int&nd, 
      const int&ndu1a,const int&ndu1b,const int&ndu2a,const int&ndu2b,
      const int&ndu3a,const int&ndu3b,const int&ndu4a,const int&ndu4b,
      const int&ndm1a,const int&ndm1b,const int&ndm2a,const int&ndm2b,const int&ndm3a,const int&ndm3b,
      real&u, const int&mask, const real& rsxy,
      const int&indexRange, const int&dimension, 
      const int&isPeriodic, const int&bc, const int&cornerBC, const int&ipar, const real&rpar );


}


// extern realMappedGridFunction Overture::nullDoubleMappedGridFunction();
// extern realMappedGridFunction Overture::nullFloatMappedGridFunction();
// ifdef OV_USE_DOUBLE
// define NULLRealMappedGridFunction Overture::nullDoubleMappedGridFunction()
// else
// define NULLRealMappedGridFunction Overture::nullFloatMappedGridFunction()
// endif


#define UX1(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
          u(i1+  (n1),i2+  (n2),i3+  (n3),n)

#define UX2(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
     + 2.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -    u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n) 


#define UX3(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
     + 3.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 3.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +    u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)

#define UX4(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
     + 4.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     - 6.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     + 4.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     -    u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)

#define UX5(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
     + 5.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -10.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +10.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     - 5.*u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)  \
     +    u(i1+5*(n1),i2+5*(n2),i3+5*(n3),n)

#define UX6(n1,n2,n3,i1,i2,i3,n)              /* @PM */ \
     + 6.*u(i1+  (n1),i2+  (n2),i3+  (n3),n)  \
     -15.*u(i1+2*(n1),i2+2*(n2),i3+2*(n3),n)  \
     +20.*u(i1+3*(n1),i2+3*(n2),i3+3*(n3),n)  \
     -15.*u(i1+4*(n1),i2+4*(n2),i3+4*(n3),n)  \
     + 6.*u(i1+5*(n1),i2+5*(n2),i3+5*(n3),n)  \
     -    u(i1+6*(n1),i2+6*(n2),i3+6*(n3),n)


// this macro is used in fixBoundaryCorners to extrapolate to different orders **** not used anymore ****
#define EXTRAP_SWITCH( i1,i2,i3,n, is1,is2,is3,j1,j2,j3,m )  \
        if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==extrapolateCorner ) \
        {  \
	  switch( orderOfExtrapolation ) \
	  { \
	  case 1: \
            u(i1,i2,i3,n)=UX1(is1,is2,is3,j1,j2,j3,m);  \
            break; \
	  case 2: \
            u(i1,i2,i3,n)=UX2(is1,is2,is3,j1,j2,j3,m);    \
            break; \
	  case 3:  \
            u(i1,i2,i3,n)=UX3(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  case 4:  \
            u(i1,i2,i3,n)=UX4(is1,is2,is3,j1,j2,j3,m);  \
            break;  \
	  case 5:  \
            u(i1,i2,i3,n)=UX5(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  case 6:  \
            u(i1,i2,i3,n)=UX6(is1,is2,is3,j1,j2,j3,m);   \
            break;  \
	  default:  \
	    cout << "fixBoundaryCorners:Error: unable to extrapolate to orderOfExtrapolation= "   \
		 << bcParameters.orderOfExtrapolation << ", can only do orders 1 to 6" << endl;  \
	  } \
        } \
        else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==symmetryCorner ) \
        { \
	    /* symmetry boundary condition */  \
          u(i1,i2,i3,n)=u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n);   \
        } \
        else \
        { \
	    cout << "fixBoundaryCorners:Error: unknown bcParameters.cornerBoundaryCondition="  \
		 << bcParameters.getCornerBoundaryCondition(side1,side2,side3) << endl;  \
	} 

// @PD realArray4[u] Range[i1,i2,i3,n,j1,j2,j3,m]


static int
assignCorners( const Index & i1, const Index & i2, const Index & i3, const Index & n, 
               int is1, int is2,int is3,
               const Index & j1, const Index & j2, const Index & j3, const Index & m,
               int side1, int side2, int side3, int orderOfExtrapolation,
               realArray & u, const BoundaryConditionParameters & bcParameters, int numberOfDimensions )
// ================================================================================================
//  /Description:
//     Apply an extrapolation or symmetry boundary condition.
//  /i1,i2,i3,n: Index;'s of points to assign.
// ===============================================================================================
{
  // printf("assignCorners: orderOfExtrapolation=%i\n",orderOfExtrapolation);
  
  

  if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::extrapolateCorner ) 
  {  
    switch( orderOfExtrapolation ) 
    { 
    case 1: 
      u(i1,i2,i3,n)=UX1(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      break; 
    case 2: 
      u(i1,i2,i3,n)=UX2(is1,is2,is3,j1,j2,j3,m);  /* @PA */   
      break; 
    case 3:  
      u(i1,i2,i3,n)=UX3(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      break;  
    case 4:  
      u(i1,i2,i3,n)=UX4(is1,is2,is3,j1,j2,j3,m);  /* @PA */  
      break;  
    case 5:  
      u(i1,i2,i3,n)=UX5(is1,is2,is3,j1,j2,j3,m); 
      break;  
    case 6:  
      u(i1,i2,i3,n)=UX6(is1,is2,is3,j1,j2,j3,m); 
      break;  
    default:  
      cout << "fixBoundaryCorners:Error: unable to extrapolate to orderOfExtrapolation= "   
	   << bcParameters.orderOfExtrapolation << ", can only do orders 1 to 6" << endl;  
    } 
  } 
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::symmetryCorner ) 
  { 
    /* symmetry boundary condition */  
    u(i1,i2,i3,n)=u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n);  /* @PA */  
  } 
  else if( bcParameters.getCornerBoundaryCondition(side1,side2,side3)==BoundaryConditionParameters::taylor2ndOrder ) 
  { 
    // Using a taylor approximation:
    //  u(+1,+1) = u(0,0) +dr*ur + ds*us + dr^2/2 urr + dr*ds*urs + ds^2/2 uss + ...
    //  u(-1,-1) = u(0,0) -dr*ur - ds*us + dr^2/2 urr + ...
    //  u(-1,-1) = u(1,1) -2dr*ur -2*ds*us + O(dr^3+...)
    //  ur = (u(1,0)-u(-1,0))/(2dr)
    // gives
    //   u(-1,-1) = u(1,1) -( u(1,0)-u(-1,0) ) - (u(0,1)-u(0,-1))
    if( numberOfDimensions==2 )
      u(i1,i2,i3,n)=(u(j1+2*(is1),j2+2*(is2),j3,n)-  
                     u(j1+2*(is1),j2+  (is2),j3,n)+
                     u(j1        ,j2+  (is2),j3,n)-
                     u(j1+  (is1),j2+2*(is2),j3,n)+
		     u(j1+  (is1),j2        ,j3,n));
    else if( numberOfDimensions==3 )
      u(i1,i2,i3,n)=(u(j1+2*(is1),j2+2*(is2),j3+2*(is3),n)-  
                     u(j1+2*(is1),j2+  (is2),j3+  (is3),n)+
                     u(j1        ,j2+  (is2),j3+  (is3),n)-
                     u(j1+  (is1),j2+2*(is2),j3+  (is3),n)+
		     u(j1+  (is1),j2        ,j3+  (is3),n)-
                     u(j1+  (is1),j2+  (is2),j3+2*(is3),n)+  
                     u(j1+  (is1),j2+  (is2),j3        ,n));
  }
  else 
  { 
    cout << "fixBoundaryCorners:Error: unknown bcParameters.cornerBoundaryCondition="  
	 << bcParameters.getCornerBoundaryCondition(side1,side2,side3) << endl;  
  } 
  return 0;
}


// #define getCPUOpt() getCPU()
#define getCPUOpt() 0.

void GenericMappedGridOperators::
fixBoundaryCorners(realMappedGridFunction & u,
                   const BoundaryConditionParameters & bcParameters /* = Overture::defaultBoundaryConditionParameters() */,
		   const Range & C0 /* =nullRange */ )
//======================================================================
//
// /Description:
// This is a fix-up routine to get the solution
// at corners, including the ghost points outside corners.
//
// /bcParameters.lineToAssign (input) : if zero assign all ghost corner points. If 1 only assign
//    ghost corner points on ghost line 2 or greater. If 2 only assign on ghost corner points on
//   ghost line3 or greater, etc.
//
// /NOTE: This function calls u.updateGhostBoundaries() for updating parallel ghost points
//======================================================================
{
  //     ---Fix periodic edges
  u.periodicUpdate(C0);
  
  real time=getCPUOpt();
  MappedGrid & c = *u.getMappedGrid();

  if ( c.getGridType()==GenericGrid::unstructuredGrid )
    return;

  const IntegerArray & boundaryCondition = c.boundaryCondition();
  const intArray & mask=c.mask();
  const realArray & uu = u;
  const int *bc = &boundaryCondition(0,0);

//   #ifndef USE_PPP
//     const intArray & mask=c.mask();
//     const realArray & uu = u;
//     const int *bc = &boundaryCondition(0,0);
//   #else
//     intSerialArray mask; getLocalArrayWithGhostBoundaries(c.mask(),mask);
//     realSerialArray uu; getLocalArrayWithGhostBoundaries(u,uu);
//     const int *bc = &boundaryCondition(0,0);
//   #endif
//   real *puu= getDataPointer(uu);

  real *puu = uu.getDataPointer();

  if( puu!=NULL )
  {
    int *pmask= mask.getDataPointer();
    
    const int orderOfExtrapolation = bcParameters.orderOfExtrapolation<0 ? orderOfAccuracy+1
      : bcParameters.orderOfExtrapolation;
//     bool isRectangular = c.isRectangular();
    const bool gridIsRectangular = this->isRectangular();
    const int gridType = gridIsRectangular ? 0 : 1;

    int indexRangec[3][2]; 
#define indexRange0(side,axis) indexRangec[axis][side]
    indexRange0(0,0)=c.indexRange(Start,axis1);
    indexRange0(1,0)=c.indexRange(End  ,axis1);
    indexRange0(0,1)=c.indexRange(Start,axis2);
    indexRange0(1,1)=c.indexRange(End  ,axis2);
    indexRange0(0,2)=c.indexRange(Start,axis3);
    indexRange0(1,2)=c.indexRange(End  ,axis3);
#undef indexRange0
#define indexRange(side,axis) indexRangec[axis][side]

    int pdim[6]={c.dimension(0,0),c.dimension(1,0),c.dimension(0,1),c.dimension(1,1),c.dimension(0,2),c.dimension(1,2)};  //
#define dim(side,axis) pdim[side+2*(axis)] 

  

    // Nv[axis] = bounds on the local u array with ghost boundaries: 
    Range Nv[4];
    for( int axis=0; axis<4; axis++)
      Nv[axis]=u.getLocalFullRange(axis);


    if( bcParameters.lineToAssign!=0 ) // *wdh* added 010825 to fix AMR problem 
    {
      // increase the size of the indexRange so we adjust fewer corner points 

      // printf("\n\n ************ INFO: fixBoundaryCorners only adjust ghost lines corner values > %i ***********\n\n",
      //    bcParameters.lineToAssign);
    
      for( int axis=0; axis<c.numberOfDimensions(); axis++ )
      {
	indexRange(0,axis)-=bcParameters.lineToAssign;
	indexRange(1,axis)+=bcParameters.lineToAssign;
      }
    }

    // *** here we use the optimised version ***

    int useWhereMask=0; // for now do not use the mask
    bool assignVectorSymmetryBC=false; // set to true if there are any corner symmetry BC's
    int cornerBC[27];
    for( int side1=0; side1<=2; side1++ )
      for( int side2=0; side2<=2; side2++ )
	for( int side3=0; side3<=2; side3++ )
	{
	  cornerBC[side1+3*(side2+3*(side3))]=bcParameters.getCornerBC(side1,side2,side3);
	  if( cornerBC[side1+3*(side2+3*(side3))]>=BoundaryConditionParameters::vectorSymmetryAxis1Corner &&
	      cornerBC[side1+3*(side2+3*(side3))]<=BoundaryConditionParameters::vectorSymmetryAxis3Corner )
	  {
	    assignVectorSymmetryBC=true;
	  }
	}
    
#ifdef USE_PPP
    const IntegerArray & dimension = c.dimension();
    int sidea[3]={0,0,0}; //
    int sideb[3]={2,2,2}; // 2: marks an edge
    for( int axis=0; axis<c.numberOfDimensions(); axis++ )
    {
      // adjust the indexRange to live on this processor.

      // indexRange(0,axis)=max(indexRange(0,axis),Nv[axis].getBase());
      // indexRange(1,axis)=min(indexRange(1,axis),Nv[axis].getBound());
      // *wdh* 2012/05/04 -- restrict bounds (taylor conditions use tangential points)
      indexRange(0,axis)=max(indexRange(0,axis),Nv[axis].getBase() +u.getGhostBoundaryWidth(axis));
      indexRange(1,axis)=min(indexRange(1,axis),Nv[axis].getBound()-u.getGhostBoundaryWidth(axis));
      
      // I don't think we need to change the dimension array but may be needed in the future
      dim(0,axis)=max(dim(0,axis),Nv[axis].getBase());
      dim(1,axis)=min(dim(1,axis),Nv[axis].getBound());
      
      for( int side=0; side<=1; side++ )
      {
	if( (side==0 && Nv[axis].getBase() +u.getGhostBoundaryWidth(axis) > dimension(0,axis)) || 
	    (side==1 && Nv[axis].getBound()-u.getGhostBoundaryWidth(axis) < dimension(1,axis)) )
	{
	  // ***** The face r(axis)==side is NOT on this processor *****

	  // turn off 4 corners and 4 edges adjacent to this face (or 2 corners in 2D)
	  sidea[axis]=side;
	  sideb[axis]=side;
	  for( int side1=sidea[0]; side1<=sideb[0]; side1++ )
	    for( int side2=sidea[1]; side2<=sideb[1]; side2++ )
	      for( int side3=sidea[2]; side3<=sideb[2]; side3++ )
		cornerBC[side1+3*(side2+3*(side3))]=BoundaryConditionParameters::doNothingCorner;
          
	  sidea[axis]=0; // reset
	  sideb[axis]=2;
	}
      }
    }
#endif

    int ca,cb;
    if( C0!=nullRange )
    {
      ca=C0.getBase(); cb=C0.getBound();
    }
    else
    {
      ca=u.getComponentBase(0); cb=u.getComponentBound(0);
    }
    
    bool rsxyNeeded =assignVectorSymmetryBC && !gridIsRectangular;
    // for a vectorSymmetry BC on a curvilinear grid we need the rsxy array
// #ifdef USE_PPP
//   realSerialArray rsxy;  if( rsxyNeeded ) getLocalArrayWithGhostBoundaries(c.inverseVertexDerivative(),rsxy);
// #else
//   const realSerialArray & rsxy = rsxyNeeded ? c.inverseVertexDerivative() : uu; 
// #endif
      real *prsxy = rsxyNeeded ? c.inverseVertexDerivative().getDataPointer() : puu;


      //   if( uu.elementCount()<=0 ) return;  // no points to assign
      // assert( puu!=NULL );

      // Found in fortranDeriv/assignCornersOpt.bf
      int ipar[10];
      ipar[0]=ca;
      ipar[1]=cb;
      ipar[2]=useWhereMask;
      ipar[3]=orderOfExtrapolation;
      ipar[4]=bcParameters.numberOfCornerGhostLinesToAssign;
      ipar[5]=bcParameters.cornerExtrapolationOption;
      ipar[6]=bcParameters.getVectorSymmetryCornerComponent();
      ipar[7]=gridType;

      real rpar[5];
      rpar[0]=REAL_MIN*100.;  // normEps
    
      fixBoundaryCornersOpt( c.numberOfDimensions(), 
			     Nv[0].getBase(),Nv[0].getBound(),
			     Nv[1].getBase(),Nv[1].getBound(),
			     Nv[2].getBase(),Nv[2].getBound(),
			     Nv[3].getBase(),Nv[3].getBound(),
			     Nv[0].getBase(),Nv[0].getBound(),
			     Nv[1].getBase(),Nv[1].getBound(),
			     Nv[2].getBase(),Nv[2].getBound(),
			     *puu,*pmask,*prsxy, indexRange(0,0), dim(0,0), 
			     c.isPeriodic()(0), bc[0], cornerBC[0], ipar[0], rpar[0] );
  }
  
  const int np= max(1,Communication_Manager::numberOfProcessors());
  if( np>1 )
    u.updateGhostBoundaries();  // *wdh* 080909 

  timeForFixBoundaryCorners+=getCPUOpt()-time;
  return;

// * OLD: 

// *   //     ---when two (or more) adjacent faces have boundary conditions
// *   //        we set the values on the fictitous line (or vertex)
// *   //        that is outside both faces ( points marked + below)
// *   //        We set values on all ghost points that lie outside the corner
// *   //
// *   //                + +                + +
// *   //                + +                + +
// *   //                    --------------
// *   //                    |            |
// *   //                    |            |
// *   //
// * 
// *   int side1,side2,side3,is1,is2,is3,i1,i3;
// *   
// * 
// *   Index I1=Range(indexRange(Start,axis1),indexRange(End,axis1));
// *   Index I2=Range(indexRange(Start,axis2),indexRange(End,axis2));
// *   Index I3=Range(indexRange(Start,axis3),indexRange(End,axis3));
// *   Index N =C0!=nullRange ? C0 : Range(u.getComponentBase(0),u.getComponentBound(0));   // ********* Is this ok ?? *************
// * 
// *   //         ---extrapolate edges---
// *   Index I1m,I2m,I3m;
// *   if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) )
// *   {
// *     //       ...Do the four edges parallel to i3
// *     side3=-1; 
// *     for( side1=Start; side1<=End; side1++ )
// *     {
// *       is1=1-2*side1;
// *       for( side2=Start; side2<=End; side2++ )
// *       {
// * 	is2=1-2*side2;
// * 	if( c.boundaryCondition(side1,axis1)>0 || c.boundaryCondition(side2,axis2)>0 )
// * 	{
// * 	  I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange(side2,axis2)-is2) :
// * 	    Range(indexRange(side2,axis2)-is2,c.dimension(side2,axis2)-1-is2);
// *           // We have to loop over i1 from inside to outside since later points depend on previous ones.
// * 	  for( i1=indexRange(side1,axis1); i1!=c.dimension(side1,axis1); i1-=is1 )
// * 	  {
// *             I1m=i1-is1;
// * 	    assignCorners(I1m,I2m,I3,N, is1,is2,0,I1m,I2m,I3,N, side1,side2,side3,orderOfExtrapolation,
// * 			  u,bcParameters,c.numberOfDimensions());
// * 	    // EXTRAP_SWITCH(i1-is1,i2-is2,I3,N, is1,is2,0,i1-is1,i2-is2,I3,N); 
// * 	  }
// * 	}
// *       }
// *     }
// *   }
// *  
// *   if( c.numberOfDimensions()<=2 ) return;
// * 
// *   if( !c.isPeriodic(axis1) && !c.isPeriodic(axis3) )
// *   {
// *     //       ...Do the four edges parallel to i2
// *     side2=-1;
// *     for( side1=Start; side1<=End; side1++ )
// *     {
// *       is1=1-2*side1;
// *       for( side3=Start; side3<=End; side3++ )
// *       {
// *         is3=1-2*side3;
// *         if( c.boundaryCondition(side1,axis1)>0 || c.boundaryCondition(side3,axis3)>0 )
// * 	{
// * 	   I3m= side3==Start ? Range(c.dimension(side3,axis3)+1-is3,indexRange(side3,axis3)-is3) :
// * 	    Range(indexRange(side3,axis3)-is3,c.dimension(side3,axis3)-1-is3);
// * 
// *           // We have to loop over i1 from inside to outside since later points depend on previous ones.
// * 	  for( i1=indexRange(side1,axis1); i1!=c.dimension(side1,axis1); i1-=is1 )
// * 	  {
// *             I1m=i1-is1;
// * 	    assignCorners(I1m,I2,I3m,N,is1,0,is3,I1m,I2,I3m,N, side1,side2,side3,orderOfExtrapolation,
// * 			  u,bcParameters,c.numberOfDimensions());
// *     	    // EXTRAP_SWITCH(i1-is1,I2,i3-is3,N,is1,0,is3,i1-is1,I2,i3-is3,N);
// * 	  
// * 	  }
// * 	}
// *       }
// *     }
// *   }
// *   if( !c.isPeriodic(axis2) && !c.isPeriodic(axis3) )
// *   {
// *     //       ...Do the four edges parallel to i1
// *     side1=-1;
// *     for( side2=Start; side2<=End; side2++ )
// *     {
// *       is2=1-2*side2;
// *       I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange(side2,axis2)-is2) :
// * 	Range(indexRange(side2,axis2)-is2,c.dimension(side2,axis2)-1-is2);
// *       for( side3=Start; side3<=End; side3++ )
// *       {
// *         is3=1-2*side3;
// *         if( c.boundaryCondition(side2,axis2)>0 || c.boundaryCondition(side3,axis3)>0 )
// * 	{
// *           // We have to loop over i3 from inside to outside since later points depend on previous ones.
// *           for( i3=indexRange(side3,axis3); i3!=c.dimension(side3,axis3); i3-=is3 )
// * 	  {
// * 	    I3m=i3-is3;
// *             assignCorners(I1,I2m,I3m,N, 0,is2,is3,I1,I2m,I3m,N, side1,side2,side3,orderOfExtrapolation,
// * 			  u,bcParameters,c.numberOfDimensions());
// *             // EXTRAP_SWITCH(I1,i2-is2,i3-is3,N, 0,is2,is3,I1,i2-is2,i3-is3,N);
// * 	  }
// * 	}
// *       }
// *     }
// *   }
// *   
// *   if( !c.isPeriodic(axis1) && !c.isPeriodic(axis2) && !c.isPeriodic(axis3) )
// *   {
// *     //    ...Do the points outside vertices in 3D
// *     for( side1=Start; side1<=End; side1++ )
// *     {
// *       is1=1-2*side1;
// *       I1m= side1==Start ? Range(c.dimension(side1,axis1)+1-is1,indexRange(side1,axis1)-is1) :
// * 	Range(indexRange(side1,axis1)-is1,c.dimension(side1,axis1)-1-is1);
// *       for( side2=Start; side2<=End; side2++ )
// *       {
// *         is2=1-2*side2;
// * 	I2m= side2==Start ? Range(c.dimension(side2,axis2)+1-is2,indexRange(side2,axis2)-is2) :
// * 	  Range(indexRange(side2,axis2)-is2,c.dimension(side2,axis2)-1-is2);
// *         for( side3=Start; side3<=End; side3++ )
// *         {
// *           is3=1-2*side3;
// *           if( c.boundaryCondition(side1,axis1)>0 || 
// *               c.boundaryCondition(side2,axis2)>0 || 
// *               c.boundaryCondition(side3,axis3)>0 )
// * 	  {
// *             for( i3=indexRange(side3,axis3); i3!=c.dimension(side3,axis3); i3-=is3 )
// * 	    {
// * 	      I3m=i3-is3;
// *               assignCorners(I1m,I2m,I3m,N, is1,is2,is3,I1m,I2m,I3m,N, side1,side2,side3,
// * 			    orderOfExtrapolation,u,bcParameters,c.numberOfDimensions());
// * 	      // EXTRAP_SWITCH(i1-is1,i2-is2,i3-is3,N, is1,is2,is3,i1-is1,i2-is2,i3-is3,N);
// * 	    }
// * 	    
// * 	  }
// * 	}
// *       }
// *     }
// *   }
// *   timeForFixBoundaryCorners+=getCPU()-time;

}

#undef UX1
#undef UX2
#undef UX3
#undef UX4
#undef UX5
#undef UX6
#undef EXTRAP_SWITCH
#undef indexRange

