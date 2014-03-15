#include "GenericGraphicsInterface.h"
#include "EllipticGridGenerator.h"
#include "display.h"
#include "LineMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "DataPointMapping.h"
#include "TridiagonalSolver.h"
#include "StretchedSquare.h"
#include "CompositeSurface.h"

#include "Overture.h"
#include "MappedGridOperators.h"

// extern realMappedGridFunction   Overture::nullRealMappedGridFunction();

EllipticGridGenerator::
EllipticGridGenerator()
// =====================================================================================
/// \details 
///    Default constructor.
/// 
// =====================================================================================
{
  //Default constructor
  initializeParameters();
  

}

int EllipticGridGenerator::
initializeParameters()
{
  debugFile = fopen("elliptic.debug","w" );      // Here is the debug file

  userMap=NULL;
  projectionMap=NULL;
  for( int axis=0; axis<3; axis++ )
    for( int side=Start; side<=End; side++ ) 
      boundaryProjectionMap[side][axis]=NULL;
  
  debug=0;
  ps=0;
  work=0.;
  useNewStuff=TRUE;

  map=NULL;
  mg=NULL;
  operators=NULL;
  u=NULL;
  rhs=NULL;
  source=NULL;
  w=NULL;
  rBoundary=NULL;
  xBoundary=NULL;
  tridiagonalSolver=NULL;
  pWeight=NULL;
  
  controlFunctions=FALSE;
  numberOfPointsOfAttraction=0;
  numberOfLinesOfAttraction=0;
  controlFunctionComputed=FALSE;
  applyBoundarySourceControlFunction=FALSE;
  normalCurvatureWeight=0.;
  userWeightFunctionDefined=FALSE;
  
  numberOfPeriods=0;
  residualTolerance=1.e-3;  // relative convergence criteria for the maximum residual
  maximumNumberOfIterations=10;
  smoothingMethod=jacobiSmooth;

  smootherNames[0]="jacobi";
  smootherNames[1]="red black";
  smootherNames[2]="line";
  smootherNames[3]="line-direction1";
  smootherNames[4]="line-direction2";
  smootherNames[5]="line-direction3";
  smootherNames[6]="zebra";
  
  useBlockTridiag=0;

  boundarySpacing.redim(2,3);
  boundarySpacing=-1.;
  
  maximumResidual=previousMaximumResidual=0.;
  residualNormalizationFactor=1.;
  
  return 0;
}


const RealMappedGridFunction & EllipticGridGenerator::
solution() const
// ==========================================================================================
/// \details 
///      Return a reference to the current solution.
// ==========================================================================================
{
  if( u!=NULL )
    return u[0];
  else
    return Overture::nullRealMappedGridFunction();
}

  

void EllipticGridGenerator::
setup(Mapping & mapToUse, 
      Mapping *projectionMappingToUse /* =NULL */ )
// ==========================================================================================
/// \details 
///      Setup the EllipticGridGenerator
/// \param mapToUse: This mapping defines the ...
// ==========================================================================================
{
  userMap=&mapToUse;
  if( projectionMappingToUse!=NULL )
    projectionMap=projectionMappingToUse;
  else
    projectionMap=&mapToUse;
  
  domainDimension=userMap->getDomainDimension();
  rangeDimension=userMap->getRangeDimension();

  int axis;
  for( axis=0; axis<3; axis++ )
    for( int side=Start; side<=End; side++ ) 
      boundaryProjectionMap[side][axis]=userMap;

  if( domainDimension==2 && rangeDimension==3 )
    boundaryProjectionMap[Start][axis3]=projectionMap;

  maximumResidual=previousMaximumResidual=0.;

  Rr=Range(0,domainDimension-1);
  Rx=Range(0,rangeDimension-1);
  if( rangeDimension==1)
     omega=2.0/3.0;
  else if( rangeDimension==2 )
     omega=4.0/5.0;
  else 
    omega=4.0/5.0;   // **** what should this be ?? 0.5;

  maximumNumberOfLevels=1;
  int dim[3] ={1,1,1};
  for( axis=0; axis<domainDimension; axis++ )
  {
    dim[axis]=userMap->getGridDimensions(axis);
    int num=dim[axis]-1;
    int numLevels=1;
    while( num>2 && num % 2 ==0 )  // keep at most 2 cells on the finest grid.
    {
      num/=2;
      numLevels++;
    }
    maximumNumberOfLevels=axis==0 ? numLevels : min(maximumNumberOfLevels,numLevels);
  }
  numberOfLevels= maximumNumberOfLevels;

  // map : pointer to the unit line, unit square or unit cube mapping.
  delete map;
  if( domainDimension==1 )
    map= new LineMapping();
  else if( domainDimension==2 ) 
    map = new SquareMapping();
  else  
    map = new BoxMapping();

  // *** only delete if not enough !! *****************************
  delete [] mg;
  mg    = new MappedGrid[numberOfLevels]; 
  delete [] operators;
  operators = new MappedGridOperators[numberOfLevels];
  delete []u;
  u     = new realMappedGridFunction[numberOfLevels];
  delete [] rhs;
  rhs   = new realMappedGridFunction[numberOfLevels];  
  delete [] w;
  w     = new realMappedGridFunction[numberOfLevels];

  delete [] source;
  source= new realMappedGridFunction[numberOfLevels];
  controlFunctionComputed=FALSE;

  delete [] rBoundary;
  rBoundary = new realMappedGridFunction[numberOfLevels];    // ****** these are only needed for slip-orthogonal
  delete [] xBoundary;
  xBoundary = new realMappedGridFunction[numberOfLevels];

  dx.redim(3,numberOfLevels);
  dx=1.;

  boundaryCondition.redim(2,3);
  boundaryCondition=dirichlet;

  numberOfCoefficients = rangeDimension==1 ? 1 : rangeDimension==2 ? 3 : 6;

  Range all;
  // Initialize working arrays
  gridIndex.redim(2,3,numberOfLevels);
  gridIndex=0;
  
  for( axis=0; axis<domainDimension; axis++ )
  {
    map->setGridDimensions(axis,dim[axis]);
    if( userMap->getIsPeriodic(axis) )
    {
      map->setIsPeriodic(axis, userMap->getIsPeriodic(axis));
      boundaryCondition(0,axis)=-1;
      boundaryCondition(1,axis)=-1;
    }
  }
  // for surface grid generation we use a slip-orthogonal BC
  if( domainDimension==2 && rangeDimension==3 )
    boundaryCondition(Start,axis3)=slipOrthogonal;

  // initialize variables at each multigrid level.
  int level;
  for( level=0; level<numberOfLevels; level++ )
  {
     
    for( axis=0; axis<domainDimension; axis++ )
    {
      const int numberOfGridPoints=int( (dim[axis]-1)/pow(2,level)+1 );
      map->setGridDimensions(axis,numberOfGridPoints);
    }
    mg[level]=MappedGrid(*map);
    mg[level].update();                                      // ******* should not be repeated *******

    
    // display(mg[level].vertex(),sPrintF(buff,"mg[%i].vertex()",level));
    
    u[level].updateToMatchGrid(mg[level],all,all,all,rangeDimension);
    u[level]=0.0;

    operators[level].updateToMatchGrid(mg[level]);
    u[level].setOperators(operators[level]);

    if( level==0 && pWeight!=NULL )
      pWeight->setOperators(operators[level]);

/* -----    
    // gridIndex : interior points plus boundaries where interior equations are applied.
    for( axis=0; axis<domainDimension; axis++ )
    {
      if( userMap->getIsPeriodic(axis) )
      {
        gridIndex(Start,axis,level)=mg[level].gridIndexRange(Start,axis);
        gridIndex(End  ,axis,level)=mg[level].gridIndexRange(End  ,axis);
      }
      else
      {
        gridIndex(Start,axis,level)=mg[level].gridIndexRange(Start,axis)+1;
        gridIndex(End  ,axis,level)=mg[level].gridIndexRange(End  ,axis)-1;
      }
    }
---- */    

    source[level].updateToMatchGrid(mg[level],all,all,all,rangeDimension);
    source[level]=0.0;
    if( level==0 )
      source[level].setOperators(operators[level]);

    rhs[level].updateToMatchGrid(mg[level],all,all,all,rangeDimension);
    rhs[level]=0.0;
    if( level>0 )
    {
      w[level].updateToMatchGrid(mg[level],all,all,all,rangeDimension);
      w[level]=0.0;
    }
    for( int axis=0; axis<rangeDimension; axis++ )
      dx(axis,level)=mg[level].gridSpacing(axis);

  }

  updateForNewBoundaryConditions();

  // Here we hold the maximum allowable omega (as determined from the control functions)
  omegaMax.redim(3,maximumNumberOfLevels);
  omegaMax=2.;


  if( domainDimension<rangeDimension )
    numberOfLevels=1;   // ******* trouble with surfaces for some reason ********************

}

int EllipticGridGenerator::    
updateForNewBoundaryConditions()
// ==========================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Update some variables that depend on the boundary consitions.
///  gridIndex : interior points plus boundaries where interior equations are applied.
/// 
//===========================================================================
{
  int side,axis,level;
  
  applyBoundarySourceControlFunction=FALSE;
  for( axis=0; axis<domainDimension; axis++ )
  {
    if( boundaryCondition(Start,axis)==noSlipOrthogonalAndSpecifiedSpacing ||
	boundaryCondition(End  ,axis)==noSlipOrthogonalAndSpecifiedSpacing )
    {
      applyBoundarySourceControlFunction=TRUE;
      break;
    }
  }
  // initialize variables at each multigrid level.
  for( level=0; level<maximumNumberOfLevels; level++ )
  {
    for( axis=0; axis<domainDimension; axis++ )
    {
      for( side=Start; side<=End; side++ )
        mg[level].setBoundaryCondition(side,axis,boundaryCondition(side,axis));
    }
    // gridIndex : interior points plus boundaries where interior equations are applied.
    for( axis=0; axis<domainDimension; axis++ )
    {
      for( side=Start; side<=End; side++ )
      {
	if( boundaryCondition(side,axis)==freeFloating || boundaryCondition(side,axis)<0 )
	  gridIndex(side,axis,level)=mg[level].gridIndexRange(side,axis);   // include boundary for equation.
	else
	  gridIndex(side,axis,level)=mg[level].gridIndexRange(side,axis)+(1-2*side);
      }
    }
  }
  return 0;
}


EllipticGridGenerator::
~EllipticGridGenerator()
{
  fclose(debugFile);

  delete map;

  delete [] mg;
  delete [] operators;
  delete [] u;
  delete [] rhs;
  delete [] source;
  delete [] w;
  delete [] rBoundary;
  delete [] xBoundary;

  delete pWeight;
  delete tridiagonalSolver;
}

realArray EllipticGridGenerator::
dot( const realArray & a, 
     const realArray & b, 
     const Index & I1 /* =nullIndex */, 
     const Index & I2 /* =nullIndex */, 
     const Index & I3 /* =nullIndex */ )
// ===============================================================
// /Description:
//    Compute the dot product of a and b
// ===============================================================
{
  if( rangeDimension==0 )
    return evaluate(a(I1,I2,I3,0)*b(I1,I2,I3,0));
  else if( rangeDimension==2 )
    return evaluate(a(I1,I2,I3,0)*b(I1,I2,I3,0)+a(I1,I2,I3,1)*b(I1,I2,I3,1));
  else
    return evaluate(a(I1,I2,I3,0)*b(I1,I2,I3,0)+a(I1,I2,I3,1)*b(I1,I2,I3,1)+a(I1,I2,I3,2)*b(I1,I2,I3,2));
}


int EllipticGridGenerator::
restrictMovement( const int & level,
		  const RealMappedGridFunction & u0, 
		  RealMappedGridFunction & u1,
		  const Index & I1_ /* =nullIndex */, 
		  const Index & I2_ /* =nullIndex */, 
		  const Index & I3_ /* =nullIndex */)               
