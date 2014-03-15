#include "Overture.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#undef printf

// =========================================================================================
//  Test the Interpolant
// ========================================================================================

bool measureCPU=TRUE;


static int 
displayMaskNew( intArray & x_, const aString & label )
{
  if( Communication_Manager::My_Process_Number==0 ) printf("%s\n",(const char*)label);
  
  const intSerialArray & x = x_.getLocalArray();

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

    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      myDisplayMask(cg[grid].mask(),"cg.mask");
    }
    

    GridCollection gc;
    int processorForGraphics = 0;
    ParallelGridUtility::redistribute( cg, gc, Range(processorForGraphics,processorForGraphics) );

    for( grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
      displayMaskNew(gc[grid].mask(),"*** gc.mask ****");
    }


    if( debug & 4 )
    {
      cg[0].mask().display("mask"); // bug here cannot display on 1 proc. if array has ghost points --> works now?
      const intSerialArray & maskLocal = cg[0].mask().getLocalArrayWithGhostBoundaries();
      maskLocal.display("maskLocal");
    }
    
    

  } // end loop over grids.


  Overture::finish();          
  return 0;
}
