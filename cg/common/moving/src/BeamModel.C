//                                   -*- c++ -*-
#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"

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


// ======================================================================================================
/// \brief Constructor.
//
// ======================================================================================================
BeamModel::BeamModel() 
{

  name = "beam";

  globalBeamCounter++;
  beamID=globalBeamCounter; //  a unique ID 
  
  domainDimension=1;        // domain dimension
  numberOfDimensions=2;     // range dimension

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

  dbase.put<real>("tension")=0.;  // T : coefficient of w_xx

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

  useExactSolution=false;


  // The relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  dbase.put<real>("addedMassRelaxationFactor",1.0);  // 1 = no relaxation

  // The (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  dbase.put<real>("subIterationConvergenceTolerance",1.0e-3);
  
  // For initial conditions: 
  dbase.put<real>("amplitude")=0.1;
  dbase.put<real>("waveNumber")=1.0;


  // { // wdh: replaces 'what' factor 
  //   dbase.put<real>("exactSolutionScaleFactorFSI");
  //   dbase.get<real>("exactSolutionScaleFactorFSI")=0.00001; // scale FSI solution so linearized approximation is valid 
  // }
  
  // --- variables for time stepping ---
  dbase.put<int>("numberOfTimeLevels")=3;  // total number of time levels we store
  dbase.put<real>("dt")=-1.;               // time step
  dbase.put<int>("current")=0;             // current time level
  dbase.put<RealArray>("time");
  dbase.put<std::vector<RealArray> >("u"); // displacement 
  dbase.put<std::vector<RealArray> >("v"); // velocity
  dbase.put<std::vector<RealArray> >("a"); // acceleration
  dbase.put<std::vector<RealArray> >("f"); // force
  
  // --- twilight-zone variables ---
  if( !dbase.has_key("exactPointer") ) dbase.put<OGFunction*>("exactPointer")=NULL;
  if( !dbase.has_key("degreeInTime") ) dbase.put<int>("degreeInTime",2);
  if( !dbase.has_key("degreeInSpace") ) dbase.put<int>("degreeInSpace",2);

  if( !dbase.has_key("twilightZone") ) dbase.put<bool>("twilightZone",false);
  if( !dbase.has_key("twilightZoneOption") ) dbase.put<int>("twilightZoneOption",0);

  // Frequencies for trig TZ: 
  if( !dbase.has_key("trigFreq") ) dbase.put<real[4]>("trigFreq");   // ft, fx, fy, [fz]
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  for( int i=0; i<4; i++ ){ trigFreq[i]=2.;  }

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

  const real & T = dbase.get<real>("tension");
  
  // std::cout << "EI = " << EI << std::endl;

  // *wdh* 2014/06/17 -- tension term added
  elementK(0,0) = EI*12./le3       + T*6./(5.*le);    
  elementK(0,1) = EI*6./le2        + T/10.; 
  elementK(0,2) = -elementK(0,0); 
  elementK(0,3) = elementK(0,1);

  elementK(1,0) = elementK(0,1);  
  elementK(1,1) = EI*4./le         + T*le*2./15.; 
  elementK(1,2) = -elementK(0,1); 
  elementK(1,3) = EI*2./le         - T*le/30.;

  elementK(2,0) = elementK(0,2); 
  elementK(2,1) = elementK(1,2); 
  elementK(2,2) = elementK(0,0); 
  elementK(2,3) = elementK(1,2);
  
  elementK(3,0) = elementK(0,1); 
  elementK(3,1) = elementK(1,3); 
  elementK(3,2) = elementK(2,3);
  elementK(3,3) = elementK(1,1);
  
  elementM(0,0) = elementM(2,2) = 13./35.*le*density*thickness;
  elementM(0,1) = elementM(1,0) = 11./210.*le2*density*thickness;
  elementM(0,2) = elementM(2,0) = 9./70.*le*density*thickness;
  elementM(1,3) = elementM(3,1) = -1./140.*le3*density*thickness;
  elementM(3,2) = elementM(2,3) = -11./210.*le2*density*thickness;
  elementM(1,2) = elementM(2,1) = 13./420.*le2*density*thickness;
  elementM(0,3) = elementM(3,0) = -13./420.*le2*density*thickness;
  elementM(1,1) = elementM(3,3) = 1./105.*le3*density*thickness;

  // initialize TZ
  initTwilightZone();

  // --- time stepping arrays ----

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  while( u.size()<numberOfTimeLevels )
  {
    u.push_back(RealArray());
    v.push_back(RealArray());
    a.push_back(RealArray());
    f.push_back(RealArray());
  }
  for( int n=0; n<numberOfTimeLevels; n++ )
  {
    u[n].redim(2*numElem+2); u[n]=0.;
    v[n].redim(2*numElem+2); v[n]=0.;
    a[n].redim(2*numElem+2); a[n]=0.;
    f[n].redim(2*numElem+2); f[n]=0.;
  }
  int & current = dbase.get<int>("current"); 
  RealArray & time = dbase.get<RealArray>("time");
  // const real & dt = dbase.get<real>("dt");
  // assert( dt>0 );
  time.redim(numberOfTimeLevels);
  time=0.;
  current=0;
  assignInitialConditions( time(current), u[current],v[current],a[current] ); 

  dtilde.redim(0); vtilde.redim(0);  // holds predicted values 
  
  dtilde = u[current];
  vtilde = v[current];




}

