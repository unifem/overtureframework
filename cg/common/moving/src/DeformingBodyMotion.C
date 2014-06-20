// ---------------------------------------------------------------------------------------
//  Main class for handling deforming bodies.
//
//
// Original version by Petri Fast. for elastic-filamanents  c. 2000? 
// Changes to support deforming grids by wdh. c. 2006
// Major updates by wdh. 2008
// ---------------------------------------------------------------------------------------

#include "DeformingBodyMotion.h"
#include "DeformingGrid.h"
#include "DeformingGridGenerationInformation.h"
#include "ElasticFilament.h"
#include "SplineMapping.h"
#include "FilamentMapping.h"
#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "NurbsMapping.h"
#include "DomainSolver.h"
#include "Parameters.h"
#include "ParallelUtility.h"
#include "Interpolate.h"
#include "BeamModel.h"
#include "NonlinearBeamModel.h"

namespace
{

enum UserDefinedDeformingBodyMotionEnum
{
  iceDeform,
  ellipseDeform,
  sphereDeform,
  advectBody,
  elasticShell,
  elasticBeam,
  nonlinearBeam,
  interfaceDeform,
  userDeformingSurface,  // deforming surface is defined by the function: userDefinedDeformingSurface
  linearDeform // for testing the elastic piston problem 
}; 
 
}

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

//==========================================================================================
/// \brief Constructor:  Set up grids, but do not initialize the mapping in the composite grid. 
///    And don't instantiate the `physics object' yet
/// \param params (input) : the DomainSolver Parameters object.
// ===================================================================================
DeformingBodyMotion::
DeformingBodyMotion( Parameters & params,
                     int numberOfTimeLevels,             /* =3  */
		     GenericGraphicsInterface *pGIDebug00, /* =NULL */ 
		     int debug00                           /* =0 */)
  : parameters(params)
{
  debug = debug00;
  if( debug & 2 ) 
    printF("\nDeformingBodyMotion -- Constructor called\n");

  // new way: add parameters to the data-base:
  deformingBodyDataBase.put<int>("numberOfDeformingGrids",0);

  deformingBodyDataBase.put<int>("numberOfFaces",0);
  deformingBodyDataBase.put<IntegerArray>("boundaryFaces");

  deformingBodyDataBase.put<int>("numberOfTimeLevels",numberOfTimeLevels);

  deformingBodyDataBase.put<DeformingBodyType>("deformingBodyType",elasticFilament);


  // These Mapping's will replace the Mapping's in the CompositeGrid's and allow for deformation. 
  // These will often be a HyperbolicMapping
  deformingBodyDataBase.put<vector<Mapping*> >("transform");
  // deformingBodyDataBase.put<int>("transformToUse",0);

  // The Mapping's will define the "surface" from which the volume mapping will be created.
  // This will often be a NurbsMapping. In 2D the surface will be a curve.
  deformingBodyDataBase.put<vector<Mapping*> >("surface");

  // These arrays hold the grid points that define the deforming "surface" 
  // These values may be interpolated to form a Mapping "surface" 
  deformingBodyDataBase.put<vector<RealArray*> >("surfaceArray",NULL);
  deformingBodyDataBase.put<vector<real*> >("surfaceArrayTime",NULL); // corresponding times

  // gridEvolution holds a time history of grids from which we can compute velocity and acceleration
  deformingBodyDataBase.put<vector<GridEvolution*> >("gridEvolution"); 
  GridEvolution::debug=debug;

  // We parameterize the boundary curve by chord-length or index (flor fluid structure problems
  //  we should parameterize by index so the grid points match)
  deformingBodyDataBase.put<int>("boundaryParameterization");
  deformingBodyDataBase.get<int>("boundaryParameterization")=NurbsMapping::parameterizeByChordLength; 
  
  // order of accuracy for the acceleration computation: 
  deformingBodyDataBase.put<int>("accelerationOrderOfAccuracy",1);
  // order of accurcy for the velocity computation
  deformingBodyDataBase.put<int>("velocityOrderOfAccuracy",1);


  // Allow the user to edit the hyperbolic grid parameters
  deformingBodyDataBase.put<bool>("changeHypeParameters",false);

  // Set this to true in "update" to query for past grid history
  deformingBodyDataBase.put<int>("providePastHistory",false);

  deformingBodyDataBase.put<real>("sub iteration convergence tolerance",1.0e-3);

  deformingBodyDataBase.put<real>("added mass relaxation factor",1.0);

  pElasticFilament = NULL;  //.. zero the physics objects

  debug=debug00;
  pGIDebug = pGIDebug00;
  if( pGIDebug != NULL )
  {
    pMapInfoDebug = new MappingInformation;
    pMapInfoDebug->graphXInterface = pGIDebug;
  } 
  else 
  {
    pMapInfoDebug = NULL;
  }

  //..DeformingGrid list -- just one componentGrid for now
  //..SHOULD USE a 'DeformingGridCollection'

  pDeformingGrid=NULL;

  pBeamModel = NULL;

  pNonlinearBeamModel = NULL;
}


DeformingBodyMotion::
~DeformingBodyMotion()
{
  delete(pElasticFilament);
  delete(pDeformingGrid);

  delete pMapInfoDebug;

  // for userDefined

  vector<Mapping*> & transform =deformingBodyDataBase.get<vector<Mapping*> >("transform");
  for( int i=0; i<transform.size(); i++ )
    delete transform[i];

  vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
  for( int i=0; i<surface.size(); i++ )
    delete surface[i];

  vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
  vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
  for( int i=0; i<surfaceArray.size(); i++ )
  {
    delete [] surfaceArray[i];
    delete [] surfaceArrayTime[i];
  }
  


  vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");
  for( int i=0; i<gridEvolution.size(); i++ )
    delete gridEvolution[i];

  if (pBeamModel)
    delete pBeamModel;

  if (pNonlinearBeamModel)
    delete pNonlinearBeamModel;
}

//
// -------------- Services
//

// ============================================================================================
/// \brief return the order of accuracy used to compute the acceleration.
// ============================================================================================
int DeformingBodyMotion::
getAccelerationOrderOfAccuracy() const
{
  return deformingBodyDataBase.get<int>("accelerationOrderOfAccuracy");
}

// ============================================================================================
/// \brief return the order of accuracy used to compute the velocity.
// ============================================================================================
int DeformingBodyMotion::
getVelocityOrderOfAccuracy() const
{
  return deformingBodyDataBase.get<int>("velocityOrderOfAccuracy");
}


// ============================================================================================
/// \brief set the order of accuracy used to compute the acceleration.
/// \param order (input) : a positive integer
// ============================================================================================
int DeformingBodyMotion::
setAccelerationOrderOfAccuracy( int order )
{
  deformingBodyDataBase.get<int>("accelerationOrderOfAccuracy")=order;
  return 0;
}

// ============================================================================================
/// \brief set the order of accuracy used to compute the velocity.
/// \param order (input) : a positive integer
// ============================================================================================
int DeformingBodyMotion::
setVelocityOrderOfAccuracy( int order )
{
  deformingBodyDataBase.get<int>("velocityOrderOfAccuracy")=order;
  return 0;
}

// ================================================================================================
/// \brief Define the deforming body in terms of faces on grids.
/// \param numberOfFaces (input) : 
/// \param boundaryFaces (input) : boundaryFaces(0:2,f) = (side,axis,grid) for face f 
// ================================================================================================
int DeformingBodyMotion::
defineBody( int numberOfFaces_, IntegerArray & boundaryFaces_ )
{
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

  numberOfFaces=numberOfFaces_;
  boundaryFaces.redim(3,numberOfFaces);
  boundaryFaces(Range(0,2),Range(numberOfFaces))=boundaryFaces_(Range(0,2),Range(numberOfFaces));

  
  numberOfDeformingGrids=numberOfFaces;  // assume this is true for now 
  

  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
    deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  if( numberOfFaces>1 &&
      ( deformingBodyType==elasticFilament ||
        (deformingBodyType==userDefinedDeformingBody && userDefinedDeformingBodyMotionOption==interfaceDeform) ) )
  {
    int sideToMove=boundaryFaces(0,0);
    int axisToMove=boundaryFaces(1,0);
    int gridToMove=boundaryFaces(2,0); 

    printF("\n ***DeformingBodyMotion:defineBody: Grid=%i has more than one face that deforms. This is not allowed.\n",gridToMove);
    printF("      ... specify fewer faces when defining the grid motion.\n");
    Overture::abort("error");
  }


  return 0;
}

// ===============================================================================================
/// \brief return the initial state (e.g. position, velocity, acceleration)
/// \details return the initial state of the deforming grid.
// ===============================================================================================

// return the initial state (position, velocity, acceleration)
int DeformingBodyMotion::
getInitialState( InitialStateOptionEnum stateOption, 
		 const real time, const int grid, MappedGrid & mg,
		 const Index &I1, const Index &I2, const Index &I3, 
		 realSerialArray & state )
{
  const DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  const UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
      deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  if( true || debug & 2 ) 
    printF("-- DBM-- DeformingBodyMotion::getInitialState: stateOption=%i, grid=%i, t=%9.3e\n",(int)stateOption,grid,time);

  const int numberOfDimensions = parameters.dbase.get<int>("numberOfDimensions");
  Range Rx=numberOfDimensions;
  if( deformingBodyType==elasticFilament )
  {
    if( stateOption==initialVelocity || stateOption==initialAcceleration )
    {
      state(I1,I2,I3,Rx)=0.;
    }
    else
    {
      OV_ABORT("getInitialState:ERROR: finish me");
    }
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    if( stateOption==initialVelocity )
    {
      if( userDefinedDeformingBodyMotionOption==elasticBeam )
      {
	printF("-- DBM --- DeformingBodyMotion::getInitialState: get initial velocity for the elasticBeam.\n");

        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
	OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);
	int i1,i2,i3;
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  pBeamModel->projectVelocity(xLocal(i1,i2,i3,0), 
				      xLocal(i1,i2,i3,1),
				      state(i1,i2,i3,0),
				      state(i1,i2,i3,1));
	}
      }
      else
      {
        // -- for now we only support constant initial grid velocity and grid acceleration --

	if( !deformingBodyDataBase.has_key("initialVelocity") )
	{
	  deformingBodyDataBase.put<real [3]>("initialVelocity");
	  real *v0 = deformingBodyDataBase.get<real [3]>("initialVelocity");
	  v0[0]=v0[1]=v0[2]=0.;
	}
	real *v0 = deformingBodyDataBase.get<real [3]>("initialVelocity");
	if( true || debug & 2 )
	  printF("-- DBM-- DeformingBodyMotion::getInitialState: Setting grid velocity to (%9.3e,%9.3e,%9.3e) at t=%9.3e.\n",v0[0],v0[1],v0[2],time);

	for( int axis=0; axis<numberOfDimensions; axis++ )
	  state(I1,I2,I3,axis)=v0[axis];
      }
      
    }
    else if( stateOption==initialAcceleration )
    {
      if( !deformingBodyDataBase.has_key("initialAcceleration") )
      {
	deformingBodyDataBase.put<real [3]>("initialAcceleration");
	real *a0 = deformingBodyDataBase.get<real [3]>("initialAcceleration");
	a0[0]=a0[1]=a0[2]=0.;
      }
      real *a0 = deformingBodyDataBase.get<real [3]>("initialAcceleration");
      for( int axis=0; axis<numberOfDimensions; axis++ )
        state(I1,I2,I3,axis)=a0[axis];

      if( true || debug & 2 )
	printF("-- DBM-- DeformingBodyMotion::getInitialState: Setting grid acceleration to (%9.3e,%9.3e,%9.3e) at t=%9.3e.\n",a0[0],a0[1],a0[2],time);

    }
    else
    {
      OV_ABORT("getInitialState:ERROR: finish me");
    }
  }
  else
  {
    OV_ABORT("getInitialState:ERROR: unknown deformingBodyType");
  }
  return 0;

}


// ==================================================================================
/// \brief Return the face number that corresponds to a given grid.
///        This is a protected routine. 
// ==================================================================================
int DeformingBodyMotion::
getFace( int grid ) const 
{
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  int face=-1;
  for( int f=0; f<numberOfFaces; f++ )
  {
    if( boundaryFaces(2,f)==grid )
    {
      face=f;
    }
  }
  return face;
}


// ===============================================================================================
/// \brief Return the grid velocity 
/// \details This routine uses the GridEvolution class to compute the grid velocity. The
/// GridEvolution class keeps a sequence of past grids and computes the time derivative 
/// of the grid motion using these grids. 
// ===============================================================================================
int DeformingBodyMotion::
getVelocity( const real time0, 
	     const int grid, 
	     CompositeGrid & cg,
	     realArray & gridVelocity)
{

  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);

  if( deformingBodyType==elasticFilament )
  {
    if(debug&2) 
      cout << "DeformingBodyMotion::getVelocity, grid=" <<  grid << " called\n";

    assert( pDeformingGrid != NULL );
    pDeformingGrid->getVelocity( time0, grid, cg, gridVelocity);
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    if(debug&2)
       printF("DeformingBodyMotion:getVelocity called for userDefinedDeformingBody for t=%9.3e\n",time0);

    Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];

    int extra=1; // evaluate the grid velocity at one ghost point too (is this needed?)
    getIndex(cg[grid].gridIndexRange(),I1,I2,I3,extra);
    
    vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");
    int face=getFace( grid );
    if( time0<=0 )
    {
      if( face>=0 && face<gridEvolution.size() && gridEvolution[face]->getNumberOfTimeLevels()>1 )
      {
        printF("DeformingBodyMotion::getVelocity:INFO: velocity can be computed at t=%9.3e since we have\n"
               " a past history of grids, face=%i, grid=%i, numberOfTimeLevels=%i\n",time0,face,grid,
               gridEvolution[face]->getNumberOfTimeLevels());
	gridEvolution[face]->getVelocity(time0,gridVelocityLocal,I1,I2,I3);
      }
      else
      {
	// For initial times, if we don't have a past history of grids,  we use the following function: 
	getInitialState( initialVelocity,time0,grid,cg[grid],I1,I2,I3,gridVelocityLocal);
      }
    }
    else
    {
      if( face>=0 && face<gridEvolution.size() )
      {
	gridEvolution[face]->getVelocity(time0,gridVelocityLocal,I1,I2,I3);
      }
      else if( cg.numberOfRefinementLevels()>0 && cg.refinementLevelNumber(grid)>0 )
      {
        // -- this is a refinement of a deforming grid ---

	int baseGrid = cg.baseGridNumber(grid);
	face = getFace( baseGrid );
	assert( face>=0 && face<gridEvolution.size() );
	
        const int level=cg.refinementLevelNumber(grid);
	printF("DeformingBodyMotion::getVelocity:INFO: grid=%i (baseGrid=%i, face=%i) is a refinement grid on level=%i. FINISH ME! \n",
	       grid,baseGrid,face,cg.refinementLevelNumber(grid));
	    

	Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
	getIndex(cg[baseGrid].dimension(),J1,J2,J3);   

        realSerialArray baseGridVelocity(J1,J2,J3,cg.numberOfDimensions()); // fix me for parallel 
	
	getIndex(cg[baseGrid].gridIndexRange(),J1,J2,J3,extra);   // could do better here
        gridEvolution[face]->getVelocity(time0,baseGridVelocity,J1,J2,J3);


        // Interpolate the grid velocity on the refinement grid from the base grid velocity

	InterpolateParameters interpParams(cg.numberOfDimensions());  
	interpParams.setInterpolateOrder(2); // ***********
        Interpolate interp(interpParams);

//   int interpolateFineFromCoarse(realArray&                   fineGridArray,
// 				const Index                  Iv[3],
// 				const realArray&             coarseGridArray,
// 				const IntegerArray&          amrRefinementRatio_=Overture::nullIntArray(),
// 				const int update=0,
//                                 const int transferWidth=useDefaultTransferWidth );

	IntegerArray ratio(3);
	int axis=0;
	ratio = cg.refinementLevel[level].refinementFactor(axis,baseGrid)/cg.refinementLevel[0].refinementFactor(axis,baseGrid);
	printF("  --> rf[%i]=%i, rf[%i]=%i, refinement ratio=%i\n",level,cg.refinementLevel[level].refinementFactor(axis,baseGrid),
	       0,cg.refinementLevel[0].refinementFactor(axis,baseGrid),  ratio(0));
	// ratio=2;
	I4=Range(cg.numberOfDimensions());

        #ifndef USE_PPP
          interp.interpolateFineFromCoarse(gridVelocity,Iv,baseGridVelocity,ratio);
        #else
	  OV_ABORT("Finish me for parallel");
        #endif	

        // ::display(baseGridVelocity,"DBM: gridVelocity on the base grid","%8.2e ");
        // ::display(gridVelocity,"DBM: gridVelocity on the refinement grid","%8.2e ");


        // gridVelocityLocal=0.; // do this for now

        // OV_ABORT("finish me");

      }
      else
      {
	if( true || debug&2 )
	{
	  printF("DeformingBodyMotion:getVelocity:WARNING: There is no velocity available for t=%9.3e\n",time0);
	  printF("DeformingBodyMotion: grid=%i, face=%i,  gridEvolution.size()=%i\n",grid,face,gridEvolution.size());

	  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
	  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
	  printF("DeformingBodyMotion: numberOfFaces=%i\n",numberOfFaces);
	  ::display(boundaryFaces,"boundaryFaces(0:2,face) = (side,axis,grid) for each face");
	}
      
	gridVelocityLocal=0.;
      }
    }
  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  }
  return 0;
}


// ===============================================================================================
/// \brief Return the grid velocity at specified points 
// ===============================================================================================
int DeformingBodyMotion::
getVelocityBC( const real time0, const int grid, MappedGrid & mg, const Index &I1, const Index &I2, const Index &I3, 
	       realSerialArray & bcVelocity)
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    assert( pElasticFilament!=NULL );
    bcVelocity = 0.;
    pElasticFilament->getVelocityBC( time0, I1,I2,I3, bcVelocity );
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    if( debug & 2 )
      printF("DeformingBodyMotion:getVelocityBC called for userDefinedDeformingBody for t=%9.3e\n",time0);

    vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");
    int face=getFace( grid );

    if( time0<=0 ) 
    {
      if( face>=0 && face<gridEvolution.size() && gridEvolution[face]->getNumberOfTimeLevels()>1 )
      {
        printF("DeformingBodyMotion::getVelocityBC:INFO: the velocity can be computed at t=%9.3e since we have\n"
               " a past history of grids, face=%i, grid=%i, numberOfTimeLevels=%i\n",time0,face,grid,
               gridEvolution[face]->getNumberOfTimeLevels());
	gridEvolution[face]->getVelocity(time0,bcVelocity,I1,I2,I3);
      }
      else
      {
	// For initial times, if we don't have a past history of grids,  we use the following function: 
	getInitialState( initialVelocity,time0,grid,mg, I1,I2,I3,bcVelocity);
      }
      
    }
    else
    {
      if( face>=0 && face<gridEvolution.size() )
      {
	gridEvolution[face]->getVelocity(time0,bcVelocity,I1,I2,I3);
      }
      else
      {
	if( true || debug&2 )
	  printF("DeformingBodyMotion:getVelocityBC:WARNING: There is no velocity available for t=%9.3e\n",time0);
      }
    }
  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  } 
  return 0;
}


