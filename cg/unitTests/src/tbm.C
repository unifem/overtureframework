// ====================================================================================
///  \file tbm.C
///  \brief test program for the Beam Model classes
// ===================================================================================


#include "Overture.h"
#include "PlotStuff.h"
#include "FEMBeamModel.h"
#include "FDBeamModel.h"

#include "NonlinearBeamModel.h"
#include "display.h"
#include "NurbsMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

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
  
//Longfei 20160120:
// Check if beam model is properly initialized
bool checkBeamInitialization();
  
// Check the internal forcing routines in the Beam Model
int checkInternalForce();

// Check the velocity projection
int checkVelocityProjection();

int getErrors( real t );

int plot(real t, GenericGraphicsInterface & gi, GraphicsParameters & psp );

int solve(GenericGraphicsInterface & gi, GraphicsParameters & psp );




  BeamModel* pbeam;   // Longfei 20160116: use pointer to handle polymorphism
  //  BeamModel beam;     
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

int forceDegreeX;  // degree of forcing polynmial for checkForce

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

  forceDegreeX=1;  // degree of forcing polynmial for checkForce

  // --- parameters for the exact solution ---
  a=.1;  // amplitude 
  k0=1.;
  k=2.*Pi*k0;
  w = sqrt( E*momOfIntertia*pow(k,4)/( rho*thickness*breadth ) );

  checkFile = fopen("tbm.check","w" );   // Here is the check file for regression tests

  //Longfei 20160116: use pointer to handle polymorphism
  pbeam = NULL;   // which BeamModel to use is determined at runtime

}

TestBeamModel::
~TestBeamModel()
{
  if(pbeam!=NULL)
    delete pbeam;

  fclose(checkFile);
}

//Longfei 20160120:
// ========================================================================================
/// \brief plot the beam solution.
// ========================================================================================
bool TestBeamModel::
checkBeamInitialization()
{
  bool ok=true;
  if(pbeam==NULL)
    {
      printF("Warning: beam model is NULL. Chose FEM or FD first\n");
      ok=false;
    }
  else if(!pbeam->dbase.get<bool>("initialized"))
    {
      printF("Warning: beam model is not initialized. Choose: change beam parameters first\n");
      ok=false;
    }

  return ok;
}

