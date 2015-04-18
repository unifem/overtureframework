#include "HyperbolicMapping.h"
#include "DataPointMapping.h"
#include "TridiagonalSolver.h"
#include "display.h"
#include "arrayGetIndex.h"
#include "MatchingCurve.h"

int numberOfPossibleMultigridLevels( const IntegerArray & gridIndexRange );

//! return the uniform dissipation coefficient as a function of the step number
/*!
     The uniform dissipation coefficient can be ramped from a small value near the
  boundary to a larger value away from the boundary. This allows the grid to be more
  orthogonal to the boundary and also allows the grid lines to march out from a sharp
  convex corner in a nicer fashion.
 */
real HyperbolicMapping::
getDissipationCoefficient( int stepNumber )
{
  if( surfaceGrid )
  {
    return uniformDissipationCoefficient;
  }
  else
  {
    if( stepNumber < dissipationTransition )
    {
      real alpha=real(stepNumber)/real(dissipationTransition);
      return boundaryUniformDissipationCoefficient*(1.-alpha) + alpha*uniformDissipationCoefficient;
    }
    else
    {
      return uniformDissipationCoefficient;
    }
  }
  
}


// int HyperbolicMapping::
// generateNew(const int & numberOfAdditionalSteps /* = 0 */)
// {
//   return generate(numberOfAdditionalSteps);
// }


int HyperbolicMapping::
generate(const int & numberOfAdditionalSteps /* = 0 */)
//===========================================================================
/// \brief  
///     Generate the hyperbolic grid. 
/// \param Notes:
///     Without any smoothing the hyperbolic equations just advance in the normal
///   direction, a constant distance per step.
///     The distance marched is adjusted by smoothing the "volumes" and by smoothing
///   the grid.   
//===========================================================================
{
  const bool evalAsNurbsSave = evalAsNurbs;
  if( evalAsNurbs )
    useNurbsToEvaluate( false ); // turn off Nurbs eval during generation

  #ifndef USE_PPP
    return generateSerial(numberOfAdditionalSteps);
  #else
    return generateParallel(numberOfAdditionalSteps);
  #endif

  if( evalAsNurbsSave )
    useNurbsToEvaluate( evalAsNurbsSave ); // reset 

}


int HyperbolicMapping::
generateSerial(const int & numberOfAdditionalSteps /* = 0 */)
//===========================================================================
/// \brief  
///     Generate the hyperbolic grid. 
/// \param Notes:
///     Without any smoothing the hyperbolic equations just advance in the normal
///   direction, a constant distance per step.
///     The distance marched is adjusted by smoothing the "volumes" and by smoothing
///   the grid.   
//===========================================================================
{
  real time0=getCPU();

  // debug=1;  // **********
  if( debugFile==NULL && debug>0  )
    debugFile = fopen("hype.debug","w" );      // Here is the debug file, who closes this?
   
  if( debug & 1 )
  {
     fPrintF(debugFile,"\n >>>>>entering generate \n");
  }

  assert( checkFile!=NULL );

  assert( surface!=NULL );

  int returnCode=0;
  bool initialize= numberOfAdditionalSteps==0;
  plotNegativeCells=false;   // set to true below if there are negative cells detected

  int i3Start;
  int axis;

  // initialize the dpm, indexRange, dimension, etc. :
  initializeMarchingParameters( numberOfAdditionalSteps, i3Start);

  bool growBothDirections = fabs(growthOption) > 1;
  int numberOfDirectionsToGrow = growBothDirections ? 2 : 1;
  
  Range xAxes(0,rangeDimension-1);

  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);
    
  RealArray & x = xHyper;
  RealArray & xt = xtHyper;

  RealArray xr, normal, xrr, xrrDotN;
  RealArray s(D1,D2), ss(D1,D2);
  RealArray kappa(D1,D2); 

  // ***********************************************************************************
  // ********* allocate space for marching arrays, assign initial conditions: **********
  // ***********************************************************************************
  initializeMarchingArrays( i3Start,numberOfAdditionalSteps, x, xt,
			    xr, normal, xrr, s, ss, xrrDotN, kappa);
  
  Index I1,I2,I3;
  ::getIndex(indexRange,I1,I2,I3);
  Index Ig1,Ig2,Ig3;
  
  RealArray ds(I1,I2);
  ds=0.;
  // work arrays
  RealArray normXr, normXs;
  RealArray xte(I1,I2); 

  // Arrays for the block tridiagonal solver.
  TridiagonalSolver tri;
  at.redim(rangeDimension,rangeDimension,I1,I2);
  bt.redim(rangeDimension,rangeDimension,I1,I2);
  ct.redim(rangeDimension,rangeDimension,I1,I2);
  RealArray xTri(rangeDimension,I1,I2);

  
  // Here we define the `volume'
  // #define VOLUME(ds) (ds*sAve)/s(I1,I2)
