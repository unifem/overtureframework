// ========================================================================
// Test the AMR InterpolateRefinement functions and Interpolate functions
//
//  ir -noplot ir1.cmd
// 
//  mpirun -np 2 ir -noplot ir1.cmd -nd=2 -ratio=2
//  srun -N1 -n1 -ppdebug ir -noplot ir1.cmd -nd=2 -ratio=2
//  srun -N1 -n1 -ppdebug ir -noplot ir1.cmd -nd=2 -ratio=4
// =======================================================================
#include "Overture.h"  
#include "PlotStuff.h"
#include "AnnulusMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "LineMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "Regrid.h"
#include "ErrorEstimator.h"
#include "interpPoints.h"
#include "InterpolateRefinements.h"
#include "OGPolyFunction.h"
#include "Interpolate.h"
#include "util.h"
#include "ParentChildSiblingInfo.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

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

void
getExactSolution( OGFunction & e, MappedGrid & mg, realArray & u, Index Iv[3], Index C, real t )
// ====================================================================================================
// 
// /Note:
//  In parallel this function will assign parallel ghost boundaries
// ====================================================================================================
{
  mg.update(MappedGrid::THEcenter);
  const realArray & center = mg.center();
  
  #ifdef USE_PPP
  // const realSerialArray & uLocal  =  u.getLocalArrayWithGhostBoundaries();
  // const realSerialArray & xLocal  =  center.getLocalArrayWithGhostBoundaries();
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(center,xLocal);
    
    Index J1 = Range(max(Iv[0].getBase(),uLocal.getBase(0)),min(Iv[0].getBound(),uLocal.getBound(0)));
    Index J2 = Range(max(Iv[1].getBase(),uLocal.getBase(1)),min(Iv[1].getBound(),uLocal.getBound(1)));
    Index J3 = Range(max(Iv[2].getBase(),uLocal.getBase(2)),min(Iv[2].getBound(),uLocal.getBound(2)));
  #else
    const realSerialArray & uLocal  =  u;
    const realSerialArray & xLocal  =  center;
    Index &J1 = Iv[0], &J2=Iv[1], &J3=Iv[2];
  #endif

  const bool isRectangular=false;  // do this for now
    
//  virtual realSerialArray& gd( realSerialArray & result,   // put result here
//  			     const realSerialArray & x,  // coordinates to use if isRectangular==true
//  			     const int numberOfDimensions,
//                               const bool isRectangular,
//                               const int & ntd, const int & nxd, const int & nyd, const int & nzd,
//  			     const Index & I1, const Index & I2, 
//  			     const Index & I3, const Index & N, 
//                               const real t=0., int option =0  ) = 0;

  int ntd=0,nxd=0,nyd=0,nzd=0;  // number of derivatives in each direction
  realSerialArray & unc = (realSerialArray&)uLocal;
  
  e.gd( unc,xLocal,mg.numberOfDimensions(),isRectangular,ntd,nxd,nyd,nzd,Iv[0],Iv[1],Iv[2],C,t);
    
/* ----------------------

    real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
    const int xDim0=xLocal.getRawDataSize(0);
    const int xDim1=xLocal.getRawDataSize(1);
    const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

  real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
  const int uDim0=uLocal.getRawDataSize(0);
  const int uDim1=uLocal.getRawDataSize(1);
  const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

  const int cBase=C.getBase(), cBound=C.getBound();

  int i1,i2,i3;
  if( mg.numberOfDimensions()==2 )
  {
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);
      for( int c=cBase; c<=cBound; c++ )
        U(i1,i2,i3,c) =e(x0,y0,0.,c,t);
    }
  }
  else
  {
    FOR_3D(i1,i2,i3,J1,J2,J3)
    {
      real x0 = X(i1,i2,i3,0);
      real y0 = X(i1,i2,i3,1);
      real z0 = X(i1,i2,i3,2);
      for( int c=cBase; c<=cBound; c++ )
        U(i1,i2,i3,c) =e(x0,y0,z0,c,t);
    }
  }
  
#undef U
#undef X

------------------ */

}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  Range all;

  printF(" ------------------------------------------------------------------- \n");
  printF(" Test InterpolateRefinements functions.                              \n");
  printF(" ------------------------------------------------------------------- \n");
  
  aString nameOfOGFile="square20.hdf";
  aString nameOfNew="square10.hdf";

  bool plotOption=true; // false;
  aString commandFileName="";
  aString line;
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      printF(" ir:parse input: argv[%i]=%s\n",i,argv[i]);
      
      if( len=line.matches("-noplot") )
      {
	plotOption=false;
        printF(" Setting plotOption=false\n");
      }
