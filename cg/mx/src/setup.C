#include "Maxwell.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "AnnulusMapping.h"
#include "MatrixTransform.h"
#include "DataPointMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "ParallelUtility.h"
#include "GridStatistics.h"

#include "ULink.h"

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// ======================================================================================
//  Return the max value of a scalar over all processors
//  /processor: return the result to this processor (-1 equals all processors)
// ======================================================================================
real Maxwell::
getMaxValue(real value, int processor /* = -1 */)
{
  real maxValue=value;
  #ifdef USE_PPP 
  if( processor==-1 )
    MPI_Allreduce(&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  else
    MPI_Reduce        (&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, processor, MPI_COMM_WORLD);
  #endif
  return maxValue;
}

int Maxwell::
getMaxValue(int value, int processor /* = -1 */)
{
  int maxValue=value;
  #ifdef USE_PPP 
  if( processor==-1 )
    MPI_Allreduce(&value, &maxValue, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  else
    MPI_Reduce        (&value, &maxValue, 1, MPI_INT, MPI_MAX, processor, MPI_COMM_WORLD);
  #endif
  return maxValue;
}

real Maxwell::
getMinValue(real value, int processor /* = -1 */ )
{
  real minValue=value;
  #ifdef USE_PPP 
  if( processor==-1 )
    MPI_Allreduce(&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  else
    MPI_Reduce        (&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, processor, MPI_COMM_WORLD);
  #endif
  return minValue;
}

int Maxwell::
getMinValue(int value, int processor /* = -1 */)
{
  int minValue=value;
  #ifdef USE_PPP 
  if( processor==-1 )
    MPI_Allreduce(&value, &minValue, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  else
    MPI_Reduce        (&value, &minValue, 1, MPI_INT, MPI_MIN, processor, MPI_COMM_WORLD);
  #endif
  return minValue;
}

//! Determine the time step
int Maxwell::
computeTimeStep()
// =============================================================================================
// =============================================================================================
{
  real time0=getCPU();

  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  deltaT=REAL_MAX*.01;
  
//    RealArray dtGrid(numberOfComponentGrids);  // time step for each grid by itself
//    dtGrid=REAL_MAX;
  
  real cMax=max(cGrid);
  if( numberOfMaterialRegions>1 )
  { // Compute maximum c for variable eps and mu
    assert( numberOfComponentGrids==1 );
    cMax = sqrt(1./min(epsv*muv));
    // cMax = ParallelUtility::getMaxValue(cMax);
    printF("computeTimeStep: numberOfMaterialRegions=%i cMax=%9.3e\n",numberOfMaterialRegions,cMax);
  }
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    MappedGrid & mg = cg[grid];

    const int numberOfDimensions=mg.numberOfDimensions();
    real c = cGrid(grid);   
    // eps = epsGrid(grid);
    // mu = muGrid(grid);

    if( numberOfMaterialRegions>1 )
      c=cMax;
	
    // SOSUP: dt depends on the order of accuracy
    // Approximate stability regions:
    //      (c*dt/dx)^sp + (c*dt/dy)^sp = lambda^sp
    // Then
    //     dt =  (lambda/c) / [  (1/dx)^sigma + (1/dy)^sigma )^(1/sigma) ]
    // 
    // where sp and lambda depend on the orderOfAccuracyInSpace:
    //   sp     = sosupPower[orderOfAccuracyInSpace], 
    //   lambda = sosupLambda[orderOfAccuracyInSpace], 
    const int maxOrderOfAccuracy=10;
    assert( orderOfAccuracyInSpace<=maxOrderOfAccuracy );
    real sosupPower2d[maxOrderOfAccuracy]  = { 2., 2., 2., 2., 2., 2., 2., 2., 2., 2. }; // note: some entries not used
    real sosupLambda2d[maxOrderOfAccuracy] = { 1., 1., 1., 1., 1., 1., 1., 1., 1., 1. }; // 
    real sosupPower3d[maxOrderOfAccuracy]  = { 2., 2., 2., 2., 2., 2., 2., 2., 2., 2. }; // 
    real sosupLambda3d[maxOrderOfAccuracy] = { 1., 1., 1., 1., 1., 1., 1., 1., 1., 1. }; // 
    
    // From Jeff Banks:
    //  2nd order: sigma=1.35,   b=.605
    //  4th order: sigma=2.175,  b=1.075
    //  6th order: sigma=1.6,    b=1.275
    sosupPower2d[2]=1.35;  sosupLambda2d[2]=.605;   // 2nd order 2D
    //sosupPower2d[4]=2.175; sosupLambda2d[4]=1.075;  // 4th order 2D, beta = 1
    sosupPower2d[4]=1.6;   sosupLambda2d[4]=1.4;    // 4th order 2D, beta = 0.8
    sosupPower2d[6]=1.6;   sosupLambda2d[6]=1.275;  // 6th order 2D
    
    // *finish me for 3D:*
    sosupPower3d[2]=1.35;  sosupLambda3d[2]=.605;   // 2nd order 3D
    //sosupPower3d[4]=2.175; sosupLambda3d[4]=1.075;  // 4th order 3D, beta = 1
    sosupPower3d[4]=1.6;   sosupLambda3d[4]=1.4;    // 4th order 3D, beta = 0.8
    sosupPower3d[6]=1.6;   sosupLambda3d[6]=1.275;  // 6th order 3D

    real dtg=REAL_MAX*.01;
    if( mg.getGridType()==MappedGrid::structuredGrid )
    {
      real dx[3];
      if( mg.isRectangular() )
      {
	mg.getDeltaX(dx);

	if( method==nfdtd || method==yee )
	{
	  if( numberOfDimensions==2 )
	    dtg=cfl*1./( c*sqrt( 1./(dx[0]*dx[0])+1./(dx[1]*dx[1]) ) );  
	  else
	    dtg=cfl*1./( c*sqrt( 1./(dx[0]*dx[0])+1./(dx[1]*dx[1])+1./(dx[2]*dx[2]) ) ); 
	}
	else if( method==sosup )
	{
          // SOSUP: dt depends on the order of accuracy: 
	  if( numberOfDimensions==2 )
	  {
	    const real lambda = sosupLambda2d[orderOfAccuracyInSpace], sp = sosupPower2d[orderOfAccuracyInSpace];
	    dtg = cfl*(lambda/c)/( pow( pow(1./dx[0],sp) + pow(1./dx[1],sp) , 1./sp ) );
	  }
	  else
	  {
	    const real lambda = sosupLambda3d[orderOfAccuracyInSpace], sp = sosupPower3d[orderOfAccuracyInSpace];
	    dtg = cfl*(lambda/c)/( pow( pow(1./dx[0],sp) + pow(1./dx[1],sp) + pow(1./dx[2],sp) , 1./sp ) );
	  }
			       
	}
	else
	{
	  OV_ABORT("computeTimeSTep::ERROR: unknown method");
	}
	
	dxMinMax(grid,0)=numberOfDimensions==2 ? min(dx[0],dx[1]) : min(dx[0],dx[1],dx[2]);
	dxMinMax(grid,1)=numberOfDimensions==2 ? max(dx[0],dx[1]) : max(dx[0],dx[1],dx[2]);
	
	// printF(" computeTimeStep: dx=%8.2e dy=%8.2e c=%8.2e, dtg=%8.2e\n",dx[0],dx[1],c,dtg);
      }
      else
      {
	mg.update(MappedGrid::THEinverseVertexDerivative);
	const realArray & rx = mg.inverseVertexDerivative();
	const intArray & mask = mg.mask();
	
      
	Index I1,I2,I3;
	getIndex( mg.indexRange(),I1,I2,I3);

	// Grid spacings on unit square:
	real dr1 = mg.gridSpacing(axis1);
	real dr2 = mg.gridSpacing(axis2);
	real dr3 = mg.gridSpacing(axis3);

	// parallel version here --- also broadcast max error in forcing.bC *************************
        #ifdef USE_PPP
          realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
          intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
        #else
          const realSerialArray & rxLocal = rx;
          const intSerialArray & maskLocal = mask;
        #endif

	real *rxp = rxLocal.Array_Descriptor.Array_View_Pointer3;
	const int rxDim0=rxLocal.getRawDataSize(0);
	const int rxDim1=rxLocal.getRawDataSize(1);
	const int rxDim2=rxLocal.getRawDataSize(2);
	const int rxDim3=mg.numberOfDimensions();   // note
#undef RX
#define RX(i0,i1,i2,i3,i4) rxp[i0+rxDim0*(i1+rxDim1*(i2+rxDim2*(i3+rxDim3*(i4))))]

	const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	const int maskDim0=maskLocal.getRawDataSize(0);
	const int maskDim1=maskLocal.getRawDataSize(1);
	const int md1=maskDim0, md2=md1*maskDim1; 
#define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]


	int includeGhost=0;
	bool ok = ParallelUtility::getLocalArrayBounds(rx,rxLocal,I1,I2,I3,includeGhost);

	int i1,i2,i3;
	real a11Min=REAL_MAX*.001;
	real a11Max=-a11Min;
	//  **** this is a guess **** check this.
	const real alpha0=1.;

	dxMinMax(grid,0)=REAL_MAX*.01; 
	dxMinMax(grid,1)=0.;
	dtg = REAL_MAX*.01;

	real a11,a12,a22;
	if( ok )
	{
	  if( numberOfDimensions==2 )
	  {

	    if( method!=sosup )
	    {
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
	      
		if( MASK(i1,i2,i3)>0 )
		{
		  a11 = ( RX(i1,i2,i3,0,0)*RX(i1,i2,i3,0,0) + RX(i1,i2,i3,0,1)*RX(i1,i2,i3,0,1) );
		  a12 = ( RX(i1,i2,i3,0,0)*RX(i1,i2,i3,1,0) + RX(i1,i2,i3,0,1)*RX(i1,i2,i3,1,1) )*2.;
		  a22 = ( RX(i1,i2,i3,1,0)*RX(i1,i2,i3,1,0) + RX(i1,i2,i3,1,1)*RX(i1,i2,i3,1,1) );

                  // we could save work by delaying the sqrt to after the loop
		  a11=1./sqrt( a11 *(1./(alpha0*dr1*dr1)) 
			       +abs(a12)*(.25/(alpha0*dr1*dr2))
			       +a22 *(1./(alpha0*dr2*dr2)) 
		    );

		  a11Min=min(a11Min,a11);
		  a11Max=max(a11Max,a11);
          
		}

	      }
	    }
	    else
	    {
              // sosup:

              // FIX dxMin dxMax !
              const real lambda = sosupLambda2d[orderOfAccuracyInSpace], sp = sosupPower2d[orderOfAccuracyInSpace];
              const real spBy2=sp*.5;
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
	      
		if( MASK(i1,i2,i3)>0 )
		{
		  a11 = ( RX(i1,i2,i3,0,0)*RX(i1,i2,i3,0,0) + RX(i1,i2,i3,0,1)*RX(i1,i2,i3,0,1) );
		  a12 = ( RX(i1,i2,i3,0,0)*RX(i1,i2,i3,1,0) + RX(i1,i2,i3,0,1)*RX(i1,i2,i3,1,1) )*2.;
		  a22 = ( RX(i1,i2,i3,1,0)*RX(i1,i2,i3,1,0) + RX(i1,i2,i3,1,1)*RX(i1,i2,i3,1,1) );

                  // we could save work by delaying the outer pow to after the loop
		  a11=lambda/pow( pow(a11 *(1./(alpha0*dr1*dr1)),spBy2) +
				  pow(abs(a12)*(.25/(alpha0*dr1*dr2)),spBy2) +
				  pow(a22 *(1./(alpha0*dr2*dr2)),spBy2), 1./sp );

		  a11Min=min(a11Min,a11);
		  a11Max=max(a11Max,a11);
          
		}
	      }
	    }
	    
	    
	  }
	  else
	  { // ***** 3D ********

#define rxDotRx(axis,dir) (RX(i1,i2,i3,axis,0)*RX(i1,i2,i3,dir,0) \
			 + RX(i1,i2,i3,axis,1)*RX(i1,i2,i3,dir,1) \
			 + RX(i1,i2,i3,axis,2)*RX(i1,i2,i3,dir,2))
      
	    if( method!=sosup )
	    {
	      // There would be a factor of 4 for the worst case plus/minus wave but we also
	      // divide by a factor of 4 for the 2nd-order time stepping.
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 )
		{
                 // we could save work by delaying the sqrt to after the loop
		  a11=1./sqrt(   rxDotRx(0,0) *(1./(dr1*dr1)) 
				 +rxDotRx(1,1) *(1./(dr2*dr2))
				 +rxDotRx(2,2) *(1./(dr3*dr3))
				 +abs(rxDotRx(1,0))*(.5/(dr2*dr1))  
				 +abs(rxDotRx(2,0))*(.5/(dr3*dr1)) 
				 +abs(rxDotRx(2,1))*(.5/(dr3*dr2)) );

		  // ** a11 =  pow(a11,-.5);
		
		  a11Min=min(a11Min,a11);
		  a11Max=max(a11Max,a11);

		}
	      }
	    }
	    else
	    { // sosup: 
              const real lambda = sosupLambda3d[orderOfAccuracyInSpace], sp = sosupPower3d[orderOfAccuracyInSpace];
              const real spBy2=sp*.5;
	      FOR_3D(i1,i2,i3,I1,I2,I3)
	      {
		if( MASK(i1,i2,i3)>0 )
		{
                  // we could save work by delaying the outer pow to after the loop
		  a11=lambda/pow( pow(rxDotRx(0,0) *(1./(dr1*dr1)),spBy2) +
				  pow(rxDotRx(1,1) *(1./(dr2*dr2)),spBy2) +
				  pow(rxDotRx(2,2) *(1./(dr3*dr3)),spBy2) +
				  pow(abs(rxDotRx(1,0))*(.5/(dr2*dr1)),spBy2) + 
				  pow(abs(rxDotRx(2,0))*(.5/(dr3*dr1)),spBy2) + 
				  pow(abs(rxDotRx(2,1))*(.5/(dr3*dr2)),spBy2), 1./sp );

		  // ** a11 =  pow(a11,-.5);
		
		  a11Min=min(a11Min,a11);
		  a11Max=max(a11Max,a11);

		}
	      }
	    }
	    
#undef rxDotRx
	  }

	  dxMinMax(grid,0)=a11Min; 
	  dxMinMax(grid,1)=a11Max;
	  dtg = (cfl/c) * dxMinMax(grid,0); 
	}
	
      }
    
      dtg=getMinValue(dtg);  // compute min over all processors
      dxMinMax(grid,0)=getMinValue(dxMinMax(grid,0));
      dxMinMax(grid,1)=getMaxValue(dxMinMax(grid,1));
      
      // Cartesian grids use: artificialDissipation
      // Curvilinear grids use: artificialDissipationCurvilinear
      const real artDiss = mg.isRectangular() ? artificialDissipation : artificialDissipationCurvilinear;
       
      if( artDiss>0. )
      {
	// Here is the correction for artificial dissipation
        //
        // The equation for dt looks like
        //   dt*dt *c*c*(  1/dx^2 + 1/dy^2 ) = 1 - beta*dt
        //
        real gamma = dtg*dtg;
        real beta;
        // const real adc = artDiss*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
        const real adc = c*artDiss; // do this now *wdh* 090602

        beta = .5*adc*( numberOfDimensions*pow(2.,real(orderOfArtificialDissipation)) );
        real factor=2.;  // safety factor
	beta *=factor;

        dtg = sqrt( gamma + pow(beta*gamma*.5,2.) ) - beta*gamma*.5;

        if( debug & 4 )
	  fprintf(pDebugFile," getTimeStep: Correct for art. dissipation: new dt=%9.3e (old = %9.3e, new/old=%4.2f) myid=%i\n",
		 dtg,sqrt(gamma),dtg/sqrt(gamma),myid);

	if( true )
	  printF("***** getTimeStep: Correct for art. dissipation: new dt=%9.3e (old = %9.3e, new/old=%4.2f)\n",
		 dtg,sqrt(gamma),dtg/sqrt(gamma));
	
      }
      
      if( timeSteppingMethod==modifiedEquationTimeStepping )
      {
	if( true || orderOfAccuracyInTime==2 || orderOfAccuracyInTime==4 )
	{
	  dtg*=1.; // Check this for 3D
	}
	else
	{
          printF("getTimeStep:ERROR: modifiedEquationTimeStepping -- orderOfAccuracyInTime=%i ??\n",
          orderOfAccuracyInTime);
	  
          Overture::abort("getTimeStep:ERROR: modifiedEquationTimeStepping -- orderOfAccuracyInTime?? ");
	}
	
      }
      else
      {

	if( orderOfAccuracyInSpace==2 )
	{
	}
	else if( orderOfAccuracyInSpace==4 )
	  dtg*=sqrt(3./4.);
	else if( orderOfAccuracyInSpace==6 )
	  dtg*=sqrt(.6618);
	else if( orderOfAccuracyInSpace==8 )
	  dtg*=sqrt(.6152);
	else
	{
          Overture::abort("getTimeStep:ERROR: modifiedEquationTimeStepping -- orderOfAccuracyInSpace?? ");
	}

	if( orderOfAccuracyInTime==4 )
	{
	  dtg*=1.41/2.;
	}
	else if( orderOfAccuracyInTime==6 )
	{
	  dtg*=.84/2.;
	}
	else if( orderOfAccuracyInTime==8 )
	{
	  dtg*=.46/2.;
	}
	else if( orderOfAccuracyInTime==3 && method==dsi )
	{
	  dtg*=(12./7.)/2.;   // ABS3
	}
	else if( orderOfAccuracyInTime!=2 )
	{
	  Overture::abort();
	}
      }
      
      
      printF(" computeTimeStep: grid=%i c=%8.2e, dtg=%8.2e min-dx=%8.2e max-dx=%8.2e\n",grid,c,dtg,dxMinMax(grid,0),dxMinMax(grid,1));
      // printf(" computeTimeStep: grid=%i c=%8.2e, dtg=%8.2e min-dx=%8.2e max-dx=%8.2e myid=%i\n",
      //          grid,c,dtg,dxMinMax(grid,0),dxMinMax(grid,1),myid);

#undef RX
    
    }
    else
    {
      // unstructured grid.

//        UnstructuredMapping & map = (UnstructuredMapping &) mg.mapping().getMapping();
//        const int numberOfElements = map.getNumberOfElements();
//        const intArray & faces = map.getFaces();
//        const intArray & elementFace = map.getElementFaces();
//        const realArray & nodes = map.getNodes();

//        real dsMax=0.;
//        int e;
//        for( e=0; e<numberOfElements; e++ )
//        {
//  	const int numFacesThisElement=map.getNumberOfFacesThisElement(e);      

//  	// select two faces from the element:
//  	int f1=elementFace(e,0);
//  	int f2=elementFace(e,numFacesThisElement-1);
      
//  	int n0=faces(f1,0), n1=faces(f1,1);
//  	real ds1=SQR( nodes(n1,0)-nodes(n0,0) )+SQR( nodes(n1,1)-nodes(n0,1) );
//  	n0=faces(f2,0), n1=faces(f2,1);
//  	real ds2=SQR( nodes(n1,0)-nodes(n0,0) )+SQR( nodes(n1,1)-nodes(n0,1) );

//  	dsMax = max( dsMax, 1./ds1+1./ds2 );
//        }
    
//        if( map.getNumberOfFacesThisElement(0)==3 )
//        {
//  	dsMax*=4.;  // **** fudge for triangular grids -- fix this ---
//        }
    
#if 0
      mg.update(MappedGrid::THEminMaxEdgeLength);
      real dsMax =8./(mg.minimumEdgeLength(0)*mg.minimumEdgeLength(0));

      printF(" computeTimeStep: dsMax=%e c=%e\n",dsMax,c);

      dtg=cfl*1./( c*sqrt(dsMax) );
#else

      UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(cg.numberOfDimensions());
      UnstructuredMapping::EntityTypeEnum cellBdyType = UnstructuredMapping::EntityTypeEnum(cg.numberOfDimensions()-1);
      UnstructuredMapping::EntityTypeEnum faceType = UnstructuredMapping::Face;
      UnstructuredMapping::EntityTypeEnum edgeType = UnstructuredMapping::Edge;

      UnstructuredMapping &umap = (UnstructuredMapping &)mg.mapping().getMapping();

      int rDim = umap.getRangeDimension();
      int dDim = umap.getDomainDimension();

      bool vCent = mg.isAllVertexCentered();
      realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
      const realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
      const realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
      const realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();

      realArray cellBdyCenters, cellCenters;
      getCenters( mg, cellBdyType, cellBdyCenters);
      getCenters( mg, cellType, cellCenters);

      const realArray &nodes  = umap.getNodes();
      const intArray &edges = umap.getEntities(edgeType);

      UnstructuredMappingIterator iter,iter_end;
      UnstructuredMappingAdjacencyIterator aiter, aiter_end;

      iter_end = umap.end(UnstructuredMapping::Face);
      real minLen = REAL_MAX;
      real minFEL = REAL_MAX;
      for ( iter = umap.begin(UnstructuredMapping::Face); iter!=iter_end; iter++ )
      {
	int f=*iter;
	real area = cFArea(f,0,0);
	real lsum = 0.;
	aiter_end = umap.adjacency_end(iter, UnstructuredMapping::Edge);
	for ( aiter = umap.adjacency_begin(iter, UnstructuredMapping::Edge); aiter!=aiter_end; aiter++ )
	{
	  int e = *aiter;
	  real edgeL = 0;
	  int v1 = edges(e,0);
	  int v2 = edges(e,1);
	  for ( int a=0; a<rDim; a++ )
	    edgeL += ( nodes(v2,a)-nodes(v1,a) )*( nodes(v2,a)-nodes(v1,a) );
	  //	      lsum += sqrt(edgeL);
	  lsum = max(lsum,edgeL);
	}

	lsum = sqrt(lsum);

	minLen = min(minLen, area/lsum);
	minFEL = min(lsum,minFEL);
      }

      iter_end = umap.end(cellBdyType);
      real minCCL = REAL_MAX;
      for ( iter=umap.begin(cellBdyType); iter!=iter_end; iter++ )
      {
	int e = *iter;
	aiter_end = umap.adjacency_end(iter,cellType);
	real edgeL = 0;
	for ( aiter=umap.adjacency_begin(iter,cellType); aiter!=aiter_end; aiter++ )
	{
	  real L = 0;
	  int c = *aiter;
	  for ( int a=0; a<rDim; a++ )
	    L += (cellCenters(c,a)-cellBdyCenters(e,a))*(cellCenters(c,a)-cellBdyCenters(e,a));
	  edgeL += sqrt(L);
	}

	minCCL = min(minCCL, edgeL);
	if ( rDim==2 )
	  minLen = min(edgeL,minLen);
	else
	{
	  aiter_end = umap.adjacency_end(iter,edgeType);
	  for ( aiter=umap.adjacency_begin(iter,edgeType); aiter!=aiter_end; aiter++ )
	  {
	    real area=0;
	    for ( int a=0; a<rDim; a++ )
	      area+=edgeAreaNormals(*aiter,0,0,a)*edgeAreaNormals(*aiter,0,0,a);
	    minLen = min(minLen, sqrt(area)/(2*edgeL));
	  }
	}
	  
      }

      minLen = min(minLen,minCCL);
      // *wdh* 100827 -- commented these next lines out since MappedGrid no longer has minMaxEdgeLength. Is this ok?
      // mg.update(MappedGrid::THEminMaxEdgeLength);
      // cout<<"minFEL = "<<minFEL<<", minCCL = "<<minCCL<<", minLen = "<<minLen<<", minEdge = "<<mg.minimumEdgeLength(0)<<endl;
      // minLen = min(minLen,mg.minimumEdgeLength(0));
      dtg=cfl*minLen/c;

#endif
      
    }
    
    
    deltaT=min(deltaT,dtg);
    
  } // end for grid

  deltaT=getMinValue(deltaT);  // min value over all processors
  printF("==== computeTimeStep: deltaT=%8.2e\n",deltaT);
  if( debug & 4 )
    fprintf(pDebugFile,"==== computeTimeStep: deltaT=%8.2e, myid=%i\n",deltaT,myid);

  timing(timeForComputingDeltaT)+=getCPU()-time0;
  return 0;
}




