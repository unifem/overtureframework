// This file supplies an empty version of the Ogmg functions to be
// used when Ogmg is not compiled with Overture.

#include "OgmgParameters.h"
#include "Ogmg.h"

int Ogmg::debug=0;

// *wdh* 100607 -- Always compile OgmgParameters.C even without Ogmg (for scripts that set Ogmg parameters)

// OgmgParameters::OgmgParameters(){}
// OgmgParameters::OgmgParameters(CompositeGrid & cg){}  
// OgmgParameters::~OgmgParameters(){}
// OgmgParameters& OgmgParameters::operator=(const OgmgParameters& par){return *this;}
// 
// int OgmgParameters::set( CompositeGrid & cg){return 0;} 
// int OgmgParameters::updateToMatchGrid( CompositeGrid & cg, int maxLevels){return 0;}  
// int OgmgParameters::setParameters( const Ogmg & ogmg){return 0;}  
// int OgmgParameters::update( GenericGraphicsInterface & gi, CompositeGrid & cg ){return 0;} 
// int OgmgParameters::set( OptionEnum option, int value ){return 0;}
// int OgmgParameters::set( OptionEnum option, float value ){return 0;}
// int OgmgParameters::set( OptionEnum option, double value ){return 0;}
// int OgmgParameters::setMaximumNumberOfIterations( const int max ){return 0;}
// int OgmgParameters::setNumberOfCycles( const int & number, const int & level ){return 0;}
// int OgmgParameters::setNumberOfSmooths(const int numberOfPreSmooths_, const int num2, const int level){return 0;}
// int OgmgParameters::setNumberOfSubSmooths( const int & numberOfSmooths_, const int & grid, const int & level){return 0;}
// int OgmgParameters::setResidualTolerance(const real residualTolerance_ ){return 0;}
// int OgmgParameters::setErrorTolerance(const real errorTolerance_ ){return 0;}
// int OgmgParameters::setProblemIsSingular( const bool trueOrFalse){return 0;}
// int OgmgParameters::setMeanValueForSingularProblem( const real meanValue ){return 0;}
// int OgmgParameters::setSmootherType(const SmootherTypeEnum & smoother, 
// 		      const int & grid, 
// 		      const int & level ){return 0;}
// int OgmgParameters::get( OptionEnum option, int & value ) const{return 0;}
// int OgmgParameters::get( OptionEnum option, real & value ) const{return 0;}
// int OgmgParameters::get( const GenericDataBase & dir, const aString & name){return 0;}
// int OgmgParameters::put( GenericDataBase & dir, const aString & name) const{return 0;}
// int OgmgParameters::display(FILE *file ){return 0;}

// -------------------------------------------------------------------------------------------------


Ogmg::Ogmg(){}
Ogmg::Ogmg( CompositeGrid & mg, GenericGraphicsInterface *ps_){}
Ogmg::~Ogmg(){}
int Ogmg::setOgmgParameters(OgmgParameters & parameters_ ){return 0;}
int Ogmg::setOrderOfAccuracy(const int & orderOfAccuracy_){return 0;}
real Ogmg::sizeOf( FILE *file ) const {return 0.;} 
int Ogmg::chooseBestSmoother(){return 0;}
real Ogmg::getMean(realCompositeGridFunction & u){return 0.;}
int Ogmg::getNumberOfIterations() const{return 0;}
real Ogmg::getMaximumResidual() const{return 0.;}
int Ogmg::update( GenericGraphicsInterface & gi ){return 0;} 
int Ogmg::update( GenericGraphicsInterface & gi, CompositeGrid & cg ){return 0;} 
void Ogmg::updateToMatchGrid( CompositeGrid & mg ){}
int Ogmg::setCoefficientArray( realCompositeGridFunction & coeff,
                               const IntegerArray & boundaryConditions /* =Overture::nullIntArray() */,
                               const RealArray & bcData ){return 0;}
void Ogmg::set( GenericGraphicsInterface *ps_ ){}
void Ogmg::set( MultigridCompositeGrid & mgcg ){}
void Ogmg::setGridName( const aString & name ){}
void Ogmg::setSolverName( const aString & name ){}

int Ogmg::solve( realCompositeGridFunction & u, realCompositeGridFunction & f ){return 0;}
void Ogmg::printStatistics(FILE *file) const{};


void Ogmg::defect(const int & level){}
void Ogmg::defect(const int & level, const int & grid){}


int Ogmg::setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, CompositeGridOperators & op,
          const IntegerArray & boundaryConditions,const RealArray & bcData, const RealArray & constantCoeff,
				  realCompositeGridFunction *variableCoeff ){return 0;} 
