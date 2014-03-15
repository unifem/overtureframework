#include "HyperbolicMapping.h"
#include "display.h"
#include "arrayGetIndex.h"
#include "MatchingCurve.h"

// Declare and define base and bounds, perform loop
#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  int I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

// Perform loop
#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase();\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

static inline 
double
tetVolume6(real *p1, real*p2, real *p3, real *p4 )
{
  // Rteurn 6 times the volume of the tetrahedra
  // (p2-p1)x(p3-p1) points in the direction of p4 ( p1,p2,p3 are counter clockwise viewed from p4 )
  // 6 vol = (p4-p1) . ( (p2-p1)x(p3-p1) )
  return  ( (p4[0]-p1[0])*( (p2[1]-p1[1])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[1]-p1[1]) ) -
	    (p4[1]-p1[1])*( (p2[0]-p1[0])*(p3[2]-p1[2]) - (p2[2]-p1[2])*(p3[0]-p1[0]) ) +
	    (p4[2]-p1[2])*( (p2[0]-p1[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]) ) ) ;
	  
}


static inline 
real
hexVolume( real *v000, real *v100, real *v010, real *v110, real *v001, real *v101, 
           real *v011, real *v111 )
// =====================================================================================================
// Return true if the hex defined by the vertices v000,v100,... has any tetrahedra that are negative.
// =====================================================================================================
{
  return (tetVolume6(v000,v100,v010, v001)+
	  tetVolume6(v110,v010,v100, v111)+
	  tetVolume6(v101,v001,v111, v100)+
	  tetVolume6(v011,v111,v001, v010)+
	  tetVolume6(v100,v010,v001, v111));
}


// -- use gridStatistics in Mapping now 
// void 
// printGridStatistics(DataPointMapping & dpm, FILE *file=stdout )
// // =================================================================================================
// // /Description:
// //   Print statistics about the grid
// // =================================================================================================
// {


//   // **** NOTE * NOTE: there is a parallel version of this in stretchUpdate.C -- FIX ME ********************************



//   real volMin=REAL_MAX,volAve=0.,volMax=0.;
//   real dsMin=REAL_MAX,dsAve=0.,dsMax=0.;
//   int numberOfNegativeVolumes=0;
//   int numberOfGridPoints=0;
  
//   const int domainDimension = dpm.getDomainDimension();
//   const int rangeDimension = dpm.getRangeDimension();

//   const realArray & x = dpm.getDataPoints();
//   const IntegerArray & gid = dpm.getGridIndexRange();

//   Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
//   ::getIndex(gid,I1,I2,I3); 

//   int axis;
//   for( axis=0; axis<domainDimension; axis++ )
//     Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);  // only compute at cell centres.

//   const real orientation = dpm.getSignForJacobian();

//   int i1,i2,i3;
//   if( domainDimension==2 && rangeDimension==2 )
//   {
//     FOR_3D(i1,i2,i3,I1,I2,I3)
//     {
//       // area of a polygon = (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i
//       real vol = ( x(i1  ,i2  ,i3,0)*x(i1+1,i2  ,i3,1)-x(i1+1,i2  ,i3,0)*x(i1  ,i2  ,i3,1) +  // (i1,i2)
// 		   x(i1+1,i2  ,i3,0)*x(i1+1,i2+1,i3,1)-x(i1+1,i2+1,i3,0)*x(i1+1,i2  ,i3,1) + 
// 		   x(i1+1,i2+1,i3,0)*x(i1  ,i2+1,i3,1)-x(i1  ,i2+1,i3,0)*x(i1+1,i2+1,i3,1) + 
// 		   x(i1  ,i2+1,i3,0)*x(i1  ,i2  ,i3,1)-x(i1  ,i2  ,i3,0)*x(i1  ,i2+1,i3,1) );
    
//       vol*=.5*orientation;
//       volMin=min(volMin,vol);
//       volMax=max(volMax,vol);
//       volAve+=vol;
//       numberOfGridPoints++;
//       if( vol<=0. ) numberOfNegativeVolumes++;
      
//     }
//     volAve/=max(1,numberOfGridPoints);
    
//     // compute first grid line spacing
//     ::getIndex(gid,I1,I2,I3);  // do all points
//     i3=gid(0,2);
//     i2=gid(0,1);
//     int numDs=0;
//     for( i1=I1Base; i1<=I1Bound; i1++ )
//     {
//       real ds= sqrt( SQR(x(i1,i2+1,i3,0)-x(i1,i2,i3,0))+
// 		     SQR(x(i1,i2+1,i3,1)-x(i1,i2,i3,1)) );
//       dsMin=min(dsMin,ds);
//       dsMax=max(dsMax,ds);
//       dsAve+=ds;
//       numDs++;

//     }
//     dsAve/=max(1,numDs);

//   }
//   else if( domainDimension==2 && rangeDimension==3 )
//   {
//     printf("printGridStatistics: not implemented yet for surface grids.\n");
//   }
//   else if( domainDimension==3 )
//   {
//     // ************ 3D ***********************

//     real v[2][2][2][3];
//     FOR_3D(i1,i2,i3,I1,I2,I3)
//     {
//       for( int axis=0; axis<3; axis++ )
//       {
// 	v[0][0][0][axis]=x(i1  ,i2  ,i3  ,axis);
// 	v[1][0][0][axis]=x(i1+1,i2  ,i3  ,axis);
// 	v[0][1][0][axis]=x(i1  ,i2+1,i3  ,axis);
// 	v[1][1][0][axis]=x(i1+1,i2+1,i3  ,axis);
// 	v[0][0][1][axis]=x(i1  ,i2  ,i3+1,axis);
// 	v[1][0][1][axis]=x(i1+1,i2  ,i3+1,axis);
// 	v[0][1][1][axis]=x(i1  ,i2+1,i3+1,axis);
// 	v[1][1][1][axis]=x(i1+1,i2+1,i3+1,axis);
//       }

//       real vol=hexVolume(v[0][0][0],v[1][0][0],v[0][1][0],v[1][1][0],
//                          v[0][0][1],v[1][0][1],v[0][1][1],v[1][1][1])*orientation;
      
//       volMin=min(volMin,vol);
//       volMax=max(volMax,vol);
//       volAve+=vol;
//       numberOfGridPoints++;
//       if( vol<=0. ) numberOfNegativeVolumes++;
      
//     }
//     volAve/=max(1,numberOfGridPoints);
    
//     // compute first grid line spacing
//     ::getIndex(gid,I1,I2,I3);  // do all points
//     i3=gid(0,2);
//     int numDs=0;
//     for( i2=I2Base; i2<=I2Bound; i2++ ) 
//     {
//       for( i1=I1Base; i1<=I1Bound; i1++ )
//       {
// 	real ds= sqrt( SQR(x(i1,i2,i3+1,0)-x(i1,i2,i3,0))+
//                        SQR(x(i1,i2,i3+1,1)-x(i1,i2,i3,1))+
//                        SQR(x(i1,i2,i3+1,2)-x(i1,i2,i3,2)) );
// 	dsMin=min(dsMin,ds);
// 	dsMax=max(dsMax,ds);
// 	dsAve+=ds;
// 	numDs++;

//       }
//     }
//     dsAve/=max(1,numDs);
//   }

