#include "MovingGrids.h"
#include "GridFunction.h"
#include "MatrixTransform.h"
#include "GenericGraphicsInterface.h"
#include "RigidBodyMotion.h"
#include "DeformingBodyMotion.h"
#include "Integrate.h"
#include "Ogshow.h"
#include "DetectCollisions.h"
#include "AnnulusMapping.h"
#include "BodyForce.h"

#include "MappingInformation.h"
#include "GenericGraphicsInterface.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "display.h"

#include "MatrixMotion.h"
#include "HDF_DataBase.h"


#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// -- limit the number of times certain warnings are printed:
static const int maxExceedsWarnings=20;
static int numberOfExceedsForceWarnings=0;
static int numberOfExceedsTorqueWarnings=0;

int MovingGrids::debug0=0;

// =======================================================================================
/// \brief This class is used to move grids.
///
/// \note: One should not expect the Parameters input object to have valid information
///  at the time of this call.
//=========================================================================================
MovingGrids::
MovingGrids(Parameters & parameters_)
  : parameters(parameters_)
{
  isInitialized=false;
  
  movingGridProblem=false;

  // matrix motion bodies:
  numberOfMatrixMotionBodies=0;
  matrixMotionBody=NULL;
  
  //..RigidBody init
  numberOfRigidBodies=0;
  body=NULL;
  integrate=NULL;
  rigidBodyInfoCount=0;
  numberOfRigidBodyInfoNames=0;
  rigidBodyInfoName = NULL;
  useHybridGridsForSurfaceIntegrals=false;
  
  numberOfDeformingBodies=0;
  deformingBodyList=NULL;

  limitForces=false;
  maximumAllowableForce=REAL_MAX;
  maximumAllowableTorque=REAL_MAX;

  // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;

  // recompute the grid velocity after correcting a moving grid
  recomputeGridVelocityOnCorrection=true;  // *new* July 5, 2016 -- for old way, set to false
}

//\begin{>MovingGridsSolverInclude.tex}{\subsection{constructor}} 
MovingGrids::
MovingGrids(const MovingGrids & mg)
  : parameters(mg.parameters)
// =======================================================================================
// /Description:
//     This class is used to move grids.
//
// /NOTE: One should not expect the Parameters input object to have valid information
//  at the time of this call.
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  isInitialized=false;
  
  movingGridProblem=false;

  // matrix motion bodies:
  numberOfMatrixMotionBodies=0;
  matrixMotionBody=NULL;
  
  //..RigidBody init
  numberOfRigidBodies=0;
  body=NULL;
  integrate=NULL;
  rigidBodyInfoCount=0;
  numberOfRigidBodyInfoNames=0;
  rigidBodyInfoName = NULL;
  useHybridGridsForSurfaceIntegrals=false;
  
  numberOfDeformingBodies=0;
  deformingBodyList=NULL;

  limitForces=false;
  maximumAllowableForce=REAL_MAX;
  maximumAllowableTorque=REAL_MAX;

  // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;

  // recompute the grid velocity after correcting a moving grid
  recomputeGridVelocityOnCorrection=true;  // *new* July 5, 2016 -- for old way, set to false
}

int MovingGrids::
initialize()
// =======================================================================================
// /Description:
//     Initialize parameters. (this is a protected function)
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  isInitialized=true;

  const int numberOfDimensions = parameters.dbase.get<int >("numberOfDimensions");
  
  rigidBodyInfoCount=0;
  numberOfRigidBodyInfoNames=numberOfDimensions==3 ? 24 : 14;

  delete [] rigidBodyInfoName;
  rigidBodyInfoName = new aString [numberOfRigidBodyInfoNames];

  int i=0;
  rigidBodyInfoName[i]="x1"; i++;
  rigidBodyInfoName[i]="x2"; i++;
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="x3"; i++; }
  rigidBodyInfoName[i]="v1"; i++;
  rigidBodyInfoName[i]="v2"; i++;
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="v3"; i++; }
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="w1"; i++; }
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="w2"; i++; }
  rigidBodyInfoName[i]="w3"; i++;
  rigidBodyInfoName[i]="f1"; i++;
  rigidBodyInfoName[i]="f2"; i++;
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="f3"; i++; }
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="g1"; i++; }
  if( numberOfDimensions==3 ){ rigidBodyInfoName[i]="g2"; i++; }
  rigidBodyInfoName[i]="g3"; i++;

  // accelerations: (new Dec 2, 2015) *wdh*
  for( int axis=0; axis<numberOfDimensions; axis++) { rigidBodyInfoName[i]=sPrintF("a%i",axis+1); i++; } // 
  if( numberOfDimensions==2 )
  {
    rigidBodyInfoName[i]="wt3";  i++;  // only one angular acceleration in 2D
  }
  else
  {
    for( int axis=0; axis<numberOfDimensions; axis++) { rigidBodyInfoName[i]=sPrintF("wt%i",axis+1); i++; } // 
  }
  
  rigidBodyInfoName[i]="fMax"; i++;
  rigidBodyInfoName[i]="gMax"; i++;
  rigidBodyInfoName[i]="pMax"; i++;

  assert( i==numberOfRigidBodyInfoNames );

  //..DeformingBody init
  numberOfDeformingBodies=0;
  deformingBodyList=NULL;
  
  return 0;
}

MovingGrids::
~MovingGrids()
{
  //..destroy MatrixMotion bodies
  if( matrixMotionBody!=NULL )
  {
    for( int b=0; b<numberOfMatrixMotionBodies; b++ )
    {
      if( matrixMotionBody[b]->decrementReferenceCount()==0 )
	delete matrixMotionBody[b];
    }
    delete [] matrixMotionBody;
  }

  //..RigidBody destructor
  if( body!=NULL )
  {
    for( int b=0; b<numberOfRigidBodies; b++ )
      delete body[b];
  
    delete [] body;
  }
  
  delete integrate;
  delete [] rigidBodyInfoName;

  //..DeformingBody destructor
  if( deformingBodyList!=NULL )
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      parameters.userDefinedDeformingSurfaceCleanup( *deformingBodyList[b] );
    }
  }
  
  delete [] deformingBodyList;

}

//=================================================================================
/// \brief  Write information to the `check file' (used for regression tests)
//=================================================================================
int MovingGrids::
writeCheckFile( real t, FILE *file )
{
  for( int b=0; b<numberOfDeformingBodies; b++ )
    deformingBodyList[b]->writeCheckFile(t, file);

  for( int b=0; b<numberOfRigidBodies; b++ )
    body[b]->writeCheckFile(t);  // currently each rigid body has it's own check file

  return 0;
}

// =================================================================================
/// \brief Write information about the moving grids.
// =================================================================================
void MovingGrids::
writeParameterSummary( FILE *file /* =stdout */ )
{
  for( int b=0; b<numberOfMatrixMotionBodies; b++ )
    matrixMotionBody[b]->writeParameterSummary(file);

  for( int b=0; b<numberOfRigidBodies; b++ )
    body[b]->writeParameterSummary(file);

  for( int b=0; b<numberOfDeformingBodies; b++ )
    deformingBodyList[b]->writeParameterSummary(file);

}

// =================================================================================
/// \brief Output probe info.
// =================================================================================
int MovingGrids::
outputProbes( GridFunction & gf0, int stepNumber )
{
  // const real t = gf0.t;

  // -- finish me:
  // for( int b=0; b<numberOfMatrixMotionBodies; b++ )
  //   matrixMotionBody[b]->outputProbes( gf0,stepNUmber );

  // -- finish me: This is where we should output rigid body info: 
  // for( int b=0; b<numberOfRigidBodies; b++ )
  //   body[b]->outputProbes( gf0,stepNUmber );

  for( int b=0; b<numberOfDeformingBodies; b++ )
    deformingBodyList[b]->outputProbes( gf0,stepNumber );
}




// =================================================================================
/// \brief Assign initial conditions and past time state.
/// \details This function is called by DomainSolver::initializeSolution()
///   The initial and past state for some deforming grids may depend on the
//    initial conditions and/or known solution and so we delay evaluation until now.
//
// New: 2014/07/11
// =================================================================================
int MovingGrids::
assignInitialConditions( GridFunction & cgf )
{
  // INITIALIZE THE GRIDS at past time levels
  if( numberOfDeformingBodies>0 )
  {
    if( TRUE || debug() & 4 )
      printF("--MvG--assignInitialConditions: INITIALIZING deformingGrid, past time levels.\n"); 

    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      real dt0=1.e-4;  // this should not be used 
      deformingBodyList[b]->initializePast( cgf.t, dt0, cgf.cg);
    }
  }
  
  return 0;
}


// =================================================================================
/// \brief Print time step info
// =================================================================================
void MovingGrids::
printTimeStepInfo( FILE *file /* =stdout */ )
{
  for( int b=0; b<numberOfDeformingBodies; b++ )
  {
    deformingBodyList[b]->printTimeStepInfo(file);
  }
}



// =================================================================================
/// \brief Return a pointer to the Integrate object.
// =================================================================================
Integrate* MovingGrids::
getIntegrate() const
{
  return integrate;
}


// =================================================================================
/// \brief Return the maximum relative change in the moving grid correction scheme.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
real MovingGrids::
getMaximumRelativeCorrection() const
{
  return maximumRelativeCorrection; 
}

// =================================================================================
/// \brief Return true if the correction steps for moving grids have converged.
///    This is usually only an issue for "light" bodies. 
// =================================================================================
bool MovingGrids::
getCorrectionHasConverged()
{
  return correctionHasConverged;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{isMovingGridProblem}} 
bool MovingGrids::
isMovingGridProblem() const
// =======================================================================================
// /Description:
//   Indicate whether any grids are moving.
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  return movingGridProblem;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{setIsMovingGridProblem}} 
int MovingGrids::
setIsMovingGridProblem( bool trueOrFalse /* = TRUE */ )
// =======================================================================================
// /Description:
//    Turn on or off moving grids.
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  movingGridProblem=trueOrFalse;
  return 0;
}


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{gridIsMoving}} 
bool MovingGrids::
gridIsMoving(int grid) const
// =======================================================================================
// /Description:
//     Indicate whether a grid is moving.
// /grid (input) : 
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  if( !movingGridProblem )
    return FALSE;
  else if( grid>=movingGrid.getBase(0) && grid<=movingGrid.getBound(0) )
    return (bool)movingGrid(grid);
  else
    return FALSE;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{movingGridOption}} 
MovingGrids::MovingGridOption MovingGrids::
movingGridOption(int grid) const
// =======================================================================================
// /Description:
//    Return the moving grid option for a given grid.
// /grid (input) : return option for this grid.
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  if( grid>=moveOption.getBase(0) && grid<=moveOption.getBound(0) )
    return (MovingGridOption)moveOption(grid);
  else
    return notMoving;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{movingGridOptionName}} 
aString MovingGrids::
movingGridOptionName(MovingGridOption option) const
// =======================================================================================
// /Description:
//     Return a aString containing the name of the moving grid option.
// /option (input):
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  switch( option )
  {
  case notMoving:
    return "notMoving";
  case rotate:
    return "rotate";
  case shift:
    return "shift";
  case oscillate:
    return "oscillate";
  case scale:
    return "scale";
  case matrixMotion:
    return "matrixMotion";
  case rigidBody:
    return "rigidBody";
  case deformingBody:
    return "deformingBody";
  case userDefinedMovingGrid:
    return "userDefinedMovingGrid";
  default:
    return "unknown";
  }
}

// =======================================================================================
/// /brief for plotting purposes return moving bodies as BodyForce objects (these are saved to the show file)
/// /param movingBodies (output) : Moving bodies represented as BodyForce objects.
//=========================================================================================
int MovingGrids::
getBodies( std::vector<BodyForce*> & movingBodies )
{
  // For now just save deforming bodies.
  if ( numberOfDeformingBodies != 0) 
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
       BodyForce *pbf = new BodyForce;
       int assigned = deformingBodyList[b]->getBody( *pbf );
       if( assigned )
         movingBodies.push_back(pbf);
       else
         delete pbf;
    }
  }

}



//\begin{>>MovingGridsSolverInclude.tex}{\subsection{parameters}} 
const RealArray & MovingGrids::
getMoveParameters() const
// =======================================================================================
// /Description:
//     Return the array of parameters that define movement.
//
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  return moveParameters;
}

// =======================================================================================
/// \brief  Return the number of matrix motion bodies. 
//=========================================================================================
int MovingGrids::
getNumberOfMatrixMotionBodies() const
{
  return numberOfMatrixMotionBodies;
}

// =======================================================================================
/// /brief Return the object that describes a matrix motion body.
/// /param bodyNumber (input) : a body number starting from 0 and ending at getNumberOfMatrixMotionBodies()-1.
//=========================================================================================
MatrixMotion & MovingGrids::
getMatrixMotionBody(const int bodyNumber)
{
  if( bodyNumber>=0 && bodyNumber<numberOfMatrixMotionBodies )
  {
    return *matrixMotionBody[bodyNumber];
  }
  else
  {
    printF("MovingGrids::getMatrixMotionBody:ERROR: attempting to access bodyNumber=%i "
           "but numberOfMatrixMotionBodies=%i\n",bodyNumber,numberOfMatrixMotionBodies);
    OV_ABORT("Error");
  }
}

// =======================================================================================
/// \brief  Return the number of rigid bodies. 
//=========================================================================================
int MovingGrids::
getNumberOfRigidBodies() const
{
  return numberOfRigidBodies;
}

// =======================================================================================
/// \brief Return the object that describes a rigid body.
/// \param bodyNumber (input) : a body number starting from 0 and ending at getNumberOfRigidBodies()-1.
//=========================================================================================
RigidBodyMotion & MovingGrids::
getRigidBody(const int bodyNumber)
{
  if( bodyNumber>=0 && bodyNumber<numberOfRigidBodies )
  {
    return *body[bodyNumber];
  }
  else
  {
    printF("MovingGrids::getRigidBody:ERROR: attempting to access bodyNumber=%i but numberOfRigidBodies=%i\n",
	   bodyNumber,numberOfRigidBodies);
    OV_ABORT("Error");
  }
}

// =======================================================================================
/// \brief Return the added damping tensors for a rigid body.
/// \param bodyNumber (input) : number of the rigid body
/// \param addedDampingTensors(3,3,2,2) (output) : 4 3x3 tensors
///    addedDampingTensors(0:2,0:2,0,0) : Dvv : coeff of linear velocity in linear velocity eqn.
///    addedDampingTensors(0:2,0:2,0,1) : Dvw : coeff of angular velocity in linear velocity eqn.
///    addedDampingTensors(0:2,0:2,1,0) : Dwv : coeff of linear velocity in angular velocity eqn.
///    addedDampingTensors(0:2,0:2,1,1) : Dww : coeff of angular velocity in angular velocity eqn.
///
/// \param cgf (input) : current grid function (holds current grid and current time)
/// \param dt : current time step
//=========================================================================================
int MovingGrids::getRigidBodyAddedDampingTensors( const int bodyNumber, RealArray & addedDampingTensors,
                                                  GridFunction & cgf, const real dt )
{
  // The viscous added damping tensors do not depend on the solution and thus
  // can be computed once and saved -- save in the RigidBody object.
  // Note: tensors will change if the grid spacing changes -- e.g. for AMR -- *fix me*

  const real t = cgf.t;
  CompositeGrid & cg = cgf.cg;
  const int numberOfDimensions = cg.numberOfDimensions();
  
  RigidBodyMotion & body = getRigidBody(bodyNumber);

  const real nu = parameters.dbase.get<real >("nu");    
  const real & fluidDensity = parameters.dbase.get<real >("fluidDensity");
  assert( fluidDensity>0. );
  const real mu = nu*fluidDensity;

  const real & addedDampingCoefficient = parameters.dbase.get<real>("addedDampingCoefficient");

  // if scaleAddedDampingWithDt==1 then adjust the added-damping tensors by an addtional factor of
  //     1 - exp(-delta) 
  //  where 
  //    delta = dy/sqrt(nu*alpha*dt)  (alpha=1/2 for Trapezoidal rule)
  //    
  const bool & scaleAddedDampingWithDt = parameters.dbase.get<bool>("scaleAddedDampingWithDt");

  // Added damping tensors may have already been computed and saved in the RigidBody: 
  //   In the case scaleAddedDampingWithDt==1 : the added-damping tensors are stored without 
  //     the factor of mu/dn , dn=sqrt(nu*dt)

  // -- retrieve the added-damping tensors if they have been already computed: 
  const int returnValue = body.getAddedDampingTensors( addedDampingTensors,t );
  bool addedDampingTensorsHaveBeenComputed = returnValue==0;

  if( addedDampingTensorsHaveBeenComputed )
  {
    // --- added-damping tensors have been pre-computed ---
    if( debug() & 4  )
      body.displayAddedDampingTensors("--MVG--getRigidBodyAddedDampingTensors:",cgf.t);

    if( scaleAddedDampingWithDt && dt>0.  )
    {
      // --- Adjust the added-damping tensors based on the current time step ---

      // addedDampingScaleFactor=addedDampingCoefficient*(mu/dy);
      const real addedDampingScaleFactor = body.getAddedDampingScaleFactor(); 
      printF("--MVG--getRigidBodyAddedDampingTensors:addedDampingScaleFactor=ADC*mu/dy=%8.2e\n",
              addedDampingScaleFactor);

      const real dy = mu*addedDampingCoefficient/addedDampingScaleFactor; // minGridSpacing 

      const real alpha = .5;                // weight for Trapezoidal rule 
      const real dnu = sqrt( nu*alpha*dt);  // viscous length-scale
      const real delta = dy/dnu;

      const real scaleFactor = delta<.1 ? delta*(1-.5*delta) : 1 - exp(-delta); 
      if( true || debug() & 4  )
        printF("--MVG--getRigidBodyAddedDampingTensors: SCALING stored added damping tensors "
               "by 1-exp(-delta) = %8.2e, (dy=%8.2e, dnu=%8.2e, delta=dy/dnu=%8.2e)\n",scaleFactor,dy,dnu,delta);
      addedDampingTensors *= scaleFactor; 
    }
      

    return 0;
  }
  
  // -------------------------------------------
  // ---- Compute the added damping tensors ----
  // -------------------------------------------
  addedDampingTensors=0.;

  real dn =0.;
  if( scaleAddedDampingWithDt && dt<=0. )
  {
    printF("--MVG--getRigidBodyAddedDampingTensors:WARNING dt<=0 !! using dn from grid instead.\n");
  }
  
  // --- Compute the minimum grid spacing in the normal direction ---

  real minGridSpacing=REAL_MAX;  // holds min-grid-spacing
  assert( integrate!=NULL );
  const int numberOfFaces=integrate->numberOfFacesOnASurface(bodyNumber);
  for( int face=0; face<numberOfFaces; face++ )
  {
    int side=-1,axis,grid;
    integrate->getFace(bodyNumber,face,side,axis,grid);
    assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
    assert( grid>=0 && grid<cg.numberOfComponentGrids());

    MappedGrid & c = cg[grid];
    OV_GET_SERIAL_ARRAY(real,c.vertex(),xLocal);
    Index Ib1,Ib2,Ib3, Ip1,Ip2,Ip3;
    getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); // boundary line 
    getGhostIndex(c.gridIndexRange(),side,axis,Ip1,Ip2,Ip3,-1); // first line in 

    int includeGhost=0;
    bool ok;
    ok=ParallelUtility::getLocalArrayBounds(c.vertex(),xLocal,Ib1,Ib2,Ib3,includeGhost); // restrict bounds to this processor
    ok=ParallelUtility::getLocalArrayBounds(c.vertex(),xLocal,Ip1,Ip2,Ip3,includeGhost);

    // Assume grid is nearly orthogonal -- we could check using the mask 
    if( ok )
    {
      real dist=minGridSpacing;
      if( numberOfDimensions==2 )
	dist = sqrt( min(SQR(xLocal(Ip1,Ip2,Ip3,0)-xLocal(Ib1,Ib2,Ib3,0))+
			 SQR(xLocal(Ip1,Ip2,Ip3,1)-xLocal(Ib1,Ib2,Ib3,1))) );
      else
	dist = sqrt( min(SQR(xLocal(Ip1,Ip2,Ip3,0)-xLocal(Ib1,Ib2,Ib3,0))+
			 SQR(xLocal(Ip1,Ip2,Ip3,1)-xLocal(Ib1,Ib2,Ib3,1))+
			 SQR(xLocal(Ip1,Ip2,Ip3,2)-xLocal(Ib1,Ib2,Ib3,2))) );
      minGridSpacing=min(minGridSpacing,dist);
    }
    
  }
  minGridSpacing=ParallelUtility::getMinValue(minGridSpacing);
  dn=minGridSpacing;
  printF("--MVG--getRigidBodyAddedDampingTensors: body=%i min-grid-spacing at boundary=%8.2e -- setting dn=%8.2e\n",
	 bodyNumber,minGridSpacing,dn);
  
  // Given the grid spacing on the boundary we evaluate the integrals defining the added damping tensors 

  // // -- Added damping for a disk --   KEEP FOR NOW FOR CHECKING 
  real radius=1.;  
  const real DwwDisk = addedDampingCoefficient*(mu/dn)*twoPi*pow(radius,3.);  // *check me*
  // addedDampingTensors(2,2,1,1)=DwwDisk;

  // printF("--MVG--getRigidBodyAddedDampingTensors : body=%i, dn=%8.2e radius=%8.2e mu=%8.2e "
  //        "addedDampingCoefficient=%4.2f Dww=%8.2e\n",
  // 	 bodyNumber,dn,radius,mu,addedDampingCoefficient, DwwDisk);

  if( true )
  {
    // ====================================================================
    // =============== Evaluate Added Damping Tensors =====================
    // ===============  From Surface Integrals        =====================
    // ====================================================================

    // Save boundary data for added damping tensors here:
    ListOfRealArray addedDampingMatrixList;  
    // numberOfAddedDampingMatrixEntries : total number of entries we need to compute through surface integrals
    const int numberOfAddedDampingMatrixEntries=numberOfDimensions==2 ? 6 : 21;

    RealArray xCM(3);
    body.getPosition( t,xCM );  // ** need time corresponding to the grid so integrals are correct

    Index Ib1,Ib2,Ib3;

    const int vbc=0, wbc=1; // component numbers of v and omega in addedDampingTensors
    int iLocal, jLocal, kLocal;


    assert( integrate!=NULL );
    const int numberOfFaces=integrate->numberOfFacesOnASurface(bodyNumber);
    for( int face=0; face<numberOfFaces; face++ )
    {
      int side=-1,axis,grid;
      integrate->getFace(bodyNumber,face,side,axis,grid);
      assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
      assert( grid>=0 && grid<cg.numberOfComponentGrids());

      MappedGrid & c = cg[grid];
      OV_GET_SERIAL_ARRAY(real,c.vertex(),xLocal);
      #ifdef USE_PPP
        realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
      #else
        realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
      #endif

      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); // boundary line 
      int includeGhost=1;
      bool ok=ParallelUtility::getLocalArrayBounds(c.vertex(),xLocal,Ib1,Ib2,Ib3,includeGhost); // restrict bounds to this processor

      // create space to save the added damping entries on the boundary
      if( face >= addedDampingMatrixList.getLength() )
	addedDampingMatrixList.addElement();
      RealArray & adm = addedDampingMatrixList[face];
      if( ok )
      {
	adm.redim(Ib1,Ib2,Ib3,numberOfAddedDampingMatrixEntries);

	if( numberOfDimensions==2 )
	{
	  // normal = [n0 n1 0] 
	  // tangent = [-n1 n0 0] 
	  RealArray temp(Ib1,Ib2,Ib3);

	  // Compute (x-xb) X tv  = yv X tv = Rt  (only 1 nonzero entry in 3D)
	  //  tv = [ -n1, n0, 0]    :tangent vector in 2D for orthogonal!  *FIX ME*
	  //
	  //   [ i   j    k ]
	  //   [ y0  y1   0 ]
	  //   [-n1  n0   0 ]
	  RealArray yCrossT(Ib1,Ib2,Ib3);
	  yCrossT(Ib1,Ib2,Ib3) = ((xLocal(Ib1,Ib2,Ib3,0)-xCM(0))*normalLocal(Ib1,Ib2,Ib3,0) + 
				  (xLocal(Ib1,Ib2,Ib3,1)-xCM(1))*normalLocal(Ib1,Ib2,Ib3,1) );

	  // -- save added-damping matrix entries ( without mu/dn)  ---
	  // if dn varied in space we could include the effect here: (?)

	  //  Dvv =  Int (mu/dn)  tv tv^T ds 
	  //  Dvw =  Int (mu/dn) tv (Rt)^t ds
	  //  Dwv = Dvw^T 
	  //  Dww =  Int (mu/dn) Rt Rt^T ds 

	  adm(Ib1,Ib2,Ib3,0) =  normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,1);  // Dvv(0,0)
	  adm(Ib1,Ib2,Ib3,1) = -normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,0);  // Dvv(0,1)=Dvv(1,0)
	  adm(Ib1,Ib2,Ib3,2) =  normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0);  // Dvv(1,1)

	  adm(Ib1,Ib2,Ib3,3) = -normalLocal(Ib1,Ib2,Ib3,1)*yCrossT(Ib1,Ib2,Ib3);        // Dvw(0,2)=Dwv(2,0)
	  adm(Ib1,Ib2,Ib3,4) =  normalLocal(Ib1,Ib2,Ib3,0)*yCrossT(Ib1,Ib2,Ib3);        // Dvw(1,2)=Dwv(2,1)

	  adm(Ib1,Ib2,Ib3,5) = yCrossT(Ib1,Ib2,Ib3)*yCrossT(Ib1,Ib2,Ib3);               // Dww(2,2)

	}
	else
	{
	  // normal = [n0 n1 n2] 
	  // Compute [x-xb]_x  (6 nonzero entry in 3D)
	  //   [ 0        -(x-xb)_2     (x-xb)_1 ]
	  //   [ (x-xb)_2  0           -(x-xb)_0 ]
	  //   [-(x-xb)_1  (x-xb)_0            0 ]
	  RealArray xFace(Ib1,Ib2,Ib3,3);
	  xFace(Ib1,Ib2,Ib3,0) = xLocal(Ib1,Ib2,Ib3,0)-xCM(0);
	  xFace(Ib1,Ib2,Ib3,1) = xLocal(Ib1,Ib2,Ib3,1)-xCM(1);
	  xFace(Ib1,Ib2,Ib3,2) = xLocal(Ib1,Ib2,Ib3,2)-xCM(2);

	  // -- save added-damping matrix entries ( without mu/dn)  ---
	  //  Dvv =  Int (mu/dn) (I-nv nv^t) ds 
	  //  Dvw =  Int (mu/dn) (I-nv nv^t)R^t ds
	  //  Dwv =  Dvw^T 
	  //  Dww =  Int (mu/dn) R(I-nv nv^t)R^t ds 

	  // Dvv
	  adm(Ib1,Ib2,Ib3,0) =1.-normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0);  // Dvv(0,0)
	  adm(Ib1,Ib2,Ib3,1) =  -normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,0);  // Dvv(0,1)=Dvv(1,0)
	  adm(Ib1,Ib2,Ib3,2) =  -normalLocal(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,0);  // Dvv(0,2)=Dvv(2,0)
	  adm(Ib1,Ib2,Ib3,3) =1.-normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,1);  // Dvv(1,1)
	  adm(Ib1,Ib2,Ib3,4) =  -normalLocal(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,1);  // Dvv(1,2)=Dvv(2,1)
	  adm(Ib1,Ib2,Ib3,5) =1.-normalLocal(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,2);  // Dvv(2,2)

	  // Dvw
	  adm(Ib1,Ib2,Ib3,6) =-adm(Ib1,Ib2,Ib3,1)*xFace(Ib1,Ib2,Ib3,2)+adm(Ib1,Ib2,Ib3,2)*xFace(Ib1,Ib2,Ib3,1);  // Dvw(0,0)
	  adm(Ib1,Ib2,Ib3,7) = adm(Ib1,Ib2,Ib3,0)*xFace(Ib1,Ib2,Ib3,2)-adm(Ib1,Ib2,Ib3,2)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(0,1)
	  adm(Ib1,Ib2,Ib3,8) =-adm(Ib1,Ib2,Ib3,0)*xFace(Ib1,Ib2,Ib3,1)+adm(Ib1,Ib2,Ib3,1)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(0,2)
	  adm(Ib1,Ib2,Ib3,9) =-adm(Ib1,Ib2,Ib3,3)*xFace(Ib1,Ib2,Ib3,2)+adm(Ib1,Ib2,Ib3,4)*xFace(Ib1,Ib2,Ib3,1);  // Dvw(1,0)
	  adm(Ib1,Ib2,Ib3,10)= adm(Ib1,Ib2,Ib3,1)*xFace(Ib1,Ib2,Ib3,2)-adm(Ib1,Ib2,Ib3,4)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(1,1)
	  adm(Ib1,Ib2,Ib3,11)=-adm(Ib1,Ib2,Ib3,1)*xFace(Ib1,Ib2,Ib3,1)+adm(Ib1,Ib2,Ib3,3)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(1,2)
	  adm(Ib1,Ib2,Ib3,12)=-adm(Ib1,Ib2,Ib3,4)*xFace(Ib1,Ib2,Ib3,2)+adm(Ib1,Ib2,Ib3,5)*xFace(Ib1,Ib2,Ib3,1);  // Dvw(2,0)
	  adm(Ib1,Ib2,Ib3,13)= adm(Ib1,Ib2,Ib3,2)*xFace(Ib1,Ib2,Ib3,2)-adm(Ib1,Ib2,Ib3,5)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(2,1)
	  adm(Ib1,Ib2,Ib3,14)=-adm(Ib1,Ib2,Ib3,2)*xFace(Ib1,Ib2,Ib3,1)+adm(Ib1,Ib2,Ib3,4)*xFace(Ib1,Ib2,Ib3,0);  // Dvw(2,2)

	  // Dww
	  adm(Ib1,Ib2,Ib3,15)= adm(Ib1,Ib2,Ib3,12)*xFace(Ib1,Ib2,Ib3,1)-adm(Ib1,Ib2,Ib3, 9)*xFace(Ib1,Ib2,Ib3,2);  // Dww(0,0)
	  adm(Ib1,Ib2,Ib3,16)=-adm(Ib1,Ib2,Ib3,10)*xFace(Ib1,Ib2,Ib3,2)+adm(Ib1,Ib2,Ib3,13)*xFace(Ib1,Ib2,Ib3,1);  // Dww(0,1)
	  adm(Ib1,Ib2,Ib3,17)=-adm(Ib1,Ib2,Ib3,11)*xFace(Ib1,Ib2,Ib3,2)+adm(Ib1,Ib2,Ib3,14)*xFace(Ib1,Ib2,Ib3,1);  // Dww(0,2)
	  adm(Ib1,Ib2,Ib3,18)=-adm(Ib1,Ib2,Ib3,13)*xFace(Ib1,Ib2,Ib3,0)+adm(Ib1,Ib2,Ib3, 7)*xFace(Ib1,Ib2,Ib3,2);  // Dww(1,1)
	  adm(Ib1,Ib2,Ib3,19)=-adm(Ib1,Ib2,Ib3,14)*xFace(Ib1,Ib2,Ib3,0)+adm(Ib1,Ib2,Ib3, 8)*xFace(Ib1,Ib2,Ib3,2);  // Dww(1,2)
	  adm(Ib1,Ib2,Ib3,20)= adm(Ib1,Ib2,Ib3,11)*xFace(Ib1,Ib2,Ib3,0)-adm(Ib1,Ib2,Ib3, 8)*xFace(Ib1,Ib2,Ib3,1);  // Dww(2,2)
	}
      } // end if ok 
      
    } // end for face
    
    // Use this function for integrating the entries in the added damping tensors  **FIX ME**
    Range all;
    RealCompositeGridFunction addedDampingCoeff(cg,all,all,all);
    addedDampingCoeff=0.;
    
    // ----------------------------------
    // --- Evaluate surface integrals ---
    // ----------------------------------
    for( int ma=0; ma<numberOfAddedDampingMatrixEntries; ma++ )
    {
      // --- fill in the grid function addedDampingCoeff ----
      for( int face=0; face<numberOfFaces; face++ )
      {
	int side=-1,axis,grid;
	integrate->getFace(bodyNumber,face,side,axis,grid);
	assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	assert( grid>=0 && grid<cg.numberOfComponentGrids());

        OV_GET_SERIAL_ARRAY(real,addedDampingCoeff[grid],addedDampingCoeffLocal);
        RealArray & adm = addedDampingMatrixList[face];

        getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3); // boundary line 
	int includeGhost=1;
	bool ok=ParallelUtility::getLocalArrayBounds(addedDampingCoeff[grid],addedDampingCoeffLocal,Ib1,Ib2,Ib3,includeGhost);
        if( ok )
	  addedDampingCoeffLocal(Ib1,Ib2,Ib3,0)=adm(Ib1,Ib2,Ib3,ma);
      }
      
      // --- integrate the components of the added mass matrix -----
      RealArray adc(1);  // answer goes here for added damping integral
      Range R(0,0); // components to integrate 
      adc=0.;

      Interpolant & interpolant = *(cgf.u.getInterpolant());
      interpolant.interpolate(addedDampingCoeff,R);  // This may be needed.
	
      integrate->surfaceIntegral(addedDampingCoeff,R,adc,bodyNumber);
      
      if( numberOfDimensions==2 )
      {
	if( ma==0 )
	{
	  addedDampingTensors(0,0,vbc,vbc)=adc(0);                                 // Dvv(0,0)
	}
	else if( ma==1 )
	{
	  addedDampingTensors(0,1,vbc,vbc)=addedDampingTensors(1,0,vbc,vbc)=adc(0);  // Dvv(0,1)=Dvv(1,0)
	}
	else if( ma==2 )
	{
	  addedDampingTensors(1,1,vbc,vbc)=adc(0);                                 // Dvv(1,1)
	}
	else if( ma==3 )
	{
	  addedDampingTensors(0,2,vbc,wbc)=addedDampingTensors(2,0,wbc,vbc)=adc(0);  // Dvw(0,2)=Dwv(2,0)
	}
	else if( ma==4 )
	{
	  addedDampingTensors(1,2,vbc,wbc)=addedDampingTensors(2,1,wbc,vbc)=adc(0);  // Dvw(1,2)=Dwv(2,1)
	}
	else if( ma==5 )
	  addedDampingTensors(2,2,wbc,wbc)=adc(0);                                 // Dww(3,3)   
	else
	{
	  OV_ABORT("error");
	}
      }
      else 
      {
	if( ma==0 || ma==15 )
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(0,0,kLocal,kLocal)=adc(0);						// Dvv(0,0) or Dww
	}
	else if( ma==1 || ma==16)
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(0,1,kLocal,kLocal)=addedDampingTensors(1,0,kLocal,kLocal)=adc(0);	// Dvv(0,1)=Dvv(1,0) or Dww
	}
	else if( ma==2 || ma==17)
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(0,2,kLocal,kLocal)=addedDampingTensors(2,0,kLocal,kLocal)=adc(0);   // Dvv(0,2)=Dvv(2,0) or Dww
	}
	else if( ma==3 || ma==18 )
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(1,1,kLocal,kLocal)=adc(0);						// Dvv(1,1) or Dww
	}      
	else if( ma==4 || ma==19)
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(1,2,kLocal,kLocal)=addedDampingTensors(2,1,kLocal,kLocal)=adc(0);   // Dvv(1,2)=Dvv(2,1) or Dww
	} 		  
	else if( ma==5 || ma==20)
	{
	  kLocal=ma<6 ? vbc:wbc;
	  addedDampingTensors(2,2,kLocal,kLocal)=adc(0);						// Dvv(2,2) or Dww
	}     	  
	else if( ma>5 && ma<15)
	{
	  jLocal=(ma-6) % 3;
	  iLocal=(ma-6-jLocal)/3;
	  addedDampingTensors(iLocal,jLocal,vbc,wbc)=addedDampingTensors(jLocal,iLocal,wbc,vbc)=adc(0);  // Dvw(i,j)=Dwv(j,i)
	}
	else
	{
	  OV_ABORT("error");
	}
      }

    } // end for ma, loop over different added mass coefficients
    
    // if( true )  // ****************** TEST ***************************************************** TEMP 
    // {  
    //   addedDampingTensors(0,0,vbc,vbc) *=2;
    //   addedDampingTensors(1,1,vbc,vbc) *=2; 
    // }
    
    if( false )  // ****************** TEST ***************************************************** TEMP 
    {
      printF("--TESTING--- zero all addedDampingTensors except for rotations...\n");
      Range all;
      addedDampingTensors(all,all,vbc,all)=0.;
      addedDampingTensors(all,all,all,vbc)=0.;
      
    }
    
    if( false )
    {
      // *** TESTING *** -- fill in all values 
      for( int i1=0; i1<3; i1++ )
	for( int i2=0; i2<3; i2++ )
	  for( int i3=0; i3<2; i3++ )
	    for( int i4=0; i4<2; i4++ )
	      addedDampingTensors(i1,i2,i3,i4)= 1+ i1+3*(i2+3*(i3+2*i4));
    }
    
    if( true  )
    {
      // -- temporarily set added-dampping tensors so we can print them out 
      real addedDampingScaleFactor=1.;
      body.setAddedDampingTensors( addedDampingTensors,t, addedDampingScaleFactor );
      body.displayAddedDampingTensors("--MVG--getRigidBodyAddedDampingTensors (Computed from surface integrals, no scaling by mu/dn) :",t);
    }
    
    if( true )
    {
      real adc = addedDampingTensors(2,2,wbc,wbc);

      const real DwwFromIntegral = addedDampingCoefficient*(mu/dn)*adc; 
      printF("--MVG--getRigidBodyAddedDampingTensors : body=%i, dn=%8.2e mu=%8.2e "
	     "scaled-Dww=%12.5e scaled-Dww/(2pi)=%12.5e, Dww=%12.5e (disk=%12.5e)\n",
	     bodyNumber,dn,mu, adc,adc/twoPi, DwwFromIntegral, DwwDisk);

      // OV_ABORT("stop here for now");
    }
    

  } // end if true 
  

  // --- Scale the added damping tensors by mu/dy ---
  real addedDampingScaleFactor=addedDampingCoefficient*(mu/dn);
  addedDampingTensors *= addedDampingScaleFactor;  // This may be adjusted below

  // printF("--MVG--getRigidBodyAddedDampingTensors: INIT: addedDampingScaleFactor=%8.2e\n",addedDampingScaleFactor);
  

  // Save the scaled added damping tensors with the RigidBody: 
  body.setAddedDampingTensors( addedDampingTensors,t, addedDampingScaleFactor );

  // Adjust the addedDampingTensors being returned if the scaling depends on dt 
  if( scaleAddedDampingWithDt && dt>0.  )
  {
    const real dy = dn;
    const real alpha = .5;  // Weight for Trapezoidal rule 
    const real dnu = sqrt( nu*alpha*dt);
    const real delta = dy/dnu;

    const real scaleFactor = delta<.1 ? delta*(1-.5*delta) : 1 - exp(-delta); 
    if( true || debug() & 4  )
      printF("--MVG--getRigidBodyAddedDampingTensors: SCALING INITIAL added damping tensors "
	     "by 1-exp(-delta) = %8.2e, (dy=%8.2e, dnu=%8.2e, delta=dy/dnu%8.2e)\n",scaleFactor,dy,dnu,delta);
    addedDampingTensors *= scaleFactor; 
  }

  return 0;
}



