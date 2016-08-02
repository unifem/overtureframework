//                                   -*- c++ -*-
// #define BOUNDS_CHECK
#include "BeamModel.h"
#include "display.h"
#include "TravelingWaveFsi.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "ParallelUtility.h"
#include "LineMapping.h"
#include "MappedGridOperators.h"
#include "TridiagonalSolver.h"

#include <sstream>


 
#define  FOR_3(i1,i2,i3,I1,I2,I3)					\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();	\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();	\
  for( i3=I3Base; i3<=I3Bound; i3++ )					\
    for( i2=I2Base; i2<=I2Bound; i2++ )					\
      for( i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)					\
  int I1Base,I2Base,I3Base;						\
  int I1Bound,I2Bound,I3Bound;						\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();	\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();	\
  for( i3=I3Base; i3<=I3Bound; i3++ )					\
    for( i2=I2Base; i2<=I2Bound; i2++ )					\
      for( i1=I1Base; i1<=I1Bound; i1++ )

// // Forward declarations of utility matrix functions on A++ arrays
// //
// RealArray 
// mult( const RealArray & a, const RealArray & b );
// 
// RealArray 
// trans( const RealArray &a );
// 
// RealArray 
// solve( const RealArray & a, const RealArray & b );

// Forward declaration of printArray() function (for debugging)
// void 
// printArray(const doubleSerialArray & u,  
//            int i1a, int i1b,   
//            int i2a, int i2b,   
//            int i3a, int i3b,   
//            int i4a, int i4b,   
//            int i5a, int i5b,   
//            int i6a, int i6b) ;


// --- assign static class variables --
//real BeamModel::exactSolutionScaleFactorFSI=.00001;  // scale factor for the exact FSI solution  //Longfei: seems unused
//int BeamModel::debug=0;  // Not static now,  put debug in dbase to  control each beam object.
int BeamModel::globalBeamCounter=0; // keeps track of number of beams that have been created


// ======================================================================================================
/// \brief Constructor.
//
// ======================================================================================================
BeamModel::BeamModel() 
{
  cout << "Constructing BeamModel\n";
  
  beamType = "genericBeamModel";

  globalBeamCounter++;
  
  dbase.put<int>("debug")=1; 
  dbase.put<int>("domainDimension")=1; 
  dbase.put<int>("rangeDimension")=2;  
  dbase.put<int>("beamID")=globalBeamCounter; //  a unique ID 
  dbase.put<aString>("name")="none";


  // parameters can be changed in BeamModel::update()
  dbase.put<real>("density")=1.;
  dbase.put<real>("length") = 1.;  // total beam length: L
  dbase.put<real>("thickness")=.1;
  dbase.put<real>("breadth")=1.;   // if 2d problem, this is the default value.
  dbase.put<real>("areaMomentOfInertia") = .1;
  dbase.put<real>("elasticModulus") = 1.;
  dbase.put<int>("numElem") = 11;
  dbase.put<real>("pressureNorm") = 1.; // 1000.0;  // scale pressure forces by this factor  
  dbase.put<real>("tension")=0.;  // T : coefficient of w_xx
  dbase.put<real>("K0")=0.;       //  coefficient of -w
  dbase.put<real>("Kt")=0.;       //  coefficient of -w_t
  dbase.put<real>("Kxxt")=0.;     //  coefficient of w_{xxt} 
  dbase.put<real>("ADxxt")=0.;    //  artificial dissipation coefficient
  dbase.put<real>("cfl")=10.;  // scale explicit dt by this cfl number (scheme is implicit)
  dbase.put<real[3]>("beamXYZ");  //[beamX0,beamY0,beamZ0]
  real *beamXYZ = dbase.get<real[3]>("beamXYZ"); 
  for( int i=0; i<3; i++ ){ beamXYZ[i]=0.; } //initialize to be [0,0,0]
  dbase.put<real>("beamInitialAngle") = 0.0;  // angle of undeformed beam 
  dbase.put<real>("newmarkBeta") = 0.25;
  dbase.put<real>("newmarkGamma") = 0.5;
  dbase.put<BoundaryCondition[2]>("boundaryConditions");
  dbase.get<BoundaryCondition[2]>("boundaryConditions")[0]=pinned; //initialize bcLeft
  dbase.get<BoundaryCondition[2]>("boundaryConditions")[1]=pinned; //initialize bcRight
  dbase.put<aString>("initialConditionOption")="none";
  dbase.put<aString>("exactSolutionOption")="none";
  //dbase.put<bool>("useNewTridiagonalSolver")=true;  //Longfei 20160219: removed. Only new triSolver is used. No longer need this flag
  dbase.put<int>("orderOfGalerkinProjection")=2; // order of accuracy of the Galerkin projection (force and velocity)
  
  //Longfei 20160303: added to control the FDBeamModel to use same order or same stencil size for all difference operators.
  // if true, all the derivatives ux,uxx,uxxx,uxxxx are approximated using 5 point stencils. 
  dbase.put<bool>("useSameStencilSize") = true; 


  
  // parameters will be updated in BeamModel::initialize()
  dbase.put<bool>("initialized")=false;
  dbase.put<real>("massPerUnitLength")=-999.99; 
  dbase.put<real>("EI") = -999.99;  
  dbase.put<real>("elementLength") = -999.99;  // was: le
  dbase.put<real>("buoyantMassPerUnitLength")=-999.99;
  dbase.put<real>("buoyantMass")=-999.99;
  dbase.put<real>("totalMass")=-999.99;
  dbase.put<real>("totalInertia")=-999.99;
  dbase.put<RealArray>("dtilde");
  dbase.put<RealArray>("vtilde");
  dbase.put<bool>("useExactSolution")=false;
  dbase.put<bool>("fluidOnTwoSides")=true; // The beam has fluid on both sides (for projecting the velocity)


  // parameters will be modeified in derived classes
  dbase.put<int>("numberOfGhostPoints")= 0; // number of ghost points on each side, a beam is assumed to have 2 sides 
  dbase.put<int>("numberOfMotionDirections")=0; // number of directions beam allowed to move: could be 1,2,3. Only 1 for FEMBeamModel 
  dbase.put<bool>("isCubicHermiteFEM")=false; // this flag indicates the data structure of solutions; Hermite FEM solves u and ux at the same time


  
  // other parameters
  dbase.put<int>("numberOfTimeSteps") = 1;
  dbase.put<bool>("hasAcceleration") = false;
  dbase.put<bool>("correctionHasConverged") = false;
  dbase.put<int>("numCorrectorIterations") = 0;
  dbase.put<real>("initialResidual")=0.;
  dbase.put<real>("projectedBodyForce") = 0.0;
  dbase.put<bool>("allowsFreeMotion") = false;
  dbase.put<MappedGrid>("beamGrid"); 


  // --- variables for time stepping ---
  int numberOfTimeLevels  = 3;
  dbase.put<int>("numberOfTimeLevels")=numberOfTimeLevels;  // total number of time levels we store
  dbase.put<real>("dt")=-1.;               // time step
  dbase.put<int>("current")=0;             // current time level
  dbase.put<RealArray>("time")=RealArray(numberOfTimeLevels);
  dbase.get<RealArray>("time")= 0.;  //initialize time to be 0
  dbase.put<std::vector<RealArray> >("u"); // displacement 
  dbase.put<std::vector<RealArray> >("v"); // velocity
  dbase.put<std::vector<RealArray> >("a"); // acceleration
  dbase.put<std::vector<RealArray> >("f"); // force
  // dbase.put<bool>("useImplicitPredictor")=true; //Longfei 20160205: removed. Handled via timeSteppingMethod choice.
  dbase.put<bool>("relaxForce")=true;
  dbase.put<RealArray>("aOld");
  dbase.put<RealArray>("fOld");
  dbase.put<RealArray>("fOlder");
  if( !dbase.has_key("useSecondOrderNewmarkPredictor") ) dbase.put<bool>("useSecondOrderNewmarkPredictor")=true; //Longfei: tb for this is removed. Handled via timeSteppingMethod choice.
  // The variable refactor is set to true when the implicit system chenges (e.g. when dt changes)
  if( !dbase.has_key("refactor") ) dbase.put<bool>("refactor")=true;
  //Longfei 20160131: new time stepping  parameters added
  dbase.put<TimeSteppingMethod>("predictorMethod")=newmark2Implicit; // default ts: 2nd order implicit newmark predictor
  dbase.put<TimeSteppingMethod>("correctorMethod")=newmarkCorrector; // default newmarkCorrector
 



  // Longfei 20160321: element matrices for FEMBeamModel
  // ================================================================================
  // These matrices are put here since they are needed by both derived classes when 
  // project values on the beam surface to the beam center/neutral line. 
  // modify these matrices in BeamModel::initialize() to change FEM base functions.
  // Currently, cubic Hermite polynomial are used, and the degrees of freedom are f_i and f.x_i
  // ================================================================================
  dbase.put<RealArray*>("elementM")=new RealArray;     // (phi_i,phi_j) 
  dbase.put<RealArray*>("elementT")=new RealArray;  // (phi.x_i,phi.x_j)
  dbase.put<RealArray*>("elementK")=new RealArray;    // (phi.xx_i,phi.xx_j)
  dbase.put<RealArray*>("elementB")= NULL; // holds "damping element matrix" for FEMBeamModel


  // Longfei 20160122:
  // these parameters needs to be modified for 3D problems. Will be back...
  dbase.put<real[2]>("initialBeamTangent");
  dbase.put<real[2]>("initialBeamNormal");
  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  initialBeamTangent[0]=1.0;      // -- beam is by default horizontal --
  initialBeamTangent[1] = 0.0;
  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  initialBeamNormal[0]=0.0;      // -- beam is by default horizontal --
  initialBeamNormal[1] = 1.0;
  dbase.put<real[2]>("bodyForce");
  real * bodyForce =  dbase.get<real[2]>("bodyForce");
  bodyForce[0] = bodyForce[1] = 0.0;
  dbase.put<real>("signForNormal") = 1.;  // flip sign of normal using this parameter


  // Longfei 20160208: new names for the tridiangonal solvers
  // implicitNewmarkSolver is initialized in each derived class
  // massMatrixSolvers are initialized in base class
  
  // implicitNewmarkSolver solves (M+alpha*K+alphaB*B) u = f
  // a new solver gets created when first called
  dbase.put<TridiagonalSolver*>("implicitNewmarkSolver")=NULL; 

  // massMatrixSolver solves M u = f with bcFixups to the matrix
  dbase.put<TridiagonalSolver*>("massMatrixSolver")=NULL;

  // massMatrixSolverNoBC solves M u = f without bcFixups to the matrix
  dbase.put<TridiagonalSolver*>("massMatrixSolverNoBC")=NULL;
  
  //Longfei 20160331: no need for this anymore
  //dbase.put<bool>("rhsSolverAddExternalForcing")=false;


  


  // save probe file results every this many time-steps:
  dbase.put<int>("probeFileSaveFrequency")=5;
  dbase.put<real>("probePosition")=1.; // probe position in [0,1], 1=end point at x=L


  // Set relaxCorrectionSteps to true for iterating for added mass effects
  dbase.put<bool>("relaxCorrectionSteps")=false;
  // The relaxation parameter used in the fixed point iteration
  // used to alleviate the added mass effect
  dbase.put<real>("addedMassRelaxationFactor")=1.0; 

  // The (relative) convergence tolerance for the fixed point iteration
  // tol: convergence tolerance (default is 1.0e-3)
  dbase.put<real>("subIterationConvergenceTolerance")=1.0e-3;
  dbase.put<real>("subIterationAbsoluteTolerance")=1.e-8;
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

  // --- twilight-zone variables ---
  if( !dbase.has_key("exactPointer") ) dbase.put<OGFunction*>("exactPointer")=NULL;
  if( !dbase.has_key("degreeInTime") ) dbase.put<int>("degreeInTime")=2;
  if( !dbase.has_key("degreeInSpace") ) dbase.put<int>("degreeInSpace")=2;

  if( !dbase.has_key("twilightZone") ) dbase.put<bool>("twilightZone")=false;
  if( !dbase.has_key("twilightZoneOption") ) dbase.put<int>("twilightZoneOption")=0;

  // Frequencies for trig TZ: 
  if( !dbase.has_key("trigFreq") ) dbase.put<real[4]>("trigFreq");   // ft, fx, fy, [fz]
  real *trigFreq = dbase.get<real[4]>("trigFreq");
  for( int i=0; i<4; i++ ){ trigFreq[i]=2.;  }

  dbase.put<real>("standingWaveTimeOffset")=0.; // time offset for standing wave solution

  //output parameters
  if( !dbase.has_key("saveProfileFile") ) 
    dbase.put<bool>("saveProfileFile")=false;

  if( !dbase.has_key("saveProbeFile") ) 
    dbase.put<bool>("saveProbeFile")=false;

  // File to which the probe data (e.g. tip displacement, velocity etc.) is written
  dbase.put<FILE*>("probeFile")=NULL;
  dbase.put<aString>("probeFileName")="beamProbeFile.text";
  
  // check file:
  FILE *& checkFile = dbase.put<FILE*>("checkFile");
  checkFile = fopen("BeamModel.check","w" );   // Here is the check file for regression tests


  // useSmallDeformationApproximation : adjust the beam surface acceleration and surface "internal force"
  //    assuming small deformations
  dbase.put<bool>("useSmallDeformationApproximation")=true;


  



  //=========================================================
  // -- free motion parameters ---
  centerOfMass[0] = 0.0;
  centerOfMass[1] = 0.0;
  angle = 0.0;   // angle of beam for free motion




  // projectedBodyForce = 0.0;

  for (int k = 0; k < 2; ++k) {

    normal[k] = initialBeamNormal[k];
    tangent[k] = initialBeamTangent[k];
  }

  penalty = 1e2;






  // { // wdh: replaces 'what' factor 
  //   dbase.put<real>("exactSolutionScaleFactorFSI");
  //   dbase.get<real>("exactSolutionScaleFactorFSI")=0.00001; // scale FSI solution so linearized approximation is valid 
  // }
  

  



 




  //---------------------------------  
  // removed stuff
  // ------------------------------- 

  // newmarkBeta = 0.5;
  // newmarkGamma = 1.;

  // numberOfTimeSteps = 1;


  // hasAcceleration = false;

  //bcLeft = bcRight = clamped;

  //  added_mass_relaxation = 1.0;

  //  numCorrectorIterations = 0;

  // convergenceTolerance = 1e-3;

  //  allowsFreeMotion = false;

  //  bodyForce[0] = bodyForce[1] = 0.0;
  

  // -- beam is by default horizontal --

  // initialBeamTangent[0] = 1.0;
  // initialBeamTangent[1] = 0.0;

  // initialBeamNormal[0] = 0.0;
  // initialBeamNormal[1] = 1.0;

  // leftCantileverMoment = 0.0; //Longfei 20160121 moved to FEMBeamModel.C

  // Longfei 20160118: moved to FEMBeamModel constructor
  // RealArray & elementB = dbase.put<RealArray>("elementB");  // holds "damping element matrix"
  // elementB.redim(4,4);


  // useExactSolution=false;


  
}

