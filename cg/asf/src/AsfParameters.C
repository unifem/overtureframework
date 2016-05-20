#include "AsfParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "Reactions.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "GridFunction.h"
#include "MultiComponent.h"

using namespace CG;
int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);


//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in AsfParameters}} 
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
// /real machNumber, reynoldsNumber, prandtlNumber: PDE parameters ASF
// /real mu, kThermal, Rg, gamma, avr, anu: for  ASF
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
AsfParameters::
AsfParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
// ==================================================================================
// /pde0: Indicated which PDE we are solving
//
//\end{ParametersInclude.tex}
//===================================================================================
{

  // testing: add some parameters to the db
  DataBase & modelParameters = dbase.get<DataBase >("modelParameters");
  
  modelParameters.put<real>("gamma", dbase.get<real >("gamma"));
  modelParameters.put<real>("mu", dbase.get<real >("mu"));
  modelParameters.put<real>("kThermal", dbase.get<real >("kThermal"));
  
  if ( !dbase.has_key("algorithmVariation") ) 
    dbase.put<AlgorithmVariation>("algorithmVariation",densityFromGasLawAlgorithm);
 
  if ( !dbase.has_key("useDivergenceBoundaryCondition") ) 
    dbase.put<bool>("useDivergenceBoundaryCondition",false); // turn this off by default
 
  if (!dbase.has_key("testProblem")) dbase.put<AsfParameters::TestProblems>("testProblem");
  dbase.get<AsfParameters::TestProblems >("testProblem")=AsfParameters::standard;  // *wdh* 070708

  dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=implicitAllSpeed;
  dbase.get<int >("explicitMethod")=false;

  registerBC((int)convectiveOutflow,"convectiveOutflow");
  registerBC((int)tractionFree,"tractionFree");
  registerBC((int)subSonicInflow,"subSonicInflow");
  registerBC((int)subSonicOutflow,"subSonicOutflow");

  // kkc 090402 add the multi-component material manager 
  if ( !dbase.has_key("Mixture")) dbase.put<MixtureP>("Mixture", new IdealGasMixture() );
  CG::setMixtureContext(*dbase.get<MixtureP>("Mixture"));

  // initialize the items that we time: 
  initializeTimings();

}

AsfParameters::
~AsfParameters()
{
}


int AsfParameters::
setParameters(const int & numberOfDimensions0 /* =2 */ , 
              const aString & reactionName_ /* =nullString */ )