// =======================================================================================
/// \brief  Return the number of deforming bodies.
//=========================================================================================
int MovingGrids::
getNumberOfDeformingBodies() const
{
  return numberOfDeformingBodies;
}


// =======================================================================================
/// \brief Return the object that describes a deforming body.
/// \param bodyNumber (input) : a body number starting from 0 and ending at getNumberOfDeformingBodies()-1.
//=========================================================================================
DeformingBodyMotion & MovingGrids::
getDeformingBody(const int bodyNumber)
{
  if( bodyNumber>=0 && bodyNumber<numberOfDeformingBodies )
  {
    return *deformingBodyList[bodyNumber];
  }
  else
  {
    printF("MovingGrids::getDeformingBody:ERROR: attempting to access bodyNumber=%i but numberOfDeformingBodies=%i\n",
	   bodyNumber,numberOfDeformingBodies);
    OV_ABORT("Error");
  }
}


real MovingGrids::
getTimeStepForMovingBodies() const
// =======================================================================================
/// \brief  Return an estimate of the maximum time step allowed for integration of the equations
///    of all moving bodies. Return -1. if no estimate is available. 
///
//=========================================================================================
{
  real dt=REAL_MAX; 

  // dt for rigid bodies: 
  real dtrb = getTimeStepForRigidBodies();

  // dt for deforming: 
  for( int b=0; b<numberOfDeformingBodies; b++ )
  {
    real dtDeform = deformingBodyList[b]->getTimeStep(); // this will return -1. if no estimate is available
    if( dtDeform>0. ) dt=min(dt,dtDeform);
  }
  if( dt==REAL_MAX )
    dt=-1.;

  return dt;
}


real MovingGrids::
getTimeStepForRigidBodies() const
// =======================================================================================
/// \brief  Return an estimate of the maximum time step allowed for integration of the equations
///    of rigid bodies. Return -1. if no estimate is available. 
///
//=========================================================================================
{
  real dt=REAL_MAX; 
  for( int b=0; b<numberOfRigidBodies; b++ )
  {
    real dtrb = body[b]->getTimeStepEstimate();  // this will return -1. if no estimate is available
    if( dtrb>0. ) dt=min(dtrb,dt);
  }
  if( dt==REAL_MAX )
    dt=-1.;
  
  return dt;
}




//! Return ramp value and derivatives
/*!
  \verbatim
  cubic ramp [0,1] : ramp(t)=t*t*(3-2*t) = 3t^2 - 2t^3
  r'=6t*(1.-t)
  ramp(1)=1
  \endverbatim
*/
int MovingGrids::
getRamp(real t, real rampInterval, real & ramp, real & rampSpeed, real & rampAcceleration )
{
  bool rampMovement=true;
// *wdh*   real rampInterval=.5; 

  if( rampMovement && t<rampInterval && rampInterval>0. )
  {
    real ts=t/rampInterval;
    ramp=ts*ts*(3.-2.*ts);
    rampSpeed=6.*ts*(1.-ts)/rampInterval;
    rampAcceleration=(6.-12.*ts)/(rampInterval*rampInterval);
  }
  else
  {
    ramp=1.;
    rampSpeed=0.;
    rampAcceleration=0.;
  }
  
  return 0;
}


//=======================================================================================================
/// /brief Construct a grid in the past (used for starting a multi-step scheme for e.g.)
/// /param cgf (input/output) : on output, this holds the grid at time cgf.t
//=======================================================================================================
int MovingGrids::
getPastTimeGrid( GridFunction & cgf  )
{

  if( true || debug() & 2 )
    printF("--MvG-- getPastTimeGrid at t=%8.2e\n",cgf.t);


  // ---------------------------------------------------------------------------------------------
  // ---- First time through generate the MatrixTransforms used by non-deforming moving grids ----
  // ---------------------------------------------------------------------------------------------
  initializeMovingGridTransforms( cgf );

  CompositeGrid & cg = cgf.cg;

  const real t0=parameters.dbase.get<real >("tInitial"); // *wdh* 040503 
  const real pastTime =cgf.t;
  const real deltaT = pastTime-t0;
  
  for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )  
  {
    if( movingGrid(grid) )
    {
      MatrixTransform & transform = *cgf.transform[grid];
      if( moveOption(grid)==notMoving )
      { 
        // nothing to do in this case
      }
      else if( moveOption(grid)==matrixMotion )
      {
        // MatrixMotion body : pre-defined matrix motions (rigid and scaling motions)
        const int b=int(moveParameters(0,grid)+.5);
        assert( b>=0 && b<numberOfMatrixMotionBodies );
        
	MatrixMotion & matMotion = *matrixMotionBody[b];
	
	// printF("moveGrids: matrixMotion: grid=%i, body: b=%i\n",grid,b);
	RealArray rMatrix(4,4);  // holds "rotation" matrix and shift 
	matMotion.getMotion( pastTime, rMatrix );
	transform.reset();
	transform.rotate( rMatrix );  
	transform.shift( rMatrix(0,3),rMatrix(1,3),rMatrix(2,3));

      }
      else if( moveOption(grid)==shift )
      {
	const RealArray & vector = moveParameters(Range(0,2),grid); // shift vector
	// shift = t*vector
        const real speed = deltaT*moveParameters(3,grid);

        if( debug() & 2 )
	{
          fprintf(parameters.dbase.get<FILE* >("moveFile")," **moveGrids:getPastTimeGrid: shift grid %i (%s) vector=(%8.2e,%8.2e,%8.2e) speed=%8.2e tStart=%8.2e, t3=%8.2e, deltaT=%9.3e\n",
		  grid,(const char*)cg[grid].getName(),vector(0),vector(1),vector(2),speed,t0,pastTime,deltaT);

          display(transform.matrix->matrix,"matrix from transform",parameters.dbase.get<FILE* >("moveFile"),"%5.3f ");
	}
	
	transform.shift(vector(0)*speed,vector(1)*speed,vector(2)*speed);
      
      }
      else if(  moveOption(grid)==rotate )
      {
	const RealArray & x0 =moveParameters(Range(0,2),grid); // centre of rotation
	// tn has base 3 here: const RealArray & tn =moveParameters(Range(3,5),grid); // tangent to rotation axis
        RealArray tn(3); tn = moveParameters(Range(3,5),grid); // tangent to rotation axis
        const Real rampInterval=moveParameters(7,grid);
	
        // static real deltaTheta=180.* (4.*atan(1.)/180); 

        real rampOld, rampNew, rampSpeed, rampAcceleration ;
        getRamp(t0,rampInterval, rampOld,rampSpeed,rampAcceleration );
        getRamp(pastTime,rampInterval, rampNew,rampSpeed,rampAcceleration );

        // const real angularSpeed = deltaT*moveParameters(6,grid)*2.*Pi;
        const real omega=moveParameters(6,grid);
        const real angularSpeed = 2.*Pi*omega*( rampNew*pastTime-rampOld*t0);
        
        if( debug() & 2 )
  	  printF("--MvG--:getPastTimeGrid: rotate grid: %s,  rotation rate =%e, center=(%e,%e,%e) ramp=[%8.2e,%8.2e,%8.2e]\n",
		 (const char *)cg[grid].mapping().getName(Mapping::mappingName) ,
		 moveParameters(6,grid),x0(0),x0(1),x0(2),rampNew,rampSpeed,rampAcceleration);
      
	// shift to centre, rotate and shift back
	transform.shift(-x0(0),-x0(1),-x0(2));
        if( cg.numberOfDimensions()==2 )
	{
  	  transform.rotate(axis3,angularSpeed);
	}
	else if( tn(0)>0. && tn(1)==0. && tn(2)==0. )
	{
  	  transform.rotate(axis1,angularSpeed);
	}
	else if( tn(0)==0. && tn(1)>0. && tn(2)==0. )
	{
  	  transform.rotate(axis2,angularSpeed);
	}
	else if( tn(0)==0. && tn(1)==0. && tn(2)>0. )
	{
  	  transform.rotate(axis3,angularSpeed);
	}
        else
	{
	  printF("MovingGrids::getPastTimeGrid::ERROR: can only rotate about the positive x, y or z-axis for now\n"
		 " tangent=(%8.2e,%8.2e,%8.2e)\n"
		 " rotation-point=(%8.2e,%8.2e,%8.2e)\n",tn(0),tn(1),tn(2),x0(0),x0(1),x0(2));
	  OV_ABORT("Error");
	}
	transform.shift( x0(0), x0(1), x0(2));
      
      }
      else if(  moveOption(grid)==scale )
      { 
        // scale grid like x(r,t) = (1+scale*t)*x(r,t=0)

	real sx=moveParameters(0,grid);
	real sy=moveParameters(1,grid);
	real sz=moveParameters(2,grid);

        if( debug() & 4 )
  	  printF("moveGrids:getPastTimeGrid: scale grid%i sx=%e sy=%e sz=%e at t=%9.3e\n",grid,sx,sy,sz,pastTime);

	real xFactor=(1.+sx*pastTime);
	real yFactor=(1.+sy*pastTime);
	real zFactor=(1.+sz*pastTime);
	bool incremental=false;  // perform an absolute scaling from the original mapping
	transform.scale(xFactor,yFactor,zFactor,incremental);
      
      }
      else if( moveOption(grid)==oscillate )
      {
	// x(t) = (1-cos([t-tOrigin]*omega/(2 pi)))*amplitude
	const RealArray & vector = moveParameters(Range(0,2),grid); // tangent
        const real omega         = moveParameters(3,grid)*2.*Pi;    // oscillation rate
        const real amplitude     = moveParameters(4,grid);          // amplitude
        const real tOrigin       = moveParameters(5,grid);                  

        // compute the shift from time t=0 to avoid accumulation of round-off.
	const real deltaX=amplitude*(cos(omega*(t0-tOrigin))-cos(omega*(pastTime-tOrigin)));
        if( debug() & 2 )
  	  printF("moveGrids:getPastTimeGrid: oscillate grid: %s, t=%8.1e, delta = (%8.1e,%8.1e,%8.1e)\n",
		 (const char*)cg[grid].mapping().getName(Mapping::mappingName),
                 pastTime,vector(0)*deltaX,vector(1)*deltaX,vector(2)*deltaX);

        transform.reset();
	transform.shift(vector(0)*deltaX,vector(1)*deltaX,vector(2)*deltaX);
      
      }
      else if( moveOption(grid)==rigidBody )
      {
	// rigid body motion consists of a translation and a rotation
        RealArray x1(3),x3(3),r(3,3);
        int b=int(moveParameters(0,grid)+.5);
        assert( b>=0 && b<numberOfRigidBodies );
        
        body[b]->getInitialConditions(x1);
        body[b]->getPosition(pastTime,x3);
	
        if( debug() & 2 )
	{
          fprintf(parameters.dbase.get<FILE* >("moveFile"),"getPastTimeGrid: transform rigid body: t0=%7.2e x1=xCM(t=0)=(%6.1e,%6.1e,%6.1e) pastTime=%7.2e"
		  " x3=(%6.1e,%6.1e,%6.1e)\n",t0,x1(0),x1(1),x1(2),pastTime,x3(0),x3(1),x3(2),pastTime);
	}
	
        body[b]->getRotationMatrix( pastTime,r );

	if( true )
	{
	  printF("--MVG-- getPastGrid: transform rigid body: t0=%7.2e x1=xCM(t=0)=(%6.1e,%6.1e,%6.1e) pastTime=%7.2e"
		  " x3=xCM(pastTime)-(%6.1e,%6.1e,%6.1e)\n",t0,x1(0),x1(1),x1(2),pastTime,x3(0),x3(1),x3(2),pastTime);
          // ::display(r,"rotation matrix at past time","%10.8f ");
        }
        
        if( parameters.dbase.get<int>("debug") & 4 )
        {
          FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
          fPrintF(debugFile,"--MVG-- getPastGrid: transform rigid body: t0=%7.2e x1=xCM(t=0)=(%6.1e,%6.1e,%6.1e) pastTime=%7.2e"
		  " x3=xCM(pastTime)-(%6.1e,%6.1e,%6.1e)\n",t0,x1(0),x1(1),x1(2),pastTime,x3(0),x3(1),x3(2),pastTime);
        }

	

        transform.reset();

        transform.shift(-x1(0),-x1(1),-x1(2));   // move xCM(0) to the origin
        bool incremental=true;
   	transform.rotate( r,incremental );     // rotate about origin (which is really xCM(0))

        // transform.shift(x1(0),x1(1),x1(2));   // shift back
   	// transform.shift(x3(0)-x1(0),x3(1)-x1(1),x3(2)-x1(2));   // absolute move to current position
        // replace above two shifts by one shift
   	transform.shift(x3(0),x3(1),x3(2));
      }
      else if ( moveOption(grid) == deformingBody ) 
      {  
        // done below ---
      }
      else if( moveOption(grid)==userDefinedMovingGrid )
      {
	printF("--MvG:getPastTimeGrid: WARNING: finish me formoveOption(grid)==userDefinedMovingGrid\n");

        //  userDefinedTransformMotion( t1,t2,pastTime,dt0,cgf1,cgf2,cgf3,grid );

      }
      else
      {
	printF("moveGrids: unknown movingGridOption = %i\n",(int)moveOption(grid));
      }
      
    } // end if (movingGrid(grid)) ...
  }

  // ***********************************************
  // ** Move DeformingBodyGrids          ***********
  // ***********************************************

  if ( numberOfDeformingBodies != 0) 
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      deformingBodyList[b]->getPastTimeGrid(  pastTime,cg );

    }
  }
  
  return 0;
}

typedef MatrixTransform *MatrixTransformPointer;


// =================================================================================================
/// /brief Initialize the MatrixTransforms that are used for (non-deforming) moving grids
// =================================================================================================
int MovingGrids::
initializeMovingGridTransforms( GridFunction & cgf3 )
{
  if( cgf3.transform==NULL )// should check if Bodies have been initialized
  { 
    // -----------------------------------------------------------
    // ---- First time through we generate the transformation ----
    // -----------------------------------------------------------

    CompositeGrid & cg = cgf3.cg;
    
    const int numberOfDimensions = cgf3.cg.numberOfDimensions();
    const int numberOfBaseGrids = cgf3.cg.numberOfBaseGrids();  // *wdh* use base grids 040314
    const int numberOfComponentGrids = cgf3.cg.numberOfComponentGrids(); 

    cgf3.numberOfTransformMappings=numberOfBaseGrids; 
    cgf3.transform= new MatrixTransformPointer [numberOfBaseGrids]; // allocate transforms for all grids **wasteful?
    //--> yes it is especially in 3D with few rigidBodies. DeformingBodies don't need transforms

    for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )  
    {
      cgf3.transform[grid]= NULL;
      if( movingGrid(grid) ) 
      {
	if ( moveOption(grid)==deformingBody ) 
        {
          // deforming grids do not use a MatrixTransform 
	} 
	else  //  moving, nondeforming, grid
	{

	  Mapping *mapPointer =cgf3.cg[grid].mapping().mapPointer;
	  // *** watch out -- bc's are reset to those in *mapPointer

	  
          if( (parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::readInitialConditionFromShowFile ||
               parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::readInitialConditionFromRestartFile  ) && 
              mapPointer->getClassName()=="MatrixTransform" )
	  {
            // This is a restart -- we can use the existing matrix transform
            // **** we need to do this otherwise the grid will not move to the correct position 
            //      once it starts to move ****
            printF(" ****** MovingGrids: re-using existing transform for grid=%i ****\n",grid);
	    
	    cgf3.transform[grid]=(MatrixTransform*)mapPointer;
	  }
	  else
	  {
	    cgf3.transform[grid]= new MatrixTransform(*mapPointer);
	  }
	  
	  MatrixTransform & transform = *cgf3.transform[grid];
	  
	  transform.setName(Mapping::mappingName,sPrintF("transform[grid=%i]",grid));
	  
	  transform.incrementReferenceCount();   // *wdh 961203 -- set reference count

          // We need to set the number of grid points since this may have changed due to AMR *wdh* 040314
          const IntegerArray & gid = cgf3.cg[grid].gridIndexRange();
	  for( int axis=0; axis<numberOfDimensions; axis++ )
	  {
	    transform.setGridDimensions(axis,gid(1,axis)-gid(0,axis)+1);
	  }
	  
	  cgf3.cg[grid].reference(transform);               // move grid 1 *************
	  cgf3.cg[grid].update(MappedGrid::THEvertex);   // ** 000308 only update vertex

          if( true )
	  {
	    const IntegerArray & d = cgf3.cg[grid].dimension();
	    printF("--MVG--initMovingGridTrans: build transform: cfg3: grid=%i dimension=[%i,%i][%i,%i], t3=%9.3e\n",
                   grid,d(0,0),d(1,0),d(0,1),d(1,1),cgf3.t);
	  }
	  
	  // display(cgf3.cg[0].boundaryCondition(),"moveGrids:after boundary conditions");
	}
      }
      //else //not moving grid

      // ** cgf3.cg.updateReferences();   // *wdh 961204
    }

    // We need to adjust any refinement grids of rectangular base grids that are moving -- the
    // refinement grids are no longer considered to be rectangular  *wdh* 040316
    for( int grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ ) 
    {
      if( cgf3.cg.refinementLevelNumber(grid)>0 )
      {
	const int base = cgf3.cg.baseGridNumber(grid);
	if( cgf3.cg[grid].isRectangular() && movingGrid(base) )
	{
	  cgf3.cg[grid].mapping().getMapping().setMappingCoordinateSystem( Mapping::general ); 
	}
      }
    }
    

    cgf3.cg.updateReferences();   // *wdh* 040221 
    
  }

  return 0;
}


