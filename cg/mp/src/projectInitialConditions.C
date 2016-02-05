// This file automatically generated from projectInitialConditions.bC with bpp.
#include "Cgmp.h"
// #include "Interface.h"
#include "AdvanceOptions.h"
#include "MpParameters.h"

#include "Cgins.h"
#include "Cgsm.h"

//========================================================================================================
// Macro: Begin loop over the interfaces
//========================================================================================================

//========================================================================================================
// Macro: End loop over the interfaces
//========================================================================================================


// ===================================================================================================================
/// \brief Project initial conditions for a multi-domain problem
/// 
/// \details For some problems the initial conditions need to be adjusted for moving grids, e.g. the
///    initial pressure for the INS may be coupled to the initial acceleration of moving modies. 
///
/// \gfIndex (input) : assign gf[gfIndex] at time gf[gfIndex].t 
// ===================================================================================================================
int Cgmp::
projectInitialConditions( real t, real dt, std::vector<int> & gfIndex )
{
    if( !gridHasMaterialInterfaces ) return 0;

    const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    if( twilightZoneFlow )
    { // we do not project IC's for TZ flow
        return 0;
    }
    
    FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
    const int numberOfDomains=domainSolver.size();
    std::vector<AdvanceOptions> advanceOptions(numberOfDomains); 

    printF("\n--MP-PIC-- project initial conditions\n");

    ForDomainOrdered(d)
    {
        printF("  domain %d: %s\n",d,(const char*)domainSolver[d]->getClassName());

    }
    
    InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
    if( interfaceList.size()>0 )
    {
        printF("--MP-PIC--  number of interfaces =%i\n",interfaceList.size());
    }
      
  // --------------------------------------------
  // --- PROJECT INITIAL CONDITIONS: INS + SM ---
  // --------------------------------------------


  // Get the solid interface values vs, sigmas

  // Assign fluid interface velocity:
  //      vf = vs 

  // ---- ITERATE USING THE TRADITIONAL PARTITIONED SCHEME ----
    int maxNumberOfCorrections=5; // fix me***************************************
    for( int correction=0; correction<maxNumberOfCorrections; correction++ )
    {
        bool isConverged = true; 
    // --- Begin loop over interfaces ---
        for( int inter=0; inter < interfaceList.size(); inter++ )
        {
            InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 
      // There may be multiple grid faces that lie on the interface:     
            for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
            {
                GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
                GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
                const int d1=gridDescriptor1.domain, 
                                    grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
                const int d2=gridDescriptor2.domain, 
                                    grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;
                assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );
        {
            
            printF("--MP-PIC-- correction=%i, interface=%i face=%i %s:(d1,grid1,side1,dir1)=(%i,%i,%i,%i)"
                            " %s:(d2,grid2,side2,dir2)=(%i,%i,%i,%i)\n",
                          correction,inter,face,
           	     (const char*)domainSolver[d1]->getClassName(),d1,grid1,side1,dir1,
                          (const char*)domainSolver[d2]->getClassName(),d2,grid2,side2,dir2);
            
            int domainFluid=-1, domainSolid=-1;  // fluid-domain and solid domain
            if( domainSolver[d1]->getClassName()=="Cgins" ) domainFluid=d1;
            if( domainSolver[d2]->getClassName()=="Cgins" ) domainFluid=d2;
            if( domainSolver[d1]->getClassName()=="Cgsm" ) domainSolid=d1;
            if( domainSolver[d2]->getClassName()=="Cgsm" ) domainSolid=d2;
            assert( domainFluid>=0 && domainSolid>=0 );


            Cgins & cgins = (Cgins&)(*domainSolver[domainFluid]);
            Cgsm & cgsm = (Cgsm&)(*domainSolver[domainSolid]);
            
      // SET RHS for solid BC's 
            assignInterfaceRightHandSide( domainSolid, t, dt, correction, gfIndex ); // NOTE: THIS WILL SET ALL INTERFACES

      // apply solid BC's 
            advanceOptions[domainSolid].takeTimeStepOption=AdvanceOptions::applyBoundaryConditionsOnly;
            domainSolver[domainSolid]->takeTimeStep( t,dt,correction,advanceOptions[domainSolid] );

      // Assign RHS for fluid BC's 
            assignInterfaceRightHandSide( domainFluid, t, dt, correction, gfIndex );

      // solve for the fluid pressure
            cgins.solveForTimeIndependentVariables( cgins.gf[gfIndex[domainFluid]] );

            isConverged = isConverged && cgins.getMovingGridCorrectionHasConverged();
            const bool & useMovingGridSubIterations = cgins.parameters.dbase.get<bool>("useMovingGridSubIterations");
            if( true || (cgins.movingGridProblem() && useMovingGridSubIterations) )
            {
      	if( true || debug() & 2 )
      	{
        	  if( correction==0 ) isConverged=false;  // Make at least 2 correction steps *wdh* 2015/06/07

        	  real delta =  cgins.getMovingGridMaximumRelativeCorrection();
        	  printF("--MP-PIC--: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
             		 delta,correction,(int)isConverged);
      	}
            }
            


/* ------
      // Get the fluid interface traction: sigmaf 
            getInterfaceData( domainFluid,t,dt,correct,gfIndex );



      // Get the solid interface values (velocity and acceleration)
            getInterfaceData( domainSolid,t,dt,correct,gfIndex );


      // apply fluid BC's 
            advanceOptions[domainFluid].takeTimeStepOption=AdvanceOptions::applyBoundaryConditionsOnly;
            domainSolver[domainFluid]->takeTimeStep( t,dt,correct,advanceOptions[domainFluid] );

      // Solve the fluid pressure equation
      // advanceOptions[domainFluid].takeTimeStepOption=AdvanceOptions::takeStepButDoNotApplyBoundaryConditions;
      // domainSolver[domainFluid]->takeTimeStep( t,dt,correct,advanceOptions[domainFluid] );

    ------ */


      // Set the solid interface traction: 
      //          sigmas = sigmaf 


      // Get the solid interface acceleration as = vs_t 

      // Set fluid acceleration on interface

      // Solve the fluid pressure equation
      //         Delta p = f 
      //         p.n = n.( -vs_t + ... )


        }
            } // end for face 
        }  // end for inter
        
        if( isConverged )
            break;

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
