#include "ModelBuilder.h"
#include "rap.h"

void ModelBuilder::
rapVolumeGrids(MappingInformation &mapInfo, CompositeSurface & model, CompositeSurface & sGrids, 
	       CompositeSurface & vGrids, GraphicsParameters & volumeParameters)
{
  aString buf, answer;
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  GraphicsParameters & gp = *mapInfo.gp_;
  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  GraphicsParameters surfaceParameters;
  surfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  surfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, TRUE);
  surfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, FALSE);

  int map;
  int nVolumeGrids=0;
  Mapping *mapPointer;
  
// make all surface grids visible
  for( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
    sGrids.setIsVisible(s, TRUE);

// count the number of volume grids in the mappingList
  for( map=0; map<mapInfo.mappingList.getLength(); map++)
  {
    mapPointer=mapInfo.mappingList[map].mapPointer;
    if (mapPointer->getDomainDimension() == 3 && mapPointer->getRangeDimension() == 3)
    {
      nVolumeGrids++;
    }
  }


  int plotReferenceSurface=true, plotSurfaceGrids=true, plotLinesOnGrids, plotShadedSurfacesOnGrids, 
    plotBoundariesOnGrids, plotNonPhys;


  volumeParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  volumeParameters.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurfacesOnGrids); // set shaded surfaces
  volumeParameters.get(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnGrids); // plot grid lines
  volumeParameters.get(GI_PLOT_MAPPING_EDGES, plotBoundariesOnGrids); // plot sub-surface edges
  volumeParameters.get(GI_PLOT_NON_PHYSICAL_BOUNDARIES, plotNonPhys); // non-physical boundaries

  GUIState interface;
  
  interface.setWindowTitle("Volume Grids");
  
  interface.setExitCommand("close", "Close");

  aString prefix="VGRD:";
  enum PickEnum { noOp=0, newGrid, editGrid, hideGrid, showGrid, deleteGrid, numberOfSelectionFunctions };
  static PickEnum selectionFunction=noOp;
  
// first option menu
  aString rbCommand[] = {"selection function no operation", "selection function new grid", 
			 "selection function edit grid", 
			 "selection function hide grid", "selection function show grid", 
			 "selection function delete grid", ""};
  aString rbLabel[] = {"No Operation", "New Grid", "Edit Grid", "Hide Grid", "Show Grid", "Delete Grid", 
			"" };

// initial choice: noOp
  addPrefix(rbCommand, prefix);
  interface.addRadioBox( "Selection Function", rbCommand, rbLabel, selectionFunction, 2); // 2 columns
  RadioBox &selectionRadioBox = interface.getRadioBox(0);

// toggle buttons
  enum ToggleButtons  {plotRefSurfaceTB=0, plotSurfaceGridsTB, plotShadedGridsTB, plotGridLinesTB, 
		       plotGridBoundariesTB , plotNonPhysTB};
  
  aString tbLabels[] = {"Ref. Surface", "Surface Grids", "Phys Bndry Shaded", "Phys Bndry Lines", 
			"Edges", "Non-Phys Bndry", ""};
  aString tbCommands[] = {"plot reference surface",
			  "plot surface grids",
			  "plot shaded surfaces on grids",
			  "plot grid lines on grids",
			  "plot boundaries on grids",
			  "plot non-physical boundaries",
			  ""};

  int tbState[] = {plotReferenceSurface,
		   plotSurfaceGrids,
		   plotShadedSurfacesOnGrids,
		   plotLinesOnGrids, 
		   plotBoundariesOnGrids, 
		   plotNonPhys
		   };
    
  addPrefix(tbCommands, prefix);
  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); // organize in 2 columns
// done defining toggle buttons

