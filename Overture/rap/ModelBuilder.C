#include "ModelBuilder.h"

#include "rap.h"


//\begin{>ModelBuilderInclude.tex}{\subsection{Constructor}}
ModelBuilder::
ModelBuilder() : points(100)
//===========================================================================
// /Description:
//     Default constructor for thr ModelBuilder class
//\end{ModelBuilderInclude.tex}
//===========================================================================
{
}


ModelBuilder::
~ModelBuilder()
{
}


int ModelBuilder::
update( MappingInformation & mapInfo, const aString & modelFileNameInput /* =nullString */  )
{

  assert( mapInfo.graphXInterface!=NULL );
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;

  aString modelFileName = modelFileNameInput;

  GraphicsParameters gp, surfaceParameters, volumeParameters;
  mapInfo.gp_ = &gp;

  SelectionInfo select;

  mapInfo.selectPtr = &select;

//    PointList points(100); // 100 points to start with
//    ListOfMappingRC curveList;

//    CompositeSurface & model = *new CompositeSurface;
//    CompositeSurface & deletedSurfaces = *new CompositeSurface;
//    CompositeSurface & sGrids = *new CompositeSurface;
//    CompositeSurface & vGrids = *new CompositeSurface;

//    model.incrementReferenceCount();
//    deletedSurfaces.incrementReferenceCount();
//    sGrids.incrementReferenceCount();
//    vGrids.incrementReferenceCount();

  printf("model globalID=%i, deletedSurfaces=%i, sGrids=%i, vGrids=%i\n", model.getGlobalID(), 
	 deletedSurfaces.getGlobalID(), sGrids.getGlobalID(), vGrids.getGlobalID());
  
  CompositeTopology *topology_;
  
  model.setName(Mapping::mappingName, "model");
  deletedSurfaces.setName(Mapping::mappingName, "deleted surfaces");
  sGrids.setName(Mapping::mappingName, "Surface grids");
  vGrids.setName(Mapping::mappingName, "Volume grids");

  mapInfo.mappingList.addElement(model); 

  aString buf;

  // MappingBuilder builder;

//    // read from a command file if given
//    if( commandFileName.length() > 0 )
//      gi.readCommandFile(commandFileName);

//  // by default, start logging all output to the echo file "rap.log"
//    aString echoFile="rap.log";
//    gi.saveEchoFile(echoFile);

//    // By default start logging the commands in the file "rap.cmd"
//    aString logFile="rap.cmd";
//    gi.saveCommandFile(logFile);
//    aString buf;
//    gi.outputString(sPrintF(buf, "User commands are being saved in the file `%s'", SC logFile));

  bool modelOpen = false, modelOK = false, madeSurfaceGrids = false, topologyAvailable = false;
  int map, nSurfaceGrids=0, len;
  
  // read the model given in the command line argument
  if (modelFileName.length() > 0 && modelFileName != " ")
  { 
    HDF_DataBase db;
    // Save names and directories (this is a database after all)
    //      db.setMode(GenericDataBase::noStreamMode); 
    if (db.mount(modelFileName,"R") == 0)
    {
      // Names and directories are saved (this is a database after all)
      //	db.setMode(GenericDataBase::noStreamMode);  BUT IT IS SOOOO SLOW

      CompositeSurface * subModel_=NULL;
      // To obtain the topology from the first model, we read it into the main model directly
      // For subsequent parts, we just add the sub surfaces
      model.get(db,"Rap model");
      sGrids.get(db,"Rap surface grids"); // get the surface grids
      vGrids.get(db,"Rap volume grids");  // get the volume grids
      db.unmount();                       // close the data base

      modelOpen = true;

      // set the colour of the sub-surfaces
      for( map=0; map<model.numberOfSubSurfaces(); map++ )
	model.setColour(map,gi.getColourName(map));

      gi.setPlotTheAxes(true);
      gp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);

      int numberOfMappings = mapInfo.mappingList.getLength();
      for (int s=0; s<numberOfMappings; s++)
      {
	Mapping *mapPointer=mapInfo.mappingList[s].mapPointer;
	printf("Mapping #%i, named `%s' is a `%s'\n", s, SC mapPointer->getName(Mapping::mappingName), 
	       SC mapPointer->getClassName());
      }
      
    } // end if db.mount successful
    else
    {
      sPrintF(buf,"Could not open the data base `%s'", SC modelFileName);
      gi.createMessageDialog(buf, errorDialog);
    }
  }
  
  bool plotReferenceSurface=true, plotSurfaceGrids=true, plotVolumeGrids=true;

  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  gp.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,TRUE);    // plot shaded surfaces
  gp.set(GI_GRID_LINE_COLOUR_OPTION,GraphicsParameters::defaultColour); // black grid lines
  gp.set(GI_BOUNDARY_COLOUR_OPTION,GraphicsParameters::colourByGrid); // individual colours for the components
  gp.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,FALSE); // no grid lines
  gp.set(GI_PLOT_MAPPING_EDGES, TRUE); // plot sub-surface edges
  gp.set(GI_SURFACE_OFFSET, (float) 7.0); // use a big offset for the reference surface

  surfaceParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  surfaceParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, FALSE); // set shaded surfaces
  surfaceParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, TRUE); // plot grid lines
  surfaceParameters.set(GI_PLOT_MAPPING_EDGES, TRUE); // plot sub-surface edges
  surfaceParameters.set(GI_SURFACE_OFFSET, (float) 1.0); // use small offset for the surface grids

  volumeParameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  volumeParameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, FALSE); // set shaded surfaces
  volumeParameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, TRUE); // plot grid lines
  volumeParameters.set(GI_PLOT_MAPPING_EDGES, TRUE); // plot sub-surface edges
  volumeParameters.set(GI_SURFACE_OFFSET, (float) 1.0); // use small offset for the surface grids

  int std_win = 0; // the default window has number 0

  aString answer,answer2;

  // setup a GUI
  GUIState interface;

  // enum PushButtons {simplePB=0, editPB, topologyPB, surfacePB, volumePB};
  
  interface.setWindowTitle("Rap");
  
  interface.setExitCommand("exit", "Exit");

  // define push buttons
  aString pbCommands[] = {"simple geometry", "liner...","heal model", "topology", "make surface grids", 
			  "make volume grids", ""};
  aString pbLabels[] = {"Simple Geometry...", "liner...","Edit model...", "Topology...", "Surface Grids...", 
			"Volume Grids...", ""};
  
  interface.setPushButtons( pbCommands, pbLabels, 3 ); // organize buttons in 3 rows

  // define pulldown menus
  aString pdCommand0[] = {"new/append model", "open model", "save model", "exit", ""};
  aString pdLabel0[] = {"Import/Append...", "Open/Append...", "Save", "Exit", ""};
  interface.addPulldownMenu("File", pdCommand0, pdLabel0, GI_PUSHBUTTON);

  aString pdCommand2[] = {"help new", "help open", "help save", "help edit", "help check", 
			  "help surface grids", "help volume grids", ""};
  aString pdLabel2[] = {"New", "Open", "Save", "Edit model", "Check model", 
			"Surface Grids", "Volume Grids", ""};
  interface.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  interface.setLastPullDownIsHelp(1);
  // done defining pulldown menus  

  // define togglebuttons