// ====================================================================================================
/// \brief Initialize the twilight zone 
// ====================================================================================================
int BeamModel::
initTwilightZone()
{

    // -- twilight zone ---
  const bool & twilightZone = dbase.get<bool>("twilightZone");

  printF("-- BM -- initTwilightZone twilightZone=%i\n",(int)twilightZone);

  if( !twilightZone )
    return 0;

  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  const int & degreeInTime = dbase.get<int>("degreeInTime");
  const int & degreeInSpace = dbase.get<int>("degreeInSpace");
  // const aString & exactSolution = dbase.get<aString>("exactSolution");

  // create a twilight-zone function for checking the errors
  OGFunction *& exactPointer = dbase.get<OGFunction*>("exactPointer");

  const int numberOfTZComponents = 1;
  
  if( twilightZoneOption==1 )
  {
    printF("-- BM -- TwilightZone: trigonometric.\n");

    real *trigFreq = dbase.get<real[4]>("trigFreq");  // ft, fx, fy, [fz]
    const real omega[4]={trigFreq[1],trigFreq[2],trigFreq[3],trigFreq[0]};

    RealArray fx( numberOfTZComponents),fy( numberOfTZComponents),fz( numberOfTZComponents),ft( numberOfTZComponents);
    RealArray gx( numberOfTZComponents),gy( numberOfTZComponents),gz( numberOfTZComponents),gt( numberOfTZComponents);
    gx=0.; gy=0.; gz=0.; gt=0.;
    RealArray amplitude( numberOfTZComponents), cc( numberOfTZComponents);
    amplitude=1.;
    cc=0.;

    fx = omega[0];
    fy = domainDimension>1 ?  omega[1] : 0.;
    fz = domainDimension>2 ?  omega[2] : 0.;
    ft = omega[3];

    exactPointer = new OGTrigFunction(fx,fy,fz,ft);
    OGTrigFunction & trig = (OGTrigFunction&)(*exactPointer);
    trig.setShifts(gx,gy,gz,gt);
    trig.setAmplitudes(amplitude);
    trig.setConstants(cc);
      
  }
  else
  {
    printF("--- BM --- TwilightZone: algebraic polynomial\n");
    int degreeOfSpacePolynomial = degreeInSpace;
    int degreeOfTimePolynomial = degreeInTime;

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, numberOfTZComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, numberOfTZComponents);      
    timeCoefficientsForTZ=0.;

    const int wc=0;  // component for w
    if( degreeInSpace==1 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=-.5;      // w=-.5+x
      spatialCoefficientsForTZ(1,0,0, wc)= 1.;
    }
    else if( degreeInSpace==2 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)= 0;      // w = x(1-x) = x - x^2 
      spatialCoefficientsForTZ(1,0,0, wc)= 1.; 
      spatialCoefficientsForTZ(2,0,0, wc)=-1.; 

    }
    else if( degreeInSpace==0 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=1.;
    }
    else if( degreeInSpace==3 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=-1.; 
      spatialCoefficientsForTZ(1,0,0, wc)=.5; 
      spatialCoefficientsForTZ(2,0,0, wc)=-.25; 
      spatialCoefficientsForTZ(3,0,0, wc)=.125;

    }
    else if( degreeInSpace==4 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=-1.; 
      spatialCoefficientsForTZ(1,0,0, wc)=.5; 
      spatialCoefficientsForTZ(2,0,0, wc)=-.25; 
      spatialCoefficientsForTZ(3,0,0, wc)=.125;
      spatialCoefficientsForTZ(4,0,0, wc)=-.3;

    }
    else
    {
      printF("-- BM -- not implemented for degree in space =%i \n",degreeInSpace);
      OV_ABORT("error");
    }

    for( int n=0; n< numberOfTZComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
      {
	timeCoefficientsForTZ(i,n)= i<=degreeInTime ? 1./(i+1) : 0. ;
      }
	  
    }



    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,domainDimension,numberOfTZComponents,
				      degreeOfTimePolynomial);

    ((OGPolyFunction*)exactPointer)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );

  }

  assert( dbase.get<OGFunction*>("exactPointer") !=NULL );

  return 0;
}

// ===================================================================================
/// \brief Set the beam parameters.
// ===================================================================================
void BeamModel::
setParameters(real momOfInertia, real E, 
	      real rho,real beamLength,
	      real thickness_,real pnorm,
	      int nElem,BoundaryCondition bcl,
	      BoundaryCondition bcr,
	      real x0, real y0,
	      bool useExactSolution_ ) 
{


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

  useExactSolution = useExactSolution_;

  initialize();

}

// ==================================================================
/// \brief return the estimatimated *explicit* time step dt 
/// \auhtor WDH
// ==================================================================
real BeamModel::
getExplicitTimeStep() const
{
  // estimate the expliciit time step 

  real beamLength=L;
  int numNodes=numElem+1;
  
  real dx = beamLength/numNodes; 
  
  real EI = elasticModulus*areaMomentOfInertia;
  const real & T = dbase.get<real>("tension");

  //  ( E*I*dt^2/dx^4 + T*dt^2/dx^2 )/( rho*h*b ) < 1 

  real breadth=1.;  // fix me 
  
  real dt = sqrt(  (density*thickness*breadth) /(  EI/pow(dx,4) + T/(dx*dx) ) );
  
  printF("BeamModel::getExplicitTimeStep: EI=%g, T=%g, dx=%8.2e, dt=%8.2e\n",EI,T,dx,dt);

  return dt;
}

