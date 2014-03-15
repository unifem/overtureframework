// 
//  Test code for the parallel "canInterpolate" routines
//
//  mpirun -np 2 tci -noplot tci1
//
#include "Overture.h"
#include "PlotStuff.h"
#include "LoadBalancer.h"
#include "display.h"
#include "ParallelUtility.h"
#include "CanInterpolate.h"

// void 
// getInterpolationStencil(const Integer&      k10,
// 			const Integer&      k20,
// 			const RealArray&    r,
// 			const IntegerArray& interpolationStencil,
// 			const IntegerArray& useBackupRules,
//                         const CompositeGrid & cg );

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)




int
checkCanInterpolate(CompositeGrid & cg, int numSteps, int debug=0 )
// ==================================================================================
//  
//   *TEST ROUTINE* Check the parallel canInterpolate function
//
//  numSteps : number of times to recall the CanInterpolate routine (for checking memory leaks)
// ==================================================================================
{
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  using namespace CanInterpolate;
  
  // Make a list of interpolation points on this processor:

  // -- count the total number of interp. pts on this processor --
  int numberToCheck=0; // counts number of queries
  int ni=0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const intSerialArray & ip = cg.interpolationPoint[grid].getLocalArray();
    int niLocal = ip.getLength(0);
    ni+=niLocal;
 
  }
  if( ni == 0 ) return 0;  // no interpolation pts to check
  
  CanInterpolateQueryData *cid = new CanInterpolateQueryData[ni*cg.numberOfComponentGrids()];
  
  int n=0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {

    const intSerialArray & ip = cg.interpolationPoint[grid].getLocalArray();
    const intSerialArray & ig = cg.interpoleeGrid[grid].getLocalArray();
    const realSerialArray & ci = cg.interpolationCoordinates[grid].getLocalArray();
    int niLocal = ip.getLength(0);
    
    if( niLocal>0 )
    {
      for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
      {
        // for testing check if we can interp. from all other donors:
        for(int donor=0; donor<cg.numberOfComponentGrids(); donor++ )
	{
	  // if( donor!=grid )
	  if( donor==ig(i) )  // only check existing values that should be value
	  {
	    cid[n].id=n; cid[n].i=i; cid[n].grid=grid; cid[n].donor=donor;
	    if( true || donor==ig(i) )  
	    {
	      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
		cid[n].rv[axis]=ci(i,axis);  // These are only correct for  donor==ig(i). Doesn't matter for testing
	    }
	    
	    n++;
	  }
	}
	
      }
    } // end if niLocal>0 
  }
  numberToCheck=n;
  if( numberToCheck==0 )
  {
    delete [] cid;
    return 0;
  }
  
  // Allocate space for results
  CanInterpolateResultData *cir = new CanInterpolateResultData[numberToCheck];
    

  // --------------------------------
  // -------check canInterpolate-----
  // --------------------------------

  for( int step=0; step<numSteps; step++ )  // check for memory leaks
  {
    canInterpolate( cg, numberToCheck,cid, cir );
    if( step % 1000 == 0 )    
      Overture::printMemoryUsage(sPrintF("canInterpolate: step=%i",step));
  }
  
  // output results
  printF("\n ****** AFTER canInterpolate ****\n");
  int numberOfErrors=0;
  for( int n=0; n<numberToCheck; n++ )
  {
    int id = cir[n].id;
    if( id!=n )
    {
      Overture::abort("ERROR: id!=n");
    }
    
    int i=cid[id].i, grid=cid[id].grid, donor=cid[id].donor;
    if( debug & 2 )
    {
      const intSerialArray & il = cg.interpoleeLocation[grid].getLocalArray();
      real *rv = cid[id].rv;
      printf(" myid=%i: canInterpolate: n=%i id=%i i=%i grid=%i donor=%i width=%i r=(%g,%g,%g)"
             "  il=(%i,%i) (true il=(%i,%i))\n",
             myid,n,id,i,grid,donor,cir[n].width,rv[0],rv[1],rv[2],cir[n].il[0],cir[n].il[1],il(i,0),il(i,1));
    }
    

    if( cir[n].width!=3 )
    {
      const intSerialArray & il = cg.interpoleeLocation[grid].getLocalArray();
      numberOfErrors++;
      real *rv = cid[id].rv;
      printf(" myid=%i: canInterpolate:ERROR: n=%i id=%i i=%i grid=%i donor=%i width=%i r=(%g,%g,%g) "
             "il=(%i,%i)\n",
             myid,n,id,i,grid,donor,cir[n].width,rv[0],rv[1],rv[2],il(i,0),il(i,1));
    }
    
  }
  numberOfErrors=ParallelUtility::getSum(numberOfErrors);
  int totalNumberToCheck=ParallelUtility::getSum(numberToCheck);
  
  printF("*********** There were %i errors (%i points checked) ***********\n",numberOfErrors,totalNumberToCheck);
  fflush(0);
  Communication_Manager::Sync();

  delete [] cid;
  delete [] cir;
  
  return 0;
  
}

