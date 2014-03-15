//  $Id: ElasticFilament.C,v 1.16 2006/08/21 02:55:43 henshaw Exp $
//
//  ElasticFilament -- motion of a 1-d (finite length) filament
//
//  ----> separate the Mapping from here
//

#include "ElasticFilament.h"
#include "GenericGraphicsInterface.h"
//#include "MappingEnums.h"
#include "Mapping.h"
#include "MappingInformation.h"

#include "SplineMapping.h"
#include "FilamentMapping.h"

//..Constructor & Destructor

ElasticFilament::
ElasticFilament(int nFilamentPoints00,   /* =21   */ 
		int nEndPoints00   /* =3    */
		)		      
{
  //..Set default values
  mass =            1.;    
  density =         1.;    // Find reasonable values for these.
  bendingMoment =   0.01;  
  el =              1.;
  //xOffset=          0.;
  //yOffset=          0.;

  numberOfSteps = -1;
  current       = -1;   // Will I use these?

  filamentDynamicsType = PRESCRIBED_DYNAMICS;

  maximumNumberToSave= 5;
  numberSaved=         0;
  nFilamentPoints=     nFilamentPoints00;

  pFilamentMapping = new FilamentMapping();
  nFilamentPoints           = pFilamentMapping->getNumberOfFilamentPoints();
  nTotalThickFilamentPoints = pFilamentMapping->getNumberOfThickFilamentPoints();

  isFilamentMappingMine=true;
  
  initialize();
}

ElasticFilament::
~ElasticFilament()
{
  if (isFilamentMappingMine && (pFilamentMapping!=NULL)) 
  {
    delete (pFilamentMapping);
  }
}

void ElasticFilament::
initialize()
{
  if (pFilamentMapping!= NULL) pFilamentMapping->initializeFilamentStorage();
  time.redim(maximumNumberToSave);
  x0.redim(nFilamentPoints,2);
  v0.redim(nFilamentPoints,2);

  xAll.redim(nFilamentPoints,2,maximumNumberToSave);  // for tstepper
  vAll.redim(nFilamentPoints,2,maximumNumberToSave);
  rhsAll.redim(nFilamentPoints,2,maximumNumberToSave);

  xFilament.redim(nFilamentPoints,2);  xr.redim(nFilamentPoints,2);  

  x_t.redim(nFilamentPoints,2);        x_tt.redim(nFilamentPoints,2);
  xr_t.redim(nFilamentPoints,2);       xr_tt.redim(nFilamentPoints,2);  
  
  //..Thick filament
  surfaceVelocity.redim(nTotalThickFilamentPoints, 2);
  surfaceAcceleration.redim(nTotalThickFilamentPoints, 2);

  //..Thick stresses (will be)--> integrate --> filamentFluidStresses
  //:: NOTE: these are not used currently. 000929 **pf
  //surfaceStress.redim(nFilamentPoints,2);    // ave. stress on 1D filam
  //stressThickFilament.redim(nTotalThickFilamentPoints,2); 
}

void ElasticFilament::
initializeFromFilamentMapping( FilamentMapping *pFilamCopy )
{
  if (pFilamentMapping !=NULL) delete(pFilamentMapping);

  pFilamentMapping = pFilamCopy;
  isFilamentMappingMine = false;

  //nFilamentPoints           = pFilamentMapping->nFilamentPoints;
  //nTotalThickFilamentPoints = pFilamentMapping->nThickSplinePoints;
  nFilamentPoints = pFilamentMapping->getNumberOfFilamentPoints();
  nTotalThickFilamentPoints = pFilamentMapping->getNumberOfThickFilamentPoints();

  initialize();

  #ifndef USE_PPP
    xFilament = pFilamentMapping->xFilament;
  #else
    OV_ABORT("finish me for parallel Bill!");
  #endif
}

void ElasticFilament::
initializeSurfaceData(double time00)
{
  tcomp = time00;
  evaluateSurfaceAtTime( time00 );
  computePrescribedSurfaceData();
  computeSurfaceData();
}

void ElasticFilament::
replaceHyperbolicMapping( HyperbolicMapping *pNewHyper )
{
  assert ( pFilamentMapping != NULL );
  pFilamentMapping->replaceHyperbolicMapping( pNewHyper );
}

