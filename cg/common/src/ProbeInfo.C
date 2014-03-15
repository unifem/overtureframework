#include "ProbeInfo.h"
#include "GenericGraphicsInterface.h"
#include "HDF_DataBase.h"
#include "Parameters.h"
#include "Parameters.h"
#include "BodyForce.h"
#include "Integrate.h"
#include "InterpolatePointsOnAGrid.h"

// ==========================================================================================
/// \brief : define a probe. A probe is a point or location in a grid where the
///    solution is periodically saved to a file. A probe may also save the solution on a 
///   bounding-box. This bounding box info may be used on subsequent computations as 
///   boundary conditions for a computation on a smaller sub-domain.
/// 
///  \details The file generated with the probe results can be read by the matlab script
///  plotProbes.m 
// ==========================================================================================
ProbeInfo::
ProbeInfo(Parameters & par) : parameters(par) 
{
  probeType=probeAtGridPoint;
  if (!dbase.has_key("probeName")) 
    dbase.put<aString>("probeName");

  dbase.get<aString>("probeName")="probeDefaultName";


  file=NULL;
  fileName="probeFile.dat";  
  pdb=NULL;
  
  grid=0;
  iv[0]=iv[1]=iv[2]=0;

  xv[0]=xv[1]=xv[2]=0.;

  #define bb(side,axis) boundingBox[(side)+2*(axis)]
  boundingBoxGrid=0;
  for( int axis=0; axis<3; axis++ ) 
  {
    bb(0,axis)=0;
    bb(1,axis)=0;
  }
  
  numberOfLayers=2;
  numberOfTimes=0;
  times=NULL;
}

// ==========================================================================================
/// \brief : destructor.
// ==========================================================================================
ProbeInfo::
~ProbeInfo()
{
  if( file!=NULL )
  {
    fclose(file);
  }
  // close the data base file for the bounding box info
  if( pdb!=NULL )
  {
    printF("++++ ProbeInfo: close the bounding box probe file +++\n");
    // save the time history:
    pdb->put(numberOfTimes,"numberOfTimes");
    if( numberOfTimes>0 )
    {
      assert( times!=NULL );
      pdb->put((*times)(Range(numberOfTimes)),"times");
    }
    delete times;
    pdb->unmount();
    delete pdb;

    HDF_DataBase db;
    db.mount("cylProbeBox.hdf","R");
    int nt=-1;
    db.get(nt,"numberOfTimes");
    printF("Re-mount the probe file, numberOfTimes=%i\n",nt);
    

  }
  
}

// ===========================================================================================
/// \brief Compute the closest grid point to each of a set of points in space.
/// \details : this function is used for examples to compute the closest grid points
///    for probes. 
/// \param points(0:numPoints,0:d-1) (input) : coordinates of points to check.
/// \param gridLocation(0:numPoints,0:3) (output) : (i1,i2,i3,grid) closest grid and point. 
/// 
// ===========================================================================================
//#include "InterpolatePoints.h"
#include "UnstructuredMapping.h"
int
getClosestGridPoint( CompositeGrid & cg, 
                     RealArray & points, 
                     IntegerArray & gridLocation )
{
  
  int numberOfPoints=points.getLength(0);

  gridLocation.redim(numberOfPoints,4);

  IntegerArray indexValues, interpoleeGrid;

  if( false )
  {
    // ***fix**** this requires the center array I think
    //InterpolatePoints interp;
    //interp.buildInterpolationInfo(positionToInterpolate,cg );
    //interp.getInterpolationInfo(cg, indexValues, interpoleeGrid);
  }
  else
  {
    // locate the nearest grid point
          
    const int numberOfDimensions = cg.numberOfDimensions();
    Range I=numberOfPoints;
    RealArray r(I,numberOfDimensions);
    RealArray x(I,numberOfDimensions);
    for( int i=0; i<numberOfPoints; i++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	x(i,axis)=points(i,axis);
      }
    }
	  
    indexValues.redim(I,numberOfDimensions);
    interpoleeGrid.redim(I);
    interpoleeGrid=-1;

    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
    int numFound=0;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      Mapping & map = mg.mapping().getMapping();
      const intArray & mask = mg.mask();
      i3=mg.gridIndexRange(0,2);

      if ( mg.getGridType()==MappedGrid::structuredGrid )
      {
	r=-1.;
        #ifdef USE_PPP
	  map.inverseMapS(x,r);
        #else
	  map.inverseMap(x,r);
        #endif 
	for( int i=0; i<numberOfPoints; i++ )
	{
	  if( interpoleeGrid(i)<0 ) // this point not yet found
	  {
	    bool ok=true;
	    for( int axis=0; axis<numberOfDimensions; axis++ )
	    {
	      // closest point:
	      iv[axis] = int( r(i,axis)/mg.gridSpacing(axis)+ mg.gridIndexRange(0,axis) +.5);
	      if( iv[axis]< mg.gridIndexRange(0,axis) ||
		  iv[axis]> mg.gridIndexRange(1,axis) )
	      {
		ok=false;
		break;
	      }
	    }
#ifndef USE_PPP
	    if( ok && mask(i1,i2,i3)>0 ) // *** fix this -- P++ problem --
#else
	      if( ok ) 
#endif
	      {
		for( int axis=0; axis<numberOfDimensions; axis++ )
		  indexValues(i,axis)=iv[axis];
		interpoleeGrid(i)=grid;
		numFound++;
	      }
	  }
		
	} // end for i
      }
      else
      {
        // --- Unstructured grid ---
	for( int i=0; i<numberOfPoints; i++ )
	{
	  real xx[3];
	  xx[2] = 0.;
	  for ( int a=0; a<mg.numberOfDimensions(); a++ )
	    xx[a] = x(i,a);

	  UnstructuredMapping & umap = (UnstructuredMapping &)mg.mapping().getMapping();
		    
	  int ent = umap.findClosestEntity(UnstructuredMapping::Face, xx[0],xx[1],xx[2]);
	  assert(ent!=-1);

	  interpoleeGrid(i) = grid;
	  indexValues(i,0) = ent;
	  for ( int a=1; a<mg.numberOfDimensions(); a++ )
	    indexValues(i,a) = 0;
	  numFound++;
	}
      }

      if( numFound==numberOfPoints ) break;
    } // end for grid
  }
	
	
  for( int i=0,j=0; i<numberOfPoints; i++ )
  {
    gridLocation(i,0)=indexValues(i,0);
    gridLocation(i,1)=indexValues(i,1);
    if( cg.numberOfDimensions()==3 )
      gridLocation(i,2)=indexValues(i,2);
    else
      gridLocation(i,2)=0;
	  
    gridLocation(i,3)=interpoleeGrid(i);
    if( interpoleeGrid(i)<0 )
    {
      printF("getClosestGridPoint: error location points %i: x=(%9.3e,%9.3e,%9.3e)\n",i,
	     points(i,0),points(i,1),points(i,2));
      Overture::abort();
    }

    printF("getClosestGridPoint: i=%i: x=(%9.3e,%9.3e,%9.3e), closest grid=%i, pt i=(%i,%i,%i)\n",i,
	   points(i,0),points(i,1),points(i,2),
	   gridLocation(i,3),gridLocation(i,0),gridLocation(i,1),gridLocation(i,2)   );
  }
  
  return 0;
}



