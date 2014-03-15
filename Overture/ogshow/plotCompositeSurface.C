#include "GL_GraphicsInterface.h" // AP: Need the GL include files for glIsList, glNewList, glEndList, glVertex3
#include "TrimmedMapping.h"
#include "CompositeSurface.h"
#include "PlotIt.h"

//..DEBUG flag for printing which display lists are being built **pf
//#define PF_DEBUG

void PlotIt:: 
plotCompositeSurface(GenericGraphicsInterface &gi, CompositeSurface & cs,
		     GraphicsParameters & parameters /* =Overture::defaultGraphicsParameters() */)
{
  if( !gi.isGraphicsWindowOpen() )
    return;

// save the current window number 
  int startWindow = gi.getCurrentWindow();

  GUIState interface;

  GraphicsParameters localParameters(TRUE);  // TRUE means this object gets default values
  GraphicsParameters & par = parameters.isDefault() ? localParameters : parameters;

  bool   plotObject               = par.plotObject;
  
  int & numberOfGhostLinesToPlot  = par.numberOfGhostLinesToPlot;
  bool & plotShadedMappingBoundaries   = par.plotShadedMappingBoundaries;
  bool & plotLinesOnMappingBoundaries= par.plotLinesOnMappingBoundaries;
  //  bool & plotGridPointsOnCurves   = par.plotGridPointsOnCurves;
  // real & surfaceOffset            = par.surfaceOffset;
  // bool & labelGridsAndBoundaries  = par.labelGridsAndBoundaries;

  //  int gridLineColourOption = (plotShadedMappingBoundaries ? GraphicsParameters::colourBlack :
  //			      GraphicsParameters::colourByGrid);
  int gridLineColourOption = par.gridLineColourOption;

  par.set(GI_PLOT_UNS_EDGES,true); // ****
  const int boundaryColourOptionOld=par.boundaryColourOption;
  par.boundaryColourOption=GraphicsParameters::colourByGrid; // *wdh* 020628
  
  aString oldColour = par.mappingColour;

  aString answer;
  if( !parameters.plotObjectAndExit )
  {
    interface.setWindowTitle("Composite Surface Plotter");
    interface.setExitCommand("exit", "Exit");

    aString menu[] = {"!Composite Surface plotter",
		      "erase and exit",
//		      " ",
//                  "plot",
//                  "lighting (toggle)",
//                  "set lighting characteristics",
//                  "set colour",
//                  "colour boundaries by boundary condition number (toggle)",
//                  "colour boundaries by share value (toggle)",
//                  "plot shaded surfaces (3D) toggle",
//                   "plot grid points on curves (toggle)",
//                   "plot grid lines on boundaries (3D) toggle",
//		   ">colour options",
//  		      "set surface colour",
//  		      "colour surface by grid number",
//  		      "colour grid lines black",
//  		      "colour grid lines by grid number",
//  		      " ",
//  		      "plot sub-surface boundaries",
//  		      "do not plot sub-surface boundaries",
//                   "plot ghost lines",
//                   "plotAxes (toggle)",
//                   "plot number labels (toggle)"
//                   "plot the back ground grid (toggle)",
//                   "erase",
//                   "exit",
		      "" };

    interface.buildPopup(menu);

// define layout of option menus
    interface.setOptionMenuColumns(1);
// first option menu
    aString opCommand1[] = {"colour grid lines by grid number", "colour grid lines black", ""};
    aString opLabel1[] = {"by surface colour", "black", "" };

// initial choice: BC number
    int initialChoice;
    if (gridLineColourOption == GraphicsParameters::colourByGrid)
      initialChoice = 0;
    else
      initialChoice = 1;
    
    interface.addOptionMenu( "Colour grid lines", opCommand1, opLabel1, initialChoice); 

// second option menu
    aString opCommand2[] = {"colour surface by grid number", "set surface colour", ""};
    aString opLabel2[] = {"by grid number", "choose colour...", "" };

// initial choice: BC number
    interface.addOptionMenu( "Colour surface", opCommand2, opLabel2, 0); 

// toggle buttons
    aString tbLabels[] = {"Shade", "Grid", "Bndry", "Labels", "Square", "BgGrid", "Axes", ""};
    aString tbCommands[] = {"plot shaded surfaces (3D) toggle",
			    "plot grid lines on boundaries (3D) toggle",
			    "plot sub-surface boundaries (toggle)",
			    ""};
    int tbState[] = {par.plotShadedMappingBoundaries, 
		     par.plotLinesOnMappingBoundaries, 
		     par.plotMappingEdges};
    
    interface.setToggleButtons(tbCommands, tbLabels, tbState, 3); // organize in 2 columns
// done defining toggle buttons

// setup a user defined menu and some user defined buttons
    aString buttonCommands[] = {"plot", "erase", ""};
    aString buttonLabels[] = {"Plot", "Erase", ""};
  
    interface.setPushButtons(buttonCommands, buttonLabels, 1);

    gi.pushGUI(interface);
  }


  gi.setKeepAspectRatio(true); 

  // By default do not plot ghost lines unless specified:  This can take too long and is usually unnecessary 
  // on Trimmed surfaces.
  const int oldNumberOfGhostLinesToPlot= numberOfGhostLinesToPlot;
  if( !cs.plotGhostLines )
    numberOfGhostLinesToPlot=0;

  int i;
  bool plotObjectAndExit = par.plotObjectAndExit;

  // first determine bounds on the mapping
  Bound b;
  RealArray xBound(2,3);
  for( int axis=0; axis<cs.getRangeDimension(); axis++ )
  {
// initialize to null bounds
    xBound(Start,axis)= REAL_MAX*.1;
    xBound(End  ,axis)=-REAL_MAX*.1;

    b = cs.getRangeBound(Start,axis);
    if( b.isFinite() )
      xBound(Start,axis)=(real)b;
    b = cs.getRangeBound(End,axis);
    if( b.isFinite() )
      xBound(End,axis)=(real)b;
  }
//  xBound.display(" plotCompositeSurface: **** xBound **** ");
  gi.setGlobalBound(xBound);

  // set default prompt
  gi.appendToTheDefaultPrompt("plotCompositeSurface>");

  // **** Plotting loop *****
  for(int it=0;;it++)
  {
    if( it==0 && (plotObject || par.plotObjectAndExit) )
      answer="plot";               // plot first time through if plotObject==TRUE
    else if( it==1 && par.plotObjectAndExit )
      answer="exit";
    else
      gi.getAnswer(answer, "");

// make sure that the currentWindow is the same as startWindow! (It might get changed 
// interactively by the user)
    if (gi.getCurrentWindow() != startWindow)
      gi.setCurrentWindow(startWindow);

//                     0123456789012345678901234567890123456789
//      else if( answer=="lighting (toggle)" )
//      {
//        outputString("This function is obsolete. Use the view characteristics dialog"
//  		   " to turn on/off lighting.");
//      }
//      else if( answer=="set lighting characteristics" )
//      {
//        outputString("This command is obsolete. Use the set view characteristics dialog ");
//        outputString(" from the View menu instead");
//      }
    else if( answer=="erase" )
    {
      plotObject=FALSE;
//        gi.deleteList(list);
//        gi.deleteList(lightList);
      gi.redraw(TRUE);
    }
    else if( answer=="erase and exit" )
    {
      plotObject=FALSE;
//        gi.deleteList(list);
//        gi.deleteList(lightList);
      gi.redraw();
      break;
    }
    else if( answer=="colour surface by grid number" )
    {
      for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	cs.setColour(s, gi.getColourName(s));
    }
    else if( answer=="set surface colour" )
    {
      aString answer2 = gi.chooseAColour();
      if( answer2!="no change" )
      {
	for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  cs.setColour(s,answer2);
      }
    }
    else if( answer=="colour grid lines by grid number" )
    {
      gridLineColourOption=GraphicsParameters::colourByGrid;
// color grid lines black or by grid number?
      par.set(GI_GRID_LINE_COLOUR_OPTION, gridLineColourOption);
    }
    else if( answer=="colour grid lines black" )
    {
//      gridLineColourOption=GraphicsParameters::defaultColour;
      gridLineColourOption=GraphicsParameters::colourBlack;
// color grid lines black or by grid number?
      par.set(GI_GRID_LINE_COLOUR_OPTION, gridLineColourOption);
    }//                     0123456789012345678901234567890123456789
    else if( answer(0,31)=="plot shaded surfaces (3D) toggle" )
    {
      plotShadedMappingBoundaries = !plotShadedMappingBoundaries;   
//        gridLineColourOption =
//  	plotShadedMappingBoundaries ? GraphicsParameters::defaultColour :
//  	GraphicsParameters::colourByGrid;
//        par.set(GI_GRID_LINE_COLOUR_OPTION, gridLineColourOption);
    }//                     01234567890123456789012345678901234567890123456789
    else if( answer(0,40)=="plot grid lines on boundaries (3D) toggle" )
    {
      plotLinesOnMappingBoundaries= !plotLinesOnMappingBoundaries;  
      par.set(GI_PLOT_UNS_EDGES,plotLinesOnMappingBoundaries);
      
      printf("plotLinesOnMappingBoundaries=%i\n",plotLinesOnMappingBoundaries);
    }
// AP: This does not seem to be implemented!
//      else if( answer=="plot grid points on curves (toggle)" )
//      {
//        plotGridPointsOnCurves= !plotGridPointsOnCurves;
//      }
//                          0123456789012345678901234567890123456789
    else if( answer(0,35)=="plot sub-surface boundaries (toggle)" )
    {
      par.plotMappingEdges = !par.plotMappingEdges;
      par.plotUnsBoundaryEdges= !par.plotUnsBoundaryEdges;
    }
    else if( answer=="plot sub-surface boundaries" )
    {
      par.set(GI_PLOT_MAPPING_EDGES,true);
    }
    else if( answer=="do plot sub-surface boundaries" )
    {
      par.set(GI_PLOT_MAPPING_EDGES,false);
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( answer=="plot" )
    {
      plotObject=TRUE;
    }
    else
    {
      char buff[100];
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }


    if( plotObject)
    {
      gi.setAxesDimension(cs.getRangeDimension());
      if ( cs.numberOfSubSurfaces() <= 0)
      {
	gi.redraw();
	continue;
      }

      par.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); // this is for the mapping plotter that will be called below

      int plotShadedSurface, plotTitleLabels, localSquares;
      
      par.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurface);

      par.get(GI_PLOT_LABELS, plotTitleLabels);
      par.get(GI_LABEL_GRIDS_AND_BOUNDARIES, localSquares);
      
      RealArray pb(2,3);
      for( int side=0; side<=1; side++ )
	for( int axis=0; axis<cs.getRangeDimension(); axis++ )
	  pb(side,axis) = cs.getRangeBound(side,axis);
	
      par.set(GI_PLOT_BOUNDS, pb);
      par.set(GI_USE_PLOT_BOUNDS, true);
      
// don't plot labels or squares in the component plotter
      par.set(GI_PLOT_LABELS, FALSE);
      par.set(GI_LABEL_GRIDS_AND_BOUNDARIES, FALSE);
      
// save the current settings
      int localPlotMappingEdges = par.plotMappingEdges;
      int localPlotLines = par.plotLinesOnMappingBoundaries;
      int localPlotShaded = par.plotShadedMappingBoundaries;
      int localPlotNonPhysicalBoundaries = par.plotNonPhysicalBoundaries;
      
//        printf("mappingedges=%i, gridlines=%i, shaded=%i\n",
//  	     localPlotMappingEdges,localPlotLines,localPlotShaded );
      
#ifdef PF_DEBUG 
      printf ("cs> PLOTTING CompositeSurface.\n"); //debug **pf
// first plot the UNLIT stuff
      printf("cs> UNLIT.\n");                      //debug **pf
#endif
      int s;
// sub-surface edges
      par.plotLinesOnMappingBoundaries = false;
      par.plotShadedMappingBoundaries = false;
      par.plotUnsEdges=false;
      par.plotNonPhysicalBoundaries=false; // for volume grids
      par.plotUnsBoundaryEdges=par.plotMappingEdges;
      
      if (par.plotMappingEdges)
      {
#ifdef PF_DEBUG
	printf("cs> UNLIT. MappingEdges.\n");
#endif
        real time=getCPU();
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
// always plot the sub-surface boundaries
	  if (cs.dList(CompositeSurface::boundary, s) > 0 &&
	      glIsList(cs.dList(CompositeSurface::boundary, s)) )
	    gi.setPlotDL(cs.dList(CompositeSurface::boundary, s), true);
	  else 
	  {// get a new list which is unlit, plotted, hideable and interactive
	    cs.dList(CompositeSurface::boundary, s) = gi.generateNewDisplayList(false, true, true, true);  
	    glNewList(cs.dList(CompositeSurface::boundary, s), GL_COMPILE);

#ifdef PF_DEBUG
	    int listnumber =cs.dList(CompositeSurface::boundary, s); //debug **pf
	    printf("cs> UNLIT. MappingEdges, list %5i = %s \n",     //debug **pf
		   listnumber,(const char*) cs[s].getClassName());  //debug **pf
#endif
            plot(gi, cs[s], par, cs.dList(CompositeSurface::boundary, s), false);
	    glEndList();
            if( (getCPU()-time) > 4. ) 
	    { // force a redraw if the mappings take a long time to plot (such as the first time through)
	      time=getCPU();
              gi.redraw(true);
	    }
	  }
	}
      } // end plotMappingEdges
      else
// toggle off all display lists with sub-surface boundaries
      {
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
	  if (glIsList(cs.dList(CompositeSurface::boundary, s)))
	    gi.setPlotDL(cs.dList(CompositeSurface::boundary, s), false);
	}
      }
      
