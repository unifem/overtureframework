#include "Cgsm.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "display.h"
#include "SmParameters.h"

// static int restartNumber=-1;

//\begin{>>SolidMechanicsInclude.tex}{\subsection{saveShow}} 
void Cgsm::
saveShow( GridFunction & gf0 )
// saveShow( int current, real t, real dt )
//=========================================================================================
// /Description:
//    Save a solution in the show file. This routine will also save restart files.
//
// /current (input) : save this grid function.
//
//\end{SolidMechanicsInclude.tex}  
//=========================================================================================
{
  Ogshow *& show = parameters.dbase.get<Ogshow*>("show");

  if( show==NULL )
    return;

  real cpu0=getCPU();

  const int myid = Communication_Manager::My_Process_Number;
  int & debug = parameters.dbase.get<int >("debug");

  const real t = gf0.t;
  const real & dt= deltaT;

  if( numberSavedToShowFile==-1 )
  {
    // first call -- save general parameters
    numberSavedToShowFile=0;
    parameters.saveParametersToShowFile();
  }
  numberSavedToShowFile++;

/* ---
  if( parameters.saveRestartFile )
  {
    // keep two restart files, just in case we crash while writing one of them
    restartNumber = (restartNumber+1) % 2;
    saveRestartFile(gf0, restartNumber==0 ? "ob1.restart" : "ob2.restart" );
  }
  --- */
  
  if( debug & 1 )
    printF("Cgsm::saveShow: saving a solution in the show file...\n");
  
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");

  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  Ogshow & showFile = *show;
  showFile.startFrame();

// -------- start new --
  HDF_DataBase *dbp=NULL;
  #ifdef OV_USE_HDF5
    bool putToDataBase=true;    // hdf5  -- put on all processors
  #else
    bool putToDataBase= parameters.dbase.get<int >("myid")==0; // hdf4 - only put on processor 0
  #endif
  if( putToDataBase )
  {
    dbp = showFile.getFrame();
    assert( dbp!=NULL );
  }

  aString timeLine;
  if( !parameters.isSteadyStateSolver() )
  {
    if( gf0.t==0. || (gf0.t > .1 && gf0.t < 1.e4) )
    {
      if( gf0.t < 1. )
        sPrintF(timeLine,"t=%4.3f, dt=%8.2e",gf0.t,dt);
      else if( gf0.t < 10. )
        sPrintF(timeLine,"t=%5.3f, dt=%8.2e",gf0.t,dt);
      else if( gf0.t < 100. )
        sPrintF(timeLine,"t=%6.3f, dt=%8.2e",gf0.t,dt);
      else if( gf0.t < 1000. )
        sPrintF(timeLine,"t=%7.3f, dt=%8.2e",gf0.t,dt);
      else
        sPrintF(timeLine,"t=%8.3f, dt=%8.2e",gf0.t,dt);
    }
    else
      sPrintF(timeLine,"t=%9.2e, dt=%8.2e",gf0.t,dt);
  }
  else
  {
    sPrintF(timeLine,"it=%i",parameters.dbase.get<int >("globalStepNumber")+1);
  }
  
  // save parameters that go in this frame
  if( putToDataBase ) 
  { 
     assert( dbp!=NULL );
     HDF_DataBase & db = *dbp;
     db.put(gf0.t,"time");
     db.put(gf0.t,"t");  
     db.put(dt,"dt");
  }
  
  parameters.dbase.put<aString>("timeLine",timeLine);
  saveShowFileComments( showFile );
  parameters.dbase.remove("timeLine");

// --- end new --

// --- old: 
  if( false )
  {
    char buffer[80]; 
    aString showFileTitle[5];

    aString methodName,buff;
    if( !useConservative )
    {
      methodName=sPrintF(buff,"NC%i%i",orderOfAccuracyInSpace,orderOfAccuracyInTime);
    }
    else
    {
      methodName=sPrintF(buff,"C%i%i",orderOfAccuracyInSpace,orderOfAccuracyInTime);
    }

    showFileTitle[0]=sPrintF(buffer,"SolidMechanics %s",(const char *)methodName);
    showFileTitle[1]=sPrintF(buffer,"t=%4.3f, dt=%8.2e",t,dt);
    showFileTitle[2]="";  // marks end of titles

    for( int i=0; showFileTitle[i]!=""; i++ )
      showFile.saveComment(i,showFileTitle[i]);
  }
  
// ----- end old

  // *** use getAugmentedSolution to form the solution for the show file ****




    // save parameters
//      db.put("incompressibleNavierStokes","pde");
//      db.put(parameters.nu,"nu");
//      db.put(parameters.reynoldsNumber,"reynoldsNumber");
//      db.put(parameters.machNumber,"machNumber");

//      db.put(pc,"pressureComponent");
//      db.put(uc,"uComponent");
//      db.put(vc,"vComponent");
//      db.put(wc,"wComponent");

/* -----------------------------------------------------------------------------
  aString *showVariableName=parameters.showVariableName;
  const IntegerArray & showVariable = parameters.showVariable;

  // first count the number of variables we are going to save
  int numberOfShowVariables=0;
  int i;
  for( i=0; showVariableName[i]!=""; i++ )
    if( showVariable(i)>=0 )
      numberOfShowVariables++;
  Range all;
  realCompositeGridFunction q(cg0,all,all,all,numberOfShowVariables);  
  q.setOperators(*gf0.u.getOperators());

  // save some parameters for a restart
  db.put(gf0.t,"t");


  aString solutionName[1] = { "u" }; // *******************************************
  q.setName(solutionName[0]);                           // name grid function

  int grid;
  i=-1;
  for( int n=0; showVariableName[n]!=""; n++ )
  {
    // printF(" n=%i showVariableName=%s showVariable=%i\n",n,(const char*)(showVariableName[n]),showVariable(n));
    
    if( showVariable(n)< 0 )
      continue;

    i++;
    
    q.setName( showVariableName[n],i);

    if( showVariable(n) < parameters.numberOfComponents )
    {
      for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        q[grid](all,all,all,i)=u[grid](all,all,all,showVariable(n));
    }
    else if( showVariableName[n]=="divergence" )
    {
      for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        if( cg0.numberOfDimensions()==1 )
          q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc);
        else if( cg0.numberOfDimensions()==2 )
	{
          q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc)+u[grid].y()(all,all,all,vc); 
	  if( parameters.isAxisymmetric() )
	  {
            // div(u) = u.x + v.y + v/y for y>0   or u.x + 2 v.y at y=0
	    RealArray radiusInverse = 1./max(REAL_MIN,cg0[grid].vertex()(all,all,all,axis2));
            Index Ib1,Ib2,Ib3;
            for( int axis=0; axis<cg0.numberOfDimensions(); axis++ )
	    {
	      for( int side=0; side<=1; side++ )
	      {
		if( cg0[grid].boundaryCondition(side,axis)==OB_Parameters::axisymmetric )
		{
		  getBoundaryIndex(cg0[grid].gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
		  radiusInverse(Ib1,Ib2,Ib3)=0.;
		  q[grid](Ib1,Ib2,Ib3,i)+=u[grid].y()(Ib1,Ib2,Ib3,vc);
		}
	      }
	    }
	    q[grid](all,all,all,i)+=u[grid](all,all,all,vc)*radiusInverse;
	  }
	}
        else
          q[grid](all,all,all,i)=u[grid].x()(all,all,all,uc)+u[grid].y()(all,all,all,vc)
                                +u[grid].z()(all,all,all,wc);
    }
    else 
    {
      cout << "saveShow: unknown showVariableName = " << (const char*) showVariableName[n] << endl;
      for( grid=0; grid<cg0.numberOfComponentGrids(); grid++ )
        q[grid](all,all,all,i)=0.;
    }
  }
  q.interpolate();          // interpolate to get divergence correct  
  
  ------------------------------------------------------- */



  // save a CompositeGridFunction...
    
  realCompositeGridFunction v;
  realCompositeGridFunction & u = getAugmentedSolution(current,v,t);

  // realCompositeGridFunction & u = getCGField(HField,current);

  // saveGridInShowFile is set to true at the start and later for AMR (addGrids.bC) or moving grids 
  if( parameters.dbase.get<int >("saveGridInShowFile") )
  {
    if( debug & 4 ) 
      printF("***Save grid in the show file: numberOfComponentGrids=%i\n",cg.numberOfComponentGrids());
    showFile.saveSolution( u,"u",Ogshow::useCurrentFrame );  // save the grid and the grid function
    parameters.dbase.get<int >("saveGridInShowFile")=false;
    showFileFrameForGrid=showFile.getNumberOfFrames();
  }
  else
  {
    if( debug & 4 )
      printF("***Save solution in the show file: showFileFrameForGrid=%i (-2=default)\n",
	     showFileFrameForGrid);
    showFile.saveSolution( u,"u",showFileFrameForGrid );  // save the grid function
  }
  
  // -- this doesn't seem to work in parallel -- fix me -- see pulse example ---
  // Here we save time sequences to the show file *wdh* 091124 
  // Only save if this is the last frame in a subFile
#ifndef USE_PPP
  if( parameters.dbase.get<bool >("saveSequencesEveryTime") && parameters.dbase.get<Ogshow* >("show")!=NULL &&
      parameters.dbase.get<Ogshow* >("show")->isLastFrameInSubFile() )
  {
    saveSequencesToShowFile();
    // time sequence info for moving grids is saved here
    if( parameters.isMovingGridProblem() )
      parameters.dbase.get<MovingGrids >("movingGrids").saveToShowFile();
  }
#endif

  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  timing(parameters.dbase.get<int>("timeForShowFile"))+=getCPU()-cpu0;
}