// ==================================================================================================
//  /reactionName (input) : optional name of a reaction oe a reaction 
//     file that defines the chemical reactions, such as
//      a Chemkin binary file. 
// ==================================================================================================
{

  dbase.get<int >("numberOfDimensions")=numberOfDimensions0;
  dbase.get<int >("rc")= dbase.get<int >("uc")= dbase.get<int >("vc")= dbase.get<int >("wc")= dbase.get<int >("pc")= dbase.get<int >("tc")= dbase.get<int >("sc")= dbase.get<int >("kc")= dbase.get<int >("epsc")= dbase.get<int >("sec")=-1;
  
  dbase.get<aString >("reactionName")=reactionName_;
  if(  dbase.get<aString >("reactionName")!=nullString &&  dbase.get<aString >("reactionName")!="" )
    {
      dbase.get<bool >("computeReactions")=TRUE;
      // This next function will assign the number of species and build  dbase.get<real >("a") reaction object.
      buildReactions();
    }
  else
    {
      dbase.get<bool >("computeReactions")=FALSE;
      dbase.get<Reactions* >("reactions")=NULL;
      dbase.get<int >("numberOfSpecies")=0;
      
    }
  
  int s, i;
  //...set component index'es, showVariables, etc. that are equation-specific

  dbase.get<int >("numberOfComponents")=0; 
  dbase.get<int >("pc")= dbase.get<int >("numberOfComponents")++;    
  dbase.get<int >("rc")= dbase.get<int >("numberOfComponents")++;    //  density = u(all,all,all, dbase.get<int >("rc"))
  dbase.get<int >("uc")= dbase.get<int >("numberOfComponents")++;    //  u velocity component = u(all,all,all, dbase.get<int >("uc"))
  if(  dbase.get<int >("numberOfDimensions")>1 )  dbase.get<int >("vc")= dbase.get<int >("numberOfComponents")++;
  if(  dbase.get<int >("numberOfDimensions")>2 )  dbase.get<int >("wc")= dbase.get<int >("numberOfComponents")++;
  dbase.get<int >("tc")= dbase.get<int >("numberOfComponents")++;
  if(  dbase.get<bool >("computeReactions") )
    {
      dbase.get<int >("sc")= dbase.get<int >("tc")+1;
      dbase.get<int >("numberOfComponents")+= dbase.get<int >("numberOfSpecies");
    }
  dbase.get<Range >("Ru")=Range( dbase.get<int >("uc"),dbase.get<int >("uc")+ dbase.get<int >("numberOfDimensions")-1);    // velocity components
  dbase.get<Range >("Rt")=Range( dbase.get<int >("pc"), dbase.get<int >("numberOfComponents")-1);      // time dependent components
  
  //     equationNumber.redim(numberOfComponents);
  //     for (i=0; i<numberOfComponents; i++) equationNumber(i) = i;
  
  dbase.get<RealArray >("artificialDiffusion").redim( dbase.get<int >("numberOfComponents"));
  dbase.get<RealArray >("artificialDiffusion")=0.;
  
  addShowVariable( "rho", dbase.get<int >("rc") );
  addShowVariable( "u", dbase.get<int >("uc") );
  if(  dbase.get<int >("numberOfDimensions")>1 )
    addShowVariable( "v", dbase.get<int >("vc") );
  if(  dbase.get<int >("numberOfDimensions")>2 )
    addShowVariable( "w", dbase.get<int >("wc") );
  
  addShowVariable( "T", dbase.get<int >("tc") );
  addShowVariable( "Mach Number", dbase.get<int >("numberOfComponents")+1 );
  addShowVariable( "p", dbase.get<int >("numberOfComponents")+1 );
  if(  dbase.get<int >("numberOfDimensions")<3 )
    addShowVariable( "vorticity", dbase.get<int >("numberOfComponents")+1,FALSE ); // FALSE=turned off by default
  else
  {
    addShowVariable( "vorticityX", dbase.get<int >("numberOfComponents")+1,FALSE ); // FALSE=turned off by default
    addShowVariable( "vorticityY", dbase.get<int >("numberOfComponents")+1,FALSE ); // FALSE=turned off by default
    addShowVariable( "vorticityZ", dbase.get<int >("numberOfComponents")+1,FALSE ); // FALSE=turned off by default
  }
  addShowVariable( "divergence", dbase.get<int >("numberOfComponents")+1,FALSE ); 
  addShowVariable( "speed", dbase.get<int >("numberOfComponents")+1,FALSE ); 
  
  // species:
  if(  dbase.get<int >("numberOfSpecies")>0 )
    {
    if(  dbase.get<aString >("reactionName")=="one equation mixture fraction" )
    {
      addShowVariable( "f", dbase.get<int >("sc")  ,true );
    }
    else if(  dbase.get<aString >("reactionName")=="two equation mixture fraction and extent of reaction" )
    {
      addShowVariable( "f", dbase.get<int >("sc")  ,true );
      addShowVariable( "c", dbase.get<int >("sc")+1  ,true );
    }
    else
    {
      assert(  dbase.get<Reactions* >("reactions")!=NULL );
      for( s=0; s< dbase.get<int >("numberOfSpecies"); s++ )
	addShowVariable(  dbase.get<Reactions* >("reactions")->getName(s), dbase.get<int >("sc")+s );
    }
  }

  delete  dbase.get<aString* >("componentName");
   dbase.get<aString* >("componentName")= new aString [ dbase.get<int >("numberOfComponents")];

  if(  dbase.get<int >("rc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("rc")]="r";
  if(  dbase.get<int >("uc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("uc")]="u";
  if(  dbase.get<int >("vc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("vc")]="v";
  if(  dbase.get<int >("wc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("wc")]="w";
  if(  dbase.get<int >("pc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("pc")]="p";
  if(  dbase.get<int >("tc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("tc")]="T";
  if(  dbase.get<int >("kc")>=0 )
  {
    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==SpalartAllmaras )
       dbase.get<aString* >("componentName")[ dbase.get<int >("kc")]="n";  // for nuT
    else
       dbase.get<aString* >("componentName")[ dbase.get<int >("kc")]="k";
  }
  
  if(  dbase.get<int >("epsc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("epsc")]="epsilon";

  int scp =  dbase.get<int >("sc");
  if(  dbase.get<bool >("advectPassiveScalar") )
  {
     dbase.get<aString* >("componentName")[scp]="s";   // use "s" as  dbase.get<real >("a") name for now, "passive";
  }
  else if(  dbase.get<int >("numberOfSpecies")>0 )
  {
    int numberOfActiveSpecies=  dbase.get<int >("numberOfSpecies");
    if( numberOfActiveSpecies>0 )
    {
      assert(  dbase.get<Reactions* >("reactions")!=NULL );
      for( s=0; s<numberOfActiveSpecies; s++ )
	 dbase.get<aString* >("componentName")[scp+s]= dbase.get<Reactions* >("reactions")->getName(s);
    }
    
  }
  
  if(  dbase.get<int >("sec")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("sec")]="Tb";

  if(  dbase.get<int >("numberOfExtraVariables")>0 )
  {
    aString buff;
    for( int e=0; e< dbase.get<int >("numberOfExtraVariables"); e++ )
    {
      int n= dbase.get<int >("numberOfComponents")- dbase.get<int >("numberOfExtraVariables")+e;
       dbase.get<aString* >("componentName")[n]=sPrintF(buff,"Var%i",e);
      addShowVariable(  dbase.get<aString* >("componentName")[n],n );
    }

  }
  

   dbase.get<int >("stencilWidthForExposedPoints")=3;
   dbase.get<int >("extrapolateInterpolationNeighbours")=false;

   dbase.get<real >("inflowPressure")=1.;
   dbase.get<RealArray >("initialConditions").redim( dbase.get<int >("numberOfComponents"));  dbase.get<RealArray >("initialConditions")=defaultValue;
  
   dbase.get<RealArray >("checkFileCutoff").redim( dbase.get<int >("numberOfComponents")+1);  // cutoff's for errors in checkfile
   dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  //  dbase.get<RealArray >("checkFileCutoff").display("checkFileCutOff");

  dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=implicitAllSpeed;
  dbase.get<int >("explicitMethod")=false;
  
  return 0;
}


//\begin{>>AsfParametersInclude.tex}{\subsection{setTwilightZoneFunction}} 
int AsfParameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
// =============================================================================================
// /Description:
//
// /choice (input): AsfParameters::polynomial or AsfParameters::trigonometric
//\end{AsfParametersInclude.tex}
// =============================================================================================
{
  TwilightZoneChoice choice=choice_;
  
  //TODO: add TZ for passive scalar=passivec
  if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
  {
    printF("Parameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
           "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
  }

  delete  dbase.get<OGFunction* >("exactSolution");
  if( choice==polynomial )
  {
    // ******* polynomial twilight zone function ******
     dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, dbase.get<int >("numberOfDimensions"), dbase.get<int >("numberOfComponents"),degreeTime);

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, dbase.get<int >("numberOfComponents"));  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, dbase.get<int >("numberOfComponents"));      
    timeCoefficientsForTZ=0.;

    if(  dbase.get<int >("numberOfDimensions")==1 )
    {
      if( degreeSpace==2 )
      {
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=1.;      // r=1+.5*x^2   
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("rc"))=.5;

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("uc"))=1.;      // u=x^2 +x
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("uc"))=1.;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=1.;      // p=1-x
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=-1.;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=1.;      // T=1+x
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("tc"))=1.;
      }
      else if( degreeSpace==1 )
      {
	printF("allSpeed: Setting TZ flow to be r=1, u=3+x+y, p=5+x+y  \n");
      
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=2.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("rc"))=.0;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("rc"))=.0;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("uc"))=3.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("uc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("uc"))=1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=5.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("pc"))=1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=8.;      // T should remain positive  (rho = p/(RT))
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("tc"))=.125;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("tc"))=.125;
      }
      else
	Overture::abort("error");
    }
    else if(  dbase.get<int >("numberOfDimensions")==2  ||  dbase.get<int >("dimensionOfTZFunction")==2 )
    {
      if( degreeSpace==2 )
      {
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=4.;      // r=1+.5*x^2 + .5y^2
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("rc"))=.1;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("rc"))=-.1;
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("rc"))=.5;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("rc"))=.5;

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("uc"))=.25;      // .25 x^2
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("uc"))=.5;       // .5 x
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("uc"))=-.4;      // -.4 y^2
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("uc"))=-.75;     // -.75 y
	spatialCoefficientsForTZ(1,1,0, dbase.get<int >("uc"))=-1.;      // - x*y

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("vc"))=-.25;
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("vc"))=-.5;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("vc"))=+.25; 
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("vc"))=+.6;
	spatialCoefficientsForTZ(1,1,0, dbase.get<int >("vc"))=+.5;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=5.;      // p ** p should be positive **
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=.1;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("pc"))=-.2;
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("pc"))=.4;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("pc"))=.5;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=8.;      // T should remain positive  (rho = p/(RT))
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("tc"))= .1;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("tc"))=-.1;
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("tc"))=.35;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("tc"))=.45;
      }
      else if( degreeSpace==1 )
      {
	printF("\n allSpeed: Setting TZ flow to be r=2, u=3+x+y, v=4+x-y, p=5+x+y  \n");
      
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=2.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("rc"))=.0;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("rc"))=.0;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("uc"))=3.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("uc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("uc"))=1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("vc"))=4.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("vc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("vc"))=-1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=5.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("pc"))=1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=8.;      // T should remain positive  (rho = p/(RT))
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("tc"))=.125;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("tc"))=.125;

      }
      else
	Overture::abort("error");
    }
    else if(  dbase.get<int >("numberOfDimensions")==3 )
    {
      if( degreeSpace==2 )
      {
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=1.;      // r=1+.5*x^2 + .5y^2 + .5z^2
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("rc"))=.2;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("rc"))=.2;      // take rho constant so exact for p equation div( p.x/rho)
	spatialCoefficientsForTZ(0,0,2, dbase.get<int >("rc"))=.2;

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("uc"))=1.;      // u=x^2 + 2xy + y^2 + xz
	// spatialCoefficientsForTZ(1,1,0, dbase.get<int >("uc"))=2.;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("uc"))=1.;
	// spatialCoefficientsForTZ(1,0,1, dbase.get<int >("uc"))=1.;

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("vc"))= 1.;      // v=x^2 -2xy - y^2 + 3yz
	// spatialCoefficientsForTZ(1,1,0, dbase.get<int >("vc"))=-2.;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("vc"))=-1.;
	// spatialCoefficientsForTZ(0,1,1, dbase.get<int >("vc"))=+3.;

	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("wc"))= 1.;      // w=x^2 + y^2 - 2 z^2
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("wc"))= 1.;
	spatialCoefficientsForTZ(0,0,2, dbase.get<int >("wc"))=-2.;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=5.;      // p=5+x+y   ** p should be positive **
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=+1.;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("pc"))=+1.;
	spatialCoefficientsForTZ(0,0,1, dbase.get<int >("pc"))=+1.;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=8.;      // T should remain positive  (rho = p/(RT))
	spatialCoefficientsForTZ(2,0,0, dbase.get<int >("tc"))=.125;
	spatialCoefficientsForTZ(0,2,0, dbase.get<int >("tc"))=.125;
	spatialCoefficientsForTZ(0,0,2, dbase.get<int >("tc"))=.125;
      }
      else if( degreeSpace==1 )
      {
	printF("\n allSpeed: Setting TZ flow to be r=1, u=3+x+y, v=4+x-y, p=5+x+y  \n");
      
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("rc"))=1.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("rc"))=.0;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("rc"))=.0;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("uc"))=3.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("uc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("uc"))=1.;     
	spatialCoefficientsForTZ(0,0,1, dbase.get<int >("uc"))=-.5;

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("vc"))=4.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("vc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("vc"))=-1.;     
	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("vc"))=.25;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("wc"))=2.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("wc"))=.5;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("wc"))=.5;     
	spatialCoefficientsForTZ(0,0,1, dbase.get<int >("wc"))=.5;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("pc"))=5.;     
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("pc"))=1.;     
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("pc"))=1.;     
	spatialCoefficientsForTZ(0,0,1, dbase.get<int >("pc"))=1.;     

	spatialCoefficientsForTZ(0,0,0, dbase.get<int >("tc"))=8.;      // T should remain positive  (rho = p/(RT))
	spatialCoefficientsForTZ(1,0,0, dbase.get<int >("tc"))=.125;
	spatialCoefficientsForTZ(0,1,0, dbase.get<int >("tc"))=.125;
      }
      else
	Overture::abort("error");
    }
    else
      Overture::abort("error");
    if(  dbase.get<bool >("computeReactions") )
    {
      // assign species
      for( int n= dbase.get<int >("sc"); n< dbase.get<int >("numberOfComponents"); n++ )
      {
	real ni =1./(n+1);
    
	spatialCoefficientsForTZ(0,0,0,n)=1.;      
	if( degreeSpace>0 )
	{
	  spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	  spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,0,1,n)=  dbase.get<int >("numberOfDimensions")==3 ? .25*ni : 0.;
	}
	if( degreeSpace>1 )
	{
	  spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	  spatialCoefficientsForTZ(0,0,2,n)=  dbase.get<int >("numberOfDimensions")==3 ? .125*ni : 0.;
	}
      }
    }

    for( int n=0; n< dbase.get<int >("numberOfComponents"); n++ )
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
    RealArray fx( dbase.get<int >("numberOfComponents")),fy( dbase.get<int >("numberOfComponents")),fz( dbase.get<int >("numberOfComponents")),ft( dbase.get<int >("numberOfComponents"));
    RealArray gx( dbase.get<int >("numberOfComponents")),gy( dbase.get<int >("numberOfComponents")),gz( dbase.get<int >("numberOfComponents")),gt( dbase.get<int >("numberOfComponents"));
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude( dbase.get<int >("numberOfComponents")), cc( dbase.get<int >("numberOfComponents"));
    amplitude=1.;
    cc=0.;

    

    fx= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
    fy =  dbase.get<int >("numberOfDimensions")>1 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1] : 0.;
    fz =  dbase.get<int >("numberOfDimensions")>2 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2] : 0.;
    ft =  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3];

    // do NOT make velocity diveregence free
    // u=   cos(pi x) cos( pi y ) cos( pi z)  
    // v=.5 sin(pi x) sin( pi y ) cos( pi z)
    // w=   sin(pi x) cos( pi y ) sin( pi z)
    gx( dbase.get<int >("vc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
    gy( dbase.get<int >("vc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1];
    amplitude( dbase.get<int >("vc"))=.5;
    if(  dbase.get<int >("numberOfDimensions")==3 )
    {
      gx( dbase.get<int >("wc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
      gz( dbase.get<int >("wc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2];
      amplitude( dbase.get<int >("wc"))=-.5;
    }
    // make the temperature, pressure and density positive
    amplitude( dbase.get<int >("rc"))=.25; cc( dbase.get<int >("rc"))=1.;  gx( dbase.get<int >("rc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
    amplitude( dbase.get<int >("tc"))=.5;  cc( dbase.get<int >("tc"))=1.;  gy( dbase.get<int >("tc"))=.5/ dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1];
    amplitude( dbase.get<int >("pc"))=.5;  cc( dbase.get<int >("pc"))=2.;  
    

     dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
      
  }
  else if( choice==pulse ) 
  {
    // ******* Pulse function chosen ******
     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( dbase.get<int >("numberOfDimensions"), dbase.get<int >("numberOfComponents")); 

    // this pulse function is not divergence free!

  }
    
  
  return 0;

}



static int 
fillCompressibleDialogValues(DialogData & dialog, 
			     Parameters & parameters )
// ======================================================================================================
// /Description:
//     Fill values into the Dialog for the Asf parameters.
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
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("Rg")));  nt++;
    // dialog.setTextLabel(nt,);  nt++;
  }
  else
  {
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("reynoldsNumber"))); nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g",parameters.dbase.get<real >("machNumber"))); nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/Re",parameters.dbase.get<real >("mu")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g== gam/((gam-1)*Pr*Re)",parameters.dbase.get<real >("kThermal")));  nt++;
    dialog.setTextLabel(nt,sPrintF(line, "%g==1/(gam*M*M)",parameters.dbase.get<real >("Rg")));  nt++;

  }

  return 0;
}


static int
buildCompressibleDialog(DialogData & dialog, 
                        aString & prefix,
                        AsfParameters & parameters )
// ===========================================================================================
// /Description: 
//    Build the dialog for the parameters for the Compressible NS equations
// ==========================================================================================
{
  dialog.closeDialog();

  ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  dialog.setWindowTitle("All-Speed NS parameters");

  const int numberOfUserVariables=parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").size();
  const int maxCommands=26+numberOfUserVariables;
  aString *cmd = new aString[maxCommands];

  const int numberOfTextStrings=27+parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").size();
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

  aString aLabel[] = {"defaultAlgorithm","densityFromGasLawAlgorithm",""}; //
  addPrefix(aLabel,prefix,cmd,maxCommands);
  dialog.addOptionMenu("algorithm", cmd,aLabel, parameters.dbase.get<AsfParameters::AlgorithmVariation >("algorithmVariation"));

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

  textLabels[nt] = "Rg (gas constant)";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("Rg"));  nt++; 
  textLabels[nt] = "gamma";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("gamma"));  nt++; 
  textLabels[nt] = "Prandtl number";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("prandtlNumber"));  nt++; 
  textLabels[nt] = "gravity";  
  sPrintF(textStrings[nt], "%g,%g,%g",gravity[0],gravity[1],gravity[2]);  nt++; 

  textLabels[nt] = "pressureLevel";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("pressureLevel"));  nt++; 
  textLabels[nt] = "anu";  sPrintF(textStrings[nt], "%g",parameters.dbase.get<real >("anu"));  nt++; 

  textLabels[nt] = "ad21,ad22";  sPrintF(textStrings[nt], "%g,%g",parameters.dbase.get<real >("ad21"),parameters.dbase.get<real >("ad22"));  nt++; 
  textLabels[nt] = "ad41,ad42";  sPrintF(textStrings[nt], "%g,%g",parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"));  nt++; 
  textLabels[nt] = "ad61,ad62";  sPrintF(textStrings[nt], "%g,%g",parameters.dbase.get<real >("ad61"),parameters.dbase.get<real >("ad62"));  nt++; 


  textLabels[nt] = "slip wall boundary condition option";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("slipWallBoundaryConditionOption"));  nt++;


  // add on user defined variables

  ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");

  std::list<ShowFileParameter>::iterator iter; 
  for(iter = pdeParameters.begin(); iter!=pdeParameters.end(); iter++ )
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

  aString tbLabels[] = {"linearize implicit method",
                        "second-order artificial diffusion", 
			"fourth-order artificial diffusion",
			"sixth-order artificial diffusion",
			""};
  int tbState[4];
  tbState[0] = parameters.dbase.get<int >("linearizeImplicitMethod");
  tbState[1] = parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion");
  tbState[2] = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion");
  tbState[3] = parameters.dbase.get<bool >("useSixthOrderArtificialDiffusion");
  int numColumns=1;
  addPrefix(tbLabels,prefix,cmd,maxCommands);
  dialog.setToggleButtons(cmd, tbLabels, tbState, numColumns); 

  // dialog.openDialog();

  delete [] textLabels;
  delete [] textStrings;
  delete [] cmd;

  return 0;
}





