// ======================================================================================
//
//  Test Evolution algorithms for Grid Motion 
//
//   evolve -movieMode=0
//
// =======================================================================================

#include "Ogen.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"
#include "HyperbolicMapping.h"
#include "Ogshow.h"
#include "GridSmoother.h"
#include "MappingInformation.h"

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

      
// ======================================================================================
/// \brief smooth the grid. 
// ======================================================================================
int
smoothGrid( GridSmoother & gridSmoother,
            Mapping *surface, DataPointMapping *dpm, 
            GenericGraphicsInterface & gi, 
	    GraphicsParameters & parameters )
{
  
  int projectGhost[2][3];
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<3; axis++ )
    {
      projectGhost[side][axis]=false; // boundaryOffset[side][axis]>0;
    }
  }
  gridSmoother.smooth(*surface,*dpm,gi,parameters,projectGhost );

  return 0;
  

// --- This next is from hype/update.C : we should add a smooth function to the HyperbolicGridGenerator --

//       if( answer.matches("GSM:smooth grid") ||
//           answer.matches("smooth grid") )
//       {
// 	assert( surface!=NULL );
// 	assert( dpm!=NULL );
      
//         int projectGhost[2][3];
// 	for( int side=0; side<=1; side++ )
// 	{
// 	  for( int axis=0; axis<3; axis++ )
// 	  {
// 	    projectGhost[side][axis]=boundaryOffset[side][axis]>0;
// 	  }
// 	}

//         // **** supply Mappings to the GridSmoother for projecting boundaries ****
//         Mapping *boundaryMappings[2][3]={NULL,NULL,NULL,NULL,NULL,NULL}; //
//         if( domainDimension==rangeDimension )
// 	{
// 	  boundaryMappings[0][domainDimension-1]=surface;  // curve/surface we start from
//           for( int axis=0; axis<domainDimension-1; axis++ )
// 	  {
// 	    for( int side=0; side<=1; side++ )
// 	    {
// 	      boundaryMappings[side][axis]=boundaryConditionMapping[side][axis];  // for match to mapping BC
// 	    }
// 	  }
// 	}
//         else 
// 	{
//           assert( domainDimension==2 && rangeDimension==3 );
// 	  boundaryMappings[0][1]=startCurve;    // curve we start from

//           // growthOption: 1=forward, -1=backward +-2=both
//           bool growBothDirections = fabs(growthOption) > 1;
// 	  int direction = (growthOption==1 || growBothDirections) ? 0 : 1;
//           for( int side=0; side<=1; side++ )
// 	  {
//             // we have a problem if we are going in both directions but match to different curves
//             //  -- this case is not yet supported by the GridSmoother. If we leave NULL then the
//             // GridSmoother will just project onto the boundary defined by the dpm
//             if( !growBothDirections || 
//                 (boundaryConditionMapping[0][direction]==boundaryConditionMapping[1][direction]) )
// 	    {
// 	      boundaryMappings[side][0]=boundaryConditionMapping[side][direction];
// 	    }
// 	  }
// 	}
	
// 	gridSmoother.setBoundaryMappings( boundaryMappings );
//         gridSmoother.setMatchingCurves( matchingCurves );
	
// 	gridSmoother.smooth(*surface,*dpm,gi,parameters,projectGhost );

// 	setBasicInverseOption(dpm->getBasicInverseOption());
// 	reinitialize();  // *wdh* 000503
      
//         const realArray & xdpm = dpm->getDataPoints();
//         realArray & x = xHyper;
	

//         Index I1,I2,I3;
// 	I1=x.dimension(0);
// 	I2=x.dimension(1);
// 	I3=x.dimension(2);
//         Range xAxes=rangeDimension;
//         // xdpm may be smaller along I1 in the periodic case
//         Index J1 =Range( max(xdpm.getBase(0),I1.getBase()),min(xdpm.getBound(0),I1.getBound()));

