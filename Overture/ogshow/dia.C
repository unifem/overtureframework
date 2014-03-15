#include "GenericGraphicsInterface.h"
#include "display.h"
#include "FileOutput.h"
#include "SplineMapping.h"
#include "GUIState.h"
#include "Overture.h"

GenericGraphicsInterface *psPointer;

void selectObject(const real & x=-1., const real & y=-1.);
void getCursor( real & x, real & y );

static void
setupDialog(DialogData & dialogSpec, real lineWidth, int plotAndExit);


#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

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

  aString nameOfOGFile;
  ps.inputFileName(nameOfOGFile, ">> Enter the name of the (old) composite grid file:", ".hdf");

  CompositeGrid cg;
  if (getFromADataBase(cg,nameOfOGFile) != 0)
  {
    throw "ERROR: Unable to open the database file";
  }
  
  cg.update(MappedGrid::THEvertex);
  

  cg.update();                // update to create usual variables

/* ---
  for( int g=0; g<cg.numberOfComponentGrids(); g++ )
  {
    cg[g].mask().display("Here is cg.mask()");
    cout << "isAllVertexCentered = " << cg[g].isAllVertexCentered() << endl;
    cout << "isAllCellCentered = " << cg[g].isAllCellCentered() << endl;
    cg[g].isCellCentered().display("cg[g].isCellCentered()");
  }
---- */

  // set up a function for contour plotting:
  Range all;
  realCompositeGridFunction u(cg,all,all,all,3), v(cg,2,all,all,all), u2, 
                            ucc(cg,all,all,all,faceRange),ucc2(cg,all,all,all,Range(0,1),faceRange);
  // u2.link(u,1);
  v=1.;
  ucc=5.;
  ucc2=3.;

  u.setName("Velocity Stuff");
  u.setName("u",0);
  u.setName("v",1);
  if( cg.numberOfDimensions()==3 )
    u.setName("w",2);
  Index I1,I2,I3;                                              // A++ Index object
  int i1,i2,i3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )  // loop over component grids
  {
    RealArray & coord = (bool)cg[grid].isAllVertexCentered() ? cg[grid].vertex() : cg[grid].center(); 

    getIndex(cg[grid].dimension(),I1,I2,I3);                   // assign I1,I2,I3 from indexRange
    if( cg.numberOfDimensions()==1 )
    {
      u[grid](I1,I2,I3,0)=sin(Pi*coord(I1,I2,I3,axis1));
      u[grid](I1,I2,I3,1)=cos(Pi*coord(I1,I2,I3,axis1));

      u[grid](I1,I2,I3,2)=1.;

      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	v[grid](0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1));
	v[grid](1,i1,i2,i3)=grid;
      }
    }
    else if( cg.numberOfDimensions()==2 )
    {
      // u[grid](I1,I2,I3,0)=1.+.00001*(
      //      sin(Pi*coord(I1,I2,I3,axis1))   // assign all interior points on this
      //     *cos(Pi*coord(I1,I2,I3,axis2))   // component grid

      u[grid](I1,I2,I3,0)=
	sin(Pi*coord(I1,I2,I3,axis1))   // assign all interior points on this
	*cos(Pi*coord(I1,I2,I3,axis2));   // component grid
      u[grid](I1,I2,I3,1)=cos(Pi*coord(I1,I2,I3,axis1))
	*sin(Pi*coord(I1,I2,I3,axis2));

      u[grid](I1,I2,I3,2)=1.;

      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	v[grid](0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1))
	  *cos(.5*Pi*coord(i1,i2,i3,axis2));
	v[grid](1,i1,i2,i3)=grid;
      }
    }
    else
    {
      u[grid](I1,I2,I3,0)=sin(.5*Pi*coord(I1,I2,I3,axis1))  
	*cos(.5*Pi*coord(I1,I2,I3,axis2))
	*cos(.5*Pi*coord(I1,I2,I3,axis3));
      u[grid](I1,I2,I3,1)=cos(.5*Pi*coord(I1,I2,I3,axis1))
	*sin(.5*Pi*coord(I1,I2,I3,axis2))
	*cos(.5*Pi*coord(I1,I2,I3,axis3));
      u[grid](I1,I2,I3,2)=cos(.5*Pi*coord(I1,I2,I3,axis1))
	*cos(.5*Pi*coord(I1,I2,I3,axis2))
	*sin(.5*Pi*coord(I1,I2,I3,axis3));
      FOR_3(i1,i2,i3,I1,I2,I3)
      {
	v[grid](0,i1,i2,i3)=sin(.5*Pi*coord(i1,i2,i3,axis1))
	  *cos(.5*Pi*coord(i1,i2,i3,axis2))
	  *cos(.5*Pi*coord(i1,i2,i3,axis3));
	v[grid](1,i1,i2,i3)=grid;
      }
    }      
 
