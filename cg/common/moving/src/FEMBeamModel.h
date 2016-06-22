
//                                   -*- c++ -*-
#ifndef FEMBEAM_MODEL_H
#define FEMBEAM_MODEL_H

#include "BeamModel.h"


// finite element beam 
class FEMBeamModel: public BeamModel
{

  static int FEMBeamCounter;

  // Current time
  //
  //real t;  //Longfei: leave it here for now. It's too tedius to change it.


public:
  FEMBeamModel();
  ~FEMBeamModel();

  //everything should be private here. 
  //public interface is hanlded via base class.
private:
  // Add to the force on the beam
  // virtual void addForce(const real & tf, const RealArray & x0, const RealArray & traction, 
  // 			const RealArray & normal,const Index & Ib1, const Index & Ib2,  
  // 			const Index & Ib3 );
  // Longfei 20160621: new addForce() function, we now separate setSurfaceForce and addForce
  //                            addForce() adds the surfaceForce to the current force
  virtual void addForce();
  

  // ??? Longfei 20160329: this function seems unused... remove for now
  // Accumulate a pressure force to the beam from a fluid element.
  // virtual void addForce(const real & tf, const real& x0_1, const real& y0_1,
  // 			real p1, real p1x, const real& nx_1,const real& ny_1,
  // 			const real& x0_2, const real& y0_2,
  // 			real p2, real p2x, const real& nx_2,const real& ny_2);

  // add internalforces such as buoyance and TZ forcing 
  virtual void addInternalForces( const real t, RealArray & f );


  // Compute the acceleration of the beam.
  // old:
  // void computeAcceleration(const real t,
  //                          const RealArray& u, const RealArray& v, 
  // 			   const RealArray& f,
  // 			   const RealArray& A,
  // 			   RealArray& a,
  // 			   real linAcceleration[2],
  // 			   real& omegadd,real dt,
  //                          const real alpha, const real alphaB, 
  //                          const aString & tridiagonalSolverName );
  // Longfei 20160208: new
  virtual void computeAcceleration(const real t,
				   const RealArray& u, const RealArray& v, 
				   const RealArray& f,
				   RealArray& a,
				   real linAcceleration[2],
				   real& omegadd,real dt,
				   const aString & solverName );
  


  // Compute the internal force in the beam, i.e., -B*v -K*u
  virtual void computeInternalForce( const RealArray& u, const RealArray& v, RealArray& f );
  
  
  // Return nodal force values on beam center-line
  virtual void getForceOnBeam( const real t, RealArray & force );


  // -- initialize the FEMBeam model given the current parameters --
  virtual void initialize();
  
  
  

  // Multiply a vector w by the mass matrix
  void multiplyByMassMatrix(const RealArray& w, RealArray& Mw);



  // smooth the solution:
  virtual void smooth( const real t, RealArray & w, const aString & label );

  // assign boundary conditions for the smooth function
  int smoothBoundaryConditions( RealArray & w1, int base, int bound, int numberOfGhost, int orderOfExtrapolation );


};




#endif
