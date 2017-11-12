// define BOUNDS_CHECK

#include "HyperbolicMapping.h"
// #include "DataPointMapping.h"
// #include "TridiagonalSolver.h"
#include "display.h"
#include "arrayGetIndex.h"
#include "TrimmedMapping.h"
#include "CompositeTopology.h"
#include "UnstructuredMapping.h"
#include "CompositeSurface.h"


#include "MappingProjectionParameters.h"
#include "MatchingCurve.h"


int HyperbolicMapping::
applyBoundaryConditions(const RealArray & x, const RealArray & x0,
                        const int & marchingDirection,
           		RealArray & normal, 
			RealArray & xr,
                        bool initialStep,  /* =false */
                        int stepNumber /* = 0 */ )
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Apply boundary conditions to the grid points. For surface grids we also
///   compute xr(I1,I2,0,0:2,1) which are the derivatives normal to the surface.
///  
/// \param x (input) : current front, apply BC's to these values.
/// \param x0 (input) : previous front, used for some BC's. On the first step it is ok to have x1==x0
/// \param xr (output) : for a surface grid, this routine will find the normal derivative
///      component of this array.
/// \param normal (input) : the marching-direction
/// \param marchingDirection (input) : +1 for forward, -1 for reverse
/// \param stepNumber (input) : current step number
// ==========================================================================
{
  real time0=getCPU();
  

  // ::display(boundaryCondition,"boundaryCondition","%3i");

  const int i3p=x.getBase(2);
  const int i3 =x0.getBase(2);
  

  int is[2]; // , &is1=is[0]; 
  int js[2]; 
  // boundary conditions
  Index I1,I2,I3;
  Range xAxes(0,rangeDimension-1);
  const int extra=1;


  // ::display(x(nullRange,nullRange,i3p,xAxes),sPrintF("Here is x at step %i BEFORE BC",i3p));

  // ************************************************************************
  // **** For surface grids we apply BC's then project then reapply BC's ****   *wdh* 020925
  // ************************************************************************
  // *** NOTE: for a BC such as bcSide==fixZfloatXY, when also projecting onto a surface
  //           the points will end up with z=fixed but may not be exactly on the surface -- need
  //           a special projection for this case 

  int numberOfBCIterations = surfaceGrid? 2 : 1;
  for( int bcIteration=0; bcIteration<numberOfBCIterations; bcIteration++ )
  {
    // Apply most boundary conditions before we project onto the reference surface.
    int axis;
    for( axis=0; axis<domainDimension-1; axis++ )
    {

      // for surface grids we store boundary conditions in axis==0 (forward) and axis==1 (backward)

      const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;
      for( int side=Start; side<=End; side++ )
      {
	is[0]=is[1]=0;
	js[0]=js[1]=0;

	const int bcSide=boundaryCondition(side,direction);

	if( bcSide==freeFloating )
	{
	  getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,1,extra);
	  is[axis]=1-2*side;
	  x(I1,I2,i3p,xAxes)=2.*x(I1+is[0],I2+is[1],i3p,xAxes)-x(I1+2*is[0],I2+2*is[1],i3p,xAxes);
	}
	else if( bcSide==outwardSplay ) 
	{
	  // make the outward splay proportional to the distance marched
	  // dist = distance marched on boundary from x0 to x1
	  // dx = vector that points outward from the boundary (boundary point - first point inside)
	  //  x_{-1} = 
	  getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,1,extra);
	  is[axis]=1-2*side;
	  RealArray dx(I1,I2,1,xAxes), dist, norm;
	  dx= x(I1+is[0],I2+is[1],i3p,xAxes)-x(I1+2*is[0],I2+2*is[1],i3p,xAxes);
	  if( rangeDimension==2 )
	  {
	    dist = SQRT( SQR(x(I1+is[0],I2+is[1],i3p,0)-x0(I1+is[0],I2+is[1],i3,0)) +
			 SQR(x(I1+is[0],I2+is[1],i3p,1)-x0(I1+is[0],I2+is[1],i3,1)) );
	    norm = SQRT( SQR(dx(I1,I2,0,0)) + SQR(dx(I1,I2,0,1)) );
	  }
	  else
	  {
	    dist = SQRT( SQR(x(I1+is[0],I2+is[1],i3p,0)-x0(I1+is[0],I2+is[1],i3,0)) +
			 SQR(x(I1+is[0],I2+is[1],i3p,1)-x0(I1+is[0],I2+is[1],i3,1)) +
			 SQR(x(I1+is[0],I2+is[1],i3p,2)-x0(I1+is[0],I2+is[1],i3,2)) );
	    norm = SQRT( SQR(dx(I1,I2,0,0)) + SQR(dx(I1,I2,0,1))  + SQR(dx(I1,I2,0,2)) );
	  }
	  where( norm>0. )
	    dist/=norm;
	  for( int dir=0; dir<rangeDimension; dir++ )
	    dx(I1,I2,0,dir)*=dist;
	
	  if( debug & 2 )
	    printf("outwardSplay: splayFactor[side=%i][axis=%i]=%e, max(|dx|) = %e \n", side,axis,
		   splayFactor[side][axis],max(dx));
	
	  // set ghost line:
	  x(I1,I2,i3p,xAxes)=2.*x(I1+is[0],I2+is[1],i3p,xAxes)-x(I1+2*is[0],I2+2*is[1],i3p,xAxes)
	    + splayFactor[side][axis]*dx;
	  // set boundary values : average of current boundary value, the ghost line and first line in
	  x(I1+is[0],I2+is[1],i3p,xAxes)=.5*x(I1+is[0],I2+is[1],i3p,xAxes)+
	    .25*(x(I1,I2,i3p,xAxes)+x(I1+2*is[0],I2+2*is[1],i3p,xAxes));
	
	}
	else if( bcSide==trailingEdge  ) 
	{
	  // this is done below as a periodic BC


	}
	else if( bcSide==singularAxis )
	{
	  // Move the axis line back onto the axis.

	  getBoundaryIndex(indexRange,side,axis,I1,I2,I3,extra);
	  is[axis]=1-2*side;
	  assert( rangeDimension==3 );
	  RealArray xNorm(I1,I2,1,xAxes);
	  xNorm = x(I1,I2,i3p,xAxes)-x0(I1,I2,i3,xAxes);
	  xNorm(I1,I2,0,0) = SQRT( SQR(xNorm(I1,I2,0,0))+SQR(xNorm(I1,I2,0,1))+SQR(xNorm(I1,I2,0,2)) );
	  int dir;
	  for( dir=0; dir<rangeDimension; dir++ )
	    x(I1,I2,i3p,dir)=x0(I1,I2,i3,dir)+xNorm(I1,I2,0,0)*normal(I1,I2,0,dir);
	
	  // extrapolate tangential components (do all)
	  x(I1-is[0],I2-is[1],i3p,xAxes)=2.*x(I1,I2,i3p,xAxes)-x(I1+is[0],I2+is[1],i3p,xAxes);
	  // normal component equals first line in
	  const RealArray & nDot = evaluate(
	    (x(I1+is[0],I2+is[1],i3p,axis1)-x(I1-is[0],I2-is[1],i3p,axis1))*normal(I1,I2,0,axis1)+
	    (x(I1+is[0],I2+is[1],i3p,axis2)-x(I1-is[0],I2-is[1],i3p,axis2))*normal(I1,I2,0,axis2)+
	    (x(I1+is[0],I2+is[1],i3p,axis3)-x(I1-is[0],I2-is[1],i3p,axis3))*normal(I1,I2,0,axis3) );
	  for( dir=0; dir<rangeDimension; dir++ )
	    x(I1-is[0],I2-is[1],i3p,dir)+=nDot*normal(I1,I2,0,dir);
	}
	else if( bcSide==fixXfloatYZ ||
		 bcSide==fixYfloatXZ ||
		 bcSide==fixZfloatXY )
	{
	  is[axis]=1-2*side;
	  int fixDir = bcSide==fixXfloatYZ ? 0 : bcSide==fixYfloatXZ ? 1 : 2;

	  // set value on boundary of fixed direction to value from the previous step.

	  getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3,extra);

	  // *wdh* 011119: make this BC look more like the match to mapping BC
	  //   project a point off the boundary
	  // *wdh* 021001 const real & ortho = matchToMappingOrthogonalFactor;  // this is usually .5 by default

          // scale ortho by the blending factor to account for the case when the marching direction
          // is not parallel to the boundary condition
	  const real ortho = matchToMappingOrthogonalFactor*2./max(1.,(numberOfLinesForNormalBlend[side][axis]));

	  if( !initialStep )
	  { // do not change the boundary points on the initial step -- finish me for other cases
	    x(I1,I2,i3p,fixDir)=x0(I1,I2,i3,fixDir);  // project onto plane x[fixDir]==const


	    x(I1,I2,i3p,xAxes)=(ortho     *x(I1+is[0],I2+is[1],i3p,xAxes)+
				(1.-ortho)*x(I1      ,I2      ,i3p,xAxes) );

	    x(I1,I2,i3p,fixDir)=x0(I1,I2,i3,fixDir);  // project onto plane x[fixDir]==const
	  }
	  

	  //if( true && bcSide==fixZfloatXY )
	  //  printf(" BC:fixZfloatXY: set x(%i,%i,%i,%i) = %9.3e (step=%i)\n",I1.getBase(),I2.getBase(),i3p,fixDir,
	  //   x(I1.getBase(),I2.getBase(),i3p,fixDir),stepNumber);

	  // set ghost values of non-fixed directions to values on boundary
	  getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,1,extra);

	  //                 E     N   \     E=extrapolated point
	  //                  .    .    \      N=normal point (from reflection)
	  //                   .   .     \
          //   -boundary--------.--.------\
          //                     . .       \
          //                      ..        \
          //                  -----X---------\
          //                        .         \

	  // *wdh* 011118  -- try using a symmetry condition on the ghost line to help the grid
	  //                  grow out of sharp corners.
	  // x(I1,I2,i3p,xAxes)=x(I1+2*is[0],I2+2*is[1],i3p,xAxes);

          // *wdh* 020101 : we blend an orthogonal condition with extrapolation

          // if ortho=.5, blend=1 : orthogonal BC
          // as ortho->0 blend->0. : extrapolation BC
	  const real blend = min(1.,max(0.,ortho*2.)); 

          if( debug & 2  )
	    printf("fixBC: matchToMappingOrthogonalFactor=%8.2e, ortho=%8.2e blend=%8.2e\n",matchToMappingOrthogonalFactor,
                       ortho,blend);

	  x(I1,I2,i3p,xAxes)=blend*x(I1+2*is[0],I2+2*is[1],i3p,xAxes)+
	                (1.-blend)*( 2.*x(I1+is[0],I2+is[1],i3p,xAxes)-x(I1+2*is[0],I2+2*is[1],i3p,xAxes));
          // we always extrap the fixed direction
	  x(I1,I2,i3p,fixDir)=2.*x(I1+is[0],I2+is[1],i3p,fixDir)-x(I1+2*is[0],I2+2*is[1],i3p,fixDir);

	}
	else if( bcSide==floatXfixYZ ||
		 bcSide==floatYfixXZ ||
		 bcSide==floatZfixXY )
	{
	  int floatDir = bcSide==floatXfixYZ ? 0 : 
	    bcSide==floatYfixXZ ? 1 : 2;

	  // set value on boundary of fixed directions to values from the previous step.
	  getBoundaryIndex(gridIndexRange,side,axis,I1,I2,I3);
	  int dir;
	  if( !initialStep )
	  { // do not change the boundary points on the initial step
	    for( dir=0; dir<rangeDimension; dir++ )
	    {
	      if( dir!=floatDir )
		x(I1,I2,i3p,dir)=x0(I1,I2,i3,dir);
	    }
	  }
	  
	  // set ghost values of non-fixed direction to values on boundary
	  getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,1,extra);
	  is[axis]=1-2*side;
	  x(I1,I2,i3p,floatDir)=x(I1+is[0],I2+is[1],i3p,floatDir);  // ******************** is this right

	  // extrapolate ghost values of fixed direction.
	  for( dir=0; dir<rangeDimension; dir++ )
	  {
	    if( dir!=floatDir )
	      x(I1,I2,i3p,dir)=2.*x(I1+is[0],I2+is[1],i3p,dir)-x(I1+2*is[0],I2+2*is[1],i3p,dir);
	  }
	}
	else if( bcSide==xSymmetryPlane ||
		 bcSide==ySymmetryPlane ||
		 bcSide==zSymmetryPlane )
	{
	  getGhostIndex(gridIndexRange,side,axis,I1,I2,I3,1,extra);
	  is[axis]=1-2*side;
	  x(I1,I2,i3p,xAxes)=x(I1+2*is[0],I2+2*is[1],i3p,xAxes);
	  const int dir=bcSide==xSymmetryPlane ? 0 : bcSide==ySymmetryPlane ? 1 : 2;
	  x(I1,I2,i3p,dir)=2.*x(I1+is[0],I2+is[1],i3p,axis1)-x(I1+2*is[0],I2+2*is[1],i3p,dir);
	}
	else if( bcSide==matchToMapping || bcSide==matchToABoundaryCurve )
	{
	  // this is done below
	}