//   fprintf(file,
//           " -----------------------------------------------------------------------\n"
//           "         Grid Statistics name=%s. \n"
// 	  " grid lines  : [%i:%i,%i:%i,%i:%i], total points = %i\n"
// 	  " cell volumes: [%8.2e,%8.2e,%8.2e] [min,ave,max] \n"
// 	  " grid spacing: [%8.2e,%8.2e,%8.2e] [min,ave,max] r3=0 (first grid line in marching direction) \n"
//           " number of negative volumes = %i \n"
//           " -----------------------------------------------------------------------\n"
//  	  ,(const char*)dpm.getName(Mapping::mappingName),
//           gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
//           numberOfGridPoints,
// 	  volMin,volAve,volMax,
// 	  dsMin,dsAve,dsMax,numberOfNegativeVolumes
//     );


// }



int HyperbolicMapping::
getNormalAndSurfaceArea(const realArray & x, 
			const int & firstStep, 
			realArray & normal, 
			realArray & s,
			realArray & xr, 
			realArray & xrr,
                        const real & dSign,
                        realArray & normXr,
                        realArray & normXs,
                        realArray & ss,
                        const int & marchingDirection,
			int stepNumber /* =0 */ )
// ================================================================================================
/// \param Access: protected.
/// \details 
///     Determine the normal (i.e. the direction to march), surface area and partial derivatives at each point.
/// \param x (input) : current position of the front.  
/// \param firstStep (input) : true if this is the first step
/// \param normal (output) : normals
/// \param s (output) :  vertex centred surface areas (or arclengths in 2D). The is an average of the areas of the cells that
///       neighbour a vertex.
/// \param xr (input/output) : first order centred differences in tangential directions. NOTE: for the surface
///     grid generator this array should hold the normal to the surface in xr(I1,I2,I3,.,1)
/// \param xrr (output) : second-order centred differences in tangential directions (un-mixed only)
/// \param normXr, normXs: norms of xr and xs at each point on the front.
/// \param ss : smoothed surface areas (or arclengths in 2D) (node centred).   
/// \param stepNumber (input) : current step number
/// \param return values:  0 =success; 1=grid spacing was less than minimumGridSpacing.
/// 
// ================================================================================================
{
// @PD realArray4[x,xr,xr0,xrr0,normalCC,norm,normal,s,ss,normXr] Range[I1,I2,xAxes,Ig1,Ig2]
  if( debug & 1 )
    fprintf(debugFile,"\n>>>>>Entering getNormalAndSurfaceArea\n");
    

  real time0=getCPU();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int is[3] = {0,0,0}, &is1=is[0], &is2=is[1], &is3=is[2];
  Range xAxes(0,rangeDimension-1);
 
  ::getIndex(gridIndexRange,I1,I2,I3,1);  // include ghost points on left edge only:
  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
    Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);

  const int i3=x.getBase(2);

  // **** first compute a face centered normal ****
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    is[axis]=1;

    const realArray & xr1 = xr(I1,I2,0,xAxes,axis);  // do this for gcc
    realArray & xr0 = (realArray&)xr1;
    
    if( domainDimension==2 )
      xr0=.5*(x(I1+is1,I2+is2,i3,xAxes)-x(I1,I2,i3,xAxes));                     // @PANS
    else
    {
      xr0=.25*(x(I1+is1,I2+is2,i3,xAxes)-x(I1    ,I2    ,i3,xAxes)+  // @PANS
	       x(I1+1  ,I2+1  ,i3,xAxes)-x(I1+is2,I2+is1,i3,xAxes)); 
    }
    
    is[axis]=0;
  }

  // Compute normalCC : normal at cell centre. We save this for later when we compute the cell volume.
  //   normalCC = xr X xs
  if( domainDimension==2 && !surfaceGrid )
  {
    normalCC(I1,I2,0,0)= xr(I1,I2,0,1); // @PANS
    normalCC(I1,I2,0,1)=-xr(I1,I2,0,0); // @PANS
  }
  else if( surfaceGrid )
  {
    // note: for surface grids xr(I1,I2,0,*,1) is defined on input as the node-centred surface-normal
    // first get a cell centred surface normal -- I don't think we need to normalize this
    for( axis=0; axis<rangeDimension; axis++ )
      normal(I1,I2,0,axis)=(xr(I1,I2,0,axis,1)+xr(I1+1,I2,0,axis,1));  // uses ghost value of xr!

    normalCC(I1,I2,0,0)=(xr(I1,I2,0,1,0)*normal(I1,I2,0,2)-xr(I1,I2,0,2,0)*normal(I1,I2,0,1)), 
    normalCC(I1,I2,0,1)=(xr(I1,I2,0,2,0)*normal(I1,I2,0,0)-xr(I1,I2,0,0,0)*normal(I1,I2,0,2)),
    normalCC(I1,I2,0,2)=(xr(I1,I2,0,0,0)*normal(I1,I2,0,1)-xr(I1,I2,0,1,0)*normal(I1,I2,0,0));
  }
  else
  {
    normalCC(I1,I2,0,0)=(xr(I1,I2,0,1,0)*xr(I1,I2,0,2,1)-xr(I1,I2,0,2,0)*xr(I1,I2,0,1,1)), 
    normalCC(I1,I2,0,1)=(xr(I1,I2,0,2,0)*xr(I1,I2,0,0,1)-xr(I1,I2,0,0,0)*xr(I1,I2,0,2,1)),
    normalCC(I1,I2,0,2)=(xr(I1,I2,0,0,0)*xr(I1,I2,0,1,1)-xr(I1,I2,0,1,0)*xr(I1,I2,0,0,1));

//     normalCC(I1,I2,0,0)=(xr(I1,I2,0,1)*xr(I1,I2,0,5)-xr(I1,I2,0,2)*xr(I1,I2,0,4)); // PANS
//     normalCC(I1,I2,0,1)=(xr(I1,I2,0,2)*xr(I1,I2,0,3)-xr(I1,I2,0,0)*xr(I1,I2,0,5)); // PANS
//     normalCC(I1,I2,0,2)=(xr(I1,I2,0,0)*xr(I1,I2,0,4)-xr(I1,I2,0,1)*xr(I1,I2,0,3)); // PANS
    
  }
  realArray norm(I1,I2);
  if( rangeDimension==2 )
    norm=max(REAL_MIN,SQRT(SQR(normalCC(I1,I2,0,0))+SQR(normalCC(I1,I2,0,1))));  // @PANS
  else
    norm=max(REAL_MIN,SQRT(SQR(normalCC(I1,I2,0,0))+SQR(normalCC(I1,I2,0,1))+SQR(normalCC(I1,I2,0,2)))); // @PANS

  s(I1,I2)=norm;

  real sMin=min(s(I1,I2));

  norm=1./norm;
  for( axis=0; axis<rangeDimension; axis++ )
    normal(I1,I2,0,axis)=normalCC(I1,I2,0,axis)*norm; // @PANS

  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    // for a surface grid we can have different BC's depending on the marching direction
    const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;
    for( int side=Start; side<=End; side++ )
    {
      // On singular axes the normal will point in the opposite direction at a ghost line.
      if( boundaryCondition(side,direction)==singularAxis )
      {
	// note: adjust the ghost line on the left, but the boundary on the right.
	getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,1-side,1);
	normal(Ig1,Ig2)*=-1.;      // @PANS
      }
      else if( (bool)getIsPeriodic(axis) )
      { // apply periodicity before averaging below *wdh* 011021
        int ghostLine=1;
	getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,ghostLine,1);
        I1=Ig1;
	I2=Ig2;
        if( side==1 ) Igv[axis]-=1;  // cell centered
        // ****NOTE: apply periodicity to a cell-centred quantity
        if( side==0 )
  	  Iv[axis]=gridIndexRange(1,axis)-ghostLine;
        else
  	  Iv[axis]=gridIndexRange(0,axis)+ghostLine-1;     // -1 since cell centred.
          
        normal(Ig1,Ig2,0,xAxes)=normal(I1,I2,0,xAxes);
	s(Ig1,Ig2)=s(I1,I2);
      }
      
    }
  }


  if( Mapping::debug & 2 || Mapping::debug & 4 )
  {
    fprintf(debugFile,"***getNormalAndSurfaceArea: gridIndexRange=[%i,%i] I1=[%i,%i]\n",
           gridIndexRange(0,0),gridIndexRange(1,0),I1.getBase(),I1.getBound());
    Range all;
    ::display(x(all,all,i3,xAxes),sPrintF("\n*** getNormalAndSurfaceArea: x at step %i",
             stepNumber),debugFile,"%8.1e ");
    ::display(normalCC,sPrintF("\n*** getNormalAndSurfaceArea: normalCC (i+1/2) at step %i",
             stepNumber),debugFile,"%8.1e ");
    if( surfaceGrid )
      ::display(normal,sPrintF("\n*** getNormalAndSurfaceArea: Normal to surface: CC normal(i+1/2) at step %i",
               stepNumber),debugFile,"%8.1e ");
    else
      ::display(normal,sPrintF("\n*** getNormalAndSurfaceArea: marching direction: CC normal(i+1/2) at step %i",
               stepNumber),debugFile,"%8.1e ");
    ::display(s,sPrintF("\n*** getNormalAndSurfaceArea: s before averaging step %i",
             stepNumber),debugFile,"%8.2e ");
    if( surfaceGrid )
       ::display(xr(all,all,0,xAxes,1),sPrintF("\n*** getNormalAndSurfaceArea: normal to surface at i at step %i",
                 stepNumber),debugFile,"%8.1e ");
  }
  


  // ::display(normal,"normal");


  ::getIndex(gridIndexRange,I1,I2,I3);
  if( domainDimension==2 )
  {
    // no need to multiply by .5 since we normalize anyway
    normal(I1,I2,0,xAxes)=(normal(I1-1,I2,0,xAxes)+normal(I1,I2,0,xAxes));            // @PANS
    s(I1,I2) = .5*(s(I1-1,I2)+s(I1,I2));                                                 // @PANS
  }
  else  
  {
    if( false )
    {
      // average in two steps so the normal at a corner is better (see cube.cmd)
      realArray n(I1,I2,1,xAxes);
      n=normal(I1-1,I2  ,0,xAxes)+normal(I1,I2  ,0,xAxes);
      norm(I1,I2) = 1./max(REAL_MIN,SQRT(SQR(n(I1,I2,0,0))+SQR(n(I1,I2,0,1))+SQR(n(I1,I2,0,2))));
      for( axis=0; axis<rangeDimension; axis++ )
	n(I1,I2,0,axis) *= norm(I1,I2,0,0);        
      
      normal(I1,I2,0,xAxes)=normal(I1-1,I2-1,0,xAxes)+normal(I1,I2-1,0,xAxes);
      norm(I1,I2) = 1./max(REAL_MIN,SQRT(SQR(normal(I1,I2,0,0))+SQR(normal(I1,I2,0,1))+SQR(normal(I1,I2,0,2)))); 
      
      for( axis=0; axis<rangeDimension; axis++ )
	normal(I1,I2,0,axis) *= norm(I1,I2,0,0);       

      normal(I1,I2,0,xAxes)+=n(I1,I2,0,xAxes);
    }
    else
    {
      // no need to multiply by .25 since we normalize anyway
      normal(I1,I2,0,xAxes)=(normal(I1-1,I2  ,0,xAxes)+normal(I1,I2  ,0,xAxes)+  // @PANS
			     normal(I1-1,I2-1,0,xAxes)+normal(I1,I2-1,0,xAxes));
    }

    s(I1,I2) = .25*(s(I1-1,I2)+s(I1,I2)+s(I1-1,I2-1)+s(I1,I2-1));                  // @PANS
  }
  if( rangeDimension==2 )
    norm(I1,I2) = 1./max(REAL_MIN,SQRT(SQR(normal(I1,I2,0,0))+SQR(normal(I1,I2,0,1))));  // @PANS
  else  
    norm(I1,I2) = 1./max(REAL_MIN,SQRT(SQR(normal(I1,I2,0,0))+SQR(normal(I1,I2,0,1))+SQR(normal(I1,I2,0,2)))); // @PANS

  for( axis=0; axis<rangeDimension; axis++ )
    normal(I1,I2,0,axis) *= norm(I1,I2,0,0);        // @PANS 

  if( Mapping::debug & 4 )
  {
    ::display(normal,sPrintF("\n*** getNormalAndSurfaceArea: marching direction: VertexCentred (normal) at step %i",
			     stepNumber),debugFile,"%9.2e ");
  }
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    is[axis]=1;
    const realArray & xr1 =xr(I1,I2,0,xAxes,axis); 
    realArray & xr0 =(realArray&)xr1;
    const realArray & xrr1 =xrr(I1,I2,0,xAxes,axis);
    realArray & xrr0 =(realArray&)xrr1;

    xr0=.5*(x(I1+is1,I2+is2,i3,xAxes)-x(I1-is1,I2-is2,i3,xAxes));   // @PANS
    xrr0=(x(I1+is1,I2+is2,i3,xAxes)-2.*x(I1      ,I2      ,i3,xAxes)+   // @PANS
				x(I1-is1,I2-is2,i3,xAxes));
    is[axis]=0;
  }


  // ***** Boundary conditions *********
  int extra=1;
  
  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    // for a surface grid we can have different BC's depending on the marching direction
    const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;

    for( int side=Start; side<=End; side++ )
    {
      const int bcValue=boundaryCondition(side,direction);

      // *wdh* 010426  getBoundaryIndex(gridIndexRange,side,axis,Ib1,Ib2,Ib3); 
      getBoundaryIndex(gridIndexRange,side,axis,Ib1,Ib2,Ib3,extra);

      // if specified normal BC
      if( bcValue==trailingEdge )
      {

        if( (firstStep || true ) && side==Start )
	{
          // compute the trailing edge direction -- average the unit tangent vectors.
          trailingEdgeDirection.redim(1,1,1,rangeDimension);
	  
	  getBoundaryIndex(gridIndexRange,Start,axis,Ib1,Ib2,Ib3);
	  // ::display(xr(Ib1,Ib2,0,xAxes,0),"xr(Ib1,Ib2,0,xAxes,0)");
	  
          realArray tangent(1,1,1,rangeDimension);
	  tangent(0,0,0,xAxes)=x(Ib1+1,Ib2,0,xAxes)-x(Ib1,Ib2,0,xAxes);

	  // trailingEdgeDirection(0,0,0,xAxes)=-normalize( xr(Ib1,Ib2,0,xAxes,0) ); // note minus
	  trailingEdgeDirection(0,0,0,xAxes)=-normalize( tangent ); // note minus

	  getBoundaryIndex(gridIndexRange,End,axis,Ib1,Ib2,Ib3);
	  // ::display(xr(Ib1-1,Ib2,0,xAxes,0),"xr(Ib1-1,Ib2,0,xAxes,0)");

	  tangent(0,0,0,xAxes)=x(Ib1,Ib2,0,xAxes)-x(Ib1-1,Ib2,0,xAxes);
	  // trailingEdgeDirection(0,0,0,xAxes)+=normalize( xr(Ib1-1,Ib2,0,xAxes,0) );  
	  trailingEdgeDirection(0,0,0,xAxes)+=normalize( tangent );

          trailingEdgeDirection(0,0,0,xAxes)=normalize(trailingEdgeDirection(0,0,0,xAxes));

	}
	
        if( debug & 2 )
          printf("**** set normal to trailing edge direction = (%8.2e,%8.2e)\n",
                   trailingEdgeDirection(0,0,0,0),trailingEdgeDirection(0,0,0,1));

        normal(Ib1,Ib2,0,xAxes)=trailingEdgeDirection(0,0,0,xAxes);
      }

      // *** Blend normals to be consistent with the tangent to the specified boundary. ****
      // do NOT blend normals for the following:
      if( ((bool)getIsPeriodic(axis) &&  bcValue!=trailingEdge)    ||
          bcValue==freeFloating ||
          bcValue==singularAxis ||
          bcValue==floatCollapsed )
        continue;

      realArray normInverse(Ib1,Ib2);
      int dir;
      if( bcValue==matchToMapping || bcValue==matchToABoundaryCurve )
      {
        // project the normal to be parallel to the boundary
	// Given n = normal to the boundary condition mapping,
	// subtract off the component of `normal' in the direction of n
	// finally renormalize


        // **** Here we assume that applyBoundaryCondition's has been called so that the points have
        //      been projected onto the BC Mapping's.  ******

	MappingProjectionParameters & mpParams = boundaryConditionMappingProjectionParameters[side][direction];
	realArray & n = mpParams.getRealArray(MappingProjectionParameters::normal);

        const int numberOfPoints=Ib1.getLength()*Ib2.getLength();
        if( n.getLength(0)!=numberOfPoints )
          continue;

	realArray nDot(Ib1,Ib2);

        projectNormalsToMatchCurve(*boundaryConditionMapping[side][direction], mpParams,Ib1,Ib2,normal,nDot );
	
      }
      else if( bcValue==fixXfloatYZ ||
               bcValue==fixYfloatXZ ||
               bcValue==fixZfloatXY  )
      {
        int fixDir = bcValue==fixXfloatYZ ? 0 : bcValue==fixYfloatXZ ? 1 : 2;
        normal(Ib1,Ib2,0,fixDir)=0.;
      }
      else if( bcValue==floatXfixYZ ||
               bcValue==floatYfixXZ ||
               bcValue==floatZfixXY )
      {
        int floatDir = bcValue==floatXfixYZ ? 0 : bcValue==floatYfixXZ ? 1 : 2;
        for( dir=0; dir<rangeDimension; dir++ )
	{
	  if( dir!=floatDir )
            normal(Ib1,Ib2,0,dir)=0.;
	}
      }
   

      // re-normalize the normal ******** not always necessary ******
      if( rangeDimension==2 )
	normInverse=1./max(REAL_MIN,SQRT(SQR(normal(Ib1,Ib2,0,0))+SQR(normal(Ib1,Ib2,0,1))));
      else
	normInverse=1./max(REAL_MIN,SQRT(SQR(normal(Ib1,Ib2,0,0))+
					 SQR(normal(Ib1,Ib2,0,1))+
					 SQR(normal(Ib1,Ib2,0,2))));
      for( dir=0; dir<rangeDimension; dir++ )
	normal(Ib1,Ib2,0,dir)*=normInverse;

      // ::display(normal(Ib1,Ib2,0,xAxes),"getNormalAndSurfaceArea boundary normal(Ib1,Ib2,0,xAxes)");
      if( debug & 2 )
      {
	int i1=Ib1.getBase(), i2=Ib2.getBase();
        fprintf(debugFile," getNormalAndSurfaceArea: stepNumber=%i, marchingDirection=%i, direction=%i\n",
		stepNumber, marchingDirection,direction);
        fprintf(debugFile," getNormalAndSurfaceArea: corrected boundary marching vector=(%7.1e,%7.1e,%7.1e)\n",
		normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
      }

      // blend nearby normals
      const int numberToBlend=min(numberOfLinesForNormalBlend[side][axis],Iv[axis].getLength()-1);
      blendNormals(normal, xr, numberToBlend,Ibv,axis,side);

    }
  }



  for( axis=0; axis<domainDimension-1; axis++ )
  {
    // for a surface grid we can have different BC's depending on the marching direction
    const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;

    for( int side=Start; side<=End; side++ )
    {
      const int bcValue=boundaryCondition(side,direction);
      if( bcValue==trailingEdge ) // tip of airfoil
      {
        if( debug & 2 )
	{
	  is1=1-2*side;
	  getBoundaryIndex(gridIndexRange,side,axis,Ib1,Ib2,Ib3);
	  // xr(Ib1,Ib2,0,xAxes,0)=xr(Ib1+is1,Ib2,0,xAxes,0); // **** this is not right for periodic ***
	  int i1=Ib1.getBase(), i2=Ib2.getBase();
	  printf("trailing edge: xm=(%7.2e,%7.2e) xp=(%7.2e,%7.2e) xr=(%7.2e,%7.2e) \n",
		 x(i1+1,i2,i3,0),x(i1+1,i2,i3,1),x(i1-1,i2,i3,0),x(i1-1,i2,i3,1),xr(i1,i2,0,0,0),xr(i1,i2,0,1,0));
	  is1=0;
	}
      }
      else if( bcValue==singularAxis )
      {
	// adjust tangential derivative on the singular boundary which is zero. Average the
        //  derivatives from the nearby lines, using the opposite sign for the point
        // on the wrong side of the singularity
	getBoundaryIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3);
        const real sgn=.25*(1-2*side);
        if( axis==0 )
   	  xr(Ig1,Ig2,0,xAxes,1)=sgn*(x(Ig1+1,Ig2+1,i3,xAxes)-x(Ig1+1,Ig2-1,i3,xAxes)-  // note - sign
	                             x(Ig1-1,Ig2+1,i3,xAxes)+x(Ig1-1,Ig2-1,i3,xAxes));
        else
   	  xr(Ig1,Ig2,0,xAxes,0)=sgn*(x(Ig1+1,Ig2+1,i3,xAxes)-x(Ig1-1,Ig2+1,i3,xAxes)-
	                             x(Ig1+1,Ig2-1,i3,xAxes)+x(Ig1-1,Ig2-1,i3,xAxes));
      }
    }
  }


  // adjust normals at **interior** matching lines
  const int numberOfMatchingCurves=matchingCurves.size();
  if( numberOfMatchingCurves>0 )
  {
    for( int i=0; i<numberOfMatchingCurves; i++ )
    {
      MatchingCurve & match = matchingCurves[i];
      if( match.curveDirection==marchingDirection || 
          match.curveDirection==0 ) // *wdh* 090718
      {
	const int gridLine=match.gridLine+boundaryOffset[0][0];  // this should have been computed when the start curve was evaluated
	assert( gridLine>=0 );
	
	assert( match.curve!=NULL && match.projectionParameters!=NULL );
       
	Mapping & matchingMap = *match.curve;
       
	MappingProjectionParameters & mpParams = *match.projectionParameters;

	Ib1=gridLine;
	Ib2=x.getBase(1);

// 	debug=7;
//         if( debugFile==NULL )
//   	  debugFile = fopen("hype.debug","w" );

//         fprintf(debugFile,"getNormal.. projectNormalsToMatchCurve.. gridLine=%i\n",gridLine);

        // printf("****getNormalAndSurfaceArea.. projectNormalsToMatchCurve.. gridLine=%i\n",gridLine);
        realArray nDot(Ib1,Ib2);
        projectNormalsToMatchCurve(matchingMap, mpParams,Ib1,Ib2,normal,nDot );

        int numberToBlend=match.numberOfLinesForNormalBlend;
        // we should not blend beyond the number of points we have:
        int axis=0;
	// numberToBlend=min(numberToBlend,Iv[axis].getBound()-gridLine,gridLine-Iv[axis].getBase());
        int side=-1;

//         printf("getNormal.. blend interior matching curves: Iv=[%i,%i] numberToBlend=%i\n",
//                         Iv[axis].getBase(),Iv[axis].getBound(),numberToBlend);
//         fprintf(debugFile,"getNormal.. blend interior matching curves: numberToBlend=%i\n",numberToBlend);

        // printf("*****.. blend interior matching curves: numberToBlend=%i.\n",numberToBlend);
        blendNormals(normal, xr, numberToBlend,Ibv,axis,side);
      }
    }
  }
  
  for( axis=0; axis<domainDimension-1; axis++ ) // **** this may not be needed since normal on End may not be used??
  {
    // apply periodic BC again to normals if blended  *** could check if normals were blended ***
    // Here the normal is vertex centred.
    const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;
    for( int side=Start; side<=End; side++ )
    {
      if( (bool)getIsPeriodic(axis) )
      { 
        int ghostLine=1;
	getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,ghostLine,1);
        I1=Ig1;
	I2=Ig2;
        if( side==0 )
  	  Iv[axis]=gridIndexRange(1,axis)-ghostLine;
        else
	{
	  Igv[axis]=gridIndexRange(1,axis)+ghostLine-1;  // assign boundary on rhs (could use indexRange??)
  	  Iv[axis]=gridIndexRange(0,axis)+ghostLine-1;     
	}
        normal(Ig1,Ig2,0,xAxes)=normal(I1,I2,0,xAxes);
      }
      
    }
  }
 ::getIndex(gridIndexRange,I1,I2,I3);
 

