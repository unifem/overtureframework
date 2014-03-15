#include "InsParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "PenaltySlipWallBC.h"
#include "PenaltyWallFunction.h"

int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);

aString InsParameters::PDEModelName[InsParameters::numberOfPDEModels+1];



//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in InsParameters}} 
//\no function header:
//
// /int numberOfDimensions: number of spacial dimensions.
// /PDE pde: one of
//
// /int numberOfComponents: number of components in the equations. 
// /real cfl, cflMin, cflOpt, cflMax: parameters to determine the time step.
// /int rc: if rc$>$0 then the density is {\tt u(all,all,all,rc)}.
// /int uc: if uc$>$0 then the x component of the velocity is {\tt u(all,all,all,uc)}.
// /int vc: if vc$>$0 then the y component of the velocity is {\tt u(all,all,all,vc)}.
// /int wc: if wc$>$0 then the z component of the velocity is {\tt u(all,all,all,wc)}.
// /int pc: if pc$>$0 then the pressure is {\tt u(all,all,all,pc)}.
// /int tc:   temperature
// /int sc:   position of first species, species m is located at sc+m
// /int kc, epsc: for k-epsilon model
//
// /real machNumber, reynoldsNumber, prandtlNumber: PDE parameters CNS and ASF
// /real mu, kThermal, Rg, gamma, avr, anu: for CNS, ASF
// /real pressureLevel, nuRho: for ASF
//
// 
// /enum TurbulenceModel turbulenceModel: One of
//  \begin{verbatim}
//   enum TurbulenceModel
//   {
//     noTurbulenceModel,
//     BaldwinLomax,
//     kEpsilon,
//     kOmega,
//     SpalartAllmaras,
//     LargeEddySimulation=5,
//     numberOfTurbulenceModels
//   };
// \end{verbatim}
//
// {\bf Boundary condition parameters:}
// /IntegerArray bcInfo(0:2,side,axis,grid): Array holding info about the parameters. The values are accessed
//    through member functions {\tt bcType(side,axis,grid)}, {\tt variableBoundaryData(side,axis,grid)}
//    and {\tt bcIsTimeDependent(side,axis,grid)}
//   \begin{description}
//      \item[bcInfo(0,side,axis,grid)] : values from enum BoundaryConditionType, e.g. uniformInflow
//      \item[bcInfo(1,side,axis,grid)] : bit flag, bit 1=BC is spatially dependent, bit 2=BC is time dependent.
//   \end{description}
// /RealArray bcData: bcData(.,side,axis,grid) : data for the boundary condition
//
// % IntegerArray bcType  bcType(side,axis,grid) : values from enum BoundaryConditionType
// /real inflowPressure:
// /RealArray bcParameters:  arrays for boundary condition parameters
// /IntegerArray variableBoundaryData: variableBoundaryData(grid) is true if variable BC data is required.
//
//\end{ParametersInclude.tex}
//===================================================================================


//\begin{>>ParametersInclude.tex}{\subsection{Constructor}} 
InsParameters::
InsParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
// ==================================================================================
// /pde0: Indicated which PDE we are solving
//
//\end{ParametersInclude.tex}
//===================================================================================
{
  Parameters::pdeName ="incompressibleNavierStokes";
  if (!dbase.has_key("pdeModel")) dbase.put<InsParameters::PDEModel>("pdeModel");
  dbase.get<InsParameters::PDEModel >("pdeModel")=standardModel;
  if (!dbase.has_key("extrapolatePoissonSolveInTime")) dbase.put<bool >("extrapolatePoissonSolveInTime",true);

  if (!dbase.has_key("numberOfImplicitVelocitySolvers")) dbase.put<int>("numberOfImplicitVelocitySolvers");
  dbase.get<int>("numberOfImplicitVelocitySolvers")=0;

  if (!dbase.has_key("implicitSolverForTemperature")) dbase.put<int>("implicitSolverForTemperature");
  dbase.get<int>("implicitSolverForTemperature")=-1;

  if (!dbase.has_key("implicitVariation")) dbase.put<InsParameters::ImplicitVariation>("implicitVariation");
  dbase.get<InsParameters::ImplicitVariation>("implicitVariation")=implicitViscous;

  if (!dbase.has_key("vsc")) dbase.put<int>("vsc");  // coefficient of (nonlinear) viscosity
  if (!dbase.has_key("nc")) dbase.put<int>("nc");  // coefficient of (nonlinear) viscosity

  // the thermalConductivity is used in boundary conditions at domain interfaces: 
  if (!dbase.has_key("thermalConductivity")) dbase.put<real>("thermalConductivity",-1.);

  if (!dbase.has_key("rho")) dbase.put<real>("rho",1.);
  if (!dbase.has_key("Cp"))  dbase.put<real>("Cp",1.);

  // Names of material properties go here: (Each name should be an entry in the dbase of type real)
  // These are coefficients that can vary over the grid.
  std::vector<aString> & materialPropertyNames = dbase.get<std::vector<aString> >("materialPropertyNames");
  materialPropertyNames.push_back("rho");
  materialPropertyNames.push_back("Cp");
  materialPropertyNames.push_back("thermalConductivity");

  // Component numbers for material properties (used for TZ functions)
  if (!dbase.has_key("rhoc")) dbase.put<int>("rhoc");
  if (!dbase.has_key("Cpc")) dbase.put<int>("Cpc");
  if (!dbase.has_key("thermalConductivityc")) dbase.put<int>("thermalConductivityc");


  if ( !dbase.has_key("useBoundaryDissipationInAFScheme")) dbase.put<bool >("useBoundaryDissipationInAFScheme",false);
  if ( !dbase.has_key("stabilizeHighOrderBoundaryConditions")) dbase.put<bool >("stabilizeHighOrderBoundaryConditions",true);

  // outflow option:
  //   0 = extrapolate (u,v,w) and set div(u)=0. 
  //   1 = Neumann BC for (u,v,w)
  if (!dbase.has_key("outflowOption")) dbase.put<int>("outflowOption");
  dbase.get<int>("outflowOption")=0;

  // a parameter used when forming the implicit system: 
  if (!dbase.has_key("fillCoefficientsScalarSystem")) dbase.put<int>("fillCoefficientsScalarSystem",0);

  // A parameter for LES models used in getLargeEddyViscosity
  // if (!dbase.has_key("largeEddySimulationOption")) dbase.put<int>("largeEddySimulationOption",-1);

  PDEModelName[standardModel]="standard model";
  PDEModelName[BoussinesqModel]="Boussinesq model";
  PDEModelName[viscoPlasticModel]="visco-plastic model";
  PDEModelName[twoPhaseFlowModel]="two-phase flow model";
  PDEModelName[numberOfPDEModels]="";

  registerBC((int)outflow,"outflow");
  registerBC((int)inflowWithVelocityGiven,"inflowWithVelocityGiven");
  registerBC((int)inflowWithPressureAndTangentialVelocityGiven,"inflowWithPressureAndTangentialVelocityGiven");
  registerBC((int)tractionFree,"tractionFree");

  registerBC((int)convectiveOutflow,"convectiveOutflow");

  registerBC((int)inflowOutflow,"inflowOutflow"); // *wdh* 2011/08/27

  registerBCModifier("penaltySlipWall",createPenaltySlipWallBC);
  registerBCModifier("penaltyWallFunction",createPenaltyWallFunctionBC);

  dbase.get<bool>("useLineSolver") = true; // this only matters when the steady-state line solver is activated

  // *wdh* 090726 -- use this for implicit time stepping: (otherwise there can be trouble with BE)
  dbase.get<int >("orderOfExtrapolationForOutflow")=2; 

  if (!dbase.has_key("discretizationOption")) dbase.put<InsParameters::DiscretizationOptions>("discretizationOption",InsParameters::standardFiniteDifference);

  // initialize the items that we time: 
  initializeTimings();
}

InsParameters::
~InsParameters()
{
}


// ===================================================================================================================
/// \brief Return true if we should save the linearized solution for implicit methods.
/// \details If implicit operator is non-linear then we may need to save the solution we
///           linearize about.
// ==================================================================================================================
bool InsParameters::
saveLinearizedSolution()
{
  const Parameters::TimeSteppingMethod & timeSteppingMethod = 
    dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
  return dbase.get<int>("useNewImplicitMethod")==1 && 
         timeSteppingMethod==Parameters::implicit &&
         dbase.get<InsParameters::ImplicitVariation>("implicitVariation")!=implicitViscous;
}


