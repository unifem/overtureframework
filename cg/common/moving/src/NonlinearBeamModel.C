//                                   -*- c++ -*-

#include "NonlinearBeamModel.h"
#include "display.h"
#include "NurbsMapping.h"

#include <fstream>

#include <sstream>

RealArray 
mult( const RealArray & a, const RealArray & b );

RealArray 
trans( const RealArray &a );

RealArray 
solve( const RealArray & a, const RealArray & b );

void 
printArray(const doubleSerialArray & u,  
           int i1a, int i1b,   
           int i2a, int i2b,   
           int i3a, int i3b,   
           int i4a, int i4b,   
           int i5a, int i5b,   
           int i6a, int i6b) ;

static void inverse3x3(const RealArray& B, RealArray& cg2) {

  real t4 = B(0, 2) * B(1, 0);
  real t6 = B(0, 2) * B(2, 0);
  real t8 = B(1, 0) * B(0, 1);
  real t10 = B(0, 1) * B(2, 0);
  real t12 = B(0, 0) * B(1, 1);
  real t14 = B(0, 0) * B(2, 1);
  real t17 = 0.1e1 / (t4 * B(2, 1) - t6 * B(1, 1) - t8 * B(2, 2) + t10 * B(1, 2) + t12 * B(2, 2) - t14 * B( 1, 2));
  cg2(0, 0) = (B(1, 1) * B(2, 2) - B(2, 1) * B(1, 2)) * t17;
  cg2(1, 0) = -(B(1, 0) * B(2, 2) - B(2, 0) * B(1, 2)) * t17;
  cg2(2, 0) = (B( 1, 0) * B(2, 1) - B(2, 0) * B(1, 1)) * t17;
  cg2(0, 1) = -(-B(0, 2) * B(2, 1) + B(0, 1) * B(2, 2)) * t17;
  cg2(1, 1) = (-t6 + B(0, 0) * B(2, 2)) * t17;
  cg2(2, 1) = -(-t10 + t14) * t17;
  cg2(0, 2) = (-B(0, 2) * B(1, 1) + B(0, 1) * B(1, 2)) * t17;
  cg2(1, 2) = -(-t4 + B(0, 0) * B(1, 2)) * t17;
  cg2(2, 2) = (t12 - t8) * t17;
  
  
}

// The last 6 arguments are for periodic boundary conditions.  
static void solveBlockTridiagonal3(const RealArray& elementM, const RealArray& f,
				   RealArray& u,
				   NonlinearBeamModel::BoundaryCondition bcLeft,
				   NonlinearBeamModel::BoundaryCondition bcRight) {

  RealArray ftmp;
  ftmp.redim(3);
  u = f;
  RealArray upper;  upper.redim(3,3);
  RealArray diag1;  diag1.redim(3,3);
  RealArray diag2;  diag2.redim(3,3);

  int numElem = f.getLength(0)/3-1;

  std::vector< RealArray > diagonal(numElem+1),
    superdiagonal(numElem),subdiagonal(numElem);
  
  Index all3(0,3);
  for (int i = 0; i <= numElem; ++i) {
    diagonal[i].redim(3,3);
  }

  for (int i = 0; i < numElem; ++i) {

    for (int k = 0; k < 3; ++k) {
      for (int l = 0; l  < 3; ++l) {
	diagonal[i](k,l) = elementM(i, k,l);
      }
    }
  }
  
  diagonal[numElem].redim(3,3);
  diagonal[numElem] = 0.0;

  for (int i = 0; i < numElem; ++i) {

    subdiagonal[i].redim(3,3);
    superdiagonal[i].redim(3,3);
    for (int k = 0; k < 3; ++k) {
      for (int l = 0; l  < 3; ++l) {

	subdiagonal[i](k,l) = elementM(i, k+3, l);
	superdiagonal[i](k,l) = elementM(i,k,l+3);
	diagonal[i+1](k,l) += elementM(i, k+3,l+3);
      }
    }
      
  }

  if (bcLeft == NonlinearBeamModel::Cantilevered) {
    diagonal[0](0,0) = diagonal[0](1,1) = diagonal[0](2,2) = 1.0;
    diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
    diagonal[0](0,2) = diagonal[0](2,0) = 0.0;
    diagonal[0](1,2) = diagonal[0](2,1) = 0.0;
    superdiagonal[0] = 0.0;
  }
  if (bcLeft == NonlinearBeamModel::Pinned) {
    diagonal[0](0,0) = diagonal[0](1,1) = 1.0;
    diagonal[0](0,1) = diagonal[0](1,0) = 0.0;
    diagonal[0](0,2) =  0.0;
    diagonal[0](1,2) =  0.0;
    superdiagonal[0](0,all3) = 0.0;
    superdiagonal[0](1,all3) = 0.0;
  }

  if (bcRight == NonlinearBeamModel::Cantilevered) {
    diagonal[numElem](0,0) = diagonal[numElem](1,1) = diagonal[numElem](2,2) = 1.0;
    diagonal[numElem](0,1) = diagonal[numElem](1,0) = 0.0;
    diagonal[numElem](0,2) = diagonal[numElem](2,0) = 0.0;
    diagonal[numElem](1,2) = diagonal[numElem](2,1) = 0.0;
    subdiagonal[numElem-1] = 0.0;
  }
  if (bcRight == NonlinearBeamModel::Pinned) {
    diagonal[numElem](0,0) = diagonal[numElem](1,1) = 1.0;
    diagonal[numElem](0,1) = diagonal[numElem](1,0) = 0.0;
    diagonal[numElem](0,2) =  0.0;
    diagonal[numElem](1,2) =  0.0;
    subdiagonal[numElem-1](0,all3) = 0.0;
    subdiagonal[numElem-1](1,all3) = 0.0;
  }
 

  RealArray inv;

  inv.redim(3,3);

  Index i3x3(0,3);
  
  for (int i = 0; i < numElem; ++i) {

    inverse3x3(diagonal[i], inv);
    superdiagonal[i] = mult(inv, superdiagonal[i]);
    u(i3x3) = mult(inv, u(i3x3) );
    u(i3x3+3) -= mult(subdiagonal[i],u(i3x3));
    diagonal[i+1] -= mult(subdiagonal[i],superdiagonal[i]);   

    i3x3 += 3;
  }

  inverse3x3(diagonal[numElem], inv);
  u(i3x3) = mult(inv, u(i3x3) );
  
  i3x3 -= 3;

  for (int i = numElem-1; i >= 0; --i) {

    u(i3x3) -= mult(superdiagonal[i], u(i3x3+3));

    i3x3 -= 3;
  }
  
}

int NonlinearBeamModel::debug=0;



//=======================================================================================
/// \brief Constructor. Thi snonlinera model follows the 'continuum-based beam" model
///    approach in the "Blue book".
/// \note Initial development by Alex Main. Summer of 2013.
/// \note
//       density : 
//       nu      : Poisson's ratio,   nu = lambda/( 2*(lambda+mu) )
//       Em      : Young's modulus    E = mu*(3*lambda+2*mu)/( lambda+mu )
//
//     lambda = 2*mu*nu/( 1-2*nu ) = E*nu/( (1+nu)*(1-2*nu) )
//     mu     = E/( 2*(1+nu) )
//
//   omegaStructure : relaxation parameter when solving the non-linear structural equations
//
//=======================================================================================
NonlinearBeamModel::
NonlinearBeamModel() 
{
  name="nlbeam1";

  Em=1000.;
  density=1000.;
  nu=.25;

  numNodes=11;
  numElem=10;
  isSteady=false;
  
  // for uniform beams: 
  beamLength=1.;
  beamThickness=.02;

  newmarkBeta = 0.25;
  newmarkGamma = 0.5;

  omegaStructure = 1.; 

  bcLeft = Cantilevered;
  bcRight = Free;

  tipfile.open("tip.txt");

  pressureNorm = 1.0;

  time_step_num = 1;


  added_mass_relaxation = 1.0;

  numCorrectorIterations = 0;

  convergenceTolerance = 1e-3;

  rayleighAlpha = rayleighBeta = 0.0;

  beamNodes = NULL;
  
  slaveStates = NULL;
  
  projectedPoints = NULL;

  elementMassMatrices = NULL;

  useExactSolution=false;
  exactSolution=standingWave;
  
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


}

// ======================================================================================
/// \brief Destructor.
// ======================================================================================
NonlinearBeamModel::
~NonlinearBeamModel()
{

  if (beamNodes)
    delete [] beamNodes;

  if (slaveStates)
    delete [] slaveStates;

  if (projectedPoints)
    delete [] projectedPoints;
  
  if (elementMassMatrices)
    delete [] elementMassMatrices;
}

// ======================================================================================
/// \brief initialize the beam model
// ======================================================================================
void NonlinearBeamModel::
initialize()
{
  printF("+++ NonlinearBeamModel:: initialize the model ++++\n");

  assert( beamNodes!=NULL );
  
  delete [] slaveStates;
  slaveStates = new SlaveState[numNodes];

  RealArray xexact(numNodes*2), vexact(numNodes*2),
    aexact(numNodes*2);

  if (useExactSolution) 
  {
    setExactSolution(0.0,xexact,vexact,aexact);
  }

  for (int i = 0; i < numNodes; ++i) 
  {

    printF(" node i=%i X=(%g,%g) angle=%g, thick=%g",beamNodes[i].X[0],beamNodes[i].X[1],beamNodes[i].undeformedRotation,
	   beamNodes[i].thickness);

    //memcpy(beamNodes[i].x, beamNodes[i].X, sizeof(beamNodes[i].x));
    memset(beamNodes[i].u, 0, sizeof(beamNodes[i].u));
    if (useExactSolution)
      beamNodes[i].u[1] = xexact(i*2);
    
    // convert to radians
    // beamNodes[i].undeformedRotation *= 3.14159265358979323846264338/180.0;

    beamNodes[i].rotation = beamNodes[i].undeformedRotation;
    beamNodes[i].angletilde = beamNodes[i].undeformedRotation;

    //beamNodes[i].rotation = beamNodes[i].angletilde = 0.0;

    //beamNodes[i].u[0] = -(beamNodes[i].X[0] -0.25);
    //beamNodes[i].u[1] = -(beamNodes[i].X[0] -0.25)*1.01;
    
    //beamNodes[i].u[1] = 0.0;//-(beamNodes[i].X[0] -0.25);
    //beamNodes[i].u[0] = (beamNodes[i].X[0] -0.25)*0.01;

    memcpy(beamNodes[i].utilde,beamNodes[i].u , sizeof(beamNodes[i].utilde));

    beamNodes[i].angularVelocity = 0.0;

    memset(beamNodes[i].v, 0, sizeof(beamNodes[i].v));
    if (useExactSolution) {
      beamNodes[i].v[1] = vexact(i*2);
      beamNodes[i].angularVelocity = vexact(i*2+1);
    }

    memcpy(beamNodes[i].vtilde, beamNodes[i].v, sizeof(beamNodes[i].vtilde));

    memset(beamNodes[i].a, 0, sizeof(beamNodes[i].a));

    beamNodes[i].angularAcceleration = 0.0;

    if (useExactSolution) 
    {
      beamNodes[i].a[1] = aexact(i*2);
      beamNodes[i].angularAcceleration = aexact(i*2+1);
    }
      
    beamNodes[i].angularVelocitytilde = beamNodes[i].angularVelocity;
    

    beamNodes[i].p0[0] = cos(beamNodes[i].undeformedRotation);
    beamNodes[i].p0[1] = sin(beamNodes[i].undeformedRotation);
        
    slaveStates[i].Xplus[0] = beamNodes[i].X[0]+ 
      0.5*beamNodes[i].thickness*beamNodes[i].p0[0];
    slaveStates[i].Xplus[1] = beamNodes[i].X[1]+ 
      0.5*beamNodes[i].thickness*beamNodes[i].p0[1];

    slaveStates[i].Xminus[0] = beamNodes[i].X[0]- 
      0.5*beamNodes[i].thickness*beamNodes[i].p0[0];
    slaveStates[i].Xminus[1] = beamNodes[i].X[1]- 
      0.5*beamNodes[i].thickness*beamNodes[i].p0[1];

    printF(" X+=(%g,%g), X-=(%g,%g)\n", slaveStates[i].Xplus[0],slaveStates[i].Xplus[1],
	   slaveStates[i].Xminus[0],slaveStates[i].Xminus[1]);
    
  }


  M.redim(numElem,6,6);
  K.redim(numElem,6,6);
  Fext.redim(numNodes,3);

  Fext = 0.0;

  Ffluid.redim(numNodes,3);
  Ffluid = 0.0;
  
  //Fext(numElem,0) = 100.0;

  bodyForce[0] = bodyForce[1] = 0.0;

  elementMassMatrices = new RealArray[numElem];

  reevaluateMassMatrix();


  t = 0.0;


}




// ================================================================================================================
/// \brief 
// ================================================================================================================
void NonlinearBeamModel::
readBeamFile(const char* filename) 
{

  std::ifstream infile(filename);

  std::cout << "Reading beam file " << filename << std::endl;

  infile >> numNodes;

  infile >> numElem;
  
  infile >> density >> nu >> Em >> omegaStructure >> isSteady;

  printF("NonlinearBeamModel: density=%g, nu=%g, Em=%g, omegaStructure=%g isSteady=%i",
	 density,nu,Em,omegaStructure,isSteady);
  
  int bc1,bc2;
  infile >> bc1 >> bc2;
  if (bc1 == 0)
    bcLeft = Cantilevered;
  if (bc1 == 1)
    bcLeft = Pinned;
  if (bc1 == 2)
    bcLeft = Free;

  if (bc2 == 0)
    bcRight = Cantilevered;
  if (bc2 == 1)
    bcRight = Pinned;
  if (bc2 == 2)
    bcRight = Free;
  
  printF(" bcLeft=%s, ",(bcLeft==Cantilevered ? "Cantilevered" : bcLeft==Pinned ? "Pinned" : "Free"));
  printF(" bcRight=%s\n",(bcRight==Cantilevered ? "Cantilevered" : bcRight==Pinned ? "Pinned" : "Free"));
  

  infile >> pressureNorm;

  infile >> useExactSolution;

  infile >> rayleighAlpha >> rayleighBeta;

  printF(" pressureNorm=%g, useExactSolution=%i, rayleighAlpha=%g, raleighBeta=%g\n",pressureNorm,
	 useExactSolution,rayleighAlpha,rayleighBeta);
  
  delete [] beamNodes;
  beamNodes = new BeamNode[numNodes];

  for (int i = 0; i < numNodes; ++i) 
  {
    infile >> beamNodes[i].X[0] >> beamNodes[i].X[1]  >> beamNodes[i].X[2];
    infile >> beamNodes[i].undeformedRotation >> beamNodes[i].thickness;

    // convert to radians
    beamNodes[i].undeformedRotation *= Pi/180.; // wdh 3.14159265358979323846264338/180.0;
  }
  infile.close();

  initialize();
  
}