//=======================================================================================================
/// \brief Move grids to the new time.
/// \details 
///   The basic idea is to advance the solution from grid g1 at time t1 to grid g3 at time t3
///  using the grid velocity from grid g2 at time t2:
///     -  g3(t3) <- g1(t1) + (t3-t1)*d(g2(t2))/dt
///  Steps:
///     - detect collisions
///     - compute forces and moments on rigid and deforming bodies
///     - compute grid velocity of g2
///     - move grids
///     - compute grid velocity of g3 
/// 
/// \param t1,cgf1 (input) : grid and solution at time t1
/// \param t2,cgf2 (input) : grid and solution at time t2. The grid velocity, cgf2.gridVelocity, is computed
///                and used from this time.
/// \param t3 (input) : new time 
/// \param cgf3 (output): holds new grid at time t3. The grid velocity, cgf3.gridVelocity, is computed at the. 
///        end of the step. NOTE: cgf3 must be different from cgf1. 
//=======================================================================================================
int MovingGrids::
moveGrids(const real & t1, 
	  const real & t2, 
	  const real & t3,
	  const real & dt0,
	  GridFunction & cgf1,  
	  GridFunction & cgf2,
	  GridFunction & cgf3 )
{
  real cpu0 = getCPU();

  // ** debug0=3; // *********************
  

  if( debug() & 2 )
    fprintf(parameters.dbase.get<FILE* >("moveFile"),"\n****moveGrids: start *** movingGridProblem=%i\n",
	    (int)movingGridProblem);

  if( !movingGridProblem )
    return 0;
  
  assert( isInitialized );
  
  assert( &cgf1 != &cgf3 );

  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  
  // Impose constraints on moving bodies  **wdh**
  //  (1) first compute where all bodies want to move, xNew(.,b)
  //  (2) determine if any bodies fail to satisfiy the constraints (e.g. collide).
  //  (3a) adjust position somehow ?? elastic collision?
  //  (3b) or adjust forces on bodies?
  //
  //  body[b1]->constraint
  // detect collisions

  detectCollisions(cgf1); // ??? is this the correct place to do this ?


  //-----------MOVE THE BODIES (not grids)------------------------
  //..Compute forces and moments on rigid bodies
  rigidBodyMotion( t1,t2,t3,dt0,cgf1,cgf2,cgf3 );

  //..Deforming bodies: generic, elastic or free-surfaces
  //  Move the BODY, later we'll move the surrounding GRIDS
  moveDeformingBodies( t1,t2,t3,dt0,cgf1,cgf2,cgf3 );
  
  // perform any general computations for user defined motion.
  userDefinedMotion( t1,t2,t3,dt0,cgf1,cgf2,cgf3 );


  CompositeGrid & cg = cgf1.cg;
  static char buff[80];

  //------------MOVE THE GRIDS-------------------------------------
  //..For moving grids --- update mapping for each component grid 
  //   replace the mapping for each moving body
  //   (do nothing for fixed grids)
  //   NOW:          create a transform
  //   SHOULD BE:    REPLACE the mapping!!

  const int numberOfBaseGrids = cg.numberOfBaseGrids();  // *wdh* use base grids 040314
  const int numberOfComponentGrids = cg.numberOfComponentGrids(); 

  // ---------------------------------------------------------------------------------------------
  // ---- First time through generate the MatrixTransforms used by non-deforming moving grids ----
  // ---------------------------------------------------------------------------------------------
  initializeMovingGridTransforms( cgf3 );


  // if( cgf3.transform==NULL )// should check if Bodies have been initialized
  // { 

  //   cgf3.numberOfTransformMappings=numberOfBaseGrids; 
  //   cgf3.transform= new MatrixTransformPointer [numberOfBaseGrids]; // allocate transforms for all grids **wasteful?
  //   //--> yes it is especially in 3D with few rigidBodies. DeformingBodies don't need transforms

  //   for( int grid=0; grid<cg.numberOfBaseGrids(); grid++ )  
  //   {
  //     cgf3.transform[grid]= NULL;
  //     if( movingGrid(grid) ) 
  //     {
  // 	if ( moveOption(grid)==deformingBody ) 
  //       {
  //         // deforming grids do not use a MatrixTransform 
  // 	} 
  // 	else  //  moving, nondeforming, grid
  // 	{

  // 	  Mapping *mapPointer =cgf3.cg[grid].mapping().mapPointer;
  // 	  // *** watch out -- bc's are reset to those in *mapPointer

	  
  //         if( (parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::readInitialConditionFromShowFile ||
  //              parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::readInitialConditionFromRestartFile  ) && 
  //             mapPointer->getClassName()=="MatrixTransform" )
  // 	  {
  //           // This is a restart -- we can use the existing matrix transform
  //           // **** we need to do this otherwise the grid will not move to the correct position 
  //           //      once it starts to move ****
  //           printF(" ****** MovingGrids: re-using existing transform for grid=%i ****\n",grid);
	    
  // 	    cgf3.transform[grid]=(MatrixTransform*)mapPointer;
  // 	  }
  // 	  else
  // 	  {
  // 	    cgf3.transform[grid]= new MatrixTransform(*mapPointer);
  // 	  }
	  
  // 	  MatrixTransform & transform = *cgf3.transform[grid];
	  
  // 	  transform.setName(Mapping::mappingName,sPrintF(buff,"transform[grid=%i]",grid));
	  
  // 	  transform.incrementReferenceCount();   // *wdh 961203 -- set reference count

  //         // We need to set the number of grid points since this may have changed due to AMR *wdh* 040314
  //         const IntegerArray & gid = cgf3.cg[grid].gridIndexRange();
  // 	  for( int axis=0; axis<numberOfDimensions; axis++ )
  // 	  {
  // 	    transform.setGridDimensions(axis,gid(1,axis)-gid(0,axis)+1);
  // 	  }
	  
  // 	  cgf3.cg[grid].reference(transform);               // move grid 1 *************
  // 	  cgf3.cg[grid].update(MappedGrid::THEvertex);   // ** 000308 only update vertex

  //         if( true )
  // 	  {
  // 	    const IntegerArray & d = cgf3.cg[grid].dimension();
  // 	    printF(" ***moveGrids: build transform: cfg3: grid=%i dimension=[%i,%i][%i,%i], t3=%9.3e\n",
  //                  grid,d(0,0),d(1,0),d(0,1),d(1,1),t3);
  // 	  }
	  
  // 	  // display(cgf3.cg[0].boundaryCondition(),"moveGrids:after boundary conditions");
  // 	}
  //     }
  //     //else //not moving grid

  //     // ** cgf3.cg.updateReferences();   // *wdh 961204
  //   }

  //   // We need to adjust any refinement grids of rectangular base grids that are moving -- the
  //   // refinement grids are no longer considered to be rectangular  *wdh* 040316
  //   for( int grid=0; grid<cgf3.cg.numberOfComponentGrids(); grid++ ) 
  //   {
  //     if( cgf3.cg.refinementLevelNumber(grid)>0 )
  //     {
  // 	const int base = cgf3.cg.baseGridNumber(grid);
  // 	if( cgf3.cg[grid].isRectangular() && movingGrid(base) )
  // 	{
  // 	  cgf3.cg[grid].mapping().getMapping().setMappingCoordinateSystem( Mapping::general ); 
  // 	}
  //     }
  //   }
    

  //   cgf3.cg.updateReferences();   // *wdh* 040221 
    
  // }

  //
  // ....... Move the grids surrounding the moving bodies .....
  //

  real oldT=cgf3.t;
  real newT=t3;
  real deltaT=t3-cgf3.t;
  cgf3.t=t3;                      // The mappings for this grid now exists at this time

  // compute the grid velocity    ----------- GET GRID VELOCITY at t2, t3

  if( debug() & 2 )
    fprintf(parameters.dbase.get<FILE* >("moveFile"),"$$$$$ MovingGrids:moveGrids: getGridVelocity for cgf2 at t2=%7.4f\n",t2);
  
  getGridVelocity( cgf2,t2 );
  // *wdh* 000504 getGridVelocity( cgf3,t3 );

  // ***********************************************
  // ** Move the matrix-motion and rigid bodies ****
  // ***********************************************

  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    // *wdh* 2012/03/07 -- unvalidate geometry on base grids AND AMR grids!
    if( movingGrid(grid) )
    {
      // Invalidate the geometry for this grid so that the vertex will be re-computed *wdh* 081121
      // The new vertex may be needed to compute the grid velocity. 
      // Previously ogen invalidated the geometry. 
      cgf3.cg[grid].geometryHasChanged(~MappedGrid::THEmask); // ** this invalidates all geometry except the mask **
    }
    
  }

  for( int grid=0; grid<numberOfBaseGrids; grid++ )  
  {
    if( debug() & 2 )
      fprintf(parameters.dbase.get<FILE* >("moveFile"),"moveGrids: grid=%i  movingGrid =%i moveOption=%i \n",
              grid, movingGrid(grid),moveOption(grid) );
    
    if( movingGrid(grid) )
    {
//       // Invalidate the geometry for this grid so that the vertex will be re-computed *wdh* 081121
//       // The new vertex may be needed to compute the grid velocity. 
//       // Previously ogen invalidated the geometry. 
//       cgf3.cg[grid].geometryHasChanged(~MappedGrid::THEmask); // ** this invalidates all geometry except the mask **


      MatrixTransform & transform = *cgf3.transform[grid];  // --- move grids in cgf3 ----
      
      if( moveOption(grid)==notMoving )
      { 
        // nothing to do in this case
      }
      else if( moveOption(grid)==matrixMotion )
      {
        // MatrixMotion body : pre-defined matrix motions (rigid and scaling motions)

        // wdh 100317 : *new* way that replaces old "shift" rotate" "scale" and "oscillate" 

        const int b=int(moveParameters(0,grid)+.5);
        assert( b>=0 && b<numberOfMatrixMotionBodies );
        
	MatrixMotion & matMotion = *matrixMotionBody[b];
	
	// printF("moveGrids: matrixMotion: grid=%i, body: b=%i\n",grid,b);
	RealArray rMatrix(4,4);  // holds "rotation" matrix and shift 
	matMotion.getMotion( newT, rMatrix );
	transform.reset();
	transform.rotate( rMatrix );  
	transform.shift( rMatrix(0,3),rMatrix(1,3),rMatrix(2,3));

      }
      else if( moveOption(grid)==shift )
      {
	const RealArray & vector = moveParameters(Range(0,2),grid); // shift vector
	// shift = t*vector
        const real speed = deltaT*moveParameters(3,grid);

        if( debug() & 2 )
	{
          fprintf(parameters.dbase.get<FILE* >("moveFile")," **moveGrids: shift grid %i (%s) vector=(%8.2e,%8.2e,%8.2e) speed=%8.2e tStart=%8.2e, t3=%8.2e, deltaT=%9.3e\n",
		  grid,(const char*)cgf3.cg[grid].getName(),vector(0),vector(1),vector(2),speed,oldT,t3,deltaT);

          display(transform.matrix->matrix,"matrix from transform",parameters.dbase.get<FILE* >("moveFile"),"%5.3f ");
	}
	
	transform.shift(vector(0)*speed,vector(1)*speed,vector(2)*speed);
      
      }
      else if(  moveOption(grid)==rotate )
      {
	const RealArray & x0 =moveParameters(Range(0,2),grid); // centre of rotation
	// tn has base 3 here: const RealArray & tn =moveParameters(Range(3,5),grid); // tangent to rotation axis
        RealArray tn(3); tn = moveParameters(Range(3,5),grid); // tangent to rotation axis
        const Real rampInterval=moveParameters(7,grid);
	
        // static real deltaTheta=180.* (4.*atan(1.)/180); 

        real rampOld, rampNew, rampSpeed, rampAcceleration ;
        getRamp(oldT,rampInterval, rampOld,rampSpeed,rampAcceleration );
        getRamp(newT,rampInterval, rampNew,rampSpeed,rampAcceleration );

        // const real angularSpeed = deltaT*moveParameters(6,grid)*2.*Pi;
        const real omega=moveParameters(6,grid);
        const real angularSpeed = 2.*Pi*omega*( rampNew*newT-rampOld*oldT);
        
        if( debug() & 2 )
  	  printF("moveGrids: rotate grid: %s,  rotation rate =%e, center=(%e,%e,%e) ramp=[%8.2e,%8.2e,%8.2e]\n",
		 (const char *)cgf3.cg[grid].mapping().getName(Mapping::mappingName) ,
		 moveParameters(6,grid),x0(0),x0(1),x0(2),rampNew,rampSpeed,rampAcceleration);
      
	// shift to centre, rotate and shift back
	transform.shift(-x0(0),-x0(1),-x0(2));
        if( cg.numberOfDimensions()==2 )
	{
  	  transform.rotate(axis3,angularSpeed);
	}
	else if( tn(0)>0. && tn(1)==0. && tn(2)==0. )
	{
  	  transform.rotate(axis1,angularSpeed);
	}
	else if( tn(0)==0. && tn(1)>0. && tn(2)==0. )
	{
  	  transform.rotate(axis2,angularSpeed);
	}
	else if( tn(0)==0. && tn(1)==0. && tn(2)>0. )
	{
  	  transform.rotate(axis3,angularSpeed);
	}
        else
	{
	  printF("MovingGrids::moveGrids:ERROR: can only rotate about the positive x, y or z-axis for now\n"
		 " tangent=(%8.2e,%8.2e,%8.2e)\n"
		 " rotation-point=(%8.2e,%8.2e,%8.2e)\n",tn(0),tn(1),tn(2),x0(0),x0(1),x0(2));
	  OV_ABORT("Error");
	}
	transform.shift( x0(0), x0(1), x0(2));
      
      }
      else if(  moveOption(grid)==scale )
      { 
        // scale grid like x(r,t) = (1+scale*t)*x(r,t=0)

	real sx=moveParameters(0,grid);
	real sy=moveParameters(1,grid);
	real sz=moveParameters(2,grid);

        if( debug() & 4 )
  	  printF("moveGrids: scale grid%i sx=%e sy=%e sz=%e at t=%9.3e\n",grid,sx,sy,sz,newT);

	real xFactor=(1.+sx*newT);
	real yFactor=(1.+sy*newT);
	real zFactor=(1.+sz*newT);
	bool incremental=false;  // perform an absolute scaling from the original mapping
	transform.scale(xFactor,yFactor,zFactor,incremental);
      
      }
      else if( moveOption(grid)==oscillate )
      {
	// x(t) = (1-cos([t-tOrigin]*omega/(2 pi)))*amplitude
	const RealArray & vector = moveParameters(Range(0,2),grid); // tangent
        const real omega         = moveParameters(3,grid)*2.*Pi;    // oscillation rate
        const real amplitude     = moveParameters(4,grid);          // amplitude
        const real tOrigin       = moveParameters(5,grid);                  

        // compute the shift from time t=0 to avoid accumulation of round-off.
        const real t0=parameters.dbase.get<real >("tInitial"); // *wdh* 040503 
	const real deltaX=amplitude*(cos(omega*(t0-tOrigin))-cos(omega*(newT-tOrigin)));
        if( debug() & 2 )
  	  printF("moveGrids: oscillate grid: %s, t=%8.1e, delta = (%8.1e,%8.1e,%8.1e)\n",
		 (const char*)cgf3.cg[grid].mapping().getName(Mapping::mappingName),
                 newT,vector(0)*deltaX,vector(1)*deltaX,vector(2)*deltaX);

        transform.reset();
	transform.shift(vector(0)*deltaX,vector(1)*deltaX,vector(2)*deltaX);
      
      }
      else if( moveOption(grid)==rigidBody )
      {
	// rigid body motion consists of a translation and a rotation
        RealArray x1(3),x3(3),r(3,3);
        int b=int(moveParameters(0,grid)+.5);
        assert( b>=0 && b<numberOfRigidBodies );
        
        body[b]->getInitialConditions(x1);
        body[b]->getPosition(t3,x3);
	
        if( debug() & 2 )
	{
          fprintf(parameters.dbase.get<FILE* >("moveFile"),"transform rigid body: t1=%7.2e x1=xCM(t=0)=(%6.1e,%6.1e,%6.1e) t3=%7.2e"
		  " x3=(%6.1e,%6.1e,%6.1e)\n",t1,x1(0),x1(1),x1(2),t3,x3(0),x3(1),x3(2));
	}
	
        body[b]->getRotationMatrix( t3,r );
        transform.reset();

        transform.shift(-x1(0),-x1(1),-x1(2));   // move xCM(0) to the origin
        bool incremental=true;
   	transform.rotate( r,incremental );     // rotate about origin (which is really xCM(0))

        // transform.shift(x1(0),x1(1),x1(2));   // shift back
   	// transform.shift(x3(0)-x1(0),x3(1)-x1(1),x3(2)-x1(2));   // absolute move to current position
        // replace above two shifts by one shift
   	transform.shift(x3(0),x3(1),x3(2));
      }
      else if ( moveOption(grid) == deformingBody ) 
      {  
	//..update below..
      }
      else if( moveOption(grid)==userDefinedMovingGrid )
      {
        userDefinedTransformMotion( t1,t2,t3,dt0,cgf1,cgf2,cgf3,grid );

      }
      else
      {
	printF("moveGrids: unknown movingGridOption = %i\n",(int)moveOption(grid));
      }
      

      // cgf3.cg[grid].update(MappedGrid::THEvertex | MappedGrid::THEcenter);

    } // end if (movingGrid(grid)) ...
  }

  // ***********************************************
  // ** Move DeformingBodyGrids          ***********
  // ***********************************************
  //
  // NOTES:  **pf
  //    * there are usually several deformingGrids/deformingBody
  //      --> hence loop through bodies--> grids for each body
  //    * need to add error checking-->should not update one grid more than once!
  //

  if ( numberOfDeformingBodies != 0) 
  {
    if( debug() & 4 ){ printF("\nMovingGrids :: moveGrids -- deformingGrid update\n");  }   
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      deformingBodyList[b]->regenerateComponentGrids(  newT, cgf3.cg ); 
    }
    

//     for( grid=0; grid < cg.numberOfComponentGrids(); grid++ )
//     {
//       if( debug() & 4 ){ cout << "++ grid number " << grid << endl; } 
//       if( movingGrid(grid) ) 
//       {
// 	if ( moveOption(grid) == deformingBody ) 
// 	{  
// 	  int b=int(moveParameters(0,grid)+.5);  // deformingBodyNumber
// 	  assert( b>=0 && b< numberOfDeformingBodies );
		
// 	  if( debug() & 4 ) cout << "++++ grid " << grid << " is deformingBody #" << b << endl;
		
// 	  int numThisBodyGrids=deformingBodyList[b]->getNumberOfGrids(); 
// 	  assert(numThisBodyGrids!=0); // should have at least the present component='grid'
// 	  assert(numThisBodyGrids==1); // Full case (several grids) not impl. yet
		
// 	  deformingBodyList[b]->regenerateComponentGrid(  newT, grid, cgf3.cg ); 
		
// 	} // end if (deformingBody==moveOption(grid))
//       } // end if(movingGrid(grid))
//     } // end for grid


  }// end if numberOfDeformingGrids !=0

  // ..Done generating new component grids, now updateRefs in cg for ogen
  cgf3.cg.updateReferences();

  if( debug() & 8  )
    fprintf(parameters.dbase.get<FILE* >("moveFile"),"$$$$$ MovingGrids:moveGrids: getGridVelocity for cgf3 at t3=%7.4f\n",t3);
  getGridVelocity( cgf3,t3 );   // do this here after the grid has been advanced
  return 0;
}

//==================================================================================
/// \brief Corrector step for moving grids.
/// \details This function is called at the corrector step to update the moving grids. For example,
///  in a predictor corrector type algorithm we may want to correct the forces and torques
///   on bodies since the solution can depend on these (For INS the pressure BC depends on
///  the acceleration on the boundary ).
/// \param t1,cgf1 (input) : solution at the old time
/// \param t2,cgf2 (input) : solution at the new time (these are valid values)
//==================================================================================
int MovingGrids::
correctGrids(const real t1,
	     const real t2, 
	     GridFunction & cgf1,
	     GridFunction & cgf2 )
{

  // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
  // -- finish me --
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;

  // we should add a special corrector step or option to the RigidBodyMotion: 
  real dt0=0.;
  rigidBodyMotion(t2,t2,t2,dt0,cgf2,cgf2,cgf2);

  if( numberOfDeformingBodies != 0) 
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      deformingBodyList[b]->correct( t1,t2,cgf1,cgf2 );
      maximumRelativeCorrection = max(maximumRelativeCorrection,deformingBodyList[b]->getMaximumRelativeCorrection());
      correctionHasConverged = correctionHasConverged && deformingBodyList[b]->hasCorrectionConverged();
    }  
  }
  
  if( recomputeGridVelocityOnCorrection ) 
  {
    // --- re-compute the grid velocity ---// *new* *wdh* July 5, 2016

    if( debug() & 4 )
      printF("--MVG--correctGrids INFO : t=%9.3e Update the grid velocity!!! ***NEW***\n",cgf2.t);
    
    //  FORCE getGridVelocity to recompute the grid velocity by 
    //     making sure cgf2.gridVelocityTime != cgf2.t
    cgf2.gridVelocityTime=cgf2.t - 1.;  
    getGridVelocity( cgf2,cgf2.t );
  }
  
  
}

//=================================================================================
/// \brief Return the mass of the fluid that would be contained in rigid body "b"
//=================================================================================
real MovingGrids::
getFluidMassOfBody( int b ) const
{
  real fluidMass=0.;
  const real fluidDensity = parameters.dbase.get<real >("fluidDensity");
    
  if( fluidDensity!=0. )
  {
    const real bodyMass    =  body[b]->getMass();
    const real bodyDensity =  body[b]->getDensity();
    real bodyVolume        =  body[b]->getVolume();

    if( bodyVolume<0. && bodyDensity>0. && bodyMass>0. )
    {
      // bodyVolume has not been set but body density and mass are known:
      bodyVolume= bodyMass/bodyDensity;
    }

    if( bodyVolume>0. )
    {
      fluidMass = fluidDensity*bodyVolume; 

    }
    else
    {
      printF("--MVG-- getFluidMassOfBody::ERROR: The fluid density is not zero but the "
	     "body volume is unknown for body b=%i.\n",b);
    }
  }
  return fluidMass;
}


//=================================================================================
/// \brief Return the integrated fluid force and torques on the rigid bodies.
///
/// \param force(0:2,b) (output) : integrated force on body b, b=0,1,..,numberOfRigidBodies-1
/// \param torque(0:2,b) (output) : integrated torque on body b, b=0,1,..,numberOfRigidBodies-1
///        Note that in 2D the only torque component is torque(2,b) . 
/// \param gf0 (input) : holds fluid solution (for pressure and viscous stresses)
/// \param includeExternal (input) : if true, include external forces including gravitation force (buoyancy)
/// \param includeViscosity (input) : if true, include viscous tractions in force.
//
// Note: This code originally appeared in MovingGrids::rigidBodyMotion
// 
//=================================================================================
int MovingGrids::
getForceOnRigidBodies( RealArray & force, RealArray & torque, GridFunction & gf0,
                       bool includeExternal /*= true */, 
                       bool includeViscosity /* = true */ )
{
  if( numberOfRigidBodies<=0 )
    return 1;

  force.redim(3,numberOfRigidBodies);   force=0.;
  torque.redim(3,numberOfRigidBodies);  torque=0.;
  
  RealCompositeGridFunction & u = gf0.u;
  CompositeGrid & cg = gf0.cg;
  const int numberOfDimensions = cg.numberOfDimensions();

  // *wdh* 120113 -- save stress and torque in the same grid function!
  Range all;
  const int numberOfStressComponents=numberOfDimensions;
  const int numberOfTorqueComponents= numberOfDimensions==2 ? 1 : 3;
  const int numberOfForceAndTorqueComponents=numberOfStressComponents+numberOfTorqueComponents;
  const int torquec =numberOfStressComponents; // first Torque component sits here 

  RealCompositeGridFunction stress(cg,all,all,all,numberOfForceAndTorqueComponents); // **** fix this ****
  stress=0.; // do we need this?

  // const int rc=parameters.dbase.get<int >("rc");
  // const int uc=parameters.dbase.get<int >("uc");
  // const int vc=parameters.dbase.get<int >("vc");
  // const int wc=parameters.dbase.get<int >("wc");
  // const int pc=parameters.dbase.get<int >("pc");
  // const int tc=parameters.dbase.get<int >("tc");
  // const real nu = parameters.dbase.get<real >("nu");
  // const Range V(parameters.dbase.get<int >("uc"),parameters.dbase.get<int >("uc")+cg.numberOfDimensions()-1);

  Index Ib1,Ib2,Ib3; 
  const int extraInTangential=2;  // *wdh* 2012/02/25 make sure we assign enough values for impedance

  // --------- LOOP OVER RIGID BODIES ------------
  for( int b=0; b<numberOfRigidBodies; b++ )
  {
    assert( integrate!=NULL );

    // ------ BEGIN: compute stress*normal ---------
    const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
    for( int face=0; face<numberOfFaces; face++ )
    {
      int side=-1,axis,grid;
      integrate->getFace(b,face,side,axis,grid);
      assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
      assert( grid>=0 && grid<cg.numberOfComponentGrids());

      MappedGrid & c = cg[grid];
      c.update(MappedGrid::THEvertexBoundaryNormal);  // fix this ********************
      OV_GET_SERIAL_ARRAY(real,c.vertex(),vertexLocal);
      OV_GET_SERIAL_ARRAY(real,stress[grid],stressLocal);
      OV_GET_SERIAL_ARRAY(int,c.mask(),maskLocal);
#ifdef USE_PPP
      realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
#else
      realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
#endif

      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extraInTangential);
      int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,Ib1,Ib2,Ib3,includeGhost);

      int ipar[] = {grid,side,axis,gf0.form}; // 
      real rpar[] = { gf0.t }; // 
      parameters.getNormalForce( gf0.u,stressLocal,ipar,rpar, includeViscosity );

      // torque:  (x-x0) X dF
      RealArray xCM(3);
      body[b]->getPosition(gf0.t,xCM);
      if( ok )
      {
	if( cg.numberOfDimensions()==2 )
	{
	  stressLocal(Ib1,Ib2,Ib3,torquec) = ( (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,1)-
				  	       (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,0) );
	}
	else
	{
	  stressLocal(Ib1,Ib2,Ib3,torquec  ) = ( (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,2)-
					         (vertexLocal(Ib1,Ib2,Ib3,2)-xCM(2))*stressLocal(Ib1,Ib2,Ib3,1) );

	  stressLocal(Ib1,Ib2,Ib3,torquec+1) = ( (vertexLocal(Ib1,Ib2,Ib3,2)-xCM(2))*stressLocal(Ib1,Ib2,Ib3,0)-
					         (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,2) );

	  stressLocal(Ib1,Ib2,Ib3,torquec+2) = ( (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,1)-
					         (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,0) );
	}
      
      } // end if OK
    } // end for face; compute stress*normal on a face

    // *wdh* 040920 : interpolate the stress to get values at the interpolation points needed for integration
    // -- do we need to worry about interior values of the stress and torque?? -- could assign nearby values to
    // equal those on the boundary
    stress.setOperators(*u.getOperators()); // operators needed for amr interpolation
    Interpolant & interpolant = *gf0.u.getInterpolant();

    interpolant.interpolate(stress); // **OPTIMZE ME -- only need to interpolant on boundaries ***

    //  ----- Integrate surface force & torque on rigid body --------
    Range F=numberOfStressComponents;
    Range T=numberOfTorqueComponents;
    Range FT=numberOfForceAndTorqueComponents;
    RealArray forceTorque(FT); // , x(3),r(3,3); 

    forceTorque=0.;
    integrate->surfaceIntegral( stress,FT,forceTorque,b );  // surface integral of forces and torques.

    // printF("--MVG:getForceRB: t=%9.3e torque=%9.3e\n",gf0.t,forceTorque(torquec));
    
    // --- save results in user supplied arrays ---
    force(F,b)=forceTorque(F);
    if( numberOfDimensions==2 )
      torque(2,b)=forceTorque(torquec);  // in 2D there is only one component of the torque
    else
      torque(T,b) = forceTorque(T+torquec);
    
    // --- Add external forces including gravity buoyancy force ---
    if( includeExternal )
    {
      // ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

      // get the gravity vector -- may be time dependent for a slow start
      real gravity[3];
      parameters.getGravityVector( gravity,gf0.t );

      // -- check if the gravity is non-zero --
      real maxGravity=0.;
      for( int d=0; d<cg.numberOfDimensions(); d++ )
	maxGravity=max(maxGravity,fabs(gravity[d]));
      
      if( maxGravity>0. ) // gravity is on 
      {
        
	const real bodyMass = body[b]->getMass();
	const real fluidMass = getFluidMassOfBody( b );

	const real dt = parameters.dbase.get<real >("dt");
	if( (debug() & 2) && gf0.t< 2.*dt ) 
	{
	  printF("--MVG-- getForceOnRigidBodies: body b=%i: bodyMass=%8.2e, fluidMass=%8.2e\n",
		 b,bodyMass,fluidMass);
	}
	
	// real fluidMass=0.;
	// const real fluidDensity = parameters.dbase.get<real >("fluidDensity");
    
	// if( fluidDensity!=0. )
	// {
	//   const real bodyDensity =  body[b]->getDensity();
	//   const real bodyVolume  =  body[b]->getVolume();

        //   if( bodyVolume<0. && bodyDensity>0. && bodyMass>0. )
	//   {
        //     // bodyVolume has not been set but body density and mass are known:
        //     bodyVolume= bodyMass/bodyDensity;
	//   }

	//   if( bodyVolume>0. )
	//   {
	//     fluidMass = fluidDensity*bodyVolume; 

	//     const real dt = parameters.dbase.get<real >("dt");
	//     if( (debug() & 2) && gf0.t< 2.*dt ) 
	//       printF("--MVG-- getForceOnRigidBodies: body b=%i: bodyMass=%8.2e, fluidMass=%8.2e, bodyVolume=%9.3e\n",
	// 	     b,bodyMass,fluidMass,bodyVolume);
	//   }
	//   else
	//   {
	//     printF("--MVG-- getForceOnRigidBodies::ERROR: The fluid density is not zero but the "
        //            "body volume is unknown for body b=%i.\n",b);
	//   }
	// }

	for( int d=0; d<cg.numberOfDimensions(); d++ )
	  force(d,b) += gravity[d]*(bodyMass-fluidMass);  // add weight: mass*g 
      }

       //add body force -QT
       RealArray bodyForce(3), bodyTorque(3);
       body[b]->getBodyForces( gf0.t,bodyForce,bodyTorque );
    
       force(F,b) +=bodyForce(F);
       if( numberOfDimensions==2 )
           torque(2,b)+=bodyTorque(torquec);  // in 2D there is only one component of the torque
       else
           torque(T,b)+=bodyTorque(T);

       if( (debug() & 2) && TRUE ) 
       {
       printF("--MVG-- getForceOnRigidBodies: body b=%i: bodyTorque(2)=%8.4e\n",
             b,bodyTorque(2));
       }

    } // end include external forces
    
  } // end for body 
  

  return 0;
}