// ===============================================================================================
/// \brief Return the grid acceleration on boundary points.
// ===============================================================================================
int DeformingBodyMotion::
getAccelerationBC( const real time0, const int grid, MappedGrid & mg, const Index &I1, const Index &I2, const Index &I3, 
		   realSerialArray & bcAcceleration)
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    assert( pElasticFilament!=NULL );
    bcAcceleration = 0.;
    pElasticFilament->getAccelerationBC( time0, I1,I2,I3, bcAcceleration );
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    // bcAcceleration = 0.;
    vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");
    int face=getFace( grid );
    
    UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
      deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

    if( userDefinedDeformingBodyMotionOption==elasticBeam )
    {
      printF("-- DBM --- DeformingBodyMotion::getAccelerationBC for the elasticBeam t=%9.3e\n",time0);

      vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
      vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
      assert( face<surfaceArray.size() );
      RealArray *px = surfaceArray[face];
      RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
      assert( face<surfaceArrayTime.size() );
      real & tx0= surfaceArrayTime[face][0];
      IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

      int sideToMove=boundaryFaces(0,face);
      int axisToMove=boundaryFaces(1,face);
      int gridToMove=boundaryFaces(2,face); 
      int numberOfDimensions = 2;
      int i1,i2,i3;
      int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction
      assert(axisp != 2);
      
      int dx[2] = {I1.getBound()-I1.getBase(),I2.getBound()-I2.getBase()};
      int start[4] = {I1.getBase(),!I1.getBase(),I2.getBase(),!I2.getBase()};
      
      if (dx[axisToMove] == 0 && !start[axisToMove*2+sideToMove]) 
      {
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	      
	  pBeamModel->projectAcceleration(x0(i1,i2,i3,0), 
					  x0(i1,i2,i3,1),
					  bcAcceleration(i1,i2,i3,0),
					  bcAcceleration(i1,i2,i3,1));
	}
      } 
      else 
      {
	bcAcceleration(I1,I2,I3,0) = 0.0;
	bcAcceleration(I1,I2,I3,1) = 0.0;
      }
	
      return 0;
    }
    else if ( userDefinedDeformingBodyMotionOption==nonlinearBeam) {


      vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
      vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
      assert( face<surfaceArray.size() );
      RealArray *px = surfaceArray[face];
      RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
      assert( face<surfaceArrayTime.size() );
      real & tx0= surfaceArrayTime[face][0];
      IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

      int sideToMove=boundaryFaces(0,face);
      int axisToMove=boundaryFaces(1,face);
      int gridToMove=boundaryFaces(2,face); 
      int numberOfDimensions = 2;
      int i1,i2,i3;
      int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction
      assert(axisp != 2);
      
      int dx[2] = {I1.getBound()-I1.getBase(),I2.getBound()-I2.getBase()};
      int start[4] = {I1.getBase(),!I1.getBase(),I2.getBase(),!I2.getBase()};
      
      if (dx[axisToMove] == 0 && !start[axisToMove*2+sideToMove]) {
	{ FOR_3D(i1,i2,i3,I1,I2,I3)
	    {
	      
	      pNonlinearBeamModel->projectAcceleration((dx[0] == 0  ? i2 : i1),
					      bcAcceleration(i1,i2,i3,0),
					      bcAcceleration(i1,i2,i3,1));
	    }}
      } else {
	bcAcceleration(I1,I2,I3,0) = 0.0;
	bcAcceleration(I1,I2,I3,1) = 0.0;
      }
	
      return 0;
    }

    
    if( time0<=0 ) 
    {
      if( face>=0 && face<gridEvolution.size() && gridEvolution[face]->getNumberOfTimeLevels()>2 )
      {
        printF("DeformingBodyMotion::getAccelerationBC:INFO: the acceleration can be computed at t=%9.3e since we have\n"
               " a past history of grids, face=%i, grid=%i, numberOfTimeLevels=%i\n",time0,face,grid,
               gridEvolution[face]->getNumberOfTimeLevels());
	gridEvolution[face]->getAcceleration(time0,bcAcceleration,I1,I2,I3);
      }
      else
      {
	// For initial times, if we don't have a past history of grids,  we use the following function:
	getInitialState( initialAcceleration,time0,grid,mg, I1,I2,I3,bcAcceleration );
      }
      
    }
    else
    {
      if( face>=0 && face<gridEvolution.size() )
      {
	gridEvolution[face]->getAcceleration(time0,bcAcceleration,I1,I2,I3);
      }
      else
      {
	if( true || debug&2 )
	  printF("DeformingBodyMotion:getAccelerationBC:WARNING: There is no data available for t=%9.3e\n",time0);
      }
    }
  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  } 
  return 0;
}

// int DeformingBodyMotion::
// getAccelerationBC( const real time0, const int grid, 
// 		   CompositeGrid & cg, realArray & bcAcceleration)
// {
//   cout<<"DeformingBodyMotion::getAccelBC(time,grid,cg,bcAccel) -- not used!!\n";
//   //  bcAcceleration = 0.;
//   //pElasticFilament->getAccelerationBC( time0, grid, cg, bcAcceleration );
//   return 0;
// }

//========================================================================================
/// \brief: return the number of grids that form the deforming body
//========================================================================================
int DeformingBodyMotion::
getNumberOfGrids()
{
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");

  return numberOfDeformingGrids; 
}



// ========================================================================================
//  \brief Initialize the component grid that is deforming
//
// ========================================================================================
int DeformingBodyMotion::
initialize( CompositeGrid & cg, real t /* = 0. */ )
{
  int ierr=0;
  const int numberOfDimensions = cg.numberOfDimensions();
  int & numberOfTimeLevels = deformingBodyDataBase.get<int>("numberOfTimeLevels");
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
    deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");


  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    if( deformingBodyType==elasticFilament )
    {

      // do this for now -- treat more grids later 
      assert( numberOfDeformingGrids==1 );
      assert( numberOfFaces==1 );


      ierr=1;
    
      // *wdh* put this here:
      pDeformingGrid = new DeformingGrid( numberOfTimeLevels, pGIDebug, debug);


      //.... We assume moving grid is an "ElasticFilament'
      //.... Use mapping in the grid
      //.... >> Create new ElasticFilament & init. with FilamMap from the grid
      pElasticFilament = new ElasticFilament();

      Mapping *pMap = (FilamentMapping*) cg[gridToMove].mapping().mapPointer;
      assert( pMap != NULL);
      if ( pMap->getClassName() == "FilamentMapping" )
      {
	FilamentMapping *pTempFilament = (FilamentMapping*) pMap;
	assert( pTempFilament != NULL );
	if(debug&4) {
	  cout << endl << "-----------------------------------------\n";
	  cout << "DeformingBodyMotion::init -- calling printHyperbDims:\n";
	}
	printFilamentHyperbolicDimensions(cg, gridToMove);
    
	pElasticFilament->initializeFromFilamentMapping( pTempFilament );
      }
      else 
      {
	ierr=0;
	cout <<endl
	     <<"DeformingBodyMotion::initializeDeformingBody:\n"
	     <<"   error: trying to move FilamentMapping "
	     << "grid #"<<gridToMove<<", of type "
	     << pMap->getClassName() 
	     <<endl<<endl;
	pElasticFilament=NULL;
      }
    }
    else if( deformingBodyType==userDefinedDeformingBody )
    {
      printF("*** DeformingBodyMotion::initialize userDefinedDeformingBody ***\n");
    
      if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
	deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);
      UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
	deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");


      vector<Mapping*> & transform =deformingBodyDataBase.get<vector<Mapping*> >("transform");
    

      // The "surface" Mapping holds the start curve in 2D
      vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
      if( face>=surface.size() )
	surface.push_back(new NurbsMapping);

      const int numberOfSurfaceArrays=3;
      vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
      vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 

      RealArray *ppx = new RealArray [numberOfSurfaceArrays];
      real *psat = new real [numberOfSurfaceArrays];
      if( face>=surfaceArray.size() )
      {
	surfaceArray.push_back(ppx);
	surfaceArrayTime.push_back(psat);
      }
      

      RealArray *px = surfaceArray[face];
      RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
      real & tx0 = surfaceArrayTime[face][0];
      tx0=t;  // set time associated with the x0 points
      
      real rad=.5, theta; 

      if( userDefinedDeformingBodyMotionOption==iceDeform )
      {
	// ************************************************      
	// ********** Specified Ice Motion ****************      
	// ************************************************      
	if( cg.numberOfDimensions()==2 )
	{
	  // Two_dimensions
	  const int n1=13;
	  x0.redim(n1,2); x1.redim(n1,2); x2.redim(n1,2);

	  // x0: points on a arc
	  x0(0,0)=0.; x0(0,1)=1.*rad;
	  theta=Pi* 5./180.; x0(1,0)=-rad*sin(theta); x0(1,1)=rad*cos(theta);
	  theta=Pi*10./180.; x0(2,0)=-rad*sin(theta); x0(2,1)=rad*cos(theta);
	  theta=Pi*15./180.; x0(3,0)=-rad*sin(theta); x0(3,1)=rad*cos(theta);
	  theta=Pi*20./180.; x0(4,0)=-rad*sin(theta); x0(4,1)=rad*cos(theta);

	  theta=Pi*50./180.; x0(5,0)=-rad*sin(theta); x0(5,1)=rad*cos(theta);
	  theta=Pi*90./180.; x0(6,0)=-rad*sin(theta); x0(6,1)=rad*cos(theta);
	  theta=Pi*130./180.;x0(7,0)=-rad*sin(theta); x0(7,1)=rad*cos(theta);

	  x0( 8,0)=x0(4,0); x0( 8,1)=-x0(4,1);
	  x0( 9,0)=x0(3,0); x0( 9,1)=-x0(3,1);
	  x0(10,0)=x0(2,0); x0(10,1)=-x0(2,1);
	  x0(11,0)=x0(1,0); x0(11,1)=-x0(1,1);
	  x0(12,0)=x0(0,0); x0(12,1)=-x0(0,1);


	  // a second start curve (deformed version)
	  x1=x0;
	  x1(5,0)=-.7; x1(5,1)=  .5;
	  x1(6,0)=-.6; x1(6,1)=  0.;
	  x1(7,0)=-.7; x1(7,1)= -.5;
	}
	else if( cg.numberOfDimensions()==3 )
	{
	  // Three dimensional case (starting from cylDeform.hdf)

	  // get width in z-direction from the backGround grid (assumed to be grdi 0)
	  assert( cg[0].isRectangular() );
	  real dx[3],xab[2][3];
	  cg[0].getRectangularGridParameters( dx, xab );


//      const int n1=13, n2=4;
	  const int n1=13, n2=11;
	  x0.redim(n1,n2,3); x1.redim(n1,n2,3); x2.redim(n1,n2,3);
	  Range all;

	  real za=xab[0][2], zb=xab[1][2];  // get these from backGround grid
	  // x0: points on a arc
	  for( int i2=0; i2<n2; i2++ )
	  {
	    real z0=za+(zb-za)*i2/(n2-1);
	
	    x0(0,i2,0)=0.; x0(0,i2,1)=1.*rad;
	    theta=Pi* 5./180.; x0(1,i2,0)=-rad*sin(theta); x0(1,i2,1)=rad*cos(theta);
	    theta=Pi*10./180.; x0(2,i2,0)=-rad*sin(theta); x0(2,i2,1)=rad*cos(theta);
	    theta=Pi*15./180.; x0(3,i2,0)=-rad*sin(theta); x0(3,i2,1)=rad*cos(theta);
	    theta=Pi*20./180.; x0(4,i2,0)=-rad*sin(theta); x0(4,i2,1)=rad*cos(theta);

	    theta=Pi*50./180.; x0(5,i2,0)=-rad*sin(theta); x0(5,i2,1)=rad*cos(theta);
	    theta=Pi*90./180.; x0(6,i2,0)=-rad*sin(theta); x0(6,i2,1)=rad*cos(theta);
	    theta=Pi*130./180.;x0(7,i2,0)=-rad*sin(theta); x0(7,i2,1)=rad*cos(theta);

	    x0( 8,i2,0)=x0(4,i2,0); x0( 8,i2,1)=-x0(4,i2,1);
	    x0( 9,i2,0)=x0(3,i2,0); x0( 9,i2,1)=-x0(3,i2,1);
	    x0(10,i2,0)=x0(2,i2,0); x0(10,i2,1)=-x0(2,i2,1);
	    x0(11,i2,0)=x0(1,i2,0); x0(11,i2,1)=-x0(1,i2,1);
	    x0(12,i2,0)=x0(0,i2,0); x0(12,i2,1)=-x0(0,i2,1);

	    x0(all,i2,2)=z0;
	  }

	  // a second start curve (deformed version)
	  x1=x0;
	  for( int i2=0; i2<n2; i2++ )
	  {
	    x1(5,i2,0)=-.7; x1(5,i2,1)=  .5;
	    x1(6,i2,0)=-.6; x1(6,i2,1)=  0.;
	    x1(7,i2,0)=-.7; x1(7,i2,1)= -.5;
	  }
      
	}
	else
	{
	  Overture::abort("ERROR: unexpected value for numberOfDimensions!");
	}
      }
      else if( userDefinedDeformingBodyMotionOption==ellipseDeform )
      {
	rad=1.;  // initial radius 
	const int n1=51;
	x0.redim(n1,2);

	for( int i=0; i<n1; i++ )
	{
	  theta = i*2.*Pi/(n1-1);
	  x0(i,0) = rad*cos(theta);
	  x0(i,1) = rad*sin(theta);
	}

	NurbsMapping & startCurve = *((NurbsMapping*) surface[face]);
	startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);

      }
      else if( userDefinedDeformingBodyMotionOption==linearDeform )
      {
	// define the initial interface (a vertical line)
        real xa=0., xb=0., ya=0., yb=1.;
	const int n1=51;
	x0.redim(n1,2);

	for( int i=0; i<n1; i++ )
	{
	  real r  = real(i)/(n1-1);
	  x0(i,0) = xa + (xb-xa)*r;
	  x0(i,1) = ya + (yb-ya)*r;
	}

	NurbsMapping & startCurve = *((NurbsMapping*) surface[face]);
	// startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);  // ***** periodic or not??

      }
      else if( userDefinedDeformingBodyMotionOption==sphereDeform )
      {

	Mapping & map = cg[gridToMove].mapping().getMapping();
	assert( map.getClassName()=="HyperbolicMapping" );
	HyperbolicMapping & hype = (HyperbolicMapping&)map;

        Mapping *pStartSurface = (Mapping*)hype.getSurface();
        assert( pStartSurface!=NULL );
        Mapping & startSurface = *pStartSurface;

	GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();

	if( false )
	{
	  gi.stopReadingCommandFile();
	  printF("DeformingBodyMotion:initialize: surface Mapping for hyperbolic grid=%i\n",
		 gridToMove);
	
	  gi.erase();
	  PlotIt::plot(gi,startSurface);
	}
	
        NurbsMapping & surf = *((NurbsMapping*)surface[face]);

        int degree=3;
        surf.interpolate(startSurface,degree,NurbsMapping::parameterizeByIndex);

        realArray surfGrid; surfGrid = surf.getGrid();
        #ifdef USE_PPP
	  getLocalArrayWithGhostBoundaries( surfGrid, x0 );
        #else
	  x0.reference(surfGrid);
        #endif 
        Index I1=x0.dimension(0), I2=x0.dimension(1);
        x0.reshape(I1,I2,numberOfDimensions);

      }
      else if( userDefinedDeformingBodyMotionOption==advectBody ||
               userDefinedDeformingBodyMotionOption==interfaceDeform )
      {
        // ** do this for now **

	Mapping & map = cg[gridToMove].mapping().getMapping();
	assert( map.getClassName()=="HyperbolicMapping" );
	HyperbolicMapping & hype = (HyperbolicMapping&)map;

        Mapping *pStartSurface = (Mapping*)hype.getSurface();
        assert( pStartSurface!=NULL );
        Mapping & startSurface = *pStartSurface;

	GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();

	if( false )
	{
	  gi.stopReadingCommandFile();
	  printF("DeformingBodyMotion:initialize: surface Mapping for hyperbolic grid=%i\n",
		 gridToMove);
	
	  gi.erase();
	  PlotIt::plot(gi,startSurface);
	}
	
        NurbsMapping & surf = *((NurbsMapping*)surface[face]);

        int degree=3;
        surf.interpolate(startSurface,degree,NurbsMapping::parameterizeByIndex);

        realArray surfGrid; surfGrid = surf.getGrid();
        #ifdef USE_PPP
	  getLocalArrayWithGhostBoundaries( surfGrid, x0 );
        #else
	  x0.reference(surfGrid);
        #endif 
        Index I1=x0.dimension(0), I2=x0.dimension(1);
	if( numberOfDimensions==2 )
	  x0.reshape(I1,numberOfDimensions);
        else 
	  x0.reshape(I1,I2,numberOfDimensions);

      }
      else if( userDefinedDeformingBodyMotionOption==elasticShell ||
               userDefinedDeformingBodyMotionOption==userDeformingSurface )
      {

	Index Ib1,Ib2,Ib3;
        const int numGhost=1;  // include ghost points 
	getBoundaryIndex(cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);

        Range Rx=numberOfDimensions;
	x0.redim(Ib1,Ib2,Ib3,Rx);
	cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
	
        #ifdef USE_PPP
          RealArray vertex; getLocalArrayWithGhostBoundaries(cg[gridToMove].vertex(),vertex);
        #else
          RealArray & vertex = cg[gridToMove].vertex();
        #endif

	// Here is the undeformed state
        x0=vertex(Ib1,Ib2,Ib3,Rx);

        // Set x1 and x2 equal to the initial state
        x1.redim(Ib1,Ib2,Ib3,Rx); x2.redim(Ib1,Ib2,Ib3,Rx);
        x1=x0;
	x2=x0;

        // --- Check for periodic boundary conditions --
        MappedGrid & mg = cg[gridToMove];
        BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

	const int axisp = (axisToMove+1) % 2;  // tangential direction (2D)
        if( mg.isPeriodic(axisp)!=Mapping::notPeriodic )
	{
	  printF("DeformingBodyMotion::initialize:INFO: Setting boundary conditions to PERIODIC.\n");
          boundaryCondition(0,axisp)=periodicBoundaryCondition;
          boundaryCondition(1,axisp)=periodicBoundaryCondition;
	}
	


      }
      else if( userDefinedDeformingBodyMotionOption==elasticBeam )
      {

	Index Ib1,Ib2,Ib3;
        const int numGhost=0;  // include ghost points 
	getBoundaryIndex(cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);

        Range Rx=numberOfDimensions;
	x0.redim(Ib1,Ib2,Ib3,Rx);
	cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
	
        #ifdef USE_PPP
          RealArray vertex; getLocalArrayWithGhostBoundaries(cg[gridToMove].vertex(),vertex);
        #else
          RealArray & vertex = cg[gridToMove].vertex();
        #endif

	// Here is the undeformed state
        x0=vertex(Ib1,Ib2,Ib3,Rx);

        // --- Check for periodic boundary conditions --
	// MappedGrid & mg = cg[gridToMove];
        // BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

	// const int axisp = (axisToMove+1) % 2;  // tangential direction (2D)
        // if( mg.isPeriodic(axisp)!=Mapping::notPeriodic )
	// {
	//   printF("DeformingBodyMotion::initialize:INFO: Setting boundary conditions for elasticShell to PERIODIC.\n");
        //  boundaryCondition(0,axisp)=periodicBoundaryCondition;
        //  boundaryCondition(1,axisp)=periodicBoundaryCondition;
	// }
	//

        assert( pBeamModel!=NULL );

	// *wdh* The next code is moved to the update function
 	// pBeamModel = new BeamModel;

	// real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
	// real & I = par[0];
	// real & Em   = par[1];
	// real & rho   = par[2];
	// real & L   = par[3];
	// real & thick =   par[4];
	// real & pnorm = par[5];
	// real & x0 = par[6];
	// real & y0 = par[7];
	// real & dec = par[8];

	// int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
	// int& nelem = ipar[0];
	// int& bcl_ = ipar[1];
	// int& bcr_ = ipar[2];
	// int& exact = ipar[3];

	// BeamModel::BoundaryCondition bcl,bcr;
	// switch (bcl_) {
	// case 0:
	//   bcl = BeamModel::Cantilevered;  break;
	// case 1:
	//   bcl = BeamModel::Pinned;  break;
	// case 2:
	//   bcl = BeamModel::Free;  break;
	// default:
	//   std::cout << "Error: unknown beam boundary condition " << bcl_ << "; " <<
	//     " setting to Cantilevered" << std::endl;
	//   bcl = BeamModel::Cantilevered; break;	  
	// }

	// switch (bcr_) {
	// case 0:
	//   bcr = BeamModel::Cantilevered;  break;
	// case 1:
	//   bcr = BeamModel::Pinned;  break;
	// case 2:
	//   bcr = BeamModel::Free;  break;
	// default:
	//   std::cout << "Error: unknown beam boundary condition " << bcr_ << "; " <<
	//     " setting to Cantilevered" << std::endl;
	//   bcr = BeamModel::Cantilevered; break;	  
	// }

	// pBeamModel->setParameters(I, Em, rho, L, thick, pnorm, nelem, bcl, bcr,x0,y0,(exact==1));

        real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
	pBeamModel->setAddedMassRelaxation(omega);
        real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");

	pBeamModel->setSubIterationConvergenceTolerance(tol);

	// pBeamModel->setDeclination(dec*3.141592653589/180.0);

	if (deformingBodyDataBase.has_key("beam free motion")) {
	  real * p = deformingBodyDataBase.get<real [3]>("beam free motion");

	  pBeamModel->setupFreeMotion(p[0],p[1],p[2]);
	}
      }
      else if( userDefinedDeformingBodyMotionOption==nonlinearBeam )
      {

	Index Ib1,Ib2,Ib3;
        const int numGhost=0;  // include ghost points 
	getBoundaryIndex(cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);

        Range Rx=numberOfDimensions;
	x0.redim(Ib1,Ib2,Ib3,Rx);
	cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
	
        #ifdef USE_PPP
          RealArray vertex; getLocalArrayWithGhostBoundaries(cg[gridToMove].vertex(),vertex);
        #else
          RealArray & vertex = cg[gridToMove].vertex();
        #endif

	// Here is the undeformed state
        x0=vertex(Ib1,Ib2,Ib3,Rx);

	pNonlinearBeamModel = new NonlinearBeamModel;

	std::string modelfile = deformingBodyDataBase.get<std::string>("nonlinearBeamModelFile");

	pNonlinearBeamModel->readBeamFile(modelfile.c_str());

        real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
	pNonlinearBeamModel->setAddedMassRelaxation(omega);
        real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");

	pNonlinearBeamModel->setSubIterationConvergenceTolerance(tol);

	if (axisToMove == 1) {

	  pNonlinearBeamModel->initializeProjectedPoints(Ib1.getBound()+1);
	  for (int i = Ib1.getBase(); i <= Ib1.getBound(); ++i)
	    pNonlinearBeamModel->projectInitialPoint(i, x0(i,Ib2.getBase(),Ib3.getBase(),0),
						     x0(i,Ib2.getBase(),Ib3.getBase(),1));
	} else {

	  pNonlinearBeamModel->initializeProjectedPoints(Ib2.getBound()+1);
	  for (int i = Ib2.getBase(); i <= Ib2.getBound(); ++i)
	    pNonlinearBeamModel->projectInitialPoint(i, x0(Ib1.getBase(),i,Ib3.getBase(),0),
						     x0(Ib1.getBase(),i,Ib3.getBase(),1));
	}

      }
      else
      {
	printF("DeformingBodyMotion:ERROR: unexpected value for userDefinedDeformingBodyMotionOption=%i\n",
	       (int)userDefinedDeformingBodyMotionOption);
	OV_ABORT("DeformingBodyMotion:ERROR: unexpected value for userDefinedDeformingBodyMotionOption!");
      }
    
    