//    enum ToggleButtons  {plotRefSurfaceTB=0, plotSurfaceGridsTB, plotVolumeGridsTB };
  
  aString tbLabels[] = {"Ref. Surface", "Surface Grids", "Volume Grids", ""};
  aString tbCommands[] = {"plot reference surface",
			  "plot surface grids",
			  "plot volume grids",
			  ""};

  int tbState[] = {plotReferenceSurface,
		   plotSurfaceGrids,
		   plotVolumeGrids
		   };
    

  interface.setToggleButtons(tbCommands, tbLabels, tbState, 1); // organize togglebuttons in 1 column
  // done defining toggle buttons

  // make info labels with the number of broken surfaces and the total number of surfaces
  int numberOfBroken=0;
  for (int s=0; s<model.numberOfSubSurfaces(); s++)
  {
    if (model[s].getClassName() == "TrimmedMapping")
    {
      TrimmedMapping &trim = (TrimmedMapping &) model[s];
      if (!trim.trimmingIsValid())
      {
	numberOfBroken++;
      }
    }
  }
  int nosLabel = interface.addInfoLabel(sPrintF(buf,"There are %i component surfaces in the model", 
						model.numberOfSubSurfaces()));
  int brokenLabel = interface.addInfoLabel(sPrintF(buf,"of which %i are broken trimmed surfaces", 
						   numberOfBroken));
  int oldNoS = model.numberOfSubSurfaces();
  int oldNoB = numberOfBroken;