#define VOLUME(ds) (ds(I1,I2)*ss(I1,I2)/s(I1,I2))

   
  // For surface grids we save the surface-normal (xr(.,.,.,.,1)) -- these is needed if we continue stepping.
  // Later we save xrSave in the xt array
  RealArray xrSave(D1,D2,2,rangeDimension);
  int direction;
  if( numberOfAdditionalSteps>0 && surfaceGrid )
  {
    // continuation run 'stepping' : recover xrSave
    for( direction=0; direction<numberOfDirectionsToGrow; direction++ )
      xrSave(D1,D2,direction,xAxes)=xt(D1,D2,direction,xAxes);
  }
  
  // #######################################################################
  // ######## Loop to march in the forward and/or reverse directions #######
  // #######################################################################
  for( direction=0; direction<numberOfDirectionsToGrow; direction++ )
  {
    // Notes:
    //   marching forward we go from i3Start ... x.getBound
    //   marching backwards we go from i3Start, i3Start-1,... x.getBase

    // growthDirection = 0 if marching in the forward direction
    //                 = 1 if marching in the reverse direction
    int growthDirection = (growthOption==1 || growBothDirections) ? direction : 1;
    int marchingDirection=1-2*growthDirection;

    if( numberOfAdditionalSteps>0 && direction==1 )
    {
      i3Start=numberOfAdditionalSteps;
    }
    if( numberOfAdditionalSteps<0 )
      continue;
    
    int i3Begin=0;  // This is where the grid in this direction starts.
    if( growBothDirections && growthDirection==0 )
    {
      i3Begin= linesToMarch[1];  
    }
    if( debug & 1 ) 
      printF(" ===== i3Begin=%i  (i3Start=%i,numberOfAdditionalSteps=%i) =====\n",
             i3Begin,i3Start,numberOfAdditionalSteps);

    real dSign = 1.-2*growthDirection;
    const int i3End= direction==0 ? I3.getBound() : I3.getBase();
    const int i3Increment =1-2*direction;
    
    // x0,x1 : hold the solution values for sub-stepping.
    RealArray x0(D1,D2,1,xAxes);
    RealArray x1(D1,D2,1,xAxes);  
    x1=0.;  // initialize to avoid UMR *wdh* 030219

    if( numberOfAdditionalSteps==0  )
    {
      bool initialStep=true;
      int option=1; // only compute projection onto the boundaries (and tangents)
      x0=x(D1,D2,i3Start,xAxes);
      x1=x0;
      if( applyBoundaryConditionsToStartCurve )
      {
        if( debug & 3 )
  	  printF("hype:generate:apply BC's to the start curve\n");
        returnCode=applyBoundaryConditions( x1,x0,marchingDirection,normal,xr,initialStep,i3Start );
        x(D1,D2,i3Start,xAxes)=x1;
      }
      else
      {
	// We must always apply some BC's to get the tangents defined at boundaries that match to other Mappings.
	// These tangent vectors will be used later in getNormalAndSurfaceArea in order to adjust the
	// marching vector at boundaries.
	returnCode=applyBoundaryConditionMatchToMapping(x0, marchingDirection, normal, xr, initialStep, option);
      }
      
      if( returnCode!=0 ) return returnCode;
    }
    
    if( surfaceGrid )
    {
      // ******************************************************************************
      // **** project start curve onto surface, choose appropriate side of corners ****
      // ******************************************************************************
      initializeSurfaceGrid( direction,numberOfAdditionalSteps,
                             i3Start,i3Begin,x,xt,xr,normal,ds,s,ss,xrr,normXr,normXs,xrSave );
    }



    if( debug & 2 )
      ::display(x(D1,D2,i3Start,xAxes),sPrintF("Here is x at (START) step %i",i3Start),debugFile);

    // ================================================================================
    // =======================   Marching Loop =========================================
    // ================================================================================

    if( debug & 1 )
      printF(" hype:generate: direction=%i i3Start=%i i3End=%i\n",direction,i3Start,i3End);
    

    real minCellVolume, maxCellVolume;
    int i3;
    for( i3=i3Start; i3!=i3End; i3+=i3Increment )
    {

      x0=x(D1,D2,i3,xAxes);
      const int i3p=i3+i3Increment;

      int numberOfSubSteps=1;

      int stepNumber=abs(i3-i3Start);
      
      real deltaT=1./numberOfSubSteps;
      for( int subStep=0; subStep<numberOfSubSteps; subStep++ )
      {
        int i3Mod2=(i3+subStep) % 2;


	// ------ compute the normal (i.e. marching vector) to the curve or surface -----
	bool firstStep=(i3+subStep)==0;
	int returnValue = getNormalAndSurfaceArea(x0,firstStep,normal,s,xr,xrr, dSign,normXr,normXs,ss,
                              marchingDirection,i3 );

	if( returnValue!=0 )
	{
	  printF("HyperbolicMapping::INFO: marching stopped at step=%i since the grid spacing became too small",i3);
	  break;
	}
    
	real time1=getCPU();
      
	getDistanceToStep(abs(i3-i3Begin),ds,growthDirection);          // get marching distance ds
	getCurvatureDependentSpeed(ds,kappa, xrr,normal,normXr,normXs); // adjust ds by the curvature

        // estimate the explicit time step parameter:
        real gamma = deltaT*max(ds(I1,I2)/normXr(I1,I2));

	if( (i3+subStep)==i3Start )
	{ // first time thru we need a value for xt
	  if( numberOfAdditionalSteps==0 )
	  {
	    // use this guess:
	    if( !surfaceGrid )
	    {
              if( debug & 4 )
	      {
		fPrintF(debugFile,"*** set xt at initial step to volume*normal**** \n");
		if( surface!=NULL )
		  fPrintF(debugFile,"*** surface.getSignForJacobian=%e\n",surface->getSignForJacobian());
	      }
	      
	      for( axis=0; axis<rangeDimension; axis++ )
		xt(I1,I2,i3Mod2,axis)=VOLUME(ds)*normal(I1,I2,0,axis);   // old value for xt
	    }
	  }
	  else
	  {
	    // continuation run, just difference to get xt:
	    xt(I1,I2,i3Mod2,xAxes)=x(I1,I2,i3,xAxes)-x(I1,I2,i3-i3Increment,xAxes);
	  }
	}

	RealArray normXt;
	if(rangeDimension==2)
	  normXt=SQRT(SQR(xt(I1,I2,i3Mod2,0))+SQR(xt(I1,I2,i3Mod2,1)));
	else
	  normXt=SQRT(SQR(xt(I1,I2,i3Mod2,0))+SQR(xt(I1,I2,i3Mod2,1))+SQR(xt(I1,I2,i3Mod2,2)));

        const real dissipationCoefficient=getDissipationCoefficient(stepNumber);
	
	// implicit time stepping
	// form the RHS
	for( axis=0; axis<rangeDimension; axis++ )
	{
	  xte=VOLUME(ds)*normal(I1,I2,0,axis)+dissipationCoefficient*xrr(I1,I2,0,axis,0);
	  if( domainDimension==3 )
	    xte+=dissipationCoefficient*xrr(I1,I2,0,axis,1);

	  if( removeNormalSmoothing )
	  { 
            // this didn't seem to work very well.
            removeNormalComponentOfSmoothing(axis,I1,I2,I3, xrr, xrrDotN, normal, xte );
	  }
	
	  xte.reshape(1,I1,I2);
	  xTri(axis,I1,I2)=xte(0,I1,I2)*deltaT;
	  xte.reshape(I1,I2);
	  if( debug & 4 )
	  {
	    // ::display(ds,sPrintF("Here is ds for xte for axis=%i at step %i",axis,i3p),debugFile);
	    // ::display(ss,sPrintF("Here is ss for xte for axis=%i at step %i",axis,i3p),debugFile);
	    // ::display(s,sPrintF("Here is s for xte for axis=%i at step %i",axis,i3p),debugFile);
	    ::display(normal(I1,I2,0,axis),sPrintF("Here is the normal for xte for axis=%i at step %i",axis,i3p),debugFile);
	    ::display(xrr(I1,I2,0,axis,0),sPrintF("Here is xrr for the xte for axis=%i at step %i",axis,i3p),debugFile);
	    ::display(xte,sPrintF("Here is the RHS xte for axis=%i at step %i",axis,i3p),debugFile);
	  }
	} 
	timing[timeForSetupRHS]+=getCPU()-time1;


	implicitSolve(xTri, i3Mod2,xr,xt,normal,normXr,normXs,normXt,tri,stepNumber);

	x0.reshape(1,D1,D2,1,xAxes); x1.reshape(1,D1,D2,1,xAxes);

	for( axis=0; axis<rangeDimension; axis++ )
	  x1(0,I1,I2,0,axis)=x0(0,I1,I2,0,axis)+xTri(axis,I1,I2);  // *** update the solution ****

	x0.reshape(D1,D2,1,xAxes); x1.reshape(D1,D2,1,xAxes);
  
	time1=getCPU();
      
	if( debug & 2 )
	  ::display(x1,sPrintF("***Here is x at BEFORE BC at step %i",i3p),debugFile,"%10.3e ");


	// boundary conditions (and projection onto a surface grid)
	returnCode=applyBoundaryConditions( x1,x0,marchingDirection,normal,xr,false,i3p );
        if( returnCode!=0 )
	{
	  break;
	}

	i3Mod2=(i3Mod2+1) % 2;
	xt(I1,I2,i3Mod2,xAxes)=x1(I1,I2,0,xAxes)-x0(I1,I2,0,xAxes);
    
	computeCellVolumes(xt,i3Mod2,minCellVolume,maxCellVolume,dSign );


	if( debug & 2 )
	  ::display(x1,sPrintF("\n *** Here is x at step %i",i3p),debugFile,"%10.3e ");

	if( info & 1 ) 
	{
          printF("...done step %4i (substep %i) min(dn)=%6.1e, max(dn)=%6.1e, min(vol)=%6.1e, max(vol)=%6.1e, "
               "cfl=%8.1e \n",i3p,subStep,min(fabs(ds)),max(fabs(ds)),minCellVolume,maxCellVolume,gamma);

          // checkFile format: (we pretend we are saving errors but we just save some data that can be used comparison)
          //   t numberOfComponents  c0 err uMax  c1 err1 uMax1 ...
          int numberOfComponentsToOutput=3;
          fPrintF(checkFile,"%9.2e %i  ",real(i3p),numberOfComponentsToOutput);
          int n=0;
	  fPrintF(checkFile,"%i %9.2e %10.3e  ",n,min(fabs(ds)),max(fabs(ds))); n++;
	  fPrintF(checkFile,"%i %9.2e %10.3e  ",n,minCellVolume,maxCellVolume); n++;
	  fPrintF(checkFile,"%i %9.2e %10.3e  ",n,gamma,gamma);
          fPrintF(checkFile,"\n");
	}
	
      
	totalNumberOfSteps+=1;
	
        if( subStep<numberOfSubSteps-1 )
	{
	  x0=x1;
	}
        timing[timeForUpdate]+=getCPU()-time1;
	
      }  // end for subStep
      
      x(D1,D2,i3p,xAxes)=x1(D1,D2,0,xAxes);

      if( minCellVolume*maxCellVolume<0. )
      {
        plotNegativeCells=true;   // set to true in generate if there are negative cells detected

	printF("HyperbolicMapping:ERROR: A negative volume has been detected at step i3p=%i (|i3p-i3Start|=%i).\n ",
               i3p,abs(i3p-i3Start));
        // **** we should have a more careful check of the cell volume --- it may not be correct especially
        // for surface grids that start on corners since the original normalCC may not correspond to the
        // corrected marching direction ****

        if( stopOnNegativeCells )
	{
          if( i3==i3Start )
	  {
            i3=i3p;
	    printF("generate:INFO: I am stopping at the current line since this is the first step. \n"
		   "               Change parameters to fix this or turn off `stop on negative cells' to avoid this.\n");
	  }
	  else
	  {
	    printF("generate:INFO: I am stopping at the previous line. \n"
		   "               Change parameters to fix this or turn off `stop on negative cells' to avoid this.\n");
	    i3=i3p-i3Increment;
	  }
          returnCode=1;
	  
	  break;
	}
      }
      else if( returnCode!=0 )
      {
	printF("\n"
               "generate:ERROR:There was an error from applyBoundaryConditions at "
               " step i3p=%i (|i3p-i3Start|=%i).\n",i3p,abs(i3p-i3Start));
        if( surfaceGrid )
	{
          printF("        :It could be that there are ghost points that cannot be projected onto the surface\n"
                 "        :You may want to turn off `project ghost [left/right]' for the appropriate side\n");
	}
	if( i3==i3Start )
	{
	  i3=i3p;
	}
	else
	{
	  i3=i3p-i3Increment;
	}
	break;
      }
      
    }  // end for i3
    
    if( i3!=i3End ) 
    {
      I3=direction==0 ? Range(I3.getBase(),i3) : Range(i3,I3.getBound());
      setGridDimensions(domainDimension-1,I3.getLength());
      int n=getGridDimensions(domainDimension-1)-1;
      dimension(End,axis3)=n;
      indexRange(End,axis3)=n;
      gridIndexRange(End,axis3)=n;
    }
    if( surfaceGrid )
    {
      // Save xr in case we take more steps.
      // normalCC(D1,D2,0,xAxes)=xr(D1,D2,0,xAxes,1);
      xrSave(D1,D2,direction,xAxes)=xr(D1,D2,0,xAxes,1);
    }
    

  } // end direction
  
  ::getIndex(dimension,I1,I2,I3);
  if( surfaceGrid )
  {
    // *************************************** may not be needed always ********************************

    // project ghost points on lines at the start and end of the marching direction
    // no need to redo the start if already done.
    for( int side=Start; side<=End; side++ )
    {
      if( (numberOfAdditionalSteps==0 || (abs(growthOption)==1 && side==End ) || abs(growthOption)==2 ) && 
             boundaryCondition(side,domainDimension-1)==0 )
      {
	const int is=2*side-1;
	int i3 = gridIndexRange(side,axis3);
	x(I1,I2,i3+is,xAxes)=2.*x(I1,I2,i3,xAxes)-x(I1,I2,i3-is,xAxes);

	if( debug & 2 )
	  printF("Projecting ghost lines on side=%i of the marching direction since bc=%i\n",side,
                 boundaryCondition(side,domainDimension-1));
	// *** only project if bc==0 ??
	// first extrap ghost line
	// now project the ghost line
	int marchingDirection=is;
	project(x(D1,D2,i3+is,xAxes),marchingDirection,xr);
      }
    }

    // save xr in the xt array.
    for( direction=0; direction<numberOfDirectionsToGrow; direction++ )
      xt(D1,D2,direction,xAxes)=xrSave(D1,D2,direction,xAxes);

  }
  

  IntegerArray gid;
  gid=gridIndexRange;
  if( debug & 2 ) 
  {
    printF("indexRange    =[%i,%i]x[%i,%i]x[%i,%i]\n",indexRange(0,0),
       indexRange(1,0),indexRange(0,1),indexRange(1,1),indexRange(0,2),indexRange(1,2));
    printF("gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i]\n",gridIndexRange(0,0),
       gridIndexRange(1,0),gridIndexRange(0,1),gridIndexRange(1,1),gridIndexRange(0,2),gridIndexRange(1,2));
    printF("dimension     =[%i,%i]x[%i,%i]x[%i,%i]\n",dimension(0,0),
       dimension(1,0),dimension(0,1),dimension(1,1),dimension(0,2),dimension(1,2));

    printF(" I1=[%i,%i] I2=[%i,%i] I3=[%i,%i]\n",I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
    printF(" D1=[%i,%i] D2=[%i,%i] D3=[%i,%i]\n",D1.getBase(),D1.getBound(),D2.getBase(),D2.getBound(),D3.getBase(),D3.getBound());
  
    printF("boundaryOffset = %i %i %i %i\n",boundaryOffset[0][0],boundaryOffset[1][0],
	   boundaryOffset[0][1],boundaryOffset[1][1]);
  
    printF("     bc=%i %i %i %i \n",getBoundaryCondition(0,0),getBoundaryCondition(1,0),
	   getBoundaryCondition(0,1),getBoundaryCondition(1,1));
    printF(" dpm:bc=%i %i %i %i \n",dpm->getBoundaryCondition(0,0),dpm->getBoundaryCondition(1,0),
	   dpm->getBoundaryCondition(0,1),dpm->getBoundaryCondition(1,1));
  }
  

  if( domainDimension==2 )
  {
    gid(Range(0,1),axis2)=gridIndexRange(Range(0,1),axis3);
    gid(Range(0,1),axis3)=0;
    x.reshape(x.dimension(0),x.dimension(2),1,xAxes);
  }

  // adjust for the boundary offset
  for( axis=0; axis<domainDimension; axis++ )
  {
    if( ! (bool)getIsPeriodic(axis) )
    {
      gid(Start,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(Start,axis)+boundaryOffset[Start][axis]));
      gid(End  ,axis)=max(x.getBase(axis),min(x.getBound(axis),gid(End  ,axis)-boundaryOffset[End  ][axis]));
    }
  }