/* -----
   else if( bcSide==corner )
   {
   // At a corner we use an average of the neighbours

   }
   ----- */
	else if( !((bool)getIsPeriodic(axis)) )
	{
	  if( bcSide==periodic )
	  {
	    printf("ERROR: boundaryCondition(%i,%i)==periodic but the grid is not periodic!\n",
		   side,axis);
	  }
	  else
	  {
	    printf("HyperbolicMapping::applyBoundaryConditions:FATAL ERROR: unknown boundary condition\n");
	    Overture::abort("error");
	  }
	}
	is[axis]=0;
	js[axis]=0;
      }
    }

    if( debug & 4 )
    {
      ::display(x(nullRange,nullRange,i3p,xAxes),sPrintF("\n***x after most BC, before project step %i",stepNumber),
		debugFile,"%10.3e ");
    }


    if( equidistributionWeight>0.  )
    {
      // equidistribute lines
      equidistributeAndStretch( i3p, x, equidistributionWeight,marchingDirection,
                                useStartCurveStretchingWhileMarching );
    }

    // *****************************************************************************************
    // project the points onto the reference surface -- do here so we project ghost points too
    // *****************************************************************************************
    if( bcIteration==0 )
    {
      if( surfaceGrid )
      {
	timing[timeForBoundaryConditions]+=getCPU()-time0;

	int rt=project( x(nullRange,nullRange,i3p,xAxes),marchingDirection,xr,true,initialStep,stepNumber ); 

        if( rt!=0 )
	{
	  printf("applyBoundaryConditions:ERROR:error return from project\n");
	  return 1;
	}

        // ***** if we have turned a corner the marching vector (normal) should be changed! ******



	if( debug & 2 && surfaceGrid )
	{
	  ::getIndex(gridIndexRange,I1,I2,I3);
	  ::display(xr(I1,I2,0,xAxes,1),sPrintF("\n*** applyBC after project: normal to surface at step %i",
						stepNumber),debugFile,"%8.1e ");
	}
    
	time0=getCPU();


	if( debug & 4 || (debug & 2 && surfaceGrid) )
	{
	  ::display(x(nullRange,nullRange,i3p,xAxes),sPrintF("\n *** x after project, step %i (initialStep=%i)",
							     stepNumber,initialStep),debugFile,"%10.3e ");
	}
      }

      // **** these boundary conditions we apply after the projection onto the reference surface ****

      // project boundaries onto BC-mapping's and interior matching curves.
      applyBoundaryConditionMatchToMapping(x, marchingDirection, normal, xr, initialStep);
    }
    

    // printF("applyBC: indexRange=[%i,%i][%i,%i] gid=[%i,%i][%i,%i] x=[%i,%i][%i,%i] xr=[%i,%i][%i,%i] "
    //        "x0=[%i,%i][%i,%i]\n",
    //        indexRange(0,0),indexRange(1,0), indexRange(0,1),indexRange(1,1),
    //        gridIndexRange(0,0),gridIndexRange(1,0), gridIndexRange(0,1),gridIndexRange(1,1),
    //        x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),
    //        xr.getBase(0),xr.getBound(0),xr.getBase(1),xr.getBound(1),
    //        x0.getBase(0),x0.getBound(0),x0.getBase(1),x0.getBound(1)
    //        );

    // --- do periodic boundaries last to get corners correct ---
    //    --- fixed Nov 12, 2017 for doubly derivative periodic --- *wdh* 
    Index Ib1,Ib2,Ib3;
    Index Ig1,Ig2,Ig3;
    // Index Ib1,Ib2,Ib3;
    for( axis=0; axis<domainDimension-1; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	is[0]=is[1]=0; // *wdh* Nov 12, 2017
        is[axis]=1-2*side; 
	js[0]=js[1]=0;
	if( (bool)getIsPeriodic(axis) )
	{
	  // getBoundaryIndex(indexRange,side,axis,Ib1,Ib2,Ib3,extra);
	  // getGhostIndex(indexRange,side,axis,I1,I2,I3,1,extra);

	  getBoundaryIndex(gridIndexRange,side,axis,Ib1,Ib2,Ib3,extra);
	  getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,1,extra);

	  js[axis]=(gridIndexRange(End,axis)-gridIndexRange(Start,axis))*is[axis];

          // printF("applyBC: [side,axis]=(%i,%i) extra=%i : Ib1=[%i,%i] Ib2=[%i,%i] I1=[%i,%i] I2=[%i,%i]"
          //      " Ib=[%i,%i][%i,%i] Ig1=[%i,%i][%i,%i] js[axis]=%i \n",
          //        side,axis,extra,
          //        Ib1.getBase(),Ib1.getBound(),Ib2.getBase(),Ib2.getBound(), 
          //        I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
          //        Ib1.getBase(),Ib1.getBound(),Ib2.getBase(),Ib2.getBound(),
          //        Ig1.getBase(),Ig1.getBound(),Ig2.getBase(),Ig2.getBound(),
          //        js[axis]
          //        );
          
          // ---- assign ghost points ----
 	  if( getIsPeriodic(axis)==Mapping::functionPeriodic )
          {
            if( debug & 2 ){ printF("applyBC: apply function periodic BC\n");}  
	    x(Ig1,Ig2,i3p,xAxes)=x(Ig1+js[0],Ig2+js[1],i3p,xAxes);
          }
          else
	  { // derivative periodic *wdh* 080521 
            x(Ig1,Ig2,i3p,xAxes)=x(Ig1+js[0],Ig2+js[1],i3p,xAxes) +
                 (x0(Ib1,Ib2,i3p,xAxes) - x0(Ib1+js[0],Ib2+js[1],i3p,xAxes));
	  }
	  
	  // apply periodicity to normals and surface normals since they may have been changed by the above BC's
	  normal(Ig1,Ig2,0,xAxes)=normal(Ig1+js[0],Ig2+js[1],0,xAxes);
	  if( surfaceGrid )
	    xr(Ig1,Ig2,0,xAxes,1)=xr(Ig1+js[0],Ig2+js[1],0,xAxes,1);
	
	  if( side==End )
	  {
            // --- Also set right-boundary values equal to left ---
	    if( getIsPeriodic(axis)==Mapping::functionPeriodic )
              x(Ib1,Ib2,i3p,xAxes)=x(Ib1+js[0],Ib2+js[1],i3p,xAxes);
	    else
	    {// derivative periodic *wdh* 080521 
              x(Ib1,Ib2,i3p,xAxes)=x(Ib1+js[0],Ib2+js[1],i3p,xAxes) +
                 (x0(Ib1,Ib2,i3p,xAxes) - x0(Ib1+js[0],Ib2+js[1],i3p,xAxes));
	    }
	    
          
	    normal(Ib1,Ib2,0,xAxes)=normal(Ib1+js[0],Ib2+js[1],0,xAxes);
	    if( surfaceGrid )
	      xr(Ib1,Ib2,0,xAxes,1)=xr(Ib1+js[0],Ib2+js[1],0,xAxes,1);
	  }
	}
	else if( surfaceGrid && !projectGhostPoints(side,axis) )
	{
        
//         const int i1=gridIndexRange(side,axis1)-is1;
//         printf("reset ghost points to unprojected values, i1=%i, is1=%i, projectGhostPoints=%i\n",
//                 i1,is1,projectGhostPoints(side,axis));
//        x(i1,0,i3p,xAxes)=xSave(0,0,0,xAxes);
	  //      xr(i1,0,0,xAxes,axis2)=xr(i1+is1,0,0,xAxes,axis2);
	}
      }
    }
  }
  

  if( debug & 2 )
  {
    ::display(x(nullRange,nullRange,i3p,xAxes),sPrintF("\n***x after applyBC step %i",stepNumber),
         debugFile,"%10.3e ");
  }
  if( debug & 2 && surfaceGrid )
  {
    ::getIndex(gridIndexRange,I1,I2,I3);
    ::display(xr(I1,I2,0,xAxes,1),sPrintF("\n*** applyBC at END: normal to surface at step %i",
					  stepNumber),debugFile,"%8.1e ");
  }
  timing[timeForBoundaryConditions]+=getCPU()-time0;
  return 0;
}