// ==================================================================
/// \brief return the estimatimated *explicit* time step dt 
/// \auhtor WDH
// ==================================================================
real NonlinearBeamModel::
getExplicitTimeStep() const
{
  // estimate the time step from the bulk model 

  // Lame parameters: 
  real lambda= Em*nu/( (1.+nu)*(1.-2.*nu) );
  real mu = Em/( 2.*(1.+nu));
  
  real cp = sqrt( (lambda+2*mu)/density );
  
  real dx = beamLength/numNodes;  // *fix me for general case*
  
  real dt = dx/cp;
  
  printF("NonlinearBeamModel::getExplicitTimeStep: cp=%8.2e, dx=%8.2e, dt=%8.2e\n",cp,dx,dt);

  return dt;
}



// ===================================================================================================================
/// \brief 
// ===================================================================================================================
void NonlinearBeamModel::
computeSlaveStates() 
{

  for (int i = 0; i < numNodes; ++i) {

    beamNodes[i].p[0] = cos(beamNodes[i].rotation);
    beamNodes[i].p[1] = sin(beamNodes[i].rotation);
    
    slaveStates[i].uplus[0] = beamNodes[i].u[0] + 0.5*beamNodes[i].thickness*(beamNodes[i].p[0]-beamNodes[i].p0[0]) ;
    slaveStates[i].uplus[1] = beamNodes[i].u[1] + 0.5*beamNodes[i].thickness*(beamNodes[i].p[1]-beamNodes[i].p0[1]);

    slaveStates[i].uminus[0] = beamNodes[i].u[0] - 0.5*beamNodes[i].thickness*(beamNodes[i].p[0]-beamNodes[i].p0[0]);
    slaveStates[i].uminus[1] = beamNodes[i].u[1] - 0.5*beamNodes[i].thickness*(beamNodes[i].p[1]-beamNodes[i].p0[1]);

    slaveStates[i].xplus[0] = beamNodes[i].u[0] + beamNodes[i].X[0]+ 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0];
    slaveStates[i].xplus[1] = beamNodes[i].u[1] + beamNodes[i].X[1]+ 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1];

    slaveStates[i].xminus[0] = beamNodes[i].u[0] + beamNodes[i].X[0]- 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0];
    slaveStates[i].xminus[1] = beamNodes[i].u[1] + beamNodes[i].X[1]- 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1];


    slaveStates[i].vplus[0] = beamNodes[i].v[0] - 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1]*beamNodes[i].angularVelocity;
    slaveStates[i].vplus[1] = beamNodes[i].v[1] + 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0]*beamNodes[i].angularVelocity;

    slaveStates[i].vminus[0] = beamNodes[i].v[0] + 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1]*beamNodes[i].angularVelocity;
    slaveStates[i].vminus[1] = beamNodes[i].v[1] - 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0]*beamNodes[i].angularVelocity;

    real omega2 = beamNodes[i].angularVelocity*beamNodes[i].angularVelocity;
    slaveStates[i].aplus[0] = beamNodes[i].a[0] - 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1]*beamNodes[i].angularAcceleration-
      0.5*beamNodes[i].thickness*omega2*beamNodes[i].p[0];
    slaveStates[i].aplus[1] = beamNodes[i].a[1] + 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0]*beamNodes[i].angularAcceleration-
      0.5*beamNodes[i].thickness*omega2*beamNodes[i].p[1];

    slaveStates[i].aminus[0] = beamNodes[i].a[0] + 
      0.5*beamNodes[i].thickness*beamNodes[i].p[1]*beamNodes[i].angularAcceleration+
      0.5*beamNodes[i].thickness*omega2*beamNodes[i].p[0];
    slaveStates[i].aminus[1] = beamNodes[i].v[1] - 
      0.5*beamNodes[i].thickness*beamNodes[i].p[0]*beamNodes[i].angularAcceleration+
      0.5*beamNodes[i].thickness*omega2*beamNodes[i].p[1];


  }
}

void NonlinearBeamModel::computeShapeFunctions(real xi, real eta, real N[4]) {

  N[0] = 0.25*(1.0-xi)*(1.0-eta);
  N[1] = 0.25*(1.0+xi)*(1.0-eta);
  N[2] = 0.25*(1.0+xi)*(1.0+eta);
  N[3] = 0.25*(1.0-xi)*(1.0+eta);
}


void NonlinearBeamModel::computeShapeFunctionGradients(real xi, real eta, real N[4][2]) {

  N[0][0] = -0.25*(1.0-eta);
  N[0][1] = -0.25*(1.0-xi);

  N[1][0] = 0.25*(1.0-eta);
  N[1][1] = -0.25*(1.0+xi);

  N[2][0] = 0.25*(1.0+eta);
  N[2][1] = 0.25*(1.0+xi);

  N[3][0] = -0.25*(1.0+eta);
  N[3][1] = 0.25*(1.0-xi);
}


void NonlinearBeamModel::computeLaminarBasis(int elem,real xi, real eta,
					     real R[4], real ex[2],
					     real ey[2],
					     real Rdef[4]) {

  real N[4][2];
  computeShapeFunctionGradients(xi, eta, N);

  real egx = 0.0,egy = 0.0;
  
  egx = slaveStates[elem].xminus[0]*N[0][0] + 
    slaveStates[elem+1].xminus[0]*N[1][0]+
    slaveStates[elem+1].xplus[0]*N[2][0]+
    slaveStates[elem].xplus[0]*N[3][0];
  
  egy = slaveStates[elem].xminus[1]*N[0][0] + 
    slaveStates[elem+1].xminus[1]*N[1][0]+
    slaveStates[elem+1].xplus[1]*N[2][0]+
    slaveStates[elem].xplus[1]*N[3][0];

  real mag = sqrt(egx*egx+egy*egy);

  ex[0] = egx/mag;
  ex[1] = egy/mag;

  ey[0] = -egy/mag;
  ey[1] = egx/mag;

  R[0] = ex[0];
  R[1] = ey[0];
  R[2] = ex[1];
  R[3] = ey[1];

  real undefRot = (1.0-eta)*0.5*beamNodes[elem].undeformedRotation + 
    (1.0+eta)*0.5*beamNodes[elem+1].undeformedRotation;

  real eundefx[2] = {sin(undefRot), -cos(undefRot)};
  real eundefy[2] = {cos(undefRot), sin(undefRot)};

  Rdef[0] = (ex[0]*eundefx[0]+ex[1]*eundefx[1]);
  Rdef[2] = (ey[0]*eundefx[0]+ey[1]*eundefx[1]);
  Rdef[1] = (ex[0]*eundefy[0]+ex[1]*eundefy[1]);
  Rdef[3] = (ey[0]*eundefy[0]+ey[1]*eundefy[1]);
}

void NonlinearBeamModel::computeLaminarComponents(int elem,real xi, real eta, real R[4],
						  real F[4], real E[4], real& J,
						  real Nx[4][2], real& Jeta) {

  
  real N[4][2];
  computeShapeFunctionGradients(xi, eta, N);

  real Xhat[4][2];

  Xhat[0][0] = R[0]*slaveStates[elem].Xminus[0] + R[2]*slaveStates[elem].Xminus[1];
  Xhat[0][1] = R[1]*slaveStates[elem].Xminus[0] + R[3]*slaveStates[elem].Xminus[1];

  Xhat[1][0] = R[0]*slaveStates[elem+1].Xminus[0] + R[2]*slaveStates[elem+1].Xminus[1];
  Xhat[1][1] = R[1]*slaveStates[elem+1].Xminus[0] + R[3]*slaveStates[elem+1].Xminus[1];
  
  Xhat[2][0] = R[0]*slaveStates[elem+1].Xplus[0] + R[2]*slaveStates[elem+1].Xplus[1];
  Xhat[2][1] = R[1]*slaveStates[elem+1].Xplus[0] + R[3]*slaveStates[elem+1].Xplus[1];
  
  Xhat[3][0] = R[0]*slaveStates[elem].Xplus[0] + R[2]*slaveStates[elem].Xplus[1];
  Xhat[3][1] = R[1]*slaveStates[elem].Xplus[0] + R[3]*slaveStates[elem].Xplus[1];

  real xhat[4][2];

  xhat[0][0] = R[0]*slaveStates[elem].xminus[0] + R[2]*slaveStates[elem].xminus[1];
  xhat[0][1] = R[1]*slaveStates[elem].xminus[0] + R[3]*slaveStates[elem].xminus[1];

  xhat[1][0] = R[0]*slaveStates[elem+1].xminus[0] + R[2]*slaveStates[elem+1].xminus[1];
  xhat[1][1] = R[1]*slaveStates[elem+1].xminus[0] + R[3]*slaveStates[elem+1].xminus[1];
  
  xhat[2][0] = R[0]*slaveStates[elem+1].xplus[0] + R[2]*slaveStates[elem+1].xplus[1];
  xhat[2][1] = R[1]*slaveStates[elem+1].xplus[0] + R[3]*slaveStates[elem+1].xplus[1];
  
  xhat[3][0] = R[0]*slaveStates[elem].xplus[0] + R[2]*slaveStates[elem].xplus[1];
  xhat[3][1] = R[1]*slaveStates[elem].xplus[0] + R[3]*slaveStates[elem].xplus[1];

  real uhat[4][2];

  uhat[0][0] = R[0]*slaveStates[elem].uminus[0] + R[2]*slaveStates[elem].uminus[1];
  uhat[0][1] = R[1]*slaveStates[elem].uminus[0] + R[3]*slaveStates[elem].uminus[1];

  uhat[1][0] = R[0]*slaveStates[elem+1].uminus[0] + R[2]*slaveStates[elem+1].uminus[1];
  uhat[1][1] = R[1]*slaveStates[elem+1].uminus[0] + R[3]*slaveStates[elem+1].uminus[1];
  
  uhat[2][0] = R[0]*slaveStates[elem+1].uplus[0] + R[2]*slaveStates[elem+1].uplus[1];
  uhat[2][1] = R[1]*slaveStates[elem+1].uplus[0] + R[3]*slaveStates[elem+1].uplus[1];
  
  uhat[3][0] = R[0]*slaveStates[elem].uplus[0] + R[2]*slaveStates[elem].uplus[1];
  uhat[3][1] = R[1]*slaveStates[elem].uplus[0] + R[3]*slaveStates[elem].uplus[1];

  // dX/deta
  real grad[4] = {Xhat[0][0]*N[0][0] + 
		  Xhat[1][0]*N[1][0]+
		  Xhat[2][0]*N[2][0]+
		  Xhat[3][0]*N[3][0],

		  Xhat[0][0]*N[0][1] + 
		  Xhat[1][0]*N[1][1]+
		  Xhat[2][0]*N[2][1]+
		  Xhat[3][0]*N[3][1],

		  Xhat[0][1]*N[0][0] + 
		  Xhat[1][1]*N[1][0]+
		  Xhat[2][1]*N[2][0]+
		  Xhat[3][1]*N[3][0],


		  Xhat[0][1]*N[0][1] + 
		  Xhat[1][1]*N[1][1]+
		  Xhat[2][1]*N[2][1]+
		  Xhat[3][1]*N[3][1]};
	
  // deta/dX
  real gradinv[4];
  
  real odet = 1.0/(grad[0]*grad[3] - grad[1]*grad[2]);

  gradinv[0] = odet*grad[3];
  gradinv[1] = -odet*grad[1];
  gradinv[2] = -odet*grad[2];
  gradinv[3] = odet*grad[0];

  // du/deta
  real gradu[4] = {uhat[0][0]*N[0][0] + 
		  uhat[1][0]*N[1][0]+
		  uhat[2][0]*N[2][0]+
		  uhat[3][0]*N[3][0],

		  uhat[0][0]*N[0][1] + 
		  uhat[1][0]*N[1][1]+
		  uhat[2][0]*N[2][1]+
		  uhat[3][0]*N[3][1],

		  uhat[0][1]*N[0][0] + 
		  uhat[1][1]*N[1][0]+
		  uhat[2][1]*N[2][0]+
		  uhat[3][1]*N[3][0],


		  uhat[0][1]*N[0][1] + 
		  uhat[1][1]*N[1][1]+
		  uhat[2][1]*N[2][1]+
		  uhat[3][1]*N[3][1]};


  // du/dX
  real fmi[4] = {gradu[0]*gradinv[0] + gradu[1]*gradinv[2],
		 gradu[0]*gradinv[1] + gradu[1]*gradinv[3],
		 gradu[2]*gradinv[0] + gradu[3]*gradinv[2],
		 gradu[2]*gradinv[1] + gradu[3]*gradinv[3]};
  
  F[0] = fmi[0] + 1.0;
  F[1] = fmi[1];
  F[2] = fmi[2];
  F[3] = fmi[3] + 1.0;

  E[0] = fmi[0]+0.5*(fmi[0]*fmi[0] + fmi[2]*fmi[2]);
  E[1] = 0.5*(fmi[1]+fmi[2] + fmi[0]*fmi[1] + fmi[2]*fmi[3]);
  E[2] = E[1];
  E[3] = fmi[3]+0.5*(fmi[1]*fmi[1] + fmi[3]*fmi[3]);

  J = F[0]*F[3]-F[1]*F[2];

  real Finv[4];
  real oj = 1.0/J;
  
  Finv[0] = F[3]*oj;
  Finv[1] = -F[1]*oj;
  Finv[2] = -F[2]*oj;
  Finv[3] = F[0]*oj;

  real NX[4][2];
  real ej[4] = {0,0,0,0};
  for (int k = 0; k < 4; ++k) {

    NX[k][0] = N[k][0]*gradinv[0]+N[k][1]*gradinv[2];
    NX[k][1] = N[k][0]*gradinv[1]+N[k][1]*gradinv[3];

    Nx[k][0] = NX[k][0]*Finv[0]+NX[k][1]*Finv[2];
    Nx[k][1] = NX[k][0]*Finv[1]+NX[k][1]*Finv[3];
    
    ej[0] += N[k][0]*xhat[k][0];
    ej[1] += N[k][1]*xhat[k][0];

    ej[2] += N[k][0]*xhat[k][1];
    ej[3] += N[k][1]*xhat[k][1];
    
    
  }
  
  Jeta = ej[0]*ej[3]-ej[1]*ej[2];
}

