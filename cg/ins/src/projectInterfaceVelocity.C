#include "Cgins.h"
#include "Parameters.h"
#include "turbulenceModels.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"
#include "BeamModel.h"
#include "BeamFluidInterfaceData.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

// =======================================================================================
/// \brief Project the velocity on the interface for FSI problems. 
///
/// NOTE:
//     To project the fluid interface velocity we set the gridVelocity on the boundary
//   as this will later be used to set the values on the boundary.
// =======================================================================================
int Cgins::
projectInterfaceVelocity(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const real & dt0 /* =-1. */  )
{
  real dt;
  if( dt<= 0. )
    dt = parameters.dbase.get<real>("dt");  // *wdh* 2017/05/31
  // const real & dt = parameters.dbase.get<real>("dt");
  assert( dt>0. );
  
  if( t <= 2.*dt )
    printF("--INS-- projectInterfaceVelocity: ADDED MASS ALGORITHM - project velocity at t=%8.2e\n",t);

  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
  const bool & projectAddedMassVelocity = parameters.dbase.get<bool>("projectAddedMassVelocity");
  const bool & projectNormalComponentOfAddedMassVelocity =
               parameters.dbase.get<bool>("projectNormalComponentOfAddedMassVelocity");
  const bool & projectVelocityOnBeamEnds = parameters.dbase.get<bool>("projectVelocityOnBeamEnds"); 

  assert(  useAddedMassAlgorithm && projectAddedMassVelocity && parameters.gridIsMoving(grid) );
  
  MappedGrid & mg = *u.getMappedGrid();
  const int numberOfDimensions = mg.numberOfDimensions();

  const bool gridIsMoving = parameters.gridIsMoving(grid);

  const int uc = parameters.dbase.get<int >("uc");
  const int vc = parameters.dbase.get<int >("vc");
  const int wc = parameters.dbase.get<int >("wc");
  const int tc = parameters.dbase.get<int >("tc");
  const int & nc = parameters.dbase.get<int >("nc");
  const int orderOfAccuracy=min(4,parameters.dbase.get<int >("orderOfAccuracy"));
  Range V = Range(uc,uc+numberOfDimensions-1);

  BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid); // this will create the BDA if it is not there
  std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
  BoundaryData & bd = boundaryDataArray[grid];
      
  // -- extract parameters from any deforming solids ---

  MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
      
  if( bd.dbase.has_key("deformingBodyNumber") )
  {
    const real & fluidDensity = parameters.dbase.get<real >("fluidDensity");
    assert( fluidDensity>0. );

    const real fluidAddedMassLengthScale =  parameters.dbase.get<real>("fluidAddedMassLengthScale");

    int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");
    Index Ib1,Ib2,Ib3;
    for( int side=0; side<=1; side++ )
    {
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	if( deformingBodyNumber[side][axis]>=0 )
	{
	  int body=deformingBodyNumber[side][axis];
	  if( t<=0. )
	    printF("--INS-- grid=%i, (side,axis)=(%i,%i) belongs to deforming body %i\n",grid,side,axis,body);

	  DeformingBodyMotion & deform = movingGrids.getDeformingBody(body);


	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  Range Rx=numberOfDimensions;
	  realArray vSolid(Ib1,Ib2,Ib3,Rx); // holds velocity of solid on the boundary
          #ifndef USE_PPP
	    deform.getVelocityBC( t, grid, mg, Ib1,Ib2,Ib3, vSolid );
          #else
            OV_ABORT("finish me");
          #endif

	  OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);
	  OV_GET_SERIAL_ARRAY(real,u,uLocal);
	  OV_GET_SERIAL_ARRAY(real,vSolid,vSolidLocal);

          if( projectNormalComponentOfAddedMassVelocity )
            mg.update(MappedGrid::THEvertexBoundaryNormal);

          OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
	  
          // #ifdef USE_PPP
          //   const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
          // #else
          //   const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
          // #endif

          if( !deform.isBulkSolidModel() && !deform.isBeamModel() )
	  {

	    OV_ABORT("projectInterfaceVelocity::ERROR: un-expected deformation type");

	  }
	  else if( deform.isBulkSolidModel() )
	  {
            // *******************************************************************
            // *************** PROJECT VELOCITY BULK SOLID ***********************
            // *******************************************************************

	    real zp;
            deform.getBulkSolidParameters( zp );
	    const real & fluidDensity = parameters.dbase.get<real >("fluidDensity");
	    
            // fluid impedance = rho*H/dt 
	    assert( dt>0. );

            const real zf=fluidDensity/dt; // ****************** fix me ***************

            const real alpha = zf/(zf+zp);
	    if( t<=3.*dt )
	      printF("--PIV-- PROJECT INTERFACE VELOCITY FOR BULK SOLID MODEL, alpha=%9.2e **FINISH ME**\n",alpha);

            // ** do this for now****
            // We should really scale tangential components by zs
	    // --- set the gridVelocity to the desired BC for the fluid velocity
            gridVelocityLocal(Ib1,Ib2,Ib3,Rx)= alpha*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);

	    continue;
	  }
	  else if( deform.isBeamModel() )
	  {
#ifndef USE_PPP

            // **********************************************************
	    // ************ PROJECT VELOCITY BEAM MODEL ******************                
            // **********************************************************
 
	    BeamModel & beamModel = deform.getBeamModel();

	    real beamMassPerUnitLength=-1.;
	    beamModel.getMassPerUnitLength( beamMassPerUnitLength );

	    real alpha = 1./( 1. + beamMassPerUnitLength/(fluidDensity*fluidAddedMassLengthScale) );

	    // alpha=0.; // ***************
	    
	    if( t<=0. )
	      printF("--PIV-- alpha=%8.2e, beamMassPerUnitLength = %8.2e, fluidDensity=%8.2e hf=%8.2e\n",
		     alpha,beamMassPerUnitLength,fluidDensity,fluidAddedMassLengthScale);

	  
	    // --- Extract the "weight" array for weighting the velocity projection ---
	    //  This is used when we do not project the velocity on the ends of the beam
	    RealArray *pWeight= &Overture::nullRealArray(); // set pWeight to a default value if it is not used.
            if( !projectVelocityOnBeamEnds )
    	    {
	      DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	      DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
	      const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
	      const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
	    
	      BeamFluidInterfaceData &  beamFluidInterfaceData = 
		deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
	      int face=-1;
	      for( int face0=0; face0<numberOfFaces; face0++ )
	      {
	        const int side0=boundaryFaces(0,face0);
	        const int axis0=boundaryFaces(1,face0);
	        const int grid0=boundaryFaces(2,face0);
	        if( grid==grid0 && side==side0 && axis==axis0 )
	        { 
		  face=face0;
		  break;
	        }
	      }

	      assert( face>=0 && face<numberOfFaces);
	      RealArray *& weightArray = beamFluidInterfaceData.dbase.get<RealArray*>("weightArray");
	      pWeight = &(weightArray[face]);
	    }
	  
	    RealArray & weight = *pWeight;
	    if( (false && t<=max(0.,dt))  || debug() & 4 )
	    {
	      ::display(u(Ib1,Ib2,Ib3,V),sPrintF("---PIV-- fluid velocity at t=%9.3e",t),"%7.2e ");
	      ::display(vSolid(Ib1,Ib2,Ib3,Rx),sPrintF("---PIV-- beam  velocity at t=%9.3e",t),"%7.2e ");
	      ::display(gridVelocity(Ib1,Ib2,Ib3,Rx),sPrintF("---PIV-- grid velocity at t=%9.3e",t),"%7.2e ");
	      if( !projectVelocityOnBeamEnds )
		::display(weight(Ib1,Ib2,Ib3),sPrintF("---PIV-- weight at t=%9.3e",t),"%5.2f ");
	    }

	    if( true ) // project v and adjust the grid velocity
	    {
	      if( projectNormalComponentOfAddedMassVelocity )
	      {
		// --- only project the normal component of the fluid velocity ---
		// Project the normal component by subtracting the current normal component and then adding the new
		//  vp = AMP projected velocity
		//   v = v - (n.v)n + (n.vp)n
		//     = v - (n.(vp-v))n 
		//
		if( t <= 10.*dt )
		  printF("--PIV--: project NORMAL component of velocity only, t=%9.3e\n",t);
	    
		if( true )
		{

		  RealArray vp(Ib1,Ib2,Ib3,Rx), nDotV(Ib1,Ib2,Ib3);
		  // vp=( alpha*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx) -gridVelocityLocal(Ib1,Ib2,Ib3,Rx) );
		  // vp= (alpha-1.)*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);
		  if( projectVelocityOnBeamEnds )
		  {
		    vp= alpha*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);
		  }
		  else
		  {
		    // use solid velocity when the weight is zero: 
		    for( int dir=0; dir<numberOfDimensions; dir++ )
		      vp(Ib1,Ib2,Ib3,dir) = ( (   alpha*weight(Ib1,Ib2,Ib3))*uLocal(Ib1,Ib2,Ib3,uc+dir) + 
					      (1.-alpha*weight(Ib1,Ib2,Ib3))*vSolidLocal(Ib1,Ib2,Ib3,dir) );
		  }
		
		  nDotV = (normal(Ib1,Ib2,Ib3,0)*vp(Ib1,Ib2,Ib3,0)+
			   normal(Ib1,Ib2,Ib3,1)*vp(Ib1,Ib2,Ib3,1) );
		  if( numberOfDimensions==3 )
		    nDotV += normal(Ib1,Ib2,Ib3,2)*vp(Ib1,Ib2,Ib3,2);

		  if( TRUE ) // *WDH* try this 2015/03/06
		  {
		    // t.v = t.vs 
		    gridVelocityLocal(Ib1,Ib2,Ib3,Rx)=vSolidLocal(Ib1,Ib2,Ib3,Rx);  // set all components equal to vs 

		    nDotV -= (normal(Ib1,Ib2,Ib3,0)*vSolidLocal(Ib1,Ib2,Ib3,0)+
			      normal(Ib1,Ib2,Ib3,1)*vSolidLocal(Ib1,Ib2,Ib3,1) );
		    if( numberOfDimensions==3 )
		      nDotV -= normal(Ib1,Ib2,Ib3,2)*vSolidLocal(Ib1,Ib2,Ib3,2);

		    for( int dir=0; dir<numberOfDimensions; dir++ )
		      gridVelocityLocal(Ib1,Ib2,Ib3,dir) += nDotV*normal(Ib1,Ib2,Ib3,dir);  // n.v = n.vp 


		    // gridVelocityLocal(Ib1,Ib2,Ib3,Rx)=vSolidLocal(Ib1,Ib2,Ib3,Rx);  // **********
		  }
		  else
		  {
		    // t.v= ZERO

		    // gridVelocityLocal(Ib1,Ib2,Ib3,Rx)=vSolidLocal(Ib1,Ib2,Ib3,Rx);
		    // gridVelocityLocal(Ib1,Ib2,Ib3,Rx)=0.;
		    for( int dir=0; dir<numberOfDimensions; dir++ )
		      gridVelocityLocal(Ib1,Ib2,Ib3,dir) = nDotV*normal(Ib1,Ib2,Ib3,dir);
		  }
		}
		else // ** FALSE ***
		{
		  // new way
		  RealArray vp(Ib1,Ib2,Ib3,Rx), nDotV(Ib1,Ib2,Ib3);
		  // vp - v  ( note (alpha-1.) in first term )

		  if( projectVelocityOnBeamEnds )
		  {
		    // vp= (alpha-1.)*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);
		    vp= (alpha)*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);
		  }
		  else
		  {
		    // use solid velocity when the weight is zero: 
		    for( int dir=0; dir<numberOfDimensions; dir++ )
		      vp(Ib1,Ib2,Ib3,dir) = ( (   alpha*weight(Ib1,Ib2,Ib3))*uLocal(Ib1,Ib2,Ib3,uc+dir) + 
					      (1.-alpha*weight(Ib1,Ib2,Ib3))*vSolidLocal(Ib1,Ib2,Ib3,dir) );
		  }

		  // vp=( alpha*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx) 
		  // 		 -gridVelocityLocal(Ib1,Ib2,Ib3,Rx) );
	    
		  nDotV = (normal(Ib1,Ib2,Ib3,0)*vp(Ib1,Ib2,Ib3,0)+
			   normal(Ib1,Ib2,Ib3,1)*vp(Ib1,Ib2,Ib3,1) );
		  if( numberOfDimensions==3 )
		    nDotV += normal(Ib1,Ib2,Ib3,2)*vp(Ib1,Ib2,Ib3,2);

		  // if( !projectVelocityOnBeamEnds ) 
		  // {
		  //   nDotV *= weight(Ib1,Ib2,Ib3);  // turn off projection near beam ends 
		  // }
	    
		  // gridVelocityLocal(Ib1,Ib2,Ib3,Rx)=vSolidLocal(Ib1,Ib2,Ib3,Rx);

		  // -- set the normal component of the velocity ---
		  // -- set tangential component of the velocity to zero --
		  for( int dir=0; dir<numberOfDimensions; dir++ )
		    gridVelocityLocal(Ib1,Ib2,Ib3,dir) = nDotV*normal(Ib1,Ib2,Ib3,dir);
		}
	    

	      }
	      else
	      {
		if( projectVelocityOnBeamEnds ) 
		{
		  gridVelocityLocal(Ib1,Ib2,Ib3,Rx) = alpha*uLocal(Ib1,Ib2,Ib3,V) + (1.-alpha)*vSolidLocal(Ib1,Ib2,Ib3,Rx);
		}
		else
		{
		  for( int dir=0; dir<numberOfDimensions; dir++ )
		  {
		    // use solid velocity when the weight is zero
		    gridVelocityLocal(Ib1,Ib2,Ib3,dir) = ( alpha*weight(Ib1,Ib2,Ib3)*uLocal(Ib1,Ib2,Ib3,uc+dir) + 
							   (1.-alpha*weight(Ib1,Ib2,Ib3))*vSolidLocal(Ib1,Ib2,Ib3,dir) );
		  }
		}
	      }
	    } // end if false

	  
	    if( true )
	    {
	      if( t<=0. )
		printF("--PIV-- ****TEST*** set gridVelocity=0 on ends\n");
	      Index I1,I2,I3;
	      getIndex(mg.gridIndexRange(),I1,I2,I3);
	      const int axisp1 = (axis+1) % numberOfDimensions;
	      assert( axisp1==0 );
	      for( int sidea=0; sidea<=1; sidea++ )
	      {
		// *** FINISH ME ***
		if( mg.boundaryCondition(sidea,axisp1)==Parameters::noSlipWall )
		{
		  int i1 = sidea==0 ? Ib1.getBase() : Ib1.getBound();
		  gridVelocityLocal(i1,I2,I3,Rx)=0.;  // set values on WHOLE FACE
		}
		else if( mg.boundaryCondition(sidea,axisp1)==Parameters::slipWall )
		{
		  int i1 = sidea==0 ? Ib1.getBase() : Ib1.getBound();
		  gridVelocityLocal(i1,I2,I3,0)=0.; // set values on WHOLE FACE
		}
		else if( mg.boundaryCondition(sidea,axisp1)==InsParameters::inflowWithPressureAndTangentialVelocityGiven ||
			 mg.boundaryCondition(sidea,axisp1)==InsParameters::inflowWithVelocityGiven ) // **FINISH ME**
		{
		  int i1 = sidea==0 ? Ib1.getBase() : Ib1.getBound();
		  gridVelocityLocal(i1,Ib2,Ib3,Rx)=0.; // set values on end point
                
		}
	      
	    
	      }
	    }