//! Setup and initialization. Build the grid.
int Maxwell::
setupGrids()
// ===================================================================================
// Build a grid
// ===================================================================================
{
  real time0=getCPU();

  // real xa=0, xb=1., ya=0., yb=1.;
  real xa=xab[0][0], xb=xab[1][0], ya=xab[0][1], yb=xab[1][1], za=xab[0][2], zb=xab[1][2];
  
  real dx[3];
  dx[0]=(xb-xa)/(nx[0]-1);
  dx[1]=(yb-ya)/(nx[1]-1);
  dx[2]=(zb-za)/(nx[2]-1);

//   xa=0, xb=1., ya=0., yb=1.25;
//   SquareMapping rectangle(xa,xb,ya,yb);
//square.setMappingCoordinateSystem( Mapping::general ); 

  Mapping *mapPointer;

  if( gridType==square && elementType==structuredElements )
  {
    mapPointer= new SquareMapping(xa,xb,ya,yb);  mapPointer->incrementReferenceCount();
  }
  else if ( gridType==box )
  {
    mapPointer = new BoxMapping(xa,xb,ya,yb,za,zb); mapPointer->incrementReferenceCount();

    for( int axis=0; axis<mapPointer->getDomainDimension(); axis++ )
    {
      mapPointer->setGridDimensions(axis,nx[axis]);
      if( bcOption==useAllPeriodicBoundaryConditions )
	mapPointer->setIsPeriodic(axis,Mapping::derivativePeriodic);
    }

    if ( elementType!=structuredElements )
    {
      UnstructuredMapping & uns = * new UnstructuredMapping;
      BoxMapping &bmap = (BoxMapping &)*mapPointer;
      mapPointer->decrementReferenceCount();
      mapPointer= &uns;  mapPointer->incrementReferenceCount();
      if ( bcOption==useAllPeriodicBoundaryConditions )
	uns.addGhostElements(true);
      else
	uns.addGhostElements(false);

      printF(" **** elementType=%i \n",elementType);
	  
      //	  UnstructuredMapping etype;
      uns.addGhostElements(true);
      uns.buildFromAMapping(bmap);
      verifyUnstructuredConnectivity(uns,true);
      for( int axis=0; axis<bmap.getDomainDimension() && bcOption==useAllPeriodicBoundaryConditions; axis++ )
      {
	uns.setIsPeriodic(axis,Mapping::derivativePeriodic);
	uns.setBoundaryCondition(Start,axis,-1);
	uns.setBoundaryCondition(End  ,axis,-1);
      }
      //	  if ( !bcOption==useAllPeriodicBoundaryConditions )
      //	    uns.expandGhostBoundary();

    }
  }
  else if( gridType==annulus )
  {
    mapPointer= new AnnulusMapping;  mapPointer->incrementReferenceCount();
  }
  else if( gridType==rotatedSquare )
  {
    Mapping *sq=new SquareMapping(xa,xb,ya,yb); 
    sq->incrementReferenceCount();
    MatrixTransform & mat = * new MatrixTransform(*sq);
    mapPointer= &mat;   mapPointer->incrementReferenceCount();
    if( sq->decrementReferenceCount()==0 ) delete sq;
    
    // mat.rotate(axis3,90.*twoPi/360.);
    // mat.rotate(axis3,45.*twoPi/360.);
    mat.rotate(axis3,30.*twoPi/360.);
    for( int axis=0; axis<mapPointer->getDomainDimension(); axis++ )
    {
      mapPointer->setGridDimensions(axis,nx[axis]);
    }

    UnstructuredMapping & uns = * new UnstructuredMapping;
    mapPointer->decrementReferenceCount();
    mapPointer= &uns;  mapPointer->incrementReferenceCount();
    uns.addGhostElements(true);

    printF(" **** elementType=%i \n",elementType);
      
    int domainDimension = 2;
    if ( elementType==defaultUnstructured && domainDimension==2)
      uns.buildFromARegularMapping(mat);
    else if ( domainDimension==2 )
    {
      uns.buildFromARegularMapping(mat,elementType==triangles ? UnstructuredMapping::triangle : elementType==quadrilaterals ? UnstructuredMapping::quadrilateral : UnstructuredMapping::hexahedron);
    }
    else
    {
      uns.buildFromAMapping(mat);
      for( int axis=0; axis<uns.getDomainDimension() && 
	     bcOption==useAllPeriodicBoundaryConditions; axis++ )
      {
	uns.setIsPeriodic(axis,Mapping::derivativePeriodic);
	uns.setBoundaryCondition(Start,axis,-1);
	uns.setBoundaryCondition(End  ,axis,-1);
      }
    }
    

    //            verifyUnstructuredConnectivity(uns,true);
    
    if( mat.decrementReferenceCount() == 0 )
      delete &mat;

  }
  else if( gridType==skewedSquare || gridType==sineSquare ||
           gridType==chevron || gridType==chevbox || gridType==sineByTriangles )
  {
    // build a DataPointMapping
    DataPointMapping & dpm = *new DataPointMapping;
    mapPointer= &dpm;   mapPointer->incrementReferenceCount();
    Index I1,I2,I3;

    // *wdh* 050818 -- use 2nd-order interpolation so the metrics are periodic when the grid is periodic
//    dpm.setOrderOfInterpolation(4);
    dpm.setOrderOfInterpolation(2);

//    int numberOfGhostLines=0; // add more for 4th-order and periodic derivatives of mapping at ghost pts (?)
    // int numberOfGhostLines=4; // add more for 4th-order and periodic derivatives of mapping at ghost pts (?)
    int numberOfGhostLines=max(4,(orderOfAccuracyInSpace+2)/2);
//   int numberOfGhostLines=8; // add more for 4th-order and periodic derivatives of mapping at ghost pts (?)

    I1=Range(-numberOfGhostLines,nx[0]+numberOfGhostLines-1);
    I2=Range(-numberOfGhostLines,nx[1]+numberOfGhostLines-1);
    I3= gridType!=chevbox ? Range(0,0) : Range(-numberOfGhostLines,nx[2]+numberOfGhostLines-1);
    
    int rDim = gridType==chevbox ? 3 : 2;
    realArray x(I1,I2,I3,rDim);
    realArray r1(I1),r2(I2),r3(I3);

    r1.seqAdd(-numberOfGhostLines*dx[0],dx[0]);
    r2.seqAdd(-numberOfGhostLines*dx[1],dx[1]);
    if ( gridType==chevbox )
      r3.seqAdd(-numberOfGhostLines*dx[2],dx[2]);

    if( gridType==sineSquare || gridType==sineByTriangles )
    {
      realArray bottom(I1);
      real amplitude=.1;    // amplitude of the sine wave
      bottom=(cos(twoPi*r1)-1.)*amplitude;
    
      int i2=0, i3=0;
      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
	x(I1,i2,i3,0)=r1(I1);
	x(I1,i2,i3,1)=bottom(I1)+r2(i2);
      }
    }
    else if( gridType==chevron || gridType==chevbox)
    {
      real amplitudeY=dx[1]*chevronAmplitude;    // amplitude of the chevron oscillation
      realArray yShift(I1);
      // .5 seemed stable, .75 was not stable
      real freqX=(Pi/dx[0])*chevronFrequency; 
      // real freq=(Pi/dx[0])*.75;  // this is a lower frequency perturbation

      bool useMoreFrequencies=true;
      if( !useMoreFrequencies )
      {
        yShift=cos(freqX*r1)*amplitudeY;
      }
      else
      {
        yShift=cos(freqX*r1)*amplitudeY + cos(.5*freqX*r1)*amplitudeY*1.5;
      }
      
      // display(yShift,"yShift for the chevron grid");
      

      // remove perturbation from the ends
      bool removePerturbationAtEdges=false; // false; // true;
      int extra=numberOfGhostLines+5;
      if( removePerturbationAtEdges )
      {
	yShift(Range(I1.getBase(),I1.getBase()+extra))=0.;
	yShift(Range(I1.getBound()-extra,I1.getBound()))=0.;
      }
      
      // here is a perturbation in the x-direction
      real amplitudeX=dx[0]*chevronAmplitude;  
      realArray xShift(I2);
      real freqY=(Pi/dx[1])*chevronFrequency; 
      xShift=cos(freqY*r2)*amplitudeX; 
      if( useMoreFrequencies )
      {
        xShift=cos(freqY*r2)*amplitudeX+ cos(.5*freqY*r2)*amplitudeX*1.5;
      }
      
      if( removePerturbationAtEdges )
      {
	xShift(Range(I2.getBase(),I2.getBase()+extra))=0.;
	xShift(Range(I2.getBound()-extra,I2.getBound()))=0.;
      }

      int i2=0, i3=0;
      if ( gridType==chevron )
      {
        if( true )
	{
	  // box with random perturbations
          int seed=184273654;
          srand(seed);
	  int i1;
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      real d1=(-1.+2.*rand()/RAND_MAX)*.25*dx[0];  // random number between [-.25,.25]*dx[0]
	      real d2=(-1.+2.*rand()/RAND_MAX)*.25*dx[1];
		
	      x(i1,i2,i3,0)= i1*dx[0]+d1;
	      x(i1,i2,i3,1)= i2*dx[1]+d2;
	    }

          // make periodic
          real xba=1., yba=1., zba=1.;
          Range Rx=2;
          for( i1=I1.getBase(); i1<0; i1++ )
	  {
            x(i1,I2,I3,Rx)=x(i1+nx[0]-1,I2,I3,Rx);
            x(i1,I2,I3,0)-=xba; 
	  }
          for( i1=nx[0]-1; i1<=I1.getBound(); i1++ )
	  {
            x(i1,I2,I3,Rx)=x(i1-nx[0]+1,I2,I3,Rx); 
            x(i1,I2,I3,0)+=xba; 
	  }
	  
          for( i2=I2.getBase(); i2<0; i2++ )
	  {
            x(I1,i2,I3,Rx)=x(I1,i2+nx[1]-1,I3,Rx);
            x(I1,i2,I3,1)-=yba; 
	  }
          for( i2=nx[1]-1; i2<=I2.getBound(); i2++ )
	  {
            x(I1,i2,I3,Rx)=x(I1,i2-nx[1]+1,I3,Rx); 
            x(I1,i2,I3,1)+=yba; 
	  }
	  
	}
	else
	{
	  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  {
	    x(I1,i2,i3,0)=r1(I1);
	    if( !removePerturbationAtEdges ||
		(i2>=I2.getBase() + extra &&
		 i2<=I2.getBound()- extra) )
	    {
	      x(I1,i2,i3,1)=yShift(I1)+r2(i2);
	    }
	    else
	    {
	      x(I1,i2,i3,1)=r2(i2);
	    }
	  
	  }
	  if( amplitudeX != 0. )
	  {
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      x(I1,i2,i3,0)+=xShift(i2);
	  }
	}
	
      }
      else 
      {
        // chevBox
        if( true )
	{
	  // box with random perturbations
          int seed=184273654;
          srand(seed);
	  int i1;
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
                real d1=(-1.+2.*rand()/RAND_MAX)*.25*dx[0];  // random number between [-.25,.25]*dx[0]
                real d2=(-1.+2.*rand()/RAND_MAX)*.25*dx[1];
                real d3=(-1.+2.*rand()/RAND_MAX)*.25*dx[2];
		
                // if( i1<=0 || i1>=nx[0]-1 ) d1=0.;
                // if( i2<=0 || i2>=nx[1]-1 ) d2=0.;
                // if( i3<=0 || i3>=nx[2]-1 ) d3=0.;

		x(i1,i2,i3,0)= i1*dx[0]+d1;
		x(i1,i2,i3,1)= i2*dx[1]+d2;
		x(i1,i2,i3,2)= i3*dx[2]+d3;
	      }

          // make periodic
          real xba=1., yba=1., zba=1.;
          Range Rx=3;
          for( int mm=0; mm<=1; mm++ )
	  {
	    for( i1=I1.getBase(); i1<0; i1++ )
	    {
	      x(i1,I2,I3,Rx)=x(i1+nx[0]-1,I2,I3,Rx);
	      x(i1,I2,I3,0)-=xba; 
	    }
	    for( i1=nx[0]-1; i1<=I1.getBound(); i1++ )
	    {
	      x(i1,I2,I3,Rx)=x(i1-nx[0]+1,I2,I3,Rx); 
	      x(i1,I2,I3,0)+=xba; 
	    }
	  
	    for( i2=I2.getBase(); i2<0; i2++ )
	    {
	      x(I1,i2,I3,Rx)=x(I1,i2+nx[1]-1,I3,Rx);
	      x(I1,i2,I3,1)-=yba; 
	    }
	    for( i2=nx[1]-1; i2<=I2.getBound(); i2++ )
	    {
	      x(I1,i2,I3,Rx)=x(I1,i2-nx[1]+1,I3,Rx); 
	      x(I1,i2,I3,1)+=yba; 
	    }
	  
	    for( i3=I3.getBase(); i3<0; i3++ )
	    {
	      x(I1,I2,i3,Rx)=x(I1,I2,i3+nx[2]-1,Rx);
	      x(I1,I2,i3,2)-=zba; 
	    }
	    for( i3=nx[2]-3; i3<=I3.getBound(); i3++ )
	    {
	      x(I1,I2,i3,Rx)=x(I1,I2,i3-nx[2]+1,Rx);
	      x(I1,I2,i3,2)+=zba; 
	    }
	  }
	  

	}
	else
	{
	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      x(I1,i2,i3,0)=r1(I1);
	      x(I1,i2,i3,1)=yShift(I1)+r2(i2);
	    }

	  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    x(I1,I2,i3,2)=r3(i3);

	  if( amplitudeX != 0. )
	  {
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      x(I1,i2,I3,0)+=xShift(i2);
	  }
	  real amplitudeZ=dx[2]*chevronAmplitude;  
	  if( amplitudeZ != 0. )
	  {
	
	    realArray zShift(I3);
	    real freqY=(Pi/dx[1])*chevronFrequency; 
	    zShift=cos(freqY*r3)*amplitudeZ; 
	    if( useMoreFrequencies )
	    {
	      zShift=cos(freqY*r3)*amplitudeZ+ cos(.5*freqY*r3)*amplitudeZ*1.5;
	    }
	    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	    {
	      x(I1,I2,i3,2)+=zShift(i3);
	    }
	  }
	}
	
      }

    }
    else
    {
      throw "error";
    }
    // display(x,"Data points for DPM");

    const int domainDimension= gridType==chevbox ? 3 : 2;
    int ng=max(2,numberOfGhostLines);
    // int numberOfGhostLinesNew[2][3]={ng,ng,ng,ng,ng,ng}; //
    Mapping::IndexRangeType numberOfGhostLinesNew;
    for( int axis=0; axis<3; axis++ )for( int side=0; side<=1; side++ ) numberOfGhostLinesNew(side,axis)=ng;
    
    // tell the dpm to use the extra ghost lines (*wdh* 050818)
    dpm.setDomainDimension(domainDimension); 
    dpm.setRangeDimension(domainDimension); 
    dpm.setNumberOfGhostLines(numberOfGhostLinesNew);
    
    // Set periodic BC's before supplying data points so the ghost points are computed properly
    // *************** wdh* 050818 **
    if( bcOption==useAllPeriodicBoundaryConditions )
    {
      for( int axis=0; axis<domainDimension; axis++ )
      {
 	dpm.setIsPeriodic(axis,Mapping::derivativePeriodic);
 	dpm.setBoundaryCondition(Start,axis,-1);
 	dpm.setBoundaryCondition(End  ,axis,-1);
      }
    }

    dpm.setDataPoints(x,3,domainDimension,numberOfGhostLines);

    if( gridType==sineByTriangles || elementType!=structuredElements )
    {
      int axis;
      for( axis=0; axis<domainDimension; axis++ )
      {
	dpm.setIsPeriodic(axis,Mapping::derivativePeriodic);
	dpm.setBoundaryCondition(Start,axis,-1);
	dpm.setBoundaryCondition(End  ,axis,-1);
      }

      UnstructuredMapping & uns = * new UnstructuredMapping;
      mapPointer->decrementReferenceCount();
      mapPointer= &uns;  mapPointer->incrementReferenceCount();
      if ( bcOption==useAllPeriodicBoundaryConditions )
	uns.addGhostElements(true);
      else
	uns.addGhostElements(false);

      printF(" **** elementType=%i \n",elementType);
      

      if ( elementType==defaultUnstructured && domainDimension==2)
	uns.buildFromARegularMapping(dpm);
      else if ( domainDimension==2 )
      {
	uns.buildFromARegularMapping(dpm,elementType==triangles ? UnstructuredMapping::triangle : elementType==quadrilaterals ? UnstructuredMapping::quadrilateral : UnstructuredMapping::hexahedron);
      }
      else
      {
// 	  uns.buildFromAMapping(dpm);
// 	  verifyUnstructuredConnectivity(uns,true);

	uns.addGhostElements(true);
	uns.buildFromAMapping(dpm);
	for( int axis=0; axis<uns.getDomainDimension() && 
	       bcOption==useAllPeriodicBoundaryConditions; axis++ )
	{
	  uns.setIsPeriodic(axis,Mapping::derivativePeriodic);
	  uns.setBoundaryCondition(Start,axis,-1);
	  uns.setBoundaryCondition(End  ,axis,-1);
	}
      }

      //            verifyUnstructuredConnectivity(uns,true);

      if( dpm.decrementReferenceCount() == 0 )
        delete &dpm;
      
    }
  }
  else if( gridType==perturbedSquare || gridType==perturbedBox )
  {
    // ************ perturbed square or box **********

    const int rangeDimension = gridType==perturbedSquare ? 2 : 3; 
    const int domainDimension= rangeDimension; 

    // build a DataPointMapping
    DataPointMapping & dpm = *new DataPointMapping;
    mapPointer= &dpm;   mapPointer->incrementReferenceCount();
    Index I1,I2,I3;

    dpm.setOrderOfInterpolation(2);

    int numberOfGhostLines=max(4,(orderOfAccuracyInSpace+2)/2);

    I1=Range(-numberOfGhostLines,nx[0]+numberOfGhostLines-1);
    I2=Range(-numberOfGhostLines,nx[1]+numberOfGhostLines-1);
    I3= domainDimension==2 ? Range(0,0) : Range(-numberOfGhostLines,nx[2]+numberOfGhostLines-1);
    
    realArray x(I1,I2,I3,rangeDimension);

    int seed=184273654;
    srand(seed);          // supply seed to the random number generator

    int i1=0, i2=0, i3=0;
    if ( gridType==perturbedSquare )
    {
      //  === square with random perturbations ===

      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  real d1=(-1.+2.*rand()/RAND_MAX)*.25*dx[0];  // random number between [-.25,.25]*dx[0]
	  real d2=(-1.+2.*rand()/RAND_MAX)*.25*dx[1];
		
	  x(i1,i2,i3,0)= i1*dx[0]+d1;
	  x(i1,i2,i3,1)= i2*dx[1]+d2;
	}

      // make periodic
      real xba=1., yba=1., zba=1.;
      Range Rx=2;
      for( i1=I1.getBase(); i1<0; i1++ )
      {
	x(i1,I2,I3,Rx)=x(i1+nx[0]-1,I2,I3,Rx);
	x(i1,I2,I3,0)-=xba; 
      }
      for( i1=nx[0]-1; i1<=I1.getBound(); i1++ )
      {
	x(i1,I2,I3,Rx)=x(i1-nx[0]+1,I2,I3,Rx); 
	x(i1,I2,I3,0)+=xba; 
      }
	  
      for( i2=I2.getBase(); i2<0; i2++ )
      {
	x(I1,i2,I3,Rx)=x(I1,i2+nx[1]-1,I3,Rx);
	x(I1,i2,I3,1)-=yba; 
      }
      for( i2=nx[1]-1; i2<=I2.getBound(); i2++ )
      {
	x(I1,i2,I3,Rx)=x(I1,i2-nx[1]+1,I3,Rx); 
	x(I1,i2,I3,1)+=yba; 
      }
	
    }
    else 
    {
      //  === Box with random perturbations ===
      // box with random perturbations
      for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
	for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    real d1=(-1.+2.*rand()/RAND_MAX)*.25*dx[0];  // random number between [-.25,.25]*dx[0]
	    real d2=(-1.+2.*rand()/RAND_MAX)*.25*dx[1];
	    real d3=(-1.+2.*rand()/RAND_MAX)*.25*dx[2];
		
	    // if( i1<=0 || i1>=nx[0]-1 ) d1=0.;
	    // if( i2<=0 || i2>=nx[1]-1 ) d2=0.;
	    // if( i3<=0 || i3>=nx[2]-1 ) d3=0.;

	    x(i1,i2,i3,0)= i1*dx[0]+d1;
	    x(i1,i2,i3,1)= i2*dx[1]+d2;
	    x(i1,i2,i3,2)= i3*dx[2]+d3;
	  }

      // make periodic
      real xba=1., yba=1., zba=1.;
      Range Rx=3;
      for( int mm=0; mm<=1; mm++ )  // loop twice to make sure all corners points are correct
      {
	for( i1=I1.getBase(); i1<0; i1++ )
	{
	  x(i1,I2,I3,Rx)=x(i1+nx[0]-1,I2,I3,Rx);
	  x(i1,I2,I3,0)-=xba; 
	}
	for( i1=nx[0]-1; i1<=I1.getBound(); i1++ )
	{
	  x(i1,I2,I3,Rx)=x(i1-nx[0]+1,I2,I3,Rx); 
	  x(i1,I2,I3,0)+=xba; 
	}
	  
	for( i2=I2.getBase(); i2<0; i2++ )
	{
	  x(I1,i2,I3,Rx)=x(I1,i2+nx[1]-1,I3,Rx);
	  x(I1,i2,I3,1)-=yba; 
	}
	for( i2=nx[1]-1; i2<=I2.getBound(); i2++ )
	{
	  x(I1,i2,I3,Rx)=x(I1,i2-nx[1]+1,I3,Rx); 
	  x(I1,i2,I3,1)+=yba; 
	}
	  
	for( i3=I3.getBase(); i3<0; i3++ )
	{
	  x(I1,I2,i3,Rx)=x(I1,I2,i3+nx[2]-1,Rx);
	  x(I1,I2,i3,2)-=zba; 
	}
	for( i3=nx[2]-3; i3<=I3.getBound(); i3++ )
	{
	  x(I1,I2,i3,Rx)=x(I1,I2,i3-nx[2]+1,Rx);
	  x(I1,I2,i3,2)+=zba; 
	}
      }
      
    }
    
    int ng=max(2,numberOfGhostLines);
    // int numberOfGhostLinesNew[2][3]={ng,ng,ng,ng,ng,ng}; //
    Mapping::IndexRangeType numberOfGhostLinesNew;
    for( int axis=0; axis<3; axis++ )for( int side=0; side<=1; side++ ) numberOfGhostLinesNew(side,axis)=ng;
    
    dpm.setDomainDimension(domainDimension); 
    dpm.setRangeDimension(rangeDimension); 
    dpm.setNumberOfGhostLines(numberOfGhostLinesNew);
    
    // Set periodic BC's before supplying data points so the ghost points are computed properly
    if( bcOption==useAllPeriodicBoundaryConditions )
    {
      for( int axis=0; axis<domainDimension; axis++ )
      {
 	dpm.setIsPeriodic(axis,Mapping::derivativePeriodic);
 	dpm.setBoundaryCondition(Start,axis,-1);
 	dpm.setBoundaryCondition(End  ,axis,-1);
      }
    }

    dpm.setDataPoints(x,3,domainDimension,numberOfGhostLines);

    
  }
  else if( gridType==squareByTriangles || gridType==squareByQuads ||
	   (gridType==square && elementType==Maxwell::defaultUnstructured) )
  {
    Mapping & sq= *new SquareMapping(xa,xb,ya,yb); 
    sq.incrementReferenceCount();

    int side,axis;
    for( axis=0; axis<sq.getDomainDimension(); axis++ )
    {
      sq.setGridDimensions(axis,nx[axis]);
      sq.setIsPeriodic(axis,Mapping::derivativePeriodic);
      sq.setBoundaryCondition(Start,axis,-1);
      sq.setBoundaryCondition(End  ,axis,-1);
    }

    UnstructuredMapping & uns = * new UnstructuredMapping;
    mapPointer= &uns;   mapPointer->incrementReferenceCount();
    uns.addGhostElements(true);
    if( gridType==squareByTriangles )
    {
      uns.buildFromARegularMapping(sq,UnstructuredMapping::triangle);
    }
    else
    {

      uns.buildFromARegularMapping(sq,UnstructuredMapping::quadrilateral);

//        // For debugging:
//        UnstructuredMappingIterator iter;
//        for( iter=uns.begin(UnstructuredMapping::Region); iter!=uns.end(UnstructuredMapping::Region); iter++ )
//        {
//  	printf(" Element %i is valid \n",*iter);
//        }

    }

    //    uns.getEntities(UnstructuredMapping::Face).display("Faces");
    //    uns.getEntities(UnstructuredMapping::Edge).display("Edges");
    //        verifyUnstructuredConnectivity(uns,true);

//      GenericGraphicsInterface & ps = *gip;
//      GraphicsParameters params;
//      params.set(GI_PLOT_UNS_EDGES,true);
//      params.set(GI_PLOT_UNS_FACES,true);

//      PlotIt::plot(ps,uns,params);
    
    if( sq.decrementReferenceCount()==0 ) delete &sq;
  }
  else if( gridType==compositeGrid )
  {
    // *wdh* 090427 The CompositeGrid is now created in the main program

    //  // In this case we grab the Mapping from the first component grid.
    //  // create and read in a CompositeGrid
    //  delete cgp;
    //  cgp=new CompositeGrid;
    //  CompositeGrid & cg = *cgp;
    //  getFromADataBase(cg,nameOfGridFile);

    assert( cgp!=NULL);
    CompositeGrid & cg = *cgp;
    mapPointer=&(cg[0].mapping().getMapping());
  }
  else
  {
    printF("Cgmx:ERROR: unknown gridType=",(int)gridType);
    OV_ABORT("ERROR");
  }
  
  if( cgp==NULL )
  {
    // ****************************************
    // *********** single grid  ***************
    // ****************************************

    Mapping & map = *mapPointer;
  
    bool unstructured=!(elementType==structuredElements);//map.getClassName()=="UnstructuredMapping";

    int side,axis;
    if( !unstructured )
    {
      for( axis=0; axis<map.getDomainDimension(); axis++ )
      {
	map.setGridDimensions(axis,nx[axis]);
	if( map.getIsPeriodic(axis)==Mapping::notPeriodic )
	{
	  if( bcOption==useAllPeriodicBoundaryConditions )
	  {
	    map.setIsPeriodic(axis,Mapping::derivativePeriodic);
	    map.setBoundaryCondition(Start,axis,-1);
	    map.setBoundaryCondition(End  ,axis,-1);
	  }
	  else if( bcOption==useAllDirichletBoundaryConditions )
	  {
	    map.setBoundaryCondition(Start,axis,dirichlet);
	    map.setBoundaryCondition(End  ,axis,dirichlet);
	  }
	  else if( bcOption==useAllPerfectElectricalConductorBoundaryConditions )
	  {
	    map.setBoundaryCondition(Start,axis,perfectElectricalConductor);
	    map.setBoundaryCondition(End  ,axis,perfectElectricalConductor);
	  }
	}
      
      }
    }
  
    // Build a MappedGrid
    mgp = new MappedGrid(map);
    MappedGrid & mg = *mgp;
  
    if( !unstructured )
    {
      const int numberOfGhostPoints=max(2,orderOfAccuracyInSpace/2); 
      printF(">>>>Create the MappedGrid with %i ghost points, orderOfAccuracyInSpace=%i\n",numberOfGhostPoints,
	     orderOfAccuracyInSpace  );
      
      for( axis=0; axis<map.getDomainDimension(); axis++ )
	for( side=Start; side<=End; side++ )
	  mg.setNumberOfGhostPoints(side,axis,numberOfGhostPoints);
  
    }

    cgp= new CompositeGrid(mg.numberOfDimensions(),1);
    CompositeGrid & cg = *cgp;
    cg[0].reference(mg);
    cg.updateReferences();
    
//    mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );  

//    cg.update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );  
    cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask );  

    if ( unstructured )
    {
      cg.update(MappedGrid::THEcorner| MappedGrid::THEfaceArea | MappedGrid::THEfaceNormal | MappedGrid::THEcellVolume | MappedGrid::THEcenterNormal | MappedGrid::THEcenterArea );

      for ( int g=0; false && bcOption==useAllPeriodicBoundaryConditions && g<cg.numberOfGrids(); g++ )
      {
	// we do this trick for convenience when working with periodic boundaries and dsi schemes
	bool vCent = mg.isAllVertexCentered();
	realArray &cFArea = vCent ? mg.centerArea() : mg.faceArea();
	realArray &cFNorm = vCent ? mg.centerNormal() : mg.faceNormal();
	realArray &cEArea = vCent ? mg.faceArea() : mg.centerArea();
	realArray &cENorm = vCent ? mg.faceNormal() : mg.centerNormal();

	MappedGrid &mg = cg[g];
	const IntegerArray &perEImages = *mg.getUnstructuredPeriodicBC(UnstructuredMapping::Edge);
	    
	for ( int e=perEImages.getBase(0); e<=perEImages.getBound(0); e++ )
	{
	  cEArea(perEImages(e,0),0,0) = cEArea(perEImages(e,1),0,0);
	  for ( int r=0; r<mg.numberOfDimensions(); r++ )
	    cENorm(perEImages(e,0),0,0,r) = cENorm(perEImages(e,1),0,0,r);
	}

	const IntegerArray &perHImages = *mg.getUnstructuredPeriodicBC(UnstructuredMapping::Face);
	for ( int h=perHImages.getBase(0); mg.numberOfDimensions()>2 &&h<=perHImages.getBound(0); h++ )
	{
	  cFArea(perHImages(h,0),0,0) = cFArea(perHImages(h,1),0,0);
	  for ( int r=0; r<mg.numberOfDimensions(); r++ )
	    cFNorm(perHImages(h,0),0,0,r) = cFNorm(perHImages(h,1),0,0,r);
	}

      }
    }

