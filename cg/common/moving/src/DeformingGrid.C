//
// DeformingGrid:
// ==================================
//   * Single component grid = mappedGrid
//   * Keep time history of (t,x,y,z) so we can 
//       compute d/dt x=u (gridVelocity) 
//

#include "Overture.h"
#include "HyperbolicMapping.h"
#include "DataPointMapping.h"
//#include "DeformingGridGenerationInformation.h"
#include "DeformingGrid.h"
#include "MappingInformation.h"

//..Constructor / Destructor

DeformingGrid::
DeformingGrid( int numberOfTimeLevels,
		GenericGraphicsInterface *pGIDebug00 /* =NULL */,
		int debug00                          /* =0    */)
{
  maximumNumberOfTimeLevels = numberOfTimeLevels;
  iMostRecentTimeLevel      = -1;
  currentNumberOfTimeLevels = 0;

  mappingHistoryList = new HyperbolicMapping[maximumNumberOfTimeLevels];
  timeHistory =        new real[maximumNumberOfTimeLevels];

  gridHistoryList = new realArray[maximumNumberOfTimeLevels];

  for (int i=0; i<maximumNumberOfTimeLevels; i++ )  {
    timeHistory[i] = 0.;
  }

  //..debug info
  debug=debug00;
  //debug=7; //for debugging feb 26 **pf
  pGIDebug = pGIDebug00;
  if (pGIDebug != NULL )
  {
    pMapInfoDebug = new MappingInformation;
    pMapInfoDebug->graphXInterface = pGIDebug;
  } else {
    pMapInfoDebug = NULL;
  }

  mappingSerialNumber = 0;
}

DeformingGrid::
~DeformingGrid()
{
   delete [] mappingHistoryList;
   delete [] timeHistory;
}

//..............SERVICES

//int DeformingGrid::
//storeNewTimeLevel( const real newTime, HyperbolicMapping & newMap )
//{}

//
// .. CALLED in the beginning of a computation to initialize
//    the mapping history list with reasonable values so the
//    grid velocity gets computed OK from the very first tstep
//

//void DeformingGrid::
//getNewMapping( const real time0, 
//	       Mapping & surface,
//	       HyperbolicMapping* &pNewMapping )
//{
// getNewMapping( time0, pNewMapping );
//
//  cout << "DeformingGrid::getNewMapping "
//       << " got mapName="<< pNewMapping->getClassName()
//       << endl;
//
//  // ?? should this be here ??  
//  // --> perhaps caller should initialize it?
//  pNewMapping->setSurface( surface ); 
//
//}

void DeformingGrid::
getNewMapping( const real time0, 
	       HyperbolicMapping*  &pNewMapping )
{
  if (currentNumberOfTimeLevels != maximumNumberOfTimeLevels) {
    //..keep adding
    currentNumberOfTimeLevels++;
    iMostRecentTimeLevel++;
  } else {
    //..reuse old mappings: find oldest & return that.
    iMostRecentTimeLevel = ( iMostRecentTimeLevel+1 ) % maximumNumberOfTimeLevels;
  }

  if (debug&4) cout << "GETNEWMAPPING: iTime="<< iMostRecentTimeLevel
		    << ",  t="<< time0<< endl;
    
  assert( (0<=iMostRecentTimeLevel) 
	  && (iMostRecentTimeLevel<currentNumberOfTimeLevels) );

  pNewMapping = &( mappingHistoryList[ iMostRecentTimeLevel ]);
  timeHistory[iMostRecentTimeLevel] = time0;

}

void DeformingGrid::
getCurrentMap( real & time0, HyperbolicMapping*  &pCurrentMapping )
{
  assert( (0<=iMostRecentTimeLevel) 
	  && (iMostRecentTimeLevel<currentNumberOfTimeLevels) );
  pCurrentMapping = & (mappingHistoryList[ iMostRecentTimeLevel ]);
  time0 = timeHistory[ iMostRecentTimeLevel ];

}