// ===================================================================================================================
/// \brief Define the dependent variables uc,vc,... and show file variables.
/// \details 
/// \aString reactionName (input) : optional name of a reaction of a reaction file that defines the chemical 
///          reactions, such as a Chemkin binary file.        
// ==================================================================================================================
int InsParameters::
setParameters(const int & numberOfDimensions0 /* =2 */ , 
              const aString & reactionName_ /* =nullString */ )
{
  PDEModel & pdeModel = dbase.get<InsParameters::PDEModel >("pdeModel");
  TurbulenceModel & turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
 

  int & numberOfDimensions=dbase.get<int >("numberOfDimensions");
  int & numberOfComponents = dbase.get<int >("numberOfComponents");
  int & numberOfSpecies = dbase.get<int >("numberOfSpecies");
  int & rc = dbase.get<int >("rc");
  int & uc = dbase.get<int >("uc");
  int & vc = dbase.get<int >("vc");
  int & wc = dbase.get<int >("wc");
  int & pc = dbase.get<int >("pc");
  int & tc = dbase.get<int >("tc");
  int & sc = dbase.get<int >("sc");
  int & kc = dbase.get<int >("kc");
  int & epsc = dbase.get<int >("epsc");
  int & sec = dbase.get<int >("sec");
  int & vsc = dbase.get<int >("vsc");
  int & nc = dbase.get<int >("nc");

  Range & Rt = dbase.get<Range >("Rt");       // time dependent components
  Range & Rtimp = dbase.get<Range >("Rtimp"); // time dependent components that may be treated implicitly
  
  aString *& componentName = dbase.get<aString* >("componentName");
  
  
  numberOfDimensions=numberOfDimensions0;
  rc= uc= vc= wc= pc= tc= sc= kc= epsc= sec= vsc= nc= -1;
  
  dbase.get<aString >("reactionName")=reactionName_;
  if(  dbase.get<aString >("reactionName")!=nullString &&  dbase.get<aString >("reactionName")!="" )
  {
    dbase.get<bool >("computeReactions")=true;
    // This next function will assign the number of species and build  dbase.get<real >("a") reaction object.
    buildReactions();
  }
  else
  {
    dbase.get<bool >("computeReactions")=false;
    dbase.get<Reactions* >("reactions")=NULL;
    numberOfSpecies=0;
    
  }
  
  int s, i;
  //...set component index'es, showVariables, etc. that are equation-specific

  //case generalizedNavierStokes: //== non-Newt. viscosity
  numberOfComponents=0;
  pc= numberOfComponents++;   //  pressure = u(all,all,all, pc)
  uc= numberOfComponents++;    
  if( numberOfDimensions>1 )  vc= numberOfComponents++;
  if( numberOfDimensions>2 )  wc= numberOfComponents++;
  dbase.get<Range >("Ru")=Range(uc,uc+numberOfDimensions-1);    // velocity components


  const bool addTemperatureEquation= pdeModel==BoussinesqModel ||  pdeModel==viscoPlasticModel;

  if( addTemperatureEquation )
  {
    tc= numberOfComponents++;
  }


  // -----------------------------------------------------
  // --- Specify additional time-dependent components  ---
  // -----------------------------------------------------

  if( pdeModel==twoPhaseFlowModel )
  {
    tc=numberOfComponents++;        // VOF
    nc=numberOfComponents++;        // level-set 
  }
  else if( turbulenceModel==kEpsilon ||  
	   turbulenceModel==kOmega )
  {
    kc= numberOfComponents++;
    epsc= numberOfComponents++;
    vsc = numberOfComponents++; // store nu + nuT here 
    Rt=Range(uc,epsc);          // time dependent components
  }
  else if( turbulenceModel==SpalartAllmaras )
  {
    kc= numberOfComponents++;
    Rt=Range(uc,kc);          // time dependent components
  }
  else if( turbulenceModel==BaldwinLomax )
  {
    // zero-equation model
    kc= numberOfComponents++;  // save the coefficient of viscosity here 
    nc=kc; // new way??
    vsc=kc; // new way??
    Rt=Range(uc,uc+numberOfDimensions-1);   
  }
  else if( turbulenceModel==LargeEddySimulation )
  {
    // time dependent components:
    if( addTemperatureEquation )
      Rt=Range(uc,uc+numberOfDimensions);
    else
      Rt=Range(uc,uc+numberOfDimensions-1);
  }
  

  if( dbase.get<bool >("advectPassiveScalar") )
  {
    numberOfSpecies=1;
  }
  
  if( numberOfSpecies>0 )
  {
    sc= numberOfComponents;    //  sc markes the first species
    numberOfComponents+= numberOfSpecies;
    Rt=Range( uc, numberOfComponents-1);      // time dependent components
  }
  else if( turbulenceModel==noTurbulenceModel )
    Rt=Range( uc, numberOfComponents-1);      // time dependent components

  assert( Rt.length()>0 );

  Rtimp= Rt;
  if( pdeModel==twoPhaseFlowModel )
  {
    Rtimp=Range(uc,tc);  // these are the components that are treated implicitly (if we use inmplicit time stepping)
  }
  

  // time independent variables:
  if( pdeModel==viscoPlasticModel )
  {
    vsc= numberOfComponents++;      // save the coefficient of viscosity (eta)
  }
  else if( pdeModel==twoPhaseFlowModel )
  {
    rc=numberOfComponents++;        // density (not evolved in time)
    vsc= numberOfComponents++;      // save the coefficient of viscosity 
  }
  else if( turbulenceModel==LargeEddySimulation )
  {
    vsc= numberOfComponents++;      // save the coefficient of viscosity 
  }
  
  // component numbers for material properties (for TZ)
  dbase.get<int>("rhoc")   =numberOfComponents;
  dbase.get<int>("Cpc")    =numberOfComponents+1;
  dbase.get<int>("thermalConductivityc")=numberOfComponents+2;
  

//     equationNumber.redim(numberOfComponents);
//     for (i=0; i<numberOfComponents; i++) equationNumber(i) = i;

  // *NOTE* For restarting we assume the first ` numberOfComponents' variables are saved in the  dbase.get<Ogshow* >("show") file
  // in the same order as they are saved in grid functions.

  addShowVariable( "p", pc );
  addShowVariable( "u", uc );
  if(  numberOfDimensions>1 )
    addShowVariable( "v", vc );
  if(  numberOfDimensions>2 )
    addShowVariable( "w", wc );

  if( turbulenceModel==kEpsilon )
  {
    addShowVariable( "k", kc );
    addShowVariable( "eps", epsc );
  }
  else if( turbulenceModel==kOmega )
  {
    addShowVariable( "k", kc );
    addShowVariable( "omega", epsc );
  } 
  else if( turbulenceModel==LargeEddySimulation )
  {
    addShowVariable( "nu", vsc );
  }
  
   
  if ( dbase.get<bool >("advectPassiveScalar")) 
    addShowVariable( "passive", sc);
    
  if( addTemperatureEquation )
    addShowVariable("T", tc);

  if( pdeModel==viscoPlasticModel )
  {
    addShowVariable("eta", vsc); 
    addShowVariable("yield", numberOfComponents+1);
    addShowVariable("eDot", numberOfComponents+1);

    addShowVariable("sigmaxx", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("sigmaxy", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("sigmaxz", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("sigmayy", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("sigmayz", numberOfComponents+1,false);  // false=turned off by default
    addShowVariable("sigmazz", numberOfComponents+1,false);  // false=turned off by default
  }

  if( numberOfDimensions<3 )
    addShowVariable( "vorticity", numberOfComponents+1,false ); // false=turned off by default
  else
  {
    addShowVariable( "vorticityX", numberOfComponents+1,false );
    addShowVariable( "vorticityY", numberOfComponents+1,false );
    addShowVariable( "vorticityZ", numberOfComponents+1,false );
  }
  addShowVariable( "divergence", numberOfComponents+1,false );
  addShowVariable( "speed", numberOfComponents+1,false );          // false=turned off by default
  addShowVariable( "minimumScale", numberOfComponents+1,false );   // false=turned off by default
  addShowVariable( "minimumScale1", numberOfComponents+1,false );  // false=turned off by default
  addShowVariable( "minimumScale2", numberOfComponents+1,false );  // false=turned off by default


  if( componentName )  delete  [] componentName;
  componentName= new aString [numberOfComponents];

  if( rc>=0 ) componentName[rc]="r";
  if( uc>=0 ) componentName[uc]="u";
  if( vc>=0 ) componentName[vc]="v";
  if( wc>=0 ) componentName[wc]="w";
  if( pc>=0 ) componentName[pc]="p";
  if( tc>=0 ) componentName[tc]="T";
  if(vsc>=0 )
  {
    if( pdeModel==viscoPlasticModel )
      componentName[vsc]="eta";
    else
      componentName[vsc]="nu";
  }
  
  if( kc>=0 )
  {
    if( turbulenceModel==SpalartAllmaras )
    {
      componentName[kc]="n";  // for nuT
    }
    else
    {
      componentName[kc]="k";
      componentName[vsc]="nut";
    }
  }
  
  if( epsc>=0 )  componentName[epsc]="eps";

  if( pdeModel==twoPhaseFlowModel )
  {
    componentName[tc]="psi";       // VOF
    componentName[nc]="phi";        // level-set 
    componentName[vsc]="nu";        // viscosity 
  }


  int scp =  sc;
  if( dbase.get<bool >("advectPassiveScalar") )
  {
    componentName[scp]="s";   // use "s" as  dbase.get<real >("a") name for now, "passive";
  }

  
  if( sec>=0 )  componentName[sec]="Tb";

  if( dbase.get<int >("numberOfExtraVariables")>0 )
  {
    aString buff;
    for( int e=0; e< dbase.get<int >("numberOfExtraVariables"); e++ )
    {
      int n= numberOfComponents- dbase.get<int >("numberOfExtraVariables")+e;
      componentName[n]=sPrintF(buff,"Var%i",e);
      addShowVariable(  componentName[n],n );
    }

  }
  
  // For methods with wider stencils we need to interpolate more exposed points for moving grids
  if(  dbase.get<int >("orderOfAccuracy")>=4 || // kkc 101116 changed == to >= for testing 6th order compact ops
       pdeModel==viscoPlasticModel )   // *wdh* 080411
  {
    dbase.get<int >("stencilWidthForExposedPoints")=5;
  }
  else
  {
    dbase.get<int >("stencilWidthForExposedPoints")=3;
  }

  // ** warning: use[Fourth/Sixth]OrderArtificialDiffusion are probably not set here yet
  if( ( dbase.get<int >("orderOfAccuracy")==2 && numberOfGhostPointsNeeded()>=2 ) ||
      ( dbase.get<int >("orderOfAccuracy")==4 && numberOfGhostPointsNeeded()>=3 ) )
  {
    dbase.get<int >("extrapolateInterpolationNeighbours")=true;
  }
  else
  {
    dbase.get<int >("extrapolateInterpolationNeighbours")=false;
  }

  dbase.get<real >("inflowPressure")=1.;
  dbase.get<RealArray >("initialConditions").redim( numberOfComponents);  dbase.get<RealArray >("initialConditions")=defaultValue;
  
  dbase.get<RealArray >("checkFileCutoff").redim( numberOfComponents+1);  // cutoff's for errors in checkfile
  dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  //  dbase.get<RealArray >("checkFileCutoff").display("checkFileCutOff");
  
  // specify values to be assigned to unused points (in fixupUnusedPoints)
  typedef vector<real> realVector;
   dbase.get<DataBase >("modelParameters").put<realVector>("unusedValue");
  realVector & unusedValue =  dbase.get<DataBase >("modelParameters").get<realVector>("unusedValue");
  unusedValue.resize( numberOfComponents,0.);
  
  if( turbulenceModel==kEpsilon )
  {
    unusedValue[kc]=1.;   // set unused values for k and epsilon to be positive
    unusedValue[epsc]=1.;
    unusedValue[vsc]=1.;
  }

  return 0;
}


//\begin{>>InsParametersInclude.tex}{\subsection{setTwilightZoneFunction}} 
int InsParameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
// =============================================================================================
// /Description:
//
// /choice (input): InsParameters::polynomial or InsParameters::trigonometric
//\end{InsParametersInclude.tex}
// =============================================================================================
{

  TwilightZoneChoice choice=choice_;
  
  //TODO: add TZ for passive scalar=passivec
  if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
  {
    printF("InsParameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
           "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
  }

  delete  dbase.get<OGFunction* >("exactSolution");

  const int numberOfDimensions=dbase.get<int >("numberOfDimensions");
  const int numberOfComponents=dbase.get<int >("numberOfComponents");
  const int dimensionOfTZFunction=dbase.get<int >("dimensionOfTZFunction");

  const TurbulenceModel turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  const PDEModel & pdeModel = dbase.get<InsParameters::PDEModel >("pdeModel");
  
  const int uc = dbase.get<int >("uc");
  const int vc = dbase.get<int >("vc");
  const int wc = dbase.get<int >("wc");
  const int pc = dbase.get<int >("pc");
  const int sc = dbase.get<int >("sc");
  const int tc = dbase.get<int >("tc");
  const int rc = dbase.get<int >("rc");
  const int kc = dbase.get<int >("kc");
  const int nc = dbase.get<int >("nc");
  const int epsc = dbase.get<int >("epsc");


  // Include variable material properties
  std::vector<aString> & materialPropertyNames = dbase.get<std::vector<aString> >("materialPropertyNames");
  const int numberOfMaterialProperties=materialPropertyNames.size();
  const int numberOfTZComponents = numberOfComponents+numberOfMaterialProperties;

  // Material property component numbers:
  const int rhoc      = dbase.get<int >("rhoc"); 
  const int Cpc       = dbase.get<int >("Cpc"); 
  const int thermalKc = dbase.get<int >("thermalConductivityc"); 

  if( choice==polynomial )
  {
    // ******* polynomial twilight zone function ******
     dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, numberOfDimensions, numberOfTZComponents,degreeTime);

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, numberOfTZComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, numberOfTZComponents);      
    timeCoefficientsForTZ=0.;


    if(  turbulenceModel==kEpsilon ||  turbulenceModel==kOmega )
    {
      // k, eps and  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega") should remain positive: nuT = cMu*k*k/eps
      spatialCoefficientsForTZ(0,0,0, kc)=.5;
      spatialCoefficientsForTZ(0,0,0, epsc)=2.;
      
      if(  numberOfDimensions>1 )
      {
        // spatialCoefficientsForTZ(0,1,0, kc)=.2;
        // spatialCoefficientsForTZ(0,1,0, epsc)=.1;
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0, kc)=.2;
	  spatialCoefficientsForTZ(0,2,0, kc)=.3;
	  spatialCoefficientsForTZ(2,0,0, epsc)=.8;
	  spatialCoefficientsForTZ(0,2,0, epsc)=.6;
	}
      }
      if(  numberOfDimensions>2 )
      {
        // spatialCoefficientsForTZ(0,0,1, kc)=.15;
        // spatialCoefficientsForTZ(0,0,1, epsc)=.25;
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(0,0,2, kc)=.5;
	  spatialCoefficientsForTZ(0,0,2, epsc)=.5;
	}
      }
    }
    else if(  turbulenceModel==SpalartAllmaras )
    {
      // nuT should remain positive
      const int nc= kc;
      spatialCoefficientsForTZ(0,0,0,nc)=.5;
      spatialCoefficientsForTZ(1,0,0,nc)=0.;
      spatialCoefficientsForTZ(0,1,0,nc)=0.;
      spatialCoefficientsForTZ(0,0,1,nc)=0.;
      
      if(  numberOfDimensions>1 )
      {
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0,nc)=.25; // .4;
	  spatialCoefficientsForTZ(0,2,0,nc)=.15; // .6;
	}
        // Do no add  dbase.get<real >("a") linear term since this could cause the viscosity coeff to be negative
//      else if( degreeSpace==1 )
//   	{
//   	  spatialCoefficientsForTZ(1,0,0,nc)=.1;
//   	  spatialCoefficientsForTZ(0,1,0,nc)=.1;
//   	}
      }
      if(  numberOfDimensions>2 )
      {
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(0,0,2,nc)=.5;
	}
//          else if( degreeSpace==1 )
//  	{
//  	  spatialCoefficientsForTZ(0,0,2,nc)=.5;
//  	}
      }
    }

    if( pdeModel==twoPhaseFlowModel )
    {
      // tc : VOF
      // nc : level-set 

      spatialCoefficientsForTZ(0,0,0, tc)=.5;
      spatialCoefficientsForTZ(0,0,0, nc)=2.;
      
      if(  numberOfDimensions>1 )
      {
	if( degreeSpace>=1 )
	{
	  spatialCoefficientsForTZ(0,1,0, tc)=.2;
	  spatialCoefficientsForTZ(0,1,0, nc)=.1;
	}
        if( degreeSpace>=2 )
	{
	  spatialCoefficientsForTZ(2,0,0, tc)=.2;
	  spatialCoefficientsForTZ(0,2,0, tc)=.3;
	  spatialCoefficientsForTZ(2,0,0, nc)=.8;
	  spatialCoefficientsForTZ(0,2,0, nc)=.6;
	}
      }
      if(  numberOfDimensions>2 )
      {
	if( degreeSpace>=1 )
	{
	  spatialCoefficientsForTZ(0,0,1, tc)=.15;
	  spatialCoefficientsForTZ(0,0,1, nc)=.25;
	}
        if( degreeSpace>=2 )
	{
	  spatialCoefficientsForTZ(0,0,2, tc)=.5;
	  spatialCoefficientsForTZ(0,0,2, nc)=.5;
	}
      }

    }
    

    if(  numberOfDimensions==1 )
    {
      //  Set twilight zone flow to satisfy u_x+v_y=0 (for rhs to pressure equation)
      if( degreeSpace==1 )
      {
	spatialCoefficientsForTZ(0,0,0, pc)=-1.;      // p=-1+x
	spatialCoefficientsForTZ(1,0,0, pc)= 1.;

	spatialCoefficientsForTZ(0,0,0, uc)=1.;      // u=1+x
	spatialCoefficientsForTZ(1,0,0, uc)=1.;
      }
      else if( degreeSpace==2 )
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 -1
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;

	spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=1 +x^2 
	spatialCoefficientsForTZ(1,1,0, uc)=2.;
      }
    
    }
    else if(  numberOfDimensions==2 ||  dimensionOfTZFunction==2 )
    {
      //  Set twilight zone flow to satisfy u_x+v_y=0 (for rhs to pressure equation)
      if( degreeSpace==1 )
      {
	spatialCoefficientsForTZ(0,0,0, pc)=-1.;      // p=-1+x + y
	spatialCoefficientsForTZ(1,0,0, pc)= 1.;
	spatialCoefficientsForTZ(0,1,0, pc)= 1.;

	if( !isAxisymmetric() )
	{
	  spatialCoefficientsForTZ(0,0,0, uc)=1.;      // u=1+x+y
	  spatialCoefficientsForTZ(1,0,0, uc)=1.;
	  spatialCoefficientsForTZ(0,1,0, uc)=1.;

	  spatialCoefficientsForTZ(0,0,0, vc)= 2.;      // v=2+x-y
	  spatialCoefficientsForTZ(1,0,0, vc)= 1.;
	  spatialCoefficientsForTZ(0,1,0, vc)=-1.;
	}
	else
	{
	  // for axisymmetric:  u_x + v_y + v/y = 0
	  //  we also want p_y=0 at y=0 since Delta p = p.xx + p.yy + p.y/y

	  spatialCoefficientsForTZ(0,0,0, uc)=1.;      // u=1+x+y
	  spatialCoefficientsForTZ(1,0,0, uc)=1.;
	  spatialCoefficientsForTZ(0,1,0, uc)=1.;   // this seems ok for some reason

	  spatialCoefficientsForTZ(0,0,0, vc)= 0.;      // v=-.5*y  
	  spatialCoefficientsForTZ(1,0,0, vc)= 0.;
	  spatialCoefficientsForTZ(0,1,0, vc)=-.5;
	}
	  
      }
      else if( degreeSpace==2 )
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 -1 +.5 xy
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,0, pc)=-1.; 
	spatialCoefficientsForTZ(1,1,0, pc)= .5;

	if( !isAxisymmetric() )
	{
	  spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 -1 
	  spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	  spatialCoefficientsForTZ(0,0,0, pc)=-1.; 

	  spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 
	  spatialCoefficientsForTZ(1,1,0, uc)=2.;
	  spatialCoefficientsForTZ(0,2,0, uc)=1.;

	  spatialCoefficientsForTZ(2,0,0, vc)= 1.;      // v=x^2 -2xy - y^2 
	  spatialCoefficientsForTZ(1,1,0, vc)=-2.;
	  spatialCoefficientsForTZ(0,2,0, vc)=-1.;

	}
	else
	{
	  // for axisymmetric:  u_x + v_y + v/y = 0
	  //  we also want p_y=0 at y=0 since Delta p = p.xx + p.yy + p.y/y

	  spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 -1 +.5 xy
	  spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	  spatialCoefficientsForTZ(0,0,0, pc)=-1.; 
	  spatialCoefficientsForTZ(1,1,0, pc)= .5;

	  spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 
	  spatialCoefficientsForTZ(1,1,0, uc)=2.;      // u_x = 2*x + 2*y 
	  spatialCoefficientsForTZ(0,2,0, uc)=1.;

	  spatialCoefficientsForTZ(2,0,0, vc)= 0.;      // v=-x*y - 2/3*y^2 
	  spatialCoefficientsForTZ(1,1,0, vc)=-1.;      // v_y + v/y = -x-4/3*y -x -2/3*y = -2*x-2*y 
	  spatialCoefficientsForTZ(0,2,0, vc)=-2./3.;

	}

      }
      else if( degreeSpace==0 )
      {
	spatialCoefficientsForTZ(0,0,0, pc)=1.;
	spatialCoefficientsForTZ(0,0,0, uc)=.1; 
	spatialCoefficientsForTZ(0,0,0, vc)=.1;
//    	  spatialCoefficientsForTZ(0,0,0,pc)=1.;
//    	  spatialCoefficientsForTZ(0,0,0,uc)=-1.; 
//    	  spatialCoefficientsForTZ(0,0,0,vc)=-.5;
      }
      else if( degreeSpace==3 )
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^3 + y^3 -.3*x*y^2 + .2*x*y^2
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,0, pc)=-1.; 
	spatialCoefficientsForTZ(1,1,0, pc)= .5;

	spatialCoefficientsForTZ(3,0,0, pc)= 1.;     
	spatialCoefficientsForTZ(0,3,0, pc)= 1.;     
	spatialCoefficientsForTZ(1,2,0, pc)= -.3;
	spatialCoefficientsForTZ(2,1,0, pc)=  .2;


	spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 + .2*x^3 + .5*y^3 + xy^2
	spatialCoefficientsForTZ(1,1,0, uc)=2.;
	spatialCoefficientsForTZ(0,2,0, uc)=1.;

	spatialCoefficientsForTZ(3,0,0, uc)=.2;   
	spatialCoefficientsForTZ(0,3,0, uc)=.5;   
	spatialCoefficientsForTZ(1,2,0, uc)=1.;   


	spatialCoefficientsForTZ(2,0,0, vc)= 1.;      // v=x^2 -2xy - y^2 +.125*x^3 -(1/3)*y^3 -.6*x^2 y
	spatialCoefficientsForTZ(1,1,0, vc)=-2.;
	spatialCoefficientsForTZ(0,2,0, vc)=-1.;

	spatialCoefficientsForTZ(3,0,0, vc)=.125;
	spatialCoefficientsForTZ(0,3,0, vc)=-1./3.;
	spatialCoefficientsForTZ(2,1,0, vc)=-.6;

      }
      else if( degreeSpace==4 )
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^4 + y^4 -.3*x^2*y^2
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,0, pc)=-1.; 
	spatialCoefficientsForTZ(1,1,0, pc)= .5;

	spatialCoefficientsForTZ(4,0,0, pc)= 1.;     
	spatialCoefficientsForTZ(0,4,0, pc)= 1.;     
	spatialCoefficientsForTZ(2,2,0, pc)= -.3;


	spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
	spatialCoefficientsForTZ(1,1,0, uc)=2.;
	spatialCoefficientsForTZ(0,2,0, uc)=1.;

	spatialCoefficientsForTZ(4,0,0, uc)=.2;   
	spatialCoefficientsForTZ(0,4,0, uc)=.5;   
	spatialCoefficientsForTZ(1,3,0, uc)=1.;   


	spatialCoefficientsForTZ(2,0,0, vc)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
	spatialCoefficientsForTZ(1,1,0, vc)=-2.;
	spatialCoefficientsForTZ(0,2,0, vc)=-1.;

	spatialCoefficientsForTZ(4,0,0, vc)=.125;
	spatialCoefficientsForTZ(0,4,0, vc)=-.25;
	spatialCoefficientsForTZ(3,1,0, vc)=-.8;

      }
      else
      {
	printF("InsParameters::INS not implemented for degree in space =%i \n",degreeSpace);
	Overture::abort("error");
      }
    }
    else if(  numberOfDimensions==3 )
    {
      if( degreeSpace==1 )
      {
	spatialCoefficientsForTZ(0,0,0, pc)=-1.;      // p=-1+x+y+z
	spatialCoefficientsForTZ(1,0,0, pc)= 1.;
	spatialCoefficientsForTZ(0,1,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,1, pc)= 1.;

	spatialCoefficientsForTZ(0,0,0, uc)=1.;      // u=1 + x + y + z
	spatialCoefficientsForTZ(1,0,0, uc)=1.;
	spatialCoefficientsForTZ(0,1,0, uc)=1.;
	spatialCoefficientsForTZ(0,0,1, uc)=1.;
 
	spatialCoefficientsForTZ(0,0,0, vc)= 2.;      // v=2+x-2y+z
	spatialCoefficientsForTZ(1,0,0, vc)= 1.;
	spatialCoefficientsForTZ(0,1,0, vc)=-2.;
	spatialCoefficientsForTZ(0,0,1, vc)= 1.;

	spatialCoefficientsForTZ(1,0,0, wc)=-1.;      // w=-x+y+z
	spatialCoefficientsForTZ(0,1,0, wc)= 1.;
	spatialCoefficientsForTZ(0,0,1, wc)= 1.;
      }
      else if( degreeSpace==2 ) 
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 + z^2 -1 + .5*xy
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,2, pc)= 1.;
	spatialCoefficientsForTZ(0,0,0, pc)=-1.;
	spatialCoefficientsForTZ(1,1,0, pc)= .5;

	spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 + xz
	spatialCoefficientsForTZ(1,1,0, uc)=2.;
	spatialCoefficientsForTZ(0,2,0, uc)=1.;
	spatialCoefficientsForTZ(1,0,1, uc)=1.;

	spatialCoefficientsForTZ(2,0,0, vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
	spatialCoefficientsForTZ(1,1,0, vc)=-2.;
	spatialCoefficientsForTZ(0,2,0, vc)=-1.;
	spatialCoefficientsForTZ(0,1,1, vc)=+3.;

	spatialCoefficientsForTZ(2,0,0, wc)= 1.;      // w=x^2 + y^2 - 2 z^2
	spatialCoefficientsForTZ(0,2,0, wc)= 1.;
	spatialCoefficientsForTZ(0,0,2, wc)=-2.;
      }
      else if( degreeSpace==0 )
      {
	spatialCoefficientsForTZ(0,0,0, pc)=1.;
	spatialCoefficientsForTZ(0,0,0, uc)=-1.; 
	spatialCoefficientsForTZ(0,0,0, vc)=-.5;
	spatialCoefficientsForTZ(0,0,0, wc)=.75; 
      }
      else if( degreeSpace==4 ) 
      {
	spatialCoefficientsForTZ(2,0,0, pc)= 1.;      // p=x^2 + y^2 + z^2 -1 + .5*xy +.25*x^4 + .25*y^4
	spatialCoefficientsForTZ(0,2,0, pc)= 1.;
	spatialCoefficientsForTZ(0,0,2, pc)= 1.;
	spatialCoefficientsForTZ(0,0,0, pc)=-1.;
	spatialCoefficientsForTZ(1,1,0, pc)= .5;

	spatialCoefficientsForTZ(4,0,0, pc)= .25;
	spatialCoefficientsForTZ(0,4,0, pc)=-.25;
	  

	spatialCoefficientsForTZ(2,0,0, uc)=1.;      // u=x^2 + 2xy + y^2 + xz
	spatialCoefficientsForTZ(1,1,0, uc)=2.;
	spatialCoefficientsForTZ(0,2,0, uc)=1.;
	spatialCoefficientsForTZ(1,0,1, uc)=1.;

	spatialCoefficientsForTZ(4,0,0, uc)=.125; 
	spatialCoefficientsForTZ(0,4,0, uc)=.125; 
	spatialCoefficientsForTZ(0,0,4, uc)=.125; 
	spatialCoefficientsForTZ(1,0,3, uc)=-.5; 


	spatialCoefficientsForTZ(2,0,0, vc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
	spatialCoefficientsForTZ(1,1,0, vc)=-2.;
	spatialCoefficientsForTZ(0,2,0, vc)=-1.;
	spatialCoefficientsForTZ(0,1,1, vc)=+3.;

	spatialCoefficientsForTZ(4,0,0, vc)=.25; 
	spatialCoefficientsForTZ(0,4,0, vc)=.25; 
	spatialCoefficientsForTZ(0,0,4, vc)=.25; 
	spatialCoefficientsForTZ(3,1,0, vc)=-.5; 


	spatialCoefficientsForTZ(2,0,0, wc)= 1.;      // w=x^2 + y^2 - 2 z^2
	spatialCoefficientsForTZ(0,2,0, wc)= 1.;
	spatialCoefficientsForTZ(0,0,2, wc)=-2.;

	spatialCoefficientsForTZ(4,0,0, wc)=.25; 
	spatialCoefficientsForTZ(0,4,0, wc)=-.2; 
	spatialCoefficientsForTZ(0,0,4, wc)=.125; 
	spatialCoefficientsForTZ(0,3,1, wc)=-1.;
      }
      else
      {
	printF("InsParameters::INS not implemented for degree in space =%i \n",degreeSpace);
	Overture::abort("error");
      }
    }

    if(  tc>=0 )
    { // define the TZ function for the Temperature
      if(  numberOfDimensions==2 )
      {
	for( int m=0; m<=degreeSpace; m++ )
	{
	  spatialCoefficientsForTZ(m,0,0, tc)= 1./(m+1.); 
	  spatialCoefficientsForTZ(0,m,0, tc)= .5/(m+1.); 
	}
      }
      else if(  numberOfDimensions==3 )
      {
	for( int m=0; m<=degreeSpace; m++ )
	{
	  spatialCoefficientsForTZ(m,0,0, tc)= 1./(m+1.); 
	  spatialCoefficientsForTZ(0,m,0, tc)= .5/(m+1.); 
	  spatialCoefficientsForTZ(0,0,m, tc)=.25/(m+1.); 
	}
      }
      else
      {
	for( int m=0; m<degreeSpace; m++ )
	  spatialCoefficientsForTZ(m,0,0, tc)= 1./(m+1.); 
      }
	
    }
    if( rc>=0 )
    { // define the TZ function for the density
      if(  numberOfDimensions==2 )
      {
	for( int m=0; m<=degreeSpace; m+=2 ) // note: only use positive powers to keep rho positive 
	{
	  spatialCoefficientsForTZ(m,0,0, rc)= 1./(m+1.); 
	  spatialCoefficientsForTZ(0,m,0, rc)= .5/(m+1.); 
	}
      }
      else if(  numberOfDimensions==3 )
      {
	for( int m=0; m<=degreeSpace; m+=2 )
	{
	  spatialCoefficientsForTZ(m,0,0, rc)= 1./(m+1.); 
	  spatialCoefficientsForTZ(0,m,0, rc)= .5/(m+1.); 
	  spatialCoefficientsForTZ(0,0,m, rc)=.25/(m+1.); 
	}
      }
      else
      {
	for( int m=0; m<degreeSpace; m+=2 )
	  spatialCoefficientsForTZ(m,0,0, rc)= 1./(m+1.); 
      }
	
    }

    if(  dbase.get<bool >("computeReactions") )
    {
      // assign species
      for( int n= sc; n< numberOfComponents; n++ )
      {
	real ni =1./(n+1);
    
	spatialCoefficientsForTZ(0,0,0,n)=1.;      
	if( degreeSpace>0 )
	{
	  spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	  spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,0,1,n)=  numberOfDimensions==3 ? .25*ni : 0.;
	}
	if( degreeSpace>1 )
	{
	  spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	  spatialCoefficientsForTZ(0,0,2,n)=  numberOfDimensions==3 ? .125*ni : 0.;
	}
      }
    }
  

    for( int n=0; n< numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
      {
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
      }
	  
    }
  
    // -------------------------------------
    // -- Assign TZ material properties ----
    // -------------------------------------

    //  NOTE: rho, Cp and K must remain positive

    // NOTE: rho, Cp, and thermalConductivity have probably NOT been set by the user yet 
    if( degreeSpace==0 )
    {
      spatialCoefficientsForTZ(0,0,0,rhoc   )  =2.;  // dbase.get<real>("rho");   
      spatialCoefficientsForTZ(0,0,0,Cpc    )  =5.;  // dbase.get<real>("Cp");   
      spatialCoefficientsForTZ(0,0,0,thermalKc)=1.; // dbase.get<real>("thermalConductivity"); 
    }
    else if( degreeSpace==1 )
    {  
      spatialCoefficientsForTZ(0,0,0,rhoc)=2.; // dbase.get<real>("rho");    // rho=rho0 + .1*x+ .05*y
      spatialCoefficientsForTZ(1,0,0,rhoc)=.1;
      spatialCoefficientsForTZ(0,1,0,rhoc)=.05;

      spatialCoefficientsForTZ(0,0,0,Cpc)=5.; // dbase.get<real>("Cp");       // Cp = Cp0 + .07*x+ .065*y
      spatialCoefficientsForTZ(1,0,0,Cpc)=.175;
      spatialCoefficientsForTZ(0,1,0,Cpc)=.165;

      spatialCoefficientsForTZ(0,0,0,thermalKc)=1.; // dbase.get<real>("thermalConductivity");  // K=K0+ .085*x+ .045*y
      spatialCoefficientsForTZ(1,0,0,thermalKc)=.085;
      spatialCoefficientsForTZ(0,1,0,thermalKc)=.045;

      if( numberOfDimensions==3 )
      {
	spatialCoefficientsForTZ(0,0,1,rhoc)=.025;
	spatialCoefficientsForTZ(0,0,1,Cpc) =.0125;
	spatialCoefficientsForTZ(0,0,1,thermalKc)=.035;
      }
    }
    else 
    {
      if( degreeSpace>2 )
      {
        // Finish me for higher degree poly's 
	printF("Cgins::InsParameters:WARNING using degree=2 TZ for material properties instead of %i.\n",
               degreeSpace);
      }
      

      spatialCoefficientsForTZ(0,0,0,rhoc)=1.;            // rho=1 + .1*x^2+ .05*y^2
      spatialCoefficientsForTZ(2,0,0,rhoc)=.1;
      spatialCoefficientsForTZ(0,2,0,rhoc)=.05;

      spatialCoefficientsForTZ(0,0,0,Cpc)=10.;            // Cp = 10 + .07*x^2+ .065*y^2
      spatialCoefficientsForTZ(2,0,0,Cpc)=.075;
      spatialCoefficientsForTZ(0,2,0,Cpc)=.065;

      spatialCoefficientsForTZ(0,0,0,thermalKc)=1.5;      // K = 1.5 + .085*x^2+ .045*y^2
      spatialCoefficientsForTZ(2,0,0,thermalKc)=.085;
      spatialCoefficientsForTZ(0,2,0,thermalKc)=.045;

      if( numberOfDimensions==3 )
      {
	spatialCoefficientsForTZ(0,0,2,rhoc)=.025;
	spatialCoefficientsForTZ(0,0,2,Cpc) =.0125;
	spatialCoefficientsForTZ(0,0,2,thermalKc)=.035;
      }
    }
    // Material properties do NOT depend on time.
    timeCoefficientsForTZ(0,rhoc   )=1.;
    timeCoefficientsForTZ(0,Cpc    )=1.;
    timeCoefficientsForTZ(0,thermalKc)=1.;



    // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
    ((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
  
  }
  else if( choice==trigonometric ) // ******* Trigonometric function chosen ******
  {
    ArraySimpleFixed<real,4,1,1,1> & omega = dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");

    RealArray fx( numberOfTZComponents),fy( numberOfTZComponents),fz( numberOfTZComponents),ft( numberOfTZComponents);
    RealArray gx( numberOfTZComponents),gy( numberOfTZComponents),gz( numberOfTZComponents),gt( numberOfTZComponents);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude( numberOfTZComponents), cc( numberOfTZComponents);
    amplitude=1.;
    cc=0.;

    

    fx= omega[0];
    fy =  numberOfDimensions>1 ?  omega[1] : 0.;
    fz =  numberOfDimensions>2 ?  omega[2] : 0.;
    ft =  omega[3];

    // make the velocity divergence free
    if(  numberOfDimensions==3 &&  dimensionOfTZFunction==2 )
    {
      // **** compare 2D to 3D *****
      int option=1;
      if( option==0 )
      {
	// u=cos(pi x) cos( pi y ) + .5
	// v=sin(pi x) sin( pi y ) + .5 
	// p=cos(    ) sin(      ) + .5
	assert(  (omega[0]==omega[1]) );

	fx= omega[0];
	fy= omega[1];
	fz=0.;
	fx( wc)=0.;
	fy( wc)=0.;
	fz( wc)=0.;
	ft( wc)=0.;
	amplitude( wc)=.0;

	gx( vc)=.5/ omega[0];   // shift by pi/2 to turn cos() into sin()
	gy( vc)=.5/ omega[1];

	gy( pc)=.5/ omega[1];
	cc( pc)=.0;

	amplitude( uc)=.5;  cc( uc)=.0;
	amplitude( vc)=.5;  cc( vc)=.0;
      }
      else
      {
	// v=cos(pi y) cos( pi z ) + .5
	// w=sin(pi y) sin( pi z ) + .5 
	// p=cos(    ) sin(      ) + .5
	assert(  (omega[1]==omega[2]) );

	fx=0.;
	fy= omega[1];
	fz= omega[2];
	fx( uc)=0.;
	fy( uc)=0.;
	fz( uc)=0.;
	ft( uc)=0.;
	amplitude( uc)=.0;


	gy( wc)=.5/ omega[1];   // shift by pi/2 to turn cos() into sin()
	gz( wc)=.5/ omega[2];

	gz( pc)=.5/ omega[1];
	cc( pc)=.0;

	amplitude( vc)=.5;  cc( vc)=.0;
	amplitude( wc)=.5;  cc( wc)=.0;

      }
	
 
    }
    else if(  numberOfDimensions==2  )
    {   
      // u=.5 cos(pi x) cos( pi y ) 
      // v=.5 sin(pi x) sin( pi y ) 
      // p=   cos(    ) sin(      ) 
      assert(  (omega[0]==omega[1]) );

      gx( vc)=.5/ omega[0];   // shift by pi/2 to turn cos() into sin()
      gy( vc)=.5/ omega[1];

      amplitude( uc)=.5;  cc( uc)=.0;
      amplitude( vc)=.5;  cc( vc)=.0;

      gy( pc)=.5/ omega[1]; // turn off for testing symmetry
      cc( pc)=.0;
 
    }
    else if(  numberOfDimensions==3 )
    {
      // u=   cos(pi x) cos( pi y ) cos( pi z)  // **** fix ***
      // v=.5 sin(pi x) sin( pi y ) cos( pi z)
      // w=.5 sin(pi x) cos( pi y ) sin( pi z)
      // p=   cos(pi x) cos( pi y ) cos( pi z)
	
      if(  omega[0]== omega[1] &&  omega[0]== omega[2] )
      {
	gx( vc)=.5/ omega[0];
	gy( vc)=.5/ omega[1];
	amplitude( vc)=.5;
	
	gx( wc)=.5/ omega[0];
	gz( wc)=.5/ omega[2];
	amplitude( wc)=.5;
      }
      else if(  omega[0]== omega[2] &&  omega[1]==0 )
      {
	// pseudo 2D case
	gx( wc)=.5/ omega[0];   // shift by pi/2 to turn cos() into sin()
	gz( wc)=.5/ omega[2];

	amplitude( uc)=.5;  cc( uc)=.0;
	amplitude( wc)=.5;  cc( wc)=.0;
      }
      else
      {
	Overture::abort("Invalid values for omega[0..2]");
      }
	
    }

    if(  turbulenceModel==SpalartAllmaras )
    {
      // nuT should remain positive
      // Don not make nuT too large since the source terms divide by the distance to the wall
      gx( kc)=.5/ omega[0];
      gy( kc)=.5/ omega[1];
      amplitude( kc)=.1;
      cc( kc)=.2;
    }
    if(  turbulenceModel==kEpsilon ||  turbulenceModel==kOmega )
    {
      cc(kc)=2.;
      cc(kc+1)=3.;
    }
    
    // make the density positive for two-phase flow
    if( pdeModel==twoPhaseFlowModel )
    {
      assert( tc>0. );
      amplitude( tc)=.125; cc( tc)=1.;  gx( tc)=.5/ omega[0];
    }
    if( rc>=0 )
    {
      amplitude( rc)=.125; cc( rc)=1.;  gx( rc)=.5/ omega[0];
    }
    
    // -----------------------------------------------
    // -- Assign material properties for TZ testing --
    // -----------------------------------------------

    //  NOTE: rho, mu and lambda must remain positive
    amplitude(rhoc)     =.125; cc(rhoc)     =1.;  gx(rhoc)     =.5/ omega[0];
    amplitude(Cpc)      =.25;  cc(Cpc)      =2.;  gy(Cpc)      =.5/ omega[1];
    amplitude(thermalKc)=.30;  cc(thermalKc)=1.5; gx(thermalKc)=.5/ omega[1];


    // Material properties do NOT depend on time.
    ft(rhoc)=0.;
    ft(Cpc)=0.;
    ft(thermalKc)=0.;


    dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
      
  }
  else if( choice==pulse ) 
  {
    // ******* Pulse function chosen ******
     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( numberOfDimensions, numberOfTZComponents); 

    // this pulse function is not divergence free!

  }
    
  
  return 0;
}

