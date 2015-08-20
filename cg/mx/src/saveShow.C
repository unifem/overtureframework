#include "Maxwell.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "display.h"

// static int restartNumber=-1;

//\begin{>>MaxwellInclude.tex}{\subsection{saveShow}} 
void Maxwell::
saveShow( int current, real t, real dt )
//=========================================================================================
// /Description:
//    Save a solution in the show file. This routine will also save restart files.
//
// /current (input) : save this grid function.
//
//\end{MaxwellInclude.tex}  
//=========================================================================================
{

  if( show==NULL )
    return;

  real cpu0=getCPU();

  const int myid = Communication_Manager::My_Process_Number;

/* ---
  if( parameters.saveRestartFile )
  {
    // keep two restart files, just in case we crash while writing one of them
    restartNumber = (restartNumber+1) % 2;
    saveRestartFile(gf0, restartNumber==0 ? "ob1.restart" : "ob2.restart" );
  }
  --- */
  
  if( debug & 1 )
    printF("saving a solution in the show file...\n");
  
  Ogshow & showFile = *show;
  showFile.startFrame();

  HDF_DataBase *dbp=NULL;
  #ifdef OV_USE_HDF5
    bool putToDataBase=true;    // hdf5  -- put on all processors
  #else
    bool putToDataBase= parameters.dbase.get<int >("myid")==0; // hdf4 - only put on processor 0
  #endif

  // printf(" ***** Maxwell:: saveShow: myid=%i, putToDataBase=%i\n",myid,(int)putToDataBase);
    

  if( putToDataBase )
  {
    dbp = showFile.getFrame();
    assert( dbp!=NULL );
    // save parameters that go in this frame
    assert( dbp!=NULL );
    HDF_DataBase & db = *dbp;
    db.put(t,"time");
    db.put(dt,"dt");
  }


  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  
  char buffer[80]; 
  aString showFileTitle[5];

  showFileTitle[0]=sPrintF(buffer,"Maxwell %s",(const char *)methodName);
  showFileTitle[1]=sPrintF(buffer,"t=%4.3f, dt=%8.2e",t,dt);
  showFileTitle[2]="";  // marks end of titles

  for( int i=0; showFileTitle[i]!=""; i++ )
    showFile.saveComment(i,showFileTitle[i]);

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


//    realMappedGridFunction & fieldCurrent = mgp==NULL ? getCGField(HField,current)[grid] : fields[current];


  if( mgp==NULL )
  {
    // save a CompositeGridFunction...
    
    realCompositeGridFunction v;
    realCompositeGridFunction & u = getAugmentedSolution(current,v,t);

    // realCompositeGridFunction & u = getCGField(HField,current);

    if( saveGridInShowFile )
    {
      if( debug & 4 ) 
         printF("***Save grid in the show file: numberOfComponentGrids=%i\n",cg.numberOfComponentGrids());
      showFile.saveSolution( u,"u",Ogshow::useCurrentFrame );  // save the grid and the grid function
      saveGridInShowFile=false;
      showFileFrameForGrid=showFile.getNumberOfFrames();
    }
    else
    {
      if( debug & 4 )
        printF("***Save solution in the show file: showFileFrameForGrid=%i (-2=default)\n",
	     showFileFrameForGrid);
      showFile.saveSolution( u,"u",showFileFrameForGrid );  // save the grid function
    }
  }
  else
  {
    // Save a MappedGridFunction...
    realMappedGridFunction & u = fields[current];
    if( saveGridInShowFile )
    {
      if( debug & 4 )
        printF("***Save grid in the show file: numberOfComponentGrids=%i\n",cg.numberOfComponentGrids());
      showFile.saveSolution( u,"u",Ogshow::useCurrentFrame );  // save the grid and the grid function
      saveGridInShowFile=false;
      showFileFrameForGrid=showFile.getNumberOfFrames();
    }
    else
    {
      if( debug & 4 )
        printF("***Save solution in the show file: showFileFrameForGrid=%i (-2=default)\n",
	     showFileFrameForGrid);
      showFile.saveSolution( u,"u",showFileFrameForGrid );  // save the grid function
    }
  }
  
  // Here we save time sequences to the show file
  // Only save if this is the last frame in a subFile
  if( dbase.get<bool >("saveSequencesEveryTime") && showFile.isLastFrameInSubFile() )
  {  
//    #ifndef USE_PPP
      // fix me for parallel -- adding this causes the program to hang when closing the show file.
      saveSequencesToShowFile();
//    #endif
  }
  showFile.endFrame();  

  timing(timeForShowFile)+=getCPU()-cpu0;
}