// ===================================================================================================================
/// \brief 
// ===================================================================================================================
void NonlinearBeamModel::
computeStressSVK(int elem,real xi, real eta, real R[4],
		 real F[4], real E[4], real J,
		 real sigma[4]) 
{

  real F21 = F[2], F12 = F[1], F22 = F[3], F11 = F[0];
  real r[2][2] = {{R[0],R[1]},{R[2],R[3]}};
  real e[2][2] = {{E[0],E[1]},{E[2],E[3]}};
  real ep[2][2];

  ep[0][0] = (r[0][0] * e[0][0] + r[1][0] * e[1][0]) * r[0][0] + (r[0][0] * e[0][1] + r[1][0] * e[1][1]) * r[1][0];
  ep[0][1] = (r[0][0] * e[0][0] + r[1][0] * e[1][0]) * r[0][1] + (r[0][0] * e[0][1] + r[1][0] * e[1][1]) * r[1][1];
  ep[1][0] = (r[0][1] * e[0][0] + r[1][1] * e[1][0]) * r[0][0] + (r[0][1] * e[0][1] + r[1][1] * e[1][1]) * r[1][0];
  ep[1][1] = (r[0][1] * e[0][0] + r[1][1] * e[1][0]) * r[0][1] + (r[0][1] * e[0][1] + r[1][1] * e[1][1]) * r[1][1];

  //std::cout << "ep = " << ep[0][0] << " " << ep[0][1] << " " << ep[1][1] << std::endl;

  
  real t1 = F21 * F21;
  real t2 = pow(r[0][0], 0.2e1);
  real t5 = t1 * r[0][0];
  real t10 = t1 * nu;
  real t11 = pow(r[1][0], 0.2e1);
  real t20 = F21 * F22;
  real t22 = r[1][0] * r[0][0] * ep[0][0];
  real t26 = r[1][0] * r[0][1] * ep[1][0];
  real t29 = r[0][0] * ep[0][1];
  real t30 = t29 * r[1][1];
  real t33 = t20 * nu;
  real t40 = F22 * F22;
  real t43 = t40 * r[1][0];
  real t48 = t40 * nu;
  real t56 = t1 * t2 * ep[0][0] + t5 * r[0][1] * ep[1][0] + t5 * ep[0][1] * r[0][1] + 
    t10 * t11 * ep[0][0] + t10 * r[1][0] * r[1][1] * ep[1][0] + 
    t10 * r[1][0] * ep[0][1] * r[1][1] + 0.2e1 * t20 * t22 + 0.2e1 * t20 * t26 + 
    0.2e1 * t20 * t30 - 0.2e1 * t33 * t22 - 0.2e1 * t33 * t26 - 0.2e1 * t33 * t30 + 
    t40 * t11 * ep[0][0] + t43 * r[1][1] * ep[1][0] + t43 * ep[0][1] * r[1][1] + 
    t48 * t2 * ep[0][0] + t48 * r[0][0] * r[0][1] * ep[1][0] + t48 * t29 * r[0][1];
  real t57 = pow(r[0][1], 0.2e1);
  real t59 = pow(r[1][1], 0.2e1);
  real t61 = r[0][1] * r[1][1];
  real t72 = -t56 / (t1 * t57 + t10 * t59 + 0.2e1 * t20 * t61 - 0.2e1 * t20 * t61 * nu + t40 * t59 + t48 * t57);

  ep[1][1] = t72;
  e[0][0] = (r[0][0] * ep[0][0] + r[0][1] * ep[1][0]) * r[0][0] + (r[0][0] * ep[0][1] + r[0][1] * ep[1][1]) * r[0][1];
  e[0][1] = (r[0][0] * ep[0][0] + r[0][1] * ep[1][0]) * r[1][0] + (r[0][0] * ep[0][1] + r[0][1] * ep[1][1]) * r[1][1];
  e[1][0] = (r[1][0] * ep[0][0] + r[1][1] * ep[1][0]) * r[0][0] + (r[1][0] * ep[0][1] + r[1][1] * ep[1][1]) * r[0][1];
  e[1][1] = (r[1][0] * ep[0][0] + r[1][1] * ep[1][0]) * r[1][0] + (r[1][0] * ep[0][1] + r[1][1] * ep[1][1]) * r[1][1];

  real Ed = Em / (1.0-nu*nu);
  real S11 = Ed*(e[0][0]+nu*e[1][1]);
  real S12 = Ed*(1.0-nu)*e[0][1];
  
  real S22 = Ed*(e[1][1]+nu*e[0][0]);

  real P[4];
  
  P[0] = S11*F[0]+S12*F[1];
  P[1] = S11*F[2]+S12*F[3];
  
  P[2] = S12*F[0]+S22*F[1];
  P[3] = S12*F[2]+S22*F[3];
  
  real S11hat = (r[0][0] * S11 + r[0][1] * S12) * r[0][0] + (r[0][0] * S12 + r[0][1] * S22) * r[0][1];


  real dzdZ = sqrt(1.0-2.0*S11hat*nu/Em);
  real oj = 1.0/(J*dzdZ*dzdZ);
  sigma[0] = oj*(F[0]*P[0]+F[1]*P[2]);
  sigma[1] = oj*(F[0]*P[1]+F[1]*P[3]);
  sigma[2] = sigma[1];
  sigma[3] = oj*(F[2]*P[1]+F[3]*P[3]);

  //if (elem == 12) {
  /*
   std::cout.precision(15);
    std::cout << "Elem = " << elem << std::endl;
    std::cout << sigma[0] << " " << sigma[1] << " " << sigma[2] << " " << sigma[3] << std::endl;
    std::cout << e[0][0] << " " << e[1][0] << " " << e[1][1] << std::endl;
    std::cout << F[0] << " " << F[1] << " " << F[2] << " " << F[3] << std::endl;
    std::cout << S11 << " " << S12 << " " << S22 << " " << J << std::endl;
    std::cout << "E = " << Em << " nu = " << nu << std::endl;
  */
    //exit(-1);
    //}
}

void NonlinearBeamModel::computeMaterialStiffness(real cg[2][2],
						  real Nxi, real Nyi,
						  real Nxj, real Nyj) {
 
  // I put <c,d> CSE < a, b> when I put this in maple
  real c = Nxi, d = Nyi;
  real a = Nxj, b = Nyj;
  real t1 = c * Em;
  real t3 = d * Em;
  real t4 = nu + 0.1e1;
  real t5 = t4 * b;
  real t8 = t4 * a;
  cg[0][0] = t1 * a + t3 * t5;
  cg[0][1] = t3 * t8;
  cg[1][0] = t1 * t5;
  cg[1][1] = t1 * t8;
}

void NonlinearBeamModel::computeGeometricStiffness(real kgeo[2][2],
						   real sigma[4],
						   real Nxi, real Nyi,
						   real Nxj, real Nyj) {

  real a = Nxi, b = Nyi;
  real c = Nxj,d = Nyj;
  real t9 = c * (a * sigma[0] + b * sigma[1]) + d * (a * sigma[2] + b * sigma[3]);

  kgeo[0][0] = kgeo[1][1] = t9;
  kgeo[0][1] = kgeo[1][0] = 0.0;
}

static real quadPoints3[3] = {-sqrt(3.0/5.0), 0.0, sqrt(3.0/5.0)};
static real quadWeights3[3] = {5.0/9.0,8.0/9.0,5.0/9.0};

static real quadPoints5[5] = {0.0, 
			      -0.5384693101056831,
			      0.5384693101056831,
			      -0.9061798459386640,
			      0.9061798459386640};

static real quadWeights5[5] = {0.5688888888888889,
			       0.4786286704993665,
			       0.4786286704993665,
			       0.2369268850561891,
			       0.2369268850561891};


void NonlinearBeamModel::computeInternalForce(int elem,RealArray& Fout,
					      RealArray& Kelem) {

  Kelem = 0.0;
  /*  int nQuadraturePoints = 3;

  real* pQuadPoints = quadPoints3;
  real* pQuadWeights = quadWeights3;
  */

  int nQuadraturePoints = 5;

  real* pQuadPoints = quadPoints5;
  real* pQuadWeights = quadWeights5;

  real R[4],ex[2],ey[2],Rdef[4];
  real J, Jeta, F[4], E[4], Nx[4][2];
  real sigma[4];
  real tmp[2],f[2];

  const static int loc_map[4] = {0,1,1,0};
  int nd;
  real Felem[4][2];

  for (int i = 0; i < nQuadraturePoints; ++i) {

    computeLaminarBasis(elem, 0.0, pQuadPoints[i], 
			R, ex, ey,Rdef);
			
		
    computeLaminarComponents(elem,0.0,  pQuadPoints[i], R,
			     F, E, J,Nx,Jeta);

    computeStressSVK(elem,0.0,  pQuadPoints[i], Rdef,
		     F, E, J,sigma) ;
    
    real h0 = (beamNodes[elem].thickness + beamNodes[elem+1].thickness)*0.5; // wdh : not used

    real hoverh0 = 0.5*sqrt((beamNodes[elem].p[0]+beamNodes[elem+1].p[0])*(beamNodes[elem].p[0]+beamNodes[elem+1].p[0]) + 
    	     (beamNodes[elem].p[1]+beamNodes[elem+1].p[1])*(beamNodes[elem].p[1]+beamNodes[elem+1].p[1]));
  
    // printF("computeInternalForce: Jeta=%9.3e\n",Jeta);
    
    //std::cout << "Jeta = " << Jeta << std::endl;
    
    real kmat[2][2],kgeo[2][2]={0,0,0,0},ktot[2][2];
    for (int k = 0; k < 4; ++k) {

      tmp[0] = Nx[k][0]*sigma[0] + Nx[k][1]*sigma[2];
      tmp[1] = Nx[k][0]*sigma[1] + Nx[k][1]*sigma[3];
      
      f[0] = R[0]*tmp[0] + R[1]*tmp[1];
      f[1] = R[2]*tmp[0] + R[3]*tmp[1];    

      //std::cout << "f = [" << f[0] << " " << f[1] << "]\n";
      Felem[k][0] = f[0]*2.0*pQuadWeights[i]*Jeta*hoverh0;
      Felem[k][1] = f[1]*2.0*pQuadWeights[i]*Jeta*hoverh0;
    }

    real sigma0[4] = {R[0]*R[0]*sigma[0] + 2.0*R[0]*R[1]*sigma[1],
			sigma[0]*R[0]*R[2]+(R[1]*R[2]+R[3]*R[0])*sigma[1],
			sigma[0]*R[0]*R[2]+(R[1]*R[2]+R[3]*R[0])*sigma[1],
			R[2]*R[2]*sigma[0]+2.0*R[2]*R[3]*sigma[1]};
		
    real r[2][2] = {{R[0],R[1]},{R[2],R[3]}};	

    real cg[2][2];

    for (int k = 0; k < 4; ++k) {
      for (int l = 0; l < 4; ++l) {

	computeMaterialStiffness(kmat, Nx[k][0], Nx[k][1],
				 Nx[l][0],Nx[l][1]);
	
	computeGeometricStiffness(kgeo,sigma ,Nx[k][0], Nx[k][1],
				  Nx[l][0],Nx[l][1]);
	


	ktot[0][0] = kmat[0][0]+kgeo[0][0];
	ktot[0][1] = kmat[0][1]+kgeo[0][1];
	ktot[1][1] = kmat[1][1]+kgeo[1][1];
	ktot[1][0] = kmat[1][0]+kgeo[1][0];

	real t3 = ktot[0][0] * r[0][0] + ktot[0][1] * r[0][1];
	real t7 = ktot[1][0] * r[0][0] + ktot[1][1] * r[0][1];
	real t12 = ktot[0][0] * r[1][0] + ktot[0][1] * r[1][1];
	real t16 = ktot[1][0] * r[1][0] + ktot[1][1] * r[1][1];
	cg[0][0] = r[0][0] * t3 + r[0][1] * t7;
	cg[0][1] = r[0][0] * t12 + r[0][1] * t16;
	cg[1][0] = r[1][0] * t3 + r[1][1] * t7;
	cg[1][1] = r[1][0] * t12 + r[1][1] * t16;


	Kelem(k*2,l*2)     += cg[0][0]*2.0*pQuadWeights[i]*Jeta*hoverh0;
	Kelem(k*2,l*2+1)   += cg[0][1]*2.0*pQuadWeights[i]*Jeta*hoverh0;
	Kelem(k*2+1,l*2+1) += cg[1][1]*2.0*pQuadWeights[i]*Jeta*hoverh0;
	Kelem(k*2+1,l*2)   += cg[1][0]*2.0*pQuadWeights[i]*Jeta*hoverh0;
      }
    }


    for (int k = 0; k < 4; ++k) {
      int nd = elem+loc_map[k];
      Fout(nd, 0) += Felem[k][0];
      Fout(nd, 1) += Felem[k][1];
      if (k == 0 || k == 1)
	Fout(nd,2) += ( beamNodes[nd].u[1] +beamNodes[nd].X[1] - slaveStates[nd].xminus[1])*Felem[k][0] + 
	  ( -beamNodes[nd].u[0] -beamNodes[nd].X[0] + slaveStates[nd].xminus[0])*Felem[k][1] ;
      else
	Fout(nd,2) += ( beamNodes[nd].u[1] +beamNodes[nd].X[1] - slaveStates[nd].xplus[1])*Felem[k][0] + 
	  ( -beamNodes[nd].u[0] -beamNodes[nd].X[0] + slaveStates[nd].xplus[0])*Felem[k][1] ;
    }
  }
      
}

void NonlinearBeamModel::computeInternalForce(RealArray& Fout,RealArray& KT) {
  
  Fout = 0.0;

  RealArray Kelem;
  Kelem.redim(8,8);

  RealArray cg;
  cg.redim(1,3,3);

  real k[4][4];
  for (int elem = 0; elem < numElem; ++elem) {
    computeInternalForce(elem, Fout,Kelem);
  
    const static int imap[2][2] = {{0,3},{1,2}};
    
    for (int i = 0; i < 2; ++i) {
      
      for (int j = 0; j < 2; ++j) {
		
	for (int m = 0; m < 2; ++m) {
	  for (int n = 0; n < 2; ++n) {

	    k[m*2][n*2]     = Kelem(imap[i][m]*2,imap[j][n]*2);
	    k[m*2+1][n*2]   = Kelem(imap[i][m]*2+1,imap[j][n]*2);
	    k[m*2+1][n*2+1] = Kelem(imap[i][m]*2+1,imap[j][n]*2+1);
	    k[m*2][n*2+1]   = Kelem(imap[i][m]*2,imap[j][n]*2+1);
	  }
	}      
	
	real p = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xminus[1];
	real q = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xminus[0]);
	real r = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xplus[1];
	real s = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xplus[0]);
	
	real x = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xminus[1];
	real y = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xminus[0]);
	real z = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xplus[1];
	real w = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xplus[0]);

	real t1 = 0.1e1 * k[0][0];
	real t2 = 0.1e1 * k[0][2];
	real t3 = 0.1e1 * k[2][0];
	real t4 = 0.1e1 * k[2][2];
	real t6 = 0.1e1 * k[0][1];
	real t7 = 0.1e1 * k[0][3];
	real t8 = 0.1e1 * k[2][1];
	real t9 = 0.1e1 * k[2][3];
	real t11 = k[0][0] * x;
	real t13 = k[0][1] * y;
	real t15 = k[0][2] * z;
	real t17 = k[0][3] * w;
	real t19 = k[2][0] * x;
	real t21 = k[2][1] * y;
	real t23 = k[2][2] * z;
	real t25 = k[2][3] * w;
	real t28 = 0.1e1 * k[1][0];
	real t29 = 0.1e1 * k[1][2];
	real t30 = 0.1e1 * k[3][0];
	real t31 = 0.1e1 * k[3][2];
	real t33 = 0.1e1 * k[1][1];
	real t34 = 0.1e1 * k[1][3];
	real t35 = 0.1e1 * k[3][1];
	real t36 = 0.1e1 * k[3][3];
	real t38 = k[1][0] * x;
	real t40 = k[1][1] * y;
	real t42 = k[1][2] * z;
	real t44 = k[1][3] * w;
	real t46 = k[3][0] * x;
	real t48 = k[3][1] * y;
	real t50 = k[3][2] * z;
	real t52 = k[3][3] * w;
	cg(0, 0, 0) = t1 + t2 + t3 + t4;
	cg(0, 0, 1) = t6 + t7 + t8 + t9;
	cg(0, 0, 2) = 0.1e1 * t11 + 0.1e1 * t13 + 0.1e1 * t15 + 0.1e1 * t17 + 0.1e1 * t19 + 0.1e1 * t21 + 0.1e1 * t23 + 0.1e1 * t25;
	cg(0, 1, 0) = t28 + t29 + t30 + t31;
	cg(0, 1, 1) = t33 + t34 + t35 + t36;
	cg(0, 1, 2) = 0.1e1 * t38 + 0.1e1 * t40 + 0.1e1 * t42 + 0.1e1 * t44 + 0.1e1 * t46 + 0.1e1 * t48 + 0.1e1 * t50 + 0.1e1 * t52;
	cg(0, 2, 0) = p * (t1 + t2) + q * (t28 + t29) + r * (t3 + t4) + s * (t30 + t31);
	cg(0, 2, 1) = p * (t6 + t7) + q * (t33 + t34) + r * (t8 + t9) + s * (t35 + t36);
	cg(0, 2, 2) = p * (t11 + t13 + t15 + t17) + q * (t38 + t40 + t42 + t44) + r * (t19 + t21 + t23 + t25) + s * (t46 + t48 + t50 + t52);

	Index ii(i*3,3),jj(j*3,3);
	KT(elem, ii,jj) = cg;
      }
    }
  }
}