int InsParameters::
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

  aString prefix = "OBPDE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  real & thermalConductivity = dbase.get<real>("thermalConductivity");

  aString answer;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();
  

  aString *pdeParametersMenu=NULL;

//\begin{>>setParametersInclude.tex}{\subsubsection{PDE parameters for INS}\label{sec:pdeParams}}
//\no function header:
//
// Here are the pde parameters that can be changed when solving the incompressible Navier-Stokes equations.
// This menu appears when {\tt `pde parameters'} is chosen from main menu.
//\begin{description}
//  \item[nu] : kinematic viscosity (constant).
//  \item[divergence damping]
//  \item[artificial diffusion] : see section~(\ref{AD}) for a description of the artificial diffusion terms.
//    \begin{description}
//      \item[second order artifical diffusion]
//        \begin{description}
//          \item[turn on second order artificial diffusion]
//          \item[turn off second order artificial diffusion]
//          \item[ad21 : coefficient of linear term]
//          \item[ad22 : coefficient of non-linear term]
//        \end{description}
//      \item[fourth order artificial diffusion]
//        \begin{description}
//          \item[turn on fourth order artificial diffusion]
//          \item[turn off fourth order artificial diffusion]
//          \item[ad41 : coefficient of linear term]
//          \item[ad42 : coefficient of non-linear term]
//        \end{description}
//    \end{description}
//  \end{description}
//
//\end{setParametersInclude.tex}
  const int numberOfMenuItems=27;
  pdeParametersMenu = new aString [numberOfMenuItems];
  int n=0;
  pdeParametersMenu[n++]="!pde parameters";
  pdeParametersMenu[n++]= "nu";
  pdeParametersMenu[n++]= "divergence damping";
  pdeParametersMenu[n++]= "gravity";
  pdeParametersMenu[n++]= "fluid density";
  pdeParametersMenu[n++]= "use old pressure boundary condition";
  pdeParametersMenu[n++]= "use p.n=0 boundary condition";
  pdeParametersMenu[n++]= "use default outflow";
  pdeParametersMenu[n++]= "check for inflow at outflow";
  pdeParametersMenu[n++]= "expect inflow at outflow";
  pdeParametersMenu[n++]= "use Neumann BC at outflow";
  pdeParametersMenu[n++]= "use extrapolate BC at outflow";
  pdeParametersMenu[n++]= "order of time extrapolation for p";
  pdeParametersMenu[n++]= ">artificial diffusion";
  pdeParametersMenu[n++]=   ">second order artifical diffusion";
  pdeParametersMenu[n++]=     "turn on second order artifical diffusion";
  pdeParametersMenu[n++]=     "turn off second order artifical diffusion";
  pdeParametersMenu[n++]=     "ad21 : coefficient of linear term";
  pdeParametersMenu[n++]=     "ad22 : coefficient of non-linear term";
  pdeParametersMenu[n++]=   "<>fourth order artifical diffusion";
  pdeParametersMenu[n++]=     "turn on fourth order artifical diffusion";
  pdeParametersMenu[n++]=     "turn off fourth order artifical diffusion";
  pdeParametersMenu[n++]=     "ad41 : coefficient of linear term";
  pdeParametersMenu[n++]=     "ad42 : coefficient of non-linear term";
  pdeParametersMenu[n++]="< ";
  pdeParametersMenu[n++]= "<done";
  pdeParametersMenu[n++]="";
  assert( n==numberOfMenuItems);

  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=40;
    aString cmd[maxCommands];


    dialog.setWindowTitle("Incompressible NS parameters");

    aString commands[] = {"use curl-curl boundary condition",
			  "use old pressure boundary condition",
			  "use p.n=0 boundary condition",
			  "" };

    dialog.addOptionMenu("", commands, commands,  dbase.get<int >("pressureBoundaryCondition"));
      
    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==SpalartAllmaras ||  
         dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==BaldwinLomax )
    {
      aString pbLabels[] = {"turbulence trip positions",""};
      addPrefix(pbLabels,prefix,cmd,maxCommands);
      int numRows=1;
      dialog.setPushButtons( cmd, pbLabels, numRows );
    }
      
    const int numberOfUserVariables= dbase.get<ListOfShowFileParameters >("pdeParameters").size();
    const int numberOfTextStrings=11+numberOfUserVariables;
    aString *textLabels  = new aString [numberOfTextStrings];
    aString *textStrings = new aString [numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "nu";  sPrintF(textStrings[nt], "%g", dbase.get<real >("nu"));  nt++; 
    textLabels[nt] = "divergence damping";  sPrintF(textStrings[nt], "%g", dbase.get<real >("cdv"));  nt++; 
    textLabels[nt] = "cDt div damping";  sPrintF(textStrings[nt], "%g", dbase.get<real >("cDt"));  nt++; 
    textLabels[nt] = "ad21,ad22";  sPrintF(textStrings[nt], "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"));  nt++; 
    textLabels[nt] = "ad41,ad42";  sPrintF(textStrings[nt], "%g,%g", dbase.get<real >("ad41"), dbase.get<real >("ad42"));  nt++; 
    textLabels[nt] = "ad61,ad62";  sPrintF(textStrings[nt], "%g,%g", dbase.get<real >("ad61"), dbase.get<real >("ad62"));  nt++; 
    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==SpalartAllmaras )
    {
      textLabels[nt] = "ad21n,ad22n";  sPrintF(textStrings[nt], "%g,%g", dbase.get<real >("ad21n"), dbase.get<real >("ad22n"));  nt++; 
      // textLabels[nt] = "ad41n,ad42n";  sPrintF(textStrings[nt], "%g,%g", dbase.get<real >("ad41n"), dbase.get<real >("ad42n"));  nt++; 
      textLabels[nt] = "SA scale factor";  sPrintF(textStrings[nt], "%g",spalartAllmarasScaleFactor);  nt++; 
      textLabels[nt] = "SA distance scale";  sPrintF(textStrings[nt], "%g",spalartAllmarasDistanceScale);  nt++; 
    }
    if(  dbase.get<int >("tc")>=0 )
    {
      textLabels[nt] = "kThermal";  sPrintF(textStrings[nt], "%g", dbase.get<real >("kThermal"));  nt++; 
      textLabels[nt] = "thermal conductivity";  sPrintF(textStrings[nt], "%g",thermalConductivity);  nt++; 
    }
      
    if(  dbase.get<bool >("advectPassiveScalar") )
    {
      textLabels[nt] = "passive scalar diffusion coefficient";
      sPrintF(textStrings[nt], "%g", dbase.get<real >("nuPassiveScalar"));  nt++; 
    }

    // Add this once we have well defined options 
    // textLabels[nt] = "LES option";  sPrintF(textStrings[nt], "%i", dbase.get<int >("largeEddySimulationOption"));  nt++; 

    // ********* add on user defined variables ************
    std::list<ShowFileParameter>::iterator iter; 
    for(iter =  dbase.get<ListOfShowFileParameters >("pdeParameters").begin(); 
        iter!= dbase.get<ListOfShowFileParameters >("pdeParameters").end(); iter++ )
    {
      ShowFileParameter & param = *iter;
      aString name; ShowFileParameter::ParameterType type; int ivalue; real rvalue; aString stringValue;
      param.get( name, type, ivalue, rvalue, stringValue );

      textLabels[nt] = name; 
      if( type==ShowFileParameter::realParameter )
      {
	textStrings[nt]=sPrintF("%e",rvalue);
      }
      else if( type==ShowFileParameter::intParameter )
      {
	textStrings[nt]=sPrintF("%i",ivalue);
      }
      else
      {
	textStrings[nt]=stringValue;
      }
      nt++; 
    }
    // null strings terminal list
    assert( nt<numberOfTextStrings );
    textLabels[nt]="";   textStrings[nt]="";  
    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

    delete [] textLabels;
    delete [] textStrings;
      
    aString tbLabels[] = {"project initial conditions",
                          "second-order artificial diffusion", 
			  "fourth-order artificial diffusion",
			  "sixth-order artificial diffusion",
			  "use implicit fourth-order artificial diffusion",
			  "use split-step implicit artificial diffusion",
			  "use new fourth order boundary conditions",
			  "use self-adjoint diffusion operator",
			  "include artificial diffusion in pressure equation",
			  "use boundary dissipation in AF scheme",
                          "stabilize high order boundary conditions",
			  ""};
    int tbState[11];
    tbState[0] =  dbase.get<bool >("projectInitialConditions");
    tbState[1] =  dbase.get<bool >("useSecondOrderArtificialDiffusion");
    tbState[2] =  dbase.get<bool >("useFourthOrderArtificialDiffusion");
    tbState[3] =  dbase.get<bool >("useSixthOrderArtificialDiffusion");
    tbState[4] =  dbase.get<bool >("useImplicitFourthArtificialDiffusion");
    tbState[5] =  dbase.get<int >("useSplitStepImplicitArtificialDiffusion");
    tbState[6] =  dbase.get<int >("useNewFourthOrderBoundaryConditions"); 
    tbState[7] =  dbase.get<bool >("includeArtificialDiffusionInPressureEquation");
    tbState[8] =  dbase.get<bool >("useBoundaryDissipationInAFScheme");
    tbState[9] =  dbase.get<bool >("stabilizeHighOrderBoundaryConditions");
    tbState[10]=0;

    int numColumns=1;
    addPrefix(tbLabels,prefix,cmd,maxCommands);
    dialog.setToggleButtons(cmd, tbLabels, tbState, numColumns); 

    
    gui.buildPopup(pdeParametersMenu);
    delete [] pdeParametersMenu;
    
    if( false && gi.graphicsIsOn() )
      dialog.openDialog(0);   // open the dialog here so we can reset the parameter values below

    updatePDEparameters();

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
    

    if( answer=="done" )
      break;
    else if( answer=="nu" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter nu (default value=%e)", dbase.get<real >("nu")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("nu"));
    }
    else if( answer=="mu"  )
    {
       dbase.get<bool >("useDimensionalParameters")=true;  // make sure we are using dimensional parameters

      gi.inputString(answer,sPrintF(buff,"Enter mu (default value=%e)", dbase.get<real >("mu")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("mu"));
      printF(" mu=%9.3e\n", dbase.get<real >("mu"));
    }
    else if( answer=="kThermal"  )
    {
       dbase.get<bool >("useDimensionalParameters")=true;  // make sure we are using dimensional parameters

      gi.inputString(answer,sPrintF(buff,"Enter kThermal (default value=%e)", dbase.get<real >("kThermal")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("kThermal"));
      printF(" kThermal=%9.3e\n", dbase.get<real >("kThermal"));
    }
    else if( answer=="Rg (gas constant)"  )
    {
       dbase.get<bool >("useDimensionalParameters")=true;  // make sure we are using dimensional parameters

      gi.inputString(answer,sPrintF(buff,"Enter Rg (default value=%e)", dbase.get<real >("Rg")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("Rg"));
      printF(" Rg=%9.3e\n",  dbase.get<real >("Rg"));
    }
    else if( answer=="prandtlNumber"  )
    {
       dbase.get<bool >("useDimensionalParameters")=false;  // make sure we are using non-dimensional parameters

      gi.inputString(answer,sPrintF(buff,"Enter prandtlNumber (default value=%e)", dbase.get<real >("prandtlNumber")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("prandtlNumber"));
      printF(" prandtlNumber=%9.3e\n", dbase.get<real >("prandtlNumber"));
    }
    else if( answer=="gamma"  )
    {
      gi.inputString(answer,sPrintF(buff,"Enter gamma (default value=%e)", dbase.get<real >("gamma")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("gamma"));
      printF(" gamma=%9.3e\n", dbase.get<real >("gamma"));
    }
    else if( answer=="gravity"  )
    {
      printF("gravity is specified as a vector, it is the accelation per unit mass.\n");
      if(  dbase.get<int >("numberOfDimensions")==2 )
      {
	gi.inputString(answer,sPrintF(buff,"Enter gravity, 2 values, default=(%8.2e,%8.2e))",
				        dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]));
	if( answer!="" )
	  sScanF(answer,"%e %e",& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]);
	printF(" gravity=(%8.2e,%8.2e)\n", dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]);
      }
      else
      {
	gi.inputString(answer,sPrintF(buff,"Enter gravity, 3 values, default=(%8.2e,%8.2e,%8.2e))",
				        dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]));
	if( answer!="" )
	  sScanF(answer,"%e %e %e",& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
	printF(" gravity=(%8.2e,%8.2e,%8.2e)\n", dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
      }
    }
    else if( answer=="fluid density"  )
    {
      printF("Enter the density of the (incompressible) fluid, current=%9.3e\n"
             " This density will be used to compute the buoyancy of moving bodies\n", dbase.get<real >("fluidDensity"));
      gi.inputString(answer,sPrintF(buff,"Enter the fluid density, default=(%8.2e))", dbase.get<real >("fluidDensity")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("fluidDensity"));
      printF(" New fluid density = %10.3e.\n", dbase.get<real >("fluidDensity"));
    }
    else if( answer=="Mach number" )
    {
      dbase.get<bool >("useDimensionalParameters")=false;  // make sure we are using non-dimensional parameters
      gi.inputString(answer,sPrintF(buff,"Enter the Mach number (default value=%e)", dbase.get<real >("machNumber")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("machNumber"));

      printF(" Mach number=%9.3e\n", dbase.get<real >("machNumber"));
      
    }
    else if( answer=="Reynolds number" )
    {
       dbase.get<bool >("useDimensionalParameters")=false;  // make sure we are using non-dimensional parameters
      gi.inputString(answer,sPrintF(buff,"Enter the Reynolds number (default value=%e)", dbase.get<real >("reynoldsNumber")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("reynoldsNumber"));

      printF(" reynoldsNumber=%9.3e\n", dbase.get<real >("reynoldsNumber"));
    }
    else if( answer=="divergence damping" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter cdv (default value=%e)", dbase.get<real >("cdv")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("cdv"));
      printF(" cdv=%9.3e\n", dbase.get<real >("cdv"));
    }
    else if( answer=="use old pressure boundary condition" )
    {
       dbase.get<int >("pressureBoundaryCondition")=! dbase.get<int >("pressureBoundaryCondition");
      if(  dbase.get<int >("pressureBoundaryCondition")==0 )
	printF("Using new form of pressure BC.  p.n=-nu n.curl(curl u)\n");
      else
	printF("Using old form of pressure BC.\n");

    }
    else if( answer=="use p.n=0 boundary condition" )
    {
       dbase.get<int >("pressureBoundaryCondition")=2;
      printF("Using p.n=0 BC.\n");
    }
    else if( answer=="use default outflow" ||
	     answer=="check for inflow at outflow" ||        
	     answer=="expect inflow at outflow" )
    {
      int & checkForInflowAtOutFlow = dbase.get<int >("checkForInflowAtOutFlow");
      int & outflowOption = dbase.get<int>("outflowOption");
      checkForInflowAtOutFlow=(answer=="check for inflow at outflow" ? 1 : 
			       answer=="expect inflow at outflow" ? 2 : 0);

      printF("Setting checkForInflowAtOutFlow=%i, and outflowOption=%i\n",checkForInflowAtOutFlow,outflowOption);
      
       
      if( checkForInflowAtOutFlow==0 )
	outflowOption=0;  // extrapolate at outflow
      else
	outflowOption=1;  // Neumann BC at outflow

    }
    else if( dialog.getTextValue(answer,"order of time extrapolation for p","%i", dbase.get<int >("orderOfTimeExtrapolationForPressure")) )
    {
    }
    else if( len=answer.matches("use new fourth order boundary conditions") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("use new fourth order boundary conditions",value);      
       dbase.get<int >("useNewFourthOrderBoundaryConditions")=value;
      if(  dbase.get<int >("useNewFourthOrderBoundaryConditions") )
	printF("use new fourth order boundary conditions\n");
      else
	printF("do not use new fourth order boundary conditions\n");
    }
    else if( len=answer.matches("slip wall boundary condition option") ) 
    {
      sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("slipWallBoundaryConditionOption")); 
      if(  dbase.get<int >("slipWallBoundaryConditionOption")==0 )
      {
	printF("** Using symmetry slip wall BC **\n");
      }
      else if(  dbase.get<int >("slipWallBoundaryConditionOption")==1 )
      {
	printF("*** Using slipWallPressureEntropySymmetry condition ***\n");
      }
      else if(  dbase.get<int >("slipWallBoundaryConditionOption")==2 )
      {
	printF("*** Using slipWallTaylor condition ***\n");
      }
      else if(  dbase.get<int >("slipWallBoundaryConditionOption")==3 )
      {
	printF("*** Using slipWallCharacteristic condition ***\n");
      }
      else if(  dbase.get<int >("slipWallBoundaryConditionOption")==4 )
      {
	printF("*** Using slipWallDerivative condition ***\n");
      }
      else
      {
	Overture::abort("Unknown slip wall boundary condition option");
      }
      
    }
    else if( len=answer.matches("use self-adjoint diffusion operator") ) 
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("use self-adjoint diffusion operator",value);      
       dbase.get<bool >("useSelfAdjointDiffusion")=value;
      if(  dbase.get<bool >("useSelfAdjointDiffusion") )
	printF("use self-adjoint diffusion operator\n");
      else
	printF("do not use self-adjoint diffusion operator\n");
    }
    else if( len=answer.matches("include artificial diffusion in pressure equation") ) 
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("include artificial diffusion in pressure equation",value);      
       dbase.get<bool >("includeArtificialDiffusionInPressureEquation")=value;
      if(  dbase.get<bool >("includeArtificialDiffusionInPressureEquation") )
	printF("include artificial diffusion in pressure equation.\n");
      else
	printF("do not include artificial diffusion in pressure equation.\n");
    }
    else if ( dialog.getToggleValue(answer,"use boundary dissipation in AF scheme",dbase.get<bool >("useBoundaryDissipationInAFScheme")) ){}
    else if ( dialog.getToggleValue(answer,"stabilize high order boundary conditions",dbase.get<bool >("stabilizeHighOrderBoundaryConditions")) )
    {
      if( dbase.get<bool >("stabilizeHighOrderBoundaryConditions") )
      {
	printF("INFO: stabilizeHighOrderBoundaryConditions is on: use a lower order artificial dissipation in the fourth-order boundary conditions\n");
      }
    }
    else if( answer=="turn on second order artificial diffusion" )
    {
       dbase.get<bool >("useSecondOrderArtificialDiffusion")=true;
       dbase.get<real >("ad21")=1.; // .25;
       dbase.get<real >("ad22")=1.; // .25;
       dbase.get<real >("av2")=.25;
       dbase.get<real >("aw2")=.008333; 
       printF("turn on second order artficial diffusion with ad21=%e, ad22=%e\n", 
               dbase.get<real >("ad21"), dbase.get<real >("ad22"));
      dialog.setTextLabel("ad21,ad22",sPrintF(answer, "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"))); 
    }
    else if( answer=="turn off second order artificial diffusion" )
    {
       dbase.get<bool >("useSecondOrderArtificialDiffusion")=false;
       dbase.get<real >("ad21")=.0;
       dbase.get<real >("ad22")=.0;
       dbase.get<real >("av2")=0.;
       dbase.get<real >("aw2")=0.;
      printF("turn off second order artficial diffusion\n");
    }
    else if( answer=="ad21 : coefficient of linear term" )
    {
       dbase.get<real >("ad21")=1.; // .25;
      gi.inputString(answer,sPrintF(buff,"Enter ad21 (default value=%e)", dbase.get<real >("ad21")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("ad21"));
      printF(" ad21=%9.3e\n", dbase.get<real >("ad21"));
      dialog.setTextLabel("ad21,ad22",sPrintF(answer, "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"))); 
    }
    else if( answer=="ad22 : coefficient of non-linear term" )
    {
       dbase.get<real >("ad22")=1.; // .25;
      gi.inputString(answer,sPrintF(buff,"Enter ad22 (default value=%e)", dbase.get<real >("ad22")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("ad22"));
      printF(" ad22=%9.3e\n", dbase.get<real >("ad22"));
      dialog.setTextLabel("ad21,ad22",sPrintF(answer, "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"))); 
    }
    else if( answer=="turn on fourth order artificial diffusion" )
    {
       dbase.get<bool >("useFourthOrderArtificialDiffusion")=true;
       dbase.get<real >("ad41")=1.; // .25;
       dbase.get<real >("ad42")=1.; // .25;
       dbase.get<real >("av4")=.1; 
       dbase.get<real >("aw4")=.01; 

      printF("turn on fourth order artificial diffusion with ad41=%e, ad42=%e\n", dbase.get<real >("ad41"), dbase.get<real >("ad42"));
      dialog.setTextLabel("ad41,ad42",sPrintF(answer, "%g,%g", dbase.get<real >("ad41"), dbase.get<real >("ad42"))); 

      if(  dbase.get<int >("orderOfAccuracy")==2 )
         dbase.get<int >("extrapolateInterpolationNeighbours")=true;
    }
    else if( answer=="turn off fourth order artificial diffusion" )
    {
       dbase.get<bool >("useFourthOrderArtificialDiffusion")=false;
       dbase.get<real >("ad41")=.0;
       dbase.get<real >("ad42")=.0;
       dbase.get<real >("av4")=.0; 
       dbase.get<real >("aw4")=.0; 
       printF("turn off fourth order artficial diffusion, ad41=%e, ad42=%e\n", 
                  dbase.get<real >("ad41"), dbase.get<real >("ad42"));
       // dbase.get<int >("extrapolateInterpolationNeighbours")=false; // *wdh* 100611
       if( ( dbase.get<int >("orderOfAccuracy")==2 && numberOfGhostPointsNeeded()>=2 ) ||
	   ( dbase.get<int >("orderOfAccuracy")==4 && numberOfGhostPointsNeeded()>=3 ) )
       {
	 dbase.get<int >("extrapolateInterpolationNeighbours")=true;
       }
       else
       {
	 dbase.get<int >("extrapolateInterpolationNeighbours")=false;
       }

    }
    else if( answer=="ad41 : coefficient of linear term" )
    {
       dbase.get<real >("ad41")=1.; // .25;
      gi.inputString(answer,sPrintF(buff,"Enter ad41 (default value=%e)", dbase.get<real >("ad41")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("ad41"));
      printF(" ad41=%9.3e\n", dbase.get<real >("ad41"));
      dialog.setTextLabel("ad41,ad42",sPrintF(answer, "%g,%g", dbase.get<real >("ad41"), dbase.get<real >("ad42"))); 
    }
    else if( answer=="ad42 : coefficient of non-linear term" )
    {
       dbase.get<real >("ad42")=1.; // .25;
      gi.inputString(answer,sPrintF(buff,"Enter ad42 (default value=%e)", dbase.get<real >("ad42")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("ad42"));
      printF(" ad42=%9.3e\n", dbase.get<real >("ad42"));
      dialog.setTextLabel("ad41,ad42",sPrintF(answer, "%g,%g", dbase.get<real >("ad41"), dbase.get<real >("ad42"))); 
    }
    else if( answer=="nuRho" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter nuRho (default value=%e)", dbase.get<real >("nuRho")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("nuRho"));
      printF(" nuRho=%9.3e\n", dbase.get<real >("nuRho"));
    }
    else if( answer=="anu" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter anu (default value=%e)", dbase.get<real >("anu")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("anu"));
      printF(" anu=%9.3e\n", dbase.get<real >("anu"));
    }
    else if( answer=="pressure level" )
    {
      dbase.get<real >("pressureLevel")=0.;
      gi.inputString(answer,sPrintF(buff,"Enter pressureLevel (default value=%e)", dbase.get<real >("pressureLevel")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("pressureLevel"));
      printF(" pressureLevel=%9.3e\n", dbase.get<real >("pressureLevel"));
    }
    else if( answer=="remove fast pressure waves (toggle)" )
    {
      dbase.get<int >("removeFastPressureWaves")=! dbase.get<int >("removeFastPressureWaves");
      printF(" removeFastPressureWaves=%9.3e\n", dbase.get<int >("removeFastPressureWaves"));
    }
    else if( answer=="characteristic interpolation" )
    {
      dbase.get<bool >("useCharacteristicInterpolation")=! dbase.get<bool >("useCharacteristicInterpolation");
      if(  dbase.get<bool >("useCharacteristicInterpolation") )
	printF("Use characteristic interpolation\n");
      else
	printF("Do NOT use characteristic interpolation\n");
    }
    else if( answer=="non-dimensional parameters" )
    {
      dbase.get<bool >("useDimensionalParameters")=false;
    }
    else if( answer=="dimensional parameters" )
    {
      dbase.get<bool >("useDimensionalParameters")=true;
    }
    else if( answer(0,14)=="Reynolds number" )
    {
      if( ! dbase.get<bool >("useDimensionalParameters") )
      {
	sScanF(answer(15,answer.length()),"%e",& dbase.get<real >("reynoldsNumber"));
	printF(" reynoldsNumber=%9.3e\n", dbase.get<real >("reynoldsNumber"));
      }
      else
        printF("You must switch to non-dimensional parameters before assigning the Reynolds number\n");
    }
    else if( answer(0,10)=="Mach number" )
    {
      if( ! dbase.get<bool >("useDimensionalParameters") )
      {
	sScanF(answer(11,answer.length()),"%e",& dbase.get<real >("machNumber"));
	printF(" machNumber=%9.3e\n", dbase.get<real >("machNumber"));
      }
      else
        printF("You must switch to non-dimensional parameters before assigning the Mach number\n");
    }
    else if( answer(0,1)=="mu" )
    {
      if(  dbase.get<bool >("useDimensionalParameters") )
      {
	sScanF(answer(2,answer.length()),"%e",& dbase.get<real >("mu"));
	printF(" mu=%9.3e\n", dbase.get<real >("mu"));
        if(  dbase.get<real >("prandtlNumber")>0. )
	{
	   dbase.get<real >("kThermal")= dbase.get<real >("mu")/ dbase.get<real >("prandtlNumber");
          printF("Assigning kThermal=mu/Prandtl. Reset kThermal or Prandtl if you want a different value.\n");
	}
      }
      else
        printF("You must switch to dimensional parameters before assigning mu\n");
    }
    else if( answer(0,7)=="kThermal" )
    {
      if(  dbase.get<bool >("useDimensionalParameters") )
      {
	sScanF(answer(8,answer.length()),"%e",& dbase.get<real >("kThermal"));
	printF(" kThermal=%9.3e\n", dbase.get<real >("kThermal"));
      }
      else
        printF("You must switch to dimensional parameters before assigning kThermal\n");
    }
    else if( dialog.getTextValue(answer,"thermal conductivity","%e",thermalConductivity) )
    {
      printF("INFO: The thermalConductivity=%g is used for flux interfaces between domains\n",thermalConductivity);
    }
    else if( len=answer.matches("Rg (gas constant)") )
    {
      sScanF(answer(len,answer.length()-1),"%e",& dbase.get<real >("Rg"));
      printF(" Rg=%9.3e\n", dbase.get<real >("Rg"));
    }
    else if( len=answer.matches("gamma ") )
    {
      sScanF(answer(len-1,answer.length()),"%e",& dbase.get<real >("gamma"));
      printF(" gamma=%9.3e\n", dbase.get<real >("gamma"));
    }
    else if( answer(0,13)=="Prandtl number" )
    {
      sScanF(answer(14,answer.length()),"%e",& dbase.get<real >("prandtlNumber"));
      if(  dbase.get<int >("myid")==0 )
      {
        printF(" prandtlNumber=%9.3e\n", dbase.get<real >("prandtlNumber"));
        printF("---assigning kThermal=mu/Prandtl\n");
      }
       dbase.get<real >("kThermal")= dbase.get<real >("mu")/ dbase.get<real >("prandtlNumber");
    }
    else if( answer(0,6)=="av2,av4" )
    {
      sScanF(answer(7,answer.length()),"%e %e",& dbase.get<real >("av2"),& dbase.get<real >("av4"));
      printF(" av2=%9.3e, av4=%9.3e\n", dbase.get<real >("av2"), dbase.get<real >("av4"));
    }
    else if( answer(0,6)=="aw2,aw4" )
    {
      sScanF(answer(7,answer.length()),"%e %e",& dbase.get<real >("aw2"),& dbase.get<real >("aw4"));
      cout << " aw2=" <<  dbase.get<real >("av2") <<  ", aw4=" <<  dbase.get<real >("av4") <<endl;
    }
    else if( len=answer.matches("artificial viscosity") )
    {
      sScanF(answer(len,answer.length()-1),"%e",& dbase.get<real >("godunovArtificialViscosity"));
      printF(" godunovArtificialViscosity=%9.3e\n", dbase.get<real >("godunovArtificialViscosity"));
    }
    else if( len=answer.matches("artificial diffusion") )
    {
      int maxNum=15;
      RealArray ad(maxNum);  // assume at most 15 components for now
      ad=0.;
      int m;
      for( m=0; m< dbase.get<int >("numberOfComponents"); m++ )
	ad(m)= dbase.get<RealArray >("artificialDiffusion")(m);
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e ",&ad(0),&ad(1),&ad(2),&ad(3),
              &ad(4),&ad(5),&ad(6),&ad(7),&ad(8),&ad(9),&ad(10),&ad(11),&ad(12),&ad(13),&ad(14) );

      if(  dbase.get<int >("numberOfComponents")>maxNum )
      {
	printF("setPdeParameters:WARNING:Only reading the first %i artificial diffusion parameters\n"
               "                :Get Bill to fix this\n",maxNum);
      }
      
      aString text;
      for( m=0; m<min(maxNum, dbase.get<int >("numberOfComponents")); m++ )
      {
	 dbase.get<RealArray >("artificialDiffusion")(m)=ad(m);
        printF("Setting Godunov constant-coefficient artficial diffusion for component %s to %8.2e\n",
	       (const char*) dbase.get<aString* >("componentName")[m],ad(m));
	
	text+=sPrintF(buff, "%g ", dbase.get<RealArray >("artificialDiffusion")(m));
      }
      dialog.setTextLabel("artificial diffusion",text);
    }
    else if( answer(0,6)=="gravity" )
    {
      sScanF(answer(7,answer.length()),"%e %e %e",& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],
	     & dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
      printF(" gravity=(%8.2e,%8.2e)\n", dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]);
    }
    else if( answer=="default interpolation type" ||
             answer=="interpolate conservative variables" ||
             answer=="interpolate primitive variables" || 
             answer=="interpolate primitive and pressure" )
    {
       dbase.get<Parameters::InterpolationTypeEnum >("interpolationType") = (answer=="default interpolation type" ? defaultInterpolationType :
			   answer=="interpolate conservative variables" ? interpolateConservativeVariables :
                           answer=="interpolate primitive variables" ? interpolatePrimitiveVariables :
			   interpolatePrimitiveAndPressure );
      dialog.getOptionMenu("Interpolation Type:").setCurrentChoice( dbase.get<Parameters::InterpolationTypeEnum >("interpolationType"));
    }
    else if( dialog.getTextValue(answer,"nu","%e", dbase.get<real >("nu")) ){}//
    else if( dialog.getTextValue(answer,"divergence damping","%e", dbase.get<real >("cdv")) ){}//
    else if( dialog.getTextValue(answer,"cDt div damping","%e", dbase.get<real >("cDt")) ){}//
    // to add eventually else if( dialog.getTextValue(answer,"LES option","%i", dbase.get<int >("largeEddySimulationOption")) ){}//
    else if( len=answer.matches("ad21,ad22") )
    {
      sScanF(answer(len,answer.length()),"%e %e",& dbase.get<real >("ad21"),& dbase.get<real >("ad22"));
      printF(" ad21=%9.3e, ad22=%9.3e\n", dbase.get<real >("ad21"), dbase.get<real >("ad22"));
      
      dialog.setTextLabel("ad21,ad22",sPrintF(answer, "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"))); 
    }
    else if( len=answer.matches("ad41,ad42") )
    {
      sScanF(answer(len,answer.length()),"%e %e",& dbase.get<real >("ad41"),& dbase.get<real >("ad42"));
      // cout << " ad41=" <<  dbase.get<real >("ad41") << " ad42=" <<  dbase.get<real >("ad42") << endl;
      
      dialog.setTextLabel("ad41,ad42",sPrintF(answer, "%g,%g", dbase.get<real >("ad41"), dbase.get<real >("ad42"))); 
    }
    else if( len=answer.matches("ad61,ad62") )
    {
      sScanF(answer(len,answer.length()),"%e %e",& dbase.get<real >("ad61"),& dbase.get<real >("ad62"));
      // cout << " ad61=" <<  dbase.get<real >("ad61") << " ad62=" <<  dbase.get<real >("ad62") << endl;
      
      dialog.setTextLabel("ad61,ad62",sPrintF(answer, "%g,%g", dbase.get<real >("ad61"), dbase.get<real >("ad62"))); 
    }
    else if( len=answer.matches("ad21n,ad22n") )
    {
      sScanF(answer(len,answer.length()),"%e %e",& dbase.get<real >("ad21n"),& dbase.get<real >("ad22n"));
      // cout << " ad21n=" <<  dbase.get<real >("ad21n") << " ad22n=" <<  dbase.get<real >("ad22n") << endl;
      
      dialog.setTextLabel("ad21n,ad22n",sPrintF(answer, "%g,%g", dbase.get<real >("ad21n"), dbase.get<real >("ad22n"))); 
    }
    else if( len=answer.matches("ad41n,ad42n") )
    {
      sScanF(answer(len,answer.length()),"%e %e",& dbase.get<real >("ad41n"),& dbase.get<real >("ad42n"));
      // cout << " ad41n=" <<  dbase.get<real >("ad41n") << " ad42n=" <<  dbase.get<real >("ad42n") << endl;
      
      dialog.setTextLabel("ad41n,ad42n",sPrintF(answer, "%g,%g", dbase.get<real >("ad41n"), dbase.get<real >("ad42n"))); 
    }
//    for backward compatibility:
    else if( answer=="project initial conditions" ){ dbase.get<bool >("projectInitialConditions")=true;} //
//
    else if( dialog.getToggleValue(answer,"project initial conditions", dbase.get<bool >("projectInitialConditions")) ){}//
    else if( dialog.getToggleValue(answer,"second-order artificial diffusion", dbase.get<bool >("useSecondOrderArtificialDiffusion")) ){}//
    else if( dialog.getToggleValue(answer,"fourth-order artificial diffusion", dbase.get<bool >("useFourthOrderArtificialDiffusion")) )
    {
//       if(  dbase.get<int >("orderOfAccuracy")==2 &&  dbase.get<bool >("useFourthOrderArtificialDiffusion") )
// 	 dbase.get<int >("extrapolateInterpolationNeighbours")=true;
//       else
// 	 dbase.get<int >("extrapolateInterpolationNeighbours")=false;
       if( ( dbase.get<int >("orderOfAccuracy")==2 && numberOfGhostPointsNeeded()>=2 ) ||
	   ( dbase.get<int >("orderOfAccuracy")==4 && numberOfGhostPointsNeeded()>=3 ) )
       {
	 dbase.get<int >("extrapolateInterpolationNeighbours")=true;
       }
       else
       {
	 dbase.get<int >("extrapolateInterpolationNeighbours")=false;
       }
    }
    else if( dialog.getToggleValue(answer,"sixth-order artificial diffusion", dbase.get<bool >("useSixthOrderArtificialDiffusion")) )
    { 
      if(  dbase.get<int >("orderOfAccuracy")==4 &&  dbase.get<bool >("useSixthOrderArtificialDiffusion") )
	 dbase.get<int >("extrapolateInterpolationNeighbours")=true;
      else
	 dbase.get<int >("extrapolateInterpolationNeighbours")=false;
    }
    else if( dialog.getToggleValue(answer,"use implicit fourth-order artificial diffusion",
                                            dbase.get<bool >("useImplicitFourthArtificialDiffusion")) ){}//
// ** this next value is an int -- why?
//      else if( dialog.getToggleValue(answer,"use split-step implicit artificial diffusion",
//                                             useSplitStepImplicitArtificialDiffusion) ){}//
    else if( answer.matches("use split-step implicit artificial diffusion") )
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("use split-step implicit artificial diffusion",value);      
       dbase.get<int >("useSplitStepImplicitArtificialDiffusion")=value;
      if(  dbase.get<int >("useSplitStepImplicitArtificialDiffusion") )
	printF("use the split step implicit artificial diffusion\n");
      else
	printF("Do not use the split step implicit artificial diffusion\n");
    }
    else if( len=answer.matches("SA scale factor") )
    {
      sScanF(&answer[len],"%e",&spalartAllmarasScaleFactor);
      dialog.setTextLabel("SA scale factor",sPrintF(answer, "%g",spalartAllmarasScaleFactor));
    }
    else if( len=answer.matches("SA distance scale") )
    {
      sScanF(&answer[len],"%e",&spalartAllmarasDistanceScale);
      dialog.setTextLabel("SA distance scale",sPrintF(answer, "%g",spalartAllmarasDistanceScale));
    }
    else if( len=answer.matches("passive scalar diffusion coefficient") )
    {
      sScanF(&answer[len],"%e",& dbase.get<real >("nuPassiveScalar"));
      dialog.setTextLabel("passive scalar diffusion coefficient",sPrintF(answer, "%g", dbase.get<real >("nuPassiveScalar")));
    }
    else if( answer.matches("turbulence trip positions") )
    {
      assert(  dbase.get<int >("numberOfDimensions")==2 );

      IntegerArray values;
      int numRead=gi.getValues("Enter positions of trips grid,i1,i2,i3",values);
      if( numRead>3 )
      {
	 dbase.get<IntegerArray >("turbulenceTripPoint").redim(4,numRead/4);
	for( int i=0; i<numRead/4; i++ )
	{
	   dbase.get<IntegerArray >("turbulenceTripPoint")(0,i)=values(i*4);
	   dbase.get<IntegerArray >("turbulenceTripPoint")(1,i)=values(i*4+1);
	   dbase.get<IntegerArray >("turbulenceTripPoint")(2,i)=values(i*4+2);
	   dbase.get<IntegerArray >("turbulenceTripPoint")(3,i)=values(i*4+3);
	  printF("Setting trip point %i : (grid=%i,i1=%i,i2=%i,i3=%i)\n", dbase.get<IntegerArray >("turbulenceTripPoint")(0,i),
                      dbase.get<IntegerArray >("turbulenceTripPoint")(1,i), dbase.get<IntegerArray >("turbulenceTripPoint")(2,i), dbase.get<IntegerArray >("turbulenceTripPoint")(3,i));
	}
      }
      else
      {
	 dbase.get<IntegerArray >("turbulenceTripPoint").redim(0);
      }
      
    }
    else if( answer=="use Neumann BC at outflow" )
    {
      dbase.get<int>("outflowOption")=1;
    }
    else if( answer=="use extrapolate BC at outflow" )
    {
      dbase.get<int>("outflowOption")=0;
    }
    else if(  dbase.get<ListOfShowFileParameters >("pdeParameters").matchAndSetValue( answer ) )
    {
      printF("*** answer=[%s] was found as a user defined parameter\n",(const char*)answer);
      
    }
    else if ( answer=="cylindrical axis is y axis" )
      {
	printf("Sorry - currently only the x axis can be the axis of symmetry for the incompressible NS.\n");
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

  updatePDEparameters();  // update parameters such as ReynoldsNumber, MachNumber, ... to be consistent.

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

 return returnValue;
}



int InsParameters::
displayPdeParameters(FILE *file /* = stdout */ )
// =====================================================================================
// /Description:
//   Display PDE parameters
// =====================================================================================
{
  const char *offOn[2] = { "off","on" };

  fprintf(file,
	  "PDE parameters: equation is `incompressible Navier Stokes'.\n");
  if( dbase.get<bool >("advectPassiveScalar"))
    fprintf(file,
	    "  with passive scalar advection. Diffusion coefficient for passive scalar is %8.2e\n", dbase.get<real >("nuPassiveScalar"));

  // The  dbase.get<DataBase >("modelParameters") will be displayed here:
  Parameters::displayPdeParameters(file);

  fprintf(file,
	  "  number of components is %i\n"
	  "  nu=%e, (kinematic viscosity)\n"
	  "  divergence damping coefficient=%e\n"
	  "  The (artificial) boundary condition p.n=0 is turned %s \n"
	  "  2nd order artificial viscosity is %s, ad21=%f, ad22=%f\n"
	  "  4th order artificial viscosity is %s, ad41=%f, ad42=%f\n"
	  "  6th order artificial viscosity is %s, ad61=%f, ad62=%f\n",
	   dbase.get<int >("numberOfComponents"),
	   dbase.get<real >("nu"),
	   dbase.get<real >("cdv"),
	  offOn[int( dbase.get<int >("pressureBoundaryCondition")==2)],
	  offOn[dbase.get<bool >("useSecondOrderArtificialDiffusion")],
	   dbase.get<real >("ad21"), dbase.get<real >("ad22"),
	  offOn[dbase.get<bool >("useFourthOrderArtificialDiffusion")],
	   dbase.get<real >("ad41"), dbase.get<real >("ad42"),
	  offOn[dbase.get<bool >("useSixthOrderArtificialDiffusion")],
	   dbase.get<real >("ad61"), dbase.get<real >("ad62"));

  if(  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0]!=0. ||  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]!=0. ||  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]!=0. )
    fprintf(file," gravity is on, acceleration due to gravity = (%8.2e,%8.2e,%8.2e) \n",
	     dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
            
  return 0;
}




