//                                   -*- c++ -*-
#include "BeamModel.h"
#include "display.h"

#include <sstream>

// Forward declarations of utility matrix functions on A++ arrays
//
RealArray 
mult( const RealArray & a, const RealArray & b );

RealArray 
trans( const RealArray &a );

RealArray 
solve( const RealArray & a, const RealArray & b );

// Forward declaration of printArray() function (for debugging)
void 
printArray(const doubleSerialArray & u,  
           int i1a, int i1b,   
           int i2a, int i2b,   
           int i3a, int i3b,   
           int i4a, int i4b,   
           int i5a, int i5b,   
           int i6a, int i6b) ;


// --- assign static class variables --
real BeamModel::exactSolutionScaleFactorFSI=.00001;  // scale factor for the exact FSI solution 
int BeamModel::debug=0;
int BeamModel::globalBeamCounter=0; // keeps track of number of beams that have been created


// Constructor.
//
BeamModel::BeamModel() {

  name = "beam";

  globalBeamCounter++;
  beamID=globalBeamCounter; //  a unique ID 
  
  elementK.redim(4,4);
  elementM.redim(4,4);

  t = 0.0;

  //setParameters(0.02*0.02*0.02/12.0,
  //		1.4e6,10000.0, 0.3,0.02, 15);

  density=1.;
  thickness=.1;
  L = 1.;

  areaMomentOfInertia = .1;
  elasticModulus = 1;
  L = 1.;
  numElem = 11;


  newmarkBeta = 0.25;
  newmarkGamma = 0.5;

  time_step_num = 1;

  pressureNorm = 1000.0;

  hasAcceleration = false;

  bcLeft = bcRight = Pinned;
  //bcLeft = bcRight = Cantilevered;

//  added_mass_relaxation = 1.0;

  numCorrectorIterations = 0;

  // convergenceTolerance = 1e-3;

  allowsFreeMotion = false;

  centerOfMass[0] = 0.0;
  centerOfMass[1] = 0.0;

  angle = 0.0;

  bodyForce[0] = bodyForce[1] = 0.0;
  
  beamInitialAngle = 0.0;

  initialBeamNormal[0] = 0.0;
  initialBeamNormal[1] = 1.0;
  initialBeamTangent[0] = 1.0;
  initialBeamTangent[1] = 0.0;

  projectedBodyForce = 0.0;

  for (int k = 0; k < 2; ++k) {

    normal[k] = initialBeamNormal[k];
    tangent[k] = initialBeamTangent[k];
  }

  penalty = 1e2;

  leftCantileverMoment = 0.0;

  if( !dbase.has_key("saveProfileFile") ) 
  {
     dbase.put<bool>("saveProfileFile");
     dbase.get<bool>("saveProfileFile")=false;
  }

  if( !dbase.has_key("saveTipFile") ) 
  {
     dbase.put<bool>("saveTipFile");
     dbase.get<bool>("saveTipFile")=false;
  }

  usesExactSolution=false;


  // The relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  dbase.put<real>("addedMassRelaxationFactor",1.0);

  // The (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  dbase.put<real>("subIterationConvergenceTolerance",1.0e-3);
  
  // { // wdh: replaces 'what' factor 
  //   dbase.put<real>("exactSolutionScaleFactorFSI");
  //   dbase.get<real>("exactSolutionScaleFactorFSI")=0.00001; // scale FSI solution so linearized approximation is valid 
  // }
  

}

BeamModel::~BeamModel() {

}


// ======================================================================================
/// \brief initialize the beam model
// ======================================================================================
void BeamModel::
initialize()
{
  le = L / numElem;

  real le2 = le*le;
  real le3 = le2*le;

  totalMass = density*L*thickness;

  massPerUnitLength = totalMass / L;

  // Buoyancy terms are used when there is a body force (gravity)
  buoyantMassPerUnitLength = (density - pressureNorm)*thickness;

  buoyantMass = buoyantMassPerUnitLength * L;

  totalInertia = totalMass*L*L/12.0;

  real EI = elasticModulus*areaMomentOfInertia;

  // std::cout << "EI = " << EI << std::endl;

  elementK(0,0) = EI*12./le3; elementK(0,1) = EI*6./le2; 
  elementK(0,2) = -elementK(0,0); elementK(0,3) = elementK(0,1);

  elementK(1,0) = elementK(0,1); elementK(1,1) = EI*4./le; 
  elementK(1,2) = -elementK(0,1); elementK(1,3) = EI*2./le;

  elementK(2,0) = elementK(0,2); elementK(2,1) = elementK(1,2); 
  elementK(2,2) = elementK(0,0); elementK(2,3) = elementK(1,2);
  
  elementK(3,0) = elementK(0,1); elementK(3,1) = elementK(1,3); 
  elementK(3,2) = elementK(2,3); elementK(3,3) = elementK(1,1);
  
  elementM(0,0) = elementM(2,2) = 13./35.*le*density*thickness;
  elementM(0,1) = elementM(1,0) = 11./210.*le2*density*thickness;
  elementM(0,2) = elementM(2,0) = 9./70.*le*density*thickness;
  elementM(1,3) = elementM(3,1) = -1./140.*le3*density*thickness;
  elementM(3,2) = elementM(2,3) = -11./210.*le2*density*thickness;
  elementM(1,2) = elementM(2,1) = 13./420.*le2*density*thickness;
  elementM(0,3) = elementM(3,0) = -13./420.*le2*density*thickness;
  elementM(1,1) = elementM(3,3) = 1./105.*le3*density*thickness;

  myPosition.redim(numElem*2+2);
  myVelocity.redim(numElem*2+2);
  myAcceleration.redim(numElem*2+2);
  myForce.redim(numElem*2+2);

  tmp.redim(numElem*2+2);

  myPosition = 0.0;
  myVelocity = 0.0;


  if (usesExactSolution) {

    setExactSolution(0.0,myPosition,myVelocity,myAcceleration);
    //    myAcceleration = 0.0;
  }

  if (bcLeft == Pinned || bcLeft == Cantilevered) {

    myPosition(0) = 0.0;
    myVelocity(0) = 0.0;
  }

  if (bcLeft == Cantilevered) {

    myPosition(1) = 0.0;
    myVelocity(1) = 0.0;
  }

  if (bcRight == Pinned || bcRight == Cantilevered) {

    myPosition(numElem*2) = 0.0;
    myVelocity(numElem*2) = 0.0;
  }

  if (bcRight == Cantilevered) {

    myPosition(numElem*2+1) = 0.0;
    myVelocity(numElem*2+1) = 0.0;
  }

  
  myPosition_nm1 = myPosition;
  myVelocity_nm1 = myVelocity;

  if (!usesExactSolution)
    myAcceleration = 0.0;

  aold = myAcceleration;

  myForce = 0.0;

  dtilde = myPosition;
  vtilde = myVelocity;



}



void BeamModel::setParameters(real momOfInertia, real E, 
			      real rho,real beamLength,
			      real thickness_,real pnorm,
			      int nElem,BoundaryCondition bcl,
			      BoundaryCondition bcr,
			      real x0, real y0,
			      bool useExactSolution) {


  areaMomentOfInertia = momOfInertia;
  elasticModulus = E;
  density = rho;
  L = beamLength;
  thickness=thickness_;

  pressureNorm = pnorm;

  numElem = nElem;

  bcLeft = bcl;
  bcRight = bcr;

  beamX0 = x0;
  beamY0 = y0;

  usesExactSolution = useExactSolution;

  initialize();

}


void BeamModel::computeProjectedForce(real p1, real p2, 
				      real a, real b,
				      RealArray& fe) {

  real le2 = le*le;
  real le3 = le2*le;

  real dp = p2-p1;
  real de = b-a;
  real t1 = p1*b-p2*a;
  
  real ab2 = (b+a)*de;
  real ab3 = de*(b*b+a*a+a*b);
  real ab4 = ab2*(a*a+b*b);
  real ab5 = pow(b,5.)-pow(a,5.);
  
  real x1 = le*dp*ab5,
    x2 = le*t1*ab4,
    x7 = le*dp*ab4,
    x8 = le*t1*ab3,
    x3 = le*dp*ab3,
    x4 = le*t1*ab2,
    x5 = le*dp*ab2,
    x6 = le*t1*de;

  real invde = 1./de;
  
  fe(0) = 1./40.*x1+1./32.*x2-1./8.*x3-3./16.*x4+1./8.*x5+1./4.*x6;
  fe(1) = 1./80.*x1+1./64.*x2-1./64.*x7-1./48.*x8-1./48.*x3-1./32.*x4+1./32.*x5+1./16.*x6;
  
  fe(2) = -1./40.*x1-1./32.*x2+1./8.*x3+3./16.*x4+1./8.*x5+1./4.*x6;
  fe(3) = 1./80.*x1+1./64.*x2+1./64.*x7+1./48.*x8-1./48.*x3-1./32.*x4-1./32.*x5-1./16.*x6;

  fe(0) *= invde;
  fe(2) *= invde;

  fe(1) *= invde*le;
  fe(3) *= invde*le;
  
}