void NonlinearBeamModel::computeBodyForce(int elem,RealArray& Fout) {

  /*
  int nQuadraturePoints = 3;

  real* pQuadPoints = quadPoints3;
  real* pQuadWeights = quadWeights3;
  */
  int nQuadraturePoints = 5;

  real* pQuadPoints = quadPoints5;
  real* pQuadWeights = quadWeights5;

  real R[4],ex[2],ey[2],Rdef[4];
  real J, Jeta, F[4], E[4], Nx[4][2],N[4];
  real sigma[4];
  real tmp[2],f[2];

  const static int loc_map[4] = {0,1,1,0};
  int nd;
  real Felem[4][2];

  for (int i = 0; i < nQuadraturePoints; ++i) {
    for (int j = 0; j < nQuadraturePoints; ++j) {

      computeShapeFunctions(pQuadPoints[i], pQuadPoints[j],N);

      computeLaminarBasis(elem,  pQuadPoints[i], pQuadPoints[j],
			  R, ex, ey,Rdef);
      
      
      computeLaminarComponents(elem, pQuadPoints[i], pQuadPoints[j], R,
			       F, E, J,Nx,Jeta);
      
      
      computeShapeFunctions(pQuadPoints[i], pQuadPoints[j],  N);
      
      real h0 = (beamNodes[elem].thickness + beamNodes[elem+1].thickness)*0.5; // wdh : not used 

      real hoverh0 = 0.5*sqrt((beamNodes[elem].p[0]+beamNodes[elem+1].p[0])*(beamNodes[elem].p[0]+beamNodes[elem+1].p[0]) + 
			      (beamNodes[elem].p[1]+beamNodes[elem+1].p[1])*(beamNodes[elem].p[1]+beamNodes[elem+1].p[1]));
  
      for (int k = 0; k < 4; ++k) {
	
	f[0] = N[k]*bodyForce[0]*density;
	f[1] = N[k]*bodyForce[1]*density;
	
	//std::cout << "f = [" << f[0] << " " << f[1] << "]\n";
	Felem[k][0] = f[0]*pQuadWeights[i]*pQuadWeights[j]*Jeta*hoverh0;
	Felem[k][1] = f[1]*pQuadWeights[i]*pQuadWeights[j]*Jeta*hoverh0;
	
      }
      
      for (int k = 0; k < 4; ++k) {
	int nd = elem+loc_map[k];
	Fout(nd, 0) += Felem[k][0];
	Fout(nd, 1) += Felem[k][1];
	if (k == 0 || k == 1)
	  Fout(nd,2) += ( beamNodes[nd].u[1] +beamNodes[nd].X[1] - slaveStates[nd].xminus[1])*Felem[k][0] + 
	    ( -beamNodes[nd].u[0] -beamNodes[nd].X[0] + slaveStates[nd].xminus[0])*Felem[k][1] ;
	else
	  Fout(nd,2) += ( beamNodes[nd].u[1] +beamNodes[nd].X[1] - slaveStates[nd].xplus[1])*Felem[k][0] + 
	    ( -beamNodes[nd].u[0] -beamNodes[nd].X[0] + slaveStates[nd].xplus[0])*Felem[k][1] ;
      }
    }

  }
}

void NonlinearBeamModel::computeBodyForce(RealArray& Fout) {
  
  for (int i = 0; i < numElem; ++i)
    computeBodyForce(i, Fout);
  
}

void NonlinearBeamModel::computeElementalMassMatrix(int elem, RealArray& Melem) {

  
  Melem = 0.0;

  /*
  int nQuadraturePoints = 3;

  real* pQuadPoints = quadPoints3;
  real* pQuadWeights = quadWeights3;
  */
  int nQuadraturePoints = 5;

  real* pQuadPoints = quadPoints5;
  real* pQuadWeights = quadWeights5;

  real Nx[4][2],J,Jeta,F[4],E[4],R[4],ex[2],ey[2],N[4],Rdef[4];
  for (int i = 0; i < nQuadraturePoints; ++i) {

    for (int j = 0; j < nQuadraturePoints; ++j) {
      
      computeShapeFunctions(pQuadPoints[i], pQuadPoints[j],  N);

      computeLaminarBasis(elem, pQuadPoints[i], pQuadPoints[j],
			  R, ex, ey,Rdef);
      
      computeLaminarComponents(elem,pQuadPoints[i],pQuadPoints[j], R,
			       F,  E, J,
			       Nx,Jeta);

      real h0 = (beamNodes[elem].thickness + beamNodes[elem+1].thickness)*0.5;  // wdh : not used 

      real hoverh0 = 0.5*sqrt((beamNodes[elem].p[0]+beamNodes[elem+1].p[0])*(beamNodes[elem].p[0]+beamNodes[elem+1].p[0]) + 
			      (beamNodes[elem].p[1]+beamNodes[elem+1].p[1])*(beamNodes[elem].p[1]+beamNodes[elem+1].p[1]));
      for (int k = 0; k < 4; ++k) {
	for (int l = 0; l < 4; ++l) {
	  Melem(k,l) += N[k]*N[l]*pQuadWeights[i]*pQuadWeights[j]*density*Jeta*hoverh0;
	}
      }
      
    }

  }
}

void NonlinearBeamModel::computeMassMatrix(int elem,RealArray& MT) {


  RealArray cg7;
  cg7.redim(1,3,3);
  RealArray Melem;
  Melem.redim(4,4);
  const static int imap[2][2] = {{0,3},{1,2}};
  computeElementalMassMatrix(elem, Melem);

  elementMassMatrices[elem] = Melem;

  for (int i = 0; i < 2; ++i) {

    for (int j = 0; j < 2; ++j) {
      
      real a = Melem(imap[i][0],imap[j][0]);
      real c = Melem(imap[i][1],imap[j][0]);
      real b = Melem(imap[i][0],imap[j][1]);
      real d = Melem(imap[i][1],imap[j][1]);
      

      real p = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xminus[1];
      real q = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xminus[0]);
      real r = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xplus[1];
      real s = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xplus[0]);
      
      real x = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xminus[1];
      real y = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xminus[0]);
      real z = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xplus[1];
      real w = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xplus[0]);
    
      
      real t1 = 0.1e1 * a;
      real t2 = 0.1e1 * b;
      real t3 = 0.1e1 * c;
      real t4 = 0.1e1 * d;
      real t5 = t1 + t2 + t3 + t4;
      real t6 = a * x;
      real t8 = b * z;
      real t10 = c * x;
      real t12 = d * z;
      real t15 = a * y;
      real t17 = b * w;
      real t19 = c * y;
      real t21 = d * w;
      real t24 = t1 + t2;
      real t26 = t3 + t4;
      cg7(0, 0, 0) = t5;
      cg7(0, 0, 1) = 0.0e0;
      cg7(0, 0, 2) = 0.1e1 * t6 + 0.1e1 * t8 + 0.1e1 * t10 + 0.1e1 * t12;
      cg7(0, 1, 0) = 0.0e0;
      cg7(0, 1, 1) = t5;
      cg7(0, 1, 2) = 0.1e1 * t15 + 0.1e1 * t17 + 0.1e1 * t19 + 0.1e1 * t21;
      cg7(0, 2, 0) = p * t24 + r * t26;
      cg7(0, 2, 1) = q * t24 + s * t26;
      cg7(0, 2, 2) = p * (t6 + t8) + q * (t15 + t17) + r * (t10 + t12) + s * (t19 + t21);

      Index ii(i*3,3),jj(j*3,3);
      MT(elem, ii,jj) = cg7;
    }
  }
}

void NonlinearBeamModel::computeExtraInertiaTerm(RealArray& t2) {

  RealArray t1(numNodes*4);

  t2.redim(numNodes,3);

  for (int i = 0; i < numNodes; ++i) {

    real p = beamNodes[i].u[1] +beamNodes[i].X[1] - slaveStates[i].xminus[1];
    real q = (beamNodes[i].u[0] +beamNodes[i].X[0] - slaveStates[i].xminus[0]);
    real r = beamNodes[i].u[1] +beamNodes[i].X[1] - slaveStates[i].xplus[1];
    real s = (beamNodes[i].u[0] +beamNodes[i].X[0] - slaveStates[i].xplus[0]);
      
    t1(i*4) =  beamNodes[i].angularVelocity*p;
    t1(i*4+1) =  beamNodes[i].angularVelocity*q;
    t1(i*4+2) =  beamNodes[i].angularVelocity*r;
    t1(i*4+3) =  beamNodes[i].angularVelocity*s;
  }

  t2 = 0.0;
  const static int imap[2][2] = {{0,3},{1,2}};

  for (int elem = 0; elem < numElem; ++elem) {
    for (int i = 0; i < 2; ++i) {
      
      for (int j = 0; j < 2; ++j) {
	
	RealArray& Melem = elementMassMatrices[elem];
	real a = Melem(imap[i][0],imap[j][0]);
	real c = Melem(imap[i][1],imap[j][0]);
	real b = Melem(imap[i][0],imap[j][1]);
	real d = Melem(imap[i][1],imap[j][1]);
	
	real p = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xminus[1];
	real q = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xminus[0]);
	real r = beamNodes[elem+i].u[1] +beamNodes[elem+i].X[1] - slaveStates[elem+i].xplus[1];
	real s = -(beamNodes[elem+i].u[0] +beamNodes[elem+i].X[0] - slaveStates[elem+i].xplus[0]);
	
	real x = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xminus[1];
	real y = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xminus[0]);
	real z = beamNodes[elem+j].u[1] +beamNodes[elem+j].X[1] - slaveStates[elem+j].xplus[1];
	real w = -(beamNodes[elem+j].u[0] +beamNodes[elem+j].X[0] - slaveStates[elem+j].xplus[0]);
	
	real om = beamNodes[elem+j].angularVelocity;
	
	real t1 = a * x;
	real t3 = b * z;
	real t5 = c * x;
	real t7 = d * z;
	real t11 = a * y;
	real t13 = b * w;
	real t15 = c * y;
	real t17 = d * w;
	t2( (elem+i),0) += (0.1e1 * t1 + 0.1e1 * t3 + 0.1e1 * t5 + 0.1e1 * t7) * om;
	t2( (elem+i),1) += (0.1e1 * t11 + 0.1e1 * t13 + 0.1e1 * t15 + 0.1e1 * t17) * om;
	t2( (elem+i),2) += (p * (t1 + t3) + q * (t11 + t13) + r * (t5 + t7) + s * (t15 + t17)) * om;
      }
    }
  }
  
}

void NonlinearBeamModel::initializeProjectedPoints(int sz) {

  projectedPoints = new ProjectedPoint[sz];
  numProjectedPoints = sz;
}

static void bezierInterpolation(real p0[2], real p3[2],real angle0, real angle3,
				real xi,real xyout[2]) {

  real le = 0.3333*sqrt((p3[0]-p0[0])*(p3[0]-p0[0])+(p3[1]-p0[1])*(p3[1]-p0[1]));

  real p1[2] = {p0[0] + le*sin(angle0), p0[1] - le*cos(angle0)};
  real p2[2] = {p3[0] - le*sin(angle3), p3[1] + le*cos(angle3)};
  
  real t = 0.5*xi+0.5;
  real ot = 1.0-t;
  
  for (int k = 0; k < 2; ++k)
    xyout[k] = p0[k]*ot*ot*ot + p1[k]*3.0*ot*ot*t+p2[k]*3.0*ot*t*t+t*t*t*p3[k];
  

}

static void bezierAccInterpolation(real p0[2], real p3[2],real angle0, real angle3,
				   real v0[2], real v3[2], real omega0,real omega3,
				   real a0[2], real a3[2], real alpha0,real alpha3,
				   real xi,real axyout[2]) {

  real le = 0.3333*sqrt((p3[0]-p0[0])*(p3[0]-p0[0])+(p3[1]-p0[1])*(p3[1]-p0[1]));

  real a1[2] = {a0[0] + le*(cos(angle0)*alpha0-sin(angle0)*omega0*omega0),
		a0[1] - le*(-sin(angle0)*alpha0-cos(angle0)*omega0*omega0)};
  
  real a2[2] = {a3[0] - le*(cos(angle3)*alpha3-sin(angle3)*omega3*omega3),
		a3[1] + le*(-sin(angle3)*alpha3-cos(angle3)*omega3*omega3)};

  real t = 0.5*xi+0.5;
  real ot = 1.0-t;
  
  for (int k = 0; k < 2; ++k)
    axyout[k] = a0[k]*ot*ot*ot + a1[k]*3.0*ot*ot*t+a2[k]*3.0*ot*t*t+t*t*t*a3[k];
  
}

