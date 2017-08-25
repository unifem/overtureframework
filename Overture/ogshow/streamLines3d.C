#include "Overture.h"
#include "GL_GraphicsInterface.h"
#include "ParallelUtility.h"
#include "interpPoints.h"
#include "PlotIt.h"
#include "InterpolatePoints.h"


#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define XSCALE(x) (parameters.xScaleFactor*(x))
#define YSCALE(y) (parameters.yScaleFactor*(y))
#define ZSCALE(z) (parameters.zScaleFactor*(z))

// These variables are local to this file.

static int nrsmx;
static real cfl=.5,uMax,uMin,xa,xb,ya,yb,za,zb,xba,yba,zba;

//==================================================================
//    Stream-Line Plots of a Pair of Composite Grid Functions
//    -------------------------------------------------------
// PURPOSE
// INPUT:
// 
//  Remarks:
//   (1) The streamlines are coloured by the relative value of u**2+v**2
//   (2) Streamlines that move too slowly are stopped  
//   (3) There is a maximum number of steps used to integrate any streamline.
//================================================================
void PlotIt::
streamLines3d(GenericGraphicsInterface &gi, GridCollection & gc, 
              const realGridCollectionFunction & uv, 
	      GraphicsParameters & parameters)
{
  int grid;
  const int numberOfGrids = gc.numberOfComponentGrids();

// save the current window number 
  int startWindow = gi.getCurrentWindow();

  int maximumNumberOfSteps=10000;

  if( !parameters.dbase.has_key("tracerPointSize")  ) // *wdh* AUg 24, 2017
  {
    parameters.dbase.put<real>("tracerPointSize")=4.;
  }
  
  real & tracerPointSize = parameters.dbase.get<real>("tracerPointSize"); 

  IntegerArray componentsToInterpolate;
  componentsToInterpolate.resize(3); 

  char buff[160];
  aString answer,answer2;
  aString menu[] = {"!streamline plotter (3d)",
                   "erase and exit",
                   "plot",
                   ">choose seeds",
                   "specify starting points",
                   "specify points on a line",
                   "specify points on a rectangle",
                   "<choose first velocity component",
                   "choose second velocity component",
                   "choose third velocity component",
                   "set min and max",
                   "choose new plot bounds",
//                   "reset plot bounds",
                   "set stream line stopping tolerance",
//                   "colour stream lines (toggle)",
                   "plot the grid",
//                   "plot labels (toggle)",
//                   "plot colour bar (toggle)",
                    ">colour table choices",
                    "rainbow",
                    "gray",
                    "red",
                    "green",
                    "blue",
                   "<plot ghost lines",
                   " ",
//                   "plot the axes (toggle)",
//                   "plot the back ground grid (toggle)",
                   "set origin for axes",
                   "erase",
                   "erase and exit",
                   "exit this menu",
                   "" };


  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  // bool colourStreamLines=true;
  enum StreamLineColourOptionsEnum
    {
      colourBySpeed=0,
      colourByNumber,
      colourBlack
    } streamLineColourOption=colourBySpeed;
  
  // *************Dialog interface ********************************************
  DialogData *interface=NULL;  // could be passed in the future
  
  GUIState gui;
  gui.setWindowTitle("3D Stream Line Plotter");
  gui.setExitCommand("exit", "exit");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  RealArray xBound(2,3);
  getPlotBounds(gc,psp,xBound);

  // Guess the position of an initial rake line:
  real rxa,rya,rza,rxb,ryb,rzb;

  rxa=xBound(0,0)+.1*(xBound(1,0)-xBound(0,0));
  rxb=rxa;

  rya=xBound(0,1)+.1*(xBound(1,1)-xBound(0,1));
  ryb=xBound(1,1)-.1*(xBound(1,1)-xBound(0,1));
  
  rza=.5*(xBound(0,2)+xBound(1,2));
  rzb=rza;

  if( !psp.plotObjectAndExit )
  {
    printf("----------------------------------------------------------------------------------------\n"
           "The 3D stream line plotter can display particle trajectories.\n"
           "-----------------------------------------------------------------------------------------\n");

//      aString *componentCmd = new aString [ numberOfComponents+1 ];
//      aString *componentLabel = new aString [ numberOfComponents+1 ];
//      for( int i=0; i<numberOfComponents; i++ )
//      {
//        componentLabel[i]=uGCF.getName(uGCF.getComponentBase(0)+i);
//        if( componentLabel[i] == "" || componentLabel[i]==" " )
//  	componentLabel[i]=sPrintF(buff,"component%i",uGCF.getComponentBase(0)+i);
//        componentCmd[i] = sPrintF(buff,"component %i",uGCF.getComponentBase(0)+i);
//      }
//      componentCmd[numberOfComponents]="";
//      componentLabel[numberOfComponents]="";

//      dialog.addOptionMenu("component:", componentCmd,componentLabel,psp.componentForContours);
//      delete [] componentCmd;
//      delete [] componentLabel;


    aString colourLabel[] = {"colour by speed", "colour by number", "colour black", "" };
    dialog.addOptionMenu("Colour:",colourLabel,colourLabel,(int)streamLineColourOption );

//     aString tbCommands[] = {"colour stream lines",
//   			    ""};
//     int tbState[10];
//     tbState[0] = colourStreamLines==true;
//     int numColumns=2;
//     dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

    aString pbLabels[] = { 
                           "compute tracers",
			   "plot",
                           "plot the grid",
                           "reset plot bounds",
			   "erase",
			   "erase and exit",
			   ""};
    int numRows=3;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 
    
    const int numberOfTextStrings=7;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];


    int nt=0;
    textLabels[nt] = "rake line A"; 
    sPrintF(textStrings[nt], "%i, %i, %.2g,%.2g,%.2g, %.2g,%.2g,%.2g (id, num, xa,ya,za, xb,yb,zb)",
          0,11,rxa,rya,rza,rxb,ryb,rzb);
    nt++;

    rxa=xBound(0,0)+.1*(xBound(1,0)-xBound(0,0));
    rxb=rxa;
    rza=xBound(0,2)+.1*(xBound(1,2)-xBound(0,2));
    rzb=xBound(1,2)-.1*(xBound(1,2)-xBound(0,2));
    rya=.5*(xBound(0,1)+xBound(1,1));
    ryb=rya;
    textLabels[nt] = "rake line B"; 
    sPrintF(textStrings[nt], "%i, %i, %.2g,%.2g,%.2g, %.2g,%.2g,%.2g (id, num, xa,ya,za, xb,yb,zb)",
          1,11,rxa,rya,rza,rxb,ryb,rzb);
    nt++;

    textLabels[nt] = "cfl"; 
    sPrintF(textStrings[nt], "%g ",cfl);
    nt++;

//      textLabels[nt] = "seed point"; 
//      sPrintF(textStrings[nt], "%.2g %.2g %.2g",xa,ya,za);
//      nt++;

    textLabels[nt] = "max number of steps"; 
    sPrintF(textStrings[nt], "%i",maximumNumberOfSteps);
    nt++;

    textLabels[nt] = "xScale, yScale, zScale";
    sPrintF(textStrings[nt], "%g %g %g",psp.xScaleFactor,psp.yScaleFactor,psp.zScaleFactor);
    nt++;

    textLabels[nt] = "point size";
    sPrintF(textStrings[nt], "%g",tracerPointSize);
    nt++;

    textLabels[nt]=""; 
  
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


    gui.buildPopup(menu);
    gi.pushGUI( gui );
  }
  // **********************************************************************************


  int & uComponent                    = psp.uComponentForStreamLines;
  int & vComponent                    = psp.vComponentForStreamLines;
  int & wComponent                    = psp.wComponentForStreamLines;

  // bool & plotTitleLabels              = psp.plotTitleLabels;
  // bool & plotColourBar                = psp.plotColourBar;
  IntegerArray & backGroundGridDimension = psp.backGroundGridDimension;
  RealArray & plotBound               = psp.plotBound;
  int & numberOfGhostLinesToPlot      = psp.numberOfGhostLinesToPlot;
  //  if( numberOfGhostLinesToPlot> 0 )
  //    setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot); // set values
  
  real & minStreamLine                = psp.minStreamLine;
  real & maxStreamLine                = psp.maxStreamLine;
  bool & minAndMaxStreamLinesSpecified= psp.minAndMaxStreamLinesSpecified;
  real & streamLineStoppingTolerance  = psp.streamLineStoppingTolerance;
  int & numberOfStreamLineStartingPoints= psp.numberOfStreamLineStartingPoints;

  realArray & streamLineStartingPoint   = psp.streamLineStartingPoint;
  IntegerArray & numberOfRakePoints     = psp.numberOfRakePoints;
  realArray & rakeEndPoint              = psp.rakeEndPoint;
  IntegerArray & numberOfRectanglePoints= psp.numberOfRectanglePoints;
  realArray & rectanglePoint            = psp.rectanglePoint;

  bool plotStreamLines=true;
  
  uMin=0.;  uMax=0.;


  int numberOfSeeds=0;  // total number of starting points
  RealArray seed;     // holds all the starting points in an array
  IntegerArray seedID;    // unique ID for each seed (for colour by number)
  RealArray uInterpolated, speed, dt, x[2];
