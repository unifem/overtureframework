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
  dbase.put<real>("K0")=0.;       //  coefficient of -w
  dbase.put<real>("Kt")=0.;       //  coefficient of -w_t
  dbase.put<real>("Kxxt")=0.;     //  coefficient of w_{xxt} 
  dbase.put<real>("ADxxt")=0.;    //  artificial dissipation coefficient

  newmarkBeta = 0.25;
  newmarkGamma = 0.5;

  // newmarkBeta = 0.5;
  // newmarkGamma = 1.;

  numberOfTimeSteps = 1;

  pressureNorm = 1.; // 1000.0;  // scale pressure forces by this factor

  hasAcceleration = false;

  bcLeft = bcRight = pinned;
  //bcLeft = bcRight = clamped;

//  added_mass_relaxation = 1.0;

  numCorrectorIterations = 0;

  // convergenceTolerance = 1e-3;

  allowsFreeMotion = false;

  beamX0=beamY0=beamZ0=0.;
  
  // -- free motion parameters ---
  centerOfMass[0] = 0.0;
  centerOfMass[1] = 0.0;
  angle = 0.0;   // angle of beam for free motion
  bodyForce[0] = bodyForce[1] = 0.0;
  

  // -- beam is by default horizontal --
  beamInitialAngle = 0.0;  // angle of undeformed beam 

  initialBeamTangent[0] = 1.0;
  initialBeamTangent[1] = 0.0;

  initialBeamNormal[0] = 0.0;
  initialBeamNormal[1] = 1.0;

  dbase.put<real>("signForNormal") = 1.;  // flip sign of normal using this parameter

  // useSmallDeformationApproximation : adjust the beam surface acceleration and surface "internal force"
  //    assuming small deformations
  dbase.put<bool>("useSmallDeformationApproximation")=true;

  projectedBodyForce = 0.0;

  for (int k = 0; k < 2; ++k) {

    normal[k] = initialBeamNormal[k];
    tangent[k] = initialBeamTangent[k];
  }

  penalty = 1e2;

  leftCantileverMoment = 0.0;

  RealArray & elementB = dbase.put<RealArray>("elementB");  // holds "damping element matrix"
  elementB.redim(4,4);

  if( !dbase.has_key("saveProfileFile") ) 
     dbase.put<bool>("saveProfileFile")=false;

  if( !dbase.has_key("saveProbeFile") ) 
     dbase.put<bool>("saveProbeFile")=false;

  // save probe file results every this many time-steps:
  dbase.put<int>("probeFileSaveFrequency")=5;
  dbase.put<real>("probePosition")=1.; // probe position in [0,1], 1=end point at x=L

  useExactSolution=false;

  dbase.put<real>("cfl")=10.;  // scale explicit dt by this cfl number (scheme is implicit)

  dbase.put<bool>("useImplicitPredictor")=true;

  dbase.put<bool>("relaxForce")=true;
  dbase.put<RealArray>("fOld");
  dbase.put<RealArray>("fOlder");

  // Set relaxCorrectionSteps to true for iterating for added mass effects
  dbase.put<bool>("relaxCorrectionSteps")=false;
  // The relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  dbase.put<real>("addedMassRelaxationFactor",1.0); 

  // The (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  dbase.put<real>("subIterationConvergenceTolerance",1.0e-3);
  dbase.put<real>("subIterationAbsoluteTolerance",1.e-8);
  dbase.put<real>("maximumRelativeCorrection")=0.;
  dbase.put<bool>("useAitkenAcceleration")=false;

  // We can optionally smooth the solution with a fourth-order filter
  dbase.put<bool>("smoothSolution")=false;
  dbase.put<int>("numberOfSmooths")=2;
  dbase.put<int>("smoothOrder")=6;  // sixth-order filter by default
  dbase.put<real>("smoothOmega")=1.;  // parameter in filter, normaLLY <= 1

  // For initial conditions: 
  dbase.put<real>("amplitude")=0.1;
  dbase.put<real>("waveNumber")=1.0;

  // For scaling displacement when plotting
  dbase.put<real>("displacementScaleFactorForPlotting")=1.;

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

  // Here is the tri-diagonal solver for the time advance 
  if( !dbase.has_key("tridiagonalSolver") ) dbase.put<TridiagonalSolver*>("tridiagonalSolver")=NULL;

  // Tridiagonal solver for the Galerkin projection:
  if( !dbase.has_key("galerkinProjection") ) dbase.put<TridiagonalSolver*>("galerkinProjection")=NULL;

  // Tridiagonal solver for computing the right-hand-side in getSurfaceInternalForce (used by the AMP scheme)
  if( !dbase.has_key("rhsSolver") ) dbase.put<TridiagonalSolver*>("rhsSolver")=NULL;
  if( !dbase.has_key("rhsSolverAddExternalForcing") ) dbase.put<bool>("rhsSolverAddExternalForcing",0);

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

  dbase.put<real>("standingWaveTimeOffset")=0.; // time offset for standing wave solution

  dbase.put<bool>("fluidOnTwoSides")=true; // The beam has fluid on both sides (for projecting the velocity)

  dbase.put<int>("orderOfGalerkinProjection")=2; // order of accuracy of the Galerkin projection (force and velocity)

  // File to which the probe data (e.g. tip displacement, velocity etc.) is written
  dbase.put<FILE*>("probeFile")=NULL;
  dbase.put<aString>("probeFileName")="beamProbeFile.text";
  
  // check file:
  FILE *& checkFile = dbase.put<FILE*>("checkFile");
  checkFile = fopen("BeamModel.check","w" );   // Here is the check file for regression tests


}

// ======================================================================================================
/// \brief Destructor.
// ======================================================================================================
BeamModel::~BeamModel() 
{
  if( dbase.get<FILE*>("checkFile")!=NULL )
    fclose(dbase.get<FILE*>("checkFile"));

  if( dbase.get<bool>("saveProbeFile") &&  dbase.get<FILE*>("probeFile")!=NULL )
    fclose(dbase.get<FILE*>("probeFile"));

  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
  if( pTri!=NULL )
    delete pTri;
  
  pTri = dbase.get<TridiagonalSolver*>("galerkinProjection");
  if( pTri!=NULL )
    delete pTri;
  
  pTri = dbase.get<TridiagonalSolver*>("rhsSolver");
  if( pTri!=NULL )
    delete pTri;
  

}

// ======================================================================================================
/// \brief return the value of a integer parameter
/// \return value : 0=success, 1=not found
// ======================================================================================================
int BeamModel::
getParameter( const aString & name, int & value ) const
{
  if( dbase.has_key(name) )
  {
    value=dbase.get<int>(name);
  }
  else
  {
    printF("BeamModel::getParameter: ERROR: did not find parameter with name=[%s]\n",(const char*)name);
    return 1;
  }
  
  return 0;
}

// ======================================================================================================
/// \brief return the value of a real parameter
/// \return value : 0=success, 1=not found
// ======================================================================================================
int BeamModel::
getParameter( const aString & name, real & value ) const
{
  if( dbase.has_key(name) )
  {
    value=dbase.get<real>(name);
  }
  else if( name=="thickness" )
  {
    value=thickness;
  }
  else if( name=="density" )
  {
    value=density;
  }
  else if( name=="length" )
  {
    value=L;
  }
  else
  {
    printF("BeamModel::getParameter: ERROR: did not find parameter with name=[%s]\n",(const char*)name);
    return 1;
  }
  
  return 0;
}



// ======================================================================================================
/// \brief Write a summary of the Beam model parameters and boundary conditions etc.
// ======================================================================================================
void BeamModel::
writeParameterSummary( FILE *file /* = stdout */ )
{
  const real & T = dbase.get<real>("tension");
  const bool & relaxForce = dbase.get<bool>("relaxForce");
  const bool & relaxCorrectionSteps=dbase.get<bool>("relaxCorrectionSteps");
  const real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  const real & subIterationAbsoluteTolerance = dbase.get<real>("subIterationAbsoluteTolerance");
  const real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  const bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  const bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");
  const bool & useSmallDeformationApproximation = dbase.get<bool>("useSmallDeformationApproximation");
  const bool & useAitkenAcceleration = dbase.get<bool>("useAitkenAcceleration");
  const bool & smoothSolution = dbase.get<bool>("smoothSolution");
  const int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  const int & smoothOrder     = dbase.get<int>("smoothOrder");
  const real & smoothOmega = dbase.get<real>("smoothOmega");
  

  fPrintF(file," --------------------------------------------------------------------------------\n");
  fPrintF(file,"                        Beam Model\n");
  fPrintF(file," --------------------------------------------------------------------------------\n");
  fPrintF(file," Type: Euler-Bernoulli beam. beamID=%i, name=%s\n",beamID,(const char*)name);
  fPrintF(file,"     (density*thickness*b)*w_tt = -K0 w + T w_xx - EI w_xxxx -Kt w_t + Kxxt w_xxt \n");
  fPrintF(file," E=%9.3e, I=%9.3e, T=%8.2e, K0=%8.2e, Kt=%8.2e Kxxt=%8.2e  ADxxt=%8.2e \n"
               " density=%9.3e, length=%9.3e, thickness=%9.3e, b=breadth=1, initial-angle=%7.3f (degrees) \n"
               " numElem=%i, allowsFreeMotion=%i, initial left end=(%12.8e,%12.8e,%12.8e)\n"
               " Newmark time-stepping, beta=%g, gamma=%g, cfl=%g,\n"
               "     useSecondOrderNewmarkPredictor=%i, useImplicitPredictor=%i,\n"
               " pressureNormalization = %8.2e (scale pressure forces by this factor)\n"
               " useSmallDeformationApproximation = %i (adjust surface accelerations assuming small deformations)\n"
               " relaxCorrectionSteps=%i, relaxForce=%i, use-Aitken-acceleration=%i\n"
               " relaxation factor=%g, sub-iteration tol=%8.2e, absolute-tol=%8.2e\n"
               " smooth solution=%i, number of smooths=%i (%ith-order filter), omega=%5.3f\n"
               " fluidOnTwoSides=%i, orderOfGalerkinProjection=%i, useNewTridiagonalSolver=%i\n"
	  , elasticModulus,areaMomentOfInertia,T,dbase.get<real>("K0"),dbase.get<real>("Kt"),dbase.get<real>("Kxxt"),
          dbase.get<real>("ADxxt"),
          density,L,thickness,beamInitialAngle*180./Pi,
	  numElem,(int)allowsFreeMotion,beamX0,beamY0,beamZ0,
          newmarkBeta,newmarkGamma,dbase.get<real>("cfl"),
	  (int)dbase.get<bool>("useSecondOrderNewmarkPredictor"),
          (int)useImplicitPredictor,
	  pressureNorm,(int)useSmallDeformationApproximation,
          (int)relaxCorrectionSteps,(int)relaxForce,(int)useAitkenAcceleration,
          addedMassRelaxationFactor,subIterationConvergenceTolerance,
          subIterationAbsoluteTolerance,
          (int)smoothSolution,numberOfSmooths,smoothOrder,smoothOmega,
          (int)dbase.get<bool>("fluidOnTwoSides"), 
          dbase.get<int>("orderOfGalerkinProjection"),
          (int)useNewTridiagonalSolver);

  aString bcName;
  for( int side=0; side<=1; side++ )
  {
    BoundaryCondition bc = side==0 ? bcLeft : bcRight;
    bcName = (bc==pinned ? "pinned" : 
              bc==clamped ? "clamped" :
              bc==slideBC ? "slide" :
              bc==periodic ? "periodic" : 
              bc==freeBC ? "free" : "unknown");
    
    fPrintF(file," %s=%s, ",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcName);
  }
  fPrintF(file,"\n");

  const bool & twilightZone = dbase.get<bool>("twilightZone");
  const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  const int & degreeInTime = dbase.get<int>("degreeInTime");
  const int & degreeInSpace = dbase.get<int>("degreeInSpace");
  const real & signForNormal = dbase.get<real>("signForNormal");
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  fPrintF(file," twilightZone=%s, option=%s. Poly: degreeT=%i, degreeX=%i, Trig: ft=%g, fx=%g\n",(twilightZone ? "on" : "off"),
          (twilightZoneOption==0 ? "polynomial" : "trigonometric"),
	  degreeInTime,degreeInSpace,trigFreq[0],trigFreq[1] );
  fPrintF(file," Exact solution option: %s\n",(const char*)exactSolutionOption);
  fPrintF(file," Initial condition option: %s\n",(const char*)initialConditionOption);
  
   fPrintF(file," Initial beam normal=[%6.4f,%6.4f], tangent=[%6.4f,%6.4f], signForNormal=%g.\n",
	   initialBeamNormal[0],initialBeamNormal[1],initialBeamTangent[0],initialBeamTangent[1],
         signForNormal );

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
  const real & K0 = dbase.get<real>("K0");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  if( false && Kxxt!=0. )
  {
    OV_ABORT("--BeamModel: ERROR: Kt!=0 or Kxxt!=0 -- this term not implemented yet");
  }
  

  // std::cout << "EI = " << EI << std::endl;

  // *wdh* 2014/06/17 -- tension term added

  // Tension matrix from (v_x,w_x)
  RealArray elementT(4,4);
  elementT(0,0) = 6./(5.*le);    
  elementT(0,1) = 1./10.; 
  elementT(0,2) = -elementT(0,0); 
  elementT(0,3) = elementT(0,1);
  elementT(1,0) = elementT(0,1);  
  elementT(1,1) = le*2./15.; 
  elementT(1,2) = -elementT(0,1); 
  elementT(1,3) = - le/30.;
  elementT(2,0) = elementT(0,2); 
  elementT(2,1) = elementT(1,2); 
  elementT(2,2) = elementT(0,0); 
  elementT(2,3) = elementT(1,2);
  
  elementT(3,0) = elementT(0,1); 
  elementT(3,1) = elementT(1,3); 
  elementT(3,2) = elementT(2,3);
  elementT(3,3) = elementT(1,1);


  // Stiffness element matrix from beam term: (v_xx, EI w_xx) 
  elementK(0,0) = EI*12./le3;
  elementK(0,1) = EI*6./le2;
  elementK(0,2) = -elementK(0,0); 
  elementK(0,3) = elementK(0,1);

  elementK(1,0) = elementK(0,1);  
  elementK(1,1) = EI*4./le;
  elementK(1,2) = -elementK(0,1); 
  elementK(1,3) = EI*2./le;

  elementK(2,0) = elementK(0,2); 
  elementK(2,1) = elementK(1,2); 
  elementK(2,2) = elementK(0,0); 
  elementK(2,3) = elementK(1,2);
  
  elementK(3,0) = elementK(0,1); 
  elementK(3,1) = elementK(1,3); 
  elementK(3,2) = elementK(2,3);
  elementK(3,3) = elementK(1,1);
  
  // Scaled element mass matrix (v,w):
  elementM(0,0) = elementM(2,2) = 13./35.*le;
  elementM(0,1) = elementM(1,0) = 11./210.*le2;
  elementM(0,2) = elementM(2,0) = 9./70.*le;
  elementM(0,3) = elementM(3,0) = -13./420.*le2;
  elementM(1,1) = elementM(3,3) = 1./105.*le3;
  elementM(1,2) = elementM(2,1) = 13./420.*le2;
  elementM(1,3) = elementM(3,1) = -1./140.*le3;
  elementM(3,2) = elementM(2,3) = -11./210.*le2;

  // Add linear stiffness term : K0*(v,w) 
  // *wdh* 2014/12/25 -- Stiffness term -K0*w added,
  //    Stiffness matrix entries from :  -K0*(v,w)   (like Mass matrix)
  elementK += K0*elementM;

  // Element damping matrix B:
  RealArray & elementB = dbase.get<RealArray>("elementB");
  elementB = Kt*elementM + Kxxt*elementT;

  // Actual mass matrix: 
  const real Abar =density*thickness*breadth;
  elementM *= Abar;

  // Stiffness element matrix including "tension term":
  elementK += T*elementT;

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

// ======================================================================================================
/// \brief Set a real beam parameter (that is in the class DataBase)
/// \param name (input) : name of a parameter in the dbase 
/// \param value (input) : value to assign
/// \return value : 0=success, 1=name not found 
// ======================================================================================================
int BeamModel::
setParameter( const aString & name, real & value ) 
{
  if( dbase.has_key(name) )
    dbase.get<real>(name)=value;
  else
  {
    printF("BeamModel::setParameter:ERROR: there is no real parameter named [%s]\n",(const char*)name);
    return 1;
  }
  
  return 0;
}


// ======================================================================================================
/// \brief Return an estimate of the time-step dt. 
// ======================================================================================================
real BeamModel::
getTimeStep() const
{
  real dt = getExplicitTimeStep();

  return dt;
}


// ==================================================================
/// \brief return the estimated *explicit* time step dt 
/// \auhtor WDH
// ==================================================================
real BeamModel::
getExplicitTimeStep() const
{
  // estimate the expliciit time step 

  // int numNodes=numElem+1;
  real beamLength=L;
  real dx = beamLength/numElem; 
  
  const real EI = elasticModulus*areaMomentOfInertia;
  const real & T = dbase.get<real>("tension");
  const real & K0 = dbase.get<real>("K0");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  
  // Guess the explicit time step: 
  //  ( c4*E*I*dt^2/dx^4 + C2*T*dt^2/dx^2 )/( rho*h*b ) < 1 

  const real cfl = dbase.get<real>("cfl");
  const real rhosAs= density*thickness*breadth;
  real dt, dtOld;
  if( true )
  {
    // *OLD WAY*
    const real c4=1., c2=4.;
    // ***fix me for Kt and Kxxt ***
    // if( Kt!=0. )
    //   printF("--BM-- WARNING: dt has NOT been adjusted for -Kt*w_t\n");
    // if( Kxxt!=0. )
    //   printF("--BM-- WARNING: dt has NOT been adjusted for Kxxt*w_xxt\n");

    dtOld = cfl*sqrt(  rhosAs /(  K0 + c4*EI/pow(dx,4) + c2*T/(dx*dx) ) );
  
  }

  if( true )
  {
    // **NEW WAY**
    // 
    // Compute the time stepping eigenvalue from the first order system
    //     u' = v 
    //   rhos*As*v' = - K0*u - Kt*v + T*uxx + Kxxt*vxxt - EI*uxxxx
    // 
    real dx2=dx*dx, dx4=dx2*dx2;
    real Bhat = ( Kt + Kxxt*(4./dx2) )/rhosAs;               // damping coefficient
    real Ahat = ( K0 + T*(4./dx2) + EI*(16./dx4 ))/rhosAs;   // 

    // Guess: explicit stability region goes to -1 on the real axis and 1 on the imaginary
    const real alpha=1., beta=1;   
  
    real lambdaReal=0, lambdaIm=0.;
    if( Bhat < 2*sqrt(Ahat) )
    {
      lambdaReal = -Bhat*.5;
      lambdaIm   =  sqrt( Ahat - SQR(Bhat*.5) );

      dt = cfl/sqrt( SQR(lambdaReal/alpha) + SQR(lambdaIm/beta) );
    }
    else
    {
      lambdaReal = Bhat*.5 + sqrt( SQR(Bhat*.5) - Ahat );

      dt = cfl*alpha/lambdaReal;
    }
  
  }
  


  if( true || debug & 1 )
    printF("BeamModel::getExplicitTimeStep: rho=%g, rho*A=%g, EI=%g, T=%g, K0=%g, Kt=%g, Kxxt=%g, dx=%8.2e, dt=%8.2e (dt-oldway=%8.2e) (cfl=%g).\n",
	   density,density*thickness*breadth,EI,T,K0,Kt,Kxxt, dx,dt,dtOld,cfl);

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
/// \brief *OLD* Compute the integral of N(eta)*p, that is, the rhs of the FEM model, for a particular element
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

// ======================================================================================================
/// \brief Compute the local contribution to the Galerkin projection of a function f(x) 
///   onto the Hermite FEM representation.
/// f(x) is represented as an Hermite polynomial on the interval [a,b]:
///     f(xi) = fa*N1(yi) + fap*N2(yi) + fb*N3(yi) + fbp*N4(yi);
///     yi := xi*(b-a)/2 + (b+a)/2; # map N to the interval [a,b] 
///  
/// /param fa,fap : f and f' at xi=a
/// /param fb,fbp : f and f' at xi=b
/// /param a,b : sub-interval fo xi=[-1,1]
/// /param f (output): 
///    f(k)= int_a^b f(xi) N_{k+1} J dxi 
// ======================================================================================================
void BeamModel::
computeGalerkinProjection(real fa, real fap, real fb, real fbp, 
			  real a, real b,
			  RealArray &  f ) 
{
  real g1,g2,g3,g4;
  real le = L / numElem;    // length of an element 
  real dxab = le*(b-a)*.5;  // length of xi sub-interval [a,b]
   
  // File generated by cgDoc/moving/codes/beam/beam.maple :
  #include "elementIntegrationHermiteOrder4.h"

  f(0)=g1;
  f(1)=g2;
  f(2)=g3;
  f(3)=g4;

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
  inv(0,0) =  odet*A(1,1);
  inv(0,1) = -odet*A(0,1);
  inv(1,0) = -odet*A(1,0);
  inv(1,1) =  odet*A(0,0);
}

// ================================================================================
/// \brief Solve A u = f
/// 
/// \param Ae (input) : "element" matrix for A 
/// \param alpha (input) coefficient of Ke in A (used in adjusting the matrix for boundary terms)
/// \param alphaB (input) coefficient of Be in A (used in adjusting the matrix for boundary terms)
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
//
//     Ae = Me + alphaB*Be + alpha*Ke 
//     Me = element mass matrix 
//     Ke = element stiffness matrix 
// ================================================================================
void BeamModel::
solveBlockTridiagonal(const RealArray& Ae, const RealArray& f, RealArray& u, 
                      const real alpha, const real alphaB, const aString & tridiagonalSolverName )
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

  const real & T = dbase.get<real>("tension");
  const real & Kxxt = dbase.get<real>("Kxxt");

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
    
    // TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("tridiagonalSolver");
    TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
    if( pTri==NULL )
    {
      pTri = new TridiagonalSolver();
      refactor=true;
    }
    
    assert( pTri!=NULL );

    TridiagonalSolver & tri = *pTri;

    RealArray at0(ndof,ndof,I1,I2), bt0(ndof,ndof,I1,I2), ct0(ndof,ndof,I1,I2); // save for checking
    if( refactor )
    {
      if( true || debug & 1 )
        printF("-- BM -- solveBlockTridiagonal : name=[%s] form block tridiagonal system and factor, isPeriodic=%i\n",
	       (const char*)tridiagonalSolverName, (int)isPeriodic);
      
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
	  else if( bc == pinned ) 
	  {
	    // replace first equation in first 2x2 block by the identity
	    if( side==0 )
	      ct(0,D,ia,0)=0.; 
	    else
	      at(0,D,ia,0)=0.;
	    
	    bt(0,0,ia,0)=1.; bt(0,1,ia,0)=0.; 
	  }
	  else if( bc == slideBC ) 
	  {
	    // replace second equation in first 2x2 block by the identity
	    if( side==0 )
	      ct(1,D,ia,0)=0.; 
	    else
	      at(1,D,ia,0)=0.;
	    
	    bt(1,0,ia,0)=0.; bt(1,1,ia,0)=1.; 
	  }
	  else if( bc == freeBC )
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

            // The boundary term K_xxt*v*w_xt also contributes
            bt(0,1,ia,0) +=  (1-2*side)*Kxxt*alphaB;

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

      if( bc == BeamModel::clamped )
      {
	diagonal[0](0,0) = diagonal[0](1,1) = 1.0;
	diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
	superdiagonal[0](0,0) = superdiagonal[0](0,1) = 0.0;
	superdiagonal[0](1,1) = superdiagonal[0](1,0) = 0.0;

      }
      else if ( bc == BeamModel::pinned ) 
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
	diagonal[0](0,1) +=  Kxxt*alphaB;
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
	diagonal[numElem](0,1) +=  -Kxxt*alphaB;
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
      printF(" NOTE: this difference could be due to the boundary conditions not being\n"
             "       fully implemented in the old scheme\n");
      OV_ABORT("error");
    }
  }
  


}



