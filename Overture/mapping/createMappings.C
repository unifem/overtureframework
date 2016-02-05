#include "HDF_DataBase.h"

//#include "GL_GraphicsInterface.h"
#include "GenericGraphicsInterface.h"
#include "MappingInformation.h"

#include "AirfoilMapping.h"
#include "AnnulusMapping.h"
#include "BoxMapping.h"
#include "CircleMapping.h"
#include "CompositeSurface.h"
#include "CrossSectionMapping.h"
#include "CylinderMapping.h"
#include "DataPointMapping.h"
#include "DepthMapping.h"
#include "FilletMapping.h"
#ifndef OV_BUILD_MAPPING_LIBRARY
#include "EllipticTransform.h"
#endif
#include "FilamentMapping.h"

#include "HyperbolicMapping.h"
#include "IntersectionMapping.h"
#include "JoinMapping.h"
#include "LineMapping.h"
#include "MatrixTransform.h"
#include "NormalMapping.h"
#include "NurbsMapping.h"
#include "PlaneMapping.h"
#include "PolynomialMapping.h"
#include "QuadraticMapping.h"
#include "ReductionMapping.h"
#include "RevolutionMapping.h"
#include "ReparameterizationTransform.h"
#include "RocketMapping.h"
#include "SmoothedPolygon.h"
#include "StretchedSquare.h"
#include "StretchTransform.h"
#include "SquareMapping.h"
#include "SphereMapping.h"
#include "StretchMapping.h"
#include "SplineMapping.h"
#include "SweepMapping.h"
#include "TFIMapping.h"
#include "TrimmedMapping.h"
#include "UnstructuredMapping.h"

#include "DataFormats.h"

#include "MappingsFromCAD.h"

#include "MappingBuilder.h"
#include "OffsetShell.h"

#include "UserDefinedMapping1.h"

#include "LoftedSurfaceMapping.h"

#include "MappingGeometry.h"

#include "TrimmedMappingBuilder.h"

//  int 
//  viewMappings( MappingInformation & mapInfo );

// int 
// readMappings( MappingInformation & mapInfo );


int
readMappingsFromAnOverlappingGridFile( MappingInformation & mapInfo, const aString & fileName=nullString );