//..Get any timelevel
//....Offset= 0   gives current
//....Offset=-1   gives previous
//....Offset=-2   gives current-2
//....Offset>0    is ILLEGAL
void DeformingGrid::
getTimeLevelMapPointer( const int offset, 
			real & time0, 
			HyperbolicMapping*  &pMap)
{
  if (offset>0) {
    cout << "++ERROR: DeformingGrid::getTimeLevel.... offset>0 not good!!\n";
    pMap=NULL;
    return;
  }

  if ( -offset>= currentNumberOfTimeLevels ) {
    cout << "++ERROR: DeformingGrid::getTimeLevel.... \n";
    cout << "++++++++++  offset< -numberOfLevels available --> not good!!\n";
    pMap=NULL;
    return;
  }

  //.. find iCurrent - offset MOD numberOfLevels:
  int iModOffset = currentNumberOfTimeLevels + offset;
  int iMap       = ( iMostRecentTimeLevel + iModOffset ) % currentNumberOfTimeLevels;
  assert( (0<=iMap) && (iMap<currentNumberOfTimeLevels));

  time0 = timeHistory[ iMap ];
  pMap  = &( mappingHistoryList[ iMap ]);

  if(debug&4)
    cout << "TIMELEVELPOINTER: Offset = " << offset 
       << ", iModOffset="<<iModOffset
       << ", iPointer="<< iMap << ", t="<<time0<<endl;

}

#ifdef DEFORMINGGRID_STORE_GRID_ARRAYS
// CURRENTLY, leave these out

//
// STORE GridArrays (=xyz for all grid points) into a time history list
// --> trying to fix the instability arising with deformingGrids
// * Mappings only contain the grid points upto boundaries
// * But maybe I need the ghostvalues, too --> store RealArrays w/ ghostbdries
// --> BUT the instab. maybe in the gridvel. computation
//      & the fact that the gridvel. gets *extrapolated*!!

void DeformingGrid::
getCurrentGridArray( real & time0, RealArray*  &pCurrentGrid )
{
  assert( (0<=iMostRecentTimeLevel) 
	  && (iMostRecentTimeLevel<currentNumberOfTimeLevels) );
  pCurrentGrid = & (gridHistoryList[ iMostRecentTimeLevel ]);
  time0 = timeHistory[ iMostRecentTimeLevel ];

}

//..Get any timelevel
//....Offset= 0   gives current
//....Offset=-1   gives previous
//....Offset=-2   gives current-2
//....Offset>0    is ILLEGAL
void DeformingGrid::
getTimeLevelGridArrayPointer( const int offset, 
			real & time0, 
			RealArray*  &pGrid)
{
  if (offset>0) {
    cout << "++ERROR: DeformingGrid::getTimeLevelGridPtr.... offset>0 not good!!\n";
    pGrid=NULL;
    return;
  }

  if ( -offset>= currentNumberOfTimeLevels ) {
    cout << "++ERROR: DeformingGrid::getTimeLevelGridPtr.... \n";
    cout << "++++++++++  offset< -numberOfLevels available --> not good!!\n";
    pGrid=NULL;
    return;
  }

  //.. find iCurrent - offset MOD numberOfLevels:
  int iModOffset = currentNumberOfTimeLevels + offset;
  int iMap       = ( iMostRecentTimeLevel + iModOffset ) % currentNumberOfTimeLevels;
  assert( (0<=iMap) && (iMap<currentNumberOfTimeLevels));

  time0 = timeHistory[ iMap ];
  pGrid  = &( gridHistoryList[ iMap ]);

  //cout << "TIMELEVELPOINTER: Offset = " << offset 
  //     << ", iModOffset="<<iModOffset
  //     << ", iPointer="<< iMap << ", t="<<time0<<endl;

}

//
// STORE current grid points from cg[grid]
// Pass in a reference to the current gridArray
//
void DeformingGrid::
storeCurrentGridArray( const int grid00,
		       CompositeGrid & cg,
		       RealArray & gridArray)
{
  Index I1,I2,I3;
  Range all;

  MappedGrid & mg = cg[grid00];
  getIndex( mg.dimension(), I1,I2,I3 );
  int numDims=mg.numberOfDimensions();
  //  x=mg.vertex()(I1,I2,I3,axis1)
  //  y=mg.vertex()(I1,I2,I3,axis2)
  Index Idim(numDims);
  gridArray.redim(I1,I2,I3,Idim);
  gridArray(I1,I2,I3,Idim) = mg.vertex()(I1,I2,I3,Idim);
}

#endif

//.........................................................................
//
//..Moving Grid Services
//
//