//       else if( len=line.matches("-grid=") )
//       {
// 	solver.nameOfGridFile=line(len,line.length()-1);
//         solver.gridType=Maxwell::compositeGrid;
//         printF(" Setting gridType=compositeGrid\n");
//       }
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("Using command file = [%s]\n",(const char*)commandFileName);
      }
      
    }
  }

  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("ir",plotOption,argc,argv);
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  // By default start saving a command file
  aString logFile="ir.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char*)logFile);


  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  FILE *pDebugFile = fopen(sPrintF("ir%i.debug",myid),"w" );

  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);
  cg0.update();

  Regrid regrid;
  InterpolateRefinements interp(cg0.numberOfDimensions());
  interp.setOrderOfInterpolation(3);

  ErrorEstimator errorEstimator(interp);
  
  int numberOfDimensions = cg0.numberOfDimensions();

  int nd=2;  // number of space dimensions for test of inter
      

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,TRUE);

  
  if( plotOption ) 
    PlotIt::plot(ps, cg0,psp );
  
  aString menu[]=
  {
    "solve",
    "check interpolate",
    "interpolate new from old",
    "test parallel interpolation",  // this option includes the next three
    "test fine from coarse",
    "test coarse from fine",
    "test interpolate refinement boundaries",
    "nd (dimensions for interp test)",
    "ratio",
    "order of interpolation",
    ">initial condition",
      "top hat",
    "<number of refinement levels",
    "minimum refinement size",
    "maximum refinement size",
    "grid efficiency",
    "refinement ratio",
    "number of buffer zones",
    "use smart bisection",
    "do not use smart bisection",
    "add new grids as refinements",
    "add new grids as base grids",
    "set zero base level",
    "set base level",
    "allow rotated grids",
    "aligned grids",
    "do not merge boxes",
    "change the plot",
    "debug",
    "exit",
    ""
  };
  aString answer;
  char buff[80];
  
  int debug=0;
  
  int numberOfRefinementLevels=2; // 3; // 3;

  real efficiency=.7; 
  int flaggedRegionGrowthSize=3;
  int minimumRefinementSize=16;
  int maximumRefinementSize=16;
  int baseLevel=-1;
 
  int degreeOfSpacePolynomial = 2; // 1; // 2;
  int degreeOfTimePolynomial = 0;
  int nComp = 1;
  OGPolyFunction exact(degreeOfSpacePolynomial,cg0.numberOfDimensions(),nComp,
		       degreeOfTimePolynomial);


  RealArray topHatCentre(3),topHatVelocity(3);
  topHatCentre(0)=.2; // .35;
  topHatCentre(1)=.2; // .35;
  topHatCentre(2)=.0;
  topHatVelocity(0)=1.;
  topHatVelocity(1)=1.;
  topHatVelocity(2)=1.;
  real topHatRadius=.15;

  int ratio=2;  // refinement ratio for testing parallel interp


  CompositeGrid cg;
  
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"choose" );
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="order of interpolation" )
    {
      int orderOfInterpolation=3;
      ps.inputString(answer,"Enter the order of interpolation");
      sScanF(answer,"%i",&orderOfInterpolation);
      printF("set orderOfInterpolation=%i\n",orderOfInterpolation);
      interp.setOrderOfInterpolation(orderOfInterpolation);
    }
    else if( answer=="number of refinement levels" )
    {
      ps.inputString(answer,"Enter the number of refinement levels");
      sScanF(answer,"%i",&numberOfRefinementLevels);
      printF("set numberOfRefinementLevels=%i\n",numberOfRefinementLevels);
      
    }
    else if( answer=="minimum refinement size" )
    {
      ps.inputString(answer,"Enter the minimum refinement size");
      sScanF(answer,"%i",&minimumRefinementSize);
      printF("set minimumRefinementSize=%i\n",minimumRefinementSize);
      
    }
    else if( answer=="maximum refinement size" )
    {
      ps.inputString(answer,"Enter the maximum refinement size");
      sScanF(answer,"%i",&maximumRefinementSize);
      printF("set maximumRefinementSize=%i\n",maximumRefinementSize);
    }
    else if( answer=="grid efficiency" )
    {
      ps.inputString(answer,"Enter the grid efficiency 0< eff < 1");
      sScanF(answer,"%e",&efficiency);
      printF("set efficiency=%e\n",efficiency);

      regrid.setEfficiency(efficiency);
      
    }
    else if( answer=="refinement ratio" )
    {
      int refinementRatio=2;
      ps.inputString(answer,"Enter the refinement ratio");
      sScanF(answer,"%i",&refinementRatio);

      printF("set refinementRatio=%i\n",refinementRatio);

      regrid.setRefinementRatio(refinementRatio);
      
    }
    else if( answer=="number of buffer zones" )
    {
      ps.inputString(answer,"Enter the flagged region growth size");
      sScanF(answer,"%i",&flaggedRegionGrowthSize);
      printF("set flaggedRegionGrowthSize=%i\n",flaggedRegionGrowthSize);
      
      regrid.setNumberOfBufferZones(flaggedRegionGrowthSize);  // expansion of tagged error points
      regrid.setWidthOfProperNesting(flaggedRegionGrowthSize); // distance between levels
    }
    else if( answer=="nd (dimensions for interp test)" )
    {
      ps.inputString(answer,"Enter nd (2 or 3)");
      sScanF(answer,"%i",&nd);
      printF("set nd=%i\n",nd);
    }
    else if( answer=="ratio" )
    {
      ps.inputString(answer,"Enter the refinement ratio (e.q. 2 or 4)");
      sScanF(answer,"%i",&ratio);
      printF("setting ratio=%i\n",ratio);
    }
    else if( answer=="use smart bisection" )
    {
      regrid.setUseSmartBisection(true);
    }
    else if( answer=="do not use smart bisection" )
    {
      regrid.setUseSmartBisection(false);
    }
    else if( answer=="add new grids as refinements" )
    {
      regrid.setGridAdditionOption(Regrid::addGridsAsRefinementGrids);
    }
    else if( answer=="add new grids as base grids" )
    {
      regrid.setGridAdditionOption(Regrid::addGridsAsBaseGrids);
    }
    else if( answer=="set base level" )
    {
      ps.inputString(answer,"Enter the base level (-1 = only build new level)");
      sScanF(answer,"%i",&baseLevel);
      printF("set baseLevel=%i\n",baseLevel);
    }
    else if( answer=="set zero base level" )
    {
      baseLevel=0;
      printF("set baseLevel=%i\n",baseLevel);
    }
    else if( answer=="allow rotated grids" )
    {
      regrid.setGridAlgorithmOption( Regrid::rotated );
    }
    else if( answer=="aligned grids" )
    {
      regrid.setGridAlgorithmOption( Regrid::aligned );
    }
    else if( answer=="do not merge boxes" )
    {
      regrid.setMergeBoxes(false);
    }
    else if( answer=="debug" )
    {
      regrid.debug=7;
      interp.debug=7;
    }
    else if( answer=="change the plot" )
    {
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,"Refined grid");
      if( cg.numberOfComponentGrids()>0 )
        PlotIt::plot(ps,cg,psp);
      else
        PlotIt::plot(ps,cg0,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="test parallel interpolation" ||
	     answer=="test fine from coarse" ||
	     answer=="test coarse from fine" ||
             answer=="test interpolate refinement boundaries" )
    {
      // ***************************************************************
      // *** test the interpolation of refinement grids in parallel ***
      // ***************************************************************


      bool testFineFromCoarse = answer=="test parallel interpolation" || answer=="test fine from coarse";
      bool testCoarseFromFine = answer=="test parallel interpolation" || answer=="test coarse from fine";
      bool testRefinementBoundaries= (answer=="test parallel interpolation" ||
                                      answer=="test interpolate refinement boundaries");
      

      InterpolateRefinements interp(nd); 
      interp.setOrderOfInterpolation(3);

      
      // We need to do this for quadratic interpolation
      MappedGrid::minimumNumberOfDistributedGhostLines=2;

      SquareMapping square(-1., 1., -1., 1.);            // Create a SquareMapping
      square.setGridDimensions(axis1,11); square.setGridDimensions(axis2,11);
      // AnnulusMapping mapping;            // Create an Annulus
      // mapping.setGridDimensions(axis1,21); mapping.setGridDimensions(axis2,11);

      BoxMapping box(-1., 1., -1., 1., -1., 1.);
      box.setGridDimensions(axis1,11); box.setGridDimensions(axis2,11); box.setGridDimensions(axis3,11);

      Mapping & mapping = nd==2 ? (Mapping&)square : (Mapping&)box;

      MappedGrid mg(mapping);      // grid for a mapping
//        int width=5;  // do this for now so we have two ghost lines
//        for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
//  	mg.setDiscretizationWidth(axis,width);
      
      mg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter);
      
      //  Create an `nd'-dimensional GridCollection with one grid.
      GridCollection gc(nd,1);
      gc[0].reference(mg);   
      gc.updateReferences();
      gc.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

      int ng=4;   // -- total number of grids

       // Add a refinement, specify position in the coarse grid index space
      IntegerArray range(2,3), factor(3);