//     const IntegerArray & gid = cg[gridToMove].gridIndexRange();
//     for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
//       deformingGridPoints[axis]=gid(1,axis)-gid(0,axis)+1;

//     printF("DeformingBodyMotion::initialize: deformingGridPoints=%i, %i, %i\n",
// 	   deformingGridPoints[0],deformingGridPoints[1],deformingGridPoints[2]);
    
    }
    else
    {
      Overture::abort("ERROR: unknown deformingBodyType");
    }
    
  } // end for face 
  

  if( deformingBodyType==userDefinedDeformingBody &&
      deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption")==elasticShell )
  {
    // -- compute the initial volume ---
    // This is needed for a constant volume elastic shell (i.e. an elastic shell tha encloses an incompressible fluid)

    if( !deformingBodyDataBase.has_key("initialVolume") )
      deformingBodyDataBase.put<real>("initialVolume");


    real & volume = deformingBodyDataBase.get<real>("initialVolume");
    real area=0.;
    getBodyVolumeAndSurfaceArea( cg, volume, area );

    printF(" advanceElasticShell: INITIAL: t=%9.3e, volume=%9.3e, volume/pi=%9.3e, area=%9.3e, area/(2*pi)=%9.3e\n",
	   t,volume,volume/Pi,area,area/(2.*Pi));


  }
  


  initializeGrid( cg,t );  // *wdh* 080221

  return( ierr );
}

// ==================================================================================
///  \brief This initialization routine will adjust the initial CompositeGrid so that
///  it can be deformed. This usually means that we change the Mapping for the grid that deforms.
/// \param cg (input) : CompositeGrid to use
/// \t (input) : time 
// ==================================================================================
int DeformingBodyMotion::
initializeGrid(CompositeGrid & cg, real t /* =0. */ ) 
{
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  for( int face=0; face<numberOfFaces; face++ )
  {
    int gridToMove= boundaryFaces(2,face);  


    if( deformingBodyType==userDefinedDeformingBody )
    {

      if( debug & 2 ) 
        printF(">>>DeformingBodyMotion::initializeGrid: grid=%i (face=%i)\n",gridToMove,face);
    
      // allocate a new GridEvolution for this grid 
      vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");
      if( face >=gridEvolution.size() )
        gridEvolution.push_back(new GridEvolution);

      // set the order of accuracy for computing the velocity and acceleration
      gridEvolution[face]->setVelocityOrderOfAccuracy(deformingBodyDataBase.get<int>("velocityOrderOfAccuracy"));
      gridEvolution[face]->setAccelerationOrderOfAccuracy(deformingBodyDataBase.get<int>("accelerationOrderOfAccuracy"));

      Mapping & map = cg[gridToMove].mapping().getMapping();

      //  ---------------------------------------------------------------------------------
      //  --- if the grid is already a hyperbolic mapping we don't need to replace it  ----
      //  ---------------------------------------------------------------------------------

      HyperbolicMapping *pHype=NULL;
      if( map.getClassName()!="HyperbolicMapping" )
      {  
        // ************ The Mapping is NOT a HyperbolicMapping : create one ******************


	// The "surface" Mapping holds the start curve in 2D
	vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
	assert( face<surface.size() );
	NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

	vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
	assert( face<surfaceArray.size() );
	RealArray *px = surfaceArray[face];

	assert( px!=NULL );
	RealArray &x0 = px[0];
        #ifdef USE_PPP
	  Overture::abort("fix me");
        #else	
	  // *wdh* 081107 startCurve.interpolate(x0);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x0,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
	vector<Mapping*> & transform =deformingBodyDataBase.get<vector<Mapping*> >("transform");

	if( face >=transform.size() )
	  transform.push_back(new HyperbolicMapping);
      

	assert( transform[face]->getClassName()=="HyperbolicMapping" );
        pHype = (HyperbolicMapping *)transform[face];
	HyperbolicMapping & hyp = *pHype;
    

	// hyp.setSurface(startCurve,false);
	const bool isSurfaceGrid=false; // this is a volume grid (in 2d or 3d) 
	const bool init=false; // this means keep existing hype parameters such as distanceToMarch, linesToMarch etc.
	hyp.setSurface(startCurve,isSurfaceGrid,init);

        // **** we need to set the marching parameters ****
        printF(">>>DeformingBodyMotion::initializeGrid: You must build the initial hyperbolic grid now...\n");
      
        GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
	
	hyp.interactiveUpdate(gi);
	// hyp.generate();

	cg[gridToMove].reference(hyp);  

	cg[gridToMove].update(MappedGrid::THEcenter | MappedGrid::THEvertex);    


      }
      else
      {
        pHype = &( (HyperbolicMapping &)map);
        
      }
      
      


      assert( pHype!=NULL );
      HyperbolicMapping & hyp = *pHype;

      if( debug & 4 )
      {
        printF("DeformingBodyMotion::initializeGrid: spacingOption=%i\n",hyp.spacingOption);
	printF("DeformingBodyMotion::initializeGrid: HyperbolicMapping Parameters:");
        hyp.display();
      }


#ifndef USE_PPP
      // turn this off temporarily for PPP until we update Overture
      const DataPointMapping *pdpm = hyp.getDataPointMapping();
#else
      DataPointMapping *pdpm = NULL; 
#endif
      assert( pdpm!=NULL );
      DataPointMapping & dpm = (DataPointMapping&)(*pdpm);
      // dpm.getDataPoints() includes ghost points

      if( debug & 2 ) 
	printF(">>>DeformingBodyMotion::add initial grid to GridEvolution, gridToMove=%i, t=%8.2e\n",gridToMove,t);

      gridEvolution[face]->addGrid(dpm.getDataPoints(),t);
      

    } // end if deformingBodyType==userDefinedDeformingBody
  
  } // end for face
  
  return 0;
}

//==================================================================================
/// \brief Initialize past history of the grid so that the grid velocity can be computed at t=0.
/// \details 

/// .. CALLED in the beginning of a computation to initialize
///    the mapping history list with reasonable values so the
///    grid velocity gets computed OK from the very first tstep
///
/// .. This will have to change once the surface gets it's own physics
/// .. --> use x_t, x_tt at t=0 to find boundary curve at t= -dt, t=-2*dt
///
//==================================================================================//
int DeformingBodyMotion::
initializePast( real time00, real dt00, CompositeGrid & cg)
{
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( true ) printF("DeformingBodyMotion::initializePast:INFO: time=%8.2e\n",time00);

  if( deformingBodyDataBase.get<int>("providePastHistory") )
  {
    // query for past time grid history
    GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
    aString answer;
    const aString initializePastMenu[]=
      {
	"deforming grid history",
	"done",
	""
      }; 

    gi.appendToTheDefaultPrompt("initializePast>");
    for( ;; ) 
    {
	    
      int response=gi.getMenuItem(initializePastMenu,answer,"Choose an option");
    

      if( answer=="done" || answer=="exit" )
      {
	break;
      }
      else if( answer=="deforming grid history" )
      {
	printF("Provide a list of grids and times that define a deforming grid at times before the initial time.\n"
	       "These grids will be used to define the grid velocity and acceleration at the initial time.\n");
      
        vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution");

	aString cgName,answer2,name;
	for( ;; )
	{
	  gi.inputString(cgName,"Enter the name of a grid file or 'done' to finish");
	  if( cgName=="done" || cgName=="exit" )
	  {
	    break;
	  }
	  else
	  {
	    CompositeGrid cgHistory;
	    int returnValue = getFromADataBase(cgHistory,cgName);
	    if( returnValue==0 )
	    {
	      // look for the domain to use: ? 
	      printF("History grid %s was successfully read\n",(const char*)cgName);
	      printF("number of domains=%i.\n",cgHistory.numberOfDomains());
	      printF("Here are the names of the grids:\n");
	      for( int grid=0; grid<cgHistory.numberOfComponentGrids(); grid++ )
	      {
		printF(" grid %i : %s\n",grid,(const char*)cgHistory[grid].getName());
	      }
              gi.inputString(answer2,"Enter the time associated with this grid");
	      real gridTime=0.;
              sScanF(answer2,"%e",&gridTime);
	      printF("This grid is at time t=%9.3e\n",gridTime);
              // here are the faces of the deforming grid: 
	      for( int face=0; face<numberOfFaces; face++ )
	      {
		int sideToMove=boundaryFaces(0,face);
		int axisToMove=boundaryFaces(1,face);
		int gridToMove=boundaryFaces(2,face); 
		for( int grid=0; grid<cgHistory.numberOfComponentGrids(); grid++ )
		{
		  if( cg[gridToMove].getName() == cgHistory[grid].getName() )
		  {
		    printF(" deformingBody face=(%i,%i) of grid=%i (%s) matches history grid %i (%s)\n",
			   sideToMove, axisToMove, gridToMove,(const char*)cg[gridToMove].getName(), 
                           grid, (const char*)cgHistory[grid].getName());
		    
                    cgHistory[grid].update(MappedGrid::THEcenter | MappedGrid::THEvertex);
                    realArray & x = cgHistory[grid].vertex();
		    gridEvolution[face]->addGrid(x,gridTime);
		    
		  }
		}
		
	      }
	    }
	  
	  }
	}
      }
    } // end for( ;; )
    gi.unAppendTheDefaultPrompt();
    
  } // end providePastHistory
  
  

  // -- Here is the old way for Petri's grids: 
  // do this for now -- treat more grids later 
  for( int face=0; face<numberOfFaces; face++ )
  {
    int gridToMove= boundaryFaces(2,face);  


    if( deformingBodyType==elasticFilament )
    {
      assert( numberOfDeformingGrids==1 );
      assert( numberOfFaces==1 );

      assert(pElasticFilament!=NULL);
      assert(pDeformingGrid!=NULL);

      if(debug&2) cout << "DeformingBodyMotion::initializePast called\n";
      real time2=time00-2*dt00;  //past time levels
      real time1=time00-dt00;

      HyperbolicMapping *pHyper;
      aString newMappingName;   // for debugging: change name of hyperb!!

      //..Initialize past time-2*dt
      pDeformingGrid->getNewMapping( time2, pHyper); // t=-2*dt
      assert( pHyper != NULL);
      pElasticFilament->evaluateSurfaceAtTime(time2); 
      if (debug &4) {
	pDeformingGrid->createMappingName( time2, newMappingName );
	pElasticFilament->copyBodyFittedMapping( *pHyper, &newMappingName );
	cout << "initPast: time level 2 = time-2dt\n";
	if(pMapInfoDebug!=NULL) pHyper->update( *pMapInfoDebug ); 
      } else {
	pElasticFilament->copyBodyFittedMapping( *pHyper ); //non debug code
      }
      pElasticFilament->referenceMap( gridToMove,  cg); //TRYING TO FORCE FILAM TO SAVE

      //..Initialize past time1-dt
      pDeformingGrid->getNewMapping( time1, pHyper); // t=  -dt
      assert( pHyper != NULL);
      pElasticFilament->evaluateSurfaceAtTime(time1);
      if (debug &4) {
	pDeformingGrid->createMappingName( time1, newMappingName );
	pElasticFilament->copyBodyFittedMapping( *pHyper, &newMappingName );
	cout << "initPast: time level 1 = time-dt\n";
	if(pMapInfoDebug!=NULL) pHyper->update( *pMapInfoDebug ); 
      } else {
	pElasticFilament->copyBodyFittedMapping( *pHyper ); //non debug code
    
      }
      //TEMP FIX **pf--not needed, feb26,2001?---still needed, closed filam;-(
      if( pMapInfoDebug!=NULL) pHyper->update( *pMapInfoDebug );

      pElasticFilament->referenceMap( gridToMove,  cg); //TRYING TO FORCE FILAM TO SAVE
  
      //..Initialize current TIME!!
      pDeformingGrid->getNewMapping( time00, pHyper); // t=  -dt
      assert( pHyper != NULL);
      //pElasticFilament->evaluateSurfaceAtTime( time00 );
      pElasticFilament-> initializeSurfaceData( time00 );

      pElasticFilament->copyBodyFittedMapping( *pHyper, &newMappingName );
      if (debug &4) {
	pDeformingGrid->createMappingName( time00, newMappingName );
	pElasticFilament->copyBodyFittedMapping( *pHyper, &newMappingName );
	cout << "initPast: time level 0 = current\n";
	//if(pMapInfoDebug!=NULL) pHyper->update( *pMapInfoDebug ); 
      } else {
	pElasticFilament->copyBodyFittedMapping( *pHyper ); //non debug code
      }
      pElasticFilament->referenceMap( gridToMove,  cg); //TRYING TO FORCE FILAM TO SAVE

    }
    else if( deformingBodyType==userDefinedDeformingBody )
    {
      printF("DeformingBodyMotion:initializePast called for userDefinedDeformingBody for t=%9.3e, dt=%9.3e\n",
	     time00,dt00);
    }
    else
    {
      Overture::abort("ERROR: unknown deformingBodyType");
    }

  } // end for face

  return 0;
}

int DeformingBodyMotion::
getBodyVolumeAndSurfaceArea( CompositeGrid & cg, real & volume, real & area )
// ==============================================================================================
/// \brief Compute the enclosed volume and surface area of the body. 
///   The body faces are assumed to define a closed region. 
///
/// \param cg (input) :
/// \param volume (output) : volume enclosed by the body faces.
/// \param area (output) : surface area of the body.
// ==============================================================================================
{
  int ierr=0;

  const int numberOfDimensions = cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

//   DeformingBodyType & deformingBodyType = 
//                   deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
//   UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
//     deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

//   assert( deformingBodyType==userDefinedDeformingBody );
//   assert( userDefinedDeformingBodyMotionOption==elasticShell );

  assert( numberOfFaces==1 );  // FIX ME 
  
  // -- compute the volume: (see MovingGrids.C line 4132 for general case --

  //  bodyVolume = int_V div.(x,0,0) dV = int_S (x,0,0).nv dS
  //     --> set f= (x,0,0).nv = x*n_x

  const int face=0;
  const int side=boundaryFaces(0,face);
  const int axis=boundaryFaces(1,face);
  const int grid=boundaryFaces(2,face); 

  MappedGrid & c = cg[grid];
  c.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal );
	  
#ifdef USE_PPP
  // realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
  realSerialArray xLocal; getLocalArrayWithGhostBoundaries(c.vertex(),xLocal);
  const realSerialArray & normalLocal = c.vertexBoundaryNormalArray(side,axis);
  OV_ABORT("finish me");
#else
  // realSerialArray & fLocal = f[grid];
  const realSerialArray & normalLocal = c.vertexBoundaryNormal(side,axis);
  const realSerialArray & xLocal = c.vertex();
#endif 

  Index Ib1,Ib2,Ib3;
  getBoundaryIndex(c.indexRange(),side,axis,Ib1,Ib2,Ib3); 

  // -- compute the surface integral of x*nx by the Trapezodial rule --
  int is1=0, is2=0;
  if( axis==0 ) 
    is2=1;
  else
    is1=1;
    
  volume = 0.;
  area=0.;
  int i1,i2,i3;
  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
  {
    real x0=xLocal(i1    ,i2    ,i3,0), y0=xLocal(i1    ,i2    ,i3,1);
    real x1=xLocal(i1+is1,i2+is2,i3,0), y1=xLocal(i1+is1,i2+is2,i3,1);
      
    real f0 = x0*normalLocal(i1    ,i2    ,i3,0);
    real f1 = x1*normalLocal(i1+is1,i2+is2,i3,0);
    real ds = sqrt( SQR(x1-x0) + SQR(y1-y0) );
    volume += (f0+f1)*ds;
    area+=ds;
  }
  volume=.5*fabs(volume);

  return 0;
}


// ===========================================================================================
/// \brief Advance the elastic shell in time.
///
/// \param option : option=0 : predictor-step, option=1 corrector step. 
///
/// Here is a simple approximation to an elastic shell
/// 
/// The equation of motion of the interface x(s,t) is:
///
///  (rhoe*ds) dv/dt = p ds n - ke*( x-x0 ) + te*(x-x0).ss - be*v 
///    dx/dt = v 
///
///  where:
/// 
///    x0(s) : undeformed state.
///    rhoe : "density" of the shell. 
///    ds : arc length 
///    ke : restoring "spring" coefficient
///    te : tension term 
///    be : damping coefficient
/// 
// ===========================================================================================
int DeformingBodyMotion::
advanceElasticShell(real t1, real t2, real t3, 
		    GridFunction & cgf1,
		    GridFunction & cgf2,
		    GridFunction & cgf3,
		    realCompositeGridFunction & stress,
                    int option )
{
  int ierr=0;

  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  assert( deformingBodyType==userDefinedDeformingBody );
  assert( userDefinedDeformingBodyMotionOption==elasticShell );

  CompositeGrid & cg = cgf3.cg;

  real tForce=t2;
  real tNew=t3;
  
  if( debug&2 )
    printF("--DeformingBodyMotion::advanceElasticShell called for tNew=%f, forces at t=%f\n",tNew, tForce);

  const bool & constantVolumeElasticShell =  deformingBodyDataBase.get<bool>("constantVolumeElasticShell");
  const real & initialVolume = deformingBodyDataBase.get<real>("initialVolume");
  const real & volumePenalty =  deformingBodyDataBase.get<real>("volumePenalty");
  real & integratedVolumeDiscrepancy = deformingBodyDataBase.get<real>("integratedVolumeDiscrepancy");
  
  real volume=0., area=0.;
  getBodyVolumeAndSurfaceArea( cgf2.cg, volume, area );

  printF(" advanceElasticShell: t2=%9.3e, volume=%9.3e, volume/pi=%9.3e, V/V0=%9.3e, area=%9.3e, area/(2*pi)=%9.3e\n",
 	 t2,volume,volume/Pi,volume/initialVolume, area,area/(2.*Pi));


  Index Ib1,Ib2,Ib3;


  // We need to grab the surface points from cgf1
  const real dt = t3-t2;   // do this for now -- assume the solution x0 is at time t2

  real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");
  real & rhoe = par[0], &te=par[1], &ke=par[2], &be=par[3], ad2=par[4];
  assert( rhoe>0. );


  real volumePenaltyForce=0.;
  if( constantVolumeElasticShell )
  {
    // we add a penalty term to keep the volume enclosed by the shell constant
    real dv = 1. - volume/initialVolume;
    
    integratedVolumeDiscrepancy+= dv*dt;

    // The penalty is a sum of a "proportional" and "integral" term (PI control)
    volumePenaltyForce = (volumePenalty/dt)*( dv + integratedVolumeDiscrepancy);
  }
  

  // --------------------------------------
  // --- LOOP over faces on the surface ---
  // --------------------------------------
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 


    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");
    const int pc = parameters.dbase.get<int >("pc");

    // The "surface" Mapping holds the start curve in 2D
    vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
    assert( face<surface.size() );
    NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);


    // The undeformed surface, x0,  is stored here: 
    vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
    vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
    assert( face<surfaceArray.size() );
    RealArray *px = surfaceArray[face];
    RealArray &x0 = px[0];
    assert( face<surfaceArrayTime.size() );
    real & tx0= surfaceArrayTime[face][0];

    const int numGhost=1;  // include ghost points 
    Range Rx=numberOfDimensions;

    // --- The current surface position and velocity are stored in the GridFunction data-base ---
    for( int m=0; m<3; m++ )
    {
      GridFunction & gf = m==0 ? cgf1 : m==1 ? cgf2 : cgf3;
      if( !gf.dbase.has_key("xShell") )
      {
	// note: include ghost points:
	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
	gf.dbase.put<RealArray>("xShell");
	gf.dbase.put<RealArray>("vShell");

	RealArray & x = gf.dbase.get<RealArray>("xShell");
	RealArray & v = gf.dbase.get<RealArray>("vShell");
 
	gf.cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
#ifdef USE_PPP
	RealArray vertex; getLocalArrayWithGhostBoundaries(gf.cg[gridToMove].vertex(),vertex);
#else
	RealArray & vertex = gf.cg[gridToMove].vertex();
#endif
	x= vertex(Ib1,Ib2,Ib3,Rx);
	v.redim(Ib1,Ib2,Ib3,Rx);

	v=0.;                       // do this for now   ** fix me **
      }
    }
	
    getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);

    RealArray & x1 = cgf1.dbase.get<RealArray>("xShell");
    RealArray & v1 = cgf1.dbase.get<RealArray>("vShell");
	
    RealArray & x2 = cgf2.dbase.get<RealArray>("xShell");
    RealArray & v2 = cgf2.dbase.get<RealArray>("vShell");
	
    RealArray & x3 = cgf3.dbase.get<RealArray>("xShell");
    RealArray & v3 = cgf3.dbase.get<RealArray>("vShell");
	

    // RealArray x1,x2,x3, v1,v2,v3;

#ifdef USE_PPP
    RealArray u2; getLocalArrayWithGhostBoundaries(cgf2.u[gridToMove],u2);
    const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormalArray(sideToMove,axisToMove);
    RealArray vertex2; getLocalArrayWithGhostBoundaries(cgf2.cg[gridToMove].vertex(),vertex2);
