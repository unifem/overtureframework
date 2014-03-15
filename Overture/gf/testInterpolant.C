#include "Overture.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "display.h"
#include "Oges.h"
#include "CompositeGridOperators.h"

// =========================================================================================
//  Test the Interpolant
// ========================================================================================
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

    RealArray err(I1,I2,I3);
    err=0.;
    real gridErr=0.;
    for( int n=C.getBase(); n<=C.getBound(); n++ )
    {
      where( cg[grid].mask()(I1,I2,I3)!=0 )
      {
	err=abs(u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n,0.))/max(abs(exact(cg[grid],I1,I2,I3,n,0.)));
        gridErr=max(gridErr,max(err));
	componentError(n)=max(componentError(n),gridErr);
	error=max(error,componentError(n));
      }
    }
    if( debug & 4 )
      display(u[grid](I1,I2,I3,C),"u",NULL,"%7.2e ");
    if( debug & 4 )
    {
      printf(" *** grid %i gridErr=%e ****\n",grid,gridErr);
      display(err,"Error",NULL,"%6.0e ");
    }
    
  }
  printf("\n >>>>Maximum relative error in interpolating = %e <<<<<<\n\n",error);  
  for( int n=C.getBase(); n<=C.getBound(); n++ )
  {
    printf(" component=%i relative error=%e\n",n,componentError(n));
    worstError=max(worstError,componentError(n));
  }

  return error;
}