void NonlinearBeamModel::projectInitialPoint(int id, real x, real y) {

  
  real minDist = 1.0e20;
  int minid_num = -1;
  real xi,eta;
  for (int i = 0; i < numElem; ++i) {

    real elem_tangent[2] = {beamNodes[i+1].X[0] - beamNodes[i].X[0],
			    beamNodes[i+1].X[1] - beamNodes[i].X[1]};
    real norm = sqrt(elem_tangent[0]*elem_tangent[0]+
		     elem_tangent[1]*elem_tangent[1]);

    elem_tangent[0] /= norm;
    elem_tangent[1] /= norm;

    real elem_normal[2] = {-elem_tangent[1],elem_tangent[0]};
    
    real vec[2] = {x-beamNodes[i].X[0],y-beamNodes[i].X[1]};
    
    real dist = vec[0]*elem_normal[0] + vec[1]*elem_normal[1];

    real tan_dist =  vec[0]*elem_tangent[0] + vec[1]*elem_tangent[1];

    real side = (dist > 0.0 ? 1.0 : -1.0);

    if (tan_dist < 0.0) {

      dist = side*sqrt((x-beamNodes[i].X[0])*(x-beamNodes[i].X[0])+
		  (y-beamNodes[i].X[1])*(y-beamNodes[i].X[1]));
      vec[0] = 0.0;
      vec[1] = 0.0;
    } else if (tan_dist > norm) {

      dist = side*sqrt((x-beamNodes[i+1].X[0])*(x-beamNodes[i+1].X[0])+
		  (y-beamNodes[i+1].X[1])*(y-beamNodes[i+1].X[1]));
      vec[0] = norm*elem_tangent[0];
      vec[1] = norm*elem_tangent[1];
      
    }

    if (fabs(dist) < fabs(minDist)) {

      minDist = dist;
      minid_num = i;
      xi = (vec[0]*elem_tangent[0] + vec[1]*elem_tangent[1])/norm;
      xi = 2.0*xi-1.0;
      real h = (1.0-xi)*0.5*beamNodes[minid_num].rotation + 
	(1.0+xi)*0.5*beamNodes[minid_num+1].rotation;
      eta = 2.0*minDist/h-1.0;
      if (eta > 1.0) eta = 1.0;
      if (eta < -1.0) eta = -1.0;
    }
    
  }

  projectedPoints[id].dist = minDist;
  projectedPoints[id].elem = minid_num;
  projectedPoints[id].xi = xi;
  projectedPoints[id].eta = eta;
  /*
  real N[4];
  computeShapeFunctions(projectedPoints[id].xi,projectedPoints[id].eta,
			N);

  real xp = slaveStates[minid_num].xminus[0]*N[0] + 
    slaveStates[minid_num+1].xminus[0]*N[1]+
    slaveStates[minid_num+1].xminus[0]*N[2] + 
    slaveStates[minid_num].xminus[0]*N[3];

  real yp = slaveStates[minid_num].xminus[1]*N[0] + 
    slaveStates[minid_num+1].xminus[1]*N[1]+
    slaveStates[minid_num+1].xminus[1]*N[2] + 
    slaveStates[minid_num].xminus[1]*N[3];
  */

  // Compute the initial projection location
  real rot = (1.0-xi)*0.5*beamNodes[minid_num].rotation + (1.0+xi)*0.5*beamNodes[minid_num+1].rotation;
  //real xp = (1.0-xi)*0.5*beamNodes[minid_num].X[0] + (1.0+xi)*0.5*beamNodes[minid_num+1].X[0];
  //real yp = (1.0-xi)*0.5*beamNodes[minid_num].X[1] + (1.0+xi)*0.5*beamNodes[minid_num+1].X[1];

  real p1[2];
  bezierInterpolation(beamNodes[minid_num].X, beamNodes[minid_num+1].X,
		      beamNodes[minid_num].rotation, beamNodes[minid_num+1].rotation,
		      xi,p1);

  real xp = p1[0], yp = p1[1];

  projectedPoints[id].projDelta[0] = xp + minDist*cos(rot) - x;
  projectedPoints[id].projDelta[1] = yp + minDist*sin(rot) - y;
  
  /*

  projectedPoints[id].projDelta[0] = xp - x;
  projectedPoints[id].projDelta[1] = yp - y;
  */
  
}


real lastproj[1000][2];

void NonlinearBeamModel::projectDisplacement(int id,  real& x, real& y) {
  
  int i = projectedPoints[id].elem;

  
  real elem_tangent[2] = {beamNodes[i+1].X[0] - beamNodes[i].X[0] +beamNodes[i+1].u[0] - beamNodes[i].u[0] ,
			  beamNodes[i+1].X[1] - beamNodes[i].X[1] +beamNodes[i+1].u[1] - beamNodes[i].u[1] };
  real norm = sqrt(elem_tangent[0]*elem_tangent[0]+
		   elem_tangent[1]*elem_tangent[1]);

  real lx0[2] = {beamNodes[i].X[0]+beamNodes[i].u[0],
		 beamNodes[i].X[1]+beamNodes[i].u[1] };

  real lx2[2] = {beamNodes[i+1].X[0]+beamNodes[i+1].u[0],
		 beamNodes[i+1].X[1]+beamNodes[i+1].u[1] };

  
  elem_tangent[0] /= norm;
  elem_tangent[1] /= norm;
  
  real elem_normal[2] = {-elem_tangent[1],elem_tangent[0]};
    
  //real xmid = (beamNodes[i+1].X[0]+beamNodes[i+1].u[0])*(projectedPoints[id].xi*0.5+0.5) +
  //  (beamNodes[i].u[0] + beamNodes[i].X[0])*(-projectedPoints[id].xi*0.5+0.5);
  
  //real ymid = (beamNodes[i+1].X[1]+beamNodes[i+1].u[1])*(projectedPoints[id].xi*0.5+0.5) +
  //  (beamNodes[i].u[1] + beamNodes[i].X[1])*(-projectedPoints[id].xi*0.5+0.5);
  
  real p1[2];
  bezierInterpolation(lx0,lx2,
		      beamNodes[i].rotation, beamNodes[i+1].rotation,
		      projectedPoints[id].xi,p1);

  real xmid = p1[0], ymid = p1[1];

  
  real rot = (beamNodes[i+1].rotation)*(projectedPoints[id].xi*0.5+0.5) +
  (beamNodes[i].rotation)*(-projectedPoints[id].xi*0.5+0.5);

  x = xmid + cos(rot)*projectedPoints[id].dist - projectedPoints[id].projDelta[0];
  y = ymid + sin(rot)*projectedPoints[id].dist - projectedPoints[id].projDelta[1];
  
  //std::cout << id << " " << rot << " " << x << " " << y << std::endl;
  /*
  std::cout << id << " " << x << " " << y << " " << 
    x-lastproj[id][0] << " " << y-lastproj[id][1] << std::endl;

  lastproj[id][0] = x;
  lastproj[id][1] = y;
  */

  /*
  real N[4];
  computeShapeFunctions(projectedPoints[id].xi,projectedPoints[id].eta,
			N);

  real xp = slaveStates[i].xminus[0]*N[0] + 
    slaveStates[i+1].xminus[0]*N[1]+
    slaveStates[i+1].xminus[0]*N[2] + 
    slaveStates[i].xminus[0]*N[3];

  real yp = slaveStates[i].xminus[1]*N[0] + 
    slaveStates[i+1].xminus[1]*N[1]+
    slaveStates[i+1].xminus[1]*N[2] + 
    slaveStates[i].xminus[1]*N[3];

  x = xp - projectedPoints[id].projDelta[0];
  y = yp - projectedPoints[id].projDelta[1];
  */
}


void NonlinearBeamModel::projectAcceleration(int id, real& ax, real& ay) {

  //std::cout << "Fix me! "<< std::endl;
  int i = projectedPoints[id].elem;

  real dx = beamNodes[i+1].X[0] - beamNodes[i].X[0];
  real dy = beamNodes[i+1].X[1] - beamNodes[i].X[1];
      
  real xmid = (beamNodes[i+1].X[0]+beamNodes[i+1].u[0])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].u[0] + beamNodes[i].X[0])*(-projectedPoints[id].xi*0.5+0.5);
  
  real xmidd =  (beamNodes[i+1].v[0])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].v[0])*(-projectedPoints[id].xi*0.5+0.5);

  real xmiddd =  (beamNodes[i+1].a[0])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].a[0])*(-projectedPoints[id].xi*0.5+0.5);
  
  real ymid = (beamNodes[i+1].X[1]+beamNodes[i+1].u[1])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].u[1] + beamNodes[i].X[1])*(-projectedPoints[id].xi*0.5+0.5);
  real ymidd = (beamNodes[i+1].v[1])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].v[1])*(-projectedPoints[id].xi*0.5+0.5);
  real ymiddd = (beamNodes[i+1].a[1])*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].a[1])*(-projectedPoints[id].xi*0.5+0.5);
  
  real rot = (beamNodes[i+1].rotation)*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].rotation)*(-projectedPoints[id].xi*0.5+0.5);
  real rotd = (beamNodes[i+1].angularVelocity)*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].angularVelocity)*(-projectedPoints[id].xi*0.5+0.5);
  real rotdd = (beamNodes[i+1].angularAcceleration)*(projectedPoints[id].xi*0.5+0.5) +
    (beamNodes[i].angularAcceleration)*(-projectedPoints[id].xi*0.5+0.5);


  real lx0[2] = {beamNodes[i].X[0]+beamNodes[i].u[0],
		 beamNodes[i].X[1]+beamNodes[i].u[1] };

  real lx2[2] = {beamNodes[i+1].X[0]+beamNodes[i+1].u[0],
		 beamNodes[i+1].X[1]+beamNodes[i+1].u[1] };

  real axy[2];
  bezierAccInterpolation(lx0,lx2,beamNodes[i].rotation,beamNodes[i+1].rotation,
			 beamNodes[i].v, beamNodes[i+1].v, 
			 beamNodes[i].angularVelocity, beamNodes[i+1].angularVelocity,
			 beamNodes[i].a, beamNodes[i+1].a,
			 beamNodes[i].angularAcceleration,
			 beamNodes[i+1].angularAcceleration,
			 projectedPoints[id].xi,axy);

  ax = axy[0] + (-rotdd*sin(rot)-rotd*rotd*cos(rot))*projectedPoints[id].dist;
  ay = axy[1] + (rotdd*cos(rot)-rotd*rotd*sin(rot))*projectedPoints[id].dist;

    //ax = xmiddd + (-rotdd*sin(rot)-rotd*rotd*cos(rot))*projectedPoints[id].dist;
    //ay = ymiddd + (rotdd*cos(rot)-rotd*rotd*sin(rot))*projectedPoints[id].dist;
  
  //ax = ay = 0.0;
  /*
  real N[4];
  computeShapeFunctions(projectedPoints[id].xi,projectedPoints[id].eta,
			N);

  real axp = slaveStates[i].aminus[0]*N[0] + 
    slaveStates[i+1].aminus[0]*N[1]+
    slaveStates[i+1].aminus[0]*N[2] + 
    slaveStates[i].aminus[0]*N[3];

  real ayp = slaveStates[i].aminus[1]*N[0] + 
    slaveStates[i+1].aminus[1]*N[1]+
    slaveStates[i+1].aminus[1]*N[2] + 
    slaveStates[i].aminus[1]*N[3];

  ax = axp;
  ay = ayp;
  */
}

// ================================================================================================================
//
// Return the (x,y) coordinates of the beam centerline
// wdh 2014/05/22
// ================================================================================================================
void NonlinearBeamModel::
getCenterLine( RealArray & xc ) const
{
  const int numberOfDimensions=2;  // *fix me*

  xc.redim(numNodes,numberOfDimensions);
  for( int i = 0; i < numNodes; i++ ) 
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
      xc(i,axis)=beamNodes[i].X[axis]+ beamNodes[i].u[axis];
  }
  // ::display(xc,"NonlinearBeamModel: xc","%6.3f ");
  
}



void NonlinearBeamModel::reevaluateMassMatrix() {

  M = 0.0;
  
  computeSlaveStates();
  for (int i = 0; i < numElem; ++i) {

    computeMassMatrix(i,M);
  }
}

// ================================================================================================================
/// \brief Predictor step.
// ================================================================================================================
void NonlinearBeamModel::
predictor(real dt)
{

  tipfile << t << " " << beamNodes[numElem].u[0] << " " << 
    beamNodes[numElem].u[1] <<" " << 
    beamNodes[numElem].rotation << std::endl;

  if( debug & 4 &&  useExactSolution) 
  {
    // -- output info about the exact solution ---

    RealArray xtmp(numNodes*2), vtmp(numNodes*2), atmp(numNodes*2);
    setExactSolution(t,xtmp,vtmp,atmp);

    std::stringstream profname;
    profname << "beam_profile" << time_step_num << ".txt";
    std::ofstream beam_profile(profname.str().c_str());
    for (int i = 0; i < numNodes; ++i) {
      
      double x = (double)i / (numNodes-1)*0.3;
      
      beam_profile << x << " " << xtmp(i*2) << " " << vtmp(i*2) << " " << atmp(i*2) << " " <<
	beamNodes[i].X[1]+beamNodes[i].u[1]-0.3 << " " << beamNodes[i].v[1] << " " << beamNodes[i].a[1];
      
      beam_profile << std::endl;
    }

    beam_profile.close();

    std::cout << "Wrote location for time " << t << " to " << profname.str() << std::endl;

  }

  t += dt;

  for (int i = 0; i < numNodes; ++i) {

    beamNodes[i].utilde[0] = beamNodes[i].u[0] + 
      beamNodes[i].v[0]*dt + dt*dt*0.5*(1.0-2.0*newmarkBeta)*beamNodes[i].a[0];
    
    beamNodes[i].utilde[1] = beamNodes[i].u[1] + 
      beamNodes[i].v[1]*dt + dt*dt*0.5*(1.0-2.0*newmarkBeta)*beamNodes[i].a[1];
    
    beamNodes[i].angletilde = beamNodes[i].rotation + 
      beamNodes[i].angularVelocity*dt + dt*dt*0.5*(1.0-2.0*newmarkBeta)*beamNodes[i].angularAcceleration;
				  
    
    beamNodes[i].vtilde[0] = beamNodes[i].v[0] + dt*(1.0-newmarkGamma)*beamNodes[i].a[0];
    
    beamNodes[i].vtilde[1] = beamNodes[i].v[1] + dt*(1.0-newmarkGamma)*beamNodes[i].a[1];
    
    beamNodes[i].angularVelocitytilde = beamNodes[i].angularVelocity + dt*(1.0-newmarkGamma)*beamNodes[i].angularAcceleration;
	 
    beamNodes[i].u[0] = beamNodes[i].utilde[0];
    beamNodes[i].u[1] = beamNodes[i].utilde[1];
    beamNodes[i]. rotation = beamNodes[i].angletilde;

    
    beamNodes[i].v[0] = beamNodes[i].vtilde[0];
    beamNodes[i].v[1] = beamNodes[i].vtilde[1];
    beamNodes[i].angularVelocity = beamNodes[i].angularVelocitytilde;
    
    memset(beamNodes[i].oldAccelerations,0,sizeof(beamNodes[i].oldAccelerations));
  }

  time_step_num++;


  computeSlaveStates();

  reevaluateMassMatrix();

  numCorrectorIterations = 0;
}