#else
    RealArray & u2 = cgf2.u[gridToMove];
    const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormal(sideToMove,axisToMove);
    const RealArray & vertex2 = cgf2.cg[gridToMove].vertex();
#endif
	
    if( numberOfDimensions==2 )
    {
      assert( uc>=0 && vc>=0 && pc>=0 );
      int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction 
      // Index I1=x0.dimension(0), I2=x0.dimension(1);
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      int ivp[3], &i1p=ivp[0], &i2p=ivp[1], &i3p=ivp[2];
      int ivm[3], &i1m=ivm[0], &i2m=ivm[1], &i3m=ivm[2];
      FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
      {
	// int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
	i1p=i1, i2p=i2, i3p=i3;
	i1m=i1, i2m=i2, i3m=i3;
	ivp[axisp]+=1;
	ivm[axisp]-=1;
	real p2=u2(i1,i2,i3,pc);
	    
	// compute the local arc-length (use vertex2 since it has ghost points)
	real ds = .5*( sqrt( SQR(vertex2(i1p,i2p,i3p,0)-vertex2(i1m,i2m,i3m,0))+
			     SQR(vertex2(i1p,i2p,i3p,1)-vertex2(i1m,i2m,i3m,1)) ) );

	real ac[3]; // holds acceleration 
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  // Curvature term: dxss = (x-x0).ss 
	  // ** we need ghost point values for this ***
	  real dxss =( (vertex2(i1p,i2p,i3p,dir)-2.*vertex2(i1,i2,i3,dir)+vertex2(i1m,i2m,i3m,dir)) - 
		       (x0(i1p,i2p,i3p,dir)-2.*x0(i1,i2,i3,dir)+x0(i1m,i2m,i3m,dir)) )/(ds*ds);

	  // smoothing terms -- note: for "leap-frog" we should lag the dissipation terms *wdh* 100828
	  //real vss = v2(i1p,i2p,i3p,dir)-2.*v2(i1,i2,i3,dir)+v2(i1m,i2m,i3m,dir);
	  //real xss = x2(i1p,i2p,i3p,dir)-2.*x2(i1,i2,i3,dir)+x2(i1m,i2m,i3m,dir);
	  real vss = v1(i1p,i2p,i3p,dir)-2.*v1(i1,i2,i3,dir)+v1(i1m,i2m,i3m,dir);
	  real xss = x1(i1p,i2p,i3p,dir)-2.*x1(i1,i2,i3,dir)+x1(i1m,i2m,i3m,dir);

	  ac[dir]=( (p2-volumePenaltyForce)*normal2(i1,i2,i3,dir) -(ke/ds)*( x2(i1,i2,i3,dir)-x0(i1,i2,i3,dir) ) 
		    + te*dxss -be*v2(i1,i2,i3,dir) )/(rhoe) + ad2*vss;
	  v3(i1,i2,i3,dir) = v1(i1,i2,i3,dir) + 2.*dt*ac[dir];
	  x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir) + 2.*dt*( v2(i1,i2,i3,dir) +ad2*xss ) ;
	}
	if( false )
	{
	  printF("elasticShell: x=(%5.2f,%5.2f) x0=(%5.2f,%5.2f) ds=%8.2e v=(%5.2f,%5.2f) a=(%8.2e,%8.2e) "
		 "(u,v,p)=(%8.2e,%8.2e,%8.2e)\n",
		 x3(i1,i2,i3,0),x3(i1,i2,i3,1),x0(i1,i2,i3,0),x0(i1,i2,i3,1),ds, 
		 v3(i1,i2,i3,0),v3(i1,i2,i3,1),ac[0],ac[1],u2(i1,i2,i3,uc),u2(i1,i2,i3,vc),p2);
	      
	}
      } // end for
      

      // 2013/03/18 -- fix periodicity
 
      // --- Boundary Conditions ---

      const int axisp1 = (axisToMove+1) % numberOfDimensions;  // tangential direction
      
      for( int side=0; side<=1; side++ ) // loop over sides
      {

//	if( side==0 && boundaryCondition(side,axisp1)==periodicBoundaryCondition )
	if( boundaryCondition(side,axisp1)==periodicBoundaryCondition )
	{
	  // printF("advanceElasticShell: t=%g, BC=periodic!\n",t2);

          // *wdh* TURN OFF 2013/08/03 -- wrong for derivativePeriodic
	  // startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic); 

	  if( axisToMove==1 )
	  {
	    // boundary: i1=i1a,i1a+1,...,i1b
	    const int i2 =Ib2.getBase(), i3 =Ib3.getBase();
	    const int i1a=Ib1.getBase(), i1b=Ib1.getBound();
	    for( int dir=0; dir<numberOfDimensions; dir++ )
	    {
	      // Set periodic images to be the same:
	      real v3Ave = .5*(v3(i1a,i2,i3,dir)+v3(i1b,i2,i3,dir));
	      v3(i1a,i2,i3,dir) = v3Ave;
	      v3(i1b,i2,i3,dir) = v3Ave;

	      real x3Ave = .5*(x3(i1a,i2,i3,dir)+x3(i1b,i2,i3,dir));
	      x3(i1a,i2,i3,dir) = x3Ave;
	      x3(i1b,i2,i3,dir) = x3Ave;
	  
	      // set ghost values by periodicity
	      v3(i1a-1,i2,i3,dir)=v3(i1b-1,i2,i3,dir);
	      v3(i1b+1,i2,i3,dir)=v3(i1a+1,i2,i3,dir);
	  
	      x3(i1a-1,i2,i3,dir)=x3(i1b-1,i2,i3,dir);
	      x3(i1b+1,i2,i3,dir)=x3(i1a+1,i2,i3,dir);

	    }
	  }
	  else
	  {
	    // boundary: i2=i2a,i2a+1,...,i2b
	    assert( axisToMove==0 );
	    const int i1 =Ib1.getBase(), i3 =Ib3.getBase();
	    const int i2a=Ib2.getBase(), i2b=Ib2.getBound();
	    for( int dir=0; dir<numberOfDimensions; dir++ )
	    {
	      // Set periodic images to be the same:
	      real v3Ave = .5*(v3(i1,i2a,i3,dir)+v3(i1,i2b,i3,dir));
	      v3(i1,i2a,i3,dir) = v3Ave;
	      v3(i1,i2b,i3,dir) = v3Ave;

	      real x3Ave = .5*(x3(i1,i2a,i3,dir)+x3(i1,i2b,i3,dir));
	      x3(i1,i2a,i3,dir) = x3Ave;
	      x3(i1,i2b,i3,dir) = x3Ave;
	  
	      // set ghost values by periodicity
	      v3(i1,i2a-1,i3,dir)=v3(i1,i2b-1,i3,dir);
	      v3(i1,i2b+1,i3,dir)=v3(i1,i2a+1,i3,dir);
	  
	      x3(i1,i2a-1,i3,dir)=x3(i1,i2b-1,i3,dir);
	      x3(i1,i2b+1,i3,dir)=x3(i1,i2a+1,i3,dir);

	    }
	

	  }
	} // end if periodic BC
	else if( boundaryCondition(side,axisp1)==dirichletBoundaryCondition )
	{
	  // printF("advanceElasticShell: t=%g, side=%i, BC=dirichlet!\n",t2,side);


	  // --- Dirichlet boundary conditions ---
	  const int i3 =Ib3.getBase();
	  const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
	  const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
	  const int is1 = axisp1==0 ? 1-2*side : 0; 
	  const int is2 = axisp1==1 ? 1-2*side : 0;
	  
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
            // Dirichlet BC: 
	    x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir);
	    v3(i1,i2,i3,dir) = 0.;
	
	    // ghost points: extrapolate: x.ss=0 
	    x3(i1-is1,i2-is2,i3,dir) = 2.*x3(i1,i2,i3,dir) - x3(i1+is1,i2+is2,i3,dir);
	    v3(i1-is1,i2-is2,i3,dir) = 2.*v3(i1,i2,i3,dir) - v3(i1+is1,i2+is2,i3,dir);
	  
	  }
	}
	else if( boundaryCondition(side,axisp1)==slideBoundaryCondition )
	{
	  // printF("advanceElasticShell: t=%g, side=%i, BC=slide!\n",t2,side);


	  // --- Slide boundary condition ---
	  const int i3 =Ib3.getBase();
	  const int i1= side==0 ? Ib1.getBase() : Ib1.getBound();
	  const int i2= side==0 ? Ib2.getBase() : Ib2.getBound();
	  const int is1 = axisp1==0 ? 1-2*side : 0; 
	  const int is2 = axisp1==1 ? 1-2*side : 0;
	  
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
            // slide: 
            if( dir==axisp1 )
	    {
  	      x3(i1,i2,i3,dir) = x1(i1,i2,i3,dir);
	      v3(i1,i2,i3,dir) = 0.;
	    }
	    
	    // ghost points: extrapolate: x.ss=0 
	    x3(i1-is1,i2-is2,i3,dir) = 2.*x3(i1,i2,i3,dir) - x3(i1+is1,i2+is2,i3,dir);
	    v3(i1-is1,i2-is2,i3,dir) = 2.*v3(i1,i2,i3,dir) - v3(i1+is1,i2+is2,i3,dir);
	  
	  }
	}
	else
	{
	  // --- un-implemented BC ---
	  OV_ABORT("elasticShell: un-implemented BC: FINISH ME BILL!");

	}
	
      } // end for side
      
      x3.reshape(x3.dimension(axisp),Rx);

      #ifdef USE_PPP
	Overture::abort("fix me");
      #else
      int option=0, degree=3;
      const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
      startCurve.interpolate(x3,option,Overture::nullRealDistributedArray(),degree,
                             (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization,numGhost);
      // 081107 startCurve.interpolate(x3,option,Overture::nullRealDistributedArray(),degree,
      //                               NurbsMapping::parameterizeByChordLength,numGhost);
      #endif

      getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
      x3.reshape(Ib1,Ib2,Ib3,Rx);

    }
    else if( numberOfDimensions==3)
    {
      Overture::abort("error");
    }
	
    tx0=t3; // x0 points now live at this time

    
  } // end for face
  
  return ierr;
}


// ===========================================================================================
/// \brief Advance the elastic beam in time.
///
/// \param option : option=0 : predictor-step, option=1 corrector step. 
///
/// 
/// 
// ===========================================================================================
int DeformingBodyMotion::
advanceElasticBeam(real t1, real t2, real t3, 
		   GridFunction & cgf1,
		   GridFunction & cgf2,
		   GridFunction & cgf3,
		   realCompositeGridFunction & stress,
                   int option )
{
  int ierr=0;

  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  assert( deformingBodyType==userDefinedDeformingBody );
  assert( userDefinedDeformingBodyMotionOption==elasticBeam );

  CompositeGrid & cg = cgf3.cg;

  real tForce=t2;
  real tNew=t3;
  
  real bf[2] = {0.0,0.0};
  if (parameters.dbase.has_key("gravity")) {
    for (int k = 0; k < 2; ++k)
      bf[k] = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[k]; 
  }
  
  pBeamModel->addBodyForce(bf);

  if( debug&2 )
    printF("--DeformingBodyMotion::advanceElasticBeam called for tNew=%f, forces at t=%f\n",tNew, tForce);
  
  Index Ib1,Ib2,Ib3;

  // We need to grab the surface points from cgf1
  real dt = t3-t2;   // do this for now -- assume the solution x0 is at time t2

  if (option == 1)
    dt = t2-t1;

  pBeamModel->resetForce(); // WDH: this should be outside loop over faces. 

  const int beamID = pBeamModel->getBeamID();
  aString beamDirName;  // name of the dbase sub-directory where we save info for this beam
  
  // --------------------------------------
  // --- LOOP over faces on the surface ---
  // --------------------------------------
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");
    const int pc = parameters.dbase.get<int >("pc");

    // The "surface" Mapping holds the start curve in 2D
    vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
    assert( face<surface.size() );
    NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);


    // The undeformed surface, x0,  is stored here: 
    vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
    vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
    assert( face<surfaceArray.size() );
    RealArray *px = surfaceArray[face];
    RealArray &x0 = px[0];
    assert( face<surfaceArrayTime.size() );
    real & tx0= surfaceArrayTime[face][0];

    sPrintF(beamDirName,"beam%iFace%i",beamID,face);


    const int numGhost=0;  // include ghost points 
    Range Rx=numberOfDimensions;
    // --- The current surface position and velocity are stored in the GridFunction data-base ---
    for( int m=0; m<3; m++ )
    {
      GridFunction & gf = m==0 ? cgf1 : m==1 ? cgf2 : cgf3;
      if( !gf.dbase.has_key(beamDirName) )
      {
        gf.dbase.put<DataBase>(beamDirName);  // save beam arrays here 
        DataBase & beamDataBase = gf.dbase.get<DataBase>(beamDirName);

	// note: include ghost points:
	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
	// beamDataBase.put<RealArray>("xBeam");
	// beamDataBase.put<RealArray>("vBeam");
	beamDataBase.put<RealArray>("beamBoundaryLocation");

	// // RealArray & x = beamDataBase.get<RealArray>("xBeam");
	// // RealArray & v = beamDataBase.get<RealArray>("vBeam");
	RealArray & xbb = beamDataBase.get<RealArray>("beamBoundaryLocation");
 
	// gf.cg[gridToMove].update(MappedGrid::THEvertex | MappedGrid::THEcenter); // do this for now 
        // #ifdef USE_PPP
  	//   RealArray vertex; getLocalArrayWithGhostBoundaries(gf.cg[gridToMove].vertex(),vertex);
        // #else
	//   RealArray & vertex = gf.cg[gridToMove].vertex();
        // #endif

        // printF("+++ DeformingBodyMotion::initialize: Assign beam conditions at t=%9.3e\n",gf.t);
	
        // RealArray a;
        // pBeamModel->assignInitialConditions( gf.t, x,v,a  );

	// x = pBeamModel->position();
	// v = pBeamModel->velocity();                       // do this for now   ** fix me **

	xbb = x0;
      }
    }

    getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);

    // RealArray & x1 = cgf1.dbase.get<DataBase>(beamDirName).get<RealArray>("xBeam");
    // RealArray & v1 = cgf1.dbase.get<DataBase>(beamDirName).get<RealArray>("vBeam");
	
    // RealArray & x2 = cgf2.dbase.get<DataBase>(beamDirName).get<RealArray>("xBeam");
    // RealArray & v2 = cgf2.dbase.get<DataBase>(beamDirName).get<RealArray>("vBeam");
	
    // RealArray & x3 = cgf3.dbase.get<DataBase>(beamDirName).get<RealArray>("xBeam");
    // RealArray & v3 = cgf3.dbase.get<DataBase>(beamDirName).get<RealArray>("vBeam");

    const RealArray & x3 = pBeamModel->position(); // current degree's of freedom **FIX ME**
    

    RealArray & xbb = cgf2.dbase.get<DataBase>(beamDirName).get<RealArray>("beamBoundaryLocation");
    RealArray & xbb3 = cgf3.dbase.get<DataBase>(beamDirName).get<RealArray>("beamBoundaryLocation");
    xbb3 = xbb;    
    // RealArray x1,x2,x3, v1,v2,v3;

#ifdef USE_PPP
    RealArray u2; getLocalArrayWithGhostBoundaries(cgf2.u[gridToMove],u2);
    const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormalArray(sideToMove,axisToMove);
    RealArray vertex2; getLocalArrayWithGhostBoundaries(cgf2.cg[gridToMove].vertex(),vertex2);
    realSerialArray stressLocal; getLocalArrayWithGhostBoundaries(stress[gridToMove],stressLocal);
#else
    RealArray & u2 = cgf2.u[gridToMove];
    const RealArray & normal2 = cgf2.cg[gridToMove].vertexBoundaryNormal(sideToMove,axisToMove);
    const RealArray & vertex2 = cgf2.cg[gridToMove].vertex();
    realSerialArray & stressLocal = stress[gridToMove];
#endif
	
    //std::cout << "Created arrays" << std::endl;

    int ipar[] = {gridToMove,sideToMove,axisToMove,cgf2.form}; // 
    real rpar[] = { t2 }; // 
    parameters.getNormalForce( cgf2.u,stressLocal,ipar,rpar );


    //std::cout << "Computed force" << std::endl;
    if( numberOfDimensions==2 )
    {
      assert( uc>=0 && vc>=0 && pc>=0 );
      int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction 
      // Index I1=x0.dimension(0), I2=x0.dimension(1);
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      int ivp[3], &i1p=ivp[0], &i2p=ivp[1], &i3p=ivp[2];
      int ivm[3], &i1m=ivm[0], &i2m=ivm[1], &i3m=ivm[2];
      assert(axisp != 2);
      Index Ibb1 = Ib1,Ibb2 = Ib2,Ibb3 = Ib3;
      if (axisp == 0)
	Ibb1 = Index(Ib1.getBase(),Ib1.getBound()-Ib1.getBase());
      else if (axisp == 1)
	Ibb2 = Index(Ib2.getBase(),Ib2.getBound()-Ib2.getBase());
      
      { FOR_3D(i1,i2,i3,Ibb1,Ibb2,Ibb3)
      {
	// int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
	i1p=i1, i2p=i2, i3p=i3;
	i1m=i1, i2m=i2, i3m=i3;
	ivp[axisp]+=1;
	ivm[axisp]-=1;
	
	real p1 = stressLocal(i1,i2,i3,0)*normal2(i1,i2,i3,0)+
	  stressLocal(i1,i2,i3,1)*normal2(i1,i2,i3,1);

	real p2 = stressLocal(i1p,i2p,i3p,0)*normal2(i1p,i2p,i3p,0)+
	  stressLocal(i1p,i2p,i3p,1)*normal2(i1p,i2p,i3p,1);

        // ******************* WDH **************** TEMP 
        // p1 = u2(i1,i2,i3,pc);
	// p2 = u2(i1p,i2p,i3p,pc);

	pBeamModel->addForce(tForce,
                             x0(i1,i2,i3,0), 
                             x0(i1,i2,i3,1), p1,
			     normal2(i1,i2,i3,0), normal2(i1,i2,i3,1),
			     x0(i1p,i2p,i3p,0), 
			     x0(i1p,i2p,i3p,1), p2,
			     normal2(i1p,i2p,i3p,0), normal2(i1p,i2p,i3p,1));
      }} // end for

      if (option == 0)
	pBeamModel->predictor(t3, dt );
      else if (option == 1)
	pBeamModel->corrector(t3, dt );
      if (option == 0) {

	{ FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	    {
	       
              // wdh: Determine the current location of a point on the beam surface (not the neutral axis)
              //    x3 = current beam degrees of freedom
              //    x0 = location of surface point on the undeformed beam 
              // xbb (output) : current position of beam boundary 
	      pBeamModel->projectDisplacement(x3, x0(i1,i2,i3,0), 
					      x0(i1,i2,i3,1),
					      xbb(i1,i2,i3,0),
					      xbb(i1,i2,i3,1));
	    }}
	int xxx =xbb.dimension(axisp).length();
	xbb.reshape(xbb.dimension(axisp),Rx);
	
#ifdef USE_PPP
	Overture::abort("fix me");
#else
	int option=0, degree=3;
	const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
	startCurve.interpolate(xbb,option,Overture::nullRealDistributedArray(),degree,
			       (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization,numGhost);
	//startCurve.interpolate(xbb,option,Overture::nullRealDistributedArray(),degree,
	//                       NurbsMapping::parameterizeByChordLength,numGhost);
#endif
	
	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
	xbb.reshape(Ib1,Ib2,Ib3,Rx);
      }
    }
    else if( numberOfDimensions==3)
    {
      Overture::abort("error");
    }
	
    tx0=t3; // x0 points now live at this time

    
  } // end for face
  
  return ierr;
}


// ===========================================================================================
/// \brief Advance the elastic beam in time.
///
/// \param option : option=0 : predictor-step, option=1 corrector step. 
///
/// 
/// 
// ===========================================================================================
int DeformingBodyMotion::
advanceNonlinearBeam(real t1, real t2, real t3, 
		     GridFunction & cgf3,
		     realCompositeGridFunction & stress,
		     int option )
{
  int ierr=0;

  const int numberOfDimensions = cgf3.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  assert( deformingBodyType==userDefinedDeformingBody );
  assert( userDefinedDeformingBodyMotionOption==nonlinearBeam );

  real tForce=t2;
  real tNew=t3;
  
  real bf[2] = {0.0,0.0};
  if (parameters.dbase.has_key("gravity")) {
    for (int k = 0; k < 2; ++k)
      bf[k] = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity")[k]; 
  }
  
  pNonlinearBeamModel->addBodyForce(bf);

  if( debug&2 )
    printF("--DeformingBodyMotion::advanceElasticBeam called for tNew=%f, forces at t=%f\n",tNew, tForce);
  
  Index Ib1,Ib2,Ib3;

  // We need to grab the surface points from cgf1
  real dt = t3-t2;   // do this for now -- assume the solution x0 is at time t2

  if (option == 1)
    dt = t2-t1;

  pNonlinearBeamModel->resetForce();  // *wdh* moved outside loop over faces.

  // --------------------------------------
  // --- LOOP over faces on the surface ---
  // --------------------------------------
  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    const int uc = parameters.dbase.get<int >("uc");
    const int vc = parameters.dbase.get<int >("vc");
    const int wc = parameters.dbase.get<int >("wc");
    const int pc = parameters.dbase.get<int >("pc");

    // The "surface" Mapping holds the start curve in 2D
    vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
    assert( face<surface.size() );
    NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);


    // The undeformed surface, x0,  is stored here: 
    vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
    vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
    assert( face<surfaceArray.size() );
    RealArray *px = surfaceArray[face];
    RealArray &x0 = px[0];
    assert( face<surfaceArrayTime.size() );
    real & tx0= surfaceArrayTime[face][0];

    const int numGhost=0;  // include ghost points 
    Range Rx=numberOfDimensions;
    // --- The current surface position and velocity are stored in the GridFunction data-base ---

    getBoundaryIndex(cgf3.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);
    
