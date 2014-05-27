#include "GL_GraphicsInterface.h" // Need GL include files for glVertex3f, etc.
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "DataPointMapping.h"
#include "interpPoints.h"
#include "InterpolatePoints.h"
// -- parallel interpolate pts:
#include "InterpolatePointsOnAGrid.h"
#include "conversion.h"
#include "UnstructuredMapping.h"
#include "display.h"
#include "CompositeGrid.h"
#include "PlotIt.h"
#include "MappingInformation.h"
#include "ShowFileParameter.h"
#include "ShowFileReader.h"
#include <float.h>
#include "App.h"
    
int PlotIt::parallelPlottingOption=1;  // old: 0=copy grid functions to proc. 0 for plotting; new: 1=plot distributed


// local version so that we can change it: 
static int isHiddenByRefinement=MappedGrid::IShiddenByRefinement;

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  i1Bound=I1.getBound(); i2Bound=I2.getBound(); i3Bound=I3.getBound(); \
  for( i3=I3.getBase(); i3<=i3Bound; i3++ )  \
  for( i2=I2.getBase(); i2<=i2Bound; i2++ )  \
  for( i1=I1.getBase(); i1<=i1Bound; i1++ )  \


#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int createMappings( MappingInformation & mapInfo );

//--------------------------------------------------------------------------------------
//  Plot contours of a grid function
//--------------------------------------------------------------------------------------

//\begin{>>PlotItInclude.tex}{\subsection{Contour a realMappedGridFunction}} 
void PlotIt::
contour(GenericGraphicsInterface &gi, const realMappedGridFunction & u, 
        GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */ )
//================================================================================
//
// /Description:
//   Plot contours of a realMappedGridFunction in 2D or 3D.
//  Optionally supply parameters that define the plot characteristics.
//  In two dimensions, plotting options include
//  \begin{itemize}
//    \item plot shaded surface 
//    \item plot (colour) contour lines
//    \item plot wire mesh surface (hidden lines are not supported here due to
//          limitations in OpenGL).
//    \item choose which component to plot
//    \item plot the solution along one or more lines that passes through the grid.
//  \end{itemize}
//   In 3D options include
//  \begin{itemize}
//    \item plot shaded surface contours on arbitrary planes that cut through the grid
//    \item plot shaded surface contours on boundaries.
//    \item plot (colour) contour lines on the planes or boundaries
//    \item plot contours on specified coordinates planes.
//    \item plot 2D contours for specified coordinates planes.
//    \item plot the solution along one or more lines that pass through the grid.
//  \end{itemize}
//
// /u (input): function to plot contours of
// /parameters (input): supply optional parameters
//
// /Author: WDH
//
//\end{PlotItInclude.tex}  
//================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  const MappedGrid & mg = *(u.mappedGrid);
  GridCollection gc(mg.numberOfDimensions(),1);  // make a GridCollection with 1 component grid
  gc[0].reference(mg);
  gc.updateReferences();

  Range all;
//   Range R[8] = { all,all,all,all,all,all,all,all };
//   int component;
//   for( component=0; component<5; component++ )
//     R[u.positionOfComponent(component)]= u.getComponentDimension(component)>0 ? 
//                    Range(u.getComponentBase(component),u.getComponentBound(component))
//                   : all;
  // realGridCollectionFunction v(gc,R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);

  Range R[2];
  int component;
  for( component=0; component<min(2,u.numberOfComponents()); component++ )
  {
    R[component]=Range(u.getComponentBase(component),u.getComponentBound(component));
  }
  realGridCollectionFunction v(gc,u.getGridFunctionType(),R[0],R[1]);
  // v[0]=u;  // *wdh* 090522
  assign(v[0],u);
  v.setName(u.getName());

  // u.display("contour: here is u");
  // v.display("contour: here is v");
  
  for( component=u.getComponentBase(0); component<=u.getComponentBound(0); component++ )
    v.setName(u.getName(component),component);
  
  // v.updateToMatchGrid(); 
  contour(gi, v,parameters);
}

//\begin{>>PlotItInclude.tex}{\subsection{Contour a CompositeGridFunction}} 
void PlotIt::
contour(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, 
	GraphicsParameters & parameters /* = Overture::defaultGraphicsParameters() */)
//================================================================================
//
// /Description:
//   Plot contours of a realMappedGridFunction in 2D or 3D.
//  Optionally supply parameters that define the plot characteristics.
//  In two dimensions, plotting options include
//  \begin{itemize}
//    \item plot shaded surface 
//    \item plot (colour) contour lines
//    \item plot wire mesh surface (hidden lines are not supported here due to
//          limitations in OpenGL).
//    \item choose which component to plot
//    \item plot the solution along one or more lines that passes through the grid.
//  \end{itemize}
//   In 3D options include
//  \begin{itemize}
//    \item plot shaded surface contours on arbitrary planes that cut through the grid
//    \item plot shaded surface contours on boundaries.
//    \item plot (colour) contour lines on the planes or boundaries
//    \item plot contours on specified coordinates planes.
//    \item plot 2D contours for specified coordinates planes.
//    \item plot the solution along one or more lines that pass through the grid.
//  \end{itemize}
//
// /u (input): function to plot contours of
// /parameters (input): supply optional parameters
//
// /Author: WDH
//
//\end{PlotItInclude.tex}  
//================================================================================
{
  if( !gi.graphicsIsOn() ) return;

  bool multiProcessorGrid=false;
  const int myid=max(0,Communication_Manager::My_Process_Number);

#ifndef USE_PPP
  const realGridCollectionFunction & v = u;
  GridCollection & gc = *u.getGridCollection();

#else

//    realGridCollectionFunction v;
//    GridCollection gc;
//    int processorForGraphics = gi.getProcessorForGraphics();
//    redistribute( u, gc,v,Range(processorForGraphics,processorForGraphics) );

  GridCollection *gcp=NULL;
  realGridCollectionFunction *vp=NULL;
  if( PlotIt::parallelPlottingOption==0 )
  {
    // In parallel: make a new grid and gridfunction that only live on one processor
    const int processorForGraphics = gi.getProcessorForGraphics();

    GridCollection & gc0 = *u.getGridCollection();
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

    gcp=&gc0; 
    vp= (realGridCollectionFunction*)(&u); 

    if( multiProcessorGrid )
    {
      if( u.getGridCollection()->getClassName()=="CompositeGrid" )
      {
	CompositeGrid & cg = *new CompositeGrid();
	realCompositeGridFunction & vcg = *new realCompositeGridFunction();

	ParallelGridUtility::redistribute( (realCompositeGridFunction &)u, cg,vcg,
					   Range(processorForGraphics,processorForGraphics) );

	gcp=&cg;
	vp=&vcg;

	// *** do this for now until we fix -- I don't think this is needed 060227 ****
	// cg.destroy(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now *** need to fix redistr.

      }
      else
      {
	gcp = new GridCollection();
	vp  = new realGridCollectionFunction();

	ParallelGridUtility::redistribute( u, *gcp,*vp,Range(processorForGraphics,processorForGraphics) );

      }
    }
  }
  else
  {
    // distributed plotting option *new* 
    gcp = u.getGridCollection();
    vp= (realGridCollectionFunction*)(&u); 
  }

  GridCollection & gc = *gcp;
  const realGridCollectionFunction & v = *vp;

#endif


  for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
  {
    if( !gc[grid].isRectangular() )
    {
      gc[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask);
    }
    else
    {
      gc[grid].update(MappedGrid::THEmask);
    }
  }

  
  // if( true ) printf("*** contour: myid=%i parallelPlottingOption=%i *****\n",myid,parallelPlottingOption);

  // this must be here for P++ (only 1 processor actually plots stuff)
  if( Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics() ||
      PlotIt::parallelPlottingOption==1 )
  {

    if( PlotIt::parallelPlottingOption==0 )
    {
      gi.setSingleProcessorGraphicsMode(true);   // this avoids communication in the graphics getAnswer etc.
      Optimization_Manager::setOptimizedScalarIndexing(On);   // stop communication for scalar indexing
    }
    
    realSerialArray *xSave=NULL; 
    bool gridWasAdjusted=false;  // keep this extra parameter in case adjustGridForDisplacement is changed in the contour plotter
    // We need to save the current displacementScaleFactor in case it is changed in the grid plotter
    const real displacementScaleFactorSave = parameters.displacementScaleFactor;
    if( parameters.adjustGridForDisplacement )
    {
      // Here we change the grid to take into account the displacement: 
      gridWasAdjusted=true;
      adjustGrid( gc,v,parameters,xSave,displacementScaleFactorSave );
    }
    
    if( gc.numberOfDimensions()==2 )
    {
      contour2d(gi, v, parameters);
    }
    else if( gc.numberOfDimensions()==3 )
    {
      bool useNewContour3d=false;
      
      if( useNewContour3d )
      {
        contour3dNew(gi, v, parameters);
      }
      else
      {
	contour3d(gi, v, parameters);
      }
      
    }
    else if( gc.numberOfDimensions()==1 )
    {
      contour1d(gi, v, parameters);
    }
    else
      cout << "PlotIt::contour:ERROR: do not know how to draw contours in " << 
	gc.numberOfDimensions() << " dimension(s)\n";

    if( gridWasAdjusted )
    {
      // Here we change the grid to take into account the displacement: 
      unAdjustGrid( gc,v,parameters,xSave,displacementScaleFactorSave );
    }

    if( PlotIt::parallelPlottingOption==0 )
    {
      gi.setSingleProcessorGraphicsMode(false);
      Optimization_Manager::setOptimizedScalarIndexing(Off);   // turn communication back on
    }
    
  }
  if( multiProcessorGrid && PlotIt::parallelPlottingOption==0 )
  {
    #ifdef USE_PPP
     delete gcp;
     delete vp;
    #endif
  }
  
}

void 
displayValuesAtAPoint(RealArray x0, realCompositeGridFunction & ucg, int numberOfComponents, 
                      int component, int showNearbyValues, int showUnusedValues,
                      IntegerArray *checkTheseGrids = NULL )
// ===============================================================================================
// /Description:
//   Given a point x0(1,0:2), print the values of a component on a grid function.
//  This function is used to print out values from a point picked on the screen when plotting
//  contour values. 
// 
// /showNearbyValues (input) : number of nearby values to show.
// /showUnusedValues (input) : 0= do not show mask=0 values, otherwise show these
// /checkTheseGrids (input) : if specified, do not interpolate from grids with checkTheseGrids(grid)==0
// ===============================================================================================
{
  GridCollection & gc = *ucg.getGridCollection();
  Range C(component,component);
	    
  RealArray uInterpolated(Range(0,0),numberOfComponents);
	    
#ifndef USE_PPP
  InterpolatePoints interp;
  if( checkTheseGrids==NULL )
    interp.interpolatePoints( x0,ucg,uInterpolated,Range(component,component));
  else
  {
    // Only attempt to interpolate from some grids
    interp.buildInterpolationInfo( x0,*ucg.getCompositeGrid(),NULL,checkTheseGrids );
    interp.interpolatePoints( ucg,uInterpolated,C);
  }
  IntegerArray status;
  status=interp.getStatus();

  IntegerArray indexValues,interpoleeGrid;
  interp.getInterpolationInfo(*ucg.getCompositeGrid(),indexValues,interpoleeGrid);

#else  
  // ******** FINISH ME ********** 2012/06/11

  // InterpolatePointsOnAGrid::debug=debug;

  CompositeGrid & cg = *ucg.getCompositeGrid();
  
  int infoLevel=1;
  int interpolationWidth=2;
  int numGhostToUse=0;  // number of ghost points we can use when interpolating from the donor grid function

  InterpolatePointsOnAGrid interpolator;
  interpolator.setInfoLevel( infoLevel );
  interpolator.setInterpolationWidth(interpolationWidth);
  // Set the number of valid ghost points that can be used when interpolating from a grid function: 
  interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
  // Assign all points, extrapolate pts if necessary:
  interpolator.setAssignAllPoints(true);

  int rt=interpolator.buildInterpolationInfo(x0,cg);
  const IntegerArray & status = interpolator.getStatus();
  if( rt!=0 )
  {
    int num=abs(rt);
    printF("displayValuesAtAPoint Error return from InterpolatePointsOnAGrid::buildInterpolationInfo could "
           "not interpolate the point!\n");
    // OV_ABORT("ERROR");
  }
	
  interpolator.interpolatePoints(ucg,uInterpolated);

  IntegerArray indexValues(1,3),interpoleeGrid(1);
  interpoleeGrid=0;
  
//  interpolator.getInterpolationInfo(cg,indexValues,interpoleeGrid);
  
#endif
	    
  if( status(0)!=0  )
  {
    assert( interpoleeGrid(0)>=0 && interpoleeGrid(0)<ucg.numberOfGrids());

    printF("Point (%9.3e,%9.3e,%9.3e) : %s = %10.4e from grid %i (%s), (i1,i2,i3)=(%i,%i,%i)\n",
	   x0(0,0),x0(0,1),x0(0,2),(const char*)ucg.getName(ucg.getComponentBase(0)+component),
	   uInterpolated(0,component),interpoleeGrid(0),(const char*)gc[interpoleeGrid(0)].getName(),
           indexValues(0,0),indexValues(0,1),indexValues(0,2));

    if( showNearbyValues>0 )
    {

      const realArray & ui = ucg[interpoleeGrid(0)];
      const intArray & maski = gc[interpoleeGrid(0)].mask();
      const IntegerArray & dimension = gc[interpoleeGrid(0)].dimension();

      const int i1c=indexValues(0,0), i2c=indexValues(0,1), i3c=indexValues(0,2);
      const int num=showNearbyValues;
      int num3 = gc.numberOfDimensions()==2 ? 0 : num;
      const int i3=dimension(Start,axis3);
      int maxDigits=max(max(abs(i1c-num),abs(i1c+num),abs(i2c-num),abs(i2c+num)),abs(i3c-num),abs(i3c+num));
      aString format;
      if( maxDigits<10 )
	format=gc.numberOfDimensions()==2 ? "i2=%i i1=[%i,%i]: " : "i3=%i i2=%i i1=[%i,%i]: ";
      else if( maxDigits<100 )
	format=gc.numberOfDimensions()==2 ? "i2=%2i i1=[%2i,%2i]: " : "i3=%2i i2=%2i i1=[%2i,%2i]: ";
      else if( maxDigits<1000 )
	format=gc.numberOfDimensions()==2 ? "i2=%3i i1=[%3i,%3i]: " : "i3=%3i i2=%3i i1=[%3i,%3i]: ";
      else if( maxDigits<10000 )
	format=gc.numberOfDimensions()==2 ? "i2=%4i i1=[%4i,%4i]: " : "i3=%4i i2=%4i i1=[%4i,%4i]: ";
      else
	format=gc.numberOfDimensions()==2 ? "i2=%i i1=[%i,%i]: " : "i3=%i i2=%i i1=[%i,%i]: ";
      
      const char *iformat=(const char*)format;
      
      for( int i3=i3c-num3; i3<=i3c+num3; i3++ )
      {
	if( i3<dimension(Start,axis3) || i3>dimension(End,axis3) )
	  continue;		
	for( int i2=i2c+num; i2>=i2c-num; i2-- )
	{
	  if( i2<dimension(Start,axis2) || i2>dimension(End,axis2) )
	    continue;
          if( gc.numberOfDimensions()==2 )
  	    printf(iformat,i2,i1c-num,i1c+num);
          else
  	    printf(iformat,i3,i2,i1c-num,i1c+num);
	  for( int i1=i1c-num; i1<=i1c+num; i1++ )
	  {
	    if( i1<dimension(Start,axis1) || i1>dimension(End,axis1) )
	      printf("   outside  ");
	    else if( maski(i1,i2,i3)!=0 || showUnusedValues!=0 )
	      printf("%11.4e ",ui(i1,i2,i3,component));
	    else
	      printf("   mask=0   ");
	  }
	  printf("\n");
	}
        printf("\n");
      }
    }
	      
  }
  else
  {
    printf("Point (%9.3e,%9.3e,%9.3e) : %s : unable to interpolate at this point\n",
	   x0(0,0),x0(0,1),x0(0,2),(const char*)ucg.getName(ucg.getComponentBase(0)+component));
  }
}


