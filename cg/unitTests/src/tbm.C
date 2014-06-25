// ====================================================================================
///  \file tbm.C
///  \brief test program for the Beam Model classes
// ===================================================================================


#include "Overture.h"
// #include "SquareMapping.h"
#include "PlotStuff.h"
// #include "MatrixTransform.h"
// #include "CrossSectionMapping.h"
#include "BeamModel.h"
#include "NonlinearBeamModel.h"

#include "display.h"
// #include "App.h"
#include "NurbsMapping.h"

// #define W1(x,t) (a*sin(k*xl - w*t))
// #define W1t(x,t) (-a*w*cos(k*xl - w*t))
// #define W1x(x,t) (-k*a*cos(k*xl - w*t))
// #define W1tx(x,t) (a*k*w*sin(k*xl - w*t))

// // standing wave: 
// #define W(x,t) (a*sin(k*(x))*cos(w*(t)))
// #define Wt(x,t) (-w*a*sin(k*(x))*sin(w*(t)))
// #define Wx(x,t) (w*a*cos(k*(x))*cos(w*(t)))
// #define Wtx(x,t) (-w*a*cos(k*(x))*sin(w*(t)))


enum BeamModelEnum
{
  linearBeamModel=0,
  nonlinearBeamModel,
} beamModelType=linearBeamModel;

enum TestProblemEnum
{
  standingWave=0,
  travelingWave,
} testProblem=standingWave;

aString testProblemName[]=
{
  "standingWave",
  "travelingWave"
};


// ========================================================================================================
/// \brief Test class for the beam models
// ========================================================================================================

// momOfIntertia:    I/b (true area moment of inertia divided by the width of the beam
// E:                Elastic modulus
// rho:              beam density
// thickness:        beam thickness (assumed to be constant)
// pnorm:            value used to scale the pressure (i.e., the fluid density)
// bcleft:           beam boundary condition on the left
// x0:               initial location of the left end of the beam (x)
// y0:               initial location of the left end of the beam (y)
// useExactSolution: This flag sets the beam model to use the initial conditions
//                   from the exact solution (FSI) in the documentation.
// 
// void setParameters(real momOfIntertia, real E, 
// 			 real rho,real beamLength,
// 			 real thickness,real pnorm,
// 			 int nElem,BoundaryCondition bcleft,
// 			 BoundaryCondition bcright, 
// 			 real x0, real y0,
// 			 bool useExactSolution);
class TestBeamModel
{

public:

TestBeamModel();
~TestBeamModel();

int addForcing( real t );

// Check the forcing routines in the Beam Model
int checkForce();

int getErrors( real t );

int plot(real t, GenericGraphicsInterface & gi, GraphicsParameters & psp );

int solve(GenericGraphicsInterface & gi, GraphicsParameters & psp );




BeamModel beam;
NonlinearBeamModel nlBeam;

real t;
real dt;

int nElem;    // number of elements 
real cfl;
real tFinal; 
real tPlot;
int plotOption;
int orderOfAccuracy;
int debug;
real beamAngle;

real momOfIntertia, E, rho, beamLength, thickness, pnorm,  x0, y0, breadth;
// --- parameters for the exact solution ---
real a;  // amplitude 
real k0;
real k;
real w;

int globalStepNumber;

FILE *checkFile;

};

TestBeamModel::
TestBeamModel()
{
  t=0.;
  globalStepNumber=0;
  
  nElem=11;    // number of elements 
  cfl=.9;
  tFinal=.5; 
  tPlot=.02;
  plotOption=1;
  orderOfAccuracy=2;
  debug=1;  
  beamAngle=0.;
  
  // -- beam parameters: *FIX ME*
  momOfIntertia=1., E=1., rho=100., beamLength=1., thickness=.1, pnorm=10.,  x0=0., y0=0.;
  breadth=1.;

  // --- parameters for the exact solution ---
  a=.1;  // amplitude 
  k0=1.;
  k=2.*Pi*k0;
  w = sqrt( E*momOfIntertia*pow(k,4)/( rho*thickness*breadth ) );

  checkFile = fopen("tbm.check","w" );   // Here is the check file for regression tests

}