void DeformingGrid::
getVelocity( const real vTime, 
	     const int grid00, 
	     CompositeGrid & cg,
	     realArray & gridVelocity)
{
  //
  // interpolate from timeHistory, calls  'computeVelocity'
  //
  if (debug&4)
    cout << endl
	 << "DeformingGrid::getVelocity, t="<< vTime
	 << ", grid="<<grid00 
	 << ", time_levels="<< currentNumberOfTimeLevels
	 << endl << endl;

  if ( currentNumberOfTimeLevels<2 ) {
    cout << "++DeformingGrid::getVelocity -- not enough time levels = ";
    cout << currentNumberOfTimeLevels << endl;
    gridVelocity=0.;
    return;
  }

  //-----------------------FROM MovingGrids.C
  // CompositeGrid c; realArray gridVelocity=OB_gf.gridVelocity[grid]
  //
  //getIndex( c.dimension(),I1,I2,I3 );
  //if( c.numberOfDimensions()==2 )
  //  {
  //    gridVelocity(I1,I2,I3,axis1)=
  //	(vertex(I1,I2,I3,axis1)-x0(axis1))*(-tsint*cost+tcost*sint)
  //	+(vertex(I1,I2,I3,axis2)-x0(axis2))*(-tsint*sint-tcost*cost);
  //      gridVelocity(I1,I2,I3,axis2)= 
  //	(vertex(I1,I2,I3,axis1)-x0(axis1))*(tcost*cost+tsint*sint) 
  //	+(vertex(I1,I2,I3,axis2)-x0(axis2))*(tcost*sint-tsint*cost);

  //..Get mappings at t0,t1,t2, where t2=most recent time level
  HyperbolicMapping *pMap0, *pMap1, *pMap2;
  real              t0, t1,t2;

  getTimeLevelMapPointer( 0  /*offset*/, t2, pMap2); if(debug&4)cout<<"++ pMap2 is at t="<<t2<<endl;
  getTimeLevelMapPointer( -1 /*offset*/, t1, pMap1); if(debug&4)cout<<"++ pMap1 is at t="<<t1<<endl;
  assert( pMap2 != NULL );
  assert( pMap1 != NULL );
  //if ((debug &8) && ( pMapInfoDebug != NULL ) ) {
  if ((debug &4) && ( pMapInfoDebug != NULL ) ) {
    cout << "DeformingGrid::getVelocity -- "
	 << "look at hyperb maps for grid vel.\n";
    //cout << " +++ pMap2 +++\n";
    //pMap2->update( *pMapInfoDebug );

    cout << " +++ pMap1 +++\n";
    pMap1->update( *pMapInfoDebug );
  }
  ////DEBUG **pf
  //cout << endl 
  //     << "REGENERATING BODY FITTED MAP -- DeformingGrid::getVelocity \n\n";
  //pMap1->generateNew();
  //pMap2->generateNew();

  if (debug&4) cout << "DeformingGrid::getVelocity -- "
		    << "calling pMap->getGrid()\n";
  const realArray & xy2 = pMap2->getGrid();
  const realArray & xy1 = pMap1->getGrid();
  
  //assert (xy2.isConformable(gridVelocity));

  Index I1,I2,I3;
  getIndex(cg[grid00].indexRange(),I1,I2,I3);   
  //getIndex(cg[grid00].dimension(),I1,I2,I3);    // causes A++ to core dump

  bool onlyUseTwoLevels = true;
  //bool onlyUseTwoLevels=false;

  if ( (currentNumberOfTimeLevels==2) || onlyUseTwoLevels) {
    if(debug&4)cout << "TWO (2) LEVELS in getVelocity computation!!\n";
    computeGridVelocity( vTime, t1, xy1, t2, xy2, 
			 gridVelocity, I1,I2,I3); 

  } 
  else if (currentNumberOfTimeLevels>=3 ) {
    getTimeLevelMapPointer( -2 /*offset*/, t0, pMap0);  
    assert( pMap0 != NULL );
    const realArray & xy0 = pMap0->getGrid();

    //..BUGS, t0=current , t1=previous , t2=next
    if(debug&4)cout << "THREE LEVELS in getVelocity computation!!\n";
    computeGridVelocity( vTime, t0, xy0, t1, xy1, t2, xy2,
			 gridVelocity, I1,I2,I3); 
  } else {
    assert ( currentNumberOfTimeLevels>=2 ); //shouldn't get here.
  }
}


