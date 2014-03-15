#include <string>
#include <cstdlib>

// =============================================================================================
//
//  srun -N1 -n2 -ppdebug thdf5 -nx 64 -ny 64 -nz 16 -writeCollective
// 
// =============================================================================================

#include "HDF5_DataBase.h"

double getCPU()
{
  return 0;
}


namespace {
  /// get a random int in [0,1000]
  inline int getrandom()
  {
    return int(1000*((double(RAND_MAX)-std::rand())/double(RAND_MAX)));
  }
}

int
main(int argc, char *argv[]) 
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O 

  int Number_Of_Processors = 0;
  Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);
  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
  Index::setBoundsCheck(on);

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np = Communication_Manager::numberOfProcessors();

  int nx=11,ny=11,nz=11;

  int stream = 0;
  int debug=1;
  int readCollective=false;
  int writeCollective=true;

  if ( myid==0 )
  {
    for ( int a=1; a<argc; a++ )
      if ( argv[a] == std::string("-nx") )
      {
	nx = std::atoi(argv[a+1]);
	a++;
      } 
      else if ( argv[a] == std::string("-ny") )
      {
	ny = std::atoi(argv[a+1]);
	a++;
      }
      else if ( argv[a] == std::string("-nz") )
      {
	nz = std::atoi(argv[a+1]);
	a++;
      }
      else if ( argv[a] == std::string("-stream") )
	stream =true;
      else if( argv[a] == std::string("-readCollective") )
      {
        readCollective=true;
      }
      else if( argv[a] == std::string("-writeCollective") )
      {
        writeCollective=true;
      }
  }

  
