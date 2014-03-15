#include "Cgmp.h"
#include "ParallelUtility.h"
#include "App.h"
#include "Interface.h"
#include "InterfaceTransfer.h"

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

void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                     IntegerArray & gidLocal, 
                                     IntegerArray & dimensionLocal, 
                                     IntegerArray & bcLocal );

#define interfaceCnsSm EXTERN_C_NAME(interfacecnssm)
extern "C"
{
void interfaceCnsSm( const int&nd, 
		    const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
		    const int&gridIndexRange1, real&u1, const int&mask1,const real&rsxy1, const real&xy1, const real&gv1,
		    const int&boundaryCondition1, 
		    const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
		    const int&gridIndexRange2, real&u2, const int&mask2,const real&rsxy2, const real&xy2, const real&gv2,
		    const int&boundaryCondition2,
		     const int&ipar, const real&rpar, const DataBase *pdb1, const DataBase *pdb2, 
		    real&aa2, real&aa4, real&aa8, 
		    int&ipvt2, int&ipvt4, int&ipvt8,
		    int&ierr );
}

// ===================================================================================================================
/// \brief Return the interface type for a given grid face on the interface.
// ==================================================================================================================
int Cgmp::
getInterfaceType( GridFaceDescriptor & gridDescriptor )
{
  const int domain=gridDescriptor.domain, grid=gridDescriptor.grid, side=gridDescriptor.side, dir=gridDescriptor.axis;
  const IntegerArray & interfaceType = domainSolver[domain]->parameters.dbase.get<IntegerArray >("interfaceType");
  return interfaceType(side,dir,grid);

}
      