//\begin{>>InsParametersInclude.tex}{\subsection{updateShowFile}} 
int InsParameters::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{InsParametersInclude.tex}  
// =================================================================================================
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  // save parameters
  showFileParams.push_back(ShowFileParameter("incompressibleNavierStokes","pde"));
    
  showFileParams.push_back(ShowFileParameter("isAxisymmetric",isAxisymmetric()));
  showFileParams.push_back(ShowFileParameter("axisymmetricBoundaryCondition",(int)(Parameters::axisymmetric)));

  showFileParams.push_back(ShowFileParameter("nu", dbase.get<real >("nu")));
  showFileParams.push_back(ShowFileParameter("reynoldsNumber", dbase.get<real >("reynoldsNumber")));

  if(  dbase.get<int >("tc")>=0 )
  {
    showFileParams.push_back(ShowFileParameter("kThermal", dbase.get<real >("kThermal")));
    showFileParams.push_back(ShowFileParameter("thermalConductivity", dbase.get<real >("thermalConductivity")));
  }
    
  // here are the new names for the velocity components:
  showFileParams.push_back(ShowFileParameter("v1Component", dbase.get<int>("uc")));
  showFileParams.push_back(ShowFileParameter("v2Component", dbase.get<int>("vc")));
  showFileParams.push_back(ShowFileParameter("v3Component", dbase.get<int>("wc")));

  // Now save parameters common to all solvers:
  Parameters::saveParametersToShowFile();    

  return 0;
}