void BeamModel::setupFreeMotion(real x0,real y0, real angle0) {

  centerOfMass[0] = x0;
  centerOfMass[1] = y0;
  angle = angle0;

  centerOfMassVelocity[0] = 0.0;
  centerOfMassVelocity[1] = 0.0;

  centerOfMassAcceleration[0] = 0.0;
  centerOfMassAcceleration[1] = 0.0;

  angularVelocity = angularAcceleration = 0.0;

  setDeclination(angle);

  recomputeNormalAndTangent();

  initialEndLeft[0] = x0 - tangent[0]*L*0.5;
  initialEndLeft[1] = y0 - tangent[1]*L*0.5;
  
  initialEndRight[0] = x0 + tangent[0]*L*0.5;
  initialEndRight[1] = y0 + tangent[1]*L*0.5;

  allowsFreeMotion = true;

}

void BeamModel::addBodyForce(const real bf[2]) {

  bodyForce[0] = bf[0];
  bodyForce[1] = bf[1];

  projectedBodyForce = normal[0]*bodyForce[0] + normal[1]*bodyForce[1];
}

static void inverse2x2(const RealArray& A, RealArray& inv) {

  inv.redim(2,2);
  real odet = 1./(A(0,0)*A(1,1) - A(0,1)*A(1,0));
  inv(0,0) = odet*A(1,1);
  inv(0,1) = -odet*A(0,1);
  inv(1,0) = -odet*A(1,0);
  inv(1,1) = odet*A(0,0);
}

// The last 6 arguments are for periodic boundary conditions.  
static void solveBlockTridiagonal(const RealArray& elementM, const RealArray& f,
				  RealArray& u,
				  BeamModel::BoundaryCondition bcLeft,
				  BeamModel::BoundaryCondition bcRight,
				  bool allowsFreeMotion,
				  bool augmented = false,
				  RealArray* augmentedRow = NULL,
				  RealArray* augmentedCol = NULL,
				  real* augmentedDiagonal = NULL,
				  real* augmentedRHS = NULL,
				  real* augmentedSolution = NULL) {

  RealArray ftmp;
  ftmp.redim(2);
  u = f;
  RealArray upper;  upper.redim(2,2);
  RealArray diag1;  diag1.redim(2,2);
  RealArray diag2;  diag2.redim(2,2);

  int numElem = f.getLength(0)/2-1;

  upper(0,0) = elementM(0,2); upper(0,1) = elementM(0,3);
  upper(1,0) = elementM(1,2); upper(1,1) = elementM(1,3);

  RealArray upperT = trans(upper);

  diag1(0,0) = elementM(0,0); diag1(0,1) = elementM(0,1);
  diag1(1,0) = elementM(1,0); diag1(1,1) = elementM(1,1);

  diag2(0,0) = elementM(2,2); diag2(0,1) = elementM(2,3);
  diag2(1,0) = elementM(3,2); diag2(1,1) = elementM(3,3);
  
  RealArray dd = evaluate(diag1+diag2);

  std::vector< RealArray > diagonal(numElem+1,dd),
    superdiagonal(numElem,upper),subdiagonal(numElem, upperT);
  
  diagonal[0] = diag1;
  diagonal[numElem] = diag2;

  if (bcLeft == BeamModel::Cantilevered && !allowsFreeMotion) {
    diagonal[0](0,0) = diagonal[0](1,1) = 1.0;
    diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
    superdiagonal[0](0,0) = superdiagonal[0](0,1) = 0.0;
    superdiagonal[0](1,1) = superdiagonal[0](1,0) = 0.0;
  }
  if (bcLeft == BeamModel::Pinned && !allowsFreeMotion) {
    diagonal[0](0,0) = 1.0;
    diagonal[0](0,1) = 0.0;
    superdiagonal[0](0,0) = 0.0;
    superdiagonal[0](0,1) = 0.0;
  }
 
  if (bcRight == BeamModel::Cantilevered && !allowsFreeMotion) {
    diagonal[numElem](0,0) = diagonal[numElem](1,1) = 1.0;
    diagonal[numElem](0,1) = diagonal[numElem](1,0) = 0.0;
    subdiagonal[numElem-1](0,0) = subdiagonal[numElem-1](0,1) = 0.0;
    subdiagonal[numElem-1](1,1) = subdiagonal[numElem-1](1,0) = 0.0;
  }
  if (bcRight == BeamModel::Pinned && !allowsFreeMotion) {
    diagonal[numElem](0,0) = 1.0;
    diagonal[numElem](0,1) = 0.0;
    subdiagonal[numElem-1](0,0) = 0.0;
    subdiagonal[numElem-1](0,1) = 0.0;
  }


  RealArray inv;

  Index i2x2(0,2);
  
  for (int i = 0; i < numElem; ++i) {

    inverse2x2(diagonal[i], inv);
    superdiagonal[i] = mult(inv, superdiagonal[i]);
    u(i2x2) = mult(inv, u(i2x2) );
    u(i2x2+2) -= mult(subdiagonal[i],u(i2x2));
    diagonal[i+1] -= mult(subdiagonal[i],superdiagonal[i]);   

    if (augmented) {

      (*augmentedCol)(i2x2) = mult(inv , (*augmentedCol)(i2x2));
      (*augmentedCol)(i2x2+2) -= mult(subdiagonal[i], (*augmentedCol)(i2x2));

      (*augmentedRow)(i2x2+2) -= mult((*augmentedRow)(i2x2),superdiagonal[i]);

      *augmentedDiagonal -= mult( (*augmentedRow)(i2x2), (*augmentedCol)(i2x2) )(0);
      *augmentedRHS -= mult( (*augmentedRow)(i2x2), u(i2x2))(0);
    }

    i2x2 += 2;
  }

  if (augmented) {
    *augmentedSolution = *augmentedRHS / *augmentedDiagonal;

    for (int i = numElem*2+1; i >= 0; --i) {

      u(i) -= (*augmentedCol)(i)*(*augmentedSolution);
    }
  }

  inverse2x2(diagonal[numElem], inv);
  u(i2x2) = mult(inv, u(i2x2) );
  
  i2x2 -= 2;

  for (int i = numElem-1; i >= 0; --i) {

    u(i2x2) -= mult(superdiagonal[i], u(i2x2+2));

    i2x2 -= 2;
  }
  
}

void BeamModel::computeInternalForce(const RealArray& u,RealArray& f) {

  RealArray elementU;
  RealArray elementForce;

  elementU.redim(4);
 
  f = 0.0;

  for (int i = 0; i < numElem; ++i) {
  
    for (int k = 0; k < 4; ++k)
      elementU(k) = u(i*2+k);
    
    elementForce = mult(elementK, elementU);
    for (int k = 0; k < 4; ++k)
      f(i*2+k) -= elementForce(k);
  }

}

void BeamModel::multiplyByMassMatrix(const RealArray& w, RealArray& Mw) {

  RealArray elementU;
  RealArray tmpv;

  elementU.redim(4);

  tmpv.redim(4);
 
  Mw = w;
  Mw = 0.0;

  for (int i = 0; i < numElem; ++i) {
  
    for (int k = 0; k < 4; ++k)
      elementU(k) = w(i*2+k);
    
    tmpv = mult(elementM, elementU);
    for (int k = 0; k < 4; ++k)
      Mw(i*2+k) += tmpv(k);
  }

  
}

//
// Return the (x,y) coordinates of the beam centerline
// wdh 2014/05/22
void BeamModel::
getCenterLine( RealArray & xc ) const
{
  xc.redim(numElem+1,2);
  for( int i=0; i<=numElem; i++ ) 
  {
    // (xl,yl) = beam position (un-rotated)
    real xl = ((real)i /numElem) *  L;   // position along neutral axis 
    real yl = myPosition(2*i);           // displacement 

    xc(i,0) = beamX0 + initialBeamTangent[0]*xl - initialBeamTangent[1]*yl;
    xc(i,1) = beamY0 - initialBeamNormal [0]*xl + initialBeamNormal [1]*yl;
  }

}




