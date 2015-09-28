#include "Cgmp.h"
// #include "Interface.h"
// #include "AdvanceOptions.h"
#include "MpParameters.h"

// ===================================================================================================================
/// \brief Project initial conditions for a multi-domain problem
/// 
/// \details For some problems the initial conditions need to be adjusted for moving grids, e.g. the
///    initial pressure for the INS may be coupled to the initial acceleration of moving modies. 
///
/// \gfIndex (input) : assign gf[gfIndex] at time gf[gfIndex].t 
// ===================================================================================================================
int Cgmp::
projectInitialConditions( std::vector<int> & gfIndex )
{
  const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  if( twilightZoneFlow )
  { // we do not project IC's for TZ flow
    return 0;
  }
  
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");

  printF("\n--MP-- project initial conditions\n");

  ForDomainOrdered(d)
  {
    printF("  domain %d: %s\n",d,(const char*)domainSolver[d]->getClassName());

  }
  
  // --------------------------------------------
  // --- PROJECT INITIAL CONDITIONS: INS + SM ---
  // --------------------------------------------


  // Get the solid interface values vs, sigmas

  // Assign fluid interface velocity:
  //      vf = vs 

  // ---- ITERATE USING THE TRADITIONAL PARTITIONED SCHEME ----
  for( int correction=0; correction<maxNumberOfCorrections; correction++ )
  {

    // Get the fluid interface traction: sigmaf 

    // Set the solid interface traction: 
    //          sigmas = sigmaf 


    // Get the solid interface acceleration as = vs_t 

    // Set fluid acceleration on interface

    // Solve the fluid pressure equation
    //         Delta p = f 
    //         p.n = n.( -vs_t + ... )

  } // end for correction

  
  OV_ABORT("finish me");
  return 0;
}


//   const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
//   if( !twilightZoneFlow )
//   {
//     // -- For moving grid problems we iterate on the initial conditions since the 
//     //    body forces depend on the pressure and the pressure depends on the forces.
// 
//     // useMovingGridSubIterations : use multiple sub-iterations per time-step for moving grid problems with light bodies
//     const bool & useMovingGridSubIterations = parameters.dbase.get<bool>("useMovingGridSubIterations");
// 
//     int numberOfCorrections=1;
//     if( movingGridProblem() && useMovingGridSubIterations  )
//       numberOfCorrections= parameters.dbase.get<int>("numberOfPCcorrections"); 
// 
//     printF("--INS--::projectInitialConditionsForMovingGrids: useMovingGridSubIterations=%i numberOfCorrections=%i\n",(int)useMovingGridSubIterations,
// 	   numberOfCorrections);
// 
//     for( int correction=0; correction<numberOfCorrections; correction++ )
//     {
//       // define initial forces on moving bodies -- we really should iterate here since the 
//       // forces depend on the pressure and the pressure depends on the forces.
//       if( movingGridProblem() && gf[gfIndex].t==0. )
// 	correctMovingGrids( gf[gfIndex].t, gf[gfIndex].t,gf[gfIndex],gf[gfIndex] ); 
//       
//       // -- compute any body forcing since the pressure may depend on this ---
//       const real tForce = gf[gfIndex].t; // evaluate the body force at this time
//       computeBodyForcing( gf[gfIndex], tForce );
// 
//       if( !parameters.dbase.get<bool >("projectInitialConditions") ) // TEMP fix for Joel's bug
// 	updateDivergenceDamping( gf[gfIndex].cg,true );
//     
//       // Evaluate the initial pressure field:
//       if( correction==0 )
// 	printF("--INS--::initializeSolution:Solve for the initial pressure field, dt=%9.3e (correction=%i) \n",
// 	       parameters.dbase.get<real >("dt"),correction);
//       solveForTimeIndependentVariables( gf[gfIndex] );     
// 
//       bool isConverged = getMovingGridCorrectionHasConverged();
//       if( movingGridProblem() && useMovingGridSubIterations )
//       {
// 	if( true || debug() & 2 )
// 	{
// 	  if( correction==0 ) isConverged=false;  // Make at least 2 correction steps *wdh* 2015/06/07
// 
// 	  real delta = getMovingGridMaximumRelativeCorrection();
// 	  printF("--INS--:projectInitialConditionsForMovingGrids: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
// 		 delta,correction,(int)isConverged);
// 	}
//       }
//       
// 
//       if( isConverged )
// 	break;
//     }
//     
//   }
// 
//   return 0;
// }