// =========================================================================================
/// \brief 
// =========================================================================================
void NonlinearBeamModel::
addForce(int i1, real p1, int i2, real p2) 
{

  int elem1,elem2;
  real eta1,eta2,t1,t2;

  real p11,p22;
  
  elem1 = projectedPoints[i1].elem;
  elem2 = projectedPoints[i2].elem;

  t1 = projectedPoints[i1].dist;
  t2 = projectedPoints[i2].dist;

  
  eta1 = projectedPoints[i1].xi;
  eta2 = projectedPoints[i2].xi;
  
  if (t1*t2 < 0.0)
    return;
  
  if (useExactSolution) {

    real x = (1.0-eta1)*0.5*beamNodes[elem1].X[0] + (1.0+eta1)*0.5*beamNodes[elem1+1].X[0];
    p1 = 0.0;//getExactPressure(t, x);
    
    x = (1.0-eta2)*0.5*beamNodes[elem2].X[0] + (1.0+eta2)*0.5*beamNodes[elem2+1].X[0];
    p2 = 0.0;//getExactPressure(t, x);
    
  }

  real sign = (t1 > 0.0 ? 1.0 : -1.0);

  p1 *= pressureNorm;
  p2 *= pressureNorm;

  if (elem1 > elem2 ||
      (elem1 == elem2 && eta1 > eta2)) {

    std::swap<real>(eta1,eta2);
    std::swap<int>(elem1,elem2);
    std::swap<real>(p1,p2);    
  }

  real deta = 2.0*(elem2-elem1) + (eta2-eta1);
  
  real etap;

  real f[2];
    
  real normal[2];

  for (int i = elem1; i <= elem2; ++i) {

    real a = eta1,b = eta2;
    real pa = p1, pb = p2;

    if (i != elem1) {
 
      etap = (1.0-eta1) + 2.0*(i-elem1-1);
      a = -1.0;
      pa = p1 + (p2-p1)*(etap)/(deta);
    }
    if (i != elem2) {
      b = 1.0;
      etap = (1.0-eta1) + 2.0*(i-elem1);
      pb = p1 + (p2-p1)*(etap)/(deta);
    }

    real t3 = 0.2500000000e0 * pa - 0.2500000000e0 * pb;
    real t4 = b * b;
    real t6 = a * a;
    real t9 = t4 * b - 0.1e1 * t6 * a;
    real t13 = t4 - 0.1e1 * t6;
    real t17 = b - 0.1e1 * a;
    real t19 = 0.2500000000e0 * pa * t17;
    real t21 = 0.2500000000e0 * pb * t17;
    f[0] = 0.3333333333e0 * t3 * t9 - 0.2500000000e0 * pa * t13 + t19 + t21;
    f[1] = -0.3333333333e0 * t3 * t9 + 0.2500000000e0 * pb * t13 + t19 + t21;

    f[0] *= -sign;
    f[1] *= -sign;

    real rot = (1.0-eta1)*0.5*beamNodes[i].rotation + 
      (1.0+eta1)*0.5*beamNodes[i+1].rotation;

    normal[0] = cos(rot);
    normal[1] = sin(rot);

    real dL;
    real x[2][2];
    if (sign > 0.0) {
      /*
      x[0][0] = (1.0-a)*0.5*slaveStates[i].xplus[0]+(1.0+a)*0.5*slaveStates[i+1].xplus[0];
      x[0][1] = (1.0-a)*0.5*slaveStates[i].xplus[1]+(1.0+a)*0.5*slaveStates[i+1].xplus[1];
      
      x[1][0] = (1.0-b)*0.5*slaveStates[i].xplus[0]+(1.0+b)*0.5*slaveStates[i+1].xplus[0];
      x[1][1] = (1.0-b)*0.5*slaveStates[i].xplus[1]+(1.0+b)*0.5*slaveStates[i+1].xplus[1];*/
      x[0][0] = slaveStates[i].xplus[0];
      x[0][1] = slaveStates[i].xplus[1];
      
      x[1][0] = slaveStates[i+1].xplus[0];
      x[1][1] = slaveStates[i+1].xplus[1];
    } else {
      x[0][0] = slaveStates[i].xminus[0];
      x[0][1] = slaveStates[i].xminus[1];
      
      x[1][0] = slaveStates[i+1].xminus[0];
      x[1][1] = slaveStates[i+1].xminus[1];

    }

    dL = sqrt((x[1][0]-x[0][0])*(x[1][0]-x[0][0]) + 
	      (x[1][1]-x[0][1])*(x[1][1]-x[0][1]));

    f[0] *= 0.5;
    f[1] *= 0.5;

    //std::cout << a << " " << b << " " << pa << " " << pb << " " << dL << " " << f[0] << " " << f[1] << std::endl;
      

    Ffluid(i, 0) += f[0]*dL*normal[0];
    Ffluid(i, 1) += f[0]*dL*normal[1];

    Ffluid(i+1, 0) += f[1]*dL*normal[0];
    Ffluid(i+1, 1) += f[1]*dL*normal[1];

    if (!useExactSolution) {
      if (sign < 0.0) {
	Ffluid(i,2) += ( beamNodes[i].u[1] +beamNodes[i].X[1] - slaveStates[i].xminus[1])*f[0]*normal[0]*dL + 
	  ( -beamNodes[i].u[0] -beamNodes[i].X[0] + slaveStates[i].xminus[0])*f[0]*normal[1]*dL ;
	
	Ffluid(i+1,2) += ( beamNodes[i+1].u[1] +beamNodes[i+1].X[1] - slaveStates[i+1].xminus[1])*f[1]*normal[0]*dL + 
	  ( -beamNodes[i+1].u[0] -beamNodes[i+1].X[0] + slaveStates[i+1].xminus[0])*f[1]*normal[1]*dL ;
	
      }
      else {
	Ffluid(i,2) += ( beamNodes[i].u[1] +beamNodes[i].X[1] - slaveStates[i].xplus[1])*f[0]*normal[0]*dL + 
	  ( -beamNodes[i].u[0] -beamNodes[i].X[0] + slaveStates[i].xplus[0])*f[0]*normal[1]*dL ;
	
	Ffluid(i+1,2) += ( beamNodes[i+1].u[1] +beamNodes[i+1].X[1] - slaveStates[i+1].xplus[1])*f[1]*normal[0]*dL + 
	  ( -beamNodes[i+1].u[0] -beamNodes[i+1].X[0] + slaveStates[i+1].xplus[0])*f[1]*normal[1]*dL ;
	
      }
    }

  }
    
}


void printArray(RealArray& F) {

  printArray(F,0,1000,0,1000,0,1000,0,1000,0,1000,0,1000);
}

void NonlinearBeamModel::
corrector(real dt) 
{

  RealArray Fint,u;
  u.redim(numNodes*3);
  RealArray flocal;
  flocal.redim(numNodes*3);

  RealArray dminusdtilde;
  dminusdtilde.redim(numNodes*3);
  dminusdtilde = 0.0;

  RealArray vcurrent(numNodes*3);

  real diff = 1000.0;
  /*
  if (useExactSolution) {

    RealArray xexact(numNodes*2), vexact(numNodes*2),
      aexact(numNodes*2);

    setExactSolution(t,xexact,vexact,aexact);    
    for (int i = 0; i < numNodes; ++i) {

      beamNodes[i].u[1] = xexact(i*2);
      beamNodes[i].v[1] = vexact(i*2);
      beamNodes[i].a[1] = aexact(i*2);      
    }

    return;
    
  }
  */

  //printArray(Ffluid);

  if (time_step_num == 1) {

    correctionHasConverged = true;
    return;
  }

  correctionHasConverged = false;

  if (isSteady) 
    dt = 0.0;

  int itcnt = 0;
  real r0 = diff;
  while (diff > 1.0e-13 && r0*1.0e-4 < diff)   // wdh: *check me*
  {

    computeSlaveStates();

    //reevaluateMassMatrix();

    Fint.redim(numNodes,3);

    computeInternalForce(Fint, K);

    Fext = 0.0;
    computeBodyForce(Fext);

    Fext += Ffluid;
    /*
    Fext = 0.0;
    
    real tl = -90.0*3.141592653589/180.0;
    Fext(numElem,0) = 1000.0*cos(tl);
    Fext(numElem,1) = 1000.0*sin(tl);
    */

    RealArray F = evaluate(Fint-Fext);

    //dt = 1.0;
    if (isSteady)
    {
      F *= -1.0;
    } 
    else 
    {

      RealArray extraInertiaF;
      computeExtraInertiaTerm(extraInertiaF);
      F -= extraInertiaF;

      F *= -newmarkBeta*dt*dt;
    }

    if (bcLeft == Cantilevered) {

      F(0,0) = F(0,1) = F(0,2) = 0.0;
    }

    if (bcRight == Cantilevered) {

      F(numElem,0) = F(numElem,1) = F(numElem,2) = 0.0;
    }


    if (bcLeft == Pinned) {

      F(0,0) = F(0,1) = 0.0;
    }

    if (bcRight == Pinned) {

      F(numElem,0) = F(numElem,1) = 0.0;
    }


    if( false )
    {
      display(M,sPrintF("Mass matrix"),"%8.2e ");
      display(K,sPrintF("Stiffness matrix"),"%8.2e ");
      OV_ABORT("temp");
    }
    

    //printArray(F);
    
    //printArray(Fint);

    /*


    Fint(0,0) = 0.0;
    Fint(0,1) = 0.0;
    Fint(0,2) = 0.0;

    Fint(numElem,0) = 0.0;//0.01;
    Fint(numElem,1) = 0.0;
    Fint(numElem,2) = 0.0;
    */
    for (int i = 0; i < numNodes; ++i) 
    {
      flocal(i*3) = F(i,0);
      flocal(i*3+1) = F(i,1);
      flocal(i*3+2) = F(i,2);

      vcurrent(i*3) = beamNodes[i].v[0];
      vcurrent(i*3+1) = beamNodes[i].v[1];
      vcurrent(i*3+2) = beamNodes[i].angularVelocity;
      
    }

    RealArray r = -1.0*flocal;
    if (isSteady) {
      solveBlockTridiagonal3(evaluate(M+K), flocal,
      		     u,bcLeft, bcRight);
      //printArray(K);
      //u = 1e-6*flocal;
    }
    else {

      for (int i = 0; i < numElem; ++i) {

	for (int j = 0; j < 6; ++j) {
	  
	  for (int k = 0; k < 6; ++k)
	    r(i*3+j) += M(i,j,k)*(dminusdtilde(i*3+k)) + 
	      (M(i,j,k)*rayleighAlpha+K(i,j,k)*rayleighBeta)*newmarkBeta*dt*dt*vcurrent(i*3+k);
	}
      }
      r *= -1.0;

      if (bcLeft == Cantilevered) {
	
	r(0) = r(1) = r(2) = 0.0;
      }
      
      if (bcRight == Cantilevered) {
	
	r(numElem*3) = r(numElem*3+1) = r(numElem*3+2) = 0.0;
      }
      
      
      if (bcLeft == Pinned) {
	
	r(0) = r(1) = 0.0;
      }
      
      if (bcRight == Pinned) {
	
	r(numElem*3) = r(numElem*3+1) = 0.0;
      }
      
      solveBlockTridiagonal3(evaluate((1.0+newmarkGamma*dt*rayleighAlpha)*M + (newmarkBeta*dt*dt+newmarkGamma*dt*rayleighBeta)*K), r,
			     u,bcLeft, bcRight);
    }

    //printArray(u);
    /*
    if (bcLeft == Cantilevered) {

      u(0) = u(1) = u(2) = 0.0;
    }

    if (bcRight == Cantilevered) {

      u(numElem*3) = u(numElem*3+1) = u(numElem*3+2) = 0.0;
    }


    if (bcLeft == Pinned) {

      u(0) = u(1) = 0.0;
    }

    if (bcRight == Pinned) {

      u(numElem*3) = u(numElem*3+1) = 0.0;
    }
    */

    dminusdtilde += u;
   
    if (isSteady) {
      for (int i = 0; i < numNodes; ++i) {
	
	u(i*3) += beamNodes[i].utilde[0];
	u(i*3+1) += beamNodes[i].utilde[1];
	
	u(i*3+2) += beamNodes[i].angletilde;
      }
    } else {
      for (int i = 0; i < numNodes; ++i) {
	
	u(i*3) = beamNodes[i].utilde[0] + dminusdtilde(i*3);
	u(i*3+1) = beamNodes[i].utilde[1] + dminusdtilde(i*3+1);
	
	u(i*3+2) = beamNodes[i].angletilde + dminusdtilde(i*3+2);
      }

    }

    diff = 0.0;
    for (int i = 0; i < numNodes; ++i) {
      
      //if (isSteady) {
	//diff += fabs(u(i*3)-beamNodes[i].u[0]);
	//diff += fabs(u(i*3+1)-beamNodes[i].u[1]);
	//diff += fabs(u(i*3+2)-beamNodes[i].rotation);
      //} else {

	diff += fabs(r(i*3)) + fabs(r(i*3+1))+ fabs(r(i*3+2));
	//}
      
      beamNodes[i].u[0] = (1.0-omegaStructure)*beamNodes[i].u[0] + omegaStructure*u(i*3);
      beamNodes[i].u[1] = (1.0-omegaStructure)*beamNodes[i].u[1] + omegaStructure*u(i*3+1);
      
      beamNodes[i].rotation = (1.0-omegaStructure)*beamNodes[i].rotation + omegaStructure*u(i*3+2);
      
      if (dt > 0.0) {
	beamNodes[i].a[0] = dminusdtilde(i*3)/(newmarkBeta*dt*dt);
	beamNodes[i].a[1] = dminusdtilde(i*3+1)/(newmarkBeta*dt*dt);

	//std::cout << i << " " << dminusdtilde(i*3+1) << " " << beamNodes[i].a[1] << std::endl;
      
	beamNodes[i].angularAcceleration = dminusdtilde(i*3+2)/(newmarkBeta*dt*dt);
      
      } else {
	beamNodes[i].a[0] = 0.0;
	beamNodes[i].a[1] = 0.0;
	beamNodes[i].angularAcceleration = 0.0;	
      }
      
      beamNodes[i].v[0] = beamNodes[i].vtilde[0] + newmarkGamma*dt*beamNodes[i].a[0];
      beamNodes[i].v[1] = beamNodes[i].vtilde[1] + newmarkGamma*dt*beamNodes[i].a[1];
      beamNodes[i].angularVelocity = beamNodes[i].angularVelocitytilde + newmarkGamma*dt*beamNodes[i].angularAcceleration;
      
      
    }
    if (isSteady)
      predictor(0.0);

    if ((isSteady && (itcnt % 100) == 0) || (debug & 4 && !isSteady && (itcnt % 5)==0 ) )
    {

      printF("NonlinearBeamModel: it=%i error = %8.2e\n",itcnt,diff);

      if (isSteady) {
	for (int i = 0; i < numNodes; ++i) {
	  
	  std::cout << "Node i ( " << beamNodes[i].X[0] << " " << beamNodes[i].X[1] << "; " << beamNodes[i].undeformedRotation << ")\n";
	  std::cout << "-> (" << beamNodes[i].X[0]+beamNodes[i].u[0] << " " << beamNodes[i].X[1]+beamNodes[i].u[1]  << "; " << 
	    beamNodes[i].rotation << std::endl;
	}
      }
    }

    if (itcnt == 0)
      r0 = diff;
    
    ++itcnt;

    if (!isSteady && itcnt > 100)
      break;
  }

    if (isSteady) {
      for (int i = 0; i < numNodes; ++i) {

	std::cout << "Node i ( " << beamNodes[i].X[0] << " " << beamNodes[i].X[1] << "; " << beamNodes[i].undeformedRotation << ")\n";
	std::cout << "-> (" << beamNodes[i].X[0]+beamNodes[i].u[0] << " " << beamNodes[i].X[1]+beamNodes[i].u[1]  << "; " << 
	  beamNodes[i].rotation << std::endl;
      }
      std::cout << "total iterations = " << itcnt << std::endl;
      exit(0);
    }

  double correction = 0.0;
  for (int i = 0; i < numNodes; ++i) {
      
    correction += (beamNodes[i].a[0]-beamNodes[i].oldAccelerations[0])*(beamNodes[i].a[0]-beamNodes[i].oldAccelerations[0]);
    correction += (beamNodes[i].a[1]-beamNodes[i].oldAccelerations[1])*(beamNodes[i].a[1]-beamNodes[i].oldAccelerations[1]);
    correction += (beamNodes[i].angularAcceleration-beamNodes[i].oldAccelerations[2])*
      (beamNodes[i].angularAcceleration-beamNodes[i].oldAccelerations[2]);

    beamNodes[i].a[0] = (1.0-added_mass_relaxation)*beamNodes[i].oldAccelerations[0] + 
      (added_mass_relaxation)*beamNodes[i].a[0];
    beamNodes[i].a[1] = (1.0-added_mass_relaxation)*beamNodes[i].oldAccelerations[1] + 
      (added_mass_relaxation)*beamNodes[i].a[1];
    beamNodes[i].angularAcceleration = (1.0-added_mass_relaxation)*beamNodes[i].oldAccelerations[2] + 
      (added_mass_relaxation)*beamNodes[i].angularAcceleration;

    beamNodes[i].oldAccelerations[0] = beamNodes[i].a[0];
    beamNodes[i].oldAccelerations[1] = beamNodes[i].a[1];
    beamNodes[i].oldAccelerations[2] = beamNodes[i].angularAcceleration;

  }

  correction = sqrt(correction);

  if (numCorrectorIterations == 0)
    initialResidual = correction;

  ++numCorrectorIterations;

  if( debug & 1 )
    printF("NonlinearBeamModel:correct: t=%9.3e, it=%i error = %8.2e\n",t,itcnt,diff);

  // std::cout << "correction value = " << correction << std::endl;
  if( debug & 4) 
    printF("NonlinearBeamModel:correct:numCorrectorIterations=%i:  initialResidual=%8.2e, correction=%8.2e <? %8.2e\n",numCorrectorIterations,initialResidual,correction,initialResidual*convergenceTolerance);
  
  if (correction < initialResidual*convergenceTolerance || correction < 1e-8)
    correctionHasConverged = true;
}