int MovingGrids::
getGridVelocity( GridFunction & gf0, const real & tGV )
//=================================================================================
// /Description:
//    Determine the gridVelocity for this grid function (if tGV!=gf0.gridVelocityTime)
//    Get gf0.gridVelocity at time gf0.t
//
//=================================================================================
{
  // debug0=3;  // **************


  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  if( debug() & 2 ) fprintf(pDebugFile," MovingGrids::getGridVelocity movingGridProblem=%i\n",movingGridProblem);
  
  if( !movingGridProblem )
    return 0;
  
  // if( tGV == gf0.gridVelocityTime )

  if( fabs(tGV-gf0.gridVelocityTime) < REAL_EPSILON*(1.+ fabs(tGV)) )
  {
    gf0.gridVelocityTime=tGV;
    return 0;
  }
  
  assert( isInitialized );

  real t0 = gf0.t;
  Index I1,I2,I3;

  // ***********************************************
  // ** Velocity for rigid bodies        ***********
  // ***********************************************
  int grid;
  for( grid=0; grid<gf0.cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = gf0.cg[grid];
    
    if( debug() & 4 )
      printF(" MovingGrids::getGridVelocity grid=%i movingGrid=%i, moveOption=%i\n",
	     grid,movingGrid(grid),moveOption(grid));
    
    if( movingGrid(grid) )
    {
      // *wdh* 090809 -- changes for parallel --

      realArray & gridVelocityd = gf0.getGridVelocity(grid);
      const bool centerNeeded = ( moveOption(grid)==rotate || 
                                  moveOption(grid)==scale  ||
                                  moveOption(grid)==matrixMotion ||
                                  moveOption(grid)==rigidBody );
      if( centerNeeded )
      {
	c.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
      }
      
#ifdef USE_PPP
      realSerialArray gridVelocity; getLocalArrayWithGhostBoundaries(gridVelocityd,gridVelocity);
      realSerialArray vertex; if( centerNeeded ){ getLocalArrayWithGhostBoundaries(c.vertex(),vertex); } //  
#else
      realSerialArray & gridVelocity = gridVelocityd;
      realSerialArray & vertex = c.vertex();
#endif
	

      getIndex( c.dimension(),I1,I2,I3 );
      bool ok = ParallelUtility::getLocalArrayBounds(gridVelocityd,gridVelocity,I1,I2,I3,1);
      if( !ok ) continue;
      
      if( moveOption(grid)==matrixMotion )
      {
        // Matrix motion (new way for pre-defined rigid motions)

        // Here is how we compute the velocity:
	//   x = R(t)*x(0) + g(t)
	//   v = R'(t)*x(0) + g'(t)
	//     = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)

	// velocity    = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)
	// acceleration= R''(t)*R^{-1}( x(t)-g(t) ) + g''(t)

        const int b=int(moveParameters(0,grid)+.5);
        assert( b>=0 && b<numberOfMatrixMotionBodies );
        
	MatrixMotion & matMotion = *matrixMotionBody[b];
	
	RealArray rMatrix(4,4), rt(4,4);
	rMatrix=0.;
  
	// compute matrix R, and R'
	const int derivative= 1;  
	matMotion.getMotion( t0, rMatrix, rt, derivative);
	RealArray ri(4,4), rpri(4,4);
	// ri = inverse of rMatrix
	MatrixMapping::matrixInversion( ri,rMatrix );
	// rpri = rt*ri : 
	MatrixMapping::matrixMatrixProduct( rpri, rt, ri );
 

	if( c.numberOfDimensions()==2 )
	{
	  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	  {
	    // Here is the velocity in 2D
	    gridVelocity(I1,I2,I3,axis) = ( rpri(axis,0)*(vertex(I1,I2,I3,0) - rMatrix(0,3))+
					    rpri(axis,1)*(vertex(I1,I2,I3,1) - rMatrix(1,3)) ) + rt(axis,3);
	  }
	}
	else if( c.numberOfDimensions()==3 )
	{
	  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	  {
	    // Here is the velocity in 3D
	    gridVelocity(I1,I2,I3,axis) = ( rpri(axis,0)*(vertex(I1,I2,I3,0) - rMatrix(0,3))+
					    rpri(axis,1)*(vertex(I1,I2,I3,1) - rMatrix(1,3))+ 
					    rpri(axis,2)*(vertex(I1,I2,I3,2) - rMatrix(2,3)) ) + rt(axis,3);
	  }
	}
	else
	{
	  OV_ABORT("ERROR: numberOfDimenions");
	}

      }
      else if( moveOption(grid)==shift )
      {
        if( debug() & 2 )
	  fprintf(pDebugFile,"getGridVelocity for shift at t=%e\n",t0);
	const RealArray & vector = moveParameters(Range(0,2),grid); // shift vector
        const real speed = moveParameters(3,grid);
	for( int axis=axis1; axis<c.numberOfDimensions(); axis++ )
	  gridVelocity(I1,I2,I3,axis)=vector(axis)*speed;
      }
      else if(  moveOption(grid)==rotate )
      {
	const RealArray & x0 =moveParameters(Range(0,2),grid); // centre of rotation
	// tn has base 3 here: const RealArray & tn =moveParameters(Range(3,5),grid); // tangent to rotation axis
        RealArray tn(3); tn = moveParameters(Range(3,5),grid); // tangent to rotation axis
        const real rampInterval=moveParameters(7,grid);

        real ramp, rampSpeed, rampAcceleration ;
        getRamp(t0,rampInterval, ramp,rampSpeed,rampAcceleration );

        if( debug() & 2 )
	  fprintf(pDebugFile,"getGridVelocity for rotate at t=%e rampInterval=%e ramp=%e rampSpeed=%e\n",
		  t0,rampInterval,ramp,rampSpeed);

        // NOTE: theta(t) = 2.*Pi*omega*ramp(t)*t 

        const real omega=moveParameters(6,grid);
        const real angularSpeed = 2.*Pi*omega*( ramp+rampSpeed*t0 );
	
        if( debug() & 2 )
	  fprintf(pDebugFile,"getGridVelocity for rotate at t=%e omega=%e\n",t0,angularSpeed);

	// -- compute the grid velocity at time t0 --

        // ----------------------------------------------------------------------------------------------
	// Rotation: 
        //           x(t) = R(t) * x(0)
        // 
	//   R(t) = [ cos(theta)  -sin(theta) ]
	//          [ sin(theta)   cos(theta) ]
        //
        //   angularSpeed = d(theta)/dt
        //   angularAcceleration = d^2(theta)/dt^2
        //
        //   velocity = angularSpeed [ -sin(theta) -cos(theta) ][ x0 ]
        //                           [ cos(theta)  -sin(theta) ][ y0 ]
        //   velocity = angularSpeed [ -y(t) ]
        //                           [  x(t) ]
	//   acceleration = [ x_tt ] = - angularSpeed^2 [ x(t) ] + angularAcceleration [ -y(t) ]
        //                  [ y_tt ]                    [ y(t) ]                       [  x(t) ]
        //
        // ----------------------------------------------------------------------------------------------

        // *wdh* 090114 : this should be theta= 2.*Pi*omega*t0*ramp I think *** fix me

	// *wdh* real theta = angularSpeed*t0;   // note: compute the grid velocity at time t0
	real theta = 2.*Pi*omega*ramp*t0;   // note: compute the grid velocity at time t0
	real cost=cos(theta);
	real sint=sin(theta);
	real tcost=angularSpeed*cost;
	real tsint=angularSpeed*sint;
	Index I1,I2,I3;

	if( c.numberOfDimensions()==2 )
	{
	  gridVelocity(I1,I2,I3,axis1)=(-angularSpeed)*(vertex(I1,I2,I3,axis2)-x0(axis2));
	  gridVelocity(I1,I2,I3,axis2)=( angularSpeed)*(vertex(I1,I2,I3,axis1)-x0(axis1));
	  
	  if( debug() & 4 )
	  {
	    ::display(gridVelocity," getGridVelocity for rotate",pDebugFile,"%5.2f ");
	  }
	}
        else if( c.numberOfDimensions()==3 )
	{
	  if( tn(0)>0. && tn(1)==0. && tn(2)==0. )
	  {
	    // rotate about the x-axis
            //
            //             ^ y
            //             |
            //       z     |
            //        <----O x (out-ward)
            //
	    gridVelocity(I1,I2,I3,axis1)=0.;
	    gridVelocity(I1,I2,I3,axis2)=(-angularSpeed)*(vertex(I1,I2,I3,axis3)-x0(axis3));
	    gridVelocity(I1,I2,I3,axis3)=( angularSpeed)*(vertex(I1,I2,I3,axis2)-x0(axis2));
	  }
	  else if( tn(0)==0. && tn(1)>0. && tn(2)==0. )
	  {
	    // rotate about the y-axis
            //
            //             ^ x
            //             |
            //             |
            //           y O---> z 
            //
	    gridVelocity(I1,I2,I3,axis1)=( angularSpeed)*(vertex(I1,I2,I3,axis3)-x0(axis3));
	    gridVelocity(I1,I2,I3,axis2)=0.; 
	    gridVelocity(I1,I2,I3,axis3)=(-angularSpeed)*(vertex(I1,I2,I3,axis1)-x0(axis1));
	  }
	  else if( tn(0)==0. && tn(1)==0. && tn(2)>0. )
	  {
	    // rotate about the z-axis
            //             ^ y
            //             |
            //             |
            //           z O---> x
            //
	    gridVelocity(I1,I2,I3,axis1)=(-angularSpeed)*(vertex(I1,I2,I3,axis2)-x0(axis2));
	    gridVelocity(I1,I2,I3,axis2)=( angularSpeed)*(vertex(I1,I2,I3,axis1)-x0(axis1));
	    gridVelocity(I1,I2,I3,axis3)=0.;
	  }
	  else
	  {
	    printF("MovingGrids::getGridVelocity:ERROR: can only rotate about the positive x, y or z-axis for now,\n"
                   " tangent=(%8.2e,%8.2e,%8.2e)\n"
                   " rotation-point=(%8.2e,%8.2e,%8.2e)\n",tn(0),tn(1),tn(2),x0(0),x0(1),x0(2));
	    OV_ABORT("Error");
	  }
	  
	    

	}

      }
      else if( moveOption(grid)==scale )
      { 
        // scale grid like x(r,t) = (1+scale*t)*x(r,t=0)
        //  gridVelocity = scale*x(r,0)
        //               = scale*( x(r,t0)/(1+scale*t0) )
        //         
        if( debug() & 4 )
	  printF("moveGrids:scale: get gridVelocity for grid %i at t0=%9.3e, tGV=%9.3e\n",grid,t0,tGV);

	// compute the grid velocity
	assert(c.numberOfDimensions()==2 );
	for( int axis=0; axis<c.numberOfDimensions(); axis++ )
	{
          // Note that the vertex array holds the grid at time t0: 
          real scaleFactor = moveParameters(axis,grid);
          real factor = (scaleFactor/(1.+scaleFactor*t0));
	  gridVelocity(I1,I2,I3,axis)=factor*vertex(I1,I2,I3,axis);
	}
	
      }
      else if( moveOption(grid)==oscillate )
      {
	// x(t) = (1-cos([t-tOrigin]*omega)))*amplitude
	// x_t = omega*sin([t-tOrigin]*omega)*amplitude

	const RealArray & vector = moveParameters(Range(0,2),grid); // tangent
        const real omega = moveParameters(3,grid)*2.*Pi;        // oscillation rate
        const real amplitude =moveParameters(4,grid);                  // amplitude
        const real tOrigin =moveParameters(5,grid);                  
    
	for( int axis=axis1; axis<c.numberOfDimensions(); axis++ )
	  gridVelocity(I1,I2,I3,axis)=vector(axis)*omega*amplitude*sin(omega*(t0-tOrigin));

      }
      else if( moveOption(grid)==rigidBody )
      {
	// total velocity is
	//    v =    vCM + R' R^{-1} (x-xCM)

	RealArray xCM(3),vCM(3),rtri(3,3);
	int b=int(moveParameters(0,grid)+.5);
	assert( b>=0 && b<numberOfRigidBodies );

	body[b]->getPosition( t0,xCM  );
        body[b]->getVelocity( t0,vCM  );
	body[b]->getPointTransformationMatrix( t0,rtri ); // rtri <- R'R^{-1}

	if( debug() & 2 )
	{
	  RealArray w(3);
	  body[b]->getAngularVelocities( t0,w );
	  fprintf(parameters.dbase.get<FILE* >("moveFile"),">>>MovingGrids::getGridVelocity: t=%8.2e, v=(%8.3e,%8.3e,%8.3e) "
		  "w=(%6.2e,%6.2e,%6.2e) \n",t0,vCM(0),vCM(1),vCM(2),w(0),w(1),w(2));
	}

	for( int axis=axis1; axis<c.numberOfDimensions(); axis++ )
	  gridVelocity(I1,I2,I3,axis)=vCM(axis);

	if( c.numberOfDimensions()==2 )
	{
	  for( int axis=axis1; axis<c.numberOfDimensions(); axis++ )
	    gridVelocity(I1,I2,I3,axis)=vCM(axis)+(rtri(axis,0)*(vertex(I1,I2,I3,0)-xCM(0))+
						   rtri(axis,1)*(vertex(I1,I2,I3,1)-xCM(1)));
	}
	else 
	{
	  for( int axis=axis1; axis<c.numberOfDimensions(); axis++ )
	    gridVelocity(I1,I2,I3,axis)=vCM(axis)+(rtri(axis,0)*(vertex(I1,I2,I3,0)-xCM(0))+
						   rtri(axis,1)*(vertex(I1,I2,I3,1)-xCM(1))+ 
						   rtri(axis,2)*(vertex(I1,I2,I3,2)-xCM(2)));
	}

      }
      else if( deformingBody == moveOption(grid) )  
      {  
	// done below
      }
      else if( moveOption(grid)==userDefinedMovingGrid )
      {
        getUserDefinedGridVelocity( gf0,tGV,grid );
      }
      else
      {
        printF(" getGridVelocity: grid=%i, unknown or un-implemeted movingGridOption=%i\n",
	       grid,moveOption(grid));
	
        OV_ABORT("error");
      }
    } // end if (movingGrid(grid))
  } // end for (grid=0; ...

  // ***********************************************
  // ** Velocity for DeformingBodyGrids    *********
  // ***********************************************
  //.. note that 'ForBoundary' uses c = link to gf0.cg[grid]
#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<c.numberOfDimensions(); axis++ ) \
    for( side=0; side<=1; side++ )

  for( grid=0; grid < gf0.cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = gf0.cg[grid];
    //RealArray & gridVelocity = gf0.gridVelocity[grid];  OLD WAY **pf
    realArray & gridVelocity = gf0.getGridVelocity(grid);
    OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);
    

    if( movingGrid(grid) )
    {
      if ( deformingBody == moveOption(grid) ) 
      {
	int b=int(moveParameters(0,grid)+.5);  // deformingBodyNumber, each body may have several component grids.
	assert( b>=0 && b< numberOfDeformingBodies );

	int numThisBodyGrids=deformingBodyList[b]->getNumberOfGrids(); 
	assert(numThisBodyGrids!=0); // should have at least the present component='grid'
	// assert(numThisBodyGrids==1); // debug (only 1 grid):Full case (several grids) not impl. yet

	if( debug() & 2 )
          printF("++MOVING GRIDS(deformingBody): gridVelocity, grid = %i, def body = %i\n", grid, b);

	deformingBodyList[b]->getVelocity( tGV, grid, gf0.cg, gridVelocity  );

	
	// NOTE: It seems NB to make the grid-velocity on the boundary, match the value
        //       returned by deformingBodyList[b]->getVelocityBC  so that the advection boundary
        //       terms in the pressure BC will cancel:
        //         p.n = n.(  -rho*v_t + (v-g_t).grad(v) + nu*\Delta(v)
        // *wdh* 2014/06/26 -- turn this back on 
	if( true ) 
	{
          // -- For each grid face that is on the deforming body we get the grid velocity ---
          //       boundaryFaces(0:2,f) = (side,axis,grid) for face f 
	  Index Ib1,Ib2,Ib3;
	  Range Rx=c.numberOfDimensions();

          const IntegerArray & boundaryFaces = deformingBodyList[b]->getBoundaryFaces();
	  const int numberOfFaces = boundaryFaces.getLength(1);
          for( int f=0; f<numberOfFaces; f++ )
	  {
	    if( boundaryFaces(2,f)==grid ) // this face of the deforming body is on this grid 
	    {
	      int side=boundaryFaces(0,f), axis=boundaryFaces(1,f);
              assert( c.boundaryCondition(side,axis) > 0 );
	      
	      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      realSerialArray boundaryVelocity(Ib1,Ib2,Ib3, c.numberOfDimensions());
	      boundaryVelocity  =  0.;
	      deformingBodyList[b]->getVelocityBC( tGV, side,axis, grid, c, Ib1,Ib2,Ib3, boundaryVelocity  );

	      gridVelocityLocal(Ib1,Ib2,Ib3,Rx) = boundaryVelocity(Ib1,Ib2,Ib3,Rx); 

	    }
	  }
	  if( false )
	  {
	    // *** OLD WAY -- THIS IS WRONG -- checks too many faces ---

	    int side,axis;
	    Index Icoord(0,c.numberOfDimensions());
	    Index Ib1,Ib2,Ib3;
	    //Index Ig1,Ig2,Ig3;
	    //..find a physical boundary & set the velocity there,
	    ForBoundary(side,axis)   
	    { 
	      if( c.boundaryCondition()(side,axis) > 0  )    
	      {
		getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		realSerialArray boundaryVelocity(Ib1,Ib2,Ib3, c.numberOfDimensions());
		boundaryVelocity  =  0.;
		deformingBodyList[b]->getVelocityBC( tGV, side,axis,grid, c, Ib1,Ib2,Ib3, boundaryVelocity  );

		gridVelocityLocal(Ib1,Ib2,Ib3,Icoord) = boundaryVelocity(Ib1,Ib2,Ib3,Icoord);

	      }
	    }
	  } 

	}
	
        if( false ) // **** TEMP ********
	  ::display(gridVelocity,sPrintF("--MvG-- MovingGrids(deformingBody): gridVelocity, t=%8.2e",tGV),"%6.3f "); 



      } //end if deformingBody
    }
  } //end for -- deformingBodyGrids
#undef  ForBoundary

  gf0.gridVelocityTime=tGV; // *wdh* 000517
  return 0;
}

//==========================================================================================
/// \brief Add the grid acceleration in the normal direction, n.x_tt,  to the function f
///   f is normally the function that holds the rhs for the pressure eqn
///
///   BC: is p.n = ... - rho n^T( u.t )
///              = ... -rho n^T( g(x,t).tt )  where g is the position of the moving grid  
//===========================================================================================
int MovingGrids::
gridAccelerationBC(const int grid, const int side, const int axis,
		   const real t0,
		   MappedGrid & c,
		   realMappedGridFunction & u ,
		   realMappedGridFunction & f ,
		   realMappedGridFunction & gridVelocity ,
		   realSerialArray & normal,
		   const Index & J1,
		   const Index & J2,
		   const Index & J3,
		   const Index & J1g,
		   const Index & J2g,
		   const Index & J3g
  )
{
  if( !movingGrid(grid) )
    return 0;

  assert( isInitialized );

#ifdef USE_PPP
  realSerialArray vertex; getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
  realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
  realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
  // intSerialArray  maskLocal;   getLocalArrayWithGhostBoundaries(mask,maskLocal);
  Index I1=J1,   I2=J2,   I3=J3;
  Index I1g=J1g, I2g=J2g, I3g=J3g;
  int includeGhost=1;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1g,I2g,I3g,includeGhost);
  if( !ok ) return 0;

#else
  const Index &I1=J1,   &I2=J2,   &I3=J3;
  const Index &I1g=J1g, &I2g=J2g, &I3g=J3g;
  realArray & vertex = c.vertex();
  realArray & uLocal = u;
  realArray & fLocal = f;
  // intSerialArray & maskLocal = mask;
