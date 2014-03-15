// ============================================================================================
//  Test routine for extrapolate interpolation neighbours
//
// Examples:
//   mpirun -np 1 testExtrapInterpNeighbours -g=cice2.order2 -useNew=1
//   mpirun -np 1 testExtrapInterpNeighbours -g=sise2.order2 -useNew=1
//   mpirun -np 1 testExtrapInterpNeighbours -g=bibe -useNew=1
//   mpirun -np 1 testExtrapInterpNeighbours -g=sibe1.order2 -useNew=1
//   mpirun -np 1 testExtrapInterpNeighbours -g=cicSplit -useNew=1
//   mpirun -np 1 testExtrapInterpNeighbours -g=cylBoxe1.order2 -useNew=1 
//   mpirun -np 1 testExtrapInterpNeighbours -g=plateWith24Holese2.order2 -useNew=1
//   mpirun -np 2 testExtrapInterpNeighbours -g=plate3dWith24Holese2.order2 -useNew=1
//   srun -N1 -n1 -ppdebug testExtrapInterpNeighbours -g=plateWith24Holese2.order2 -useNew=1
//   srun -N1 -n4 -ppdebug testExtrapInterpNeighbours -g=plate3dWith24Holese4.order2 -useNew=1
// 
// Good parallel tests for new version:
//   mpirun -np 3 testExtrapInterpNeighbours -g=cice2.order2 -useNew=1
//   mpirun -np 3 testExtrapInterpNeighbours -g=sibe1.order2 -useNew=1
// Trouble: (grid with interior boundary points : likely some extrapolation is prevented -- see BoundaryOperators.C
//          around line 849).
//   mpirun -np 1 testExtrapInterpNeighbours -g=pipes -useNew=1
// Trouble with old version:
//   mpirun -np 2 testExtrapInterpNeighbours -g=sibe1.order2 -numParallelGhost=3 -useNew=1
//
// After running use:
//   smartDiff.p tein.dp.check tein.dp.check.new
// ============================================================================================


#include "Overture.h"
#include "CompositeGridOperators.h"
#include "NameList.h"
#include "OGTrigFunction.h"
#include "OGPolyFunction.h"
#include "display.h"
#include "ParallelUtility.h"
#include "Checker.h"

// *new way* Include this for now: 
#include "AssignInterpNeighbours.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )
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



bool measureCPU=TRUE;

real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return getCPU();
  else
    return 0;
}

//================================================================================
//  **** Test routine for extrapolate interpolation neighbours *****
//================================================================================