// =======================================================================================
/// /brief  Compute the internal force in the beam, f = -B*v -K*u
/// /param u (input) : position of the beam 
/// /param v (input) : velocity of the beam
/// /param f (output) :internal force [out]
// =======================================================================================
void BeamModel::
computeInternalForce(const RealArray& u, const RealArray& v, RealArray& f) 
{

  RealArray elementU(4);
  RealArray elementForce(4);

  f = 0.0;
  for( int i = 0; i < numElem; ++i )
  {
    // elementU = [ u_i, ux_i, u_{i+1} ux_{i+1} ]
    for (int k = 0; k < 4; ++k)
      elementU(k) = u(i*2+k);
    
    elementForce = mult(elementK, elementU);
    for( int k = 0; k < 4; ++k )
      f(i*2+k) -= elementForce(k);
  }

  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  if( Kt!=0. || Kxxt!=0. )
  {
    // add damping terms to internal force
    RealArray & elementB = dbase.get<RealArray>("elementB");
    RealArray & elementV = elementU; // reuse space
    for( int i = 0; i < numElem; ++ i)
    {
      // elementV = [ v_i, vx_i, v_{i+1} vx_{i+1} ]
      for (int k = 0; k < 4; ++k )
	elementV(k) = v(i*2+k);
    
      elementForce = mult(elementB, elementV);
      for( int k = 0; k < 4; ++k )
	f(i*2+k) -= elementForce(k);
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
/// \param xc (output) : center line coordinates
/// \param scaleDisplacementForPlotting (input) : if true, scale the displacement of the beam for
///      plotting purposes by the factor "displacementScaleFactorForPlotting"
/// \author wdh 2014/05/22
//==============================================================================================
void BeamModel::
getCenterLine( RealArray & xc, bool scaleDisplacementForPlotting /* =false */ ) const
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  RealArray & uc = u[current];

  // Optionally scale the displacement for plotting purposes:
  real scaleFactor=1.;
  if( scaleDisplacementForPlotting )
    scaleFactor= dbase.get<real>("displacementScaleFactorForPlotting");

  xc.redim(numElem+1,2);
  for( int i=0; i<=numElem; i++ ) 
  {
    // (xl,yl) = beam position (un-rotated)
    real xl = ((real)i /numElem) *  L;       // position along neutral axis 
    real yl = uc(2*i)*scaleFactor;           // displacement 

    // *wdh* 2018/02/28 xc(i,0) = beamX0 + initialBeamTangent[0]*xl - initialBeamTangent[1]*yl;
    // *wdh* 2018/02/28 xc(i,1) = beamY0 - initialBeamNormal [0]*xl + initialBeamNormal [1]*yl;
    xc(i,0) = beamX0 + initialBeamTangent[0]*xl + initialBeamNormal[0]*yl;
    xc(i,1) = beamY0 + initialBeamTangent[1]*xl + initialBeamNormal[1]*yl;
  }

}


//==============================================================================================
/// \brief Get beam reference coordinates and direction array (indicates which side of the beam)
/// \param x0(i) (input) : coordinates on the surface of the UNDEFORMED beam.
/// \param s0(i) (output) : beam reference coordinates in [-1,1]
/// \param elementNumber(i) (output) : element number
/// \param signedDistance(i) (output) : signed distance to x0(i)
//==============================================================================================
int BeamModel::
getBeamReferenceCoordinates( const RealArray & x0, RealArray & s0, IntegerArray & elementNumber,
                             RealArray & signedDistance )
{
  const real beamLength=L;
  const real dx = le/beamLength;
  Range I=x0.dimension(0);
  for( int i=I.getBase(); i<=I.getBound(); i++ )
  {
    real eta; // natural coordinate on the element [-1,1]
    projectPoint( x0(i,0),x0(i,1), elementNumber(i),eta,signedDistance(i));

    // // elemNum : closest node less than point xl:
    // elemNum = (int)(xl / le);
    // eta = 2.0*(xl-le*elemNum)/le-1.0;

    s0(i) = (elementNumber(i)+ (eta+1.)*.5)*dx; // parameter coordinate on whole beam unit interval [0,1]
    
    // printF("--BM-- getBeamReferenceCoordinates: i=%i, x0=%g y0=%g s0=%g\n",i,x0(i,0),x0(i,1),s0(i));
  }

  return 0;
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
/// \brief Compute the acceleration of the beam.
///
/// \param u (input):               current beam position (For Newmark this is un + dt*vn + .5*dt^2*(1-2*beta)*an )
/// \param  (input)v:               current beam velocity (NOT USED CURRENTLY)
/// \param f (input):               external force on the beam
/// \param A (input):               matrix by which the acceleration is multiplied
///                  (e.g., in the newmark beta correction step it is 
///                   M+beta*dt^2*K)
/// \param a (input):               beam acceleration [out]
/// \param linAcceleration (input): acceleration of the CoM of the beam (for free motion) [out]
/// \param omegadd (input):         angular acceleration of the beam (for free motion) [out]
/// \param dt (input):              time step
/// \param alpha (input) :  coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
/// \param alphaB (input) : coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
/// \param tridiagonalSolverName (input) : 
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
                    const real alpha, const real alphaB,
                    const aString & tridiagonalSolverName )
{

  if( debug & 2 )
    printF("--BM-- BeamModel::computeAcceleration, t=%8.2e, dt=%8.2e\n",t,dt);
  
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & ua = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  RealArray & uc = ua[current];  // current displacement 

  RealArray rhs(numElem*2+2); // *wdh* 2014/06/19

  // Compute:   rhs = -B*v -K*u 
  computeInternalForce(u, v, rhs);


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
    const real & Kxxt = dbase.get<real>("Kxxt");

    RealArray g;
    getBoundaryValues( t, g );

    real accelerationScaleFactor=1.;
    if( tridiagonalSolverName=="rhsSolver" )
    {
      // when we compute the RHS directly we are solving for rho*hs*b utt (not utt )
      accelerationScaleFactor=density*thickness*breadth;
    }
    
    for( int side=0; side<=1; side++ )
    {
      BoundaryCondition bc = side==0 ? bcLeft : bcRight;
      const int ia = side==0 ? 0 : numElem*2;
      const int ib = side==0 ? ia+2 : ia-2;
      const int is = 1-2*side;
      
      // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
      if( bc==clamped && EI==0. ) bc=pinned;

      // real x = side==0 ? 0 : L;
      if( bc == clamped ) 
      {
        // First two equations in the matrix are
        //       w_tt = given
        //       wx_tt = given 
	if( false )
	{
	  printF("--BM-- side=%i set clamped BC gtt=%e, gttx=%e, accelerationScaleFactor=%8.2e\n",
		 gtt(0,side),gtt(1,side),accelerationScaleFactor);
	}
	
	rhs(ia  )=gtt(0,side)*accelerationScaleFactor;   // w_tt is given
	rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 
      }
      else if( bc==pinned )
      {
	rhs(ia)=gtt(0,side)*accelerationScaleFactor;   // w_tt is given

	if( bc == pinned && EI != 0.) 
	{
	  // Boundary term is of the form:  -EI* v_x*w_xx
	  // -- correct for natural BC:  E*I*w_xx = +/- g(2,side)
	  if( debug & 1 )	printF("-- BM -- set rhs for pinned BC wxx = g(2,side)=%8.2e, EI=%g\n",g(2,side),EI);
	  rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)
	}
      }
      else if( bc==slideBC )
      {
        // ---- slide BC ---
        // Equation 2 in matrix:    wx_tt = given 
        rhs(ia+1)=gtt(1,side)*accelerationScaleFactor;   // wxtt is given 

        // Equation 1 is adjusted:
        rhs(ia  ) +=  (is)*EI*g(3,side);                 // add : -E*I*wxxx(0,t) * N(0)
        rhs(ia  ) += -(is)*T *g(1,side);                 // add :  T wx(0,t)*N(0) 
        rhs(ia  ) += -(is)*Kxxt*g(2,side);               // add :  Kxxt*wxt(0,t)
      }
      else if( bc==freeBC )
      {
	// Boundary terms are of the form:  T*v*w_x  -EI* v*w_xxx - EI* v_x*w_xx
	// Free BC: wxx=EI* g(2,side), w_xxx= EI*g(3,side)

	// printF("-- BM -- set rhs for free BC, g(2)=%e, g(3)=%e, T*u(ia+1)=%8.2e \n",g(2,side),g(3,side),T*u(ia+1));

	rhs(ia  ) +=  (is)*EI*g(3,side);   // add : -E*I*wxxx(0,t) * N(0)
	rhs(ia+1) += -(is)*EI*g(2,side);   // add : -E*I*wxx(0,t) * Np_x(0)

	// The boundary term T*v*w_x is only non-zero for v=N_1, and w_x = Np_1_x
        // ***CHECK ME*** IS THIS RIGHT?
	rhs(ia  ) += -(is)*T*u(ia+1);         // add : T*N1(0)*Np_1_x(0)*w'_1
	rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*wxt_1

      }
      else if( bc==internalForceBC )
      { // BC used when computing the "internal force"  F = L(u,v) + f , given (u,v)
	rhs(ia  ) += -(is)*T*u(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1
	rhs(ia  ) += -(is)*Kxxt*v(ia+1);      // add : T*N1(0)*Np_1_x(0)*w'_1

        // evaluate wxx and wxxx on the boundary
	const real beamLength=L;
	const real dx = beamLength/numElem;  
	real wxxx, wxx;
	if( FALSE )  
	{
	  const real dxidx = 2./dx;  // d(xi)/dx
	  // find the 2nd and third derivatives of the basis functions at the ends
	  real phijxx = -1.5*dxidx*dxidx, phijxxx=1.5*dxidx*dxidx*dxidx;
	  real psijxx = -dx*dxidx*dxidx,  psijxxx=.75*dx*dxidx*dxidx*dxidx;
	
	  // Here is wxx to 2nd-order and wxxx to first order accuracy
	  // This formula is the same as found from Taylor series
	  wxx  = (u(ia)- u(ib))*phijxx  + (is)*( u(ia+1) +.5*u(ib+1) )*psijxx;
	  wxxx = (is)*(u(ia)- u(ib) )*phijxxx + (u(ia+1)+u(ib+1))*psijxxx;

	  if( false )
	  {
	    printF(" side=%i: phijxx=%9.3e, phijxxx=%9.3e  dx=%8.2e, 1/dx=%8.2e\n",side,phijxx,phijxxx,dx,1/dx);
	    printF(" side=%i: psijxx=%9.3e, psijxxx=%9.3e\n",side,psijxx,psijxxx);
	    printF(" side=%i: (u,u')(ia)=(%e,%e) (u,u')(ib)=%e,%e)\n",u(ia),u(ia+1),u(ib),u(ib+1));
	    printF(" side=%i: wxx=%9.3e, wxxx=%9.3e\n",side,wxx,wxxx);
	  }
	}
	else
	{
          // **NEW** 
	  // From cgDoc/moving/codes/beam/interp.maple
	  // 4-order in upp, 2nd-order in uppp: 
	  const int ic = ib+2*is; // 2nd point inside
	  real h = is*dx, h2=h*h, h3=h2*h;
	  real u0=u(ia),     u1=u(ib),    u2=u(ic);
	  real up0=u(ia+1), up1=u(ib+1), up2=u(ic+1);
	  // wxx =-1/50.*(244*h*up0+176*h*up1-6*h*up2+407*u0-400*u1-7*u2)/h2;
	  // wxxx=3/50.*(189*h*up0+256*h*up1-11*h*up2+417*u0-400*u1-17*u2)/h3;
	  wxx =-1/2.*(12*h*up0+16*h*up1+2*h*up2+23*u0-16*u1-7*u2)/h2;
	  wxxx=3/2.*(13*h*up0+32*h*up1+5*h*up2+33*u0-16*u1-17*u2)/h3;
	
	  if( FALSE ) 
	  {
	    // *** THIS DOES NOT WORK: WHY???
	    printF("--BM-- internalForceBC:  side=%i: OLD: wxx=%9.2e, wxxx=%9.2e",side,wxx,wxxx);	
            // these formulas assume u=ux=0 and uxxxx=uxxxxx=0 
            // wxx = -1./2.*(7.*u0-8.*u1+u2)/h2;
	    // wxxx = 3./2.*(3.*u0-4.*u1+u2)/h3;

	    wxx = -1./194*(704*h*up1+34*h*up2+1491*u0-1344*u1-147*u2)/h2;
	    wxxx = 3./194*(832*h*up1+49*h*up2+1233*u0-1024*u1-209*u2)/h3;
	    

	    printF(", new: wxx=%9.2e, wxxx=%9.2e (u0=%8.2e, up0=%8.2e)\n",wxx,wxxx,u0,up0);	

	    // wxx=0.; wxxx=0.;
	  }
	}
	

	rhs(ia  ) +=  (is)*EI*wxxx;   // add : -E*I*wxxx(0,t) * N(0)
	rhs(ia+1) += -(is)*EI*wxx;    // add : -E*I*wxx(0,t) * Np_x(0)
	
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
  solveBlockTridiagonal(A, rhs, a, alpha, alphaB,tridiagonalSolverName );

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
/// /param adjustEnds (input) : if true then adjust the ends of the beam surface for 
///     clamped/pinned end conditions so that the end points on the beam surface do not move --
///     This is needed if we are generating a fluid grid exterior to the beam.
///
// ====================================================================================
void BeamModel::
getSurface( const real t, const RealArray & x0,  const RealArray & xs, 
	    const Index & Ib1, const Index & Ib2,  const Index & Ib3,
            const bool adjustEnds /* = false */ )
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

  bool clipToBounds=false; // To handle rounded ends that lie outside [0,L] do not clip points to beam length [0,L]
  
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    projectDisplacement(t, uc, x0(i1,i2,i3,0),x0(i1,i2,i3,1),xs(i1,i2,i3,0),xs(i1,i2,i3,1),clipToBounds);
  }

  if( adjustEnds )
  {

    // **FIX ME if beam has overlapping grids on a single side ***
    int boundaryCondition[2] = { bcLeft, bcRight}; // 
    for( int side=0; side<=1; side++ )
    {
      const int bc = boundaryCondition[side];
      const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
      const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
      const int i3= side==0 ? Ib3.getBase() : Ib3.getBound();
      
      if( bc == pinned || bc == clamped )
      {
	// -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
	xs(i1,i2,i3,0)=x0(i1,i2,i3,0); // set ends equal to initial position
	xs(i1,i2,i3,1)=x0(i1,i2,i3,1);
      }
      else if( bc==slideBC )
      {
        // t.x = t.x0  on ends
        real tDotX = (initialBeamTangent[0]*(xs(i1,i2,i3,0)-x0(i1,i2,i3,0)) +
		      initialBeamTangent[1]*(xs(i1,i2,i3,1)-x0(i1,i2,i3,1)) );
                       
        xs(i1,i2,i3,0) -= tDotX*initialBeamTangent[0];
        xs(i1,i2,i3,1) -= tDotX*initialBeamTangent[1];
	
      }
      
    }

    // *OLD* 2015/06/18
    // // **FIX ME if beam has overlapping grids on a single side ***
    // if( bcLeft == pinned || bcLeft == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBase(), i2=Ib2.getBase(), i3=Ib3.getBase();
    //   xs(i1,i2,i3,0)=x0(i1,i2,i3,0); // set ends equal to initial position
    //   xs(i1,i2,i3,1)=x0(i1,i2,i3,1);
    // }
    // if( bcRight == pinned || bcRight == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBound(), i2=Ib2.getBound(), i3=Ib3.getBound();
    //   xs(i1,i2,i3,0)=x0(i1,i2,i3,0);
    //   xs(i1,i2,i3,1)=x0(i1,i2,i3,1);
    // }
    
  }
  

}



// ====================================================================================
/// \brief Determine points on the beam surface
/// Return the displacement of the point on the surface (not the neutral axis)
/// of the beam of the point whose undeformed location is (x0,y0).
/// This function is used to update the boundary of the CFD grid.
/// \param X:       current beam solution vector
/// \param x0:      undeformed location of the point on the surface of the beam (x)
/// \param y0:      undeformed location of the point on the surface of the beam (y)
/// \param wx [out]: deformed location of the point on the surface of the beam (x)
/// \param wy [out]: deformed location of the point on the surface of the beam (y)
/// \param clipToBounds (input) : if true, clip points to the beam length [0,L]
// ====================================================================================
void BeamModel::
projectDisplacement(const real t, const RealArray& X, const real& x0, const real& y0, real& wx, real& wy,
                    bool clipToBounds /* =true */ ) 
{

  int elemNum;
  real eta, halfThickness;
  
  // Compute the half-thickness at this point (needed for rounded ends)
  projectPoint(x0,y0, elemNum, eta,halfThickness, clipToBounds);

  real eta0=eta;    // may be outside [-1,1] if clipToBounds=false
  if( !clipToBounds )
    eta=max(-1.,min(1.,eta));  // evaluate the displacement and slope on clipped bounds 
  
  real displacement, slope;
  interpolateSolution(X, elemNum, eta, displacement, slope); // compute the displacement and slope
  
  if( !clipToBounds && eta!=eta0 )
  { // If point is off the end of the beam then adjust the displacement on the beam ends by the constant slope at the end
    // Note: evaluating the solution at points outside [0,L] did not work well
    const real dx = L/numElem;  
    displacement += slope*.5*(eta0-eta)*dx;
  }

  real omag = 1./sqrt(slope*slope+1.0);
  real normall[2] = {-slope*omag, omag};  // normal to beam center-line

  if (!allowsFreeMotion) 
  {
    // Compute position along beam: 
    //    dxt = (x0,y0)*tangent
    //    dyt = (x0,y0)*normal
    real dxt = (x0-beamX0)*initialBeamTangent[0] + (y0-beamY0)*initialBeamTangent[1];
    real dyt = (x0-beamX0)* initialBeamNormal[0] + (y0-beamY0)* initialBeamNormal[1];

    real xl = dxt+normall[0]*halfThickness;
    real yl =     normall[1]*halfThickness + displacement;
    
    // *wdh* 2015/02/28 wx = beamX0 + initialBeamTangent[0]*xl-initialBeamTangent[1]*yl;
    // *wdh* 2015/02/28  wy = beamY0 -  initialBeamNormal[0]*xl +  initialBeamNormal[1]*yl;
    wx = beamX0 + initialBeamTangent[0]*xl + initialBeamNormal[0]*yl;
    wy = beamY0 + initialBeamTangent[1]*xl + initialBeamNormal[1]*yl;

    // if( eta0>1.001 )
    // {
    //   printF(" ---BM-- projDispl: (x0,y0)=(%9.2e,%9.2e) eta=%5.2f eta0=%5.2f slope=%5.2f u=%6.3f nb=[%5.2f,%5.2f] (wx,wy)=(%6.3f,%6.3f) h/2=%9.3e\n",
    // 	     x0,y0,eta,eta0,slope,displacement, normall[0],normall[1], wx,wy,halfThickness);
    // }
    

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
    
  if( debug & 4 && exactSolutionOption=="travelingWaveFSI" )
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
    
    // convert vector to rotated beam:
    ax = initialBeamTangent[0]*axl + initialBeamNormal[0]*ayl;
    ay = initialBeamTangent[1]*axl + initialBeamNormal[1]*ayl;
    // *wdh* 2015/02/28 ax = initialBeamTangent[0]*axl-initialBeamTangent[1]*ayl;
    // *wdh* 2015/02/2 ay =  initialBeamNormal[0]*axl +  initialBeamNormal[1]*ayl;
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
  if(( useExact || debug & 4 ) && exactSolutionOption=="travelingWaveFSI" )
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
    real ayl = normaldd[1]*thicknessFactor + DDdisplacement;
    
    // rotate vector along beam axis 
    ax = initialBeamTangent[0]*axl + initialBeamNormal[0]*ayl;
    ay = initialBeamTangent[1]*axl + initialBeamNormal[1]*ayl;
    // *wdh* 2015/02/28 ax = initialBeamTangent[0]*axl-initialBeamTangent[1]*ayl;
    // *wdh* 2015/02/28ay =  initialBeamNormal[0]*axl +  initialBeamNormal[1]*ayl;
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
/// \param normal (input) : normal to the surface
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
/// /param adjustEnds (input) : if true then adjust the ends of the beam velocity for 
///     clamped/pinned end conditions so that the end points on the beam surface do not move --
///     This is needed if we are generating a fluid grid exterior to the beam.
///
// ====================================================================================
void BeamModel::
getSurfaceAcceleration( const real t, const RealArray & x0, RealArray & as, const RealArray & normal, 
			const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                        const bool adjustEnds /* = false */ )
{
  if( true )
  {
    // *wdh* 2015/01/05 *new* way
    const bool addExternalForcing=true;
    getSurfaceInternalForce(t, x0, as, normal, Ib1,Ib2,Ib3,addExternalForcing );

    const real Abar =density*thickness*breadth;
    as *= 1./Abar;
    
  }
  else
  {
    int i1,i2,i3;
    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
    {
      projectAcceleration(t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),as(i1,i2,i3,0),as(i1,i2,i3,1) );
    }

  }

  if( adjustEnds )
  {
    // **FIX ME if beam has overlapping grids on a single side ***
    int boundaryCondition[2] = { bcLeft, bcRight}; // 
    for( int side=0; side<=1; side++ )
    {
      const int bc = boundaryCondition[side];
      const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
      const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
      const int i3= side==0 ? Ib3.getBase() : Ib3.getBound();
      
      if( bc == pinned || bc == clamped )
      {
	// -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
        as(i1,i2,i3,0)=0.;
        as(i1,i2,i3,1)=0.;
      }
      else if( bc==slideBC )
      {
        // t.a = t.a  on ends
        real tDotA = (initialBeamTangent[0]*as(i1,i2,i3,0)+
		      initialBeamTangent[1]*as(i1,i2,i3,1));
        as(i1,i2,i3,0) -= tDotA*initialBeamTangent[0];
        as(i1,i2,i3,1) -= tDotA*initialBeamTangent[1];
      }
    }

    // if( bcLeft == pinned || bcLeft == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBase(), i2=Ib2.getBase(), i3=Ib3.getBase();
    //   as(i1,i2,i3,0)=0.;
    //   as(i1,i2,i3,1)=0.;
    // }
    // if( bcRight == pinned || bcRight == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBound(), i2=Ib2.getBound(), i3=Ib3.getBound();
    //   as(i1,i2,i3,0)=0.;
    //   as(i1,i2,i3,1)=0.;
    // }
    
  }

}


// ====================================================================================
/// \brief Get the velocity of points on the beam surface
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param vs (output) : current velocity of the beam boundary 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
/// /param adjustEnds (input) : if true then adjust the ends of the beam velocity for 
///     clamped/pinned end conditions so that the end points on the beam surface do not move --
///     This is needed if we are generating a fluid grid exterior to the beam.
///
// ====================================================================================
void BeamModel::
getSurfaceVelocity( const real t, const RealArray & x0,  const RealArray & vs, 
		    const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                    const bool adjustEnds /* = false */ )
{
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    projectVelocity(t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),vs(i1,i2,i3,0),vs(i1,i2,i3,1) );
  }

  if( adjustEnds )
  {
    // **FIX ME if beam has overlapping grids on a single side ***
    int boundaryCondition[2] = { bcLeft, bcRight}; // 
    for( int side=0; side<=1; side++ )
    {
      const int bc = boundaryCondition[side];
      const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
      const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
      const int i3= side==0 ? Ib3.getBase() : Ib3.getBound();
      
      if( bc == pinned || bc == clamped )
      {
	// -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
	vs(i1,i2,i3,0)=0.;
	vs(i1,i2,i3,1)=0.;
      }
      else if( bc==slideBC )
      {
        // t.v = 0 on ends
        real tDotV = initialBeamTangent[0]*vs(i1,i2,i3,0)+initialBeamTangent[1]*vs(i1,i2,i3,1);
        vs(i1,i2,i3,0) -= tDotV*initialBeamTangent[0];
        vs(i1,i2,i3,1) -= tDotV*initialBeamTangent[1];
	
      }
      
    }
    
    // *OLD* 2015/06/18
    // if( bcLeft == pinned || bcLeft == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBase(), i2=Ib2.getBase(), i3=Ib3.getBase();
    //   vs(i1,i2,i3,0)=0.;
    //   vs(i1,i2,i3,1)=0.;
    // }
    // if( bcRight == pinned || bcRight == clamped ) 
    // {
    //   // -- force the end points of the beam surface to remain fixed -- **COULD DO BETTER**
    //   int i1=Ib1.getBound(), i2=Ib2.getBound(), i3=Ib3.getBound();
    //   vs(i1,i2,i3,0)=0.;
    //   vs(i1,i2,i3,1)=0.;
    // }
    
  }
}


// ====================================================================================
/// \brief Get the 'internal force' on the beam surface (used by added mass algorithms)
///
/// \param x0 (input) :  location of surface points on the undeformed beam 
/// \param fs (output) : current internal-force of the beam boundary 
/// \param normal (input) : normal to the surface
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
/// /param addExternalForcing (input) : if true add the external forcing to the RHS
///
// ====================================================================================
void BeamModel::
getSurfaceInternalForce( const real t0, const RealArray & x0, RealArray & fs, 
                         const RealArray & normal, 
			 const Index & Ib1, const Index & Ib2,  const Index & Ib3,
                         const bool addExternalForcing )
{
  
  int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  // When initializing the solution we may evaluate at t0<0 
  // In this case evaluate the internal force at t=0. 
  // Is this right??? *wdh* 2015/06/08 -- 
  real t= t0;
  if( t<0. )
  {
    const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
    const int prev = ( current -1 + numberOfTimeLevels) % numberOfTimeLevels;
    printF("--BM-- BeamModel::getSurfaceInternalForce:WARNING: t=%10.3e < 0. : evaluate at t=0.\n"
           " time(current)=%10.3e, time(prev)=%10.3e\n",t,time(current),time(prev));
    t=0.;
  }
  



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
  // The internal force is L(u) + fe     
  // 
  // FEM approximation:
  //       ms Ms a = K u + Fe 
  //  where M= ms*Ms is the mass matrix and Ms is the mass matrix without the factor of ms.
  // 
  // To solve for the internal force we solve
  //
  //     L(u) =  ms*a = Ms^{-1} ( K u + Fe )
  //  

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

  if( addExternalForcing ) 
  {
    fe=fc;  // *wdh* 2015/01/04  -- include external force
  }
  else
  {
    fe=0.;  // set external force to zero
  }
    
  if( false )
    ::display(fe,sPrintF("--BM-- getSurfaceInternalForce: external force fe at t=%8.2e",t),"%8.2e ");  

  // We need to refactor if the addExternalForcing optin has changed (as this changes the BC's)
  bool & refactor = dbase.get<bool>("refactor"); 
  if( addExternalForcing != dbase.get<bool>("rhsSolverAddExternalForcing") )
  {
    refactor=true;          
  }
  dbase.get<bool>("rhsSolverAddExternalForcing") =addExternalForcing; // save current option
  
 // ---- Boundary Conditions ----
  BoundaryCondition bcLeftSave =bcLeft;  // save current 
  BoundaryCondition bcRightSave=bcRight;

  // NOTE: When the forcing is included we can use the regular BC's for u
  //       When the forcing is not included we CANNOT use the regular BC's for u
  if( false   
       ||  !addExternalForcing   // ***TEST*** 2015/02/21
     )
  {
    // bcLeft=unknownBC;
    // bcRight=unknownBC;

    // -- BC's used when computing the "internal force"  F = L(u,v) + f , given (u,v) 
    bcLeft=internalForceBC;
    bcRight=internalForceBC;
  }
  
  const real alpha =0.;  // coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
  const real alphaB=0.;  // coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
  const real dt=0.;
  computeAcceleration( t, xc,vc,fe, Me, internalForce, centerOfMassAcceleration, angularAcceleration, dt,
		       alpha,alphaB,"rhsSolver" );

  refactor=false;
  
  if( false )
    ::display(internalForce,sPrintF("--BM-- getSurfaceInternalForce: internalForce at t=%8.2e",t),"%8.2e ");  



  // refactor=true;          

  bcLeft =bcLeftSave;   // reset 
  bcRight=bcRightSave; 

  if( FALSE && !addExternalForcing ) 
  {
    if( t<100*dt)
      printF("--BM-- Smooth the internalForce at t=%8.2e\n",t); 
    bool & smoothSolution = dbase.get<bool>("smoothSolution");
    bool smoothSolutionSave = smoothSolution;
    smoothSolution=true;
    int & numberOfSmooths= dbase.get<int>("numberOfSmooths");
    int numberOfSmoothsSave=numberOfSmooths;
    numberOfSmooths=5;
    
    smooth( t,internalForce, "Smooth the internalForce" );
    smoothSolution=smoothSolutionSave;
    numberOfSmooths=numberOfSmoothsSave;
  }

  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    // NOTE: this function will add the rotation term due to finite thickness
    projectInternalForce( internalForce, t, x0(i1,i2,i3,0),x0(i1,i2,i3,1),fs(i1,i2,i3,0),fs(i1,i2,i3,1) );
  }

  if( false )
    ::display(fs,"--BM-- getSurfaceInternalForce : fs after projectInternalForce","%8.2e ");


  // ::display(fs,"--BM-- internalForce from projectInternalForce","%8.2e ");
    
  bool useExact=false;
  if( useExact && exactSolutionOption=="travelingWaveFSI" )
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
  

  // -- Adjust the surface force to match the surface normals ---
  // -- THE AMP scheme wants a beam force Fs that satisfies
  //      nv.Fs = +/- nbv.fs
  //      tv.Fs = +/- tbv.fs  
  //  where nv = fluid normal, tv = fluid tangent
  //  where nbv, tv = beam normal and tangent (undeformed beam)
  //
  //   Normal component of force = fs(Ib1,Ib2,Ib3,1)
  //   Tangential component of force = fs(Ib1,Ib2,Ib3,0)
  //   Tangent = ( n1, -n0 )
  // 
  //     F = fs(0)*tangent + fs(1)*normal 
  // 
  const bool & useSmallDeformationApproximation = dbase.get<bool>("useSmallDeformationApproximation");
  if( useSmallDeformationApproximation )
  {
    RealArray normalBeamForce(Ib1,Ib2,Ib3), tangentialBeamForce(Ib1,Ib2,Ib3);
    // beam reference normal and tangential components of force:
    normalBeamForce     = fs(Ib1,Ib2,Ib3,0)*initialBeamNormal[0]  + fs(Ib1,Ib2,Ib3,1)*initialBeamNormal[1];
    tangentialBeamForce = fs(Ib1,Ib2,Ib3,0)*initialBeamTangent[0] + fs(Ib1,Ib2,Ib3,1)*initialBeamTangent[1];
    
    fs(Ib1,Ib2,Ib3,0) =  tangentialBeamForce*normal(Ib1,Ib2,Ib3,1) + normalBeamForce*normal(Ib1,Ib2,Ib3,0);
    // fluid tangent = (-nv0, nv1)
    fs(Ib1,Ib2,Ib3,1) = -tangentialBeamForce*normal(Ib1,Ib2,Ib3,0) + normalBeamForce*normal(Ib1,Ib2,Ib3,1);

    // Now adjust the sign depending on whether the beam has fluid on the top or bottom:
    const bool fluidOnTwoSides =dbase.get<bool>("fluidOnTwoSides");
    if( true || fluidOnTwoSides ) // I think we always need to do this 
    {
      RealArray & normalSign = normalBeamForce;// reuse
      normalSign = ( (x0(Ib1,Ib2,Ib3,0)- beamX0)*initialBeamNormal[0] + 
		     (x0(Ib1,Ib2,Ib3,1)- beamY0)*initialBeamNormal[1] );
      where( normalSign > 0. )
      {
	fs(Ib1,Ib2,Ib3,0)=-fs(Ib1,Ib2,Ib3,0);
	fs(Ib1,Ib2,Ib3,1)=-fs(Ib1,Ib2,Ib3,1);
      }
    }
  
  }
  
  
  // FINISH ME -- add on rotation term 

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
  real eta, halfThickness;
  
  projectPoint(x0,y0, elemNum, eta,halfThickness);  // halfThickness will be computed here

  //std::cout << x0 << " " << y0 << " " << elemNum << " " << eta << " " << halfThickness << std::endl;
  
  real displacement, slope;
  interpolateSolution(uc, elemNum, eta, displacement, slope);       // displacement=u, slope = u_x 

  real Ddisplacement, Dslope;
  interpolateSolution(vc, elemNum, eta, Ddisplacement, Dslope);     //  Ddisplacement = v, Dslope=v_x 

  if( debug & 2 )
    printF(" -- BM -- projectVelocity: x=(%g,%g) Ddisplacement=%g (beam velocity)\n",x0,y0,Ddisplacement);
  
  
  // A point on the beam surface is equal to the point on the neutral surface plus an offset in the normal direction
  //       p  = x(eta) + (0,w) + nv(eta)*halfThickness

  // The velocity of the point is 
  //       vp = (0,w_t) + d(nv)/(dt)*halfThickness

  real omag = 1./sqrt(slope*slope+1.0);
  real omag3 = omag*omag*omag;
  // real omag5 = omag*omag*omag3;
  // real normall[2] = {-slope*omag, omag};
  real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};

  if( !allowsFreeMotion ) 
  {
    real vxl =                 normald[0]*halfThickness;
    real vyl = Ddisplacement + normald[1]*halfThickness;   // v = w_t + (ny)_t * thick
    
    // convert vector to rotated beam 
    vx = initialBeamTangent[0]*vxl + initialBeamNormal[0]*vyl;
    vy = initialBeamTangent[1]*vxl + initialBeamNormal[1]*vyl;
    // *wdh* 2015/02/28 vx = initialBeamTangent[0]*vxl - initialBeamTangent[1]*vyl;
    // *wdh* 2015/02/28 vy = initialBeamNormal[0] *vxl + initialBeamNormal[1] *vyl;
  }
  else 
  {
    // --- free motion ---

    OV_ABORT("finish me");
    
  }

  if( debug & 4  && exactSolutionOption=="travelingWaveFSI" )
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

  const real & signForNormal = dbase.get<real>("signForNormal");  // flip sign of normal using this parameter
  initialBeamNormal[0] = -sin(dec) *signForNormal;
  initialBeamNormal[1] =  cos(dec) *signForNormal;

  initialBeamTangent[0] = cos(dec);
  initialBeamTangent[1] = sin(dec);


  for (int k = 0; k < 2; ++k) {

    normal[k] = initialBeamNormal[k];
    tangent[k] = initialBeamTangent[k];
  }
}