int HyperbolicMapping::
applyBoundaryConditionMatchToMapping(const RealArray & x, 
				     const int & marchingDirection,
				     RealArray & normal, 
				     RealArray & xr,
				     bool initialStep /* = false */ ,
				     int option /* =0 */ )
// ==============================================================================================================
// /Description:
//     Apply the matchToMapping and matchToABoundaryCurve BC's.
//  In this BC we project the boundary point (first-step) or first line in, onto the Mapping defining
// the boundary condition. (The interior normals are blended with boundary normals in getNormalAndSurfaceArea).
// 
// /initialStep (input) : true of this is the initial marching step.
// /option (input) : option==1 means do not change x or the normals, just project onto the boundary. Use this
// option to get correct values for the normals and tangents to the BC Mapping saved in the 
// boundaryConditionMappingProjectionParameters[][] objects. This must be done before the first call to
// getNormalsAndSurfaceAreas.
//
// 
// ==============================================================================================================
{
  int side,axis;
  Index I1,I2,I3;
  const int i3p=x.getBase(2);
  int is[2];
  Range xAxes(0,rangeDimension-1);

  for( axis=0; axis<domainDimension-1; axis++ )
  {

    const int direction = !surfaceGrid || marchingDirection==1 ? axis : axis+1;

    for( int side=Start; side<=End; side++ )
    {
      is[0]=is[1]=0;
      
      if( boundaryCondition(side,direction)==matchToMapping || 
          boundaryCondition(side,direction)==matchToABoundaryCurve )
      {
	// project the boundary values to lie on another Mapping
        // Assign ghost line values consistent with the normal being tangential to the boundary.

        assert( boundaryConditionMapping[side][direction]!=NULL );
        int axisp1 = (axis+1) % (domainDimension-1);
        int extra= (bool)getIsPeriodic(axisp1) ? 0 : 1;
        getBoundaryIndex(indexRange,side,axis,I1,I2,I3,extra);  // *wdh* 011127: don't project periodic ends

	MappingProjectionParameters & mpParams = boundaryConditionMappingProjectionParameters[side][direction];

        is[axis]=1-2*side;

	bool projectBoundary=true;
	matchToCurve( projectBoundary,I1,I2,*boundaryConditionMapping[side][direction],
		      mpParams,is[0],is[1],x,marchingDirection,normal,xr,side,axis,initialStep,option);

	// n.reshape(numberOfPoints,xAxes);  // reset the shape.
      }
    }
  }

  // Here we match to interior curves
  const int numberOfMatchingCurves=matchingCurves.size();
  if( numberOfMatchingCurves>0 )
  {
    for( int i=0; i<numberOfMatchingCurves; i++ )
    {
      MatchingCurve & match = matchingCurves[i];
      //if( true ||  // *wdh* 081102 -- the matching curve may go in two direction -- we may need to fix ---
      //    match.curveDirection==marchingDirection )
      if( match.curveDirection==marchingDirection ||  // *wdh* 090708
          match.curveDirection==0 )
      {
	const int gridLine=match.gridLine+boundaryOffset[0][0];  // this should have been computed when the start curve was evaluated
        assert( gridLine>=0 );

	assert( match.curve!=NULL && match.projectionParameters!=NULL );
       
	Mapping & matchingMap = *match.curve;
       
	MappingProjectionParameters & mpParams = *match.projectionParameters;
       
	int i2=x.getBase(1);

        if( true )
	{
	  if( true || debug & 2 ) printf(" Project onto the matching curve %i: gridLine=%i "
                     "x=[%i,%i][%i,%i] boundaryOffset=[%i,%i]\n",i,gridLine,x.getBase(0),x.getBound(0),
                         x.getBase(1),x.getBound(1), boundaryOffset[0][0],boundaryOffset[1][0]     );
	  
	  I1=gridLine;
	  I2=i2;
          bool projectBoundary=false;
	  matchToCurve( projectBoundary,I1,I2,matchingMap,
			mpParams,0,0,x,marchingDirection,normal,xr,0,0,initialStep,option);
	}
	else
	{
	  RealArray xx(1,3);
	  int axis;
	  for( axis=0; axis<3; axis++ )
	    xx(0,axis)=x(gridLine,i2,i3p,axis);

	  matchingMap.project(xx,mpParams);

	  printf(" Project onto the matching curve: gridLine=%i, x=(%8.2e,%8.2e,%8.2e) xx=(%8.2e,%8.2e,%8.2e)\n",
		 gridLine,x(gridLine,0,i3p,0),x(gridLine,0,i3p,1),x(gridLine,0,i3p,2),
		 xx(0,0),xx(0,1),xx(0,2));

	  for( axis=0; axis<3; axis++ )
	    x(gridLine,i2,i3p,axis)=xx(0,axis);
	  
	}
      }
      else
      {
         printF("INFO: match to interior curves: skipping matching curve %i since match.curveDirection=%i\n",
		i,match.curveDirection);
      }
      
    }
  }
  

  return 0;
}