//    u[grid](I1,I2,I3,0)=1.;
//    u[grid](I1,I2,I3,1)=2.;

//    where( cg[grid].mask()(I1,I2,I3)==0 ) 
//      u[grid](I1,I2,I3,0)=1000.;
    
  }    
    
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
  for (i=0; i<cg.numberOfComponentGrids(); i++)
  {
    sPrintF(opLabel0[i],"mapping %i", i);
    sPrintF(opCommand0[i],"mapping to plot %i", i);
  }
// the arrays of strings are terminated by an empty string
  opLabel0[cg.numberOfComponentGrids()] = "";
  opCommand0[cg.numberOfComponentGrids()] = "";

  int mapToPlot = 0, mgToPlot = 0;
  
// initial choice: element mapToPlot
  interface.addOptionMenu( "Mapping to plot", opCommand0, opLabel0, mapToPlot); 

// recycle the strings
  for (i=0; i<cg.numberOfComponentGrids(); i++)
  {
    sPrintF(opLabel0[i],"grid %i", i);
    sPrintF(opCommand0[i],"grid to plot %i", i);
  }
  opCommand0[cg.numberOfComponentGrids()] = "";
  opLabel0[cg.numberOfComponentGrids()] = "";
  
// initial choice: element mgToPlot
  interface.addOptionMenu( "Grid to plot", opCommand0, opLabel0, mgToPlot); 

  aString answer,answer2;
  aString menu[] = { "!DIA test program",
		     "contour v",
		     "contour and wait",
		     "contour a MappedGridFunction",
		     "streamLines of a MappedGridFunction",
		     "enter 2D points",
		     "plot points",
		     "plot points with colour",
		     "file output",
		     "file name test",
		     "exit",
		     "" };

// just for fun, make some window buttons and a pulldown menu
// define push buttons
  aString pbCommands[] = {"plot a mapping", "plot a grid", "clear points", "spline", 
			  "long computation", "another level", 
			  "plot points with colour", "enter 2D points", ""};
  aString pbLabels[] = {"Plot one mapping", "Plot one grid", "Clear points", "Spline", 
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
  ds.setExitCommand("close sibling 1", "Close");
// define push buttons
  aString pbCommands2[] = {"long computation", ""};
  aString pbLabels2[] = {"Start comp", ""};
  ds.setPushButtons( pbCommands2, pbLabels2, 1 ); // default is 2 rows

// bring up the interface on the screen
  ps.pushGUI( interface );

  int pickList=0, j; 
  realArray xp(100,3);
  int nPickPoints = 0;

  SplineMapping spline;
  realArray xSpline, ySpline, zSpline;

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

// add the new point to the array
	xp(nPickPoints,0) = select.x[0];
	xp(nPickPoints,1) = select.x[1];
	xp(nPickPoints,2) = select.x[2];
	nPickPoints++;
  
	if (ps.graphicsIsOn())
	{
// What if the user presses the "Clear" button in the middle of entering all coordinates?
// Then all display lists are deleted, so we would have to make a new one!
	  if (!glIsList(pickList))
	  {
	    printf("Making a new display list\n");
	    pickList = ps.generateNewDisplayList();  // get a new (unlit) display list to use
	    assert(pickList!=0);
	  }
	  else
	    glDeleteLists(pickList, 1); // clear the existing list

// draw the list
	  glNewList(pickList,GL_COMPILE);

	  real pointSize=4.;

	  glPointSize(pointSize);   
	  ps.setColour(ps.textColour);

	  glBegin(GL_POINTS);  
	  for (j=0; j<nPickPoints; j++)
	    glVertex3(xp(j,0), xp(j,1), xp(j,2));
	  glEnd();

	  glEndList(); 
	  ps.redraw();
	}
	
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
    
    if( answer(0,22)=="line width scale factor" )
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
    else if( answer=="contour" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u, psp);  // contour/surface plots
    }
    else if( answer=="contour v" )
    {
      PlotIt::contour(ps,v, Overture::defaultGraphicsParameters());  // contour/surface plots
    }
    else if( answer=="contour and wait" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u, psp);  // contour/surface plots
      // wait here
      cout << "enter answer\n";
      cin >> answer;
    }
    else if( answer=="contour cell centred")
    {
      psp.set(GI_TOP_LABEL,"Contour a Cell-Centred Grid Function");  // set title
      PlotIt::contour(ps,ucc, Overture::defaultGraphicsParameters());  // contour/surface plots
      PlotIt::contour(ps,ucc2, Overture::defaultGraphicsParameters());  // contour/surface plots
    }
    else if( answer=="plot a GridCollection" )
    {
      psp.set(GI_TOP_LABEL,"My Grid");  // set title
      ps.plot(cg, psp);   // plot the composite grid
    }
    else if( answer=="stream lines" )
    {
      psp.set(GI_TOP_LABEL,"Streamlines");  // set title
      PlotIt::streamLines(ps,u,psp);  // streamlines
    }