//  display(mg.vertex(),"mg.vertex()");
    // display(mg.center(),"mg.center()");

//   display(mg.dimension(),"mg.dimension()");
//   display(mg.gridIndexRange(),"mg.gridIndexRange()");
  
//    GenericGraphicsInterface & ps = *gip;
//    PlotIt::plot(ps,mg);
  
//  mg.updateReferences();

//   mg.update(MappedGrid::THEcenterJacobian | MappedGrid::THEinverseCenterDerivative );
//   display(mg.center(),"mg.center()");
//   display(mg.centerJacobian(),"mg.centerJacobian()");
//   display(mg.inverseCenterDerivative(),"inverseCenterDerivative");
  

    if( method==defaultMethod )
    {
      if( mg.isRectangular() )
	method=yee;
      else
	method=dsi;
    }
  
    if( mapPointer->decrementReferenceCount()==0 ) delete mapPointer;

  }
  else
  {
    // *********************************************
    // *********** CompositeGrid *******************
    // *********************************************

    CompositeGrid & cg = *cgp;

    //    cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask );  

    if ( cg[0].getGridType()==MappedGrid::unstructuredGrid )
    {
      UnstructuredMapping &umap = (UnstructuredMapping &) cg[0].mapping().getMapping();
      umap.expandGhostBoundary();
      verifyUnstructuredConnectivity(umap,true);
      //	umap.expandGhostBoundary();
      //	verifyUnstructuredConnectivity(umap,true);

      cg.destroy( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask |
		  MappedGrid::THEcorner | MappedGrid::THEcellVolume | MappedGrid::THEcenterNormal |
		  MappedGrid::THEfaceArea | MappedGrid::THEfaceNormal | 
		  MappedGrid::THEcellVolume  | MappedGrid::THEcenterArea );	

      cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask |
		 MappedGrid::THEcorner | MappedGrid::THEcellVolume | MappedGrid::THEcenterNormal |
		 MappedGrid::THEfaceArea | MappedGrid::THEfaceNormal | 
		 MappedGrid::THEcellVolume  | MappedGrid::THEcenterArea );	

    }
    else
    {
      // cg.update(MappedGrid::THEmask );
      
      // *wdh* 031202 cg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );  
    }

    pinterpolant = new Interpolant(cg);
    pinterpolant->incrementReferenceCount();
