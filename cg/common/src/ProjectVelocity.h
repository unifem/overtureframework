#ifndef PROJECT_VELOCITY_H
#define PROJECT_VELOCITY_H

#include "Overture.h"
class GenericCompositeGridOperators;
class Oges;

class ProjectVelocity
{
 public:


  ProjectVelocity();
  ~ProjectVelocity();
  

  real computeDivergence(const realCompositeGridFunction & u, 
			 realCompositeGridFunction & divergence );

  real computeDivergence(const realCompositeGridFunction & u, 
	  	         realCompositeGridFunction & divergence,
		         real & divl2Norm );

  void smoothVelocity(realCompositeGridFunction & u, 
		      const int numberOfSmooths );

  int projectVelocity(realCompositeGridFunction & u, 
		      GenericCompositeGridOperators & op );
  
  int setCompare3Dto2D( int value );
  int setIsAxisymmetric( bool trueOrFalse=TRUE );
  int setNumberOfSmoothsPerProjectionIteration(int number );
  int setMinimumNumberOfProjectionIterations(int number );
  int setMaximumNumberOfProjectionIterations(int number );
  int setDebug(int number );
  int setConvergenceTolerance(real value);
  int setVelocityComponent(int uc);
  int setPoissonSolver(Oges *poissonSolver);

 protected:

  int uc;
  bool axisymmetric;
  int numberOfSmoothsPerProjectionIteration;
  int minimumNumberOfProjectionIterations;
  int maximumNumberOfProjectionIterations;
  int debug;
  int compare3Dto2D;
  real convergenceTolerance;
  Oges *poissonSolver;

};


#endif