#else
	  OV_ABORT("FINISH ME FOR PARALLEL");
#endif
	  
	  } // end deform.isBeamModel() ************* END PROJECT BEAM MODEL ******************

          // *********** FIX ME -- THIS IS DUPLICATED ************
          // -- Add a fourth-order filter to interface velocity --
	  const bool & smoothInterfaceVelocity = parameters.dbase.get<bool>("smoothInterfaceVelocity");
	  const int numberOfInterfaceVelocitySmooths=parameters.dbase.get<int>("numberOfInterfaceVelocitySmooths");
	  if( smoothInterfaceVelocity )
	  {
	    const real omega=1.; // .5;
	    // real omega=.125; 
            if( t <= 10.*dt )
	      printF("--PIV--: smooth interface velocity, numberOSmooths=%i (4th order filter, omega=%g) grid=%i t=%9.3e...\n",
		     numberOfInterfaceVelocitySmooths,omega,grid,t);
	    
            int extra=-1;
            getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // leave off end points
	    Range Rx=numberOfDimensions;

	    assert( numberOfDimensions==2 );  // *FIX ME for 3D*
	    
	    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
	    is1=is2=is3=0;
            const int axisp1 = (axis+1) % numberOfDimensions;
	    isv[axisp1]=1;
            realArray & gv = gridVelocity;
	    for( int smooth=0; smooth<numberOfInterfaceVelocitySmooths; smooth++ )
	    {
              // ADJACENT boundary conditions **FINISH ME**
	      if( true )
	      {
                assert( axisp1==0 );
		int i1a=mg.gridIndexRange(0,0), i1b=mg.gridIndexRange(1,0);
                // -- extrapolate ghost points ---
		gv(i1a-1,Ib2,Ib3,Rx)=3.*gv(i1a,Ib2,Ib3,Rx)-3.*gv(i1a+1,Ib2,Ib3,Rx)+gv(i1a+2,Ib2,Ib3,Rx);
		gv(i1b+1,Ib2,Ib3,Rx)=3.*gv(i1b,Ib2,Ib3,Rx)-3.*gv(i1b-1,Ib2,Ib3,Rx)+gv(i1b-2,Ib2,Ib3,Rx);
	      }
	      

	      // smooth interface values
              // NOTE: for now we smooth all components of the velocity
              gv(Ib1,Ib2,Ib3,Rx)= gv(Ib1,Ib2,Ib3,Rx) + 
		(omega/16.)*( -   gv(Ib1-2*is1,Ib2-2*is2,Ib3,Rx) 
			      +4.*gv(Ib1-  is1,Ib2-  is2,Ib3,Rx) 
			      -6.*gv(Ib1,      Ib2      ,Ib3,Rx) 
			      +4.*gv(Ib1+  is1,Ib2+  is2,Ib3,Rx) 
			      -   gv(Ib1+2*is1,Ib2+2*is2,Ib3,Rx) );
	    } // end smooths
	  } // end smoothSurface
	  


	}
      }
    }
	

  } // end if bd.dbase.has_key("deformingBodyNumber") )


  return 0;
}