void DeformingGrid::
computeGridVelocity( real vTime,
		     real t0, realArray const & xy0 /* const */,
		     real t1, realArray const & xy1 /* const */,
		     real t2, realArray const & xy2 /* const */,
		     realArray & gridVelocity,
		     Index & I1, Index & I2, Index & I3)
{
  if (debug&4) {
    cout << "\nComputeGridVelocity (3 levels): ";
    cout << "t0="<<t0<<", t1="<<t1<<", t2="<<t2<< ", vTime="<<vTime<<endl;
  }
  //.....................CHECKS
  if (  (t2 < t1) || (t1 < t0) ) {
    cout << "++DeformingGrid::computeMappingVelocity(t0,t1,t2) -- \n";
    cout << "++++++++times must be consequtive:  t0 < t1 < t2 <\n";
    return;
  }

  if (  (vTime<t0) ||  (vTime>t2) ){
    cout << "++WARNING; DeformingGrid::computeGridVelocity,"; 
    cout << " trying to extrapolate, not recommended\n";
  }

  if (debug&4) {
    if( !( xy0.isConformable(xy1) && xy1.isConformable(xy2))) {
      cout << "++DeformingGrid::computeMappingVelocity(t0,t1,t2)-- \n";
      cout << "++++++++grids must be conforming  <\n";
      //return;
    }
    
    if( !gridVelocity.isConformable(xy2) ) {
      cout << "++DeformingGrid::computeMappingVelocity(t0,t1,t2)-- \n";
      cout << "+++++++ gridVelocity not conformable with xy2\n";
      cout << ".........but we'll keep going!\n";
      //return;
    } 
  }

  //....................COMPUTE THE VELOCITY
  // Assume data is (t0,f0) (t1,f1)
  //
  // A quadratic interpolant is f(t)= f0 + a*(t-t0) + b*(t-t0)^2
  // where 
  //
  //       a=( (t2-t0)^2 *(f1-f0) - (t1-t0)^2 * (f2-f0) )/J
  //       b=( -(t2-t0)  *(f1-f0) + (t1-t0) * (f2-f0)   )/J
  //       J=(t1-t0)*(t2-t0)*(t1-t2)
  //
  //     f'(t) = a + 2*b*(t-t0)
  //
  real invJac=1./( (t1-t0)*(t2-t0)*(t1-t2) );
  real d2=t2-t0;
  real d1=t1-t0;

  //SHOULD factorize to eval. (xy1-xy0), (xy2-xy0) just once
  //  gridVelocity = invJac*( d2*d2*(xy1-xy0) - d1*d1*(xy2-xy0)
  //		    +2.*( -d2*(xy1-xy0) + d1*(xy2-xy0))*(vTime-t0) );

  Range all;

  real aa= invJac*(  d2*d2 - 2.*d2 );
  real bb= invJac*( -d1*d1 + 2.*d1 );
  gridVelocity(I1,I2,I3,all) = aa*(xy1(I1,I2,I3,all) - xy0(I1,I2,I3,all)) 
    + bb*( xy2(I1,I2,I3,all) - xy0(I1,I2,I3,all) );

  if(debug&8)gridVelocity.display("GRID VELOCITY");

}