// =================================================================================
//  /Description:
//    Restrict the movement of the grid points to move at most a factor
//  theta in the direction of any of the nearest neighbours along coordinate lines.
//
// /u0 (input) : the old grid points.
// /u1 (input/output) : the new grid points. On output these points will be constrained.
// =================================================================================  
{
  printf("Restrict movement at level=%i \n",level);
  
  Index I1,I2,I3;
  if( I1.getLength()>0 )
  {
    I1=I1_;
    I2=I2_;
    I3=I3_;
  }
  else
  {
    getIndex(mg[level].gridIndexRange(),I1,I2,I3);
  }

  realArray du(I1,I2,I3,Rx), du0(I1,I2,I3,Rx);
  
  du=u1(I1,I2,I3,Rx)-u0(I1,I2,I3,Rx);

  int axis,dir;
  int is[3], &is1=is[0], &is2=is[1], &is3=is[2];
  is1=is2=is3=0;
  
  const real theta=.5;  // move at most this fraction of a cell width.

  for( axis=0; axis<domainDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      is[axis]=1-2*side;
      du0 = evaluate(u0(I1+is1,I2+is2,I3+is3,Rx)-u0(I1,I2,I3,Rx));
      const realArray & duDotDu0=dot(du,du0);
      const realArray & du0DotDu0=dot(du0,du0);
      where( duDotDu0 > theta*du0DotDu0 )
      {
        const realArray & limit = evaluate(theta-duDotDu0/du0DotDu0);
	for( dir=0; dir<rangeDimension; dir++ )
	  du(I1,I2,I3,dir)=limit*du0(I1,I2,I3,dir);
      }
    }
    is[axis]=0;
  }
  u1(I1,I2,I3,Rx)=u0(I1,I2,I3,Rx)+du(I1,I2,I3,Rx);
  periodicUpdate(u1);
  
  return 0;
}


realArray EllipticGridGenerator::
signOf( const realArray & uarray)
{
  realArray u1;

  u1.redim(uarray);
  u1=0.0;
  where(uarray>0.0) u1=1.0;
  elsewhere(uarray<0.0) u1=-1.0;

  return u1;
}



int EllipticGridGenerator::
plot( const RealMappedGridFunction & v, const aString & label )
//===========================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Plot a grid function.
//===========================================================================
{
  if( debug & 2 )
    display(v,label,debugFile);

  if( ps!=NULL )
  {
    ps->erase();
    psp.set(GI_TOP_LABEL,label);
    PlotIt::contour(*ps,v,psp);
  }
  return 0;
}


int EllipticGridGenerator::
projectBoundaryPoints(const int & level,
		      RealMappedGridFunction & uu, 
		      const int & side,  
		      const int & axis,
		      const Index & I1,
		      const Index & I2,
		      const Index & I3 )
// ===================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Project boundary points onto the Mappings`s that define the actual boundary.
/// 
///  boundaryProjectionMap[2][3] : pointer to the mapping to use to project a point onto a boundary.
// ====================================================================================
{
  realArray & u1 = uu;
  realArray & rB = rBoundary[level];
  realArray & xB = xBoundary[level];
  
  assert( boundaryProjectionMap[side][axis] !=NULL );
  Mapping & project = *(boundaryProjectionMap[side][axis]);

  if( domainDimension==rangeDimension || projectionMap==userMap || axis!=axis3 )
  {
    realArray r2(I1,I2,I3,Rr), u2(I1,I2,I3,Rx);
    r2=rB(I1,I2,I3,Rr);
    if( domainDimension==rangeDimension || axis!=axis3 )
    {
      project.mapGrid(r2,u2);  
    }
    else
    {
      u2=u1(I1,I2,I3,Rx);
      project.inverseMapGrid(u2,r2);  
      project.mapGrid(r2,u2);  
      if( debug &2 )
      {
	if( domainDimension==rangeDimension )
	  printf("projectBoundaryPoints: level=%i, max(r2-rB)=%e \n",level,max(fabs(rB(I1,I2,I3,Rr)-r2)));
	else
	  printf("projectBoundaryPoints: project onto referance surf, level=%i, max(r2-rB)=%e \n",level,
		 max(fabs(rB(I1,I2,I3,Rr)-r2)));
      }
      rB(I1,I2,I3,Rr)=r2;
    }
    u1(I1,I2,I3,Rx)=u2;
    xB(I1,I2,I3,Rx)=u2;
  }
  else 
  {

    if( project.getClassName()!="CompositeSurface" )
    {
      printf("project onto a normal surface... \n");
      realArray r2(I1,I2,I3,Rr),u2(I1,I2,I3,Rx);
      u2=u1(I1,I2,I3,Rx);
      project.inverseMapGrid(u2,r2);   // find the unit square coord's of the closest point.
      project.mapGrid(r2,u2);
      u1(I1,I2,I3,Rx)=u2;
    }
    else 
    {
      printf("project onto a CompositeSurface...\n");

      // project onto a composite surface.
      CompositeSurface & cs = (CompositeSurface&)project;
      // for a composite surface we keep track of the previous sub-surface that a point was on.
      const int numberOfPoints = I1.getLength()*I2.getLength()*I3.getLength();
      Range R(0,numberOfPoints-1);
      if( subSurfaceIndex.getLength(0)!=numberOfPoints )
      {
	subSurfaceIndex.redim(R);
	subSurfaceIndex=-1;
	// for a composite surface subSurfaceNormal holds the previous normal on input to project
	subSurfaceNormal.redim(numberOfPoints,3);
	subSurfaceNormal=0.;           // what about the first time??
      }
      
      realArray r(R,Rr), xx(I1,I2,I3,Rx), xP(I1,I2,I3,Rx), xxr(R,Rx,Rr);
      r=0.;
      
      xx(I1,I2,I3,Rx)=u1(I1,I2,I3,Rx);  // project these points.
      xP(I1,I2,I3,Rx)=xB(I1,I2,I3,Rx);  // previous location.
      
      xx.reshape(R,Rx);
      xP.reshape(R,Rx);

      // project the point onto the composite surface 
#ifndef USE_PPP
      cs.project( subSurfaceIndex,xx,r,xP,xxr,subSurfaceNormal );
#else
      printf("ERROR:EllipticGridGenerator needs to be fixed for P++\n");
      throw "error";
      
//       cs.project( subSurfaceIndex,xx.getLocalArrayWithGhostBoundaries(),
// 		  r.getLocalArrayWithGhostBoundaries(),xP.getLocalArrayWithGhostBoundaries(),
// 		  xxr.getLocalArrayWithGhostBoundaries(),subSurfaceNormal.getLocalArrayWithGhostBoundaries() );
#endif
      
      xx.reshape(I1,I2,I3,Rx);
      xP.reshape(I1,I2,I3,Rx);
      u1(I1,I2,I3,Rx)=xx(I1,I2,I3,Rx);
      xB(I1,I2,I3,Rx)=xx(I1,I2,I3,Rx);

    }
  }
  return 0;
}


