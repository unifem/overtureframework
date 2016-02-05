// ========================================================================================================
/// \class CnsParameters
/// \brief This class holds parameters for Cgcns.
// ========================================================================================================

#include "CnsParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "Reactions.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "EquationDomain.h"
#include "FlowSolutions.h"
#include "NameList.h"
#include "GridFunction.h"
#include "MultiComponent.h"

#define ADDPSI EXTERN_C_NAME(addpsi)
#define CONSPRIM EXTERN_C_NAME(consprim)

using namespace CG;

extern "C"
{
  void ADDPSI(const int & nd1a, const int & nd1b, const int & nd2a, const int & nd2b,
              const real & fact, const real & rho, real & u);

  void CONSPRIM(const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b, 
		const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b, 
		const int &nd,const int &ns, 
		const int &rc,const int &uc,const int &vc,const int &wc,const int &tc, const int &sc,
		real & q, const int & mask, const real & val, const int & ipar, const real & rpar, 
                const int & option, const int & fixup, const real & epsRho );

}

int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);


// #include "EquationDomain.h"
// ListOfEquationDomains equationDomainList; // This is in the global name space for now.

// #include "SurfaceEquation.h"
// SurfaceEquation surfaceEquation;  // This is in the global name space for now.


//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in CnsParameters}} 
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
CnsParameters::
CnsParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
// ==================================================================================
// /pde0: Indicated which PDE we are solving
//
//\end{ParametersInclude.tex}
//===================================================================================
{
  Parameters::pdeName ="compressibleNavierStokes";

  dbase.get<DataBase >("modelParameters").put<real>("gamma", dbase.get<real >("gamma"));
  dbase.get<DataBase >("modelParameters").put<real>("mu", dbase.get<real >("mu"));
  dbase.get<DataBase >("modelParameters").put<real>("kThermal", dbase.get<real >("kThermal"));
  
  if (!dbase.has_key("riemannSolver")) dbase.put<RiemannSolverEnum>("riemannSolver");
  if (!dbase.has_key("pdeVariation")) dbase.put<PDEVariation>("pdeVariation");
  if (!dbase.has_key("conservativeGodunovMethod")) dbase.put<CnsParameters::GodunovVariation>("conservativeGodunovMethod");
  if (!dbase.has_key("orderOfAccuracyForGodunovMethod")) dbase.put<int>("orderOfAccuracyForGodunovMethod");
  if (!dbase.has_key("godunovArtificialViscosity")) dbase.put<real>("godunovArtificialViscosity");
  if (!dbase.has_key("equationOfState")) dbase.put<CnsParameters::EquationOfStateEnum>("equationOfState");
  if (!dbase.has_key("pde")) dbase.put<PDE>("pde");
  if (!dbase.has_key("testProblem")) dbase.put<CnsParameters::TestProblems>("testProblem");
  if (!dbase.has_key("strickwerdaCoeff")) dbase.put<real>("strickwerdaCoeff",1./6.);
  if (!dbase.has_key("thermalConductivity")) dbase.put<real>("thermalConductivity",1.);

  // Lower bounds for density and pressure
  if (!dbase.has_key("densityLowerBound")) dbase.put<real>("densityLowerBound",1.e-5);
  if (!dbase.has_key("pressureLowerBound")) dbase.put<real>("pressureLowerBound",1.e-6);
  if (!dbase.has_key("velocityLimiterEps")) dbase.put<real>("velocityLimiterEps",1.e-4);

  // Add a fix for wall heating/cooling: 
  if (!dbase.has_key("checkForWallHeating")) dbase.put<int>("checkForWallHeating",0);

  // offset for the pressure when computing the boundary force: (used for multiphysics problems)
  if (!dbase.has_key("boundaryForcePressureOffset")) dbase.put<real>("boundaryForcePressureOffset",0.);

  if (!dbase.has_key("numberOfAdvectedScalars")) dbase.put<int>("numberOfAdvectedScalars");
  dbase.get<int >("numberOfAdvectedScalars")=0;
  

  dbase.get<PDE>("pde")=compressibleNavierStokes;
  dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver")=roeRiemannSolver;
  dbase.get<CnsParameters::PDEVariation >("pdeVariation")=nonConservative;
  dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod")=fortranVersion; 
  dbase.get<int >("orderOfAccuracyForGodunovMethod")=2;
  dbase.get<real >("godunovArtificialViscosity")=.5;  // for Don's Godunov Solver
  dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")=idealGasEOS;

  dbase.get<int >("scalarSystemForImplicitTimeStepping")=false;

  registerBC((int)outflow,"outflow");
  registerBC((int)superSonicInflow,"superSonicInflow");
  registerBC((int)superSonicOutflow,"superSonicOutflow");
  registerBC((int)subSonicInflow,"subSonicInflow");
  registerBC((int)subSonicOutflow,"subSonicOutflow");
  registerBC((int)farField,"farField");
  
  // kkc 070131 NOTE : these three would have been "invalid" in the old bc checking stuff
  //                   but were still referred to in the setDefaultDataForABC...
  registerBC((int)inflowWithVelocityGiven,"inflowWithVelocityGiven");
  registerBC((int)tractionFree,"tractionFree");
  registerBC((int)convectiveOutflow,"convectiveOutflow");

  // kkc 090402 add the multi-component material manager 
  if ( !dbase.has_key("Mixture")) dbase.put<MixtureP>("Mixture", new IdealGasMixture() );
  CG::setMixtureContext(*dbase.get<MixtureP>("Mixture"));

  // initialize the items that we time: 
  initializeTimings();

}

CnsParameters::
~CnsParameters()
{
}


int CnsParameters::
setParameters(const int & numberOfDimensions0 /* =2 */ , 
              const aString & reactionName_ /* =nullString */ )