// ======================================================================================================
/// \brief Compute the integral of N(eta)*p, that is, the rhs of the FEM model, for a particular element
// p1:   pressure at the first point within the element
// p2:   pressure at the second point within the element
// a=eta1: location (natural coordinate)
// b=eta2: location (natural coordinate)
// fe:   element external force vector [out]
//
///    fe = (N,p)_[a,b] = int_a^b N(xi) p(xi) J dxi 
// ======================================================================================================
void BeamModel::
computeProjectedForce(real p1, real p2, 
		      real a, real b,
		      RealArray& fe) 
{

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

void BeamModel::
setupFreeMotion(real x0,real y0, real angle0) 
{

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

// ================================================================================
/// \brief Solve A u = f
/// 
/// The last 6 arguments are for periodic boundary conditions.  
// ================================================================================
static void 
solveBlockTridiagonal(const RealArray& elementM, const RealArray& f,
		      RealArray& u,
		      BeamModel::BoundaryCondition bcLeft,
		      BeamModel::BoundaryCondition bcRight,
		      bool allowsFreeMotion,
		      bool augmented = false,
		      RealArray* augmentedRow = NULL,
		      RealArray* augmentedCol = NULL,
		      real* augmentedDiagonal = NULL,
		      real* augmentedRHS = NULL,
		      real* augmentedSolution = NULL) 
{

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


// =======================================================================================
/// /brief  Compute the internal force in the beam, i.e., -K*u
/// u: position of the beam
/// f: internal force [out]
// =======================================================================================
void BeamModel::
computeInternalForce(const RealArray& u,RealArray& f) 
{

  RealArray elementU(4);
  RealArray elementForce(4);

  f = 0.0;
  for (int i = 0; i < numElem; ++i)
  {
    // elementU = [ u_i, ux_i, u_{i+1} ux_{i+1} ]
    for (int k = 0; k < 4; ++k)
      elementU(k) = u(i*2+k);
    
    elementForce = mult(elementK, elementU);
    for (int k = 0; k < 4; ++k)
      f(i*2+k) -= elementForce(k);
  }

}

//==============================================================================================
/// \brief Multiply a vector w by the mass matrix
/// w:  vector
/// Mw: M*w [out]
///
//==============================================================================================
void BeamModel::
multiplyByMassMatrix(const RealArray& w, RealArray& Mw)
{

  RealArray elementU(4);
  RealArray tmpv(4);

  Mw = w;
  Mw = 0.0;

  for (int i = 0; i < numElem; ++i) 
  {
    // elementu = [ w_i wx_i w_{i+1} wx_{i+1} ]  
    for (int k = 0; k < 4; ++k)
      elementU(k) = w(i*2+k);
    
    tmpv = mult(elementM, elementU);

    for (int k = 0; k < 4; ++k)
      Mw(i*2+k) += tmpv(k);
  }

  
}

//==============================================================================================
/// \brief Return the (x,y) coordinates of the current beam centerline
/// \author wdh 2014/05/22
//==============================================================================================
void BeamModel::
getCenterLine( RealArray & xc ) const
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  RealArray & uc = u[current];

  xc.redim(numElem+1,2);
  for( int i=0; i<=numElem; i++ ) 
  {
    // (xl,yl) = beam position (un-rotated)
    real xl = ((real)i /numElem) *  L;   // position along neutral axis 
    real yl = uc(2*i);           // displacement 

    xc(i,0) = beamX0 + initialBeamTangent[0]*xl - initialBeamTangent[1]*yl;
    xc(i,1) = beamY0 - initialBeamNormal [0]*xl + initialBeamNormal [1]*yl;
  }

}


//================================================================================================
/// \brief Provide the TravelingWaveFsi object that defines an exact solution.
///
/// \param tw (input) : use this object for computing the TravelingWaveFsi solution
//================================================================================================
int BeamModel::
setTravelingWaveSolution( TravelingWaveFsi & tw )
{
  if( !dbase.has_key("travelingWaveFsi") ) dbase.put<TravelingWaveFsi*>("travelingWaveFsi")=NULL;
 
  dbase.get<TravelingWaveFsi*>("travelingWaveFsi")=&tw;

  return 0;
}


//================================================================================================
/// \brief Compute the acceleration.
///
// Compute the acceleration of the beam.
// u:               current beam position
// v:               current beam velocity
// f:               external force on the beam
// A:               matrix by which the acceleration is multiplied
//                  (e.g., in the newmark beta correction step it is 
//                   M+beta*dt^2*K)
// a:               beam acceleration [out]
// linAcceleration: acceleration of the CoM of the beam (for free motion) [out]
// omegadd:         angular acceleration of the beam (for free motion) [out]
// dt:              time step
// locbeta:         [unused]
// locgamma:        [unused]
//
//================================================================================================
void BeamModel::
computeAcceleration(const RealArray& u, const RealArray& v, 
		    const RealArray& f,
		    const RealArray& A,
		    RealArray& a,
		    real linAcceleration[2],
		    real& omegadd,
		    real dt,
		    real loc_beta,
		    real loc_gamma)
{

  if( debug & 2 )
    printF("--BM-- BeamModel::computeAcceleration, dt=%8.2e\n",dt);
  
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & ua = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  RealArray & uc = ua[current];  // current displacement 

  RealArray rhs(numElem*2+2); // *wdh* 2014/06/19

  // Compute:   rhs = -K*u 
  computeInternalForce(u, rhs);

  rhs += f;

  if( debug & 1 )
  {
    rhs.reshape(2,numElem+1);
    ::display(rhs,"-- BM -- computeAcceleration: rhs after computeInternalForce","%9.2e ");
    rhs.reshape(numElem*2+2);
  }
  

  if( !allowsFreeMotion ) 
  {
    // --- Apply boundary conditions to f - Ku  *wdh* Why do we do this ??

    if (bcLeft == BeamModel::Cantilevered) 
    {
      rhs(0) = 0.0;
      rhs(1) = 0.0;
    }
    
    if (bcLeft == BeamModel::Pinned) {
      rhs(0) = 0.0;
    }
    
    if (bcRight == BeamModel::Cantilevered) {
      rhs(numElem*2) = 0.0;
      rhs(numElem*2+1) = 0.0;
    }
    
    if (bcRight == BeamModel::Pinned) {
      rhs(numElem*2) = 0.0;
    }
  }

  if( allowsFreeMotion ) 
  {
    // --- free body motion ---

    
    //std::cout << "Total pressure force = " << totalPressureForce << std::endl;
    linAcceleration[0] = totalPressureForce*normal[0] / totalMass + bodyForce[0] * buoyantMass / totalMass;
    linAcceleration[1] = totalPressureForce*normal[1] / totalMass + bodyForce[1] * buoyantMass / totalMass;
    omegadd = totalPressureMoment / totalInertia;

    if (bcLeft == BeamModel::Pinned ||
	bcLeft == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      linAcceleration[0] -= penalty*(end[0]-initialEndLeft[0])/totalMass;
      linAcceleration[1] -= penalty*(end[1]-initialEndLeft[1])/totalMass;
      
      real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/-normal[0]*L*0.5)+
			  (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/-normal[1]*L*0.5));
      omegadd -= mom / totalInertia;

      real shear = penalty*((end[0]-initialEndLeft[0])*(-normal[0])+
			    (end[1]-initialEndLeft[1])*(-normal[1]));
      
      rhs(0) += shear;
    }
    
    if (bcRight == BeamModel::Pinned ||
	bcRight == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = numElem-1;
      real eta = 1.0;
      interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      linAcceleration[0] -= penalty*(end[0]-initialEndRight[0])/totalMass;
      linAcceleration[1] -= penalty*(end[1]-initialEndRight[1])/totalMass;
      
      real mom = penalty*((end[0]-initialEndLeft[0])*(/*-wend*tangent[0]*/normal[0]*L*0.5)+
			  (end[1]-initialEndLeft[1])*(/*-wend*tangent[1]*/normal[1]*L*0.5));
      omegadd -= mom / totalInertia;

      real shear = penalty*((end[0]-initialEndRight[0])*(normal[0])+
			    (end[1]-initialEndRight[1])*(normal[1]));
      
      rhs(numElem*2) += shear;
    }

    if (bcLeft == BeamModel::Cantilevered) {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(uc, elem,eta, wend, wendslope);
      
      
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
      
      //rhs(1) += mom; 
    }

    RealArray ones = f,res;
    ones = 0.0;
    for (int i = 0; i < numElem*2+2; i+=2)
      ones(i) = linAcceleration[0]*normal[0]+linAcceleration[1]*normal[1];

    //printArray(ones,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    
    multiplyByMassMatrix(ones, res);
    //res *= 1.0/massPerUnitLength*totalMass;
    rhs -= res;

    multiplyByMassMatrix(u, res);
    rhs += angularVelocityTilde*angularVelocityTilde*res;

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

    rhs -= res*omegadd;

  } // end if allows free motion
  

  // Solve M a = rhs 
  solveBlockTridiagonal(A, rhs, a, bcLeft,bcRight,allowsFreeMotion);
  
}