// =================================================================================
/// \brief Return the element, half-thickness, and natural coordinate for
/// a point (x0,y0) on the undeformed SURFACE of the beam
/// 
/// \param x0:        undeformed location of the point on the surface of the beam (x)
/// \param y0:        undeformed location of the point on the surface of the beam (y)
/// \param elemNum:   element corresponding to this point (closest node <= x0) [out]
/// \param eta:       natural (element) coordinate corresponding to this point: 
///                  eta is in [-1,1] on element elemNum [out]
/// \param  halfThickness: half-thickness of the beam at this point (i.e. approximate
///                normal distance from centerline to surface) [out]
/// \param clipToBounds (input) : if true, clip points to the beam length [0,L]
//
/// \note: Points off the end of the beam are pointed onto the end if clipToBounds=true .
// =================================================================================
void BeamModel::
projectPoint(const real& x0,const real& y0,
	     int& elemNum, real& eta, real& halfThickness,
             bool clipToBounds /* =true */) 
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

  // Compute position along beam: 
  //    xl = (x0,y0)*tangent
  //    yl = (x0,y0)*normal
  real xl = xll*initialBeamTangent[0] + yll*initialBeamTangent[1];
  real yl = xll* initialBeamNormal[0] + yll*initialBeamNormal[1];

  //std::cout << "(" << x0 << ", " << y0 << ") " << xl << "--" << yl << " " << le << std::endl;

  // elemNum : closest node less than point xl:
  elemNum = min(numElem-1,max(0, (int)(xl / le)));  // closest active node 
  eta = 2.0*(xl-le*elemNum)/le-1.0;

  // Project points off the end back onto the end-point:
  if( clipToBounds )
  {
    if (eta < -1.0)
      eta = -1.0;
    else if (eta > 1.0)
      eta = 1.0;
  }
  else
  {
    // if( fabs(eta)>1. )
    //   printF("--BM-- INFO: projectPoint (x0,y0)=(%g,%g) : elemNum=%i eta=%g\n",x0,y0,elemNum,eta);
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


  // *new way* 2015/01/13
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];  // force at current time

  RealArray fDotN(Ib1,Ib2,Ib3);

  // --- transfer the normal component of the force -- here we use the current fluid normal
  // NOTE: we could alternatively have used:
  //              (1) current beam normal
  //              (2) initial beam normal
  fDotN(Ib1,Ib2,Ib3)= (traction(Ib1,Ib2,Ib3,0)*normal(Ib1,Ib2,Ib3,0)+
		       traction(Ib1,Ib2,Ib3,1)*normal(Ib1,Ib2,Ib3,1) );
  if( false )
  {
    ::display(traction(Ib1,Ib2,Ib3,Range(0,1)),sPrintF("--BM-- addForce: input traction t=%9.3e",tf),"%9.2e ");
    ::display(fDotN,sPrintF("--BM-- addForce: input fDotN t=%9.3e",tf),"%9.2e ");
  }
  

  bool addToForce=true;
  addToElementIntegral( tf,x0,fDotN,normal,Ib1,Ib2,Ib3,fc, addToForce );

  if( false )
  {
    ::display(fc,sPrintF("--BM-- addForce : fc after addToElementIntegral, t=%9.3e\n",t),"%9.2e ");
  }
  

  return;


  // // ** old way**

  // // Jb1, Jb2, Jb3 : for looping over cells instead of grid points -- decrease by 1 along active axis
  // Index Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
  // Index Jg1=Ib1, Jg2=Ib2, Jg3=Ib3;
  // int ia, ib; // index for boundary points
  // int iga, igb; // index for ghost points
  // int axis=-1, is1=0, is2=0, is3=0;
  // if( Jb1.getLength()>1 )
  // { // grid points on boundary are along axis=0
  //   axis=0; is1=1;
  //   ia=Ib1.getBase(); ib=Ib1.getBound();
  //   Jb1=Range(Jb1.getBase(),Jb1.getBound()-1); // decrease length by 1
  //   assert( Jb2.getLength()==1 );
  //   Jg1=Range(Jg1.getBase()-1,Jg1.getBound()+1); // add ghost
  //   iga = Jg1.getBase(); igb=Jg1.getBound();
  // }
  // else
  // { // grid points on boundary are along axis=1
  //   axis=1; is2=1;
  //   ia=Ib2.getBase(); ib=Ib2.getBound();
  //   Jb2=Range(Jb2.getBase(),Jb2.getBound()-1); // decrease length by 1
  //   assert( Jb1.getLength()==1 );
  //   Jg2=Range(Jg2.getBase()-1,Jg2.getBound()+1); // add ghost
  //   iga = Jg2.getBase(); igb=Jg2.getBound();
  // }
  // assert( axis>=0 );
  
  
  // const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  // const int orderOfAccuracyForDerivative=orderOfGalerkinProjection;

  // RealArray fDotN(Jg1,Jg2,Jg3); // add ghost

  // int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  // FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  // {
  //   fDotN(iv[axis])= (traction(i1,i2,i3,0)*normal(i1,i2,i3,0)+
  // 	              traction(i1,i2,i3,1)*normal(i1,i2,i3,1) );
  // }
  // // extrapolate ghost
  // if( orderOfAccuracyForDerivative==4 && igb > iga+5 )
  // { // this is needed for fourth-order accuracy 
  //   fDotN(iga)=5.*fDotN(iga+1)-10.*fDotN(iga+2)+10.*fDotN(iga+3)-5.*fDotN(iga+4)+fDotN(iga+5);
  //   fDotN(igb)=5.*fDotN(igb-1)-10.*fDotN(igb-2)+10.*fDotN(igb-3)-5.*fDotN(igb-4)+fDotN(igb-5);
  // }
  // else if( orderOfAccuracyForDerivative==4 &&  igb > iga+4 )
  // {
  //   fDotN(iga)=4.*fDotN(iga+1)-6.*fDotN(iga+2)+4.*fDotN(iga+3)-fDotN(iga+4);
  //   fDotN(igb)=4.*fDotN(igb-1)-6.*fDotN(igb-2)+4.*fDotN(igb-3)-fDotN(igb-4);
  // }
  // else if( igb > iga+3 )
  // {
  //   fDotN(iga)=3.*fDotN(iga+1)-3.*fDotN(iga+2)+fDotN(iga+3);
  //   fDotN(igb)=3.*fDotN(igb-1)-3.*fDotN(igb-2)+fDotN(igb-3);
  // }
  // else
  // {
  //   assert( igb> iga+2 );
  //   fDotN(iga)=2.*fDotN(iga+1)-fDotN(iga+2);
  //   fDotN(igb)=2.*fDotN(igb-1)-fDotN(igb-2);
  // }
 
  //   // printF("--BM-- addForce : tf=%9.3e (p1,p2)=(%8.2e,%8.2e)\n",tf,p1,p2);
  // real beamLength=L;
  // const real dx = beamLength/numElem;  
  // FOR_3(i1,i2,i3,Jb1,Jb2,Jb3)
  // {
  //   int i1p=i1+is1, i2p=i2+is2, i3p=i3+is3;
  //   real f1x, f2x;
  //   if( orderOfAccuracyForDerivative==2 )
  //   {
  //     f1x = (fDotN(iv[axis]+1)-fDotN(iv[axis]-1))/(2.*dx);
  //     f2x = (fDotN(iv[axis]+2)-fDotN(iv[axis]  ))/(2.*dx);
  //   }
  //   else
  //   {
  //     int i=iv[axis];
  //     if( i-2 >= iga && i+2 <= igb )
  //       f1x = ( 8.*(fDotN(i+1)-fDotN(i-1)) - fDotN(i+2) +fDotN(i-2) )/(12.*dx);
  //     else if( i==ia && i+4 <=igb )
  //     { // fourth-order one-sided
  //       f1x = ( -(25./12.)*fDotN(i) + 4.*fDotN(i+1) -3.*fDotN(i+2) + (4./3.)*fDotN(i+3) -.25*fDotN(i+4) )/dx;
  //     }
  //     else
  // 	f1x = (fDotN(i+1)-fDotN(i-1))/(2.*dx);
      
  //     i=iv[axis]+1;
  //     if( i-2 >= iga && i+2 <= igb )
  //       f2x = ( 8.*(fDotN(i+1)-fDotN(i-1)) - fDotN(i+2) +fDotN(i-2) )/(12.*dx);
  //     else if( i==ib && i-4 >=iga )
  //     { // fourth-order one-sided
  //       f2x = -( -(25./12.)*fDotN(i) + 4.*fDotN(i-1) -3.*fDotN(i-2) + (4./3.)*fDotN(i-3) -.25*fDotN(i-4) )/dx;
  //     }
  //     else
  // 	f2x = (fDotN(i+1)-fDotN(i-1))/(2.*dx);
  //   }
    
  //   printF("--BM-- addForce: x0=[%8.2e,%8.2e] f1=%8.2e f1x=%8.2e, x0=[%8.2e,%8.2e] f2=%8.2e f2x=%8.2e\n",
  // 	   x0(i1,i2,i3,0),x0(i1,i2,i3,1),  fDotN(iv[axis]), f1x, 
  //          x0(i1p,i2p,i3p,0),x0(i1p,i2p,i3p,1), fDotN(iv[axis]+1), f2x );    

  //   addForce(tf,
  // 	     x0(i1,i2,i3,0), 
  // 	     x0(i1,i2,i3,1),  fDotN(iv[axis]), f1x,
  // 	     normal(i1,i2,i3,0), normal(i1,i2,i3,1),
  // 	     x0(i1p,i2p,i3p,0), 
  // 	     x0(i1p,i2p,i3p,1), fDotN(iv[axis]+1), f2x, 
  // 	     normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1));
  // }
  
  // if( false )
  // {
  //   const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  //   const int & current = dbase.get<int>("current"); 

  //   RealArray & time = dbase.get<RealArray>("time");
  //   std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  //   RealArray & fc = f[current];  // force at current time
  //   ::display(fc,sPrintF("--BM-- addForce:END: : fc at t=%9.3e",tf),"%8.2e ");
  // }
  

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
	  real p1, real p1x, const real& nx_1,const real& ny_1,
	  const real& x0_2, const real& y0_2,
	  real p2, real p2x, const real& nx_2,const real& ny_2)
{

  // new way *wdh* 2015/01/13
  real x1[2]={ x0_1,y0_1}, nv1[2]={nx_1,ny_1}; //
  real x2[2]={ x0_2,y0_2}, nv2[2]={nx_2,ny_2}; //

  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  const int & current = dbase.get<int>("current"); 
  RealArray & fc = f[current];  // force at current time
  bool addToForce=true;
  
  addToElementIntegral( tf,x1,p1,p1x,nv1, x2,p2,p2x,nv2,fc,addToForce );


  return;
    
  //  // * OLD WAY **

  //  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  //  const int & current = dbase.get<int>("current"); 

  //  RealArray & time = dbase.get<RealArray>("time");
  //  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  //  RealArray & fc = f[current];  // force at current time
  //  if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
  //  {
  //    printF("--BM-- BeamModel::addForce:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
  //        tf,time(current),current);
  //    OV_ABORT("ERROR");
  //  }

  //  int elem1,elem2;
  //  real eta1,eta2,t1,t2;

  //  real p11, p22, p11x, p22x;

  //  //std::cout << x0_1 << " " << p1 << std::endl;
  
  // // if (p1 != p1/* || p1 > 100.0*/) {
  // //   //std::cout << "Found nan!" << std::endl;
  // // }

  //  //std::cout << getExactPressure(t,x0_1) << " " << p1 << std::endl;
  //  //
  
  //  //p1 = getExactPressure(t,x0_1)*1000.0;
  //  //p2 = getExactPressure(t,x0_2)*1000.0;
  
  
  //  // (xll,yll) = point_1 - beam_0 
  //  real xll = x0_1-beamX0;
  //  real yll = y0_1-beamY0;

  //  // (xl,yl) = relative coordinates along (rotated) beam 
  //  real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  //  real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  //  // myx0 = relative beam coordinate in [0,L] of point_1
  //  real myx0 = xl;

  //  // (xll,yll) = point_2 - beam_0
  //  xll = x0_2-beamX0;
  //  yll = y0_2-beamY0;
  //  //  // (xl,yl) = relative coordinates along (rotated) beam 
  //  xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  //  yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  //  // myx1 = relative beam coordinate in [0,L] of point_2
  //  real myx1 = xl;

  //  // point_1 lies in elem1, offset eta1
  //  // point_2 lies in elem2, offset eta2
  //  if (myx1 > myx0) 
  //  {
  //    projectPoint(x0_1,y0_1,elem1, eta1,t1); 
  //    projectPoint(x0_2,y0_2,elem2, eta2,t2);   
  //    p11 = p1*pressureNorm;  p11x = p1x*pressureNorm; 
  //    p22 = p2*pressureNorm;  p22x = p2x*pressureNorm;
  //  } 
  //  else 
  //  {
  //    projectPoint(x0_1,y0_1,elem2, eta2,t2); 
  //    projectPoint(x0_2,y0_2,elem1, eta1,t1);  
  //    p22 = p1*pressureNorm; p22x = p1x*pressureNorm;
  //    p11 = p2*pressureNorm; p11x = p2x*pressureNorm; 
  //    std::swap<real>(myx0,myx1);
  //  }

  //  //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << p1 << " " << p2 << std::endl;

  //  const real dx12 = max(fabs(myx0-myx1),REAL_MIN*100.); // distance between point_1 and point_2

  //  const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  
  //  //                 elem1            elem2
  //  //        +----------+--X------+-----X--+-----
  //  //                      myx0        myx1
  //  RealArray lt(4);
  //  for (int i = elem1; i <= elem2; ++i) 
  //  {

  //    real a = eta1, b = eta2;
  //    real pa = p11, pax=p11x, pb = p22, pbx=p22x;
  //    real x0 = myx0, x1 = myx1;

  //    if (i != elem1) 
  //    {
  //      // We have moved to the next element from the first
  //      a = -1.0;  // xi value
  //      x0 = le*i; // x-value 
  //      if( orderOfGalerkinProjection==2 )
  //      {
  //        pa = p11 + (p22-p11)*(x0-myx0)/dx12;
  //      }
  //      else
  //      {
  //        // -- evaluate an Hermite interpolant fit to (myx0,p11)--(myx1,p22) --
  //        //         p11,p11x            p22,p22x
  //        //           X-------+-----------X
  //        //          myx0                myx1
  //        //           -1      xi          +1
  //        real xi = -1. + 2.*(x0-myx0)/dx12;
  //        // ** CHECK ME ***
  //        real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
  //        real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
  //        real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
  //        real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
  //        real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
  //        real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
  //        real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
  //        real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;
	
  //        pa = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
  //        pax = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 

  //      }
      
  //    }
  //    // -- right end is not on a node -- adjust pb,pbx --
  //    if (i != elem2) 
  //    {
  //      b = 1.0;
  //      x1 = le*(i+1);
  //      if( orderOfGalerkinProjection==2 )
  //      {
  //        pb = p11 + (p22-p11)*(x1-myx0)/dx12;
  //      }
  //      else
  //      {
  //        // -- evaluate the Hermite interpolant --
  //        real xi = -1. + 2.*(x1-myx0)/dx12;

  //        real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
  //        real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
  //        real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
  //        real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
  //        real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
  //        real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
  //        real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
  //        real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;

  //        pb = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
  //        pbx = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 
  //      }
  //    }
    
  //    // printF("--AF-- x0=%7.5f pa=%7.5f pax=%7.5f, x1=%7.5f pb=%7.5f pbx=%7.5f [a,b]=[%7.4f,%7.4f]\n",
  //    //       x0,pa,pax,x1,pb,pbx,a,b);
    
  //    Index idx(i*2,4);
  //    if (t1 > 0) 
  //    { // Flip sign of force to account for the normal 
  //      pa = -pa; pax = -pax;
  //      pb = -pb; pbx = -pbx;
  //    }
    
      
  //    //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
  //    //  p1 << " " << p2 << std::endl;
  

  //    if (fabs(b-a) > 1.0e-10)  // *WDH* FIX ME -- is this needed?
  //    {
  //      // -- compute (N,p)_[a,b] = int_a^b N(xi) p(xi) J dxi 
  //      if( orderOfGalerkinProjection==2 )
  //        computeProjectedForce(pa,pb, a,b, lt);
  //      else
  //        computeGalerkinProjection(pa,pax, pb,pbx,  a,b, lt);

  //      //    std::cout << "a = " << a << " b = " << b << std::endl;
  //      fc(idx) += lt;

  //      real gradp = 1.0;
  //      totalPressureForce += (lt(0)+lt(2));
  //      totalPressureMoment += (lt(0)*(le*i-0.5*L)+lt(1)*gradp+lt(2)*(le*(i+1)-0.5*L) + lt(3)*gradp);
  //    }
  //    //printArray(lt,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);

  //  }
}

