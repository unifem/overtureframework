// **************  Save a show file for a multi-domain problem ****************


#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "Ogshow.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )



int 
main(int argc, char** argv)
{
  Overture::start(argc,argv);  // initialize Overture

//  aString nameOfOGFile="innerOuter2.hdf";
  aString nameOfOGFile="innerOuter2d.hdf";

  // create and read in a CompositeGrid
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);
  cg0.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter);
  cg0.update(GridCollection::THEdomain);
  
  int useStreamMode=true; // false; // true;
  aString nameOfShowFile="tds.show";
  
  Ogshow show(nameOfShowFile,".",useStreamMode);  // create a show file
  int flushFrequency=5;
  show.setFlushFrequency(flushFrequency);

  show.saveGeneralComment("Test domains in show files"); // save a general comment in the show file
   ListOfShowFileParameters showFileParams;
  showFileParams.push_back(ShowFileParameter("myValue",123));
  show.saveGeneralParameters(showFileParams,Ogshow::THEShowFileRoot);
    
  // save parameters in each frame series
  for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
  {
    real nu=domain+1.2;
    ListOfShowFileParameters showFileParams;
    showFileParams.push_back(ShowFileParameter("nu",nu));
    showFileParams.push_back(ShowFileParameter("ival",domain));
    show.setCurrentFrameSeries(cg0.getDomainName(domain));
    show.saveGeneralParameters(showFileParams);
  }
 

  printf(" cg0.numberOfDomains()=%i\n",cg0.numberOfDomains());
  Range all;
  char buffer[180];
  real t=0., dt=.2;
  int numberOfTimeSteps=11;
  for( int i=0; i<numberOfTimeSteps; i++ )
  {  

    // Here we save the titles that will go on the plots that show solutions from multiple domains
    ListOfShowFileParameters frameSeriesParameters;
    aString title;
    sPrintF(title,"Multi-domain solution %i, t=%5.2f",i+1,t);
    frameSeriesParameters.push_back(ShowFileParameter("title",title));

    aString dirName; sPrintF(dirName,"FrameSeriesHeader%i",i+1); // save parameters in this directory

    if( false )
    {
      show.setCurrentFrameSeries(cg0.getDomainName(0));
      show.saveParameters(dirName,frameSeriesParameters);
    }
    
    for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
    {
      CompositeGrid & cg = cg0.domain[domain];

      realCompositeGridFunction q;
      if( domain==0 )
      {
	q.updateToMatchGrid(cg,all,all,all,2); // create a grid function with 2 components 
	q.setName("u",0);                          // name of first component
	q.setName("T",1);                          // name of second component
      }
      else
      {
	q.updateToMatchGrid(cg,all,all,all,1); // create a grid function with 1 component
	q.setName("T",0);    
      }

      // Ogshow::FrameSeriesID frameSeries = show.newFrameSeries(cg0.getDomainName(domain));
      show.setCurrentFrameSeries(cg0.getDomainName(domain));

      if( domain==1 )
	show.setIsMovingGridProblem(true);
      else
	show.setIsMovingGridProblem(false);

      show.startFrame();                       // start a new frame
      show.saveComment(0,sPrintF(buffer,"Here is solution %i",i));              // comment 0 (shown on plot)
      show.saveComment(1,sPrintF(buffer,"  t=%e ",t));         

      show.saveParameters(dirName,frameSeriesParameters);

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
      
	qg(all,all,all,0)=sin(fx*x)*cos(fx*y);      
	if( q[grid].getComponentBound(0)>0 )
  	  qg(all,all,all,1)=cos(fx*x)*cos(fx*y);
	if( q[grid].getComponentBound(0)>1 )
  	  qg(all,all,all,2)=sin(fx*x)*sin(fx*y);
      }


      printf(" Save solution %i in domain %i\n",i,domain);
      show.saveSolution(q);
      show.endFrame();

      q=(real)i;
    } // end for domain
    
    t+=dt;
    
  }

  show.close();
  printF("\n **** Wrote the show file %s ****\n",(const char*)nameOfShowFile);

  Overture::finish();   

  return 0;
}

