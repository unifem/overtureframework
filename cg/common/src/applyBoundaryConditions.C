#include "DomainSolver.h"
#include "App.h"
#include "MappedGridOperators.h"
#include "ParallelUtility.h"
#include "StretchTransform.h"

#include "EquationDomain.h"
extern ListOfEquationDomains equationDomainList; // This is in the global name space for now.

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


#define FOR_3D_AND_OK(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound && ok; i3++) \
for(i2=I2Base; i2<=I2Bound && ok; i2++) \
for(i1=I1Base; i1<=I1Bound && ok; i1++)

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{applyBoundaryConditions}} 
int DomainSolver::
applyBoundaryConditions(GridFunction & cgf,
                        const int & option /* =-1 */,
                        int grid_ /* = -1 */,
                        GridFunction *puOld /* =NULL */, 
                        const real & dt /* =-1. */ )
//===============================================================================================
//  /Description:
//    Apply boundary conditions.
// /grid\_ (input) by default do all grids, otherwise just this grid.
//\end{CompositeGridSolverInclude.tex}  
//===============================================================================================
{
  real time0 = getCPU();
  
  if( parameters.isMovingGridProblem() && fabs(cgf.t-cgf.gridVelocityTime)> REAL_EPSILON*(1.+cgf.t) )
  {
    cout << "DomainSolver::applyBoundaryConditions:ERROR:cgf.t !=cgf.gridVelocityTime \n";
    printf(" cgf.t=%14.8e, cgf.gridVelocityTime=%14.8e, diff=%8.2e\n",
	   cgf.t,cgf.gridVelocityTime,cgf.t-cgf.gridVelocityTime);
    
    Overture::abort("error");
  }


  // If necessary, convert to primitive variables before applying the BC's
  const GridFunction::Forms form = cgf.form;
  if( form==GridFunction::conservativeVariables )
    cgf.conservativeToPrimitive(grid_);             // we could fixup unused points here

  if( puOld!=NULL && puOld->form==GridFunction::conservativeVariables )
    puOld->conservativeToPrimitive(grid_);             // we could fixup unused points here

  // this next call also checks the memory usage (if turned on)
  checkArrayIDs(" applyBoundaryConditions (before applyBC's)"); 

  Range G = grid_==-1 ? Range(0,cgf.cg.numberOfGrids()-1) : Range(grid_,grid_);

  // *wdh* July 3, 2016 -- we need to update any known solution here before is it used grid-by-grid
  const Parameters::KnownSolutionsEnum & knownSolution = 
    parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
  if( knownSolution!=Parameters::noKnownSolution )
  {
    parameters.updateKnownSolutionToMatchGrid(cgf.cg);
  }
   


  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
  {
    realMappedGridFunction & gridVelocity = cgf.getGridVelocity(grid);

    // determine time dependent conditions:
    getTimeDependentBoundaryConditions( cgf.cg[grid],cgf.t,grid ); 

    // Variable boundary values:
    setVariableBoundaryValues( cgf.t,cgf,grid );

    if( parameters.thereAreTimeDependentUserBoundaryConditions(nullIndex,nullIndex,grid)>0 )
    {
      // there are user defined boundary conditions
      userDefinedBoundaryValues( cgf.t,cgf,grid);
    }

    real timeBC=getCPU();
    if( puOld==NULL )
    {
      applyBoundaryConditions(cgf.t,cgf.u[grid],gridVelocity,grid,option);
    }
    else
    {
      applyBoundaryConditions(cgf.t,cgf.u[grid],cgf.getGridVelocity(grid),grid,option,
			      &((*puOld).u[grid]),&((*puOld).getGridVelocity(grid)),dt);
    }
    timeBC = getCPU()-timeBC;
    // The above call is timed since that function may be called separately (?) -- thus subtract off that time
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions")) -= timeBC;

  }
  // **wdh 060324 -- check for error return from BC operators ---
  bool errorFound=false;
  for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
  {
    MappedGridOperators & op = *cgf.u[grid].getOperators();
    if( op.errorStatus==GenericMappedGridOperators::errorInFindInterpolationNeighbours )
    {
      printf("DomainSolver:::applyBoundaryConditions:ERROR\n"
	     "  Error in findInterpolationNeighbours for grid=%i (t=%9.3e, step=%i).\n",grid,cgf.t,parameters.dbase.get<int >("globalStepNumber"));
      errorFound=true;
    }
  }
  if( errorFound )
  {
    aString gridFileName=sPrintF("errorGridStep%i.hdf",max(0,parameters.dbase.get<int >("globalStepNumber")));
    printf("Saving the grid that had the problem, named %s.\n",(const char*)gridFileName);
    cgf.cg.saveGridToAFile(gridFileName,"errorGrid");
    if( false )
    {
      Overture::abort("error");
    }
    else
    { // for now -- reset error status and continue ---
      printf("DomainSolver:::applyBoundaryConditions: ...continuing, answers may be incorrect...\n");
      for( int grid=G.getBase(); grid<=G.getBound(); grid++ )
      {
	MappedGridOperators & op = *cgf.u[grid].getOperators();
	op.errorStatus==GenericMappedGridOperators::noErrors;
      }
    }
  }

  // Assign some interface boundary conditions here: 
  assignInterfaceBoundaryConditions(cgf,option,grid_,puOld,dt);

  checkArrayIDs(" applyBoundaryConditions (after applyBC's)"); 

  if( form==GridFunction::conservativeVariables && 
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::forwardEuler &&
      parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::midPoint )
    cgf.primitiveToConservative(grid_);


  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-time0;
  return 0;
}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{getTimeDerivativeOfBoundaryValues}} 
int DomainSolver::
getTimeDerivativeOfBoundaryValues(GridFunction & gf0,
                                  const real & t, 
				  const int & grid,
				  int side /* =-1 */,
				  int axis /* =-1 */ )