// bring up the interface on the screen
  gi.pushGUI( interface );

  int pickList=0, j; 
  realArray xp(100,3);
  int nPickPoints = 0;

  gi.setDefaultPrompt("");
  int retCode;
  
  PullDownMenu &fileMenu = interface.getPulldownMenu(0); // the file menu is # 0
   
  // initially, only the New, Open and Exit menu items are active
  fileMenu.pbList[0].setSensitive(true); // New
  fileMenu.pbList[1].setSensitive(true); // Open
  fileMenu.pbList[2].setSensitive(false); // Save
  fileMenu.pbList[3].setSensitive(true); // Exit


//    interface.setSensitive(false, DialogData::pushButtonWidget, editPB);
//    interface.setSensitive(false, DialogData::pushButtonWidget, topologyPB);
//    interface.setSensitive(false, DialogData::pushButtonWidget, surfacePB);
//    interface.setSensitive(false, DialogData::pushButtonWidget, volumePB);

  interface.setSensitive(false, DialogData::pushButtonWidget, "Edit model...");
  interface.setSensitive(false, DialogData::pushButtonWidget, "Topology...");
  interface.setSensitive(false, DialogData::pushButtonWidget, "Surface Grids...");
  interface.setSensitive(false, DialogData::pushButtonWidget, "Volume Grids...");


  bool found, plotObject;
  ViewLocation loc;
  
  for(int it=0; ; it++)
  {
    // count the number of broken surfaces
    int numberOfBroken=0;
    for (int s=0; s<model.numberOfSubSurfaces(); s++)
    {
      if (model[s].getClassName() == "TrimmedMapping")
      {
	TrimmedMapping &trim = (TrimmedMapping &) model[s];
	if (!trim.trimmingIsValid())
	{
	  numberOfBroken++;
	}
      }
    }

    interface.setSensitive(model.numberOfSubSurfaces()>0, DialogData::pushButtonWidget,"Edit model..."); 
    // doing the topology requires all trimmed surfaces to be fixed
    interface.setSensitive(model.numberOfSubSurfaces()>0 && numberOfBroken==0, 
			   DialogData::pushButtonWidget,"Topology...");
    topologyAvailable = (model.getCompositeTopology() != NULL);
    interface.setSensitive(topologyAvailable, DialogData::pushButtonWidget,"Surface Grids...");
    madeSurfaceGrids = (sGrids.numberOfSubSurfaces() > 0);
    interface.setSensitive(madeSurfaceGrids, DialogData::pushButtonWidget, "Volume Grids...");
    fileMenu.pbList[2].setSensitive(model.numberOfSubSurfaces()>0); // Save (file menu button)

    // change the labels
    if (oldNoS != model.numberOfSubSurfaces())
    {
      interface.setInfoLabel(nosLabel, sPrintF(buf,"There are %i component surfaces in the model", 
					       model.numberOfSubSurfaces()));
      oldNoS = model.numberOfSubSurfaces();
    }
    if (oldNoB != numberOfBroken)
    {
      interface.setInfoLabel(brokenLabel,sPrintF(buf,"of which %i are broken trimmed surfaces", 
						 numberOfBroken));
      oldNoB = numberOfBroken;
    }
    
    plotObject=true;

    // plot the initial model before reading any commands
    if (!(it==0 && modelOpen))
      retCode = gi.getAnswer(answer, "");

    if (answer == "new/append model")
    {
      if( newModel(gi, mapInfo, model) )
      {
        // set the plot label
	gp.set(GI_PLOT_LABELS, TRUE);
	gi.setPlotTheAxes(true);
	
	gp.set(GI_TOP_LABEL, model.getName(Mapping::mappingName));
	gp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);
        //  	  editInterface.setTextLabel(0,  model.getName(Mapping::mappingName)); // set the name textlabel

	modelOpen = true;

        // set the colour of the sub-surfaces
	for( map=0; map<model.numberOfSubSurfaces(); map++ )
	  model.setColour(map,gi.getColourName(map));

      }
    }
    else if (answer == "open model")
    {
      HDF_DataBase db;
      gi.inputFileName(modelFileName, "", ".hdf");
      // Save names and directories (this is a database after all)
      //      db.setMode(GenericDataBase::noStreamMode); 

      if (modelFileName.length() > 0 && modelFileName != " " && db.mount(modelFileName,"R") == 0)
      {
        // Names and directories are saved (this is a database after all)
        //	db.setMode(GenericDataBase::noStreamMode);  BUT IT IS SOOOO SLOW

	CompositeSurface * subModel_=NULL;
        // To obtain the topology from the first model, we read it into the main model directly
        // For subsequent parts, we just add the sub surfaces
	if (model.numberOfSubSurfaces()>0)
	{
	  subModel_ = new CompositeSurface;
	  subModel_->incrementReferenceCount();
	
	  subModel_->get(db,"Rap model");          // get the model from data base
	}
	else
	{
	  model.get(db,"Rap model");
	}
	sGrids.get(db,"Rap surface grids"); // get the surface grids
	vGrids.get(db,"Rap volume grids");  // get the volume grids
	db.unmount();                       // close the data base

	modelOpen = true;

	if (subModel_)
	{
	  printf("Adding...\n");
	  int map;
	  for( map=0; map<subModel_->numberOfSubSurfaces(); map++ )
	  {
            // AP: An offset should be added to the surface ID
	    model.add((*subModel_)[map], subModel_->getSurfaceID(map) ); 
	    printf("%i,",map);
	    fflush(stdout);
	  }
	  printf("\n");

          // delete the subModel
	  if (subModel_->decrementReferenceCount() == 0)
	    delete subModel_;
	}

        // set the colour of the sub-surfaces
	for( map=0; map<model.numberOfSubSurfaces(); map++ )
	  model.setColour(map,gi.getColourName(map));

	gi.setPlotTheAxes(true);
	gp.set(GI_LABEL_GRIDS_AND_BOUNDARIES, TRUE);

	int numberOfMappings = mapInfo.mappingList.getLength();
	for (int s=0; s<numberOfMappings; s++)
	{
	  Mapping *mapPointer=mapInfo.mappingList[s].mapPointer;
	  printf("Mapping #%i, named `%s' is a `%s'\n", s, SC mapPointer->getName(Mapping::mappingName), 
		 SC mapPointer->getClassName());
	}
	
      }
      else
      {
	sPrintF(buf,"Could not open the data base `%s'", SC modelFileName);
	gi.createMessageDialog(buf, errorDialog);
      }
    }
    else if (model.numberOfSubSurfaces()>0 && answer == "save model")
    {
      HDF_DataBase db;
      gi.inputFileName(modelFileName, "", ".hdf");

      if (modelFileName.length() > 0 && modelFileName != " " && db.mount(modelFileName,"I") ==0 ) 
      {
        // Save names and directories (this is a database after all)
        //	db.setMode(GenericDataBase::noStreamMode);  TOO SLOW FOR AP

	model.put(db,"Rap model");          // put the model from data base
	sGrids.put(db,"Rap surface grids"); // put the surface grids
	vGrids.put(db,"Rap volume grids");  // put the volume grids
	db.unmount();                       // close the data base
      }
      else
      {
	sPrintF(buf,"Bad file name: `%s'", SC modelFileName);
	gi.createMessageDialog(buf, errorDialog);
      }
      
    }
    else if (answer == "simple geometry")
    {
      simpleGeometry(mapInfo, model, curveList, points);
    }
    else if (answer == "liner...")
    {
      NurbsMapping *curve = new NurbsMapping;
      curve->incrementReferenceCount();
      curve->setRangeDimension(2);

      SphereLoading sphereLoading;  // 
      
      linerGeometry(model, gi, points, sphereLoading);
    }
    else if (model.numberOfSubSurfaces()>0 && answer == "heal model")
    {
      editModel(mapInfo, model, deletedSurfaces, curveList, points);
    }
    else if (model.numberOfSubSurfaces()>0 && answer == "topology")
    {
      model.updateTopology();
      gi.erase(); // resets the globalbound
      // test
      topology_ = model.getCompositeTopology();
      topologyAvailable = (topology_ != NULL);
      printf("Topology %sassigned to model\n", (topology_)? "": "NOT ");
    }