//        range(0,0) = 2; range(1,0) = 6;
//        range(0,1) = 2; range(1,1) = 6;
      Integer level = 1;
      int grid = 0;                              // refine this base grid
      if( ng>=2 ) // add a refinement grid on level 1
      {
        //  [1,5]x[1,5]x[1,5]
        //    -> [2,10]x[2,10]x[2,10] : if ratio==2
        //    -> [4,20]x[4,20]x[4,20] : if ratio==4
	range(0,0) = 1; range(1,0) = 5;
	range(0,1) = 1; range(1,1) = 5;
	if( nd==2 )
	{
	  range(0,2) = 0; range(1,2) =  0;
	}
	else
	{
	  range(0,2) = 1; range(1,2) =  5;
	}
      
	factor = ratio;                            // refinement factor = 4
	level = 1;
	grid = 0;                              // refine this base grid
	gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1
      }
      
      if( ng>=3 ) // add a refinement grid on level 1
      {
        // This grid is adjacent to the previous
        
        //    [5,7]x[2,6][3,7]
        // 
  	range(0,0) = 5; range(1,0) = 7;
  	range(0,1) = 2; range(1,1) = 6;
	if( nd==2 )
	{
	  range(0,2) = 0; range(1,2) =  0;
	}
	else
	{
	  range(0,2) = 1; range(1,2) =  4;
	}
	
	factor = ratio;                            // refinement factor = 4
	level = 1;
	grid = 0;                              // refine this base grid
	gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 1
      }
      
      if( ng>=4 ) // add a refinement grid on level 2
      {
	// This level 2 grid must be properly nested inside the two grids at level 1

  	range(0,0) = 8*ratio/2; range(1,0) = 12*ratio/2;
  	range(0,1) = 5*ratio/2; range(1,1) = 8*ratio/2;
	if( nd==2 )
	{
	  range(0,2) = 0; range(1,2) =  0;
	}
	else
	{
          range(0,2) = 4*ratio/2; range(1,2) = 6*ratio/2;
//           if( ratio==2 )
// 	  {
// 	    range(0,0) = 10; range(1,0) = 12;
// 	    range(0,1) =  6; range(1,1) =  7;
// 	    range(0,2) =  8; range(1,2) =  9; // 20
// 	  }
// 	  else
// 	  {
// 	    range(0,0) = 22; range(1,0) = 24;
// 	    range(0,1) = 12; range(1,1) = 14;
// 	    range(0,2) = 16; range(1,2) = 18; // 20
// 	  }
	  
	}

	factor = ratio;                            // refinement factor = 4
	level = 2;
	grid = 1;                              // refine this base grid
	gc.addRefinement(range, factor, level, grid);    // add a refinement grid to level 2
      }
      
      gc.update(GridCollection::THErefinementLevel); 
      gc.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );
  
      gc.setMaskAtRefinements();
      
      if( plotOption )
      {
        ps.erase();
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
        psp.set(GI_TOP_LABEL,"Refined grid for parallel test");
        PlotIt::plot(ps,gc,psp);
      }

      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

      realGridCollectionFunction u(gc);
      u=0.;

      // Put the exact solution into u
      for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];
	getIndex(mg.dimension(),I1,I2,I3);
	// u[grid]=exact(mg,I1,I2,I3,0);
	getExactSolution(exact,mg,u[grid],Iv,0, 0. );
      }


      InterpolateParameters interpParams(gc.numberOfDimensions());
      interpParams.setInterpolateOrder(degreeOfSpacePolynomial+1);
      Interpolate interpolate(interpParams);


      MappedGrid & mgCoarse = gc[0];
      realArray & uCoarse = u[0];
      // const realSerialArray & ucLocal = uCoarse.getLocalArrayWithGhostBoundaries();
      realSerialArray ucLocal; getLocalArrayWithGhostBoundaries(uCoarse,ucLocal);
      ucLocal=0.;
      
      realMappedGridFunction uCoarseExact(gc[0]);
      getIndex(mgCoarse.dimension(),I1,I2,I3);