void BeamModel::computeAcceleration(const RealArray& u, const RealArray& v, 
				    const RealArray& f,
				    const RealArray& A,
				    RealArray& a,
				    real linAcceleration[2],
				    real& omegadd,
				    real dt,
				    real loc_beta,
				    real loc_gamma) {

  computeInternalForce(u, tmp);

  tmp += f;

  if (!allowsFreeMotion) {

    if (bcLeft == BeamModel::Cantilevered) {
      tmp(0) = 0.0;
      tmp(1) = 0.0;
    }
    
    if (bcLeft == BeamModel::Pinned) {
      tmp(0) = 0.0;
    }
    
    if (bcRight == BeamModel::Cantilevered) {
      tmp(numElem*2) = 0.0;
      tmp(numElem*2+1) = 0.0;
    }
    
    if (bcRight == BeamModel::Pinned) {
      tmp(numElem*2) = 0.0;
    }
  }

  if (allowsFreeMotion) {
    
    //std::cout << "Total pressure force = " << totalPressureForce << std::endl;
    linAcceleration[0] = totalPressureForce*normal[0] / totalMass + bodyForce[0] * buoyantMass / totalMass;
    linAcceleration[1] = totalPressureForce*normal[1] / totalMass + bodyForce[1] * buoyantMass / totalMass;
    omegadd = totalPressureMoment / totalInertia;

    if (bcLeft == BeamModel::Pinned ||
	bcLeft == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(myPosition, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      linAcceleration[0] -= penalty*(end[0]-initialEndLeft[0])/totalMass;
      linAcceleration[1] -= penalty*(end[1]-initialEndLeft[1])/totalMass;
      
      real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/-normal[0]*L*0.5)+
			  (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/-normal[1]*L*0.5));
      omegadd -= mom / totalInertia;

      real shear = penalty*((end[0]-initialEndLeft[0])*(-normal[0])+
			    (end[1]-initialEndLeft[1])*(-normal[1]));
      
      tmp(0) += shear;
    }
    
    if (bcRight == BeamModel::Pinned ||
	bcRight == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = numElem-1;
      real eta = 1.0;
      interpolateSolution(myPosition, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      linAcceleration[0] -= penalty*(end[0]-initialEndRight[0])/totalMass;
      linAcceleration[1] -= penalty*(end[1]-initialEndRight[1])/totalMass;
      
      real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/normal[0]*L*0.5)+
			  (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/normal[1]*L*0.5));
      omegadd -= mom / totalInertia;

      real shear = penalty*((end[0]-initialEndRight[0])*(normal[0])+
			    (end[1]-initialEndRight[1])*(normal[1]));
      
      tmp(numElem*2) += shear;
    }

    if (bcLeft == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(myPosition, elem,eta, wend, wendslope);
      
      
      real slopeend[2] = {normal[0]*wendslope+tangent[0],
			  normal[1]*wendslope+tangent[1]};
      
      real proj = (-tangent[0]*wendslope+initialBeamNormal[0])*normal[0] + 
	(-tangent[1]*wendslope+initialBeamNormal[1])*normal[1] ;

      real err = slopeend[0]*initialBeamNormal[0] + 
	slopeend[1]*initialBeamNormal[1] ;

      real mom = 0.1*penalty*err*proj;
      
      real rf = 1.0;
      leftCantileverMoment = mom*rf + (1.0-rf)*leftCantileverMoment;
      omegadd -= leftCantileverMoment / totalInertia;

      std::cout << "End slope error = " << err << std::endl;
      
      //tmp(1) += mom; 
    }

    RealArray ones = f,res;
    ones = 0.0;
    for (int i = 0; i < numElem*2+2; i+=2)
      ones(i) = linAcceleration[0]*normal[0]+linAcceleration[1]*normal[1];

    //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
    multiplyByMassMatrix(ones, res);
    //res *= 1.0/massPerUnitLength*totalMass;
    tmp -= res;

    multiplyByMassMatrix(u, res);
    tmp += angularVelocityTilde*angularVelocityTilde*res;

    for (int i = 0; i < numElem*2+2; i+=2) {
      ones(i) = -0.5*L+le*(i/2);
      ones(i+1) = 1.0;
    }
    
    //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
    
    //real R = 0.0;
    //for (int i = 0; i < numElem*2+2; ++i) 
    //  R += f(i)*ones(i);
    //std::cout << "R = " << R << std::endl;
    
    //std::cout << "computed pressure moments = " << mult(evaluate(transpose(ones)),f)(0) << " " << totalPressureMoment << std::endl;

    multiplyByMassMatrix(ones, res);

    //std::cout << "inertias = " << mult(evaluate(transpose(ones)),res)(0) << " " << totalInertia << std::endl;

    tmp -= res*omegadd;
  }

  solveBlockTridiagonal(A, tmp,a,bcLeft,bcRight,allowsFreeMotion);
  
}

void BeamModel::projectDisplacement(const RealArray& X, const real& x0,
				    const real& y0, real& x, real& y) {

  int elemNum;
  real eta, thickness;
  
  projectPoint(x0,y0, elemNum, eta,thickness);
  
  real displacement, slope;
  interpolateSolution(X, elemNum, eta, displacement, slope);
  
  real omag = 1./sqrt(slope*slope+1.0);
  real normall[2] = {-slope*omag, omag};

  if (!allowsFreeMotion) {
    real dxt = (x0-beamX0)*initialBeamTangent[0] + 
      (y0-beamY0)*initialBeamTangent[1];
    real dyt = (x0-beamX0)*initialBeamNormal[0] + 
      (y0-beamY0)*initialBeamNormal[1];

    real xl = dxt+normall[0]*thickness;
    real yl = normall[1]*thickness+displacement;
    
    x = beamX0 + initialBeamTangent[0]*xl-initialBeamTangent[1]*yl;
    y = beamY0 - initialBeamNormal[0]*xl + initialBeamNormal[1]*yl;
  } else {

    real xbar = ((x0-beamX0)*initialBeamTangent[0]+
		 (y0-beamY0)*initialBeamTangent[1]-L*0.5);
    assert(xbar >= -L*0.6 && xbar <= L*0.6);
    x = centerOfMass[0] + normal[0] * displacement + xbar*tangent[0];
    y = centerOfMass[1] + normal[1] * displacement + xbar*tangent[1];

    x += (tangent[0] * normall[0] + normal[0]*normall[1])*thickness;
    y += (tangent[1] * normall[0] + normal[1]*normall[1])*thickness;
  }
    
}

void BeamModel::projectAcceleration(const real& x0,
				    const real& y0, real& ax, real& ay) {

  int elemNum;
  real eta, thickness;
  
  projectPoint(x0,y0, elemNum, eta,thickness);

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << thickness << std::endl;
  
  real displacement, slope;
  interpolateSolution(myPosition, elemNum, eta, displacement, slope);

  real Ddisplacement, Dslope;
  interpolateSolution(myVelocity, elemNum, eta, Ddisplacement, Dslope);

  real DDdisplacement, DDslope;
  interpolateSolution(myAcceleration, elemNum, eta, DDdisplacement, DDslope);
  
  real omag = 1./sqrt(slope*slope+1.0);
  real omag3 = omag*omag*omag;
  real omag5 = omag*omag*omag3;
  real normall[2] = {-slope*omag, omag};
  real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};
  real normaldd[2] = {3.0*slope*Dslope*omag5*Dslope - omag3*DDslope,
		      3.0*slope*Dslope*omag5*slope*Dslope-omag3*(Dslope*Dslope+slope*DDslope)};

  if (!allowsFreeMotion) {
    real axl = normaldd[0]*thickness;
    real ayl = normaldd[1]*thickness+DDdisplacement;
    
    ax = initialBeamTangent[0]*axl-initialBeamTangent[1]*ayl;
    ay = initialBeamNormal[0]*axl + initialBeamNormal[1]*ayl;
  } else {

    real xbar = ((x0-beamX0)*initialBeamTangent[0]+
		 (y0-beamY0)*initialBeamTangent[1]-L*0.5);
    ax = centerOfMassAcceleration[0] + 
      (normal[0]*DDdisplacement + (-tangent[0]*angularVelocity)*Ddisplacement + 
       (-tangent[0]*angularAcceleration-normal[0]*angularVelocity*angularVelocity)*displacement);
    ax += xbar*(normal[0]*angularAcceleration - tangent[0]*angularVelocity*angularVelocity);
    ax += thickness*(tangent[0]*normaldd[0] + normald[0]*normal[0]*angularVelocity+
		     (normal[0]*angularAcceleration - tangent[0]*angularVelocity*angularVelocity)*normall[0]);
    ax += thickness*(normal[0]*normaldd[1] - normald[1]*tangent[0]*angularVelocity+
		     (-tangent[0]*angularAcceleration - normal[0]*angularVelocity*angularVelocity)*normall[1]);
 
   
    ay = centerOfMassAcceleration[1] + 
      (normal[1]*DDdisplacement + (-tangent[1]*angularVelocity)*Ddisplacement + 
       (-tangent[1]*angularAcceleration-normal[1]*angularVelocity*angularVelocity)*displacement);
    ay += xbar*(normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity);

    ay += thickness*(tangent[1]*normaldd[0] + normald[0]*normal[1]*angularVelocity+
		     (normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity)*normall[0]);
    ay += thickness*(normal[1]*normaldd[1] - normald[1]*tangent[1]*angularVelocity+
		     (-tangent[1]*angularAcceleration - normal[1]*angularVelocity*angularVelocity)*normall[1]);
 
    //std::cout << "x0 = " << x0 << " y0 = " << y0 << " ax = " << ax << " ay = " << ay << std::endl;
  }
  /*
  if (fabs(ax) >= 1e-12) {
    std::cout << "ax = " << ax << " ay = " << ay << std::endl;
    std::cout << centerOfMassAcceleration[0] << " " << centerOfMassAcceleration[1] << std::endl;
    std::cout << angularAcceleration << " " << displacement << " " << Ddisplacement << " " << DDdisplacement << std::endl;
    }*/
}