//--------------------------------------------------------------------------------------
//  Plot contours of a grid function in 2D
//--------------------------------------------------------------------------------------
void PlotIt::
contour2d(GenericGraphicsInterface &gi, const realGridCollectionFunction & uGCF, GraphicsParameters & parameters)
{
  const bool showTimings=false;

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();
  
  if( false && PlotIt::parallelPlottingOption==1 )
  {
#ifdef USE_PPP
    printf("*** contour2d: myid=%i distributed plotting is ON *****\n",myid);
    fflush(0);
    MPI_Barrier(Overture::OV_COMM);
#endif
  }

  
  // save the current window
  int startWindow = gi.getCurrentWindow();

  const GridCollection & gc = *(uGCF.gridCollection);

  const int numberOfGrids =  gc.numberOfComponentGrids();
  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(true);  // true means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;
  bool & adjustMappingForDisplacement = psp.dbase.get<bool>("adjustMappingForDisplacement");
  

  real uMin=0.,uMax=1.;
  int  & component            = psp.componentForContours; // Can be fatal if this routine is called from
   // plot this component by default:
  component = min(max(component,uGCF.getComponentBase(0)),uGCF.getComponentBound(0)); 
  

  char buff[80];
  aString answer,answer2;
  aString menu0[]= { "!contour plotter",
//                    "erase and exit",
//                    "plot",
                    ">choose a component",
                    "<wire frame (toggle)",
		    "user defined output",
		    "print solution info",
//                    "wire frame without hidden lines (toggle)",
                    "toggle grids on and off",
                    ">contour line options",
//                      "plot contour lines (toggle)",
//                      "colour line contours (toggle)",
//                      "number of contour levels",
//                      "set min and max",
                      "set minimum contour spacing",
                      "specify contour levels",
                      "set contour line width",
                      "dashed lines for negative contours (toggle)",
                    "<query values with mouse",
                    "line plots",
                    ">options",
//                      "vertical scale factor",
//                      "plot boundaries (toggle)",
//                        "plot colour bar (toggle)",
//                        "plot labels (toggle)",
//                      "plot ghost lines",
		      "set plot bounds",
		      "reset plot bounds",
  		      "change colour bar",
                      ">colour table choices",
                        "rainbow",
                        "gray",
                        "red",
                        "green",
                        "blue",
                        "user defined",
                      "<set origin for axes",
//		      "lighting (toggle)",
//                        "plot the axes (toggle)",
//                        "plot the back ground grid (toggle)",
		      "keep aspect ratio",
                      "do not keep aspect ratio",
                    "<plot the grid",
//                    "erase",
                    "exit this menu",
                    "" };

//  // setup a user defined menu and some user defined buttons
//    aString buttons[][2] = {{"plot contour lines (toggle)",        "Lines"}, 
//  			 {"plot surface (toggle)",              "Surf"},
//  			 {"wire frame (toggle)",                "WireFr"},
//  //			 {"plot the axes (toggle)",             "Axes"}, 
//  			 {"colour line contours (toggle)",      "ColLin"},
//  //			 {"plot the back ground grid (toggle)", "BgGrid"}, 
//  //			 {"plot labels (toggle)",               "Labels"},
//  //			 {"plot colour bar (toggle)",           "ColBar"},
//  			 {"plot boundaries (toggle)",           "Bndry"},
//  			 {"plot",                               "Plot"},
//  			 {"erase",                              "Erase"},
//  			 {"exit this menu",                     "Exit"},
//  			 {"",                                   ""}};
  // aString pulldownMenu[] = {"set min and max", "number of contour levels", "" };
  // aString menuTitle = "Contour";
  
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

  enum PickingOptionEnum
  {
    pickingOff,
    pickToQueryValue, 
    pickToHideGrids
  } pickingOption=pickToQueryValue;

  GUIState dialog;
  if( !psp.plotObjectAndExit )
  {
    dialog.setWindowTitle("Contour Plotter");
    dialog.setExitCommand("exit", "exit");

    dialog.setOptionMenuColumns(1);

    dialog.buildPopup(menu);
    // dialog.setUserButtons(buttons);
    // dialog.setUserMenu(pulldownMenu, "Grid");
    
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
                            max(uGCF.getComponentBase(0),min(uGCF.getComponentBound(0),component)));
    delete [] cmd;
    delete [] label;


    aString opcmd[] = {"pick off","pick to query value","pick to hide grids",""};
    aString opLabel[] = {"off","query value","hide grids",""};
//     aString opcmd[] = {"pick off","pick to hide grids",""};
//     aString opLabel[] = {"off","hide grids",""};
    dialog.addOptionMenu("Pick to:", opcmd,opLabel,pickingOption);


    aString pbCommands[] = {"plot contour lines (toggle)",
			    "plot surface (toggle)",
			    "wire frame (toggle)",
			    "colour line contours (toggle)",
			    "plot boundaries (toggle)",
                            "plot the grid",
			    "plot",
                            "show all",
                            "reset min max",
			    "erase",
			    "erase and exit",
			    "exit this menu",
			    ""};
    aString pbLabels[] = {"Lines",
                          "Surface",
                          "Wire frame",
                          "Colour lines",
                          "Plot boundary", 
                          "Plot Grid",
                          "Plot",
                          "Show all",
                          "reset min max",
                          "Erase",
                          "Erase and Exit",
                          "Exit",
			  ""};
    int numRows=4;
    dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

    // *** specify toggle buttons ***
    const int numberOfToggleButtons=6;
    aString tbCommands[numberOfToggleButtons];
    int tbState[numberOfToggleButtons];

    int i=0;
    tbCommands[i] = "plot hidden refinement points"; 
    tbState[i]=psp.plotHiddenRefinementPoints;  i++;
    
    tbCommands[i] = "compute coarsening factor"; 
    tbState[i]=psp.computeCoarseningFactor;  i++;
    
    tbCommands[i] = "flat shading"; 
    tbState[i]=psp.flatShading;  i++;
    
    tbCommands[i] = "adjust grid for displacement"; 
    tbState[i]=psp.adjustGridForDisplacement;  i++;

    tbCommands[i] = "adjust mapping for displacement"; 
    tbState[i]=adjustMappingForDisplacement;  i++;
    
    tbCommands[i] = "";
    tbState[i]=0;

    assert( i<numberOfToggleButtons );

    const int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=8;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "min max"; 
    sPrintF(textStrings[nt], "%g %g",uMin,uMax);
    nt++;

    textLabels[nt] ="vertical scale factor";
    sPrintF(textStrings[nt], "%g",psp.contourSurfaceVerticalScaleFactor);
    nt++;

    textLabels[nt] ="number of levels";
    sPrintF(textStrings[nt], "%i",psp.numberOfContourLevels);
    nt++;
    
    textLabels[nt] ="ghost lines";
    sPrintF(textStrings[nt], "%i",psp.numberOfGhostLinesToPlot);
    nt++;

    textLabels[nt] ="coarsening factor";
    sPrintF(textStrings[nt], "%i (<0 : adaptive)",psp.computeCoarseningFactor);
    nt++;
    
    textLabels[nt] = "xScale, yScale"; 
    sPrintF(textStrings[nt], "%g %g",psp.xScaleFactor,psp.yScaleFactor);
    nt++;

    textLabels[nt] = "displacement scale factor";
    sPrintF(textStrings[nt], "%g",psp.displacementScaleFactor);
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

  bool & plotWireFrame        = psp.plotWireFrame;
  bool & colourLineContours   = psp.colourLineContours;
  bool & plotColourBar        = psp.plotColourBar;
  bool & plotContourLines     = psp.plotContourLines;
  bool & plotShadedSurface    = psp.plotShadedSurface;
  bool & plotTitleLabels      = psp.plotTitleLabels;
  int  & numberOfContourLevels= psp.numberOfContourLevels;
  IntegerArray & gridOptions          = psp.gridOptions; 
  
// contour3d and local changes are made that make component out of bounds on return.
  IntegerArray & gridsToPlot      = psp.gridsToPlot;
  IntegerArray & minAndMaxContourLevelsSpecified  = psp.minAndMaxContourLevelsSpecified;
  RealArray & minAndMaxContourLevels = psp.minAndMaxContourLevels;
  real & minimumContourSpacing = psp.minimumContourSpacing;
  RealArray & contourLevels    = psp.contourLevels;
  bool & plotDashedLinesForNegativeContours = psp.plotDashedLinesForNegativeContours;
  const	aString topLabel1 = psp.topLabel1; // save original topLabel1

  gi.setAxesOrigin(psp.axesOrigin(0), psp.axesOrigin(1), psp.axesOrigin(2));

//  bool & linePlots             = psp.linePlots;
  
  bool & plotGridBoundariesOnContourPlots = psp.plotGridBoundariesOnContourPlots;
  
  real & contourSurfaceVerticalScaleFactor=psp.contourSurfaceVerticalScaleFactor;  // add to parameters.

  bool plotContours=true;

  bool contourLevelsSpecified=contourLevels.getLength(0)>0;

  int & numberOfGhostLinesToPlot = psp.numberOfGhostLinesToPlot;

  bool recomputeVelocityMinMax=true;

  IntegerArray checkTheseGrids(gc.numberOfComponentGrids()); checkTheseGrids=true;

  gi.setKeepAspectRatio(psp.keepAspectRatio); 

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    gridsToPlot.redim(numberOfGrids);  
    gridsToPlot=GraphicsParameters::toggleSum; // by default plot all grids, contours etc.
    gridOptions.redim(numberOfGrids);  
    gridOptions=GraphicsParameters::plotGrid | GraphicsParameters::plotBlockBoundaries | 
                GraphicsParameters::plotInteriorBoundary | GraphicsParameters::plotInterpolation;
  }
  else
  {
    if( gridsToPlot.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      gridsToPlot.redim(numberOfGrids);  gridsToPlot=GraphicsParameters::toggleSum;
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
  // Make sure some arrays are big enough:
  if( minAndMaxContourLevelsSpecified.getLength(0) < uGCF.getComponentDimension(0) )
  {
    int newSize = uGCF.getComponentDimension(0)+10; // leave room for all components plus extra
    Range R(minAndMaxContourLevelsSpecified.getBound(0)+1,newSize-1);
    minAndMaxContourLevelsSpecified.resize(newSize);
    minAndMaxContourLevels.resize(2,newSize);
    minAndMaxContourLevelsSpecified(R)=false;   // give values to new entries
    minAndMaxContourLevels(Range(0,1),R)=0.;
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
  
  int grid, side, axis;
  
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
  
  // set the grid coarsening factor: for very fine grids we do not plot at the highest resolution
  if( psp.computeCoarseningFactor )
  {
    const int maxPlotablePoints=500000; // maximum number of points we plot at the highest resolution

    int numberOfGridPoints=0;
    Index I1,I2,I3;
    for( grid=0; grid<numberOfGrids; grid++ )
    {
      getIndex(gc[grid].gridIndexRange(),I1,I2,I3);
      numberOfGridPoints+=I1.getLength()*I2.getLength()*I3.getLength();
    }
    // printf("contour:INFO:numberOfGridPoints=%i \n",numberOfGridPoints);
  
    if( numberOfGridPoints>maxPlotablePoints )
    {
      gi.gridCoarseningFactor=int(numberOfGridPoints/real(maxPlotablePoints)+.5);
      if( false )
	printf("contour:INFO: Setting  gi.gridCoarseningFactor = %i since there are %i total grid points\n",
	       gi.gridCoarseningFactor,  numberOfGridPoints);
      if( !psp.plotObjectAndExit && gi.isGraphicsWindowOpen() )
	dialog.setTextLabel("coarsening factor",sPrintF(answer,"%i (<0 : adaptive)",gi.gridCoarseningFactor));
    }
  }
  


  // get Bounds on the grids ** this uses the grid points **
  RealArray xBound(2,3);

  bool updateGeometry=true;     // we compute geometry arrays later, if needed.
  bool computePlotBounds=true;  // this means compute the plot bounds at the appropriate

  // *wdh* 070910 if( gi.isGraphicsWindowOpen() || PlotIt::parallelPlottingOption==1 ) // is this right for parallel?
  if( gi.isInteractiveGraphicsOn() )
  {
    updateGeometry=false;
    computePlotBounds=false;
    getPlotBounds(gc,psp,xBound); // do later so we can defer creating the center *wdh* 030916
    gi.setGlobalBound(xBound);
  }
  else
  {
    xBound=0;  xBound(1,Range(0,2))=1.;  // set some default values
  }

  // set default prompt
  gi.appendToTheDefaultPrompt("contour>");

  int menuItem=-1;

  SelectionInfo select; select.nSelect=0;
  int len=0;

  int showNearbyValues=2; // For query values: show this many nearby values to left and right
  int showUnusedValues=0; // For query values: do no show values at mask=0 pts

  // We need a local value for adjustGridForDisplacementLocal since we don't want to change this
  // until the user exits.
  int adjustGridForDisplacementLocal = psp.adjustGridForDisplacement; 
  
  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==true
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
      menuItem=gi.getAnswer(answer,"", select);
      gi.savePickCommands(true); // turn back on
    }
    
#ifdef USE_PPP
    if( false && PlotIt::parallelPlottingOption==1 )
    {
      printf(" contour:After getAnswer: myid=%i it=%i answer=%s\n",myid,it,(const char*)answer);
      fflush(0);
      MPI_Barrier(Overture::OV_COMM);
    }
#endif
    
    // make sure the currentWindow is the same as on entry
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( answer=="plot contour lines (toggle)")
    {
      plotContourLines= !plotContourLines;
    }
    else if( answer=="plot surface (toggle)" )
    {
      plotShadedSurface= !plotShadedSurface;
    }
    else if( answer=="wire frame (toggle)" )
    {
      plotWireFrame= !plotWireFrame;
    }
    else if( answer=="wire frame" )
    {
    }
    else if( answer=="no wire frame" )
    {
    }
    else if(answer=="colour line contours (toggle)")
    {
      colourLineContours= !colourLineContours;
    }
    else if(answer=="number of contour levels")
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the number of contour levels(>=2) (current=%i)",
				     numberOfContourLevels));
      if( answer2 !="" && answer2!=" ")
      {
	for( int i=0; i<answer2.length() && i<80; i++)
	  buff[i]=answer2[i];
	sScanF(buff,"%i",&numberOfContourLevels);
        if( numberOfContourLevels<2 )
	{
	  cout << "Error, number of contour levels must be greater than 1! \n";
	  numberOfContourLevels=11;
	}
      }
    }
    else if( len=answer.matches("number of levels") )  // new way
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfContourLevels);
      dialog.setTextLabel("number of levels",sPrintF(answer,"%i",numberOfContourLevels));
      if( numberOfContourLevels<2 )
      {
	cout << "Error, number of contour levels must be greater than 1! \n";
	numberOfContourLevels=11;
      }
    }
    else if( answer=="vertical scale factor" )  // old way
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the contour surface vertical scale factor (current=%f)",
				     contourSurfaceVerticalScaleFactor));
      sScanF(answer2,"%e",&contourSurfaceVerticalScaleFactor); 
    }
    else if( len=answer.matches("vertical scale factor") )  // new way
    {
      sScanF(answer(len,answer.length()-1),"%e",&contourSurfaceVerticalScaleFactor);
      dialog.setTextLabel("vertical scale factor",sPrintF(answer,"%g",contourSurfaceVerticalScaleFactor));
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
      dialog.getOptionMenu("Plot component:").setCurrentChoice(component);

      if( !minAndMaxContourLevelsSpecified(component) )
      {
        // recompute the plot bounds if the user has not explicitly set the min and max values.
	getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
	recomputeVelocityMinMax=false;
	dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
      }
    }
    else if( answer=="lighting (toggle)" )
    {
      gi.outputString("This function is obsolete. Use the view characteristics dialog"
		      " to turn on/off lighting.");
      continue;
    }
    else if(answer=="plot boundaries (toggle)")
    {
      plotGridBoundariesOnContourPlots= !plotGridBoundariesOnContourPlots;
    }
    else if( menuItem > chooseAComponentMenuItem && menuItem <= chooseAComponentMenuItem+numberOfComponents )
    {
      component=menuItem-chooseAComponentMenuItem-1 + uGCF.getComponentBase(0);
      recomputeVelocityMinMax=true;
      // cout << "chose component number=" << component << endl;
    }
    else if( dialog.getTextValue(answer,"displacement scale factor","%e",psp.displacementScaleFactor) )
    {
      printF("INFO: You will have to exit this menu and replot to see the new displacement scale factor"
             " take effect. displacementScaleFactor=%9.3e\n",psp.displacementScaleFactor);
    }
    else if( len=answer.matches("xScale, yScale") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e",&psp.xScaleFactor,&psp.yScaleFactor);
      printF("New values are xScale = %g, yScale = %g \n",psp.xScaleFactor,psp.yScaleFactor);
      dialog.setTextLabel("xScale, yScale",sPrintF(answer,"%g %g",psp.xScaleFactor,psp.yScaleFactor));
    }
    else if( answer=="reset min max" )
    {
      getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
      recomputeVelocityMinMax=false;

      minAndMaxContourLevels(0,component)=uMin; // save levels for this component
      minAndMaxContourLevels(1,component)=uMax;
      minAndMaxContourLevelsSpecified(component)=false;

      dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
      printF("Resetting min and max to [%20.14e,%20.14e] for component%i (%s)\n",uMin,uMax,component,
	     (const char*)uGCF.getName(component));
    }
    else if( len=answer.matches("min max") )
    {

      sScanF(answer(len,answer.length()-1),"%e %e",&uMin,&uMax);

      dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));

      printf("New values are min = %e, max = %e \n",uMin,uMax);
      printf("Note that these values will only be applied to the current component\n");

      minAndMaxContourLevelsSpecified(component)=true;
      minAndMaxContourLevels(0,component)=uMin; // save levels for this component
      minAndMaxContourLevels(1,component)=uMax;
    }
    else if( answer=="set min and max" )
    {
      if( !minAndMaxContourLevelsSpecified(component) && recomputeVelocityMinMax )
      {
        // Get Bounds on u -- treat the general case when the component can be in any Index position of u
        recomputeVelocityMinMax=false;
        getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
      }
      printf("Set min and max for contours, current=(%e,%e) \n"
             "Enter a blank line to use actual values computed from the grid function",uMin,uMax);
      gi.inputString(answer2,sPrintF(buff,"Enter min, max contour values (current=%9.2e,%9.2e)",uMin,uMax));
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e %e",&uMin,&uMax);
        printf("New values are min = %e, max = %e \n",uMin,uMax);
        printf("Note that these values will only be applied to the current component\n");
        minAndMaxContourLevelsSpecified(component)=true;
        minAndMaxContourLevels(0,component)=uMin; // save levels for this component
        minAndMaxContourLevels(1,component)=uMax;
      }
      else
        minAndMaxContourLevelsSpecified(component)=false;
    }
    else if( answer=="set minimum contour spacing" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the minimum contour spacing (current=%e)",minimumContourSpacing)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%e ",&minimumContourSpacing);
        gi.outputString(sPrintF(buff,"New minimum contour spacing = %e\n",minimumContourSpacing));
      }
    }
    else if( answer=="specify contour levels" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter number of contour levels (current=%i)"
				     "(0=reset)(enter to continue)",
				     numberOfContourLevels)); 
      if( answer2 !="" && answer2!=" ")
      {
        sscanf(answer2,"%i",&numberOfContourLevels);
        if( numberOfContourLevels>0 )
	{
	  contourLevelsSpecified=true;
	  contourLevels.redim(numberOfContourLevels);
	  for( int i=0; i<numberOfContourLevels; i++ )
	  {
	    gi.inputString(answer2,sPrintF(buff,"Enter contour level %i (levels should be increasing in value)",i));
	    sScanF(answer2,"%e ",&contourLevels(i));
	  }
	}
	else
	{ 
          contourLevels.redim(0);
	  contourLevelsSpecified=false;
	  numberOfContourLevels=11;
	}
      }
    }
    else if( answer=="set contour line width" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter contour line width (current=%e)",
				     psp.size(GraphicsParameters::minorContourWidth)));
      if( answer2 !="" && answer2!=" ")
      {
        real newWidth;
	sScanF(answer2,"%e ",&newWidth);
        psp.set(GraphicsParameters::minorContourWidth,newWidth);
        psp.set(GraphicsParameters::majorContourWidth,newWidth*2.);
        gi.outputString(sPrintF(buff,"New contour line width = %e\n",
				psp.size(GraphicsParameters::minorContourWidth)));
      }
    }
    else if( answer=="dashed lines for negative contours (toggle)" )
    {
      plotDashedLinesForNegativeContours= !plotDashedLinesForNegativeContours;
    }
    else if( dialog.getToggleValue(answer,"compute coarsening factor",psp.computeCoarseningFactor) ){} //
    else if( dialog.getToggleValue(answer,"plot hidden refinement points",psp.plotHiddenRefinementPoints) )
    { 
      isHiddenByRefinement = psp.plotHiddenRefinementPoints ? 0 : MappedGrid::IShiddenByRefinement; 
    }
    else if( dialog.getToggleValue(answer,"flat shading",psp.flatShading) ){} //
    else if( dialog.getToggleValue(answer,"adjust grid for displacement",adjustGridForDisplacementLocal) )
    {
      printF("contour:INFO:You must exit the contour plotter and re-enter to see the grid adjusted or not for the displacement\n");
    } 
    else if( dialog.getToggleValue(answer,"adjust mapping for displacement",adjustMappingForDisplacement) )
    {
      printF("contour:INFO:Setting adjustMappingForDisplacement=%i\n",(int)adjustMappingForDisplacement);
      if( adjustMappingForDisplacement && !adjustGridForDisplacementLocal )
      {
	adjustGridForDisplacementLocal=true;
	printF("contour:INFO:Also setting adjustGridForDisplacementLocal=true.\n");
        dialog.setToggleState("adjust grid for displacement",true);
      }
      printF("contour:INFO:You must exit the contour plotter and re-enter to see the grid adjusted or not for the displacement\n");
    } 
    else if( answer=="line plots" )
    {
      int oldPBGG = gi.getPlotTheBackgroundGrid();
      int oldKAR  = gi.getKeepAspectRatio();
      const int componentOld=component;

      // plot solution on lines that cut the 2D grid
      contourCuts(gi, uGCF,psp );

      // Restore plotbackgroundgrid and keepAspectRatio after this call
      gi.setPlotTheBackgroundGrid(oldPBGG);
      
      psp.keepAspectRatio=oldKAR;
      gi.setKeepAspectRatio(psp.keepAspectRatio); 

      // the boundingbox is messed up (set for 1D) after this call
      if( computePlotBounds )
      {
	computePlotBounds=false;
        getPlotBounds(gc,psp,xBound);
      }
      
      gi.setGlobalBound(xBound);

      // erase the labels and replot them
      gi.eraseLabels(psp);

      // replot the 3D object
      component=componentOld;  // reset
      plotObject = true;
      plotContours = true;
    }
    else if( answer=="toggle grids on and off" )
    {
      aString answer2;
      aString *menu2 = new aString[numberOfGrids+2];
      for(;;)
      {
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  menu2[grid]=sPrintF(buff,"%i : %s is (%s)",grid,
			      (const char*)gc[grid].mapping().getName(Mapping::mappingName),
			      (gridsToPlot(grid) & GraphicsParameters::toggleContours ? "on" : "off"));
	}
	menu2[numberOfGrids]="exit this menu";
	menu2[numberOfGrids+1]="";   // null string terminates the menu
        gi.getMenuItem(menu2,answer2);
        if( answer2=="exit this menu" )
          break;
	else 
	{
          int gridToToggle = atoi(&answer2[0]);
          assert(gridToToggle>=0 && gridToToggle<numberOfGrids);
          gridsToPlot(gridToToggle)^=GraphicsParameters::toggleContours;

          // For picking values: 
	  checkTheseGrids(gridToToggle)=gridsToPlot(gridToToggle) & GraphicsParameters::toggleContours;
	}
      }
      delete [] menu2;
    }
    else if( answer=="set plot bounds" )
    {
      RealArray & pb = parameters.plotBound;
      parameters.usePlotBounds=true;
      gi.inputString(answer,"Enter plot bounds to use xa,xb, ya,yb, za,zb");
      sScanF(answer,"%e %e %e %e %e %e\n",&pb(0,0),&pb(1,0),&pb(0,1),&pb(1,1),&pb(0,2),&pb(1,2));
      printF(" Using plot bounds = [%9.3e,%9.3e][%9.3e,%9.3e][%9.3e,%9.3e]\n",
	     pb(0,0),pb(1,0),pb(0,1),pb(1,1),pb(0,2),pb(1,2));

      xBound=pb;  // *wdh* 101122
      computePlotBounds=false;
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(psp.plotBound);

    }
    else if( answer=="reset plot bounds" )
    {
      parameters.usePlotBounds=false;
    }
    else if( len=answer.matches("toggle grid") )
    {
      int onOff, gridToToggle=-1;
      sScanF(&answer[len],"%i %i", &gridToToggle, &onOff);

      if( gridToToggle>=0 && gridToToggle<numberOfGrids )
      {
	gridsToPlot(gridToToggle) ^=GraphicsParameters::toggleContours;
	// For picking values: 
	checkTheseGrids(grid)=gridsToPlot(gridToToggle) & GraphicsParameters::toggleContours;
      }
    }
    else if( answer=="query values with mouse" )
    {
      const bool & adjustMappingForDisplacement = psp.dbase.get<bool>("adjustMappingForDisplacement");
      if( parameters.adjustGridForDisplacement && !adjustMappingForDisplacement )
      {
	printF("query values:ERROR: adjustGridForDisplacement=true but adjustMappingForDisplacement=false\n"
	       " The query values will not be correct. You should choose option `adjust mapping for displacement'\n");
	gi.stopReadingCommandFile();
      }

      aString menu[]=
      {
	"!pick points",
        "show nearby values",
        "do not show nearby values",
        "show values at unused points",
        "do not show values at unused points",
        "enter a grid point",
	"done",
	""
      };
      GUIState queryInterface;
      queryInterface.buildPopup(menu);
      gi.pushGUI(queryInterface);
      
      SelectionInfo select;
//      PickInfo3D pick;
      for( ;; )
      {
    
//	int numberSelected=getMenuItem(menu,answer,"select a point",selection,pickRegion);
	gi.getAnswer(answer,"select a point", select);
	if( answer=="done" || answer=="exit" )
	  break;
        else if( answer=="show nearby values" )
	{
          gi.inputString(answer,sPrintF(buff,"Enter the number of values to show (current=%i)\n",showNearbyValues));
	  if( answer!="" )
	  {
	    sScanF(answer,"%i",&showNearbyValues);
	  }
          if(showNearbyValues>0 ) 
      	    printf("show %i nearby values\n",showNearbyValues);
          else
      	    printf("do NOT show nearby values\n");
	}
        else if( answer=="do not show nearby values" )
	{
          showNearbyValues=0;
	}
        else if( answer=="show values at unused points" )
	{
	  showUnusedValues=1;
	}
        else if( answer=="do not show values at unused points" )
	{
	  showUnusedValues=0;
	}
        else if( answer=="enter a grid point" )
	{
          int grid=0, i1=0, i2=0, i3=0;
          gi.inputString(answer,"Enter grid, i1,i2,i3 of the point to check");
          sScanF(answer,"%i %i %i %i",&grid,&i1,&i2,&i3);
	  
          if( grid>=0 && grid<gc.numberOfComponentGrids() )
	  {
	    realArray & uu = uGCF[grid];
	    if( i1>=uu.getBase(0) && i1<=uu.getBound(0)  &&
                i1>=uu.getBase(0) && i1<=uu.getBound(0) && 
                i1>=uu.getBase(0) && i1<=uu.getBound(0) )
	    {
              int m = gc[grid].mask()(i1,i2,i3);
	      printf(" grid=%i (i1,i2,i3)=(%i,%i,%i) mask=%i u=%14.8e\n",grid,i1,i2,i3,
                     (m==0 ? 0 : m>0 ? 1 : -1), uGCF[grid](i1,i2,i3,component));
	      
	    }
            else
	    {
              printf("ERROR: Invalid values for (i1,i2,i3)=(%,%i,%i) \n",i1,i2,i3);
	    }
	  }
          else
	  {
            printf("ERROR: invalid value for grid=%i\n",grid);
	  }
	}
	else if (select.active && select.nSelect > 0)
	{
          RealArray x0(1,3);
	  x0(0,0) = select.x[0];
	  x0(0,1) = select.x[1];
	  x0(0,2) = select.x[2];
          
	  realCompositeGridFunction & ucg = (realCompositeGridFunction &)uGCF;
	  displayValuesAtAPoint(x0,ucg,numberOfComponents,component,showNearbyValues,showUnusedValues,
                                &checkTheseGrids);

/* -----
          RealArray r(2,3),x0(2,3);
	  
	  r(0,0)=pickRegion(0,0); r(0,1)=pickRegion(0,1);
	  r(1,0)=pickRegion(1,0); r(1,1)=pickRegion(1,1);
	  
	  if( numberSelected==0 )
	    r(0,2)=r(1,2)=zBufferResolution*.5; // what should this be?
	  else
	    r(0,2)=r(1,2)=min(selection(nullRange,1)); // *** 3d
	  pickToWorldCoordinates(r,x0);
	  if( gc.numberOfDimensions()==2 )
	    x0(0,2)=x0(1,2)=0.;
      
	  printf(" Box chosen: [%9.2e,%9.2e]x[%9.2e,%9.2e]\n",x0(0,0),x0(1,0),x0(0,1),x0(1,1));
---- */
	  
	}
      }
      gi.popGUI(); // restore the GUI
    }

    else if( answer=="pick to query value" || answer=="pick to hide grids" || answer=="pick off" )
    {
      pickingOption= ( answer=="pick to query value" ? pickToQueryValue :
	               answer=="pick to hide grids"  ? pickToHideGrids : pickingOff);
      
      dialog.getOptionMenu(1).setCurrentChoice((int)pickingOption);
    }
    else if( pickingOption==pickToHideGrids && 
	     (select.active || select.nSelect || answer.matches("hide grid") ) )
    {
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	for( int i=0; i<select.nSelect; i++ )
	{
	  if( gc[grid].getGlobalID()==select.selection(i,0) )
	  {
	    printf("Hide grid %i (%s) \n",grid,(const char*)gc[grid].getName());
	    gridsToPlot(grid)^=GraphicsParameters::toggleContours;

            int value=gridsToPlot(grid)&GraphicsParameters::toggleContours;

	    // For picking values: 
	    checkTheseGrids(grid)=gridsToPlot(grid) & GraphicsParameters::toggleContours;

	    gi.outputToCommandFile(sPrintF(answer,"toggle grid %i %i\n",grid,value));
	    break;
	  }
	}
      }
      
      plotObject = true;
      plotContours = true;
      
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

      // ::display(gridsToCheck,"pickToQueryValue: gridsToCheck","%2i");
      
      displayValuesAtAPoint(x0,ucg,numberOfComponents,component,showNearbyValues,showUnusedValues,
                            &checkTheseGrids);
            	  
    }
    else if( answer=="show all" )
    {
      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	gridsToPlot(grid) |= GraphicsParameters::toggleContours;
      }
      checkTheseGrids = true;
      plotObject = true;
      plotContours = true;
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
    else if( answer=="plot the grid" )
    {
      plot(gi, (GridCollection&)gc, psp);
    }
    else if( answer=="user defined output" )
    {
      printf("Calling the function userDefinedOutput (by default in Overture/Ogshow/userDefinedOutput.C)\n"
             "See the comments in userDefinedOutput.C for how to output results in your favourite format\n");
      
      PlotIt::userDefinedOutput(uGCF,psp,"contour");
    }
    else if( answer=="print solution info" )
    {
      real minu,maxu;
      for( int n=0; n<numberOfComponents; n++ )
      {
	getBounds(uGCF,minu,maxu,parameters,Range(n,n));
	printF("Component %i (%s) [min,max]=[%20.14e,%20.14e].\n",n,(const char*)uGCF.getName(n),minu,maxu);
      }
    }
    else if( answer=="erase" )
    {
      plotObject=false;
      if( plotOnThisProcessor ) glDeleteLists(list,1);
      gi.redraw();
    }
    else if( answer=="exit this menu" || answer=="exit" )
    {
      break;
    }
    else if( answer=="erase and exit" )
    {
      plotObject=false;
      if( plotOnThisProcessor ) glDeleteLists(list,1);
      
      gi.redraw();
      break;
    }
    else if( answer=="set origin for axes" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter origin for axes (x,y,z) (enter return for default)")); 

      if( answer2 !="" && answer2!=" ")
      {
	real xo=GenericGraphicsInterface::defaultOrigin, 
	  yo=GenericGraphicsInterface::defaultOrigin, zo=GenericGraphicsInterface::defaultOrigin;
	sScanF(answer2,"%e %e %e", &xo, &yo, &zo);
	gi.setAxesOrigin(xo, yo, zo);
      }
    }
    else if( answer=="change colour bar" )
    {
      gi.updateColourBar(psp);
    }
    else if( answer=="plot ghost lines" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the number of ghost lines (or cells) to plot (current=%i)",
             numberOfGhostLinesToPlot)); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF(buff,"Plot %i ghost lines\n",numberOfGhostLinesToPlot));

        // getPlotBounds(gc,psp,xBound);  // get new plot bounds
        computePlotBounds=true;
      }
    }
    else if( len=answer.matches("ghost lines") )  // new way
    {
      sScanF(answer(len,answer.length()-1),"%i",&numberOfGhostLinesToPlot);
      dialog.setTextLabel("ghost lines",sPrintF(answer,"%i",numberOfGhostLinesToPlot));

      if( !minAndMaxContourLevelsSpecified(component) )
      {
        // recompute the plot bounds if the user has not explicitly set the min and max values.
	getBounds(uGCF,uMin,uMax,parameters,Range(component,component));
	recomputeVelocityMinMax=false;
	dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));
      }
      
    }
    else if( len=answer.matches("coarsening factor") ) 
    {
      sScanF(answer(len,answer.length()-1),"%i",&gi.gridCoarseningFactor);
      dialog.setTextLabel("coarsening factor",sPrintF(answer,"%i (<0 : adaptive)",gi.gridCoarseningFactor));

      printF("=================================INFO====================================================\n"
             "The coarsening factor is used to plot contours at a reduced resolution,\n"
             "which may be required for very fine grids to avoid using too much memory.\n"
             "The coarsening factor may be any positive or negative integer.\n"
             " o Setting this coarsening-factor to `2', for example, will cause the contour plotter\n"
             "   to use every other grid point (except near boundaries or interpolation points).\n"
             "   Choosing a value of `3' will use every third grid point. \n"
             " o Setting the coarsening factor to be a negative integer will cause the contour plotter\n"
             "   to only use the coarser grid where the colour is constant. The result will be a contour\n"
             "   plot which is the same as if a fine grid were plotted everywhere.\n"
             "=========================================================================================\n");

      // Usually we want to turn off the computation of the coarseining factor when we set
      // the coarsening factor. *wdh* 2012/03/29
      printF("NOTE: I am turning off `compute coarsening factor'. \n");
      psp.computeCoarseningFactor=false;
      dialog.setToggleState("compute coarsening factor",false); 
    }
    else if( answer=="plot" )
    {
      plotObject=true;
    }
    else if( answer=="rainbow" || answer=="gray" || answer=="red" || answer=="green" || answer=="blue" ||
             answer=="user defined" )
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
      printF("contour: Unknown response = %s (myid=%i)",(const char*)answer,myid);
      gi.stopReadingCommandFile();
    }

    // -----------------------------------------------------------------------------------
    // --------------- Plot the contours -------------------------------------------------
    // -----------------------------------------------------------------------------------

    // raise contour lines by this amount:
    real uRaise=0.; // *** .075*uScaleFacto/deltaUInverse; 

    real time0=getCPU();
    if( plotObject && gi.isInteractiveGraphicsOn() )
    {
      if( updateGeometry )
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
      }
      if( computePlotBounds )
      {
        computePlotBounds=false;
        getPlotBounds(gc,psp,xBound); 
      }
      
      gi.setAxesDimension(gc.numberOfDimensions());
      if( plotContours )
      {
 	// ====== plot contours =========
	if( plotOnThisProcessor )
	{
	  glDeleteLists(list,1);  // clear the plot
	  glNewList(list,GL_COMPILE);
	}
	
        contourOpt2d(gi,uGCF,psp,uMin,uMax,uRaise,recomputeVelocityMinMax,contourLevelsSpecified,xBound );

	// set current min max of contour values
        if( !psp.plotObjectAndExit &&  plotOnThisProcessor )
          dialog.setTextLabel("min max",sPrintF(answer,"%g %g",uMin,uMax));

 	// -------Now plot the boundaries.-------
	if( plotGridBoundariesOnContourPlots )
 	{
   	  int colourOption = 0;
	  int plotNonPhysicalBoundariesOld; psp.get(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundariesOld);
	  psp.plotNonPhysicalBoundaries=false;
	  plotGridBoundaries(gi, gc, boundaryConditionList, numberOfBoundaryConditions,
 			     colourOption, uRaise, psp);
	  psp.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES,plotNonPhysicalBoundariesOld);
 	}
	
	if( plotOnThisProcessor )
	{
	  glPopName();
	  glEndList();
	}
	
      }

      // -------- Draw the labels -----
      if( plotTitleLabels && plotOnThisProcessor )
      {
 	// plot labels on top and bottom
 	aString topLabel=psp.topLabel;       // remember original values
	
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
      if( plotColourBar && plotOnThisProcessor )
      {
	
	gi.displayColourBar(numberOfContourLevels,contourLevels,uMin,uMax,psp);
	// drawColourBar(numberOfContourLevels,contourLevels,uMin,uMax,psp);
      }
      
      gi.redraw();
      plotContours=true;

      real time3=getCPU();
      if( showTimings ) printf("contour2d: Total time=%8.2e\n",time3-time0);

    }
  }
  delete [] menu;

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  
  psp.adjustGridForDisplacement = adjustGridForDisplacementLocal;  // now set this 
  