void DeformingGrid::
computeGridVelocity( real vTime,
		     real t0, realArray const & xy0 /* const */,
		     real t1, realArray const & xy1 /* const */,
		     realArray & gridVelocity,
		     Index & I1, Index & I2, Index & I3)
{
  if (debug&4) {
    cout << "\nComputeGridVelocity (2 levels): ";
    cout << "t0="<<t0<<", t1="<<t1<< ", vTime="<<vTime<<endl;
  }

  //.....................CHECKS
  if (  t1 < t0  ) {
    cout << "++DeformingGrid::computeMappingVelocity(t0,t1)-- \n";
    cout << "++++++++times must be consequtive:  t0 < t1  <\n";
    return;
  }

  if (debug&4) {
    if (  (vTime<t0) ||  (vTime>t1) ){
      cout << "++WARNING; DeformingGrid::computeGridVelocity,"; 
      cout << " trying to extrapolate, not recommended\n";
    }
    
    if( !xy0.isConformable(xy1) ) {
      cout << "++DeformingGrid::computeMappingVelocity(t0,t1)-- \n";
      cout << "++++++++grids must be conforming  <\n";
      //return;
    }
    
    //NB: gridVel has ghostlines, xy1 does NOT  **pf
    // --> seems to work, but I wonder if there's trouble at bdry? **pf
    if( !gridVelocity.isConformable(xy1) ) {
      cout << "++DeformingGrid::computeMappingVelocity(t0,t1,t2)-- \n";
      cout << "+++++++ gridVelocity not conformable with xy1\n";
      cout << ".........but we'll keep going!\n";   
      //return;
    } 
  } // end if debug&4    

  //....................COMPUTE THE VELOCITY
  // Assume data is (t0,f0) (t1,f1)
  //
  // A linear interpolant is f(t)= f0 + (t-t0)*(f1-f0)/(t1-t0)
  // HENCE
  //       df/dt(t) = (f1-f0)/(t1-t0) is a constant
  //
  // -----> what happens in the ghostpoints?? Or masked out grid points??
  //

  Range all;
  real dd=1./(t1-t0);
  //gridVelocity= dd*( xy1 - xy0 );
  gridVelocity(I1,I2,I3,all)= dd*( xy1(I1,I2,I3,all) - xy0(I1,I2,I3,all) );

  if (debug&8) {
    cout << "-----------------DUMPING GRID VEL DATA -- TWO LEVELS --\n";
    xy1.display("GRID VEL ===== XY1 ");
    xy0.display("GRID VEL ===== XY0 ");
    gridVelocity.display("GRID VELOCITY -- two level computation");
  }

  // EVIL DEBUG CODE == Fudges velocity to (1,1)/sqrt(2) **pf DEBUG
  //  cout << endl
  //     << "DeformingGrid::computeGridVelocity -- WARNING!! "
  //     << "Setting gridvel = (1,1)/sqrt(2)!!"
  //     << endl << endl;
  //gridVelocity(I1,I2,I3,all) = 1./sqrt(2);
  //cout << endl
  //     << "DeformingGrid::computeGridVelocity 2 levels -- WARNING!! "
  //     << "Setting gridvel = (2, -1 )!!"
  //     << endl << endl;
  //gridVelocity(I1,I2,I3,0) = 2;
  //gridVelocity(I1,I2,I3,1) = -1;

}

void DeformingGrid::
getPastLevelGrid(   const int grid00,   CompositeGrid & cg,
		    int iLevel, realArray & gridVelocity)
{
  HyperbolicMapping *pMapster;
  Index I1,I2,I3; Range all;
  getIndex(cg[grid00].indexRange(),I1,I2,I3);   

  real tz=0;
  getTimeLevelMapPointer( iLevel  /*offset*/, tz, pMapster);

  if ( pMapster != NULL )   /// ??? WHAT IS THIS??? **pf
  {
    cout << "DG::getPastLevelGrid tz="<<tz<<endl;
    const realArray & xy = pMapster->getGrid();
    
    gridVelocity(I1,I2,I3,all) = xy(I1,I2,I3,all);
  } else {
    cout << "DG::getPastLevelGrid -- PROBLEMO, didn't get the pMapster\n";
    gridVelocity(I1,I2,I3,all) = -7;
  }
}


void DeformingGrid::
simpleGetVelocity( const real vTime, 
		   const int grid00, 
		   CompositeGrid & cg,
		   realArray & gridVelocity)
{
  HyperbolicMapping *pMapster;
  Index I1,I2,I3; Range all;
  getIndex(cg[grid00].indexRange(),I1,I2,I3);   

  if(debug&1) cout << "DeformingGrid::SimpleGetVelocity called.\n";

  //
  // interpolate from timeHistory, calls  'computeVelocity'
  //
  if (debug&4) {
    cout << endl
	 << "DeformingGrid::simpleGetVelocity, t="<< vTime
	 << ", grid="<<grid00 
	 << ", time_levels="<< currentNumberOfTimeLevels
	 << endl << endl;
  }    

  if ( currentNumberOfTimeLevels<2 ) {
    cout << "++DeformingGrid::simpleGetVelocity -- not enough time levels = ";
    cout << currentNumberOfTimeLevels << endl;
    gridVelocity=0.;
    return;
  }

  HyperbolicMapping *pMap0, *pMap1, *pMap2;
  real              t0, t1,t2;

  getTimeLevelMapPointer( 0  /*offset*/, t2, pMap2);
  getTimeLevelMapPointer( -1 /*offset*/, t1, pMap1);

  assert( pMap2 != NULL );
  assert( pMap1 != NULL );

  const realArray & xy2 = pMap2->getGrid();
  const realArray & xy1 = pMap1->getGrid();

  real dd=1./(t2-t1);
  //gridVelocity= dd*( xy1 - xy0 );
  //gridVelocity(I1,I2,I3,all)= dd*( xy2(I1,I2,I3,all) - xy1(I1,I2,I3,all) );
  gridVelocity(I1,I2,I3,all)=  dd*(xy2(I1,I2,I3,all) - xy1(I1,I2,I3,all));

}

