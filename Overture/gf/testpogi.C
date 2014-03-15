
#include "Overture.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "display.h"

#include "ParallelOverlappingGridInterpolator.h"

#undef printf

// =========================================================================================
//  Test the Interpolant
// ========================================================================================

bool measureCPU=TRUE;

real
CPU()
// In this version of getCPU we can turn off the timing
{
  if( measureCPU )
    return MPI_Wtime(); // getCPU();
  else
    return 0;
}

real
getError( CompositeGrid & cg,
	  realCompositeGridFunction & u,
	  Range & C,
          OGFunction & exact,
          real & worstError,
          int debug  )
{
  int numberOfComponents=C.getLength();
  real error=0.;
  RealArray componentError(C); componentError=0;
  
  Index I1,I2,I3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const int numberOfGhostPoints = (max(cg[grid].discretizationWidth())-1)/2;
      
    getIndex(cg[grid].indexRange(),I1,I2,I3,numberOfGhostPoints); 

    const realArray & uu = u[grid];
    
    // err.partion(u[grid].getPartition()); err.redim(I1,I2,I3,u[grid].getPartition());
    realArray err; err.partition(uu.getPartition());
    err.redim(uu.dimension(0),uu.dimension(1),uu.dimension(2));

    err=0.;
    real gridErr=0.;
    for( int n=C.getBase(); n<=C.getBound(); n++ )
    {
      where( cg[grid].mask()(I1,I2,I3)!=0 )
      {
	err(I1,I2,I3)=abs(uu(I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n,0.))/max(abs(exact(cg[grid],I1,I2,I3,n,0.)));

        gridErr=max(gridErr,max(err(I1,I2,I3)));    // this could be moved outside where
	componentError(n)=max(componentError(n),gridErr);
	error=max(error,componentError(n));
      }
    }
    if( true || debug & 4 )
    {
      display(uu(I1,I2,I3,C),"getError: u",NULL,"%7.2e ");
      // uu(I1,I2,I3,C).display("getError: u");
      // exact(cg[grid],I1,I2,I3,C.getBase(),0.).display("getError: exact");

      if( Communication_Manager::My_Process_Number==0 ) printf(" *** getError: grid %i gridErr=%e ****\n",grid,gridErr);
      display(err,"getError: Error",NULL,"%6.0e ");
      // err.display("getError: Error");
    }
    
  }
  if( Communication_Manager::My_Process_Number==0 )
  {
    printf("\n >>>>Maximum relative error in interpolating = %e <<<<<<\n\n",error);  
    for( int n=C.getBase(); n<=C.getBound(); n++ )
    {
      printf(" component=%i relative error=%e\n",n,componentError(n));
      worstError=max(worstError,componentError(n));
    }
  }
  
  return error;
}



int 
myDisplayMask( intArray & x, const aString & label )
{
  if( Communication_Manager::My_Process_Number==0 ) printf("%s\n",(const char*)label);
  
  int i3=x.getBase(2);
  for( int i2=x.getBase(1); i2<= x.getBound(1); i2++ )
  {
    if( Communication_Manager::My_Process_Number==0 ) printf("i2=%3i:",i2);
    for( int i1=x.getBase(0); i1<= x.getBound(0); i1++ )
    {
      int value = 0;
      if( x(i1,i2,i3)>0 ) 
	value=1;
      else if( x(i1,i2,i3)<0 )
	value=-1;
      if( Communication_Manager::My_Process_Number==0 ) printf(" %2i ", value );
    }
    if( Communication_Manager::My_Process_Number==0 ) printf("\n");
  }

  return 0;
}