#ifdef USE_PPP
    RealArray u2; getLocalArrayWithGhostBoundaries(cgf3.u[gridToMove],u2);
    const RealArray & normal2 = cgf3.cg[gridToMove].vertexBoundaryNormalArray(sideToMove,axisToMove);
    RealArray vertex2; getLocalArrayWithGhostBoundaries(cgf3.cg[gridToMove].vertex(),vertex2);
#else
    RealArray & u2 = cgf3.u[gridToMove];
    const RealArray & normal2 = cgf3.cg[gridToMove].vertexBoundaryNormal(sideToMove,axisToMove);
    const RealArray & vertex2 = cgf3.cg[gridToMove].vertex();
#endif
    

    RealArray xbb  =x0;

    if( numberOfDimensions==2 )
    {
      assert( uc>=0 && vc>=0 && pc>=0 );
      int axisp = (axisToMove + 1) % numberOfDimensions;  // axis in the tangential direction 
      // Index I1=x0.dimension(0), I2=x0.dimension(1);
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      int ivp[3], &i1p=ivp[0], &i2p=ivp[1], &i3p=ivp[2];
      int ivm[3], &i1m=ivm[0], &i2m=ivm[1], &i3m=ivm[2];
      assert(axisp != 2);
      Index Ibb1 = Ib1,Ibb2 = Ib2,Ibb3 = Ib3;
      if (axisp == 0)
	Ibb1 = Index(Ib1.getBase(),Ib1.getBound()-Ib1.getBase());
      else if (axisp == 1)
	Ibb2 = Index(Ib2.getBase(),Ib2.getBound()-Ib2.getBase());
      
      { FOR_3D(i1,i2,i3,Ibb1,Ibb2,Ibb3)
      {
	// int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
	i1p=i1, i2p=i2, i3p=i3;
	i1m=i1, i2m=i2, i3m=i3;
	ivp[axisp]+=1;
	ivm[axisp]-=1;

	int idx = (axisp == 0 ? i1 : i2);
	pNonlinearBeamModel->addForce(idx,u2(i1,i2,i3,pc),
				      idx+1,
				      u2(i1p,i2p,i3p,pc));
      }} // end for
      
      if (option == 0)
	pNonlinearBeamModel->predictor(dt);
      else if (option == 1)
	pNonlinearBeamModel->corrector(dt);
      
      if (option == 0) {
	
	{ FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	    {
	      int idx = (axisp == 0 ? i1 : i2);
	      pNonlinearBeamModel->projectDisplacement(idx,
					      xbb(i1,i2,i3,0),
					      xbb(i1,i2,i3,1));
					      } }
	
	int xxx =xbb.dimension(axisp).length();
	xbb.reshape(xbb.dimension(axisp),Rx);
	
#ifdef USE_PPP
	Overture::abort("fix me");
#else
	int option=0, degree=3;
	const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
	startCurve.interpolate(xbb,option,Overture::nullRealDistributedArray(),degree,
			       (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization,numGhost);
	//startCurve.interpolate(xbb,option,Overture::nullRealDistributedArray(),degree,
	//                       NurbsMapping::parameterizeByChordLength,numGhost);
#endif
	
	getBoundaryIndex(cgf3.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3,numGhost);
	//xbb.reshape(Ib1,Ib2,Ib3,Rx);
      }
    }
    else if( numberOfDimensions==3)
    {
      Overture::abort("error");
    }
	
    tx0=t3; // x0 points now live at this time

    
  } // end for face
  
  return ierr;
}


//\begin{>>DeformingBodyMotionInclude.tex}{\subsection{integrate}} 
int DeformingBodyMotion::
integrate( real t1, real t2, real t3, 
           GridFunction & cgf1,
           GridFunction & cgf2,
           GridFunction & cgf3,
           realCompositeGridFunction & stress )
//==================================================================================
// /Description:
//    Integrate the Deforming body equations from time t1 to time t3 using the solution at time t2
//   to compute the forces etc. 
// \begin{verbatim}
//          u3(t3) <- u1(t1) + (t3-t1)*d(u2(t2))/dt
// \end{verbatim}
//
//  cgf1 and cgf3 must be DIFFERENT
//
//  /t1,cgf1 (input): grid and solution at time t1
//  /t2,cgf2 (input): grid velocity is taken from this time
//
//  /cgf3 (output): cgf3 can be used to hold the new solution at time t3 for variables that are
//         computed with by Deforming body class. 
//
//
/// NOTES: 
/// The routine MovingGrids::movingGrids first calls DeformingBodyMotion::integrate
/// to move the deforming body 
/// and then calls DeformingBodyMotion::regenerateComponentGrids to actually move the grid.
///
//\end{DeformingBodyMotionInclude.tex}  
//=========================================================================================
{

  const int numberOfDimensions = cgf1.cg.numberOfDimensions();
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
    deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  const bool & smoothSurface = deformingBodyDataBase.get<bool>("smoothSurface");
  const int & numberOfSurfaceSmooths = deformingBodyDataBase.get<int>("numberOfSurfaceSmooths");

  CompositeGrid & cg = cgf3.cg;

  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  real tForce=t2;
  real tNew=t3;
  
  if( debug&2 )
    printF("--DeformingBodyMotion::Integrate called for tNew=%f, forces at t=%f\n",tNew, tForce);
  int ierr=0;

  if( deformingBodyType==userDefinedDeformingBody && 
      userDefinedDeformingBodyMotionOption==elasticShell )
  {
    int option=0;  // predictor 
    ierr=advanceElasticShell(t1,t2,t3,cgf1,cgf2,cgf3,stress,option);
    return ierr;
   
  }
  else if( deformingBodyType==userDefinedDeformingBody && 
	   userDefinedDeformingBodyMotionOption==userDeformingSurface )
  {
    int option=0;  // predictor 
    ierr=parameters.userDefinedDeformingSurface(*this,t1,t2,t3,cgf1,cgf2,cgf3,option);
    return ierr;
  } 
  else if( deformingBodyType==userDefinedDeformingBody && 
	     userDefinedDeformingBodyMotionOption==elasticBeam )
  {
    int option=0;  // predictor 
    ierr=advanceElasticBeam(t1,t2,t3,cgf1,cgf2,cgf3,stress,option);
    return ierr;
   
  }
  else if( deformingBodyType==userDefinedDeformingBody && 
	     userDefinedDeformingBodyMotionOption==nonlinearBeam )
  {
    int option=0;  // predictor 
    ierr=advanceNonlinearBeam(t1,t2,t3,cgf3,stress,option);
    return ierr;
   
  }

  for( int face=0; face<numberOfFaces; face++ )
  {
    int sideToMove=boundaryFaces(0,face);
    int axisToMove=boundaryFaces(1,face);
    int gridToMove=boundaryFaces(2,face); 

    assert( gridToMove>=0 && gridToMove<numberOfComponentGrids );

    if( deformingBodyType==elasticFilament )
    {
      //call DeformingBodySolver --> now just special case=ElasticFilament
      assert( pElasticFilament!=NULL );
      ierr=pElasticFilament->integrate(tForce, stress, tNew);
      //int pElasticFilament->evaluateSurfaceAtTime( tNew );
    }
    else if( deformingBodyType==userDefinedDeformingBody )
    {

      if(  userDefinedDeformingBodyMotionOption==advectBody )
      {
        // advect the body from time t1 to t3 using velocity from time t2 

	if( debug & 2 )
	  printF("DeformingBodyMotion:integrate called for userDefinedDeformingBody for t=%9.3e\n",tNew);

	const int uc = parameters.dbase.get<int >("uc");
	const int vc = parameters.dbase.get<int >("vc");
	const int wc = parameters.dbase.get<int >("wc");

	vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
        vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
	assert( face<surfaceArray.size() );
	RealArray *px = surfaceArray[face];
	RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
        assert( face<surfaceArrayTime.size() );
	real & tx0= surfaceArrayTime[face][0];
	
	// Mapping & map = cg[gridToMove].mapping().getMapping();
	// assert( map.getClassName()=="HyperbolicMapping" );
	// HyperbolicMapping & hyp = (HyperbolicMapping&)map;
    
        // We need to grab the surface points from cgf1
        // const real dt = t3-t2;   // do this for now -- assume the solution x0 is at time t2
        // *wdh* 081211 -- use the proper time (needed for advancePC which takes an initial back step to t=-dt)
        const real dt = t3-tx0;   
        tx0=t3; // x0 will now live at this time
        realArray & u = cgf2.u[gridToMove];
	
	Index Ib1,Ib2,Ib3;
	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);

	if( numberOfDimensions==2 )
	{
          assert( uc>=0 && vc>=0 );
          int axisp = (axisToMove + 1) % numberOfDimensions;
	  Index I1=x0.dimension(0), I2=x0.dimension(1);
	  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
          FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
            int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
	    real x = x0(j1,0), y=x0(j1,1);
	    real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc);
	      
            // u0=1.;  v0=0.; // do this for testing 
	     
	    x0(j1,0) = x + dt*u0;
	    x0(j1,1) = y + dt*v0;

	    // printF("advectBody: x=(%5.2f,%5.2f) u=(%5.2f,%5.2f) x0=(%5.2f,%5.2f)\n",x,y,u0,v0,x0(j1,0),x0(j1,1));
	    
	  }
          // -- Add a fourth-order filter to the new positions --
	  if( smoothSurface )
	  {
	    printF("advectBody: smooth the boundary curve, numberOfSurfaceSmooths=%i (4th order filter)...\n",
                   numberOfSurfaceSmooths);
	    
            const int base=x0.getBase(0), bound=x0.getBound(0);
            RealArray x1(Range(base-2,bound+2),2);  // add 2 ghost points
            Range R2(0,1);
            x1(I1,R2)=x0(I1,R2);

            // I : smooth these points. Keep the boundary points fixed, except for periodic .. fix me ..
            Range I(base+1,bound-1); 

	    real omega=.5;
	    for( int smooth=0; smooth<numberOfSurfaceSmooths; smooth++ )
	    {
              // -- boundary conditions  **FIX ME for periodic**
              x1(base-1,R2) = 2.*x1(base,R2)-x1(base+1,R2);  // what should this be ??
              x1(base-2,R2) = 2.*x1(base,R2)-x1(base+2,R2);
	      
              x1(bound+1,R2) = 2.*x1(bound,R2)-x1(bound-1,R2);
              x1(bound+2,R2) = 2.*x1(bound,R2)-x1(bound-2,R2);
	      
	      // smooth interior: 
	      
	      x1(I,R2)= x1(I,R2) + (omega/16.)*(-x1(I-2,R2) + 4.*x1(I-1,R2) -6.*x1(I,R2) + 4.*x1(I+1,R2) -x1(I+2,R2) );
	      
	    } // end smooths
	    
	    x0(I1,R2)= x1(I1,R2);

	  } // end filterSurface

	}
	else if( numberOfDimensions==3)
	{
          assert( uc>=0 && vc>=0 && wc>= 0 );

	  Index I1=x0.dimension(0), I2=x0.dimension(1);
	  int i1,i2,i3;
	  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	    {
	      real x = x0(i1,i2,0), y=x0(i1,i2,1), z=x0(i1,i2,2);
	      real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc), w0=u(i1,i2,i3,wc);
	      
	      x0(i1,i2,0) = x + dt*u0;
	      x0(i1,i2,1) = y + dt*v0;
	      x0(i1,i2,2) = z + dt*w0;

	    }
	}
	
	// The "surface" Mapping holds the start curve in 2D
	vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
	assert( face<surface.size() );
	NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

	// *** do this for now *** FIX ME ******************************************************
	// startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);

	// startCurve.setIsPeriodic(axis1,Mapping::derivativePeriodic);

        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
	  // *wdh* 081107 startCurve.interpolate(x0);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x0,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
        // NOTE: We should evaluate the GRID-POINTS at interpolation pts by interpolating
        //       the grid-points from other grids -- this should make sure that overlapping grids
        //       stay connected. 
	
      }
      else if( userDefinedDeformingBodyMotionOption==interfaceDeform )
      {
        // ----------------------------------------------------------------------------------------------------
        // --- The interface is defined by the "boundaryData" array used for the RHS to boundary conditions ---
        // ----------------------------------------------------------------------------------------------------

	if( debug & 4 )
	  printF("DeformingBodyMotion:integrate called for interfaceDeform at t=%9.3e\n",tNew);

	

	// Here is the array that defines the domain interfaces, interfaceType(side,axis,grid) 
	const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");


        RealArray & bd = parameters.getBoundaryData(sideToMove,axisToMove,gridToMove,cg[gridToMove]);
 	if( debug & 8  )
 	{
 	  ::display(bd,"DeformingBodyMotion::integrate:interfaceDeform: Here is the boundary data","%6.3f ");
 	}
	
	const int uc = parameters.dbase.get<int >("uc");
	const int vc = parameters.dbase.get<int >("vc");
	const int wc = parameters.dbase.get<int >("wc");

	vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
        vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
	assert( face<surfaceArray.size() );
	RealArray *px = surfaceArray[face];
	RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
        assert( face<surfaceArrayTime.size() );
	real & tx0= surfaceArrayTime[face][0];

	// Mapping & map = cg[gridToMove].mapping().getMapping();
	// assert( map.getClassName()=="HyperbolicMapping" );
	// HyperbolicMapping & hyp = (HyperbolicMapping&)map;
    
        // We need to grab the surface points from cgf1
        // *wdh* const real dt = t3-t2;   // do this for now -- assume the solution x0 is at time t2
        // *wdh* 081211 -- use the proper time (needed for advancePC which takes an initial back step to t=-dt)
        const real dt = t3-tx0;   
        tx0=t3; // x0 will not live at this time

        realArray & u = cgf2.u[gridToMove];
	
	Index Ib1,Ib2,Ib3;
	getBoundaryIndex(cgf1.cg[gridToMove].gridIndexRange(),sideToMove,axisToMove,Ib1,Ib2,Ib3);
	int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
	
        // *** fix me for 3D ***
        assert( numberOfDimensions==2 );

	if( Ib1.getLength()>1 ) // *** NOTE: we know which (side,axis) the boundaryData lives on -- fix this --
	{ // the boundary follows "i1"
	  if( x0.getBase()!=Ib1.getBase() || x0.getBound(0)!=Ib1.getBound() )
	  {
	    printF("DeformingBodyMotion::integrate:interfaceDeform:ERROR: boundary data array does not "
		   "match the start curve dimensions!\n"
		   " gridToMove=%i sideToMove=%i axisToMove=%i uc=%i\n"
		   " Ib1=[%i,%i], Ib2=[%i,%i], Ib2=[%i,%i],   x0 : [%i,%i]\n",
		   gridToMove,sideToMove,axisToMove,uc,
		   Ib1.getBase(),Ib1.getBound(),
		   Ib2.getBase(),Ib2.getBound(),
		   Ib3.getBase(),Ib3.getBound(),
		   x0.getBase(0),x0.getBound(0));
	    Overture::abort("error");
	  }
            
	  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
	    for( int m=0; m<numberOfDimensions; m++ )
	      x0(i1,m)=bd(i1,i2,i3,m+uc);
	  }
	}
	else
	{ // the boundary follows "i2"
	  if( x0.getBase()!=Ib2.getBase() || x0.getBound(0)!=Ib2.getBound() )
	  {
	    printF("DeformingBodyMotion::integrate:interfaceDeform:ERROR: boundary data array does not "
		   "match the start curve dimensions!\n"
		   " gridToMove=%i sideToMove=%i axisToMove=%i uc=%i\n"
		   " Ib1=[%i,%i], Ib2=[%i,%i], Ib2=[%i,%i],   x0 : [%i,%i]\n",
		   gridToMove,sideToMove,axisToMove,uc,
		   Ib1.getBase(),Ib1.getBound(),
		   Ib2.getBase(),Ib2.getBound(),
		   Ib3.getBase(),Ib3.getBound(),
		   x0.getBase(0),x0.getBound(0));
	    Overture::abort("error");
	  }
	    

	  FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	  {
	    for( int m=0; m<numberOfDimensions; m++ )
	      x0(i2,m)=bd(i1,i2,i3,m+uc);
	  }
	}
	  
	// The "surface" Mapping holds the start curve in 2D
	vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
	assert( face<surface.size() );
	NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

	// -- Add a fourth-order filter to the new positions -- *wdh* 2014/04/20
	if( smoothSurface )
	{
	    
	  const int base=x0.getBase(0), bound=x0.getBound(0);
	  RealArray x1(Range(base-2,bound+2),2);  // add 2 ghost points
	  Range R2(0,1);
          Range I1(base,bound);
	  x1(I1,R2)=x0(I1,R2);

          bool isPeriodic = startCurve.getIsPeriodic(axis1)==Mapping::functionPeriodic;
	  printF("interfaceDeform: smooth boundary curve, numberOfSurfaceSmooths=%i (4th order filter),\n"
                 "   curve: base=%i, bound=%i, isPeriodic=%i\n",
		 numberOfSurfaceSmooths,base,bound,(int)isPeriodic );
	  
	  // I : smooth these points. Keep the boundary points fixed, except for periodic
          const int i1a= isPeriodic ? base : base+1;
	  const int i1b= isPeriodic ? bound : bound-1;
	  Range I(i1a,i1b); 

	  real omega=.5;
	  for( int smooth=0; smooth<numberOfSurfaceSmooths; smooth++ )
	  {
	    // -- boundary conditions  **FIX ME for periodic**
            if( isPeriodic )
	    {
              x1(base-1,R2)=x1(bound-1,R2);
              x1(base-2,R2)=x1(bound-2,R2);
              x1(bound+1,R2)=x1(base+1,R2);
              x1(bound+2,R2)=x1(base+2,R2);
	    }
	    else
	    {
	      x1(base-1,R2) = 2.*x1(base,R2)-x1(base+1,R2);  // what should this be ??
	      x1(base-2,R2) = 2.*x1(base,R2)-x1(base+2,R2);
	      
	      x1(bound+1,R2) = 2.*x1(bound,R2)-x1(bound-1,R2);
	      x1(bound+2,R2) = 2.*x1(bound,R2)-x1(bound-2,R2);
	    }
	    
	    // smooth interior: 
	      
	    x1(I,R2)= x1(I,R2) + (omega/16.)*(-x1(I-2,R2) + 4.*x1(I-1,R2) -6.*x1(I,R2) + 4.*x1(I+1,R2) -x1(I+2,R2) );
	      
            if( isPeriodic )
	    {
              x1(bound,R2)=x1(base,R2);
	    }
	    
	  } // end smooths
	    
	  x0(I1,R2)= x1(I1,R2);

	} // end filterSurface

	if( debug & 8  )
	{
	  printF("*** DeformingBodyMotion::integrate: gridToMove=%i sideToMove=%i axisToMove=%i uc=%i\n",
		 gridToMove,sideToMove,axisToMove,uc);
	  ::display(x0,"DeformingBodyMotion::integrate:interfaceDeform: Here are the start curve pts","%7.4f ");
	}

	
        // -- OLD STUFF --
	// else if( numberOfDimensions==2 )
	// {
        //   // ***** finish me *****

        //   assert( uc>=0 && vc>=0 );

        //   int axisp = (axisToMove + 1) % numberOfDimensions;
	//   Index I1=x0.dimension(0), I2=x0.dimension(1);
        //   FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	//   {
        //     int j1 = iv[axisp];  // j1 = i1 or i2 or i3 
	//     real x = x0(j1,0), y=x0(j1,1);
	//     // real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc);
	      
        //     real u0=1.,  v0=0.; 
	     
	//     x0(j1,0) = x + dt*u0;
	//     x0(j1,1) = y + dt*v0;

	//     printF("interfaceDeform: x=(%5.2f,%5.2f) u=(%5.2f,%5.2f) x0=(%5.2f,%5.2f)\n",x,y,u0,v0,x0(j1,0),x0(j1,1));
	    
	//   }

	// }
	// else if( numberOfDimensions==3)
	// {
        //   // ***** finish me *****

        //   assert( uc>=0 && vc>=0 && wc>= 0 );

	//   Index I1=x0.dimension(0), I2=x0.dimension(1);
	//   for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	//     for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	//     {
	//       real x = x0(i1,i2,0), y=x0(i1,i2,1), z=x0(i1,i2,2);
	//       // real u0=u(i1,i2,i3,uc), v0=u(i1,i2,i3,vc), w0=u(i1,i2,i3,wc);
	      
        //       real u0=1., v0=0., w0=0.; 

	//       x0(i1,i2,0) = x + dt*u0;
	//       x0(i1,i2,1) = y + dt*v0;
	//       x0(i1,i2,2) = z + dt*w0;

	//     }
	// }
	

	// *** do this for now *** FIX ME ******************************************************
	// startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);

	// startCurve.setIsPeriodic(axis1,Mapping::derivativePeriodic);

        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
	  // *wdh* 081107 startCurve.interpolate(x0);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x0,option,Overture::nullRealDistributedArray(),degree,
                                (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
        // NOTE: We should evaluate the GRID-POINTS at interpolation pts by interpolating
        //       the grid-points from other grids -- this should make sure that overlapping grids
        //       stay connected. 
	
      }
      else if( userDefinedDeformingBodyMotionOption==elasticShell ||
               userDefinedDeformingBodyMotionOption==userDeformingSurface )
      {
        // this is done above 


      } else if( userDefinedDeformingBodyMotionOption==elasticBeam)
      {
        // this is done above 


      } else if( userDefinedDeformingBodyMotionOption==nonlinearBeam)
      {
        // this is done above 


      }

    }
    else
    {
      Overture::abort("ERROR: unknown deformingBodyType");
    } 
  }
  
  return ierr;
}


