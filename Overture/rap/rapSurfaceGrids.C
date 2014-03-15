#include "ModelBuilder.h"
#include "rap.h"

void ModelBuilder::
rapSurfaceGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
		GraphicsParameters & surfaceParameters)
{
  aString buf, answer;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  GraphicsParameters & gp = *mapInfo.gp_;

  int map;
  int nSurfaceGrids=0;
  Mapping *mapPointer;
  
  for( map=0; map<mapInfo.mappingList.getLength(); map++)
  {
    mapPointer=mapInfo.mappingList[map].mapPointer;
    if (mapPointer->getDomainDimension() == 2 && mapPointer->getRangeDimension() == 3)
    {
      nSurfaceGrids++;
    }
  }

  int plotReferenceSurface=true, plotLinesOnGrids, plotShadedSurfacesOnGrids, 
    plotBoundariesOnGrids;


  surfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  surfaceParameters.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurfacesOnGrids); // set shaded surfaces
  surfaceParameters.get(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnGrids); // plot grid lines
  surfaceParameters.get(GI_PLOT_MAPPING_EDGES, plotBoundariesOnGrids); // plot sub-surface edges

  GUIState interface;
  
  interface.setWindowTitle("Surface Grids");
  
  interface.setExitCommand("close", "Close");

  aString prefix="SGRD:";
  enum PickEnum { noOp=0, editGrid, hideGrid, showGrid, deleteGrid, numberOfSelectionFunctions };
  static PickEnum selectionFunction=noOp;
  
// first option menu
  aString rbCommand[] = {"selection function no operation", "selection function edit grid", 
			  "selection function hide grid", "selection function show grid", 
			 "selection function delete grid", ""};
  aString rbLabel[] = {"No Operation", "Edit Grid", "Hide Grid", "Show Grid", "Delete Grid", 
			"" };

// initial choice: noOp
  addPrefix(rbCommand, prefix);
  interface.addRadioBox( "Selection Function", rbCommand, rbLabel, selectionFunction, 2); // 2 columns
  RadioBox &selectionRadioBox = interface.getRadioBox(0);

// toggle buttons
  enum ToggleButtons  {plotRefSurfaceTB=0, plotShadedGridsTB, plotGridLinesTB, plotGridBoundariesTB };
  
  aString tbLabels[] = {"Ref. Surface", "Shaded Grids", "Grid Lines", "Grid Bndry", ""};
  aString tbCommands[] = {"plot reference surface",
			  "plot shaded surfaces on grids",
			  "plot grid lines on grids",
			  "plot boundaries on grids",
			  ""};

  int tbState[] = {plotReferenceSurface,
		   plotShadedSurfacesOnGrids,
		   plotLinesOnGrids, 
		   plotBoundariesOnGrids, 
		   };
    
  addPrefix(tbCommands, prefix);
  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); // organize in 2 columns
// done defining toggle buttons

// setup a user defined menu and some user defined buttons
  aString buttonCommands[] = {"make surface grid",
			      "show all grids",
			      "hide all grids",
			      ""};
  aString buttonLabels[] = {"New Grid",
			    "Show all",
			    "Hide all",
			    ""};
  
  addPrefix(buttonCommands, prefix);
  interface.setPushButtons(buttonCommands, buttonLabels, 1); // one row

// define pulldown menus
  aString pdCommand2[] = {"help selection function", ""};
  aString pdLabel2[] = {"Selection function", ""};
  interface.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  interface.setLastPullDownIsHelp(true);