//   windowScaleFactor[0]=1.;  // reset these values
//   windowScaleFactor[1]=1.;
//   windowScaleFactor[2]=1.;
  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)
}


static MappingInformation *mapInfoPointer=NULL;   // -- could add to OvertureInit

void PlotIt::
contourCuts(GenericGraphicsInterface &gi, const realGridCollectionFunction & u, GraphicsParameters & parameters)
// ==========================================================================================
//  Plot the solution along a line that passes through a 2d or 3d grid function
// ==========================================================================================
{
  const GridCollection & gc = *(u.gridCollection);
  const int numberOfGrids = gc.numberOfComponentGrids();
  const int numberOfDimensions=gc.numberOfDimensions();
  
  GraphicsParameters localParameters;
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  char buff[80];
  aString answer,answer2;
  aString menu[]= { "!contour cuts",
		    "specify lines",
		    "specify a boundary",
		    "build a curve",
		    "choose a curve",
		    "plot",
		    "use normalized distance",
		    "use actual distance",
                    "turn off grids for interpolation",
                    "set bogus value for points not interpolated",
                    "clip to boundary",
		    "exit this menu",
		    "" };

  bool plotObject=false;
  int & numberOfLines          = psp.numberOfLines;
  int & numberOfPointsPerLine  = psp.numberOfPointsPerLine;
  RealArray & linePlotEndPoints= psp.linePlotEndPoints;

  bool useNormalizedDistanceForContourCuts=true;
  const int numberOfComponents=u.getComponentDimension(0);
  const int numberOfComponentsToPlot = numberOfComponents+gc.numberOfDimensions();  // plot (x,y,z) coords too
  // bool clipToBoundary=false; // if true, only allow interpolation from inside grids, not ghost
  
  bool bogusValueIsSet=false;
  real bogusValue=0.;

  Range R3(0,2);
  Range C(u.getComponentBase(0),u.getComponentBound(0));
  RealArray x,uInterpolated;
  IntegerArray wasInterpolated;
  IntegerArray checkTheseGrids(gc.numberOfComponentGrids()); checkTheseGrids=true;

  IntegerArray lineInfo(5,1); lineInfo=0;
  Mapping *curveMapping=NULL;
  
  const bool & adjustMappingForDisplacement = psp.dbase.get<bool>("adjustMappingForDisplacement");
  if( parameters.adjustGridForDisplacement && !adjustMappingForDisplacement )
  {
    printF("contourCuts:ERROR: adjustGridForDisplacement=true but adjustMappingForDisplacement=false\n"
	   " The line plots will not be correct. You should choose option `adjust mapping for displacement'\n");
    gi.stopReadingCommandFile();
  }


  // set default prompt
  gi.appendToTheDefaultPrompt("linePlot>");

  GUIState dialog;
  if( !psp.plotObjectAndExit )
  {
    dialog.buildPopup(menu);
    gi.pushGUI( dialog );
  }

  InterpolatePoints interp;
  // int len=0;
  
  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==true
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
      gi.getAnswer(answer, "");

    if( answer=="specify lines" )
    {
      numberOfLines=1;
      numberOfPointsPerLine=20;
      
      gi.inputString(answer2,sPrintF(buff,"Enter number of lines and number of points per line (default=1,20)")); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i %i",&numberOfLines, &numberOfPointsPerLine);
      }
      numberOfPointsPerLine=max(2,numberOfPointsPerLine);
      if( numberOfLines>0)
      {
	linePlotEndPoints.redim(2,3,numberOfLines);
        lineInfo.redim(4,numberOfLines);
	lineInfo=0;
      }
      
      for( int i=0; i<numberOfLines; i++ )
      {
	if( numberOfDimensions==2 )
	{
	  gi.inputString(answer2,sPrintF(buff,"Line %i: enter `x0,y0, x1,y1'",i)); 
	  if( answer2 !="" && answer2!=" ")
	  {
	    sScanF(answer2,"%e %e %e %e",
		   &linePlotEndPoints(Start,0,i),&linePlotEndPoints(Start,1,i), 
		   &linePlotEndPoints(End  ,0,i),&linePlotEndPoints(End  ,1,i));
	  }
	}
	else
	{
	  gi.inputString(answer2,sPrintF(buff,"Line %i: enter the endpoints (x0,y0,z0) and (x1,y1,z1) (6 values)")); 
	  if( answer2 !="" && answer2!=" ")
	  {
	    sScanF(answer2,"%e %e %e %e %e %e",
		   &linePlotEndPoints(Start,axis1,i),&linePlotEndPoints(Start,axis2,i),&linePlotEndPoints(Start,axis3,i), 
		   &linePlotEndPoints(End  ,axis1,i),&linePlotEndPoints(End  ,axis2,i),&linePlotEndPoints(End  ,axis3,i));
	  }
	}
      }
      plotObject=true;
    }
    else if( answer=="specify a boundary" )
    {
      // ****** Define a curve from the boundary of a grid **************
      numberOfLines=1;
      numberOfPointsPerLine=20;
      int i=0;
      lineInfo.redim(5,1); lineInfo=0;
      if( numberOfDimensions==2 )
      {
	gi.outputString("A line may be specified as the boundary of a grid\n"
			"   Enter `numberOfPoints grid side axis'\n");
        for( int grid=0; grid<gc.numberOfBaseGrids(); grid++ )
	{
          for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    for( int side=0; side<=1; side++ )
	    {
              if( gc[grid].boundaryCondition(side,axis)>0 )
		printf(" base grid %i (%s) bc(side=%i,axis=%i) = %i share=%i\n",
		       grid,(const char*)gc[grid].getName(),side,axis,gc[grid].boundaryCondition(side,axis),
		       gc[grid].sharedBoundaryFlag(side,axis));
	    }
	  }
	}
        for( ;; )
	{
	  gi.inputString(answer2,"Enter `numberOfPoints grid side axis'"); 

	  if( answer2!="" )
	  {
	    int grid=-1, side=-1, axis=-1;
	    sScanF(answer2,"%i %i %i %i",&numberOfPointsPerLine,&grid,&side,&axis);
	    if( grid>=0 && grid<gc.numberOfGrids() && side>=0 && side<=1 && axis>=0 && axis<gc.numberOfDimensions())
	    {
	      lineInfo(0,i)=1;    // this means the "line" is a boundary of a grid
	      lineInfo(1,i)=grid;
	      lineInfo(2,i)=side;
	      lineInfo(3,i)=axis;
	      printf("INFO:The chosen grid=%i (%s) side=%i axis=%i has bc=%i and share=%i\n",grid,
		     (const char*)gc[grid].getName(),side,axis,gc[grid].boundaryCondition(side,axis),
		     gc[grid].sharedBoundaryFlag(side,axis));
              break;
	    }
	    else
	    {
	      printf("ERROR: invalid values: grid=%i, side=%i axis=%i\n",grid,side,axis);
	      gi.stopReadingCommandFile();
	      i--;
	      continue;
	    }
	  }
          else
	  {
            break;
	  }
	}
	plotObject=true;
      }
      else
      {
	gi.outputString("Sorry: this option is currently only implemented for 2D grids");
      }
      
    }
    else if( answer=="build a curve" )
    {
      gi.outputString("Build a curve for plotting results on.\n"
                      "All the mappings from the GridCollection have been made available.\n"
                      "You could for example build a spline or use the `reduce domain dimension' "
                      "to choose a line from a grid\n");
      if( mapInfoPointer==NULL )
      {
	mapInfoPointer = new MappingInformation;  // this will never be deleted -- could add to OvertureInit
        // Add all Mapping's from the grids.
	for( int grid=0; grid<gc.numberOfBaseGrids(); grid++ )
	{
	  mapInfoPointer->mappingList.addElement(gc[grid].mapping());
	}
      }
      MappingInformation & mapInfo = *mapInfoPointer;
      mapInfo.graphXInterface=&gi;

      
      createMappings( mapInfo );  // allow the user to create a curve
      plotObject=false;
    }
    else if( answer=="choose a curve" )
    {
      if( mapInfoPointer==NULL || mapInfoPointer->mappingList.getLength()==0 )
      {
	gi.outputString("You should first `build a curve' before you can choose a curve.");
        continue;
      }
      MappingInformation & mapInfo = *mapInfoPointer;
      mapInfo.graphXInterface=&gi;
	
      numberOfLines=1;
      numberOfPointsPerLine=20;
      lineInfo.redim(5,1); lineInfo=0;

      gi.inputString(answer2,sPrintF(buff,"Enter the number of points per line (default=20)")); 
      if( answer2 !="" && answer2!=" ")
      {
	sScanF(answer2,"%i",&numberOfPointsPerLine);
      }
      //kkc 060811 anything less than 2 will use the curve's grid      
      numberOfPointsPerLine=max(1,numberOfPointsPerLine);

      gi.outputString("Choose a curve for plotting results on");

      // Make a menu with the Mapping names (only curves)
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==1 && map.getRangeDimension()==gc.numberOfDimensions() )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(Mapping::mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      for(;;)
      {
	int mapNumber = gi.getMenuItem(menu2,answer2);
	if( mapNumber<0 )
	{
	  printf("contour::ERROR: unknown mapping to use\n");
	  gi.stopReadingCommandFile();
	  break;
	}
	if( answer2=="none" )
	{
	  numberOfLines=0;
	  break;
	}
	mapNumber=subListNumbering(mapNumber);  // map number in the original list
	curveMapping=mapInfo.mappingList[mapNumber].mapPointer;
	if ( numberOfPointsPerLine==1 ) numberOfPointsPerLine=curveMapping->getGridDimensions(0);
        lineInfo(0,0)=2; // this means use curveMapping
	plotObject=true;
        break;
      }
      delete [] menu2;
    }
    else if( answer=="use normalized distance" )
    {
      useNormalizedDistanceForContourCuts=true;
    }
    else if( answer=="use actual distance" )
    {
      useNormalizedDistanceForContourCuts=false;
      printf("INFO: actual distance can only be used if only 1 line is plotted\n");
    }
    else if( answer=="set bogus value for points not interpolated" )
    {
      gi.inputString(answer2,"Enter the bogus value");
      sScanF(answer2,"%e",&bogusValue);
      bogusValueIsSet=true;
    }
    else if( answer=="clip to boundary" )
    {
      printF("Turn on clip to boundary : only allow interpolation from inside grids (not ghost points)\n");
      // clipToBoundary=true;
      // default is widthInGridLines=2.5
      real widthInGridLines=1.e-3; // allow points this far outside (in grid lines) 
      interp.setInterpolationOffset(widthInGridLines);
    }
    else if( answer=="turn off grids for interpolation" )
    {
      for(;;)
      {
        int grid=-1;
	gi.inputString(answer2,"Enter the grid to turn off (-1 to finish)");
	sScanF(answer2,"%i",&grid);
	if( grid>=0 && grid<gc.numberOfComponentGrids() )
	{
          printF("Do not use grid %i (%s) for interpolation.\n",grid,(const char*)gc[grid].getName());
	  checkTheseGrids(grid)=false;
	}
	else
	{
          break;
	}
      }
      
    }
    else if( answer=="plot" )
    {
      if( numberOfLines<=0 )
	printf("PlotIt::contourCuts:INFO: You should `specify lines' before calling `plot'\n");
      else
        plotObject=true;
    }
    else if( answer=="exit this menu" || answer=="exit" )
    {
      break;
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
    }
    if( plotObject )
    {
      gi.erase();

      // now compute x and u
      Range R(0,numberOfPointsPerLine-1);
      Range Rc(0,numberOfComponentsToPlot*numberOfLines-1);
      uInterpolated.redim(R,Rc);
      x.redim(numberOfPointsPerLine,3);
      wasInterpolated.redim(0);
      RealArray uI(R,C);
      int i;
      for( i=0; i<numberOfLines; i++ )
      {
        if( lineInfo(0,i)==0 )
	{
	  // this is a line in space
	  for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	    x(R,axis).seqAdd(linePlotEndPoints(Start,axis,i),
			     (linePlotEndPoints(End,axis,i)-linePlotEndPoints(Start,axis,i))/(numberOfPointsPerLine-1.));
	}
	else if( lineInfo(0,i)==1 )
	{
          // Choose the side of a grid
          // this is only for 2D for now

	  int grid=lineInfo(1,i);
	  int side=lineInfo(2,i);
	  int axis=lineInfo(3,i);
         
          MappedGrid & mg = (MappedGrid&)gc[grid];
	  RealArray r(numberOfPointsPerLine,numberOfDimensions);
          int axisp1 = (axis+1) % numberOfDimensions;
          real dr=1./max(1,numberOfPointsPerLine-1);
          r(R,axisp1).seqAdd(0.,dr);
	  r(R,axis)=(real)side;
  	  mg.mapping().mapS(r,x);
	}
	else if( lineInfo(0,i)==2 )
	{
	  // **** use a curve ****
          assert( curveMapping!=NULL );
	  if ( numberOfPointsPerLine>1 ) 
	  {
	    RealArray r(numberOfPointsPerLine,1);
	    real dr=1./max(1,numberOfPointsPerLine-1);
	    r(R,0).seqAdd(0.,dr);
	    curveMapping->mapS(r,x);
	  }
	  else // kkc 060811
	  {
	    x.redim(0);
	    x = curveMapping->getGridSerial();
	    x.resize(x.getLength(0),x.getLength(3));
	  }

          printF("contour:INFO: curve extends from (xMin,yMin)=(%e,%e) to (xMax,yMax)=(%e,%e)\n",
		 min(x(R,0)),min(x(R,1)),max(x(R,0)),max(x(R,1)));
	}
	else
	{
	  printF("ERROR: unknown value for lineInfo(0,%i)=%i\n",i,lineInfo(0,i));
	  Overture::abort("ERROR");
	}
	
	// interpolate u at these points
	if( false &&  psp.adjustGridForDisplacement )
	{
          // Here is the old way that uses the grid points inly to invert --
          // This should work when the grid is adjusted for displacements
     	  printf("PlotIt::contourCuts:INFO:Use OLD interpolatePoints to handle .adjustGridForDisplacement\n");

	  wasInterpolated.redim(R);
	  interpolatePoints(x,u,uI,
			    C,nullRange,
			    nullRange,nullRange,nullRange,
                            Overture::nullIntegerDistributedArray(),
			    Overture::nullIntegerDistributedArray(),
			    wasInterpolated );
        }
	else 
	{
	  if( false )
	  {
	    interp.interpolatePoints(x, u, uI, C);
	  }
	  else
	  { // Here we can specify which grids to use for interpolation:
	    interp.buildInterpolationInfo(x,(CompositeGrid&)gc,NULL,&checkTheseGrids );
	    interp.interpolatePoints(u,uI,C);
	  }
	

	  wasInterpolated=interp.getStatus();
	  wasInterpolated=wasInterpolated>0; // ==int(InterpolatePoints::interpolated);
	}
	
//          interpolatePoints(x, u, uI, C,  // uI has same components are u!
//  			  nullRange,
//  			  nullRange,
//  			  nullRange,
//  			  nullRange,
//  			  Overture::nullIntegerDistributedArray(),
//  			  Overture::nullIntegerDistributedArray(),
//  			  wasInterpolated);

        uInterpolated(R,C-C.getBase()+i*numberOfComponentsToPlot)=uI(R,C);

        // *wdh* 050622
        // Add the x-coordinates to the list of components that can be plotted

        const int m0 = i*numberOfComponentsToPlot+numberOfComponents;
	for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	{
	  uInterpolated(R,m0+axis)=x(R,axis);
	}
	
        // x.display("here is x");
	// uInterpolated.display("uInterpolated");
	
        if( numberOfLines>1 || bogusValueIsSet )
	{ 
          // we do not mask out bad values if there are more than 1 line plotted --- put some
          // bogus values in
	  for( int c=C.getBase(); c<=C.getBound(); c++ )
	  {
	    real uMin;
	    if( bogusValueIsSet )
	    {
	      uMin=bogusValue;
	    }
	    else
	    {
	      where( wasInterpolated(R) )
		uMin=min(uInterpolated(R,c+i*numberOfComponentsToPlot));
	    }
	    
	    // printF("contourCuts: Setting bogus values to %e\n",uMin);
	    // ::display(wasInterpolated,"wasInterpolated");
	    
	    where( !wasInterpolated(R) )
	    {
	      uInterpolated(R,c+i*numberOfComponentsToPlot)=uMin;
	    }
            // ::display(uInterpolated(R,c+i*numberOfComponentsToPlot),"uInterpolated(R,c+i*numberOfComponentsToPlot)");
	  }
	}
      }
      

      // define an equally spaced grid on the unit interval
      realArray t;
      t.redim(R);
      real length=1.;
      if( lineInfo(0,0)==0 )
      {
	if( numberOfLines==1 && !useNormalizedDistanceForContourCuts )
	{
	  Range Rx=gc.numberOfDimensions();
	  length=sqrt(sum(SQR(linePlotEndPoints(End,Rx,0)-linePlotEndPoints(Start,Rx,0))));
	  t.seqAdd(0.,length/(numberOfPointsPerLine-1));
	}
	t.seqAdd(0.,length/(numberOfPointsPerLine-1));
      }
      else
      {
        // use arclength for curves
        useNormalizedDistanceForContourCuts=false;
	
        t(0)=0;
	if( gc.numberOfDimensions()==2 )
	{
	  for( int i=1; i<numberOfPointsPerLine; i++ )
	    t(i)=t(i-1)+sqrt( SQR(x(i,0)-x(i-1,0))+
			      SQR(x(i,1)-x(i-1,1)) );
	}
	else
	{
	  for( int i=1; i<numberOfPointsPerLine; i++ )
	    t(i)=t(i-1)+sqrt( SQR(x(i,0)-x(i-1,0))+
			      SQR(x(i,1)-x(i-1,1))+
			      SQR(x(i,2)-x(i-1,2)) );
	}
	
      }
      

      DataPointMapping line;
      t.reshape(1,R);
      line.setDataPoints(t,0,1);  // 0=position of coordinates, 1=domain dimension
      
      MappedGrid c(line);   // a grid
      c.update(MappedGrid::THEvertex | MappedGrid::THEmask);

      // ::display(t,"Here is t -- parameterization");

      // *** we can only set the mask if there is 1 line plotted ***
      if( numberOfLines==1 )
#ifndef USE_PPP
        c.mask()(R+c.indexRange()(Start,axis1))=wasInterpolated(R);
#else
      for( i=R.getBase(); i<=R.getBound(); i++ )
        c.mask()(i+c.indexRange()(Start,axis1))=wasInterpolated(i);  
#endif
      Range all;
      int nv=numberOfComponentsToPlot*numberOfLines;
      realMappedGridFunction uu(c,all,all,all,nv);
      Range I1(c.indexRange()(Start,axis1),c.indexRange()(End,axis1));
#ifndef USE_PPP
      for( i=0; i<nv; i++ )
	uu(I1,all,all,i)=uInterpolated(R,i);
#endif

      // set component names
      for(i=0; i<numberOfLines; i++ )
      {
	for( int c=C.getBase(); c<=C.getBound(); c++ )
	{
	  if( numberOfLines==1 )
	    uu.setName(u.getName(c),c);
	  else
	    uu.setName(sPrintF(buff,"%s_line_%i",(const char *)u.getName(c),i),c+i*numberOfComponentsToPlot);
	  //kkc 060811 this breaks matlab output	    uu.setName(sPrintF(buff,"%s (line %i)",(const char *)u.getName(c),i),c+i*numberOfComponentsToPlot);
	}
        for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	{ // Give names to the components that represent (x,y,z) along the curve
	  int c=C.getBound()+axis+1;
	  if( numberOfLines==1 )
	    uu.setName(sPrintF(buff,"x%i",axis),c);
	  else
	    uu.setName(sPrintF(buff,"x%i_line_%i",axis,i),c+i*numberOfComponentsToPlot);
	    //kkc 060811 this breaks matlab output	  uu.setName(sPrintF(buff,"x%i (line %i)",axis,i),c+i*numberOfComponentsToPlot);
	}
	
      }
      aString title[3];
      
      if( numberOfLines==1 )
      {
        if( gc.numberOfDimensions()>=2 )
	{
          if( lineInfo(0,0)==0 )
	  {
	    title[0] = sPrintF(buff,"line: (%g,%g) to (%g,%g)", 
			       linePlotEndPoints(Start,0,0),linePlotEndPoints(Start,1,0), 
			       linePlotEndPoints(End  ,0,0),linePlotEndPoints(End  ,1,0));
	  }
	  else if( lineInfo(0,0)==1 )
	  {
            int grid=lineInfo(1,0), side=lineInfo(2,0), axis=lineInfo(3,0);
	    title[0] = sPrintF(buff,"line: grid %i (%s) (side,axis)=(%i,%i)", 
			       grid,(const char*)gc[grid].getName(),side,axis);
	  }
	  else if( lineInfo(0,0)==2 )
	  {
            int grid=lineInfo(1,0), side=lineInfo(2,0), axis=lineInfo(3,0);
	    title[0] = sPrintF(buff,"curve %s",(const char*)curveMapping->getName(Mapping::mappingName));
	  }
	  
	}
        else
  	  title[0] = sPrintF(buff,"line: (%g,%g,%g) to (%g,%g,%g)", 
			  linePlotEndPoints(Start,0,0),linePlotEndPoints(Start,1,0),linePlotEndPoints(Start,2,0), 
			  linePlotEndPoints(End  ,0,0),linePlotEndPoints(End  ,1,0),linePlotEndPoints(End  ,2,0));
      }
      else
      {
        for( int i=0; i<min(3,numberOfLines); i++ )
	{
	  if( gc.numberOfDimensions()==2 )
	  {
	    title[i] = sPrintF(buff,"line %i: (%g,%g) to (%g,%g)",i,
			       linePlotEndPoints(Start,0,i),linePlotEndPoints(Start,1,i), 
			       linePlotEndPoints(End  ,0,i),linePlotEndPoints(End  ,1,i));
	  }
	  else
	    title[i] = sPrintF(buff,"line %i: (%g,%g,%g) to (%g,%g,%g)",i,
			    linePlotEndPoints(Start,0,i),linePlotEndPoints(Start,1,i),linePlotEndPoints(Start,2,i), 
			    linePlotEndPoints(End  ,0,i),linePlotEndPoints(End  ,1,i),linePlotEndPoints(End  ,2,i));
	}
      }
      
      aString topLabel1=psp.topLabel1, topLabel2=psp.topLabel2, topLabel3=psp.topLabel3;
      psp.set(GI_TOP_LABEL_SUB_1,title[0]);
      psp.set(GI_TOP_LABEL_SUB_2,title[1]);
      psp.set(GI_TOP_LABEL_SUB_3,title[2]);

      bool colourLineContours=psp.colourLineContours;
      psp.set(GI_COLOUR_LINE_CONTOURS,true);
  
      aString xLabel=gi.getXAxisLabel();
      // psp.set(GI_X_AXIS_LABEL,"normalized distance");
      if( numberOfLines==1 && !useNormalizedDistanceForContourCuts )
      {
  	gi.setAxesLabels("distance");
      }
      else
	gi.setAxesLabels("normalized distance");

      //  We should not adjust for displacement 2 times for the same grid! *wdh* 2014/04/22
      const bool adjustGridForDisplacementSave = parameters.adjustGridForDisplacement;
      
      parameters.adjustGridForDisplacement=false;
      
      contour(gi,uu,psp);

      // reset
      parameters.adjustGridForDisplacement=adjustGridForDisplacementSave;
      
      psp.set(GI_TOP_LABEL_SUB_1,topLabel1);
      psp.set(GI_TOP_LABEL_SUB_2,topLabel2);
      psp.set(GI_TOP_LABEL_SUB_3,topLabel3);
      psp.set(GI_COLOUR_LINE_CONTOURS,colourLineContours);
      gi.setAxesLabels(xLabel[gi.getCurrentWindow()]);
    }
  }

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  
}





