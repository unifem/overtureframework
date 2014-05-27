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
#include "display.h"
// #include "App.h"


int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  printF("Usage: tbm -cmd=<command file> -noplot -nElem=<> -cfl=<> -tFinal=<> -tPlot=<> -debug=<> ... \n" );


  int debug = 1; 

  int nElem=11;    // number of elements 
  int plotOption=1; 
  int orderOfAccuracy=2;
  int addedMass=0;
  int plotBody=1;
  int testProblem=0;
  real cfl=.9, tFinal=.1, tPlot=.1;
  
  aString commandFileName="";

  char buff[180];
  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot" or "-cfl=<value>"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
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
      else if( len=line.matches("-tPlot=") )
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
    }
  }

  PlotStuff gi(plotOption,"Beam model tester");
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

  aString opCommand1[] = {"traveling wave",
			  "standing wave",
			  ""};

  dialog.setOptionMenuColumns(1);
  dialog.addOptionMenu( "Type:", opCommand1, opCommand1, testProblem );

  aString cmds[] = {"solve",
                    "convergence rate",
                    "leak check",
                    "exit",
		    ""};

  int numberOfPushButtons=3;  // number of entries in cmds
  int numRows=numberOfPushButtons; // (numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"added mass",
                          "plot body",
			  ""};
  int tbState[10];
  tbState[0] = addedMass;
  tbState[1] = plotBody;
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

  // textLabels[nt] = "mass:"; 
  // sPrintF(textStrings[nt],"%g",mass);  nt++; 

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
    else if( dialog.getTextValue(answer,"tFinal:","%e",tFinal) ){} //
    else if( dialog.getTextValue(answer,"tPlot:","%e",tPlot) ){} //
    else if( dialog.getTextValue(answer,"cfl:","%e",cfl) ){} //
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
    else if( answer=="solve" )
    {
      // ------------ Solve for the beam motion ----------------

      // trb.solve(gi);

      BeamModel bm;

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


      real momOfIntertia=1., E=1., rho=100., beamLength=1., thickness=.1, pnorm=10.,  x0=0., y0=0.;
      real breadth=1.;
      BeamModel::BoundaryCondition bcLeft=BeamModel::Pinned, bcRight=BeamModel::Pinned;
      // BeamModel::BoundaryCondition bcLeft=BeamModel::Periodic, bcRight=BeamModel::Periodic;
      bool useExactSolution=false;
      bm.setParameters(momOfIntertia, E, 
		       rho,beamLength,
		       thickness,pnorm,
		       nElem, bcLeft,bcRight,
		       x0, y0, useExactSolution);
      
      // bm.setDeclination(20.*twoPi/360.);

      RealArray x1,v1,x2,v2,x3,v3;
      x1 = bm.position(); 
      v1 = bm.velocity();

      x2 = bm.position(); 
      v2 = bm.velocity();

      x3 = bm.position(); 
      v3 = bm.velocity();

      // --- parameters for the exact solution ---
      real a=.1;  // amplitude 
      real k0=1.;
      real k=2.*Pi*k0;
      real w = sqrt( E*momOfIntertia*pow(k,4)/( rho*thickness*breadth ) );

#define W1(x,t) (a*sin(k*xl - w*t))
#define W1t(x,t) (-a*w*cos(k*xl - w*t))
#define W1x(x,t) (-k*a*cos(k*xl - w*t))
#define W1tx(x,t) (a*k*w*sin(k*xl - w*t))

// standing wave: 
#define W(x,t) (a*sin(k*(x))*cos(w*(t)))
#define Wt(x,t) (-w*a*sin(k*(x))*sin(w*(t)))
#define Wx(x,t) (w*a*cos(k*(x))*cos(w*(t)))
#define Wtx(x,t) (-w*a*cos(k*(x))*sin(w*(t)))


      // wave speed c= w/k ,   c*dt/dx = cfl 
      real dx=beamLength/nElem;
      real dt= cfl*dx/(w/k); 
      int numberOfSteps= int( tFinal/dt + .5);
      dt = tFinal/numberOfSteps;  // adjust dt so we reach tFinal exactly
      
      real t=0.;
   


      for (int i = 0; i <= nElem; ++i)
      {
        real xl = ( (real)i /nElem) *  beamLength;

        t=-dt;

	x1(i*2)   = W(xl,t);     // w 
	x1(i*2+1) = Wx(xl,t);    // w_x
    
	v1(i*2)   = Wt(xl,t);    // w_t 
	v1(i*2+1) = Wtx(xl,t);   // w_xt 

        t=0.;
	
	x2(i*2)   = W(xl,t);     // w 
	x2(i*2+1) = Wx(xl,t);    // w_x
    
	v2(i*2)   = Wt(xl,t);    // w_t 
	v2(i*2+1) = Wtx(xl,t);   // w_xt 

      }

      aString cNames[2]={"w","we"};  // 
      psp.componentsToPlot.redim(2);
      psp.componentsToPlot(0)=0;
      psp.componentsToPlot(1)=1;
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);     // set this to run in "movie" mode (after first plot)
      
      for( int step=1; step<=numberOfSteps; step++ )
      {

	bm.predictor(dt, x1,v1,x2,v2,x3,v3);

	bm.corrector(dt, x3,v3);

        t = (step)*dt;
	
        // compute the max error:
	if( (step % 1) == 0 )
	{
	  real errMax=0.;
	  for (int i = 0; i <= nElem; ++i)
	  {
	    real xl = ( (real)i /nElem) *  beamLength;
	    real we = W(xl,t);

	    real err = fabs( x3(i*2)-we );
	    errMax=max(errMax,err);
	  }
	  printF("Max error t=%9.3e : %8.2e\n",t,errMax);

          RealArray x(nElem+1), u(nElem+1,2);
	  for (int i = 0; i <= nElem; ++i)
	  {
	    real xl = ( (real)i /nElem) *  beamLength;
            x(i)=xl;
	    u(i,0)=x3(i*2);
            u(i,1)=W(xl,t);
	  }
	  
          gi.erase();
          // -- plot points to set plot bounds : fix me ---
          RealArray points(2,2);
	  points(0,0)=0.; points(0,1)=-a;
	  points(1,0)=beamLength; points(1,1)=a;
	  gi.plotPoints(points,psp);
	  psp.set(GI_USE_PLOT_BOUNDS,true);
	  PlotIt::plot(gi,x,u,"w, we","x",cNames,psp);


          gi.redraw(true);
	  usleep(100000);  // sleep in mirco-seconds
	  
	  // plot(GenericGraphicsInterface &gi, 
	  //      const realArray & t, 
	  //      const realArray & x, 
	  //      const aString & title = nullString, 
	  //      const aString & tName       = nullString,
	  //      const aString *xName        = NULL,
	  //      GraphicsParameters & parameters=Overture::defaultGraphicsParameters()  );




	}
	
        x1=x2; v1=v2;
        x2=x3; v2=v3;
	
      }
      



      if( false )
      {
	const RealArray & xBeam = bm.position();  // (x1,x2) coordinates of the beam
	const RealArray & vBeam = bm.velocity();  // (v1,v2) components of the velocity of the beam
      
	::display(xBeam,"xBeam","%5.2f ");
	::display(vBeam,"vBeam","%5.2f ");
      }
      




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
