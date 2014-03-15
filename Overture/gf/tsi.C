#include "Overture.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "display.h"
#include "Oges.h"

// =========================================================================================
//  Test the special interpolant options where only some grids are interpolated.
// ========================================================================================


int 
main(int argc, char **argv) 
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0;
  int debug=0;
  Oges::debug=0;
  
  char buff[80];
  int measureCPU=false;
  
  const int maxNumberOfGridsToTest=1;
  int numberOfGridsToTest=maxNumberOfGridsToTest;
  aString gridName[maxNumberOfGridsToTest] =   { "valvee.hdf" };
    
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
	gridName[0]=argv[1];
      }
    }
  }
  else
    cout << "Usage: `testInterpolant [<gridName>] [-noTiming]' \n";

  real worstError=0., error=0.;
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
    int numberOfComponents=1;
    Range all;

    realCompositeGridFunction u(cg,all,all,all,numberOfComponents);
    
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

    Interpolant interpolant(cg);
    interpolant.debug=1;
    
    // interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
    interpolant.setInterpolationMethod(Interpolant::optimized);
    
    Index I1,I2,I3;
  
    exact.assignGridFunction(u);
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      getIndex(cg[grid].dimension(),I1,I2,I3); 
  
      where( cg[grid].mask()(I1,I2,I3)<=0 )
      {
	for( int n=u.getComponentBase(0); n<=u.getComponentBound(0); n++ )
	{
	  u[grid](I1,I2,I3,n)=-999.;
	}
      }
    }
    if( debug & 4 )
      u.display("u before","%7.2f");
  
  
    real time0;
    Range C;
    

    IntegerArray gridsToInterpolate(cg.numberOfComponentGrids());
    IntegerArray gridsToInterpolateFrom(cg.numberOfComponentGrids());
    
    gridsToInterpolate=false;
    gridsToInterpolateFrom=false;

    gridsToInterpolate(0)=true;
    gridsToInterpolateFrom=true;
    
    printf("tsi: interpolate grid=0 from all other grids..\n");
    u.getInterpolant()->interpolate(u,gridsToInterpolate,gridsToInterpolateFrom);


    gridsToInterpolate=false;
    gridsToInterpolateFrom=false;
    gridsToInterpolate(Range(1,2))=true;
    gridsToInterpolateFrom(0)=gridsToInterpolateFrom(2)=true;
    printf("tsi: interpolate grid=1,2 from grid 0,2..\n");
    u.getInterpolant()->interpolate(u,gridsToInterpolate,gridsToInterpolateFrom);

    // real error=getError(cg,u,C,exact,worstError,debug);


    worstError=max(worstError,error);
    

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
