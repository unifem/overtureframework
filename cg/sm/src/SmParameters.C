#include "SmParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "GridFunctionFilter.h"

int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);

aString SmParameters::PDEModelName[SmParameters::numberOfPDEModels+1];
aString SmParameters::PDEVariationName[SmParameters::numberOfPDEVariations+1];

aString SmParameters::bcName[numberOfBCNames];

//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in SmParameters}} 
//\no function header:
//
// /int numberOfDimensions: number of spacial dimensions.
//\end{ParametersInclude.tex}
//===================================================================================


//\begin{>>ParametersInclude.tex}{\subsection{Constructor}} 
SmParameters::
SmParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
// ==================================================================================
//
//\end{ParametersInclude.tex}
//===================================================================================
{
  Parameters::pdeName ="solidMechanics";

  int & numberOfComponents = dbase.get<int>("numberOfComponents"); 

  numberOfComponents=numberOfDimensions0;  // default

  if (!dbase.has_key("pdeModel")) dbase.put<PDEModel>("pdeModel");
  dbase.get<PDEModel>("pdeModel")=linearElasticity;
  PDEModelName[linearElasticity]="elasticity";
  PDEModelName[nonlinearMechanics]="non-linear mechanics";
  PDEModelName[numberOfPDEModels]="";

  if (!dbase.has_key("pdeVariation")) dbase.put<PDEVariation>("pdeVariation");
  dbase.get<PDEVariation>("pdeVariation")=nonConservative;
  PDEVariationName[nonConservative]="non-conservative";
  PDEVariationName[conservative]   ="conservative";
  PDEVariationName[godunov]        ="godunov";
  PDEVariationName[hemp]           ="hemp";
  PDEVariationName[numberOfPDEVariations]="";

  if (!dbase.has_key("timeSteppingMethodSm")) dbase.put<TimeSteppingMethodSm>("timeSteppingMethodSm");
  dbase.get<TimeSteppingMethodSm>("timeSteppingMethodSm")=defaultTimeStepping;

  if (!dbase.has_key("uc")) dbase.put<int>("uc");
  if (!dbase.has_key("vc")) dbase.put<int>("vc");
  if (!dbase.has_key("wc")) dbase.put<int>("wc");
  if (!dbase.has_key("rc")) dbase.put<int>("rc");
  if (!dbase.has_key("tc")) dbase.put<int>("tc");

  // methodComputesDisplacements=true : we store the displacements from the reference state
  //                            =false: we store the deformed state positions 
  if (!dbase.has_key("methodComputesDisplacements")) dbase.put<bool>("methodComputesDisplacements",true);

  // These are used by hemp -- we could use these names instead of u,v,w
  if (!dbase.has_key("u1c")) dbase.put<int>("u1c");
  if (!dbase.has_key("u2c")) dbase.put<int>("u2c");
  if (!dbase.has_key("u3c")) dbase.put<int>("u3c");

  // some methods need the velocities: 
  if (!dbase.has_key("v1c")) dbase.put<int>("v1c",-1);
  if (!dbase.has_key("v2c")) dbase.put<int>("v2c",-1);
  if (!dbase.has_key("v3c")) dbase.put<int>("v3c",-1);

  // some methods need the stresses 
  if (!dbase.has_key("s11c")) dbase.put<int>("s11c",-1);
  if (!dbase.has_key("s12c")) dbase.put<int>("s12c",-1);
  if (!dbase.has_key("s13c")) dbase.put<int>("s13c",-1);
  if (!dbase.has_key("s21c")) dbase.put<int>("s21c",-1);
  if (!dbase.has_key("s22c")) dbase.put<int>("s22c",-1);
  if (!dbase.has_key("s23c")) dbase.put<int>("s23c",-1);
  if (!dbase.has_key("s31c")) dbase.put<int>("s31c",-1);
  if (!dbase.has_key("s32c")) dbase.put<int>("s32c",-1);
  if (!dbase.has_key("s33c")) dbase.put<int>("s33c",-1);

  if (!dbase.has_key("pc")) dbase.put<int>("pc",-1);  // for hemp 
  if (!dbase.has_key("qc")) dbase.put<int>("qc",-1);  // for hemp 
  if (!dbase.has_key("hempInitialConditionOption")) dbase.put<aString>("hempInitialConditionOption","default"); 

  if (!dbase.has_key("specialInitialConditionOption")) dbase.put<aString>("specialInitialConditionOption","default"); 
    
//   // the thermalConductivity is used in boundary conditions at domain interfaces: 
//   if (!dbase.has_key("thermalConductivity")) dbase.put<real>("thermalConductivity",-1.);

  if (!dbase.has_key( "stressRelaxation" )) dbase.put<int>( "stressRelaxation",0 );
  if (!dbase.has_key( "relaxAlpha" )) dbase.put<real>( "relaxAlpha",0.1 );
  if (!dbase.has_key( "relaxDelta" )) dbase.put<real>( "relaxDelta",0.0 );

  if (!dbase.has_key("dtSave")) dbase.put<real>("dtSave",-1.); // save dt from getTimeStep

  if (!dbase.has_key("rho")) dbase.put<real>("rho",1.);

  if (!dbase.has_key("mu")) dbase.put<real>("mu");
  if (!dbase.has_key("lambda")) dbase.put<real>("lambda");

  if (!dbase.has_key("muGrid")) dbase.put<RealArray>("muGrid");
  if (!dbase.has_key("lambdaGrid")) dbase.put<RealArray>("lambdaGrid");

  if( !dbase.has_key("tzInterfaceVelocity") )
  { // Here is the (artificial) interface velocity for testing moving interfaces and TZ
    dbase.put<real [3]>("tzInterfaceVelocity");
    real *v0 = dbase.get<real [3]>("tzInterfaceVelocity");
    v0[0]=v0[1]=v0[2]=0.;
  }
  if( !dbase.has_key("tzInterfaceAcceleration") )
  { // Here is the (artificial) interface acceleration for testing moving interfaces and TZ
    dbase.put<real [3]>("tzInterfaceAcceleration");
    real *a0 = dbase.get<real [3]>("tzInterfaceAcceleration");
    a0[0]=a0[1]=a0[2]=0.;
  }
  
  // Names of material properties go here: (Each name should be an entry in the dbase of type real)
  // These are coefficients that can vary over the grid.
  std::vector<aString> & materialPropertyNames = dbase.get<std::vector<aString> >("materialPropertyNames");
  materialPropertyNames.push_back("rho");
  materialPropertyNames.push_back("mu");
  materialPropertyNames.push_back("lambda");

  // Component numbers for material properties (used for TZ functions)
  if (!dbase.has_key("rhoc")) dbase.put<int>("rhoc");
  if (!dbase.has_key("muc")) dbase.put<int>("muc");
  if (!dbase.has_key("lambdac")) dbase.put<int>("lambdac");

  if (!dbase.has_key("gridHasMaterialInterfaces")) dbase.put<bool>("gridHasMaterialInterfaces");

  if (!dbase.has_key("recomputeDt")) dbase.put<bool>("recomputeDt",true);
  
  // keep track of the old dt in case it changes. 
  if (!dbase.has_key("dtOld")) dbase.put<real>("dtOld",-1.);

  if (!dbase.has_key("adjustTimeStep")) dbase.put<bool>("adjustTimeStep",false);

  dbase.get<bool >("twilightZoneFlow")=false; // ****************** do this for now ***************
  
  if (!dbase.has_key("computeTimeSteppingEigenValues")) dbase.put<bool>("computeTimeSteppingEigenValues",false);
  if (!dbase.has_key("realPartOfTimeSteppingEigenValue")) dbase.put<real>("realPartOfTimeSteppingEigenValue",-1.);
  if (!dbase.has_key("imaginaryPartOfTimeSteppingEigenValue")) dbase.put<real>("imaginaryPartOfTimeSteppingEigenValue",-1.);
  // Worst case eigenvalue of the dissipation that has a coefficient proportional to 1/dt : 
  if (!dbase.has_key("dtInverseDissipationEigenvalue")) dbase.put<real>("dtInverseDissipationEigenvalue",0.);
  dbase.get<real>("dtInverseDissipationEigenvalue")=0.;

  if (!dbase.has_key("orderOfAccuracyForGodunovMethod") ) dbase.put<int >("orderOfAccuracyForGodunovMethod",2);
  if (!dbase.has_key("fluxMethodForGodunovMethod") ) dbase.put<int >("fluxMethodForGodunovMethod",0);
  if (!dbase.has_key("slopeLimitingForGodunovMethod") ) dbase.put<int >("slopeLimitingForGodunovMethod",0);
  if (!dbase.has_key("slopeUpwindingForGodunovMethod") ) dbase.put<int >("slopeUpwindingForGodunovMethod",0);
  if (!dbase.has_key("pdeTypeForGodunovMethod") ) dbase.put<int >("pdeTypeForGodunovMethod",0);

  // --- parameters for the Hemp code ---
  if (!dbase.has_key("initialStateGridFunction")) dbase.put<realCompositeGridFunction*>("initialStateGridFunction");
  dbase.get<realCompositeGridFunction*>("initialStateGridFunction")=NULL;
  
  if (!dbase.has_key("Rg")) dbase.put<real>("Rg");
  dbase.get<real>("Rg")=8.314/27.;  // default value for hemp
  
  if (!dbase.has_key("yieldStress")) dbase.put<real>("yieldStress",1.e10);
  if (!dbase.has_key("basePress")) dbase.put<real>("basePress",1.0);
  if (!dbase.has_key("c0Visc")) dbase.put<real>("c0Visc",2.0);
  if (!dbase.has_key("clVisc")) dbase.put<real>("clVisc",1.0);
  if (!dbase.has_key("hgVisc")) dbase.put<real>("hgVisc",4.0e-2);

  if (!dbase.has_key("polyEos"))
  {
    dbase.put<std::vector<real> >("polyEos");
    std::vector<real> & polyEos = dbase.get<std::vector<real> >("polyEos");
    polyEos.resize(4);
    polyEos[0]=1.; polyEos[1]=1.; polyEos[2]=1.; polyEos[3]=1.; 
  }

  if (!dbase.has_key("hourGlassFlag")) dbase.put<int>("hourGlassFlag");
  dbase.get<int>("hourGlassFlag")=2;  // default value for hemp ... this means "regular" diffusion operator
  
  // Fourth-order AD: 
  if( !dbase.has_key("artificialDiffusion4") ) dbase.put<RealArray >("artificialDiffusion4");

  // Dissipation that has a coefficient proportional to 1/dt : 
  if( !dbase.has_key("artificialDiffusion2dt") ) dbase.put<RealArray >("artificialDiffusion2dt");
  if( !dbase.has_key("artificialDiffusion4dt") ) dbase.put<RealArray >("artificialDiffusion4dt");

  // FOS (SVK) artificial dissipation for the "tangential" components of the stress on a face
  //     beta0 + beta1/dt 
  if( !dbase.has_key("tangentialStressDissipation") ) dbase.put<real>("tangentialStressDissipation");
  dbase.get<real>("tangentialStressDissipation")=.1; // *wdh* 2014/05/02 - new default -- .5;  
  if( !dbase.has_key("tangentialStressDissipation1") ) dbase.put<real>("tangentialStressDissipation1");
  dbase.get<real>("tangentialStressDissipation1")=0.;

  // Use new way to extrap. interp. neighbours: 
  if (!dbase.has_key("useNewExtrapInterpNeighbours")) dbase.put<int>("useNewExtrapInterpNeighbours");
  dbase.get<int>("useNewExtrapInterpNeighbours")=1; // set to zero to use old way
  
  // Option to "pin" an edge or corner of a grid
  // pinBoundaryCondition(0:4,numberOfPins) : (grid,side1,side2,side3,pinOption) , side1=0,1 or -1 for edge.
  if( !dbase.has_key("pinBoundaryCondition") ) dbase.put<IntegerArray>("pinBoundaryCondition");
  if( !dbase.has_key("pinValues") ) dbase.put<RealArray>("pinValues");


  bcName[interpolation]="interpolation";
  bcName[displacementBC]="displacementBC";
  bcName[tractionBC]="tractionBC";
  bcName[slipWall]="slipWall";
  bcName[symmetry]="symmetry";
  bcName[interfaceBoundaryCondition]="interfaceBoundaryCondition"; // for the interface between two regions 
  bcName[abcEM2]="abcEM2";     // absorbing BC; Engquist-Majda order 2 
  bcName[abcPML]="abcPML";     // perfectly matched layer
  bcName[abc3]="abc3";           // future absorbing BC
  bcName[abc4]="abc4";           // future absorbing BC
  bcName[abc5]="abc5";           // future absorbing BC
  bcName[rbcNonLocal]="rbcNonLocal";    // radiation BC, non-local
  bcName[rbcLocal]="rbcLocal";        // radiation BC, local
  bcName[dirichletBoundaryCondition]="dirichletBoundaryCondition";

  for( int id=0; id<numberOfBCNames; id++ )
  {
    registerBC(id,bcName[id],true);  // replace existing BC id's 
  }

  // initialize the items that we time: 
  if (!dbase.has_key("cpuInitial")) dbase.put<real>("cpuInitial",0.); // holds initial call to getCPU so we can print current usage anywhere
  initializeTimings();

}