// ==================================================================================
/// \brief 
// ==================================================================================
void NonlinearBeamModel::
addBodyForce( const real bf[2]) 
{

  bodyForce[0] = bf[0];
  bodyForce[1] = bf[1];
}

// ==================================================================================
/// \brief 
// ==================================================================================
void NonlinearBeamModel::
resetForce()
{
  Ffluid = 0.0;
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


// standing wave: 
#define W(x,t) (a0*sin(k*(x))*cos(w*(t)))
#define Wt(x,t) (-w*a0*sin(k*(x))*sin(w*(t)))
#define Wx(x,t) (w*a0*cos(k*(x))*cos(w*(t)))
#define Wtx(x,t) (-w*a0*cos(k*(x))*sin(w*(t)))
#define Wtt(x,t) (-w*w*a0*sin(k*(x))*cos(w*(t)))
#define Wttx(x,t) (-w*w*a0*k*cos(k*(x))*cos(w*(t)))

void NonlinearBeamModel::
setExactSolution(double t,RealArray& x, RealArray& v, RealArray& a) 
{
  
  if( exactSolution==standingWave )
  {
    // --- parameters for the exact solution ---
    OV_real a0=.1;  // amplitude 
    OV_real k0=2.;
    OV_real k=Pi*k0;

    // *** fix me : 
    OV_real E=Em,  breadth=beamThickness, thickness=beamThickness;
    OV_real rho=density; 
    OV_real Ioverb= pow(thickness,3.)/12.;  // what should his be ?   Integral_{-h/2}^{h/2}  y^2 dydz  = (1/12) h^3 b 
    
    // real w = sqrt( E*momOfIntertia*pow(k,4)/( rho*thickness*breadth ) );
    OV_real w = sqrt( E*Ioverb*pow(k,4)/( rho*thickness ) );  // note: breadth scales out 

    printF("NonlinearBeamModel::setExactSolution: standing wave: k=%9.3e, w=%9.3e (rho=%8.2e, E=%8.2e, h=%8.2e, I/b=%8.2e)\n",
           k,w,rho,E,thickness,Ioverb);

    for (int i = 0; i <= numElem; ++i) 
    {
      double xl = (double)i / numElem*beamLength;
    
      x(i*2) = W(xl,t);
      x(i*2+1) = Wx(xl,t);
    
      v(i*2) = Wt(xl,t);   // initial velocity is zero 
      v(i*2+1) = Wtx(xl,t);
    
      // Acceleration: 
      a(i*2) = Wtt(xl,t);
      a(i*2+1) = Wttx(xl,t);
    }

  }
  else if( exactSolution==fluidStructureTravelingWave )
  {
    // "Exact" solution for a traveling wave between an incompressible fluid and a beam

    double L = 0.30;

    double elasticModulus = 1.4e6;

    double h=0.02;
    double Ioverb=6.6667e-7;  // wdh: I/b = h^3/12 = 6.6667e-7
    double H=0.3;
    double k=2.0*3.141592653589/L;
    double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
    double what = 0.00001;
    double omegar = 0.8907148069, omegai = -0.9135887123e-2;
    std::complex<LocalReal> omega_tilde(omegar, omegai);
    std::complex<LocalReal> I(0.0,1.0);

    omega_tilde = 1.0;

    for (int i = 0; i <= numElem; ++i) 
    {

      double xl = (double)i / numElem*L;
    
      std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);
      std::complex<LocalReal> fp = I*k*(exp(I*k*xl-I*omega_tilde*omega0*t)+exp(-I*k*xl-I*omega_tilde*omega0*t));
    

    
      x(i*2) = 2.0*(what*f).real();
      x(i*2+1) = 2.0*(what*fp).real();
    
      v(i*2) = 2.0*(-what*f*I*omega_tilde*omega0).real();
      v(i*2+1) = 2.0*(-what*fp*I*omega_tilde*omega0).real();
    
      a(i*2) = 2.0*(-what*f*omega_tilde*omega0*omega_tilde*omega0).real();
      a(i*2+1) = 2.0*(-what*fp*omega_tilde*omega0*omega_tilde*omega0).real();
    }
  }
  else
  {
    OV_ABORT("ERROR: unknown exact solution");
  }
  

}

double NonlinearBeamModel::getExactPressure(double t, double xl) {

  double h=0.02;
  double Ioverb=6.6667e-7;
  double nu = 0.001;
  double H=0.3;
  double L = 0.30;

  double elasticModulus = 1.4e6;
  std::complex<LocalReal> I(0.0,1.0);
  double omegar = 0.8907148069, omegai = -0.9135887123e-2;
  std::complex<LocalReal> omega_tilde(omegar, omegai);
  double k=2.0*3.141592653589/L;
  double omega0=sqrt(elasticModulus*Ioverb*k*k*k*k/(density*h));
   
  LocalReal beta = omega0/(k*k)/nu;
  std::complex<LocalReal> alpha = k*sqrt(-I*beta*omega_tilde+1.0);

  double what = 0.00001;
  std::complex<LocalReal> omega = omega_tilde*omega0;
  
  
  std::complex<LocalReal> f = exp(I*k*xl-I*omega_tilde*omega0*t)-exp(-I*k*xl-I*omega_tilde*omega0*t);

  std::complex<LocalReal> a = phi2d(alpha,k,H),b = phi1d(alpha,k,H);
  std::complex<LocalReal> A = -1.0*what*omega*omega*alpha*0.5/k;
  std::complex<LocalReal> c = a/b;
  A /= (-c*phi1(alpha,k,H)+phi2(alpha,k,H));
  std::complex<LocalReal> phat = A*(cosh(k*H)-c/alpha*sinh(k*H));

  //std::cout << xl << " " << 2.0*(f*phat).real() << std::endl;

  LocalReal result = 2.0*(f*phat).real();

  std::complex<LocalReal> r1 = (elasticModulus*Ioverb*k*k*k*k-density*h*omega*omega)*what, r2 = phat*1000.0 ;
  ///std::cout << r1 << " " << r2 << std::endl;

  return result;

}

void NonlinearBeamModel::setAddedMassRelaxation(double omega) {

  added_mass_relaxation = omega;
}

bool NonlinearBeamModel::hasCorrectionConverged() const {

  //return true;
  return correctionHasConverged;
}

void NonlinearBeamModel::setSubIterationConvergenceTolerance(double tol) {

  convergenceTolerance = tol;
}

int NonlinearBeamModel::
get( const GenericDataBase & dir, const aString & name)
{  


  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"NonlinearBeamModel");

  aString className;
  subDir.get( className,"className" );

  subDir.get( numNodes, "numNodes" );
  subDir.get( numElem, "numElem");
  subDir.get( isSteady, "isSteady");
  subDir.get( useExactSolution, "useExactSolution");
  subDir.get( numProjectedPoints, "numProjectedPoints");

  if (beamNodes)
    delete [] beamNodes;

  if (slaveStates)
    delete [] slaveStates;

  if (projectedPoints)
    delete [] projectedPoints;
  

  beamNodes = new BeamNode[numNodes];
  slaveStates = new SlaveState[numNodes];
  projectedPoints = new ProjectedPoint[numProjectedPoints];

  // # of reals in the beam node class to be stored
  const unsigned int numBR = 22;

  RealArray beamNodePackage;//(numNodes*numBR);

  subDir.get( beamNodePackage, "beamNodes");

  for (int i = 0; i < numNodes; ++i) {

    memcpy(beamNodes[i].X,&beamNodePackage(i*numBR),  sizeof(beamNodes[i].X));
    memcpy( beamNodes[i].u,&beamNodePackage(i*numBR+3), sizeof(beamNodes[i].u));
    memcpy(&beamNodes[i].undeformedRotation,&beamNodePackage(i*numBR+6), 
	   sizeof(beamNodes[i].undeformedRotation));
    memcpy(&beamNodes[i].rotation,&beamNodePackage(i*numBR+7), 
	   sizeof(beamNodes[i].rotation));
    memcpy( beamNodes[i].p0,&beamNodePackage(i*numBR+8),
	   sizeof(beamNodes[i].p0));
    memcpy(beamNodes[i].p,&beamNodePackage(i*numBR+10), 
	   sizeof(beamNodes[i].p));
    memcpy( &beamNodes[i].thickness,&beamNodePackage(i*numBR+13),
	   sizeof(beamNodes[i].thickness));
    memcpy(beamNodes[i].v,&beamNodePackage(i*numBR+14), 
	   sizeof(beamNodes[i].v));
    memcpy( beamNodes[i].a,&beamNodePackage(i*numBR+17),
	   sizeof(beamNodes[i].a));
    memcpy(&beamNodes[i].angularVelocity,&beamNodePackage(i*numBR+20), 
	   sizeof(beamNodes[i].angularVelocity));
    memcpy( &beamNodes[i].angularAcceleration,&beamNodePackage(i*numBR+21),
	   sizeof(beamNodes[i].angularAcceleration));    
  }

  
  // # of reals in the slave state class to be stored
  const unsigned int numSR = 30;
  RealArray slaveStatePackage;//(numNodes*numSR);

  subDir.get( slaveStatePackage, "slaveStates");
  for (int i = 0; i < numNodes; ++i) {

    memcpy( slaveStates[i].uplus,&slaveStatePackage(i*numSR), sizeof(slaveStates[i].uplus));
    memcpy( slaveStates[i].uminus,&slaveStatePackage(i*numSR+3), sizeof(slaveStates[i].uminus));
    memcpy(slaveStates[i].xplus, &slaveStatePackage(i*numSR+6), sizeof(slaveStates[i].xplus));
    memcpy( slaveStates[i].xminus,&slaveStatePackage(i*numSR+9), sizeof(slaveStates[i].xminus));
    memcpy( slaveStates[i].Xplus, &slaveStatePackage(i*numSR+12),sizeof(slaveStates[i].Xplus));
    memcpy(slaveStates[i].Xminus,&slaveStatePackage(i*numSR+15),  sizeof(slaveStates[i].Xminus));
    memcpy(slaveStates[i].vplus,&slaveStatePackage(i*numSR+18),  sizeof(slaveStates[i].vplus));
    memcpy( slaveStates[i].vminus,&slaveStatePackage(i*numSR+21), sizeof(slaveStates[i].vminus));
    memcpy(slaveStates[i].aplus,&slaveStatePackage(i*numSR+24),  sizeof(slaveStates[i].aplus));
    memcpy( slaveStates[i].aminus,&slaveStatePackage(i*numSR+27), sizeof(slaveStates[i].aminus));
    
  }

  
  intSerialArray projPointElemArray;//(numNodes);
  RealArray projPointRealArray;//(numNodes*4);
  subDir.get( projPointElemArray, "projPointElemArray");
  subDir.get( projPointRealArray, "projPointRealArray");
  for (int i = 0; i < numProjectedPoints; ++i) {

    projectedPoints[i].elem = projPointElemArray(i);
    projectedPoints[i].dist = projPointRealArray(i*5);
    projectedPoints[i].xi = projPointRealArray(i*5+1);
    projectedPoints[i].eta = projPointRealArray(i*5+2);
    projectedPoints[i].projDelta[0] = projPointRealArray(i*5+3);
    projectedPoints[i].projDelta[1] = projPointRealArray(i*5+4);
  }


  subDir.get( density, "density");
  subDir.get( nu, "nu");
  subDir.get( Em, "Em");
  
  //subDir.get(M, "massMatrices");
  //subDir.get(K, "stiffnessMatrices");
  
  subDir.get( newmarkBeta, "newmarkBeta");
  subDir.get( newmarkGamma, "newmarkGamma");

  subDir.get(omegaStructure, "omegaStructure");

  int tv;
  subDir.get(tv, "bcLeft");
  bcLeft = (BoundaryCondition)tv;

  subDir.get(tv, "bcRight");
  bcRight = (BoundaryCondition)tv;

  subDir.get(bodyForce, "bodyForce",2);
  
  subDir.get(t, "t");

  subDir.get(pressureNorm, "pressureNorm");

  subDir.get(time_step_num, "time_step_num");
  
  subDir.get(added_mass_relaxation, "added_mass_relaxation");

  subDir.get(convergenceTolerance, "convergenceTolerance");

  subDir.get(initialResidual, "initialResidual");

  subDir.get(rayleighAlpha, "rayleighAlpha");

  subDir.get(rayleighBeta, "rayleighBeta");


  M.redim(numElem,6,6);
  K.redim(numElem,6,6);
  Fext.redim(numNodes,3);

  Fext = 0.0;

  Ffluid.redim(numNodes,3);
  Ffluid = 0.0;
  
  if (elementMassMatrices)
    delete [] elementMassMatrices;

  elementMassMatrices = new RealArray[numElem];

  reevaluateMassMatrix();


  delete &subDir;

  return 0;  
}