// done defining pulldown menus  

  gi.pushGUI(interface);
  bool plotObject;
  int retCode, len;
  SelectionInfo select;
  ViewLocation loc;

  int numberOfMappings;

  for(;;)
  {
// set the sensitivity of some buttons

    plotObject = true;

    gi.savePickCommands(false); // temporarily turn off saving of pick commands.     
    retCode = gi.getAnswer(answer, "", select);
    gi.savePickCommands(true); // restore

// take off the prefix
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);
    
    if( answer.matches("make surface grid" ) )
    {
      HyperbolicMapping & hyp = *new HyperbolicMapping(); 
      hyp.incrementReferenceCount();
      hyp.setName(Mapping::mappingName, sPrintF(buf, "Surface grid %i", nSurfaceGrids++));
      hyp.setSurface(model);
      mapPointer=&hyp;
      mapPointer->update(mapInfo);
// check if a grid was made...
      if (hyp.isDefined())
      {
// add the surface grid to the sGrids composite surface
	sGrids.add(hyp);
// set the colour
	sGrids.setColour(sGrids.numberOfSubSurfaces()-1, "gray50");
      }
      if (hyp.decrementReferenceCount() == 0)
        delete &hyp;
      
    }
    else if (answer.matches("close"))
    {
      break;
    }
    else if( select.nSelect > 0 && selectionFunction == noOp )
    {
      continue;
    }
    else if( select.nSelect > 0 && selectionFunction == editGrid )
    {
      bool singleSelect = true; /*(fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);*/
      bool found=false;
      for( int i=0; i<select.nSelect && !found; i++ )
      {
// go through the hyperbolic surface grids made so far
	for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == sGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Editing grid `%s'...", 
				    SC sGrids[s].getName(Mapping::mappingName)));
	    gi.getView(loc); // get the viewpoint info
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "edit surface grid %i\n", s));
// update that surface grid
	    sGrids[s].update(mapInfo);
	    sGrids.eraseCompositeSurface(gi, s); // this grid needs to be redrawn...
	    gi.setView(loc); // reset the view point
	    if (singleSelect)
	    {
	      found = true;
	      break; // only edit one grid if a point was picked...
	    }
	    
	  }
	}
      } // end for select...
    }
    else if( select.nSelect > 0 && selectionFunction == deleteGrid )
    {
      bool singleSelect = true; /*(fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);*/
      for( int i=0; i<select.nSelect; i++ )
      {
// go through the hyperbolic surface grids made so far
	for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == sGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Deleting grid `%s'...", 
				    SC sGrids[s].getName(Mapping::mappingName)));
// delete the displaylists
	    sGrids.eraseCompositeSurface(gi, s);
