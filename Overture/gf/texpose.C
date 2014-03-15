// ====================================================================================
//   Test  the ExposedPoints class for interpolating exposed points.
//
// examples:
//          texpose -grid=stir.hdf -debug=3
//          texpose -grid=bib2.hdf
//          texpose -grid=twoDrop.hdf
//          texpose -grid=sib.hdf
//          texpose -grid=drop3d.hdf
//          texpose -grid=cg1Exposed.hdf -grid2=cg2Exposed.hdf
// parallel: 091126 
//  mpirun -np 1 texpose -grid=stir.hdf -debug=3
//  srun -N1 -n1 -ppdebug texpose -grid=stir.hdf -debug=3
//  srun -N1 -n2 -ppdebug memcheck_all texpose -grid=stir.hdf -debug=3
//  totalview srun -a -N1 -n2 -ppdebug texpose -grid=stir.hdf -debug=3
// Trouble: 
//   totalview srun -a -N1 -n4 -ppdebug texpose -noplot -grid=stir.hdf -debug=3
// ===================================================================================

#include "ExposedPoints.h"

#include "Ogen.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "interpPoints.h"
#include "CompositeGridOperators.h"
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  // Mapping::debug=7;

  printF("Usage: texpose [-noplot][-grid=<gridName>][-grid2=<gridName2>][-debug=<value>][file.cmd]\n");

  aString nameOfOGFile;
  nameOfOGFile="sisMove.hdf";  
  aString nameOfOGFile2="";  

  // also use grids: stir.hdf, bib2.hdf, twoDrop

  aString line,commandFileName;
  int plotOption=true; // true; // false;

  int len=0;
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      
      if( line=="-noplot" )
        plotOption=false;
      else if( line=="-plot" )
        plotOption=true;
      else if( len=line.matches("-grid=") )
      {
	nameOfOGFile=line(len,line.length()-1);
      }
      else if( len=line.matches("-grid2=") )
      {
	nameOfOGFile2=line(len,line.length()-1);
      }
      else if( len=line.matches("-debug=") )
      {
	sScanF(line(len,line.length()-1),"%i",&ExposedPoints::debug);
	printF(" Setting ExposedPoints::debug=%i \n",ExposedPoints::debug);
      }
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("Using command file = [%s]\n",(const char*)commandFileName);
      }
      
    }
  }

  PlotStuff ps(plotOption);
  // Note: options "noplot", "nopause" and "abortOnEnd" are handled in the next call:
  //GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("texpose",false,argc,argv);

  aString logFile="texpose.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char*)logFile);

  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);


  PlotStuffParameters psp;
  psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);

  if( nameOfOGFile2!="" )
  {
    // two grids are given on input -- 
    CompositeGrid cg1;
    getFromADataBase(cg1,nameOfOGFile);
    CompositeGrid cg2;
    getFromADataBase(cg2,nameOfOGFile2);
    
    if( true )
    {
      psp.set(GI_TOP_LABEL,"Old grid cg1");  
      PlotIt::plot(ps,cg1,psp);

      psp.set(GI_TOP_LABEL,"New grid cg2");  
      PlotIt::plot(ps,cg2,psp);
    }
    

    ExposedPoints exposedPoints;  // for interpolating exposed points
    int stencilWidth=5;
    exposedPoints.initialize(cg1,cg2,stencilWidth);
    //    exposedPoints.interpolate(u[oldCG],&exact);

    printF("Call to exposedPoints.initialize completed!\n");
    
    return 0;
  }
  

  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);

  CompositeGrid cg[2];                              // use these two grids for moving

  cg0.update(MappedGrid::THEvertex | MappedGrid::THEcenter);

  cg[0].reference(cg0);
  cg[1]=cg0; cg[1].update(MappedGrid::THEvertex | MappedGrid::THEcenter); 


  
  // Here is the grid generator
  Ogen gridGenerator(ps);


  psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
//  PlotIt::plot(ps,cg0,psp);


  
  LogicalArray hasMoved(cg[0].numberOfComponentGrids());
  hasMoved    = LogicalFalse;

  int numberOfGridsToMove=1;

  int gridToMove=cg0.numberOfComponentGrids()-1; // =1;



  int numberOfSteps=3;
  real deltaAngle;
  real xShift=.01, yShift=.01, zShift=.0;
  int debug=0;
  enum MoveOptions
  {
    rotate=0,
    shift
  } moveOption;

  int move;
  move=shift;
  
  if( nameOfOGFile.matches("cic") )
  {
    numberOfSteps=10;
    xShift=yShift=.05; // .01;
  }
  else if( nameOfOGFile.matches("stir") )
  {
    numberOfSteps=5;
    xShift=yShift=.02;
  }
  else if( nameOfOGFile.matches("twoDrop") || nameOfOGFile.matches("drop") )
  {
    gridToMove=1;
    
    numberOfSteps=1;
    xShift=0.; yShift=-.1; 
  }
  else if( nameOfOGFile.matches("bib") )
  {
    numberOfSteps=1;
    xShift=yShift=zShift=.05; // .01;
  }
  else if( nameOfOGFile.matches("sib") )
  {
    gridToMove=0;  //move background grid (should move two sphere grids)
    numberOfSteps=3;
    xShift=yShift=zShift=.05; // .01;
  }
  else if( nameOfOGFile.matches("drop3d") )
  {
    gridToMove=0;  //move background grid (should move two sphere grids)
    numberOfSteps=3;
    xShift=yShift=zShift=.0;
    yShift=.1;
  }

  moveOption = (MoveOptions)move;
  