#endif


  if( moveOption(grid)==matrixMotion )
  {
    // Matrix motion (new way for pre-defined rigid motions)

    // Here is how we compute the velocity and acceleration: 
    //   x = R(t)*x(0) + g(t)
    //   v = R'(t)*x(0) + g'(t)
    //     = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)

    // velocity    = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)
    // acceleration= R''(t)*R^{-1}( x(t)-g(t) ) + g''(t)

    const int b=int(moveParameters(0,grid)+.5);
    assert( b>=0 && b<numberOfMatrixMotionBodies );
        
    MatrixMotion & matMotion = *matrixMotionBody[b];
	
    RealArray rMatrix(4,4), rt(4,4);
    rMatrix=0.;
  
    // compute matrix R, and R''
    const int derivative= 2;  
    matMotion.getMotion( t0, rMatrix, rt, derivative);
    RealArray ri(4,4), rpri(4,4);
    // ri = inverse of rMatrix
    MatrixMapping::matrixInversion( ri,rMatrix );
    // rpri = rt*ri : 
    MatrixMapping::matrixMatrixProduct( rpri, rt, ri );
 

#define matMotionAcceleration2d(I1,I2,I3,axis)				\
    ( ( rpri(axis,0)*(vertex(I1,I2,I3,0) - rMatrix(0,3))+		\
	rpri(axis,1)*(vertex(I1,I2,I3,1) - rMatrix(1,3)) ) + rt(axis,3) ) 
#define matMotionAcceleration3d(I1,I2,I3,axis)				\
    ( ( rpri(axis,0)*(vertex(I1,I2,I3,0) - rMatrix(0,3))+		\
	rpri(axis,1)*(vertex(I1,I2,I3,1) - rMatrix(1,3))+		\
	rpri(axis,2)*(vertex(I1,I2,I3,2) - rMatrix(2,3)) ) + rt(axis,3) )

    if( c.numberOfDimensions()==2 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(matMotionAcceleration2d(I1,I2,I3,0))+
			    normal(I1,I2,I3,1)*(matMotionAcceleration2d(I1,I2,I3,1)) );
    }
    else if( c.numberOfDimensions()==3 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(matMotionAcceleration3d(I1,I2,I3,0))+
			    normal(I1,I2,I3,1)*(matMotionAcceleration3d(I1,I2,I3,1))+
			    normal(I1,I2,I3,2)*(matMotionAcceleration3d(I1,I2,I3,2)) );
    }
    else
    {
      OV_ABORT("ERROR: numberOfDimenions");
    }

  }
  else if( moveOption(grid)==shift )
  { // do nothing
  }
  else if( moveOption(grid)==rotate )
  {
    if( debug() & 8 )
      printF("gridAccelerationBC for rotate at t=%12.4e\n",t0);

    const RealArray & x0 =moveParameters(Range(0,2)); // centre of rotation
    // tn has base 3 here: const RealArray & tn =moveParameters(Range(3,5),grid); // tangent to rotation axis
    RealArray tn(3); tn = moveParameters(Range(3,5),grid); // tangent to rotation axis
    const real rampInterval=moveParameters(7,grid);

    real ramp, rampSpeed, rampAcceleration ;
    getRamp(t0,rampInterval, ramp,rampSpeed,rampAcceleration );

    const real omega=moveParameters(6,grid);
    const real angularSpeed = 2.*Pi*omega*(ramp+rampSpeed*t0);
    const real angularAcceleration=2.*Pi*omega*(2.*rampSpeed+rampAcceleration*t0);
    
    // ----------------------------------------------------------------------------------------------
    // Rotation: 
    //           x(t) = R(t) * x(0)
    // 
    //   R(t) = [ cos(theta)  -sin(theta) ]
    //          [ sin(theta)   cos(theta) ]
    //
    //   angularSpeed = d(theta)/dt
    //   angularAcceleration = d^2(theta)/dt^2
    //
    //   velocity = angularSpeed [ -sin(theta) -cos(theta) ][ x0 ]
    //                           [ cos(theta)  -sin(theta) ][ y0 ]
    //   velocity = angularSpeed [ -y(t) ]
    //                           [  x(t) ]
    //   acceleration = [ x_tt ] = - angularSpeed^2 [ x(t) ] + angularAcceleration [ -y(t) ]
    //                  [ y_tt ]                    [ y(t) ]                       [  x(t) ]
    //
    // ----------------------------------------------------------------------------------------------
    if( c.numberOfDimensions()==2 )
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(x0(0)-vertex(I1,I2,I3,0))+  // x_tt = -(x-x0)
		            normal(I1,I2,I3,1)*(x0(1)-vertex(I1,I2,I3,1)))*(SQR(angularSpeed));
      if( angularAcceleration!=0. )
      {
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vertex(I1,I2,I3,1)-x0(1))+
			      normal(I1,I2,I3,1)*(x0(0)-vertex(I1,I2,I3,0)))*angularAcceleration;
      }
    }
    else 
    {
      if( tn(0)==1. && tn(1)==0. && tn(2)==0. )
      {
	// rotate about the x-axis
        //
        //             ^ y
        //             |
        //       z     |
        //        <----O x (out-ward)
        //
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,1)*(x0(1)-vertex(I1,I2,I3,1))+  // x_tt = -(x-x0)
			      normal(I1,I2,I3,2)*(x0(2)-vertex(I1,I2,I3,2)))*(SQR(angularSpeed));
	if( angularAcceleration!=0. )
	{
	  fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,1)*(vertex(I1,I2,I3,2)-x0(2))+   
				normal(I1,I2,I3,2)*(x0(1)-vertex(I1,I2,I3,1)))*angularAcceleration;
	}
      }
      else if( tn(0)==0. && tn(1)>0. && tn(2)==0. ) // *wdh* bug fixed 2013/10/06
      {
	// rotate about the y-axis
        //
        //             ^ x
        //             |
        //             |
        //           y O---> z 
        //
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,2)*(x0(2)-vertex(I1,I2,I3,2))+  // x_tt = -(x-x0)
			      normal(I1,I2,I3,0)*(x0(0)-vertex(I1,I2,I3,0)))*(SQR(angularSpeed));
	if( angularAcceleration!=0. )
	{
	  fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,2)*(vertex(I1,I2,I3,0)-x0(0))+ 
				normal(I1,I2,I3,0)*(x0(2)-vertex(I1,I2,I3,2)))*angularAcceleration;
	}
      }
      else if( tn(0)==0. && tn(1)==0. && tn(2)>0. )
      {
	// rotate about the z-axis
        //             ^ y
        //             |
        //             |
        //           z O---> x
        //
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(x0(0)-vertex(I1,I2,I3,0))+  // x_tt = -(x-x0)
			      normal(I1,I2,I3,1)*(x0(1)-vertex(I1,I2,I3,1)))*(SQR(angularSpeed));
	if( angularAcceleration!=0. )
	{
	  fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vertex(I1,I2,I3,1)-x0(1))+
				normal(I1,I2,I3,1)*(x0(0)-vertex(I1,I2,I3,0)))*angularAcceleration;
	}
      }
      else
      {
	printF("MovingGrids::gridAccelerationBC:ERROR: can only rotate about the positive x, y or z-axis for now\n"
	       " tangent=(%8.2e,%8.2e,%8.2e)\n"
	       " rotation-point=(%8.2e,%8.2e,%8.2e)\n",tn(0),tn(1),tn(2),x0(0),x0(1),x0(2));
	OV_ABORT("Error");
      }
      
    }
  }
  else if( moveOption(grid)==oscillate )
  {
    // x(t) = (1-cos([t-tOrigin]*omega)))*amplitude
    // x_t = omega*sin([t-tOrigin]*omega)*amplitude
    // x_tt = omega*omega*cos([t-tOrigin]*omega)*amplitude

    const RealArray & vector = moveParameters(Range(0,2),grid); // tangent
    const real omega = moveParameters(3,grid)*2.*Pi;        // oscillation rate
    const real amplitude =moveParameters(4,grid);                  // amplitude
    const real tOrigin =moveParameters(5,grid);                  

    const real factor=omega*omega*cos(omega*(t0-tOrigin))*amplitude;
    
    if( c.numberOfDimensions()==2 )
    {
      if( false )
	display(fLocal(I1g,I2g,I3g)," f before adding acceleration BC",
                parameters.dbase.get<FILE* >("pDebugFile"),"%6.2f ");

      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vector(0)*factor)+
		            normal(I1,I2,I3,1)*(vector(1)*factor) );
      if( false )
	display(fLocal(I1g,I2g,I3g)," f after adding acceleration BC",
                parameters.dbase.get<FILE* >("pDebugFile"),"%6.2f ");
      

    }
    else
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vector(0)*factor)+
		            normal(I1,I2,I3,1)*(vector(1)*factor)+
		            normal(I1,I2,I3,2)*(vector(2)*factor) );
    }
    
  }
  else if( moveOption(grid)==rigidBody )   
  {
  
    RealArray xCM(3),aCM(3),rttri(3,3);
    int b=int(moveParameters(0,grid)+.5);
    assert( b>=0 && b<numberOfRigidBodies );

    body[b]->getPosition( t0,xCM  );

    // -- these are done below now *wdh* June 4, 2016
    // body[b]->getAcceleration( t0,aCM  );
    // // total acceleration is
    // //    a =    aCM + R'' R^{-1} (x-xCM)
    // body[b]->getPointTransformationMatrix( t0,Overture::nullRealArray(),rttri ); 

    // directProjectionAddedMass: if true, we are using the direct projection scheme 
    const bool directProjectionAddedMass = body[b]->getDirectProjectionAddedMass();
    if( !directProjectionAddedMass )
    {
      // --- Normal case ---
      // --- not directProjection added-mass ---

      // total acceleration is
      //    a =    aCM + R'' R^{-1} (x-xCM)
      body[b]->getAcceleration( t0,aCM  );

      //  rttri =  R'' R^{-1} (x-xCM)
      body[b]->getPointTransformationMatrix( t0,Overture::nullRealArray(),rttri ); 

    }
    else
    {
      // Direct-projection AMP scheme: do NOT include the acceleration terms as these are 
      // incorporated into the pressure BC

      if( FALSE ) // TURNED OFF -- *wdh* Sept 25, 2016
      {
	// TEMPORARY FUDGE
	// RealArray bodyForce(3), bodyTorque(3);
	// body[b]->getBodyForces( t0,bodyForce,bodyTorque );

	real bodyMass = body[b]->getMass();

	// ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
	// get the gravity vector -- may be time dependent for a slow start
	real gravity[3];
	parameters.getGravityVector( gravity,t0 );

	// printF("--INS: Accel-BC: directProjectionAddedMass: exclude RB acceleration terms, t=%9.2e\n"
	//        "        bodyForce=(%9.2e,%9.2e,%9.2e), gravity=(%9.2e,%9.2e,%9.2e)\n",
	//        t0,bodyForce(0),bodyForce(1),bodyForce(2),gravity[0],gravity[1],gravity[2]);

	// for( int d=0; d<c.numberOfDimensions(); d++ )
	// 	aCM(d)=(bodyForce(d)+gravity[d]*(bodyMass-fluidMass))/bodyMass;  // remove pressure term from acceleration 

        const real fluidMass = getFluidMassOfBody( b );

	// real fluidMass=0.;
	// real fluidDensity =  parameters.dbase.get<real >("fluidDensity");
	// assert( fluidDensity!=0. );
	// if( body[b]->getDensity()>0. )
	// {
	//   assert( bodyMass!=0. );
	//   real volume = bodyMass/body[b]->getDensity();
	//   fluidMass = parameters.dbase.get<real >("fluidDensity")*volume; 

	//   const real dt = parameters.dbase.get<real >("dt");
	//   if( (debug() & 2) && t0< 2.*dt ) 
	//     printF(" --- body b=%i: bodyMass=%8.2e, fluidMass=%8.2e, volume=%9.3e\n",b,bodyMass,fluidMass,volume);

	// }
	// else
	// {
	//   printF("MovingGrids:ERROR: The fluid density is not zero but the body density is unknown.\n"
	// 	 " Thus the volume of the body b=%i cannot be computed! \n",b);
	// }

	if( debug() & 64 )
	{
	  RealArray omegaDot(3);
	  body[b]->getAngularAcceleration( t0, omegaDot  );
	  printF("--MVG-- Accel-BC: directProjectionAddedMass: exclude RB acceleration terms, t=%9.2e\n",t0);
	  printF("       BEFORE: body: aCM = (%9.2e,%9.2e,%9.2e) wDot = (%9.2e,%9.2e,%9.2e)\n", 
		 aCM(0),aCM(1),aCM(2), omegaDot(0),omegaDot(1),omegaDot(2));

	}
	
	for( int d=0; d<c.numberOfDimensions(); d++ )
	  aCM(d) = gravity[d]*(bodyMass-fluidMass)/bodyMass; 

	if( debug() & 8 ) 
	  printF("--MVG--  gravity=(%9.2e,%9.2e,%9.2e) fluidMass=%9.3e bodyMass=%9.3e Buoyancy=(%9.2e,%9.2e,%9.2e)\n",
		 gravity[0],gravity[1],gravity[2],fluidMass,bodyMass, aCM(0),aCM(1),aCM(2));
      
      } // end if true
      

      // In the direct-projection case we replace 
      //      rttri =  R'' R^{-1} (x-xCM) = [ omegaDot X] + [omega X][omega X]
      //    by
      //      rttri =  [omega X][omega X] = rDotRt*rDotRt
      // rDotRt = R' R^T = matrix representing [omega X ]
      // -- added June 4, 2016 *wdh*
      RealArray rDotRt(3,3);
      body[b]->getPointTransformationMatrix( t0,rDotRt ); 
      rttri = RigidBodyMotion::mult(rDotRt,rDotRt);  

      if( TRUE )
      {
        // --- do this for now --- (later: no need to evaluate RB values at all)
		aCM=0.;
		// rttri=0.;
      }
      
    } // end if direct projection
    
    
    if( parameters.dbase.get<bool>("printMovingBodyInfo") || debug() & 2 )
    {
      RealArray vCM(3),w(3),omegaDot(3);
      body[b]->getVelocity( t0,vCM  );
      body[b]->getAngularVelocities( t0,w );
      body[b]->getAngularAcceleration( t0, omegaDot  );
      if( c.numberOfDimensions()==2 )
	printF("--MVG--::gridAccelBC: t0=%9.2e, xCM=(%9.2e,%9.2e), vCM=(%9.2e,%9.2e), aCM=(%9.2e,%9.2e), "
               "wCM=(0,0,%9.2e),wDot=(0,0,%9.2e)\n",
	       t0,xCM(0),xCM(1),vCM(0),vCM(1),aCM(0),aCM(1),w(2),omegaDot(2));
      else
	printF("--MVG--::gridAccelBC: t0=%9.2e, vCM=(%9.2e,%9.2e,%9.2e), aCM=(%9.2e,%9.2e,%9.2e), wCM=(%9.2e,%9.2e,%9.2e) \n",
	       t0,vCM(0),vCM(1),vCM(2),aCM(0),aCM(1),aCM(2),w(0),w(1),w(2));
    }
        

    if( c.numberOfDimensions()==2 )
    {
      // For testing -- turn off nu*Delta v 
      // if( TRUE ) 
      // {
      // 	fLocal(I1g,I2g,I3g)=0.; // ZERO OUT nu*Delta(v) term in BC  
      // }

      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
							 rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))))+
			    normal(I1,I2,I3,1)*(aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
						         rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1)))));
      
      if( debug() & 8 )
      {
	RealArray abc(I1,I2,I3);
	abc=(normal(I1,I2,I3,0)*(aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
			 			         rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))))+
                            normal(I1,I2,I3,1)*(aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
						         rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1)))));
	::display(abc,"--MVG-- boundary accel from Rigid Body","%9.2e ");
	::display(fLocal(I1g,I2g,I3g),"--MVG-- boundary accel total","%9.2e ");
      }

    }
    else 
    {
      fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
						         rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))+
							 rttri(0,2)*(vertex(I1,I2,I3,2)-xCM(2))))+
			    normal(I1,I2,I3,1)*(aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
							 rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
							 rttri(1,2)*(vertex(I1,I2,I3,2)-xCM(2))))+ 
			    normal(I1,I2,I3,2)*(aCM(2) +(rttri(2,0)*(vertex(I1,I2,I3,0)-xCM(0))+
							 rttri(2,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
							 rttri(2,2)*(vertex(I1,I2,I3,2)-xCM(2)))));
    }
  }
  else if( moveOption(grid)==deformingBody )    // gridAccelerationBC for deformingBodies
  {
    // **NOTE** 2014/07/01 -- there is a new way to get the correct deforming body number from the boundaryData

    int b=int(moveParameters(0,grid)+.5);  // deformingBodyNumber
    assert( b>=0 && b< numberOfDeformingBodies );
    
    int numThisBodyGrids=deformingBodyList[b]->getNumberOfGrids(); 
    assert(numThisBodyGrids!=0); // should have at least the present component='grid'

    // *** July 3, 2014 : FIXED -- NOT ALL faces lie on the deforming body surface --
    const IntegerArray & boundaryFaces = deformingBodyList[b]->getBoundaryFaces();
    const int numberOfFaces = boundaryFaces.getLength(1);
    for( int f=0; f<numberOfFaces; f++ ) 
    {
      if( boundaryFaces(2,f)==grid  && // this face of the deforming body is on this grid 
          boundaryFaces(0,f)==side  &&
          boundaryFaces(1,f)==axis )
      {

	if( debug() & 2 )     
	  printF("++MOVING GRIDS(deformingBody): gridAccelerationBC, grid = %i, def body = %i\n", grid, b);

	// *wdh* getAccelerationBC should return the acceleration of the boundary on the boundary nodes,
	// I1,I2,I3 from : getBoundaryIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3);     // boundary line    
	realSerialArray boundaryAcceleration(I1,I2,I3, c.numberOfDimensions());
	boundaryAcceleration  =  0.;
	deformingBodyList[b]->getAccelerationBC( t0, grid, side, axis, c ,I1,I2,I3, boundaryAcceleration  );

	if( false )
	  ::display(boundaryAcceleration(I1,I2,I3,Range(0,1)),sPrintF("--MVG-- bcAcceleration t=%g ",t0),"%9.2e ");

	if( c.numberOfDimensions()==2 ) 
	{
	  fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*boundaryAcceleration(I1,I2,I3,0)+ 
				normal(I1,I2,I3,1)*boundaryAcceleration(I1,I2,I3,1));

	  // fLocal(I1g,I2g,I3g)-=boundaryAcceleration(I1,I2,I3,1);   // ********** TEST

	  if( false )
	    ::display(fLocal(I1g,I2g,I3g),sPrintF("--MVG-- f = f - n.boundaryAcceleration (rhs for pressure) t=%g",t0),"%9.2e ");

	  //cout << "@@@ NORMAL ACCELERATION AT THE BOUNDARY!!\n";
	  //dd.display( normal(I1,I2,I3,0)*boundaryAcceleration(I1,I2,I3,0)+ 
	  //		       normal(I1,I2,I3,1)*boundaryAcceleration(I1,I2,I3,1) );
	}
	else 
	{
	  fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*boundaryAcceleration(I1,I2,I3,0)+ 
				normal(I1,I2,I3,1)*boundaryAcceleration(I1,I2,I3,1)+
				normal(I1,I2,I3,1)*boundaryAcceleration(I1,I2,I3,2));
	}  
      }
    }
    
  }
  else if( moveOption(grid)==userDefinedMovingGrid ) 
  {
    userDefinedGridAccelerationBC(grid,t0,c,u,f,gridVelocity,normal,I1,I2,I3,I1g,I2g,I3g);
  }
  else
  {
    printF("gridAccelerationBC:ERROR: unknown movingGridOption = %i\n",(int)moveOption(grid));
  }
  
  return 0; 
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{getBoundaryAcceleration}} 
int MovingGrids::
getBoundaryAcceleration( MappedGrid & c, realSerialArray & gtt, int grid, real t0, int option )
// ============================================================================================
// /Description: provide the acceleration (or other time derivative) of the boundary. (This routine
//  is called by cnsBC and insBC4.bC).
// 
//
// /gtt (output) : the acceleration of the boundary, g'', (if option==0). 
// /grid (input) : which grid
// /t0 (input) : time to evaluate the acceleration. 
// /option (input): if option==2^2 then return g'' the second time derivative of the boundary motion. If 
//   option==2^3 then return g''', the third time derivative of the boundary motion. If option=2^2+2^3 then
// return both g'' and g''' ( consecutively in the array gtt).
// /Author: wdh: 050507
//\end{MovingGridsSolverInclude.tex} 
// ============================================================================================
{
  if( !movingGrid(grid) )
    return 0;

  assert( isInitialized );

  const bool computeTwoTimeDerivatives  = (option/4) % 2;
  const bool computeThreeTimeDerivatives= (option/8) % 2;
  assert( computeTwoTimeDerivatives | computeThreeTimeDerivatives );
  
  Range R2 = c.numberOfDimensions();  // put gtt in these components
  Range R3 = c.numberOfDimensions();  // put gttt in these components
  if( computeTwoTimeDerivatives )
    R3=R2+c.numberOfDimensions();
  
  #ifdef USE_PPP
    realSerialArray vertex; getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
  #else
    const realSerialArray & vertex = c.vertex();
  #endif

  #ifndef USE_PPP
    const IntegerArray & boundaryCondition = c.boundaryCondition();
  #else
    // -- in parallel we need the "local" boundary conditions for this processor --
    IntegerArray boundaryCondition(2,3);
    ParallelGridUtility::getLocalBoundaryConditions( c.vertex(),boundaryCondition );
  #endif


  for( int axis=0; axis<c.numberOfDimensions(); axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( boundaryCondition(side,axis)>0 )
      {
        Index I1,I2,I3;
	getBoundaryIndex(c.gridIndexRange(),side,axis,I1,I2,I3);
        #ifdef USE_PPP
	  int includeGhost=1;
	  bool ok = ParallelUtility::getLocalArrayBounds(c.vertex(),vertex,I1,I2,I3,includeGhost);
	  assert (ok );  // this should be true if the local BC > 0 
        #endif

	if( moveOption(grid)==matrixMotion )
	{
	  // Matrix motion (new way for pre-defined rigid motions)

	  // Here is how we compute the velocity and acceleration: 
	  //   x = R(t)*x(0) + g(t)
	  //   v = R'(t)*x(0) + g'(t)
	  //     = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)

	  // velocity    = R'(t)*R^{-1}( x(t)-g(t) ) + g'(t)
	  // acceleration= R''(t)*R^{-1}( x(t)-g(t) ) + g''(t)

	  const int b=int(moveParameters(0,grid)+.5);
	  assert( b>=0 && b<numberOfMatrixMotionBodies );
        
	  MatrixMotion & matMotion = *matrixMotionBody[b];
	
	  RealArray rMatrix(4,4), rt(4,4);
	  rMatrix=0.;
  
	  // compute matrix R, and R''
	  const int derivative= 2;  
	  matMotion.getMotion( t0, rMatrix, rt, derivative);
	  RealArray ri(4,4), rpri(4,4);
	  // ri = inverse of rMatrix
	  MatrixMapping::matrixInversion( ri,rMatrix );
	  // rpri = rt*ri : 
	  MatrixMapping::matrixMatrixProduct( rpri, rt, ri );
 
	  if( computeTwoTimeDerivatives )
	  {
	    if( c.numberOfDimensions()==2 )
	    {
	      gtt(I1,I2,I3,0)=matMotionAcceleration2d(I1,I2,I3,0);
	      gtt(I1,I2,I3,1)=matMotionAcceleration2d(I1,I2,I3,1);
	    }
	    else
	    {
	      gtt(I1,I2,I3,0)=matMotionAcceleration3d(I1,I2,I3,0);
	      gtt(I1,I2,I3,1)=matMotionAcceleration3d(I1,I2,I3,1);
	      gtt(I1,I2,I3,2)=matMotionAcceleration3d(I1,I2,I3,2);
	    }
	      
	  }
	  if( computeThreeTimeDerivatives )
	  {
	    // finish this, not hard
	    OV_ABORT("MovingGrids::getBoundaryAcceleration:ERROR:computeThreeTimeDerivatives:  finish this Bill!");
	  }


	}
	else if( moveOption(grid)==shift )
	{ // do nothing
          Range R=c.numberOfDimensions();
          if( computeTwoTimeDerivatives ) 
            gtt(I1,I2,I3,R2)=0.;
          if( computeThreeTimeDerivatives )
             gtt(I1,I2,I3,R3)=0.;
	}
	else if( moveOption(grid)==rotate )
	{
	  if( debug() & 8 )
	    printF("MovingGrids::getBoundaryAcceleration for rotate at t=%e\n",t0);

	  const RealArray & x0 =moveParameters(Range(0,2)); // centre of rotation
          // tn has base 3 here: const RealArray & tn =moveParameters(Range(3,5),grid); // tangent to rotation axis
          RealArray tn(3); tn = moveParameters(Range(3,5),grid); // tangent to rotation axis
	  const real rampInterval=moveParameters(7,grid);

	  real ramp, rampSpeed, rampAcceleration ;
	  getRamp(t0,rampInterval, ramp,rampSpeed,rampAcceleration );

	  const real omega=moveParameters(6,grid);
	  const real angularSpeed = 2.*Pi*omega*(ramp+rampSpeed*t0);
	  const real angularAcceleration=2.*Pi*omega*(2.*rampSpeed+rampAcceleration*t0);
    
	  // ----------------------------------------------------------------------------------------------
	  // Rotation: 
	  //           x(t) = R(t) * x(0)
	  // 
	  //   R(t) = [ cos(theta)  -sin(theta) ]
	  //          [ sin(theta)   cos(theta) ]
	  //
	  //   angularSpeed = d(theta)/dt
	  //   angularAcceleration = d^2(theta)/dt^2
	  //
	  //   velocity = angularSpeed [ -sin(theta) -cos(theta) ][ x0 ]
	  //                           [ cos(theta)  -sin(theta) ][ y0 ]
	  //   velocity = angularSpeed [ -y(t) ]
	  //                           [  x(t) ]
	  //   acceleration = [ x_tt ] = - angularSpeed^2 [ x(t) ] + angularAcceleration [ -y(t) ]
	  //                  [ y_tt ]                    [ y(t) ]                       [  x(t) ]
	  //
	  // ----------------------------------------------------------------------------------------------
	  if( c.numberOfDimensions()==2 )
	  {
	    assert( tn(0)==0. && tn(1)==0. );

            if( computeTwoTimeDerivatives )
	    {
	      gtt(I1,I2,I3,0)=(x0(0)-vertex(I1,I2,I3,0))*(SQR(angularSpeed)); // x_tt = -(x-x0)
	      gtt(I1,I2,I3,1)=(x0(1)-vertex(I1,I2,I3,1))*(SQR(angularSpeed)); 
	      if( angularAcceleration!=0. )
	      {
		gtt(I1,I2,I3,0)+= (vertex(I1,I2,I3,1)-x0(1))*angularAcceleration;
		gtt(I1,I2,I3,1)+= (x0(0)-vertex(I1,I2,I3,0))*angularAcceleration;
	      }
	    }
	    if( computeThreeTimeDerivatives )
	    {
	      // finish this
              Overture::abort("ERROR:computeThreeTimeDerivatives:  finish this Bill!");
	    }
	  }
	  else 
	  {
	    if( tn(0)==1. && tn(1)==0. && tn(2)==0. )
	    {
	      // rotate about the x-axis
	      //
	      //             ^ y
	      //             |
	      //       z     |
	      //        <----O x (out-ward)
	      //
	      if( computeTwoTimeDerivatives )
	      {
		gtt(I1,I2,I3,0)=0.;
		gtt(I1,I2,I3,1)=(x0(1)-vertex(I1,I2,I3,1))*(SQR(angularSpeed)); // x_tt = -(x-x0)
		gtt(I1,I2,I3,2)=(x0(2)-vertex(I1,I2,I3,2))*(SQR(angularSpeed)); 
		if( angularAcceleration!=0. )
		{
		  gtt(I1,I2,I3,1)+=(vertex(I1,I2,I3,2)-x0(2))*angularAcceleration;  
		  gtt(I1,I2,I3,2)+=(x0(1)-vertex(I1,I2,I3,1))*angularAcceleration;
		}
	      }
	      if( computeThreeTimeDerivatives )
	      {
		// finish this
		OV_ABORT("ERROR:computeThreeTimeDerivatives:  finish this Bill!");
	      }

	    }
	    else if( tn(0)==0. && tn(1)>0. && tn(2)==0. ) // *wdh* 2013/10/06 
	    {
	      // rotate about the y-axis
	      //
	      //             ^ x
	      //             |
	      //             |
	      //           y O---> z 
	      //
	      if( computeTwoTimeDerivatives )
	      {
		gtt(I1,I2,I3,0)=(x0(0)-vertex(I1,I2,I3,0))*(SQR(angularSpeed));      // x_tt = -(x-x0)
		gtt(I1,I2,I3,1)=0.;
		gtt(I1,I2,I3,2)=(x0(2)-vertex(I1,I2,I3,2))*(SQR(angularSpeed)); 
		if( angularAcceleration!=0. )
		{
		  gtt(I1,I2,I3,2)+=(vertex(I1,I2,I3,0)-x0(0))*angularAcceleration;   
		  gtt(I1,I2,I3,0)+=(x0(2)-vertex(I1,I2,I3,2))*angularAcceleration;
		}
	      }
	      if( computeThreeTimeDerivatives )
	      {
		// finish this
		OV_ABORT("ERROR:computeThreeTimeDerivatives:  finish this Bill!");
	      }
	    }
	    else if( tn(0)==0. && tn(1)==0. && tn(2)>0. )
	    {
	      // rotate about the z-axis
	      //             ^ y
	      //             |
	      //             |
	      //           z O---> x
	      //
	      if( computeTwoTimeDerivatives )
	      {
		gtt(I1,I2,I3,0)=(x0(0)-vertex(I1,I2,I3,0))*(SQR(angularSpeed)); // x_tt = -(x-x0)
		gtt(I1,I2,I3,1)=(x0(1)-vertex(I1,I2,I3,1))*(SQR(angularSpeed)); 
		gtt(I1,I2,I3,2)=0.;
		if( angularAcceleration!=0. )
		{
		  gtt(I1,I2,I3,0)+=(vertex(I1,I2,I3,1)-x0(1))*angularAcceleration;
		  gtt(I1,I2,I3,1)+=(x0(0)-vertex(I1,I2,I3,0))*angularAcceleration;
		}
	      }
	      if( computeThreeTimeDerivatives )
	      {
		// finish this
		OV_ABORT("ERROR:computeThreeTimeDerivatives:  finish this Bill!");
	      }

	    }
	    else
	    {
	      printF("MovingGrids::getBoundaryAcceleration:ERROR: can only rotate about the positive x, y or z-axis for now\n"
		     " tangent=(%8.2e,%8.2e,%8.2e)\n"
		     " rotation-point=(%8.2e,%8.2e,%8.2e)\n",tn(0),tn(1),tn(2),x0(0),x0(1),x0(2));
	      OV_ABORT("Error");
	    }

	  }
	  
	}
	else if( moveOption(grid)==oscillate )
	{
	  // x(t) = (1-cos([t-tOrigin]*omega)))*amplitude
	  // x_t = omega*sin([t-tOrigin]*omega)*amplitude
	  // x_tt = omega*omega*cos([t-tOrigin]*omega)*amplitude

	  const RealArray & vector = moveParameters(Range(0,2),grid); // tangent
	  const real omega = moveParameters(3,grid)*2.*Pi;        // oscillation rate
	  const real amplitude =moveParameters(4,grid);                  // amplitude
	  const real tOrigin =moveParameters(5,grid);                  

	  if( computeTwoTimeDerivatives )
	  {
  	    const real factor=omega*omega*cos(omega*(t0-tOrigin))*amplitude;
            for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	    {
	      gtt(I1,I2,I3,dir)=vector(dir)*factor;
	    }
	  }
	  if( computeThreeTimeDerivatives )
	  {
  	    const real factor=-omega*omega*omega*sin(omega*(t0-tOrigin))*amplitude;
            for( int dir=0; dir<c.numberOfDimensions(); dir++ )
	    {
	      gtt(I1,I2,I3,dir)=vector(dir)*factor;
	    }
	  }
	  
// 	  if( c.numberOfDimensions()==2 )
// 	  {
// // 	    f(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vector(0)*factor)+
// // 			     normal(I1,I2,I3,1)*(vector(1)*factor) );

// 	    gtt(I1,I2,I3,0)=vector(0)*factor;
// 	    gtt(I1,I2,I3,1)=vector(1)*factor; 
// 	  }
// 	  else
// 	  {
// // 	    f(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(vector(0)*factor)+
// // 			     normal(I1,I2,I3,1)*(vector(1)*factor)+
// // 			     normal(I1,I2,I3,2)*(vector(2)*factor) );
// 	    gtt(I1,I2,I3,0)=vector(0)*factor;
// 	    gtt(I1,I2,I3,1)=vector(1)*factor; 
// 	    gtt(I1,I2,I3,2)=vector(2)*factor; 

// 	  }
    
	}
	else if( moveOption(grid)==rigidBody )   
	{
  
	  RealArray xCM(3),aCM(3),rttri(3,3);
	  int b=int(moveParameters(0,grid)+.5);
	  assert( b>=0 && b<numberOfRigidBodies );

	  body[b]->getPosition( t0,xCM  );
	  body[b]->getAcceleration( t0,aCM  );
	  // total acceleration is
	  //    a =    aCM + R'' R^{-1} (x-xCM)
	  body[b]->getPointTransformationMatrix( t0,Overture::nullRealArray(),rttri ); 


	  if( debug() & 2 ) // ***** TEMP
	  {
	    RealArray w(3);
	    body[b]->getAngularVelocities( t0,w );
	    printF(" MovingGrids::getBoundaryAcceleration: aCM=(%8.2e,%8.2e,%8.2e), w=(%6.2e,%6.2e,%6.2e) \n",
                   aCM(0),aCM(1),aCM(2), w(0),w(1),w(2));
	  }
	  if( false )
	  {
            printF("******** MovingGrids::getBoundaryAcceleration: Setting boundary acceleration to ZERO ********\n"); // ************
	    aCM=0.;
	  }


	  if( computeTwoTimeDerivatives )
	  {
	    if( c.numberOfDimensions()==2 )
	    {
// 	    f(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
// 							  rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))))+
// 			     normal(I1,I2,I3,1)*(aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
// 							  rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1)))));
	      gtt(I1,I2,I3,0)=aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
				       rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1)));
	      gtt(I1,I2,I3,1)=aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
				       rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1)));
	    }
	    else 
	    {
// 	    f(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*(aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
// 							  rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))+
// 							  rttri(0,2)*(vertex(I1,I2,I3,2)-xCM(2))))+
// 			     normal(I1,I2,I3,1)*(aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
// 							  rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
// 							  rttri(1,2)*(vertex(I1,I2,I3,2)-xCM(2))))+ 
// 			     normal(I1,I2,I3,2)*(aCM(2) +(rttri(2,0)*(vertex(I1,I2,I3,0)-xCM(0))+
// 							  rttri(2,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
// 							  rttri(2,2)*(vertex(I1,I2,I3,2)-xCM(2)))));
	      gtt(I1,I2,I3,0)=aCM(0) +(rttri(0,0)*(vertex(I1,I2,I3,0)-xCM(0))+
				       rttri(0,1)*(vertex(I1,I2,I3,1)-xCM(1))+
				       rttri(0,2)*(vertex(I1,I2,I3,2)-xCM(2)));
	    
	      gtt(I1,I2,I3,1)=aCM(1) +(rttri(1,0)*(vertex(I1,I2,I3,0)-xCM(0))+
				       rttri(1,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
				       rttri(1,2)*(vertex(I1,I2,I3,2)-xCM(2)));
	
	      gtt(I1,I2,I3,2)=aCM(2) +(rttri(2,0)*(vertex(I1,I2,I3,0)-xCM(0))+
				       rttri(2,1)*(vertex(I1,I2,I3,1)-xCM(1))+   
				       rttri(2,2)*(vertex(I1,I2,I3,2)-xCM(2)));
	
	    }
	  }
	  if( computeThreeTimeDerivatives )
	  {
	    // finish this
	    Overture::abort("ERROR:  finish this Bill!");
	  }	  

	}
	else if( deformingBody == moveOption(grid) )    // gridAccelerationBC for deformingBodies
	{
	  int b=int(moveParameters(0,grid)+.5);  // deformingBodyNumber
	  assert( b>=0 && b< numberOfDeformingBodies );
    
	  int numThisBodyGrids=deformingBodyList[b]->getNumberOfGrids(); 
	  assert(numThisBodyGrids!=0); // should have at least the present component='grid'
	  // assert(numThisBodyGrids==1); // Full case (several grids) not impl. yet
	  if( debug() & 2 )     
	    printF("MovingGrids::getBoundaryAcceleration: deforming, grid = %i, def body = %i\n", grid, b);

	  if( computeTwoTimeDerivatives )
	  {
            deformingBodyList[b]->getAccelerationBC( t0, grid, side, axis, c, I1,I2,I3, gtt );
	  }
	  if( computeThreeTimeDerivatives )
	  {
	    // finish this
	    Overture::abort("ERROR:  finish this Bill!");
	  }	  

	}
	else if( moveOption(grid)==userDefinedMovingGrid )   
        {
	  getUserDefinedBoundaryAcceleration( c,gtt,grid,t0,option,side,axis );
	}
	else
	  cout << "MovingGrids::getBoundaryAcceleration: unknown movingGridOption = " << moveOption(grid) << endl;
	
      } // end if bc>0
    } // end side
  } // end axis
      
  return 0; 

}



//  computeSurfaceStress::
//  ----------------------
//   * for RigidBodyMotion() & deformingBodyMotion()
//   * Compute viscous stress at the surf.
//
//
//void MovingGrids::
//computeSurfaceStress(const real & t1, 
//		const real & t2, 
//		const real & t3,
//		const real & dt0,
//		GridFunction & cgf1,  
//		GridFunction & cgf2,
//		GridFunction & cgf3 )
//{
//
//
//}


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{rigidBodyMotion}} 
int MovingGrids::
rigidBodyMotion(const real & t1, 
		const real & t2, 
		const real & t3,
		const real & dt0,
		GridFunction & cgf1,  
		GridFunction & cgf2,
		GridFunction & cgf3 )
// =========================================================================================
// /Description:
//    Determine rigid body motion. Compute the forces and moments on rigid bodies and integrate
// the equations of motion for the bodies up to time t3.
// 
// /Notes:
// The rigid body is defined by 
//    M = total mass
//    xCM(0) = inital position of the centre of mass
//    I1,I2,I3 : moments of inertia
//    3 principal axes of inertia
//    
//  We compute the 6 degrees of freedom, xCM_i(t) and omega_i(t),
//    omega_i(t) : angular velocity of the i'th axis of inertial.
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  
  // compute the force and torques on the bodies -- we need pressure and stresses ***

  // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
  correctionHasConverged=true;
  maximumRelativeCorrection=0.;

  if( numberOfRigidBodies==0 )
    return 0;
  
  assert( isInitialized );

  const bool isCorrectionStep = !(t3>t1);
  // printF("!!! MovingGrids: t1=%9.3e, t2=%9.3e, t3=%9.3e, isCorrectionStep=%i\n",t1,t2,t3,isCorrectionStep);

  RealCompositeGridFunction & u = cgf2.u;
  CompositeGrid & cg = cgf2.cg;
  const int numberOfDimensions = cg.numberOfDimensions();

  // *wdh* 120113 -- save stress and torque in the same grid function!
  Range all;
  const int numberOfStressComponents=numberOfDimensions;
  const int numberOfTorqueComponents= numberOfDimensions==2 ? 1 : 3;
  const int numberOfForceAndTorqueComponents=numberOfStressComponents+numberOfTorqueComponents;
  const int torquec =numberOfStressComponents; // first Torque component sits here 
  RealCompositeGridFunction stress(cg,all,all,all,numberOfForceAndTorqueComponents); // **** fix this ****
  // stress=0.; // do we need this?

  // Save added mass matrix entries here: **FIX ME : just create smaller TEMP space **
  // NOTE: we can NOT save these coefficients in interior points of the stress since we INTERPOLATE the stress
  // RealCompositeGridFunction amm(cg,all,all,all,numberOfForceAndTorqueComponents); // **** fix this ****
  // amm=0.; 

  // Save boundary data for added mass matrices here:
  ListOfRealArray addedMassMatrixList;  
  // numberOfAddedMassMatrixEntries : total number of entries we need to compute through surface integrals
  const int numberOfAddedMassMatrixEntries=numberOfDimensions==2 ? 6 : 12;  // FIX ME FOR 3D
  
  const int rc=parameters.dbase.get<int >("rc");
  const int uc=parameters.dbase.get<int >("uc");
  const int vc=parameters.dbase.get<int >("vc");
  const int wc=parameters.dbase.get<int >("wc");
  const int pc=parameters.dbase.get<int >("pc");
  const int tc=parameters.dbase.get<int >("tc");
  const real nu = parameters.dbase.get<real >("nu");
  const Range V(parameters.dbase.get<int >("uc"),parameters.dbase.get<int >("uc")+cg.numberOfDimensions()-1);

  // Do this here so we assign stress on the AMR grids too.
  integrate->useAdaptiveMeshRefinementGrids(true);
  integrate->updateForAMR(cg);   // ************************ only do after regrid *******

  Index Ib1,Ib2,Ib3; 
  const int extraInTangential=2;  // *wdh* 2012/02/25 make sure we assign enough values for impedance

  int b;
  for( b=0; b<numberOfRigidBodies; b++ )
  {
    assert( integrate!=NULL );

    real pMax=0.,fMax=0., gMax=0.;

    const bool useAddedMass = body[b]->useAddedMass();
    int numberOfAddedMassComponents = numberOfDimensions==2 ? 10 : 21;

    real zFluid=1.;  // fluid impedance -- do this for now --
    
    // BEGIN: compute stress*normal 
    const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
    // printF(">>>MovingGrids:: body=%i, numberOfFaces=%i\n",b,numberOfFaces);
    for( int face=0; face<numberOfFaces; face++ )
    {
      int side=-1,axis,grid;
      integrate->getFace(b,face,side,axis,grid);
      assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
      assert( grid>=0 && grid<cg.numberOfComponentGrids());


      // **** check refinement level grids too ***
      // The integrate knows which refinement grids to check --- add an access function


      if( debug() & 2 ) 
	printF("\nMovingGrids::rigidBodyMotion: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
	       b,face,side,axis,grid);

      MappedGrid & c = cg[grid];
      c.update(MappedGrid::THEvertexBoundaryNormal);  // fix this ********************

      // c.update(MappedGrid::THEvertex | MappedGrid::THEcenter);  //  ********************  2012/03/06 *************** TRY THIS
      
      const intArray & mask = c.mask();
      // OV_GET_SERIAL_ARRAY(real,amm[grid],ammLocal) 

      OV_GET_VERTEX_BOUNDARY_NORMAL(c,side,axis,normalLocal);
      OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
      OV_GET_SERIAL_ARRAY(real,c.vertex(),vertexLocal);

      OV_GET_SERIAL_ARRAY(real,stress[grid],stressLocal);
      stressLocal=0.;  // Is this needed? 

      // #ifdef USE_PPP
      //   realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
      //   realSerialArray vertexLocal; getLocalArrayWithGhostBoundaries(c.vertex(),vertexLocal);
      //   realSerialArray stressLocal; getLocalArrayWithGhostBoundaries(stress[grid],stressLocal);
      //   intSerialArray  maskLocal;   getLocalArrayWithGhostBoundaries(mask,maskLocal);
	
      // #else
      //   realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
      //   realSerialArray & vertexLocal = c.vertex();
      //   realSerialArray & stressLocal = stress[grid];
      //   const intSerialArray & maskLocal = mask;
      // #endif

      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extraInTangential);
      int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,includeGhost);

      // *********** WE NEED TO FIX THIS NEXT SECTION -- MAYBE SHOULD CALL A NEW VERSION OF
      //  getDerivedFunction:
      //     parameters.getDerivedFunction( cgf1, "pressure", p, Ib1,Ib2,Ib3 );
      //     parameters.getDerivedFunction( cgf1, "normalStress", stressg, Ib1,Ib2,Ib3 );
      // 
      // ** Just fill in the full normal stress (including p) **
      //     --> need to pass ipar[] = (grid,side,axis) for normal <---
      //     parameters.getDerivedFunction( cgf1, "normalStress", stress, Ib1,Ib2,Ib3, ipar, rpar );
      //
      // --> OR just define a special function to compute the normal force 
      // int ipar[] = {grid,side,axis,form}; real rpar[] = {t2}; 
      // Parameters::getNormalForce( realCompositeGridFunction & cgf2, realArray & normalForce, ipar, rpar );

 
      int ipar[] = {grid,side,axis,cgf2.form}; // 
      real rpar[] = { t2 }; // 
      parameters.getNormalForce( cgf2.u,stressLocal,ipar,rpar );

      pMax=0.;  // do this for now -- should we return pMax from the above routine ? 

      // torque:  (x-x0) X dF
      RealArray xCM(3);
      body[b]->getPosition(t2,xCM);
      if( ok )
      {
	if( cg.numberOfDimensions()==2 )
	{
	  stressLocal(Ib1,Ib2,Ib3,torquec) = ( (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,1)-
				  	       (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,0) );
	}
	else
	{
	  stressLocal(Ib1,Ib2,Ib3,torquec  ) = ( (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,2)-
					         (vertexLocal(Ib1,Ib2,Ib3,2)-xCM(2))*stressLocal(Ib1,Ib2,Ib3,1) );

	  stressLocal(Ib1,Ib2,Ib3,torquec+1) = ( (vertexLocal(Ib1,Ib2,Ib3,2)-xCM(2))*stressLocal(Ib1,Ib2,Ib3,0)-
					         (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,2) );

	  stressLocal(Ib1,Ib2,Ib3,torquec+2) = ( (vertexLocal(Ib1,Ib2,Ib3,0)-xCM(0))*stressLocal(Ib1,Ib2,Ib3,1)-
					         (vertexLocal(Ib1,Ib2,Ib3,1)-xCM(1))*stressLocal(Ib1,Ib2,Ib3,0) );
	}
      
	// for time history info - keep track of the maximum norm of the stress, torque, and pressure
	// on the boundary of the body
	where( maskLocal(Ib1,Ib2,Ib3)!=0 )
	{
	  for( int axis=0; axis<numberOfStressComponents; axis++ )
	  {
	    fMax = max(fMax,max(fabs(stressLocal(Ib1,Ib2,Ib3,axis))));
	  }
	  for( int axis=0; axis<numberOfTorqueComponents; axis++ )
	  {
	    gMax = max(gMax,max(fabs(stressLocal(Ib1,Ib2,Ib3,torquec+axis))));
	  }
	}
 
        // NOTE: DO WE need to compute these for the PREDICTOR stage (if it just extrapolates)
        if( useAddedMass ) 
	{
          // -- Correct stress with added mass terms  ---

	  GridFunction & cgf = isCorrectionStep ? cgf3 : cgf1;

          OV_GET_SERIAL_ARRAY(real,cgf.u[grid],uLocal);

	  const real & gamma = parameters.dbase.get<real>("gamma");
	  real rhof,v1f,v2f,ef,pf;

          // ---------------------------------------------------------------------------------
          // --- Save the components of the added mass matrices that need to be integrated ---
          // ---------------------------------------------------------------------------------
	  RealArray yv(3),xCM(3);
	  body[b]->getPosition( cgf.t,xCM );

          RealArray zfa(Ib1,Ib2,Ib3), yCrossN(Ib1,Ib2,Ib3,3);

          // create space to save the added mass extries on the boundary
          // (If there are multiple bodies we can reuse the arrays)
	  if( face >= addedMassMatrixList.getLength() )
  	    addedMassMatrixList.addElement();
	  RealArray & amm = addedMassMatrixList[face];
	  amm.redim(Ib1,Ib2,Ib3,numberOfAddedMassMatrixEntries);
	  
	  int i1,i2,i3;
          FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
	    if( cgf.form==GridFunction::conservativeVariables )
	    {
	      rhof= uLocal(i1,i2,i3,rc);
	      v1f = uLocal(i1,i2,i3,uc)/rhof;
	      v2f = uLocal(i1,i2,i3,vc)/rhof;
	      ef  = uLocal(i1,i2,i3,tc);  // ! in conservative vars this is E = p/(gamma-1) + .5*rho*v^2 
	      pf = (gamma-1.)*( ef-.5*rhof*(v1f*v1f+v2f*v2f) ); //  p 

	      // printF(" (i1,i2)=(%i,%i) conservative: (rho,u,v,p_proj)=(%8.2e,%8.2e,%8.2e,%8.2e)\n",
	      //	     i1,i2,rhof,v1f,v2f,pf);

	    }
	    else
	    {
	      //  input vars are (rho,u,v,w,T)
	      rhof= uLocal(i1,i2,i3,rc);
	      v1f = uLocal(i1,i2,i3,uc);
	      v2f = uLocal(i1,i2,i3,vc);
	      pf = rhof*uLocal(i1,i2,i3,tc); //  ! p=rho*T
	      // printF(" (i1,i2)=(%i,%i) primitive: (rho,u,v,T,p)=(%8.2e,%8.2e,%8.2e,%8.2e,%8.2e) z=%8.2e\n",
	      //        i1,i2,rhof,v1f,v2f,uLocal(i1,i2,i3,tc),pf,rhof*sqrt(gamma*pf/rhof));
	      
	    }

	    // pt= point on the body, vp=velocity of that point
	    // RealArray pt(3), vp(3);
	    // RealArray vp(3);
	    // body[b]->getVelocity( cgf.t,vp ); // For now assume the body is not rotating - use vCM  **CHECK TIME**

	    // --- We need to compute the original impedance that was used to project p: --
	    //  p = p0 + zf*( vf-vb )    [ p=projected p. p0 = unprojected)
	    //  zf = r0*a0 , a0=sqrt( gamma*p0/r0)

	    //  zf^2 = r0^2 a0^2 = gamma*r0*p0 = gamma*r0*( p - zf*(vf-vb) )
	    //  zf^2 + .5*bb*zf + cc = 0

	    // 345 real nDotDeltaV = normalLocal(i1,i2,i3,0)*( v1f-vp(0) ) + normalLocal(i1,i2,i3,1)*( v2f-vp(1) );// ******* 123
            real zf;
	    if( isCorrectionStep )
	    {
	      real nDotDeltaV = normalLocal(i1,i2,i3,0)*v1f + normalLocal(i1,i2,i3,1)*v2f;
	      real bb = .5*gamma*rhof*nDotDeltaV;
	      real cc = -gamma*rhof*pf;

	      zf = -bb + sqrt( bb*bb-cc );
	      // real pf0 = pf - zf*nDotDeltaV;
	      // real af = sqrt(gamma*pf0/rhof);
	    }
	    else
	    {
              real af =sqrt(gamma*pf/rhof);
              zf = rhof*af;
	    }
	    
	    zfa(i1,i2,i3)=zf;

	    for( int dir=0; dir<numberOfDimensions; dir++ )
	      yv(dir)=vertexLocal(i1,i2,i3,dir)-xCM(dir);

	    if( numberOfDimensions==2 )
	    {
	      yCrossN(i1,i2,i3,0)= 0.;
	      yCrossN(i1,i2,i3,1)= 0.;
	      yCrossN(i1,i2,i3,2)= yv(0)*normalLocal(i1,i2,i3,1) - yv(1)*normalLocal(i1,i2,i3,0);
	      // printF(" -- (i1,i2)=(%i,%i) y=(%8.2e,%8.2e) n=(%8.2e,%8.2e) Y*n = y X n = %8.2e\n",
              //       i1,i2,yv(0),yv(1),normalLocal(i1,i2,i3,0),normalLocal(i1,i2,i3,1),yCrossN(i1,i2,i3,2));
	    }
	    else
	    {
	      yCrossN(i1,i2,i3,0)= yv(1)*normalLocal(i1,i2,i3,2) - yv(2)*normalLocal(i1,i2,i3,1);
	      yCrossN(i1,i2,i3,1)= yv(2)*normalLocal(i1,i2,i3,0) - yv(0)*normalLocal(i1,i2,i3,2);
	      yCrossN(i1,i2,i3,2)= yv(0)*normalLocal(i1,i2,i3,1) - yv(1)*normalLocal(i1,i2,i3,0);
	    }
	    
	  } // end for_3d
	  
// 	  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
// 	  is1=is2=is3=0;
// 	  isv[axis]=1-2*side;
          if( numberOfDimensions==2 )
	  {
            // -- save added mass matrix extries ---
            amm(Ib1,Ib2,Ib3,0)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0);  // A11(0,0)
            amm(Ib1,Ib2,Ib3,1)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,1);  // A11(0,1)==A11(1,0)
            amm(Ib1,Ib2,Ib3,2)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,1);  // A11(1,1)
            amm(Ib1,Ib2,Ib3,3)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*yCrossN(Ib1,Ib2,Ib3,2);          // A22(2,2)
            amm(Ib1,Ib2,Ib3,4)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,0);
            amm(Ib1,Ib2,Ib3,5)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,1);      // A12(1,2)=A21(2,1)