/* ----
  if( FALSE && domainDimension==2 && rangeDimension==3 && 
      (arcLengthWeight!=0. || curvatureWeight!=0. || normalCurvatureWeight!=0. ) )
  {
    // Here we redefine the smoothing term xrr to be weighted
    Index J1,J2,J3;
    ::getIndex(dimension,J1,J2,J3);
    gridDensityWeight.redim(J1,J2,domainDimension-1);

    gridDensityWeight(I1,I2)=1.;

	
    real sMax = max(REAL_MIN,max(s(I1,I2)));
    if( arcLengthWeight!=0. )
      gridDensityWeight(I1,I2)+=(arcLengthWeight/sMax)*s(I1,I2);
    if( curvatureWeight!=0. )
    {
      // compute the tangential component of the curvature xrr*tangent
      realArray curvature(I1,I2);
      if( domainDimension==2 )
      {
	curvature(I1,I2)=fabs(xrr(I1,I2,0,axis1,0)*xr(I1,I2,0,axis1,0)+
			      xrr(I1,I2,0,axis2,0)*xr(I1,I2,0,axis2,0)+
			      xrr(I1,I2,0,axis3,0)*xr(I1,I2,0,axis3,0));
      }
      const real cMax=max(REAL_MIN,max(curvature(I1,I2)));
      gridDensityWeight(I1,I2)+=(curvatureWeight/cMax)*curvature(I1,I2);
    }
    if( normalCurvatureWeight!=0. && rangeDimension==3 )
    {
      // compute the normal component of the curvature xrr*(normal to surface)
      realArray curvature(I1,I2);
      if( domainDimension==2 )
      {
	curvature(I1,I2)=fabs(xrr(I1,I2,0,axis1,0)*xrr(I1,I2,0,axis1,1)+
			      xrr(I1,I2,0,axis2,0)*xrr(I1,I2,0,axis2,1)+
			      xrr(I1,I2,0,axis3,0)*xrr(I1,I2,0,axis3,1));
      }
      const real cMax=max(REAL_MIN,max(curvature(I1,I2)));
      gridDensityWeight(I1,I2)+=(curvatureWeight/cMax)*curvature(I1,I2);
    }
    gridDensityWeight(I1,I2)=min(gridDensityWeight(I1,I2))/gridDensityWeight(I1,I2);
    jacobiSmooth( gridDensityWeight,2 );
 
    // ********************* fix centering ************************
    is1=1;
    is2=0;
    for( axis=0; axis<domainDimension-1; axis++ )
      for( int dir=0; dir<rangeDimension; dir++ )
        xrr(I1,I2,0,dir,axis)*=gridDensityWeight(I1,I2);
 
  }
---- */

  normal*=dSign;