// ===================================================================================================================
/// \brief Return the number of ghost lines needed by this method. This is effectively the half width of the stencil. 
/// \note Fourth-order dissipation requires 2 ghost lines.
///       The visco-plastic model uses 2 ghost lines since the coefficient of viscosity depends on the first derivatives of u.
// ==================================================================================================================
int InsParameters::
numberOfGhostPointsNeeded() const  // number of ghost points needed by this method.
{
  int numGhost = Parameters::numberOfGhostPointsNeeded();
  const int orderOfAccuracy=dbase.get<int >("orderOfAccuracy");
  if( orderOfAccuracy==4 ||
      ( dbase.get<bool >("useFourthOrderArtificialDiffusion") && ( dbase.get<real >("ad41")!=0. ||  dbase.get<real >("ad42")!=0.)) ||
      ( orderOfAccuracy==2 && dbase.get<InsParameters::PDEModel >("pdeModel")==viscoPlasticModel) )
  {
    numGhost=max(numGhost,2);
  }
  else if( orderOfAccuracy==6 ||
           dbase.get<bool >("useSixthOrderArtificialDiffusion") ||
           ( orderOfAccuracy==4 && dbase.get<InsParameters::PDEModel >("pdeModel")==viscoPlasticModel) )
  {
    numGhost=max(numGhost,3);
  }
  
  return numGhost;
}