//             // save added mass components in interior grid lines (temp storage)
//             int js1=is1, js2=is2, js3=is3;
//             Index Ia1=Ib1+js1, Ia2=Ib2+js2, Ia3=Ib3+js3, Ia4=0;
//             ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0);  // A11(0,0)

//             Ia4=1;
// 	    ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,1);  // A11(0,1)==A11(1,0)

//             Ia4=2;
// 	    ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,1)*normalLocal(Ib1,Ib2,Ib3,1);  // A11(1,1)
            
//             // In 2d we compute A22(2,2) and A12(0,2)=A21(2,0) and A12(1,2)=A21(2,1) 
//             js1+=is1; js2+=is2; js3+=is3;  // next interior line 
//             Ia1=Ib1+js1; Ia2=Ib2+js2; Ia3=Ib3+js3; Ia4=0;
//             ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*yCrossN(Ib1,Ib2,Ib3,2);     // A22(2,2)

//             Ia4=1;
//             ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,0); // A12(0,2)=A21(2,0)

//             Ia4=2;
//             ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*yCrossN(Ib1,Ib2,Ib3,2)*normalLocal(Ib1,Ib2,Ib3,1); // A12(1,2)=A21(2,1)

            // printF(" Save added mass M22(2,2): Ia1=[%i,%i] Ia2=[%i,%i] max=%8.2e\n",
	    // 	   Ia1.getBase(),Ia1.getBound(),Ia2.getBase(),Ia2.getBound(),
	    // 	   max(fabs(ammLocal(Ia1,Ia2,Ia3,Ia4))));

            // Ia4=1;
	    // ammLocal(Ia1,Ia2,Ia3,Ia4)=zfa(Ib1,Ib2,Ib3)*normalLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,1);
	  }
	  else
	  {
            OV_ABORT("finish me");
	  }
	  
	  
	  if( debug() & 4 )
	  {
	    printF("MovingGrids: grid=%i, t2=%8.2e, zFluid : [min,max]=[%9.3e,%9.3e] cgf.form=%i\n",
                    grid,t2,min(zfa),max(zfa),cgf.form);
	  }
	  
	} // end if use added mass
	


      } // end if OK
      
    } // end for face; compute stress*normal on a face

    fMax = ParallelUtility::getMaxValue(fMax);
    gMax = ParallelUtility::getMaxValue(gMax);

    // *wdh* 040920 : interpolate the stress to get values at the interpolation points needed for integration
    // -- do we need to worry about interior values of the stress and torque?? -- could assign nearby values to
    // equal those on the boundary
    stress.setOperators(*u.getOperators()); // operators needed for amr interpolation
    Interpolant & interpolant = *cgf2.u.getInterpolant();

    interpolant.interpolate(stress); // **OPTIMZE ME -- only need to interpolant on boundaries ***


    //..Integrate surface force & torque on rigid body

    bool useOldIntegrate=false;

    if( useOldIntegrate )
      integrate->useAdaptiveMeshRefinementGrids(false); // -- turn off AMR for testing --



    Range F=numberOfStressComponents;
    Range T=numberOfTorqueComponents;
    Range FT=numberOfForceAndTorqueComponents;
    RealArray forceTorque(FT), f(3),g(3), x(3),r(3,3); 
    f=0.;
    g=0.;

    forceTorque=0.;
    integrate->surfaceIntegral(stress,FT,forceTorque,b);  // surface integral of forces and torques.

    f(F)=forceTorque(F);
    if( numberOfDimensions==2 )
    {
      g(2)=forceTorque(torquec);  // in 2D there is only one component of the torque
    }
    else
    {
      g = forceTorque(T+torquec);
    }
    
    
    const bool directProjectionAddedMass = body[b]->getDirectProjectionAddedMass();
    if( directProjectionAddedMass || debug() & 1 )
	printF("--MVG:RB-- body %i : t2=%8.4e fCM=(%6.2e,%6.2e,%6.2e)"
		" torque=(%6.2e,%6.2e,%6.2e)\n",b,t2,f(0),f(1),f(2),g(0),g(1),g(2));
    // g=0.; 
    // integrate->surfaceIntegral(torque,T,g,b);

    // if( cg.numberOfDimensions()==2 )
    // {
    //   g(2)=g(0);
    //   g(0)=g(1)=0.;
     //  }
    
    if( useOldIntegrate && cg.numberOfRefinementLevels()>1 )
    {
      // **** test the integration that uses the AMR grids too ****

      OV_ABORT("finish me for new forceTorque");

      /* --
      Range C=numberOfDimensions;

      RealArray f2(3), g2(3);
      integrate->useAdaptiveMeshRefinementGrids(true);

      f2=0.;
      integrate->surfaceIntegral(stress,C,f2,b);
      g2=0.;
      integrate->surfaceIntegral(torque,T,g2,b);
      integrate->useAdaptiveMeshRefinementGrids(false);

      if( debug() & 4 ) 
	fprintf(parameters.dbase.get<FILE* >("moveFile"),
         "\nMovingGrids::rigidBodyMotion: body %i : t2=%7.3e fCM=(%12.7e,%12.7e,%12.7e)\n"
	   "                                                AMR:fCM=(%12.7e,%12.7e,%12.7e)\n"
           "                                                 torque=(%12.7e,%12.7e,%12.7e)\n"
	   "                                             AMR:torque=(%12.7e,%12.7e,%12.7e)\n",
               b,t2,f(0),f(1),f(2),f2(0),f2(1),f2(2),g(0),g(1),g(2),g2(0),g2(1),g2(2));
      

      f=f2; // *****
      g=g2;
      ---- */
      
    }
    else
    {
      if( debug() & 4 ) 
	fprintf(parameters.dbase.get<FILE* >("moveFile"),"\nMovingGrids::rigidBodyMotion: body %i : t2=%6.2e fCM=(%6.2e,%6.2e,%6.2e)"
		" torque=(%6.2e,%6.2e,%6.2e)\n",b,t2,f(0),f(1),f(2),g(0),g(1),g(2));
    }
    

    static int numberOfExceedsForceWarnings=0;
    static int numberOfExceedsTorqueWarnings=0;

    if( limitForces ) 
    {
      if( debug() & 2 )
	printF(" MovingGrids: t2=%8.2e, body=%i, f=(%8.2e,%8.2e) pMax=%8.2e, fMax=%8.2e, fMax*dt = %8.2e (dt=%8.2e)\n",
	       t2, b,f(0),f(1),pMax,fMax,fMax*parameters.dbase.get<real >("dt"),parameters.dbase.get<real >("dt"));

      // for testing: limit max force
      real fNorm = sqrt(f(0)*f(0)+f(1)*f(1)+f(2)*f(2));
      if( fNorm > maximumAllowableForce )
      {
	body[b]->getPosition( cgf2.t,x );
	
	if( numberOfExceedsForceWarnings < maxExceedsWarnings )
	{
	  numberOfExceedsForceWarnings++;
	  printF(" +++++ WARNING: body=%i, xCM=(%8.2e,%8.2e,%8.2e) |f| = %9.2e exceeds maxValue=%8.2e, "
		 "limiting force! ++++ \n",
		 b,x(0),x(1),x(2), fNorm,maximumAllowableForce);
	}
	else if( numberOfExceedsForceWarnings == maxExceedsWarnings )
	{
         numberOfExceedsForceWarnings++;
	  printF("  +++++ Too many exceeds limiting force warnings. I am not printing any more.  +++++\n");
	}
	
	for( int n=0; n<cg.numberOfDimensions(); n++ )
	  f(n) *= maximumAllowableForce/fNorm; 

      }
      real gNorm = sqrt(g(0)*g(0)+g(1)*g(1)+g(2)*g(2));
      if( gNorm > maximumAllowableTorque )
      {
	if( numberOfExceedsTorqueWarnings < maxExceedsWarnings )
	{
	  numberOfExceedsTorqueWarnings++;
	  printF(" +++++ WARNING: body=%i, |g| = %9.2e exceeds maxValue=%8.2e, limiting torque! ++++ \n",
		 b,gNorm,maximumAllowableTorque);
	}
	else if( numberOfExceedsTorqueWarnings == maxExceedsWarnings )
	{
	  numberOfExceedsTorqueWarnings++;
	  printF("  +++++ Too many exceeds limiting torque warnings. I am not printing any more.  +++++\n");
	}

	for( int n=0; n<cg.numberOfDimensions(); n++ )
	  g(n) *= maximumAllowableTorque/gNorm; 

      }
    }

    // add weight after limiting *wdh* 040916 : subtract off the fluid density
    // first compute the mass of the displaced fluid
    const real bodyMass = body[b]->getMass();
    const real fluidMass = getFluidMassOfBody( b );

    // real fluidMass=0.;
    // if( parameters.dbase.get<real >("fluidDensity")!=0. )
    // {
    //   if( body[b]->getDensity()>0. )
    //   {
    //     assert( bodyMass!=0. );
    //     real volume = bodyMass/body[b]->getDensity();
    // 	fluidMass = parameters.dbase.get<real >("fluidDensity")*volume; 

    //     const real dt = parameters.dbase.get<real >("dt");
    //     if( (debug() & 2) && t2< 2.*dt ) 
    //       printF(" --- body b=%i: bodyMass=%8.2e, fluidMass=%8.2e, volume=%9.3e\n",b,bodyMass,fluidMass,volume);

    //   }
    //   else
    //   {
    // 	printF("MovingGrids:ERROR: The fluid density is not zero but the body density is unknown.\n"
    //            " Thus the volume of the body b=%i cannot be computed! \n",b);
    //   }
    // }
    

    // get the gravity vector -- may be time dependent for a slow start
    real gravity[3];
    parameters.getGravityVector( gravity,t2 );

    for( int n=0; n<cg.numberOfDimensions(); n++ )
      f(n)+=gravity[n]*(bodyMass-fluidMass);  // add weight: mass*g 

    // if( true ) // ***** TEMP ***
    //  printF("  body=%i : t=%9.2e, bodyMass=%9.3e fluidMass=%9.3e force=[%9.3e,%9.3e]\n",b,t3,bodyMass,fluidMass,f(0),f(1));


    // --- compute added mass matrices ... finish me ...

    RealArray A11, A12, A21, A22;
    if( useAddedMass )
    {
      A11.redim(3,3); A12.redim(3,3); A21.redim(3,3); A22.redim(3,3);
	  
      A11=0.; A12=0.; A21=0.; A22=0.;

      // Here is the added mass for a cylinder with radius rad
      //real rad=.5;
      //real a11 = zFluid*Pi*rad;
 
      // Piston problem:
      real area=1.;
      real a11=zFluid*area;
	  
      //  A11(0,0)=a11; A11(1,1)=a11; A11(2,2)=a11;

      // -- copy the added mass coefficients from the temp storage to the boundary so that
      //    we can integrate ---

      // We need to loop multiple times since we cannot save all the entries at once in the stress grid function
      int numberOfAddedMassIterations= 2;  // FIX ME for 3D
      for( int ma=0; ma<numberOfAddedMassIterations; ma++ )
      {
	for( int face=0; face<numberOfFaces; face++ )
	{
	  int side=-1,axis,grid;
	  integrate->getFace(b,face,side,axis,grid);
	  assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	  assert( grid>=0 && grid<cg.numberOfComponentGrids());

	  MappedGrid & c = cg[grid];
	  const intArray & mask = c.mask();
	  OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
	  OV_GET_SERIAL_ARRAY(real,stress[grid],stressLocal);
	  // OV_GET_SERIAL_ARRAY(real,amm[grid],ammLocal);
      
	  RealArray & amm = addedMassMatrixList[face];

	  getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extraInTangential);
	  int includeGhost=1;
	  bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,includeGhost);
	  if( ok )
	  {
// 	    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
// 	    is1=is2=is3=0;
// 	    isv[axis]=1-2*side;
	    if( numberOfDimensions==2 )
	    {
// 	      int js1=is1, js2=is2, js3=is3;
//               Index Ia1, Ia2, Ia3, Ia4;
	      Range R3=3;
	      if( ma==0 )
	      {
                // we integrate these three entries the first time thru
                stressLocal(Ib1,Ib2,Ib3,R3)=amm(Ib1,Ib2,Ib3,R3);

// 	        Ia1=Ib1+js1; Ia2=Ib2+js2; Ia3=Ib3+js3; Ia4=0;
// 		stressLocal(Ib1,Ib2,Ib3,0)=ammLocal(Ia1,Ia2,Ia3,Ia4);
// 		Ia4=1;
// 		stressLocal(Ib1,Ib2,Ib3,1)=ammLocal(Ia1,Ia2,Ia3,Ia4);
// 		Ia4=2;
// 		stressLocal(Ib1,Ib2,Ib3,2)=ammLocal(Ia1,Ia2,Ia3,Ia4);
            
	      }
	      else
	      {
                // We integrate these entries the last time through
                stressLocal(Ib1,Ib2,Ib3,R3)=amm(Ib1,Ib2,Ib3,R3+3);

// 		js1+=is1; js2+=is2; js3+=is3;  // these next values are saved on one additional line in.
// 		Ia1=Ib1+js1; Ia2=Ib2+js2; Ia3=Ib3+js3; Ia4=0;
// 		stressLocal(Ib1,Ib2,Ib3,0)=ammLocal(Ia1,Ia2,Ia3,Ia4);
// 		Ia4=1;
// 		stressLocal(Ib1,Ib2,Ib3,1)=ammLocal(Ia1,Ia2,Ia3,Ia4);
// 		Ia4=2;
// 		stressLocal(Ib1,Ib2,Ib3,2)=ammLocal(Ia1,Ia2,Ia3,Ia4);

	      }
	      
	    }
	    else
	    {
	      OV_ABORT("finish me");
	    }
	  }

	} // end for face

        // --- integrate the components of the added mass matrix -----
	int numberOfAddedMassComponents=3; // fix me for 3D
	RealArray amc(numberOfAddedMassComponents);  // added mass coefficients
	amc=0.;
	Range A = numberOfAddedMassComponents;
	interpolant.interpolate(stress,A);  // *wdh* 2012/03/07 
	
	integrate->surfaceIntegral(stress,A,amc,b);


	real addedMassScaleFactor=1.; // 1.; // 1.05;  // ****************************** FIX ME 

	if( numberOfDimensions==2 )
	{
	  if( ma==0 )
	  {
	    // printF("--- MovingGrids:: body=%i integral: A11(0,0)=%9.3e, A11(0,1)=%9.3e, A11(1,1)=%9.3e "
            //       "(addedMassScaleFactor=%8.2e)\n",b,amc(0),amc(1),amc(2),addedMassScaleFactor);
	    A11(0,0)=amc(0)*addedMassScaleFactor;
	    A11(0,1)=amc(1)*addedMassScaleFactor;
	    A11(1,0)=A11(0,1);
	    A11(1,1)=amc(2)*addedMassScaleFactor;
	    A11(2,2)=1.;  // this value doesn't matter in 2D 
	  }
	  else
	  {
	    // real addedMassScaleFactor=1.1; // ellipse stops quickly
	    // real addedMassScaleFactor=1.01; // 
	    // real addedMassScaleFactor=1.05;  // ellipse slows down
	    A22(2,2)=amc(0)*addedMassScaleFactor;

	    A12(0,2)=amc(1)*addedMassScaleFactor;
	    A12(1,2)=amc(2)*addedMassScaleFactor;

	    A21(2,0)=A12(0,2);
	    A21(2,1)=A12(1,2);
	      
	    // printF("--- MovingGrids:: body=%i integral: A22(2,2)= %9.3e, A12(0,2)=%9.3e, A12(1,2)=%9.3e "
            //        "(addedMassScaleFactor=%8.2e)\n",b,A22(2,2),A12(0,2),A12(1,2),addedMassScaleFactor);
	  }
	}
	else
	{
	  OV_ABORT("finish me");
	}
	  
	
      } // end for ma -- loop to compute the different added mass terms
      

    } // end if useAddedMass
    

    //..GIVEN FORCES--> integrate Newton's 2nd
    if( t3>t1 )
    {
      if( !useAddedMass )
      {
	body[b]->integrate( t2,f,g, t3,x,r  );
      }
      else
      {

	printF(" ++++ MovingGrids:PREDICT: body=%i t3=%9.3e, f=(%8.2e,%8.2e,%8.2e)"
               ", g=(%8.2e,%8.2e,%8.2e)\n",b,t3,f(0),f(1),f(2),g(0),g(1),g(2));
        printF("      A11(0,0)=%9.3e A11(0,1)=%9.3e A11(1,1)=%9.3e A22(2,2)=%9.3e A12(0,2)=%9.3e A12(1,2)=%9.3e\n",
               A11(0,0),A11(0,1),A11(1,1),A22(2,2),A12(0,2),A12(1,2));
	if( false )
	{
	  A11(0,1)=0.; A11(1,1)=0.; A22(2,2)=0.; A12(0,2)=0.; A12(1,2)=0.;
	}
	


// 	  RealArray vCM(3);
// 	  body[b]->getVelocity(t2,vCM); // force is at time t2

// 	  RealArray vFluid(Range(uc,uc+2));  // holds average fluid velocity on the body
// 	  vFluid=0.;
// 	  integrate->surfaceIntegral(cgf2.u,V,vFluid,b);
// 	  vFluid /= 2.*Pi*rad;  // divide by circumference

// 	  for( int j=0; j<3; j++ ){ f(j) += A11(j,j)*vFluid(uc+j); } //  subtract off added mass contriuution from f

// 	  printF(" ++++ predict: body %i:  vCM=(%8.2e,%8.2e,%8.2e) vFluid=(%8.2e,%8.2e,%8.2e)\n",
// 		 b, vCM(0),vCM(1),vCM(2), vFluid(uc),vFluid(uc+1),vFluid(uc+2) );
	
        body[b]->integrate( t2,f,g,t3, A11,A12,A21,A22, x,r  );

        // for( int j=0; j<3; j++ ){ f(j) -= A11(j,j)*vFluid(uc+j); } //  reset
	if( true )
	{
	  RealArray vCM(3);
	  body[b]->getVelocity( t3,vCM ); // For now assume the body is not rotating - use vCM  **CHECK TIME**
          printF(" ++++          After predict: t3=%9.3e, vb=(%8.2e,%8.2e)\n",t3,vCM(0),vCM(1));
	}

      }
    }
    else
    {
      if( debug() & 4 ) 
        printF("--MVG--rigidBodyMotion: correct solution at time t=%e\n",t3);

      if( !useAddedMass )
      {
	body[b]->correct( t3,f,g,x,r );
      }
      else
      {
	printF(" ++++ MovingGrids:CORRECT: body=%i t3=%9.3e, f=(%8.2e,%8.2e,%8.2e)"
               ", g=(%8.2e,%8.2e,%8.2e)\n",b,t3,f(0),f(1),f(2),g(0),g(1),g(2));

        printF("      A11(0,0)=%9.3e A11(0,1)=%9.3e A11(1,1)=%9.3e A22(2,2)=%9.3e A12(0,2)=%9.3e A12(1,2)=%9.3e\n",
               A11(0,0),A11(0,1),A11(1,1),A22(2,2),A12(0,2),A12(1,2));


// 	  RealArray vCM(3);
// 	  body[b]->getVelocity(t3,vCM); // force is at time t3

// 	  RealArray vFluid(Range(uc,uc+2));  // holds average fluid velocity on the body
// 	  vFluid=0.;
// 	  integrate->surfaceIntegral(cgf2.u,V,vFluid,b); // is cgf2 correct here ? **********
// 	  vFluid /= 2.*Pi*rad;  // divide by circumference

// 	  for( int j=0; j<3; j++ ){ f(j) += A11(j,j)*vFluid(uc+j); } //  subtract off added mass contriuution from f

// 	  printF(" ++++ correct: body %i:  vCM=(%8.2e,%8.2e,%8.2e) vFluid=(%8.2e,%8.2e,%8.2e)\n",
// 		 b, vCM(0),vCM(1),vCM(2), vFluid(uc),vFluid(uc+1),vFluid(uc+2) );
	
	body[b]->correct( t3,f,g,A11,A12,A21,A22, x,r );


	if( false )
	{
	  RealArray vCM(3), w(3);
	  body[b]->getVelocity( t3,vCM ); 
	  body[b]->getAngularVelocities( t3,w ); 
          printF(" RBM: ++++  After correct: t3=%9.3e, vCM=(%8.2e,%8.2e,%8.2e), w=(%8.2e,%8.2e,%8.2e)\n",
		 t3,vCM(0),vCM(1),vCM(2),w(0),w(1),w(2));
	}
	

        // for( int j=0; j<3; j++ ){ f(j) -= A11(j,j)*vFluid(uc+j); } //  reset
      }
      

      // We keep track if the correction steps have converged (N.B. for "light" rigid bodies for e.g.)
      // -- finish me --
      correctionHasConverged=correctionHasConverged && body[b]->getCorrectionHasConverged();
      maximumRelativeCorrection=max( maximumRelativeCorrection,body[b]->getMaximumRelativeCorrection());

      if( debug() & 4  )
      {
	real dtMax = body[b]->getTimeStepEstimate();
	printF("  body=%i : t=%9.2e, estimated maximum dt = %9.2e (dt=%9.2e)\n",b,t3,dtMax,parameters.dbase.get<real >("dt"));
      }
      
    }
    

    // ----------------------------------------------------------------------------------------
    // --- Save info about the rigid body: position, velocity, forces etc. to be saved as a ---
    // ---  a sequence in the show file ---
    // ----------------------------------------------------------------------------------------
    

    // This next is just for debugging *wdh* 2012/07/15
    if( false &&
        t2>0. && isCorrectionStep ) // *wdh* 2012/07/15 -- body[b] may not be initialized at t=0. yet
    {
      RealArray f0(3), g0(3);
      body[b]->getForce(t2,f0,g0);
      printF("~~rigidBody %i: correction: t=%8.4e f=(%9.3e,%9.3e,%9.3e) g=(%6.1e,%6.1e,%6.1e) (from getForce)\n",
	     b,t2,f0(0),f0(1),f0(2),g0(0),g0(1),g0(2));
    }
    


    // For added mass case do not save info on the correction step as the force is not the true force but
    // the partially projected value : -p + n.vf 
    if( !( isCorrectionStep && useAddedMass ) )
    {

      // *** get values at t2 for outputing to a file

      RealArray vCM(3); vCM=0.;

      body[b]->getPosition(t2,x);  // could merge these calls

      body[b]->getVelocity(t2,vCM);
    
      RealArray w(3);
      body[b]->getAngularVelocities(t2,w);

      RealArray aCM(3);
      body[b]->getAcceleration(t2,aCM);

      RealArray wDot(3);
      body[b]->getAngularAcceleration(t2,wDot);



      if( debug() & 1 ) 
      {

	fprintf(parameters.dbase.get<FILE* >("moveFile"),"rigidBody %i: t=%8.4e dt=%8.4e x=(%6.2e,%6.2e,%6.2e) vCM=(%9.3e,%9.3e,%9.3e)\n"
		"               f=(%9.3e,%9.3e,%9.3e) g=(%6.1e,%6.1e,%6.1e) w=(%7.2e,%7.2e,%7.2e)\n",
		b,t2,parameters.dbase.get<real >("dt"),x(0),x(1),x(2),vCM(0),vCM(1),vCM(2),
		f(0),f(1),f(2),g(0),g(1),g(2),w(0),w(1),w(2));
      
	//RealArray r(3,3);
	//body[b]->getRotationMatrix(t2,r);
        //        fprintf(parameters.dbase.get<FILE* >("moveFile"),
        //  	      "     [ %8.6f  %8.6f %8.6f ]\n"
        //  	      " r = [ %8.6f  %8.6f %8.6f ]\n"
        //  	      "     [ %8.6f  %8.6f %8.6f ]\n"
        //  	      ,r(0,0),r(0,1),r(0,2),r(1,0),r(1,1),r(1,2),r(2,0),r(2,1),r(2,2));
      
      }
    
      int i=rigidBodyInfoCount;
      if(  i>rigidBodyInfo.getBound(0) )
      {
	// allocate more space for info.
	int num=rigidBodyInfo.getLength(0);
	Range R(0,num-1);
	RealArray rbi,rbt; rbi=rigidBodyInfo; rbt=rigidBodyInfoTime; // save old values.
	num=int(num*1.5+100);
      
	rigidBodyInfo.redim(num,numberOfRigidBodyInfoNames,numberOfRigidBodies);
	rigidBodyInfoTime.redim(num);
	rigidBodyInfo(R,all,all)=rbi;
	rigidBodyInfoTime(R)=rbt;
      }

      if( i>0 && rigidBodyInfoTime(i-1)==t2 )
	i--;  // we already have this time, must be a correction step so just replace current values.
      else      
	rigidBodyInfoCount++;

      rigidBodyInfoTime(i)=t2;

      // printf("********* Save rigid body data: t1,t2,t3 = %10.4e, %10.4e %10.4e,\n",t1,t2,t3);
    

      // RealArray w(3);
      // body[b]->getAngularAcceleration(t3,w);  // Save omega instead if the angular acceleration

      if( i<=rigidBodyInfo.getBound(0) )
      {
	int j=0;
	rigidBodyInfo(i,j,b)=x(0); j++;
	rigidBodyInfo(i,j,b)=x(1); j++;
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=x(2); j++;  }
	rigidBodyInfo(i,j,b)=vCM(0); j++;
	rigidBodyInfo(i,j,b)=vCM(1); j++;
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=vCM(2); j++; }
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=w(0); j++; }
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=w(1); j++; }
	rigidBodyInfo(i,j,b)=w(2); j++;
	rigidBodyInfo(i,j,b)=f(0); j++;
	rigidBodyInfo(i,j,b)=f(1); j++;
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=f(2); j++; }
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=g(0); j++; }
	if( numberOfDimensions==3 ){ rigidBodyInfo(i,j,b)=g(1); j++; }
	rigidBodyInfo(i,j,b)=g(2); j++;

        for( int axis=0; axis<numberOfDimensions; axis++ ) { rigidBodyInfo(i,j,b)=aCM(axis); j++; } // 
	if( numberOfDimensions==2 )
	{
	  rigidBodyInfo(i,j,b)=wDot(2); j++;  // only one angular acceleration in 2D
	}
	else
	{
	  for( int axis=0; axis<numberOfDimensions; axis++ ) { rigidBodyInfo(i,j,b)=wDot(axis); j++; } // 
	}
	
	rigidBodyInfo(i,j,b)=fMax; j++;
	rigidBodyInfo(i,j,b)=gMax; j++;
	rigidBodyInfo(i,j,b)=pMax; j++;

	assert( (j-1)<= rigidBodyInfo.getBound(1) );
      }
    
    } // end saving rigid body info
	

  } // END for( b=0; b<numberOfRigidBodies ...)

  addedMassMatrixList.destroy();
      

  return 0;
}



//..New DeformingBody update -- no viscous stresses yet
//\begin{>>MovingGridsSolverInclude.tex}{\subsection{moveDeformingBodies}}
int MovingGrids::
moveDeformingBodies(const real & t1, 
		    const real & t2, 
		    const real & t3,
		    const real & dt0,
		    GridFunction & cgf1,  
		    GridFunction & cgf2,
		    GridFunction & cgf3 )
// =========================================================================================
// /Description:
//    Determine {\em body} motion for deforming objects. 
//    Integrate the equations of motion for the bodies up to time t3.
//
//    E.g., for the filament, this routine moves just the filament by integrating
//    the elastic equations of motion for the bdry (spline) curve. The body-fitted
//    grid is generated elsewhere. 
//
//    As a first pass, move the bdry according to a {\em given} function, so trivial dynamics. 
//    Later, add the Kirchoff rod-eqns, which are forced by surface stresses from the fluid. Allow
//    for a body-force, too.
//
//    Viscous stress computation not implemented yet.
//
// /Author: pf, wdh
// /Notes:
//\end{MovingGridsSolverInclude.tex}  
//========================================================================================={
{
  if ( numberOfDeformingBodies==0 )
    return 0;

  if( debug() & 4 )
  {
    cout << "MovingGrids::moveDeformingBodies called with " << numberOfDeformingBodies << " deformingBodies\n";
  }
  
  assert( isInitialized );
  // RealCompositeGridFunction & u = cgf2.u;
  CompositeGrid & cg =  cgf2.cg;
  const int numberOfDimensions = cg.numberOfDimensions();

  Range all;
  RealCompositeGridFunction stress;
  if( true )
  {
    stress.updateToMatchGrid(cg,all,all,all,cg.numberOfDimensions());
    stress=0.;
  }
  
  Index Ib1,Ib2,Ib3;

  //..Loop through deformingBodies
  for ( int b=0; b<numberOfDeformingBodies; b++ )
  {
    if( debug() & 4 ){ cout << "++ Looping: deformingBody b="<<b<<endl;}
    //
    //..Here: surfaceStress stuff -- omitted for now
    //..ALSO, add forces to RHS (e.g. gravity)
    //
    stress=0.; //.. & COMPUTE STRESSES
    
    //..GIVEN FORCES--> integrate Newton's 2nd
    // deformingBodyList[b]->integrate( t2, stress, t3);  // *wdh* 050917
    deformingBodyList[b]->integrate( t1,t2,t3,cgf1,cgf2,cgf3, stress);

    //..STORE INFO for plotting the bodies HERE..
    // nix for now
  }
  return 0;
}