int 
main(int argc, char **argv) 
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0;
  int debug=0;
  Oges::debug=0;
  
  char buff[80];

  const int maxNumberOfGridsToTest=3;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "cic", "cicCC", "sib" };
    
  if( argc > 1 )
  { 
    for( int i=1; i<argc; i++ )
    {
      aString arg = argv[i];
      if( arg=="-noTiming" )
        measureCPU=FALSE;
      else if( arg(0,6)=="-debug=" )
      {
        sScanF(arg(7,arg.length()-1),"%i",&Oges::debug);
	printf("Setting Oges::debug=%i\n",Oges::debug);
        debug=Oges::debug;
      }
      else
      {
	numberOfGridsToTest=1;
	gridName[0]=argv[i];
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
    cg.update();

  
  // Here are some grid functions that we will use to interpolate exposed points
    int numberOfComponents=3; // 4;
    Range all;
//realCompositeGridFunction u(cg,all,all,all,numberOfComponents);

    realCompositeGridFunction w(cg,all,all,all,numberOfComponents);
    realCompositeGridFunction u(cg,all,all,all,numberOfComponents);

    CompositeGridOperators cgop(cg);  // operators are needed for AMR grids
    u.setOperators(cgop);
    w.setOperators(cgop);

//      realCompositeGridFunction u;
//      u.link(w,Range(min(1,numberOfComponents-1),min(3,numberOfComponents-1)));
    u=0.;

// ******* fix needed in A++ ***************************8
//  u.link(w,Range(0,0));
//  cout << "u.numberOfComponentGrids = " << u.numberOfComponentGrids() << endl;
//  u=w;
    
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
      int dw = max(cg[0].discretizationWidth());
      
      int degreeOfSpacePolynomial = min(6,dw-1); // 2; // 6; // 2; // 2;

      printf(">>>dw=%i, Setting TZ degreeOfSpacePolynomial=%i<<<\n",dw,degreeOfSpacePolynomial);

      int degreeOfTimePolynomial = 1;
      exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);
    
    }
    OGFunction & exact = *exactPointer;

    Interpolant interpolant(cg);

    interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

    interpolant.setInterpolationMethod(Interpolant::optimized);

    // use the sparse storage option unless the grid is implicit interpolation in 2D (when we do a direct solve)
    int useOptStorage=cg.interpolationIsAllExplicit() || cg.numberOfDimensions()==3 ? 2: 0;  // 0=full, 1=tensorProduct, 2=minimal
    
    if( useOptStorage==1 )
      interpolant.setExplicitInterpolationStorageOption(Interpolant::precomputeSomeCoefficients);
    else if( useOptStorage==2 )
      interpolant.setExplicitInterpolationStorageOption(Interpolant::precomputeNoCoefficients);
    else    
      interpolant.setExplicitInterpolationStorageOption(Interpolant::precomputeAllCoefficients);

    Index I1,I2,I3;
  
    for( int iteration=0; iteration<=1; iteration++ )
    {

      exact.assignGridFunction(u);
      int grid;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
//	getIndex(cg[grid].gridIndexRange(),I1,I2,I3); 
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
	    displayMask(cg[grid].mask(),"mask");
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
	    intArray & mask = cg[cg.interpoleeGrid[grid](n)].mask();
	    if( mask(i1,i2  )==0 || mask(i1+1,i2  )==0 ||  mask(i1+2,i2  )==0 ||
		mask(i1,i2+1)==0 || mask(i1+1,i2+1)==0 ||  mask(i1+2,i2+1)==0 ||
		mask(i1,i2+2)==0 || mask(i1+1,i2+2)==0 ||  mask(i1+2,i2+2)==0 )
	    {
	      printf("****Invalid interpolation **** grid=%i, n=%i, interpoleeLocation=(%i,%i) \n",grid,n,i1,i2);
	    }
	  }
	}
      }
      if( debug & 4 )
	u.display("u before","%7.2f");
  
  
      real time0;
      Range C;
    
      if( iteration==0 )
      {
	C=Range(u.getComponentBase(0),u.getComponentBound(0));
        time0=CPU();
	u.interpolate();
	real time = CPU()-time0;
	printf("Time to interpolate =%8.2e (%i components)\n",time,C.getBound()-C.getBase()+1);
	time0=CPU();
  
	u.interpolate();

	time = CPU()-time0;
	printf("Time to interpolate again =%8.2e (%i components)\n",time,C.getBound()-C.getBase()+1);

	if( true )
	{
          // shock ellipse 
	  int grid=12, i1=973, i2=1252, i3=0;
	  if( grid<cg.numberOfComponentGrids() )
	  {
	    const IntegerArray & d = cg[grid].dimension();
	    const IntegerArray & gid = cg[grid].gridIndexRange();
	    printF("After AMR: grid=%i gid=[%i,%i][%i,%i]\n",grid,gid(0,0),gid(1,0),gid(0,1),gid(1,1));
	    if( i1>=d(0,0) && i1<=d(1,0) && i2>=d(0,1) && i2<=d(1,1) )
	    {
	      printF("SHOCK-ELLIPSE: (grid,i1,i2,i3)=(%i,%i,%i,%i) mask=%i u=%9.3e,%9.3e,%9.3e\n"
		     ,grid,i1,i2,i3,cg[grid].mask()(i1,i2,i3),
		     u[grid](i1,i2,i3,0),u[grid](i1,i2,i3,1),u[grid](i1,i2,i3,2));
	    }
	  }
	}
      }

      if( (iteration>0 && numberOfComponents>1) )
      {
	printf("\n\n +++++++++++++++++++ only interpolate component 1 +++++++++++++++++++++++++\n\n");
	C=Range(1,1);
        time0=CPU();
	u.interpolate(C);
	real time = CPU()-time0;
	printf("Time to interpolate =%8.2e (1 component)\n",time);

        time0=CPU();
	u.interpolate(C);
	time = CPU()-time0;
	printf("Time to interpolate again =%8.2e (1 component)\n",time);
      }
    
  
      if( debug & 4 )
	u.display("u after","%7.2f");

      real error=getError(cg,u,C,exact,worstError,debug);

//       RealArray componentError(numberOfComponents); componentError=0;
  
//       for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//       {
// 	const int numberOfGhostPoints = (max(cg[grid].discretizationWidth())-1)/2;
      
// 	getIndex(cg[grid].indexRange(),I1,I2,I3,numberOfGhostPoints); 
// 	// getIndex(cg[grid].dimension(),I1,I2,I3); 