// ===================================================================================================================
/// \brief Project values on the interface to stabilize the scheme.
/// \details For an interface between a compressible fluid and an elastic solid we adjust the velocity
///   and stress on the interface so that the time stepping scheme remains stable for different
///   material properties. For example this correction is useful for the case of a "light" solid 
///   next to a "heavy" fluid is
///   
/// \param t (input) : current time
/// \param dt (input) : current time step
/// \param correct (input) : correction step number.
/// \param gfIndex (input) : Domain d should use the grid function : domainSolver[d]->gf[gfIndex[d]]
/// \param option (input) : 0=set values on the interface , 1=apply BC's to ghost points 
// 
// ==================================================================================================================
int Cgmp::
interfaceProjection( real t, real dt, int correct, std::vector<int> & gfIndex, int option )
{
  if( !parameters.dbase.get<bool>("projectInterface") ) return 0;
  
  // *** NOTE: The initial version of this code came from assignInterfaceBoundaryConditions (solve the coupled problem) ***********

  real cpu0=getCPU();

  if( !gridHasMaterialInterfaces ) return 0;

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  
  assert( interfaceList.size()!=0 );

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
  

  // *******************************************
  // ******* Project interface values.  ********
  // *******************************************

  // We need to interpolate the grid function if an interface is covered by more than 1 face
  std::vector<bool> interpolateThisDomain(numberOfDomains,false);

  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // there may be multiple grid faces that lie on the interface:     
    for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptora = interfaceDescriptor.gridListSide1[face];
      GridFaceDescriptor & gridDescriptorb = interfaceDescriptor.gridListSide2[face];

      const int interfaceType = getInterfaceType( gridDescriptora );
      
      // For now we only handle traction boundaries:
      if( interfaceType!=Parameters::tractionInterface )
        continue;
      
      // -- put this somewhere --
      enum DomainSolverTypeEnum
      {
        unknownDomainSolver=-1,
        CgadDomainSolver=0,
	CgcnsDomainSolver,
	CginsDomainSolver,
        CgsmDomainSolver
      };
      
      // We need a function to get the domainSolverType
      DomainSolverTypeEnum domainSolverTypea=unknownDomainSolver;
      aString className = domainSolver[gridDescriptora.domain]->getClassName();
      if( className=="Cgcns" )
      {
        domainSolverTypea=CgcnsDomainSolver;
      }
      else if( className=="Cgsm" )
      {
        domainSolverTypea=CgsmDomainSolver;
      }

      DomainSolverTypeEnum domainSolverTypeb=unknownDomainSolver;
      className = domainSolver[gridDescriptorb.domain]->getClassName();
      if( className=="Cgcns" )
      {
        domainSolverTypeb=CgcnsDomainSolver;
      }
      else if( className=="Cgsm" )
      {
        domainSolverTypeb=CgsmDomainSolver;
      }

      // -- For now we only handle the case of a fluid next to a solid.
      if( domainSolverTypea==unknownDomainSolver || domainSolverTypeb==unknownDomainSolver || domainSolverTypea==domainSolverTypeb )
      {
	continue;
      }
      
      assert( (domainSolverTypea==CgcnsDomainSolver && domainSolverTypeb==CgsmDomainSolver) ||
              (domainSolverTypeb==CgcnsDomainSolver && domainSolverTypea==CgsmDomainSolver) );
      

      // gridDescriptor1 = fluid, 
      // gridDescriptor2 = solid

      GridFaceDescriptor & gridDescriptor1 = domainSolverTypea==CgcnsDomainSolver ? gridDescriptora : gridDescriptorb;
      GridFaceDescriptor & gridDescriptor2 = domainSolverTypea==CgcnsDomainSolver ? gridDescriptorb : gridDescriptora;

      
      const int d1=gridDescriptor1.domain, grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );

      GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];

      CompositeGrid & cg1 = gf1.cg;
      assert( grid1>=0 && grid1<=cg1.numberOfComponentGrids());
      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();

      CompositeGrid & cg2 = gf2.cg;
      assert( grid2>=0 && grid2<=cg2.numberOfComponentGrids());
      MappedGrid & mg2 = cg2[grid2];
      const IntegerArray & bc2 = mg2.boundaryCondition();
      const IntegerArray & share2 = mg2.sharedBoundaryFlag();

      IntegerArray gidLocal1(2,3), dimLocal1(2,3), bcLocal1(2,3);
      getLocalBoundsAndBoundaryConditions( gf1.u[grid1],gidLocal1,dimLocal1,bcLocal1 );

      IntegerArray gidLocal2(2,3), dimLocal2(2,3), bcLocal2(2,3);
      getLocalBoundsAndBoundaryConditions( gf2.u[grid2],gidLocal2,dimLocal2,bcLocal2 );

      mg1.update(MappedGrid::THEvertexBoundaryNormal);
      mg2.update(MappedGrid::THEvertexBoundaryNormal);
      
      #ifdef USE_PPP
        const realSerialArray & normal1  = mg1.vertexBoundaryNormalArray(side1,dir1);
        const realSerialArray & normal2  = mg2.vertexBoundaryNormalArray(side2,dir2);
      #else
        const realSerialArray & normal1  = mg1.vertexBoundaryNormal(side1,dir1);
        const realSerialArray & normal2  = mg2.vertexBoundaryNormal(side2,dir2);
      #endif


      const int numberOfDimensions = cg1.numberOfDimensions();

      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,extra);
      getBoundaryIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,extra);
  
      // check that the number of points in the tangential directions match -- eventually we will fix this
      for( int dir=1; dir<mg1.numberOfDimensions(); dir++ )
      {
	int dir1p = (dir1+dir) % mg1.numberOfDimensions();
	int dir2p = (dir2+dir) % mg2.numberOfDimensions();
	if( Iv[dir1p].getLength()!=Jv[dir2p].getLength() )
	{
	  printF("interfaceProjection:ERROR: The number of grid points on the two interfaces do not match\n"
	         " (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i) Iv=[%i,%i][%i,%i][%i,%i]\n"
		 " (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
		 d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                 d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2),
                   J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());
	  cout<<"grid names are "<<mg1.getName()<<" , "<<mg2.getName()<<endl;
	  OV_ABORT("error");
	}

        // We need to interpolate the grid function if an interface has interpolation points on it:

        // **** NOTE: I think this is needed because we do not check the mask array when assigning
        //            interface points and thus over-write interpolation points **** FIX ME ***

        if( bc1(0,dir1p)==0 || bc1(1,dir1p)==0 )
	{
         interpolateThisDomain[d1]=true;
	}
        if( bc2(0,dir2p)==0 || bc2(1,dir2p)==0 )
	{
         interpolateThisDomain[d2]=true;
	}

      }

      realArray & u1 = gf1.u[grid1];
      realArray & u2 = gf2.u[grid2];

      OV_GET_SERIAL_ARRAY(real,u1,u1Local);
      OV_GET_SERIAL_ARRAY(real,u2,u2Local);

      // We need the grid velocity from the fluid
      realMappedGridFunction & gv1 = gf1.getGridVelocity(grid1);
      OV_GET_SERIAL_ARRAY(real,gv1,gv1Local);
      
      // For now we do not need the grid velocity from the solid. 
      assert( !domainSolver[d2]->parameters.isMovingGridProblem() );  // solid is assumed NOT to be a moving grid
      // Do this for now:
      realSerialArray & gv2Local = u2Local;
      

      #ifdef USE_PPP
        OV_ABORT("finish me for parallel");

      #endif


	

      real t1=gf1.t;
      real t2=gf2.t;
      if( fabs(t1-t2) > REAL_EPSILON*100.*max(t1,t2) )
      {
	printf("interfaceProjection:WARNING: t1=%9.3e and t2=%9.3e are not the same, t1-t2=%8.2e\n",t1,t2,t1-t2);
      }
      
      // --- Extract the impedance, velocity and stress from both sides ----
      // eps1 = 1/(rho1*c1)   ( rho*c = impedance )

      // check that the variables are in conservative form:
      // assert( gf1.form==GridFunction::conservativeVariables );

      // fluid: 
      const int rc = domainSolver[d1]->parameters.dbase.get<int >("rc");
      const int uc = domainSolver[d1]->parameters.dbase.get<int >("uc");
      const int vc = domainSolver[d1]->parameters.dbase.get<int >("vc");
      const int wc = domainSolver[d1]->parameters.dbase.get<int >("wc");
      const int pc = domainSolver[d1]->parameters.dbase.get<int >("pc");
      const int tc = domainSolver[d1]->parameters.dbase.get<int >("tc");
      const real gamma   = domainSolver[d1]->parameters.dbase.get<real >("gamma");
      const real pOffset = domainSolver[d1]->parameters.dbase.get<real >("boundaryForcePressureOffset");
      
      // solid:
      //const int u1c  = domainSolver[d2]->parameters.dbase.get<int >("u1c");
      //const int u2c  = domainSolver[d2]->parameters.dbase.get<int >("u2c");
      //const int u3c  = domainSolver[d2]->parameters.dbase.get<int >("u3c");
      const int u1c  = domainSolver[d2]->parameters.dbase.get<int >("uc");
      const int u2c  = domainSolver[d2]->parameters.dbase.get<int >("vc");
      const int u3c  = domainSolver[d2]->parameters.dbase.get<int >("wc");

      const int v1c  = domainSolver[d2]->parameters.dbase.get<int >("v1c");
      const int v2c  = domainSolver[d2]->parameters.dbase.get<int >("v2c");
      const int v3c  = domainSolver[d2]->parameters.dbase.get<int >("v3c");
      const int s11c = domainSolver[d2]->parameters.dbase.get<int >("s11c");
      const int s12c = domainSolver[d2]->parameters.dbase.get<int >("s12c");
      const int s13c = domainSolver[d2]->parameters.dbase.get<int >("s13c");
      const int s21c = domainSolver[d2]->parameters.dbase.get<int >("s21c");
      const int s22c = domainSolver[d2]->parameters.dbase.get<int >("s22c");
      const int s23c = domainSolver[d2]->parameters.dbase.get<int >("s23c");
      const int s31c = domainSolver[d2]->parameters.dbase.get<int >("s31c");
      const int s32c = domainSolver[d2]->parameters.dbase.get<int >("s32c");
      const int s33c = domainSolver[d2]->parameters.dbase.get<int >("s33c");

      const real rhos   = domainSolver[d2]->parameters.dbase.get<real >("rho");
      const real lambda = domainSolver[d2]->parameters.dbase.get<real >("lambda");
      const real mu     = domainSolver[d2]->parameters.dbase.get<real >("mu");
      const real cp = sqrt((lambda+2.*mu)/rhos);
      const real eps2 = 1./(rhos*cp);
      
      // Pass pointers to the parameter dbase's so we can look up parameters from Fortran
      DataBase *pdb1 = &domainSolver[d1]->parameters.dbase;
      DataBase *pdb2 = &domainSolver[d2]->parameters.dbase;

      if( t<=dt )
      {
	printF("IP: fluid: t1=%9.3e I1=[%i,%i] I2=[%i,%i] gamma=%5.2f, pOffset=%8.2e, tc=%i pc=%i, \n"
	       "    solid: t2=%9.3e J1=[%i,%i] J2=[%i,%i] rhos=%8.2e, lambda=%8.2e, mu=%8.2e \n",
	       t1,I1.getBase(),I1.getBound(), I2.getBase(),I2.getBound(),gamma,pOffset,tc,pc,
	       t2,J1.getBase(),J1.getBound(), J2.getBase(),J2.getBound(),rhos,lambda,mu);
      }
      
      if( v1c<0 || v2c<0 || s11c<0 || s12c<0 || s21c<0 || s22c<0 )
      {
	printF("Cgmp:interfaceProjection:ERROR: invalid components: v1c=%i v2c=%i s11c=%i s12c=%i s21c=%i s22c=%i\n",
	       v1c,v2c,s11c,s12c,s21c,s22c);
	printF("This routine expects the FOS cgsm solver.\n");
	OV_ABORT("error");
      }

      bool useNewWay=true; // false; // true; 
      
      if( useNewWay )
      {
        // *new* way 

	int n1a=I1.getBase(),n1b=I1.getBound(),
	    n2a=I2.getBase(),n2b=I2.getBound(),
	    n3a=I3.getBase(),n3b=I3.getBound();

	int m1a=J1.getBase(),m1b=J1.getBound(),
	    m2a=J2.getBase(),m2b=J2.getBound(),
	    m3a=J3.getBase(),m3b=J3.getBound();


	bool isRectangular1= mg1.isRectangular();
	real dx1[3]={0.,0.,0.}; //
	if( isRectangular1 )
	  mg1.getDeltaX(dx1);

	bool isRectangular2= mg2.isRectangular();
	real dx2[3]={0.,0.,0.}; //
	if( isRectangular2 )
	  mg2.getDeltaX(dx2);

	if( true )
	{ // for testing -- make rectangular grids look curvilinear ***************************************
	  isRectangular1=false;
	  isRectangular2=false;
	}
	if( !isRectangular1 )
	{
	  mg1.update(MappedGrid::THEinverseVertexDerivative);
	  mg2.update(MappedGrid::THEinverseVertexDerivative);
	}
		  
	assert(isRectangular1==isRectangular2);

	int useForcing = parameters.dbase.get<bool >("twilightZoneFlow");
        assert( parameters.dbase.get<bool >("twilightZoneFlow")==domainSolver[d1]->parameters.dbase.get<bool >("twilightZoneFlow"));
        assert( parameters.dbase.get<bool >("twilightZoneFlow")==domainSolver[d2]->parameters.dbase.get<bool >("twilightZoneFlow"));
	if( useForcing )
	{
	  mg1.update(MappedGrid::THEcenter);
	  mg2.update(MappedGrid::THEcenter);
	}
	
        int orderOfAccuracyInSpace=2; // ** fix me **

	int gridType = isRectangular1 ? 0 : 1;  // ******************************* fix this --> gridType[1,2]
	int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
	int useWhereMask=true;


        // We take the normal from one side the of the interface. normalSign1 and normalSign2
        // are used to flip the sign from one side to the other
        int normalSign1=2*side1-1;    // for an outward normal
	int normalSign2=2*side2-1;

	int materialInterfaceOption=0;  // ** fix me **
	
        int numberOfIterationsForInterfaceBC=1;
	int interfaceInitialized=0;

	int ierr=0;
	int ipar[]={ //
	  side1, dir1, grid1,
	  n1a,n1b,n2a,n2b,n3a,n3b,
	  side2, dir2, grid2,
	  m1a,m1b,m2a,m2b,m3a,m3b,
 	  gridType,            
 	  orderOfAccuracyInSpace,    
 	  orderOfExtrapolation,
 	  useForcing,          
 	  np,
 	  myid,
          normalSign1,
	  normalSign2,
 	  useWhereMask,       
 	  parameters.dbase.get<int >("debug"),
 	  numberOfIterationsForInterfaceBC,
 	  materialInterfaceOption,
	  interfaceInitialized,
          rc,uc,vc,wc,tc, 
          u1c,u2c,u3c,v1c,v2c,v3c,s11c,s12c,s13c,s21c,s22c,s23c,s31c,s32c,s33c,
          option,
          (int)parameters.dbase.get<bool >("twilightZoneFlow"),
          parameters.dbase.get<int >("interfaceProjectionOption"),   // ipar[53]
          parameters.dbase.get<int>("interfaceProjectionGhostOption")
	};
		  
	real rpar[]={ //
	  dx1[0],
	  dx1[1],
	  dx1[2],
	  mg1.gridSpacing(0),
	  mg1.gridSpacing(1),
	  mg1.gridSpacing(2),
	  dx2[0],
	  dx2[1],
	  dx2[2],
	  mg2.gridSpacing(0),
	  mg2.gridSpacing(1),
	  mg2.gridSpacing(2),
	  t,    
	  (real &)domainSolver[d1]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	  (real &)domainSolver[d2]->parameters.dbase.get<OGFunction* >("exactSolution"),  // twilight zone pointer
	  dt,    
          gamma,pOffset, 
          rhos, mu, lambda
	};

	real *u1p=u1Local.getDataPointer();
	real *prsxy1=isRectangular1 ? u1p : mg1.inverseVertexDerivative().getLocalArray().getDataPointer();
	real *pxy1= !useForcing ? u1p : mg1.center().getLocalArray().getDataPointer(); 
	int *mask1p=mg1.mask().getLocalArray().getDataPointer();
        real *gv1p = gv1Local.getDataPointer();

	real *u2p=u2Local.getDataPointer();
	real *prsxy2=isRectangular2 ? u2p : mg2.inverseVertexDerivative().getLocalArray().getDataPointer();
	real *pxy2= !useForcing ? u2p : mg2.center().getLocalArray().getDataPointer(); 
	int *mask2p=mg2.mask().getLocalArray().getDataPointer();
        real *gv2p = gv2Local.getDataPointer();



        // work spaces are not currently used: 
        real rwk[1];
	int iwk[1];
	// assign pointers into the work spaces
	int pa2=0,pa4=0,pa8=0, pipvt2=0,pipvt4=0,pipvt8=0;

	interfaceCnsSm( mg1.numberOfDimensions(), 
			u1Local.getBase(0),u1Local.getBound(0),
			u1Local.getBase(1),u1Local.getBound(1),
			u1Local.getBase(2),u1Local.getBound(2),
			mg1.gridIndexRange(0,0), *u1p, *mask1p,*prsxy1, *pxy1, *gv1p, bc1(0,0), 
			u2Local.getBase(0),u2Local.getBound(0),
			u2Local.getBase(1),u2Local.getBound(1),
			u2Local.getBase(2),u2Local.getBound(2),
			mg2.gridIndexRange(0,0), *u2p, *mask2p,*prsxy2, *pxy2, *gv2p, bc2(0,0), 
			ipar[0], rpar[0], pdb1, pdb2, 
			rwk[pa2],rwk[pa4],rwk[pa8], iwk[pipvt2],iwk[pipvt4],iwk[pipvt8],
			ierr );

      }
      else
      {

	// --- This only works in 2D: 
	j1=J1.getBase(), j2=J2.getBase(), j3=J3.getBase();
	const int dir2p1 = (dir2+1) % mg2.numberOfDimensions();
	FOR_3D(i1,i2,i3,I1,I2,I3)
	{
	  // fluid: 
	  real rhof = u1(i1,i2,i3,rc);
	  real v1f = u1(i1,i2,i3,uc)/rhof;
	  real v2f = u1(i1,i2,i3,vc)/rhof;

	  real ef = u1(i1,i2,i3,tc); // in conservative vars this is E = p/(gamma-1) + .5*rho*v^2 

	  real pf = (gamma-1.)*( ef-.5*rhof*(SQR(v1f)+SQR(v2f)) );    // p 
	  real pf0 = -(pf-pOffset);          // traction = pf0*nf

	
	  real af = sqrt(gamma*pf/rhof);
	  real eps1 = 1./(rhof*af);
      
	  real n1f=-normal1(i1,i2,i3,0), n2f=-normal1(i1,i2,i3,1);  // NOTE: flip sign of fluid normal 
	
	  real vf = n1f*v1f + n2f*v2f; // normal component of the fluid velocity

	  // solid:
	  real v1s = u2(j1,j2,j3,v1c), v2s=u2(j1,j2,j3,v2c);
	  real s11s = u2(j1,j2,j3,s11c), s12s=u2(j1,j2,j3,s12c), s22s=u2(j1,j2,j3,s22c);
	
	  real n1s=normal2(j1,j2,j3,0), n2s=normal2(j1,j2,j3,1);
	  real vs = n1s*v1s + n2s*v2s; // normal component of the solid velocity 

	  // traction is n.sigma:
	  real traction1 = n1s*s11s + n2s*s12s;  
	  real traction2 = n1s*s12s + n2s*s22s;

	  real ps = n1s*traction1+n2s*traction2;  // ps = n.sigma.n 

	  // projected values: 
	  real vi = (eps2*vf  + eps1*vs)/( eps1+eps2 );  // interface velocity 
 
	  real pi = (eps1*pf0 + eps2*ps)/( eps1+eps2 );  // interface "pressure" n.s.n 
	
// 	  printF("IP: fluid: (i1,i2)=(%2i,%2i) n=(%8.2e,%8.2e) (rhof,v1f,v2f,pf)=(%9.3e,%9.3e,%9.3e,%9.3e) eps1=%9.3e af=%9.3e\n"
// 		 "    solid: (j1,j2)=(%2i,%2i) n=(%8.2e,%8.2e) (rhos,v1s,v2s,ps)=(%9.3e,%9.3e,%9.3e,%9.3e) eps2=%9.3e cp=%9.3e -> vi=%9.3e, pi=%9.3e\n"
//		 ,i1,i2,n1f,n2f,rhof,v1f,v2f,pf,eps1,af, j1,j2,n1s,n2s,rhos,v1s,v2s,ps,eps2,cp, vi,pi);

	  // vi = vs;
	  // pi = pf0;
	


	  // -- here is the projection --
	  if( true )
	  {
	    // Set normal component of fluid velocity to be vi:
	    v1f += (vi-vf)*n1f;
	    v2f += (vi-vf)*n2f;
	  
	    // Adjust rhof using: Entropy const : p/rho^gamma = K

	    // real rhofi = rhof;
	    real rhofi = rhof*pow(pi/pf0,1./gamma); // choose interface rho from S=const
	    u1(i1,i2,i3,rc) = rhofi;

	    u1(i1,i2,i3,uc) = v1f*rhofi;
	    u1(i1,i2,i3,vc) = v2f*rhofi;
	
	    real pif = -pi+pOffset;
	    real eif = pif/(gamma-1.)+.5*rhofi*(SQR(v1f)+SQR(v2f));
	    u1(i1,i2,i3,tc)=eif;
	
	    // Set normal component of solid velocity to be vi:
	    u2(j1,j2,j3,v1c) +=  (vi-vs)*n1s;
	    u2(j1,j2,j3,v2c) +=  (vi-vs)*n2s;
        
	    // Assign the stress in the solid: s.n =g = pi n 
	    // In 2d we use the 3 equations:
	    //     n.s.n = n.g = f11
	    //     t.s.n = t.g = f12 
	    //     t.s.t = t.s(old).t = f22  (i.e. do not change this component)
	    real t1s=-n2s, t2s=n1s;  // solid tangent 
	    real f11 = pi, f12=0., f22=t1s*t1s*s11s + 2.*t1s*t2s*s12s + t2s*t2s*s22s; 
	    u2(j1,j2,j3,s11c) = n1s*n1s*f11 + 2.*n1s*t1s      *f12 + t1s*t1s*f22;
	    u2(j1,j2,j3,s12c) = n1s*n2s*f11 +(n1s*t2s+n2s*t1s)*f12 + t1s*t2s*f22;
	    u2(j1,j2,j3,s22c) = n2s*n2s*f11 + 2.*n2s*t2s      *f12 + t2s*t2s*f22;
	
	    u2(j1,j2,j3,s21c) =   u2(j1,j2,j3,s12c);
	  }
	
	
	  jv[dir2p1]++; // increment j1 or j2 
	
	} // end for 3D
      
	
      } // end old way
      

    } // end for face
  } // end for inter
  

  // We need to interpolate the grid function if an interface has interpolation points on it:
  // **** NOTE: I think this is needed because we do not check the mask array when assigning
  //            interface points and thus over-write interpolation points **** FIX ME ***
  ForDomain(d)
  {
    if( interpolateThisDomain[d] )
    {
      if( debug() & 8 )
	printF("\n ++++++++++  Cgmp:interfaceProjection: Interpolate domain %i after assigning the "
	       "interface values +++++++++++++++\n",d);
      GridFunction & gf = domainSolver[d]->gf[gfIndex[d]];
      gf.u.interpolate();
    }
    else
    {
      GridFunction & gf = domainSolver[d]->gf[gfIndex[d]];
      gf.u.periodicUpdate();  // *wdh* 101217 - do this for now
    }
    
  }


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;

  return 0;
}