int EllipticGridGenerator::
applyBoundaryConditions(const int & level,
                      RealMappedGridFunction & uu )
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///     Apply boundary conditions. 
/// 
///   {\bf slip orthogonal boundary:}
/// 
///   Adjust the points on the boundary to make the grid orthogonal at the boundary.
///   To do this we compute the amount to shift the point in the unit square coordinates.
///   We then recompute the $\xv$ coordinates by evaluating the Mapping on the boundary.
/// 
///   Suppose that  $r,s$ are the coordinates tangential to the boundary and $t$ is the coordinate
///   normal to the boundary. Let $\xv_0$ be the grid point on the boundary that we
///  want to adjust and let $\xv_1$ be the grid point one line away from the boundary.
///   Then we want to choose a new boundary 
///  point $\xv(r,s)$ so that
///  \begin{align*}
///     (\xv-\xv_1) \cdot \xv_r &= 0 \\
///     (\xv-\xv_1) \cdot \xv_s &= 0
///  \end{align*}
///  We use the approximation
///  \[
///     \xv(r,s) \approx \xv_0 + \Delta r \xv_r^0 + \Delta s \xv_s^0 
///  \]
///  and thus the equation for $(\Delta r,\Delta s)$ is
///  \begin{align*}
///    \begin{bmatrix}  
///        \xv_r^0\cdot\xv_r^0 & \xv_r^0\cdot\xv_s^0 \\
///        \xv_r^0\cdot\xv_s^0 & \xv_s^0\cdot\xv_s^0 
///    \end{bmatrix}  
///    \begin{bmatrix} \Delta r \\ \Delta s \end{bmatrix} =
///    \begin{bmatrix} (\xv_1-\xv_0)\cdot \xv_r^0 \\ (\xv_1-\xv_0)\cdot \xv_s^0 \end{bmatrix}
///  \end{align*}
///  with solution
///  \begin{align*}
///    \begin{bmatrix} \Delta r \\ \Delta s \end{bmatrix} =
///    {1\over \xv_r^0\cdot\xv_r^0 \xv_s^0\cdot\xv_s^0 - (\xv_r^0\cdot\xv_s^0)^2 }
///     \begin{bmatrix}  
///        \xv_s^0\cdot\xv_s^0 &-\xv_r^0\cdot\xv_s^0 \\
///       -\xv_r^0\cdot\xv_s^0 & \xv_r^0\cdot\xv_r^0 
///    \end{bmatrix}  
///    \begin{bmatrix} (\xv_1-\xv_0)\cdot \xv_r^0 \\ (\xv_1-\xv_0)\cdot \xv_s^0 \end{bmatrix}
///  \end{align*}
///  In 2D this reduces to
///  \[
///   \Delta r = {1\over \xv_r^0\cdot\xv_r^0} (\xv_1-\xv_0)\cdot \xv_r^0
///  \]
/// 
// =================================================================================
{
  if( level>0 || rangeDimension<2 ) 
   return 0;

  realArray & u1 = uu;
  realArray & rB = rBoundary[level];
  realArray & xB = xBoundary[level];

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  uu.applyBoundaryCondition(Rx,BCTypes::extrapolate,BCTypes::allBoundaries);

  BoundaryConditionParameters extrapParams;
  extrapParams.orderOfExtrapolation=2;   // u_{-1} = 2 u_0 - u_1
  uu.applyBoundaryCondition(Rx,BCTypes::extrapolate,freeFloating,0.,0.,extrapParams); 


  int axis;
/* ----
  for( axis=0; axis<domainDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( boundaryCondition
--- */

  if( level>0 ) // *** only apply slip BC to level 0 *****
   return 0;

  // int is[3];   is[0]=is[1]=is[2]=0;
  const real omegaSlip=1.;  // under-relax slip orthogonal correction.

  bool applySlipOrthogonal = FALSE;
  for( axis=0; axis<rangeDimension; axis++ )
    applySlipOrthogonal = applySlipOrthogonal || 
                       boundaryCondition(0,axis)==slipOrthogonal || 
                       boundaryCondition(1,axis)==slipOrthogonal;

  if( applySlipOrthogonal )
  {
    // ****Note***** this boundary condition is also used to project a surface
    //  grid back onto the original surface.

    // printf("ERROR: applyOrthogonalSlipBC not implemented yet\n");

    Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];
    
    for( axis=0; axis<rangeDimension; axis++ )
    {
      const int axisp1 = (axis+1) % domainDimension;  // this works even for 3D surfaces.
      const int axisp2 = (axis+2) % domainDimension;
      // is[axisp1]=1;
      for( int side=Start; side<=End; side++ )
      {
        if( boundaryCondition(side,axis)==slipOrthogonal )
	{
           // do not adjust end points. These must remain at 0. and 1.
          const int extra=domainDimension==rangeDimension ? -1 : 0;
	  getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3,extra);
          getGhostIndex(mg[level].gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1,extra); // first line inside
	  
          realArray du(I1,I2,I3,Rx);
          if( domainDimension==rangeDimension || axis!=axis3 )
            du=u1(Ip1,Ip2,Ip3,Rx)-xB(I1,I2,I3,Rx);  // xB holds the old position of the boundary point.
          else // project a surface grid:
            du=u1(I1,I2,I3,Rx)-xB(I1,I2,I3,Rx);  // xB holds the old position of the boundary point.
	  realArray ur(I1,I2,I3,Rx); 
	  if( axisp1==0 )
	    ur=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	  else if( axisp1==1 )
	    ur=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	  else
	    ur=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
          if( rangeDimension==2 )
	  {
	    const realArray & urDotDu=evaluate( du(I1,I2,I3,0)*ur(I1,I2,I3,0)+du(I1,I2,I3,1)*ur(I1,I2,I3,1) );
	    const realArray & urDotUr=evaluate( SQR(ur(I1,I2,I3,0))+SQR(ur(I1,I2,I3,1)) );
	    realArray dr = evaluate(urDotDu/urDotUr);
	    // we need the current r values of the boundary points
/* -----
            where( dr>.5*(rB(I1+is1,I2+is2,I3,axisp1)-rB(I1,I2,I3,axisp1)) )
	    {
              dr=.5*(rB(I1+is1,I2+is2,I3,axisp1)-rB(I1,I2,I3,axisp1));
	    }
            where( dr<.5*(rB(I1-is1,I2-is2,I3,axisp1)-rB(I1,I2,I3,axisp1)) )
	    {
              dr=.5*(rB(I1-is1,I2-is2,I3,axisp1)-rB(I1,I2,I3,axisp1));
	    }
----- */
            if( debug & 4 )
              display(rBoundary[level](I1,I2,I3,axisp1),
                sPrintF(buff,"applyOrthogonalSlipBC: rBoundary[level] on (axis,side)=(%i,%i) level=%i",
                 axis,side,level));
	    rB(I1,I2,I3,axisp1)+=omegaSlip*dr;
            if( debug & 4 )
              display(dr,sPrintF(buff,"applyOrthogonalSlipBC: dr on (axis,side)=(%i,%i)",axis,side));

            if( debug & 4 )
              printf("slipOrthogonal BC: level=%i, (side,axis)=(%i,%i) max(dr)=%e\n",level,side,axis,max(fabs(dr)));
	    
	  }
	  else if( domainDimension==2 && rangeDimension==3 && axis!=axis3 )
	  {
            // move points on the "edge" of a surface grid.
	    const realArray & urDotDu=evaluate( du(I1,I2,I3,0)*ur(I1,I2,I3,0)+
                                          du(I1,I2,I3,1)*ur(I1,I2,I3,1)+
                                          du(I1,I2,I3,2)*ur(I1,I2,I3,2) );
	    const realArray & urDotUr=evaluate( SQR(ur(I1,I2,I3,0))+SQR(ur(I1,I2,I3,1))+SQR(ur(I1,I2,I3,2)) );
	    const realArray & dr = evaluate(urDotDu/urDotUr);

	    rB(I1,I2,I3,axisp1)+=omegaSlip*dr;  // ***************************


	  }
	  else if( domainDimension==3 || axis==axis3 )
	  {
            //  Project the face of a volume grid *OR* project the surface of a surface grid
            // Here we solve the system
            //    [ ur.ur  ur.us ] [ dr ] = [ ur.du ]
            //    [ ur.us  us.us ] [ ds ] = [ us.du ]
            // for dr and ds.
	    realArray us(I1,I2,I3,Rx); 
	    if( axisp2==0 )
	      us=uu.r1(I1,I2,I3,Rx)(I1,I2,I3,Rx);
	    else if( axisp2==1 )
	      us=uu.r2(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    else 
	      us=uu.r3(I1,I2,I3,Rx)(I1,I2,I3,Rx); // tangential derivative
	    const realArray & urDotDu=evaluate(du(I1,I2,I3,0)*ur(I1,I2,I3,0)+
					 du(I1,I2,I3,1)*ur(I1,I2,I3,1)+
					 du(I1,I2,I3,2)*ur(I1,I2,I3,2));
	    const realArray & usDotDu=evaluate(du(I1,I2,I3,0)*us(I1,I2,I3,0)+
					 du(I1,I2,I3,1)*us(I1,I2,I3,1)+
					 du(I1,I2,I3,2)*us(I1,I2,I3,2));
	    const realArray & urDotUr=evaluate( SQR(ur(I1,I2,I3,0)) + SQR(ur(I1,I2,I3,1)) + SQR(ur(I1,I2,I3,2)) );
	    const realArray & usDotUs=evaluate( SQR(us(I1,I2,I3,0)) + SQR(us(I1,I2,I3,1)) + SQR(us(I1,I2,I3,2)) );
	    const realArray & urDotUs=evaluate(ur(I1,I2,I3,0)*us(I1,I2,I3,0)+
                                         ur(I1,I2,I3,1)*us(I1,I2,I3,1)+
                                         ur(I1,I2,I3,2)*us(I1,I2,I3,2));
            const realArray & detInverse = evaluate(1./(urDotUr*usDotUs-SQR(urDotUs)));
	    const realArray & dr = evaluate((usDotUs*urDotDu-urDotUs*usDotDu)*detInverse);
	    const realArray & ds = evaluate((urDotUr*usDotDu-urDotUs*urDotDu)*detInverse);
	    
	    // we need the current r values of the boundary points
	    rB(I1,I2,I3,axisp1)+=omegaSlip*dr;
	    rB(I1,I2,I3,axisp2)+=omegaSlip*ds;

            if( FALSE )
	    {
	      display(u1(I1,I2,I3,Rx),"\nu1 to be projected",debugFile);
	      display(xB(I1,I2,I3,Rx),"\nxB previous boundary point",debugFile);
	      display(rB(I1,I2,I3,Rr),"\nrB previous unit square coordinates",debugFile);
	      display(du,"\ndu",debugFile);
	      display(dr,"\ndr",debugFile);
	      display(ds,"\nds",debugFile);
	    }
	    
	    if( debug & 2 && domainDimension==2 && rangeDimension==3 )
	      printf("projection BC: level=%i, (side,axis)=(%i,%i) max(dr)=%6.2e, max(ds)=%6.2e, max(du)=%6.2e\n",level,side,axis,
		     max(fabs(dr)),max(fabs(ds)),max(fabs(du)));

	  }
	  
          // we may have to put guards on rBoundary.
          if( debug & 4 )
  	    display(rBoundary[level](I1,I2,I3,Rr),
		  sPrintF(buff,"applyOrthogonalSlipBC: AFTER : rBoundary[level] on (axis,side)=(%i,%i) level=%i",
			  axis,side,level));

	  // userMap->mapGrid(rBoundary[level](I1,I2,I3,Rx),u2); // this doesn't work -- reshape view is wrong.

          projectBoundaryPoints(level,uu,side,axis,I1,I2,I3);
/* --------
	  if( domainDimension<rangeDimension && projectionMap!=userMap )
	  {
            printf("project the surface \n");
	    realArray r2(I1,I2,I3,Rr),u2(I1,I2,I3,Rx);
            u2=u1(I1,I2,I3,Rx);
	    projectionMap->inverseMapGrid(u2,r2);
	    projectionMap->mapGrid(r2,u2);
	    u1(I1,I2,I3,Rx)=u2;
	  }
	  else
	  {
	    realArray r2(I1,I2,I3,Rr), u2(I1,I2,I3,Rx);
	    r2=rB(I1,I2,I3,Rr);
	    if( domainDimension<rangeDimension && axis==axis3 )
	      projectionMap->mapGrid(r2,u2);  // this is the surface we project onto.
	    else
	      userMap->mapGrid(r2,u2);  
	    u1(I1,I2,I3,Rx)=u2;
	    xB(I1,I2,I3,Rx)=u2;
	  }
----------- */	  
	}
      } // end for side
      // is[axisp1]=0;  // reset
    }

    // *wdh* 990610 periodicUpdate(uu); // *****   can we only do a periodic update after applyBoundaryCondition ???
    uu.finishBoundaryConditions();
    
    

  }
  return 0;
}


int EllipticGridGenerator::
stretchTheGrid(Mapping & mapToStretch)
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
/// 
///    Determine a starting guess for grids with stretched boundary layer spacing.
/// 
///   If the user has requested a very small spacing near the boundary we can
///  can explicitly stretch the initial grid to approximately statisfy the grid spacing.
///  To do this we measure the actual grid spacing near each boundary that needs to be stretched.
///  We then determine use stretching functions to determine a new grid by composing a 
///  stretched
///  
///  
///  
///  
// =================================================================================
{
  if( rangeDimension<2 )
    return 0;
  
  const int level=0;
  //  RealMappedGridFunction & uu = u[level];
  // realArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

  realArray spacingRatio(2,3);
  spacingRatio=1.;
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
      {
	getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3);    // boundary line
	getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,-1); // first interior line
	  
	// determine the average current grid spacing
        real averageSpacing,minimumSpacing,maximumSpacing;
	determineBoundarySpacing( side, axis,averageSpacing,minimumSpacing,maximumSpacing );
	
/* ----
	realArray uDiff(I1,I2,I3,Rx);
	uDiff = u1(I1,I2,I3,Rx)-u1(Ig1,Ig2,Ig3,Rx);
	real averageSpacing;
	if( rangeDimension==2 )
	  averageSpacing = sum( SQRT( SQR(uDiff(I1,I2,I3,0))+SQR(uDiff(I1,I2,I3,1)) ) );
	else
	  averageSpacing = sum( SQRT( SQR(uDiff(I1,I2,I3,0))+SQR(uDiff(I1,I2,I3,1))+SQR(uDiff(I1,I2,I3,2)) ) );
	int num=I1.getLength()*I2.getLength()*I3.getLength();
	averageSpacing/=max(1,num);
---- */	  
	spacingRatio(side,axis) = averageSpacing/boundarySpacing(side,axis);
        printf("stretchTheGrid: (side,axis)=(%i,%i) average spacing = %e, spacingRatio=%e \n",
             side,axis,averageSpacing,spacingRatio(side,axis));

      }
    }
  }

  StretchedSquare stretchedSquare(domainDimension);
  bool StretchStart =FALSE;
  bool StretchEnd   =FALSE;
  for( axis=0; axis<domainDimension; axis++ )
  {
    stretchedSquare.setGridDimensions(axis,mg[0].gridIndexRange(End,axis)-mg[0].gridIndexRange(Start,axis)+1);
    stretchedSquare.setIsPeriodic(axis,userMap->getIsPeriodic(axis));

    StretchStart =spacingRatio(0,axis)>1.5 || spacingRatio(0,axis)<.5;
    StretchEnd   =spacingRatio(1,axis)>1.5 || spacingRatio(1,axis)<.5;
     
    if( StretchStart || StretchEnd )
    {
      const int numberOfSidesToStretch=StretchStart+StretchEnd;
      stretchedSquare.stretchFunction(axis).setStretchingType(StretchMapping::inverseHyperbolicTangent);
      stretchedSquare.stretchFunction(axis).setNumberOfLayers(numberOfSidesToStretch);
      // assign stretching parameters for a*tanh(b(r-c))  : a=1., b=spacingRatio, c=0 or 1.
      for( int i=0; i<numberOfSidesToStretch; i++ )
      {
	int side = StretchStart ? i : i+1;
	stretchedSquare.stretchFunction(axis).setLayerParameters( side, 1.,spacingRatio(side,axis),side);
      }
    }
  }

  if(  StretchStart || StretchEnd )
  {
    printf("Explicitly stretching the grid lines using the stretching functions...\n");
    getIndex(mg[0].gridIndexRange(),I1,I2,I3);

  // stretch the current parameterization, held in rBoundary[0]
    realArray rStretch(I1,I2,I3,Rx), uStretch(I1,I2,I3,Rx);
    uStretch(I1,I2,I3,Rr)=rBoundary[0](I1,I2,I3,Rr);
    stretchedSquare.mapGrid(uStretch,rStretch);
    // rStretch=stretchedSquare.getGrid();
  
    mapToStretch.mapGrid(rStretch,uStretch);  // ghost lines ?...
  
    // reassign the starting grid and all coarser levels.
    startingGrid(uStretch,rStretch,mg[0].gridIndexRange());
  }
  
  return 0;
}

int EllipticGridGenerator::
determineBoundarySpacing(const int & side, 
                         const int & axis,
                         real & averageSpacing,
                         real & minimumSpacing,
                         real & maximumSpacing )
// =================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Compute the current spacing of the first grid line from the boundary
///     
/// \param side,axis (input) : determine spacing for this side.
/// \param averageSpacing,minimumSpacing,maximumSpacing (output) :
/// 
// =================================================================================
{
  if( rangeDimension<2 )
    return 0;
  
  const int level=0;
  RealMappedGridFunction & uu = u[level];
  realArray & u1 = uu;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

  getBoundaryIndex(mg[level].gridIndexRange(),side,axis,I1,I2,I3);    // boundary line
  getGhostIndex(mg[level].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,-1); // first interior line
	  
  // determine the average current grid spacing
  realArray uDiff(I1,I2,I3,Rx);
  uDiff = u1(Ig1,Ig2,Ig3,Rx)-u1(I1,I2,I3,Rx);
  if( rangeDimension==2 )
    uDiff(I1,I2,I3,0)=SQRT( SQR(uDiff(I1,I2,I3,0))+SQR(uDiff(I1,I2,I3,1)) );
  else
    uDiff(I1,I2,I3,0)=SQRT( SQR(uDiff(I1,I2,I3,0))+
			    SQR(uDiff(I1,I2,I3,1))+
			    SQR(uDiff(I1,I2,I3,2)) );
  minimumSpacing=min(uDiff(I1,I2,I3,0));
  maximumSpacing=max(uDiff(I1,I2,I3,0));
  
  averageSpacing=sum(uDiff(I1,I2,I3,0));
  int num=I1.getLength()*I2.getLength()*I3.getLength();
  averageSpacing/=max(1,num);

  return 0;
}