// ================================================================================================================
/// \brief Project points onto a boundary curve (or boundary surface for a 3d volume grid) 
///   or interior matching curve and adjust normals.
///
///
///  /param projectBoundary: if true we are projecting onto a boundary (as opposed to an interior line)
///  /param i1Shift: one of 1,0,-1. 1=left boundary (start), 0=interior curve, 1=right boundary
///
// ================================================================================================================
int HyperbolicMapping::
matchToCurve( bool projectBoundary,
	      Index & I1, Index & I2,
              Mapping & matchingMapping,
              MappingProjectionParameters & mpParams,
              int i1Shift, 
              int i2Shift, 
              const RealArray & x, 
	      const int & marchingDirection,
	      RealArray & normal, 
	      RealArray & xr,
              const int sideBlend, const int axisBlend, // side axis for numberOfLinesForNormalBlend
	      bool initialStep /* = false */ ,
	      int option /* =0 */
               )
{
  const int i3p=x.getBase(2);
  Range xAxes(0,rangeDimension-1);
  
  RealArray & n = mpParams.getRealSerialArray(MappingProjectionParameters::normal);
  const int numberOfPoints=I1.getLength()*I2.getLength();

  if( n.getLength(0)!=numberOfPoints || initialStep )
  { // number of points has changed, reset the Mapping Projection Parameters.
    mpParams.reset();
    n.redim(numberOfPoints,rangeDimension);
  }
  else
  {
    n.reshape(numberOfPoints,rangeDimension);
  }
	
  RealArray xx(I1,I2,1,rangeDimension);

  // is[axis]=1-2*side;

  // scale ortho by the blending factor
  if( debug & 2 )
    fprintf(debugFile," >>>>Entering matchToCurve: marchingDirection=%i, i1Shift=%i i2Shift=%i numberOfLinesForNormalBlend=%i %i %i %i "
            "<<<< \n",marchingDirection,i1Shift,i2Shift,numberOfLinesForNormalBlend[0][0],
	    numberOfLinesForNormalBlend[1][0],numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][1]);