SmParameters::
~SmParameters()
{
  delete dbase.get<realCompositeGridFunction*>("initialStateGridFunction");
}

// ===================================================================================================================
/// \brief Define the items that will be timed (this is a virtual function that may be overloaded by derived classes)
// ===================================================================================================================
int SmParameters::
initializeTimings()
{
  addTiming("totalTime",                           "total time");
  addTiming("timeForInitialize",                   "setup and initialize");
  addTiming("timeForInitialConditions",            "initial conditions");
  addTiming("timeForAdvance",                      "advance");
  addTiming("timeForAdvanceRectangularGrids",      "  advance rectangular grids");
  addTiming("timeForAdvanceCurvilinearGrids",      "  advance curvilinear grids");
  addTiming("timeForAdvanceUnstructuredGrids",     "  advance unstructured grids");
  addTiming("timeForAdvOpt",                       "   (advOpt)");
  addTiming("timeForForcing",                      "  add forcing");
  addTiming("timeForFilter",                       "  filter");
  addTiming("timeForProject",                      "  project    ");
  addTiming("timeForDissipation",                  "  add dissipation");
  addTiming("timeForBoundaryConditions",           "  boundary conditions");

  addTiming("timeForAmrRegrid",                    "  AMR regrid");
  addTiming("timeForAmrErrorFunction",             "    compute error function");
  addTiming("timeForAmrRegridBaseGrids",           "    regrid base grids");
  addTiming("timeForAmrRegridOverlap",             "    regrid overlap");
  addTiming("timeForAmrUpdate",                    "    update grids and functions");
  addTiming("timeForAmrInterpolateRefinements",    "    interpolate refinements");
  addTiming("timeForAmrBoundaryConditions",        "    boundary conditions");
  addTiming("timeForUpdateInterpolant",            "    update interpolant");

  addTiming("timeForInterfaces",                   "  interfaces");
  addTiming("timeForRadiationBC",                  "  radiation bc");
  addTiming("timeForRaditionKernel",               "  radiationKernel");
  addTiming("timeForUpdateGhostBoundaries",        "  update ghost (parallel)");
  addTiming("timeForInterpolate",                  "  interpolation");
  addTiming("timeForComputingDeltaT",              "  compute dt");
  addTiming("timeForGetError",                     "  get errors");
  addTiming("timeForPlotting",                     "  plotting");
  addTiming("timeForShowFile",                     "  showFile");
  addTiming("timeForOther",                        "other");
  addTiming("timeForWaiting",                      "waiting (not counted)");
  addTiming("timeForUnknown",                      "unknown");

  // printF(" SmParameters::initializeTimings: totalTime=%i timeForAdvance=%i\n",totalTime,timeForAdvance);

//   // Here are items from the base class that we just include in other -- they should not contribute
//   timeForGetUt                        =timeForUnknown;
//   timeForAddUt                        =timeForUnknown;
//   timeForMovingGrids                  =timeForUnknown;
//   timeForMovingUpdate                 =timeForUnknown;
//   timeForUpdateOperators              =timeForUnknown;
//   timeForUpdateInterpolant            =timeForUnknown;
//   timeForInterpolateExposedPoints     =timeForUnknown;
//   timeForLineImplicit                 =timeForUnknown;
//   timeForLineImplicitSolve            =timeForUnknown;
//   timeForLineImplicitFactor           =timeForUnknown;
//   timeForLineImplicitResidual         =timeForUnknown;
//   timeForLineImplicitJacobian         =timeForUnknown;
//   timeForLineImplicitSetupA           =timeForUnknown;
//   timeForTimeIndependentVariables     =timeForUnknown;
//   timeForUpdatePressureEquation       =timeForUnknown;
//   timeForPressureSolve                =timeForUnknown;
//   timeForAssignPressureRHS            =timeForUnknown;

  int & maximumNumberOfTimings = dbase.get<int>("maximumNumberOfTimings");
  maximumNumberOfTimings = dbase.get<std::vector<aString> >("timingName").size();

  dbase.get<RealArray >("timing").redim(maximumNumberOfTimings);
  dbase.get<RealArray >("timing")=0.;

  return 0;
}


