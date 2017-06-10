#include "MxParameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "ProbeInfo.h"

// ******* FINISH ME -- This is not used yet May, 2017 *wdh* *********

int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands);


//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in MxParameters}} 
//\no function header:
//
// /int numberOfDimensions: number of spacial dimensions.
//\end{ParametersInclude.tex}
//===================================================================================


// ==================================================================================
/// \brief Constructor.
//===================================================================================
MxParameters::
MxParameters(const int & numberOfDimensions0) : Parameters(numberOfDimensions0)
{
  int & numberOfComponents = dbase.get<int>("numberOfComponents"); 
  numberOfComponents=1;  // default
  
 //  if (!dbase.has_key("kappa")) dbase.put<std::vector<real> >("kappa");
 //  if (!dbase.has_key("a")) dbase.put<std::vector<real> >("a");
 //  if (!dbase.has_key("b")) dbase.put<std::vector<real> >("b");
 //  if (!dbase.has_key("c")) dbase.put<std::vector<real> >("c");


 //  // the thermalConductivity is used in boundary conditions at domain interfaces: 
 //  if (!dbase.has_key("thermalConductivity")) dbase.put<real>("thermalConductivity",-1.);

 //  if (!dbase.has_key("variableDiffusivity")) dbase.put<bool>("variableDiffusivity",false);
 //  if (!dbase.has_key("diffusivityIsTimeDependent")) dbase.put<bool>("diffusivityIsTimeDependent",false);
 //  if (!dbase.has_key("kappaVar"))
 //  {  // save variable diffusion coefficients here:
 //     dbase.put<realCompositeGridFunction*>("kappaVar");
 //     dbase.get<realCompositeGridFunction*>("kappaVar")=NULL;
 //  }

 //  if (!dbase.has_key("variableAdvection")) dbase.put<bool>("variableAdvection",false);
 //  if (!dbase.has_key("advectionIsTimeDependent")) dbase.put<bool>("advectionIsTimeDependent",false);
 //  if (!dbase.has_key("advectVar"))
 //  {  // save variable advection coefficients here:
 //     dbase.put<realCompositeGridFunction*>("advectVar");
 //     dbase.get<realCompositeGridFunction*>("advectVar")=NULL;
 //  }
 //  registerBC((int)mixedBoundaryCondition,"mixedBoundaryCondition");

 //  if( !dbase.has_key("implicitAdvection") ) dbase.put<bool>("implicitAdvection")=false;

 //  if( !dbase.has_key("inverseCapillaryNumber") )    dbase.put<real>("inverseCapillaryNumber")=1.e-6;
 //  if( !dbase.has_key("scaledStokesNumber") )        dbase.put<real>("scaledStokesNumber")=0.;
 //  if( !dbase.has_key("thinFilmBoundaryThickness") ) dbase.put<real>("thinFilmBoundaryThickness")=1.5;
 //  if( !dbase.has_key("thinFilmLidThickness") )      dbase.put<real>("thinFilmLidThickness")=0.;

 // if( !dbase.has_key("manufacturedTearFilm") ) dbase.put<bool >("manufacturedTearFilm")=false;

  // initialize the items that we time: 
  // initializeTimings();
}

// ==================================================================================
/// \brief Destructor. 
//===================================================================================
MxParameters::
~MxParameters()
{

}