int 
main(int argc, char **argv) 
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0;
  int debug=0; // 7
  
  ParallelOverlappingGridInterpolator::debug=1;

  char buff[80];

  const int maxNumberOfGridsToTest=2;
  int numberOfGridsToTest=1;
  aString gridName[maxNumberOfGridsToTest] =   { "sis.p", "cic.p" };
    
  if( false && argc > 1 )  // picks up machine name in parallel
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( arg(0,6)=="-debug=" )
      {
//         sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
// 	printf("Setting Oges::debug=%i\n",Oges::debug);
//         debug=Oges::debug;
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `testInterpolant [<gridName>] [-noTiming]' \n";

  real worstError=0.;
  for( int it=0; it<numberOfGridsToTest; it++ )
  {
    aString nameOfOGFile=gridName[it];

    cout << "\n *****************************************************************\n";
    cout << " ******** Checking grid: " << nameOfOGFile << " ************ \n";
    cout << " *****************************************************************\n\n";

    CompositeGrid cg;
    getFromADataBase(cg,nameOfOGFile);
    cg.update(MappedGrid::THEmask | MappedGrid::THEvertex );

    if( debug & 4 )
    {
      cg[0].mask().display("mask"); // bug here cannot display on 1 proc. if array has ghost points --> works now?
      const intSerialArray & maskLocal = cg[0].mask().getLocalArrayWithGhostBoundaries();
      maskLocal.display("maskLocal");
    }
    
    
     // Here are some grid functions that we will use to interpolate exposed points
    int numberOfComponents=1;
    Range all;
    realCompositeGridFunction u(cg,all,all,all,numberOfComponents);

//     realCompositeGridFunction w(cg,all,all,all,numberOfComponents);
//      realCompositeGridFunction u;
//      u.link(w,Range(1,3));
    
    // create a twilight-zone function for checking the errors
    OGFunction *exactPointer;
    if( min(abs(cg[0].isPeriodic()(Range(0,cg.numberOfDimensions()-1))-Mapping::derivativePeriodic))==0 )
    {
      // this grid is probably periodic in space, use a trig function
      printf("TwilightZone: trigonometric polynomial\n");
      exactPointer = new OGTrigFunction(2.,2.);  // 2*Pi periodic
    }
    else
    {
      printf("TwilightZone: algebraic polynomial\n");
      int degreeOfSpacePolynomial = 2; // 2;
      int degreeOfTimePolynomial = 1;
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);
    
    }
    OGFunction & exact = *exactPointer;

//     Interpolant interpolant(cg);

//     interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
//     interpolant.setInterpolationMethod(Interpolant::optimized);
    
    ParallelOverlappingGridInterpolator pi;
    pi.setup(u);
  
    const int numberOfComponentGrids=cg.numberOfComponentGrids();
    int grid;

    
/* --------------
    intArray *interpolationPoint=new intArray [numberOfComponentGrids];
    intArray *interpoleeLocation=new intArray [numberOfComponentGrids];
    intArray *interpoleeGrid=new intArray [numberOfComponentGrids];
    intArray *variableInterpolationWidth=new intArray [numberOfComponentGrids];
    realArray *interpolationCoordinates=new realArray [numberOfComponentGrids];
   
    intSerialArray* dimension = new intSerialArray[numberOfComponentGrids];
    
    intSerialArray*indexRange=new intSerialArray [numberOfComponentGrids];
    intSerialArray*isCellCentered= new intSerialArray [numberOfComponentGrids];
    realSerialArray* gridSpacing=new realSerialArray [numberOfComponentGrids];


//     intSerialArray interpolationStartEndIndex;
//     interpolationStartEndIndex.redim(2,numberOfComponentGrids,numberOfComponentGrids); 
//     IntegerArray & ise = cg.interpolationStartEndIndex();
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
//       {
// 	interpolationStartEndIndex(0,grid,grid2)=ise(0,grid,grid2);
// 	interpolationStartEndIndex(1,grid,grid2)=ise(1,grid,grid2);
//       }
    

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      dimension[grid]=cg[grid].dimension();
      indexRange[grid]=cg[grid].indexRange();
      isCellCentered[grid]=cg[grid].isCellCentered();
      gridSpacing[grid]=cg[grid].gridSpacing();
      interpolationPoint[grid]=cg.interpolationPoint[grid];
      interpoleeLocation[grid]=cg.interpoleeLocation[grid];
      interpoleeGrid[grid]=cg.interpoleeGrid[grid];
      variableInterpolationWidth[grid]=cg.variableInterpolationWidth[grid];
      interpolationCoordinates[grid]=cg.interpolationCoordinates[grid];

      interpoleeGrid[grid].display("interpoleeGrid[grid]");
      
    }

    cg.interpolationStartEndIndex().display("cg.interpolationStartEndIndex(),"); // *** why is this the wrong shape
    // cg.interpolationStartEndIndex().reshape(4,numberOfComponentGrids,numberOfComponentGrids); 
    
    pi.setup(cg.numberOfDimensions(),cg.numberOfComponentGrids(),cg.numberOfInterpolationPoints(),
                dimension,
                indexRange,isCellCentered,gridSpacing,cg.interpolationStartEndIndex(),
		interpolationPoint, interpoleeLocation, interpoleeGrid, variableInterpolationWidth,
		interpolationCoordinates,&u,&u);
----------- */

    Index I1,I2,I3;
    real error;
    
    for( int iteration=0; iteration<=0; iteration++ )
    {

      printf("Before exact.assignGridFunction: node=%i\n",Communication_Manager::My_Process_Number);
      exact.assignGridFunction(u);
      printf("After exact.assignGridFunction: node=%i\n",Communication_Manager::My_Process_Number);

      printf("After exact: ERRORS\n");

      Range C(u.getComponentBase(0),u.getComponentBound(0));
      error=getError(cg,u,C,exact,worstError,debug);

      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	getIndex(cg[grid].dimension(),I1,I2,I3); 
	if( debug & 2 && iteration==0 )
	{
	  display(cg.interpolationPoint[grid],sPrintF(buff,"interpolationPoint[%s]",
						      (const char*)cg[grid].getName()),NULL,"%3i ");
	  display(cg.interpoleeLocation[grid],sPrintF(buff,"cg.interpoleeLocation[%s]",
						      (const char*)cg[grid].getName()),"%3i ");
	  display(cg.interpoleeGrid[grid],    sPrintF(buff,"cg.interpoleeGrid[%s]",
						      (const char*)cg[grid].getName()),NULL,"%3i ");
	  display(cg.interpolationCoordinates[grid],sPrintF(buff,"cg.interpolationCoords",
							    (const char*)cg[grid].getName()),NULL,"%3.2f ");
	  printf("\n\n");
	  if( debug & 4 )
	  {
	    intArray & mask = cg[grid].mask();
            myDisplayMask(mask,"mask");
//	    mask.display("mask");
//	    displayMask(cg[grid].mask(),"mask");
          }
	  
	}
  
	where( cg[grid].mask()(I1,I2,I3)<=0 )
	{
	  for( int n=u.getComponentBase(0); n<=u.getComponentBound(0); n++ )
	  {
	    u[grid](I1,I2,I3,n)=-999.;
	  }
	}
	if( cg.numberOfDimensions()==2 )
	{
	  for( int n=0; n<cg.numberOfInterpolationPoints(grid); n++ )
	  {
	    int i1=cg.interpoleeLocation[grid](n,0);
	    int i2=cg.interpoleeLocation[grid](n,1);
            int interpolee=cg.interpoleeGrid[grid](n);
	    intArray & mask = cg[interpolee].mask();
	    if( mask(i1,i2  )==0 || mask(i1+1,i2  )==0 ||  mask(i1+2,i2  )==0 ||
		mask(i1,i2+1)==0 || mask(i1+1,i2+1)==0 ||  mask(i1+2,i2+1)==0 ||
		mask(i1,i2+2)==0 || mask(i1+1,i2+2)==0 ||  mask(i1+2,i2+2)==0 )
	    {
	      printf("****Invalid interpolation **** grid=%i, n=%i, interpolee=%i interpoleeLocation=(%i,%i) \n",
                    grid,n,interpolee,i1,i2);
	    }
	  }
	}
      }
      if( debug & 4 )
	u.display("u before","%7.2f");
  
  
      real time0;
    
      if( iteration==0 )
      {
	C=Range(u.getComponentBase(0),u.getComponentBound(0));
        time0=CPU();
	// u.interpolate();
        printf(" ******* Interpolation should be done here *******\n");
        pi.interpolate(u);

        // MPI_Barrier(MPI_COMM_WORLD);
//         for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//           u[grid].updateGhostBoundaries();

	real time = CPU()-time0;
	printf("Time to interpolate =%e\n",time);
	time0=CPU();
  
        if( false )
	{
//	u.interpolate();
	  pi.interpolate(u);
	  printf(" ******* Interpolate again should be done here *******\n");
        
	  time = CPU()-time0;
	  printf("Time to interpolate again =%e\n",time);
	}
	
      }
      else
      {
	printf("\n\n +++++++++++++++++++ only interpolate component 1 +++++++++++++++++++++++++\n\n");
	C=Range(1,1);
        time0=CPU();
	// u.interpolate(C);
        pi.interpolate(u);
        printf(" ******* Interpolation should be done here *******\n");

	real time = CPU()-time0;
	printf("Time to interpolate =%e\n",time);
      }
    
  
      if( debug & 4 )
	u.display("u after","%7.2f");

      error=getError(cg,u,C,exact,worstError,debug);

    }


  } // end loop over grids.

  printf("\n\n ************************************************************************************************\n");
  if( worstError > .01 )
    printf(" ************** Warning, there is a large error somewhere, worst error =%e ******************\n",
	   worstError);
  else
    printf(" ************** Test apparently successful, worst error =%e ******************\n",worstError);
  printf(" **************************************************************************************************\n\n");

  Overture::finish();          
  return 0;
}