// delete the grid
	    sGrids.remove(s);
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "delete surface grid %i\n", s));

	    if (singleSelect) break; // only edit one grid if a point was picked...
	  }
	}
      } // end for select...
    }
    else if( select.nSelect > 0 && selectionFunction == hideGrid )
    {
// hide the selected sub-surface(s)
      bool singleSelect = true; /*(fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);*/
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == sGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Surface %i named `%s' will be hidden",s,
				    SC sGrids[s].getName(Mapping::mappingName)));
	    sGrids.setIsVisible(s, FALSE);
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "hide surface grid %i\n", s));
	    if (singleSelect) break; // only hide one grid if a point was picked...
	  }
	}
      }
    }
    else if( select.nSelect > 0 && selectionFunction == showGrid )
    {
// hide the selected sub-surface(s)
      bool singleSelect = true; /*(fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);*/
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == sGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Surface %i named `%s' will be shown",s,
				    SC sGrids[s].getName(Mapping::mappingName)));
	    sGrids.setIsVisible(s, TRUE);
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "show surface grid %i\n", s));
	    if (singleSelect) break; // only hide one grid if a point was picked...
	  }
	}
      }
    }
    else if( answer.matches("plot reference surface") )
    {
      plotReferenceSurface=!plotReferenceSurface;
      interface.setToggleState(plotRefSurfaceTB, plotReferenceSurface);
      gi.erase(); // calls resetGlobalBounds
    }
    else if( answer.matches("plot shaded surfaces on grids") )
    {
      plotShadedSurfacesOnGrids=!plotShadedSurfacesOnGrids;
      interface.setToggleState(plotShadedGridsTB, plotShadedSurfacesOnGrids);
      surfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurfacesOnGrids);
    }
    else if( answer.matches("plot grid lines on grids") )
    {
      plotLinesOnGrids=!plotLinesOnGrids;
      interface.setToggleState(plotGridLinesTB, plotLinesOnGrids);
      surfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnGrids);
    }
    else if( answer.matches("plot boundaries on grids") )
    {
      plotBoundariesOnGrids = !plotBoundariesOnGrids;
      interface.setToggleState(plotGridBoundariesTB, plotBoundariesOnGrids);
      surfaceParameters.set(GI_PLOT_MAPPING_EDGES, plotBoundariesOnGrids);
    }
    else if( answer.matches("show all grids") )
    {
      for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
      {
	gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be shown",s,
				SC sGrids[s].getName(Mapping::mappingName)));
	sGrids.setIsVisible(s, TRUE);
      }
    }
    else if( answer.matches("hide all grids") )
    {
      for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
      {
	gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be hidden",s,
				SC sGrids[s].getName(Mapping::mappingName)));
	sGrids.setIsVisible(s, FALSE);
      }
    }
    
    else if( len = answer.matches("selection function") )
    {
      while( len < answer.length() && answer(len,len) == " ")
	len++;
      
      buf = answer(len,answer.length()-1);
      if (buf.matches("no operation"))
	selectionFunction = noOp;
      else if (buf.matches("edit grid"))
	selectionFunction = editGrid;
      else if (buf.matches("hide grid"))
	selectionFunction = hideGrid;
      else if (buf.matches("show grid"))
	selectionFunction = showGrid;
      else if (buf.matches("delete grid"))
	selectionFunction = deleteGrid;
      else
      {
	printf("Unknown selection function `%s'\n", SC buf);
      }

      if ( noOp <= selectionFunction && selectionFunction < numberOfSelectionFunctions )
      {
	if (selectionRadioBox.setCurrentChoice(selectionFunction))
	{
	  gi.outputString(sPrintF(buf, "selection function %s (#%d)", SC rbLabel[selectionFunction],
				  selectionFunction));
	}
	else
	{
	  gi.outputString(sPrintF(buf, "ERROR: selection function %s (#%d) is inactive", 
				  SC rbLabel[selectionFunction], selectionFunction));
	  selectionFunction = noOp;
	}
	
      }
      else
      {
	gi.outputString(sPrintF(buf, "Error: Bad selection function: %d", selectionFunction));
      }
    }
// replay commands
    else if (len=answer.matches("edit surface grid"))
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	gi.getView(loc); // get the viewpoint info
	sGrids[s].update(mapInfo);
	sGrids.eraseCompositeSurface(gi, s); // this grid needs to be redrawn...
	gi.setView(loc); // reset the view point
      }
    }
    else if (len=answer.matches("delete surface grid"))
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
// delete the displaylists
	sGrids.eraseCompositeSurface(gi, s);
// delete the grid
	sGrids.remove(s);
      }
    }
    else if (len=answer.matches("hide surface grid"))
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	sGrids.setIsVisible(s, false);
      }
    }
    else if (len=answer.matches("show surface grid"))
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	sGrids.setIsVisible(s, true);
      }
    }
// unknown response
    else if (!select.active) // if a mogl-pickOutside event occurs, select.active will be true
    {
      gi.outputString( sPrintF(buf,"Unknown response=%s", (const char*)answer) );
      gi.stopReadingCommandFile();
      plotObject=false;
    }
    
    

    if (plotObject)
    {
// plot the reference surface
      if (plotReferenceSurface)
      {
	gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	PlotIt::plot(gi, model, gp);
      }
      else
      {
// just set the bounding box
	RealArray xBound(2,3);
	for( int axis=0; axis<model.getRangeDimension(); axis++ )
	{
	  xBound(Start,axis)=(real)model.getRangeBound(Start,axis);
	  xBound(End,axis)=(real)model.getRangeBound(End,axis);
	}
//  xBound.display(" plotCompositeSurface: **** xBound **** ");
	gi.setGlobalBound(xBound);
      }
      
// plot the hyperbolic surface grids made so far
      PlotIt::plot(gi, sGrids, surfaceParameters);
      
    }
  } // end for(;;)
  gi.popGUI();
  
}