TestBeamModel::
~TestBeamModel()
{
  fclose(checkFile);
}
    

// ========================================================================================
/// \brief plot the beam solution.
// ========================================================================================
int TestBeamModel::
plot(real t, GenericGraphicsInterface & gi, GraphicsParameters & psp )
{

  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);     // set this to run in "movie" mode (after first plot)
  gi.erase();

  psp.set(GI_TOP_LABEL,sPrintF("beam model t=%9.3e",t));     // set this to run in "movie" mode (after first plot)

  RealArray xc;
  if( beamModelType==linearBeamModel )
  {
    beam.getCenterLine(xc);

    // ::display(xc,"beam center line","%8.2e ");
    
    NurbsMapping map; 
    map.interpolate(xc);

    real lineWidth=2;
    // psp.get(GraphicsParameters::lineWidth,lineWidthSave);  // default is 1
    // psp.set(GraphicsParameters::lineWidth,lineWidth);  

    Range I=xc.dimension(0);
    real xMin = min(xc(I,0)), xMax=max(xc(I,0));
    real yMin = min(xc(I,1)), yMax=max(xc(I,1));
    

    RealArray pb(2,3);  // plot bounds 
    pb(0,0)=xMin;  pb(1,0)=xMax;
    pb(0,1)=min(yMin,-yMax)-.1; pb(1,1)=max(yMax,-yMin)+.1;
    pb(0,2)=0.;  pb(1,2)=1.;

    psp.set(GI_PLOT_BOUNDS, pb);
    psp.set(GI_USE_PLOT_BOUNDS, true);
    // psp.set(GI_USE_PLOT_BOUNDS_OR_LARGER, true);

    // // -- plot points to set plot bounds : fix me ---
    // RealArray points(2,2);
    // points(0,0)=0.; points(0,1)=-a;
    // points(1,0)=beamLength; points(1,1)=a;
    // gi.plotPoints(points,psp);
    // psp.set(GI_USE_PLOT_BOUNDS,true);

    PlotIt::plot(gi, map,psp);      
    psp.set(GraphicsParameters::lineWidth,1);  // reset

  }
  else if( beamModelType==nonlinearBeamModel )
  {
    // nlBeam.getCenterLine(xc);
    nlBeam.plot(gi,psp);
  }
  else
  {
    OV_ABORT("ERROR: unknown beam model");
  }
  
  gi.redraw(true);
  usleep(100000);  // sleep in mirco-seconds
}



int TestBeamModel::
getErrors( real t )
{
  // RealArray x3,v3;
  // x3 = beam.position(); 
  // v3 = beam.velocity();
  
  real errMax=0., l2Err=0., yNorm=0.;

  RealArray xc;
  if( beamModelType==linearBeamModel )
  {
    beam.getCenterLine(xc);

    int nElem=beam.getNumberOfElements();

    RealArray ue(2*nElem+2), ve(2*nElem+2), ae;


    // beam.getStandingWave( t, ue, ve, ae );
    beam.getExactSolution( t, ue, ve, ae );

    // ::display(xc,"getErrors: beam center line","%8.2e ");

    for (int i = 0; i <= nElem; ++i)
    {
      real xl = ( (real)i /nElem) *  beamLength;
      // real we = W(xl,t);
      real we = ue(i*2);

      real err = fabs( xc(i,1)- we );

      printF("t=%9.3e i=%3i x=%9.3e w=%9.3e we=%9.3e err=%9.2e\n",t,i,xl,xc(i,1),we,err);
      
      errMax=max(errMax,err);
      l2Err += SQR(err);
      yNorm=yNorm+SQR(we);

    }
    l2Err=sqrt(l2Err/(nElem+1));
    yNorm=sqrt(yNorm/(nElem+1));
  }
  else if( beamModelType==nonlinearBeamModel )
  {
    nlBeam.getCenterLine(xc);

    int numNodes=nlBeam.getNumberOfNodes();

    RealArray xe(numNodes*2),ve(numNodes*2),ae(numNodes*2);
    
    nlBeam.setExactSolution(t,xe,ve,ae );

    printF("*** numNodes=%i\n",numNodes);
    // display(xc,sPrintF("xc at t=%9.3e",t),"%6.3f ");
    // display(xe,sPrintF("xe at t=%9.3e",t),"%6.3f ");
    

    printF("t=%9.3e: ",t);
    for (int i = 0; i < numNodes; ++i)
    {
      real err = fabs( xc(i,1)-xe(i*2) );
      errMax=max(errMax,err);
      l2Err += SQR(err);
      yNorm=yNorm+SQR(xe(i*2));
      printF(" y=%9.3e (ye=%9.3e), ",xc(i,1),xe(2*i));
    }
    l2Err=sqrt(l2Err/numNodes);
    yNorm=sqrt(yNorm/numNodes);
    printF("\n");
    
  }
  else
  {
    OV_ABORT("ERROR: unknown beam model");
  }

  printF("Error t=%9.3e, dt=%8.2e, numSteps=%i : max=%8.2e, l2=%8.2e, l2-rel=%8.2e\n",t,dt,globalStepNumber,errMax,l2Err,l2Err/max(1.e-12,yNorm));

  const int numberOfComponentsToOutput=1;
  fPrintF(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput);
  fPrintF(checkFile,"%i %9.2e %10.3e  ",0,errMax,yNorm);
  fPrintF(checkFile,"\n");

  return 0;
}

