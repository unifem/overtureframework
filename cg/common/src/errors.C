#include "DomainSolver.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"
#include "FileOutput.h"
#include "ProbeInfo.h"
#include "HDF_DataBase.h"
#include "BodyForce.h"

void DomainSolver::
determineErrors(GridFunction & cgf,
		const aString & label /* =nullString */)
//===================================================================
/// \brief 
///     Compute the errors in the computed solution
/// 
//===================================================================
{

  RealArray err(numberOfComponents()+3);
  const GridFunction::Forms form = cgf.form;
  if( form==GridFunction::conservativeVariables )
    cgf.conservativeToPrimitive();
  
  determineErrors(cgf.u,cgf.gridVelocity,cgf.t,0,err,label);

  if( form==GridFunction::conservativeVariables )
    cgf.primitiveToConservative();

}


void DomainSolver::
determineErrors(realCompositeGridFunction & u,
		realMappedGridFunction **gridVelocity,
		const real & t, 
		const int options,
                RealArray & err,
		const aString & label /* =nullString */  )
//===================================================================
/// \brief 
///     Compute the errors in the computed solution
/// 
/// \param options (input): 0=compute error in u, 1=compute error in u.t
/// 
/// \param err (output) : Maximum errors in each component
/// 
//===================================================================
{
  assert( u.getCompositeGrid()!=NULL );
  CompositeGrid & cg = *u.getCompositeGrid();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  Index I1,I2,I3;
  
  //  if( !parameters.dbase.get<bool >("twilightZoneFlow") &&  parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")==Parameters::noKnownSolution )
  if( !parameters.dbase.get<bool >("twilightZoneFlow") &&  
      !parameters.dbase.get<realCompositeGridFunction* >("pKnownSolution") )
  {
    if( label!=nullString && ( debug() & 2 || debug() & 8 )  )
    {
      if( parameters.dbase.get<int >("myid")==0 )
      {
	fprintf(parameters.dbase.get<FILE* >("debugFile"),(const char*)label);
	if( parameters.dbase.get<FILE* >("debugFile")!=parameters.dbase.get<FILE* >("pDebugFile") )
          fprintf(parameters.dbase.get<FILE* >("pDebugFile"),(const char*)label);
      }
    }
    
    if( debug() & 2 )
    {
      // output the min/max of all components
      // determine the max/min of all components: uMax, uMin, uvMax
      RealArray uMin, uMax;
      real uvMax;
      getBounds(u,uMin,uMax,uvMax);

      if( parameters.dbase.get<int >("myid")==0 )
      {
	fprintf(parameters.dbase.get<FILE* >("debugFile")," -----  t = %10.3e \n",t);
	int n;
	for( n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
	  fprintf(parameters.dbase.get<FILE* >("debugFile"),"   %10s : (min,max)=(%14.7e,%14.7e) \n",(const char*)parameters.dbase.get<aString* >("componentName")[n],
		  uMin(n),uMax(n));
      }
      
    }
    if( debug() & 8 )
    {
      if( parameters.dbase.get<int >("myid")==0 ) 
        fprintf(parameters.dbase.get<FILE* >("debugFile"),"determineErrors: real-run: just printing the solution at t=%e\n",t);
      outputSolution( u,t );
    }
    return;
  }
  //  else if( parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=Parameters::noKnownSolution )
  else if( parameters.dbase.get<realCompositeGridFunction* >("pKnownSolution") )
  {
    // ***** Compute the errors for a knownSolution ********

    RealArray errk(numberOfComponentGrids,numberOfComponents);
    realCompositeGridFunction & uKnown = parameters.getKnownSolution( cg, t );
    

    realCompositeGridFunction v;  // **fix me for parallel**
    v=u-uKnown;
    const int maskOption=0;  // check points where mask != 0

    // We print the max norm and optionally some lp norms
    const int errorNorm = parameters.dbase.get<int >("errorNorm");
    int numberOfNormsToPrint=1;
    if( errorNorm<10000 ) numberOfNormsToPrint+=errorNorm;
    for( int inorm=0; inorm<numberOfNormsToPrint; inorm++ )
    { // inorm==0 : max-norm, otherwise Lp-norm with p=norm
      int pNorm = inorm==0 ? INT_MAX : inorm;
      err=0.;
      for( int n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
      {
	if( pNorm<10000 )
	{
	  err(n)=lpNorm(pNorm,v,n,maskOption,parameters.dbase.get<int >("checkErrorsAtGhostPoints") );
	}
	else
	{ // assume this is the max-norm
	  err(n)=maxNorm(v,n,maskOption,parameters.dbase.get<int >("checkErrorsAtGhostPoints") );
	}
      }

      aString normName;
      if( pNorm<1000 )
	sPrintF(normName,"l%i-norm",pNorm);
      else
	normName="maxNorm";
      printf("determineErrors: t=%9.3e, %s errors: [rho,u,v,T]=[%8.2e,%8.2e,%8.2e,%8.2e]\n",t,(const char*)normName,
	     err(0),err(1),err(2),err(3));
    }
    

//     errk=0.;
//     err=0.;
//     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     {
//       const intArray & mask = cg[grid].mask();
//       getIndex(cg[grid].gridIndexRange(),I1,I2,I3,parameters.dbase.get<int >("checkErrorsAtGhostPoints"));
//       where( mask(I1,I2,I3)!=0 )
//       {
// 	for( int n=0; n<parameters.dbase.get<int >("numberOfComponents"); n++ )
// 	{
// 	  errk(grid,n)=max(errk(grid,n),max(fabs(u[grid](I1,I2,I3,n)-uKnown[grid](I1,I2,I3,n)))); 
// 	  err(n)=max(err(n),errk(grid,n));
// 	}
	  
//       }
//     }
//     printf("determineErrors: t=%9.3e, Max errors: [rho,u,v,T]=[%8.2e,%8.2e,%8.2e,%8.2e]\n",t,
//     	   err(0),err(1),err(2),err(3));


    return;
  }
  

  OGFunction & e = *parameters.dbase.get<OGFunction* >("exactSolution");
  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *pDebugFile = parameters.dbase.get<FILE* >("pDebugFile");

  
  RealArray errk(numberOfComponentGrids,numberOfComponents);
  RealArray umaxk(numberOfComponentGrids,numberOfComponents);
  IntegerArray ive(3,numberOfComponentGrids,numberOfComponents);
  ive=0;
  RealArray xv(3);

  real umax0=0.;

  real error,er,um;

  if( label!=nullString )
    fprintf(pDebugFile,(const char*)label);

  err=0.;
  xv(axis3)=0.;
  for( int n=0; n<numberOfComponents; n++ )
  {
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
      MappedGrid & c = cg[grid];
      realArray & uu = u[grid];
      intArray & mask = c.mask();
      
      // **** watch out *** the error in the extrapolated ghost corner point can be large -- this can lead to 
      // a convergence rate that looks larger than 2 
      int nExtra= parameters.dbase.get<int >("checkErrorsAtGhostPoints"); // debug() & 4 ? 1 : 0;
      
      getIndex(c.gridIndexRange(),I1,I2,I3,nExtra);

      er=0.;
      um=0.;

      c.update(MappedGrid::THEcenter);
      realArray & x= c.center();
      #ifdef USE_PPP
        realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(uu,uLocal);
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      #else
        const realSerialArray & uLocal = uu; 
        const realSerialArray & xLocal = x;
        const intSerialArray & maskLocal = mask; 
      #endif 


      // *wdh* 040930
      int nv[2][3], &n1a=nv[0][0], &n1b=nv[1][0], &n2a=nv[0][1], &n2b=nv[1][1], &n3a=nv[0][2], &n3b=nv[1][2]; 
      #ifdef USE_PPP
       // loop bounds for this boundary:
       n1a=max(I1.getBase(),uLocal.getBase(0)); n1b=min(I1.getBound(),uLocal.getBound(0));
       n2a=max(I2.getBase(),uLocal.getBase(1)); n2b=min(I2.getBound(),uLocal.getBound(1));
       n3a=max(I3.getBase(),uLocal.getBase(2)); n3b=min(I3.getBound(),uLocal.getBound(2));
      #else
       // loop bounds for this boundary:
       n1a=I1.getBase(); n1b=I1.getBound();
       n2a=I2.getBase(); n2b=I2.getBound();
       n3a=I3.getBase(); n3b=I3.getBound();
      #endif

      if( debug() & 4 )
        fprintf(pDebugFile,"Errors(gridIndexRange+%i): n= %i, (%s), grid %i, t =%12.4e (p=%i)"
               " [n1a,n1b]...=[%i,%i][%i,%i][%i,%i]\n",nExtra,
               n,(const char*)u.getName(n),grid,t,parameters.dbase.get<int >("myid"),n1a,n1b,n2a,n2b,n3a,n3b);

      if( n1a<=n1b && n2a<=n2b && n3a<=n3b )
      {

	I1=Range(n1a,n1b);
	I2=Range(n2a,n2b);
	I3=Range(n3a,n3b);

	realSerialArray ee(I1,I2,I3,Range(n,n));
	const bool isRectangular=false; // do this for now
	if( options==0 )
	{

          // get the exact solution
	  e.gd( ee,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,t);

	  // ee=e(c,I1,I2,I3,n,t);
	  
	}
	else
	{
	  if( !parameters.useConservativeVariables() )
	  {
            // get the time derivative of the exact solution
	    e.gd( ee,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,n,t);

	    // ee=e.t(c,I1,I2,I3,n,t);
	  }
	  else
	  {
            // ***** errors for conservative variables ******
	    const int rc = parameters.dbase.get<int >("rc");
	    const int uc = parameters.dbase.get<int >("uc");
	    const int vc = parameters.dbase.get<int >("vc");
	    const int wc = parameters.dbase.get<int >("wc");
	    const int tc = parameters.dbase.get<int >("tc");
	  
	    if( n==rc )
	    {
	      e.gd( ee,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,n,t);
	      // ee=e.t(c,I1,I2,I3,rc,t);
	    }
	    else if( n==uc || n==vc || n==wc )
	    {
	      realSerialArray r(I1,I2,I3), rt(I1,I2,I3), u(I1,I2,I3), ut(I1,I2,I3);
	      e.gd( r ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,rc,t);
	      e.gd( rt,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,rc,t);

	      e.gd( u ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,n,t);
	      e.gd( ut,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,n,t);

	      ee=rt*u+r*ut; 
	    }
	    else
	    {
	      // Energy = rho*( (Rg/(gamma-1.))*te+ .5*(uu*uu+v*v) ); 
	      const real Rg = parameters.dbase.get<real >("Rg");
	      const real gamma = parameters.dbase.get<real >("gamma");
	    
	      realSerialArray r(I1,I2,I3), rt(I1,I2,I3), u(I1,I2,I3), ut(I1,I2,I3), v(I1,I2,I3), vt(I1,I2,I3),
		te(I1,I2,I3), tet(I1,I2,I3);
	      e.gd( r ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,rc,t);
	      e.gd( rt,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,rc,t);

	      e.gd( te ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,tc,t);
	      e.gd( tet,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,tc,t);

	      e.gd( u ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,uc,t);
	      e.gd( ut,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,uc,t);

	      e.gd( v ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,vc,t);
	      e.gd( vt,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,vc,t);

	      realSerialArray energy; 
	      if( c.numberOfDimensions()==2 )
	      {
		energy = r*( Rg/(gamma-1.)*te + .5*( u*u+v*v ) );
		ee=rt*energy/r+r*( (Rg/(gamma-1.))*tet +  u*ut +v*vt );
	      
	      }
	      else
	      {
		realSerialArray w(I1,I2,I3), wt(I1,I2,I3);
		e.gd( w ,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,wc,t);
		e.gd( wt,xLocal,c.numberOfDimensions(),isRectangular,1,0,0,0,I1,I2,I3,wc,t);
	      
		energy = r*( Rg/(gamma-1.)*te + .5*( u*u+v*v+w*w ) );
		ee=rt*energy/r+r*( (Rg/(gamma-1.))*tet +  u*ut + v*vt + w*wt );
	      }
	    
// 	    const realArray & r  = e(c,I1,I2,I3,rc,t);
// 	    const realArray & uu = e(c,I1,I2,I3,uc,t);
// 	    const realArray & vv = e(c,I1,I2,I3,vc,t);
// 	    const realArray & te = e(c,I1,I2,I3,tc,t);
	  
// 	    const realArray & energy = evaluate( r*( Rg/(gamma-1.)*te + .5*( uu*uu+vv*vv ) ) );
// 	    ee=e.t(c,I1,I2,I3,rc,t)*energy/r+r*( (Rg/(gamma-1.))*e.t(c,I1,I2,I3,tc,t) +
// 						 uu*e.t(c,I1,I2,I3,uc,t)+vv*e.t(c,I1,I2,I3,vc,t) );
	    }
	  }
	  if( parameters.gridIsMoving(grid) )
	  {
            #ifdef USE_PPP
	     realSerialArray gridVelocityLocal; getLocalArrayWithGhostBoundaries((*gridVelocity[grid]),gridVelocityLocal);
	    #else
	     const realSerialArray & gridVelocityLocal =(*gridVelocity[grid]); 
            #endif


	    realSerialArray ux(I1,I2,I3),uy(I1,I2,I3);
	    e.gd( ux,xLocal,c.numberOfDimensions(),isRectangular,0,1,0,0,I1,I2,I3,n,t);
	    e.gd( uy,xLocal,c.numberOfDimensions(),isRectangular,0,0,1,0,I1,I2,I3,n,t);
	  
	    if( c.numberOfDimensions()==2 )
	    {
	      ee+=gridVelocityLocal(I1,I2,I3,0)*ux+gridVelocityLocal(I1,I2,I3,1)*uy;
	    }
	    else
	    {
	      realSerialArray uz(I1,I2,I3); 
	      e.gd( uz,xLocal,c.numberOfDimensions(),isRectangular,0,0,0,1,I1,I2,I3,n,t);

	      ee+=gridVelocityLocal(I1,I2,I3,0)*ux+gridVelocityLocal(I1,I2,I3,1)*uy+
		gridVelocityLocal(I1,I2,I3,2)*uz; 
	    }

	    if( (debug() & 8) && n==1 )
	    {
	      display(gridVelocityLocal,sPrintF("determineErrors: **** gridVelocity *** t=%e, grid=%i",t,grid),pDebugFile,"%12.9f ");
	      display(xLocal,sPrintF("determineErrors: **** xLocal *** t=%e, grid=%i",t,grid),pDebugFile,"%12.9f ");
	      display(ee,sPrintF("determineErrors: **** ee *** t=%e, grid=%i",t,grid),pDebugFile,"%12.9f ");
	    }
	    
// 	  ee+=(*gridVelocity[grid])(I1,I2,I3,0)*e.x(c,I1,I2,I3,n,t)+
// 	        (*gridVelocity[grid])(I1,I2,I3,1)*e.y(c,I1,I2,I3,n,t);
// 	  if( c.numberOfDimensions()>2 )
// 	    ee+=(*gridVelocity[grid])(I1,I2,I3,2)*e.z(c,I1,I2,I3,n,t);
	  }
	}
      

        realSerialArray es;
	es=ee;
	
	um=max(abs(ee));

	ee=abs(ee-uLocal(I1,I2,I3,n));
	if( options==0 )
	{
	  where( maskLocal(I1,I2,I3)==0 )
	    ee=0.;
	}
	else
	{
	  where( maskLocal(I1,I2,I3)<=0 )
	    ee=0.;
	}

      
	/// These next loops need to be fixed for parallel
	for( int i3=n3a; i3<=n3b; i3++ )


	{
	  if( debug() & 4 && c.numberOfDimensions() == 3 ) 
	    fprintf(pDebugFile,"   ++++ i3= %6i +++\n",i3);
	  for( int i2=n2a; i2<=n2b; i2++ )
	  {
	    for( int i1=n1a; i1<=n1b; i1++ )
	    {
	      error=ee(i1,i2,i3);
	      if( fabs(error) > er )
	      {
		er=fabs(error);
		ive(0,grid,n)=i1;
		ive(1,grid,n)=i2;
		ive(2,grid,n)=i3;
	      }
	      if( debug() & 4 ) fprintf(pDebugFile,"%9.1e (%5.2f)",error,es(i1,i2,i3));
	      // if( debug() & 4 ) fprintf(pDebugFile,"%9.1e ",error);
	    }
	    if( debug() & 4  ) fprintf(pDebugFile,"\n");
	  }
	}
      }


      errk(grid,n)=ParallelUtility::getMaxValue(er); 
      err(n)=max(err(n),errk(grid,n));
      umaxk(grid,n)=ParallelUtility::getMaxValue(um);
      umax0=max(umax0,umaxk(grid,n));
    }
  }
	     
  if( parameters.dbase.get<int >("myid")==0 && debug() & 2 )
  {
    const int numFiles = parameters.dbase.get<FILE* >("debugFile")!=parameters.dbase.get<FILE* >("pDebugFile") ? 2 : 1;
    for(int ifile=0; ifile<numFiles; ifile++ )
    {
      FILE *file = ifile==0 ? parameters.dbase.get<FILE* >("debugFile") : parameters.dbase.get<FILE* >("pDebugFile");
      if( label!=nullString )
        fprintf(file,(const char*)label);
      fprintf(file,
	      "     Maximum Errors at t =%12.4e,  umax =%12.4e\n"
	      "    n  grid  i1  i2  i3  uMax(grid,n) err(grid,n)\n",t,umax0);
      for( int n=0; n<numberOfComponents; n++ )
      {
	for( int grid=0; grid<numberOfComponentGrids; grid++)
	{
	  fprintf(file,
		  "  %3s %4i  %3i %3i %3i  %10.3e   %14.7e\n",(const char*)parameters.dbase.get<aString* >("componentName")[n],
		  grid,ive(0,grid,n),ive(1,grid,n),ive(2,grid,n),umaxk(grid,n),errk(grid,n));
	}
      }
    }
  }
  
}


void DomainSolver::
outputSolution( realCompositeGridFunction & u, const real & t,
		const aString & label /* =nullString */,
                int printOption /* = 0 */  )
//==============================================================================
/// \brief 
///      Output the solution
///  
/// \param label (input) : label to print
/// \param printOption (input) : 
///    When running in parallel, this option determines whether
///    to print the local arrays to separate files parameters.dbase.get<FILE* >("pDebugFile") (printOption=1)
///   or to print all values to the single file parameters.dbase.get<FILE* >("debugFile") (printOption=0) which
///   requires communication to transfer the solution to processor 0.
/// 
/// \note The mappedGridFunction version of this is in OB_MappedGridSolver
/// 
//==============================================================================
{
  const int numProc= max(1,Communication_Manager::numberOfProcessors());
  const bool printOnThisProcessor = parameters.dbase.get<int >("myid")==0 || printOption==1;
  FILE *debugFile = printOption==0 ? parameters.dbase.get<FILE* >("debugFile") : parameters.dbase.get<FILE* >("pDebugFile"); 
  CompositeGrid & cg = *u.getCompositeGrid();

  if( label!=nullString )
  {
    if( printOnThisProcessor ) fprintf(debugFile,(const char*)label);
  }
  int nghost=1;
  //kkc 070125  if( parameters.dbase.get<int >("orderOfAccuracy")==4 )
  //kkc 070125    nghost=2;
  nghost = parameters.numberOfGhostPointsNeeded() ;

  bool showMasked=true;
  
  Partitioning_Type partition; 
  const int processorForDisplay=0;
  partition.SpecifyProcessorRange(Range(processorForDisplay,processorForDisplay));

// char format[] = "%10.3e "; // output format
// char format[] = "%14.7e "; // output format
//  char format[] = "%16.9e "; // output format
//  char format[] = "%20.13e "; // output format
  const char *format = (const char*)parameters.dbase.get<aString >("outputFormat");

  Index I1,I2,I3;
  for( int grid=0; grid < cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];
    intArray *pmask=&c.mask(); 
    realArray *pua=&u[grid]; 
    
    if( printOption==0 && numProc>1 )
    {
      // copy the solution and mask to processor 0 so that we can output the results to a single file

      const realArray & x = u[grid];
      if( x.getInternalPartitionPointer()!=NULL )
      {
	Partitioning_Type xPartition=x.getPartition();
	for( int axis=0; axis<MAX_ARRAY_DIMENSION; axis++ )
	{
	  int ghost=xPartition.getGhostBoundaryWidth(axis);
	  if( ghost>0 )
	    partition.partitionAlongAxis(axis, true , ghost);
	  else
	    partition.partitionAlongAxis(axis, false, 0);
	}
      }
  
      realArray & ub = *new realArray;  
      ub.partition(partition); 
      Range R0=x.dimension(0), R1=x.dimension(1), R2=x.dimension(2), R3=x.dimension(3);
      ub.redim(R0,R1,R2,R3); 

      // ub=x;  // copy data to processor. 0    ******* *wdh* 060530 -- this caused problems
      int nd=4;
      Index Iv[4]; Iv[0]=R0; Iv[1]=R1; Iv[2]=R2; Iv[3]=R3; 
      ParallelUtility::copy(ub,Iv,x,Iv,nd);

      intArray & maskb = *new intArray;
      maskb.partition(partition); 
      maskb.redim(R0,R1,R2); 

      // maskb=c.mask();  // copy data to processor. 0  ******* *wdh* 060530 -- this caused problems
      Iv[3]=Range(0,0);
      ParallelUtility::copy(maskb,Iv,c.mask(),Iv,nd);

      pmask = &maskb;
      pua = &ub;
    }

    const intArray & mask0 = *pmask; 
    const realArray & ua = *pua; 


    #ifdef USE_PPP
      intSerialArray mask; getLocalArrayWithGhostBoundaries(mask0,mask);
      realSerialArray ug;  getLocalArrayWithGhostBoundaries(ua,ug);
    #else 
      const intSerialArray & mask = mask0; 
      const realSerialArray & ug = ua; 
    #endif
    if( printOnThisProcessor ) 
      fprintf(debugFile,
      " ---------grid = %6i : values shown on gridIndexRange + %i ghost line (mask values %s) (p=%i)------------\n",
                grid,nghost,(showMasked ? "shown" : "set to zero"),parameters.dbase.get<int >("myid"));
    getIndex(c.gridIndexRange(),I1,I2,I3,nghost);

    bool ok = ParallelUtility::getLocalArrayBounds(ua,ug,I1,I2,I3); // restrict bounds to the local array
    if( !ok ) continue;
      
    for( int n=u.getComponentBase(0); n<=u.getComponentBound(0); n++ )
    {
      if( printOnThisProcessor ) 
        fprintf(debugFile," ***** %s ****  t = %e, grid=%i (%s)\n",(const char*)parameters.dbase.get<aString* >("componentName")[n],t,grid,
                      (const char*)cg[grid].getName());
      for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
      {
        if( c.numberOfDimensions() == 3 ) fprintf(debugFile,"   ++++ i3= %6i +++\n",i3);
        for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	{
	  for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	  {
            if( showMasked || mask(i1,i2,i3)!=0 )
	    {
              if( printOnThisProcessor ) fprintf(debugFile,format,ug(i1,i2,i3,n));
	    }
	    else
	    {
              if( printOnThisProcessor ) fprintf(debugFile,format,0.);
	    }
	  }
          if( printOnThisProcessor ) fprintf(debugFile,"\n");
	}
      }
    }

    if( printOption==0 && numProc>1 )
    {
      delete pua;
      delete pmask;
    }
  }
}