int 
checkIndexedType()
{
  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  int debug=0; // 1
   
  const int numSteps=1000000;
  for( int step=0; step<numSteps; step++ )  // check for memory leaks
  {

    int vector[4][4] = { 5, 5, 5, 5, 5, 4, 4, 5, 5, 4, 4, 5, 5, 5, 5, 5 };
    int wvector[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int blocklengths[2] = {2, 2};
    int displacements[2] = {5, 9};
    int rank;
    MPI_Datatype mytype;
    MPI_Status mystatus;
    int i, j;

    // MPI_Init( &argc, &argv );

    MPI_Type_indexed( 2, blocklengths, displacements, MPI_INT, &mytype );
    MPI_Type_commit( &mytype );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    if( true )
    {
      MPI_Request *receiveRequest = new MPI_Request[np];  
      MPI_Request *sendRequest = new MPI_Request[np];  

      const int tag1=4118623;
      int loc=0;
      for( int p=0; p<np; p++ )
      {  
	int tag=tag1+myid;
	MPI_Irecv( wvector, 1, mytype, p, tag, MPI_COMM_WORLD, &receiveRequest[p] );
      }

      for( int p=0; p<np; p++ )
      {
	int tag=tag1+p;
	MPI_Isend(vector, 1, mytype, p, tag, MPI_COMM_WORLD, &sendRequest[p] ); 
      }

      // --- wait for all the receives to finish ---
      MPI_Status *receiveStatus= new MPI_Status[np];  
      MPI_Waitall(np,receiveRequest,receiveStatus);
  
      // wait for sends to finish on this processor before we can clean up
      MPI_Waitall(np,sendRequest,receiveStatus);

      delete [] receiveRequest;
      delete [] sendRequest;
      delete [] receiveStatus;


    }
    else
    {
      if ( rank == 0 )
      {
	MPI_Send( vector, 1, mytype, 1, 0, MPI_COMM_WORLD );
      }
      else
      {   
	if( debug==1 )
	{
	  printf(" rank=%i\n",rank );
	  for (i = 0; i < 4; i++)
	  {
	    printf("\n" );
	    for (j=0; j < 4; j++)
	      printf("%i ", wvector[i][j] );
	  }
	}
      
	MPI_Recv( wvector, 1, mytype, 0, 0, MPI_COMM_WORLD, &mystatus );
	if( debug==1 )
	{
	  printf("\n" );
	  for (i = 0; i < 4; i++)
	  {
	    printf("\n" );
	    for (j=0; j < 4; j++)
	      printf("%i ", wvector[i][j] );
	  }
	  printf("\n" );
	}
      
      }    
      
    }
    
    MPI_Type_free( &mytype );

    if( step % 10000 == 0 )    
      Overture::printMemoryUsage(sPrintF("indexedType: step=%i",step));
  }



  return 0;
}



int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  const int np= max(1,Communication_Manager::numberOfProcessors());
  const int myid=max(0,Communication_Manager::My_Process_Number);

  real memoryUsageScaleFactor=1.0;
  Overture::turnOnMemoryChecking(true,memoryUsageScaleFactor);

  int debug=0;

  int numSteps=1;  // for memory leak check, take many steps 

  MappedGrid::setMinimumNumberOfDistributedGhostLines(2); 

  char buff[80];

  bool plotOption=true;
  aString commandFileName="", line;
  if( argc>1 )
  {
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      // printf("myid=%i: argv[%i]=%s\n",myid,i,argv[i]);
      
      line=argv[i];
      if( len=line.matches("-noplot") )
      {
	plotOption=false;
	printF(" Setting plotOption=false\n");
      }
      else if( len=line.matches("-numSteps=") )
      {
	sScanF(line(len,line.length()-1),"%i",&numSteps);
	printF(" Setting numSteps=%i (for memory leak check).\n",numSteps);
      }
      else if( commandFileName=="" )
      {
	commandFileName=line;    
	printf("myid=%i: Using command file = [%s]\n",myid,(const char*)commandFileName);
      }
    }
  }
  
  LoadBalancer loadBalancer;
//  loadBalancer.setLoadBalancer(LoadBalancer::sequentialAssignment);
//  loadBalancer.setLoadBalancer(LoadBalancer::randomAssignment);
  

  PlotStuff ps(plotOption,"tci");               // for plotting
  GraphicsParameters psp;

  aString logFile="tci.cmd";
  ps.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";
  if( commandFileName!="" )
    ps.readCommandFile(commandFileName);

  aString menu[]=
  {
    "choose a grid",
    "grid plot",
    "check mpi",
    "erase",
    "exit",
    ""
  };
  aString answer,answer2;
  
  int currentGrid=0;
  CompositeGrid cg;
  realCompositeGridFunction u;
  
  psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
  psp.set(GI_COLOUR_INTERPOLATION_POINTS,true);

  int grid=0, level=1;
  bool plotGrid=false;
  aString nameOfOGFile;
  
  for( ;; )
  {

    if( plotOption && plotGrid )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      PlotIt::plot(ps,cg,psp);
    }
    
    ps.getMenuItem(menu,answer,"choose");
    
    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="choose a grid" )
    {

      ps.inputString(nameOfOGFile,"Enter the name of the grid");
      cout << "read grid " << nameOfOGFile << endl;
      getFromADataBase(cg,nameOfOGFile,loadBalancer);
      cg.update(MappedGrid::THEmask);
      plotGrid=true;

      checkCanInterpolate(cg,numSteps);


    }
    else if( answer=="grid plot" )
    {
      ps.erase();
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::plot(ps,cg,psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="check mpi" )
    {

      checkIndexedType();


    }
    else if( answer=="erase" )
    {
      ps.erase();
      plotGrid=false;
    }
    else if( answer=="debug" )
    {
      ps.inputString(answer,"Enter debug");
//       sScanF(answer,"%i",&ogen.debug);
//       printF(" ogen.debug = %i\n",ogen.debug);
    }
    else
    {
      printF("Unknown response: [%s] \n",(const char*)answer);
      ps.stopReadingCommandFile();
    }
  }
  

  Overture::finish();          
  return 0;
}