int MxParameters::
setParameters(const int & numberOfDimensions0 /* =2 */,const aString & reactionName )
// ==================================================================================================
//  /reactionName (input) : optional name of a reaction or a reaction 
//     file that defines the chemical reactions, such as a Chemkin binary file. 
// ==================================================================================================
{
  int & numberOfComponents     = dbase.get<int>("numberOfComponents");

  // printF("MxParameters::setParameters: number of components=%i\n",numberOfComponents);

  // int & numberOfExtraVariables = dbase.get<int>("numberOfExtraVariables");
  // int & tc = dbase.get<int >("tc");
  // aString *& componentName = dbase.get<aString* >("componentName");
   
  // dbase.get<int >("numberOfDimensions")=numberOfDimensions0;
  // //  dbase.get<Parameters::PDE >("pde")=pde0;
  // dbase.get<int >("rc")= dbase.get<int >("uc")= dbase.get<int >("vc")= dbase.get<int >("wc")= 
  //   dbase.get<int >("pc")= tc= dbase.get<int >("sc")= dbase.get<int >("kc")= 
  //   dbase.get<int >("epsc")= dbase.get<int >("sec")=-1;
  
  // dbase.get<bool >("computeReactions")=false;
  // dbase.get<Reactions* >("reactions")=NULL;
  // dbase.get<int >("numberOfSpecies")=0;
  
  // const real & S  = dbase.get<real>("inverseCapillaryNumber");
  // const real & G  = dbase.get<real>("scaledStokesNumber");
  // const real & h0 = dbase.get<real>("thinFilmBoundaryThickness");
  // const real & he = dbase.get<real>("thinFilmLidThickness");


  // int s, i;
  // //...set component index'es, showVariables, etc. that are equation-specific

  // printF("--AD-- MxParameters::setParameters: pdeName=[%s]\n",(const char*)pdeName);
  // if( pdeName=="thinFilmEquations" )
  // {
  //   // --- thin film equations ---
  //   //  Unknowns are
  //   //        h : height
  //   //        q : flux
  //   //        s : flourenscence concentration (optional)
  //   dbase.get<Range >("Rt")= numberOfComponents;  // "time dependent" components 
  //   delete  componentName;
  //   componentName= new aString [numberOfComponents];
  //   tc=0;
  //   componentName[tc  ]="h";
  //   if( numberOfComponents>=2 )
  //     componentName[tc+1]="p";
  //   if( numberOfComponents>=3 )
  //     componentName[tc+2]="s";

  //   addShowVariable( "h",tc );
  //   if( numberOfComponents>=2 )
  //     addShowVariable( "p",tc+1 );
  //   if( numberOfComponents>=3 )
  //     addShowVariable( "s",tc+2 );
  // }
  // else
  // {
  //   // --- advection diffusion ---
  //   dbase.get<Range >("Rt")= numberOfComponents;

  //   if( numberOfExtraVariables>0 )
  //     numberOfComponents+=numberOfExtraVariables;

  //   tc=0;    
  //   dbase.get<Range >("Rt")=Range( tc,tc);              // time dependent components

  //   addShowVariable( "T",tc );


  //   delete  componentName;
  //   componentName= new aString [numberOfComponents];

  //   if( tc>=0 )  componentName[tc]="T";

  //   if( numberOfExtraVariables>0 )
  //   {
  //     aString buff;
  //     for( int e=0; e< numberOfExtraVariables; e++ )
  //     {
  // 	int n= numberOfComponents- numberOfExtraVariables+e;
  // 	componentName[n]=sPrintF(buff,"Var%i",e);
  // 	addShowVariable(  componentName[n],n );
  //     }

  //   }
  // }
  
  // std::vector<real> & kappa = dbase.get<std::vector<real> >("kappa");
  // std::vector<real> & a = dbase.get<std::vector<real> >("a");
  // std::vector<real> & b = dbase.get<std::vector<real> >("b");
  // std::vector<real> & c = dbase.get<std::vector<real> >("c");

  // kappa.resize(numberOfComponents,.1);
  // a.resize(numberOfComponents,0.);
  // b.resize(numberOfComponents,0.);
  // c.resize(numberOfComponents,0.);

  // dbase.get<int >("stencilWidthForExposedPoints")=3;
  // dbase.get<int >("extrapolateInterpolationNeighbours")=false;

  // dbase.get<RealArray >("initialConditions").redim( numberOfComponents);  
  // dbase.get<RealArray >("initialConditions")=defaultValue;
  
  // dbase.get<RealArray >("checkFileCutoff").redim( numberOfComponents+1);  // cutoff's for errors in checkfile
  // dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  
  return 0;
}








