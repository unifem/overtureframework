// *************************************************************
// *********** Test Oges Solvers (in parallel)     *************
// *************************************************************

// **** NOTE: set petsc options in .petscrc


// mpirun -np 2 -all-local tos -ksp_monitor
// mpirun -np 2 -all-local -gdb tos 
//
// mcr: 
//    mpirun-wdh -np 1 tos
//    mpirun-wdh -np 2 tos


// /* Program usage:  mpirun -np <procs> ex2 [-help] [all PETSc options] */ 

// static char help[] = "Solves a linear system in parallel with KSP.\n\
// Input parameters include:\n\
//   -random_exact_sol : use a random exact solution vector\n\
//   -view_exact_sol   : write exact solution vector to stdout\n\
//   -m <mesh_x>       : number of mesh points in x-direction\n\
//   -n <mesh_n>       : number of mesh points in y-direction\n\n";



#include "Overture.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"

#include "PETScSolver.h"

#include "gridFunctionNorms.h"
#include "PlotStuff.h"


#undef __FUNCT__
#define __FUNCT__ "main"
int main(int argc,char **argv)
{
  int debug=1;
  bool usePredefined=true;

  Overture::start(argc,argv);  // initialize Overture
  Optimization_Manager::setForceVSG_Update(Off);
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());
  
  printF("Usage: `tos file.cmd' \n");


  // Here is where we output the primary results
  aString resultsFileName="tos.results";

//  aString nameOfOGFile="square5.hdf";
//  aString nameOfOGFile="square5np.hdf";  // polynomial TZ does not work in this case
//  aString nameOfOGFile="annulus.hdf";  // polynomial TZ does not work in this case
  aString nameOfOGFile="sise.hdf";