/* ----
  if( domainDimension==2 && rangeDimension==3 )
  {
    // For surface grids use last line on interpolation boundaries as the ghost point
    int dir;
    if( ghostLineOption==useLastLineAsGhostLine )
    {
      for( dir=0; dir<domainDimension; dir++ )
      {
	if( getBoundaryCondition(0,dir)==0 )
	  gid(0,dir)+=1;
	if( getBoundaryCondition(1,dir)==0 )
	  gid(1,dir)-=1;
      }
    }
  }
---- */

  for( axis=0; axis<domainDimension; axis++ )
    setGridDimensions(axis,gid(End,axis)-gid(Start,axis)+1);   // is this right?

  boundaryOffsetWasApplied=true;  // this means the grid dimensions were adjusted by the boundaryOffset
  

#ifndef USE_PPP
  // ---- SERIAL VERSION ---
  if( evalAsNurbs )
  {
    // *new* way 2014/08/15 

    // *** CHECK THIS -- building3 example

    // --- NOTE -- ghost points may be different if setDataPoints evals NurbsMapping without
    //   computing addition ghost 

    IntegerArray dim(2,3);
    dim=0;
    dim(0,0)=I1.getBase();
    dim(1,0)=I1.getBound();
    if( domainDimension==2 )
    {
      dim(0,1)=I3.getBase();
      dim(1,1)=I3.getBound();
    }
    else
    {
      dim(0,1)=I2.getBase();
      dim(1,1)=I2.getBound();
      dim(0,2)=I3.getBase();
      dim(1,2)=I3.getBound();
    }
    
    // ::display(gid,"gid");
    // ::display(dim,"dim");
    
    dpm->setDataPoints(x,domainDimension,rangeDimension,dim,gid);

  }
  else
  {
    if( domainDimension==2 )
    {
      dpm->setDataPoints(x(I1,I3,0,xAxes),3,domainDimension,0,gid);
    }
    else
      dpm->setDataPoints(x(I1,I2,I3,xAxes),3,domainDimension,0,gid);
  }
  

  setBasicInverseOption(dpm->getBasicInverseOption());
  reinitialize();  // *wdh* 000503
      
  mappingHasChanged();
  