//    const int side= i1Shift==1 ? 0 : 1;
//    const int axis=0; //  marchingDirection==-1 ? 0 : 1; ***NOTE*** always use 0 until blend is extended

  // printf(" =================== matchToCurve: using numberOfLinesForNormalBlend[%i][%i]=%i\n",
  // 	 sideBlend,axisBlend,numberOfLinesForNormalBlend[sideBlend][axisBlend]);
  

  const real ortho = matchToMappingOrthogonalFactor*2./max(1.,(numberOfLinesForNormalBlend[sideBlend][axisBlend]));

  if( !projectBoundary || initialStep  )
  {

    xx(I1,I2,0,xAxes)=x(I1,I2,i3p,xAxes);  // initial conditions, project the bndry.
  }
  else
  {
    // project first line in onto the boundary -- -this will give a normal BC
    
    if( debug & 2 )
      fprintf(debugFile,"\n    : project first line in, matchToMappingOrthogonalFactor=%f\n",
                matchToMappingOrthogonalFactor);

    xx(I1,I2,0,xAxes)=    ortho *x(I1+i1Shift,I2+i2Shift,i3p,xAxes)+
                      (1.-ortho)*x(I1        ,I2        ,i3p,xAxes);
  }

  if( debug & 2 ) 
  {
    fprintf(debugFile,"    : projectBoundary=%i, initialStep=%i\n",projectBoundary,initialStep);
    if( domainDimension==2 && rangeDimension==3 )
      fprintf(debugFile,"    : before project onto the boundary xx=(%10.4e,%10.4e,%10.4e)\n",
	   xx(I1.getBase(),I2.getBase(),0,0),xx(I1.getBase(),I2.getBase(),0,1),xx(I1.getBase(),I2.getBase(),0,2));
    else
      ::display(xx,"    :xx before project onto the boundary ",debugFile,"%10.4e ");
  }
  xx.reshape(Range(numberOfPoints),xAxes);
  // project the point xx onto the BC mapping
  // On output the array "n" will hold the normal to the BC surface (for 3D volume grids) or the tangent
  // to the BC mapping for 3D surface grids ( ?? 2D grids ?? )

  //   RealArray xSave;
  //   xSave=xx;

  mpParams.setAdjustForCornersWhenMarching(false);

  if( false )  // *wdh* 081102 -- for testing 
  {
    RealArray & r  = mpParams.getRealSerialArray(MappingProjectionParameters::r);
    if( r.getLength(0)>0 )
      r=-1;
  }
  
  //   printf("***matchToCurve: projectBoundary=%i, initialStep=%i\n",projectBoundary,initialStep);
  //   ::display(xx,"*****matchToCurve:xx before project onto the boundary ","%7.1e ");

  // When the matching surface is a CompositeSurface we can project onto the reference surface
  // or just use the unstructured surface:
  mpParams.setProjectOntoReferenceSurface(projectOntoReferenceSurface);

  matchingMapping.project(xx,mpParams);

  //   ::display(xx,"****matchToCurve:xx after project onto the boundary ","%7.1e ");

  // ==============================================================================================
  if( initialStep && domainDimension==3 && matchingMapping.getClassName()=="CompositeSurface" )
  {
    // ***** Volume grid ******
    // if we are projecting onto a composite surface

    int marchDirection=i1Shift!=0 ? i1Shift : i2Shift; // axis==0 ? 1-2*side : 2*side-1;
    marchDirection*=+marchingDirection; // -marchingDirection;
    if( debug & 2 )
      printf("    :correct the projection of a BC edge onto a CompositeSurface isShift=%i,%i, marchDirection=%i ***\n",
	   i1Shift,i2Shift,marchDirection);

    // To guess the correct value for marchDirection we 
    //   1) take a point on the boundary x_b with tangent tv (tangent to the boundary curve)
    //   2) take a small step in the marching direction x_m = x_b+ eps*mv
    //   3) project x_m onto the surface (adjusting for corners) -> xp and get normal to surface nv
    //   4) marchingDirection = +/-  (tv X nv) dot (xp-x_b)

    RealArray xb(1,3), xp(1,3);
    real tv[3], mv[3];
    
    const int ib =numberOfPoints/2;        // check this point
    // ib = i1-base + I1.getLength()*( i2-i2Base )
    const int i2=ib/I1.getLength()+I2.getBase();
    const int i1=ib+I1.getBase()-I1.getLength()*(i2-I2.getBase());
    const int i3=i3p;
    // printf(" ib=%i, i1=%i, i2=%i, I1=[%i,%i] I2=[%i,%i]\n",ib,i1,i2,I1.getBase(),
    //    I1.getBound(),I2.getBase(),I2.getBound());
    

    assert(ib<numberOfPoints-1);
    xb(0,xAxes)=xx(ib,xAxes);
    int axis;
    for( axis=0; axis<3; axis++ )
      tv[axis]=xx(ib+1,axis)-xx(ib,axis);  // tangent to boundary curve -- this uses the projected points ??

    real nDist=.01*sqrt( tv[0]*tv[0]+tv[1]*tv[1]+tv[2]*tv[2] );  // use to get marching distance
    assert( nDist>0. );
    
    MappingProjectionParameters mpParams2;
    mpParams2.setAdjustForCornersWhenMarching(true);
    
    RealArray & n2 = mpParams2.getRealSerialArray(MappingProjectionParameters::normal);
    n2.redim(1,rangeDimension);

    xp=xb;
    matchingMapping.project(xp,mpParams2);  // project boundary point
    
    // compute a marching normal since this has not yet been determined (normal==0)
    real nv[3];
    nv[0] = ( (x(i1+1,i2,i3,1)-x(i1,i2,i3,1))*(x(i1,i2+1,i3,2)-x(i1,i2,i3,2))-
	      (x(i1+1,i2,i3,2)-x(i1,i2,i3,2))*(x(i1,i2+1,i3,1)-x(i1,i2,i3,1)) );
    nv[1] = ( (x(i1+1,i2,i3,2)-x(i1,i2,i3,2))*(x(i1,i2+1,i3,0)-x(i1,i2,i3,0))-
	      (x(i1+1,i2,i3,0)-x(i1,i2,i3,0))*(x(i1,i2+1,i3,2)-x(i1,i2,i3,2)) );
    nv[2] = ( (x(i1+1,i2,i3,0)-x(i1,i2,i3,0))*(x(i1,i2+1,i3,1)-x(i1,i2,i3,1))-
	      (x(i1+1,i2,i3,1)-x(i1,i2,i3,1))*(x(i1,i2+1,i3,0)-x(i1,i2,i3,0)) );
    
    real nNorm=marchingDirection/max(REAL_EPSILON*100.,sqrt( nv[0]*nv[0]+nv[1]*nv[1]+nv[2]*nv[2] ));
    nv[0]*=nNorm;
    nv[1]*=nNorm;
    nv[2]*=nNorm;

    //     mv[0]=tv[1]*n2(0,2)-tv[2]*n2(0,1);
    //     mv[1]=tv[2]*n2(0,0)-tv[0]*n2(0,2);
    //     mv[2]=tv[0]*n2(0,1)-tv[1]*n2(0,0);
    for( axis=0; axis<3; axis++ )
      xp(0,axis)=xb(0,axis)+nDist*nv[axis];

    if( debug & 2 )
    {
      printf("    :marching normal=(%8.2e,%8.2e,%8.2e) ...after project boundary n2=(%8.2e,%8.2e,%8.2e) \n",
	     nv[0],nv[1],nv[2],n2(0,0),n2(0,1),n2(0,2));
      printf("    :Before project: xb=(%8.2e,%8.2e,%8.2e)\n",xb(0,0),xb(0,1),xb(0,2));
      printf("    :Before project: xp=(%8.2e,%8.2e,%8.2e)\n",xp(0,0),xp(0,1),xp(0,2));
    }
    // *** the cs.project with triangulation does not march around corners yet so do a global search:
    // this will not work if there is too sharp a corner in the marching direction.

    intArray & subSurfaceIndex = mpParams2.getIntArray(MappingProjectionParameters::subSurfaceIndex);
    intArray & elementIndex = mpParams2.getIntArray(MappingProjectionParameters::elementIndex);
    subSurfaceIndex=-1;  // force a global search
    elementIndex=-1;

    matchingMapping.project(xp,mpParams2);  // project nearby point ** does this correct for corners?

    // printf("After  project: xp=(%8.2e,%8.2e,%8.2e)\n",xp(0,0),xp(0,1),xp(0,2));

    real dot =((xp(0,0)-xb(0,0))*( tv[1]*n2(0,2)-tv[2]*n2(0,1))+
               (xp(0,1)-xb(0,1))*( tv[2]*n2(0,0)-tv[0]*n2(0,2))+
	       (xp(0,2)-xb(0,2))*( tv[0]*n2(0,1)-tv[1]*n2(0,0)) );

    if( debug & 2 )
      printf("    :marchDirection=%i dot = %8.2e  ****** tv=(%8.2e,%8.2e,%8.2e) n2=(%8.2e,%8.2e,%8.2e) "
             "dx=(%8.2e,%8.2e,%8.2e) \n", marchDirection,dot,tv[0],tv[1],tv[2],
	     n2(0,0),n2(0,1),n2(0,2),xp(0,0)-xb(0,0),xp(0,1)-xb(0,1),xp(0,2)-xb(0,2));

    marchDirection= dot>0. ? 1 : -1;
    
    correctProjectionOfInitialCurve(xx,xr,(CompositeSurface&)matchingMapping,marchDirection,mpParams);

    //     // now project again to get the correct normals
    //     xx=xSave;
    //     matchingMapping.project(xx,mpParams);

    if( debug & 2 )
    {
      CompositeTopology *compositeTopology = ((CompositeSurface&)matchingMapping).getCompositeTopology();
      assert( compositeTopology!=NULL );
      assert( compositeTopology->getTriangulation()!=NULL );
      UnstructuredMapping & uns=*compositeTopology->getTriangulation();

      const intArray & elementSurface = uns.getTags();

      const intArray & elementIndex = mpParams.getIntArray(MappingProjectionParameters::elementIndex);
      for( int i=0; i<numberOfPoints; i++ )
      {
        int e=elementIndex(i);
	int s=elementSurface(e);
	printf("    :after correct project: i=%i projected onto e=%i s=%i\n",i,e,s);
      }
    }
  } // end project volume grid
  // ==============================================================================================

  //  n.reshape(I1,I2,1,xAxes);
  xx.reshape(I1,I2,1,xAxes);

  if( debug & 2 )
  {
    if( domainDimension==2 && rangeDimension==3 )
    {
      fprintf(debugFile,"    :after project onto the boundary xx=(%10.4e,%10.4e,%10.4e)\n",
	      xx(I1.getBase(),I2.getBase(),0,0),xx(I1.getBase(),I2.getBase(),0,1),xx(I1.getBase(),I2.getBase(),0,2));
    }
    else
      ::display(xx,"    :xx after project onto the boundary ",debugFile,"%10.4e ");
  }
  

  
  if( debug & 2 )
  {
    n.reshape(I1,I2,1,xAxes);
    int i1=I1.getBase(), i2=I2.getBase();

    if( initialStep )
    {
      ///  -----   ESTIMATE THE MARCHING DIRECTION (normal) ------
      // --- The normal has not been yet set on the inital step ------
      //

      // ** TEST *** 2015/11/17 
      //   ---  The marching-vector is not set at the initial step --
      // printF("--HYP-- matchToCurve: TEMPORARY TEST Set marching direction to follow boundary *FIX ME*\n");
      if( domainDimension==2 && rangeDimension==2 )
      {
        // In 2D n holds the tangent to the boundary condition curve I think
        normal(I1,I2,0,xAxes)=n(I1,I2,0,xAxes);
      }
      else if( domainDimension==3 && rangeDimension==3 )
      {
        // In 3D n holds the normal to the boundary condition surface I think

        // We need normal : an estimated vector in the marching direction 

	if( debug & 2 )
	{
	  fprintf(debugFile," --HYP-- matchToCurve: I1=[%i,%i] I2=[%i,%i] i3p=%i\n",I1.getBase(),I1.getBound(),
		  I2.getBase(),I2.getBound(),i3p);
           fprintf(debugFile," --HYP-- matchToCurve: normal = marching-vector =(%7.1e,%7.1e,%7.1e)\n",
            normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
	   // ::display(x," --HYP-- matchToCurve: x ",debugFile,"%10.4e ");
	}
        // ---------------------------------------------
        // --- compute a normal to the start surface ---
        //   The normal has not been set on the initial step 
        //
	const int i3=i3p;
	for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )	
	for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )	
	{
	  // compute a marching normal since this has not yet been determined (normal==0)
          // eval normal at point (j1,j2,j3) 
          const int j1 = min( i1, x.getBound(0)-1);
          const int j2 = min( i2, x.getBound(1)-1);
          const int j3=i3;
	  
	  real nv[3];
	  nv[0] = ( (x(j1+1,j2,j3,1)-x(j1,j2,j3,1))*(x(j1,j2+1,j3,2)-x(j1,j2,j3,2))-
		    (x(j1+1,j2,j3,2)-x(j1,j2,j3,2))*(x(j1,j2+1,j3,1)-x(j1,j2,j3,1)) );
	  nv[1] = ( (x(j1+1,j2,j3,2)-x(j1,j2,j3,2))*(x(j1,j2+1,j3,0)-x(j1,j2,j3,0))-
		    (x(j1+1,j2,j3,0)-x(j1,j2,j3,0))*(x(j1,j2+1,j3,2)-x(j1,j2,j3,2)) );
	  nv[2] = ( (x(j1+1,j2,j3,0)-x(j1,j2,j3,0))*(x(j1,j2+1,j3,1)-x(j1,j2,j3,1))-
		    (x(j1+1,j2,j3,1)-x(j1,j2,j3,1))*(x(j1,j2+1,j3,0)-x(j1,j2,j3,0)) );
    
	  real nNorm=marchingDirection/max(REAL_EPSILON*100.,sqrt( nv[0]*nv[0]+nv[1]*nv[1]+nv[2]*nv[2] ));
	  nv[0]*=nNorm;
	  nv[1]*=nNorm;
	  nv[2]*=nNorm;
	  for( int dir=0; dir<rangeDimension; dir++ )
	    normal(i1,i2,i3,dir)=nv[dir];

	  if( debug & 2 )
	    fprintf(debugFile," --HYP-- matchToCurve: (i1,i2,i3)=(%i,%i,%i) normal=[%9.2e,%9.2e,%9.2e)\n",
		    i1,i2,i3,nv[0],nv[1],nv[2]);
	}
	
	// THIS IS REALLY A KLUDGE
        // normal(I1,I2,0,0)= n(I1,I2,0,1);
        // normal(I1,I2,0,1)=-n(I1,I2,0,0);
      }
      
    }

    fprintf(debugFile,"    : marchingDirection=%i: tangent to BC Mapping=(%7.1e,%7.1e,%7.1e), "
                "marching-vector =(%7.1e,%7.1e,%7.1e)\n",
	    marchingDirection,n(i1,i2,0,0),n(i1,i2,0,1),n(i1,i2,0,2),
            normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
    n.reshape(numberOfPoints,xAxes);  // reset the shape.

  }

  // *wdh* 2015/11/17 : TRY THIS: apply BC's to start curve too --  DOESN'T WORK ??
  // 
  if( true )
  {
   bool adjustStartCurve = applyBoundaryConditionsToStartCurve && initialStep; // *wdh* 2015/11/17 
   if( option==1 && !adjustStartCurve )
   {
     if( debug & 2 )
       fprintf(debugFile," >>>>Leaving matchToCurve (option=1 && !adjustStartCurve : do not project)\n");
     return 0;
   }
  }
  else
  {
    // *OLD WAY*
    // option=1: Do not change the point
    if( option==1 )
    {
      if( debug & 2 )
	fprintf(debugFile," >>>>Leaving matchToCurve (option=1 : do not project)\n");
      return 0;
    }
    
  }
  
  x(I1,I2,i3p,xAxes)=xx(I1,I2,0,xAxes);

  // project the marching-normal to be parallel to the boundary
  // Given n = normal to the boundary condition mapping,
  // subtract off the component of `normal' in the direction of n
  // finally renormalize
  // printf("********* applyBC: adjust normals on (side,axis)=(%i,%i) onto a Mapping\n",side,axis); 

  RealArray nDot(I1,I2);

  // mpParams.n holds tangent to the BC mapping
  projectNormalsToMatchCurve(matchingMapping, mpParams,I1,I2,normal,nDot );
  
  if( surfaceGrid )
  {
    // Set the surface-normal in xr to be consistent with the new marching-normal
    //     surface normal = tangent-to-curve X marching-normal
    int i1=I1.getBase(), i2=I2.getBase();

    if( debug & 2 )
    {
      fprintf(debugFile,"    : marchingDir=%i:  corrected marching-vector (normal)=(%7.1e,%7.1e,%7.1e)\n",
	      marchingDirection,normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
      fprintf(debugFile,"    : surface-normal at boundary before correction = (%7.1e,%7.1e,%7.1e)\n",
	      xr(i1,i2,0,0,1),xr(i1,i2,0,1,1),xr(i1,i2,0,2,1));
	  
      fprintf(debugFile,"   :i1=%i, i2=%i, i1Shift=%i, x(i1,i2,i3p,0)=%e, x(i1+i1Shift,i2,i3p,0)=%e\n",
	      i1,i2,i1Shift,x(i1,i2,i3p,0),x(i1+i1Shift,i2,i3p,0));
	    
    }
	  
    int i1m=i1-1, i1p=i1+1;
    if( i1Shift>0 )
    { // left side
      i1m=i1;
    }
    else if( i1Shift<0 )
    { // right side
      i1p=i1;
    }

// 	    xr(i1,i2,0,0,1)=((x(i1,i2,i3p,1)-x(i1+is[0],i2,i3p,1))*normal(i1,i2,0,2)-
// 			     (x(i1,i2,i3p,2)-x(i1+is[0],i2,i3p,2))*normal(i1,i2,0,1));

    xr(i1,i2,0,0,1)=((x(i1p,i2,i3p,1)-x(i1m,i2,i3p,1))*normal(i1,i2,0,2)-
		     (x(i1p,i2,i3p,2)-x(i1m,i2,i3p,2))*normal(i1,i2,0,1));
    xr(i1,i2,0,1,1)=((x(i1p,i2,i3p,2)-x(i1m,i2,i3p,2))*normal(i1,i2,0,0)-
		     (x(i1p,i2,i3p,0)-x(i1m,i2,i3p,0))*normal(i1,i2,0,2));
    xr(i1,i2,0,2,1)=((x(i1p,i2,i3p,0)-x(i1m,i2,i3p,0))*normal(i1,i2,0,1)-
		     (x(i1p,i2,i3p,1)-x(i1m,i2,i3p,1))*normal(i1,i2,0,0));

    real normInverse=1./max(REAL_MIN, SQRT( SQR(xr(i1,i2,0,0,1)) +SQR(xr(i1,i2,0,1,1)) +SQR(xr(i1,i2,0,2,1)) ) );
          
    // normInverse*=marchingDirection*i1Shift;  // **** fix
    normInverse*=-marchingDirection;

    xr(i1,i2,0,0,1)*=normInverse;   // normalize
    xr(i1,i2,0,1,1)*=normInverse;   // normalize
    xr(i1,i2,0,2,1)*=normInverse;   // normalize
	  
    if( i1Shift!=0 )
      xr(i1-i1Shift,i2,0,xAxes,1)=xr(i1,i2,0,xAxes,1);  // set surface-normal at ghost line too

    if( debug & 2 )
    {
      fprintf(debugFile,"    :surface-normal at boundary after correction = (%7.1e,%7.1e,%7.1e)\n",
	      xr(i1,i2,0,0,1),xr(i1,i2,0,1,1),xr(i1,i2,0,2,1));
    }
  }  // end if surfaceGrid
  
  // ::display(normal(I1,I2,0,xAxes),"applyBC boundary normal(Ib1,Ib2,0,xAxes)",debugFile,"%10.3e ");

  if( projectBoundary )
  {
    // assign ghost points : tangential component extrapolated, normal component equal to first line in
    // this will cause the points to be nearly orthogonal
    // is[axis]=1-2*side;

    x(I1-i1Shift,I2-i2Shift,i3p,xAxes)=2.*x(I1,I2,i3p,xAxes)-x(I1+i1Shift,I2+i2Shift,i3p,xAxes);

    nDot = rangeDimension==2 ?
      (x(I1+i1Shift,I2+i2Shift,i3p,axis1)-x(I1-i1Shift,I2-i2Shift,i3p,axis1))*normal(I1,I2,0,axis1)+
      (x(I1+i1Shift,I2+i2Shift,i3p,axis2)-x(I1-i1Shift,I2-i2Shift,i3p,axis2))*normal(I1,I2,0,axis2)
      : 
      (x(I1+i1Shift,I2+i2Shift,i3p,axis1)-x(I1-i1Shift,I2-i2Shift,i3p,axis1))*normal(I1,I2,0,axis1)+
      (x(I1+i1Shift,I2+i2Shift,i3p,axis2)-x(I1-i1Shift,I2-i2Shift,i3p,axis2))*normal(I1,I2,0,axis2)+
      (x(I1+i1Shift,I2+i2Shift,i3p,axis3)-x(I1-i1Shift,I2-i2Shift,i3p,axis3))*normal(I1,I2,0,axis3) ;

    const real blendingFactor=ortho*2.;  // make orthogonal if ortho=.5, just extrap if ortho=0.
    
    for( int dir=0; dir<rangeDimension; dir++ )
      x(I1-i1Shift,I2-i2Shift,i3p,dir)+=nDot*normal(I1,I2,0,dir)*blendingFactor;

    if( debug & 2 )
    {
      int i1=I1.getBase()-i1Shift;
      int i2=I2.getBase()-i2Shift;
      if( domainDimension==2 && rangeDimension==3 )
        fprintf(debugFile,"    :assign ghost points x=(%8.2e,%8.2e,%8.2e)\n",
		x(i1,i2,i3p,0),x(i1,i2,i3p,1),x(i1,i2,i3p,2));
    }
    
  }
  
  if( debug & 2 )
    fprintf(debugFile," >>>>Leaving matchToCurve (at end)\n");

  return 0;
}


