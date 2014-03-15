#include "DomainSolver.h"
#include "ParallelUtility.h"
#include "GenericGraphicsInterface.h"
#include "GridMaterialProperties.h"

//==============================================================================================
//
/// \brief Assign the user defined material properties.
/// \details Rewrite or add new options to 
///   this function and to setupUserDefinedMaterialProperties to define new options.
//
/// \return values: 0 : material properties were set. -1 : no material properties were set.
//==============================================================================================
int DomainSolver::
userDefinedMaterialProperties(GridFunction & gf)
{

  if( true )
    printF("****** userDefinedMaterialProperties ********\n");

  CompositeGrid & cg = gf.cg;

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  
  // Look for the sub-directory in the data-base used to store user defined material property options:
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedMaterialPropertyData") )
  {
    return -1;
  }
  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedMaterialPropertyData");

  aString & option= db.get<aString>("option");
  if( option=="none" )
    return -1;

  // Names of material properties are stored in materialPropertyNames:
  std::vector<aString> & materialPropertyNames = parameters.dbase.get<std::vector<aString> >("materialPropertyNames");
  const int numberOfMaterialProperties=materialPropertyNames.size();

  // Material properties are stored in an array of GridMaterialProperties objects:
  std::vector<GridMaterialProperties> & materialProperties = 
    parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");

  // Loop over all grids 
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    c.update(MappedGrid::THEvertex | MappedGrid::THEcenter );  // make sure the vertex array has been created
    OV_GET_SERIAL_ARRAY(int,c.mask(),maskLocal);
    OV_GET_SERIAL_ARRAY(real,c.vertex(),vertexLocal);

    Index I1,I2,I3;
    getIndex( c.dimension(),I1,I2,I3 );          // all points including ghost points.
    // getIndex( c.gridIndexRange(),I1,I2,I3 );  // boundary plus interior points.
    #ifdef USE_PPP
    // restrict bounds to local processor, include ghost
    bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3,1);   
    if( !ok ) continue;  // no points on this processor
    #endif

    // The material property values are stored in arrays found in:
    GridMaterialProperties & matProp = materialProperties[grid];
    GridMaterialProperties::MaterialFormatEnum materialFormat = matProp.getMaterialFormat();
    
    IntegerArray & matIndex = matProp.getMaterialIndexArray();
    RealArray & matVal      = matProp.getMaterialValuesArray();


    if( option=="bubbles" )
    {
      // define a set of bubbles -- circular regions with constant properties.
      const int & numberOfBubbles = db.get<int>("numberOfBubbles");
      const RealArray & bubbleCentre = db.get<RealArray>("bubbleCentre");
      const RealArray & bubbleRadius = db.get<RealArray>("bubbleRadius");
      const RealArray & bubbleValues = db.get<RealArray>("bubbleValues");
      const RealArray & backGroundValues = db.get<RealArray>("backGroundValues");

      // --- Set the background values ---
      if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
      {

        // -- The material properties are piecewise constant (see comments below)
        matIndex.redim(I1,I2,I3);
	matIndex=0;

        const int numberOfMaterialRegions=numberOfBubbles+1;  // background + bubbles 
	matVal.redim(numberOfMaterialProperties,numberOfMaterialRegions);

        int matRegion=0;  // index for background
	for( int m=0; m<numberOfMaterialProperties; m++ )
	{
	  matVal(m,matRegion)=backGroundValues(m);
	}
      }
      else if( materialFormat==GridMaterialProperties::variableMaterialProperties )
      {
        // material properties vary from grid-point to grid-point
	matVal.redim(I1,I2,I3,numberOfMaterialProperties);

	for( int m=0; m<numberOfMaterialProperties; m++ )
	{
	  matVal(I1,I2,I3,m)=backGroundValues(m);
	}
      }
      else
      {
        OV_ABORT("ERROR: unexpected materialFormat");
      }
      
      
      for( int b=0; b<numberOfBubbles; b++ )
      {
	RealArray radius;
	if( numberOfDimensions==2 )
	  radius = sqrt( SQR(vertexLocal(I1,I2,I3,axis1)-bubbleCentre(b,0))+
			 SQR(vertexLocal(I1,I2,I3,axis2)-bubbleCentre(b,1)) );
	else
	  radius = sqrt( SQR(vertexLocal(I1,I2,I3,axis1)-bubbleCentre(b,0))+
			 SQR(vertexLocal(I1,I2,I3,axis2)-bubbleCentre(b,1))+
			 SQR(vertexLocal(I1,I2,I3,axis3)-bubbleCentre(b,2)) );


	
	if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
	{
	  // -- The material properties are piecewise constant : we store the properties
          //    in a compact format. Example: To access the values for the material properties 
          //    rho,mu,lambda at a grid point (i1,i2,i3) use:
          //       int matRegion = matIndex(i1,i2,i3);
          //       real rho    = matValue(0,matRegion);
          //       real mu     = matValue(1,matRegion);
          //       real lambda = matValue(1,matRegion);

	  int matRegion=b+1;  // material region number for bubble "b"
	  for( int m=0; m<numberOfMaterialProperties; m++ )
	  {
	    matVal(m,matRegion)=bubbleValues(b,m);
	  }
	
	  where( radius<=bubbleRadius(b) )
	  {
	    for( int m=0; m<numberOfMaterialProperties; m++ )
	    {
              matIndex(I1,I2,I3)= matRegion;   // grid points in the bubble belong to material region "matRegion"
	    }
	  }
	}
	else if( materialFormat==GridMaterialProperties::variableMaterialProperties )
	{
	  // material properties vary from grid-point to grid-point

	  where( radius<=bubbleRadius(b) )
	  {
	    for( int m=0; m<numberOfMaterialProperties; m++ )
	    {
              matVal(I1,I2,I3,m)= bubbleValues(b,m);
	    }
	  }
	}
	
      }
      

    }
    else
    {
      printF("userDefinedMaterialProperties: Unknown option =[%s]",(const char*)option);
    }
    
  } // end for grid 
  
  return 0;
}