HyperbolicMapping *
ElasticFilament::getHyperbolicMappingPointer()
{
  assert( pFilamentMapping != NULL );
  //.. pFilamMap->getHyperbMapPtr is BUGGY --> changes dims of the pHyper
  //return( pFilamentMapping->getHyperbolicMappingPointer() ); BUGGY!! 

  if (pFilamentMapping->pHyper != NULL)
  {
    cout << "ElastFilam.getHyperbPtr Filam->pHyper->gridDimension(axis1) = " // **pf DEBUG
	 << pFilamentMapping->pHyper->getGridDimensions(axis1) << endl;
    cout << "ElastFilam.getHyperbPtr Filam->pHyper->gridDimension(axis2) = " // **pf DEBUG
	 << pFilamentMapping->pHyper->getGridDimensions(axis2) << endl;
  }

  return( pFilamentMapping->pHyper );  //  **pf DEBUG, since above is no good
}

Mapping *
ElasticFilament::getSurface()
{
  assert( pFilamentMapping != NULL );
  return ( (Mapping *) pFilamentMapping->getSurfaceForHyperbolicMap());
}

void ElasticFilament::
regenerateBodyFittedMapping()
{
  assert (pFilamentMapping != NULL );
  cout << "ElasticFilament::regenerateBodyFittedGrid() called...\n";
  pFilamentMapping->regenerateBodyFittedMapping();
}

// *** THIS IS A HACK -- for getVelocity to start working!! **pf DEBUG
void ElasticFilament::
regenerateBodyFittedMapping( HyperbolicMapping *pHyper00)
{
  assert (pFilamentMapping != NULL );
  cout << "ElasticFilament::regenerateBodyFittedGrid() called...\n";
  pFilamentMapping->regenerateBodyFittedMapping( pHyper00 );
}

//..MOVE THE FILAMENT---> time integration routines
int ElasticFilament::
integrate( real t0, const RealCompositeGridFunction & surfaceStress00, real t)
  // INTEGRATE filament equations from time t0--> t using
  // surfaceStresses at t0 (=explicit scheme)
{
  cout << "ElasticFilament::integrate called with filamentDynamicsType="
       << filamentDynamicsType << endl;
  assert(pFilamentMapping !=NULL);
  tcomp=t;
  switch (filamentDynamicsType) 
    {
    case PRESCRIBED_DYNAMICS: 
      {
	cout <<"-->PRESCRIBED_DYNAMICS: calling FilamentMapping::integrate\n";
	pFilamentMapping->evaluateAtTime( tcomp );    // --> specific to prescribed
	computePrescribedSurfaceData();
	computeSurfaceData();
//         pFilamentMapping->getVelocity(x_t, xr_t);
// 	pFilamentMapping->getAcceleration(x_tt, xr_tt);
// 	pFilamentMapping->getCorePoints( xFilament );
// 	pFilamentMapping->getCoreDerivative( xr );    // <--- specific to prescribed

// 	pFilamentMapping->setupTimeDerivatives(x_t,x_tt,xr, xr_t,xr_tt);  // --> generic
// 	pFilamentMapping->getSurfaceVelocity(x_t, surfaceVelocity);
// 	pFilamentMapping->getSurfaceAcceleration(x_tt, surfaceAcceleration); // <-- generic
// 	surfaceVelocityTime     = tcomp;
// 	surfaceAccelerationTime = tcomp;
	break; 
      }
    default:
      {
	cout <<"ERROR: ElasticFilament::integrate called with"
	     <<" unknown filamentDynamicsType = "<<filamentDynamicsType<<endl;
	break;
      }
    }
  //..Compute surface velocity/acceleration      //??
  return 0;
}

//..For integrating prescribed filament motion
//....NB: these data would be available inside ElasticFilam 
//        for more general eqs of motion
//..AUX to integrate
void ElasticFilament::
computePrescribedSurfaceData()
{
  assert( pFilamentMapping != NULL );
#ifndef USE_PPP
  pFilamentMapping->getVelocity(x_t, xr_t);
  pFilamentMapping->getAcceleration(x_tt, xr_tt);
  pFilamentMapping->getCorePoints( xFilament );
  pFilamentMapping->getCoreDerivative( xr ); 
#else
  OV_ABORT("finish me for parallel Bill!");
#endif
}

//..After integration, gets surface velocity & acceleration at time tcomp
//..AUX to integrate
void ElasticFilament::
computeSurfaceData()
{
  assert( pFilamentMapping != NULL );
#ifndef USE_PPP
  pFilamentMapping->setupTimeDerivatives(x_t,x_tt,xr, xr_t,xr_tt);  
  pFilamentMapping->getSurfaceVelocity(x_t, surfaceVelocity);
  pFilamentMapping->getSurfaceAcceleration(x_tt, surfaceAcceleration); 
  surfaceVelocityTime     = tcomp;
  surfaceAccelerationTime = tcomp;
#else
  OV_ABORT("finish me for parallel Bill!");
#endif
}