// grid lines
      par.plotMappingEdges = false;
      par.plotLinesOnMappingBoundaries = localPlotLines;
      par.plotShadedMappingBoundaries = false;
      par.plotUnsEdges=localPlotLines;
      par.plotUnsBoundaryEdges=false;
      par.plotNonPhysicalBoundaries=localPlotNonPhysicalBoundaries; // for volume grids
      
      if (par.plotLinesOnMappingBoundaries || par.plotNonPhysicalBoundaries)
      {
//	printf("Plotting gridlines\n");
#ifdef PF_DEBUG
	printf("cs>        GridLines\n");
#endif
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
	  if (cs.isVisible(s))
	  {
	    if (cs.dList(CompositeSurface::gridLines, s) > 0 &&
		glIsList(cs.dList(CompositeSurface::gridLines, s)) )
	    {
	      gi.setPlotDL(cs.dList(CompositeSurface::gridLines, s), true);
//	      printf("Reusing DL=%i for s=%i\n", cs.dList(CompositeSurface::gridLines, s), s);
	    }
	    else
	    { // get a new list which is unlit, plotted, hideable and interactive
	      cs.dList(CompositeSurface::gridLines, s) = gi.generateNewDisplayList(false, true, true, true);  
	      glNewList(cs.dList(CompositeSurface::gridLines, s), GL_COMPILE);
//	      printf("Making new DL=%i for s=%i\n", cs.dList(CompositeSurface::gridLines, s), s);
#ifdef PF_DEBUG
	      int listnumber =cs.dList(CompositeSurface::gridLines, s); //debug **pf
	      printf("cs>        GridLines, list %5i = %s \n",          //debug **pf
		     listnumber,(const char*)  cs[s].getClassName());   //debug **pf
#endif
	      par.set(GI_MAPPING_COLOUR, cs.getColour(s));
// call the appropriate mapping plotter to do the actual work
              plot( gi, cs[s], par, cs.dList(CompositeSurface::gridLines, s), false );
	      glEndList();
	    }
	  }
	  else // this might be better to do in the update function for CS
	  {
	    if (glIsList(cs.dList(CompositeSurface::gridLines, s)))
	      gi.setPlotDL(cs.dList(CompositeSurface::gridLines, s), false);
	  }
	}
      } // end plotLinesOnMappingBoundaries
      else
