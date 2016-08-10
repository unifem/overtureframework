
//                                   -*- c++ -*-
#ifndef FDMBEAM_MODEL_H
#define FDMBEAM_MODEL_H

#include "BeamModel.h"


// finite difference beam
class FDBeamModel: public BeamModel
{

  static int FDBeamCounter;


public:
  FDBeamModel();
  ~FDBeamModel();

  //everything should be private here. 
  //public interface is hanlded via base class.
  //private:
  
  // add internalforces such as buoyance and TZ forcing 
  virtual void addInternalForces( const real t, RealArray & f );
  
  // Add to the force on the beam
  // virtual void addForce(const real & tf, const RealArray & x0, const RealArray & traction, 
  // 			const RealArray & normal,const Index & Ib1, const Index & Ib2,  
  // 			const Index & Ib3 );
  // Longfei 20160621: new addForce() function, we now separate setSurfaceForce and addForce
  //                            addForce() adds the surfaceForce to the current force
  virtual void addForce();

  // assign boundary conditions
  virtual int assignBoundaryConditions( real t, RealArray & x, RealArray & v, RealArray & a, const RealArray & f);

  // -- initialize the FEMBeam model given the current parameters --
  virtual void initialize();

  // Compute the acceleration of the beam.
  virtual  void computeAcceleration(const real t,
				    const RealArray& u, const RealArray& v, 
				    const RealArray& f,
				    RealArray& a,
				    real linAcceleration[2],
				    real& omegadd,real dt,
				    const aString & solverName );

  // Compute the internal force in the beam, i.e., -B(v) -K(u), where B and K are operators such that
  // K(u) = K0*u-T*uxx+EI*uxxxx,  B(v) = Kt*v-Kxxt*vxx
  virtual void computeInternalForce( const RealArray& u, const RealArray& v, RealArray& f );

  // modify the pentadiagonal matrix defined by at,...,et for extrapolation
  void modifyMatrixForExtrapolation(RealArray & at,RealArray & bt,RealArray & ct,RealArray & dt,RealArray &et, int ie, int side);


  //  Return the time derivatives of the forces on the boundary
  int getBoundaryForces( const real t, RealArray & f, const int ntd=0 );
  
 // Return nodal force values on beam center-line
  virtual void getForceOnBeam( const real t, RealArray & force );

  //Longfei 20160216: new function to factor  tridiangnal solvers for FDBeamModel
  int factorTridiagonalSolver(const aString & tridiagonalSolverName);

  // smooth the solution: Longfei 20160302: made this virtual
  virtual void smooth( const real t, RealArray & w, const aString & label );

  
  void solveTridiagonal(const RealArray& f, RealArray& u, const aString & tridiagonalSolverName );
  
};



#endif