// ===================================================================================================================
/// \brief return the number of ghost points needed by this method for the implicit matrix.
///
// ==================================================================================================================
int InsParameters::
numberOfGhostPointsNeededForImplicitMatrix() const  
{
  // NOTE: we do not include the extra ghost for the visco-plastic model in the matrix
  int numGhost = Parameters::numberOfGhostPointsNeeded();
  const int orderOfAccuracy=dbase.get<int >("orderOfAccuracy");
  if( orderOfAccuracy==4 ||
      ( dbase.get<bool >("useFourthOrderArtificialDiffusion") && 
        ( dbase.get<real >("ad41")!=0. ||  dbase.get<real >("ad42")!=0.)) )
  {
    numGhost=max(numGhost,2);
  }
  else if( orderOfAccuracy==6 ||
           dbase.get<bool >("useSixthOrderArtificialDiffusion") )
  {
    numGhost=max(numGhost,3);
  }
  
  return numGhost;
}


int InsParameters::
getComponents( IntegerArray &component )
//==================================================================================
// /Description:
//    Get an array of component indices
//
// /component (output): the list of component indices
//\end{ParametersInclude.tex} 
//=================================================================================
{
  int numberToSet=dbase.get<int >("numberOfDimensions");  // this may not be correct 
  if ( dbase.get<int >("tc")>=0 ) numberToSet++;
  component.redim(numberToSet);
  int n=0;
  component(n++)=dbase.get<int >("uc");
  component(n++)=dbase.get<int >("vc");
  if( dbase.get<int >("wc")>=0 ) component(n++)=dbase.get<int >("wc");
  if( dbase.get<int >("tc")>=0 ) component(n++)=dbase.get<int >("tc");
  
  return 0;
}