// ====================================================================================
/// /brief Determine points on the beam surface
  // Return the displacement of the point on the surface (not the neutral axis)
  // of the beam of the point whose undeformed location is (x0,y0).
  // This function is used to update the boundary of the CFD grid.
  // X:       current beam solution vector
  // x0:      undeformed location of the point on the surface of the beam (x)
  // y0:      undeformed location of the point on the surface of the beam (y)
  // x [out]: deformed location of the point on the surface of the beam (x)
  // y [out]: deformed location of the point on the surface of the beam (y)
// ====================================================================================
void BeamModel::
projectDisplacement(const RealArray& X, const real& x0, const real& y0, real& x, real& y) 
{

  int elemNum;
  real eta, thickness;
  
  projectPoint(x0,y0, elemNum, eta,thickness);
  
  real displacement, slope;
  interpolateSolution(X, elemNum, eta, displacement, slope);
  
  real omag = 1./sqrt(slope*slope+1.0);
  real normall[2] = {-slope*omag, omag};

  if (!allowsFreeMotion) 
  {
    real dxt = (x0-beamX0)*initialBeamTangent[0] + (y0-beamY0)*initialBeamTangent[1];
    real dyt = (x0-beamX0)*initialBeamNormal[0] +  (y0-beamY0)*initialBeamNormal[1];

    real xl = dxt+normall[0]*thickness;
    real yl = normall[1]*thickness+displacement;
    
    x = beamX0 + initialBeamTangent[0]*xl-initialBeamTangent[1]*yl;
    y = beamY0 - initialBeamNormal[0]*xl + initialBeamNormal[1]*yl;
  }
  else 
  {

    real xbar = ((x0-beamX0)*initialBeamTangent[0]+
		 (y0-beamY0)*initialBeamTangent[1]-L*0.5);
    assert(xbar >= -L*0.6 && xbar <= L*0.6);
    x = centerOfMass[0] + normal[0] * displacement + xbar*tangent[0];
    y = centerOfMass[1] + normal[1] * displacement + xbar*tangent[1];

    x += (tangent[0] * normall[0] + normal[0]*normall[1])*thickness;
    y += (tangent[1] * normall[0] + normal[1]*normall[1])*thickness;
  }
    
}



// ==============================================================================================
/// /brief Return the acceleration of the point on the surface (not the neutral axis)
/// of the beam of the point whose undeformed location is (x0,y0).
/// This function is used to enforce the pressure boundary condition for the fluid
/// x0:       undeformed location of the point on the surface of the beam (x)
/// y0:       undeformed location of the point on the surface of the beam (y)
/// ax [out]: acceleration of the point on the surface of the beam (x)
/// ay [out]: acceleration of the point on the surface of the beam (y)
///
// ==============================================================================================
void BeamModel::
projectAcceleration(const real& x0,
		    const real& y0, real& ax, real& ay) 
{

  // A point on the beam surface is equal to the point on the neutral surface plus an offset in the normal direction
  //       p  = x(eta) + (0,w) + nv(eta)*thickness

  // The velocity of the point is 
  //       vp = (0,w_t) + d(nv)/(dt)*thickness

  // The acceleration of the point is 
  //       ap = (0,w_tt) + d^2(nv)/(dt^2)*thickness

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration DOF
  RealArray & uc = u[current];  // current displacement 
  RealArray & vc = v[current];  // current velocity 
  RealArray & ac = a[current];  // current acceleration


  int elemNum;
  real eta, thickness;
  
  projectPoint(x0,y0, elemNum, eta,thickness);

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << thickness << std::endl;
  
  real displacement, slope;
  interpolateSolution(uc, elemNum, eta, displacement, slope);

  real Ddisplacement, Dslope;
  interpolateSolution(vc, elemNum, eta, Ddisplacement, Dslope);

  real DDdisplacement, DDslope;
  interpolateSolution(ac, elemNum, eta, DDdisplacement, DDslope);

  if( debug & 2 )
    printF(" -- BM -- projectAcceleration: x=(%g,%g) DDdisplacement=%g (beam accel)\n",x0,y0,DDdisplacement);
  
  
  real omag = 1./sqrt(slope*slope+1.0);
  real omag3 = omag*omag*omag;
  real omag5 = omag*omag*omag3;
  real normall[2] = {-slope*omag, omag};
  real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};
  real normaldd[2] = {3.0*slope*Dslope*omag5*Dslope - omag3*DDslope,
		      3.0*slope*Dslope*omag5*slope*Dslope-omag3*(Dslope*Dslope+slope*DDslope)};

  if (!allowsFreeMotion) 
  {
    real axl = normaldd[0]*thickness;
    real ayl = normaldd[1]*thickness+DDdisplacement;
    
    ax = initialBeamTangent[0]*axl-initialBeamTangent[1]*ayl;
    ay = initialBeamNormal[0]*axl + initialBeamNormal[1]*ayl;
  }
  else 
  {
    // --- free motion ---

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

  // if( true && initialConditionOption=="travelingWaveFSI" )
  // {
  //   // -- for testing set acceleration to the true value
  //   RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2);
  //   x(0,0,0,0)=x0;
  //   x(0,0,0,1)=0.;
  //   travelingWaveFsi.getExactShellSolution( x,ue,ve,t, I1,I2,I3 );

  // }
  


  /*
    if (fabs(ax) >= 1e-12) {
    std::cout << "ax = " << ax << " ay = " << ay << std::endl;
    std::cout << centerOfMassAcceleration[0] << " " << centerOfMassAcceleration[1] << std::endl;
    std::cout << angularAcceleration << " " << displacement << " " << Ddisplacement << " " << DDdisplacement << std::endl;
    }*/
}