// 	if( I3.getBase()< xdpm.getBase(domainDimension-1) || I3.getBound()>xdpm.getBound(domainDimension-1) )
// 	{
// 	  printf("After smooth: dpm bounds [%i,%i][%i,%i][%i,%i], x bounds: [%i,%i][%i,%i][%i,%i]"
//                  " gridIndexRange=[%i,%i][%i,%i][%i,%i] \n",
// 		 xdpm.getBase(0),xdpm.getBound(0),xdpm.getBase(1),xdpm.getBound(1),xdpm.getBase(2),xdpm.getBound(2),
// 		 x.getBase(0),x.getBound(0),x.getBase(1),x.getBound(1),x.getBase(2),x.getBound(2),
//                  gridIndexRange(0,0),gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),
//                  gridIndexRange(0,2),gridIndexRange(1,2));

// 	  printf("ERROR: after smoothing: The smoothed grid is smaller than the hyperbolic grid\n"
// 		 "    It could be that the grid generator ended in an error with a negative cell\n");
// 	  gi.stopReadingCommandFile();
// 	}
// 	else
// 	{
// 	  if( domainDimension==2 )
// 	  {
// 	    x.reshape(I1,I3,1,x.dimension(3));
// 	    x(J1,I3,0,xAxes)=xdpm(J1,I3,0,xAxes);
// 	    x.reshape(I1,I2,I3,x.dimension(3));
// 	  }
// 	  else	  
// 	    x(J1,I2,I3,xAxes)=xdpm(J1,I2,I3,xAxes);
// 	}
	

// 	mappingHasChanged();
// 	plotObject=true; 
// 	plotHyperbolicSurface=true;

//       }
//     }
  
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0; 

  int numberOfSteps=41; // 80;

  aString nameOfOGFile="cice2.order2.hdf"; // use this grid to start


  enum MoveOptionEnum
  {
    hype,           // grid is define by hyperbolic grid generator (no relaxation)
    relaxInTime,
    leastSquares    // fit a least square fit to the grid motion
  } moveOption = relaxInTime;


  int plotOption=true;
  int plotGrid=false;
  int movieMode=1;