// toggle off all display lists with grid lines
      {
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
	  if (glIsList(cs.dList(CompositeSurface::gridLines, s)))
	    gi.setPlotDL(cs.dList(CompositeSurface::gridLines, s), false);
	}
      }
      
// then plot the LIT stuff
#ifdef PF_DEBUG
      printf("cs> LIT  \n");                                    //debug **pf
#endif 
    
// plot shaded surfaces
      par.plotMappingEdges = false;
      par.plotLinesOnMappingBoundaries = false;
      par.plotShadedMappingBoundaries = localPlotShaded;
      par.plotUnsEdges=false;
      par.plotNonPhysicalBoundaries=false; // for volume grids
      
// debug
//      printf("plotting CompositeSurface polygons...\n");

      if (par.plotShadedMappingBoundaries)
      {
//	printf("Plotting shaded surfaces\n");
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
	  if (cs.isVisible(s))
	  {
	    if (cs.dList(CompositeSurface::shadedSurface, s) > 0 &&
	        glIsList(cs.dList(CompositeSurface::shadedSurface, s)) )
	    {
	      gi.setPlotDL(cs.dList(CompositeSurface::shadedSurface, s), true);
//	      printf("Reusing DL=%i for s=%i\n", cs.dList(CompositeSurface::shadedSurface, s), s);
	    }
	    else // get a new list which is lit, plotted, hideable, but not interactive
	    {
	      cs.dList(CompositeSurface::shadedSurface, s) = gi.generateNewDisplayList(true, true, true, false);  
	      glNewList(cs.dList(CompositeSurface::shadedSurface, s), GL_COMPILE);
// debug
//	      printf("New DL=%i for s=%i\n", cs.dList(CompositeSurface::shadedSurface, s), s);
#ifdef PF_DEBUG
	      int listnumber =cs.dList(CompositeSurface::shadedSurface, s); //debug **pf
	      printf("cs> LIT. Shaded surf, list %5i = %s \n",              //debug **pf
		     listnumber,(const char*)  cs[s].getClassName());       //debug **pf
#endif
              // printf(" plotCS: cs.getColour(%i)=%s \n",s,(const char*)cs.getColour(s));
	      
// Bill's compromise for colouring
              if( cs[s].getDomainDimension()==2 && cs[s].getRangeDimension()==3 )
	      {// Surfaces are coloured by the value in the CompositeSurface
                par.boundaryColourOption=GraphicsParameters::colourByGrid; 
  	        par.set(GI_MAPPING_COLOUR, cs.getColour(s));
	      }
              else
	      { // Not a 3D surface: colour according to the option in the graphics parameters.
                par.boundaryColourOption=boundaryColourOptionOld;
	      }

// 	      if ( cs[s].getClassName() == "TrimmedMapping" )
// 		plotTrimmedMapping((TrimmedMapping &)cs[s], par, cs.dList(CompositeSurface::shadedSurface, s), 
// 				   true);
// 	      else if ( cs[s].getClassName() == "NurbsMapping" )
// 		plotNurbsMapping((NurbsMapping &)cs[s], par, cs.dList(CompositeSurface::shadedSurface, s), true);
// 	      else
// 		plotStructured(cs[s], par, cs.dList(CompositeSurface::shadedSurface, s), true);

              plot(gi, cs[s], par, cs.dList(CompositeSurface::shadedSurface, s), true);
	      glEndList();
	    }
	  }
	  else // this might be better to do in the update function for CS
	  {
	    if (glIsList(cs.dList(CompositeSurface::shadedSurface, s)))
	      gi.setPlotDL(cs.dList(CompositeSurface::shadedSurface, s), false);
	  }
	}
      } // end plotShadedMappingBoundaries
      else