// ==================================================================================================================
/// \brief Add forcing at time t 
// ==================================================================================================================
int TestBeamModel::
addForcing( real t )
{
  // finish me 

  return 0;
}

// ==================================================================================================================
/// \brief Main time-stepping routine
// ==================================================================================================================
int TestBeamModel::
solve(GenericGraphicsInterface & gi, GraphicsParameters & psp )
{


  BeamModel::BoundaryCondition bcLeft=BeamModel::pinned, bcRight=BeamModel::pinned;
  // BeamModel::BoundaryCondition bcLeft=BeamModel::Periodic, bcRight=BeamModel::Periodic;
  bool useExactSolution=false;

  if( beamModelType==linearBeamModel )
  {
    beam.writeParameterSummary();
  }
  

  if( beamModelType==nonlinearBeamModel )
  {
    // aString beamFile = "mybeam.beam"; // *fix me* 
    // nlBeam.readBeamFile((const char*)beamFile);

    // real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
    real omega=.5;
    nlBeam.setAddedMassRelaxation(omega);

    // real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");
    real tol=1.e-5;
    nlBeam.setSubIterationConvergenceTolerance(tol);

    // do this for now:
    RealArray xc;
    nlBeam.getCenterLine(xc);
    int nbl = xc.getLength(0);

    nlBeam.initializeProjectedPoints(nbl);
    for (int i = 0; i<nbl; i++ )
    nlBeam.projectInitialPoint(i, xc(i,0),xc(i,1) );

  }

  // wave speed c= w/k ,   c*dt/dx = cfl 
  real dx=-1;
  dt=-1;
  if( beamModelType==linearBeamModel )
  {
    dt = beam.getExplicitTimeStep();
    dt=dt*cfl;
  }
  else if( beamModelType==nonlinearBeamModel )
  {
    dt = nlBeam.getExplicitTimeStep();
    dt=dt*cfl;
  }
  else
  {
    OV_ABORT("error");
  }
  
  // output to check file
  fPrintF(checkFile,"\\caption{tbm: test beam model: %s}\n",(beamModelType==linearBeamModel ? "linear beam model" : "nonlinear beam model"));


  int numberOfSteps= max(1, int( tFinal/dt + .5) );
  int nPlot = max(1,int( tPlot/dt+.5 ));

  dt = tPlot/nPlot;  // adjust dt so we reach tPlot exactly
      
  printF("+++ solve: dt=%9.3e, numberOfSteps=%i (cfl=%9.3e)\n",dt,numberOfSteps,cfl);



  int maximumNumberOfSteps=tFinal/dt+10;

  t=0.;

  GUIState dialog;
  dialog.setWindowTitle("tbm run-time");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"continue",
		    "movie mode",
		    "contour",
		    "" };
  int numberOfPushButtons=0;  // number of entries in cmds
  while( cmds[numberOfPushButtons]!="" ){numberOfPushButtons++;}; // 
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

   const int numberOfTextStrings=15;  // max number allowed
   aString textLabels[numberOfTextStrings];
   aString textStrings[numberOfTextStrings];

   int nt=0;
   textLabels[nt] = "tFinal:";  sPrintF(textStrings[nt],"%g",tFinal);  nt++; 
   textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  nt++; 

   // null strings terminal list
   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
   dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("tbm>");

  bool movieMode=false;
  aString answer;
  for( int step=0; step<maximumNumberOfSteps; step++ )
  {
    globalStepNumber=step;
    
    bool finished=t>tFinal-.5*dt;
    int plotThisStep=(step % nPlot == 0) || finished;
    if( plotThisStep )
    {
      if( finished )
	movieMode=false;

      // compute the max error:
      getErrors( t );
      
      // plot solution
      plot(t, gi,psp);

      // -- output results to the check file.
      // fprintf(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput+2); // print |\uv| and divergence too.
      // for( n=0; n<numberOfComponentsToOutput; n++ )
      // {
      //   real err = error(n) > checkFileCutoff(n) ? error(n) : 0.;
      //   real uc = max(fabs(uMin(n)),fabs(uMax(n)));
      //   if( uc<checkFileCutoff(n) ) uc=0.;
      //   fprintf(checkFile,"%i %9.2e %10.3e  ",n,err,uc);
      // }

    }

    if( !movieMode && plotThisStep )
    {
      for( ;; )
      {
	gi.getAnswer(answer,"");  
 
	if( answer=="continue" )
	{
	  if( finished )
	  {
	    printF("-- tbm-- Final time has been reached. Increase tFinal to continue.\n");
	    continue;
	  }
	  break;
	}
	else if( answer=="exit" || answer=="done" )
	{
	  gi.unAppendTheDefaultPrompt();
	  gi.popGUI(); // restore the previous GUI
	  return 0;
	}
	else if( answer=="movie mode" )
	{
	  movieMode=true;
	  break;
	}
	else if( answer=="contour" )
	{
	  // gi.erase();
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  // PlotIt::contour(gi,uPlot,psp);
	  // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

          // plot( t, ua[mCur],gi,psp ); // replot all
	}
	else if( dialog.getTextValue(answer,"tFinal:","%g",tFinal) )
	{
	  // for now we keep dt the same
	  maximumNumberOfSteps=tFinal/dt+10;
	  finished=t>tFinal-.5*dt;
	}
	else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
	else
	{
	  printF("Unknown response=[%s]\n",(const char*)answer);
	}
      }
    }


    // --------------------------------
    // ---- advance one time step -----
    // --------------------------------


    if( beamModelType==linearBeamModel )
    {
      addForcing( t );
      beam.predictor(t+dt, dt );

      addForcing( t+dt );
      beam.corrector(t+dt, dt );
    }
    else if( beamModelType==nonlinearBeamModel )
    {
      nlBeam.predictor(dt);

      nlBeam.corrector(dt);

    }
    else
    {
      OV_ABORT("ERROR: unknown beam model");
    }


    t = (step+1)*dt;
	
  }
      
  gi.unAppendTheDefaultPrompt();
  gi.popGUI(); // restore the previous GUI


  if( false )
  {
    const RealArray & xBeam = beam.position();  // (x1,x2) coordinates of the beam
    const RealArray & vBeam = beam.velocity();  // (v1,v2) components of the velocity of the beam
      
    ::display(xBeam,"xBeam","%5.2f ");
    ::display(vBeam,"vBeam","%5.2f ");
  }
      

  return 0;
}

