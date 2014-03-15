#include "Ogmg.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "display.h"
#include "PlotStuff.h"
#include "ParallelUtility.h"

#undef ForBoundary
#define ForBoundary(side,axis)  \
       for( axis=0; axis<cg.numberOfDimensions(); axis++ ) \
         for( side=0; side<=1; side++ )

int Ogmg::
coarseGridSolverTest( int plotOption /* =0  */ )
// ===============================================================================================
// test the accuracy of the coarse grid equations
// ===============================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int level = mgcg.numberOfMultigridLevels()-1;  // coarse level
  
  realCompositeGridFunction & u = uMG.multigridLevel[level];
  realCompositeGridFunction & f = fMG.multigridLevel[level];
  CompositeGrid & cg = *u.getCompositeGrid();

  int twilightZone=2;  // 1=trig, 2=poly
  
  real fx=1., fy=1., fz=1.;
  OGTrigFunction tzTrig(fx,fy,fz);  // create an exact solution

  if( directSolver.isSolverIterative() ) 
  {
    real tol=REAL_EPSILON*100.;
    // directSolver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    directSolver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
    directSolver.set(OgesParameters::THEmaximumNumberOfIterations,10000);
  } 


  const int maxDegree= twilightZone==2 ? orderOfAccuracy : 0;
  for( int degree=0; degree<=maxDegree; degree++ )
  {
    printF("\n-------------------------------------------------------------------------------------\n");
    if( twilightZone==2 )
      printF("test a polynomial of degree %i...\n",degree);

    int degreeOfSpacePolynomial=degree;
  
    OGPolyFunction tzPoly(degreeOfSpacePolynomial,cg.numberOfDimensions());      // create an exact solution
    OGFunction & tz = twilightZone==1 ? (OGFunction&)tzTrig : (OGFunction&)tzPoly;

    const int numberOfComponents=1;
  
    RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5,numberOfComponents);      
    timeCoefficientsForTZ=0.;
    timeCoefficientsForTZ(0,0)=1.;
    if( degreeOfSpacePolynomial==0 )
    {
      spatialCoefficientsForTZ(0,0,0)=1.; 
    }
    else if( degreeOfSpacePolynomial==2 )
    {
      spatialCoefficientsForTZ(1,0,0)=-.5;
      spatialCoefficientsForTZ(0,1,0)=-.5;
      spatialCoefficientsForTZ(2,0,0)=1.;  // x^2
      spatialCoefficientsForTZ(0,2,0)=1.;
      if( cg.numberOfDimensions()==3 )
      {
	spatialCoefficientsForTZ(0,0,1)=-.5;
	spatialCoefficientsForTZ(0,0,2)=.5;
      }
    }
    else if( degreeOfSpacePolynomial==1 )
    {
      spatialCoefficientsForTZ(1,0,0)=1.; 
      spatialCoefficientsForTZ(0,1,0)=1.;
      if( cg.numberOfDimensions()==3 )
	spatialCoefficientsForTZ(0,0,1)=-.25;
    }
    else if( degreeOfSpacePolynomial==4 )
    {
      spatialCoefficientsForTZ(1,0,0)=-.5;
      spatialCoefficientsForTZ(0,1,0)= .5;

      spatialCoefficientsForTZ(2,0,0)= .3;  // x^2
      spatialCoefficientsForTZ(0,2,0)=-.3;

      spatialCoefficientsForTZ(3,1,0)= .1;  // x^2
      spatialCoefficientsForTZ(1,3,0)=-.1;

      spatialCoefficientsForTZ(4,0,0)=.2;  // x^4
      spatialCoefficientsForTZ(0,4,0)=.1;  // y^4

      if( cg.numberOfDimensions()==3 )
      {
	spatialCoefficientsForTZ(0,0,1)= .6;
	spatialCoefficientsForTZ(0,0,2)=-.3;
	spatialCoefficientsForTZ(0,0,4)= .1;

      }
    
    }
    else
    {
      spatialCoefficientsForTZ(0,0,0)=1.; 
    }
    
    tzPoly.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  


    Index I1,I2,I3;
    Index I1b,I2b,I3b,I1g,I2g,I3g;

    // assign the rhs for all the equations (including BC's)
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
    
      getIndex(mg.dimension(),I1,I2,I3);
      #ifdef USE_PPP
	realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
	realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(mg.center(),xLocal);

	bool ok=ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,1);
	if( !ok ) continue;
	  
      #else
	realSerialArray & fLocal = f[grid];
	const realSerialArray & xLocal = mg.center();
      #endif

      const bool isRectangular=false;  // do this for now
      realSerialArray ue(I1,I2,I3), uLap(I1,I2,I3); 
      int ntd=0, nxd=2, nyd=0, nzd=0; 
      tz.gd( uLap,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.xx
      nxd=0; nyd=2; nzd=0;
      tz.gd( ue  ,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.yy 
      uLap+=ue;
      if( mg.numberOfDimensions()==3 )
      {
	nxd=0; nyd=0; nzd=2;
	tz.gd( ue,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.zz
	uLap+=ue;
      }
      nxd=0; nyd=0; nzd=0;
      tz.gd( ue,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.);	 // e


      if( equationToSolve==OgesParameters::heatEquationOperator )
      {
	fLocal(I1,I2,I3)=equationCoefficients(0,grid)*ue(I1,I2,I3)+equationCoefficients(1,grid)*uLap(I1,I2,I3); 
      }
      else
      {
	fLocal(I1,I2,I3)=uLap(I1,I2,I3);
      }
      
      // Assign Boundary Conditions 
      int side,axis;
      ForBoundary(side,axis)
      {
	getBoundaryIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b);
	getGhostIndex   (mg.gridIndexRange(),side,axis,I1g,I2g,I3g);
	bool ok=ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1b,I2b,I3b,1);
	ok=ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1g,I2g,I3g,1);
	if( !ok ) continue;

	if( mg.boundaryCondition(side,axis)==OgmgParameters::dirichlet )
	{
	  fLocal(I1b,I2b,I3b)=ue(I1b,I2b,I3b);
          if( orderOfAccuracy==2 )
	  {
  	    fLocal(I1g,I2g,I3g)=0.; // extrap
	  }
          else
	  {
            // for 4th order we fill in the eqn at the bndry as the rhs at the ghost point
	    if( equationToSolve==OgesParameters::heatEquationOperator )
	      fLocal(I1g,I2g,I3g)=equationCoefficients(0,grid)*ue(I1b,I2b,I3b)+
 		                  equationCoefficients(1,grid)*uLap(I1b,I2b,I3b);
            else
	      fLocal(I1g,I2g,I3g)=uLap(I1b,I2b,I3b); // tz.laplacian(cg[grid],I1b,I2b,I3b);
	  }
	}
	else
	{
	  realSerialArray uex(I1b,I2b,I3b), uey(I1b,I2b,I3b), uez;
	  nxd=1; nyd=0; nzd=0;
          tz.gd( uex,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.x
	  nxd=0; nyd=1; nzd=0;
          tz.gd( uey,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.y
	  if( mg.numberOfDimensions()==3 )
	  {
	    uez.redim(I1b,I2b,I3b);
	    nxd=0; nyd=0; nzd=1;
	    tz.gd( uez,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e.zz
	  }

	  if( mg.isRectangular() )
	  {
	    if( axis==axis1 )
	      fLocal(I1g,I2g,I3g)=uex(I1b,I2b,I3b)*(2*side-1.); // normal is 2*side-1
	    else if( axis==axis2 )
	      fLocal(I1g,I2g,I3g)=uey(I1b,I2b,I3b)*(2*side-1.);
	    else
	      fLocal(I1g,I2g,I3g)=uez(I1b,I2b,I3b)*(2*side-1.);
	  }
	  else
	  {
	    mg.update(MappedGrid::THEvertexBoundaryNormal );
            #ifdef USE_PPP
              const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            #else
              const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
            #endif
	    if( mg.numberOfDimensions()==2 )
	      fLocal(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*uex(I1b,I2b,I3b)
		                 +normal(I1b,I2b,I3b,1)*uey(I1b,I2b,I3b);
	    else 
	      fLocal(I1g,I2g,I3g)=normal(I1b,I2b,I3b,0)*uex(I1b,I2b,I3b)
	         	         +normal(I1b,I2b,I3b,1)*uey(I1b,I2b,I3b)
		                 +normal(I1b,I2b,I3b,2)*uez(I1b,I2b,I3b);
	  }
	  
	  if( mg.boundaryCondition(side,axis)==OgmgParameters::mixed )
	  {
	    fLocal(I1g,I2g,I3g)=bcParams.a(0)*ue(I1b,I2b,I3b)+bcParams.a(1)*fLocal(I1g,I2g,I3g);
	  }

          if( orderOfAccuracy==4 )
	  {
            // *wdh* 100705 -- lower level Neumann/mixed BC applied BC at 2nd ghost line too
	    Index J1g,J2g,J3g;
	    getGhostIndex(mg.gridIndexRange(),side,axis,J1g,J2g,J3g,2); // second ghost 
	    bool ok=ParallelUtility::getLocalArrayBounds(f[grid],fLocal,J1g,J2g,J3g,1);
	    if( ok )
	    {
              fLocal(J1g,J2g,J3g)=fLocal(I1g,I2g,I3g); // same RHS as first ghost lint 
	    }
	    

	  }
	  

	}
      }
    }
  

    u=0.;   // initial guess

    // ************************
    // ********Solve***********
    // ************************
    directSolver.solve( u,f );

    printF("coarseGridSolver: max residual=%8.2e (iterations=%i) ***\n",directSolver.getMaximumResidual(),
	   directSolver.getNumberOfIterations());


    RealCompositeGridFunction e(cg);
    e=0.;
    RealArray error(cg.numberOfComponentGrids());
    error=0.;
    real uMax=0.;
    int numGhost=1;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )     
    {
      MappedGrid & mg = cg[grid];
      mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
      
      getIndex(mg.gridIndexRange(),I1,I2,I3,numGhost);         // include ghost points 

      OV_GET_SERIAL_ARRAY(real,u[grid],uLocal);
      OV_GET_SERIAL_ARRAY(real,e[grid],eLocal);
      OV_GET_SERIAL_ARRAY(real,mg.center(),xLocal);
      OV_GET_SERIAL_ARRAY(int,mg.mask(),maskLocal);

      bool ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);

      if( ok )
      {
	realSerialArray uExact(I1,I2,I3);
	int ntd=0, nxd=0, nyd=0, nzd=0;
	const bool isRectangular=false;  // do this for now
	tz.gd( uExact,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,I1,I2,I3,0,0.); // e

	where( maskLocal(I1,I2,I3)!=0 )
	{
	  uMax=max(uMax,max(fabs(uExact)));
	
	  eLocal(I1,I2,I3)=fabs(uLocal(I1,I2,I3)-uExact);
	  error(grid)=max(fabs(eLocal(I1,I2,I3)));
	}
      }
      
      error(grid)=ParallelUtility::getMaxValue(error(grid));
    }
    uMax=ParallelUtility::getMaxValue(uMax);

    if( Oges::debug & 16 )
    {
      u.display("Solution from coarseGridSolver","%6.2f ");
      e.display("Error from coarseGridSolver","%8.1e ");
    }

    if( twilightZone==2 )
    {
      printF(" coarseGridSolverTest: exact solution is a polynomial of degree %i\n",degreeOfSpacePolynomial);
    }
    else
    {
      printF(" coarseGridSolverTest: exact solution is a trigonometric polynomial\n");
    }
  
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )     
    {
      printF("coarseGridSolverTest: Maximum error on grid: %15s = %8.2e, max relative error=%8.2e\n",
	     (const char *)cg[grid].mapping().getName(Mapping::mappingName),error(grid),error(grid)/uMax);
    }
    printF("-------------------------------------------------------------------------------------\n");
    
    if( plotOption )
    {
      // plot the errors
      GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("ogmgt", true);
      PlotStuffParameters psp;
      psp.set(GI_TOP_LABEL,"error"); 
      PlotIt::contour(ps,e,psp);

    }

  }
  
  

