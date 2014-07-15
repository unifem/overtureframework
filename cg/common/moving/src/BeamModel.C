//                                   -*- c++ -*-
// #define BOUNDS_CHECK

#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "TridiagonalSolver.h"

#include <sstream>

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base,I2Base,I3Base;\
  int I1Bound,I2Bound,I3Bound;\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( i3=I3Base; i3<=I3Bound; i3++ )  \
  for( i2=I2Base; i2<=I2Bound; i2++ )  \
  for( i1=I1Base; i1<=I1Bound; i1++ )

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

  exactSolutionOption="none";
  initialConditionOption="none";

  elementK.redim(4,4);
  elementM.redim(4,4);

  t = 0.0;

  //setParameters(0.02*0.02*0.02/12.0,
  //		1.4e6,10000.0, 0.3,0.02, 15);

  density=1.;
  thickness=.1;
  breadth=1.;
  L = 1.;

  areaMomentOfInertia = .1;
  elasticModulus = 1;
  L = 1.;
  numElem = 11;

  dbase.put<real>("tension")=0.;  // T : coefficient of w_xx

  newmarkBeta = 0.25;
  newmarkGamma = 0.5;

  time_step_num = 1;

  pressureNorm = 1.; // 1000.0;  // scale pressure forces by this factor

  hasAcceleration = false;

  bcLeft = bcRight = pinned;
  //bcLeft = bcRight = clamped;

//  added_mass_relaxation = 1.0;

  numCorrectorIterations = 0;

  // convergenceTolerance = 1e-3;

  allowsFreeMotion = false;

  beamX0=beamY0=beamZ0=0.;
  
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

  dbase.put<bool>("useImplicitPredictor")=true;

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
  
  if( !dbase.has_key("useSecondOrderNewmarkPredictor") ) dbase.put<bool>("useSecondOrderNewmarkPredictor")=true;
  if( !dbase.has_key("useNewTridiagonalSolver") ) dbase.put<bool>("useNewTridiagonalSolver")=true;

  // Here is the tri-diagonal solver class:
  if( !dbase.has_key("tridiagonalSolver") ) dbase.put<TridiagonalSolver*>("tridiagonalSolver")=NULL;
  // The variable refactor is set to true when the implicit system chenges (e.g. when dt changes)
  if( !dbase.has_key("refactor") ) dbase.put<bool>("refactor")=true;

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

// ======================================================================================================
/// \brief Destructor.
// ======================================================================================================
BeamModel::~BeamModel() 
{
  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
  if( pTri!=NULL )
  {
    delete pTri;
  }
  

}

// ======================================================================================================
/// \brief Write a summary of the Beam model parameters and boundary conditions etc.
// ======================================================================================================
void BeamModel::
writeParameterSummary( FILE *file /* = stdout */ )
{
  const real & T = dbase.get<real>("tension");
  const real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  const real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  const bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  const bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");

  fPrintF(file," --------------------------------------------------------------------------------\n");
  fPrintF(file,"                        Beam Model\n");
  fPrintF(file," --------------------------------------------------------------------------------\n");
  fPrintF(file," Type: Euler-Bernoulli beam. beamID=%i, name=%s\n",beamID,(const char*)name);
  fPrintF(file,"     (density*thickness*b)*w_tt = T w_xx + EI w_xxxx\n");
  fPrintF(file," E=%9.3e, I=%9.3e, T=%8.2e, \n"
               " density=%9.3e, length=%9.3e, thickness=%9.3e, initial-angle=%7.3f (degrees) \n"
               " numElem=%i, allowsFreeMotion=%i, initial left end=(%12.8e,%12.8e,%12.8e)\n"
               " Newmark time-stepping, beta=%g, gamma=%g, (useImplicitPredictor=%i),\n"
               " pressureNormalization = %8.2e (scale pressure forces by this factor)\n"
               " added-mass relaxation factor=%g, sub-iteration tol=%9.3e\n"
               " useNewTridiagonalSolver=%i\n"
	  , elasticModulus,areaMomentOfInertia,T,density,L,thickness,beamInitialAngle*180./Pi,
	  numElem,(int)allowsFreeMotion,beamX0,beamY0,beamZ0,
          newmarkBeta,newmarkGamma,(int)useImplicitPredictor,
	  pressureNorm,addedMassRelaxationFactor,subIterationConvergenceTolerance,
          (int)useNewTridiagonalSolver);

  aString bcName;
  for( int side=0; side<=1; side++ )
  {
    BoundaryCondition bc = side==0 ? bcLeft : bcRight;
    bcName = (bc==pinned ? "pinned" : 
              bc==clamped ? "clamped" :
              bc==periodic ? "periodic" : 
              bc==freeBC ? "free" : "unknown");
    
    fPrintF(file," %s=%s, ",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcName);
  }
  fPrintF(file,"\n");

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  const int & degreeInTime = dbase.get<int>("degreeInTime");
  const int & degreeInSpace = dbase.get<int>("degreeInSpace");
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  fPrintF(file," twilightZone=%s, option=%s. Poly: degreeT=%i, degreeX=%i, Trig: ft=%g, fx=%g\n",(twilightZone ? "on" : "off"),
          (twilightZoneOption==0 ? "polynomial" : "trigonometric"),
	  degreeInTime,degreeInSpace,trigFreq[0],trigFreq[1] );
  fPrintF(file," Exact solution option: %s\n",(const char*)exactSolutionOption);
  fPrintF(file," Initial condition option: %s\n",(const char*)initialConditionOption);
  

  fPrintF(file," --------------------------------------------------------------------------------\n");
  fPrintF(file," --------------------------------------------------------------------------------\n");

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
  // Tension term from -T(v_x,w_x)
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

  bool & refactor = dbase.get<bool>("refactor");  // et to true when we need to refactor the implicit time-stepping matrix
  refactor=true;

}

// ====================================================================================================
/// \brief Initialize the twilight zone 
// ====================================================================================================
int BeamModel::
initTwilightZone()
{

  // -- twilight zone ---
  const bool & twilightZone = dbase.get<bool>("twilightZone");

  if( debug & 1 )
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
    // printF("-- BM -- TwilightZone: trigonometric.\n");

    real *trigFreq = dbase.get<real[4]>("trigFreq");  // ft, fx, fy, [fz]
    const real omega[4]={trigFreq[1],trigFreq[2],trigFreq[3],trigFreq[0]};

    RealArray fx( numberOfTZComponents),fy( numberOfTZComponents),fz( numberOfTZComponents),ft( numberOfTZComponents);
    RealArray gx( numberOfTZComponents),gy( numberOfTZComponents),gz( numberOfTZComponents),gt( numberOfTZComponents);
    gx=0.; gy=0.; gz=0.; gt=0.;
    RealArray amplitude( numberOfTZComponents), cc( numberOfTZComponents);
    amplitude= dbase.get<real>("amplitude");
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
    // printF("--- BM --- TwilightZone: algebraic polynomial : degreeInSpace=%i, degreeInTime=%i \n",degreeInSpace,degreeInTime);
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
      spatialCoefficientsForTZ(0,0,0, wc)=-.25;      // w = x(1-x) = -.25 + x - .75*x^2 
      spatialCoefficientsForTZ(1,0,0, wc)= 1.; 
      spatialCoefficientsForTZ(2,0,0, wc)=-.75; 

    }
    else if( degreeInSpace==0 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=1.;
    }
    else if( degreeInSpace==3 )
    {
      spatialCoefficientsForTZ(0,0,0, wc)=-.1; 
      spatialCoefficientsForTZ(1,0,0, wc)=2.; 
      spatialCoefficientsForTZ(2,0,0, wc)=-2.5;
      spatialCoefficientsForTZ(3,0,0, wc)=1.;

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

    // printF("OGPolyFunction: degreeInTime=%i\n",degreeInTime);
    
    for( int n=0; n< numberOfTZComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
      {
	timeCoefficientsForTZ(i,n)= i<=degreeInTime ? 1./(i+1) : 0. ;
      }
	  
    }


    int numberOfDimensions=2; // domainDimension;
    
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,numberOfDimensions,numberOfTZComponents,
				      degreeOfTimePolynomial);

    ((OGPolyFunction*)exactPointer)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );

  }

  assert( dbase.get<OGFunction*>("exactPointer") !=NULL );

  return 0;
}

// ===================================================================================
/// \brief Set the beam parameters.
// momOfIntertia:    I/b (true area moment of inertia divided by the width of the beam
// E:                Elastic modulus
// rho:              beam density
// thickness:        beam thickness (assumed to be constant)
// pnorm:            value used to scale the pressure (i.e., the fluid density)
// bcleft:           beam boundary condition on the left
// x0:               initial location of the left end of the beam (x)
// y0:               initial location of the left end of the beam (y)
// useExactSolution: This flag sets the beam model to use the initial conditions
//                   from the exact solution (FSI) in the documentation.
// 
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
  
  const real EI = elasticModulus*areaMomentOfInertia;
  const real & T = dbase.get<real>("tension");

  // Guess the explicit time step: 
  //  ( c4*E*I*dt^2/dx^4 + C2*T*dt^2/dx^2 )/( rho*h*b ) < 1 

  const real c4=1., c2=4.;
  real dt = sqrt(  (density*thickness*breadth) /(  c4*EI/pow(dx,4) + c2*T/(dx*dx) ) );
  
  if( debug & 1 )
    printF("BeamModel::getExplicitTimeStep: EI=%g, T=%g, dx=%8.2e, dt=%8.2e\n",EI,T,dx,dt);

  return dt;
}

