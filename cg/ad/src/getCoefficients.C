// =============================================================================================
//      Functions that compute variable coefficients in the equations
// =============================================================================================


#include "Cgad.h"
#include "AdParameters.h"
#include "PlotStuff.h"
#include "ParallelUtility.h"

// ===========================================================================================================
/// \brief Assign the variable advection coefficients
// ===========================================================================================================
void Cgad::
getAdvectionCoefficients( GridFunction & cgf )
{
  real cpu0=getCPU();


  assert(  parameters.dbase.get<bool >("variableAdvection") );

  const bool & variableAdvection = parameters.dbase.get<bool >("variableAdvection");
  const bool & advectionIsTimeDependent =  parameters.dbase.get<bool>("advectionIsTimeDependent");

  DataBase & db =  parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedCoefficientsData");
  bool & advectionCoefficientsAreUpToDate = db.get<bool>("advectionCoefficientsAreUpToDate");
  if( !advectionIsTimeDependent && advectionCoefficientsAreUpToDate )
  {
    printF("getAdvectionCoefficients: coefficients are up to date\n");
    return;
  }

  const aString & userCoefficientsOption = db.get<aString>("userCoefficientsOption");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");

  const int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents");

  CompositeGrid & cg = cgf.cg;
  const real t = cgf.t;

  printF("++Cgad::getAdvectionCoefficients:INFO: evaluating variable advection coefficients at t=%9.3e\n",t);

  const int numberOfDimensions = cg.numberOfDimensions();
  
  realCompositeGridFunction*& pAdvectVar= parameters.dbase.get<realCompositeGridFunction*>("advectVar");
  if( pAdvectVar==NULL )
  {
    Range all;
    pAdvectVar = new realCompositeGridFunction(cg,all,all,all,numberOfDimensions); // WHO WILL DELETE ME ?
  }
  realCompositeGridFunction & advectVar = *pAdvectVar;

  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const bool & gridIsMoving = parameters.gridIsMoving(grid);
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );


    OV_GET_SERIAL_ARRAY_CONST(real,cgf.u[grid],uLocal); // here is the current solution

    OV_GET_SERIAL_ARRAY_CONST(real,advectVar[grid],advectVarLocal);
    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);

    getIndex(mg.dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(advectVar[grid],advectVarLocal,I1,I2,I3);
    if( !ok ) continue;
    
    if( userCoefficientsOption== "polynomial coefficients" )
    {
      // advection velocity is a polynomial in space and time
      const RealArray & act = db.get<RealArray>("act"); // polynomial coeff's in time
      const RealArray & acx = db.get<RealArray>("acx"); // polynomial coeff's in space

      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
        printF("axis=%i: act=%g,%g,%g,  acx=%g,%g,%g,%g,%g,%g\n",axis,act(axis,0),act(axis,1),act(axis,2), acx(axis,0,0,0),
	       acx(axis,1,0,0),acx(axis,1,1,0), acx(axis,2,0,0), acx(axis,0,1,0),acx(axis,0,2,0));
	
        real timeFunction = act(axis,0)+t*(act(axis,1)+t*act(axis,2));
	advectVarLocal(I1,I2,I3,axis)=( 
	  acx(axis,0,0,0) 
	  + xLocal(I1,I2,I3,0)*( acx(axis,1,0,0) + acx(axis,1,1,0)*xLocal(I1,I2,I3,1) + acx(axis,2,0,0)*xLocal(I1,I2,I3,0))
	  + xLocal(I1,I2,I3,1)*( acx(axis,0,1,0) + acx(axis,0,2,0)*xLocal(I1,I2,I3,1))
	  )*timeFunction;

	
      }
    }
    else
    {
      printF("Cgad::getAdvectionCoefficients:ERROR: Unknown userCoefficientsOption=[%s]\n",
             (const char*)userCoefficientsOption);
      OV_ABORT("error");
    }
    
  } // end for grid 
  
  advectionCoefficientsAreUpToDate=true;


//   OV_ABORT("finish me");

  // parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetCoefficients"))+=getCPU()-cpu0;
}

