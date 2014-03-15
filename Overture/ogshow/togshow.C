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
  
  printF("Usage:togshow -g=gridName -show=showFileName -append\n");
  Ogshow::ShowFileOpenOption showFileOpenOption = Ogshow::openNewFileForWriting;

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
      else if( line=="-append" )
      {
        showFileOpenOption=Ogshow::openOldFileForWriting;
      }
    }
  }

//   #ifndef USE_PPP
//   if( argc>1 )
//     nameOfOGFile=argv[1];
//   if( argc>2 )
//     nameOfShowFile=argv[2];
//   #endif
  
//   if( nameOfOGFile=="" )
//   {
//     cout << "togshow>> Enter the name of the (old) overlapping grid file:" << endl;
//     cin >> nameOfOGFile;
//   }
//   if( nameOfShowFile=="" )
//   {
//     cout << "togshow>> Enter the name of the (new) show file (blank for none):" << endl;
//     cin >> nameOfShowFile;
//   }
  
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);          // read from a data base file
  cg.update(MappedGrid::THEmask | MappedGrid::THEvertex );

  // HDF_DataBase::debug=1;
  
  int useStreamMode=false; // true;

  Ogshow show(nameOfShowFile,".",useStreamMode,showFileOpenOption);  // create a show file
  
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
  int numberOfTimeSteps=3;
  int flushFrequency=1; // 2;
//  cout << "Enter number of steps and the flush frequency" << endl;
//  cin >> numberOfTimeSteps >> flushFrequency;

  show.setFlushFrequency(flushFrequency);
  
  for( int i=1; i<=numberOfTimeSteps; i++ )  // Now save the grid functions at different time steps
  {
    show.startFrame();                       // start a new frame
    real t=i*.1;
    show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));              // comment 0 (shown on plot)
    show.saveComment(1,sPrintF(buffer,"  t=%e ",t));              // comment 1 (shown on plot)
    
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
    
    printF("save solution %i\n",i);

    // if( ( (i % flushFrequency) ==0 ) || i==numberOfTimeSteps )
    if( show.isLastFrameInSubFile() )
    {
      printf(" -- save seq info at step=%i --- \n",i);
      
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
    
    if( i==numberOfTimeSteps-3 ) Overture::abort("error");

  }

  show.close();

  Overture::finish();

  return 0;
}