//      if( Ogmg::debug & 8 )
//        e.display("error including ghost points","%6.2e ");


  return 0;
}








//--------------------------------------------------------------------------------------
//  Test the smoothers
//--------------------------------------------------------------------------------------
int Ogmg::
smoothTest(GenericGraphicsInterface & ps, int plotOption )
{

//    uMG.reference(u0);  
//    fMG.reference(f0);
  CompositeGrid & mgcg = multigridCompositeGrid();
  realCompositeGridFunction & u0 = uMG;
  realCompositeGridFunction & f0 = fMG;

  const int np= max(1,Communication_Manager::numberOfProcessors());

  printF("\n"
         " ============== Ogmg::smoothTest np=%i =================\n",np);

//   printf("u0.numberOfGrids = %i \n",u0.numberOfGrids());
//   printf("u0.multigridLevel[0].numberOfGrids = %i \n",u0.multigridLevel[0].numberOfGrids());

//   printf("uMG.numberOfGrids = %i \n",uMG.numberOfGrids());
//   printf("uMG.multigridLevel[0].numberOfGrids = %i \n",uMG.multigridLevel[0].numberOfGrids());

  parameters.numberOfSubSmooths=1;
  

  for( int level=0; level<mgcg.numberOfMultigridLevels()-1; level++ )
  {
    printF("====================Level = %i ======================\n",level);
    realCompositeGridFunction & u = u0.multigridLevel[level];
    realCompositeGridFunction & f = f0.multigridLevel[level];
    realCompositeGridFunction & df = defectMG.multigridLevel[level];
    CompositeGrid & cg = mgcg.multigridLevel[level];
    
    u=0.;  // initial guess
    f=1.;  
    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.dimension(),I1,I2,I3);
      bool ok=true;
      #ifdef USE_PPP
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
	realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
	ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
      #else
	realSerialArray & uLocal = u[grid];
	realSerialArray & fLocal = f[grid];
	intSerialArray & maskLocal = mg.mask();
      #endif

      if( ok )
      {
	where( maskLocal(I1,I2,I3)==0 )
	{
	  uLocal(I1,I2,I3)=123456789.;  // put bogus values at unused points
	}
      }
      
      // fill in the BC's into f
      for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( mg.boundaryCondition(side,axis) ==OgmgParameters::dirichlet  )
	  {
	    getBoundaryIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3);
	    ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
	    if( !ok ) continue;
	    fLocal(I1,I2,I3)=0.;   // NOTE: must be zero to work at level > 0
	  }
	  else if( mg.boundaryCondition(side,axis) ==OgmgParameters::neumann ||
                   mg.boundaryCondition(side,axis) ==OgmgParameters::mixed ||
                   mg.boundaryCondition(side,axis) ==OgmgParameters::extrapolate )  
	  {
	    getGhostIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3);
	    ok=ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
	    if( !ok ) continue;
	    fLocal(I1,I2,I3)=0.;   // NOTE: must be zero to work at level > 0
	  }
	  else if( mg.boundaryCondition(side,axis)>0 )
	  {
	    printF("smoothTest::ERROR: unknown BC \n");
	    OV_ABORT("error");
	  }
	}
      }
      if( Ogmg::debug & 8 )
	display(f[grid],"Here is f for the smooth test",Ogmg::debugFile);

    }
    
    defect(level);
    // *wdh* 091216 real maximumDefectOld=max(df);
    real maximumDefectOld=maxNorm(df);
    printF(" smoothTest: initial defect=%8.4e\n",maximumDefectOld);
    int numIterations=25;
    numberOfCycles=0;
    for( int it=0; it<numIterations; it++ )
    {
      int numberOfSmoothingSteps=parameters.numberOfSmooths(0,level)+parameters.numberOfSmooths(1,level);

      smooth(level,numberOfSmoothingSteps,it);
      numberOfCycles++;
      
      defect(level);

      // *wdh* 091216 real maximumDefect=max(fabs(df));
      real maximumDefect=maxNorm(df);

      if( Ogmg::debug & 32 )
        df.display("defect");

      printF(" smoothTest: it= %4i,  max(defect) = %8.4e, defect/defectOld = %6.3f\n",
	     it,maximumDefect,maximumDefect/(maximumDefectOld+FLT_MIN));

      if( debug & 2 )
      {
        int grid=0;
        realArray & u = u0.multigridLevel[level][grid];
        ::display(u,sPrintF("u level=%i it=%i", level,it),debugFile,"%10.7f ");
        ::display(df[grid],sPrintF("df level=%i it=%i", level,it),debugFile,"%10.7f ");
      }
      

      maximumDefectOld=maximumDefect;
      if( maximumDefect<1.e-5 )
        break;

      if( plotOption )
      {
        PlotStuffParameters psp;

	aString answer;
	aString menu[]=
	{
	  "solution",
	  "error",
	  "defect",
	  "grid",
	  "rhs (for Ogmg)",
	  "exit",
	  ""
	};
    
	for( ;; )
	{
	  ps.getMenuItem(menu,answer,"choose an option");
	  if( answer=="exit" )
	  {
	    break;
	  }
	  else if( answer=="solution" )
	  {
	    psp.set(GI_TOP_LABEL,"Solution u"); 
	    PlotIt::contour(ps,u,psp);
	  }
	  else if( answer=="defect" )
	  {
	    psp.set(GI_TOP_LABEL,"defect"); 
	    PlotIt::contour(ps,df,psp);
	  }
	  else if( answer=="rhs" )
	  {
	    psp.set(GI_TOP_LABEL,"fMG"); 
	    PlotIt::contour(ps,f,psp);
	  }
	  else if( answer=="grid" )
	  {
	    psp.set(GI_TOP_LABEL,"grid"); 
	    PlotIt::plot(ps,cg,psp);
	  }
      
	}
      }
      
    }
  }
  return 0;
}