//===============================================================================================
//  This routine creates Mappings interactively with a graphics interface
//===============================================================================================
int 
createMappings( MappingInformation & mapInfo )
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
    
  // --------------------------------------
  // ----- Create Mappings Main Menu ------
  // --------------------------------------

  real triangleResolutionFactor=1.;

  GUIState dialog;
  dialog.setWindowTitle("Create Mappings");
  dialog.setExitCommand("exit", "exit");

  // aString opCmd[] =   {"parameterize by chord length",
  //  		       "parameterize by index (uniform)",
  //  		       ""}; //
  // int parOption =  (parameterizationType==parameterizeByChordLength ? 0 : 
  //  		    parameterizationType==parameterizeByIndex ? 1 : 2);
  // dialog.addOptionMenu("1D Mappings:", opCmd,opCmd,parOption);


  aString cmds[] = {"1D Mappings...",
                    "2D Mappings...",
                    "3D Mappings...",
                    "transform Mappings...",
                    "builder...",
                    "view Mappings...",
                    "read from a file...",
                    "save to a file...",
                    "edit a mapping...",
                    "copy a mapping",
                    "delete a mapping",
                    "check mapping",
                    "plot mapping quality",
                    "help",
		    ""};

  int numberOfPushButtons=0;;  // number of entries in cmds
  while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  // aString tbCommands[] = {"plot control points",
  //                         "plot curve",
  //                         "plot sub curves",
  //                         "plot points on curves",
  //                         "use robust inverse",
  // 			  ""};
  // int tbState[10];
  // tbState[0] = plotControlPoints;
  // tbState[1] = plotCurve;
  // tbState[2] = plotSubCurves;
  // tbState[3] = plotPointsOnCurves;
  
  // int numColumns=3;
  // dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=5;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",Mapping::debug);  nt++; 
  textLabels[nt] = "triangulation factor:";  sPrintF(textStrings[nt],"%g",triangleResolutionFactor);  nt++; 
  // textLabels[nt] = "debug:";  sPrintF(textStrings[nt],"%i",debug);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  // *old* popup

  aString answer,answer2;
  aString menu[] = {
                    "!create mappings",
                    "help",
//                     ">1D Mappings",
//                     "line",
//                     "stretching function",
//                     "spline (1D)",
//                     "<>2D Mappings",
//                     "airfoil",
//                     "annulus",
//                     "circle or ellipse",
//                     "dataPointMapping",
//                     "filamentMapping",
//                     "line (2D)",
//                     "nurbs (curve)",
//                     "quadratic (curve)",
//                     "rectangle",
//                     "rocket (2D)",
//                     "polynomial",
//                     "smoothedPolygon",
//                     "spline",
//                     "tfi",
//                     "unstructured",
//                     "<>3D Mappings",
//                     "box",
//                     "cylinder",
//                     "circle or ellipse (3D)",
//                     "composite surface",
//                     "crossSection",
//                     "dataPointMapping",
//                     "line (3D)",
//                     "lofted surface",
//                     "nurbs (surface)",
//                     "plane or rhombus",
//                     "quadratic (surface)",
//                     "rocket (3D)",
//                     "sphere",
//                     "spline (3D)",
//                     "tfi",
//                     "unstructured",
//                     "<>transform",
//                     "body of revolution",
//                     "build trimmed mapping",
// #ifndef OV_BUILD_MAPPING_LIBRARY
//                     "elliptic",
// #endif
//                     "fillet",
//                     "depth mapping",
//                     "hyperbolic",
//                     "intersection",
//                     "join",
//                     "mapping from normals",
//                     "offset shell",
//                     "reparameterize",
//                     "reduce domain dimension",
//                     "rotate/scale/shift",
//                     "stretch coordinates",
//                     "sweep",
//                     "trimmed mapping",
//                    "<builder",
                    "user defined 1",
//                    ">change",
		    //  "change a mapping",
                    // "copy a mapping",
                    // "delete a mapping",
                    ">data base",
		    "open a data-base",
		    "get from the data-base",
		    "get all mappings from the data-base",
		    "put to the data-base",
		    "close the data-base",
                    // "<>read from file",
                    //   "read plot3d file",
                    //   "read iges file",
                    //   "read overlapping grid file",
                    //   "read grids from a show file",
		    //   "read ingrid style file",
		    //   "read ply polygonal file",
  		    //   "read stl file",
		    //   "read rap model",
                    // "<>save to a file",
                    //   "save plot3d file",
		    //   "save ingrid file",
                    // "<view mappings",
                    // "check mapping",
                    // "plot mapping quality",
                    // ">parameters",
		    //   "debug",
  		    //   "resolution factor for triangulations",
                    "<open graphics",
                    "get geometric properties",
		    "erase",
		    "exit this menu",
                    "" };

  aString help[] = {
                    "help                 : print this list",
                    "airfoil              : ",
                    "annulus              : ",
                    "body of revolution   : revolve a mapping about a line",
                    "box                  : ",
                    "circle or ellipse    : ",
                    "composite surface    : create a surface composed of separate mappings",
                    "crossSection         : create an ellipse, or banana or wing...",
                    "cylinder             : ",
                    "dataPointMapping     : grid defined from data points",
                    "depth mapping        : Add a depth to a 2D Mapping",
#ifndef OV_BUILD_MAPPING_LIBRARY
                    "elliptic             : smooth a grid with an elliptic transformation",
#endif
                    "filamentMapping      : centerline + a bodyfitted grid",
                    "fillet               : create a smooth grid where two curves intersect",
                    "line                 : ",
                    "lofted surface       : create lofted surfaces such as wings and blades",
                    "normal mapping       : ",
                    "nurbs                : non-uniform rational B-spline",
                    "plane or rhombus     :",
                    "quadratic            : parabola or hyperbola (curve or surface)",
                    "reparameterize       : remove coordinate singularities or create a sub-patch",
                    "reduce domain dimension : define a new mapping that is a face or edge of another"
                    "rectangle            : ",
                    "rocket               : ",
                    "rotate/scale/shift   : ",
                    "smoothedPolygon      : ",
                    "sphere               : ",
                    "spline               : ",
                    "stretch coordinates  : ",
                    "sweep                : sweep a surface along a line",
                    "tfi                  : trans-finite interpolation (Coon's patch)",
                    "unstructured         : build a mapping for an unstructured grid",
                    " ",
                    "read plot3d file     : read mappings from a file in plot3d format",
                    "read iges file       : ",
                    "read overlapping grid file: import all mappings that are in an overlapping grid file",
                    "read grids from a show file: import all mappings that are in one grid found in a show file",
		    "read ingrid style file : read an unstructured mapping from an ingrid file",
		    "read ply polygonal file : read an unstructured mapping from a PLY format file",
                    "read rap model",
                    "save plot3d file     : save a mapping in plot3d format",
		    "save ingrid file     : save a mapping in ingrid format",
                    " ",
                    "change a mapping     : ",
                    "copy a mapping       : ",
                    "delete a mapping     : ",
                    " ",
                    "view mappings        : ",
                    "check mapping        : check mapping derivatives and inverse",
                    "plot mapping quality : plot mapping quality indicators",
                    "open graphics        : open a graphics window if it is not already open.",
                    "get geometric properties : determine properties such as the volume, surface area etc.",
		    "erase",
		    "exit this menu",
                    "" };

  dialog.buildPopup(menu);

  dialog.addInfoLabel("See popup menu for more options.");

  // --- Build the sibling dialog for 1D Mappings ---
  DialogData & mappings1D = dialog.getDialogSibling();
  mappings1D.setWindowTitle("1D Mappings");
  mappings1D.setExitCommand("close 1D Mappings", "close");
  if( true )
  {
    aString cmds[] = {"line",
		      "stretching function",
		      "spline (1D)",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    mappings1D.setPushButtons( cmds, cmds, numRows );
  }
  
  // --- Build the sibling dialog for 2D Mappings ---
  DialogData & mappings2D = dialog.getDialogSibling();
  mappings2D.setWindowTitle("2D Mappings");
  mappings2D.setExitCommand("close 2D Mappings", "close");
  if( true )
  {
    aString cmds[] = {"airfoil",
		      "annulus",
		      "circle or ellipse",
		      "dataPointMapping",
		      "filamentMapping",
		      "line (2D)",
		      "nurbs (curve)",
		      "quadratic (curve)",
		      "rectangle",
		      "rocket (2D)",
		      "polynomial",
		      "smoothedPolygon",
		      "spline",
		      "tfi",
		      "unstructured",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    mappings2D.setPushButtons( cmds, cmds, numRows );
  }
  
  // --- Build the sibling dialog for 3D Mappings ---
  DialogData & mappings3D = dialog.getDialogSibling();
  mappings3D.setWindowTitle("3D Mappings");
  mappings3D.setExitCommand("close 3D Mappings", "close");
  if( true )
  {
    aString cmds[] = {"box",
		      "cylinder",
		      "circle or ellipse (3D)",
		      "composite surface",
		      "crossSection",
		      "dataPointMapping",
		      "line (3D)",
		      "lofted surface",
		      "nurbs (surface)",
		      "plane or rhombus",
		      "quadratic (surface)",
		      "rocket (3D)",
		      "sphere",
		      "spline (3D)",
		      "tfi",
		      "unstructured",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    mappings3D.setPushButtons( cmds, cmds, numRows );
  }
  
  // --- Build the sibling dialog for Transform Mappings ---
  DialogData & mappingsTransform = dialog.getDialogSibling();
  mappingsTransform.setWindowTitle("Transform Mappings");
  mappingsTransform.setExitCommand("close Transform Mappings", "close");
  if( true )
  {
    aString cmds[] = {"body of revolution",
                    "build trimmed mapping",
                    #ifndef OV_BUILD_MAPPING_LIBRARY
                      "elliptic",
                    #endif
                    "fillet",
                    "depth mapping",
                    "hyperbolic",
                    "intersection",
                    "join",
                    "mapping from normals",
                    "offset shell",
                    "reparameterize",
                    "reduce domain dimension",
                    "rotate/scale/shift",
                    "stretch coordinates",
                    "sweep",
                    "trimmed mapping",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    mappingsTransform.setPushButtons( cmds, cmds, numRows );
  }

  // --- Build the sibling dialog for read from a file commands ---
  DialogData & readFromAFileDialog = dialog.getDialogSibling();
  readFromAFileDialog.setWindowTitle("Read Mappings From a File");
  readFromAFileDialog.setExitCommand("close Read Mappings From a File", "close");
  if( true )
  {
    aString cmds[] = {"read plot3d file",
                      "read iges file",
                      "read overlapping grid file",
                      "read grids from a show file",
		      "read ingrid style file",
		      "read ply polygonal file",
  		      "read stl file",
		      "read rap model",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    readFromAFileDialog.setPushButtons( cmds, cmds, numRows );
  }

  // --- Build the sibling dialog for saving Mappings to a file ---
  DialogData & saveToAFileDialog = dialog.getDialogSibling();
  saveToAFileDialog.setWindowTitle("Save Mappings To a File");
  saveToAFileDialog.setExitCommand("close Save Mappings To a File", "close");
  if( true )
  {
    aString cmds[] = {"save plot3d file",
		      "save ingrid file",
		      ""};

    numberOfPushButtons=0;;  // number of entries in cmds
    while( cmds[numberOfPushButtons] != "" ){ numberOfPushButtons++; } //  count number of commands
    numRows=(numberOfPushButtons+1)/2;
    saveToAFileDialog.setPushButtons( cmds, cmds, numRows );
  }

  gi.pushGUI(dialog);


  HDF_DataBase root;
  bool dataBaseIsOpen=FALSE;
  // ** don't do this here ** 980218 initializeMappingList();
  char buff[80];

  Mapping *mapPointer;  // pointer to Mapping that we are currently working on

  gi.appendToTheDefaultPrompt("create mappings>"); // set the default prompt

  for(;;)
  {
  
    gi.getAnswer(answer,"");

    // gi.getMenuItem(menu,answer);

    if( answer=="1D Mappings..." )
      mappings1D.showSibling();
    else if( answer=="close 1D Mappings" )
      mappings1D.hideSibling();

    else if( answer=="2D Mappings..." )
      mappings2D.showSibling();
    else if( answer=="close 2D Mappings" )
      mappings2D.hideSibling();

    else if( answer=="3D Mappings..." )
      mappings3D.showSibling();
    else if( answer=="close 3D Mappings" )
      mappings3D.hideSibling();

    else if( answer=="transform Mappings..." )
      mappingsTransform.showSibling();
    else if( answer=="close Transform Mappings" )
      mappingsTransform.hideSibling();

    else if( answer=="read from a file..." )
      readFromAFileDialog.showSibling();
    else if( answer=="close Read Mappings From a File" )
      readFromAFileDialog.hideSibling();

    else if( answer=="save to a file..." )
      saveToAFileDialog.showSibling();
    else if( answer=="close Save Mappings To a File" )
      saveToAFileDialog.hideSibling();


    else if( dialog.getTextValue(answer,"debug:","%i",Mapping::debug) ){}  //

    else if( answer=="Airfoil" || answer=="airfoil" )
    {
      // we need to increment and decrement the reference count so that the Mappings are deleted
      // properly. Note that the mappingList is a list of MappingRC -- the MappingRC constructor that
      // takes a Mapping is automatically called.
      mapPointer=new AirfoilMapping();              mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="annulus" || answer=="Annulus" )
    {
      mapPointer=new AnnulusMapping();              mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer); 
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="box" || answer=="Box" )
    {
      mapPointer=new BoxMapping();                  mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="circle or ellipse" || answer=="Circle or ellipse" )
    {
      mapPointer=new CircleMapping();               mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="circle or ellipse (3D)" || answer=="Circle or ellipse (3D)"  )
    {
      mapPointer=new CircleMapping();               mapPointer->incrementReferenceCount();
      mapPointer->setRangeDimension(3);
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="composite surface" || answer=="Composite surface" )
    {
      mapPointer=new CompositeSurface();            mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="crossSection" || answer=="CrossSection" )
    {
      mapPointer=new CrossSectionMapping();         mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="cylinder" || answer=="Cylinder" )
    {
      mapPointer=new CylinderMapping();             mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="dataPointMapping" || answer=="DataPointMapping" )
    {
      mapPointer=new DataPointMapping();            mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="depth mapping" || answer=="Depth mapping" )
    {
      mapPointer=new DepthMapping();                mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
     else if( answer=="filamentMapping" || answer=="FilamentMapping" )
     {
       mapPointer=new FilamentMapping();             mapPointer->incrementReferenceCount();
       mapInfo.mappingList.addElement(*mapPointer);
       mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
     }
    else if( answer=="fillet" || answer=="Fillet" )
    {
      mapPointer=new FilletMapping();               mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
#ifndef OV_BUILD_MAPPING_LIBRARY
    else if( answer=="elliptic" )
    {
      mapPointer=new EllipticTransform();           mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
#endif
    else if( answer=="hyperbolic" )
    {
      mapPointer=new HyperbolicMapping();           mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="intersection" )
    {
      mapPointer=new IntersectionMapping();         mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="join" )
    {
      mapPointer=new JoinMapping();                 mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="line" )
    {
      mapPointer=new LineMapping();                 mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="line (2D)" )
    {
      mapPointer=new LineMapping();                 mapPointer->incrementReferenceCount();  
      mapPointer->setRangeDimension(2);
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="line (3D)" )
    {
      mapPointer=new LineMapping();                 mapPointer->incrementReferenceCount();
      mapPointer->setRangeDimension(3);
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="nurbs (curve)" || answer=="nurbs")
    {
      mapPointer=new NurbsMapping(1,2);             mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="offset shell" )
    {
      OffsetShell offset;
      offset.createOffsetMappings( mapInfo );
    }
    else if( answer=="plane or rhombus" )
    {
      mapPointer=new PlaneMapping();                mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="polynomial" )
    {
      mapPointer=new PolynomialMapping();           mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="nurbs (surface)" )
    {
      mapPointer=new NurbsMapping(2,3);             mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="quadratic (curve)" || answer=="quadratic (surface)" )
    {
      mapPointer=new QuadraticMapping();            mapPointer->incrementReferenceCount();
      int rangeDimension= answer=="quadratic (curve)" ? 2 : 3;
      ((QuadraticMapping*)mapPointer)->chooseQuadratic(QuadraticMapping::parabola,rangeDimension);
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="rectangle" )
    {
      mapPointer=new SquareMapping();               mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="smoothedPolygon" || answer=="SmoothedPolygon"  )
    {
      mapPointer=new SmoothedPolygon();             mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer(0,5)=="spline" )
    {
      if( answer=="spline (1D)" )
        mapPointer=new SplineMapping(1);
      else if( answer=="spline (3D)" )
        mapPointer=new SplineMapping(3);
      else
        mapPointer=new SplineMapping(2);         
                                                    mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="tfi" )
    {
      mapPointer=new TFIMapping();                  mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="sphere" || answer=="Sphere" )
    {
      mapPointer=new SphereMapping();               mapPointer->incrementReferenceCount();
      mapInfo.mappingList.addElement(*mapPointer);
      mapPointer->update(mapInfo);                  mapPointer->decrementReferenceCount();
    }
    else if( answer=="rotate/scale/shift" )
    {
      mapPointer=new MatrixTransform();             mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  // first update, then add
                                                    mapPointer->decrementReferenceCount();
    }
    else if( answer=="rocket (2D)" || answer=="rocket (3D)" )
    {
      if( answer=="rocket (2D)" )
	mapPointer=new RocketMapping(2);
      else
	mapPointer=new RocketMapping(3);

      mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  // first update, then add
                                                    mapPointer->decrementReferenceCount();
    }
    else if( answer=="stretch coordinates" )
    {
      mapPointer=new StretchTransform();            mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="stretching function" )
    {
      mapPointer=new StretchMapping();              mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="mapping from normals" )
    {
      mapPointer=new NormalMapping();               mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="body of revolution" )
    {
      mapPointer=new RevolutionMapping();           mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="reparameterize" )
    {
      mapPointer=new ReparameterizationTransform(); mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="reduce domain dimension" )
    {
      mapPointer=new ReductionMapping();            mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="sweep" )
    {
      mapPointer=new SweepMapping();              mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="trimmed mapping" )
    {
      mapPointer=new TrimmedMapping(); mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="unstructured" )
    {
      mapPointer=new UnstructuredMapping(); mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="user defined 1" )
    {
      mapPointer=new UserDefinedMapping1(); mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="lofted surface" )
    {
      mapPointer=new LoftedSurfaceMapping(); mapPointer->incrementReferenceCount();
      mapPointer->update(mapInfo);
      mapInfo.mappingList.addElement(*mapPointer);  mapPointer->decrementReferenceCount();
    }
    else if( answer=="builder..." ||
             answer=="builder" )
    {
      // build multiple hyperbolic Mappings on a composite surface.
      MappingBuilder builder;
      builder.build(mapInfo);
    }
    else if( answer=="build trimmed mapping" )
    {
      TrimmedMappingBuilder tmb;
      tmb.buildTrimmedMapping( mapInfo );
    }
    else if( answer.matches("get geometric properties") )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printF("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+2];
      int i=0;
      menu2[i++]="!geometric";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"Get properties for which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && map<num )
      {
        // Here is the mapping to be used
        mapPointer=mapInfo.mappingList[map].mapPointer;
        Mapping & map = *mapPointer;
	
        printF("use mapping %s \n",(const char *) map.getName(Mapping::mappingName));

        RealArray rvalues; 
        IntegerArray ivalues;
        MappingGeometry::getGeometricProperties( map,rvalues,ivalues );

        printF("**************************************************************\n"
               " Mapping: %s \n"
               " number of grid points=%i\n"
               " number of volumes=%i\n"
               " number of negative volumes=%i\n"
               " total volume=%8.2e, Cells: (min,ave,max)=(%8.2e,%8.2e,%8.2e) \n"
               " Centre of mass = (%10.4e,%10.4e,%10.4e)\n"
               "**************************************************************\n",
	       (const char*)map.getName(Mapping::mappingName),
               ivalues(0),ivalues(1),ivalues(2),
               rvalues(0),rvalues(1),rvalues(2),rvalues(3),
	       rvalues(4),rvalues(5),rvalues(6));
//     rvalues( 7)=momentOfInertial(0,0);
//     rvalues( 8)=momentOfInertial(0,1);
//     rvalues( 9)=momentOfInertial(0,2);
//     rvalues(10)=momentOfInertial(1,0);
//     rvalues(11)=momentOfInertial(1,1);
//     rvalues(12)=momentOfInertial(1,2);
//     rvalues(13)=momentOfInertial(2,0);
//     rvalues(14)=momentOfInertial(2,1);
//     rvalues(15)=momentOfInertial(2,2);
    
      }
      else
        gi.outputString("Error, unknown mapping!");

    }
    else if( answer.matches("edit a mapping...") ||
             answer.matches("change a mapping") )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printF("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+2];
      int i=0;
      menu2[i++]="!change a mapping";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"change which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && map<num )
      {
        // Here is the mapping to be changed
        mapPointer=mapInfo.mappingList[map].mapPointer;
        printF("edit mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));
        mapPointer->update(mapInfo);
      }
      else
        gi.outputString("Error, unknown mapping!");
    }
    else if( answer=="copy a mapping" )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printF("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+3];
      int i=0;
      menu2[i++]="!copy a mapping";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"copy which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && answer2!="none" )
      {
	// Here is the mapping to be rotated/scaled etc.
	mapPointer=mapInfo.mappingList[map].mapPointer;
        printF("copy and edit mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));
	
	Mapping *newMapping;
	newMapping=mapPointer->make(mapPointer->getClassName());  // make this mapping type
	(ReferenceCounting&)*newMapping=(ReferenceCounting&)*mapPointer;  // deep copy, call virtual =
	mapInfo.mappingList.addElement(*newMapping); 
	newMapping->update(mapInfo);
      }
      else if( map<0 )
      {
        gi.outputString("Error: unknown mapping name!");
        gi.stopReadingCommandFile();
      }
    }
    else if( answer=="delete a mapping" )
    {
      int num=mapInfo.mappingList.getLength();
      if( num<=0 )
      {
	printF("There are no mappings available! \n");
	continue;
      }

      aString *menu2 = new aString[num+3];
      int i=0;
      menu2[i++]="!delete a mapping";
      int mappingListStart=i;
      for( int j=0; j<num; j++ )
	menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
      int mappingListEnd=i-1;
      menu2[i++]="none"; 
      menu2[i]="";   // null string terminates the menu

      gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

      int map = gi.getMenuItem(menu2,answer2,"delete which mapping?");
      delete [] menu2;

      gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

      if( map>=0 && answer2!="none" )
      {
	mapPointer=mapInfo.mappingList[map].mapPointer;
        printF("delete mapping %s \n",(const char *) mapPointer->getName(Mapping::mappingName));
        mapInfo.mappingList.deleteElement(map);  // delete the mapping
      }
      else if( map<0 )
      {
        gi.outputString("Error: unknown mapping name!");
        gi.stopReadingCommandFile();
      }
    }
    else if( answer=="open a data-base" )
    {
      if( dataBaseIsOpen )
      {
	gi.outputString("closing the data base that is already open");
        root.unmount();
        dataBaseIsOpen=FALSE;
      }
      aString fileName;
      gi.inputString(fileName,"Enter the file name");
      aString menu2[] = {"open a new file",
                        "open an old file read-write",
                        "open an old file read-only",
                        "exit this menu", 
                        "" };
      int choice = gi.getMenuItem(menu2,answer2,"choose file option");
      dataBaseIsOpen=TRUE;
      if( choice==0 )
        root.mount(fileName,"I");     // mount a new file (I=Initialize)
      else if( choice==1 )
      {
// check if the file exists
        if ( (root.mount(fileName,"W")==-1) )
	{
	  gi.outputString("The data base file couldn't be opened");    
	  dataBaseIsOpen=FALSE;
	}
      }
      else if( choice==2 )
      {
// check if the file exists
        if ( (root.mount(fileName,"R")==-1) )
	{
	  gi.outputString("The data base file couldn't be opened");    
	  dataBaseIsOpen=FALSE;
	}
      }
      else
        dataBaseIsOpen=FALSE;
    }
    else if( answer=="get from the data-base" )
    {
      if( dataBaseIsOpen )
      {
        int maxNumber=50;  // first guess
	int num, actualNumber;	
	aString *mapNames = NULL;
        int i;
        for( i=0; i<=1; i++ )
	{
          mapNames=new aString [maxNumber];  

  	  num = root.find(mapNames,"Mapping",50,actualNumber);
          if( actualNumber>num )
	  { // try again with more elements in mapNames[]
	    maxNumber=actualNumber+1;
	    delete [] mapNames;
	  }
          else
            break;
	}
	printF("Number of mappings found=%i\n",num);

        aString *menu2 = new aString[num+3];
	i=0;
	menu2[i++]="!get from a data-base";
	int mappingListStart=i;
	for( int j=0; j<num; j++ )
	  menu2[i++]=mapNames[j];
	int mappingListEnd=i-1;
	menu2[i++]="none"; 
	menu2[i]="";   // null string terminates the menu

	gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

	int map = gi.getMenuItem(menu2,answer2,"get which mapping?");
	delete [] menu2;

	gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

	if( map>=0 && answer2!="none" )
	{
	  MappingRC *newMapping= new MappingRC();
	  newMapping->get(root,mapNames[map]);  // this will "make" the correct type of Mapping
	  mapInfo.mappingList.addElement(*newMapping); 
	}
	else if( map<0 )
	  gi.outputString("Error: unknown mapping name!");

        delete [] mapNames;
      }
      else
	gi.outputString("You must open a data-base first");
    }
    else if( answer=="get all mappings from the data-base" )
    {
      if( dataBaseIsOpen )
      {
        real time0=getCPU();
	aString mapNames[50];  // **** fix this *****
	int num, actualNumber;
	num = root.find(mapNames,"Mapping",50,actualNumber);
	printF("Number of mappings found=%i\n",num);

	for( int i=0; i<num; i++ )
	{
	  MappingRC *newMapping= new MappingRC();
	  newMapping->get(root,mapNames[i]);  // this will "make" the correct type of Mapping
	  mapInfo.mappingList.addElement(*newMapping); 
	}

	printF("Time to read mappings = %8.2e\n",getCPU()-time0);
      }
      else
	gi.outputString("You must open a data-base first");
    }
    else if( answer=="put to the data-base" )
    {
      if( dataBaseIsOpen )
      {
	int num=mapInfo.mappingList.getLength();
        aString *menu2 = new aString[num+3];
	int i=0;
	menu2[i++]="!put to the data-base";
	int mappingListStart=i;
	for( int j=0; j<num; j++ )
	  menu2[i++]=mapInfo.mappingList[j].getName(Mapping::mappingName);
	int mappingListEnd=i-1;
	menu2[i++]="none"; 
	menu2[i]="";   // null string terminates the menu

	gi.buildCascadingMenu( menu2,mappingListStart,mappingListEnd );

	int map = gi.getMenuItem(menu2,answer2,"put which mapping?");
	delete [] menu2;

	gi.indexInCascadingMenu( map,mappingListStart,mappingListEnd);

	if( map>=0 && answer2!="none" )
	{
	  MappingRC & mapping = mapInfo.mappingList[map];
	  mapping.put(root,mapping.getName(Mapping::mappingName));  // put the mapping
	}
	else if( map<0 )
	  gi.outputString("Error: unknown mapping name!");
      }
      else
	gi.outputString("You must open a data-base first");
    }
    else if( answer=="close the data-base" )
    {
      if( dataBaseIsOpen )
      {
	root.unmount();
	dataBaseIsOpen=FALSE;
      }
      else
	gi.outputString("There is no data-base open");
    }
    else if( answer=="save plot3d file" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      for( int i=0; i<num; i++ )
	menu2[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      menu2[num]="none"; 
      menu2[num+1]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2,"save which mapping?");
      delete [] menu2;
      if( mapNumber>=0 && answer2!="none" )
      {
	Mapping & map = mapInfo.mappingList[mapNumber].getMapping();

	DataFormats::writePlot3d(map);
      }
    }
    else if( answer=="save ingrid file" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      for( int i=0; i<num; i++ )
	menu2[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      menu2[num]="none"; 
      menu2[num+1]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2,"save which mapping?");
      delete [] menu2;
      if( mapNumber>=0 && answer2!="none" )
      {
	Mapping & map = mapInfo.mappingList[mapNumber].getMapping();

	DataFormats::writeIngrid(map);
      }
    }
    else if( answer=="read iges file" )
    {
      MappingsFromCAD mapCad;
      mapCad.readMappings(mapInfo);
    }
    else if( answer=="read overlapping grid file" ||
             answer=="read grids from a show file" )
    {
      readMappingsFromAnOverlappingGridFile(mapInfo);
    }
    else if( answer=="read plot3d file" )
    {
      intArray *imask=0;
      DataFormats::readPlot3d(mapInfo,"",imask);
      if (imask) delete [] imask;
    }
    else if( answer=="read ingrid style file" )
    {
      UnstructuredMapping *umap = new UnstructuredMapping();
      DataFormats::readIngrid(*((UnstructuredMapping *)umap));
      mapInfo.mappingList.addElement(*umap);

    }
    else if( answer== "read ply polygonal file")
    {
      UnstructuredMapping *umap = new UnstructuredMapping();

      DataFormats::readPly(*((UnstructuredMapping *)umap));
      mapInfo.mappingList.addElement(*umap);

    }
    else if( answer== "read stl file")
    {
      UnstructuredMapping *umap = new UnstructuredMapping();

      DataFormats::readSTL(*((UnstructuredMapping *)umap));
      mapInfo.mappingList.addElement(*umap);

    }
    else if( answer=="read rap model" )
    {
      HDF_DataBase db;
      aString modelFileName;
      gi.inputFileName(modelFileName, "", ".hdf");
      if (modelFileName.length() > 0 && modelFileName != " " && db.mount(modelFileName,"R") == 0)
      {
        CompositeSurface & model = *new CompositeSurface();  model.incrementReferenceCount();
	model.get(db,"Rap model");          // get the model from data base

	CompositeSurface sGrids;
	CompositeSurface vGrids;
	sGrids.get(db,"Rap surface grids"); // get the surface grids
	vGrids.get(db,"Rap volume grids"); // get the surface grids
	db.unmount();                       // close the data base
        mapInfo.mappingList.addElement(model);

	for ( int s=0; s<sGrids.numberOfSubSurfaces(); s++ )
	  mapInfo.mappingList.addElement(sGrids[s]);

	for ( int v=0; v<sGrids.numberOfSubSurfaces(); v++ )
	  mapInfo.mappingList.addElement(vGrids[v]);

	model.decrementReferenceCount();
	
      }
    }
    else if( answer=="view Mappings..." || 
             answer=="view mappings" )
    {
      viewMappings(mapInfo);
    }
    else if( answer=="check mapping" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      menu2[num]="none"; 
      menu2[num+1]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2,"check which mapping?");
      delete [] menu2;
      if( mapNumber<0 )
      {
	printF("unknown mapping to check!\n");
      }
      else if( answer2!="none" )
      {
         mapInfo.mappingList[mapNumber].checkMapping();  // check the mapping and derivatives
      }
    }
    else if( answer=="plot mapping quality" )
    {
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      for( int i=0; i<num; i++ )
        menu2[i]=mapInfo.mappingList[i].getName(Mapping::mappingName);
      menu2[num]="none"; 
      menu2[num+1]="";   // null string terminates the menu
      int mapNumber = gi.getMenuItem(menu2,answer2,"check which mapping?");
      delete [] menu2;
      if( mapNumber<0 )
      {
	printF("unknown mapping to plot quality!\n");
      }
      else if( answer2!="none" )
      {
        PlotIt::plotMappingQuality(gi,mapInfo.mappingList[mapNumber].getMapping());
      }
    }
    else if( answer=="debug" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the debug bit flag (current value=%i)",Mapping::debug));
      if( answer2!="" )
        sScanF(answer2,"%i ",&Mapping::debug);
    }
    else if( dialog.getTextValue(answer,"triangulation factor:","%e",triangleResolutionFactor) )
    {
      printF("Setting the resolution factor for triangulations of Trimmed Mappings to %9.2e (used when plotting)\n"
             "   The resolution factor should be greater than zero and less than 100 \n"
             "   1=use default, 2=use a coarser triangulation, .5= use a finer triangulation\n",
             triangleResolutionFactor );

      if( triangleResolutionFactor<0. || triangleResolutionFactor>100. )
      {
	printF("ERROR: triangleResolutionFactor=%e should be larger than zero and less than 100\n");
      }
      else
      {
        // adjust the minimum angle for triangulations -- the smaller this value the fewer triangles needed
	TrimmedMapping::defaultMinAngleForTriangulation=min(20.,20./triangleResolutionFactor);
        // adjust the elementDensityTolerance, num-grid pts is related to curvature/elementDensityTolerance
	TrimmedMapping::defaultElementDensityToleranceForTriangulation= .05*triangleResolutionFactor;
        printF(" *** Now using minAngleForTriangulation=%8.2e and elementDensityTolerance=%8.2e\n",
	       TrimmedMapping::defaultMinAngleForTriangulation,
               TrimmedMapping::defaultElementDensityToleranceForTriangulation);
	
      }
    }

    else if( answer=="resolution factor for triangulations" ) // old way 
    {
      printF("Enter the resolution factor for triangulations of Trimmed Mappings (used when plotting)\n"
             "   The resolution factor should be greater than zero and less than 100 \n"
             "   1=use default, 2=use a coarser triangulation, .5= use a finer triangulation\n");
      gi.inputString(answer2,sPrintF(buff,"Enter the triangle resolution factor (default=1.)",
                   triangleResolutionFactor));

      sScanF(answer2,"%e",&triangleResolutionFactor);

      if( triangleResolutionFactor<0. || triangleResolutionFactor>100. )
      {
	printF("ERROR: triangleResolutionFactor=%e should be larger than zero and less than 100\n");
      }
      else
      {
        // adjust the minimum angle for triangulations -- the smaller this value the fewer triangles needed
	TrimmedMapping::defaultMinAngleForTriangulation=min(20.,20./triangleResolutionFactor);
        // adjust the elementDensityTolerance, num-grid pts is related to curvature/elementDensityTolerance
	TrimmedMapping::defaultElementDensityToleranceForTriangulation= .05*triangleResolutionFactor;
        printF(" *** Now using minAngleForTriangulation=%8.2e and elementDensityTolerance=%8.2e\n",
	       TrimmedMapping::defaultMinAngleForTriangulation,
               TrimmedMapping::defaultElementDensityToleranceForTriangulation);
	
      }
      
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="open graphics" )
    {
      if( !gi.graphicsIsOn() )
        gi.createWindow("PlotStuff");
    }
    else if( answer=="erase" )
    {
      gi.erase();
    }
    else if( answer=="exit" ||
             answer=="exit this menu" )
    {
      break;
    }
    else if( answer=="create mappings" )
    {
      // this command will do nothing but lets command files for ogen to be used with pm
    }
    else
    {
      printF("Unknown response: [%s]\n",(const char*) answer);
      gi.stopReadingCommandFile();
    }

  }
  if( dataBaseIsOpen )
    root.unmount();
  
  gi.unAppendTheDefaultPrompt();  // reset prompt

  gi.popGUI(); // restore the previous GUI

  return 0;
}