//--------------------------------------------------------------------------------------
//  Plot contours of a grid function in 1D
//
// ************ plot multiple components *****
//--------------------------------------------------------------------------------------
void PlotIt::
contour1d(GenericGraphicsInterface &gi, const realGridCollectionFunction & uGCF, GraphicsParameters & parameters)
{

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

  // save the current window
  int startWindow = gi.getCurrentWindow();

  GridCollection & gc = *(uGCF.gridCollection);
  const int numberOfGrids = gc.numberOfComponentGrids();

  gc.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

  char buff[80];
  aString answer,answer2;
  aString menu0[]= { "!contour plotter (1d)",
                    "erase and exit",
                    "plot",
                    ">choose a component",
                    "<colour line contours (toggle)",
                    ">add a component",
                     "<set bounds",
                    "toggle grids on and off",
		    "plot grid points (toggle)",
                    "plot line markers (toggle)",
                    "plot the grid",
                    "ghost lines",
		    "save results to a matlab file",
		    "save results to a text file",
		    "user defined output",
                    " ",
//                    "plot labels (toggle)",
//                    "plot the axes (toggle)",
                    "set origin for axes",
//                    "plot the back ground grid (toggle)",
                    "erase",
                    "erase and exit",
                    "exit this menu",
                    "" };

  // create the real menu by adding in the component names, these will appear as a 
  // cascaded menu
  int chooseAComponentMenuItem;  // menu[chooseAComponentMenuItem]=">choose a component"
  int addAComponentMenuItem;  // menu[addAComponentMenuItem]=">add a component"
  int numberOfMenuItems0=0;
  while( menu0[numberOfMenuItems0]!="" )
  {
    numberOfMenuItems0++;
  }  
  numberOfMenuItems0++;


  int numberOfComponents=uGCF.getComponentDimension(0);
  aString *menu = new aString [numberOfMenuItems0+2*numberOfComponents];
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
    if( menu[i]==">add a component" )
    {
      addAComponentMenuItem=i;
      for( int j=0; j<numberOfComponents; j++ )
      {
        menu[++i]="add "+uGCF.getName(uGCF.getComponentBase(0)+j);
        if( menu[i] == "add " || menu[i]=="add  " )
          menu[i]=sPrintF(buff,"add component%i",uGCF.getComponentBase(0)+j);
      }
    }
  }

  int list=0, labelList=0;
  if( gi.isGraphicsWindowOpen() )
  {
    list=gi.generateNewDisplayList();  // get a new display list to use
    assert(list!=0);
    labelList=gi.getNewLabelList(gi.getCurrentWindow());
    assert( labelList!=0 );
  }
  // If the user has passed a parameters object then we use it -- otherwise we
  // use a local copy (we cannot use the default "parameters" because we may change it!)
  GraphicsParameters localParameters(true);  // true means this is gets default values
  GraphicsParameters & psp = parameters.isDefault() ? localParameters : parameters;

  int  & component                = psp.componentForContours;
  IntegerArray & gridsToPlot      = psp.gridsToPlot;
  bool & colourLineContours       = psp.colourLineContours;
  IntegerArray & componentsToPlot = psp.componentsToPlot;
  bool & plotGridPointsOnCurves   = psp.plotGridPointsOnCurves;

  bool plotColourBarSaved = psp.plotColourBar;
  psp.plotColourBar=false;
  if( gi.isInteractiveGraphicsOn() )
    gi.eraseColourBar();
  
  bool plotLineMarkers=false;
  isHiddenByRefinement = psp.plotHiddenRefinementPoints ? 0 : MappedGrid::IShiddenByRefinement;

  // Make a local copy of this:
  bool plotObject             = psp.plotObject;

  if( psp.isDefault() )
  { // user has NOT supplied parameters, so we set them to default
    gridsToPlot.redim(numberOfGrids);  gridsToPlot=GraphicsParameters::toggleSum;
  }
  else
  {
    if( gridsToPlot.getLength(0) < numberOfGrids )
    { // make enough room in this array:
      gridsToPlot.redim(numberOfGrids);  gridsToPlot=GraphicsParameters::toggleSum;
    }    
  }
  