// Check the forcing routines in the Beam Model
int TestBeamModel::
checkForce()
{

  
  // The pressure is p(X1) = p1, p(X2) = p2
  // x0_1: undeformed location of the point on the surface of the beam (x1)  
  // y0_1: undeformed location of the point on the surface of the beam (y1)
  // p1:   pressure at the point (x1,y1)
  // nx_1: normal at x1 (x) [unused]
  // ny_1: normal at x1 (y) [unused]
  // x0_2: undeformed location of the point on the surface of the beam (x2)  
  // y0_2: undeformed location of the point on the surface of the beam (y2)  
  // p2:   pressure at the point (x2,y2)
  // nx_2: normal at x2 (x) [unused]
  // ny_2: normal at x2 (y) [unused]
  // beam.addForce(const real& x0_1, const real& y0_1,
  // 		real p1,const real& nx_1,const real& ny_1,
  // 		const real& x0_2, const real& y0_2,
  // 		real p2,const real& nx_2,const real& ny_2);

  if( beamModelType==linearBeamModel )
  {
    real tf=0.;  // time to apply force

    real h=thickness*.5;
    real x0=.1, y0=h, x1=.5, y1=h;
    real nx0=0., ny0=1.;
    real nx1=0., ny1=1.;
  
    real p0=1., p1=1.;
    beam.resetForce();
    beam.addForce(tf, x0,y0,p1,nx0,ny0,  x1,y1,p1, nx1,ny1 );

    const RealArray & force = beam.force();
    ::display(force(Range(0,2*nElem,2)),"Top force","%8.2e ");

    beam.resetForce();
    x0=.5, y0=-h, x1=.1, y1=-h;
    beam.addForce(tf, x0,y0,p1,nx0,ny0,  x1,y1,p1, nx1,ny1 );
    ::display(force(Range(0,2*nElem,2)),"Bottom force","%8.2e ");

  }
  else if( beamModelType==nonlinearBeamModel )
  {
    OV_ABORT("ERROR: finish me!");
  }
  else
  {
    OV_ABORT("ERROR: unknown beam model");
  }



  return 0;
}