/*!
   Project the marching-normal to be parallel to the boundary
   Given n = normal to the boundary condition mapping (or tangent to BC curve),
   subtract off the component of `normal' in the direction of n
   finally renormalize
   /param mpParams.n (input) : matching surface normal or matching curve tangent.
   /param normal (output): the new un-normalized marching direction.
   */
int HyperbolicMapping::
projectNormalsToMatchCurve(Mapping & matchingMapping,
                           MappingProjectionParameters & mpParams,
                           Index & I1, Index & I2,
                           RealArray & normal, RealArray & nDot )
{
  if( !projectNormalsOnMatchingBoundaries )  // *wdh* 081102
    return 0;

  Range xAxes(0,rangeDimension-1);
  
  RealArray & n = mpParams.getRealSerialArray(MappingProjectionParameters::normal);

  const int numberOfPoints=I1.getLength()*I2.getLength();
  if( n.getLength(0)!=numberOfPoints )
    return 0;

  n.reshape(I1,I2,1,xAxes);
	
  if( rangeDimension==2 )
    nDot=n(I1,I2,0,axis1)*normal(I1,I2,0,axis1)+n(I1,I2,0,axis2)*normal(I1,I2,0,axis2);
  else
    nDot=(n(I1,I2,0,axis1)*normal(I1,I2,0,axis1)+
	  n(I1,I2,0,axis2)*normal(I1,I2,0,axis2)+
	  n(I1,I2,0,axis3)*normal(I1,I2,0,axis3));

	

  if( matchingMapping.getDomainDimension()==2 ) 
  {
    if( debug & 2 )
    {
      if( debugFile==NULL ) debugFile = fopen("hype.debug","w" );

      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
      {
	
	fprintf(debugFile,".....projectNormals: i1=%i,i2=%i normal to BC Map=(%5.2f,%5.2f,%5.2f) nDot=%8.2e "
		"normal=(%5.2f,%5.2f,%5.2f)\n",i1,i2,
		n(i1,i2,0,0),n(i1,i2,0,1),n(i1,i2,0,2),nDot(i1,0,0),
                normal(i1,i2,0,0),normal(i1,i2,0,1),normal(i1,i2,0,2));
      }
    }

    // if( false ) // ****************************************************************** 081101
    for( int dir=0; dir<rangeDimension; dir++ )
      normal(I1,I2,0,dir)-=nDot*n(I1,I2,0,dir);
  }
  else
  {
    // if the boundaryCondition mapping is a curve the "normal" is actually the tangent.
    // Use +/- the tangent and renormalize
    if( debug & 2 )
    {
      if( debugFile==NULL ) debugFile = fopen("hype.debug","w" );

      int i1=I1.getBase(), i2=I2.getBase();
      fprintf(debugFile,"....projectNormalsToMatchCurve tangent to BC Mapping=(%7.1e,%7.1e,%7.1e) nDot=%8.2e\n",
	      n(i1,i2,0,0),n(i1,i2,0,1),n(i1,i2,0,2),nDot(i1,0,0));
    }

    // *wdh* temp fix -- do not adjust where marching-vector dot curve-tangent is small
    //    since this could mean that we have turned a corner and the marching vector is wrong
    where( fabs(nDot)>.2 )  // *wdh* temp fix 
    {
      for( int dir=0; dir<rangeDimension; dir++ )
	normal(I1,I2,0,dir)=nDot*n(I1,I2,0,dir);
    }
    
  }	  

  n.reshape(numberOfPoints,xAxes); // reset the shape.

  return 0;
}