//  intArray indexGuess, interpoleeGrid, wasInterpolated, mask;
  bool recomputeVelocityMinMax=TRUE;  // TRUE if we need to compute the max, min of the velocity
  bool seedsHaveChanged=TRUE;  // TRUE if the initial points for the streamlines have changed
  
  gi.setKeepAspectRatio(true); 

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    backGroundGridDimension=0;  // this means choose values below
    uComponent = uv.getComponentBase(0)-1;
    vComponent = uComponent+1;
    wComponent = vComponent+1;
  }
  // Try to guess which components are "u" and "v"
  if( uComponent<uv.getComponentBase(0) || vComponent<uv.getComponentBase(0) )
  {
    uComponent = min(uv.getComponentBound(0),max(uv.getComponentBase(0),uComponent));
    vComponent = min(uv.getComponentBound(0),uComponent+1);
    wComponent = min(uv.getComponentBound(0),uComponent+2);
    int n;
    for( n=uv.getComponentBase(0); n<=uv.getComponentBound(0); n++ )
    {
      if( uv.getName(n)[0]=='u' )
      {
        uComponent=n;
        break;
      }
    }
    for( n=uv.getComponentBase(0); n<=uv.getComponentBound(0); n++ )
    {
      if( uv.getName(n)[0]=='v' )
      {
        vComponent=n;
        break;
      }
    }      
    for( n=uv.getComponentBase(0); n<=uv.getComponentBound(0); n++ )
    {
      if( uv.getName(n)[0]=='w' )
      {
        wComponent=n;
        break;
      }
    }      
  }

  int list=0;
  if( gi.isGraphicsWindowOpen() )
  {
    list=gi.generateNewDisplayList();  // get a new display list to use
    assert(list!=0);
  }

  // get Bounds on the grids
  int i;
  Index I1,I2,I3;

  if(plotBound(End,axis1) < plotBound(Start,axis1) )  // assign plot bounds if they have not been assigned
    plotBound=xBound;

  // set default prompt
  gi.appendToTheDefaultPrompt("streamLines>");
  int len=0;
  
  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
      gi.getAnswer(answer, "");

// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( answer=="exit this menu" || answer=="exit" )
    {
      break;
    }
    else if( answer=="erase" || answer=="erase and exit" )
    {
      plotObject=false;
      glDeleteLists(list,1);
      gi.redraw();
      if( answer=="erase and exit" )
       break;
    }
    else if( dialog.getTextValue(answer,"point size","%e",tracerPointSize) ){} // 

    else if( answer=="compute tracers" )
    {
      plotStreamLines=true;
      seedsHaveChanged=true;
    }
    else if( (len=answer.matches("max number of steps")) )
    {
      sScanF(answer(len,answer.length()-1),"%i",&maximumNumberOfSteps);

      dialog.setTextLabel("max number of steps",sPrintF(answer2,"%i",maximumNumberOfSteps));
      seedsHaveChanged=true;  // recompute
    }
    else if( (len=answer.matches("xScale, yScale, zScale")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&psp.xScaleFactor,&psp.yScaleFactor,&psp.zScaleFactor);
      printF("New values are xScale = %g, yScale = %g, zScale = %g \n",psp.xScaleFactor,psp.yScaleFactor,
              psp.zScaleFactor);
      dialog.setTextLabel("xScale, yScale, zScale",sPrintF(answer,"%g %g %g",psp.xScaleFactor,
                          psp.yScaleFactor,psp.zScaleFactor));
    }
    else if( answer=="set origin for axes" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter origin for axes (x,y,z) (enter return for default)")); 
      real xo, yo, zo;
      xo = yo =zo = GenericGraphicsInterface::defaultOrigin;
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e %e %e",&xo, &yo, &zo);
	gi.setAxesOrigin(xo, yo, zo);
      }
    }
    else if(answer=="choose new plot bounds")
    {
      sPrintF(buff,"Current values: xMin=%e, xMax=%e, yMin=%e, yMax=%e",
	   	   plotBound(Start,0),plotBound(End,0),plotBound(Start,1),plotBound(End,1));
      gi.outputString(buff);
      // outputString("Enter plot bounds: xMin xMax yMin yMax");
      gi.inputString(answer2,"Enter plot bounds: xMin xMax yMin yMax");
      // cout << "answer2 = " << answer2 << endl;
      for( i=0; i<answer2.length(); i++)
	buff[i]=answer2[i];
      // sScanF(answer2,"%e %e %e %e",&xa,&xb,&ya,&yb);
      sScanF(buff,"%e %e %e %e",&xa,&xb,&ya,&yb);
      // printf("xa=%e, xb=%e, ya=%e, yb=%e \n",xa,xb,ya,yb);
      plotBound(Start,0)=xa; plotBound(End,0)=xb; plotBound(Start,1)=ya; plotBound(End,1)=yb;
      // plotBound(Start,0)+=.5*(plotBound(End,0)-plotBound(Start,0));
      // plotBound(Start,1)+=.5*(plotBound(End,1)-plotBound(Start,1));
      recomputeVelocityMinMax=TRUE;
    }
    else if(answer=="reset plot bounds")
    {
      plotBound=xBound;
      recomputeVelocityMinMax=TRUE;
    }
    else if( answer=="choose first velocity component" )
    { // Make a menu with the component names. If there are no names then use the component numbers
      int numberOfComponents=uv.getComponentDimension(0);
      aString *menu2 = new aString[numberOfComponents+1];
      for( int i=0; i<numberOfComponents; i++ )
      {
        menu2[i]=uv.getName(uv.getComponentBase(0)+i);
        if( menu2[i] == "" || menu2[i]==" " )
        {
          sPrintF(buff,"component%i",uv.getComponentBase(0)+i);
          menu2[i]=buff;
	}
      }
      menu2[numberOfComponents]="";   // null string terminates the menu
// AP: getMenuItem is a place where the user could change the window focus
      uComponent = gi.getMenuItem(menu2,answer2); 
// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
      if (gi.getCurrentWindow() != startWindow)
	gi.setCurrentWindow(startWindow);

      uComponent+=uv.getComponentBase(0);
      delete [] menu2;
      recomputeVelocityMinMax=TRUE;
    }
    else if( answer=="choose second velocity component" )
    { // Make a menu with the component names. If there are no names then use the component numbers
      int numberOfComponents=uv.getComponentDimension(0);
      aString *menu2 = new aString[numberOfComponents+1];
      for( int i=0; i<numberOfComponents; i++ )
      {
        menu2[i]=uv.getName(uv.getComponentBase(0)+i);
        if( menu2[i] == "" || menu2[i]==" " )
	{
          sPrintF(buff,"component%i",uv.getComponentBase(0)+i);
          menu2[i]=buff;
	}
      }
      menu2[numberOfComponents]="";   // null string terminates the menu
      // AP: getMenuItem is a place where the user could change the window focus
      vComponent = gi.getMenuItem(menu2,answer2);
      // make sure that the currentWindow is the same as startWindow! (It might get changed 
      // interactively by the user)
      if (gi.getCurrentWindow() != startWindow)
	gi.setCurrentWindow(startWindow);

      vComponent+=uv.getComponentBase(0);
      delete [] menu2;
      recomputeVelocityMinMax=TRUE;
    }
    else if( answer=="choose third velocity component" )
    { // Make a menu with the component names. If there are no names then use the component numbers
      int numberOfComponents=uv.getComponentDimension(0);
      aString *menu2 = new aString[numberOfComponents+1];
      for( int i=0; i<numberOfComponents; i++ )
      {
        menu2[i]=uv.getName(uv.getComponentBase(0)+i);
        if( menu2[i] == "" || menu2[i]==" " )
	{
          sPrintF(buff,"component%i",uv.getComponentBase(0)+i);
          menu2[i]=buff;
	}
      }
      menu2[numberOfComponents]="";   // null string terminates the menu
      // AP: getMenuItem is a place where the user could change the window focus
      wComponent = gi.getMenuItem(menu2,answer2);
      // make sure that the currentWindow is the same as startWindow! (It might get changed 
      // interactively by the user)
      if (gi.getCurrentWindow() != startWindow)
	gi.setCurrentWindow(startWindow);

      wComponent+=uv.getComponentBase(0);
      delete [] menu2;
      recomputeVelocityMinMax=TRUE;
    }
    else if( answer=="set min and max" )
    {
      gi.outputString("Min and max stream line values only change how the stream lines are coloured");
      gi.outputString("(Enter a blank line to use actual max/min values computed from the grid function)");
      gi.inputString(answer2,sPrintF(buff,"Enter the min and max stream lines (sqrt(u^2+v^2)), (current=%e,%e)",
                   uMin,uMax )); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e %e",&minStreamLine,&maxStreamLine);
        printf("New values are min = %e, max = %e \n",minStreamLine,maxStreamLine);
        minAndMaxStreamLinesSpecified=TRUE;
        uMin=minStreamLine;
	uMax=maxStreamLine;
      }
      else
        minAndMaxStreamLinesSpecified=FALSE;
    }
    else if( answer=="set stream line stopping tolerance" )
    {
      gi.outputString("Stream line stopping tolerance : lines are drawn until the velocity decreases by this factor");
      gi.inputString(answer2,sPrintF(buff,"Enter stopping tolerance for stream lines, (current=%e)",
                   streamLineStoppingTolerance )); 
      if( answer2 !="" && answer2!=" ")
	sScanF(answer2,"%e",&streamLineStoppingTolerance);
    }
    else if( answer=="colour by speed" ||
             answer=="colour by number" ||
             answer=="colour black" )
    {
      streamLineColourOption = (answer=="colour by speed" ? colourBySpeed :
                                answer=="colour by number" ? colourByNumber : colourBlack);
      dialog.getOptionMenu("Colour:").setCurrentChoice((int)streamLineColourOption);
    }
    else if( answer=="plot ghost lines" )
    {
//      setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot,1);  // reset
      gi.inputString(answer2,sPrintF(buff,"Enter the number of ghost lines (or cells) to plot (current=%i)",
             numberOfGhostLinesToPlot)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF(buff,"Plot %i ghost lines\n",numberOfGhostLinesToPlot));
      }
