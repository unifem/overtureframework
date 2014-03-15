#include "GL_GraphicsInterface.h" // Need GL include files for glNewList, glEndList, etc.
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "PlotIt.h"
// This function interpolates a grid function u at some points
int
xInterpolate(const int numberOfPointsToInterpolate,
             const IntegerArray & componentsToInterpolate,
             const RealArray & positionToInterpolate,
             IntegerArray & indexGuess,
             RealArray & uInterpolated, 
             const realGridCollectionFunction & u,
             const GridCollection & gc,
             const int intopt);



void
computeMaximumSpeed( const realGridCollectionFunction & uv, GridCollection & gc,
                     int numberOfGhostLinesToPlot,  int uComponent, int vComponent, 
                      real xa, real xb, real ya, real yb, real & uMin, real & uMax );

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


//\begin{>>PlotItInclude.tex}{\subsection{StreamLines of a realMappedGridFunction}} 
void PlotIt::
streamLines(GenericGraphicsInterface &gi, const realMappedGridFunction & uv, 
            GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */ )
//================================================================================
//
// /Description:
//  Plot stream lines of a two-dimensional vector field.
//  Optionally supply parameters that define the plot characteristics.
//  This routine draws lines that are parallel to a vector field defined by
//  two components of the grid function {\ff uv}. By default component values
//  0 and 1 of the first component of {\ff uv} are used for ``u'' and ``v''.
//  Plotting options include
//  \begin{itemize}
//    \item choose the components to use for ``u'' and ``v''.
//    \item choose new plot bounds (to zoom in on a particular region). In this case
//          new streamlines are drawn on the new region as opposed to the plot
//          being simply magnified.
//  \end{itemize}
//
// /uv (input): function to plot streamlines of.
// /parameters (input): supply optional parameters
//
// /Remarks: \hspace{1mm}
//  
//  \begin{itemize}
//    \item The streamlines are coloured by the relative value of $u^2+v^2$
//    \item Streamlines that move too slowly are stopped  
//    \item There is a maximum number of steps used to integrate any streamline.
//    \item To plot streamlines to cover a CompositeGrid, a rectangular
//          background grid is made that covers some region (this region could be
//          smaller than the entire grid if we are zooming). The number of
//          points on this grid is nxg*nyg. The IntegerArray ig(nxg,nyg) is used
//          to mark cells in the background grid. Streamlines are drawn starting
//          at the midpoints of the background grid. Whenever a streamline
//          passes through a cell of the background grid, the value of ig(i,j) is 
//          increased by one. Only two streamlines are allowed per cell
//          or else the streamline is stopped (or never started).
//            In this way streamlines cover the domain in a reasonably
//          uniform manner.
//  \end{itemize}
//
// /Author: WDH \& AP
//
//\end{PlotItInclude.tex}  
//================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  const MappedGrid & mg = *(uv.mappedGrid);
  GridCollection gc(mg.numberOfDimensions(),1);
  gc[0].reference(mg);
  gc.updateReferences();
  
  Range all;
  realGridCollectionFunction v(gc,all,all,all,Range(uv.getComponentBase(0),uv.getComponentBound(0)));
  v[0]=uv;
  v.setName(uv.getName());
  for( int component=0; component<uv.getComponentDimension(0); component++ )
    v.setName(uv.getName(component),component);
  // v.updateToMatchGrid();
  streamLines(gi, v,parameters);
}


//\begin{>>PlotItInclude.tex}{\subsection{StreamLines of a CompositeGridFunction}} 
void PlotIt::
streamLines(GenericGraphicsInterface &gi, const realGridCollectionFunction & uv0, 
	    GraphicsParameters & parameters)