// ==============================================================================================
/// /brief Return the velocity of the point on the surface (not the neutral axis)
/// of the beam of the point whose undeformed location is (x0,y0).
/// x0:       undeformed location of the point on the surface of the beam (x)
/// y0:       undeformed location of the point on the surface of the beam (y)
/// vx [out]: velocity of the point on the surface of the beam (x)
/// vy [out]: velocity of the point on the surface of the beam (y)
///
/// /author WDH
// ==============================================================================================
void BeamModel::
projectVelocity( const real& x0, const real& y0, real& vx, real& vy ) 
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
  RealArray & uc = u[current];  // current displacement DOF
  RealArray & vc = v[current];  // current velocity DOF

  int elemNum;
  real eta, thickness;
  
  projectPoint(x0,y0, elemNum, eta,thickness);

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << thickness << std::endl;
  
  real displacement, slope;
  interpolateSolution(uc, elemNum, eta, displacement, slope);

  real Ddisplacement, Dslope;
  interpolateSolution(vc, elemNum, eta, Ddisplacement, Dslope);

  if( debug & 2 )
    printF(" -- BM -- projectVelocity: x=(%g,%g) Ddisplacement=%g (beam velocity)\n",x0,y0,Ddisplacement);
  
  
  // A point on the beam surface is equal to the point on the neutral surface plus an offset in the normal direction
  //       p  = x(eta) + (0,w) + nv(eta)*thickness

  // The velocity of the point is 
  //       vp = (0,w_t) + d(nv)/(dt)*thickness

  real omag = 1./sqrt(slope*slope+1.0);
  real omag3 = omag*omag*omag;
  // real omag5 = omag*omag*omag3;
  // real normall[2] = {-slope*omag, omag};
  real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};

  if( !allowsFreeMotion ) 
  {
    real vxl =                 normald[0]*thickness;
    real vyl = Ddisplacement + normald[1]*thickness;   // v = w_t + (ny)_t * thick
    
    vx = initialBeamTangent[0]*vxl - initialBeamTangent[1]*vyl;
    vy = initialBeamNormal[0] *vxl + initialBeamNormal[1] *vyl;
  }
  else 
  {
    // --- free motion ---

    OV_ABORT("finish me");
    
  }

}



// ==============================================================================
/// \brief Set the initial angle of the beam, from the x axis
/// dec: angle in radians
//
// ==============================================================================
void BeamModel::
setDeclination(real dec) 
{

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

// ================================================================================
/// \brief Compute the slope and displacement of the beam at a given element # and coordinate
/// X:            Beam solution (position)
/// elemNum:      element number on which the solution is desired
/// eta:          element natural coordinate where the solution is desired
/// displacement: displacement at this point [out]
/// slope:        slope at this point [out]
///
// ================================================================================
void BeamModel::
interpolateSolution(const RealArray& X,
		    int& elemNum, real& eta,
		    real& displacement, real& slope) 
{

  // Hermite Shape functions are
  //     f(y) = .25 * (1-y)^2 (y+2 )    : f(-1)=1,   f'(-1)=0,   f(1)=0,    f'(1)=0 
  //     g(y) = .125*le (1-y)^2 (1+y)   : g(-1)=0,   g'(-1)=1,   g(1)=0,    g'(1)=0

  //  le = L / numElem;

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

// ======================================================================================
/// \brief Compute the third derivative, w'''(x), of the beam displacement w(x) at a given
//// element # and coordinate
/// X:       Beam solution (position)
/// elemNum: element number on which the solution is desired
/// eta:     element natural coordinate where the solution is desired
/// deriv3:  Third derivative, w'''(x) at this point
///
// ======================================================================================
void BeamModel::
interpolateThirdDerivative(const RealArray& X,
					   int& elemNum, real& eta,
					   real& deriv3) 
{

  // compute the shape functions.
  real eta1 = 1.-eta;
  real eta2 = 2.-eta;
  real etap1 = eta+1.0;
  real etap2 = eta+2.0;
  
  real sf[4] = {12.0/(le*le*le),6.0/(le*le), -12.0/ (le*le*le),6.0/(le*le)};

		
  deriv3 = sf[0]*X(elemNum*2)+sf[1]*X(elemNum*2+1)+
    sf[2]*X(elemNum*2+2) +sf[3]*X(elemNum*2+3) ;
  
}

// ======================================================================================
// Accumulate a pressure force to the beam from the fluid element whose 
// undeformed location is X1 = (x0_1, y0_1), X2 = (x0_2, y0_2).
// The pressure is p(X1) = p1, p(X2) = p2
/// \param tf : add force at this time 
// x0_1: undeformed location of the point on the surface of the beam (x1)  
// y0_1: undeformed location of the point on the surface of the beam (y1)
// p1:   pressure at the point (x1,y1)
// nx_1: normal at x1 (x) [unused]
// ny_1: normal at x1 (y) [unused]
// x0_2: undeformed location of the point on the surface of the beam (x2)  
// y0_2: undeformed location of the point on the surface of the beam (y2)  
// p2:   pressure at the point (x2,y2)
// nx_2: normal at x2 (x) [unused]
// ny_2: normal at x2 (y) [unused]
//
// ======================================================================================
void BeamModel::
addForce( const real & tf,
	  const real& x0_1, const real& y0_1,
	  real p1,const real& nx_1,const real& ny_1,
	  const real& x0_2, const real& y0_2,
	  real p2,const real& nx_2,const real& ny_2)
{

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  RealArray & fc = f[current];  // force at current time
  assert( fabs(time(current)-tf) < 1.e-10*tf );

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

  
  RealArray lt(4);
  for (int i = elem1; i <= elem2; ++i) 
  {

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
    if (t1 > 0) 
    { // *wdh* Turn this back on 2014/05/23
      pa = -pa;
      pb = -pb;
    }
    
      
    //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
    //  p1 << " " << p2 << std::endl;
  

    if (fabs(b-a) > 1.0e-10)
    {
      // -- compute (N,p)_[a,b] = int_a^b N(xi) p(xi) J dxi 
      computeProjectedForce(pa,pb, a,b, lt);
      //    std::cout << "a = " << a << " b = " << b << std::endl;
      fc(idx) += lt;

      real gradp = 1.0;
      totalPressureForce += (lt(0)+lt(2));
      totalPressureMoment += (lt(0)*(le*i-0.5*L)+lt(1)*gradp+lt(2)*(le*(i+1)-0.5*L) + lt(3)*gradp);
    }
    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  }
}

void BeamModel::resetForce()
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];


  fc=0.;

  totalPressureForce = 0.0;
  totalPressureMoment = 0.0;
}


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
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  return f[current];  // force at current time
}


