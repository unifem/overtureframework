//===============================================================================
//  Test the Overlapping Grid Show file class Ogshow
//
// Examples:
//   togshow -g=cic.hdf -show=cic.show
//  -- append to an old file: 
//   togshow -g=cic.hdf -show=cic.show -append
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "HDF_DataBase.h"

int
main(int argc, char *argv[])
{
  Overture::start(argc,argv);

  aString nameOfOGFile="cic.hdf", nameOfShowFile="cic.show";
  
  printF("Usage:togshow -g=gridName -show=showFileName [-append] [-numberOfTimeSteps=<i>] [-flushFrequency=<i>]\n");
  Ogshow::ShowFileOpenOption showFileOpenOption = Ogshow::openNewFileForWriting;
  bool append=false;
  
  int numberOfTimeSteps=3;
  int flushFrequency=1; // 2;

  Ogshow::debug=3; // set to 3 = 1+2 for debug info from Ogshow

  if( argc>1 )
  {
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( len=line.matches("-g=") )
      {
        nameOfOGFile=line(len,line.length()-1);
      }
      else if( len=line.matches("-show=") )
      {
        nameOfShowFile=line(len,line.length()-1);
      }
      else if( len=line.matches("-numberOfTimeSteps=") )
      {
        sScanF(line(len,line.length()-1),"%i",&numberOfTimeSteps);
	printF("Setting numberOfTimeSteps=%i\n",numberOfTimeSteps);
      }
      else if( len=line.matches("-flushFrequency=") )
      {
        sScanF(line(len,line.length()-1),"%i",&flushFrequency);
	printF("Setting flushFrequency=%i\n",flushFrequency);
      }
      else if( line=="-append" )
      {
        showFileOpenOption=Ogshow::openOldFileForWriting;
	append=true;
      }
    }
  }

  
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);          // read from a data base file
  cg.update(MappedGrid::THEmask | MappedGrid::THEvertex );

  // HDF_DataBase::debug=1;
  
  int useStreamMode=false; // true;

  Ogshow show(nameOfShowFile,".",useStreamMode,showFileOpenOption);  // create a show file
  
  // save general comments unless we are appending to an existing show file
  show.saveGeneralComment("Solution to the Navier-Stokes"); // save a general comment in the show file
  show.saveGeneralComment(" file written on April 1");      // save another general comment

  Range all;
  realCompositeGridFunction q(cg,all,all,all,3); // create a grid function with 3 components
//   realCompositeGridFunction u,v,machNumber;  // create grid functions for components
//   u.link(q,Range(0,0));                               // link u to the first component of q
//   v.link(q,Range(1,1));                               // link v to the second component of q
//  machNumber.link(q,Range(2,2));                      // ...

  // save the names of components,  first name is the name of the vector
  q.setName("q");                            // assign name to grid function and components
  q.setName("u",0);                          // name of first component
  q.setName("v",1);                          // name of second component
  q.setName("T",2);                          // name of third component

  char buffer[80];                           // buffer for sprintf
//  cout << "Enter number of steps and the flush frequency" << endl;
//  cin >> numberOfTimeSteps >> flushFrequency;

  show.setFlushFrequency(flushFrequency);
  
  real t0=0., dt=.1;
  int solutionNumber=0;
  if( append )
  {
    solutionNumber=show.getNumberOfFrames();
    t0 = solutionNumber*dt;
  }

  for( int i=1; i<=numberOfTimeSteps; i++ )  // Now save the grid functions at different time steps
  {
    show.startFrame();                       // start a new frame
    real t= t0 + i*dt;
    show.saveComment(0,sPrintF(buffer,"Here is solution %i",solutionNumber+i));  // comment 0 (shown on plot)
    show.saveComment(1,sPrintF(buffer,"  t=%e ",t));                             // comment 1 (shown on plot)
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const realArray & x = cg[grid].vertex()(all,all,all,0);
      const realArray & y = cg[grid].vertex()(all,all,all,1);
      
      real fx=Pi*i/numberOfTimeSteps;
      
      q[grid](all,all,all,0)=sin(fx*x)*cos(fx*y); 
      q[grid](all,all,all,1)=cos(fx*x)*cos(fx*y);
      q[grid](all,all,all,2)=sin(fx*x)*sin(fx*y);
    }
    show.saveSolution( q );              // save the current grid function

    if( false )
    { // testing: 
      show.saveSolution( q,"v" );  // save under a different name (these are not currently found by plotStuff)
    }
    
    show.getFrame()->put(t,"t");         // save some extra info using data base functions
    
    printF("--togshow-- save solution %i\n",i);

    // if( ( (i % flushFrequency) ==0 ) || i==numberOfTimeSteps )
    if( show.isLastFrameInSubFile() )
    {
      printf("--togshow-- save seq info at step=%i --- \n",i);
      
      const int n=10*i;
      RealArray time(n);
      time.seqAdd(0,1./(n-1));
      RealArray value(n);
      value=sin(2.*Pi*time);
      show.saveSequence( "my seq",time,value);

      value=sin(Pi*time);
      show.saveSequence( "my next seq",time,value);
    }

    if( true ) show.endFrame();
    
    // if( i==numberOfTimeSteps-3 ) Overture::abort("error");

  }

  show.close();

  Overture::finish();

  return 0;
}