int NonlinearBeamModel::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"NonlinearBeamModel");                 // create a sub-directory 

  aString className="NonlinearBeamModel";
  subDir.put( className,"className" );

  subDir.put( numNodes, "numNodes" );
  subDir.put( numElem, "numElem");
  subDir.put( isSteady, "isSteady");
  subDir.put( useExactSolution, "useExactSolution");
  subDir.put( numProjectedPoints, "numProjectedPoints");

  // # of reals in the beam node class to be stored
  const unsigned int numBR = 22;

  RealArray beamNodePackage(numNodes*numBR);

  for (int i = 0; i < numNodes; ++i) {

    memcpy(&beamNodePackage(i*numBR), beamNodes[i].X, sizeof(beamNodes[i].X));
    memcpy(&beamNodePackage(i*numBR+3), beamNodes[i].u, sizeof(beamNodes[i].u));
    memcpy(&beamNodePackage(i*numBR+6), &beamNodes[i].undeformedRotation,
	   sizeof(beamNodes[i].undeformedRotation));
    memcpy(&beamNodePackage(i*numBR+7), &beamNodes[i].rotation,
	   sizeof(beamNodes[i].rotation));
    memcpy(&beamNodePackage(i*numBR+8), beamNodes[i].p0,
	   sizeof(beamNodes[i].p0));
    memcpy(&beamNodePackage(i*numBR+10), beamNodes[i].p,
	   sizeof(beamNodes[i].p));
    memcpy(&beamNodePackage(i*numBR+13), &beamNodes[i].thickness,
	   sizeof(beamNodes[i].thickness));
    memcpy(&beamNodePackage(i*numBR+14), beamNodes[i].v,
	   sizeof(beamNodes[i].v));
    memcpy(&beamNodePackage(i*numBR+17), beamNodes[i].a,
	   sizeof(beamNodes[i].a));
    memcpy(&beamNodePackage(i*numBR+20), &beamNodes[i].angularVelocity,
	   sizeof(beamNodes[i].angularVelocity));
    memcpy(&beamNodePackage(i*numBR+21), &beamNodes[i].angularAcceleration,
	   sizeof(beamNodes[i].angularAcceleration));    
  }

  subDir.put( beamNodePackage, "beamNodes");
  
  // # of reals in the slave state class to be stored
  const unsigned int numSR = 30;
  RealArray slaveStatePackage(numNodes*numSR);
  for (int i = 0; i < numNodes; ++i) {

    memcpy(&slaveStatePackage(i*numSR), slaveStates[i].uplus, sizeof(slaveStates[i].uplus));
    memcpy(&slaveStatePackage(i*numSR+3), slaveStates[i].uminus, sizeof(slaveStates[i].uminus));
    memcpy(&slaveStatePackage(i*numSR+6), slaveStates[i].xplus, sizeof(slaveStates[i].xplus));
    memcpy(&slaveStatePackage(i*numSR+9), slaveStates[i].xminus, sizeof(slaveStates[i].xminus));
    memcpy(&slaveStatePackage(i*numSR+12), slaveStates[i].Xplus, sizeof(slaveStates[i].Xplus));
    memcpy(&slaveStatePackage(i*numSR+15), slaveStates[i].Xminus, sizeof(slaveStates[i].Xminus));
    memcpy(&slaveStatePackage(i*numSR+18), slaveStates[i].vplus, sizeof(slaveStates[i].vplus));
    memcpy(&slaveStatePackage(i*numSR+21), slaveStates[i].vminus, sizeof(slaveStates[i].vminus));
    memcpy(&slaveStatePackage(i*numSR+24), slaveStates[i].aplus, sizeof(slaveStates[i].aplus));
    memcpy(&slaveStatePackage(i*numSR+27), slaveStates[i].aminus, sizeof(slaveStates[i].aminus));
    
  }

  subDir.put( slaveStatePackage, "slaveStates");
  
  intSerialArray projPointElemArray(numProjectedPoints);
  RealArray projPointRealArray(numProjectedPoints*5);
  for (int i = 0; i < numProjectedPoints; ++i) {

    projPointElemArray(i) = projectedPoints[i].elem;
    projPointRealArray(i*5) = projectedPoints[i].dist;
    projPointRealArray(i*5+1) = projectedPoints[i].xi;
    projPointRealArray(i*5+2) = projectedPoints[i].eta;
    projPointRealArray(i*5+3) = projectedPoints[i].projDelta[0];
    projPointRealArray(i*5+4) = projectedPoints[i].projDelta[1];
  }

  subDir.put( projPointElemArray, "projPointElemArray");
  subDir.put( projPointRealArray, "projPointRealArray");

  subDir.put( density, "density");
  subDir.put( nu, "nu");
  subDir.put( Em, "Em");
  
  //subDir.put(M, "massMatrices");
  //subDir.put(K, "stiffnessMatrices");
  
  subDir.put( newmarkBeta, "newmarkBeta");
  subDir.put( newmarkGamma, "newmarkGamma");

  subDir.put(omegaStructure, "omegaStructure");

  subDir.put((int)bcLeft, "bcLeft");
  subDir.put((int)bcRight, "bcRight");

  subDir.put(bodyForce, "bodyForce",2);
  
  subDir.put(t, "t");

  subDir.put(pressureNorm, "pressureNorm");

  subDir.put(time_step_num, "time_step_num");
  
  subDir.put(added_mass_relaxation, "added_mass_relaxation");

  subDir.put(convergenceTolerance, "convergenceTolerance");

  subDir.put(initialResidual, "initialResidual");

  subDir.put(rayleighAlpha, "rayleighAlpha");

  subDir.put(rayleighBeta, "rayleighBeta");


  delete &subDir;
  return 0;  
}


// =================================================================================================
/// \brief Plot the beam
// =================================================================================================
int NonlinearBeamModel::
plot(GenericGraphicsInterface & gi, GraphicsParameters & psp )
{

  printF("NonlinearBeamModel::plot...\n");

  OV_real lineWidth=2;
  // psp.get(GraphicsParameters::lineWidth,lineWidthSave);  // default is 1
  // psp.set(GraphicsParameters::lineWidth,lineWidth);  

  // *** do this for now **
  RealArray pb(2,3);  // plot bounds 
  pb=-.1; pb(1,Range(0,2))=1.1;

  pb(0,0)=0.; pb(1,0)=beamLength;
  pb(0,1)=-.2;  pb(1,1)=.2;

  psp.set(GI_PLOT_BOUNDS, pb);
  psp.set(GI_USE_PLOT_BOUNDS, true);

  for( int curve=0; curve<3; curve++ )
  {
    RealArray xc;
    aString buff;
    if( curve==0 )
    {
      getCenterLine(xc);
      // ::display(xc,sPrintF(buff,"%s: center line",(const char*)pBeamModel->getName()),"%8.2e ");
      // ::display(xc,"center line","%8.2e ");
    }
    else 
    {
      const int numberOfDimensions=2; // **FIX ME**
      xc.redim(numNodes,numberOfDimensions);
      for( int i = 0; i < numNodes; i++ ) 
      {
	for( int axis=0; axis<numberOfDimensions; axis++ )
	{
	  if( curve==1 )
	    xc(i,axis)=slaveStates[i].xplus[axis];
	  else
	    xc(i,axis)=slaveStates[i].xminus[axis];
	}
      }
    }
    
    NurbsMapping map; 
    map.interpolate(xc);

    PlotIt::plot(gi, map,psp);      
  }
  

  psp.set(GraphicsParameters::lineWidth,1);  // reset


  return 0;
}


// =================================================================================================
/// \brief  Define the NonlinearBeamModel parameters interactively.
// =================================================================================================
int NonlinearBeamModel::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  // *********** FINISH ME ****************

  GUIState gui;
  gui.setWindowTitle("Nonlinear Beam Model");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  // OV_real & subIterationConvergenceTolerance = dbase.get<OV_real>("subIterationConvergenceTolerance");
  // OV_real & addedMassRelaxationFactor = dbase.get<OV_real>("addedMassRelaxationFactor");

  aString beamFileName = "mybeam.beam";

  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString pbLabels[] = {"build beam",
    			  ""};
    GUIState::addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=4;
    dialog.setPushButtons( cmd, pbLabels, numRows ); 

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


    aString tbCommands[] = {"use exact solution",
                            "steady state",
                            "save profile file",
                            "save tip file",
    			    ""};
    int tbState[10];
    tbState[0] = useExactSolution; 
    tbState[1] = isSteady; 
    tbState[2] = dbase.get<bool>("saveProfileFile");
    tbState[3] = dbase.get<bool>("saveTipFile");
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "name:"; sPrintF(textStrings[nt], "%s",(const char*)name);  nt++; 
    textLabels[nt] = "beam file:"; sPrintF(textStrings[nt], "%s",(const char*)beamFileName);  nt++; 
    textLabels[nt] = "number of elements:"; sPrintF(textStrings[nt], "%i",numElem);  nt++; 
    // textLabels[nt] = "area moment of inertia:"; sPrintF(textStrings[nt], "%g",areaMomentOfInertia);  nt++; 
    textLabels[nt] = "elastic modulus:"; sPrintF(textStrings[nt], "%g",Em);  nt++; 
    textLabels[nt] = "density:"; sPrintF(textStrings[nt], "%g",density);  nt++; 
    textLabels[nt] = "nu:"; sPrintF(textStrings[nt], "%g",nu);  nt++; 
    textLabels[nt] = "thickness:"; sPrintF(textStrings[nt], "%g",beamThickness);  nt++; 
    textLabels[nt] = "length:"; sPrintF(textStrings[nt], "%g",beamLength);  nt++; 
    textLabels[nt] = "structure omega:"; sPrintF(textStrings[nt], "%g",omegaStructure);  nt++; 
    // textLabels[nt] = "initial declination:"; sPrintF(textStrings[nt], "%g (degrees)",beamInitialAngle*180./Pi);  nt++; 
    // textLabels[nt] = "position:"; sPrintF(textStrings[nt], "%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0);  nt++; 

    // textLabels[nt] = "added mass relaxation:"; sPrintF(textStrings[nt], "%g",addedMassRelaxationFactor);  nt++; 
    // textLabels[nt] = "added mass tol:"; sPrintF(textStrings[nt], "%g",subIterationConvergenceTolerance);  nt++; 

    textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",debug);  nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    // dialog.setTextBoxes(cmd, textLabels, textStrings);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  }
  
  aString answer,buff;

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("nlbeam>");
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
    else if( dialog.getTextValue(answer,"beam file:","%s",beamFileName) )
    {
      readBeamFile((const char*)beamFileName);
    }
    else if( dialog.getTextValue(answer,"number of elements:","%i",numElem) ){} //

    // else if( dialog.getTextValue(answer,"area moment of inertia:","%g",areaMomentOfInertia) ){} //

    else if( dialog.getTextValue(answer,"elastic modulus:","%g",Em) ){} //
    else if( dialog.getTextValue(answer,"density:","%g",density) ){} //
    else if( dialog.getTextValue(answer,"nu:","%g",nu) ){} //
    else if( dialog.getTextValue(answer,"thickness:","%g",beamThickness) ){} //
    else if( dialog.getTextValue(answer,"length:","%g",beamLength) ){} //
    else if( dialog.getTextValue(answer,"structure omega:","%g",omegaStructure) ){} //
    // else if( dialog.getTextValue(answer,"pressure norm:","%g",pressureNorm) ){} //
    // else if( dialog.getTextValue(answer,"added mass relaxation:","%g",addedMassRelaxationFactor) )
    // {
    //   printF("The relaxation parameter used in the fixed point iteration\n"
    //          " used to alleviate the added mass effect\n");
    // }

    // else if( dialog.getTextValue(answer,"added mass tol:","%g",subIterationConvergenceTolerance) )
    // {
    //   printF("The (relative) convergence tolerance for the fixed point iteration\n"
    // 	     " tol: convergence tolerance (default is 1.0e-3)\n");
    // }

    // else if( dialog.getTextValue(answer,"initial declination:","%g",beamInitialAngle) )
    // {  
    //   setDeclination(beamInitialAngle*Pi/180.);
    //   printF("INFO: The beam will be inclined %8.4f degrees from the left end\n",beamInitialAngle*180./Pi);
    //   dialog.setTextLabel("initial declination:",sPrintF(buff,"%g, (degrees)",beamInitialAngle*180./Pi));
    // } 
    // else if( (len=answer.matches("position:")) )
    // {
    //   sScanF(answer(len,answer.length()-1),"%e %en %e",&beamX0,&beamY0,&beamZ0);
    //   printF("INFO: Setting the position of the left end of the beam to (%e,%e,%e)\n",beamX0,beamY0,beamZ0);
    //   dialog.setTextLabel("position:",sPrintF(buff,"%g, %g, %g (x0,y0,z0)",beamX0,beamY0,beamZ0));
    // }
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
	printF("NonlinearBeamModel:ERROR: unknown BC : answer=[%s], bcOption=[%s]\n",(const char*)answer,(const char*)bcOption);
	gi.stopReadingCommandFile();
      }

      printF("NonlinearBeamModel:INFO: setting %s = %s.\n",(side==0 ? "bcLeft" : "bcRight"),(const char*)bcOption);
      if( side==0 )
         dialog.getOptionMenu("BC left:").setCurrentChoice(answer);
      else      
         dialog.getOptionMenu("BC right:").setCurrentChoice(answer);
    }
    else if( dialog.getToggleValue(answer,"use exact solution",useExactSolution) ){} // 
    else if( dialog.getToggleValue(answer,"steady state",isSteady) ){} // 
    else if( dialog.getToggleValue(answer,"save profile file",dbase.get<bool>("saveProfileFile")) ){} // 
    else if( dialog.getToggleValue(answer,"save tip file",dbase.get<bool>("saveTipFile")) )
    {
      aString tipFileName = sPrintF(buff,"%s_tip.text",(const char*)name);
// *FIX ME*      output.open(name);
      printF("NonlinearBeamModel: tip position info will be saved to file 'tip.txt'\n");
    }
    else if( answer=="build beam" )
    { 
      // --- construct a (horizontal) beam ---

      numNodes=numElem+1;

      delete [] beamNodes;
      beamNodes = new BeamNode[numNodes];

      OV_real angle =90.*Pi/180;
      for (int i = 0; i < numNodes; ++i) 
      {
        OV_real xl = i*beamLength/(numNodes-1.);
	
	beamNodes[i].X[0]=xl; beamNodes[i].X[1]=0.; beamNodes[i].X[2]=0.;
        beamNodes[i].thickness=beamThickness;

	beamNodes[i].undeformedRotation=angle; 
      }
      initialize();

      GraphicsParameters psp;
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true); 
      plot(gi,psp);
      
    }
    
    else
    {
      printF("NonlinearBeamModel::update:ERROR:unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

  }
    
  // -- initialize the beam model given the current parameters --
  initialize();

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;

}