//  =========================================================================================
/// \brief Assign boundary conditions
/// 
//  =========================================================================================
int BeamModel::
assignBoundaryConditions( real t, RealArray & u, RealArray & v, RealArray & a )
{

  if( bcLeft == Cantilevered && !allowsFreeMotion ) 
  {
    // Set u=0, u_x=0 
    u(0) = u(1) = 0.0;
    v(0) = v(1) = 0.0;
    a(0) = a(1) = 0.0;
  }
  
  if( bcRight == Cantilevered && !allowsFreeMotion ) 
  {
    // Set u=0, u_x=0 
    u(numElem*2) = u(numElem*2+1) = 0.0;
    v(numElem*2) = v(numElem*2+1) = 0.0;
    a(numElem*2) = a(numElem*2+1) = 0.0;
  }

  if( bcLeft == Pinned && !allowsFreeMotion )
  {
    // Set u=0
    u(0) = 0.0;
    v(0) = 0.0;
    a(0) = 0.0;
  }

  if( bcRight == Pinned && !allowsFreeMotion )
  {
    // Set u=0
    u(numElem*2) = 0.0;
    v(numElem*2) = 0.0;
    a(numElem*2) = 0.0;
  }

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  if( twilightZone && !allowsFreeMotion )
  {
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
    const real y=0, z=0;
    const int wc=0;
    for( int side=0; side<=1; side++ )
    {
      BoundaryCondition bc = side==0 ? bcLeft : bcRight;
      int ia = side==0 ? 0 : numElem*2;
      real x = side==0 ? 0 : L;
      if( bc==Pinned || bc == Cantilevered ) 
      {
	u(ia  ) = exact(x,y,z,wc,t);
	v(ia  ) = exact.t(x,y,z,wc,t);            // w.t 
	a(ia  ) = exact.gd(2,0,0,0, x,y,z,wc,t);  // w.tt
      }
      if( bc == Cantilevered ) 
      {
	u(ia+1) = exact.x(x,y,z,wc,t);
	v(ia+1) = exact.gd(1,1,0,0, x,y,z,wc,t);  // w.tx
	a(ia+1) = exact.gd(2,1,0,0, x,y,z,wc,t);  // w.ttx
      }

    }
  }
  
  return 0;
}



// =========================================================================================
/// \brief Predict the structural state at t^{n+1} = tn + dt, using the Newmark beta predictor.
/// The predictor is only first order accurate.
/// tnp1:  new time
/// dt:  current time step
/// x1:  solution state (position) at t^{n-1} [unused]
/// v1:  solution state (velocity) at t^{n-1} [unused]
/// x2:  solution state (position) at t^n
/// v2:  solution state (velocity) at t^n
/// x3:  solution state (position) at t^{n+1} [out]
/// v3:  solution state (velocity) at t^{n+1} [out]
///
// =========================================================================================
void BeamModel::
predictor(real tnp1, real dt )
{

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  int & current = dbase.get<int>("current"); 

  // -- set current to point to the new time level t^{n+1}
  const int prev = current; //  prev points to solutions at t^n
  current = ( current + 1 ) % numberOfTimeLevels;
  const int prev2 = ( prev -1 + numberOfTimeLevels) % numberOfTimeLevels; // points to solution at t^{n-1} 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  RealArray & x2 = u[prev];
  RealArray & v2 = v[prev];
  RealArray & a2 = a[prev];
  RealArray & f2 = f[prev];
  
  RealArray & x3 = u[current];
  RealArray & v3 = v[current];
  RealArray & a3 = a[current];
  RealArray & f3 = f[current];
  
  t += dt;  // new time 
  time(current)=t;

  assert( fabs(time(current)-tnp1) < dt*1.e-6 );


  if( false && t<2.*dt )
  { 
    // wdh: debug info: 
    printF("************** BeamModel::predictor: t=%9.3e ***********************\n",t);
    ::display(x2,"x2","%8.2e ");
    ::display(v2,"v2","%8.2e ");
  }

  RealArray lt(4);
  for (int i = 0; i < numElem; ++i) 
  {
    Index idx(i*2,4);
    // -- compute (N_i, . )
    computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
			  -1.0,1.0, lt);
    f2(idx) += lt;
  }

  if( debug & 4 )
  {
    ::display(f2(Range(0,2*numElem,2)),"BeamModel::predictor: force (displacement)","%8.2e ");
  }
  

  if( !hasAcceleration ) 
  {
    computeAcceleration(x2,v2,f2, elementM, a2,
			centerOfMassAcceleration, angularAcceleration,
			dt);
    hasAcceleration = true;
  }

  RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);

  if( debug & 1 )
  {
    int nn=numElem+1;
    v2.reshape(2,nn); x2.reshape(2,nn); a2.reshape(2,nn); f2.reshape(2,nn);
    ::display(f2,"-- BM -- predictor: f2","%8.2e ");
    ::display(x2,"-- BM -- predictor: u2","%8.2e ");
    ::display(v2,"-- BM -- predictor: v2","%8.2e ");
    ::display(a2,"-- BM -- predictor: a2","%8.2e ");
    v2.reshape(2*nn); x2.reshape(2*nn); a2.reshape(2*nn); f2.reshape(2*nn);
  }
  
  // -- here are the predicted u and v: 
  dtilde = x2+dt*v2+dt*dt*0.5*(1.0-2.0*newmarkBeta)*a2;
  vtilde = v2+dt*(1.0-newmarkGamma)*a2;

  if (allowsFreeMotion) 
  {
    
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

    if (bcLeft == Pinned || bcLeft == Cantilevered) 
    {

      real wend,wendslope;
      int elem = 0;
      real eta = -1.0;
      interpolateSolution(x2, elem,eta, wend, wendslope);
      
      
      real end[2] = {centerOfMass[0] + normal[0]*wend - tangent[0]*L*0.5,
		     centerOfMass[1] + normal[1]*wend - tangent[1]*L*0.5};
      
      std::cout << "End error = " << end[0] - initialEndLeft[0] << " " <<  end[1] - initialEndLeft[1] << std::endl;
    }
  }

  // *wdh* aold = 0.0;
  aold = a2;   // set aold to previous acceleration *wdh* 2014/06/19 
  
  memset(old_rb_acceleration,0,sizeof(old_rb_acceleration));

  //printArray(x2,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);


  v3 = vtilde;
  x3 = dtilde;
  a3 = a2;     // predicted acceleration
  assignBoundaryConditions( tnp1, x3,v3,a3 );
  
 
  if( dbase.get<bool>("saveTipFile") )
  {
    output << t << " " <<  x3(numElem*2) << " " << v3(numElem*2) << " " <<  a3(numElem*2) << std::endl;
  }
  

  const bool & saveProfileFile = dbase.get<bool>("saveProfileFile");

  if( useExactSolution && saveProfileFile ) 
  {
    RealArray xtmp = x3;
    RealArray vtmp = v3;
    RealArray atmp = a2;

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
      interpolateSolution(a3, elemNum, eta, displacement, slope);
      beam_profile << displacement <<  " ";
      
      //if (useExactSolution) {
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

  numCorrectorIterations = 0;

}

