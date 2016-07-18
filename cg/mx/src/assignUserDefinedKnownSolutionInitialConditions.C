// ========================================================================================================
// 
// Cgmx:  Assign initial conditions from the userDefinedKnownSolution
// 
// ========================================================================================================

#include "Maxwell.h"
#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

//==============================================================================================
/// \brief: Assign initial condtions from a user defined known solution.
///
/// \Notes:
///  When using adaptive mesh refinement, this function may be called multiple times as the
///  AMR hierarchy is built up.
///
//==============================================================================================
int Maxwell::
assignUserDefinedKnownSolutionInitialConditions(int current, real t, real dt )
{

  assert( initialConditionOption==userDefinedKnownSolutionInitialCondition );

  // if( ! dbase.has_key("userDefinedKnownSolutionData") )
  // {
  //   printF("--MX-- getUserDefinedKnownSolution:ERROR: sub-directory `userDefinedKnownSolutionData' not found!\n");
  //   OV_ABORT("error");
  // }
  // DataBase & db =  dbase.get<DataBase>("userDefinedKnownSolutionData");

  // const aString & userKnownSolution = db.get<aString>("userKnownSolution");

  if( true )
    printF("--MX-- assignUserDefinedKnownSolutionInitialConditions: t=%9.3e\n",t);

  // Here is the CompositeGrid: 
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int numberOfDimensions = cg.numberOfDimensions();

  // Here is the current and previous solutions: 
  realCompositeGridFunction & u  = cgfields[current];
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  realCompositeGridFunction & um = cgfields[prev];


  Index I1,I2,I3;

  // --- Loop over all grids and assign values ----
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex( mg.dimension(),I1,I2,I3 );          // all points including ghost points.

      
    int numberOfTimeDerivatives=0;
    if( method==nfdtd  ) 
    {
      // -- assign solution at time t:
      getUserDefinedKnownSolution(   t, cg,grid, u[grid],I1,I2,I3,numberOfTimeDerivatives);

      // assign solution at time t-dt
      getUserDefinedKnownSolution(t-dt, cg,grid,um[grid],I1,I2,I3,numberOfTimeDerivatives);

    }
    else if( method==sosup )
    {
      // -- assign solution at time t:
      getUserDefinedKnownSolution(   t, cg,grid, u[grid],I1,I2,I3,numberOfTimeDerivatives);

    }
    else
    {
      OV_ABORT("assignUserDefinedKnownSolutionInitialConditions:ERROR: finish me");
    }
    
      

  }

  return 0;
}