#endif

  if( domainDimension==2 )
    x.reshape(x.dimension(0),1,x.dimension(1),xAxes);

  if( info & 1 )
  {
    printF("\n time to generate the hyperbolic grid = %e\n",getCPU()-time0);
    printF("After adjustment for boundary offset: gridIndexRange=[%i,%i]x[%i,%i]x[%i,%i]"
           " Possible multigrid levels=%i\n",gid(0,0),
	   gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
           numberOfPossibleMultigridLevels(gid));
  }
  
  if( debug & 1 ) fPrintF(debugFile,"\n <<<<<leaving generate \n");

  timing[totalTime]+=getCPU()-time0;
  
  return returnCode;
}

int HyperbolicMapping::
initializeMarchingParameters(int numberOfAdditionalSteps, int & i3Start )
// ============================================================================================
/// \param Access: protected.
/// \details 
///     Initialize the parameters used by generate.
// ============================================================================================
{
  int axis;
  if( dpm==NULL || numberOfAdditionalSteps==0 )
  {
    if( dpm==NULL )
    {
      dpm=new DataPointMapping;
      dpm->incrementReferenceCount();
    }
    dpm->setName(mappingName,aString("hyperbolic-")+surface->getName(mappingName));
    dpm->setDomainDimension(domainDimension);
    dpm->setRangeDimension(rangeDimension);

    useNurbsToEvaluate(evalAsNurbs);

    for( axis=0; axis<domainDimension-1; axis++ )
    {
      dpm->setIsPeriodic(axis,getIsPeriodic(axis));
      for( int side=Start; side<=End; side++ )
      {
	dpm->setBoundaryCondition(side,axis,getBoundaryCondition(side,axis));
	dpm->setShare(side,axis,getShare(side,axis));
      }
    }

    
//      for( int growthDirection=0; growthDirection<=1; growthDirection++ )
//        geometricNormalization[growthDirection]=(geometricFactor-1.)/
//                        (pow(geometricFactor,linesToMarch[growthDirection]-1)-1.);

    // we now define the geometricNormalization to be the initial grid spacing
    for( int growthDirection=0; growthDirection<=1; growthDirection++ )
    {
      if( initialSpacing<0 )
      {
        //      a  a*r  a*r^2 ...  a*r^{n-1}    n=steps = numLines-1
        //   distance = SUM_{i=0,n} a*r^i  = a*(1-r^n)/(1-r)   , n=lines-1
        //   initial spacing:  ds = a
	geometricNormalization[growthDirection]=distance[growthDirection]*
	  (geometricFactor-1.)/(pow(geometricFactor,linesToMarch[growthDirection]-1.)-1.);

        if( debug & 2 )
	  printf(" ++++ initial spacing=%8.2e (from distance=%8.2e lines=%i, gamma=%8.2e)\n",
		 geometricNormalization[growthDirection],distance[growthDirection],
		 linesToMarch[growthDirection],geometricFactor);

      }
      else
      {
	geometricNormalization[growthDirection]= initialSpacing;
      }
    }
  }
  else
  {
    assert( dpm!=NULL );
  }
  
  bool growBothDirections = fabs(growthOption) > 1;
  int numberOfDirectionsToGrow = growBothDirections ? 2 : 1;
  
  // int i3Start;
  if( numberOfAdditionalSteps==0 )
  {
    // *wdh* 110806 i3Start=growBothDirections ? linesToMarch[1]-1 : 0;
    i3Start=growBothDirections ? linesToMarch[1] : 0;
  }
  else
  {
    if( growthOption==1 || growBothDirections )
    {
      adjustDistanceToMarch(numberOfAdditionalSteps,0);
      linesToMarch[0]+=numberOfAdditionalSteps;
    }
    if( growthOption==-1 || growBothDirections )
    {
      adjustDistanceToMarch(numberOfAdditionalSteps,1);
      linesToMarch[1]+=numberOfAdditionalSteps;
    }

    // Here is where we start marching. forward, reverse or both directions.
    i3Start=growthOption==+1 ? linesToMarch[0]-1-numberOfAdditionalSteps :
            growthOption==-1 ? linesToMarch[1]-1-numberOfAdditionalSteps : 
                               linesToMarch[1]-1 + linesToMarch[0]-1-numberOfAdditionalSteps;  
  }


  // *wdh* 110806 -- we were missing a line in the forward direction when marching in both directions
  // int gridLines = growBothDirections ? linesToMarch[0] + linesToMarch[1]-1 : 
  //                 growthOption==1 ? linesToMarch[0] : linesToMarch[1];
  int gridLines = growBothDirections ? linesToMarch[0] + linesToMarch[1] : 
                   growthOption==1 ? linesToMarch[0] : linesToMarch[1];
  setGridDimensions(domainDimension-1,gridLines);
  

  dimension.redim(2,3);
  indexRange.redim(2,3);
  gridIndexRange.redim(2,3);

  IntegerArray numberOfGhostPoints(2,3);
  numberOfGhostPoints=1;
  
  indexRange=0;
  gridIndexRange=0;
  dimension=0;
  int n;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    n=getGridDimensions(axis)-1;
    if( boundaryOffsetWasApplied && ! (bool)getIsPeriodic(axis) )
      n+=boundaryOffset[End][axis]+boundaryOffset[Start][axis];

    // include ghost points, boundaryOffset
    dimension(Start,axis)=0 -numberOfGhostPoints(Start,axis);
    dimension(End,axis)  =n +numberOfGhostPoints(End  ,axis); //+boundaryOffset[End][axis]+boundaryOffset[Start][axis];

    indexRange(Start,axis)=0;
    indexRange(End  ,axis)=n; // + boundaryOffset[End  ][axis] +boundaryOffset[Start][axis];
    if( (bool)getIsPeriodic(axis) )
      indexRange(End,axis)-=1;

    gridIndexRange(Start,axis)=0;
    gridIndexRange(End  ,axis)=n; // + boundaryOffset[End  ][axis] +boundaryOffset[Start][axis];
  }

  // always put the marching in position i3
  const int dir=domainDimension-1;
  n=getGridDimensions(dir)-1;  // no need to adjust this value since it was set correctly above

  dimension(Start,axis3)=0;