void DeformingGrid::
simpleGetVelocityAndPoints( const real vTime, 
			    const int grid00, 
			    CompositeGrid & cg,
			    realArray & gridVelocity, 
			    realArray & xpoints1, realArray & xpoints2)
{
  if (debug&1)cout <<"DeformingGrid::simpleGetVelocityAndPoints called.\n:";
  HyperbolicMapping *pMapster;
  Index I1,I2,I3; Range all;
  getIndex(cg[grid00].indexRange(),I1,I2,I3);   

  //
  // interpolate from timeHistory, calls  'computeVelocity'
  //
  if(debug&4) {
    cout << endl
	 << "DeformingGrid::simpleGetVelocity, t="<< vTime
	 << ", grid="<<grid00 
	 << ", time_levels="<< currentNumberOfTimeLevels
	 << endl << endl;
  }

  if ( currentNumberOfTimeLevels<2 ) {
    cout << "++DeformingGrid::simpleGetVelocity -- not enough time levels = ";
    cout << currentNumberOfTimeLevels << endl;
    gridVelocity=0.;
    return;
  }

  HyperbolicMapping *pMap0, *pMap1, *pMap2;
  real              t0, t1,t2;

  getTimeLevelMapPointer( 0  /*offset*/, t2, pMap2);
  getTimeLevelMapPointer( -1 /*offset*/, t1, pMap1);

  assert( pMap2 != NULL );
  assert( pMap1 != NULL );

  const realArray & xy2 = pMap2->getGrid();
  const realArray & xy1 = pMap1->getGrid();

  

  real dd=1./(t2-t1);
  //gridVelocity= dd*( xy1 - xy0 );
  //gridVelocity(I1,I2,I3,all)= dd*( xy2(I1,I2,I3,all) - xy1(I1,I2,I3,all) );
  gridVelocity(I1,I2,I3,all)=  xy2(I1,I2,I3,all) - xy1(I1,I2,I3,all);

  xpoints1(I1,I2,I3,all)=xy1(I1,I2,I3,all);
  xpoints2(I1,I2,I3,all)=xy2(I1,I2,I3,all);

}

//..ACCELERATIONS  -- Not available yet
// BACKUP CODE: uses fd to get the acceleration of the grid points
// ========> should rather use the gridAccelBC from 
//           the DeformingBodySolver!!


//
// ..Regenerates component #grid 
//
void DeformingGrid::
regenerateGrid( HyperbolicMapping *pHyper, 
		const int grid00, 
		CompositeGrid & cg)
{
  cout << "DEFORMINGGRID::regenerateGrid called (should not happen!!)\n";
  //..Regenerate component #grid with the Hyperb. grid gen.
  //..Replace mapping in cg[grid] with 'hyper' after it's regen

  //>> Get 'deformingGridGenerationInformation' from physicsObject
  // ==> passed in through dggi

  //..Use HyperbMapping parameters in *pHyper for the generator

  assert( pHyper != NULL );
  pHyper->generate();

  //cg[grid].reference( *pHyper ); BUG!! 001002 **pf
  //cg[grid].update();             --> done by caller 001002 **pf

}


//
// ..Create mapping name -- bodyFitted t=xx
//
void DeformingGrid::
createMappingName( real time00, aString & newMappingName)
{
  char buff[180];
  sPrintF(buff, "bodyFitted t= %4.2f, no:%3i",time00, mappingSerialNumber);
  newMappingName= buff;
  mappingSerialNumber++;
}