// ======================================================================================================
/// \brief Obtain a past time solution (e.g. needed by deforming grids)
/// \param pastTime (input) : obtain solution at this time in the past.
/// \param xPast (output) : points on the beam surface
/// \param t0,x0 (input) : known state at initial time t0
// ======================================================================================================
int BeamModel::
getPastTimeState( const real pastTime, RealArray & xPast, const real t0, const RealArray x0 )
{

  RealArray u,v,a;
  assignInitialConditions( pastTime,u,v,a );

  for( int i3=x0.getBase(2); i3<=x0.getBound(2); i3++ )
  {
    for( int i2=x0.getBase(1); i2<=x0.getBound(1); i2++ )
    {
      for( int i1=x0.getBase(0); i1<=x0.getBound(0); i1++ )
      {
        // u : current beam DOF's
        // x0 : undeformed surface points
        // xPast (output) : current points on beam surface
	projectDisplacement( pastTime, u, x0(i1,i2,i3,0),x0(i1,i2,i3,1),xPast(i1,i2,i3,0), xPast(i1,i2,i3,1));
      }
    }
  }

  return 0;
}

// ======================================================================================================
/// \brief Get the beam's mass per unit length (rho*A = rho*h*b). This value is assumed constant here.
/// \param rhoA (output) 
// ======================================================================================================
int BeamModel::
getMassPerUnitLength( real & rhoA ) const
{
  rhoA=density*thickness*breadth;

  return 0;
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
  
  // printF("computeProjectedForce: p1=%e p2=%e fe=[%e,%e,%e,%e]\n",p1,p2,fe(0),fe(1),fe(2),fe(3));

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

void BeamModel::
addBodyForce(const real bf[2]) 
{

  bodyForce[0] = bf[0];
  bodyForce[1] = bf[1];

  projectedBodyForce = normal[0]*bodyForce[0] + normal[1]*bodyForce[1];
}

static void 
inverse2x2(const RealArray& A, RealArray& inv) 
{

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
/// \param Ae (input) : "element" matrix for A 
//
//     Ae = Me + alpha*Ke 
//     Me = element mass matrix 
//     Ke = element stiffness matrix 
// ================================================================================
void BeamModel::
solveBlockTridiagonal(const RealArray& Ae, const RealArray& f, RealArray& u, const real alpha )
{

  const bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  bool & refactor = dbase.get<bool>("refactor");



  const bool isPeriodic = bcLeft==periodic;
  if( isPeriodic ) 
  { // consistency check:
    assert( bcRight==periodic );
  }
  
  bool checkResidual= (debug & 1 ) && refactor;  // for testing block tridiagonal solver 
  bool useBoth=(debug & 1 ) && useNewTridiagonalSolver && !isPeriodic;  // check new solver with old


  // refactor=true;  // *********************************

  const real & T = dbase.get<real>("tension");
  RealArray lower(2,2), upper(2,2), diag1(2,2), diag2(2,2);

  // int numElem = f.getLength(0)/2-1;

  //  Ae = [ a00 a01 | a02 a03 ]
  //       [ a10 a11 | a12 a13 ]
  //       [ -------- ---------]  
  //       [ a20 a21 | a22 a23 ]
  //       [ a30 a31 | a32 a33 ]

  // upper = upper right quad
  upper(0,0) = Ae(0,2); upper(0,1) = Ae(0,3);
  upper(1,0) = Ae(1,2); upper(1,1) = Ae(1,3);

  // lower = lower left quad (= upper^T  since Me = Me^T
  lower(0,0) = Ae(2,0); lower(0,1) = Ae(2,1);
  lower(1,0) = Ae(3,0); lower(1,1) = Ae(3,1);

  // RealArray upperT(2,2);
  // upperT = trans(upper);

  // ::display(upper ,"--BM-- solveBlockTridiagonal: upper","%8.2e ");
  // ::display(upperT,"--BM-- solveBlockTridiagonal: upperT","%8.2e ");

  diag1(0,0) = Ae(0,0); diag1(0,1) = Ae(0,1);
  diag1(1,0) = Ae(1,0); diag1(1,1) = Ae(1,1);

  diag2(0,0) = Ae(2,2); diag2(0,1) = Ae(2,3);
  diag2(1,0) = Ae(3,2); diag2(1,1) = Ae(3,3);
  
  RealArray dd(2,2);
  dd = diag1+diag2;

  // Solve the block tridiagonal system: 
  //     [ D2[0] D3[0]                         ]
  //     [ D1[0] D2[1] D3[1]                   ]
  //     [       D1[1] D2[2] D3[2]             ]
  //     [                                     ]
  //     [                  ...    ...         ]
  //     [          D1[ne-1] D2[ne-1] D3[ne-1] ]
  //     [                   D1[ne-1] D2[ne]   ]

  RealArray uNew;
  
  if( useBoth || useNewTridiagonalSolver )
  {
    // -- For periodic systems we do not include the last point in the system --
    //       x----+----+----+   ... ----+----x
    //       0    1    2               nTri  numElem
    //                                  
    //    u(0) = u(numElem)
    // 
    int nTri = numElem;
    if( isPeriodic ) nTri=numElem-1;

    Index  I1=Range(0,nTri), I2=Range(0,0);
    // Range I1=Range(0,numElem), I2=Range(0,0);
    // Range I1(0,numElem), I2(0,0);
    const int ndof=2;  // number of degrees of freedom per node 

    // TridiagonalSolver::periodic
    const TridiagonalSolver::SystemType systemType = isPeriodic ? TridiagonalSolver::periodic : TridiagonalSolver::normal;
    
    TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
    if( pTri==NULL )
      pTri = new TridiagonalSolver();

    assert( pTri!=NULL );

    TridiagonalSolver & tri = *pTri;

    RealArray at0(ndof,ndof,I1,I2), bt0(ndof,ndof,I1,I2), ct0(ndof,ndof,I1,I2); // save for checking
    if( refactor )
    {
      if( debug & 1 )
        printF("-- BM -- solveBlockTridiagonal : form block tridiagonal system and factor, isPeriodic=%i\n",(int)isPeriodic);
      
      RealArray at(ndof,ndof,I1,I2), bt(ndof,ndof,I1,I2), ct(ndof,ndof,I1,I2);

      Index D=Range(0,1); // =ndof;
      for( int i=0; i<=nTri; i++ ) 
      {
	if( i>0 || isPeriodic )
	  at(D,D,i,0) = lower;  // lower diagonal 
	else 
	  at(D,D,i,0)=0.;   

	// diagonal :
	if( i==0 && !isPeriodic )
	{
	  bt(D,D,i,0) = diag1;
	}
	else if( i==nTri && !isPeriodic )
	{
	  bt(D,D,i,0) = diag2;
	}
	else
	{
	  bt(D,D,i,0) = dd;      
	}
	
	if( i<nTri || isPeriodic )
	  ct(D,D,i,0) = upper;   // upper diagonal 
	else 
	  ct(D,D,i,0) = 0.;     


      }  // end for i 
      
      // -- Boundary fixup ---
      if( !allowsFreeMotion )
      {
	// --- Boundary conditions ---
	// Adjust the matrix for essential BC's -- these will set the DOF's at boundaries
	for( int side=0; side<=1; side++ )
	{
	  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
	  int ia = side==0 ? 0 : numElem;

	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  const real EI = elasticModulus*areaMomentOfInertia;
	  if( bc==clamped && EI==0. ) bc=pinned;
	  
	  if( bc == clamped ) 
	  {
	    // Replace first block of equations by the identity
	    bt(0,0,ia,0)=1.; bt(0,1,ia,0)=0.;
	    bt(1,0,ia,0)=0.; bt(1,1,ia,0)=1.; 
	    if( side==0 )
	      ct(D,D,ia,0)=0.;
	    else
	      at(D,D,ia,0)=0.;
	  }
	  if( bc == pinned ) 
	  {
	    // replace first equation in first 2x2 block by the identity
	    if( side==0 )
	      ct(0,D,ia,0)=0.; 
	    else
	      at(0,D,ia,0)=0.;
	    
	    bt(0,0,ia,0)=1.; bt(0,1,ia,0)=0.; 
	  }
	  if( bc == freeBC )
	  {
	    // --- correct the stiffnes matrix for a free BC
            // The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
            //  T*N1(0)*Np_1_x(0)*w'_1
            
            // -- freeBC is not allowed with string model ---
            if(  EI==0. )
	    {
	      printF("--BM-- ERROR: A `free' BC is not allowed with the string model for a beam, EI=0\n");
	      OV_ABORT("ERROR");
	    }
	    

	    bt(0,1,ia,0) +=  (1-2*side)*T*alpha;
	  }
	  

	}
      }

      at0=at; bt0=bt; ct0=ct;
      
      // Factor the block tridiagonal system:
      tri.factor(at,bt,ct,systemType,axis1,ndof);

    } // end factor
    
      // -- rhs --

    RealArray xTri(ndof,I1,I2);
    for( int i=0; i<=nTri; i++ ) 
    {
      xTri(0,i,0)=f(i*2);
      xTri(1,i,0)=f(i*2+1);
    }
      
    // solve the block tridiagonal system: 
    tri.solve(xTri);

    // assign the solution 
    for( int i = 0; i<=nTri; i++ ) 
    {
      u(i*2) = xTri(0,i,0);
      u(i*2+1)=xTri(1,i,0);
    }
    if( isPeriodic )
    { // -- assign values on last node
      int i=numElem;
      u(i*2) = u(0); u(i*2+1) = u(1);
    }
    

    if( useBoth )
      uNew=u;
    
    if( refactor && checkResidual )
    {
      // double check solution:
      real resid=0.;
      for( int i = 0; i<=nTri; i++ ) 
      {
	// resid = at*u[i-1] + bt*u[i] + ct*u[i+1];
	int im1=max(0,i-1), ip1=min(i+1,numElem);
	if( i==0       && isPeriodic ){ im1=numElem-1; } // perioidic case : i=0 <-> i=numElem
	if( i==numElem && isPeriodic ){ ip1=1; }         // perioidic case : i=0 <-> i=numElem

	real r=0.;
	r += at0(0,0,i)*u(2*im1) + at0(0,1,i)*u(2*im1+1) + bt0(0,0,i)*u(2*i) + bt0(0,1,i)*u(2*i+1) + ct0(0,0,i)*u(2*ip1) + ct0(0,1,i)*u(2*ip1+1) -f(2*i);
	r += at0(1,0,i)*u(2*im1) + at0(1,1,i)*u(2*im1+1) + bt0(1,0,i)*u(2*i) + bt0(1,1,i)*u(2*i+1) + ct0(1,0,i)*u(2*ip1) + ct0(1,1,i)*u(2*ip1+1) -f(2*i+1);

	// printF("BT: i=%i resid=%e\n",i,r);
	
	resid=max(resid,r);
      }
      printF("--BM-- BLOCK-TRI : max-residual =%8.2e\n",resid);
      if( resid > REAL_EPSILON*1000.*SQR(numElem) )
      {
	OV_ABORT("error");
      }
      

    }
    
    refactor=false;

  }

  if( useBoth || !useNewTridiagonalSolver )
  {
    // ** old way ***

    u = f;

    std::vector< RealArray > diagonal(numElem+1,dd),
      superdiagonal(numElem,upper),subdiagonal(numElem, lower);
  
    diagonal[0] = diag1;
    diagonal[numElem] = diag2;

    if( !allowsFreeMotion )
    {
      // --- Boundary conditions ---
      const real EI = elasticModulus*areaMomentOfInertia;
      BoundaryCondition bc = bcLeft;
      if( bc==clamped && EI==0. ) bc=pinned;

      if (bc == BeamModel::clamped )
      {
	diagonal[0](0,0) = diagonal[0](1,1) = 1.0;
	diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
	superdiagonal[0](0,0) = superdiagonal[0](0,1) = 0.0;
	superdiagonal[0](1,1) = superdiagonal[0](1,0) = 0.0;

      }
      if (bc == BeamModel::pinned ) 
      {
	// replace first equation in first 2x2 block by the identity
	diagonal[0](0,0) = 1.0;
	diagonal[0](0,1) = 0.0;
	superdiagonal[0](0,0) = 0.0;
	superdiagonal[0](0,1) = 0.0;
      }
      if( bc == freeBC )
      {
	// --- correct the stiffnes matrix for a free BC
	// The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
	//  T*N1(0)*Np_1_x(0)*w'_1
            
	diagonal[0](0,1) +=  T*alpha;
      }
 
      bc = bcRight;
      if( bc==clamped && EI==0. ) bc=pinned;

      if (bc == BeamModel::clamped ) 
      {
	diagonal[numElem](0,0) = diagonal[numElem](1,1) = 1.0;
	diagonal[numElem](0,1) = diagonal[numElem](1,0) = 0.0;
	subdiagonal[numElem-1](0,0) = subdiagonal[numElem-1](0,1) = 0.0;
	subdiagonal[numElem-1](1,1) = subdiagonal[numElem-1](1,0) = 0.0;

      }
      if (bc == pinned ) 
      {
	// replace "first" equation in last 2x2 block by the identity
	diagonal[numElem](0,0) = 1.0;
	diagonal[numElem](0,1) = 0.0;
	subdiagonal[numElem-1](0,0) = 0.0;
	subdiagonal[numElem-1](0,1) = 0.0;
      }
      if( bc == freeBC )
      {
	// --- correct the stiffnes matrix for a free BC
	// The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
	//  T*N1(0)*Np_1_x(0)*w'_1

	// printF("-- BM -- solveBlock : add correction term, alpha=%g,  T*alpha = %8.2e\n",alpha, -T*alpha);
	
	diagonal[numElem](0,1) +=  -T*alpha;
      }


    }
  

    RealArray inv;

    Index i2x2(0,2);
  
    for (int i = 0; i < numElem; ++i) 
    {

      inverse2x2(diagonal[i], inv);
      superdiagonal[i] = mult(inv, superdiagonal[i]);
      u(i2x2) = mult(inv, u(i2x2) );
      u(i2x2+2) -= mult(subdiagonal[i],u(i2x2));
      diagonal[i+1] -= mult(subdiagonal[i],superdiagonal[i]);   

      //   if (augmented) {

      // 	(*augmentedCol)(i2x2) = mult(inv , (*augmentedCol)(i2x2));
      // 	(*augmentedCol)(i2x2+2) -= mult(subdiagonal[i], (*augmentedCol)(i2x2));

      // 	(*augmentedRow)(i2x2+2) -= mult((*augmentedRow)(i2x2),superdiagonal[i]);

      // 	*augmentedDiagonal -= mult( (*augmentedRow)(i2x2), (*augmentedCol)(i2x2) )(0);
      // 	*augmentedRHS -= mult( (*augmentedRow)(i2x2), u(i2x2))(0);
      // }

      i2x2 += 2;
    }
    // if (augmented) {
    //   *augmentedSolution = *augmentedRHS / *augmentedDiagonal;

    //   for (int i = numElem*2+1; i >= 0; --i) {

    // 	u(i) -= (*augmentedCol)(i)*(*augmentedSolution);
    //   }
    //  }

    inverse2x2(diagonal[numElem], inv);
    u(i2x2) = mult(inv, u(i2x2) );
  
    i2x2 -= 2;

    for (int i = numElem-1; i >= 0; --i) {

      u(i2x2) -= mult(superdiagonal[i], u(i2x2+2));

      i2x2 -= 2;
    }
  }

  if( useBoth )
  {
    real err = max(fabs(u-uNew));
    printF("--BM-- Block tridiagonal |new - old|=%8.2e\n",err);
    
    if( err >  REAL_EPSILON*1000.*SQR(numElem) )
    {
      printF("*********** ERROR: old and new are different! ***************\n");
      OV_ABORT("error");
    }
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

  const bool isPeriodic = bcLeft==periodic;
  if( isPeriodic )
  {
    Index Is(0,2), Ie(2*numElem,2);
    f(Is) += f(Ie);
    f(Ie) = f(Is);
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
// u:               current beam position (For Newmark this is un + dt*vn + .5*dt^2*(1-2*beta)*an )
// v:               current beam velocity (NOT USED CURRENTLY)
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
computeAcceleration(const real t,
		    const RealArray& u, const RealArray& v, 
		    const RealArray& f,
		    const RealArray& A,
		    RealArray& a,
		    real linAcceleration[2],
		    real& omegadd,
		    real dt,
                    const real alpha,
		    real loc_beta,
		    real loc_gamma)
{

  if( debug & 2 )
    printF("--BM-- BeamModel::computeAcceleration, t=%8.2e, dt=%8.2e\n",t,dt);
  
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & ua = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  RealArray & uc = ua[current];  // current displacement 

  RealArray rhs(numElem*2+2); // *wdh* 2014/06/19

  // Compute:   rhs = -K*u 
  computeInternalForce(u, rhs);


  if( debug & 2 )
  {
    rhs.reshape(2,numElem+1);
    ::display(rhs,"-- BM -- computeAcceleration: rhs after computeInternalForce","%9.2e ");
    rhs.reshape(numElem*2+2);
  }
  
  rhs += f;

  if( !allowsFreeMotion ) 
  {
    // --- Apply boundary conditions to f - Ku  ----

    // --- If the boundary degrees of freedom are given  (e.g. w(0,t)=g0(t) or wx(0,t)=h0(t))
    //     then we eliminate the corresponding equation from the matrix equation (by setting it to the indentity)

    // Get two time derivatives of the boundary functions for "acceleration BC"
    RealArray gtt;
    int ntd=2;  
    getBoundaryValues( t, gtt, ntd );

    // For natural BC's we need EI*wxx(0,t) 
    const real EI = elasticModulus*areaMomentOfInertia;
    const real & T = dbase.get<real>("tension");
    RealArray g;
    getBoundaryValues( t, g );

    for( int side=0; side<=1; side++ )
    {
      BoundaryCondition bc = side==0 ? bcLeft : bcRight;
      const int ia = side==0 ? 0 : numElem*2;
      const int ib = side==0 ? ia+2 : ia-2;

      // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
      if( bc==clamped && EI==0. ) bc=pinned;

      // real x = side==0 ? 0 : L;
      if( bc == clamped ) 
      {
	rhs(ia)=gtt(0,side);   // w_tt is given
	rhs(ia+1)=gtt(1,side);   // wxtt is given 
      }
      else if( bc==pinned )
      {
	rhs(ia)=gtt(0,side);   // w_tt is given
      }
      if( bc == pinned && EI != 0.) 
      {
	// Boundary term is of the form:  -EI* v_x*w_xx
	// -- correct for natural BC:  E*I*w_xx = +/- g(2,side)
        if( debug & 1 )	printF("-- BM -- set rhs for pinned BC wxx = g(2,side)=%8.2e, EI=%g\n",g(2,side),EI);
	rhs(ia+1) += -(1-2*side)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)
      }
      if( bc==freeBC )
      {
	// Boundary terms are of the form:  T*v*w_x  -EI* v*w_xxx - EI* v_x*w_xx
	// Free BC: wxx=EI* g(2,side), w_xxx= EI*g(3,side)

	// printF("-- BM -- set rhs for free BC, g(2)=%e, g(3)=%e, T*u(ia+1)=%8.2e \n",g(2,side),g(3,side),T*u(ia+1));

	rhs(ia  ) +=  (1-2*side)*EI*g(3,side);   // add : -E*I*wxxx(0,t) * N(0)
	rhs(ia+1) += -(1-2*side)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)

	// The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
	rhs(ia  ) += -(1-2*side)*T*u(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1

      }
      
      
    }

  }

  if( allowsFreeMotion ) 
  {
    // --- free body motion ---

    
    //std::cout << "Total pressure force = " << totalPressureForce << std::endl;
    linAcceleration[0] = totalPressureForce*normal[0] / totalMass + bodyForce[0] * buoyantMass / totalMass;
    linAcceleration[1] = totalPressureForce*normal[1] / totalMass + bodyForce[1] * buoyantMass / totalMass;
    omegadd = totalPressureMoment / totalInertia;

    if (bcLeft == BeamModel::pinned ||
	bcLeft == BeamModel::clamped) {

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
    
    if (bcRight == BeamModel::pinned ||
	bcRight == BeamModel::clamped) {

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

    if (bcLeft == BeamModel::clamped) {

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

    for (int i = 0; i < numElem*2+2; i+=2) 
    {
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
  

  if( debug & 2 )
  {
    rhs.reshape(2,numElem+1);
    ::display(rhs,"-- BM -- computeAcceleration: rhs before solve Ma=rhs","%11.4e ");
    rhs.reshape(numElem*2+2);
  }

  // Solve M a = rhs 
  solveBlockTridiagonal(A, rhs, a, alpha );

  if( debug & 2 )
  {
    a.reshape(2,numElem+1);
    ::display(a,"-- BM -- computeAcceleration: solution a after solve","%11.4e ");
    a.reshape(numElem*2+2);
  }

  // solveBlockTridiagonal(A, rhs, a, bcLeft,bcRight,allowsFreeMotion);
  
}

// ====================================================================================
/// \brief Get points on the beam surface
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param xs (output) : current position of beam boundary 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
///
// ====================================================================================
void BeamModel::
getSurface( const real t, const RealArray & x0,  const RealArray & xs, 
	    const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  RealArray & uc = u[current];  // current displacement 

  // ::display(x0,sPrintF("x0: getSurface (undeformed beam), t=%8.2e",t),"%9.2e ");

  RealArray & time = dbase.get<RealArray>("time");
  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
  {
    printF("--BM-- BeamModel::getSurface:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
        t,time(current),current);
    OV_ABORT("ERROR");
  }

  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    projectDisplacement(t, uc, x0(i1,i2,i3,0),x0(i1,i2,i3,1),xs(i1,i2,i3,0),xs(i1,i2,i3,1));
  }

}



// ====================================================================================
/// /brief Determine points on the beam surface
/// Return the displacement of the point on the surface (not the neutral axis)
/// of the beam of the point whose undeformed location is (x0,y0).
/// This function is used to update the boundary of the CFD grid.
/// X:       current beam solution vector
/// x0:      undeformed location of the point on the surface of the beam (x)
/// y0:      undeformed location of the point on the surface of the beam (y)
/// x [out]: deformed location of the point on the surface of the beam (x)
/// y [out]: deformed location of the point on the surface of the beam (y)
// ====================================================================================
void BeamModel::
projectDisplacement(const real t, const RealArray& X, const real& x0, const real& y0, real& wx, real& wy) 
{

  int elemNum;
  real eta, halfThickness;
  
  projectPoint(x0,y0, elemNum, eta,halfThickness);
  
  real displacement, slope;
  interpolateSolution(X, elemNum, eta, displacement, slope);
  
  real omag = 1./sqrt(slope*slope+1.0);
  real normall[2] = {-slope*omag, omag};

  if (!allowsFreeMotion) 
  {
    real dxt = (x0-beamX0)*initialBeamTangent[0] + (y0-beamY0)*initialBeamTangent[1];
    real dyt = (x0-beamX0)*initialBeamNormal[0] +  (y0-beamY0)*initialBeamNormal[1];

    real xl = dxt+normall[0]*halfThickness;
    real yl = normall[1]*halfThickness+displacement;
    
    wx = beamX0 + initialBeamTangent[0]*xl-initialBeamTangent[1]*yl;
    wy = beamY0 - initialBeamNormal[0]*xl + initialBeamNormal[1]*yl;
  }
  else 
  {

    real xbar = ((x0-beamX0)*initialBeamTangent[0]+
		 (y0-beamY0)*initialBeamTangent[1]-L*0.5);
    assert(xbar >= -L*0.6 && xbar <= L*0.6);
    wx = centerOfMass[0] + normal[0] * displacement + xbar*tangent[0];
    wy = centerOfMass[1] + normal[1] * displacement + xbar*tangent[1];

    wx += (tangent[0] * normall[0] + normal[0]*normall[1])*halfThickness;
    wy += (tangent[1] * normall[0] + normal[1]*normall[1])*halfThickness;
  }
    
  if( debug & 4 && initialConditionOption=="travelingWaveFSI" )
  {
    // -- compare to exact ---
    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    // -- for testing set displacement to the true value
    Index I1(0,1),I2(0,1),I3(0,1);
    RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
    x(0,0,0,0)=x0;
    x(0,0,0,1)=0.;
    travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

    if( debug & 4 )
      printF(" -- BM -- projectDisplacement: t=%8.2e, x0=%8.2e computed w=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	     t,x0,wx,wy-y0,ue(0,0,0,0),ue(0,0,0,1),wx-ue(0,0,0,0),(wy-y0)-ue(0,0,0,1));
    if( false )
    {
      // use exact 
      wx=ue(0,0,0,0);
      wy=y0+ue(0,0,0,1);
    }
    
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
projectAcceleration(const real t, 
                    const real& x0,
		    const real& y0, real& ax, real& ay) 
{

  // A point on the beam surface is equal to the point on the neutral surface plus an offset in the normal direction
  //       p  = x(eta) + (0,w) + nv(eta)*halfThickness

  // The velocity of the point is 
  //       vp = (0,w_t) + d(nv)/(dt)*halfThickness

  // The acceleration of the point is 
  //       ap = (0,w_tt) + d^2(nv)/(dt^2)*halfThickness

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration DOF
  RealArray & uc = u[current];  // current displacement 
  RealArray & vc = v[current];  // current velocity 
  RealArray & ac = a[current];  // current acceleration


  int elemNum;
  real eta, halfThickness;
  
  projectPoint(x0,y0, elemNum, eta,halfThickness);

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << halfThickness << std::endl;
  
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
    real axl = normaldd[0]*halfThickness;
    real ayl = normaldd[1]*halfThickness+DDdisplacement;
    
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
    ax += halfThickness*(tangent[0]*normaldd[0] + normald[0]*normal[0]*angularVelocity+
		     (normal[0]*angularAcceleration - tangent[0]*angularVelocity*angularVelocity)*normall[0]);
    ax += halfThickness*(normal[0]*normaldd[1] - normald[1]*tangent[0]*angularVelocity+
		     (-tangent[0]*angularAcceleration - normal[0]*angularVelocity*angularVelocity)*normall[1]);
 
   
    ay = centerOfMassAcceleration[1] + 
      (normal[1]*DDdisplacement + (-tangent[1]*angularVelocity)*Ddisplacement + 
       (-tangent[1]*angularAcceleration-normal[1]*angularVelocity*angularVelocity)*displacement);
    ay += xbar*(normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity);

    ay += halfThickness*(tangent[1]*normaldd[0] + normald[0]*normal[1]*angularVelocity+
		     (normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity)*normall[0]);
    ay += halfThickness*(normal[1]*normaldd[1] - normald[1]*tangent[1]*angularVelocity+
		     (-tangent[1]*angularAcceleration - normal[1]*angularVelocity*angularVelocity)*normall[1]);
 
    //std::cout << "x0 = " << x0 << " y0 = " << y0 << " ax = " << ax << " ay = " << ay << std::endl;
  }

  bool useExact=false;
  if(( useExact || debug & 4 ) && initialConditionOption=="travelingWaveFSI" )
  {
    // -- compare to exact ---
    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    // -- for testing set acceleration to the true value
    Index I1(0,1),I2(0,1),I3(0,1);
    RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
    x(0,0,0,0)=x0;
    x(0,0,0,1)=0.;
    travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

    if( ( useExact || debug & 4 ) )
      printF(" -- BM -- projectAcceleration: t=%8.2e, x0=%8.2e computed a=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	     t,x0,ax,ay,ae(0,0,0,0),ae(0,0,0,1),ax-ae(0,0,0,0),ay-ae(0,0,0,1));
    
    if( useExact )
    {
      // use exact
      ax=ae(0,0,0,0);
      ay=ae(0,0,0,1);
    }
    
  }
  


  /*
    if (fabs(ax) >= 1e-12) {
    std::cout << "ax = " << ax << " ay = " << ay << std::endl;
    std::cout << centerOfMassAcceleration[0] << " " << centerOfMassAcceleration[1] << std::endl;
    std::cout << angularAcceleration << " " << displacement << " " << Ddisplacement << " " << DDdisplacement << std::endl;
    }*/
}

// ==============================================================================================
/// /brief Return the "internal force" of the point on the surface (not the neutral axis)
/// of the beam of the point whose undeformed location is (x0,y0).
///
/// This function is used to as part of the added-mass algorithm
/// 
/// /param internalForce (input) : internal force solution of the beam
/// /paramx0:       undeformed location of the point on the surface of the beam (x)
/// /param y0:       undeformed location of the point on the surface of the beam (y)
/// /paramax [out]: acceleration of the point on the surface of the beam (x)
/// /paramay [out]: acceleration of the point on the surface of the beam (y)
///
// ==============================================================================================
void BeamModel::
projectInternalForce(const RealArray & internalForce,
		     const real t, 
		     const real& x0,
		     const real& y0, real& ax, real& ay) 
{

  // A point on the beam surface is equal to the point on the neutral surface plus an offset in the normal direction
  //       p  = x(eta) + (0,w) + nv(eta)*halfThickness   (NOTE: halfThickness here is THE half halfThickness)

  // The velocity of the point is 
  //       vp = (0,w_t) + d(nv)/(dt)*halfThickness

  // The acceleration of the point is 
  //       ap = (0,w_tt) + d^2(nv)/(dt^2)*halfThickness

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration DOF
  RealArray & uc = u[current];  // current displacement 
  RealArray & vc = v[current];  // current velocity 
  // RealArray & ac = a[current];  // current acceleration


  int elemNum;
  real eta, halfThickness;
  
  projectPoint(x0,y0, elemNum, eta,halfThickness);

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << halfThickness << std::endl;
  
  real displacement, slope;
  interpolateSolution(uc, elemNum, eta, displacement, slope);

  real Ddisplacement, Dslope;
  interpolateSolution(vc, elemNum, eta, Ddisplacement, Dslope);

  real DDdisplacement, DDslope;

  // *** Here is the only difference between projectAcceleration and projectInternalForce ***
  interpolateSolution(internalForce, elemNum, eta, DDdisplacement, DDslope);
  // interpolateSolution(ac, elemNum, eta, DDdisplacement, DDslope);

  
  // *************************************************
  // **************** CHECK ME ***********************
  // *************************************************


  // --- compute the correction for finite thickness ---
  //   correction is 
  //            +density*hs*hs/2 * n_tt 
  // Note the extra factor of density*hs compared to projectAcceleration 

  const real thicknessFactor = density*(2.*halfThickness)*halfThickness;

  real omag = 1./sqrt(slope*slope+1.0);
  real omag3 = omag*omag*omag;
  real omag5 = omag*omag*omag3;
  real normall[2] = {-slope*omag, omag};
  real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};
  real normaldd[2] = {3.0*slope*Dslope*omag5*Dslope - omag3*DDslope,
		      3.0*slope*Dslope*omag5*slope*Dslope-omag3*(Dslope*Dslope+slope*DDslope)};

  if (!allowsFreeMotion) 
  {
    real axl = normaldd[0]*thicknessFactor;
    real ayl = normaldd[1]*thicknessFactor+DDdisplacement;
    
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
    ax += thicknessFactor*(tangent[0]*normaldd[0] + normald[0]*normal[0]*angularVelocity+
		     (normal[0]*angularAcceleration - tangent[0]*angularVelocity*angularVelocity)*normall[0]);
    ax += thicknessFactor*(normal[0]*normaldd[1] - normald[1]*tangent[0]*angularVelocity+
		     (-tangent[0]*angularAcceleration - normal[0]*angularVelocity*angularVelocity)*normall[1]);
 
   
    ay = centerOfMassAcceleration[1] + 
      (normal[1]*DDdisplacement + (-tangent[1]*angularVelocity)*Ddisplacement + 
       (-tangent[1]*angularAcceleration-normal[1]*angularVelocity*angularVelocity)*displacement);
    ay += xbar*(normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity);

    ay += thicknessFactor*(tangent[1]*normaldd[0] + normald[0]*normal[1]*angularVelocity+
		     (normal[1]*angularAcceleration - tangent[1]*angularVelocity*angularVelocity)*normall[0]);
    ay += thicknessFactor*(normal[1]*normaldd[1] - normald[1]*tangent[1]*angularVelocity+
		     (-tangent[1]*angularAcceleration - normal[1]*angularVelocity*angularVelocity)*normall[1]);
 
    //std::cout << "x0 = " << x0 << " y0 = " << y0 << " ax = " << ax << " ay = " << ay << std::endl;
  }

}

// ====================================================================================
/// \brief Get the acceleration of points on the beam surface
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param as (output) : current acceleration of the beam boundary 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
///
// ====================================================================================
void BeamModel::
getSurfaceAcceleration( const real t, const RealArray & x0,  const RealArray & as, 
			const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    projectAcceleration(t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),as(i1,i2,i3,0),as(i1,i2,i3,1) );
  }
}


// ====================================================================================
/// \brief Get the velocity of points on the beam surface
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param vs (output) : current velocity of the beam boundary 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
///
// ====================================================================================
void BeamModel::
getSurfaceVelocity( const real t, const RealArray & x0,  const RealArray & vs, 
		    const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    projectVelocity(t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),vs(i1,i2,i3,0),vs(i1,i2,i3,1) );
  }
}


// ====================================================================================
/// \brief Get the 'internal force' on the beam surface (used by added mass algorithms)
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param fs (output) : current internal-force of the beam boundary 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
///
// ====================================================================================
void BeamModel::
getSurfaceInternalForce( const real t, const RealArray & x0,  const RealArray & fs, 
			const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  
  int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  RealArray & xc = u[current];
  RealArray & vc = v[current];
  RealArray & ac = a[current];
  RealArray & fc = f[current];

  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
  {
    printF("--BM-- BeamModel::getSurfaceInternalForce:ERROR: t=%10.3e != time(current)=%10.3e, current=%i\n",
        t,time(current),current);
    OV_ABORT("ERROR");
  }

  // Governing equation:
  //      ms* v_t = L(u) + fe
  //      ms=rho*h*b 
  // The internal force is L(u), if fe=0 then L(u) = ms*v_t = ms*a 
  // 
  // FEM approximation:
  //       ms Ms a = K u + Fe 
  //  where M= ms*Ms is the mass matrix and Ms is the mass matrix without the factor of ms.
  // 
  // To solve for the internal force we set Fe=0 and solve
  //
  //     L(u) =  ms*a = Ms^{-1} Ku         
  //  


  // compute internal force at time t
  if( true )
  {
    RealArray Me(4,4);  // Element mass matrix without the factor of ms=rhos*hs*bs

    // le = L / numElem;

    const real le2 = le*le;
    const real le3 = le2*le;

    Me(0,0) = Me(2,2) = 13./35.*le;
    Me(0,1) = Me(1,0) = 11./210.*le2;
    Me(0,2) = Me(2,0) = 9./70.*le;
    Me(1,3) = Me(3,1) = -1./140.*le3;
    Me(3,2) = Me(2,3) = -11./210.*le2;
    Me(1,2) = Me(2,1) = 13./420.*le2;
    Me(0,3) = Me(3,0) = -13./420.*le2;
    Me(1,1) = Me(3,3) = 1./105.*le3;


    RealArray internalForce(2*numElem+2), fe(2*numElem+2);
    fe=0.;  // set external force to zero

    bool & refactor = dbase.get<bool>("refactor"); 
    refactor=true;          

    const real alpha=0.;  // coeff of K in  (M+alpha*K)*a = RHS
    const real dt=0.;
    computeAcceleration( t, xc,vc,fe, Me, internalForce, centerOfMassAcceleration, angularAcceleration, dt, alpha );

    refactor=true;          

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
    {
      projectInternalForce( internalForce, t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),fs(i1,i2,i3,0),fs(i1,i2,i3,1) );
    }

    // ::display(ac,"--BM-- internalForce : ac -- acceleration","%8.2e ");
    // ::display(internalForce,"--BM-- internalForce : internalForce ","%8.2e ");


    // ::display(fs,"--BM-- internalForce from projectInternalForce","%8.2e ");
    
  }
  
  bool useExact=false;
  if( useExact && initialConditionOption=="travelingWaveFSI" )
  {
    // -- compare to exact ---
    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    // -- for testing set internal force to the true value
    Range Rx=numberOfDimensions;
    RealArray ue(Ib1,Ib2,Ib3 ,Rx),ve(Ib1,Ib2,Ib3 ,Rx),ae(Ib1,Ib2,Ib3 ,Rx);
    travelingWaveFsi.getExactShellSolution( x0,ue,ve,ae, t, Ib1,Ib2,Ib3 );

    RealArray uf(Ib1,Ib2,Ib3,numberOfDimensions+1); // hold fluid solution (p,v1,v2)
    real t0=t;
    travelingWaveFsi.getExactFluidSolution( uf, t0, x0, Ib1,Ib2,Ib3 );

    // *TESTING* -- use exact: 
    
    real ms=density*thickness*breadth;
    
    fs(Ib1,Ib2,Ib3,Rx)=ms*ae(Ib1,Ib2,Ib3,Rx);
    fs(Ib1,Ib2,Ib3,1) -= uf(Ib1,Ib2,Ib3,0);  // Lu = rhos*As*v_t - p 
    
    printF("--BM--getSurfaceInternalForce *TEST* set internal force to exact, t=%8.2e\n",t);

    ::display(fs,"--BM-- internalForce from EXACT","%8.2e ");

  }
  

  // int i1,i2,i3;
  // FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  // {
  //   projectAcceleration(t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),as(i1,i2,i3,0),as(i1,i2,i3,1) );
  // }

  
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
projectVelocity( const real t, const real& x0, const real& y0, real& vx, real& vy ) 
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

  if( debug & 4  && initialConditionOption=="travelingWaveFSI" )
  {
    // -- compare to exact ---

    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    // -- for testing set acceleration to the true value
    Index I1(0,1),I2(0,1),I3(0,1);
    RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
    x(0,0,0,0)=x0;
    x(0,0,0,1)=0.;
    travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

    if( debug & 4 )
      printF(" -- BM -- projectVelocity: t=%8.2e, x0=%8.2e computed v=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	     t,x0,vx,vy,ve(0,0,0,0),ve(0,0,0,1),vx-ve(0,0,0,0),vy-ve(0,0,0,1));

    if( false )
    {
      vx=ve(0,0,0,0);
      vy=ve(0,0,0,1);
    }
    
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


// =================================================================================
/// /brief Return the element, thickness, and natural coordinate for
/// a point (x0,y0) on the undeformed SURFACE of the beam
/// 
/// x0:        undeformed location of the point on the surface of the beam (x)
/// y0:        undeformed location of the point on the surface of the beam (y)
/// elemNum:   element corresponding to this point [out]
/// eta:       natural coordinate corresponding to this point [out]
/// halfThickness: half-thickness of the beam at this point (i.e. approximate
///                normal distance from centerline to surface) [out]
//
// =================================================================================
void BeamModel::
projectPoint(const real& x0,const real& y0,
	     int& elemNum, real& eta, real& halfThickness) 
{

  //                  x0

  //   x--------------|--------------   beam surface
  //   
  //   +-----+-----+--|----+-----+
  //   0     1     2       3   
  // 
  //               +--|----+
  //              -1  eta  1

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

  if( elemNum >= numElem )
  {
    elemNum = numElem-1;
    eta = 1.0;
  }

  if( elemNum < 0 )
  {
    elemNum = 0;
    eta = -1.0;
  }
     
 
  halfThickness = yl;
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

// =================================================================================================
/// /brief Assign the force on the beam
/// /param x0 (input) : array of (undeformed) locations on the beam surface
/// /param traction (input) : traction on the deformed surface
/// /param normal (input) : normal to the deformed surface
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
// =================================================================================================
void BeamModel::
addForce(const real & tf, const RealArray & x0, const RealArray & traction, const RealArray & normal,  
         const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  // Jb1, Jb2, Jb3 : for looping over cells instead of grid points -- decrease by 1 along active axis
  Index Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
  int axis=-1, is1=0, is2=0, is3=0;
  if( Jb1.getLength()>1 )
  { // grid points on boundary are along axis=0
    axis=0; is1=1;
    Jb1=Range(Jb1.getBase(),Jb1.getBound()-1); // decrease length by 1
    assert( Jb2.getLength()==1 );
  }
  else
  { // grid points on boundary are along axis=1
    axis=1; is2=1;
    Jb2=Range(Jb2.getBase(),Jb2.getBound()-1); // decrease length by 1
    assert( Jb1.getLength()==1 );
  }
  assert( axis>=0 );
  
  
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Jb1,Jb2,Jb3)
  {
    int i1p=i1+is1, i2p=i2+is2, i3p=i3+is3;
    
    real p1 = (traction(i1,i2,i3,0)*normal(i1,i2,i3,0)+
	       traction(i1,i2,i3,1)*normal(i1,i2,i3,1) );

    real p2 = (traction(i1p,i2p,i3p,0)*normal(i1p,i2p,i3p,0)+
	       traction(i1p,i2p,i3p,1)*normal(i1p,i2p,i3p,1) );


    addForce(tf,
	     x0(i1,i2,i3,0), 
	     x0(i1,i2,i3,1), p1,
	     normal(i1,i2,i3,0), normal(i1,i2,i3,1),
	     x0(i1p,i2p,i3p,0), 
	     x0(i1p,i2p,i3p,1), p2,
	     normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1));
  }
  
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
  if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
  {
    printF("--BM-- BeamModel::addForce:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
        tf,time(current),current);
    OV_ABORT("ERROR");
  }

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

void BeamModel::
resetForce()
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];

  // printF("-- BM -- resetForce current=%i \n",current);
  

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
/// \brief Return the RHS values for the boundary conditions.
/// \param ntd (input) number of time derivatives 
/// \param g(0:3,0:1) (output) : g(i,side), i=0,1,2,3, and side=0,1 (left or right)
///   Example, for side=0:
///     u(0,t)     = g(0,0)   (or = ut(0,t) if ntd=1 , etc, )
///     ux(0,t)    = g(1,0)   (or = uxt(0,t), if ntd=1, etc. )
///     uxx(0,t)   = g(2,0)   (or = uxxt ...
///     uxxx(0,t)  = g(3,0)   (or = uxxxt ...
/// /Note: Only some vaues apply, depending on the BC
//  =========================================================================================
int BeamModel::
getBoundaryValues( const real t, RealArray & g, const int ntd /* = 0 */   )
{

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

  if( g.getLength(0)==0 ) g.redim(4,2);
  
  g=0.;
  
  if( !allowsFreeMotion )
  {
    const real y=0, z=0;
    const int wc=0;
    for( int side=0; side<=1; side++ )
    {
      BoundaryCondition bc = side==0 ? bcLeft : bcRight;
      int ia = side==0 ? 0 : numElem*2;
      real x = side==0 ? 0 : L;

      if( bc == clamped ) 
      {
        // --- clamped BC ---
	if( twilightZone )
	{
	  g(0,side) = exact.gd(ntd,0,0,0, x,y,z,wc,t);  // Give w 
          g(1,side) = exact.gd(ntd,1,0,0, x,y,z,wc,t);  // Give w.x 
	}
	else
	{
          g(0,side)=0.;
          g(1,side)=0.;
	}
      }

      else if( bc==pinned ) 
      {
        // --- pinned BC ---
	if( twilightZone )
	{
	  g(0,side) = exact.gd(ntd,0,0,0, x,y,z,wc,t);  // Give w 
          g(2,side) = exact.gd(ntd,2,0,0, x,y,z,wc,t);  // give EI*w.xx 
	}
	else
	{
          g(0,side)=0.;
          g(2,side)=0.;
	}
      }

      else if( bc==freeBC ) 
      {
        // --- free BC ---
	if( twilightZone )
	{
	  g(2,side) = exact.gd(ntd,2,0,0,  x,y,z,wc,t);   // Give EI*w_xx 
	  g(3,side) = exact.gd(ntd,3,0,0,  x,y,z,wc,t);   // Give EI*w_xxx
	}
	else
	{
          g(2,side)=0.;
          g(3,side)=0.;
	}
      }

      else if( bc==periodic )
      {
      }
      else
      {
	OV_ABORT("ERROR - unknown bc");
      }
      
    }
  }

  
  return 0;
}


//  =========================================================================================
/// \brief Assign boundary conditions
/// 
//  =========================================================================================
int BeamModel::
assignBoundaryConditions( real t, RealArray & u, RealArray & v, RealArray & a )
{
  const real EI = elasticModulus*areaMomentOfInertia;

  if( !allowsFreeMotion )
  {
    RealArray ue,ve,ae;
    bool assignExact=false;
    if( bcLeft!=periodic && initialConditionOption=="travelingWaveFSI" )
    {
      assignExact=true;
      getTravelingWaveFSI( t, ue, ve, ae  );  // this is inefficient
    }

    for( int side=0; side<=1; side++ )
    {
      BoundaryCondition bc = side==0 ? bcLeft : bcRight;
      int ia = side==0 ? 0 : numElem*2;

      // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
      if( bc==clamped && EI==0. ) bc=pinned;

      real u0=0.,  v0=0.,  a0=0.;
      real ux0=0., vx0=0., ax0=0.;
      if( assignExact )
      {
	u0=ue(ia); ux0=ue(ia+1);
	v0=ve(ia); vx0=ve(ia+1);
	a0=ae(ia); ax0=ae(ia+1);
      }
	
      if( bc == clamped || bc==pinned ) 
      {
	// Set u=g
	u(ia) = u0;
	v(ia) = v0;
	a(ia) = a0;
      }
      if( bc==clamped )
      {
	// Set u_x=h
	u(ia+1) = ux0;
	v(ia+1) = vx0;
	a(ia+1) = ax0;
      }



    }
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

      // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
      if( bc==clamped && EI==0. ) bc=pinned;

      if( bc==pinned || bc == clamped ) 
      {
	u(ia  ) = exact(x,y,z,wc,t);
	v(ia  ) = exact.t(x,y,z,wc,t);            // w.t 
	a(ia  ) = exact.gd(2,0,0,0, x,y,z,wc,t);  // w.tt
      }
      if( bc == clamped ) 
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
/// \brief Add internal forces such as buoyancy and TZ forces
///
/// Compute the element force vectors 
// =========================================================================================
int BeamModel::
addInternalForces( const real t, RealArray & f )
{
  // if( false )
  // { 
  //   f=0.; // ******************************************************* TEST 
  // }

  const real beamLength=L;

  if( exactSolutionOption=="travelingWaveFSI" )
  {
    // add forces for the FSI traveling wave solution
    Index I1,I2,I3;
    I1=Range(0,numElem); I2=0; I3=0;

    RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
    const real beamLength=L;
    const real dx=beamLength/numElem;
    real heightFluidRegion=1.;
    for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = heightFluidRegion;    // should match value in travelingWaveFsi
    }

    assert( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")!=NULL );
    TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

    RealArray ufe(I1,I2,I3,3);  // holds (p,v1f,v2f)

    // Evaluate the exact fluid solution on the interface
    travelingWaveFsi.getExactFluidSolution( ufe, t, x, I1,I2,I3 );

    // ::display(ufe(I1,0,0,0),sPrintF(" Exact fluid pressure at t=%8.2e",t),"%8.2e ");

    RealArray lt(4); // local traction
    const int pc=0;
    for ( int i = 0; i<numElem; i++ )
    {
      real p0=ufe(i,0,0,pc), p1=ufe(i+1,0,0,pc);
      computeProjectedForce( p0,p1, -1.0,1.0, lt);
      Index idx(i*2,4);
      f(idx) += lt;
    }

  }
  

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  if( twilightZone )
  {
    OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
    Index I1,I2,I3;
    I1=Range(0,numElem); I2=0; I3=0;

    RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
    const real dx=beamLength/numElem;
    for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = 0.;    // should this be y0 ?
    }

    const real EI = elasticModulus*areaMomentOfInertia;
    const real & T = dbase.get<real>("tension");

    RealArray utte(I1,I2,I3,1), uxxe(I1,I2,I3,1), uxxxxe(I1,I2,I3,1);
    int isRectangular=0;
    const int wc=0;
    exact.gd( utte   ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
    exact.gd( uxxe   ,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,t );
    exact.gd( uxxxxe ,x,domainDimension,isRectangular,0,4,0,0,I1,I2,I3,wc,t );

    // exact.gd( ve ,x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
    // exact.gd( ae ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );

    // exact.gd( uxe,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
    // exact.gd( vxe,x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
    // exact.gd( axe,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );

    // printF("-- BM -- addInternalForce: t=%9.3e max(fabs(f))=%8.2e, |utte|=%8.2e |u_xxxxe|=%8.2e\n",
    //        t,max(fabs(f)),max(fabs(utte)),max(fabs(uxxxxe)));

    RealArray ftz(I1,I2,I3,1); 
    ftz = (density*thickness)*utte - (T)*uxxe + (EI)*uxxxxe;

    // ::display(utte,"utte","%8.2e ");
    // ::display(uxxe,"uxxe","%8.2e ");
    // ::display(ftz,"ftz","%8.2e ");
    
    RealArray lt(4); // local traction
    for ( int i = 0; i<numElem; i++ )
    {
      computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);
      Index idx(i*2,4);
      f(idx) += lt;
    }
   
  }

  if( projectedBodyForce*buoyantMassPerUnitLength!=0. )
  {
    // --- add buyouncy force
    RealArray lt(4);
    for (int i = 0; i < numElem; ++i) 
    {
      // -- compute (N_i, . )
      computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
			    -1.0,1.0, lt);
      Index idx(i*2,4);
      f(idx) += lt;
    }
  }
  
  const bool isPeriodic = bcLeft==periodic;
  if( isPeriodic )
  {
    Index Is(0,2), Ie(2*numElem,2);
    f(Is) += f(Ie);
    f(Ie) = f(Is);
  }
  

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
  const bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  bool & refactor = dbase.get<bool>("refactor");
  const bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");
  
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

  if( fabs(time(current)-tnp1) > 1.e-10*(1.+tnp1) )
  {
    printF("--BM-- BeamModel::predictor:ERROR: tnp1=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
        tnp1,time(current),current);
    OV_ABORT("ERROR");
  }

  real & dtOld = dbase.get<real>("dt"); 
  if( fabs(dt-dtOld) > REAL_EPSILON*10.*dt )
  {
    refactor=true;
    printF("-- BM -- predictor: dt has changed, dt=%9.3e, dtOld=%9.3e, will refactor.\n",dt,dtOld);
  }
  dtOld=dt;

  if( false && t<2.*dt )
  { 
    // wdh: debug info: 
    printF("************** BeamModel::predictor: t=%9.3e ***********************\n",t);
    ::display(x2,"x2","%8.2e ");
    ::display(v2,"v2","%8.2e ");
  }

  // add internalforces such as buoyancy and TZ forcing
  addInternalForces( t-dt, f2 );
  
  // RealArray lt(4);
  // for (int i = 0; i < numElem; ++i) 
  // {
  //   // -- compute (N_i, . )
  //   computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
  // 			  -1.0,1.0, lt);
  //   Index idx(i*2,4);
  //   f2(idx) += lt;
  // }

  if( debug & 4 )
  {
    ::display(f2(Range(0,2*numElem  ,2)),"BeamModel::predictor: RHS force f2(0:2:)","%8.2e ");
    ::display(f2(Range(1,2*numElem+1,2)),"BeamModel::predictor: RHS force f2(1:2:)","%8.2e ");
  }
  

  if( !hasAcceleration ) 
  {
    // On the very first time-step we may not know the acceleration at t=0.
    refactor=true;          

    // compute acceleration at time tn=t-dt 
    const real alpha=0.;  // coeff of K in  (M+alpha*K)*a = RHS
    computeAcceleration(t-dt, x2,v2,f2, elementM, a2,
			centerOfMassAcceleration, angularAcceleration,
			dt, alpha );
    refactor=true;          
    hasAcceleration = true;
  }


  if( debug & 4 )
  {
    int nn=numElem+1;
    v2.reshape(2,nn); x2.reshape(2,nn); a2.reshape(2,nn); f2.reshape(2,nn);
    ::display(a2,"-- BM -- predictor: a2","%8.2e ");
    ::display(f2,"-- BM -- predictor: f2","%8.2e ");
    ::display(x2,"-- BM -- predictor: u2","%8.2e ");
    ::display(v2,"-- BM -- predictor: v2","%8.2e ");
    v2.reshape(2*nn); x2.reshape(2*nn); a2.reshape(2*nn); f2.reshape(2*nn);
  }
  
  //  -- first order predictor --
  dtilde = x2 + dt*v2 + (dt*dt*0.5*(1.0-2.0*newmarkBeta))*a2;
  vtilde = v2 + dt*(1.0-newmarkGamma)*a2;

  // -- here are the predicted u and v: 
  if( useSecondOrderNewmarkPredictor )
  {
    if( useImplicitPredictor )
    {
      // -- The implicit predictor makes a guess for the forcing at time tnp1 = t+dt  ---

      if( debug & 2 )
	printF("--BM-- use implicit predictor tnp1=%8.2e\n",tnp1);

      RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);

      real linaccel[2],omegadd;
      // compute acceleration at time t^{n+1}
      const real alpha=newmarkBeta*dt*dt;  // coeff of K in A

      if( false ) // apply BC's to first-order predictor 
        assignBoundaryConditions( tnp1, dtilde,vtilde,a3 );

      if( t>= .5*dt )
      {
        RealArray & f1 = f[prev2];
        f3=2.*f2-f1;            // extrapolate in time ** FIX ME for variable dt **
      }
      else
      {
        f3=f2; 
      }
      
      computeAcceleration(tnp1, dtilde,vtilde,f3, A, a3,  linaccel,omegadd,dt, alpha, newmarkBeta, newmarkGamma);

      v3 = vtilde + (newmarkGamma*dt)*a3;
      x3 = dtilde + (newmarkBeta*dt*dt)*a3;

      if( false )
      { // --- debug output ---
        RealArray xpe, vpe;
	xpe = x2 + dt*v2+ (.5*dt*dt)*a2;
	vpe = v2 + dt*a2;
	printF("--BM-- t=%8.2e: predicted: |vpi-vpe|=%8.2e, |xpi-xpe|=%8.2e |a2-a3|=%8.2e\n",
	       t,max(fabs(v3-vpe)), max(fabs(x3-xpe)), max(fabs(a3-a2)));
      
        getErrors( tnp1, xpe,vpe,a3,sPrintF("-- BM : xpe,vp,a3, after predict t=%9.3e",tnp1));
        getErrors( tnp1, x3,v3,a3,sPrintF("-- BM : x3,v3,a3, after predict t=%9.3e",tnp1));
	// ::display(f3,"f3","%8.2e ");
	// ::display(a3,"a3","%8.2e ");
	// ::display(f2,"f2","%8.2e ");
	// ::display(a2,"a2","%8.2e ");

	::display(vpe,"vpe","%8.2e ");
	::display(v3,"v3","%8.2e ");
      }
      
    }
    else
    {
      x3 = x2 + dt*v2+ (.5*dt*dt)*a2;
      v3 = v2 + dt*a2;
      a3 = a2;     // predicted acceleration
    }
    
  }
  else
  { //  -- use first order predictor
    x3 = dtilde;
    v3 = vtilde;
    a3 = a2;     // predicted acceleration
  }
  
  // *wdh* aold = 0.0;
  aold = a2;   // set aold to previous acceleration *wdh* 2014/06/19 

  if( false ) // do not apply BC's to first-order predictor 
    assignBoundaryConditions( tnp1, dtilde,vtilde,a3 );

  if (allowsFreeMotion) 
  {
    assert( !useSecondOrderNewmarkPredictor );
    
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

    if (bcLeft == pinned || bcLeft == clamped) 
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

  
  memset(old_rb_acceleration,0,sizeof(old_rb_acceleration));

  //printArray(x2,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  
  if( debug & 2 )
  {
    aString buff;
    getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM : after %s predict t=%9.3e",
				      (useSecondOrderNewmarkPredictor ? "2nd-order" : "first-order"),tnp1));
  }
  

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
/// /param tnp1 (input): new time t^{n+1} 
/// /param dt (input) :  current time step
///
/// /notes: The Newmark beta scheme for
///          u_t = v
///          v_t = a 
///         M a = f - K u 
///  is    
///        unp1 = un + dt*vn + (dt^2/2)*( (1-2*beta) an + 2*beta*anp1 )
///       M anp1 = fnp1 - Kunp1
///  The acceleration is computed from 
///      ( M + dt^2*beta*K ) anp1 = fnp1 - K*[ un + dt*vn + (dt^2/2)*(1-2*beta)*an ]
///      
// ===================================================================================
void BeamModel::
corrector(real tnp1, real dt )
{
  const bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  
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

  if( fabs(time(current)-tnp1) > 1.e-10*(1.+tnp1) )
  {
    printF("--BM-- BeamModel::corrector:ERROR: tnp1=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
        tnp1,time(current),current);
    OV_ABORT("ERROR");
  }

  RealArray A = evaluate(elementM+newmarkBeta*dt*dt*elementK);
    
  // add internalforces such as buoyance and TZ forcing
  addInternalForces( tnp1, f3 );

  // if( projectedBodyForce*buoyantMassPerUnitLength != 0. )
  // {
  //   RealArray lt(4);
  //   for (int i = 0; i < numElem; ++i) 
  //   {
  //     Index idx(i*2,4);
  //     computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
  // 			    -1.0,1.0, lt);
  //     //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
  //     f3(idx) += lt;
  //   }
  // }
  
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
  // compute acceleration at time t^{n+1}
  const real alpha=newmarkBeta*dt*dt;  // coeff of K in A
  computeAcceleration(tnp1, dtilde,v3,f3, A, a3,  linaccel,omegadd,dt, alpha, newmarkBeta, newmarkGamma);

  //v3 = vtilde+newmarkGamma*dt*(myAcceleration-aold)*omega;
  //x3 = dtilde+newmarkBeta*dt*dt*(myAcceleration-aold)*omega;

  v3 = vtilde+newmarkGamma*dt*a3;
  x3 = dtilde+newmarkBeta*dt*dt*a3;

  if( debug & 4 )
  {
    int nn=numElem+1;
    v3.reshape(2,nn); x3.reshape(2,nn); a3.reshape(2,nn); f3.reshape(2,nn);
    printF("-- BM -- corrector: tnp1=%9.3e, dt=%9.3e\n",tnp1,dt);
    ::display(f3,"-- BM -- corrector: f3","%8.2e ");
    ::display(x3,"-- BM -- corrector: u3","%8.2e ");
    ::display(v3,"-- BM -- corrector: v3","%8.2e ");
    ::display(a3,"-- BM -- corrector: a3","%8.2e ");
    v3.reshape(2*nn); x3.reshape(2*nn); a3.reshape(2*nn); f3.reshape(2*nn);
  }

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
  
  if( debug & 2 )
  {
    aString buff;
    getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM : after correct t=%9.3e",tnp1));
  }
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
/// \brief  Compute errors in the solution (when the solution is known).
// =================================================================================================
int BeamModel::
getErrors( const real t, const RealArray & u, const RealArray & v, const RealArray & a,const aString & label )
{

  const real beamLength = L;
  
  RealArray ue, ve, ae;
  getExactSolution( t, ue, ve, ae );

  printF("-- BM -- %s: Errors at t=%9.3e:\n",(const char*)label,t);
  real errMax=0., l2Err=0., yNorm=0.;
  for( int i = 0; i <= numElem; ++i )
  {
    real xl = ( (real)i /numElem) *  beamLength;

    real we = ue(i*2);  // exact solution
    real err = fabs( u(i*2) - we );

    real verr = fabs( v(i*2) - ve(i*2) );

    if( debug & 2 )
      printF("-- BM -- t=%8.2e i=%3i u=%9.2e ue=%9.2e err=%9.2e, v=%9.2e ve=%9.2e err=%8.2e\n",t,i,u(2*i),we,err, v(2*i),ve(2*i),verr);
      
    errMax=max(errMax,err);
    l2Err += SQR(err);
    yNorm=yNorm+SQR(we);

  }
  l2Err=sqrt(l2Err/(numElem+1));
  yNorm=sqrt(yNorm/(numElem+1));

  printF("-- BM -- Summary: Error t=%9.3e : max=%8.2e, l2=%8.2e, l2-rel=%8.2e\n",t,errMax,l2Err,l2Err/max(1.e-12,yNorm));

  return 0;
}



// =================================================================================================
/// \brief  Define the BeamModel parameters interactively.
// =================================================================================================
int BeamModel::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  real & tension = dbase.get<real>("tension");
  bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  
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
  bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");

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
    aString bcOptions[] = { "clamped",
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

    aString exactOptions[] = { "none",
			       "standing wave",
			       "traveling wave FSI",
			       "" };

    GUIState::addPrefix(exactOptions,"Exact solution:",cmd,maxCommands);
    dialog.addOptionMenu("Exact solution:",cmd,cmd,(exactSolutionOption=="none" ? 0 : 
                          exactSolutionOption=="standingWave" ? 1 : 2) );


    aString tbCommands[] = {"use exact solution",
                            "save profile file",
                            "save tip file",
                            "twilight-zone",
                            "use second order Newmark predictor",
                            "use new tridiagonal solver",
                            "use implicit predictor",
    			    ""};
    int tbState[10];
    tbState[0] = useExactSolution;
    tbState[1] = dbase.get<bool>("saveProfileFile");
    tbState[2] = dbase.get<bool>("saveTipFile");
    tbState[3] = twilightZone;
    tbState[4] = useSecondOrderNewmarkPredictor;
    tbState[5] = useNewTridiagonalSolver;
    tbState[6] = useImplicitPredictor;
    
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
      sScanF(answer(len,answer.length()-1),"%e %e %e",&beamX0,&beamY0,&beamZ0);
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
      
      bcValue= (bcOption=="clamped"  ? clamped :
                bcOption=="cantilever"  ? clamped : // for backward compatibility
                bcOption=="pinned"   ? pinned :
                bcOption=="free"     ? freeBC : 
                bcOption=="periodic" ? periodic : unknownBC );

      if( bcValue==unknownBC )
      {
	printF("ERROR: unknown BC : answer=[%s], bcOption=[%s]\n",(const char*)answer,(const char*)bcOption);
	gi.stopReadingCommandFile();
      }

      printF("BeamModel:INFO: setting %s = %s.\n",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcOption);

    }
    else if( dialog.getToggleValue(answer,"use new tridiagonal solver",useNewTridiagonalSolver) ){} // 
    else if( dialog.getToggleValue(answer,"use second order Newmark predictor",useSecondOrderNewmarkPredictor) )
    {
      if(  useSecondOrderNewmarkPredictor )
	printF("-- BM -- use SECOND order Newmark predictor\n");
      else
	printF("-- BM -- use FIRST order Newmark predictor\n");
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
    else if( dialog.getToggleValue(answer,"use implicit predictor",useImplicitPredictor) ){}//

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
    else if( len=answer.matches("Exact solution:") )
    {
      aString option = answer(len,answer.length()-1);
      if( option=="none" )
	exactSolutionOption="none";
      else if( option=="standing wave" )
        exactSolutionOption="standingWave";
      else if( option=="traveling wave FSI" )
        exactSolutionOption="travelingWaveFSI";
      else
      {
	printF("ERROR: unknown exact solution=[%s]\n",(const char*)option);
	gi.stopReadingCommandFile();
      }
      printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
      
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

  if( true )
  {
    writeParameterSummary();
  }
  
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



// =================================================================================
  // Return the exact velocity of the FLUID for the FSI analytical solution
  // derived in the documentation
  // (x,y):      point in the fluid grid where the exact velocity is desired
  // t:          Time at which to compute the exact solution
  // k:          Wave number for the exact solution being computed
  // H:          Height of the fluid domain
  // omega_real: real part of the angular frequency (see documentation)
  // omega_imag: imaginary part of the angular frequency (see documentation)
  // omega0:     Natural (free) frequency of the beam
  // nu:         fluid kinematic viscosity
  // what:       magnitude of the beam deformation
  // u:          fluid velocity (x) [out]
  // v:          fluid velocity (y) [out]
  //
// =================================================================================
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


  // Return the exact pressure of the FLUID for the FSI analytical solution
  // derived in the documentation
  // (x,y):      point in the fluid grid where the exact velocity is desired
  // t:          Time at which to compute the exact solution
  // k:          Wave number for the exact solution being computed
  // H:          Height of the fluid domain
  // omega_real: real part of the angular frequency (see documentation)
  // omega_imag: imaginary part of the angular frequency (see documentation)
  // omega0:     Natural (free) frequency of the beam
  // nu:         fluid kinematic viscosity
  // what:       magnitude of the beam deformation
  // p:          fluid pressure (x) [out]
  //
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