//      else if (modelOpen && answer == "check model")
//      {
//        rapCheckModel(gi);
//        modelOK = true;
//      }
    else if (topologyAvailable && answer == "make surface grids")
    {
      rapSurfaceGrids(mapInfo, model, sGrids, surfaceParameters);
    }
    else if (madeSurfaceGrids && answer == "make volume grids")
    {
      rapVolumeGrids(mapInfo, model, sGrids, vGrids, volumeParameters);
    }
    //                           01234
    else if (answer.matches("check") )
    {
      int nUnCounted=0;
      // tmp check for uncounted references
      printf("The model has globalID = %i\n", model.getGlobalID());
      if (model.uncountedReferencesMayExist())
      {
	printf("Uncounted references may exist in model\n");
	nUnCounted++;
      }
      for (int qq=0; qq<model.numberOfSubSurfaces(); qq++)
      {
	printf("Surface #%i has globalID = %i\n", qq, model[qq].getGlobalID());
	if (model[qq].uncountedReferencesMayExist())
	{
	  printf("Uncounted references may exist in model[%i]\n", qq);
	  nUnCounted++;
	}
      }
      if (nUnCounted == 0)
	printf("There are no uncounted references in the model\n");
    }
    else if (answer(0,3) == "help")
    {
      aString topic;
      topic = answer(5,answer.length()-1);
      if (!gi.displayHelp(topic))
      {
	aString msg;
	sPrintF(msg,"Sorry, there is currently no help for `%s'", SC topic);
	gi.createMessageDialog(msg, informationDialog);
      }
      
    }
    else if( answer=="exit" )
    {
      break;
    }
    else if( interface.getToggleValue(answer,"plot reference surface",plotReferenceSurface) ){gi.erase();}//
    else if( interface.getToggleValue(answer,"plot surface grids",plotSurfaceGrids) ){gi.erase();}//
    else if( interface.getToggleValue(answer,"plot volume grids",plotVolumeGrids) ){gi.erase();}//