// toggle off all display lists with shaded surfaces
      {
        for( s=0; s<cs.numberOfSubSurfaces(); s++)
	{
	  if (glIsList(cs.dList(CompositeSurface::shadedSurface, s)))
	    gi.setPlotDL(cs.dList(CompositeSurface::shadedSurface, s), false);
	}
      }

      if( par.plotMappingNormals )
	plotSubSurfaceNormals(gi, cs, par);


// reset graphics parameters
      par.plotMappingEdges = localPlotMappingEdges;
      par.plotLinesOnMappingBoundaries = localPlotLines;
      par.plotShadedMappingBoundaries = localPlotShaded;
      par.plotUnsEdges=par.plotLinesOnMappingBoundaries;
      par.plotUnsBoundaryEdges=par.plotMappingEdges;
      par.plotNonPhysicalBoundaries=localPlotNonPhysicalBoundaries; // for volume grids

// don't use the plot bounds in graphicsParameters after this
      par.set(GI_USE_PLOT_BOUNDS, false);

      par.set(GI_PLOT_THE_OBJECT_AND_EXIT,plotObjectAndExit);
      par.set(GI_PLOT_LABELS, plotTitleLabels);
      par.set(GI_LABEL_GRIDS_AND_BOUNDARIES, localSquares);

      if( par.plotTheAxes )
      {
  	gi.setColour(GenericGraphicsInterface::textColour);
        gi.setAxesLabels(cs.getName(Mapping::rangeAxis1Name),
			 cs.getName(Mapping::rangeAxis2Name),
			 cs.getName(Mapping::rangeAxis3Name));
      }

      if( true ||    // *wdh* 100220 -- always plot here, can be turned off in Options menu
          par.labelGridsAndBoundaries )
      {
	IntegerArray numberList(min(GenericGraphicsInterface::numberOfColourNames,
				    cs.numberOfSubSurfaces())); 
	numberList=0;
	for( i=0; i<min(GenericGraphicsInterface::numberOfColourNames, cs.numberOfSubSurfaces()); i++)
	  numberList(i)=i;
    
        gi.drawColouredSquares(numberList, par/*, GenericGraphicsInterface::numberOfColourNames, colourNames*/);//AP
      }

      // plot labels on top and bottom
      if( par.plotTitleLabels )
      {
        gi.plotLabels( par );
      }

      gi.redraw();
    }
  }
  
  if( !plotObjectAndExit )
  {
//    par.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);  // reset
//  currentGUI->setUserButtons(NULL); // remove push buttons
    gi.popGUI(); // restore the previous GUI
  }
  
  par.set(GI_MAPPING_COLOUR, oldColour); // reset default mapping colour

  numberOfGhostLinesToPlot=oldNumberOfGhostLinesToPlot;  // reset
  par.boundaryColourOption=boundaryColourOptionOld;  // reset

  gi.unAppendTheDefaultPrompt(); // reset defaultPrompt

}


