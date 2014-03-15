// -----------------------------------------------------------------------------------------------------------
// This file contains the functions:
// 
// cycleZero()
// setupAdvance()
// advance(real &tFinal) : advance the solution to time tFinal.
// finishAdvance()
// 
// -----------------------------------------------------------------------------------------------------------

#include "Cgmp.h"
#include "Ogshow.h"

int 
Cgmp::
cycleZero()
{
  // parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=Parameters::forwardEuler;  // ***** do this for now ***
  setupAdvance();
}

// ===================================================================================================================
/// \brief perform tasks needed prior to an actual advance (file io stuff mostly), returns nonzero if the computation is finished.
///
// ===================================================================================================================
int 
Cgmp::
setupAdvance()
{
  // most of this code is taken from cg/mp/src/solve.C
  //  real dtNew= getTimeStep( gf[current] ); //       ===Choose time step====
  
  ForDomain(d)
  {
    domainSolver[d]->updateForNewTimeStep(domainSolver[d]->gf[0],dt);
  }
  
  return 0;
}

// ===================================================================================================================
/// \brief Advance the solution to time tFinal.
/// \details This routine advances the solution a number of sub-steps.
/// \param tFinal (input) : integrate in time to this value.
///
// ===================================================================================================================
int Cgmp::
advance(real &tFinal)
{  
  real cpu0=getCPU();
  const Parameters::TimeSteppingMethod & timeSteppingMethod=
    parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  
  const int numberOfDomains = domainSolver.size(); 
  int numberOfSubSteps=parameters.dbase.get<int>("numberOfSubSteps");
  real t = gf[current].t;
  //  parameters.dbase.get<real >("dt")=dt;


  if( timeSteppingMethod==Parameters::forwardEuler ||
      timeSteppingMethod==Parameters::implicit ||
      timeSteppingMethod==Parameters::Parameters::adamsPredictorCorrector2 )
  {
    // here is the generic multi-domain advance routine    
    multiDomainAdvance( t, tFinal );
    
  }
//   else if( timeSteppingMethod==Parameters::forwardEuler )
//   {
//     // ************* forward Euler ******************
//     for( int i=0; i<numberOfSubSteps; i++ )
//     {
//       const int next = (current+1) %2;
//       ForDomainOrdered(d)
//       {
// 	if  ( false ) 
// 	{
// 	  GridFunction & gf0 = domainSolver[d]->gf[current];
// 	  GridFunction & gf1 = domainSolver[d]->gf[next];
// 	  realCompositeGridFunction & fn0 = domainSolver[d]->fn[0];
		      
// 	  domainSolver[d]->eulerStep(t,t,t+dt,dt,gf0,gf0,gf1,fn0,fn0,i  ,numberOfSubSteps);
// 	}
// 	else 
// 	{
// 	  // new way:
// 	  // domainSolver[d]->takeOneStep( t,dt,i,numberOfSubSteps );
//           int numberOfSubSteps0=1;
// 	  domainSolver[d]->takeOneStep( t,dt,i,numberOfSubSteps0 ); // ********************** numberOfSubSteps must be 1 for now ******
// 	}
		  
// 	domainSolver[d]->numberOfStepsTaken++; 
//       }

//       // now apply interface boundary conditions 
//       // gfIndex[domain] : indicates which solution to use in each domain 
//       std::vector<int> gfIndex(numberOfDomains,next); 
//       assignInterfaceBoundaryConditions(gfIndex, dt );

//       t+=dt; 	numberOfStepsTaken++; 
//       current=next;
//       ForDomainOrdered(d)
//         domainSolver[d]->current=current;
	
//       ForDomainOrdered(d)
// 	domainSolver[d]->output( domainSolver[d]->gf[current],numberOfStepsTaken );
//     }
//   }
//   else if( timeSteppingMethod==Parameters::midPoint )
//   {
//     for( int i=0; i<numberOfSubSteps; i++ )
//     {
//       const int next = (current+1) %2;
//       ForDomainOrdered(d)
//       {
// 	GridFunction & gf0 = domainSolver[d]->gf[current];
// 	GridFunction & gf1 = domainSolver[d]->gf[next];
// 	realCompositeGridFunction & fn0 = domainSolver[d]->fn[0];
// 	domainSolver[d]->eulerStep(t,t,t+dt,dt,gf0,gf0,gf1,fn0,fn0,i  ,numberOfSubSteps);
//       }

//       // now apply interface boundary conditions 
//       // gfIndex[domain] : indicates which solution to use in each domain 
//       std::vector<int> gfIndex(numberOfDomains,next); 
//       assignInterfaceBoundaryConditions(gfIndex, dt );
	  
//       // now try a corrector step
//       ForDomainOrdered(d)
//       {
// 	GridFunction & gf0 = domainSolver[d]->gf[current];
// 	GridFunction & gf2 = domainSolver[d]->gf[next];
// 	GridFunction gf1;
// 	gf1.updateToMatchGrid(gf0.cg);
// 	gf1.u.updateToMatchGridFunction(gf0.u);
// 	for ( int grid=0; grid<gf0.cg.numberOfComponentGrids(); grid++ )
// 	{
// 	  gf1.u[grid] = .5*(gf0.u[grid] + gf2.u[grid]);
// 	}
// 	gf1.t = gf0.t + .5*dt;
// 	realCompositeGridFunction & fn0 = domainSolver[d]->fn[0];
// 	domainSolver[d]->eulerStep(t,t,t+dt,dt,gf0,gf1,gf2,fn0,fn0,i  ,numberOfSubSteps);
// 	domainSolver[d]->numberOfStepsTaken++; 
//       }

//       // now apply interface boundary conditions 
//       // gfIndex[domain] : indicates which solution to use in each domain 
//       //	  std::vector<int> gfIndex(numberOfDomains,next); 
//       assignInterfaceBoundaryConditions(gfIndex, dt );

//       t+=dt; 	numberOfStepsTaken++; 
//       current=next;
//       ForDomainOrdered(d)
// 	domainSolver[d]->current=current;
	
//       ForDomainOrdered(d)
// 	domainSolver[d]->output( domainSolver[d]->gf[current],numberOfStepsTaken );
	  
//     }
//   }
  else
  {
    printF("Cgmp:advance:ERROR: unexpected timeSteppingMethod=%i\n",(int)timeSteppingMethod);
    Overture::abort("error");
  }

  gf[current].t = t;
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAdvance"))+=getCPU()-cpu0;
  
  return 0;
}

int 
Cgmp::
finishAdvance()
{
  return 0;
}