// ======================================================================================================
/// \brief Destructor.
// ======================================================================================================
BeamModel::~BeamModel() 
{

  printF("-- BM%i -- destruct  %s\n",getBeamID(),beamType.c_str());

  //delete element matrices
  delete dbase.get<RealArray*>("elementM");
  delete dbase.get<RealArray*>("elementT");
  delete dbase.get<RealArray*>("elementK");

  if (dbase.get<RealArray*>("elementB")!=NULL)
    delete dbase.get<RealArray*>("elementB");
  

  //delete TridiagonalSolver
  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>("implicitNewmarkSolver");
  if( pTri!=NULL )
    delete pTri;

  pTri = dbase.get<TridiagonalSolver*>("massMatrixSolver");
  if( pTri!=NULL )
    delete pTri;

  pTri = dbase.get<TridiagonalSolver*>("massMatrixSolverNoBC");
  if( pTri!=NULL )
    delete pTri; 


  
  if( dbase.get<FILE*>("checkFile")!=NULL )
    fclose(dbase.get<FILE*>("checkFile"));

  if( dbase.get<bool>("saveProbeFile") &&  dbase.get<FILE*>("probeFile")!=NULL )
    fclose(dbase.get<FILE*>("probeFile"));

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
      ::display(x0,"-- BM -- addToElementIntegral: x0","%9.2e ");
      ::display(fg,"-- BM -- addToElementIntegral: fg","%9.2e ");
    }
  

  // printF("--BM-- addToElementIntegral : tf=%9.3e (p1,p2)=(%8.2e,%8.2e)\n",tf,p1,p2);
  // Longfei 20160121: new way of handling parameters
  const real & beamLength=dbase.get<real>("length");

  //real beamLength=L;
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
	printF("-- BM%i -- addForce: x0=[%8.2e,%8.2e] f1=%8.2e f1x=%8.2e, x0=[%8.2e,%8.2e] f2=%8.2e f2x=%8.2e\n",
	       getBeamID(),x0(i1,i2,i3,0),x0(i1,i2,i3,1),  fg(iv[axis]), f1x, 
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
      ::display(fe,sPrintF("-- BM -- addToElementIntegral:END: : fe at t=%9.3e",tf),"%8.2e ");
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

  // Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  const real & L = dbase.get<real>("length");
  const real & pressureNorm = dbase.get<real>("pressureNorm");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
  real x0_1=x1[0], y0_1=x1[1], p1=f1, p1x=f1x;
  real x0_2=x2[0], y0_2=x2[1], p2=f2, p2x=f2x;

  const int & current = dbase.get<int>("current"); 
  RealArray & time = dbase.get<RealArray>("time");
  if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
    {
      printF("-- BM%i -- BeamModel::addToElementIntegral:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),tf,time(current),current);
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
	printF("-- BM%i -- addToElementIntegral: x1=[%g,%g] x2=[_%g,%g] pa=%g, pax=%g, pb=%g, pbx=%g a=%g b=%g f1=%g f1x=%g f2=%g f2x=%g\n",
	       getBeamID(),x0_1,y0_1, x0_2,y0_2,pa,pax,pb,pbx,a,b,f1,f1x,f2,f2x );

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

  fPrintF(file,"---------------------------------------------------------------------\n");
  fPrintF(file,"                        Beam %i  \n",getBeamID());
  fPrintF(file,"---------------------------------------------------------------------\n");
  fPrintF(file,"Euler-Bernoulli beam: Type=%s\n",(const char*)beamType);
  fPrintF(file,"(density*thickness*b)*w_tt = -K0 w + T w_xx - EI w_xxxx -Kt w_t + Kxxt w_xxt \n");
  fPrintF(file,"---------------------------------------------------------------------\n");

  //Longfei 20160126: new way to print parameters
  displayDBase(file);
  
  // old way
  // const real & T = dbase.get<real>("tension");
  // const bool & relaxForce = dbase.get<bool>("relaxForce");
  // const bool & relaxCorrectionSteps=dbase.get<bool>("relaxCorrectionSteps");
  // const real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  // const real & subIterationAbsoluteTolerance = dbase.get<real>("subIterationAbsoluteTolerance");
  // const real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  // const bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  // const bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor");
  // const bool & useSmallDeformationApproximation = dbase.get<bool>("useSmallDeformationApproximation");
  // const bool & useAitkenAcceleration = dbase.get<bool>("useAitkenAcceleration");
  // const bool & smoothSolution = dbase.get<bool>("smoothSolution");
  // const int & numberOfSmooths = dbase.get<int>("numberOfSmooths");
  // const int & smoothOrder     = dbase.get<int>("smoothOrder");
  // const real & smoothOmega = dbase.get<real>("smoothOmega");
  // const real & elasticModulus = dbase.get<real>("elasticModulus");
  // const real & areaMomentOfInertia = dbase.get<real>("areaMomentOfInertia");
  // const real & density = dbase.get<real>("density");
  // const real & L = dbase.get<real>("length");  
  // const real & thickness = dbase.get<real>("thickness");
  // const real & breadth = dbase.get<real>("breadth");
  // const int & numElem = dbase.get<int>("numElem");
  // const real & pressureNorm = dbase.get<real>("pressureNorm");
  // const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  // const real & beamX0 = beamXYZ[0];
  // const real & beamY0 = beamXYZ[1];
  // const real & beamZ0 = beamXYZ[2];
  // const real & beamInitialAngle = dbase.get<real>("beamInitialAngle");
  // const real & newmarkBeta = dbase.get<real>("newmarkBeta");
  // const real & newmarkGamma = dbase.get<real>("newmarkGamma");
  // const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  // const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  // const aString & initialConditionOption = dbase.get<aString>("initialConditionOption");
  // const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  // const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  // const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");

  

  // fPrintF(file," E=%9.3e, I=%9.3e, T=%8.2e, K0=%8.2e, Kt=%8.2e Kxxt=%8.2e  ADxxt=%8.2e \n"
  //              " density=%9.3e, length=%9.3e, thickness=%9.3e, b=breadth=%9.3e, initial-angle=%7.3f (degrees) \n"
  //              " numElem=%i, allowsFreeMotion=%i, initial left end=(%12.8e,%12.8e,%12.8e)\n"
  //              " Newmark time-stepping, beta=%g, gamma=%g, cfl=%g,\n"
  //              "     useSecondOrderNewmarkPredictor=%i, useImplicitPredictor=%i,\n"
  //              " pressureNormalization = %8.2e (scale pressure forces by this factor)\n"
  //              " useSmallDeformationApproximation = %i (adjust surface accelerations assuming small deformations)\n"
  //              " relaxCorrectionSteps=%i, relaxForce=%i, use-Aitken-acceleration=%i\n"
  //              " relaxation factor=%g, sub-iteration tol=%8.2e, absolute-tol=%8.2e\n"
  //              " smooth solution=%i, number of smooths=%i (%ith-order filter), omega=%5.3f\n"
  //              " fluidOnTwoSides=%i, orderOfGalerkinProjection=%i, useNewTridiagonalSolver=%i\n"
  // 	  , elasticModulus,areaMomentOfInertia,T,dbase.get<real>("K0"),dbase.get<real>("Kt"),dbase.get<real>("Kxxt"),
  //         dbase.get<real>("ADxxt"),
  //         density,L,thickness,breadth,beamInitialAngle*180./Pi,
  // 	  numElem,(int)allowsFreeMotion,beamX0,beamY0,beamZ0,
  //         newmarkBeta,newmarkGamma,dbase.get<real>("cfl"),
  // 	  (int)dbase.get<bool>("useSecondOrderNewmarkPredictor"),
  //         (int)useImplicitPredictor,
  // 	  pressureNorm,(int)useSmallDeformationApproximation,
  //         (int)relaxCorrectionSteps,(int)relaxForce,(int)useAitkenAcceleration,
  //         addedMassRelaxationFactor,subIterationConvergenceTolerance,
  //         subIterationAbsoluteTolerance,
  //         (int)smoothSolution,numberOfSmooths,smoothOrder,smoothOmega,
  //         (int)dbase.get<bool>("fluidOnTwoSides"), 
  //         dbase.get<int>("orderOfGalerkinProjection"),
  //         (int)useNewTridiagonalSolver);

  // aString bcName;
  // for( int side=0; side<=1; side++ )
  // {
  //   // BoundaryCondition bc = side==0 ? bcLeft : bcRight;
  //   //Longfei 20160122: new way
  //  const  BoundaryCondition bc = boundaryConditions[side];
  //   bcName = (bc==pinned ? "pinned" : 
  //             bc==clamped ? "clamped" :
  //             bc==slideBC ? "slide" :
  //             bc==periodic ? "periodic" : 
  //             bc==freeBC ? "free" : "unknown");
    
  //   fPrintF(file," %s=%s, ",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcName);
  // }
  // fPrintF(file,"\n");

  // const bool & twilightZone = dbase.get<bool>("twilightZone");
  // const int & twilightZoneOption = dbase.get<int>("twilightZoneOption");
  // const int & degreeInTime = dbase.get<int>("degreeInTime");
  // const int & degreeInSpace = dbase.get<int>("degreeInSpace");
  // const real & signForNormal = dbase.get<real>("signForNormal");
  // real *trigFreq = dbase.get<real[4]>("trigFreq");
  // fPrintF(file," twilightZone=%s, option=%s. Poly: degreeT=%i, degreeX=%i, Trig: ft=%g, fx=%g\n",(twilightZone ? "on" : "off"),
  //         (twilightZoneOption==0 ? "polynomial" : "trigonometric"),
  // 	  degreeInTime,degreeInSpace,trigFreq[0],trigFreq[1] );
  // fPrintF(file," Exact solution option: %s\n",(const char*)exactSolutionOption);
  // fPrintF(file," Initial condition option: %s\n",(const char*)initialConditionOption);
  
  //  fPrintF(file," Initial beam normal=[%6.4f,%6.4f], tangent=[%6.4f,%6.4f], signForNormal=%g.\n",
  // 	   initialBeamNormal[0],initialBeamNormal[1],initialBeamTangent[0],initialBeamTangent[1],
  //        signForNormal );

  // fPrintF(file," --------------------------------------------------------------------------------\n");
  // fPrintF(file," --------------------------------------------------------------------------------\n");

  


}

// ======================================================================================
/// \brief return number of elements
// ======================================================================================
int BeamModel::
getNumberOfElements() const
{
  return dbase.get<int>("numElem");
}



// ======================================================================================
/// \brief initialize the beam model
// ======================================================================================
void BeamModel::
initialize()
{
  //Longfei 20160121: new way of handling parameters
  const real & density = dbase.get<real>("density");
  const real & L = dbase.get<real>("length");  
  const real & thickness = dbase.get<real>("thickness");
  const real & breadth = dbase.get<real>("breadth");  
  const real & areaMomentOfInertia = dbase.get<real>("areaMomentOfInertia") ;
  const real & elasticModulus = dbase.get<real>("elasticModulus");
  const int & numElem = dbase.get<int>("numElem");
  const real & pressureNorm = dbase.get<real>("pressureNorm");
  const aString & exactSolutionOption=dbase.get<aString>("exactSolutionOption");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");
  
  // initialize other parameters
  dbase.get<real>("EI") = elasticModulus*areaMomentOfInertia;
  dbase.get<real>("massPerUnitLength") = density*thickness*breadth;
  dbase.get<real>("elementLength") = L/numElem;  // was: le
  dbase.get<real>("buoyantMassPerUnitLength") = (density - pressureNorm)*thickness;
  dbase.get<real>("buoyantMass") = dbase.get<real>("buoyantMassPerUnitLength") * L;
  dbase.get<real>("totalMass") = density*L*thickness;
  dbase.get<real>("totalInertia") = dbase.get<real>("totalMass")*L*L/12.0;
  dbase.get<bool>("useExactSolution") = (exactSolutionOption=="twilightZone"  ? true : 
					 exactSolutionOption=="standingWave"          ? true :
					 exactSolutionOption=="travelingWaveFSI"     ? true :
					 exactSolutionOption=="beamPiston"              ? true : 
					 exactSolutionOption=="beamUnderPressure"  ? true : 
					 exactSolutionOption=="eigenmode"                ? true :
					 false);


  // initialize FEM element matrices
  // Do this here, since both derived class needs mass matrix to project values on 
  // beam surface to beam center/neutral line
  RealArray & elementM = *dbase.get<RealArray*>("elementM"); // (phi_i,phi_j)
  RealArray & elementT = *dbase.get<RealArray*>("elementT");//(phi.x_i,phi.x_j)
  RealArray & elementK = *dbase.get<RealArray*>("elementK");//(phi.xx_i,phi.xx_j)

  const real & le = dbase.get<real>("elementLength");
  real le2 = le*le;
  real le3 = le2*le;

  // Currently, use cubic hermit polynomials
  const int ndof = 2; // number of degrees of freedom per nodes. f_i and f.x_i 
  elementM.redim(ndof*2,ndof*2);
  elementT.redim(ndof*2,ndof*2);
  elementK.redim(ndof*2,ndof*2);

  //  element mass matrix (phi_i,phi_j):
  elementM(0,0) = elementM(2,2) = 13./35.*le;
  elementM(0,1) = elementM(1,0) = 11./210.*le2;
  elementM(0,2) = elementM(2,0) = 9./70.*le;
  elementM(0,3) = elementM(3,0) = -13./420.*le2;
  elementM(1,1) = elementM(3,3) = 1./105.*le3;
  elementM(1,2) = elementM(2,1) = 13./420.*le2;
  elementM(1,3) = elementM(3,1) = -1./140.*le3;
  elementM(3,2) = elementM(2,3) = -11./210.*le2;

  // T matrix (phi.x_i,phi.x_j):
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

  // Kness element matrix (phi.xx_i,phi.xx_j):
  elementK(0,0) = 12./le3;
  elementK(0,1) = 6./le2;
  elementK(0,2) = -elementK(0,0); 
  elementK(0,3) = elementK(0,1);

  elementK(1,0) = elementK(0,1);  
  elementK(1,1) = 4./le;
  elementK(1,2) = -elementK(0,1); 
  elementK(1,3) = 2./le;

  elementK(2,0) = elementK(0,2); 
  elementK(2,1) = elementK(1,2); 
  elementK(2,2) = elementK(0,0); 
  elementK(2,3) = elementK(1,2);
  
  elementK(3,0) = elementK(0,1); 
  elementK(3,1) = elementK(1,3); 
  elementK(3,2) = elementK(2,3);
  elementK(3,3) = elementK(1,1);


  
  //Longfei 20160126: ----- build a mappedGrid as beam grid on the domain [0,L] -----
  LineMapping line;
  line.setGridDimensions(axis1,numElem+1); // numberOfNodes  = numElem+1
  line.setPoints(0,L);    // beam domain is [0,L]
  MappedGrid &mg = dbase.get<MappedGrid>("beamGrid");
  mg = MappedGrid(line);
  mg.setNumberOfGhostPoints(0,0,numOfGhost);
  mg.setNumberOfGhostPoints(1,0,numOfGhost);
  mg.update(MappedGrid::THEvertex | MappedGrid::THEmask);

  
  // initialize TZ
  initTwilightZone();

  // --- initialize time stepping arrays ----

  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  
  Index I1,I2,I3,C;
  getSolutionArrayIndex(I1,I2,I3,C);
  
  while( u.size()<numberOfTimeLevels )
    {
      u.push_back(RealArray(I1,I2,I3,C)); 
      v.push_back(RealArray(I1,I2,I3,C)); 
      a.push_back(RealArray(I1,I2,I3,C)); 
      f.push_back(RealArray(I1,I2,I3,C));
      u.back()=0.;
      v.back()=0.;
      a.back()=0.;
      f.back()=0.;
    }
  
  
  int & current = dbase.get<int>("current"); 
  RealArray & time = dbase.get<RealArray>("time");

  assignInitialConditions( time(current), u[current],v[current],a[current] ); 

  RealArray &dtilde = dbase.get<RealArray>("dtilde");
  RealArray &vtilde = dbase.get<RealArray>("vtilde");
  dtilde = u[current];
  vtilde = v[current];
  
  //Longfei 20160617: initialize timeStep
  dbase.get<real>("dt") = getTimeStep(); 
 
  dbase.get<bool>("initialized")=true;
}

// ====================================================================================================
/// \brief Initialize the twilight zone 
// ====================================================================================================
int BeamModel::
initTwilightZone()
{
  //Longfei 20160120:
  const int & domainDimension = dbase.get<int>("domainDimension");


  // -- twilight zone ---
  const bool & twilightZone = dbase.get<bool>("twilightZone");

  if( debug() & 1 )
    printF("-- BM%i -- initTwilightZone twilightZone=%i\n",getBeamID(),(int)twilightZone);

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
	  printF("-- BM%i -- not implemented for degree in space =%i \n",getBeamID(),degreeInSpace);
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

      // Longfei 20160120:
      //int numberOfDimensions=2; // domainDimension;
      int numberOfDimensions= domainDimension;
    
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,numberOfDimensions,numberOfTZComponents,
					degreeOfTimePolynomial);

      ((OGPolyFunction*)exactPointer)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );

    }

  assert( dbase.get<OGFunction*>("exactPointer") !=NULL );

  return 0;
}


// Longfei 20160120: this function seems unused. Removed
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
// void BeamModel::
// setParameters(real momOfInertia, real E, 
// 	      real rho,real beamLength,
// 	      real thickness_,real pnorm,
// 	      int nElem,BoundaryCondition bcl,
// 	      BoundaryCondition bcr,
// 	      real x0, real y0,
// 	      bool useExactSolution_ ) 
// {


//   areaMomentOfInertia = momOfInertia;
//   elasticModulus = E;
//   density = rho;
//   L = beamLength;
//   thickness=thickness_;

//   pressureNorm = pnorm;

//   numElem = nElem;

//   bcLeft = bcl;
//   bcRight = bcr;

//   beamX0 = x0;
//   beamY0 = y0;

//   useExactSolution = useExactSolution_;

//   initialize();

// }


  // Longfei 20160503: this function is removed.
  // parameters can only be changed via update() function.
// ======================================================================================================
/// \brief Set a real beam parameter (that is in the class DataBase)
/// \param name (input) : name of a parameter in the dbase 
/// \param value (input) : value to assign
/// \return value : 0=success, 1=name not found 
// ======================================================================================================
// int BeamModel::
// setParameter( const aString & name, real & value ) 
// {

//   if( dbase.has_key(name) )
//     dbase.get<real>(name)=value;
//   else
//     {
//       printF("BeamModel::setParameter:ERROR: there is no real parameter named [%s]\n",(const char*)name);
//       return 1;
//     }

//   return 0;
// }


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
  // ****************************************
  // note that this time-step does not work for the FEMBeamModel using explicite methods.
  // for FEMBeamModel, it works with cfl=0.1. I think this is due to the eigenvalue of mass matrix.
  // ****************************************


  //Longfei 20160121: new way of handling paramters
  const real & density = dbase.get<real>("density");
  
  // estimate the expliciit time step 

  // int numNodes=numElem+1;
  //real beamLength=L;
  //real dx = beamLength/numElem; 
  const real & dx = dbase.get<real>("elementLength");
  
  const real & EI = dbase.get<real>("EI");
  const real & T = dbase.get<real>("tension");
  const real & K0 = dbase.get<real>("K0");
  const real & Kt = dbase.get<real>("Kt");
  const real & Kxxt = dbase.get<real>("Kxxt");
  
  // Guess the explicit time step: 
  //  ( c4*E*I*dt^2/dx^4 + C2*T*dt^2/dx^2 )/( rho*h*b ) < 1 

  const real cfl = dbase.get<real>("cfl");
  //const real rhosAs= density*thickness*breadth;
  const real & rhosAs = dbase.get<real>("massPerUnitLength");


  
  real dt, dtOld;
  //Longfei 20150515: this dtOld is not used any more. Disabled
  if( false )
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

  aString caseName="under-damped";
  real delta=0.;
  if(!dbase.get<bool>("isCubicHermiteFEM") && dbase.get<bool>("useSameStencilSize"))
    {
      delta=1.; // use the same five-point stencil for all the difference operators
    }
  if( true )
    {
      // **NEW WAY**
      // 
      // Compute the time stepping eigenvalue from the first order system
      //     u' = v 
      //   rhos*As*v' = - K0*u - Kt*v + T*uxx + Kxxt*vxxt - EI*uxxxx
      //
      // Longfei 20160516:
      // ** NEWER WAY**
      // we approximate the stability region using a superellipse: abs(lambdaRe/a)^n +abs(lambdaIm/b)^n <=1
      // the parameters used are:
      //      if LP  +AM2, then a = 1.; b = 1.3; n = 1.5;
      //      if AB2+AM2, then a = 1.75; b = 1.2; n = 1.5;
      //      othwesie, a = 1.; b = 1.; n = 2.;
     
      real dx2=dx*dx, dx4=dx2*dx2;
      real Bhat = ( Kt + Kxxt*(4./dx2) )/rhosAs;               // damping coefficient
      real Khat = ( K0 + T*(4./dx2+delta*4./(3.*dx2)) + EI*(16./dx4 ))/rhosAs;   // beam operator coefficient

      // Guess: explicit stability region goes to a on the real axis and b on the imaginary within a superellipse.
      real a=1., b=1., n=2.;
      if( dbase.get<TimeSteppingMethod>("correctorMethod")==adamsMoultonCorrector)
	{
	  if(dbase.get<TimeSteppingMethod>("predictorMethod")==leapFrog )
	    {
	      a = 1.; b = 1.3; n=1.5;
	    }
	  else if(dbase.get<TimeSteppingMethod>("predictorMethod")==adamsBashforth2 )
	    {
	      a = 1.; b = 1.3; n=1.5;
	    }
	}
      

      // Longfei 20160515:
      real dtMax = 0.1; // max allowed dt.
      real lambdaReal=0, lambdaIm=0.;
      if( Bhat < 2*sqrt(Khat) )
	{
	  lambdaReal = -Bhat*.5;
	  lambdaIm   =  sqrt( Khat - SQR(Bhat*.5) );

	  // dt = cfl/sqrt( SQR(lambdaReal/a) + SQR(lambdaIm/b) ); // old way using ellipse to approximate the stability region
	  dt = cfl/pow(pow(abs(lambdaReal/a),n)+pow(abs(lambdaIm/b),n),1./n); // new way using superellipse with power n  to approximate the stability region
	}
      else
	{
	  //Longfei: this  old lambdaReal was wrong...check the  beamPaper for over-damped case
	  // lambdaReal = Bhat*.5 + sqrt( SQR(Bhat*.5) - Ahat );  // 
	  // new:
	  caseName="over-damped";
	  lambdaReal = -Bhat;
	  dt = cfl*a/abs(lambdaReal);
	}

      //Longfei 20160515: make sure dt not exceed dtMax
      dt = min(dt,dtMax);
  
    }
  


  if( true || debug() & 1 )
    {
      // printF("BeamModel::getExplicitTimeStep: rho=%g, rho*A=%g, EI=%g, T=%g, K0=%g, Kt=%g, Kxxt=%g, dx=%8.2e, dt=%8.2e (dt-oldway=%8.2e) (cfl=%g).\n",
      // 	   density,density*thickness*breadth,EI,T,K0,Kt,Kxxt, dx,dt,dtOld,cfl);
      printF("-- BM%i -- BeamModel::getExplicitTimeStep (%s): rho=%g, rho*A=%g, EI=%g, T=%g, K0=%g, Kt=%g, Kxxt=%g, dx=%8.2e, dt=%8.2e  (cfl=%g).\n",
	     getBeamID(),caseName.c_str(),density,rhosAs,EI,T,K0,Kt,Kxxt, dx,dt,cfl);
    }
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
  // Longfei 20160121: new way of handling parameters
  rhoA = dbase.get<real>("massPerUnitLength");
  
  if (rhoA<0 )   // the default uninitialized value is -999.99
    {
      const real & density = dbase.get<real>("density");
      const real & thickness = dbase.get<real>("thickness");
      const real & breadth = dbase.get<real>("breadth");
      rhoA=density*thickness*breadth;
    }

  return 0;
}