//   if( moveOption==shift )
//   {
//     cout << "Enter numberOfSteps, shift amount, debug (1=interpolant, 2=Oges, 4=Ogshow) \n";
//     // cin >> numberOfSteps >> xShift >> debug;

//     numberOfSteps=2;
//     xShift=.05; // .01;
//     debug=1;

    
//   }
//   else
//   {
//     cout << "Enter numberOfSteps, rotation angle (degrees), debug (1=interpolant, 2=Oges, 4=Ogshow 8=exposed) \n";
//     cin >> numberOfSteps >> deltaAngle >> debug;
// //    numberOfSteps=51;
// //    deltaAngle=5.;
//     // ** debug =2;
    
//     deltaAngle*=Pi/180.;
//   }
  printF(" numberOfSteps=%i debug=%i xShift=%8.2e\n",numberOfSteps,debug,xShift);


  hasMoved(gridToMove) = LogicalTrue;  // Only this grid will move.

  // ---- Move the grid a bunch of times.----
  

  // Change the mapping on gridToMove:
  Mapping *mapPointer = cg0[gridToMove].mapping().mapPointer;

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform *transform[2];
  transform[0]= new MatrixTransform(*mapPointer); transform[0]->incrementReferenceCount();
  transform[1]= new MatrixTransform(*mapPointer); transform[1]->incrementReferenceCount();

  // Change the mappings that the grids point to:
  // Change the mapping of component grid 1:
  cg[0][gridToMove].reference(*transform[0]); // this will invalidate the mask ! 
  cg[1][gridToMove].reference(*transform[1]); 

  cg[0][gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter);  // the previous reference seems to destroy the data
  cg[1][gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter);  // the previous reference seems to destroy the data

  // update the initial grid, since the above reference destroys the mask
  gridGenerator.updateOverlap(cg[0]);
  


  // Here are some grid functions that we will use to interpolate exposed points
  realCompositeGridFunction u[2];
    
  Range all;
  u[0].updateToMatchGrid(cg[0],all,all,all,2); 
  u[1].updateToMatchGrid(cg[1],all,all,all,2); 
  u[0].setName("u");
  u[0].setName("u0",0);
  u[0].setName("u1",1);
  u[1].setName("u");
  u[1].setName("u0",0);
  u[1].setName("u1",1);
  // use this twilight-zone function so we can compute errors in interpolating exposed points
  int degreeX=1;
  OGPolyFunction exact(degreeX,cg0.numberOfDimensions(),2,1);   
  exact.assignGridFunction(u[0]);
  exact.assignGridFunction(u[1]);

  Interpolant interpolant;
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

//   CompositeGridOperators op(cg[0]);                            // create some differential operators
//   op.setStencilSize(stencilSize);

  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  ExposedPoints exposedPoints;  // for interpolating exposed points


  aString buff;
//  aString showFileTitle[2];
  real currentAngle=0.;
  int grid;

  real matrixSetUpTime=0.;
  real matrixSolveTime=0.;

  int numberOfArrays=GET_NUMBER_OF_ARRAYS;

  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    // Draw the overlapping grid

    if( plotOption )
    {
      if( moveOption==rotate )
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i, angle=%6.2e",i,currentAngle*180./Pi));  // set title
      else
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",i));  // set title
      //     psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      ps.erase();
      PlotIt::plot(ps,cg[oldCG],psp);
      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      ps.redraw(true);   // force a redraw
    }
    
    //  Rotate the grid...
    // After the first step we must double the angle since we start from the old grid
    if( moveOption==rotate )
    {
      real angle = i==1 ? deltaAngle : deltaAngle*2.; 
      transform[newCG]->rotate(axis3,angle);
      currentAngle+=deltaAngle;
    }
    else
    {
      real deltaX = i==1 ? xShift : xShift*2.; 
      real deltaY = i==1 ? yShift : yShift*2.; 
      real deltaZ = i==1 ? zShift : zShift*2.; 
      deltaZ= cg0.numberOfDimensions()==2 ? 0. : deltaZ;
      transform[newCG]->shift(deltaX,deltaY,deltaZ);
    }
    
    //      Update the overlapping newCG, starting with and sharing data with oldCG.    
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    
    int useFullAlgorithmInterval=10; // 10000;
    #ifdef USE_PPP
    useFullAlgorithmInterval=1;  // for now we only use the full algorithm in parallel
    #endif 
    if( i% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      printF("\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n");
      option=Ogen::useFullAlgorithm;
    }
    // gridGenerator.debug=7;
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option);
    
    // Interpolate any exposed points on the old grid function
    // (pass a TwilightZone function and the routine will compute errors)
    printF("Interpolate exposed points...\n");
    real time0=getCPU();

    // *wdh* old: interpolateExposedPoints(cg[oldCG],cg[newCG],u[oldCG],&exact);

    exposedPoints.initialize(cg[oldCG],cg[newCG]);
    exposedPoints.interpolate(u[oldCG],&exact);

    real time=getCPU()-time0;
    printF(" Time for interpolateExposedPoints=%8.2e\n",time);


    u[newCG].updateToMatchGrid(cg[newCG]);
    exact.assignGridFunction(u[newCG]);

  } // end for
  
  printF("Done! ...\n");
  for( int m=0; m<=1; m++ )
  {
    if( transform[m]->decrementReferenceCount()==0 )
      delete transform[m];
  }

  Overture::finish();          
  return 0;
}