//   real sTotal=sum(s(I1,I2));
//   real sAve=sTotal/(I1.getLength()*I2.getLength());

  normXr.redim(I1,I2);
  if( rangeDimension==2 )
    normXr= SQRT(SQR(xr(I1,I2,0,0))+SQR(xr(I1,I2,0,1))); // @PANS
  else
    normXr=SQRT(SQR(xr(I1,I2,0,0,0))+SQR(xr(I1,I2,0,1,0))+SQR(xr(I1,I2,0,2,0)));

  normXr=max(REAL_MIN,normXr);
  
  if( rangeDimension==2 )
    normXs= normXr;
  else
  {
    normXs.redim(I1,I2);
    normXs=SQRT(SQR(xr(I1,I2,0,0,1))+SQR(xr(I1,I2,0,1,1))+SQR(xr(I1,I2,0,2,1)));
    normXs=max(REAL_MIN,normXs);
  }

  ss(I1,I2)=s(I1,I2);
  jacobiSmooth( ss,numberOfVolumeSmoothingIterations );


//   if( surfaceGrid && rangeDimension==3 )
//   {
//     for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
//       for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
//       {
// 	printf("getNormalAnd.. i1=%i i2=%i, normal=(%8.2e,%8.2e,%8.2e)\n",i1,i2,
//              normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
//       }
//   }
  

  if( Mapping::debug & 4 )
  {
    ::display(s,sPrintF("\n*** getNormalAndSurfaceArea: Here is s at step %i",stepNumber),debugFile);
    ::display(ss,sPrintF("\n*** getNormalAndSurfaceArea: Here is ss at step %i",stepNumber),debugFile);
    ::display(normal,sPrintF("\n*** getNormalAndSurfaceArea: normal at step %i",stepNumber),debugFile);
  }
  if( Mapping::debug & 4 )
  {
    ::display(xr,sPrintF("\n*** getNormalAndSurfaceArea: Here is xr at step %i",stepNumber),debugFile);
    ::display(xrr,sPrintF("\n*** getNormalAndSurfaceArea: Here is xrr at step %i",stepNumber),debugFile);
  }


  timing[timeForNormalAndSurfaceArea]+=getCPU()-time0;

  if( debug & 1 )
    fprintf(debugFile,"<<<<<Leaving getNormalAndSurfaceArea\n");


  if( sMin<minimumGridSpacing )
    return 1;
  else  
    return 0;
}


