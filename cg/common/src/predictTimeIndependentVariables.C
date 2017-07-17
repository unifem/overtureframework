#include "DomainSolver.h"
#include "Oges.h"
#include "ParallelUtility.h"
#include "AdamsPCData.h"

// =====================================================================================================
/// \brief Make predictions for time independent variables (e.g. the pressure for the INS) during the
///        predictor step of some time-stepping methods.
/// 
/// \param numberOfTimeLevels : number of time levels available into the array of grid functions, gf[].
/// \param gfIndex[0:numLevels-1] : array of index's into the gf[] array. 
///       gfIndex[0]=time-level to assign (mNew), gfIndex[1]=previous-time-level, gfIndex[2]=2nd-previous-time-level, etc. 
//       
// =====================================================================================================
int DomainSolver::
predictTimeIndependentVariables( const int numberOfTimeLevels, const int *gfIndex )
{
  const int orderOfAccuracy = parameters.dbase.get<int >("orderOfAccuracy");

  // Some solvers will set the following variable:
  const bool & predictedPressureNeeded = parameters.dbase.get<bool>("predictedPressureNeeded");

  const bool predictPressure = predictedPressureNeeded || (poisson!=NULL && poisson->isSolverIterative() && orderOfAccuracy!=4);

  if( TRUE || debug() & 4  )
    printF("--DS-- predictTimeIndependentVariables: predictPressure=%i (t=%8.2e)\n",
	   (int)predictPressure,gf[gfIndex[0]].t);
  

  if( !predictPressure )
    return 0;

  const aString & className = getClassName();
  if( className!="Cgins" &&  className!="Cgasf"  )
  {
    printF("WARNING: DomainSolver::predictTimeIndependentVariables: solver is not Cgins or Cgasf! className=%s\n",
	   (const char*)getClassName() );
  }

 FILE *& debugFile = parameters.dbase.get<FILE* >("debugFile");
 FILE *& pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  assert( numberOfTimeLevels>=3 );
  
  const int mNew=gfIndex[0];
  const int mCur=gfIndex[1];
  const int mOld=gfIndex[2];
  
   if( !parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") )
     parameters.dbase.get<DataBase >("modelData").put<AdamsPCData>("AdamsPCData");
   AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
  
   const real & dtb=adamsData.dtb;
   const int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
   const int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
   const int &ndt0=adamsData.ndt0;
   const real *dtp = adamsData.dtp;

   const real dt0 = dtp[ndt0];
   const real dt1=dtp[(ndt0+1)%5];
   const real dt2=dtp[(ndt0+2)%5];
   const real dt3=dtp[(ndt0+3)%5];
   const real dt4=dtp[(ndt0+4)%5];

   // coefficients for 2nd order extrap:
   assert( dt0>0. && dtb>0. );
   const real cex2a=1.+dt0/dtb;       // -> 2.
   const real cex2b=-dt0/dtb;         // -> -1.

  if( gf[mNew].t <= 2.*dt0 )
    printF("--DS-- predictTimeIndependentVariables (pressure) at t=%9.3e (dt0=%9.3e, dtb=%9.3e) "
           "cex2a=%5.2f cex2b=%5.2f mNew=%i mCur=%i mOld=%i\n", gf[mNew].t,dt0,dtb,cex2a,cex2b,mNew,mCur,mOld);

  // extrapolate p in time as an initial guess for iterative solvers
  const int & pc = parameters.dbase.get<int >("pc");
  assert( pc>= 0 );
  Index I1,I2,I3;
  for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
  {
    getIndex(gf[mCur].cg[grid].dimension(),I1,I2,I3);
    // note that initially gf[mNew](.,.,.,pc) = p(t-dt)
    // **** check this -- it's doesn't seem to make much difference whether we
    // extrapolate or use the old value ??

    OV_GET_SERIAL_ARRAY_CONST(real,gf[mNew].u[grid],uNew);
    OV_GET_SERIAL_ARRAY_CONST(real,gf[mCur].u[grid],uCur);
    OV_GET_SERIAL_ARRAY_CONST(real,gf[mOld].u[grid],uOld);
    bool ok = ParallelUtility::getLocalArrayBounds(gf[mNew].u[grid],uNew,I1,I2,I3); 
    if( !ok ) continue;

// #ifdef USE_PPP
//     realSerialArray uNew; getLocalArrayWithGhostBoundaries(gf[mNew].u[grid],uNew);
//     realSerialArray uCur; getLocalArrayWithGhostBoundaries(gf[mCur].u[grid],uCur);
//     realSerialArray uOld; getLocalArrayWithGhostBoundaries(gf[mOld].u[grid],uOld);
//     bool ok = ParallelUtility::getLocalArrayBounds(gf[mNew].u[grid],uNew,I1,I2,I3); 
//     if( !ok ) continue;
// #else
//     const realSerialArray & uNew=gf[mNew].u[grid];
//     const realSerialArray & uCur=gf[mCur].u[grid];
//     const realSerialArray & uOld=gf[mOld].u[grid];
// #endif

    uNew(I1,I2,I3,pc)=cex2a*uCur(I1,I2,I3,pc)+cex2b*uOld(I1,I2,I3,pc);

    // if( true || (debug() & 4) )
    // {
    //   ::display(uCur(I1,I2,I3,pc),sPrintF("--PTIV-- predictTimeIndependentVariables: p-current"
    // 							   " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%10.7f ");
    //   ::display(uOld(I1,I2,I3,pc),sPrintF("--PTIV-- predictTimeIndependentVariables: p-old"
    // 							   " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%10.7f ");
    // }
    
    if( debug() & 4 )
    {
      ::display(uNew(I1,I2,I3,pc),sPrintF("--PTIV-- predictTimeIndependentVariables: after extrap p in time"
                                          " t=%9.4e grid=%i\n",gf[mNew].t,grid),debugFile,"%7.4f ");
      ::display(uCur(I1,I2,I3,pc),sPrintF("--PTIV-- p(Current) t=%9.3e",gf[mCur].t),"%7.4f ");
      ::display(uOld(I1,I2,I3,pc),sPrintF("--PTIV-- p(Old)     t=%9.3e",gf[mOld].t),"%7.4f ");
    }

  } // end for grid 
  
  return 0;
}