int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture
 
  fflush(0);
  Communication_Manager::Sync();

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int maxNumberOfGridsToTest=4;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "sise2.order2", "cice2.order2", "cicSplit", "sibe1.order2" };
  int degreex=1;
  int debug=0;
  AssignInterpNeighbours::debug=0;

  int numParallelGhost=2;
  int useNew=0;

  if( argc > 1 )
  { 
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      aString line = argv[i];
      if( line=="-noTiming" )
        measureCPU=FALSE;
      else if( len=line.matches("-degreex=") )
      {
        sScanF(line(len,line.length()-1),"%i",&degreex);
	printF("Setting degree of polynomial to %i\n",degreex);
      }
      else if( len=line.matches("-debug=") )
      {
        sScanF(line(len,line.length()-1),"%i",&debug);
	printF("Setting debug=%i\n",debug);
        AssignInterpNeighbours::debug=debug;
      }
      else if( len=line.matches("-numParallelGhost=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numParallelGhost);
	printF("Setting numParallelGhost=%i\n",numParallelGhost);
      }
      else if( len=line.matches("-useNew=") )
      {
        sScanF(line(len,line.length()-1),"%i",&useNew);
	printF("Setting useNew=%i\n",useNew);
      }
      else if( len=line.matches("-g=") )
      {
	numberOfGridsToTest=1;
	gridName[0]=line(len,line.length()-1);
      }
      else
      {
	printF("testExtrapInterpNeighbours:ERROR: unknown arg=[%s]\n",(const char*)line);
      }
    }
  }
  else
    printF("Usage: `testExtrapInterpNeighbours -g=<name> -useNew=[0|1] -noTiming -degreex=<> "
           "-numParallelGhost=<>' \n");


  #ifdef USE_PPP
    // On Parallel machines always add at least this many ghost lines on local arrays
    MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);
  #endif

  aString checkFileName;
  if( REAL_EPSILON == DBL_EPSILON )
    checkFileName="tein.dp.check.new";  // double precision
  else  
    checkFileName="tein.sp.check.new";
  Checker checker(checkFileName);  // for saving a check file.
  real cutOff = REAL_EPSILON == DBL_EPSILON ? 1.e-12 : 1.e-5;
  checker.setCutOff(cutOff);

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
  Index Ipv[3], &Ip1=Ipv[0], &Ip2=Ipv[1], &Ip3=Ipv[2];

  real worstError=0.;

  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    fflush(0);
    Communication_Manager::Sync();

    aString nameOfOGFile=gridName[it];
    checker.setLabel(nameOfOGFile,0);

    printF("\n *****************************************************************\n"
           " ******** Checking grid: %s ************ \n"
	   " *****************************************************************\n\n",(const char*)nameOfOGFile);

    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEmask );

    // make some shorter names for readability
    BCTypes::BCNames 
      dirichlet                  = BCTypes::dirichlet,
      neumann                    = BCTypes::neumann,
      mixed                      = BCTypes::mixed,
      extrapolate                = BCTypes::extrapolate,
      normalComponent            = BCTypes::normalComponent,
      extrapolateNormalComponent = BCTypes::extrapolateNormalComponent,
      extrapolateTangentialComponent0 = BCTypes::extrapolateTangentialComponent0,
      extrapolateTangentialComponent1 = BCTypes::extrapolateTangentialComponent1,
      aDotU                      = BCTypes::aDotU,
      normalDotScalarGrad        = BCTypes::normalDotScalarGrad,
      generalizedDivergence      = BCTypes::generalizedDivergence,
      generalMixedDerivative     = BCTypes::generalMixedDerivative,
      aDotGradU                  = BCTypes::aDotGradU,
      vectorSymmetry             = BCTypes::vectorSymmetry,
      tangentialComponent        = BCTypes::tangentialComponent,
      tangentialComponent0       = BCTypes::tangentialComponent0,
      tangentialComponent1       = BCTypes::tangentialComponent1,
      normalDerivativeOfNormalComponent = BCTypes::normalDerivativeOfNormalComponent,
      normalDerivativeOfTangentialComponent0 = BCTypes::normalDerivativeOfTangentialComponent0,
      normalDerivativeOfTangentialComponent1 = BCTypes::normalDerivativeOfTangentialComponent1,
      allBoundaries              = BCTypes::allBoundaries,
      boundary1                  = BCTypes::boundary1; 

    // define an exact solution for testing
    // each component is a different polynomial of degree "degreex"
    int degreeSpace = degreex;
    int degreeTime = 1;
    int numberOfComponents = 1; // cg.numberOfDimensions();
    OGPolyFunction exact(degreeSpace,cg.numberOfDimensions(),numberOfComponents,degreeTime);

    RealArray spatialCoefficientsForTZ(6,6,6,numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(6,numberOfComponents);      
    timeCoefficientsForTZ=0.;
    int n;
    for( n=0; n<numberOfComponents; n++ )
    {
      real ni =1./(n+1);
      spatialCoefficientsForTZ(0,0,0,n)=1.;      
      if( degreeSpace>0 )
      {
	spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,0,1,n)= cg.numberOfDimensions()==3 ? .25*ni : 0.;
      }
      if( degreeSpace>1 )
      {
	spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,0,2,n)= cg.numberOfDimensions()==3 ? .125*ni : 0.;
	spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
	spatialCoefficientsForTZ(1,0,1,n)=-.125*ni;
	spatialCoefficientsForTZ(0,1,1,n)=.25*ni;
      }
      if( degreeSpace>2 )
      {
	spatialCoefficientsForTZ(3,0,0,n)=-.5*ni;
	spatialCoefficientsForTZ(0,3,0,n)=-.25*ni;
	spatialCoefficientsForTZ(0,0,3,n)= cg.numberOfDimensions()==3 ? -.125*ni : 0.;
        spatialCoefficientsForTZ(1,2,0,n)=-.125*ni;
        spatialCoefficientsForTZ(2,1,0,n)=.25*ni;
        spatialCoefficientsForTZ(0,1,2,n)=.125*ni;
        spatialCoefficientsForTZ(0,2,1,n)=-.25*ni;
        spatialCoefficientsForTZ(1,0,2,n)=.125*ni;
        spatialCoefficientsForTZ(2,0,1,n)=-.25*ni;
      }
      if( degreeSpace>3 )
      {
	spatialCoefficientsForTZ(4,0,0,n)=.25*ni;
	spatialCoefficientsForTZ(0,4,0,n)=.125*ni;
	spatialCoefficientsForTZ(0,0,4,n)= cg.numberOfDimensions()==3 ? .25*ni : 0.;

	spatialCoefficientsForTZ(2,2,0,n)=.125*ni;
	spatialCoefficientsForTZ(2,0,2,n)=-.25*ni;
	spatialCoefficientsForTZ(0,2,2,n)=.125*ni;
	spatialCoefficientsForTZ(3,1,0,n)=.25*ni;
	spatialCoefficientsForTZ(1,0,3,n)=-.25*ni;
	spatialCoefficientsForTZ(0,3,1,n)=.125*ni;
	spatialCoefficientsForTZ(1,3,0,n)=.25*ni;
      }
      if( degreeSpace>4 )
      {
	spatialCoefficientsForTZ(5,0,0,n)=.125*ni;
	spatialCoefficientsForTZ(0,5,0,n)=-.125*ni;
	spatialCoefficientsForTZ(0,0,5,n)= cg.numberOfDimensions()==3 ? .125*ni : 0.;
      }
    }
    for( n=0; n<numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
    }
    exact.setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 

    Range all;
    realCompositeGridFunction ue(cg,all,all,all,numberOfComponents); // holds exact solution
    exact.assignGridFunction(ue);


    real error=0.;
    real time,time1,time2;
    aString buff;
    
    CompositeGridOperators cgop(cg);

    // loop over all component grids
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
//      cout << "+++++++Checking component grid = " << grid << "+++++++" << endl;

      MappedGrid & mg = cg[grid]; 
      checker.setLabel(mg.getName(),1);
      // checker.setLabel("",2);
      // checker.setLabel("",3);



      realMappedGridFunction u(mg);