//                          01234567890123456789
    else if( answer(0,14)=="mapping to plot" )
    {
      int newMapToPlot;
      sScanF(&answer[15],"%i",&newMapToPlot);
      if (newMapToPlot < 0 || newMapToPlot >= cg.numberOfComponentGrids())
      {
	ps.outputString("ERROR: mapToPlot out of bounds");
      }
      else
	mapToPlot = newMapToPlot;
      
    }
//                          01234567890123456789
    else if( answer(0,11)=="grid to plot" )
    {
      int newMgToPlot;
      sScanF(&answer[12],"%i",&newMgToPlot);
      if (newMgToPlot < 0 || newMgToPlot >= cg.numberOfComponentGrids())
      {
	ps.outputString("ERROR: mgToPlot out of bounds");
      }
      else
      {
	mgToPlot = newMgToPlot;
	aString msg;
	sPrintF(msg,"Next time you plot one grid, you will see grid # %i.\n"
		"(unless you change the grid to plot before you plot the grid).", mgToPlot);
//  	ps.createMessageDialog("Oh no!", errorDialog);
//  	ps.createMessageDialog("This ought to warn you", warningDialog);
	ps.createMessageDialog(msg, informationDialog);
//	ps.createMessageDialog("I'm a message!", messageDialog);
      }
      
    }
    else if( answer=="plot a mapping" )
    {
      psp.set(GI_TOP_LABEL,"My little mapping");  // set title
      ps.plot(cg[mapToPlot].mapping().getMapping(), psp);  
      interface.setInfoLabel(mappingCounterLabel, 
			     sPrintF(buff, "You have plotted %i mappings", ++nMappingsPlotted));
      
    }
    else if( answer=="plot a grid" )
    {
      psp.set(GI_TOP_LABEL,"A (mapped) grid");  // set title
      ps.plot(cg[mgToPlot], psp);  
      interface.setInfoLabel(gridCounterLabel, 
			     sPrintF(buff, "You have plotted %i grids", ++nGridsPlotted));
    }
    else if( answer=="contour a MappedGridFunction" )
    {
      psp.set(GI_TOP_LABEL,"A mapped grid function");  // set title
      PlotIt::contour(ps,u[mgToPlot], psp);  
    }