//==================================================================
// /Description:
//  Streamline plots of the "velocity pair" (u,v) (where u and v are
//  both grid collection functions).
// 
// /uv (input): function to plot streamlines of.
// /parameters (input): supply optional parameters
//  /Remarks:
// \begin{enumerate}
//   \item The streamlines are coloured by the relative value of u**2+v**2
//   \item Streamlines that move too slowly are stopped  
//   \item There is a maximum number of steps used to integrate any streamline.
//   \item To plot streamlines to cover a GridCollection, I make a rectangular
//     background grid that covers some region (this region could be
//     smaller than the entire grid if we are zooming). The number of
//     points on this grid is nxg*nyg. The IntegerArray maskForStreamLines(nxg,nyg) is used
//     to mark cells in the background grid. I draw streamlines starting
//     at the midpoints of the background grid. Whenever a streamline
//     pass through a cell of the background grid I increase the value
//     of maskForStreamLines(i,j) by one. Only two streamlines are allowed per cell
//     or else the streamline is stopped (or never started).
//       In this way streamlines cover the domain in a reasonably
//     uniform manner.
//  \end{enumerate}
//
// /Author: WDH \& AP
//
//\end{PlotItInclude.tex}  
//================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  bool multiProcessorGrid=false;
#ifndef USE_PPP
  const realGridCollectionFunction & uv = uv0;
  GridCollection & gc = *(uv.gridCollection);
#else

//    // In parallel: make a new grid and grid function that only live on one processor
//    realGridCollectionFunction uv;
//    GridCollection gc;
//    int processorForGraphics = gi.getProcessorForGraphics();
//    redistribute( uv0, gc,uv,Range(processorForGraphics,processorForGraphics) );

  // In parallel: make a new grid and gridfunction that only live on one processor
  const int processorForGraphics = gi.getProcessorForGraphics();

  GridCollection & gc0 = *uv0.getGridCollection();
  // Check whether this GridCollection already lives on the processor used for graphics.
  for( int grid=0; grid<gc0.numberOfComponentGrids(); grid++ )
  {
    Partitioning_Type & partition = (Partitioning_Type &)gc0[grid].getPartition();
    const intSerialArray & processorSet = partition.getProcessorSet();
    if( processorSet.getLength(0)!=1 || processorSet(0)!=processorForGraphics )
    {
      multiProcessorGrid=true;
      break;
    }
  }

  GridCollection *gcp=&gc0; 
  realGridCollectionFunction *uvp= (realGridCollectionFunction*)(&uv0); 

  if( multiProcessorGrid )
  {
    if( uv0.getGridCollection()->getClassName()=="CompositeGrid" )
    {
      CompositeGrid & cg = *new CompositeGrid();
      realCompositeGridFunction & uvcg = *new realCompositeGridFunction();

      ParallelGridUtility::redistribute( (realCompositeGridFunction &)uv0, cg,uvcg,
                                        Range(processorForGraphics,processorForGraphics) );

      gcp=&cg;
      uvp=&uvcg;
    }
    else
    {
      gcp = new GridCollection();
      uvp  = new realGridCollectionFunction();

      ParallelGridUtility::redistribute( uv0, *gcp,*uvp,Range(processorForGraphics,processorForGraphics) );

    }
  }
  
  GridCollection & gc = *gcp;
  const realGridCollectionFunction & uv = *uvp;

#endif
  const int numberOfGrids = gc.numberOfComponentGrids();

  // these geometry arrays are needed:
  gc.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
  int grid;
  for( grid=0; grid<numberOfGrids; grid++)
    gc[grid].update(MappedGrid::THEcenterDerivative);

  // this must be here for P++ (only 1 processor actually plots stuff)
  if( Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics() )
  {

    gi.setSingleProcessorGraphicsMode(true);   // on parallel machines we only plot on one processor
    Optimization_Manager::setOptimizedScalarIndexing(On);   // stop communication for scalar indexing

    if( gc.numberOfDimensions()==2 )
      streamLines2d(gi, gc, uv, parameters );
    else if( gc.numberOfDimensions()==3 )
      streamLines3d(gi, gc, uv, parameters );
    else
    {
      cout << "PlotIt::streamLines:ERROR: soory, no stream lines in 1D \n";
    }

    gi.setSingleProcessorGraphicsMode(false);
    Optimization_Manager::setOptimizedScalarIndexing(Off);   // turn communication back on
  }
  if( multiProcessorGrid )
  {
    #ifdef USE_PPP
     delete gcp;
     delete uvp;
    #endif
  }
  
}

