#include "GL_GraphicsInterface.h"
#include "CompositeGrid.h"
#include "SquareMapping.h"
#include "PlotIt.h"
#include "ContourSurface.h"
#include "display.h"
#include "xColours.h"

int ContourSurface::globalIDCounter=12345678; // this should be unique from reference counted class


#define ISOSURF EXTERN_C_NAME(isosurf)
extern "C"
{
  void ISOSURF(const int &nTet, real & scalarFieldAtVertices, const int & leadingDimensionOfDAndPx, 
	       const int & numberOfDataComponents,
	       real & dataAtVertices, const int & cornerIndex1, const int & cornerIndex2, 
	       const int & cornerIndex3,
	       const int & numberOfContourLevels, real & contourLevels, 
	       const int & numberOfVertices,
	       real & vertexList);
}

void 
displayValuesAtAPoint(RealArray x0, realCompositeGridFunction & ucg, int numberOfComponents, 
                      int component, int showNearbyValues, int showUnusedValues,
                      IntegerArray *checkTheseGrids = NULL );

void 
getNormal(const realArray & x, 
	  const int iv[3],
          const int axis,
	  real normal[3],
          const int & recursion=TRUE );


#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  \

#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

//! Initialize the list of ContourSurface objects.
void
initContourSurfaceList( ContourSurface *cs, const GridCollection & gc, IntegerArray & gridsToPlot, 
			IntegerArray & plotContourOnGridFace, int & numberOfCoordinatePlanes, 
			IntegerArray & coordinatePlane,
			int & numberOfContourPlanes, int & numberOfIsoSurfaces, 
                        RealArray & isoSurfaceValue,
			IntegerArray & numberOfPolygonsPerSurface, 
			IntegerArray & numberOfVerticesPerSurface )
{
  const int numberOfGrids=gc.numberOfGrids();
  
  const int totalNumberOfCoordinatePlanes=numberOfCoordinatePlanes+sum(plotContourOnGridFace);
  const int totalNumberOfSurfaces=totalNumberOfCoordinatePlanes+
                              numberOfContourPlanes+ numberOfIsoSurfaces;

  // build ContourSurface objects
  int ns=0; // counts number of surfaces
  int numberOfBoundaryFaces=0; // counts number of boundary faces plotted.
  int grid;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    if( !(gridsToPlot(grid)&2) )
    {
      printF("**** initContourSurfaceList: skip grid =%i ******** \n",grid);
      continue;
    }
    
    const MappedGrid & mg = gc[grid];
    int side,axis;
    ForBoundary(side,axis)
    {
      if( plotContourOnGridFace(side,axis,grid) )
      {
	cs[ns].surfaceType=ContourSurface::coordinateSurface;
	cs[ns].surfaceStatus=ContourSurface::notBuilt;
	cs[ns].side=side;
	cs[ns].axis=axis;
	cs[ns].grid=grid;
	ns++;
	numberOfBoundaryFaces++;
      }
    }
    for( int plane=0; plane<numberOfCoordinatePlanes; plane++ )
    {
      if( coordinatePlane(0,plane)==grid )
      {
	axis=coordinatePlane(1,plane);
	int index=coordinatePlane(2,plane);
	if( axis<0 || axis>2 || 
	    index<mg.gridIndexRange(Start,axis) || index > mg.gridIndexRange(End,axis) )
	{
	  printF("initContourSurfaceList:ERROR: there are invalid values specifying a coordinate plane, grid=%i, axis=%i, index=%i \n",
		 grid,axis,index);
	  continue;
	}
	cs[ns].surfaceType=ContourSurface::coordinateSurface;
	cs[ns].surfaceStatus=ContourSurface::notBuilt;
	cs[ns].index=index;
	cs[ns].axis=axis;
	cs[ns].grid=grid;
	ns++;
      }
    }
  }
  int n;
  for( int n=0; n<numberOfContourPlanes; n++ )
  {
    cs[ns].surfaceType=ContourSurface::contourPlane;
    cs[ns].surfaceStatus=ContourSurface::notBuilt;
    ns++;
  }
  for( int n=0; n<numberOfIsoSurfaces; n++ )
  {
    cs[ns].surfaceType=ContourSurface::isoSurface;
    cs[ns].surfaceStatus=ContourSurface::notBuilt;

    cs[ns].surfaceColourType=ContourSurface::SurfaceColourTypeEnum(int(isoSurfaceValue(n,1)+.5));
    cs[ns].colourIndex=int(isoSurfaceValue(n,2)+.5);

    printF("initContourSurfaceList: isoSurf %i : setting surfaceColourType=%i and colourIndex=%i\n",n,(int)cs[ns].surfaceColourType,
           cs[ns].colourIndex);

    ns++;
  }
  if( false && ns!=totalNumberOfSurfaces )
  {
    printF("initContourSurfaceList:ERROR:  ns=%i is not equal to totalNumberOfSurfaces=%i\n"
           " numberOfCoordinatePlanes=%i, numberOfContourPlanes=%i, numberOfIsoSurfaces=%i, numberOfBoundaryFaces=%i"
           " sum(plotContourOnGridFace)=%i\n",
	   ns,totalNumberOfSurfaces,numberOfCoordinatePlanes,numberOfContourPlanes,numberOfIsoSurfaces,numberOfBoundaryFaces,
           sum(plotContourOnGridFace));
    printF("numberOfGrids=%i, plotContourOnGridFace.getLength(2)=%i, max()=%i min=%i\n",numberOfGrids,plotContourOnGridFace.getLength(2),
               max(plotContourOnGridFace),min(plotContourOnGridFace));
    
  }
  assert( ns<=totalNumberOfSurfaces );

  const int maxNumberOfPolygons=10000; // 20;  // **** initially allocate this much space to hold polygons on a surface

  numberOfPolygonsPerSurface.redim(totalNumberOfSurfaces); numberOfPolygonsPerSurface=0;
  numberOfVerticesPerSurface.redim(totalNumberOfSurfaces,maxNumberOfPolygons);
  numberOfVerticesPerSurface=0;
}

static int plotContoursOnGridBoundaries=false; // ******* add to GraphicsParameters ***********