void PlotIt::
plotSubSurfaceNormals(GenericGraphicsInterface &gi, CompositeSurface & cs, GraphicsParameters & params)
// =====================================================================================
// /visibility:
//   This is a protected routine.
// /Description:
//   Plot the normals on all the sub-surfaces.
//   
// =====================================================================================
{
//  if( !topologyDetermined )
//      determineTopology();
  
// make sure the topology has been determined before proceeding
  if ( !cs.isTopologyDetermined() )
  {
    printf("INFO: Can't plot normals before the topology has been determined\n");
    return;
  }
  
  if( !gi.isGraphicsWindowOpen() )
    return;
  
  int list = gi.generateNewDisplayList();  // get a new display list to use
  assert(list!=0);
  glNewList(list,GL_COMPILE);

  Range xAxes(0,cs.getRangeDimension()-1);

//   realArray xBound; 
//   params.get(GI_PLOT_BOUNDS,xBound);

//   // Scale the picture to fit in [-1,1]
//   glMatrixMode(GL_MODELVIEW);
//   glPushMatrix();

//   real scale = max(xBound(End,xAxes)-xBound(Start,xAxes));
//   printf("plot normals: scale=%e \n",scale);
//   real fractionOfScreen=.75;                                  // scale to [fOS,fOS]
//   real aspectScaleFactor=fractionOfScreen*2./scale;                 
    
//   glScalef(aspectScaleFactor,aspectScaleFactor,aspectScaleFactor);                  
//   glTranslatef(-(xBound(Start,0)+xBound(End,0))*.5,
// 	       -(xBound(Start,1)+xBound(End,1))*.5,
// 	       -(xBound(Start,2)+xBound(End,2))*.5);     // centre object

  gi.setColour(GenericGraphicsInterface::textColour);
  real lineWidth;
  glLineWidth(params.get(GraphicsParameters::lineWidth,lineWidth)*3.);

  
  const int n1=3, n2=3;
  int numberOfPoints=n1*n2;
  Range I(0,numberOfPoints-1);
  
  RealArray xBound;
  xBound = gi.getGlobalBound();
  real scale = max(xBound(End,xAxes)-xBound(Start,xAxes));
  real lengthOfArrow=scale*.03;  // *.02;  // **** in normalized coordinates


  realArray r(numberOfPoints,2),rc(numberOfPoints,1),x(numberOfPoints,3),xr(numberOfPoints,3,2),
            normal(numberOfPoints,3),arrow(numberOfPoints,3,2);
  intArray subSurfaceIndex(numberOfPoints);


  glBegin(GL_LINES);
  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    Mapping & map = cs[s];  // subSurface
    
#ifdef OLDSTUFF
    if( map.getClassName()=="TrimmedMapping" && ((TrimmedMapping &)map).outerCurve!=0  )
    {
      TrimmedMapping & trim = (TrimmedMapping &) map;
      // place some points on the outer trimming curve
      Mapping & curve = *trim.outerCurve;
      const int numberOfPointsPerSide = n1*n2;
      for( int n=0; n<numberOfPointsPerSide; n++ )
      {
	rc(n,0)=(n+.5)/numberOfPointsPerSide;
	curve.map(rc,r); // get the unit square coordinates for the sub-surface
      }
    }
#else
    if ( map.getClassName()=="TrimmedMapping" && ((TrimmedMapping &)map).getNumberOfTrimCurves()>0 )
    {
      TrimmedMapping & trim = (TrimmedMapping &) map;
      // place some points on the outer trimming curve
      Mapping & curve = * trim.getTrimCurve(0);
      const int numberOfPointsPerSide = n1*n2;
      for( int n=0; n<numberOfPointsPerSide; n++ )
      {
	rc(n,0)=(n+.5)/numberOfPointsPerSide;
	curve.map(rc,r); // get the unit square coordinates for the sub-surface
      }
    }
#endif
    else
    { // place points over the surface
      r.reshape(n1,n2,2);
      for( int i2=0; i2<n2; i2++ )
	for( int i1=0; i1<n1; i1++ )
	{
	  r(i1,i2,0)=(i1+1)/(n1+1.);
	  r(i1,i2,1)=(i2+1)/(n2+1.);
	}
      r.reshape(n1*n2,2);
    }
    
    map.map(r,x,xr);  // find point on the subsurface
    subSurfaceIndex=s;
    cs.getNormals(subSurfaceIndex,xr,normal);

    // x.display(sPrintF(buff,"x for sub-surface %i",s));
    // normal.display(sPrintF(buff,"normals for sub-surface %i",s));

    // starting points
    arrow(I,xAxes,0) = x(I,xAxes);
    // ending points
    arrow(I,xAxes,1) = x(I,xAxes)+ normal(I,xAxes)*lengthOfArrow;
    
    for( int i=0; i<numberOfPoints; i++ )
    {
      // draw the normal as a line. First half black, second half the colour of the sub-surface
      gi.setColour(GenericGraphicsInterface::textColour);
      glVertex3(arrow(i,0,0),arrow(i,1,0),arrow(i,2,0));
      glVertex3(.5*(arrow(i,0,0)+arrow(i,0,1)),
                .5*(arrow(i,1,0)+arrow(i,1,1)),
                .5*(arrow(i,2,0)+arrow(i,2,1)));
      gi.setColour(gi.getColourName(s));
      glVertex3(.5*(arrow(i,0,0)+arrow(i,0,1)),
                .5*(arrow(i,1,0)+arrow(i,1,1)),
                .5*(arrow(i,2,0)+arrow(i,2,1)));
      glVertex3(arrow(i,0,1),arrow(i,1,1),arrow(i,2,1));
    }
    
  }
  
  glEnd();     // GL_LINES

  glEndList(); 

}