// ===================================================================================================================
/// \brief Build the region options dialog.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int ProbeInfo::
buildRegionOptionsDialog(DialogData & dialog )
{

  dialog.setOptionMenuColumns(1);

  // -- here is the list of region types that we know about --
  // TODO: add ellipse, cylinder, sphere, unstructured
  int regionTypeOption=0;  
  aString regionCommands[] = { "box region",
                               "full domain region",
                               "boundary region",
                               "box boundary region",
				 "" };
  dialog.addOptionMenu("region:",regionCommands,regionCommands,regionTypeOption );



//   aString tbCommands[] = {"save a restart file",
// 			  "allow user defined output",
// 			  ""};
//   int tbState[10];
//   tbState[0] = parameters.dbase.get<bool >("saveRestartFile"); 
//   tbState[1] = parameters.dbase.get<int >("allowUserDefinedOutput"); 
//   int numColumns=1;
//   dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


//   aString pbCommands[] = {"show file options...",
//                           "create a probe",
//                           "output periodically to a file",
// 			  ""};
//   aString *pbLabels = pbCommands;
//   int numRows=2;
//   dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "body forcing region:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "none"); nt++;

  textCommands[nt] = "boundary forcing region:";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "none"); nt++;

//   textCommands[nt] = "box:";       
//   sPrintF(textStrings[nt],"%g,%g, %g,%g, %g,%g (xa,xb, ya,yb, za,zb)",xa,xb,ya,yb,za,zb);  nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