int Maxwell::
saveParametersToShowFile()
// =================================================================================================
// /Description:
//     Save PDE specific parameters in the show file.
//     These parameters can be used for a restart. They can also be used, for example,
//     by the user defined derived functions (when viewing the show file with plotStuff).
// 
//\end{OB_ParametersInclude.tex}  
// =================================================================================================
{
  assert( show!=NULL );

  ListOfShowFileParameters showFileParams;

  // save parameters
  showFileParams.push_back(ShowFileParameter("Maxwell's Equations","pde"));
    
  showFileParams.push_back(ShowFileParameter("exFieldComponent",ex));
  showFileParams.push_back(ShowFileParameter("eyFieldComponent",ey));
  showFileParams.push_back(ShowFileParameter("ezFieldComponent",ez));

  showFileParams.push_back(ShowFileParameter("hxFieldComponent",hx));
  showFileParams.push_back(ShowFileParameter("hyFieldComponent",hy));
  showFileParams.push_back(ShowFileParameter("hzFieldComponent",hz));

  showFileParams.push_back(ShowFileParameter("eps",eps));
  showFileParams.push_back(ShowFileParameter("mu",mu));

  show->saveGeneralParameters(showFileParams);
    

  return 0;
}


//\begin{>>MaxwellInclude.tex}{\subsection{saveSequenceInfo}} 
int Maxwell::
saveSequenceInfo( real t0, RealArray & sequenceData )
//=========================================================================================
// /Description:
//    Save info into the time history arrays.
// 
//\end{MaxwellInclude.tex}  
//=========================================================================================
{
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



//\begin{>>MaxwellInclude.tex}{\subsection{saveSequencesToShowFile}} 
int Maxwell::
saveSequencesToShowFile()
//=========================================================================================
// /Description:
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  if( show==NULL || sequenceCount<=0 )
    return 0;
  
   assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
 
  Range I(0,sequenceCount-1);
  Range N=sequence.dimension(1);
  
  // Is this next line correct?
  // int numberOfComponents = method==nfdtd ? hz-ex+1 : max(ey,ez) + hz + 1 -ex +1;
  int numberOfComponents=0;
  if( method==nfdtd )
  {
    numberOfComponents = 3;
  }
  else if( method==sosup )
  {
    // 2D: ex,ey,hz, ext,eyt,hzt  
    // 3D: ex,ey,ez, ext,eyt,ezt  
    numberOfComponents = 6; 
  }
  else if( method==yee )
  {
    numberOfComponents = numberOfDimensions==2 ? 3 : 6;
  }
  else
  {
    numberOfComponents = max(ey,ez) + hz + 1 -ex +1;
  }
  
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
    name[numberOfComponents+1]="Delta_E";
  }
  
  // display(sequence(I,N),"saveSequencesToShowFile: sequence(I,N)");
  
  printf("saveSequencesToShowFile() myid=%i sequenceCount=%i\n",myid,sequenceCount);
  fflush(0);


  // NOTE: This function must be called by ALL processors in parallel
  show->saveSequence("errors",timeSequence(I),sequence(I,N),name);
  
  delete [] name;
  
  return 0;
}