int EllipticGridGenerator::
redBlack(const int & level, 
       RealMappedGridFunction & uu )
// ===============================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///      Red black smooth.
/// \param uu (input/output) : On input and output : current solution valid at all points, including periodic points.
/// 
// ===============================================================================================
{ 
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  realArray & u1 = uu;

  const real omega0 = min( omega, omegaMax(0,level) );

  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  getIndex(mg[level].gridIndexRange(),K1,K2,K3);
  realArray resid(K1,K2,K3,rangeDimension);  
  
  realArray coeff(J1,J2,J3,numberOfCoefficients);
  realArray ur,us,ut;
  ur = uu.r1();
  if( domainDimension>1 )
    us = uu.r2();
  if( domainDimension>2 )
    ut = uu.r3();

  getCoefficients(coeff,J1,J2,J3,ur,us,ut);
  // do not recompute coefficients on subsequent calls
  const bool computeCoefficients=FALSE,includeRightHandSide=TRUE,computeControlFunctions=FALSE;

  const int rb3End = domainDimension<3 ? 1 : 2;
  const int rb2End = domainDimension<2 ? 1 : 2;

  for( int rb3=0; rb3<rb3End; rb3++ )
  {
    for( int rb2=0; rb2<rb2End; rb2++ )
    {
      for( int rb1=0; rb1<2; rb1++ )
      {
	int shift1= (rb1+rb2+rb3) % 2;
	int shift2= rb1;
	int shift3= domainDimension==3 ? rb2 : 0;
	K1=Range(J1.getBase()+shift1,J1.getBound(),2);  // stride 2
	K2=domainDimension>1 ? Range(J2.getBase()+shift2,J2.getBound(),2) : Range(J2);  // stride 2
	K3=domainDimension>2 ? Range(J3.getBase()+shift3,J3.getBound(),2) : Range(J3);

	getResidual( resid,level,Kv,coeff,computeCoefficients,includeRightHandSide,computeControlFunctions);
	
	const real dxSq=dx(0,level)*dx(0,level);
	const real dySq=dx(1,level)*dx(1,level);
	const real dzSq=dx(2,level)*dx(2,level);
	realArray omegaOverDiag(K1,K2,K3);

	if( domainDimension==2 )
	  omegaOverDiag =  (-.5*omega0)/(coeff(K1,K2,K3,0)*(1./dxSq)+coeff(K1,K2,K3,1)*(1./dySq));
	else if( domainDimension==3 )
	  omegaOverDiag=(-.5*omega0)/(coeff(K1,K2,K3,0)*(1./dxSq)+coeff(K1,K2,K3,1)*(1./dySq)+
                                     coeff(K1,K2,K3,2)*(1./dzSq));
	else 
	  omegaOverDiag = (-.5*omega0)/(coeff(K1,K2,K3,0)*(1./dxSq)) ;
  
	for( int j=0; j<rangeDimension; j++ )
	  u1(K1,K2,K3,j)+=resid(K1,K2,K3,j)*omegaOverDiag;

	periodicUpdate(uu);
      }
    }
  }
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);

  work+=1./pow(pow(2,domainDimension),double(level));
  
  return 0;
}


int EllipticGridGenerator::
jacobi(const int & level, 
       RealMappedGridFunction & uu )
// ===============================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///      Jacobi smooth.
/// \param uu (input/output) : On input and output : current solution valid at all points, including periodic points.
/// 
// ===============================================================================================
{ 
  // J1,J2,J3 : interior plus periodic boundaries
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(gridIndex(Range(0,1),Range(0,2),level),J1,J2,J3);

  realArray & u1 = uu;

  realArray coeff(J1,J2,J3,numberOfCoefficients);

  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  getIndex(mg[level].gridIndexRange(),K1,K2,K3);
  realArray resid(K1,K2,K3,rangeDimension);  
  
  getResidual( resid,level,Jv,coeff);

  const real omega0 = min( omega, omegaMax(0,level) );
  if( debug & 2 )
    printf("jacobi:START:omega0=%f, level=%i, residual=%e \n",omega0,level,max(fabs(resid(J1,J2,J3,Rx))));

  const real dxSq=dx(0,level)*dx(0,level);
  const real dySq=dx(1,level)*dx(1,level);
  const real dzSq=dx(2,level)*dx(2,level);
  realArray omegaOverDiag(J1,J2,J3);

  if( domainDimension==2 )
    omegaOverDiag=(-.5*omega0)/(coeff(J1,J2,J3,0)*(1./dxSq)+coeff(J1,J2,J3,1)*(1./dySq));
  else if( domainDimension==3 )
    omegaOverDiag=(-.5*omega0)/(coeff(J1,J2,J3,0)*(1./dxSq)+coeff(J1,J2,J3,1)*(1./dySq)+coeff(J1,J2,J3,2)*(1./dzSq));
  else 
    omegaOverDiag=(-.5*omega0)/(coeff(J1,J2,J3,0)*(1./dxSq)) ;
  
  for( int j=0; j<rangeDimension; j++ )
    u1(J1,J2,J3,j)+=resid(J1,J2,J3,j)*omegaOverDiag;

  if( debug & 2 )
  {
    getResidual( resid,level,Jv,coeff,FALSE); 
    printf("jacobi:2    :omega0=%f, level=%i, residual=%e \n",omega0,level,max(fabs(resid(J1,J2,J3,Rx))));
  }

  periodicUpdate(uu);
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);
  
  if( debug & 2 )
  {
    getResidual( resid,level,Jv,coeff,FALSE); 
    printf("jacobi:END  :omega0=%f, level=%i, residual=%e \n",omega0,level,max(fabs(resid(J1,J2,J3,Rx))));
  }
  
  work+=1./pow(pow(2,domainDimension),double(level));
  return 0;
}

int EllipticGridGenerator::
lineSmoother(const int & direction,
             const int & level,
             RealMappedGridFunction & uu )
// ===================================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Perform a line smooth.
/// \param direction (input) : perform a line smooth along this axis, 0,1, or 2.
/// \param uu (input/output) : On input and output : current solution valid at all points, including periodic points.
/// 
// ===================================================================================================
{
  if( tridiagonalSolver==NULL )
    tridiagonalSolver=new TridiagonalSolver;
    
  assert( tridiagonalSolver!=NULL );
  TridiagonalSolver & tri = *tridiagonalSolver;

  assert( direction>=0 && direction<domainDimension );
  
  realArray & u1 = uu;

  // J1,J2,J3 : interior 
  // Jv[direction] : include boundaries except for right periodic boundary
  // Jv[axis] : interior plus left periodic boundary, axis!=direction

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];

  getIndex(mg[level].gridIndexRange(),K1,K2,K3,1);
  realArray resid(K1,K2,K3,rangeDimension);  

  // Kv will now hold the points where we evaluate the residual

  getIndex(mg[level].gridIndexRange(),J1,J2,J3,-1);  // interior
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    // include periodic boundary at left edge
    Kv[axis]=Jv[axis];
    if( boundaryCondition(Start,axis)<0 )
    {
      Jv[axis]=Range(Jv[axis].getBase()-1,Jv[axis].getBound()); 
      Kv[axis]=Jv[axis];
    }
    else if( axis==direction )
    {
      int jStart=Jv[axis].getBase()-1;
      int jEnd=Jv[axis].getBound()+1;

      // include ghost lines in Jv for free floating.
      if( level==0 && boundaryCondition(Start,axis)==freeFloating )
        jStart--;
      if( level==0 && boundaryCondition(End,axis)==freeFloating )
        jEnd++;

      Jv[axis]=Range(jStart,jEnd);

      Kv[axis]=Range(jStart+1,jEnd-1);
    }
  }
  
  realArray a(J1,J2,J3),b(J1,J2,J3),c(J1,J2,J3),r(J1,J2,J3,Rx);
  realArray coeff(J1,J2,J3,numberOfCoefficients);

  real dxSq[3];
  dxSq[0]=dx(0,level)*dx(0,level);
  dxSq[1]=dx(1,level)*dx(1,level);
  dxSq[2]=SQR(dx(2,level));
  
  int is[3]=  {0,0,0};    //
  is[direction]=1;

/* ----
  realArray ur,us,ut;
  ur = uu.r1();
  if( domainDimension>1 )
    us = uu.r2();
  if( domainDimension>2 )
    ut = uu.r3();
  getCoefficients(coeff,J1,J2,J3,ur,us,ut);
---- */
  
  SmoothingTypes lineSmoothType = direction==0 ? line1Smooth : direction==1 ? line2Smooth : line3Smooth;
  if( FALSE )
    lineSmoothType=jacobiSmooth;
  
  const bool computeCoefficients=TRUE,includeRightHandSide=TRUE,computeControlFunctions=TRUE;
  getResidual( resid,level,Kv,coeff,computeCoefficients,includeRightHandSide,computeControlFunctions,
               lineSmoothType); 
      

  // The tridiagonal system is
  //   a = lower diagonal
  //   b = diagonal
  //   c = upper diagonal
  
  a(J1,J2,J3)=(1./dxSq[direction])*coeff(J1,J2,J3,direction);

  if( domainDimension==2 )
    b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0)+(-2.0/dxSq[1])*coeff(J1,J2,J3,1);
  else if( domainDimension==3 )
    b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0)+(-2.0/dxSq[1])*coeff(J1,J2,J3,1)+(-2.0/dxSq[2])*coeff(J1,J2,J3,2);
  else
    b(J1,J2,J3)=(-2.0/dxSq[0])*coeff(J1,J2,J3,0);
  
  c(J1,J2,J3)=(1./dxSq[direction])*coeff(J1,J2,J3,direction);
  
  if( controlFunctions )
  {
    const realArray & sourceTerm =evaluate((.5/dx(direction,level))*coeff(J1,J2,J3,direction)*
					   source[level](J1,J2,J3,direction)); 
    a(J1,J2,J3)-=sourceTerm;
    c(J1,J2,J3)+=sourceTerm;
  }

  int component;
  if( lineSmoothType==jacobiSmooth )
  {
    for( component=0; component<rangeDimension; component++ ) 
    {
      // resid = RHS - L(u)
      // subtract the left hand side operator from the entire residual
      r(J1,J2,J3,component)=resid(J1,J2,J3,component)+( a(J1,J2,J3)*u1(J1-is[0],J2-is[1],J3-is[2],component)+
							b(J1,J2,J3)*u1(J1      ,J2      ,J3      ,component)+
							c(J1,J2,J3)*u1(J1+is[0],J2+is[1],J3+is[2],component) );
    }
  }
  else
    r(J1,J2,J3,Rx)=resid(J1,J2,J3,Rx);
  
  TridiagonalSolver::SystemType systemType;
  // Boundary Conditions

  if( userMap->getIsPeriodic(direction)==Mapping::functionPeriodic )
  {
    systemType=TridiagonalSolver::periodic;
  }
  else
  {
    systemType=TridiagonalSolver::normal;  // may be changed below to extended.


    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
    Ib1=J1; Ib2=J2; Ib3=J3;
    for( int side=Start; side<=End; side++ )
    {
      int n= side==Start ? Jv[direction].getBase() : Jv[direction].getBound();
      Ibv[direction]=Range(n,n);
      if( boundaryCondition(side,direction)==dirichlet || 
	  boundaryCondition(side,direction)==noSlipOrthogonalAndSpecifiedSpacing )
      {
	// dirichlet boundary conditions:
	a(Ib1,Ib2,Ib3)=0.;
	b(Ib1,Ib2,Ib3)=1.;
	c(Ib1,Ib2,Ib3)=0.;
	r(Ib1,Ib2,Ib3,Rx)=u1(Ib1,Ib2,Ib3,Rx);

      }
      else if( level==0 && boundaryCondition(side,direction)==freeFloating )
      {
        //  Thus just extrapolate :  u_0 = 2*u_1-u2

        systemType=TridiagonalSolver::extended;

        b(Ib1,Ib2,Ib3)=1.;
        if( side==Start )
	{
	  c(Ib1,Ib2,Ib3)=-2.;
	  a(Ib1,Ib2,Ib3)=1.;  // for an extended system this will be the coeff of u_2
	}
	else
	{
	  a(Ib1,Ib2,Ib3)=-2.;
	  c(Ib1,Ib2,Ib3)=1.;  // for an extended system this will be the coeff of u_{n-2}
	}
	r(Ib1,Ib2,Ib3,Rx)=0.;
      }
      else if( FALSE && boundaryCondition(side,direction)==slipOrthogonal )
      {
        // ******* this didn't seem to work *****

	// slip orthogonal boundary condition
        //  (x_1-x_0) . x_t = 0  : this will mean differnt tridiagonal systems for each component!

        //  Thus just extrapolate :  u_0 = 2*u_1-u2

        systemType=TridiagonalSolver::extended;

        b(Ib1,Ib2,Ib3)=1.;
        if( side==Start )
	{
	  c(Ib1,Ib2,Ib3)=-2.;
	  a(Ib1,Ib2,Ib3)=1.;  // for an extended system this will be the coeff of u_2
	}
	else
	{
	  a(Ib1,Ib2,Ib3)=-2.;
	  c(Ib1,Ib2,Ib3)=1.;  // for an extended system this will be the coeff of u_{n-2}
	}
	r(Ib1,Ib2,Ib3,Rx)=0.;
      }
      else if( userMap->getIsPeriodic(direction)==Mapping::derivativePeriodic )
      {
        systemType=TridiagonalSolver::periodic;
	// derivativePeriodic :
        //  a0*u_{-1} + b0*u_0 + c0*u_1 = r0
        //  u_{-1} = u_n - periodicShift
        // ==> a0*u_n b0*u_0 + c0*u_1 = r0 + periodicShift*a0
        //     an*u_{n-1} + b_n*u_n + c_n u_0 = r_n - periodicShift*c0
        for( int dir=0; dir<rangeDimension; dir++ )
	{
          real periodVector=userMap->getPeriodVector(dir,direction);
          if( periodVector!=0. )
	  {
	    if( side==Start )
	      r(Ib1,Ib2,Ib3,dir)+=periodVector*a(Ib1,Ib2,Ib3);
	    else
	      r(Ib1,Ib2,Ib3,dir)-=periodVector*c(Ib1,Ib2,Ib3);
	  }
	}
      }
      else 
      {
	// slip orthogonal boundary conditions??
	a(Ib1,Ib2,Ib3)=0.;
	b(Ib1,Ib2,Ib3)=1.;
	c(Ib1,Ib2,Ib3)=0.;
        r(Ib1,Ib2,Ib3,Rx)=u1(Ib1,Ib2,Ib3,Rx);
      }
    }
  }
  
  
  if( debug & 8 && systemType==TridiagonalSolver::periodic )
    printf("***** periodic TridiagonalSolver called ***** \n");


  // **remember** the factor will change a,b,c
  #ifndef USE_PPP
    tri.factor(a,b,c,systemType,direction);
    for( component=0; component<rangeDimension; component++ ) 
      tri.solve(r(J1,J2,J3,Range(component,component)),J1,J2,J3);
  #else
    Overture::abort("EllipticGridGenerator::ERROR:finish me for parallel Bill!");
  #endif

  const real omega0 = min( omega, omegaMax(0,level) );

  if( omega0==1. )
    u1(J1,J2,J3,Rx)=r(J1,J2,J3,Rx);
  else
    u1(J1,J2,J3,Rx)=(1.0-omega0)*u1(J1,J2,J3,Rx)+omega0*r(J1,J2,J3,Rx);

  periodicUpdate(uu);
  // apply the boundary conditions
  applyBoundaryConditions(level,uu);
  