// 	RealArray err(I1,I2,I3);
// 	for( int n=C.getBase(); n<=C.getBound(); n++ )
// 	{
// 	  where( cg[grid].mask()(I1,I2,I3)!=0 )
// 	  {
// 	    err=abs(u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n,0.))/max(abs(exact(cg[grid],I1,I2,I3,n,0.)));
// 	    componentError(n)=max(componentError(n),max(err));
// 	    error=max(error,componentError(n));
// 	  }
// 	}
// 	if( debug & 4 )
// 	{
// 	  display(u[grid](I1,I2,I3,C),"u",NULL,"%7.2e ");
// 	  display(err,"Error",NULL,"%7.1e ");
// 	}
//       }
//       printf("\n >>>>Maximum relative error in interpolating = %e <<<<<<\n\n",error);  
//       for( int n=C.getBase(); n<=C.getBound(); n++ )
//       {
// 	printf(" component=%i relative error=%e\n",n,componentError(n));
//         worstError=max(worstError,componentError(n));
//       }



    }  // end for iteration
    

    Range C(0,0);
    const int number=2;
    real time[Interpolant::numberOfInterpolationMethods]={0.,0.,0.};

    if( !useOptStorage )
    {
      for( int method=0; method<Interpolant::numberOfInterpolationMethods; method+=1 )
      {
	if( useOptStorage && method!=(int)Interpolant::optimized )
	  continue;
      
	interpolant.setInterpolationMethod((Interpolant::InterpolationMethodEnum)method);

	time[method]=0.;
	for( int num=0; num<number; num++ )
	{
	  int grid;
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    getIndex(cg[grid].dimension(),I1,I2,I3); 
	    where( cg[grid].mask()(I1,I2,I3)<=0 )
	    {
	      for( int n=u.getComponentBase(0); n<=u.getComponentBound(0); n++ )
		u[grid](I1,I2,I3,n)=0.;  // initial guess
	    }
	  }
	  real time0=CPU();
	  u.interpolate();
	  time[method] += CPU()-time0;
	}
      
	printf("\n*** method %s (time=%8.2e,%i components): ",(method==0 ? "standard" : method==1 ? "optimized" : "optimizedC"),
	       time[method]/number,u.getComponentBound(0)-u.getComponentBase(0)+1);
	real error=getError(cg,u,C,exact,worstError,debug);
      }
    
      printf("\nAverage time to interpolate =%8.1e(standard) %8.1e(opt speedup=%3.1f) %8.1e (C speedup=%3.1f) for grid %s\n",
	     time[0]/number,time[1]/number,time[0]/max(REAL_MIN,time[1]),
	     time[2]/number,time[0]/max(REAL_MIN,time[2]),(const char*)gridName[it]);
      
    }
    else
    {

      for( int method=0; method<3; method+=1 )
      {
        interpolant.updateToMatchGrid(cg);
	interpolant.setInterpolationMethod(Interpolant::optimized);
	interpolant.setExplicitInterpolationStorageOption((Interpolant::ExplicitInterpolationStorageOptionEnum)method);

	time[method]=0.;
	for( int num=0; num<number; num++ )
	{
	  int grid;
	  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    getIndex(cg[grid].dimension(),I1,I2,I3); 
	    where( cg[grid].mask()(I1,I2,I3)<=0 )
	    {
	      for( int n=u.getComponentBase(0); n<=u.getComponentBound(0); n++ )
		u[grid](I1,I2,I3,n)=0.; // REAL_MAX; // 0.;  // initial guess
	    }
	  }
	  real time0=CPU();
	  u.interpolate();
	  time[method] += CPU()-time0;
	}
      
	printf("\n*** method %s (time=%8.2e,%i components): ",(method==0 ? "full storage" : method==1 ? "tensor product" : "sparse"),
	       time[method]/number,u.getComponentBound(0)-u.getComponentBase(0)+1);
	real error=getError(cg,u,C,exact,worstError,debug);
      }
    
      printf("\nAverage time to interpolate =%8.1e(full) %8.1e(tensor speedup=%3.1f) %8.1e (sparse speedup=%3.1f) for grid %s\n",
	     time[0]/number,time[1]/number,time[0]/max(REAL_MIN,time[1]),
	     time[2]/number,time[0]/max(REAL_MIN,time[2]),(const char*)gridName[it]);

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