int AsfParameters::
setPdeParameters(CompositeGrid & cg, const aString & command /* = nullString */,
                 DialogData *interface /* =NULL */ )
// =====================================================================================
// /Description:
//   Prompt for changes in the PDE parameters.
// =====================================================================================
{
  int returnValue=0;

  // printF("\n &&&&&&&&&&&&&&& AsfParameters::setPdeParameters &&&&&&&&&&&&&\n");

  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  aString prefix = "OBPDE:"; // prefix for commands to make them unique.

  // ** Here we only look for commands that have the proper prefix ****
  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  aString answer;
  char buff[100];
//  const int numberOfDimensions = cg.numberOfDimensions();


  aString *pdeParametersMenu=NULL;

//\begin{>>setParametersInclude.tex}{\subsubsection{PDE parameters for ASF}\label{sec:asfPdeParams}}
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

  pdeParametersMenu[n++]="pressure level";
  pdeParametersMenu[n++]="remove fast pressure waves (toggle)";
	
  pdeParametersMenu[n]="";
  assert( n<maxMenuItems );

  ArraySimpleFixed<real,3,1,1,1> & gravity = dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  GUIState gui;
  gui.setExitCommand("done", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=40;
    aString cmd[maxCommands];


    updatePDEparameters();  // update parameters such as ReynoldsNumber, MachNumber, ... to be consistent.

    buildCompressibleDialog(dialog,prefix,*this);

    gui.buildPopup(pdeParametersMenu);
    delete [] pdeParametersMenu;
 
    if( false && gi.graphicsIsOn() )
      dialog.openDialog(0);   // open the dialog here so we can reset the parameter values below

    updatePDEparameters();

    fillCompressibleDialogValues(dialog,*this );

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
    updatePDEparameters();
    fillCompressibleDialogValues(dialog,*this );

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
	printF("ERROR: setting nu for allSpeed! set machNumber and reynoldsNumber instead \n");
	Overture::abort("error");

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
				        gravity[0], gravity[1]));
	if( answer!="" )
	  sScanF(answer,"%e %e",& gravity[0],& gravity[1]);
	printF(" gravity=(%8.2e,%8.2e)\n", gravity[0], gravity[1]);
      }
      else
      {
	gi.inputString(answer,sPrintF(buff,"Enter gravity, 3 values, default=(%8.2e,%8.2e,%8.2e))",
				        gravity[0], gravity[1], gravity[2]));
	if( answer!="" )
	  sScanF(answer,"%e %e %e",& gravity[0],& gravity[1],& gravity[2]);
	printF(" gravity=(%8.2e,%8.2e,%8.2e)\n", gravity[0], gravity[1], gravity[2]);
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
    else if( answer=="defaultAlgorithm" ||
             answer=="densityFromGasLawAlgorithm" )
    {
      dbase.get<AlgorithmVariation>("algorithmVariation")=answer=="defaultAlgorithm" ? defaultAlgorithm : densityFromGasLawAlgorithm;
      dialog.getOptionMenu("algorithm").setCurrentChoice(answer);
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
    else if( len=answer.matches("linearize implicit method") ) 
    {
      int value;
      sScanF(answer(len,answer.length()-1),"%i",&value);
      dialog.setToggleState("linearize implicit method",value);      

      dbase.get<int >("linearizeImplicitMethod")=value;
      if( dbase.get<int >("linearizeImplicitMethod") )
	printF("linearize the implicit all speed algorithm.\n");
      else
	printF("do not linearize the implicit all speed algorithm.\n");
    }    
    else if( answer=="turn on second order artificial diffusion" )
    {
       dbase.get<bool >("useSecondOrderArtificialDiffusion")=true;
       dbase.get<real >("ad21")=1.; // .25;
       dbase.get<real >("ad22")=1.; // .25;
	printF("turn on second order artficial diffusion with ad21=%e, ad22=%e\n", dbase.get<real >("ad21"), dbase.get<real >("ad22"));
      dialog.setTextLabel("ad21,ad22",sPrintF(answer, "%g,%g", dbase.get<real >("ad21"), dbase.get<real >("ad22"))); 
    }
    else if( answer=="turn off second order artificial diffusion" )
    {
       dbase.get<bool >("useSecondOrderArtificialDiffusion")=false;
       dbase.get<real >("ad21")=.0;
       dbase.get<real >("ad22")=.0;
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
       printF("turn off fourth order artficial diffusion, ad41=%e, ad42=%e\n", dbase.get<real >("ad41"), dbase.get<real >("ad42"));
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
//     else if( answer=="nuRho" )
//     {
//       gi.inputString(answer,sPrintF(buff,"Enter nuRho (default value=%e)",nuRho));
//       if( answer!="" )
// 	sScanF(answer,"%e",&nuRho);
//       printF(" nuRho=%9.3e\n",nuRho);
//     }
//     else if( answer=="anu" )
//     {
//       gi.inputString(answer,sPrintF(buff,"Enter anu (default value=%e)",anu));
//       if( answer!="" )
// 	sScanF(answer,"%e",&anu);
//       printF(" anu=%9.3e\n",anu);
//     }
//     else if( answer=="pressure level" )
//     {
//       pressureLevel=0.;
//       gi.inputString(answer,sPrintF(buff,"Enter pressureLevel (default value=%e)",pressureLevel));
//       if( answer!="" )
// 	sScanF(answer,"%e",&pressureLevel);
//       printF(" pressureLevel=%9.3e\n",pressureLevel);
//     }
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
      sScanF(answer(7,answer.length()),"%e %e %e",& gravity[0],& gravity[1],
	     & gravity[2]);
      printF(" gravity=(%8.2e,%8.2e)\n", gravity[0], gravity[1]);
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
    else if( dialog.getTextValue(answer,"pressureLevel","%e", dbase.get<real >("pressureLevel")) ){}//
    else if( dialog.getTextValue(answer,"anu","%e", dbase.get<real >("anu")) ){}//
    else if( dialog.getTextValue(answer,"nuRho","%e", dbase.get<real >("nuRho")) ){}//
    else if( dialog.getTextValue(answer,"nu","%e", dbase.get<real >("nu")) ){}//
    else if( dialog.getTextValue(answer,"divergence damping","%e", dbase.get<real >("cdv")) ){}//
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
    // else if( dialog.getToggleValue(answer,"explicit method", dbase.get<int >("explicitMethod")) ){}//
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
    // --- these have neen turned off for now ---
//       else if( name=="testProblem" )
//       {
// 	aString test=nl.getString(answer);  
// 	if( test=="bomb" )
// 	{
// 	  parameters.dbase.get<Parameters::TestProblems >("testProblem")=Parameters::bomb;
// 	  parameters.dbase.get<bool >("twilightZoneFlow")=FALSE;
// 	}
// 	else if( test=="laminarFlame" )
// 	{
// 	  parameters.dbase.get<Parameters::TestProblems >("testProblem")=Parameters::laminarFlame;
// 	  parameters.dbase.get<bool >("twilightZoneFlow")=FALSE;
// 	}
// 	else if( test=="standard" )
// 	  parameters.dbase.get<Parameters::TestProblems >("testProblem")=Parameters::standard;
// 	else
// 	{
// 	  printF("unknown testProblem=%s\n",(const char*)test);
// 	}
//       }

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



int AsfParameters::
displayPdeParameters(FILE *file /* = stdout */ )
// =====================================================================================
// /Description:
//   Display PDE parameters
// =====================================================================================
{
  const char *offOn[2] = { "off","on" };


  fprintf(file,
	  "PDE parameters: equation is `call-speed Navier Stokes'.\n");
   
  fprintf(file,
	  "  number of components is %i\n"
	  "  Reynolds number=%e, Mach number=%e, \n"
	  "  mu=%e \n"
	  "  kThermal=%e \n"
	  "  Rg=%e (gas constant) \n"
	  "  gamma=%e \n"
	  "  2nd order artificial viscosity is %s, ad21=%f, ad22=%f\n"
	  "  4th order artificial viscosity is %s, ad41=%f, ad42=%f\n"
	  "  6th order artificial viscosity is %s, ad61=%f, ad62=%f\n",
	   dbase.get<int >("numberOfComponents"),
	   dbase.get<real >("reynoldsNumber"),
	   dbase.get<real >("machNumber"),
	   dbase.get<real >("mu"), dbase.get<real >("kThermal"), dbase.get<real >("Rg"), dbase.get<real >("gamma"),
	  offOn[ dbase.get<bool >("useSecondOrderArtificialDiffusion")],
	   dbase.get<real >("ad21"), dbase.get<real >("ad22"),
	  offOn[ dbase.get<bool >("useFourthOrderArtificialDiffusion")],
	   dbase.get<real >("ad41"), dbase.get<real >("ad42"),
	  offOn[ dbase.get<bool >("useSixthOrderArtificialDiffusion")],
	   dbase.get<real >("ad61"), dbase.get<real >("ad62"));

  const ArraySimpleFixed<real,3,1,1,1> & gravity = dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  if(  gravity[0]!=0. ||  gravity[1]!=0. ||  gravity[2]!=0. )
    fprintf(file," gravity is on, acceleration due to gravity = (%8.2e,%8.2e,%8.2e) \n",
	     gravity[0], gravity[1], gravity[2]);

   fprintf(file,
	  "  ad21,ad22 = %g,%g\n"
	   "  ad41,ad42 = %g,%g\n",
	    dbase.get<real >("ad21"), dbase.get<real >("ad22"), dbase.get<real >("ad41"), dbase.get<real >("ad42"));

  // The  dbase.get<DataBase >("modelParameters") will be displayed here:
  Parameters::displayPdeParameters(file);

  return 0;
}


//\begin{>>OverBlownInclude.tex}{\subsection{updatePDEparameters}} 
int AsfParameters::
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
 
  return 0;
}


//\begin{>>AsfParametersInclude.tex}{\subsection{updateShowFile}} 
int AsfParameters::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{AsfParametersInclude.tex}  
// =================================================================================================
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  showFileParams.push_back(ShowFileParameter("pde","compressibleNavierStokes"));

  showFileParams.push_back(ShowFileParameter("reynoldsNumber", dbase.get<real >("reynoldsNumber")));
  showFileParams.push_back(ShowFileParameter("machNumber", dbase.get<real >("machNumber")));
  showFileParams.push_back(ShowFileParameter("mu", dbase.get<real >("mu")));
  showFileParams.push_back(ShowFileParameter("kThermal", dbase.get<real >("kThermal")));
  showFileParams.push_back(ShowFileParameter("gamma", dbase.get<real >("gamma")));
  showFileParams.push_back(ShowFileParameter("Rg", dbase.get<real >("Rg")));

  showFileParams.push_back(ShowFileParameter("numberOfSpecies", dbase.get<int >("numberOfSpecies")));
 
  if(  dbase.get<int >("numberOfSpecies")>=0 )
  {
    aString reactionName = ( dbase.get<Parameters::ReactionTypeEnum >("reactionType")==noReactions ? "noReactions" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==oneStep ? "onestep" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==branching ? "branching" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==ignitionAndGrowth ? "ignitionAndGrowth" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==oneEquationMixtureFraction ? "oneEquationMixtureFraction" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==twoEquationMixtureFractionAndExtentOfReaction ? 
			    "twoEquationMixtureFractionAndExtentOfReaction" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==oneStepPress ? "oneStepPressureLaw" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==igDesensitization ? "igDesensitization" :
			     dbase.get<Parameters::ReactionTypeEnum >("reactionType")==chemkinReaction ? "chemkinReaction" : "unknown reactionType");

    showFileParams.push_back(ShowFileParameter("reactionType", reactionName));

  }

  // here are the new names for the velocity components:
  showFileParams.push_back(ShowFileParameter("v1Component", dbase.get<int>("uc")));
  showFileParams.push_back(ShowFileParameter("v2Component", dbase.get<int>("vc")));
  showFileParams.push_back(ShowFileParameter("v3Component", dbase.get<int>("wc")));

  // Now save parameters common to all solvers:
  Parameters::saveParametersToShowFile();    

  return 0;
}


//\begin{>>ParametersInclude.tex}{\subsubsection{getDerivedFunction}}
int AsfParameters::
getDerivedFunction( const aString & name, const realCompositeGridFunction & u,
                    realCompositeGridFunction & v, const int component, 
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

  int ok=0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    ok = getDerivedFunction(name,u[grid],v[grid],grid,component,parameters);
    if ( ok!=0 ) break;
  }
  return ok;
}

