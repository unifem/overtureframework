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

#include "BeamModel.h"
// ==================================================================================================
/// \brief Return the equation number in the sparse matrix of a particular "extra equation"
// ==================================================================================================
int getSparseMatrixEquationNumberFromExtraEquationNumber( int extraEquationNumber, int totalNumberOfExtraEquations, 
                                                          Oges & pSolver)
{
  // NOTE extra equations are stored in reverse order:
  int eqn = totalNumberOfExtraEquations-1 -extraEquationNumber; 
  return pSolver.extraEquationNumber(eqn);
}


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
// #define setClassify(n,i1,i2,i3, type) \
//     classify(i1,i2,i3,n)=type

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
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  int i1,i2,i3, j1,j2,j3, i1m,i2m,i3m, j1m,j2m,j3m, m1,m2,m3;
  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  int jsv[3], &js1=jsv[0], &js2=jsv[1], &js3=jsv[2];

  CompositeGridOperators & cgop = *cgf.u.getOperators();

  const int & pc = parameters.dbase.get<int >("pc");
  
  MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
  const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();

  const real & dt=parameters.dbase.get<real>("dt");
  if( numberOfDeformingBodies>0 && cgf.t<3.*dt )
    printF("--INS-- adjustPressureCoefficients for AMP schemes, t=%9.3e\n",cgf.t);


  if( numberOfDeformingBodies>0 )
  {
    // ======================================================================
    // =============== DEFORMING BODY AMP SCHEME ============================
    // ======================================================================
    
    // --- Look for any bodies that are BEAMS ----
    // --- construct the beam-fluid interface data needed for the AMP scheme for two-sided beams  --

    // *** THIS ONLY NEEDS TO BE DONE ONCE ***
    // Longfei 20170119: moved to DeformingBodyMotion::initialize()
    // for( int body=0; body<numberOfDeformingBodies; body++ )
    // {
    //   DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
    //   deformingBody.buildBeamFluidInterfaceData( cg0 );
    // }

    realCompositeGridFunction & coeff = poisson->coeff;

    if( true )   // *new* way
    {
      // -- NEW WAY --
      int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      const int numberOfComponentGrids = cg0.numberOfComponentGrids();
      const int numberOfDimensions = cg0.numberOfDimensions();
      for( int body=0; body<numberOfDeformingBodies; body++ )
      {
	DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);

	if( !deformingBody.beamModelHasFluidOnTwoSides() )
	{ 
          // ---- this is NOT a beam model with fluid on two sides ----
	  continue;   
	}

	if( cgf.t<3.*dt )
	  printF("--INS-- *NEW WAY* adjustPressureCoefficients for two-sided beams (if any), t=%9.3e\n",cgf.t);

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

	  MappedGrid & mg0 = cg0[grid0]; // grid of the current boundary point
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

	    MappedGrid & mg1 = cg0[grid1];  // grid of donor

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
	    
	      // Longfei 20170110: try fix the PBF method: 
	      // The AMP condition for this case is
	      // n^T n0 n2^T(p*n+p_d*nd)+ (rhos*hs/rho)*p.n = rhs,
	      // where n is the normal of the current boundary point, nd is the normal of its donor
	      // and n0 is the initial beam normal and n2 is the normal used to project the fluid force onto the beam centerline
	      // So the coeff of p is nDotn0*n2Dotn
	      // and the coeff of p_d is nDotn0*n2Dotnd
	      const bool & useApproximateAMPcondition = parameters.dbase.get<bool>("useApproximateAMPcondition");
	      bool fixCoeff=false;  
	      if( useApproximateAMPcondition && fixCoeff ) // PBF method
		{
		  cout << "--adjustPressureCoefficients--: use the fix for useApproximateAMPcondition\n";
		  //NOTE: for convience, I put everything needed here. It might not be efficient. 
		  
                  #ifdef USE_PPP
		    const realSerialArray & normal = mg0.vertexBoundaryNormalArray(side0,axis0);
		    const realSerialArray & normalDonor = mg1.vertexBoundaryNormalArray(side1,axis1);
                  #else
		    const realSerialArray & normal = mg0.vertexBoundaryNormal(side0,axis0);
		    const realSerialArray & normalDonor = mg1.vertexBoundaryNormal(side1,axis1);
                  #endif

		  DeformingBodyMotion & deform = movingGrids.getDeformingBody(body);
		  BeamModel & beamModel = deform.getBeamModel();
		  DataBase & beamDbase = beamModel.dbase;

		  vector<RealArray*> & surfaceArray = deformingBodyDataBase.get<vector<RealArray*> >("surfaceArray");
		  assert( face<surfaceArray.size() );
		  RealArray *px = surfaceArray[face];
		  RealArray &x0 = px[0], &x1 = px[1], &x2=px[2];
		  
		  Index Ib1,Ib2,Ib3;
		  getGhostIndex( mg0.extendedIndexRange(),side0,axis0,Ib1 ,Ib2 ,Ib3 ,0);     // boundary line
		  RealArray normal2=normal; // normal used for project the force onto the beam centerline
		  const aString & normalOption = beamDbase.get<aString>("normalOption"); 
		  beamModel.getCurrentNormalForProjection(cgf.t,x0,normal2,Ib1,Ib2,Ib3,normalOption);
		  const real *initialBeamNormal = beamDbase.get<real[2]>("initialBeamNormal");
		  
		  // NOTE the fluid normal are always at the boundary points (i1,i2,i3), donorNormal is at (j1,j2,j3)
		  real nDotn0 = normal(i1,i2,i3,0)*initialBeamNormal[0]+ normal(i1,i2,i3,1)*initialBeamNormal[1]; 
		  real n2Dotn = normal2(i1,i2,i3,0)*normal(i1,i2,i3,0)+normal2(i1,i2,i3,1)*normal(i1,i2,i3,1);
		  real n2Dotnd= normal2(i1,i2,i3,0)*normalDonor(j1,j2,j3,0)+normal2(i1,i2,i3,1)*normalDonor(j1,j2,j3,1);
		  //coeff of p
		  
		  if(false)
		    {
		      printF("x=%10.2e,y=%10.2e,nDotn0=%10.2e,n2Dotn=%10.2e,n2Dotnd=%10.2e\n",x0(i1,i2,i3,0),x0(i1,i2,i3,1),nDotn0,n2Dotn,n2Dotnd);
		  
		      cout<<"before overwrite: coeff0(md,i1m,i2m,i3m)=" << coeff0(md,i1m,i2m,i3m) << endl;
		      cout<<"before overwrite: coeff0(me,i1m,i2m,i3m)=" << coeff0(me,i1m,i2m,i3m) << endl;
		    }
		      
		  
		  // overwrites the coeficient of p. It was 1 so subtract 1 and add the new coeff: n^T n_0 n2^T*n
		  coeff0(md,i1m,i2m,i3m) = coeff0(md,i1m,i2m,i3m)-1.+nDotn0*n2Dotn;
		  // overwrites the coeficient of the donor. It was -1 so add 1 and add the new coeff: 
		  coeff0(me,i1m,i2m,i3m)= coeff0(me,i1m,i2m,i3m)+1.+nDotn0*n2Dotnd;

		  if(false)
		    {
		      cout<<"after overwrite: coeff0(md,i1m,i2m,i3m)=" << coeff0(md,i1m,i2m,i3m) << endl;
		      cout<<"after overwrite: coeff0(me,i1m,i2m,i3m)=" << coeff0(me,i1m,i2m,i3m) << endl;
		    }
		  
		}
	      

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
     int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
      const int numberOfComponentGrids = cg0.numberOfComponentGrids();
      const int numberOfDimensions = cg0.numberOfDimensions();
      for( int body=0; body<numberOfDeformingBodies; body++ )
      {
	DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
	if(  !deformingBody.isBeamModel()  ||
	     !deformingBody.beamModelHasFluidOnTwoSides() )
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
	      if( donor <0 )  // This means there is no opposite grid point -- could be the end of the beam
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
    }
    
    // else // **OLD WAY**
    // {
      
    //   std::vector<BoundaryData> & boundaryDataArray =parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
    //   // first locate any boundaries that lie on the beam
    //   const int maxFaces=10;  // fix me 
    //   int sideBody[maxFaces], axisBody[maxFaces], gridBody[maxFaces]; 
    //   int numFaces=0;
    //   for( int grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
    //   {
    // 	if( parameters.gridIsMoving(grid) )
    // 	{
    // 	  BoundaryData & bd = boundaryDataArray[grid];

    // 	  if( bd.dbase.has_key("deformingBodyNumber") )
    // 	  {
    // 	    // --- this grid has a face attached to a deforming body ---

    // 	    int (&deformingBodyNumber)[2][3] = bd.dbase.get<int[2][3]>("deformingBodyNumber");
    // 	    for( int side=0; side<=1; side++ )
    // 	    {
    // 	      for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
    // 	      {
    // 		if( deformingBodyNumber[side][axis]>=0 )
    // 		{
    // 		  int body=deformingBodyNumber[side][axis];
    // 		  gridBody[numFaces]=grid;
    // 		  sideBody[numFaces]=side;
    // 		  axisBody[numFaces]=axis;
    // 		  numFaces++;
    // 		  if( debug()& 4 || cgf.t <= 0. )
    // 		    printF("--APC-- body=%i  has face (grid,side,axis)=(%i,%i,%i)\n",body,grid,side,axis);
    // 		}
	    

    // 	      }
    // 	    }
    // 	  }
      
    // 	}
    //   }

    //   if( numFaces>1 )  // There are multiple grids adjacent to the beam
    //   {
    // 	assert( numFaces==2 ); // do this case for now
    
    // 	int grid0 = gridBody[0], side0=sideBody[0], axis0=axisBody[0];
    // 	int grid1 = gridBody[1], side1=sideBody[1], axis1=axisBody[1];
    
    // 	realMappedGridFunction & coeff0 = coeff[grid0];
    // 	realMappedGridFunction & coeff1 = coeff[grid1];
    
    // 	assert( coeff0.sparse!=NULL );
    // 	SparseRepForMGF & sparse0 = *coeff0.sparse;
    // 	const int equationOffset0=sparse0.equationOffset;
    // 	const int numberOfComponentsForCoefficients0 = sparse0.numberOfComponents;  // size of the system of equations
    // 	const int numberOfGhostLines0 = sparse0.numberOfGhostLines;
    // 	const int stencilSize0 = sparse0.stencilSize;
    // 	const int stencilDim0=stencilSize0*numberOfComponentsForCoefficients0; // number of coefficients per equation

    // 	intArray & equationNumber0 = sparse0.equationNumber;
    // 	intArray & classify0 = sparse0.classify;
    
    // 	MappedGrid & mg0 = cg0[grid0];
    // 	OV_GET_SERIAL_ARRAY_CONST(real,mg0.center(),xy0Local);
    // 	getBoundaryIndex(mg0.gridIndexRange(),side0,axis0,Ib1,Ib2,Ib3); // boundary index's for mg0

    // 	assert( coeff1.sparse!=NULL );
    // 	SparseRepForMGF & sparse1 = *coeff1.sparse;
    // 	const int equationOffset1=sparse1.equationOffset;
    // 	intArray & equationNumber1 = sparse1.equationNumber;
    // 	intArray & classify1 = sparse1.classify;

    // 	MappedGrid & mg1 = cg0[grid1];
    // 	OV_GET_SERIAL_ARRAY_CONST(real,mg1.center(),xy1Local);

    // 	getBoundaryIndex(mg1.gridIndexRange(),side1,axis1,Jb1,Jb2,Jb3);  // boundary index's for mg1 

    // 	is1=is2=is3=0;
    // 	isv[axis0]=1-2*side0;

    // 	js1=js2=js3=0;
    // 	jsv[axis1]=1-2*side1;

    // 	Range all;

    // 	bool gridPointsMatch = true;
    // 	if( gridPointsMatch )
    // 	{
    // 	  // --- CASE I: Separate grids and grid points match on both sides of the beam ----
    // 	  FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Jb1,Jb2,Jb3) // loop over points on the boundary 
    // 	  {
    // 	    i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)
    // 	    j1m=j1-js1, j2m=j2-js2, j3m=j3-js3; //  ghost point is (j1m,j2m,j3m)
    // 	    // coeff(mm,i1m,i2m,i3m)
    // 	    // add the extra equations:
    // 	    //   p0 + (rhos*hs/rho)*p0.n  - p1 =     (add -p1 to this Robin BC)
    // 	    //   p1 + (rhos*hs/rho)*p1.n  - p0 =     (add -p0 to this Robin BC)

    // 	    int me = stencilDim0-1;  // "extra" equation goes here at end of the coefficients
    // 	    int md=4;  // hard code for now -- this should "diagonal" entry

    // 	    // add a "-1" coefficient to the AMP Robin BC on grid0
    // 	    assert( coeff0(me,i1m,i2m,i3m)==0. );
    // 	    coeff0(me,i1m,i2m,i3m)=-1.;
    // 	    equationNumber0(me,i1m,i2m,i3m)=equationNumber1(md,j1m,j2m,j3m);  // -1 multiplies eqn1

    // 	    // add a "-1" coefficient to the AMP Robin BC on grid1
    // 	    assert( coeff1(me,j1m,j2m,j3m)==0. );
    // 	    coeff1(me,j1m,j2m,j3m)=-1.;
    // 	    equationNumber1(me,j1m,j2m,j3m)=equationNumber0(md,i1m,i2m,i3m);
      
    // 	    if( debug() & 8 && cgf.t <= 0. )
    // 	    {
    // 	      printF(" grid0=%i (i1m,i2m)=(%i,%i) x=(%9.3e,%9.3e)\n",grid0,i1m,i2m,xy0Local(i1m,i2m,i3m,0),xy0Local(i1m,i2m,i3m,1));
    // 	      printF(" grid1=%i (j1m,j2m)=(%i,%i) x=(%9.3e,%9.3e)\n",grid1,j1m,j2m,xy1Local(j1m,j2m,j3m,0),xy1Local(j1m,j2m,j3m,1));
      
    // 	      ::display(coeff0(all,i1m,i2m,i3m),sPrintF("coeff on ghost point (%i,%i) of grid=%i",i1m,i2m,grid0),"%8.2e ");
    // 	      ::display(equationNumber0(all,i1m,i2m,i3m),sPrintF("equationNumber on ghost point (%i,%i) of grid=%i",i1m,i2m,grid0),"%8i ");

    // 	      ::display(coeff1(all,j1m,j2m,j3m),sPrintF("coeff on ghost point (%i,%i) of grid=%i",j1m,j2m,grid1),"%8.2e ");
    // 	      ::display(equationNumber1(all,j1m,j2m,j3m),sPrintF("equationNumber on ghost point (%i,%i) of grid=%i",j1m,j2m,grid1),"%8i ");
    // 	    }
      

    // 	  }
    
    // 	}
    
    //   }
    // } // end if old way
    
  
  } // end deforming bodies
  
  const int numberOfRigidBodies = movingGrids.getNumberOfRigidBodies();

  if( numberOfRigidBodies>0 )
  {
    // ======================================================================
    // =================== RIGID BODY AMP SCHEME ============================
    // ======================================================================
    
    const real & t = cgf.t;
    if( debug()& 4 || cgf.t <= dt )
      printF("--INS:APC-- rigidy body AMP scheme -- numberOfRigidBodies=%i, t=%9.3e: adjust pressure BC and"
	     " add direct projection equations\n",numberOfRigidBodies,t);
    
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
    if( debug()& 4 || cgf.t <= dt )
      printF("--APC-- numberOfDenseExtraEquations=%i, totalNumberOfExtraEquations=%i\n",
	     numberOfDenseExtraEquations,totalNumberOfExtraEquations);

    // --- there are user defined extra equations ----
    pSolver.set(OgesParameters::THEuserSuppliedEquations,true);

    pSolver.initialize(); // ---- Need to call initialize now ********** FIX ME **********

    if( cgf.t<= dt && (debug() & 1)  )
      ::display(pSolver.extraEquationNumber,"pSolver.extraEquationNumber");

    assert( pSolver.extraEquationNumber.getBound(0)==totalNumberOfExtraEquations-1 );

    Index Ib1,Ib2,Ib3;

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
        
        OV_GET_SERIAL_ARRAY(int,mg.mask(),maskLocal);
	int includeGhost=0;
	bool ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ib1,Ib2,Ib3,includeGhost);

	int numSurfacePoints=0;
	if( ok )
          numSurfacePoints = Ib1.getLength()*Ib2.getLength()*Ib3.getLength();
	
	if( debug()& 4 || cgf.t <= dt )
	  printF("--APC--- rigidBody %i: face=%i (side,axis,grid)=(%i,%i,%i) numSurfacePoints=%i\n",b,face,side,axis,grid,numSurfacePoints);

        totalNumberOfSurfacePoints+= numSurfacePoints;

      }
    }

    // ---------------------------------------------
    // --- add up the total number of nonzeros: ---
    // --------------------------------------------
    // Each extra equation contains an integral constraint: 
    int numberOfNonzerosInConstraint = totalNumberOfSurfacePoints*numberOfExtraEquations; // surface integrals 
    if( numberOfDimensions==2 )
    {
      // mb*a1, mb*a2, A33*b3 terms: 
      numberOfNonzerosInConstraint += numberOfRigidBodies*numberOfExtraEquationsPerBody;
    }
    else
    {
      // mb*a1, mb*a2, mb*a3, A*bv terms 
      numberOfNonzerosInConstraint += numberOfRigidBodies*(  numberOfDimensions + SQR(numberOfDimensions));
    }

    if( useAddedDampingAlgorithm )
    {
      // count added damping terms: 
      // Added damping looks like this (in 3D)
      //       [           ][ a1 ]
      //       [           ][ a2 ]
      //    dt*[   Matrix  ][ a3 ]
      //       [           ][ b1 ]
      //       [           ][ b2 ]
      //       [           ][ b3 ]
      numberOfNonzerosInConstraint +=  numberOfRigidBodies*( SQR(numberOfExtraEquationsPerBody) );
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
      else
      {
	addedDampingTensors=0.;
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
      //    mb*a1 + INT p n_1 ds + dt*Dvv(1,1)*a1 + Dvv(1,2)*a2 + Dvw(1,3)*b3  = ...
      //    mb*a2 + INT p n_2 ds + dt*Dvv(2,1)*a1 + Dvv(2,2)*a2 + Dvw(2,3)*b3 = ...
      //    A(3,3)*b3 + INT n_1 r_2 - n_2 r_1 ds + dt*Dwv(3,1)*a1 + D3v(3,2)*a2+  dt*Dww(3,3)*b3 = ...    
      //
      // --- 3D: AMP constraint equations are: ( e1=[1 0 0],  e2=[0 1 0], e3= [ 0 0 1] )
      //                        mb*a1 - INT p n_1 ds +    [     |     ][ a1 ]  = ...
      //                        mb*a2 - INT p n_2 ds +    [ Dvv | Dvw ][ a2 ]  = ...
      //                        mb*a3 - INT p n_3 ds + dt*[_____|_____][ a3 ]  = ...
      //   [       ][ b1 ] - INT e1 . rv X (p nv) ds +    [     |     ][ b1 ]  = ...
      //   [   A   ][ b2 ] - INT e2 . rv X (p nv) ds +    [ Dwv | Dww ][ b2 ]  = ... 
      //   [       ][ b3 ] - INT e3 . rv X (p nv) ds +    [     |     ][ b3 ]  = ...
      //
      // -------------------------------------------------------------------------------------

      
      // Determine the equation numbers in the sparse matrix for the 3 (or 6) rigid body unknowns
      // ieqv[i] = index into the sparse matrix for unknown i
      int ieqv[6];
      for( int i=0; i<numberOfExtraEquationsPerBody; i++ ) // counts extra equations for this body 
      {
        int ee = i + b*numberOfExtraEquationsPerBody;  // counts all extra equations (i.e. for all bodies)
	
        ieqv[i] = getSparseMatrixEquationNumberFromExtraEquationNumber( ee,totalNumberOfExtraEquations,pSolver);

        equation(ee)=ieqv[i];  // Constraint equation is located here in the sparse matrix

	if( debug()& 4 || cgf.t <= 0. )
	  printF(" body=%i: extraEquation=%i --> sparse matrix equation=%i\n",b,ee,ieqv[i]);
      }


      const int vbc=0, wbc=1; // component numbers of v and omega in addedDampingTensors

      // The combined matrix of added mass and added damping coefficients looks like this in 2D
      //  {  [mb   0   0  ]      [           ] }  [ a1 ]
      //  {  [ 0  mb      ] + dt*[   Matrix  ] }  [ a2 ]
      //  {  [ 0   0  I33 ]      [           ] }  [ b3 ]
      RealArray Amd(numberOfExtraEquationsPerBody,numberOfExtraEquationsPerBody);
      Amd=0.;
      RealArray & adt = addedDampingTensors;  // short form
      RealArray & Ib = inertiaTensor;         // short form
      
      const int nd=numberOfDimensions; // short form
      if( nd==2 )
      {
        // ---- Fill in 2D matrix of added mass and added damping coefficients ----
	Amd(0,0)=massBody;  // diagonal terms from mb*av 
	Amd(1,1)=massBody;
	Amd(2,2)=Ib(2,2);   // moment of inertia matrix terms 
       
	if( useAddedDampingAlgorithm )
	{
          // -- added damping --
	  // Eqn 1: ()*a1 + ()*a2 + ()*b3 
	  Amd(0,0)+= dt*adt(0,0,vbc,vbc);
	  Amd(0,1)+= dt*adt(0,1,vbc,vbc);
	  Amd(0,2)+= dt*adt(0,2,vbc,wbc); // note wbc
	 
	  // Eqn 2: 
	  Amd(1,0)+= dt*adt(1,0,vbc,vbc);
	  Amd(1,1)+= dt*adt(1,1,vbc,vbc);
	  Amd(1,2)+= dt*adt(1,2,vbc,wbc); // note wbc
	 
	  // Eqn 3
	  Amd(2,0)+= dt*adt(2,0,wbc,vbc);
	  Amd(2,1)+= dt*adt(2,1,wbc,vbc);
	  Amd(2,2)+= dt*adt(2,2,wbc,wbc);
	}
	
	if( dt==0. || (t < 3.*dt) || (debug() & 4) )
	{
	  printF("--AdjustPressureCoefficients : dt=%9.2e, useAddedDampingAlgorithm=%i\n"
		 "   Mass + dt(Added Damping) body=%i   \n"
		 " -------------------------------------\n"
		 "       [ %12.4e %12.4e %12.4e ] \n"
		 " Amd = [ %12.4e %12.4e %12.4e ] \n"
		 "       [ %12.4e %12.4e %12.4e ] \n",
		 dt,(int)useAddedDampingAlgorithm,b,
		 Amd(0,0),Amd(0,1),Amd(0,2),
		 Amd(1,0),Amd(1,1),Amd(1,2),
		 Amd(2,0),Amd(2,1),Amd(2,2));
	  printF("  --APC-- dt=%9.3e, adt(0,0,vbc,vbc)=%12.5e, params.dt=%9.3e\n",dt,adt(0,0,vbc,vbc),
                  parameters.dbase.get<real >("dt"));

	}


      }
      else
      {
        // ---- Fill in 3D matrix of added mass and added damping coefficients ----
        // **CHECK ME**
        for( int dir=0; dir<nd; dir++ )
	{
          Amd(dir,dir)=massBody;                // diagonal terms from mb*av 
	  for( int dir2=0; dir2<nd; dir2++ )
	    Amd(dir+nd,dir2+nd)=Ib(dir,dir2);  // moment of inertia matrix terms 
	}
	if( useAddedDampingAlgorithm )
	{
	  // -- added damping --
	  for( int dir=0; dir<nd; dir++ )
	  {
	    for( int dir2=0; dir2<nd; dir2++ )
	    {
	      Amd(dir   ,dir2   ) += dt*adt(dir,dir2,vbc,vbc);
	      Amd(dir   ,dir2+nd) += dt*adt(dir,dir2,vbc,wbc);

	      Amd(dir+nd,dir2   ) += dt*adt(dir,dir2,wbc,vbc);
	      Amd(dir+nd,dir2+nd) += dt*adt(dir,dir2,wbc,wbc);
	    }
	  }
	}
	
        if (false)
        {	
            ::display(adt,"--APC-- addedDamping =");
        }

        if( dt==0. || (t < 3.*dt) || (debug() & 4) )
	{
	  printF("--AdjustPressureCoefficients : dt=%9.2e, useAddedDampingAlgorithm=%i\n"
		 "   Mass + dt(Added Damping) body=%i   \n"
		 " -------------------------------------\n"
		 "       [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n"
		 "       [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n"
		 " Amd = [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n"
		 "       [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n"
		 "       [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n"
		 "       [ %12.4e %12.4e %12.4e %12.4e %12.4e %12.4e] \n",
		 dt,(int)useAddedDampingAlgorithm,b,
		 Amd(0,0),Amd(0,1),Amd(0,2),Amd(0,3),Amd(0,4),Amd(0,5),
		 Amd(1,0),Amd(1,1),Amd(1,2),Amd(1,3),Amd(1,4),Amd(1,5),
		 Amd(2,0),Amd(2,1),Amd(2,2),Amd(2,3),Amd(2,4),Amd(2,5), 
		 Amd(3,0),Amd(3,1),Amd(3,2),Amd(3,3),Amd(3,4),Amd(3,5), 
		 Amd(4,0),Amd(4,1),Amd(4,2),Amd(4,3),Amd(4,4),Amd(4,5), 
		 Amd(5,0),Amd(5,1),Amd(5,2),Amd(5,3),Amd(5,4),Amd(5,5)) ;
	  printF("  --APC-- dt=%9.3e, adt(0,0,vbc,vbc)=%12.5e, params.dt=%9.3e\n",dt,adt(0,0,vbc,vbc),
                  parameters.dbase.get<real >("dt"));

	}
      }


      for( int vbType=0; vbType<=1; vbType++ )
      {
        if( debug()& 4 || cgf.t <= 0. )
	  printF("--APC: body b=%i, vbType=%i , massBody=%9.3e\n",b,vbType,massBody);

        //  numDim = numberOfDimensions in 3D or in 2D 
        //         = 1 for 2D angular velocity
        const int numDim = (vbType==0 || numberOfDimensions==3) ? numberOfDimensions : 1;
	for( int dir=0; dir<numDim; dir++ )
	{
          // current extra equation: 
	  const int localEqn = dir + numberOfDimensions*(vbType);
	  const int extraEqn = dir + numberOfDimensions*(vbType) + b*( numberOfExtraEquationsPerBody ); 

	  // solver.extraEquationNumber : NOTE extra equations are stored in reverse order:
          //                             last extra eqn is optional dense constraint. 
          // ieqn : fill in this extra equation 
	  
          const int jeqn=totalNumberOfExtraEquations - extraEqn -1;
	  const int ieqn = pSolver.extraEquationNumber(jeqn);

          // printF("--APC-- extraEqn=%i, ieqn=%i, jeqn=%i, totalNumberOfExtraEquations=%i numberOfDenseExtraEquations=%i\n",
	  //    extraEqn,ieqn,jeqn,totalNumberOfExtraEquations,numberOfDenseExtraEquations);

	  equation(extraEqn)=ieqn;  // Constraint equation is located here in the sparse matrix
	  ia(extraEqn)=nnz;         // This extra equation (extraEqn) starts at nnz in (ja,a)
	  if( true )
	  {
            // ** new way **
	    for( int j=0; j<numberOfExtraEquationsPerBody; j++ )
	    {
	      if( Amd(localEqn,j)!=0. )
	      {
		ja(nnz)=ieqv[j];
		a(nnz)=Amd(localEqn,j);
		nnz++;
	      }
	    }
	  }
	  else
	  {
	    // *OLD WAY*
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

		  const real Dww = addedDampingTensors(2,2,wbc,wbc);  // coeff of the angular velocity in the omega_t eqn
		  real impFactor=1.;
                  if( debug()& 4 || cgf.t <= 0. )
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
	  } // end old way
	  

	  const int numberOfFaces=integrate->numberOfFacesOnASurface(b);
          // --------------- LOOP OVER FACES OF THE RIGID BODY --------------------
	  for( int face=0; face<numberOfFaces; face++ )
	  {
	    int side=-1,axis,grid;
	    integrate->getFace(b,face,side,axis,grid);
	    assert( side>=0 && side<=1 && axis>=0 && axis<cg.numberOfDimensions());
	    assert( grid>=0 && grid<cg.numberOfComponentGrids());

            if( debug()& 4 || cgf.t <= 0. )
	      printF("--APC: body b=%i, vbType=%i dir=%i face=%i\n",b,vbType,dir,face);
	    
	    MappedGrid & mg = cg0[grid];
	    const IntegerArray & gid = mg.gridIndexRange();
	
	    mg.update(MappedGrid::THEvertexBoundaryNormal);
            OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
            
            // #ifdef USE_PPP
	    //   const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
            // #else
	    //   const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
            // #endif

	    int extra=1; // include the corner. is this needed??
	    getBoundaryIndex(cg[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3,1,extra); // boundary points
	    getGhostIndex(cg[grid].gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1,extra);    // ghost points

	    OV_GET_SERIAL_ARRAY(real,mg.vertex(),xLocal);
	    OV_GET_SERIAL_ARRAY(real,weights[grid],weightsLocal);

	    int includeGhost=0;
	    bool ok=ParallelUtility::getLocalArrayBounds(mg.vertex(),xLocal,Ib1,Ib2,Ib3,includeGhost);
	    ok=ParallelUtility::getLocalArrayBounds(mg.vertex(),xLocal,Ig1,Ig2,Ig3,includeGhost);
	    if( !ok ) continue;
	    
            // #ifdef USE_PPP
	    //   // *TEMP FIX:
            //   // -- Get a copy of the weights from all processors so that the user supplied equations
            //   //    are the same on all processors. Do this until Oges is fixed to handle distributed user supplied equations
            //   RealArray weightsLocal(Ig1,Ig2,Ig3);
	    //   const int np=max(1,Communication_Manager::Number_Of_Processors);
            //   IndexBox vBox[np]; // specifies bounds of weightsLocal on all processors
	    //   for( int p=0; p<np; p++ )
	    //   {
            //     vBox[p].setBounds(Ig1.getBase(),Ig1.getBound(),
	    // 			  Ig2.getBase(),Ig2.getBound(),
	    // 			  Ig3.getBase(),Ig3.getBound());
	    //   }
            //   // Copy all weights into local array weightsLocal:
	    //   CopyArray::copyArray(weights[grid],Igv,vBox,weightsLocal);
            // #else
            //   realArray & weightsLocal = weights[grid];
            // #endif

	    if( false && vbType==0 && dir==0 && face==0 )
	    {
	      ::display(weightsLocal(Ig1,Ig2,Ig3),"--APC-- Integration weights");
	    }
        

	    FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Ig1,Ig2,Ig3)
	    {
              // [i1,i2,i3] = index of point on the boundary
              // [j1,j2,j3] = index of corresponding host point

    	      const int n=0; // component number
              // jeqn : equation number in sparse matrix for boundary pt (i1,i2,i3) on grid 
	      const int jeqn = pSolver.equationNo( n,i1,i2,i3,grid ); 

              // Note: surface weights are stored in the ghost points: weightsLocal(j1,j2,j3)
              assert( nnz<numberOfNonzerosInConstraint );
	      ja(nnz)=jeqn;
	      if( vbType==0 )
	      {
                // -- for RB linear velocity the coeff of p is the weight*normal
                //  INCLUDE TERM:    INT_B p \nv 

		if( false && dir==0 )
		  printF(" --ADPC-- add INT{ p nv}ds term - dir=%i, w(j)=%10.3e [i1,i2]=[%i,%i] [j1,j2]=[%i,%i]\n",
			 dir,weightsLocal(j1,j2,j3),i1,i2,j1,j2);
		
		// Note: surface integral weights are stored in the ghost points of weightsLocal
  	        a(nnz)= - weightsLocal(j1,j2,j3)*normal(i1,i2,i3,dir);  
	      }
	      else
	      {
                // -- for angular velocity the coeff of p is
                //  INCLUDE TERM:    INT_B ( rv- xvb) X  (p nv)

                //       - (x-xb) X nv 
		real rv[3]; // holds x-xb
   	        for( int d=0; d<numberOfDimensions; d++ ){  rv[d]=xLocal(i1,i2,i3,d)-xb(d); } //             
		if( numberOfDimensions==2 )
		{
                  // This looks correct (from ellipse case)
		  a(nnz)= - weightsLocal(j1,j2,j3)*(normal(i1,i2,i3,1)*rv[0]-normal(i1,i2,i3,0)*rv[1]);

		  if( false && fabs(a(nnz))>1.e-12  )
		  {
		    // this entry should be zero for a disk:
		    printF(" --ADPC-- WARNING: t=%1.3e  dir=%i, weight*( nv X (rv-xvb)=%10.3e\n",
			   t,dir,-a(nnz));
		  }
		    
		}
		else
		{
                  const int dirp1 = (dir+1) % numberOfDimensions;
     		  const int dirp2 = (dir+2) % numberOfDimensions;
                  a(nnz)= - weightsLocal(j1,j2,j3)*(normal(i1,i2,i3,dirp2)*rv[dirp1]-normal(i1,i2,i3,dirp1)*rv[dirp2]);

		  if( false && fabs(a(nnz))>1.e-7 )
		  {
		    // this entry should be zero for a sphere as well:
		    printF(" --APC-- : t=%1.3e  dir=%i, weight*( nv X (rv-xvb)=%10.3e\n",
                            t,dir,-a(nnz));
		  }
		}
	      }
	      
	      nnz++;
	    }
	  }

	} // end for dir 
      } // end for vbType 
      
    } // end for body b
    ia(numberOfExtraEquations)=nnz; 
    

    assert( nnz <= numberOfNonzerosInConstraint );

    if( debug() & 32 )
    {
      printF("--APC-- nnz=%i, numberOfNonzerosInConstraint=%i\n",nnz,numberOfNonzerosInConstraint);
      printF("--APC-- constraint equations in CSR format:\n");
      for( int i=0; i<numberOfExtraEquations; i++ )
      {
	printF("\n eqn=%i, i=%i: ",equation(i),i);
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
        OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal);
 	
	realMappedGridFunction & coeff0 = coeff[grid];
        OV_GET_SERIAL_ARRAY(real,coeff0,coeffLocal);
    
	assert( coeff0.sparse!=NULL );
	SparseRepForMGF & sparse = *coeff0.sparse;
	// intArray & equationNumber = sparse.equationNumber;
        OV_GET_SERIAL_ARRAY(int,sparse.equationNumber,equationNumber);
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

	int includeGhost=0;
	bool ok=ParallelUtility::getLocalArrayBounds(classify,classifyLocal,Ib1,Ib2,Ib3,includeGhost);
	ok=ParallelUtility::getLocalArrayBounds(classify,classifyLocal,Ig1,Ig2,Ig3,includeGhost);
        if( !ok ) continue;

	FOR_3IJD(i1,i2,i3,Ib1,Ib2,Ib3,j1,j2,j3,Ig1,Ig2,Ig3)
	{
	  // (i1,i2,i3) : boundary pt
	  // (j1,j2,j3) : ghost pt

	  // classify: -2=periodic, -1=interpolation, 1=boundary, 2=interior
	  bool neumannBC = classifyLocal(i1,i2,i3)==SparseRepForMGF::boundary || 
                           classifyLocal(i1,i2,i3)==SparseRepForMGF::interior;
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
	    int me = emptySpot[dir]; // put the coefficient here in the stencil
	     
	    if(  coeffLocal(me,j1,j2,j3)!=0. )
	    {
	      printF("ERROR: me=%i (j1,j2,j3)=(%i,%i,%i) coeff=%9.2e mask(i1,i2,i3)=%i classify(i1,i2,i3)=%i classify(j1,j2,j3)=%i \n",
		     me,j1,j2,j3,coeffLocal(me,j1,j2,j3),maskLocal(i1,i2,i3),classifyLocal(i1,i2,i3),classifyLocal(j1,j2,j3));
	    }
	    
	    assert( coeffLocal(me,j1,j2,j3)==0. );
	    coeffLocal(me,j1,j2,j3)=fluidDensity*normal(i1,i2,i3,dir);
	    // we have changed the equation number 
	    equationNumber(me,j1,j2,j3)=ieqn;   // we have set the coeff of a[dir]
            if( false )
            {
              printF("--APC-- RB-AMP: p.n + c*a : dir=%i add term %10.3e to ieqn=%i, extraEqn=%i\n",
                     dir,coeffLocal(me,j1,j2,j3),ieqn,extraEqn);
            }
            

	    // --- add bv terms ---
	    //     bv X ( xv - xb ) = cv . bv 
	    //     c_i = n[i+2]*r[i+1] - nv[i+1]*rv[i+2]   i=1,2,3
	    real rv[3]; // holds x - xb
	    for( int d=0; d<numberOfDimensions; d++ ){  rv[d]=xLocal(i1,i2,i3,d)-xb(d); } //             
            if( numberOfDimensions==2 )
	    {
	      if( dir==0 ) // we only keep component b3 of the angular acceleration in 2D 
	      {
		extraEqn = numberOfDimensions + dir + b*( numberOfExtraEquationsPerBody ); //  b-equation
		ieqn = pSolver.extraEquationNumber(totalNumberOfExtraEquations - extraEqn -1);
		me = emptySpot[numberOfDimensions + dir]; // put the coefficient here in the stencil
		assert( coeffLocal(me,j1,j2,j3)==0. );
	    
		coeffLocal(me,j1,j2,j3)=fluidDensity*( normal(i1,i2,i3,1)*rv[0]-normal(i1,i2,i3,0)*rv[1] ); // coeff of b3
		equationNumber(me,j1,j2,j3)=ieqn;   
	      }
	      
	    }
	    else
	    { // In 3D there are 3 components of the angular acceleration
              // 
	      extraEqn = numberOfDimensions + dir + b*( numberOfExtraEquationsPerBody ); //  b-equation
	      ieqn = pSolver.extraEquationNumber(totalNumberOfExtraEquations - extraEqn -1);
	      me = emptySpot[numberOfDimensions + dir]; // put the coefficient here in the stencil
	      assert( coeffLocal(me,j1,j2,j3)==0. );
	    
	      const int dirp1 = (dir+1) % numberOfDimensions;
	      const int dirp2 = (dir+2) % numberOfDimensions;
	      coeffLocal(me,j1,j2,j3)=fluidDensity*( normal(i1,i2,i3,dirp2)*rv[dirp1]-normal(i1,i2,i3,dirp1)*rv[dirp2] );
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