//============================================================================================
/// \brief This function is called from applyBoundaryConditions to assign some 
/// interface conditions (e.g. velocity projection for beams) that are not handled by cgmp. 
//============================================================================================
int Cgins::
assignInterfaceBoundaryConditions(GridFunction & cgf,
				  const int & option /* =-1 */,
				  int grid_ /* = -1 */,
				  GridFunction *puOld /* =NULL */, 
				  const real & dt /* =-1. */ )
{
  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
  const bool & projectAddedMassVelocity = parameters.dbase.get<bool>("projectAddedMassVelocity");
  const int initialConditionsAreBeingProjected = parameters.dbase.get<int>("initialConditionsAreBeingProjected");
  const bool & projectBeamVelocity = parameters.dbase.get<bool>("projectBeamVelocity");
  if( useAddedMassAlgorithm && projectAddedMassVelocity && !initialConditionsAreBeingProjected 
      && projectBeamVelocity 
      && cgf.t>0. )
  {

    // --- project the velocity of the beam to match that from the fluid ---
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();

    // ----- We only project defomring body interfaces for now ----
    if( numberOfDeformingBodies==0 )
      return 0;


    if( cgf.t <= 5*dt ) 
      printF("--INS-- assignInterfaceBoundaryConditions: PROJECT-INTERFACE-VELOCITY at t=%8.2e\n",cgf.t);

    movingGrids.projectInterfaceVelocity( cgf );

    if( true  ) // this could be an option : reprojectFluidVelocity
    {
      // ---- After projecting the solid  (beam) velocity we need to make sure the
      //   the fluid velocity on both sides of the beam matches the new beam velocity.
      //   This ensures the fluid velocity on opposite sides of the beam are now consistent with the single beam velocity,
      //   otherwise the fluid velocity on opposite sides of the beam could get out of sync.
      CompositeGrid & cg = cgf.cg;
      const int numberOfDimensions = cg.numberOfDimensions();
      const int uc = parameters.dbase.get<int >("uc");
      Range V = Range(uc,uc+numberOfDimensions-1);

      for( int body=0; body<numberOfDeformingBodies; body++ )
      {
	DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
        const bool beamModelHasFluidOnTwoSides = deformingBody.beamModelHasFluidOnTwoSides();
	if( beamModelHasFluidOnTwoSides )
	{
          // --- this body is a beam with fluid on two sides ---
	  DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
	  const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
	  const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");
	    

	  BeamFluidInterfaceData &  beamFluidInterfaceData = 
	    deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
	  Index Ib1,Ib2,Ib3;
	  for( int face=0; face<numberOfFaces; face++ )
	  {
	    const int side=boundaryFaces(0,face);
	    const int axis=boundaryFaces(1,face);
	    const int grid=boundaryFaces(2,face);
	    MappedGrid & mg = cg[grid];
	    OV_GET_SERIAL_ARRAY(real,cgf.u[grid],uLocal);
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3); // boundary index's for mg

	    Range Rx=numberOfDimensions;
	    RealArray vSolid(Ib1,Ib2,Ib3,Rx); // holds velocity of solid on the boundary
	    deformingBody.getVelocityBC( cgf.t, grid, mg, Ib1,Ib2,Ib3, vSolid );

	    uLocal(Ib1,Ib2,Ib3,V)=vSolid(Ib1,Ib2,Ib3,Rx);


            // *********** FIX ME -- THIS IS DUPLICATED ************
	    // -- Add a fourth-order filter to interface velocity --
	    const bool & smoothInterfaceVelocity = parameters.dbase.get<bool>("smoothInterfaceVelocity");
	    const int numberOfInterfaceVelocitySmooths=parameters.dbase.get<int>("numberOfInterfaceVelocitySmooths");
	    if( smoothInterfaceVelocity )
	    {
	      const real omega=1.; // .5 
	      // real omega=.125; 
	      if( cgf.t <= 10.*dt )
		printF("--IBC--: smooth interface velocity, numberOSmooths=%i (4th order filter, omega=%g) grid=%i t=%9.3e...\n",
		       numberOfInterfaceVelocitySmooths,omega,grid,cgf.t);
	    
	      int extra=-1;
	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3,extra); // leave off end points

	      assert( numberOfDimensions==2 );  // *FIX ME for 3D*
	    
	      int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
	      is1=is2=is3=0;
	      const int axisp1 = (axis+1) % numberOfDimensions;
	      isv[axisp1]=1;
	      RealArray & gv = uLocal;
	      for( int smooth=0; smooth<numberOfInterfaceVelocitySmooths; smooth++ )
	      {
		// ADJACENT boundary conditions **FINISH ME**
		if( true )
		{
		  assert( axisp1==0 );
		  int i1a=mg.gridIndexRange(0,0), i1b=mg.gridIndexRange(1,0);
		  // -- extrapolate ghost points ---
		  gv(i1a-1,Ib2,Ib3,V)=3.*gv(i1a,Ib2,Ib3,V)-3.*gv(i1a+1,Ib2,Ib3,V)+gv(i1a+2,Ib2,Ib3,V);
		  gv(i1b+1,Ib2,Ib3,V)=3.*gv(i1b,Ib2,Ib3,V)-3.*gv(i1b-1,Ib2,Ib3,V)+gv(i1b-2,Ib2,Ib3,V);
		}
	      

		// smooth interface values
		// NOTE: for now we smooth all components of the velocity
		gv(Ib1,Ib2,Ib3,V)= gv(Ib1,Ib2,Ib3,V) + 
		  (omega/16.)*( -   gv(Ib1-2*is1,Ib2-2*is2,Ib3,V) 
				+4.*gv(Ib1-  is1,Ib2-  is2,Ib3,V) 
				-6.*gv(Ib1,      Ib2      ,Ib3,V) 
				+4.*gv(Ib1+  is1,Ib2+  is2,Ib3,V) 
				-   gv(Ib1+2*is1,Ib2+2*is2,Ib3,V) );
	      } // end smooths
	    } // end smoothSurface


	  }
	  
	} // end if beamModelHasFluidOnTwoSides
	
      } // end for body
    }
    
  }
  
  return 0;
}
