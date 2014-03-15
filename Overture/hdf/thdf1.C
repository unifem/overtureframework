#include "Overture.h"
#include "HDF_DataBase.h"
#include "display.h"
#include "ParallelUtility.h"

//
//  HDF_DataBase: example1
//
// mpirun -np 1 thdf1 -multipleFileIO
// mpirun -np 2 thdf1 -multipleFileIO -readOnly
// 
// mpirun -np 1 thdf1 -multipleFileIO -stream
// mpirun -np 2 thdf1 -multipleFileIO -stream -readOnly


int
main(int argc, char *argv[] ) 
{
  Overture::start(argc,argv);  // initialize Overture

  HDF_DataBase::debug = 3; // 3; 

  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

//   int Number_Of_Processors = 0;
//   Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);
//   Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
//   Index::setBoundsCheck(on);

  const int myid = max(0,Communication_Manager::My_Process_Number);
  const int np = Communication_Manager::numberOfProcessors();

  printF("Usage: thdf1 [-stream] [-readCollective] [-writeCollective] [-multipleFileIO] [-readOnly] [-maxFiles=<>]\n");

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
    else if( argv[i] == std::string("-readOnly") )
    {
      readOnly=true;
    }
  }
  

  floatArray x(Range(-1,2),Range(3,5));
  floatSerialArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
  for( int i1=xLocal.getBase(1); i1<=xLocal.getBound(1); i1++ )
  {
    for( int i0=xLocal.getBase(0); i0<=xLocal.getBound(0); i0++ )
    {
      xLocal(i0,i1)= i0-x.getBase(0) + x.getLength(0)*( i1-x.getBase(1) );
    }
  }

  int num=5;
  int ival=1234;
  aString label; 
  label="my label";
  doubleSerialArray y(Range(1,3),Range(-1,2));
  for( int i1=y.getBase(1); i1<=y.getBound(1); i1++ )
  {
    for( int i0=y.getBase(0); i0<=y.getBound(0); i0++ )
    {
      y(i0,i1)= i0-y.getBase(0) + y.getLength(0)*( i1-y.getBase(1) );
    }
  }

  floatArray z(Range(0,3),Range(2,6));
  floatSerialArray zLocal; getLocalArrayWithGhostBoundaries(z,zLocal);
  
  for( int i1=zLocal.getBase(1); i1<=zLocal.getBound(1); i1++ )
  {
    for( int i0=zLocal.getBase(0); i0<=zLocal.getBound(0); i0++ )
    {
      zLocal(i0,i1)= i0-z.getBase(0) + z.getLength(0)*( i1-z.getBase(1) );
    }
  }


  HDF_DataBase root;

  if( !readOnly )
  {
    printF("\n ++++Mount the file and write some data, useStreamMode=%i +++++ \n",(int)useStreamMode);


    root.mount("ex1.hdf","I");     // mount a new file (I=Initialize)
    if( useStreamMode )
      root.setMode(GenericDataBase::streamOutputMode);

    root.putDistributed(x,"x");               // save an A++ array in the "root" directory

    root.put(num,"num");           // save an int in the "root" directory
  
    // HDF_DataBase subDir1;      
    GenericDataBase& subDir1 = *root.virtualConstructor();
    root.create(subDir1,"stuff","directory");   // create a sub-directory, class="directory"

    subDir1.put(ival,"ival"); 

    subDir1.put(label,"label1");   // save a aString in the sub-directory  

    subDir1.put(y,"y");  

    subDir1.putDistributed(z,"z");  

    HDF_DataBase subDir1a;
    subDir1.create(subDir1a,"moreStuff","directory");
    subDir1a.putDistributed(z,"x1");

    delete & subDir1;
    if( useStreamMode )
      root.setMode(GenericDataBase::normalMode);
    root.unmount();                // flush the data and close the file
  }
  
  #ifdef USE_PPP
  fflush(0);
  MPI_Barrier(Overture::OV_COMM);
  #endif
    
  printF("\n ++++Mount the file again, read-only, useStreamMode=%i +++++ \n",(int)useStreamMode);

  root.mount("ex1.hdf","R");   // mount read-only
  if( useStreamMode )
    root.setMode(GenericDataBase::streamInputMode);

  floatArray x2;
  root.getDistributed(x2,"x");            // get "x"
  ::display(x2,"Here is x2 (should be x2(-1:2,3:5)=0,1,2,..)","%3.0f ");
    
  int num2;
  root.get(num2,"num");  
  printf("myid=%i : num from file=%i, should be %i\n",myid,num2,num);

  HDF_DataBase subDir2;
  root.find(subDir2,"stuff","directory");
    
  int ival2;
  subDir2.get(ival2,"ival");
  printf("myid=%i : ival from file=%i, should be %i\n",myid,ival2,ival);

  aString label2;
  subDir2.get(label2,"label1"); // get label1
  printf("myid=%i : label2 from file =[%s]\n",myid,(const char *) label2);

  doubleSerialArray y2;
  subDir2.get(y2,"y");            // get "y"
  ::display(y2,"Here is y2 (should be y2(1:3,-1:2)=0,1,2,...)","%3.0f ");

  floatArray z2;
  subDir2.getDistributed(z2,"z");     
  ::display(z2,"Here is z2 (should be z2(0:3,2:6)=0,1,2,...)","%3.0f ");

  if( !useStreamMode )
  { // we cannot re-read "y" when in stream mode.
    doubleSerialArray ys;
    Index Iv[6];
    Iv[0]=Range(2,3); Iv[1]=Range(-1,1); Iv[2]=0; Iv[3]=0; Iv[4]=0; Iv[5]=0;
  
    subDir2.get(ys,"y",Iv);            // get "x"
    ::display(ys,"Here is ys, a sub-array of y (should be ys(2:3,-1:1))","%3.0f ");
  }
  
  HDF_DataBase subDir2a;
  subDir2.find(subDir2a,"moreStuff","directory");
  subDir2a.getDistributed(z,"x1");

  if( useStreamMode )
    root.setMode(GenericDataBase::normalMode);
  root.unmount();

  Overture::finish();
  
  return 0;
}