//--------------------------------------------------------------------------------------
//  Test the coarse to fine transfer operator
//--------------------------------------------------------------------------------------
int Ogmg::
coarseToFineTest()
{

//    uMG.reference(u);  
//    fMG.reference(f);

  CompositeGrid & mgcg = multigridCompositeGrid();
  realCompositeGridFunction & u = uMG;
  realCompositeGridFunction & f = fMG;
  

  const int degreeSpace=2;
  OGPolyFunction exact(degreeSpace,mgcg.numberOfDimensions());
  const int numberOfComponents=1;
  RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
  spatialCoefficientsForTZ=0.;
  RealArray timeCoefficientsForTZ(5,numberOfComponents);      
  timeCoefficientsForTZ=0.;
  timeCoefficientsForTZ(0,0)=1.;

  Index I1,I2,I3;
  int grid;
  aString buff;
  
  // debug=31;  // ***********************
  int debugcf=debug;

  useForcingAsBoundaryConditionOnAllLevels=true; // use f so define the dirichlet BC on all levels (normally only level=0)
  

  mgcg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
  
  for( int level=0; level<mgcg.numberOfMultigridLevels()-1; level++ )
  {
    printF("==================== level = %i ======================\n",level);
    realCompositeGridFunction & uCoarse = u.multigridLevel[level+1];
    realCompositeGridFunction & uFine   = u.multigridLevel[level];

    realCompositeGridFunction & fFine   = f.multigridLevel[level];

    CompositeGrid & cgCoarse = mgcg.multigridLevel[level+1];
    CompositeGrid & cgFine = mgcg.multigridLevel[level];
    
//    uCoarse.display("uCoarse");
    

//      CompositeGridOperators & op = *uFine.getOperators();
  
//      op.setTwilightZoneFlow(TRUE);           // set twilight-zone flow for level 0 only
//      op.setTwilightZoneFlowFunction(exact);

    realCompositeGridFunction ucExact(cgCoarse);
    realCompositeGridFunction ufExact(cgFine);
    


    const int maxDegree=2;
    for( int degree=0; degree<=maxDegree; degree++ )
    {
      spatialCoefficientsForTZ=0.;
      spatialCoefficientsForTZ(0,0,0,0)=1.;
      if( degree>0 )
      {
	spatialCoefficientsForTZ(0,0,0,0)=1.;
	spatialCoefficientsForTZ(1,0,0,0)=1.;
	spatialCoefficientsForTZ(0,1,0,0)=2.;
	if( mgcg.numberOfDimensions()==3 )
	  spatialCoefficientsForTZ(0,0,1,0)=-1.;
      }
      if( degree>1 )
      {
	spatialCoefficientsForTZ(2,0,0,0)=-1.;
	spatialCoefficientsForTZ(0,2,0,0)=1.5;
	if( mgcg.numberOfDimensions()==3 )
	  spatialCoefficientsForTZ(0,0,2,0)=3.;
      }
      exact.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
      exact.assignGridFunction(ufExact);
      exact.assignGridFunction(ucExact);
      

      uFine=0.;
      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cgCoarse[grid];
        MappedGrid & mgFine = cgFine[grid];

        #ifdef USE_PPP
  	  realSerialArray ucLocal; getLocalArrayWithGhostBoundaries(uCoarse[grid],ucLocal);
  	  realSerialArray ufeLocal; getLocalArrayWithGhostBoundaries(ufExact[grid],ufeLocal);
  	  realSerialArray uceLocal; getLocalArrayWithGhostBoundaries(ucExact[grid],uceLocal);
  	  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
  	  realSerialArray fFineLocal; getLocalArrayWithGhostBoundaries(fFine[grid],fFineLocal);
        #else
  	  realSerialArray & ucLocal=uCoarse[grid];
  	  realSerialArray & ufeLocal=ufExact[grid];
  	  realSerialArray & uceLocal=ucExact[grid];
  	  intSerialArray & maskLocal=mg.mask();
  	  realSerialArray & fFineLocal=fFine[grid];
        #endif

	getIndex(mg.dimension(),I1,I2,I3);
  	bool ok=ParallelUtility::getLocalArrayBounds(uCoarse[grid],ucLocal,I1,I2,I3,1);
	if( ok )
	{
	  ucLocal(I1,I2,I3)=uceLocal(I1,I2,I3);
	  where( maskLocal(I1,I2,I3)==0 )
	    ucLocal(I1,I2,I3)=123456789.;  // put bogus values in un-used points.
	}
	
        // For predefined equations we should set the RHS for dirichlet BC's
	for( int axis=axis1; axis<mgcg.numberOfDimensions(); axis++ )
	{
	  for( int side=Start; side<=End; side++ )
	  {
	    if( mgFine.boundaryCondition(side,axis)>0 ) //  ==dirichlet  )
	    {
	      getBoundaryIndex(mgFine.gridIndexRange(),side,axis,I1,I2,I3);
              ok=ParallelUtility::getLocalArrayBounds(fFine[grid],fFineLocal,I1,I2,I3,1);
	      if( ok )
		fFineLocal(I1,I2,I3)=ufeLocal(I1,I2,I3);
	    }
	  }
	}
	
      }
      if( debugcf & 16 )
	uCoarse.display(sPrintF(buff,"coarseToFine: BEFORE:uCoarse level=%i degree=%i",level,degree),NULL,"%7.1e ");
      if( debugcf & 16 )
	uFine.display(sPrintF(buff,"coarseToFine: BEFORE:uFine level=%i degree=%i",level,degree),NULL,"%7.1e ");

      coarseToFine( level );

      if( debugcf & 16 )
	uFine.display(sPrintF(buff,"coarseToFine: AFTER:uFine level=%i degree=%i",level,degree),NULL,"%7.1e ");

      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cgFine[grid];
	getIndex(extendedGridIndexRange(mg),I1,I2,I3);

        #ifdef USE_PPP
  	  realSerialArray ufLocal; getLocalArrayWithGhostBoundaries(uFine[grid],ufLocal);
  	  realSerialArray ufeLocal; getLocalArrayWithGhostBoundaries(ufExact[grid],ufeLocal);
  	  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
        #else
  	  realSerialArray & ufLocal=uFine[grid];
  	  realSerialArray & ufeLocal=ufExact[grid];
  	  intSerialArray & maskLocal=mg.mask();
        #endif

  	bool ok=ParallelUtility::getLocalArrayBounds(uFine[grid],ufLocal,I1,I2,I3,1);
	real error=0.;
	if( ok )
	{
	  where( maskLocal(I1,I2,I3)!=0 )
	    error= max(fabs(ufLocal(I1,I2,I3)-ufeLocal(I1,I2,I3)));
	}
	error=ParallelUtility::getMaxValue(error);
	printF("  Polynomial degree=%i, maximum error in coarse to fine on grid=%i is %e \n",degree,grid,error);
	if( ok && debugcf & 16 )
	{
	  realSerialArray err(I1,I2,I3);
	  err=0;
	  where( maskLocal(I1,I2,I3)!=0 )
	    err=fabs(ufLocal(I1,I2,I3)-ufeLocal(I1,I2,I3));
	  display(err,sPrintF(buff,"coarseToFine Error level=%i grid=%i degree=%i",level,grid,degree),NULL,"%6.1e ");
	}
      }
    }
  }
  useForcingAsBoundaryConditionOnAllLevels=false;
  
  return 0;
}