//==================================================================================
/// \brief Corrector step for deforming grids.
/// \details This function is called at the corrector step to update the moving grids. 
///  the acceleration on the boundary ).
/// \param t1,cgf1 (input) : solution at the old time
/// \param t2,cgf2 (input) : solution at the new time (these are valid values)
//==================================================================================
int DeformingBodyMotion::
correct( real t1, real t2, 
	 GridFunction & cgf1,GridFunction & cgf2 )
{
  if(debug&2)
    printF("--DeformingBodyMotion::correct called at t=%f (Not Implemented!!)\n", t2);

  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

  int ierr=0;
  if( deformingBodyType==elasticFilament )
  {
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    Range all;
    RealCompositeGridFunction stress(cgf2.cg,all,all,all,cgf2.cg.numberOfDimensions()); // **** fix this ****
    stress=0.; // do we need this?

    // *wdh* new 081206  -- add something like this ---
   // regenerateComponentGrids( t2,cgf2.cg,correctGrid );
    if(userDefinedDeformingBodyMotionOption==elasticBeam ) {

      realCompositeGridFunction empty;
      int option=1;  // corrector
      ierr=advanceElasticBeam(t1,t2,t2,cgf1,cgf2,cgf2,stress,option);
      //regenerateComponentGrids( t2,cgf2.cg);
    }
    else if(userDefinedDeformingBodyMotionOption==nonlinearBeam ) {

      realCompositeGridFunction empty;
      int option=1;  // corrector
      int ierr=advanceNonlinearBeam(t1,t2,t2,cgf2,stress,option);
      //regenerateComponentGrids( t2,cgf2.cg);
    }
    else if( userDefinedDeformingBodyMotionOption==userDeformingSurface )
    {
      int option=1;  // corrector
      ierr=parameters.userDefinedDeformingSurface(*this,t1,t2,t2,cgf1,cgf2,cgf2,option);
    } 

  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  }
  return ierr;
}


//........Access routines 

// ===================================================================================
/// \brief: Set the deforming body type. 
///    One of elasticFilament or userDefinedDeformingBody. 
// ===================================================================================
int DeformingBodyMotion::
setType( const DeformingBodyType bodyType )
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  deformingBodyType=bodyType;
  return 0;
}


// ===================================================================================
/// \brief: regenerate all the component grids associated with this deforming body.
/// 
/// The routine MovingGrids::movingGrids first calls DeformingBodyMotion::integrate
/// to move the deforming body 
/// and then calls DeformingBodyMotion::regenerateComponentGrids to actually generate the grid.
/// 
// ===================================================================================
int DeformingBodyMotion::
regenerateComponentGrids( const real newT, CompositeGrid & cg)
{
  int & numberOfDeformingGrids = deformingBodyDataBase.get<int>("numberOfDeformingGrids");
  int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
  IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  // do this for now -- treat more grids later 
  for( int face=0; face<numberOfFaces; face++ )
  {
    int gridToMove= boundaryFaces(2,face);  

    if( deformingBodyType==elasticFilament )
    {
      assert( numberOfFaces==1 );

      if( debug&4 )
	printF("DeformingBodyMotion::regenCompGrid called, gridToMove=%i\n",gridToMove);

      assert( pElasticFilament!=NULL );
      assert(pDeformingGrid   !=NULL );

      HyperbolicMapping *pHyper;
      aString newMappingName;

      pDeformingGrid->getNewMapping( newT, pHyper );
      assert( pHyper != NULL );
      pElasticFilament->evaluateSurfaceAtTime(newT);
      if (debug &4) 
      {
	pDeformingGrid->createMappingName( newT, newMappingName );
	pElasticFilament->copyBodyFittedMapping( *pHyper, &newMappingName );
	cout << "DeformingBodyMotion::regenCompGrid -- "
	     << "look at stored time level\n";
	if(pMapInfoDebug!=NULL) pHyper->update( *pMapInfoDebug ); 
      } 
      else 
      {
	//cout << "DeformingBodyMotion::regenCompGrid -- "
	//	 << "look at stored time level @@ NON DEBUG\n";
	pElasticFilament->copyBodyFittedMapping( *pHyper ); //non debug code
      }
      pElasticFilament->referenceMap( gridToMove,  cg); //TRYING TO FORCE FILAM TO SAVE?? Is this needed feb26,2001**pf??
    }
    else if( deformingBodyType==userDefinedDeformingBody )
    {

      if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
	deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);
      UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
	deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");

      if( debug & 2 )
	printF("DeformingBodyMotion:regenerateComponentGrid: userDefinedDeformingBody for gridToMove=%i, t=%9.3e\n",
	       gridToMove,newT);
    
      vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
      vector<real*> & surfaceArrayTime = deformingBodyDataBase.get<vector<real*> >("surfaceArrayTime"); 
      assert( face<surfaceArray.size() );
      RealArray *px = surfaceArray[face];
      RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
      assert( face<surfaceArrayTime.size() );
      real & tx0= surfaceArrayTime[face][0];

      Mapping & map = cg[gridToMove].mapping().getMapping();
      assert( map.getClassName()=="HyperbolicMapping" );
      HyperbolicMapping & hyp = (HyperbolicMapping&)map;
    
      // The "surface" Mapping holds the start curve in 2D
      vector<Mapping*> & surface = deformingBodyDataBase.get<vector<Mapping*> >("surface");
      assert( face<surface.size() );
      NurbsMapping & startCurve = *((NurbsMapping*)surface[face]);

      if( !deformingBodyDataBase.has_key("deformationFrequency") )
	deformingBodyDataBase.put<real>("deformationFrequency",1.);
      real & deformationFrequency = deformingBodyDataBase.get<real>("deformationFrequency");
      if( !deformingBodyDataBase.has_key("deformationAmplitude") )
	deformingBodyDataBase.put<real>("deformationAmplitude",1.);
      real & deformationAmplitude = deformingBodyDataBase.get<real>("deformationAmplitude");


      real omega=pow(sin(deformationFrequency*Pi*newT),2.);  // omega varies in the interval [0,1]
    
      if( userDefinedDeformingBodyMotionOption==iceDeform )
      {
	if( true )
	{
	  // perturbation varies along the axis of of the cylinder:
	  Range I1=x2.dimension(0), I2=x2.dimension(1);
	  int ns= max(2,I2.getLength());
	  for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	  {
	    real s = real(i2)/(ns-1);  // axial variable

	    real varOmega=deformationAmplitude*omega*SQR(cos(2.*Pi*s));

	    x2(I1,i2) = (1.-varOmega)*x0(I1,i2) + varOmega*x1(I1,i2);
	  }
      
	}
	else
	{
	  x2 = (1.-omega)*x0 + omega*x1;
	}

        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
	  // *wdh* 081107 startCurve.interpolate(x2);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x2,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
      }
      else  if( userDefinedDeformingBodyMotionOption==ellipseDeform )
      {
	real rad=1.;  // initial radius 
	real da=rad*.1*deformationAmplitude;
	real db=rad*.1*deformationAmplitude;
	real a = rad + da*sin(omega);
	real b = rad - db*sin(omega);
      
	const int n1=51;
	x0.redim(n1,2);

	for( int i=0; i<n1; i++ )
	{
	  real theta = i*2.*Pi/(n1-1);
	  x0(i,0) = a*cos(theta);
	  x0(i,1) = b*sin(theta);
	}

	startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);
        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
	  // *wdh* 081107 startCurve.interpolate(x0);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x0,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
      }
      else  if( userDefinedDeformingBodyMotionOption==linearDeform )
      {
        // define the interface at time t=newT

	real *rpar = deformingBodyDataBase.get<real[2]>("linearDeformParameter");
	real ap=rpar[0], pp=rpar[1];
	real g = ap*pow(newT,pp);      // position of the interface

	printF("DeformingBodyMotion: linearDeform: t=%9.3e, (a,p)=(%8.2e,%8.2e) g(t)=%9.3e\n",newT,ap,pp,g);

        real xa=g, xb=g, ya=0., yb=1.;
	const int n1=51;
	x0.redim(n1,2);

	for( int i=0; i<n1; i++ )
	{
	  real r  = real(i)/(n1-1);
	  x0(i,0) = xa + (xb-xa)*r;
	  x0(i,1) = ya + (yb-ya)*r;
	}


	// startCurve.setIsPeriodic(axis1,Mapping::functionPeriodic);  // ********************

        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x0,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
      }
      else  if( userDefinedDeformingBodyMotionOption==sphereDeform )
      {
        // Here is a sphere that expands to an ellipse

        // -- The array x0(i1,i2,0:2) holds the grid points on the original surface --

        real omega=sin(deformationFrequency*Pi*newT);  // omega varies in the interval [-1,1]

        real a0= .8*deformationAmplitude, 
             b0=1.2*deformationAmplitude, 
             c0=1.2*deformationAmplitude;  // final ellipse shape

        real a  = (1. + (a0-1.)*omega); 
        real b  = (1. + (b0-1.)*omega); 
        real c  = (1. + (c0-1.)*omega); 

        // realArray x0;
	// x0= startCurve.getGrid();
        Index I1=x0.dimension(0), I2=x0.dimension(1);
        // x0.reshape(I1,I2,3);
        RealArray x1(I1,I2,cg.numberOfDimensions());
        int i1,i2,i3;
	for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	{
	  real x = x0(i1,i2,0), y=x0(i1,i2,1), z=x0(i1,i2,2);
	  real r = sqrt(x*x + y*y + z*z);
	  
          x1(i1,i2,0) = x*a;
          x1(i1,i2,1) = y*b;
          x1(i1,i2,2) = z*c;
	  

	}
        #ifdef USE_PPP
  	  Overture::abort("fix me");
        #else
	  // *wdh* 081107 startCurve.interpolate(x1);
          int option=0, degree=3;
          const int boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
          startCurve.interpolate(x1,option,Overture::nullRealDistributedArray(),degree,
                                 (NurbsMapping::ParameterizationTypeEnum)boundaryParameterization);
        #endif
      }
      else if( userDefinedDeformingBodyMotionOption==advectBody ||
               userDefinedDeformingBodyMotionOption==interfaceDeform ||
               userDefinedDeformingBodyMotionOption==elasticShell ||
               userDefinedDeformingBodyMotionOption==elasticBeam ||
               userDefinedDeformingBodyMotionOption==nonlinearBeam ||
               userDefinedDeformingBodyMotionOption==userDeformingSurface)
      {
      }
      else
      {
	printF("DeformingBodyMotion:regenerateComponentGrid:ERROR: unexpected value for "
               "userDefinedDeformingBodyMotionOption=%i\n",
	       (int)userDefinedDeformingBodyMotionOption);
	Overture::abort("ERROR: unexpected value for userDefinedDeformingBodyMotionOption!");
      }

      if( debug & 2 )
	printF("DeformingBodyMotion:regenerateComponentGrid: call hype to generate the grid t=%9.3e\n",
	       newT);

      // printF("DeformingBodyMotion::regenerateComponentGrid: BEFORE hyp.setSurface spacingOption=%i\n",hyp.spacingOption);

      const bool isSurfaceGrid=false; // this is a volume grid (in 2d or 3d) 
      const bool init=false; // this means keep existing hype parameters such as distanceToMarch, linesToMarch etc.
      hyp.setSurface(startCurve,isSurfaceGrid,init);

      // printF("DeformingBodyMotion::regenerateComponentGrid: AFTER hyp.setSurface spacingOption=%i\n",hyp.spacingOption);
      if( debug & 4 )
      {
	printF("DeformingBodyMotion::regenerateComponentGrid: HyperbolicMapping Parameters:");
	hyp.display();
      }

      bool & changeHypeParameters = deformingBodyDataBase.get<bool>("changeHypeParameters");
      if( changeHypeParameters )
      {
        // --- Allow the user to change the hyperbolic grid generation parameters ---

	changeHypeParameters=false;  // turn off 

	GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
	
        gi.stopReadingCommandFile();
	printF("DeformingBodyMotion:regenerateComponentGrid: gridToMove=%i, t=%9.3e\n",gridToMove,newT);
	hyp.interactiveUpdate(gi);

      }
      else
      {

       
        // *************************************************************
        // ************* Generate the new hyperbolic grid **************
        // *************************************************************
	int returnCode = hyp.generate();


        if( returnCode!=0 )
	{
	  printF("DeformingBodyMotion:regenerateComponentGrid:ERROR return from HyperbolicMapping::generate\n"
                 "  ... I am going to enter interactive mode so you can generate the hyperbolic grid interactively\n");
	  printF("  ... gridToMove=%i, t=%9.3e\n",gridToMove,newT);

	  if( true )
	    display(startCurve.getGrid(),"Here are points on the start curve","%6.2f");

	  GenericGraphicsInterface & gi = *Overture::getGraphicsInterface();
	
	  gi.stopReadingCommandFile();
	  hyp.interactiveUpdate(gi);
	}
	
      }
      

#ifndef USE_PPP
      // turn this off temporarily for PPP until we update Overture
      const DataPointMapping *pdpm = hyp.getDataPointMapping();
#else
      DataPointMapping *pdpm = NULL; 
#endif
      assert( pdpm!=NULL );
      DataPointMapping & dpm = (DataPointMapping&)(*pdpm);

      // *note* 101102 - The hyperbolic grid generator DOES keep the order of interpolation in the DPM from that specified in ogen
      // if( true )
      //   printF("DeformingBodyMotion::After hype: dpm.getOrderOfInterpolation()=%i\n",dpm.getOrderOfInterpolation());
      

      // dpm.getDataPoints() includes ghost points

      vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution"); 

      assert( face>=0 && face<gridEvolution.size() );
      
      gridEvolution[face]->addGrid(dpm.getDataPoints(),newT);
    

    }
    else
    {
      Overture::abort("ERROR: unknown deformingBodyType");
    }  
  }  // end for face
  
  return 0;
}


int DeformingBodyMotion::
update(  GenericGraphicsInterface & gi )
// ***** check this -- called by MovingGrids ---
{
  // GUI --> ADD
  printF("DeformingBodyMotion::update called\n");
  int ierr=0;

  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    assert( pElasticFilament!= NULL );
    ierr=pElasticFilament->update( gi );
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  }  
  return ierr;
}

//..DEBUG interface
void DeformingBodyMotion::
printFilamentHyperbolicDimensions(CompositeGrid & cg, int gridToMove)
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    Mapping *pMap = (FilamentMapping*) cg[gridToMove].mapping().mapPointer;
    assert( pMap != NULL);
    if ( pMap->getClassName() == "FilamentMapping" )
    {
      FilamentMapping *pTempFilament = (FilamentMapping*) pMap;
      assert( pTempFilament != NULL );
  
      cout << "DBM.init Filam->pHyper->gridDimension(axis1) = " // **pf DEBUG
	   << pTempFilament->pHyper->getGridDimensions(axis1) << endl;
      cout << "DBM.init Filam->pHyper->gridDimension(axis2) = " // **pf DEBUG
	   << pTempFilament->pHyper->getGridDimensions(axis2) << endl;
    
      if ( (debug&2) && (pMapInfoDebug != NULL) ) 
      {
	cout << "DeformingBodyMotion::printHyperbDimensions "
	     << "LOOK AT THE *HYPERB* IN THE GRID!\n";
	pTempFilament->pHyper->update( *pMapInfoDebug );
	cout << "DeformingBodyMotion::printHyperbDimensions "
	     << "LOOK AT THE FILAM IN THE GRID!\n";
	pTempFilament->update( *pMapInfoDebug );
      }
    }
  }
  
}

int DeformingBodyMotion::
getPastLevelGrid( const int level, 
		  const int grid, 
		  CompositeGrid & cg,
		  realArray & gridVelocity)
{
  if(debug&4)
    cout << "DeformingBodyMotion::getPastLevelGrid, grid="
	 <<  grid << ", level=" << level << " called\n";

  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    assert( pDeformingGrid != NULL );
    pDeformingGrid->getPastLevelGrid( grid, cg, level, gridVelocity);
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    printF("DeformingBodyMotion:getPastLevelGrid called userDefinedDeformingBody for grid=%i, level=%i\n",
	   grid,level);
    
    // update cg[grid] here 

  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  } 

  return 0;
}

void DeformingBodyMotion::
simpleGetVelocity( const real vTime, 
		   const int grid00, 
		   CompositeGrid & cg,
		   realArray & gridVelocity)
		   //   realArray & xpoints1, realArray & xpoints2)
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( deformingBodyType==elasticFilament )
  {
    assert( pDeformingGrid != NULL );
    pDeformingGrid->simpleGetVelocity( vTime,  grid00,  cg, gridVelocity);
    // xpoints1, xpoints2);
  }
  else if( deformingBodyType==userDefinedDeformingBody )
  {
    printF("DeformingBodyMotion:simpleGetVelocity called for userDefinedDeformingBody time=%9.3e\n",
	   vTime);
    gridVelocity=0.;
  }
  else
  {
    Overture::abort("ERROR: unknown deformingBodyType");
  }
  
}

//================================PRIVATE:

//..Handle component grids
//.......Should we have a 'deformingGridMapping'???
//.........Would know which Body it belongs to
//.........Could encapsulate Hyperb., DPM, or more general
void DeformingBodyMotion::
registerDeformingComponentGrid( const int grid, CompositeGrid & cg)
{
  // not needed?
}




// ===================================================================================================================
/// \brief Build the options dialog for the elastic shell parameters.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DeformingBodyMotion::
buildElasticShellOptionsDialog(DialogData & dialog )
{

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  dialog.setOptionMenuColumns(1);

  const aString bcNames[] = { "periodic",
			      "Dirichlet",
			      "Neumann",
                              "slide",
			      "" };  
  const int maxCommands=numberOfBoundaryConditions+1;
  aString bcCmd[maxCommands];

  GUIState::addPrefix(bcNames,"BC left: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC left:",bcCmd,bcNames,boundaryCondition(0,0) );

  GUIState::addPrefix(bcNames,"BC right: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC right:",bcCmd,bcNames,boundaryCondition(1,0) );

  GUIState::addPrefix(bcNames,"BC bottom: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC bottom:",bcCmd,bcNames,boundaryCondition(0,1) );

  GUIState::addPrefix(bcNames,"BC top: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC top:",bcCmd,bcNames,boundaryCondition(1,1) );


  if( !deformingBodyDataBase.has_key("elasticShellParameters") )
  {
    deformingBodyDataBase.put<real [10]>("elasticShellParameters");
    real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");
    // -- set defaults ---
    par[0]=10.; // rhoe
    par[1]=1.;  // te 
    par[2]=5.;  // ke 
    par[3]=.1;  // be 
    par[4]=1.;  // ad2 : coefficient of artificial diffusion  // real & elasticShellDensity = ;

    deformingBodyDataBase.put<bool>("constantVolumeElasticShell",false);
    deformingBodyDataBase.put<real>("integratedVolumeDiscrepancy",0.);  // holds int_t (1-V/V0) dt 
    deformingBodyDataBase.put<real>("volumePenalty");
    deformingBodyDataBase.get<real>("volumePenalty")=1.; // actual penalty is .5/dt 
  }
  
  real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");
  real & rhoe = par[0];
  real & te   = par[1];
  real & ke   = par[2];
  real & be   = par[3];
  real & ad2=   par[4];

  bool & constantVolumeElasticShell =  deformingBodyDataBase.get<bool>("constantVolumeElasticShell");
  real & volumePenalty =  deformingBodyDataBase.get<real>("volumePenalty");


  aString tbCommands[] = {"constant volume",
 			  ""};
  int tbState[10];
  tbState[0] = constantVolumeElasticShell;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


//   aString pbCommands[] = {"show file options...",
//                           "create a probe",
//                           "output periodically to a file",
// 			  ""};
//   aString *pbLabels = pbCommands;
//   int numRows=2;
//   dialog.setPushButtons( pbCommands, pbLabels, numRows ); 

  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "elastic shell density:";  sPrintF(textStrings[nt],"%g",rhoe); nt++;
  textLabels[nt] = "elastic shell tension:";  sPrintF(textStrings[nt],"%g",te); nt++;
  textLabels[nt] = "elastic shell stiffness:";  sPrintF(textStrings[nt],"%g",ke); nt++;
  textLabels[nt] = "elastic shell damping:";  sPrintF(textStrings[nt],"%g",be); nt++;
  textLabels[nt] = "elastic shell dissipation:";  sPrintF(textStrings[nt],"%g",ad2); nt++;
  textLabels[nt] = "volume penalty parameter:";  sPrintF(textStrings[nt],"%g",volumePenalty); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}