//--------------------------------------------------------------------------------------
//  Plot contours on planes through a 3D grid function
//--------------------------------------------------------------------------------------
void PlotIt::
contour3d( GenericGraphicsInterface &gi, const realGridCollectionFunction & uGCF, GraphicsParameters & parameters)
{
  const bool showTimings=false; // true;
  
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int graphicsProcessor = gi.getProcessorForGraphics();
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

#ifdef USE_PPP
  if( false && PlotIt::parallelPlottingOption==1 )
  {
    printf("*** contour3d: START myid=%i ***\n",myid);
    fflush(0);
    MPI_Barrier(Overture::OV_COMM);
  }
#endif



  // save the current window
  int startWindow = gi.getCurrentWindow();

  const GridCollection & gc = *(uGCF.gridCollection);
  const int numberOfGrids = gc.numberOfComponentGrids();
  const int numberOfBaseGrids = gc.numberOfBaseGrids();
  const int numberOfDimensions = gc.numberOfDimensions();

  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;
  psp.objectWasPlotted=0;

  real contourShift=.1; //  add to graphics parameters
  real uMin=REAL_MAX;
  real uMax=-uMin;

  int side,axis;
  char buff[100];
  aString answer,answer2;
  aString menu0[]= { "!contour plotter (3d)",
                    "erase and exit",
		    " ",
//                    "plot",
                    ">choose a component",
//                  "<>contour planes",
//  		      "choose contour planes",
//		      "add contour planes",
//		      "remove contour planes",
//  		      "+shift contour planes",
//  		      "-shift contour planes",
//                    "plot contour lines (toggle)",
//                    "plot shaded contours (toggle)",
//                    "colour line contours (toggle)",
		    "<>contour options",
//                    "number of contour levels",
//                    "set min and max",
                    "user defined output",
                    "set minimum contour spacing",
                    "toggle grids on and off",
                    "<plot contours on grid boundaries",
                    "plot contours on coordinate planes",
                    "iso-surface",
                    "2D contours on coordinate planes",
                    "line plots",
//		    "plot the grid",
                    ">colour table choices",
                    "rainbow",
                    "gray",
                    "red",
                    "green",
                    "blue",
                    "user defined",
		   "< ",
		    "set plot bounds",
		    "reset plot bounds",
//                    "<plot ghost lines",
//                    " ",
//                    "erase",
//                    "exit",
                    "" };

  


// // setup a user defined menu and some user defined buttons
//   aString buttons[][2] = {{"plot contour lines (toggle)",        "Lines"}, 
// 			 {"plot shaded contours (toggle)",      "Shade"},
// 			 {"colour line contours (toggle)",      "ColLin"}, 
// 			 {"plot",                               "Plot"},
// 			 {"erase",                              "Erase"},
// 			 {"exit",                               "Exit"},
// 			 {"",                                   ""}};
//   aString pulldownMenu[] = {"set min and max", "number of contour levels", "" };
//   aString menuTitle = "Contour";
  
  
  // create the real menu by adding in the component names, these will appear as a 
  // cascaded menu
  int chooseAComponentMenuItem;  // menu[chooseAComponentMenuItem]=">choose a component"
  int numberOfMenuItems0=0;
  while( menu0[numberOfMenuItems0]!="" )
  {
    numberOfMenuItems0++;
  }  
  numberOfMenuItems0++;

  int numberOfComponents=uGCF.getComponentDimension(0);
  aString *menu = new aString [numberOfMenuItems0+numberOfComponents];
  int i=-1;
  for( int i0=0; i0<numberOfMenuItems0 ; i0++ )
  {
    menu[++i]=menu0[i0];    
    if( menu[i]==">choose a component" )
    {
      chooseAComponentMenuItem=i;
      for( int j=0; j<numberOfComponents; j++ )
      {
        menu[++i]=uGCF.getName(uGCF.getComponentBase(0)+j);
        if( menu[i] == "" || menu[i]==" " )
          menu[i]=sPrintF(buff,"component%i",uGCF.getComponentBase(0)+j);
      }
    }
  }


  // *************Dialog interface ********************************************
  DialogData *interface=NULL;  // could be passed in the future
  
  GUIState gui;
  gui.setWindowTitle("3D Contour and Isosurface Plotter");
  gui.setExitCommand("exit", "exit");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  enum PickingOptionEnum
  {
    pickingOff,
    pickToDeleteContourPlane,
    pickToAddContourPlaneX,
    pickToAddContourPlaneY,
    pickToAddContourPlaneZ,
    pickToAddBoundarySurface,
    pickToDeleteBoundarySurface,
    pickToDeleteCoordinateSurface,
    pickToAddCoordinateSurface1,
    pickToAddCoordinateSurface2,
    pickToAddCoordinateSurface3,
    pickToQueryValue,
    pickToColourSurface,
  } pickingOption=pickingOff;


  DialogData & colourDialog = gui.getDialogSibling();
  colourDialog.setWindowTitle("Pick colour");
  colourDialog.setExitCommand("close colour choices", "close");

  int pickColourIndex=getXColour("aquamarine");  // index of the colour used for pick to colour grids

  if( !psp.plotObjectAndExit )
  {
    printF("----------------------------------------------------------------------------------------\n"
           "The 3D contour plotter can display\n"
           " 1. contour planes : contours on a plane that cuts the entire grid.\n"
           "            The contour plane is defined by a normal vector and a point on the plane.\n"
           " 2. coordinate surface: contours on a surface on a single component grid corresponding \n"
           "                to i1=const or i2=const or i3=const\n"
           " 3. iso-surface : surface defined by u=constant.\n"
           " 4. 2D contours on coordinate planes: contours on a coordinate plane i1=const, i2=const\n"
           "                or i3=const. These plots are in the 2d parameter space of the plane.\n"
           "-----------------------------------------------------------------------------------------\n");

    PlotIt::buildColourDialog(colourDialog);
    
    dialog.setOptionMenuColumns(1);

//     aString *componentCmd = new aString [ numberOfComponents+1 ];
//     aString *componentLabel = new aString [ numberOfComponents+1 ];
//     for( int i=0; i<numberOfComponents; i++ )
//     {
//       componentLabel[i]=uGCF.getName(uGCF.getComponentBase(0)+i);
//       if( componentLabel[i] == "" || componentLabel[i]==" " )
// 	componentLabel[i]=sPrintF(buff,"component%i",uGCF.getComponentBase(0)+i);
//       componentCmd[i] = sPrintF(buff,"component %i",uGCF.getComponentBase(0)+i);
//     }
//     componentCmd[numberOfComponents]="";
//     componentLabel[numberOfComponents]="";

//     dialog.addOptionMenu("component:", componentCmd,componentLabel,psp.componentForContours);
//     delete [] componentCmd;
//     delete [] componentLabel;

    // *wdh* new way matches the 2d contour plotter 
    const int numberOfComponents = uGCF.getComponentBound(0)-uGCF.getComponentBase(0)+1;
    // create a new menu with options for choosing a component.
    aString *cmd = new aString[numberOfComponents+1];
    aString *label = new aString[numberOfComponents+1];
    for( int n=0; n<numberOfComponents; n++ )
    {
      label[n]=uGCF.getName(n);
      cmd[n]="plot:"+uGCF.getName(n);

    }
    cmd[numberOfComponents]="";
    label[numberOfComponents]="";
    
    dialog.addOptionMenu("Plot component:", cmd,label,
                            max(uGCF.getComponentBase(0),min(uGCF.getComponentBound(0),psp.componentForContours)));
    delete [] cmd;
    delete [] label;


    aString opcmd[] = {"pick off",
                       "pick to delete contour planes",
                       "pick to add contour plane x",
                       "pick to add contour plane y",
                       "pick to add contour plane z",
                       "pick to add boundary surface",
                       "pick to delete boundary surface",
                       "pick to delete coordinate surface",
                       "pick to add coordinate surface 1",
                       "pick to add coordinate surface 2",
                       "pick to add coordinate surface 3",
                       "pick to query value",
                       "pick to colour surface",
                       ""};
    aString opLabel[] = {"off",
                         "delete contour planes",
                         "add contour plane x",
                         "add contour plane y",
                         "add contour plane z",
			 "add boundary surface",
			 "delete boundary surface",
                         "delete coordinate surface",
			 "add coordinate surface 1",
			 "add coordinate surface 2",
			 "add coordinate surface 3",
                         "pick to query value",
                         "pick to colour surface",
                         ""};
    dialog.addOptionMenu("Pick to:", opcmd,opLabel,pickingOption);

    aString opcmd2[] = {"base min/max on global values",
                        "base min/max on contour plane values",
                        ""};
    dialog.addOptionMenu("Min/Max:", opcmd2,opcmd2,psp.contour3dMinMaxOption);




    aString tbCommands[] = {"shaded surfaces",
                            "contour lines",
			    "colour lines",
                            "adjust grid for displacement",
			    ""};
    int tbState[10];
    tbState[0] = psp.plotShadedSurface==true;
    tbState[1] = psp.plotContourLines==true; 
    tbState[2] = psp.colourLineContours==true; 
    tbState[3] = psp.adjustGridForDisplacement;  
    tbState[4] = 0; 
    int numColumns=2;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

    aString pbLabels[] = { "choose contour planes",
			   "add contour planes",
			   "remove contour planes",
			   "+shift contour planes",
			   "-shift contour planes",
//                           "choose coordinate planes",
			   "iso-surface",
                           "turn on grid boundaries",
                           "turn off grid boundaries",
			   "plot",
                           "plot the grid",
                           "reset min max",
                           "pick colour...",
			   "erase",
			   "erase and exit",
			   ""};
    int numRows=7;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 
    

    const int numberOfTextStrings=9;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int nt=0;

    textLabels[nt] = "min max"; 
    sPrintF(textStrings[nt], "%g %g",uMin,uMax);
    nt++;

    textLabels[nt] = "contour shift"; 
    sPrintF(textStrings[nt], "%.2g",contourShift);
    nt++;

    textLabels[nt] ="number of levels";
    sPrintF(textStrings[nt], "%i",psp.numberOfContourLevels);
    nt++;
    
    textLabels[nt] ="ghost lines";
    sPrintF(textStrings[nt], "%i",psp.numberOfGhostLinesToPlot);
    nt++;

    textLabels[nt] ="query grid point"; 
    sPrintF(textStrings[nt], "%i %i %i %i (grid, i1,i2,i3)",0,0,0,0); 
    nt++;

    textLabels[nt] ="iso-surface values"; 
    sPrintF(textStrings[nt], "%i %3.1f %3.1f (num, value1, value2, ...)",0,-1.,1.); 
    nt++;

    textLabels[nt] = "xScale, yScale, zScale";
    sPrintF(textStrings[nt], "%g %g %g",psp.xScaleFactor,psp.yScaleFactor,psp.zScaleFactor);
    nt++;

    textLabels[nt] = "displacement scale factor";
    sPrintF(textStrings[nt], "%g",psp.displacementScaleFactor);
    nt++;

    textLabels[nt]=""; 
  
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


    gui.buildPopup(menu);
    gi.pushGUI( gui );
  }
  // **********************************************************************************


  int list=0, lightList=0;
  if( gi.isGraphicsWindowOpen() )
  {
    list=gi.generateNewDisplayList(0);       // get a new (unlit) display list to use
    assert(list!=0);
    lightList=gi.generateNewDisplayList(1);  // get a new (lit) display list to use
    assert(lightList!=0);
  }

  bool & plotWireFrame        = psp.plotWireFrame;
  bool & colourLineContours   = psp.colourLineContours;
  bool & plotColourBar        = psp.plotColourBar;
  bool & plotContourLines     = psp.plotContourLines;
  bool & plotShadedSurface    = psp.plotShadedSurface;
  bool & plotTitleLabels      = psp.plotTitleLabels;
  int  & numberOfContourLevels= psp.numberOfContourLevels;
//  gi.setPlotTheAxes(psp.plotTheAxes);
  
  int  & component            = psp.componentForContours;
  IntegerArray & gridsToPlot      = psp.gridsToPlot;
  IntegerArray & minAndMaxContourLevelsSpecified  = psp.minAndMaxContourLevelsSpecified;
  RealArray & minAndMaxContourLevels = psp.minAndMaxContourLevels;
  real & minimumContourSpacing = psp.minimumContourSpacing;
//  bool & linePlots             = psp.linePlots;

  int & numberOfIsoSurfaces     = psp.numberOfIsoSurfaces;
  RealArray & isoSurfaceValue   = psp.isoSurfaceValue;
  int & numberOfCoordinatePlanes= psp.numberOfCoordinatePlanes; 
  IntegerArray & coordinatePlane    = psp.coordinatePlane;
  // We use this array to plot contours on the faces of grids
  IntegerArray & plotContourOnGridFace = psp.plotContourOnGridFace;

  bool recomputeVelocityMinMax=TRUE;
  bool plotContours=TRUE;
  const	aString topLabel1 = psp.topLabel1; // save original topLabel1

  int & numberOfGhostLinesToPlot = psp.numberOfGhostLinesToPlot;
//  if( numberOfGhostLinesToPlot> 0 )
//    setMaskAtGhostPoints(gc,numberOfGhostLinesToPlot); // set values


  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    gridsToPlot.redim(numberOfGrids);  gridsToPlot=1+2+4+8+16+32;  // by default plot all grids, contours etc.
    plotContourOnGridFace.redim(2,3,numberOfGrids);
    plotContourOnGridFace=FALSE;
  }
  else
  {
    if( gridsToPlot.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      gridsToPlot.redim(numberOfGrids);  gridsToPlot=63;
    }
    if( plotContourOnGridFace.getLength(2) != numberOfGrids )
    { // number of grids has changed

      int oldNumberOfGrids= plotContourOnGridFace.getLength(2);
      
      plotContourOnGridFace.resize(2,3,numberOfGrids);   // keep old values 

      if( numberOfGrids>oldNumberOfGrids )
      {
	Range all;
	plotContourOnGridFace(all,all,Range(oldNumberOfGrids,numberOfGrids-1))=false;

	// assign values to new base grids
	for( int grid=oldNumberOfGrids; grid<numberOfBaseGrids; grid++ ) 
	{
	  ForBoundary(side,axis)
	  {
	    if( gc[grid].boundaryCondition()(side,axis)>0 )
	      plotContourOnGridFace(side,axis,grid)=plotContoursOnGridBoundaries ? 1 : 0;
  
	  }
	}
      }
    }    
  }


  // Make sure some arrays are big enough:
  if( minAndMaxContourLevelsSpecified.getLength(0) < uGCF.getComponentDimension(0) )
  {
    int newSize = uGCF.getComponentDimension(0)+10; // leave room for all components plus extra
    Range R(minAndMaxContourLevelsSpecified.getBound(0)+1,newSize-1);
    minAndMaxContourLevelsSpecified.resize(newSize);
    minAndMaxContourLevels.resize(2,newSize);
    minAndMaxContourLevelsSpecified(R)=FALSE;   // give values to new entries
    minAndMaxContourLevels(Range(0,1),R)=0.;
  }
  


  gi.setKeepAspectRatio(true); 

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  // get Bounds on the grids
  RealArray xBound(2,3);
  Range R(0,2);
  // get Bounds on the grids
  // *wdh* 000317 getGridBounds(gc,psp,xBound);

  bool updateGeometry=true;     // we compute geometry arrays later, if needed.
  bool computePlotBounds=true;  // this means compute the plot bounds at the appropriate
  // *wdh* 070910 if( gi.isGraphicsWindowOpen() || PlotIt::parallelPlottingOption==1 )
  if( gi.isInteractiveGraphicsOn() )
  {
    updateGeometry=false;
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = (MappedGrid&) gc[grid]; // cast away const
      if( mg.isRectangular() )  
	mg.update(MappedGrid::THEmask ); 
      else
	mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
    }

    computePlotBounds=false;
    getPlotBounds(gc,psp,xBound); // do later so we can defer creating the center *wdh* 030916
    gi.setGlobalBound(xBound);
  }
  else
  {
    xBound=0;  xBound(1,Range(0,2))=1.;  // set some default values
    ((GridCollection&)gc).update(MappedGrid::THEmask ); // *wdh* 041024
  }
  
  //  xBound.display("globalBound in countour3d");

  // ---- variables for contour planes ------
  // int numberOfContourPlanes=psp.numberOfContourPlanes; // **** wdh*** 020130
  int & numberOfContourPlanes=psp.numberOfContourPlanes;
  RealArray & contourPlane=psp.contourPlane;   // values 0:2 = normal, 3:5 = point on plane
  if( numberOfContourPlanes<0 )
  {
    // Define the default contour planes, one plane in each direction, through the
    // centre of the bounding box
    numberOfContourPlanes=3;
    contourPlane.redim(6,numberOfContourPlanes);
    contourPlane=0.;
    contourPlane(0,0)=1.; contourPlane(1,1)=1.; contourPlane(2,2)=1.;
  
    for( axis=0; axis<3; axis++ )
    { // planes go through the centre of the bounding box by default
      contourPlane(axis+3,nullRange)=.5*(xBound(Start,axis) + xBound(End,axis));
    }
  }

  // set default prompt
  gi.appendToTheDefaultPrompt("contour>");

   // plot this component by default:
  component = min(max(component,uGCF.getComponentBase(0)),uGCF.getComponentBound(0)); 
  int menuItem=-1;


  if( !minAndMaxContourLevelsSpecified(component) && recomputeVelocityMinMax )
  {
    // Get Bounds on u -- treat the general case when the component can be in any Index position of u
    recomputeVelocityMinMax=FALSE;
    getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
  }
  if( !psp.plotObjectAndExit )
    dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));

  ContourSurface *cs=NULL;  // holds the list of surfaces we plot (contour planes, coord surfaces, iso)
  bool recomputeContourPlanes=true;
  bool recomputeCoordinatePlanes=true;
  
  int maxNumberOfContourSurfaces=25;  // this may increase
  IntegerArray numberOfPolygonsPerSurface;
  IntegerArray numberOfVerticesPerSurface;
  
  int showNearbyValues=2; // For query values: show this many nearby values to left and right
  int showUnusedValues=0; // For query values: do no show values at mask=0 pts

  // We need a local value for adjustGridForDisplacementLocal since we don't want to change this
  // until the user exits.
  int adjustGridForDisplacementLocal = psp.adjustGridForDisplacement; 

  aString pickColour;
  SelectionInfo select; select.nSelect=0;
  int grid,len;

  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit";
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
      menuItem=gi.getAnswer(answer,"", select);
      gi.savePickCommands(true); // turn back on