#ifdef USE_PPP
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray ueLocal; getLocalArrayWithGhostBoundaries(ue[grid],ueLocal);
#else
      const intSerialArray & maskLocal = mg.mask();
      realSerialArray & uLocal = u;
      realSerialArray & ueLocal = ue[grid];
#endif


      int component=0;

      // MappedGridOperators operators(mg);                     // define some differential operators
      MappedGridOperators & operators=cgop[grid];
      u.setOperators( operators );                           // Tell u which operators to use

      // ****************************************************************
      //       extrapolateInterpolationNeighbours
      // ****************************************************************

      // Note: the extendedIndexRange includes ghost lines on bc==0 boundaries
      // getIndex(mg.extendedIndexRange(),I1,I2,I3,1);  // include 1 ghost line
      getIndex(mg.dimension(),I1,I2,I3); 

//       if( debug & 1 )
//       {
// 	::display(mg.extendedIndexRange(),"mg.extendedIndexRange()");
// 	::display(mg.gridIndexRange(),"mg.gridIndexRange()");
//       }
      

      int includeGhost=1; // we must assign true solution on parallel ghost below
      bool ok = ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,I1,I2,I3,includeGhost);

      uLocal=99.;
      if( ok )
      {
	where( maskLocal(I1,I2,I3)!=0 )
	  uLocal(I1,I2,I3,0)=ueLocal(I1,I2,I3,0);
      }
      
      // u.display("u before extrapolateInterpolationNeighbour");