void PlotIt::
streamLines2d(GenericGraphicsInterface &gi, GridCollection & gc, 
              const realGridCollectionFunction & uv, 
	      GraphicsParameters & parameters)
{
  bool showTimings=false; // true;

  int nxg,nyg,nrsmx;
  real uMax,uMin,xa,xb,ya,yb,xba,yba;

  int grid;
  const int numberOfGrids = gc.numberOfComponentGrids();

// save the current window number 
  int startWindow = gi.getCurrentWindow();

//  realGridCollectionFunction u,v;
  IntegerArray componentsToInterpolate;
  componentsToInterpolate.resize(2); 




  char buff[160];
  aString answer,answer2;
  aString menu[] = {"!stream line plotter",
                   // "erase and exit",
                   // "plot",
                   "choose first velocity component",
                   "choose second velocity component",
		    // "set min and max",
                   // "choose new plot bounds",
		    // "reset plot bounds",
                   // "set stream line stopping tolerance",
                   // "set streamline density",
		   // "set arrow size",
		    // "plot the grid",
                    ">colour table choices",
                    "rainbow",
                    "gray",
                    "red",
                    "green",
                    "blue",
                   "<plot ghost lines",
                   " ",
//                     "plot the axes (toggle)",
//                     "plot the back ground grid (toggle)",
                   "set origin for axes",
		   "keep aspect ratio",
                   "do not keep aspect ratio",
                   // "erase",
                   // "erase and exit",
                   // "exit this menu",
                   "" };




  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(true);  // true means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  int & uComponent                    = psp.uComponentForStreamLines;
  int & vComponent                    = psp.vComponentForStreamLines;

  bool & plotTitleLabels              = psp.plotTitleLabels;
  bool & plotColourBar                = psp.plotColourBar;
  IntegerArray & backGroundGridDimension = psp.backGroundGridDimension;
  RealArray & plotBound               = psp.plotBound;
  int & numberOfGhostLinesToPlot      = psp.numberOfGhostLinesToPlot;
  IntegerArray & gridsToPlot          = psp.gridsToPlot;
  IntegerArray & gridOptions          = psp.gridOptions;   
  real & minStreamLine                = psp.minStreamLine;
  real & maxStreamLine                = psp.maxStreamLine;
  bool & minAndMaxStreamLinesSpecified=psp.minAndMaxStreamLinesSpecified;
  real & streamLineStoppingTolerance  =psp.streamLineStoppingTolerance;
  real & arrowSize                    =psp.streamLineArrowSize;
  uMin=0.;  uMax=0.;
  bool recomputeVelocityMinMax=true;  // true if we need to compute the max, min of the velocity
  const	aString topLabel1 = psp.topLabel1; // save original topLabel1

  bool plotStreamLines=true;

  gi.setKeepAspectRatio(psp.keepAspectRatio); 

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    backGroundGridDimension=0;  // this means choose values below
    uComponent = uv.getComponentBase(0)-1;
    vComponent = uv.getComponentBound(0)-1;
    gridsToPlot.redim(numberOfGrids);  
    gridsToPlot=GraphicsParameters::toggleSum;  // by default plot all grids, contours, etc.
    gridOptions.redim(numberOfGrids);  
    gridOptions=GraphicsParameters::plotGrid | GraphicsParameters::plotBlockBoundaries | 
                GraphicsParameters::plotInteriorBoundary | GraphicsParameters::plotInterpolation;
  }
  else
  {
    if( gridsToPlot.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      gridsToPlot.redim(numberOfGrids);  
      gridsToPlot=GraphicsParameters::toggleSum;  // by default plot all grids, contours, etc.
    }    
    if( gridOptions.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      int size=gridOptions.getLength(0);
      gridOptions.resize(numberOfGrids); 
      // assign parameters for the new grids:
      for( int grid=size; grid<numberOfGrids; grid++ )
      {
        gridOptions(grid)=GraphicsParameters::plotGrid;
        // interp points on "grid" are plotted if plotInterpolationPoints==true and :
	gridOptions(grid)|=GraphicsParameters::plotInterpolation;

        if( psp.plotGridBlockBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBlockBoundaries;
	if( psp.plotLinesOnGridBoundaries )
	  gridOptions(grid)|=GraphicsParameters::plotBoundaryGridLines;
	if( psp.plotShadedSurfaceGrids )
	  gridOptions(grid)|=GraphicsParameters::plotShadedSurfaces;
	if( psp.plotBackupInterpolationPoints )
	  gridOptions(grid)|=GraphicsParameters::plotBackupInterpolation;
        if( psp.plotInteriorBoundaryPoints )
	  gridOptions(grid)|=GraphicsParameters::plotInteriorBoundary;

        gridOptions(grid)|=GraphicsParameters::plotInteriorGridLines;
      }
    }    
  }
  
  // setup the gridBoundaryConditionOptions array 
  // make a list of all the distinct boundary condition numbers, i=0,1,2,...,numberOfBoundaryConditions
  // gridBoundaryConditionOption(i) & 1 == true : plot this bc
  int numberOfBoundaryConditions=0;
  psp.gridBoundaryConditionOptions.redim(numberOfGrids*gc.numberOfDimensions()*2+1);
  IntegerArray boundaryConditionList;
  boundaryConditionList.redim(numberOfGrids*gc.numberOfDimensions()*2+1);

  psp.gridBoundaryConditionOptions(numberOfBoundaryConditions)=0;
  boundaryConditionList(numberOfBoundaryConditions++)=0;
  
  int side, axis;
  
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    ForBoundary(side,axis)
    { // by default, add to the list:

      psp.gridBoundaryConditionOptions(numberOfBoundaryConditions)=gc[grid].boundaryCondition(side,axis)>0;
      // no: && map.getTypeOfCoordinateSingularity(side,axis)!=Mapping::polarSingularity;
    
      boundaryConditionList(numberOfBoundaryConditions++)=gc[grid].boundaryCondition(side,axis);
      for( int i=0; i<numberOfBoundaryConditions-1; i++ ) // check if it is already in the list
      {
	if(boundaryConditionList(i)==gc[grid].boundaryCondition()(side,axis) )
	{
	  numberOfBoundaryConditions--;  // remove from the list
	  break;
	}
      }
    }
  }

  int nxgMax=40; // by default the background grid has this size as a max

  // Try to guess which components are "u" and "v"
  if( uComponent<uv.getComponentBase(0) || vComponent<uv.getComponentBase(0) )
  {
    uComponent = min(uv.getComponentBound(0),max(uv.getComponentBase(0),uComponent));
    vComponent = min(uv.getComponentBound(0),uComponent+1);
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
  }

  // get Bounds on the grids

  RealArray xBound(2,3);
  getPlotBounds(gc,psp,xBound);
  gi.setGlobalBound(xBound);

  if(plotBound(End,axis1) < plotBound(Start,axis1) )  // assign plot bounds if they have not been assigned
  {
    plotBound=xBound;
  }

  const bool dialogIsOn = !psp.plotObjectAndExit;
  
  GUIState dialog;
  if( dialogIsOn )
  {
    // ******** Build the dialog for changing parameters *********

    dialog.setWindowTitle("Stream-line Plotter");
    dialog.setExitCommand("exit", "exit");

    dialog.setOptionMenuColumns(1);

    dialog.buildPopup(menu);
    // dialog.setUserButtons(buttons);
    // dialog.setUserMenu(pulldownMenu, "Grid");
    
    aString pbCommands[] = {"plot",
                            "reset min max",
                            "reset plot bounds",
                            "plot the grid",
			    "erase",
			    "erase and exit",
			    ""};
    int numRows=4;
    dialog.setPushButtons( pbCommands, pbCommands, numRows ); 

    const int numberOfTextStrings=8;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "streamline density"; 
    sPrintF(textStrings[nt], "%i",nxgMax);
    nt++;
     
    textLabels[nt] = "arrow size"; 
    sPrintF(textStrings[nt], "%g",arrowSize);
    nt++;
     
    textLabels[nt] = "plot bounds"; 
    sPrintF(textStrings[nt], "%g %g %g %g",plotBound(Start,0),plotBound(End,0),plotBound(Start,1),plotBound(End,1));
    nt++;

    textLabels[nt] = "min max"; 
    sPrintF(textStrings[nt], "%g %g",uMin,uMax);
    nt++;

    textLabels[nt] = "stopping tolerance"; 
    sPrintF(textStrings[nt], "%g",streamLineStoppingTolerance);
    nt++;
     

    textLabels[nt] ="ghost lines";
    sPrintF(textStrings[nt], "%i",psp.numberOfGhostLinesToPlot);
    nt++;

    textLabels[nt] = "xScale, yScale"; 
    sPrintF(textStrings[nt], "%g %g",psp.xScaleFactor,psp.yScaleFactor);
    nt++;

    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textLabels[nt]="";   textStrings[nt]="";  

    // addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


    gi.pushGUI( dialog );
  }



  int list=0;
  if( gi.isGraphicsWindowOpen() )
  {
    list=gi.generateNewDisplayList();  // get a new display list to use
    assert(list!=0);
  }
  
  
  // set default prompt
  gi.appendToTheDefaultPrompt("streamLines>");
  int len=0;
  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==true
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
      gi.getAnswer(answer,""); // gi.getMenuItem(menu,answer);

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
    else if( answer=="set origin for axes" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter origin for axes (x,y,z) (enter return for default)")); 
      real xo, yo, zo;
      xo = yo = zo = GenericGraphicsInterface::defaultOrigin;
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e %e %e",&xo, &yo, &zo);
	gi.setAxesOrigin(xo, yo, zo);
      }
    }
    else if( answer=="keep aspect ratio" )
    {
      psp.keepAspectRatio=true;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 
    }
    else if( answer=="do not keep aspect ratio" )
    {
      psp.keepAspectRatio=false;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 
    }
    else if(answer=="choose new plot bounds")
    {
      sPrintF(buff,"Current values: xMin=%e, xMax=%e, yMin=%e, yMax=%e",
	   	   plotBound(Start,0),plotBound(End,0),plotBound(Start,1),plotBound(End,1));
      gi.outputString(buff);
      // outputString("Enter plot bounds: xMin xMax yMin yMax");
      gi.inputString(answer2,"Enter plot bounds: xMin xMax yMin yMax");
      // cout << "answer2 = " << answer2 << endl;
      real xa,xb,ya,yb;
      for( int i=0; i<answer2.length(); i++)
	buff[i]=answer2[i];
      // sScanF(answer2,"%e %e %e %e",&xa,&xb,&ya,&yb);
      sScanF(buff,"%e %e %e %e",&xa,&xb,&ya,&yb);
      // printf("xa=%e, xb=%e, ya=%e, yb=%e \n",xa,xb,ya,yb);
      plotBound(Start,0)=xa; plotBound(End,0)=xb; plotBound(Start,1)=ya; plotBound(End,1)=yb;
      // plotBound(Start,0)+=.5*(plotBound(End,0)-plotBound(Start,0));
      // plotBound(Start,1)+=.5*(plotBound(End,1)-plotBound(Start,1));
      recomputeVelocityMinMax=true;

      psp.usePlotBounds=true;
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(psp.plotBound);
      
    }
    else if( answer=="reset plot bounds" )
    {
      recomputeVelocityMinMax=true;

      plotBound=xBound;
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(plotBound);

      if( dialogIsOn )
	dialog.setTextLabel("plot bounds",sPrintF("%g %g %g %g",plotBound(Start,0),plotBound(End,0),plotBound(Start,1),
						  plotBound(End,1)));

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
      uComponent = gi.getMenuItem(menu2,answer2);
      // make sure that the currentWindow is the same as startWindow! (It might get changed 
      // interactively by the user)
      if (gi.getCurrentWindow() != startWindow)
	gi.setCurrentWindow(startWindow);

      uComponent+=uv.getComponentBase(0);
      delete [] menu2;
      recomputeVelocityMinMax=true;
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
      vComponent = gi.getMenuItem(menu2,answer2);
      // make sure that the currentWindow is the same as startWindow! (It might get changed 
      // interactively by the user)
      if (gi.getCurrentWindow() != startWindow)
	gi.setCurrentWindow(startWindow);

      vComponent+=uv.getComponentBase(0);
      delete [] menu2;
      recomputeVelocityMinMax=true;
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
        minAndMaxStreamLinesSpecified=true;
        uMin=minStreamLine;
	uMax=maxStreamLine;
      }
      else
        minAndMaxStreamLinesSpecified=false;
    }
    else if( len=answer.matches("min max") ) // new way
    {
      gi.outputString("Min and max stream line values only change how the stream lines are coloured");

      sScanF(answer(len,answer.length()-1),"%e %e",&minStreamLine,&maxStreamLine);
      printF("New values are min = %e, max = %e \n",minStreamLine,maxStreamLine);
      minAndMaxStreamLinesSpecified=true;
      uMin=minStreamLine;
      uMax=maxStreamLine;

      if( dialogIsOn ) dialog.setTextLabel("min max",sPrintF(answer,"%g %g",minStreamLine,maxStreamLine));

    }
    else if( answer=="reset min max" )
    {
      minAndMaxStreamLinesSpecified=false;
      recomputeVelocityMinMax=true;
    }
    else if( len=answer.matches("xScale, yScale") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&psp.xScaleFactor,&psp.yScaleFactor);
      printF("New values are xScale = %g, yScale = %g \n",psp.xScaleFactor,psp.yScaleFactor);
      dialog.setTextLabel("xScale, yScale",sPrintF(answer,"%g %g",psp.xScaleFactor,psp.yScaleFactor));
    }
    else if( len=answer.matches("ghost lines") ) // does this work ??
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfGhostLinesToPlot);
      dialog.setTextLabel("ghost lines",sPrintF(answer,"%i",numberOfGhostLinesToPlot));
      gi.outputString(sPrintF(buff,"Plot %i ghost lines\n",numberOfGhostLinesToPlot));
    }

    else if(  len=answer.matches("plot bounds") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&plotBound(Start,0),&plotBound(End,0),&plotBound(Start,1),
	     &plotBound(End,1));

      dialog.setTextLabel("plot bounds",sPrintF("%g %g %g %g",plotBound(Start,0),plotBound(End,0),plotBound(Start,1),
						plotBound(End,1)));

      recomputeVelocityMinMax=true;

      psp.usePlotBounds=true;
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(plotBound);
			  
    }
    else if( len = answer.matches("streamline density") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&nxgMax);

      if( nxgMax<=0 || nxgMax>10000 )
      {
	printf("Error: streamline density=%i is <=0 or too large. Try a number between 1 and 10000\n",nxgMax);
	nxgMax=40;
      }
      else
      {
	if( backGroundGridDimension(0)>0 && backGroundGridDimension(1)>0 )
	{
	  const int maxBack=max(backGroundGridDimension(Range(0,1)));
	  backGroundGridDimension(0)=(int)max(2,nxgMax*backGroundGridDimension(0)/maxBack+.5);
	  backGroundGridDimension(1)=(int)max(2,nxgMax*backGroundGridDimension(1)/maxBack+.5);
	}
	else
	{
	  backGroundGridDimension(0)=0;
	  backGroundGridDimension(1)=0;
	}
      }
      dialog.setTextLabel("streamline density",sPrintF("%i",nxgMax));
      
      plotStreamLines=plotObject;  
    }
    else if( dialog.getTextValue(answer,"arrow size","%e",arrowSize) ){}//
    else if( dialog.getTextValue(answer,"stopping tolerance","%e",streamLineStoppingTolerance) ){}//

    else if( answer=="set stream line stopping tolerance" )
    {
      gi.outputString("Stream line stopping tolerance : lines are drawn until the velocity decreases by this factor");
      gi.inputString(answer2,sPrintF(buff,"Enter stopping tolerance for stream lines, (current=%e)",
                   streamLineStoppingTolerance )); 
      if( answer2 !="" && answer2!=" ")
	sScanF(answer2,"%e",&streamLineStoppingTolerance);
    }
    else if( answer=="set arrow size" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the arrow size, (current=%e)",arrowSize));
      if( answer2 !="" && answer2!=" ")
	sScanF(answer2,"%e",&arrowSize);
    }
    else if( answer=="set streamline density" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the resolution for stream lines (default=%i)",
                   nxgMax)); 
      if( answer2 !="" )
      {
	sScanF(answer2,"%i",&nxgMax);
        if( nxgMax<=0 || nxgMax>10000 )
	{
	  printf("Error: streamline density=%i is <=0 or too large. Try a number between 1 and 10000\n",nxgMax);
	  nxgMax=40;
	}
	else
	{
          if( backGroundGridDimension(0)>0 && backGroundGridDimension(1)>0 )
	  {
            const int maxBack=max(backGroundGridDimension(Range(0,1)));
	    backGroundGridDimension(0)=(int)max(2,nxgMax*backGroundGridDimension(0)/maxBack+.5);
	    backGroundGridDimension(1)=(int)max(2,nxgMax*backGroundGridDimension(1)/maxBack+.5);
	  }
	  else
	  {
	    backGroundGridDimension(0)=0;
	    backGroundGridDimension(1)=0;
	  }
	}
      }
      plotStreamLines=plotObject;  
    }
    else if( answer=="plot ghost lines" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the number of ghost lines (or cells) to plot (current=%i)",
             numberOfGhostLinesToPlot)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF(buff,"Plot %i ghost lines\n",numberOfGhostLinesToPlot));
      }
    }
    else if( answer=="plot the grid" )
    {
      plot(gi, (GridCollection&)gc, psp);
    }
    else if( answer=="plot" )
    {
      plotObject=true;
      plotStreamLines=plotObject;  
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
    if( plotObject && gi.isGraphicsWindowOpen()  )
    {
      if( uComponent<uv.getComponentBase(0) || uComponent>uv.getComponentBound(0))
      {
	cout << "PlotIt::streamLines:ERROR Invalid component for u : " << uComponent << endl;
        cout << "You must choose a valid component for u before the stream lines can be drawn\n";
        plotObject=false;
      }
      if( vComponent<uv.getComponentBase(0) || vComponent>uv.getComponentBound(0))
      {
	cout << "PlotIt::streamLines:ERROR Invalid component for v : " << vComponent << endl;
        cout << "You must choose a valid component for v before the stream lines can be drawn\n";
        plotObject=false;
      }
    }
    if( plotObject )
    {
      real time0=getCPU();
      
      gi.setAxesDimension( gc.numberOfDimensions() );
  
//       printF("StreamLines: aspectRatio = %e \n",gi.getAspectRatio());
//       if( true )
//       {
// 	GL_GraphicsInterface & gigl = (GL_GraphicsInterface &)gi;
//         RealArray & gb = gigl.globalBound[0];
//         real *rc = gigl.rotationCenter[0];
// 	printF(" globalBound = [%e,%e][%e,%e][%e,%e]\n",gb(0,0),gb(1,0),gb(0,1),gb(1,1),gb(0,2),gb(1,2));
// 	printF(" rotationCenter = [%e,%e,%e]\n",rc[0],rc[1],rc[2]);
//       }


      if( plotStreamLines )
      {
	glDeleteLists(list,1);  // clear the plot

      // printf(" uv.getName = %s \n",(const char*)uv.getName());
      // printf(" uv[0].getName = %s \n",(const char*)uv[0].getName());
      // printf(" uv.getName(0) = %s \n",(const char*)uv.getName(0));
      // printf(" uv[0].getName(0) = %s \n",(const char*)uv[0].getName(0));
      
	componentsToInterpolate(0)=uComponent; // these are used by xInterpolate
	componentsToInterpolate(1)=vComponent;

	int intopt=2+8;   //  intopt: 2=2nd order interpolation, 8=use xyrs array

	xa=plotBound(Start,axis1); xb=plotBound(End,axis1); xba=xb-xa;
	ya=plotBound(Start,axis2); yb=plotBound(End,axis2); yba=yb-ya;

	nrsmx=0;
	Range Axes(0,gc.numberOfDimensions()-1);
	for( grid=0; grid<numberOfGrids; grid++)
	  nrsmx=max(nrsmx,max(gc[grid].gridIndexRange()(End,Axes)-gc[grid].gridIndexRange()(Start,Axes)));

         //.......determine max and min value of sqrt( u**2+v**2 )    
	if( !minAndMaxStreamLinesSpecified )
	{
	  if( recomputeVelocityMinMax )
	  {
	    recomputeVelocityMinMax=false;

            computeMaximumSpeed( uv,gc,numberOfGhostLinesToPlot,uComponent,vComponent,xa,xb,ya,yb, uMin,uMax );

	    if( dialogIsOn ) dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
	    
	  }
	}
	else
	{
	  uMin=minStreamLine;
	  uMax=maxStreamLine;
	}
      
	// -----determine how big to make the background grid------
	// *** this is wrong if we zoom in ****
	if( backGroundGridDimension(0)>0 && backGroundGridDimension(1)>0 )
	{
	  nxg=backGroundGridDimension(0);
	  nyg=backGroundGridDimension(1);
	}
	else
	{
	  real dwx=plotBound(End,axis1)-plotBound(Start,axis1);
	  real dwy=plotBound(End,axis2)-plotBound(Start,axis2);

	  // if( (nrsmx % 10 ) < 2 || (nrsmx % 10) >8  ) nrsmx=nrsmx+4;  // ******8

	  nxg=0;  // ****
	  nyg=0;

	  if( dwy>dwx )
	  {
	    nyg=min(nxgMax,nrsmx);
	    nxg=(int)max(.3*nyg,nyg*dwx/dwy);
	  }
	  else
	  {
	    nxg=min(nxgMax,nrsmx);
	    nyg=(int)max(.3*nxg,nxg*dwy/dwx);
	  }
	  nxg+=3;  // *********************************
	  nyg+=3;
	  backGroundGridDimension(0)=nxg;
	  backGroundGridDimension(1)=nyg;
	}      
	// the array maskForStreamLines(ixg,iyg) is set >= 1 if a streamline passes through the cell (ixg,iyg)
	// maskForStreamLines.redim(Range(-5,nxg+5),Range(-5,nyg+5)); maskForStreamLines=0;
	IntegerArray maskForStreamLines;
	maskForStreamLines.redim(nxg,nyg); maskForStreamLines=0;



	glNewList(list,GL_COMPILE);

        glPushName(gc.getGlobalID()); // assign a name for picking

        //......Plot boundary curves of the grid
        real uRaise=0.;
	plotGridBoundaries(gi, gc, boundaryConditionList, numberOfBoundaryConditions, 0, uRaise, psp);
      
	//............Plot Streamlines on each component grid............
	if( uMax>uMin ) // no streamLines drawn if velocity is constant
	{
	  PlotIt::plotStreamLines(gi, gc, uv, componentsToInterpolate, maskForStreamLines, arrowSize, psp,
	      	  xa, ya,  xb, yb, xba, yba, uMin, uMax, nrsmx, nxg,  nyg,  intopt );
	}
	glPopName();
	glEndList();
      }
      
//       if( psp.plotTitleLabels )
//       {
// 	// plot labels on top and bottom
// 	aString topLabel=psp.topLabel;
// 	if( psp.topLabel!="" || (uv.getName(uComponent)!="" && uv.getName(vComponent)!="") )
// 	  psp.topLabel=psp.topLabel+" ("+uv.getName(uComponent)+","+uv.getName(vComponent)+")";
// 	gi.plotLabels( psp );
// 	psp.topLabel=topLabel;
//       }

      if( plotTitleLabels )
      {
 	// plot labels on top and bottom
 	aString topLabel=psp.topLabel;       // remember original values
	// aString topLabel1 = psp.topLabel1;
	
        if( psp.labelComponent )
	{
	if( psp.topLabel!="" || (uv.getName(uComponent)!="" && uv.getName(vComponent)!="") )
	  psp.topLabel=psp.topLabel+" ("+uv.getName(uComponent)+","+uv.getName(vComponent)+")";
	}
	
	if( psp.labelMinMax )
	{
	  // label min max of components
          aString label = "("+uv.getName(uComponent)+","+uv.getName(vComponent)+")";
	  
	  if(  max(fabs(uMax),fabs(uMin)) < .01 )
	    label += sPrintF(buff,"=[%8.2e,%8.2e]",uMin,uMax);
	  else if( max(fabs(uMax),fabs(uMin)) < 10. )
	    label += sPrintF(buff,"=[%6.3f,%6.3f]",uMin,uMax);
	  else if( max(fabs(uMax),fabs(uMin)) < 100. )
	    label += sPrintF(buff,"=[%6.2f,%6.2f]",uMin,uMax);
	  else if( max(fabs(uMax),fabs(uMin)) < 1000. )
	    label += sPrintF(buff,"=[%6.1f,%6.1f]",uMin,uMax);
	  else 
	    label += sPrintF(buff,"=[%8.2e,%8.2e]",uMin,uMax);
	  
//           printf(" contour: psp.topLabel1=%s (before) label=%s, psp.labelMinMax=%i\n",
//                   (const char*)psp.topLabel1,(const char*)label,psp.labelMinMax);
	  
          if( psp.labelMinMax==1 )
            psp.topLabel1 = label;  // set label
          else
            psp.topLabel1 = topLabel1 + " " + label;  // add to the label
	}

	gi.plotLabels( psp );

	psp.topLabel=topLabel;  // reset
        // no: psp.topLabel1=topLabel1;
      }

      // ----------Draw the colour Bar-----------------
      if( plotColourBar )
      {
	int numberOfColourBarLabels=11;
        gi.drawColourBar(numberOfColourBarLabels,Overture::nullRealArray(),uMin,uMax,psp);
      }
      gi.redraw();
      plotStreamLines=true;

      if( showTimings ) printf("Total time for streamLines=%8.2e \n",getCPU()-time0);
    }
    
  }

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)

}