//==============================================================================================
/// \brief Interactively choose material properties (e.g. rho,mu,lambda for elasticity)
/// \details This function is called after choosing 'user defined material properties'. 
///    The function that actually evaluate the material properties is userDefinedMaterialProperties.
//==============================================================================================
int DomainSolver::
setupUserDefinedMaterialProperties()
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  const int & numberOfComponents=parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions=parameters.dbase.get<int >("numberOfDimensions");
  const int & rc = parameters.dbase.get<int >("rc");   //  density = u(all,all,all,rc)  (if appropriate for this PDE)
  const int & uc = parameters.dbase.get<int >("uc");   //  u velocity component =u(all,all,all,uc)
  const int & vc = parameters.dbase.get<int >("vc");  
  // const int & wc = parameters.dbase.get<int >("wc");
  const int & tc = parameters.dbase.get<int >("tc");   //  temperature
  const int & sc = parameters.dbase.get<int >("sc");   //  mass fraction lambda
  //  const int & pc = parameters.dbase.get<int >("pc");
  
  // here is a menu of possible material configurations
  aString menu[]=  
  {
    "bubbles",
    "exit",
    ""
  };
  aString answer,answer2;
  char buff[100];
  gi.appendToTheDefaultPrompt(">user matProp");

  // Make a sub-directory in the data-base to store variables used here and in userDefinedMaterialProperties
  if( !parameters.dbase.get<DataBase >("modelData").has_key("userDefinedMaterialPropertyData") )
    parameters.dbase.get<DataBase >("modelData").put<DataBase>("userDefinedMaterialPropertyData");

  DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedMaterialPropertyData");

  // Different options are stored as a string:
  aString & option= db.put<aString>("option");
  option="none";


  // ----------------------------------------------------------------------------------------------------------------------------
  // Names of material properties are stored in materialPropertyNames (Each name should be an entry in the dbase of type real)
  // 
  // For example, for linear elasticity: (defined in SmParameters.C)
  //      materialPropertyNames[0] = "rho"
  //      materialPropertyNames[1] = "mu"
  //      materialPropertyNames[2] = "lambda"
  // and the default value is
  //     real rho =  parameters.dbase.get<real>(materialPropertyNames[0]);
  // 
  // ----------------------------------------------------------------------------------------------------------------------------
  std::vector<aString> & materialPropertyNames = parameters.dbase.get<std::vector<aString> >("materialPropertyNames");
  const int numberOfMaterialProperties=materialPropertyNames.size();

  printF("setupUserDefinedMaterialProperties: INFO: there are %i material properties:\n",numberOfMaterialProperties);
  for( int m=0; m<numberOfMaterialProperties; m++ )
  {
    printF(" %s : default value = %g\n",(const char*)materialPropertyNames[m],parameters.dbase.get<real>(materialPropertyNames[m]));
  }
  

  for( ;; )
  {
    gi.getMenuItem(menu,answer,"enter an option");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="bubbles" )
    {
      // --- define a set of bubbles: circular regions with constant properties. ---

      option="bubbles";
      // Material properties are piecewise constant, store in an efficient way:
      parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::piecewiseConstantMaterialProperties;

      // Use this option if material properties vary continuously from grid point to grid point.
      // parameters.dbase.get<int>("variableMaterialPropertiesOption")=GridMaterialProperties::variableMaterialProperties;

      db.put<int>("numberOfBubbles",0);
      db.put<RealArray>("bubbleCentre");
      db.put<RealArray>("bubbleRadius");
      db.put<RealArray>("bubbleValues");
      db.put<RealArray>("backGroundValues");

      int & numberOfBubbles  = db.get<int>("numberOfBubbles");
      RealArray & bubbleCentre = db.get<RealArray>("bubbleCentre");
      RealArray & bubbleRadius = db.get<RealArray>("bubbleRadius");
      RealArray & bubbleValues = db.get<RealArray>("bubbleValues");
      RealArray & backGroundValues = db.get<RealArray>("backGroundValues");
      
      gi.inputString(answer2,"Enter the number of bubbles");
      sScanF(answer2,"%i",&numberOfBubbles);  
      printF("numberOfBubbles = %i \n",numberOfBubbles);

      bubbleCentre.redim(numberOfBubbles,3); bubbleCentre=0.;
      bubbleRadius.redim(numberOfBubbles); bubbleRadius=1.;
      bubbleValues.redim(numberOfBubbles,numberOfMaterialProperties); bubbleValues=1.;
      backGroundValues.redim(numberOfMaterialProperties); backGroundValues=1.;


      // Choose the background material values.
      for( int m=0; m<numberOfMaterialProperties; m++ )
      {
        real val = parameters.dbase.get<real>(materialPropertyNames[m]);
        backGroundValues(m)=val;
	gi.inputString(answer2,sPrintF(buff,"Background values: enter %s (default=%g)",(const char*)materialPropertyNames[m],val)); 
	if( answer2!="" )
  	  sScanF(answer2,"%e",&backGroundValues(m));
      }

      // Define the bubble locations and sizes and set the values.
      for( int b=0; b<numberOfBubbles; b++ )
      {
	gi.inputString(answer2,sPrintF(buff,"Enter radius and centre of bubble %i",b));
	sScanF(answer2,"%e %e %e %e",&bubbleRadius(b),&bubbleCentre(b,0),&bubbleCentre(b,1),&bubbleCentre(b,2));

	for( int m=0; m<numberOfMaterialProperties; m++ )
	{
	  gi.inputString(answer2,sPrintF(buff,"Bubble %i: enter %s",b,(const char*)materialPropertyNames[m])); 
	  sScanF(answer2,"%e",&bubbleValues(b,m));
	}
	
        printF("Setting bubble b=%i: radius=%g, center=(%g,%g,%g),",b,bubbleRadius(b),bubbleCentre(b,0),bubbleCentre(b,1),bubbleCentre(b,2));
	for( int m=0; m<numberOfMaterialProperties; m++ )
	{
          printF(" %s=%g,",(const char*)materialPropertyNames[m],bubbleValues(b,m));
	}
	printF("\n");
      } // end for( b )

    }
    else 
    {
      printF("setupUserDefinedMaterialProperties::unknown answer =[%s]",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
  
  gi.unAppendTheDefaultPrompt();
  return 0;
}



//! This routine is called when DomainSolver is finished and can be used to clean up memory.
void DomainSolver::
userDefinedMaterialPropertiesCleanup()
{
  if( parameters.dbase.get<int >("myid")==0 ) 
    printF("***userDefinedMaterialPropertiesCleanup...\n");

}

