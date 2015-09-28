#include "Cgmp.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "App.h"
#include "Interface.h"
#include "InterfaceTransfer.h"

#define FOR_4D(i1,i2,i3,i4,I1,I2,I3,I4) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
for(i4=I4Base; i4<=I4Bound; i4++) \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_4(i1,i2,i3,i4,I1,I2,I3,I4) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
for(i4=I4Base; i4<=I4Bound; i4++) \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

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


#define FOR_4IJD(i1,i2,i3,i4,I1,I2,I3,I4,j1,j2,j3,j4,J1,J2,J3,J4) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
int J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase(),  J4Base =J4.getBase();  \
for(i4=I4Base,j4=J4Base; i4<=I4Bound; i4++,j4++) \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)

#define FOR_4IJ(i1,i2,i3,i4,I1,I2,I3,I4,j1,j2,j3,j4,J1,J2,J3,J4) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase(),  I4Base =I4.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(), I4Bound=I4.getBound(); \
J1Base =J1.getBase(),   J2Base =J2.getBase(),  J3Base =J3.getBase(),  J4Base =J4.getBase();  \
for(i4=I4Base,j4=J4Base; i4<=I4Bound; i4++,j4++) \
for(i3=I3Base,j3=J3Base; i3<=I3Bound; i3++,j3++) \
for(i2=I2Base,j2=J2Base; i2<=I2Bound; i2++,j2++) \
for(i1=I1Base,j1=J1Base; i1<=I1Bound; i1++,j1++)


#define interfaceCgCm EXTERN_C_NAME(interfacecgcm)
extern "C"
{
void interfaceCgCm( const int&nd, 
		    const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
		    const int&gridIndexRange1, real&u1, const int&mask1,const real&rsxy1, const real&xy1, 
		    const int&boundaryCondition1, 
		    const int&md1a,const int&md1b,const int&md2a,const int&md2b,const int&md3a,const int&md3b,
		    const int&gridIndexRange2, real&u2, const int&mask2,const real&rsxy2, const real&xy2, 
		    const int&boundaryCondition2,
		    const int&ipar, const real&rpar, 
		    real&aa2, real&aa4, real&aa8, 
		    int&ipvt2, int&ipvt4, int&ipvt8,
		    int&ierr );
}


