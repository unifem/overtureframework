#include "Cgad.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "AdamsPCData.h"
#include "AdvanceOptions.h"


#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

// ===================================================================================================================
/// \brief Advance the THIN FILM EQUATIONS one time step for the BDF scheme.
///
///   **THIS VERSION IS A PLACEHOLDER FOR THE REAL EQUATIONS ***
/// 
/// \details This routine is called by the implicitTimeStep routine which is in turn called by
/// the BDF takeTimeStep routine which handles details of moving and adaptive grids.
/// 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int Cgad::
thinFilmSolver(  real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
  
  if( t0<5*dt0 )
    printF("--AD-- thinFilmSolver WARNING : FINISH ME, t=%9.3e\n",t0);



  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
  const int & orderOfBDF = parameters.dbase.get<int>("orderOfBDF");

  if( true || debug() & 4 )
    printP("--DS-- implicitTimeStep t0=%e, dt0=%e, correction=%i BDF%i++++\n",t0,dt0,correction,orderOfBDF );
  if( debug() & 2 )
  {
    fprintf(debugFile,"--DS-- implicitTimeStep (start): t0=%e, dt0=%e, correction=%i*** \n",t0,dt0,correction);
  }

  assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

  assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );
  AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
  
  real & dtb=adamsData.dtb;
  int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
  int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
  int &ndt0=adamsData.ndt0;
  real *dtp = adamsData.dtp;

  const int & predictorOrder = parameters.dbase.get<int>("predictorOrder");
  
  int numberOfCorrections=parameters.dbase.get<int>("numberOfPCcorrections"); 

  // If we check a convergence tolerance when correcting (e.g. for moving grids) then this is
  // the minimum number of corrector steps we must take:
  const int minimumNumberOfPCcorrections = parameters.dbase.get<int>("minimumNumberOfPCcorrections"); 
  
  // For moving grids we keep gf[mab0], gf[mab1] and gf[mab2]
  // For non-moving grids we keep gf[mab0], gf[mab1] and we set mab2==mab1

  const int numberOfGridFunctions =  orderOfBDF+1; // movingGridProblem() ? 3 : 2; 

  int & mCur = mab0;
  const int mNew   = (mCur + 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t+dt)
  const int mOld   = (mCur - 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-dt)
  const int mOlder = (mCur - 2 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-2*dt)
  const int mMinus3= (mCur - 3 + numberOfGridFunctions*2) % numberOfGridFunctions; // holds u(t-3*dt)

  Parameters::ImplicitOption & implicitOption = parameters.dbase.get<Parameters::ImplicitOption >("implicitOption");
  
  implicitOption=Parameters::doNotComputeImplicitTerms; // no need to compute during initialization
  // parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")=Parameters::crankNicolson;
  int iparam[10];
  real rparam[10];

  RealCompositeGridFunction & uti = gf[mOld].u; // NOT USED *CHECK ME*

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2]; 
  Range N = parameters.dbase.get<Range >("Rt");   // time dependent variables
  RealArray error(numberOfComponents()+3);
  
  // BDF: 
  // Matrix is:  I - dt*implicitFactor*A 
  real & implicitFactor = parameters.dbase.get<real >("implicitFactor");
  if( orderOfBDF==1 )
    implicitFactor=1.;
  else if( orderOfBDF==2 )
    implicitFactor=2./3.;
  else if( orderOfBDF==3 )
    implicitFactor=6./11.;
  else if( orderOfBDF==4 )
    implicitFactor=12./25.;
  else
  {
    OV_ABORT("ERROR: unexpected orderOfBDF");
  }
  
  

  if( correction>1  && debug() & 2 )
    printP("implicitTimeStep: correction=%i\n",correction);
      
  // BDF:
  //    [ I -       dt*A ] u(t+dt) = u(t)                        : BDF1 (backward-Euler)
  //    [ I - (2/3)*dt*A ] u(t+dt) = (4/3)*u(t) - (1/3)*u(t-dt)  : BDF2 


  // We only need to compute the "explicit" part of the implicit terms once for correction==0: 
  // These values are stored in utImplicit 
  implicitOption =correction==0 ? Parameters::computeImplicitTermsSeparately : Parameters::doNotComputeImplicitTerms;

  // Optionally refactor the matrix : if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
  // (We need to do this after the grids have moved but before dudt is evaluated (for nonlinear problems)
  if( correction==0 && (parameters.dbase.get<int >("initializeImplicitTimeStepping") || parameters.dbase.get<int >("globalStepNumber")>0) )
    formMatrixForImplicitSolve(dt0,gf[mNew], gf[mCur] );

  const int maba = correction==0 ? mCur : mNew;
  const int naba = correction==0 ? nab0 : nab1;

  // -- evaluate any body forcing (this is saved in realCompositeGridFunction bodyForce found in the data-base) ---
  const real tForce = t0+dt0; // evaluate the body force at this time  ***CHECK ME**
  computeBodyForcing( gf[maba], tForce );    // ***CHECK ME**

  // addArtificialDissipation(gf[maba].u,dt0);	// add "implicit" dissipation to u 

  if( debug() & 4 )
  {
    determineErrors( gf[mCur],sPrintF("--AD-- errors in mCur=%i at t=%9.3e\n",mCur,gf[mCur].t));
    if( orderOfBDF>=2 )
      determineErrors( gf[mOld],sPrintF("--AD-- errors in mOld=%i at t=%9.3e\n",mOld,gf[mOld].t));
    if( orderOfBDF>=3 )
      determineErrors( gf[mOlder],sPrintF("--AD-- errors in mOlder at t=%9.3e\n",gf[mOlder].t));
  }
  
  //  -----------------------------------
  //  --- Assign the right-hand-side  ---
  //  -----------------------------------
  for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
  {
    realMappedGridFunction & uNew = gf[mNew].u[grid];  
    realMappedGridFunction & rhs = uNew;
    realMappedGridFunction & uCur = gf[mCur].u[grid];  
    OV_GET_SERIAL_ARRAY(real,rhs,rhsLocal);     
    OV_GET_SERIAL_ARRAY(real,gf[mCur].u[grid],u0Local);
    OV_GET_SERIAL_ARRAY(real,gf[mOld].u[grid],u1Local);
    OV_GET_SERIAL_ARRAY(real,gf[mOlder].u[grid],u2Local);
    OV_GET_SERIAL_ARRAY(real,gf[mMinus3].u[grid],u3Local);
    
    getIndex(gf[mNew].cg[grid].extendedIndexRange(),I1,I2,I3);
    bool ok = ParallelUtility::getLocalArrayBounds(gf[mCur].u[grid],u0Local,I1,I2,I3);
    if( !ok ) continue;

    // --- compute external forcing ---
    // rhs = forcing 
    const real tNew = t0+dt0;
    rparam[0]=tNew;
    rparam[1]=tNew; // tforce
    rparam[2]=tNew; // tImplicit
    iparam[0]=grid;
    iparam[1]=gf[mNew].cg.refinementLevelNumber(grid);
    iparam[2]=numberOfStepsTaken;   
    rhsLocal=0.;
    addForcing(rhs,uNew,iparam,rparam,uti[grid],&gf[mNew].getGridVelocity(grid)); // grid comes from uNew
    if( debug() & 16  )
    {
      display(rhs,sPrintF("--AD-ITS-- rhs after add forcing, grid=%i tNew=%9.3e",grid,tNew),debugFile,"%9.2e ");
    }

    // -- evaluate RHS to BDF scheme ---
    if( orderOfBDF==1 )
      rhsLocal(I1,I2,I3,N) = u0Local(I1,I2,I3,N) + dt*rhsLocal(I1,I2,I3,N);
    else if( orderOfBDF==2 )    
      rhsLocal(I1,I2,I3,N) = (4./3.)*u0Local(I1,I2,I3,N) -(1./3.)*u1Local(I1,I2,I3,N) + (dt*2./3.)*rhsLocal(I1,I2,I3,N);
    else if( orderOfBDF==3 )    
      rhsLocal(I1,I2,I3,N) = (18./11.)*u0Local(I1,I2,I3,N) -(9./11.)*u1Local(I1,I2,I3,N)
                        +(2./11.)*u2Local(I1,I2,I3,N) + (dt*6./11.)*rhsLocal(I1,I2,I3,N);
    else if( orderOfBDF==4 )    
      rhsLocal(I1,I2,I3,N) = (48./25.)*u0Local(I1,I2,I3,N) -(36./25.)*u1Local(I1,I2,I3,N)
                       +(16./25.)*u2Local(I1,I2,I3,N) -( 3./25.)*u3Local(I1,I2,I3,N) + (dt*12./25.)*rhsLocal(I1,I2,I3,N);
    else
    {
      OV_ABORT("orderOfBDF>4 not implemented");
    }
    
  }

  if( correction==0 )
  {
    // printF(" +++ ims: gf[mNew].t=%9.3e --> change to t0+dt0=%9.3e +++\n",gf[mNew].t,t0+dt0);
    gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
  }
      

  // *** assign boundary conditions for the implicit method 
  DomainSolver::applyBoundaryConditionsForImplicitTimeStepping( gf[mNew] ); // ***** gf[mNew].gridVelocity must be correct here
    
  if( Parameters::checkForFloatingPointErrors!=0 )
    checkSolution(gf[mNew].u,"--AD-- implicitTimeStep: after applyBCIMP",true);


  if( debug() & 4 )
  {
    aString label = sPrintF("--AD-- implicitTimeStep: RHS Before implicitSolve t=%e, ,correction=%i\n",gf[mNew].t,correction);
    if( twilightZoneFlow() )
    {
      gf[mNew].u.display(label,debugFile,"%8.5f ");
    }
    label = sPrintF("--AD-- implicitTimeStep: Errors in rhs gf before implicitSolve t=%e, correction=%i\n",gf[mNew].t,correction);
    determineErrors( gf[mNew],label );
  }

  // **** fix this *** we could refactor for each correction here !