// aString nameOfOGFile="sisa.hdf";   // this matches
//   aString nameOfOGFile="square32.hdf";
//    aString nameOfOGFile="sic1.hdf";
//    aString nameOfOGFile="cice.hdf";
//    aString nameOfOGFile="cic3e.hdf";
//    aString nameOfOGFile="cic6e.hdf";
//    aString nameOfOGFile="square256.hdf";


  #ifdef USE_PPP
    aString fileName = "tosInputFile";
    ParallelUtility::getArgsFromFile(fileName,argc,argv );
  #endif

  
  int bcOption=0;  // 0=Dirichlet, 1=Neumann BC's, 2= mixed
  int singular=1;
  int luSolver=0;  // 1= superlu
  bool useDirectBlockSolver=false;
  bool useBoomerAMG=false;
  int solveAgainWithConvergedSolution=true;
  aString commandFileName="";

  if( argc > 1 )
  {
    aString line;
    int i,len=0;
    for( i=1; i<argc; i++ )
    {
      if( argv[i]!=0 )
        line=argv[i];
//       if( len=line.matches("-grid=")  )
//       {
// 	nameOfOGFile=line(len,line.length()-1);
//       }
//       else if( line.matches("-neumann") )
//       {
// 	bcOption=1;
//       }
//       else if( line.matches("-useBoomer") )
//       {
// 	useBoomerAMG=true;
//       }
//       else if( line.matches("-usePredefined") )
//       {
// 	usePredefined=true; 
//         if( myid<=0 ) printf(" *** usePredefined equations*** \n");
//       }
//       else if( len=line.matches("-singular=") )
//       {
// 	sScanF(line(len,line.length()-1),"%i",&singular);
//         if( myid<=0 ) printf(" Setting singular=%i\n",singular);
//       }
//       else if( line.matches("-superlu") )
//       {
// 	luSolver=1; 
//         if( myid<=0 ) printf(" Setting luSolver=%i (SuperLU)\n",luSolver);
//       }
//       else if( line.matches("-useDirectBlockSolver") )
//       {
// 	useDirectBlockSolver=true; 
//         if( myid<=0 ) printf(" use a direct solver on each block\n");
//       }

      commandFileName = line;
      printf("tos: reading commands from file [%s]\n",(const char*)commandFileName);

    }
  }
  #ifdef USE_PPP
    ParallelUtility::deleteArgsFromFile(argc,argv);
  #endif

  PlotStuff gi(false,"testing oges solvers");

  // By default start saving the command file called "tos.cmd"
  aString logFile="tos.cmd";
  gi.saveCommandFile(logFile);
  cout << "User commands are being saved in the file `" << (const char *)logFile << "'\n";

  gi.appendToTheDefaultPrompt("tos>");
  // read from a command file if given
  if( commandFileName!="" )
  {
    cout << "read command file =" << commandFileName << endl;
    gi.readCommandFile(commandFileName);
  }

  // get options:

  aString answer;
  int len=0;
  for( ;; )
  {
    gi.inputString(answer,"Enter the option or `done' to finish");
    if( answer=="done" || answer=="exit" )
    {
      break;
    }
    else if( len=answer.matches("grid=") )
    {
      nameOfOGFile=answer(len,answer.length()-1);
    }
    else if( len=answer.matches("results=") )
    {
      resultsFileName=answer(len,answer.length()-1);
    }
    else if( len=answer.matches("solveAgainWithConvergedSolution") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&solveAgainWithConvergedSolution);
    }
    else if( answer=="bc=dirichlet" || answer=="bc=neumann" || answer=="bc=mixed" )
    {
      bcOption = answer=="bc=dirichlet" ? 0 : answer=="bc=neumann" ? 1 : answer=="bc=mixed" ? 2 : -1;
      assert( bcOption>=0 );
    }
    else if( len=answer.matches("debug=") )
    {
      sScanF(answer(len,answer.length()-1),"%i",&debug);
      printF(" Setting debug=%i\n",debug);
    }
  }
  
    // create and read in a CompositeGrid
  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  // cg.update();
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( !cg[grid].isRectangular() )
      cg[grid].update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEvertexBoundaryNormal );
    else
      cg[grid].update(MappedGrid::THEmask | MappedGrid::THEvertexBoundaryNormal );
  }


  int grid;
  // make a grid function to hold the coefficients
  Range all;
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1.5 );  // add 1 for interpolation equations
  realCompositeGridFunction coeff;

    
  // create grid functions: 
  realCompositeGridFunction uu(cg),f(cg),w(cg),eu(cg);

  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);
  
  // make some shorter names for readability
  BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
                   extrapolate           = BCTypes::extrapolate,
                   allBoundaries         = BCTypes::allBoundaries; 


  IntegerArray bc(2,3,cg.numberOfComponentGrids());
  RealArray bcData(2,2,3,cg.numberOfComponentGrids());
  if( bcOption==0 )
  {
    bc=OgesParameters::dirichlet;
  }
  else if( bcOption==1 )
  {
    bc=OgesParameters::neumann;
  }
  else if( bcOption==2 )
  {
    bc=OgesParameters::neumann;

    // apply a mixed BC at "outflow" which we assume is the right face of grid 0
    // bc(1,0,0)=OgesParameters::dirichlet;
    int side=1, axis=0, grid=0;  
     
    bc(side,axis,grid)=OgesParameters::mixed;  
    bcData(0,side,axis,grid)=1.; // coeff of u 
    bcData(1,side,axis,grid)=1.; // coeff of u.n 

  }
  
  int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2]; 
  int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2]; 
  Index I1,I2,I3;

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {  
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );

    realArray & vertex = mg.vertex();
    realSerialArray wLocal; getLocalArrayWithGhostBoundaries(w[grid],wLocal);
    realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
    realSerialArray xLocal; getLocalArrayWithGhostBoundaries(vertex,xLocal); 
    
    

    // true solution is x+y [+z]
    if( cg.numberOfDimensions()==2 )
      wLocal=xLocal(all,all,all,0) + xLocal(all,all,all,1);
    else
      wLocal=xLocal(all,all,all,0) + xLocal(all,all,all,1) + xLocal(all,all,all,2);
    
    // coeff[grid].updateGhostBoundaries();
    
    // display(coeff[grid],sPrintF("coeff on grid=%i",grid),"%6.4f ");

    // Assign the RHS

    fLocal=0.;
    
    intSerialArray mask; getLocalArrayWithGhostBoundaries(cg[grid].mask(),mask);
      
    realArray & ug= uu[grid];
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(ug,uLocal); 

    int isv[3],  &is1=isv[0], &is2=isv[1], &is3=isv[2];
    
    for( int side=0; side<=1; side++ )
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	if( mg.boundaryCondition(side,axis)>0 )
	{
  	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
    
          #ifdef USE_PPP
            const realSerialArray & normal = mg.vertexBoundaryNormalArray(side,axis);
          #else
            const realSerialArray & normal = mg.vertexBoundaryNormal(side,axis);
          #endif
	  int n1a = max(I1.getBase(), fLocal.getBase(0)); 
	  int n1b = min(I1.getBound(),fLocal.getBound(0)); 
	  int n2a = max(I2.getBase(), fLocal.getBase(1)); 
	  int n2b = min(I2.getBound(),fLocal.getBound(1)); 
	  int n3a = max(I3.getBase(), fLocal.getBase(2)); 
	  int n3b = min(I3.getBound(),fLocal.getBound(2)); 
      
	  const IntegerArray & gid = cg[grid].gridIndexRange(); 

          real nSign=2*side-1;  // sign of the outward normal
          is1=is2=is3=0;
	  isv[axis]=1-2*side;
	  for( i3=n3a; i3<=n3b; i3++ )
	    for( i2=n2a; i2<=n2b; i2++ )
	      for( i1=n1a; i1<=n1b; i1++ )
	      {
		// if( i1<=gid(0,0) || i1>=gid(1,0) || i2<=gid(0,1) || i2>=gid(1,1) ) // || mask(i1,i2,i3)==0 )
                if( mask(i1,i2,i3)>0 )
		{
                  if( bc(side,axis,grid)==OgesParameters::dirichlet )
		  {
     		    fLocal(i1,i2,i3)=wLocal(i1,i2,i3);  // Dirichlet BC
		  }
                  else if( bc(side,axis,grid)==OgesParameters::neumann )
		  {
                    if( cg.numberOfDimensions()==2 )
		      fLocal(i1-is1,i2-is2,i3-is3)=normal(i1,i2,i3,0)+normal(i1,i2,i3,1);
		    else
		      fLocal(i1-is1,i2-is2,i3-is3)=normal(i1,i2,i3,0)+normal(i1,i2,i3,1)+normal(i1,i2,i3,2);
		  }
                  else if( bc(side,axis,grid)==OgesParameters::mixed )
		  {
                    if( cg.numberOfDimensions()==2 )
		    {
		      fLocal(i1-is1,i2-is2,i3-is3)=bcData(0,side,axis,grid)*wLocal(i1,i2,i3) + 
			bcData(1,side,axis,grid)*(normal(i1,i2,i3,0)+normal(i1,i2,i3,1));
		    }
		    else
		    {
		      fLocal(i1-is1,i2-is2,i3-is3)=bcData(0,side,axis,grid)*wLocal(i1,i2,i3) + 
			bcData(1,side,axis,grid)*(normal(i1,i2,i3,0)+normal(i1,i2,i3,1)+normal(i1,i2,i3,2));
		    }
		    
		  }
		  
		  
		}
	      }
	}
      }
  }
  
  