//! Blend normals 
/*!
  /param I1,I2,I3 : blend normals next to these points.
  /axis : blend lines along this axis.
  /side : 0,1 -- blend the boundary (left or right), -1 -- blend an interior line
  */
int HyperbolicMapping::
blendNormals(realArray & normal, 
             realArray & xr,
	     int numberToBlend,
	     Index Iv[3],
	     int axis, int side)
{
  // printf("****** blend normals ***** debug=%i\n",debug);
  
  // blend nearby normals
  int extra=1;
  Range xAxes(0,rangeDimension-1);
  Index &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

  // const int numberToBlend=min(numberOfLinesForNormalBlend[side][axis],Iv[axis].getLength()-1);

  if( debug & 2 )
  {
    int i1=I1.getBase(), i2=I2.getBase();
    fprintf(debugFile," blendNormals: (i1,i2)=(%i,%i) INPUT bndry marching vector=(%7.1e,%7.1e,%7.1e)\n",
	    i1,i2,normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
  }

  // for an interior line (side<0) we blend in both directions.
  int lineStart = side>=0 ? 1 : -numberToBlend;
  for( int line=lineStart; line<=numberToBlend; line++ )
  {
    if( line==0 )
      continue;
    
    real omega;
    if( side>=0 )
    {
      getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,-line,extra);  // interior line
      omega=line/(numberOfLinesForNormalBlend[side][axis]+1.);
    }
    else
    {
      Ig1=I1;
      Ig2=I2;
      Igv[axis]+=line;

      int ig = Igv[axis].getBase();
      if( ig<0 && (bool)getIsPeriodic(axis) )
      { // periodic wrap
	ig+=gridIndexRange(1,axis);
        Igv[axis]=ig;
      }
      if( ig<-1 || ig>gridIndexRange(1,axis)+1 )
	continue;  // skip this line, outside range of grid lines and ghost points.
      
      omega=abs(line)/(numberToBlend+1.);
    }
    
    if( debug & 2 )
    {
      int i1=Ig1.getBase(), i2=Ig2.getBase();
      fprintf(debugFile," blendNormals: (i1,i2)=(%i,%i) blend weight omega=%8.2e line=%i BEFORE blend marching "
              "vec=(%7.1e,%7.1e,%7.1e)\n",
	      i1,i2,omega,line,normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
    }

    // blend the normal on this line with the normal on the boundary
    normal(Ig1,Ig2,0,xAxes)=omega*normal(Ig1,Ig2,0,xAxes)+(1.-omega)*normal(I1,I2,0,xAxes);

    realArray normInverse;
    if( rangeDimension==2 )
      normInverse=1./max(REAL_MIN,SQRT(SQR(normal(Ig1,Ig2,0,0))+SQR(normal(Ig1,Ig2,0,1))));
    else
      normInverse=1./max(REAL_MIN,SQRT(SQR(normal(Ig1,Ig2,0,0))+
				       SQR(normal(Ig1,Ig2,0,1))+
				       SQR(normal(Ig1,Ig2,0,2))));
    for( int dir=0; dir<rangeDimension; dir++ )
      normal(Ig1,Ig2,0,dir)*=normInverse;
	
    if( debug & 2 )
    {
      int i1=Ig1.getBase(), i2=Ig2.getBase();
      fprintf(debugFile," blendNormals: (i1,i2)=(%i,%i) AFTER: blended marching vector=(%7.1e,%7.1e,%7.1e)\n",
	      i1,i2,normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
    }


    // *** blend xr and xs for the implicit method ****** wdh 010425
    Range G=domainDimension-1;
    xr(Ig1,Ig2,0,xAxes,G)=omega*xr(Ig1,Ig2,0,xAxes,G)+(1.-omega)*xr(I1,I2,0,xAxes,G);

  }

  return 0;
}




int HyperbolicMapping::
formBlockTridiagonalSystem(const int & direction, realArray & f )
//===========================================================================
/// \param Access: protected.
/// \brief  
///      Form the implicit time stepping equations in a given direction.
/// \param direction (input) : 0 or 1
/// \param f (input) : rhs which may be changed for some boundary conditions (?)
/// 
//===========================================================================
{
  real time0=getCPU();
  
// @PD realArray4[at,bt,ct,c,lambda,lambdaP,lambdaM] Range[I1,I2]

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int is[3] = {0,0,0};
  Range xAxes(0,rangeDimension-1);
 
  ::getIndex(indexRange,I1,I2,I3); 

  c.reshape(1,c.dimension(0),c.dimension(1),c.dimension(2));
  lambda.reshape(1,lambda.dimension(0),lambda.dimension(1));

  if( rangeDimension==2 )
  {
    at.reshape(4,at.dimension(2),at.dimension(3));
    bt.reshape(4,bt.dimension(2),bt.dimension(3));
    ct.reshape(4,ct.dimension(2),ct.dimension(3));
 
    at(0,I1,I2)=-.5*( c(0,I1,I2,0)+lambda(0,I1-1,I2));     // @PANS
    at(2,I1,I2)=-.5*( c(0,I1,I2,1));                       // @PANS

    at(3,I1,I2)=-.5*(-c(0,I1,I2,0)+lambda(0,I1-1,I2));     // @PANS
    at(1,I1,I2)=-.5*( c(0,I1,I2,1));                       // @PANS
   
    bt(0,I1,I2)=1.+.5*(lambda(0,I1,I2)+lambda(0,I1-1,I2)); // @PANS
    bt(2,I1,I2)=0.;                                        // @PANS

    bt(3,I1,I2)=1.+.5*(lambda(0,I1,I2)+lambda(0,I1-1,I2)); // @PANS
    bt(1,I1,I2)=0.;                                        // @PANS
   
    ct(0,I1,I2)= .5*( c(0,I1,I2,0)-lambda(0,I1,I2));       // @PANS
    ct(2,I1,I2)= .5*( c(0,I1,I2,1));                       // @PANS

    ct(3,I1,I2)= .5*(-c(0,I1,I2,0)-lambda(0,I1,I2));       // @PANS
    ct(1,I1,I2)= .5*( c(0,I1,I2,1));                       // @PANS
  }
  else if( rangeDimension==3 )
  {
    at.reshape(9,at.dimension(2),at.dimension(3));
    bt.reshape(9,bt.dimension(2),bt.dimension(3));
    ct.reshape(9,ct.dimension(2),ct.dimension(3));

    is[direction]=1;
 
    const realArray & lambdaP = lambda(0,I1,I2);
    const realArray & lambdaM = lambda(0,I1-is[0],I2-is[1]);
 
    at(0,I1,I2)=-.5*( c(0,I1,I2,0)+lambdaM ); // @PANS
    at(3,I1,I2)=-.5*( c(0,I1,I2,1) );         // @PANS
    at(6,I1,I2)=-.5*( c(0,I1,I2,2) );         // @PANS

    at(1,I1,I2)=-.5*( c(0,I1,I2,1) );         // @PANS
    at(4,I1,I2)=-.5*( c(0,I1,I2,3)+lambdaM ); // @PANS
    at(7,I1,I2)=-.5*( c(0,I1,I2,4) );         // @PANS
   
    at(2,I1,I2)=-.5*( c(0,I1,I2,2) );         // @PANS
    at(5,I1,I2)=-.5*( c(0,I1,I2,4) );         // @PANS
    at(8,I1,I2)=-.5*( c(0,I1,I2,5)+lambdaM ); // @PANS
   
    bt(0,I1,I2)=1.+.5*(lambdaP+lambdaM );     // @PANS
    bt(3,I1,I2)=0.;                           // @PANS
    bt(6,I1,I2)=0.;                           // @PANS
	
    bt(1,I1,I2)=0.;                           // @PANS
    bt(4,I1,I2)=1.+.5*(lambdaP+lambdaM );     // @PANS
    bt(7,I1,I2)=0.;                           // @PANS
	
    bt(2,I1,I2)=0.;                           // @PANS
    bt(5,I1,I2)=0.;                           // @PANS
    bt(8,I1,I2)=1.+.5*(lambdaP+lambdaM);      // @PANS
   	
    ct(0,I1,I2)= .5*( c(0,I1,I2,0)-lambdaP);  // @PANS
    ct(3,I1,I2)= .5*( c(0,I1,I2,1) );         // @PANS
    ct(6,I1,I2)= .5*( c(0,I1,I2,2) );         // @PANS
	
    ct(1,I1,I2)= .5*( c(0,I1,I2,1) );         // @PANS
    ct(4,I1,I2)= .5*( c(0,I1,I2,3)-lambdaP);  // @PANS
    ct(7,I1,I2)= .5*( c(0,I1,I2,4) );         // @PANS
	
    ct(2,I1,I2)= .5*( c(0,I1,I2,2) );         // @PANS
    ct(5,I1,I2)= .5*( c(0,I1,I2,4) );         // @PANS
    ct(8,I1,I2)= .5*( c(0,I1,I2,5)-lambdaP);  // @PANS
  }
  else
  {
    {throw "error";}
  }


  // *** boundary conditions:  ***

  Range R= rangeDimension*rangeDimension;
  for( int side=Start; side<=End; side++ )
  {
    getBoundaryIndex(indexRange,side,direction,I1,I2,I3);
    if( boundaryCondition(side,direction)==freeFloating )
    {
      // remember: we are solving for u = delta x
      if( false )
      {
	// Set: u(-1)=u(1)   
	if( side==Start )
	  ct(R,I1,I2)+=at(R,I1,I2);
	else
	  at(R,I1,I2)+=ct(R,I1,I2);
      }
      else if( true )
      {
	// Set: u(-1)=u(0)   
	if( side==Start )
	  bt(R,I1,I2)+=at(R,I1,I2);
	else
	  bt(R,I1,I2)+=ct(R,I1,I2);
      }
      else
      {
        // extrapolate  u(-1) = 2 u(0) - u(1)
        //  a*u(-1) + b*u(0) + c*u(1) 
	if( side==Start )
	{
	  bt(R,I1,I2)+=2.*at(R,I1,I2);
	  ct(R,I1,I2)-=at(R,I1,I2);
	}
	else
	{
	  bt(R,I1,I2)+=2.*ct(R,I1,I2);
	  at(R,I1,I2)-=ct(R,I1,I2);
	}
      }
      
    }
    // *wdh* 080121 else if( (bool)getIsPeriodic(axis1) )
    else if( (bool)getIsPeriodic(direction) )
    {
      // values should already be correct in this case
    }
    else
    {
      if( false )
      {
	// Set: u(-1)=u(1)   
	if( side==Start )
	  ct(R,I1,I2)+=at(R,I1,I2);
	else
	  at(R,I1,I2)+=ct(R,I1,I2);
      }
      else if( true )
      {
	// Set: u(-1)=u(0)   
	if( side==Start )
	  bt(R,I1,I2)+=at(R,I1,I2);
	else
	  bt(R,I1,I2)+=ct(R,I1,I2);
      }
      else
      {
        // extrapolate  u(-1) = 2 u(0) - u(1)
        //  a*u(-1) + b*u(0) + c*u(1) 
	if( side==Start )
	{
	  bt(R,I1,I2)+=2.*at(R,I1,I2);
	  ct(R,I1,I2)-=at(R,I1,I2);
	}
	else
	{
	  bt(R,I1,I2)+=2.*ct(R,I1,I2);
	  at(R,I1,I2)-=ct(R,I1,I2);
	}
      }

    }
  }

  c.reshape(c.dimension(1),c.dimension(2),c.dimension(3));
  lambda.reshape(lambda.dimension(1),lambda.dimension(2));

  at.reshape(rangeDimension,rangeDimension,at.dimension(1),at.dimension(2),at.dimension(3));
  bt.reshape(rangeDimension,rangeDimension,bt.dimension(1),bt.dimension(2),bt.dimension(3));
  ct.reshape(rangeDimension,rangeDimension,ct.dimension(1),ct.dimension(2),ct.dimension(3));

  if( Mapping::debug & 8 )
  {
    ::display(lambda,sPrintF("lambda for direction %i",direction));
    ::display(c,sPrintF("c for direction %i",direction));
    ::display(at,sPrintF("at for direction %i",direction));
    ::display(bt,sPrintF("bt for direction %i",direction));
    ::display(ct,sPrintF("ct for direction %i",direction));
  }

  timing[timeForFormBlockTridiagonalSystem]+=getCPU()-time0;

  return 0;
}



int HyperbolicMapping::
jacobiSmooth( realArray & u,  const int & numberOfSmooths )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Perform some smoothing steps.
//===========================================================================
{    
// @PD realArray2[u] Range[I1,I2,I3,J1,J2,J3,K1,K2,K3]
  real time0=getCPU();
  
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Ib1[2][2],Ib2[2][2],Ib3[2][2],Ig1[2][2],Ig2[2][2],Ig3[2][2];
  ::getIndex(indexRange,I1,I2,I3); 
  int is[3] = {0,0,0};
    
  int side,axis;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    for( side=Start; side<=End; side++ )
    {
      if( (bool)getIsPeriodic(axis)  ) // does include trailingEdge 
      {
	getBoundaryIndex(gridIndexRange,  side,axis,Ib1[side][axis],Ib2[side][axis],Ib3[side][axis]);
	getBoundaryIndex(gridIndexRange,1-side,axis,Ig1[side][axis],Ig2[side][axis],Ig3[side][axis]);
	if( side==Start )
	{
	  is[axis]=1;
	  Ib1[side][axis]-=is[0];
	  Ib2[side][axis]-=is[1];
	  Ig1[side][axis]-=is[0];
	  Ig2[side][axis]-=is[1];
	  is[axis]=0;
	}
      }
      else
      {
	getGhostIndex(gridIndexRange,   side,axis,Ig1[side][axis],Ig2[side][axis],Ig3[side][axis]);
	getBoundaryIndex(gridIndexRange,side,axis,Ib1[side][axis],Ib2[side][axis],Ib3[side][axis]);
      }
    }
  }
  
  const real omega=.1625;
  for( int smooth=0; smooth<numberOfSmooths+1; smooth++ )
  {
    // first assign boundary conditions
    for( axis=0; axis<domainDimension-1; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	Index &J1=Ib1[side][axis], &J2=Ib2[side][axis];
	Index &K1=Ig1[side][axis], &K2=Ig2[side][axis];
        if( (bool)getIsPeriodic(axis)  ) // does include trailingEdge 
	{
          if( side==Start )
  	    u(J1,J2)= u(K1,K2); // @PANS
	  else
	  {
  	    u(J1,J2)=u(K1,K2); // @PANS
	  }
	}
	else
	{
	  u(K1,K2)=u(J1,J2); // @PANS
	}
      }
    }
    if( smooth<numberOfSmooths )
    {
      if( domainDimension==2 )
	u(I1,I2)=(1.-omega)*u(I1,I2)+omega*.5*( u(I1+1,I2)+u(I1-1,I2) );  // @PANS
      else
	u(I1,I2)=(1.-omega)*u(I1,I2)+omega*.25*( u(I1+1,I2)+u(I1-1,I2)+u(I1,I2-1)+u(I1,I2+1) ); // @PANS
    }
  }

  timing[timeForSmoothing]+=getCPU()-time0;
  return 0;
}