//       uCoarseExact=exact(mgCoarse,I1,I2,I3,0);
//       uCoarseExact.updateGhostBoundaries();  // is this needed?
      getExactSolution(exact,mgCoarse,uCoarseExact,Iv,0, 0. );


      real maxError=0.;
      
      #ifdef USE_PPP 
  	realSerialArray uCoarseLocal; getLocalArrayWithGhostBoundaries(uCoarse,uCoarseLocal);
  	realSerialArray uCoarseExactLocal; getLocalArrayWithGhostBoundaries(uCoarseExact,uCoarseExactLocal);
      #else
        realSerialArray & uCoarseLocal = uCoarse;
        realSerialArray & uCoarseExactLocal = uCoarseExact;
      #endif

      for( int gr=1; gr<gc.numberOfComponentGrids(); gr++ ) // loop over refinement grids
      {
        printF("\n --------- coarse grid=%i, fine grid=%i  (degree of poly=%i) --------------------------------------\n",
	       0,gr,degreeOfSpacePolynomial);

	getIndex(mgCoarse.dimension(),I1,I2,I3);
	// uCoarse=exact(mgCoarse,I1,I2,I3,0); // assign uCoarse to be the exact solution
        getExactSolution(exact,mgCoarse,uCoarse,Iv,0, 0. );



	MappedGrid & mgFine = gc[gr];
	realArray & uFine = u[gr];
	// const realSerialArray & ufLocal = uFine.getLocalArrayWithGhostBoundaries();
	realSerialArray ufLocal; getLocalArrayWithGhostBoundaries(uFine,ufLocal);
	ufLocal=0.;


	realMappedGridFunction uFineExact(gc[gr]);
	getIndex(mgFine.dimension(),I1,I2,I3);
// 	uFineExact=exact(mgFine,I1,I2,I3,0);
// 	uFineExact.updateGhostBoundaries();  // is this needed?
        getExactSolution(exact,mgFine,uFineExact,Iv,0, 0. );

	IntegerArray refinementRatio(3);
	refinementRatio=1;
	for( int dir=0; dir<gc.numberOfDimensions(); dir++ )
	{
	  refinementRatio(dir)=int(pow(ratio,gc.refinementLevelNumber(gr))+.5);
	}


        #ifdef USE_PPP 
  	  realSerialArray uFineLocal; getLocalArrayWithGhostBoundaries(uFine,uFineLocal);
  	  realSerialArray uFineExactLocal; getLocalArrayWithGhostBoundaries(uFineExact,uFineExactLocal);
        #else
          realSerialArray & uFineLocal = uFine;
          realSerialArray & uFineExactLocal = uFineExact;
        #endif

        if( testFineFromCoarse )
	{
          const int numWidths=2;
	  for( int n=0; n<numWidths; n++ )  // loop over interpolation widths 
	  {
	    int transferWidth = degreeOfSpacePolynomial+n;

	    uFine=-9.;
	    getIndex(mgFine.gridIndexRange(),I1,I2,I3);
      
	    int update=0;
	    interpolate.interpolateFineFromCoarse(uFine,Iv,uCoarse,refinementRatio,update,transferWidth); 

	    uFine.updateGhostBoundaries();  // This IS currently needed.  *** fix this ***

	    int includeGhost=1;  // include parallel ghost
	    bool ok = ParallelUtility::getLocalArrayBounds(uFine,uFineLocal,I1,I2,I3,includeGhost);
	    maxError=0.;
	    if( ok )
	      maxError=max(fabs( uFineLocal(I1,I2,I3)-uFineExactLocal(I1,I2,I3,0)) );
          
	    maxError=ParallelUtility::getMaxValue(maxError);

	    getIndex(mgFine.gridIndexRange(),I1,I2,I3);
	    printF("Interpolate: fine from coarse (width=%i) : grid %i from grid 0, I1=(%i,%i) I2=(%i,%i) I3=(%i,%i), "
		   " ratio=%i,%i,%i Max error = %8.2e\n",transferWidth,
		   gr,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		   refinementRatio(0),refinementRatio(1),refinementRatio(2),maxError);

	    if( false )
	    {
	    

	      // display(uFine(I1,I2,I3),"uFine after interpolateFineFromCoarse","%6.3f ");
	      // display(fabs(uFine(I1,I2,I3)-uFineExact(I1,I2,I3,0)),"error","%8.1e ");
	      // display(uFineExact(I1,I2,I3,0),"exact solution","%6.3f ");

	      realSerialArray ufl; getLocalArrayWithGhostBoundaries(uFine,ufl);
	      realSerialArray ufe; getLocalArrayWithGhostBoundaries(uFineExact,ufe);
	      realSerialArray ue(ufe);
	      ue=fabs(ufe-ufl);

	      ::display(ufl,"interpolateFineFromCoarse: uFineLocal ",pDebugFile," %4.1f");
	      ::display(ue,"interpolateFineFromCoarse: error on local array ",pDebugFile,"%8.1e");

	      Index J1=I1, J2=I2, J3=I3;
	      ParallelUtility::getLocalArrayBounds(uFine,ufl,J1,J2,J3); 

//             ::display(ufl(J1,J2,J3),"interpolateFineFromCoarse: uFineLocal ",pDebugFile," %4.1f");
//             ::display(ue(J1,J2,J3) ,"interpolateFineFromCoarse: error on local array ",pDebugFile,"%8.1e");
	      fflush(pDebugFile);
	    
	    }
	  }
	  
	}
	


	if( testCoarseFromFine )
	{
	  uFine=uFineExact;

	  getIndex(mgFine.gridIndexRange(),I1,I2,I3);
	  I1 = Range((I1.getBase()+refinementRatio(0)-1)/refinementRatio(0),I1.getBound()/refinementRatio(0));
	  I2 = Range((I2.getBase()+refinementRatio(1)-1)/refinementRatio(1),I2.getBound()/refinementRatio(1));
	  I3 = Range((I3.getBase()+refinementRatio(2)-1)/refinementRatio(2),I3.getBound()/refinementRatio(2));
          Index J1=I1, J2=I2, J3=I3;

          #ifdef USE_PPP 
  	    intSerialArray maskCoarse; getLocalArrayWithGhostBoundaries(mgCoarse.mask(),maskCoarse);
          #else
            intSerialArray & maskCoarse = mgCoarse.mask();
          #endif

          for( int n=0; n<=1; n++ )  // interpOption
	  {
	    Interpolate::InterpolateOptionEnum interpOption = n==0 ? Interpolate::injection : 
	      ( numberOfDimensions==2 ? Interpolate::fullWeighting110 : Interpolate::fullWeighting111);
	    for( int m=0; m<=1; m++ )  // mask option
	    {
	      uCoarse=-99.;
	      if( m==0 )
		interpolate.interpolateCoarseFromFine(uCoarse,Iv,uFine,refinementRatio,interpOption); 
	      else
		interpolate.interpolateCoarseFromFine(uCoarse,maskCoarse,Iv,uFine,refinementRatio,interpOption); 

	      int includeGhost=1;  // include parallel ghost
	      bool ok = ParallelUtility::getLocalArrayBounds(uCoarse,uCoarseLocal,I1,I2,I3,includeGhost);
	      maxError=0.;
	      if( ok )
		maxError=max(fabs( uCoarseLocal(I1,I2,I3)-uCoarseExactLocal(I1,I2,I3,0)) );
	      maxError=ParallelUtility::getMaxValue(maxError);

	      I1=J1; I2=J2; I3=J3;
	      printF("Interpolate: coarse from fine %s (%s): grid 0 from grid %i, I1=(%i,%i) I2=(%i,%i) I3=(%i,%i) ratio=%i, "
		     "Max error = %8.2e\n",(n==0 ? "injection " : "fullweight"),(m==0 ? "no mask" : " mask  "),
		     gr,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		     refinementRatio(0),maxError);
	    
	    }
	  }
	}
	
      }
      

      // put the exact solution in u 
      for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = gc[grid];
	getIndex(mg.dimension(),I1,I2,I3);
	// u[grid]=exact(mg,I1,I2,I3,0);

	getExactSolution(exact,mg,u[grid],Iv,0, 0. );
      }
      
 
      if( testRefinementBoundaries )
      {
	const int numGhost=1;  // *************
	for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = gc[grid];
	  const intArray & mask = mg.mask();
	
	  for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	  {
	    for( int side=0; side<=1; side++ )
	    {
	      if( mg.boundaryCondition(side,axis)==0 )
	      {
		for( int ghost=1; ghost<=numGhost; ghost++ )
		{
		  getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,ghost);
		  u[grid](I1,I2,I3)=5.;
		}
	      }
	    }
	  }
	  if( false ) display(u[grid],sPrintF("u on grid=%i BEFORE interpolateRefinementBoundaries",grid),"%5.2f ");
	}

	interp.interpolateRefinementBoundaries( u );  // check for updateGhostBoundaries in here !!

	if( false ) // this is now done in the above routine
	{
	  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	  {
	    u[grid].updateGhostBoundaries();  // could be more selective here -- do in above function
	  }
	}
      
	// compute the error
	realCompositeGridFunction v(gc);
	for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = gc[grid];

	  getIndex(mg.dimension(),I1,I2,I3);
	  // v[grid]=exact(mg,I1,I2,I3,0);
	  getExactSolution(exact,mg,v[grid],Iv,0, 0. );
	  // not needed: v[grid].updateGhostBoundaries();  
	}
      
	v-=u;  // v now holds the error
      
	real err,errMax=0.;

	int extra=1;
	int maskOption=0;  // check points where mask!=0 
	errMax=maxNorm(v,0,maskOption,extra );

	printF("\n ****Maximum error after interpolateRefinementBoundaries = %8.2e ****\n\n",errMax);

	if( false )
	{
	  for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	  {
	    display(u[grid],sPrintF("u on grid=%i after interpolateRefinementBoundaries",grid),"%5.2f ");
            real maxErr=max(fabs(v[grid]));
	    display(v[grid],
		    sPrintF("error on grid=%i after interpolateRefinementBoundaries (max=%8.2e)",grid,maxErr),"%8.1e ");
	  }
	}
      
	if( plotOption )
	{
	  ps.erase();
	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,"After interpolateRefinementBoundaries");
	  PlotIt::contour(ps,u,psp);
	}      

      } // end testInterpolateRefinementBoundaries

      if( testCoarseFromFine )
      {
	// put the exact solution in u 
	for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = gc[grid];
	  getIndex(mg.dimension(),I1,I2,I3);
	  // u[grid]=exact(mg,I1,I2,I3,0);

	  getExactSolution(exact,mg,u[grid],Iv,0, 0. );
	}

	for( int gr=1; gr<gc.numberOfComponentGrids(); gr++ )
	{
	  MappedGrid & mgFine = gc[gr];
	
	  getIndex(mgFine.gridIndexRange(),I1,I2,I3);
	  IntegerArray refinementRatio(3);
	  refinementRatio=1;
	  for( int dir=0; dir<gc.numberOfDimensions(); dir++ )
	  {
	    refinementRatio(dir)=int(pow(ratio,gc.refinementLevelNumber(gr))+.5);
	  }
	  I1 = Range(I1.getBase()/refinementRatio(0),I1.getBound()/refinementRatio(0));
	  I2 = Range(I2.getBase()/refinementRatio(1),I2.getBound()/refinementRatio(1));
	  I3 = Range(I3.getBase()/refinementRatio(2),I3.getBound()/refinementRatio(2));

	  int includeGhost=1;
          bool ok = ParallelUtility::getLocalArrayBounds(uCoarse,uCoarseLocal,I1,I2,I3,includeGhost);
	  if( ok )
	    uCoarseLocal(I1,I2,I3)=-99.;
	}
      
	interp.interpolateCoarseFromFine( u ); 

        getIndex(mg.dimension(),I1,I2,I3);
	Index J1=I1, J2=I2, J3=I3;
	
	int includeGhost=1;  // include parallel ghost
	bool ok = ParallelUtility::getLocalArrayBounds(uCoarse,uCoarseLocal,I1,I2,I3,includeGhost);
	maxError=0.;
	if( ok )
	  maxError=max(fabs( uCoarseLocal(I1,I2,I3)-uCoarseExactLocal(I1,I2,I3,0)) );
	maxError=ParallelUtility::getMaxValue(maxError);

	I1=J1; I2=J2; I3=J3;

	printF("InterpolateRefinements coarse from fine: (all grids) I1=(%i,%i) I2=(%i,%i) I3=(%i,%i)"
	       "  Max error = %8.2e\n",
	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),maxError);
      }
      
    }
    else if( answer=="interpolate new from old" )
    {
      if( nameOfNew=="" )
      {
	ps.inputString(nameOfNew,"Enter the name of the new grid");
      }
      
      CompositeGrid cgNew;
      getFromADataBase(cgNew,nameOfNew);
      
      cg0.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
      cgNew.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
      

      realCompositeGridFunction u(cg0), uNew(cgNew), error;
      CompositeGridOperators op(cg0);
      u.setOperators(op);
      Interpolant interpolant(cg0);
      
      real t=0.;
      u=exact(cg0,0,t);

      for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      {
	where( cg0[grid].mask()==0 )
	{
	  u[grid]=-999.;
	}
      }
      

      if( true )
      { // is this needed for sis with rf=4? ---> YES
	u.applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
      }
	  
      printF("interpolate new refinements from old ...\n");

      uNew=-1.;
      
      // this will interp interior points and call interpolateRefinementBoundaries
      interp.interpolateRefinements( u,uNew );
 	  
      op.updateToMatchGrid(cgNew);

      interpolant.updateToMatchGrid(cgNew);

      u.updateToMatchGrid(cgNew);
      u.dataCopy(uNew);

      u.interpolate();  // ****************************

      
      printF("After regrid: "); 
      real err = checkError(u,t,exact,"After regrid");
	      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

      error.updateToMatchGrid(cgNew);
      error=u-exact(cgNew,0,t);

      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      psp.set(GI_TOP_LABEL,sPrintF(buff,"error at t=%8.2e",t));
      PlotIt::contour(ps,error,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

    }
    else if( answer=="check interpolate" )
    {
      SquareMapping square;
      square.setGridDimensions(0,11);
      square.setGridDimensions(1,11);
      LineMapping line;
      line.setGridDimensions(0,11);
      Mapping & map = cg0.numberOfDimensions()==1 ? (Mapping &)line : (Mapping &)square;

      MappedGrid mgCoarse(map);
      mgCoarse.update();

      for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	map.setGridDimensions(axis,21);

      line.setGridDimensions(0,21);

      MappedGrid mgFine(map);
      mgFine.update();
      
//       SquareMapping square;
//       square.setGridDimensions(0,11);
//       square.setGridDimensions(1,11);
//       MappedGrid mgCoarse(square);
//       mgCoarse.update();

//       square.setGridDimensions(0,21);
//       square.setGridDimensions(1,21);
//       MappedGrid mgFine(square);
//       mgFine.update();


      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      int ratio=2;

      realMappedGridFunction uFine(mgFine), uCoarse(mgCoarse), exactFine(mgFine);
      
      realMappedGridFunction uFineExact(mgFine);
      getIndex(mgFine.dimension(),I1,I2,I3);
      uFineExact=exact(mgFine,I1,I2,I3,0);

      getIndex(mgCoarse.dimension(),I1,I2,I3);
      uCoarse=exact(mgCoarse,I1,I2,I3,0);
      uFine=-999.;
      
      I1=Range(2,5);
      I2=cg0.numberOfDimensions()==1 ? Range(0,0) : Range(3,7);
      I3=0;

      InterpolateParameters interpParams(cg0.numberOfDimensions());
      interpParams.setInterpolateOrder(degreeOfSpacePolynomial+1);

      Interpolate interpolate(interpParams);
      

      IntegerArray refinementRatio(3);
      refinementRatio=ratio;
      // *wdh* interpolate.interpolateCoarseToFine (uFine,Iv,uCoarse,refinementRatio);
      interpolate.interpolateFineFromCoarse(uFine,Iv,uCoarse,refinementRatio); 

//      interp.interpolate(uFine,Iv,uCoarse,ratio);

      display(uFine(I1,I2,I3),"uFine(I1,I2,I3)");
      display(uFineExact(I1,I2,I3,0),"exact(mgFine,I1,I2,I3,0)");
      
      real maxError=max(fabs( uFine(I1,I2,I3)-uFineExact(I1,I2,I3,0)) );
      printF("I1=(%i,%i) I2=(%i,%i), Max error = %8.2e\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
           maxError);
      
      uFine=-999.;
      I1=Range(1,1);
      I2=cg0.numberOfDimensions()==1 ? Range(0,0) : Range(4,8);
      I3=0;
      // interp.interpolate(uFine,Iv,uCoarse,ratio);
      // *wdh* interpolate.interpolateCoarseToFine (uFine,Iv,uCoarse,refinementRatio);
      interpolate.interpolateFineFromCoarse(uFine,Iv,uCoarse,refinementRatio);

      maxError=max(fabs( uFine(I1,I2,I3)-uFineExact(I1,I2,I3,0)) );
      printF("I1=(%i,%i) I2=(%i,%i), Max error = %8.2e\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
           maxError);
      
      uFine=-999.;
      I1=Range(1,1);
      I2=cg0.numberOfDimensions()==1 ? Range(0,0) : Range(2,2);
      I3=0;
      // interp.interpolate(uFine,Iv,uCoarse,ratio);
      // *wdh* interpolate.interpolateCoarseToFine (uFine,Iv,uCoarse,refinementRatio);
      interpolate.interpolateFineFromCoarse(uFine,Iv,uCoarse,refinementRatio);
      
      maxError=max(fabs( uFine(I1,I2,I3)-uFineExact(I1,I2,I3,0)) );
      printF("I1=(%i,%i) I2=(%i,%i), Max error = %8.2e\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),
           maxError);
      
      // -------------------------

      I1=Range(2,5);
      I2=cg0.numberOfDimensions()==1 ? Range(0,0) : Range(3,7);
      I3=0;

      ratio=1;
      refinementRatio=ratio;
      uFine=-99.;
      // *wdh* interpolate.interpolateCoarseToFine (uFine,Iv,uFineExact,refinementRatio);
      interpolate.interpolateFineFromCoarse(uFine,Iv,uFineExact,refinementRatio);

//      interp.interpolate(uFine,Iv,uCoarse,ratio);

      display(uFine(I1,I2,I3),"uFine(I1,I2,I3)");
      display(uFineExact(I1,I2,I3,0),"exact(mgFine,I1,I2,I3,0)");
      
      maxError=max(fabs( uFine(I1,I2,I3)-uFineExact(I1,I2,I3,0)) );
      printF("ratio==1 : I1=(%i,%i) I2=(%i,%i), Max error = %8.2e\n",I1.getBase(),I1.getBound(),
              I2.getBase(),I2.getBound(),maxError);



    }
    else if( answer=="solve" )
    {
      cg.destroy();
      cg=cg0;
      cg.update(MappedGrid::THEvertex | MappedGrid::THEmask );

      display(cg.numberOfInterpolationPoints,"cg.numberOfInterpolationPoints");
      
      Index I1,I2,I3;                                            
      realCompositeGridFunction u(cg), error;
      CompositeGridOperators op;
      Interpolant interpolant; 
      
      int baseLevel=0; // always regenerate to this level

      real errorThreshhold=.1;

      getTrueSolution(u,0.,topHatCentre,topHatVelocity,topHatRadius);

      for( int level=1; level<numberOfRefinementLevels; level++ )
      {
      

        error.updateToMatchGrid(cg);
        interpolant.updateToMatchGrid(cg);
	errorEstimator.computeErrorFunction(u,error );
        printF("errorEstimator error function: min=%e, max=%e \n",min(error),max(error));

	if( debug & 2 )
	{
  	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  psp.set(GI_TOP_LABEL,"error function");
	  ps.erase();
	  PlotIt::contour(ps,error,psp);
	}
	

        CompositeGrid cgNew;
	regrid.regrid(cg, cgNew, error, errorThreshhold, level, baseLevel);
	cg=cgNew;

        cg.update(MappedGrid::THEvertex | MappedGrid::THEmask );

	cg.setMaskAtRefinements();
	
        if( debug & 2 )
	{
	  psp.set(GI_TOP_LABEL,"Refined grid");
	  PlotIt::plot(ps,cg,psp);
	}
	
        u.updateToMatchGrid(cg);
        getTrueSolution(u,0.,topHatCentre,topHatVelocity,topHatRadius);
        if( debug & 2 )
	{
	  psp.set(GI_TOP_LABEL,"u");
	  PlotIt::contour(ps,u,psp);
	}
      }
      
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      if( debug & 2 )
      {
	psp.set(GI_TOP_LABEL,"Refined grid");
	PlotIt::plot(ps,cg,psp);
      }

//       psp.set(GI_TOP_LABEL,"initial conditions on refined grid");
//       PlotIt::contour(ps,u,psp);
//       psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);


      cg.update(MappedGrid::THEcenter);
      
      u=exact(cg,Range(0,0));
      
      int grid;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
        const intArray & mask = mg.mask();
	
        for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
            if( mg.boundaryCondition(side,axis)==0 )
	    {
	      for( int ghost=1; ghost<=2; ghost++ )
	      {
		getGhostIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,ghost);
		u[grid](I1,I2,I3)=5.;
	      }
	    }
	  }
	}
	
      }
      
      // By default ghost line are plotted at interpolation boundaries.
      // psp.set(GI_NUMBER_OF_GHOST_LINES_TO_PLOT,1);

      
      if( false )
      {
	psp.set(GI_TOP_LABEL,"before interpolateRefinementBoundaries");
	ps.erase();
	PlotIt::contour(ps,u,psp);
      }
      
      if( true )
      {
	ListOfParentChildSiblingInfo listOfPCSInfo;
	ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( cg, listOfPCSInfo );
	if( true )
	{
	  for( int grid=0; grid<listOfPCSInfo.getLength(); grid++ )
	  {
	    cout << "\n *****grid " << grid << ": PCSInfo:\n";
	    cout << listOfPCSInfo[grid];
	  }
	}

        interp.interpolateRefinementBoundaries( listOfPCSInfo,u );

        interp.interpolateCoarseFromFine( listOfPCSInfo,u );

      }
      else
      {
        interp.interpolateRefinementBoundaries( u );
        interp.interpolateCoarseFromFine( u ); 
      }
      
      
      // compute the error
      realCompositeGridFunction v(cg);
      v=exact(cg,Range(0,0));
      real err,errMax=0.;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
        const intArray & mask = mg.mask();
        int extra=1;
        getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
        where( mask(I1,I2,I3)!=0 )
	{
	  err=max(fabs(u[grid](I1,I2,I3)-v[grid](I1,I2,I3)));
	}
	errMax=max(errMax,err);
      }
      printF("\n ****Maximum error after interpolate = %8.2e ****\n\n",errMax);
      

      psp.set(GI_TOP_LABEL,"after interpolateRefinementBoundaries");
      ps.erase();
      PlotIt::contour(ps,u,psp);


      regrid.printStatistics(cg);

    }
    else
    {
      printF("unknown response\n");
    }
  }
  
  fclose(pDebugFile);

  Overture::finish();          
  return 0;
}


