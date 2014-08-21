#include "GL_GraphicsInterface.h"
#include "Mapping.h"
#include "PlotIt.h"
#include "display.h"

#define ForBoundary(side,axis)   for( axis=0; axis<domainDimension; axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3(i1,i2,i3,I1,I2,I3) \
  for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  \
  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  \
  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

void PlotIt::
plotStructured(GenericGraphicsInterface &gi, Mapping & map, 
	       GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */,
	       int dList /* =0 */, bool lit /* =FALSE */)
//----------------------------------------------------------------------
// /Description:
//   Plot a structured mapping. Plot curves, surfaces and volumes in 1,2 or 3
//     space dimensions. Plot grid lines. In 3D plot shaded surfaces
//     and grid lines.
// 
// /map (input): Mapping to plot.
// /parameters (input/output): supply optional parameters to change
//    plotting characteristics.
// /Errors:  Some...
// /Return Values: none.
//
//  /Author: WDH \& AP
//----------------------------------------------------------------------
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();

  // save the current window number 
  int startWindow = gi.getCurrentWindow();

  GraphicsParameters localParameters(TRUE);  // TRUE means this is gets default values
  GraphicsParameters & par = parameters.isDefault() ? localParameters : parameters;

  int side,axis;
  aString answer;
  char buff[80];
  aString menu[] = {"!Structured mapping plotter",
                   "erase and exit",
                   " ",
//                   "plot",
//                   "lighting (toggle)",
//                   "set lighting characteristics",
                   ">colour options",
		     ">boundaries",
                       "colour boundaries by bc number",
                       "colour boundaries by grid number",
                       "colour boundaries by share value",
		       "colour boundaries in the default way",
                     "<set colour",
                   "<plot shaded surfaces (3D) toggle",
                   "plot grid points on curves (toggle)",
                   "plot grid lines on boundaries (3D) toggle",
                   "plot non-physical boundaries (toggle)",
                   "plot grid lines on coordinate planes",
                   "plot mapping edges",
                   "do not plot mapping edges",
		   "plot next coordinate plane",
		   "plot previous coordinate plane",
                   "plot ghost lines",
//                     "plot labels (toggle)",
//                     "plot number labels (toggle)",
//                     "plot axes (toggle)",
//                     "plot back ground grid (toggle)",
		   "keep aspect ratio",
                   "do not keep aspect ratio",
                   "erase",
                   "exit",
                   "" };

  aString menuTitle = "Mapping";

  GUIState interface;
  if( !par.plotObjectAndExit )
  {
// setup a user defined menu and some user defined buttons
    aString buttons[][2] = {{"plot shaded surfaces (3D) toggle",          "Shade"}, 
			   {"plot grid lines on boundaries (3D) toggle", "Grid"},
			   {"plot non-physical boundaries (toggle)",     "N-Phy"},
			   {"plot mapping edges (toggle)",               "Edges"},
//  			   {"plot labels (toggle)",                      "Labels"},
//  			   {"plot number labels (toggle)",               "Square"},
//  			   {"plot axes (toggle)",                        "Axes"},
//  			   {"plot back ground grid (toggle)",            "BgGrid"},
			   {"plot",                                      "Plot"},
			   {"erase",                                     "Erase"},
			   {"exit",                                      "Exit"},
			   {"",                                          ""}};
    aString pulldownMenu[] = {"plot ghost lines", "" };

    interface.buildPopup(menu);
    interface.setUserButtons(buttons);
    interface.setUserMenu(pulldownMenu, "Mapping");
    
    gi.pushGUI( interface );
  }
  
  int list=0, lightList=0;
  if( gi.isGraphicsWindowOpen() && dList == 0 )
  {
    list = gi.generateNewDisplayList();  // get a new display list to use
    assert(list!=0);

    lightList = gi.generateNewDisplayList(1);  // get a display list with lighting
    assert(lightList!=0);
// debug
//    printf("plotStructured: unlitDL=%i, litDL=%i\n", list, lightList);
  }

  bool   plotObject                  = par.plotObject;
  
  int  & boundaryColourOption        = par.boundaryColourOption;
  // int  & gridLineColourOption        = par.gridLineColourOption; 
  int  & numberOfGhostLinesToPlot    = par.numberOfGhostLinesToPlot;
  bool & plotShadedMappingBoundaries = par.plotShadedMappingBoundaries;
  bool & plotLinesOnMappingBoundaries= par.plotLinesOnMappingBoundaries;
  bool & plotNonPhysicalBoundaries   = par.plotNonPhysicalBoundaries;
  bool & plotGridPointsOnCurves      = par.plotGridPointsOnCurves;
  real & surfaceOffset               = par.surfaceOffset;
  bool & labelGridsAndBoundaries     = par.labelGridsAndBoundaries;
  int & numberOfGridCoordinatePlanes = par.numberOfGridCoordinatePlanes; 
  IntegerArray & gridCoordinatePlane     = par.gridCoordinatePlane;

  if( par.isDefault() )
  { // user has NOT supplied par, so we set them to default
    plotObject=TRUE;
  }
  else
  {
    par.plotBoundsChanged=FALSE;
  }  

  gi.setKeepAspectRatio(par.keepAspectRatio); 

  // *************************************
  // *** Evaluate the mapping on a grid***
  // *************************************

  RealArray r,x;                      // x: save vertices for plotting
  RealArray rgb(3);  
  IntegerArray iv(3),iv3(3);
  int & i1 = iv(0);
  int & i2 = iv(1);
  int & i3 = iv(2);

  int domainDimension=map.getDomainDimension();
  int rangeDimension =map.getRangeDimension();
  int dimension = max(2,rangeDimension);       // for plotting maps from R1->R1
  
  if( Mapping::debug & 16 )
    cout << "plotStructuredMapping: domainDimension = " << domainDimension << ", rangeDimension =" 
      << rangeDimension << endl;
  
  IntegerArray dim(3); dim=0;      // dimensions for vertex
  for( axis=0; axis<domainDimension; axis++ )
    dim(axis)=map.getGridDimensions(axis);

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  I1=0; I2=0; I3=0;
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];   // include ghost points
  Ig1=0; Ig2=0; Ig3=0;
  for( axis=0; axis<domainDimension; axis++ )
  {
    Iv[axis]=Range(0,dim(axis)-1);
    Igv[axis]=Range(-numberOfGhostLinesToPlot,dim(axis)+numberOfGhostLinesToPlot-1);
  }
  
  Index Axes(0,domainDimension);
  Index xAxes(0,rangeDimension);

  MappingParameters mapParams;  // use these to get the mask for Trimmed surfaces
  bool computeGrid=TRUE;
  RealArray xBound(2,3); xBound=0.;
  
  // set default prompt
  gi.appendToTheDefaultPrompt("plotStructuredMapping>");

  // **** Plotting loop *****
  for(int i=0;;i++)
  {
    if( i==0 && (plotObject || par.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( i==1 && par.plotObjectAndExit )
      answer="exit";
    else
      gi.getAnswer(answer, "");

    // make sure that the currentWindow is the same as startWindow! (It might get changed 
    // interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

    if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else if( answer=="lighting (toggle)" )
    {
      gi.outputString("This function is obsolete. Use the view characteristics dialog"
		   " to turn on/off lighting.");
    }
    else if( answer=="keep aspect ratio" )
    {
      par.keepAspectRatio=true;
      gi.setKeepAspectRatio(par.keepAspectRatio); 
    }
    else if( answer=="do not keep aspect ratio" )
    {
      par.keepAspectRatio=false;
      gi.setKeepAspectRatio(par.keepAspectRatio); 
    }
    else if( answer=="set lighting characteristics" )
    {
      gi.outputString("This command is obsolete. Use the set view characteristics dialog ");
      gi.outputString(" from the View menu instead");
      //      setViewCharacteristics();
    }
    else if( answer=="erase" )
    {
      plotObject=FALSE;
      glDeleteLists(list,1);
      glDeleteLists(lightList,1);
      gi.redraw(TRUE); // AP: Why redraw directly?
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
      glDeleteLists(list,1);
      glDeleteLists(lightList,1);
      gi.redraw();
      break;
    }
    else if( answer=="set colour" )
    {
      aString answer2 = gi.chooseAColour();
      if( answer2!="no change" )
        par.mappingColour = answer2;
    }
    else if( answer=="colour boundaries by bc number" )
      boundaryColourOption=GraphicsParameters::colourByBoundaryCondition;
    else if( answer=="colour boundaries by grid number" )
      boundaryColourOption=GraphicsParameters::colourByGrid;
    else if( answer=="colour boundaries by share value" )
      boundaryColourOption=GraphicsParameters::colourByShare;
    else if( answer=="colour boundaries in the default way" )
      boundaryColourOption=GraphicsParameters::defaultColour;

    else if( answer=="plot shaded surfaces (3D) toggle" )
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
    }
    else if( answer=="plot grid lines on boundaries (3D) toggle" )
    {
      plotLinesOnMappingBoundaries= !plotLinesOnMappingBoundaries;  
    }
    else if( answer=="plot mapping edges (toggle)" )
    {
      par.plotMappingEdges = !par.plotMappingEdges;
    }
    else if( answer=="plot mapping edges" )
    {
      par.set(GI_PLOT_MAPPING_EDGES,TRUE);
    }
    else if( answer=="do not plot mapping edges" )
    {
      par.set(GI_PLOT_MAPPING_EDGES,FALSE);
    }
    else if( answer==  "plot non-physical boundaries (toggle)" )
    {
      plotNonPhysicalBoundaries= !plotNonPhysicalBoundaries;
    }
    else if( answer=="plot grid points on curves (toggle)" )
    {
      plotGridPointsOnCurves= !plotGridPointsOnCurves;
    }
    else if( answer=="plot ghost lines" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter the number of ghost lines (or cells) to plot (current=%i)",
				  numberOfGhostLinesToPlot)); 
      if( answer!="" )
      {
	sScanF(answer,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF(buff,"plotMapping::plot %i ghost lines\n",numberOfGhostLinesToPlot));
      }
      for( axis=0; axis<domainDimension; axis++ )
	Igv[axis]=Range(-numberOfGhostLinesToPlot,dim(axis)+numberOfGhostLinesToPlot-1);

      computeGrid=x.dimension(0)!=Ig1;
    }
    else if( answer=="plot grid lines on coordinate planes" )
    {
      if( domainDimension!=3 )
      {
	printf("Sorry: plot grid lines on coordinate planes on makes sense for domainDimension==3\n");
	continue;
      }
      printf(" coordinate   starting      ending    \n");
      printf(" direction    grid index    grid index\n");
      for( int axis=0; axis<3; axis++ )
      {
	printf("   %1i        %5i          %7i  \n",axis,Igv[axis].getBase(),Igv[axis].getBound());
      }
      numberOfGridCoordinatePlanes=0;
      gridCoordinatePlane=0;
      for(;;)
      {
	gi.inputString(answer,sPrintF(buff,"Enter the coordinate direction and grid index (enter -1 finish)"));
	if( answer !="" && answer!=" ")
	{
          int dir,index;
	  sScanF(answer,"%i %i",&dir,&index);
	  if( dir<0 )
            break;
          if( dir<0 || dir>2 )
	  {
	    printf("Error, the coordinate direction=%i must be 0,1, or 2 \n",dir);
	  }
	  else if( index<Igv[dir].getBase() || index>Igv[dir].getBound() )
	  {
	    printf("Error, index=%i should be in the range [%i,%i] \n",index,Igv[dir].getBase(),Igv[dir].getBound());
	  }
	  else
	  {
            if( numberOfGridCoordinatePlanes>=gridCoordinatePlane.getLength(1) )
	    {
	      gridCoordinatePlane.resize(3,gridCoordinatePlane.getLength(1)*2+5);
	    }
	    gridCoordinatePlane(0,numberOfGridCoordinatePlanes)=0;
	    gridCoordinatePlane(1,numberOfGridCoordinatePlanes)=dir; 
	    gridCoordinatePlane(2,numberOfGridCoordinatePlanes)=index;
            numberOfGridCoordinatePlanes++;
	  }
	}
        else
	  break;
      }
    }
    else if( answer=="plot next coordinate plane" || answer=="plot previous coordinate plane" )
    {
      // shift all coordinate planes by +1 or -1
      // periodic wrap at the ends 
      int increment = answer=="plot next coordinate plane" ? 1 : -1;
      for( int plane=0; plane<numberOfGridCoordinatePlanes; plane++ )
      {
	int axis=gridCoordinatePlane(1,plane);
        if( axis>=0 && axis<3 )
	{
          gridCoordinatePlane(2,plane)+=increment;
	  if( gridCoordinatePlane(2,plane) < Igv[axis].getBase() )
            gridCoordinatePlane(2,plane)=Igv[axis].getBound();
	  else if( gridCoordinatePlane(2,plane)>Igv[axis].getBound() )
            gridCoordinatePlane(2,plane)=Igv[axis].getBase();
	}
      }
    }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      char buff[100];
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }
      
