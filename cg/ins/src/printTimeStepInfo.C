#include "Cgins.h"
#include "ParallelUtility.h"
#include "App.h"

#define getDivAndNorms EXTERN_C_NAME(getdivandnorms)
extern "C"
{

 void getDivAndNorms(const int &nd,
      const int &n1a,const int &n1b,const int &n2a,const int &n2b,const int &n3a,const int &n3b,
      const int &nd1a,const int &nd1b,const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,
      const int &nd4a,const int &nd4b,
      const int &mask,const real &xy,const real &rsxy, const real &u, 
      const real &div,  const int &ipar, real &rpar, const int &ierr );
}



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{printTimeStepInfo}} 
void Cgins::
printTimeStepInfo( const int & step, const real & t, const real & cpuTime )
//=================================================================================
// /Description:
//    Print information about the current solution in a nicely formatted way
//  ** This is a virtual function **
//\end{CompositeGridSolverInclude.tex}  
//=================================================================================
{
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *checkFile = parameters.dbase.get<FILE* >("checkFile");
  const int & debug = parameters.dbase.get<int >("debug");
  const RealArray & checkFileCutoff = parameters.dbase.get<RealArray >("checkFileCutoff");
  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel >("pdeModel");
  Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  
  GridFunction & solution = gf[current];

  if( (false || debug & 4) &&  // *wdh* 060302
      (false || !parameters.dbase.get<bool >("twilightZoneFlow")) )
  {
    if( parameters.dbase.get<int >("myid")==0 ) 
      fprintf(debugFile," ***printTimeStepInfo: Solution at t=%e***\n",t);
    outputSolution( solution.u,t );
  }

  int n;
  RealArray error(numberOfComponents+5);  

  // ===Check errors, print results====
  determineErrors( solution.u,solution.gridVelocity,t,0,error );

  // determine the max/min of all components: uMax, uMin, uvMax
  RealArray uMin(numberOfComponents), uMax(numberOfComponents);
  real uvMax;
  // *wdh* 060909 getSolutionBounds(uMin,uMax,uvMax);
  getBounds(solution.u,uMin,uMax,uvMax);

  real divMax=0., vorMax=0., divl2Norm=0.;
  if( true )// !parameters.dbase.get<bool >("twilightZoneFlow") && 
     //      (parameters.dbase.get<Parameters::PDE >("pde")==Parameters::incompressibleNavierStokes ||
     //       parameters.dbase.get<Parameters::PDE >("pde")==Parameters::allSpeedNavierStokes) )
  {
    divMax=0.;
    vorMax=0.;
    divl2Norm=0.;
    int numberOfPoints=0;
      
    CompositeGrid & cg = *solution.u.getCompositeGrid();
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const realArray & u = solution.u[grid];
      MappedGrid & mg = cg[grid];
      const IntegerArray & gid = mg.gridIndexRange();
	
      bool isRectangular = mg.isRectangular();
      const int gridType = isRectangular ? 0 : 1;
	
      real dx[3]={1.,1.,1.};
      real xab[2][3]={0.,1.,0.,1.,0.,1.};
      if( isRectangular )
	mg.getRectangularGridParameters( dx, xab );

      int i1a=mg.gridIndexRange(0,0);
      int i2a=mg.gridIndexRange(0,1);
      int i3a=mg.gridIndexRange(0,2);
	
      int ierr=0;
      const int option=0; // compute norms but do not save div(i1,i2,i3)
      int ipar[20] ={ parameters.dbase.get<int >("pc"),parameters.dbase.get<int >("uc"),parameters.dbase.get<int >("vc"),parameters.dbase.get<int >("wc"),grid,parameters.dbase.get<int >("orderOfAccuracy"),
                      (int)parameters.isAxisymmetric(),gridType,option,
		      i1a,i2a,i3a,0,0,0,0,0,0,0,0	};  //
	

      const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
      real rpar[20]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2), dx[0],dx[1],dx[2],
		     xab[0][0],xab[0][1],xab[0][2],yEps,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};  //


#ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      const real *pu = uLocal.getDataPointer();
      const real *prsxy = isRectangular ? pu : mg.inverseVertexDerivative().getLocalArray().getDataPointer();
      const real *pxy = isRectangular ? pu : mg.vertex().getLocalArray().getDataPointer();
      const int *pmask = mg.mask().getLocalArray().getDataPointer();
#else
      const realSerialArray & uLocal=u; 
      const real *pu = uLocal.getDataPointer();
      const real *prsxy = isRectangular ? pu : mg.inverseVertexDerivative().getDataPointer();
      const real *pxy = isRectangular ? pu : mg.vertex().getDataPointer();
      const int *pmask = mg.mask().getDataPointer();
#endif

      const int n1a = max(gid(0,0), uLocal.getBase(0)+u.getGhostBoundaryWidth(0));
      const int n1b = min(gid(1,0),uLocal.getBound(0)-u.getGhostBoundaryWidth(0));
      const int n2a = max(gid(0,1), uLocal.getBase(1)+u.getGhostBoundaryWidth(1));
      const int n2b = min(gid(1,1),uLocal.getBound(1)-u.getGhostBoundaryWidth(1));
      const int n3a = max(gid(0,2), uLocal.getBase(2)+u.getGhostBoundaryWidth(2));
      const int n3b = min(gid(1,2),uLocal.getBound(2)-u.getGhostBoundaryWidth(2));

      if( n1a>n1b || n2a>n2b || n3a>n3b ) continue;
      
      const real *pdiv = pu; // not used

      getDivAndNorms(mg.numberOfDimensions(),
		     n1a,n1b,n2a,n2b,n3a,n3b,
		     uLocal.getBase(0),uLocal.getBound(0),
		     uLocal.getBase(1),uLocal.getBound(1),
		     uLocal.getBase(2),uLocal.getBound(2),
		     uLocal.getBase(3),uLocal.getBound(3),
		     *pmask, *pxy, *prsxy, *pu, *pdiv,  ipar[0], rpar[0], ierr );

      divMax=max(divMax,rpar[10]);
      vorMax=max(vorMax,rpar[11]);
      divl2Norm+=rpar[12];
      numberOfPoints+=ipar[10];
    } // end for grid

    divMax=ParallelUtility::getMaxValue(divMax);
    vorMax=ParallelUtility::getMaxValue(vorMax);
    numberOfPoints=ParallelUtility::getSum(numberOfPoints);
    divl2Norm=ParallelUtility::getSum(divl2Norm);
      

    divl2Norm = sqrt(divl2Norm/max(1,numberOfPoints));
      
  }
  
  if( turbulenceModel!=Parameters::noTurbulenceModel && !parameters.dbase.get<bool >("twilightZoneFlow") )
  { // Output y+ of first grid line etc.
    computeTurbulenceQuantities(solution);
  }


  // In Parallel -- compute the max over all statistics
  // ***** For some values it woudl be better to compute sum(times)/NP
  for( int i=0; i<parameters.dbase.get<RealArray >("statistics").getLength(0); i++ )
  {
    parameters.dbase.get<RealArray>("statistics")(i)=ParallelUtility::getMaxValue(parameters.dbase.get<RealArray>("statistics")(i));
  }

  if( parameters.dbase.get<int >("myid")!=0 ) return; // only print on processor 0

  int numberOfComponentsToOutput = numberOfComponents;
  if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    numberOfComponentsToOutput-=2; // do not output errors for density and viscosity
  }
  

  aString blanks="                            ";
  aString *& componentName = parameters.dbase.get<aString* >("componentName");
  if( parameters.dbase.get<bool >("twilightZoneFlow") || parameters.dbase.get<realCompositeGridFunction* >("pKnownSolution") )
  {
    // ****************************************************************
    // *********** twilightzone flow or knownSolution *****************
    // ****************************************************************
    for( int io=0; io<3; io++ )
    {
      FILE *file = io==0 ? stdout : io==1 ? parameters.dbase.get<FILE* >("debugFile") : 
                                            parameters.dbase.get<FILE* >("logFile");

      // do not put cpu times into the debug file so we can compare serial to parallel for e.g.
      const real cpu = io!=1 ? cpuTime : 0.;  

      if( file==NULL ) continue;   

      if( numberOfComponentsToOutput<10 )
      {
	if( step<=2 || ((step-1) % 10 == 0) || true || // always print header *wdh* 080509
            parameters.dbase.get<int >("showResiduals")!=0 || debug>1 )
	{ // print the header: 
	  if( !parameters.isSteadyStateSolver() )
            fprintf(file,"     t ");
          else
            fprintf(file,"       it");

	  for( n=0; n<numberOfComponentsToOutput; n++)
	    fprintf(file,"   err(%s)",(const char*)componentName[n]);
	    fprintf(file,"    div   ");
	  fprintf(file,"    uMax     dt       cpu\n");
	}
	if( !parameters.isSteadyStateSolver() )
          fprintf(file," %7.3f",t);
        else
          fprintf(file,"%10i",parameters.dbase.get<int >("globalStepNumber")+1);
	for( n=0; n<numberOfComponentsToOutput; n++)
	{
          fprintf(file,"%s",(const char*)blanks(0,componentName[n].length()-1));  // add extra blanks for long names 
	  fprintf(file,"%8.2e",error(n));
	}
        fprintf(file," %8.2e ",divMax);
	fprintf(file," %8.2e %8.2e %8.2e \n",uvMax,dt,cpu);
      }
      else
      {
	if( !parameters.isSteadyStateSolver() )
	  fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds (%i steps)\n",t,dt,cpu,parameters.dbase.get<int >("globalStepNumber")+1);
	else
	  fprintf(file," >>> it= %10i, dt =%9.2e, cpu =%9.2e seconds \n",parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu);

	for( n=0; n<numberOfComponentsToOutput; n++ )
	  fprintf(file," %s%s : (min,max)=(%13.6e,%13.6e), error=%10.3e \n",
		 (const char*)blanks(0,max(0,10-componentName[n].length())),
		 (const char*)componentName[n],
		 uMin(n),uMax(n),error(n));
	  fprintf(file," Divergence: divMax/vorMax = %8.2e divl2Norm/vorMax=%8.2e vorMax=%8.2e\n",
		  divMax/max(REAL_MIN*100.,vorMax),divl2Norm/max(REAL_MIN*100.,vorMax),vorMax);

	fprintf(file,"Max errors:");
	for( n=0; n<numberOfComponentsToOutput; n++ )
	  fprintf(file," %10.3e  &",error(n));
	fprintf(file,"\n");
      }
    }

//      // print errors to the debug file
//      fprintf(debugFile," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds \n",t,dt,cpuTime);
//      for( n=0; n<numberOfComponentsToOutput; n++ )
//        fprintf(debugFile," %s%s : (min,max)=(%13.6e,%13.6e), error=%16.9e \n",
//                (const char*)blanks(0,max(0,10-componentName[n].length())),
//  	      (const char*)componentName[n],
//                   uMin(n),uMax(n),error(n));

    // output results to the check file
    fprintf(checkFile,"%9.2e %i  ",t,numberOfComponentsToOutput+2); // print |\uv| and divergence too.

    for( n=0; n<numberOfComponentsToOutput; n++ )
    {
      real err = error(n) > checkFileCutoff(n) ? error(n) : 0.;
      real uc = max(fabs(uMin(n)),fabs(uMax(n)));
      if( uc<checkFileCutoff(n) ) uc=0.;
      fprintf(checkFile,"%i %9.2e %10.3e  ",n,err,uc);
    }
    
      real err=max(error(parameters.dbase.get<int >("uc")),error(parameters.dbase.get<int >("vc")));
      real uvMax= max(fabs(uMin(parameters.dbase.get<int >("uc"))),fabs(uMax(parameters.dbase.get<int >("uc"))),
                      fabs(uMin(parameters.dbase.get<int >("vc"))),fabs(uMax(parameters.dbase.get<int >("vc"))));

      if( parameters.dbase.get<int >("numberOfDimensions")==3 )
      {
        err=max(err,error(parameters.dbase.get<int >("wc")));
	uvMax=max(uvMax,fabs(uMin(parameters.dbase.get<int >("wc"))),fabs(uMax(parameters.dbase.get<int >("wc"))));
      }
      if( err < checkFileCutoff(parameters.dbase.get<int >("uc")) ) 
        err=0.;
      fprintf(checkFile,"%i %9.2e %10.3e  ",numberOfComponentsToOutput,err,uvMax);  // max error in u,v,w
      
      real divc=divMax > checkFileCutoff(numberOfComponentsToOutput) ? divMax : 0.;
      fprintf(checkFile,"%i %9.2e %10.3e  ",numberOfComponentsToOutput+1,divc,uvMax);
    fprintf(checkFile,"\n");

  }
  else
  {
    // **********************************************
    // ************** real run **********************
    // **********************************************
    for( int io=0; io<3; io++ )
    {
      FILE *file = io==0 ? stdout : io==1 ? parameters.dbase.get<FILE* >("debugFile") : parameters.dbase.get<FILE* >("logFile");
      if( file==NULL ) continue;
      
      // do not put cpu times into the debug file so we can compare serial to parallel for e.g.
      const real cpu = io!=1 ? cpuTime : 0.;  

      if( !parameters.isSteadyStateSolver() )
        fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds (%i steps)\n",t,dt,cpu,parameters.dbase.get<int >("globalStepNumber")+1);
      else
        fprintf(file," >>> it=   %10i, dt =%9.2e, cpu =%9.2e seconds \n",parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu);
      for( n=0; n<numberOfComponentsToOutput; n++ )
        fprintf(file," %s%s : (min,max)=(%13.6e,%13.6e) \n",
		(const char*)blanks(0,max(0,10-componentName[n].length())),
		(const char*)componentName[n],
		uMin(n),uMax(n));

//        fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds \n",t,dt,cpu);
//        for( n=0; n<numberOfComponentsToOutput; n++ )
//          fprintf(file," %s%s : (min,max)=(%14.7e,%14.7e) \n",
//  		(const char*)blanks(0,max(0,10-componentName[n].length())),
//  		(const char*)componentName[n],
//  		uMin(n),uMax(n));

        fprintf(file," Divergence: divMax/vorMax = %8.2e divl2Norm/vorMax=%8.2e vorMax=%8.2e\n",
		divMax/max(REAL_MIN*100.,vorMax),divl2Norm/max(REAL_MIN*100.,vorMax),vorMax);

      // *wdh* 030317 fprintf(file," maximum divergence on all interior points: divMax = %e \n",divMax);
      
    }

    // output results to the check file
    fprintf(checkFile,"%8.1e %i  ",t,numberOfComponentsToOutput);
    for( n=0; n<numberOfComponentsToOutput; n++ )
    {
      real uMinCheck = fabs(uMin(n))<checkFileCutoff(n) ? 0. : uMin(n);
      real uMaxCheck = fabs(uMax(n))<checkFileCutoff(n) ? 0. : uMax(n);
      fprintf(checkFile,"%i %8.1e %10.3e  ",n,uMinCheck,uMaxCheck);
    }
    fprintf(checkFile,"\n");

  }


  if( true )
    checkArrayIDs(" printTimeStepInfo: done");
 

  checkArrays(sPrintF(" printTimeStepInfo (t=%9.3e)",t));  
}
