#include "DomainSolver.h"
#include "GridFunction.h"
#include "SplineMapping.h"
#include "GenericGraphicsInterface.h"
#include "LineMapping.h"
#include "TFIMapping.h"
#include "ReductionMapping.h"
#include "ReparameterizationTransform.h"
#include "IntersectionMapping.h"

int DomainSolver::
userDefinedGrid( GridFunction & gfct,  
                 Mapping *&newMapping, 
                 int newGridNumber, 
                 IntegerArray & sharedBoundaryCondition )
// =========================================================================================
// /Description:
//   This routine is called every time step to give the user a chance to add a new grid.
//
// /gfct (input) : current grid function (see below for details)
//
//\end{CompositeGridSolverInclude.tex} 
// =========================================================================================
{
  int debugUser=0;
  
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  realCompositeGridFunction & u = gf[current].u; // current solution
  CompositeGrid & cg = gf[current].cg;           // current grid
//  real time = gfct.t;                     // current time
  
// cout << " Number of Grids = " << cg.numberOfComponentGrids() << endl;

  int gridForCurve=cg.numberOfBaseGrids()-1;

  Index I1,I2,I3;
  MappedGrid & c = cg[gridForCurve];
  c.update(MappedGrid::THEvertex);       // make sure the vertex array has been created
  realArray & vertex = c.vertex();       // grid points
  realMappedGridFunction & v = u[gridForCurve];

  const IntegerArray & nr = c.gridIndexRange();      // grid bounds

  int ndim=nr(1,1)-nr(0,1)+1;
  realArray points(ndim,2);

// locate position of maximal first difference of density along i2=const grid lines
  int minIndex=nr(1,0);
  int maxIndex=nr(0,0);
  
  for (int i2=nr(0,1); i2<=nr(1,1); i2++)
  {
    int i1max=nr(0,0);
    real drmax=0.;

    for (int i1=nr(0,0); i1<nr(1,0); i1++)
    {
      real dr=fabs(v(i1+1,i2,0,0)-v(i1,i2,0,0));
      if (dr>drmax)
      {
        drmax=dr;
        i1max=i1;
      }
    }

//    cout << "i2 = " << i2 << "  i1max = " << i1max << endl;

    points(i2-nr(0,1),0)=vertex(i1max,i2,0,0);
    points(i2-nr(0,1),1)=vertex(i1max,i2,0,1);

    minIndex=min(minIndex,i1max);
    maxIndex=max(maxIndex,i1max);
    

//    cout << "points = " << points(i2-nr(0,1),0) << " " << points(i2-nr(0,1),1) << endl;

  }

  printf("track: nr(0,0)=%i, minIndex=%i, maxIndex=%i, nr(1,0)=%i\n",nr(0,0),minIndex,maxIndex,nr(1,0));
  if( minIndex>nr(0,0)+parameters.dbase.get<int >("trackingFrequency") && maxIndex<nr(1,0)-parameters.dbase.get<int >("trackingFrequency") )
  {
    printf(" **** no need to regrid. front is far enough from the boundaries\n");
    return 1;
  }
  

  // wdh: average points down to a fewer number
  //   ex: average [0..4] [4..8] [8..12] ..   if istride=4
  int i2stride=max(2,(nr(1,1)-nr(0,1))/6);
  int ndim2 = 3+ndim/(i2stride-1); //  *wdh* i2stride+3;
  realArray avgPoints(ndim2,2);

  int n,m=0;
  int j2,k2;
  realArray avg(2);
  for (j2=0; j2<ndim; j2+=i2stride)         // compute average of these points
  {
    n=0;
    avg(0)=0.;
    avg(1)=0.;

    int k2max=min(j2+i2stride,ndim);
    for (k2=j2; k2<k2max; k2++)
    {
      n+=1;
      avg(0)+=points(k2,0);
      avg(1)+=points(k2,1);
    }

    m+=1;
    if (m>=ndim2)
      cout << "yikes: m>=ndim2 " << endl;
    avgPoints(m,0)=avg(0)/n;
    avgPoints(m,1)=avg(1)/n;

//    cout << "avgPoints = " << avgPoints(m,0) << " " << avgPoints(m,1) << endl;

  }

// smooth the averaged points
  int nsmooth=2;
  realArray avgP(ndim2,2);
  for (int ns=0; ns<nsmooth; ns++)
  {
    for (k2=1; k2<=m; k2++)
    {
      avgP(k2,0)=avgPoints(k2,0);
      avgP(k2,1)=avgPoints(k2,1);
    }
    for (k2=2; k2<m; k2++)
    {
      avgPoints(k2,0)=avgP(k2,0)+(avgP(k2+1,0)-2*avgP(k2,0)+avgP(k2-1,0))/3;
      avgPoints(k2,1)=avgP(k2,1)+(avgP(k2+1,1)-2*avgP(k2,1)+avgP(k2-1,1))/3;
    }
  }

  real scaleFactor=5.;

// extrapolate by extending the curve out linearly at the ends
  real topExtend=3.*scaleFactor;
  real bottomExtend=3.*scaleFactor;
  avgPoints(0,0)=avgPoints(2,0)+bottomExtend*(avgPoints(1,0)-avgPoints(2,0));
  avgPoints(0,1)=avgPoints(2,1)+bottomExtend*(avgPoints(1,1)-avgPoints(2,1));
  avgPoints(m+1,0)=avgPoints(m-1,0)+topExtend*(avgPoints(m,0)-avgPoints(m-1,0));
  avgPoints(m+1,1)=avgPoints(m-1,1)+topExtend*(avgPoints(m,1)-avgPoints(m-1,1));

  if( debugUser & 2 )
    for (k2=0; k2<=m+1; k2++)
      cout << "avgPoints = " << avgPoints(k2,0) << " " << avgPoints(k2,1) << endl;

  int mp2=m+2;
  Range I=mp2;

  SplineMapping & sMiddle = * new SplineMapping; sMiddle.incrementReferenceCount();
  SplineMapping & sFront  = * new SplineMapping; sFront.  incrementReferenceCount();
  SplineMapping & sBack   = * new SplineMapping; sBack.   incrementReferenceCount();

  sMiddle.setParameterizationType(SplineMapping::arcLength);
  sMiddle.setPoints(avgPoints(I,0),avgPoints(I,1));

// compute derivative
  int i;
  realArray r(mp2), x(mp2,2), xr(mp2,2,1), a(mp2), b(mp2), x0(mp2,2), x1(mp2,2);
  for (i=0; i<mp2; i++)
    r(i)=i/(m+1.);
  sMiddle.map(r,x,xr);
  r=SQRT(SQR(xr(I,0,0))+SQR(xr(I,1,0)));
  a= xr(I,1,0)/r;
  b=-xr(I,0,0)/r;
  if( debugUser & 2 )
  {
    for (i=0; i<mp2; i++)
    {
      cout << "normals = " << a(i) << " " << b(i) << endl;
    }
  }
  
// compute front and back curves
  real backLength=.2*scaleFactor;
  real frontLength=.4*scaleFactor;

  x0(I,0)=x(I,0)-backLength*a(I);
  x0(I,1)=x(I,1)-backLength*b(I);
  x1(I,0)=x(I,0)+frontLength*a(I);
  x1(I,1)=x(I,1)+frontLength*b(I);

  sBack.setParameterizationType(SplineMapping::arcLength);
  sBack.setPoints(x0(I,0),x0(I,1));
  sFront.setParameterizationType(SplineMapping::arcLength);
  sFront.setPoints(x1(I,0),x1(I,1));

  if( false ) // || stepNumber%40==0 )
  {
    PlotIt::plot(gi,sMiddle,psp);
    PlotIt::plot(gi,sFront,psp);
    PlotIt::plot(gi,sBack,psp);
  }


  printf("build a TFIMapping from the left and right curves. \n");

  Mapping & xLeft = sBack;
  Mapping & xRight = sFront;
	
  int bottomBaseGrid=min(1,cg.numberOfComponentGrids()-1);
  int topBaseGrid=0;   // hard code this for now.

  Mapping *map[2];
  map[0] = & cg[bottomBaseGrid].mapping().getMapping();
  map[1] = & cg[topBaseGrid].mapping().getMapping();


  int bc[2][3]={0,0,0,0,0,0}; // default BC is interpolation
	
  bc[0][1]=cg[bottomBaseGrid].boundaryCondition(0,1);
  bc[1][1]=cg[topBaseGrid].boundaryCondition(1,1);

  int share[2][3]={0,0,0,0,0,0}; 
  share[0][1]=cg[bottomBaseGrid].sharedBoundaryFlag(0,1);
  share[1][1]=cg[topBaseGrid].sharedBoundaryFlag(1,1);


  IntersectionMapping intersect;
  realArray r1,r2,xi;
   
  Mapping *edgeCurve[2];
   
  real edgeEndPoint[2][2], leftEndPoint[2], rightEndPoint[2];
  int side;
  for( side=0; side<=1; side++ )
  {
    // determine the intersection of the curves with sides of the base grid.

     // form curves for the bottom or top of the base grid
    real sa=side;
    edgeCurve[side] = new ReductionMapping(*map[side],(real)-1.,sa);  edgeCurve[side]->incrementReferenceCount();
    Mapping & edge = *edgeCurve[side];

    // Intersect the left and right curves with this edge

    int numberOfIntersectionPoints;
    intersect.intersectCurves(edge,xLeft,numberOfIntersectionPoints,r1,r2,xi);

    if( numberOfIntersectionPoints==0 )
    {
      printf("ERROR: No intersections found between the left-curve and the boundary, side=%i\n",side);
      return 1;
    }
    
    edgeEndPoint[0][side]=r1(0);
    leftEndPoint[side]=r2(0);
    intersect.intersectCurves(edge,xRight,numberOfIntersectionPoints,r1,r2,x);
    if( numberOfIntersectionPoints==0 )
    {
      printf("ERROR: No intersections found between the right-curve and the boundary, side=%i\n",side);
      return 1;
    }

    edgeEndPoint[1][side]=r1(0);
    rightEndPoint[side]=r2(0);
     
  }
  if( debugUser & 2 )
  {
    printf("leftEndPoint=[%e,%e] rightEndPoint=[%e,%e] \n",leftEndPoint[0],leftEndPoint[1],
	   rightEndPoint[0],rightEndPoint[1]);
    printf("edgeEndPoint (bottom) = [%e,%e] edgeEndPoint (top) = [%e,%e] \n",
	   edgeEndPoint[0][0],edgeEndPoint[1][0],edgeEndPoint[0][1],edgeEndPoint[1][1]);
  }
  
  // Form the sub-sections of the curves that will be used for the TFI mapping

  ReparameterizationTransform & left = 
    *new ReparameterizationTransform(xLeft,ReparameterizationTransform::restriction);
  left.incrementReferenceCount();
  left.setBounds(leftEndPoint[0],leftEndPoint[1]);

  ReparameterizationTransform & right = 
    *new ReparameterizationTransform(xRight,ReparameterizationTransform::restriction);
  right.incrementReferenceCount();
  right.setBounds(rightEndPoint[0],rightEndPoint[1]);

  ReparameterizationTransform & bottom = 
    *new ReparameterizationTransform(*edgeCurve[0],ReparameterizationTransform::restriction);
  bottom.incrementReferenceCount();
  bottom.setBounds(edgeEndPoint[0][0],edgeEndPoint[1][0]);

  ReparameterizationTransform & top = 
    *new ReparameterizationTransform(*edgeCurve[1],ReparameterizationTransform::restriction);
  top.incrementReferenceCount();
  top.setBounds(edgeEndPoint[0][1],edgeEndPoint[1][1]);



  // Mapping = new TFIMapping(&sBack,&sFront);
  newMapping = new TFIMapping(&left,&right,&bottom,&top); newMapping->incrementReferenceCount();
  newMapping->setName(Mapping::mappingName,"tracking");


  real arcLength=right.getArcLength();
  int numLines=int(5*arcLength+1.5);
  printf("tracking: arcLength=%8.2e, horizontal lines=%i \n",arcLength,numLines);

  newMapping->setGridDimensions(axis1,41); // map.getGridDimensions(axis1));
  newMapping->setGridDimensions(axis2,numLines);

  int axis;
  for( side=0; side<=1; side++ )
  {
    for( axis=0; axis<2; axis++ )
    {
      newMapping->setBoundaryCondition(side,axis,bc[side][axis]);
      newMapping->setShare(side,axis,share[side][axis]);
    }
  }
      
  gi.erase();
  PlotIt::plot(gi,cg,psp);
    
  psp.set(GI_MAPPING_COLOUR,"red");
  PlotIt::plot(gi, *newMapping,psp );

  sMiddle.decrementReferenceCount();
  sFront .decrementReferenceCount();
  sBack  .decrementReferenceCount();

  left.decrementReferenceCount();
  right.decrementReferenceCount();
  bottom.decrementReferenceCount();
  top.decrementReferenceCount();

  edgeCurve[0]->decrementReferenceCount();
  edgeCurve[1]->decrementReferenceCount();

  // ** newMapping=NULL;  // ****************************** remove for now *****
  
  return 0;
}