int HyperbolicMapping::
formCMatrix(realArray & xr, 
	    realArray & xt, 
            const int & i3Mod2,
	    realArray & normal, 
	    realArray & normXr,
            const int & direction )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Compute the matrices $C^{-1} A$ or $C^{-1} B$
/// 
/// \param c (output) : $C^{-1} A$ if {\tt direction==0} or  $C^{-1} B$ if {\tt direction==1}
///     Since the output matrix is symmetric we only compute some of the terms.
///    In 2D : $c_{00}=c(I1,I2,0)$, $c_{10}=c_{01}=c(I1,I2,1)$, $c_{11}=c(I1,I2,2)$.
///    In 3D : $c_{00}=c(I1,I2,0)$, $c_{10}=c_{01}=c(I1,I2,1)$, $c_{20}=c_{02}=c(I1,I2,2)$,
///           $c_{11}=c(I1,I2,3)$, $c_{21}=c_{12}=c(I1,I2,4)$, $c_{22}=c(I1,I2,5)$
///  
/// \param Details:
///  \[   
///   A (\xv-\xv_0)_r + B ( \xv-\xv_0)_s + C( \xv-\xv_0)_t = \fv
///  \]
///  \[
///   A =  \begin{bmatrix}   \xv_t^T   \\
///                            0      \\ 
///                          \av^T    \end{bmatrix} \qquad
///   B =  \begin{bmatrix}    0        \\
///                          \xv_t^T  \\ 
///                          \bv^T    \end{bmatrix} \qquad
///   C =  \begin{bmatrix}   \xv_r^T   \\
///                          \xv_s^T  \\ 
///                          \nv^T    \end{bmatrix}
///  \]
///  \begin{align*}
///   \av &= \xv_s \times \xv_t \\
///   \bv &= \xv_r \times \xv_t
///  \end{align*}
//===========================================================================
{    
  real time0=getCPU();
  
  if( implicitCoefficient==0. )
  {
    c=0.;
    timing[timeForFormCMatrix]+=getCPU()-time0;
    return 0;
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  ::getIndex(indexRange,I1,I2,I3); 

// @PD realArray2[xrDotXr,normXr] Range[I1,I2]
  if( rangeDimension==2 )
  {
    realArray xrDotXr(I1,I2);
    xrDotXr=1./(normXr(I1,I2)*normXr(I1,I2));    // @PANS
    c(I1,I2,0)=(xr(I1,I2,0,0)*xt(I1,I2,i3Mod2,0)-xr(I1,I2,0,1)*xt(I1,I2,i3Mod2,1))*xrDotXr;  // c_{0,0} = -c_{1,1}
    c(I1,I2,1)=(xr(I1,I2,0,0)*xt(I1,I2,i3Mod2,1)+xr(I1,I2,0,1)*xt(I1,I2,i3Mod2,0))*xrDotXr;  // c_{1,0} =  c_{0,1}
  }
  else
  {
// @PD realArray5[jac,xr,normal,cInverse,a,c,xt] Range[I1,I2]
    realArray cInverse(I1,I2,3,3), jac(I1,I2), xrxt(I1,I2,3);
      
    jac(I1,I2)=(xr(I1,I2,0,0,0)*(xr(I1,I2,0,1,1)*normal(I1,I2,0,2)-xr(I1,I2,0,2,1)*normal(I1,I2,0,1))+ // @PANS
		xr(I1,I2,0,1,0)*(xr(I1,I2,0,2,1)*normal(I1,I2,0,0)-xr(I1,I2,0,0,1)*normal(I1,I2,0,2))+
		xr(I1,I2,0,2,0)*(xr(I1,I2,0,0,1)*normal(I1,I2,0,1)-xr(I1,I2,0,1,1)*normal(I1,I2,0,0)));
    where( jac==0. )
      jac=1.;       // @PAW
    jac=1./jac;     // @PANS
    if( direction==0 )
    {
      cInverse(I1,I2,0,0)=(xr(I1,I2,0,1,1)*normal(I1,I2,0,2)-xr(I1,I2,0,2,1)*normal(I1,I2,0,1))*jac; // @PANS
      cInverse(I1,I2,0,1)=(xr(I1,I2,0,2,1)*normal(I1,I2,0,0)-xr(I1,I2,0,0,1)*normal(I1,I2,0,2))*jac; // @PANS
      cInverse(I1,I2,0,2)=(xr(I1,I2,0,0,1)*normal(I1,I2,0,1)-xr(I1,I2,0,1,1)*normal(I1,I2,0,0))*jac; // @PANS
    }
    else
    {
      cInverse(I1,I2,1,0)=(normal(I1,I2,0,1)*xr(I1,I2,0,2,0)-normal(I1,I2,0,2)*xr(I1,I2,0,1,0))*jac; // @PANS
      cInverse(I1,I2,1,1)=(normal(I1,I2,0,2)*xr(I1,I2,0,0,0)-normal(I1,I2,0,0)*xr(I1,I2,0,2,0))*jac; // @PANS
      cInverse(I1,I2,1,2)=(normal(I1,I2,0,0)*xr(I1,I2,0,1,0)-normal(I1,I2,0,1)*xr(I1,I2,0,0,0))*jac; // @PANS
    }
//     cInverse(I1,I2,2,0)=(xr(I1,I2,0,1,0)*xr(I1,I2,0,2,1)-xr(I1,I2,0,2,0)*xr(I1,I2,0,1,1))*jac; // @PANS
//     cInverse(I1,I2,2,1)=(xr(I1,I2,0,2,0)*xr(I1,I2,0,0,1)-xr(I1,I2,0,0,0)*xr(I1,I2,0,2,1))*jac; // @PANS
//     cInverse(I1,I2,2,2)=(xr(I1,I2,0,0,0)*xr(I1,I2,0,1,1)-xr(I1,I2,0,1,0)*xr(I1,I2,0,0,1))*jac; // @PANS
    // *wdh* 010426
    cInverse(I1,I2,2,0)=normal(I1,I2,0,0)*jac; // @PANS
    cInverse(I1,I2,2,1)=normal(I1,I2,0,1)*jac; // @PANS
    cInverse(I1,I2,2,2)=normal(I1,I2,0,2)*jac; // @PANS
      
      
    // a = xs X xt or a = xr X xt
    const int dp= (direction+1)%2;
    xrxt(I1,I2,0)=xr(I1,I2,0,1,dp)*xt(I1,I2,i3Mod2,2)-xr(I1,I2,0,2,dp)*xt(I1,I2,i3Mod2,1); // @PANS
    xrxt(I1,I2,1)=xr(I1,I2,0,2,dp)*xt(I1,I2,i3Mod2,0)-xr(I1,I2,0,0,dp)*xt(I1,I2,i3Mod2,2); // @PANS
    xrxt(I1,I2,2)=xr(I1,I2,0,0,dp)*xt(I1,I2,i3Mod2,1)-xr(I1,I2,0,1,dp)*xt(I1,I2,i3Mod2,0); // @PANS
    if( direction==1 )
      xrxt=-xrxt;

    for( int k=0; k<3; k++ )
      c(I1,I2,k)=cInverse(I1,I2,direction,k)*xt(I1,I2,i3Mod2,0)+cInverse(I1,I2,2,k)*xrxt(I1,I2,0); // @PANS

    c(I1,I2,3)  =cInverse(I1,I2,direction,1)*xt(I1,I2,i3Mod2,1)+cInverse(I1,I2,2,1)*xrxt(I1,I2,1); // @PANS
    c(I1,I2,4)  =cInverse(I1,I2,direction,2)*xt(I1,I2,i3Mod2,1)+cInverse(I1,I2,2,2)*xrxt(I1,I2,1); // @PANS

    c(I1,I2,5)  =cInverse(I1,I2,direction,2)*xt(I1,I2,i3Mod2,2)+cInverse(I1,I2,2,2)*xrxt(I1,I2,2); // @PANS
	
  }

  if( implicitCoefficient!=1. )
    c*=implicitCoefficient;

  if( Mapping::debug & 4 )
  {
    Range Rx=rangeDimension;
    ::display(xt(I1,I2,i3Mod2,Rx),"formCMatrix: xt after step ?",debugFile);
    ::display(c,"formCMatrix: solution c after step ?",debugFile);
  }
  
  timing[timeForFormCMatrix]+=getCPU()-time0;
  return 0;
}