//   if( surfaceGrid && boundaryCondition(Start,dir)==0 )
//     dimension(Start,axis3)-=1;

  dimension(End,axis3)=n+numberOfGhostPoints(End,dir);  // +boundaryOffset[End][dir]+boundaryOffset[Start][dir];
//   if( surfaceGrid && boundaryCondition(End,dir)==0 )
//     dimension(End,axis3)+=1;

  indexRange(Start,axis3)=0;
  indexRange(End  ,axis3)=n+numberOfGhostPoints(End,dir);     // boundaryOffset[End][dir]+boundaryOffset[Start][dir];
  gridIndexRange(Start,axis3)=0;
  gridIndexRange(End  ,axis3)=n+1; // boundaryOffset[End][dir]+boundaryOffset[Start][dir];
  
  return 0;
}


int HyperbolicMapping::
evaluateStartCurve( RealArray & xStart )
// =================================================================================
///  Evaluate points on the start curve. Adjust for stretching etc.
/// 
///  
// ================================================================================
{
  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);

  RealArray r(D1,D2,1,domainDimension-1);
  // ***************************************************
  // ** evaluate the surface : including ghost points **
  // ***************************************************

  if( surfaceGrid )
  {
    // first compute r:
    real h1=1./max(1.,(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1)));
    r(D1,0,0,0).seqAdd(dimension(Start,axis1)*h1,h1);
    if( debug & 1 ) printf("evaluateStartCurve: evaluate the start curve\n");

    startCurve->mapGridS(r,xStart);
  }
  else
  {
    // first compute r:
    real h1=1./max(1.,(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1)));
    for( int i2=D2.getBase(); i2<=D2.getBound(); i2++ )
      r(D1,i2,0,0).seqAdd(dimension(Start,axis1)*h1,h1);
    if( domainDimension>2 )
    {
      real h2=1./max(1.,(gridIndexRange(End,axis2)-gridIndexRange(Start,axis2)));
      for( int i1=D1.getBase(); i1<=D1.getBound(); i1++ )
	r(i1,D2,0,axis2).seqAdd(dimension(Start,axis2)*h2,h2);
    }
    if( debug & 1 ) printf("evaluateStartCurve: evaluate points on initial curve or surface\n");

    surface->mapGridS(r,xSurface);
  }



  if( debug & 4 )
    ::display(xStart,"start curve after initial evaluation",debugFile,"%8.2e ");

  if( equidistributionWeight>0. || startCurveStretchMapping!=NULL )
  {
    // redistribute points on the initial curve based on equidistriubution 
    printf(" *** equidistribute points on the initial curve ****\n");
    // We need to choose a different weight than equidistributionWeight since we
    // want to achieve the equilibrium state
    const real weight=1.; // curvatureWeight/(arcLengthWeight+curvatureWeight);

//        real arcWeightSave=arclengthWeight;
//        real curvatureSave=curvatureWeight;
//        real factor=
//        arclengthWeight*=factor;
//        curvatureWeight=

    int marchingDirection=0;
    equidistributeAndStretch(0,xStart,weight,marchingDirection);

//        arclengthWeight=arcWeightSave;
//        curvatureWeight=curvatureSave;
      
//        printf(" After equidistribute points: weight=%8.2e max(diff)=%8.2e\n",weight,
//            max(fabs(x(D1,D2,i3Start,xAxes)-xSurface(D1,D2,0,xAxes))));
      
  }

  // adjust points for interior matching curves
  const int numberOfMatchingCurves=matchingCurves.size();
  if( numberOfMatchingCurves>0 )
  {
    int *matchLine = new int[numberOfMatchingCurves];
    real *matchShift = new real [numberOfMatchingCurves];
    
    for( int i=0; i<numberOfMatchingCurves; i++ )
    {
      MatchingCurve & match = matchingCurves[i];
      int & gridLine = match.gridLine;
      gridLine=int( match.curvePosition*(numberOfPointsOnStartCurve-1) +.5); // initial guess
      
#define DIST(i) (SQR(xStart(i,0,0,0)-match.x[0])+ \
                 SQR(xStart(i,0,0,1)-match.x[1])+ \
                 SQR(xStart(i,0,0,2)-match.x[2]))

      // The grid may be stretch: find a new closest grid point
      int iMin=gridLine;
      real distMin=DIST(gridLine);
      while( gridLine>0 && DIST(gridLine-1)<distMin )
      {
	gridLine--;
	distMin=DIST(gridLine);
      }
      while( gridLine<(numberOfPointsOnStartCurve-1) && DIST(gridLine+1)<distMin )
      {
	gridLine++;
	distMin=DIST(gridLine);
      }
#undef DIST      

      printf("***startCurve:adjust  pt %i (%8.2e,%8.2e,%8.2e) -> (%8.2e,%8.2e,%8.2e) for interior matching curve %i\n",
	     gridLine,xStart(gridLine,0,0,0),xStart(gridLine,0,0,1),(rangeDimension==2? 0. :
             xStart(gridLine,0,0,2)),match.x[0],match.x[1],match.x[2], i);


      matchLine[i]=gridLine;
      matchShift[i]=0.;  // fraction of a grid spacing that the point moved

      xStart(gridLine,0,0,0)=match.x[0];
      xStart(gridLine,0,0,1)=match.x[1];
      xStart(gridLine,0,0,2)=match.x[2];
      
      // The actual grid line should be offset by the boundaryOffset (so it is correct when used with the GridSmoother)
      gridLine -= boundaryOffset[0][0];  // ********** note **********
      
    }

    delete [] matchLine;
    delete [] matchShift;
  }
  

  return 0;
}



