#include "Cgins.h"
#include "App.h"
#include "ParallelUtility.h"
#include "DeformingBodyMotion.h"
#include "BeamFluidInterfaceData.h"
#include "Oges.h"
#include "RigidBodyMotion.h"

#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)

// void Cgins::
// gridAccelerationBC(const int & grid,
// 		   const real & t0,
// 		   MappedGrid & c,
// 		   realMappedGridFunction & u ,
// 		   realMappedGridFunction & f ,
// 		   realMappedGridFunction & gridVelocity ,
// 		   realSerialArray & normal,
// 		   const Index & I1,
// 		   const Index & I2,
// 		   const Index & I3,
// 		   const Index & I1g,
// 		   const Index & I2g,
// 		   const Index & I3g,
// 		   int side,
//                    int axis   )
//\begin{>>MappedGridSolverInclude.tex}{\subsection{gridAccelerationBC}} 
void Cgins::
gridAccelerationBC(const int & grid,
		   const real & t0,
		   GridFunction & gf0, 
		   realCompositeGridFunction & f0,
		   int side,
                   int axis   )
//=================================================================================
// /Description:
// Add the grid acceleration in the normal direction, -n.x_tt,  to the function f
//   f is normally the function that holds the rhs for the pressure eqn
// /I1,I2,I3 (input) : Index's on boundary
// /I1g,I2g,I3g (input) : Index's on ghost line, fill in f(I1g,I2g,I3g)
// /side,axis (input) : fill in this face.
//\end{MappedGridSolverInclude.tex}  
//=================================================================================
{

  MappedGrid & c = gf0.cg[grid];
  realMappedGridFunction & u = gf0.u[grid];
  realMappedGridFunction & f = f0[grid];
  realMappedGridFunction & gridVelocity = gf0.getGridVelocity(grid);

  OV_GET_SERIAL_ARRAY(real,f,fLocal);
  OV_GET_SERIAL_ARRAY(real,gridVelocity,gridVelocityLocal);
  #ifdef USE_PPP
    realSerialArray & normal = c.vertexBoundaryNormalArray(side,axis);
    // realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries(gridVelocity,gridVelocityLocal);
    // realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f,fLocal);
  #else
    realSerialArray & normal = c.vertexBoundaryNormal(side,axis);
    // realSerialArray & gridVelocityLocal=gridVelocity;
    // realArray & fLocal = f;
  #endif

  const int numberOfDimensions = c.numberOfDimensions();
  const int numberOfComponentGrids = gf0.cg.numberOfComponentGrids();
  const int & pc = parameters.dbase.get<int >("pc");

  Index I1,I2,I3;
  Index I1g,I2g,I3g;
  getGhostIndex( c.extendedIndexRange(),side,axis,I1 ,I2 ,I3 ,0);     // boundary line
  getGhostIndex( c.extendedIndexRange(),side,axis,I1g,I2g,I3g,1);  // first ghost line


  if( parameters.gridIsMoving(grid) 
      && parameters.dbase.get<real >("advectionCoefficient")!=0. )  // this is needed by project for some reason? )
  {

    // -- get the grid acceleration from the MovingGrid's class ---

    std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
    BoundaryData & bd = boundaryDataArray[grid];

    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
    const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();
    const int numberOfRigidBodies= movingGrids.getNumberOfRigidBodies();

    const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
    if( useAddedMassAlgorithm && numberOfDeformingBodies>0 )
    {
      // -------- DEFORMING BODY AMP STAGE I  --------------

      // For the added-mass (beam) pressure BC, we scaled by rhos*As/rho 
      // This will case the RHS to the prssure equation (e.g. n^T( nu*Delta(v) )
      const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

      // mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
      const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
      RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
      if( deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
          mixedCoeff(pc,side,axis,grid)==1. )  
      {
	if( debug() & 4 && t0 <= dt )
	  printF("--INS-- gridAccelerationBC: t=%8.2e, scale pressure BC by rhos*As/rho =%8.2e grid=%i, (side,axis)=(%i,%i:  mixedCoeff=%8.2e )\n",
		 t0,mixedNormalCoeff(pc,side,axis,grid),grid,side,axis, mixedCoeff(pc,side,axis,grid));
	f(I1g,I2g,I3g) *= mixedNormalCoeff(pc,side,axis,grid);
      }
      // ----------- END DEFORMING BODY AMP STAGE I  ------------
    }
    


    // ----------------------------------------------
    // ------ Get the grid acceleration term --------
    // ----------------------------------------------
    movingGrids.gridAccelerationBC(grid,side,axis,t0,c,u,f,gridVelocity,normal,I1,I2,I3,I1g,I2g,I3g);



    if( useAddedMassAlgorithm && numberOfDeformingBodies>0 )
    {
      // -------- DEFORMING BODY AMP STAGE II ----------------

      const bool & useApproximateAMPcondition = parameters.dbase.get<bool>("useApproximateAMPcondition");
      if( useApproximateAMPcondition )
      {
	// ------------------------------------
	// --- APPROXIMATE AMP PRESSURE BC ----
	// ------------------------------------
	// DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

	const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
	RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
	const int & pc = parameters.dbase.get<int >("pc");
	if( deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
	    mixedCoeff(pc,side,axis,grid)==1. )
	{
	  if( t0 < 5.*dt )
	    printF("--INS-- Approx. AMP BC: Add on traction term from AMP BC t=%9.3e\n",t0);

	  // Add on traction term  from AMP BC
	  // For two sided beams there are contributions from both sides
	  // f +=  [ nv^T( tau ) nv]_+^- 

	  // --- compute the forces on the surface ---
	  int ipar[] = {grid,side,axis,gf0.form}; // 
	  real rpar[] = { t0 }; // 
	  RealArray stressLocal(I1,I2,I3,numberOfDimensions);
	  parameters.getNormalForce( gf0.u,stressLocal,ipar,rpar );

	  const real fluidDensity = parameters.dbase.get<real>("fluidDensity")!=0. ? parameters.dbase.get<real>("fluidDensity") : 1.;
	  OV_GET_SERIAL_ARRAY(real,u,uLocal);

	  if( numberOfDimensions==2 )
	  {
	    // ADD viscous traction term (remove the pressure component)
	    // AMP:   p + (rhos*hs)/rho p.n = L   + n^T tau n    (plus sign)
	    if( t0 < 10.*dt  )
	    {
	      RealArray nTaun(I1,I2,I3);
	      nTaun=(normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
		     normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) ) 
		-fluidDensity*uLocal(I1,I2,I3,pc);

	      printF("--INS-- Approx. AMP BC: (side,axis,grid)=(%i,%i,%i) t=%9.3e |n.taun|=%8.2e\n",side,axis,grid,t0,max(fabs(nTaun)));
	    }
	  
	    fLocal(I1g,I2g,I3g) += (normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
				    normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) ) 
	      -fluidDensity*uLocal(I1,I2,I3,pc);
	  }
	  else
	  {
	    fLocal(I1g,I2g,I3g) += (normal(I1,I2,I3,0)*stressLocal(I1,I2,I3,0) +  
				    normal(I1,I2,I3,1)*stressLocal(I1,I2,I3,1) + 
				    normal(I1,I2,I3,2)*stressLocal(I1,I2,I3,2) ) 
	      -fluidDensity*uLocal(I1,I2,I3,pc);
	  }
	
	
	}

      }
      else
      {
	// ------------------------------------
	// --- ADJUSTED AMP PRESSURE BC ----
	// ------------------------------------

	// subtract off the current guess for sigma*n : *wdh* 2015/01/04 
	const int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");

	// mixedNormalCoeff(pc,side,axis,grid)=beamMassPerUnitLength[side][axis]/fluidDensity;
	const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
	RealArray & bcData = parameters.dbase.get<RealArray>("bcData");      
	const int & pc = parameters.dbase.get<int >("pc");
	if( TRUE &&
	    deformingBodyNumber[side][axis]>=0 &&   // -- this is an interface --
	    mixedCoeff(pc,side,axis,grid)==1. )
	{
	  if( (true || debug() & 4) && t0 <= dt )
	    printF("--INS-- gridAccelerationBC: t=%8.2e, ADD p to grid acceleration RHS: grid=%i, (side,axis)=(%i,%i)\n",
		   t0,grid,side,axis);
	  if( (false ||  debug() & 4) && t0 <= dt )
	  {
	    ::display(u(I1,I2,I3,pc),sPrintF("--INS-- gridAccelerationBC: pressure p at t=%g (grid,side,axis)=(%i,%i,%i)",t0,grid,side,axis),"%9.2e ");
	    ::display(f(I1g,I2g,I3g),sPrintF("--INS-- gridAccelerationBC: f=gDot at t=%g (grid,side,axis)=(%i,%i,%i)",t0,grid,side,axis),"%9.2e "); 
	  }
	
	  f(I1g,I2g,I3g) += u(I1,I2,I3,pc) ; 

	  // For two sided beams we need to adjust the opposite side 
	  if( true ) 
	  {
	    // *NEW WAY*

	    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
	    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

	    MappedGrid & mg = gf0.cg[grid];
	    const IntegerArray & gid = mg.gridIndexRange();
	    for( int dir=0; dir<3; dir++ ){ iv[dir]=gid(0,dir); } //
	    const int axisp1= (axis +1) % numberOfDimensions;
	  
	    for( int body=0; body<numberOfDeformingBodies; body++ ) // *FIX* We know the body number from above ***
	    {
	      DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	      if( !deformingBody.beamModelHasFluidOnTwoSides() )
	      { // this is NOT a beam model with fluid on two sides.
		continue;   
	      }

	      DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
	      const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
	      const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

	      // --- beam-fluid interface data is stored here:
	      BeamFluidInterfaceData &  beamFluidInterfaceData = 
		deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
	      IntegerArray *& donorInfoArray = beamFluidInterfaceData.dbase.get<IntegerArray*>("donorInfoArray");

	      is1=is2=is3=0;
	      isv[axis]=1-2*side;

	      for( int face=0; face<numberOfFaces; face++ )
	      {
		const int side0=boundaryFaces(0,face);
		const int axis0=boundaryFaces(1,face);
		const int grid0=boundaryFaces(2,face); 
		if( grid0==grid && side0==side && axis0==axis )  // beam face is found
		{
		  const IntegerArray & donorInfo= donorInfoArray[face]; 
		  Range I0=donorInfo.dimension(0);
		  for( int i=I0.getBase(); i<=I0.getBound(); i++ )  // NOTE: loop index i is incremented below
		  {
		    // Here is the donor on the opposite face of the beam:
		    const int grid1 = donorInfo(i,0), side1=donorInfo(i,1), axis1=donorInfo(i,2);

		    if( grid1<0 )  // This means there is do opposite grid point -- could be the end of the beam
		      continue;

		    assert( grid1>=0 && grid1<numberOfComponentGrids );
		  
		    const realArray & u1 = gf0.u[grid1];
		    for( ; i<=I0.getBound(); i++ ) // NOTE: this increments "i" from the outer loop
		    {
		      if( donorInfo(i,0)!=grid1 )
		      {
			i--;
			break;
		      }
		      // closest grid pt on opposite side:
		      const int j1=donorInfo(i,3), j2=donorInfo(i,4), j3=donorInfo(i,5); 

		      iv[axisp1]=i+gid(0,axisp1); // index that varies along the interface of grid
		      int i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)

		      f(i1m,i2m,i3m) -= u1(j1,j2,j3,pc) ;
		    }
		  }
		}
	      }
	    }
	  
	  }
	  else
	  {
	    const int bodyNumber=deformingBodyNumber[side][axis];
	    for( int grid2=0; grid2<numberOfComponentGrids; grid2++ )
	    {
	      if( grid2!=grid && parameters.gridIsMoving(grid2) )
	      {
		// **FIX ME** this will only work for deforming bodies.
		BoundaryData & bd2 = boundaryDataArray[grid2];
		const int (&deformingBodyNumber2)[2][3] = bd2.dbase.get<int[2][3]>("deformingBodyNumber");
		for( int dir2=0; dir2<numberOfDimensions; dir2++ )
		{
		  for( int side2=0; side2<=1; side2++ )
		  {
		    if( deformingBodyNumber2[side2][dir2]==bodyNumber )
		    {
		      realMappedGridFunction & u2 = gf0.u[grid2];
		      // Here we assume that the gridlines match *FIX ME*
		      Index Jb1,Jb2,Jb3;
		      getGhostIndex( gf0.cg[grid2].extendedIndexRange(),side2,dir2,Jb1,Jb2,Jb3,0);     // boundary line
		      if( !( I1==Jb1 && I2==Jb2 && I3==Jb3 ) )
		      {
			OV_ABORT("finish me");
		      }
		   
		      f(I1g,I2g,I3g) -= u2(Jb1,Jb2,Jb3,pc) ;

		      if( debug() & 4 || t0 <= dt )
			printF("--INS-- gridAccelerationBC: t=%8.2e, SUBTRACT p on grid2=%i from grid acceleration RHS: grid=%i, (side,axis)=(%i,%i)\n",
			       t0,grid2,grid,side,axis);
              
		    }
		  }
		}
	      }
	    }
	  }
	}
      
      } // end else adjusted AMP
      
    } // end if useAddedMass
    
    
  }

  // if( parameters.dbase.get< >("timeDependentBoundaryConditions") ) // ********** fix this *****
  if( parameters.bcIsTimeDependent(side,axis,grid) )
  {
    // add grid acceleration from time dependent boundary conditions (e.g. user defined)
    // if( getTimeDependentBoundaryConditions(t0,grid,side,axis,computeTimeDerivativeOfForcing) )
    if( getTimeDerivativeOfBoundaryValues( gf0, t0, grid,side,axis) )
    {
      // realArray & bd = gridVelocity; // ************************** fix this
      const int uc=parameters.dbase.get<int >("uc");
      const int vc=parameters.dbase.get<int >("vc");
      const int wc=parameters.dbase.get<int >("wc");
      // printf("gridAccelerationBC: add acceleration BC to grid=%i, side=%i, axis=%i\n",grid,side,axis);
      if( parameters.dbase.get<IntegerArray>("variableBoundaryData")(grid) )
      {
        RealArray & bd = parameters.getBoundaryData(side,axis,grid,c); 
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*bd(I1,I2,I3,uc)+
			      normal(I1,I2,I3,1)*bd(I1,I2,I3,vc));
	if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	{
	  fLocal(I1g,I2g,I3g)-=normal(I1,I2,I3,2)*bd(I1,I2,I3,wc);
	}
      }
      else
      {
        const RealArray & bcData  = parameters.dbase.get<RealArray>("bcData");
	
	fLocal(I1g,I2g,I3g)-=(normal(I1,I2,I3,0)*bcData(uc,side,axis,grid)+
			      normal(I1,I2,I3,1)*bcData(vc,side,axis,grid));
	if( parameters.dbase.get<int >("numberOfDimensions")==3 )
	{
	  fLocal(I1g,I2g,I3g)-=normal(I1,I2,I3,2)*bcData(wc,side,axis,grid);
	}
      }
    }
  }
  
}