//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

// ============================================================================================
/// \brief Assign the default values for the data required by the boundary conditions.
// ============================================================================================
int InsParameters::
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg)
{
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  RealArray & bcData = dbase.get<RealArray>("bcData");

  Range all;
  const Range & Ru = dbase.get<Range >("Ru");
  const int & pc = dbase.get<int >("pc");
  const int & tc = dbase.get<int >("tc");
  
  switch( cg[grid].boundaryCondition(side,axis) ) 
  {
  case InsParameters::inflowWithVelocityGiven:
  case InsParameters::noSlipWall:
    // data is set n.u = ...
    bcData(Ru,side,axis,grid)=0.;
    // The default BC for T is dirichlet (set int Parameters.C: updateToMatchGrid)
    if( tc>=0 )  // *wdh* 110202 -- set default BC for T to be dirichlet
    {
      mixedRHS(tc,side,axis,grid)=0.;
      mixedCoeff(tc,side,axis,grid)=1.;
      mixedNormalCoeff(tc,side,axis,grid)=0.;
    }
    
    break;
  case InsParameters::outflow:
  case InsParameters::tractionFree:
   //  data is a*p+b*p.n = c
//     bcData(0,side,axis,grid)=1.;  // *wdh* 070704
//     bcData(1,side,axis,grid)=1.;
//     bcData(2,side,axis,grid)=0.;
    
    mixedRHS(pc,side,axis,grid)=0.;
    mixedCoeff(pc,side,axis,grid)=1.;
    mixedNormalCoeff(pc,side,axis,grid)=1.;
    //  printF("*** InsParameters::setDefaultDataForABC: set default pressure outflow BC to p+p.n=0 ****\n");
    break;

  case symmetry:
  case axisymmetric:
    if( tc >= 0 )
    {
      // set the default BC for T to be Neumann  *wdh* 080725 
      printF(" **************** set default BC for T to Neumann on bc0=%i ************",cg[grid].boundaryCondition(side,axis));

      mixedRHS(tc,side,axis,grid)=0.;
      mixedCoeff(tc,side,axis,grid)=0.;
      mixedNormalCoeff(tc,side,axis,grid)=1.;
    }
    break ;

  }
  return 0;
  
}