#ifdef USE_PPP
      if( false && PlotIt::parallelPlottingOption==1 )
      {
	printf("Contour3d: after getAnswer: myid=%i select.active=%i select.nSelect=%i\n",myid,select.active,select.nSelect);
	fflush(0);
	MPI_Barrier(Overture::OV_COMM);
      }
#endif

    }
    

    // make sure the currentWindow is the same as on entry
    if( gi.getCurrentWindow() != startWindow )
      gi.setCurrentWindow(startWindow);

    if( answer=="plot contour lines (toggle)")
    {
      plotContourLines= !plotContourLines;
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
      // get Bounds on the grids
      // *wdh* 000317 getGridBounds(gc,psp, xBound);
      getPlotBounds(gc,psp, xBound);
      
    }
    else if( len=answer.matches("shaded surfaces") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotShadedSurface=value;
      dialog.setToggleState("shaded surfaces",plotShadedSurface);
    }
    else if( len=answer.matches("contour lines") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); plotContourLines=value;
      dialog.setToggleState("contour lines",plotContourLines);
    }
    else if( len=answer.matches("colour lines") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value); colourLineContours=value;
      dialog.setToggleState("colour lines",colourLineContours);
    }
    else if( answer=="plot shaded contours (toggle)" )
    {
      plotShadedSurface= !plotShadedSurface;
    }
    else if( answer=="wire frame (toggle)" )
    {
      plotWireFrame= !plotWireFrame;
    }
    else if(answer=="colour line contours (toggle)")
    {
      colourLineContours= !colourLineContours;
    }
    else if( dialog.getToggleValue(answer,"adjust grid for displacement",adjustGridForDisplacementLocal) )
    {
      printF("contour:INFO:You must exit the contour plotter and re-enter to see the grid adjusted or not for the displacement\n");
    } 
    else if( dialog.getTextValue(answer,"displacement scale factor","%e",psp.displacementScaleFactor) )
    {
      printF("INFO: You will have to exit this menu and replot to see the new displacement scale factor"
             " take effect. displacementScaleFactor=%9.3e\n",psp.displacementScaleFactor);
    }
    else if( answer=="line plots" )
    {

      int oldPBGG = gi.getPlotTheBackgroundGrid();
      int oldKAR  = gi.getKeepAspectRatio();
      const int componentOld=component;

      // plot solution on lines that cut through the 2/3-D grid
      contourCuts(gi, uGCF, psp );

      // Restore plotbackgroundgrid and keepAspectRatio after this call
      gi.setPlotTheBackgroundGrid(oldPBGG);
      
      psp.keepAspectRatio=oldKAR;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 

      // the bounding box is messed up (set for 1D) after this call
      gi.setGlobalBound(xBound);

      // erase the labels and replot them
      gi.eraseLabels(psp);

      // replot the 3D object
      component=componentOld;  // reset
      plotObject = TRUE;
      plotContours = TRUE;
    }
    else if( menuItem > chooseAComponentMenuItem && menuItem <= chooseAComponentMenuItem+numberOfComponents )
    {
      component=menuItem-chooseAComponentMenuItem-1 + uGCF.getComponentBase(0);
      // cout << "chose component number=" << component << endl;
      recomputeVelocityMinMax=true;
      recomputeContourPlanes=true;
    }
    else if( len=answer.matches("plot:") )
    {
      // plot a new component
      aString name = answer(len,answer.length()-1);
      int c=-1;
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( name==uGCF.getName(n) )
	{
	  c=n;
	  break;
	}
      }
      if( c==-1 )
      {
	printf("ERROR: unknown component name =[%s]\n",(const char*)name);
	c=0;
      }
      component=c;
      recomputeVelocityMinMax=true;
      recomputeContourPlanes=true;
      dialog.getOptionMenu("Plot component:").setCurrentChoice(component);

      if( !minAndMaxContourLevelsSpecified(component) )
      {
        // recompute the plot bounds if the user has not explicitly set the min and max values.
	getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
	recomputeVelocityMinMax=false;
	dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
      }
    }

    else if( len=answer.matches("component") )  // *wdh* 080710 -- old way, keep for backward compatibility **
    { // new way from dialog
      sScanF(answer(len,answer.length()-1),"%i",&component); 
      component=max(0,min(numberOfComponents-1,component));
      dialog.getOptionMenu(0).setCurrentChoice(component);       
      recomputeVelocityMinMax=true;
      recomputeContourPlanes=true;

      if( !minAndMaxContourLevelsSpecified(component) )
      {
        // recompute the plot bounds if the user has not explicitly set the min and max values.
	getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
	recomputeVelocityMinMax=false;
	dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
      }

    }
    else if( answer=="reset min max" )
    {
      getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
      recomputeVelocityMinMax=false;

      minAndMaxContourLevels(0,component)=uMin; // save levels for this component
      minAndMaxContourLevels(1,component)=uMax;
      minAndMaxContourLevelsSpecified(component)=false;

      dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
    }
    else if( len=answer.matches("min max") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&uMin,&uMax);
      dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));

      printF("New values are min = %e, max = %e \n",uMin,uMax);
      printF("Note that these values will only be applied to the current component\n");
      minAndMaxContourLevelsSpecified(component)=TRUE;
      minAndMaxContourLevels(0,component)=uMin; // save levels for this component
      minAndMaxContourLevels(1,component)=uMax;
    }
    else if( answer=="set plot bounds" )
    {
      RealArray & pb = parameters.plotBound;
      parameters.usePlotBounds=true;
      gi.inputString(answer,"Enter plot bounds to use xa,xb, ya,yb, za,zb");
      sScanF(answer,"%e %e %e %e %e %e\n",&pb(0,0),&pb(1,0),&pb(0,1),&pb(1,1),&pb(0,2),&pb(1,2));
      printF(" Using plot bounds = [%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
	     pb(0,0),pb(1,0),pb(0,1),pb(1,1),pb(0,2),pb(1,2));
    }
    else if( answer=="reset plot bounds" )
    {
      parameters.usePlotBounds=false;
    }
    else if( len=answer.matches("contour shift") )
    {
      sScanF(answer(len,answer.length()-1),"%e",&contourShift); 
      dialog.setTextLabel("contour shift",sPrintF(answer,"%.2g",contourShift));
    }
    else if( len=answer.matches("number of levels") )  // new way
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfContourLevels);
      dialog.setTextLabel("number of contour levels",sPrintF(answer,"%i",numberOfContourLevels));
      if( numberOfContourLevels<2 )
      {
	cout << "Error, number of contour levels must be greater than 1! \n";
	numberOfContourLevels=11;
      }
    }
    else if( len=answer.matches("ghost lines") )  // new way
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfGhostLinesToPlot);
      dialog.setTextLabel("ghost lines",sPrintF(answer,"%i",numberOfGhostLinesToPlot));
      getPlotBounds(gc,psp, xBound);

      if( !minAndMaxContourLevelsSpecified(component) )
      {
	recomputeVelocityMinMax=false;
	getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
      }

      recomputeContourPlanes=true; 
    }
    else if( answer=="base min/max on global values" ||
             answer=="base min/max on contour plane values" )
    {
      
      if( answer=="base min/max on global values" )
      {
        psp.contour3dMinMaxOption=GraphicsParameters::baseMinMaxOnGlobalValues;
        printF("INFO:The colour bar for plotting contour planes or coordinate planes will be based on the global values"
               " of the component being plotted\n");
	recomputeVelocityMinMax=true;
      }
      else if( answer=="base min/max on contour plane values" )
      {
	psp.contour3dMinMaxOption=GraphicsParameters::baseMinMaxOnContourPlaneValues;
        printF("INFO:The colour bar for plotting contour planes or coordinate planes will be based on the values"
               " found on the planes.\n");
      }
    }
    else if( answer=="pick colour..." )
    {
      colourDialog.showSibling();
      printF("INFO: setting picking option to `pick to colour surface'.\n");
      pickingOption=pickToColourSurface;
      gui.getOptionMenu("Pick to:").setCurrentChoice((int)pickingOption);

    }
    else if( answer=="close colour choices" )
    {
      colourDialog.hideSibling();
    }
    else if( PlotIt::getColour( answer,colourDialog,pickColour ) )
    {
      printF("contour3d: answer=%s was processed by the colourDialog\n",(const char*)answer);
      pickColourIndex=getXColour(pickColour);
      if( pickColourIndex==0 )
      {
	printf(" ERROR: colour=[%s] not recognized! using aquamarine instead\n",(const char*)pickColour);
	pickColourIndex=getXColour("aquamarine");
      }
    }
    else if( answer=="add contour planes" )
    {
      RealArray values;
      int extra=gi.getValues("Enter n1,n2,n3, x,y,z : normal and a point on the plane",values);
      extra/=6;
      if( extra>0 )
      {
	numberOfContourPlanes+=extra;
	contourPlane.resize(6,numberOfContourPlanes);
	if( numberOfContourPlanes==0 )
	  continue;
        values.resize(6*extra);
	values.reshape(6,extra);
        Range R(0,extra-1);
        contourPlane(Range(0,5),R+numberOfContourPlanes-extra)= values(Range(0,5),R);

        recomputeContourPlanes=true; // for now we just recompute all **fix this **
      }
    }
    else if( answer=="remove contour planes" )
    {
      IntegerArray values;
      int i;
      for( i=0; i<numberOfContourPlanes; i++ )
      {
	sPrintF(buff, "contour plane %i : normal=(%8.1e,%8.1e,%8.1e) point=(%8.1e,%8.1e,%8.1e)",
		i, contourPlane(0,i), contourPlane(1,i), contourPlane(2,i),
		contourPlane(3,i), contourPlane(4,i), contourPlane(5,i));
	gi.outputString(buff);
      }
      int numberToRemove=gi.getValues("Enter a list of planes to remove (enter `done' when finished)",values,0); // minimumvalue==0
      
      // *NOTE* this is not correct if invalid values are entered to be removed !! e.g. too large

      const int nr=numberToRemove;
      for( int i=0; i<nr; i++ )
      {
	if( values(i)<0 || values(i)>=numberOfContourPlanes )
	{
	  printF("contour:ERROR: invalid plane to remove = %i, should be in the range [%i,%i]\n",values(i),0,numberOfContourPlanes-1);
	  numberToRemove--;
	}
      }
      if( numberToRemove <= 0 )
      {
	gi.outputString("Nothing to remove!\n");
	continue;
      }
      

      int nc=numberOfContourPlanes;
      numberOfContourPlanes-=numberToRemove;
      if( numberOfContourPlanes<=0 )
      {
	contourPlane.redim(0);
      }
      else
      {
	int k=0;
	for( i=0; i<nc; i++ )
	{
	  if( min(abs(values-i))!=0 ) // keep plane i 
	  {
	    contourPlane(Range(0,5),k)=contourPlane(Range(0,5),i);
	    k++;
	  }
	}
	assert( k==numberOfContourPlanes );
	contourPlane.resize(Range(0,5),numberOfContourPlanes);

      }
      recomputeContourPlanes=true;  // for now we just recompute all **fix this **
    }
    else if( answer=="pick to delete contour planes" || 
             answer=="pick to add contour plane x" || 
             answer=="pick to add contour plane y" || 
             answer=="pick to add contour plane z" || 
	     answer=="pick to add boundary surface" ||
	     answer=="pick to delete boundary surface" ||
             answer=="pick to delete coordinate surface" ||
	     answer=="pick to add coordinate surface 1" ||
	     answer=="pick to add coordinate surface 2" ||
	     answer=="pick to add coordinate surface 3" ||
             answer=="pick off" ||
             answer=="pick to query value" ||
             answer=="pick to colour surface" )
    {
      pickingOption= (answer=="pick to delete contour planes"    ? pickToDeleteContourPlane :
		      answer=="pick to add contour plane x"      ? pickToAddContourPlaneX :
		      answer=="pick to add contour plane y"      ? pickToAddContourPlaneY :
		      answer=="pick to add contour plane z"      ? pickToAddContourPlaneZ :
                      answer=="pick to add boundary surface"     ? pickToAddBoundarySurface :
                      answer=="pick to delete boundary surface"  ? pickToDeleteBoundarySurface :
                      answer=="pick to delete coordinate surface"? pickToDeleteCoordinateSurface :
	              answer=="pick to add coordinate surface 1" ? pickToAddCoordinateSurface1 :
	              answer=="pick to add coordinate surface 2" ? pickToAddCoordinateSurface2 :
	              answer=="pick to add coordinate surface 3" ? pickToAddCoordinateSurface3 :
                      answer=="pick to query value"              ? pickToQueryValue :
                      answer=="pick to colour surface"           ? pickToColourSurface :
                        pickingOff);
      
      gui.getOptionMenu("Pick to:").setCurrentChoice((int)pickingOption);
    }
    else if( ( pickingOption==pickToColourSurface && (select.active || select.nSelect ) ) ||
             answer.matches("colour iso-surface") )
    {
      bool selectionMade=select.active || select.nSelect;
      PickingOptionEnum option=pickingOption;

      if( selectionMade )
      {
	printF("Look for the closest item picked...\n");
	for( int ns=0; ns<maxNumberOfContourSurfaces; ns++)
	{
	  if( cs[ns].getGlobalID()==select.globalID )
	  {
            cs[ns].colourIndex=pickColourIndex;
            cs[ns].surfaceColourType=ContourSurface::colourSurfaceByIndex;

	    aString colourName = getXColour(cs[ns].colourIndex);
	    printF("Contour surface %i chosen. Setting to colour=[%s] (pickColourIndex=%i)\n",ns,(const char*)colourName,pickColourIndex);

            // If this is an iso-surface -- find out which one so we can save the values
	    if( cs[ns].surfaceType==ContourSurface::isoSurface )
	    {
              int isoSurfaceNumber=-1;
	      for( int ms=0; ms<=ns; ms++ )
	      {
		if( cs[ms].surfaceType==ContourSurface::isoSurface )
		  isoSurfaceNumber++;
	      }
	      assert( isoSurfaceNumber>=0 && isoSurfaceNumber<numberOfIsoSurfaces );
	      
	      isoSurfaceValue(isoSurfaceNumber,1)=ContourSurface::colourSurfaceByIndex;
	      isoSurfaceValue(isoSurfaceNumber,2)=pickColourIndex;      

	      printF("pick: isoSurf %i : setting surfaceColourType=%i and colourIndex=%i\n",isoSurfaceNumber,
                     int(isoSurfaceValue(isoSurfaceNumber,1)+.5),int(isoSurfaceValue(isoSurfaceNumber,2)+.5));

              gi.outputToCommandFile(sPrintF(buff,"colour iso-surface %i %s\n",isoSurfaceNumber,
                                     (const char*)colourName));

	    }
	    break;
	  }
	}
      }
      else if( len=answer.matches("colour iso-surface") )
      {
	int isoSurfaceNumber=-1;
        sScanF(answer(len,answer.length()-1),"%i %s",&isoSurfaceNumber,buff);
	
        aString colourName = buff;
	if( isoSurfaceNumber>=0 && isoSurfaceNumber<numberOfIsoSurfaces )
	{
	  printF("Setting isosurface %i to colour [%s]\n",isoSurfaceNumber,(const char*)colourName);
	  isoSurfaceValue(isoSurfaceNumber,1)=ContourSurface::colourSurfaceByIndex;
	  isoSurfaceValue(isoSurfaceNumber,2)=pickColourIndex;      

	  if( cs!=NULL )
	  {
            // look for the iso surface in the cs[] array
	    int ns=-1, iso=-1;
	    for( int s=0; s<maxNumberOfContourSurfaces; s++ )
	    {
	      if( cs[s].surfaceType==ContourSurface::isoSurface )
	      {
		iso++;
		if( iso==isoSurfaceNumber )
		{
		  ns=s;
		  break;
		}
	      }
	    }	    
	    if( ns>=0 )
	    { // set the isosurface colour:
	      cs[ns].colourIndex=getXColour(colourName);
	      cs[ns].surfaceColourType=ContourSurface::colourSurfaceByIndex;
	    }
	  }
	  
// 	  // If this is an iso-surface -- find out which one so we can save the values
// 	  if( cs[ns].surfaceType==ContourSurface::isoSurface )
// 	  {
// 	    int isoSurfaceNumber=-1;
// 	    for( int ms=0; ms<=ns; ms++ )
// 	    {
// 	      if( cs[ms].surfaceType==ContourSurface::isoSurface )
// 		isoSurfaceNumber++;
// 	    }
// 	    assert( isoSurfaceNumber>=0 && isoSurfaceNumber<numberOfIsoSurfaces );
	      
// 	  }

	}
	else
	{
	  printF("ERROR: invalid isosurface number=%i. There are %i isosurfaces.\n",isoSurfaceNumber,
              numberOfIsoSurfaces);
	}
      }
    }
    else if( ((pickingOption==pickToAddBoundarySurface ||
               pickingOption==pickToDeleteBoundarySurface ||
	       pickingOption==pickToAddCoordinateSurface1 ||
	       pickingOption==pickToAddCoordinateSurface2 ||
	       pickingOption==pickToAddCoordinateSurface3 )  &&  (select.active || select.nSelect )) ||
	     answer.matches("add coordinate surface") ||
             answer.matches("add boundary surface") || 
             answer.matches("delete boundary surface") )
    {

      bool selectionMade=select.active || select.nSelect;
      PickingOptionEnum option=pickingOption;

      real xv[3]={select.x[0],select.x[1],select.x[2]};
      realArray r(1,3);

      int grid=-1, axis=-1, index=-1;
      if( selectionMade )
      {
	if( select.active && select.nSelect==0 )
	{
	  printF("pickToAddContourPlane: Point was picked but no item was selected\n");
	  continue;
	}
	printF(" Point chosen=(%8.1e,%8.1e,%8.1e)  \n",xv[0],xv[1],xv[2]);

	printF("Look for the closest grid picked...\n");
	for( int g=0; g<numberOfGrids; g++ )
	{
	  if( gc[g].getGlobalID()==select.globalID )
	  {
	    grid=g;
	    break;
	  }
	}
	if( grid<0 || grid>=numberOfGrids )
	{
	  printF("ERROR: no grid was picked (or no grid was found!)\n");
	  continue;
	}
      
	assert( grid>=0 && grid<numberOfGrids );
	const MappedGrid & mg=gc[grid];

	// Find where we are in coordinate space
	realArray x(1,3);
	x(0,0)=xv[0];
	x(0,1)=xv[1];
	x(0,2)=xv[2];
	r=-1.;
	mg.mapping().getMapping().inverseMap(x,r);
	if( max(fabs(r))>5. )
	{
	  printF("ERROR inverting pt=(%8.1e,%8.1e,%8.1e) on grid %i  \n",xv[0],xv[1],xv[2],grid);
	  continue;
	}
      
      }
      else if( len=answer.matches("add coordinate surface") )
      {
	sScanF(answer(len,answer.length()-1),"%i %i %i",&grid,&axis,&index);
        option=axis==0 ? pickToAddCoordinateSurface1 : axis==1 ? pickToAddCoordinateSurface2 : pickToAddCoordinateSurface3;
      }
      else if( len=answer.matches("add boundary surface") )
      {
	sScanF(answer(len,answer.length()-1),"%i %i %i",&grid,&side,&axis);
        option=pickToAddBoundarySurface;
      }
      else if( len=answer.matches("delete boundary surface") )
      {
	sScanF(answer(len,answer.length()-1),"%i %i %i",&grid,&side,&axis);
        option=pickToDeleteBoundarySurface;
      }
      else
      {
        printF("ERROR: unknown option! answer=%s\n",(const char*) answer);
	gi.stopReadingCommandFile();
	continue;
      }
      

      if( option==pickToAddBoundarySurface || option==pickToDeleteBoundarySurface )
      {
	if( selectionMade )
	{
	  // find the closest boundary
          printF(" Find closest bndry on grid=%i, r=(%8.2e,%8.2e,%8.2e)\n",grid,r(0,0),r(0,1),r(0,2));
	  
	  const real minDist=min( min(fabs(r)), min(fabs(r-1.)) ); // distance to closest boundary
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    if( fabs(r(0,dir)) <= minDist )
	    {
	      side=0, axis=dir; 
	      break;
	    }
	    else if( fabs(r(0,dir)-1.) <= minDist )
	    {
	      side=1, axis=dir;
	      break;
	    }
	  }
	}
	if( side<0 || side>1 || axis<0 || axis>2 )
	{
	  printF("contour3d:ERROR: invalid value for side or axis, (side,axis)=(%i,%i)\n",side,axis);
	  gi.stopReadingCommandFile();
	  continue;
	}
	
	plotContourOnGridFace(side,axis,grid)= option==pickToAddBoundarySurface;
	if( plotContourOnGridFace(side,axis,grid) )
	{
	  printF(" Plot contours on boundary of grid=%i (side,axis)=(%i,%i)\n",grid,side,axis);
          if( selectionMade ) gi.outputToCommandFile(sPrintF(answer,"add boundary surface %i %i %i \n",grid,side,axis));
	}
	else
	{
	  printF(" Delete contours on boundary of grid=%i (side,axis)=(%i,%i)\n",grid,side,axis);
          if( selectionMade ) gi.outputToCommandFile(sPrintF(answer,"delete boundary surface %i %i %i \n",grid,side,axis));
	}
	
      }
      else
      {
	// find closest coordinate plane
	if( grid<0 || grid>=numberOfGrids )
	{
	  printF("contour2d:addCoordinatePlane:ERROR:invalid grid=%i\n",grid);
	  continue;
	}
	if( selectionMade )
	{
	  assert( grid>=0 && grid<numberOfGrids );
	  const MappedGrid & mg=gc[grid];
	  axis= (pickingOption==pickToAddCoordinateSurface1 ? axis1 :
		 pickingOption==pickToAddCoordinateSurface2 ? axis2 : axis3);

	  index=int(r(0,axis)/mg.gridSpacing(axis)+mg.gridIndexRange(0,axis)+.5);
	  printF(" Adding coordinate surface: grid=%i axis=%i index=%i\n",grid,axis,index);
	
	  gi.outputToCommandFile(sPrintF(answer,"add coordinate surface %i %i %i \n",grid,axis,index));
	}

	if( numberOfCoordinatePlanes>=coordinatePlane.getLength(1) )
	{
	  coordinatePlane.resize(3,coordinatePlane.getLength(1)*2+5);
	}
	coordinatePlane(0,numberOfCoordinatePlanes)=grid;
	coordinatePlane(1,numberOfCoordinatePlanes)=axis;
	coordinatePlane(2,numberOfCoordinatePlanes)=index;
	numberOfCoordinatePlanes++;

      }
      recomputeContourPlanes=true; // for now we just recompute all **fix this **


    }
    else if( ((pickingOption==pickToAddContourPlaneX ||
	       pickingOption==pickToAddContourPlaneY ||
	       pickingOption==pickToAddContourPlaneZ )  &&  (select.active || select.nSelect )) ||
	     answer.matches("add contour plane")  )
    {
      real x[3]={select.x[0],select.x[1], select.x[2]}; //
      real n[3]={0.,0.,0.};
      if( len=answer.matches("add contour plane") )
      {
	sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e ",&n[0],&n[1],&n[2],&x[0],&x[1],&x[2]);
      }
      else
      {
	if( select.active && select.nSelect==0 )
	{
	  printF("pickToAddContourPlane: Point was picked but no item was selected\n");
	  continue;
	}
	printF(" Point chosen=(%8.1e,%8.1e,%8.1e)  \n",x[0],x[1],x[2]);

	int dir=pickingOption==pickToAddContourPlaneX ? 0 :
	  pickingOption==pickToAddContourPlaneY ? 1 : 2;
      
	n[dir]=1.;
	
      }
      
      printF(" Adding contour plane %i with normal=(%8.1e,%8.1e,%8.1e) point=(%8.1e,%8.1e,%8.1e)  \n",
	     numberOfContourPlanes,n[0],n[1],n[2],x[0],x[1],x[2]);

      gi.outputToCommandFile(sPrintF(answer,"add contour plane %12.5e %12.5e %12.5e %12.5e %12.5e %12.5e \n",
                     n[0],n[1],n[2],x[0],x[1],x[2]));

      RealArray values(6);

      values(0)=n[0];
      values(1)=n[1];
      values(2)=n[2];
      values(3)=x[0];
      values(4)=x[1];
      values(5)=x[2];
      
      int extra=1;
      if( extra>0 )
      {
	numberOfContourPlanes+=extra;
	contourPlane.resize(6,numberOfContourPlanes);
	if( numberOfContourPlanes==0 )
	  continue;
        values.resize(6*extra);
	values.reshape(6,extra);
        Range R(0,extra-1);
        contourPlane(Range(0,5),R+numberOfContourPlanes-extra)= values(Range(0,5),R);

        recomputeContourPlanes=true; // for now we just recompute all **fix this **
      }

    }
    else if( (pickingOption==pickToDeleteContourPlane  &&  (select.active || select.nSelect )) ||
             answer.matches("delete contour plane") )
    {
#ifdef USE_PPP
      if( false && PlotIt::parallelPlottingOption==1 )
      {
	printf("*** delete contour plane: START myid=%i ***\n",myid);
	fflush(0);
	MPI_Barrier(Overture::OV_COMM);
      }
#endif

      int iPlane=-1;
      if( len=answer.matches("delete contour plane") )
      {
        sScanF(answer(len,answer.length()-1),"%i",&iPlane);
      }
      else
      {
	// point selected with the mouse

	if( select.active && select.nSelect==0 )
	{
	  printF("pickToDeleteContourPlane: Point was picked but no item was selected\n");
	  continue;
	}
      

	real x[3]={select.x[0],select.x[1], select.x[2]}; //

	printF(" Point chosen=(%8.1e,%8.1e,%8.1e)  \n",x[0],x[1],x[2]);

	real distMin=REAL_MAX;
        real tol=.1*(1.+fabs(x[0])+fabs(x[1])+fabs(x[2]));
	int i;
	for( i=0; i<numberOfContourPlanes; i++ )
	{
	  real dist=fabs(contourPlane(0,i)*(x[0]-contourPlane(3,i))+
			 contourPlane(1,i)*(x[1]-contourPlane(4,i))+
			 contourPlane(2,i)*(x[2]-contourPlane(5,i)));

	  printF(" plane %i: `dist'=%8.2e \n",i,dist);
	
	  if( dist<distMin && dist<tol)
	  {
	    iPlane=i;
	    distMin=dist;
	  }
	}
        gi.outputToCommandFile(sPrintF(answer,"delete contour plane %i\n",iPlane));
      }
      
      if( iPlane<0 || iPlane>=numberOfContourPlanes )
      {
	if( numberOfContourPlanes==0 )
	  printF("contour:ERROR:delete contour plane: there are no planes to delete\n");
        else
	  printF("contour:ERROR:delete contour plane: invalid plane=%i, should be in the range [%i,%i]\n",iPlane,0,numberOfContourPlanes-1);
	continue;
      }
      else if( iPlane>=0 )
      {
        printF("Deleting plane %i: normal=(%8.1e,%8.1e,%8.1e) point=(%8.1e,%8.1e,%8.1e)\n",
	       iPlane,contourPlane(0,iPlane), contourPlane(1,iPlane), contourPlane(2,iPlane),
               contourPlane(3,iPlane), contourPlane(4,iPlane), contourPlane(5,iPlane));

	int numberToRemove=1;
	IntegerArray values(numberToRemove);
	values=iPlane;
	
	int nc=numberOfContourPlanes;
	numberOfContourPlanes-=numberToRemove;
	if( numberOfContourPlanes<=0 )
	{
	  contourPlane.redim(0);
	}
	else
        {
	  int k=0;
	  for( i=0; i<nc; i++ )
	  {
	    if( min(abs(values-i))!=0 )
	    {
	      contourPlane(Range(0,5),k)=contourPlane(Range(0,5),i);
	      k++;
	    }
	  }
	  assert( k==numberOfContourPlanes );
	  contourPlane.resize(Range(0,5),numberOfContourPlanes);

	}
	recomputeContourPlanes=true;  // for now we just recompute all **fix this **
      }
    }
    else if( answer=="choose contour planes" )
    {
      RealArray values;
      numberOfContourPlanes=gi.getValues("Enter n1,n2,n3, x,y,z : normal and a point on the plane",values);
      numberOfContourPlanes/=6;
      contourPlane.redim(6*numberOfContourPlanes);
      if( numberOfContourPlanes==0 )
        continue;
      
      Range R(0,6*numberOfContourPlanes-1);
      contourPlane(R)=values(R);
      contourPlane.reshape(6,numberOfContourPlanes);

      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( answer=="+shift contour planes" )
    {
      printF("Shifting each plane by contourShift*normal (note: normal is not normalized)\n");
      contourPlane(Range(3,5),nullRange)+=contourShift*contourPlane(Range(0,2),nullRange);
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( answer=="-shift contour planes" )
    {
      printF("Shifting each plane by contourShift*normal. (note: normal is not normalized)\n");
      contourPlane(Range(3,5),nullRange)-=contourShift*contourPlane(Range(0,2),nullRange);
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( (pickingOption==pickToDeleteCoordinateSurface  &&  (select.active || select.nSelect )) ||
             answer.matches("delete coordinate plane") ||
             answer.matches("delete grid boundary") )
    {
      int iPlane=-1;
      int sideFace=-1, axisFace=-1, gridFace=-1;
      if( len=answer.matches("delete coordinate plane") )
      {
        sScanF(answer(len,answer.length()-1),"%i",&iPlane);
      }
      else if( len=answer.matches("delete grid boundary") )
      {
        sScanF(answer(len,answer.length()-1),"%i %i %i",&sideFace,&axisFace,&gridFace);
      }
      else
      {
	// point selected with the mouse

	if( select.active && select.nSelect==0 )
	{
	  printF("pickToDeleteContourPlane: Point was picked but no item was selected\n");
	  continue;
	}
      

	real x[3]={select.x[0],select.x[1],select.x[2]}; //

	printF(" Point chosen=(%8.1e,%8.1e,%8.1e)  \n",x[0],x[1],x[2]);

	real distMin=REAL_MAX;
        real tol=.5; 
	int i;
        RealArray rcp(1,3), xcp(1,3);
	xcp(0,0)=x[0]; xcp(0,1)=x[1]; xcp(0,2)=x[2];
	
	// *** first check how close we are to any boundary faces that are plotted ***
	for( int grid=0; grid<gc.numberOfGrids(); grid++ )
	{
	  ForBoundary(side,axis)
	  {
	    if( plotContourOnGridFace(side,axis,grid) )
	    {
	      MappedGrid & mg = (MappedGrid&)gc[grid];

              #ifdef USE_PPP
	        mg.mapping().inverseMapS(xcp,rcp);
              #else
	        mg.mapping().inverseMap(xcp,rcp);
              #endif
	  
	  
	      real dist=fabs( rcp(axis)/mg.gridSpacing(axis)+mg.gridIndexRange(0,axis)- mg.gridIndexRange(side,axis) );
	  
	      printF(" distance to grid boundary (side,axis,grid) is =%8.2e \n",side,axis,grid,dist);
	
	      if( dist<distMin && dist<tol )
	      {
                sideFace=side; axisFace=axis; gridFace=grid;
		distMin=dist;
	      }
	    }
	  }
	}
	
	// *** Now check how close we are to any coordinate planes ***
	for( i=0; i<numberOfCoordinatePlanes; i++ )
	{
          const int grid = coordinatePlane(0,i);
          const int dir  = coordinatePlane(1,i);
          const int index= coordinatePlane(2,i);

          assert( grid>=0 && grid<gc.numberOfComponentGrids() );
          MappedGrid & mg = (MappedGrid&)gc[grid];
          #ifdef USE_PPP
	    mg.mapping().inverseMapS(xcp,rcp);
          #else
	    mg.mapping().inverseMap(xcp,rcp);
          #endif
	  
	  
	  real dist=fabs(rcp(dir)/mg.gridSpacing(dir)+mg.gridIndexRange(0,dir)-index);
	  
	  printF(" plane %i: `dist'=%8.2e \n",i,dist);
	
	  if( dist<distMin && dist<tol )
	  {
	    iPlane=i;
	    distMin=dist;
	  }
	}
      }

      if( iPlane>=0 )
      {
        printF("Deleting plane %i: grid=%i axis=%i index=%i \n",
               iPlane,coordinatePlane(0,iPlane),coordinatePlane(1,iPlane),coordinatePlane(2,iPlane));
	
	
        gi.outputToCommandFile(sPrintF(answer,"delete coordinate plane %i\n",iPlane));

	int numberToRemove=1;
	IntegerArray values(numberToRemove);
	values=iPlane;
	
	int nc=numberOfCoordinatePlanes;
	numberOfCoordinatePlanes-=numberToRemove;
	if( numberOfCoordinatePlanes<=0 )
	{
	  coordinatePlane.redim(0);
	}
	else
        {
	  int k=0;
	  Range C=coordinatePlane.dimension(0);
	  for( i=0; i<nc; i++ )
	  {
	    if( min(abs(values-i))!=0 )
	    {
	      coordinatePlane(C,k)=coordinatePlane(C,i);
	      k++;
	    }
	  }
	  assert( k==numberOfCoordinatePlanes );
	  coordinatePlane.resize(Range(0,5),numberOfCoordinatePlanes);

	}
	recomputeCoordinatePlanes=true;  // for now we just recompute all **fix this **

      }
      else if( sideFace>=0 )
      {
        printF("Turn off grid boundary (side,axis,grid)=(%i,%i,%i)\n",sideFace,axisFace,gridFace);
	plotContourOnGridFace(sideFace,axisFace,gridFace)=false;

	gi.outputToCommandFile(sPrintF(answer,"delete grid boundary %i %i %i\n",sideFace,axisFace,gridFace));

	recomputeCoordinatePlanes=true;  // for now we just recompute all **fix this **
      }
      

    }
    else if( pickingOption==pickToQueryValue && 
	     (select.active || select.nSelect || answer.matches("query value") ) )
    {
      RealArray x0(1,3);
      if( len=answer.matches("query value") )
      {
	sScanF(answer(len,answer.length()-1),"%e %e %e",&x0(0,0),&x0(0,1),&x0(0,2));
      }
      else
      {
	x0(0,0) = select.x[0];
	x0(0,1) = select.x[1];
	x0(0,2) = select.x[2];
      }
      
      gi.outputToCommandFile(sPrintF(answer,"query value %e %e %e\n",x0(0,0),x0(0,1),x0(0,2)));

      realCompositeGridFunction & ucg = (realCompositeGridFunction &)uGCF;

      displayValuesAtAPoint(x0,ucg,numberOfComponents,component,showNearbyValues,showUnusedValues);
            	  
    }
    else if( answer.matches("query grid point") )
    {
      int grid=-1;
      int iv[3], &i1 = iv[0], &i2=iv[1], &i3=iv[2];
      i1=0; i2=0; i3=0;
      
      printF("\n ----------------------------------------------------------------------------------\n");
      if( len=answer.matches("query grid point") )
      {
	sScanF(answer(len,answer.length()-1),"%i %i %i %i",&grid,&i1,&i2,&i3);
        dialog.setTextLabel("query grid point",sPrintF("%i %i %i %i (grid, i1,i2,i3)",grid,i1,i2,i3));

        if( grid<0 || grid>=gc.numberOfComponentGrids() )
	{
	  printF("ERROR:query a grid point: grid=%i is not valid.\n",grid);
	  continue;
	}
	
      }
      else // old -- not used any more:
      {
        bool found=false;
	for (int i=0; i<select.nSelect && !found ; i++)
	{
	  for( int g=0; g<numberOfGrids; g++ )
	  {
	    // if( gc[g].getGlobalID()==select.globalID )
	    if( gc[g].getGlobalID()==select.selection(i,0) )
	    {
	      found=true;
	      grid=g;
	    
	      // Find the closest grid point
	      realArray x(1,3),r(1,3);
	      x(0,0)=select.x[0];
	      x(0,1)=select.x[1];
	      x(0,2)=select.x[2];
	      r=-1.;
	      gc[grid].mapping().getMapping().inverseMap(x,r);
	    
//  	    printF("selection: x=(%8.2e,%8.2e,%8.2e) grid=%i r=(%8.2e,%8.2e,%8.2e)\n",
//  		   x(0,0),x(0,1),x(0,2),grid,r(0,0),r(0,1),r(0,2));
	    
	      printF(" Point x=(%9.3e,%9.3e,%9.3e) lies on grid=%i (name=%s) at r=(%9.3e,%9.3e,%9.3e)\n",
		     x(0,0),x(0,1),x(0,2),grid,(const char*)gc[grid].getName(),r(0,0),r(0,1),r(0,2) );

	      MappedGrid & mg = (MappedGrid&)gc[grid];
	      // find the closest grid point
	      i3=0;
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {
		real dr = mg.gridSpacing(dir);
		iv[dir]=int(r(0,dir)/dr+20.5)-20+ mg.gridIndexRange(0,dir); // watch out for negative r
		iv[dir]=max(mg.dimension(0,dir),min(mg.dimension(1,dir),iv[dir]));
	      }
	      printF(" The closest grid point is (i1,i2,i3)=(%i,%i,%i)\n",i1,i2,i3);

	      break;
	    }
	  }
	}
      }
      if( grid<0 || grid>=gc.numberOfComponentGrids() )
      {
	printF("ERROR:query a grid point: grid=%i is not valid.\n",grid);
	continue;
      }
      MappedGrid & mg = (MappedGrid&)gc[grid];
      const intArray & mask = mg.mask();

      if( i1>=mg.dimension(0,0) && i1<=mg.dimension(1,0) &&
	  i2>=mg.dimension(0,1) && i2<=mg.dimension(1,1) &&
	  i3>=mg.dimension(0,2) && i3<=mg.dimension(1,2) )
      {
        gi.outputToCommandFile(sPrintF("query grid point %i %i %i %i\n",grid,i1,i2,i3));


        real xv[3] = {0.,0.,0.};  //
	if( mg.isRectangular() )  
	{
	  real dx[3],xab[2][3];
	  mg.getRectangularGridParameters( dx, xab );
          for( axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=xab[0][axis] + (iv[axis]-mg.gridIndexRange(0,axis))*dx[axis];
	}
	else
	{
          for( axis=0; axis<numberOfDimensions; axis++ )
	    xv[axis]=mg.vertex()(i1,i2,i3,axis);
	}
	

        printF("*** Solution values on grid=%i point=(%i,%i,%i) x=(%9.3e,%9.3e,%9.3e) (mask=%i) are ****\n",
                   grid,i1,i2,i3,xv[0],xv[1],xv[2],mask(i1,i2,i3));
	for( int c=0; c<numberOfComponents; c++ )
	{
	  printF(" u(%i,%i,%i,%i)=%14.8e  (%s) \n", i1,i2,i3,c, uGCF[grid](i1,i2,i3,c),
		 (const char*)uGCF.getName(c));
	      
	}
        #ifndef USE_PPP
        aString pickedPointColour="yellow";
        real pickedPointSize=8.;

	real pointSize;
	psp.get(GI_POINT_SIZE,pointSize);
        psp.set(GI_POINT_SIZE,pickedPointSize*gi.getLineWidthScaleFactor());      // point size in pixels
	psp.set(GI_POINT_COLOUR,pickedPointColour); 

        realArray pickedPoint(1,3);
        pickedPoint=0.;
	for( axis=0; axis<numberOfDimensions; axis++ )
	  pickedPoint(0,axis)=xv[axis]; 

        gi.plotPoints(pickedPoint,psp);

	psp.set(GI_POINT_SIZE,(real)pointSize);      // reset
        #endif
	
      }
      else
      {
        printF("ERROR:query a grid point: (i1,i2,i3,grid)=(%i,%i,%i,%i) is not valid\n",i1,i2,i3,grid);
      }
      printF(" ----------------------------------------------------------------------------------\n");
    }
    else if( len=answer.matches("xScale, yScale, zScale") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&psp.xScaleFactor,&psp.yScaleFactor,&psp.zScaleFactor);
      printF("New values are xScale = %g, yScale = %g, zScale = %g \n",psp.xScaleFactor,psp.yScaleFactor,
              psp.zScaleFactor);
      dialog.setTextLabel("xScale, yScale, zScale",sPrintF(answer,"%g %g %g",psp.xScaleFactor,
                          psp.yScaleFactor,psp.zScaleFactor));
    }
    else if( answer.matches("iso-surface values") ||
             answer=="iso-surface" )
    {
      int oldNumber=numberOfIsoSurfaces; // isoSurfaceValue.getLength(0);
      
      if( len=answer.matches("iso-surface values") )
      {
        const int maxNumberOfValues=10;
        real v[maxNumberOfValues]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
        sScanF(answer(len,answer.length()-1),"%i %e %e %e %e %e %e %e %e %e ",&numberOfIsoSurfaces,
              &v[0],&v[1],&v[2],&v[3],&v[4],&v[5],&v[6],&v[7],&v[8],&v[9]);
        if( numberOfIsoSurfaces>maxNumberOfValues )
	{
          printF("WARNING:Too many iso-surfaces, only %i iso-surfaces will be plotted.\n",maxNumberOfValues);
	}
	
        numberOfIsoSurfaces=max(0,min(maxNumberOfValues,numberOfIsoSurfaces));
	
        aString label; // label for text entry in the dialog
        sPrintF(label,"%i ",numberOfIsoSurfaces);

        isoSurfaceValue.redim(numberOfIsoSurfaces,3);
	for(i=0; i<numberOfIsoSurfaces; i++)
	{
          isoSurfaceValue(i,0)=v[i];
          isoSurfaceValue(i,1)=ContourSurface::colourSurfaceDefault;  // isoSurfaceColourType
          isoSurfaceValue(i,2)=0;                                     // isoSurfaceColourIndex
	  
	  label = label + sPrintF(answer2,"%8.2e ",v[i]);
	}
	label = label + "(num, value1, value2, ...)";
	
        dialog.setTextLabel("iso-surface values",label);
	
        // dialog.setTextLabel("iso-surface values",sPrintF(answer2,"%i %8.2e %8.2e (num, value1, value2, ...)",0,uMin,uMax));
      }
      else
      {
	gi.inputString(answer2,sPrintF(buff,"Enter the number of different iso-surfaces (current=%i)",numberOfIsoSurfaces));
	sScanF(answer2,"%i",&numberOfIsoSurfaces);
	numberOfIsoSurfaces=max(0,numberOfIsoSurfaces);
	if( numberOfIsoSurfaces>0 )
	{
	  isoSurfaceValue.redim(numberOfIsoSurfaces,3);
	  for(i=0; i<numberOfIsoSurfaces; i++)
	  {
	    isoSurfaceValue(i,0)=0.;                                    // default value
	    isoSurfaceValue(i,1)=ContourSurface::colourSurfaceDefault;  // isoSurfaceColourType
	    isoSurfaceValue(i,2)=0;                                     // isoSurfaceColourIndex
            
	    if( uMax>uMin )
	      gi.inputString(answer2,sPrintF(buff,"Enter the value for iso-surface %i (min=%e,max=%e)\n",i,uMin,uMax));
	    else
	      gi.inputString(answer2,sPrintF(buff,"Enter the value for iso-surface %i \n",i));
	    sScanF(answer2,"%e",&isoSurfaceValue(i,0));
	  }
	}
      }
      
      if( cs!=NULL )
      {
        // Here we add the new iso-surfaces to the list of ContourSurfaces
        //  In this case we do not need to recompute other existing iso-surfaces

        // first make all existing iso surfaces inactive ** we could look for old iso-values that are the same as new
        Range all;
        for( int s=0; s<maxNumberOfContourSurfaces; s++ )
	{
	  if( cs[s].surfaceType==ContourSurface::isoSurface )
	  {
	    cs[s].destroy();
	    numberOfPolygonsPerSurface(s)=0;
	    numberOfVerticesPerSurface(s,all)=0;
	  }
	}
	
	for( int i=0; i<numberOfIsoSurfaces; i++ )
	{
	  int ns=-1;
	  for( int s=0; s<maxNumberOfContourSurfaces; s++ )
	  {
	    if( cs[s].surfaceStatus==ContourSurface::inactive )
	    {
	      ns=s;
	      break;
	    }
	  }
	  if( ns>=0 )
	  {
	    cs[ns].surfaceStatus=ContourSurface::notBuilt;
	    cs[ns].surfaceType=ContourSurface::isoSurface;
	    cs[ns].value=isoSurfaceValue(i,0);
	  }
	  else
	  {
	    printF("ERROR: no inactive ContourSurface found. **fix this Bill***\n");
	    recomputeContourPlanes=true;
	    break;
	  }
	}
        int totalNumberOfSurfaces=numberOfPolygonsPerSurface.getLength(0)+numberOfIsoSurfaces-oldNumber;
	numberOfPolygonsPerSurface.resize(totalNumberOfSurfaces); 
	numberOfVerticesPerSurface.resize(totalNumberOfSurfaces,numberOfVerticesPerSurface.getLength(1));

      }
      else
      {
	recomputeContourPlanes=true;
      }
    
    }
    else if( answer=="user defined output" )
    {
      printf("Calling the function userDefinedOutput (by default in Overture/Ogshow/userDefinedOutput.C)\n"
             "See the comments in userDefinedOutput.C for how to output results in your favourite format\n");
      
      PlotIt::userDefinedOutput(uGCF,psp,"contour");
    }
    else if( answer=="turn on grid boundaries" ||
             answer=="grid boundaries" ) // backward compatibility
    {
      plotContoursOnGridBoundaries=true;
      plotContourOnGridFace=false;
      for( grid=0; grid<numberOfBaseGrids; grid++ ) 
      {
	ForBoundary(side,axis)
	{
	  if( gc[grid].boundaryCondition()(side,axis)>0 )
	    plotContourOnGridFace(side,axis,grid)=true;
	}
      }
      printF("INFO: Choose `plot contours on grid boundaries' to toggle some boundaries on or off\n");
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( answer=="turn off grid boundaries" )
    {
      plotContoursOnGridBoundaries=false;
      plotContourOnGridFace=false;

      printF("INFO: Choose `plot contours on grid boundaries' to toggle some boundaries on or off\n");
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( answer=="plot contours on grid boundaries" )
    {
      aString *menu2 = new aString[numberOfGrids*6+3];
      for(;;)
      {
	i = 0;
	menu2[i++]="!Choose grid boundary";
        // only assign base grid faces here -- set AMR faces below automatically 
	for( grid=0; grid<numberOfBaseGrids; grid++ ) 
	{
	  ForBoundary(side,axis)
	    if( gc[grid].boundaryCondition()(side,axis)>0 )
	      menu2[i++]=sPrintF(buff,"(%i,%i,%i) = (%s,side,axis) %s",grid,side,axis,
                                 (const char*)gc[grid].mapping().getName(Mapping::mappingName),
				 plotContourOnGridFace(side,axis,grid)==TRUE ? "(on)" : "(off)");
	}
	menu2[i++]="exit"; 
	menu2[i]="";   // null string terminates the menu

        gi.getMenuItem(menu2,answer2, "Choose grid boundary>");

        if( answer2=="exit" )
          break;
        if( sScanF(answer2,"(%i %i %i)",&grid,&side,&axis)==3 )
          plotContourOnGridFace(side,axis,grid)=!plotContourOnGridFace(side,axis,grid);
        else
          cout << "ERROR: unknown response: [" << answer2 << "]\n";
      }
      delete [] menu2;
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if( answer=="plot contours on coordinate planes" )
    {
      printF(" grid   coordinate   starting      ending       grid\n");
      printF("        direction    grid index    grid index   name\n");
      for( grid=0; grid<numberOfGrids; grid++ )
      {
        for( int axis=0; axis<3; axis++ )
	{
	  printF(" %6i    %1i      %5i       %7i        %s \n",grid,axis,gc[grid].indexRange()(Start,axis),
                 gc[grid].indexRange()(End,axis),(const char *)gc[grid].mapping().getName(Mapping::mappingName));
	}
      }
      numberOfCoordinatePlanes=0;
      coordinatePlane=0;
      for(;;)
      {
	gi.inputString(answer2,sPrintF(buff,"Enter grid, coordinate direction and grid index (enter -1 finish)"));
	if( answer2 !="" && answer2!=" ")
	{
          int dir,index;
	  sScanF(answer2,"%i %i %i",&grid,&dir,&index);
	  if( grid<0 )
            break;
	  if( grid>=numberOfGrids )
	  {
	    printF("Error, the grid number=%i must be in the range [0,%i]\n",grid,numberOfGrids-1);
	  }
          else if( dir<0 || dir>2 )
	  {
	    printF("Error, the coordinate direction=%i must be 0,1, or 2 \n",dir);
	  }
	  else if( index<gc[grid].indexRange()(Start,dir) || index>gc[grid].indexRange()(End,dir) )
	  {
	    printF("Error, index=%i should be in the range [%i,%i] \n",index,gc[grid].indexRange()(Start,dir),
		   gc[grid].indexRange()(End,dir));
	  }
	  else
	  {
            if( numberOfCoordinatePlanes>=coordinatePlane.getLength(1) )
	    {
	      coordinatePlane.resize(3,coordinatePlane.getLength(1)*2+5);
	    }
	    coordinatePlane(0,numberOfCoordinatePlanes)=grid;
	    coordinatePlane(1,numberOfCoordinatePlanes)=dir; 
	    coordinatePlane(2,numberOfCoordinatePlanes)=index;
            numberOfCoordinatePlanes++;
	  }
	}
        else
	  break;
      }
      recomputeContourPlanes=true; // for now we just recompute all **fix this **
    }
    else if(answer=="number of contour levels")
    {
      gi.outputString(sPrintF(buff,"Enter the number of contour levels (current=%i)",
	   	   numberOfContourLevels));
      gi.inputString(answer2); // ,"Enter plot bounds: xMin xMax yMin yMax");
      if( answer2 !="" && answer2!=" ")
      {
	for( i=0; i<answer2.length() && i<80; i++)
	  buff[i]=answer2[i];
	sScanF(buff,"%i",&numberOfContourLevels);
        if( numberOfContourLevels<0 )
	{
	  cout << "Error, number of contour levels must be positive! \n";
	  numberOfContourLevels=11;
	}
      }
    }
    else if( answer=="set min and max" )
    {
      if( !minAndMaxContourLevelsSpecified(component) && recomputeVelocityMinMax )
      {
        // Get Bounds on u -- treat the general case when the component can be in any Index position of u
        recomputeVelocityMinMax=FALSE;
        getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
      }
      printF("Set min and max for contours, current=(%e,%e) \n"
             "Enter a blank line to use actual values computed from the grid function",uMin,uMax);
      gi.inputString(answer2,sPrintF(buff,"Enter min, max contour values (current=%9.2e,%9.2e)",uMin,uMax));
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e %e",&uMin,&uMax);
        printF("New values are min = %e, max = %e \n",uMin,uMax);
        printF("Note that these values will only be applied to the current component\n");
        minAndMaxContourLevelsSpecified(component)=TRUE;
        minAndMaxContourLevels(0,component)=uMin; // save levels for this component
        minAndMaxContourLevels(1,component)=uMax;
      }
      else
        minAndMaxContourLevelsSpecified(component)=FALSE;
    }
    else if( answer=="set minimum contour spacing" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the minimum contour spacing (current=%e)",minimumContourSpacing));
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e ",&minimumContourSpacing);
        printF("New minimum contour spacing = %e\n",minimumContourSpacing);
      }
    }
    else if( answer=="2D contours on coordinate planes" )
    {
      grid=0;       // this grid
      int normalAxis=2; // plot plane normal to this axis
      if( numberOfGrids>1 )
      {
        gi.inputString(answer2,"Enter the component grid to plot coordinate planes on");
        if( answer2!="" && answer2!=" ")
  	  sscanf(answer2,"%i ",&grid);
      }
      gi.inputString(answer2,sPrintF(buff,"Enter the direction normal to the planes (0,1,2) (default=%i)",normalAxis));
      if( answer2!="" && answer2!=" ")
  	sscanf(answer2,"%i ",&normalAxis);
      if( normalAxis<0 || normalAxis>2 )
      {
	gi.outputString("Error, the normal direction must be 0,1, or 2 -- setting to 2...");
        normalAxis=2;
      }
      SquareMapping square;
      Index I1,I2,I3;
      getIndex(gc[grid].gridIndexRange(),I1,I2,I3);
      for( int axis=0; axis<=1; axis++ )
      {
        int dir = (normalAxis + axis +1) % 3;
        square.setGridDimensions(axis,gc[grid].gridIndexRange()(End,dir)-gc[grid].gridIndexRange()(Start,dir)+1);
      }
      
      MappedGrid mg(square);

      // mg.isAllCellCentered() =gc[grid].isAllCellCentered() ;
      // mg.isAllVertexCentered()=gc[grid].isAllVertexCentered();
      if( gc[grid].isAllCellCentered() )
        mg.changeToAllCellCentered();                   // make a cell centered grid

      //mg.gridSpacing()=gc[grid].gridSpacing();
      //mg.isCellCentered()=gc[grid].isCellCentered();
      mg->discretizationWidth=gc[grid].discretizationWidth();
      mg->isPeriodic=gc[grid].isPeriodic();
      //mg.minimumEdgeLength=gc[grid].minimumEdgeLength;
      //mg.maximumEdgeLength=gc[grid].maximumEdgeLength;
      mg->boundaryCondition=gc[grid].boundaryCondition();
      mg->boundaryDiscretizationWidth=gc[grid].boundaryDiscretizationWidth();
      mg->sharedBoundaryFlag=gc[grid].sharedBoundaryFlag();
      mg->sharedBoundaryTolerance=gc[grid].sharedBoundaryTolerance();
      //mg.gridIndexRange()=gc[grid].gridIndexRange();
      //mg.indexRange()=gc[grid].indexRange();
      //mg.numberOfGhostPoints()=gc[grid].numberOfGhostPoints();
      //mg.discretizationWidth()=gc[grid].discretizationWidth();
      //mg.dimension()=gc[grid].dimension();

      mg.update(MappedGrid::THEmask); 
      // make "components" be all the planes 
      //      mg.mask()(I1,I2,0)=gc[grid].mask()(I1,I2,0);  // ***  this won't work ***  need different mask's
      Range C = (normalAxis==2) ? I3 : ( normalAxis==0 ? I1 : I2 );
      Range all;
      realMappedGridFunction u2D(mg,all,all,all,C);
      u2D.setName("u");
      if( normalAxis==2 )
      {
        mg.mask()(I1,I2,0)=1; 
	for( i=C.getBase(); i<=C.getBound(); i++ )
	{
	  u2D(I1,I2,0,i)=uGCF[grid](I1,I2,i,component);
	  u2D.setName(sPrintF(buff,"%s, plane i2=%i",(const char*)uGCF.getName(component),i),i);
	}
      }
      else if( normalAxis==0 )
      {
        mg.mask()(I2,I3,0)=1; 
	for( i=C.getBase(); i<=C.getBound(); i++ )
	{
          for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
            for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	      u2D(i2,i3,0,i)=uGCF[grid](i,i2,i3,component);
	  u2D.setName(sPrintF(buff,"%s, plane i0=%i",(const char*)uGCF.getName(component),i),i);
	}
      }
      else
      {
        mg.mask()(I3,I1,0)=1; 
	for( i=C.getBase(); i<=C.getBound(); i++ )
	{
          for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
            for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      u2D(i3,i1,0,i)=uGCF[grid](i1,i,i3,component);
	  u2D.setName(sPrintF(buff,"%s, plane i1=%i",(const char*)uGCF.getName(component),i),i);
	}
      }
      gi.erase();
      gi.outputString("Different planes will appear as different components in the 2D contour plot");
      
       // save the component number, which might get out of bounds inside contour
      int oldComponent = component;

      contour(gi,u2D,psp);

      // restore the component value
      component = oldComponent;
      
      // the bounding box is messed up (set for 2D) after this call
      gi.setGlobalBound(xBound);

      // replot the 3D object
      plotObject = TRUE;
      plotContours = TRUE;
    }
    else if( answer=="toggle grids on and off" )
    {
      aString answer2;
      aString *menu2 = new aString[numberOfGrids+2];
      for(;;)
      {
	for( grid=0; grid<numberOfGrids; grid++ )
	{
	  menu2[grid]=sPrintF(buff,"%i : %s is (%s)",grid,
                                (const char*)gc[grid].mapping().getName(Mapping::mappingName),
				(gridsToPlot(grid) & GraphicsParameters::toggleContours ? "on" : "off"));
	}
	menu2[numberOfGrids]="exit";
	menu2[numberOfGrids+1]="";   // null string terminates the menu
        gi.getMenuItem(menu2,answer2);
        if( answer2=="exit" )
          break;
	else 
	{
          int gridToToggle = atoi(&answer2[0]);
          assert(gridToToggle>=0 && gridToToggle<numberOfGrids);
          gridsToPlot(gridToToggle)^=GraphicsParameters::toggleContours;
	}
      }
      delete [] menu2;
    }
    else if( answer=="plot the grid" )
    {
      plot(gi, (GridCollection&)gc, psp);
      if( psp.objectWasPlotted )
        psp.objectWasPlotted=2;
    }
    else if( answer=="erase" )
    {
      plotObject=false;
      if( plotOnThisProcessor )
      {
	glDeleteLists(list,1);
	glDeleteLists(lightList,1);
      }
      gi.redraw();

      recomputeContourPlanes=true;
      recomputeCoordinatePlanes=true;
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      if( plotOnThisProcessor )
      {
	glDeleteLists(list,1);
	glDeleteLists(lightList,1);
      }
      
      gi.redraw();
      break;
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else if( answer=="rainbow" || answer=="gray" || answer=="red" || answer=="green" 
	     || answer=="blue" || answer=="user defined" )
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
      else
        ct=GraphicsParameters::userDefined;
  
      psp.colourTable = GraphicsParameters::ColourTables(max(min(ct,GraphicsParameters::numberOfColourTables),0));
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
      gi.stopReadingCommandFile();
    }

     // *wdh* 070910 if( plotObject && (gi.isGraphicsWindowOpen()|| PlotIt::parallelPlottingOption==1 ) )
    if( gi.isInteractiveGraphicsOn() )
    {
      if( updateGeometry )
      {
	updateGeometry=false;
	// these geometry arrays are needed:
	for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
	{
          MappedGrid & mg = (MappedGrid&) gc[grid]; // cast away const
	  if( mg.isRectangular() )  
	    mg.update(MappedGrid::THEmask ); 
	  else
	    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );
	}
      }
      if( computePlotBounds )
      {
	computePlotBounds=false;
	getPlotBounds(gc,psp,xBound); // do later so we can defer creating the center *wdh* 030916
	gi.setGlobalBound(xBound);
      }
      
      real time0=getCPU(), time1;

      if( recomputeContourPlanes || recomputeCoordinatePlanes )
      {
        // rebuild ContourSurfaces initially
	recomputeContourPlanes=false;
	recomputeCoordinatePlanes=false;
	
        // For contours on grid boundaries -- plot AMR grids on that boundary too
	for( grid=numberOfBaseGrids; grid<numberOfGrids; grid++ )
	{
          const int baseGrid = gc.baseGridNumber(grid);
	  ForBoundary(side,axis)
	  {
            if( gc[grid].boundaryCondition(side,axis)>0 )
  	      plotContourOnGridFace(side,axis,grid)=plotContourOnGridFace(side,axis,baseGrid);
            else
	      plotContourOnGridFace(side,axis,grid)=false;
	  }
	}

	const int totalNumberOfCoordinatePlanes=numberOfCoordinatePlanes+sum(plotContourOnGridFace);
	const int totalNumberOfSurfaces=totalNumberOfCoordinatePlanes+ numberOfContourPlanes+ numberOfIsoSurfaces;

        // leave room for extra surfaces
	maxNumberOfContourSurfaces=max(maxNumberOfContourSurfaces,totalNumberOfSurfaces+10);

	delete [] cs;
	cs=NULL;
	cs = new ContourSurface[maxNumberOfContourSurfaces];
    
        if( showTimings ) printF("*** initContourSurfaceList ***\n");

	initContourSurfaceList(cs,gc,gridsToPlot, 
			       plotContourOnGridFace,numberOfCoordinatePlanes,
			       coordinatePlane, numberOfContourPlanes, numberOfIsoSurfaces, 
                               isoSurfaceValue, numberOfPolygonsPerSurface, numberOfVerticesPerSurface );
	
      }


      gi.setAxesDimension( gc.numberOfDimensions() );

      if( plotContours )
      {
	
        PlotIt::plot3dContours( gi,uGCF, psp,list,lightList,plotContours,recomputeVelocityMinMax,uMin,uMax,
                               cs, numberOfPolygonsPerSurface, numberOfVerticesPerSurface );

	// set current min max of contour values
        if( !psp.plotObjectAndExit )
          dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));

       } 


      // -------- Draw the labels -----
      if( plotTitleLabels && plotOnThisProcessor )
      {
 	// plot labels on top and bottom
 	aString topLabel=psp.topLabel;       // remember original values
	// aString topLabel1 = psp.topLabel1;
	
        if( psp.labelComponent )
	{
	  if( psp.topLabel!="" || uGCF.getName(component)!="" )
	    psp.topLabel=psp.topLabel+" "+uGCF.getName(component);
	}
	
	if( psp.labelMinMax )
	{
	  // label min max of components
          aString label = uGCF.getName(component);
	  
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
	  
//           printF(" contour: psp.topLabel1=%s (before) label=%s, psp.labelMinMax=%i\n",
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
      if( plotColourBar && plotOnThisProcessor )
      {
        gi.drawColourBar(numberOfContourLevels,psp.contourLevels,uMin,uMax,psp);
      }
      gi.redraw();
      plotContours=TRUE;

      real timeFinal=getCPU();
      if( showTimings ) printF("contour3d: total time=%8.3e\n",timeFinal-time0);

    } // end if plotObject
    
  } // end for(it=0;;it++)

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  delete [] menu;
  delete [] cs;
  
  psp.adjustGridForDisplacement = adjustGridForDisplacementLocal;  // now set this 

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted+= (int)plotObject ;  // this indicates that the object appears on the screen (not erased)
}

    
#undef FOR_3



// -- new version : (actual version is elsewhere)
void PlotIt::
contour3dNew( GenericGraphicsInterface &gi, const realGridCollectionFunction & uGCF, GraphicsParameters & parameters)
{
}