// ========================================================================================
/// \brief plot the beam solution.
// ========================================================================================
int TestBeamModel::
plot(real t, GenericGraphicsInterface & gi, GraphicsParameters & psp )
{
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;

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
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;
  // RealArray x3,v3;
  // x3 = beam.position(); 
  // v3 = beam.velocity();
  
  real errMax=0., l2Err=0., yNorm=0.;

  RealArray xc;
  int numNodes=0;
  if( beamModelType==linearBeamModel )
  {
    if( true )
    {
      real uvErr[3], uvNorm[3]; //Longfei 20160301: changed size from 2 to 3 to hold errror for acceleeration as well
      beam.getErrors( "tbm",  stdout, uvErr,uvNorm  );
      //Longfei 20160301: old way
      // errMax=uvErr[0];
      //yNorm=uvNorm[0];
      // new way
      beam.writeCheckFile(checkFile);
      
    }
    else
    {
      // ** OLD WAY**
      beam.getCenterLine(xc);

      int nElem=beam.getNumberOfElements();
      numNodes=nElem+1;

      RealArray ue(2*nElem+2), ve(2*nElem+2), ae;


      beam.getExactSolution( t, ue, ve, ae );

      // ::display(xc,"getErrors: beam center line","%8.2e ");

      for (int i = 0; i <= nElem; ++i)
      {
	real xl = ( (real)i /nElem) *  beamLength;
	// real we = W(xl,t);
	real we = ue(i*2);

	real err = fabs( xc(i,1)- we );

	if( beam.debug & 2 )
	  printF("t=%9.3e i=%3i x=%9.3e w=%9.3e we=%9.3e err=%9.2e\n",t,i,xl,xc(i,1),we,err);
      
	errMax=max(errMax,err);
	l2Err += SQR(err);
	yNorm=yNorm+SQR(we);

      }
      l2Err=sqrt(l2Err/(nElem+1));
      yNorm=sqrt(yNorm/(nElem+1));
    }
    
  }
  else if( beamModelType==nonlinearBeamModel )
  {
    nlBeam.getCenterLine(xc);

    numNodes=nlBeam.getNumberOfNodes();

    RealArray xe(numNodes*2),ve(numNodes*2),ae(numNodes*2);
    
    nlBeam.setExactSolution(t,xe,ve,ae );

    // printF("*** numNodes=%i\n",numNodes);
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
    
    printF("Error Ne=%i, t=%9.3e, dt=%8.2e, numSteps=%i : max=%8.2e, l2=%8.2e, l2-rel=%8.2e\n",numNodes-1,t,dt,globalStepNumber,errMax,l2Err,l2Err/max(1.e-12,yNorm));


    // Longfei 20160301: keep this for nonlinear beam. For linear beam we use beamModel.writeCheckfile(file)
    const int numberOfComponentsToOutput=1;

    fPrintF(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput);
    fPrintF(checkFile,"%i %9.2e %10.3e  ",0,errMax,yNorm);
    fPrintF(checkFile,"\n");
    

  }
  else
  {
    OV_ABORT("ERROR: unknown beam model");
  }



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
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;

  BeamModel::BoundaryCondition bcLeft=BeamModel::pinned, bcRight=BeamModel::pinned;
  // BeamModel::BoundaryCondition bcLeft=BeamModel::Periodic, bcRight=BeamModel::Periodic;
  bool useExactSolution=false;

  if( beamModelType==linearBeamModel )
    {
      beam.setParameter("cfl",cfl);

      //beam.writeParameterSummary();
    }
  

  if( beamModelType==nonlinearBeamModel )
    {
      nlBeam.setParameter("cfl",cfl);

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
      // dt=dt*cfl;
    }
  else if( beamModelType==nonlinearBeamModel )
    {
      dt = nlBeam.getExplicitTimeStep();
      // dt=dt*cfl;
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

  bool plotBeam=true;   // plot beam, or plot beam variables 
  aString tbCommands[] = {"plot beam",
                          ""};
  int tbState[10];
  tbState[0] = plotBeam;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "tFinal:";  sPrintF(textStrings[nt],"%g",tFinal);  nt++; 
  textLabels[nt] = "tPlot:";  sPrintF(textStrings[nt],"%g",tPlot);  nt++; 
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
	  if( plotBeam )
	    {
	      // plot beam center line
	      plot(t, gi,psp);
	    }
	  else
	    {
	      // plot beam variables, errors, etc. 
	      gi.erase();
	      psp.set(GI_USE_PLOT_BOUNDS,false);
	      aString label="laal";
	      if( beamModelType==linearBeamModel )
		beam.plot( t, gi,psp,label );

	      gi.redraw(true);
	    }
      

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
		  gi.erase();
		  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
		  psp.set(GI_USE_PLOT_BOUNDS,false);

		  // plot the solution, errors, etc.
		  aString label="tbm";
		  if( beamModelType==linearBeamModel )
		    beam.plot( t, gi,psp,label );

		  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

		  // plot( t, ua[mCur],gi,psp ); // replot all
		}
	      else if( dialog.getToggleValue(answer,"plot beam",plotBeam) ){}//
	      else if( dialog.getTextValue(answer,"tFinal:","%g",tFinal) )
		{
		  // for now we keep dt the same
		  maximumNumberOfSteps=tFinal/dt+10;
		  finished=t>tFinal-.5*dt;
		}
	      else if( dialog.getTextValue(answer,"tPlot:","%g",tPlot) )
		{
		  // for now we keep dt the same
		  nPlot = max(1,int( tPlot/dt+.5 ));
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
	  beam.resetForce();
	  addForcing( t );
	  beam.predictor(t+dt, dt );

	  beam.resetForce();
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
      const RealArray & xBeam = beam.displacement();  // (x1,x2) coordinates of the beam
      const RealArray & vBeam = beam.velocity();  // (v1,v2) components of the velocity of the beam
      
      ::display(xBeam,"xBeam","%5.2f ");
      ::display(vBeam,"vBeam","%5.2f ");
    }
      

  return 0;
}