int SmParameters::
setParameters(const int & numberOfDimensions0 /* =2 */,const aString & reactionName )
// ==================================================================================================
//  /reactionName (input) : optional name of a reaction oe a reaction 
//     file that defines the chemical reactions, such as
//      a Chemkin binary file. 
// ==================================================================================================
{
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");
  PDEVariation & pdeVariation = dbase.get<PDEVariation>("pdeVariation");
  
  int & uc =  dbase.get<int >("uc");
  int & vc =  dbase.get<int >("vc");
  int & wc =  dbase.get<int >("wc");
  int & rc =  dbase.get<int >("rc");
  int & tc =  dbase.get<int >("tc");

  int & u1c =  dbase.get<int >("u1c");
  int & u2c =  dbase.get<int >("u2c");
  int & u3c =  dbase.get<int >("u3c");

  uc=vc=wc=rc=tc=u1c=u2c=u3c=-1;
  
  numberOfDimensions=numberOfDimensions0;
  
  dbase.get<int >("stencilWidthForExposedPoints")=3;
  dbase.get<int >("extrapolateInterpolationNeighbours")=false;


  if( pdeVariation==SmParameters::nonConservative ||
      pdeVariation==SmParameters::conservative )
  {
    numberOfComponents=numberOfDimensions;

    uc=0;
    vc=1;
    wc=numberOfDimensions>2 ? 2 : -1;

    // *wdh* 110705 : we should switch from (u,v,w) to (u1c,u2c,u3c) for the displacement
    u1c = 0;
    u2c = 1;
    u3c = numberOfDimensions>2 ? 2 : -1;

    aString *& componentName = dbase.get<aString* >("componentName");
    delete  componentName;
    componentName= new aString [ numberOfComponents];
    if( uc>=0 ) componentName[uc]="u";
    if( vc>=0 ) componentName[vc]="v";
    if( wc>=0 ) componentName[wc]="w";
    if( rc>=0 ) componentName[rc]="r";
    if( tc>=0 ) componentName[tc]="T";
  

    addShowVariable( "u",uc );
    addShowVariable( "v",vc );
    if( numberOfDimensions>2 )
    {
      addShowVariable( "w",wc );
    }

    addShowVariable("div", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("vor", numberOfComponents+1,false);  // false=turned off by default

    addShowVariable("v1", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("v2", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("v3", numberOfComponents+1,false);  // false=turned off by default

    addShowVariable("s11", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s12", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s13", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s21", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s22", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s23", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s31", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s32", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("s33", numberOfComponents+1,false);  // false=turned off by default

  }
  else if( pdeVariation==SmParameters::godunov )
  {
    // for now Don wants velocity, full stress tensor and displacement 
    numberOfComponents = numberOfDimensions + numberOfDimensions + numberOfDimensions*numberOfDimensions;

    aString *& componentName = dbase.get<aString* >("componentName");
    delete  componentName;
    componentName= new aString [ numberOfComponents];

    int c=0;
    componentName[c]="v1"; addShowVariable( "v1",c );  dbase.get<int >("v1c")=c; c++;
    componentName[c]="v2"; addShowVariable( "v2",c );  dbase.get<int >("v2c")=c; c++;
    if( numberOfDimensions==3 )
    { 
      componentName[c]="v3"; addShowVariable( "v3",c ); dbase.get<int >("v3c")=c; c++; 
    }

    // define stress components s11c, s12c, ... 
    for( int m1=1; m1<=numberOfDimensions; m1++ )for( int m2=1; m2<=numberOfDimensions; m2++ )
    {
      aString cName; sPrintF(cName,"s%i%i",m1,m2);
      componentName[c]=cName; addShowVariable( cName,c ); 
      cName=cName + "c"; dbase.get<int >(cName)=c; c++;
    }

    componentName[c]="u"; uc=c; addShowVariable( "u",c ); c++;
    componentName[c]="v"; vc=c; addShowVariable( "v",c ); c++;
    if( numberOfDimensions==3 )
    {
      componentName[c]="w"; wc=c; addShowVariable( "w",c );  c++;
    }
    
    // *wdh* 110705 : we should switch from (u,v,w) to (u1c,u2c,u3c) for the displacement
    u1c = uc;
    u2c = vc;
    u3c = wc;

    assert( c==numberOfComponents );

    addShowVariable("div", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("vor", numberOfComponents+1,false);  // false=turned off by default


    // The Godunov stencil is 5 points wide: 
    dbase.get<int >("extrapolateInterpolationNeighbours")=true;
    
  }
  else if( pdeVariation==SmParameters::hemp )
  {
    // Jeff Banks wants:
    //   displacement, velocity, p, s11, s12, s22 and q

    numberOfComponents = numberOfDimensions + numberOfDimensions + numberOfDimensions*(numberOfDimensions+1)/2 +2;

    // ** for now add extra space to hold the deformed mesh when solving linear elasticity ***
    bool addExtraSpaceForDeformedMesh=true;
    if( addExtraSpaceForDeformedMesh )
      numberOfComponents+=numberOfDimensions;
    
    aString *& componentName = dbase.get<aString* >("componentName");
    delete  componentName;
    componentName= new aString [ numberOfComponents];

    int c=0;

    componentName[c]="u"; uc=c; addShowVariable( "u",c ); c++;
    componentName[c]="v"; vc=c; addShowVariable( "v",c ); c++;
    if( numberOfDimensions==3 )
    {
      componentName[c]="w"; wc=c; addShowVariable( "w",c );  c++;
    }

    componentName[c]="v1"; addShowVariable( "v1",c ); dbase.get<int >("v1c")=c; c++;
    componentName[c]="v2"; addShowVariable( "v2",c ); dbase.get<int >("v2c")=c; c++;
    if( numberOfDimensions==3 )
    { 
      componentName[c]="v3"; addShowVariable( "v3",c ); dbase.get<int >("v3c")=c; c++; 
    }

    // define stress components s11c, s12c, ... 
    for( int m1=1; m1<=numberOfDimensions; m1++ )for( int m2=m1; m2<=numberOfDimensions; m2++ )
    {
      aString cName; sPrintF(cName,"s%i%i",m1,m2);
      componentName[c]=cName; addShowVariable( cName,c ); cName=cName + "c"; dbase.get<int >(cName)=c; c++;
    }
    // do this: 
    dbase.get<int >("s21c")=dbase.get<int >("s12c");
    dbase.get<int >("s31c")=dbase.get<int >("s13c");
    dbase.get<int >("s32c")=dbase.get<int >("s23c");

    componentName[c]="p"; addShowVariable( "p",c ); dbase.get<int >("pc")=c; c++;
    componentName[c]="q"; addShowVariable( "q",c ); dbase.get<int >("qc")=c; c++;


    // ** for now add extra space to hold the deformed mesh when solving linear elasticity ***
    if( addExtraSpaceForDeformedMesh )
    {
      componentName[c]="u1c"; u1c=c; addShowVariable( "u1",c ); c++;
      componentName[c]="u2c"; u2c=c; addShowVariable( "u2",c ); c++;
      if( numberOfDimensions==3 )
      {
	componentName[c]="u3c"; u3c=c; addShowVariable( "u3",c );  c++;
      }
    }
    

    assert( c==numberOfComponents );
  }
  else 
  {
    printF("SmParameters::setParameters:ERROR: unknown pdeVariation=%i\n",(int)pdeVariation);
    Overture::abort("error");
  }
  
  RealArray & artificialDiffusion2 = dbase.get<RealArray >("artificialDiffusion");
  RealArray & artificialDiffusion4 = dbase.get<RealArray >("artificialDiffusion4");
  RealArray & ad2dt = dbase.get<RealArray >("artificialDiffusion2dt");
  RealArray & ad4dt = dbase.get<RealArray >("artificialDiffusion4dt");
  if( artificialDiffusion2.getLength(0)==0 )  // *** fix me ***
  {
    artificialDiffusion2.redim(numberOfComponents); artificialDiffusion2=0.;
    artificialDiffusion4.redim(numberOfComponents); artificialDiffusion4=0.;
    ad2dt.redim(numberOfComponents); ad2dt=0.;
    ad4dt.redim(numberOfComponents); ad4dt=0.;
  }


  // component numbers for material properties (for TZ)
  dbase.get<int>("rhoc")   =numberOfComponents;
  dbase.get<int>("muc")    =numberOfComponents+1;
  dbase.get<int>("lambdac")=numberOfComponents+2;


  dbase.get<RealArray >("initialConditions").redim( numberOfComponents);  
  dbase.get<RealArray >("initialConditions")=defaultValue;

  dbase.get<RealArray >("checkFileCutoff").redim( numberOfComponents+1);  // cutoff's for errors in checkfile
  dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  
  return 0;
}


// ===================================================================================================================
/// \brief return the name of the time-stepping method
// ==================================================================================================================
aString SmParameters::
getTimeSteppingName() const
{
  SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = dbase.get<TimeSteppingMethodSm>("timeSteppingMethodSm");
  if( timeSteppingMethodSm==defaultTimeStepping )
  {
    return "defaultTimeStepping";
  }
  else if( timeSteppingMethodSm==adamsBashforthSymmetricThirdOrder )
  {
    return "adamsBashforthSymmetricThirdOrder";
  }
  else if( timeSteppingMethodSm==rungeKuttaFourthOrder )
  {
    return "rungeKuttaFourthOrder";
  }
  else if( timeSteppingMethodSm==stoermerTimeStepping )
  {
    return "stoermerTimeStepping";
  }
  else if( timeSteppingMethodSm==modifiedEquationTimeStepping )
  {
    return "modifiedEquationTimeStepping";
  }
  else if( timeSteppingMethodSm==forwardEuler )
  {
    return "forwardEuler";
  }
  else if( timeSteppingMethodSm==improvedEuler )
  {
    return "improvedEuler";
  }
  else if( timeSteppingMethodSm==adamsBashforth2 )
  {
    return "adamsBashforth2";
  }
  else if( timeSteppingMethodSm==adamsPredictorCorrector2 )
  {
    return "adamsPredictorCorrector2";
  }
  else if( timeSteppingMethodSm==adamsPredictorCorrector4 )
  {
    return "adamsPredictorCorrector4";
  }
  else 
  {
    return "unknown";
  }
  
}

// ===============================================================================
/// \brief return true if we are solving a first order system
// ===============================================================================
bool SmParameters::
isFirstOrderSystem() const
{
  SmParameters::PDEVariation & pdeVariation = dbase.get<SmParameters::PDEVariation>("pdeVariation");
  return pdeVariation==godunov || pdeVariation==hemp;
}


// ===============================================================================
/// \brief return true if we are solving a second order system
// ===============================================================================
bool SmParameters::
isSecondOrderSystem() const
{
  SmParameters::PDEVariation & pdeVariation = dbase.get<SmParameters::PDEVariation>("pdeVariation");
  return pdeVariation==nonConservative || pdeVariation==conservative;
}



//\begin{>>SmParametersInclude.tex}{\subsection{setTwilightZoneFunction}} 
int SmParameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
// =============================================================================================
// /Description:
//
// /choice (input): SmParameters::polynomial or SmParameters::trigonometric
//\end{SmParametersInclude.tex}
// =============================================================================================
{
  const int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  const int & uc =  dbase.get<int >("uc");
  const int & vc =  dbase.get<int >("vc");
  const int & wc =  dbase.get<int >("wc");
  const int & rc =  dbase.get<int >("rc");
  const int & tc =  dbase.get<int >("tc");

  const int & u1c = dbase.get<int >("u1c");
  const int & u2c = dbase.get<int >("u2c");
  const int & u3c = dbase.get<int >("u3c");

  const int v1c = dbase.get<int >("v1c");
  const int v2c = dbase.get<int >("v2c");
  const int v3c = dbase.get<int >("v3c");

  bool assignVelocities= v1c>=0 ;
  const int s11c = dbase.get<int >("s11c");
  const int s12c = dbase.get<int >("s12c");
  const int s13c = dbase.get<int >("s13c");
  const int s21c = dbase.get<int >("s21c");
  const int s22c = dbase.get<int >("s22c");
  const int s23c = dbase.get<int >("s23c");
  const int s31c = dbase.get<int >("s31c");
  const int s32c = dbase.get<int >("s32c");
  const int s33c = dbase.get<int >("s33c");
    
  bool assignStress = s11c >=0 ;

  // Material property component numbers:
  const int rhoc = dbase.get<int >("rhoc"); 
  const int muc = dbase.get<int >("muc"); 
  const int lambdac = dbase.get<int >("lambdac"); 

  const int & tzDegreeSpace=  dbase.get<int >("tzDegreeSpace");
  const int & tzDegreeTime=  dbase.get<int >("tzDegreeTime");
  
  SmParameters::PDEVariation & pdeVariation = dbase.get<SmParameters::PDEVariation>("pdeVariation");

  OGFunction *& tz = dbase.get<OGFunction* >("exactSolution");
  Parameters::TwilightZoneChoice & twilightZoneChoice = 
    dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
  
  // Make the TZ solutions for the stress tensor have symmetric components  *wdh* 100826
  const bool useSymmetricStressSolution = true;  

  std::vector<aString> & materialPropertyNames = dbase.get<std::vector<aString> >("materialPropertyNames");
  const int numberOfMaterialProperties=materialPropertyNames.size();
  int numberOfTZComponents = numberOfComponents+numberOfMaterialProperties;
  

  if( twilightZoneChoice==polynomial )
  {
    tz = new OGPolyFunction(tzDegreeSpace,numberOfDimensions,numberOfTZComponents,degreeTime);

    const int ndp=max(max(5,tzDegreeSpace+1),degreeTime+1);
    
    printF("\n $$$$$$$ setTwilightZoneFunction: tzDegreeSpace=%i, degreeTime=%i ndp=%i $$$$\n",
	   tzDegreeSpace,degreeTime,ndp);
    printF(" $$$$$$$ setTwilightZoneFunction: numberOfDimensions=%i, numberOfTZComponents=%i"
           " numberOfMaterialProperties=%i\n",numberOfDimensions,
           numberOfTZComponents,numberOfMaterialProperties);

    RealArray spatialCoefficientsForTZ(ndp,ndp,ndp,numberOfTZComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(ndp,numberOfTZComponents);      
    timeCoefficientsForTZ=0.;


    if( pdeVariation==SmParameters::nonConservative ||
	pdeVariation==SmParameters::conservative )
    {  
      // ---- second-order system ----

      assert( !assignVelocities );
      assert( !assignStress );
      

      if( numberOfDimensions==2 )
      {
	if( tzDegreeSpace==0 )
	{
	  spatialCoefficientsForTZ(0,0,0,uc)=1.;       // u1=1
	  spatialCoefficientsForTZ(0,0,0,vc)=2.;      // u2=2
	}
	else if( tzDegreeSpace==1 )
	{
	  spatialCoefficientsForTZ(0,0,0,uc)=1.;      // u=1+x+y
	  spatialCoefficientsForTZ(1,0,0,uc)=1.;
	  spatialCoefficientsForTZ(0,1,0,uc)=1.;

	  spatialCoefficientsForTZ(0,0,0,vc)= 2.;      // v=2+x-y
	  spatialCoefficientsForTZ(1,0,0,vc)= 1.;
	  spatialCoefficientsForTZ(0,1,0,vc)=-1.;
	}
	else if( tzDegreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;

	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	}
	else if( tzDegreeSpace==3 )
	{
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + .5*y^3 + .25*x^2*y + .2*x^3  - .3*x*y^2
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;
	  spatialCoefficientsForTZ(0,3,0,uc)=.5;
	  spatialCoefficientsForTZ(2,1,0,uc)=.25;
	  spatialCoefficientsForTZ(3,0,0,0,uc)=.2;
	  spatialCoefficientsForTZ(1,2,0,0,uc)=-.3;

	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 -.5*x^3 -.25*x*y^2  -.6*x^2*y + .1*y^3
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	  spatialCoefficientsForTZ(3,0,0,vc)=-.5;
	  spatialCoefficientsForTZ(1,2,0,vc)=-.25;
	  spatialCoefficientsForTZ(2,1,0,vc)=-.6;
	  spatialCoefficientsForTZ(0,3,0,vc)= .1;
	}
	else if( tzDegreeSpace==4 || tzDegreeSpace==5 )
	{
	  if( tzDegreeSpace!=4 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
	  
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;

	  spatialCoefficientsForTZ(4,0,0,uc)=.2;   
	  spatialCoefficientsForTZ(0,4,0,uc)=.5;   
	  spatialCoefficientsForTZ(1,3,0,uc)=1.;   


	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;

	  spatialCoefficientsForTZ(4,0,0,vc)=.125;
	  spatialCoefficientsForTZ(0,4,0,vc)=-.25;
	  spatialCoefficientsForTZ(3,1,0,vc)=-.8;
	}
	else if( tzDegreeSpace>=6 )
	{
	  if( tzDegreeSpace!=6 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
	  
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;

	  spatialCoefficientsForTZ(4,0,0,uc)=.2;   
	  spatialCoefficientsForTZ(0,4,0,uc)=.5;   
	  spatialCoefficientsForTZ(1,3,0,uc)=1.;   

	  spatialCoefficientsForTZ(3,2,0,uc)=.1;      // .1*x^3*y^2

	  spatialCoefficientsForTZ(4,2,0,uc)=.3;      // .3 x^4 y^2 ** III
	  spatialCoefficientsForTZ(3,3,0,uc)=.4;      // .4 x^3 y^3 ** IV 

	  spatialCoefficientsForTZ(6,0,0,uc)=.1;      //  + .1*x^6 +.25*y^6 -.6*x*y^5
	  spatialCoefficientsForTZ(0,6,0,uc)=.25;
	  spatialCoefficientsForTZ(1,5,0,uc)=-.6;


	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;

	  spatialCoefficientsForTZ(2,3,0,vc)=-.1;      // -.1*x^2*y^3

	  spatialCoefficientsForTZ(3,3,0,vc)=-.4;     //-.4 x^3 y^3 ** III 
	  spatialCoefficientsForTZ(2,4,0,vc)=-.3;      //-.3 x^2 y^4 ** IV

	  spatialCoefficientsForTZ(4,0,0,vc)=.125;
	  spatialCoefficientsForTZ(0,4,0,vc)=-.25;
	  spatialCoefficientsForTZ(3,1,0,vc)=-.8;

	  spatialCoefficientsForTZ(6,0,0,vc)=.3;    //   .3*x^6 +.1*y^6  + .6*x^5*y 
	  spatialCoefficientsForTZ(0,6,0,vc)=.1;
	  spatialCoefficientsForTZ(5,1,0,vc)=-.6;
	}
	else
	{
	  printF("Cgsm:: not implemented for degree in space =%i \n",tzDegreeSpace);
	  Overture::abort("error");
	}
      }
      // *****************************************************************
      // ******************* Three Dimensions ****************************
      // *****************************************************************
      else if( numberOfDimensions==3 )
      {
	int degreeSpaceX = tzDegreeSpace;  // do this for now 
	int degreeSpaceY = numberOfDimensions>1 ? tzDegreeSpace : 0;  
	int degreeSpaceZ = numberOfDimensions>2 ? tzDegreeSpace : 0;
      
	if( (degreeSpaceX==0 || degreeSpaceY==0 || degreeSpaceZ==0) 
	    && (degreeSpaceX!=0 || degreeSpaceY!=0 || degreeSpaceZ!=0)  )
	{
	  // For testing we can set the TZ function in 3D to equal that of the 2D function

	  int e1,e2;
	  if( degreeSpaceX==0 )
	  { // here we rotate about the y-axis so (x->z, y->y)
	    e1=wc; e2=vc;
	  }
	  else if( degreeSpaceY==0 )
	  { // here we rotate about the x-axis  (y->z x->x
	    e1=uc; e2=wc;
	  }
	  else
	  {
	    e1=uc; e2=vc;
	  }
	  int degreeSpace2=max(degreeSpaceX,degreeSpaceY,degreeSpaceZ);

	  if( degreeSpace2==1 )
	  {
	    spatialCoefficientsForTZ(0,0,0,e1)=1.;      // u=1+x+y
	    spatialCoefficientsForTZ(1,0,0,e1)=1.;
	    spatialCoefficientsForTZ(0,1,0,e1)=1.;

	    spatialCoefficientsForTZ(0,0,0,e2)= 2.;      // v=2+x-y
	    spatialCoefficientsForTZ(1,0,0,e2)= 1.;
	    spatialCoefficientsForTZ(0,1,0,e2)=-1.;
	  }
	  else if( degreeSpace2==2 )
	  {
	    spatialCoefficientsForTZ(2,0,0,e1)=1.;      // u=x^2 + 2xy + y^2 
	    spatialCoefficientsForTZ(1,1,0,e1)=2.;
	    spatialCoefficientsForTZ(0,2,0,e1)=1.;

	    spatialCoefficientsForTZ(2,0,0,e2)= 1.;      // v=x^2 -2xy - y^2 
	    spatialCoefficientsForTZ(1,1,0,e2)=-2.;
	    spatialCoefficientsForTZ(0,2,0,e2)=-1.;
	  }
	  else if( degreeSpace2==3 )
	  {
	    spatialCoefficientsForTZ(2,0,0,e1)=1.;      // u=x^2 + 2xy + y^2 + .5*y^3 + .25*x^2*y + .2*x^3  - .3*x*y^2
	    spatialCoefficientsForTZ(1,1,0,e1)=2.;
	    spatialCoefficientsForTZ(0,2,0,e1)=1.;
	    spatialCoefficientsForTZ(0,3,0,e1)=.5;
	    spatialCoefficientsForTZ(2,1,0,e1)=.25;
	    spatialCoefficientsForTZ(3,0,0,0,e1)=.2;
	    spatialCoefficientsForTZ(1,2,0,0,e1)=-.3;

	    spatialCoefficientsForTZ(2,0,0,e2)= 1.;      // v=x^2 -2xy - y^2 -.5*x^3 -.25*x*y^2  -.6*x^2*y + .1*y^3
	    spatialCoefficientsForTZ(1,1,0,e2)=-2.;
	    spatialCoefficientsForTZ(0,2,0,e2)=-1.;
	    spatialCoefficientsForTZ(3,0,0,e2)=-.5;
	    spatialCoefficientsForTZ(1,2,0,e2)=-.25;
	    spatialCoefficientsForTZ(2,1,0,e2)=-.6;
	    spatialCoefficientsForTZ(0,3,0,e2)= .1;
	  }
	  else if( degreeSpace2==4 ) 
	  {
	    if( degreeSpaceZ==0 )
	    {
	      spatialCoefficientsForTZ(2,0,0,e1)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	      spatialCoefficientsForTZ(1,1,0,e1)=2.;
	      spatialCoefficientsForTZ(0,2,0,e1)=1.;

	      spatialCoefficientsForTZ(4,0,0,e1)=.2;   
	      spatialCoefficientsForTZ(0,4,0,e1)=.5;   
	      spatialCoefficientsForTZ(1,3,0,e1)=1.;   


	      spatialCoefficientsForTZ(2,0,0,e2)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	      spatialCoefficientsForTZ(1,1,0,e2)=-2.;
	      spatialCoefficientsForTZ(0,2,0,e2)=-1.;

	      spatialCoefficientsForTZ(4,0,0,e2)=.125;
	      spatialCoefficientsForTZ(0,4,0,e2)=-.25;
	      spatialCoefficientsForTZ(3,1,0,e2)=-.8;
	    }
	    else if( degreeSpaceX==0 )// degreeSpaceX==0
	    {
              
	      // switch x->z
	      spatialCoefficientsForTZ(0,0,2,e1)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	      spatialCoefficientsForTZ(0,1,1,e1)=2.;
	      spatialCoefficientsForTZ(0,2,0,e1)=1.;

	      spatialCoefficientsForTZ(0,0,4,e1)=.2;   
	      spatialCoefficientsForTZ(0,4,0,e1)=.5;   
	      spatialCoefficientsForTZ(0,3,1,e1)=1.;   


	      spatialCoefficientsForTZ(0,0,2,e2)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	      spatialCoefficientsForTZ(0,1,1,e2)=-2.;
	      spatialCoefficientsForTZ(0,2,0,e2)=-1.;

	      spatialCoefficientsForTZ(0,0,4,e2)=.125;
	      spatialCoefficientsForTZ(0,4,0,e2)=-.25;
	      spatialCoefficientsForTZ(0,1,3,e2)=-.8;
	    }
	    else  // degreeY==0   
	    {
	      spatialCoefficientsForTZ(2,0,0,e1)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	      spatialCoefficientsForTZ(1,0,1,e1)=2.;
	      spatialCoefficientsForTZ(0,0,2,e1)=1.;

	      spatialCoefficientsForTZ(4,0,0,e1)=.2;   
	      spatialCoefficientsForTZ(0,0,4,e1)=.5;   
	      spatialCoefficientsForTZ(1,0,3,e1)=1.;   


	      spatialCoefficientsForTZ(2,0,0,e2)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	      spatialCoefficientsForTZ(1,0,1,e2)=-2.;
	      spatialCoefficientsForTZ(0,0,2,e2)=-1.;

	      spatialCoefficientsForTZ(4,0,0,e2)=.125;
	      spatialCoefficientsForTZ(0,0,4,e2)=-.25;
	      spatialCoefficientsForTZ(3,0,1,e2)=-.8;
	    }
	    
	  }
	  else
	  {
	    Overture::abort("unimplemented values of degreeSpace");
	  }
	}
	else if( tzDegreeSpace==1 )
	{
	  spatialCoefficientsForTZ(0,0,0,uc)=1.;      // u=1 + x + y + z
	  spatialCoefficientsForTZ(1,0,0,uc)=1.;
	  spatialCoefficientsForTZ(0,1,0,uc)=1.;
	  spatialCoefficientsForTZ(0,0,1,uc)=1.;

	  spatialCoefficientsForTZ(0,0,0,vc)= 2.;      // v=2+x-2y+z
	  spatialCoefficientsForTZ(1,0,0,vc)= 1.;
	  spatialCoefficientsForTZ(0,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,0,1,vc)= 1.;
    
	  spatialCoefficientsForTZ(1,0,0,wc)=-1.;      // w=-x+y+z
	  spatialCoefficientsForTZ(0,1,0,wc)= 1.;
	  spatialCoefficientsForTZ(0,0,1,wc)= 1.;

	}
	else if( tzDegreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;
	  spatialCoefficientsForTZ(1,0,1,uc)=1.;
	  spatialCoefficientsForTZ(0,1,1,uc)=-.25;
	  spatialCoefficientsForTZ(0,0,2,uc)=-.5;
      
	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	  spatialCoefficientsForTZ(0,1,1,vc)=+3.;
	  spatialCoefficientsForTZ(1,0,1,vc)=.25;
	  spatialCoefficientsForTZ(0,0,2,vc)=.5;
      
	  spatialCoefficientsForTZ(2,0,0,wc)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
	  spatialCoefficientsForTZ(0,2,0,wc)= 1.;
	  spatialCoefficientsForTZ(0,0,2,wc)=-2.;
	  spatialCoefficientsForTZ(1,1,0,wc)=.25;
	}
	else if( tzDegreeSpace==0 )
	{
	  spatialCoefficientsForTZ(0,0,0,uc)=1.; // -1.; 
	  spatialCoefficientsForTZ(0,0,0,vc)=1.; //-.5;
	  spatialCoefficientsForTZ(0,0,0,wc)=1.; //.75; 
	}
	else if( tzDegreeSpace==3 )
	{
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + xz 
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;
	  spatialCoefficientsForTZ(1,0,1,uc)=1.;
      
	  spatialCoefficientsForTZ(3,0,0,uc)=.125; 
	  spatialCoefficientsForTZ(0,3,0,uc)=.125; 
	  spatialCoefficientsForTZ(0,0,3,uc)=.125; 
	  spatialCoefficientsForTZ(1,2,0,uc)=-.75;
	  spatialCoefficientsForTZ(2,0,1,uc)=+1.; 
	  spatialCoefficientsForTZ(0,1,1,uc)=.4; 


	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	  spatialCoefficientsForTZ(0,1,1,vc)=+3.;
      
	  spatialCoefficientsForTZ(3,0,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,3,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,0,3,vc)=.25; 
	  spatialCoefficientsForTZ(2,1,0,vc)=-3.*.125; 
	  spatialCoefficientsForTZ(0,1,2,vc)=-3.*.125; 
      
      
	  spatialCoefficientsForTZ(2,0,0,wc)= 1.;      // w=x^2 + y^2 - 2 z^2 
	  spatialCoefficientsForTZ(0,2,0,wc)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
	  spatialCoefficientsForTZ(0,0,2,wc)=-2.;
      
	  spatialCoefficientsForTZ(3,0,0,wc)=.25; 
	  spatialCoefficientsForTZ(0,3,0,wc)=-.2; 
	  spatialCoefficientsForTZ(0,0,3,wc)=.125; 
	  spatialCoefficientsForTZ(1,0,2,wc)=-1.;
	  spatialCoefficientsForTZ(1,2,0,wc)=-.6;
	}
	else if( tzDegreeSpace==4 )
	{
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + xz
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;
	  spatialCoefficientsForTZ(1,0,1,uc)=1.;
	  spatialCoefficientsForTZ(3,0,0,uc)=.5;      // + .5*x^3

	  spatialCoefficientsForTZ(4,0,0,uc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
	  spatialCoefficientsForTZ(0,4,0,uc)=.125;    
	  spatialCoefficientsForTZ(0,0,4,uc)=.125; 
	  spatialCoefficientsForTZ(1,0,3,uc)=-.5; 
	  spatialCoefficientsForTZ(0,1,3,uc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
	  spatialCoefficientsForTZ(0,2,2,uc)=-.25; 
	  spatialCoefficientsForTZ(0,3,1,uc)=.25; 
      
      
	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	  spatialCoefficientsForTZ(0,1,1,vc)=+3.;
      
	  spatialCoefficientsForTZ(2,1,0,vc)=-1.5;     // -1.5x^2*y
      
	  spatialCoefficientsForTZ(4,0,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,4,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,0,4,vc)=.25; 
	  spatialCoefficientsForTZ(3,1,0,vc)=-.5; 
	  spatialCoefficientsForTZ(1,0,3,vc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
	  spatialCoefficientsForTZ(2,0,2,vc)=-.25; 
	  spatialCoefficientsForTZ(3,0,1,vc)=.25; 
      
      
	  spatialCoefficientsForTZ(2,0,0,wc)= 1.;      // w=x^2 + y^2 - 2 z^2
	  spatialCoefficientsForTZ(0,2,0,wc)= 1.;
	  spatialCoefficientsForTZ(0,0,2,wc)=-2.;
      
	  spatialCoefficientsForTZ(4,0,0,wc)=.25; 
	  spatialCoefficientsForTZ(0,4,0,wc)=-.2; 
	  spatialCoefficientsForTZ(0,0,4,wc)=.125; 
	  spatialCoefficientsForTZ(0,3,1,wc)=-1.;
	  spatialCoefficientsForTZ(1,3,0,wc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
	  spatialCoefficientsForTZ(2,2,0,wc)=-.25; 
	  spatialCoefficientsForTZ(3,1,0,wc)=.25; 
	}
	else if( tzDegreeSpace>=5 )
	{
	  if( true || tzDegreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
	  
	  spatialCoefficientsForTZ(2,0,0,uc)=1.;      // u=x^2 + 2xy + y^2 + xz
	  spatialCoefficientsForTZ(1,1,0,uc)=2.;
	  spatialCoefficientsForTZ(0,2,0,uc)=1.;
	  spatialCoefficientsForTZ(1,0,1,uc)=1.;
    
	  spatialCoefficientsForTZ(4,0,0,uc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
	  spatialCoefficientsForTZ(0,4,0,uc)=.125;    
	  spatialCoefficientsForTZ(0,0,4,uc)=.125; 
	  spatialCoefficientsForTZ(1,0,3,uc)=-.5; 
	  spatialCoefficientsForTZ(0,1,3,uc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
	  spatialCoefficientsForTZ(0,2,2,uc)=-.25; 
	  spatialCoefficientsForTZ(0,3,1,uc)=.25; 
    
	  spatialCoefficientsForTZ(0,5,0,uc)=.125;   // y^5
    
    
	  spatialCoefficientsForTZ(2,0,0,vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
	  spatialCoefficientsForTZ(1,1,0,vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0,vc)=-1.;
	  spatialCoefficientsForTZ(0,1,1,vc)=+3.;
    
	  spatialCoefficientsForTZ(4,0,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,4,0,vc)=.25; 
	  spatialCoefficientsForTZ(0,0,4,vc)=.25; 
	  spatialCoefficientsForTZ(3,1,0,vc)=-.5; 
	  spatialCoefficientsForTZ(1,0,3,vc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
	  spatialCoefficientsForTZ(2,0,2,vc)=-.25; 
	  spatialCoefficientsForTZ(3,0,1,vc)=.25; 
    
	  // spatialCoefficientsForTZ(5,0,0,vc)=.125;  // x^5
    
    
	  spatialCoefficientsForTZ(2,0,0,wc)= 1.;      // w=x^2 + y^2 - 2 z^2
	  spatialCoefficientsForTZ(0,2,0,wc)= 1.;
	  spatialCoefficientsForTZ(0,0,2,wc)=-2.;
    
	  spatialCoefficientsForTZ(4,0,0,wc)=.25; 
	  spatialCoefficientsForTZ(0,4,0,wc)=-.2; 
	  spatialCoefficientsForTZ(0,0,4,wc)=.125; 
	  spatialCoefficientsForTZ(0,3,1,wc)=-1.;
	  spatialCoefficientsForTZ(1,3,0,wc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
	  spatialCoefficientsForTZ(2,2,0,wc)=-.25; 
	  spatialCoefficientsForTZ(3,1,0,wc)=.25; 
    
	  // spatialCoefficientsForTZ(5,0,0,wc)=.125;
	}
	else
	{
	  printF("Cgsm:: not implemented for degree in space =%i \n",tzDegreeSpace);
	  Overture::abort("error");
	}



      }
      else
      {
	Overture::abort("ERROR:unimplemented number of dimensions");
      }
    }
    else
    {
      // --------------------------------------------
      // --------- First Order System ---------------
      // --------------------------------------------
      for( int n=0; n< numberOfComponents; n++ )
      {
	const int tzDegreeSpace3 = numberOfDimensions==3 ? tzDegreeSpace : 0;
        for( int m1=0; m1<=tzDegreeSpace; m1++ )for( int m2=0; m2<=tzDegreeSpace; m2++ )for( int m3=0; m3<=tzDegreeSpace3; m3++ )
	{
	  if( (m1+m2+m3)<=tzDegreeSpace )
	  { // choose "random" coefficients
	    spatialCoefficientsForTZ(m1,m2,m3,n)=(pow(-1.,m1+2*m2+3*m3+n) )/(1.+ (.25+n)*m1+m2+(1.5+n)*m3);
	  }
	}
      }
    }
    
    for( int n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<ndp; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
  
    if( useSymmetricStressSolution && isFirstOrderSystem() )
    {
      // Make the TZ stress tensor symmetric: 
      Range all;
      spatialCoefficientsForTZ(all,all,all,s21c)=spatialCoefficientsForTZ(all,all,all,s12c);
      timeCoefficientsForTZ(all,s21c)=timeCoefficientsForTZ(all,s12c);
      if( numberOfDimensions==3 )
      {
	spatialCoefficientsForTZ(all,all,all,s31c)=spatialCoefficientsForTZ(all,all,all,s13c);
	timeCoefficientsForTZ(all,s31c)=timeCoefficientsForTZ(all,s13c);
	spatialCoefficientsForTZ(all,all,all,s32c)=spatialCoefficientsForTZ(all,all,all,s23c);
	timeCoefficientsForTZ(all,s32c)=timeCoefficientsForTZ(all,s23c);
      }
    }

    // -------------------------------------
    // -- Assign TZ material properties ----
    // -------------------------------------

    //  NOTE: rho, mu and lambda must remain positive
    if( tzDegreeSpace==0 )
    {
      spatialCoefficientsForTZ(0,0,0,rhoc   )=dbase.get<real>("rho");   
      spatialCoefficientsForTZ(0,0,0,muc    )=dbase.get<real>("mu");   
      spatialCoefficientsForTZ(0,0,0,lambdac)=dbase.get<real>("lambda"); 
    }
    else if( tzDegreeSpace==1 )
    {  
      spatialCoefficientsForTZ(0,0,0,rhoc)=dbase.get<real>("rho");    // rho=rho0 + .1*x+ .05*y
      spatialCoefficientsForTZ(1,0,0,rhoc)=.1;
      spatialCoefficientsForTZ(0,1,0,rhoc)=.05;

      spatialCoefficientsForTZ(0,0,0,muc)=dbase.get<real>("mu");       // mu = mu0 + .07*x+ .065*y
      spatialCoefficientsForTZ(1,0,0,muc)=.075;
      spatialCoefficientsForTZ(0,1,0,muc)=.065;

      spatialCoefficientsForTZ(0,0,0,lambdac)=dbase.get<real>("lambda");  // lambda=lambda0+ .085*x+ .045*y
      spatialCoefficientsForTZ(1,0,0,lambdac)=.085;
      spatialCoefficientsForTZ(0,1,0,lambdac)=.045;

      if( numberOfDimensions==3 )
      {
	spatialCoefficientsForTZ(0,0,1,rhoc)=.025;
	spatialCoefficientsForTZ(0,0,1,muc) =.0125;
	spatialCoefficientsForTZ(0,0,1,lambdac)=.035;
      }
    }
    else 
    {
      if( tzDegreeSpace>2 )
      {
        // Finish me for higher degree poly's 
	printF("Cgsm::SmParameters:WARNING using degree=2 TZ for material properties instead of %i.\n",tzDegreeSpace);
      }
      

      spatialCoefficientsForTZ(0,0,0,rhoc)=1.;      // rho=1 + .1*x^2+ .05*y^2
      spatialCoefficientsForTZ(2,0,0,rhoc)=.1;
      spatialCoefficientsForTZ(0,2,0,rhoc)=.05;

      spatialCoefficientsForTZ(0,0,0,muc)=2.;      // mu = 2 + .07*x^2+ .065*y^2
      spatialCoefficientsForTZ(2,0,0,muc)=.075;
      spatialCoefficientsForTZ(0,2,0,muc)=.065;

      spatialCoefficientsForTZ(0,0,0,lambdac)=1.5;      // lambda=1.5 + .085*x^2+ .045*y^2
      spatialCoefficientsForTZ(2,0,0,lambdac)=.085;
      spatialCoefficientsForTZ(0,2,0,lambdac)=.045;

      if( numberOfDimensions==3 )
      {
	spatialCoefficientsForTZ(0,0,2,rhoc)=.025;
	spatialCoefficientsForTZ(0,0,2,muc) =.0125;
	spatialCoefficientsForTZ(0,0,2,lambdac)=.035;
      }
    }
    // Material properties do NOT depend on time.
    timeCoefficientsForTZ(0,rhoc   )=1.;
    timeCoefficientsForTZ(0,muc    )=1.;
    timeCoefficientsForTZ(0,lambdac)=1.;

    // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
    ((OGPolyFunction*)tz)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

  }
  // -------------------------------------------------------------------------------------------
  else if( twilightZoneChoice==trigonometric )
  {
    const int nc = numberOfTZComponents; 

    RealArray fx(nc),fy(nc),fz(nc),ft(nc);
    RealArray gx(nc),gy(nc),gz(nc),gt(nc);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude(nc), cc(nc);
    amplitude=1.;
    cc=0.;

    ArraySimpleFixed<real,4,1,1,1> & omega = dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");

    fx=omega[0];
    fy = numberOfDimensions>1 ? omega[1] : 0.;
    fz = numberOfDimensions>2 ? omega[2] : 0.;
    ft = omega[3];

    if( numberOfDimensions==2  )
    {   
      // u1= .5*cos(pi x) cos( pi y )
      // u2= .5*sin(pi x) cos( pi y )
      assert( omega[0]==omega[1] );

      // gx or gy : shift by pi/2 to turn cos() into sin()
      amplitude(uc)=.5;                        
      amplitude(vc)=.5;  gx(vc)=.5/omega[0];  
      
      
      if( assignVelocities )
      {
        // v1c = .75 * sin(pi x) cos( pi y )
        // v2c = .25 * cos(pi x) sin( pi y )
        amplitude(v1c)=.75; gx(v1c)=.5/omega[0]; 
        amplitude(v2c)=.25; gy(v2c)=.5/omega[0]; 
      }
      if( assignStress )
      {
        // s11 = -.5* cos(pi x) cos( pi y )
        // s12 =  .4* sin(pi x) cos( pi y )
        // s21 =  .4* sin(pi x) cos( pi y )
        // s22 =  .6* cos(pi x) sin( pi y )
        amplitude(s11c)=-.5;                      
        amplitude(s12c)= .4; gx(s12c)=.5/omega[0]; 
        amplitude(s21c)= .4; gx(s21c)=.5/omega[0]; 
        amplitude(s22c)= .6; gy(s22c)=.5/omega[0]; 
      }

    }
    else if( numberOfDimensions==3 )
    {
      // u1=    cos(pi x) cos( pi y ) cos( pi z)  
      // u2=.5  cos(pi x) sin( pi y ) cos( pi z)
      // u3=.75 cos(pi x) cos( pi y ) sin( pi z)
	
      if( omega[0]==omega[1] && omega[0]==omega[2] )
      {
	amplitude(uc)=1.; 
	amplitude(vc)=.5;  gy(vc)=.5/omega[1]; 
	amplitude(wc)=.75; gz(wc)=.5/omega[2];

	if( assignVelocities )
	{
	  // v1c = .75 * sin(pi x) cos( pi y ) cos( pi z)
	  // v2c = .25 * cos(pi x) cos( pi y ) sin( pi z)
	  // v3c =-.5  * sin(pi x) sin( pi y ) sin( pi z)
	  amplitude(v1c)=.75; gx(v1c)=.5/omega[0]; 
	  amplitude(v2c)=.25; gz(v2c)=.5/omega[0]; 
	  amplitude(v3c)=-.5; gx(v3c)=.5/omega[0]; gy(v3c)=.5/omega[0]; gz(v3c)=.5/omega[0];
	}
	if( assignStress )
	{
	  // s11 = -.5* cos(pi x) cos( pi y ) cos( pi z)
	  // s12 =  .4* sin(pi x) cos( pi y ) cos( pi z)
	  // s13 =  .6* cos(pi x) cos( pi y ) sin( pi z)
	  // s21 =  .4* sin(pi x) cos( pi y ) cos( pi z)
	  // s22 = -.7* sin(pi x) cos( pi y ) sin( pi z)
	  // s23 = .65* cos(pi x) sin( pi y ) sin( pi z)
	  // s31 =  .6* cos(pi x) cos( pi y ) sin( pi z)
	  // s32 = .65* cos(pi x) sin( pi y ) sin( pi z)
	  // s33 =-.20* sin(pi x) sin( pi y ) sin( pi z)
	  amplitude(s11c)=-.5;                      
	  amplitude(s12c)= .4; gx(s12c)=.5/omega[0]; 
	  amplitude(s13c)= .6; gz(s13c)=.5/omega[0]; 

	  amplitude(s21c)= .4; gx(s21c)=.5/omega[0];
	  amplitude(s22c)=-.7; gx(s22c)=.5/omega[0]; gz(s22c)=.5/omega[0]; 
	  amplitude(s23c)=.65; gy(s23c)=.5/omega[0]; gz(s23c)=.5/omega[0]; 

	  amplitude(s31c)= .6; gz(s31c)=.5/omega[0];
	  amplitude(s32c)=.65; gy(s32c)=.5/omega[0]; gz(s32c)=.5/omega[0]; 
	  amplitude(s33c)=-.2; gx(s33c)=.5/omega[0]; gy(s33c)=.5/omega[0]; gz(s33c)=.5/omega[0]; 
	}

      }
      else if( omega[0]==omega[2] && omega[1]==0 )
      {
	// pseudo 2D case
	gx(wc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
	gz(wc)=.5/omega[2];

	amplitude(uc)=.5;  cc(uc)=.0;
	amplitude(wc)=.5;  cc(wc)=.0;

	if( assignVelocities )
	{
	  OV_ABORT("Setup TZ - finish me");
	}
	if( assignStress )
	{
	  OV_ABORT("Setup TZ - finish me");
	}

      }
      else
      {
	Overture::abort("Invalid values for omega[0..2]");
      }
  
	
    }

    if( useSymmetricStressSolution )
    {
      // Make the TZ stress tensor symmetric: 
      Range all;
      amplitude(s21c)=amplitude(s12c); 
      gx(s21c)=gx(s12c); gy(s21c)=gy(s12c); gz(s21c)=gz(s12c); gt(s21c)=gt(s12c); cc(s21c)=cc(s12c);
      if( numberOfDimensions==3 )
      {
	amplitude(s31c)=amplitude(s13c); 
        gx(s31c)=gx(s13c); gy(s31c)=gy(s13c); gz(s31c)=gz(s13c); gt(s31c)=gt(s13c); cc(s31c)=cc(s13c);
	amplitude(s32c)=amplitude(s23c); 
        gx(s32c)=gx(s23c); gy(s32c)=gy(s23c); gz(s32c)=gz(s23c); gt(s32c)=gt(s23c); cc(s32c)=cc(s23c);
      }
    }

    // -----------------------------------------------
    // -- Assign material properties for TZ testing --
    // -----------------------------------------------

    //  NOTE: rho, mu and lambda must remain positive
    amplitude(rhoc)=.125;   cc(rhoc)=1.;     gx(rhoc)=.5/ omega[0];
    amplitude(muc)=.25;     cc(muc)=2.;      gy(muc)=.5/ omega[1];
    amplitude(lambdac)=.30; cc(lambdac)=1.5; gx(lambdac)=.5/ omega[1];

    // Optionally scale amplitudes: 
    const real & trigonometricTwilightZoneScaleFactor=
      dbase.get<real>("trigonometricTwilightZoneScaleFactor");  // scale factor for Trigonometric TZ
    amplitude *= trigonometricTwilightZoneScaleFactor;
    printF("*** SmParameters:INFO: scaling trig TZ by the factor %9.3e\n",trigonometricTwilightZoneScaleFactor);

    // if( dbase.get<int>("pdeTypeForGodunovMethod")>1  ) // *check me* *wdh* 2013/11/01
    // {
    //   // scale amplitudes for non-linear solid models: 
    //   amplitude *= 1.e-3;
    // }

    // Material properties do NOT depend on time.
    ft(rhoc   )=0.;
    ft(muc    )=0.;
    ft(lambdac)=0.;

    tz = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*)tz)->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*)tz)->setAmplitudes(amplitude);
    ((OGTrigFunction*)tz)->setConstants(cc);

  }
  // -------------------------------------------------------------------------------------------
  else if( twilightZoneChoice==pulse )
  {
    // ******* Pulse function chosen ******
    ArraySimpleFixed<real,9,1,1,1> & pulseData = dbase.get<ArraySimpleFixed<real,9,1,1,1> >("pulseData");

    // printF("Cgsm:setTwilightZoneFunction:INFO: create the OGPulseFunction\n");

    tz  =  new OGPulseFunction( numberOfDimensions, numberOfTZComponents, 
                                pulseData[0],pulseData[1],pulseData[2],pulseData[3],pulseData[4],pulseData[5],
			        pulseData[6],pulseData[7],pulseData[8]); 
  }
  else
  {
    printF("assignInitialConditions:ERROR:unknown value for twilightZoneOption=%i\n",(int)twilightZoneChoice);
    OV_ABORT("assignInitialConditions:ERROR");
  }
    



//   TwilightZoneChoice choice=choice_;
//   int & numberOfComponents     = dbase.get<int>("numberOfComponents");
  
//   //TODO: add TZ for passive scalar=passivec
//   if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
//   {
//     printF("Parameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
//            "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
//   }

//   delete  dbase.get<OGFunction* >("exactSolution");
//   if( choice==polynomial )
//   {
//     // ******* polynomial twilight zone function ******
//      dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, dbase.get<int >("numberOfDimensions"), numberOfComponents,degreeTime);

//     Range R5(0,4);
//     RealArray spatialCoefficientsForTZ(5,5,5, numberOfComponents);  
//     spatialCoefficientsForTZ=0.;
//     RealArray timeCoefficientsForTZ(5, numberOfComponents);      
//     timeCoefficientsForTZ=0.;


//     // default case:
//     for( int n=0; n< numberOfComponents; n++ )
//     {
//       real ni =1./(n+1);
    
//       spatialCoefficientsForTZ(0,0,0,n)=2.+n;      
//       if( degreeSpace>0 )
//       {
// 	spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
// 	spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
// 	spatialCoefficientsForTZ(0,0,1,n)=  dbase.get<int >("numberOfDimensions")==3 ? .25*ni : 0.;
//       }
//       if( degreeSpace>1 )
//       {
// 	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
// 	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
// 	spatialCoefficientsForTZ(0,0,2,n)=  dbase.get<int >("numberOfDimensions")==3 ? .125*ni : 0.;

// 	if( false ) // *wdh* 050610
// 	{
// 	  // add cross terms
// 	  printF("\n\n ************* add cross terms to TZ ************** \n\n");
	    

// 	  spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
// 	  if(  dbase.get<int >("numberOfDimensions")>2 )
// 	  {
// 	    spatialCoefficientsForTZ(1,0,1,n)=.1*ni;
// 	    spatialCoefficientsForTZ(0,1,1,n)=-.15*ni;
// 	  }
	    
// 	}
	  
//       }
//     }

//     for( int n=0; n< numberOfComponents; n++ )
//     {
//       for( int i=0; i<=4; i++ )
//       {
// 	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
//       }
	  
//     }
  
//     // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
//     ((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
  
//   }
//   else if( choice==trigonometric ) // ******* Trigonometric function chosen ******
//   {
//     RealArray fx( numberOfComponents),fy( numberOfComponents),fz( numberOfComponents),ft( numberOfComponents);
//     RealArray gx( numberOfComponents),gy( numberOfComponents),gz( numberOfComponents),gt( numberOfComponents);
//     gx=0.;
//     gy=0.;
//     gz=0.;
//     gt=0.;
//     RealArray amplitude( numberOfComponents), cc( numberOfComponents);
//     amplitude=1.;
//     cc=0.;

//     fx= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
//     fy =  dbase.get<int >("numberOfDimensions")>1 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1] : 0.;
//     fz =  dbase.get<int >("numberOfDimensions")>2 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2] : 0.;
//     ft =  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3];

//      dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
    
//     ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
//     ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
//     ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
      
//   }
//   else if( choice==pulse ) 
//   {
//     // ******* Pulse function chosen ******
//      dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( dbase.get<int >("numberOfDimensions"), numberOfComponents); 

//     // this pulse function is not divergence free!

//   }
    
  
  
  return 0;
}

// ===================================================================================
/// \brief Utility routine to read coefficients 
// ===================================================================================
int SmParameters::
readCoefficients( DialogData & dialog, const aString & answer, const aString & name, RealArray & coeff )
{
  
  int len=0;
  aString buff;
  if( len=answer.matches(name) )
  {
    const int maxNum=30;        // assume at most this many components for now
    RealArray ad(maxNum); 
    ad=0.;
    int m;
    for( m=0; m< min(maxNum,dbase.get<int >("numberOfComponents")); m++ )
      ad(m)= coeff(m);
    sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e",
	   &ad(0),&ad(1),&ad(2),&ad(3),&ad(4),&ad(5),&ad(6),&ad(7),&ad(8),&ad(9),
	   &ad(10),&ad(11),&ad(12),&ad(13),&ad(14),&ad(15),&ad(16),&ad(17),&ad(18),&ad(19),
	   &ad(20),&ad(21),&ad(22),&ad(23),&ad(24),&ad(25),&ad(26),&ad(27),&ad(28),&ad(29));

    if(  dbase.get<int >("numberOfComponents")>maxNum )
    {
      printF("setPdeParameters:WARNING:Only reading the first %i %s coefficients. Other values will be set to 1.\n"
	     "                :Get Bill to fix this\n",maxNum,(const char*)name);
    }
   
    aString text;
    for( m=0; m<dbase.get<int >("numberOfComponents"); m++ )
    {
      if( m<maxNum )
	coeff(m)=ad(m);
      else
	coeff(m)=1.;  // default value
	
      printF("Setting %s coefficient for component %s to %8.2e\n",
	     (const char*)name, (const char*) dbase.get<aString* >("componentName")[m],coeff(m));
	
      text+=sPrintF(buff, "%g ", coeff(m));
    }
    dialog.setTextLabel(name,text);
  }
  else
  {
    printF(" SmParameters::readCoefficients:ERROR: inconsistent answer=[%s] and name=[%s]\n",
	   (const char*)answer,(const char*)name);
   
    OV_ABORT("ERROR: This should not happen.");
  }
  
  return 0;
}




int SmParameters::
setPdeParameters(CompositeGrid & cg, const aString & command /* = nullString */,
                 DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//   Prompt for changes in the PDE parameters.
// =====================================================================================
{
  int returnValue=0;

  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "SMPDE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  int & stressRelaxation = dbase.get<int>( "stressRelaxation" );
  real & relaxAlpha = dbase.get<real>( "relaxAlpha" );
  real & relaxDelta = dbase.get<real>( "relaxDelta" );

  real & rho= dbase.get<real>("rho");
  real & mu = dbase.get<real>("mu");
  real & lambda = dbase.get<real>("lambda");
  RealArray & muGrid = dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = dbase.get<RealArray>("lambdaGrid");
  bool & gridHasMaterialInterfaces = dbase.get<bool>("gridHasMaterialInterfaces");
  PDEVariation & pdeVariation = dbase.get<PDEVariation>("pdeVariation");

  const int & u1c = dbase.get<int>("u1c");
  const int & u2c = dbase.get<int>("u2c");
  const int & u3c = dbase.get<int>("u3c");

  real & Rg = dbase.get<real>("Rg");
  real & yieldStress = dbase.get<real>("yieldStress");
  real & basePress = dbase.get<real>("basePress");
  real & c0Visc = dbase.get<real>("c0Visc");
  real & clVisc = dbase.get<real>("clVisc");
  real & hgVisc = dbase.get<real>("hgVisc");
  std::vector<real> & polyEos = dbase.get<std::vector<real> >("polyEos");
  int & hourGlassFlag = dbase.get<int>("hourGlassFlag");

  int & fluxMethod = dbase.get<int>("fluxMethodForGodunovMethod");
  int & slopeLimiting = dbase.get<int>("slopeLimitingForGodunovMethod");
  int & slopeUpwinding = dbase.get<int>("slopeUpwindingForGodunovMethod");
  int & pdeTypeForGodunovMethod = dbase.get<int>("pdeTypeForGodunovMethod");

  real & tangentialStressDissipation = dbase.get<real>("tangentialStressDissipation");
  real & tangentialStressDissipation1 = dbase.get<real>("tangentialStressDissipation1");
  real displacementDissipation=0.;
  real displacementDissipation1=0.;

  aString answer,line;
  char buff[100];
  //  const int numberOfDimensions = cg.numberOfDimensions();

  RealArray & artificialDiffusion = dbase.get<RealArray >("artificialDiffusion");
  RealArray & artificialDiffusion4 = dbase.get<RealArray >("artificialDiffusion4");
  RealArray & ad2dt = dbase.get<RealArray >("artificialDiffusion2dt");
  RealArray & ad4dt = dbase.get<RealArray >("artificialDiffusion4dt");

    
  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    dialog.setWindowTitle("Cgsm parameters");

    // ----- Text strings ------
    const int numberOfTextStrings=31;
    aString textCommands[numberOfTextStrings];
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;

    textLabels[nt]="lambda"; sPrintF(textStrings[nt], "%g",lambda); nt++; 

    textLabels[nt]="mu"; sPrintF(textStrings[nt], "%g",mu); nt++; 

    textLabels[nt]="rho"; sPrintF(textStrings[nt], "%g",rho); nt++; 

    textLabels[nt]="coefficients"; sPrintF(textStrings[nt], "%g %g %s (lambda,mu,grid-name)",lambda,mu,"all"); nt++; 

    if( true || pdeVariation==godunov ) // *wdh* always add these so we don't get warning messages about labels not found
    {
      textLabels[nt]="Godunov order of accuracy";
      sPrintF(textStrings[nt], "%i",dbase.get<int >("orderOfAccuracyForGodunovMethod")); nt++; 
      textLabels[nt]="flux method for Godunov";     sPrintF(textStrings[nt], "%i",dbase.get<int >("fluxMethodForGodunovMethod"));      nt++; 
      textLabels[nt]="slope limiting for Godunov";  sPrintF(textStrings[nt], "%i",dbase.get<int >("slopeLimitingForGodunovMethod"));   nt++; 
      textLabels[nt]="slope upwinding for Godunov"; sPrintF(textStrings[nt], "%i",dbase.get<int >("slopeUpwindingForGodunovMethod"));  nt++; 
      textLabels[nt]="PDE type for Godunov";        sPrintF(textStrings[nt], "%i",dbase.get<int >("pdeTypeForGodunovMethod"));         nt++; 
      textLabels[nt]="stressRelaxation";            sPrintF(textStrings[nt], "%i",dbase.get<int >("stressRelaxation"));                nt++;
      textLabels[nt]="relaxAlpha";                  sPrintF(textStrings[nt], "%g",dbase.get<real>("relaxAlpha"));                      nt++;
      textLabels[nt]="relaxDelta";                  sPrintF(textStrings[nt], "%g",dbase.get<real>("relaxDelta"));                      nt++;

      textLabels[nt]="tangential stress dissipation";  sPrintF(textStrings[nt], "%g, %g",tangentialStressDissipation,tangentialStressDissipation1);  nt++; 
      textLabels[nt]="displacement dissipation";  sPrintF(textStrings[nt], "%g %g",displacementDissipation,displacementDissipation1);  nt++; 

      real *v0 = dbase.get<real [3]>("tzInterfaceVelocity");
      textLabels[nt]="TZ interface velocity";  sPrintF(textStrings[nt], "%g %g %g",v0[0],v0[1],v0[2]);  nt++; 
      real *a0 = dbase.get<real [3]>("tzInterfaceAcceleration");
      textLabels[nt]="TZ interface acceleration";  sPrintF(textStrings[nt], "%g %g %g",a0[0],a0[1],a0[2]);  nt++; 
    }
    if( true || pdeVariation==hemp )  // *wdh* always add these so we don't get warning messages about labels not found
    {
      textLabels[nt]="Rg"; sPrintF(textStrings[nt], "%g",Rg); nt++; 
      textLabels[nt]="yield stress"; sPrintF(textStrings[nt], "%g",yieldStress); nt++;
      textLabels[nt]="base pressure"; sPrintF(textStrings[nt], "%g",basePress); nt++; 
      textLabels[nt]="c0 viscosity"; sPrintF(textStrings[nt], "%g",c0Visc); nt++; 
      textLabels[nt]="cl viscosity"; sPrintF(textStrings[nt], "%g",clVisc); nt++; 
      textLabels[nt]="hg viscosity"; sPrintF(textStrings[nt], "%g",hgVisc); nt++; 
      textLabels[nt]="EOS polynomial"; sPrintF(textStrings[nt], "%g %g %g %g",polyEos[0],polyEos[1],polyEos[2],polyEos[3]); nt++; 
      textLabels[nt]="hourglass control"; sPrintF(textStrings[nt], "%i",hourGlassFlag); nt++; 
    }
  
    aString buff;
    textLabels[nt] = "artificial diffusion";  textStrings[nt]=""; 
    for( int m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      textStrings[nt]+=sPrintF(buff, "%g ",artificialDiffusion(m)); 
    nt++;
    textLabels[nt] = "fourth-order artificial diffusion";  textStrings[nt]=""; 
    for( int m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      textStrings[nt]+=sPrintF(buff, "%g ",artificialDiffusion4(m)); 
    nt++;

    textLabels[nt] = "second-order dt dissipation";  textStrings[nt]=""; 
    for( int m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      textStrings[nt]+=sPrintF(buff, "%g ",ad2dt(m)); 
    nt++;

    textLabels[nt] = "fourth-order dt dissipation";  textStrings[nt]=""; 
    for( int m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      textStrings[nt]+=sPrintF(buff, "%g ",ad4dt(m)); 
    nt++;

    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  

    addPrefix(textLabels,prefix,textCommands,numberOfTextStrings); // add the prefix to the commands 
    dialog.setTextBoxes(textCommands, textLabels, textStrings);

    if( executeCommand ) return 0;
  }

  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("pde parameters>");  
  }
  int len;
  for(int it=0; ; it++)
  {
    if( !executeCommand )
    {
      gi.getAnswer(answer,"");
    }
    else
    {
      if( it==0 ) 
	answer=command;
      else
	break;
    }

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);   // strip off the prefix

    // printf("setPdeParameters: answer=[%s]\n",(const char*)answer);
  

    if( len=answer.matches("lambda") )
    {
      sScanF(answer(len,answer.length()),"%e",&lambda);
      lambdaGrid=lambda;
      dialog.setTextLabel("lambda",sPrintF("%g",lambda));
    }
    else if( len=answer.matches("mu") ) 
    {
      sScanF(answer(len,answer.length()),"%e",&mu);
      muGrid=mu;
      dialog.setTextLabel("mu",sPrintF("%g",mu));
    }
    else if( dialog.getTextValue(answer,"stressRelaxation","%i",stressRelaxation) )
    {
      printF("Stress relaxation=%i : 0 = off\n"
             "                       2 = second order version\n"
             "                       4 = fourth order version\n",stressRelaxation);
    }
    
    else if( dialog.getTextValue(answer,"relaxAlpha","%e",relaxAlpha) ){}//
    else if( dialog.getTextValue(answer,"relaxDelta","%e",relaxDelta) ){}//
    else if( dialog.getTextValue(answer,"rho","%e",rho) ){}//
    else if( len=answer.matches("Godunov order of accuracy") ) 
    {
      sScanF(answer(len,answer.length()),"%i",&dbase.get<int >("orderOfAccuracyForGodunovMethod"));
      if( pdeVariation==godunov )
	dialog.setTextLabel("Godunov order of accuracy",sPrintF("%i",
                             dbase.get<int >("orderOfAccuracyForGodunovMethod")));
    }
    else if( dialog.getTextValue(answer,"Rg","%e",Rg) ){}//
    else if( dialog.getTextValue(answer,"yield stress","%e",yieldStress) ){}//
    else if( dialog.getTextValue(answer,"base pressure","%e",basePress) ){}//
    else if( dialog.getTextValue(answer,"c0 viscosity","%e",c0Visc) ){}//
    else if( dialog.getTextValue(answer,"cl viscosity","%e",clVisc) ){}//
    else if( dialog.getTextValue(answer,"hg viscosity","%e",hgVisc) ){}//
    else if( dialog.getTextValue(answer,"flux method for Godunov","%i",fluxMethod) ){}//
    else if( dialog.getTextValue(answer,"slope limiting for Godunov","%i",slopeLimiting) ){}//
    else if( dialog.getTextValue(answer,"slope upwinding for Godunov","%i",slopeUpwinding) ){}//
    else if( dialog.getTextValue(answer,"PDE type for Godunov","%i",pdeTypeForGodunovMethod) )
    {
      printF("Setting `PDE type for Godunov' to %i.\n"
             "  0 = linear elasticity. \n"
             "  1 = Nonlinear model in linear mode \n"
             "  2 = Saint-Venant Kirkoff model.\n"
             "  3 = Saint-Venant Kirkoff model with rotated linear stress-strain.\n",pdeTypeForGodunovMethod);

      if( !dbase.has_key("pdeNameModifier") )
        dbase.put<aString>("pdeNameModifier");
      aString & pdeNameModifier = dbase.get<aString>("pdeNameModifier");
      if( pdeTypeForGodunovMethod==0 )
        pdeNameModifier="linear-elasticity";
      else if( pdeTypeForGodunovMethod==1 )
        pdeNameModifier="nonlinear model in linear mode";
      else if( pdeTypeForGodunovMethod>1 )
        pdeNameModifier="nonlinear model"; 
    }
    else if( len=answer.matches("tangential stress dissipation") )
    {
      sScanF(answer(len,answer.length()),"%e %e",&tangentialStressDissipation,&tangentialStressDissipation1);
      printF("Setting coefficients of tangential stress dissipatio, beta0+beta1/dt  to beta0=%9.3e, beta1=%9.3e\n",
	     tangentialStressDissipation,tangentialStressDissipation1);
    }
    
    else if( len=answer.matches("displacement dissipation") )
    {
      if( u1c>=0 )
      {
	sScanF(answer(len,answer.length()),"%e %e",&displacementDissipation,&displacementDissipation1);
	printF("Setting coefficients of 4th-order displacement dissipation, beta0+beta1/dt to beta0=%9.3e, beta1=%9.3e\n",
	       displacementDissipation,displacementDissipation1);
	artificialDiffusion4(u1c)=displacementDissipation;
	artificialDiffusion4(u2c)=displacementDissipation;
	if( cg.numberOfDimensions()>2 )
	  artificialDiffusion4(u3c)=displacementDissipation;

        ad4dt(u1c)=displacementDissipation1;
	ad4dt(u2c)=displacementDissipation1;
	if( cg.numberOfDimensions()>2 )
	  ad4dt(u3c)=displacementDissipation1;
      }
    }

    else if( len=answer.matches("TZ interface velocity") )
    {
       real *v0 = dbase.get<real [3]>("tzInterfaceVelocity");
       sScanF(answer(len,answer.length()),"%e %e %e",&v0[0],&v0[1],&v0[2]);
       printF("Setting the interface velocity for (%g,%g,%g) for TZ with a moving interface.\n",v0[0],v0[1],v0[2]);
    }
    else if( len=answer.matches("TZ interface acceleration") )
    {
       real *a0 = dbase.get<real [3]>("tzInterfaceAcceleration");
       sScanF(answer(len,answer.length()),"%e %e %e",&a0[0],&a0[1],&a0[2]);
       printF("Setting the interface acceleration for (%g,%g,%g) for TZ with a moving interface.\n",a0[0],a0[1],a0[2]);
    }
    else if( len=answer.matches("EOS polynomial") ) 
    {
      sScanF(answer(len,answer.length()),"%e %e %e %e",&polyEos[0],&polyEos[1],&polyEos[2],&polyEos[3]);
    }
    else if( len=answer.matches("artificial diffusion") )
    {
      readCoefficients( dialog, answer,"artificial diffusion",artificialDiffusion);

      // RealArray & artificialDiffusion = dbase.get<RealArray >("artificialDiffusion");
      
      // const int maxNum=30;        // assume at most this many components for now
      // RealArray ad(maxNum); 
      // ad=0.;
      // int m;
      // for( m=0; m< min(maxNum,dbase.get<int >("numberOfComponents")); m++ )
      // 	ad(m)= artificialDiffusion(m);
      // sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e",
      //        &ad(0),&ad(1),&ad(2),&ad(3),&ad(4),&ad(5),&ad(6),&ad(7),&ad(8),&ad(9),
      // 	     &ad(10),&ad(11),&ad(12),&ad(13),&ad(14),&ad(15),&ad(16),&ad(17),&ad(18),&ad(19),
      // 	     &ad(20),&ad(21),&ad(22),&ad(23),&ad(24),&ad(25),&ad(26),&ad(27),&ad(28),&ad(29));

      // if(  dbase.get<int >("numberOfComponents")>maxNum )
      // {
      // 	printF("setPdeParameters:WARNING:Only reading the first %i artificial diffusion parameters. Other values will be set to 1.\n"
      //          "                :Get Bill to fix this\n",maxNum);
      // }
   
      // aString text;
      // for( m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      // {
      // 	if( m<maxNum )
      //     artificialDiffusion(m)=ad(m);
      //   else
      //     artificialDiffusion(m)=1.;  // default value
	
      //   printF("Setting Godunov constant-coefficient artficial diffusion for component %s to %8.2e\n",
      // 	       (const char*) dbase.get<aString* >("componentName")[m],artificialDiffusion(m));
	
      // 	text+=sPrintF(buff, "%g ", artificialDiffusion(m));
      // }
      // dialog.setTextLabel("artificial diffusion",text);

    }

    else if( len=answer.matches("fourth-order artificial diffusion") )
    {
      readCoefficients( dialog, answer,"fourth-order artificial diffusion",artificialDiffusion4);

      // RealArray & artificialDiffusion4 = dbase.get<RealArray >("artificialDiffusion4");
      
      // const int maxNum=30;        // assume at most this many components for now
      // RealArray ad(maxNum); 
      // ad=0.;
      // int m;
      // for( m=0; m< min(maxNum,dbase.get<int >("numberOfComponents")); m++ )
      // 	ad(m)= artificialDiffusion4(m);
      // sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e",
      //        &ad(0),&ad(1),&ad(2),&ad(3),&ad(4),&ad(5),&ad(6),&ad(7),&ad(8),&ad(9),
      // 	     &ad(10),&ad(11),&ad(12),&ad(13),&ad(14),&ad(15),&ad(16),&ad(17),&ad(18),&ad(19),
      // 	     &ad(20),&ad(21),&ad(22),&ad(23),&ad(24),&ad(25),&ad(26),&ad(27),&ad(28),&ad(29));

      // if(  dbase.get<int >("numberOfComponents")>maxNum )
      // {
      // 	printF("setPdeParameters:WARNING:Only reading the first %i fourth-order artificial diffusion parameters. Other values will be set to 1.\n"
      //          "                :Get Bill to fix this\n",maxNum);
      // }
   
      // aString text;
      // for( m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      // {
      // 	if( m<maxNum )
      //     artificialDiffusion4(m)=ad(m);
      //   else
      //     artificialDiffusion4(m)=1.;  // default value
	
      //   printF("Setting Godunov constant-coefficient fourth-order artficial diffusion for component %s to %8.2e\n",
      // 	       (const char*) dbase.get<aString* >("componentName")[m],artificialDiffusion4(m));
	
      // 	text+=sPrintF(buff, "%g ", artificialDiffusion4(m));
      // }
      // dialog.setTextLabel("fourth-order artificial diffusion",text);
    }

    else if( len=answer.matches("second-order dt dissipation") )
    {
      readCoefficients( dialog, answer,"second-order dt dissipation",ad2dt );
    }
    else if( len=answer.matches("fourth-order dt dissipation") )
    {
      readCoefficients( dialog, answer,"fourth-order dt dissipation",ad4dt );
    }
    
    else if( dialog.getTextValue(answer,"hourglass control","%i",hourGlassFlag) )
    {
      printF(" INFO: hourGlassFlag: \n"
             "             1 : ??               \n"
             "             2 : regular diffusion\n");
    }
    else if( len=answer.matches("coefficients") )
    {
      // **** assign different values of mu and lambda for different grids ***

      char *buff = new char [answer.length()];
      sScanF(answer(len,answer.length()),"%e %e %s",&lambda,&mu,buff);
      aString gridName=buff;
      delete [] buff;
      printF("coefficients:  lambda=%e, mu=%e, grid-name=[%s]\n",lambda,mu,(const char*)gridName);
      
      // ** Range G=cg.numberOfComponentGrids();

      // New: Allow for wild cards of the file name*
      
      std::vector<int> gridsToCheck;
      if( gridName=="all" )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  gridsToCheck.push_back(grid);
	}
      }
      else if( gridName[gridName.length()-1]=='*' )
      {
	// wild card: final char is a '*'
        printF(" INFO: looking for a wild card match since the final character is a '*' ...\n");
        bool found=false;
        gridName=gridName(0,gridName.length()-2); // remove trailing '*'
	
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          // printF(" Check [%s] matches [%s] \n",(const char*)gridName,(const char*)cg[grid].getName());
	  if( cg[grid].getName().matches(gridName) )
	  {
	    gridsToCheck.push_back(grid);
	    printF(" -- (wild card match) Set coefficients for grid=%i (%s) to lambda=%8.2e mu=%8.2e\n",grid,
               (const char*)cg[grid].getName(),lambda,mu);
	    
            found=true;
	  }
	}
        if( !found )
	{
	  printF("WARNING: No match for the wildcard name [%s*]\n",(const char*)gridName);
	  continue;
	}
      }
      else
      {
        bool found=false;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( cg[grid].getName()==gridName )
	  {
	    gridsToCheck.push_back(grid);
            found=true;
	    break;
	  }
	}
        if( !found )
	{
	  printF("ERROR looking for the grid named [%s]\n",(const char*)gridName);
	  gi.stopReadingCommandFile();
	  continue;
	}
      }
      
      if( gridsToCheck.size()>=1 )
        gridHasMaterialInterfaces=true;
      printF(" **** setting gridHasMaterialInterfaces=true ****\n");
      for( int g=0; g<gridsToCheck.size(); g++ )
      {
        int grid=gridsToCheck[g];
	
	MappedGrid & mg = cg[grid];
	const IntegerArray & bc = mg.boundaryCondition();
	const IntegerArray & share = mg.sharedBoundaryFlag();
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    if( share(side,axis)>=100 ) // **** for now -- material interfaces have share > 100
	    {
	      bc(side,axis)=interfaceBoundaryCondition;

	      printF(" ++++ setting bc(%i,%i) on grid=%i = interfaceBoundaryCondition\n",side,axis,grid);
		
	    }
	  }
	}
	
	lambdaGrid(grid)=lambda;
	muGrid(grid)=mu;

      }

      dialog.setTextLabel("coefficients",sPrintF("%g %g %s (lambda,mu,grid-name)",
              lambda,mu,"all"));

    }
    else
    {
      if( executeCommand )
      {
 	returnValue= 1;  // when executing  dbase.get<real >("a") single command, return 1 if the command was not recognised.
	break;
      }
      else
      {
 	printF("Unknown response=[%s]\n",(const char*)answer);
 	gi.stopReadingCommandFile();
      }
     
    }

  }

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

 return returnValue;

}



int SmParameters::
displayPdeParameters(FILE *file /* = stdout */ )
// =====================================================================================
// /Description:
//   Display PDE parameters
// =====================================================================================
{
  const char *offOn[2] = { "off","on" };
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  fprintf(file,
	  "PDE parameters: equation is `solid mechanics'.\n");

  // The  dbase.get<DataBase >("modelParameters") will be displayed here:
  Parameters::displayPdeParameters(file);

  fprintf(file,
	  "  number of components is %i\n",
	  numberOfComponents);

//   std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
//   std::vector<real> & a = dbase.get<std::vector<real> >("a");
//   std::vector<real> & b = dbase.get<std::vector<real> >("b");
//   std::vector<real> & c = dbase.get<std::vector<real> >("c");
//   for( int n=0; n<4; n++ )
//   {
//     std::vector<real> & par = n==0 ? kappa : n==1 ? a : n==2 ? b : c;
//     aString name = n==0 ? "kappa" : n==1 ? "a" : n==2 ? "b" : "c";
//     for( int m=0; m<numberOfComponents; m++ )
//     {
//       if( numberOfComponents==1 )
// 	fprintf(file," %s=%g",(const char*)name,par[m]);
//       else
// 	fprintf(file," %s[%i]=%g,",(const char*)name,m,par[m]);
//     }
//     fprintf(file,"\n");
//   }

  return 0;
}




//\begin{>>SmParametersInclude.tex}{\subsection{updateShowFile}} 
int SmParameters::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{SmParametersInclude.tex}  
// =================================================================================================
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  real & mu = dbase.get<real>("mu");
  real & lambda = dbase.get<real>("lambda");
  RealArray & muGrid = dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = dbase.get<RealArray>("lambdaGrid");

  showFileParams.push_back(ShowFileParameter("lambda",lambda));
  showFileParams.push_back(ShowFileParameter("mu",mu));

  // *new* way: always save displacement parameters
  showFileParams.push_back(ShowFileParameter("u1Component",dbase.get<int>("u1c")));
  showFileParams.push_back(ShowFileParameter("u2Component",dbase.get<int>("u2c")));
  showFileParams.push_back(ShowFileParameter("u3Component",dbase.get<int>("u3c")));

  // Now save parameters common to all solvers:
  Parameters::saveParametersToShowFile();    

  return 0;
}


int
SmParameters::
updateToMatchGrid(CompositeGrid & cg, IntegerArray & sharedBoundaryCondition )
{
  Parameters::updateToMatchGrid(cg, sharedBoundaryCondition);

  real & mu = dbase.get<real>("mu");
  real & lambda = dbase.get<real>("lambda");
  RealArray & muGrid = dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = dbase.get<RealArray>("lambdaGrid");

  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  int oldNumber= muGrid.getLength(0);
  int newNumber= cg.numberOfComponentGrids();

  lambdaGrid.resize(numberOfComponentGrids);
  muGrid.resize(numberOfComponentGrids);

  // assign values on all refinement grids to equal the values from the base grid.
  for( int grid=0; grid<newNumber; grid++ )  
  {
    if( cg.refinementLevelNumber(grid)>0 ) // this is a refinement grid 
    {
      int baseGrid = cg.baseGridNumber(grid);
      lambdaGrid(grid)=lambdaGrid(baseGrid);
      muGrid(grid)=muGrid(baseGrid);

    }
  }


// From cnsParameters: 
//   // !!! kkc fudge to make implicit cns code work with amr
//   if (  dbase.get<PDE>("pde")==compressibleNavierStokes &&  dbase.get<Parameters::ImplicitMethod >("implicitMethod")!=notImplicit )
//     dbase.get<IntegerArray >("gridIsImplicit") = 1;

//   Range all;
//   if( (  dbase.get<PDE>("pde")==compressibleNavierStokes &&  
// 	 dbase.get<CnsParameters::PDEVariation >("pdeVariation")==conservativeGodunov &&  
// 	 dbase.get<int >("numberOfSpecies")>0 ) ||
//       dbase.get<PDE>("pde")==compressibleMultiphase )
//   {
//     if(  dbase.get<realCompositeGridFunction* >("truncationError")==NULL )
//       {
// 	dbase.get<realCompositeGridFunction* >("truncationError") = new realCompositeGridFunction(cg,all,all,all);
//       }
//     else
//       dbase.get<realCompositeGridFunction* >("truncationError")->updateToMatchGrid(cg,all,all,all);
    
//     (* dbase.get<realCompositeGridFunction* >("truncationError"))=0.;
    
//   }

}