//--------------------------------------------------------------------------------------
//  Test the fine to coarse transfer operator
//--------------------------------------------------------------------------------------
int Ogmg::
fineToCoarseTest()
{
  const int np= max(1,Communication_Manager::numberOfProcessors());

  CompositeGrid & mgcg = multigridCompositeGrid();
  realCompositeGridFunction & u = uMG;
  realCompositeGridFunction & f = fMG;

//    uMG.reference(u);  
//    fMG.reference(f);

//  realCompositeGridFunction defect(mgcg);
//  defectMG.reference(defect);  

  const int degreeSpace=2;
  OGPolyFunction exact(degreeSpace,mgcg.numberOfDimensions());
  const int numberOfComponents=1;
  RealArray spatialCoefficientsForTZ(5,5,5,numberOfComponents);  
  spatialCoefficientsForTZ=0.;
  RealArray timeCoefficientsForTZ(5,numberOfComponents);      
  timeCoefficientsForTZ=0.;
  timeCoefficientsForTZ(0,0)=1.;

  mgcg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

  Index I1,I2,I3;
  int grid;
  for( int level=0; level<mgcg.numberOfMultigridLevels()-1; level++ )
  {
    printF("==================== Level = %i ======================\n",level);
    realCompositeGridFunction & fCoarse      = fMG.multigridLevel[level+1];
    realCompositeGridFunction & defectFine   = defectMG.multigridLevel[level];
    CompositeGrid & cgCoarse                 = mgcg.multigridLevel[level+1];
    CompositeGrid & cgFine                   = mgcg.multigridLevel[level];
    
    realCompositeGridFunction ucExact(cgCoarse);
    realCompositeGridFunction ufExact(cgFine);

    for( int degree=0; degree<=2; degree++ )
    {
      spatialCoefficientsForTZ=0.;
      spatialCoefficientsForTZ(0,0,0,0)=1.;
      if( degree>0 )
      {
	spatialCoefficientsForTZ(0,0,0,0)=1.;
	spatialCoefficientsForTZ(1,0,0,0)=1.;
	spatialCoefficientsForTZ(0,1,0,0)=2.;
	if( mgcg.numberOfDimensions()==3 )
	  spatialCoefficientsForTZ(0,0,1,0)=-1.;
      }
      if( degree>1 )
      {
	spatialCoefficientsForTZ(2,0,0,0)=-1.;
	spatialCoefficientsForTZ(0,2,0,0)=1.5;
	if( mgcg.numberOfDimensions()==3 )
	  spatialCoefficientsForTZ(0,0,2,0)=3.;
      }
      exact.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
      exact.assignGridFunction(ufExact);
      exact.assignGridFunction(ucExact);
    
      fCoarse=0.;
      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cgFine[grid];
        #ifdef USE_PPP
  	  realSerialArray ufeLocal; getLocalArrayWithGhostBoundaries(ufExact[grid],ufeLocal);
  	  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
  	  realSerialArray defectFineLocal; getLocalArrayWithGhostBoundaries(defectFine[grid],defectFineLocal);
        #else
  	  realSerialArray & ufeLocal=ufExact[grid];
  	  intSerialArray & maskLocal=mg.mask();
  	  realSerialArray & defectFineLocal=defectFine[grid];
        #endif

	getIndex(mg.dimension(),I1,I2,I3);
  	bool ok=ParallelUtility::getLocalArrayBounds(defectFine[grid],defectFineLocal,I1,I2,I3,1);
	if( ok )
	{
	  defectFineLocal(I1,I2,I3)=ufeLocal(I1,I2,I3);
	  where( maskLocal(I1,I2,I3) ==0 )
	    defectFineLocal(I1,I2,I3)=123456789.;  // put bogus values in un-used points.
	}
	
      }

      if( false && level==1 ) // *** testing ***
      {
	// int grid=1, gridI=0; 
	int grid=0, gridI=1; 

//  	display(defectFine[grid],sPrintF("Before interp: defectFine, grid=%i, level=%i, degree=%i",grid,level,degree),
//                    debugFile,"%8.2e ");

        intArray & ip = cgFine.interpolationPoint[grid];
	intArray & il = cgFine.interpoleeLocation[grid];
	intArray & varWidth = cgFine.variableInterpolationWidth[grid];
        realArray & u = defectFine[gridI];

	for( int i=0 ; i<cgFine.numberOfInterpolationPoints(grid); i++ )
	{
          int width = varWidth(i);
	  int i1=il(i,0), i2=il(i,1), i3=0;
          real sum=0.;
	  for( int iw1=0; iw1<width; iw1++ )
	  for( int iw2=0; iw2<width; iw2++ )
	  {
	    sum+=u(i1+iw1,i2+iw2,i3);
	  }
          if( fabs(sum)>10. )
	  {
   	    fPrintF(debugFile,"*** i=%i ip=(%i,%i) il=(%i,%i) sum=%e \n",i,ip(i,0),ip(i,1),i1,i2,sum);
	    for( int iw1=0; iw1<width; iw1++ )
	      for( int iw2=0; iw2<width; iw2++ )
	      {
		fPrintF(debugFile,"...u(%i,%i) = %e \n",i1+iw1,i2+iw2,u(i1+iw1,i2+iw2,i3));
		
	      }
	  
	  }
	  
	}
	fPrintF(debugFile," ******************* done checking interpolation ********\n");
	
//         interpolate( defectFine );
       
//  	display(defectFine[grid],sPrintF("After interp: defectFine, grid=%i, level=%i, degree=%i",grid,level,degree),
//                    debugFile,"%8.2e ");

      }
      
       
     if( debug & 16 )
	defectFine.display("Here is defectFine before fineToCoarse");

      fineToCoarse( level );

      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cgCoarse[grid];

        #ifdef USE_PPP
  	  realSerialArray fcLocal; getLocalArrayWithGhostBoundaries(fCoarse[grid],fcLocal);
  	  realSerialArray uceLocal; getLocalArrayWithGhostBoundaries(ucExact[grid],uceLocal);
  	  intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
        #else
  	  realSerialArray & fcLocal=fCoarse[grid];
  	  realSerialArray & uceLocal=ucExact[grid];
  	  intSerialArray & maskLocal=mg.mask();
        #endif

	getIndex(extendedGridIndexRange(mg),I1,I2,I3,-1);  // -1: not not check boundary (dirichlet condition)
  	bool ok=ParallelUtility::getLocalArrayBounds(fCoarse[grid],fcLocal,I1,I2,I3,1);
	real error=0.;
	if( ok )
	{
	  where( maskLocal(I1,I2,I3) > 0 )
	    error= max(fabs(fcLocal(I1,I2,I3)-uceLocal(I1,I2,I3)));
	}
        error=ParallelUtility::getMaxValue(error);

	printF("  degree=%i, maximum error in fine to coarse on grid=%i is %e \n",degree,grid,error);

	display(defectFine[grid],sPrintF("defectFine on grid=%i, level=%i, degree=%i",grid,level,degree),
		debugFile,"%8.2e ");
	display(fCoarse[grid],sPrintF("fCoarse on grid=%i, level=%i, degree=%i",grid,level,degree),
		debugFile,"%8.2e ");


	if( ok && ( (grid==1 && level==1) || debug & 8) )
	{
	  realSerialArray err(I1,I2,I3);
          err= fabs(fcLocal(I1,I2,I3)-uceLocal(I1,I2,I3));
	  where( maskLocal(I1,I2,I3) <= 0 )
            err=0.;

	  display(err,sPrintF("The error in fCoarse on grid=%i, level=%i, degree=%i",grid,level,degree),
                  debugFile,"%6.1e ");
	}
      } // end for grid
      
      if( debug & 16 )
      {
	fCoarse.display("Here is fCoarse after fineToCoarse");
      }

      // -- timing: call a few times to get better stats
      tm[timeForInterpolateCoarseFromFine]=0.;

      int nit=50;
      real time0=getCPU();
      for( int it=0; it<nit; it++ )
	fineToCoarse( level );

      real time = getCPU()-time0;
      time=ParallelUtility::getMaxValue(time)/nit;
      real timeCF = ParallelUtility::getMaxValue(tm[timeForInterpolateCoarseFromFine])/nit;
      printF(" np=%i, cpu=%9.2e(s), interp-coarse-from-fine=%9.2e(s)\n", np,time,timeCF);


    }

  }
  
  return 0;
}


