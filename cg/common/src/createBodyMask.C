// ===========================================================================================================
// This program can be used to create a body mask for use with the cg solver.
//  
//  Examples:
//     motion motion1.cmd
//     motion motion2.cmd
// ===================================================================================

#include "Overture.h"
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"
#include "Ogshow.h"
#include "CgSolverUtil.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

int 
main(int argc, char *argv[]) 
{
  Mapping::debug=0;

  Overture::start(argc,argv);  // initialize Overture and A++/P++

  char buff[180];
  int plotOption=true;
  
  printF("Usage: createBodyMask [file.cmd] \n");

  aString commandFileName="";

  int len=0;
  if( argc > 1 )
  { // look at arguments for "-noplot"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" )
        plotOption=false;
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printF("createBodyMask: reading commands from file [%s]\n",(const char*)commandFileName);
      }

    }
  }

  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface("motion",plotOption,argc,argv);

  // By default start saving the command file called "motion.cmd"
  aString logFile="createBodyMask.cmd";
  gi.saveCommandFile(logFile);
  printF("User commands are being saved in the file [%s]\n",(const char *)logFile);

  gi.appendToTheDefaultPrompt("createBodyMask>");

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("Read command file =%s.\n",(const char*)commandFileName);
    gi.readCommandFile(commandFileName);
  }


  PlotStuffParameters gip;
    
  
  aString nameOfGridFile;
  aString showFileName="bodyMask.show";
  
  CompositeGrid cg;
  realCompositeGridFunction bodyForceMask;  // holds the mask (using reals)
  

  GUIState dialog;
  bool buildDialog=true;
  if( buildDialog )
  {
    dialog.setWindowTitle("Create a Body Mask");
    dialog.setExitCommand("exit", "exit");

    aString cmds[] = {"read grid",
                      "add a refinement",
                      "create mask",
                      "save show file",
                      "plot",
		      ""};

    int numberOfPushButtons=5;  // number of entries in cmds
    int numRows=(numberOfPushButtons+1)/2;
    dialog.setPushButtons( cmds, cmds, numRows ); 

//     aString tbCommands[] = {"check derivatives",
//                             "check grid velocity",
// 			    ""};
//     int tbState[10];
//     tbState[0] = checkDerivatives;
//     tbState[1] = checkGridVelocity;
//     int numColumns=1;
//     dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

    const int numberOfTextStrings=7;  // max number allowed
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "show file name:";  sPrintF(textStrings[nt],"%s",(const char*)showFileName);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    dialog.setTextBoxes(textLabels, textLabels, textStrings);

    // dialog.buildPopup(menu);
    gi.pushGUI(dialog);
  }
  

  aString answer, answer2;
  bool plotMask=false;
  int numberOfDimensions=2;
  
  for(;;)
  {
    gi.getAnswer(answer,"");
    if( answer=="exit" )
      break;
  
    if( answer=="read grid" )
    {
      nameOfGridFile = readOrBuildTheGrid( gi, cg );

      bodyForceMask.updateToMatchGrid(cg);
      numberOfDimensions=cg.numberOfDimensions();
      
    }
    else if( answer=="add a refinement" )
    {
      // cg.update(GridCollection::THErefinementLevel);  // indicate that we are want a refinement level

      int grid=0;
      int level=1;
      IntegerArray range(2,3), factor(3);
      range=0;
      int refinementRatio=2;
      int p0=-1, p1=-1;
      gi.inputString(answer2,"Enter grid,level,i1a,i1b,i2a,i2b,i3a,ratio,p0,p1");
      sScanF(answer2,"%i %i %i %i %i %i %i %i %i %i %i",&grid,&level,&range(0,0),&range(1,0),
       	     &range(0,1),&range(1,1),&range(0,2),&range(1,2),&refinementRatio,&p0,&p1);

      if( refinementRatio<2 || refinementRatio>1024 )
      {
	printF("ERROR: invalid refinement ratio=%i. Setting equal to 2.\n",refinementRatio);
	refinementRatio=2;
      }
      
      printF("*** refinementRatio=%i, [p0,p1]=[%i,%i] \n",refinementRatio,p0,p1);

      factor=refinementRatio;
      cg.addRefinement(range, factor, level, grid); 

      cg.update(GridCollection::THErefinementLevel);  // this seems to be needed.
      // cg.update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex);

      bodyForceMask.updateToMatchGrid(cg);
      bodyForceMask=0.;
    }
    
    else if( answer=="create mask" )
    {
      if( cg.numberOfComponentGrids()==0 )
      {
	printF("You should read in a grid before you define the mask.\n");
	continue;
      }
      
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
	

	realArray & bfMask = bodyForceMask[grid];
	OV_GET_SERIAL_ARRAY(real,bfMask,bfMaskLocal);
	OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

	Index I1,I2,I3;
	getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.
	// restrict bounds to local processor, include ghost
	bool ok = ParallelUtility::getLocalArrayBounds(bfMask,bfMaskLocal,I1,I2,I3,1);

	if( ok )
	{
	  bfMaskLocal=0.;
	  
          // Make a mask for an ellipse (for now)
	  real x0=.5, y0=.5, z0=.5;
	  real a0=.25, b0=.125, c0=.15;

	  real rad, xv[3] ={0.,0.,0.}; //
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	      xv[axis]=xLocal(i1,i2,i3,axis);

	    if( numberOfDimensions==2 )
	      rad =  SQR( (xv[0]-x0)/a0 ) + SQR( (xv[1]-y0)/b0);
            else
	      rad =  SQR( (xv[0]-x0)/a0 ) + SQR( (xv[1]-y0)/b0) + SQR( (xv[2]-z0)/c0);

//             if( rad < 1. )
// 	    {
// 	      bfMaskLocal(i1,i2,i3)=1.;
// 	    }
            // -- try this:
            bfMaskLocal(i1,i2,i3)=rad-1.;
	  }
	}
      }
      

      plotMask=true;
    }
    else if( dialog.getTextValue(answer,"show file name:","%s",showFileName) ){} // 
    else if( answer=="plot" )
    {
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      gip.set(GI_TOP_LABEL,"mask");  // set title
      gi.erase();
      PlotIt::contour(gi,bodyForceMask,gip); 
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      
    }
    else if( answer=="save show file" )
    {

      Ogshow show( showFileName );                               // create a show file
      show.saveGeneralComment("body mask created with createBodyMask"); 
      show.startFrame();  
      // show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));   // comment 0 (shown on plot)
      // show.saveComment(1,sPrintF(buffer,"  t=%e ",t));               // comment 1 (shown on plot)
      show.saveSolution( bodyForceMask ); 

      show.close();
      printF("mask saved in the show file [%s].\n",(const char*) showFileName);
    }
    else
    {
      printf("Unknown answer=[%s]\n",(const char*)answer);
      continue;
    }
    
    if( plotMask )
    {
      gip.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      gip.set(GI_TOP_LABEL,"mask");  // set title
      gi.erase();
      PlotIt::contour(gi,bodyForceMask,gip); 
      plotMask=false;
    }
    
  } // for(;;)
  
  gi.unAppendTheDefaultPrompt();  // reset prompt
  gi.popGUI(); // restore the previous GUI

  Overture::finish(); 
  return 0;
}