int ElasticFilament::
getVelocityBC( const real time0, 
	       const Index &I1, const Index &I2, const Index &I3, 
	       realSerialArray & bcVelocity)
{
  //ASSUMES that the boundary acceleration has been computed at the current time!!
  cout << "++ElasticFilament::getVelocityBC  called for t="<<time0
       << ",  returning velocity at time="<<surfaceVelocityTime;
  if ( fabs(time0 - surfaceVelocityTime)>1.e-12 ) cout << "(times not equal...)\n";
  else cout << "\n";
  const int spaceDimension =2; // only 2D for now
  Index Ic(0,spaceDimension); 
  int lenIn[4], lenOut[4];
  int i;
  for (i=0; i<4; ++i) lenIn[i] =  surfaceVelocity.getLength(i);
  for (i=0; i<4; ++i) lenOut[i] = bcVelocity.getLength(i);
  printf("+++getVelocityBC:  surfVelocity( %i, %i, %i, %i ),    bcVelocity( %i, %i, %i, %i ) \n", 
	 lenIn[0], lenIn[1],lenIn[2],lenIn[3], lenOut[0], lenOut[1],lenOut[2],lenOut[3]);

  assert(pFilamentMapping!= NULL);
  //pFilamentMapping->xThickFilament.display("ElasticFilam--getAccelBC -- xThickFilament\n");//DEBUG
  //..surfAccel has dim(n+1,2,1,1), bcAccel has dim(n,1,1,2) for FilamentMaps
  //surfaceAcceleration.reshape( I1,I2,I3, Ic);
  //bcAcceleration = surfaceAcceleration;  // FIX **pf, is this conformable? --no
  bcVelocity(I1,I2,I3,0) = surfaceVelocity(I1,0); //should NOT be dependent on the dimension
  bcVelocity(I1,I2,I3,1) = surfaceVelocity(I1,1);
  //bcVelocity(I1,I2,I3,2) = surfaceVelocity(I1,2); //... could loop i=0, numberOfDimensions

  return 0;
}

int ElasticFilament::
getAccelerationBC( const real time0, 
		   const Index &I1, const Index &I2, const Index &I3, 
		   realSerialArray & bcAcceleration)
{
  //ASSUMES that the boundary acceleration has been computed at the current time!!
  cout << "++ElasticFilament::getAccelerationBC  called for t="<<time0
       << ",  returning acceleration at time="<<surfaceAccelerationTime;
  if ( fabs(time0 - surfaceAccelerationTime)>1.e-12 ) cout << "(times not equal...)\n";
  else cout << "\n";
  const int spaceDimension =2; // only 2D for now
  Index Ic(0,spaceDimension); 
  int lenIn[4], lenOut[4];
  int i;
  for (i=0; i<4; ++i) lenIn[i] =  surfaceAcceleration.getLength(i);
  for (i=0; i<4; ++i) lenOut[i] = bcAcceleration.getLength(i);
  printf("+++getAccelBC:  surfAccel( %i, %i, %i, %i ),    bcAccel( %i, %i, %i, %i ) \n", 
	 lenIn[0], lenIn[1],lenIn[2],lenIn[3], lenOut[0], lenOut[1],lenOut[2],lenOut[3]);

  assert(pFilamentMapping!= NULL);
  //pFilamentMapping->xThickFilament.display("ElasticFilam--getAccelBC -- xThickFilament\n");//DEBUG
  //..surfAccel has dim(n+1,2,1,1), bcAccel has dim(n,1,1,2) for FilamentMaps
  //....NOTE: if I change the dimensions of the bodyf. mapping, but not nFilamentPoints,
  //....      bcAccel & surfaceAccel will have different dimensions.
  //....    --> ONLY change dimensions of the centerline & the # of end points!! May 15
  //....    ---> this became an issue with the acceleration BCs
  //
  bcAcceleration(I1,I2,I3,0) = surfaceAcceleration(I1,0); //could loop i=0, numberOfDimensions
  bcAcceleration(I1,I2,I3,1) = surfaceAcceleration(I1,1);

  //printf("@@@@@@@ Index I1:\n");  ///DEBUG CODE
  //pDisplay->display(I1);
  //printf("@@@@@@@ SURFACE acceleration:\n");
  //pDisplay->display(surfaceAcceleration);
  //printf("@@@@@@@ BC-ACCELERATION:\n");
  //pDisplay->display(bcAcceleration);
  

  return 0;
}