// ==========================================================================================================================
/// \brief Set the values for the right-hand-sides of the constraint equations in the pressure equation.
///    This function is called by assignPressureRHS before the pressure solve
//
// --- set the right-hand-side values of the constraint equations in the pressure equation ----
//   (1) mean pressure (if the pressure equation is singular)
//   (2) Rigid body added mass equations. 
//
// ==========================================================================================================================
int Cgins::
setPressureConstraintValues( GridFunction & gf0, realCompositeGridFunction & f )
{

  const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  
  FILE *&debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *&pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  real t0 = gf0.t;
  CompositeGrid & cg = gf0.cg;

  assert( poisson!=NULL );
  Oges & pSolver = *poisson;

  const int numberOfExtraEquations = pSolver.getNumberOfExtraEquations();

  real pressureMeanValue=0.;  // holds RHS to the pressure global constraint
  
  if( poisson->getCompatibilityConstraint() )
  { 
     // ---- Evaluate the value of the compatibility equation for the pressure ---
     //      (This value is passed to Oges below)

    if( debug() & 4 ) 
       printF("))) assignPressureRHS: set compatibility constraint for singular problem solver=%s (((\n",
                (const char*)getName() );

    const int & pc = parameters.dbase.get<int >("pc");

    const Parameters::KnownSolutionsEnum & knownSolution = parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");

    if( twilightZoneFlow ||
        knownSolution!=InsParameters::noKnownSolution )
    {
      // For TZ: the RHS for the constraint equation is the dot product of the constraint equation
      //         with the exact solution
      Range all;
      realCompositeGridFunction ue(gf0.cg,all,all,all,Range(pc,pc));

      if( twilightZoneFlow )
      {
        // --- evaluate the TZ solution for the pressure ---
	parameters.dbase.get<OGFunction* >("exactSolution")->assignGridFunction(ue,t0);
      }
      else if( knownSolution!=InsParameters::noKnownSolution )
      {
        // --- evaluate the known solution so we can set the pressure constraint --- *wdh* 2013/09/28 
        realCompositeGridFunction & uKnown = parameters.getKnownSolution(cg,t0);
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          OV_GET_SERIAL_ARRAY(real,ue[grid],ueLocal);
          OV_GET_SERIAL_ARRAY_CONST(real,uKnown[grid],uKnownLocal);
	  
	  ueLocal(all,all,all,pc)=uKnownLocal(all,all,all,pc);
	}
      }
      else
      {
	OV_ABORT("Unknown option");
      }
      
      // real value2;
      poisson->evaluateExtraEquation(ue,pressureMeanValue);
      if( debug() & 4 ) printF("assignPressureRHS: compatibility constraint: exact value=%14.9g \n",pressureMeanValue);
    }
    
  }

  if( numberOfExtraEquations==1 && poisson->getCompatibilityConstraint() )
  { 
    // ---- Set the "mean" value of the pressure ----
    //   (if there are no extra constraint equations)
    poisson->setExtraEquationValues(f,&pressureMeanValue );
  }
  


  // --- set the values at constraint equations for added-mass rigid body solve ---
  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
  if( useAddedMassAlgorithm && numberOfExtraEquations>1 )
  {
    MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");    
    const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();

    const bool & useAddedDampingAlgorithm = parameters.dbase.get<bool>("useAddedDampingAlgorithm");

    if( numberOfRigidBodies>0 )
    {
      // ======================================================================
      // =================== RIGID BODY AMP SCHEME ============================
      // ======================================================================

      // RBINS-AMP FINISH ME: 
      // Compute rigid-body "internal force"  
      //     mb*a = Fi   (a=linear accleration)
      //     Mb*b = Ti   (b=angular acceleration)
      // Compute inviscid force and torque (from pressure only)
      // Set RHS to contraints:
      //   mb a - INT ( p nv) ds      =  Fi - Fp
      //   Mb b - INT ( r X p nv) ds  =  Ti - Tp

      
      // ----------- Compute pressure contributions to the force and torque on the rigid body ----------
      //             using the current guess for the pressure.

      RealArray bodyForceFromPressure, bodyTorqueFromPressure;
      bool includeGravity = false, includeViscosity = false;
      movingGrids.getForceOnRigidBodies( bodyForceFromPressure,bodyTorqueFromPressure, gf0,includeGravity,includeViscosity );

      // ************* TESTING ***************
      // Compute force on body due to viscous stress
      RealArray bodyForceFromViscousStress, bodyTorqueFromViscousStress;
      includeGravity = false; includeViscosity = true;
      movingGrids.getForceOnRigidBodies( bodyForceFromViscousStress,bodyTorqueFromViscousStress, 
                                         gf0,includeGravity,includeViscosity );
      bodyForceFromViscousStress-=bodyForceFromPressure;
      bodyTorqueFromViscousStress-=bodyTorqueFromPressure;

      printF("--INS:setPressureConstraintValues: t=%9.3e bodyTorqueFromViscousStress=%9.3e\n",
             gf0.t,bodyTorqueFromViscousStress(2,0));

      // ***************************

      // Extra equations:
      //    For each rigid body:
      //      2D    nd +1 : extra equation for RB linear and 1 angular acceleration 
      //      3D:   2*nd  : extra equation for RB linear and angular acceleration 
      const int numberOfDimensions=cg.numberOfDimensions();
      int numberOfExtraEquationsPerBody;
      if( numberOfDimensions==2 )
      {
	// In 2D we keep (a1,a2) and (b3)
	numberOfExtraEquationsPerBody= numberOfDimensions + 1;
      }
      else
      {
	// int 3D we keep (a1,a2,a3) and (b1,b2,b3)
	numberOfExtraEquationsPerBody = numberOfDimensions + numberOfDimensions;
      }
      // There may be one "dense" constraint equation setting the mean of the pressure: 
      int numberOfDenseExtraEquations=0;
      if( pSolver.getCompatibilityConstraint() )
      {
	numberOfDenseExtraEquations=1;// constraint setting mean-value of p
      }

      const int totalNumberOfExtraEquations=numberOfDenseExtraEquations+numberOfExtraEquations;

      // ***FIX ME*** Use latest predicted pressure ***
      printF("--INS--setPressureConstraintValues: t=%9.3e numberOfExtraEquations=%i numberOfDenseExtraEquations=%i numberOfRigidBodies=%i\n"
             "       ******FIX ME: Use latest predicted pressure ***\n",t0,numberOfExtraEquations,numberOfDenseExtraEquations,numberOfRigidBodies);

      real *value= new real[numberOfExtraEquations];
      for( int i=0; i<numberOfExtraEquations; i++ ){ value[i]=0.; }  // NOTE constraintValues are in reverse order

      RealArray mvDot,mOmegaDot; // Holds body force and torque 
      RealArray addedDampingTensors(3,3,2,2);  // holds added damping Tensors - 4 3x3 matrices 
      RealArray vDotPredicted(3), omegaDotPredicted(3);
      
      // --------------- LOOP OVER RIGID BODIES --------------------
      for( int b=0; b<numberOfRigidBodies; b++ )
      {
	RigidBodyMotion & body = movingGrids.getRigidBody(b);
	// return the "total force" on the body : pressure forces, viscous forces, gravity, etc.
	body.getMassTimesAcceleration( t0,mvDot,mOmegaDot );

	if( useAddedDampingAlgorithm )
	{
	  movingGrids.getRigidBodyAddedDampingTensors( b, addedDampingTensors, gf0,dt );

          body.getAcceleration( t0, vDotPredicted  ); // predicted value for vDot

          body.getAngularAcceleration( t0, omegaDotPredicted  ); // predicted value for omegaDot
	}

        // Extra equations:
        //     Body   
        //  0    0     a1
        //  1    0    
        //  2
        const int vbc=0, wbc=1; // component numbers of v and omega in addedDampingTensors
        const real & dt=parameters.dbase.get<real>("dt");
	for( int d=0; d<numberOfDimensions; d++ )
	{
          const int extraEqn = d + (b)*numberOfExtraEquationsPerBody; // current extra equation 
          // int ival = totalNumberOfExtraEquations -extraEqn -1;        // equations are stored in reverse order
          // CHECK ME: numberOfDenseExtraEquations: 
          int ival = totalNumberOfExtraEquations -extraEqn -1 -numberOfDenseExtraEquations;        // equations are stored in reverse order
	  assert( ival<numberOfExtraEquations );
	  value[ival] = mvDot(d) - bodyForceFromPressure(d,b);  // *** "+"
	  if( useAddedDampingAlgorithm )
	  {
	    for( int dir2=0; dir2<3; dir2++ ) // note 3 for dimensions since AD tensor is always 3x3
	    {
              value[ival] += dt*addedDampingTensors(d,dir2,vbc,vbc)*vDotPredicted(dir2);
              value[ival] += dt*addedDampingTensors(d,dir2,vbc,wbc)*omegaDotPredicted(dir2);
	    }
	  }
	  

	  printF("--INS--setPressureConstraintValues: body=%i, d=%i, m*a=%9.2e, bodyForce: (pressure=%9.2e,viscous=%9.2e), value=%9.3e\n",
                 b,d,mvDot(d),bodyForceFromPressure(d,b),bodyForceFromViscousStress(d,b),value[ival]);
	  
	}
	const int numberOfAngularVelocities = numberOfDimensions==2 ? 1 : numberOfDimensions;
	for( int d=0; d<numberOfAngularVelocities; d++ )
	{
          const int extraEqn = numberOfDimensions + d + (b)*numberOfExtraEquationsPerBody; // current extra equation 
          int ival = totalNumberOfExtraEquations -extraEqn -1 -numberOfDenseExtraEquations;        // equations are stored in reverse order
	  assert( ival<numberOfExtraEquations );
          int dir = numberOfDimensions==2 ? 2 : d;                    // In 2D we use component 2 of the angular acceleration
	  value[ival] = mOmegaDot(dir) - bodyTorqueFromPressure(dir,b);  

	  if( useAddedDampingAlgorithm )
	  {
	    for( int dir2=0; dir2<3; dir2++ )  // note 3 for dimensions since AD tensor is always 3x3
	    {
	      value[ival] += dt*addedDampingTensors(dir,dir2,wbc,vbc)*vDotPredicted(dir2);
	      value[ival] += dt*addedDampingTensors(dir,dir2,wbc,wbc)*omegaDotPredicted(dir2);
	    }
	    // const real Dww = addedDampingTensors(2,2,1,1);  // coeff of the angular velocity in the omega_t eqn

            // // *wdh* 2016/03/03 const real impFactor=.1;  // **************** CHECK ME ************
            // const real impFactor=1.;  // SHOULD MATCH VALUE IN ADJUSTPRESSURECOEFFICIENTS.C

	    // printF("--INS-SPC-- addedDamping coeff: Dww=%8.2e, dt=%8.2e, omegaDotPredicted=%8.2e\n",
            //        Dww,dt,omegaDotPredicted(2));
	    // value[ival] += impFactor*dt*Dww*omegaDotPredicted(2);

	  }

	  if( FALSE )// ***TEST****
          {
	    value[ival] = bodyTorqueFromViscousStress(dir,b);
	  }
	  
	  printF("--INS--setPressureConstraintValues: body=%i, d=%i, A*w_t=%9.3e, bodyTorque (pressure=%9.3e,viscous=%9.3e), value=%9.3e\n",
                 b,dir,mOmegaDot(dir),bodyTorqueFromPressure(dir,b),bodyTorqueFromViscousStress(dir,b),  value[ival]);
	}
      } // end for body b
    
      if( poisson->getCompatibilityConstraint() )
      {
	value[0]=pressureMeanValue;  // Assign the mean pressure constraint too
      }

      // --- Now assign the right-hand-side values for the RBINS AMP constraints on the acceleration ---
      poisson->setExtraEquationValues( f,value );

      delete [] value;

    } // end if numberOfRigidBodies
    
  } //end if useAddedMass
  


}



