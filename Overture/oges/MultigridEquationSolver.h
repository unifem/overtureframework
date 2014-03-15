#ifndef MULTIGRID_EQUATION_SOLVER_H
#define MULTIGRID_EQUATION_SOLVER_H

//
//  Multigrid solver for Oges
// 

#include "OvertureDefine.h"
#include OV_STD_INCLUDE(iostream)

#include <math.h>
#include <assert.h>

#include "EquationSolver.h"
#include "Ogmg.h"


class MultigridEquationSolver : public EquationSolver
{
 public:
  MultigridEquationSolver(Oges & oges_);
  virtual ~MultigridEquationSolver();

  virtual int solve(realCompositeGridFunction & u,
		    realCompositeGridFunction & f);

  // new way to set coefficients:
  virtual int setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff,
                                                    const IntegerArray & boundaryConditions,
					            const RealArray & bcData );
  
  // old way to set coefficients: 
  virtual int setCoefficientArray( realCompositeGridFunction & coeff,
			           const IntegerArray & boundaryConditions=Overture::nullIntArray(),
                                   const RealArray & bcData=Overture::nullRealArray() );

  virtual int setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
                                                CompositeGridOperators & op,
                                                const IntegerArray & boundaryConditions,
						const RealArray & bcData, 
                                                RealArray & constantCoeff = Overture::nullRealArray(),
                                                realCompositeGridFunction *variableCoeff=NULL );

  // call this function when the grid changes (and before setCoefficientsAndBoundaryConditions)
  virtual int setGrid( CompositeGrid & cg );

  // Set the MultigridCompositeGrid to use: (for use with Ogmg)
  virtual int set( MultigridCompositeGrid & mgcg );

  virtual int printStatistics( FILE *file = stdout ) const;   // output any relevant statistics 

  virtual real sizeOf( FILE *file=NULL ); // return number of bytes allocated 

 protected:

  Ogmg ogmg;
  
};


#endif