//  real ds=.05;          // factor=2
  real amp=.75;         // amplitude of the sinusoidal motion 
  real cfl=.5;
  int debug=1;  
  real perturb=.0; // amplitude of a perturbation to the boundary 
  real pFreq=9; // frequency of the perturbation
   
  real shiftx=1., shifty=0.;   // shift for 'final' position of the curve 

  bool smooth=false;
   
  printF("Usage: evolve -moveOption=[hype|relax|ls] -g=<grid> -numSteps=<> -debug=<> -amp=<> -cfl=<> [-plotGrid] \n"
         "-movieMode=[0|1] -perturb=<value> -pFreq=<> shift=<val> -smooth=[0|1]\n");
  if( argc > 1 )
  { // look at arguments for "noplot"
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( line=="-plotGrid" )
        plotGrid=true;
      else if( len=line.matches("-g=") )
      {
        nameOfOGFile=line(len,line.length()-1);
	printF("Will use grid=[%s]\n",(const char*)nameOfOGFile);
      }
      else if( len=line.matches("-movieMode=") )
      {
        sScanF(line(len,line.length()-1),"%i",&movieMode);
	printF("Setting movieMode=%i\n",movieMode);
      }
      else if( len=line.matches("-numSteps=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfSteps);
	printF("Setting numberOfSteps=%i\n",numberOfSteps);
      }
      else if( len=line.matches("-amp=") )
      {
        sScanF(line(len,line.length()-1),"%e",&amp);
	printF("Setting amp=%e\n",amp);
      }
      else if( len=line.matches("-cfl=") )
      {
        sScanF(line(len,line.length()-1),"%e",&cfl);
	printF("Setting cfl=%e\n",cfl);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("Setting debug=%i\n",debug);
      }
//       else if( len=line.matches("-ds=") )
//       {
//         sScanF(line(len,line.length()-1),"%e",&ds);
// 	printF("Setting ds=%e\n",ds);
//       }
      else if( len=line.matches("-perturb=") )
      {
        sScanF(line(len,line.length()-1),"%e",&perturb);
	printF("Setting the amplitude of the perturbation to perturb=%e\n",perturb);
      }
      else if( len=line.matches("-pFreq=") )
      {
        sScanF(line(len,line.length()-1),"%e",&pFreq);
	printF("Setting the frequency of the perturbation to pFreq=%e\n",pFreq);
      }
      else if( len=line.matches("-shiftx=") )
      {
        sScanF(line(len,line.length()-1),"%e",&shiftx);
	printF("Setting the shift (for the final position of the boundary) to shiftx=%e\n",shiftx);
      }
      else if( len=line.matches("-shifty=") )
      {
        sScanF(line(len,line.length()-1),"%e",&shifty);
	printF("Setting the shift (for the final position of the boundary) to shifty=%e\n",shifty);
      }
      else if( len=line.matches("-moveOption=hype") )
      {
        moveOption=hype;
	printF("Setting the move option to hype.\n");
      }
      else if( len=line.matches("-moveOption=relax") )
      {
        moveOption=relaxInTime;
	printF("Setting the move option to relaxInTime.\n");
      }
      else if( len=line.matches("-moveOption=ls") )
      {
        moveOption=leastSquares;
	printF("Setting the move option to leastSquares.\n");
      }
      else if( len=line.matches("-smooth=") )
      {
        sScanF(line(len,line.length()-1),"%i",&smooth);
	printF("Setting smooth=%i\n",smooth);
      }
      else
      {
	printF("ERROR: unknown command line option: [%s]\n",(const char*)line);
      }
      
    }
  }

  // Create two CompositeGrid objects, cg[0] and cg[1]
  const int numberOfLevels=5;
  CompositeGrid cg[numberOfLevels];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file
  for( int i=1; i<numberOfLevels; i++ )
    cg[i]=cg[0];                                      // copy cg[0] into cg[1]

  const int numberOfDimensions = cg[0].numberOfDimensions();

  // Deform component grid 2 (do this by changing the mapping)
  int gridToDeform=1;

  // The target grid will be a Hyperbolic Mapping
  HyperbolicMapping targetGrid;

  // The grid we deform will be saved as a DPM:
  DataPointMapping transform[numberOfLevels];
  
  // The grid history will be saved here:
  RealArray gridHistory[numberOfLevels];


  NurbsMapping *startCurve = new NurbsMapping [numberOfLevels]; // start curve representing the ice surface
  

  // -- here we assume grid=1 is the annular grid 
  const IntegerArray & gida = cg[0][1].gridIndexRange();
  const int numPoints = gida(1,0)-gida(0,0)+1;

  
  // The inner radius of the annulus is assumed to be: 
  real rad=.5, theta; 
  real ds = twoPi*rad/(numPoints-1);


  real nDist=.75;
  int nr = int( nDist/ds + 1.5 );

  printF(" numPoints=%i, ds=%8.2e, nr=%i\n",numPoints,ds,nr);

  //  const int numPoints=int( twoPi*(rad+nDist)*.5/ds + 1.5 );
  realArray x0(numPoints,2), x1(numPoints,2), x2(numPoints,2);

  // Create two curves that define the initial and final locations of the deforming surface
  //   x0 : initial location of the curve
  //   x1 : 'final' location of the curve 
  for( int i=0; i<numPoints; i++ )
  {
    real theta = i*twoPi/(numPoints-1.);
    x0(i,0)=rad*cos(theta);
    x0(i,1)=rad*sin(theta);

    // curve 1 is offset as a shift
    x1(i,0)=rad*cos(theta) + shiftx ;
    // x1(i,0)=rad*cos(theta);
    x1(i,1)=rad*sin(theta) + shifty;
  }

  for( int i=0; i<numberOfLevels; i++ )
  {
    startCurve[i].setIsPeriodic(0,Mapping::functionPeriodic);
    startCurve[i].interpolate(x0);
  }
  

  const bool isSurfaceGrid=false, init=true;
  targetGrid.setSurface(startCurve[0],isSurfaceGrid,init);
  targetGrid.setGridDimensions(axis1,numPoints);
  targetGrid.setParameters(HyperbolicMapping::distanceToMarch,nDist);
  targetGrid.setParameters(HyperbolicMapping::linesInTheNormalDirection,nr-1);
  targetGrid.setShare(0,1,1);
  targetGrid.generate();
  
  DataPointMapping & dpm = *((DataPointMapping*)targetGrid.getDataPointMapping());
  const IntegerArray & gid = dpm.getGridIndexRange();
  const IntegerArray & dim = dpm.getDimension();
  const realArray & xy = dpm.getDataPoints();
  printF("Target grid: xy = [%i,%i][%i,%i][%i,%i][%i,%i]\n",xy.getBase(0),xy.getBound(0),
	 xy.getBase(1),xy.getBound(1),xy.getBase(2),xy.getBound(2),xy.getBase(3),xy.getBound(3));
  printF("            dim = [%i,%i][%i,%i][%i,%i]\n",dim(0,0),dim(1,0),dim(0,1),dim(1,1),dim(0,2),dim(1,2));
  printF("            gid = [%i,%i][%i,%i][%i,%i]\n",gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
    
  int numberOfGhostLinesInData[2][3];
  for( int axis=0; axis<3; axis++)for( int side=0; side<=1; side++ )
  {
    numberOfGhostLinesInData[side][axis]=abs(gid(side,axis)-dim(side,axis));
  }
    
  int positionOfCoordinates=3;
  int domainDimension=numberOfDimensions;
  for( int i=0; i<numberOfLevels; i++ )
  {
    for( int axis=0; axis<numberOfDimensions; axis++)
    {
      transform[i].setIsPeriodic(axis,targetGrid.getIsPeriodic(axis));
      for( int side=0; side<=1; side++ )
      {
	transform[i].setBoundaryCondition(side,axis,targetGrid.getBoundaryCondition(side,axis));
	transform[i].setShare(side,axis,targetGrid.getShare(side,axis));
      }
    }
    transform[i].setDataPoints(xy,positionOfCoordinates,domainDimension,numberOfGhostLinesInData,gid);
  }
  
  // Initialize the grid history
  for( int i=0; i<numberOfLevels; i++ )
    gridHistory[i].reference(xy);
  

  // Replace the mapping of the component grid that we want to move:
  for( int i=0; i<numberOfLevels; i++ )
  {
    cg[i][gridToDeform].reference(transform[i]); 
    cg[i].update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter );
  }
  


  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do but it will save space
  // for( int i=1; i<numberOfLevels; i++ )