void BeamModel::setDeclination(real dec) {

  beamInitialAngle = dec;

  initialBeamNormal[0] = -sin(dec);
  initialBeamNormal[1] = cos(dec);

  initialBeamTangent[0] = cos(dec);
  initialBeamTangent[1] = sin(dec);


  for (int k = 0; k < 2; ++k) {

    normal[k] = initialBeamNormal[k];
    tangent[k] = initialBeamTangent[k];
  }
}

void BeamModel::projectPoint(const real& x0,const real& y0,
			     int& elemNum, real& eta, real& thickness) {

  real xll = x0-beamX0;
  real yll = y0-beamY0;

  real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  //std::cout << "(" << x0 << ", " << y0 << ") " << xl << "--" << yl << " " << le << std::endl;

  // Assume the beam is oriented along the x axis.
  elemNum = (int)(xl / le);
  eta = 2.0*(xl-le*elemNum)/le-1.0;

  if (eta < -1.0)
    eta = -1.0;
  else if (eta > 1.0)
    eta = 1.0;

  if (elemNum >= numElem) {
    elemNum = numElem-1;
    eta = 1.0;
  }

  if (elemNum < 0) {

    elemNum = 0;
    eta = -1.0;
  }
     
 
  thickness = yl;
}

void BeamModel::interpolateSolution(const RealArray& X,
				    int& elemNum, real& eta,
				    real& displacement, real& slope) {

  // compute the shape functions.
  real eta1 = 1.-eta;
  real eta2 = 2.-eta;
  real etap1 = eta+1.0;
  real etap2 = eta+2.0;
  
  real sf[4] = {0.25*eta1*eta1*etap2,
		0.125*le*eta1*eta1*etap1,
		0.25*etap1*etap1*eta2,
		-0.125*le*eta1*etap1*etap1 };

  real sfd[4] = {(-0.5*eta1*etap2+0.25*eta1*eta1)/le,
		 -0.25*eta1*etap1+0.25*eta1*eta1,
		 (0.5*etap1*eta2-0.25*etap1*etap1)/le,
		 -0.25*etap1*eta1+0.25*etap1*etap1 };

		
  displacement = sf[0]*X(elemNum*2)+sf[1]*X(elemNum*2+1)+
    sf[2]*X(elemNum*2+2) +sf[3]*X(elemNum*2+3) ;
  slope = sfd[0]*X(elemNum*2)+sfd[1]*X(elemNum*2+1)+
    sfd[2]*X(elemNum*2+2) +sfd[3]*X(elemNum*2+3);
  
}

void BeamModel::interpolateThirdDerivative(const RealArray& X,
					   int& elemNum, real& eta,
					   real& deriv3) {

  // compute the shape functions.
  real eta1 = 1.-eta;
  real eta2 = 2.-eta;
  real etap1 = eta+1.0;
  real etap2 = eta+2.0;
  
  real sf[4] = {12.0/(le*le*le),6.0/(le*le), -12.0/ (le*le*le),6.0/(le*le)};

		
  deriv3 = sf[0]*X(elemNum*2)+sf[1]*X(elemNum*2+1)+
    sf[2]*X(elemNum*2+2) +sf[3]*X(elemNum*2+3) ;
  
}

void BeamModel::addForce(const real& x0_1, const real& y0_1,
			 real p1,const real& nx_1,const real& ny_1,
			 const real& x0_2, const real& y0_2,
			 real p2,const real& nx_2,const real& ny_2) {

  int elem1,elem2;
  real eta1,eta2,t1,t2;

  real p11,p22;

  //std::cout << x0_1 << " " << p1 << std::endl;
  
 // if (p1 != p1/* || p1 > 100.0*/) {
 //   //std::cout << "Found nan!" << std::endl;
 // }

  //std::cout << getExactPressure(t,x0_1) << " " << p1 << std::endl;
  //
  
  //p1 = getExactPressure(t,x0_1)*1000.0;
  //p2 = getExactPressure(t,x0_2)*1000.0;
  
  

  real xll = x0_1-beamX0;
  real yll = y0_1-beamY0;

  real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  real myx0 = xl;

  xll = x0_2-beamX0;
  yll = y0_2-beamY0;

  xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  real myx1 = xl;

  if (myx1 > myx0) {

    projectPoint(x0_1,y0_1,elem1, eta1,t1); 
    projectPoint(x0_2,y0_2,elem2, eta2,t2);   
    p11 = p1*pressureNorm;
    p22 = p2*pressureNorm;
  } else {

    projectPoint(x0_1,y0_1,elem2, eta2,t2); 
    projectPoint(x0_2,y0_2,elem1, eta1,t1);  
    p22 = p1*pressureNorm;
    p11 = p2*pressureNorm; 
    std::swap<real>(myx0,myx1);
  }

  //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << p1 << " " << p2 << std::endl;

  real dx = fabs(myx0-myx1);

  
  for (int i = elem1; i <= elem2; ++i) {

    real a = eta1,b = eta2;
    real pa = p11, pb = p22;
    real x0 = myx0, x1 = myx1;
    if (i != elem1) {
      a = -1.0;
      x0 = le*i;
      pa = p11 + (p22-p11)*(x0-myx0)/(dx);
    }
    if (i != elem2) {
      b = 1.0;
      x1 = le*(i+1);
      pb = p11 + (p22-p11)*(x1-myx0)/(dx);
    }
    
    Index idx(i*2,4);
    RealArray lt;
    if (t1 > 0) 
    { // *wdh* Turn this back on 2014/05/23
      pa = -pa;
      pb = -pb;
    }
    
      
    //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
    //  p1 << " " << p2 << std::endl;
  

    lt.redim(4);
    if (fabs(b-a) > 1.0e-10) {
      computeProjectedForce(pa,pb, a,b, lt);
    //    std::cout << "a = " << a << " b = " << b << std::endl;
      myForce(idx) += lt;

      real gradp = 1.0;
      totalPressureForce += (lt(0)+lt(2));
      totalPressureMoment += (lt(0)*(le*i-0.5*L)+lt(1)*gradp+lt(2)*(le*(i+1)-0.5*L) + lt(3)*gradp);
    }
    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  }
}

void BeamModel::resetForce() {

  myForce = 0.0;

  totalPressureForce = 0.0;
  totalPressureMoment = 0.0;
}

  /*
  myForce = 0.0;
  for (int i = 0; i < numElem; ++i) {
    Index idx(i*2,4);
    RealArray lt;
    lt.redim(4);
    computeProjectedForce(-2.0*0.02*1000.0,-2.0*0.02*1000.0,-1.0,1.0, lt);
    myForce(idx) += lt;
  }
  */

void BeamModel::recomputeNormalAndTangent() {

  normal[0] = -sin(angle);
  normal[1] = cos(angle);

  tangent[0] = cos(angle);
  tangent[1] = sin(angle);
}

// Return the current force of the structure.
//
const RealArray& BeamModel::force() const
{
  return myForce;
}