// =================================================================================================
/// \brief Detect collisions
// =================================================================================================
int MovingGrids::
detectCollisions( GridFunction & cgf1 )
{
  if( !parameters.dbase.get<bool >("detectCollisions") )
    return 0;
  
  assert( isInitialized );
  assert( integrate!=NULL );
  ::detectCollisions( cgf1.t,cgf1.cg,numberOfRigidBodies,body,integrate->getBodyDefinition(),parameters.dbase.get<real >("collisionDistance"));

  return 0;
}


// ===============================================================================================
/// \brief Project the interface velocity (for added mass schemes)
// ===============================================================================================
int MovingGrids::
projectInterfaceVelocity( GridFunction & cgf )
{

  for ( int b=0; b<numberOfDeformingBodies; b++ )
  {
    deformingBodyList[b]->projectInterfaceVelocity( cgf );
  }

  return 0;
}



// =================================================================================================
/// \brief Save moving grid info the the show file. We save a "sequence" for each rigid body.
// =================================================================================================
int MovingGrids::
saveToShowFile( ) const
{
  if( debug() & 2 )
    printF("***** MovingGrids::saveToShowFile ***\n");
  if( parameters.dbase.get<Ogshow* >("show")!=NULL && numberOfRigidBodies>0 && rigidBodyInfoCount>0 )
  {
    if( debug() & 2 )
      printF("***** MovingGrids::saveToShowFile: save a sequence ***\n");
    
    Range all,N(0,rigidBodyInfoCount-1);
    char buff[40];
    for( int b=0; b<numberOfRigidBodies; b++ )
    {
      parameters.dbase.get<Ogshow* >("show")->saveSequence( sPrintF(buff,"rigid body %i",b),rigidBodyInfoTime(N),rigidBodyInfo(N,all,b),
				 rigidBodyInfoName);
    }
  }
  return 0;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{updateToMatchGrid}} 
int MovingGrids::
updateToMatchGrid( CompositeGrid & cg )
// =================================================================================================
// /Description:
//    Update the parameters when the grid has changed, such as the number of grids has changed
// during an AMR regrid.
//\end{MovingGridsSolverInclude.tex}  
// =================================================================================================
{

  int oldNumber=movingGrid.getLength(0);
  int newNumber=cg.numberOfComponentGrids();

  //   gridsToMove.resize(newNumber);  --- what is this ? ---

  movingGrid.resize(newNumber);
  moveOption.resize(newNumber);
  moveParameters.resize(8,newNumber);
  
  Range all;
  Range R(oldNumber,max(oldNumber,newNumber-1));
  if( newNumber>oldNumber )
  {
    movingGrid(R)=false;
    moveOption(R)=-1;
    moveParameters(all,R)=0;
  }
  for( int grid=0; grid<newNumber; grid++ )
  {
    if( cg.refinementLevelNumber(grid)>0 )
    {
      // refinement grids are moved in the same way as their parent
      int base = cg.baseGridNumber(grid);
      movingGrid(grid)=movingGrid(base);
      moveOption(grid)=moveOption(base);
      moveParameters(all,grid)=moveParameters(all,base);
      
    }
  }
  
//   if( cg.numberOfRefinementLevels()>0 && numberOfRigidBodies>0 )
//   {

//     printf("****MovingGrids::updateToMatchGrid:ERROR rigid bodies not updated for AMR grids\n"
// 	   " This needs to be finished, fix this Bill!\n");
//   }
  
  return 0;
}

//\begin{>>MovingGridsSolverInclude.tex}{\subsection{get}} 
int MovingGrids::
get( const GenericDataBase & dir, const aString & name)
// =======================================================================================
// /Description:
//  Read parameters to a data-base file
//\end{MovingGridsSolverInclude.tex}  
//=========================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"MovingGrids");

  aString className;
  subDir.get( className,"className" ); 

  subDir.get(gridsToMove,"gridsToMove"); 
  subDir.get(isInitialized,"isInitialized"); 
  subDir.get(movingGridProblem,"movingGridProblem"); 
  subDir.get(moveOption,"moveOption"); 
  subDir.get(movingGrid,"movingGrid"); 
  subDir.get(moveParameters,"moveParameters"); 

  if( matrixMotionBody!=NULL )
  {
    for( int b=0; b<numberOfMatrixMotionBodies; b++ )
    {
      if( matrixMotionBody[b]->decrementReferenceCount()==0 )
	delete matrixMotionBody[b];
    }
    delete [] matrixMotionBody;
    matrixMotionBody=NULL;
  }

  subDir.get(numberOfMatrixMotionBodies,"numberOfMatrixMotionBodies"); 
  if( numberOfMatrixMotionBodies>0 )
  {
    matrixMotionBody = new MatrixMotion* [numberOfMatrixMotionBodies];  
    for( int b=0; b<numberOfMatrixMotionBodies; b++ )
    {
      matrixMotionBody[b] = new MatrixMotion;
      matrixMotionBody[b]->get(subDir,sPrintF("matrixMotionBody%i",b));
    }
  }    

  
  if( body!=NULL )
  {
    for( int b=0; b<numberOfRigidBodies; b++ )
      delete body[b];
  
    delete [] body;
    body=NULL;
  }
  subDir.get(numberOfRigidBodies,"numberOfRigidBodies"); 
  if( numberOfRigidBodies>0 )
  {
    body = new RigidBodyMotion* [numberOfRigidBodies];  // usually space is allocated for all grids
    for( int b=0; b<numberOfRigidBodies; b++ )
    {
      body[b] = new RigidBodyMotion();
      body[b]->get(subDir,sPrintF("rigidBody%i",b));

      body[b]->setBodyNumber( b );
      body[b]->setParameters( parameters );
      
    }
  }
  
  subDir.get(rigidBodyInfoCount,"rigidBodyInfoCount");
  subDir.get(rigidBodyInfo,"rigidBodyInfo");
  subDir.get(rigidBodyInfoTime,"rigidBodyInfoTime");

  delete [] rigidBodyInfoName;
  subDir.get(numberOfRigidBodyInfoNames,"numberOfRigidBodyInfoNames");
  rigidBodyInfoName=new aString [numberOfRigidBodyInfoNames];
  subDir.get(rigidBodyInfoName,"rigidBodyInfoName",numberOfRigidBodyInfoNames);

  // *** todo: deforming bodies ****
  subDir.get(numberOfDeformingBodies,"numberOfDeformingBodies"); 
  /*if( deformingBodyList!=NULL )
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
      delete deformingBodyList[b];
  
    delete [] deformingBodyList;
    deformingBodyList=NULL;
  }
  */
  if (numberOfDeformingBodies > 0) {
    
    //deformingBodyList = new DeformingBodyMotion*[numberOfDeformingBodies];
    
    for( int b=0; b<numberOfDeformingBodies; b++ )
      {
	//deformingBodyList[b] = new DeformingBodyMotion(parameters);
	deformingBodyList[b]->get(subDir,sPrintF("deformingBody%i",b));
      }
  }
    


  delete &subDir;
  return 0;
}


//\begin{>>MovingGridsSolverInclude.tex}{\subsection{putt}} 
int MovingGrids::
put( GenericDataBase & dir, const aString & name) const
// ======================================================================================
// /Description: 
//    Save parameters to a data-base file
//\end{MovingGridsSolverInclude.tex}  
// ======================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"MovingGrids");                 // create a sub-directory 
  aString className="MovingGrids";
  subDir.put( className,"className" );

  subDir.put(gridsToMove,"gridsToMove"); 
  subDir.put(isInitialized,"isInitialized"); 
  subDir.put(movingGridProblem,"movingGridProblem"); 
  subDir.put(moveOption,"moveOption"); 
  subDir.put(movingGrid,"movingGrid"); 
  subDir.put(moveParameters,"moveParameters"); 

  subDir.put(numberOfMatrixMotionBodies,"numberOfMatrixMotionBodies"); 
  if( numberOfMatrixMotionBodies>0 )
  {
    for( int b=0; b<numberOfMatrixMotionBodies; b++ )
      matrixMotionBody[b]->put(subDir,sPrintF("matrixMotionBody%i",b));
  }
  
  subDir.put(numberOfRigidBodies,"numberOfRigidBodies"); 
  if( numberOfRigidBodies>0 )
  {
    for( int b=0; b<numberOfRigidBodies; b++ )
      body[b]->put(subDir,sPrintF("rigidBody%i",b));
  }
  
  subDir.put(rigidBodyInfoCount,"rigidBodyInfoCount");
  subDir.put(rigidBodyInfo,"rigidBodyInfo");
  subDir.put(rigidBodyInfoTime,"rigidBodyInfoTime");

  subDir.put(numberOfRigidBodyInfoNames,"numberOfRigidBodyInfoNames");
  subDir.put(rigidBodyInfoName,"rigidBodyInfoName",numberOfRigidBodyInfoNames);

  // *** deforming bodies ****
  subDir.put(numberOfDeformingBodies,"numberOfDeformingBodies"); 
  if (numberOfDeformingBodies > 0) 
  {
    for( int b=0; b<numberOfDeformingBodies; b++ )
    {
      deformingBodyList[b]->put(subDir,sPrintF("deformingBody%i",b));
    }
  }
    


  delete &subDir;
  return 0;  
}


// =================================================================================================
/// \brief Plot things related to moving grids (e.g. the center lines of beams or shells)
// =================================================================================================
int MovingGrids::
plot(GenericGraphicsInterface & gi, GridFunction & cgf, GraphicsParameters & psp )
{
  // turn off plotting of titles and labels
  int plotTitleLabels;
  psp.get(GI_PLOT_LABELS, plotTitleLabels);
  psp.set(GI_PLOT_LABELS, false);

  for( int b=0; b<numberOfDeformingBodies; b++ )
  {
    // printF("MovingGrids::plot deforming body %i\n",b);
    deformingBodyList[b]->plot( gi,cgf,psp );
  }

  psp.set(GI_PLOT_LABELS, plotTitleLabels); // reset 
  

  return 0;
}

// =================================================================================================
/// \brief Compute rigid body properties such as the mass, center of mass and moments of inertia
/// \details Compute properties of the rigid body that have not been specified by the user using surface ietgrals:
///   - center of mass
///   - Mass of the body given the density
///   - Moment of inertia matrix given the density
///
/// \param bodyNumber (input) : compute properties for this body
/// \param cg (input) : current grid
// =================================================================================================
int MovingGrids::
computeRigidBodyProperties( const int bodyNumber, CompositeGrid & cg )
{
  // Precompute the initial centre of mass for any Rigid Body that was not given a center of mass,
  // assuming a uniform density.

  RigidBodyMotion & rigidBody = *(body[bodyNumber]);

  const int numberOfDimensions = cg.numberOfDimensions();
  

  realCompositeGridFunction f(cg);   // *************** fix me *****************
  f=1.;
  real surfaceArea=integrate->surfaceIntegral(f,bodyNumber);
  printF("--MVG--computeRigidBodyProperties:Surface area computed by Integrate for body=%i is %10.4e \n",
         bodyNumber,surfaceArea);

  if( !(rigidBody.centerOfMassHasBeenInitialized()) )
  {
    const BodyDefinition & bodyDefinition = integrate->getBodyDefinition();

    RealArray x0(3);
    x0=0.;
    int numberOfPointsOnBody=0;
	
    //  get the grid(s) that form the body
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(bodyNumber);
    Index Ib1,Ib2,Ib3;
    //  *************************************************************
    //  **** Compute the centre of mass using surface integrals  ****
    //  *************************************************************
    for( int dir=0; dir<numberOfDimensions; dir++ )
    {
      f=0.;
      for( int face=0; face<numberOfFaces; face++ )
      {
	int side=-1,axis,grid;
	bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
	assert( side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions);
	assert( grid>=0 && grid<cg.numberOfComponentGrids());

	// printF("\nMovingGrids::detectCollisions: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
	//   b,face,side,axis,grid);

	MappedGrid & c = cg[grid];
	c.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
	  
        int extra=1;  // do we need extra for periodic ? 
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // NB to use gridIndexRange, not indexRange
	realArray & x = c.vertex();
        OV_GET_SERIAL_ARRAY(real,x,xLocal);
        OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);
	    
	// f[grid](Ib1,Ib2,Ib3)=x(Ib1,Ib2,Ib3,dir);
	int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);
	if( ok )
	  fLocal(Ib1,Ib2,Ib3)=xLocal(Ib1,Ib2,Ib3,dir);
	  
      }
      x0(dir)=integrate->surfaceIntegral(f,bodyNumber)/surfaceArea;
    }


    printF("--MVG--CRBP-- INFO: body %i has a center of mass: (integrate) (%8.2e,%8.2e,%8.2e)\n",
	   bodyNumber,x0(0),x0(1),x0(2));

    rigidBody.setInitialCentreOfMass(x0);
  }

  if( !(rigidBody.massHasBeenInitialized()) && rigidBody.getDensity()>=0   )
  {
    // If the mass has not been given but the density has, we compute the mass

    // the volume of the body can be computed as a surface integral using the divergence theorem
    //    
    //  bodyVolume = int_V div.(x,0,0) dV = int_S (x,0,0).nv dS
    //     --> set f= (x,0,0).nv = x*n_x

    const BodyDefinition & bodyDefinition = integrate->getBodyDefinition();
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(bodyNumber);
    Index Ib1,Ib2,Ib3;

    f=0.;
    for( int face=0; face<numberOfFaces; face++ )
    {
      int side=-1,axis,grid;
      bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
      assert( side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions);
      assert( grid>=0 && grid<cg.numberOfComponentGrids());

      // printF("\nMovingGrids::detectCollisions: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
      //   b,face,side,axis,grid);

      MappedGrid & c = cg[grid];
      c.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal );
	  
      OV_GET_SERIAL_ARRAY(real,c.vertex(),xLocal);
      OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);

      #ifdef USE_PPP
        const realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
      #else
         const realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
      #endif 

      int extra=1;  // do we need extra for periodic ? 
      getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // NB to use gridIndexRange, not indexRange

      int includeGhost=1;
      bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);
      if( ok )
	fLocal(Ib1,Ib2,Ib3)=xLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0); 

    }

    // The surface integral should be negative since the outward normal to the body is minus the
    // outward normal to the grid.
    real bodyVolume = fabs( integrate->surfaceIntegral(f,bodyNumber) );
    printF("--MVG--CRBP-- INFO: Body %i volume from surface-integral = %10.4e \n",bodyNumber,bodyVolume);
	
    if( numberOfFaces==1 ) // -- check the result for an Annulus ---
    {
      int side=-1,axis,grid;
      int face=0;
	  
      bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
      Mapping & map = cg[grid].mapping().getMapping();
      if( map.getClassName()=="AnnulusMapping" )
      {
	AnnulusMapping & annulus = (AnnulusMapping &)map;
	real radius = annulus.innerRadius; 
        bodyVolume=Pi*radius*radius;

	printF("--MVG--CRBP-- INFO: Body %i is an annulus with radius=%10.4e, true volume = %10.4e \n",
	       bodyNumber,radius,bodyVolume);

      }
    }

    rigidBody.setVolume(bodyVolume); // *wdh* Sept 25, 2016

    real density=rigidBody.getDensity();
    real mass= bodyVolume*density;
    rigidBody.setMass(mass);
    printF("--MVG--CRBP-- INFO: Body %i has volume=%12.5e, density=%9.3e; Setting mass=%12.5e\n",
	   bodyNumber,bodyVolume,density,mass);
  }
      
  // --- Moment of Inertial Matrix ---
  printF(" rigidBody.momentsOfInertiaHaveBeenInitialized()=%i\n",rigidBody.momentsOfInertiaHaveBeenInitialized());
  

  if( !(rigidBody.momentsOfInertiaHaveBeenInitialized()) && 
      rigidBody.getDensity()>=0   )
  {
    // --- Compute the  moments of inertia given the density -----

    // In 2D we only need to compute one number 
    //         Ib(2,2) = density *  int_V x^2 + y^2 dV

    // In 3D we need to compute 6 numbers:
    //        Ib(0,0) = density * int_V y^2 + z^2 dV
    //        Ib(0,1) = density * int_V -x*y dV
    //        Ib(0,2) = density * int_V -x*z dV
    //        Ib(1,1) = density * int_V x^2 + y^2 dV
    //        Ib(1,2) = density * int_V -y*z dV
    //        Ib(2,2) = density * int_V x^2 + y^2 dV
    //
    // From matrix Ib we can compute the eigenvalues and eigenvectors 

    const int numberOfInertiaEntries = numberOfDimensions==2 ? 1 : 6;


    RealArray xCM(3);
    real t0=parameters.dbase.get<real>("tInitial");
    rigidBody.getPosition( t0,xCM );  // ** need time corresponding to the grid so integrals are correct

    const BodyDefinition & bodyDefinition = integrate->getBodyDefinition();
    const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(bodyNumber);
    Index Ib1,Ib2,Ib3;

    f=0.;

    const real density=rigidBody.getDensity();

    RealArray InertiaMatrix(3,3); // holds entries in inertia matrix
    InertiaMatrix=0.;
    for( int mi=0; mi<numberOfInertiaEntries; mi++ ) // moment of inertia entries
    {
      
      for( int face=0; face<numberOfFaces; face++ )
      {
	int side=-1,axis,grid;
	bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
	assert( side>=0 && side<=1 && axis>=0 && axis<numberOfDimensions);
	assert( grid>=0 && grid<cg.numberOfComponentGrids());

	MappedGrid & c = cg[grid];
	c.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal );
	  
	OV_GET_SERIAL_ARRAY(real,c.vertex(),xLocal);
	OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);
        #ifdef USE_PPP
          const realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
        #else
	  const realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
        #endif 

	int extra=1;  // do we need extra for periodic ? 
	getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // NB to use gridIndexRange, not indexRange

	int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);

        // **NOTE** There are many choices for converting the volume integral to the surface integral
        // Some choices are probably better conditioned than others *check me*

	// int_V x^2 + y^2 dV = int_V div.(x^3,y^3,0)/3  dV = int_S (1/3)*(x^3,y^3,0).nv dS
#define CUBE(x) ((x)*(x)*(x))
	if( ok )
	{
          // RV(dir) = vector from center-of-mass to a point on body surface 
          #define RV(dir) (xLocal(Ib1,Ib2,Ib3,dir)-xCM(dir))
	  if( numberOfDimensions==2 )
	  {
            // diagonal: x^2 + y^2 -> (1/3)( x^3, y^3, 0 )
	    fLocal(Ib1,Ib2,Ib3) = ( CUBE(RV(0))*normalLocal(Ib1,Ib2,Ib3,0) +
				    CUBE(RV(1))*normalLocal(Ib1,Ib2,Ib3,1)  )*(1./3);
	  }
	  else
	  {
	    if( mi==0 )
	    {
              // diagonal: y^2 + z^2 -> (1/3)( 0, y^3, z^3 )
	      fLocal(Ib1,Ib2,Ib3) = ( CUBE(RV(1))*normalLocal(Ib1,Ib2,Ib3,1) +
				      CUBE(RV(2))*normalLocal(Ib1,Ib2,Ib3,2)  )*(1./3);
	    }
	    else if( mi==1 )
	    {
              // -x*y  ->  ( 0, 0, -x*y*z )
	      fLocal(Ib1,Ib2,Ib3) = -RV(0)*RV(1)*RV(2)*normalLocal(Ib1,Ib2,Ib3,2);
	    }
	    else if( mi==2 )
	    {
              // -x*z  -> ( 0, -x*y*z, 0 )
	      fLocal(Ib1,Ib2,Ib3) = -RV(0)*RV(1)*RV(2)*normalLocal(Ib1,Ib2,Ib3,1);
	    }
	    else if( mi==3 )
	    {
              //diagonal:  x^2 + z^2 -> (1/3)( x^3, 0, z^3 )
	      fLocal(Ib1,Ib2,Ib3) = ( CUBE(RV(0))*normalLocal(Ib1,Ib2,Ib3,0) +
				      CUBE(RV(2))*normalLocal(Ib1,Ib2,Ib3,2)  )*(1./3);
	    }
	    else if( mi==4 )
	    {
              // -y*z  -> ( -x*y*z,0,0 )
	      fLocal(Ib1,Ib2,Ib3) = -RV(0)*RV(1)*RV(2)*normalLocal(Ib1,Ib2,Ib3,0);
	    }
	    else if( mi==5 )
	    {
              // diagonal: x^2 + y^2 -> (1/3)( x^3, y^3, 0 )
	      fLocal(Ib1,Ib2,Ib3) = ( CUBE(RV(0))*normalLocal(Ib1,Ib2,Ib3,0) +
				      CUBE(RV(1))*normalLocal(Ib1,Ib2,Ib3,1)  )*(1./3);
	    }
	    else 
	    {
	      OV_ABORT("error !?");
	    }
	  }
	 
	} // end if ok 
	
      }  // end for face
      

      // The surface integral should be negative since the outward normal to the body is minus the
      // outward normal to the grid.
	real moi = -( integrate->surfaceIntegral(f,bodyNumber) );
      printF("--MVG--CRBP-- INFO: Body %i : moment of inertia integral = %10.4e \n",bodyNumber,moi);
	
      if( numberOfDimensions==2 )
      {
	InertiaMatrix(0,0)=1.;
	InertiaMatrix(1,1)=1.;
	InertiaMatrix(2,2)=density*moi;
	printF("--MVG--CRBP-- INFO: Body %i density=%9.3e : InertiaMatrix(2,2) = %12.5e\n",
	       bodyNumber,density,InertiaMatrix(2,2));
      }
      else
      {
	if( mi==0 )
	{
	  InertiaMatrix(0,0)=density*moi;
	}
	else if( mi==1 )
	{
	  InertiaMatrix(0,1)=InertiaMatrix(1,0)=density*moi;
	}
	else if( mi==2 )
	{
	  InertiaMatrix(0,2)=InertiaMatrix(2,0)=density*moi;
	}
	else if( mi==3 )
	{
	  InertiaMatrix(1,1)=density*moi;
	}
	else if( mi==4 )
	{
	  InertiaMatrix(1,2)=InertiaMatrix(2,1)=density*moi;
	}
	else if( mi==5 )
	{
	  InertiaMatrix(2,2)=density*moi;
	}
	else 
	{
	  OV_ABORT("error !?");
	}

      }
      
    } // end for mi 
    
    RealArray mI(3);
    if( numberOfDimensions==3 )
    {
      // compute eigenvalues and eigenvectors of Inertia matrix:
      OV_ABORT("finish me for 3D");
    }
    else
    {
      mI(0)=1.; mI(1)=1.; mI(2)=InertiaMatrix(2,2);
    }
       
    // set moments of inertia
    rigidBody.setMomentsOfInertia(mI(0),mI(1),mI(2));


  } // end compute moments of inertia
      
  return 0;
}