//      else if( answer.matches("plot reference surface") )
//      {
//        plotReferenceSurface=!plotReferenceSurface;
//        interface.setToggleState(plotRefSurfaceTB, plotReferenceSurface);
//        gi.erase(); // calls resetGlobalBounds
//      }
//      else if( answer.matches("plot surface grids") )
//      {
//        plotSurfaceGrids = !plotSurfaceGrids;
//        interface.setToggleState(plotSurfaceGridsTB, plotSurfaceGrids);
//        gi.erase(); // calls resetGlobalBounds
//      }
//      else if( answer.matches("plot volume grids") )
//      {
//        plotVolumeGrids = !plotVolumeGrids;
//        interface.setToggleState(plotVolumeGridsTB, plotVolumeGrids);
//        gi.erase(); // calls resetGlobalBounds
//      }

    if (plotObject)
    {
      // plot the model
      if (plotReferenceSurface)
	PlotIt::plot(gi, model, gp);
      // plot the surface grids
      if (plotSurfaceGrids)
	PlotIt::plot(gi, sGrids, surfaceParameters);
      // plot the surface grids
      if (plotVolumeGrids)
	PlotIt::plot(gi, vGrids, volumeParameters);
    }
  

  }

//    if (model.decrementReferenceCount()==0)
//      delete &model;
//    if (deletedSurfaces.decrementReferenceCount()==0)
//      delete &deletedSurfaces;
//    if (sGrids.decrementReferenceCount()==0)
//      delete &sGrids;
//    if (vGrids.decrementReferenceCount()==0)
//      delete &vGrids;
  

  gi.popGUI(); // cleanup



  return 0;
}