//                          012345678901234567890123456789
    else if (answer(0,23)=="plot the object and exit")
    {
      sScanF(&answer[24],"%i", &plotAndExit);
      if (plotAndExit)
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      else
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
    }
    
    else if( answer=="streamLines of a MappedGridFunction" )
    {
      psp.set(GI_TOP_LABEL,"My stream lines");  // set title
      PlotIt::streamLines(ps,u[mgToPlot],psp);  
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
    else if( answer=="spline" )
    {
      if (nPickPoints > 1)
      {
	Range points=nPickPoints;
	xSpline.redim(points);
	ySpline.redim(points);
	zSpline.redim(points);
	xSpline(points) = xp(points,0);
	ySpline(points) = xp(points,1);
	zSpline(points) = xp(points,2);
	spline.setPoints(xSpline, ySpline, zSpline);
// plot the new spline
	ps.plot(spline, psp);
      }
      
    }
    
    else if( answer=="clear points" )
    {
      nPickPoints = 0;
      if (pickList>0) glDeleteLists(pickList, 1);
      ps.redraw();
    }
    else if( answer=="enter 2D points" )
    {
      RealArray x(100,2);
      ps.erase();
      RealArray xBound(2,3);
      xBound=0.;
      xBound(1,nullRange)=1.;
      ps.setGlobalBound(xBound);
      ps.setPlotTheBackgroundGrid(true);
      ps.setAxesDimension(2);
      ps.setPlotTheAxes(true);
      
      ps.pickPoints(x);

      ps.setPlotTheBackgroundGrid(false);
      ps.erase();
    }
    else if( answer=="plot points" || answer=="plot points with colour" )
    {
      int n=51;
      RealArray points(n,3), value(n);
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
    else if( answer=="file output" )
    {
      // ps.fileOutput(u);
      FileOutput fileOutput;
      fileOutput.update(u,ps);
    }
    else if( answer=="file name test" )
    {
      aString fn;
      ps.inputFileName(fn,"Enter filename>");
      aString buf;
      buf = "FileName=" + fn;
      ps.outputString(buf);
    }//                       01234567890123456789
    else if (answer(0,13) == "show sibling 1")
    {
      int onOff=1;
      sScanF(&answer[14],"%i", &onOff);
      if (onOff)
	ds.showSibling();
      else
	ds.hideSibling();
    }//                       01234567890123456789
    else if (answer(0,12) == "another level")
    {
      GUIState i2;
      i2.setWindowTitle("DIA test dialog");
      i2.setExitCommand("exit level 2", "Exit");

// define push buttons
      aString pbc2[] = {"do nothing", ""};
      aString pbl2[] = {"Do nothing", ""};
  
      i2.setPushButtons( pbc2, pbl2, 1 ); // default is 2 rows
  
// make a dialog sibling
      DialogData &ds2 = i2.getDialogSibling();
      ds2.setWindowTitle("Sibling 2");
      ds2.setExitCommand("close sibling 2", "Close");
// define push buttons
      aString pbCommands[] = {"long computation", ""};
      aString pbLabels[] = {"Start comp", ""};
      ds2.setPushButtons( pbCommands, pbLabels, 1 ); // default is 2 rows

// bring up the i2 on the screen
      ps.pushGUI( i2 );
      ds2.showSibling();
      for (;;)
      {
	ps.getAnswer(answer,"Another level>");
//                           01234567890123456789
	if (answer(0,11) == "exit level 2")
	{
	  break;
	}
      }
      ps.popGUI();
    }
    else if (answer == "close sibling 1")
    {
      ds.hideSibling();
// unset the toggle button on the main dialog
      interface.setToggleState( siblingToggle , 0);
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
      for (k=0; k<2001; k++)
      {
	if (k%100 == 0)
	{
          psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
          ps.plot(cg,psp);
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
  aString pdCommand0[] = {"contour", "contour cell cenetered", "stream lines", 
			  "plot a GridCollection", ""};
  aString pdLabel0[] = {"Contour", "Cell Centered Contour", "Streamlines", "Grid Collection", 
			""};
  dialogSpec.addPulldownMenu("Plot", pdCommand0, pdLabel0, GI_PUSHBUTTON);
  
  
  aString pdCommand2[] = {"Stuffed help", "Other stuffed help", "Button help", "Command file help", ""};
  aString pdLabel2[] = {"Stuff", "Other Stuff", "Buttons", "Command files", ""};
  dialogSpec.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  dialogSpec.setLastPullDownIsHelp(1);
// done defining pulldown menus  
}