// =================================================================================================
/// \brief Define moving grid properties interactively.
/// \details This routine is used to define how bodies move. A moving body may be
///  constructed with multiple grids (e.g. a sphere may be covered with 2 or three patches). 
///   There are two main steps to define a motion of a body
///     - define the motion of a body
///     - define which grids belong to the body
///   The types of motions are
///     - "matrix motions" define predetermined rigid motions and are given by x(t) = R(t) x(0) + g(t)
///       where R(t) is a "rotation" (or scaling) matrix and g(t) is a translation. 
///     - "rigid body motions" define motions of rigid bodies whose position depend on the forces
///        and torques imposed by the fluid.
///     - "deforming body motions" define bodies that can move and deform (either specified deformations or
///       deformations that depend on the fluid). 
///     - "user defined motion" are motions defined by the user in the file UserDefinedMotion.C
// =================================================================================================
int MovingGrids::
update( CompositeGrid & cg, GenericGraphicsInterface & gi )
{

  if( ! isInitialized )
  {
    assert( cg.numberOfDimensions()==parameters.dbase.get<int >("numberOfDimensions") );
    initialize();
  }

  // movingGrid(grid) is true if a grid is moving
  if( movingGrid.getLength(0)<cg.numberOfComponentGrids() )
  {
    movingGrid.redim(cg.numberOfComponentGrids());        
    movingGrid=false;
    // type of movement:
    moveOption.redim(cg.numberOfComponentGrids());
    moveOption=-1;
  
    // moveParameters(.,grid) : parameters for a moving grid
    moveParameters.redim(8,cg.numberOfComponentGrids());        
    moveParameters=false;
  }

  bool & improveQualityOfInterpolation = parameters.dbase.get<bool >("improveQualityOfInterpolation");
  real & interpolationQualityBound = parameters.dbase.get<real >("interpolationQualityBound");
  real & maximumAngleDifferenceForNormalsOnSharedBoundaries = parameters.dbase.get<real >("maximumAngleDifferenceForNormalsOnSharedBoundaries");;

  MovingGridOption movingGridOption=notMoving;
  bool & printMovingBodyInfo = parameters.dbase.get<bool>("printMovingBodyInfo");
  
  GUIState gui;
  gui.setWindowTitle("Moving Grids");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString pbLabels[] = {"added mass options..."
                          "help",
    			  ""};

    int numRows=4;
    dialog.setPushButtons( pbLabels, pbLabels, numRows ); 

    dialog.setOptionMenuColumns(1);
    aString motionOptions[] = {"no motion",
                               "matrix motion",  
			       "rigid body",
			       "deforming body",
			       "user defined",
			       "unknown or deprecated",
			       "" };

    dialog.addOptionMenu("Motion:",motionOptions,motionOptions, (movingGridOption==notMoving             ? 0 :
								 movingGridOption==matrixMotion          ? 1 :
								 movingGridOption==rigidBody             ? 2 :  
								 movingGridOption==deformingBody         ? 3 :  
								 movingGridOption==userDefinedMovingGrid ? 4 : 5
			   ));
    // GUIState::addPrefix(motionOptions,"Motion:",cmd,maxCommands);
    // dialog.addOptionMenu("Motion:",cmd,cmd,movingGridOption );


    aString tbCommands[] = {"use hybrid grid for surface integrals",
                            "improve quality of interpolation",
                            "print moving body info",
                            "recompute grid velocity on correction",
			    // "limit forces",
    			    ""};
    int tbState[10];
    tbState[0] = useHybridGridsForSurfaceIntegrals;
    tbState[1] = improveQualityOfInterpolation;
    tbState[2] = printMovingBodyInfo;
    tbState[3] = recomputeGridVelocityOnCorrection;
    
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "interpolation quality:"; sPrintF(textStrings[nt], "%g",interpolationQualityBound);  nt++; 
    textLabels[nt] = "shared normal tolerance:"; sPrintF(textStrings[nt], "%g",maximumAngleDifferenceForNormalsOnSharedBoundaries);  nt++; 
    textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",debug0);  nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    // dialog.setTextBoxes(cmd, textLabels, textStrings);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  }


  aString answer,answer2;
  int grid;
  
  aString *gridMenu = new aString [cg.numberOfComponentGrids()+3];
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    gridMenu[grid]=cg[grid].getName();
  gridMenu[cg.numberOfComponentGrids()+0]="all";
  gridMenu[cg.numberOfComponentGrids()+1]="none";
//  gridMenu[cg.numberOfComponentGrids()+2]="specify faces";
//  gridMenu[cg.numberOfComponentGrids()+3]="choose grids by share flag";
//  gridMenu[cg.numberOfComponentGrids()+4]="done";
  gridMenu[cg.numberOfComponentGrids()+2]="";

  // -- add *old* popup menu
  const aString moveMenu[]=
    {
      // "matrix motion",   // new general way 
      "oscillate",       // old - deprecate at some point
      "rotate",          // old - deprecate at some point
      "translate",       // old - deprecate at some point
      "scale",           // old - deprecate at some point
      // "rigid body",
      // "deforming body",
      // "user defined",
      ">options",
      // "use hybrid grid for surface integrals",
      // "do not use hybrid grid for surface integrals",
      // "improve quality of interpolation",               // *fix me* these should be saved with the grid by Ogen
      // "interpolation quality bound",
      // "default shared boundary normal tolerance",
      "limit forces",
      // "debug",
      "<done",
      ""
    }; 

  gui.buildPopup(moveMenu);

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("move>");

  int len=0;
  for( ;; ) // Pick MovingGrid type
  {
	    
    // int response=gi.getMenuItem(moveMenu,answer2,"Choose the movement");

    gi.getAnswer(answer2,"");
  
    // printF(answer2,"answer=[answer]\n",(const char *)answer2);

    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);

    RealArray par(moveParameters.getLength(0));
    par=0.;
    
    if( answer2=="done" || answer2=="exit" )
    {
      break;
    }
    else if( answer2=="no motion" )
    {
      movingGridOption=notMoving;
    }
    
    else if( answer2=="matrix motion" )
    {
      // --- matrix motion ---
      printF("MovingGrids::update:INFO: For more information on `matrix motion' see the documentation in movingBodies.pdf\n");

      movingGridOption=matrixMotion;
      if( numberOfMatrixMotionBodies==0 )
      {
	matrixMotionBody = new MatrixMotion* [cg.numberOfComponentGrids()];  // allocate a pointer for each grid for now
      }
	
      matrixMotionBody[numberOfMatrixMotionBodies]=new MatrixMotion; 
      matrixMotionBody[numberOfMatrixMotionBodies]->incrementReferenceCount();

      // allow user to define the properties of the motion
      matrixMotionBody[numberOfMatrixMotionBodies]->update(gi);

      par(0)=numberOfMatrixMotionBodies; // *wdh* 110607
      numberOfMatrixMotionBodies++;

    }
    else if( answer2=="oscillate" )
    {
      printF("Oscillation: x(t) = x(0) + tangent { [ 1-cos( (t-t0)*(omega *2*pi) ) ]*amplitude }\n");
      movingGridOption=oscillate;
      real x0=0., y0=0., z0=0.;
      gi.inputString(answer2,"Enter the tangent of the line of oscillation");
      if( answer2!="" )
      {
	if( cg.numberOfDimensions()==2 )
	  sScanF(answer2,"%e %e",&x0,&y0);
	else
	  sScanF(answer2,"%e %e %e",&x0,&y0,&z0);
      }
      par(0)=x0;
      par(1)=y0;
      par(2)=z0;
	      
      real omega=1.;
      gi.inputString(answer2,"Enter omega, the number of oscillations per second");
      if( answer2!="" )
	sScanF(answer2,"%e",&omega);
      par(3)=omega;

      real amplitude=.5;
      gi.inputString(answer2,"Enter amplitude, the amplitude of the oscillation");
      if( answer2!="" )
	sScanF(answer2,"%e",&amplitude);
      par(4)=amplitude;

      real t0=0.;
      gi.inputString(answer2,"Enter t0, the origin of the oscillation");
      if( answer2!="" )
	sScanF(answer2,"%e",&t0);
      par(5)=t0;
    }
    else if( answer2=="rotate" )
    {
      movingGridOption=rotate;
      if( cg.numberOfDimensions()==2 )
      {
	gi.inputString(answer2,"Enter the point to rotate about");
	real x0=0., y0=0.;
	if( answer2!="" )
	  sScanF(answer2,"%e %e",&x0,&y0);
	par(0)=x0;
	par(1)=y0;
      }
      else
      {
	assert( par.getLength(0)>=7 );
	
	real x0=0., y0=0., z0=0.;
	gi.inputString(answer2,"Enter a point on the line to rotate about");
	if( answer2!="" )  
	  sScanF(answer2,"%e %e %e",&x0,&y0,&z0);
	par(0)=x0;
	par(1)=y0;
	par(2)=z0;

	x0=0.; y0=0.; z0=0.;
	gi.inputString(answer2,"Enter the tangent of the line to rotate about");
	if( answer2!="" )
	  sScanF(answer2,"%e %e %e",&x0,&y0,&z0);
	par(3)=x0;
	par(4)=y0;
	par(5)=z0;
	printF("tangent = (%8.2e,%8.2e,%8.2e).\n",par(3),par(4),par(5));
      }
      real omega=1.;
      real rampInterval=-1.;
      printF(" INFO: The rampInterval is the time over which the motion is ramped up slowly.\n"
             "       Choose a value of -1 for no ramp (implusive start).\n");
      gi.inputString(answer2,"Enter the number of rotations per second and the ramp interval");
      if( answer2!="" )
	sScanF(answer2,"%e %e ",&omega,&rampInterval);
      par(6)=omega;
      par(7)=rampInterval;
	      
    }
    else if( answer2=="translate" )
    {
      movingGridOption=shift;
      real x0=0., y0=0., z0=0.;
      gi.inputString(answer2,"Enter the tangent of the line of translation");
      if( answer2!="" )
      {
	if( cg.numberOfDimensions()==2 )
	  sScanF(answer2,"%e %e",&x0,&y0);
	else
	  sScanF(answer2,"%e %e %e",&x0,&y0,&z0);
      }
      real norm = SQRT( x0*x0+y0*y0+z0*z0 );
      if( norm==0. )
	norm=1.;
      par(0)=x0/norm;
      par(1)=y0/norm;
      par(2)=z0/norm;
      printF("The normalized tangent vector is (%9.3e,%9.3e,%9.3e)\n",par(0),par(1),par(2));
      
      real speed=1.;
      gi.inputString(answer2,"Enter the speed of translation");
      if( answer2!="" )
	sScanF(answer2,"%e",&speed);
      par(3)=speed;
    }
    else if( answer2=="scale" )
    {
      printF("INFO: The grid is scaled by 1+scaleFactor*t in each direction\n");
      movingGridOption=scale;
      real sx=0., sy=0., sz=0.;
      gi.inputString(answer2,"Enter the scale factors in each direction sx,sy,sz");
      if( answer2!="" )
      {
	if( cg.numberOfDimensions()==2 )
	  sScanF(answer2,"%e %e",&sx,&sy);
	else
	  sScanF(answer2,"%e %e %e",&sx,&sy,&sz);
      }
      par(0)=sx;
      par(1)=sy;
      par(2)=sz;
      printF("Using scale factors (%9.3e,%9.3e,%9.3e)\n",par(0),par(1),par(2));
    }
    else if( answer2=="rigid body" )
    {
      movingGridOption=rigidBody;

      if(  numberOfRigidBodies==0 )
      { 
        // -- first rigid body : create an Integrate object, used to compute surface integrals --

	body = new RigidBodyMotion* [cg.numberOfComponentGrids()];

        delete integrate;
        integrate = new Integrate(cg);
        aString & nameOfGridFile = parameters.dbase.get<aString>("nameOfGridFile");
        if( nameOfGridFile!="" )
	{
	  // Look for an Integrate object in the grid file -- the Integration coefficients may
          // have already been computed
	  HDF_DataBase db;
	  printF("MovingGrids::Look for an Integrate object in file=[%s]\n"
                 "  This object will hold pre-computed integration weights.\n",
                 (const char*)nameOfGridFile);
	  db.mount(nameOfGridFile,"R");

          const int maxNumberToFind=5;
	  aString name[maxNumberToFind]; 
	  int num, actualNumber;
	  num = db.find(name,"Integrate",maxNumberToFind,actualNumber);  

	  printF("There were %i Integrate objects in the file, name of first=[%s]\n",num,(const char*)name[0]);

	  if( num>0 )
  	    integrate->get( db,name[0] );
      
	  db.unmount();
          
	  if( false )
	  { // Testing... we should not need to recompute the integration weights...
            Integrate::debug=1;
	    realCompositeGridFunction u(cg);
	    u=1.;
	    real surfaceArea = integrate->surfaceIntegral(u);
	    printF("total surface area = %9.3e\n",surfaceArea);
	    Integrate::debug=0;
	  }

	}
	

        // printF(" +++++ MovingGrids: set useHybridGrids = %i \n",useHybridGridsForSurfaceIntegrals);
        integrate->useHybridGrids(useHybridGridsForSurfaceIntegrals);
	
      }
	
      body[numberOfRigidBodies]=new RigidBodyMotion( cg.numberOfDimensions());

      body[numberOfRigidBodies]->setBodyNumber( numberOfRigidBodies );  // *wdh* May 30, 2016
      body[numberOfRigidBodies]->setParameters( parameters );


      // set default initial conditions
      body[numberOfRigidBodies]->setInitialConditions(0.);

      // allow user to change the defaults
      body[numberOfRigidBodies]->update(gi);

      par(0)=numberOfRigidBodies;
      numberOfRigidBodies++;
      
    }
    else if( answer2=="deforming body" )
    {
      movingGridOption=deformingBody;

      if( deformingBodyList == NULL )
      {
	deformingBodyList = new DeformingBodyMotion* [cg.numberOfComponentGrids()];
      }

      if( debug() & 2 ) 
        printF("++MovingGrids::Appending to the DeformingBodyList, b=%i\n", numberOfDeformingBodies);
      deformingBodyList[ numberOfDeformingBodies ] =
	new DeformingBodyMotion(parameters, 3, NULL, debug() ); // #timelevels, *gi, debug

      // *wdh* 050102 - interactively choose deforming body options
      deformingBodyList[ numberOfDeformingBodies ]->update(cg,gi);

	// We may not always need to integrate -- fix this ---
      if (integrate == NULL)  integrate = new Integrate(cg);


      //deformingBodyList[numberOfDeformingBodies]->update(gi); //later
      par(0)=numberOfDeformingBodies;
      numberOfDeformingBodies++;
    }
    else if( answer2=="user defined" )
    {
      movingGridOption=userDefinedMovingGrid;
      updateUserDefinedMotion(cg,gi); 

    }
    else if( answer2=="ramp" )
    {
       gi.inputString(answer2,"Enter the tangent of the line of translation");
       continue;   // skip choosing a grid below
    }
    else if( dialog.getToggleValue(answer2,"print moving body info",printMovingBodyInfo)  )
    {
      continue;   // skip choosing a grid below
    }
    else if( answer2=="use hybrid grid for surface integrals" ||
             answer2=="do not use hybrid grid for surface integrals" )
    {
      useHybridGridsForSurfaceIntegrals= answer2=="use hybrid grid for surface integrals";
      printF("Moving grids: set useHybridGridsForSurfaceIntegrals=%i \n",useHybridGridsForSurfaceIntegrals);
      
      if( integrate!=NULL )
        integrate->useHybridGrids(useHybridGridsForSurfaceIntegrals);
       continue;   // skip choosing a grid below
    }
    else if( answer2=="improve quality of interpolation" )
    {
      improveQualityOfInterpolation=true;
      printF("improveQualityOfInterpolation=%i\n",improveQualityOfInterpolation);
      continue;   // skip choosing a grid below
    }
    else if( answer2=="interpolation quality bound" )
    {
      real quality = parameters.dbase.get<real >("interpolationQualityBound")>1. ? parameters.dbase.get<real >("interpolationQualityBound") : 2.;
      
      gi.inputString(answer2,sPrintF("Enter the interpolation quality bound (>1) (current=%5.2f)",quality));

      sScanF(answer2,"%e",&quality); 
      if( quality>1. )
      {
        parameters.dbase.get<real >("interpolationQualityBound")=quality;
	printF(" setting interpolationQualityBound=%5.2f\n",parameters.dbase.get<real >("interpolationQualityBound"));
      }
      else
      {
        printF(" setting interpolationQualityBound= default\n");
      }
      continue;   // skip choosing a grid below
    }
    else if( answer2=="default shared boundary normal tolerance" )
    {
      printF("This is a tolerance used by Ogen\n");
      real & tol = parameters.dbase.get<real >("maximumAngleDifferenceForNormalsOnSharedBoundaries");
      
      gi.inputString(answer2,sPrintF("Enter the tolerance (between 0 and 1) (current=%8.2e)",tol));

      sScanF(answer2,"%e",&tol); 
      printF("Setting default shared boundary normal tolerance=%5.2f\n",tol);
      continue;   // skip choosing a grid below
    }
    else if( answer2=="limit forces" )
    {
      limitForces=true;
      gi.inputString(answer2,"Enter the maximum values for the force and torque");
      sScanF(answer2,"%e %e",&maximumAllowableForce,&maximumAllowableTorque);
      printF(" Setting maximumAllowableForce=%8.2e, maximumAllowableTorque=%8.2e\n",
	    maximumAllowableForce, maximumAllowableTorque);

      continue;   // skip choosing a grid below
    }
    else if( answer2=="debug" )
    {
      gi.inputString(answer2,"Enter the value for the debug (bit flag, e.g. 1 or 3 or 7 or 15)");
      sScanF(answer2,"%i",&debug0);
      continue;   // skip choosing a grid below
    }

    // ********************* new way ****************************

    else if( answer2=="help" )
    {
      printF("INFO: To specify a moving or deforming body one first must choose the motion type of the body (e.g. rigid).\n"
             "  One then chooses the grids that have boundary faces adjacent to body. In general there are multiple\n"
             "  grid faces adjacent to the body (i.e. from multiple overlapping grids). Perhaps the easiest way to choose the different grids\n"
             "  is choose a `shared flag' value (assuming the overlapping grid was constructed with a unique share flag for boundaries\n"
             "  adjacent to the body).\n");
      continue;   // skip choosing a grid below
    }
    else if( dialog.getTextValue(answer2,"interpolation quality:","%g",interpolationQualityBound) )
    {
      if( interpolationQualityBound>1. )
      {
	printF("--MVG-- Setting interpolationQualityBound=%5.2f .\n",interpolationQualityBound);
      }
      else
      {
	interpolationQualityBound=2.;
        printF("--MVG-- Setting interpolationQualityBound=%g (default).\n",interpolationQualityBound);
      }
      continue;   // skip choosing a grid below
    }
    else if( dialog.getTextValue(answer2,"shared normal tolerance:","%g",maximumAngleDifferenceForNormalsOnSharedBoundaries) )
    {
      printF("--MVG-- This tolerance should be between 0 and 1, and is a relative measure of the maximum angle difference between\n"
             " normals on two shared boundaries. A value of zero means the normals must match exactly.\n");
      printF("--MVG-- Setting default shared boundary normal tolerance=%5.2f\n",maximumAngleDifferenceForNormalsOnSharedBoundaries);
      continue;   // skip choosing a grid below
    }
    else if( dialog.getTextValue(answer2,"debug:","%i",debug0) )
    {
      continue;   // skip choosing a grid below
    } 

    else if( dialog.getToggleValue(answer2,"use hybrid grid for surface integrals",useHybridGridsForSurfaceIntegrals) )
    {
      if( useHybridGridsForSurfaceIntegrals )
      {
	printF("--MVG-- useHybridGridsForSurfaceIntegrals=%i\n",(int)useHybridGridsForSurfaceIntegrals);
	if( integrate!=NULL )
	  integrate->useHybridGrids(useHybridGridsForSurfaceIntegrals);
      }
      
      continue;   // skip choosing a grid below
    }
    else if( dialog.getToggleValue(answer2,"improve quality of interpolation",improveQualityOfInterpolation) )
    {
      if( improveQualityOfInterpolation )
        printF("--MVG-- improveQualityOfInterpolation=%i\n",improveQualityOfInterpolation);
      continue;   // skip choosing a grid below
    }

    else if( dialog.getToggleValue(answer2,"recompute grid velocity on correction",recomputeGridVelocityOnCorrection) )
    {
      printF("--MVG-- recomputeGridVelocityOnCorrection%i : 1=recompute gridVelocity after each moving grid "
             "correction step.\n", (int)recomputeGridVelocityOnCorrection);

      continue;   // skip choosing a grid below
    }

    else
    {
      printF("MovingGrids:update: unknown response=[%s]\n",(const char*)answer2);
      gi.stopReadingCommandFile();
    }



    // *********************************************************************************************
    // *********************************************************************************************
    // ************  Given the motion type --> choose the grids to move ****************************
    // *********************************************************************************************
    // *********************************************************************************************

    //
    // NOTE: multiple grids can contribute faces to the same rigid/deforming body, e.g. a sphere with two patches


    // ********************* choose grids dialog ********************************
    GUIState chooseGridsGui;
    chooseGridsGui.setWindowTitle("Choose grids for body");
    chooseGridsGui.setExitCommand("close choose grids", "close");
    DialogData & chooseGridsDialog = (DialogData &)chooseGridsGui;

    if( buildDialog )
    {
      aString pbCommands[] = {"specify faces",
			      "choose grids by share flag",
			      "done",
			      ""};
      aString *pbLabels = pbCommands;
      int numRows=3;
      chooseGridsDialog.setPushButtons( pbCommands, pbLabels, numRows ); 
    }
    chooseGridsGui.buildPopup(gridMenu);
    gi.pushGUI(chooseGridsGui);
    gi.appendToTheDefaultPrompt("choose grids>");


    aString motionName;
    if( movingGridOption==rigidBody ) 
      motionName="rigid body";
    else if( movingGridOption==deformingBody ) 
      motionName="deforming body";
    else
      motionName="?";


    int bodyNumber = (movingGridOption == deformingBody ? numberOfDeformingBodies-1 : 
                      movingGridOption == rigidBody ? numberOfRigidBodies-1 :
                      movingGridOption == matrixMotion ? numberOfMatrixMotionBodies-1 : -1 );

    // We build a list of faces that live on the rigid or deforming body 
    // *wdh* 110806 -- this next is not needed for matrixMotion:
    // bool buildBodyFaceList = movingGridOption==rigidBody || movingGridOption==deformingBody || movingGridOption == matrixMotion;
    bool buildBodyFaceList = movingGridOption==rigidBody || movingGridOption==deformingBody;
    IntegerArray boundary; 
    int numberOfFaces=0;
    if( buildBodyFaceList )
    {
      boundary.redim(3,cg.numberOfComponentGrids()*cg.numberOfDimensions()*2);
    }
    if( movingGridOption==rigidBody )
    {
      assert( integrate!=NULL );
    }
    
    for( ;;) // choose grids to move
    {
      Range G; // moved here to be accessible from def body init code

      grid=gi.getAnswer(answer,"Move which grid(s)?");
      // grid=gi.getMenuItem(gridMenu,answer,"Move which grid(s)?");

      if( grid<cg.numberOfComponentGrids() )
        printF("MovingGrids:: choosing moving grid = %i\n",grid);

      if( answer=="none" || answer=="done" ||  answer=="close choose grids" )
      {
	break;
      }
      else if( grid<0 )
      {
	printF("MovingGrids::unknown response=[%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
	continue;
      }
      else if( answer=="choose grids by share flag" )
      {
	int share=0;
	gi.inputString(answer2,"Enter the share flag value");
	sScanF(answer2,"%i",&share);
        if( share!=0 )
	  printF("MovingGrids::Adding any grids with a share flag=%i\n",share);
	else
	{
	  printF("MovingGrids::You must specify a non-zero share flag!\n");
	  continue;
	}
	
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ ) 
	{
          const IntegerArray & sharedBoundaryFlag = cg[grid].sharedBoundaryFlag();
          bool gridWasAdded=false; // only add one face from a grid 
	  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	    {
              if( sharedBoundaryFlag(side,axis)==share )
	      {
		if( !gridWasAdded )
		{ 
		  gridWasAdded=true;
		  if( buildBodyFaceList ) 
		  { // add this face to the rigid/deforming body
		    printF("MovingGrids::assign body faces: (side,axis)=(%i,%i) of grid %s being added to body %i\n",
			   side,axis,(const char*)cg[grid].getName(),bodyNumber);
		    boundary(0,numberOfFaces)=side;
		    boundary(1,numberOfFaces)=axis;
		    boundary(2,numberOfFaces)=grid;
		    numberOfFaces++;
		  }
		  else
		  {
		    printF("MovingGrids:: grid %s will be moved.\n", (const char*)cg[grid].getName());
		  }
		  
                  Range G(grid,grid);
		  movingGrid(G)=true;
		  moveOption(G)=movingGridOption;
		  Range all;
		  for( int g=G.getBase(); g<=G.getBound(); g++ )
		    moveParameters(all,g)=par(all);

		}
		else
		{
		  printF("MovingGrids::ERROR: There is another face on this grid with the same share flag!\n"
			 " This is not supported\n");
                  Overture::abort("MovingGrids::ERROR");
		}
	      }
	    }
	  }
	}
	continue;
      }
      else if( answer=="specify faces" )
      {
	// --- remove faces from the list faces that define the current rigid body ---
        //     This is needed in special cases where too many faces are added from a grid

	for( ;; )
	{
	  printF("Here are the faces of the %s:\n",(const char*)motionName);
	  for( int f=0; f<numberOfFaces; f++ )
	  {
	    printF(" face=%i:   grid=%i (side,axis)=(%i,%i) \n",f,boundary(2,f),boundary(0,f),boundary(1,f));
	  }

	  gi.inputString(answer,sPrintF(answer2,"Enter a face to delete (in the range [%i,%i]) or `done'\n",
                                        0,numberOfFaces-1));
	  if( answer=="done" ) break;
	  
          int face=-1;
          sScanF(answer,"%i",&face);
	  if( face>=0 && face<numberOfFaces )
	  {
	    printF("...deleting face=%i: grid=%i (side,axis)=%i \n",face,boundary(2,face),boundary(0,face),
                                                                    boundary(1,face));
            Range F0(face,numberOfFaces-2), F1(face+1,numberOfFaces-1),all;
	    
	    boundary(all,F0)=boundary(all,F1);
	    numberOfFaces-=1;
	  }
	  else
	  {
	    printF("Invalid face chosen, face=%i -- must be in the range [%i,%i]\n",face,0,numberOfFaces-1);
	  }
	}

	continue;  // I think we need this, *wdh* 080221 
      }
      else
      {
	//Range G; // I would like to make this accessible by the def body init code below
	if( answer=="all" )
	{
	  G=Range( 0,cg.numberOfComponentGrids()-1);
	  printF("Choosing all %i grids to move.\n",cg.numberOfComponentGrids());
	}
	else
	{
	  G=Range(grid,grid);
	}
	  
	movingGrid(G)=true;
        moveOption(G)=movingGridOption;
        Range all;
        for( int g=G.getBase(); g<=G.getBound(); g++ )
          moveParameters(all,g)=par(all);

      }

      if( buildBodyFaceList )  
      {
        // ------------------------------------------------------------------------
        // --- Add boundary faces of "grid" to the current rigid/deforming body ---
        // ------------------------------------------------------------------------

        // NOTE: multiple grids can contribute faces to the same body, e.g. a sphere with two patches

        for( int grid=G.getBase(); grid<=G.getBound(); grid++ )  // should only handle grids that were chosen
	{
          const IntegerArray & boundaryCondition = cg[grid].boundaryCondition();
	  // if( moveOption(grid)==(int)rigidBody && moveParameters(0,grid)==surface )
	  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    for( int side=Start; side<=End; side++ )
	    {
	      // if( boundaryCondition(side,axis)>100 )
	      // 	printF("MovingGrids::assign body faces: skipping side,axis)=(%i,%i) of grid %s since bc>100\n"
	      // 	       "this is a fudge until we fix the problem of adjacent sides\n",
	      // 	       side,axis,(const char*)cg[grid].getName());
		
	      if( boundaryCondition(side,axis)>0 )
		//  boundaryCondition(side,axis)<=100 ) // this is a fudge until we fix the problem of adjacent sides
	      {
		printF("MovingGrids::assign body faces: (side,axis)=(%i,%i) of grid %s being added to body %i\n",
		       side,axis,(const char*)cg[grid].getName(),bodyNumber);
		boundary(0,numberOfFaces)=side;
		boundary(1,numberOfFaces)=axis;
		boundary(2,numberOfFaces)=grid;
		numberOfFaces++;

		for( int dir=1; dir<cg.numberOfDimensions(); dir++)
		{
		  int axisp = (axis+dir) % cg.numberOfDimensions();
		  for( int side2=Start; side2<=End; side2++ )
		  {
		    if( boundaryCondition(side2,axisp)>0 ) // && boundaryCondition(side2,axisp)<=100 )
		    {
		      printF("MovingGrids::WARNING: setting faces for a body, grid=%i (%s)\n"
			     " The face (side,axis,bc)=(%i,%i,%i) has an adjacent face (side,axisp,bc)=(%i,%i,%i)\n"
			     " A corner on a body will not be treated correctly since the normal\n"
			     " is not well defined. Use `specify faces' to choose the faces to use\n",
			     grid,(const char*)cg[grid].getName(),
			     side,axis, boundaryCondition(side,axis),
			     side2,axisp, boundaryCondition(side2,axisp));
		    }
		  }
		}
	      }
	    }
	  }
	}

        if(numberOfFaces==0 )
	{
	  printF("MovingGrids::ERROR: No valid faces found for a %s! \n",(const char*)motionName);
	  Overture::abort("error");
	}

//  	integrate->defineSurface( surface,numberOfFaces,boundary ); 
//          if( true )
//  	{
//            realCompositeGridFunction f(cg);
//  	  f=1.;
//  	  real surfaceArea=integrate->surfaceIntegral(f,surface);
//  	  printF(">>> MovingGrids:Surface area computed by Integrate for surface=%i is %10.4e \n",surface,surfaceArea);
//  	}
	

      }
    } // end for(;;) -- choosing grid names


    gi.popGUI();
    gi.unAppendTheDefaultPrompt();


    if( movingGridOption == deformingBody ) 
    {
      // ----------------------------------------------------
      // --- specify the faces that form a deforming body ---
      // ----------------------------------------------------

      int b= numberOfDeformingBodies-1;   // deformingBodyNumber
      assert( deformingBodyList[b] != NULL );
	    
      for( int f=0; f<numberOfFaces; f++ )
      {
	const int side=boundary(0,f);
	const int axis=boundary(1,f);
	const int grid=boundary(2,f);
	printF("--MVG-- Add face %i (side,axis,grid)=(%i,%i,%i) to deforming body %i\n",f,side,axis,grid,b);

	// --- save the deforming body number in the BoundaryData so that we can look up information such 
	//     as added-mass information

        BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid); // this will create the BDA if it is not there
	std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");

	BoundaryData & bd = boundaryDataArray[grid];

	if( !bd.dbase.has_key("deformingBodyNumber") )
	{
          printF("--MVG--add deformingBodyNumber to boundaryDataArray[grid=%i].dbase\n",grid);

	  int (&deformingBodyNumber)[2][3] = bd.dbase.put<int[2][3]>("deformingBodyNumber");
	  for( int s=0; s<=1; s++) for( int a=0; a<3; a++ ){ deformingBodyNumber[s][a]=-1; } // 
	    
	}
	int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");
	deformingBodyNumber[side][axis]=b;

      }
	    
      deformingBodyList[b]->defineBody( numberOfFaces,boundary );

      real time00=0.;   // DEBUG -- (assumed) INITIAL TIME  ** fix me ***
      real dt00= 1e-4;  // DEBUG -- (assumed) reasonable dt for getting initial timelevel grids
	    
      deformingBodyList[b]->initialize( cg,time00 );

      deformingBodyList[b]->update(gi); // *wdh* why is this here ? 
	    
      // 	  int numThisBodyGrids=deformingBodyList[b]->getNumberOfComponentGrids(); 
      // 	  assert(numThisBodyGrids!=0); // should have at least the present component='grid'
      // 	  assert(numThisBodyGrids==1); // Full case (several grids) not impl. yet
	    
      // INITIALIZE THE GRIDS at past time levels
      if( FALSE )
      {
	// *** THIS HAS BEEN MOVED TO assignInitialConditions ****
	if( debug() & 4 ){ printF("INITIALIZING deformingGrid, past time levels\n"); }

	deformingBodyList[b]->initializePast( time00, dt00, cg);
      }
      
    }
      

    if( movingGridOption==rigidBody ) 
    {
      // Now add all the faces to the Integrate class for integrating forces on the boundary
      if( numberOfFaces==0 )
      {
	printF("MovingGrids::ERROR: No valid faces found for a rigid body! \n");
	Overture::abort("error");
      }
      const int bodyNumber = numberOfRigidBodies-1;

      integrate->defineSurface( bodyNumber,numberOfFaces,boundary ); 
      

      // --- Compute properties of the rigid body that have not been specified by the user ----
      //   (1) center of mass
      //   (2) Mass of the body given the density
      //   (3) Moment of inertia matrix given the density
      computeRigidBodyProperties(  bodyNumber, cg );

      // *** OLD ***
      // realCompositeGridFunction f(cg);   // *************** fix me *****************
      // f=1.;
      // real surfaceArea=integrate->surfaceIntegral(f,bodyNumber);
      // printF(">>> MovingGrids:Surface area computed by Integrate for body=%i is %10.4e \n",bodyNumber,surfaceArea);

      
      // // Precompute the initial centre of mass for any Rigid Body that was not given a center of mass,
      // // assuming a uniform density.


      // if( !(body[bodyNumber]->centerOfMassHasBeenInitialized()) )
      // {
      //   const BodyDefinition & bodyDefinition = integrate->getBodyDefinition();

      // 	RealArray x0(3);
      // 	x0=0.;
      // 	int numberOfPointsOnBody=0;
	
      // 	//  get the grid(s) that form the body
      // 	const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(bodyNumber);
      //   Index Ib1,Ib2,Ib3;
      //   //  *************************************************************
      //   //  **** Compute the centre of mass using surface integrals  ****
      //   //  *************************************************************
      // 	for( int dir=0; dir<cg.numberOfDimensions(); dir++ )
      // 	{
      //     f=0.;
      // 	  for( int face=0; face<numberOfFaces; face++ )
      // 	  {
      // 	    int side=-1,axis,grid;
      // 	    bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
      // 	    assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
      // 	    assert( grid>=0 && grid<cg.numberOfComponentGrids());

      // 	    // printF("\nMovingGrids::detectCollisions: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
      // 	    //   b,face,side,axis,grid);

      // 	    MappedGrid & c = cg[grid];
      // 	    c.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
	  
      //       int extra=1;  // do we need extra for periodic ? 
      // 	    getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // NB to use gridIndexRange, not indexRange
      // 	    realArray & x = c.vertex();
      //       OV_GET_SERIAL_ARRAY(real,x,xLocal);
      //       OV_GET_SERIAL_ARRAY(real,f[grid],fLocal);
	    
      // 	    // f[grid](Ib1,Ib2,Ib3)=x(Ib1,Ib2,Ib3,dir);
      // 	    int includeGhost=1;
      // 	    bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);
      // 	    if( ok )
      // 	      fLocal(Ib1,Ib2,Ib3)=xLocal(Ib1,Ib2,Ib3,dir);
	  
      // 	  }
      //     x0(dir)=integrate->surfaceIntegral(f,bodyNumber)/surfaceArea;
      // 	}

    
      // 	printF(" **** INFO: body %i has a center of mass: (integrate) (%8.2e,%8.2e,%8.2e)\n",
      //           bodyNumber,x0(0),x0(1),x0(2));

      // 	body[bodyNumber]->setInitialCentreOfMass(x0);
      // }

      // if( !(body[bodyNumber]->massHasBeenInitialized()) && body[bodyNumber]->getDensity()>0   )
      // {
      //   // If the mass has not been given but the density has, we compute the mass

      //   // the volume of the body can be computed as a surface integral using the divergence theorem
      //   //    
      //   //  bodyVolume = int_V div.(x,0,0) dV = int_S (x,0,0).nv dS
      //   //     --> set f= (x,0,0).nv = x*n_x

      //   const BodyDefinition & bodyDefinition = integrate->getBodyDefinition();
      // 	const int numberOfFaces=bodyDefinition.numberOfFacesOnASurface(bodyNumber);
      //   Index Ib1,Ib2,Ib3;

      // 	f=0.;
      // 	for( int face=0; face<numberOfFaces; face++ )
      // 	{
      // 	  int side=-1,axis,grid;
      // 	  bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
      // 	  assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
      // 	  assert( grid>=0 && grid<cg.numberOfComponentGrids());

      // 	  // printF("\nMovingGrids::detectCollisions: body %i : face %i: (side,axis,grid)=(%i,%i,%i)\n",
      // 	  //   b,face,side,axis,grid);

      // 	  MappedGrid & c = cg[grid];
      // 	  c.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal );
	  
      //     #ifdef USE_PPP
      // 	    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
      // 	    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(c.vertex(),xLocal);
      // 	    const realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
      //     #else
      // 	    realSerialArray & fLocal = f[grid];
      // 	    const realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
      // 	    const realSerialArray & xLocal = c.vertex();
      // 	  #endif 

      //     int extra=1;  // do we need extra for periodic ? 
      //     getBoundaryIndex(c.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // NB to use gridIndexRange, not indexRange

      //     int includeGhost=1;
      //     bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,includeGhost);
      // 	  if( ok )
      // 	    fLocal(Ib1,Ib2,Ib3)=xLocal(Ib1,Ib2,Ib3,0)*normalLocal(Ib1,Ib2,Ib3,0); 

      // 	}

      //   // The surface integral should be negative since the outward normal to the body is minus the
      //   // outward normal to the grid.
      //   real bodyVolume = fabs( integrate->surfaceIntegral(f,bodyNumber) );
      // 	printF(" **** INFO: Body %i volume from surface-integral = %10.4e \n",bodyNumber,bodyVolume);
	
      //   if( numberOfFaces==1 )
      // 	{
      // 	  int side=-1,axis,grid;
      //     int face=0;
	  
      // 	  bodyDefinition.getFace(bodyNumber,face,side,axis,grid);
      // 	  Mapping & map = cg[grid].mapping().getMapping();
      // 	  if( map.getClassName()=="AnnulusMapping" )
      // 	  {
      // 	    AnnulusMapping & annulus = (AnnulusMapping &)map;
      // 	    real radius = annulus.innerRadius; 
      //       bodyVolume=Pi*radius*radius;

      // 	    printF(" **** INFO: Body %i is an annulus with radius=%10.4e, true volume = %10.4e \n",
      //               bodyNumber,radius,bodyVolume);

      // 	  }
      // 	}

      //   real density=body[bodyNumber]->getDensity();
      //   real mass= bodyVolume*density;
      // 	body[bodyNumber]->setMass(mass);
      // 	printF(" **** INFO: Body %i has volume=%9.3e, density=%9.3e; Setting mass=%9.3e\n",
      //           bodyNumber,bodyVolume,density,mass);
      // }
      
      
    } //end if rigid body

    
    
  }
  
  if( numberOfRigidBodies>0 )
  {
    rigidBodyInfo.redim(200,numberOfRigidBodyInfoNames,numberOfRigidBodies);
    rigidBodyInfoTime.redim(200);

  }
    
  delete [] gridMenu;

  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;
}