// =================================================================================================
/// /brief Add to the element integral for a function f (defined on an adjacent fluid grid).
/// /param x0 (input) : array of (undeformed) locations on the beam surface
/// /param f(Ib1,Ib2,Ib3) (input) : function on the deformed surface
/// /param normal (input) : normal to the deformed surface 
/// /param Ib1,Ib2,Ib3 (input) : index of points on the boundary.
/// /param fe(2*numElem+2) (input/output) : holds element integrals 
// =================================================================================================
void BeamModel::
addToElementIntegral(const real & tf, const RealArray & x0, const RealArray & f, const RealArray & normal,  
		     const Index & Ib1, const Index & Ib2,  const Index & Ib3, RealArray & fe, 
		     bool addToForce /* = false */  )
{
  const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  const int orderOfAccuracyForDerivative=orderOfGalerkinProjection;

  // Jb1, Jb2, Jb3 : for looping over cells instead of grid points -- decrease by 1 along active axis

  Index Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;

  Index Jgv[3], &Jg1=Jgv[0], &Jg2=Jgv[1], &Jg3=Jgv[2];
  Jg1=Ib1, Jg2=Ib2, Jg3=Ib3;

  int ia, ib; // index for boundary points
  int iga, igb; // index for ghost points
  int axis=-1, is1=0, is2=0, is3=0;
  if( Jb1.getLength()>1 )
  { // grid points on boundary are along axis=0
    axis=0; is1=1;
    ia=Ib1.getBase(); ib=Ib1.getBound();
    Jb1=Range(Jb1.getBase(),Jb1.getBound()-1); // decrease length by 1
    assert( Jb2.getLength()==1 );
    Jg1=Range(Jg1.getBase()-1,Jg1.getBound()+1); // add ghost
    iga = Jg1.getBase(); igb=Jg1.getBound();
  }
  else
  { // grid points on boundary are along axis=1
    axis=1; is2=1;
    ia=Ib2.getBase(); ib=Ib2.getBound();
    Jb2=Range(Jb2.getBase(),Jb2.getBound()-1); // decrease length by 1
    assert( Jb1.getLength()==1 );
    Jg2=Range(Jg2.getBase()-1,Jg2.getBound()+1); // add ghost
    iga = Jg2.getBase(); igb=Jg2.getBound();
  }
  assert( axis>=0 );
  
  
   // add ghost points to force vector so we can take derivatives for Hermite approx.
  RealArray fg(Jgv[axis]);

  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    fg(iv[axis])= f(i1,i2,i3);
  }
  // extrapolate ghost
  if( orderOfAccuracyForDerivative==4 && igb > iga+5 )
  { // this is needed for fourth-order accuracy 
    fg(iga)=5.*fg(iga+1)-10.*fg(iga+2)+10.*fg(iga+3)-5.*fg(iga+4)+fg(iga+5);
    fg(igb)=5.*fg(igb-1)-10.*fg(igb-2)+10.*fg(igb-3)-5.*fg(igb-4)+fg(igb-5);
  }
  else if( orderOfAccuracyForDerivative==4 &&  igb > iga+4 )
  {
    fg(iga)=4.*fg(iga+1)-6.*fg(iga+2)+4.*fg(iga+3)-fg(iga+4);
    fg(igb)=4.*fg(igb-1)-6.*fg(igb-2)+4.*fg(igb-3)-fg(igb-4);
  }
  else if( igb > iga+3 )
  {
    fg(iga)=3.*fg(iga+1)-3.*fg(iga+2)+fg(iga+3);
    fg(igb)=3.*fg(igb-1)-3.*fg(igb-2)+fg(igb-3);
  }
  else
  {
    assert( igb> iga+2 );
    fg(iga)=2.*fg(iga+1)-fg(iga+2);
    fg(igb)=2.*fg(igb-1)-fg(igb-2);
  }
 
  if( false )
  {
    ::display(x0,"--BM-- addToElementIntegral: x0","%9.2e ");
    ::display(fg,"--BM-- addToElementIntegral: fg","%9.2e ");
  }
  

  // printF("--BM-- addToElementIntegral : tf=%9.3e (p1,p2)=(%8.2e,%8.2e)\n",tf,p1,p2);
  real beamLength=L;
  FOR_3(i1,i2,i3,Jb1,Jb2,Jb3)
  {
    const int i1p=i1+is1, i2p=i2+is2, i3p=i3+is3;
    const int ii =iv[axis];
    
    // grid spacing for function f
    // *wdh* 2015/03/01 const real dx = x0(ii+1)-x0(ii); 
    const real dx = sqrt( SQR(x0(i1p,i2p,i3p,0)-x0(i1,i2,i3,0)) +
                          SQR(x0(i1p,i2p,i3p,1)-x0(i1,i2,i3,1)) );

    real f1x, f2x;
    if( orderOfAccuracyForDerivative==2 )
    {
      f1x = (fg(iv[axis]+1)-fg(iv[axis]-1))/(2.*dx);
      f2x = (fg(iv[axis]+2)-fg(iv[axis]  ))/(2.*dx);
    }
    else
    {
      int i=iv[axis];
      if( i-2 >= iga && i+2 <= igb )
	f1x = ( 8.*(fg(i+1)-fg(i-1)) - fg(i+2) +fg(i-2) )/(12.*dx);
      else if( i==ia && i+4 <=igb )
      { // fourth-order one-sided
	f1x = ( -(25./12.)*fg(i) + 4.*fg(i+1) -3.*fg(i+2) + (4./3.)*fg(i+3) -.25*fg(i+4) )/dx;
      }
      else
	f1x = (fg(i+1)-fg(i-1))/(2.*dx);
      
      i=iv[axis]+1;
      if( i-2 >= iga && i+2 <= igb )
	f2x = ( 8.*(fg(i+1)-fg(i-1)) - fg(i+2) +fg(i-2) )/(12.*dx);
      else if( i==ib && i-4 >=iga )
      { // fourth-order one-sided
	f2x = -( -(25./12.)*fg(i) + 4.*fg(i-1) -3.*fg(i-2) + (4./3.)*fg(i-3) -.25*fg(i-4) )/dx;
      }
      else
	f2x = (fg(i+1)-fg(i-1))/(2.*dx);
    }
    
    if( false )
      printF("--BM-- addForce: x0=[%8.2e,%8.2e] f1=%8.2e f1x=%8.2e, x0=[%8.2e,%8.2e] f2=%8.2e f2x=%8.2e\n",
	     x0(i1,i2,i3,0),x0(i1,i2,i3,1),  fg(iv[axis]), f1x, 
	     x0(i1p,i2p,i3p,0),x0(i1p,i2p,i3p,1), fg(iv[axis]+1), f2x );    

    // new way
    real xv1[2]={ x0(i1,i2,i3,0),x0(i1,i2,i3,1)},       nv1[2]={normal(i1,i2,i3,0), normal(i1,i2,i3,1)}; //
    real xv2[2]={ x0(i1p,i2p,i3p,0),x0(i1p,i2p,i3p,1)}, nv2[2]={normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1)}; //

    addToElementIntegral( tf,xv1,fg(iv[axis]),f1x,nv1, xv2,fg(iv[axis]+1),f2x,nv2,fe,addToForce );

    // addForce(tf,
    // 	     x0(i1,i2,i3,0), 
    // 	     x0(i1,i2,i3,1),  fg(iv[axis]), f1x,
    // 	     normal(i1,i2,i3,0), normal(i1,i2,i3,1),
    // 	     x0(i1p,i2p,i3p,0), 
    // 	     x0(i1p,i2p,i3p,1), fg(iv[axis]+1), f2x, 
    // 	     normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1));
  }
  
  if( false )
  {
    ::display(fe,sPrintF("--BM-- addToElementIntegral:END: : fe at t=%9.3e",tf),"%8.2e ");
  }
  

}