//===============================================================================================
//  /Description:
//    Determine the time derivative of the boundary condition values. These are required for
// some boundary conditions such as the boundary condition for the pressure in INS.
//
// /t (input) : compute forcing at this time
// /grid (input) : compute forcing for this grid.
// /side,axis (input) : get values for this face (get all faces by default).
//
// /Return value: equals the number of boundaries assigned.
// \end{CompositeGridSolverInclude.tex}  
//===============================================================================================
{
  int numberOfBoundariesAssigned=0;

  MappedGrid & mg = gf0.cg[grid];
  numberOfBoundariesAssigned+=getTimeDependentBoundaryConditions( mg,t,grid,side,axis,computeTimeDerivativeOfForcing );

  // Here are "variable" boundary forcings:
  numberOfBoundariesAssigned+=
      setVariableBoundaryValues( t,gf0,grid,side,axis,computeTimeDerivativeOfForcing );


  // Here are user defined boundary values:
  if( parameters.thereAreTimeDependentUserBoundaryConditions(nullIndex,nullIndex,grid)>0 )
  {
    numberOfBoundariesAssigned+= 
      userDefinedBoundaryValues( t,gf0,grid,side,axis,computeTimeDerivativeOfForcing );
  }

  return numberOfBoundariesAssigned;
}


//\begin{>>MappedGridSolverInclude.tex}{\subsection{getTimeDependentBoundaryConditions}} 
int DomainSolver::
getTimeDependentBoundaryConditions( MappedGrid & mg,
                                    real t, 
                                    int grid /* = 0 */, 
                                    int side0 /* = -1 */,
                                    int axis0 /* = -1 */,
                                    ForcingTypeEnum forcingType /* =computeForcing */ )