int HyperbolicMapping::
initializeMarchingArrays( int i3Start, int numberOfAdditionalSteps, RealArray & x, RealArray & xt,
                           RealArray & xr, RealArray & normal, RealArray & xrr,
                           RealArray & s, RealArray & ss, RealArray & xrrDotN,
                            RealArray & kappa )
// ============================================================================================
/// \param Access: protected.
/// \details 
///     Dimension time stepping arrays and assign the initial front (the first grid line on the hyperbolic grid)
///   if this is the first step. 
// ============================================================================================
{
  if( debug & 1 ) fPrintF(debugFile,"\n >>>>>Entering initializeMarchingArrays \n");

  Range xAxes(0,rangeDimension-1);

  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);
  if( numberOfAdditionalSteps==0 )
  {
    // **** Fill in the first grid line on the hyperbolic grid ****
    //   Use the `surface' Mapping for volume grids or the `startCurve' for surface grids.

    x.redim(D1,D2,D3,xAxes);
    xt.redim(D1,D2,2,xAxes);  // fix for grow both directions
    xt=0.;


    if( evaluateTheSurface || 
        surfaceGrid || // *wdh* to debug
        xSurface.getLength(0)!=D1.getLength() || xSurface.getLength(1)!=D2.getLength() )
  
    {
      // ***************************************************
      // ** evaluate the surface : including ghost points **
      // ***************************************************
      evaluateTheSurface=FALSE;
      xSurface.redim(D1,D2,1,xAxes);
      xSurface=0.;  

      evaluateStartCurve( xSurface );

//        if( surfaceGrid )
//        {
//        }
//        else
//        {
//           // first compute r:
//  	RealArray r(D1,D2,1,domainDimension-1);
//  	real h1=1./max(1.,(gridIndexRange(End,axis1)-gridIndexRange(Start,axis1)));
//  	if( domainDimension==2 )
//  	  r(D1,0,0,0).seqAdd(dimension(Start,axis1)*h1,h1);
//  	else
//  	{
//  	  for( int i2=D2.getBase(); i2<=D2.getBound(); i2++ )
//  	    r(D1,i2,0,0).seqAdd(dimension(Start,axis1)*h1,h1);
//  	  real h2=1./max(1.,(gridIndexRange(End,axis2)-gridIndexRange(Start,axis2)));
//  	  for( int i1=D1.getBase(); i1<=D1.getBound(); i1++ )
//  	    r(i1,D2,0,axis2).seqAdd(dimension(Start,axis2)*h2,h2);
//  	}

//  	if( debug & 1 ) printf("initializeMarchingArrays: evaluate points on initial curve or surface\n");
//  	surface->mapGrid(r,xSurface);
//        }

    }
  

    x(D1,D2,i3Start,xAxes)=xSurface(D1,D2,0,xAxes);

//      if( equidistributionWeight>0. )
//      {
//        // redistribute points on the initial curve based on equidistriubution 
//        printf(" *** equidistribute points on the initial curve ****\n");
//        // We need to choose a different weight than equidistributionWeight since we
//        // want to achieve the equilibrium state
//        const real weight=1.; // curvatureWeight/(arcLengthWeight+curvatureWeight);

//  //        real arcWeightSave=arclengthWeight;
//  //        real curvatureSave=curvatureWeight;
//  //        real factor=
//  //        arclengthWeight*=factor;
//  //        curvatureWeight=

//        equidistribute(i3Start,x,weight);

//  //        arclengthWeight=arcWeightSave;
//  //        curvatureWeight=curvatureSave;
      
//        printf(" After equidistribute points: weight=%8.2e max(diff)=%8.2e\n",weight,
//            max(fabs(x(D1,D2,i3Start,xAxes)-xSurface(D1,D2,0,xAxes))));
      
//        xSurface(D1,D2,0,xAxes)=x(D1,D2,i3Start,xAxes);
      
//      }

    // these arrays are saved in the class 
    c.redim(D1,D2,2+(rangeDimension-2)*4);
    c=0.;
    normalCC.redim(D1,D2,1,xAxes);


  }
  else
  {
    // continuation run, increase the size
    if( abs(growthOption)==2 )
    {
      x.resize(D1,D2,D3-numberOfAdditionalSteps,xAxes); // add extra space to the start
      x.reshape(D1,D2,D3,xAxes);                        // reset lower bound to zero
    }
    else
      x.resize(D1,D2,D3,xAxes); 

  }

  int ddm1=domainDimension-1;


  if( !surfaceGrid )
    xr.redim(D1,D2,1,xAxes,ddm1);
  else
    xr.redim(D1,D2,1,xAxes,2);
  xr=0.;
  normal.redim(D1,D2,1,xAxes);
  normal=0.;
  xrr.redim(D1,D2,1,xAxes,ddm1);
  xrr=0.;
  s.redim(D1,D2);
  s=0.;
  ss.redim(D1,D2);
  ss=0.;
  // lambda holds the uniform dissipation and nonlinear-upwind-dissipation.
  lambda.redim(D1,D2);
  lambda=0.;
  
  if( removeNormalSmoothing )
  {
    xrrDotN.redim(D1,D2,1,ddm1);
  }
  if( curvatureSpeedCoefficient!=0.  )
  {
    kappa.redim(D1,D2); 
    kappa=0.;
  }


  if( debug & 1 ) fPrintF(debugFile,"\n <<<<<Leaving initializeMarchingArrays \n");

  return 0;
}


