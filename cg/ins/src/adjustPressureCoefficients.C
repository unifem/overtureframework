#include "Cgins.h"

#include "Parameters.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "SparseRep.h"
#include "Oges.h"
#include "DeformingBodyMotion.h"
#include "BeamFluidInterfaceData.h"

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
  if( !useAddedMassAlgorithm || !parameters.isMovingGridProblem() )
    return;

  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Jbv[3], &Jb1=Jbv[0], &Jb2=Jbv[1], &Jb3=Jbv[2];
  int i1,i2,i3, j1,j2,j3, i1m,i2m,i3m, j1m,j2m,j3m, m1,m2,m3;
  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  int jsv[3], &js1=jsv[0], &js2=jsv[1], &js3=jsv[2];

  CompositeGridOperators & cgop = *cgf.u.getOperators();

  const int & pc = parameters.dbase.get<int >("pc");

  const real & dt=parameters.dbase.get<real>("dt");
  if( cgf.t<3.*dt )
    printF("--INS-- adjustPressureCoefficients for two-sided beams (if any), t=%9.3e\n",cgf.t);
  
  MovingGrids & movingGrids = parameters.dbase.get<MovingGrids >("movingGrids");
  const int numberOfDeformingBodies= movingGrids.getNumberOfDeformingBodies();
  if( numberOfDeformingBodies<=0 )
    return;

  // -- construct the beam-fluid interface data needed for the AMP scheme for two-sided beams  --
  // *** THIS ONLY NEEDS TO BE DONE ONCE ***
  for( int body=0; body<numberOfDeformingBodies; body++ )
  {
    DeformingBodyMotion & deformingBody = movingGrids.getDeformingBody(body);
    deformingBody.buildBeamFluidInterfaceData( cg0 );
  }

  realCompositeGridFunction & coeff = poisson->coeff;

  if( true ) 
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

	  if( grid1<0 )  // This means there is do opposite grid point -- could be the end of the beam
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
