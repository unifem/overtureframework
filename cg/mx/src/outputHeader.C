#include "Maxwell.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "OgmgParameters.h"

// =======================================================================================
/// \brief Output the header banner with parameters and grid info.
// =======================================================================================
void Maxwell::
outputHeader()
{
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

  int numberOfComponents=0;
  if( cg.numberOfDimensions()==2 )
  {
    numberOfComponents= method==yee || method==nfdtd || method==sosup ? 
      (int)numberOfComponentsRectangularGrid : 
      (int)numberOfComponentsCurvilinearGrid; 
	    
  }
  else
  {
    numberOfComponents=(int(solveForElectricField)+int(solveForMagneticField))*3;
  }

  for( int fileio=0; fileio<2; fileio++ )
  {
    FILE *file = fileio==0 ? logFile : stdout; 
    fPrintF(file,"\n"
	    "******************************************************************\n"
	    "           Cgmx : Maxwell Solver                    \n"
	    "           ---------------------                  \n");

    fPrintF(file," tFinal=%f, dt=%9.3e, tPlot=%9.3e cfl=%3.2f adr=%3.2f, adc=%3.2f  \n",
	    tFinal,deltaT,tPlot,cfl,artificialDissipation,artificialDissipationCurvilinear );

    
    fPrintF(file," Using method %s\n",(const char *)methodName);

    if( timeSteppingMethod==modifiedEquationTimeStepping )
      fPrintF(file," Time stepping method is modifiedEquation\n");
    

    fPrintF(file," order of accuracy: space=%i, time=%i\n",orderOfAccuracyInSpace,orderOfAccuracyInTime);
    fPrintF(file," artificial diffusion: order=%i, coefficient=%8.2e (rectangular grids), coefficient=%8.2e (curvilinear grids)\n",
            orderOfArtificialDissipation,artificialDissipation,artificialDissipationCurvilinear);
    if( applyFilter )
      fPrintF(file," apply high order filter, order=%i, frequency=%i, iterations=%i, coefficient=%g\n",
                     orderOfFilter,filterFrequency,numberOfFilterIterations,filterCoefficient);
    else
      fPrintF(file," do not apply the high order filter\n");
    fPrintF(file," divergence damping coefficient=%8.2e\n",divergenceDamping);
            
    fPrintF(file," divergence cleaning is %s. coefficient=%g\n",(useDivergenceCleaning ? "on" : "off"),divergenceCleaningCoefficient);

    const bool & useNewForcingMethod= dbase.get<bool>("useNewForcingMethod");
    const int & numberOfForcingFunctions= dbase.get<int>("numberOfForcingFunctions"); 
    fPrintF(file," Work-space:\n");
    fPrintF(file,"   Solution arrays: numberOfTimeLevels=%i (%i components)\n",numberOfTimeLevels,numberOfComponents);
    fPrintF(file,"   RHS : numberOfFunctions=%i  (%i components).\n",numberOfFunctions,numberOfComponents);
    fPrintF(file,"   External forcing: useNewForcingMethod=%i, numberOfForcingFunctions=%i (%i components).\n",
            (int)useNewForcingMethod,numberOfForcingFunctions,numberOfComponents);
    

    fPrintF(file," plane wave solution: (kx,ky,kz)=(%8.2e,%8.2e,%8.2e), omega=%8.2e \n"
                 "     E: a=(%8.2e,%8.2e,%8.2e), H: b=(%8.2e,%8.2e,%8.2e)\n",
                   kx,ky,kz,omegaTimeHarmonic,pwc[0],pwc[1],pwc[2],pwc[3],pwc[4],pwc[5]);
    if( forcingOption==twilightZoneForcing )
      fPrintF(file," Twilightzone flow is on.");
    if( forcingOption==twilightZoneForcing && twilightZoneOption==polynomialTwilightZone )
      fPrintF(file," Polynomial solution, degreeSpace=%i, degreeTime=%i\n",degreeSpace,degreeTime);
    else
      fPrintF(file,"\n");

    if( method==nfdtd )
    {
      MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[0];
      fPrintF(file," curvilinear grid operators use %s difference approximations.\n",
	      mgop.usingConservativeApproximations() ? "conservative" : "non-conservative" );
      
    }
    if( projectFields )
    {
      fPrintF(file," Project fields to satisfy divergence conditions, projection frequency=%i\n",
              frequencyToProjectFields);
    }
    else
    {
      fPrintF(file," Do not project fields to satisfy divergence conditions\n");
    }
    
    
    if( projectFields && poisson!=NULL  )
    {
      OgesParameters & ogesParameters=poisson->parameters;
      if( !poisson->isSolverIterative() )
      {
	fPrintF(file," project fields using solver: solver=%s, \n",
		(const char*)ogesParameters.getSolverName()); 
      }
      else
      {
	real tolerance;
	int maximumNumberOfIterations;
      
	ogesParameters.get(OgesParameters::THErelativeTolerance,tolerance);
	ogesParameters.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	fPrintF(file," project fields using solver: solver=%s, \n"
		"                         : tolerance=%8.2e, max number of iterations=%i (0=choose default)\n",
		(const char*)ogesParameters.getSolverName(),tolerance,maximumNumberOfIterations);
      }
	
    }
    if( useVariableDissipation )
    {
      fPrintF(file," use variable dissipation (only add dissipation near interpolation points\n");
    }
    if(  max(adjustFarFieldBoundariesForIncidentField)>0 )
    {
      fPrintF(file," adjust far field boundaries for any incident fields.\n");
    }
    
    fPrintF(file," %s project interpolation points to satisfy the divergence constraint.\n",
	    ( projectInterpolation ? "do" : "do not" ));
    

    if( true )
    {
      fPrintF(file," PML parameters: width=%i, strength=%g, power=%i\n",numberLinesForPML,pmlLayerStrength,pmlPower);
    }
    
    aString initialConditionName[numberOfInitialConditionNames]={
      "defaultInitialCondition",
      "planeWaveInitialCondition",
      "gaussianPlaneWave",
      "gaussianPulseInitialCondition",
      "squareEigenfunctionInitialCondition",  
      "annulusEigenfunctionInitialCondition",
      "zeroInitialCondition",
      "planeWaveScatteredFieldInitialCondition",
      "planeMaterialInterfaceInitialCondition",
      "gaussianIntegralInitialCondition",   
      "twilightZoneInitialCondition",
      "userDefinedInitialConditionsOption",
      "userDefinedKnownSolutionInitialCondition"
    };

    if( initialConditionOption>=0 && initialConditionOption<numberOfInitialConditionNames )
      fPrintF(file,"\n initialConditionOption = %s\n",(const char*)initialConditionName[initialConditionOption]);
    else
      fPrintF(file," initialConditionOption = %i\n",(const int)initialConditionOption);

    if( projectInitialConditions )
      fPrintF(file," Project initial conditions to satisfy divergence constraint.\n");
    else
      fPrintF(file," Do not project initial conditions to satisfy divergence constraint.\n");

    const RealArray & icBox = initialConditionBoundingBox;
    if( (icBox(0,0) <= icBox(1,0)) && ( icBox(0,1) <= icBox(1,1) ) )
    {
      fPrintF(file," initialConditionBoundingBox=[%9.2e,%9.2e][%9.2e,%9.2e][%9.2e,%9.2e]\n",
	      icBox(0,0),icBox(1,0),
	      icBox(0,1),icBox(1,1),
	      icBox(0,2),icBox(1,2)); 
    }
    else
    {
      fPrintF(file," initialConditionBoundingBox is OFF.\n");
    }
    
    aString forcingName[numberOfForcingNames]={
      "noForcing",
      "magneticSinusoidalPointSource",
      "gaussianSource",
      "twilightZoneForcing",
      "planeWaveBoundaryForcing",
      "gaussianChargeSource",
      "userDefinedForcingOption"
    };
    if( forcingOption>=0 && forcingOption< numberOfForcingNames )
     fPrintF(file," forcingOption = %s\n",(const char*)forcingName[forcingOption]);
    else
    fPrintF(file," forcingOption = %i\n",(const int)forcingOption);


    const aString & knownSolutionName=dbase.get<aString>("knownSolutionName");
    fPrintF(file," knownSolutionOption = %s\n",(const char*)knownSolutionName);
    // fPrintF(file," knownSolutionOption = %i\n",(const int)knownSolutionOption);
    
    const int & setDivergenceAtInterfaces = dbase.get<int>("setDivergenceAtInterfaces");
    const int & useImpedanceInterfaceProjection = dbase.get<int>("useImpedanceInterfaceProjection");
    fPrintF(file,"\n");
    fPrintF(file," materialInterfaceOption=%i (1=extrap ghost as initial guess)\n",materialInterfaceOption);
    fPrintF(file," interfaceEquationsOption=%i : 1=use equations for 3D order 4, 0=use extrap for 2nd ghost (old)\n",
            interfaceEquationsOption);
    fPrintF(file," setDivergenceAtInterfaces = %i (0: set [div(E)]=0,  1: setdiv(E)=0)\n",setDivergenceAtInterfaces);
    fPrintF(file," useImpedanceInterfaceProjection = %i\n",useImpedanceInterfaceProjection);

    fPrintF(file," number of interface interations=%i, omega=%5.2f, use new interface routines=%i \n",numberOfIterationsForInterfaceBC,
	    omegaForInterfaceIteration,(int)useNewInterfaceRoutines);

    if( numberOfMaterialRegions>1 )
    {
      fPrintF(file," number of material regions = %i\n",numberOfMaterialRegions);
      const int maxNumberOfRegionsToPrint=10;
      for( int r=0; r<min(maxNumberOfRegionsToPrint,numberOfMaterialRegions); r++ )
      {
	fPrintF(file,"  region %i : eps=%9.3e, mu=%9.3e, sigmaE=%9.3e, sigmaH=%9.3e\n",
		r,epsv(r),muv(r),sigmaEv(r),sigmaHv(r));
      }
      if( numberOfMaterialRegions>maxNumberOfRegionsToPrint )
      {
	fPrintF(file," ... there are more regions but I will not print anymore\n");
      }
      
    }
    else if( fabs(max(epsGrid)-min(epsGrid))==0. && fabs(max(muGrid)-min(muGrid))==0. )
    {
      fPrintF(file," eps=%9.3e, mu=%9.3e\n",eps,mu);
    }
    else
    {  // variableCoefficients
      fPrintF(file," Material parameters:\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
        fPrintF(file,"  Grid %i : eps=%9.3e mu=%9.3e (name=%s)\n",grid,epsGrid(grid),muGrid(grid),
                      (const char*)cg[grid].getName());
      }
    }

    fPrintF(file,"\n");
    if( method==sosup )
      fPrintF(file," sosup: orderOfExtrapolationForInterpolationNeighbours=%i (-1 means used orderOfAccuracy+1)\n",
	      dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours"));

    // -- output info about the projection Poisson solver 
    if( poisson )
    {
      OgesParameters & params =poisson->parameters;
      // If we have store initial paramerters separately then here they are: 
      OgesParameters & poissonParams =poisson->parameters; // pressureSolverParameters
      fPrintF(file,"\n Projection poisson solver: solver=%s, \n",(const char*)params.getSolverName());
      if(  poisson->isSolverIterative() )
      {
	real rtol,atol;
	int maximumNumberOfIterations;
	if( poisson->parameters.getSolverType()!=OgesParameters::multigrid )
	{
	  poissonParams.get(OgesParameters::THErelativeTolerance,rtol);
	  poissonParams.get(OgesParameters::THEabsoluteTolerance,atol);
	  poissonParams.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	}
	else
	{
	  OgmgParameters* ogmgPar = poisson->parameters.getOgmgParameters();
	  assert( ogmgPar!=NULL );
	  ogmgPar->get(OgmgParameters::THEresidualTolerance,rtol);  // note: residual
	  ogmgPar->get(OgmgParameters::THEabsoluteTolerance,atol);
	  ogmgPar->get(OgmgParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	}
	fPrintF(file,"                         : rel-tol=%8.2e, abs-tol=%8.2e, max iterations=%i (0=choose default)\n",
		rtol,atol,maximumNumberOfIterations);
	if( params.getSolverType()==OgesParameters::multigrid )
	{ // Here is the MG convergence criteria: 
          fPrintF(file,"                         : convergence: max-defect < (rel-tol)*L2NormRHS + abs-tol.\n");
	}
	
      }
    }
    else
    {
      fPrintF(file,"\n There is no poisson solver needed to project the fields.\n");
    }
    

    int maxNameLength=3;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      maxNameLength=max( maxNameLength,cg[grid].getName().length());

    fPrintF(file,"\n");
    fPrintF(file," Grid: %s \n",(const char*)nameOfGridFile);
    aString blanks="                                                                           ";
    fPrintF(file,"               Grid Data\n"
	    "               ---------\n"
	    "grid     name%s  gridIndexRange(0:1,0:2)           gridPoints        hmx      hmn  \n",
	    (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
    char buff[180];
    sPrintF(buff,"%%4i: %%%is   ([%%2i:%%5i],[%%2i:%%5i],[%%2i:%%5i])  %%12g   %%8.2e %%8.2e \n",maxNameLength);
    real maxMax=0.,maxMin=0.,minMin=REAL_MAX;
    numberOfGridPoints=0.; // this is a global value 
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
      real & hMin = dxMinMax(grid,0);
      real & hMax = dxMinMax(grid,1);
      maxMax=max(maxMax,hMax);
      maxMin=max(maxMin,hMin);
      minMin=min(minMin,hMin);
    
      real numGridPoints = c.mask().elementCount();
    
      // fPrintF(file,"%4i: %20s ([%2i:%5i],[%2i:%5i],[%2i:%5i])  %8i   %8.2e %8.2e \n",
      fPrintF(file,buff,grid, (const char *)cg[grid].getName(),
	      c.gridIndexRange(Start,axis1),c.gridIndexRange(End,axis1),
	      c.gridIndexRange(Start,axis2),c.gridIndexRange(End,axis2),
	      c.gridIndexRange(Start,axis3),c.gridIndexRange(End,axis3),
	      numGridPoints,hMax,hMin);
      numberOfGridPoints+=numGridPoints;
    }
    fPrintF(file," total number of grid points =%g, min(hmn)=%6.2e, max(hmn)=%6.2e, max(hmx)=%6.2e,  \n\n",
	    numberOfGridPoints,minMin,maxMin,maxMax);

    displayBoundaryConditions(file);

    fPrintF(file,"******************************************************************\n\n");
    
  }

}
