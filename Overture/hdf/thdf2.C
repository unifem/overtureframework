// ---------- test saving a large distributed array ------------

#include "Overture.h"
#include "HDF_DataBase.h"
#include "display.h"
#include "ParallelUtility.h"

//
//  HDF_DataBase: test saving a large distributed array
//
// mpirun -np 1 thdf2 -multipleFileIO -nx=11 -ny=11 -nz=11
// mpirun -np 2 thdf2 -multipleFileIO -stream -readOnly


int
main(int argc, char *argv[] ) 
{
  Overture::start(argc,argv);  // initialize Overture

  HDF_DataBase::debug = 0; // 3; // 3; 

  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

//   int Number_Of_Processors = 0;
//   Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);
//   Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
//   Index::setBoundsCheck(on);

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np = Communication_Manager::numberOfProcessors();

  printF("Usage: thdf2 -stream -readCollective -writeCollective -multipleFileIO -readOnly -maxFiles=<> -debug=\n");

  int nx=11,ny=11,nz=11;

  bool useStreamMode=false;
  bool readOnly=false;
  int len=0;
  for( int i=1; i<argc; i++ )
  {
    aString arg=argv[i];
    if ( argv[i] == std::string("-stream") )
    {
      useStreamMode =true;
    }
    else if( argv[i] == std::string("-readCollective") )
    {
      GenericDataBase::setParallelReadMode(GenericDataBase::collectiveIO);
    }
    else if( argv[i] == std::string("-writeCollective") )
    {
      GenericDataBase::setParallelWriteMode(GenericDataBase::collectiveIO);
    }
    else if( argv[i] == std::string("-multipleFileIO") )
    {
      GenericDataBase::setParallelWriteMode(GenericDataBase::multipleFileIO);
      GenericDataBase::setParallelReadMode(GenericDataBase::multipleFileIO);
    }
    else if( len=arg.matches("-maxFiles=") )
    {
      int maxFiles=128;
      sScanF(arg(len,arg.length()-1),"%i",&maxFiles);
      printF("Setting GenericDataBase::setMaximumNumberOfFilesForWriting to %i\n",maxFiles);
      GenericDataBase::setMaximumNumberOfFilesForWriting(maxFiles);
    }
    else if( len=arg.matches("-nx=") )
    {
      sScanF(arg(len,arg.length()-1),"%i",&nx);
    }
    else if( len=arg.matches("-ny=") )
    {
      sScanF(arg(len,arg.length()-1),"%i",&ny);
    }
    else if( len=arg.matches("-nz=") )
    {
      sScanF(arg(len,arg.length()-1),"%i",&nz);
    }
    else if( len=arg.matches("-debug=") )
    {
      sScanF(arg(len,arg.length()-1),"%i",&HDF_DataBase::debug);
    }
    else if( argv[i] == std::string("-readOnly") )
    {
      readOnly=true;
    }
  }
  
  const int numberOfDimensions= nz==1 ? 2 : 3;  // number of dimensions that are distributed

  Partitioning_Type partition;
  partition.SpecifyDecompositionAxes(numberOfDimensions);
  int numGhost=1;
  for( int axis=0; axis<numberOfDimensions; axis++ )
    partition.partitionAlongAxis(axis, true, numGhost ); 
  for( int axis=numberOfDimensions; axis<MAX_ARRAY_DIMENSION; axis++ )
    partition.partitionAlongAxis(axis, false, 0);

  Range R1(-1,nx-2), R2(-2,ny-3), R3(0,nz-1), R4(1,2);

  realArray x; x.partition(partition);
  x.redim(R1,R2,R3,R4);
  x=1.;

  HDF_DataBase root;

  if( !readOnly )
  {
    real cpu0 = getCPU();
    // printf("cpu0=%e\n",cpu0);
    
    printF("\n ++++Mount the file and write some data, useStreamMode=%i +++++ \n",(int)useStreamMode);


    root.mount("thdf2.hdf","I");     // mount a new file (I=Initialize)
    if( useStreamMode )
      root.setMode(GenericDataBase::streamOutputMode);

    printF("Writing: x=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
	   x.getBase(0),x.getBound(0),
	   x.getBase(1),x.getBound(1),
	   x.getBase(2),x.getBound(2),
	   x.getBase(3),x.getBound(3));

    root.putDistributed(x,"x");               // save an A++ array in the "root" directory

    if( useStreamMode )
      root.setMode(GenericDataBase::normalMode);
    root.unmount();                // flush the data and close the file

    real cpu = getCPU()-cpu0;
    cpu = ParallelUtility::getMaxValue(cpu);
    printF("Time to write and unmount file=%8.2e\n",cpu);
    fflush(0);
  }
  
  #ifdef USE_PPP
  fflush(0);
  MPI_Barrier(Overture::OV_COMM);
  #endif
    
  printF("\n ++++Mount the file again, read-only, useStreamMode=%i +++++ \n",(int)useStreamMode);

  real cpu0=getCPU();
  root.mount("thdf2.hdf","R");   // mount read-only
  if( useStreamMode )
    root.setMode(GenericDataBase::streamInputMode);

  real cpu = getCPU()-cpu0;
  cpu = ParallelUtility::getMaxValue(cpu);
  printF("Time to mount the file=%8.2e\n",cpu);
  fflush(0);


  Partitioning_Type partition2;
  partition2.SpecifyProcessorRange( Range(0,0) );  // do this first (will set number of parallel ghost to default)
  partition2.SpecifyDecompositionAxes(numberOfDimensions);
  int numGhost2=1;
  for( int axis=0; axis<numberOfDimensions; axis++ )
    partition2.partitionAlongAxis(axis, true, numGhost2 ); 
  for( int axis=numberOfDimensions; axis<MAX_ARRAY_DIMENSION; axis++ )
    partition2.partitionAlongAxis(axis, false, 0);


  realArray x2;
  x2.partition(partition2);
  cpu0=getCPU();
  root.getDistributed(x2,"x");            // get "x"

  cpu = getCPU()-cpu0;
  cpu = ParallelUtility::getMaxValue(cpu);
  printF("Time to read the array=%8.2e\n",cpu);
  fflush(0);

  printF("After reading, x2=[%i,%i][%i,%i][%i,%i][%i,%i]\n",
	 x2.getBase(0),x2.getBound(0),
	 x2.getBase(1),x2.getBound(1),
	 x2.getBase(2),x2.getBound(2),
	 x2.getBase(3),x2.getBound(3));

  printF("After reading, x2 parallelGhost = %i,%i,%i,%i\n",x2.getGhostBoundaryWidth(0),x2.getGhostBoundaryWidth(1),
         x2.getGhostBoundaryWidth(2),x2.getGhostBoundaryWidth(3));


  cpu0=getCPU();
  if( useStreamMode )
    root.setMode(GenericDataBase::normalMode);
  root.unmount();

  cpu = getCPU()-cpu0;
  cpu = ParallelUtility::getMaxValue(cpu);
  printF("Time to unmount the file=%8.2e\n",cpu);
  fflush(0);

  Overture::finish();
  
  return 0;
}