int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  printF("Usage: tbm -cmd=<command file> -noplot -nElem=<> -cfl=<> -tFinal=<> -tPlot=<> -debug=<> ... \n" );


  TestBeamModel tbm;
  CompositeGrid cg;  // not currently used

  int & debug = BeamModel::debug;
  NonlinearBeamModel::debug=3;

  debug=1;

  int & nElem= tbm.nElem;    // number of elements 
  int & plotOption = tbm.plotOption; 
  int & orderOfAccuracy = tbm.orderOfAccuracy;
  real & cfl = tbm.cfl;
  real & tFinal= tbm.tFinal; 
  real & tPlot= tbm.tPlot;
  real & beamAngle = tbm.beamAngle;
  
  aString commandFileName="";

  char buff[180];
  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot" or "-cfl=<value>"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false;
      else if( len=line.matches("-cfl=") )
      {
        sScanF(line(len,line.length()-1),"%e",&cfl);
	printF("cfl = %6.2f\n",cfl);
      }
      else if( len=line.matches("-tFinal=") )
      {
        sScanF(line(len,line.length()-1),"%e",&tFinal);
	printF("tFinal = %6.2f\n",tFinal);
      }
      else if( len=line.matches("-tf=") )
      {
        sScanF(line(len,line.length()-1),"%e",&tFinal);
	printF("tFinal = %6.2f\n",tFinal);
      }
      else if( len=line.matches("-nl") )
      {
	printF("Setting beamModelType=nonlinearBeamModel\n");
        beamModelType=nonlinearBeamModel;
      }
      else if( len=line.matches("-tp=") )
      {
        sScanF(line(len,line.length()-1),"%e",&tPlot);
	printF("tPlot = %6.3f\n",tPlot);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("debug = %i\n",debug);
        // RigidBodyMotion::debug=debug;
      }
      else if( len=line.matches("-nElem=") )
      {
        sScanF(line(len,line.length()-1),"%i",&nElem);
	printF("nElem = %i\n",nElem);
      }
      else if( len=line.matches("-cmd=") )
      {
        commandFileName=line(len,line.length()-1);
        printF("tbm: reading commands from file [%s]\n",(const char*)commandFileName);
      }
      else if( commandFileName=="" )
      {
	commandFileName=line;
	printF("tbm: setting command file to [$s]\n",(const char*)commandFileName);
      }
      
    }
  }

  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("tbm",plotOption,argc,argv);
  PlotStuffParameters psp;
  
  // By default start saving the command file called "tbm.cmd"
  aString logFile="tbm.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }

  aString answer;

  GUIState dialog;

  dialog.setWindowTitle("Beam model tester");
  dialog.setExitCommand("exit", "exit");

  aString opCommand1[] = {"linear beam model",
			  "nonlinear beam model",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Type:", opCommand1, opCommand1, beamModelType );



  aString opCommand2[] = {"traveling wave",
			  "standing wave",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Type:", opCommand2, opCommand2, testProblem );

  aString cmds[] = {"solve",
                    "check force",
                    "convergence rate",
                    "leak check",
                    "change beam parameters",
                    "exit",
		    ""};

  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"added mass",
                          "plot body",
			  ""};
  int tbState[10];
  // tbState[0] = addedMass;
  // tbState[1] = plotBody;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textLabels[nt] = "tFinal:"; 
  sPrintF(textStrings[nt],"%g",tFinal);  nt++; 

  textLabels[nt] = "tPlot:"; 
  sPrintF(textStrings[nt],"%g",tPlot);  nt++; 

  textLabels[nt] = "cfl:"; 
  sPrintF(textStrings[nt],"%g",cfl);  nt++; 

  textLabels[nt] = "beam angle:"; 
  sPrintF(textStrings[nt],"%g (degrees)",beamAngle);  nt++; 

  textLabels[nt] = "order of accuracy:"; 
  sPrintF(textStrings[nt],"%i",orderOfAccuracy);  nt++; 

  // textLabels[nt] = "numResolutions:"; 
  // sPrintF(textStrings[nt],"%i",numResolutions);  nt++; 

  textLabels[nt] = "debug:"; 
  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  //  textLabels[nt] = "output file:"; 
  // sPrintF(textStrings[nt],"%s",(const char*)outputFileName);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);


  for(;;)
  {
    
    gi.getAnswer(answer,"");  //  testProblem = (TestProblemEnum)ps.getMenuItem(menu,answer,"Choose a test");

    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="linear beam model" )
    {
      beamModelType=linearBeamModel;
    }
    else if( answer=="nonlinear beam model" )
    {
      beamModelType=nonlinearBeamModel;
    }
    
    else if( answer=="change beam parameters" )
    {
      if( beamModelType==linearBeamModel )
      {
	tbm.beam.update(cg,gi);
      }
      else if(  beamModelType==nonlinearBeamModel )
      {
	tbm.nlBeam.update(cg,gi);
      }
      else
      {
	OV_ABORT("error");
      }

    }

    else if( dialog.getTextValue(answer,"tFinal:","%e",tFinal) ){} //
    else if( dialog.getTextValue(answer,"tPlot:","%e",tPlot) ){} //
    else if( dialog.getTextValue(answer,"cfl:","%e",cfl) ){} //
    else if( dialog.getTextValue(answer,"beam angle:","%e",beamAngle) ){ } //
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    // else if( dialog.getTextValue(answer,"mass:","%e",trb.mass) ){} //
    else if( dialog.getTextValue(answer,"order of accuracy:","%i",orderOfAccuracy) ){} //
    // else if( dialog.getTextValue(answer,"numResolutions:","%i",numResolutions) ){} //
    // else if( dialog.getTextValue(answer,"output file:","%s",outputFileName) ){} //
    else if( answer=="traveling wave"  ||
	     answer=="standing wave" )
    {
      printF("testProblem=%i\n",(int)testProblem);

    }
    else if( answer=="check force" )
    {
      tbm.checkForce();
    }
    else if( answer=="solve" )
    {
      // ------------ Solve for the beam motion ----------------

      // trb.solve(gi);

      tbm.solve(gi,psp); 



    }
    else
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

  } // for(;;)
  
  gi.popGUI(); // restore the previous GUI

  Overture::finish(); 
  return 0;
}