//       if( mst>1 || correction>0 )
//       {
//         // Optionally refactor the matrix : if parameters.dbase.get<int >("globalStepNumber") % refactorFrequency == 0 
// 	formMatrixForImplicitSolve(dt0,gf[mNew], gf[mCur] );
//       }
      

  // ------------------------------------
  // --- Solve the implicit equations ---
  // ------------------------------------
 
  implicitSolve( dt0,gf[mNew], gf[mCur] );  // gf[mNew]=RHS  gf[mCur]=used for initial guess and linearization

  if( Parameters::checkForFloatingPointErrors!=0 )
    checkSolution(gf[mNew].u,"--AD-- implicitTimeStep: after implicitSolve",true);

  if( debug() & 4 )
  {
    if( twilightZoneFlow() )
    {
      gf[mNew].u.display(sPrintF("--AD-- implicitTimeStep: gf[mNew].u after implicitSolve but BEFORE BC's (t=%8.2e), correction=%i",
				 gf[mNew].t,correction),debugFile,"%8.5f ");
    }
    aString label = sPrintF("--AD-- implicitTimeStep: after implicitSolve but BEFORE BC's, t=%e, correction=%i\n",gf[mNew].t,correction);
    determineErrors( gf[mNew],label );
  }

  // apply explicit BC's  --- > really only have to apply to implicit grids I think?
  DomainSolver::applyBoundaryConditions(gf[mNew]);   // ***** gf[mNew].gridVelocity must be correct here!


  updateStateVariables( gf[mNew],1 );  

  if( debug() & 4 )
  {
    if( twilightZoneFlow() )
    {
      gf[mNew].u.display(sPrintF("--AD-- implicitTimeStep: gf[mNew].u after implicitSolve and BC's (t=%8.2e), correction=%i",
				 gf[mNew].t,correction),debugFile,"%8.5f ");
    }
    aString label = sPrintF("--AD-- implicitTimeStep: after implicitSolve and BC's, t=%e, correction=%i\n",gf[mNew].t,correction);
    determineErrors( gf[mNew],label );
  }
  

  return 0;
}