// AP: Disabling this off for now (1/4/02)
//    if( lighting[currentWindow] ) // should this really be done here?
//    {
//      // When lighting is on we need to turn on material properties
//      glColorMaterial(GL_FRONT,GL_DIFFUSE);   // this causes the grids to reflect according to their colour
//      glColorMaterial(GL_FRONT,GL_AMBIENT); 
//      glEnable(GL_COLOR_MATERIAL);
//    }

  const int numberOfLineColours=6, numberOfLineMarkers=16;
  aString lineColour[numberOfLineColours] = {"blue","red","green","yellow","violetred","orange"};
  aString lineMarker[numberOfLineMarkers] = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P"};

  if( componentsToPlot.getLength(0)==0 )
  {
     // plot this component by default:
    component = min(max(component,uGCF.getComponentBase(0)),uGCF.getComponentBound(0)); 
    // plot these components by default:
    componentsToPlot.redim(1);
    componentsToPlot=component;
  }
  else
  { // make sure compnents to plot are valid:
    for( int n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++ )
    {
      componentsToPlot(n) = min(max(componentsToPlot(n),uGCF.getComponentBase(0)),uGCF.getComponentBound(0));
    }
  }
  
  bool userDefinedBounds=false; // has user defined bounds for plot
  real userMin=0.0, userMax=1.0;

  real uMin=0.,uMax=0.;
  aString yAxisLabelSaved=gi.getYAxisLabel();
  RealArray xBound(2,3); 

  const bool plotBackGroundGridSave = gi.getPlotTheBackgroundGrid(startWindow);
  gi.setPlotTheBackgroundGrid(true);
  
  GUIState dialog;
  if( !psp.plotObjectAndExit )
  {
    dialog.buildPopup(menu);
    gi.pushGUI( dialog );
  }

  // set default prompt
  gi.appendToTheDefaultPrompt("contour>");
  int menuItem=-1;

  for( int it=0; ;it++)
  {
    if( it==0 && (plotObject || psp.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==true
    else if( it==1 && psp.plotObjectAndExit )
      answer="exit this menu";
    else
      menuItem=gi.getAnswer(answer, "");

// make sure the currentWindow is the same as on entry
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( menuItem > chooseAComponentMenuItem && menuItem <= chooseAComponentMenuItem+numberOfComponents )
    {
      component=menuItem-chooseAComponentMenuItem-1 + uGCF.getComponentBase(0);
      // cout << "chose component number=" << component << endl;
      componentsToPlot.resize(1);
      componentsToPlot(0)=component;
    }
    else if( menuItem > addAComponentMenuItem && menuItem <= addAComponentMenuItem+numberOfComponents )
    {
      component=menuItem-addAComponentMenuItem-1 + uGCF.getComponentBase(0);
      // cout << "add a component, component = " << component << endl;
      for( int n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++ )
      {
	if( component==componentsToPlot(n) )
	{
          gi.outputString("contour1d: This component has already been plotted");
	  component=-1;
          break;
	}
      }
      if( component>=0 )
      {
        componentsToPlot.resize(componentsToPlot.getLength(0)+1);
        componentsToPlot(componentsToPlot.getLength(0)-1)=component;
      }
    }
//      else if( answer=="choose a component" )
//      { // **** not used *** Make a menu with the component names. If there are no names then use the component numbers
//        int numberOfComponents=uGCF.getComponentDimension(0);
//        aString *menu2 = new aString[numberOfComponents+1];
//        for( int i=0; i<numberOfComponents; i++ )
//        {
//          menu2[i]=uGCF.getName(uGCF.getComponentBase(0)+i);
//          if( menu2[i] == "" || menu2[i]==" " )
//            menu2[i]=sPrintF(buff,"component%i",uGCF.getComponentBase(0)+i);
//        }
//        menu2[numberOfComponents]="";   // null string terminates the menu
//        component = gi.getMenuItem(menu2,answer2);
//        component+=uGCF.getComponentBase(0);
//        // cout << "chose component number=" << component << endl;
//        delete [] menu2;
//      }
    else if( answer=="toggle grids on and off" )
    {
      aString answer2;
      aString *menu2 = new aString[numberOfGrids+2];
      for(;;)
      {
	for( int grid=0; grid<numberOfGrids; grid++ )
	{
	  menu2[grid]=sPrintF(buff,"%i : %s is (%s)",grid,
                                (const char*)gc[grid].mapping().getName(Mapping::mappingName),
				(gridsToPlot(grid) & GraphicsParameters::toggleContours ? "on" : "off"));
	}
	menu2[numberOfGrids]="exit this menu";
	menu2[numberOfGrids+1]="";   // null string terminates the menu
        gi.getMenuItem(menu2,answer2);
        if( answer2=="exit this menu" )
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
    else if(answer=="colour line contours (toggle)")
    {
      colourLineContours= !colourLineContours;
      plotLineMarkers=true;
    }
    else if( answer=="plot line markers (toggle)" )
    {
      plotLineMarkers= !plotLineMarkers;
    }
    else if( answer=="plot grid points (toggle)" )
    {
      plotGridPointsOnCurves= !plotGridPointsOnCurves;
    }
    else if( answer=="plot the grid" )
    {
      plot(gi, (GridCollection&)gc, psp);
    }
    else if( answer=="erase" )
    {
      // plotObject=false;
      if( plotOnThisProcessor ) glDeleteLists(list,1);
      gi.redraw();
    }
    else if( answer=="ghost lines" )
    {
      gi.inputString(answer,
               sPrintF("Enter the number of ghost lines to plot (current=%i)",psp.numberOfGhostLinesToPlot));
      sScanF(answer,"%i",&psp.numberOfGhostLinesToPlot);
      printf("Plotting %i ghost lines",psp.numberOfGhostLinesToPlot);
      plotObject=true;
    }
    else if( answer == "set bounds" )
    {
      userDefinedBounds = true;
      gi.inputString(answer2,sPrintF("Enter min and max bounds for solution."));
      sScanF(answer2,"%e %e",&userMin, &userMax);
    }
    else if( answer=="exit this menu" )
    {
      break;
    }
    else if( answer=="erase and exit" )
    {
      plotObject=false;
      if( plotOnThisProcessor )
      {
	glDeleteLists(list,1);
	glDeleteLists(labelList,1);  
      }
      gi.redraw();
      break;
    }
    else if( answer=="set origin for axes" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter origin for axes (x,y,z) (enter return for default)")); 

      if( answer2 !="" && answer2!=" ")
      {
	real xo=GenericGraphicsInterface::defaultOrigin, 
	  yo=GenericGraphicsInterface::defaultOrigin, zo=GenericGraphicsInterface::defaultOrigin;
	sScanF(answer2,"%e %e %e",&xo, &yo, &zo);
        gi.setAxesOrigin(xo, yo, zo);	
      }
    }
    else if( answer=="plot" )
    {
      plotObject=true;
    }
    else if( answer=="save results to a matlab file" )
    {
      aString fileName="contour.m";
      gi.inputString(fileName,sPrintF(answer,"Enter the name of the matlab file (default=%s)\n",(const char*)fileName));
      if( fileName=="" )
	fileName="contour.m";

      FILE *matlabFile = fopen((const char*)fileName,"w" ); 

      aString label;
      psp.get(GI_TOP_LABEL,label);
      fprintf(matlabFile,"%% %s \n",(const char*)label);
      psp.get(GI_TOP_LABEL_SUB_1,label);
      fprintf(matlabFile,"%% %s \n",(const char*)label);

      if( psp.showFileReader!=NULL || psp.showFileParameters!=NULL )
      { // look for the current time

        real time=0.;
       
	if(  psp.showFileParameters!=NULL )
	{ // look for the time in the provided show file parameters
	  ListOfShowFileParameters & sfp = *((ListOfShowFileParameters*)psp.showFileParameters);
	  bool ok = sfp.getParameter("time",time);
	  printf(" linePlot: psp.showFileParameters found, time found? ok=%i\n",ok);
	}
	else if( psp.showFileReader!=NULL )
	{
	  // Look for parameters in the show file
	  // (These values are saved, for example, in OverBlown when the show file is written)

	  ShowFileReader & showFileReader = *((ShowFileReader*)psp.showFileReader);
// 	  bool found;
// 	  found=showFileReader.getGeneralParameter("pde",pdeName);
// 	  printf(" pdeName =%s (from ShowFile)\n",(const char*)pdeName);
      
// 	  found=showFileReader.getGeneralParameter("densityComponent",rc);
// 	  found=showFileReader.getGeneralParameter("uComponent",uc);
// 	  found=showFileReader.getGeneralParameter("vComponent",vc);
// 	  found=showFileReader.getGeneralParameter("wComponent",wc);
// 	  found=showFileReader.getGeneralParameter("temperatureComponent",tc);
// 	  found=showFileReader.getGeneralParameter("pressureComponent",pc);

// 	  found=showFileReader.getGeneralParameter("numberOfSpecies",numberOfSpecies);

// 	  found=showFileReader.getGeneralParameter("reactionType",reactionName);

	  // get parameters from the current frame
	  GenericDataBase *dbp= showFileReader.getFrame();
	  assert( dbp!=NULL );
	  GenericDataBase & db = *dbp;
	  db.get(time,"time");
          real dt;
	  db.get(dt,"dt");
	  printf(" linePlot: time=%e, dt=%e (from ShowFile)\n",time,dt);
      
	}

        fprintf(matlabFile,"time=%20.14e; \n",time);

      }
      
      int grid;
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	realMappedGridFunction & u = uGCF[grid];
	const RealDistributedArray & x = gc[grid].center();
        const IntegerDistributedArray & mask = gc[grid].mask();
        Index I1,I2,I3;
	getIndex(gc[grid].gridIndexRange(),I1,I2,I3);

	// ::display(x,"contour1d : Grid x for line plot");

	intArray cMask(I1);
        // Watch out for no ghost points when plotting sequences mask(I1+1) is bad.
	if( numberOfGrids>1 )
	{
	  cMask=mask(I1,I2,I3)!=0 && mask(I1+1,I2,I3)!=0 && x(I1)>=xBound(Start,0) && x(I1)<=xBound(End,0);
	  // on refinement grids do not plot cells with all corners hidden by refinement
	  if( gc.numberOfRefinementLevels()>1 )
	    cMask=cMask && !( (mask(I1  ,I2  ,I3) & isHiddenByRefinement) ||
			      (mask(I1+1,I2  ,I3) & isHiddenByRefinement) );
	}
	else
	{
	  cMask=1;
	}
	
        const int numPerLine=5;
	fprintf(matlabFile,"x%i=[",grid);
	int i1;
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  fprintf(matlabFile,"%17.10e ",x(i1,0,0,0));
          if( (i1 % numPerLine)==numPerLine-1 ) fprintf(matlabFile,"...\n");
	}
	fprintf(matlabFile,"];\n");

	for( int n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++ )
	{
	  component=componentsToPlot(n);
	  fprintf(matlabFile,"%s%i=[",(const char*)uGCF.getName(component),grid);

	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
            if( cMask(i1) )
	      fprintf(matlabFile,"%17.10e ",u(i1,0,0,component));
            else
              fprintf(matlabFile,"NaN ");  // mark unused point with NaN
            if( (i1 % numPerLine)==numPerLine-1 ) fprintf(matlabFile,"...\n");
	  }
	  fprintf(matlabFile,"];\n");
	}
      } // end for grid
      
      const int numberOfSymbols=11;
      aString symbol[numberOfSymbols]={"r-o","g-x","b-s","c-<","m->","r-+","g-o","b-x","c-s","m-<","r->"}; //

      fprintf(matlabFile,"%% Uncomment the next lines to create a plot\n");
      fprintf(matlabFile,"%% plot(");
      int m=0;
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	for( int n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++,m++ )
	{
	  component=componentsToPlot(n);
	  fprintf(matlabFile,"x0,%s%i,'%s'",(const char*)uGCF.getName(component),grid,
		  (const char*)symbol[(m%numberOfSymbols)]);
	  if( grid<numberOfGrids && n<componentsToPlot.getBound(0) ) fprintf(matlabFile,",");
	}
      }
      fprintf(matlabFile,");\n");
      fprintf(matlabFile,"%% legend(");
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	for( int n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++ )
	{
	  component=componentsToPlot(n);
          if( numberOfGrids==1 )
	    fprintf(matlabFile,"'%s'",(const char*)uGCF.getName(component));
	  else
           fprintf(matlabFile,"'%s (grid %i)'",(const char*)uGCF.getName(component),grid);
	  if( grid<numberOfGrids && n<componentsToPlot.getBound(0) )
	    fprintf(matlabFile,",");
	  else
	    fprintf(matlabFile,");\n");
	}
      }

      fclose(matlabFile);
      printf("Results saved to file %s\n",(const char*)fileName);
    }
    else if( answer=="save results to a text file" )
    {
//Then, on the first line of the file put
//t 5 1 1
//where t is the current time.  On the next line put
//m 1
//where m is the number of data points.  On the next line put
//spec-volume velocity    temperature pressure    product
//exactly as written.  Then, finally, put the data in the form
//x u1 u2 u3 u4 u5 0

      aString fileName="contour.dat";
      gi.inputString(fileName,sPrintF(answer,"Enter the name of the file (default=%s)\n",(const char*)fileName));

      real time=0.;
      gi.inputString(answer2,sPrintF("Enter t, the time for this solution (default=%9.3e)",time));
      sScanF(answer2,"%e",&time);
      
      int *cc = new int [numberOfComponents+10];
      aString *cName = new aString [numberOfComponents+10];
      int numberOfComponentsToPlot=0;
      char buff[200];
      for( ;; )
      {
        gi.inputString(answer2,sPrintF(buff,"Enter component number and name (enter `done' to finish)")); 
	if( answer2 == "done" ) break;
	int component=-1; 
        sScanF(answer2,"%i %s",&component,buff);
        if( component<0 || component>=numberOfComponents )
	{
	  printF("Invalid component number=%i! Wake up Don! Try again...\n");
	}
	else
	{
	  printf(" Will save component=%i, name=[%s]\n",component,buff);
	  cc[numberOfComponentsToPlot]=component;
	  cName[numberOfComponentsToPlot]=buff;
          printf(" Will save component=%i, name=[%s]\n",component,(const char*)cName[numberOfComponentsToPlot]);
  	  numberOfComponentsToPlot++;
	}
      }
      if( numberOfComponentsToPlot==0 ) continue;
      
      if( fileName=="" || fileName==" " ) fileName="contour.dat";
      FILE *file = fopen((const char*)fileName,"w" ); 


      fprintf(file,"%e %i %i\n%i\n",time,numberOfComponentsToPlot,1,1);
      int grid;
      for( grid=0; grid<numberOfGrids; grid++ )
      {
	realMappedGridFunction & u = uGCF[grid];
	const RealDistributedArray & x = gc[grid].center();
        const IntegerDistributedArray & mask = gc[grid].mask();
        Index I1,I2,I3;
	getIndex(gc[grid].gridIndexRange(),I1,I2,I3);

	fprintf(file,"%i %i\n",I1.getLength(),1);

	for( int n=0; n<numberOfComponentsToPlot; n++ )
	  fprintf(file,"%12s ",(const char*)cName[n]);
//	  fprintf(file,"%12.0s ",(const char*)cName[n]);
	fprintf(file,"\n");
	
        // *** finish me for parallel ***

	intArray cMask(I1);
	cMask=mask(I1,I2,I3)!=0 && mask(I1+1,I2,I3)!=0 && x(I1)>=xBound(Start,0) && x(I1)<=xBound(End,0);
	// on refinement grids do not plot cells with all corners hidden by refinement
	if( gc.numberOfRefinementLevels()>1 )
	  cMask=cMask && !( (mask(I1  ,I2  ,I3) & isHiddenByRefinement) ||
			    (mask(I1+1,I2  ,I3) & isHiddenByRefinement) );

	int i1;
	for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  fprintf(file,"%e ",x(i1,0,0,0));  // x holds the normalized distance
	  for( int n=0; n<numberOfComponentsToPlot; n++ )
	  {
	    fprintf(file,"%e ",u(i1,0,0,cc[n]));
	  }
	  
	  fprintf(file,"%i",(cMask(i1)? 0 : 1));
	  fprintf(file,"\n");
	}
	
      } // end for grid
      
      delete [] cc;
      delete [] cName;
      fclose(file);
      printf("Results saved to file %s\n",(const char*)fileName);

    }
    else if( answer=="user defined output" )
    {
      printf("Calling the function userDefinedOutput (by default in Overture/Ogshow/userDefinedOutput.C)\n"
             "See the comments in userDefinedOutput.C for how to output results in your favourite format\n");
      
      PlotIt::userDefinedOutput(uGCF,parameters,"contour1d");
    }
    else
    {
      cout << "Unknown response = " << answer << endl;
      gi.stopReadingCommandFile();
    }
    if( plotObject && gi.isInteractiveGraphicsOn() )
    {
      Index I1,I2,I3;
      int i1,i2,i3;

      glDeleteLists(list,1);  // clear the plot
      glDeleteLists(labelList,1);  

      // get Bounds on the grids
      // getPlotBounds(gc,psp,xBound);
      getGridBounds(gc,psp,xBound); // includes ghost lines if needed for plotting
      const real xScale=xBound(1,0)-xBound(0,0);

      xBound(0,0)-=xScale*REAL_EPSILON*100.;
      xBound(1,0)+=xScale*REAL_EPSILON*100.;
      


      // Get Bounds on u -- treat the general case when the component can be in any Index position of u
      int n;
      if( userDefinedBounds == true )
      {
	uMin = userMin;
	uMax = userMax;
      }
      else
      {
	for( n=componentsToPlot.getBase(0); n<=componentsToPlot.getBound(0); n++ )
	{
	  component=componentsToPlot(n);
	  real cMin,cMax;
	  getBounds(uGCF,cMin,cMax,parameters,Range(component,component));
	  uMin= n==componentsToPlot.getBase(0) ? cMin : min(uMin,cMin);
	  uMax= n==componentsToPlot.getBase(0) ? cMax : max(uMax,cMax);


	}
      }
       
      real deltaU = uMax-uMin;
      if( deltaU==0. )
      {
	uMax+=.5;
	uMin-=.5;
      }

      // --- put u into the xBound array ----
      xBound(Start,1)=uMin;
      xBound(End  ,1)=uMax;
      xBound(Start,2)=0.; xBound(End,2)=0.;

      // *wdh* 100628 : use plot bounds if specified
      if( parameters.usePlotBounds )
       xBound=parameters.plotBound;

      // ::display(xBound,"contour1d: xBound");
      
      gi.resetGlobalBound(gi.getCurrentWindow());
      gi.setGlobalBound(xBound);
      gi.setKeepAspectRatio(false);


      // plot labels on top and bottom
      if( parameters.plotTitleLabels )
      {
        gi.plotLabels( psp );
      }

//       int labelList=getFirstUserLabelDL(currentWindow);
//       glNewList(labelList,GL_COMPILE);
//       setColour("red"); //  label colour 
//       aString name="test";
      
//       label(name,.5,.75,.1,0,0.,psp);
//       glEndList();

      glNewList(list,GL_COMPILE); 

      int m;
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);  // Is this needed?

      for( int grid=0; grid<numberOfGrids; grid++ )
      {
	if( !(gridsToPlot(grid)&GraphicsParameters::toggleContours) )
	  continue;

	realMappedGridFunction & u = uGCF[grid];
	const RealDistributedArray & coord = gc[grid].center();
        const IntegerDistributedArray & mask = gc[grid].mask();
	getIndex(gc[grid].gridIndexRange(),I1,I2,I3);

        I1=Range(gc[grid].dimension(Start,axis1),gc[grid].dimension(End,axis1)-1);  // *wdh* 020331
	
	i2 = I2.getBase();
	i3 = I3.getBase();

        for(n=componentsToPlot.getBase(0),m=0; n<=componentsToPlot.getBound(0); n++,m++ )
	{
          component=componentsToPlot(n);
          if( colourLineContours )
            gi.setColour(lineColour[m%numberOfLineColours]);
          else
            gi.setColour(GenericGraphicsInterface::textColour);

	  intArray cMask(I1);
	  cMask=mask(I1,I2,I3)!=0 && mask(I1+1,I2,I3)!=0 && coord(I1)>=xBound(Start,0) && coord(I1)<=xBound(End,0)
                                                         && coord(I1+1)>=xBound(Start,0) && coord(I1+1)<=xBound(End,0);
	  // on refinement grids do not plot cells with all corners hidden by refinement
	  if( gc.numberOfRefinementLevels()>1 )
	    cMask=cMask && !( (mask(I1  ,I2  ,I3) & isHiddenByRefinement) ||
			      (mask(I1+1,I2  ,I3) & isHiddenByRefinement) );

	  glLineWidth(psp.size(GraphicsParameters::minorContourWidth)*psp.size(GraphicsParameters::lineWidth)*
		      gi.getLineWidthScaleFactor());
	  glBegin(GL_LINES); 
	  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
	    // if( mask(i1)!=0 && mask(i1+1)!=0 && coord(i1)>=xBound(Start,0) && coord(i1)<=xBound(End,0) )
	    if( cMask(i1) )
	    { // the .sa function converts to standard arguments (component can be in any position)
	      glVertex3f(coord(i1)  ,u.sa(  i1,i2,i3,component),1.e-3);
	      glVertex3f(coord(i1+1),u.sa(i1+1,i2,i3,component),1.e-3);
	    }
	  }
	  glEnd();
           // **** trouble with xLabel and aspect ratio
          if( plotLineMarkers && componentsToPlot.getLength(0)>1 )
	  {
	    // RealArray x(1,3),r(1,3);
            int skip = max(1,I1.length()/10);   // at most 10 markers on a line
	    for( i1=I1.getBase(); i1<I1.getBound(); i1++ )
	    {
	      if( (i1 % skip) == 0 && mask(i1)!=0 && mask(i1+1)!=0 
		  && coord(i1)>=xBound(Start,0) && coord(i1)<=xBound(End,0)  )
	      {
		gi.xLabel(lineMarker[m%numberOfLineMarkers],coord(i1+1),u.sa(i1+1,i2,i3,component),.025,0,0.,psp);
	      }
	    }
	  }

	  if( plotGridPointsOnCurves )
	  {
	    // *** plot points on curves ****
	    cMask=mask(I1,I2,I3)!=0 && coord(I1)>=xBound(Start,0) && coord(I1)<=xBound(End,0);
	    // on refinement grids do not plot cells with all corners hidden by refinement
	    if( gc.numberOfRefinementLevels()>1 )
	      cMask=cMask && !( (mask(I1  ,I2  ,I3) & isHiddenByRefinement) );

            gi.setColour(GenericGraphicsInterface::textColour);
	    glPointSize((1.+psp.size(GraphicsParameters::curveLineWidth))*psp.size(GraphicsParameters::lineWidth)*
			gi.getLineWidthScaleFactor());   
	    glBegin(GL_POINTS);  
	    for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      // if( mask(i1)!=0 && coord(i1)>=xBound(Start,0) && coord(i1)<=xBound(End,0) )
	      if( cMask(i1) )
		glVertex3f(coord(i1)  ,u.sa(  i1,i2,i3,component),1.e-3);
	    }
	    glEnd();
	  }


	}
      } // end for grid
      
      glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor()); // reset
      glEndList();

      gi.setAxesDimension(2); // plot axes as if for a 2D grid.

      // Label contour lines with a coloured name
     
      if( parameters.plotTitleLabels )
      {
	aString nameLabel;
	glNewList(labelList,GL_COMPILE);
	for( n=componentsToPlot.getBase(0),m=0; n<=componentsToPlot.getBound(0); n++,m++ )
	{
	  component=componentsToPlot(n);
	
	  real size=.05;
	  const int maxNumberOfFullSizeLabels=int(1./(size*1.2));
	  if( (componentsToPlot.getBound(0)-componentsToPlot.getBase()) > maxNumberOfFullSizeLabels )
	    size*= real(maxNumberOfFullSizeLabels)/(componentsToPlot.getBound(0)-componentsToPlot.getBase());
	  
	  real xpos=.95, ypos=.75-size*m*1.15;
	  int centering=+1;
	  aString labelColour;
	  if( colourLineContours )
	    labelColour=lineColour[m%numberOfLineColours];

	  aString nameLabel;
	  if( plotLineMarkers )
	    nameLabel=lineMarker[m%numberOfLineMarkers]+" : "+uGCF.getName(component);
	  else
	    nameLabel=uGCF.getName(component);

	  // printf(" gi.label=%s (x,y)=(%e,%e) \n",(const char*)nameLabel,xpos,ypos);
	
	  gi.label(nameLabel,xpos,ypos,size,centering,0,parameters,labelColour);   // plot the labels

	  if( false && plotLineMarkers && componentsToPlot.getLength(0)>1 )
	  {
	    // **** this doesn't work *****
	    RealArray x(1,3),r(1,3);
	    for( int grid=0; grid<numberOfGrids; grid++ )
	    {
	      if( !(gridsToPlot(grid)&GraphicsParameters::toggleContours) )
		continue;

	      realMappedGridFunction & u = uGCF[grid];
	      const RealDistributedArray & coord = gc[grid].center();
	      const IntegerDistributedArray & mask = gc[grid].mask();
	      getIndex(gc[grid].gridIndexRange(),I1,I2,I3);

	      I1=Range(gc[grid].dimension(Start,axis1),gc[grid].dimension(End,axis1)-1); 
	
	      i2 = I2.getBase();
	      i3 = I3.getBase();
	      int skip = max(1,I1.length()/10);   // at most 10 markers on a line
	      for( i1=I1.getBase(); i1<I1.getBound(); i1++ )
	      {
		if( (i1 % skip) == 0 && mask(i1)!=0 && mask(i1+1)!=0 
		    && coord(i1)>=xBound(Start,0) && coord(i1)<=xBound(End,0)  )
		{
		  x(0,0)=coord(i1+1);
		  x(0,1)=u.sa(i1+1,i2,i3,component);
		  x(0,2)=0.;
		  gi.worldToNormalizedCoordinates(x,r);
//    		printf("worldToNormalizedCoordinates: x=(%8.2e,%8.2e) r= (%8.2e,%8.2e)\n",
//                           x(0,0),x(0,1),r(0,0),r(0,1));
		  gi.label(lineMarker[m%numberOfLineMarkers],r(0,0),r(0,1),.025,0);
		}
	      }
	    }
	  }

	}
	glEndList();

	if( !colourLineContours && plotLineMarkers )
	{
	  nameLabel="";
	  for( n=componentsToPlot.getBase(0),m=0; n<=componentsToPlot.getBound(0); n++,m++ )
	  {

	    component=componentsToPlot(n);
	    nameLabel+=uGCF.getName(component);
	    if( plotLineMarkers && componentsToPlot.getLength(0)>1 )
	      nameLabel+=" ("+lineMarker[m%numberOfLineMarkers]+")";
	    if( plotLineMarkers && n<componentsToPlot.getBound(0) )
	      nameLabel+=",  ";
	  }
	  gi.setYAxisLabel(nameLabel);
	}
	else
	{
	  gi.setYAxisLabel();
	}
      }
      
      gi.redraw();

    }
  }
// AP Turn this off for now (1/4/02)
//  if( lighting[currentWindow] ) // should this really be done here?
//    glDisable(GL_COLOR_MATERIAL);

  if( !psp.plotObjectAndExit )
  {
    gi.popGUI(); // restore the previous GUI
  }

  gi.setYAxisLabel(yAxisLabelSaved);
  psp.plotColourBar=plotColourBarSaved;

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
  psp.objectWasPlotted=plotObject;  // this indicates that the object appears on the screen (not erased)

  // reset 
  gi.setPlotTheBackgroundGrid(plotBackGroundGridSave);

}




#undef FOR_3