// Longfei 20160121: moved to FEMBeamModel
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
// void BeamModel::
// computeGalerkinProjection(real fa, real fap, real fb, real fbp, 
// 			  real a, real b,
// 			  RealArray &  f ) 
// {
//   //Longfei 20160121: new way of handling paramters
//   const real & le = dbase.get<real>("elementLength");

//   real g1,g2,g3,g4;
//   //real le = L / numElem;    // length of an element 
//   real dxab = le*(b-a)*.5;  // length of xi sub-interval [a,b]
   
//   // File generated by cgDoc/moving/codes/beam/beam.maple :
//   #include "elementIntegrationHermiteOrder4.h"

//   f(0)=g1;
//   f(1)=g2;
//   f(2)=g3;
//   f(3)=g4;

// }

void BeamModel::
setupFreeMotion(real x0,real y0, real angle0) 
{
  //Longfei 20160121: new way of handling paramters
  const real & L = dbase.get<real>("length");
  bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");

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
  real * bodyForce =  dbase.get<real[2]>("bodyForce");

  bodyForce[0] = bf[0];
  bodyForce[1] = bf[1];

  real & projectedBodyForce= dbase.get<real>("projectedBodyForce");
  projectedBodyForce = normal[0]*bodyForce[0] + normal[1]*bodyForce[1];
}

