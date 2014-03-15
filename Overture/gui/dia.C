
//#include "OvertureDefine.h"
#include "GenericGraphicsInterface.h"
#include "GUIState.h"

using namespace std;
//using GUITypes::real;
GenericGraphicsInterface *psPointer;

void selectObject(const real & x=-1., const real & y=-1.);
void getCursor( real & x, real & y );

static void
setupDialog(DialogData & dialogSpec, real lineWidth, int plotAndExit);


#ifndef NO_APP
#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  
#endif
//===============================================================================================
//     Example routine demonstrating the use of the GL_GraphicsInterface Class
//
//  This example shows the use of:
//    o prompting for a menu
//    o plotting grids functions and grids
//    o reading and saving command files
//===============================================================================================


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  int plotOption=TRUE;
  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=FALSE;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `dia [noplot][file.cmd]' \n"
            "          noplot:   run without graphics \n" 
            "          file.cmd: read this command file \n";

// create a GraphicsInterface object
  psPointer = Overture::getGraphicsInterface("GUI test program", plotOption); 
  GenericGraphicsInterface & ps = *psPointer;

  // read from a command file if given
  if( commandFileName.length() > 0 )
    ps.readCommandFile(commandFileName);

  GraphicsParameters psp;               // create an object that is used to pass parameters
    
  int std_win = 0; // the default window has number 0

//  ps.setCurrentWindow(std_win);    // reset the plot focus
      
  aString buff;
// setup a GUI
  GUIState interface;

  real lineWidth = 1.0;
  int plotAndExit=1;
  if (plotAndExit)
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  else
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

  setupDialog(interface, lineWidth, plotAndExit);
// specify toggle buttons
  int siblingToggle=0;
  aString tbCommands[] = {"plot the object and exit", "show sibling 1", ""};
  aString tbLabels[] = {"Plot and exit", "more options", ""};
  int tbState[3];
  tbState[0] = plotAndExit;
  tbState[1] = 0;
  tbState[2] = 0; siblingToggle=2;
  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); // organize in 2 columns
  
// first option menu (space for 9 mappings)
  aString opCommand0[10];
  aString opLabel0[10];
  int i;

  int mapToPlot = 0, mgToPlot = 0;
  
  aString answer,answer2;
  aString menu[] = { "!DIA test program",
		     "enter 2D points",
		     "plot points",
		     "plot points with colour",
		     "file name test",
		     "exit",
		     "" };

  // *wdh& 100324 -- an an option menu
  aString opCommand1[] = {"option 1",
			  "option 2",
			  ""};
    
  aString opLabel="type:";
//  interface.addOptionMenu( "type:", opCommand1, opCommand1, 0);
  interface.addOptionMenu( opLabel, opCommand1, opCommand1, 0);


// just for fun, make some window buttons and a pulldown menu
// define push buttons
  aString pbCommands[] = {"clear points", "line", 
			  "long computation", "another level", 
			  "plot points with colour", "enter 2D points", ""};
  aString pbLabels[] = {"Clear points", "Line", 
			"Start comp", "Push GUI",
			"Plot points", "Enter 2D points", ""};

  interface.setPushButtons(pbCommands, pbLabels, 3);
  
  interface.buildPopup(menu);

// add an info label
  int nMappingsPlotted=0, nGridsPlotted=0;
  int mappingCounterLabel = interface.addInfoLabel("You have plotted 0 mappings");
  int gridCounterLabel = interface.addInfoLabel("You have plotted 0 grids");

// make a dialog sibling
  DialogData &ds = interface.getDialogSibling();
  ds.setWindowTitle("Sibling 1");
  ds.setExitCommand("long computation", "Start comp");
// define label
  ds.addInfoLabel("Some more widgets could go here");
  
//    aString pbCommands2[] = {"long computation", ""};
//    aString pbLabels2[] = {"Start comp", ""};
//    ds.setPushButtons( pbCommands2, pbLabels2, 1 ); // default is 2 rows

// bring up the interface on the screen
  ps.pushGUI( interface );

  int j, pickList = ps.generateNewDisplayList();  // get a new (unlit) display list to use

  realArray xp(100,3);
  int nPickPoints = 0;

  realArray lines;

  SelectionInfo select;
  ps.setDefaultPrompt("Dia test>");
  int retCode;
  
  for(;;)
  {
    retCode = ps.getAnswer(answer, "", select);
//    cout << answer << endl;
//                     012345678901234567890123456789  
    if (select.active == 1)
    {
      printf("A point was picked! retCode=%i\n",retCode);
      printf("Window coordinates: %e, %e\n", select.r[0], select.r[1]);
      if (select.nSelect > 0)
      {
	printf("World coordinates: %e, %e, %e\n", select.x[0], select.x[1], select.x[2]);
      }
    }
    

    if( select.nSelect )
    {
      printf("Selection retCode=%i\n", retCode);
      for (i=0; i<select.nSelect; i++)
      {
	printf("%i\n", select.selection(i,0));
      }
    }
    
    if( answer.substr(0,23)=="line width scale factor" )
    {
      real newLineWidth;
      int nRead = sScanF(&answer[23],"%g",&newLineWidth);
// need to check that the answer is ok, and if not, correct it
	  if (nRead < 1 || newLineWidth <= 0)
	  {
	    // couldn't read a real number
	    printf("ERROR: invalid input\n");
	    // write back the default value in the text string
	    sPrintF(buff, "%g", lineWidth);
	    interface.setTextLabel(0, buff); // This is text label # 0
	  }
	  else
	  {
	    lineWidth = newLineWidth;
	    // write back the value in the text string using the standard format
	    sPrintF(buff, "%g", lineWidth);
	    interface.setTextLabel(0, buff); // This is text label # 0
	    printf("Read line width: %g\n", lineWidth);

	    ps.setLineWidthScaleFactor(lineWidth);
	  }
    }
    else if (answer.substr(0,24)=="plot the object and exit")
    {
      sScanF(&answer[24],"%i", &plotAndExit);
      if (plotAndExit)
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      else
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="line" )
    {
      if (nPickPoints > 1)
      {
#ifndef NO_APP
	lines.redim(nPickPoints-1,3,2);
	Range segs=nPickPoints-1, threeD=3;
	lines(segs,threeD,0) = xp(segs,threeD);
	lines(segs,threeD,1) = xp(segs+1,threeD);
#else
	lines.resize(nPickPoints-1,3,2);
	for (int q=0; q<nPickPoints-1; q++)
	{
	  for (int ax=0; ax<3; ax++)
	  {
	    lines(q, ax, 0) = xp(q, ax);
	    lines(q, ax, 1) = xp(q+1, ax);
	  }
	}
#endif

// plot the new line segment
	ps.plotLines(lines, psp);
      }
      else
	ps.outputString("Sorry, no points have been entered");
      
    }
    
    else if( answer=="clear points" )
    {
      nPickPoints = 0;
      ps.deleteList(pickList);
      ps.redraw();
      pickList = ps.generateNewDisplayList();
    }
    else if( answer=="enter 2D points" )
    {
      ps.erase();
      RealArray xBound(2,3);
      xBound=0.;
      for (int ax=0; ax<3; ax++)
	xBound(1,ax)=1.;
      ps.setGlobalBound(xBound);
      ps.setPlotTheBackgroundGrid(true);
      ps.setAxesDimension(2);
      ps.setPlotTheAxes(true);
      
      nPickPoints = ps.pickPoints(xp);

      ps.setPlotTheBackgroundGrid(false);
    }
    else if( answer=="plot points" || answer=="plot points with colour" )
    {
      int n=51;
      //kkc 060505      RealArray points(n,3), value(n);
      realArray points(n,3), value(n);
      for( int i=0; i<n; i++ )
      {
        real radius=i/(n+1.);
        real theta=twoPi*i/(n+1.);
        points(i,axis1)=cos(theta)*radius;
        points(i,axis2)=sin(theta)*radius;
	points(i,axis3)=cos(2.*theta)*radius;
        value(i)= SQR(points(i,axis1))+SQR(points(i,axis2))+SQR(points(i,axis3));
      }
      psp.set(GI_POINT_SIZE,(real) 6.);  // size in pixels
      if( answer=="plot points" )
        ps.plotPoints(points,psp);
      else
        ps.plotPoints(points,value,psp); // colour point i by value(i)
    }
    else if( answer=="option 1" )
    {
      printF("option 1 chosen\n");
    }
    else if( answer=="option 2" )
    {
      printF("option 2 chosen\n");
    }
    else if( answer=="file name test" )
    {
      aString fn;
      ps.inputFileName(fn,"Enter filename>");
      aString buf;
      buf = "FileName=" + fn;
      ps.outputString(buf);
    }//                       01234567890123456789
    else if (answer.substr(0,14) == "show sibling 1")
    {
      int onOff=1;
      sScanF(&answer[14],"%i", &onOff);
      if (onOff)
	ds.showSibling();
      else
	ds.hideSibling();
    }//                       01234567890123456789
    else if (answer.substr(0,13) == "another level")
    {
      GUIState i2;
      i2.setWindowTitle("DIA test dialog");
      i2.setExitCommand("exit level 2", "Exit");

// define push buttons
      aString pbc2[] = {"do nothing", ""};
      aString pbl2[] = {"Do nothing", ""};
  
      i2.setPushButtons( pbc2, pbl2, 1 ); // default is 2 rows
  
// bring up the i2 on the screen
      ps.pushGUI( i2 );
      for (;;)
      {
	ps.getAnswer(answer,"Another level>");

	if (answer.find("exit level 2")!=string::npos )
	{
	  break;
	}
      }
      ps.popGUI();
    }
    else if (answer == "long computation")
    {
      GUIState cInterface;
      cInterface.setWindowTitle("Comput monitor");
      cInterface.setExitCommand("abort", "Abort");
// define push buttons
      aString cCommands[] = {"pause", ""};
      aString cLabels[] = {"Pause", ""};
      cInterface.setPushButtons( cCommands, cLabels, 1 ); // default is 2 rows
// bring up the interface on the screen
      ps.pushGUI( cInterface );
      int k;
      aString buf, answer2;
      float a[10000], b[10000], c[10000], d[10000], sc;
      for (k=0; k<20001; k++)
      {
	if (k%100 == 0)
	{
          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
//          PlotIt::plot(ps, cg, psp);
          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	  
	  ps.outputString(sPrintF(buf, "Checking events at k=%i", k));
	  ps.getAnswerNoBlock(answer2, "Monitor>");
	  if (answer2 == "abort")
	  {
	    ps.outputString("Aborting...");
	    break;
	  }
	  else if (answer2 == "pause")
	  {
	    ps.pause();
	  }
	}
// do some computation...
	int q;
	for (q=0; q<10000; q++)
	{
	  a[q] = 1+0.001*q;
	  b[q] = 0.003*q;
	  c[q] = .2*q;
	  d[q] = .1*q+k;
	}
	sc = 0;
	for (q=0; q<10000; q++)
	  sc += a[q]*b[q] + c[q]*d[q];
	
      }
      ps.outputString(sPrintF(buf, "k=%i, sc=%g", k, sc));
      ps.popGUI();
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  ps.popGUI(); // cleanup

  Overture::finish();          
  return 0;
}


static void
setupDialog(DialogData & dialogSpec, real lineWidth, int plotAndExit)
{
  int i,j;
  
  dialogSpec.setWindowTitle("DIA test dialog");
  
  dialogSpec.setExitCommand("exit", "Exit");

// text labels
  aString textCommands[] = {"line width scale factor", ""};
  aString textLabels[] = {"Line width", ""};
  aString textStrings[2];
  sPrintF(textStrings[0], "%g", lineWidth);
  textStrings[1] = "";
  dialogSpec.setTextBoxes(textCommands, textLabels, textStrings);
  
// define layout of option menus
  dialogSpec.setOptionMenuColumns(2);
  
// define pulldown menus
  aString pdCommand0[] = {"erase", ""};
  aString pdLabel0[] = {"Erase", ""};
  dialogSpec.addPulldownMenu("Plot", pdCommand0, pdLabel0, GI_PUSHBUTTON);
  
  
  aString pdCommand2[] = {"Stuffed help", "Other stuffed help", "Button help", "Command file help", ""};
  aString pdLabel2[] = {"Stuff", "Other Stuff", "Buttons", "Command files", ""};
  dialogSpec.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  dialogSpec.setLastPullDownIsHelp(1);
// done defining pulldown menus  
}
