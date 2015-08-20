//===============================================================================
//  Test reading and writing a show file
//
// setenv OvertureGridDirectories $ovg5/sampleGrids
// togshow cic.hdf cic.show
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
  
  int debug=0;

//  aString nameOfOGFile="cic.hdf", nameOfShowFile="cic.show";
  printF("Usage: tsf [name] \n"
         "  name= name of an old overlapping grid file, or `squareXXX' where XXX=number of grid points\n");
//  aString nameOfOGFile="square256";
//  aString nameOfOGFile="square4096";
  aString nameOfOGFile="cice2.order2";
//  aString nameOfOGFile="cic3.order4";
//  aString nameOfOGFile="cice2.order4";
//  aString nameOfOGFile="cice2";

  if( myid==0 )
  {
    if( argc>1 )
      nameOfOGFile=argv[1];
  }
  broadCast(nameOfOGFile);
  aString nameOfShowFile="tsf-" + nameOfOGFile + ".show";
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
  int flushFrequency=2; // 3;
  show.setFlushFrequency(flushFrequency);

  show.saveGeneralComment("Test show files"); // save a general comment in the show file
    
  Range all;
  realCompositeGridFunction q(cg,all,all,all,3); // create a grid function with 3 components
  q.setName("u",0);                          // name of first component
  q.setName("v",1);                          // name of second component
  q.setName("Mach Number",2);                // name of third component

  printF("Interpolate the old grid function q...\n");
  Interpolant & interpolant = *new Interpolant(cg);
  q.interpolate();


  char buffer[80];                           // buffer for sprintf
//  show.setFlushFrequency(flushFrequency);
  
  int i=1,numberOfTimeSteps=1;
  real t=0.;

  ListOfShowFileParameters seriesParams_root;
  seriesParams_root.push_back(ShowFileParameter("IntParameter_root",50));
  seriesParams_root.push_back(ShowFileParameter("RealParameter_root",1.57));
  seriesParams_root.push_back(ShowFileParameter("StringParameter_root","a string parameter for series root"));
  show.saveGeneralParameters(seriesParams_root,Ogshow::THEShowFileRoot);

  ListOfShowFileParameters seriesParams_1;
  seriesParams_1.push_back(ShowFileParameter("IntParameter_1",100));
  seriesParams_1.push_back(ShowFileParameter("RealParameter_1",3.1415));
  seriesParams_1.push_back(ShowFileParameter("StringParameter_1","a string parameter for series 1"));
  show.saveGeneralParameters(seriesParams_1);

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
      
    real fx=Pi*i/numberOfTimeSteps;
      
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

  

  Ogshow::FrameSeriesID newSeries = show.newFrameSeries("SecondFrameSeries");
  assert(newSeries>0);
  ListOfShowFileParameters seriesParams_2;
  seriesParams_2.push_back(ShowFileParameter("IntParameter_2",200));
  seriesParams_2.push_back(ShowFileParameter("RealParameter_2",6.19));
  seriesParams_2.push_back(ShowFileParameter("StringParameter_2","a string parameter for series 2"));
  show.saveGeneralParameters(seriesParams_2);

  show.setIsMovingGridProblem(true);
  real tt=0.;
  real dt=.5;
  real f=0.;
  const int numberOfSteps=3;
  RealArray timeSeq(numberOfSteps), fSeq(numberOfSteps);
  real time0=getCPU();
  for ( int s=0; s<numberOfSteps; s++ ) 
  {
    show.startFrame();
    time=getCPU();
    show.saveSolution( q ); // save in the current frame series
    time=ParallelUtility::getMaxValue(getCPU()-time);
    printF("save solution in frame %i of 2nd frame series. cpu=%8.2e(s)\n",s,time);
    show.getFrame()->put(s,"step");

    timeSeq(s) = tt;
    fSeq(s) = tt*tt;
    tt+=dt;

//     Range R=s+1;
//     if( s==(numberOfSteps-1) || show.isLastFrameInSubFile() ) // save sequence once in each sub-show file
//       show.saveSequence("functionSequence",timeSeq(R),fSeq(R));

    show.endFrame();
  }
  time=ParallelUtility::getMaxValue(getCPU()-time0);
  printF(" time to save %i frames=%8.2e(s)\n",numberOfSteps,time);
  
  // show.saveSequence("functionSequence",timeSeq,fSeq);

  show.setCurrentFrameSeries(0); // go back to default frame series

  show.startFrame();
  printF("save solution in frame 2 of the first frame series.\n");
  show.saveSolution( q ); // save a second frame into the default series
  show.endFrame();
 
  show.startFrame();
  show.saveSolution( q );     
  show.endFrame();
  
  show.startFrame();
  show.saveSolution( q );     
  // show.endFrame(); -- call after saving sequences --
 
  // Sequences are NOT saved in a frame, they are saved in the show file. 
  // We always need to at least save the sequences at the end: (sequences are taken from the last sub-show-file)
  // ** But note: we can save sequences before the last call to endFrame() since otherwise the
  //    show sub-file may be closed and we will have to reopen it to save the sequences.
  timeSeq.resize(10), fSeq.resize(10);
  for( int s=4; s<10; s++ )
  {
    tt=s*dt;
    timeSeq(s) = tt;
    fSeq(s) = tt*tt*tt;
  }
  show.saveSequence("mySequence",timeSeq,fSeq);

  show.setCurrentFrameSeries(1);
  show.saveSequence("functionSequence",timeSeq,fSeq);
  show.saveSequence("functionSequence2",timeSeq,fSeq);

  // now call end frame
  show.endFrame();

  time=getCPU();
  show.close();
  time=ParallelUtility::getMaxValue(getCPU()-time);
  printF("Close the show file, cpu=%8.2e(s)\n",time);

  // ===================================================================

  printF("Open the show file\n");
  ShowFileReader showFileReader(nameOfShowFile);

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
      
      showFileReader.getASolution(solutionNumber,cg2,u);        // read in a grid and solution

      assert(cg2.numberOfComponentGrids()==cg.numberOfComponentGrids());

      const aString *headerComment;
      int numberOfHeaderComments;
      // read any header comments that go with this solution
      headerComment=showFileReader.getHeaderComments(numberOfHeaderComments);
      
      for( int i=0; i<numberOfHeaderComments; i++ )
	printf("myid=%i: Header comment: %s \n",myid,(const char *)headerComment[i]);
      
      real maxDiff=0.;
      for( int grid=0; grid<cg2.numberOfComponentGrids(); grid++ )
	{ 
	  maxDiff=max(maxDiff,max(fabs(u[grid]-q[grid])));
	  
	  if( debug>0 )
	    {
	      ::display(u[grid],sPrintF(buffer,"u[grid=%i]",grid),"%5.2f ");
	    }
	}
      printF("Maximum diff after reading back in = %8.2e\n",maxDiff);
      
      if( true )
      {
	printF("Interpolate on the new grid function u...\n");
	Interpolant & interpolant = *new Interpolant(cg2);
	u.interpolate();
      }
    }
  showFileReader.close();
      

  if( mapping!=NULL && mapping->decrementReferenceCount()==0 ) delete mapping;
  Overture::finish();

  return 0;
}