//     cg[i].destroy(CompositeGrid::EVERYTHING);  

  // we tell the grid generator which grids have changed
  LogicalArray hasMoved(cg[0].numberOfGrids());
  hasMoved    = LogicalFalse;
  hasMoved(gridToDeform) = LogicalTrue;  // Only this grid will change
  char buff[80];

  PlotStuff ps(plotOption,"Deforming Grid Example");         // for plotting
  PlotStuffParameters psp;

  psp.set(GI_CONTOUR_SURFACE_VERTICAL_SCALE_FACTOR,0.);
  psp.set(GI_PLOT_WIRE_FRAME,true);


  // Here is the overlapping grid generator
  Ogen gridGenerator(ps);

  // update the initial grid, since the above reference destroys the mask
  gridGenerator.updateOverlap(cg[0]);

  // Here is an interpolant
  Interpolant & interpolant = *new Interpolant(cg[0]); interpolant.incrementReferenceCount();
  realCompositeGridFunction u(cg[0]);
  // Here is a show file
  Ogshow show("deform.show");
  show.setIsMovingGridProblem(true);

  Range Rx=numberOfDimensions;
  Range all;
  realCompositeGridFunction gva(cg[0],all,all,all,2*numberOfDimensions);  // holds grid velocity and acceleration
  gva=0.;
  for(int c=0; c<numberOfDimensions; c++ )
  {
    gva.setName(sPrintF("v%i",c),c);
    gva.setName(sPrintF("a%i",c),c+numberOfDimensions);
  }
  
  // --- Grid Smoother --
  int rangeDimension = domainDimension;

  GridSmoother gridSmoother(domainDimension,rangeDimension);
  IntegerArray bc(2,3);
  bc = (int) GridSmoother::pointsFixed; // pointsSlide;
  gridSmoother.setBoundaryConditions( bc );

  if( smooth )
  {

    GUIState gui;
    gui.setWindowTitle("evolve");
    gui.setExitCommand("exit", "continue");

    // --- Build the sibling dialog for smooth dialog ---
    DialogData & smoothDialog = gui.getDialogSibling();
    smoothDialog.setWindowTitle("Smoothing");
    smoothDialog.setExitCommand("close smoothing options", "close");

    gridSmoother.buildDialog(smoothDialog);

    ps.pushGUI(gui);

    smoothDialog.showSibling();

    MappingInformation mapInfo;
    mapInfo.graphXInterface = &ps;
    
    // Query for changes to the GridSmoother Options 
    aString answer;
    for( ;; )
    {
      ps.getAnswer(answer,"");      
      if( answer=="close smoothing options" || answer=="exit" || answer=="continue" )
      {
	break;
      }
      else if( gridSmoother.updateOptions( answer,smoothDialog,mapInfo ) )
      {
	printF("Answer found in gridSmoother.updateOptions. answer=[%s]\n",(const char*)answer);
      }
    }
    
    smoothDialog.hideSibling();
    ps.popGUI();


  }



  real dt = cfl*ds;
  real t=0.;
  
  // -----------------------------------------------
  // ---- Deform the grid in a periodic fashion.----
  // -----------------------------------------------

  for (int step=1; step<=numberOfSteps; step++) 
  {
    t=step*dt;

    int newCG = step % numberOfLevels;        // new grid
    int oldCG = (step-1+numberOfLevels) % numberOfLevels;    // old grid
    

    int n0 = (step-1+numberOfLevels) % numberOfLevels;  // time t 
    int n1 = (step-2+numberOfLevels) % numberOfLevels;  // time t-dt
    int n2 = (step-3+numberOfLevels) % numberOfLevels;  // time t-2*dt 
    int n3 = (step-4+numberOfLevels) % numberOfLevels;  // time t-3*dt 
    int n4 = (step-5+numberOfLevels) % numberOfLevels;  // time t-4*dt 

    // Grid velocity and acceleration:
    gva.updateToMatchGrid(cg[oldCG]);
    gva=0.;
      
    if( true || step>4 )
    {
      // compute the grid velocity and acceleration

      // cg[n0].update(MappedGrid::THEcenter | MappedGrid::THEvertex);
      // cg[n1].update(MappedGrid::THEcenter | MappedGrid::THEvertex);
      // cg[n2].update(MappedGrid::THEcenter | MappedGrid::THEvertex);

  
      Index I1,I2,I3;
      for( int grid=0; grid<cg[0].numberOfComponentGrids(); grid++ )
      {
	realArray & gv = gva[grid];
	
	getIndex(cg[n0][grid].dimension(),I1,I2,I3);
	const realArray & vertex0 = cg[n0][grid].vertex();
	const realArray & vertex1 = cg[n1][grid].vertex();
	const realArray & vertex2 = cg[n2][grid].vertex();
	
	
	gv(I1,I2,I3,Rx) = (vertex0(I1,I2,I3,Rx) - vertex1(I1,I2,I3,Rx))*(1./dt);
	gv(I1,I2,I3,Rx+numberOfDimensions) = (vertex0(I1,I2,I3,Rx) -2.*vertex1(I1,I2,I3,Rx)+ 
                                              vertex2(I1,I2,I3,Rx))*(1./(dt*dt));
      }
    }
    

    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step=%i",step));  // set title
    if( plotGrid )
    {
      PlotIt::plot(ps,cg[oldCG],psp);      // plot the current overlapping grid
    }
    else
    {
      PlotIt::contour(ps,gva,psp);         // plot the current solution
    }
    if( movieMode )
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);     // set this to run in "movie" mode (after first plot)
    ps.redraw(TRUE);


    // real deltaOmega=1./20.;
    real deltaOmega=cfl/10.;
    // real omega=step*deltaOmega;
    real omega=amp*pow(sin(Pi*step*deltaOmega),2.);  // omega varies in the interval [0,1]
    
    // The current boundary lies between x0 and x1 
    x2 = (1.-omega)*x0 + omega*x1;
    // add a high frequency perturbation to the deforming surface
    if( perturb>0. )
    {
      for( int j=0; j<numPoints; j++ )
      {
	// real freq = .25*numPoints;
	real theta = pFreq*((j+step)*twoPi/(numPoints-1.));  // perturbation 'rotates' around the curve (step)
	real delta=rad*perturb;
	x2(j,0)+=delta*cos(theta);
	x2(j,1)+=delta*sin(theta);
      }
    }
    
    startCurve[newCG].interpolate(x2);   // form the new start curve (NurbsMapping)
    
    const bool isSurfaceGrid=false;
    const bool init=false; // this means keep existing hype parameters such as distanceToMarch, linesToMarch etc. 