int Ogmg::
bcTest()
// =======================================================================================
// /Description:
//    Test the boundary conditions.
// =======================================================================================
{

  CompositeGrid & mgcg = multigridCompositeGrid();
  realCompositeGridFunction & u0 = uMG;
  realCompositeGridFunction & f0 = fMG;
//    uMG.reference(u0);  
//    fMG.reference(f0);

  printf("bcTest: u0.numberOfGrids = %i \n",u0.numberOfGrids());
  printf("bcTest: u0.multigridLevel[0].numberOfGrids = %i \n",u0.multigridLevel[0].numberOfGrids());

  printf("bcTest: uMG.numberOfGrids = %i \n",uMG.numberOfGrids());
  printf("bcTest: uMG.multigridLevel[0].numberOfGrids = %i \n",uMG.multigridLevel[0].numberOfGrids());

  

  for( int level=0; level<mgcg.numberOfMultigridLevels(); level++ )
  {
    cout << "====================Level = " << level << "======================\n";
    realCompositeGridFunction & u = u0.multigridLevel[level];
    realCompositeGridFunction & f = f0.multigridLevel[level];
    CompositeGrid & cg = mgcg.multigridLevel[level];
    
    u=2.; 
    f=1.;  
    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.dimension(),I1,I2,I3);
      where( mg.mask()(I1,I2,I3)==0 )
      {
	u[grid](I1,I2,I3)=789.;  // put bogus values at unused points
      }

      // fill in the BC's into f
      int extra=1;
      for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
	  if( boundaryCondition(side,axis,grid)==OgmgParameters::equation )  // this is a neumann like BC
	  {
	    getGhostIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3,1,extra);
            u[grid](I1,I2,I3)=123.;  
	    f[grid](I1,I2,I3)=0.;   
	  }
	  else 
	  { // extrapolation
	    getGhostIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3,1,extra);
            u[grid](I1,I2,I3)=123.;  
	    f[grid](I1,I2,I3)=0.;   
	  }
	}
      }
    }
    
    u.display("bcTest: u before applyBoundaryConditions","%9.2e");
    applyBoundaryConditions( level,u,f );
    u.display("bcTest: u after applyBoundaryConditions","%9.2e");
    
  }
  return 0;
}

