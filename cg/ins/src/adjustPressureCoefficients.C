#include "Cgins.h"

#include "Parameters.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "SparseRep.h"
#include "Oges.h"
#include "DeformingBodyMotion.h"
#include "BeamFluidInterfaceData.h"
#include "Integrate.h"
#include "RigidBodyMotion.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))


#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

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

#define FOR_3IJD(i1,i2,i3,I1,I2,I3,j1,j2,j3,J1,J2,J3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase();  \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


// =======================================================================
// indexToEquation( n,i1,i2,i3 ) : defines the global index for each unknown in the system
//     n=component number (uc,vc,...) 
//    (i1,i2,i3) = grid point 
// =======================================================================
#define indexToEquation( n,i1,i2,i3 ) (n+1+ \
 numberOfComponentsForCoefficients*(i1-equationNumberBase1+\
             equationNumberLength1*(i2-equationNumberBase2+\
             equationNumberLength2*(i3-equationNumberBase3))) + equationOffset)

// =======================================================================
// =======================================================================
#define setEquationNumber(m, ni,i1,i2,i3,  nj,j1,j2,j3 )\
          equationNumber(m,i1,i2,i3)=indexToEquation( nj,j1,j2,j3)

// =======================================================================
// =======================================================================
#define setClassify(n,i1,i2,i3, type) \
    classify(i1,i2,i3,n)=type

// =======================================================================
//  Macro to zero out the matrix coefficients for equations e1,e1+1,..,e2
// =======================================================================
#define zeroMatrixCoefficients( coeff,e1,e2, i1,i2,i3 )\
for( int m=CE(0,e1); m<=CE(0,e2+1)-1; m++ ) \
  coeff(m,i1,i2,i3)=0.