// ==================================================================================================
//  /reactionName (input) : optional name of a reaction oe a reaction 
//     file that defines the chemical reactions, such as
//      a Chemkin binary file. 
// ==================================================================================================
{
  const CnsParameters::PDE & pde = dbase.get<CnsParameters::PDE >("pde");
  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");

  int & numberOfComponents = dbase.get<int >("numberOfComponents");
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  int & rc = dbase.get<int >("rc");
  int & uc = dbase.get<int >("uc");
  int & vc = dbase.get<int >("vc");
  int & wc = dbase.get<int >("wc");
  int & pc = dbase.get<int >("pc");
  int & tc = dbase.get<int >("tc");
  int & ec = dbase.get<int >("ec");
  int & kc = dbase.get<int >("kc");
  int & sc = dbase.get<int >("sc");
  int & sec = dbase.get<int >("sec");
  int & epsc = dbase.get<int >("epsc");
  Parameters::TurbulenceModel & turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  int & numberOfSpecies = dbase.get<int >("numberOfSpecies");
  int & numberOfExtraVariables = dbase.get<int >("numberOfExtraVariables");
  int & numberOfAdvectedScalars = dbase.get<int >("numberOfAdvectedScalars");
  
  CnsParameters::EquationOfStateEnum & equationOfState = dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
  const ReactionTypeEnum & reactionType = dbase.get<CnsParameters::ReactionTypeEnum >("reactionType");
  
  aString* & componentName = dbase.get<aString* >("componentName");
  aString & reactionName = dbase.get<aString >("reactionName");
  Range & Rt = dbase.get<Range >("Rt");
  Range & Ru = dbase.get<Range >("Ru");

  numberOfDimensions=numberOfDimensions0;
  //   dbase.get<CnsParameters::PDE >("pde")=pde0;
  rc= uc= vc= wc= pc= tc= sc= kc= epsc= sec=-1;
  int firstAdvectedScalar=-1;
  
  reactionName=reactionName_;
  if( reactionName!=nullString &&  reactionName!="" )
  {
    dbase.get<bool >("computeReactions")=true;
    // This next function will assign the number of species and build a reaction object.
    buildReactions();
  }
  else
  {
    dbase.get<bool >("computeReactions")=false;
    dbase.get<Reactions* >("reactions")=NULL;
    numberOfSpecies=0;
 
  }
  if( conservativeGodunovMethod==multiComponentVersion )
  {
    // add extra species for the multi component case
    if( equationOfState!=jwlEOS )
    {
      numberOfSpecies+=1;
    }
    else
    {
      // buildReactions alread added lambda, vs, and vg
      // ...we still need vi and  dbase.get<real >("mu") added
      numberOfSpecies+=2;
    }
  }
  else if( conservativeGodunovMethod==multiFluidVersion )
  {
    // add extra species for the multi fluid case
    if( equationOfState==idealGasEOS )
    {
      // multi-fluid option with ideal EOS => 5 components (rho, rho*v1, rho*v2, rho*E, mu1)
      numberOfSpecies+=1;
      if( reactionType!=noReactions )
      {
        // With any reaction rate with ideal EOS => 8 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3), 
	numberOfSpecies+=3;
      }
    }
    else if( equationOfState==stiffenedGasEOS )
    {
       // multi-fluid option with stiffened EOS => 6 components (rho, rho*v1, rho*v2, rho*E, mu1, mu2)
       numberOfSpecies+=2;
      if( reactionType!=noReactions )
      {
        // With any reaction rate with stiffened EOS => 10 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3, mu4, mu5)
	numberOfSpecies+=4;
      }
    }
    else
    {
      printF("ERROR:multiFluidVersion: eos=%i is not supported yet\n",(int)equationOfState);
      OV_ABORT("error");
    }
    if( pde==compressibleMultiphase )
    {
      // compressible multi-phase also includes "lambda"
       numberOfSpecies+=1;
    }
    

  }

  int s, i;
  //...set component index'es, showVariables, etc. that are equation-specific
  switch ( pde)
  {
  case compressibleNavierStokes: 
  {
    numberOfComponents=0;
    rc= numberOfComponents++;    //  density = u(all,all,all, rc)
    uc= numberOfComponents++;    //  u velocity component = u(all,all,all, uc)
    if(  numberOfDimensions>1 )  vc= numberOfComponents++;
    if(  numberOfDimensions>2 ||  dbase.get<bool >("axisymmetricWithSwirl") )  wc= numberOfComponents++;
    tc= numberOfComponents++;

    pc  =  tc;
    if ( conservativeGodunovMethod==cppVersionII) 
      ec =  numberOfComponents++;
    else
      ec= pc;

    Ru=Range( uc, uc+ numberOfDimensions-1);    // velocity components
    Rt=Range( rc, tc);      // time dependent components

    if(  numberOfSpecies>0 )
    {
       sc= numberOfComponents;    //  sc marks the first species
       numberOfComponents+= numberOfSpecies;
       Rt=Range( rc, numberOfComponents-1);      // time dependent components
    }
    if( numberOfAdvectedScalars>0 )
    {
      if( sc<0 ) sc=numberOfComponents;            //  sc marks the first species
      numberOfSpecies+=numberOfAdvectedScalars;    // advected scalars are treated as species so that BC's are applied. 
      firstAdvectedScalar=numberOfComponents;
      numberOfComponents+= numberOfAdvectedScalars;  // marks where the advected scalars start
      Rt=Range( rc, numberOfComponents-1);      // time dependent components
    }

    if(  dbase.get<int >("numberOfSurfaceEquationVariables")>0 )
    {
       sec= numberOfComponents;
       numberOfComponents+= dbase.get<int >("numberOfSurfaceEquationVariables");
       Rt=Range( rc, numberOfComponents-1);      // time dependent components
    }
    if(  numberOfExtraVariables>0 )
    {
       numberOfComponents+= numberOfExtraVariables;
    }
 

    addShowVariable( "rho", rc );
    addShowVariable( "u", uc );
    if(  numberOfDimensions>1 )
      addShowVariable( "v", vc );
    if(  wc>=0 )
      addShowVariable( "w", wc );

    addShowVariable( "T", tc );

     dbase.get<RealArray >("artificialDiffusion").redim( numberOfComponents);
     dbase.get<RealArray >("artificialDiffusion")=0.;


    if(  numberOfSpecies>0 )  //  dbase.get<Parameters::PDEVariation >("pdeVariation")==conservativeGodunov )
    { // add species here so they appear in the  dbase.get<Ogshow* >("show") file in an order that
      // can be read back in
      int scp =  sc;
      if( conservativeGodunovMethod==multiComponentVersion )
      {
        addShowVariable( "mu",scp,true ); 
	if( reactionName=="ignition and growth" )
	{
	  addShowVariable( "lambda",scp+1,true );
	  addShowVariable( "vi",   scp+2,true );
	  addShowVariable( "vs",   scp+3,true );
	  addShowVariable( "vg",   scp+4,true );
	}
	else if( reactionName=="one step" ||  reactionName=="one step pressure law" )
	{
	  addShowVariable( "lambda",scp+1,true );
	}
	else if( reactionName=="ignition and growth desensitization" )
	{
	  addShowVariable( "lambda",scp+1,true );
	  addShowVariable( "phi",   scp+2,true );
	  addShowVariable( "vi",    scp+3,true );
	  addShowVariable( "vs",    scp+4,true );
	  addShowVariable( "vg",    scp+5,true );
	}
      }
      else if( conservativeGodunovMethod==multiFluidVersion )
      {
        // --- Don's multi-fluid version ---
	if( equationOfState==idealGasEOS )
	{
	  // multi-fluid option with ideal EOS => 5 components (rho, rho*v1, rho*v2, rho*E, mu1)
	  if( reactionType==noReactions )
	  {
            addShowVariable( "mu1",scp,true ); 
	  }
	  else
	  {
	    // With any reaction rate with ideal EOS => 8 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3), 
	    addShowVariable( "lambda",scp  ,true ); 
	    addShowVariable( "mu1"   ,scp+1,true ); 
	    addShowVariable( "mu2"   ,scp+2,true ); 
	    addShowVariable( "mu3"   ,scp+3,true ); 
	  }
	}
	else if( equationOfState==stiffenedGasEOS )
	{
	  // multi-fluid option with stiffened EOS => 6 components (rho, rho*v1, rho*v2, rho*E, mu1, mu2)
	  if( reactionType==noReactions )
	  {
            addShowVariable( "mu1",scp  ,true ); 
	    addShowVariable( "mu2",scp+1,true ); 
	  }
	  if( reactionType!=noReactions )
	  {
	    // With any reaction rate with stiffened EOS => 10 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3, mu4, mu5)
	    addShowVariable( "lambda",scp  ,true ); 
	    addShowVariable( "mu1"   ,scp+1,true ); 
	    addShowVariable( "mu2"   ,scp+2,true ); 
	    addShowVariable( "mu3"   ,scp+3,true ); 
	    addShowVariable( "mu4"   ,scp+4,true ); 
	    addShowVariable( "mu5"   ,scp+5,true ); 
	  }
	}
	else
	{
	  printF("multiFluidVersion:ERROR: unexpected equationOfState=%i\n",(int)equationOfState);
	  OV_ABORT("error");
	}
	
      }
      else if( reactionName=="one step" )
      {
	addShowVariable( "lambda",scp,true ); 
      }
      else if( reactionName=="branching" )
      {
        addShowVariable( "f",scp  ,true );
        addShowVariable( "y",scp+1,true );
      }
      else if( reactionName=="one equation mixture fraction" )
      {
        addShowVariable( "f",scp  ,true );
      }
      else if( reactionName=="two equation mixture fraction and extent of reaction" )
      {
        addShowVariable( "f",scp  ,true );
        addShowVariable( "c",scp+1  ,true );
      }
      else if( reactionName=="ignition and growth" )
      {
        addShowVariable( "lambda",scp,true );
        addShowVariable( "vs",scp+1,true );
        addShowVariable( "vg",scp+2,true );
      }
      else if( reactionName=="ignition and growth desensitization" )
      {
        addShowVariable( "lambda",scp,true );
        addShowVariable( "phi",   scp+1,true );
        addShowVariable( "vs",    scp+2,true );
        addShowVariable( "vg",    scp+3,true );
      }
      else if( conservativeGodunovMethod!=multiComponentVersion )
      {
	printF("CnsParameters:ERROR: unknown reaction=[%s]\n",(const char *) reactionName);
	Overture::abort("error");
      }
   
    }

    if(  dbase.get<int >("numberOfSurfaceEquationVariables")>0 )
    {
      addShowVariable("Tb", sec,true); 
    }

    Parameters::TimeSteppingMethod &timeSteppingMethod = dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
    if ( timeSteppingMethod==Parameters::implicit || timeSteppingMethod==Parameters::steadyStateNewton )
      setGridIsImplicit(-1,1);

    if( (dbase.get<PDEVariation >("pdeVariation")!=nonConservative) ||
	dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit)
    {
      // *wdh* 070506 -- make 2 default from 1 for better accuracy (quarterSphere case) -- we may need a limited
      // extrapolation for hard cases
      dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours")=1; 
      dbase.get<int >("orderOfExtrapolationForSecondGhostLine")=1;
    }

    break;
  }
  case compressibleMultiphase: 
  {
    // ****************************************************************
    // ****************** compressible multiphase *********************
    // ****************************************************************

    numberOfComponents=0;

    rc= numberOfComponents++;    
    uc= numberOfComponents++;    
    if(  numberOfDimensions>1 )  vc= numberOfComponents++;
    if(  numberOfDimensions>2 )  wc= numberOfComponents++;
    tc= numberOfComponents++;

    pc  =  tc;
    ec= pc;

    numberOfComponents = 2*numberOfComponents +1;  // solid and gas phase

    Ru=Range( uc,uc+ numberOfDimensions-1);    // velocity components

    if( numberOfSpecies>0 )
    {
       sc= numberOfComponents;    //  sc marks the first species
       numberOfComponents+= numberOfSpecies;
       Rt=Range( rc, numberOfComponents-1);      // time dependent components
    }
    if( numberOfAdvectedScalars>0 )
    {
      if( sc<0 ) sc=numberOfComponents;            //  sc marks the first species
      numberOfSpecies+=numberOfAdvectedScalars;    // advected scalars are treated as species so that BC's are applied. 
      firstAdvectedScalar=numberOfComponents;
      numberOfComponents+= numberOfAdvectedScalars;  // marks where the advected scalars start
    }

    Rt= numberOfComponents;      // time dependent components

    if( numberOfExtraVariables>0 ) 
    {
       numberOfComponents+= numberOfExtraVariables;
    }


    int rs=0, us=1, vs=2, ws=3, ts=us+ numberOfDimensions;
    int rg=ts+1, ug=rg+1, vg=ug+1, wg=vg+1, tg=ug+ numberOfDimensions;
    int as = tg+1;  // alpha-solid

    addShowVariable( "rs",rs );
    addShowVariable( "us",us );
    if(  numberOfDimensions>1 )
      addShowVariable( "vs",vs );
    if(  numberOfDimensions>2 )
      addShowVariable( "ws",ws );
    addShowVariable( "Ts",ts );

    addShowVariable( "rg",rg );
    addShowVariable( "ug",ug );
    if(  numberOfDimensions>1 )
      addShowVariable( "vg",vg );
    if(  numberOfDimensions>2 )
      addShowVariable( "wg",wg );
    addShowVariable( "Tg",tg );
    addShowVariable( "as",as );

    if( conservativeGodunovMethod==multiFluidVersion )
    {
      int scp=sc;
      addShowVariable( "mu1",scp,true );  scp++;
      if( numberOfSpecies>2 )
      {
	addShowVariable( "mu2",scp,true );  scp++;
      }
      else
      {
	printF("compressible multipahsemultiFluidVersion:ERROR: unexpected number of species=%i\n",numberOfSpecies);
	OV_ABORT("error");
      }
      addShowVariable( "lambda",scp,true ); scp++;
	
    }

    dbase.get<RealArray >("artificialDiffusion").redim( numberOfComponents);
    dbase.get<RealArray >("artificialDiffusion")=0.;

    // kkc 070125 these two lines were moved from DomainSolver::setParametersInteractively
    //            BILL are these right?  --> ok wdh

    // *wdh* 070506 -- make 2 default from 1 for better accuracy (quarterSphere case) -- we may need a limited
    // extrapolation for hard cases
    dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours")=1;
    dbase.get<int >("orderOfExtrapolationForSecondGhostLine")=1;

    break;
 
  }

  default:
    printF("CnsParameters::setParameters:ERROR: unknown type for pde! \n");
    Overture::abort("error");
  }

  
  // ******************************************************
  // *************** assign component names ***************
  // ******************************************************
  delete  componentName;
   componentName= new aString [ numberOfComponents];

  if( pde!= compressibleMultiphase )
  {
    if(  rc>=0 )  componentName[ rc]="r";
    if(  uc>=0 )  componentName[ uc]="u";
    if(  vc>=0 )  componentName[ vc]="v";
    if(  wc>=0 )  componentName[ wc]="w";
    if(  pc>=0 )  componentName[ pc]="p";
    if(  tc>=0 )  componentName[ tc]="T";
    if(  kc>=0 )
    {
      if(  turbulenceModel==SpalartAllmaras )
	 componentName[ kc]="n";  // for nuT
      else
	 componentName[ kc]="k";
    }

    if(  epsc>=0 )  componentName[ epsc]="epsilon";

    int scp =  sc;
    if( conservativeGodunovMethod==multiComponentVersion )
    { // the first "species" in the multi-component case is the tracer variable "lambda"
       componentName[scp]="lambda";  scp++;
    }
    else if( conservativeGodunovMethod==multiFluidVersion )
    { 

        // --- Don's multi-fluid version ---
	if( equationOfState==idealGasEOS )
	{
	  // multi-fluid option with ideal EOS => 5 components (rho, rho*v1, rho*v2, rho*E, mu1)
	  if( reactionType==noReactions )
	  {
            componentName[scp]="mu1"; scp++;
	  }
	  else
	  {
	    // With any reaction rate with ideal EOS => 8 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3), 
	    componentName[scp]="lambda"; scp++;
	    componentName[scp]="mu1"   ; scp++;
	    componentName[scp]="mu2"   ; scp++;
	    componentName[scp]="mu3"   ; scp++;
	  }
	}
	else if( equationOfState==stiffenedGasEOS )
	{
	  // multi-fluid option with stiffened EOS => 6 components (rho, rho*v1, rho*v2, rho*E, mu1, mu2)
	  if( reactionType==noReactions )
	  {
            componentName[scp]="mu1"; scp++;
	    componentName[scp]="mu2"; scp++;
	  }
	  if( reactionType!=noReactions )
	  {
	    // With any reaction rate with stiffened EOS => 10 components (rho, rho*v1, rho*v2, rho*E, lambda, mu1, mu2, mu3, mu4, mu5)
	    componentName[scp]="lambda"; scp++;
	    componentName[scp]="mu1"   ; scp++;
	    componentName[scp]="mu2"   ; scp++;
	    componentName[scp]="mu3"   ; scp++;
	    componentName[scp]="mu4"   ; scp++;
	    componentName[scp]="mu5"   ; scp++;
	  }
	}
	else
	{
	  printF("multiFluidVersion:ERROR: unexpected equationOfState=%i\n",(int)equationOfState);
	  OV_ABORT("error");
	}
    }
    if(  dbase.get<bool >("advectPassiveScalar") )
    {
       componentName[scp]="s";   // use "s" as  dbase.get<real >("a") name for now, "passive";
    }
    else if( reactionName=="one equation mixture fraction" )
    {
       componentName[scp]="f";   
    }
    else if( reactionName=="two equation mixture fraction and extent of reaction" )
    {
       componentName[scp]="f";   
       componentName[scp+1]="c";   
    }
    else if( reactionName=="ignition and growth" )
    {
       componentName[scp]="s";     // lambda
    }
    else if( reactionName=="ignition and growth desensitization" )
    {
       componentName[scp]="s";     // lambda
       componentName[scp+1]="phi";     // phi
    }
    else if( numberOfSpecies>0 )
    {
      int numberOfActiveSpecies=  numberOfSpecies;
      if(  conservativeGodunovMethod==multiComponentVersion )
      {
	if( equationOfState!=jwlEOS )
	{
	  //  dbase.get<real >("mu") is  dbase.get<real >("a") non-reacting species
	  numberOfActiveSpecies-=1;
	}
	else
	{
	  //  dbase.get<real >("mu"), vi, vs, vg are non-reacting species
	  numberOfActiveSpecies-=4;
	}
      }
      else if( conservativeGodunovMethod==multiFluidVersion )
      {
	numberOfActiveSpecies=0;
      }
      else if(  equationOfState==jwlEOS )
	numberOfActiveSpecies-=2;
 
      if( numberOfActiveSpecies>0 )
      {
	assert(  dbase.get<Reactions* >("reactions")!=NULL );
	for( s=0; s<numberOfActiveSpecies; s++ )
	   componentName[scp+s]= dbase.get<Reactions* >("reactions")->getName(s);
      }
 
    }
    if( equationOfState==jwlEOS && 
       ( conservativeGodunovMethod!=multiComponentVersion  && conservativeGodunovMethod!=multiFluidVersion) )
    {
      // add vs and vg
      assert(  numberOfSpecies>2 );
      componentName[ sc+ numberOfSpecies-2]="a";   // specific volume (solid)
      componentName[ sc+ numberOfSpecies-1]="b";   // specific volume (gas)
    }
    if( equationOfState==jwlEOS && 
        (conservativeGodunovMethod==multiComponentVersion || conservativeGodunovMethod==multiFluidVersion) )
    {
      assert(  numberOfSpecies>4 );
       componentName[ sc+ numberOfSpecies-3]="vi";   // specific volume (inert)
       componentName[ sc+ numberOfSpecies-2]="vs";   // specific volume (solid)
       componentName[ sc+ numberOfSpecies-1]="vg";   // specific volume (gas)
    }
  }
  else
  {
    // component names for compressible multiphase
    int n=0;
     componentName[n]="rs"; n++; // rho-solid
     componentName[n]="us"; n++; // u-solid
    if(  numberOfDimensions>1 ){  componentName[n]="vs"; n++; }
    if(  numberOfDimensions>2 ){  componentName[n]="ws"; n++; }
     componentName[n]="ts"; n++; 

     componentName[n]="rg"; n++; // rho-gas
     componentName[n]="ug"; n++; // u-gas
    if(  numberOfDimensions>1 ){  componentName[n]="vg"; n++; }
    if(  numberOfDimensions>2 ){  componentName[n]="wg"; n++; }
     componentName[n]="tg"; n++; 

     componentName[n]="as"; n++; // alpha-solid

     printF("***** numberOfComponents=%i\n",numberOfComponents);
     
     if( conservativeGodunovMethod==multiFluidVersion )
     {
       componentName[n]="m1"; n++;
       if( numberOfSpecies>2 )
       {
 	componentName[n]="m2"; n++;
       }
       else
       {
 	printF("compressible multipahsemultiFluidVersion:ERROR: unexpected number of species=%i\n",numberOfSpecies);
 	OV_ABORT("error");
       }
       componentName[n]="lm"; n++;
	
     }
     assert( n<=numberOfComponents );


  }

  if( sec>=0 )  componentName[ sec]="Tb";

  if( numberOfExtraVariables>0 )
  {
    aString buff;
    for( int e=0; e<numberOfExtraVariables; e++ )
    {
      int n= numberOfComponents- numberOfExtraVariables+e;
       componentName[n]=sPrintF(buff,"Var%i",e);
      addShowVariable(  componentName[n],n );
    }

  }
  if( numberOfAdvectedScalars>0 )
  {
    aString buff;
    for( int e=0; e<numberOfAdvectedScalars; e++ )
    {
      assert( firstAdvectedScalar>=0 );
      int n= firstAdvectedScalar+e;
      componentName[n]=sPrintF(buff,"sc%i",e);
      addShowVariable(  componentName[n],n );
    }

  }


  // *******************************************************************
  // ************ Add auxillary show variables last ********************
  // ************ These are not used for restarts   ********************
  // *******************************************************************
  if( pde == compressibleNavierStokes )
  {

    addShowVariable( "Mach Number", numberOfComponents+1,false ); // false=turned off by default
    addShowVariable( "p", numberOfComponents+1 );
    if(  numberOfDimensions<3 )
      addShowVariable( "vorticity", numberOfComponents+1,false ); // false=turned off by default
    else
    {
      addShowVariable( "vorticityX", numberOfComponents+1,false ); // false=turned off by default
      addShowVariable( "vorticityY", numberOfComponents+1,false ); // false=turned off by default
      addShowVariable( "vorticityZ", numberOfComponents+1,false ); // false=turned off by default
    }
    addShowVariable( "divergence", numberOfComponents+1,false ); 
    addShowVariable( "speed", numberOfComponents+1,false ); 

  }
  if( pde == compressibleMultiphase )
  {
    // also plot pressure in the solid and gas
    addShowVariable( "ps", numberOfComponents+1 );
    addShowVariable( "pg", numberOfComponents+1 );
  }
  



  // specify values to be assigned to unused points (in fixupUnusedPoints)
  typedef vector<real> realVector;
   dbase.get<DataBase >("modelParameters").put<realVector>("unusedValue");
  realVector & unusedValue =  dbase.get<DataBase >("modelParameters").get<realVector>("unusedValue");
  unusedValue.resize( numberOfComponents,0.);
  
  if(  pde==CnsParameters::compressibleNavierStokes )
  {
    unusedValue[ rc]=1.;  // set unused points of rho to this value
    unusedValue[ tc]=.5;  // set unused points of T to this value
    for( int s=0; s< numberOfSpecies; s++ )
      unusedValue[ sc+s]=1.;  // Don S. wants lambda=1
  }
  else if(  pde==CnsParameters::compressibleMultiphase )
  {
    int rs=0, rg= numberOfDimensions+2;
    unusedValue[rs]=1.;  // set unused points of rho to this value
    unusedValue[rg]=1.;  // set unused points of rho to this value

    int ts= numberOfDimensions+1, tg=ts+ numberOfDimensions+2;
    unusedValue[ts]=1.;  // set unused points of T to this value
    unusedValue[tg]=1.;  // set unused points of T to this value

    int as =( numberOfDimensions+2)*2;
    unusedValue[as]=1.e-3;  // solid volume fraction
  }
  
  if(  equationOfState==CnsParameters::jwlEOS )
  {
    if( conservativeGodunovMethod!=CnsParameters::multiComponentVersion &&
        conservativeGodunovMethod!=CnsParameters::multiFluidVersion )
    {
      // 041130: specify default values for IG and jwlEOS
      if(  reactionType != CnsParameters::igDesensitization )
      {
	assert(  numberOfSpecies>2 );          
	assert(  componentName[ sc+1]=="a" );
	assert(  componentName[ sc+2]=="b" );
	// unusedValue[ tc]=.05;  // ** is this better ? ***

	unusedValue[ sc+1]=0.;   // specific volume (solid)
	unusedValue[ sc+1]=1./unusedValue[ rc];   // specific volume (gas)
      }
      else
      {
	assert(  numberOfSpecies>3 );          
	assert(  componentName[ sc+2]=="a" );
	assert(  componentName[ sc+3]=="b" );
	// unusedValue[ tc]=.05;  // ** is this better ? ***

	unusedValue[ sc+2]=0.;   // specific volume (solid)
	unusedValue[ sc+2]=1./unusedValue[ rc];   // specific volume (gas)
      }
    }
    else
    {
      if(  reactionType != CnsParameters::igDesensitization )
      {
	assert(  numberOfSpecies>4 );
	assert(  componentName[ sc+2]=="vi" );
	assert(  componentName[ sc+3]=="vs" );
	assert(  componentName[ sc+4]=="vg" );
	unusedValue[ sc]=0.0;
	unusedValue[  sc+2] = 1.0;
	unusedValue[  sc+3] = 1.0;
	unusedValue[  sc+4] = 1.0;
      }
      else
      {
	assert(  numberOfSpecies>5 );
	assert(  componentName[ sc+3]=="vi" );
	assert(  componentName[ sc+4]=="vs" );
	assert(  componentName[ sc+5]=="vg" );
	unusedValue[ sc+1]=0.0;
	unusedValue[  sc+3] = 1.0;
	unusedValue[  sc+4] = 1.0;
	unusedValue[  sc+5] = 1.0;
      }
    }
  }    



  // For methods with wider stencils we need to interpolate more exposed points for moving grids
  if( ( pde==compressibleNavierStokes &&  
        dbase.get<CnsParameters::PDEVariation >("pdeVariation")!=nonConservative) ||
      ( pde==compressibleMultiphase )  )
  {
     dbase.get<int >("stencilWidthForExposedPoints")=5;
  }
  else
  {
     dbase.get<int >("stencilWidthForExposedPoints")=3;
  }
  // ** warning: use[Fourth/Sixth]OrderArtificialDiffusion are probably not set here yet
  if( ( pde==compressibleNavierStokes &&  
        dbase.get<CnsParameters::ImplicitMethod >("implicitMethod")==notImplicit) ||
      ( pde==compressibleMultiphase )  )
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

#if 0
   // kkc 070131 WHY IS THIS HERE?? DID I PUT IT HERE??  ARGGG!
   if ( ( pde==CnsParameters::compressibleNavierStokes  || 
	  pde==CnsParameters::compressibleMultiphase)
	&&        ( cgf.u.getInterpolant()->interpolationIsImplicit()))
     dbase.get<DataBase >("modelParameters").get<int>("fixupUnusedPointsFrequency") = 1;
#endif

  return 0;
}


