#include "Ogen.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"
#include "HyperbolicMapping.h"
#include "Ogshow.h"

//
// Deforming Grid Example: 
//   o 
//   o interpolate a grid function, update the interpolant for the new grid
//   o save solutions in a show file
//
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0; 

  int numberOfSteps=5; // 80;

  int plotOption=true;
  if( argc > 1 )
  { // look at arguments for "noplot"
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=false;
      else if( len=line.matches("-numSteps=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfSteps);
	printF("Setting numberOfSteps=%i\n",numberOfSteps);
      }
      
    }
  }

  aString nameOfOGFile="iceCircle.hdf"; // use this grid to start
//  cout << "Enter the name of the (old) overlapping grid file:" << endl;
//  cin >> nameOfOGFile;

  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg[2];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file
  cg[1]=cg[0];                                      // copy cg[0] into cg[1]

  // Deform component grid 2 (do this by changing the mapping)
  int gridToDeform=2;

  // The grid we deform will be a Hyperbolic Mapping, we need 2 of them:
  HyperbolicMapping transform[2];
  
  NurbsMapping *startCurve = new NurbsMapping [2]; // start curve representing the ice surface
  
  const int n0=13;
  realArray x0(n0,2), x1(n0,2), x2(n0,2);

  real rad=.5, theta; 

  // x0: points on a arc

  x0(0,0)=0.; x0(0,1)=1.*rad;
  theta=Pi* 5./180.; x0(1,0)=-rad*sin(theta); x0(1,1)=rad*cos(theta);
  theta=Pi*10./180.; x0(2,0)=-rad*sin(theta); x0(2,1)=rad*cos(theta);
  theta=Pi*15./180.; x0(3,0)=-rad*sin(theta); x0(3,1)=rad*cos(theta);
  theta=Pi*20./180.; x0(4,0)=-rad*sin(theta); x0(4,1)=rad*cos(theta);

  theta=Pi*50./180.; x0(5,0)=-rad*sin(theta); x0(5,1)=rad*cos(theta);
  theta=Pi*90./180.; x0(6,0)=-rad*sin(theta); x0(6,1)=rad*cos(theta);
  theta=Pi*130./180.;x0(7,0)=-rad*sin(theta); x0(7,1)=rad*cos(theta);

  x0( 8,0)=x0(4,0); x0( 8,1)=-x0(4,1);
  x0( 9,0)=x0(3,0); x0( 9,1)=-x0(3,1);
  x0(10,0)=x0(2,0); x0(10,1)=-x0(2,1);
  x0(11,0)=x0(1,0); x0(11,1)=-x0(1,1);
  x0(12,0)=x0(0,0); x0(12,1)=-x0(0,1);


  // a second start curve (deformed version)
  x1=x0;
  x1(5,0)=-.7; x1(5,1)=  .5;
  x1(6,0)=-.6; x1(6,1)=  0.;
  x1(7,0)=-.7; x1(7,1)= -.5;
  

  startCurve[0].interpolate(x0);
  startCurve[1].interpolate(x0);
  

  for( int i=0; i<=1; i++ )
  {
    const bool isSurfaceGrid=false, init=true;
    transform[i].setSurface(startCurve[i],isSurfaceGrid,init);
    transform[i].setGridDimensions(axis1,61);
    transform[i].setParameters(HyperbolicMapping::distanceToMarch,.25);
    transform[i].setParameters(HyperbolicMapping::linesInTheNormalDirection,9);
    transform[i].setShare(0,1,1);
    transform[i].generate();
  }
  
  // Replace the mapping of the component grid that we want to move:
  for( int i=0; i<=1; i++ )
  {
    cg[i][gridToDeform].reference(transform[i]); 
    cg[i].update();
  }
  


  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do but it will save space
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  // we tell the grid generator which grids have changed
  LogicalArray hasMoved(cg[0].numberOfGrids());
  hasMoved    = LogicalFalse;
  hasMoved(gridToDeform) = LogicalTrue;  // Only this grid will change
  char buff[80];

  PlotStuff ps(plotOption,"Deforming Grid Example");         // for plotting
  PlotStuffParameters psp;
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

  real delta=1./numberOfSteps;

  // ---- Deform the grid in a periodic fashion.----
  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    
    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step=%i",i));  // set title
    PlotIt::plot(ps,cg[oldCG],psp);      // plot the current overlapping grid
    if( i>10 )
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
    ps.redraw(TRUE);


    real deltaOmega=1./20.;
    real omega=pow(sin(Pi*i*deltaOmega),2.);  // omega varies in the interval [0,1]
    
    x2 = (1.-omega)*x0 + omega*x1;
    startCurve[newCG].interpolate(x2);   // form the new start curve (NurbsMapping)
    
    const bool isSurfaceGrid=false;
    const bool init=false; // this means keep existing hype parameters such as distanceToMarch, linesToMarch etc. 
    transform[newCG].setSurface(startCurve[newCG],isSurfaceGrid,init);  // supply a new start curve

    // ** generate the new grid with the hyperbolic grid generator **
    transform[newCG].generate();   

    // Update the overlapping grid newCG, starting with and sharing data with oldCG.
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    int useFullAlgorithmInterval=1; // 10;
    if( i% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      cout << "\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n";
      option=Ogen::useFullAlgorithm;
    }
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option );

    interpolant.updateToMatchGrid(cg[newCG]);
    u.updateToMatchGrid(cg[newCG]);

    // Make sure the vertex array is built:
    cg[newCG].update( MappedGrid::THEvertex | MappedGrid::THEcenter );
    
    // assign values to u
    Index I1,I2,I3;
    for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      const realArray & vertex = mg.vertex();
      getIndex(mg.dimension(),I1,I2,I3);
      real freq = 2.*i/numberOfSteps;
      u[grid](I1,I2,I3)=cos(freq*vertex(I1,I2,I3,axis1))*sin(freq*vertex(I1,I2,I3,axis2));  
    }
    u.interpolate();
    // save the result in a show file, every fourth step
    if( (i % 4) == 1 )
    {
      show.startFrame();
      show.saveComment(0,sPrintF(buff,"Solution at step = %i",i));
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