//================================================================================
/// \brief: Look for an elastic shell option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DeformingBodyMotion::
getElasticShellOption(const aString & answer,
		      DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");
  real & rhoe = par[0];
  real & te   = par[1];
  real & ke   = par[2];
  real & be   = par[3];
  real & ad2=   par[4];

  bool & constantVolumeElasticShell =  deformingBodyDataBase.get<bool>("constantVolumeElasticShell");
  real & volumePenalty =  deformingBodyDataBase.get<real>("volumePenalty");

  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  if(      dialog.getTextValue(answer,"elastic shell density:","%g",rhoe) ){} // 
  else if( dialog.getTextValue(answer,"elastic shell tension:","%g",te) ){}// 
  else if( dialog.getTextValue(answer,"elastic shell stiffness:","%g",ke) ){}// 
  else if( dialog.getTextValue(answer,"elastic shell damping:","%g",be) ){}// 
  else if( dialog.getTextValue(answer,"elastic shell dissipation:","%g",ad2) ){}// 

  else if( answer.matches("BC left: ") ||
           answer.matches("BC right: ") ||
           answer.matches("BC bottom: ") ||
           answer.matches("BC top: ") )
  {
    int side=0, axis=0;
    if( len=answer.matches("BC left: ") )
    {
      side=0; axis=0;
    }
    else if( len=answer.matches("BC right: ") )
    {
      side=1; axis=0;
    }
    else if( len=answer.matches("BC bottom: ") )
    {
      side=0; axis=1;
    }
    else if( len=answer.matches("BC top: ") )
    {
      side=1; axis=1;
    }
    else
    {
      OV_ABORT("ERROR: unexpected answer!");
    }
    

    const aString bcNames[] = { "periodic",
				"Dirichlet",
				"Neumann",
                                "slide",
				"" };  

    line = answer(len,answer.length()-1);
    for( int bc=0; bc<numberOfBoundaryConditions; bc++ )
    {
      if( line==bcNames[bc] )
      {
        printF("Setting boundaryCondition(%i,%i)=%i (%s).\n",side,axis,bc,(const char*)line);
        boundaryCondition(side,axis)=bc;
      }
    }
    
  }
  

//   else if( dialog.getTextValue(answer,"BC left:"  ,"%i",boundaryCondition(0,0)) ){}// 
//   else if( dialog.getTextValue(answer,"BC right:" ,"%i",boundaryCondition(1,0)) ){}// 
//   else if( dialog.getTextValue(answer,"BC bottom:","%i",boundaryCondition(0,1)) ){}// 
//   else if( dialog.getTextValue(answer,"BC top:"   ,"%i",boundaryCondition(1,1)) ){}// 

  else if( dialog.getToggleValue(answer,"constant volume",constantVolumeElasticShell) ){}//
  else if( dialog.getTextValue(answer,"volume penalty parameter:","%g",volumePenalty) ){}// 
  else
  {
    found=false;
  }
  

  return found;
}

// ===================================================================================================================
/// \brief Build the options dialog for the elastic beam parameters.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int DeformingBodyMotion::
buildElasticBeamOptionsDialog(DialogData & dialog )
{

  //BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");

  dialog.setOptionMenuColumns(1);
  /*
  const aString bcNames[] = { "periodic",
			      "Dirichlet",
			      "Neumann",
                              "slide",
			      "" };  
  const int maxCommands=numberOfBoundaryConditions+1;
  aString bcCmd[maxCommands];

  GUIState::addPrefix(bcNames,"BC left: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC left:",bcCmd,bcNames,boundaryCondition(0,0) );

  GUIState::addPrefix(bcNames,"BC right: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC right:",bcCmd,bcNames,boundaryCondition(1,0) );

  GUIState::addPrefix(bcNames,"BC bottom: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC bottom:",bcCmd,bcNames,boundaryCondition(0,1) );

  GUIState::addPrefix(bcNames,"BC top: ",bcCmd,maxCommands);
  dialog.addOptionMenu("BC top:",bcCmd,bcNames,boundaryCondition(1,1) );

  */
  if( !deformingBodyDataBase.has_key("elasticBeamParameters") )
  {
    deformingBodyDataBase.put<real [10]>("elasticBeamParameters");
    real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
    // -- set defaults ---
    par[0]=0.02*0.02*0.02/12.0; // area moment of inertia
    par[1]=1.4e6;  // elastic modulus
    par[2]=1e4;  // density 
    par[3]=0.35;  // length 
    par[4]=0.02;  // thickness
    par[5]=1000.0;  // pressure_norm
    par[6]=0.0;
    par[7] = 0.3;
    par[8] = 0.0;

    deformingBodyDataBase.put<int [10]>("elasticBeamIntegerParameters");

    int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
    ipar[0] = 15;
    ipar[1] = 0;
    ipar[2] = 0;
    ipar[3] = 0;
    /*
    deformingBodyDataBase.put<bool>("constantVolumeElasticShell",false);
    deformingBodyDataBase.put<real>("integratedVolumeDiscrepancy",0.);  // holds int_t (1-V/V0) dt 
    deformingBodyDataBase.put<real>("volumePenalty");
    deformingBodyDataBase.get<real>("volumePenalty")=1.; // actual penalty is .5/dt */
  }
  
  real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
  real & I = par[0];
  real & Em   = par[1];
  real & rho   = par[2];
  real & L   = par[3];
  real & thick =   par[4];
  real & pnorm =   par[5];
  real & x0 =   par[6];
  real & y0 =   par[7];
  real & dec =   par[8];

  int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
  int& nelem = ipar[0];
  int& bcl_ = ipar[1];
  int& bcr_ = ipar[2];
  int& exact = ipar[3];


  /*
  aString tbCommands[] = {"constant volume",
 			  ""};
  int tbState[10];
  tbState[0] = constantVolumeElasticShell;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 
  */
  // ----- Text strings ------
  const int numberOfTextStrings=20;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "elastic beam area moment of inertia:";  sPrintF(textStrings[nt],"%g",I); nt++;
  textLabels[nt] = "elastic beam modulus:";  sPrintF(textStrings[nt],"%g",Em); nt++;
  textLabels[nt] = "elastic beam density:";  sPrintF(textStrings[nt],"%g",rho); nt++;
  textLabels[nt] = "elastic beam length:";  sPrintF(textStrings[nt],"%g",L); nt++;
  textLabels[nt] = "elastic beam thickness:";  sPrintF(textStrings[nt],"%g",thick); nt++;
  textLabels[nt] = "elastic beam pressure norm:";  sPrintF(textStrings[nt],"%g",pnorm); nt++;
  textLabels[nt] = "elastic beam x0:";  sPrintF(textStrings[nt],"%g",x0); nt++;
  textLabels[nt] = "elastic beam y0:";  sPrintF(textStrings[nt],"%g",y0); nt++;
  textLabels[nt] = "elastic beam declination:";  sPrintF(textStrings[nt],"%g",dec); nt++;

  textLabels[nt] = "elastic beam number of elements:";  sPrintF(textStrings[nt],"%d",nelem); nt++;
  textLabels[nt] = "elastic beam boundary condition (left):";  sPrintF(textStrings[nt],"%d",bcl_); nt++;
  textLabels[nt] = "elastic beam boundary condition (right):";  sPrintF(textStrings[nt],"%d",bcr_); nt++;
  textLabels[nt] = "elastic beam use exact solution:";  sPrintF(textStrings[nt],"%d",exact); nt++;

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);

  return 0;
}


//================================================================================
/// \brief: Look for an elastic beam option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int DeformingBodyMotion::
getElasticBeamOption(const aString & answer,
		      DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
  real & I = par[0];
  real & Em   = par[1];
  real & rho   = par[2];
  real & L   = par[3];
  real & thick =   par[4];
  real & pnorm =   par[5];
  real & x0 =   par[6];
  real & y0 =   par[7];
  real & dec =   par[8];

  int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
  int& nelem = ipar[0];
  int& bcl_ = ipar[1];
  int& bcr_ = ipar[2];
  int& exact = ipar[3];

  if(      dialog.getTextValue(answer,"elastic beam area moment of inertia:","%g",I) ){} // 
  else if( dialog.getTextValue(answer,"elastic beam modulus:","%g",Em) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam density:","%g",rho) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam length:","%g",L) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam thickness:","%g",thick) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam pressure norm:","%g",pnorm) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam x0:","%g",x0) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam y0:","%g",y0) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam declination:","%g",dec) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam number of elements:","%d",nelem) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam boundary condition (left):","%d",bcl_) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam boundary condition (right):","%d",bcr_) ){}// 
  else if( dialog.getTextValue(answer,"elastic beam boundary use exact solution:","%d",exact) ){}// 

  else
  {
    found=false;
  }
  

  return found;
}



// =================================================================================================
/// \brief  Define deforming body properties interactively.
// =================================================================================================
int DeformingBodyMotion::
update(CompositeGrid & cg, GenericGraphicsInterface & gi )
{
  DeformingBodyType & deformingBodyType = 
                  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType");

  if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
    deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);
  UserDefinedDeformingBodyMotionEnum & userDefinedDeformingBodyMotionOption = 
                 deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");
  

  // --- here are the parameters used by the surface smoother ---
  if( !deformingBodyDataBase.has_key("smoothSurface") )
  {
    deformingBodyDataBase.put<bool>("smoothSurface",false);
    deformingBodyDataBase.put<int>("numberOfSurfaceSmooths",3);
  }
  bool & smoothSurface = deformingBodyDataBase.get<bool>("smoothSurface");
  int & numberOfSurfaceSmooths = deformingBodyDataBase.get<int>("numberOfSurfaceSmooths");
  bool & changeHypeParameters = deformingBodyDataBase.get<bool>("changeHypeParameters");
  
  // -- Boundary conditions ---
  if( !deformingBodyDataBase.has_key("boundaryCondition") )
  {
    deformingBodyDataBase.put<BcArray>("boundaryCondition");
    BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");
    for( int side=0; side<=1; side++ )for( int axis=0; axis<2; axis++ )
    {
      boundaryCondition(side,axis)=dirichletBoundaryCondition;
    }
  }
  BcArray & boundaryCondition = deformingBodyDataBase.get<BcArray>("boundaryCondition");
  
  GUIState gui;
  gui.setWindowTitle("Deforming Body");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

  aString prefix = ""; // prefix for commands to make them unique.

  bool buildDialog=true;
  if( buildDialog )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString pbLabels[] = {"elastic shell options...",
			  "grid evolution parameters...",
                          "elastic beam parameters...",
			  "help",
			  ""};
    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=4;
    dialog.setPushButtons( cmd, pbLabels, numRows ); 

    dialog.setOptionMenuColumns(1);
    aString typeCommands[] = { "elastic filament",
			       "elastic shell",
                               "unknown",
			       "" };
    // --- fix me: 
    int option = (deformingBodyType==elasticFilament ? 0 : 
		  deformingBodyType==userDefinedDeformingBody && userDefinedDeformingBodyMotionOption==elasticShell ? 1 : 2);
    dialog.addOptionMenu("Type:",typeCommands,typeCommands,option );


    aString tbCommands[] = {"smooth surface",
                            "change hype parameters",
			    ""};
    int tbState[10];
    tbState[0] = smoothSurface;
    tbState[1] = changeHypeParameters;
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "debug:"; sPrintF(textStrings[nt], "%i",debug);  nt++; 
    textLabels[nt] = "number of surface smooths:"; sPrintF(textStrings[nt], "%i",numberOfSurfaceSmooths);  nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    // addPrefix(textLabels,prefix,cmd,maxCommands);
    // dialog.setTextBoxes(cmd, textLabels, textStrings);
    dialog.setTextBoxes(textLabels, textLabels, textStrings);


  }
  
  // ********************* Elastic Shell options ********************************
  DialogData & elasticShellOptionsDialog = gui.getDialogSibling();

  elasticShellOptionsDialog.setWindowTitle("Elastic Shell Options");
  elasticShellOptionsDialog.setExitCommand("close elastic shell options", "close");
  if( buildDialog )
  {
    buildElasticShellOptionsDialog(elasticShellOptionsDialog);
  }

  DialogData & elasticBeamOptionsDialog = gui.getDialogSibling();

  elasticBeamOptionsDialog.setWindowTitle("Elastic Beam Options");
  elasticBeamOptionsDialog.setExitCommand("close elastic beam options", "close");
  if( buildDialog )
  {
    buildElasticBeamOptionsDialog(elasticBeamOptionsDialog);
  }

  aString answer,buff;


  const aString deformMenu[]=
    {
      "elastic filament",
      "user defined deforming body",
      ">types",
        "ice deform",
        "ellipse deform",
        "sphere deform",
        "advect body",
        "elastic shell",
        "elastic beam",
        "interface deform",
        "user defined deforming surface",
        "linear deform",  // for testing the elastic piston problem
      "<>parameters",
        "debug",
        "deformation frequency",
        "deformation amplitude",
        "elastic shell parameters",
        "boundary parameterization",
        "sub iteration convergence tolerance",
        "added mass relaxation factor",
        "velocity order of accuracy",
        "acceleration order of accuracy",
        "initial velocity",
        "initial acceleration",
        "provide past history",
        "grid evolution parameters...",
        "beam free motion",
      "<done",
      ""
    }; 

  gui.buildPopup(deformMenu);

  gi.pushGUI(gui);
  gi.appendToTheDefaultPrompt("deform>");

  for( ;; ) 
  {
	    
    // int response=gi.getMenuItem(deformMenu,answer,"Choose the deformation type");
    
    gi.getAnswer(answer,"");
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);


    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( answer=="elastic filament" )
    {
      deformingBodyType=elasticFilament;
    }
    else if( answer=="user defined deforming body" )
    {
      deformingBodyType=userDefinedDeformingBody; 
    }
    else if( answer=="ice deform" )
    {
      userDefinedDeformingBodyMotionOption=iceDeform;
    }
    else if( answer=="ellipse deform" )
    {
      userDefinedDeformingBodyMotionOption=ellipseDeform;
    }
    else if( answer=="linear deform" )
    {
      userDefinedDeformingBodyMotionOption=linearDeform;

      if( !deformingBodyDataBase.has_key("linearDeformParameter") )
	deformingBodyDataBase.put<real[2]>("linearDeformParameter");
      real *rpar = deformingBodyDataBase.get<real[2]>("linearDeformParameter");


      rpar[0]=-1./4.; rpar[1]=4.;

      printF("linear deform: x(t) = a*t^p\n");
      gi.inputString(answer,"Enter a,p for x(t) = a*t^p");
      sScanF(answer,"%e %e",&rpar[0],&rpar[1]);
      printF("The linear deform parameters are a=%e, p=%e\n",rpar[0],rpar[1]);

    }
    else if( answer=="sphere deform" )
    {
      userDefinedDeformingBodyMotionOption=sphereDeform;
    }
    else if( answer=="advect body" )
    {
      userDefinedDeformingBodyMotionOption=advectBody;
    }
    else if( answer=="interface deform" )
    {
      userDefinedDeformingBodyMotionOption=interfaceDeform;
      printF(" INFO interface deform is used for multi-domain deforming interfaces\n");
    }
    else if( answer=="elastic shell" )
    {
      deformingBodyType=userDefinedDeformingBody; 
      userDefinedDeformingBodyMotionOption=elasticShell;
    }
    else if( answer=="elastic beam" )
    {
      deformingBodyType=userDefinedDeformingBody; 
      userDefinedDeformingBodyMotionOption=elasticBeam;

      if( pBeamModel==NULL );
        pBeamModel = new BeamModel;

    }
    else if( answer=="nonlinear beam" )
    {
      deformingBodyType=userDefinedDeformingBody; 
      userDefinedDeformingBodyMotionOption=nonlinearBeam;

    }
    else if( answer=="user defined deforming surface" )
    {
      userDefinedDeformingBodyMotionOption=userDeformingSurface;
      // -- choose user defined deforming surface and set parameters ---
      parameters.userDefinedDeformingSurfaceSetup( *this );
    }
    else if( answer=="deformation frequency" )
    {
      if( !deformingBodyDataBase.has_key("deformationFrequency") )
	deformingBodyDataBase.put<real>("deformationFrequency",1.);
      real & deformationFrequency = deformingBodyDataBase.get<real>("deformationFrequency");

      gi.inputString(answer,sPrintF("Enter the deformation frequency (for user defined motion) (current=%6.2f)",
				     deformationFrequency));
      sScanF(answer,"%e",&deformationFrequency);
      printF("Setting the deformation frequency to %9.3e for the user defined motion\n",deformationFrequency);
    }
    else if( answer=="deformation amplitude" )
    {
      if( !deformingBodyDataBase.has_key("deformationAmplitude") )
	deformingBodyDataBase.put<real>("deformationAmplitude",1.);
      real & deformationAmplitude = deformingBodyDataBase.get<real>("deformationAmplitude");

      gi.inputString(answer,sPrintF("Enter the deformation amplitude (for user defined motion) (current=%6.2f)",
				     deformationAmplitude));
      sScanF(answer,"%e",&deformationAmplitude);
      printF("Setting the deformation amplitude to %9.3e for the user defined motion\n",deformationAmplitude);
    }
    else if( answer=="elastic shell parameters" )
    {
      if( !deformingBodyDataBase.has_key("elasticShellParameters") )
      {
	deformingBodyDataBase.put<real [10]>("elasticShellParameters");
	deformingBodyDataBase.put<bool>("constantVolumeElasticShell",false);
	deformingBodyDataBase.put<real>("integratedVolumeDiscrepancy",0.);  // holds int_t (1-V/V0) dt 
	deformingBodyDataBase.put<real>("volumePenalty");
	deformingBodyDataBase.get<real>("volumePenalty")=10.;

	real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");
	par[0]=10.; // rhoe
	par[1]=1.;  // te 
	par[2]=5.;  // ke 
	par[3]=.1;  // be 
	par[4]=1.;  // ad2 : coefficient of artificial diffusion

      }

      real *par = deformingBodyDataBase.get<real [10]>("elasticShellParameters");

      gi.inputString(answer,sPrintF("Enter rhoe,te,ke,be,ad2 (default=(%g,%g,%g,%g,%g)",par[0],par[1],par[2],par[3],par[4]));
      sScanF(answer,"%e %e %e %e %e",&par[0],&par[1],&par[2],&par[3],&par[4]);
      printF("Setting rhoe=%g, te=%e, ke=%e, be=%e ad2=%e for the elastic shell\n",par[0],par[1],par[2],par[3],par[4]);
    }
    else if( answer=="nonlinear beam model file" )
    {
      if( !deformingBodyDataBase.has_key("nonlinearBeamModelFile") )
      {
	deformingBodyDataBase.put<std::string>("nonlinearBeamModelFile");
	std::string& filename = deformingBodyDataBase.get<std::string>("nonlinearBeamModelFile");
	filename = "default.beam";
      }
      std::string& filename = deformingBodyDataBase.get<std::string>("nonlinearBeamModelFile");

      gi.inputString(answer,sPrintF("Enter %c filename (default=%s):",'a',filename.c_str()));
      char fn[256];
      sScanF(answer,"%s",fn);
      filename = fn;

      printF("Setting filename=%s for nonlinear beam\n",fn);

      

    }
    else if( answer=="elastic beam parameters..." )
    {
      // new way 
      assert( pBeamModel!=NULL );
      pBeamModel->update(cg,gi);
      
    }
    
    else if( answer=="elastic beam parameters" )
    {
      // **** OLD WAY ****


      if( !deformingBodyDataBase.has_key("elasticBeamParameters") )
      {
	deformingBodyDataBase.put<real [10]>("elasticBeamParameters");

	real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
	par[0]=0.02*0.02*0.02/12.0; // area moment of inertia
	par[1]=1.4e6;  // elastic modulus
	par[2]=1e4;  // density 
	par[3]=0.35;  // length 
	par[4]=0.02;  // thickness
	par[5]=1000.0;  // pressure norm
	par[6]=0.0;  // x0
	par[7]=0.3;  // y0
	par[8]=0.0;  // declination

	deformingBodyDataBase.put<int [10]>("elasticBeamIntegerParameters");
	
	int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
	ipar[0] = 15;
	ipar[1] = 0;
	ipar[2] = 0;
	ipar[3] = 0;
      }

      real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");

      gi.inputString(answer,sPrintF("Enter I,E,rho,L,t,pnorm,x0,y0,dec, scaleFactor (default=(%g,%g,%g,%g,%g,%g,%g,%g,%g,%g)",
				    par[0],par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],BeamModel::exactSolutionScaleFactorFSI));
      sScanF(answer,"%e %e %e %e %e %e %e %e %e %e",&par[0],&par[1],&par[2],&par[3],&par[4],&par[5],&par[6],&par[7],&par[8],&BeamModel::exactSolutionScaleFactorFSI);
      printF("Setting I=%g, E=%e, rho=%e, L=%e t=%e pnorm=%e x0=%e y0=%e dec=%e scaleFactor=%e for the elastic beam\n",
	     par[0],par[1],par[2],par[3],par[4],par[5],par[6],par[7],par[8],BeamModel::exactSolutionScaleFactorFSI);

      int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");

      gi.inputString(answer,sPrintF("Enter nelem, bcl, bcr, exact (default=(%d,%d,%d,%d) (0=cantilevered, 1=pinned, 2=free)",ipar[0],ipar[1],ipar[2],ipar[3]));
      sScanF(answer,"%d %d %d %d",&ipar[0],&ipar[1],&ipar[2],&ipar[3]);
      printF("Setting nelem=%d, bcl=%d, bcr=%d, exact=%d for the elastic beam\n",ipar[0],ipar[1],ipar[2],ipar[3]);


      assert( pBeamModel!=NULL );
      // pBeamModel = new BeamModel;

      // real *par = deformingBodyDataBase.get<real [10]>("elasticBeamParameters");
      real & I = par[0];
      real & Em   = par[1];
      real & rho   = par[2];
      real & L   = par[3];
      real & thick =   par[4];
      real & pnorm = par[5];
      real & x0 = par[6];
      real & y0 = par[7];
      real & dec = par[8];

      // int *ipar = deformingBodyDataBase.get<int [10]>("elasticBeamIntegerParameters");
      int& nelem = ipar[0];
      int& bcl_ = ipar[1];
      int& bcr_ = ipar[2];
      int& exact = ipar[3];

      BeamModel::BoundaryCondition bcl,bcr;
      switch (bcl_) {
      case 0:
	bcl = BeamModel::Cantilevered;  break;
      case 1:
	bcl = BeamModel::Pinned;  break;
      case 2:
	bcl = BeamModel::Free;  break;
      default:
	std::cout << "Error: unknown beam boundary condition " << bcl_ << "; " <<
	  " setting to Cantilevered" << std::endl;
	bcl = BeamModel::Cantilevered; break;	  
      }

      switch (bcr_) {
      case 0:
	bcr = BeamModel::Cantilevered;  break;
      case 1:
	bcr = BeamModel::Pinned;  break;
      case 2:
	bcr = BeamModel::Free;  break;
      default:
	std::cout << "Error: unknown beam boundary condition " << bcr_ << "; " <<
	  " setting to Cantilevered" << std::endl;
	bcr = BeamModel::Cantilevered; break;	  
      }

      pBeamModel->setParameters(I, Em, rho, L, thick, pnorm, nelem, bcl, bcr,x0,y0,(exact==1));

      pBeamModel->setDeclination(dec*Pi/180.0);

      // real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
      // pBeamModel->setAddedMassRelaxation(omega);
      // real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");

      // pBeamModel->setSubIterationConvergenceTolerance(tol);


      // if (deformingBodyDataBase.has_key("beam free motion")) {
      // 	real * p = deformingBodyDataBase.get<real [3]>("beam free motion");

      // 	pBeamModel->setupFreeMotion(p[0],p[1],p[2]);
      // }



    }
    else if( answer=="boundary parameterization" )
    {
     int & boundaryParameterization = deformingBodyDataBase.get<int>("boundaryParameterization");
      
     printF("INFO: you should set the boundary parameterization to `index' for fluid structure problems\n"
            "      so that the grid lines match across the interface\n");
     gi.inputString(answer,sPrintF(buff,"Enter the boundary parameterization: %i=chord, %i=index (current=%i)",
		(int)NurbsMapping::parameterizeByChordLength,(int)NurbsMapping::parameterizeByIndex,boundaryParameterization));
      sScanF(answer,"%i",&boundaryParameterization);
      if( boundaryParameterization!=NurbsMapping::parameterizeByChordLength &&
          boundaryParameterization!=NurbsMapping::parameterizeByIndex )
      { 
        printF("DeformingBodyMotion::update:ERROR: invalid value for boundary parameterization = %i\n",boundaryParameterization);
	gi.stopReadingCommandFile();

	boundaryParameterization=NurbsMapping::parameterizeByChordLength;
      }
      printF("Setting the boundary parameterization to %i\n",boundaryParameterization);

    }
    else if( answer=="sub iteration convergence tolerance" )
    {
      if (!deformingBodyDataBase.has_key("sub iteration convergence tolerance")) {
	deformingBodyDataBase.put<real>("sub iteration convergence tolerance");
	real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");
	tol = 1e-3;
      }
      
      real & tol = deformingBodyDataBase.get<real>("sub iteration convergence tolerance");

      gi.inputString(answer,sPrintF("Enter tol (default=%g)",tol));
      sScanF(answer,"%e",&tol);
      printF("Setting convergence tolerance = %g\n",tol);
	
    }
    else if( answer=="added mass relaxation factor" )
    {
      if (!deformingBodyDataBase.has_key("added mass relaxation factor")) {
	deformingBodyDataBase.put<real>("added mass relaxation factor");
	real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");
	omega = 1.0;
      }
      
      real & omega = deformingBodyDataBase.get<real>("added mass relaxation factor");

      gi.inputString(answer,sPrintF("Enter omega (default=%g)",omega));
      sScanF(answer,"%e",&omega);
      printF("Setting added mass relaxation factor = %g\n",omega);
	
    }


    else if( answer=="beam free motion" )
    {
      if (!deformingBodyDataBase.has_key("beam free motion")) {
	//real beamFreeMotionParams[] = {0.0,0.0,0.0};
	deformingBodyDataBase.put<real[3]>("beam free motion");
	real* p = deformingBodyDataBase.get<real [3]>("beam free motion");
	memset(p,0,sizeof(real)*3);
      }

      real* beamFreeMotionParams = deformingBodyDataBase.get<real [3]>("beam free motion");
      gi.inputString(answer,sPrintF("Enter beam free motion parameters x0,y0,angle0 (default=%g,%g,%g)",
				    beamFreeMotionParams[0],beamFreeMotionParams[1],beamFreeMotionParams[2]));
      sScanF(answer,"%e %e %e",
	     &beamFreeMotionParams[0],
	     &beamFreeMotionParams[1],
	     &beamFreeMotionParams[2]);
      printF("Setting beam free motion parameters= x0=%g,y0=%g,angle0=%g\n",
	     beamFreeMotionParams[0],beamFreeMotionParams[1],beamFreeMotionParams[2]);
    }

    else if( answer=="velocity order of accuracy" )
    {
      int & order=deformingBodyDataBase.get<int>("velocityOrderOfAccuracy");
      gi.inputString(answer,sPrintF("Enter the velocity order of accuracy (default=%i)",order));
      sScanF(answer,"%i",&order);
      printF("DeformingBodyMotion::update: set the velocity order of accuracy =%i\n",order);
    }
    else if( answer=="acceleration order of accuracy" )
    {
      int & order=deformingBodyDataBase.get<int>("accelerationOrderOfAccuracy");
      gi.inputString(answer,sPrintF("Enter the acceleration order of accuracy (default=%i)",order));
      sScanF(answer,"%i",&order);
      printF("DeformingBodyMotion::update: set the acceleration order of accuracy =%i\n",order);
    }
    else if( answer=="initial velocity" )
    {
      if( !deformingBodyDataBase.has_key("initialVelocity") )
      {
	deformingBodyDataBase.put<real [3]>("initialVelocity");
        real *v0 = deformingBodyDataBase.get<real [3]>("initialVelocity");
        v0[0]=v0[1]=v0[2]=0.;
      }
      real *v0 = deformingBodyDataBase.get<real [3]>("initialVelocity");
      gi.inputString(answer,sPrintF("Enter the initial velocity: v0,v1,v2 (default=(%g,%g,%g)",v0[0],v0[1],v0[2]));
      sScanF(answer,"%e %e %e",&v0[0],&v0[1],&v0[2]);
      printF("Setting the inital velocity to (%9.3e,%9.3e,%9.3e)\n",v0[0],v0[1],v0[2]);
    }
    else if( answer=="initial acceleration" )
    {
      if( !deformingBodyDataBase.has_key("initialAcceleration") )
      {
	deformingBodyDataBase.put<real [3]>("initialAcceleration");
	real *a0 = deformingBodyDataBase.get<real [3]>("initialAcceleration");
	a0[0]=a0[1]=a0[2]=0.;
      }
      real *a0 = deformingBodyDataBase.get<real [3]>("initialAcceleration");
      gi.inputString(answer,sPrintF("Enter the initial acceleration: a0,a1,a2 (default=(%g,%g,%g)",a0[0],a0[1],a0[2]));
      sScanF(answer,"%e %e %e",&a0[0],&a0[1],&a0[2]);
      printF("Setting the inital acceleration to (%9.3e,%9.3e,%9.3e)\n",a0[0],a0[1],a0[2]);
    }
    else if( answer=="provide past history" )
    {
      printF("INFO: After the deforming bodies are defined you will be asked to provide grid history information.\n");
      deformingBodyDataBase.get<int>("providePastHistory")=true;
    }
    else if( answer=="debug" )
    {
      gi.inputString(answer,sPrintF("Enter debug (default=%i)",debug));
      sScanF(answer,"%i",&debug);
      printF("DeformingBodyMotion::update: set debug=%i\n",debug);
    }
    else if( answer=="grid evolution parameters..." )
    {
      // we assume that there is only one face of one grid on this deforming body

      vector<GridEvolution*> & gridEvolution = deformingBodyDataBase.get<vector<GridEvolution*> >("gridEvolution"); 

      const int face=0;
      if( face >=gridEvolution.size() )
        gridEvolution.push_back(new GridEvolution);

      assert( face>=0 && face<gridEvolution.size() );

      // set the order of accuracy for computing the velocity and acceleration
      gridEvolution[face]->setVelocityOrderOfAccuracy(deformingBodyDataBase.get<int>("velocityOrderOfAccuracy"));
      gridEvolution[face]->setAccelerationOrderOfAccuracy(deformingBodyDataBase.get<int>("accelerationOrderOfAccuracy"));
      
      gridEvolution[face]->update(gi);

    }

    // ***************** NEW WAY **********
    else if( dialog.getTextValue(answer,"debug:","%i",debug) ){} //

    else if( getElasticShellOption(answer,elasticShellOptionsDialog ) )
    {
      printF("Answer=[%s] found in getElasticShellOption.\n",(const char*)answer);


    }
    else if( getElasticBeamOption(answer,elasticBeamOptionsDialog ) )
    {
      printF("Answer=[%s] found in getElasticBeamOption.\n",(const char*)answer);


    }
    else if( dialog.getToggleValue(answer,"smooth surface",smoothSurface) )
    {
      if( smoothSurface )
	printF("Surface smoothing is on. The deforming surface will be smoothed with a 4th-order filter.\n"
               "  You may also set `number of surface smooths' to define the number of smoothing iterations\n");
      else
	printF("Surface smoothing is off\n");
    }
    else if( dialog.getToggleValue(answer,"change hype parameters",changeHypeParameters) )
    {
      if( changeHypeParameters )
	printF("changeHypeParameters=true : You may edit the hyperbolic grid parameters the first time the\n"
               "  hyperbolic grid generator is called.\n");
    }
    
    else if( dialog.getTextValue(answer,"number of surface smooths:","%i",numberOfSurfaceSmooths) ){}// 

    else if( answer=="elastic shell options..." )
    {
      elasticShellOptionsDialog.showSibling();
    }
    else if( answer=="close elastic shell options" )
    {
      elasticShellOptionsDialog.hideSibling(); 
    }

    else if( answer=="elastic beam options..." )
    {
      elasticBeamOptionsDialog.showSibling();
    }
    else if( answer=="close elastic beam options" )
    {
      elasticBeamOptionsDialog.hideSibling(); 
    }

    else
    {
      printF("DeformingBodyMotion::update:ERROR:unknown response=[%s]\n",(const char*)answer);
      gi.stopReadingCommandFile();
    }
    
  }
    
  gi.popGUI();
  gi.unAppendTheDefaultPrompt();

  return 0;

}