//================================================================================
/// \brief: Look for an region option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int ProbeInfo::
getRegionOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  aString bodyName;
  


  if( answer=="box region"         ||
      answer=="full domain region" ||
      answer=="boundary region"    ||
      answer=="box boundary region" )
  {
    // Choose a region type 

    if (!dbase.has_key("regionParameters")) 
      dbase.put<BodyForceRegionParameters>("regionParameters");
    BodyForceRegionParameters & regionPar = dbase.get<BodyForceRegionParameters>("regionParameters");
       

    aString & regionType = regionPar.dbase.get<aString>("regionType");
    if( answer=="box region" )
    {
      regionType="box";
      probeType=probeRegion;
    }
    else if( answer=="full domain region" )
    {
      regionType="fullDomain";
      probeType=probeRegion;
    }
    else if( answer=="boundary region" )
    {
      regionType="boundaryRegion";
      probeType=probeRegion;
    }
    else if( answer=="box boundary region" )
    {
      regionType="boxBoundaryRegion";
      probeType=probeRegion;
    }
    else
    {
      printF("ProbeInfo:ERROR: unexpected regionType=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    dialog.getOptionMenu("region:").setCurrentChoice(answer);
  }
  else if( dialog.getTextValue(answer,"body forcing region:","%s",bodyName) )
  {

    if( !parameters.dbase.get<bool>("turnOnBodyForcing") )
    {
      printF("WARNING: There are currently no body force regions.");
    }
    else
    {
      // Here is the array of boundary forcings:
      std::vector<BodyForce*> & bodyForcings =  parameters.dbase.get<std::vector<BodyForce*> >("bodyForcings");

      printF("--Define a probe from an existing body region\n"
	     "  There are %i body forcing regions defined.\n",bodyForcings.size());
	
      int b=-1;
      
      for( int bf=0; bf<bodyForcings.size(); bf++ )
      {
        const BodyForce & bodyForce = *bodyForcings[bf];
	if( bodyName==bodyForce.dbase.get<aString>("bodyForcingName") )
	{
	  b=bf;
	  break;
	}
      }
      if( b==-1 )
      {
	printF("ERROR: body forcing named [%s] not found!\n"
               "Here are the existing body forcing regions:\n",(const char*)bodyName);
	for( int bf=0; bf<bodyForcings.size(); bf++ )
	{
	  const BodyForce & bodyForce = *bodyForcings[bf];
	  const aString & forcingType = bodyForce.dbase.get<aString >("forcingType");
	  const aString & regionType = bodyForce.dbase.get<aString>("regionType");
	  const aString & bodyForcingName = bodyForce.dbase.get<aString>("bodyForcingName");
	  printF("Body forcing %i: name=[%s] type=[%s] region=[%s]\n",bf,(const char*)bodyForcingName,
		 (const char*)forcingType,  (const char*)regionType);
	}
      }
      else
      {
	printF(" Body forcing region [%s] found! Creating a probe of type=probeRegion.\n",(const char*)bodyName);

        probeType=probeRegion;

        // --- save the info about the region associated with this body forcing ----
        const BodyForce & bodyForce = *bodyForcings[b];
	if (!dbase.has_key("regionParameters")) 
	  dbase.put<BodyForceRegionParameters>("regionParameters");
        BodyForceRegionParameters & regionPar = dbase.get<BodyForceRegionParameters>("regionParameters");
       

	aString & regionType = regionPar.dbase.get<aString>("regionType");

	// Save the region type:
	regionType = bodyForce.dbase.get<aString>("regionType");  // region type

	if( regionType=="box" )
	{
	  const real *boxBounds =  bodyForce.dbase.get<real[6]>("boxBounds");
	  real *bpar = regionPar.dbase.get<real[6]>("boxBounds");
	  for( int i=0; i<6; i++ )
	    bpar[i]=boxBounds[i];
	}
	else if( regionType=="ellipse" )
	{
	  const real *ellipse    =  bodyForce.dbase.get<real[6]>("ellipse");
	  real *epar =  regionPar.dbase.get<real[6]>("ellipse");
	  for( int i=0; i<6; i++ )
	    epar[i]=ellipse[i];
	}
	else
	{
	  printF("ProbeInfo: ERROR: unexpected regionType=%s\n",(const char*)regionType);
	  OV_ABORT("ERROR: finish me...");
	}
        

      }

    }
      

  }
  else if( dialog.getTextValue(answer,"boundary forcing region:","%s",bodyName) )
  {

    if( !parameters.dbase.get<bool>("turnOnBoundaryForcing") )
    {
      printF("WARNING: There are currently no boundary force regions.");
    }
    else
    {
      // Here is the array of boundary forcings:
      std::vector<BodyForce*> & bodyForcings =  parameters.dbase.get<std::vector<BodyForce*> >("boundaryForcings");

      printF("--Define a probe from an existing boundary forcing region\n"
	     "  There are %i boundary forcing regions defined.\n",bodyForcings.size());
	
      int b=-1;
      
      for( int bf=0; bf<bodyForcings.size(); bf++ )
      {
        const BodyForce & bodyForce = *bodyForcings[bf];
	if( bodyName==bodyForce.dbase.get<aString>("bodyForcingName") )
	{
	  b=bf;
	  break;
	}
      }
      if( b==-1 )
      {
	printF("ERROR: boundary forcing named [%s] not found!\n"
               "Here are the existing boundary forcing regions:\n",(const char*)bodyName);
	for( int bf=0; bf<bodyForcings.size(); bf++ )
	{
	  const BodyForce & bodyForce = *bodyForcings[bf];
	  const aString & forcingType = bodyForce.dbase.get<aString >("forcingType");
	  const aString & regionType = bodyForce.dbase.get<aString>("regionType");
	  const aString & bodyForcingName = bodyForce.dbase.get<aString>("bodyForcingName");
	  printF("Boundary forcing %i: name=[%s] type=[%s] region=[%s]\n",bf,(const char*)bodyForcingName,
		 (const char*)forcingType,  (const char*)regionType);
	}
      }
      else
      {
	printF(" Boundary forcing region [%s] found! Creating of probe of type=probeRegion.\n",(const char*)bodyName);

        probeType=probeRegion;

        // --- save the info about the region associated with this body forcing ----
        const BodyForce & bodyForce = *bodyForcings[b];
	if( !dbase.has_key("regionParameters") ) 
	  dbase.put<BodyForceRegionParameters>("regionParameters");
        BodyForceRegionParameters & regionPar = dbase.get<BodyForceRegionParameters>("regionParameters");
       

        // Save sideAxisGrid[]: 
        const int *sideAxisGrid = bodyForce.dbase.get<int[3]>("sideAxisGrid");
        if( !dbase.has_key("sideAxisGrid") )
          dbase.put<int[3]>("sideAxisGrid");
        int *sag                = dbase.get<int[3]>("sideAxisGrid");
	for( int i=0; i<3; i++ )
	  sag[i]=sideAxisGrid[i];

	aString & regionType = regionPar.dbase.get<aString>("regionType");

	// Save the region type:
	regionType = bodyForce.dbase.get<aString>("regionType");  // region type

	if( regionType=="box" )
	{
	  const real *boxBounds =  bodyForce.dbase.get<real[6]>("boxBounds");
	  real *bpar = regionPar.dbase.get<real[6]>("boxBounds");
	  for( int i=0; i<6; i++ )
	    bpar[i]=boxBounds[i];
	}
	else if( regionType=="ellipse" )
	{
	  const real *ellipse    =  bodyForce.dbase.get<real[6]>("ellipse");
	  real *epar =  regionPar.dbase.get<real[6]>("ellipse");
	  for( int i=0; i<6; i++ )
	    epar[i]=ellipse[i];
	}
	else
	{
	  printF("ProbeInfo: ERROR: unexpected regionType=%s\n",(const char*)regionType);
	  OV_ABORT("ERROR: finish me...");
	}
        

      }

    }
      

  }

  else
  {
    found=false;
  }
  

  return found;
}

int ProbeInfo:: 
buildSurfaceProbe( CompositeGrid & cg )
// ===================================================================================================
/// Build a boundary surface probe.
///
/// This routine will create an Integrate object as needed and store the surface defintion there.
// ===================================================================================================
{
  if( probeType==probeBoundarySurface && dbase.get<aString>("measureType")=="integral" )
  {
    IntegerArray & boundaryFaces = dbase.get<IntegerArray>("boundaryFaces");
    const int numberOfFaces = boundaryFaces.getLength(1);
    if( numberOfFaces>0 )
    {
      // -- define a new boundary surface ---

      // The Integrate object is accessible to other apps
      if( !parameters.dbase.has_key("integrate")) 
      {
	printF("ProbeInfo:update: create an integrate object...\n");
	parameters.dbase.put<Integrate*>("integrate");  
	parameters.dbase.get<Integrate*>("integrate")=NULL;
      }

      Integrate *& pIntegrate = parameters.dbase.get<Integrate*>("integrate");
      // cout << "pIntegrate=" << pIntegrate << endl;
      if( pIntegrate==NULL )
      {
	printF("ProbeInfo:update: Build an Integrate object...\n");
	pIntegrate = new Integrate(cg);  // ************************************ who deletes this??
      }
      Integrate & integrate = *pIntegrate;
	  
      int surfaceID=integrate.numberOfSurfaces()+1;

      integrate.defineSurface( surfaceID,numberOfFaces,boundaryFaces ); 
      printF("ProbeInfo:update:define a new boundary surface with %i faces, surfaceID=%i.\n"
	     " NOTE: this surface will over-ride any previously defined surface for this probe.\n",
	     numberOfFaces,surfaceID);

      // save the surfaceID with this probeInfo object
      if (!dbase.has_key("surfaceID")) 
	dbase.put<int>("surfaceID");
      dbase.get<int>("surfaceID")=surfaceID;

    }
  }
  return 0;
}

// ===========================================================================================
/// \brief define the properties of the probe
///
/// \params cg (input) : the probe will be a point or region or surface on this grid
// ===========================================================================================
int ProbeInfo::
update( CompositeGrid & cg, GenericGraphicsInterface & gi )
{
  int returnValue=0;
  
  GUIState gui;
  gui.setWindowTitle("Define a Probe");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  aString & probeName = dbase.get<aString>("probeName");


  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString pbLabels[] = {"define region...",
                          "define surface...",
                          "bounding box...",
                          "help",
                          ""};
    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=4;
    dialog.setPushButtons( cmd, pbLabels, numRows ); 

    dialog.setOptionMenuColumns(1);
    
    aString typeCommands[] = { "grid point probe",
                               "location probe",
                               "bounding box probe",
			       "region probe",
			       "surface probe",
			       "" };
    dialog.addOptionMenu("Type:",typeCommands,typeCommands,probeType );

    int quantityOption=0;  // fix me 
    aString quantityCommands[] = { "all components",
                                   "temperature",
                                   "pressure",
				   "heat flux",
                                   "surface traction",
                                   "user defined",
				   //	 "lift",
				   //      "drag",
				   "" };
    dialog.addOptionMenu("Quantity:",quantityCommands,quantityCommands,quantityOption );


    // We can compute the average or 'total' of a quantity:
    int quantityMeasureOption=1; 
    aString quantityMeasureCommands[] = { "values",
                                          "average",
					  "total",
					  "" };
    dialog.addOptionMenu("Quantity measure:",quantityMeasureCommands,quantityMeasureCommands,quantityMeasureOption );

    // We can compute a sum or integral:
    int measureTypeOption=0; 
    aString measureTypeCommands[] = { "sum",
                                      "volume weighted sum",
				      "integral",
				      "" };
    dialog.addOptionMenu("Measure type:",measureTypeCommands,measureTypeCommands,measureTypeOption);

    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];


    int nt=0;
    
    textLabels[nt] = "probe name"; sPrintF(textStrings[nt], "%s",(const char*)probeName);  nt++; 

    textLabels[nt] = "grid point";  sPrintF(textStrings[nt], "%i %i %i %i (grid,i1,i2,i3)",grid,iv[0],iv[1],iv[2]); nt++;
    textLabels[nt] = "nearest grid point to"; sPrintF(textStrings[nt], "%g %g %g",0.,0.,0.); nt++;
    textLabels[nt] = "location"; sPrintF(textStrings[nt], "%g %g %g",0.,0.,0.); nt++;
														      

    textLabels[nt] = "file name"; sPrintF(textStrings[nt], "%s",(const char*)fileName);  nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    // dialog.setTextBoxes(cmd, textLabels, textStrings);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  }
  
  // ********************* Region options ********************************
  DialogData & regionOptionsDialog = gui.getDialogSibling();

  regionOptionsDialog.setWindowTitle("Region Options");
  regionOptionsDialog.setExitCommand("close region options", "close");
  if( buildDialog )
  {
    buildRegionOptionsDialog(regionOptionsDialog);
  }

  // Set defaults:
  if (!dbase.has_key("quantityMeasure"))
  {
    dbase.put<aString>("quantityMeasure");
    dbase.get<aString>("quantityMeasure")="average";
  }
  if (!dbase.has_key("measureType"))
  {
    dbase.put<aString>("measureType");
    dbase.get<aString>("measureType")="sum";
  }
  
  aString answer,answer2;
  
  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("probe>");  

  int len=0;
  for(int it=0; ; it++)
  {
    gi.getAnswer(answer,"");
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);

    ProbeTypesEnum probeTypeOld=probeType;

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="help" )
    {
      printF("Probes are used to record information about the solution.\n");
      printF("There are different types of probes:\n"
	     " probeAtGridPoint     : probe is located at a grid point.\n"
	     " probeAtLocation      : probe is located at a fixed position.\n"
	     " probeBoundingBox     : save probe data on the boundary of a box\n"
             "                        (e.g. as boundary conditions for a subsequent sub-domain computation.)\n"
	     " probeRegion          : probe is some average or integral over a region.\n"
	     " probeBoundarySurface : probe is some average or integral over a boundary surface.\n");
      
    }

    else if( answer=="grid point probe"    ||
             answer=="location probe"      ||
             answer=="bounding box probe" ||
	     answer=="region probe"       ||
	     answer=="surface probe" )
    {
      probeType = ProbeTypesEnum(answer=="grid point probe"   ? 0 : 
				 answer=="location probe"     ? 1 :
				 answer=="bounding box probe" ? 2 :
				 answer=="region probe"       ? 3 :
				 answer=="surface probe"      ? 4 : 0 );

      
    }

    else if( answer=="all components"   ||
             answer=="temperature"      ||
             answer=="pressure"         ||
	     answer=="heat flux"        ||
	     answer=="surface traction" ||
	     answer=="user defined" )
    {
      if (!dbase.has_key("quantity")) 
	dbase.put<aString>("quantity");

      dbase.get<aString>("quantity")=answer;
      printF("Setting quantity=[%s]\n",(const char*)dbase.get<aString>("quantity"));

      dialog.getOptionMenu("Quantity:").setCurrentChoice(answer);
    }

    else if( answer=="values" ||
             answer=="average" ||
	     answer=="total" )
    {
      dbase.get<aString>("quantityMeasure")=answer;
      printF("Setting quantityMeasure=[%s]\n",(const char*)dbase.get<aString>("quantityMeasure"));

      dialog.getOptionMenu("Quantity measure:").setCurrentChoice(answer);
    }

    else if( answer=="sum" ||
             answer=="volume weighted sum" ||
	     answer=="integral" )
    {
      dbase.get<aString>("measureType")=answer;
      printF("Setting measureType=[%s]\n",(const char*)dbase.get<aString>("measureType"));
      dialog.getOptionMenu("Measure type:").setCurrentChoice(answer);
    }

    else if( len=answer.matches("grid point") )
    {
      probeType=probeAtGridPoint;
      
      sScanF(answer(len,answer.length()-1),"%i %i %i %i",&grid,&iv[0],&iv[1],&iv[2]);
      printF("ProbeInfo:Using grid point (grid,i1,i2,i3)=(%i,%i,%i)\n",grid,iv[0],iv[1],iv[2]);
      dialog.setTextLabel("grid point",sPrintF(answer2,"%i %i %i %i (grid,i1,i2,i3)",grid,iv[0],iv[1],iv[2]));
    }
    else if( (len=answer.matches("nearest grid point to")) ||
	     (len=answer.matches("location")) )
    {
      // -- New way (works in parallel) *wdh* 2013/08/28 ---
      RealArray x(1,3); x=0.;
      sScanF(answer(len,answer.length()-1),"%e %e %e",&x(0,0),&x(0,1),&x(0,2));

      if( answer.matches("nearest grid point to") )
      {
	probeType=probeAtGridPoint; 
	if( probeType!=probeAtGridPoint )
	  printF("INFO: setting probeType=probeAtGridPoint\n");
	dialog.setTextLabel("nearest grid point to",sPrintF(answer2,"%g %g %g",x(0,0),x(0,1),x(0,2)));

      }
      else
      {
	if( probeType!=probeAtLocation )
	  printF("INFO: setting probeType=probeAtLocation\n");
	probeType=probeAtLocation;
	dialog.setTextLabel("location",sPrintF(answer2,"%g %g %g",x(0,0),x(0,1),x(0,2)));
      }

      IntegerArray il(1,4); il=0; // holds (donor,i1,i2,i3)
      RealArray ci(1,3);    ci=0.;

      InterpolatePointsOnAGrid::findNearestValidGridPoint( cg, x, il, ci );

      grid = il(0,0);  // donor grid 
      assert( grid>=0 && grid <cg.numberOfComponentGrids() );
      MappedGrid & mg = cg[grid];
      const IntegerArray & gid = mg.gridIndexRange();

      real alpha0[3];
      real *alpha=alpha0;
      if( probeType==probeAtLocation )
      {
	if( !dbase.has_key("alpha") ){ dbase.put<real[3]>("alpha"); }
	alpha = dbase.get<real[3]>("alpha");
      }
      
      for( int axis=0; axis<3; axis++ )
      {
	xv[axis]=x(0,axis);       // probe physical location
	iv[axis]=il(0,axis+1);    // closest point 
	alpha[axis] = ci(0,axis)/mg.gridSpacing(axis)+gid(0,axis) - iv[axis];

        if( probeType==probeAtLocation && alpha[axis]<0. )
	{ // shift iv to be lower left corner of interp. stencil
          iv[axis]--; 
          alpha[axis]+=1.;
	}
	
      }

      if( probeType==probeAtGridPoint )
      {
	printF("ProbeInfo:INFO: nearest grid pt to x=(%e,%e,%e) : grid=%i (i1,i2,i3)=(%i,%i,%i)\n",
	       x(0,0),x(0,1),x(0,2),grid,iv[0],iv[1],iv[2]);
      }
      else
      {
        real *alpha = dbase.get<real[3]>("alpha");
	printF("ProbeInfo:INFO: The point (%8.2e,%8.2e,%8.2e) can be interpolated from donor=%i, unit square coords=[%8.2e,%8.2e,%8.2e],\n"
	       "  The lower left corner of interpolation stencil is il=[%i,%i,%i], alpha=[%8.2e,%8.2e,%8.2e] (for Lagrange interpolant).\n",
	       x(0,0),x(0,1),x(0,2),grid,ci(0,0),ci(0,1),ci(0,2),iv[0],iv[1],iv[2],alpha[0],alpha[1],alpha[2]);
      }

      // Double check: find position nearest" point from the mapping
      if( true )
      {
	RealArray xgv(1,3); xgv=0.;

	Mapping & map = mg.mapping().getMapping();
	map.mapS( ci,xgv );

	real dist=0.;
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	  dist+=SQR(x(0,axis)-xgv(0,axis));
	dist=sqrt(dist);
	printF("ProbeInfo:INFO: pt=(%e,%e,%e), mapping-location=(%e,%e,%e), dist=%8.2e\n",x(0,0),x(0,1),x(0,2),
	       xgv(0,0),xgv(0,1),xgv(0,2),dist);
      }

    }
    
    else if( len=answer.matches("nearest grid point to") )
    {
      // **OLD WAY **
      probeType=probeAtGridPoint; 
      RealArray x(1,3);
      sScanF(answer(len,answer.length()-1),"%e %e %e",&x(0,0),&x(0,1),&x(0,2));

      IntegerArray gridLocation(1,4);

      printF("ProbeInfo: finding closest grid point to x=(%e,%e,%e)\n",x(0,0),x(0,1),x(0,2));
      getClosestGridPoint( cg,x,gridLocation );
      if( gridLocation(0,3)>=0 )
      {
	iv[0]=gridLocation(0,0);
	iv[1]=gridLocation(0,1);
	iv[2]=gridLocation(0,2);
	grid=gridLocation(0,3);
	printF("ProbeInfo:Using grid=%i, (i1,i2,i3)=(%i,%i,%i)\n",grid,iv[0],iv[1],iv[2]);
	if( true )
	{
	  int i1=iv[0], i2=iv[1], i3=iv[2];
	  cg[grid].update(MappedGrid::THEcenter);  // *** FIX ME for rectangular **

	  realArray & center = cg[grid].center();
	  real dist = SQR(x(0,0)-center(i1,i2,i3,0)) + SQR(x(0,1)-center(i1,i2,i3,1));
	  if( cg.numberOfDimensions()==3 ) dist += SQR(x(0,2)-center(i1,i2,i3,2));
	  dist=sqrt(dist);
	  printF("ProbeInfo: pt=(%e,%e,%e), grid-pt=(%e,%e,%e), dist=%8.2e\n",x(0,0),x(0,1),x(0,2),
		 center(i1,i2,i3,0),center(i1,i2,i3,1),(cg.numberOfDimensions()==3 ? center(i1,i2,i3,2) : 0.),
		 dist);
	}
      }
      else
      {
	printF("ProbeInfo:ERROR finding closest grid point to x=(%e,%e,%e)\n",x(0,0),x(0,1),x(0,2));
        
      }
      
    }
    else if( len=answer.matches("location") )
    {
      // **OLD WAY **
  
      // --- Point probe for a specified location ---

      if( probeType!=probeAtLocation )
	printF("INFO: setting probeType=probeAtLocation\n");
	
      probeType=probeAtLocation;

      RealArray x(1,3);
      sScanF(answer(len,answer.length()-1),"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
      dialog.setTextLabel("location",sPrintF(answer2,"%g %g %g",x(0,0),x(0,1),x(0,2)));

      InterpolatePointsOnAGrid interpolator;
      int infoLevel=1;
      interpolator.setInfoLevel( infoLevel );
      int interpolationWidth=2;  // use linear interp. for now 
      interpolator.setInterpolationWidth(interpolationWidth);
      // Set the number of valid ghost points that can be used when interpolating from a grid function: 
      int numGhostToUse=1;
      interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
      // Assign all points, extrapolate pts if necessary:
      interpolator.buildInterpolationInfo(x,cg );

      IntegerArray indexValues(1,3), interpoleeGrid(1);
      RealArray interpCoord(1,3);
      interpolator.getInterpolationInfo(cg,indexValues,interpoleeGrid,interpCoord );

      // Here is alpha for the Lagrange interpolation weights 0<= alpha <= interpolationWidth-1
      grid = interpoleeGrid(0);
      assert( grid>=0 && grid <cg.numberOfComponentGrids() );
      MappedGrid & mg = cg[grid];
      const IntegerArray & gid = mg.gridIndexRange();
      if( !dbase.has_key("alpha") ){ dbase.put<real[3]>("alpha"); }
      real *alpha = dbase.get<real[3]>("alpha");
      for( int axis=0; axis<3; axis++ )
      {
        xv[axis]=x(0,axis);           // probe physical location
	iv[axis]=indexValues(0,axis); // lower left corner of interp. stencil
	alpha[axis] = interpCoord(0,axis)/mg.gridSpacing(axis)+gid(0,axis) - indexValues(0,axis);
      }
      printF("outputProbes:INFO: The point (%8.2e,%8.2e,%8.2e) can be interpolated from donor=%i, unit square coords=[%8.2e,%8.2e,%8.2e],\n"
             "  The lower left corner of interpolation stencil is il=[%i,%i,%i], alpha=[%8.2e,%8.2e,%8.2e] (for Lagrange interpolant).\n",
	     x(0,0),x(0,1),x(0,2),interpoleeGrid(0),interpCoord(0,0),interpCoord(0,1),interpCoord(0,2),indexValues(0,0),indexValues(0,1),indexValues(0,2),
             alpha[0],alpha[1],alpha[2]);

    }
    
    else if( answer=="bounding box..." )
    {
      printF("Define a bounding box probe - save data on the boundary of a bounding box.\n");

      GUIState gui;
      gui.setWindowTitle("Bounding Box Probe");
      gui.setExitCommand("exit", "continue");
      DialogData & dialog = (DialogData &)gui;

      const int numberOfTextStrings=5;
      aString textLabels[numberOfTextStrings];
      aString textStrings[numberOfTextStrings];

      int nt=0;
    
      textLabels[nt] = "bounding box grid";  sPrintF(textStrings[nt], "%i",boundingBoxGrid); nt++; 
    
      textLabels[nt] = "bounding box";  sPrintF(textStrings[nt], "%i %i, %i %i, %i %i (i1a,i1b, i2a,i2b, i3a,i3b)",
						bb(0,0),bb(1,0), bb(0,1), bb(1,1), bb(0,2), bb(1,2)); nt++;
    
      textLabels[nt] = "number of layers";  sPrintF(textStrings[nt], "%i",numberOfLayers);  nt++; 

      // null strings terminal list
      textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
      // addPrefix(textLabels,prefix,cmd,maxCommands);
      // dialog.setTextBoxes(cmd, textLabels, textStrings);
      dialog.setTextBoxes(textLabels, textLabels, textStrings);
 
      gi.pushGUI(gui);
      gi.appendToTheDefaultPrompt("boundingBox>");  


      for( ;; )
      {
	gi.getAnswer(answer,"");
	
	if( answer=="exit" || answer=="done" )
	{
	  break;
	}
	else if( dialog.getTextValue(answer,"bounding box grid","%i",boundingBoxGrid) )
	{
	  probeType=probeBoundingBox;
	  if( boundingBoxGrid>=0 && boundingBoxGrid<cg.numberOfComponentGrids() )
	  {
	    const IntegerArray & gid = cg[boundingBoxGrid].gridIndexRange();
	    printF("ProbeInfo: The grid index range on grid=%i (%s) is [%i,%i]x[%i,%i]x[%i,%i]\n",
		   boundingBoxGrid, (const char*)cg[boundingBoxGrid].getName(),
		   gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2));
	  }
	  else
	  {
	    printF("ProbeInfo:ERROR: invalid choice for grid=%i. There are only %i grids\n",
		   boundingBoxGrid,cg.numberOfComponentGrids());
	    boundingBoxGrid=0;
	    dialog.setTextLabel("ounding box grid",sPrintF("%i",boundingBoxGrid));
	  }
	}
	else if( len=answer.matches("bounding box") )
	{
	  probeType=probeBoundingBox;

	  sScanF(answer(len,answer.length()-1),"%i %i %i %i %i %i",&bb(0,0),&bb(1,0), &bb(0,1), &bb(1,1), 
		 &bb(0,2), &bb(1,2));
	  if( boundingBoxGrid>=0 && boundingBoxGrid<cg.numberOfComponentGrids() )
	  {
	    const IntegerArray & gid = cg[boundingBoxGrid].gridIndexRange();
	    for( int axis=0; axis<3; axis++ )
	      for( int side=0; side<=1; side++ )
	      {
		if( (side==0 && bb(0,axis)<gid(0,axis)) || ( side==1 && bb(1,axis)>gid(1,axis)) )
		{
		  printF("ProbeInfo: error: bb(%i,%i)=%i is outside the gridIndexRange(%i,%i)=%i. Setting to %i\n",
			 side,axis,bb(side,axis), side,axis,gid(side,axis), gid(side,axis));
		  bb(side,axis)=gid(side,axis);
		}
	      }
	
	  }
      
	  printF("ProbeInfo: Saving the solution from grid %i (%s) on the bounding box:\n"
		 "  [i1a,i1b]x[i2a,i2b]x[i3a,i3b]=[%i,%i]x[%i,%i]x[%i,%i]\n"
		 "  You should also set the `number of layers'. Current number of layers=%i\n",
		 boundingBoxGrid, (const char*)cg[boundingBoxGrid].getName(),
		 bb(0,0),bb(1,0), bb(0,1), bb(1,1), bb(0,2), bb(1,2), numberOfLayers);
	  dialog.setTextLabel("bounding box",sPrintF(answer2,"%i %i, %i %i, %i %i (i1a,i1b, i2a,i2b, i3a,i3b)",
						     bb(0,0),bb(1,0), bb(0,1), bb(1,1), bb(0,2), bb(1,2)));
	}
	else if( dialog.getTextValue(answer,"number of layers","%i",numberOfLayers) ){} // 
	else
	{
	  printF("Define bounding box:ERROR: Unknown response: [%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}

      } // end for(;; ) (bounding box)

      gi.popGUI();
      gi.unAppendTheDefaultPrompt();

    }


    else if( dialog.getTextValue(answer,"probe name","%s",probeName) ){} // 

    else if( len=answer.matches("file name") )
    {
      // remove initial and trailing blanks.
      int length=answer.length();
      int istart=len;
      while( istart<length && answer[istart]==' ') istart++;
      int iend=length-1;
      while( iend>=istart && answer[iend]==' ') iend--;
      if( iend>=istart )
      {
	fileName=answer(istart,iend);
	printF("ProbeInfo::update: setting probe file name = [%s]\n",(const char*)fileName);
      }
      else
      {
        printF("ProbeInfo::update:ERROR: file name was empty!\n");
        gi.stopReadingCommandFile();
      }
    }

    else if( getRegionOption(answer,regionOptionsDialog ) )
    {
      printF("Answer=[%s] found in getRegionOption\n",(const char*)answer);


    }
    else if( answer=="define region..." )
    {
      regionOptionsDialog.showSibling();
    }
    else if( answer=="close region options" )
    {
      regionOptionsDialog.hideSibling(); 
    }


    
    else if( answer=="boundary forcing region..." )
    {
      if( !parameters.dbase.get<bool>("turnOnBoundaryForcing") )
      {
	printF("WARNING: There are currently no boundary forcing regions.");
      }
      else
      {
	// Here is the array of boundary forcings:
	std::vector<BodyForce*> & boundaryForcings =  parameters.dbase.get<std::vector<BodyForce*> >("boundaryForcings");

	printF("--Define a probe from an existing boundary region\n"
	       "  There are %i boundary regions defined.\n",boundaryForcings.size());
	
      }
      
    }
    
    else if( answer=="define surface..." )
    {
      // --------------------------------------------------------
      // --- Define a boundary surface to be used for a probe ---
      // --------------------------------------------------------

      //-- This could be a separate function ---

      printF("Define a surface from one or more boundary faces of the overlapping grid.\n");

      GUIState gui;
      gui.setWindowTitle("Boundary Surface");
      gui.setExitCommand("exit", "continue");
      DialogData & dialog = (DialogData &)gui;


      aString pbLabels[] = {"define surface by grid faces",
			    "define surface by share flag",
                            "print valid grid faces",
			    ""};
      // addPrefix(pbLabels,prefix,cmd,maxCommands);
      int numRows=3;
      dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

      const int numberOfTextStrings=5;
      aString textLabels[numberOfTextStrings];
      aString textStrings[numberOfTextStrings];

      int nt=0;
    
      int shareFlagForSurface=1;
      textLabels[nt] = "share flag"; sPrintF(textStrings[nt], "%i",shareFlagForSurface);  nt++; 
      // null strings terminal list
      textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
      dialog.setTextBoxes(textLabels, textLabels, textStrings);

      gi.pushGUI(gui);
      gi.appendToTheDefaultPrompt("defineSurface>");  

      // boundary(0:2,face) = (side,axis,grid) for a face=0,1,2,..,numberOfFaces-1

      if( !dbase.has_key("boundaryFaces") )
      {
	dbase.put<IntegerArray>("boundaryFaces");
      }
      IntegerArray & boundaryFaces = dbase.get<IntegerArray>("boundaryFaces");

      boundaryFaces.redim(3,20);  // initially space for 20 faces on the surface
      
      for( ;; )
      {
	gi.getAnswer(answer,"");
  
        int numberOfFaces=0;

	if( answer(0,prefix.length()-1)==prefix )
	  answer=answer(prefix.length(),answer.length()-1);

	if( answer=="exit" || answer=="done" )
	{
	  break;
	}
	else if( dialog.getTextValue(answer,"share flag","%i",shareFlagForSurface) )
	{
	  printF("Setting shareFlagForSurface=%i. Choose `define surface by share flag' to define the"
                 " surface using faces with this share flag\n",shareFlagForSurface);
	}
	else if( answer=="print valid grid faces" )
	{
          // -- output info of physical faces of grids --

          printF(" side axis grid  BC  share    name\n");
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )for( int side=0; side<=1; side++ )
	    {
	      if( mg.boundaryCondition(side,axis)>0 )
	      {
		printF("   %i    %i    %i    %i     %i   %s\n",side,axis,grid,mg.boundaryCondition(side,axis),
                       mg.sharedBoundaryFlag(side,axis),(const char*)mg.getName());
		
	      }
	    }
	  }
	}
	else if( answer=="define surface by grid faces" )
	{
	  // ---- define a boundary surface as a list of grid faces ---

	  for( ;; )
	  {
	    gi.inputString(answer,"Enter side,axis,grid for the grid face to add"
			   " (Enter `done' to finish)");
	    if( answer=="done" )
	      break;
	    else 
	    {
	
	
	      int side=-1, axis=-1, grid=-1;
	      sScanF(answer,"%i %i %i",&side,&axis,&grid);
	
	      if( grid<0 || grid >= cg.numberOfComponentGrids() || 
		  side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
	      {
		if( grid<0 || grid >cg.numberOfComponentGrids() )
		  printF("ERROR: invalid value for grid=%i. There are %i component grids\n",
			 grid,cg.numberOfComponentGrids());
		if( side<0 || side>1 || axis<0 || axis>=cg.numberOfDimensions() )
		  printF("ERROR: invalid value for side=%i or axis=%i\n",side,axis);
		gi.stopReadingCommandFile();
		break;
	      }
	      if( numberOfFaces>=boundaryFaces.getLength(1) )
	      {
                // increase number of allowable faces:
		boundaryFaces.resize(boundaryFaces.getLength(0),boundaryFaces.getLength(1)+20); 
	      }
	
	      boundaryFaces(0,numberOfFaces)=side;
	      boundaryFaces(1,numberOfFaces)=axis;
	      boundaryFaces(2,numberOfFaces)=grid;
	      numberOfFaces++;
	      
	    }
	    
	  } // end for( ;; ) -- input faces
	  
	} // end define by grid faces
	else if( answer=="define surface by share flag" )
	{
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    MappedGrid & mg = cg[grid];
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )for( int side=0; side<=1; side++ )
	    {
	      if( mg.sharedBoundaryFlag(side,axis)==shareFlagForSurface )
	      {
		if( numberOfFaces>=boundaryFaces.getLength(1) )
		{
                  // increase number of allowable faces:
		  boundaryFaces.resize(boundaryFaces.getLength(0),boundaryFaces.getLength(1)+20); 
		}
		printF(" adding face (%i,%i,%i)=(side,axis,grid) with share flag=%i to the surface.\n",
                       side,axis,grid,shareFlagForSurface);
		boundaryFaces(0,numberOfFaces)=side;
		boundaryFaces(1,numberOfFaces)=axis;
		boundaryFaces(2,numberOfFaces)=grid;
		numberOfFaces++;
		
	      }
	    }
	  }
	}
	else
	{
	  printF("Define boundary surface:ERROR: Unknown response: [%s]\n",(const char*)answer);
	  gi.stopReadingCommandFile();
	}

	if( numberOfFaces>0 )
	{
          // resize to actual number of faces.
          boundaryFaces.resize(boundaryFaces.getLength(0),numberOfFaces);
	  probeType=probeBoundarySurface;
	}

      } // end for( ;; )

      gi.popGUI();
      gi.unAppendTheDefaultPrompt();

    }
    else
    {
      printF("ProbeInfo::update:ERROR: Unknown response: [%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
       
    }
    
    // initialize the boundary surface probe (store surface faces in an Integrate object if needed)
    buildSurfaceProbe(cg);
    
//     if( probeType==probeBoundarySurface && dbase.get<aString>("measureType")=="integral" )
//     {
//       // --- for surface probes that use integration we build a surface in the Integrate object ----
//       IntegerArray & boundaryFaces = dbase.get<IntegerArray>("boundaryFaces");
//       const int numberOfFaces = boundaryFaces.getLength(1);
//       if( numberOfFaces>0 )
//       {
// 	// -- define a new boundary surface ---

// 	// The Integrate object is accessible to other apps
// 	if( !parameters.dbase.has_key("integrate")) 
// 	{
// 	  printF("ProbeInfo:update: create an integrate object...\n");
// 	  parameters.dbase.put<Integrate*>("integrate");  
// 	  parameters.dbase.get<Integrate*>("integrate")=NULL;
// 	}

// 	Integrate *& pIntegrate = parameters.dbase.get<Integrate*>("integrate");
// 	// cout << "pIntegrate=" << pIntegrate << endl;
// 	if( pIntegrate==NULL )
// 	{
// 	  printF("ProbeInfo:update: Build an Integrate object...\n");
// 	  pIntegrate = new Integrate(cg);  // ************************************ who deletes this??
// 	}
// 	Integrate & integrate = *pIntegrate;
	  
// 	int surfaceID=integrate.numberOfSurfaces()+1;

// 	integrate.defineSurface( surfaceID,numberOfFaces,boundaryFaces ); 
// 	printF("ProbeInfo:update:define a new boundary surface with %i faces, surfaceID=%i.\n"
// 	       " NOTE: this surface will over-ride any previously defined surface for this probe.\n",
// 	       numberOfFaces,surfaceID);

// 	// save the surfaceID with this probeInfo object
// 	if (!dbase.has_key("surfaceID")) 
// 	  dbase.put<int>("surfaceID");
// 	dbase.get<int>("surfaceID")=surfaceID;

//       }

//     }

    // --- update the probeType if it has changed ---
    if( probeType!=probeTypeOld )
    {
      printF("Setting the probe type to %i\n",(int)probeType);
      dialog.getOptionMenu("Type:").setCurrentChoice((int)probeType);
    }

  } // end for (it) -- main query loop 
  



  if( true ) // !executeCommand 
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  
  return returnValue;

}