/* ---
  // LU factor, n-1 multiplies, n-1 divisions **** fix for 3D ****
  real wu = rangeDimension==2  ? 2./6. : 2./8;
  // back substitute 2n-2 multiplies, n divisions
  wu += rangeDimension==2 ? 3./6. : 3./8.;
---- */
  work+=(1.5)/pow(pow(2,domainDimension),double(level));   // check this.
  return 0;
}




int EllipticGridGenerator::
smooth(const int & level, 
       const SmoothingTypes & smoothingType,
       const int & numberOfSubIterations /* =1 */ )
// ====================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Handles the different smoothing methods.
// ====================================================================

{
  RealMappedGridFunction uOld;
  uOld=u[level];

  int axis;
  for( int subiter=0; subiter<numberOfSubIterations; subiter++ )
  {
    switch( smoothingType )
    {
    case jacobiSmooth:       //underelaxed Jacobi
      // printf("jacobi smooth at level=%i...\n",i);
      jacobi(level,u[level]);
      break;

    case redBlackSmooth:
      // printf("red black smooth at level=%i...\n",i);
      redBlack(level,u[level]);
      break;

    case lineSmooth:
    case zebraSmooth:
      // printf("line smooth at level=%i...\n",i);
      if( useBlockTridiag )
	printf("****** BlockTridiag not implemented yet. Using non-block line smooth *****\n");
    
      if( smoothingMethod==zebraSmooth )
	printf("****** zebraSmooth not implemented yet, using lineSmooth *****\n");
    
      for( axis=0; axis<domainDimension; axis++ )
        lineSmoother( axis, level,u[level] );  // solve for x,y,z 
      break; 

    case line1Smooth:
        lineSmoother( axis1, level,u[level] );  // line smooth in along axis1
	break;
	
    case line2Smooth:
      axis=min(axis2,domainDimension-1);
      lineSmoother( axis, level,u[level] );   // line smooth in along axis2
      break;
	
    case line3Smooth:
      axis=min(axis3,domainDimension-1);
      lineSmoother( axis, level,u[level] );  // line smooth in along axis3
      break;
	
    default:
      printf("smooth:ERROR:Unknown method. Exiting\n");
      exit(1);
    }
  }

  restrictMovement(level,uOld,u[level]);
  
  return 0;
}



#define FULL_WEIGHTING_1D(i1,i2,i3,Rx) (  \
      cr(-1,cf1)*defectFine(i1-1,i2,i3,Rx)           \
     +cr( 0,cf1)*defectFine(i1  ,i2,i3,Rx)           \
     +cr(+1,cf1)*defectFine(i1+1,i2,i3,Rx)           \
                                    )
#define FULL_WEIGHTING_1D_001(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*defectFine(i1,i2,i3-1,Rx)           \
     +cr( 0,cf3)*defectFine(i1,i2,i3  ,Rx)           \
     +cr(+1,cf3)*defectFine(i1,i2,i3+1,Rx)           \
                                    )

#define FULL_WEIGHTING_2D(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D(i1,i2+1,i3,Rx)  \
                                    )

#define FULL_WEIGHTING_3D(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*FULL_WEIGHTING_2D(i1,i2,i3-1,Rx)  \
     +cr( 0,cf3)*FULL_WEIGHTING_2D(i1,i2,i3  ,Rx)  \
     +cr(+1,cf3)*FULL_WEIGHTING_2D(i1,i2,i3+1,Rx)  \
                                    )

#define BOUNDARY_DEFECT_PLANE_110(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D(i1,i2+1,i3,Rx)  \
                                    )
#define BOUNDARY_DEFECT_PLANE_101(i1,i2,i3,Rx) (  \
      cr(-1,cf3)*FULL_WEIGHTING_1D(i1,i2,i3-1,Rx)  \
     +cr( 0,cf3)*FULL_WEIGHTING_1D(i1,i2,i3  ,Rx)  \
     +cr(+1,cf3)*FULL_WEIGHTING_1D(i1,i2,i3+1,Rx)  \
                                    )
#define BOUNDARY_DEFECT_PLANE_011(i1,i2,i3,Rx) (  \
      cr(-1,cf2)*FULL_WEIGHTING_1D_001(i1,i2-1,i3,Rx)  \
     +cr( 0,cf2)*FULL_WEIGHTING_1D_001(i1,i2  ,i3,Rx)  \
     +cr(+1,cf2)*FULL_WEIGHTING_1D_001(i1,i2+1,i3,Rx)  \
                                    )

// The boundary defect in 2D should be called in one of two ways
#define BOUNDARY_DEFECT_LINE(is1,is2,is3,i1,i2,i3,Rx)                           \
    ( .5*defectFine(i1,i2,i3,Rx)+.25*(defectFine(i1+is1,i2+is2,i3+is3,Rx)+defectFine(i1-is1,i2-is2,i3+is3,Rx)) )


int EllipticGridGenerator::
fineToCoarse(const int & level, 
	     const RealMappedGridFunction & uFine, 
	     RealMappedGridFunction & uCoarse,
             const bool & isAGridFunction /* = FALSE */ )
// ============================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Compute the restriction of the defect
/// \param isAGridFunction (input) : If true then this variable defines some (x,y,z) coordinates on a grid.
///     This is used to get the correct periodicity.
// ============================================================================================
{
  realArray cr(Range(-1,1),Range(1,2));   // coefficients for restriction
  cr(-1,1)=0.;  cr(0,1)=1.; cr(+1,1)=0.;  // coarsening factor of 1
  cr(-1,2)=.25; cr(0,2)=.5; cr(+1,2)=.25; // coarsening factor of 2
  
  MappedGrid & mgFine      = mg[level];
  MappedGrid & mgCoarse    = mg[level+1];  
  const realArray & defectFine   = uFine;
  realArray & fCoarse      = uCoarse;

  int cf1,cf2,cf3,cf[3];  // ***** coarsening factors are all 2 for now *****
  cf1=cf[0]=2;
  cf2=cf[1]=2;
  cf3=cf[2]=2;

  assert(cf[0]==2 && (cf[1]==2 || domainDimension<2) && (cf[2]==2 || domainDimension<3));

  fCoarse=0.;   // **** could do better ***

  // int numberOfFictitiousPoints = 1;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];

  getIndex(mgFine.indexRange(),I1,I2,I3);                    // Index's for fine grid, 
  // set stride
  int dir;
  for( dir=0; dir<3; dir++ )
    Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound(),cf[dir]); 
  
  getIndex(mgCoarse.indexRange(),J1,J2,J3);                  // Index's for coarse grid

  // Average interior points using the full weighting operator
  if( domainDimension==1 )
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_1D(I1,I2,I3,Rx);      
  else if( domainDimension==2 )
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_2D(I1,I2,I3,Rx);          
  else
    fCoarse(J1,J2,J3,Rx)=FULL_WEIGHTING_3D(I1,I2,I3,Rx);    
  
  //   === Boundaries ===
  Index Iev[3], &Ie1=Iev[0], &Ie2=Iev[1], &Ie3=Iev[2];
  Index Jev[3], &Je1=Jev[0], &Je2=Jev[1], &Je3=Jev[2];
  
  int side,axis;
  int is[3]={0,0,0};
  for( axis=0; axis<domainDimension; axis++ ) 
  {
    for( side=0; side<=1; side++ )
    {
      if( boundaryCondition(side,axis)>0 )
      { 
	getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);  // bndry pts
	for( dir=0; dir<3; dir++ )
	  Iv[dir]=Range(Iv[dir].getBase(),Iv[dir].getBound(),2);          // set stride to 2

	getBoundaryIndex(mgCoarse.gridIndexRange(),side,axis,J1,J2,J3); 

	if( domainDimension==1 )
	  fCoarse(J1,J2,J3,Rx)=defectFine(I1,I2,I3,Rx);    // inject boundary defects in 1D
	else if( domainDimension==2 )
	{
	  const int axisp1= (axis+1) % domainDimension; // tangential direction

       	  is[axisp1]=1;   // we average in this direction
	  fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_LINE(is[0],is[1],is[2],I1,I2,I3,Rx);  // average
       	  is[axisp1]=0;   // reset

  	  // Inject values at corners in 2D or edges in 3D (some points are done twice but who cares...)
	  //                    X -------- X
	  //                    |          |
	  //                    |          |
	  //                    |          |
	  //                    |          |
	  //                    X -------- X
	  if( boundaryCondition(Start,axisp1)>0 )
	    fCoarse(J1.getBase(),J2.getBase(),J3.getBase(),Rx)=defectFine(I1.getBase(),I2.getBase(),I3.getBase(),Rx);
	  if( boundaryCondition(End,axisp1)>0 )
	    fCoarse(J1.getBound(),J2.getBound(),J3.getBase(),Rx)=defectFine(I1.getBound(),I2.getBound(),I3.getBound(),Rx);

	}
	else
	{
          // boundaries in 3D:  
          //          o average faces using 2D full weighting
          //          o average edges using 1D full weighting
          //          o vertices are injected.
          if( axis==0 )
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_011(I1,I2,I3,Rx); // average along boundary face
          else if( axis==1 )
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_101(I1,I2,I3,Rx); // average along boundary face
          else
  	    fCoarse(J1,J2,J3,Rx)=BOUNDARY_DEFECT_PLANE_110(I1,I2,I3,Rx); // average along boundary face
          
          // do the edges of this plane
          Ie1=I1; Ie2=I2; Ie3=I3;  // defines the edge on the fine grid.
          Je1=J1; Je2=J2; Je3=J3;  // defines the edge on the coarse grid.
	  int side2;
	  for( side2=Start; side2<=End; side2++ )                // loop over left and right side
	  {
	    for( int axist=0; axist<domainDimension-1; axist++ )    // 2 tangential directions
	    {
	      const int axisp1= (axis+axist+1) % domainDimension; // edge is defined by axis==side, axisp1=side2
              if( boundaryCondition(side2,axisp1)>0 )
	      {
		Iev[axisp1]= side2==0 ? Iv[axisp1].getBase() : Iv[axisp1].getBound();
		Jev[axisp1]= side2==0 ? Jv[axisp1].getBase() : Jv[axisp1].getBound();

		const int axisp2= (axisp1+1) % domainDimension; // this direction still varies.
                is[axisp2]=1;  
		fCoarse(Je1,Je2,Je3,Rx)=BOUNDARY_DEFECT_LINE(is[0],is[1],is[2],Ie1,Ie2,Ie3,Rx); 
                is[axisp2]=0;  

                // vertices are injected
		if( boundaryCondition(Start,axisp2)>0 )
		  fCoarse(J1.getBase(),J2.getBase(),J3.getBase(),Rx)=defectFine(I1.getBase(),I2.getBase(),I3.getBase(),Rx);
		if( boundaryCondition(End,axisp2)>0 )
		  fCoarse(J1.getBound(),J2.getBound(),J3.getBase(),Rx)=defectFine(I1.getBound(),I2.getBound(),I3.getBound(),Rx);

	      }
	    }
	  }
	}
      
      }
    }
  }
  
  periodicUpdate(uCoarse,nullRange,isAGridFunction); // ***** should this be fCoarse ***************
  work+=.5/pow(pow(2,domainDimension),double(level));
  return 0;
}

