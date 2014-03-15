#include "Oges.h"  
#include "OGTrigFunction.h"  // Trigonometric function
#include "OGPolyFunction.h"  // polynomial function
#include "NameList.h"        // For inputing values by name
#include "PlotStuff.h"

void printArray( realCompositeGridFunction & u );
void assignRightHandSide( Oges & og, realCompositeGridFunction & f, 
                          OGFunction & exactSolution );
void assignRightHandSide( Oges & og, realCompositeGridFunction & f, 
                          RealArray & constraintRHS );

#define ForBoundary(side,axis)   for( axis=0; axis<og.numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

//====================================================================
//    Test Program for the Overlapping Grid Equation Solver OGES
//    ----------------------------------------------------------
//
// Who to blame:
//   Bill Henshaw
//======================================================================
void ogest( Oges & og, CompositeGrid & cg, int & errorNumber )
{
  cout << "Enter ogest..." << endl;
  
  IntegerArray bc0(3,2,og.numberOfGrids); bc0.setBase(1);
  aString errorMessage;
  NameList nl;
  
  cout << "set default values.. " << endl;
  
  bc0=0;

  // Here are the parameters for this test routine
  bool plotResults=FALSE;
  int twilightZone=1;  // 1=trig, 2=poly
  int printOptions = 1;  // 1:print max errors
  RealArray constraintRHS(10); constraintRHS=0.;
  int computeIntegrationWeights=0;
  int saveLeftNullVector=0;
  int solveNonlinearProblem=0;
  int numberOfNewtonIterations=10;
  int numberOfQuasiNewton=1; // number of quasi-Newton steps per Newton step
  int assignInitialConditions=0;
  real fx=2., fy=2., fz=2.;
  int iplot=0;
  real epsc=1.e-5; //  convergence for Newton
  int ipf=0;       // for path following


  // Here are the parameters for oges:
  // Default values:  *** use get ***
  int conjugateGradientType=0;
  int conjugateGradientPreconditioner=0;
  int orderOfAccuracy=2;
  int equationType=Oges::LaplaceDirichlet;
  int preconditionBoundary=FALSE;
  int numberOfComponents=1;
  int conjugateGradientNumberOfIterations=0;
  int solverType=Oges::yale;
  int debug = 15;
  real matrixCutoff=0.;
  real zeroRatio=0.;
  real fillinRatio=0.;
  real harwellTolerance=.1;
  int transpose=FALSE;
  int coefficientType=Oges::continuous;  // equations are defined in this manner
  int numberOfGhostLines=1;
  const int maxNumberOfGhostLines=5;
  const int maxNumberOfComponents=10;
  IntegerArray ghostLineOption(maxNumberOfGhostLines,maxNumberOfComponents);
  ghostLineOption=Oges::extrapolateGhostLine;
  
  aString name(80),answer(80);
  printf(
   " Parameters for this test routine: \n"
   " -------------------------------- \n"
   "   name                                type       default \n"
   "plotResults                            (bool)     %4i     \n"
   "twilightZone (1=trig, 2=poly)          (int)      %4i     \n"
   "printOptions (1=max err, 8=errors)     (int)      %4i     \n"
   "contraintRHS                           (RealArray)        \n"
   "computeIntegrationWeights              (bool)     %4i     \n"
   "saveLeftNullVector                     (bool)     %4i     \n"
   "solveNonlinearProblem                  (bool)     %4i     \n"
   "assignInitialConditions                (bool)     %4i     \n"
   "bc0(kd,ks,k) new bc's                  (IntegerArray)         \n"
   " Parameters for oges: \n"
   " -------------------- \n"
   "   name                                type       default \n"
   "coefficientType  (0=cont, 1=discrete)  (int)      %4i     \n"
   "conjugateGradientType                  (int)      %4i     \n"
   "conjugateGradientNumberOfIterations    (int)     default  \n"
   "conjugateGradientPreconditioner        (int)      %4i     \n"
   "debug                                  (int)      %4i     \n"
   "equationType                           (int)      %4i     \n"
   "ghostLineOption(ghostLine,n)           (int)      %2i     \n"
   "harwellTolerance                       (real)     %4e     \n"
   "fillinRatio                            (real)     %4e     \n"
   "matrixCutoff                           (real)     %4e     \n"
   "numberOfGhostLines                     (int)      %4i     \n"
   "numberOfComponents                     (int)      %4i     \n"
   "orderOfAccuracy                        (int)      %4i     \n"
   "preconditionBoundary                   (bool)     %4i     \n"
   "solverType                             (int)      %4i     \n"
   "transpose                              (int)      %4i     \n"
   "zeroRatio                              (real)     %4e     \n",

   plotResults,
   twilightZone,
   printOptions,
   computeIntegrationWeights,
   solveNonlinearProblem,
   assignInitialConditions,
   saveLeftNullVector,
   coefficientType ,
   conjugateGradientType,
   conjugateGradientPreconditioner,
   debug,
   equationType,
   ghostLineOption(1,0),
   fillinRatio,
   harwellTolerance,
   matrixCutoff,
   numberOfGhostLines,
   numberOfComponents,
   orderOfAccuracy,
   preconditionBoundary,
   solverType,
   transpose,
   zeroRatio
	 );
  
  // ==========Loop for changing parameters========================
  for( ;; ) 
  {
    cout << "Enter changes to variables, exit to continue" << endl;
    cin >> answer;
    
    if( answer=="exit" ) break;

    nl.getVariableName( answer, name );   // parse the answer

    if( name=="twilightZone" )
    {
      twilightZone=nl.intValue(answer);  
      cout << " twilightZone=" << twilightZone << endl;
    }
    else if( name=="printOptions" )
    {
      printOptions=nl.intValue(answer);  
      cout << " printOptions=" << printOptions << endl;
    }
    else if( name=="constraintRHS" )
    {
      nl.getRealArray( answer,constraintRHS );
      cout << " constraintRHS(0)=" << constraintRHS(0) << endl;
    }
    else if( name=="computeIntegrationWeights" )
    {
      computeIntegrationWeights=nl.intValue(answer);  
      cout << " computeIntegrationWeights=" << computeIntegrationWeights << endl;
    }
    else if( name=="saveLeftNullVector" )
    {
      saveLeftNullVector=nl.intValue(answer);  
      cout << " saveLeftNullVector=" << saveLeftNullVector << endl;
    }
    else if( name=="solveNonlinearProblem" )
    {
      solveNonlinearProblem=nl.intValue(answer);  
      cout << " solveNonlinearProblem=" << solveNonlinearProblem << endl;
    }
    else if( name=="assignInitialConditions" )
    {
      assignInitialConditions=nl.intValue(answer);  
      cout << " assignInitialConditions=" << assignInitialConditions << endl;
    }
    else if( name=="bc0" )
    {
      nl.getIntArray( answer,bc0 );  
    }

    else if( name=="coefficientType" )
    {
      coefficientType=nl.intValue(answer);  
      cout << " coefficientType=" << coefficientType << endl;
      og.setCoefficientType(Oges::coefficientTypes(coefficientType));
    }
    else if( name=="conjugateGradientNumberOfIterations" )
    {
      conjugateGradientNumberOfIterations=nl.intValue(answer);  
      cout << " conjugateGradientNumberOfIterations=" << conjugateGradientNumberOfIterations << endl;
      og.setConjugateGradientNumberOfIterations(conjugateGradientNumberOfIterations);
    }
    else if( name=="conjugateGradientType" )
    {
      conjugateGradientType=nl.intValue(answer);  
      cout << " conjugateGradientType=" << conjugateGradientType << endl;
      og.setConjugateGradientType(Oges::conjugateGradientTypes(conjugateGradientType));
    }
    else if( name=="conjugateGradientPreconditioner" )
    {
      conjugateGradientPreconditioner=nl.intValue(answer);  
      cout << " conjugateGradientPreconditioner=" << conjugateGradientPreconditioner << endl;
      og.setConjugateGradientPreconditioner(
         Oges::conjugateGradientPreconditioners(conjugateGradientPreconditioner));
    }
    else if( name=="debug" )
    {
      debug=nl.intValue(answer);  
      cout << " debug=" << debug << endl;
      Oges::debug=debug;
    }
    else if( name=="plotResults" )
    {
      plotResults=nl.intValue(answer);  
      cout << " plotResults=" << plotResults << endl;
    }
    else if( name=="equationType" )
    {
      equationType=nl.intValue(answer);  
      cout << " equationType=" << equationType << endl;
      og.setEquationType(Oges::equationTypes(equationType));
    }
    else if( name=="fillinRatio" )
    {
      fillinRatio=nl.realValue(answer);  
      cout << " fillinRatio=" << fillinRatio << endl;
      og.setFillinRatio(fillinRatio);
    }
    else if( name=="ghostLineOption" )
    {
      int ghost,n;
      nl.getIntArray( answer,ghostLineOption, ghost,n );
      printf(" ghostLineOption(%i,%i)=%i\n",ghost,n,ghostLineOption(ghost,n));
      og.setGhostLineOption(ghost,Oges::ghostLineOptions(ghostLineOption(ghost,n)),n);
    }
    else if( name=="harwellTolerance" )
    {
      harwellTolerance=nl.realValue(answer);  
      cout << " harwellTolerance=" << harwellTolerance << endl;
      og.setHarwellTolerance(harwellTolerance);
    }
    else if( name=="matrixCutoff" )
    {
      matrixCutoff=nl.realValue(answer);  
      cout << " matrixCutoff=" << matrixCutoff << endl;
      og.setMatrixCutoff(matrixCutoff);
    }
    else if( name=="numberOfComponents" )
    {
      numberOfComponents=nl.intValue(answer);  
      cout << " numberOfComponents=" << numberOfComponents << endl;
      og.setNumberOfComponents(numberOfComponents);
    }
    else if( name=="numberOfGhostLines" )
    {
      numberOfGhostLines=nl.intValue(answer);  
      cout << " numberOfGhostLines=" << numberOfGhostLines << endl;
      og.setNumberOfGhostLines(numberOfGhostLines);
    }
    else if( name=="orderOfAccuracy" )
    {
      orderOfAccuracy=nl.intValue(answer);  
      cout << " orderOfAccuracy=" << orderOfAccuracy << endl;
      og.setOrderOfAccuracy(orderOfAccuracy);
    }
    else if( name=="preconditionBoundary" )
    {
      preconditionBoundary=nl.intValue(answer);  
      cout << " preconditionBoundary=" << preconditionBoundary << endl;
      og.setPreconditionBoundary(preconditionBoundary);
    }
    else if( name=="solverType" )
    {
      solverType=nl.intValue(answer);  
      cout << " solverType=" << solverType << endl;
      og.setSolverType(Oges::solvers(solverType));
    }
    else if( name=="transpose" )
    {
      transpose=nl.intValue(answer);  
      cout << " transpose=" << transpose << endl;
      og.setTranspose(transpose);
    }
    else if( name=="zeroRatio" )
    {
      zeroRatio=nl.realValue(answer);  
      cout << " zeroRatio=" << zeroRatio << endl;
      og.setZeroRatio(zeroRatio);
    }
    else
      cout << "unknown response: [" << name << "]" << endl;

  }


  //  ==== Special Boundary Conditions ====
  if( equationType==Oges::LaplaceMixed )
  {
    cout << "****Assign boundary conditions for LaplaceMixed..." << endl;
    int side,axis;
    for( int grid=0; grid<og.numberOfGrids; grid++ )
    {
      ForBoundary(side,axis)
      {
        og.operators[grid].setNumberOfBoundaryConditions( 1,side,axis );
	if( cg[grid].boundaryCondition()(side,axis)==1 )
	{
          printf("Setting bc on grid %i, side %i, axis %i to Dirichlet...\n",grid,side,axis);
          og.operators[grid].setBoundaryCondition(0,                  // boundary condition number
						  side,axis,          // applied to this boundary
						  MappedGridOperators::dirichlet,
						  0);                 // applied to this component
//          og.setGhostLineOption(1,Oges::useGhostLineExceptCornerAndNeighbours,0,grid,side,axis);
	}
	else if( cg[grid].boundaryCondition()(side,axis) > 1 )
	{
          printf("Setting bc on grid %i, side %i, axis %i to Neumann...\n",grid,side,axis);
          og.operators[grid].setBoundaryCondition(0,                  // boundary condition number
						  side,axis,          // applied to this boundary
						  MappedGridOperators::neumann,
						  0);                 // applied to this component
//          og.setGhostLineOption(1,Oges::useGhostLineExceptCorner,0,grid,side,axis);
	}
      }
    }
  }  

//    og.setReorderRows( TRUE ); // **** add this ****
//    og.setFillinRatio2( fratio2 );
//    og.setConjugateGradientNumberOfSaveVectors( nsave );
//    og.setConjugateGradientTolerance( tol );
//    og.setSorNumberOfIterations(nit);
//    og.setSorTolerance( tol );                          // tol
//    og.setSorOmega( omega );                              // omega


  errorNumber=og.initialize();  // initialize Oges (assign classify array used for rhs routine)
  

  //............Number of multi-grid levels:

/* ------
  cout << "look for mg..." << endl;
  int mg = id( cdskfnd( &id(1),mgdir,"mg") );
  if( l<1 || l>mg )
  {
    cout << "OGEST>>>Error invalid l, l,mg =" << l << "," << mg << endl;
    l=max(1,min(mg,l));
    cout << "....Continuing with new l =" << l << endl;
  }
----- */
  
  //........Directory pointer to multigrid level l

  //  int cgdir= cdskfnd( &id(1),mgdir,"composite grid")+l-1;   // ** is this used? ***
  //  cout << "cgdir=" << cgdir << endl;
  
  
  //........print dimensions of grids

  printf("             Grid Dimensions \n"
         "             --------------- \n"
         "               indexRange               gridDimensions    isPeriodic \n"
         "  grid  ra  rb  sa  sb  ta  tb   ra  rb  sa  sb  ta  tb    r  s  t\n"
	 );
  for( int grid=0; grid<og.numberOfGrids; grid++ )
  {
    printf("   %2i %4i%4i%4i%4i%4i%4i %4i%4i%4i%4i%4i%4i    %i  %i  %i  \n",
    grid,
    cg[grid].indexRange()(Start,axis1),cg[grid].indexRange()(End,axis1),
    cg[grid].indexRange()(Start,axis2),cg[grid].indexRange()(End,axis2),
    cg[grid].indexRange()(Start,axis3),cg[grid].indexRange()(End,axis3),
    cg[grid].dimension()(Start,axis1),cg[grid].dimension()(End,axis1),
    cg[grid].dimension()(Start,axis2),cg[grid].dimension()(End,axis2),
    cg[grid].dimension()(Start,axis3),cg[grid].dimension()(End,axis3),
    cg[grid].isPeriodic()(axis1),cg[grid].isPeriodic()(axis2),cg[grid].isPeriodic()(axis3) );
  }
  
  Range all;
  realCompositeGridFunction f(cg,all,all,all,numberOfComponents),u1(cg,all,all,all,numberOfComponents);
  realCompositeGridFunction uTZ(cg,all,all,all,numberOfComponents);       // Twilight-zone flow

  OGFunction *tz;
  OGTrigFunction tzTrig(fx,fy,fz);  // create an exact solution
  int degreeOfSpacePolynomial=2;
  OGPolyFunction tzPoly(degreeOfSpacePolynomial,cg.numberOfDimensions());      // create an exact solution
  if( twilightZone==1 )
    tz = &tzTrig;
  else
    tz = &tzPoly;
    
  tz->assignGridFunction( uTZ );  // gives values to uTZ
  

  if( assignInitialConditions )
  {
    // Specify some Initial Conditions:
    RealArray uv0(og.numberOfComponents);
    cout << "ogest: >>>>Specify Initial Conditions<<<< " << endl;
    cout << "     : Enter the Uniform State uv0(n) n=0,..," << og.numberOfComponents-1
         << endl;
    for( int n=0; n<og.numberOfComponents; n++ )
      cin >> uv0(n);

    for( int grid=0; grid<og.numberOfGrids; grid++ )
    {
      Index I1(u1[grid].getBase(axis1),u1[grid].getLength(axis1));
      Index I2(u1[grid].getBase(axis2),u1[grid].getLength(axis2));
      Index I3(u1[grid].getBase(axis3),u1[grid].getLength(axis3));
      for( n=0; n<og.numberOfComponents; n++ )
      {
        u1[grid](I1,I2,I3,n)=uv0(n);
      }
    }
    
  }
  else if( solverType>2 )
  {
    // give initial values for iterative solvers
    u1=0.;
  }
  

  
  real time1;


  if( !solveNonlinearProblem && ipf==0 )  // linear && no path following
  {
    // ===linear problem

    // ...Assign the right hand side
    cout << "call ogesr..." << endl;

    if( twilightZone )
      assignRightHandSide( og,f,*tz );
    else
      assignRightHandSide( og,f,constraintRHS ); // ...add forcing for real live run
    

    if( Oges::debug & 32 )
      f.display(" ***ogest: here is the rhs f:");
    
    
    //  ===call the Equation solver===
    int pu1=0;

    time1=getCPU();
    
    cout << "Call oges..." << endl;
    errorNumber=og.solve( u1,f );

    if( Oges::debug & 16 )
    {
      u1.display("solution after first solve","%5.2f ");
      f.display("f after first solve","%5.2f ");
    }
    
    cout << "Time for oges =" << getCPU()-time1 << endl;
    if( errorNumber==0 )
    {
      cout << "Call oges again..." << endl;
      time1=getCPU();
      errorNumber=og.solve( u1,f );
      cout << "Time for oges =" << getCPU()-time1 << endl;
    }
    
    cout << "ogest>> errorNumber=" << errorNumber << ", comment=" << og.getErrorMessage(errorNumber) << endl;
    if( errorNumber >  0 )
    {
      cerr << "ogetst>> Fatal error from solver" << endl;
      exit(1);
    }

/* ----
    if( saveLeftNullVector ) //   ...save the left null vector
    {
      cout << "Call CGEST1..." << endl;      
      CGEST1( id(1),rd(1),root,l,cgdir,u1vn,ng,nv,errorNumber, strlen(u1vn[0]) );
    }
    
--- */

    if( computeIntegrationWeights )
    {
      // --- Compute Integration coefficients ---

      if( Oges::debug & 4 )
        u1.display(">>>>>>>>>>>Here are the weights before scaling<<<<<<<<<<<"); 
      cout << " Call scaleIntegrationCoefficients..." << endl;
      og.scaleIntegrationCoefficients( u1 );   // weights saved in u1
      if( Oges::debug & 4 )
        u1.display(">>>>>>>>>>>>>>>Here are the weights after scaling<<<<<<<<<"); 
      
      // integrate a function:
      int degreeOfPolynomial=0;
      OGPolyFunction poly(degreeOfPolynomial,og.numberOfDimensions);    // define a function
      poly.assignGridFunction(f);

      real volumeIntegral,surfaceIntegral;
      og.integrate( u1,f,volumeIntegral,surfaceIntegral );   // Integrate f

      exit(0);
    }

  }
  else if( solveNonlinearProblem && ipf==0 ) // nonlinear and no path following
  {
    //       === Nonlinear Problem ===
    //
    //           Solve:  F(u)=0   ( or F(u) = F(twilightZoneFlow) )
    //
    //    u1 : current guess at solution
    //    u2 : correction
    //    residual : F(twilightZoneFlow)-F(u1)
    //    f  : right hand side for TZ flow, f = F(twilightZoneFlow)
      
    realCompositeGridFunction correction(cg,1,3),residual(cg,1,3);
      
    int grid;
    real maximumResidual=0.;
    real maximumCorrection;
	

    // ...to determine the rhs and residual evaluate F(u) instead of the Jacobian

    cout << "Newton: assign the right hand side..." << endl;
     
    og.uLinearized.reference(uTZ);            // set pointer to linearized solution
    og.setEvaluateJacobian( FALSE );  // evaluate F(u) not F_u

//    if( twilightZone )
//      og.ogesrtz( f,tz );     //  ...add forcing so that the true solution is known
//    else
//      og.ogesrc( f,constraintRHS ); // ...add forcing for real live run

    if( twilightZone )
      assignRightHandSide( og,f,*tz );
    else
      assignRightHandSide( og,f,constraintRHS ); // ...add forcing for real live run

    og.setEvaluateJacobian( TRUE );   // reset to evaluate F_u
    og.uLinearized.reference(u1);             // set pointer to linearized solution

    //   --- Iterate until convergence ---
    for( int it=1; it<=numberOfNewtonIterations*(numberOfQuasiNewton+1); it++ )
    {

      cout << "Newton: call ogres..." << endl;

      og.setEvaluateJacobian( FALSE );                  // evaluate F(u) not F_u
      og.ogres( u1, f, residual, maximumResidual );     // Determine : residual <- F(uTZ) - F(u)
      og.setEvaluateJacobian( TRUE );                   // evaluate F_u
	
	
      if( Oges::debug & 2 )
        cout << "ogest: it = " << it << ", maximumResidual = " << maximumResidual << endl;

      if( Oges::debug & 32 )
      {
	cout << " ****Solution**** it= " << it-1 << endl;
	printArray( u1 );
	cout << " ****Residual**** it= " << it << endl;
	printArray( residual );
      }

      time1=getCPU();

      cout << "Newton: call oges..." << endl;
      errorNumber=og.solve( correction,residual );      // compute the correction

      if( Oges::debug & 4 )
        cout << "Time for oges =" << getCPU()-time1 << endl;

      if( errorNumber > 0 )
      {
        cerr << "ogest>> Fatal error from solver" << endl;
        cout << "ogest>>" << og.getErrorMessage(errorNumber) << endl;
        exit(1);
      }

      // refactor every numberOfQuasiNewton steps
      og.setRefactor( ( it % (numberOfQuasiNewton+1) ) ==0 ); 
      
      if( Oges::debug & 32 )
      {
	cout << " ****Correction**** it= " << it << endl;
	printArray( correction );
      }

      maximumCorrection=0.;
      for( grid=0; grid<og.numberOfGrids; grid++ )
      {
	maximumCorrection=max(maximumCorrection,max(fabs(correction[grid])));
        u1[grid]=u1[grid]+correction[grid];
      }

      if( Oges::debug & 1 )
      {
	printf(" ogest: it =%2i, max corr =%10.2e, max residual=%10.2e \n",
		 it,maximumCorrection,maximumResidual);
      }

      if( fabs(maximumCorrection) < epsc ) break;
    }
  }
  else
  {
    //    ===Continuation problem
    //    ---call path following routine
    //  call cgpf( id,rd,nd,ng,nv,idopt,pu1vn,pu2vn,pfvn,pu3vn,pu4vn,
    //&   id(pu1vn),id(pu2vn),id(pfvn),id(pu3vn),id(pu4vn),
    //&   u1vn,u2vn,fvn,u3vn,u4vn,cgdir,wdir,flags,pijac,
    //&   ip,rp,id(pipcf),rd(prpcf),idebug,icf,ierr )

  }

  if( twilightZone ) // ...Calculate the maximum error  (for Twilight-zone flow )
    og.determineErrors(  u1, *tz, printOptions );
  
  if( plotResults )
  {

    PlotStuff ps;                                 // create a PlotStuff object
    PlotStuffParameters psp;                      // This object is used to change plotting parameters
    
    aString answer;
    aString menu[] = { "contour",                  // Make some menu items
		      "stream lines",
		      "grid",
		      "read command file",
		      "save command file",
		      "erase",
		      "exit",
		      "" };                       // empty string denotes the end of the menu
    for(;;)
    {
      ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
      if( answer=="contour" )
      {
	psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
	PlotIt::contour(ps,u1,psp);                        // contour/surface plots
      }
      else if( answer=="grid" )
      {
	PlotIt::plot(ps,cg);                              // plot the composite grid
      }
      else if( answer=="erase" )
      {
	ps.erase();
      }
      else if( answer=="exit" )
      {
	break;
      }
    }
  }

}



//==============================================================================
// Output a realCompositeGridFunction
//==============================================================================
void printArray( realCompositeGridFunction & u ) // ofstream & outputFile )
{
//  ofstream errorFile( "oges2.out", ios::out );  // or use ios::app
//  if( !errorFile )
//  {
//    cerr << "ogmxer: error opening the errorFile! " << endl;
//    exit (1);
//  }

  for( int grid=0; grid < u.numberOfComponentGrids(); grid++ )
  {
    printf(" ---------grid = %6i ---------------\n",grid);
    for( int n=u[grid].getBase(axis3+1); n<=u[grid].getBound(axis3+1); n++ )
    {
      for( int i3=u[grid].getBase(axis3); i3<=u[grid].getBound(axis3); i3++ )
      {
        if( u[grid].getBound(axis3)-u[grid].getBase(axis3) > 0 )
          printf("   ++++ i3= %6i +++\n",grid);
	for( int i2=u[grid].getBase(axis2); i2<=u[grid].getBound(axis2); i2++ )
	{
          for( int i1=u[grid].getBase(axis1); i1<=u[grid].getBound(axis1); i1++ )
          {
            printf(" %6.2e",u[grid](i1,i2,i3,n));
	  }
	  printf("\n");
	}
      }
    }
  }
}











