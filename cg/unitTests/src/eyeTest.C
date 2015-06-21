//==============================================================================================
// eyeTest: plot the boundary of an eye at different times.
//
//=============================================================================================

#include "EyeCurves.h"  
#include "display.h"
#include "PlotStuff.h"
#include "NurbsMapping.h"


// ====================================================================================
/// \brief plot a curve given as a set of points x(i,0:1)
// ====================================================================================
void
plotCurve( RealArray & x, GenericGraphicsInterface & gi, PlotStuffParameters & psp )
{
  
  NurbsMapping nurbs;
  nurbs.interpolate(x);
      
  // interpolate(const RealArray & x, 
  // 	    const int & option    /* = 0 */,
  // 	    RealArray & parameterization /* =Overture::nullRealArray() */,
  //       int degree /* = 3 */,
  //       ParameterizationTypeEnum parameterizationType /* =parameterizeByChordLength */,
  //       int numberOfGhostPoints /* =0 */ )

      
  bool colourLineContours=psp.colourLineContours;
  real lineWidthSave=1.;
  psp.get(GraphicsParameters::lineWidth,lineWidthSave);
  psp.set(GraphicsParameters::lineWidth,2.);
  psp.set(GI_COLOUR_LINE_CONTOURS,true);
  
  PlotIt::plot(gi,nurbs,psp);

  // reset
  psp.set(GraphicsParameters::lineWidth,lineWidthSave);
  psp.set(GI_COLOUR_LINE_CONTOURS,colourLineContours);
}


int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  aString commandFileName="";
  bool plotOption=true;
  int debug=0;
  int numPoints=101;
  printF("Usage: eyeTest [-debug=<i>]\n");
  int len=0;
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noplot" || arg=="noplot" )
        plotOption=false;
      else if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&debug);
	printF("Setting debug=%i\n",debug);
      }
      else if( commandFileName=="" )
      {
        commandFileName=arg;    
        printf("eyeTest: reading commands from file [%s]\n",(const char*)commandFileName);
      }
    }
  }
  else
  {
    printF("Usage: `tes [-noplot] [-g=<gridName>] [file.cmd] [-debug=<value>] ' \n");
  }
  
  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("tes",plotOption,argc,argv);
  PlotStuffParameters psp;

  // By default start saving the command file called:
  aString logFile="eyeTest.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  aString outputFileName="eyeTest.log";
  FILE *outFile = NULL;
  

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =[%s].\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }

  // Create the object that knows how to evaluate the boundary of the eye-lid
  EyeCurves eyeCurves;
  
  real time=0.; // plot curve at this time
  
  // ========== create the GUI and dialog ================
  GUIState dialog;
  dialog.setWindowTitle("Eye Test Code");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"plot",
		    "movie",
                    "erase",
                    "save file",
		    "" };
  int numberOfPushButtons=0;  // number of entries in cmds
  while( cmds[numberOfPushButtons]!="" ){numberOfPushButtons++;}; // 
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "time:";  sPrintF(textStrings[nt],"%g",time);  nt++; 
  textLabels[nt] = "numPoints:";  sPrintF(textStrings[nt],"%i",numPoints);  nt++; 
  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("eyeTest>");
  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
 
  aString answer,buff;  
  for( ;; )
  {
    gi.getAnswer(answer,"");  
 
    if( answer=="continue" )
    {
      break;
    }
    else if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"time:","%g",time) ){}// 
    else if( dialog.getTextValue(answer,"numPoints:","%i",numPoints) ){}// 
    else if( answer=="erase" )
    {
      gi.erase();
    }
    else if( answer=="movie" )
    {
      // --- plot a movie of the eye motion ---

      // fix the plot bounds as the eye moves 
      RealArray xBound(2,3);
      xBound(0,0)=-1.2; xBound(1,0)= 1.2;
      xBound(0,1)=-1.2; xBound(1,1)= 1.2;
      xBound(0,2)=-1.;  xBound(1,2)= 1.;

      real dt=.005*Pi, tFinal=10*Pi;
      int nStep=int( tFinal/dt + .5 );
      dt = tFinal/(nStep);

      real yMax=-1.e10; // keep track of the largest y value of the eye-lid 

      RealArray x;
      for( int step=0; step<nStep; step++ )
      {
	real t=step*dt;
	
	eyeCurves.getEyeCurve( x,t,numPoints );

        Range R = x.dimension(0);
        real yTop = max(x(R,1));
        if( yTop>yMax )
	{
          yMax=yTop;
	}
	else if( yTop>yMax*(.99999) )
	{
          printF("Eye reaches yMax=%9.3e at t=%9.3e t/(2*pi)=%9.3e\n",yMax,t,t/twoPi);
	}
	
        gi.erase();
        psp.set(GI_TOP_LABEL,sPrintF(buff,"EyeTest: t=%9.2e, yMax=%8.2e",t,yMax));
	plotCurve( x, gi,psp );
        gi.setGlobalBound(xBound); // set plot bounds         

        gi.redraw(true);
      }

    }
    else if( answer=="plot" )
    {
      // plot the eye-lid at t=time
      RealArray x;
      eyeCurves.getEyeCurve( x,time,numPoints );
      psp.set(GI_TOP_LABEL,sPrintF(buff,"EyeTest: t=%9.2e",time));
      plotCurve( x, gi,psp );

    }
    else if( answer=="save file" )
    {
      aString fileName = "eyeCurveDataPoints.dat";
      eyeCurves.saveEyeCurve( time, numPoints, fileName );
      printF("Eye coordinates written to file=[%s]\n",(const char*)fileName);
    }
    
    else
    {
      printF("Unknown response=[%s]\n",(const char*)answer);
    }
  }
  

  gi.unAppendTheDefaultPrompt();
  gi.popGUI(); // restore the previous GUI


  Overture::finish();  
  return(0);
}