//\begin{>>CompositeGridFunctionInclude.tex}{\subsubsection{getDerivedFunction}}
int AsfParameters::
getDerivedFunction( const aString & name, const realMappedGridFunction & uIn,
                    realMappedGridFunction & vIn, const int grid,
                    const int component, Parameters & parameters)
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

  const int rc=parameters.dbase.get<int >("rc");
  const int tc=parameters.dbase.get<int >("tc");
  const int sc=parameters.dbase.get<int >("sc");

  #ifdef USE_PPP
   realSerialArray v; getLocalArrayWithGhostBoundaries(vIn,v);
   realSerialArray u; getLocalArrayWithGhostBoundaries(uIn,u);
  #else
    realSerialArray & v = vIn;
    const realSerialArray & u = uIn;
  #endif

  if( name=="pressure" )
  {
    // Note we assume p=rho*R*T ... so temperature means nothing so we can make sense of pressure
    v(all,all,all,component)=parameters.dbase.get<real >("Rg")*u(all,all,all, rc)*u(all,all,all, tc);
  }
  else if( name=="temperature-from-pressure" )
  {
    const int  pc= dbase.get<int >("tc");
    v(all,all,all,component)=u(all,all,all, pc)/(parameters.dbase.get<real >("Rg")*u(all,all,all, rc));

  }
  else
  {
    printf("getDerivedFunction:ERROR: unknown derived function! name=%s\n",(const char*)name);
    return 1;
  }
  return 0;
}

