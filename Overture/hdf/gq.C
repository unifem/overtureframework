#include "Overture.h"  
#include "PlotStuff.h"
#include "display.h"
#include "ParallelUtility.h"
#include "Ogshow.h"  
#include "HDF_DataBase.h"
#include "ShowFileReader.h"

#ifndef USE_PPP
#define MPI_Wtime getCPU
#endif


extern double timeForParallelGetArray;
extern double timeForSerialGetArray;
extern double timeForScalarGet;

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  HDF_DataBase::debug = 0; // 3; 

  printF(" ------------------------------------------------------------------- \n");
  printF("        Test of the HDF Data Base and Show files                     \n");
  printF("   gq -g=<grid> [-stream]                                            \n");
  printF(" ------------------------------------------------------------------- \n");

//  aString nameOfOGFile="square10.hdf";
  aString nameOfOGFile="cic.hdf";
//  aString nameOfOGFile="oneBump2e.order2.hdf";
  // aString nameOfOGFile="sibe1.order2.hdf";
  // aString nameOfOGFile="box10.hdf";

  int useStreamMode=false; // true;
  int len=0;
  for( int i=1; i<argc; i++ )
  {
    aString arg = argv[i];
    if ( argv[i] == std::string("-stream") )
    {
      useStreamMode =true;
      printF("**** turn on stream mode ***\n");
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
    else if( len=arg.matches("-g=") )
    {
      nameOfOGFile=arg(len,arg.length()-1);
    }
  }
  

//   if( argc>1 )
//   {
//     nameOfOGFile=argv[1];
//   }
//   else
//   {
//     cout << "gridQuery>> Enter the name of the (old) overlapping grid file:" << endl;
//     cin >> nameOfOGFile;
//   }
  
  // create and read in a CompositeGrid
  CompositeGrid cg;

  double time0=MPI_Wtime();
  
  getFromADataBase(cg,nameOfOGFile);
//  cg.update(MappedGrid::THEmask);
  printF(" getFromADataBase for %s, np=%i\n",(const char*)nameOfOGFile,np);
  
  printF(" Cpu from  MPI_Wtime =  %8.2e\n",MPI_Wtime()-time0);

  timeForParallelGetArray=ParallelUtility::getMaxValue(timeForParallelGetArray);
  printF(" time for parallel get(array) = %8.2e\n",timeForParallelGetArray);
  printF(" time for serial get(array)   = %8.2e\n",timeForSerialGetArray);
  printF(" time for scalar get          = %8.2e\n",timeForScalarGet);


  cg.update( MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );

  // HDF_DataBase::debug=1;
  

  aString nameOfShowFile="gq.show";

  time0=getCPU();
  Ogshow show(nameOfShowFile,".",useStreamMode);  // create a show file
  real timeToMount=ParallelUtility::getMaxValue(getCPU()-time0);
  
  printF("*** useStreamMode=%i, time to mount a new show file = %8.2e (s) ***\n",useStreamMode,timeToMount);
  
  show.setFlushFrequency(3);

  show.saveGeneralComment("Test show files"); // save a general comment in the show file
    
  Range all;
  realCompositeGridFunction q(cg,all,all,all,3); // create a grid function with 3 components
  q.setName("u",0);                          // name of first component
  q.setName("v",1);                          // name of second component
  q.setName("Mach Number",2);                // name of third component

  char buffer[80];                           // buffer for sprintf
//  show.setFlushFrequency(flushFrequency);
  
  int i=1,numberOfTimeSteps=1;
  real t=0.;

  show.startFrame();                       // start a new frame
  show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));              // comment 0 (shown on plot)
  show.saveComment(1,sPrintF(buffer,"  t=%e ",t));              // comment 1 (shown on plot)
    
  q=0.;
  
  printF(" *** save a solution...\n");
  fflush(0);
  
  time0=getCPU();

  show.saveSolution( q );              // save the current grid function
  show.getFrame()->put(t,"t");         // save some extra info using data base functions
  show.endFrame();
    
  real timeForSave=ParallelUtility::getMaxValue(getCPU()-time0);
  
  printF("*** save solution %i, cpu=%8.2e (s)\n",i,timeForSave);

  time0=getCPU();
  show.close();

  real timeToClose=ParallelUtility::getMaxValue(getCPU()-time0);
  printF("*** close the show file, cpu=%8.2e\n",timeToClose);

  // ===================================================================

  time0=getCPU();
  ShowFileReader showFileReader(nameOfShowFile);
  real timeForOpen=ParallelUtility::getMaxValue(getCPU()-time0);
  printF("Open the show file: cpu=%8.2e\n",timeForOpen);

  for ( int fs=0; fs<showFileReader.getNumberOfFrameSeries(); fs++ )
  {
    cout<<"READING Frame Series :  "<<showFileReader.getFrameSeriesName(fs)<<endl;
    showFileReader.setCurrentFrameSeries(fs);
    int numberOfFrames=showFileReader.getNumberOfFrames();
    int numberOfSolutions = max(1,numberOfFrames);
    printF(" numberOfFrames=%i, numberOfSolutions=%i\n",numberOfFrames,numberOfSolutions);
      
    int solutionNumber;
      
    CompositeGrid cg2;
    realCompositeGridFunction u;
      
    //   GL_GraphicsInterface ps;          // create a GL_GraphicsInterface object
    //   GraphicsParameters psp;           // create an object that is used to pass parameters
      
    solutionNumber=1;
      
    time0=getCPU();
    showFileReader.getASolution(solutionNumber,cg2,u);        // read in a grid and solution
    real timeForGetASolution=ParallelUtility::getMaxValue(getCPU()-time0);
    printF("Show file: get a solution:  cpu=%8.2e\n",timeForGetASolution);

    assert(cg2.numberOfComponentGrids()==cg.numberOfComponentGrids());

  }
  showFileReader.close();

  Overture::finish();          
  return 0;
}