//   int ngp=0;
//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//   {  
//     realArray & coeffg= coeff[grid];
//     int nd1a = coeffg.getBase(1), nd1b=coeffg.getBound(1);
//     int nd2a = coeffg.getBase(2), nd2b=coeffg.getBound(2);
//     int nd3a = coeffg.getBase(3), nd3b=coeffg.getBound(3);

//     ngp+= (nd1b-nd1a+1)*(nd2b-nd2a+1)*(nd3b-nd3a+1);
//   }
//   int numberOfGridPoints=ngp;
  
//   if( myid==0 ) printf(" Total number of grid points is numberOfGridPoints=%i\n",numberOfGridPoints);
  


  int numberOfProcessors=Communication_Manager::Number_Of_Processors;


  Vec            u;  /* exact solution */

  PetscReal      norm;     /* norm of solution error */
  PetscInt       i,j,I,J,Istart,Iend,m = 8,n = 7,its;
  PetscErrorCode ierr;
  PetscTruth     flg;
  PetscScalar    v,one = 1.0,neg_one = -1.0;

  Oges & oges = * new Oges;
  int solverType=OgesParameters::PETScNew;
  oges.set(OgesParameters::THEsolverType,solverType); 

  if( usePredefined )
  {
    oges.setGrid(cg);
    
    
    oges.setEquationAndBoundaryConditions( OgesParameters::laplaceEquation,op,bc,bcData ); 
  }
  else
  {
    coeff.updateToMatchGrid(cg,stencilSize,all,all,all); 
    coeff.setIsACoefficientMatrix(true,stencilSize);  
    coeff=0.;
    coeff.setOperators(op);

    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid]);
    }
    // fill in the coefficients for the boundary conditions
    if( bcOption==0 )
    {
      coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries); // extrap ghost line
    }
    else
    {
      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::neumann,  allBoundaries);
    }
  
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      coeff[grid].finishBoundaryConditions();
    }

    oges.setCoefficientArray(coeff);
  }
  
  real relativeTol=1.e-7;

  oges.set(OgesParameters::THErelativeTolerance,relativeTol);