int HyperbolicMapping::
initializeSurfaceGrid(int direction,
                      int numberOfAdditionalSteps, int i3Start, int i3Begin,
                      RealArray & x, RealArray & xt,
                      RealArray & xr, 
                      RealArray & normal, RealArray & ds, RealArray & s, RealArray & ss, RealArray & xrr, 
                      RealArray & normXr, RealArray & normXs,
                      RealArray & xrSave )
// ============================================================================================
/// \param Access: protected.
/// \details 
///     Initialize the surface grid. If this is the first step then project the initial curve
///   onto the reference surface.
// ============================================================================================
{
  if( debug & 1 ) fPrintF(debugFile,"\n >>>>>entering initializeSurfaceGrid \n");
  
  bool growBothDirections = fabs(growthOption) > 1;
  int growthDirection = (growthOption==1 || growBothDirections) ? direction : 1;
  int marchingDirection=1-2*growthDirection;
  real dSign = 1.-2*growthDirection;
  const int i3Increment =1-2*direction;

  Index I1,I2,I3;
  ::getIndex(indexRange,I1,I2,I3);

  Index D1,D2,D3;
  ::getIndex(dimension,D1,D2,D3);

  Range xAxes(0,rangeDimension-1);
   
  int axis;
  if( numberOfAdditionalSteps==0 )
  {
    bool initialStep=true;

    // Apply the boundary conditions and project initial curve if a surface grid.
    // project the points onto the reference surface -- do here so we project ghost points too
    if( surfaceGrid )
    {
      printf("****** project initial curve onto the surface ****\n");

      // ** force a robust projection here (with initialStep==true) ***
      project( x(D1,D2,i3Start,xAxes),marchingDirection,xr,true,initialStep ); 
	
      // If we don't project ghost points we need initial values for the ghost points.
      // We cannot use applyBoundaryConditions since we don't have the normal computed yet.
      // We can't compute the normal without the ghost point values, thus just extrapolate
      // the ghost points.

      // ** should we check projectGhostPoints(Start,axis1) ??

      // extrapolate the ghost points
      const int extra=1;
      Index Ig1,Ig2,Ig3;
      int is[2];
      for( axis=0; axis<domainDimension-1; axis++ )
      {
	for( int side=Start; side<=End; side++ )
	{
          if( boundaryCondition(side,axis)==trailingEdge || getIsPeriodic(axis)==notPeriodic )  // *wdh* 011021
	  {
	    is[0]=is[1]=0;
	    getGhostIndex(gridIndexRange,side,axis,Ig1,Ig2,Ig3,1,extra);
	    is[axis]=1-2*side;
	    x(Ig1,Ig2,i3Start,xAxes)=2.*x(Ig1+is[0],Ig2+is[1],i3Start,xAxes)-x(Ig1+2*is[0],Ig2+2*is[1],i3Start,xAxes);
	  }
	  
	}
      }
	
    }
      
    if( Mapping::debug & 4 )
      ::display(x(D1,D2,i3Start,xAxes),sPrintF("Here is x at the start, after project"),debugFile,"%10.3e ");

    if( surfaceGrid )
    {
      // take a small step in the normal direction and project. If we are starting on 
      // a corner then we want to choose the correct normal
      printf("**** Take an initial small step in the normal direction *****\n");
      int i3=i3Start;
      int i3p=i3+i3Increment;
      bool firstStep=i3==0;
      getNormalAndSurfaceArea(x(D1,D2,i3Start,xAxes),firstStep,normal,s,xr,xrr, dSign,normXr,normXs,ss,
			      marchingDirection,i3Start );
      getDistanceToStep(abs(i3-i3Begin),ds,growthDirection);                    // get marching distance ds

      const real delta=.25; // take this fraction of a step
      x(D1,D2,i3p,xAxes)=0.;
      for( axis=0; axis<rangeDimension; axis++ )
	x(I1,I2,i3p,axis)=x(I1,I2,i3,axis)+delta*VOLUME(ds)*normal(I1,I2,0,axis);

      // Now recompute the normal
      // *not needed? **turn off project onto reference surface to avoid problems when starting on corners. *wdh* 010504
        
      if( Mapping::debug & 4 )
	::display(x(D1,D2,i3p,xAxes),sPrintF("Here is x after small step"),debugFile,"%10.3e ");

      if( false )
      {
	bool projectOntoReferenceSurfaceSave=projectOntoReferenceSurface;
	projectOntoReferenceSurface=false;
	// force a robust projection here with initialStep==true
	applyBoundaryConditions( x(D1,D2,i3p,xAxes),x(D1,D2,i3p,xAxes),marchingDirection,normal,xr,initialStep,i3p );
	projectOntoReferenceSurface=projectOntoReferenceSurfaceSave;
      }
	
      // compute the initial guess for xt:
      int i3Mod2=i3 % 2;
      xt(I1,I2,i3Mod2,xAxes)=(x(I1,I2,i3p,xAxes)-x(I1,I2,i3,xAxes))*(1./delta);
    }
      
  }
  else if( surfaceGrid ) 
  {
    // for a surface grid we need to compute xr(I1,I2,I3,xAxes,1)
    // ** applyBoundaryConditions( x(D1,D2,i3Start,xAxes),x(D1,D2,i3Start,xAxes),marchingDirection,normal,xr,false,i3Start );

    // we have saved xr in the normalCC array
    // xr(D1,D2,0,xAxes,1)=normalCC(D1,D2,0,xAxes);
    // we have saved xr in the xt array
    xr(D1,D2,0,xAxes,1)=xrSave(D1,D2,direction,xAxes);
  }
  
  if( debug & 1 ) fPrintF(debugFile,"\n <<<<<leaving initializeSurfaceGrid \n");
  return 0;
}


