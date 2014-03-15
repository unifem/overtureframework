#include "Mapping.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
// #include <typeinfo>

//===============================================================================================
//  View different Mappings
//
//  View mappings that are in the list found in the MappingInformation object, mapInfo.
//
//===============================================================================================
int 
viewMappings( MappingInformation & mapInfo )
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
    
  aString answer;
  // Make a menu containing the names of all the Mapping's
  int numberOfMappings=mapInfo.mappingList.getLength();

  char buff[80];
  int numberOfSolutions=numberOfMappings;
  
  int numberOfMenuItems0=31;  // this many entries in menu if there are no mappings
  const int maximumNumberOfEntriesInMenu=numberOfMenuItems0+numberOfSolutions+2;
  aString *menu = new aString [maximumNumberOfEntriesInMenu];
  int i=0;
  menu[i++]="!view mappings";
  const int mappingListStart=i;
  int j;
  for( j=0; j<numberOfSolutions; j++ )
    menu[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);

  const int mappingListEnd=i-1;
  // add extra menu items
  int extra=i;
  menu[extra++]="choose all";
  menu[extra++]=">plot options";
  menu[extra++]="change colours";
  menu[extra++]="colour boundaries by boundary condition number";
  menu[extra++]="colour boundaries by share value";
  menu[extra++]="colour boundaries by grid number";
  menu[extra++]="plot shaded surfaces (3D) toggle";
  menu[extra++]="toggle shaded surfaces (3D)";
  menu[extra++]="plot grid lines on boundaries (3D) toggle";
  menu[extra++]="plot grid points on curves (toggle)";
  menu[extra++]="plot non-physical boundaries (toggle)";
  menu[extra++]="plot mapping edges";
  menu[extra++]="do not plot mapping edges";
  menu[extra++]="plot ghost lines" ;
  menu[extra++]=" ";
  menu[extra++]="change plot parameters of last plotted object";   
  menu[extra++]="<erase";   
  menu[extra++]="erase and exit";   
  menu[extra++]="exit";   
  menu[extra++]="";   // null string terminates the menu
  assert( extra<maximumNumberOfEntriesInMenu );

  // replace menu with a new cascading menu if there are too many items.
  gi.buildCascadingMenu( menu,mappingListStart,mappingListEnd );



  const int numberOfColourNames=25;
  aString colourNames[numberOfColourNames]
     = { "blue",
         "green",
         "red",
         "CORAL",
         "VIOLETRED",
         "DARKTURQUOISE",
         "STEELBLUE",
         "ORANGE",
         "ORCHID",
         "NAVYBLUE",
         "SALMON",
         "yellow",
         "AQUAMARINE",
         "MEDIUMGOLDENROD",
         "DARKGREEN",
         "WHEAT",
         "SEAGREEN",
         "KHAKI",
         "MAROON",
         "SKYBLUE",
         "SLATEBLUE",
         "DARKORCHID",
         "PLUM",
         "VIOLET",
         "PINK"
	 };

  // keep a separate list of colour names so we can remember changes
  aString localColourNames[numberOfColourNames];
  for( i=0; i<numberOfColourNames; i++ )
    localColourNames[i]=colourNames[i];  

  Mapping *mapPointer;  // pointer to Mapping that we are currently working on
  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  params.set(GI_LABEL_GRIDS_AND_BOUNDARIES,true); // turn on plotting of coloured squares

  bool & labelGridsAndBoundaries = params.getLabelGridsAndBoundaries();

  IntegerArray plotShadedSurface(numberOfMappings); // for 3D mappings
  plotShadedSurface=TRUE;

  IntegerArray mapNumber(mapInfo.mappingList.getLength()+1);
  int numberOfMapsPlotted=0;
  bool changePlotParameters=FALSE;

  gi.appendToTheDefaultPrompt("view mappings>"); // set the default prompt

  int redraw=0;  // 1=conditional redraw, 2=redraw

  for(;;)
  {
    int map = gi.getMenuItem(menu,answer);
 
    gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

    if( map>=0 && map<numberOfMappings )
    {
      // we will plot below
      mapPointer=mapInfo.mappingList[map].mapPointer;
      printf("plot mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));

    }
    else if( answer=="choose all" )
    {
      numberOfMapsPlotted=0;
      for(j=0; j<numberOfMappings; j++ )
        mapNumber(numberOfMapsPlotted++)=-(j+1);  // negative means plot this mapping
      redraw=2; // force a full redraw
    }
    else if( answer=="change plot parameters of last plotted object" && numberOfMapsPlotted > 0 )
    {
      changePlotParameters=TRUE;
      map=abs(mapNumber(numberOfMapsPlotted-1))-1;
    }
    else if( answer=="colour boundaries by boundary condition number" )
    {
      params.getBoundaryColourOption()=GraphicsParameters::colourByBoundaryCondition;
      redraw=2; // force a full redraw
    }
    else if( answer=="colour boundaries by share value" )
    {
      params.getBoundaryColourOption()=GraphicsParameters::colourByShare;
      redraw=2; // force a full redraw
    }
    else if( answer=="colour boundaries by grid number" )
    {
      params.getBoundaryColourOption()=GraphicsParameters::colourByGrid;
      redraw=2; // force a full redraw
    }
    else if( answer=="plot shaded surfaces (3D) toggle" ) // add these
    {
      params.getPlotShadedSurface() = !params.getPlotShadedSurface();   
      plotShadedSurface=params.getPlotShadedSurface();
      redraw=2; // force a full redraw
    }
    else if( answer=="toggle shaded surfaces (3D)" )
    {
      aString *menu2 = new aString[numberOfMapsPlotted+2];
      for(;;)
      {
	for( int i=0; i<numberOfMapsPlotted; i++ )
	{
          int mapNum=abs(mapNumber(i))-1;
	  menu2[i]=sPrintF(buff,"%i : %s is (%s)",i,
			  (const char*)(*(mapInfo.mappingList[mapNum].mapPointer)).getName(Mapping::mappingName),
			  (plotShadedSurface(mapNum) ? "on" : "off"));
	}
	menu2[numberOfMapsPlotted]="exit this menu";
	menu2[numberOfMapsPlotted+1]="";   // null string terminates the menu
	int mapToToggle=gi.getMenuItem(menu2,answer);
	if( answer=="exit this menu" )
	  break;
	else 
	{
	  assert(mapToToggle>=0 && mapToToggle<numberOfMapsPlotted);
          int mapNum=abs(mapNumber(mapToToggle))-1;
	  plotShadedSurface(mapNum)=!plotShadedSurface(mapNum);
          mapNumber(mapToToggle)=-abs(mapNumber(mapToToggle));  // negative -> replot this mapping
	}
      }
      delete [] menu2;
    }
    else if( answer=="plot grid lines on boundaries (3D) toggle" )
    {
      params.getPlotLinesOnMappingBoundaries()= !params.getPlotLinesOnMappingBoundaries();
      redraw=2; // force a full redraw
    }
    else if( answer=="plot non-physical boundaries (toggle)" )
    {
      params.getPlotNonPhysicalBoundaries()= !params.getPlotNonPhysicalBoundaries();
      redraw=2; // force a full redraw
    }
    else if( answer=="plot mapping edges" )
    {
      params.set(GI_PLOT_MAPPING_EDGES,TRUE);      
      redraw=2; // force a full redraw
    }
    else if( answer=="do not plot mapping edges" )
    {
      params.set(GI_PLOT_MAPPING_EDGES,FALSE);      
      redraw=2; // force a full redraw
    }
    else if( answer=="plot grid points on curves (toggle)" )
    {
      params.getPlotGridPointsOnCurves()= !params.getPlotGridPointsOnCurves();  
      redraw=2; // force a full redraw
    }
    else if( answer=="plot ghost lines" )
    {
      int numberOfGhostLinesToPlot=params.getNumberOfGhostLinesToPlot();
      gi.inputString(answer,sPrintF(buff,"Enter the number of ghost lines (or cells) to plot (current=%i)",
				  numberOfGhostLinesToPlot)); 
      if( answer!="" )
      {
	sScanF(answer,"%i ",&numberOfGhostLinesToPlot);
        gi.outputString(sPrintF(buff,"Plot %i ghost lines\n",numberOfGhostLinesToPlot));
        params.getNumberOfGhostLinesToPlot()=numberOfGhostLinesToPlot;
        redraw=2;
      }
    }
// this functionality is now built into the GUI
//      else if( answer=="plot number labels (toggle)" )
//      {
//        labelGridsAndBoundaries=!labelGridsAndBoundaries;
//        if( !labelGridsAndBoundaries && gi.graphicsIsOn() )
//        {
//  	glDeleteLists( gi.getColouredSquaresDL(gi.getCurrentWindow()), 1 ); 
//        }
      
//        redraw=1;
//      }
    else if( answer=="change colours" )
    { 
      // change the colours that appear in the localColourNames array
      printf("Colour changes only apply to mappings plotted by grid number\n");
      aString *menu2 = new aString[numberOfColourNames+3];
      for(;;)
      {
        int j=0;
	menu2[j++]="all";
	for( i=0; i<numberOfColourNames; i++ )
	{
	  menu2[j++]=sPrintF(buff,"%i : (%s)",i, (const char*) localColourNames[i]);
	}
	menu2[j++]="exit this menu";
	menu2[j++]="";   // null string terminates the menu
        aString answer2;
        int colourNumber=gi.getMenuItem(menu2,answer2,"change which colour?")-1;
        if( answer2=="exit this menu" )
          break;
	else 
	{
          aString answer3 = gi.chooseAColour();
          if( answer3!="no change" )
	  {
            if( colourNumber>=0 )
	    {
	      assert( colourNumber<numberOfColourNames);
              localColourNames[colourNumber] = answer3;
	    }
            else
	    {
	      for( j=0; j<numberOfColourNames; j++ )
		localColourNames[j] = answer3;
	    }
	  }
	}
      }
      delete [] menu2;
      redraw=2; // force a full redraw
    }
    else if( answer=="erase and exit" )
    {
      gi.erase();
      // params.set(GI_PLOT_BOUNDS,nullBounds);  // reset the plot bounds
      numberOfMapsPlotted=0;
      break;
    }
    else if( answer=="erase" )
    {
      gi.erase();
      // params.set(GI_PLOT_BOUNDS,nullBounds);  // reset the plot bounds
      numberOfMapsPlotted=0;
      plotShadedSurface=params.getPlotShadedSurface();
    }
    else if( answer=="exit" )
    {
      break;
    }
    else 
    {
      
      bool mapFound=FALSE;
//       if( numberOfSolutions>maximumNumberOfSolutionsInTheMenu )
//       {
// 	// if we haven't displayed all names then we search through the list of Mapping names
//         // to see if the user has typed in the name 
	
//         for( int i=0; i<numberOfMappings; i++ )
// 	{
// 	  if( answer==mapInfo.mappingList[i].getName(Mapping::mappingName) )
// 	  {
// 	    map=i;
//             mapFound=TRUE;
//             break;
// 	  }
// 	}
//       }
      if( !mapFound )      
      {
        cout << "Unknown response:" << (const char*) answer << endl;
	gi.stopReadingCommandFile();
      }
      
    }


    if( map>=0 && map<numberOfMappings )
    {
      mapPointer=mapInfo.mappingList[map].mapPointer;

      if( changePlotParameters )
      {  // replot last object and allow changes to the plot
        gi.erase();
        params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
        params.set(GI_MAPPING_COLOUR,localColourNames[(numberOfMapsPlotted-1) % numberOfColourNames]);

        PlotIt::plot(gi,*mapPointer,params);

        params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
        // remember the colour in case it was changed:
        localColourNames[(numberOfMapsPlotted-1) % numberOfColourNames]=params.getMappingColour();
        changePlotParameters=FALSE;
        redraw=numberOfMapsPlotted>1 ? 2 : 0;
        continue;
      }
      else  
      {
        // plot a new mapping -- first check that we have not already plotted it

        bool newMap=TRUE;
	for( int i=0; i<numberOfMapsPlotted; i++ )
	{
	  if( (abs(mapNumber(i))-1)==map )
          {
            newMap=FALSE;
	    break;
	  }
	}
	if( newMap )
	{
          mapNumber(numberOfMapsPlotted++)=-(map+1); // negative means plot this new mapping
          redraw=1; // partial redraw
	}
        else
	{
	  printf("This mapping is already plotted\n");
	}
      }
    }
    if( redraw && gi.isGraphicsWindowOpen() )
    {

      if( redraw==2 )
      {
	mapNumber=-abs(mapNumber);  // redraw all mappings.
        gi.erase();
      }
      
      const bool oldlabelGridsAndBoundaries=labelGridsAndBoundaries;
      labelGridsAndBoundaries=FALSE;  // turn off so a composite surface does not plot coloured squares

      for( i=0; i<numberOfMapsPlotted; i++)
      {
        if( mapNumber(i)<0 )
	{
          // only plot new mapping's denoted by a negative map number.
	  mapNumber(i)=abs(mapNumber(i));

	  int mapNum=abs(mapNumber(i))-1;

	  params.set(GI_MAPPING_COLOUR,localColourNames[i % numberOfColourNames]);
	  params.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, plotShadedSurface(mapNum) );

          // printf("plot mapNum=%i\n",mapNum);
	  // ------ plot each mapping here -----
	  PlotIt::plot(gi,*(mapInfo.mappingList[mapNum].mapPointer),params);
	}
	
      }

      labelGridsAndBoundaries=oldlabelGridsAndBoundaries;  // reset
      
      // ** draw coloured squares to label the boundaries ***
      if( true || labelGridsAndBoundaries )  // alway draw since they can be toggled off
      {
	IntegerArray numberList(numberOfMapsPlotted*6+1); numberList=-1;
	int side,axis,number=0;
	for( i=0; i<numberOfMapsPlotted; i++)
	{
	  int mapNum=abs(mapNumber(i))-1;

	  Mapping & map = *(mapInfo.mappingList[mapNum].mapPointer);
	  for( axis=0; axis<map.getDomainDimension(); axis++ )
	    for( side=Start; side<=End; side++ )
	    {
	      if( params.getBoundaryColourOption()==GraphicsParameters::colourByShare )
		numberList(number++)=map.getShare(side,axis);	  
	      else if( params.getBoundaryColourOption()==GraphicsParameters::colourByBoundaryCondition )
		numberList(number++)=map.getBoundaryCondition(side,axis);	  
	      else
		numberList(number++)=i;
	    }
	}
        if( params.getBoundaryColourOption()==GraphicsParameters::colourByBoundaryCondition || 
            params.getBoundaryColourOption()==GraphicsParameters::colourByShare )
	{
	  gi.drawColouredSquares(numberList);
	}
	else
	{
	  gi.drawColouredSquares(numberList,params,numberOfColourNames,localColourNames);
	}
	  
      }

      if( redraw==1 )
        gi.redraw();
      
      redraw=0;
    }

  }
  delete [] menu;
  gi.unAppendTheDefaultPrompt();
  return 0;
}