// ==========================================================================================================================
/// \brief Check and adjust the values of the solutions to the constraint equations in the pressure equation.
///    This function is called by solveForTimeIndependentVariables after the pressure solve.
// ==========================================================================================================================
int Cgins::
checkPressureConstraintValues( GridFunction & cgf )
{
  real & t = cgf.t;
  realCompositeGridFunction & u = cgf.u;
  
  const int & myid = parameters.dbase.get<int >("myid");
  const int & pc = parameters.dbase.get<int >("pc");
  FILE *&debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *&pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");
  
  CompositeGrid & cg = cgf.cg;
  Index I1,I2,I3;

  if( debug() & 4 )
  {
    fPrintF(debugFile," After pressure solve: compatibilityConstraint=%i, numberOfExtraEquations = %i\n",
	    poisson->getCompatibilityConstraint(),poisson->numberOfExtraEquations);
    if( poisson->getCompatibilityConstraint() )
    {
      real value=0.;
      poisson->getExtraEquationValues( p(),&value );

//       int ne,i1e,i2e,i3e,gride;
//       poisson->equationToIndex( poisson->extraEquationNumber(0),ne,i1e,i2e,i3e,gride);
//       if( myid==0 )
//         fprintf(debugFile," After pressure solve: value of constraint = %e\n",p()[gride](i1e,i2e,i3e));
      fPrintF(debugFile," After pressure solve: value of constraint = %e\n",value);


    }
  }

  // --- check values at constraint equations for added-mass rigid body solve
  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
  if( useAddedMassAlgorithm )
  {
    const int numberOfExtraEquations = poisson->numberOfExtraEquations;
    if( numberOfExtraEquations>1 ) // AMP scheme will have more than 1 extra equation
    {
      // ===== Get solutions to constraint equations =======
      real *constraintValues = new real [numberOfExtraEquations];
	
      poisson->getExtraEquationValues( u, constraintValues );  
      for( int i=0; i<numberOfExtraEquations; i++ )
      { // NOTE constraintValues are in reverse order
	printF("--INS-STI-- After pressure solve: extraEquation: i=%i : constraint value=%10.3e\n",i,
               constraintValues[numberOfExtraEquations-i-1]);
      }

      // ====================================================================================
      // ======== SET THE ACCELERATION OF THE RIGID BODY EQUAL TO THE CONSTRAINT VALUE ======
      // ====================================================================================

      MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");    
      const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();

      if( numberOfRigidBodies>0 )
      {
	// ======================================================================
	// =================== RIGID BODY AMP SCHEME ============================
	// ======================================================================


        RealArray vDot(3), omegaDot(3);
	vDot=0.; omegaDot=0.;
	const int numberOfDimensions=cg.numberOfDimensions();
	int numberOfExtraEquationsPerBody;
	if( numberOfDimensions==2 )
	  numberOfExtraEquationsPerBody= numberOfDimensions + 1;
        else
	  numberOfExtraEquationsPerBody = numberOfDimensions + numberOfDimensions;
	// There may be one "dense" constraint equation setting the mean of the pressure: 
	int numberOfDenseExtraEquations=0;
	if( poisson->getCompatibilityConstraint() )
	{
	  numberOfDenseExtraEquations=1;// constraint setting mean-value of p
	}
	const int totalNumberOfExtraEquations=numberOfDenseExtraEquations+numberOfExtraEquations;

	// --------------- LOOP OVER RIGID BODIES --------------------
	for( int b=0; b<numberOfRigidBodies; b++ )
	{
	  RigidBodyMotion & body = movingGrids.getRigidBody(b);
	  // Fill in the acceleration of the body
	  for( int d=0; d<numberOfDimensions; d++ )
	  {
	    const int extraEqn = d + (b)*numberOfExtraEquationsPerBody; // current extra equation 
	    int ival = totalNumberOfExtraEquations -extraEqn -1 -numberOfDenseExtraEquations;        // equations are stored in reverse order
	    assert( ival<numberOfExtraEquations );
            vDot(d)=constraintValues[ival];
	  }
	  const int numberOfAngularVelocities = numberOfDimensions==2 ? 1 : numberOfDimensions;
	  for( int d=0; d<numberOfAngularVelocities; d++ )
	  {
	    const int extraEqn = numberOfDimensions + d + (b)*numberOfExtraEquationsPerBody; // current extra equation 
	    int ival = totalNumberOfExtraEquations -extraEqn -1- numberOfDenseExtraEquations;        // equations are stored in reverse order
	    assert( ival<numberOfExtraEquations );
	    int dir = numberOfDimensions==2 ? 2 : d;                    // In 2D we use component 2 of the angular acceleration
	    omegaDot(dir) = constraintValues[ival];
	  }
	
	  body.setAcceleration( t,vDot,omegaDot );

	}
      }

      delete [] constraintValues;
    }
    

  }
  

  // if( myid==0 ) printf(" ** after pressure solve 3\n"); 

  if( poisson->getCompatibilityConstraint() )
  {
    // The solver may have trouble satisfying the compatability constraint (true for yale and ins/annulus.tz)
    // so we explicitly enforce it here by just shifting the solution by a constant 
    // *** note that we over-write the value of the constraint, p[gride](i1e,i2e,i3e)
    if( true )
    {
      real nullVectorDotP=0., sumOfNullVector=0.;
      poisson->evaluateExtraEquation(p(),nullVectorDotP,sumOfNullVector);    
      //   "   ***not enforcing compatibility constraint for parallel *** FIX THIS *** \n",nullVectorDotP);

      real constraintValue;
      poisson->getExtraEquationValues(pressureRightHandSide, &constraintValue );

      if( debug() & 4 )
      {
	printF("solveForTimeIndepVarsINS INFO: nullVectorDotP=%14.9g, "
	       "constraintValue=%g, diff=%g,  sumOfNullVector=%g \n",
	       nullVectorDotP,constraintValue,fabs(nullVectorDotP-constraintValue),sumOfNullVector);
      }
      
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
	realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
	bool ok = ParallelUtility::getLocalArrayBounds(p()[grid],pLocal,I1,I2,I3);
	if( !ok ) continue;
      
	pLocal+=(constraintValue-nullVectorDotP)/(max(1.,sumOfNullVector));
      }
    }
    else
    {
      // old way
      real nullVectorDotP=0., sumOfNullVector=0.;
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3);
	nullVectorDotP+=sum(poisson->rightNullVector[grid](I1,I2,I3)*p()[grid](I1,I2,I3));
	sumOfNullVector+=sum(poisson->rightNullVector[grid](I1,I2,I3));
      }
      int ne,i1e,i2e,i3e,gride;
      poisson->equationToIndex( poisson->extraEquationNumber(0),ne,i1e,i2e,i3e,gride);

      if( debug() & 2 )
      {
	real diff=nullVectorDotP-pressureRightHandSide[gride](i1e,i2e,i3e);
	if( myid==0 )
	  fprintf(debugFile,"After solve: compatibility sum(null*p)= %14.10e,  nullVectorDotP-rhs=%e \n",
		  nullVectorDotP,diff);
      }
    
      p()+=(pressureRightHandSide[gride](i1e,i2e,i3e)-nullVectorDotP)/(max(1.,sumOfNullVector));
    }

  }

}