#undef FULL_WEIGHTING_1D
#undef FULL_WEIGHTING_2D
#undef FULL_WEIGHTING_3D
#undef BOUNDARY_DEFECT_2D
#undef BOUNDARY_DEFECT_3D



//---------------------------------------------------------------------------------------------
//   Prolongation on a component grid
//
//     u.multigridLevel[level] += Prolongation[ u.multigridLevel[level+1] ]
//   
//---------------------------------------------------------------------------------------------
//     ...2nd order interpolation
#define Q2000(j1,j2,j3) ( uCoarse(j1,j2,j3) )
#define Q2100(j1,j2,j3) ( cp2(0,cf1)*uCoarse(j1,j2,j3)+cp2(1,cf1)*uCoarse(j1+1,j2  ,j3  ) )
#define Q2010(j1,j2,j3) ( cp2(0,cf2)*uCoarse(j1,j2,j3)+cp2(1,cf2)*uCoarse(j1  ,j2+1,j3  ) )
#define Q2001(j1,j2,j3) ( cp2(0,cf3)*uCoarse(j1,j2,j3)+cp2(1,cf3)*uCoarse(j1  ,j2  ,j3+1) )
#define Q2110(j1,j2,j3) ( cp2(0,cf2)*  Q2100(j1,j2,j3)+cp2(1,cf2)*  Q2100(j1  ,j2+1,j3  ) )
#define Q2101(j1,j2,j3) ( cp2(0,cf3)*  Q2100(j1,j2,j3)+cp2(1,cf3)*  Q2100(j1  ,j2  ,j3+1) )
#define Q2011(j1,j2,j3) ( cp2(0,cf3)*  Q2010(j1,j2,j3)+cp2(1,cf3)*  Q2010(j1  ,j2  ,j3+1) )
#define Q2111(j1,j2,j3) ( cp2(0,cf3)*  Q2110(j1,j2,j3)+cp2(1,cf3)*  Q2110(j1  ,j2  ,j3+1) )

//     ...fourth order interpolation
#define Q4000(j1,j2,j3) ( uCoarse(j1,j2,j3) )

#define Q4100(j1,j2,j3) ( cp4( 0,cf1)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf1)*uCoarse(j1+1,j2  ,j3  ) \
                         +cp4(-1,cf1)*uCoarse(j1-1,j2  ,j3  )+cp4(2,cf1)*uCoarse(j1+2,j2  ,j3  ) )

#define Q4010(j1,j2,j3) ( cp4( 0,cf2)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf2)*uCoarse(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*uCoarse(j1  ,j2-1,j3  )+cp4(2,cf2)*uCoarse(j1  ,j2+2,j3  ) )

#define Q4001(j1,j2,j3) ( cp4( 0,cf3)*uCoarse(j1  ,j2  ,j3  )+cp4(1,cf3)*uCoarse(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*uCoarse(j1  ,j2  ,j3-1)+cp4(2,cf3)*uCoarse(j1  ,j2  ,j3+2) )

#define Q4110(j1,j2,j3) ( cp4( 0,cf2)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf2)*  Q4100(j1  ,j2+1,j3  ) \
                         +cp4(-1,cf2)*  Q4100(j1  ,j2-1,j3  )+cp4(2,cf2)*  Q4100(j1  ,j2+2,j3  ) )

#define Q4101(j1,j2,j3) ( cp4( 0,cf3)*  Q4100(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4100(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4100(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4100(j1  ,j2  ,j3+2) )

#define Q4011(j1,j2,j3) ( cp4( 0,cf3)*  Q4010(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4010(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4010(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4010(j1  ,j2  ,j3+2) )

#define Q4111(j1,j2,j3) ( cp4( 0,cf3)*  Q4110(j1  ,j2  ,j3  )+cp4(1,cf3)*  Q4110(j1  ,j2  ,j3+1) \
                         +cp4(-1,cf3)*  Q4110(j1  ,j2  ,j3-1)+cp4(2,cf3)*  Q4110(j1  ,j2  ,j3+2) )


int EllipticGridGenerator::
coarseToFine(const int & level,  
	     const RealMappedGridFunction & uCoarse, 
	     RealMappedGridFunction & uFineGF,
             const bool & isAGridFunction /* = FALSE */  )