//   // oges.set(OgesParameters::THEparallelSolverMethod,OgesParameters::gmres);
//   oges.set(OgesParameters::THEparallelSolverMethod,OgesParameters::biConjugateGradientStabilized);
//   oges.set(OgesParameters::THEparallelSolverMethod,OgesParameters::richardson);

//   //   oges.set(OgesParameters::THEsolverMethod,OgesParameters::gmres);
//   // oges.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
//   oges.set(OgesParameters::THEsolverMethod,OgesParameters::preonly);  // solver on each block (each processor)

//   int iluLevels=3;
//   oges.set(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
  
//   // The parallel preconditioner can be blockJacobiPreconditioner, additiveSchwarzPreconditioner or luPreconditioner
//   oges.set(OgesParameters::THEparallelPreconditioner,OgesParameters::blockJacobiPreconditioner);
//   // oges.set(OgesParameters::THEparallelPreconditioner,OgesParameters::additiveSchwarzPreconditioner);

//   if( useDirectBlockSolver )
//   {
//     // here we use an lu method -- in parallel this will be the solver on each processor
//     oges.set(OgesParameters::THEsolverMethod,OgesParameters::preonly);
//     oges.set(OgesParameters::THEpreconditioner,OgesParameters::luPreconditioner);
//   }
  


//   // for SuperLU or other LU solvers
//   if( luSolver==1 )
//   {
//     oges.set(OgesParameters::THEsolverMethod,OgesParameters::preonly);
//     oges.set(OgesParameters::THEpreconditioner,OgesParameters::luPreconditioner);

//     oges.set(OgesParameters::THEparallelExternalSolver,OgesParameters::superlu_dist);

//     // PETScSolver::luSolver=luSolver;  // do this for now
//   }
  
  OgesParameters & ogesParameters = oges.parameters;
  
  ogesParameters.update(gi,cg);

//   if( useBoomerAMG )
//   {
//     ogesParameters.setPetscOption("-pc_type","hypre" );
//     ogesParameters.setPetscOption("-pc_hypre_type","boomeramg");

//     ogesParameters.setPetscOption("-pc_hypre_boomeramg_strong_threshold",".5");
//     // -pc_hypre_boomeramg_coarsen_type <Falgout> (one of) CLJP Ruge-Stueben  modifiedRuge-Stueben   Falgout
//     ogesParameters.setPetscOption("-pc_hypre_boomeramg_coarsen_type","Falgout");
//     // PetscOptionsSetValue("-pc_hypre_boomeramg_coarsen_type","modifiedRuge-Stueben");
//     //   PetscOptionsSetValue("-pc_hypre_boomeramg_coarsen_type","CLJP");
//     //   PetscOptionsSetValue("-pc_hypre_boomeramg_coarsen_type","Ruge-Stueben");
    
//   }
  
  Oges & solver = oges;

  if( bcOption==1 ) 
  {
    Overture::abort("finish this");
//    solver.setProblemIsSingular((PETScSolver::SingularProblemEnum)singular);
  }
  
//   if( bcOption==1 ) solver.setProblemIsSingular((PETScSolver::SingularProblemEnum)singular);
  // if( bcOption==1 ) solver.setProblemIsSingular(PETScSolver::specifyNullVector);
  // if( bcOption==1 ) solver.setProblemIsSingular(PETScSolver::addExtraEquation);
  