void BeamModel::predictor(real dt, const RealArray& x1, const RealArray& v1, 
			  const RealArray& x2, const RealArray& v2,
			  RealArray& x3, RealArray& v3) {

  t += dt;
  //myForce = 0.0;

  if( false && t<2.*dt )
  { 
    // wdh: debug info: 
    printF("************** BeamModel::predictor: t=%9.3e ***********************\n",t);
    ::display(x2,"x2","%8.2e ");
    ::display(v2,"v2","%8.2e ");
    
  }
  


  for (int i = 0; i < numElem; ++i) {
    Index idx(i*2,4);
    RealArray lt;
    lt.redim(4);
    computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
			  -1.0,1.0, lt);
    myForce(idx) += lt;
  }

  if( debug & 1 )
  {
    ::display(myForce(Range(0,2*numElem,2)),"BeamModel::predictor: force (displacement)","%8.2e ");
  }
  

  //printArray(myForce,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
 
  if (!hasAcceleration) {
    computeAcceleration(x2,v2,myForce, elementM, myAcceleration,
			centerOfMassAcceleration, angularAcceleration,
			dt);
    hasAcceleration = true;
  }

  RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);

  dtilde = x2+dt*v2+dt*dt*0.5*(1.0-2.0*newmarkBeta)*myAcceleration;
  vtilde = v2+dt*(1.0-newmarkGamma)*myAcceleration;

  if (allowsFreeMotion) {
    
    comXtilde[0] = centerOfMass[0] + dt*centerOfMassVelocity[0] +
      dt*dt*0.5*(1.0-2.0*newmarkBeta)*centerOfMassAcceleration[0];
    comXtilde[1] = centerOfMass[1] + dt*centerOfMassVelocity[1] +
      dt*dt*0.5*(1.0-2.0*newmarkBeta)*centerOfMassAcceleration[1];


    comVtilde[0] = centerOfMassVelocity[0] +
      dt*(1.0-newmarkGamma)*centerOfMassAcceleration[0];
    comVtilde[1] = centerOfMassVelocity[1] +
      dt*(1.0-newmarkGamma)*centerOfMassAcceleration[1];
    
    angletilde = angle + dt*angularVelocity + 
      dt*dt*0.5*(1.0-2.0*newmarkBeta)*angularAcceleration;
    angularVelocityTilde = angularVelocity + 
      dt*(1.0-newmarkGamma)*angularAcceleration;

    recomputeNormalAndTangent();
    std::cout << "t = " << t;
    std::cout << " CoM V = " << centerOfMassVelocity[0] << " " << centerOfMassVelocity[1];
    std::cout << " CoM A = " << centerOfMassAcceleration[0] << " " << centerOfMassAcceleration[1];
    std::cout << " CoM W = " << centerOfMass[0] << " " << centerOfMass[1] << std::endl;

    std::cout << "Angle = " << angle << " ang. velocity = " << angularVelocity << 
      " angularAcceleration = " << angularAcceleration << std::endl;

    if (bcLeft == Pinned || bcLeft == Cantilevered) {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(myPosition, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      std::cout << "End error = " << end[0] - initialEndLeft[0] << " " <<  end[1] - initialEndLeft[1] << std::endl;
    }
  }

  aold = 0.0;

  memset(old_rb_acceleration,0,sizeof(old_rb_acceleration));

  //printArray(x2,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  //setExactSolution(t, dtilde, vtilde, myAcceleration);

  //x3 = x2+dt*v2+dt*dt*0.5*(1.0-2.0*newmarkBeta)*myAcceleration;
  //v3 = v2+(1.0-newmarkGamma)*dt*myAcceleration;
  //x3 = x2+dt*v2+dt*dt*0.5*myAcceleration;

  //v3 = v2+dt*myAcceleration;
  /*
  computeAcceleration(dtilde,v3,myForce, A, myAcceleration, dt);
  v3 = vtilde+newmarkGamma*dt*myAcceleration;
  x3 = dtilde+newmarkBeta*dt*dt*myAcceleration;
  
  if (bcLeft == Cantilevered) {
    x3(0) = x3(1) = 0.0;
    v3(0) = v3(1) = 0.0;
  }

  if (bcRight == Cantilevered) {
    x3(numElem*2) = x3(numElem*2+1) = 0.0;
    v3(numElem*2) = v3(numElem*2+1) = 0.0;
  }

  if (bcLeft == Pinned) {
    x3(0) = 0.0;
    v3(0) =  0.0;
  }

  if (bcRight == Pinned) {
    x3(numElem*2) = 0.0;
    v3(numElem*2) = 0.0;
  }
  
  //output << t << " " <<  x3(numElem*2) << " " << v3(numElem*2) << std::endl;
    
  std::cout << "Tip displacement: " << x3(numElem*2) << std::endl;
  std::cout << "Tip acceleration: " << myAcceleration(numElem*2) << std::endl;

  myPosition = x3;
  myVelocity = v3;

  myPosition_nm1 = x2;
  myVelocity_nm1 = v2;*/

  v3 = vtilde;
  x3 = dtilde;
  
  if (bcLeft == Cantilevered && !allowsFreeMotion) {
    x3(0) = x3(1) = 0.0;
    v3(0) = v3(1) = 0.0;
  }

  if (bcRight == Cantilevered && !allowsFreeMotion) {
    x3(numElem*2) = x3(numElem*2+1) = 0.0;
    v3(numElem*2) = v3(numElem*2+1) = 0.0;
  }

  if (bcLeft == Pinned && !allowsFreeMotion) {
    x3(0) = 0.0;
    v3(0) =  0.0;
  }

  if (bcRight == Pinned && !allowsFreeMotion) {
    x3(numElem*2) = 0.0;
    v3(numElem*2) = 0.0;
  }
  
  //myAcceleration = 0.0;

  //x3 = 0.0;
  //v3 = 0.0;

  if( dbase.get<bool>("saveTipFile") )
  {
    output << t << " " <<  x3(numElem*2) << " " << v3(numElem*2) << " " <<  myAcceleration(numElem*2) << std::endl;
  }
  
  RealArray xtmp = x3;
  RealArray vtmp = v3;
  RealArray atmp = myAcceleration;

  const bool & saveProfileFile = dbase.get<bool>("saveProfileFile");

  if( usesExactSolution && saveProfileFile ) 
  {
    setExactSolution(t,xtmp,vtmp,atmp);

    std::stringstream profname;
    profname << "beam_profile" << time_step_num << ".txt";
    std::ofstream beam_profile(profname.str().c_str());
    for (int i = 0; i < 100; ++i) {
      
      double x = (double)i / 99*L;
      int elemNum;
      real eta, thickness;
      
      projectPoint(beamX0+x,beamY0, elemNum, eta,thickness);
      
      real displacement, slope;
      interpolateSolution(x3, elemNum, eta, displacement, slope);
      beam_profile << x << " " << displacement <<  " ";
      interpolateSolution(v3, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      interpolateSolution(myAcceleration, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      
      //if (usesExactSolution) {
      interpolateSolution(xtmp, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      interpolateSolution(vtmp, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      interpolateSolution(atmp, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      //}
      
      beam_profile << std::endl;
      
    }

    beam_profile.close();

    std::cout << "Wrote location for time " << t << " to " << profname.str() << std::endl;

  }
  time_step_num++;
    /*
    
  }

  /*
  std::cout << "End angle displacement: " << x3(1) << std::endl;
  std::cout << "End angle velocity: " << v3(1) << std::endl;
  std::cout << "End angle acceleration: " << myAcceleration(1) << std::endl;

  std::cout << "Tip angle displacement: " << x3(numElem*2+1) << std::endl;
  std::cout << "Tip angle velocity: " << v3(numElem*2+1) << std::endl;
  std::cout << "Tip angle acceleration: " << myAcceleration(numElem*2+1) << std::endl;

  std::cout << "End displacement: " << x3(0) << std::endl;
  std::cout << "End acceleration: " << myAcceleration(0) << std::endl;

  std::cout << "Tip displacement: " << x3(numElem*2) << std::endl;
  std::cout << "Tip acceleration: " << myAcceleration(numElem*2) << std::endl;
  */
  myPosition_nm1 = x2;
  myVelocity_nm1 = v2;

  myPosition = x3;
  myVelocity = v3;

  numCorrectorIterations = 0;
}

void BeamModel::corrector(real dt,
			  RealArray& x3, RealArray& v3) {
  
  RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);
    
  //myForce = 0.0;
    for (int i = 0; i < numElem; ++i) {
    Index idx(i*2,4);
    RealArray lt;
    lt.redim(4);
    computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
			  -1.0,1.0, lt);
    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    myForce(idx) += lt;
    }
  
  //myForce = 0.0;

  if (time_step_num == 1) {
    correctionHasConverged = true;
    x3 = myPosition;
    v3 = myVelocity;
    centerOfMassAcceleration[0] = buoyantMass / totalMass * bodyForce[0];
    centerOfMassAcceleration[1] = buoyantMass / totalMass * bodyForce[1];

    return;
  }

  real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");

  real omega = addedMassRelaxationFactor;

  flocal = myForce;

  //if (t == 0.0)
  //  flocal = 0.0;
  real linaccel[2],omegadd;
  computeAcceleration(dtilde,v3,flocal, A, myAcceleration,
		      linaccel,omegadd,dt, newmarkBeta, newmarkGamma);

  //v3 = vtilde+newmarkGamma*dt*(myAcceleration-aold)*omega;
  //x3 = dtilde+newmarkBeta*dt*dt*(myAcceleration-aold)*omega;

  v3 = vtilde+newmarkGamma*dt*myAcceleration;
  x3 = dtilde+newmarkBeta*dt*dt*myAcceleration;

  if (allowsFreeMotion) {

    centerOfMassAcceleration[0] = linaccel[0];
    centerOfMassAcceleration[1] = linaccel[1];
    
    centerOfMassVelocity[0] = comVtilde[0] + newmarkGamma*dt*linaccel[0];
    centerOfMassVelocity[1] = comVtilde[1] + newmarkGamma*dt*linaccel[1];
    
    centerOfMass[0] = comXtilde[0] + newmarkBeta*dt*dt*linaccel[0];
    centerOfMass[1] = comXtilde[1] + newmarkBeta*dt*dt*linaccel[1];

    angularAcceleration = omegadd;
    angularVelocity = angularVelocityTilde + dt*newmarkGamma*angularAcceleration;
    angle = angletilde + dt*dt*newmarkBeta*angularAcceleration;
    
    recomputeNormalAndTangent();
    
  }

  //printArray(x3,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  tmp = evaluate(myAcceleration-aold);

  correctionHasConverged = false;
  double correction = 0.0;
  for (int i = 0; i < numElem; ++i) {
    
    correction += tmp(i*2)*tmp(i*2)/(le*le)+
      tmp(i*2+1)*tmp(i*2+1);
  }
  
  if (allowsFreeMotion) {

    real tmpp[3];
    tmpp[0] = old_rb_acceleration[0]-centerOfMassAcceleration[0];
    tmpp[1] = old_rb_acceleration[1]-centerOfMassAcceleration[1];

    tmpp[2] = old_rb_acceleration[2]-angularAcceleration;

    for (int k = 0; k < 3; ++k)
      correction += tmpp[k]*tmpp[k];
  }

  myAcceleration = omega*myAcceleration+(1.0-omega)*aold;
  /*
  myAcceleration = 0.0;
  x3 = 0.0;
  v3 = 0.0;
  */
  if (allowsFreeMotion) {
    
    centerOfMassAcceleration[0] = omega*centerOfMassAcceleration[0]+
      (1.0-omega)*old_rb_acceleration[0];
    
    centerOfMassAcceleration[1] = omega*centerOfMassAcceleration[1]+
      (1.0-omega)*old_rb_acceleration[1];

    angularAcceleration = omega*angularAcceleration + 
      (1.0-omega)*old_rb_acceleration[2];
    
    old_rb_acceleration[0] = centerOfMassAcceleration[0];
    old_rb_acceleration[1] = centerOfMassAcceleration[1];
    old_rb_acceleration[2] = angularAcceleration;
  }
    
  

  aold = myAcceleration;
  
  correction = sqrt(correction);

  if (numCorrectorIterations == 0)
    initialResidual = correction;

  ++numCorrectorIterations;

  // std::cout << "correction value = " << correction << std::endl;
  if (correction < initialResidual*subIterationConvergenceTolerance || correction < 1e-8)
    correctionHasConverged = true;

  //setExactSolution(t, x3, v3, myAcceleration);

  if (bcLeft == Cantilevered && !allowsFreeMotion) {
    x3(0) = x3(1) = 0.0;
    v3(0) = v3(1) = 0.0;
  }
  
  if (bcRight == Cantilevered && !allowsFreeMotion) {
    x3(numElem*2) = x3(numElem*2+1) = 0.0;
    v3(numElem*2) = v3(numElem*2+1) = 0.0;
  }

  if (bcLeft == Pinned && !allowsFreeMotion) {
    x3(0) = 0.0;
    v3(0) =  0.0;
  }

  if (bcRight == Pinned && !allowsFreeMotion) {
    x3(numElem*2) = 0.0;
    v3(numElem*2) = 0.0;
  }
  
  myPosition = x3;
  myVelocity = v3;
}

const RealArray& BeamModel::position() const  {

  return myPosition;
}

const RealArray& BeamModel::velocity() const  {

  return myVelocity;
}

bool BeamModel::hasCorrectionConverged() const {

  //return true;
  return correctionHasConverged;
}

// Include complex down here to minimize name conflicts
#include <complex>

typedef ::real LocalReal;
typedef ::real OV_real;

static std::complex<LocalReal> phi1(std::complex<LocalReal> alpha, LocalReal k,
				    LocalReal y) {

  return 0.5*(cosh(alpha*y)-cosh(k*y));
}

static std::complex<LocalReal> phi2(std::complex<LocalReal> alpha, LocalReal k,
				    LocalReal y) {

  return 0.5*(k*sinh(alpha*y)-alpha*sinh(k*y));
}

static std::complex<LocalReal> phi1d(std::complex<LocalReal> alpha, LocalReal k,
				    LocalReal y) {

  return 0.5*(alpha*sinh(alpha*y)-k*sinh(k*y));
}

static std::complex<LocalReal> phi2d(std::complex<LocalReal> alpha, LocalReal k,
				    LocalReal y) {

  return 0.5*k*alpha*(cosh(alpha*y)-cosh(k*y));
}



void BeamModel::exactSolutionVelocity(LocalReal x, LocalReal y,
				      LocalReal t,
				      LocalReal k, LocalReal H, 
				      LocalReal omega_real, LocalReal omega_imag,
				      LocalReal omega0, LocalReal nu,
				      LocalReal what,   // not needed
				      LocalReal& u, LocalReal& v) {
  
  what=exactSolutionScaleFactorFSI;  // wdh 
  //  printF("@@@BeamModel::exactSolutionVelocity what=%9.3e\n",what);

  std::complex<LocalReal> omega_tilde(omega_real, omega_imag);
  LocalReal beta = omega0/(k*k)/nu;
  std::complex<LocalReal> I(0.0,1.0);
  std::complex<LocalReal> alpha = k*sqrt(-I*beta*omega_tilde+1.0);

  std::complex<LocalReal> a = phi2d(alpha,k,H),b = phi1d(alpha,k,H);
  std::complex<LocalReal> vhat = -what*I*omega_tilde*omega0*
    (-a*phi1(alpha,k,y)+b*phi2(alpha,k,y))/
    (-a*phi1(alpha,k,H)+b*phi2(alpha,k,H));

  std::complex<LocalReal> uhat = what*omega_tilde*omega0/k*
    (-a*phi1d(alpha,k,y)+b*phi2d(alpha,k,y))/
    (-a*phi1(alpha,k,H)+b*phi2(alpha,k,H));

  std::complex<LocalReal> U = uhat*(exp(I*k*x-I*omega_tilde*omega0*t)+exp(-I*k*x-I*omega_tilde*omega0*t));
  std::complex<LocalReal> V = vhat*(exp(I*k*x-I*omega_tilde*omega0*t)-exp(-I*k*x-I*omega_tilde*omega0*t));

  //std::cout << y << " " << V << std::endl;

  u = 2.0*U.real();
  v = 2.0*V.real();

  
}


void BeamModel::exactSolutionPressure(LocalReal x, LocalReal y,
				      LocalReal t,
				      LocalReal k, LocalReal H, 
				      LocalReal omega_real, LocalReal omega_imag,
				      LocalReal omega0, LocalReal nu,
				      LocalReal what, // not needed
				      LocalReal& p) {
  
  what=exactSolutionScaleFactorFSI;  // wdh 

  std::complex<LocalReal> omega_tilde(omega_real, omega_imag);
  LocalReal beta = omega0/(k*k)/nu;
  std::complex<LocalReal> I(0.0,1.0);
  std::complex<LocalReal> alpha = k*sqrt(-I*beta*omega_tilde+1.0);

  std::complex<LocalReal> a = phi2d(alpha,k,H),b = phi1d(alpha,k,H);
  std::complex<LocalReal> phat = what*omega_tilde*omega0*omega_tilde*omega0*1.0/(2.0*k)*
    (a*sinh(k*y)-alpha*b*cosh(k*y))/
    (-a*phi1(alpha,k,H)+b*phi2(alpha,k,H));

  std::complex<LocalReal> P = phat*(exp(I*k*x-I*omega_tilde*omega0*t)-exp(-I*k*x-I*omega_tilde*omega0*t));

  //if (y == H)
  // std::cout <<x << " " <<2.0*P.real() << " " <<  getExactPressure(t, x);

  p = 2.0*P.real();
  
}

void BeamModel::setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) {
  
  // printF("@@@BeamModel::setExactSolution exactSolutionScaleFactorFSI=%9.3e\n", dbase.get<OV_real>("exactSolutionScaleFactorFSI"));

  double h=0.02;
  double Ioverb=6.6667e-7;
  double H=0.3;
  double k=2.0*3.141592653589/L;
  double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
  // -- wHat determines the "amplitude" of the surface, scaled by some factor ...wdh
  double what = exactSolutionScaleFactorFSI; // 0.00001;
  double omegar = 0.8907148069, omegai = -0.9135887123e-2;
  std::complex<LocalReal> omega_tilde(omegar, omegai);
  std::complex<LocalReal> I(0.0,1.0);

  for (int i = 0; i <= numElem; ++i) {

    double xl = (double)i / numElem*L;
    
    std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);
    std::complex<LocalReal> fp = I*k*(exp(I*k*xl-I*omega_tilde*omega0*t)+exp(-I*k*xl-I*omega_tilde*omega0*t));
    

    
    x(i*2) = 2.0*(what*f).real();      // displacement w ...wdh
    x(i*2+1) = 2.0*(what*fp).real();   // slope w'  ...wdh
    
    v(i*2) = 2.0*(-what*f*I*omega_tilde*omega0).real();
    v(i*2+1) = 2.0*(-what*fp*I*omega_tilde*omega0).real();
    
    a(i*2) = 2.0*(-what*f*omega_tilde*omega0*omega_tilde*omega0).real();
    a(i*2+1) = 2.0*(-what*fp*omega_tilde*omega0*omega_tilde*omega0).real();
  }
}

double BeamModel::getExactPressure(double t, double xl) {

  printF("@@@BeamModel::getExactPressure exactSolutionScaleFactorFSI=%9.3e\n", dbase.get<OV_real>("exactSolutionScaleFactorFSI"));

  double h=0.02;
  double Ioverb=6.6667e-7;
  double nu = 0.001;
  double H=0.3;
  std::complex<LocalReal> I(0.0,1.0);
  double omegar = 0.8907148069, omegai = -0.9135887123e-2;
  std::complex<LocalReal> omega_tilde(omegar, omegai);
  double k=2.0*3.141592653589/L;
  double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
  LocalReal beta = omega0/(k*k)/nu;
  std::complex<LocalReal> alpha = k*sqrt(-I*beta*omega_tilde+1.0);

  double what = exactSolutionScaleFactorFSI; // 0.00001;
  std::complex<LocalReal> omega = omega_tilde*omega0;
  
  
  std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);

  std::complex<LocalReal> a = phi2d(alpha,k,H),b = phi1d(alpha,k,H);
  std::complex<LocalReal> A = -1.0*what*omega*omega*alpha*0.5/k;
  std::complex<LocalReal> c = a/b;
  A /= (-c*phi1(alpha,k,H)+phi2(alpha,k,H));
  std::complex<LocalReal> phat = A*(cosh(k*H)-c/alpha*sinh(k*H));

  std::cout << xl << " " << 2.0*(f*phat).real() << std::endl;

  LocalReal result = 2.0*(f*phat).real();

  std::complex<LocalReal> r1 = (elasticModulus*Ioverb*k*k*k*k-density*h*omega*omega)*what, r2 = phat*1000.0 ;
  std::cout << r1 << " " << r2 << std::endl;

  return result;

}