//       time=CPU();
//       u.applyBoundaryCondition(component,BCTypes::extrapolateInterpolationNeighbours);
//       time1=CPU()-time;


      

      BoundaryConditionParameters extrapParams;
      extrapParams.orderOfExtrapolation=3;
      Range C=numberOfComponents;

      if( (bool)useNew )
      {
	// new way 
	// printF(" *********** Use AssignInterpNeighbours ***************\n");
	

	AssignInterpNeighbours ain;
	ain.setInterpolationPoint( cg.interpolationPoint[grid] );

        time1=CPU();
	ain.assign( u, C, extrapParams );
        time1=CPU()-time1;

        // Call again for timing without initialization
        time=CPU();
        ain.assign( u, C, extrapParams );
        time=CPU()-time;
      }
      else
      {
        time1=CPU();
	u.applyBoundaryCondition(component,BCTypes::extrapolateInterpolationNeighbours);
        time1=CPU()-time1;

        time=CPU();
	u.applyBoundaryCondition(component,BCTypes::extrapolateInterpolationNeighbours);
        time=CPU()-time;

      }
      


 
      // cicSplit illustrates a hard case: Discr. pt A needs point B (5x5 stencil)
      //  
      //       E--I--D--D--D--       D=discretization pt, 
      //       E--I--D--D--D--       I=interp pt
      //       E--I--A--D--D--       E=mask=0, extrap. interp. neighbour
      //       E--2--2--2--2--       2=ghost pt with mask>0
      //       B--2--2--2--2--




      // extrap 2nd ghost line extended
      extrapParams.ghostLineToAssign=2;
      extrapParams.orderOfExtrapolation=3;
      extrapParams.extraInTangentialDirections=2;

      real t=0.;
      u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);

      // u.updateGhostBoundaries(); // this is called in finishBoundaryConditions (fixBoundaryCorners)
      u.finishBoundaryConditions();

      // u.display("u after extrapolateInterpolationNeighbour");


      error=0.;
      // these next checks assume there are 2 ghost !!

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      includeGhost=0;
      ok = ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,I1,I2,I3,includeGhost);