// ===================================================================================
/// \brief Apply the corrector at t^{n+1}
// tnp1: new time t^{n+1} 
// dt:  current time step
// x3:  solution state (position) at t^{n+1} [out]
// v3:  solution state (velocity) at t^{n+1} [out]
//
// ===================================================================================
void BeamModel::
corrector(real tnp1, real dt )
{
  
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  RealArray & x3 = u[current];
  RealArray & v3 = v[current];
  RealArray & a3 = a[current];

  RealArray & f3 = f[current];  // force at new time


  assert( fabs(time(current)-tnp1) < dt*1.e-6 );


  RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);
    
  RealArray lt(4);
  for (int i = 0; i < numElem; ++i) 
  {
    Index idx(i*2,4);
    computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
			  -1.0,1.0, lt);
    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
    f3(idx) += lt;
  }
  
  if (time_step_num == 1)  // *wdh* -- what is this ?
  {
    correctionHasConverged = true;
    centerOfMassAcceleration[0] = buoyantMass / totalMass * bodyForce[0];
    centerOfMassAcceleration[1] = buoyantMass / totalMass * bodyForce[1];

    return;
  }

  real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");

  real omega = addedMassRelaxationFactor;

  real linaccel[2],omegadd;
  computeAcceleration(dtilde,v3,f3, A, a3,  linaccel,omegadd,dt, newmarkBeta, newmarkGamma);

  //v3 = vtilde+newmarkGamma*dt*(myAcceleration-aold)*omega;
  //x3 = dtilde+newmarkBeta*dt*dt*(myAcceleration-aold)*omega;

  v3 = vtilde+newmarkGamma*dt*a3;
  x3 = dtilde+newmarkBeta*dt*dt*a3;

  if( allowsFreeMotion ) 
  {

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


  correctionHasConverged = false;

  RealArray tmp;
  tmp = a3-aold;
  double correction = 0.0;
  for (int i = 0; i < numElem; ++i) 
  {
    correction += tmp(i*2)*tmp(i*2)/(le*le)+ tmp(i*2+1)*tmp(i*2+1);  // norm of | a3 - aold |   *wdh: should we scale by dx ?  
  }
  
  if (allowsFreeMotion) 
  {
    real tmpp[3];
    tmpp[0] = old_rb_acceleration[0]-centerOfMassAcceleration[0];
    tmpp[1] = old_rb_acceleration[1]-centerOfMassAcceleration[1];

    tmpp[2] = old_rb_acceleration[2]-angularAcceleration;

    for (int k = 0; k < 3; ++k)
      correction += tmpp[k]*tmpp[k];
  }

  // under-relax the acceleration 
  a3 = omega*a3+(1.0-omega)*aold;    

  if( allowsFreeMotion ) 
  {
    // -- under-relax the rigid body motion --
    centerOfMassAcceleration[0] = omega*centerOfMassAcceleration[0]+(1.0-omega)*old_rb_acceleration[0];
    
    centerOfMassAcceleration[1] = omega*centerOfMassAcceleration[1]+(1.0-omega)*old_rb_acceleration[1];

    angularAcceleration = omega*angularAcceleration + (1.0-omega)*old_rb_acceleration[2];
    
    old_rb_acceleration[0] = centerOfMassAcceleration[0];
    old_rb_acceleration[1] = centerOfMassAcceleration[1];
    old_rb_acceleration[2] = angularAcceleration;
  }
    
  

  aold = a3; // aold holds current value of acceleration
  
  correction = sqrt(correction);

  if (numCorrectorIterations == 0)
    initialResidual = correction;

  ++numCorrectorIterations;

  // std::cout << "correction value = " << correction << std::endl;
  if (correction < initialResidual*subIterationConvergenceTolerance || correction < 1e-8)
    correctionHasConverged = true;

  assignBoundaryConditions( t,x3,v3,a3 );
  
}

// ====================================================================================
/// \brief Return current position DOF's
// ====================================================================================
const RealArray& BeamModel::
position() const  
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 

  return u[current];
}