// ********************** OLD VERSION ****************
//
// ===============================================================================================
/// \brief Assign interface right-hand-side(s) for domain d
//
// for( interface )
//   for( face )
// 
//     Stage I:
//          get data from source domain
//     Stage II:
//          transfer date from source domain to target domain 
//     Stage III:
//          set data on target domain 
//  
//   end (face)
// end( interface )     
// ===============================================================================================
int Cgmp::
assignInterfaceRightHandSideOld( int d, real t, real dt, int correct, std::vector<int> & gfIndex )
{
  real cpu0=getCPU();

  // ************* Much of this code is duplicated from assignInterfaceBoundaryConditions: FIX THIS *********

  if( !gridHasMaterialInterfaces ) return 0;
  
  if( debug() & 2 )
    printF("***  USE OLD assignInterfaceRightHandSide ***\n");

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  
  if( interfaceList.size()==0 )
  {
    // -- Initialize the list of interfaces --
    initializeInterfaces(gfIndex);

    if( interfaceList.size()>0 ) gridHasMaterialInterfaces=true;

    if( interfaceList.size()>0 )
    {
      printF("**** assignInterfaceBoundaryConditions:initializeInterfaces: number of interfaces =%i\n",
	     interfaceList.size());
    }
  }
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  const int numberOfDomains=domainSolver.size();
  int numberOfInterfaceHistoryValuesToSave = parameters.dbase.get<int>("numberOfInterfaceHistoryValuesToSave");
  int numberOfInterfaceIterateValuesToSave = parameters.dbase.get<int>("numberOfInterfaceIterateValuesToSave");
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");
  std::vector<real> & maxResidual = parameters.dbase.get<std::vector<real> >("maxResidual");
  
  // real & omega = parameters.dbase.get<real>("interfaceOmega");

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
  Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  
  Index Iav[4], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2], &Ia4=Iav[3];
  Index Jav[4], &Ja1=Jav[0], &Ja2=Jav[1], &Ja3=Jav[2], &Ja4=Jav[3];
  

  // ************************************************
  // ******* Apply the interface conditions. ********
  // ************************************************

  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // There may be multiple grid faces that lie on the interface:     
    for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
      GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
      
      const int d1=gridDescriptor1.domain, 
                grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, 
                grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );

      // look for an interface on domain d : 
      if( d1!=d && d2!=d ) continue;
      

      GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];

      CompositeGrid & cg1 = gf1.cg;
      assert( grid1>=0 && grid1<=cg1.numberOfComponentGrids());
      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();
      const IntegerArray & interfaceType1 = domainSolver[d1]->parameters.dbase.get<IntegerArray >("interfaceType");
      const intArray & mask1 = mg1.mask();

      CompositeGrid & cg2 = gf2.cg;
      assert( grid2>=0 && grid2<=cg2.numberOfComponentGrids());
      MappedGrid & mg2 = cg2[grid2];
      const IntegerArray & bc2 = mg2.boundaryCondition();
      const IntegerArray & share2 = mg2.sharedBoundaryFlag();
      const IntegerArray & interfaceType2 = domainSolver[d2]->parameters.dbase.get<IntegerArray >("interfaceType");
      const intArray & mask2 = mg2.mask();

      IntegerArray gidLocal1(2,3), dimLocal1(2,3), bcLocal1(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf1.u[grid1],gidLocal1,dimLocal1,bcLocal1 );

      IntegerArray gidLocal2(2,3), dimLocal2(2,3), bcLocal2(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf2.u[grid2],gidLocal2,dimLocal2,bcLocal2 );

      const int numberOfDimensions = cg1.numberOfDimensions();

      const int orderOfAccuracyInSpace=2;  // **** do this for now ***

      #ifdef USE_PPP
        intSerialArray mask1Local; getLocalArrayWithGhostBoundaries(mask1,mask1Local);
        intSerialArray mask2Local; getLocalArrayWithGhostBoundaries(mask2,mask2Local);
      #else
        const intSerialArray & mask1Local = mask1;
        const intSerialArray & mask2Local = mask2;
      #endif

      if( ( debug() & 4 ) && correct==0 )
      {
	printF("Cgmp::assignInterfaceRightHandSide: interface found (t=%e):\n"
	       "  (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i)\n"
	       "  (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i)\n",t,
	       d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
	       d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2));
      }
      

      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,extra);
      getBoundaryIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,extra);
      Ia1=I1, Ia2=I2, Ia3=I3; // save full range 
      Ja1=J1, Ja2=J2, Ja3=J3;
  
      // Check whether the number of points in the tangential directions match 

      // ***THIS CHECK WILL NOT WORK IN GENERAL -- FIX ME ****

      bool pointsOnInterfaceMatch=true;
      for( int dir=1; dir<mg1.numberOfDimensions(); dir++ )
      {
	int dir1p = (dir1+dir) % mg1.numberOfDimensions();
	int dir2p = (dir2+dir) % mg2.numberOfDimensions();
	if( Iv[dir1p].getLength()!=Jv[dir2p].getLength() )
	{
          pointsOnInterfaceMatch=false;  

	  printF("assignInterfaceRightHandSide:INFO: The number of grid points on the two interfaces do not match\n"
	         " (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i) Iv=[%i,%i][%i,%i][%i,%i]\n"
		 " (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
		 d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                 d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2),
                   J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());
	  // Overture::abort("error");
	}
      }


      int includeGhost=0;  // is this right ? 
      bool ok1 = ParallelUtility::getLocalArrayBounds(mask1,mask1Local,I1,I2,I3,includeGhost);
      bool ok2 = ParallelUtility::getLocalArrayBounds(mask2,mask2Local,J1,J2,J3,includeGhost);


      GridFaceDescriptor info1(d1,grid1,side1,dir1);
      GridFaceDescriptor info2(d2,grid2,side2,dir2);

      // Domain da (=d1 or d2) is the domain "d" that we are assigning values to 
      // Domain db (=d1 or d2) is the domain "d" that we are getting values from
      const int da = d==d1 ? d1 : d2;
      const int db = d==d1 ? d2 : d1;
      const bool oka = d==d1 ? ok1 : ok2;
      const bool okb = d==d1 ? ok2 : ok1;
      
      GridFaceDescriptor & gridDescriptora = d==d1 ? gridDescriptor1 : gridDescriptor2;
      GridFaceDescriptor & gridDescriptorb = d==d1 ? gridDescriptor2 : gridDescriptor1;
      
      GridFaceDescriptor & infoa = d==d1 ? info1 : info2;
      GridFaceDescriptor & infob = d==d1 ? info2 : info1;

      RealArray u1,u2;
      RealArray & ua = d==d1 ? u1 : u2;
      RealArray & ub = d==d1 ? u2 : u1;
      info1.u=&u1;
      info2.u=&u2;

      real a0,a1;
      a0=gridDescriptora.a[0];
      a1=gridDescriptora.a[1];

      bool matchFlux= a1!=0.;

      Range Ia[4], Ib[4]; 

      assert( interfaceType1(side1,dir1,grid1) == interfaceType2(side2,dir2,grid2) );

      // -- find out, from the domain we are assigning values to, which interface data it wants:  
      int interfaceDataOptions=0;
      int numDataItems = domainSolver[da]->getInterfaceDataOptions( infoa,interfaceDataOptions );

      Range Ca, Cb;
      if( interfaceType1(side1,dir1,grid1)==Parameters::heatFluxInterface )
      {
        // ********************************************
	// ********** Heat Flux Interface *************
        // ********************************************

	const int tc1 = domainSolver[d1]->parameters.dbase.get<int >("tc");  
	const int tc2 = domainSolver[d2]->parameters.dbase.get<int >("tc");

	real ktc[2]={1.,1.};

	ktc[0] = domainSolver[d1]->parameters.dbase.get<real>("thermalConductivity");
	ktc[1] = domainSolver[d2]->parameters.dbase.get<real>("thermalConductivity");
	if( ktc[0] <=0. || ktc[1]<=0. )
	{
	  printF("assignInterfaceRightHandSide:ERROR: a negative thermal conductivity was found, "
		 "ktc[0]=%e ktc[1]=%e\n",ktc[0],ktc[1]);
	  Overture::abort("error");
	}

	real ktd[2]={-1.,-1.}; //  coefficient of thermal diffusion 
	for( int dd=0; dd<=1; dd++ )
	{
	  const int d = dd==0 ? d1 : d2;
	  if( domainSolver[d]->parameters.dbase.has_key("kappa") )
	  {
	    ktd[dd]=domainSolver[d]->parameters.dbase.get<std::vector<real> >("kappa")[0];
	  } 
	  else if( domainSolver[d]->parameters.dbase.has_key("kThermal") )
	  {
	    ktd[dd]=domainSolver[d]->parameters.dbase.get<real>("kThermal");
	  }
	}

	real ktc1=ktc[0], ktc2=ktc[1];

	Range C1(tc1,tc1), C2(tc2,tc2);
        Iv[3]=C1; Jv[3]=C2;
	if( ok1 ) u1.redim(I1,I2,I3,C1);
	if( ok2 ) u2.redim(J1,J2,J3,C2);

	if( d==d1 )
	{
          Ca=C1; Cb=C2;
	  Ia[0]=I1; Ia[1]=I2; Ia[2]=I3; Ia[3]=Range(tc1,tc1);
	  Ib[0]=J1; Ib[1]=J2; Ib[2]=J3; Ib[3]=Range(tc2,tc2);
	}
	else
	{
          Ca=C2; Cb=C1;
	  Ia[0]=J1; Ia[1]=J2; Ia[2]=J3; Ia[3]=Range(tc2,tc2);
	  Ib[0]=I1; Ib[1]=I2; Ib[2]=I3; Ib[3]=Range(tc1,tc1);
	}
	
        // *wdh* 080717 -- just use:  (?)
        //       infob.a[0]=  gridDescriptora.a[0];
        //       infob.a[1]= -gridDescriptora.a[1];  // flip sign of normal to match 


	// -- fix this for a mixed BC ---
	if( parameters.dbase.get<bool>("useMixedInterfaceConditions") )
	{
          // *note* flip the sign of the normal to match normal from domain da: 
          // *note* flip k1 <--> k2 
          infob.a[0]=gridDescriptora.a[0]; infob.a[1]=-gridDescriptora.a[1]*ktc[db]/ktc[da];
	}
	else if( matchFlux )
	{
	  // *note* flip the sign of the normal to match normal from domain 1

          const real kRatio =  (ktc[0]/ktc[1])*sqrt(ktd[1]/ktd[0]);

	  // if( ktc1 > ktc2 ) // *wdh* 080522 
	  if( kRatio > 1. ) // *wdh* 080522 
	  {
  	    infob.a[0]=0.; infob.a[1]=-ktc2;
	  }
          else
	  {
  	    infob.a[0]=0.; infob.a[1]=-ktc1;
	  }


          // infob.a[1]=-ktc[db];  // **** use this ? **********************************************************************
	  
	}
	else
	{
	  infob.a[0]=1.; infob.a[1]=0.;
	}

        // -- get the data from domain db : (save in infob.u == ub ) --
	if( okb )
          domainSolver[db]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,infob,gridDescriptorb,gfIndex[db],t );

      }
      else if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface )
      {
        // *******************************************
	// ********** Traction Interface *************
        // *******************************************

        // If domain "d" is an elastic-solid, we need to get the surface traction from the other domain
        // If domain "d" is a fluid, we need to get the interface position from the other domain

	// if( domainSolver[da]->getClassName()=="Cgsm" )

	// const int uc1 = domainSolver[d1]->parameters.dbase.get<int >("uc");
	// const int uc2 = domainSolver[d2]->parameters.dbase.get<int >("uc");

	// *wdh* 081201 Range C1(uc1,uc1+numberOfDimensions-1), C2(uc2,uc2+numberOfDimensions-1); // *** fix me *****
        // for now add some extra space: 
	// Range C1(uc1,uc1+2*numberOfDimensions-1), C2(uc2,uc2+2*numberOfDimensions-1); // ** fix me *******
        // *new* 110706: dimension source and target arrays based on numDataItems
        Range C1=numDataItems, C2=numDataItems;
	
        Iv[3]=C1; Jv[3]=C2;
	if( ok1 ) u1.redim(I1,I2,I3,C1);
	if( ok2 ) u2.redim(J1,J2,J3,C2);

        if( ok1 ) u1=0.;
	if( ok2 ) u2=0.;

        // The source domain is db
        // The target domain is da

	if( d==d1 )
	{
          Ca=C1; Cb=C2;
	  Ia[0]=I1; Ia[1]=I2; Ia[2]=I3; Ia[3]=C1;
	  Ib[0]=J1; Ib[1]=J2; Ib[2]=J3; Ib[3]=C2;
	}
	else
	{
          Ca=C2; Cb=C1;
	  Ia[0]=J1; Ia[1]=J2; Ia[2]=J3; Ia[3]=C2;
	  Ib[0]=I1; Ib[1]=I2; Ib[2]=I3; Ib[3]=C1;
	}

        // -- get the data from the source domain db : (save in infob.u == ub ) --
        if( okb )
  	  domainSolver[db]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,infob,
                                                    gridDescriptorb,gfIndex[db],t );

      }
      else
      {
	printF("Cgmp::assignInterfaceRightHandSide:ERROR:unexpected interfaceType=%i\n",
	       interfaceType1(side1,dir1,grid1));
	Overture::abort("error");
      }

      //  --- Copy source array u2 to the target array u1  ---
      //          " u1 <- u2 "
      // Note: the array dimensions of u1 and u2 may not match, even if the grid points match since
      // one boundary may lie on i1=const and the other on i2=const (for exmaple).

      if( !pointsOnInterfaceMatch || parameters.dbase.get<bool>("useNewInterfaceTransfer") )
      // if( !pointsOnInterfaceMatch )
      {
        // **NEW WAY** (not done yet)
        if( interfaceDescriptor.interfaceTransfer==NULL )
	{
          interfaceDescriptor.interfaceTransfer = new InterfaceTransfer;
	}
	
        InterfaceTransfer & interfaceTransfer = *interfaceDescriptor.interfaceTransfer;

	// interfaceTransfer.initialize(interfaceDescriptor,domainSolver,gfIndex,parameters);

	const int numberOfComponentGridsa = d==d1 ? cg1.numberOfComponentGrids() : cg2.numberOfComponentGrids();
	const int numberOfComponentGridsb = d==d1 ? cg2.numberOfComponentGrids() : cg1.numberOfComponentGrids();
        const int grida = d==d1 ? grid1 : grid2;
        const int gridb = d==d1 ? grid2 : grid1;
	
        RealArray **sourceArray = new RealArray * [numberOfComponentGridsb];
        RealArray **targetArray = new RealArray * [numberOfComponentGridsa];
	for( int grid=0; grid<numberOfComponentGridsa; grid++ )
	  targetArray[grid]=NULL;
	for( int grid=0; grid<numberOfComponentGridsb; grid++ )
	  sourceArray[grid]=NULL;
	
        sourceArray[gridb]=&ub;
	targetArray[grida]=&ua;

	interfaceTransfer.transferData( db,da, // domainSource, domainTarget, 
					sourceArray, Cb, // source
					targetArray, Ca,  // target
					interfaceDescriptor,
					domainSolver,
					gfIndex,
					parameters );

	if( false )
	{ // ********* TESTING ***********
          RealArray ua1;
          ua1=ua;

	  ua=0.;
	  if( Ia[0].getLength()==Ib[0].getLength() && 
	      Ia[1].getLength()==Ib[1].getLength() &&
	      Ia[2].getLength()==Ib[2].getLength() )
	  {
	    ua(Ia[0],Ia[1],Ia[2],Ia[3])=ub(Ib[0],Ib[1],Ib[2],Ib[3]);
	  }
	  else if( Ia[0].getLength()==Ib[1].getLength() && 
		   Ia[1].getLength()==Ib[0].getLength() &&
		   Ia[2].getLength()==Ib[2].getLength()  )
	  {
	    int i1,i2,i3,i4,j1,j2,j3,j4;
	    // FOR_3D(i1,i2,i3,Ia[0],Ia[1],Ia[2]) 
	    // {
	    //   for( int i4=Ia[3].getBase(), j4=Ib[3].getBase(); i4<=Ia[3].getBound(); i4++, j4++ )
	    //     ua(i1,i2,i3,i4)=ub(i2,i1,i3,j4);  // note switch of i1,i2 in ub 
	    // }
	    FOR_4IJD(i1,i2,i3,i4,Ia[0],Ia[1],Ia[2],Ia[3],j1,j2,j3,j4,Ib[0],Ib[1],Ib[2],Ib[3])
	    {
	      ua(i1,i2,i3,i4)=ub(j2,j1,j3,j4);  // note switch of j1,j2 in ub 
	    }

	    printF("*** max(fabs(ua-ua1))=%8.2e ***\n",max(fabs(ua-ua1)));
	    
            ::display(ua,"ua from copy (old way)","%6.2f ");
            ::display(ua1,"ua from copy (new way)","%6.2f ");
	    

	  }
	  else
	  {
	    printF("Cgmp::assignInterfaceRightHandSide:ERROR: arrays on interface do not match conformally\n");
	    OV_ABORT("error");
	  }
	}
	
	// OV_ABORT("finish me");
      }
      else
      {
        // **OLD WAY** -- assumes points match on the interface

      #ifdef USE_PPP 

	  // parallel :  we need to transfer the local ub arrays onto the parallel distribution of ua
    
          // **** --- check me ----

          Index D1,D2,D3;

          realArray u2d; 
	  u2d.partition(gf2.u[grid2].getPartition());
          getIndex(mg2.dimension(),D1,D2,D3);
	  u2d.redim(D1,D2,D3,Jv[3]);
          RealArray u2Local; getLocalArrayWithGhostBoundaries(u2d,u2Local);
	  
          realArray u1d;
	  u1d.partition(gf1.u[grid1].getPartition());
          getIndex(mg1.dimension(),D1,D2,D3);
	  u1d.redim(D1,D2,D3,Iv[3]);
          RealArray u1Local; getLocalArrayWithGhostBoundaries(u1d,u1Local);

          Iav[3]=Iv[3]; Jav[3]=Jv[3];

	  if( Ia1.getLength()==Ja1.getLength() && 
	      Ia2.getLength()==Ja2.getLength() &&
	      Ia3.getLength()==Ja3.getLength() )
	  {
	  }
	  else if( Ia1.getLength()==Ja1.getLength() && 
		   Ia2.getLength()==Ja2.getLength() &&
		   Ia3.getLength()==Ja3.getLength()  )
	  {
	    printF("Cgmp::assignInterfaceRightHandSide:ERROR: arrays on interface do not match conformally\n"
                   "  The first two Index's are switched -- finish me for parallel. \n");
	    Overture::abort("error");
	  }
	  else
	  {
	    printF("Cgmp::assignInterfaceRightHandSide:ERROR: arrays on interface do not match conformally\n");
            printF(" Ia1=[%i,%i] Ia2=[%i,%i] Ia3=[%i,%i]  Ja1=[%i,%i] Ja2=[%i,%i] Ja3=[%i,%i] \n",
		   Ia1.getBase(),Ia1.getBound(),Ia2.getBase(),Ia2.getBound(),Ia3.getBase(),Ia3.getBound(), 
		   Ja1.getBase(),Ja1.getBound(),Ja2.getBase(),Ja2.getBound(),Ja3.getBase(),Ja3.getBound());
	    Overture::abort("error");
	  }
	
	  if( d==d1 )
	  {
	    if( ok2) u2Local(Ib[0],Ib[1],Ib[2],Ib[3])=ub(Ib[0],Ib[1],Ib[2],Ib[3]);  
	    CopyArray::copyArray( u1d, Iav, u2d, Jav );  
  	    if( oka ) ua(Ia[0],Ia[1],Ia[2],Ia[3])=u1Local(Ia[0],Ia[1],Ia[2],Ia[3]);
	  }
	  else
	  {
	    if( ok1) u1Local(Ib[0],Ib[1],Ib[2],Ib[3])=ub(Ib[0],Ib[1],Ib[2],Ib[3]);  
	    CopyArray::copyArray( u2d, Jav, u1d, Iav );  
  	    if( oka ) ua(Ia[0],Ia[1],Ia[2],Ia[3])=u2Local(Ia[0],Ia[1],Ia[2],Ia[3]);
	  }
	  


      #else
        // serial 

        ua=0.;
	if( Ia[0].getLength()==Ib[0].getLength() && 
	    Ia[1].getLength()==Ib[1].getLength() &&
	    Ia[2].getLength()==Ib[2].getLength() )
	{
	  ua(Ia[0],Ia[1],Ia[2],Ia[3])=ub(Ib[0],Ib[1],Ib[2],Ib[3]);
	}
	else if( Ia[0].getLength()==Ib[1].getLength() && 
		 Ia[1].getLength()==Ib[0].getLength() &&
		 Ia[2].getLength()==Ib[2].getLength()  )
	{
	  int i1,i2,i3,i4,j1,j2,j3,j4;
	  // FOR_3D(i1,i2,i3,Ia[0],Ia[1],Ia[2]) 
	  // {
	  //   for( int i4=Ia[3].getBase(), j4=Ib[3].getBase(); i4<=Ia[3].getBound(); i4++, j4++ )
	  //     ua(i1,i2,i3,i4)=ub(i2,i1,i3,j4);  // note switch of i1,i2 in ub 
	  // }
	  FOR_4IJD(i1,i2,i3,i4,Ia[0],Ia[1],Ia[2],Ia[3],j1,j2,j3,j4,Ib[0],Ib[1],Ib[2],Ib[3])
	  {
            ua(i1,i2,i3,i4)=ub(j2,j1,j3,j4);  // note switch of j1,j2 in ub 
	  }
	  
	}
	else
	{
	  printF("Cgmp::assignInterfaceRightHandSide:ERROR: arrays on interface do not match conformally\n");
	  OV_ABORT("error");
	}
       #endif

      }  // end old way 
      
      
      if( debug() & 8 )
      {
	::display(ub,"RHS ub (from get)");
	::display(ua,"RHS ua (=ub) (for set)");
      }
	

      // *********************************************************
      // *********** Extrapolate the initial guess ***************
      // *********************************************************


      bool extrapolateFirstGuess=parameters.dbase.get<bool>("extrapolateInitialInterfaceValues");
      if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface ||
          interfaceType2(side2,dir2,grid2)==Parameters::tractionInterface )
      {
        // we currently don't extrapolate for traction interfaces
	extrapolateFirstGuess=false;
      }
      

      // We should only need to extrapolate for one side the interface: 
      //   Extrapolate the side with the lower domain number -- this is assumed to appear first

      extrapolateFirstGuess = extrapolateFirstGuess && (d1<d2 ? d==d1 : d==d2);

      if( extrapolateFirstGuess && correct==0 )
      {
	// We have saved both u and k*u.n from previous times (for TZ we have saved u-ue, k*u.n - k*ue.n ).
        // We can extrapolate, in time, the RHS for the interface condition

        //  If we are solving the interface condition by iteration: (j= iteration number)
        //    a*k1*u.n[j+1] + b*u[j+1] = a*k2*v.n[j] + b*v[j]  

        // As a first guess we can use
        //    a*k1*u.n[0] + b*u[0] = 2.*f(t-dt) - f(t-2*dt) 
        // where
        //    f(t) = a*k2*v.n(t) + b*v(t)
        // OR
        //    f(t) = a*k1*u.n(t) + b*u(t)   <- this is better to use for parallel
       
	// a history of interface values at past times: 
        // **** For parallel: I think we can use gridDescriptora here and switch sign of a1 below ****
	// ** InterfaceDataHistory & idh = gridDescriptorb.interfaceDataHistory;   // values from the opposite side of the interface
 
        // consistency check: we currently don't extrapolate for traction interfaces
        assert( interfaceType1(side1,dir1,grid1)!=Parameters::tractionInterface );
        assert( interfaceType2(side2,dir2,grid2)!=Parameters::tractionInterface );
	

	InterfaceDataHistory & idh = gridDescriptora.interfaceDataHistory;   // values from the same side of the interface

	if( idh.interfaceDataList.size()>=2 )
	{
	  const int prev = ( idh.current -1 + numberOfInterfaceHistoryValuesToSave ) % 
	    numberOfInterfaceHistoryValuesToSave;
	    
	  real tc = idh.interfaceDataList[idh.current].t;
	  real tp = idh.interfaceDataList[prev       ].t;
	    
	    
	  // extrap in time: f(t) = (t-t1)/(t2-t1)*u2 + (t2-t)/(t2-t1)*u1

	  // 2nd order extrap in time:
	  real cex1=2., cex2=-1.;
	  real dtex = tc-tp;
	  if( dtex > dt*.1 )
	  { // adjust for variable time step (if dtex is not too small)
	    cex1=(t-tp)/dtex; cex2=(tc-t)/dtex;
	  }
	  if( debug() & 2 )
	    fPrintF(interfaceFile," +++interfaceRHS: interface %i: domain d=%i, t=%9.3e extrap RHS in time from "
		    " tc=%8.2e tp=%8.2e cex=(%8.2e,%8.2e)\n",
		    inter,d,t,tc,tp,cex1,cex2);
	    
	  if( oka )
	  {
	    // new way 
	    RealArray & uc = idh.interfaceDataList[idh.current].u;
	    RealArray & fc = idh.interfaceDataList[idh.current].f;
	    RealArray & up = idh.interfaceDataList[prev       ].u;
	    RealArray & fp = idh.interfaceDataList[prev       ].f;

            // const real a0 = gridDescriptora.a[0], a1=-gridDescriptora.a[1];  // note: flip sign of the normal
	    // ua(Ia[0],Ia[1],Ia[2],Ia[3]) = ( cex1*( a0*uc(Ib[0],Ib[1],Ib[2],Ib[3]) + a1*fc(Ib[0],Ib[1],Ib[2],Ib[3]) ) + 
	    // 				    cex2*( a0*up(Ib[0],Ib[1],Ib[2],Ib[3]) + a1*fp(Ib[0],Ib[1],Ib[2],Ib[3]) ) );

            const real a0 = gridDescriptora.a[0], a1=gridDescriptora.a[1]; 
	    ua(Ia[0],Ia[1],Ia[2],Ia[3]) = ( cex1*( a0*uc(Ia[0],Ia[1],Ia[2],Ia[3]) + a1*fc(Ia[0],Ia[1],Ia[2],Ia[3]) ) + 
					    cex2*( a0*up(Ia[0],Ia[1],Ia[2],Ia[3]) + a1*fp(Ia[0],Ia[1],Ia[2],Ia[3]) ) );

	  }

// 	  else
// 	  {
// 	    // old way
//             InterfaceDataHistory & idh = gridDescriptora.interfaceDataHistory;   
// 	    RealArray & uc = matchFlux ? idh.interfaceDataList[idh.current].f : idh.interfaceDataList[idh.current].u;
// 	    RealArray & up = matchFlux ? idh.interfaceDataList[prev       ].f : idh.interfaceDataList[prev       ].u;
// 	    ua(Ia[0],Ia[1],Ia[2],Ia[3]) = cex1*uc(Ia[0],Ia[1],Ia[2],Ia[3])+cex2*up(Ia[0],Ia[1],Ia[2],Ia[3]);
// 	  }
	  
	}
      }

      // *********************************************************
      // ************** Under-relaxed iteration ******************
      // *********************************************************

      // ***** NOTE: should we relax the first time through ??? --> I don't think so.

      const bool & relaxCorrectionSteps = parameters.dbase.get<bool>("relaxCorrectionSteps");
      bool relaxPredictor=false;
      bool underRelaxGuess=false;
      if( parameters.dbase.get<bool>("useMixedInterfaceConditions") )
      { // with mixed-conditions we under-relax the RHS for one side of the interface
	underRelaxGuess= d==d1;
      }
      else if( matchFlux )
      { // for D-N conditions we under-relax the Neumann condition
	underRelaxGuess=true;
      }
      if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface ||
          interfaceType2(side2,dir2,grid2)==Parameters::tractionInterface )
      {
        // we sometimes under-relax for traction interfaces
	if(  domainSolver[d]->getClassName()=="Cgsm" )
	{
	  underRelaxGuess=relaxCorrectionSteps;
	  relaxPredictor=true;
	  if( underRelaxGuess )
	    fPrintF(interfaceFile,"--MP-- tractionInterface: under-relax traction values, domain d=%i, correct=%i\n",
                d,correct);
	}
	
      }      


      // *wdh* 080722 -- only relax for correct > 0 for conjugate-heat-transfer
      if( underRelaxGuess  && (relaxPredictor || correct>0) ) 
      {
        // We have saved both u and k*u.n from previous times. These can be used to under-relax the 
        // iteration. If we are solving the interface condition by iteration: (j= iteration number)
        //    a*k1*u.n[j+1] + b*u[j+1] = a*k2*v.n[j] + b*v[j]  
        // The the relaxed iteration is 
        //    a*k1*u.n[j+1] + b*u[j+1] = omega*( a*k2*v.n[j] + b*v[j] ) + (1-omega)*( a*k1*u.n[j] + b*u[j] )
        // If omega=1 : no-relaxation. If omega=0, u[j+1] = u[j] 

        // consistency check: we currently don't under-relax for traction interfaces
        // assert( interfaceType1(side1,dir1,grid1)!=Parameters::tractionInterface );
        // assert( interfaceType2(side2,dir2,grid2)!=Parameters::tractionInterface );


	// interface values at past iterates for the current time:
	InterfaceDataHistory & idi  = gridDescriptora.interfaceDataIterates;
	InterfaceDataHistory & idib = gridDescriptorb.interfaceDataIterates;

	if( idi.interfaceDataList.size()>=1 )
	{
	  real tp = idi.interfaceDataList[idi.current].t;
          real & omega = interfaceDescriptor.interfaceOmega;
	  if( omega != 1. )
	  {
	    if( debug() & 4 )
	      fPrintF(interfaceFile,"+++interfaceRHS: interface %i: relax flux RHS, t=%9.3e, omega=%5.2f "
		      "(data iterates: current=%i,tp=%9.3e)\n",inter,t,omega,idi.current,tp);


	    if( interfaceType1(side1,dir1,grid1)==Parameters::heatFluxInterface )
	    {
	      // new way 
	      RealArray & up = idi.interfaceDataList[idi.current].u; 
	      RealArray & fp = idi.interfaceDataList[idi.current].f; 

	      real a0 = gridDescriptora.a[0], a1=gridDescriptora.a[1];  
              // fp already includes the factor of k so we need to divide a1 by this amount
              real ktca = domainSolver[da]->parameters.dbase.get<real>("thermalConductivity");
              a1 = a1/ktca;
	      if( oka )
	      {
		ua(Ia[0],Ia[1],Ia[2],Ia[3]) = ( omega*ua(Ia[0],Ia[1],Ia[2],Ia[3]) + 
			      (1.-omega)*( a0*up(Ia[0],Ia[1],Ia[2],Ia[3]) + a1*fp(Ia[0],Ia[1],Ia[2],Ia[3]) ) );
	      }
	    }
            else if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface )
	    {
              // --- NOTE: we get f from the other side!!

              // ******* FIX ME ************
              // NOTE: We save tractions in f
	      RealArray & uPrev = idib.interfaceDataList[idi.current].f;   // current iterate

              InterfaceDataHistory & idh  = gridDescriptorb.interfaceDataHistory; // Note "b"
              RealArray & uOld = idh.interfaceDataList[idh.current].f;          // previous time values 

              // RealArray & up = correct==0 ? uOld : uPrev;
              RealArray & up = uPrev;
	      
	      // Range Sc(numberOfDimensions,2*numberOfDimensions-1);  // traction (stress) components are stored here (traction-rates are first)
	      Range Sc(0,2*numberOfDimensions-1);  // traction-rates and traction (stress) 
	      
	      printF("--MP-AIRHS-- relax traction on interface, d=%i, correct=%i, omega=%6.3f, t=%9.3e\n",
		     d,correct,omega,t);
	      if( true )
	      {
		fprintf(interfaceFile,"--MP-AIRHS-- relax traction on interface, d=%i, correct=%i, omega=%6.3f,"
			" t=%9.3e (inter=%i, face=%i)\n", d,correct,omega,t,inter,face);
		if( correct<=1 )
		{
		  ::display(up(I1,I2,I3,Range(1,3,2))," up : Traction + traction-rate OLD",interfaceFile,"%9.3e ");
		  ::display(ua(I1,I2,I3,Range(1,3,2))," ua : Traction + traction-rate NEW",interfaceFile,"%9.3e ");
		}
	      }
	      
	      if( false )
	      {
		Range all;
		::display(ua(I1,I2,I3,Sc),"ua");
		::display(up(I1,I2,I3,Sc),"up");
		real maxDiff = max(fabs(ua(I1,I2,I3,Sc)-up(I1,I2,I3,Sc)));
		printF("       max-diff=%8.2e\n",maxDiff);
		OV_ABORT("stop here for now");
	      }
	      real maxDiff = max(fabs(ua(I1,I2,I3,Sc)-up(I1,I2,I3,Sc)));
	      printF("       max-diff=%8.2e\n",maxDiff);

              // // -------------- Assign interface residual here ?? ----------------
              // // *FIX ME* use a relative measure
	      if( face==0 )
	       	maxResidual[inter]=maxDiff;
	      else
	       	maxResidual[inter]=max(maxResidual[inter],maxDiff);

	      if( oka )
	      {
		ua(I1,I2,I3,Sc) = omega*ua(I1,I2,I3,Sc) + (1.-omega)*up(I1,I2,I3,Sc);
	      }
	    }
	    
// 	    else
// 	    {
// 	      // old way 
// 	      RealArray & up = matchFlux ? idi.interfaceDataList[idi.current].f : idi.interfaceDataList[idi.current].u; 
// 	      if( debug() & 8 )
// 	      {
// 		::display(ua(Ia[0],Ia[1],Ia[2],Ia[3]),"ua",interfaceFile,"%8.2e ");
// 		::display(up(Ia[0],Ia[1],Ia[2],Ia[3]),"up",interfaceFile,"%8.2e ");
// 	      }
	  
// 	      ua(Ia[0],Ia[1],Ia[2],Ia[3]) = omega*ua(Ia[0],Ia[1],Ia[2],Ia[3]) + (1.-omega)*up(Ia[0],Ia[1],Ia[2],Ia[3]);
//	    }
	  }
	}
      }

      if( false )
      {
	RealArray & ua = *infoa.u;
	::display(ua,"****OLD: targetArray: ua ");
      }


      // --- Set the data for interface da (values in infoa.u == ua ) ---
      infoa.a[0]=a0; infoa.a[1]= a1;  // These are used with "set" for TZ
      if( oka )
        domainSolver[da]->interfaceRightHandSide( setInterfaceRightHandSide,interfaceDataOptions,infoa,gridDescriptora,gfIndex[da],t );

      
    } // end for face
  } // end for inter


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;
  return 0;
  
}