// ===========================================================================================================
/// \brief Assign the variable diffusion coefficients
// ===========================================================================================================
void Cgad::
getDiffusionCoefficients( GridFunction & cgf )
{
  real cpu0=getCPU();

  assert(  parameters.dbase.get<bool >("variableDiffusivity") );

  const bool & diffusivityIsTimeDependent =  parameters.dbase.get<bool>("diffusivityIsTimeDependent");

  DataBase & db =  parameters.dbase.get<DataBase >("modelData").get<DataBase>("userDefinedCoefficientsData");
  bool & diffusionCoefficientsAreUpToDate = db.get<bool>("diffusionCoefficientsAreUpToDate");
  if( !diffusivityIsTimeDependent && diffusionCoefficientsAreUpToDate )
  {
    printF("getDiffusionCoefficients: coefficients are up to date\n");
    return;
  }
  

  const aString & userCoefficientsOption = db.get<aString>("userCoefficientsOption");

  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");

  const int & numberOfComponents = parameters.dbase.get<int>("numberOfComponents");
  std::vector<real> & kappa = parameters.dbase.get<std::vector<real> >("kappa");

  CompositeGrid & cg = cgf.cg;
  const real t = cgf.t;

  printF("++Cgad::getDiffusionCoefficients:INFO: evaluating variable diffusion coefficients at t=%9.3e\n",t);

  realCompositeGridFunction*& pKappaVar= parameters.dbase.get<realCompositeGridFunction*>("kappaVar");
  if( pKappaVar==NULL )
  {
    Range all;
    pKappaVar = new realCompositeGridFunction(cg,all,all,all,numberOfComponents); // WHO WILL DELETE ME ?
  }
  realCompositeGridFunction & kappaVar = *pKappaVar;

  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const bool & gridIsMoving = parameters.gridIsMoving(grid);
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );

    OV_GET_SERIAL_ARRAY_CONST(real,cgf.u[grid],uLocal); // here is the current solution

    OV_GET_SERIAL_ARRAY_CONST(real,kappaVar[grid],kappaVarLocal);
    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);

    getIndex(mg.dimension(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(kappaVar[grid],kappaVarLocal,I1,I2,I3);
    if( !ok ) continue;
    
    if( userCoefficientsOption== "polynomial coefficients" )
    {
      // kappa is a polynomial in space and time
      const RealArray & pct = db.get<RealArray>("pct"); // polynomial coeff's in time
      const RealArray & pcx = db.get<RealArray>("pcx"); // polynomial coeff's in space

      for( int n=0; n<numberOfComponents; n++ )
      {
        printF("n=%i: pct=%g,%g,%g,  pcx=%g,%g,%g,%g,%g,%g, kappa=%g\n",n,pct(0,n),pct(1,n),pct(2,n), pcx(0,0,0,n),
	       pcx(1,0,0,n),pcx(1,1,0,n), pcx(2,0,0,n), pcx(0,1,0,n),pcx(0,2,0,n), kappa[n]);
	
        real timeFunction = pct(0,n)+t*(pct(1,n)+t*pct(2,n));
	kappaVarLocal(I1,I2,I3,n)=( 
	  pcx(0,0,0,n) 
	  + xLocal(I1,I2,I3,0)*( pcx(1,0,0,n) + pcx(1,1,0,n)*xLocal(I1,I2,I3,1) + pcx(2,0,0,n)*xLocal(I1,I2,I3,0))
	  + xLocal(I1,I2,I3,1)*( pcx(0,1,0,n) + pcx(0,2,0,n)*xLocal(I1,I2,I3,1))
	  )*timeFunction;

	// kappaVarLocal(I1,I2,I3,n)=kappa[n]; // pcx(0,0,0,n);
	
      }
    }
    else
    {
      printF("Cgad::getDiffusionCoefficients:ERROR: Unknown userCoefficientsOption=[%s]\n",
             (const char*)userCoefficientsOption);
      OV_ABORT("error");
    }
    
  } // end for grid 
  
  diffusionCoefficientsAreUpToDate=true;
  
  // parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForGetCoefficients"))+=getCPU()-cpu0;
}