int DomainSolver::
output( GridFunction & gf0, int stepNumber )
// ===================================================================================
/// \brief  This routine is called every time step in order to output results.
///
/// This routine will call userDefinedOutput, tracking, and outputProbes. Any FileOutput
/// files will also be saved. 
// ==================================================================================
{
  int & outputStepNumber = parameters.dbase.get<int>("outputStepNumber");

  // In some cases this routine is called more than once for the same stepNumber.
  // We therefore return if the stepNumber is the same as the previous time.
  if( outputStepNumber == stepNumber )
    return 0;

  outputStepNumber=stepNumber;

  if( parameters.dbase.get<int >("allowUserDefinedOutput") )
    userDefinedOutput( gf0, stepNumber );

  if( parameters.dbase.get<int >("trackingIsOn") )
    tracking( gf0, stepNumber );

  // output to any probe files
  outputProbes( gf0, stepNumber );

  // flush standard out in case it is being re-directed (and buffered) to a file
  fflush(stdout);

  if( parameters.dbase.get<int >("numberOfOutputFiles")==0 )
    return 0;

  // --- Save FileOutput files that the user has created ---
  // These are ascii files in which various grid function info is saved

  char buff[80];
  realCompositeGridFunction v;
  realCompositeGridFunction & u = getAugmentedSolution( gf0,v );

  // we set the names for the component grids
  for( int grid=0; grid<gf0.cg.numberOfComponentGrids(); grid++ )
  {
    for( int c=u.getComponentBase(0); c<=u.getComponentBound(0); c++ )  
      u[grid].setName(u.getName(c),c);
  }

  for( int n=0; n<parameters.dbase.get<int >("numberOfOutputFiles"); n++ )
  {
    if( stepNumber % parameters.dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[n] == 0 )
    {
      printf("output: u.getName(1)=%s u[0].getName(0)=%s \n",(const char*)u.getName(1),
              (const char*)u[0].getName(0));
      parameters.dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n]->save(u,sPrintF(buff,"%e  %i  : time and step number",gf0.t,stepNumber));
    }
  }

    
  return 0;
}