#ifdef USE_PPP
    if( !pinterpolant->interpolationIsExplicit() )
    {
      printF("*** ERROR: The parallel composite grid interpolator needs explicit interpolation ****\n");
      Overture::abort();
    }
#endif

    //kkc can now use dsi, this would override the command line spec    method=nfdtd;
    
    // Set the default order of accuracy from the grid parameters
    int minDiscretizationWidth=INT_MAX;
    int minInterpolationWidth=INT_MAX;
    Range R=cg.numberOfDimensions();
    const IntegerArray & iw = cg.interpolationWidth;
    // iw.display("iw");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      const IntegerArray & dw = mg.discretizationWidth();
      
      // dw.display("dw");
      
      minDiscretizationWidth=min(minDiscretizationWidth,min(dw(R)));
      
      for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
      {
        if( grid!=grid2 )
  	  minInterpolationWidth=min( minInterpolationWidth,min(iw(R,grid,grid2)));
      }
    }
    if( minInterpolationWidth==INT_MAX ) minInterpolationWidth=minDiscretizationWidth;
    printF(" *** minDiscretizationWidth=%i, minInterpolationWidth=%i ****\n",minDiscretizationWidth,
	   minInterpolationWidth);

    const int maxOrderOfAccuracy=8;  // *************
    
    orderOfAccuracyInSpace=min(maxOrderOfAccuracy,minDiscretizationWidth-1,minInterpolationWidth-1);
    if( orderOfAccuracyInSpace%2 ==1 )
      orderOfAccuracyInSpace--;   // must be even
    
    orderOfAccuracyInTime =orderOfAccuracyInSpace;
    orderOfArtificialDissipation=orderOfAccuracyInSpace;
    
    printF("***Setting orderOfAccuracyInSpace=%i, orderOfAccuracyInTime=%i, orderOfArtificialDissipation=%i\n",
	   orderOfAccuracyInSpace,orderOfAccuracyInTime,orderOfArtificialDissipation);

    if( orderOfAccuracyInSpace>4 )
    {
      printF("***Setting useConservative=false by default for order of accuracy >4.\n");
      useConservative=false;
    }
    
  } // end compositeGrid

  timing(timeForInitialize)+=getCPU()-time0;

  // ********** By default do not solve for H in 3D ************
  if( cgp!=NULL && cgp->numberOfDimensions()==3 && method!=yee )
  {
    solveForMagneticField=false;
  }
  else if( cgp!=NULL && cgp->numberOfDimensions()==2 )
  {
    kz=0; // *wdh* 040626 
  }
  
  // These next arrays hold (eps,mu,c) in the case when they are constant on each grid but
  // may vary from grid to grid
  const int numberOfComponentGrids = mgp!=NULL ? 1 : cgp->numberOfComponentGrids();
  epsGrid.redim(numberOfComponentGrids); epsGrid=eps;
  muGrid.redim(numberOfComponentGrids);  muGrid=mu;
  cGrid.redim(numberOfComponentGrids);   cGrid=c;
  sigmaEGrid.redim(numberOfComponentGrids);  sigmaEGrid=0.;
  sigmaHGrid.redim(numberOfComponentGrids);  sigmaHGrid=0.;

 // subtract out the incident field before apply NRBC's
  adjustFarFieldBoundariesForIncidentField.redim(numberOfComponentGrids);
  adjustFarFieldBoundariesForIncidentField=0;

  return 0;
}