int HyperbolicMapping::
removeNormalComponentOfSmoothing(int axis, const Index & I1, const Index & I2, const Index & I3,
                                 RealArray & xrr, RealArray & xrrDotN, 
                                 RealArray & normal, RealArray & xte )
// ==================================================================================================
// /Description:
//   Remove normal component of the smoothing term -- this was supposed to prevent
// the grid from marching in the wrong direction -- *** but doesn't seem to work very well. ****
// ==================================================================================================
{
	    
  if( axis==0 )
  {
    for( int dir=0; dir<domainDimension-1; dir++ )
    {
      xrrDotN(I1,I2,0,dir)=(xrr(I1,I2,0,axis1,dir)*normal(I1,I2,0,axis1)+
			    xrr(I1,I2,0,axis2,dir)*normal(I1,I2,0,axis2));
      if( rangeDimension==3 )
	xrrDotN(I1,I2,0,dir)+=xrr(I1,I2,0,axis3,dir)*normal(I1,I2,0,axis3); 
    }
    xrrDotN=min(0,xrrDotN);
  }


  // remove the normal component of the smoothing. Also remove the part coming from the
  //  implicit time stepping -- here we approximate xrr at the new time step using the old value.

  // ***uniformDissipationCoefficient is no longer correct here

  const real dissipationFactor = (1.+implicitCoefficient)*uniformDissipationCoefficient;
  if( domainDimension==2 )
    xte-=dissipationFactor*xrrDotN(I1,I2,0,0)*normal(I1,I2,0,axis);
  else
    xte-=dissipationFactor*(xrrDotN(I1,I2,0,0)+xrrDotN(I1,I2,0,1))*normal(I1,I2,0,axis);
  

  return 0;
}