// ==========================================================================================
/// \brief Choose an option that defines variable coefficients.
/// \details This routine is called at startup to allow the user to choose from a list
/// of options that define the advection and diffusion coefficients. 
///
// ==========================================================================================
int AdParameters::
updateUserDefinedCoefficients(GenericGraphicsInterface & gi)
{

  if( ! dbase.get<DataBase >("modelData").has_key("userDefinedCoefficientsData") )
     dbase.get<DataBase >("modelData").put<DataBase>("userDefinedCoefficientsData");
  DataBase & db =  dbase.get<DataBase >("modelData").get<DataBase>("userDefinedCoefficientsData");

  if( !db.has_key("userCoefficientsOption") )
  {
    db.put<aString>("userCoefficientsOption");
    db.get<aString>("userCoefficientsOption")="no user coefficients";

    db.put<bool>("diffusionCoefficientsAreUpToDate");    
    db.put<bool>("advectionCoefficientsAreUpToDate");    

    db.put<real[20]>("rpar");
    db.put<int[20]>("ipar");
  }
  aString & userCoefficientsOption = db.get<aString>("userCoefficientsOption");
  real *rpar = db.get<real[20]>("rpar");
  int *ipar = db.get<int[20]>("ipar");

  bool & variableDiffusivity = dbase.get<bool >("variableDiffusivity");
  bool & variableAdvection = dbase.get<bool >("variableAdvection");

  bool & diffusionCoefficientsAreUpToDate = db.get<bool>("diffusionCoefficientsAreUpToDate");
  diffusionCoefficientsAreUpToDate=false;

  bool & advectionCoefficientsAreUpToDate = db.get<bool>("advectionCoefficientsAreUpToDate");
  advectionCoefficientsAreUpToDate=false;

  // We should indicate whether the coefficients depend on time. 
  bool & diffusivityIsTimeDependent = dbase.get<bool>("diffusivityIsTimeDependent");
  bool & advectionIsTimeDependent = dbase.get<bool>("advectionIsTimeDependent");

  const int & numberOfComponents = dbase.get<int>("numberOfComponents");
  const int & numberOfDimensions = dbase.get<int>("numberOfDimensions");
  std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  
  const aString menu[]=
    {
      "no user coefficients",
      "polynomial coefficients",
      "done",
      ""
    }; 

  gi.appendToTheDefaultPrompt("userDefinedCoefficients>");
  aString answer;
  for( ;; ) 
  {

    int response=gi.getMenuItem(menu,answer,"Choose an option");
    
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="no user coefficients" )
    {
      userCoefficientsOption="no user coefficients";
    }
    else if( answer=="polynomial coefficients" )
    {
      userCoefficientsOption="polynomial coefficients";
      variableDiffusivity=true;
      variableAdvection=true;
      
      printF("User defined coefficients: polynomial coefficients chosen\n");
      gi.inputString(answer,"Enter degreeInTime, degreeinSpace (the degree's of the polynomials in time and space");
      int & degreeInTime=ipar[0], &degreeInSpace=ipar[1];
      
      sScanF(answer,"%i %i",&degreeInTime,&degreeInSpace);
      printf("Setting degreeInTime=%i, degreeInSpace=%i\n",degreeInTime,degreeInSpace);

      // Save the polynomial coefficients in the arrays 
      //   pct(it,n) = coeff of t^it 
      //   pcx(ix,iy,iz,n) = coeff of x^ix * y^iy * z^iz 
      if( !db.has_key("pct") )
      {
        db.put<RealArray>("pct");
        db.put<RealArray>("pcx");

        // advection coefficients: 
        db.put<RealArray>("act");
        db.put<RealArray>("acx");
      }
      RealArray & pct = db.get<RealArray>("pct"); // polynomial coeff's in time
      RealArray & pcx = db.get<RealArray>("pcx"); // polynomial coeff's in space
      const int maxDegree=2;
      pct.redim(maxDegree+1,numberOfComponents); pct=0.;
      pcx.redim(maxDegree+1,maxDegree+1,maxDegree+1,numberOfComponents); pcx=0.;
      for( int n=0; n<numberOfComponents; n++ )
      {
        // choose default coefficients so kappa remains positive 
	pct(0,n)=1.;
	if( degreeInTime>=2 )
	  pct(2,n)=.05;
	 
        // space = kappa[n]*( 1 + .1*x^2 + .15*y^2 )
        pcx(0,0,0,n)=kappa[n];
	if( degreeInSpace>=2 )
	{
	  pcx(2,0,0,n)=.1*kappa[n];
	  pcx(0,2,0,n)=.15*kappa[n];
	}
      }
      
      // For now the advection coefficients do not depend on the component number
      RealArray & act = db.get<RealArray>("act"); // polynomial coeff's in time
      RealArray & acx = db.get<RealArray>("acx"); // polynomial coeff's in space
      act.redim(3,maxDegree+1); act=0.;
      acx.redim(3,maxDegree+1,maxDegree+1,maxDegree+1); acx=0.;
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	act(axis,0)=1.;
	if( degreeInTime>=1  )
	  act(axis,1)=.2*(axis+1);
	if( degreeInTime>=2  )
	  act(axis,2)=.1*(axis+1);
	 
        // space: 
        acx(axis,0,0,0)=1.;
	if( degreeInSpace>=1 )
	{
	  acx(axis,1,0,0)= .2/(1.+axis);
	  acx(axis,0,1,0)=.25/(1.+axis);
	  acx(axis,0,0,1)=-.2/(1.+axis);
	}
	if( degreeInSpace>=2 )
	{
	  acx(axis,2,0,0)= .1/(1.+axis);
	  acx(axis,0,2,0)=.15/(1.+axis);
	  acx(axis,0,0,2)=-.1/(1.+axis);
	}
      }
      
      diffusivityIsTimeDependent=degreeInTime!=0;
      advectionIsTimeDependent=degreeInTime!=0;
    }
    else
    {
      printF("unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }

  gi.unAppendTheDefaultPrompt();
  bool userCoefficientsChosen = userCoefficientsOption!="no user coefficients";
  return userCoefficientsChosen;
}