//Longfei 20160121: modified this to be a static member function of BeamModel
// so that others can use it
void BeamModel:: 
inverse2x2(const RealArray& A, RealArray& inv) 
{
  assert(A.getLength(0)==2 && A.getLength(1)==2); // make sure A is 2x2

  inv.redim(2,2);
  real odet = 1./(A(0,0)*A(1,1) - A(0,1)*A(1,0));
  inv(0,0) =  odet*A(1,1);
  inv(0,1) = -odet*A(0,1);
  inv(1,0) = -odet*A(1,0);
  inv(1,1) =  odet*A(0,0);
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
  //Longfei 20160121: new way of handling paramters
  const real & L = dbase.get<real>("length"); 
  const int & numElem = dbase.get<int>("numElem");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");
  
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

      int si  = isFEM? 2*i:i;    // Longfei 20160216: solution index at node i is 2*i for FEM, i otherwise
      real yl = uc(si)*scaleFactor;           // displacement 

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
  //Longfei 20160121: new way of handling parameters
  const real & beamLength=dbase.get<real>("length");
  const real & le=dbase.get<real>("elementLength");
  
  
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


// Longfei 20160121: this function is no longer needed. Removed
//================================================================================================
/// \brief Provide the TravelingWaveFsi object that defines an exact solution.
///
/// \param tw (input) : use this object for computing the TravelingWaveFsi solution
//================================================================================================
// int BeamModel::
// setTravelingWaveSolution( TravelingWaveFsi & tw )
// {
//   if( !dbase.has_key("travelingWaveFsi") ) dbase.put<TravelingWaveFsi*>("travelingWaveFsi")=NULL;
 
//   dbase.get<TravelingWaveFsi*>("travelingWaveFsi")=&tw;

//   return 0;
// }



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
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
  RealArray & uc = u[current];  // current displacement 

  // ::display(x0,sPrintF("x0: getSurface (undeformed beam), t=%8.2e",t),"%9.2e ");

  RealArray & time = dbase.get<RealArray>("time");
  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("-- BM%i -- BeamModel::getSurface:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),t,time(current),current);
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
      // int boundaryCondition[2] = { bcLeft, bcRight}; // old way
      //Longfei 20160122: new way
      const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");

      for( int side=0; side<=1; side++ )
	{
	  const int bc = boundaryConditions[side];
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
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
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

      //Longfei 20160121: new way of handling parameters
      //const real dx = L/numElem;
      const real & dx = dbase.get<real>("elementLength");
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
      //Longfei 20160121: new way of handling parameters
      const real & L = dbase.get<real>("length");
    
      real xbar = ((x0-beamX0)*initialBeamTangent[0]+
		   (y0-beamY0)*initialBeamTangent[1]-L*0.5);
      assert(xbar >= -L*0.6 && xbar <= L*0.6);
      wx = centerOfMass[0] + normal[0] * displacement + xbar*tangent[0];
      wy = centerOfMass[1] + normal[1] * displacement + xbar*tangent[1];

      wx += (tangent[0] * normall[0] + normal[0]*normall[1])*halfThickness;
      wy += (tangent[1] * normall[0] + normal[1]*normall[1])*halfThickness;
    }
    
  if( debug() & 4 && exactSolutionOption=="travelingWaveFSI" )
    {
      // -- compare to exact ---
      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      // -- for testing set displacement to the true value
      Index I1(0,1),I2(0,1),I3(0,1);
      RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
      x(0,0,0,0)=x0;
      x(0,0,0,1)=0.;
      travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

      if( debug() & 4 )
	printF(" -- BM%i -- projectDisplacement: t=%8.2e, x0=%8.2e computed w=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	       getBeamID(), t,x0,wx,wy-y0,ue(0,0,0,0),ue(0,0,0,1),wx-ue(0,0,0,0),(wy-y0)-ue(0,0,0,1));
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
  //Longfei 20160121: new way of handling parameters
  const real & L = dbase.get<real>("length");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
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

  if( debug() & 2 )
    printF(" -- BM%i -- projectAcceleration: x=(%g,%g) DDdisplacement=%g (beam accel)\n",getBeamID(),x0,y0,DDdisplacement);
  
  
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
  if(( useExact || debug() & 4 ) && exactSolutionOption=="travelingWaveFSI" )
    {
      // -- compare to exact ---
      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      // -- for testing set acceleration to the true value
      Index I1(0,1),I2(0,1),I3(0,1);
      RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
      x(0,0,0,0)=x0;
      x(0,0,0,1)=0.;
      travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

      if( ( useExact || debug() & 4 ) )
	printF(" -- BM%i -- projectAcceleration: t=%8.2e, x0=%8.2e computed a=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	       getBeamID(),t,x0,ax,ay,ae(0,0,0,0),ae(0,0,0,1),ax-ae(0,0,0,0),ay-ae(0,0,0,1));
    
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

  // Longfei 20160121: new way of handling parameters
  const real & density = dbase.get<real>("density");
  const real & L = dbase.get<real>("length");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
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
  // Longfei 20160121: new way of handling parameters
  const real & density = dbase.get<real>("density");
  const real & thickness = dbase.get<real>("thickness");
  const real & breadth = dbase.get<real>("breadth");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");

  if( true )
    {
      // *wdh* 2015/01/05 *new* way
      const bool addExternalForcing=true;
      getSurfaceInternalForce(t, x0, as, normal, Ib1,Ib2,Ib3,addExternalForcing );

      const real &Abar = dbase.get<real>("massPerUnitLength");
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
      //int boundaryCondition[2] = { bcLeft, bcRight}; // old way
      // Longfei 20160122: new way
      const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");

      for( int side=0; side<=1; side++ )
	{
	  const int bc = boundaryConditions[side];
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
      // int boundaryCondition[2] = { bcLeft, bcRight}; //
      // Longfei 20160122: new way
      const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
      const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
      const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
    
      for( int side=0; side<=1; side++ )
	{
	  const int bc = boundaryConditions[side];
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



// Longfei 20160331: modifications are made for this function to work for both FEM and FD beamModel
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

  //const int & numElem = dbase.get<int>("numElem");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");

  BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  BoundaryCondition & bcLeft = boundaryConditions[0];
  BoundaryCondition & bcRight = boundaryConditions[1];
  
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
      printF("-- BM%i -- BeamModel::getSurfaceInternalForce:WARNING: t=%10.3e < 0. : evaluate at t=0.\n"
	     " time(current)=%10.3e, time(prev)=%10.3e\n",getBeamID(),t,time(current),time(prev));
      t=0.;
    }
  



  RealArray & xc = u[current];
  RealArray & vc = v[current];
  RealArray & ac = a[current];
  RealArray & fc = f[current];

  if( fabs(time(current)-t) > 1.e-10*(1.+t) )
    {
      printF("-- BM%i -- FEMBeamModel::getSurfaceInternalForce:ERROR: t=%10.3e != time(current)=%10.3e, current=%i\n",
	     getBeamID(),t,time(current),current);
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
  // if addExternalForcing
  //     L(u) +f =  ms*a 
  // else 
  //     L(u) =  Ms^{-1} ( K u + Bv)

  // old:
  //RealArray internalForce(2*numElem+2), fe(2*numElem+2);
  //Longfei 20160331: new. Array dimensions are given by solution
  RealArray internalForce, fe;
  internalForce.redim(fc);
  fe.redim(fc);

  if( addExternalForcing ) 
    {
      fe=fc;  // *wdh* 2015/01/04  -- include external force
    }
  else
    {
      fe=0.;  // set external force to zero
    }
    
  if( false )
    ::display(fe,sPrintF("-- BM -- getSurfaceInternalForce: external force fe at t=%8.2e",t),"%8.2e ");  



  //Longfei: no need for these anymore
  //bool & refactor = dbase.get<bool>("refactor"); 
  // dbase.get<bool>("rhsSolverAddExternalForcing") =addExternalForcing; // save current option

    
  // ---- Boundary Conditions ----boundaryConditions
  // old way:
  //  BoundaryCondition bcLeftSave =bcLeft;  // save current 
  // BoundaryCondition bcRightSave=bcRight;
  // Longfei 20160122: new way
  BoundaryCondition bcLeftSave =boundaryConditions[0];  // save current 
  BoundaryCondition bcRightSave=boundaryConditions[1];

  
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

  // Longfei 20160209: no need for this any more
  // const real alpha =0.;  // coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
  // const real alphaB=0.;  // coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
  const real dt=0.;
  // old:
  // computeAcceleration( t, xc,vc,fe, Me, internalForce, centerOfMassAcceleration, angularAcceleration, dt,
  // 		       alpha,alphaB,"rhsSolver" );
  // bool & refactor = dbase.get<bool>("refactor");
  // refactor=true; //Longfei 20160209: no longer need to refactor solvers
  
  if(addExternalForcing)
    { 
      // Longfei 20160701: recompute ac. The velocity is changed after projecting the beam and fluid velocities at the interface
      computeAcceleration( t, xc,vc,fe, ac, centerOfMassAcceleration, angularAcceleration, dt,"explicitSolver" );
      internalForce=Abar*ac;
      
    }
  else
    {
      aString tridiagonalSolverName= isFEM? "massMatrixSolverNoBC" : "explicitSolver";
      computeAcceleration( t, xc,vc,fe, internalForce, centerOfMassAcceleration, angularAcceleration, dt,tridiagonalSolverName );
    }
  
  if( debug() & 16 && false )
    {
      ::display(internalForce,sPrintF("-- BM -- getSurfaceInternalForce: internalForce at t=%8.2e",t),"%8.2e ");  
      ::display(fc,sPrintF("-- BM -- getSurfaceInternalForce: current force fc at t=%8.2e",t),"%8.2e ");  
    }


  // refactor=true;          

  bcLeft =bcLeftSave;   // reset 
  bcRight=bcRightSave; 

  if( FALSE && !addExternalForcing ) 
    {
      if( t<100*dt)
	printF("-- BM%i -- Smooth the internalForce at t=%8.2e\n",getBeamID(),t); 
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
    ::display(fs,"-- BM -- getSurfaceInternalForce : fs after projectInternalForce","%8.2e ");


  // ::display(fs,"--BM-- internalForce from projectInternalForce","%8.2e ");
    
  bool useExact=false;
  if( useExact && exactSolutionOption=="travelingWaveFSI" )
    {
      // -- compare to exact ---
      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      // -- for testing set internal force to the true value
      //Longfei 20160120:
      const int numberOfDimensions=dbase.get<int>("rangeDimension");


      Range Rx=numberOfDimensions;
      RealArray ue(Ib1,Ib2,Ib3 ,Rx),ve(Ib1,Ib2,Ib3 ,Rx),ae(Ib1,Ib2,Ib3 ,Rx);
      travelingWaveFsi.getExactShellSolution( x0,ue,ve,ae, t, Ib1,Ib2,Ib3 );

      RealArray uf(Ib1,Ib2,Ib3,numberOfDimensions+1); // hold fluid solution (p,v1,v2)
      real t0=t;
      travelingWaveFsi.getExactFluidSolution( uf, t0, x0, Ib1,Ib2,Ib3 );

      // *TESTING* -- use exact: 

      // Longfei 20160121: new way of handling parameters  
      const  real &  ms=dbase.get<real>("massPerUnitLength");
      // real ms=density*thickness*breadth;
    
      fs(Ib1,Ib2,Ib3,Rx)=ms*ae(Ib1,Ib2,Ib3,Rx);
      fs(Ib1,Ib2,Ib3,1) -= uf(Ib1,Ib2,Ib3,0);  // Lu = rhos*As*v_t - p 
    
      printF("-- BM%i --getSurfaceInternalForce *TEST* set internal force to exact, t=%8.2e\n",getBeamID(),t);

      ::display(fs,"-- BM -- internalForce from EXACT","%8.2e ");

    }
  

  // -- Adjust the surface force to match the surface normals ---
  // -- THE AMP scheme wants a beam force Fs that satisfies
  //      nv.Fs = +/- nbv.fs
  //      tv.Fs = +/- tbv.fs  
  //  where nv = fluid normal, tv = fluid tangent
  //  where nbv, tbv = beam normal and tangent (undeformed beam)
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
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
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

  if( debug() & 2 )
    printF(" -- BM%i -- projectVelocity: x=(%g,%g) Ddisplacement=%g (beam velocity)\n",getBeamID(),x0,y0,Ddisplacement);
  
  
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

  if( debug() & 4  && exactSolutionOption=="travelingWaveFSI" )
    {
      // -- compare to exact ---

      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      // -- for testing set acceleration to the true value
      Index I1(0,1),I2(0,1),I3(0,1);
      RealArray x(1,1,1,2), ue(1,1,1,2), ve(1,1,1,2), ae(1,1,1,2);
      x(0,0,0,0)=x0;
      x(0,0,0,1)=0.;
      travelingWaveFsi.getExactShellSolution( x,ue,ve,ae, t, I1,I2,I3 );

      if( debug() & 4 )
	printF(" -- BM%i -- projectVelocity: t=%8.2e, x0=%8.2e computed v=(%9.2e,%9.2e) exact=(%9.2e,%9.2e) err=(%9.2e,%9.2e)\n",
	       getBeamID(), t,x0,vx,vy,ve(0,0,0,0),ve(0,0,0,1),vx-ve(0,0,0,0),vy-ve(0,0,0,1));

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
  real & beamInitialAngle = dbase.get<real>("beamInitialAngle");
  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
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

  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  const int & numElem = dbase.get<int>("numElem");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
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
  
  halfThickness = yl;  // signed distance
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

  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");


  
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

  //Longfei 20160802: this is wrong!!!
  // real sfd[4] = {(-0.5*eta1*etap2+0.25*eta1*eta1)/le,
  // 		 -0.25*eta1*etap1+0.25*eta1*eta1,
  // 		 (0.5*etap1*eta2-0.25*etap1*etap1)/le,
  // 		 -0.25*etap1*eta1+0.25*etap1*etap1 };
  // new:  CHECKME.......THIS IS FIXED!
    real sfd[4] = {(-0.5*eta1*etap2+0.25*eta1*eta1)*2./le,
    		 -0.5*eta1*etap1+0.25*eta1*eta1,
    		 (0.5*etap1*eta2-0.25*etap1*etap1)*2./le,
    		 -0.5*etap1*eta1+0.25*etap1*etap1 };

	
  //Longfei 20160308:
  // solutions and slope at grid points elemNum and elemNum+1
  real Xsolution[2]={0.,0.};
  real Xslope[2]={0.,0.};
  if(isFEM)
    {
      // FEM solutions contains x and its derivatives
      Xsolution[0]=X(elemNum*2,0,0,0);
      Xsolution[1]=X(elemNum*2+2,0,0,0);
      Xslope[0]=X(elemNum*2+1,0,0,0);
      Xslope[1]=X(elemNum*2+3,0,0,0);
    }
  else
    {
      // FD solutions contains only x. We need to evalute its derivatives using FD schemes
      Xsolution[0]=X(elemNum,0,0,0);
      Xsolution[1]=X(elemNum+1,0,0,0);

      const bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");
      real delta = useSameStencilSize?1.:0.;
      
      //evaluate the slope at grid points  elemNum and elemNum+1

      //Longfei 20160719: use the approximated derivative instead of X. It was wrong before. FIXED!
      int i=elemNum;
      Xslope[0]=(delta/12.*X(i-2,0,0,0)-(.5+delta/6.)*X(i-1,0,0,0)+(.5+delta/6.)*X(i+1,0,0,0)-(delta/12.)*X(i+2,0,0,0))/le;
      i=elemNum+1;
      Xslope[1]=(delta/12.*X(i-2,0,0,0)-(.5+delta/6.)*X(i-1,0,0,0)+(.5+delta/6.)*X(i+1,0,0,0)-(delta/12.)*X(i+2,0,0,0))/le;
    }


  displacement = sf[0]*Xsolution[0]+sf[1]*Xslope[0]+
    sf[2]*Xsolution[1] +sf[3]*Xslope[1] ;
  slope = sfd[0]*Xsolution[0]+sfd[1]*Xslope[0]+
    sfd[2]*Xsolution[1] +sfd[3]*Xslope[1];

}


//Longfei 20160131: solveBlockTridiagonal new
// old implementation is commnented out in FEMBeamModel.C
// ================================================================================
/// \brief Solve A u = f, A is associated with the solver
/// \param f (input): rhs of the system
/// \param u (output): solution
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
//
// ================================================================================
void BeamModel::
solveBlockTridiagonal(const RealArray& f, RealArray& u, const aString & tridiagonalSolverName )
{


  const int & numElem = dbase.get<int>("numElem");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft = boundaryConditions[0];
  const BoundaryCondition & bcRight = boundaryConditions[1];

  const bool isPeriodic = bcLeft==periodic;

  factorBlockTridiagonalSolver(tridiagonalSolverName);  // factor the solver if refactor==true	    
  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
  assert( pTri!=NULL );

  TridiagonalSolver & tri = *pTri;

  int nTri = numElem;
  if( isPeriodic ) nTri=numElem-1;

  Index  I1=Range(0,nTri), I2=Range(0,0);
  const int ndof=2;  // number of degrees of freedom per node 

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
      u(i*2,0,0,0) = xTri(0,i,0);
      u(i*2+1,0,0,0)=xTri(1,i,0);
    }
  if( isPeriodic )
    { // -- assign values on last node
      int i=numElem;
      u(i*2,0,0,0) = u(0,0,0,0);
      u(i*2+1,0,0,0) = u(1,0,0,0);
    }
    
}






//Longfei 20160206: new function to factor block tridiangnal solvers
//for FEMBeamModel and/or projections between beam surface and beam neutal line
//================================================================================
///\brief factor the tridiangnal solver 
/// \param tridiagonalSolverName (input) : name of the tridiagonal solver
// output: factor the solver with matrix Ae
// if(tridiagonalSolverName==newmarkSolver)  Ae =  elementM+alpha*elementK+alphB*elementB
// if(tridiagonalSolverName==massMatrixSolver)  Ae = elementM/Abar
//================================================================================
int BeamModel::
factorBlockTridiagonalSolver(const aString & tridiagonalSolverName)
{
  TridiagonalSolver *& pTri = dbase.get<TridiagonalSolver*>(tridiagonalSolverName);
  bool & refactor = dbase.get<bool>("refactor");
  if( pTri==NULL )
    {
      pTri = new TridiagonalSolver();
      refactor=true;
      printF("-- BM%i -- construct TridiagonalSolver=[%s]\n",getBeamID(),(const char*)tridiagonalSolverName);
    }
  if(!refactor) return 0;

  assert( pTri!=NULL );

  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  boundaryConditions[0];
  const BoundaryCondition & bcRight =  boundaryConditions[1];
 
  TridiagonalSolver & tri = *pTri;
  const bool isPeriodic = bcLeft==periodic;
  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const RealArray & elementK = *dbase.get<RealArray*>("elementK");
  const RealArray & elementM = *dbase.get<RealArray*>("elementM");
  const RealArray & elementB = *dbase.get<RealArray*>("elementB");
  const int & numElem = dbase.get<int>("numElem");
  const real & EI = dbase.get<real>("EI");
  const real & T = dbase.get<real>("tension");
  const real & Kxxt = dbase.get<real>("Kxxt");
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
 

    
  
  if( isPeriodic ) 
    { // consistency check:
      assert( bcRight==periodic );
    }
   


  real alpha=0.;
  real alphaB=0.;
  bool bcFixup = true; // flag to indicate if we need boundary fixup
  // prepare the matrix for different solvers
  RealArray Ae;
  if(tridiagonalSolverName=="implicitNewmarkSolver") // solve (M+alpha*K+alphB*B) u = f
    {
      const real & newmarkBeta = dbase.get<real>("newmarkBeta");
      const real & newmarkGamma = dbase.get<real>("newmarkGamma");
      const real & dt = dbase.get<real>("dt");
      
      alpha =newmarkBeta*dt*dt;  // coeff of K in A
      alphaB=newmarkGamma*dt;    // coeff of B in A
      Ae = Abar*elementM + alpha*elementK;
      if( addDampingMatrix )
	{ // add damping matrix B 
	  Ae += alphaB*elementB;
	}
      
    }
  else if(tridiagonalSolverName=="massMatrixSolver") // solve M u = f
    {
      Ae  = elementM;
    }
  else if(tridiagonalSolverName=="massMatrixSolverNoBC") // solver M u = f with no bcFixups done to the matrix 
    {
      // Note: We use a separate TridiagonalSolver for galerkinProjection.
      //  We apply no boundary conditions to this solver
      Ae = elementM;
      bcFixup=false;
    }
  else
    {
      printF("-- BM%i -- unknown tridiagonalSolverName=[%s]\n",getBeamID(),(const char*)tridiagonalSolverName);
      OV_ABORT("Error: unknown tridiagonalSolverName");
    }
      
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
  


  if( true || debug() & 1 )
    printF("-- BM%i -- solveBlockTridiagonal : name=[%s] form block tridiagonal system and factor, isPeriodic=%i\n",
	   getBeamID(),(const char*)tridiagonalSolverName, (int)isPeriodic);
      
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
  if( !allowsFreeMotion && bcFixup )
    {
      // --- Boundary conditions ---
      // Adjust the matrix for essential BC's -- these will set the DOF's at boundaries
      for( int side=0; side<=1; side++ )
	{
	  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
	  int ia = side==0 ? 0 : numElem;

	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  // const real EI = elasticModulus*areaMomentOfInertia;
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
		  printF("-- BM%i -- ERROR: A `free' BC is not allowed with the string model for a beam, EI=0\n",getBeamID());
		  OV_ABORT("ERROR");
		}
	    
	      bt(0,1,ia,0) +=  (1-2*side)*T*alpha;

	      // The boundary term K_xxt*v*w_xt also contributes
	      bt(0,1,ia,0) +=  (1-2*side)*Kxxt*alphaB;

	    }
	  

	}
    }

      
  // Factor the block tridiagonal system:
  tri.factor(at,bt,ct,systemType,axis1,ndof);
 
  refactor=false;

}

// Longfei 20160731: this function looks like unused. Removed
// ======================================================================================
/// \brief Compute the third derivative, w'''(x), of the beam displacement w(x) at a given
//// element # and coordinate
/// X:       Beam solution (position)
/// elemNum: element number on which the solution is desired
/// eta:     element natural coordinate where the solution is desired
/// deriv3:  Third derivative, w'''(x) at this point
///
// ======================================================================================
// void BeamModel::
// interpolateThirdDerivative(const RealArray& X,
// 			   int& elemNum, real& eta,
// 			   real& deriv3) 
// {
//   //Longfei 20160121: new way of handling parameters
//   const real & le = dbase.get<real>("elementLength");

  
//   // compute the shape functions.
//   real eta1 = 1.-eta;
//   real eta2 = 2.-eta;
//   real etap1 = eta+1.0;
//   real etap2 = eta+2.0;
  
//   real sf[4] = {12.0/(le*le*le),6.0/(le*le), -12.0/ (le*le*le),6.0/(le*le)};

		
//   deriv3 = sf[0]*X(elemNum*2)+sf[1]*X(elemNum*2+1)+
//     sf[2]*X(elemNum*2+2) +sf[3]*X(elemNum*2+3) ;
  
// }



//Longfei: new...
// reset dbase.get<RealArray>("surfaceForce");
void BeamModel::
resetForce()
{

  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force
  RealArray & fc = f[current];
  fc=0.; // reset fc
  totalPressureForce = 0.0;
  totalPressureMoment = 0.0;
  // printF("-- BM -- resetForce current=%i \n",current);

  //Longfei 20160330: new:
  // save the load vector of surface force on the beam neutral line as "surfaceForce"
  // for FEMBeamModel, the load vector is used directly on the rhs of the beam equation
  // for FDBeamMode, a projection is needed to convert the load vector to nodal values
  if( !dbase.has_key("surfaceForce") )
    {
      const int & numElem = dbase.get<int>("numElem");
      const int numMotionDirections = dbase.get<int>("numberOfMotionDirections");
      Index I1,I2,I3,C;
      I1 = Range(2*numElem+2); // each node i has 2 solutions,i.e. ui and uxi
      I2 = 0; I3=0; // Beam Domain is assumed to be 1D
      C = Range(numMotionDirections);
      RealArray & surfaceForce = dbase.put<RealArray>("surfaceForce");
      surfaceForce.redim(I1,I2,I3,C);
   
    }
  RealArray & surfaceForce = dbase.get<RealArray>("surfaceForce");
  surfaceForce=0.; //reset surface force



}


// Longfei 20160330: new function that sets the surface force:
// ================================================================================
/// \brief  Set the surface velocity (used to project the beam velocity) 
///         results are load vector of the force(traction) on beam neutral line.
///         The load vector is stored in dbase.get<RealArray>("surfaceForece")
/// \param t (input) : set velocity at this time.
/// \param x0 (input) : initial positions of the points on the beam surface. 
/// \param traction (input) : traction on the surface,  traction = -sigma*n
/// \param normal (input) : normal to the surface (inward!) (currently not used?)
/// \param Ib1,Ib2,Ib3 (input) : index values for points to assign. 
// ================================================================================
void BeamModel::
setSurfaceForce(const real & t, const RealArray & x0, const RealArray & traction, 
				  const RealArray & normal, const Index & Ib1, const Index & Ib2,  const Index & Ib3 )
{
  
  // *new way* 2015/01/13
  RealArray & sf =  dbase.get<RealArray>("surfaceForce");
  
  RealArray fDotN(Ib1,Ib2,Ib3);

  // --- transfer the normal component of the force -- here we use the current fluid normal
  // NOTE: we could alternatively have used:
  //              (1) current beam normal
  //              (2) initial beam normal
  fDotN(Ib1,Ib2,Ib3)= (traction(Ib1,Ib2,Ib3,0)*normal(Ib1,Ib2,Ib3,0)+
		       traction(Ib1,Ib2,Ib3,1)*normal(Ib1,Ib2,Ib3,1) );
  if( false )
    {
      ::display(traction(Ib1,Ib2,Ib3,Range(0,1)),sPrintF("-- BM -- addForce: input traction t=%9.3e",t),"%9.2e ");
      ::display(fDotN,sPrintF("-- BM -- addForce: input fDotN t=%9.3e",t),"%9.2e ");
    }
  

  bool addToForce=true;
  addToElementIntegral( t,x0,fDotN,normal,Ib1,Ib2,Ib3,sf, addToForce );

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
  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  
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
  //Longfei 20160121: new way of handling paramters
  const real & le = dbase.get<real>("elementLength");

  real g1,g2,g3,g4;
  //real le = L / numElem;    // length of an element 
  real dxab = le*(b-a)*.5;  // length of xi sub-interval [a,b]
   
  // File generated by cgDoc/moving/codes/beam/beam.maple :
#include "elementIntegrationHermiteOrder4.h"

  f(0)=g1;
  f(1)=g2;
  f(2)=g3;
  f(3)=g4;

}



// Longfei 20160211: modifications are done to make this function work for both FEM and FD
// ================================================================================
/// \brief Set the surface velocity to zero. (used to project the velocity)
// ================================================================================
void BeamModel::
resetSurfaceVelocity()
{
  if( !dbase.has_key("surfaceVelocity") )
    {
      const int & numElem = dbase.get<int>("numElem");
      const int numMotionDirections = dbase.get<int>("numberOfMotionDirections");
      Index I1,I2,I3,C;
      I1 = Range(2*numElem+2); // each node i has 2 solutions,i.e. ui and uxi
      I2 = 0; I3=0; // Beam Domain is assumed to be 1D
      C = Range(numMotionDirections);
      RealArray & surfaceVelocity = dbase.put<RealArray>("surfaceVelocity");
      surfaceVelocity.redim(I1,I2,I3,C);
    
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
  //Longfei 20160330: this function converts velocity on beam surface "vSurface" to
  //                  load vector of velocity on the beam neutral line (\bar{v},phi_i).
  //                  The load vector is stored in dbase.get<RealArray>("surfaceVelocity")

  // *new way* 2015/01/13

  // Transfer the normal component of the velocity, stored here:
  RealArray vDotN(Ib1,Ib2,Ib3);

  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
  
  //Longfei 20160622:  make correctForSurfaceRotation=true
  const bool correctForSurfaceRotation=true;  // *CHECK ME* 2015/06/02 
  if( correctForSurfaceRotation )
    {
      // ---  Remove the surface rotation term "W" before projecting the velocity onto the beam reference line ----
      // Added: 2015/05/17 *wdh*
      const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
      const int & current = dbase.get<int>("current"); 
      std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement DOF 
      std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity DOF
      RealArray & uc = u[current];  // current displacement DOF
      RealArray & vc = v[current];  // current velocity DOF

      const RealArray & time = dbase.get<RealArray>("time");
      if( fabs(time(current)-t) > 1.e-10*(1.+t) )
	{
	  printF("-- BM%i -- BeamModel::setSurfaceVelocity:ERROR: t=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
		 getBeamID(),t,time(current),current);
	  OV_ABORT("ERROR");
	}

      const int & rangeDimension=dbase.get<int>("rangeDimension");
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
      
	  //Longfei 20160623: normald after rotation
	  real normaldRot[2];
	  normaldRot[0]=initialBeamTangent[0]*normald[0] + initialBeamNormal[0]*normald[1];
	  normaldRot[1]=initialBeamTangent[1]*normald[0] + initialBeamNormal[1]*normald[1];

	  if(debug() & 8)
	    {
	      printF("|normald-normaldRot|*h/2 = [%10.3e,%10.3e]\n",
		     abs((normald[0]-normaldRot[0])*halfThickness),abs((normald[1]-normaldRot[1])*halfThickness));
	    }

	  if( !allowsFreeMotion ) 
	    { // -- subtract off "w"
	      for( int axis=0; axis<rangeDimension; axis++ )
		{
		  // Longfei 20160623:
		  //old: 
		  //vBeam(i1,i2,i3,axis) = vSurface(i1,i2,i3,axis) - normald[axis]*halfThickness; 
		  //new: should use normaldRot not normald
		  vBeam(i1,i2,i3,axis) = vSurface(i1,i2,i3,axis) - normaldRot[axis]*halfThickness;
		}
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
surfaceVelocity() const
{
  return dbase.get<RealArray>("surfaceVelocity"); 
}



// Longfei 20160116: Removed since *THIS FUNCTION NOT USED ANYMORE*
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
// void BeamModel::
// setSurfaceVelocity( const real & tf,
// 		    const real& x0_1, const real& y0_1,
// 		    real v1, real v1x, const real& nx_1,const real& ny_1,
// 		    const real& x0_2, const real& y0_2,
// 		    real v2, real v2x, const real& nx_2,const real& ny_2)
// {

//   // new way *wdh* 2015/01/13
//   real x1[2]={ x0_1,y0_1}, nv1[2]={nx_1,ny_1}; //
//   real x2[2]={ x0_2,y0_2}, nv2[2]={nx_2,ny_2}; //

//   RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");
//   addToElementIntegral( tf,x1,v1,v1x,nv1, x2,v2,v2x,nv2,surfaceVelocity );

//   // **OLD **

//   // // THIS FUNCTION IS ALMOST THE SAME AS ADDFORCE ** FIX ME**

//   // const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
//   // const int & current = dbase.get<int>("current"); 

//   // RealArray & time = dbase.get<RealArray>("time");
//   // RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");

//   // if( fabs(time(current)-tf) > 1.e-10*(1.+tf) )
//   // {
//   //   printF("--BM-- BeamModel::setSurfaceVelocity:ERROR: tf=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
//   //       tf,time(current),current);
//   //   OV_ABORT("ERROR");
//   // }

//   // int elem1,elem2;
//   // real eta1,eta2,t1,t2;

//   // real v11,v22, v11x, v22x;


//   // real xll = x0_1-beamX0;
//   // real yll = y0_1-beamY0;

//   // real xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
//   // real yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

//   // real myx0 = xl;

//   // xll = x0_2-beamX0;
//   // yll = y0_2-beamY0;

//   // xl = xll*initialBeamTangent[0]+yll*initialBeamTangent[1];
//   // yl = xll*initialBeamNormal[0]+yll*initialBeamNormal[1];

//   // real myx1 = xl;

//   // if (myx1 > myx0) 
//   // {
//   //   projectPoint(x0_1,y0_1,elem1, eta1,t1); 
//   //   projectPoint(x0_2,y0_2,elem2, eta2,t2);   
//   //   v11 = v1; v11x=v1x;
//   //   v22 = v2; v22x=v2x;
//   // } 
//   // else 
//   // {
//   //   projectPoint(x0_1,y0_1,elem2, eta2,t2); 
//   //   projectPoint(x0_2,y0_2,elem1, eta1,t1);  
//   //   v22 = v1; v22x=v1x;
//   //   v11 = v2; v11x=v2x;
//   //   std::swap<real>(myx0,myx1);
//   // }

//   // //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << v1 << " " << v2 << std::endl;

//   // real dx = fabs(myx0-myx1);


//   // const int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");

//   // RealArray lt(4);
//   // for (int i = elem1; i <= elem2; ++i) 
//   // {
//   //   real a = eta1, b = eta2;
//   //   real va = v11, vax=v11x, vb = v22, vbx=v22x;
//   //   real x0 = myx0, x1 = myx1;
//   //   if (i != elem1)
//   //   {
//   //     // estimate v at xi=x0 : 
//   //     a = -1.0;
//   //     x0 = le*i;
      
//   //     if( orderOfGalerkinProjection==2 )
//   //     {
//   //       va = v11 + (v22-v11)*(x0-myx0)/(dx);
//   //     }
//   //     else
//   //     {
//   //       // -- evaluate the Hermite interpolant --
//   //       real xi = -1. + 2.*(x0-myx0)/le;

//   //       real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
//   //       real N2 = .125*dx*(1.-xi)*(1.-xi)*(1.+xi);
//   //       real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
//   //       real N4 = .125*dx*(1.+xi)*(1.+xi)*(xi-1.);
	
//   //       real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx);
//   //       real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
//   //       real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx) ;
//   //       real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;
	
//   //       va = v11*N1 + v11x*N2 + v22*N3 + v22x*N4; 
//   //       vax = v11*N1x + v11x*N2x + v22*N3x + v22x*N4x; 

//   //       // vax = v11x + (v22x-v11x)*(x0-myx0)/(dx);
//   //     }
      
//   //   }
//   //   if (i != elem2) 
//   //   {
//   //     b = 1.0;
//   //     x1 = le*(i+1);
//   //     if( orderOfGalerkinProjection==2 )
//   //     {
//   //       vb = v11 + (v22-v11)*(x1-myx0)/(dx);
//   //     }
//   //     else
//   //     {
//   //       // -- evaluate the Hermite interpolant --
//   //       real xi = -1. + 2.*(x1-myx0)/le;

//   //       real N1 = .25*(1.-xi)*(1.-xi)*(2.+xi);
//   //       real N2 = .125*dx*(1.-xi)*(1.-xi)*(1.+xi);
//   //       real N3 = .25*(1.+xi)*(1.+xi)*(2.-xi);
//   //       real N4 = .125*dx*(1.+xi)*(1.+xi)*(xi-1.);
	
//   //       real N1x = ( .5*(xi-1.)*(2.+xi)  + .25*(1.-xi)*(1.-xi) )*(2./dx);
//   //       real N2x = ( .25*(xi-1.)*(1.+xi) + .125*(1.-xi)*(1.-xi) )*2.;
//   //       real N3x = ( .5*(1.+xi)*(2.-xi)  - .25*(1.+xi)*(1.+xi) )*(2./dx) ;
//   //       real N4x = ( .25*(1.+xi)*(xi-1.) + .125*(1.+xi)*(1.+xi) )*2.;

//   //       vb = v11*N1 + v11x*N2 + v22*N3 + v22x*N4; 
//   //       vbx = v11*N1x + v11x*N2x + v22*N3x + v22x*N4x; 
//   //       // vbx = v11x + (v22x-v11x)*(x1-myx0)/(dx);
//   //     }
      
//   //   }
    
//   //   Index idx(i*2,4);
//   //   if( FALSE && t1 > 0)   // TURN OFF FOR VELOCITY 
//   //   { 
//   //     va = -va; vax=-vax;
//   //     vb = -vb; vbx=-vbx;
//   //   }
      
//   //   //std::cout << elem1 << " " << elem2 << " " << eta1 << " " << eta2 << " " << 
//   //   //  v1 << " " << v2 << std::endl;
  
    
//   //   if( fabs(b-a) > 1.0e-10 )  // *WDH* FIX ME -- is this needed?
//   //   {
//   //     // -- compute (N,p)_[a,b] = int_a^b N(xi) v(xi) J dxi 
//   //     if( orderOfGalerkinProjection==2 )
//   //       computeProjectedForce(va,vb, a,b, lt);
//   //     else
//   //       computeGalerkinProjection(va,vax, vb,vbx,  a,b, lt);
        
//   //     //    std::cout << "a = " << a << " b = " << b << std::endl;
//   //     surfaceVelocity(idx) += lt;  
//   //   }

//   // }
// }


// ===================================================================================================
/// \brief  Project the current surface velocity onto the beam (and over-write current beam velocity)
// ====================================================================================================
void BeamModel::
projectSurfaceVelocityOntoBeam( const real t )
{

  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  const int & numElem = dbase.get<int>("numElem");
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const bool & isFEM=dbase.get<bool>("isCubicHermiteFEM");

  RealArray & surfaceVelocity = dbase.get<RealArray>("surfaceVelocity");

  // On entry:
  //   surfaceVelocity should hold the integrals (phi_i,v) (psi_i,v)

  // Stage I: Compute the FEM  coefficients v_j and v_j' coefficients in
  //       v = SUM_j {  v_j phi_j(x) + v_j' psi_j(x) }
  // by solving
  //     SUM_j {  v_j (phi_i(x),phi_j(x)) + v_j' (phi_i(x),psi_j(x)) }  = (phi_i,v)
  //     SUM_j {  v_j (psi_i(x),phi_j(x)) + v_j' (psi_i(x),psi_j(x)) }  = (psi_i,v)

  //real le = L / numElem;
  //Longfei 20160209: no need to do this. factorTridiangonalSolver takes care of the matrix now
  // real le2 = le*le;
  // real le3 = le2*le;
  // RealArray elementMass(4,4);
  
  // elementMass(0,0) = elementMass(2,2) = 13./35.*le;
  // elementMass(0,1) = elementMass(1,0) = 11./210.*le2;
  // elementMass(0,2) = elementMass(2,0) = 9./70.*le;
  // elementMass(0,3) = elementMass(3,0) = -13./420.*le2;
  // elementMass(1,1) = elementMass(3,3) = 1./105.*le3;
  // elementMass(1,2) = elementMass(2,1) = 13./420.*le2;
  // elementMass(1,3) = elementMass(3,1) = -1./140.*le3;
  // elementMass(3,2) = elementMass(2,3) = -11./210.*le2;

  RealArray rhs;
  if( dbase.get<bool>("fluidOnTwoSides") )
    rhs=.5*surfaceVelocity;  // scale by .5 for two-sided fluid 
  else
    rhs=surfaceVelocity;

  //real alpha=0., alphaB=0.; // coefficients of stiffness and damping matrices

  // Assign BC's on v (rhs)
  RealArray uTemp(2*numElem+2), aTemp(2*numElem+2), fTemp(2*numElem+2);
  if( true )
    BeamModel::assignBoundaryConditions( t, uTemp, rhs, aTemp, fTemp);

  // Note: We use a separate TridiagonalSolver for this Galerkin projection: 
  //solveBlockTridiagonal(elementMass, rhs, surfaceVelocity, alpha,alphaB, "galerkinProjection" );
  // convert (v,phi_i) to v_i
  // Longfei 20160209: new
  solveBlockTridiagonal( rhs, surfaceVelocity, "massMatrixSolver" );

  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  const int & current = dbase.get<int>("current");

  if( FALSE )
    {
      ::display(v[current],sPrintF("-- BM -- projectSurfaceVelocityOntoBeam:END: : v[current] at t=%9.3e",t),"%8.2e ");

      ::display(surfaceVelocity,sPrintF("-- BM -- projectSurfaceVelocityOntoBeam: surfaceVelocity at t=%9.3e",t),"%8.2e ");
      real maxErr = max(fabs(v[current]-surfaceVelocity));
      printF("-- BM%i -- projectSurfaceVelocityOntoBeam max-err=%8.2e\n",getBeamID(),maxErr);

    }

  // Replace the current velocity
  if(isFEM)
    {
      v[current]=surfaceVelocity;
    }
  else
    {
      // copy only the values not the derivatives
      Index I1,I2,I3,C;
      I1 = Range(0,numElem);
      I2=I3=C=0;
      v[current](I1,I2,I3,C)=surfaceVelocity(2*I1,I2,I3,C);
    }
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

  //Longfei 20160121: new way of handling parameters
  const real & L = dbase.get<real>("length");
  const int & numElem = dbase.get<int>("numElem");
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");

  //Longfei 20160308: use fd approximations to give g1,g2,g3 for FDBeamModel. 
  // this reduce kinks in the ghost points (source of the wiggles in some situations)

  
  if( g.getLength(0)==0 ) g.redim(4,2); 
  
  g=0.;
  
  if( !allowsFreeMotion )
    {
      const real y=0, z=0;
      const int wc=0;
      // Longfei 20160122: new way
      const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
      for( int side=0; side<=1; side++ )
	{
	  //BoundaryCondition bc = side==0 ? bcLeft : bcRight;
	  const  BoundaryCondition bc = boundaryConditions[side];
	  //int ia = side==0 ? 0 : numElem*2;
	  real x = side==0 ? 0 : L;

	  if( bc==periodic || bc==unknownBC || bc==internalForceBC )
	    {
	    }
	  else if(bc!=pinned && bc!=clamped && bc!=freeBC && bc!=slideBC)
	    {
	      OV_ABORT("ERROR - unknown bc");
	    }
	  else
	    {
	      // Longfei 20160308: regrouped below according to given, u(c,p),u.x(c,s),u.xx(p,f) and u.xxx(f,s)
	      // u=g
	      if( bc==clamped || bc==pinned ) 
		{
		  // --- give g0=exact.u ---
		  if( twilightZone )
		    {
		      g(0,side) = exact.gd(ntd,0,0,0, x,y,z,wc,t);  // Give w 
		    }
		  else
		    {
		      g(0,side)=0.;
		    }
		}

	      // u.x=g
	      if( bc==clamped || bc==slideBC  ) 
		{
	      
		  // --- give g1=exact.ux ---
		  if( twilightZone )
		    {
		      g(1,side) = exact.gd(ntd  ,1,0,0, x,y,z,wc,t);    // Give w.x 
		    }
		  else
		    {
		      g(1,side)=0.;
		    }
		}

	      // u.xx=g
	      if( bc==pinned || bc==freeBC) 
		{
		  // --- give g2=exact.uxx  ---
		  if( twilightZone )
		    {
		      g(2,side) = exact.gd(ntd,2,0,0,  x,y,z,wc,t);   // Give EI*w_xx 
     
		    }
		  else
		    {
		      g(2,side)=0.;
		    }
		}

	      // u.xxx=g
	      if( bc==freeBC || bc==slideBC ) 
		{
		  // --- give g3=exact.uxxx for FEM, g3 = D0(D+D-)exact.u for FD ---
		  if( twilightZone )
		    {
		      if(isFEM)
			{
			  // provide exact uxxx for FEM tz tests
			  g(3,side) = exact.gd(ntd,3,0,0,  x,y,z,wc,t);   // Give EI*w_xxx
			}
		      else
			{
			  //Longfei 20160308:  
			  //give g3 from D0(D+D-)g, this reduces kinks in the ghost points
			  const real & dx = dbase.get<real>("elementLength");
			  const real & dx3=dx*dx*dx;
			  g(3,side) =( -0.5*exact.gd(ntd,0,0,0, x-2*dx,y,z,wc,t)
				       +exact.gd(ntd,0,0,0, x-dx,y,z,wc,t)
				       -exact.gd(ntd,0,0,0, x+dx,y,z,wc,t)
				       +0.5*exact.gd(ntd,0,0,0, x+2*dx,y,z,wc,t))/dx3;
			}
		      // old: use exact u.xxx for all
		      // g(3,side) = exact.gd(ntd,3,0,0,  x,y,z,wc,t);   // Give EI*w_xxx
		    }
		  else
		    {
		      g(3,side)=0.;
		    }
		}

	    }

      
	}
    }

  
  return 0;
}



//  =========================================================================================
/// \brief Assign boundary conditions for u,v,a on cubic hermite space.
///        FDBeamModel::assignBoundaryConditions overwrites this function
///        This function is called for projection of surface quantities or FEMBeamModel solves
/// f is needed for compatibility BC
//  =========================================================================================
int BeamModel::
assignBoundaryConditions( real t, RealArray & u, RealArray & v, RealArray & a, const RealArray & f)
{
  // Longfei 20160120:
  //const real EI = elasticModulus*areaMomentOfInertia;
  const real & EI = dbase.get<real>("EI");
  const real & L = dbase.get<real>("length");
  const int & numElem = dbase.get<int>("numElem");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft =  boundaryConditions[0];
  const BoundaryCondition & bcRight =  boundaryConditions[1];
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  
  if( !allowsFreeMotion )
    {
      RealArray ue,ve,ae;
      bool assignExact=false;
      if( bcLeft!=periodic && exactSolutionOption=="travelingWaveFSI" )
	{
	  assignExact=true;
	  //getTravelingWaveFSI( t, ue, ve, ae  );  // this is inefficient, we only need to evaluate at boundary nodes
	  getExactSolution( t, ue, ve, ae  );
	}

      for( int side=0; side<=1; side++ )
	{
	  BoundaryCondition bc = side==0 ? bcLeft : bcRight;
	  int ia = side==0 ? 0 : numElem*2;

	  // Special case when EI=0 : (we only have 1 BC for clamped instead of 2)
	  if( bc==clamped && EI==0. ) bc=pinned;
	  // for Hermite FEM: solutions are u = [u0[0],ux0[0],u0[1],ux0[1],...]
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
/// Compute the nodal values of the forces, for FEM beam, the x derivatives of the forces are computed as well
// =========================================================================================
void BeamModel::
addInternalForces( const real t, RealArray & f )
{
  //Longfei 20160121: new way of handling parameters
  //const real beamLength=L;
  const real &beamLength = dbase.get<real>("length");
  const real &dx = dbase.get<real>("elementLength");
  //const real EI = elasticModulus*areaMomentOfInertia;
  const real & EI = dbase.get<real>("EI");
  const real & Abar = dbase.get<real>("massPerUnitLength");
  const int & numElem = dbase.get<int>("numElem");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");
  const real & buoyantMassPerUnitLength = dbase.get<real>("buoyantMassPerUnitLength");
  // const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  // const BoundaryCondition & bcLeft = boundaryConditions[0];
  // const BoundaryCondition & bcRight = boundaryConditions[1];
  const real & projectedBodyForce= dbase.get<real>("projectedBodyForce");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");

  f=0.; //zero out 

  // Longfei 20160214: modification done to make sure work  for both FEM and FD method
  Index I1,I2,I3;
  I1=Range(-numOfGhost,numElem+numOfGhost); I2=0; I3=0; // index for all nodes on the beam domain
  Index J1;
  if(isFEM)
    J1 = 2*I1;  // [0,2,4,...,2*numElem]  for Cubic Hermite FEM results
  else
    J1= I1;   //[0,1,2,....,numElem] for FD results
  
  
  RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
  real heightFluidRegion=1.;
  for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
    {
      x(i1,0,0,0) = i1*dx; 
      x(i1,0,0,1) = heightFluidRegion;    // should match value in travelingWaveFsi
    }
  
  
  if( exactSolutionOption=="travelingWaveFSI" )
    {
      // add forces for the FSI traveling wave solution
    
      assert( dbase.get<TravelingWaveFsi*>("travelingWaveFsi")!=NULL );
      TravelingWaveFsi & travelingWaveFsi = *dbase.get<TravelingWaveFsi*>("travelingWaveFsi");

      RealArray ufe(I1,I2,I3,3);  // holds (p,v1f,v2f)

      // Evaluate the exact fluid solution on the interface
      travelingWaveFsi.getExactFluidSolution( ufe, t, x, I1,I2,I3 );
      const int pc=0;
      f(J1,I2,I3,0) =  ufe(I1,I2,I3,pc);
      // ::display(ufe(I1,0,0,0),sPrintF(" Exact fluid pressure at t=%8.2e",t),"%8.2e ");

      // Longfei 20160214: do this in the FEM version
      // RealArray lt(4); // local traction
      // const int pc=0;
      // for ( int i = 0; i<numElem; i++ )
      // 	{
      // 	  real p0=ufe(i,0,0,pc), p1=ufe(i+1,0,0,pc);
      // 	  computeProjectedForce( p0,p1, -1.0,1.0, lt);
      // 	  Index idx(i*2,4);
      // 	  f(idx) += lt;
      // 	}

    }
  

  const int & domainDimension = dbase.get<int>("domainDimension");
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  if( twilightZone )
    {
      OGFunction & exact = *dbase.get<OGFunction*>("exactPointer");
      // RealArray x(I1,I2,I3,2);  // beam axis (undeformed)
      // //const real dx=beamLength/numElem; // Longfei 20160120: dx  already defined in this function
      // for( int i1 = I1.getBase(); i1<=I1.getBound(); i1++ )
      // 	{
      // 	  x(i1,0,0,0) = i1*dx; 
      // 	  x(i1,0,0,1) = 0.;    // should this be y0 ?
      // 	}
      x(I1,I2,I3,1) = 0.;  //y=0


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

      f(J1,I2,I3,0) = Abar*utte + K0*ue - (T)*uxxe + (EI)*uxxxxe;

      if( isFEM )
	{
	  RealArray uxe(I1,I2,I3,1), uttxe(I1,I2,I3,1), uxxxe(I1,I2,I3,1), uxxxxxe(I1,I2,I3,1);
	  exact.gd( uxe     ,x,domainDimension,isRectangular,0,1,0,0,I1,I2,I3,wc,t );
	  exact.gd( uttxe   ,x,domainDimension,isRectangular,2,1,0,0,I1,I2,I3,wc,t );
	  exact.gd( uxxxe   ,x,domainDimension,isRectangular,0,3,0,0,I1,I2,I3,wc,t );
	  exact.gd( uxxxxxe ,x,domainDimension,isRectangular,0,5,0,0,I1,I2,I3,wc,t );

	  f(J1+1,I2,I3,0) = Abar*uttxe + K0*uxe - (T)*uxxxe + (EI)*uxxxxxe;  // x-derivative of the TZ force
	}
    
      if( Kt!=0. )
	{
	  RealArray & ute = uxxe;  // re-use space
	  exact.gd( ute, x,domainDimension,isRectangular,1,0,0,0,I1,I2,I3,wc,t );
	  f(J1,I2,I3,0) += Kt*ute;
	  if( isFEM )
	    {
	      RealArray & utxe = uxxe;  // re-use space
	      exact.gd( utxe, x,domainDimension,isRectangular,1,1,0,0,I1,I2,I3,wc,t );
	      f(J1+1,I2,I3,0) += Kt*utxe;
	    }
	}
      if( Kxxt!=0. )
	{
	  RealArray & utxxe = uxxe;  // re-use space
	  exact.gd( utxxe, x,domainDimension,isRectangular,1,2,0,0,I1,I2,I3,wc,t );
	  f(J1,I2,I3,0) += (-Kxxt)*utxxe;
	  if(isFEM)
	    {
	      RealArray & utxxxe = uxxe;  // re-use space
	      exact.gd( utxxxe, x,domainDimension,isRectangular,1,3,0,0,I1,I2,I3,wc,t );
	      f(J1+1,I2,I3,0) += (-Kxxt)*utxxxe;
	    }
      
	}
    
      // ::display(utte,"utte","%8.2e ");
      // ::display(uxxe,"uxxe","%8.2e ");
      // ::display(ftz,"ftz","%8.2e ");
    
      // RealArray lt(4); // local traction
      // for ( int i = 0; i<numElem; i++ )
      // 	{
      // 	  // computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);

      // 	  if( orderOfGalerkinProjection==2 )
      // 	    computeProjectedForce( ftz(i),ftz(i+1), -1.0,1.0, lt);
      // 	  else
      // 	    computeGalerkinProjection( ftz(i),ftzx(i), ftz(i+1),ftzx(i+1),   -1.0,1.0, lt);

      // 	  Index idx(i*2,4);
      // 	  f(idx) += lt;
      // 	}
   
    }

  if( projectedBodyForce*buoyantMassPerUnitLength!=0. )
    {

      f= projectedBodyForce*buoyantMassPerUnitLength; //Longfei 20160214:  f is a  constant in this case
      // --- add buyouncy force
      // RealArray lt(4);
      // for (int i = 0; i < numElem; ++i) 
      // 	{
      // 	  // -- compute (N_i, . )
      // 	  computeProjectedForce(projectedBodyForce*buoyantMassPerUnitLength,projectedBodyForce*buoyantMassPerUnitLength,
      // 				-1.0,1.0, lt);
      // 	  Index idx(i*2,4);
      // 	  f(idx) += lt;
      // 	}
    }

  // Longfei 20160214: do i need to do anything here for periodic bc???
  
  // const bool isPeriodic = bcLeft==periodic;
  // if( isPeriodic )
  //   {
  //     Index Is(0,2), Ie(2*numElem,2);
  //     f(Is) += f(Ie);
  //     f(Ie) = f(Is);
  //   }
  
  

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
  // Longfei: predictor should not be responsible to construct the matrix for tri solvers. 
  
  //Longfei 20160121: new way of handling parameters
  const real & L = dbase.get<real>("length");
  const int & numElem = dbase.get<int>("numElem");
  // Longfei 20160210: no longer need matrices here
  // const RealArray & elementK = *dbase.get<RealArray*>("elementK");
  // const RealArray & elementM = *dbase.get<RealArray*>("elementM");
  // const RealArray & elementB = *dbase.get<RealArray*>("elementB");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const real & newmarkBeta = dbase.get<real>("newmarkBeta");
  const real & newmarkGamma = dbase.get<real>("newmarkGamma");
  const BoundaryCondition * boundaryConditions = dbase.get<BoundaryCondition[2]>("boundaryConditions");
  const BoundaryCondition & bcLeft = boundaryConditions[0];
  const BoundaryCondition & bcRight = boundaryConditions[1];
  // const bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor"); // old way for time stepping
  // Longfei 20160131: new way for timestepping:
  const TimeSteppingMethod & predictorMethod = dbase.get<TimeSteppingMethod>("predictorMethod");
  const TimeSteppingMethod & correctorMethod = dbase.get<TimeSteppingMethod>("correctorMethod");
  const bool & twilightZone = dbase.get<bool>("twilightZone");
  // const bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor"); // Longfei: removed, old way
  const bool & relaxForce = dbase.get<bool>("relaxForce");
  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const bool & useExactSolution = dbase.get<bool>("useExactSolution");
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const real & Abar =dbase.get<real>("massPerUnitLength");

  
  
  bool & refactor = dbase.get<bool>("refactor");
  int & current = dbase.get<int>("current"); 
  int & numberOfTimeSteps = dbase.get<int>("numberOfTimeSteps");
  bool & hasAcceleration = dbase.get<bool>("hasAcceleration");
  
  // -- set current to point to the new time level t^{n+1}
  const int prev = current; //  prev points to solutions at t^n
  current = ( current + 1 ) % numberOfTimeLevels;
  const int prev2 = ( prev -1 + numberOfTimeLevels) % numberOfTimeLevels; // points to solution at t^{n-1} 

  RealArray & time = dbase.get<RealArray>("time");
  real &t=time(current); //for backward compatibility since t used to be a class member

  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  //Longfei 20160203: needed by leap-frog
  RealArray & x1 = u[prev2];
  RealArray & v1 = v[prev2];
  RealArray & a1 = a[prev2];
  RealArray & f1 = f[prev2];

  RealArray & x2 = u[prev];
  RealArray & v2 = v[prev];
  RealArray & a2 = a[prev];
  RealArray & f2 = f[prev];
  
  RealArray & x3 = u[current];
  RealArray & v3 = v[current];
  RealArray & a3 = a[current];
  RealArray & f3 = f[current];

  t =time(prev)+dt;  // new time 


  if( fabs(time(current)-tnp1) > 1.e-10*(1.+tnp1) )
    {
      printF("-- BM%i -- BeamModel::predictor:ERROR: tnp1=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),tnp1,time(current),current);
      OV_ABORT("ERROR");
    }


  if( false && t<2.*dt )
    { 
      // wdh: debug info: 
      printF("************** BeamModel::predictor: t=%9.3e ***********************\n",t);
      ::display(x2,"x2","%8.2e ");
      ::display(v2,"v2","%8.2e ");
    }
  if( debug() & 4 )
    {
      ::display(f2,"BeamModel::predictor: RHS force f2 BEFORE addInternalForces","%8.2e ");
    }

  if( false )
    {
      // do not need to apply smoother to at predictor
      smooth( t-dt,x2, "u: predictor" );
      smooth( t-dt,v2, "v: predictor" );
      smooth( t-dt,a2, "a: predictor" );
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

  if( debug() & 4 )
    {
      ::display(f2,"BeamModel::predictor: RHS force f2","%8.2e ");
    }
  

  if( !hasAcceleration ) 
    {
      // On the very first time-step we may not know the acceleration at t=0.
      //refactor=true;  // massMatrixSolver is time-independent, no need to refactor it          
      // compute acceleration at time tn=t-dt 
      // old:
      // const real alpha =0.;  // coeff of K in  (M + alphaB*B + alpha*K)*a = RHS
      // const real alphaB=0.;  // coeff of B in  (M + alphaB*B + alpha*K)*a = RHS
      // computeAcceleration(t-dt, x2,v2,f2, elementM, a2,
      // 			  centerOfMassAcceleration, angularAcceleration,
      // 			  dt, alpha,alphaB, "tridiagonalSolver" );
      // Longfei 20160209: new
      // explicit solver solves M (Abar*a) = RHS, M=massMatrix for FEMBeam, M=I for FDBeam
      computeAcceleration(t-dt, x2,v2,f2, a2,centerOfMassAcceleration, angularAcceleration,dt, "explicitSolver" );
      hasAcceleration = true;
    }


  if( debug() & 4 )
    {
      ::display(a2,"-- BM -- predictor: a2","%8.2e ");
      ::display(f2,"-- BM -- predictor: f2","%8.2e ");
      ::display(x2,"-- BM -- predictor: u2","%8.2e ");
      ::display(v2,"-- BM -- predictor: v2","%8.2e ");
    }

  const real dtOld = dbase.get<real>("dt"); 
  if( fabs(dt-dtOld) > REAL_EPSILON*10.*dt )
    {
      refactor=true;
      printF("-- BM%i -- predictor: dt has changed, dt=%9.3e, dtOld=%9.3e, will refactor.\n",getBeamID(),dt,dtOld);
    }
  dbase.get<real>("dt")=dt; // adjust "dtOld"



  
  // predict force f3. Implicit Solver and compatibility bc needs force at new time
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


  
  // Predicted position/velocity of the beam
  RealArray &dtilde = dbase.get<RealArray>("dtilde");
  RealArray &vtilde = dbase.get<RealArray>("vtilde");
  
  if( predictorMethod==newmark1 || predictorMethod==newmark2Implicit || predictorMethod==newmark2Explicit || correctorMethod==newmarkCorrector) 
    {
      // newmarks schemes need dtilde and vtilde
      //  -- first order predictor --
      dtilde = x2 + dt*v2 + (dt*dt*0.5*(1.0-2.0*newmarkBeta))*a2;
      vtilde = v2 + dt*(1.0-newmarkGamma)*a2;

      //Longfei 20160628: implicitNewmarkSolver need valid ghost values for dtilde,vtilde
      assignBoundaryConditions( tnp1,dtilde,vtilde,a3,f3); 
    }

  
  // -- here are the predicted u and v:

  real linaccel[2],omegadd; // needed for computeAcceleration

  // -- implicit methods:
  if( predictorMethod==newmark2Implicit )
    {
      // -- The implicit predictor makes a guess for the forcing at time tnp1 = t+dt  ---

      if( debug() & 2 )
	printF("-- BM%i -- use implicit predictor tnp1=%8.2e\n",getBeamID(),tnp1);



      // old:
      // compute acceleration at time t^{n+1}
      // const real alpha =newmarkBeta*dt*dt;  // coeff of K in A
      // const real alphaB=newmarkGamma*dt;    // coeff of B in A
      // RealArray A; 
      // A = elementM + alpha*elementK;
      // if( addDampingMatrix )
      // 	{ // add damping matrix B 
      // 	  A += alphaB*elementB;
      // 	}
      

      // old:
      //computeAcceleration( tnp1, dtilde,vtilde,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver" );



      // Longfei 20160209: new
      computeAcceleration( tnp1, dtilde,vtilde,f3, a3,  linaccel,omegadd,dt,"implicitNewmarkSolver" );     

      v3 = vtilde + (newmarkGamma*dt)*a3;
      x3 = dtilde + (newmarkBeta*dt*dt)*a3;
      
      //Longfei 20160630: implement bc for the predicted values
      assignBoundaryConditions( tnp1, x3,v3,a3,f3);


      if( false && t<=2.*dt )
	{
	  printF("-- BM%i -- predictor: tnp1=%9.3e\n",getBeamID(),t,tnp1);
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
	  printF("-- BM%i -- t=%8.2e: predicted: |vpi-vpe|=%8.2e, |xpi-xpe|=%8.2e |a2-a3|=%8.2e\n",
		 getBeamID(),t,max(fabs(v3-vpe)), max(fabs(x3-xpe)), max(fabs(a3-a2)));
      
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
  // --explicit methods:
  else
    {
  
      if(predictorMethod==leapFrog) // use leapFrog predictor
	{
	  if( debug() & 2 )
	    printF("-- BM%i -- use leapFrog predictor tnp1=%8.2e\n",getBeamID(),tnp1);
	  //Longfei 20160204: note this is only first order predictor if dt!=dtOld
	  if( tnp1 >= 1.5*dt ) // 
	    {
	      x3=x1+(dt+dtOld)*v2;
	      v3=v1+(dt+dtOld)*a2;
	    }
	  else
	    {
	      // -- we only have 1 old solutions
	      // We know pastTimeSolution from exact solutions
	      x3=x2+dt*v2;
	      v3=v2+dt*a2;
	    }
	}
      else if(predictorMethod==adamsBashforth2)
	{

	  if( debug() & 2 )
	    printF("-- BM%i -- use adamsBashforth2 predictor tnp1=%8.2e\n",getBeamID(),tnp1);
	  real ab1,ab2;
	  if( tnp1 >= 1.5*dt ) 
	    {
	      // 2nd -order predictor
	      ab1= dt*(1.+dt/(2.*dtOld));  // becomes 1.5*dt0  if dt0==dtb
	      ab2= -dt*dt/(2.*dtOld);      //         -.5*dt0
	    }
	  else
	    {
	      // -- we only have 1 old solutions
	      // first order predictor// fix me for tzTests. We know pastTimeSolution from exact solutions
	      ab1=dt;
	      ab2=0.;
	    }
	  x3=x2+ab1*v2+ab2*v1;
	  v3=v2+ab1*a2+ab2*a1;
	}
 
      else if( predictorMethod==newmark2Explicit)
	{
	  // ???Longfei: is this second order?
	  //  x and v are second order.  Acceleration is corrected to 2nd order in corrector step
	  x3 = x2 + dt*v2+ (.5*dt*dt)*a2;
	  v3 = v2 + dt*a2;
	}
      else if ( predictorMethod==newmark1)
	{ //  -- use first order predictor
	  x3 = dtilde;
	  v3 = vtilde;
	}
      else
	{
	  OV_ABORT("Error: unsupported time-stepping method");
	}
      
      // Longfei 20160630: we should have valid ghost point values before compute acceleration using explicit methods
      assignBoundaryConditions( tnp1, x3,v3,a3,f3);
  
      //predict acceleration for all explicit predictors:
      //Longfei: note that computeAcceleration takes care of the acceleration boundary conditions
      computeAcceleration(tnp1, x3,v3,f3, a3,  linaccel,omegadd,dt,"explicitSolver" );
      
    }
  // *wdh* aold = 0.0;
  RealArray & fOld = dbase.get<RealArray>("fOld");
  RealArray & aold = dbase.get<RealArray>("aOld");

  if( relaxForce )
    {
      aold = a2;   // set aold to previous acceleration *wdh* 2014/06/19 
      fOld = f2;
      // aold=0.;  // **** TEST
    }
  


  // if( false ) // do not apply BC's to first-order predictor 
  //   assignBoundaryConditions( tnp1, dtilde,vtilde,a3 );

  if (allowsFreeMotion) 
    {
      // assert( !useSecondOrderNewmarkPredictor );
      assert(predictorMethod==newmark1);
	
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


  if( debug() & 2 )
    {
      aString buff;
      getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM%i --: after predict t=%9.3e",getBeamID(),tnp1));
    }
  


  const bool & saveProfileFile = dbase.get<bool>("saveProfileFile");

  if( useExactSolution && saveProfileFile ) 
    {
      RealArray xtmp = x3;
      RealArray vtmp = v3;
      RealArray atmp = a2;

      // Longfei 20160120: setExactSolution is removed
      //setExactSolution(t,xtmp,vtmp,atmp);
      getExactSolution(t,xtmp,vtmp,atmp);

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

  dbase.get<int>("numCorrectorIterations") = 0;  //reset numCorrectorIterations here

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
  const int & numElem = dbase.get<int>("numElem");
  real uNorm=0;
  
  //Longfei 20160624: fix idx for FDBeamModel
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");
  int idxFactor=1;
  if(isFEM)
    idxFactor=2;
  for (int i = 0; i < numElem; ++i) 
    {
      // norm of | a3 - aold |   *wdh: should we scale by dx ?  
   
      uNorm += u(i*idxFactor)*u(i*idxFactor);

    }

  if(debug() & 8)
    {
      ::display(u,"u in norm","%10.3e");
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
///  The acceleration is compute from 
///      ( M + dt^2*beta*K ) anp1 = fnp1 - K*[ un + dt*vn + (dt^2/2)*(1-2*beta)*an ]
///      
// ===================================================================================
void BeamModel::
corrector(real tnp1, real dt )
{
  //Longfei 20160121: new way of handling parameters
  const real & le = dbase.get<real>("elementLength");
  const int & numElem = dbase.get<int>("numElem");
  const real & buoyantMass= dbase.get<real>("buoyantMass"); 
  const real & totalMass=  dbase.get<real>("totalMass");
  // Longfei 20160210: no longer need matrices here
  // const RealArray & elementK = *dbase.get<RealArray*>("elementK");
  // const RealArray & elementM = *dbase.get<RealArray*>("elementM");
  // const RealArray & elementB = *dbase.get<RealArray*>("elementB");
  const real & newmarkBeta = dbase.get<real>("newmarkBeta");
  const real & newmarkGamma = dbase.get<real>("newmarkGamma");
  const bool & relaxForce = dbase.get<bool>("relaxForce");
  const bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor");
  const bool addDampingMatrix = dbase.get<real>("Kt")!=0. || dbase.get<real>("Kxxt")!=0.;
  const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
  const int & current = dbase.get<int>("current");
  const int & numberOfTimeSteps = dbase.get<int>("numberOfTimeSteps");
  const bool & allowsFreeMotion = dbase.get<bool>("allowsFreeMotion");
  const real & Abar = dbase.get<real>("massPerUnitLength");

  // Longfei 20160205: new way
  const bool & smoothSolution = dbase.get<bool>("smoothSolution");
  const TimeSteppingMethod & correctorMethod = dbase.get<TimeSteppingMethod>("correctorMethod");

  //Longfei 20160210: 
  //In some cases, corrector is called first, make sure the beamModel knows what the dt is now. 
  //dbase.get<real>("dt") is used to factor implicitNewmarkSolver
  real & dtBeam = dbase.get<real>("dt");
  if(dtBeam<0) 
    {
      dtBeam=dt;
    }



  int  & numCorrectorIterations = dbase.get<int>("numCorrectorIterations"); 
  bool & correctionHasConverged = dbase.get<bool>("correctionHasConverged");
  const RealArray & time = dbase.get<RealArray>("time"); // Longfei 20160210: make this const. corrector should not change time
  const real &t=time(current); //for backward compatibility since t used to be a class member
  std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
  std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
  std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  RealArray & x3 = u[current];
  RealArray & v3 = v[current];
  RealArray & a3 = a[current];
  RealArray & f3 = f[current];  // force at new time



  real & initialResidual = dbase.get<real>("initialResidual");
  
  if( fabs(time(current)-tnp1) > 1.e-10*(1.+tnp1) )
    {
      printF("-- BM%i -- BeamModel::corrector:ERROR: tnp1=%10.3e is not equal to time(current)=%10.3e, current=%i\n",
	     getBeamID(),tnp1,time(current),current);
      OV_ABORT("ERROR");
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
      // fOld.redim(2*(numElem+1));
      fOld.redim(f3); //Longfei 20160329: use the size of f3
      fOld=0.;
    }
  
  if( relaxForce && relaxCorrectionSteps ) //Longfei 20160617: call this only when relaxCorrectionSteps==true
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
      const real * bodyForce=dbase.get<real[2]>("bodyForce");
      correctionHasConverged = true;
      centerOfMassAcceleration[0] = buoyantMass / totalMass * bodyForce[0];
      centerOfMassAcceleration[1] = buoyantMass / totalMass * bodyForce[1];

      return;
    }

  // Predicted position/velocity of the beam
  const RealArray &dtilde = dbase.get<RealArray>("dtilde");
  const RealArray &vtilde = dbase.get<RealArray>("vtilde");
      
  real linaccel[2],omegadd;
  // old:
  // real alpha=0., alphaB=0.;
  // RealArray A=elementM;

  // recompute acceleration at time t^{n+1} with updated force
  if(correctorMethod==newmarkCorrector)
    {
      // old: now solver knows the matrix, no need to pass the matrix in
      // alpha=newmarkBeta*dt*dt;   // coeff of K in A
      // alphaB=newmarkGamma*dt;    // coeff of B in A
      // A+= alpha*elementK;
      // if( addDampingMatrix )
      // 	{ // add damping matrix B 
      // 	  A += alphaB*elementB;
      // 	} 
      //Longfei 20160203: v3 should be vtilde
      // for newmark: (M+alpha*K+alphaB*B)*a^{n+1} = f^{n+1}-K*dtilde-B*vtilde
      //computeAcceleration(tnp1, dtilde,vtilde,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver");

      //Longfei 20160209: new
      computeAcceleration(tnp1, dtilde,vtilde,f3, a3,  linaccel,omegadd,dt,"implicitNewmarkSolver");
    }
  else if(correctorMethod==adamsMoultonCorrector)
    {
      // for AM2: M*a^{n+1} = f^{n+1}-K*x^{p}-B*v^{p}
      //computeAcceleration(tnp1, x3,v3,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver");

      computeAcceleration(tnp1, x3,v3,f3, a3,  linaccel,omegadd,dt,"explicitSolver" );

    }
  else
    {
      OV_ABORT("Error: unknonw beam corrector");
    }
  // computeAcceleration(tnp1, x3,v3,f3, A, a3,  linaccel,omegadd,dt, alpha, alphaB,"tridiagonalSolver");

  //v3 = vtilde+newmarkGamma*dt*(myAcceleration-aold)*omega;
  //x3 = dtilde+newmarkBeta*dt*dt*(myAcceleration-aold)*omega;

  // // under-relax the acceleration
  // Longfei 20160122:
  RealArray & aold = dbase.get<RealArray>("aOld");
  
  if( !relaxForce )
    {
      if( relaxCorrectionSteps )
	a3 = omega*a3+(1.0-omega)*aold;    
    }

  //Longfei 20160204: new time stepping methods
  // use  use second order Adams-moulton corrector
  if(correctorMethod==adamsMoultonCorrector)
    {
      if( debug() & 2 )
	printF("-- BM%i -- use second order Adams-Moulton corrector tnp1=%8.2e\n",getBeamID(),tnp1);

      const int prev = ( current -1 + numberOfTimeLevels) % numberOfTimeLevels; // points to solution at t^{n} 
      const RealArray & x2 = u[prev];
      const RealArray & v2 = v[prev];
      const RealArray & a2 = a[prev];

      const real am1=0.5*dt;
      const real am2=0.5*dt;
      v3 = v2+am1*a3+am2*a2;
      x3 = x2+am1*v3+am2*v2;
      //Longfei 20160630: we need valid ghost values for v3 and x3 before compute acceleration
      assignBoundaryConditions( t,x3,v3,a3,f3);
      
      computeAcceleration(tnp1, x3,v3,f3, a3,  linaccel,omegadd,dt,"explicitSolver" );	    	    
    }
  else if(correctorMethod==newmarkCorrector)
    {
      if( debug() & 2 )
	printF("-- BM%i -- use implicit second order newmark corrector tnp1=%8.2e\n",getBeamID(),tnp1);


      v3 = vtilde+newmarkGamma*dt*a3;
      x3 = dtilde+newmarkBeta*dt*dt*a3;
      
      //Longfei 20160630: apply bc for corrected values
      assignBoundaryConditions( t,x3,v3,a3,f3);
      
    }
  if( debug() & 4 )
    {
      //int nn=numElem+1;
      //v3.reshape(2,nn); x3.reshape(2,nn); a3.reshape(2,nn); f3.reshape(2,nn);
      printF("-- BM%i -- corrector: tnp1=%9.3e, dt=%9.3e\n",getBeamID(),tnp1,dt);
      ::display(f3,"-- BM -- corrector: f3","%8.2e ");
      ::display(x3,"-- BM -- corrector: u3","%8.2e ");
      ::display(v3,"-- BM -- corrector: v3","%8.2e ");
      ::display(a3,"-- BM -- corrector: a3","%8.2e ");
      //v3.reshape(2*nn); x3.reshape(2*nn); a3.reshape(2*nn); f3.reshape(2*nn);
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
      if( true && (debug() & 2) )
	{
	  printF("-- BM%i -- TP-iteration: omega=%6.3f, iter=%i, corr=%8.2e rel-corr=%8.2e, rtol=%8.2e, atol=%8.2e converged=%i\n",
		 getBeamID(),omega,numCorrectorIterations,correction,maximumRelativeCorrection, subIterationConvergenceTolerance,
		 subIterationAbsoluteTolerance, (int)correctionHasConverged );
	}
    
    }


  // optionally smooth the solution:
  if( smoothSolution )
    {
      smooth( t,x3, "u: corrector" );
      smooth( t,v3, "v: corrector" );
      smooth( t,a3, "a: corrector" );
      // re-apply bc for the smoothed solutions
      assignBoundaryConditions( t,x3,v3,a3,f3);
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
  

  if( debug() & 2 )
    {
      aString buff;
      getErrors( tnp1, x3,v3,a3,sPrintF(buff,"-- BM%i -- : after correct t=%9.3e",getBeamID(),tnp1));
    }

}





// Longfei 20160622: made this function pure virtual. No longer need this
// void  BeamModel::
// computeInternalForce( const RealArray& u, const RealArray& v, RealArray& f )
// {
//   // implementation moved to FEMBeamModel
//   OV_ABORT("Error: BeamModel::computeInternalForce base version called");
// }



// Longfei 20160329: made this pure virtual function, no need to implement here in base class
// Compute the acceleration of the beam.
// void BeamModel::
// computeAcceleration(const real t,
// 		    const RealArray& u, const RealArray& v, 
// 		    const RealArray& f,
// 		    RealArray& a,
// 		    real linAcceleration[2],
// 		    real& omegadd,real dt,
// 		    const aString & solverName )
// {
//   OV_ABORT("Error: BeamModel::computeAcceleration base version called");
// }




// Longfei 20160622: getForceOnBeam is pure virtual now. Do not need this anymore
// void BeamModel::
// getForceOnBeam( const real t, RealArray & force )
// {
//   // implementation moved to FEMBeamModel
//   OV_ABORT("Error: BeamModel::getForceOnBeam base version called");
// }

// ====================================================================================
/// \brief Output probe info.
/// \param t (input) : current time
/// \param stepNumber (input) : global time step number 
// ====================================================================================
int BeamModel::
outputProbes( real t, int stepNumber )
{

  //Longfei 20160121: new way of handling parameters
  const real & L = dbase.get<real>("length");
  const real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  const real & beamX0 = beamXYZ[0];
  const real & beamY0 = beamXYZ[1];
  const real & beamZ0 = beamXYZ[2];
  const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
  const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");

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

      if( debug() & 2 )
	printf("-- BM%i -- save probe: t=%9.3e (xp0,yp0)=(%9.3e,%9.3e) (up,vp,ap)=(%9.3e,%9.3e,%9.3e) elemNum=%i eta=%9.3e\n",
	       getBeamID(),t,xp0,yp0,up,vp,ap,elemNum,eta);
      
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
/// \brief Return current displacement DOF's
// Longfei 20160120: renamed position to displacement
// ====================================================================================
const RealArray& BeamModel::
displacement() const  
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
  return  dbase.get<bool>("correctionHasConverged");
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


// Longfei 20160120: this function seems unused. Remove
// void BeamModel::setSubIterationConvergenceTolerance(double tol) 
// {
//   dbase.get<real>("subIterationConvergenceTolerance") = tol;
// }


// =================================================================================================
/// \brief  Compute errors in the CURRENT solution (when the solution is known).
/// \param label (input) : label for output.
/// \param file (input) : write output to this file.
/// \param uvErr[3] (output) :  maximum errors for u,v,a
/// \param uvNorm[3] (output) : norm of the solution
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
/// \param uvErr[3] (output) :  maximum errors for u,v,a
/// \param uvNorm[3] (output) : norm of the solution
// =================================================================================================
int BeamModel::
getErrors( const real t, const RealArray & u, const RealArray & v, const RealArray & a,
           const aString & label,
           FILE *file /* = stdout */,
           real *uvErr /* = NULL */, real *uvNorm /* = NULL */ )
{
  // Longfei 20160121: new way of handling parameters
  //const real beamLength = L;
  const int & numberOfTimeSteps = dbase.get<int>("numberOfTimeSteps");
  
  RealArray ue, ve, ae;
  getExactSolution( t, ue, ve, ae );

  const real dx=dbase.get<real>("elementLength");
  const bool xd = dbase.get<bool>("isCubicHermiteFEM"); // x derivative?
  const int & numElem = dbase.get<int>("numElem");
  const int & numGhost = dbase.get<int>("numberOfGhostPoints");
  Index I1 = Range(-numGhost,numElem+numGhost); // index of all nodes (include ghost nodes if any)
  
  if( file !=NULL && debug() & 2 )
    fPrintF(file,"-- BM%i -- %s: Errors at t=%9.3e:\n",getBeamID(),(const char*)label,t);
  real uErr=0., uNorm=0.; // ul2err=0. 
  real vErr=0., vNorm=0.;
  //Longfei 20160301: add aErr and aNorm
  real aErr=0., aNorm=0.;
  
  
  //Longfei 20160118: loop over interior and boundary nodes only.
  // we might need options to turn on ghost errors
  for (int i = 0; i <= numElem; ++i)
    {
      int si = xd? 2*i : i; // the solution index (si) at node i is 2*i for FEM, otherwise is i; 
      
      real erru = fabs( u(si) -  ue(si) );
      uErr=max(uErr,erru);
      uNorm=uNorm+SQR(ue(si));  //???Longfei 20160125: why evaluate the norm for exact solutions?
      //ul2err += SQR(erru); // Longfei: not used anymore

      real errv = fabs( v(si) - ve(si) );
      vErr=max(vErr,errv);
      vNorm=vNorm+SQR(ve(si));  //???Longfei 20160125: why evaluate the norm for exact solutions?

      //Longfei 20160301: add errors for a
      real erra = fabs( a(si) - ae(si) );
      aErr=max(aErr,erra);
      aNorm=aNorm+SQR(ae(si));  //???Longfei 20160125: why evaluate the norm for exact solutions?
      

      if( FALSE && debug() & 2 )
	printF("-- BM%i -- t=%8.2e i=%3i u=%9.2e ue=%9.2e err=%9.2e, v=%9.2e ve=%9.2e err=%8.2e,  a=%9.2e ae=%9.2e err=%8.2e\n",
	       getBeamID(),t,i,u(si),ue(si),erru,v(si),ve(si),errv,a(si),ae(si),erra);

    }
  
  // ul2err=sqrt(ul2err/(numElem+1)); // Longfei: not used anymore
  uNorm=sqrt(uNorm/(numElem+1));
  vNorm=sqrt(vNorm/(numElem+1));
  aNorm=sqrt(aNorm/(numElem+1));
  
  if( file !=NULL )
    {
      fPrintF(file,"-- BM%i -- %s: Error t=%9.3e Ne=%i: uErr=(%8.2e,%8.2e)=(max,max/uNorm),"
	      " vErr=(%8.2e,%8.2e)=(max,max/vNorm),"
	      " aErr=(%8.2e,%8.2e)=(max,max/aNorm) (steps=%i)\n",getBeamID(),(const char*)label,t,numElem,
	      uErr,uErr/max(1.e-12,uNorm),
	      // ul2err,ul2err/max(1.e-12,uNorm),
	      vErr,vErr/(max(1.e-12,vNorm)),
	      aErr,aErr/(max(1.e-12,aNorm)),	      
	      numberOfTimeSteps);
    }
  
  if( uvErr!=NULL )
    {
      uvErr[0]=uErr;
      uvErr[1]=vErr;
      uvErr[2]=aErr;
    }
  if( uvNorm!=NULL )
    {
      uvNorm[0]=uNorm;
      uvNorm[1]=vNorm;
      uvNorm[2]=aNorm;
    }
  
  // -- THIS SHOULD BE MOVED : check file is called too many times --  //???Longfei 20160301: where should I put this?
  if( file !=NULL )
    {
      FILE *checkFile = dbase.get<FILE*>("checkFile");
      writeCheckFile( t, checkFile );
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
writeCheckFile( real t, FILE *file )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  if( file!=NULL && myid==0 )
    {
      const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
      const int & current = dbase.get<int>("current"); 
      const aString & name = dbase.get<aString>("name");

      RealArray & time = dbase.get<RealArray>("time");
      std::vector<RealArray> & u = dbase.get<std::vector<RealArray> >("u"); // displacement 
      std::vector<RealArray> & v = dbase.get<std::vector<RealArray> >("v"); // velocity
      std::vector<RealArray> & a = dbase.get<std::vector<RealArray> >("a"); // acceleration

      aString label=name;
      real uvErr[3], uvNorm[3]; // Longfei 20160301: changed size to 3 to hold errors for a
      getErrors( time(current), u[current], v[current], a[current] ,label, NULL, uvErr, uvNorm );

      const int numberOfComponentsToOutput=3;
      fPrintF(file,"%9.2e %i  ",time(current),numberOfComponentsToOutput);
      fPrintF(file,"%i %9.2e %10.3e  ",0,uvErr[0],uvNorm[0]);
      fPrintF(file,"%i %9.2e %10.3e  ",1,uvErr[1],uvNorm[1]);
      fPrintF(file,"%i %9.2e %10.3e  ",2,uvErr[2],uvNorm[2]);    // Longfei: add a errors to checkFile
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
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  
  if( exactSolutionOption != "none" )
    {
      const int & numberOfTimeLevels = dbase.get<int>("numberOfTimeLevels");
      const int & current = dbase.get<int>("current"); 
      const aString & name = dbase.get<aString>("name");

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
  //Longfei 20160128: label is not used here. we might need it to pass some info to display on the plot
   
  //Longfei 20160121: new way of handling parameters
  const real & L = dbase.get<real>("length");
  const int & numElem = dbase.get<int>("numElem");
  const int & numOfGhost = dbase.get<int>("numberOfGhostPoints");
  const aString & exactSolutionOption = dbase.get<aString>("exactSolutionOption");
  
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


  // old way:
  // realArray x(1,numNodes);
  // for( int i=0; i<=numElem; i++ ) 
  //   x(0,i)=((real)i /numElem) *  L;       // position along neutral axis 
  // DataPointMapping line;
  // line.setDataPoints(x,0,1);  // 0=position of coordinates, 1=domain dimension
  // MappedGrid c(line);   // a grid
  // c.update(MappedGrid::THEvertex | MappedGrid::THEmask);
  
  // Longfei 20160126: new way to get beam grid
  MappedGrid &c =dbase.get<MappedGrid>("beamGrid");

  bool plotErrors=exactSolutionOption!="none";

  const bool & xd = dbase.get<bool>("isCubicHermiteFEM");
  // number of components: 
  int nv=3;    // [u,v,a]
  if(xd) nv*=2;  // + [ux,vx,ax]
  if( plotErrors ) nv*=2;  // doulbe number of components for errors

  
  Range all;
  // *fix me for parallel* : 
  realMappedGridFunction w(c,all,all,all,nv); // holds things to plot 

  OV_GET_SERIAL_ARRAY(real,w,wLocal);
  wLocal=0.;

  Index I=Range(-numOfGhost,numElem+numOfGhost); // [0,1,2,.....,numElem]
  Index J;
  if(xd)
    J = 2*I;  // [0,2,4,...,2*numElem]  for Cubic Hermite FEM results
  else
    J= I;   //[0,1,2,....,numElem] for FD results

  
  w.setName("w");
  int nc = 0;
  w.setName("u",nc);
  wLocal(I,0,0, nc++)= uc(J,0,0,0);   // u at nodes
  w.setName("v",nc);
  wLocal(I,0,0, nc++)= vc(J,0,0,0);
  w.setName("a",nc);
  wLocal(I,0,0, nc++)= ac(J,0,0,0);
  if(xd)
    {
      w.setName("ux",nc);
      wLocal(I,0,0, nc++)= uc(J+1,0,0,0);  // ux at nodes
      w.setName("vx",nc);
      wLocal(I,0,0, nc++)= vc(J+1,0,0,0);  // vx at nodes
      w.setName("ax",nc);
      wLocal(I,0,0, nc++)= ac(J+1,0,0,0);
    }
  
  if( plotErrors )
    {
      RealArray ue, ve, ae;
      getExactSolution( t, ue, ve, ae );
      w.setName("uErr",nc);
      wLocal(I,0,0, nc++)= uc(J,0,0,0)-ue(J,0,0,0);   // u-error at nodes
      w.setName("vErr",nc);
      wLocal(I,0,0, nc++)= vc(J,0,0,0)-ve(J,0,0,0);
      w.setName("aErr",nc);
      wLocal(I,0,0, nc++)= ac(J,0,0,0)-ae(J,0,0,0);
      if(xd)
	{
	  w.setName("uxErr",nc);
	  wLocal(I,0,0, nc++)= uc(J+1,0,0,0)-ue(J+1,0,0,0);   // ux-error at nodes
	  w.setName("vxErr",nc);
	  wLocal(I,0,0,nc++)= vc(J+1,0,0,0)-ve(J+1,0,0,0);
	  w.setName("axErr",nc);
	  wLocal(I,0,0,nc++)= ac(J+1,0,0,0)-ae(J+1,0,0,0);
	}
    }
  assert(nc == nv); 
  

  // ** TODO ** we could eval the Hermite interpolant on a finer grid **

  
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
  // the parameters to be updated:
  real & density = dbase.get<real>("density");
  real & elasticModulus = dbase.get<real>("elasticModulus");
  real & areaMomentOfInertia = dbase.get<real>("areaMomentOfInertia");
  real & thickness = dbase.get<real>("thickness");
  real & L = dbase.get<real>("length");
  int & numElem = dbase.get<int>("numElem");
  real & pressureNorm = dbase.get<real>("pressureNorm");
  real & tension = dbase.get<real>("tension"); // coefficient of w_xx
  real & K0 = dbase.get<real>("K0");           //  coefficient of -w
  real & Kt = dbase.get<real>("Kt");           //  coefficient of -w_t
  real & Kxxt = dbase.get<real>("Kxxt");       //  coefficient of w_{xxt} 
  real & ADxxt = dbase.get<real>("ADxxt");       //  coefficient of artificial dissipation
  real & cfl = dbase.get<real>("cfl");  
  bool & useSecondOrderNewmarkPredictor = dbase.get<bool>("useSecondOrderNewmarkPredictor"); //keep this option for backward compatibility. predictor option is the new way
  //bool & useNewTridiagonalSolver = dbase.get<bool>("useNewTridiagonalSolver");
  bool & fluidOnTwoSides = dbase.get<bool>("fluidOnTwoSides");
  int & orderOfGalerkinProjection = dbase.get<int>("orderOfGalerkinProjection");\
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
  bool & relaxForce = dbase.get<bool>("relaxForce");
  bool & relaxCorrectionSteps=dbase.get<bool>("relaxCorrectionSteps");
  real & subIterationConvergenceTolerance = dbase.get<real>("subIterationConvergenceTolerance");
  real & addedMassRelaxationFactor = dbase.get<real>("addedMassRelaxationFactor");
  bool & useAitkenAcceleration = dbase.get<bool>("useAitkenAcceleration");
  //bool & useImplicitPredictor = dbase.get<bool>("useImplicitPredictor"); // Longfei: removed. Replaced with new time stepping optionMenu
  real * beamXYZ = dbase.get<real[3]>("beamXYZ");
  real & beamX0 = beamXYZ[0];
  real & beamY0 = beamXYZ[1];
  real & beamZ0 = beamXYZ[2];
  real & beamInitialAngle = dbase.get<real>("beamInitialAngle");
  real & newmarkBeta = dbase.get<real>("newmarkBeta");
  real & newmarkGamma = dbase.get<real>("newmarkGamma");
  BoundaryCondition & bcLeft = dbase.get<BoundaryCondition[2]>("boundaryConditions")[0];
  BoundaryCondition & bcRight = dbase.get<BoundaryCondition[2]>("boundaryConditions")[1];
  bool & useExactSolution = dbase.get<bool>("useExactSolution");
  aString & name = dbase.get<aString>("name");
  //Longfei 20160131: new way to specify time stepping methods
  TimeSteppingMethod & predictorMethod = dbase.get<TimeSteppingMethod>("predictorMethod");
  TimeSteppingMethod & correctorMethod = dbase.get<TimeSteppingMethod>("correctorMethod");

  //Longfei 20160303: option to test same order vs. same stencil size for FDBeamModel
  bool & useSameStencilSize = dbase.get<bool>("useSameStencilSize");
  int & dbg = dbase.get<int>("debug");
  
  GUIState gui;
  gui.setWindowTitle("Beam Model");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.


  bool buildDialog=true;
  if( buildDialog )
    {

      const int maxCommands=40;
      aString cmd[maxCommands];

      const int numColumns=2;

      dialog.setOptionMenuColumns(numColumns);
    
      aString tsOptions[] = { "leapFrog",
			      "adamsBashforth2",
			      "newmark1",
			      "newmark2Explicit",
			      "newmark2Implicit",
			      "" };
    
      GUIState::addPrefix(tsOptions,"predictor: ",cmd,maxCommands);
      dialog.addOptionMenu("predictor:",cmd,tsOptions,4 );  // default is newmark2Implicit

    
      aString corrOptions[] = { "newmarkCorrector",
				"adamsMoultonCorrector",
				"" };  
      GUIState::addPrefix(corrOptions,"corrector: ",cmd,maxCommands);
      dialog.addOptionMenu("corrector:",cmd,corrOptions,0); // default is   newmarkCorrector


    
    
      aString bcOptions[] = { "pinned",
			      "clamped",
			      "slide",
			      "free",
			      "periodic",
			      "" };

      GUIState::addPrefix(bcOptions,"bc left:",cmd,maxCommands);
      dialog.addOptionMenu("BC left:",cmd,bcOptions,0 ); //default bc is pinned

      GUIState::addPrefix(bcOptions,"bc right:",cmd,maxCommands);
      dialog.addOptionMenu("BC right:",cmd,bcOptions,0 ); //default bc is pinned


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


      // Longfei 20160131:  "use second order Newmark predictor" is removed from GUI;
      // but this option can still be recognized from cmd file for backward compatibility.
      // An optionMenu is added for choices of timeSteppingMethod.
      aString tbCommands[] = {"use exact solution",
			      "save profile file",
			      "save probe file",
			      "twilight-zone",
			      // "use second order Newmark predictor", 
			      // "use new tridiagonal solver",
			      //"use implicit predictor",   
			      "fluid on two sides",
			      "use small deformation approximation",
			      "relax correction steps",
			      "relax force",
			      "use Aitken acceleration",
			      "smooth solution",
			      "use same stencil size for FD",
			      ""};
      int tbState[15];
      tbState[0] = useExactSolution;
      tbState[1] = dbase.get<bool>("saveProfileFile");
      tbState[2] = dbase.get<bool>("saveProbeFile");
      tbState[3] = twilightZone;
      //tbState[4] = useSecondOrderNewmarkPredictor;
      //tbState[4] = useNewTridiagonalSolver; 
      // tbState[6] = useImplicitPredictor;  //Longfei 20160202: replaced with new time stepping choices
      tbState[4] = fluidOnTwoSides;
      tbState[5] = useSmallDeformationApproximation;
      tbState[6] = relaxCorrectionSteps;
      tbState[7] = relaxForce;
      tbState[8] = useAitkenAcceleration;
      tbState[9] = smoothSolution;
      tbState[10] = useSameStencilSize;
    
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
      textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",dbg);  nt++; 
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

  //Longfei 20160128: add some checks to see if the current beam
  // is an FEM beam. If not, some parameters specified here will
  // be igored since they are not needed by FD beam. I am not changing the layouts
  // of the GUI for now so that all the cmd files already in use are still working
  const bool & initialized = dbase.get<bool>("initialized");
  const bool & isFEM = dbase.get<bool>("isCubicHermiteFEM");
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
	  // Longfei 20160128: can not plot since beam is not initialized at this moment.
	  //GraphicsParameters psp;
	  //aString label="BeamModel";
	  //plot( t,gi, psp , "BeamModel" );
	  printF("-- BM%i -- Warning: can not plot since beam is not initialized at this moment.\n",getBeamID());
	  assert(!initialized);
      
	}
    
      else if( dialog.getTextValue(answer,"debug:","%i",dbg) ){} //
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
      else if( dialog.getTextValue(answer,"order of Galerkin projection:","%i",orderOfGalerkinProjection) )
	{
	  //Longfei 20160128: this option is for FEMBeamModel only
	  if(isFEM)
	    {
	      if( orderOfGalerkinProjection!=2 && orderOfGalerkinProjection!=4 )
		{
		  printF("-- BM%i -- Error: orderOfGalerkinProjection=%i is invalid, must be 2 or 4. \n",getBeamID(),orderOfGalerkinProjection);
		}
	  
	      printF("Setting the order of accuracy of the Galerkin projection =%i (e.g. for the force integral)\n",
		     orderOfGalerkinProjection);
	    }
	  else
	    {
	      printF("-- BM%i -- Warning: Option ignored. This is an FDBeamModel. orderOfGalerkinProjection is for FEMBeamModel only.\n",getBeamID());
	    }
      
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
      else if( len=answer.matches("use new tridiagonal solver") ) // Longfei 20160210: backward compatibility
	{
	  printF("-- BM%i  -- Warning: the toggle button \"use new tridiagonal solver\" is removed.",
		 "We  always use the new tridiagonal solver now\n",getBeamID());	  
	  
	} //
      //Longfei 20160131: this option is replaced by optionMenu time stepping. This is kept here for backward compatibility
      else if( dialog.getToggleValue(answer,"use second order Newmark predictor",useSecondOrderNewmarkPredictor))
	{
	  printF("-- BM%i -- Warning: the toggle button \"use second order Newmark predictor\" is removed. ",
		 "Please specify time stepings using the \"predictor\" and \"corrector\" option menus in the future\n",getBeamID());

	  if(  useSecondOrderNewmarkPredictor )
	    {
	      printF("use SECOND order implicit Newmark predictor and Newmark corrector\n");

	      //Longfei 20160131: new way for time stepping:
	      predictorMethod = newmark2Implicit;
	      correctorMethod = newmarkCorrector;
	    }
	  else
	    {
	      printF("use FIRST order Newmark predictor and Newmark corrector\n");
	      //Longfei 20160131: new way for time stepping:
	      predictorMethod = newmark1;
	      correctorMethod = newmarkCorrector;
	    }
	}
      else if( dialog.getToggleValue(answer,"use exact solution",useExactSolution) )
	{
	  // *old way*
	  // Longfei 20160122: Removed
	  // if( useExactSolution )
	  //  initialConditionOption="oldTravelingWaveFsi";
	}
      else if( dialog.getToggleValue(answer,"use small deformation approximation",useSmallDeformationApproximation) )
	{
	  printF("-- BM%i -- useSmallDeformationApproximation=true : adjust the beam surface acceleration and"
		 " surface 'internal force' assuming small deformations\n",getBeamID());
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

	      printF("-- BM%i --  BeamModel: info of the probed position will be saved to file '%s'\n",getBeamID(),(const char*)probeFileName);

	      // Probe file header info:
	      // Get the current date
	      time_t *tp= new time_t;
	      time(tp);
	      // tm *ptm=localtime(tp);
	      const char *dateString = ctime(tp);
	      const  real * initialBeamTangent = dbase.get<real[2]>("initialBeamTangent");
	      const  real * initialBeamNormal = dbase.get<real[2]>("initialBeamNormal");
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
      //else if( dialog.getToggleValue(answer,"use implicit predictor",useImplicitPredictor) ){}// removed. This is handled using new time stepping choices
      else if( len=answer.matches("use implicit predictor"))
	{
	  printF("-- BM%i -- Warning: Option ignored. The  \"use implicit predictor\" toggle button is removed. "
		 "Please specify time steping using the \"predictor\" and \"corrector\" option menus in the future\n",getBeamID());
	}
      else if( dialog.getToggleValue(answer,"relax force",relaxForce) )
	{
	  if( relaxForce )
	    printF("-- BM%i -- relax the force in the added mass iteration\n",getBeamID());
	  else
	    printF("-- BM%i -- relax the acceleration in the added mass iteration\n",getBeamID());
	}
    
      else if( dialog.getToggleValue(answer,"relax correction steps",relaxCorrectionSteps) )
	{
	  printF("-- BM%i --  The BeamModel correction steps can be relaxed. This may be necessary for 'light beams',\n" 
		 "with FSI simulations using the traditional partitioned schemes.\n",getBeamID()
		 );
	}
      else if( dialog.getToggleValue(answer,"use Aitken acceleration",useAitkenAcceleration) )
	{
	  if( useAitkenAcceleration )
	    printF("-- BM%i --  Use Aitken acceleration to accelerate the correction sub-iterations used for light body FSI\n",getBeamID());
	  else
	    printF("-- BM%i --  Do NOT use Aitken acceleration to accelerate the correction sub-iterations used for light body FSI\n",getBeamID());
	}

      else if( dialog.getToggleValue(answer,"smooth solution",smoothSolution) )
	{
	  printF("-- BM%i -- smooth solution\n",getBeamID());
	}//

      else if( dialog.getToggleValue(answer,"use same stencil size for FD",useSameStencilSize) )
	{
	  if(!useSameStencilSize)
	    {
	      printF("-- BM%i -- Warning: use same order for FD approximations. This is only to show same order is bad. DO NOT USE THIS OPTION FOR REAL RUNS\n",getBeamID());
	    }
	}//

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
	      printF("-- BM%i -- Error: unexpected TZ=[%s]\n",getBeamID(),(const char*)name);
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
	  printF("-- BM%i --  Setting trigonometric TZ frequencies to ft=%g, fx=%g, fy=%g, fz=%g.\n",
		 getBeamID(),trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]);

	  dialog.setTextLabel("trig frequencies:",sPrintF(buff,"%g, %g, %g, %g (ft,fx,fy,fz)",
							  trigFreq[0],trigFreq[1],trigFreq[2],trigFreq[3]));
	}
      else if( dialog.getTextValue(answer,"degree in space:","%i",degreeInSpace) ){} //
      else if( dialog.getTextValue(answer,"degree in time:","%i",degreeInTime) ){} //


      //Longfei 20160131: new OptionMenu for time stepping  methods 
      else if( len=answer.matches("predictor: ") )  
	{

	  aString name=answer(len,answer.length()-1);
	  predictorMethod = (name=="leapFrog" ? leapFrog:
			     name=="adamsBashforth2" ? adamsBashforth2:
			     name=="newmark1" ? newmark1:
			     name=="newmark2Explicit" ?  newmark2Explicit:
			     name=="newmark2Implicit" ? newmark2Implicit:
			     unknownTimeStepping);
      
	  if(predictorMethod==unknownTimeStepping)
	    {
	      printF("-- BM%i -- Error: unexpected predictor=[%s]\n",getBeamID(),(const char*)name);
	      gi.stopReadingCommandFile();
	      continue;
	    }

	  dialog.getOptionMenu("predictor:").setCurrentChoice(name);
	  printF("-- BM%i --  setting predictor=[%s]\n",getBeamID(),(const char*)name);

	}

      else if( len=answer.matches("corrector: ") )  
	{
	  aString name=answer(len,answer.length()-1);
	  correctorMethod = (name=="newmarkCorrector" ? newmarkCorrector:
			     name=="adamsMoultonCorrector" ? adamsMoultonCorrector:
			     unknownTimeStepping);
      
	  if(correctorMethod==unknownTimeStepping)
	    {
	      printF("-- BM%i -- Error: unexpected corrector=[%s]\n",getBeamID(),(const char*)name);
	      gi.stopReadingCommandFile();
	      continue;
	    }
      
	  dialog.getOptionMenu("corrector:").setCurrentChoice(name);
	  printF("-- BM%i --  setting corrector=[%s]\n",getBeamID(),(const char*)name);
	}
        
      else
	{
	  printF("-- BM%i -- BeamModel::update:ERROR:unknown response=[%s]\n",getBeamID(),(const char*)answer);
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
      printF("-- BM%i -- Parameters are updated:\n",getBeamID());
      //writeParameterSummary(); // Longfei 20160621: do not print summary here. Parameters are printed when called.
    }
  
  //  pBeamModel->setParameters(I, Em, rho, L, thick, pnorm, nelem, bcl, bcr,x0,y0,(exact==1));


  return 0;

}

///=====================================================================================================
//Longfei 20160211: this function determines the data structure of the solution arrays for FEM and FD beamModels
// input(output) I1: index on axis1, if FEM model, 2*i1,2*i1+1 stores u and ux respectively 
// input(output) I2: index on axis2,
// input(output) I3: index on axis3,
// input(output)  C: dimension index
///=====================================================================================================
int BeamModel::
getSolutionArrayIndex(Index & I1, Index &I2, Index & I3, Index &C) const
{

  const int & numElem = dbase.get<int>("numElem");
  const int numOfGhost = dbase.get<int>("numberOfGhostPoints");
  const int numMotionDirections = dbase.get<int>("numberOfMotionDirections");
  const bool xd =  dbase.get<bool>("isCubicHermiteFEM");
  if(xd)
    {
      assert(numOfGhost==0);  // no ghost points needed for FEM method
      I1 = Range(2*numElem+2); // each node i has 2 solutions,i.e. ui and uxi
    }
  else
    {
      I1 = Range(-numOfGhost,numElem+numOfGhost);
    }
  I2 = 0; I3=0; // Beam Domain is assumed to be 1D
  C = Range(numMotionDirections);

  return 0;
}


//Longfei 20160117: new function added to set the dimension of solution array.
int BeamModel::
redimSolutionArray(RealArray & u, RealArray & v, RealArray & a ) const
{
  
  if(u.getLength(0)!=0 && v.getLength(0)!=0 && a.getLength(0)!=0) //already done redim
    return 0;

  Index I1,I2,I3,C;
  getSolutionArrayIndex(I1,I2,I3,C);
  
  u.redim(I1,I2,I3,C);
  v.redim(I1,I2,I3,C);
  a.redim(I1,I2,I3,C);

  if(false)
    {
      // check index
      printF("Check Index in BeamModel::redimSolutionArray:\n");
      const int & numElem = dbase.get<int>("numElem");
      const int &numOfGhost =  dbase.get<int>("numberOfGhostPoints");
      const int &numMotionDirections =  dbase.get<int>("numberOfMotionDirections");

      printF("numElem = %3d, numOfGhost = %3d, numMotionDirections = %3d\n",numElem,numOfGhost,numMotionDirections);
      I1.display("I1");
      I2.display("I2");
      I3.display("I3");
      C.display("C");

      u.dimension(0).display("u I1 after redim:");
      u.dimension(1).display("u I2 after redim:");
      u.dimension(2).display("u I3 after redim:");
      u.dimension(3).display("u C after redim:");
      OV_ABORT("THIS IS A CHEKC");

    }


  return 0;
}



//Longfei 20160122:
// for public access of exactSolutionOption
aString BeamModel::
getExactSolutionOption() const
{
  return dbase.get<aString>("exactSolutionOption");
}



// for public access of beamID (read only)
// Return the beam ID (a unique ID for this beam)
const int& BeamModel::
getBeamID() const
{
  return dbase.get<int>("beamID");
}

// for public access of beamType (read only)
const aString& BeamModel::
getBeamType() const
{
  return beamType;
}

// Longfei 20160621: renamed BeamModel::force() to BeamModel::getCurrentForce()
// Return the current force of the structure.
//
const RealArray& BeamModel::getCurrentForce() const
{
  const int & current = dbase.get<int>("current"); 
  std::vector<RealArray> & f = dbase.get<std::vector<RealArray> >("f"); // force

  return f[current];  // force at current time
}

real BeamModel::
getCurrentTime() const
{
  const int & current = dbase.get<int>("current");
  const RealArray & time = dbase.get<RealArray>("time");
  return time(current);
}


// make this  for now.
aString BeamModel::
getBCName(const BoundaryCondition & bc) const
{
  return (bc==pinned ? "pinned" : 
	  bc==clamped ? "clamped" :
	  bc==slideBC ? "slide" :
	  bc==periodic ? "periodic" : 
	  bc==freeBC ? "free" :
	  "unknown");
}

aString BeamModel::
getTSName(const TimeSteppingMethod & ts) const
{
  return (ts==leapFrog ? "leapFrog" : 
	  ts==adamsBashforth2 ? "adamsBashforth2" :
	  ts==newmark1 ? "newmark1" :
	  ts==newmark2Explicit ? "newmark2Explicit" : 
	  ts==newmark2Implicit ? "newmark2Implicit" :
	  ts==newmarkCorrector ? "newmarkCorrector" :
	  ts==adamsMoultonCorrector ? "adamsMoultonCorrector" :
	  "unknown");
}



// Longfei 20160127: new way to writeParameterSummary
void BeamModel::
displayDBase(FILE *file /*=stdout*/ ) 
{        
  fPrintF(file,"------- Parameters contained in BeamModel::dbase -------\n");
  fPrintF(file,"--------------------------------------------------------\n");

  // display parameters help in the  dbase
  for( DataBase::iterator e= dbase.begin(); e!= dbase.end(); e++ )
    {   
      string name=e->first;
      fPrintF(file,"%s=",name.c_str());
      DBase::Entry &entry = *(e->second);
      if( DBase::can_cast_entry<Real>(entry) )
	{
	  const Real value=cast_entry<Real>(entry);  
	  fPrintF(file,"%9.3e\n",value);
	}
      else if( DBase::can_cast_entry<int>(entry) )
	{
	  fPrintF(file,"%i\n",cast_entry<int>(entry));
	}
      else if( DBase::can_cast_entry<bool>(entry) )
	{
	  fPrintF(file,"%s\n",cast_entry<bool>(entry)? "true":"false");
	}
      else if( DBase::can_cast_entry<aString>(entry) )
	{
	  const string & s = cast_entry<aString>(entry);
	  fPrintF(file,"%s\n",s.c_str());
	}
      else if( DBase::can_cast_entry<RealArray>(entry) )
	{
	  fPrintF(file,"RealArray\n");
	}
      else if( DBase::can_cast_entry<RealArray*>(entry) )
	{
	  fPrintF(file,"RealArray*\n");
	}
      else if( DBase::can_cast_entry<FILE*>(entry) )
	{
	  fPrintF(file,"FILE*\n");
	}
      else if( DBase::can_cast_entry<Real[2]>(entry) )
	{
	  const Real *value=cast_entry<Real[2]>(entry);  
	  fPrintF(file,"[%9.3e,%9.3e]\n",value[0],value[1]);
	}
      else if( DBase::can_cast_entry<Real[3]>(entry) )
	{
	  const Real *value=cast_entry<Real[3]>(entry);  
	  fPrintF(file,"[%9.3e,%9.3e,%9.3e]\n",value[0],value[1],value[2]);
	}
      else if( DBase::can_cast_entry<Real[4]>(entry) )
	{
	  const Real *value=cast_entry<Real[4]>(entry);  
	  fPrintF(file,"[%9.3e,%9.3e,%9.3e,%9.3e]\n",value[0],value[1],value[2],value[3]);
	}
      else if( DBase::can_cast_entry<MappedGrid>(entry) )
	{  
	  fPrintF(file,"MappedGrid\n");
	}
      else if( DBase::can_cast_entry<BoundaryCondition[2]>(entry) )
	{
	  const BoundaryCondition *value=cast_entry<BoundaryCondition[2]>(entry); 
	  fPrintF(file,"[%s,%s]\n",(const char*)getBCName(value[0]),(const char*)getBCName(value[1]));
	}
      else if( DBase::can_cast_entry<TimeSteppingMethod>(entry) )
	{
	  const TimeSteppingMethod value=cast_entry<TimeSteppingMethod>(entry); 
	  fPrintF(file,"%s\n",(const char*)getTSName(value));
	}
      else if( DBase::can_cast_entry<std::vector<RealArray> >(entry) ) 
	{
	  fPrintF(file,"vector<RealArray>\n");
	}
      else if( DBase::can_cast_entry<OGFunction*>(entry) ) 
	{
	  fPrintF(file,"OGFunction*\n");
	}
      else
	{
	  fPrintF(file,"? (user-defined type, can't be displayed here)\n");
	}
      
    }
  fPrintF(file,"--------------------------------------------------------\n");

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
				      LocalReal& u, LocalReal& v)
{
   
  // what=exactSolutionScaleFactorFSI;  // wdh //Longfei: seems unused,removed
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
  
  //what=exactSolutionScaleFactorFSI;  // wdh // Longfei: seems unused, remove

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


//Longfei 20160120: oldTravelingWaveFsi removed
// void BeamModel::
// setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) const
// {
  
//   // printF("@@@BeamModel::setExactSolution exactSolutionScaleFactorFSI=%9.3e\n", dbase.get<OV_real>("exactSolutionScaleFactorFSI"));

//   double h=0.02;
//   double Ioverb=6.6667e-7;
//   double H=0.3;
//   double k=2.0*3.141592653589/L;
//   double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
//   // -- wHat determines the "amplitude" of the surface, scaled by some factor ...wdh
//   double what = exactSolutionScaleFactorFSI; // 0.00001;
//   double omegar = 0.8907148069, omegai = -0.9135887123e-2;
//   std::complex<LocalReal> omega_tilde(omegar, omegai);
//   std::complex<LocalReal> I(0.0,1.0);

//   for (int i = 0; i <= numElem; ++i) {

//     double xl = (double)i / numElem*L;
    
//     std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);
//     std::complex<LocalReal> fp = I*k*(exp(I*k*xl-I*omega_tilde*omega0*t)+exp(-I*k*xl-I*omega_tilde*omega0*t));
    

    
//     x(i*2) = 2.0*(what*f).real();      // displacement w ...wdh
//     x(i*2+1) = 2.0*(what*fp).real();   // slope w'  ...wdh
    
//     v(i*2) = 2.0*(-what*f*I*omega_tilde*omega0).real();
//     v(i*2+1) = 2.0*(-what*fp*I*omega_tilde*omega0).real();
    
//     a(i*2) = 2.0*(-what*f*omega_tilde*omega0*omega_tilde*omega0).real();
//     a(i*2+1) = 2.0*(-what*fp*omega_tilde*omega0*omega_tilde*omega0).real();
//   }
// }


// Longfei 20160120: this function seems unused. Removed
// double BeamModel::getExactPressure(double t, double xl) {

//   printF("@@@BeamModel::getExactPressure exactSolutionScaleFactorFSI=%9.3e\n", dbase.get<OV_real>("exactSolutionScaleFactorFSI"));

//   double h=0.02;
//   double Ioverb=6.6667e-7;
//   double nu = 0.001;
//   double H=0.3;
//   std::complex<LocalReal> I(0.0,1.0);
//   double omegar = 0.8907148069, omegai = -0.9135887123e-2;
//   std::complex<LocalReal> omega_tilde(omegar, omegai);
//   double k=2.0*3.141592653589/L;
//   double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
//   LocalReal beta = omega0/(k*k)/nu;
//   std::complex<LocalReal> alpha = k*sqrt(-I*beta*omega_tilde+1.0);

//   double what = exactSolutionScaleFactorFSI; // 0.00001;
//   std::complex<LocalReal> omega = omega_tilde*omega0;
  
  
//   std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);

//   std::complex<LocalReal> a = phi2d(alpha,k,H),b = phi1d(alpha,k,H);
//   std::complex<LocalReal> A = -1.0*what*omega*omega*alpha*0.5/k;
//   std::complex<LocalReal> c = a/b;
//   A /= (-c*phi1(alpha,k,H)+phi2(alpha,k,H));
//   std::complex<LocalReal> phat = A*(cosh(k*H)-c/alpha*sinh(k*H));

//   std::cout << xl << " " << 2.0*(f*phat).real() << std::endl;

//   LocalReal result = 2.0*(f*phat).real();

//   std::complex<LocalReal> r1 = (elasticModulus*Ioverb*k*k*k*k-density*h*omega*omega)*what, r2 = phat*1000.0 ;
//   std::cout << r1 << " " << r2 << std::endl;

//   return result;

// }