//      setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot);
      
    }
    else if( answer=="specify starting points" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the number points (current=%i)",
             numberOfStreamLineStartingPoints)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i ",&numberOfStreamLineStartingPoints);
        if( numberOfStreamLineStartingPoints>0 )
	{
	  streamLineStartingPoint.redim(numberOfStreamLineStartingPoints,3);
	  for(int i=0; i<numberOfStreamLineStartingPoints; i++)
	  {
	    gi.inputString(answer2,sPrintF(buff,"Enter point %i\n",i));
	    sScanF(answer2,"%e %e %e",&streamLineStartingPoint(i,0),&streamLineStartingPoint(i,1),
		   &streamLineStartingPoint(i,2));
	  }
	}
      }
      seedsHaveChanged=TRUE; 
    }
    else if( (len=answer.matches("cfl")) )
    {
      sScanF(answer(len,answer.length()-1),"%e",&cfl);
      dialog.setTextLabel("cfl",sPrintF(answer2,"%g",cfl));
      
    }
    else if( (len=answer.matches("rake line A")) ||
             (len=answer.matches("rake line B")) )
    {
      // new way
      int rake=0, num=10;
      aString name=answer(0,len-1);

      sScanF(answer(len,answer.length()-1),"%i %i",&rake,&num);
      if( rake>=0 )
      {
	if( rake >= numberOfRakePoints.getLength(0) )
	{ // allocate more space
	  int oldNumber=numberOfRakePoints.getLength(0);
	  int newNumber=rake+5;
	  numberOfRakePoints.resize(newNumber);  numberOfRakePoints(Range(oldNumber,newNumber-1))=0;
	  rakeEndPoint.resize(2,3,newNumber);
	}
	numberOfRakePoints(rake)=num;
	if( num>0 )
	{
	  sScanF(answer(len,answer.length()-1),"%i %i %e %e %e %e %e %e",&rake,&num,
		 &rakeEndPoint(0,0,rake),&rakeEndPoint(0,1,rake),&rakeEndPoint(0,2,rake),
		 &rakeEndPoint(1,0,rake),&rakeEndPoint(1,1,rake),&rakeEndPoint(1,2,rake));
	}
        printf("Setting rake line ID=%i with %i points from (%8.2e,%8.2e,%8.2e) to (%8.2e,%8.2e,%8.2e)\n",
	       rake,num,
               rakeEndPoint(0,0,rake),rakeEndPoint(0,1,rake),rakeEndPoint(0,2,rake),
	       rakeEndPoint(1,0,rake),rakeEndPoint(1,1,rake),rakeEndPoint(1,2,rake));

        dialog.setTextLabel(name,sPrintF(answer2, "%i, %i, %.2g,%.2g,%.2g, %.2g,%.2g,%.2g "
                    "(id, num, xa,ya,za, xb,yb,zb)",rake,num,
                     rakeEndPoint(0,0,rake),rakeEndPoint(0,1,rake),rakeEndPoint(0,2,rake),
		     rakeEndPoint(1,0,rake),rakeEndPoint(1,1,rake),rakeEndPoint(1,2,rake)));

      }
      else
      {
      dialog.setTextLabel("rake line",sPrintF(answer2, "%i, %i, %.2g,%.2g,%.2g, %.2g,%.2g,%.2g "
                    "(id, num, xa,ya,za, xb,yb,zb)",0,11,rxa,rya,rza,rxb,ryb,rzb));
      }
      seedsHaveChanged=TRUE; 

    }
    else if( answer=="specify points on a line" )
    {
      int rake=0, num=10;
      gi.inputString(answer2,sPrintF(buff,"Enter a line ID (0,1,...)(default=%i) and the number points (default=%i)",
             rake,num)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i %i",&rake,&num);  
        if( rake>=0 )
	{
	  if( rake >= numberOfRakePoints.getLength(0) )
	  { // allocate more space
            int oldNumber=numberOfRakePoints.getLength(0);
            int newNumber=rake+5;
	    numberOfRakePoints.resize(newNumber);  numberOfRakePoints(Range(oldNumber,newNumber-1))=0;
	    rakeEndPoint.resize(2,3,newNumber);
	  }
	  numberOfRakePoints(rake)=num;
          if( num>0 )
	  {
	    gi.inputString(answer2,sPrintF(buff,"Enter start and end points: x0,y0,z0, x1,y1,z1  (6 values)\n"));
	    sScanF(answer2,"%e %e %e %e %e %e",
		   &rakeEndPoint(0,0,rake),&rakeEndPoint(0,1,rake),&rakeEndPoint(0,2,rake),
		   &rakeEndPoint(1,0,rake),&rakeEndPoint(1,1,rake),&rakeEndPoint(1,2,rake));
	  }
	}
      }
      seedsHaveChanged=TRUE; 
    }
    else if( answer=="specify points on a rectangle" )
    {
      // by default a rectangle has 5x5 points on it:
      int rectangle=0, num0=5, num1=5;
      gi.inputString(answer2,sPrintF(buff,"Enter a rectangle ID (0,1,...)(default=%i) and the number points (default=%i, %i)",
             rectangle,num0,num1)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i %i %i",&rectangle,&num0,&num1);  
        if( rectangle>=0 )
	{
	  if( rectangle >= numberOfRectanglePoints.getLength(0) )
	  { // allocate more space, copy old values
            int oldNumber=numberOfRectanglePoints.getLength(0);
            int newNumber=rectangle+5;
	    numberOfRectanglePoints.resize(newNumber,2);  
            numberOfRectanglePoints(Range(oldNumber,newNumber-1),Range(0,1))=0;
	    rectanglePoint.resize(2,3,2,newNumber);
	  }
	  numberOfRectanglePoints(rectangle,0)=num0;
	  numberOfRectanglePoints(rectangle,1)=num1;
          if( num0>0 && num1>0 )
	  {
//          rectanglePoint(Start,R3,0,i)= 0;  // bottom left
//          rectanglePoint(End  ,R3,0,i)= 0;  // bottom right
//          rectanglePoint(Start,R3,1,i)= 0;  // top left
//          rectanglePoint(End  ,R3,1,i)= 0;  // top right
//
	    gi.inputString(answer2,sPrintF(buff,"Enter the bottom left point of the rectangle: x,y,z \n"));
	    sScanF(answer2,"%e %e %e",
		   &rectanglePoint(0,0,0,rectangle),&rectanglePoint(0,1,0,rectangle),&rectanglePoint(0,2,0,rectangle));

	    gi.inputString(answer2,sPrintF(buff,"Enter the bottom right point of the rectangle: x,y,z \n"));
	    sScanF(answer2,"%e %e %e",
		   &rectanglePoint(1,0,0,rectangle),&rectanglePoint(1,1,0,rectangle),&rectanglePoint(1,2,0,rectangle));
	    gi.inputString(answer2,sPrintF(buff,"Enter the top left point of the rectangle: x,y,z \n"));
	    sScanF(answer2,"%e %e %e",
		   &rectanglePoint(0,0,1,rectangle),&rectanglePoint(0,1,1,rectangle),&rectanglePoint(0,2,1,rectangle));
	    gi.inputString(answer2,sPrintF(buff,"Enter the top right point of the rectangle: x,y,z \n"));
	    sScanF(answer2,"%e %e %e",
		   &rectanglePoint(1,0,1,rectangle),&rectanglePoint(1,1,1,rectangle),&rectanglePoint(1,2,1,rectangle));



	  }
	}
      }
      seedsHaveChanged=TRUE; 
    }
    else if( answer=="plot the grid" )
    {
      plot(gi, (GridCollection&)gc, psp);
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else if( answer=="rainbow" || answer=="gray" || answer=="red" || answer=="green" || answer=="blue" )
    {
      int ct;
      if( answer=="rainbow" )
        ct=GraphicsParameters::rainbow;
      else if( answer=="gray" )
        ct=GraphicsParameters::gray;
      else if( answer=="red" )
        ct=GraphicsParameters::red;
      else if( answer=="green" )
        ct=GraphicsParameters::green;
      else if( answer=="blue" )
        ct=GraphicsParameters::blue;
  
      psp.colourTable = GraphicsParameters::ColourTables(max(min(ct,GraphicsParameters::numberOfColourTables),0));
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
    }
    if( plotObject )
    {
      gi.setAxesDimension( gc.numberOfDimensions() );
  
      if( uComponent<uv.getComponentBase(0) || uComponent>uv.getComponentBound(0))
      {
	cout << "GL_GraphicsInterface::streamLines:ERROR Invalid component for u : " << uComponent << endl;
        cout << "You must choose a valid component for u before the stream lines can be drawn\n";
        plotObject=FALSE;
      }
      if( vComponent<uv.getComponentBase(0) || vComponent>uv.getComponentBound(0))
      {
	cout << "GL_GraphicsInterface::streamLines:ERROR Invalid component for v : " << vComponent << endl;
        cout << "You must choose a valid component for v before the stream lines can be drawn\n";
        plotObject=FALSE;
      }
    }
    if( plotObject  && gi.isGraphicsWindowOpen() )
    {
      // printf(" uv.getName = %s \n",(const char*)uv.getName());
      // printf(" uv[0].getName = %s \n",(const char*)uv[0].getName());
      // printf(" uv.getName(0) = %s \n",(const char*)uv.getName(0));
      // printf(" uv[0].getName(0) = %s \n",(const char*)uv[0].getName(0));
      
      if( plotStreamLines )
      {
        real time0=getCPU();
	real timeToInterpolate=0.;
	
	if( recomputeVelocityMinMax )
	{
	  recomputeVelocityMinMax=FALSE;
	  xa=plotBound(Start,axis1); xb=plotBound(End,axis1); xba=xb-xa;
	  ya=plotBound(Start,axis2); yb=plotBound(End,axis2); yba=yb-ya;
	  za=plotBound(Start,axis3); zb=plotBound(End,axis3); zba=zb-za;
	  nrsmx=0;
	  Range Axes(0,gc.numberOfDimensions()-1);
	  for( grid=0; grid<numberOfGrids; grid++)
	    nrsmx=max(nrsmx,max(gc[grid].gridIndexRange()(End,Axes)-gc[grid].gridIndexRange()(Start,Axes)));
      
	  //.......determine max and min value of sqrt( u**2+v**2 )
	  if( !minAndMaxStreamLinesSpecified )
	  {
	    uMax=0.;
	    uMin=REAL_MAX;
	    for( grid=0; grid<numberOfGrids; grid++)
	    {  
	      const RealDistributedArray & coord = (int)gc[grid].isAllVertexCentered() ? gc[grid].vertex() : gc[grid].center();
	      const RealDistributedArray & u = uv[grid];

	      getIndex(gc[grid].dimension(),I1,I2,I3);
	      speed.redim(I1,I2,I3);
	      where(gc[grid].mask()(I1,I2,I3)!=0 && 
		    coord(I1,I2,I3,axis1) >= xa && coord(I1,I2,I3,axis1) <= xb &&
		    coord(I1,I2,I3,axis2) >= ya && coord(I1,I2,I3,axis2) <= yb &&
		    coord(I1,I2,I3,axis3) >= za && coord(I1,I2,I3,axis3) <= zb )
	      {
                #ifndef USE_PPP
		speed=SQR(u(I1,I2,I3,uComponent))+SQR(u(I1,I2,I3,vComponent))+SQR(u(I1,I2,I3,wComponent));
                #else
		Overture::abort("finish me Bill!");
                #endif
	    
		uMax=max(uMax,max(speed));
		uMin=min(uMin,min(speed));
	      }
/* ------ 

   FOR_3(i1,i2,i3,I1,I2,I3)
   {
   if( gc[grid].mask()(i1,i2,i3)!=0 && 
   coord(i1,i2,i3,axis1) >= xa && coord(i1,i2,i3,axis1) <= xb &&
   coord(i1,i2,i3,axis2) >= ya && coord(i1,i2,i3,axis2) <= yb &&
   coord(i1,i2,i3,axis3) >= za && coord(i1,i2,i3,axis3) <= zb )
   {
   real uValue=SQR(uv[grid](i1,i2,i3,uComponent))
   +SQR(uv[grid](i1,i2,i3,vComponent))
   +SQR(uv[grid](i1,i2,i3,wComponent));
   uMax=max(uMax,uValue);
   uMin=min(uMin,uValue);
   }
   }
   ---- */
	    }
	    uMax=SQRT(uMax);
	    uMin=SQRT(uMin);
	  }
	}
	if( minAndMaxStreamLinesSpecified )
	{
	  uMin=minStreamLine;
	  uMax=maxStreamLine;
	}
      
        real uvFactor=uMax-uMin;
	if( uvFactor!=0. )
          uvFactor=1./(uMax-uMin);     // normalization factor for colour table
        else
          uvFactor=1.;
      
	glDeleteLists(list,1);  // clear the plot
	glNewList(list,GL_COMPILE);

        gi.setGlobalBound(plotBound);
	
        // Scale the picture to fit in [-1,1]
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// 	real scale = max(plotBound(End,Range(0,1))-plotBound(Start,Range(0,1)));
// 	real fractionOfScreen=.75;                                  // scale to [fOS,fOS]
// 	windowScaleFactor[0]=fractionOfScreen*2./scale;                
// 	windowScaleFactor[1]=fractionOfScreen*2./scale;                 
// 	windowScaleFactor[2]=fractionOfScreen*2./scale; 
// 	glScalef(windowScaleFactor[0],windowScaleFactor[1],windowScaleFactor[2]);                  
// 	glTranslatef(-(plotBound(Start,axis1)+plotBound(End,axis1))*.5,
// 		     -(plotBound(Start,axis2)+plotBound(End,axis2))*.5,
// 		     -(plotBound(Start,axis3)+plotBound(End,axis3))*.5);


      //......Plot boundary curves of the grid
      // *   plotGridBoundaries(gc,0,0.,psp);
      
      //............Plot Streamlines on each component grid............



	int uStart=min(uComponent,min(vComponent,wComponent));  // these may not be continguous ** problem here **
	int uEnd  =max(uComponent,max(vComponent,wComponent));
	Range Ruvw(uStart,uEnd);

	Range all;
	Range R3(0,2), R;
        int totalNumberOfSeeds=max(2,numberOfSeeds);

	if( seedsHaveChanged )
	{
	  // Collect all the seed points into a single vector
	  seedsHaveChanged=false;
	  numberOfSeeds=numberOfStreamLineStartingPoints
	    +sum(numberOfRakePoints)
	    +sum(numberOfRectanglePoints(all,0)*numberOfRectanglePoints(all,1));

	  R=Range(0,numberOfSeeds-1);
	  seed.redim(R,3);

          totalNumberOfSeeds=max(2,numberOfSeeds);
          seedID.redim(R);
	  seedID.seqAdd(0,1);
	  
	  if( numberOfStreamLineStartingPoints>0 )
	  {
	    Range R0(0,numberOfStreamLineStartingPoints-1);
            #ifndef USE_PPP
	      seed(R0,R3)=streamLineStartingPoint(R0,R3);
            #else
              Overture::abort("finish me Bill!");
            #endif
	  }
	  int s=numberOfStreamLineStartingPoints;
	  // fill in seeds for the rake's
	  int i;
	  for( i=0; i<numberOfRakePoints.getLength(0); i++ )
	  {
	    for( int j0=0; j0<numberOfRakePoints(i); j0++ )
	    {
	      real dr0=j0/real(max(1,numberOfRakePoints(i)-1));
            #ifndef USE_PPP
	      seed(s++,R3)=(1.-dr0)*rakeEndPoint(Start,R3,i)+dr0*rakeEndPoint(End,R3,i);
            #else
              Overture::abort("finish me Bill!");
            #endif
	    }
	  }
	  // fill in seeds for each rectangle
	  for( i=0; i<numberOfRectanglePoints.getLength(0); i++ )
	  {
	    for( int j0=0; j0<numberOfRectanglePoints(i,0); j0++ )
	    {
	      real dr0=j0/real(max(1,numberOfRectanglePoints(i,0)-1));
	      for( int j1=0; j1<numberOfRectanglePoints(i,1); j1++ )
	      {
		real dr1=j1/real(max(1,numberOfRectanglePoints(i,1)-1)); 
                #ifndef USE_PPP
		seed(s++,R3)=
		  (1.-dr1)*( (1.-dr0)*rectanglePoint(Start,R3,0,i)+dr0*rectanglePoint(End,R3,0,i) )
		  +dr1 *( (1.-dr0)*rectanglePoint(Start,R3,1,i)+dr0*rectanglePoint(End,R3,1,i) );
                #else
                  Overture::abort("finish me Bill!");
                #endif

	      }
	    }
	  }
      

	  uInterpolated.redim(R,Ruvw);  
//	  indexGuess.redim(numberOfSeeds,3);   indexGuess=-1;
//	  interpoleeGrid.redim(numberOfSeeds); interpoleeGrid=-1;
//	  wasInterpolated.redim(numberOfSeeds); 
      
	  speed.redim(R);
	  dt.redim(R);
//	  mask.redim(R);
	} // seedsHaveChanged
      
	if( uInterpolated.getBase(1)!=Ruvw.getBase() || uInterpolated.getBound(1)!=Ruvw.getBound() )
	  uInterpolated.redim(R,Ruvw);

	const real epsU = psp.streamLineStoppingTolerance*uMax;   // stop streamlines when |u|+|v| < epsU )
	real dxmx=max(xba,max(yba,zba));

	real cfla=fabs(cfl)*dxmx/nrsmx;    // *** could use dxmx and nrsmx from a particular grid?? ****
      
	if( uMax>uMin && numberOfSeeds>0 ) // no streamLines drawn if velocity is constant
	{
             
	  int numberOfSteps=maximumNumberOfSteps;  // max number of steps
	  real dtmx=cfla*2.;

	  // plot points at initial positions of the seeds
	  glColor3(0.,0.,0.);
	  glPointSize(tracerPointSize*gi.getLineWidthScaleFactor());   
	  glBegin(GL_POINTS);  
	  for( i=0; i<numberOfSeeds; i++ )
	    glVertex3(XSCALE(seed(i,0)),YSCALE(seed(i,1)),ZSCALE(seed(i,2)));
	  glEnd();
	  glPointSize(1.);

	  InterpolatePoints interp;

//  	  // get initial velocity at the points seed
//  	  interpolatePoints(seed, // interpolate u at these points
//  			    uv,
//  			    uInterpolated, 
//  			    Ruvw, nullRange, nullRange,	nullRange, nullRange,
//  			    indexGuess, interpoleeGrid, wasInterpolated);

          real time1=getCPU();
	  interp.interpolatePoints(seed,uv,uInterpolated,Ruvw);
// **	  interp.getInterpolationInfo(gc,indexGuess,interpoleeGrid);
          timeToInterpolate+=getCPU()-time1;
	  
	  IntegerArray & wasInterpolated = (IntegerArray &)interp.getStatus();
	  
	  int num = numberOfSeeds; // *** fix this *** sum(wasInterpolated);
	  if( num!=numberOfSeeds )
	  {
	    // eliminate points that are outside the domain
	    printf("streamLines3d: there were %i initial stream line points were outside the domain\n",
                   numberOfSeeds-num);
	  
	    int j=0;
	    for( i=0; i<numberOfSeeds; i++ )
	    {
	      if( wasInterpolated(i)==InterpolatePoints::interpolated )  // do not allow extrapolated points
	      {
		if( i!=j )
		{
		  seed(j)=seed(i);
                  seedID(j)=seedID(i);
		  uInterpolated(j,Ruvw)=uInterpolated(i,Ruvw);
                  wasInterpolated(j)=wasInterpolated(i);
		}
		j++;
	      }
	    }
	    numberOfSeeds=num;
	    R=Range(0,numberOfSeeds-1);
	    speed.redim(R);
	    dt.redim(R);
//	    mask.redim(R);
	  }

	  int m0=0, m1=1;   // for cycling through the array x[.]
	  x[0].redim(numberOfSeeds,3);  x[0]=seed(R,R3);
	  x[1].redim(numberOfSeeds,3);

	  glLineWidth(2.*psp.size(GraphicsParameters::streamLineWidth)*
		      psp.size(GraphicsParameters::lineWidth)*
		      gi.getLineWidthScaleFactor());
	  glBegin(GL_LINES); 

          // ************************************************************
          // *************    Advance the seeds   ***********************
          // ************************************************************

          const real cflb=min(dtmx,cfla);
	  
	  int numberRemaining=R.getLength();
	  for( int step=0; step<numberOfSteps; step++ )
	  {

            RealArray & x0 = x[m0];
            RealArray & x1 = x[m1];
	    
            int i;
	    int j=0;
            for( i=0; i<numberRemaining; i++ )
	    {
	      speed(j)=sqrt(SQR(uInterpolated(i,uComponent))+SQR(uInterpolated(i,vComponent))+
                            SQR(uInterpolated(i,wComponent)));

              // remove points that have left the region or are moving too slow.

              if( wasInterpolated(i)==InterpolatePoints::interpolated &&
                  x0(i,0)>=xa && x0(i,0)<=xb && 
		  x0(i,1)>=ya && x0(i,1)<=yb && 
		  x0(i,2)>=za && x0(i,2)<=zb && speed(j) > epsU )
	      {

                real deltaT= cflb/speed(j);
		x1(j,0)=x0(i,0)+deltaT*uInterpolated(i,uComponent);
		x1(j,1)=x0(i,1)+deltaT*uInterpolated(i,vComponent);
		x1(j,2)=x0(i,2)+deltaT*uInterpolated(i,wComponent);
		  
		if( i!=j )
		{
                  x0(j,0)=x0(i,0);
                  x0(j,1)=x0(i,1);
                  x0(j,2)=x0(i,2);

		  seedID(j)=seedID(i);

//  		  indexGuess(j)=indexGuess(i);
//  		  interpoleeGrid(j)=interpoleeGrid(i);
		}
		j++;
	      }
	    }
            numberRemaining=j;
	    
	    if( numberRemaining==0 )
	    {
	      printf("All points have left the region or are too slow, step=%i. \n",step);
	      break;
	    }
            else
	    {
	      printf("step %i:  %i particles traces remain in the box [%8.2e,%8.2e][%8.2e,%8.2e][%8.2e,%8.2e]\n",
		     step,numberRemaining,xa,xb,ya,yb,za,zb);
	    }
	    
            R=numberRemaining;
            time1=getCPU();
	    interp.interpolatePoints(x1(R,all), uv, uInterpolated, Ruvw);
            timeToInterpolate+=getCPU()-time1;

//  	    interpolatePoints(x1(R,all), // interpolate u at these points
//  			      uv,
//  			      uInterpolated, 
//  			      Ruvw, nullRange, nullRange,	nullRange, nullRange,
//  			      indexGuess, interpoleeGrid);

	    // modified-modified Euler:  
	    //               x1 = x0 + dt*u0                 : euler step
	    //               u1 = u(x1)
	    //          x(t+dt) = x0 + .5*dt*(u0 + u1)       : second order correction
	    //                  = .5*( x1 + x0 + dt*u1 )
	    // this is not really modified Euler because the velocity is only evaluated once
            if( streamLineColourOption==colourBlack) // !colourStreamLines )
		gi.setColour(GenericGraphicsInterface::textColour);

	    for( i=0; i<numberRemaining; i++ )
	    {
	      real deltaT= cflb/speed(i);
	      x1(i,0)=.5*(x1(i,0)+x0(i,0) +deltaT*uInterpolated(i,uComponent));
	      x1(i,1)=.5*(x1(i,1)+x0(i,1) +deltaT*uInterpolated(i,vComponent));
	      x1(i,2)=.5*(x1(i,2)+x0(i,2) +deltaT*uInterpolated(i,wComponent));

//                printf(" step=%i i=%i x0=(%8.2e,%8.2e,%8.2e) x1=(%8.2e,%8.2e,%8.2e) u=(%8.2e,%8.2e,%8.2e)\n",
//  		     step,i,x0(i,0),x0(i,1),x0(i,2),x1(i,0),x1(i,1),x1(i,2),
//  		     uInterpolated(i,uComponent),uInterpolated(i,vComponent),uInterpolated(i,wComponent));

	      if( streamLineColourOption==colourBySpeed )
		gi.setColourFromTable( (speed(i)-uMin)*uvFactor,psp);
              else if( streamLineColourOption==colourByNumber )
		gi.setColourFromTable( real(seedID(i))/(totalNumberOfSeeds-1.),psp);

	      glVertex3(XSCALE(x0(i,0)),YSCALE(x0(i,1)),ZSCALE(x0(i,2)));
	      glVertex3(XSCALE(x1(i,0)),YSCALE(x1(i,1)),ZSCALE(x1(i,2)));

	    }
/* -----
	    if( FALSE && step>0 && (step % 5 == 0) )   // **** need to keep track of a time for each particle
	    {
	      glEnd();
	      for( int i=0; i<numberOfSeeds; i++ )
	      {
		if( mask(i) )
		{
		  // plot points at equal time intervals
		  glColor3(0.,0.,0.);
		  glPointSize(2.);   
		  glBegin(GL_POINTS);  
		  for( int i=0; i<numberOfSeeds; i++ )
		    glVertex3(x[m1](i,0),x[m1](i,1),x[m1](i,2));
		  glEnd();
		  glPointSize(1.);
		
		}
	      }
	      glBegin(GL_LINES); 
	    }
---- */
	    m0 = (m0+1) % 2;
	    m1 = (m1+1) % 2;
	  }
	  glEnd();
	}
      
	// glPopMatrix();
	glEndList();
	
        real totalTime=getCPU()-time0;
        printf("Time to determine %i tracers was %8.2e (interpolate=%8.2e %4.1f%%)\n",numberOfSeeds,totalTime,
                            timeToInterpolate,100.*timeToInterpolate/totalTime);
	
      } // end if plotStreamLines
      
      
      
      if( psp.plotTitleLabels )
      {
	// plot labels on top and bottom
	aString topLabel=psp.topLabel;
	if( psp.topLabel!="" || (uv.getName(uComponent)!="" && uv.getName(vComponent)!="") )
	  psp.topLabel=psp.topLabel+" ("+uv.getName(uComponent)+","+uv.getName(vComponent)+","+uv.getName(wComponent)+")";
	gi.plotLabels( psp );
	psp.topLabel=topLabel;
      }
      // ----------Draw the colour Bar-----------------
      if( psp.plotColourBar )
      {
	int numberOfColourBarLabels=11;
        gi.drawColourBar(numberOfColourBarLabels,Overture::nullRealArray(),uMin,uMax,psp);
      }
      gi.redraw();
      plotStreamLines=true;
    } // plotObject
  }
  

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)
}