//\begin{>>CnsParametersInclude.tex}{\subsection{setTwilightZoneFunction}} 
int CnsParameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
// =============================================================================================
// /Description:
//
// /choice (input): CnsParameters::polynomial or CnsParameters::trigonometric
//\end{CnsParametersInclude.tex}
// =============================================================================================
{
  const CnsParameters::PDE & pde = dbase.get<CnsParameters::PDE >("pde");
  assert(  pde==compressibleNavierStokes ||  pde==compressibleMultiphase );
  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");

  int & numberOfComponents = dbase.get<int >("numberOfComponents");
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  int & rc = dbase.get<int >("rc");
  int & uc = dbase.get<int >("uc");
  int & vc = dbase.get<int >("vc");
  int & wc = dbase.get<int >("wc");
  int & pc = dbase.get<int >("pc");
  int & tc = dbase.get<int >("tc");
  int & ec = dbase.get<int >("ec");
  int & kc = dbase.get<int >("kc");
  int & sc = dbase.get<int >("sc");
  int & epsc = dbase.get<int >("epsc");
  Parameters::TurbulenceModel & turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  int & numberOfSpecies = dbase.get<int >("numberOfSpecies");

  TwilightZoneChoice choice=choice_;

  //TODO: add TZ for passive scalar=passivec
  if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
  {
    printF("CnsParameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
           "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
  }

  delete  dbase.get<OGFunction* >("exactSolution");
  if( choice==polynomial )
  {
    // ******* polynomial twilight zone function ******
     dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, numberOfDimensions, numberOfComponents,degreeTime);

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, numberOfComponents);      
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

    if(  turbulenceModel==noTurbulenceModel )
    {
      // default case:
      for( int n=0; n< numberOfComponents; n++ )
      {
	real ni =1./(n+1);
 
	spatialCoefficientsForTZ(0,0,0,n)=2.+n;      
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

          if( false ) // *wdh* 050610
	  {
	    // add cross terms
            printF("\n\n ************* add cross terms to TZ ************** \n\n");
	    

            spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
            if(  numberOfDimensions>2 )
	    {
	      spatialCoefficientsForTZ(1,0,1,n)=.1*ni;
	      spatialCoefficientsForTZ(0,1,1,n)=-.15*ni;
	    }
	    
          }
	  
	}
      }

      if( conservativeGodunovMethod == CnsParameters::multiComponentVersion ||
          conservativeGodunovMethod == CnsParameters::multiFluidVersion )
      {
	// Do stuff to multi component version
	spatialCoefficientsForTZ(R5,R5,R5, rc)=0.;
	spatialCoefficientsForTZ(0, 0, 0, rc)=1.;
	if( degreeSpace>1 )
	{
	  spatialCoefficientsForTZ(2,0,0, rc)=.2;
	  spatialCoefficientsForTZ(0,2,0, rc)=.3;
	  spatialCoefficientsForTZ(0,0,2, rc)=  numberOfDimensions==3 ? .25 : 0.;
	}
	if(  numberOfSpecies>0 )
	{
	  assert(  sc>=0 );
	  spatialCoefficientsForTZ(R5,R5,R5, sc)=0.;
	  spatialCoefficientsForTZ( 0, 0, 0, sc)=0.5; 
	}
      }
      else
      {
	// Do stuff for 1 component version (I think this is for Don's code?)
	// make rho constant in space if degreeSpace<=1
	spatialCoefficientsForTZ(R5,R5,R5, rc)=0.;
	spatialCoefficientsForTZ( 0, 0, 0, rc)=1.;   


	if( false )
	{
	  Range all;
	  spatialCoefficientsForTZ(all,all,all, uc)*=-1.;
	}
	  
	if( false )  // for testing new slip wall BC's on an annulus
	{
	  // Set (u,v) = ( -y,x) so that n.uv=0 on  dbase.get<real >("a") circle  (normal=(cos,sin) = (x,y)/r)
	  spatialCoefficientsForTZ(0,0,0, uc)=0.;   
	  spatialCoefficientsForTZ(1,0,0, uc)=0.;  
	  spatialCoefficientsForTZ(0,1,0, uc)=-1.; 

	  spatialCoefficientsForTZ(0,0,0, vc)=0.;   
	  spatialCoefficientsForTZ(1,0,0, vc)=1.;  
	  spatialCoefficientsForTZ(0,1,0, vc)=0.; 

	}
	else if( false ) //for testing new slip wall BC's
	{
	  // make sure u=0 on x=0 : 
	  spatialCoefficientsForTZ(0,0,0, uc)=0.;   
	  spatialCoefficientsForTZ(1,0,0, uc)=1.;  
	  spatialCoefficientsForTZ(0,1,0, uc)=0.; 

// 	    spatialCoefficientsForTZ(0,0,0,vc)=0.;
// 	    spatialCoefficientsForTZ(1,0,0,vc)=0.;
// 	    spatialCoefficientsForTZ(0,1,0,vc)=0.; 

// 	    spatialCoefficientsForTZ(0,0,0,pc)=1.;
// 	    spatialCoefficientsForTZ(1,0,0,pc)=1.;
// 	    spatialCoefficientsForTZ(0,1,0,pc)=0.; 
	}
	  

	if( degreeSpace>1 )
	{
	  spatialCoefficientsForTZ(2,0,0, rc)=.2;
	  spatialCoefficientsForTZ(0,2,0, rc)=.3;
	  spatialCoefficientsForTZ(0,0,2, rc)=  numberOfDimensions==3 ? .25 : 0.;

	  if( false )
	  { // set u=x(1-x) v=y(1-y) for slip wall test on  dbase.get<real >("a") square 

	    spatialCoefficientsForTZ(0,0,0, uc)=0.;   
	    spatialCoefficientsForTZ(1,0,0, uc)=1.;  
	    spatialCoefficientsForTZ(0,1,0, uc)=0.;  
	    spatialCoefficientsForTZ(2,0,0, uc)=-1.; 
	    spatialCoefficientsForTZ(1,1,0, uc)=0.; 
	    spatialCoefficientsForTZ(0,2,0, uc)=0.; 

	    spatialCoefficientsForTZ(0,0,0, vc)=0.;   
	    spatialCoefficientsForTZ(1,0,0, vc)=0.;  
	    spatialCoefficientsForTZ(0,1,0, vc)=1.;  
	    spatialCoefficientsForTZ(2,0,0, vc)=0.; 
	    spatialCoefficientsForTZ(1,1,0, vc)=0.; 
	    spatialCoefficientsForTZ(0,2,0, vc)=-1.; 


	  }
	  else if( false )
	  {  // Set (u,v) = ( -y,x)*f(x,y) so that n.uv=0 on  dbase.get<real >("a") circle  (normal=(cos,sin) = (x,y)/r)
            
	    // f(x,y)=1+.5*(x-y)
	    spatialCoefficientsForTZ(0,0,0, uc)= 0.;   
	    spatialCoefficientsForTZ(1,0,0, uc)= 0.;  
	    spatialCoefficientsForTZ(0,1,0, uc)=-1.;  
	    spatialCoefficientsForTZ(2,0,0, uc)= 0.; 
	    spatialCoefficientsForTZ(1,1,0, uc)=-.5; 
	    spatialCoefficientsForTZ(0,2,0, uc)=+.5; 

	    spatialCoefficientsForTZ(0,0,0, vc)= 0.;   
	    spatialCoefficientsForTZ(1,0,0, vc)= 1.;  
	    spatialCoefficientsForTZ(0,1,0, vc)= 0.;  
	    spatialCoefficientsForTZ(2,0,0, vc)= .5; 
	    spatialCoefficientsForTZ(1,1,0, vc)=-.5; 
	    spatialCoefficientsForTZ(0,2,0, vc)= 0.; 



	  }
	    

	}
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

    // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
 
    ((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u

  }
  else if( choice==trigonometric ) // ******* Trigonometric function chosen ******
  {
    ArraySimpleFixed<real,4,1,1,1> & omega = dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");

    RealArray fx( numberOfComponents),fy( numberOfComponents),fz( numberOfComponents),ft( numberOfComponents);
    RealArray gx( numberOfComponents),gy( numberOfComponents),gz( numberOfComponents),gt( numberOfComponents);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude( numberOfComponents), cc( numberOfComponents);
    amplitude=1.;
    cc=0.;

 

    fx= omega[0];
    fy =  numberOfDimensions>1 ?  omega[1] : 0.;
    fz =  numberOfDimensions>2 ?  omega[2] : 0.;
    ft =  omega[3];

    if( conservativeGodunovMethod == CnsParameters::multiComponentVersion ||
        conservativeGodunovMethod == CnsParameters::multiFluidVersion )
    {
      if( numberOfSpecies>0 )
      {
	cc( sc)=0.5;
	amplitude( sc)=.125;
      }
    }
    gx( vc)=.5/ omega[0];
    gy( vc)=.5/ omega[1];
    amplitude( vc)=.5;
    if(  numberOfDimensions==3 )
    {
      gx( wc)=.5/ omega[0];
      gz( wc)=.5/ omega[2];
      amplitude( wc)=-.5;
    }
    // make the temperature, pressure and density positive
    amplitude( rc)=.125; cc( rc)=1.;  gx( rc)=.5/ omega[0];
    amplitude( tc)=.25;  cc( tc)=1.;  gy( tc)=.5/ omega[1];
    //kkc    amplitude( tc)=.25;  cc( tc)=10.;  gy( tc)=.5/ omega[1];
 
    // Optionally scale amplitudes: 
    const real & trigonometricTwilightZoneScaleFactor=
      dbase.get<real>("trigonometricTwilightZoneScaleFactor");  // scale factor for Trigonometric TZ
    printF("*** CnsParameters:INFO: scaling trig TZ by the factor %9.3e\n",trigonometricTwilightZoneScaleFactor);
    amplitude *= trigonometricTwilightZoneScaleFactor;
    cc*=trigonometricTwilightZoneScaleFactor;
    
    dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
 
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
   
  }
  else if( choice==pulse ) 
  {
    // ******* Pulse function chosen ******
     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( numberOfDimensions, numberOfComponents); 

    // this pulse function is not divergence free!

  }
 

  return 0;
}



static int 
fillCompressibleDialogValues(DialogData & dialog, 
			     Parameters & parameters )
// ======================================================================================================
// /Description:
//     Fill values into the Dialog for the CNS parameters.
// ======================================================================================================
{

  int nt=0;
  aString line;
  if( parameters.dbase.get<bool >("useDimensionalParameters") )
  {
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/mu",parameters.dbase.get<real >("reynoldsNumber"))); nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/sqrt(gam*Rg)",parameters.dbase.get<real >("machNumber"))); nt++;

    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("mu")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("kThermal")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("thermalConductivity")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("Rg")));  nt++;
    // dialog.setTextLabel(nt,);  nt++;
  }
  else
  {
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("reynoldsNumber"))); nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("machNumber"))); nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/Re",parameters.dbase.get<real >("mu")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g== gam/((gam-1)*Pr*Re)",parameters.dbase.get<real >("kThermal")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("thermalConductivity")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/(gam*M*M)",parameters.dbase.get<real >("Rg")));  nt++;

  }

  return 0;
}


static int
buildCompressibleDialog(DialogData & dialog, 
                        aString & prefix,
                        Parameters & parameters )
// ===========================================================================================
// /Description: 
//    Build the dialog for the parameters for the Compressible NS equations
// ==========================================================================================
{
  dialog.closeDialog();

  dialog.setWindowTitle("Compressible NS parameters");

  const int numberOfUserVariables=parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").size();
  const int maxCommands=26+numberOfUserVariables;
  aString *cmd = new aString[maxCommands];

  const int numberOfTextStrings=33+parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").size();
  aString *textLabels = new aString [numberOfTextStrings];
  aString *textStrings = new aString [numberOfTextStrings];


//     pdeParametersMenu[n++]=  "heat release";
//     pdeParametersMenu[n++]=  "reciprocal activation energy";
//     pdeParametersMenu[n++]=  "rate constant";

//   aString pbLabels[] = {"update parameters",""};
//   addPrefix(pbLabels,prefix,cmd,maxCommands);
//   int numRows=1;
//   dialog.setPushButtons( cmd, pbLabels, numRows ); 

  dialog.setOptionMenuColumns(1);

  aString label[] = {"non-dimensional parameters","dimensional parameters",""}; //
  addPrefix(label,prefix,cmd,maxCommands);
  dialog.addOptionMenu("Use", cmd,label, parameters.dbase.get<bool >("useDimensionalParameters"));

  aString rsLabel[] = {"exact Riemann solver","Roe Riemann solver","future Riemann solver","HLL Riemann solver",""}; //
  addPrefix(rsLabel,prefix,cmd,maxCommands);
  dialog.addOptionMenu("Riemann Solver", cmd,rsLabel, parameters.dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver"));

  aString itLabel[] = {"default interpolation type",
                       "interpolate conservative variables",
                       "interpolate primitive variables",
                       "interpolate primitive and pressure",""}; //
  addPrefix(itLabel,prefix,cmd,maxCommands);
  dialog.addOptionMenu("Interpolation Type:", cmd,itLabel, (int)parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType"));


  int nt=0;

  textLabels[nt] = "Reynolds number";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("reynoldsNumber"));  nt++; 
  textLabels[nt] = "Mach number";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("machNumber"));  nt++; 

  textLabels[nt] = "mu";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("mu"));  nt++; 
  textLabels[nt] = "kThermal";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("kThermal"));  nt++; 
  textLabels[nt] = "thermal conductivity";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real>("thermalConductivity"));  nt++;

  textLabels[nt] = "Rg (gas constant)";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("Rg"));  nt++; 
  textLabels[nt] = "gamma";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("gamma"));  nt++; 
  textLabels[nt] = "Prandtl number";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("prandtlNumber"));  nt++; 
  textLabels[nt] = "gravity";  
  sPrintF(textStrings[nt], "%g,%g,%g",parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);  nt++; 

  if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::nonConservative )
  {
    textLabels[nt] = "nuRho";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("nuRho"));  nt++; 
  }

  // For Jameson artificial viscosity:
  if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeWithArtificialDissipation ||
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit || parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton )
  {
    textLabels[nt] = "av2,av4";  sPrintF(textStrings[nt], "%g,%g",parameters.dbase.get<real >("av2"),parameters.dbase.get<real >("av4"));  nt++; 
    textLabels[nt] = "aw2,aw4";  sPrintF(textStrings[nt], "%g,%g",parameters.dbase.get<real >("aw2"),parameters.dbase.get<real >("aw4"));  nt++; 
    textLabels[nt] = "scoeff";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("strickwerdaCoeff")); nt++;
  }

  textLabels[nt] = "slip wall boundary condition option";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("slipWallBoundaryConditionOption"));  nt++;


  if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
  {
    textLabels[nt] = "Godunov order of accuracy"; 
    sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod")); nt++; 

    textLabels[nt] = "artificial viscosity";  sPrintF(textStrings[nt], "%g",
                     parameters.dbase.get<real >("godunovArtificialViscosity"));  nt++; 
    if( parameters.dbase.get<int >("numberOfSpecies")==1 )
    {
      textLabels[nt] = "heat release";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("heatRelease"));  nt++; 
      textLabels[nt] = "1/(activation Energy)";  
      sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("reciprocalActivationEnergy"));  nt++; 
      textLabels[nt] = "rate constant";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("rateConstant"));  nt++; 
    }
    else if( parameters.dbase.get<int >("numberOfSpecies") > 1 )
    {
      textLabels[nt] = "heat release";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("heatRelease"));  nt++; 

      textLabels[nt] = "1/(activation Energy I)";  
      sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("reciprocalActivationEnergyI"));  nt++; 
      textLabels[nt] = "1/(activation Energy B)";  
      sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("reciprocalActivationEnergyB"));  nt++; 

      textLabels[nt] = "cross-over temperature I";  
      sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("crossOverTemperatureI"));  nt++; 
      textLabels[nt] = "cross-over temperature B";  
      sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("crossOverTemperatureB"));  nt++; 

      textLabels[nt] = "absorbed energy";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("absorbedEnergy"));  nt++; 

    }

    aString buff;
    textLabels[nt] = "artificial diffusion";  textStrings[nt]=""; 
    for( int m=0; m<parameters.dbase.get<int >("numberOfComponents"); m++ )
      textStrings[nt]+=sPrintF(buff, "%g ",parameters. dbase.get<RealArray >("artificialDiffusion")(m)); 
    nt++; 
  }
   
  textLabels[nt] = "boundary pressure offset";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("boundaryForcePressureOffset"));  nt++;

  textLabels[nt] = "density lower bound";  
  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("densityLowerBound"));  nt++;
  textLabels[nt] = "pressure lower bound";  
  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("pressureLowerBound"));  nt++;
  textLabels[nt] = "velocity limiter epsilon";  
  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("velocityLimiterEps"));  nt++;

  // add on user defined variables

  ListOfShowFileParameters &  pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

  std::list<ShowFileParameter>::iterator iter; 
  for(iter = pdeParameters.begin(); iter!= pdeParameters.end(); iter++ )
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


  assert( nt<numberOfTextStrings );

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(cmd, textLabels, textStrings);


  aString tbCommands[] = {"check for wall heating", 
			  ""};
  int tbState[4];
  tbState[0] = parameters.dbase.get<int>("checkForWallHeating");
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // dialog.openDialog();

  delete [] textLabels;
  delete [] textStrings;
  delete [] cmd;

  return 0;
}