// =================================================================================================
/// \brief Make adjustments to the pressure coefficient matrix (e.g. for the added mass algorithm).
// ==================================================================================================
void Cgins::
adjustPressureCoefficients(CompositeGrid & cg0, GridFunction & cgf  )
{

  const bool & useAddedMassAlgorithm = parameters.dbase.get<bool>("useAddedMassAlgorithm");
  const bool & useAddedDampingAlgorithm = parameters.dbase.get<bool>("useAddedDampingAlgorithm");

  if( !useAddedMassAlgorithm || !parameters.isMovingGridProblem() )
    return;

  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Jbv[3], &Jb1=Jbv[0], &Jb2=Jbv[1], &Jb3=Jbv[2];
  int i1,i2,i3, j1,j2,j3, i1m,i2m,i3m, j1m,j2m,j3m, m1,m2,m3;
  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  int jsv[3], &js1=jsv[0], &js2=jsv[1], &js3=jsv[2];

  CompositeGridOperators & cgop = *cgf.u.getOperators();

  const int & pc = parameters.dbase.get<int >("pc");
  
  MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
  const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();

  const real & dt=parameters.dbase.get<real>("dt");
  if( numberOfDeformingBodies>0 && cgf.t<3.*dt )
    printF("--INS-- adjustPressureCoefficients for two-sided beams (if any), t=%9.3e\n",cgf.t);


  if( numberOfDeformingBodies>0 )
  {
    // ======================================================================
    // =============== DEFORMING BODY AMP SCHEME ============================
    // ======================================================================
    
    // -- construct the beam-fluid interface data needed for the AMP scheme for two-sided beams  --
    // *** THIS ONLY NEEDS TO BE DONE ONCE ***
    for( int body=0; body<numberOfDeformingBodies; body++ )
    {
      DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
      deformingBody.buildBeamFluidInterfaceData( cg0 );
    }

    realCompositeGridFunction & coeff = poisson->coeff;

    if( true )   // *new* way
    {
      if( cgf.t<3.*dt )
	printF("--INS-- *NEW WAY* adjustPressureCoefficients for two-sided beams (if any), t=%9.3e\n",cgf.t);

      // -- NEW WAY --
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      const int numberOfComponentGrids = cg0.numberOfComponentGrids();
      const int numberOfDimensions = cg0.numberOfDimensions();
      for( int body=0; body<numberOfDeformingBodies; body++ )
      {
	DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	if( !deformingBody.beamModelHasFluidOnTwoSides() )
	{ // this is NOT a beam model with fluid on two sides.
	  continue;   
	}

	DataBase & deformingBodyDataBase = deformingBody.deformingBodyDataBase;
	const int & numberOfFaces = deformingBodyDataBase.get<int>("numberOfFaces");
	const IntegerArray & boundaryFaces = deformingBodyDataBase.get<IntegerArray>("boundaryFaces");

	BeamFluidInterfaceData &  beamFluidInterfaceData = 
	  deformingBodyDataBase.get<BeamFluidInterfaceData>("beamFluidInterfaceData");
	IntegerArray *& donorInfoArray = beamFluidInterfaceData.dbase.get<IntegerArray*>("donorInfoArray");

	for( int face=0; face<numberOfFaces; face++ )
	{
	  const int side0=boundaryFaces(0,face);
	  const int axis0=boundaryFaces(1,face);
	  const int grid0=boundaryFaces(2,face); 
    
	  realMappedGridFunction & coeff0 = coeff[grid0];
    
	  assert( coeff0.sparse!=NULL );
	  SparseRepForMGF & sparse0 = *coeff0.sparse;
	  const int equationOffset0=sparse0.equationOffset;
	  const int numberOfComponentsForCoefficients0 = sparse0.numberOfComponents;  // size of the system of equations
	  const int numberOfGhostLines0 = sparse0.numberOfGhostLines;
	  const int stencilSize0 = sparse0.stencilSize;
	  const int stencilDim0=stencilSize0*numberOfComponentsForCoefficients0; // number of coefficients per equation

	  intArray & equationNumber0 = sparse0.equationNumber;
	  intArray & classify0 = sparse0.classify;

	  MappedGrid & mg0 = cg0[grid0];
	  const IntegerArray & gid0 = mg0.gridIndexRange();

	  for( int axis=0; axis<3; axis++ ){ iv[axis]=gid0(0,axis); } //

	  const int axisp1= (axis0 +1) % numberOfDimensions;

	  const IntegerArray & donorInfo= donorInfoArray[face]; 
	  Range I0=donorInfo.dimension(0);
	  for( int i=I0.getBase(); i<=I0.getBound(); i++ )  // NOTE: loop index i is incremented below
	  {
	    // Here is the donor on the opposite face of the beam:
	    const int grid1 = donorInfo(i,0), side1=donorInfo(i,1), axis1=donorInfo(i,2);

	    if( grid1<0 )  // This means there is no opposite grid point -- could be the end of the beam
	      continue;

	    assert( grid1>=0 && grid1<numberOfComponentGrids );

	    realMappedGridFunction & coeff1 = coeff[grid1];
	    assert( coeff1.sparse!=NULL );
	    SparseRepForMGF & sparse1 = *coeff1.sparse;
	    const int equationOffset1=sparse1.equationOffset;
	    intArray & equationNumber1 = sparse1.equationNumber;
	    intArray & classify1 = sparse1.classify;

	    MappedGrid & mg1 = cg0[grid1];

	    // loop over points with the same donor grid
	    for( ; i<=I0.getBound(); i++ ) // NOTE: this increments "i" from the outer loop
	    {
	      const int donor = donorInfo(i,0);
	      if( donor <0 )  // This means there is do opposite grid point -- could be the end of the beam
		continue;

	      if( donor!=grid1 )
	      {
		i--;
		break;
	      }
	    
	      iv[axisp1]=i+gid0(0,axisp1); // index that varies along the interface of grid0

	      is1=is2=is3=0;
	      isv[axis0]=1-2*side0;

	      js1=js2=js3=0;
	      jsv[axis1]=1-2*side1;

	      // closest grid pt on opposite side:
	      const int j1=donorInfo(i,3), j2=donorInfo(i,4), j3=donorInfo(i,5); 
	    
	      if( grid0==grid1 && i1==j1 && i2==j2 && i3==j3)
	      {
		OV_ABORT("ERROR - donor = source point for AMP pressure BC!");
	      }
	    

	      i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)
	      j1m=j1-js1, j2m=j2-js2, j3m=j3-js3; //  ghost point is (j1m,j2m,j3m)
	      // coeff(mm,i1m,i2m,i3m)
	      // add the extra equations:
	      //   p0 + (rhos*hs/rho)*p0.n  - p1 =     (add -p1 to this Robin BC)
	      //   p1 + (rhos*hs/rho)*p1.n  - p0 =     (add -p0 to this Robin BC)

	      int me = stencilDim0-1;  // "extra" equation goes here at end of the coefficients
	      int md=4;  // hard code for now -- this should "diagonal" entry

	      // add a "-1" coefficient to the AMP Robin BC on grid0
	      assert( coeff0(me,i1m,i2m,i3m)==0. );
	      // if( i<13 || i>15 )
	      coeff0(me,i1m,i2m,i3m)=-1.;
	      equationNumber0(me,i1m,i2m,i3m)=equationNumber1(md,j1m,j2m,j3m);  // -1 multiplies eqn1
	    
	      // // add a "-1" coefficient to the AMP Robin BC on grid1
	      // assert( coeff1(me,j1m,j2m,j3m)==0. );
	      // coeff1(me,j1m,j2m,j3m)=-1.;
	      // equationNumber1(me,j1m,j2m,j3m)=equationNumber0(md,i1m,i2m,i3m);

	    }
	  } // end for i 
	} // end for face
      } // end for body
    } // end if   
    else // **OLD WAY**
    {
      
      std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
      // first locate any boundaries that lie on the beam
      const int maxFaces=10;  // fix me 
      int sideBody[maxFaces], axisBody[maxFaces], gridBody[maxFaces]; 
      int numFaces=0;
      for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
      {
	if( parameters.gridIsMoving(grid) )
	{
	  BoundaryData & bd = boundaryDataArray[grid];

	  if( bd.dbase.has_key("deformingBodyNumber") )
	  {
	    // --- this grid has a face attached to a deforming body ---

	    int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");
	    for( int side=0; side<=1; side++ )
	    {
	      for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	      {
		if( deformingBodyNumber[side][axis]>=0 )
		{
		  int body=deformingBodyNumber[side][axis];
		  gridBody[numFaces]=grid;
		  sideBody[numFaces]=side;
		  axisBody[numFaces]=axis;
		  numFaces++;
		  if( debug()& 4 || cgf.t <= 0. )
		    printF("--APC-- body=%i  has face (grid,side,axis)=(%i,%i,%i)\n",body,grid,side,axis);
		}
	    

	      }
	    }
	  }
      
	}
      }

      if( numFaces>1 )  // There are multiple grids adjacent to the beam
      {
	assert( numFaces==2 ); // do this case for now
    
	int grid0 = gridBody[0], side0=sideBody[0], axis0=axisBody[0];
	int grid1 = gridBody[1], side1=sideBody[1], axis1=axisBody[1];
    
	realMappedGridFunction & coeff0 = coeff[grid0];
	realMappedGridFunction & coeff1 = coeff[grid1];
    
	assert( coeff0.sparse!=NULL );
	SparseRepForMGF & sparse0 = *coeff0.sparse;
	const int equationOffset0=sparse0.equationOffset;
	const int numberOfComponentsForCoefficients0 = sparse0.numberOfComponents;  // size of the system of equations
	const int numberOfGhostLines0 = sparse0.numberOfGhostLines;
	const int stencilSize0 = sparse0.stencilSize;
	const int stencilDim0=stencilSize0*numberOfComponentsForCoefficients0; // number of coefficients per equation

	intArray & equationNumber0 = sparse0.equationNumber;
	intArray & classify0 = sparse0.classify;
    
	MappedGrid & mg0 = cg0[grid0];
	OV_GET_SERIAL_ARRAY_CONST(real,mg0.center(),xy0Local);
	getBoundaryIndex(mg0.gridIndexRange(),side0,axis0,Ib1,Ib2,Ib3); // boundary index's for mg0

	assert( coeff1.sparse!=NULL );
	SparseRepForMGF & sparse1 = *coeff1.sparse;
	const int equationOffset1=sparse1.equationOffset;
	intArray & equationNumber1 = sparse1.equationNumber;
	intArray & classify1 = sparse1.classify;

	MappedGrid & mg1 = cg0[grid1];
	OV_GET_SERIAL_ARRAY_CONST(real,mg1.center(),xy1Local);

	getBoundaryIndex(mg1.gridIndexRange(),side1,axis1,Jb1,Jb2,Jb3);  // boundary index's for mg1 

	is1=is2=is3=0;
	isv[axis0]=1-2*side0;

	js1=js2=js3=0;
	jsv[axis1]=1-2*side1;

	Range all;

	bool gridPointsMatch = true;
	if( gridPointsMatch )
	{
	  // --- CASE I: Separate grids and grid points match on both sides of the beam ----
	  FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Jb1,Jb2,Jb3) // loop over points on the boundary 
	  {
	    i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)
	    j1m=j1-js1, j2m=j2-js2, j3m=j3-js3; //  ghost point is (j1m,j2m,j3m)
	    // coeff(mm,i1m,i2m,i3m)
	    // add the extra equations:
	    //   p0 + (rhos*hs/rho)*p0.n  - p1 =     (add -p1 to this Robin BC)
	    //   p1 + (rhos*hs/rho)*p1.n  - p0 =     (add -p0 to this Robin BC)

	    int me = stencilDim0-1;  // "extra" equation goes here at end of the coefficients
	    int md=4;  // hard code for now -- this should "diagonal" entry

	    // add a "-1" coefficient to the AMP Robin BC on grid0
	    assert( coeff0(me,i1m,i2m,i3m)==0. );
	    coeff0(me,i1m,i2m,i3m)=-1.;
	    equationNumber0(me,i1m,i2m,i3m)=equationNumber1(md,j1m,j2m,j3m);  // -1 multiplies eqn1

	    // add a "-1" coefficient to the AMP Robin BC on grid1
	    assert( coeff1(me,j1m,j2m,j3m)==0. );
	    coeff1(me,j1m,j2m,j3m)=-1.;
	    equationNumber1(me,j1m,j2m,j3m)=equationNumber0(md,i1m,i2m,i3m);
      
	    if( debug() & 8 && cgf.t <= 0. )
	    {
	      printF(" grid0=%i (i1m,i2m)=(%i,%i) x=(%9.3e,%9.3e)\n",grid0,i1m,i2m,xy0Local(i1m,i2m,i3m,0),xy0Local(i1m,i2m,i3m,1));
	      printF(" grid1=%i (j1m,j2m)=(%i,%i) x=(%9.3e,%9.3e)\n",grid1,j1m,j2m,xy1Local(j1m,j2m,j3m,0),xy1Local(j1m,j2m,j3m,1));
      
	      ::display(coeff0(all,i1m,i2m,i3m),sPrintF("coeff on ghost point (%i,%i) of grid=%i",i1m,i2m,grid0),"%8.2e ");
	      ::display(equationNumber0(all,i1m,i2m,i3m),sPrintF("equationNumber on ghost point (%i,%i) of grid=%i",i1m,i2m,grid0),"%8i ");

	      ::display(coeff1(all,j1m,j2m,j3m),sPrintF("coeff on ghost point (%i,%i) of grid=%i",j1m,j2m,grid1),"%8.2e ");
	      ::display(equationNumber1(all,j1m,j2m,j3m),sPrintF("equationNumber on ghost point (%i,%i) of grid=%i",j1m,j2m,grid1),"%8i ");
	    }
      

	  }
    
	}
    
      }
    }
  
  } // end deforming bodies
  
  const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();

  if( numberOfRigidBodies>0 )
  {
    // ======================================================================
    // =================== RIGID BODY AMP SCHEME ============================
    // ======================================================================
    
    const real & t = cgf.t;
    printF("--INS:APC-- rigidy body AMP scheme -- t=%9.3e: adjust pressure BC and add direct projection equations\n",t);
    
    assert( poisson!=NULL );
    Oges & pSolver = *poisson;

    // sparse matrix coefficients are stored here 
    realCompositeGridFunction & coeff = poisson->coeff;

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
    const int numberOfExtraEquations = numberOfRigidBodies*numberOfExtraEquationsPerBody;

    // There may be one "dense" constraint equation setting the mean of the pressure: 
    int numberOfDenseExtraEquations=0;
    if( pSolver.getCompatibilityConstraint() )
    {
      numberOfDenseExtraEquations=1;// constraint setting mean-value of p
    }
    
    const int totalNumberOfExtraEquations=numberOfDenseExtraEquations+numberOfExtraEquations;
    pSolver.setNumberOfExtraEquations(totalNumberOfExtraEquations);
    printF("--APC-- numberOfDenseExtraEquations=%i, totalNumberOfExtraEquations=%i\n",
           numberOfDenseExtraEquations,totalNumberOfExtraEquations);

    // --- there are user defined extra equations ----
    pSolver.set(OgesParameters::THEuserSuppliedEquations,true);

    pSolver.initialize(); // ---- Need to call initialize now ********** FIX ME **********
    ::display(pSolver.extraEquationNumber,"pSolver.extraEquationNumber");
    assert( pSolver.extraEquationNumber.getBound(0)==totalNumberOfExtraEquations-1 );

    Index Ib1,Ib2,Ib3;
    Index Ig1,Ig2,Ig3;




    // integrate: holds the integration weights and 
    //   also the info on which grid faces are adjacent to the body (should fix this)
    Integrate *integrate = movingGrids.getIntegrate();  
    assert( integrate!=NULL );

    // -------------------------------------------------------------------------
    // --- Stage I: count the number of surface points on the rigid bodies:-----
    // --- For each rigid body we count up the number of grid points on the body surface ---
    // --- so that we can compute the number of nonzeros in the constraint equations---
    // -------------------------------------------------------------------------

    int totalNumberOfSurfacePoints=0;  // counts number of grid points on the body surfaces.
    // --------------- LOOP OVER RIGID BODIES --------------------
    for( int b=0; b<numberOfRigidBodies; b++ )
    {
      RigidBodyMotion & body = movingGrids.getRigidBody(b);

      const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
      // --------------- LOOP OVER FACES OF THE RIGID BODY --------------------
      for( int face=0; face<numberOfFaces; face++ )
      {
	int side=-1,axis,grid;
	integrate->getFace(b,face,side,axis,grid);
	assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	assert( grid>=0 && grid<cg.numberOfComponentGrids());


	MappedGrid & mg = cg0[grid];
        const IntegerArray & gid = mg.gridIndexRange();
        int extra=1; // include the corner. is this needed??
	getBoundaryIndex(gid,side,axis,Ib1,Ib2,Ib3,extra);
	
        int numSurfacePoints = Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	
	printF("--APC--- rigidBody %i: face=%i (side,axis,grid)=(%i,%i,%i) numSurfacePoints=%i\n",b,face,side,axis,grid,numSurfacePoints);

        totalNumberOfSurfacePoints+= numSurfacePoints;

      }
    }

    // -----------------------------
    // --- add up the total number of nonzeros: 
    int numberOfNonzerosInConstraint = totalNumberOfSurfacePoints*numberOfExtraEquations; // surface integrals 
    if( numberOfDimensions==2 )
    {
      // mb*a1, mb*a2, A33*b3 terms: 
      numberOfNonzerosInConstraint += numberOfExtraEquations;  
    }
    else
    {
      // mb*a1, mb*a2, mb*a3, A*bv terms 
      numberOfNonzerosInConstraint += numberOfRigidBodies*(  numberOfDimensions + SQR(numberOfDimensions));
    }
    
    // ------------------------------------------------------------------
    // ---- Stage II:  FILL-IN CONSTRAINT EQUATIONS --------------------
    // ------------------------------------------------------------------

    // Extra equations are stored in compressed-row storage (CSR) format 
    IntegerArray equation(numberOfExtraEquations), ia(numberOfExtraEquations+1), ja(numberOfNonzerosInConstraint);
    RealArray a(numberOfNonzerosInConstraint);


    // -- integration weights are here (surface weights are stored in the ghost points): 
    RealCompositeGridFunction & weights = integrate->integrationWeights();

    int nnz=0;      // running count of the non-zeros in extra equations
    // --------------- LOOP OVER RIGID BODIES --------------------
    for( int b=0; b<numberOfRigidBodies; b++ )
    {
      RigidBodyMotion & body = movingGrids.getRigidBody(b);

      const real massBody = body.getMass(); // mass of the rigid-body

      RealArray inertiaTensor(3,3); // moment-of-inertial matrix of the rigid-body
      body.getMomentOfInertiaTensor( t, inertiaTensor);
      if( false )
      {
	::display(inertiaTensor,"--APC-- inertiaTensor");
      }

      RealArray xb(3);  // x-location of the center of mass of the body 
      body.getPosition( t,xb );
      
      RealArray addedDampingTensors(3,3,2,2);  // holds added damping Tensors - 4 3x3 matrices 
      if( useAddedDampingAlgorithm )
      {
	movingGrids.getRigidBodyAddedDampingTensors( b, addedDampingTensors, cgf, dt );
      }
      

      // integrate: holds the info on which grid faces are adjacent to the body (should fix this)
      Integrate *integrate = movingGrids.getIntegrate();  
      assert( integrate!=NULL );

      // const bool useAddedMass = body.useAddedMass(); // true when using added-mass matrices

      // -------------------------------------------------------------------------------------
      // vbType=0 : RB linear velocity constraints
      //       =1 : RB angular velocity constraints
      // 
      // --- 2D: AMP constraint equations are:
      //    mb*a1 + INT p n_1 ds = ...
      //    mb*a2 + INT p n_2 ds = ...
      //    A(3,3)*b3 + INT n_1 r_2 - n_2 r_1 ds = ...    
      //
      // --- 3D: AMP constraint equations are: ( e1=[1 0 0],  e2=[0 1 0], e3= [ 0 0 1] )
      //    mb*a1 + INT p n_1 ds = ...
      //    mb*a2 + INT p n_2 ds = ...
      //    mb*a3 + INT p n_3 ds = ...
      //    A(1,1)*b1 + A(1,2)*b2 + A(1,3)*b3 + INT e1 . rv X (p nv) ds = ...
      //    A(2,1)*b1 + A(2,2)*b2 + A(2,3)*b3 + INT e2 . rv X (p nv) ds = ... 
      //    A(3,1)*b1 + A(3,2)*b2 + A(3,3)*b3 + INT e3 . rv X (p nv) ds = ...
      //
      // -------------------------------------------------------------------------------------
      for( int vbType=0; vbType<=1; vbType++ )
      {
	printF("--APC: body b=%i, vbType=%i , massBody=%9.3e\n",b,vbType,massBody);

        //  numDim = numberOfDimensions in 3D or in 2D 
        //         = 1 for 2D angular velocity
        const int numDim = (vbType==0 || numberOfDimensions==3) ? numberOfDimensions : 1;
	for( int dir=0; dir<numDim; dir++ )
	{
	  const int extraEqn = dir + numberOfDimensions*(vbType); // current extra equation 

	  // solver.extraEquationNumber : NOTE extra equations are stored in reverse order:
          //                             last extra eqn is optional dense constraint. 
          // ieqn : fill in this extra equation 
	  
          const int jeqn=totalNumberOfExtraEquations - extraEqn -1;
	  const int ieqn = pSolver.extraEquationNumber(jeqn);

          // printF("--APC-- extraEqn=%i, ieqn=%i, jeqn=%i, totalNumberOfExtraEquations=%i numberOfDenseExtraEquations=%i\n",
	  //    extraEqn,ieqn,jeqn,totalNumberOfExtraEquations,numberOfDenseExtraEquations);

	  equation(extraEqn)=ieqn;  // Constraint equation is located here in the sparse matrix
	  ia(extraEqn)=nnz;         // This extra equation (extraEqn) starts at nnz in (ja,a)
	  if( vbType==0 )
	  {
            // Body linear velocity: 
  	    ja(nnz)=ieqn;    // diagonal entry in the matrix 
  	    a(nnz)=massBody;  
	  }
	  else
	  {
	    // body angular velocity
	    if( numberOfDimensions==2 )
	    {
	      ja(nnz)=ieqn;     // diagonal entry in the matrix 
	      a(nnz)=inertiaTensor(2,2); 
	      if( useAddedDampingAlgorithm )
	      {
                const int vbc=0, wbc=1; // component numbers of v and omega in addedDampingTensors

                const real Dww = addedDampingTensors(2,2,wbc,wbc);  // coeff of the angular velocity in the omega_t eqn
                real impFactor=1.;
                printF("--APC-- addedDamping coeff: Dww=%8.2e, dt=%8.2e, inertiaTensor(2,2)=%8.2e %3.1f*dt*Dww=%8.2e \n",
                       Dww,dt,inertiaTensor(2,2),impFactor,impFactor*dt*Dww  );
		a(nnz) += impFactor*dt*Dww;
	      }
	      
	    }
	    else
	    {
              // *** FINISH ME***
              OV_ABORT("finish me");
	    }
	    
	  } 
          nnz++;

	  const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
          // --------------- LOOP OVER FACES OF THE RIGID BODY --------------------
	  for( int face=0; face<numberOfFaces; face++ )
	  {
	    int side=-1,axis,grid;
	    integrate->getFace(b,face,side,axis,grid);
	    assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	    assert( grid>=0 && grid<cg.numberOfComponentGrids());

	    printF("--APC: body b=%i, vbType=%i dir=%i face=%i\n",b,vbType,dir,face);
	    
	    MappedGrid & mg = cg0[grid];
	    const IntegerArray & gid = mg.gridIndexRange();
	
	    mg.update(MappedGrid::THEvertexBoundaryNormal);
            #ifdef USE_PPP
	      const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            #else
	      const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
            #endif

	    int extra=1; // include the corner. is this needed??
	    getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra); // boundary points
	    getGhostIndex(cg[grid].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,extra);    // ghost points

	    // int includeGhost=0;  // do NOT include parallel ghost
	    // ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,Ib1,Ib2,Ib3,includeGhost);
	    // ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,Ig1,Ig2,Ig3,includeGhost);
	    // if( !ok ) continue;
	    
	    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
	    OV_GET_SERIAL_ARRAY(real,weights[grid],weightsLocal);
	    if( false && vbType==0 && dir==0 && face==0 )
	    {
	      ::display(weightsLocal(Ig1,Ig2,Ig3),"--APC-- Integration weights");
	    }
	

	    //  integral(n)+=sum(uLocal(Ib1,Ib2,Ib3,n)*weightsLocal(I1,I2,I3));

	    FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Ig1,Ig2,Ig3)
	    {
    	      const int n=0; // component number
              // jeqn : equation number in sparse matrix for boundary pt (i1,i2,i3) on grid 
	      const int jeqn = pSolver.equationNo( n,i1,i2,i3,grid ); 

              // Note: surface weights are stored in the ghost points: weightsLocal(j1,j2,j3)
              assert( nnz<numberOfNonzerosInConstraint );
	      ja(nnz)=jeqn;
	      if( vbType==0 )
	      {
                // -- for RB linear velocity the coeff of p is the weight*normal
  	        a(nnz)= - weightsLocal(j1,j2,j3)*normal(i1,i2,i3,dir);  
	      }
	      else
	      {
                // -- for angular velocity the coeff of p is
                //       - (x-xb) X nv 
		real rv[3]; // holds x-xb
   	        for( int d=0; d<numberOfDimensions; d++ ){  rv[d]=xLocal(i1,i2,i3,d)-xb(d); } //             
		if( numberOfDimensions==2 )
		{
                  // This looks correct (from ellipse case)
		  a(nnz)= - weightsLocal(j1,j2,j3)*(normal(i1,i2,i3,1)*rv[0]-normal(i1,i2,i3,0)*rv[1]);
		}
		else
		{
                  const int dirp1 = (dir+1) % numberOfDimensions;
     		  const int dirp2 = (dir+2) % numberOfDimensions;
                  // *CHECK ME*
                  a(nnz)= - weightsLocal(j1,j2,j3)*(normal(i1,i2,i3,dirp2)*rv[dirp1]-normal(i1,i2,i3,dirp1)*rv[dirp2]);

                  OV_ABORT("finish me");
		}
	      }
	      
	      nnz++;
	    }
	  }

	} // end for dir 
      } // end for vbType 
      
    } // end for body b
    ia(numberOfExtraEquations)=nnz; 
    
    if( false )
    {
      printF("--APC-- nnz=%i, numberOfNonzerosInConstraint=%i\n",nnz,numberOfNonzerosInConstraint);
      printF("--APC-- constraint equations in CSR format:\n");
      for( int i=0; i<numberOfExtraEquations; i++ )
      {
	printF(" eqn=%i, i=%i: ",equation(i),i);
	for( int j=ia(i); j<ia(i+1); j++ )
	  printF(" (j=%i,%8.2e)",ja(j),a(j));
	printF("\n");
      }
    }
    
    // Tell Oges about the extra equations 
    pSolver.setEquations( numberOfExtraEquations, equation,ia,ja,a );




    // -------------------------------------------------------------------------
    // --- Stage III: Adjust the pressure Neumann BC
    // 
    // Direct projection equations for a rigid body (RB) :
    //      p.n = -rho n.[  av + bv X ( xv - xb ) ] + stuff 
    //
    //  mb av = INT_{body surface} [  - p(s) nv(s) ds ]
    //  Mb bv = INT_{body surface} [ ( xv - xb ) X ( -p nv ) ] ds
    // 
    //     av = [a1 a2 a3 ] = RB linear acceleration
    //     bv = [b1 b2 b3 ] = RB angular acceleration
    // -------------------------------------------------------------------------


    const real & fluidDensity = parameters.dbase.get<real >("fluidDensity");
    assert( fluidDensity>0. );

    // --------------- LOOP OVER RIGID BODIES --------------------
    for( int b=0; b<numberOfRigidBodies; b++ )
    {
      RigidBodyMotion & body = movingGrids.getRigidBody(b);
      RealArray xb(3);  // x-location of the center of mass of the body 
      body.getPosition( t,xb );

      const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
      // --------------- LOOP OVER FACES OF THE RIGID BODY --------------------
      for( int face=0; face<numberOfFaces; face++ )
      {
	int side=-1,axis,grid;
	integrate->getFace(b,face,side,axis,grid);
	assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	assert( grid>=0 && grid<cg.numberOfComponentGrids());

	MappedGrid & mg = cg0[grid];

        OV_GET_SERIAL_ARRAY(int,mg.mask(),maskLocal);
        OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
        #ifdef USE_PPP
	  const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
        #else
	  const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
        #endif

	realMappedGridFunction & coeff0 = coeff[grid];
    
	assert( coeff0.sparse!=NULL );
	SparseRepForMGF & sparse = *coeff0.sparse;
	intArray & equationNumber = sparse.equationNumber;
	intArray & classify = sparse.classify;
        OV_GET_SERIAL_ARRAY(int,classify,classifyLocal);

        const IntegerArray & gid = mg.gridIndexRange();
	getBoundaryIndex(gid,side,axis,Ib1,Ib2,Ib3);                       // boundary points
	getGhostIndex(cg[grid].gridIndexRange(),side,axis,Ig1,Ig2,Ig3);    // ghost points

        // The stencil coefficients for p.n should be zero in the "corners" -- put the extra coefficients there 
        //          6 7 8   15 16 17    24 25 26
        //          3 4 5   12 13 14    21 22 23
        //          0 1 2    9 10 11    18 19 20
	int emptySpot[] = { 0,2,6,8, 9,11, 15, 17, 18, 19, 20, 21, 23, 24, 25, 26 };  // there are a few more in 3D
	FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Ig1,Ig2,Ig3)
	{
	  // (i1,i2,i3) : boundary pt
          // (j1,j2,j3) : ghost pt

          // classify: -2=periodic, -1=interpolation, 1=boundary, 2=interior
          bool neumannBC = classify(i1,i2,i3)==SparseRepForMGF::boundary || classify(i1,i2,i3)==SparseRepForMGF::interior;
	  if( !neumannBC )
	  {
            // -- skip this point : could be a ghost outside of periodic, interpolation or unused ---
	    continue;
	  }
	  

	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
  	    // --- Add rho nv.av  terms ---

            int extraEqn = dir + b*( numberOfExtraEquationsPerBody ); //  a-equation
            // ieqn = location in sparse matrix of extraEqn 
            int ieqn = pSolver.extraEquationNumber(totalNumberOfExtraEquations - extraEqn -1);
            // The coefficients for p.n should be zero in the "corners" -- put the extra coefficients there 
            int me = emptySpot[extraEqn]; // put the coefficient here in the stencil
	     
	    if(  coeff0(me,j1,j2,j3)!=0. )
	    {
	      printF("ERROR: me=%i (j1,j2,j3)=(%i,%i,%i) coeff=%9.2e mask(i1,i2,i3)=%i classify(i1,i2,i3)=%i classify(j1,j2,j3)=%i \n",
		     me,j1,j2,j3,coeff0(me,j1,j2,j3),maskLocal(i1,i2,i3),classify(i1,i2,i3),classify(j1,j2,j3));
	    }
	    
	    assert( coeff0(me,j1,j2,j3)==0. );
	    coeff0(me,j1,j2,j3)=fluidDensity*normal(i1,i2,i3,dir);
            // we have changed the equation number 
	    equationNumber(me,j1,j2,j3)=ieqn;   // we have set the coeff of a[dir]

            // --- add bv terms ---
            //     bv X ( xv - xb ) = cv . bv 
            //     c_i = n[i+2]*r[i+1] - nv[i+1]*rv[i+2]   i=1,2,3
            extraEqn = numberOfDimensions + dir + b*( numberOfExtraEquationsPerBody ); //  b-equation
            ieqn = pSolver.extraEquationNumber(totalNumberOfExtraEquations - extraEqn -1);

            me = emptySpot[extraEqn]; // put the coefficient here in the stencil
	    assert( coeff0(me,j1,j2,j3)==0. );
	    
	    real rv[3]; // holds x - xb
	    for( int d=0; d<numberOfDimensions; d++ ){  rv[d]=xLocal(i1,i2,i3,d)-xb(d); } //             
            if( numberOfDimensions==2 )
	    {
	      if( dir==0 ) // we only keep component b3 of the angular acceleration in 2D 
	      {
		coeff0(me,j1,j2,j3)=fluidDensity*( normal(i1,i2,i3,1)*rv[0]-normal(i1,i2,i3,0)*rv[1] ); // coeff of b3
		equationNumber(me,j1,j2,j3)=ieqn;   
	      }
	      
	    }
	    else
	    { // In 3D there are 3 components of the angular acceleration
              // 
	      const int dirp1 = (dir+1) % numberOfDimensions;
	      const int dirp2 = (dir+2) % numberOfDimensions;
	      coeff0(me,j1,j2,j3)=fluidDensity*( normal(i1,i2,i3,dirp2)*rv[dirp1]-normal(i1,i2,i3,dirp1)*rv[dirp2] );
              equationNumber(me,j1,j2,j3)=ieqn;   // we have set the coeff of b[dir]  
	    }
	    

	  }
	  
	}
	

      }
    }


  } // end rigid bodies
  


  

  if( false )
  {
    coeff.display("coeff");
    for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    {
      const intMappedGridFunction & classify = coeff[grid].sparse->classify;
      ::display(classify,"classify");
    }
  }

}