//     #ifdef USE_PPP
//     if( computeGrid && gi.isGraphicsWindowOpen() && map.usesDistributedMap() )
//     #else
//     if( false )
//     #endif
//     {
//       printF("plotMapping::ERROR: I cannot plot this mapping since the map function requires communication!\n");
//       x.redim(Ig1,Ig2,Ig3,rangeDimension);
//       x=0.;
//       computeGrid=false;
//     }
//    else 

    // *wdh* 2013/08/19 if( computeGrid && gi.isGraphicsWindowOpen() ) 
    if( computeGrid && gi.isInteractiveGraphicsOn() ) 
    {
      // ==================================
      // ==== compute the grid points =====
      // ==================================

      computeGrid=false;
      RealArray dr(3);
      int axis;
      for( axis=axis1; axis<=axis3; axis++ )
	dr(axis)=1./max(dim(axis)-1,1);

      if( domainDimension==1 && rangeDimension==1 )
      {
	// for maps from R1->R1 we plot x versus r

	// Fill in the x array     
	real delta1=1./max(1.,dim(0)-1.);

	r.redim(Ig1,Ig2,Ig3,domainDimension);
	int xDimension=max(2,rangeDimension);
	x.redim(Ig1,Ig2,Ig3,xDimension);

	r(Ig1,0).seqAdd(dr(axis1)*Ig1.getBase(),dr(axis1));
        #ifdef USE_PPP
	  map.mapGridS(r,x);
        #else
	  map.mapGrid(r,x);
        #endif
	
	if( rangeDimension==1 )
	{ // for maps from R1->R1 we plot x versus r:
	  x(Ig1,Ig2,Ig3,1)=x(Ig1,Ig2,Ig3,0);
	  x(Ig1,Ig2,Ig3,0)=r(Ig1,Ig2,Ig3,0);
	}
      }
      else if( domainDimension>=1 && domainDimension<=3  && rangeDimension>=1 && rangeDimension<=3 )
      {
        bool useGridFromMapping=numberOfGhostLinesToPlot==0;
	#ifdef USE_PPP
	  // the grid from the mapping is distributed --  do this for now
   	  useGridFromMapping=false;
        #endif
        if( useGridFromMapping )
	{
	  x.reference(map.getGridSerial(mapParams));
	}
        else 
	{
          // if there are ghost points we need to compute the extra values.
	  // printf("plotMapping: compute ghost values\n");

	  // ********************************** fix this -- only compute ghost values *************

	  RealArray r;
	  if( plotOnThisProcessor )
	  { 
            // -- here is a quick fix: evaluate the whole grid on one processor ----

	    x.resize(Ig1,Ig2,Ig3,rangeDimension);
	    r.redim(Ig1,Ig2,Ig3,domainDimension);

	    for( int i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
	    {
	      for( int i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
		r(Ig1,i2,i3,0).seqAdd(dr(axis1)*Ig1.getBase(),dr(axis1));
	      if( domainDimension>1 )
	      {
		for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		  r(i1,Ig2,i3,1).seqAdd(dr(axis2)*Ig2.getBase(),dr(axis2));
	      }
	    }
	    if( domainDimension>2 )
	    {
	      for( int i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
		for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		  r(i1,i2,Ig3,2).seqAdd(dr(axis3)*Ig3.getBase(),dr(axis3));
	    }
	  } // end if plotOnThisProcessor
	  
          #ifdef USE_PPP
  	    map.mapGridS(r,x,Overture::nullRealArray(),mapParams);
          #else
  	    map.mapGrid(r,x,Overture::nullRealDistributedArray(),mapParams);
          #endif

	}
        // some mappings are masked: (TrimmedMapping)
	if( mapParams.mask.getLength(0)==Ig1.getLength()*Ig2.getLength()*Ig3.getLength() )
	  mapParams.mask.reshape(Ig1,Ig2,Ig3);
      }
      else
      {
	printF("GL_GraphicsInterface::plotStructuredMapping:ERROR: domainDimension=%i\n",domainDimension);
	OV_ABORT("GL_GraphicsInterface::plotStructuredMapping:ERROR: domainDimension");
      }

      if( Mapping::debug & 16 )
	::display(x,"plotStructuredMapping: Here is x","%5.2f ");

      // get Bounds on the Mapping that we plot

      if( par.usePlotBounds )
	xBound=par.plotBound;
      else
      {
	for( axis=0; axis<dimension; axis++ )
	{
          if( axis<rangeDimension )
	  {
	    Bound bMin = map.getRangeBound(Start,axis);
	    Bound bMax = map.getRangeBound(End  ,axis);
	    if( bMin.isFinite() && bMax.isFinite() )
	    {
	      xBound(Start,axis)=(real)bMin;
	      xBound(End  ,axis)=(real)bMax;
	    }
	    else
	    {
	      xBound(Start,axis)= min(x(I1,I2,I3,axis));
	      xBound(End,axis)  = max(x(I1,I2,I3,axis));
	    }
	  }
	  else
	  {
	    xBound(Start,axis)= min(x(I1,I2,I3,axis));
	    xBound(End,axis)  = max(x(I1,I2,I3,axis));
	  }
	  
	}

	if( par.usePlotBoundsOrLarger )
	{ // Use existing plot bounds unless the new bounds are larger by a certain factor
	  real relativeBoundsChange;
	  if (fabs(max(par.plotBound(End,xAxes)-par.plotBound(Start,xAxes))) < 10*REAL_EPSILON)
	    relativeBoundsChange = 10*par.relativeChangeForPlotBounds; // make sure xBound gets updated below...
	  else
	    relativeBoundsChange = max(fabs(par.plotBound-xBound))
	      /max(par.plotBound(End,xAxes)-par.plotBound(Start,xAxes));

	  if( relativeBoundsChange>par.relativeChangeForPlotBounds ) // *** fix .2 to be a variable ****
	  {
	    for( int axis=0; axis<3; axis++ )
	    {
	      xBound(Start,axis)=min(xBound(Start,axis),par.plotBound(Start,axis));
	      xBound(End  ,axis)=max(xBound(End  ,axis),par.plotBound(End  ,axis));
	    }
	  }
	}
	if( !par.isDefault() )
	{
	  par.plotBound=xBound;
	  par.plotBoundsChanged=TRUE;
	}
      }
      
    }  // end computeGrid
    

    if( plotObject && gi.isGraphicsWindowOpen() && plotOnThisProcessor )
    {
      gi.setAxesDimension(max(2,map.getRangeDimension()));

      // plot labels on top and bottom
      if( par.plotTitleLabels )
      {
        gi.plotLabels( par );
      }
      
//      glDeleteLists(list,1); // clear list (AP: not necessary; it will be overwritten.)
      gi.setGlobalBound(xBound);

      real eps=.002*max(xBound(End,Range(0,2))-xBound(Start,Range(0,2)));

      if( plotOnThisProcessor )
	glDisable(GL_POLYGON_OFFSET_FILL);
  
      if( rangeDimension<=2 || domainDimension==1 )
      { 
	// *********************************************
	// ***** plot curves or 2D mappings UNLIT!!! ***
	// *********************************************
	
	if (dList == 0)
	  glNewList(list,GL_COMPILE);

// plot this if we have a local display list, or if we only plot unlit stuff in dList
	if (dList == 0 || !lit) 
	{
	  glPushName(map.getGlobalID()); // assign a name for picking

	  gi.setColour(par.mappingColour); 

	  if( plotShadedMappingBoundaries )
	  {
	    //..................Draw Lines...........................
	    glLineWidth(par.size(GraphicsParameters::curveLineWidth)*par.size(GraphicsParameters::lineWidth)*
			gi.getLineWidthScaleFactor() );
	    glBegin(GL_LINES);
	
	    for( axis=axis1; axis<domainDimension; axis++ )  // draw grid lines parallel to axis
	    {
	      int is1= axis==axis1 ? 1 : 0;
	      int is2= axis==axis2 ? 1 : 0;
	      int is3= axis==axis3 ? 1 : 0;

	      Index I1a=Range(Ig1.getBase(),Ig1.getBound()-is1); 
	      Index I2a=Range(Ig2.getBase(),Ig2.getBound()-is2); 
	      Index I3a=Range(Ig3.getBase(),Ig3.getBound()-is3); 

	      if( rangeDimension<3 )
	      {
		FOR_3(i1,i2,i3,I1a,I2a,I3a)
		{
		  glVertex2f( x(i1    ,i2    ,i3,0),x(i1    ,i2    ,i3,1) );
		  glVertex2f( x(i1+is1,i2+is2,i3,0),x(i1+is1,i2+is2,i3,1) );
		}
	      }
	      else
	      {
		FOR_3(i1,i2,i3,I1a,I2a,I3a)
		{
		  glVertex3(x(i1    ,i2    ,i3    ,0),x(i1    ,i2    ,i3    ,1),x(i1    ,i2    ,i3    ,2));
		  glVertex3(x(i1+is1,i2+is2,i3+is3,0),x(i1+is1,i2+is2,i3+is3,1),x(i1+is1,i2+is2,i3+is3,2));
		}
	      }
	    }
	    glEnd();     // GL_LINES
	  }

	  if( domainDimension==1 && plotGridPointsOnCurves )
	  {
	    // *** plot points on curves ****
            gi.setColour(par.pointColour);

	    // glPointSize((1.+par.size(curveLineWidth))*par.size(lineWidth)*
	    // 	lineWidthScaleFactor[currentWindow]);   

            glPointSize(par.pointSize*gi.getLineWidthScaleFactor() );  

	    glBegin(GL_POINTS);  
	    i2=Ig2.getBase();
	    i3=Ig3.getBase();
	    if( rangeDimension==2 || rangeDimension==1 )
	    {
	      for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		glVertex3(x(i1,i2,i3,0),x(i1,i2,i3,1),0.);
	    }
	    else if( rangeDimension==3 )
	    {
	      for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
		glVertex3(x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2));
	    }
	    
	    glEnd();
	  }

	  if( domainDimension==1 && par.plotEndPointsOnCurves )
	  {
	    // *** plot end points on curves ****
            gi.setColour(gi.getColour(GenericGraphicsInterface::textColour));

            glPointSize(int(1.5*par.pointSize*gi.getLineWidthScaleFactor()+.5));  

	    glBegin(GL_POINTS);  
	    i2=Ig2.getBase();
	    i3=Ig3.getBase();
	    if( rangeDimension==2 )
	    {
	      for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1+=(Ig1.getBound()-Ig1.getBase()) )
		glVertex3(x(i1,i2,i3,0),x(i1,i2,i3,1),0.);
	    }
	    else if( rangeDimension==3 )
	    {
	      for( int i1=Ig1.getBase(); i1<=Ig1.getBound(); i1+=(Ig1.getBound()-Ig1.getBase()) )
		glVertex3(x(i1,i2,i3,0),x(i1,i2,i3,1),x(i1,i2,i3,2));
	    }
	    glEnd();
	  }

	  if( domainDimension==2 && rangeDimension==2  && par.plotMappingEdges )
	  {
	    // ***** plot boundaries for 2D grids ****
	    const int colourOption = 1;
	    real zRaise= rangeDimension==2 ? 1.e-3 : 0.; 
            // plot boundaries, coloured by the BC or share
	    plotMappingBoundaries(gi, map, x, colourOption, zRaise, par);  
	  }

	  glPopName();
	} // end if (dList == 0 || lit == 0)
	
	if (dList == 0)
	  glEndList();
	
      } // end if rangDimension <=2 || domainDimension == 1
      
      else
      { // ----- plot 3D surfaces or volumes (but not curves)------
// do the unlit list first
	if (dList == 0)
	  glNewList(list,GL_COMPILE);

	if (dList == 0 || !lit)
	{
	  glPushName(map.getGlobalID()); // assign a name for picking

	  if( domainDimension==2 )
	  {  // plot lines on boundaries
	    axis=axis3;
	    side=1;
	    if( plotLinesOnMappingBoundaries )
	    {
	      bool offsetLines=plotShadedMappingBoundaries;
	      plotLinesOnSurface(gi, x, Ig1, Ig2, I3, axis, offsetLines, eps, par, mapParams.mask);
	    }
	  } // end if domainDimension == 2
	  else
	  { // domainDimension == 3
	    // 3D: plot shaded surfaces or lines on the boundaries and optionally on selected planes.
	    // for a 3d volume, only plot the 6 faces. If bc>0 plot a shaded surface, else plot lines
	    // **** The first 6 times through this loop we try and plot lines on grid boundaries,
	    // **** after that we plot lines on specified coordinate planes.
	    int index;
	    for( int plane=-6; plane<numberOfGridCoordinatePlanes; plane++ )
	    {
	      if( plane<0 )
	      {
		side=(plane+6) % 2;
		axis=(plane+6)/2;
		index=0;
	      }
	      else 
	      {
		axis=gridCoordinatePlane(1,plane);
		index=gridCoordinatePlane(2,plane);
		side=Start;
	      }
	      if( axis<0 || axis>2 || index<Igv[axis].getBase() || index > Igv[axis].getBound() )
	      {
		printf("ERROR: there are invalid values specifying a coordinate plane, axis=%i, index=%i \n",
		       axis,index);
	      }
	      else
	      {
		Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
		I1a = axis==0 ? (side==0 ? Index(I1.getBase(),1) : Index(I1.getBound(),1) ) : Ig1;
		I2a = axis==1 ? (side==0 ? Index(I2.getBase(),1) : Index(I2.getBound(),1) ) : Ig2;
		I3a = axis==2 ? (side==0 ? Index(I3.getBase(),1) : Index(I3.getBound(),1) ) : Ig3;
		if( plane<0 )
		{
		  if( (map.getBoundaryCondition(side,axis) > 0 && plotLinesOnMappingBoundaries ) ||
		      (map.getBoundaryCondition(side,axis) <=0 && plotNonPhysicalBoundaries ) )
		  {
		    bool offsetLines=plotShadedMappingBoundaries  && map.getBoundaryCondition(side,axis) > 0;
		    plotLinesOnSurface(gi, x, I1a, I2a, I3a, axis, offsetLines, eps, par);
		  }
		}
		else
		{
		  Iva[axis]+=index;       // offset from side=0 to the correct line
		  bool offsetLines=FALSE;
		  plotLinesOnSurface(gi, x, I1a, I2a, I3a, axis, offsetLines, eps, par);
		}
	      } // end if (error)...
	    } // end for plane =...
	  } // end if domainDimension == 3

// plot sub-surface boundaries here...
	  if( par.plotMappingEdges )
	  {
	    IntegerArray gridIndexRange(2,3);
	    gridIndexRange(0,0)=I1.getBase();
	    gridIndexRange(0,1)=I2.getBase();
	    gridIndexRange(0,2)=I3.getBase();
	    gridIndexRange(1,0)=I1.getBound();
	    gridIndexRange(1,1)=I2.getBound();
	    gridIndexRange(1,2)=I3.getBound();
	    
	    // printf("plot mapping edges\n");
	    plotMappingEdges(gi, x, gridIndexRange, par, mapParams.mask );
	  }

	  glPopName();
	}
	
	if (dList == 0)
	  glEndList();
	
// do the lit stuff
	if (dList == 0)
	  glNewList(lightList,GL_COMPILE);

	if (dList == 0 || lit)
	{
	  glPushName(map.getGlobalID()); // assign a name for picking

	  glEnable(GL_POLYGON_OFFSET_FILL);
	  glPolygonOffset(1.,surfaceOffset*OFFSET_FACTOR);  
//	  printf("9:POLYGON_OFFSET_FACTOR=%f\n", surfaceOffset);

	  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	  glShadeModel(GL_SMOOTH);     // interpolate colours between vertices

//  	  if( !lighting[currentWindow] )
//  	    getXColour(par.mappingColour,rgb );  // get rgb values for this colour

	  if( domainDimension==2 )
	  {  // plot a shaded surface
	    axis=axis3;
	    side=1;
          
	    if( plotShadedMappingBoundaries )
	    {
	      gi.setColour(par.mappingColour); 
	      plotShadedFace(gi, x, Ig1, Ig2, I3, axis, side, domainDimension, rangeDimension, rgb, mapParams.mask);
	    }
	  }
	  else
	  { // domainDimension == 3
	    // 3D: plot shaded surfaces or lines on the boundaries and optionally on selected planes.
	    // for a 3d volume, only plot the 6 faces. If bc>0 plot a shaded surface, else plot lines
	    // **** The first 6 times through this loop we try and plot lines on grid boundaries,
	    // **** after that we plot lines on specified coordinate planes.
	    int index;
	    for( int plane=-6; plane<numberOfGridCoordinatePlanes; plane++ )
	    {
	      if( plane<0 )
	      {
		side=(plane+6) % 2;
		axis=(plane+6)/2;
		index=0;
	      }
	      else 
	      {
		axis=gridCoordinatePlane(1,plane);
		index=gridCoordinatePlane(2,plane);
		side=Start;
	      }
	      if( axis<0 || axis>2 || index<Igv[axis].getBase() || index > Igv[axis].getBound() )
	      {
		printf("ERROR: there are invalid values specifying a coordinate plane, axis=%i, index=%i \n",
		       axis,index);
	      }
	      else
	      {
		Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
		I1a = axis==0 ? (side==0 ? Index(I1.getBase(),1) : Index(I1.getBound(),1) ) : Ig1;
		I2a = axis==1 ? (side==0 ? Index(I2.getBase(),1) : Index(I2.getBound(),1) ) : Ig2;
		I3a = axis==2 ? (side==0 ? Index(I3.getBase(),1) : Index(I3.getBound(),1) ) : Ig3;
		if( plane<0 )
		{
		  if( plotShadedMappingBoundaries && map.getBoundaryCondition(side,axis) > 0 )
		  {
		    if( par.boundaryColourOption==GraphicsParameters::colourByShare )  
		      gi.setColour( gi.getColourName(map.getShare(side,axis)) );
		    else if( par.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition )
		    {
		      gi.setColour( gi.getColourName(map.getBoundaryCondition(side,axis)) );
		    }
		    else
		      gi.setColour(par.mappingColour); 
		    plotShadedFace(gi, x, I1a, I2a, I3a, axis, side, domainDimension, rangeDimension, rgb );
		  }
		}
	      }
	    }
	  } // end if domainDimension == 3
	
	  glDisable(GL_POLYGON_OFFSET_FILL);
      
	  glPopName();
	}
	
	if (dList == 0)
	  glEndList();
	
      } // end 3-D
      
      if( par.plotTheAxes )
      {
  	gi.setColour(GenericGraphicsInterface::textColour);
        gi.setAxesLabels(map.getName(Mapping::rangeAxis1Name),
			 map.getName(Mapping::rangeAxis2Name),
			 map.getName(Mapping::rangeAxis3Name));
      }

      // *note* the coloured squares go in their own list
      if( labelGridsAndBoundaries  &&  // *wdh* 100220 -- turn back on -- otherwise a square is plotted in topology
          ( (domainDimension==2 && rangeDimension==2) ||
	     par.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition || 
	     par.boundaryColourOption==GraphicsParameters::defaultColour ||
	    par.boundaryColourOption==GraphicsParameters::colourByShare ) )
      {  // label colours for boundary conditions
	IntegerArray numberList(12); numberList=-1;
	int side,axis,number=0;
	ForBoundary(side,axis)
	{
	  if( par.boundaryColourOption==GraphicsParameters::colourByShare )
	    numberList(number++)=map.getShare(side,axis);	  
	  else if( par.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition ||
		   par.boundaryColourOption==GraphicsParameters::defaultColour )
	    numberList(number++)=map.getBoundaryCondition(side,axis);	  
	}
	gi.drawColouredSquares(numberList);
      }
      
      gi.redraw();
    } // end if plotObject...
    
  }
  
// only reset the user menu and buttons if we changed them in the first place!
  if( !par.plotObjectAndExit )
  {
    gi.popGUI();
  }
  
  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt
}