//===============================================================================================
//  /Description:
//    Determine the forcing for time dependent boundary conditions. 
//   We also need to determine the time derivative of the forcing (acceleration) for the
// pressure BC for INS.
// /t (input) : compute forcing at this time
// /grid (input) : compute forcing for this grid.
// /side0,axis0 (input) : get values for this face (get all faces by default).
// /forcingType (input) : 0=compute forcing, 1=compute time derivative of the forcing.
//
// /Return value: equals the number of boundaries assigned.
// \end{CompositeGridSolverInclude.tex}  
//===============================================================================================
{
  assert( side0>=-1 && side0<2 );
  assert( axis0>=-1 && axis0<parameters.dbase.get<int >("numberOfDimensions") );
  
  int axisStart= axis0==-1 ? 0 : axis0;
  int axisEnd  = axis0==-1 ? parameters.dbase.get<int >("numberOfDimensions")-1 : axis0;
  int sideStart= side0==-1 ? 0 : side0;
  int sideEnd  = side0==-1 ? 1 : side0;
  
  int returnValue=0;
  
  const int numberOfDimensions = parameters.dbase.get<int >("numberOfDimensions");
  
  Parameters *pde = &parameters;
  if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")!=NULL )
  {
    ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));
    const int numberOfEquationDomains=equationDomainList.size();
    const int equationDomainNumber=equationDomainList.gridDomainNumberList[grid];
    assert( equationDomainNumber>=0 && equationDomainNumber<numberOfEquationDomains );
    EquationDomain & equationDomain = equationDomainList[equationDomainNumber];

    pde = equationDomain.getPDE();
  }
 
  #ifdef USE_PPP
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
  #else
    const intSerialArray & maskLocal = mg.mask();
  #endif  

  int includeGhost=1;

  BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);
  for( int axis=axisStart; axis<=axisEnd; axis++ )
  {
    for( int side=sideStart; side<=sideEnd; side++ )
    {
      // printF("parameters.bcType(side=%i,axis=%i,grid=%i)=%i\n",side,axis,grid,parameters.bcType(side,axis,grid));

      if( parameters.bcType(side,axis,grid)==Parameters::ramped ||  // new generic way
          parameters.bcType(side,axis,grid)==Parameters::rampInflow ||
          parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped
          )
      {
        if( parameters.bcType(side,axis,grid)!=Parameters::parabolicInflowRamped &&
            (parameters.bcVariesInSpace(side,axis,grid) || pBoundaryData[side][axis]!=NULL) )
	{
	  printf("ERROR: getTimeDependentBoundaryConditions: something is wrong here\n"
                 " parameters.bcVariesInSpace(side=%i,axis=%i,grid=%i)=%i "
                 " pBoundaryData[side=%i][axis=%i]!=NULL =%i \n",
		 side,axis,grid,parameters.bcVariesInSpace(side,axis,grid),
                 side,axis,pBoundaryData[side][axis]!=NULL);
	  Overture::abort("error");
	}

        returnValue++;
	
        // ** This should be fixed -- parameters should be put in a data base by name and looked up here
        //  for e.g.   getPar("ramp(side=0,axis=1,grid=2,ua)",value)

        int numberToSet=0; 
        IntegerArray component;
	parameters.getComponents(component);
	numberToSet = component.getLength(0);

	const int nDim=max(component)+1;
        RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
        RealArray par(2+2*nDim);
        parameters.getTimeDependenceBoundaryConditionParameters(side,axis,grid,par);

	// ramped values: u = ua
        real ta=par(0);
        real tb=par(1);
	
	const int & uc = parameters.dbase.get<int >("uc");
	// const int & vc = parameters.dbase.get<int >("vc");
	// const int & wc = parameters.dbase.get<int >("wc");

	
        #define TIMEPAR(n,c) par(2+(n)+2*(c))
	for( int n=0; n<numberToSet; n++ )
	{
	  // int c = uc+n;
	  const int c = component(n);  

          real ua=TIMEPAR(0,c); 
          real ub=TIMEPAR(1,c); 

	  //           printf(" BC: ramp: t=%8.2e, [ta,tb]=[%7.2e,%7.2e], ramp component c=%i in [%7.2e,%7.2e]\n",
	  //                     t,ta,tb,c,ua,ub);
	  
	  if( t>=tb )
	  {
	    if( forcingType==computeForcing )
	      bcData(c,side,axis,grid)=ub;
	    else
	      bcData(c,side,axis,grid)=0.;
	  }
	  else if( t<=ta )
	  {
	    if( forcingType==computeForcing )
	      bcData(c,side,axis,grid)=ua;
	    else
	      bcData(c,side,axis,grid)=0.;
	  }
	  else
	  {
	    // linear ramp:
	    // real dtr= (t-ta)/max(REAL_EPSILON,tb-ta);
	    // bcData(uc,side,axis,grid)=ua+(ub-ua)*dtr;
	    // bcData(vc,side,axis,grid)=va+(vb-va)*dtr;
	    // bcData(wc,side,axis,grid)=wa+(wb-wa)*dtr;

	    // cubic ramp
	    real tba=max(REAL_EPSILON,tb-ta);
	    real dta=t-ta;
	  
	    //  r = dta*dta*( -dta/3.+.5*tba )*6.(ub-ua)/(tba*tba*tba) +ua;
	    if( forcingType==computeForcing )
	      bcData(c,side,axis,grid)=dta*dta*( -dta/3.+.5*tba )*6.*(ub-ua)/(tba*tba*tba) +ua;
	    else
	      bcData(c,side,axis,grid)=dta*( -dta + tba )*6.*(ub-ua)/(tba*tba*tba) +ua;

	    // printf("ramp BC: t=%6.2e: u=%6.2e\n",t,bcData(uc,side,axis,grid));
	  
	  }
	}
	#undef TIMEPAR

        if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflowRamped )
 	{
	  // scale the parabolic inflow profile by a ramp.
          // The original parabolic profile is saved in the ghost points : bd(g1,Ig2,Ig3,c)
          Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
          getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);
          getGhostIndex(mg.extendedIndexRange(),side,axis,Ig1,Ig2,Ig3);

	  bool ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ib1,Ib2,Ib3,includeGhost);
	  ok=ok && ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ig1,Ig2,Ig3,includeGhost);
	  if( !ok ) continue;  // no points on this processor

	  Range C=parameters.dbase.get<int >("numberOfComponents");
 	  RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	  // printf("parabolic ramp BC: t=%6.2e: force=%i u(ramp)=%6.2e\n",t,(int)forcingType,bcData(uc,side,axis,grid));

	  for( int c=0; c<parameters.dbase.get<int >("numberOfComponents"); c++ )
     	    bd(Ib1,Ib2,Ib3,c)=bd(Ig1,Ig2,Ig3,c)*bcData(c,side,axis,grid);
 	}

      }
      else if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflowOscillating )
      {
        RealArray par(8);
        parameters.getTimeDependenceBoundaryConditionParameters(side,axis,grid,par);
        real u0[3], a0,a1;
        real omega=par(0), t0=par(1);
        a0=par(2);
	a1=par(3);
        u0[0]=par(4), u0[1]=par(5), u0[2]=par(6);
	real maxtime=par(7);
	
	Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
	getBoundaryIndex(mg.extendedIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.extendedIndexRange(),side,axis,Ig1,Ig2,Ig3);

	bool ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ib1,Ib2,Ib3,includeGhost);
	ok=ok && ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ig1,Ig2,Ig3,includeGhost);
	if( !ok ) continue;  // no points on this processor

	RealArray & bd = parameters.getBoundaryData(side,axis,grid,mg);

	real factor;
	if (maxtime < 0.0 || t < maxtime) {
	  if( forcingType==computeForcing )
	    factor=a0+a1*cos(2.*Pi*omega*(t-t0));
	  else
	    {
	      // compute the time derivative of the forcing.
	      u0[0]=u0[1]=u0[2]=0.;
	      factor=-2.*Pi*omega*a1*sin(2.*Pi*omega*(t-t0));
	    }
	} else {
	  if( forcingType==computeForcing )
	    factor=a0+a1*cos(2.*Pi*omega*(maxtime-t0));
	  else {
	    // compute the time derivative of the forcing.
	    u0[0]=u0[1]=u0[2]=0.;
	    factor=0.0;
	  }
	}
 
	printf("parabolic oscillate BC: t=%6.2e: force=%i t0=%6.2e a0=%6.2e a1=%6.2e omega=%6.2e "
               "factor=%6.2e u0=%8.2e v0=%8.2e w0=%8.2e\n",
	       t,(int)forcingType,t0,a0,a1,omega,factor,u0[0],u0[1],u0[2]);

        // **FIX ME FOR pressure info ***
	for( int n=0; n<numberOfDimensions; n++ )
	{
	  int c = parameters.dbase.get<int >("uc")+n;
          bd(Ib1,Ib2,Ib3,c)=u0[n]+factor*bd(Ig1,Ig2,Ig3,c);
	}
	
      }
      else if( parameters.bcType(side,axis,grid)==Parameters::uniformInflowOscillating )
      {
        const int uc = parameters.dbase.get<int >("uc");
        RealArray par(7);
        parameters.getTimeDependenceBoundaryConditionParameters(side,axis,grid,par);
        real omega=par(0), t0=par(1), a0=par(2), a1=par(3), u0[3];
        u0[0]=par(4), u0[1]=par(5), u0[2]=par(6);
        
        RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
        RealArray & bcParameters = parameters.dbase.get<RealArray>("bcParameters");

        // *wdh* FIXED 2011/08/30, (also fixed setBoundaryConditions.C line 850)
	real factor;
	if( forcingType==computeForcing )
	  factor=a0+a1*cos(2.*Pi*omega*(t-t0));
	else
	  {
	    // compute the time derivative of the forcing.
	    u0[0]=u0[1]=u0[2]=0.;
	    factor=-2.*Pi*omega*a1*sin(2.*Pi*omega*(t-t0));
	  }

//         printF(" bcParameters=[%g,%g,%g,%g]\n",
// 	       bcParameters(0,side,axis,grid),
// 	       bcParameters(1,side,axis,grid),
// 	       bcParameters(2,side,axis,grid),
// 	       bcParameters(3,side,axis,grid));

        real uVal[3]={0.,0.,0.}; 
        for( int n=0; n<numberOfDimensions; n++  )
	{
	  int c = uc+n;
          real uu = bcParameters(c,side,axis,grid); // holds value set by u=1., v=2., etc
          uVal[n] = u0[n]+factor*uu;
	}
	
	printf("uniform oscillate BC: t=%6.2e: force=%i a0=%6.2e, a1=%6.2e omega=%6.2e (u0,v0,w0)=(%g,%g,%g) factor=%6.2e,"
                " set (u,v,w)=[%8.2e,%8.2e,%8.2e]\n",
               t,(int)forcingType,a0,a1,omega,u0[0],u0[1],u0[2],factor,uVal[0],uVal[1],uVal[2]);

	for( int n=0; n<numberOfDimensions; n++ )
	{
	  int c = uc+n;
	  bcData(c,side,axis,grid)=uVal[n];
	}

      }
    }
  }

  return returnValue;
}




