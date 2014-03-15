//===============================================================================
//  Test writing a very big show file
//
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "HDF_DataBase.h"
#include "ShowFileReader.h"
#include "display.h"
#include "SquareMapping.h"
#include "ParallelUtility.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);
  const int myid=max(0,Communication_Manager::My_Process_Number);
  
  GenericDataBase::setParallelWriteMode(GenericDataBase::collectiveIO);
  int debug=0;

//  aString nameOfOGFile="cic.hdf", nameOfShowFile="cic.show";
  printF("Usage: tbsf [name] \n"
         "  name= name of an old overlapping grid file, or `squareXXX' where XXX=number of grid points\n");
//  aString nameOfOGFile="square256";
//  aString nameOfOGFile="square4096";
  aString nameOfOGFile="cice";
//  aString nameOfOGFile="cic3.order4";
//  aString nameOfOGFile="cice2.order4";
//  aString nameOfOGFile="cice2";

  if( myid==0 )
  {
    if( argc>1 )
      nameOfOGFile=argv[1];
  }
  broadCast(nameOfOGFile);
  aString nameOfShowFile="tbsf-" + nameOfOGFile + ".show";
  printf("myid=%i, nameOfOGFile=%s\n",myid,(const char*)nameOfOGFile);
  
  Mapping *mapping=NULL;
  CompositeGrid cg; // (2,1);
  int len=0;
  if( len=nameOfOGFile.matches("square") ) // expects an answer of "square256" or "square512" 
  {
    SquareMapping & square = *new SquareMapping(); 
    mapping = &square;  mapping->incrementReferenceCount();
    int nx=11;
    sScanF(nameOfOGFile(len,nameOfOGFile.length()-1),"%i",&nx);
    printF("Creating a square with nx=%i and ny=%i\n",nx,nx);
    square.setGridDimensions(axis1,nx);
    square.setGridDimensions(axis2,nx);
//     int numberOfDimensions=2, numberOfGrids=1;
//             cg.setNumberOfDimensionsAndGrids(numberOfDimensions,numberOfGrids);
//             cg[0].reference(*mapping);
            cg.updateReferences();
    cg.add(square);
    assert(cg.numberOfComponentGrids()==1);
  }
  else
  {
    getFromADataBase(cg,nameOfOGFile);          // read from a data base file
  }
  cg.update( MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex );

  // HDF_DataBase::debug=1;
  
  int useStreamMode=true; // false; // true;

  Ogshow show(nameOfShowFile,".",useStreamMode);  // create a show file
  int flushFrequency=3;
  show.setFlushFrequency(flushFrequency);

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
    
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    #ifdef USE_PPP
      realSerialArray vertex; getLocalArrayWithGhostBoundaries(cg[grid].vertex(),vertex);
      realSerialArray qg;     getLocalArrayWithGhostBoundaries(q[grid],qg);
    #else
      const realSerialArray & vertex = cg[grid].vertex();
      realSerialArray & qg = q[grid];
    #endif  

    const realSerialArray & x = vertex(all,all,all,0);
    const realSerialArray & y = vertex(all,all,all,1);
      
    real fx=10.*Pi*i/numberOfTimeSteps;
      
    qg(all,all,all,0)=sin(fx*x)*cos(fx*y);           //  get u and v from some computation
    qg(all,all,all,1)=cos(fx*x)*cos(fx*y);
    qg(all,all,all,2)=sin(fx*x)*sin(fx*y);
  }
  real time=getCPU();
  show.saveSolution( q );              // save the current grid function
  time=ParallelUtility::getMaxValue(getCPU()-time);
  printF("save solution frame 1 of the first frame series. cpu=%8.2e(s)\n",time);
  show.getFrame()->put(t,"t");         // save some extra info using data base functions
  show.endFrame();
    

  time=getCPU();
  show.close();
  time=ParallelUtility::getMaxValue(getCPU()-time);
  printF("Close the show file, cpu=%8.2e(s)\n",time);


  if( mapping!=NULL && mapping->decrementReferenceCount()==0 ) delete mapping;
  Overture::finish();

  return 0;
}