//===================================================================
/// \param Access: {\bf Protected}.
/// \details 
///              Correct a Component Grid
///       u(i,j) = u(i,j) + P[ u2(i,j) ]   ( P : Prolongation )
///   cp21,cp22,cp23 : coeffcients for prolongation, 2nd order
///   cp41,cp41,cp43 : coeffcients for prolongation, 4th order
/// 
//===================================================================
{
  const int orderOfAccuracy=2;
  realArray & uFine = uFineGF;

  // ** const real c41=9./16.,c42=-1./16.;
  //  ....cp2,cp4 : coefficients for prolongation (2nd or 4th order)
  //         cp2(.,cf) :  kf=coarsening factor (1 or 2)
  realArray cp2(Range(0,1),Range(1,2));
  realArray cp4(Range(-1,2),Range(1,2));
  cp2(0,1)=1.; cp2(1,1)=0.;  // if coarsening factor =1 we just transfer the data
  cp2(0,2)=.5; cp2(1,2)=.5;  // coarsen factor = 2

  cp4(-1,1)=0.;     cp4(0,1)=1.;    cp4(1,1)=0.;    cp4(2,1)=0.;  
  cp4(-1,2)=-.0625; cp4(0,2)=.5625; cp4(1,2)=.5625; cp4(2,2)=-.0625;  // 4-point order interpolation

  MappedGrid & mgFine   = mg[level];  
  MappedGrid & mgCoarse = mg[level+1];  

  int cf1,cf2,cf3, cf[3];
  cf1=cf[0]=2;  // coarsening factor
  cf2=cf[1]=2;
  cf3=cf[2]=2;

  assert(cf[0]==2 && (cf[1]==2 || domainDimension<2) && (cf[2]==2 || domainDimension<3));
  
  //----------------------------------------------------------------------------------------
  // There are two types of corrections:
  //   (1) when a fine grid and coarse grid point coincide, use Index's I1,I2,I3
  //   (2) when a fine grid point is midway between coarse grid points, use I1p,I2p,I3p
  //
  //        1--2--1--2--1--2------ ... -----2--1--2--1  fine grid
  //        X-----B-----X--------- ... -----X--B-----X  coarse grid
  //   
  //           B=boundary
  //
  //   Note that we use more fictitious points on the fine grid than on the coarse
  //-----------------------------------------------------------------------------------------
  int numberOfFictitiousPoints = 1;
  Index Iav[3], &I1a = Iav[0], &I2a=Iav[1], &I3a=Iav[2];
  Index Jav[3], &J1a = Jav[0], &J2a=Jav[1], &J3a=Jav[2];
  Index I1,I2,I3, J1,J2,J3;
  Index I1p,I2p,I3p,J1p,J2p,J3p;
  int nf0,nf1;

  getIndex(mgFine.indexRange(),I1a,I2a,I3a);
  // ************* this only works for coarsening factor=2 *********
  //----------------------------------
  //---  Get Index's for fine grid ---
  //----------------------------------
  nf0=((numberOfFictitiousPoints+1)/2)*2;   
  nf1=((numberOfFictitiousPoints-2)/2)*2;
  I1p=                          Range(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2p= domainDimension > 1 ? Range(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : Range(I2a);
  I3p= domainDimension > 2 ? Range(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : Range(I3a);

  nf0=((numberOfFictitiousPoints)/2)*2;  
  nf1=((numberOfFictitiousPoints)/2)*2;
  I1 =                          Range(I1a.getBase()-nf0,I1a.getBound()+nf1,2);
  I2 = domainDimension > 1 ? Range(I2a.getBase()-nf0,I2a.getBound()+nf1,2) : Range(I2a);
  I3 = domainDimension > 2 ? Range(I3a.getBase()-nf0,I3a.getBound()+nf1,2) : Range(I3a);

  //------------------------------------
  //---  Get Index's for coarse grid ---
  //------------------------------------
  getIndex(mgCoarse.indexRange(),J1a,J2a,J3a);   // this is ok

  nf0=((numberOfFictitiousPoints+1)/2);  
  nf1=((numberOfFictitiousPoints-2)/2);
  J1p=                          Range(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2p= domainDimension > 1 ? Range(J2a.getBase()-nf0,J2a.getBound()+nf1) : Range(J2a);
  J3p= domainDimension > 2 ? Range(J3a.getBase()-nf0,J3a.getBound()+nf1) : Range(J3a);
  nf0=((numberOfFictitiousPoints)/2);  
  nf1=((numberOfFictitiousPoints)/2);
  J1 =                          Range(J1a.getBase()-nf0,J1a.getBound()+nf1);
  J2 = domainDimension > 1 ? Range(J2a.getBase()-nf0,J2a.getBound()+nf1) : Range(J2a);
  J3 = domainDimension > 2 ? Range(J3a.getBase()-nf0,J3a.getBound()+nf1) : Range(J3a);

  uFine(I1,I2,I3)+=uCoarse(J1,J2,J3);
  
  if( orderOfAccuracy==2 )
  {
    uFine(I1p+1,I2,I3)+=Q2100(J1p,J2 ,J3);
    if( domainDimension > 1 )
    {
      uFine(I1   ,I2p+1,I3)+=Q2010(J1 ,J2p,J3);
      uFine(I1p+1,I2p+1,I3)+=Q2110(J1p,J2p,J3);
    }  
    if( domainDimension>2 )
    {
      uFine(I1   ,I2   ,I3p+1)+=Q2001(J1 ,J2 ,J3p);
      uFine(I1p+1,I2   ,I3p+1)+=Q2101(J1p,J2 ,J3p);
      uFine(I1   ,I2p+1,I3p+1)+=Q2011(J1 ,J2p,J3p);
      uFine(I1p+1,I2p+1,I3p+1)+=Q2111(J1p,J2p,J3p);
    }
  }
  else
  {   // -------- fourth-order ------------
    uFine(I1p+1,I2,I3)+=Q4100(J1p,J2 ,J3);
    if( domainDimension > 1 )
    {
      uFine(I1   ,I2p+1,I3)+=Q4010(J1 ,J2p,J3);
      uFine(I1p+1,I2p+1,I3)+=Q4110(J1p,J2p,J3);
    }  
    if( domainDimension>2 )
    {
      uFine(I1   ,I2   ,I3p+1)+=Q4001(J1 ,J2 ,J3p);
      uFine(I1p+1,I2   ,I3p+1)+=Q4101(J1p,J2 ,J3p);
      uFine(I1   ,I2p+1,I3p+1)+=Q4011(J1 ,J2p,J3p);
      uFine(I1p+1,I2p+1,I3p+1)+=Q4111(J1p,J2p,J3p);
    }
  }
  periodicUpdate(uFineGF,nullRange,isAGridFunction);
  work+=.5/pow(pow(2.,double(domainDimension)),double(level));
  return 0;
}

#undef Q2000
#undef Q2100
#undef Q2010
#undef Q2001
#undef Q2110
#undef Q2101
#undef Q2011
#undef Q2111
#undef Q4000
#undef Q4100
#undef Q4010
#undef Q4001
#undef Q4110
#undef Q4101
#undef Q4011
#undef Q4111






int EllipticGridGenerator::
multigridVcycle(const int & level )
// =========================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///    Multigrid V cycle.
// =========================================================================================
{
  if( level==0 )
  {
   rhs[level]=0.0;
  }
  
  if( debug & 4 && level>0 )
    plot( rhs[level],sPrintF(buff,"rhs at start of cycle, level %i\n",level));


  int numberOfSmooths=1;
  if( level==numberOfLevels-1 )
  {
    // coarse grid : do more iterations.
    numberOfSmooths=(int)pow(2,numberOfLevels+1);
    smooth(level,smoothingMethod,numberOfSmooths);
    return 0;
  }
  
  if( debug & 4 )
    plot( u[level],sPrintF(buff,"solution BEFORE smooth on level %i\n",level));
  
  smooth(level,smoothingMethod,numberOfSmooths);

  Range all;
  RealMappedGridFunction restemp1(mg[level],all,all,all,Rx); // *** is this really needed ??

  if( debug & 2 )
  {
    getResidual(restemp1,level);
    printf("maximum residual = %e after initial smooth at level %i\n",max(fabs(restemp1)),level);
  }
  if( debug & 4 )
    plot( u[level],sPrintF(buff,"solution AFTER smooth on level %i\n",level));
  
  if (level != numberOfLevels-1 )
  {
    Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
    getIndex(mg[level].gridIndexRange(),J1,J2,J3,1); // include 1 ghost point

    Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
    getIndex(mg[level+1].gridIndexRange(),K1,K2,K3,1);

    // restemp1=0.0;
    w[level+1]=0.0;
    getResidual(restemp1,level);
    restemp1.periodicUpdate();  // this seems to be needed.

    if( debug & 4  )
      printf("**** maximum residual=%e on level %i after initial smooth\n",max(fabs(restemp1)),level);
      
    if( debug & 4 )
      plot( restemp1,sPrintF(buff,"residual after smooth on level %i\n",level));

    fineToCoarse(level,restemp1,rhs[level+1]);
    const bool isAGridFunction=TRUE;
    fineToCoarse(level,u[level],w[level+1],isAGridFunction);

    u[level+1]=w[level+1];

    if( debug & 4 )
      plot( u[level+1],sPrintF(buff,"Initial restricted u level %i\n",level+1));

    updateRightHandSideWithFASCorrection(level+1);

    multigridVcycle(level+1);

    w[level+1]=u[level+1]-w[level+1];
    
    coarseToFine(level,w[level+1],u[level],isAGridFunction);
    
  }

  smooth(level,smoothingMethod,numberOfSmooths);

  if( applyBoundarySourceControlFunction )
  {
    for( int axis=0; axis<domainDimension; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
	{
	  real averageSpacing,minimumSpacing,maximumSpacing;
	  determineBoundarySpacing( side, axis,averageSpacing,minimumSpacing,maximumSpacing );

	  printf("BC: (side,axis)=(%i,%i) requested spacing=%6.2e, actual spacing: average=%6.2e,"
		 " min=%6.2e, max=%6.2e\n",side,axis,boundarySpacing(side,axis),averageSpacing,
		 minimumSpacing,maximumSpacing);
	}
      }
    }
  }

  return 0;
}
  

// static int globalIteration=0;

int EllipticGridGenerator::
generateGrid()
// ===========================================================================================
/// \param Access: {\bf Protected}.
/// \details 
///      Perform some multigrid iterations.
/// 
/// \param u0 (input) : initial guess.     // ****** fix this -- we should keep ghost values ------------------
/// 
// ===========================================================================================
{
  printf("applyMultigrid: number of levels=%i, smoother=%s \n",numberOfLevels,
          (const char *)smootherNames[smoothingMethod]);

  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  getIndex(mg[0].gridIndexRange(),J1,J2,J3);

  realArray residtemp1(J1,J2,J3,rangeDimension);
  residtemp1=0.0;

  real totalWork=0., ratio;
  real time0,time;

  getResidual(residtemp1,0);
  maximumResidual=max(fabs(residtemp1));
  printf("** Initial (normalized) residual=%e \n",  maximumResidual/residualNormalizationFactor);

  // Do few iterations of the V-cycle
  time0=getCPU();
  int iter;
  // real scaleFactor=0.;
  for(iter=0; iter<maximumNumberOfIterations; iter++)
  {
    work=0.; // this variable will be incremented by all functions that do significant work.

   if( iter % 10 ==0 )
     getControlFunctions(0); // only evaluate control functions once per cycle
/* -----
//   scaleFactor=min(1.,max(scaleFactor,1./(100.*maximumResidual)));
   if( globalIteration<100 )
     scaleFactor=1./(100-globalIteration);
   else
     scaleFactor=1.;
   globalIteration++;
   
   if( scaleFactor!=1. )
   {
     printf("scale the control functions by %e \n",scaleFactor);
     for( int level=0; level<numberOfLevels; level++ )
       source[level]*=scaleFactor;
   }
----- */   
    // ***** call multigrid ******

    multigridVcycle(0);
    totalWork+=work;

    getResidual(residtemp1,0);
    previousMaximumResidual=maximumResidual;
    maximumResidual=max(fabs(residtemp1));

    ratio=previousMaximumResidual==0. ? 1. : maximumResidual/previousMaximumResidual;

    time=getCPU()-time0;
    printf("iter=%i\t resid=%6.2e ratio=%6.2f ECR=%6.2f WU=%6.2e cpu=%6.2e levels=%i\n",iter, 
           maximumResidual/residualNormalizationFactor, ratio, 
           pow(ratio,1./max(1.,work)), work, time,numberOfLevels);

    if( maximumResidual<residualTolerance )  // scale by the number of grid points.
      break;

    //  printf("%g\t %g\n",work,log10(maximumResidual));
    // fflush(stdout);
    //     if( FALSE && ratio>1. && iter>5 && numberOfLevels>1 )
    //     {
    //       numberOfLevels--;
    //       printf("Decreasing the number of levels, ratio=%g\n",ratio);
    //     }
  }
  return 0;
}

  
int EllipticGridGenerator::
update(DataPointMapping & dpm,
       GenericGraphicsInterface *gi_ /* = NULL */, 
       GraphicsParameters & parameters /* =Overture::nullMappingParameters() */ )
//===========================================================================
/// \details 
///     Prompt for changes to parameters and compute the grid.
/// \param dpm (input) : build this mapping with the grid.
/// \param gi (input) : supply a graphics interface if you want to see the grid as it
///     is being computed.
/// \param parameters (input) : optional parameters used by the graphics interface.
/// 
//===========================================================================
{
  GenericGraphicsInterface & gi = *gi_;
//PlotStuff & gi = (PlotStuff &)(*gi_);  // *** need to check if this is safe.
  ps=&gi;

  aString menu[] = 
    {
      "generate grid",
      ">convergence tolerance",
        "residual tolerance",
      "<>attraction",
        "point attraction",
        "line attraction",
        "plane attraction",
        "normal curvature weight",
      "<boundary conditions",
      ">project",
        "project onto original mapping",
        "do not project onto original mapping",
      "<reset elliptic transform",
      ">parameters",
        "source interpolation coefficient",
        "order of interpolation",
        "change resolution for elliptic grid",
      "<>multigrid",
        ">smoothing method",
	  "Jacobi",
	  "red black",
	  "line",
          "line1",
          "line2",
          "line3",
	  "zebra",
        "<maximum number of iterations",
	"number of multigrid levels",
        ">parameters",
  	  "smoother relaxation coefficient",
	  "use block tridiagonal solver",
	  "do not use block tridiagonal solver",
	  "source relaxation coefficient",
	  "source interpolation power",
        "< ",
      "<useNewStuff",
      "change plot parameters",
      "plot residual",
      "plot control function",
      "test orthogonal",
      "smooth",
      "debug",
      " ",
      "lines",
      "boundary conditions",
      "periodicity",
      "show parameters",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "    Transform a Mapping by Elliptic Grid Generation",
      "transform which mapping? : 		choose the mapping to transform",
      "elliptic smoothing : 			smooth out grid with elliptic transform",
      "change resolution for elliptic grid:	change iDim,jDim for elliptic solver",
      "set number of periods: 			make sources periodic",
      "project onto original mapping (toggle)",
      "reset elliptic transform                 start iterations from scratch",
      "set order of interpolation               order of interpolation for data point mapping",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  bool plotObject=TRUE;
  aString answer,line,answer2; 
  gi.appendToTheDefaultPrompt("elliptic>"); // set the default prompt

  for( int it=0;; it++ )
  {

    if( it==0 && plotObject )
      answer="plotObject";  // plot first time through
    else
      gi.getMenuItem(menu,answer);
    
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="generate grid" )
    {
      generateGrid();
      plotObject=TRUE;
    }
    else if( answer=="debug" )
    {
      gi.inputString(line, sPrintF(buff,"Enter debug (4=plot) (current = %i)",debug));
      if ( line != "")
        sScanF( line,"%i",&debug);
      cout << "debug=" << debug << endl;
    }
    else if( answer=="useNewStuff" )
    {
      useNewStuff=!useNewStuff;
      cout << "useNewStuff=" << useNewStuff << endl;
    }
    else if( answer=="residual tolerance" )
    {
      gi.inputString(line, sPrintF(buff,"Enter the residual tolerance (current = %e)",residualTolerance));
      if ( line != "")
        sScanF( line,"%e",&residualTolerance);
      if( residualTolerance<REAL_EPSILON )
      {
	residualTolerance=REAL_EPSILON*10.;
        printf("ERROR: residualTolerance to small. Setting to %e \n",residualTolerance);
      }
    }
    else if( answer=="point attraction" )
    {
      
      printf("A point of attraction is defined by a source terms of the form \n"
             "  P_m =  - a sign( r_m-c_m ) exp( - b | r - c| ) m=0,1[,2] \n"
             " In 2D [3D] there are 2 [3] source terms, where \n"
             "    a = weight   (example: a=5.)\n"
             "    b = exponent (example: b=5.) \n"
             "    c = location of the point source (c0,c1[,c2]), each c_m in the range [0,1] \n");

      numberOfPointsOfAttraction=0;
      gi.inputString(line,sPrintF(buff,"Enter the number of points of attraction"));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfPointsOfAttraction);
        if( numberOfPointsOfAttraction>0 )
	{
	  pointAttractionParameters.redim(5,numberOfPointsOfAttraction); pointAttractionParameters=0.;
	  for (int n=0; n<numberOfPointsOfAttraction; n++)	       
	  {
	    if( domainDimension==1 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));
	    else if( domainDimension==2 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0,c1 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));
	    else if( domainDimension==3 )
	      gi.inputString(line,sPrintF(buff,"Enter a,b,c0,c1,c2 for P_m = -a sign( r_m-c_m ) exp( -b| r-c | )"));

	    sScanF(line,"%e %e %e %e %e %e",
		   &pointAttractionParameters(0,n),
		   &pointAttractionParameters(1,n),
		   &pointAttractionParameters(2,n),
		   &pointAttractionParameters(3,n),
		   &pointAttractionParameters(4,n));
	  }
	}
      }
      controlFunctionComputed=FALSE;

    }
    else if( answer=="line attraction" )
    {
      printf("A line of attraction is defined by a source term of the form \n"
             "  P_m =  - a sign( r_m-c ) exp( - b |r_m-c| ) \n"
             " where \n"
             "    m = direction (a coordinate direction 0,1, or 2)\n"
             "    a = weight    (example: a=5.)\n"
             "    b = exponent  (example: b=5.)\n"
             "    c = location along coordinate m, in the range [0,1] \n");

      numberOfLinesOfAttraction=0;
      gi.inputString(line,sPrintF(buff,"Enter the number of lines of attraction"));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfLinesOfAttraction);
        if( numberOfLinesOfAttraction>0 )
	{
	  lineAttractionDirection.redim(numberOfLinesOfAttraction); lineAttractionDirection=0;
	  lineAttractionParameters.redim(3,numberOfLinesOfAttraction); lineAttractionParameters=0.;
	  for (int n=0; n<numberOfLinesOfAttraction; n++)	       
	  {
	    gi.inputString(line,sPrintF(buff,"Enter m,a,b,c for P(r) = -a sign( r_m-c ) exp( - b |r_m-c| )"));
	    sScanF(line,"%i %e %e %e",&lineAttractionDirection(n),
		   &lineAttractionParameters(0,n),
		   &lineAttractionParameters(1,n),
		   &lineAttractionParameters(2,n));
            if( lineAttractionDirection(n)<0 || lineAttractionDirection(n) >domainDimension )
	    {
	      printf("Invalid value for m =%i \n",lineAttractionDirection(n));
	      gi.stopReadingCommandFile();
	    }
	  }
	}
      }
      controlFunctionComputed=FALSE;
    }
    else if( answer=="plane attraction" )
    {
      printf("Sorry, this option not implemented\n");
    }
    else if( answer=="set number of periods (for sourced problems)") // ***** remove this ****
    {
      gi.inputString(line,sPrintF(buff,"Enter number of periods for resolving"
		     " periodic problem with source (default==%d):",numberOfPeriods));
      if (line!="")
	 sScanF(line,"%d",&numberOfPeriods);
      if (numberOfPeriods%2==0) 
	 numberOfPeriods++;
      controlFunctionComputed=FALSE;
    }
    else if( answer=="normal curvature weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the normal curvature weight (default==%e)",normalCurvatureWeight));
      if (line!="")
	 sScanF(line,"%e",&normalCurvatureWeight);
      printf("normalCurvatureWeight=%e \n",normalCurvatureWeight);
      controlFunctionComputed=FALSE;
    }
    else if( answer=="boundary conditions" || answer=="elliptic boundary conditions" )
    {

      gi.appendToTheDefaultPrompt("bc>"); 
      aString bcMenu[] =
      {
        "left   (side=0,axis=0)",
        "right  (side=1,axis=0)",
        "bottom (side=0,axis=1)",
        "top    (side=1,axis=1)",
        "back   (side=0,axis=2)",
        "front  (side=1,axis=2)",
        "all sides",
        "exit",
        ""
      };
      aString bcChoices[] = 
      {
        "dirichlet",
        "slip orthogonal",
        "noSlip orthogonal and specified spacing",
        "noSlip orthogonal",
        "free floating",
        "no change",
        ""
      };

      for( ;; )
      {
	int sideChosen = gi.getMenuItem(bcMenu,answer,"choose a menu item");
	if( answer=="exit" )
	{
	  break;
	}
	else if( sideChosen<0 )
	{
	  gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer2) );
	  gi.stopReadingCommandFile();
	  break;
	}
	else if( sideChosen>=0 )
	{
          Range A,S;
          if( answer=="all sides" )
	  {
            A=Range(0,domainDimension-1);
	    S=Range(0,1);
	  }
	  else
	  {
            int side=sideChosen %2;
	    int axis=sideChosen/2;
	    S=Range(side,side);
	    A=Range(axis,axis);
	  }
	  int itemChosen = gi.getMenuItem(bcChoices,answer2,"choose a boundary condition");
          itemChosen++;
          if( itemChosen>0 )
	  {
            where( boundaryCondition(S,A)>=0 )
              boundaryCondition(S,A)=(BoundaryConditionTypes)itemChosen;
            for( int side=S.getBase(); side<=S.getBound(); side++ )
	    {
	      for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
	      {
		if( boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing )
		{
		  real averageSpacing,minimumSpacing,maximumSpacing;
		  determineBoundarySpacing( side, axis,averageSpacing,minimumSpacing,maximumSpacing );
		  printf("Spacing of first grid line for (side,axis)=(%i,%i),  average=%6.3e, minimum=%6.3e, maximum=%6.3e\n",
			 side,axis,averageSpacing,minimumSpacing,maximumSpacing);
                  if( boundarySpacing(side,axis)<=0. )
  		    boundarySpacing(side,axis)=averageSpacing;
		  gi.inputString(line, sPrintF(buff,"Enter the spacing for (side,axis)=(%i,%i) (current = %e)",
					       side,axis,boundarySpacing(side,axis)));
		  if ( line != "")
		    sScanF( line,"%e",&boundarySpacing(side,axis));
		  if( boundarySpacing(side,axis)<=0. )
		  {
		    boundarySpacing(side,axis)=REAL_EPSILON*10.;
		    printf("ERROR: boundarySpacing value is <=0 ! Setting to %e \n",boundarySpacing(side,axis));
		  }
		}
	      }
	    }
	  }
	  else
	  {
	    gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer2) );
	    gi.stopReadingCommandFile();
	    break;
	  }
	}
        controlFunctionComputed=FALSE;
      }
      gi.unAppendTheDefaultPrompt();  // reset

      updateForNewBoundaryConditions();
      
      applyBoundarySourceControlFunction=FALSE;
      for( int axis=0; axis<domainDimension; axis++ )
      {
	if( boundaryCondition(Start,axis)==noSlipOrthogonalAndSpecifiedSpacing ||
	    boundaryCondition(End  ,axis)==noSlipOrthogonalAndSpecifiedSpacing )
	{
	  applyBoundarySourceControlFunction=TRUE;
	  break;
	}
      }

      if( applyBoundarySourceControlFunction )
      {
//	stretchTheGrid(*userMap);
	stretchTheGrid(dpm);
        plotObject=TRUE;
      }
      
    }
    else if( answer=="project onto original mapping" )
    {
    }
    else if( answer=="do not project onto original mapping" )
    {
    }
    else if( answer=="reset elliptic transform" )
    {
      startingGrid(userMap->getGrid());
      plotObject=TRUE;
    }
    else if( answer=="order of interpolation" )
    {
    }
    else if( answer=="change resolution for elliptic grid" )
    {
    }
    else if( answer=="Jacobi" ||answer=="jacobi" )
    {
      smoothingMethod=jacobiSmooth;
      omega=4./5.; // ***** 2d/3d ******
    }
    else if( answer=="red black" )
    {
      smoothingMethod=redBlackSmooth;
      omega=1.;
    }
    else if( answer=="line" )
    {
      smoothingMethod=lineSmooth;
      omega=1.;
    }
    else if( answer=="line1" )
    {
      smoothingMethod=line1Smooth;
      omega=1.;
    }
    else if( answer=="line2" )
    {
      smoothingMethod=line2Smooth;
      omega=1.;
    }
    else if( answer=="line3" )
    {
      smoothingMethod=line3Smooth;
      omega=1.;
    }
    else if( answer=="zebra" )
    {
      smoothingMethod=zebraSmooth;
      omega=1.;
    }
    else if( answer=="maximum number of iterations" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the maximum number of iterations (default=%i): ",
           maximumNumberOfIterations));
      if( line != "" )
      {
	sScanF(line,"%i",&maximumNumberOfIterations);
      }
    }
    else if( answer=="number of multigrid levels" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the number of levels (current==%i,max=%i): ",numberOfLevels,
              maximumNumberOfLevels));
      if (line != "")
      {
	sScanF(line,"%i",&numberOfLevels);
	if( numberOfLevels>maximumNumberOfLevels)
        {
	  gi.outputString("Error:: Too big. Using the maximum");
	  numberOfLevels=maximumNumberOfLevels;
	}
      }
    }
    else if( answer=="smoother relaxation coefficient" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the relaxation coefficient (default=%f): ",omega));
      if( line != "" ) sScanF(line,"%e", &omega);
    }
    else if( answer=="use block tridiagonal solver" )
    {
      useBlockTridiag=1;
      if( smoothingMethod==3 || smoothingMethod==4 )
	printf(" Will use block tridiagonal solver\n");
    }
    else if( answer=="do not use block tridiagonal solver" )
    {
      useBlockTridiag=0;
      if( smoothingMethod==3 || smoothingMethod==4 )
	printf(" Will not use block tridiagonal solver\n");
    }
    else if( answer=="source relaxation coefficient" )
    {
    }
    else if( answer=="smooth" )
    {
      smooth(0,smoothingMethod,1);
      plotObject=TRUE;
    }
    else if( answer=="show parameters" )
    {
      printf("--------------- parameters for EllipticGridGeneration ------------------------\n");
      printf("number of multigrid levels=%i (maximum=%i)\n",numberOfLevels,maximumNumberOfLevels);
      printf("residualTolerance = %e \n",residualTolerance);
      
      for( int axis=0; axis<domainDimension; axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
	  printf(" boundaryCondition(side=%i,axis=%i) = %s \n",side,axis,
		 boundaryCondition(side,axis)==-1 ? "periodic" : 
                 boundaryCondition(side,axis)==dirichlet ? "dirichlet" :
                 boundaryCondition(side,axis)==slipOrthogonal ? "slip orthogonal" :
		 boundaryCondition(side,axis)==noSlipOrthogonalAndSpecifiedSpacing ? 
 		                              "noSlip orthogonal and specified spacing" :
                 boundaryCondition(side,axis)==noSlipOrthogonal ? "noSlip orthogonal" :
                 boundaryCondition(side,axis)==freeFloating ? "free floating" :
                 "unknown");
	}
      }
      if( numberOfLinesOfAttraction>0 )
      {
        printf("Attraction to a line: P_m =  - a sign( r_m-c ) exp( - b |r_m-c| ) \n"
               "  point    m     a     b     c \n");
	for(int n=0; n<numberOfPointsOfAttraction; n++)	       
	{
          printf("%6i %6.2e %6.2e %6.2e  \n",
		 lineAttractionDirection(n),
                 lineAttractionParameters(0,n),
                 lineAttractionParameters(1,n),
                 lineAttractionParameters(2,n));
	}
      }
      if( numberOfPointsOfAttraction>0 )
      {
	printf("Attraction to a point: P_m =  - a sign( r_m-c_m ) exp( - b | r - c| ) m=0,1[,2] \n"
               "  point    a     b     c_0     c_1    c_2 \n");
	for(int n=0; n<numberOfPointsOfAttraction; n++)	       
	{
          printf("%6.2e %6.2e %6.2e %6.2e %6.2e \n",
		 pointAttractionParameters(0,n),
		 pointAttractionParameters(1,n),
		 pointAttractionParameters(2,n),
		 pointAttractionParameters(3,n),
		 pointAttractionParameters(4,n));
	}
      }
    }
    else if( answer=="plot residual" )
    {
      Range all;
      RealMappedGridFunction res(mg[0],all,all,all,Rx);
      getResidual(res,0);
      gi.erase();
      psp.set(GI_TOP_LABEL,"residual");
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      PlotIt::contour(gi,res,psp);

      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      plotObject=TRUE;
    }
    else if( answer=="plot control function" )
    {
      gi.erase();
      getControlFunctions(0);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      psp.set(GI_TOP_LABEL,"control function");

      PlotIt::contour(gi,source[0],psp);

      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      plotObject=TRUE;
    }
    else if( answer=="test orthogonal" )
    {
      printf("Apply orthogonal BC to level 0\n");
      applyBoundaryConditions(0,u[0]);
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      PlotIt::plot(gi,dpm,(GraphicsParameters&)psp);
      plotObject=TRUE;
    }
    else if( answer=="plotObject" )
    {
      plotObject=TRUE;
    }
    else if( answer=="change plot parameters" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      gi.erase();
      PlotIt::plot(gi,dpm,(GraphicsParameters&)psp);
    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
    
    if( plotObject )
    {
      plotObject=FALSE;
      gi.erase();
      dpm.setDataPoints(u[0],3,domainDimension,0,mg[0].gridIndexRange());
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      psp.set(GI_TOP_LABEL,"elliptic grid");
      PlotIt::plot(gi,dpm,(GraphicsParameters&)psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }

  }

  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

