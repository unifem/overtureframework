// ================================================================================
// Cgmp : CG multi-domain multi-physics solver
// ================================================================================


#ifndef CGMP_H
#define CGMP_H

#include "DomainSolver.h"
#include "Interface.h"
#include "MpParameters.h"



class Cgmp : public DomainSolver
{
public:

Cgmp(CompositeGrid & cg, GenericGraphicsInterface *ps=NULL, Ogshow *show=NULL, const int & plotOption=1 );

virtual ~Cgmp();

virtual int 
assignInterfaceBoundaryConditions(std::vector<int> & newIndex, 
				  const real dt );

virtual int 
assignInterfaceRightHandSide( int d, real t, real dt, int correct, std::vector<int> & gfIndex );

// Old version: 
virtual int 
assignInterfaceRightHandSideOld( int d, real t, real dt, int correct, std::vector<int> & gfIndex );

virtual DomainSolver* 
buildModel( const aString & modelName, CompositeGrid & cg, GenericGraphicsInterface *ps=NULL, Ogshow *show=NULL, const int & plotOption=1 );

virtual int 
buildRunTimeDialog();

virtual int
buildTimeSteppingDialog(DialogData & dialog );

bool
checkInterfaceForConvergence( const int correct,
                              const int numberOfCorrectorSteps,
                              const int numberOfRequiredCorrectorSteps,
                              const real tNew,
                              const bool alwaysSetBoundaryData,
                              std::vector<int> & gfIndex,
                              std::vector<real> & oldResidual,
                              std::vector<real> & initialResidual,
                              std::vector<real> & firstResidual,
                              std::vector<real> & maxResidual,
                              bool & interfaceIterationsHaveConverged );

/// last minute checks and setups prior to actually running
virtual int 
cycleZero();

bool 
checkIfInterfacesMatch(Mapping &map1, int &dir1, int &side1, Mapping &map2, int &dir2, int &side2);

/// perform tasks needed right after an advance (nothing right now), returns nonzero if the computation is finished
virtual int 
finishAdvance(); 

enum InterfaceValueEnum
{
  doNotSaveInterfaceValues,
  saveInterfaceTimeHistoryValues,
  saveInterfaceIterateValues
};
  
virtual int 
getInterfaceResiduals( real t, real dt, std::vector<int> & gfIndex, std::vector<real> & maxResidual, 
                       InterfaceValueEnum saveInterfaceValues=doNotSaveInterfaceValues );

// OLD version -- only for interfaces with a single face: 
virtual int 
getInterfaceResidualsOld( real t, real dt, std::vector<int> & gfIndex, std::vector<real> & maxResidual, 
                          InterfaceValueEnum saveInterfaceValues=doNotSaveInterfaceValues );

virtual int 
getModelInfo( std::vector<aString> & modelName );

virtual real 
getTimeStep( GridFunction & gf); 

virtual int
getTimeSteppingOption(const aString & answer, DialogData & dialog );

virtual int 
initializeInterfaceBoundaryConditions( real t, real dt, std::vector<int> & gfIndex );

virtual int
initializeInterfaces(std::vector<int> & newIndex);

virtual int 
interfaceProjection( real t, real dt, int correct, std::vector<int> & gfIndex, int option );

// The next routine is used to advance solutions on multi-domains and supports forward-Euler, 
// predictor-corrector, implicit methods etc.
virtual int 
multiDomainAdvance(real & t, real &tFinal );

// here is the new multi-domain advance routine that works with AMR
virtual int 
multiDomainAdvanceNew(real & t, real &tFinal );

// Here is the even newer multi-stage algorithm (user defined stages)
int 
multiStageAdvance( real &t, real & tFinal );

virtual int
plot(const real & t, const int & optionIn, real & tFinal );

int 
plotDomainQuantities( std::vector<realCompositeGridFunction*> u, real t );

virtual int 
printStatistics(FILE *file = stdout);

virtual void
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

int
projectInitialConditions( real t, real dt, std::vector<int> & gfIndex );

virtual int
setParametersInteractively(bool runSetupOnExit=true);

/// perform tasks needed prior to an actual advance (file io stuff mostly), returns nonzero if the computation is finished
virtual int 
setupAdvance(); 

virtual int 
setupDomainSolverParameters( int domain, std::vector<aString> & modelNames );

virtual int 
setupPde(aString & reactionName,bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual void setup(const real & time = 0.);

virtual void
setTopLabel(std::vector<realCompositeGridFunction*> u, real t);

virtual void
saveShow( GridFunction & gf );

virtual int 
advance(real &tFinal);

virtual int
solve();

// std::vector<DomainSolver*> domainSolver;  // holds PDE solvers for each domain : now in base class

Interpolant *interpolant;

protected:

// utility routine to return the interface type for a grid face on the interface:
int
getInterfaceType( GridFaceDescriptor & gridDescriptor );



private:


};

#endif