// =======================================================================================================
/// \brief Add to the element integral for a function f.
///
///  Given values of a function (and optionally its derivative) at two points increment the element integrals
///             int phi_i(x) f(x) dx 
///             int psi_i(x) f(x) dx
/// 
///  /param tf (input) : current time
///  /param x1[] (input) : location of point 1 (undeformed)
///  /param f1,f1x  (input) : value of function and (optionally) the derivative at point 1.
///  /param nv1[] (input) : normal at point 1 (unused?)
///  /param fe(2*numElem+2) (input/output) : on input, the vector of current element integrals
///  /param addToForce (input) : if true, we are adding to the force on the beam.
// =========================================================================================================
void BeamModel::
addToElementIntegral( const real & tf,
		      const real *x1, const real f1, const real f1x, const real *nv1, 
		      const real *x2, const real f2, const real f2x, const real *nv2,
		      RealArray & fe, bool addToForce /* = false */ )
{

  real x0_1=x1[0], y0_1=x1[1], p1=f1, p1x=f1x;
  real x0_2=x2[0], y0_2=x2[1], p2=f2, p2x=f2x;

  const int & current = dbase.get<int>("current"); 
  RealArray & time = dbase.get<RealArray>("time");
  if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
  {
    printF("--BM-- BeamModel::addToElementIntegral:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	   tf,time(current),current);
    OV_ABORT("ERROR");
  }

  int elem1,elem2;
  real eta1,eta2,t1,t2;

  real p11, p22, p11x, p22x;

  // (xll,yll) = point_1 - beam_0 
  real xll = x0_1-beamX0;
  real yll = y0_1-beamY0;

  // (xl,yl) = relative coordinates along (rotated) beam 
  real xl = xll*initialBeamTangent[0] + yll*initialBeamTangent[1];
  real yl = xll* initialBeamNormal[0] + yll* initialBeamNormal[1];

  // myx0 = relative beam coordinate in [0,L] of point_1
  real myx0 = xl;

  // (xll,yll) = point_2 - beam_0
  xll = x0_2-beamX0;
  yll = y0_2-beamY0;
  //  // (xl,yl) = relative coordinates along (rotated) beam 
  xl = xll*initialBeamTangent[0] + yll*initialBeamTangent[1];
  yl = xll* initialBeamNormal[0] + yll* initialBeamNormal[1];

  // myx1 = relative beam coordinate in [0,L] of point_2
  real myx1 = xl;

  // point_1 lies in elem1, offset eta1 in [-1,1]
  // point_2 lies in elem2, offset eta2 in [-1,1]
  if (myx1 > myx0) 
  {
    projectPoint(x0_1,y0_1,elem1, eta1,t1);  // t1 = halfThickness
    projectPoint(x0_2,y0_2,elem2, eta2,t2);  // t2 = halfThickness  
    p11 = p1*pressureNorm;  p11x = p1x*pressureNorm; 
    p22 = p2*pressureNorm;  p22x = p2x*pressureNorm;
  } 
  else 
  {
    projectPoint(x0_1,y0_1,elem2, eta2,t2); 
    projectPoint(x0_2,y0_2,elem1, eta1,t1);  
    p22 = p1*pressureNorm; p22x = p1x*pressureNorm;
    p11 = p2*pressureNorm; p11x = p2x*pressureNorm; 
    std::swap<real>(myx0,myx1);
  }

  //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << p1 << " " << p2 << std::endl;

  const real dx12 = max(fabs(myx0-myx1),REAL_MIN*100.); // distance between point_1 and point_2

  const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  
  //                 elem1            elem2
  //        +----------+--X------+-----X--+-----
  //                      myx0        myx1
  RealArray lt(4);
  for (int i = elem1; i <= elem2; ++i) 
  {

    real a = eta1, b = eta2;
    real pa = p11, pax=p11x, pb = p22, pbx=p22x;
    real x0 = myx0, x1 = myx1;

    if (i != elem1) 
    {
      // We have moved to the next element from the first
      a = -1.0;  // xi value
      x0 = le*i; // x-value 
      if( orderOfGalerkinProjection==2 )
      {
	pa = p11 + (p22-p11)*(x0-myx0)/dx12;
      }
      else
      {
	// -- evaluate an Hermite interpolant fit to (myx0,p11)--(myx1,p22) --
	//         p11,p11x            p22,p22x
	//           X-------+-----------X
	//          myx0                myx1
	//           -1      xi          +1
	real xi = -1. + 2.*(x0-myx0)/dx12;
	// ** CHECK ME ***
	real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
	real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
	real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
	real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
	real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
	real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
	real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
	real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;
	
	pa = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
	pax = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 

      }
      
    }
    // -- right end is not on a node -- adjust pb,pbx --
    if (i != elem2) 
    {
      b = 1.0;
      x1 = le*(i+1);
      if( orderOfGalerkinProjection==2 )
      {
	pb = p11 + (p22-p11)*(x1-myx0)/dx12;
      }
      else
      {
	// -- evaluate the Hermite interpolant --
	real xi = -1. + 2.*(x1-myx0)/dx12;

	real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
	real N2 = .125*dx12*(1.-xi)*(1.-xi)*(1.+xi);
	real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
	real N4 = .125*dx12*(1.+xi)*(1.+xi)*(xi-1.);
	
	real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx12);
	real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
	real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx12) ;
	real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;

	pb = p11*N1 + p11x*N2 + p22*N3 + p22x*N4; 
	pbx = p11*N1x + p11x*N2x + p22*N3x + p22x*N4x; 
      }
    }
    
    // printF("--AF-- x0=%7.5f pa=%7.5f pax=%7.5f, x1=%7.5f pb=%7.5f pbx=%7.5f [a,b]=[%7.4f,%7.4f]\n",
    //       x0,pa,pax,x1,pb,pbx,a,b);
    
    Index idx(i*2,4);
    if( addToForce && t1 > 0 ) // t1 = halfThickness
    { // Flip sign of force to account for the normal 
      pa = -pa; pax = -pax;
      pb = -pb; pbx = -pbx;
    }
    
      
    //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
    //  p1 << " " << p2 << std::endl;
  
    if( false )
      printF("--BM-- addToElementIntegral: x1=[%g,%g] x2=[_%g,%g] pa=%g, pax=%g, pb=%g, pbx=%g a=%g b=%g f1=%g f1x=%g f2=%g f2x=%g\n",
	     x0_1,y0_1, x0_2,y0_2,pa,pax,pb,pbx,a,b,f1,f1x,f2,f2x );

    if (fabs(b-a) > 1.0e-10)  // *WDH* FIX ME -- is this needed?
    {
      // -- compute (N,p)_[a,b] = int_a^b N(xi) p(xi) J dxi 
      if( orderOfGalerkinProjection==2 )
	computeProjectedForce(pa,pb, a,b, lt);
      else
	computeGalerkinProjection(pa,pax, pb,pbx,  a,b, lt);

      //    std::cout << "a = " << a << " b = " << b << std::endl;
      fe(idx) += lt;

      if( addToForce )
      {
	// Is this needed?
	real gradp = 1.0;
	totalPressureForce += (lt(0)+lt(2));
	totalPressureMoment += (lt(0)*(le*i-0.5*L)+lt(1)*gradp+lt(2)*(le*(i+1)-0.5*L) + lt(3)*gradp);
      }
      
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


// ===================================================================================================
/// \brief  Return the nodal force values on the beam reference line. These are computed from
///   the current local element force integrals: (phi_i,f) and (psi,f) 
///  \param force (output) : force components on the beam reference-line.
// ====================================================================================================
void BeamModel::
getForceOnBeam( const real t, RealArray & force )
{

  const int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force in elemnt integral form 

  RealArray & fc = f[current];  // force at current time
  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
  {
    printF("--BM-- BeamModel::getForceOnBeam:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	   t,time(current),current);
    OV_ABORT("ERROR");
  }

  // On entry:
  //    fc holds : (phi_i,f) (psi_i,f)

  // Compute: 
  //  force  = SUM_j {  f_j phi_j(x) + f_j' psi_j(x) }
  // by solving
  //     SUM_j {  f_j (phi_i(x),phi_j(x)) + f_j' (phi_i(x),psi_j(x)) }  = (phi_i,f)
  //     SUM_j {  f_j (psi_i(x),phi_j(x)) + f_j' (psi_i(x),psi_j(x)) }  = (psi_i,f)

  real le = L / numElem;
  real le2 = le*le;
  real le3 = le2*le;
  RealArray elementMass(4,4);
  
  elementMass(0,0) = elementMass(2,2) = 13./35.*le;
  elementMass(0,1) = elementMass(1,0) = 11./210.*le2;
  elementMass(0,2) = elementMass(2,0) = 9./70.*le;
  elementMass(0,3) = elementMass(3,0) = -13./420.*le2;
  elementMass(1,1) = elementMass(3,3) = 1./105.*le3;
  elementMass(1,2) = elementMass(2,1) = 13./420.*le2;
  elementMass(1,3) = elementMass(3,1) = -1./140.*le3;
  elementMass(3,2) = elementMass(2,3) = -11./210.*le2;

  // ---- Boundary Conditions ----
  BoundaryCondition bcLeftSave =bcLeft;  // save current 
  BoundaryCondition bcRightSave=bcRight;

  // We want no boundary conditions to be applied so set:
  bcLeft=unknownBC;
  bcRight=unknownBC;

  // Note: We use a separate TridiagonalSolver for this Galerkin projection: 
  RealArray ff(2*numElem+2);
  real alpha=0., alphaB=0.; // coefficients of stiffness and damping matrices
  solveBlockTridiagonal(elementMass, fc, ff, alpha,alphaB, "galerkinProjection" );

  bcLeft =bcLeftSave;   // reset 
  bcRight=bcRightSave; 


  force.redim(numElem+1);
  force=ff(Range(0,2*numElem,2));  // extract out nodal force values, every second value
  
  if( false )
  {
    ::display(force,sPrintF("--BM-- getForceOnBeam: force at t=%9.3e",t),"%8.2e ");
  }
  
}



// ================================================================================
/// \brief Set the surface velocity to zero. (used to project the velocity) 
// ================================================================================
void BeamModel::
resetSurfaceVelocity()
{
  if( !dbase.has_key("surfaceVelocity") )
  {
    RealArray & surfaceVelocity = dbase.put<RealArray>("surfaceVelocity");
    surfaceVelocity.redim(2*numElem+2);
  }
  RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");
  surfaceVelocity=0.;
}

// ================================================================================
/// \brief  Set the surface velocity (used to project the beam velocity)
/// \param t (input) : set velocity at this time.
/// \param x0 (input) : initial positions of the points on the beam surface. 
/// \param vSurface (input) : values of the velocity on the surface
/// \param normal (input) : normal to the surface (inward!) (currently not used?)
/// \param Ib1,Ib2,Ib3 (input) : index values for points to assign. 
// ================================================================================
void BeamModel::
setSurfaceVelocity(const real & t, const RealArray & x0, const RealArray & vSurface, 
		   const RealArray & normal, const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  // *new way* 2015/01/13

  // Transfer the normal component of the velocity, stored here:
  RealArray vDotN(Ib1,Ib2,Ib3);

  const bool correctForSurfaceRotation=false;  // *CHECK ME* 2015/06/02 
  if( correctForSurfaceRotation )
  {
    // ---  Remove the surface rotation term "W" before projecting the velocity onto the beam reference line ----
    // Added: 2015/05/17 *wdh*

    const int & current = dbase.get<int>("current"); 
    std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
    std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
    RealArray & uc = u[current];  // current displacement DOF
    RealArray & vc = v[current];  // current velocity DOF

    const RealArray & time = dbase.get<RealArray>("time");
    if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("--BM-- BeamModel::setSurfaceVelocity:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     t,time(current),current);
      OV_ABORT("ERROR");
    }

    const int rangeDimension=2; // fix me 
    RealArray vBeam(Ib1,Ib2,Ib3,rangeDimension); // vBeam = vSurface - w 

    int i1,i2,i3;
    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
    {
      // (xb0,yb0) :  initial position of the point on the beam surface. 
      real xb0 = x0(i1,i2,i3,0);
      real yb0 = x0(i1,i2,i3,1);

      // --- compute "w" using the current reference line values for u, u_x, v, v_x ---
      int elemNum;
      real eta, halfThickness;
      projectPoint(xb0,yb0, elemNum, eta,halfThickness);  // halfThickness (includes sign) will be computed here
      real displacement, slope;
      interpolateSolution(uc, elemNum, eta, displacement, slope);       // displacement=u, slope = u_x 
      real Ddisplacement, Dslope;
      interpolateSolution(vc, elemNum, eta, Ddisplacement, Dslope);     //  Ddisplacement = v, Dslope=v_x 

      real omag = 1./sqrt(slope*slope+1.0);
      real omag3 = omag*omag*omag;
      real normald[2] = {-Dslope*omag3,-slope*Dslope*omag3};

      if( !allowsFreeMotion ) 
      { // -- subtract off "w"
	for( int axis=0; axis<rangeDimension; axis++ )
	  vBeam(i1,i2,i3,axis) = vSurface(i1,i2,i3,axis) - normald[axis]*halfThickness;
      }
      else
      {
        OV_ABORT("finish me");
      }
      
    }
    // Transfer the normal component of the velocity: (this needs to be fixed when beam supports motion
    //   in two directions)
    vDotN(Ib1,Ib2,Ib3)= (vBeam(Ib1,Ib2,Ib3,0)*initialBeamNormal[0] +
			 vBeam(Ib1,Ib2,Ib3,1)*initialBeamNormal[1] );

  }
  else
  {
    // -- no correction for surface rotation

    // we transfer the normal component of the velocity -- here we use the initial beam normal vector *IS THIS RIGHT ?? **
    vDotN(Ib1,Ib2,Ib3)= (vSurface(Ib1,Ib2,Ib3,0)*initialBeamNormal[0] +
			 vSurface(Ib1,Ib2,Ib3,1)*initialBeamNormal[1] );
  }
  
  RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");
  addToElementIntegral( t,x0,vDotN,normal,Ib1,Ib2,Ib3,surfaceVelocity );

  return;

  // if( false )
  // {
  //   //   ** OLD WAY **
  //   // ---- THIS CODE IS VERY SIMILAR TO addForce : should combine somehow ----

  //   // THIS CODE IS IN-EFFICIENT --> RE-WRITE

  //   // Jb1, Jb2, Jb3 : for looping over cells instead of grid points -- decrease by 1 along active axis
  //   Index Jb1=Ib1, Jb2=Ib2, Jb3=Ib3;
  //   Index Jg1=Ib1, Jg2=Ib2, Jg3=Ib3;
  //   int ia, ib; // index for boundary points
  //   int iga, igb; // index for ghost points
  //   int axis=-1, is1=0, is2=0, is3=0;
  //   if( Jb1.getLength()>1 )
  //   { // grid points on boundary are along axis=0
  //     axis=0; is1=1;
  //     ia=Ib1.getBase(); ib=Ib1.getBound();
  //     Jb1=Range(Jb1.getBase(),Jb1.getBound()-1); // decrease length by 1
  //     assert( Jb2.getLength()==1 );
  //     Jg1=Range(Jg1.getBase()-1,Jg1.getBound()+1); // add ghost
  //     iga = Jg1.getBase(); igb=Jg1.getBound();
  //   }
  //   else
  //   { // grid points on boundary are along axis=1
  //     axis=1; is2=1;
  //     ia=Ib2.getBase(); ib=Ib2.getBound();
  //     Jb2=Range(Jb2.getBase(),Jb2.getBound()-1); // decrease length by 1
  //     assert( Jb1.getLength()==1 );
  //     Jg2=Range(Jg2.getBase()-1,Jg2.getBound()+1); // add ghost
  //     iga = Jg2.getBase(); igb=Jg2.getBound();
  //   }
  //   assert( axis>=0 );
  
  
  //   const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
  //   const int orderOfAccuracyForDerivative=orderOfGalerkinProjection;

  //   RealArray vDotN(Jg1,Jg2,Jg3); // add ghost

  //   int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  //   FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  //   {
  //     vDotN(iv[axis]) = (vSurface(i1,i2,i3,0)*initialBeamNormal[0]+
  // 			 vSurface(i1,i2,i3,1)*initialBeamNormal[1] );
  //     // vDotN(iv[axis]) = (vSurface(i1,i2,i3,0)*normal(i1,i2,i3,0)+
  //     // 	               vSurface(i1,i2,i3,1)*normal(i1,i2,i3,1) );
  //   }
  //   // extrapolate ghost
  //   if( orderOfAccuracyForDerivative==4 && igb > iga+5 )
  //   { // this is needed for fourth-order accuracy 
  //     vDotN(iga)=5.*vDotN(iga+1)-10.*vDotN(iga+2)+10.*vDotN(iga+3)-5.*vDotN(iga+4)+vDotN(iga+5);
  //     vDotN(igb)=5.*vDotN(igb-1)-10.*vDotN(igb-2)+10.*vDotN(igb-3)-5.*vDotN(igb-4)+vDotN(igb-5);
  //   }
  //   else if( orderOfAccuracyForDerivative==4 &&  igb > iga+4 )
  //   {
  //     vDotN(iga)=4.*vDotN(iga+1)-6.*vDotN(iga+2)+4.*vDotN(iga+3)-vDotN(iga+4);
  //     vDotN(igb)=4.*vDotN(igb-1)-6.*vDotN(igb-2)+4.*vDotN(igb-3)-vDotN(igb-4);
  //   }
  //   else if( igb > iga+3 )
  //   {
  //     vDotN(iga)=3.*vDotN(iga+1)-3.*vDotN(iga+2)+vDotN(iga+3);
  //     vDotN(igb)=3.*vDotN(igb-1)-3.*vDotN(igb-2)+vDotN(igb-3);
  //   }
  //   else
  //   {
  //     assert( igb> iga+2 );
  //     vDotN(iga)=2.*vDotN(iga+1)-vDotN(iga+2);
  //     vDotN(igb)=2.*vDotN(igb-1)-vDotN(igb-2);
  //   }
  
  
  //   real beamLength=L;
  //   const real dx = beamLength/numElem;  
  //   FOR_3(i1,i2,i3,Jb1,Jb2,Jb3)
  //   {
  //     int i1p=i1+is1, i2p=i2+is2, i3p=i3+is3;
  //     real v1x, v2x;
  //     if( orderOfAccuracyForDerivative==2 )
  //     {
  // 	v1x = (vDotN(iv[axis]+1)-vDotN(iv[axis]-1))/(2.*dx);
  // 	v2x = (vDotN(iv[axis]+2)-vDotN(iv[axis]  ))/(2.*dx);
  //     }
  //     else
  //     {
  // 	int i=iv[axis];
  // 	if( i-2 >= iga && i+2 <= igb )
  // 	  v1x = ( 8.*(vDotN(i+1)-vDotN(i-1)) - vDotN(i+2) +vDotN(i-2) )/(12.*dx);
  // 	else if( i==ia && i+4 <=igb )
  // 	{ // fourth-order one-sided
  // 	  v1x = ( -(25./12.)*vDotN(i) + 4.*vDotN(i+1) -3.*vDotN(i+2) + (4./3.)*vDotN(i+3) -.25*vDotN(i+4) )/dx;
  // 	}
  // 	else
  // 	  v1x = (vDotN(i+1)-vDotN(i-1))/(2.*dx);
      
  // 	i=iv[axis]+1;
  // 	if( i-2 >= iga && i+2 <= igb )
  // 	  v2x = ( 8.*(vDotN(i+1)-vDotN(i-1)) - vDotN(i+2) +vDotN(i-2) )/(12.*dx);
  // 	else if( i==ib && i-4 >=iga )
  // 	{ // fourth-order one-sided
  // 	  v2x = -( -(25./12.)*vDotN(i) + 4.*vDotN(i-1) -3.*vDotN(i-2) + (4./3.)*vDotN(i-3) -.25*vDotN(i-4) )/dx;
  // 	}
  // 	else
  // 	  v2x = (vDotN(i+1)-vDotN(i-1))/(2.*dx);
  //     }
    
  //     printF("--BM-- setSurfaceVelocity: x0=[%8.2e,%8.2e] v1=%8.2e v1x=%8.2e, x0=[%8.2e,%8.2e] v2=%8.2e v2x=%8.2e\n",
  // 	     x0(i1,i2,i3,0),x0(i1,i2,i3,1),  vDotN(iv[axis]), v1x, 
  // 	     x0(i1p,i2p,i3p,0),x0(i1p,i2p,i3p,1), vDotN(iv[axis]+1), v2x );
  //     setSurfaceVelocity(t,
  // 			 x0(i1,i2,i3,0), 
  // 			 x0(i1,i2,i3,1), vDotN(iv[axis]), v1x, 
  // 			 normal(i1,i2,i3,0), normal(i1,i2,i3,1),
  // 			 x0(i1p,i2p,i3p,0), 
  // 			 x0(i1p,i2p,i3p,1), vDotN(iv[axis]+1), v2x, 
  // 			 normal(i1p,i2p,i3p,0), normal(i1p,i2p,i3p,1));
  //   }
  
  //   if( true )
  //   {
  //     RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");

  //     ::display(surfaceVelocity,sPrintF("--BM-- setSurfaceVelocity:END: : surfaceVelocity at t=%9.3e",t),"%8.2e ");

  //   }
  // }
  
}

// ==========================================================================================================
/// \brief Return the "surfaceVelocity" array (used for projecting the beam velocity in FSI simulations)
// ==========================================================================================================
const RealArray& BeamModel::
getSurfaceVelocity() const
{
  return dbase.get<RealArray>("surfaceVelocity"); 
}


// ======================================================================================
/// *THIS FUNCTION NOT USED ANYMORE*
/// \brief  Set the surface velocity over an interval
// undeformed location is X1 = (x0_1, y0_1), X2 = (x0_2, y0_2).
// The velocity is v(X1) = v1, v(X2) = v2
/// \param tf : set surface velocity at this time 
/// \param  x0_1: undeformed location of the point on the surface of the beam (x1)  
/// \param  y0_1: undeformed location of the point on the surface of the beam (y1)
/// \param  v1:   velocity at the point (x1,y1)
/// \param  v1x:   x-derivative of the velocity at the point (x1,y1) (for 4th-order projection)
/// \param  nx_1: normal at x1 (x) [unused]
/// \param  ny_1: normal at x1 (y) [unused]
/// \param  x0_2: undeformed location of the point on the surface of the beam (x2)  
/// \param  y0_2: undeformed location of the point on the surface of the beam (y2)  
/// \param  v2:   velocity at the point (x2,y2)
/// \param  vx2:  x-derivative of the  velocity at the point (x2,y2) (for 4th-order projection)
/// \param  nx_2: normal at x2 (x) [unused]
/// \param  ny_2: normal at x2 (y) [unused]
//
// ======================================================================================
void BeamModel::
setSurfaceVelocity( const real & tf,
		    const real& x0_1, const real& y0_1,
		    real v1, real v1x, const real& nx_1,const real& ny_1,
		    const real& x0_2, const real& y0_2,
		    real v2, real v2x, const real& nx_2,const real& ny_2)
{

  // new way *wdh* 2015/01/13
  real x1[2]={ x0_1,y0_1}, nv1[2]={nx_1,ny_1}; //
  real x2[2]={ x0_2,y0_2}, nv2[2]={nx_2,ny_2}; //

  RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");
  addToElementIntegral( tf,x1,v1,v1x,nv1, x2,v2,v2x,nv2,surfaceVelocity );

  // **OLD **

  // // THIS FUNCTION IS ALMOST THE SAME AS ADDFORCE ** FIX ME**

  // const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  // const int & current = dbase.get<int>("current"); 

  // RealArray & time = dbase.get<RealArray>("time");
  // RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");

  // if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
  // {
  //   printF("--BM-- BeamModel::setSurfaceVelocity:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
  //       tf,time(current),current);
  //   OV_ABORT("ERROR");
  // }

  // int elem1,elem2;
  // real eta1,eta2,t1,t2;

  // real v11,v22, v11x, v22x;


  // real xll = x0_1-beamX0;
  // real yll = y0_1-beamY0;

  // real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  // real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  // real myx0 = xl;

  // xll = x0_2-beamX0;
  // yll = y0_2-beamY0;

  // xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
  // yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

  // real myx1 = xl;

  // if (myx1 > myx0) 
  // {
  //   projectPoint(x0_1,y0_1,elem1, eta1,t1); 
  //   projectPoint(x0_2,y0_2,elem2, eta2,t2);   
  //   v11 = v1; v11x=v1x;
  //   v22 = v2; v22x=v2x;
  // } 
  // else 
  // {
  //   projectPoint(x0_1,y0_1,elem2, eta2,t2); 
  //   projectPoint(x0_2,y0_2,elem1, eta1,t1);  
  //   v22 = v1; v22x=v1x;
  //   v11 = v2; v11x=v2x;
  //   std::swap<real>(myx0,myx1);
  // }

  // //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << v1 << " " << v2 << std::endl;

  // real dx = fabs(myx0-myx1);


  // const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");

  // RealArray lt(4);
  // for (int i = elem1; i <= elem2; ++i) 
  // {
  //   real a = eta1, b = eta2;
  //   real va = v11, vax=v11x, vb = v22, vbx=v22x;
  //   real x0 = myx0, x1 = myx1;
  //   if (i != elem1)
  //   {
  //     // estimate v at xi=x0 : 
  //     a = -1.0;
  //     x0 = le*i;
      
  //     if( orderOfGalerkinProjection==2 )
  //     {
  //       va = v11 + (v22-v11)*(x0-myx0)/(dx);
  //     }
  //     else
  //     {
  //       // -- evaluate the Hermite interpolant --
  //       real xi = -1. + 2.*(x0-myx0)/le;

  //       real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
  //       real N2 = .125*dx*(1.-xi)*(1.-xi)*(1.+xi);
  //       real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
  //       real N4 = .125*dx*(1.+xi)*(1.+xi)*(xi-1.);
	
  //       real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx);
  //       real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
  //       real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx) ;
  //       real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;
	
  //       va = v11*N1 + v11x*N2 + v22*N3 + v22x*N4; 
  //       vax = v11*N1x + v11x*N2x + v22*N3x + v22x*N4x; 

  //       // vax = v11x + (v22x-v11x)*(x0-myx0)/(dx);
  //     }
      
  //   }
  //   if (i != elem2) 
  //   {
  //     b = 1.0;
  //     x1 = le*(i+1);
  //     if( orderOfGalerkinProjection==2 )
  //     {
  //       vb = v11 + (v22-v11)*(x1-myx0)/(dx);
  //     }
  //     else
  //     {
  //       // -- evaluate the Hermite interpolant --
  //       real xi = -1. + 2.*(x1-myx0)/le;

  //       real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
  //       real N2 = .125*dx*(1.-xi)*(1.-xi)*(1.+xi);
  //       real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
  //       real N4 = .125*dx*(1.+xi)*(1.+xi)*(xi-1.);
	
  //       real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx);
  //       real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
  //       real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx) ;
  //       real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;

  //       vb = v11*N1 + v11x*N2 + v22*N3 + v22x*N4; 
  //       vbx = v11*N1x + v11x*N2x + v22*N3x + v22x*N4x; 
  //       // vbx = v11x + (v22x-v11x)*(x1-myx0)/(dx);
  //     }
      
  //   }
    
  //   Index idx(i*2,4);
  //   if( FALSE && t1 > 0)   // TURN OFF FOR VELOCITY 
  //   { 
  //     va = -va; vax=-vax;
  //     vb = -vb; vbx=-vbx;
  //   }
      
  //   //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
  //   //  v1 << " " << v2 << std::endl;
  
    
  //   if( fabs(b-a) > 1.0e-10 )  // *WDH* FIX ME -- is this needed?
  //   {
  //     // -- compute (N,p)_[a,b] = int_a^b N(xi) v(xi) J dxi 
  //     if( orderOfGalerkinProjection==2 )
  //       computeProjectedForce(va,vb, a,b, lt);
  //     else
  //       computeGalerkinProjection(va,vax, vb,vbx,  a,b, lt);
        
  //     //    std::cout << "a = " << a << " b = " << b << std::endl;
  //     surfaceVelocity(idx) += lt;  
  //   }

  // }
}


// ===================================================================================================
/// \brief  Project the current surface velocity onto the beam (and over-write current beam velocity)
// ====================================================================================================
void BeamModel::
projectSurfaceVelocityOntoBeam( const real t )
{

  RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");

  // On entry:
  //   surfaceVelocity should hold the integrals (phi_i,v) (psi_i,v)

  // Stage I: Compute the FEM  coefficients v_j and v_j' coefficients in
  //       v = SUM_j {  v_j phi_j(x) + v_j' psi_j(x) }
  // by solving
  //     SUM_j {  v_j (phi_i(x),phi_j(x)) + v_j' (phi_i(x),psi_j(x)) }  = (phi_i,v)
  //     SUM_j {  v_j (psi_i(x),phi_j(x)) + v_j' (psi_i(x),psi_j(x)) }  = (psi_i,v)

  real le = L / numElem;
  real le2 = le*le;
  real le3 = le2*le;
  RealArray elementMass(4,4);
  
  elementMass(0,0) = elementMass(2,2) = 13./35.*le;
  elementMass(0,1) = elementMass(1,0) = 11./210.*le2;
  elementMass(0,2) = elementMass(2,0) = 9./70.*le;
  elementMass(0,3) = elementMass(3,0) = -13./420.*le2;
  elementMass(1,1) = elementMass(3,3) = 1./105.*le3;
  elementMass(1,2) = elementMass(2,1) = 13./420.*le2;
  elementMass(1,3) = elementMass(3,1) = -1./140.*le3;
  elementMass(3,2) = elementMass(2,3) = -11./210.*le2;

  RealArray rhs;
  if( dbase.get<bool>("fluidOnTwoSides") )
    rhs=.5*surfaceVelocity;  // scale by .5 for two-sided fluid 
  else
    rhs=surfaceVelocity;

  real alpha=0., alphaB=0.; // coefficients of stiffness and damping matrices

  // Assign BC's on v (rhs)
  RealArray uTemp(2*numElem+2), aTemp(2*numElem+2);
  if( true )
    assignBoundaryConditions( t, uTemp, rhs, aTemp );

  // Note: We use a separate TridiagonalSolver for this Galerkin projection: 
  solveBlockTridiagonal(elementMass, rhs, surfaceVelocity, alpha,alphaB, "galerkinProjection" );

  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  const int & current = dbase.get<int>("current");

  if( FALSE )
  {
    ::display(v[current],sPrintF("--BM-- projectSurfaceVelocityOntoBeam:END: : v[current] at t=%9.3e",t),"%8.2e ");

    ::display(surfaceVelocity,sPrintF("--BM-- projectSurfaceVelocityOntoBeam: surfaceVelocity at t=%9.3e",t),"%8.2e ");
    real maxErr = max(fabs(v[current]-surfaceVelocity));
    printF("--BM-- projectSurfaceVelocityOntoBeam max-err=%8.2e\n",maxErr);

  }

  // Replace the current velocity DOF's 
  v[current]=surfaceVelocity;

  if( true )
  {
    // -- smooth the projected velocity ---
    smooth( t,v[current], "v: projectSurfaceVelocityOntoBeam" );
  }
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

      else if( bc==slideBC ) 
      {
	// --- slide BC ---
	if( twilightZone )
	{
	  g(1,side) = exact.gd(ntd  ,1,0,0, x,y,z,wc,t);    // Give w.x 
	  g(2,side) = exact.gd(ntd+1,1,0,0, x,y,z,wc,t);    // Give w.xt
	  g(3,side) = exact.gd(ntd  ,3,0,0, x,y,z,wc,t);    // Give EI*w_xxx
	}
	else
	{
	  g(1,side)=0.;
	  g(3,side)=0.;
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


      else if( bc==periodic || bc==unknownBC || bc==internalForceBC )
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
    if( bcLeft!=periodic && exactSolutionOption=="travelingWaveFSI" )
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
      if( bc==clamped || bc==slideBC )
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
      if( bc == clamped  || bc==slideBC ) 
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
    const real & K0 = dbase.get<real>("K0");
    const real & Kt = dbase.get<real>("Kt");
    const real & Kxxt = dbase.get<real>("Kxxt");

    RealArray ue(I1,I2,I3,1), utte(I1,I2,I3,1), uxxe(I1,I2,I3,1), uxxxxe(I1,I2,I3,1);
    int isRectangular=0;
    const int wc=0;
    exact.gd( ue     ,x,domainDimension,isRectangular,0,0,0,0,I1,I2,I3,wc,t );
    exact.gd( utte   ,x,domainDimension,isRectangular,2,0,0,0,I1,I2,I3,wc,t );
    exact.gd( uxxe   ,x,domainDimension,isRectangular,0,2,0,0,I1,I2,I3,wc,t );
    exact.gd( uxxxxe ,x,domainDimension,isRectangular,0,4,0,0,I1,I2,I3,wc,t );

    // printF("-- BM -- addInternalForce: t=%9.3e max(fabs(f))=%8.2e, |utte|=%8.2e |u_xxxxe|=%8.2e\n",
    //        t,max(fabs(f)),max(fabs(utte)),max(fabs(uxxxxe)));

    RealArray ftz(I1,I2,I3,1), ftzx(I1,I2,I3,1); 
    ftz = (density*thickness)*utte + K0*ue - (T)*uxxe + (EI)*uxxxxe;

    const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");
    if( orderOfGalerkinProjection==4 )
    {
      RealArray uxe(I1,I2,I3,1), uttxe(I1,I2,I3,1), uxxxe(I1,I2,I3,1), uxxxxxe(I1,I2,I3,1);
      exact.gd( uxe     ,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
      exact.gd( uttxe   ,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
      exact.gd( uxxxe   ,x,domainDimension,isRectangular,0,3,0,0,I1,I2,I3,wc,t );
      exact.gd( uxxxxxe ,x,domainDimension,isRectangular,0,5,0,0,I1,I2,I3,wc,t );

      ftzx = (density*thickness)*uttxe + K0*uxe - (T)*uxxxe + (EI)*uxxxxxe;  // x-derivative of the TZ force
    }
    
    if( Kt!=0. )
    {
      RealArray & ute = uxxe;  // re-use space
      exact.gd( ute, x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
      ftz += Kt*ute;
      if( orderOfGalerkinProjection==4 )
      {
	RealArray & utxe = uxxe;  // re-use space
	exact.gd( utxe, x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
	ftzx += Kt*utxe;
      }
    }
    if( Kxxt!=0. )
    {
      RealArray & utxxe = uxxe;  // re-use space
      exact.gd( utxxe, x,domainDimension,isRectangular,1,2,0,0,I1,I2,I3,wc,t );
      ftz += (-Kxxt)*utxxe;
      if( orderOfGalerkinProjection==4 )
      {
	RealArray & utxxxe = uxxe;  // re-use space
	exact.gd( utxxxe, x,domainDimension,isRectangular,1,3,0,0,I1,I2,I3,wc,t );
	ftzx += (-Kxxt)*utxxxe;
      }
      
    }
    
    // ::display(utte,"utte","%8.2e ");
    // ::display(uxxe,"uxxe","%8.2e ");
    // ::display(ftz,"ftz","%8.2e ");
    
    RealArray lt(4); // local traction
    for ( int i = 0; i<numElem; i++ )
    {
      // computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);

      if( orderOfGalerkinProjection==2 )
        computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);
      else
        computeGalerkinProjection( ftz(i),ftzx(i), ftz(i+1),ftzx(i+1),   -1.0,1.0, lt);

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
  const bool & relaxForce = dbase.get<bool>("relaxForce");

  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const RealArray & elementB = dbase.get<RealArray>("elementB");
  
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

  const real dtOld = dbase.get<real>("dt"); 
  if( fabs(dt-dtOld) > REAL_EPSILON*10.*dt )
  {
    refactor=true;
    printF("-- BM -- predictor: dt has changed, dt=%9.3e, dtOld=%9.3e, will refactor.\n",dt,dtOld);
  }
  dbase.get<real>("dt")=dt; // adjust "dtOld"

  if( false && t<2.*dt )
  { 
    // wdh: debug info: 
    printF("************** BeamModel::predictor: t=%9.3e ***********************\n",t);
    ::display(x2,"x2","%8.2e ");
    ::display(v2,"v2","%8.2e ");
  }
  if( debug & 4 )
  {
    ::display(f2(Range(0,2*numElem  ,2)),"BeamModel::predictor: RHS force f2(0:2:) BEFORE addInternalForces","%8.2e ");
    ::display(f2(Range(1,2*numElem+1,2)),"BeamModel::predictor: RHS force f2(1:2:) BEFORE addInternalForces","%8.2e ");
  }

  if( false )
  {
    smooth( t-dt,x2, "u: predictor" );
    smooth( t-dt,v2, "v: predictor" );
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
    const real alpha =0.;  // coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
    const real alphaB=0.;  // coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
    computeAcceleration(t-dt, x2,v2,f2, elementM, a2,
			centerOfMassAcceleration, angularAcceleration,
			dt, alpha,alphaB, "tridiagonalSolver" );
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

      RealArray A; 
      A = elementM + (newmarkBeta*dt*dt)*elementK;
      if( addDampingMatrix )
      { // add damping matrix B 
	A += (newmarkGamma*dt)*elementB;
      }
      

      real linaccel[2],omegadd;
      // compute acceleration at time t^{n+1}
      const real alpha =newmarkBeta*dt*dt;  // coeff of K in A
      const real alphaB=newmarkGamma*dt;    // coeff of B in A

      if( false ) // apply BC's to first-order predictor 
        assignBoundaryConditions( tnp1, dtilde,vtilde,a3 );

      if( tnp1 >= 1.5*dt ) // *wdh* 2015/01/29 : NOTE: t==tnp1
      {
        // -- we have 2 old forces available: f(tnp1-dt) and f(tnp1-2*dt) --
        RealArray & f1 = f[prev2];
        // f(t+dt) = f(t   ) + dt*f'            = f2 + dt*f'
        // f(t+dt) = f(t-dtOld) + (dt+dtOld)*f' = f1 + dtp*f' 
        real dtr=dt/dtOld;
     
        f3=(1.+dtr)*f2-dtr*f1;  // extrapolate in time (first order)
        // f3=2.*f2-f1;            // extrapolate in time ** FIX ME for variable dt **
      }
      else
      {
        // -- we only have 1 old forces at f(tnp1-dt)
        f3=f2; 
      }
      
      computeAcceleration( tnp1, dtilde,vtilde,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver" );

      v3 = vtilde + (newmarkGamma*dt)*a3;
      x3 = dtilde + (newmarkBeta*dt*dt)*a3;

      if( false && t<=2.*dt )
      {
	printF("--BM-- predictor: tnp1=%9.3e\n",t,tnp1);
	::display(f2,"f2","%8.2e ");
	::display(f3,"f3","%8.2e ");
	::display(v3,"v3","%8.2e ");
	::display(a2,"a2","%8.2e ");
	::display(a3,"a3","%8.2e ");
      }
      

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
  RealArray & fOld = dbase.get<RealArray>("fOld");
  if( relaxForce )
  {
    aold = a2;   // set aold to previous acceleration *wdh* 2014/06/19 
    fOld = f2;
    // aold=0.;  // **** TEST
  }
  
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

  
  // optionally smooth the solution:
  //  smooth( tnp1,x3, "u: predictor" );
  //   smooth( tnp1,v3, "v: predictor" );


  if( debug & 2 )
  {
    aString buff;
    getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM : after %s predict t=%9.3e",
				      (useSecondOrderNewmarkPredictor ? "2nd-order" : "first-order"),tnp1));
  }
  


  const bool & saveProfileFile = dbase.get<bool>("saveProfileFile");

  if( useExactSolution && saveProfileFile ) 
  {
    RealArray xtmp = x3;
    RealArray vtmp = v3;
    RealArray atmp = a2;

    setExactSolution(t,xtmp,vtmp,atmp);

    std::stringstream profname;
    profname << "beam_profile" << numberOfTimeSteps << ".txt";
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

  numberOfTimeSteps++;

  numCorrectorIterations = 0;

}

// =================================================================================
/// \brief  Return the maximum relative correction for sub-iterations
// =================================================================================
real BeamModel::
getMaximumRelativeCorrection() const
{
  return dbase.get<real>("maximumRelativeCorrection");
}


// ===================================================================================
// \brief compute the 2-norm squared of a Hermite Grid function
// ===================================================================================
real BeamModel::
norm( RealArray & u ) const
{
  real uNorm=0;
  for (int i = 0; i < numElem; ++i) 
  {
    // norm of | a3 - aold |   *wdh: should we scale by dx ?  
    uNorm += u(i*2)*u(i*2);
  }
  return uNorm;
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
  const bool & relaxForce = dbase.get<bool>("relaxForce");
  const bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const RealArray & elementB = dbase.get<RealArray>("elementB");
  
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

  RealArray A;
  A = elementM + (newmarkBeta*dt*dt)*elementK;
  if( addDampingMatrix )
  { // add damping matrix B 
    A += (newmarkGamma*dt)*elementB;
  }  
    
  const bool & relaxCorrectionSteps=dbase.get<bool>("relaxCorrectionSteps");
  const real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  const real & subIterationAbsoluteTolerance = dbase.get<real>("subIterationAbsoluteTolerance");
  const real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  const bool & useAitkenAcceleration = dbase.get<bool>("useAitkenAcceleration");
  real & maximumRelativeCorrection = dbase.get<real>("maximumRelativeCorrection"); 

  // Traditional partitioned scheme: we under-relax the acceleration and iterate:
  real omega = addedMassRelaxationFactor;

  RealArray & fOld = dbase.get<RealArray>("fOld");
  RealArray & fOlder = dbase.get<RealArray>("fOlder");
  if( fOld.elementCount()==0 )
  {
    // This corrector may be called at t=0. -- in this case set the "old" force to be zero
    fOld.redim(2*(numElem+1));
    fOld=0.;
  }
  
  if( relaxForce  )
  {
    // ::display(fOld,"fOld","%8.2e ");
    // ::display(f3,"f3","%8.2e ");
    f3 = omega*f3+(1.0-omega)*fOld; 
    
  }

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
  
  if( FALSE &&    // *wdh* 2015/03/10 
      numberOfTimeSteps == 1 )  // *wdh* -- what is this ?
  {
    correctionHasConverged = true;
    centerOfMassAcceleration[0] = buoyantMass / totalMass * bodyForce[0];
    centerOfMassAcceleration[1] = buoyantMass / totalMass * bodyForce[1];

    return;
  }


  real linaccel[2],omegadd;
  // compute acceleration at time t^{n+1}
  const real alpha=newmarkBeta*dt*dt;   // coeff of K in A
  const real alphaB=newmarkGamma*dt;    // coeff of B in A
  computeAcceleration(tnp1, dtilde,v3,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver");

  //v3 = vtilde+newmarkGamma*dt*(myAcceleration-aold)*omega;
  //x3 = dtilde+newmarkBeta*dt*dt*(myAcceleration-aold)*omega;

  // // under-relax the acceleration 
  if( !relaxForce )
  {
    if( relaxCorrectionSteps )
      a3 = omega*a3+(1.0-omega)*aold;    
  }
  
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

  // --- Check for convergence of sub-iteration ---

  real correction = 0.0;
  if( relaxCorrectionSteps )
  {
    correctionHasConverged = false;
    RealArray tmp;
    if( relaxForce )
      tmp = f3-fOld;
    else
      tmp = a3-aold;
    // if( numCorrectorIterations==0 )
    //   ::display(tmp,"--BM-- a3-aold : first correction","%8.2e ");
    
    if( true )
    {
      correction=norm(tmp); // no need to scale by dx since we compare to another unscaled norm
    }
    else
    {
      for (int i = 0; i < numElem; ++i) 
      {
	// norm of | a3 - aold |   *wdh: should we scale by dx ?  
	correction += tmp(i*2)*tmp(i*2)/(le*le)+ tmp(i*2+1)*tmp(i*2+1);  
      }
    }
    
  }
  else
  {
    correctionHasConverged = true;
  }
  
  if (allowsFreeMotion) 
  {
    real tmpp[3];
    tmpp[0] = old_rb_acceleration[0]-centerOfMassAcceleration[0];
    tmpp[1] = old_rb_acceleration[1]-centerOfMassAcceleration[1];

    tmpp[2] = old_rb_acceleration[2]-angularAcceleration;

    if( relaxCorrectionSteps )
      for (int k = 0; k < 3; ++k)
	correction += tmpp[k]*tmpp[k];
  }

  // // under-relax the acceleration 
  // if( relaxCorrectionSteps )
  //   a3 = omega*a3+(1.0-omega)*aold;    

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
    
  
  if( relaxCorrectionSteps )
  {
    correction = sqrt(correction);

    if( numCorrectorIterations == 0 )
    {
      // *wdh* 2015/03/07 initialResidual = correction;
      // initialResidual = sqrt(norm(aold));
      initialResidual = sqrt(norm(f3));
    }
    
    ++numCorrectorIterations;

  
    // --- Here is the convergence test ---
    // if (correction < initialResidual*subIterationConvergenceTolerance || correction < 1e-8)
    //   correctionHasConverged = true;
    // maximumRelativeCorrection=correction/max(initialResidual,1.e-5);  // save current value
    const real eps=1.e-10; // WHAT SHOULD THIS BE ? 
    maximumRelativeCorrection=correction/max(initialResidual,eps);  // save current value

    if( maximumRelativeCorrection < subIterationConvergenceTolerance || 
        correction < subIterationAbsoluteTolerance )
    {
      correctionHasConverged = true;
    }
    if( true && (debug & 2) )
    {
      printF("--BM-- TP-iteration: omega=%6.3f, iter=%i, corr=%8.2e rel-corr=%8.2e, rtol=%8.2e, atol=%8.2e converged=%i\n",
	     omega,numCorrectorIterations,correction,maximumRelativeCorrection, subIterationConvergenceTolerance,
	     subIterationAbsoluteTolerance, (int)correctionHasConverged );
    }
    
  }

  assignBoundaryConditions( t,x3,v3,a3 );

  // optionally smooth the solution:
  if( true )
  {
    smooth( t,x3, "u: corrector" );
    smooth( t,v3, "v: corrector" );
  }
  
  if( relaxForce )
  {
    // *** THIS DOES NOT WORK YET ***
    // Steffensen's Method:
    //   1. Choose x0
    //   2. Compute x1, x2 from Fixed-Point iteration
    //   3. Use Aitkens method to compute new x0
    //   4. Repeat steps 2,3

    // ***FIX ME -- This is not working yet ****
    if( useAitkenAcceleration 
        && numCorrectorIterations>1  // NOTE: numCorrectorIterations has been incremented already
        // && numCorrectorIterations>10
        && (numCorrectorIterations % 4)==0    // *try this*
        && (numCorrectorIterations % 2)==0 )
    {
      // ::display(f3,"f3");
      // ::display(fOld,"fOld");
      // ::display(fOlder,"fOlder");

      RealArray fNew, denom;
      denom = f3 -2.*fOld + fOlder;    // watch out for denom = 0 ??
      
      fOlder = fOld;                   // save (not used?)
      fNew = f3 - SQR(f3-fOld)/denom;  // Aitken value
      f3 = .5*fNew + .5*f3;            // under-relax Aitken   
      // f3=fNew; 
      fOld = f3;                     
    }
    else
    {
      aold = a3; // aold holds current value of acceleration
      fOlder = fOld;
      fOld = f3;
    }
  }
  

  if( debug & 2 )
  {
    aString buff;
    getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM : after correct t=%9.3e",tnp1));
  }

}
// ====================================================================================
/// \brief Output probe info.
/// \param t (input) : current time
/// \param stepNumber (input) : global time step number 
// ====================================================================================
int BeamModel::
outputProbes( real t, int stepNumber )
{

  // --- Output to the probe file ---
  const int & probeFileSaveFrequency = dbase.get<int>("probeFileSaveFrequency");
  if( dbase.get<bool>("saveProbeFile") && 
      (stepNumber % probeFileSaveFrequency)==0 )
  {
    FILE *probeFile = dbase.get<FILE*>("probeFile");
    assert( probeFile!=NULL );

    // const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
    const int & current = dbase.get<int>("current"); 

    RealArray & time = dbase.get<RealArray>("time");
    std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
    std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
    std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

    RealArray & x3 = u[current];
    RealArray & v3 = v[current];
    RealArray & a3 = a[current];

    if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("BeamModel::outputProbes:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     t,time(current),current);
      OV_ABORT("ERROR");
    }

    // *new* 2015/06/04
    const real & probePosition =  dbase.get<real>("probePosition");
    const real ss = probePosition*L;
    const real xp0 = beamX0 + ss*initialBeamTangent[0];
    const real yp0 = beamY0 + ss*initialBeamTangent[1];
    int elemNum;
    real eta, halfThickness;
    projectPoint(xp0,yp0, elemNum, eta,halfThickness);  // halfThickness (includes sign) will be computed here
    real up, upx, vp,vpx, ap,apx;
    interpolateSolution(x3, elemNum,eta, up, upx);
    interpolateSolution(v3, elemNum,eta, vp, vpx);
    interpolateSolution(a3, elemNum,eta, ap, apx);

    if( debug & 2 )
      printf("--BM-- save probe: t=%9.3e (xp0,yp0)=(%9.3e,%9.3e) (up,vp,ap)=(%9.3e,%9.3e,%9.3e) elemNum=%i eta=%9.3e\n",
	     t,xp0,yp0,up,vp,ap,elemNum,eta);
      
    fPrintF(probeFile,"%16.10e %16.10e %16.10e %16.10e\n", t, up, vp, ap);

    // *old way* 
    // const real beamLength=L;
    // const real dx=beamLength/numElem;
    // int i1=numElem; // save tip for now
    // real xb = i1*dx; 
    // real yb = 0.;
    
    // fPrintF(probeFile,"%16.10e %16.10e %16.10e %16.10e\n", t, x3(2*i1), v3(2*i1),a3(2*i1));
    // output << t << " " <<  x3(numElem*2) << " " << v3(numElem*2) << " " <<  a3(numElem*2) << std::endl;
  }


  return 0;
}
  
// ====================================================================================
/// \brief Apply boundary conditions for the smooth function.
// ====================================================================================
int BeamModel::
smoothBoundaryConditions( RealArray & w1, int base, int bound,
                          int numberOfGhost, int orderOfExtrapolation )
{
    
  // -- boundary conditions -- 
  const int bc[2]={bcLeft,bcRight};  
  const bool isPeriodic = bcLeft==periodic;
  Range R2(0,1);

  if( isPeriodic )
  {
    w1(bound,R2)=w1(base,R2);
    for( int g=1; g<=numberOfGhost; g++ )
    {
      w1(base -g,R2)=w1(bound-g,R2);
      w1(bound+g,R2)=w1(base +g,R2);
    }
  }
  else
  {
    // --- Assign Ghost ---
    for( int side=0; side<=1; side++ )
    {
      int ib  = side==0 ? base : bound; // boundary point
      int is = side==0 ? 1 : -1;
      if( bc[side]==freeBC )
      {
        // results from cgDoc/moving/codes/beam/beambc.maple

	// expansion for u when uxx=uxxx=0  u^(6)=0 u^(7)=0 
        //   u := x -> u0 + x*ux + x^4/(4!)*ux4 + x^5/(5!)*ux5;

	assert( numberOfGhost==2 || numberOfGhost==3 );

	real u0  = w1(ib     ,0);
	real up1 = w1(ib+  is,0);
	real up2 = w1(ib+2*is,0);
	real up3 = w1(ib+3*is,0);
	real up4 = w1(ib+4*is,0);
	  
        // N.B. : set boundary value too:
	w1(ib     ,0)=368./145*up1-318./145*up2+112./145*up3-17./145*up4;
	w1(ib-  is,0)=122./29*up1-136./29*up2+51./29*up3-8./29*up4;
	w1(ib-2*is,0)=208./29*up1-297./29*up2+144./29*up3-26./29*up4;
        if( numberOfGhost>=3 )
	  w1(ib-3*is,0)=455./29*up1-840./29*up2+518./29*up3-104./29*up4;
	
	// w1(ib-  is,0)=40./17.*u0-30./17.*up1+8./17.*up2-1./17.*up3;
	// w1(ib-2*is,0)=130./17.*u0-208./17.*up1+111./17.*up2-16./17.*up3;
	// if( numberOfGhost>=3 )
	//   w1(ib-3*is,0)=520./17.*u0-1053./17.*up1+648./17.*up2-98./17.*up3;
	
        // // w1(ib-  is,0)=2.*u0-up1;
        // w1(ib-  is,0)=3.*u0-3.*up1+up2;
        // w1(ib-2*is,0)=2.*u0-up2;
	
	// w=ux: 
        // expansion for w=ux when wx=0 wxx=0 w^(5)=0 w^(6)=0 
        //  w := x -> w0 + x^3/(3!)*wx3 + x^4/(4!)*wx4 + x^7/(7!)*wx7;
	real w0  = w1(ib     ,1);
	real wp1 = w1(ib+  is,1);
	real wp2 = w1(ib+2*is,1);
	real wp3 = w1(ib+3*is,1);
	real wp4 = w1(ib+4*is,1);

        // set boundary value too:
	w1(ib     ,1)=6336./4795*wp1-1944./4795*wp2+64./685*wp3-9./959*wp4;
	w1(ib-  is,1)=1898./959*wp1-1258./959*wp2+51./137*wp3-38./959*wp4;
	w1(ib-2*is,1)=7696./959*wp1-9423./959*wp2+432./137*wp3-338./959*wp4;
        if( numberOfGhost>=3 )
  	  w1(ib-3*is,1)=4095./137*wp1-5670./137*wp2+1946./137*wp3-234./137*wp4;

	// w1(ib-  is,1)=38./9.*w0-18./5.*wp1+2./5.*wp2-1./45.*wp3;
	// w1(ib-2*is,1)=338./9.*w0-208./5.*wp1+27./5.*wp2-16./45.*wp3;
	// if( numberOfGhost>=3 )
	//   w1(ib-3*is,1)=182.*w0-1053./5.*wp1+162./5.*wp2-14./5.*wp3;
	
	// // TEST: 
        // // w1(ib-  is,1)=wp1;
        // // w1(ib-2*is,1)=2.*w0-wp2;
        // w1(ib-  is,1)=2.*w0-wp1;
        // w1(ib-2*is,1)=2.*w0-wp2;

	// // Free BC: w_xx = 0    -> D_+^2 w_{-1} =0  IS THIS ACCURATE ENOUGH ??
	// //          w_xxx = 0   -> D_+^3 w_{-2} =0 
	// assert( numberOfGhost==2 );
	
	// int ig = ib - is; // 1st ghost point 
	// w1(ig,0) = 2.*w1(ig+is,0) -w1(ig+2*is,0);
	// w1(ig,1) = 3.*w1(ig+is,1) - 3.*w1(ig+2*is,1) + w1(ig+3*is,1);
	// ig = ib - 2*is;  // 2nd ghost point 
	// w1(ig,0) = 3.*w1(ig+is,0) - 3.*w1(ig+2*is,0) + w1(ig+3*is,0);
	// w1(ig,1) = 4.*w1(ig+is,1) -6.*w1(ig+2*is,1) + 4.*w1(ig+3*is,1) - w1(ig+4*is,1);  
      }
      else if( bc[side]==pinned )
      {
        //  Pinned: u=u_xx=0  -> u_xxxx=u_xxxxxx = 0  etc.
        //  u is an odd function 
        //  u_x is an even function 
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
   	  w1(ig,0)=2.*w1(ib,0) - w1(ib+g*is,0);  // u (or v) is odd
   	  w1(ig,1)=   w1(ib+g*is,1);             // u_x or v_x is even
	}

      }
      else if( bc[side]==slideBC )
      {
        //  Slide: u_x=u_xxx=0  -> all odd derivatives are zero
        //  u is an even function 
        //  u_x is an odd function 
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
   	  w1(ig,0)= w1(ib+g*is,0) ;               // u (or v) is even
   	  w1(ig,1)=2.*w1(ib,1) - w1(ib+g*is,1);   // u_x or v_x is odd
	}

      }
      else if( bc[side]==clamped )
      {
        //  Clamped: u=u_x=0   --> u_xxxx=0, u_xxxxx=0, ...

        // results from cgDoc/moving/codes/beam/beambc.maple

	// expansion for u when u=0, ux=0, uxxxx=0, uxxxxx=0, ...
        // u := x -> u0 + x^2/2*uxx + x^3/6*uxxx + x^6/(6!)*ux6 + x^7/(7!)*ux7 
        
	assert( numberOfGhost==2 || numberOfGhost==3 );

	real u0  = w1(ib     ,0);
	real up1 = w1(ib+  is,0);
	real up2 = w1(ib+2*is,0);
	real up3 = w1(ib+3*is,0);
	real up4 = w1(ib+4*is,0);
	  
        w1(ib     ,0)=0.;
	w1(ib-  is,0)=-385./174.*u0+122./29.*up1-34./29.*up2+17./87.*up3-1./58.*up4;
	w1(ib-2*is,0)=-1127./58.*u0+832./29.*up1-297./29.*up2+64./29.*up3-13./58.*up4;
	if( numberOfGhost>=3 )
    	  w1(ib-3*is,0)=-5271./58.*u0+4095./29.*up1-1890./29.*up2+518./29.*up3-117./58.*up4;
	  
	// w=ux: 
        //  expansion for w=ux when wxxx=0 wxxxx=0 w^(7)=0 w^(8)=0 
        // w := x -> w0 + x*wx + x^2/2*wxx + x^5/(5!)*wx5 + x^6/(6!)*wx6;

	real w0  = w1(ib     ,1);
	real wp1 = w1(ib+  is,1);
	real wp2 = w1(ib+2*is,1);
	real wp3 = w1(ib+3*is,1);
	real wp4 = w1(ib+4*is,1);

        w1(ib     ,1)=0.;
	w1(ib-  is,1)=98./29.*w0-122./29.*wp1+68./29.*wp2-17./29.*wp3+2./29.*wp4;
	w1(ib-2*is,1)=231./29.*w0-416./29.*wp1+297./29.*wp2-96./29.*wp3+13./29.*wp4;
	if( numberOfGhost>=3 )
  	  w1(ib-3*is,1)=574./29.*w0-1365./29.*wp1+1260./29.*wp2-518./29.*wp3+78./29.*wp4;


        // 4th-order filter: Obtain 2 ghost from
        //   u: 
        //        u_x = 0  
        //        u_xxxx = 0 
        //   w=u_x:
        //        w_xxx = 0 
        //        w_xxxx = 0 
        //
        // 6th-order filter: obtain 3 ghost from
        //   u:  (do not smooth boundary) 
        //       u_x = 0
        //       u_xxxx = 0
        //       u_xxxxx = 0 
        //   w=u_x: (do not smooth boundary) 
        //        w_xxx = 0 
        //        w_xxxx = 0 
        //        D_x^7 u = 0    Dz (D+D-)^3 

        // results from cgDoc/moving/codes/beam/beambc.maple
        // if( numberOfGhost==2 )
	// {
        //   // // Clamped: u:
        //   // // u(i-2) = 16*u(i+1)-3*u(i+2)-12*u(i)
        //   // // u(i-1) = 3*u(i+1)-1/2*u(i+2)-3/2*u(i)
	//   // w1(ib-2*is,0) = 16.*w1(ib+is,0)-3.*w1(ib+2*is,0)-12.*w1(ib,0);
	//   // w1(ib-  is,0) =  3.*w1(ib+is,0)-.5*w1(ib+2*is,0)-1.5*w1(ib,0);

        //   // // Clamped: u_x:
        //   // // u(i-2) = -8*u(i+1)+3*u(i+2)+6*u(i)
        //   // // u(i-1) = -3*u(i+1)+u(i+2)+3*u(i)
	//   // w1(ib-2*is,1) = -8.*w1(ib+is,1)+3.*w1(ib+2*is,1)+6.*w1(ib,1);
        //   // w1(ib-1*is,1) = -3.*w1(ib+is,1)+   w1(ib+2*is,1)+3.*w1(ib,1);

	//   OV_ABORT("finish me");
	// }
	// // for( int g=1; g<=numberOfGhost; g++ )
	// {
	//   int ig = ib - g*is; // ghost point 
   	//   w1(ig,0)=2.*w1(ib,0) - w1(ib+g*is,0);  // u (or v) is odd
   	//   w1(ig,1)=   w1(ib+g*is,1);             // u_x or v_x is even
	// }

      }
      else
      {
	// -- just extrapolate for now *FIX ME*
	for( int g=1; g<=numberOfGhost; g++ )
	{
	  int ig = ib - g*is; // ghost point 
	  if( orderOfExtrapolation==5 )
	    w1(ig,R2) = 5.*w1(ig+is,R2) -10.*w1(ig+2*is,R2) + 10.*w1(ig+3*is,R2) - 5.*w1(ig+4*is,R2) + w1(ig+5*is,R2);  
	  else if( orderOfExtrapolation==4 )
	    w1(ig,R2) = 4.*w1(ig+is,R2) -6.*w1(ig+2*is,R2) + 4.*w1(ig+3*is,R2) - w1(ig+4*is,R2);  
	  else if( orderOfExtrapolation==3 )
	    w1(ig,R2) = 3.*w1(ig+is,R2) - 3.*w1(ig+2*is,R2) + w1(ig+3*is,R2);
	  else if( orderOfExtrapolation==2 )
	    w1(ig,R2) = 2.*w1(ig+is,R2) -w1(ig+2*is,R2);
	  else
	  {
	    OV_ABORT("error: finish me");
	  }
	    
	}
      }
	
    }
  }
}



// ====================================================================================
/// \brief Smooth the Hermite solution with a fourth-order filter.
///   *** NOTE: THIS IS NOT REALLY WORKING YET****
/// \param t (input) : current time
/// \param w (input/output) : Hermite solution to smooth (u or v)
/// \param label (input) : label for debug output.
// ====================================================================================
void BeamModel::
smooth( const real t, RealArray & w, const aString & label )
{
  const bool & smoothSolution = dbase.get<bool>("smoothSolution");

  if( !smoothSolution )
    return;
  
  const int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  const int & smoothOrder     = dbase.get<int>("smoothOrder");

  // const int & current = dbase.get<int>("current"); 
  // std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  // std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF


  // add ghost points so we add apply filter up to boundary if needed
  int numberOfGhost = smoothOrder/2;    // 2 for 4th-order, 3 for 6th order filter
  int base =0, bound = numElem;
  RealArray w1(Range(base-numberOfGhost,bound+numberOfGhost),2);  // compute filtered solution here 

  // -- copy input into w1 
  for( int i=base; i<=bound; i++ )
  {
    w1(i,0)= w(2*i  );    //  u (or v)
    w1(i,1)= w(2*i+1);    //  u_x (or v_x)
  }
  
  const bool isPeriodic = bcLeft==periodic;
  
  const int orderOfExtrapolation=smoothOrder+1; 

  // we need at least this many elements to apply the smoother:
  assert( numElem >= orderOfExtrapolation );

  const real & dt = dbase.get<real>("dt"); 
  const real omega= dbase.get<real>("smoothOmega");  // parameter in smoother (=1 : kill plus minus mode)
  if( t < 3.*dt )
  {
    printF("--BM-- smooth %s, numberOfSmooths=%i (%ith order filter), omega=%9.3e isPeriodic=%i t=%8.2e.\n",
	   (const char*)label,numberOfSmooths,smoothOrder,omega,(int)isPeriodic,t );
	  
  }


//  omega *=dt ;  /// **TRY THIS **

  const int bc[2]={bcLeft,bcRight};  // 

  // I : smooth these points for u or v . Keep the boundary points fixed, except for
  //  periodic 
  //  slide 
  // int freeEnd = freeBC;
  int freeEnd = -10; // turn off smooth on the boundary pts for a free end

  const int i1a= (isPeriodic || bc[0]==freeEnd || bc[0]==slideBC ) ? base  : base+1;
  const int i1b= (isPeriodic || bc[1]==freeEnd || bc[1]==slideBC ) ? bound : bound-1;

  // J : smooth these points of u_x or v_x . Keep the boundary points fixed, except for 
  //  periodic
  //  freeBC
  //  pinned: u_xx=0 : smooth u_x on the boundary
  //  slide
  const int j1a= (isPeriodic || bc[0]==pinned || bc[0]==freeEnd || bc[0]==slideBC ) ? base  : base+1;
  const int j1b= (isPeriodic || bc[1]==pinned || bc[1]==freeEnd || bc[1]==slideBC  ) ? bound : bound-1;

  Range I(i1a,i1b), J(j1a,j1b);

  smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  for( int smooth=0; smooth<numberOfSmooths; smooth++ )
  {
    // smooth interior pts (and boundary pts sometimes): 

    if( smoothOrder==4 )
    {
      // 4th order filter: 
      w1(I,0)= w1(I,0) + (omega/16.)*(-w1(I-2,0) + 4.*w1(I-1,0) -6.*w1(I,0) + 4.*w1(I+1,0) -w1(I+2,0) );
      w1(J,1)= w1(J,1) + (omega/16.)*(-w1(J-2,1) + 4.*w1(J-1,1) -6.*w1(J,1) + 4.*w1(J+1,1) -w1(J+2,1) );
    }
    else if( smoothOrder==6 )
    {
      // 6th order filter: 
      // 1 4 6 4 1 
      // 1 5 10 10 5 1
      // 1 6 15 20 15 6 1 
      w1(I,0)= w1(I,0) + (omega/64.)*(w1(I-3,0) - 6.*w1(I-2,0) +15.*w1(I-1,0) -20.*w1(I,0)
				      + 15.*w1(I+1,0) -6.*w1(I+2,0) + w1(I+3,0) );
      w1(J,1)= w1(J,1) + (omega/64.)*(w1(J-3,1) - 6.*w1(J-2,1) +15.*w1(J-1,1) -20.*w1(J,1)
				      + 15.*w1(J+1,1) -6.*w1(J+2,1) + w1(J+3,1) );
    }
    else
    {
      printF("BeamModel::smooth:ERROR: not implemented for smoothOrder=%i.\n",smoothOrder);
      OV_ABORT("error");
    }
    
    smoothBoundaryConditions( w1, base, bound, numberOfGhost,orderOfExtrapolation );

  } // end smooths


  // copy smoothed solution back to w
  for( int i=0; i<=numElem; i++ )
  {
    w(2*i  ) = w1(i,0);
    w(2*i+1) = w1(i,1);
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


// ====================================================================================
/// \brief Does the beam on fluid on both sides?
// ====================================================================================
bool BeamModel::
hasFluidOnTwoSides() const
{
  return dbase.get<bool>("fluidOnTwoSides");
}




bool BeamModel::hasCorrectionConverged() const {

  //return true;
  return correctionHasConverged;
}



// =================================================================================================
/// \brief Set the relaxation factor when iterating to solve an coupled FSI problem
// =================================================================================================
void BeamModel::
setAddedMassRelaxation(double omega) 
{
  dbase.get<real>("addedMassRelaxationFactor") = omega;
  printF("\n  @@@@@@@@@@ BeamModel:: set addedMassRelaxationFactor=%g @@@@@@@@\n",
	 dbase.get<real>("addedMassRelaxationFactor"));
}

void BeamModel::setSubIterationConvergenceTolerance(double tol) 
{
  dbase.get<real>("subIterationConvergenceTolerance") = tol;
}

// =================================================================================================
/// \brief  Compute errors in the CURRENT solution (when the solution is known).
/// \param label (input) : label for output.
/// \param file (input) : write output to this file.
/// \param uvErr[2] (output) :  maximum errors
/// \param uvNorm[2] (output) : norm of the solution
// =================================================================================================
int BeamModel::
getErrors( const aString & label,
           FILE *file /* = stdout */,
           real *uvErr /* = NULL */, real *uvNorm /* = NULL */ )
{
  const int & current = dbase.get<int>("current"); 
  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

  return getErrors( time(current), u[current], v[current], a[current] ,label, file, uvErr, uvNorm );

}



// =================================================================================================
/// \brief  Compute errors in the solution (when the solution is known).
/// \param label (input) : label for output.
/// \param file (input) : write output to this file.
/// \param uvErr[2] (output) :  maximum errors
/// \param uvNorm[2] (output) : norm of the solution
// =================================================================================================
int BeamModel::
getErrors( const real t, const RealArray & u, const RealArray & v, const RealArray & a,
           const aString & label,
           FILE *file /* = stdout */,
           real *uvErr /* = NULL */, real *uvNorm /* = NULL */ )
{

  const real beamLength = L;
  
  RealArray ue, ve, ae;
  getExactSolution( t, ue, ve, ae );

  if( file !=NULL && debug & 2 )
    fPrintF(file,"-- BM -- %s: Errors at t=%9.3e:\n",(const char*)label,t);
  real uErr=0., ul2err=0., uNorm=0.;
  real vErr=0., vNorm;
  for( int i = 0; i <= numElem; ++i )
  {
    real xl = ( (real)i /numElem) *  beamLength;

    real we = ue(i*2);  // exact solution
    real erru = fabs( u(i*2) - we );
      
    uErr=max(uErr,erru);
    ul2err += SQR(erru);
    uNorm=uNorm+SQR(we);

    real errv = fabs( v(i*2) - ve(i*2) );
    vErr=max(vErr,errv);
    vNorm=vNorm+SQR(ve(i*2));

    if( debug & 2 )
      printF("-- BM -- t=%8.2e i=%3i u=%9.2e ue=%9.2e err=%9.2e, v=%9.2e ve=%9.2e err=%8.2e\n",
             t,i,u(2*i),we,erru, v(2*i),ve(2*i),errv);

  }
  ul2err=sqrt(ul2err/(numElem+1));
  uNorm=sqrt(uNorm/(numElem+1));
  vNorm=sqrt(vNorm/(numElem+1));
  if( file !=NULL )
  {
    fPrintF(file,"-- BM -- %s: Error t=%9.3e Ne=%i: uErr=(%8.2e,%8.2e)=(max,max/uNorm),"
	    " vErr=(%8.2e,%8.2e)=(max,max/vNorm) (steps=%i)\n",(const char*)label,t,numElem,
	    uErr,uErr/max(1.e-12,uNorm),
	    // ul2err,ul2err/max(1.e-12,uNorm),
	    vErr,vErr/(max(1.e-12,vNorm)),
	    numberOfTimeSteps);
  }
  
  if( uvErr!=NULL )
  {
    uvErr[0]=uErr;
    uvErr[1]=vErr;
  }
  if( uvNorm!=NULL )
  {
    uvNorm[0]=uNorm;
    uvNorm[1]=vNorm;
  }
  
  // -- THIS SHOULD BE MOVED : check file is called too many times --
  if( file !=NULL )
  {
    FILE *checkFile = dbase.get<FILE*>("checkFile");
    writeCheckFile( checkFile );
  }

  // const int myid=max(0,Communication_Manager::My_Process_Number);
  // if( checkFile!=NULL && myid==0 )
  // {
  //   const int numberOfComponentsToOutput=2;
  //   fPrintF(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput);
  //   fPrintF(checkFile,"%i %9.2e %10.3e  ",0,uErr,uNorm);
  //   fPrintF(checkFile,"%i %9.2e %10.3e  ",1,vErr,vNorm);
  //   fPrintF(checkFile,"\n");
  // }
  

  return 0;
}

//=================================================================================
/// \brief  Write information to the `check file' (used for regression tests)
//=================================================================================
int BeamModel::
writeCheckFile( FILE *file )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  if( file!=NULL && myid==0 )
  {
    const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
    const int & current = dbase.get<int>("current"); 

    RealArray & time = dbase.get<RealArray>("time");
    std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
    std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
    std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

    aString label=name;
    real uvErr[2], uvNorm[2];
    getErrors( time(current), u[current], v[current], a[current] ,label, NULL, uvErr, uvNorm );

    const int numberOfComponentsToOutput=2;
    fPrintF(file,"%9.2e %i  ",t,numberOfComponentsToOutput);
    fPrintF(file,"%i %9.2e %10.3e  ",0,uvErr[0],uvNorm[0]);
    fPrintF(file,"%i %9.2e %10.3e  ",1,uvErr[1],uvNorm[1]);
    fPrintF(file,"\n");
  }

  return 0;
}


void BeamModel::
printTimeStepInfo( FILE *file /*= stdout */ )
//=================================================================================
/// \brief  Print information about the current solution in a nicely formatted way
//  ** This is a virtual function **
//=================================================================================
{
  if( exactSolutionOption != "none" )
  {
    const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
    const int & current = dbase.get<int>("current"); 

    RealArray & time = dbase.get<RealArray>("time");
    std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
    std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
    std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

    aString label=name;
    getErrors( time(current), u[current], v[current], a[current] ,label, file );
  }
  
}

// ========================================================================================
/// \brief plot the beam solution and errors.
///
/// \param t (input) : time to plot
///
// ========================================================================================
int BeamModel::
plot( real t, GenericGraphicsInterface & gi, GraphicsParameters & psp , const aString & label )
{

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const int & current = dbase.get<int>("current"); 

  RealArray & time = dbase.get<RealArray>("time");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
  {
    printF("BeamModel::plot:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	   t,time(current),current);
    OV_ABORT("ERROR");
  }

  RealArray & uc = u[current];
  RealArray & vc = v[current];
  RealArray & ac = a[current];

  int numNodes=numElem+1;
  
  realArray x(1,numNodes);
  for( int i=0; i<=numElem; i++ ) 
    x(0,i)=((real)i /numElem) *  L;       // position along neutral axis 
      
  DataPointMapping line;
  line.setDataPoints(x,0,1);  // 0=position of coordinates, 1=domain dimension
  MappedGrid c(line);   // a grid
  c.update(MappedGrid::THEvertex | MappedGrid::THEmask);

  bool plotErrors=exactSolutionOption!="none";

  // number of components: 
  int nv=3 + 3;    // [u,v,a] + [ux,vx,ax] 
  if( plotErrors )
    nv+= 6;        // plot errors 
  
  Range all;
  realMappedGridFunction w(c,all,all,all,nv); // holds things to plot 
  w=0.;

  Range I=numNodes;
  
  Range J = Range(0,2*numElem,2);  // [0,2,4,...,2*numElem]
  w(I,0,0, 0)= uc(J);   // u at nodes
  w(I,0,0, 1)= vc(J);
  w(I,0,0, 2)= ac(J);
    
  w(I,0,0, 3)= uc(J+1);  // ux at nodes 
  w(I,0,0, 4)= vc(J+1);  // vx at nodes 
  w(I,0,0, 5)= ac(J+1);
  
  if( plotErrors )
  {
    RealArray ue, ve, ae;
    getExactSolution( t, ue, ve, ae );

    w(I,0,0, 6)= uc(J)-ue(J);   // u-error at nodes
    w(I,0,0, 7)= vc(J)-ve(J);
    w(I,0,0, 8)= ac(J)-ae(J);

    w(I,0,0, 9)= uc(J+1)-ue(J+1);   // ux-error at nodes
    w(I,0,0,10)= vc(J+1)-ve(J+1);
    w(I,0,0,11)= ac(J+1)-ae(J+1);
  }
  

  // ** TODO ** we could eval the Hermite interpolant on a finer grid **

  w.setName("w");
  w.setName("u",0);
  w.setName("v",1);
  w.setName("a",2);
  w.setName("ux",3);
  w.setName("vx",4);
  w.setName("ax",5);
  if( plotErrors )
  {
    w.setName("uErr",6);
    w.setName("vErr",7);
    w.setName("aErr",8);
    w.setName("uxErr",9);
    w.setName("vxErr",10);
    w.setName("axErr",11);
  }
  
  // bool colourLineContours=psp.colourLineContours;
  // real lineWidthSave=1.;
  // psp.get(GraphicsParameters::lineWidth,lineWidthSave);
  // psp.set(GraphicsParameters::lineWidth,2.);
  psp.set(GI_COLOUR_LINE_CONTOURS,true);
  
  psp.set(GI_TOP_LABEL,sPrintF("beam model t=%9.3e",t)); 

  PlotIt::contour(gi,w,psp);

  // reset
  // psp.set(GraphicsParameters::lineWidth,lineWidthSave);
  // psp.set(GI_COLOUR_LINE_CONTOURS,colourLineContours);



  return 0;
  
}


// =================================================================================================
/// \brief  Define the BeamModel parameters interactively.
// =================================================================================================
int BeamModel::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  real & tension = dbase.get<real>("tension"); // coefficient of w_xx
  real & K0 = dbase.get<real>("K0");           //  coefficient of -w
  real & Kt = dbase.get<real>("Kt");           //  coefficient of -w_t
  real & Kxxt = dbase.get<real>("Kxxt");       //  coefficient of w_{xxt} 
  real & ADxxt = dbase.get<real>("ADxxt");       //  coefficient of artificial dissipation

  real & cfl = dbase.get<real>("cfl");  

  bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  bool & fluidOnTwoSides = dbase.get<bool>("fluidOnTwoSides");
  int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");

  bool & smoothSolution = dbase.get<bool>("smoothSolution");
  int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  int & smoothOrder     = dbase.get<int>("smoothOrder");
  real & smoothOmega    = dbase.get<real>("smoothOmega");
  
  real & signForNormal = dbase.get<real>("signForNormal");  // flip sign of normal using this parameter

  // useSmallDeformationApproximation : adjust the beam surface acceleration and surface "internal force"
  //    assuming small deformations
  bool & useSmallDeformationApproximation = dbase.get<bool>("useSmallDeformationApproximation");


  bool & twilightZone = dbase.get<bool>("twilightZone");
  int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  int & degreeInTime = dbase.get<int>("degreeInTime");
  int & degreeInSpace = dbase.get<int>("degreeInSpace");
  real *trigFreq = dbase.get<real[4]>("trigFreq");

  real & displacementScaleFactorForPlotting =  dbase.get<real>("displacementScaleFactorForPlotting");
  aString & probeFileName = dbase.get<aString>("probeFileName");
  int & probeFileSaveFrequency = dbase.get<int>("probeFileSaveFrequency");
  real & probePosition =  dbase.get<real>("probePosition");
  
  GUIState gui;
  gui.setWindowTitle("Beam Model");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  bool & relaxForce = dbase.get<bool>("relaxForce");
  bool & relaxCorrectionSteps=dbase.get<bool>("relaxCorrectionSteps");
  real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  bool & useAitkenAcceleration = dbase.get<bool>("useAitkenAcceleration");

  bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");

  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    const int numColumns=2;

    dialog.setOptionMenuColumns(numColumns);
    aString bcOptions[] = { "pinned",
                            "clamped",
                            "slide",
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

    // aString exactOptions[] = { "none",
    // 			       "standing wave",
    // 			       "traveling wave FSI",
    // 			       "" };

    // GUIState::addPrefix(exactOptions,"Exact solution:",cmd,maxCommands);
    // dialog.addOptionMenu("Exact solution:",cmd,cmd,(exactSolutionOption=="none" ? 0 : 
    //                       exactSolutionOption=="standingWave" ? 1 : 2) );

    aString pbLabels[] = {"initial conditions...",
                          "exact solution...",
                          "show parameters",
                          "plot",
    			  "help",
    			  ""};
    int numEntries=5;
    int numRows=numEntries/numColumns;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 


    aString tbCommands[] = {"use exact solution",
                            "save profile file",
                            "save probe file",
                            "twilight-zone",
                            "use second order Newmark predictor",
                            "use new tridiagonal solver",
                            "use implicit predictor",
                            "fluid on two sides",
                            "use small deformation approximation",
                            "relax correction steps",
                            "relax force",
                            "use Aitken acceleration",
                            "smooth solution",
    			    ""};
    int tbState[15];
    tbState[0] = useExactSolution;
    tbState[1] = dbase.get<bool>("saveProfileFile");
    tbState[2] = dbase.get<bool>("saveProbeFile");
    tbState[3] = twilightZone;
    tbState[4] = useSecondOrderNewmarkPredictor;
    tbState[5] = useNewTridiagonalSolver;
    tbState[6] = useImplicitPredictor;
    tbState[7] = fluidOnTwoSides;
    tbState[8] = useSmallDeformationApproximation;
    tbState[9] = relaxCorrectionSteps;
    tbState[10] = relaxForce;
    tbState[11] = useAitkenAcceleration;
    tbState[12] = smoothSolution;
    
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "name:"; sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 
    textLabels[nt] = "number of elements:"; sPrintF(textStrings[nt], "%i",numElem);  nt++; 
    textLabels[nt] = "cfl:"; sPrintF(textStrings[nt], "%g",cfl);  nt++; 
    textLabels[nt] = "area moment of inertia:"; sPrintF(textStrings[nt], "%g",areaMomentOfInertia);  nt++; 
    textLabels[nt] = "elastic modulus:"; sPrintF(textStrings[nt], "%g",elasticModulus);  nt++; 
    textLabels[nt] = "tension:"; sPrintF(textStrings[nt], "%g",tension);  nt++; 
    textLabels[nt] = "K0:"; sPrintF(textStrings[nt], "%g",K0);  nt++; 
    textLabels[nt] = "Kt:"; sPrintF(textStrings[nt], "%g",Kt);  nt++; 
    textLabels[nt] = "Kxxt:"; sPrintF(textStrings[nt], "%g",Kxxt);  nt++; 
    textLabels[nt] = "ADxxt:"; sPrintF(textStrings[nt], "%g",ADxxt);  nt++; 
    textLabels[nt] = "density:"; sPrintF(textStrings[nt], "%g",density);  nt++; 
    textLabels[nt] = "thickness:"; sPrintF(textStrings[nt], "%g",thickness);  nt++; 
    textLabels[nt] = "length:"; sPrintF(textStrings[nt], "%g",L);  nt++; 
    textLabels[nt] = "pressure norm:"; sPrintF(textStrings[nt], "%g",pressureNorm);  nt++; 
    textLabels[nt] = "initial declination:"; sPrintF(textStrings[nt], "%g (degrees)",beamInitialAngle*180./Pi);  nt++; 
    textLabels[nt] = "position:"; sPrintF(textStrings[nt], "%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0);  nt++; 

    textLabels[nt] = "sign for normal:"; sPrintF(textStrings[nt], "%g (+1 or -1)",signForNormal);  nt++; 
    textLabels[nt] = "added mass relaxation:"; sPrintF(textStrings[nt], "%g",addedMassRelaxationFactor);  nt++; 
    textLabels[nt] = "added mass tol:"; sPrintF(textStrings[nt], "%g",subIterationConvergenceTolerance);  nt++; 

    textLabels[nt] = "order of Galerkin projection:";  sPrintF(textStrings[nt],"%i",orderOfGalerkinProjection);  nt++; 

    textLabels[nt] = "Newmark beta:"; sPrintF(textStrings[nt], "%g",newmarkBeta);  nt++; 
    textLabels[nt] = "Newmark gamma:"; sPrintF(textStrings[nt], "%g",newmarkGamma);  nt++; 

    textLabels[nt] = "degree in space:";  sPrintF(textStrings[nt],"%i",degreeInSpace);  nt++; 
    textLabels[nt] = "degree in time:";  sPrintF(textStrings[nt],"%i",degreeInTime);  nt++; 
    textLabels[nt] = "trig frequencies:";  sPrintF(textStrings[nt],"%g, %g, %g, %g (ft,fx,fy,fz)",
						 trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]); nt++;

    textLabels[nt] = "plotting scale factor:"; sPrintF(textStrings[nt], "%g",displacementScaleFactorForPlotting);  nt++; 

    textLabels[nt] = "probe file name:"; sPrintF(textStrings[nt], "%s",(const char*)probeFileName);  nt++; 
    textLabels[nt] = "probe file save frequency:"; sPrintF(textStrings[nt], "%i",probeFileSaveFrequency);  nt++; 

    textLabels[nt] = "number of smooths:"; sPrintF(textStrings[nt], "%i",numberOfSmooths);  nt++; 
    textLabels[nt] = "smooth order:"; sPrintF(textStrings[nt], "%i",smoothOrder);  nt++; 
    textLabels[nt] = "smooth omega:"; sPrintF(textStrings[nt], "%e",smoothOmega);  nt++; 
    textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",debug);  nt++; 
    textLabels[nt] = "probe position:"; sPrintF(textStrings[nt], "%g (in [0,1])",probePosition);  nt++; 


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
    else if( answer=="help" )
    {
      printF("The Beam model defines a generalized Euler-Bernoulli Beam.\n"
             "For more information see the documentation `beamModel.pdf'.\n");
    }
    else if( answer=="show parameters" )
    {
      writeParameterSummary();
    }
    else if( answer=="exact solution..." )
    {
      chooseExactSolution( cg,gi );
    }
    else if( answer=="initial conditions..." )
    {
      chooseInitialConditions( cg,gi );
    }
    else if( answer=="plot" )
    {
      GraphicsParameters psp;
      // aString label="BeamModel";
      plot( t,gi, psp , "BeamModel" );
    }
    
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //
    else if( dialog.getTextValue(answer,"name:","%s",name) ){} //
    else if( dialog.getTextValue(answer,"probe position:","%g",probePosition) )
    {
      printF("Setting the location of the probe to %g (normalized beam coordinates [0,1]\n",probePosition);
    }
    else if( dialog.getTextValue(answer,"probe file name:","%s",probeFileName) ){} //
    else if( dialog.getTextValue(answer,"probe file save frequency:","%i",probeFileSaveFrequency) ){} //

    else if( dialog.getTextValue(answer,"number of elements:","%i",numElem) ){} //
    else if( dialog.getTextValue(answer,"cfl:","%g",cfl) ){} //

    else if( dialog.getTextValue(answer,"area moment of inertia:","%g",areaMomentOfInertia) ){} //

    else if( dialog.getTextValue(answer,"elastic modulus:","%g",elasticModulus) ){} //
    else if( dialog.getTextValue(answer,"tension:","%g",tension) ){} //
    else if( dialog.getTextValue(answer,"K0:","%g",K0) ){} //
    else if( dialog.getTextValue(answer,"Kt:","%g",Kt) ){} //
    else if( dialog.getTextValue(answer,"Kxxt:","%g",Kxxt) ){} //
    else if( dialog.getTextValue(answer,"ADxxt:","%g",ADxxt) ){} //
    else if( dialog.getTextValue(answer,"density:","%g",density) ){} //
    else if( dialog.getTextValue(answer,"thickness:","%g",thickness) ){} //
    else if( dialog.getTextValue(answer,"length:","%g",L) ){} //
    else if( dialog.getTextValue(answer,"plotting scale factor:","%g",displacementScaleFactorForPlotting) ){} //
    else if( dialog.getTextValue(answer,"pressure norm:","%g",pressureNorm) ){} //
    else if( dialog.getTextValue(answer,"Newmark beta:","%g",newmarkBeta) ){} //
    else if( dialog.getTextValue(answer,"Newmark gamma:","%g",newmarkGamma) ){} //
    else if( dialog.getTextValue(answer,"sign for normal:","%g",signForNormal) )
    {
      if( signForNormal>0. )
      {
	signForNormal=1.;
	printF("Setting signForNormal=%g (right-handed coordinate system: tangent X normal = +zHat\n",signForNormal);
      }
      else
      {
	signForNormal=-1.;
	printF("Setting signForNormal=%g (left-handed coordinate system: tangent X normal = -zHat\n",signForNormal);
      }
      setDeclination(beamInitialAngle);  // recompute normal etc.
    }
    else if( dialog.getTextValue(answer,"order of Galerkin projection:","%g",orderOfGalerkinProjection) )
    {
      if( orderOfGalerkinProjection!=2 && orderOfGalerkinProjection!=4 )
      {
	printF("--BM-- Error: orderOfGalerkinProjection=%i is invalid, must be 2 or 4. \n",orderOfGalerkinProjection);
      }
      
      printF("Setting the order of accuracy of the Galerkin projection =%i (e.g. for the force integral)\n",
             orderOfGalerkinProjection);
      
    }

    else if( dialog.getTextValue(answer,"added mass relaxation:","%g",addedMassRelaxationFactor) )
    {
      printF("The relaxation parameter used in the fixed point iteration\n"
             " used to alleviate the added mass effect\n");
    }

    else if( dialog.getTextValue(answer,"added mass tol:","%g",subIterationConvergenceTolerance) )
    {
      printF("The (relative) convergence tolerance for the fixed point iteration is %9.3e\n",
	     subIterationConvergenceTolerance);
    }

    else if( dialog.getTextValue(answer,"initial declination:","%g",beamInitialAngle) )
    {  
      setDeclination(beamInitialAngle*Pi/180.);
      printF("INFO: The beam will be inclined %8.4f degrees from the left end\n",beamInitialAngle*180./Pi);
      dialog.setTextLabel("initial declination:",sPrintF(buff,"%g, (degrees)",beamInitialAngle*180./Pi));
    } 

    else if( dialog.getTextValue(answer,"number of smooths:","%i",numberOfSmooths) ){} //

    else if( dialog.getTextValue(answer,"smooth order:","%i",smoothOrder) ){} // 
    else if( dialog.getTextValue(answer,"smooth omega:","%e",smoothOmega) ){} // 

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
                bcOption=="slide"    ? slideBC :
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
    else if( dialog.getToggleValue(answer,"use small deformation approximation",useSmallDeformationApproximation) )
    {
      printF("useSmallDeformationApproximation=true : adjust the beam surface acceleration and"
	     " surface 'internal force' assuming small deformations\n");
    }
    else if( dialog.getToggleValue(answer,"save profile file",dbase.get<bool>("saveProfileFile")) ){} // 

    else if( dialog.getToggleValue(answer,"save probe file",dbase.get<bool>("saveProbeFile")) )
    {
      if( dbase.get<bool>("saveProbeFile") )
      {
        FILE *& probeFile = dbase.get<FILE*>("probeFile");

        // 	aString tipFileName = sPrintF(buff,"%sTip.text",(const char*)name);

	probeFile = fopen((const char*)probeFileName,"w");
	assert( probeFile!=NULL );

	printF("BeamModel: tip position info will be saved to file '%s'\n",(const char*)probeFileName);

        // Probe file header info:
	// Get the current date
	time_t *tp= new time_t;
	time(tp);
	// tm *ptm=localtime(tp);
	const char *dateString = ctime(tp);

        const real ss = probePosition*L;
	const real xp0 = beamX0 + ss*initialBeamTangent[0];
	const real yp0 = beamY0 + ss*initialBeamTangent[1];

        fPrintF(probeFile,"%% Probe file written from the BeamModel class: %s"
                "%% Probe location = (%12.5e,%12.5e)  (normalized coordinates: %12.5e in [0,1])\n"
	      	"%%       t       displacement       velocity      acceleration\n",xp0,yp0,probePosition,dateString);
	
	delete tp;
      }
      
    }
    else if( dialog.getToggleValue(answer,"fluid on two sides",fluidOnTwoSides) ){}//
    else if( dialog.getToggleValue(answer,"twilight-zone",twilightZone) ){}//
    else if( dialog.getToggleValue(answer,"use implicit predictor",useImplicitPredictor) ){}//
    else if( dialog.getToggleValue(answer,"relax force",relaxForce) )
    {
      if( relaxForce )
	printF("--BM-- relax the force in the added mass iteration\n");
      else
	printF("--BM-- relax the acceleration in the added mass iteration\n");
    }
    
    else if( dialog.getToggleValue(answer,"relax correction steps",relaxCorrectionSteps) )
    {
      printF(" The BeamModel correction steps can be relaxed. This may be necessary for 'light beams',\n"
             " with FSI simulations using the traditional partitioned schemes.\n"
	);
    }
    else if( dialog.getToggleValue(answer,"use Aitken acceleration",useAitkenAcceleration) )
    {
      if( useAitkenAcceleration )
	printF(" Use Aitken acceleration to accelerate the correction sub-iterations used for light body FSI\n");
      else
	printF(" Do NOT use Aitken acceleration to accelerate the correction sub-iterations used for light body FSI\n");
    }

    else if( dialog.getToggleValue(answer,"smooth solution",smoothSolution) ){}//

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
    // else if( len=answer.matches("Exact solution:") )
    // {
    //   aString option = answer(len,answer.length()-1);
    //   if( option=="none" )
    // 	exactSolutionOption="none";
    //   else if( option=="standing wave" )
    //     exactSolutionOption="standingWave";
    //   else if( option=="traveling wave FSI" )
    //     exactSolutionOption="travelingWaveFSI";
    //   else
    //   {
    // 	printF("ERROR: unknown exact solution=[%s]\n",(const char*)option);
    // 	gi.stopReadingCommandFile();
    //   }
    //   printF("Setting exactSolutionOption=[%s]\n",(const char*)exactSolutionOption);
      
    // }
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
setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) const
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