void BeamModel::setAddedMassRelaxation(double omega) 
{

  dbase.get<OV_real>("addedMassRelaxationFactor") = omega;
}

void BeamModel::setSubIterationConvergenceTolerance(double tol) 
{
  dbase.get<OV_real>("subIterationConvergenceTolerance") = tol;
}



// =================================================================================================
/// \brief  Define the BeamModel parameters interactively.
// =================================================================================================
int BeamModel::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  // *********** FINISH ME ****************

  // DeformingBodyType & deformingBodyType = 
  //                 deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  // if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
  //   deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);
  // UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
  //                deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");
  

  // // --- here are the parameters used by the surface smoother ---
  // if( !deformingBodyDataBase.has_key("smoothSurface") )
  // {
  //   deformingBodyDataBase.put<bool>("smoothSurface",false);
  //   deformingBodyDataBase.put<int>("numberOfSurfaceSmooths",3);
  // }
  // bool & smoothSurface = deformingBodyDataBase.get<bool>("smoothSurface");
  // int & numberOfSurfaceSmooths = deformingBodyDataBase.get<int>("numberOfSurfaceSmooths");
  // bool & changeHypeParameters = deformingBodyDataBase.get<bool>("changeHypeParameters");
  
  // // -- Boundary conditions ---
  // if( !deformingBodyDataBase.has_key("boundaryCondition") )
  // {
  //   deformingBodyDataBase.put<BcArray>("boundaryCondition");
  //   BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");
  //   for( int side=0; side<=1; side++ )for( int axis=0; axis<2; axis++ )
  //   {
  //     boundaryCondition(side,axis)=dirichletBoundaryCondition;
  //   }
  // }
  // BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");
  
  GUIState gui;
  gui.setWindowTitle("Beam Model");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  OV_real & subIterationConvergenceTolerance = dbase.get<OV_real>("subIterationConvergenceTolerance");
  OV_real & addedMassRelaxationFactor = dbase.get<OV_real>("addedMassRelaxationFactor");


  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    // aString pbLabels[] = {"elastic shell options...",
    // 			  "grid evolution parameters...",
    // 			  "help",
    // 			  ""};
    // addPrefix(pbLabels,prefix,cmd,maxCommands);
    // int numRows=4;
    // dialog.setPushButtons( cmd, pbLabels, numRows ); 

    dialog.setOptionMenuColumns(1);
    aString bcOptions[] = { "cantilever",
			    "pinned",
			    "free",
                            "periodic",
			    "" };

    GUIState::addPrefix(bcOptions,"bc left:",cmd,maxCommands);
    dialog.addOptionMenu("BC left:",cmd,cmd,bcLeft );

    GUIState::addPrefix(bcOptions,"bc right:",cmd,maxCommands);
    dialog.addOptionMenu("BC right:",cmd,cmd,bcRight );


    aString tbCommands[] = {"save profile file",
                            "save tip file",
    			    ""};
    int tbState[10];
    tbState[0] = dbase.get<bool>("saveProfileFile");
    tbState[1] = dbase.get<bool>("saveTipFile");
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "name:"; sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 
    textLabels[nt] = "number of elements:"; sPrintF(textStrings[nt], "%i",numElem);  nt++; 
    textLabels[nt] = "area moment of inertia:"; sPrintF(textStrings[nt], "%g",areaMomentOfInertia);  nt++; 
    textLabels[nt] = "elastic modulus:"; sPrintF(textStrings[nt], "%g",elasticModulus);  nt++; 
    textLabels[nt] = "density:"; sPrintF(textStrings[nt], "%g",density);  nt++; 
    textLabels[nt] = "thickness:"; sPrintF(textStrings[nt], "%g",thickness);  nt++; 
    textLabels[nt] = "length:"; sPrintF(textStrings[nt], "%g",L);  nt++; 
    textLabels[nt] = "pressure norm:"; sPrintF(textStrings[nt], "%g",pressureNorm);  nt++; 
    textLabels[nt] = "initial declination:"; sPrintF(textStrings[nt], "%g (degrees)",beamInitialAngle*180./Pi);  nt++; 
    textLabels[nt] = "position:"; sPrintF(textStrings[nt], "%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0);  nt++; 

    textLabels[nt] = "added mass relaxation:"; sPrintF(textStrings[nt], "%g",addedMassRelaxationFactor);  nt++; 
    textLabels[nt] = "added mass tol:"; sPrintF(textStrings[nt], "%g",subIterationConvergenceTolerance);  nt++; 

    textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",debug);  nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    // dialog.setTextBoxes(cmd, textLabels, textStrings);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  }
  
  aString answer,buff;

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("beam>");
  int len=0;
  for( ;; ) 
  {
	    
    gi.getAnswer(answer,"");
  
    // printF(answer,"answer=[answer]\n",(const char *)answer);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);


    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    else if( dialog.getTextValue(answer,"name:","%s",name) ){} //
    else if( dialog.getTextValue(answer,"number of elements:","%i",numElem) ){} //

    else if( dialog.getTextValue(answer,"area moment of inertia:","%g",areaMomentOfInertia) ){} //

    else if( dialog.getTextValue(answer,"elastic modulus:","%g",elasticModulus) ){} //
    else if( dialog.getTextValue(answer,"density:","%g",density) ){} //
    else if( dialog.getTextValue(answer,"thickness:","%g",thickness) ){} //
    else if( dialog.getTextValue(answer,"length:","%g",L) ){} //
    else if( dialog.getTextValue(answer,"pressure norm:","%g",pressureNorm) ){} //
    else if( dialog.getTextValue(answer,"added mass relaxation:","%g",addedMassRelaxationFactor) )
    {
      printF("The relaxation parameter used in the fixed point iteration\n"
             " used to alleviate the added mass effect\n");
    }

    else if( dialog.getTextValue(answer,"added mass tol:","%g",subIterationConvergenceTolerance) )
    {
      printF("The (relative) convergence tolerance for the fixed point iteration\n"
	     " tol: convergence tolerance (default is 1.0e-3)\n");
    }

    else if( dialog.getTextValue(answer,"initial declination:","%g",beamInitialAngle) )
    {  
      setDeclination(beamInitialAngle*Pi/180.);
      printF("INFO: The beam will be inclined %8.4f degrees from the left end\n",beamInitialAngle*180./Pi);
      dialog.setTextLabel("initial declination:",sPrintF(buff,"%g, (degrees)",beamInitialAngle*180./Pi));
    } 
    else if( (len=answer.matches("position:")) )
    {
      sScanF(answer(len,answer.length()-1),"%e %en %e",&beamX0,&beamY0,&beamZ0);
      printF("INFO: Setting the position of the left end of the beam to (%e,%e,%e)\n",beamX0,beamY0,beamZ0);
      dialog.setTextLabel("position:",sPrintF(buff,"%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0));
    }
    else if( answer.matches("bc left:") ||
             answer.matches("bc right:") )
    {
      // Assign BC's 

      aString bcOption;
      int side=0;
      if( (len=answer.matches("bc left:")) )
	side=0;
      else if( (len=answer.matches("bc right:")) )
	side=1;
      else
      {
	OV_ABORT("error");
      }
      
      bcOption = answer(len,answer.length()-1);
      BoundaryCondition & bcValue = side==0 ? bcLeft : bcRight;
      
      bcValue= (bcOption=="cantilever" ? Cantilevered :
                bcOption=="pinned"     ? Pinned :
                bcOption=="free"       ? Free : 
                bcOption=="periodic"   ? Periodic : UnknownBC );

      if( bcValue==UnknownBC )
      {
	printF("ERROR: unknown BC : answer=[%s], bcOption=[%s]\n",(const char*)answer,(const char*)bcOption);
	gi.stopReadingCommandFile();
      }

      printF("BeamModel:INFO: setting %s = %s.\n",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcOption);

    }
    else if( dialog.getToggleValue(answer,"save profile file",dbase.get<bool>("saveProfileFile")) ){} // 
    else if( dialog.getToggleValue(answer,"save tip file",dbase.get<bool>("saveTipFile")) )
    {
      aString tipFileName = sPrintF(buff,"%s_tip.text",(const char*)name);
      output.open(name);
      printF("BeamModel: tip position info will be saved to file 'tip.txt'\n");
    }
    else
    {
      printF("BeamModel::update:ERROR:unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    // else if( answer=="elastic beam parameters" )
    // {
    //   if( !deformingBodyDataBase.has_key("elasticBeamParameters") )
    //   {
    // 	deformingBodyDataBase.put<real [10]>("elasticBeamParameters");

    // 	real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
    // 	par[0]=0.02*0.02*0.02/12.0; // area moment of inertia
    // 	par[1]=1.4e6;  // elastic modulus
    // 	par[2]=1e4;  // density 
    // 	par[3]=0.35;  // length 
    // 	par[4]=0.02;  // thickness
    // 	par[5]=1000.0;  // pressure norm
    // 	par[6]=0.0;  // x0
    // 	par[7]=0.3;  // y0
    // 	par[8]=0.0;  // declination

    // 	deformingBodyDataBase.put<int [10]>("elasticBeamIntegerParameters");
	
    // 	int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
    // 	ipar[0] = 15;
    // 	ipar[1] = 0;
    // 	ipar[2] = 0;
    // 	ipar[3] = 0;
    //   }

    //   real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");

    //   gi.inputString(answer,sPrintF("Enter I,E,rho,L,t,pnorm,x0,y0,dec, scaleFactor (default=(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
    // 				    par[0],par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],BeamModel::exactSolutionScaleFactorFSI));
    //   sScanF(answer,"%e %e %e %e %e %e %e %e %e %e",&par[0],&par[1],&par[2],&par[3],&par[4],&par[5],&par[6],&par[7],&par[8],&BeamModel::exactSolutionScaleFactorFSI);
    //   printF("Setting I=%g, E=%e, rho=%e, L=%e t=%e pnorm=%e x0=%e y0=%e dec=%e scaleFactor=%e for the elastic beam\n",
    // 	     par[0],par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],BeamModel::exactSolutionScaleFactorFSI);

    //   int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");

    //   gi.inputString(answer,sPrintF("Enter nelem, bcl, bcr, exact (default=(%d,%d,%d,%d) (0=cantilevered, 1=pinned, 2=free)",ipar[0],ipar[1],ipar[2],ipar[3]));
    //   sScanF(answer,"%d %d %d %d",&ipar[0],&ipar[1],&ipar[2],&ipar[3]);
    //   printF("Setting nelem=%d, bcl=%d, bcr=%d, exact=%d for the elastic beam\n",ipar[0],ipar[1],ipar[2],ipar[3]);

    // }
    // else if( answer=="sub iteration convergence tolerance" )
    // {
    //   if (!deformingBodyDataBase.has_key("sub iteration convergence tolerance")) {
    // 	deformingBodyDataBase.put<real>("sub iteration convergence tolerance");
    // 	real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");
    // 	tol = 1e-3;
    //   }
      
    //   real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");

    //   gi.inputString(answer,sPrintF("Enter tol (default=%g)",tol));
    //   sScanF(answer,"%e",&tol);
    //   printF("Setting convergence tolerance = %g\n",tol);
	
    // }
    // else if( answer=="added mass relaxation factor" )
    // {
    //   if (!deformingBodyDataBase.has_key("added mass relaxation factor")) {
    // 	deformingBodyDataBase.put<real>("added mass relaxation factor");
    // 	real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
    // 	omega = 1.0;
    //   }
      
    //   real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");

    //   gi.inputString(answer,sPrintF("Enter omega (default=%g)",omega));
    //   sScanF(answer,"%e",&omega);
    //   printF("Setting added mass relaxation factor = %g\n",omega);
	
    // }


    // else if( answer=="beam free motion" )
    // {
    //   if (!deformingBodyDataBase.has_key("beam free motion")) {
    // 	//real beamFreeMotionParams[] = {0.0,0.0,0.0};
    // 	deformingBodyDataBase.put<real[3]>("beam free motion");
    // 	real* p = deformingBodyDataBase.get<real [3]>("beam free motion");
    // 	memset(p,0,sizeof(real)*3);
    //   }

    //   real* beamFreeMotionParams = deformingBodyDataBase.get<real [3]>("beam free motion");
    //   gi.inputString(answer,sPrintF("Enter beam free motion parameters x0,y0,angle0 (default=%g,%g,%g)",
    // 				    beamFreeMotionParams[0],beamFreeMotionParams[1],beamFreeMotionParams[2]));
    //   sScanF(answer,"%e %e %e",
    // 	     &beamFreeMotionParams[0],
    // 	     &beamFreeMotionParams[1],
    // 	     &beamFreeMotionParams[2]);
    //   printF("Setting beam free motion parameters= x0=%g,y0=%g,angle0=%g\n",
    // 	     beamFreeMotionParams[0],beamFreeMotionParams[1],beamFreeMotionParams[2]);
    // }



    // else if( dialog.getToggleValue(answer,"smooth surface",smoothSurface) )
    // {
    //   if( smoothSurface )
    // 	printF("Surface smoothing is on. The deforming surface will be smoothed with a 4th-order filter.\n"
    //            "  You may also set `number of surface smooths' to define the number of smoothing iterations\n");
    //   else
    // 	printF("Surface smoothing is off\n");
    // }

    
  }
    
  // -- initialize the beam model given the current parameters --
  initialize();

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  //  pBeamModel->setParameters(I, Em, rho, L, thick, pnorm, nelem, bcl, bcr,x0,y0,(exact==1));


  return 0;

}