//..EVALUATE the current surface at time=time0 -- extrapolate if necessary
int ElasticFilament::
evaluateSurfaceAtTime( real time00 )
{
  assert( pFilamentMapping != NULL );

  cout << "ElasticFilament::evaluateSurfaceAtTime t="
       << time00 << "  called \n";

  switch (filamentDynamicsType) 
    {
    case PRESCRIBED_DYNAMICS: 
      pFilamentMapping->evaluateAtTime( time00 );
    break;
    default:
      {
	cout <<"ERROR: ElasticFilament::evaluateSurfaceAtTime called with unknown dynamicsType"
	     << "= "<< filamentDynamicsType<<endl;
      }
    break;
    }
  
  //pFilamentMapping->initializeBodyFittedMapping();
  return 0;
}


void ElasticFilament::
copyBodyFittedMapping( HyperbolicMapping &copyMap, 
		       aString *pNewMappingName /* = NULL */)
{
  assert( pFilamentMapping != NULL );
  pFilamentMapping->copyBodyFittedMapping( copyMap, pNewMappingName );
  //copyMap.reinitialize();
}

void ElasticFilament::
referenceMap( int gridToMove, CompositeGrid &cg)   // DEBUG -- updates the grid??
{
  assert( pFilamentMapping != NULL );

  //cg[gridToMove].reference( *pFilamentMapping );
  //cg[gridToMove].update();
  cg[gridToMove].geometryHasChanged( ~MappedGrid::THEmask );
  cg[gridToMove].update(CompositeGrid::THEmask |  
			CompositeGrid::THEinterpolationCoordinates | 
			CompositeGrid::THEinterpoleeGrid |           
			CompositeGrid::THEinterpoleeLocation | 
			CompositeGrid::THEinterpolationPoint |   
			CompositeGrid::THEinverseMap);

}

//
// getHyperbolicMapping -- return HyperbolicMapping in Filament
//  --> used to copy the parameters.
//
HyperbolicMapping ElasticFilament::
getHyperbolicMapping()
{
  assert( pFilamentMapping != NULL );
  return( *( pFilamentMapping-> getHyperbolicMappingPointer() ));
}


// ---------------------------setGridDimensions == User interface
//\begin{>>ElasticFilamentSolverInclude.tex}{\subsection{update}} 
int ElasticFilament::
update( GenericGraphicsInterface & gi )
// ===============================================================
// /Description:
//    Define moving grid properties interavtively.
//\end{ElasticFilamentSolverInclude.tex}  
// ===============================================================
{

  cout << "ElasticFilament::update -- do not change grid dimensions\n";
  assert( pFilamentMapping != NULL );

  //aString answer,answer2;
  // int numMenus=5;  // ?? number of menus
  
  //  aString *gridMenu = new aString [numMenus];
  //for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  //  gridMenu[grid]=cg[grid].getName();
  //gridMenu[cg.numberOfComponentGrids()+0]="all";
  //gridMenu[cg.numberOfComponentGrids()+1]="none";
  //gridMenu[cg.numberOfComponentGrids()+2]="done";
  //gridMenu[cg.numberOfComponentGrids()+3]="";

  //
  // ..WORK IN PROGRESS....
  //

  MappingInformation mapInfo;
  mapInfo.graphXInterface = &gi;

  pFilamentMapping->update( mapInfo );

  // .. OLD SKETCH OF A USER INTERFACE **pf
  //   for( ;; ) 
  //   {
  //     const aString filamentMenu[]=
  //     {
  //       "oscillate",
  //       "rotate",
  //       "translate",
  //       "rigid body",
  //       "deforming body",
  //       "user defined",
  //       "done",
  //       ""
  //     }; 
  //     gi.appendToTheDefaultPrompt("ElasticFilament>");
  //     int response=gi.getMenuItem(filamentMenu,answer2,"Choose option");
  //     if( answer2=="done" || answer2=="exit" )
  //     {
  //       break;
  //     }
  //     else if ( answer2=="number of filament points" )
  //     {
  // 
  
  return 0;
}


//
//------------------TODO-------------------------------------
//
//..ACCESS routines
void ElasticFilament::
setProperties(enum FilamentPropertyFlag flag, real value )
{
  cout << "ElasticFilament::setProperties --- "
       << "NOT IMPLEMENTED, Using defaults." << endl;
}

void ElasticFilament::
setProperties(enum FilamentPropertyFlag flag, int value )
{
  cout << "ElasticFilament::setProperties --- "
       << "NOT IMPLEMENTED, Using defaults." << endl;
}

void ElasticFilament::
getProperties(enum FilamentPropertyFlag flag, int & value )
{
  cout << "ElasticFilament::getProperties --- "
       << "NOT IMPLEMENTED." << endl;
}

void ElasticFilament::
getProperties(enum FilamentPropertyFlag flag, real & value )
{
  cout << "ElasticFilament::getProperties --- "
       << "NOT IMPLEMENTED." << endl;
}


//
// ---- OBSOLETE
//


