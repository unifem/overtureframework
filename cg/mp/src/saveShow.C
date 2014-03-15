#include "Cgmp.h"
#include "Ogshow.h"

static int restartNumber=-1;

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{saveShow}} 
void Cgmp::
saveShow( GridFunction & gf0 )
//=========================================================================================
// /Description:
//    Save a solution in the show file.
//
// /gf0 (input) : save this grid function.
//
// /Notes: The array of Strings, parameters.dbase.get<aString* >("showVariableName"), holds a list of the things
//    to save in the show file.
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{

  Ogshow *&ogshow=parameters.dbase.get<Ogshow*>("show");
  
  if( ogshow!=NULL )
  {
    if( numberSavedToShowFile==-1 )
    {
      numberSavedToShowFile=0;

      // first call -- save general parameters to the show file root: 

      // we can't do this since it creates a defaultFrameSeries with no frames and plotStuff get confused.
      // parameters.saveParametersToShowFile(); 


      // *wdh* 080530 : allow each domain solver to save its parameters
      for( int d=0; d<domainSolver.size(); d++ )
      {
	if ( domainSolver[d] )
	{
	  ogshow->setCurrentFrameSeries(cg.getDomainName(d));
	    
	  assert( domainSolver[d]->parameters.dbase.get<Ogshow*>("show")==NULL );
	  domainSolver[d]->parameters.dbase.get<Ogshow*>("show")=ogshow; 
	  domainSolver[d]->parameters.saveParametersToShowFile();
	  domainSolver[d]->numberSavedToShowFile=0;
	  domainSolver[d]->parameters.dbase.get<Ogshow*>("show")=NULL;
	}
      }
      
    }
    numberSavedToShowFile++;

    // ogshow->setFrameSeriesName(0,cg.getDomainName(0)); // change the name of frame series 0

    // Here we save the titles that will go on the plots that show solutions from multiple domains
    ListOfShowFileParameters frameSeriesParameters;

    real time = domainSolver[0]->gf[domainSolver[0]->current].t;
    aString title; 
    sPrintF(title,"Cgmp: t=%8.2e, dt=%8.2e",time,dt);  // fix this 
    frameSeriesParameters.push_back(ShowFileParameter("title",title));

    aString dirName; sPrintF(dirName,"FrameSeriesHeader%i",numberSavedToShowFile); // save parameters in this directory

    for( int d=0; d<domainSolver.size(); d++ )
    {
      // The solutions from different domains are put into different frame series.
//       if( d==0 )
//         ogshow->setCurrentFrameSeries(0);
//       else
      if ( domainSolver[d] )
      {
	ogshow->setCurrentFrameSeries(cg.getDomainName(d));
	// We need set the moving grid property for each frame series.
	ogshow->setIsMovingGridProblem( domainSolver[d]->parameters.isMovingGridProblem() );
	  
	Ogshow *&domainShow = domainSolver[d]->parameters.dbase.get<Ogshow*>("show");
	  
	Ogshow *domainShowSave = domainShow;
	domainShow=ogshow;  // the domain show file now points to the Cgmp show file
	  
	printF("\n ============= Cgmp::saveShow for domain %i (%s) ==================\n",d,
	       (const char*)cg.getDomainName(d));
	  
	GridFunction & ud = domainSolver[d]->gf[domainSolver[d]->current];
	domainSolver[d]->saveShow(ud);
	  
	ogshow->saveParameters(dirName,frameSeriesParameters);
	  
	  
	domainShow=domainShowSave; // reset
      
	// we may also want to save results in separate domain show files ?

//       if( domainSolver[d]->parameters.dbase.get<Ogshow* >("show")!=NULL )
//       {
// 	printF("\n ============= Cgmp::saveShow for domain %i (%s) ==================\n",d,
// 	       (const char*)cg.getDomainName(d));

// 	GridFunction & ud = domainSolver[d]->gf[domainSolver[d]->current];
// 	domainSolver[d]->saveShow(ud);
//       }
      }
    }
  }
  
}