//  solver.buildMatrix(coeff,uu);
//  solver.buildSolver();
    
  uu=0.;
  
  real time1=getCPU();
  solver.solve( uu,f );
  time1=ParallelUtility::getMaxValue(getCPU()-time1);
  // ierr = KSPGetIterationNumber(solver.ksp,&its);CHKERRQ(ierr);
  its=solver.getNumberOfIterations();
  printF("\n ===== Time for 1st solve=%8.2e (iterations=%i) =====\n\n",time1,its);

  // extract a name of the solver -- here we get the name from petsc:
  aString name;
  const int maxLen=100;
  char buff[maxLen+1];
  PetscOptionsGetString(PETSC_NULL,"-ksp_type",buff,maxLen,&flg);
  if( flg )
  {
    name += buff;
    PetscOptionsGetString(PETSC_NULL,"-pc_type",buff,maxLen,&flg);
    aString pcType=buff;
    name = name + "-" + pcType;
    if( pcType=="hypre" )
    {
      PetscOptionsGetString(PETSC_NULL,"-pc_hypre_type",buff,maxLen,&flg);
      aString hypreType=buff;
      if( hypreType=="boomeramg" ) hypreType="AMG";
      name = name + "-" + hypreType;
    }
    if( pcType!="hypre" )
    {
      PetscOptionsGetString(PETSC_NULL,"-sub_ksp_type",buff,maxLen,&flg);
      name = name + "-" + buff;

      PetscOptionsGetString(PETSC_NULL,"-sub_pc_type",buff,maxLen,&flg);
      aString subPCType=buff;
      if( subPCType=="ilu" )
      {
//        PetscOptionsGetString(PETSC_NULL,"-sub_pc_ilu_levels",buff,maxLen,&flg);
        PetscOptionsGetString(PETSC_NULL,"-sub_pc_factor_levels",buff,maxLen,&flg);
        if( flg )
          name = name + "-" + subPCType + "(" + buff + ")";
      }
      else
      {
	name = name + "-" + subPCType;
      }
      
    }
  }
  else
  {
    name=ogesParameters.getSolverName();
  }
  

// -sub_ksp_type gmres
// -sub_pc_ilu_levels


  resultsFileName = resultsFileName + sPrintF(answer,".np%i",np);

  FILE *resultsFile=NULL;
  if( myid==0 ) resultsFile = fopen((const char*)resultsFileName,"w");


  int its2;
  real time2;
  if( true || debug & 2 )
  {
    uu=0.;  // start from scratch

    time2=getCPU();
    solver.solve( uu,f );
    time2=ParallelUtility::getMaxValue(getCPU()-time2);
    // ierr = KSPGetIterationNumber(solver.ksp,&its);CHKERRQ(ierr);
    its2=solver.getNumberOfIterations();
    printF("\n ===== Time for 2nd solve=%8.2e (iterations=%i)=====\n\n",time2,its2);

  }

  real time3;
  int its3;
  if( solveAgainWithConvergedSolution )
  {
    time3=getCPU();
    solver.solve( uu,f );
    time3=ParallelUtility::getMaxValue(getCPU()-time3);
    // ierr = KSPGetIterationNumber(solver.ksp,&its);CHKERRQ(ierr);
    its3=solver.getNumberOfIterations();
    printF("\n ===== Time for 3rd solve=%8.2e (iterations=%i)=====\n\n",time3,its3);

  }
  


  