// setup a user defined menu and some user defined buttons
  aString buttonCommands[] = {"show all grids",
			      "hide all grids",
			      ""};
  aString buttonLabels[] = {"Show all",
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
    
    if (answer.matches("close"))
    {
      break;
    }
    else if( select.nSelect > 0 && selectionFunction == noOp )
    {
      continue;
    }
    else if( select.nSelect > 0 && selectionFunction == newGrid )
    {
// find the active surface
      bool foundSurface=false;
      int i, s=-1;
      
      for( i=0; i<select.nSelect; i++ )
      {
// go through the hyperbolic surface grids made so far
	for( s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == sGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Using surface grid `%s'...", 
				    SC sGrids[s].getName(Mapping::mappingName)));
	    foundSurface = true;
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "new volume grid %i\n", s));
	    break;
	  }
	}
	if (foundSurface) break;
      } // end for select...

      if (foundSurface)
      {
	Mapping & activeSurface = sGrids[s];

	HyperbolicMapping & hyp = *new HyperbolicMapping();
	mapPointer=&hyp;
	mapPointer->incrementReferenceCount();
	mapInfo.mappingList.addElement(*mapPointer);
 
	hyp.setName(Mapping::mappingName, sPrintF(buf, "Volume grid %i", nVolumeGrids++));
	hyp.setSurface( activeSurface, false ); // false means that a volume grid should be created
        hyp.setPlotOption(HyperbolicMapping::setPlotBoundsFromGlobalBounds,true);  // keep the same view.

	mapPointer->update(mapInfo);
// check if a grid was made...
	if (hyp.isDefined())
	{
// add the surface grid to the vGrids composite surface
	  vGrids.add(hyp);
// set the colour
	  vGrids.setColour(vGrids.numberOfSubSurfaces()-1, "seagreen");
	}
	if (hyp.decrementReferenceCount() == 0)
	    delete &hyp;
	
      }
    }
    else if( select.nSelect > 0 && selectionFunction == editGrid )
    {
      bool singleSelect = true;
      for( int i=0; i<select.nSelect; i++ )
      {
// go through the hyperbolic surface grids made so far
	for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == vGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Editing grid `%s'...", 
				    SC vGrids[s].getName(Mapping::mappingName)));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "edit volume grid %i\n", s));

	    gi.getView(loc); // get the viewpoint info
	    vGrids[s].update(mapInfo);
	    vGrids.eraseCompositeSurface(gi, s); // this grid needs to be redrawn...
	    gi.setView(loc); // reset the view point
	    if (singleSelect) break; // only edit one grid if a point was picked...
	  }
	}
      } // end for select...
    }
    else if( select.nSelect > 0 && selectionFunction == deleteGrid )
    {
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      for( int i=0; i<select.nSelect; i++ )
      {
// go through the hyperbolic surface grids made so far
	for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == vGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Deleting grid `%s'...", 
				    SC vGrids[s].getName(Mapping::mappingName)));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "delete volume grid %i\n", s));
// delete the displaylists
	    vGrids.eraseCompositeSurface(gi, s);
// delete the grid
	    vGrids.remove(s);

	    if (singleSelect) break; // only edit one grid if a point was picked...
	  }
	}
      } // end for select...
    }
    else if( select.nSelect > 0 && selectionFunction == hideGrid )
    {
// hide the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == vGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Volume grid %i named `%s' will be hidden",s,
				    SC vGrids[s].getName(Mapping::mappingName)));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "hide volume grid %i\n", s));
	    vGrids.setIsVisible(s, FALSE);
	    if (singleSelect) break; // only hide one grid if a point was picked...
	  }
	}
      }
    }
    else if( select.nSelect > 0 && selectionFunction == showGrid )
    {
// hide the selected sub-surface(s)
      bool singleSelect = (fabs(select.r[0]-select.r[1]) + fabs(select.r[2]-select.r[3]) <= 1.e-5);
      for( int i=0; i<select.nSelect; i++ )
      {
	for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
	{
	  if( select.selection(i,0) == vGrids[s].getGlobalID() )
	  {
	    gi.outputString(sPrintF(buf, "Volume grid %i named `%s' will be shown",s,
				    SC vGrids[s].getName(Mapping::mappingName)));
// save equivalent command
	    gi.outputToCommandFile(sPrintF(buf, "show volume grid %i\n", s));
	    vGrids.setIsVisible(s, TRUE);
	    if (singleSelect) break; // only hide one grid if a point was picked...
	  }
	}
      }
    }
    else if( (len = answer.matches("new volume grid")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	Mapping & activeSurface = sGrids[s];

	HyperbolicMapping & hyp = *new HyperbolicMapping();
	mapPointer=&hyp;
	mapPointer->incrementReferenceCount();
	mapInfo.mappingList.addElement(*mapPointer);
 
	hyp.setName(Mapping::mappingName, sPrintF(buf, "Volume grid %i", nVolumeGrids++));
	hyp.setSurface( activeSurface, false ); // false means that a volume grid should be created
        hyp.setPlotOption(HyperbolicMapping::setPlotBoundsFromGlobalBounds,true);  // keep the same view.

	mapPointer->update(mapInfo);
// check if a grid was made...
	if (hyp.isDefined())
	{
// add the surface grid to the vGrids composite surface
	  vGrids.add(hyp);
// set the colour
	  vGrids.setColour(vGrids.numberOfSubSurfaces()-1, "seagreen");
	}
	if (hyp.decrementReferenceCount() == 0)
	    delete &hyp;
	
      }

    }
    else if( (len = answer.matches("edit volume grid")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	gi.getView(loc); // get the viewpoint info
	vGrids[s].update(mapInfo);
	vGrids.eraseCompositeSurface(gi, s); // this grid needs to be redrawn...
	gi.setView(loc); // reset the view point
      }
    }
    else if( (len = answer.matches("delete volume grid")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
// delete the displaylists
	vGrids.eraseCompositeSurface(gi, s);
// delete the grid
	vGrids.remove(s);
      }
    }
    else if( (len = answer.matches("hide volume grid")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	vGrids.setIsVisible(s, FALSE);
      }
    }
    else if( (len = answer.matches("show volume grid")) )
    {
      int s=-1;
      if (sScanF(answer(len,answer.length()-1),"%i", &s) == 1 && s>=0)
      {
	vGrids.setIsVisible(s, TRUE);
      }
    }
    else if( answer.matches("plot reference surface") )
    {
      plotReferenceSurface=!plotReferenceSurface;
      interface.setToggleState(plotRefSurfaceTB, plotReferenceSurface);
      gi.erase(); // calls resetGlobalBounds
    }
    else if( answer.matches("plot surface grids") )
    {
      plotSurfaceGrids=!plotSurfaceGrids;
      interface.setToggleState(plotSurfaceGridsTB, plotSurfaceGrids);
      gi.erase(); // calls resetGlobalBounds
    }
    else if( answer.matches("plot shaded surfaces on grids") )
    {
      plotShadedSurfacesOnGrids=!plotShadedSurfacesOnGrids;
      interface.setToggleState(plotShadedGridsTB, plotShadedSurfacesOnGrids);
      volumeParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurfacesOnGrids);
    }
    else if( answer.matches("plot grid lines on grids") )
    {
      plotLinesOnGrids=!plotLinesOnGrids;
      interface.setToggleState(plotGridLinesTB, plotLinesOnGrids);
      volumeParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, plotLinesOnGrids);
      vGrids.eraseCompositeSurface(gi);
    }
    else if( answer.matches("plot boundaries on grids") )
    {
      plotBoundariesOnGrids = !plotBoundariesOnGrids;
      interface.setToggleState(plotGridBoundariesTB, plotBoundariesOnGrids);
      volumeParameters.set(GI_PLOT_MAPPING_EDGES, plotBoundariesOnGrids);
      vGrids.eraseCompositeSurface(gi);
    }
    else if( answer.matches("plot non-physical boundaries") )
    {
      plotNonPhys = !plotNonPhys;
      printf("New value of plot non-physical boundaries: %i\n", plotNonPhys);
      interface.setToggleState(plotNonPhysTB, plotNonPhys);
      volumeParameters.set(GI_PLOT_NON_PHYSICAL_BOUNDARIES, plotNonPhys);
      vGrids.eraseCompositeSurface(gi);
    }
    else if( answer.matches("show all grids") )
    {
      for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
      {
	gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be shown",s,
				SC vGrids[s].getName(Mapping::mappingName)));
	vGrids.setIsVisible(s, TRUE);
      }
    }
    else if( answer.matches("hide all grids") )
    {
      for( int s=0; s<vGrids.numberOfSubSurfaces(); s++ )
      {
	gi.outputString(sPrintF(buf, "Sub-surface %i named `%s' will be hidden",s,
				SC vGrids[s].getName(Mapping::mappingName)));
	vGrids.setIsVisible(s, FALSE);
      }
    }
    
    else if( len = answer.matches("selection function") )
    {
      while( len < answer.length() && answer(len,len) == " ")
	len++;
      
      buf = answer(len,answer.length()-1);
      if (buf.matches("no operation"))
	selectionFunction = noOp;
      else if (buf.matches("new grid"))
	selectionFunction = newGrid;
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
      printf("Current selction function: %i\n", selectionFunction);
    }
    

    if (plotObject)
    {
// plot the reference surface
      if (plotReferenceSurface)
      {
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
      
// plot all hyperbolic surface grids and the volume grids made so far
      if (plotSurfaceGrids)
	PlotIt::plot(gi, sGrids, surfaceParameters);
      PlotIt::plot(gi, vGrids, volumeParameters);
      
    }
  } // end for(;;)
  gi.popGUI();
  
}