#ifdef USE_PPP  
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Bcast(&nx,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&ny,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&nz,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&readCollective,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&writeCollective,1,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Bcast(&stream,1,MPI_INT,0,MPI_COMM_WORLD);

  double tstart = MPI_Wtime();
#endif

  if( readCollective )
  {
    if( myid==0 ) printf("***Set DataBase READ mode to collective ***\n");
    GenericDataBase::setParallelReadMode(GenericDataBase::collectiveIO);
  }
  if( writeCollective )
  {
    if( myid==0 ) printf("***Set DataBase WRITE mode to collective ***\n");
    GenericDataBase::setParallelWriteMode(GenericDataBase::collectiveIO);
  }

  // cout<<"proc : "<<myid<<" : "<<nx<<"  "<<ny<<"  "<<nz<<endl;
  // 
  // create a parallel array
  //
  const int numberOfDimensions= nz==1 ? 2 : 3;  // number of dimensions that are distributed
  Partitioning_Type partition;
  partition.SpecifyDecompositionAxes(numberOfDimensions);
  int numGhost=1;
  for( int axis=0; axis<numberOfDimensions; axis++ )
    partition.partitionAlongAxis(axis, true, numGhost ); 
  for( int axis=numberOfDimensions; axis<MAX_ARRAY_DIMENSION; axis++ )
    partition.partitionAlongAxis(axis, false, 0);

//  Range R1(-1,nx-2), R2(-2,ny-3), R3(-3,nz-4), R4(1,2);
  Range R1(-1,nx-2), R2(-2,ny-3), R3(0,nz-1), R4(1,2);

  intArray parallelArray; parallelArray.partition(partition);
  parallelArray.redim(R1,R2,R3,R4);
  
  // initialize the parallel array to some random data
  intSerialArray al;
  al.reference(parallelArray.getLocalArray());

  printf(" proc=%i: global bounds=[%i,%i][%i,%i][%i,%i][%i,%i], local bounds=[%i,%i][%i,%i][%i,%i][%i,%i]\n",myid,
	 parallelArray.getBase(0),parallelArray.getBound(0),
	 parallelArray.getBase(1),parallelArray.getBound(1),
	 parallelArray.getBase(2),parallelArray.getBound(2),
	 parallelArray.getBase(3),parallelArray.getBound(3),
	 al.getBase(0),al.getBound(0),
	 al.getBase(1),al.getBound(1),
	 al.getBase(2),al.getBound(2),
	 al.getBase(3),al.getBound(3));
  
  for ( int i3=al.getBase(3); i3<=al.getBound(3); i3++ )
  for ( int i2=al.getBase(2); i2<=al.getBound(2); i2++ )
    for ( int i1=al.getBase(1); i1<=al.getBound(1); i1++ )
      for ( int i0=al.getBase(0); i0<=al.getBound(0); i0++ )
	al(i0,i1,i2,i3) = getrandom();
    
  parallelArray.updateGhostBoundaries();  // sync internal ghost boundaries

  //
  // also store a small serial array
  //
  intSerialArray serialArray(11,21);

  // initialize the serial array to some random data
  for ( int i2=serialArray.getBase(2); i2<=serialArray.getBound(2); i2++ )
    for ( int i1=serialArray.getBase(1); i1<=serialArray.getBound(1); i1++ )
      for ( int i0=serialArray.getBase(0); i0<=serialArray.getBound(0); i0++ )
	serialArray(i0,i1,i2) = i0+serialArray.getLength(0)*(i1+serialArray.getLength(1)*i2);

  //
  // and try to store empty serial and parallel arrays
  //
  doubleArray nullParallelArray;
  doubleSerialArray nullSerialArray;

  // and a scalar int
  int scalarInt = 1234;
  // and an aString
  aString aStringToSave = "an aString";

  //
  // open the test file
  //
  bool err = false;

  aString fileName = "thdf5.hdf";
  if( myid==0 && debug>0 ) printf("Open an hdf file %s...\n",(const char*)fileName);

  HDF_DataBase root;
  root.mount(fileName,"I");     // mount a new file (I=Initialize)
  if ( stream )
    root.setMode(GenericDataBase::streamOutputMode);

  //
  // store the arrays into the test file
  //
  root.putDistributed(parallelArray,"parray");
  root.put(serialArray,"sarray");

  root.putDistributed(nullParallelArray,"null_parray");
  root.put(nullSerialArray,"serial_parray");

  root.put(scalarInt,"scalarInt");
  root.put(aStringToSave,"aStringToSave");

  int scalarIntIn = -1;
  root.get(scalarIntIn,"scalarInt");
  err = !(scalarIntIn==scalarInt);
  if( err )
  {
    printf("ERROR : myid=%i Scalar int put was %i, read was %i\n",myid,scalarInt,scalarIntIn);
  }
 
  bool ok=true, ok2=false;
  root.put(ok,"ok");
  root.get(ok2,"ok");
  printf(" get: ok2=%i\n",int(ok2));
  

//  HDF_DataBase *subDir; // sub-directory  --- this works ---
  GenericDataBase *subDir; // sub-directory   --- this fails -- ... works now
  subDir = new HDF_DataBase();  // delete this later
  root.create(*subDir,"subDir","sub-directory");
  subDir->put(scalarInt,"scalarInt");

  if( debug>0 && myid==0 ) printf("Look for a variable that does not exist\n");
  int notThere=-1;
  subDir->turnOffWarnings(); // turn off warnings for next line:
  int status=subDir->get(notThere,"notThere");
  subDir->turnOnWarnings();
  if( debug>0 && myid==0 )
  {
    if( status=0 )
     printf("ERROR: variable found! (not as expected)\n");
    else
     printf("variable not found (as expected)\n");
  }
  //
  // close the test file
  //
  if ( stream )
    root.setMode(GenericDataBase::normalMode);
  root.unmount();

  //
  // open up the file again, in a different db variable just to make sure it works
  //
  // aString fileName2 = "thdf5p.hdf";
  aString fileName2 = fileName;
  if( myid==0 && debug>0 ) printf("Read the file %s\n",(const char*)fileName2);


  HDF_DataBase root2;
  root2.mount(fileName2,"R");
  if ( stream ) 
    root2.setMode(GenericDataBase::streamInputMode);

  int numberOfDimensions2=numberOfDimensions; // =1;   // number of axes to partition when reading back in 
  Partitioning_Type partition2;

  if( false )
  { // test the case when the parallel array only lives on 1 processor:
    Range processorRange(0,0); // (0,np-1) // *wdh* 060821
    partition2.SpecifyProcessorRange(processorRange); 
  }
  
  partition2.SpecifyDecompositionAxes(numberOfDimensions2);
  int numGhost2=1;
  for( int axis=0; axis<numberOfDimensions2; axis++ )
    partition2.partitionAlongAxis(axis, true, numGhost2 ); 
  for( int axis=numberOfDimensions2; axis<MAX_ARRAY_DIMENSION; axis++ )
    partition2.partitionAlongAxis(axis, false, 0);

  intArray parallelArray2;
  parallelArray2.partition(partition2); 
  //  parallelArray2 = -1;

  intSerialArray serialArray2;
  
  // 
  // the the arrays from the db
  //
  root2.getDistributed(parallelArray2,"parray");
  root2.get(serialArray2,"sarray");

  doubleArray nullParallelArray2;
  doubleSerialArray nullSerialArray2;
  root2.getDistributed(nullParallelArray2,"null_parray");
  root2.get(nullSerialArray2,"serial_parray");

  assert(nullParallelArray2.elementCount()==0);
  assert(nullSerialArray2.elementCount()==0);

  scalarIntIn = -1;
  root2.get(scalarIntIn,"scalarInt");
  aString aStringIn="";
  root2.get(aStringIn,"aStringToSave");

  root2.get(ok2,"ok");
  printf(" get2: myid=%i ok2=%i, sizeof(bool)=%i sizeof(int)=%i \n",myid,int(ok2),sizeof(bool),sizeof(int));


  if ( stream )
    root2.setMode(GenericDataBase::normalMode);
  root2.unmount();

  //
  // now compare old and new arrays
  // 

  for ( int a=0; a<numberOfDimensions+1; a++ )
  {
    if ( parallelArray.getBase(a)!=parallelArray2.getBase(a) )
    {
      cout<<"ERROR : proc "<<myid<<" : base "<<a<<" does not match : "<<
	parallelArray.getBase(a)<<" : "<<parallelArray2.getBase(a)<<endl;
      err = true;
    }
    if ( parallelArray.getBound(a)!=parallelArray2.getBound(a) )
    {
      cout<<"ERROR : proc "<<myid<<" : bound "<<a<<" does not match : "<<
	parallelArray.getBound(a)<<" : "<<parallelArray2.getBound(a)<<endl;
      err = true;
    }
  }

  if ( err ) 
  {
    Optimization_Manager::Exit_Virtual_Machine();
    return 1;
  }
  else
  {
    if( myid==0 && debug>0 ) printf("read parallel array: array has the same bounds as written array\n");
  }
  
  intSerialArray al2;
  al2.reference(parallelArray2.getLocalArray());
  for ( int i3=al.getBase(3); i3<=al.getBound(3); i3++ )
  for ( int i2=al.getBase(2); i2<=al.getBound(2); i2++ )
    for ( int i1=al.getBase(1); i1<=al.getBound(1); i1++ )
      for ( int i0=al.getBase(0); i0<=al.getBound(0); i0++ )
	if ( al2(i0,i1,i2,i3)!=al(i0,i1,i2,i3) )
	{
	  cout<<"ERROR : proc "<<myid<<" : parallel arrays do not match : should be "<<
	    al(i0,i2,i2,i3)<<", is "<<al2(i0,i1,i2,i3)<<" : at "<<i0<<" "<<i1<<" "<<i2<<" "<<i3<<endl;
	  err = true;

	}

  if ( err ) 
  {
    Optimization_Manager::Exit_Virtual_Machine();
    return 1;
  }
  else
  {
    if( myid==0 && debug>0 ) printf("read parallel array: array has the same values as written array\n");
  }

  for ( int i2=serialArray.getBase(2); i2<=serialArray.getBound(2); i2++ )
    for ( int i1=serialArray.getBase(1); i1<=serialArray.getBound(1); i1++ )
      for ( int i0=serialArray.getBase(0); i0<=serialArray.getBound(0); i0++ )
	if ( serialArray2(i0,i1,i2)!=serialArray(i0,i1,i2) )
	  {
	    cout<<"ERROR : proc "<<myid<<" : serial arrays do not match : should be "<<
	      serialArray(i0,i2,i2)<<", is "<<serialArray2(i0,i1,i2)<<" : at "<<i0<<" "<<i1<<" "<<i2<<endl;
	  }

#ifdef USE_PPP
  MPI_Barrier(MPI_COMM_WORLD);
  if ( myid==0 ) cout<<"TOTAL TIME = "<<MPI_Wtime()-tstart<<endl;
#endif

  if ( err )
  {
    cout<<"SERIAL ARRAY : PROC : "<<myid;
    serialArray.display("SARRAY");
  }
  else
  {
    if( myid==0 && debug>0 ) printf("read serial array: array has the same values as written array\n");
  }
  
  err = !(scalarIntIn==scalarInt);
  if ( err )
  {
    cout<<"ERROR : Scalar int put was "<<scalarInt<<", read was "<<scalarIntIn<<endl;
  }

  err = !(aStringIn==aStringToSave);
  if ( err )
  {
    cout<<"ERROR : aString put was "<<aStringToSave<<", read was "<<aStringIn<<endl;
  }

  if ( !err ) 
  {
    cout<<"SUCCESS : proc "<<myid<<endl;
  }

  fflush(0);
  
  Optimization_Manager::Exit_Virtual_Machine();

  return err ? 1 : 0;
}