//\begin{>>InsParametersInclude.tex}{\subsubsection{getDerivedFunction}}
int InsParameters::
getDerivedFunction( const aString & name, const realMappedGridFunction & uIn,
                    realMappedGridFunction & vIn, 
                    const int grid, const int component, const real t, 
                    Parameters & parameters
                    )
//==================================================================================
// /Description:
//     Assign the values of a derived quantity
//
// /name (input): the name of the grid function on the database.
// /u (input) : evaluate the derived function using this grid function
// /v (input) : fill in a component of this grid function
// /component : component index to fill, i.e. fill v(all,all,all,component)
//\end{InsParametersInclude.tex} 
//=================================================================================
{
  MappedGrid & mg = *vIn.getMappedGrid();

  Index all;
  // Index I1,I2,I3;

  const int rc=parameters.dbase.get<int >("rc");
  const int tc=parameters.dbase.get<int >("tc");
  const int sc=parameters.dbase.get<int >("sc");
  const int pc=parameters.dbase.get<int >("pc");

  #ifdef USE_PPP
   realSerialArray v; getLocalArrayWithGhostBoundaries(vIn,v);
   realSerialArray u; getLocalArrayWithGhostBoundaries(uIn,u);
  #else
    realSerialArray & v = vIn;
    const realSerialArray & u = uIn;
  #endif

  if( name=="pressure" || 
      name=="ps" || name=="pg" ) 
  {
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3); // only compute derivatives here 
    if( parameters.dbase.get<real >("fluidDensity")!=0. )
      v(all,all,all,component)=u(I1,I2,I3,pc); // for backward compatibility
    else
      v(all,all,all,component)=u(I1,I2,I3,pc)*parameters.dbase.get<real >("fluidDensity");  // *wdh* 062406 -- from Dominic Chandar
  }
  else if( name=="viscosity" ||
           name=="viscoPlasticYield" || name=="yield" ||
           name=="viscoPlasticStrainRate" || name=="eDot" ||
           name=="viscoPlasticVariables" ||                      // this means evaluate both 
           name=="sigmaxx" ||
           name=="sigmaxy" ||
           name=="sigmaxz" ||
           name=="sigmayy" ||
           name=="sigmayz" ||
           name=="sigmazz"  )
  {
    // evaluate variables for the visco-plastic model

    getViscoPlasticVariables( name, uIn, vIn, grid,component,t );

  }
  else
  {
    printf("getDerivedFunction:ERROR: unknown derived function! name=%s\n",(const char*)name);
    return 1;
  }

  return 0;

}

//\begin{>>InsParametersInclude.tex}{\subsubsection{getNormalForce}}
int InsParameters::
getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar )
//==================================================================================
// /Description:
//     Return the normal force (traction) on a boundary. This routine is called, for example,
//  by MovingGrids::rigidBodyMotion to determine the motion of a rigid body.
//
// /u (input): solution to compute the force from.
// /normalForce (output) : fill in the components of the normal force. 
// /ipar (input) : integer parameters. The boundary is defined by 
//           grid=ipar[0], side=ipar[1], axis=ipar[2]
// /rpar (input) : real parameters. The current time is t=rpar[0]
//\end{InsParametersInclude.tex} 
//=================================================================================
{
  int grid=ipar[0], side=ipar[1], axis=ipar[2];
  real time =rpar[0];
  
  CompositeGrid & cg = *u.getCompositeGrid();
  assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
  assert( grid>=0 && grid<cg.numberOfComponentGrids());

  const int uc=dbase.get<int >("uc");
  const int vc=dbase.get<int >("vc");
  const int wc=dbase.get<int >("wc");
  const int pc=dbase.get<int >("pc");
  const real nu = dbase.get<real >("nu");
  const Range V(uc,uc+cg.numberOfDimensions()-1);


  MappedGrid & mg = cg[grid];
  mg.update(MappedGrid::THEvertexBoundaryNormal);  // fix this ********************
      
  realSerialArray & fn = normalForce;

  // const intArray & mask = mg.mask();
  #ifdef USE_PPP
    realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
    // realSerialArray vertexLocal; getLocalArrayWithGhostBoundaries(mg.vertex(),vertexLocal);
    realSerialArray uLocal;      getLocalArrayWithGhostBoundaries(u[grid],uLocal);
    // intSerialArray  maskLocal;   getLocalArrayWithGhostBoundaries(mask,maskLocal);
  #else
    realArray & normal = mg.vertexBoundaryNormal(side,axis);
    // realArray & vertexLocal = mg.vertex();
    realArray & uLocal = u[grid];
    // intSerialArray & maskLocal = mask;
  #endif
      
  Index Ib1,Ib2,Ib3;
  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
  int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,Ib1,Ib2,Ib3,includeGhost);
  if( !ok ) return 0;

  CompositeGridOperators & cgop = *u.getOperators();
  MappedGridOperators & op = cgop[grid];
	  
  realSerialArray ux(Ib1,Ib2,Ib3,V), uy(Ib1,Ib2,Ib3,V), uz;
  op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,V);
  op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,V);
  if( mg.numberOfDimensions()>=3 )
  {
    uz.redim(Ib1,Ib2,Ib3,V);
    op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,V);
  }
	  
  real fluidDensity = dbase.get<real>("fluidDensity")!=0. ? dbase.get<real>("fluidDensity") : 1.;

  // printF("InsParameters::getNormalForce: fluidDensity=%g --> %g \n",dbase.get<real>("fluidDensity"), fluidDensity);

  // ----------------------------------------------------------------------------------------------
  // -- NOTE: In the incompressible equations "p" is really p/rho so we need to multiply by rho ---
  // ----------------------------------------------------------------------------------------------

  const real mu = nu*fluidDensity;  // *wdh* 2013/01/23 

  if( cg.numberOfDimensions()==2 )
  {
    fn(Ib1,Ib2,Ib3,0)=( fluidDensity*uLocal(Ib1,Ib2,Ib3,pc)*normal(Ib1,Ib2,Ib3,0)
			-(mu*((ux(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
			      (uy(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)) ) );
    fn(Ib1,Ib2,Ib3,1)=( fluidDensity*uLocal(Ib1,Ib2,Ib3,pc)*normal(Ib1,Ib2,Ib3,1)
			-(mu*((ux(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
			      (uy(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)) ) );
  }
  else
  {
    fn(Ib1,Ib2,Ib3,0)=( fluidDensity*uLocal(Ib1,Ib2,Ib3,pc)*normal(Ib1,Ib2,Ib3,0)
			-(mu*((ux(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
			      (uy(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)+ 
			      (uz(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,wc))*normal(Ib1,Ib2,Ib3,2)) ) );

    fn(Ib1,Ib2,Ib3,1)=( fluidDensity*uLocal(Ib1,Ib2,Ib3,pc)*normal(Ib1,Ib2,Ib3,1)
			-(mu*((ux(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
			      (uy(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)+ 
			      (uz(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,wc))*normal(Ib1,Ib2,Ib3,2)) ) );

    fn(Ib1,Ib2,Ib3,2)=( fluidDensity*uLocal(Ib1,Ib2,Ib3,pc)*normal(Ib1,Ib2,Ib3,2)
			-(mu*((ux(Ib1,Ib2,Ib3,wc)+uz(Ib1,Ib2,Ib3,uc))*normal(Ib1,Ib2,Ib3,0)+
			      (uy(Ib1,Ib2,Ib3,wc)+uz(Ib1,Ib2,Ib3,vc))*normal(Ib1,Ib2,Ib3,1)+ 
			      (uz(Ib1,Ib2,Ib3,wc)+uz(Ib1,Ib2,Ib3,wc))*normal(Ib1,Ib2,Ib3,2)) ) );
  }

  return 0;
}


bool
InsParameters::isMixedBC(int bc) 
{ 
  return  bc==InsParameters::outflow ||       
    //    bc==Parameters::subSonicOutflow || 
    bc==InsParameters::convectiveOutflow ||
    bc==InsParameters::tractionFree;
}