//       if( debug & 2  && mg.numberOfDimensions()==2 )
//       {
// 	displayMask(maskLocal,sPrintF(" mask on grid=%i",grid));
//       }

      if( ok )
      {
        Index J1,J2,J3=I3;
	int i1,i2,i3, j1,j2,j3;
	if( mg.numberOfDimensions()==2 )
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
            int i1p=i1+1, i1m=i1-1, i2p=i2+1, i2m=i2-1;
            // look for a discretization pt that is next to an interp. pt: 
	    if( maskLocal(i1,i2,i3)>0 && 
 		( maskLocal(i1m,i2m,i3 )<0 || maskLocal(i1 ,i2m,i3 )<0 || maskLocal(i1p,i2m,i3 )<0 ||
 		  maskLocal(i1m,i2 ,i3 )<0 ||                             maskLocal(i1p,i2 ,i3 )<0 ||
 		  maskLocal(i1m,i2p,i3 )<0 || maskLocal(i1 ,i2p,i3 )<0 || maskLocal(i1p,i2p,i3 )<0 ) )
	    {
	      // This is a valid discretization point -- check the 5 point stencil for unused pts:
	      J1=Range(i1-2,i1+2);
	      J2=Range(i2-2,i2+2);
	      FOR_3D(j1,j2,j3,J1,J2,J3)
	      {
		if( maskLocal(j1,j2,j3)==0 )
		{
		  // this unused point (j1,j2,j3) is needed 
		  real err = fabs( uLocal(j1,j2,j3,0)-ueLocal(j1,j2,j3,0) );
		  error=max(error,err);
		  if( true && err>1. )
		  {
		    printf(" TEIN: myid=%i Error is large for neighbour pt j=(%i,%i,%i) "
                           "mask(j)=%i u=%12.6e ue=%12.6e err=%8.2e\n",
			   myid,j1,j2,j3,maskLocal(j1,j2,j3),uLocal(j1,j2,j3,0),ueLocal(j1,j2,j3,0),err);
                    int maski = maskLocal(i1,i2,i3);
		    maski = maski>0 ? 1 : (maski<0 ? -1 : 0);
                    printf("     : grid=%i, disc. pt i=(%i,%i,%i) mask(i)=%i\n",grid,i1,i2,i3,maski);

		  }
		}
	      }
	    }
	  }
	}
	else
	{
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
            int i1p=i1+1, i1m=i1-1, i2p=i2+1, i2m=i2-1, i3p=i3+1, i3m=i3-1;
            // look for a discretization pt that is next to an interp. pt: 
	    if( maskLocal(i1,i2,i3)>0 && 
 		( maskLocal(i1m,i2m,i3m)<0 || maskLocal(i1 ,i2m,i3m)<0 || maskLocal(i1p,i2m,i3m)<0 ||
 		  maskLocal(i1m,i2 ,i3m)<0 || maskLocal(i1 ,i2 ,i3m)<0 || maskLocal(i1p,i2 ,i3m)<0 ||
 		  maskLocal(i1m,i2p,i3m)<0 || maskLocal(i1 ,i2p,i3m)<0 || maskLocal(i1p,i2p,i3m)<0 ||
 		  maskLocal(i1m,i2m,i3 )<0 || maskLocal(i1 ,i2m,i3 )<0 || maskLocal(i1p,i2m,i3 )<0 ||
 		  maskLocal(i1m,i2 ,i3 )<0 ||                             maskLocal(i1p,i2 ,i3 )<0 ||
 		  maskLocal(i1m,i2p,i3 )<0 || maskLocal(i1 ,i2p,i3 )<0 || maskLocal(i1p,i2p,i3 )<0 ||
 		  maskLocal(i1m,i2m,i3p)<0 || maskLocal(i1 ,i2m,i3p)<0 || maskLocal(i1p,i2m,i3p)<0 ||
 		  maskLocal(i1m,i2 ,i3p)<0 || maskLocal(i1 ,i2 ,i3p)<0 || maskLocal(i1p,i2 ,i3p)<0 ||
 		  maskLocal(i1m,i2p,i3p)<0 || maskLocal(i1 ,i2p,i3p)<0 || maskLocal(i1p,i2p,i3p)<0 ) )
	    {
	      // This is a valid discretization point -- check the 5 point stencil for unused pts:
	      J1=Range(i1-2,i1+2);
	      J2=Range(i2-2,i2+2);
	      J3=Range(i3-2,i3+2);
              bool notOk=false;
	      FOR_3D(j1,j2,j3,J1,J2,J3)
	      {
		if( maskLocal(j1,j2,j3)==0 )
		{
		  // this unused point (j1,j2,j3) is needed 
		  real err = fabs( uLocal(j1,j2,j3,0)-ueLocal(j1,j2,j3,0) );
		  error=max(error,err);
		  if( true && err>1. )
		  {
                    notOk=true;
		    printF(" TEIN: Error is large for neighbour pt j=(%i,%i,%i) mask(j)=%i u=%12.6e ue=%12.6e err=%8.2e\n",
			   j1,j2,j3,maskLocal(j1,j2,j3),uLocal(j1,j2,j3,0),ueLocal(j1,j2,j3,0),err);
                    int maski = maskLocal(i1,i2,i3);
		    maski = maski>0 ? 1 : (maski<0 ? -1 : 0);
                    printF("     : grid=%i, disc. pt i=(%i,%i,%i) mask(i)=%i\n",grid,i1,i2,i3,maski);
		  }
		}
	      }
	      if( notOk )
	      {
		printf("mask near i=(%i,%i,%i)\n",i1,i2,i3);
		FOR_3D(j1,j2,j3,J1,J2,J3)
		{
		  int maski = maskLocal(j1,j2,j3);
		  maski = maski>0 ? 1 : (maski<0 ? -1 : 0);
		  printf(" %2i ",maski);
		  if( j1==i1+2 ) 
		  {
		    printf("\n");
		    if( j2==i2+2 ) printf("\n");
		  }
		}
	      }

	    }
	  }
	}
	
      }
      error=ParallelUtility::getMaxValue(error);
      worstError=max(worstError,error);
      // printF("Maximum error in extrapolateInterpolationNeighbours= %8.2e, cpu=%8.2e \n",error,time);   
      checker.printMessage("extrapInterpNeighbours",error,time,time1);

      getIndex(mg.extendedIndexRange(),I1,I2,I3); // reset
      
    } // end for grid 
    

  }  // loop over all component grids
    

  printF("\n\n ************************************************************************************************\n");
  if( worstError > 1. )
    printF(" ************** Warning, there is a large error somewhere, worst error =%8.2e ******************\n",
	   worstError);
  else
    printF(" ************** Test apparently successful, worst error =%8.2e ******************\n",worstError);
  printF(" **************************************************************************************************\n\n");
    
  fflush(0);
  Communication_Manager::Sync();

  Overture::finish();          

  return 0;

}