int CnsParameters::
setPdeParameters(CompositeGrid & cg, const aString & command /* = nullString */,
                 DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//   Prompt for changes in the PDE parameters.
// =====================================================================================
{
  int returnValue=0;
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");

  // printF("\n &&&&&&&&&&&&&&& CnsParameters::setPdeParameters &&&&&&&&&&&&&\n");

  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "OBPDE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  const CnsParameters::PDE & pde = dbase.get<CnsParameters::PDE >("pde");
  GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");

  aString answer;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();


  aString *pdeParametersMenu=NULL;
  if(  pde==CnsParameters::compressibleNavierStokes )
  {
//\begin{>>setParametersInclude.tex}{\subsubsection{PDE parameters for CNS}\label{sec:cnsPdeParams}}
//\no function header:
//
// Here are the pde parameters that can be changed when solving the compressible Navier-Stokes equations.
// This menu appears when {\tt `pde parameters'} is chosen from main menu and you are solving the compressible
// Navier-Stokes equations. Normally one would specify either the {\tt Mach number} and {\tt Reynolds number}
//  or alternatively one could specify values for {\tt mu}, and ...
//\begin{description}
//  \item[Mach number] : global Mach number.
//  \item[Reynolds number] : global Reynolds number.
//  \item[mu] : viscosity (currently constant)
//  \item[Prandtl number] : 
//  \item[kThermal] : thermal conductivity (currently constant).
//  \item[Rg] : gas constant
//  \item[gamma] : ratio of specific heats.
//  \item[gravity] : a vector specifying the acceleration per unit mass due to gravity.
//  \item[algorithms] :
//    \begin{description}
//      \item[conservative with artificial dissipation]: Use conservative differencing with a Jameson style
//            artificial dissipation that mixes a second-order and fourth order dissipation.
//      \item[non-conservative]: use a centered non-conservative scheme, not recommended if you have un-resolved
//            shocks.
//      \item[conservative Godunov] : Use a conservative Godunov Scheme by Don Schwendeman
//    \end{description}
//  \end{description}
//
//\end{setParametersInclude.tex}
//\begin{>>setParametersInclude.tex}{\subsubsection{PDE parameters for ASF}\label{sec:asfPdeParams}}
//\no function header:
//
// Here are the pde parameters that can be changed when solving the all-speed flow version of the
//  compressible Navier-Stokes equations.
// This menu appears when {\tt `pde parameters'} is chosen from main menu and you are solving the 
//  {\tt allSpeedNavierStokes}. 
// Normally one would specify either the {\tt Mach number} and {\tt Reynolds number}
//  or alternatively one could specify values for {\tt mu}, and ...
//\begin{description}
//  \item[Mach number] : global Mach number.
//  \item[Reynolds number] : global Reynolds number.
//  \item[mu] : viscosity (currently constant)
//  \item[Prandtl number] : 
//  \item[kThermal] : thermal conductivity (currently constant).
//  \item[Rg] : gas constant
//  \item[gamma] : ratio of specific heats.
//  \item[gravity] : a vector specifying the acceleration per unit mass due to gravity.
//  \item[nuRho] :
//  \item[pressure level] : the constant background level of the pressure, normally determined automatically
//     from the Mach number.
//  \item[remove fast pressure waves (toggle)] : remove the $p_{tt}$ term from the pressure equation to
//       eliminate sound waves with a fast time scale.
// \end{description}
//
//\end{setParametersInclude.tex}
    const int maxMenuItems=40;
    pdeParametersMenu = new aString [maxMenuItems];
    int n=0;
    pdeParametersMenu[n++]="!pde parameters";
    pdeParametersMenu[n++]="Mach number";
    pdeParametersMenu[n++]="Reynolds number";
    pdeParametersMenu[n++]="mu";
    pdeParametersMenu[n++]="kThermal";
    pdeParametersMenu[n++]="Rg (gas constant)";
    pdeParametersMenu[n++]="gamma";
    pdeParametersMenu[n++]="gravity";
    pdeParametersMenu[n++]="nuRho";
    pdeParametersMenu[n++]=">One step reaction";
    pdeParametersMenu[n++]=  "heat release";
    pdeParametersMenu[n++]=  "reciprocal activation energy";
    pdeParametersMenu[n++]=  "rate constant";
    pdeParametersMenu[n++]="< ";
    if(  pde==CnsParameters::compressibleNavierStokes )
    {
      pdeParametersMenu[n++]=">algorithms";
      pdeParametersMenu[n++]=  "conservative with artificial dissipation";
      pdeParametersMenu[n++]=  "non-conservative";
      pdeParametersMenu[n++]=  "conservative Godunov";
      pdeParametersMenu[n++]=  "new conservative Godunov";
      pdeParametersMenu[n++]=  "newer conservative Godunov";
      pdeParametersMenu[n++]=  "characteristic interpolation";
      pdeParametersMenu[n++]= "<>artificial diffusion";
      pdeParametersMenu[n++]=   ">second order artifical diffusion";
      pdeParametersMenu[n++]=     "turn on second order artifical diffusion";
      pdeParametersMenu[n++]=     "turn off second order artifical diffusion";
      pdeParametersMenu[n++]=     "av2";
      pdeParametersMenu[n++]=     "av4";
      pdeParametersMenu[n++]=   "<>fourth order artifical diffusion";
      pdeParametersMenu[n++]=     "turn on fourth order artifical diffusion";
      pdeParametersMenu[n++]=     "turn off fourth order artifical diffusion";
      pdeParametersMenu[n++]=   "< ";
      pdeParametersMenu[n++]= "<done";
    }
    else
    {
      pdeParametersMenu[n++]= "artificial diffusion";
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
      pdeParametersMenu[n++]= "<done";
    }
    pdeParametersMenu[n]="";
    assert( n<maxMenuItems );
  }


  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=40;
    aString cmd[maxCommands];
    if(  pde==CnsParameters::compressibleNavierStokes ||
	 pde==CnsParameters::compressibleMultiphase ||
             TRUE )
    {
      updatePDEparameters();  // update parameters such as ReynoldsNumber, MachNumber, ... to be consistent.

      buildCompressibleDialog(dialog,prefix,*this);


      // fillCompressibleDialogValues(dialog,parameters,  dbase.get<bool >("useDimensionalParameters") );

    }
 
    gui.buildPopup(pdeParametersMenu);
    delete [] pdeParametersMenu;
 
    if( false && gi.graphicsIsOn() )
      dialog.openDialog(0);   // open the dialog here so we can reset the parameter values below

    updatePDEparameters();
    if(  pde==CnsParameters::compressibleNavierStokes ||
         pde==CnsParameters::compressibleMultiphase )
    {
      fillCompressibleDialogValues(dialog,*this );
    }

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
    if(  pde==CnsParameters::compressibleNavierStokes ||
         pde==CnsParameters::compressibleMultiphase )
    {
      updatePDEparameters();
      fillCompressibleDialogValues(dialog,*this );
    }


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
      if(  numberOfDimensions==2 )
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
      if(  pde==CnsParameters::compressibleNavierStokes &&
           dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
      {
        gi.inputString(answer,sPrintF(buff,"Enter the Mach number (default value=%e)", dbase.get<real >("machNumber")));
	printF("Sorry: you should not change the Mach number for this option. Default is M=1/sqrt(gamma*Rg) \n");
      }
      else
      {
	 dbase.get<bool >("useDimensionalParameters")=false;  // make sure we are using non-dimensional parameters
	gi.inputString(answer,sPrintF(buff,"Enter the Mach number (default value=%e)", dbase.get<real >("machNumber")));
	if( answer!="" )
	  sScanF(answer,"%e",& dbase.get<real >("machNumber"));

      }
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
    else if( answer=="heat release" )
    {
      printF(" Reation: D(lambda)/Dt = sigma(1-lambda) exp( (1-1/T)/eps )\n"
	     "    sigma=rate constant, eps=recriprocal activation energy \n");
      gi.inputString(answer,sPrintF(buff,"Enter the heat release (default value=%e)", dbase.get<real >("heatRelease")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("heatRelease"));
      printF(" heatRelease=%9.3e\n", dbase.get<real >("heatRelease"));
    }
    else if( answer=="reciprocal activation energy" )
    {
      printF(" Reation: D(lambda)/Dt = sigma(1-lambda) exp( (1-1/T)/eps )\n"
	     "    sigma=rate constant, eps=recriprocal activation energy \n");
      gi.inputString(answer,sPrintF(buff,"Enter the recriprocal activation energy (default value=%e)",
				      dbase.get<real >("reciprocalActivationEnergy")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("reciprocalActivationEnergy"));
      printF(" reciprocalActivationEnergy=%9.3e\n", dbase.get<real >("reciprocalActivationEnergy"));
    }
    else if( answer=="rate constant" )
    {
      printF(" Reation: D(lambda)/Dt = sigma(1-lambda) exp( (1-1/T)/eps )\n"
	     "    sigma=rate constant, eps=recriprocal activation energy \n");
      gi.inputString(answer,sPrintF(buff,"Enter the rate constant (default value=%e)", dbase.get<real >("rateConstant")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("rateConstant"));
      printF(" rateConstant=%9.3e\n", dbase.get<real >("rateConstant"));
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
       dbase.get<int >("checkForInflowAtOutFlow")=(answer=="check for inflow at outflow" ? 1 : 
			       answer=="expect inflow at outflow" ? 2 : 0);

      printF("Setting checkForInflowAtOutFlow=%i\n", dbase.get<int >("checkForInflowAtOutFlow"));
   
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
    else if( answer=="turn on second order artificial diffusion" )
    {
       dbase.get<bool >("useSecondOrderArtificialDiffusion")=true;
       dbase.get<real >("ad21")=1.; // .25;
       dbase.get<real >("ad22")=1.; // .25;
       dbase.get<real >("av2")=.25;
       dbase.get<real >("aw2")=.008333; 
       printF("turn on second order artficial diffusion with av2=%e, aw2=%e\n", dbase.get<real >("av2"), dbase.get<real >("aw2"));
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
       
       printF("turn off fourth order artficial diffusion, av4=%e, aw4=%e\n", dbase.get<real >("av4"), dbase.get<real >("aw4"));

       dbase.get<int >("extrapolateInterpolationNeighbours")=false;
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
    else if( answer=="conservative with artificial dissipation" )
    {
       dbase.get<CnsParameters::PDEVariation >("pdeVariation")=CnsParameters::conservativeWithArtificialDissipation;
    }
    else if( answer=="non-conservative" )
    {
       dbase.get<CnsParameters::PDEVariation >("pdeVariation")=CnsParameters::nonConservative;
    }
    else if( answer=="conservative Godunov" )
    {
       dbase.get<CnsParameters::PDEVariation >("pdeVariation")=CnsParameters::conservativeGodunov;
       dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::forwardEuler;
      //  dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::midPoint;
       conservativeGodunovMethod=fortranVersion;
    }
    else if( answer=="new conservative Godunov" )
    {
       dbase.get<CnsParameters::PDEVariation >("pdeVariation")=CnsParameters::conservativeGodunov;
       dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::forwardEuler;
       conservativeGodunovMethod=cppVersionI;
      printF("Use new conservative C++ Godunov\n");
	  
    }
    else if( answer=="newer conservative Godunov" )
    {
       dbase.get<CnsParameters::PDEVariation >("pdeVariation")=CnsParameters::conservativeGodunov;
       dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::forwardEuler;
       conservativeGodunovMethod=cppVersionII;
      printF("Use David's newer conservative Godunov\n");
	  
    }
    else if( answer=="characteristic interpolation" )
    {
       dbase.get<bool >("useCharacteristicInterpolation")=! dbase.get<bool >("useCharacteristicInterpolation");
      if(  dbase.get<bool >("useCharacteristicInterpolation") )
	printF("Use characteristic interpolation\n");
      else
	printF("Do NOT use characteristic interpolation\n");
    }
    else if( answer=="av2" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter av2 (default value=%e)", dbase.get<real >("av2")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("av2"));
      printF(" av2=%9.3e\n", dbase.get<real >("av2"));
    }
    else if( answer=="av4" )
    {
      gi.inputString(answer,sPrintF(buff,"Enter av4 (default value=%e)", dbase.get<real >("av4")));
      if( answer!="" )
	sScanF(answer,"%e",& dbase.get<real >("av4"));
      printF(" av4=%9.3e\n", dbase.get<real >("av4"));
    }
//     else if( answer=="update parameters" )
//     {
//       printf("Update the parameters to be consistent.\n");
//       updatePDEparameters();  // update parameters such as ReynoldsNumber, MachNumber, ... to be consistent.
//     }
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
    else if( dialog.getTextValue(answer,"thermal conductivity","%e",dbase.get<real>("thermalConductivity")) ){} // 
    else if( answer(0,6)=="av2,av4" )
    {
      sScanF(answer(7,answer.length()),"%e %e",& dbase.get<real >("av2"),& dbase.get<real >("av4"));
      printF(" av2=%9.3e, av4=%9.3e\n", dbase.get<real >("av2"), dbase.get<real >("av4"));
    }
    else if ( answer.matches("scoeff"))
      {
	sScanF(answer(6,answer.length()),"%e",& dbase.get<real >("strickwerdaCoeff"));
	printF(" strick. coeff. = %e\n",dbase.get<real >("strickwerdaCoeff"));
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
      RealArray & artificialDiffusion = dbase.get<RealArray >("artificialDiffusion");
      
      const int maxNum=30;        // assume at most this many components for now
      RealArray ad(maxNum); 
      ad=0.;
      int m;
      for( m=0; m< min(maxNum,dbase.get<int >("numberOfComponents")); m++ )
	ad(m)= artificialDiffusion(m);
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e  %e %e %e %e %e %e %e %e %e %e",
             &ad(0),&ad(1),&ad(2),&ad(3),&ad(4),&ad(5),&ad(6),&ad(7),&ad(8),&ad(9),
	     &ad(10),&ad(11),&ad(12),&ad(13),&ad(14),&ad(15),&ad(16),&ad(17),&ad(18),&ad(19),
	     &ad(20),&ad(21),&ad(22),&ad(23),&ad(24),&ad(25),&ad(26),&ad(27),&ad(28),&ad(29));

      if(  dbase.get<int >("numberOfComponents")>maxNum )
      {
	printF("setPdeParameters:WARNING:Only reading the first %i artificial diffusion parameters. Other values will be set to 1.\n"
               "                :Get Bill to fix this\n",maxNum);
      }
   
      aString text;
      for( m=0; m<dbase.get<int >("numberOfComponents"); m++ )
      {
	if( m<maxNum )
          artificialDiffusion(m)=ad(m);
        else
          artificialDiffusion(m)=1.;  // default value
	
        printF("Setting Godunov constant-coefficient artficial diffusion for component %s to %8.2e\n",
	       (const char*) dbase.get<aString* >("componentName")[m],artificialDiffusion(m));
	
	text+=sPrintF(buff, "%g ", artificialDiffusion(m));
      }
      dialog.setTextLabel("artificial diffusion",text);
    }
    else if( answer(0,6)=="gravity" )
    {
      sScanF(answer(7,answer.length()),"%e %e %e",& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0],& dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1],
	     & dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
      printF(" gravity=(%8.2e,%8.2e)\n", dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]);
    }
    else if( dialog.getTextValue(answer,"Godunov order of accuracy","%i", dbase.get<int >("orderOfAccuracyForGodunovMethod")) )
    {
      if(  dbase.get<int >("orderOfAccuracyForGodunovMethod")<1 ||   dbase.get<int >("orderOfAccuracyForGodunovMethod")>2 )
      {
	printF("***ERROR: invalid orderOfAccuracyForGodunovMethod=%i\n"
	       "        : setting equal to 2\n", dbase.get<int >("orderOfAccuracyForGodunovMethod"));
	 dbase.get<int >("orderOfAccuracyForGodunovMethod")=2;
        dialog.setTextLabel("Godunov order of accuracy",sPrintF("%i", dbase.get<int >("orderOfAccuracyForGodunovMethod")));
      }
    }
    else if( answer=="exact Riemann solver" ||
             answer=="Roe Riemann solver" || 
             answer=="future Riemann solver" ||
             answer=="HLL Riemann solver" )
    {
      int rsChoice=(int) dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver");
      if( answer=="exact Riemann solver" )
         dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver")=exactRiemannSolver;
      else if( answer=="Roe Riemann solver" )
         dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver")=roeRiemannSolver;
      else
      {
         dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver")=hLLRiemannSolver;  // this value is 3 -- 
      }
   
      dialog.getOptionMenu("Riemann Solver").setCurrentChoice(rsChoice);
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
    else if( dialog.getToggleValue(answer,"check for wall heating",dbase.get<int>("checkForWallHeating")) ){}//

    else if( dialog.getTextValue(answer,"heat release","%e", dbase.get<real >("heatRelease")) ){}//
    else if( dialog.getTextValue(answer,"1/(activation Energy)","%e", dbase.get<real >("reciprocalActivationEnergy")) ){}//
    else if( dialog.getTextValue(answer,"rate constant","%e", dbase.get<real >("rateConstant")) ){}//
    else if( dialog.getTextValue(answer,"1/(activation Energy I)","%e", dbase.get<real >("reciprocalActivationEnergyI")) ){}//
    else if( dialog.getTextValue(answer,"1/(activation Energy B)","%e", dbase.get<real >("reciprocalActivationEnergyB")) ){}//
    else if( dialog.getTextValue(answer,"cross-over temperature I","%e", dbase.get<real >("crossOverTemperatureI")) ){}//
    else if( dialog.getTextValue(answer,"cross-over temperature B","%e", dbase.get<real >("crossOverTemperatureB")) ){}//
    else if( dialog.getTextValue(answer,"absorbed energy","%e", dbase.get<real >("absorbedEnergy")) ){}//
    else if( dialog.getTextValue(answer,"nu","%e", dbase.get<real >("nu")) ){}//
    else if( dialog.getTextValue(answer,"divergence damping","%e", dbase.get<real >("cdv")) ){}//
    else if( dialog.getTextValue(answer,"boundary pressure offset","%e", dbase.get<real>("boundaryForcePressureOffset")) ){}//
    else if( dialog.getTextValue(answer,"density lower bound","%e", dbase.get<real>("densityLowerBound")) ){}//
    else if( dialog.getTextValue(answer,"pressure lower bound","%e", dbase.get<real>("pressureLowerBound")) ){}//
    else if( dialog.getTextValue(answer,"velocity limiter epsilon","%e", dbase.get<real>("velocityLimiterEps")) ){}//

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
    else if( dialog.getToggleValue(answer,"second-order artificial diffusion", dbase.get<bool >("useSecondOrderArtificialDiffusion")) ){}//
    else if( dialog.getToggleValue(answer,"fourth-order artificial diffusion", dbase.get<bool >("useFourthOrderArtificialDiffusion")) )
    {
      if(  dbase.get<int >("orderOfAccuracy")==2 &&  dbase.get<bool >("useFourthOrderArtificialDiffusion") )
	 dbase.get<int >("extrapolateInterpolationNeighbours")=true;
      else
	 dbase.get<int >("extrapolateInterpolationNeighbours")=false;
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
      assert(  numberOfDimensions==2 );

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
    else if(  dbase.get<ListOfShowFileParameters >("pdeParameters").matchAndSetValue( answer ) )
    {
      printF("*** answer=[%s] was found as a user defined parameter\n",(const char*)answer);
   
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



int CnsParameters::
displayPdeParameters(FILE *file /* = stdout */ )
// =====================================================================================
// /Description:
//   Display PDE parameters
// =====================================================================================
{
  const char *offOn[2] = { "off","on" };

  if(   dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes )
  {
    fprintf(file,
	    "PDE parameters: equation is `compressible Navier Stokes'.\n");
    if(  dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeWithArtificialDissipation )
    {
      fprintf(file," Using conservative with artificial diffusion: av2=%7.3e, aw2=%7.3e, av4=%7.3e, aw4=%7.3e\n",
	       dbase.get<real >("av2"), dbase.get<real >("aw2"), dbase.get<real >("av4"), dbase.get<real >("aw4"));
    }
    else if(  dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
    {
      fprintf(file," Using conservative Godunov method\n");
    }
    else if(  dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::nonConservative )
    {
      fprintf(file," Using nonconservative method: nuRho = %7.3e, anu=%e\n", dbase.get<real >("nuRho"), dbase.get<real >("anu"));
    }
   
    fprintf(file,
	    "  number of components is %i\n"
            "  Reynolds number=%e, Mach number=%e, \n"
            "  mu=%e \n"
            "  kThermal=%e \n"
            "  thermalConductivity=%e \n"
            "  Rg=%e (gas constant) \n"
            "  gamma=%e \n",
	     dbase.get<int >("numberOfComponents"),
             dbase.get<real >("reynoldsNumber"),
             dbase.get<real >("machNumber"),
             dbase.get<real >("mu"), dbase.get<real >("kThermal"), dbase.get<real >("thermalConductivity"), 
             dbase.get<real >("Rg"), dbase.get<real >("gamma"));

    if(  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0]!=0. ||  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1]!=0. ||  dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]!=0. )
      fprintf(file," gravity is on, acceleration due to gravity = (%8.2e,%8.2e,%8.2e) \n",
	       dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[0], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[1], dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[2]);
   

//     for( DataBase::iterator e=modelParameters.begin(); e!=modelParameters.end(); e++ )
//     {
//       string name=(*e).first;
//       printf("modelParameters: %s=",name.c_str());
//       DBase::Entry &entry = *((*e).second);
//       if( DBase::can_cast_entry<real>(entry) )
//       {
// 	real value=cast_entry<real>(entry);  
// 	printf("%9.3e\n",value);
//       }
//       else if( DBase::can_cast_entry<int>(entry) )
//       {
// 	printf("%i\n",cast_entry<int>(entry));
//       }
//       else if( DBase::can_cast_entry<string>(entry) )
//       {
//         const string & s = cast_entry<string>(entry);
// 	printf("%s\n",s.c_str());
//       }
//       else
//       {
// 	printf("? (unknown type)\n");
//       }
//     }
    


//     pdeParametersMenu[n++]="Mach number";
//     pdeParametersMenu[n++]="Reynolds number";
//     pdeParametersMenu[n++]="mu";
//     pdeParametersMenu[n++]="kThermal";
//     pdeParametersMenu[n++]="Rg (gas constant)";
//     pdeParametersMenu[n++]="gamma";
//     pdeParametersMenu[n++]="gravity";
//     pdeParametersMenu[n++]="nuRho";
//     pdeParametersMenu[n++]=">One step reaction";
//     pdeParametersMenu[n++]=  "heat release";
//     pdeParametersMenu[n++]=  "reciprocal activation energy";
//     pdeParametersMenu[n++]=  "rate constant";
//     pdeParametersMenu[n++]="< ";
//     if( pde==Parameters::allSpeedNavierStokes )
//     {
//       pdeParametersMenu[n++]="pressure level";
//       pdeParametersMenu[n++]="remove fast pressure waves (toggle)";
//     }
	
//     if( pde==Parameters::compressibleNavierStokes )
//     {
//       pdeParametersMenu[n++]=">algorithms";
//       pdeParametersMenu[n++]=  "conservative with artificial dissipation";
//       pdeParametersMenu[n++]=  "non-conservative";
//       pdeParametersMenu[n++]=  "conservative Godunov";
//       pdeParametersMenu[n++]=  "new conservative Godunov";
//       pdeParametersMenu[n++]=  "newer conservative Godunov";
//       pdeParametersMenu[n++]=  "characteristic interpolation";
//       pdeParametersMenu[n++]= "<>artificial diffusion";
//       pdeParametersMenu[n++]=   ">second order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn on second order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn off second order artifical diffusion";
//       pdeParametersMenu[n++]=     "av2";
//       pdeParametersMenu[n++]=     "av4";
//       pdeParametersMenu[n++]=   "<>fourth order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn on fourth order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn off fourth order artifical diffusion";
//       pdeParametersMenu[n++]=   "< ";
//       pdeParametersMenu[n++]= "<done";
//     }
//     else
//     {
//       pdeParametersMenu[n++]= "artificial diffusion";
//       pdeParametersMenu[n++]=   ">second order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn on second order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn off second order artifical diffusion";
//       pdeParametersMenu[n++]=     "ad21 : coefficient of linear term";
//       pdeParametersMenu[n++]=     "ad22 : coefficient of non-linear term";
//       pdeParametersMenu[n++]=   "<>fourth order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn on fourth order artifical diffusion";
//       pdeParametersMenu[n++]=     "turn off fourth order artifical diffusion";
//       pdeParametersMenu[n++]=     "ad41 : coefficient of linear term";
//       pdeParametersMenu[n++]=     "ad42 : coefficient of non-linear term";
//       pdeParametersMenu[n++]= "<done";
//     }


  }
  else if(   dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleMultiphase )
  {
    fprintf(file,
	    "PDE parameters: equation is `compressible multiphase'.\n");

    fprintf(file," ... finish this Bill! ....\n");
  }
         
  // The  dbase.get<DataBase >("modelParameters") will be displayed here:
  Parameters::displayPdeParameters(file);

  return 0;
}


//\begin{>>OverBlownInclude.tex}{\subsection{updatePDEparameters}} 
int CnsParameters::
updatePDEparameters()
//===================================================================================
// /Description:
//    Update the PDE the dimensional PDE parameters such as mu if the non-dimensional
// parameters (Reynolds number, mach number etc) were specified.
//
// /Author: WDH
//\end{OverBlownInclude.tex}  
//===================================================================================
{
  if(  dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes )
  {

    const real infinity = 1./REAL_MIN;

    if(  dbase.get<real >("reynoldsNumber")!=(real)Parameters::defaultValue && ! dbase.get<bool >("useDimensionalParameters") )
    {
      if(  dbase.get<real >("reynoldsNumber") != 1./max( dbase.get<real >("mu"),REAL_MIN) )
      {
        if(  dbase.get<real >("reynoldsNumber") > .1*infinity )
           dbase.get<real >("mu")=0.;
	else
	{
  	  printF("---assigning mu to match reynoldsNumber and machNumber----\n");
  	   dbase.get<real >("mu")=1./ dbase.get<real >("reynoldsNumber");
	}
      }
    }
    else
    {
      printF("---assigning reynoldsNumber to match mu----\n");
       dbase.get<real >("reynoldsNumber")=1./max( dbase.get<real >("mu"),REAL_MIN);
    }
    if(  dbase.get<real >("machNumber")!=(real)Parameters::defaultValue && ! dbase.get<bool >("useDimensionalParameters") )
    {
      if(  dbase.get<real >("Rg") != 1./( dbase.get<real >("gamma")*SQR( dbase.get<real >("machNumber"))) )
      {
	printF("---assigning Rg to match machNumber----\n");
	 dbase.get<real >("Rg")=1./( dbase.get<real >("gamma")*SQR( dbase.get<real >("machNumber")));
      }
    }
    else
    {
      printF("---assigning machNumber to match Rg----\n");
       dbase.get<real >("machNumber")=1./SQRT( dbase.get<real >("gamma")* dbase.get<real >("Rg"));
    }

    if(  dbase.get<real >("kThermal")==(real)Parameters::defaultValue || ! dbase.get<bool >("useDimensionalParameters") )
    {
      printF("---assigning kThermal to match Reynolds, gamma, prandtlNumber----\n");
      if(  dbase.get<real >("mu")>0 )
	 dbase.get<real >("kThermal")= dbase.get<real >("gamma")/( ( dbase.get<real >("gamma")-1)* dbase.get<real >("prandtlNumber")* dbase.get<real >("reynoldsNumber") );
      else
	 dbase.get<real >("kThermal")=0.;
    }
    else
    {
      if(  dbase.get<real >("kThermal")==(real)Parameters::defaultValue &&  dbase.get<bool >("useDimensionalParameters") )
      {
	printF("---assigning kThermal=mu/Prandtl\n");
         dbase.get<real >("kThermal")= dbase.get<real >("mu")/ dbase.get<real >("prandtlNumber");
      }
   
    }
 
  }
  return 0;
}


//\begin{>>CnsParametersInclude.tex}{\subsection{updateShowFile}} 
int CnsParameters::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{CnsParametersInclude.tex}  
// =================================================================================================
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  int & rc = dbase.get<int >("rc");
  int & uc = dbase.get<int >("uc");
  int & vc = dbase.get<int >("vc");
  int & wc = dbase.get<int >("wc");
  int & pc = dbase.get<int >("pc");
  int & tc = dbase.get<int >("tc");
  int & ec = dbase.get<int >("ec");
  int & kc = dbase.get<int >("kc");
  int & sc = dbase.get<int >("sc");
  int & sec = dbase.get<int >("sec");
  int & epsc = dbase.get<int >("epsc");
  const ReactionTypeEnum & reactionType = dbase.get<CnsParameters::ReactionTypeEnum >("reactionType");

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  if(  dbase.get<CnsParameters::PDE >("pde")==compressibleNavierStokes )
  {
    // save parameters

    // new way to save parameters

    showFileParams.push_back(ShowFileParameter("pde","compressibleNavierStokes"));

    showFileParams.push_back(ShowFileParameter("reynoldsNumber", dbase.get<real >("reynoldsNumber")));
    showFileParams.push_back(ShowFileParameter("machNumber", dbase.get<real >("machNumber")));
    showFileParams.push_back(ShowFileParameter("mu", dbase.get<real >("mu")));
    showFileParams.push_back(ShowFileParameter("kThermal", dbase.get<real >("kThermal")));
    showFileParams.push_back(ShowFileParameter("thermalConductivity", dbase.get<real >("thermalConductivity")));
    showFileParams.push_back(ShowFileParameter("gamma", dbase.get<real >("gamma")));
    showFileParams.push_back(ShowFileParameter("Rg", dbase.get<real >("Rg")));

// ***** these next are wrong if the user only saves some variables to the show file **** fix this ****
//     showFileParams.push_back(ShowFileParameter("densityComponent", rc));
//     showFileParams.push_back(ShowFileParameter("temperatureComponent", tc));
//     showFileParams.push_back(ShowFileParameter("pressureComponent", pc));
//     showFileParams.push_back(ShowFileParameter("uComponent", uc));
//     showFileParams.push_back(ShowFileParameter("vComponent", vc));
//     showFileParams.push_back(ShowFileParameter("wComponent", wc));



    CnsParameters::EquationOfStateEnum & equationOfState = dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState");
    
    aString eosName;
    eosName=( equationOfState==CnsParameters::idealGasEOS ? "ideal gas" :
	      equationOfState==CnsParameters::jwlEOS ? "JWL" : 
              equationOfState==CnsParameters::mieGruneisenEOS ? "Mie-Gruneisen" : 
              equationOfState==CnsParameters::userDefinedEOS ? "user defined" : 
              equationOfState==CnsParameters::stiffenedGasEOS ? "stiffened gas" : 
              equationOfState==CnsParameters::taitEOS ? "Tait" : 
	     "unknown");
 
    showFileParams.push_back(ShowFileParameter("equationOfState",eosName));


    showFileParams.push_back(ShowFileParameter("numberOfSpecies", dbase.get<int >("numberOfSpecies")));
 
    if(  dbase.get<int >("numberOfSpecies")>=0 )
    {
      showFileParams.push_back(ShowFileParameter("speciesComponent", sc));

      aString reactionName = ( reactionType==noReactions ? "noReactions" :
                               reactionType==oneStep ? "onestep" :
                               reactionType==branching ? "branching" :
                               reactionType==ignitionAndGrowth ? "ignitionAndGrowth" :
                               reactionType==oneEquationMixtureFraction ? "oneEquationMixtureFraction" :
                               reactionType==twoEquationMixtureFractionAndExtentOfReaction ? 
                                            "twoEquationMixtureFractionAndExtentOfReaction" :
			       reactionType==oneStepPress ? "oneStepPressureLaw" :
			       reactionType==igDesensitization ? "igDesensitization" :
                               reactionType==chemkinReaction ? "chemkinReaction" : "unknown reactionType");

      showFileParams.push_back(ShowFileParameter("reactionType", reactionName));

      // for oneStep:
      showFileParams.push_back(ShowFileParameter("heatRelease", dbase.get<real >("heatRelease")));
      showFileParams.push_back(ShowFileParameter("reciprocalActivationEnergy", dbase.get<real >("reciprocalActivationEnergy")));
      showFileParams.push_back(ShowFileParameter("rateConstant", dbase.get<real >("rateConstant")));

      // for chain branching
      showFileParams.push_back(ShowFileParameter("reciprocalActivationEnergyI", dbase.get<real >("reciprocalActivationEnergyI")));
      showFileParams.push_back(ShowFileParameter("reciprocalActivationEnergyB", dbase.get<real >("reciprocalActivationEnergyB")));
      showFileParams.push_back(ShowFileParameter("crossOverTemperatureI", dbase.get<real >("crossOverTemperatureI")));
      showFileParams.push_back(ShowFileParameter("crossOverTemperatureB", dbase.get<real >("crossOverTemperatureB")));
      showFileParams.push_back(ShowFileParameter("absorbedEnergy", dbase.get<real >("absorbedEnergy")));

    }
 


  }
  else if(  dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleMultiphase )
  {
    // save parameters

    showFileParams.push_back(ShowFileParameter("pde","compressibleMultiphase"));
  }
  else
  {
    printF("CnsParameters:saveParametersToShowFile:ERROR: unknown pde ! \n");
    Overture::abort("error");
  }

  // here are the new names for the velocity components:
  showFileParams.push_back(ShowFileParameter("v1Component", dbase.get<int>("uc")));
  showFileParams.push_back(ShowFileParameter("v2Component", dbase.get<int>("vc")));
  showFileParams.push_back(ShowFileParameter("v3Component", dbase.get<int>("wc")));


  // Now save parameters common to all solvers:
  Parameters::saveParametersToShowFile();
  

//   // Add on user defined parameters
//   std::list<ShowFileParameter>::iterator iter; 
//   for(iter =  dbase.get<ListOfShowFileParameters >("pdeParameters").begin(); iter!= dbase.get<ListOfShowFileParameters >("pdeParameters").end(); iter++ )
//   {
//     showFileParams.push_back(*iter);
//   }

//    dbase.get<Ogshow* >("show")->saveGeneralParameters(showFileParams);
 

  return 0;
}


//\begin{>>ParametersInclude.tex}{\subsubsection{getDerivedFunction}}
int CnsParameters::
getDerivedFunction( const aString & name, const realCompositeGridFunction & u,
                    realCompositeGridFunction & v, const int component, const real t, 
                    Parameters & parameters)
//==================================================================================
// /Description:
//     Assign the values of a derived quantity
//
// /name (input): the name of the grid function on the database.
// /u (input) : evaluate the derived function using this grid function
// /v (input) : fill in a component of this grid function
// /component : component index to fill, i.e. fill v(all,all,all,component)
//\end{ParametersInclude.tex} 
//=================================================================================
{
  CompositeGrid & cg = *v.getCompositeGrid();

  int ok = 0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    ok = getDerivedFunction(name,u[grid],v[grid],grid,component,t,parameters);
    if ( ok!=0 ) break;
  }
  return ok;
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{getDerivedFunction}}
int CnsParameters::
getDerivedFunction( const aString & name, const realMappedGridFunction & uIn, 
                    realMappedGridFunction & vIn, const int grid,
                    const int component, const real t, Parameters & parameters)
//==================================================================================
// /Description:
//     Assign the values of a derived quantity
//
// /name (input): the name of the grid function on the database.
// /u (input) : evaluate the derived function using this grid function
// /v (input) : fill in a component of this grid function
// /component : component index to fill, i.e. fill v(all,all,all,component)
//\end{CompositeGridFunctionInclude.tex} 
//=================================================================================
{
  MappedGrid & mg = *vIn.getMappedGrid();

  Index all;
  // Index I1,I2,I3;

  const PDE pde = parameters.dbase.get<PDE>("pde");
  const int rc=parameters.dbase.get<int >("rc");
  const int tc=parameters.dbase.get<int >("tc");
  const int sc=parameters.dbase.get<int >("sc");
  const real Rg = parameters.dbase.get<real >("Rg");
  
  const EquationOfStateEnum equationOfState = parameters.dbase.get<EquationOfStateEnum >("equationOfState");
  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  

  #ifdef USE_PPP
   realSerialArray u; getLocalArrayWithGhostBoundaries(uIn,u);
   realSerialArray v; getLocalArrayWithGhostBoundaries(vIn,v);
  #else
    const realSerialArray & u = uIn;
    realSerialArray & v = vIn;
  #endif

  if( name=="pressure" || 
      name=="ps" || name=="pg" ) // for compressible multiphase
  {
    if( pde==compressibleNavierStokes )
    {
      // getIndex(mg.dimension(),I1,I2,I3);
      if( conservativeGodunovMethod==multiComponentVersion ||
          conservativeGodunovMethod==multiFluidVersion )
      {
	// Here we compute the pressure for the multi-component Godunov method
        // Note we assume p=rho*R*T ... so temperature means nothing so we can make sense of pressure
	v(all,all,all,component)=Rg*u(all,all,all,rc)*u(all,all,all,tc);
      }
      else
      {
        if( equationOfState==idealGasEOS ||
            equationOfState==jwlEOS  ||
            equationOfState==userDefinedEOS )
	{
	  // assume ideal gas law for now
	  v(all,all,all,component)=Rg*u(all,all,all, rc)*u(all,all,all,tc);
	}
	else if( equationOfState==CnsParameters::mieGruneisenEOS )
	{
          real alphaMG,betaMG,v0MG,kappaMG;
          ListOfShowFileParameters pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");
	  
	  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",alphaMG);
	  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",betaMG);
	  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("V0MG",v0MG);
	  dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("kappaMG",kappaMG);
	  
          // p = rho*kappa* dbase.get<real >("Rg")*T + F(rho)

          realSerialArray vv; // volume fraction
          vv = 1./(v0MG*u(all,all,all, rc)); 
	  
          v(all,all,all,component)=(Rg*kappaMG)*u(all,all,all,rc)*u(all,all,all,tc)
	    + (vv-1.)*( alphaMG + betaMG*(vv-1.) );
	}
        else if( equationOfState==CnsParameters::stiffenedGasEOS )
	{
          //  p = (gammaStiff-1)* rho * e -  gammaStiff*pStiff
          // ! *ve* rho*e = (rho*T-gammaStiff*pStiff)/(gammaStiff-1)
          // real gammaStiff=1.4,  pStiff=0.;
          // dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",gammaStiff);
	  // dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",pStiff);

          // p = rho*T (T is defined as p/rho)
          v(all,all,all,component)=Rg*u(all,all,all, rc)*u(all,all,all,tc);
	}
	else if( equationOfState==CnsParameters::taitEOS )
	{
          Overture::abort("finish me");
	}
        else
	{
	  printf("getDerivedFunction:ERROR: unknown equation of state =%i\n",
            (int)equationOfState);
	  Overture::abort("error");
	}
	
      }
    }
    else if( pde==compressibleMultiphase )
    {

      // Here we compute the pressure for the multi-component Godunov method
      // Note we assume p=rho*R*T ... so temperature means nothing so we can make sense of pressure
      int rs=0, ts=3, rg=4, tg=7;
      
      int pressureComponent=component;
      if( name=="pressure" || name=="ps" )
      {
	v(all,all,all,pressureComponent)=u(all,all,all,rs)*u(all,all,all,ts);
        pressureComponent++;
      }
      if( name=="pressure" || name=="pg" )
      {
	v(all,all,all,pressureComponent)=u(all,all,all,rg)*u(all,all,all,tg);
      }
      
    }
    else
    {
      printf("getDerivedFunction:ERROR: I don't know how to compute the pressure for this pde\n");
      return 1;
    }
  }
  else if( name=="temperature-from-pressure" )
  {
    // **** NOTE: this case assumes that the pressure is defined -- this is used by the 
    //   interpolation function to convert the pressure with primitive variables back to the temperature
    if( parameters.dbase.get<PDE>("pde")==compressibleNavierStokes )
    {
      // getIndex(mg.dimension(),I1,I2,I3);
       

      if( conservativeGodunovMethod==multiComponentVersion ||
          conservativeGodunovMethod==multiFluidVersion )
      {
	// Here we compute the pressure for the multi-component Godunov method
        // Note we assume p=rho*R*T ... so temperature means nothing so we can make sense of pressure
        const int pc= dbase.get<int >("tc");
	v(all,all,all,component)=u(all,all,all,pc)/(Rg*u(all,all,all,rc));
      }
      else
      {
        if( equationOfState==idealGasEOS ||
            equationOfState==jwlEOS ||
            equationOfState==userDefinedEOS ||
            equationOfState==CnsParameters::stiffenedGasEOS )
	{
	  // assume ideal gas law for now  : T = p/(rho* dbase.get<real >("Rg"))
          v(all,all,all,component)=u(all,all,all,tc)/(Rg*u(all,all,all,rc));
	}
	else if( equationOfState==CnsParameters::mieGruneisenEOS )
	{
          real alphaMG,betaMG,v0MG,kappaMG;
          ListOfShowFileParameters pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");
	  
	   pdeParameters.getParameter("alphaMG",alphaMG);
	   pdeParameters.getParameter("betaMG",betaMG);
	   pdeParameters.getParameter("V0MG",v0MG);
	   pdeParameters.getParameter("kappaMG",kappaMG);
	  
          // p = rho*kappa* dbase.get<real >("Rg")*T + F(rho)

          realSerialArray vv;
          vv = 1./(v0MG*u(all,all,all, rc)); 
	  
          const int pc= tc;
          v(all,all,all,component)=( u(all,all,all,pc) - (vv-1.)*( alphaMG + betaMG*(vv-1.) ) )/
                    ((Rg*kappaMG)*u(all,all,all, rc));
	}
        else if( equationOfState==CnsParameters::taitEOS )
	{
          Overture::abort("ERROR: finish me!");
	}
        else
	{
	  printf("getDerivedFunction:ERROR: unknown equation of state =%i\n",
            (int)equationOfState);
	  Overture::abort("error");
	}


      }
    }
    else
    {
      printf("getDerivedFunction:ERROR: I don't know how to compute the temperature-from-pressure for this pde\n");
      return 1;
    }
  }
  else
  {
    printf("getDerivedFunction:ERROR: unknown derived function! name=%s\n",(const char*)name);
    return 1;
  }
  return 0;
}

int
CnsParameters::
buildReactions()
{
  Parameters::buildReactions();
  if(  dbase.get<aString >("reactionName")=="one step" )
  {
    dbase.get<CnsParameters::PDEVariation >("pdeVariation")=conservativeGodunov;
  }
  else if(  dbase.get<aString >("reactionName")=="branching" )
  {
    dbase.get<CnsParameters::PDEVariation >("pdeVariation")=conservativeGodunov;
  }
  if(  dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")==jwlEOS )
  {
    // alpha and beta
    dbase.get<int >("numberOfSpecies")+=2;
  }
 
}

int
CnsParameters::
updateToMatchGrid(CompositeGrid & cg, IntegerArray & sharedBoundaryCondition )
{
  Parameters::updateToMatchGrid(cg, sharedBoundaryCondition);

  // !!! kkc fudge to make implicit cns code work with amr
  if (  dbase.get<PDE>("pde")==compressibleNavierStokes &&  dbase.get<Parameters::ImplicitMethod >("implicitMethod")!=notImplicit )
    dbase.get<IntegerArray >("gridIsImplicit") = 1;

  Range all;
  if( (  dbase.get<PDE>("pde")==compressibleNavierStokes &&  
	 dbase.get<CnsParameters::PDEVariation >("pdeVariation")==conservativeGodunov &&  
	 dbase.get<int >("numberOfSpecies")>0 ) ||
      dbase.get<PDE>("pde")==compressibleMultiphase )
  {
    if(  dbase.get<realCompositeGridFunction* >("truncationError")==NULL )
      {
	dbase.get<realCompositeGridFunction* >("truncationError") = new realCompositeGridFunction(cg,all,all,all);
      }
    else
      dbase.get<realCompositeGridFunction* >("truncationError")->updateToMatchGrid(cg,all,all,all);
    
    (* dbase.get<realCompositeGridFunction* >("truncationError"))=0.;
    
  }

}

bool
CnsParameters::
useConservativeVariables(int grid /* =-1 */ ) const
{
  Parameters *gridPDE= (Parameters*)this;
  if( grid!=-1 &&  dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
    {
      // look up the  dbase.get<Parameters::PDE >("pde") used on this grid
      ListOfEquationDomains & equationDomainList = * dbase.get<ListOfEquationDomains* >("pEquationDomainList");
      const int numberOfEquationDomains=equationDomainList.size();
      const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
      assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
      EquationDomain & equationDomain = equationDomainList[equationDomainNumber];
      
      gridPDE = equationDomain.getPDE();
    }
  
  CnsParameters *gridCNSParams = dynamic_cast<CnsParameters*>(gridPDE);
  return ( (gridCNSParams && gridCNSParams->dbase.get<PDE>("pde")==compressibleNavierStokes && 
	    dbase.get<CnsParameters::PDEVariation >("pdeVariation")!=nonConservative)
	   || (gridCNSParams && gridCNSParams->dbase.get<PDE>("pde")==compressibleMultiphase));
}


int 
CnsParameters::
numberOfGhostPointsNeeded() const  // number of ghost points needed by this method.
{
  int numGhost = Parameters::numberOfGhostPointsNeeded();

  if (  (  dbase.get<PDE >("pde")==compressibleNavierStokes && 
	   (
	    ( dbase.get<PDEVariation >("pdeVariation")==conservativeWithArtificialDissipation &&  dbase.get<real >("av4")!=0.) || 
	    dbase.get<PDEVariation >("pdeVariation")==conservativeGodunov ||
	    dbase.get<PDEVariation >("pdeVariation")==nonConservative)  // for 4th-order dissipation
	   )
	||   dbase.get<PDE >("pde")==compressibleMultiphase )
    {
      numGhost=max(numGhost,2);
    }

  return numGhost;
}


int 
CnsParameters::
get(const GenericDataBase & dir, const aString & name)
{
  Parameters::get(dir,name);
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Parameters");
  int temp;
  subDir.get(temp,"pde");  dbase.get<PDE >("pde")=(PDE)temp; 
  subDir.get(temp,"pdeVariation");    dbase.get<PDEVariation >("pdeVariation")=(PDEVariation)temp;
  subDir.get(temp,"riemannSolver");  dbase.get<RiemannSolverEnum >("riemannSolver")=(RiemannSolverEnum)temp; 
  subDir.get(temp,"conservativeGodunovMethod");  dbase.get<GodunovVariation >("conservativeGodunovMethod")=(GodunovVariation)temp;

  return 0;
}
 
int 
CnsParameters::
put(GenericDataBase & dir, const aString & name)
{
  Parameters::put(dir,name);
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Parameters");

  subDir.put((int) dbase.get<CnsParameters::PDE >("pde"),"pde");
  subDir.put((int) dbase.get<CnsParameters::PDEVariation >("pdeVariation"),"pdeVariation"); 
  subDir.put((int) dbase.get<CnsParameters::RiemannSolverEnum >("riemannSolver"),"riemannSolver"); 
  subDir.put((int) dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod"),"conservativeGodunovMethod");

  return 0;
}   

int CnsParameters::
assignParameterValues(const aString & label, RealArray & values,
                      const int & numRead, aString *c, real val[],
                      char *extraName1 /* = 0 */, const int & extraValue1Location /* = 0 */, 
                      char *extraName2 /* = 0 */, const int & extraValue2Location /* = 0 */, 
                      char *extraName3 /* = 0 */, const int & extraValue3Location /* = 0 */ )
// ==============================================================================================
//  /Description:
//     Assign parameter values with names in the array $c$ and values in the array {\tt val}.
// The entries in {\tt c} correspond to one of componentName[n] or to one of the extra names.
// /label (input) : print this label when showing the responses. For example label="initial conditions".
// /numRead (input) :
// /c (input) : names of components
// /val (input) : value to assign to the component.
// /values (output) : return values in this array. For example the assignment `u=1.' will result in
//   values(uc)=1.
// /Return value: The number of variables assigned.
//\end{ParametersInclude.tex}  
// ==============================================================================================
{
  Parameters::assignParameterValues(label,values,
				    numRead,c,val,
				    extraName1, extraValue1Location,
				    extraName2, extraValue2Location,
				    extraName3, extraValue3Location);

  const int numberOfExtraNames=3;
  char *extraName[numberOfExtraNames] = {extraName1,extraName2,extraName3};
  const int extraValueLocation[numberOfExtraNames]={extraValue1Location,extraValue2Location,extraValue3Location};
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");

  const GodunovVariation & conservativeGodunovMethod = 
                           dbase.get<CnsParameters::GodunovVariation >("conservativeGodunovMethod");
  aString name;
  int i,n;
  real energy=-1.;
  int ie=-1;
  for( i=0; i<numRead; i++ )
  {
    bool found=false;
    name = c[i];	
    for( n=0; n< dbase.get<int >("numberOfComponents"); n++ )
    {
      if( name== dbase.get<aString* >("componentName")[n] )
      {
	values(n)=val[i];
	printF("assigning %s: %s=%e \n",(const char *)label,(const char *)c[i],val[i]);
	found=true;
	break;
      } 
    }
    if( !found )
    {
      for( int n=0; n<numberOfExtraNames; n++ )
      {
	if( extraName[n]!=0 && name==*extraName[n] )
	{
	  values(extraValueLocation[n])=val[i];
	  found=true;
          break;
	}
      }
      if( !found )
      {
	if( name=="e" )
	{
	  energy = val[i];
	  ie=i;
	  printF("assigning %s: %s=%e \n",(const char *)label,(const char *)c[i],val[i]);
	  found=true;
	}
      }
    }
    if( !found )
    {
      printF("ERROR: unknown parameter being assigned: name=%s, value=%e \n",(const char *)c[i],val[i]);
      // printF("     : input string=[%s]\n",(const char*)answer);
    }
  }

  if( ie>=0 )
  {
    // determine the temperature from the energy
    const real & rho = values(dbase.get<int >("rc"));

    if( values( dbase.get<int >("uc"))==(real)defaultValue )
      values( dbase.get<int >("uc"))=0.;
    if(  numberOfDimensions>1 && values( dbase.get<int >("vc"))==(real)defaultValue )
      values( dbase.get<int >("vc"))=0.;
    if(  numberOfDimensions>2 && values( dbase.get<int >("wc"))==(real)defaultValue )
      values( dbase.get<int >("wc"))=0.;
    
      
    real uSq = SQR(values( dbase.get<int >("uc")));
    if(  numberOfDimensions>1 )
      uSq+=SQR(values( dbase.get<int >("vc")));
    if(  numberOfDimensions>2 )
      uSq+=SQR(values( dbase.get<int >("wc")));

    if( dbase.get<CnsParameters::EquationOfStateEnum >("equationOfState")==idealGasEOS &&  
        conservativeGodunovMethod!=multiComponentVersion && conservativeGodunovMethod!=multiFluidVersion)
    {

      if(  dbase.get<real >("Rg")<=0. ||  dbase.get<real >("Rg")>1.e10 )
      {
	printF("assignParameterValues:ERROR: Rg=%e seems to be invalid. I am unable to set the temperature\n"
	       "from the other variables\n", dbase.get<real >("Rg"));
      }
      else
      {
	values( dbase.get<int >("tc"))=(( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))*(energy/rho-.5*uSq);
	printF("assignParameterValues:assigning the temperature=%7.3e from e=%7.3e, rho=%7.3e, Rg=%7.3e, |u|^2=%7.3e"
	       " gamma=%7.3e\n",values( dbase.get<int >("tc")),energy,rho, dbase.get<real >("Rg"),uSq, dbase.get<real >("gamma"));
      }
    }
    else
    {
      printF("ERROR: assignParameterValues: non-Ideal EOS -- you specify T instead for E for this EOS\n");
      Overture::abort("error");
    }
    
  }

  return 0;
}

// *wdh* 100808 -- this function was merged back into the base class version
//* int CnsParameters::
//* setTwilightZoneParameters(const aString & command /* = nullString */,
//* 			  DialogData *interface /* =NULL */ )
//* // =====================================================================================
//* // /Description:
//* //     Assign parameters for twilight zone.
//* // =====================================================================================
//* {
//*   // kkc 070125 most of this is copied from the Parameters version.
//*   int returnValue=0;
//*   int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
//*   
//*   assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
//*   GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");
//*   aString answer2;
//*   char buff[80];
//*   
//*   aString prefix = "OBTZ:"; // prefix for commands to make them unique.
//* 
//*   const bool executeCommand = command!=nullString;
//*   if( false &&   // don't check prefix for now
//*       executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
//*     return 1;
//* 
//*   KnownSolutionsEnum & knownSolution = dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
//* 
//*   GUIState gui;
//*   gui.setWindowTitle("Twilight Zone Options");
//*   gui.setExitCommand("exit", "continue");
//*   DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;
//* 
//* 
//*   if( interface==NULL || command=="build dialog" )
//*   {
//*     const int maxCommands=40;
//*     aString cmd[maxCommands];
//*     aString pushButtonCommands[maxCommands];
//* 
//*     int n,numRows;
//*     n=0;
//*     pushButtonCommands[n]="assign polynomial coefficients"; n++;
//*     pushButtonCommands[n]=""; n++;
//*     assert( n<maxCommands );
//* 
//*     numRows=n;
//*     addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
//*     dialog.setPushButtons( cmd, pushButtonCommands, numRows );
//* 
//* 
//*     dialog.setOptionMenuColumns(1);
//* 
//*     aString label[] = {"polynomial","trigonometric","pulse",""}; //
//*     addPrefix(label,prefix,cmd,maxCommands);
//*     dialog.addOptionMenu("type", cmd,label, (int) dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"));
//* 
//*     aString label2[] = {"no known solution",
//* 			// "supersonic flow in an expanding channel", -- this is now in userDefinedKnownSolution
//* 			"axisymmetric rigid body rotation",
//* 			"user defined known solution",""}; //
//*     addPrefix(label2,prefix,cmd,maxCommands);
//*     dialog.addOptionMenu("known solution", cmd,label2, (int) knownSolution);
//* 
//*     aString label3[] = {"maximum norm",
//* 			"l1 norm",
//* 			"l2 norm",""}; //
//*     addPrefix(label3,prefix,cmd,maxCommands);
//*     dialog.addOptionMenu("Error Norm", cmd,label3, ( dbase.get<int >("errorNorm")>2 ? 0 :  dbase.get<int >("errorNorm")));
//* 
//*     aString tbLabel[] = {"twilight zone flow",
//*                          "use 2D function in 3D",
//*                          "compare 3D run to 2D",
//*                          "assign TZ initial conditions",
//*                          ""};
//*     int tbState[5];
//*     tbState[0] =  dbase.get<bool >("twilightZoneFlow");
//*     tbState[1] =  dbase.get<int >("dimensionOfTZFunction")==2;
//*     tbState[2] =  dbase.get<int >("compare3Dto2D"); 
//*     tbState[3] =  dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow");
//*     addPrefix(tbLabel,prefix,cmd,maxCommands);
//* 
//*     int numColumns=1;
//*     dialog.setToggleButtons(cmd, tbLabel, tbState, numColumns); 
//* 
//*     const int numberOfTextStrings=4;
//*     aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];
//* 
//*     int nt=0;
//*     textLabels[nt] = "degree in space"; 
//*     sPrintF(textStrings[nt], "%i",  dbase.get<int >("tzDegreeSpace")); nt++; 
//* 
//*     textLabels[nt] = "degree in time"; 
//*     sPrintF(textStrings[nt], "%i",  dbase.get<int >("tzDegreeTime")); nt++; 
//* 
//*     textLabels[nt] = "frequencies (x,y,z,t)"; 
//*     sPrintF(textStrings[nt], "%g, %g, %g, %g", dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],
//* 	     dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]); 
//*     nt++; 
//*     // null strings terminal list
//*     textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//* 
//*     addPrefix(textLabels,prefix,cmd,maxCommands);
//*     dialog.setTextBoxes(cmd, textLabels, textStrings);
//*   }
//* 
//*   aString answer;
//*   
//*   if( !executeCommand  )
//*   {
//*     gi.pushGUI(gui);
//*     gi.appendToTheDefaultPrompt("TZ parameters>");
//*   }
//* 
//*   int len;
//*   for(int it=0; ; it++)
//*   {
//*     if( !executeCommand )
//*       gi.getAnswer(answer,"");
//*     else
//*     {
//*       if( it==0 ) 
//*         answer=command;
//*       else
//*         break;
//*     }
//*   
//*     if( answer(0,prefix.length()-1)==prefix )
//*       answer=answer(prefix.length(),answer.length()-1);
//* 
//*     if( answer.matches("polynomial") )
//*     {
//*        dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::polynomial;
//*       printF("use polynomial\n");
//*     }
//*     else if( answer.matches("trigonometric") )
//*     {
//*        dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::trigonometric;
//*       printF("use trigonometric\n");
//* 
//*        dbase.get<bool >("userDefinedTwilightZoneCoefficients")=false;
//*     }
//*     else if( answer(0,4)=="pulse" )
//*     {
//*        dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::pulse;
//*       printF("use pulse\n");
//* 
//*        dbase.get<bool >("userDefinedTwilightZoneCoefficients")=false;
//*     }
//*     else if( answer=="turn on twilight zone" ||
//*              answer=="turn on twilight" )
//*     {
//*        dbase.get<bool >("twilightZoneFlow")=true;
//*       printF("Setting twilightZoneFlow=%i", dbase.get<bool >("twilightZoneFlow"));
//*     }
//*     else if( answer=="turn off twilight zone" ||
//*              answer=="turn off twilight" )
//*     {
//*        dbase.get<bool >("twilightZoneFlow")=false;
//*       printF("Setting twilightZoneFlow=%i", dbase.get<bool >("twilightZoneFlow"));
//*     }
//*     else if( answer=="turn on polynomial" )
//*     {
//*        dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::polynomial;
//*        dbase.get<bool >("twilightZoneFlow")=true;
//*       printF("turn on polynomial\n");
//*     }
//*     else if( answer=="turn on trigonometric" )
//*     {
//*        dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::trigonometric;
//*        dbase.get<bool >("twilightZoneFlow")=true;
//*       printF("turn on trigonometric\n");
//*     }
//*     else if( answer=="frequencies" )
//*     {
//*       gi.inputString(answer2,sPrintF(buff,"Enter the x,y,z,t frequencies (default =%f,%f,%f,%f)",
//* 				      dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]));
//*       if( answer2!="" )
//* 	sScanF(answer2,"%e %e %e %e",& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]);
//*       printF("(omegaX,omegaY,omegaZ,omegaT)=(%e,%e,%e,%e)\n", dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]);
//*     }
//*     else if( answer=="degree in space" )
//*     {
//*       gi.inputString(answer2,sPrintF(buff,"Enter degree in space (default =%i)", dbase.get<int >("tzDegreeSpace")));
//*       if( answer2!="" )
//* 	sScanF(answer2,"%i",& dbase.get<int >("tzDegreeSpace"));
//*       printF(" tzDegreeSpace= %i\n", dbase.get<int >("tzDegreeSpace"));
//*     }
//*     else if( answer=="degree in time" )
//*     {
//*       gi.inputString(answer2,sPrintF(buff,"Enter degree in time (default =%i)", dbase.get<int >("tzDegreeTime")));
//*       if( answer2!="" )
//* 	sScanF(answer2,"%i",& dbase.get<int >("tzDegreeTime"));
//*       printF(" tzDegreeTime=%i \n", dbase.get<int >("tzDegreeTime"));
//*     }
//*     else if( answer=="use 2D function in 3D" )
//*     {
//*        dbase.get<int >("dimensionOfTZFunction")=2;
//*     }
//*     else if( answer=="compare 3D run to 2D" )
//*     {
//*        dbase.get<int >("compare3Dto2D")=true;
//*        dbase.get<int >("dimensionOfTZFunction")=2;
//*     }
//* // ------- new versions
//*     else if( dialog.getToggleValue(answer,"twilight zone flow",dbase.get<bool >("twilightZoneFlow")) ){}//
//*     else if( answer.matches("frequencies (x,y,z,t)") )
//*     {
//*       answer2=answer(21,answer.length()-1);
//*       sScanF(answer2,"%e %e %e %e",& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],& dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]);
//*       printF("(omegaX,omegaY,omegaZ,omegaT)=(%e,%e,%e,%e)\n", dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]);
//* 
//*       dialog.setTextLabel("frequencies (x,y,z,t)",
//*                sPrintF(answer2, "%g, %g, %g, %g", dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1], dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3])); 
//*     }
//*     else if( len=answer.matches("degree in space") )
//*     {
//*       sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("tzDegreeSpace"));
//*       printF(" tzDegreeSpace= %i\n", dbase.get<int >("tzDegreeSpace"));
//*       dialog.setTextLabel("degree in space",sPrintF(answer2, "%i", dbase.get<int >("tzDegreeSpace")));
//*     }
//*     else if( len=answer.matches("degree in time") )
//*     {
//*       sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("tzDegreeTime"));
//*       printF(" tzDegreeTime=%i \n", dbase.get<int >("tzDegreeTime"));
//*       dialog.setTextLabel("degree in time",sPrintF(answer2, "%i", dbase.get<int >("tzDegreeTime")));
//*     }
//*     else if( answer=="assign polynomial coefficients" )
//*     {
//*       // printf(
//*       const int ndp=5;
//*       RealArray cx(ndp,ndp,ndp, dbase.get<int >("numberOfComponents"));   
//*       RealArray ct(ndp, dbase.get<int >("numberOfComponents")); 
//*       NameList nl;       
//* 
//*       if(  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==polynomial )
//*       {
//*         if(  dbase.get<OGFunction* >("exactSolution")==NULL )
//* 	{
//* 	  setTwilightZoneFunction( dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"), dbase.get<int >("tzDegreeSpace"), dbase.get<int >("tzDegreeTime"));
//* 	}
//* 	
//*         // get the current values of the coefficients
//* 	((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->getCoefficients( cx,ct );  // for u
//* 
//* 	displayPolynomialCoefficients(cx,ct, dbase.get<aString* >("componentName"), dbase.get<int >("numberOfComponents"),stdout);
//* 
//*       }
//*       else
//*       { // we allow changes to the coefficients even if we don't have  dbase.get<real >("a") polynomial TZ function -- this means we
//*         // can keep these changes in the command file even if they are not used.
//* 	cx=0.;
//* 	ct=0.;
//*       }
//*       
//*       printF("Make changes to the current coefficients of the polynomial twilight-zone function.\n"
//*              "Enter cx(mx,my,mz,mc)=value to set the coefficient of x^{mx} y^{my} z^mz for component mc \n"
//* 	     "Enter ct(mt,mc)=value to set the coefficient of t^{mt} for component mc \n"
//* 	     "Enter `done' to finish\n");
//*     
//*       int i0,i1,i2,i3;
//*       aString name;
//*       // ==========Loop for changing coefficients========================
//*       for( ;; ) 
//*       {
//* 	gi.inputString(answer,"Enter changes to cx or ct or `done' to finish\n"); 
//* 	if( answer=="done" || answer=="continue" || answer=="exit" ) break;
//* 	nl.getVariableName( answer, name );   // parse the answer
//* 
//* 	if( name== "cx" )   
//* 	{
//* 	  nl.getRealArray( answer,cx,i0,i1,i2,i3 );
//*           printF(" Setting cx(%i,%i,%i,%i)=%9.3e\n",i0,i1,i2,i3,cx(i0,i1,i2,i3));
//* 
//* 	   dbase.get<bool >("userDefinedTwilightZoneCoefficients")=true;
//* 	}
//* 	else if( name== "ct" )   
//* 	{
//* 	  nl.getRealArray( answer,ct,i0,i1 );
//*           printF(" Setting ct(%i,%i)=%9.3e\n",i0,i1,ct(i0,i1));
//* 
//* 	   dbase.get<bool >("userDefinedTwilightZoneCoefficients")=true;
//* 	}
//* 	else
//* 	  printF("unknown response: answer=[%s]\n",(const char*)answer);
//*       }
//* 
//*       if(  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==polynomial )
//*       {
//* 	((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( cx,ct );  // for u
//* 	displayPolynomialCoefficients(cx,ct, dbase.get<aString* >("componentName"), dbase.get<int >("numberOfComponents"),stdout);
//*       }
//*       else
//*       {
//* 	printF("WARNING: To set the polynomial coefficients th twilightzone function must be a polynomial\n"
//*                " The coefficients have not been changed");
//*       }
//*       
//* 
//*     }
//*     else if( len=answer.matches("use 2D function in 3D") )
//*     {
//*       int state=0;
//*       sScanF(answer(len,answer.length()-1),"%i",&state);
//*       if( state==1 )
//* 	 dbase.get<int >("dimensionOfTZFunction")=2;
//*       else
//* 	 dbase.get<int >("dimensionOfTZFunction")= numberOfDimensions; // is this set?
//*       dialog.setToggleState("use 2D function in 3D",state);
//*     }
//*     else if( len=answer.matches("compare 3D run to 2D") )
//*     {
//*       sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("compare3Dto2D"));
//*       if(  dbase.get<int >("compare3Dto2D") )
//* 	 dbase.get<int >("dimensionOfTZFunction")=2;
//*       else
//* 	 dbase.get<int >("dimensionOfTZFunction")= numberOfDimensions;
//*       dialog.setToggleState("compare 3D run to 2D", dbase.get<int >("compare3Dto2D"));
//*     }
//*     else if( len=answer.matches("assign TZ initial conditions") )
//*     {
//*       int state=0;
//*       sScanF(&answer[len],"%i",&state);
//*        dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow")=state;
//*       dialog.setToggleState("assign TZ initial conditions",state);
//*     }
//*     else if( answer=="no known solution" || 
//*              // answer=="supersonic flow in an expanding channel" ||  // no in userDefinedKnownSolution
//* 	     answer=="axisymmetric rigid body rotation" || 
//*              answer=="user defined known solution" )
//*     {
//*       knownSolution=(answer=="no known solution" ? noKnownSolution :userDefinedKnownSolution );
//* 
//*       if(  knownSolution==userDefinedKnownSolution )
//*       { // choose  dbase.get<real >("a") user defined known solution:
//*         if( updateUserDefinedKnownSolution(gi)==0 )
//* 	{
//*            knownSolution=noKnownSolution; // reset -- no known solution chosen.
//* 	}
//*       }
//*       printF(" Setting the known solution to %i (%s) \n",(int) knownSolution,(const char*)answer);
//*       if(  knownSolution!=noKnownSolution )
//*       {
//* 	 dbase.get<Parameters::InitialConditionOption >("initialConditionOption")=knownSolutionInitialCondition;
//*       }
//*       
//*     }
//*     else if( answer=="maximum norm" || 
//*              answer=="l1 norm" ||
//*              answer=="l2 norm" )
//*     {
//*        dbase.get<int >("errorNorm")= (answer=="maximum norm" ? INTEGER_MAX :
//*                   answer=="l1 norm" ? 1 :
//*                   answer=="l2 norm" ? 2 : INTEGER_MAX);
//*     }
//*     else
//*     {
//*       if( executeCommand )
//*       {
//* 	returnValue= 1;  // when executing  dbase.get<real >("a") single command, return 1 if the command was not recognised.
//*         break;
//*       }
//*       else
//*       {
//* 	printF("Unknown response: [%s]\n",(const char*)answer);
//* 	gi.stopReadingCommandFile();
//*       }
//*     }
//*   }
//*   
//*   if( !executeCommand  )
//*   {
//*     gi.popGUI();
//*     gi.unAppendTheDefaultPrompt();
//*   }
//* 
//*   return returnValue;
//* }

//* realMappedGridFunction& CnsParameters::
//* getKnownSolution(real t, int grid, const Index & I1, const Index &I2, const Index &I3, bool initialCall /* =false */  )
//* // ========================================================================================
//* // /Description:
//* //     Return a known solution on a component grid.
//* //    This routine assumes that getKnownSolution(cg,t) has been initially called to allocate space. 
//* // ========================================================================================
//* {
//*   if(  dbase.get<realCompositeGridFunction* >("pKnownSolution")==NULL )
//*   {
//*     printF("Parameters::getKnownSolution(grid):ERROR: you should call getKnownSolution(cg,t) first\n");
//*     Overture::abort("error");
//*   }
//*   
//*   int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
//*   realCompositeGridFunction & uKnown = * dbase.get<realCompositeGridFunction* >("pKnownSolution");
//*   CompositeGrid & cg = *uKnown.getCompositeGrid();
//* 
//*   // For moving grids or AMR refinement grids we always re-evaluate the known solution
//*   initialCall = initialCall || gridIsMoving(grid) ||
//*                 (  dbase.get<bool >("adaptiveGridProblem") && cg.refinementLevelNumber(grid)>0);  
//* 
//*   assert( grid < uKnown.numberOfComponentGrids() );
//* 
//*   MappedGrid & mg = cg[grid];
//*   realArray & ua = uKnown[grid]; 
//*   
//* 
//*   if(  dbase.get<KnownSolutionsEnum >("knownSolution")==userDefinedKnownSolution )
//*   {
//*     getUserDefinedKnownSolution(t,cg,grid,ua,I1,I2,I3);
//*   }
//*   else
//*   {
//*     printF("CnsParameters::getKnownSolution:ERROR: unknown knownSolution=%i\n", dbase.get<Parameters::KnownSolutionsEnum >("knownSolution"));
//*     Overture::abort("ERROR");
//*   }
//* 
//*   return uKnown[grid];
//* }

int CnsParameters::
getComponents( IntegerArray &component )
//==================================================================================
// /Description:
//    Get an array of component indices
//
// /component (output): the list of component indices
//\end{ParametersInclude.tex} 
//=================================================================================
{
  int numberToSet=int(dbase.get<int >("uc")>=0) + int(dbase.get<int >("vc")>=0) + 
    int(dbase.get<int >("wc")>=0) +
    int(dbase.get<int >("tc")>=0);
  component.redim(numberToSet+1);
  int n=0;
  component(n)=dbase.get<int >("rc"); n++;
  component(n)=dbase.get<int >("uc"); n++;
  component(n)=dbase.get<int >("vc"); n++;
  if( dbase.get<int >("wc")>=0 )
    { 
      component(n)=dbase.get<int >("wc"); n++;
    }
  component(n)=dbase.get<int >("tc"); 
  
  return 0;
}


//\begin{>>CnsParametersInclude.tex}{\subsection{setDefaultDataForBoundaryConditions}} 
int CnsParameters::
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg)
// ============================================================================================
// /Description:
//    Assign the default values for the data required by the boundary conditions.
//\end{CnsParametersInclude.tex}  
// ============================================================================================
{
  Range all;
  switch (cg[grid].boundaryCondition()(side,axis)) 
  {
    //kkc 080708 this case is not used and causes an error because the enum shares the number with Parameters::  
  case CnsParameters::inflowWithVelocityGiven:
  case CnsParameters::subSonicInflow:
    // data is set n.u = ...
    dbase.get<RealArray>("bcData")(all,side,axis,grid)=0.;
    break;
  case CnsParameters::outflow:
  case CnsParameters::subSonicOutflow:
  case CnsParameters::convectiveOutflow:
  case CnsParameters::tractionFree:
   //  data is a*p+b*p.n = c
    dbase.get<RealArray>("bcData")(0,side,axis,grid)=1.;
    dbase.get<RealArray>("bcData")(1,side,axis,grid)=1.;
    dbase.get<RealArray>("bcData")(2,side,axis,grid)=0.;
    
  }
  return 0;
  
}

bool
CnsParameters::isMixedBC(int bc) 
{ 
  Parameters::TimeSteppingMethod &timeSteppingMethod = dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  bool isImp = timeSteppingMethod==Parameters::implicit || timeSteppingMethod==Parameters::steadyStateNewton ;

  return  (bc==CnsParameters::outflow ||       
    bc==CnsParameters::subSonicOutflow || 
    bc==CnsParameters::convectiveOutflow ||
    bc==CnsParameters::tractionFree) && !isImp; 
}


// ===================================================================================================================
/// \brief Return the normal force on a boundary.
/// \details This routine is called, for example, by MovingGrids::rigidBodyMotion to determine 
///       the motion of a rigid body.
/// \param u (input): solution to compute the force from.
/// \param normalForce (output) : fill in the components of the normal force. 
/// \param ipar (input) : integer parameters. The boundary is defined by grid=ipar[0], side=ipar[1], axis=ipar[2] 
/// \param rpar (input) : real parameters. The current time is t=rpar[0]
/// \param includeViscosity (input) : if true include viscous stress terms in the force.
// ===================================================================================================================
int CnsParameters::
getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar,
		bool includeViscosity /* = true */ )
{
  int grid=ipar[0], side=ipar[1], axis=ipar[2];
  int form = ipar[3];
  real time =rpar[0];
  
  CompositeGrid & cg = *u.getCompositeGrid();
  assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
  assert( grid>=0 && grid<cg.numberOfComponentGrids());

  const int rc=dbase.get<int >("rc");
  const int uc=dbase.get<int >("uc");
  const int vc=dbase.get<int >("vc");
  const int wc=dbase.get<int >("wc");
  const int tc=dbase.get<int >("tc");

  const EquationOfStateEnum equationOfState = dbase.get<EquationOfStateEnum >("equationOfState");

  real mu = dbase.get<real >("mu");
  const Range V(uc,uc+cg.numberOfDimensions()-1);


  MappedGrid & mg = cg[grid];
  mg.update(MappedGrid::THEvertexBoundaryNormal);  // fix this ********************
      
  realArray & normal = mg.vertexBoundaryNormal(side,axis);
  const intArray & mask = mg.mask();
  realArray & vertex = mg.vertex();
  realArray & ug = u[grid];
  #ifndef USE_PPP
    realArray & fn = normalForce;
  #else
    realArray fn;
    Overture::abort("ERROR: finish me for parallel");
  #endif
      
  Index Ibv[3], &Ib1 =Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

 
  const real Rg=dbase.get<real >("Rg");
  const real gamma = dbase.get<real >("gamma");

  if( !includeViscosity )
  {
    mu=0.;  // turn off the viscous terms
  }
  
  realArray p(Ib1,Ib2,Ib3);  // ********* fix me for parallel ****************
  

  if( form==GridFunction::primitiveVariables )
  {
    if( equationOfState==idealGasEOS )
    {
      p=Rg*ug(Ib1,Ib2,Ib3,rc)*ug(Ib1,Ib2,Ib3,tc);
    }
    else if( equationOfState==stiffenedGasEOS )
    {
      // *wdh* 090408 -- check this --
      real gammaStiff=gamma;
      real pStiff=0.;
      dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",gammaStiff);
      dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",pStiff);
      
      printF("CnsParameters::getNormalForce:prim:stiffenedGasEOS:INFO: gammaStiff=%9.3e, pStiff=%9.3e\n",
             gammaStiff,pStiff);

      // p = rho*T (since T is defined as p/rho, even for a stiffened gas)

      p=Rg*ug(Ib1,Ib2,Ib3,rc)*ug(Ib1,Ib2,Ib3,tc);

    }
    else
    {
      printF("CnsParameters::getNormalForce:ERROR: equationOfState=%i not implemeneted yet!\n",(int)equationOfState);
    }
    
  }
  else
  {
    // --- conservative variables ---
    if( equationOfState==idealGasEOS )
    {
      //  E = ( p/(gamma-1) + .5*rho*u*u )

      if( mg.numberOfDimensions()==1 )
      {
	p= (gamma-1.)*( ug(Ib1,Ib2,Ib3,tc)-.5*( SQR(ug(Ib1,Ib2,Ib3,uc)))/ug(Ib1,Ib2,Ib3,rc) );
      }
      else if( mg.numberOfDimensions()==2 )
      {
	p= (gamma-1.)*( ug(Ib1,Ib2,Ib3,tc)-
			.5*( SQR(ug(Ib1,Ib2,Ib3,uc))+ SQR(ug(Ib1,Ib2,Ib3,vc)) )/ug(Ib1,Ib2,Ib3,rc) );
      }
      else
      {
	p= (gamma-1.)*( ug(Ib1,Ib2,Ib3,tc)-
			.5*( SQR(ug(Ib1,Ib2,Ib3,uc))+ SQR(ug(Ib1,Ib2,Ib3,vc))+ SQR(ug(Ib1,Ib2,Ib3,wc)) )/ug(Ib1,Ib2,Ib3,rc) );
      }
    }
    else if( equationOfState==stiffenedGasEOS )
    {
      // *wdh* 090408 -- check this --
      real gammaStiff=gamma;
      real pStiff=0.;
      dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("alphaMG",gammaStiff);
      dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("betaMG",pStiff);

      printF("CnsParameters::getNormalForce:cons:stiffenedGasEOS:INFO: gammaStiff=%9.3e, pStiff=%9.3e\n",
             gammaStiff,pStiff);
      
      //  p = (gammaStiff-1)* rho * e -  gammaStiff*pStiff
      // rho*e = E - .5*m^2/rho , m=rho*u
      // note that here, ug(Ib1,Ib2,Ib3,tc) actually holds E, the total energy
      if( mg.numberOfDimensions()==1 )
      { 
	p= (gammaStiff-1.)*( ug(Ib1,Ib2,Ib3,tc)-.5*( SQR(ug(Ib1,Ib2,Ib3,uc)))/ug(Ib1,Ib2,Ib3,rc) ) 
            -  gammaStiff*pStiff;
      }
      else if( mg.numberOfDimensions()==2 )
      {
	p= (gammaStiff-1.)*( ug(Ib1,Ib2,Ib3,tc)-
			.5*( SQR(ug(Ib1,Ib2,Ib3,uc))+ SQR(ug(Ib1,Ib2,Ib3,vc)) )/ug(Ib1,Ib2,Ib3,rc) )
             -  gammaStiff*pStiff;
      }
      else
      {
	p= (gammaStiff-1.)*( ug(Ib1,Ib2,Ib3,tc)-
			.5*( SQR(ug(Ib1,Ib2,Ib3,uc))+ SQR(ug(Ib1,Ib2,Ib3,vc))+ SQR(ug(Ib1,Ib2,Ib3,wc)) )/ug(Ib1,Ib2,Ib3,rc) )
            -  gammaStiff*pStiff;
      }


    }
    else
    {
      printF("CnsParameters::getNormalForce:ERROR: equationOfState=%i not implemeneted yet!\n",(int)equationOfState);
    }

    
  }


  // Offset the pressure by this amount
  const real boundaryForcePressureOffset =dbase.get<real>("boundaryForcePressureOffset");
  if( boundaryForcePressureOffset!=0. )
  {
    p-=boundaryForcePressureOffset;
  }
  

  if( mu==0. )
  {
    if( cg.numberOfDimensions()==2 )
    {
      fn(Ib1,Ib2,Ib3,0)=p*normal(Ib1,Ib2,Ib3,0);
      fn(Ib1,Ib2,Ib3,1)=p*normal(Ib1,Ib2,Ib3,1);
    }
    else
    {
      fn(Ib1,Ib2,Ib3,0)=p*normal(Ib1,Ib2,Ib3,0);
      fn(Ib1,Ib2,Ib3,1)=p*normal(Ib1,Ib2,Ib3,1);
      fn(Ib1,Ib2,Ib3,2)=p*normal(Ib1,Ib2,Ib3,2);
    }
  }
  else
  {
    
    // This next section also appears in AsfParameters

    CompositeGridOperators & cgop = *u.getOperators();
    MappedGridOperators & op = cgop[grid];
	  
    // **NOTE** we assume here that we have the velocity components 
    assert( form==GridFunction::primitiveVariables );

    realArray ux(Ib1,Ib2,Ib3,V), uy(Ib1,Ib2,Ib3,V), uz, div(Ib1,Ib2,Ib3);
    op.derivative(MappedGridOperators::xDerivative,ug,ux,Ib1,Ib2,Ib3,V);
    op.derivative(MappedGridOperators::yDerivative,ug,uy,Ib1,Ib2,Ib3,V);
    if( mg.numberOfDimensions()>=3 )
    {
      uz.redim(Ib1,Ib2,Ib3,V);
      op.derivative(MappedGridOperators::zDerivative,ug,uz,Ib1,Ib2,Ib3,V);
    }
	  
    const real lambda = -(2./3.)*mu; // Stokes hypothesis
  

    if( cg.numberOfDimensions()==2 )
    {
      // stress = [ 2 mu u_x + lambda div(u)    mu( u_y + v_x)                mu(w_x + u_z)  ]
      //          [ mu(v_x+u_y)              2 mu v_y + lambda div(u)         mu(v_z+w_y)    ]
      //          [ mu(w_x+u_z)                 mu(v_z+w_y)           2 mu w_z + lambda div(u) ]
      div=ux(Ib1,Ib2,Ib3,uc)+uy(Ib1,Ib2,Ib3,vc);
      fn(Ib1,Ib2,Ib3,0)=( p(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,0)
			  -( ( (2.*mu)*ux(Ib1,Ib2,Ib3,uc) +lambda*div     )*normal(Ib1,Ib2,Ib3,0)+
			     ( mu*(uy(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,vc)) )*normal(Ib1,Ib2,Ib3,1)) );

      fn(Ib1,Ib2,Ib3,1)=( p(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,1)
			  -( ( mu*(ux(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,uc)) )*normal(Ib1,Ib2,Ib3,0)+
			     ( lambda*div +(2.*mu)*uy(Ib1,Ib2,Ib3,vc)     )*normal(Ib1,Ib2,Ib3,1) ) );
    }
    else
    {
      div=ux(Ib1,Ib2,Ib3,uc)+uy(Ib1,Ib2,Ib3,vc)+uz(Ib1,Ib2,Ib3,wc);
      fn(Ib1,Ib2,Ib3,0)=( p(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,0)
			  -( ( (2.*mu)*ux(Ib1,Ib2,Ib3,uc) +lambda*div     )*normal(Ib1,Ib2,Ib3,0)+
			     ( mu*(uy(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,vc)) )*normal(Ib1,Ib2,Ib3,1)+
			     ( mu*(uz(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,wc)) )*normal(Ib1,Ib2,Ib3,2) ) );


      fn(Ib1,Ib2,Ib3,1)=( p(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,1)
			  -( ( mu*(ux(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,uc)) )*normal(Ib1,Ib2,Ib3,0)+
			     ( lambda*div +(2.*mu)*uy(Ib1,Ib2,Ib3,vc)     )*normal(Ib1,Ib2,Ib3,1)+
			     ( mu*(uz(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,wc)) )*normal(Ib1,Ib2,Ib3,2) ) );

      fn(Ib1,Ib2,Ib3,2)=( p(Ib1,Ib2,Ib3)*normal(Ib1,Ib2,Ib3,2)
			  -( ( mu*(uz(Ib1,Ib2,Ib3,uc)+ux(Ib1,Ib2,Ib3,wc)) )*normal(Ib1,Ib2,Ib3,0)+
			     ( mu*(uz(Ib1,Ib2,Ib3,vc)+uy(Ib1,Ib2,Ib3,wc)) )*normal(Ib1,Ib2,Ib3,1)+
			     ( lambda*div +(2.*mu)*uz(Ib1,Ib2,Ib3,wc)     )*normal(Ib1,Ib2,Ib3,2) ) );
    }
  }
  
  // ----------------------------------------------------------------------
  // -------- fill in ghost values on this face by extrapolation ----------
  // --These are needed for FSI problems as cgsm wants ghost values too ---
  // ----------------------------------------------------------------------
  // *wdh* added 2015/07/15 

  if( false )
  {
    const int numberOfDimensions=mg.numberOfDimensions();
    Range Rx=numberOfDimensions;

    const IntegerArray & gid =mg.gridIndexRange();
    int numGhost=1;
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

    // --- face = (side,axis) has 2 adjacent faces in 2D and 4 adjacent faces in 3D 
    for( int dir=1; dir<numberOfDimensions; dir++ )  // loop over other axes
    {
      const int axisp = (axis+dir) % numberOfDimensions;  
    
      is1=is2=is3=0;
      for( int s1=0; s1<=1; s1++ ) // two sides in direction axisp
      {
	getBoundaryIndex(gid,side,axis,Ib1,Ib2,Ib3,numGhost); // include ghost so that corners get done in 3D after two steps

	Ibv[axisp] = gid(s1,axisp);  // defines the adjacent face 
	isv[axisp]= 1-2*s1;

	for( int ghost=0; ghost<numGhost; ghost++ )
	{
	  Index Jb1=Ib1-ghost*is1, Jb2=Ib2-ghost*is2, Jb3=Ib3-ghost*is3;
	  fn(Jb1-is1,Jb2-is2,Jb3-is3,Rx)=3.*fn(Jb1,Jb2,Jb3,Rx)-3.*fn(Jb1+is1,Jb2+is2,Jb3+is3,Rx)+fn(Jb1+2*is1,Jb2+2*is2,Jb3+2*is3,Rx);
	}
      }
    }
  }
  

  return 0;
}