// ====================================================================================
/// \brief Return current velocity DOF's
// ====================================================================================
const RealArray& BeamModel::
velocity() const  
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF

  return v[current];
}

bool BeamModel::hasCorrectionConverged() const {

  //return true;
  return correctionHasConverged;
}





void BeamModel::setAddedMassRelaxation(double omega) 
{

  dbase.get<real>("addedMassRelaxationFactor") = omega;
}

void BeamModel::setSubIterationConvergenceTolerance(double tol) 
{
  dbase.get<real>("subIterationConvergenceTolerance") = tol;
}



// =================================================================================================
/// \brief  Define the BeamModel parameters interactively.
// =================================================================================================
int BeamModel::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  real & tension = dbase.get<real>("tension");

  bool & twilightZone = dbase.get<bool>("twilightZone");
  int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  int & degreeInTime = dbase.get<int>("degreeInTime");
  int & degreeInSpace = dbase.get<int>("degreeInSpace");
  real *trigFreq = dbase.get<real[4]>("trigFreq");

  GUIState gui;
  gui.setWindowTitle("Beam Model");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");


  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString pbLabels[] = {"initial conditions...",
    			  "help",
    			  ""};

    int numRows=4;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

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


    aString twilightZoneOptions[] = {"polynomial",
				     "trigonometric",
				     ""};
    GUIState::addPrefix(twilightZoneOptions,"Twilight-zone: ",cmd,maxCommands);
    dialog.addOptionMenu( "Twilight-zone:", cmd, twilightZoneOptions, (int)twilightZoneOption );

    aString tbCommands[] = {"use exact solution",
                            "save profile file",
                            "save tip file",
                            "twilight-zone",
    			    ""};
    int tbState[10];
    tbState[0] = useExactSolution;
    tbState[1] = dbase.get<bool>("saveProfileFile");
    tbState[2] = dbase.get<bool>("saveTipFile");
    tbState[3] = twilightZone;
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
    textLabels[nt] = "tension:"; sPrintF(textStrings[nt], "%g",tension);  nt++; 
    textLabels[nt] = "density:"; sPrintF(textStrings[nt], "%g",density);  nt++; 
    textLabels[nt] = "thickness:"; sPrintF(textStrings[nt], "%g",thickness);  nt++; 
    textLabels[nt] = "length:"; sPrintF(textStrings[nt], "%g",L);  nt++; 
    textLabels[nt] = "pressure norm:"; sPrintF(textStrings[nt], "%g",pressureNorm);  nt++; 
    textLabels[nt] = "initial declination:"; sPrintF(textStrings[nt], "%g (degrees)",beamInitialAngle*180./Pi);  nt++; 
    textLabels[nt] = "position:"; sPrintF(textStrings[nt], "%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0);  nt++; 

    textLabels[nt] = "added mass relaxation:"; sPrintF(textStrings[nt], "%g",addedMassRelaxationFactor);  nt++; 
    textLabels[nt] = "added mass tol:"; sPrintF(textStrings[nt], "%g",subIterationConvergenceTolerance);  nt++; 

    textLabels[nt] = "degree in space:";  sPrintF(textStrings[nt],"%i",degreeInSpace);  nt++; 
    textLabels[nt] = "degree in time:";  sPrintF(textStrings[nt],"%i",degreeInTime);  nt++; 
    textLabels[nt] = "trig frequencies:";  sPrintF(textStrings[nt],"%g, %g, %g, %g (ft,fx,fy,fz)",
						 trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]); nt++;

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
    else if( answer=="initial conditions..." )
    {
      chooseInitialConditions( cg,gi );
    }
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    else if( dialog.getTextValue(answer,"name:","%s",name) ){} //
    else if( dialog.getTextValue(answer,"number of elements:","%i",numElem) ){} //

    else if( dialog.getTextValue(answer,"area moment of inertia:","%g",areaMomentOfInertia) ){} //

    else if( dialog.getTextValue(answer,"elastic modulus:","%g",elasticModulus) ){} //
    else if( dialog.getTextValue(answer,"tension:","%g",tension) ){} //
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
    else if( dialog.getToggleValue(answer,"use exact solution",useExactSolution) )
    {
      // *old way*
      if( useExactSolution )
       initialConditionOption="oldTravelingWaveFsi";
    }
    else if( dialog.getToggleValue(answer,"save profile file",dbase.get<bool>("saveProfileFile")) ){} // 
    else if( dialog.getToggleValue(answer,"save tip file",dbase.get<bool>("saveTipFile")) )
    {
      aString tipFileName = sPrintF(buff,"%s_tip.text",(const char*)name);
      output.open(name);
      printF("BeamModel: tip position info will be saved to file 'tip.txt'\n");
    }
    else if( dialog.getToggleValue(answer,"twilight-zone",twilightZone) ){}//
    else if( len=answer.matches("Twilight-zone: ") )
    {
      aString name=answer(len,answer.length()-1);
      if( name=="polynomial" )
      {
	twilightZoneOption=0;
      }
      else if( name=="trigonometric" )
      {
	twilightZoneOption=1;
      }
      else
      {
	printF("Error: unexpected TZ=[%s]\n",(const char*)name);
	gi.stopReadingCommandFile();
        continue;
      }
      dialog.getOptionMenu("Twilight-zone:").setCurrentChoice(name);
    }
    else if( len=answer.matches("trig frequencies:") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e %e",&trigFreq[0],&trigFreq[1],&trigFreq[2],&trigFreq[3]);
      printF("Setting trigonometric TZ frequencies to ft=%g, fx=%g, fy=%g, fz=%g.\n",
	     trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]);

      dialog.setTextLabel("trig frequencies:",sPrintF(buff,"%g, %g, %g, %g (ft,fx,fy,fz)",
                                                      trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]));
    }
    else if( dialog.getTextValue(answer,"degree in space:","%i",degreeInSpace) ){} //
    else if( dialog.getTextValue(answer,"degree in time:","%i",degreeInTime) ){} //
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



// ********************************************************************************************************************
// ********************************************************************************************************************
// ********************************************************************************************************************


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

void BeamModel::
setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) 
{
  
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