bool DeformingBodyMotion::hasCorrectionConverged() const {

  bool res = true;
  if (pBeamModel)
    res = res && pBeamModel->hasCorrectionConverged();

  if (pNonlinearBeamModel)
    res = res && pNonlinearBeamModel->hasCorrectionConverged();


  return res;
}

namespace {

template <class T>
void getValueDeformingBody(const GenericDataBase & subDir, const aString & name,
                           DataBase& db) {

  subDir.get(db.template get<T>(name), name);
}

template <class T>
void putValueDeformingBody(GenericDataBase & subDir, const aString & name,
                           const DataBase& db) {

  subDir.put(db.template get<T>(name), name);
}

template <class T>
void getLocalElem(const GenericDataBase & subDir, T*  elem, 
		  const aString & name) {

  elem->get(subDir, name);
}

template <>
void getLocalElem(const GenericDataBase & subDir, Mapping*  elem, 
		  const aString & name) {

  elem->get(subDir, name);
}

template <class T>
void putLocalElem(GenericDataBase & subDir, const T& elem, 
		  const aString & name) {

  elem.put(subDir, name);
}

template <>
void putLocalElem(GenericDataBase & subDir, const Mapping& elem, 
		  const aString & name) {

  subDir.put(elem.getClassName(),"MappingClassName");
  elem.put(subDir, name);
}


template <>
void getLocalElem<real>(const GenericDataBase & subDir, real*  elem, 
			const aString & name) {
  subDir.get(*elem,name);
}

template <>
void getLocalElem<RealArray>(const GenericDataBase & subDir, RealArray* elem, 
			     const aString & name) {
  subDir.get(*elem,name);
}
  
template <>
void putLocalElem<real>(GenericDataBase & subDir, const real& elem, 
			const aString & name) {
  subDir.put(elem,name);
}

template <>
void putLocalElem<RealArray>(GenericDataBase & subDir, const RealArray& elem, 
			     const aString & name) {
  subDir.put(elem,name);
}

template <class T>
T* local_vec_new(const GenericDataBase & subDir,int ls) {

  if (ls > 1)
    return new T[ls];
  else
    return new T;
}

template <>
Mapping* local_vec_new(const GenericDataBase & subDir, int ls) {

  aString className;
  subDir.get(className,"MappingClassName");
  return Mapping::makeMapping(className);

}
  

template <class T>
void getVectorState(const GenericDataBase & subDir,  const char* name,
		    DataBase& db,int ls) {

  int sz;
  subDir.get(sz, aString(name)+"vec_size");

  vector<T*>& vals = db.get<vector<T*> >(name);

  vals.resize(sz);

  for (int i = 0; i < sz; ++i) {

    T* p = local_vec_new<T>(subDir,ls);

    for (int j = 0; j < ls; ++j) {
      
      getLocalElem(subDir,p+j, sPrintF("%d%s_LocalElem_%i",1, name, j));
    }
    vals[i] = p;
  }

}

template <class T>
void putVectorState(GenericDataBase & subDir, const char* name,
		    const DataBase& db,int ls) {

  const vector<T*>& vals = db.get<vector<T*> >(name);

  int sz = vals.size();
  subDir.put(sz, aString(name)+"vec_size");


  for (int i = 0; i < sz; ++i) {

    T* const p = vals[i];
    for (int j = 0; j < ls; ++j) {

      unsigned int one = 1;
      aString R = sPrintF("%d%s_LocalElem_%i",one, name, j);
      // std::cout << "R = " << R << std::endl;
      putLocalElem(subDir,p[j], R);
    }
  }

}

}


int DeformingBodyMotion::get( const GenericDataBase & dir, const aString & name) {

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"DeformingBodyMotion");

  aString className;
  subDir.get( className,"className" ); 

  int temp;

  const int numberOfSurfaceArrays=3;

  getValueDeformingBody<int>(subDir,"numberOfDeformingGrids",deformingBodyDataBase);
  getValueDeformingBody<int>(subDir,"numberOfFaces",deformingBodyDataBase);
  getValueDeformingBody<IntegerArray>(subDir,"boundaryFaces",deformingBodyDataBase);

  getValueDeformingBody<int>(subDir,"numberOfTimeLevels",deformingBodyDataBase);

  subDir.get(temp, "deformingBodyType"); 
  deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType") = (DeformingBodyType)temp;
  
  getVectorState<Mapping>(subDir, "transform",deformingBodyDataBase, 1);
  getVectorState<Mapping>(subDir, "surface",deformingBodyDataBase, 1);
  getVectorState<GridEvolution>(subDir, "gridEvolution",deformingBodyDataBase, 1);
  

  getVectorState<RealArray>(subDir, "surfaceArray",deformingBodyDataBase, numberOfSurfaceArrays);
  getVectorState<real>(subDir, "surfaceArrayTime",deformingBodyDataBase, numberOfSurfaceArrays);
  
  
  getValueDeformingBody<int>(subDir,"boundaryParameterization",deformingBodyDataBase);
  getValueDeformingBody<int>(subDir,"accelerationOrderOfAccuracy",deformingBodyDataBase);
  getValueDeformingBody<int>(subDir,"velocityOrderOfAccuracy",deformingBodyDataBase);

  getValueDeformingBody<real>(subDir,"sub iteration convergence tolerance",deformingBodyDataBase);
  
  getValueDeformingBody<real>(subDir,"added mass relaxation factor",deformingBodyDataBase);


  switch (deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType")) {
  case userDefinedDeformingBody: {
    
    if( !deformingBodyDataBase.has_key("userDefinedDeformingBodyMotionOption") )
      deformingBodyDataBase.put<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption",iceDeform);

    subDir.get(temp, "userDefinedDeformingBodyMotionOption"); 
    deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption") = 
      (UserDefinedDeformingBodyMotionEnum)temp;
    
    switch ((UserDefinedDeformingBodyMotionEnum)temp) {

    case nonlinearBeam:
      
      //pNonlinearBeamModel = new NonlinearBeamModel;
      pNonlinearBeamModel->get(subDir, "NonlinearBeamModel");
      break;
    default:
      break;
    }


    break;
  }
    
  default:
    
    std::cout << "ERROR: Unsupported type of deforming body on restart: " << 
      (int)deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType")
	      << std::endl;
    exit(-1);
    break;
  }

  delete &subDir;

  return 0;
}

int DeformingBodyMotion::put( GenericDataBase & dir, const aString & name) const {

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.create(subDir,name,"DeformingBodyMotion");

  aString className = "DeformingBodyMotion";
  subDir.put( className,"className" ); 

  int temp;

  const int numberOfSurfaceArrays=3;

  putValueDeformingBody<int>(subDir,"numberOfDeformingGrids",deformingBodyDataBase);
  putValueDeformingBody<int>(subDir,"numberOfFaces",deformingBodyDataBase);
  putValueDeformingBody<IntegerArray>(subDir,"boundaryFaces",deformingBodyDataBase);

  putValueDeformingBody<int>(subDir,"numberOfTimeLevels",deformingBodyDataBase);

  temp = deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType") ;
  subDir.put(temp, "deformingBodyType"); 
  
  putVectorState<Mapping>(subDir, "transform",deformingBodyDataBase, 1);
  putVectorState<Mapping>(subDir, "surface",deformingBodyDataBase, 1);
  putVectorState<GridEvolution>(subDir, "gridEvolution",deformingBodyDataBase, 1);
  

  putVectorState<RealArray>(subDir, "surfaceArray",deformingBodyDataBase, numberOfSurfaceArrays);
  putVectorState<real>(subDir, "surfaceArrayTime",deformingBodyDataBase, numberOfSurfaceArrays);
  
  
  putValueDeformingBody<int>(subDir,"boundaryParameterization",deformingBodyDataBase);
  putValueDeformingBody<int>(subDir,"accelerationOrderOfAccuracy",deformingBodyDataBase);
  putValueDeformingBody<int>(subDir,"velocityOrderOfAccuracy",deformingBodyDataBase);

  putValueDeformingBody<real>(subDir,"sub iteration convergence tolerance",deformingBodyDataBase);
  
  putValueDeformingBody<real>(subDir,"added mass relaxation factor",deformingBodyDataBase);


  switch (deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType")) {
  case userDefinedDeformingBody: {
    
    temp = (int)deformingBodyDataBase.get<UserDefinedDeformingBodyMotionEnum>("userDefinedDeformingBodyMotionOption");
    subDir.put(temp, "userDefinedDeformingBodyMotionOption");
    
    switch ((UserDefinedDeformingBodyMotionEnum)temp) {

    case nonlinearBeam:
      
      pNonlinearBeamModel->put(subDir, "NonlinearBeamModel");
      break;
    default:
      break;
    }


    break;
  }
    
  default:
    
    //    std::cout << "ERROR: Unsupported type of deforming body on restart: " << 
    //  (int)deformingBodyDataBase.get<DeformingBodyType>("deformingBodyType")
    //	      << std::endl;
    //exit(-1);
    break;
  }

  delete &subDir;

  return 0;
}



// =================================================================================================
/// \brief Plot things related to deforming grids (e.g. the center lines of beams or shells)
// =================================================================================================
int DeformingBodyMotion::
plot(GenericGraphicsInterface & gi, GridFunction & cgf, GraphicsParameters & psp )
{

  printF("DeformingBodyMotion::plot...\n");


  if( pBeamModel!=NULL || pNonlinearBeamModel!=NULL )
  {
    printF("DeformingBodyMotion::plot the beam model.\n");


    RealArray xc;
    aString buff;
    if( pBeamModel!=NULL )
    {
      pBeamModel->getCenterLine(xc);
      ::display(xc,sPrintF(buff,"%s: center line",(const char*)pBeamModel->getName()),"%8.2e ");
    }
    else
    {
      pNonlinearBeamModel->getCenterLine(xc);
      // ::display(xc,sPrintF(buff,"%s: center line",(const char*)pBeamModel->getName()),"%8.2e ");
      ::display(xc,"center line","%8.2e ");
    }
    
    NurbsMapping map; 
    map.interpolate(xc);

    real lineWidth=2;
    // psp.get(GraphicsParameters::lineWidth,lineWidthSave);  // default is 1
    psp.set(GraphicsParameters::lineWidth,lineWidth);  
    PlotIt::plot(gi, map,psp);      
    psp.set(GraphicsParameters::lineWidth,1);  // reset


  }
  

  return 0;
}