int AsfParameters::
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
  component.redim(numberToSet);
  int n=0;
  component(n)=dbase.get<int >("uc"); n++;
  component(n)=dbase.get<int >("vc"); n++;
  if( dbase.get<int >("wc")>=0 )
  { 
    component(n)=dbase.get<int >("wc"); n++;
  }
  component(n)=dbase.get<int >("tc"); 
  
  return 0;
}


//    Mixed-derivative BC for component i: 
//          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

int AsfParameters::
setDefaultDataForABoundaryCondition(const int & side,
				    const int & axis,
				    const int & grid,
				    CompositeGrid & cg)
// ============================================================================================
// /Description:
//    Assign the default values for the data required by the boundary conditions.
//\end{AsfParametersInclude.tex}  
// ============================================================================================
{
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  RealArray & bcData = dbase.get<RealArray>("bcData");

  Range all;
  const Range & Ru = dbase.get<Range >("Ru");
  const int & pc = dbase.get<int >("pc");
  
  switch (cg[grid].boundaryCondition()(side,axis)) 
  {
  case AsfParameters::subSonicInflow:
  case AsfParameters::noSlipWall:
    // what else should we do here ? 
    bcData(Ru,side,axis,grid)=0.;  
    break;
  case AsfParameters::subSonicOutflow:
  case AsfParameters::convectiveOutflow:
    mixedRHS(pc,side,axis,grid)=0.;
    mixedCoeff(pc,side,axis,grid)=1.;
    mixedNormalCoeff(pc,side,axis,grid)=1.;
    //  printF("*** AsfParameters::setDefaultDataForABC: set default pressure outflow BC to p+p.n=0 ****\n");
    break;
    
  }
  return 0;

}