// =======================================================================================
/// \brief Check the forcing routines in the Beam Model
// =======================================================================================
int TestBeamModel::
checkForce()
{
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;
  

  const int numberOfDimensions=2;
  
  const int nElem=beam.getNumberOfElements();
  int numElem=nElem;

  if( beamModelType==linearBeamModel )
  {
    real tf=0.;  // time to apply force

    real h=thickness*.5;
    real x0=.0, y0=h, x1=1., y1=h;
    real nx0=0., ny0=1.;
    real nx1=0., ny1=1.;
  

#define FORCE(x) (p0+pow(x,forceDegreeX)*(p1-p0))
#define FORCEX(x) (forceDegreeX*pow(x,forceDegreeX-1)*(p1-p0))
    real p0=0., p1=1.;
    beam.resetForce();

    // --- exact force on beam nodes ---
    real dxe=1./numElem; // assumes Beam length of 1
    RealArray xe(numElem+1), fe(numElem+1);
    for( int i=0; i<=numElem; i++ )
    {
      xe(i) = x0 + i*dxe;  fe(i) = FORCE(xe(i));   // exact solution on beam nodes
    }

    // Assign force at a different number of points: 
    int numForce=numElem+7; // +1;    // number of forcing grid points 
    real dxf = (x1-x0)/(numForce-1);
    Index Ib1=Range(numForce), Ib2=Range(0,0), Ib3=Range(0,0);

    RealArray xi(Ib1,Ib2,Ib3,numberOfDimensions), fi(Ib1,Ib2,Ib3,numberOfDimensions), normal(Ib1,Ib2,Ib3,numberOfDimensions);
    for( int i=0; i<numForce; i++ )
    {
      xi(i,0,0,0) = x0 + i*dxf; 
      xi(i,0,0,1) = +h;  // top of beam
      fi(i,0,0,0) = 0.; fi(i,0,0,1) = FORCE(xi(i)); 
    }
    // do this: (should match force vector above)
    normal(Ib1,Ib2,Ib3,0)=0.;
    normal(Ib1,Ib2,Ib3,1)=1.;
      
    // Assign forces to points on the beam 
   
    beam.addForce(  tf,xi,fi,normal,Ib1,Ib2,Ib3 );
      
    const RealArray & force = beam.force();
    ::display(force(Range(0,2*nElem,2)),"Top (element) force","%8.2e ");
    ::display(fe,"Exact force","%8.2e ");

    RealArray fc;
    beam.getForceOnBeam( tf, fc );  // point-wise force values on the center-line
    ::display(fc,"Force on center-line","%8.2e ");
    real signForNormal=-1.;
    real maxErr1 = max(fabs(fc-fe*signForNormal));
    printF("\n +++ Max error in center-line force = %8.2e (degreeForce=%i) \n\n",maxErr1,forceDegreeX );

    beam.resetForce();
    for( int i=0; i<numForce; i++ )
    {
      xi(i,0,0,0) = x0 + i*dxf; 
      xi(i,0,0,1) = -h;  // bottom of beam
      fi(i,0,0,0) = 0.; fi(i,0,0,1) = FORCE(xi(i)); 
    }

    beam.addForce(  tf,xi,fi,normal,Ib1,Ib2,Ib3 );
      
    ::display(force(Range(0,2*nElem,2)),"Bottom force","%8.2e ");

    beam.getForceOnBeam( tf, fc );  // point-wise force values on the center-line
    ::display(fc,"Force on center-line","%8.2e ");

    signForNormal=1.;
    real maxErr2 = max(fabs(fc-fe*signForNormal));
    printF("\n +++ Max error in center-line force = %8.2e (degreeForce=%i) \n\n",maxErr2,forceDegreeX );

    // output results to the check file 
    // output to check file
    int orderOfGalerkinProjection;
    beam.getParameter( "orderOfGalerkinProjection",orderOfGalerkinProjection);
      
    fPrintF(checkFile,"\\caption{tbm: test beam model: %s, CHECK FORCE, forceDegreeX=%i, orderOfGalerkinProjection=%i}\n",
	    (beamModelType==linearBeamModel ? "linear beam model" : "nonlinear beam model"),forceDegreeX,orderOfGalerkinProjection);

    const int numberOfComponentsToOutput=2;
    real fNorm=1.;
    fPrintF(checkFile,"%9.2e %i  ",tf,numberOfComponentsToOutput);
    fPrintF(checkFile,"%i %9.2e %10.3e  %i %9.2e %10.3e  ",0,maxErr1,fNorm,1,maxErr2,fNorm);
    fPrintF(checkFile,"\n");

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


// =======================================================================================
/// \brief Check the internal forcing routines in the Beam Model
// =======================================================================================
int TestBeamModel::
checkInternalForce()
{
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;

  // void getSurfaceInternalForce( const real t, const RealArray & x0,  const RealArray & fs, 
  //                               const Index & Ib1, const Index & Ib2,  const Index & Ib3,
  //                               const bool addExternalForcing );

  beam.getParameter("thickness",thickness);

  const int numberOfDimensions=2;
  
  const int nElem=beam.getNumberOfElements();
  int numElem=nElem;
  real beamLength;
  beam.getParameter("length",beamLength);

  if( beamModelType==linearBeamModel )
  {
    // Longfei 20160120: new way of handling parameters
    //const real EI = beam.elasticModulus*beam.areaMomentOfInertia;
    const real & EI = beam.dbase.get<real>("EI");
    //real density = beam.density;
    const real & density = beam.dbase.get<real>("density");


    real tf=0.;  // time to apply force
    t=tf;

    real h=thickness*.5;

    printF("---- checkInternalForce thickness=%8.2e -----\n",thickness);
    
    RealArray u,v,a;
    beam.getExactSolution( tf, u,v,a );
    Range N=numElem+1, N2=2*(numElem+1);
    if( false )
    {
      u.reshape(2,N); v.reshape(2,N);
      ::display(u,"exact solution: u","%6.3f ");
      ::display(v,"exact solution: v","%6.3f ");
      u.reshape(N2); v.reshape(N2);
    }
    
    real x0=.0, y0=h, x1=1., y1=h;
    int numForce=numElem+1; // +7;    // number of forcing grid points 
    real dxf = (x1-x0)/(numForce-1);
    Index Ib1=Range(numForce), Ib2=Range(0,0), Ib3=Range(0,0);

    real signForNormal=-1.;  // beam on top 

    RealArray xi(Ib1,Ib2,Ib3,numberOfDimensions), fs(Ib1,Ib2,Ib3,numberOfDimensions);
    for( int i=0; i<numForce; i++ )
    {
      xi(i,0,0,0) = x0 + i*dxf; 
      xi(i,0,0,1) = +h;  // top of beam
      // fi(i,0,0,0) = 0.; fi(i,0,0,1) = FORCE(xi(i)); 
    }
    RealArray normal(Ib1,Ib2,Ib3,numberOfDimensions);
    // do this: (should match force vector)
    normal(Ib1,Ib2,Ib3,0)=0.;
    normal(Ib1,Ib2,Ib3,1)=1.;

    bool addExternalForcing=false;
    beam.getSurfaceInternalForce(tf,xi,fs, normal,Ib1,Ib2,Ib3,addExternalForcing);

    ::display(fs,"surface  force L(u,v)","%9.2e ");

    if( false )
    {
      RealArray fi(nElem+1,2);

      const int & current = beam.dbase.get<int>("current"); 
      std::vector<RealArray> & ua = beam.dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
      std::vector<RealArray> & va = beam.dbase.get<std::vector<RealArray> >("v"); // velocity DOF
      RealArray & uc = ua[current];  // current displacement 
      RealArray & vc = va[current];  // current velocity 

      beam.computeInternalForce(uc,vc,fi);

      uc.reshape(2,N); vc.reshape(2,N); fi.reshape(2,N);
      ::display(uc,"u[current]","%9.2e ");
      ::display(vc,"v[current]","%9.2e ");
      ::display(fi,"internal force L(uc,vc)","%9.2e ");
      uc.reshape(N2); vc.reshape(N2); fi.reshape(N2);
    }
    
    // exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );

    assert( beam.getExactSolutionOption()=="twilightZone" ) ;
    
    // ---- Compute the EXACT internal force on the beam reference line ----

    bool & twilightZone =  beam.dbase.get<bool>("twilightZone");
    assert( twilightZone );
    assert(  beam.dbase.get<OGFunction*>("exactPointer")!=NULL );
    OGFunction & exact = * beam.dbase.get<OGFunction*>("exactPointer");

    Index I1,I2,I3;
    I1=Range(0,numElem); I2=0; I3=0;

    RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
    const real dx=beamLength/numElem;
    for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = 0.;    // should this be y0 ?
    }

    RealArray ue(I1,I2,I3,1), ve(I1,I2,I3,1),  uxxe(I1,I2,I3,1), uxxte(I1,I2,I3,1), uxxxxe(I1,I2,I3,1);
    const int isRectangular=0;
    const int wc=0;
    const int domainDimension=1;
    exact.gd( ue    ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,tf );
    exact.gd( ve    ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,tf );
    exact.gd( uxxe  ,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,tf );
    exact.gd( uxxte ,x,domainDimension,isRectangular,1,2,0,0,I1,I2,I3,wc,tf );
    exact.gd( uxxxxe,x,domainDimension,isRectangular,0,4,0,0,I1,I2,I3,wc,tf );

    RealArray beamOp(I1,I2,I3,1);

      
    const real & T = beam.dbase.get<real>("tension");
    const real & K0 = beam.dbase.get<real>("K0");
    const real & Kt = beam.dbase.get<real>("Kt");
    const real & Kxxt = beam.dbase.get<real>("Kxxt");
    printF(" K0=%g, Kt=%g, Kxxt=%g\n",K0,Kt,Kxxt);

    beamOp= signForNormal*( -K0*ue -Kt*ve + Kxxt*uxxte + T*uxxe - EI*uxxxxe );
    ::display(beamOp,"exact internal force: L(u,v)","%9.2e ");

    real err = max(fabs(beamOp(I1,I2,I3)-fs(Ib1,Ib2,Ib3,1)));
    printF("\n +++ numElem = %i, maximum err in beam operator L(u,v) = %8.2e\n",numElem,err);

    // output results to the check file 
    int degreeInSpace, degreeInTime;
    beam.getParameter( "degreeInSpace",degreeInSpace);
    beam.getParameter( "degreeInTime",degreeInTime);
      
    fPrintF(checkFile,"\\caption{tbm: test beam model: %s, CHECK INTERNAL FORCE, degreeInSpace=%i}\n",
	    (beamModelType==linearBeamModel ? "linear beam model" : "nonlinear beam model"),degreeInSpace);

    const int numberOfComponentsToOutput=1;
    real fNorm=1.;
    fPrintF(checkFile,"%9.2e %i  ",tf,numberOfComponentsToOutput);
    fPrintF(checkFile,"%i %9.2e %10.3e ",0,err,fNorm);
    fPrintF(checkFile,"\n");


    // -- Now apply a force 
    printF("\n --- Apply a force : density=%e, thickness=%e\n",density,thickness);

    RealArray utte(I1,I2,I3,1);
    exact.gd( utte   ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,tf );
    RealArray ftz(I1,I2,I3,2);
    ftz=0.;
    ftz(I1,I2,I3,1) = signForNormal*( (density*thickness)*utte + K0*ue +Kt*ve - Kxxt*uxxte - (T)*uxxe + (EI)*uxxxxe );



    // Assign force at a different number of points: 
    // int numForce=numElem+7; // +1;    // number of forcing grid points 
    // real dxf = (x1-x0)/(numForce-1);
    Ib1=Range(numForce), Ib2=Range(0,0), Ib3=Range(0,0);

    for( int i=0; i<numForce; i++ )
    {
      xi(i,0,0,0) = x0 + i*dxf; 
      xi(i,0,0,1) = +h;  // top of beam
    }
	

      
    // Assign forces to points on the beam
    beam.resetForce();
    beam.addForce(  tf,xi,ftz,normal,Ib1,Ib2,Ib3 );

    const RealArray & force = beam.force();
    ::display(force(Range(0,2*nElem,2)),"Top (element) force","%8.2e ");
    ::display(ftz(I1,I2,I3,1),"Exact force","%8.2e ");

    RealArray fc;
    beam.getForceOnBeam( tf, fc );  // point-wise force values on the center-line
    ::display(fc,"Force on center-line","%8.2e ");
    real maxErr1 = max(fabs(fc-ftz(I1,I2,I3,1)*signForNormal));
    printF("\n +++ Max error in center-line force = %8.2e (degreeX=%i, degreeT=%i) \n\n",maxErr1,degreeInSpace,degreeInTime );

    addExternalForcing=true;
    beam.getSurfaceInternalForce(tf,xi,fs, normal,Ib1,Ib2,Ib3,addExternalForcing);

    ::display(fs,"surface  force = L(u,v) + f ","%9.2e ");

    RealArray ft(Ib1,Ib2,Ib3);
    ft =  (density*thickness)*utte;

    // ::display(utte,"utte ","%9.2e ");
    ::display(ft," rho*h*utte ","%9.2e ");

    real maxErr = max(fabs(fs(Ib1,Ib2,Ib3,1)-ft));
    printF("\n +++  numElem = %i, Max error in L(u,v)+f = %8.2e +++\n",numElem,maxErr);

/* -----
    // RealArray fi(Ib1,Ib2,Ib3,numberOfDimensions
    // beam.addInternalForces( tf, fa )


    real p0=0., p1=1.;
    beam.resetForce();

    // --- exact force on beam nodes ---
    real dxe=1./numElem; // assumes Beam length of 1
    RealArray xe(numElem+1), fe(numElem+1);
    for( int i=0; i<=numElem; i++ )
    {
      xe(i) = x0 + i*dxe;  fe(i) = FORCE(xe(i));   // exact solution on beam nodes
    }

    // Assign force at a different number of points: 
    // int numForce=numElem+7; // +1;    // number of forcing grid points 
    // real dxf = (x1-x0)/(numForce-1);
    Ib1=Range(numForce), Ib2=Range(0,0), Ib3=Range(0,0);

    RealArray fi(Ib1,Ib2,Ib3,numberOfDimensions), normal(Ib1,Ib2,Ib3,numberOfDimensions);
    for( int i=0; i<numForce; i++ )
    {
      xi(i,0,0,0) = x0 + i*dxf; 
      xi(i,0,0,1) = +h;  // top of beam
      fi(i,0,0,0) = 0.; fi(i,0,0,1) = FORCE(xi(i)); 
    }
    // do this: (should match force vector above)
    normal(Ib1,Ib2,Ib3,0)=0.;
    normal(Ib1,Ib2,Ib3,1)=1.;
      
    // Assign forces to points on the beam 
   
    beam.addForce(  tf,xi,fi,normal,Ib1,Ib2,Ib3 );
      
    const RealArray & force = beam.force();
    ::display(force(Range(0,2*nElem,2)),"Top (element) force","%8.2e ");
    ::display(fe,"Exact force","%8.2e ");

    RealArray fc;
    beam.getForceOnBeam( tf, fc );  // point-wise force values on the center-line
    ::display(fc,"Force on center-line","%8.2e ");
    real signForNormal=-1.;
    real maxErr1 = max(fabs(fc-fe*signForNormal));
    printF("\n +++ Max error in center-line force = %8.2e (degreeForce=%i) \n\n ",maxErr1,forceDegreeX );

    addExternalForcing=true;
    beam.getSurfaceInternalForce(tf,xi,fs,Ib1,Ib2,Ib3,addExternalForcing);

    ::display(fs,"surface  force = L(u,v) + f ","%9.2e ");

   ---- */


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


// ===========================================================================================
/// \brief Check the velocity projection
// ===========================================================================================
int TestBeamModel::
checkVelocityProjection()
{
  // Longfei 20160116: member beam is replaced by its pointer pbeam.
  // Create a reference named beam to avoid code changes.
  BeamModel &beam = *pbeam;

  // Given a velocity defined on the beam surface(s) project these values
  // onto the beam neutral surface. 
  if( beamModelType==linearBeamModel )
  {

    printF("---- CHECK VELOCITY PROJECTION ----\n");

    real thickness;
    beam.getParameter("thickness",thickness);
    const real halfThickness= thickness*.5;

    real t=0.;

    RealArray xc;
    // beam.getCenterLine( xc );
    // ::display(xc,"Beam centerline");

    // Extract the velocity on the surface of the beam

    int numElem=beam.getNumberOfElements();

    // Evaluate the surface velocity at a different number of nodes from the beam
    int numVelocityNodes =  numElem + 7;

    Index Ib1,Ib2,Ib3;
    Ib1=Range(numVelocityNodes); Ib2=Range(0,0); Ib3=Range(0,0);
    int numberOfDimensions=2;
    Range Rx=numberOfDimensions;
    RealArray x0Plus(Ib1,Ib2,Ib3,Rx), x0Minus(Ib1,Ib2,Ib3,Rx), vPlus(Ib1,Ib2,Ib3,Rx), vMinus(Ib1,Ib2,Ib3,Rx),
              normalPlus(Ib1,Ib2,Ib3,Rx), normalMinus(Ib1,Ib2,Ib3,Rx);

    // --- evaluate the beam centerline on a different number of points:
    real beamLength;
    beam.getParameter("length",beamLength);
    real dxv=beamLength/(numVelocityNodes-1);
    for( int i1=Ib1.getBase(); i1<=Ib1.getBound(); i1++ )
      x0Plus(i1,0,0)=i1*dxv;
    x0Plus(Ib1,Ib2,Ib3,1) = 0.;   // y value 
    xc.redim(Ib1,Ib2,Ib3,Rx);
    beam.getSurface( t,x0Plus,xc,Ib1,Ib2,Ib3 );
    ::display(xc,"Beam centerline (with a different number of points");

    // x0Plus : initial locations of points on the beam upper surface
    x0Plus(Ib1,Ib2,Ib3,0) =xc(Ib1,Ib2,Ib3,0); x0Plus(Ib1,Ib2,Ib3,1) = halfThickness;
    x0Minus(Ib1,Ib2,Ib3,0)=xc(Ib1,Ib2,Ib3,0); x0Minus(Ib1,Ib2,Ib3,1)=-halfThickness;

    RealArray xsPlus(Ib1,Ib2,Ib3,Rx), xsMinus(Ib1,Ib2,Ib3,Rx), norm(Ib1,Ib2,Ib3);
    beam.getSurface( t,x0Plus,xsPlus,Ib1,Ib2,Ib3 );
    beam.getSurface( t,x0Minus,xsMinus,Ib1,Ib2,Ib3 );

    // Here are the normals (inward to the beam)
    for( int axis=0; axis<numberOfDimensions; axis++ )
      normalPlus(Ib1,Ib2,Ib3,axis)=-(xsPlus(Ib1,Ib2,Ib3,axis)-xc(Ib1,Ib2,Ib3,axis));
    norm = sqrt( SQR(normalPlus(Ib1,Ib2,Ib3,0))+SQR(normalPlus(Ib1,Ib2,Ib3,1)));
    for( int axis=0; axis<numberOfDimensions; axis++ )
      normalPlus(Ib1,Ib2,Ib3,axis)/=norm;
    
    for( int axis=0; axis<numberOfDimensions; axis++ )
      normalMinus(Ib1,Ib2,Ib3,axis)= -(xsMinus(Ib1,Ib2,Ib3,axis)-xc(Ib1,Ib2,Ib3,axis));
    norm = sqrt( SQR(normalMinus(Ib1,Ib2,Ib3,0))+SQR(normalMinus(Ib1,Ib2,Ib3,1)));
    for( int axis=0; axis<numberOfDimensions; axis++ )
      normalMinus(Ib1,Ib2,Ib3,axis)/=norm;
    
    ::display(xsPlus,"Beam top surface x+");
    ::display(xsMinus,"Beam bottom surface x-");

    ::display(normalPlus,"Beam top surface normal+");
    ::display(normalMinus,"Beam bottom surface normal-");

    beam.getSurfaceVelocity( t,x0Plus,vPlus,Ib1,Ib2,Ib3 );
    beam.getSurfaceVelocity( t,x0Minus,vMinus,Ib1,Ib2,Ib3 );

    ::display(vPlus,"Beam top surface velocity v+");
    ::display(vMinus,"Beam bottom surface velocity v-");

    // Now assign the surface velocity
    beam.resetSurfaceVelocity();
    
    beam.setSurfaceVelocity( t,x0Plus,vPlus,normalPlus,Ib1,Ib2,Ib3 );
    beam.setSurfaceVelocity( t,x0Minus,vMinus,normalMinus,Ib1,Ib2,Ib3 );

    RealArray vc;
    vc = beam.velocity();  // current beam velocity DOF's

    // Project the surface velocity onto the beam and over-write beam velocity
    beam.projectSurfaceVelocityOntoBeam( t );

    const RealArray & surfaceVelocity = beam.surfaceVelocity();
    const RealArray & vcNew =  beam.velocity();  // new beam velocity DOF's

    real maxDiff =max(fabs(vcNew-surfaceVelocity)); // this should be zero 
    
    real maxErr = max(fabs(vc-surfaceVelocity));
    printF("--TBM-- projectSurfaceVelocityOntoBeam: maxDiff=%8.3e,  max-err=%8.2e\n",maxDiff,maxErr);

    // output results to the check file 
    // output to check file
    int orderOfGalerkinProjection;
    beam.getParameter( "orderOfGalerkinProjection",orderOfGalerkinProjection);
    int degreeInSpace;
    beam.getParameter("degreeInSpace",degreeInSpace);
    fPrintF(checkFile,"\\caption{tbm: test beam model: %s, CHECK VELOCITY PROJECTION, degreeInSpace=%i,"
            " orderOfGalerkinProjection=%i}\n", 
           (beamModelType==linearBeamModel ? "linear beam model" : "nonlinear beam model"),
            degreeInSpace,orderOfGalerkinProjection);

    const int numberOfComponentsToOutput=2;
    real fNorm=1.;
    fPrintF(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput);
    fPrintF(checkFile,"%i %9.2e %10.3e  %i %9.2e %10.3e  ",0,maxDiff,fNorm,1,maxErr,fNorm);
    fPrintF(checkFile,"\n");


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
  
  //Longfei 20160116: add option for spatial discretization
  aString opCommand3[] = {"Finite Element",
			  "Finite Difference",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Type:", opCommand3, opCommand3, testProblem );

  aString cmds[] = {"solve",
                    "change beam parameters",
                    "print beam parameters",
                    "check force",
                    "check velocity projection",
                    "check internal force",
                    "convergence rate",
                    "leak check",
                    "exit",
		    ""};

  int numberOfPushButtons=0;  // number of entries in cmds
  while( cmds[numberOfPushButtons]!="" ){numberOfPushButtons++;}; // 
  int numRows=(numberOfPushButtons+1)/2;
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

  textLabels[nt] = "force polynomial degree x:"; 
  sPrintF(textStrings[nt],"%g",tbm.forceDegreeX);  nt++; 

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
    // Longfei 20160116: determine which beam model at run time
    else if(answer=="Finite Element")
      {
	printF("testing FEMBeamModel\n");
	tbm.pbeam = new FEMBeamModel;
      }
    else if(answer=="Finite Difference")
      {
	printF("testing FDBeamModel\n");
	tbm.pbeam = new FDBeamModel;
      }
    
    else if( answer=="change beam parameters" )
    {
      if(tbm.pbeam==NULL)
	{
	  printF("Warning: beam model is NULL. Choose FEM or FD first");
	  continue;
	}
      
      
      if( beamModelType==linearBeamModel )
      {
	printF("Change Beam Parameters and initialize the beam model\n");
	tbm.pbeam->update(cg,gi);
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
    else if( answer=="print beam parameters" )
    {
      //Longfei 20160121: check if beam model is properly set up
      if(! tbm.checkBeamInitialization())
	continue;

      if( beamModelType==linearBeamModel )
      {
	tbm.pbeam->writeParameterSummary();
      }
      else if(  beamModelType==nonlinearBeamModel )
      {
	tbm.nlBeam.writeParameterSummary();
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
    else if( dialog.getTextValue(answer,"force polynomial degree x:","%e",tbm.forceDegreeX) ){} //
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
      //Longfei 20160121: check if beam model is properly set up
      if(! tbm.checkBeamInitialization())
	continue;
      tbm.checkForce();
    }
    else if( answer=="check internal force" )
    {
      //Longfei 20160121: check if beam model is properly set up
      if(! tbm.checkBeamInitialization())
	continue;

      tbm.checkInternalForce();
    }

    else if( answer=="check velocity projection" )
    {
      //Longfei 20160121: check if beam model is properly set up
      if(! tbm.checkBeamInitialization())
	continue;

      tbm.checkVelocityProjection();
    }

    else if( answer=="solve" )
    {
      //Longfei 20160121: check if beam model is properly set up
      if(! tbm.checkBeamInitialization())
	continue;
      
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
