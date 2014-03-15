#include "Parameters.h"
#include "Regrid.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{userDefinedErrorEstimator}}  
int 
userDefinedErrorEstimator( realCompositeGridFunction & u, 
                           real t,
                           Parameters & parameters,
                           realCompositeGridFunction & error )
// =========================================================================================
// /Description:
//   This routine is called before an adaptive mesh refinement regrid. This routine allows
// you to over-ride or add to the the error estimator used by cg. 
//
//
// /u (input) : current grid function (see below for details)
// /t (input) : current time.
// /error (input/output) : assign error values. On input this grid function will hold errors
//    computed by cg (unless the computation of these values has been turned off).
//    Grid points will be marked for refinement where the error is greater than
//    {\tt parameters.dbase.get<real >("errorThreshold")}.
//    
//
//\end{CompositeGridSolverInclude.tex} 
// =========================================================================================
{

  CompositeGrid & cg = *u.getCompositeGrid();           // current grid

    // cg parameters are available through the parameter object

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  // const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc"); 

  enum ErrorEstimatorOptionEnum
  {
    markSquare,
    excludeRegion,
    scaleByLocalValue
  };
  ErrorEstimatorOptionEnum errorEstimatorOption=excludeRegion;
  
  if( true )
    printF(" userDefinedErrorEstimator called at t=%9.3e\n",t);
      

  Index I1,I2,I3;
  const int numberOfRefinementLevels = parameters.dbase.get<Regrid* >("regrid")->getDefaultNumberOfRefinementLevels();
  for( int grid=0; grid<cg.numberOfGrids(); grid++ )
  {
    // we do not need to assign the error mask on grids at the highest allowable refinement level
    // since this info will not be used to build any refinement grids.

    if( cg.refinementLevelNumber(grid)<numberOfRefinementLevels-1 )
    {
      MappedGrid & mg = cg[grid];
      realArray & v   = u[grid];
      realArray & err = error[grid];


      if( errorEstimatorOption==excludeRegion )
      {
	// In this example we set the error to zero in a given region (where we do not want refinements)
        
        // We turn off mesh refinement in the region outside a "sphere". The radius of the
        // sphere decreases over time according to:
        //     radius(t) = outerRadius - speed*t 

	real x0=0., y0=0., z0=0.;             // center of the sphere 
	real outerRadius=75., speed=2.4;      // parameters for the converging shock problem
        real radius = outerRadius - speed*t;

        realSerialArray errLocal; getLocalArrayWithGhostBoundaries(err,errLocal);

        int i1,i2,i3;
        Index I1,I2,I3;
        getIndex( mg.dimension(),I1,I2,I3 );

        bool ok = ParallelUtility::getLocalArrayBounds(err,errLocal,I1,I2,I3);
	if( !ok ) continue;  // there are no points on this processor

	const bool isRectangular = mg.isRectangular();

        if( isRectangular )
        {
          real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
          int iv0[3]={0,0,0}; //
  	  mg.getRectangularGridParameters( dx, xab );
  	  for( int dir=0; dir<numberOfDimensions; dir++ )
  	    iv0[dir]=mg.gridIndexRange(0,dir);


          #define XC0(i1,i2,i3) (xab[0][0]+dx[0]*(i1-iv0[0]))
          #define XC1(i1,i2,i3) (xab[0][1]+dx[1]*(i2-iv0[1]))
          #define XC2(i1,i2,i3) (xab[0][2]+dx[2]*(i3-iv0[2]))

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    real rad = sqrt( SQR(XC0(i1,i2,i3)-x0)+SQR(XC1(i1,i2,i3)-y0) );
	    if( rad > radius )
	    {
	      errLocal(i1,i2,i3)=0.;
	    }
	  }


	}
	else
	{
	  // curvilinear grid
  	  cg.update(MappedGrid::THEcenter); // make sure the coordinates are available
          realSerialArray x; getLocalArrayWithGhostBoundaries(mg.center(),x);

	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    real rad = sqrt( SQR(x(i1,i2,i3,0)-x0)+SQR(x(i1,i2,i3,1)-y0) );
	    if( rad > radius )
	    {
	      errLocal(i1,i2,i3)=0.;
	    }
	  }
	}
	

      }
      else if( errorEstimatorOption==markSquare )
      {
        const intArray & mask = mg.mask();
	cg.update(MappedGrid::THEcenter);
	const realArray & x = mg.center();  // here are the grid points.
			
	// In this test case we mark a square where the error is large

	if( t<=0. ) continue;  // *****
      
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	real xa=.4, xb=.6, ya=.0, yb=.4;

	// u[grid](I1,I2,I3,rc)=1.+x(I1,I2,I3,0);  
      

	where( x(I1,I2,I3,0)>=xa && x(I1,I2,I3,0)<=xb &&
	       x(I1,I2,I3,1)>=ya && x(I1,I2,I3,1)<=yb )
	{
	  err(I1,I2,I3)=1.;
	}
      }
      else if( errorEstimatorOption==scaleByLocalValue )
      {
        // In this example we evaluate the "standard" error estimator but scale the error estimate
        // locally by the value of the solution. This is useful if the solution changes in size by
        // orders of magnitude in different parts of the domain.


        // getIndex( mg.gridIndexRange(),I1,I2,I3);
        getIndex( mg.dimension(),I1,I2,I3,-1);  // do as many points as possible
        const intArray & mask = mg.mask();
    
        #ifdef USE_PPP
          realSerialArray errLocal; getLocalArrayWithGhostBoundaries(err,errLocal);
          realSerialArray vLocal;   getLocalArrayWithGhostBoundaries(v,vLocal);
          intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
    
          bool ok = ParallelUtility::getLocalArrayBounds(err,errLocal,I1,I2,I3); 
          if( !ok ) continue;
        #else
          const realSerialArray & errLocal  = err;
          const realSerialArray & vLocal  =  v;
          const intSerialArray & maskLocal  =  mask;
        #endif
    
        real *ep = errLocal.Array_Descriptor.Array_View_Pointer3;
        const int eDim0=errLocal.getRawDataSize(0);
        const int eDim1=errLocal.getRawDataSize(1);
        const int eDim2=errLocal.getRawDataSize(2);
        #undef E
        #define E(i0,i1,i2,i3) ep[i0+eDim0*(i1+eDim1*(i2+eDim2*(i3)))]
    
        real *vp = vLocal.Array_Descriptor.Array_View_Pointer3;
        const int vDim0=vLocal.getRawDataSize(0);
        const int vDim1=vLocal.getRawDataSize(1);
        const int vDim2=vLocal.getRawDataSize(2);
        #undef V
        #define V(i0,i1,i2,i3) vp[i0+vDim0*(i1+vDim1*(i2+vDim2*(i3)))]
    
        const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
        const int maskDim0=maskLocal.getRawDataSize(0);
        const int maskDim1=maskLocal.getRawDataSize(1);
        const int md1=maskDim0, md2=md1*maskDim1; 
        #define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]
    
        const int numberOfComponents=u.getComponentBound(0)-u.getComponentBase(0)+1;
        for( int c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )
        {
    
          // assign basic scale factors for this component:
          real c1 = 1.; 
          real c2 = 1.; 
    
	  int i1,i2,i3;
	  if( cg.numberOfDimensions()==2 )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3)>0 )
	      {
                // Form the solution dependent scale factors:
                real uScale = fabs(V(i1,i2,i3,c))+1.; // scale by this positive number
                real c1u = c1/uScale;
                real c2u = c2/uScale;
  
		E(i1,i2,i3,0) += (fabs(V(i1+1,i2  ,i3,c)-2.*V(i1,i2,i3,c)+V(i1-1,i2  ,i3,c))*c2u+
				  fabs(V(i1  ,i2+1,i3,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2-1,i3,c))*c2u+
				  fabs(V(i1+1,i2  ,i3,c)-V(i1-1,i2  ,i3,c))*c1u+
				  fabs(V(i1  ,i2+1,i3,c)-V(i1  ,i2-1,i3,c))*c1u	);
	      }
	    }
	  }
	  else if( cg.numberOfDimensions()==3 )
	  {
	    FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      if( MASK(i1,i2,i3)>0 )
	      {
                // Form the solution dependent scale factors:
                real uScale = fabs(V(i1,i2,i3,c))+1.; // scale by this positive number
                real c1u = c1/uScale;
                real c2u = c2/uScale;
  
		E(i1,i2,i3,0) += (fabs(V(i1+1,i2  ,i3  ,c)-2.*V(i1,i2,i3,c)+V(i1-1,i2  ,i3  ,c))*c2u+
				  fabs(V(i1  ,i2+1,i3  ,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2-1,i3  ,c))*c2u+
				  fabs(V(i1  ,i2  ,i3+1,c)-2.*V(i1,i2,i3,c)+V(i1  ,i2  ,i3-1,c))*c2u+
				  fabs(V(i1+1,i2  ,i3  ,c)-V(i1-1,i2  ,i3  ,c))*c1u+
				  fabs(V(i1  ,i2+1,i3  ,c)-V(i1  ,i2-1,i3  ,c))*c1u+
				  fabs(V(i1  ,i2  ,i3+1,c)-V(i1  ,i2  ,i3-1,c))*c1u	);
     
	      }
	    }
	  }
          
        } // end for c
        #undef E
        #undef V
        #undef MASK	
  
  
      }
      else
      {
        printF("userDefinedErrorEstimator:ERROR: unknown errorEstimatorOption!\n");
	
      }
      
    } // if refinement level
    
    
  } // end for grid
  

  return 0;
}