bool
AsfParameters::isMixedBC(int bc) 
{ 
  return  //bc==Parameters::outflow ||       
    //    bc==Parameters::subSonicOutflow || 
    bc==AsfParameters::convectiveOutflow ||
    bc==AsfParameters::tractionFree;
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
int AsfParameters::
getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar,
		bool includeViscosity /* = true */ )
{
  int grid=ipar[0], side=ipar[1], axis=ipar[2];
  int form = ipar[3];
  real time =rpar[0];
  
  CompositeGrid & cg = *u.getCompositeGrid();
  assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
  assert( grid>=0 && grid<cg.numberOfComponentGrids());

//   const EquationOfStateEnum equationOfState = dbase.get<EquationOfStateEnum >("equationOfState");
//   if( equationOfState!=idealGasEOS )
//   {
//     printF("AsfParameters::getNormalForce:ERROR: equationOfState!=idealGasEOS\n");
//   }

  const int rc=dbase.get<int >("rc");
  const int uc=dbase.get<int >("uc");
  const int vc=dbase.get<int >("vc");
  const int wc=dbase.get<int >("wc");
  const int tc=dbase.get<int >("tc");

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
      
  Index Ib1,Ib2,Ib3;
  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);

  // *********** finish this for viscous stresses ******************
 
  const real Rg=dbase.get<real >("Rg");
  const real gamma = dbase.get<real >("gamma");
	
  if( mu != 0. )
    printF("ASFParameters::getNormalForce:ERROR: viscous stress terms missing on normal force\n");
	
  if( !includeViscosity )
  {
    mu=0.;  // turn off the viscous terms *FIX ME* -- We can optimize the code below if mu=0 ************
  }
  
  realArray p(Ib1,Ib2,Ib3);
  
  if( form==GridFunction::primitiveVariables )
  {
    p=Rg*ug(Ib1,Ib2,Ib3,rc)*ug(Ib1,Ib2,Ib3,tc);
  }
  else
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


  // This next section also appears in CnsParameters

  CompositeGridOperators & cgop = *u.getOperators();
  MappedGridOperators & op = cgop[grid];
	  
  realArray ux(Ib1,Ib2,Ib3,V), uy(Ib1,Ib2,Ib3,V), uz, div(Ib1,Ib2,Ib3);
  op.derivative(MappedGridOperators::xDerivative,ug,ux,Ib1,Ib2,Ib3,V);
  op.derivative(MappedGridOperators::yDerivative,ug,uy,Ib1,Ib2,Ib3,V);
  if( mg.numberOfDimensions()>=3 )
  {
    uz.redim(Ib1,Ib2,Ib3,V);
    op.derivative(MappedGridOperators::zDerivative,ug,uz,Ib1,Ib2,Ib3,V);
  }
	  
  const real lambda = -(2./3.)*mu; // Stokes hypothesis
  
  // **NOTE** we assume here that we have the velocity components 
  assert( form==GridFunction::primitiveVariables );


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

  return 0;
}