// ======================================================================================================
/// \brief Return true if the face corresponding to the side of a grid is parallel
///   to an x, y, or z coordinate plane. This function is used, for example, to determine if a slip wall
///   is really flat, in which case the boundary conditions may be simpler.
///
/// \param side, axis,grid  (input) : face to check
/// \param cg (input) : Composite grid
/// \param normalAxis (output) : 0,1 or 2 indicates coordinate axis (x,y, or z) in the normal direction.
// ======================================================================================================
bool DomainSolver::
faceIsACoordinatePlane( const int side, const int axis, int grid, CompositeGrid & cg,
                        int & normalAxis )
{

  int ok=false; // set to true if this face is parallel to a coordinate axis
  normalAxis=axis; // default for Cartesian grids, "axis" is the normalAxis
  

  if( !parameters.gridIsMoving(grid) ) // for now assume moving grids do not satisfy the condition
  {
    MappedGrid & mg = cg[grid];
    bool isRectangular = mg.isRectangular();
    if( isRectangular )
    {
      ok=true;  // non-moving rectangular grid 
    }
    else
    {
      // non-rectangular grid 
      // Check for:
      //    1. stretched cartesian grid
      //    2. flat end of a cylinder
      Mapping & map = mg.mapping().getMapping();
      if( map.getClassName()=="StretchTransform" )
      {
	MappingRC & map2 = ((StretchTransform&)map).map2;
	const aString & mapClassName = map2.getClassName();
	if( mapClassName=="SquareMapping" || mapClassName=="BoxMapping" )
	{ // stretched square or box 
	  ok=true;
	}
	else if( mapClassName=="CylinderMapping" )
	{ // stretched cylinder
           ok = axis==axis2;  // axis2 is the axial direction
	}
	
      }
      else if( map.getClassName()=="CylinderMapping" )
      { // flat end of a cylinder
	// CylinderMapping & cyl = (CylinderMapping&)map;
	ok = axis==axis2;  // axis2 is the axial direction
      }
      else // general case 
      {
	// --- General case: check the normals on the face:
        //    1. normals must be all the same (flat face)
        //    2. normals must be in a coordinate direction

	
        // ***** check the  normals on the face ******
        mg.update(MappedGrid::THEvertexBoundaryNormal);
        OV_GET_VERTEX_BOUNDARY_NORMAL(mg,side,axis,normal)

        const int numberOfDimensions=cg.numberOfDimensions();

        Index Ib1,Ib2,Ib3;
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);     
	OV_GET_SERIAL_ARRAY_CONST(int,mg.mask(),maskLocal);
	int includeGhost=0;
	bool localDataExist =ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,Ib1,Ib2,Ib3,includeGhost);

        ok=true;  // Assume true on all processors
	if( localDataExist ) // this processor has data
	{
	  real nv0[3]; // normal at some point
	  int i1=Ib1.getBase(), i2=Ib2.getBase(), i3=Ib3.getBase();
	  for( int dir=0; dir<numberOfDimensions; dir++ ){ nv0[dir]=normal(i1,i2,i3,dir); } // 

	  const real normalTol=REAL_EPSILON*1000.; // tolerance for normals being the same at different points 

	  ok=false;
	  // Normal must be in the direction of a coordinate axis (x, y, or z)
	  for( int dir=0; dir<numberOfDimensions; dir++ )
	  {
	    if( fabs( fabs(nv0[dir])-1. ) < normalTol )  // one nv[dir] should be 1 or -1 
	    {
	      normalAxis=dir;
	      ok=true;
	      break;
	    }
	
	  }
	  if( ok )
	  {
	    // --- nv0 was in a coordinate direction, now check that all normals match
      
	    real diff=0.;
	    FOR_3D_AND_OK(i1,i2,i3,Ib1,Ib2,Ib3)
	    {
	      for( int dir=0; dir<numberOfDimensions; dir++ )
	      {  
		diff=max(diff,fabs(normal(i1,i2,i3,dir)-nv0[dir]));  // normals should all match 
	      }
	      if( diff>normalTol )
	      {
		ok=false;  // normals do not match 
		break;
	      }
	    }
	  }
	} // end localDataExist 
      
	ok = ParallelUtility::getMinValue(ok ); // all processors must agree

      } // end else general case 
      
    }
  }

  if( ok && (debug() & 4) )
    printF("--DS-- faceIsACoordinatePlane: INFO: face (side,axis,grid,name)=(%i,%i,%i,%s) is a coordinate plane,"
           " normalAxis=%i.\n",
	   side,axis,grid,(const char*)cg[grid].getName(),normalAxis);

  return ok;
}