//   if( bcOption!=0 )
//   {
//     real uSum=0., wSum=0.;
//     int count=0;
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     { 
//       uSum+=sum(uu[grid]);
//       wSum+=sum(w[grid]);
//       count+=;
//     }
    
//     real diffSum=(wSum-diffSum)/count;
//     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     { 
//       uu[grid]+=diffSum; 
//     }
    
//   }
  

  const int extra=1;  // number of ghost pts

  // compute the error
  eu=0.; 
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {  
    MappedGrid & mg = cg[grid];
    mg.update( MappedGrid::THEcenter );
    intSerialArray mask; getLocalArrayWithGhostBoundaries(cg[grid].mask(),mask);
    realSerialArray uLocal; getLocalArrayWithGhostBoundaries(uu[grid],uLocal);
    realSerialArray wLocal; getLocalArrayWithGhostBoundaries(w[grid],wLocal);
    realSerialArray euLocal; getLocalArrayWithGhostBoundaries(eu[grid],euLocal);
    
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
    int n1a = max(I1.getBase(), uLocal.getBase(0)); 
    int n1b = min(I1.getBound(),uLocal.getBound(0)); 
    int n2a = max(I2.getBase(), uLocal.getBase(1)); 
    int n2b = min(I2.getBound(),uLocal.getBound(1)); 
    int n3a = max(I3.getBase(), uLocal.getBase(2)); 
    int n3b = min(I3.getBound(),uLocal.getBound(2)); 

    for( i3=n3a; i3<=n3b; i3++ )
      for( i2=n2a; i2<=n2b; i2++ )
	for( i1=n1a; i1<=n1b; i1++ )
	{
	  if( mask(i1,i2,i3)!=0 )
	    euLocal(i1,i2,i3)=wLocal(i1,i2,i3)-uLocal(i1,i2,i3);
	}
    
  }
  
  real norm2 = l2Norm(eu,0,0,extra);
  real normMax = maxNorm(eu,0,0,extra);
  
  if( debug & 2 )
  {
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {  
      display(uu[grid],sPrintF("Solution: uu[%i]",grid),"%5.2f ");
    }
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {  
      display(eu[grid],sPrintF("Errors: eu[%i]",grid),"%8.1e ");
    }
  }
  
  if( myid==0 ) printf(" ERRORS: l2=%9.3e max=%9.3e max-residual=%8.2e (iterations=%i)\n",
                norm2,normMax,solver.getMaximumResidual(),its );
  

//   if( debug & 4 )
//      KSPView(solver.ksp,PETSC_VIEWER_STDOUT_WORLD);


  /*
     Always call PetscFinalize() before exiting a program.  This routine
       - finalizes the PETSc libraries as well as MPI
       - provides summary and diagnostic information if certain runtime
         options are chosen (e.g., -log_summary). 
  */

//  solver.destroy();

//  solver.finalizePETSc();  // do this if we don't call the destructor before now
  
//  ierr = PetscFinalize();CHKERRQ(ierr);


  printF("\n ** results written to file %s\n",(const char*)resultsFileName);

  real factorTime=time1-time2;
  real solveTime = time2;
  aString bcName = (bcOption==0 ? "dirichlet" : bcOption==1 ? "neumann" : bcOption==2 ? "mixed" : "??");
  
//  fPrintF(resultsFile,"solver=%s\n",(const char *)ogesParameters.getSolverName());
  fPrintF(resultsFile,"solver=%s\n",(const char *)name);
  fPrintF(resultsFile,"bc=%s\n",(const char*)bcName);
  fPrintF(resultsFile,"grid=%s\n",(const char*)nameOfOGFile);
  fPrintF(resultsFile,"np=%i\n",np);
  fPrintF(resultsFile,"factorTime=%e\n"
                    "factorAndSolveIterations=%i\n",factorTime,its);
  fPrintF(resultsFile,"solveTime=%e\n"
	    "solveIterations=%i\n",solveTime,its2);
  fPrintF(resultsFile,"reSolveTime=%e\n"
	    "reSolveIterations=%i\n",time3,its3);

  // the final line in the results file is for insertion in a latex table
  //     NP       & factor(s)  & solve(s) &  its    &  option             
  aString gridName = nameOfOGFile;
  int lastChar = gridName.length()-1;  
  if( lastChar>3 && gridName(lastChar-3,lastChar)==".hdf"  )
     gridName = gridName(0,lastChar-4);  // remove ".hdf"
  fPrintF(resultsFile,"  NP   & factor(s)  & solve(s) &  its    \\\\\n");
  fPrintF(resultsFile,"  %i   &  %9.2f  & %9.2f & %i \\\\ \n",
	  np, factorTime,solveTime,its2);

  fPrintF(resultsFile,"\\caption{Results for grid %s, %s, %s boundary conditions. } \n",
	  (const char*)gridName,(const char *)name,(const char*)bcName);


  fPrintF(stdout,"  NP   & factor(s)  & solve(s) &  its    \\\\\n");
  fPrintF(stdout,"  %i   &  %9.2f  & %9.2f & %i \\\\ \n",np, factorTime,solveTime,its2);

  if( resultsFile!=NULL ) fclose(resultsFile);
  
  delete &oges;

  Overture::finish();          

  return 0;
}