//     transform[newCG].setSurface(startCurve[newCG],isSurfaceGrid,init);  // supply a new start curve

//     // ** generate the new grid with the hyperbolic grid generator **
//     transform[newCG].generate();   


    targetGrid.setSurface(startCurve[newCG],isSurfaceGrid,init);

    // -- generate the hyperbolic grid ---
    targetGrid.generate();  
  
    DataPointMapping & dpm = *((DataPointMapping*)targetGrid.getDataPointMapping());

    // -- optionally smooth the grid --
    if( smooth )
    {
      smoothGrid( gridSmoother, &startCurve[newCG], &dpm, ps, psp );
    }
    

    const IntegerArray & gid = dpm.getGridIndexRange();
    const IntegerArray & dim = dpm.getDimension();
    const realArray & xy = dpm.getDataPoints();
    if( debug & 2 )
    {
      printF("Target grid: xy = [%i,%i][%i,%i][%i,%i][%i,%i]\n",xy.getBase(0),xy.getBound(0),
	     xy.getBase(1),xy.getBound(1),xy.getBase(2),xy.getBound(2),xy.getBase(3),xy.getBound(3));
      printF("            dim = [%i,%i][%i,%i][%i,%i]\n",dim(0,0),dim(1,0),dim(0,1),dim(1,1),dim(0,2),dim(1,2));
      printF("            gid = [%i,%i][%i,%i][%i,%i]\n",gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
    }
    
    int numberOfGhostLinesInData[2][3];
    for( int axis=0; axis<3; axis++)for( int side=0; side<=1; side++ )
    {
      numberOfGhostLinesInData[side][axis]=abs(gid(side,axis)-dim(side,axis));
    }
    
    int positionOfCoordinates=3;
    int domainDimension=numberOfDimensions;
    

    // gridHistory[n0].reference(xy);  // save current target grid 



    // --- Evolve the grid ----

    // The target grid is defined by the hyperbolic grid.
    // The actual grid solves the evolution equation:
    // 
    //     x' = ( gTarget - x )/geps 

    realArray & x0 = gridHistory[n0];   // t
    realArray & x1 = gridHistory[n1];   // t-dt
    realArray & x2 = gridHistory[n2];   // t-2*dt
    realArray & x3 = gridHistory[n3];   // t-3*dt
    realArray & x4 = gridHistory[n4];   // t-4*dt

    x0.redim(xy.dimension(0),xy.dimension(1),xy.dimension(2),xy.dimension(3));
    x0=xy;
    Index I1,I2,I3;
    if( moveOption==relaxInTime )
    {
      printF(" Relax grid in time, step=%i...\n",step);
      
      getIndex(dim,I1,I2,I3);
      I2=Range(gid(0,1),I2.getBound());  // we only change points with i2 > gid(0,1)
      const int i2a=I2.getBase(), i2b=I2.getBound();
      real delta = 1./(i2b-i2a);
      real omegag=.9;
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	for( int axis=0; axis<numberOfDimensions; axis++)
	{
	  // Grid near i2=i2a must match x0, otherwise average with older grid
	  real omega=omegag*(i2-i2a)*delta;
	  x0(i1,i2,i3,axis) = (1.-omega)*xy(i1,i2,i3,axis) + omega*(2.*x1(i1,i2,i3,axis)-x2(i1,i2,i3,axis));
	}
      }
    }
    
    if( step>1 && moveOption==leastSquares )
    {

      printF(" Least squares relaxation, step=%i, dt=%9.3e...\n",step,dt);
  
      
      // perform a least squares fit the grid point motion
      //    x(t) = a*t + b 

      int numberOfLevels = min(step,5); // numberOfLevels; 
      real xv[10], tv[10];

      tv[0]=t;	   
      tv[1]=t-dt;   
      tv[2]=t-2.*dt;
      tv[3]=t-3.*dt;
      tv[4]=t-4.*dt;

      getIndex(dim,I1,I2,I3);
      
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	for( int axis=0; axis<rangeDimension; axis++ )
	{
	  xv[0]= xy(i1,i2,i3,axis);
	  xv[1]= x1(i1,i2,i3,axis);
	  xv[2]= x2(i1,i2,i3,axis);  
	  xv[3]= x3(i1,i2,i3,axis);
	  xv[4]= x4(i1,i2,i3,axis);
	
	  real xBar=0., tBar=0., tSqBar=0., xtBar=0.;
	  for( int n=0; n<numberOfLevels; n++ )
	  {
	    xBar+= xv[n];
	    tBar+= tv[n];
	    tSqBar+= tv[n]*tv[n];
	    xtBar+= xv[n]*tv[n];
	  }
	  xBar/=numberOfLevels; tBar/=numberOfLevels; tSqBar/=numberOfLevels; xtBar/=numberOfLevels;
	
	  // x = a*t + b 
	  real a = (xtBar - xBar*tBar)/( tSqBar-tBar*tBar);
	  real b = xBar - a*tBar;

          // printF(" (i1,i2)=(%i,%i) axis=%i x0=%8.2e, x1=%8.2e, LS=%8.2e\n",i1,i2,axis,xv[0],xv[1],a*tv[0]+b);
	  

          x0(i1,i2,i3,axis)= a*tv[0]+b;  // grid point = least-squares curve at t=tv[0]

	} // end for axis
      }
      
    } // end leastSquares
    

    // Set the new grid : 
    transform[newCG].setDataPoints(x0,positionOfCoordinates,domainDimension,numberOfGhostLinesInData,gid);

    // Update the overlapping grid newCG, starting with and sharing data with oldCG.
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    int useFullAlgorithmInterval=1; // 10;
    if( step% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      cout << "\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n";
      option=Ogen::useFullAlgorithm;
    }
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option );

    cg[newCG].update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex);

    interpolant.updateToMatchGrid(cg[newCG]);
    u.updateToMatchGrid(cg[newCG]);
    // assign values to u
    for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      getIndex(mg.dimension(),I1,I2,I3);
      real freq = 2.*step/numberOfSteps;
      u[grid](I1,I2,I3)=cos(freq*mg.vertex()(I1,I2,I3,axis1))*sin(freq*mg.vertex()(I1,I2,I3,axis2));  
    }
    u.interpolate();
    // save the result in a show file, every fourth step
    if( (step % 4) == 1 )
    {
      show.startFrame();
      show.saveComment(0,sPrintF(buff,"Solution at step = %i",step));
      show.saveSolution(u);
    }
  } 
  printF("Results saved in deform.show, use Overture/bin/plotStuff to view this file\n");

  printF("deform: interpolant.getReferenceCount=%i\n",interpolant.getReferenceCount());
  if( interpolant.decrementReferenceCount()==0 )
  {
    printF("deform: delete Interpolant\n");
    delete &interpolant;
  }

  Overture::finish();          
  return 0;
}