//\begin{>>SolidMechanicsInclude.tex}{\subsection{saveSequenceInfo}} 
int Cgsm::
saveSequenceInfo( real t0, RealArray & sequenceData )
//=========================================================================================
// /Description:
//    Save info into the time history arrays.
// 
//\end{SolidMechanicsInclude.tex}  
//=========================================================================================
{
  Ogshow *& show = parameters.dbase.get<Ogshow*>("show");
  if( show==NULL )
    return 0;

  if( sequenceCount >= timeSequence.getLength(0) )
  {
    int num=timeSequence.getLength(0);
    Range R(0,num-1),all;
    RealArray seq;  seq=sequence;
    num=int(num*1.5+100);
    timeSequence.resize(num);
    sequence.redim(num,numberOfSequences);
    sequence(R,all)=seq;
  }

  timeSequence(sequenceCount)=t0;
  for( int n=sequenceData.getBase(0); n<=sequenceData.getBound(0); n++ )
    sequence(sequenceCount,n)=sequenceData(n); 

  sequenceCount++;
  return 0;
}



//\begin{>>SolidMechanicsInclude.tex}{\subsection{saveSequencesToShowFile}} 
int Cgsm::
saveSequencesToShowFile()
//=========================================================================================
// /Description:
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  Ogshow *& show = parameters.dbase.get<Ogshow*>("show");
  if( show==NULL || sequenceCount<=0 )
    return 0;
  
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");
  
  Range I(0,sequenceCount-1);
  Range N=sequence.dimension(1);
  
  aString *name = new aString [numberOfSequences]; 
  for( int n=0; n<numberOfComponents; n++ )
  {
    if( cgerrp!=NULL )
    {
      name[n]=cgerrp[0].getName(n);
    }
    else
    {
      name[n]=sPrintF("error%i",n);
    }
    for( int i=0; i<name[n].length(); i++ )
    { // change blanks to underscores (for matlab)
      if( name[n][i]==' ' )
      {
	name[n][i]='_';
      }
    }
  }


  if( computeEnergy && numberOfSequences>numberOfComponents )
  {
    name[numberOfComponents  ]="Energy";
    name[numberOfComponents+1]="Delta_U";
  }
  
  // display(sequence(I,N),"saveSequencesToShowFile: sequence(I,N)");
  

  show->saveSequence("errors",timeSequence(I),sequence(I,N),name);

  delete [] name;
  
  return 0;
}