// =======================================================================================================
// OLD version -- only for interfaces with a single face
// =======================================================================================================
int Cgmp::
getInterfaceResidualsOld( real t, real dt, std::vector<int> & gfIndex, std::vector<real> & maxResidual,
                       InterfaceValueEnum saveInterfaceValues /* =doNotSaveInterfaceValues */ )
{


  if( t<2.*dt )
    printF("\n****** Entering *OLD* getInterfaceResidualsOld ********\n");

  real cpu0=getCPU();

  // ************* Much of this code is duplicated from assignInterfaceBoundaryConditions: FIX THIS *********

  if( !gridHasMaterialInterfaces ) return 0;

  InterfaceList & interfaceList = parameters.dbase.get<InterfaceList>("interfaceList");
  assert( interfaceList.size()!=0 );

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int & myid = parameters.dbase.get<int>("myid");
  
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
  FILE *& interfaceFile =parameters.dbase.get<FILE* >("interfaceFile");

  const int numberOfDomains=domainSolver.size();

  int numberOfInterfaceHistoryValuesToSave = parameters.dbase.get<int>("numberOfInterfaceHistoryValuesToSave");
  int numberOfInterfaceIterateValuesToSave = parameters.dbase.get<int>("numberOfInterfaceIterateValuesToSave");

  Index Iv[4], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2], &I4=Iv[3];
  Index Jv[4], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2], &J4=Jv[3];
  Index Kv[4], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2], &K4=Kv[3];
  
  Index Iav[4], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2], &Ia4=Iav[3];
  Index Jav[4], &Ja1=Jav[0], &Ja2=Jav[1], &Ja3=Jav[2], &Ja4=Jav[3];
  

  // ************************************************
  // ******* Check the Interfaces            ********
  // ************************************************

  if( maxResidual.size() < interfaceList.size() )
    maxResidual.resize(interfaceList.size(),0.);


  // loop over interfaces
  for( int inter=0; inter < interfaceList.size(); inter++ )
  {
    InterfaceDescriptor & interfaceDescriptor = interfaceList[inter]; 

    // maxResidual[inter]=0.;
    
    if( interfaceDescriptor.gridListSide1.size()>1 || interfaceDescriptor.gridListSide2.size()>1  )
    {
      maxResidual[inter]=1.;  // do this for now 
      printF("getInterfaceResiduals:WARNING: NOT computing the residual on the interface for multi-FACE interface. FINISH ME ***\n");
      continue;
    }


    // There may be multiple grid faces that lie on the interface:     
    for( int face=0; face<interfaceDescriptor.gridListSide1.size(); face++ )
    {
      GridFaceDescriptor & gridDescriptor1 = interfaceDescriptor.gridListSide1[face];
      GridFaceDescriptor & gridDescriptor2 = interfaceDescriptor.gridListSide2[face];
      
      const int d1=gridDescriptor1.domain, 
                grid1=gridDescriptor1.grid, side1=gridDescriptor1.side, dir1=gridDescriptor1.axis;
      const int d2=gridDescriptor2.domain, 
                grid2=gridDescriptor2.grid, side2=gridDescriptor2.side, dir2=gridDescriptor2.axis;

      assert( d1>=0 && d1<numberOfDomains && d2>=0 && d2<numberOfDomains );


      GridFunction & gf1 = domainSolver[d1]->gf[gfIndex[d1]];
      GridFunction & gf2 = domainSolver[d2]->gf[gfIndex[d2]];

      CompositeGrid & cg1 = gf1.cg;
      assert( grid1>=0 && grid1<=cg1.numberOfComponentGrids());
      MappedGrid & mg1 = cg1[grid1];
      const IntegerArray & bc1 = mg1.boundaryCondition();
      const IntegerArray & share1 = mg1.sharedBoundaryFlag();
      const IntegerArray & interfaceType1 = domainSolver[d1]->parameters.dbase.get<IntegerArray >("interfaceType");
      const intArray & mask1 = mg1.mask();
    
      CompositeGrid & cg2 = gf2.cg;
      assert( grid2>=0 && grid2<=cg2.numberOfComponentGrids());
      MappedGrid & mg2 = cg2[grid2];
      const IntegerArray & bc2 = mg2.boundaryCondition();
      const IntegerArray & share2 = mg2.sharedBoundaryFlag();
      const IntegerArray & interfaceType2 = domainSolver[d2]->parameters.dbase.get<IntegerArray >("interfaceType");
      const intArray & mask2 = mg2.mask();

      IntegerArray gidLocal1(2,3), dimLocal1(2,3), bcLocal1(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf1.u[grid1],gidLocal1,dimLocal1,bcLocal1 );

      IntegerArray gidLocal2(2,3), dimLocal2(2,3), bcLocal2(2,3);
      ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( gf2.u[grid2],gidLocal2,dimLocal2,bcLocal2 );

      const int numberOfDimensions = cg1.numberOfDimensions();

      const int orderOfAccuracyInSpace=2;  // **** do this for now ***

      if( debug() & 8 )
      {
	printF("Cgmp::getInterfaceResiduals: interface found (t=%e):\n"
	       "  (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i)\n"
	       "  (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i)\n",t,
	       d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
	       d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2));
      }
      

      const int extra=0; // orderOfAccuracyInSpace/2;
      getBoundaryIndex(mg1.gridIndexRange(),side1,dir1,I1,I2,I3,extra);
      getBoundaryIndex(mg2.gridIndexRange(),side2,dir2,J1,J2,J3,extra);
      Ia1=I1, Ia2=I2, Ia3=I3; // save full range 
      Ja1=J1, Ja2=J2, Ja3=J3;
      
  
      #ifdef USE_PPP
        intSerialArray mask1Local; getLocalArrayWithGhostBoundaries(mask1,mask1Local);
        intSerialArray mask2Local; getLocalArrayWithGhostBoundaries(mask2,mask2Local);
      #else
        const intSerialArray & mask1Local = mask1;
        const intSerialArray & mask2Local = mask2;
      #endif
	

      // check that the number of points in the tangential directions match -- eventually we will fix this
      bool pointsOnInterfaceMatch=true;
      for( int dir=1; dir<mg1.numberOfDimensions(); dir++ )
      {
	int dir1p = (dir1+dir) % mg1.numberOfDimensions();
	int dir2p = (dir2+dir) % mg2.numberOfDimensions();
	if( Iv[dir1p].getLength()!=Jv[dir2p].getLength() )
	{
          pointsOnInterfaceMatch=false; 
	  printF("getInterfaceResiduals:ERROR: The number of grid points on the two interfaces do not match\n"
	         " (d1,grid1,side1,dir1,bc1)=(%i,%i,%i,%i,%i) Iv=[%i,%i][%i,%i][%i,%i]\n"
		 " (d2,grid2,side2,dir2,bc2)=(%i,%i,%i,%i,%i) Jv=[%i,%i][%i,%i][%i,%i]\n",
		 d1,grid1,side1,dir1,mg1.boundaryCondition(side1,dir1),
                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                 d2,grid2,side2,dir2,mg2.boundaryCondition(side2,dir2),
                   J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound());
	  // Overture::abort("error");
	}
      }

      if( !pointsOnInterfaceMatch )
      {
	maxResidual[inter]=1.;  // do this for now 
        printF("getInterfaceResiduals:WARNING: NOT computing the residual on the interface for non-matching. FINISH ME ***\n");
        continue;
      }
      


      int includeGhost=0;  // is this right ? 
      bool ok1 = ParallelUtility::getLocalArrayBounds(mask1,mask1Local,I1,I2,I3,includeGhost);
      bool ok2 = ParallelUtility::getLocalArrayBounds(mask2,mask2Local,J1,J2,J3,includeGhost);

     
      assert( interfaceType1(side1,dir1,grid1) == interfaceType2(side2,dir2,grid2) );


      if( interfaceType1(side1,dir1,grid1)==Parameters::heatFluxInterface )
      {
        // ********************************************
	// ********** Heat Flux Interface *************
        // ********************************************


	const int tc1 = domainSolver[d1]->parameters.dbase.get<int >("tc");    
	const int tc2 = domainSolver[d2]->parameters.dbase.get<int >("tc");


	real ktc[2]={1.,1.};

	ktc[0] = domainSolver[d1]->parameters.dbase.get<real>("thermalConductivity");
	ktc[1] = domainSolver[d2]->parameters.dbase.get<real>("thermalConductivity");
	if( ktc[0] <=0. || ktc[1]<=0. )
	{
	  printF("getInterfaceResiduals:ERROR: a negative thermal conductivity was found, "
		 "ktc[0]=%e ktc[1]=%e\n",ktc[0],ktc[1]);
	  Overture::abort("error");
	}
	real ktc1=ktc[0], ktc2=ktc[1];

	// **** finish this ****
      
	GridFaceDescriptor info2(d2,grid2,side2,dir2);
	// info2.component=tc2;
	Range N2(tc2,tc2);
	RealArray u2;
        if( ok2 ) u2.redim(J1,J2,J3,N2);
	info2.u=&u2;

	GridFaceDescriptor info1(d1,grid1,side1,dir1);
	// info1.component=tc1;
	Range N1(tc1,tc1);
	RealArray u1;
        if( ok1 ) u1.redim(I1,I2,I3,N1);
	info1.u=&u1;

       
	// --- Evaluate [u] ---
        int interfaceDataOptions=Parameters::heatFluxInterfaceData;
	info1.a[0]=1.; info1.a[1]=0.;
        if( ok1 )
  	  domainSolver[d1]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info1,gridDescriptor1,gfIndex[d1],t );

	info2.a[0]=1.; info2.a[1]=0.;
        if( ok2 )
  	  domainSolver[d2]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info2,gridDescriptor2,gfIndex[d2],t );
	
      


	if( saveInterfaceValues==saveInterfaceTimeHistoryValues ||
	    saveInterfaceValues==saveInterfaceIterateValues )
	{ // *** save the interface solution values  ****
	  for( int iface=0; iface<=1; iface++ )
	  {
	    GridFaceDescriptor & gfd = iface==0 ? gridDescriptor1 : gridDescriptor2;
	    RealArray & ui = iface==0 ? u1 : u2;
	    InterfaceDataHistory & idh = 
	      (saveInterfaceValues==saveInterfaceTimeHistoryValues ? gfd.interfaceDataHistory
	       : gfd.interfaceDataIterates);
	
	    const int numToSave = 
	      (saveInterfaceValues==saveInterfaceTimeHistoryValues ? numberOfInterfaceHistoryValuesToSave :
	       numberOfInterfaceIterateValuesToSave );

	    if( idh.interfaceDataList.size()<numToSave )
	    {
	      idh.interfaceDataList.push_back(InterfaceData());
	      idh.current = idh.interfaceDataList.size()-1;
	    }
	    else
	    {
	      idh.current = (idh.current+1) % numToSave;
	    }
	    if( debug() & 4 )
	    {
	      if( saveInterfaceValues==saveInterfaceTimeHistoryValues )
	      {
		fPrintF(interfaceFile,"interfaceRes: interface %i (face=%i): save interface history data, "
                       "t=%9.2e, current=%i\n",inter,iface,t,idh.current);
	      }
	      else
	      {
		fPrintF(interfaceFile,"interfaceRes: interface %i (face=%i): save interface iterate data, "
                       "t=%9.2e, current=%i\n",inter,iface,t,idh.current);
	      }
	    }
	    
	    InterfaceData & id = idh.interfaceDataList[idh.current];
	    id.t=t;
	    id.u=ui;
	  }
	
	}
      


	if( false )
	{
	  ::display(u1(I1,I2,I3,N1) ,"interface-residual u1 for [u]");
	  ::display(u2(J1,J2,J3,N2) ,"interface-residual u2 for [u]");
	}
      
	// *fix this* for mask , parallel, etc.
	// real jumpInU = max(fabs(u1(I1,I2,I3,N1)-u2(J1,J2,J3,N2)));

        int i1,i2,i3,i4, j1,j2,j3,j4;
        real jumpInU=0.;

        #ifdef USE_PPP

	  // parallel :  we need to transfer the local u2 arrays onto the parallel distribution of u1
          // 
          //     Allocate distributed arrays u22d(mg2),  u21d(mg1)
          //        u22Local = u2; 
          //        copyArray u22d -> u21d;
          //        Use u21Local instead or u2 
          //        
    
          realArray u22d; 
	  u22d.partition(gf2.u[grid2].getPartition());
          Index D1,D2,D3;
          getIndex(mg2.dimension(),D1,D2,D3);
	  u22d.redim(D1,D2,D3,N2);
	  
          RealArray u22Local; getLocalArrayWithGhostBoundaries(u22d,u22Local);
          if( ok2 ) u22Local(J1,J2,J3,N2)=u2(J1,J2,J3,N2);  // fill the boundary of u22d 
	  
          realArray u21d;
	  u21d.partition(gf1.u[grid1].getPartition());
          getIndex(mg1.dimension(),D1,D2,D3);
	  u21d.redim(D1,D2,D3,N1);

          Iv[3]=N1; Jv[3]=N2;
          Iav[3]=N1; Jav[3]=N2;
          // copy the boundary of u22d to u21d :  u21d(I1,I2,I3,I4) = u22d(J1,J2,J3,J4) (requires communication)
	  CopyArray::copyArray( u21d, Iav, u22d, Jav );  
 
          RealArray u21Local; getLocalArrayWithGhostBoundaries(u21d,u21Local);
          
          int J1Base, J2Base, J3Base, J4Base; 
	  if( ok1 )
	  {
	    FOR_4D(i1,i2,i3,i4,I1,I2,I3,I4)
	    {
	      if( mask1Local(i1,i2,i3) > 0 )
	      {
		jumpInU=max(jumpInU,fabs(u1(i1,i2,i3,i4)-u21Local(i1,i2,i3,i4)));
	      }
	    }
	  }
	  jumpInU=ParallelUtility::getMaxValue(jumpInU);
	  
        #else
          // serial case:           

          FOR_4IJD(i1,i2,i3,i4,I1,I2,I3,N1,j1,j2,j3,j4,J1,J2,J3,N2)
	  {
            if( mask1Local(i1,i2,i3) > 0 )
	    {
	      jumpInU=max(jumpInU,fabs(u1(i1,i2,i3,i4)-u2(j1,j2,j3,j4)));
	    }
	  }
	#endif


	// --- Evaluate [ktc*u.n] ---
	info1.a[0]=0.; info1.a[1]= ktc1;
	if( ok1 )
  	  domainSolver[d1]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info1,gridDescriptor1,gfIndex[d1],t );

	info2.a[0]=0.; info2.a[1]= ktc2;  // do not flip sign here (since we may save u2 below)
        if( ok2 )
  	  domainSolver[d2]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info2,gridDescriptor2,gfIndex[d2],t );

	if( saveInterfaceValues==saveInterfaceTimeHistoryValues ||
	    saveInterfaceValues==saveInterfaceIterateValues )
	{ // *** save the interface RHS values  ****
	  for( int iface=0; iface<=1; iface++ )
	  {
	    GridFaceDescriptor & gfd = iface==0 ? gridDescriptor1 : gridDescriptor2;
	    RealArray & ui = iface==0 ? u1 : u2;
	    InterfaceDataHistory & idh = 
	      (saveInterfaceValues==saveInterfaceTimeHistoryValues ? gfd.interfaceDataHistory
	       : gfd.interfaceDataIterates);
	
	    InterfaceData & id = idh.interfaceDataList[idh.current];
	    id.t=t;
	    id.f=ui;
	  }
	
	}

	//  *note* flip the sign of the normal to match normal from domain 1
        if( ok2 ) 
          u2(J1,J2,J3,N2)=-u2(J1,J2,J3,N2);

        real jumpInUn=0.;

        #ifdef USE_PPP

	  // parallel :  we need to transfer the local u2 arrays onto the parallel distribution of u1
    
          if( ok2 ) 
            u22Local(J1,J2,J3,N2)=u2(J1,J2,J3,N2);  // fill the boundary of u22d 
	  
          // copy the boundary of u22d to u21d :  u21d(I1,I2,I3,I4) = u22d(J1,J2,J3,J4) (requires communication)
	  CopyArray::copyArray( u21d, Iav, u22d, Jav );  
 
          // *** NOTE: for now we do not check the consistency of the mask's ***** fix me !
	  if( ok1 )
	  {
	    FOR_4D(i1,i2,i3,i4,I1,I2,I3,I4)
	    {
	      if( mask1Local(i1,i2,i3) > 0 )
	      {
		jumpInUn=max(jumpInUn,fabs(u1(i1,i2,i3,i4)-u21Local(i1,i2,i3,i4)));
	      }
	    }
	  }
	  jumpInUn=ParallelUtility::getMaxValue(jumpInUn);

        #else
          // serial case:           

          // do not check the residual on end-pts with adjacent dirichlet BC's   *** fix me **
          // int dir1p1 = (dir1+1) % mg1.numberOfDimensions();
          // int dir2p1 = (dir2+1) % mg2.numberOfDimensions();
	  // Iv[dir1p1]=Range(Iv[dir1p1].getBase()+1,Iv[dir1p1].getBound()-1);
	  // Jv[dir2p1]=Range(Jv[dir2p1].getBase()+1,Jv[dir2p1].getBound()-1);
	  

          FOR_4IJ(i1,i2,i3,i4,I1,I2,I3,N1,j1,j2,j3,j4,J1,J2,J3,N2)
	  {
            // the mask's must both be positive at the same points : 
            if(  int(mask1Local(i1,i2,i3)>0) + int(mask2(j1,j2,j3)>0) == 1 )
	    {
	      printF("Cgmp::getInterfaceResiduals: mask's do not agree!\n"
		     " The interface equation solvers currently require the masks on opposite sides of\n"
		     " the interface to agree\n");
	      printF(" domain1=%i (%s) grid1=%i (%s) mask1(%i,%i,%i)=%i\n"
		     " domain2=%i (%s) grid2=%i (%s) mask2(%i,%i,%i)=%i\n",
		     d1,(const char*)domainSolver[d1]->getName(),grid1,(const char*)mg1.getName(),i1,i2,i3,mask1Local(i1,i2,i3), 
		     d2,(const char*)domainSolver[d2]->getName(),grid2,(const char*)mg2.getName(),j1,j2,j3,mask2(j1,j2,j3) );
	      Overture::abort("error");
	    }
	  
	  
	    if( mask1Local(i1,i2,i3) > 0 )
	    {
	      jumpInUn=max(jumpInUn,fabs(u1(i1,i2,i3,i4)-u2(j1,j2,j3,j4)));
	    }
	  }

        #endif

	if( false )
	{
	  ::display(u1(I1,I2,I3,N1) ,"interface-residual: nu1*u1.n");
	  ::display(u2(J1,J2,J3,N2) ,"interface-residual: nu2*u2.n");
	}

        real interfaceResidual=max(jumpInU,jumpInUn);
	if( debug() & 2 )
	{
	  fPrintF(interfaceFile,
		  "interface %i step=%i: residuals: [u]=%8.2e [k*u.n]=%8.2e (omega=%9.3e,tol=%8.2e)\n",
		  inter,parameters.dbase.get<int >("globalStepNumber"),jumpInU,jumpInUn,
                  parameters.dbase.get<real>("interfaceOmega"),interfaceDescriptor.interfaceTolerance);
	}

	if( face==0 )
	  maxResidual[inter]=interfaceResidual;
        else
  	  maxResidual[inter]=max(maxResidual[inter],interfaceResidual);
      
      }
      else if( interfaceType1(side1,dir1,grid1)==Parameters::tractionInterface )
      {
        // *******************************************
	// ********** Traction Interface *************
        // *******************************************

        // For now we do not compute residuals, just save a time history 
	if( !(saveInterfaceValues==saveInterfaceTimeHistoryValues ||  parameters.dbase.get<bool>("relaxCorrectionSteps") ) )
	{
	  continue;
	}
 
	// --- Evaluate the interface data ---

	// const int uc1 = domainSolver[d1]->parameters.dbase.get<int >("uc");
	// const int uc2 = domainSolver[d2]->parameters.dbase.get<int >("uc");

	// *wdh* 081201 Range C1(uc1,uc1+numberOfDimensions-1), C2(uc2,uc2+numberOfDimensions-1); // ************ fix me **************
        // for now add some extra space: 
	// Range C1(uc1,uc1+2*numberOfDimensions-1), C2(uc2,uc2+2*numberOfDimensions-1); // ************ fix me **************
        Range C1, C2;

	GridFaceDescriptor info2(d2,grid2,side2,dir2);
	RealArray u2;
        // if( ok2 ) u2.redim(J1,J2,J3,C2);
	info2.u=&u2;

	GridFaceDescriptor info1(d1,grid1,side1,dir1);
	RealArray u1;
        // if( ok1 ) u1.redim(I1,I2,I3,C1);
	info1.u=&u1;

	int interfaceDataOptions=0;

	if( ok1 )
	{
	  // find out from domain d2 what interface data it wants: 
	  int numDataItems=domainSolver[d2]->getInterfaceDataOptions( info2,interfaceDataOptions );
          C1=numDataItems;
	  u1.redim(I1,I2,I3,C1);

          // evaluate the interface data:
	  domainSolver[d1]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info1,gridDescriptor1,gfIndex[d1],t );
	}
      
        if( ok2 )
	{
	  // find out from domain d1 what interface data it wants:
	  int numDataItems=domainSolver[d1]->getInterfaceDataOptions( info1,interfaceDataOptions );
          C2=numDataItems;
	  u2.redim(J1,J2,J3,C2);

          // evaluate the interface data:
  	  domainSolver[d2]->interfaceRightHandSide( getInterfaceRightHandSide,interfaceDataOptions,info2,gridDescriptor2,gfIndex[d2],t );
	}
	
	if( saveInterfaceValues==saveInterfaceTimeHistoryValues ||
	    saveInterfaceValues==saveInterfaceIterateValues )
	{ 
          // *** save the interface RHS values  ****

	  if( true || debug() & 4 )
	  {
	    if( saveInterfaceValues==saveInterfaceTimeHistoryValues )
	      fPrintF(interfaceFile,"Cgmp::getInterfaceResiduals: saving time history of traction: interface=%i, t=%9.3e\n",inter,t);
            else
	      fPrintF(interfaceFile,"Cgmp::getInterfaceResiduals: saving interface iterate of traction: interface=%i, t=%9.3e\n",inter,t);
	  }
	  
	  for( int iface=0; iface<=1; iface++ )
	  {
	    GridFaceDescriptor & gfd = iface==0 ? gridDescriptor1 : gridDescriptor2;
	    RealArray & ui = iface==0 ? u1 : u2;
	    InterfaceDataHistory & idh = 
	      (saveInterfaceValues==saveInterfaceTimeHistoryValues ? gfd.interfaceDataHistory : gfd.interfaceDataIterates);
	
	    const int numToSave = 
	      (saveInterfaceValues==saveInterfaceTimeHistoryValues ? numberOfInterfaceHistoryValuesToSave : numberOfInterfaceIterateValuesToSave );

	    // *wdh* 2015/08/26 -- overwrite time history if value are for the same time. THIS IS PROBABLY NOT NEEDED FOR ITERATES
            bool overwriteHistory=false;
	    if( idh.interfaceDataList.size()>0 )
	    {
	      InterfaceData & idCurrent = idh.interfaceDataList[idh.current];
	      if( FALSE && idCurrent.t ==t )
	      {
		overwriteHistory=true;
	      }
	    }
	    
	    if( !overwriteHistory )
	    {
	      if( idh.interfaceDataList.size()<numToSave )
	      { // add a new entry: 
		idh.interfaceDataList.push_back(InterfaceData());
		idh.current = idh.interfaceDataList.size()-1;
	      }
	      else
	      { // over-write oldest entry: 
		idh.current = (idh.current+1) % numToSave;
	      }
	    }
	    
	    InterfaceData & id = idh.interfaceDataList[idh.current];
	    id.t=t;
	    id.f=ui;

	    if( true )
	    {
	      aString label = saveInterfaceValues==saveInterfaceTimeHistoryValues ? "HISTORY" : "ITERATE";
	      aString buff;
	      ::display(id.f,sPrintF(buff," SAVE %s - INTERFACE TRACTION DATA current=%i t=%9.3e [domain %s] numToSave=%i overwrite=%i",
				     (const char*)label,
				     idh.current,id.t,
				     (iface==0 ? (const char*)(domainSolver[d1]->getClassName()) : (const char*)(domainSolver[d2]->getClassName())), 
				     numToSave,(int)overwriteHistory),
			interfaceFile,"%9.3e ");
	    }
	    

	  }
	
	} // end saveInterfaceValues

      }
      else
      {
	printF("Cgmp::getInterfaceResiduals:ERROR:unexpected interfaceType=%i\n",interfaceType1(side1,dir1,grid1));
	OV_ABORT("error");
      }


      
    } // end for face
  } // end for inter

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterfaces"))+=getCPU()-cpu0;

  return 0;
}

