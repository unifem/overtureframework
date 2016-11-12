//===============================================================================
//  Compare solutions for convergence testing.
//
//   o read solutions from show files that correspond to different resolutions
//   o estimate errors and convergence rates
//
//==============================================================================
#include "Overture.h"
#include "Ogshow.h"  
#include "ShowFileReader.h"
#include "interpPoints.h"
#include "display.h"
#include "FortranIO.h"
#include "PlotStuff.h"
#include "InterpolatePoints.h"
#include "gridFunctionNorms.h"
#include "ParallelUtility.h"
#include "InterpolatePointsOnAGrid.h"
#include <time.h>

#include OV_STD_INCLUDE(vector)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// ====================================================================================================
//
// Estimate the convergence rate and actual error for a sequence
// of solutions with
//      h(i) : i=0,...,n
//      e(i) = u(h(i))-u(h(n)) i=0,1,...,n-1
// 
//  Assume the solution satisfies:
//          u(h) - u = C h^{sigma}
//
//     then 
//          e(i) == u(h(i)) - u(h(n)) = C( h^s - h(n)^s )   (s==sigma)
//
// Given at least 3 solutions (N>=2) we can get estimates for sigma and C
//   To get sigma we need to find a solution to the nonlinear equation
//      e(0)/e(1) = ( d(0)^s -1 ) /( d(1)^s -1 ) 
//   where
//        d(i) = h(i)/h(n) > 1 and 
// Thus we need to solve
//    f(s) = delta (d(1)^s -1)  - (d(0)^s -1) = 0    (delta=e(0)/e(1)
//  
// Following the algorithm taken from C.J. Roy "Review of Discretization Error Estimators in Scientific Computing",
//   AIAA 2010-126
//
//    [ u(1) - u(2) ] / [ u(2) - u(n) ] =   [ h(1)^s - h(2)^s ]/[ h(2)^s - h(n)^s ]
// 
// Case I: Constant grid refinement:
//   if h(1)=r*h(2) and h(2)=r*h(n) then
//    [ u(1) - u(2) ] / [ u(2) - u(n) ] =   [h(2)/h(n)]^s =  r^s 
//   then 
//    sigma = s = log( [u(1)-u(2)] / [ u(2) - u(n) ] )/ log( r )
//
// Case II: Non-const grid refinement:
//
// Solve by iteration:
//     s(k+1) = log( (rp-1)*( [u(1)-u(2)] / [ u(2) - u(n) ] ) + rp^s )/log( h(1)/h(n) )
//     rp = [h(2)/h(n)]^s(k)
//  with initial guess for s(0)
// 
// *wdh* new version 110416
// =====================================================================================================
real
computeRate( const int & n, const RealArray & h, const RealArray & e, real & sigma, real & c,
             const bool assumeFineGridHoldsExactSolution=false )
{
  assert( n>=2 );
  
  const int m0=n-2, m1=n-1;
  real delta=e(m0)/e(m1);
  real d0=h(m0)/h(n);
  real d1=h(m1)/h(n);
  real s= log(delta)/log(d0/d1);   // initial guess
  real ds=REAL_MAX;
  const real eps=1.e-3;
  
  const real r0 = h(m0)/h(m1), r1=h(m1)/h(n), rtol=1.e-5;
     
  if( assumeFineGridHoldsExactSolution )
  {
    // If the fine grid holds the exact solution then the rate is:
    sigma = log( e(m0)/e(m1) )/log( h(m0)/h(m1) );
    c=e(m0)/pow(h(m0),sigma);

    printF("++computeRate: Assuming the fine grid holds the exact solution: rate=%8.2e\n",s);
  }
  else 
  {
    if( fabs( r0-r1 )< rtol )
    {
      // Constant refinement factor:
      real eRatio = (e(m0) - e(m1) )/e(m1);
      if( eRatio>0. )
      {
        s= log( eRatio )/log( r1 );
        s=max(.01,s);  // cap rate at a small value for tables
      }
      else
      {
	s=-1;
	printF("++computeRate: WARNING: component n=%i, errors are not decreasing! Setting convergence rate to -1\n",n);
      }
      

      printF("++computeRate: there is a constant refinement factor r=%4.2f: e0=%8.2e, e1=%5.2e rate=%8.2e\n",
	     r0,e(m0),e(m1),s);
    }
    else
    {
      // Non constant grid refinement factor:
      // Algorithm taken from C.J. Roy "Review of Discretization Error Estimators in Scientific Computing",
      //   AIAA 2010-126
      real r12 = h(m1)/h(n), r23=h(m0)/h(m1);
       
      real s0 = s;  // initial guess at the rate
      real dRatio = (e(m0) - e(m1) )/e(m1);
      real logr = log( r12*r23 );
      const int maxIterations=50;
      for( int it=0; it<=maxIterations; it++ )
      {
	real rp = pow(r12,s0);
	s = log( (rp-1.)*dRatio + rp )/logr;
	ds=s-s0;
	s0=s;
	if( fabs(ds)<eps )
	  break;
      }
      if( fabs(ds)>eps )      
      {
	printF("++computeRate:ERROR: no convergence in computing the convergence rate!\n");
	s=-1.;
      }

      printF("++computeRate: non-constant refinement factor, r0=%4.2f, r1=%4.2f:  rate=%8.2e\n",r0,r1,s);
    }


    sigma=s;
    c=e(m0)/(pow(h(m0),s)-pow(h(n),s));
  }
  
  return s;
}

// *************** OLD VERSION ***************
real
computeRateOld( const int & n, const RealArray & h, const RealArray & e, real & sigma, real & c )
//
// Estimate the convergence rate and actual error for a sequence
// of solutions with
//      h(i) : i=0,...,n
//      e(i) = u(h(i))-u(h(n)) i=0,1,...,n-1
// 
//  Assume the solution satisfies:
//          u(h) - u = C h^{sigma}
//
//     then 
//          e(i) == u(h(i)) - u(h(n)) = C( h^s - h(n)^s )   (s==sigma)
//
// Given at least 3 solutions (N>=2) we can get estimates for sigma and C
//   To get sigma we need to find a solution to the nonlinear equation
//      e(0)/e(1) = ( d(0)^s -1 ) /( d(1)^s -1 ) 
//   where
//        d(i) = h(i)/h(n) > 1 and 
// Thus we need to solve
//    f(s) = delta (d(1)^s -1)  - (d(0)^s -1) = 0    (delta=e(0)/e(1)
//  
{
  assert( n>=2 );
  
  if( n>2 )
  {
    printF(" ** computeRate ** WARNING: I will only consider the 3 finest solutions when computing the rates\n");
  }
  

//   real delta=e(0)/e(1);
//   real d0=h(0)/h(n);
//   real d1=h(1)/h(n);
  
  const int m0=n-2, m1=n-1;
  real delta=e(m0)/e(m1);
  real d0=h(m0)/h(n);
  real d1=h(m1)/h(n);
  
  real s= log(delta)/log(d0/d1);   // initial guess
  const real eps=1.e-3; 
  real ds=2.*eps;
 
  if( delta>=3. )
  {
    // this doesn't seem to work if delta is too small!
    real f0,df,t0,t1;
    for( int it=0; it<21; it++ )
    {
      f0=delta*(pow(d1,s)-1.) - (pow(d0,s)-1);  
      df= delta*log(d1)*pow(d1,s)-log(d0)*pow(d0,s);
//    t1=pow(d1,s);
//    t0=pow(d0,s);
//    f0=delta*(t1-1.)/(t0-1.)-1.;
//    df=delta*( log(d1)*t1*(t0-1.)-(t1-1.)*log(d0)*t0 )/( (t0-1.)*(t0-1.) );
      ds = -f0/df;
      if( fabs(ds)>.1 )
	ds=.1*ds/fabs(ds);
      s+=ds;
      if( it>10 )
	printF("computeRate: it=%i, delta=%e, f0=%e, df=%e, ds=%e, s=%e \n",it,delta,f0,df,ds,s);
      if( fabs(ds)<eps )
	break;
    }
    if( fabs(ds)>eps )
    {
      printF("computeRate:ERROR in estimating the convergence rate! Will try alternate method...\n");
      printf(" e(%i)=%e, e(%i)=%e \n",m0,e(m0),m1,e(m1));
    }
    else
    {
     printF("computeRate: Estimated rate is %8.2e (ds=%8.2e)\n",s,ds);
    }
    
  }

  if( fabs(ds)>eps )
  { // ** here is a backup method ***
    // do a search for the best guess at s
    real sa=.1, sb=2.;
    real ns=1001;
    real sBest=sa, fMin=fabs(delta*log(d1)*pow(d1,sa)-log(d0)*pow(d0,sa));
    for( int i=0; i<ns; i++ )
    {
      real s = sa+(sb-sa)*i/ns;
      real f= fabs( delta*log(d1)*pow(d1,s)-log(d0)*pow(d0,s) );
      if( f<fMin )
      {
	sBest=s;
        fMin=f;
      }
    }
    s=sBest;
    printF("computeRate: Estimated rate is %8.2e fMin=%8.2e\n",s,fMin);
  }
  
  sigma=s;
  c=e(m0)/(pow(h(m0),s)-pow(h(n),s));
  return s;
}


//==============================================================================
/// \brief Output results in the form of a latex table 
//==============================================================================
int
outputLatexTable( const std::vector<aString> gridName,
		  const std::vector<aString> cName, 
		  const RealArray & cErr, 
		  const RealArray & cSigma,  
                  const RealArray & timeArray,
                  const int norm=-1,
		  FILE *file=stdout,
                  const bool assumeFineGridHoldsExactSolution=false )
{
  if( norm==0 )
    fprintf(file,"%% -------------- max norm results -------------\n");
  else if( norm==1 )
    fprintf(file,"%% -------------- l2 norm results -------------\n");
  else 
    fprintf(file,"%% -------------- l1 norm results -------------\n");

  if( assumeFineGridHoldsExactSolution )
    fprintf(file,"%% NOTE: the errors were computed assuming the fine grid holds the exact solution.\n");

  fprintf(file,"\\begin{table}[hbt]\\tableFont %% you should set \\tableFont to \\footnotesize or other size\n");
  fprintf(file,"%% \\newcommand{\\num}[2]{#1e{#2}} %% use this command to set the format of numbers in the table.\n");
  fprintf(file,"%% \\newcommand{\\errFormat}[1]{#1}} %% use this command to set the format of the error label.\n");
  fprintf(file,"\\begin{center}\n");

  const int numberOfComponents=cName.size();
  
  fprintf(file,"\\begin{tabular}{|l|");
  for( int j=0; j<numberOfComponents; j++ )
    fprintf(file,"c|c|");
  fprintf(file,"} \\hline \n");

  fprintf(file,"   show file        ");
  for( int c=0; c<cName.size(); c++ )
    fprintf(file," & \\errFormat{%s} &  r  ",(const char*)cName[c]);
  fprintf(file,"\\\\ \\hline\n");
  for( int grid=0; grid<gridName.size(); grid++ )
  {
    fprintf(file," %s ",(const char*)gridName[grid]);
    
    for( int c=0; c<cName.size(); c++ )
    {
      int exp = int( log10(cErr(grid,c))-.999999999999999);
      real frac = cErr(grid,c)/pow(10.,exp);
      //  fprintf(file,"& %2.1f{e%i} ",frac,exp);
      fprintf(file,"& \\num{%2.1f}{%i} ",frac,exp);
      if( grid>0 )
	fprintf(file,"& %4.1f ",cErr(grid-1,c)/cErr(grid,c));
      else
        fprintf(file,"&      ");
    }
    fprintf(file,"\\\\ \\hline\n");
  } 
  fprintf(file,"                     ");
  for( int c=0; c<cName.size(); c++ )
    fprintf(file," &    %5.2f      &     ",cSigma(c));
  fprintf(file,"\\\\ \\hline\n");

  fprintf(file,"\\end{tabular}\n");
  // fprintf(file,"\\hfill\n");

  // Get the current date
  time_t *tp= new time_t;
  time(tp);
  // const char *dateString = ctime(tp);
  aString dateString = ctime(tp);
  delete tp;
    
  fprintf(file,"\\caption{%s-norm self convergence results, t=%12.6e, %s. }\n",
	  (norm==0 ? "Max" : norm==1 ? "L2" : "L1"),timeArray(0),
          (const char*)dateString(0,dateString.length()-2) );   // caption

  fprintf(file,"\\end{center}\n");
  fprintf(file,"\\end{table}\n");

  return 0;
}


struct ComponentVector
{
aString name;
std::vector<int> component;
};


// ==============================================================================================================
///  \brief read solutions from all files 
//  ==============================================================================================================
int readSolutions( int numberOfFiles, 
                   CompositeGrid *cg, 
                   realCompositeGridFunction *u,
                   ShowFileReader *showFileReader, int *solutionNumber, 
                   IntegerArray & frameSeries,
                   aString *fileName,
                   RealArray & time,
                   IntegerArray &  numComponentsPerFile, 
                   IntegerArray & componentsPerFile, 
                   bool & closeShowAfterUse,
                   FILE *& outFile, aString & outputFileName, 
                   bool & timesMatch,
                   real & maxTimeDiff )
{
  const int myid=max(0,Communication_Manager::My_Process_Number);

  for( int i=0; i<numberOfFiles; i++ )
  {
    if( closeShowAfterUse )
    {
      int displayInfo=0; // do not print header info etc.
      showFileReader[i].open(fileName[i],displayInfo);
      if( i <= frameSeries.getBound(0) )
	showFileReader[i].setCurrentFrameSeries(frameSeries(i));

    }
    real timea=getCPU();
    if( solutionNumber[i]<=0 )
    {
      // solutionNumber[i] = -1 : means choose last solution
      const int numberOfSolutions = showFileReader[i].getNumberOfSolutions();
      solutionNumber[i]=numberOfSolutions;
      printF("INFO: Setting solution=%i (last in file) since solutionNumber[i]<=0\n",solutionNumber[i]);
    }
	  
    showFileReader[i].getASolution(solutionNumber[i],cg[i],u[i]);        // read in a grid and solution
    timea=getCPU()-timea; timea=ParallelUtility::getMaxValue(timea);
	
    if( numComponentsPerFile.getLength(0)==numberOfFiles )
    {
      // User has specified a subset of components to use per file
      // Make a new grid function with just these components. 
      int nc = numComponentsPerFile(i);
      Range all;
      realCompositeGridFunction v(cg[i],all,all,all,nc);
      for( int c=0; c<nc; c++ )
      {
	int cc=componentsPerFile(i,c);
	if( cc<u[i].getComponentBase(0) || cc>u[i].getComponentBound(0) )
	{
	  printF("ERROR: component chosen is out of bounds: file=%i component=%i valid=[%i,%i]\n"
		 "  Changing component to %i\n",
		 i,cc,u[i].getComponentBase(0),u[i].getComponentBound(0),u[i].getComponentBound(0));
	  cc=u[i].getComponentBound(0);
	  componentsPerFile(i,c)=cc;
	}
	printF(" Choosing component %i for file %i\n",cc,i);
	v.setName(u[i].getName(cc),c);
      }
	  
      for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
      {
        OV_GET_SERIAL_ARRAY(real,v[grid],vLocal);
        OV_GET_SERIAL_ARRAY(real,u[i][grid],uLocal);

	    
	for( int c=0; c<nc; c++ )
	{
	  const int cc=componentsPerFile(i,c);
	  assert( cc>=u[i].getComponentBase(0) && cc<=u[i].getComponentBound(0) );
	  vLocal(all,all,all,c)=uLocal(all,all,all,cc);

	}
	    
      }
      u[i].destroy();
      u[i].reference(v);
          
    }
	


    HDF_DataBase & db = *(showFileReader[i].getFrame());
    db.get(time(i),"time");  
    printF(" file[%i] solutionNumber=%i time=%20.12e (%8.2e(s) to read)\n",i,solutionNumber[i],time(i),timea);

    if( outFile==NULL )
    {
      if( myid==0 )
	outFile=fopen((const char*)outputFileName,"w" );
      printF("Output being saved in file %s\n",(const char*)outputFileName);

    }
	

    fPrintF(outFile,"Choosing solution %i, t=%9.3e,  from showFile=%s\n",solutionNumber[i],time(i),(const char*)fileName[i]);
	

    if( closeShowAfterUse )
      showFileReader[i].close();

    cg[i].update(MappedGrid::THEmask );
    // cg[i].update(MappedGrid::THEmask | MappedGrid::THEcenter | MappedGrid::THEvertex |
    //	     MappedGrid::THEinverseVertexDerivative);
	
  } // end for i<numberOfFiles
      

  // --------- check that times match -----------------------
  timesMatch=true;
  maxTimeDiff=0.;
  for( int i=1; i<numberOfFiles; i++ )
  {
    if( fabs(time(i)-time(i-1)) > REAL_EPSILON*max(fabs(time(i)),fabs(time(i-1)))*100. )
    {
      timesMatch=false;

      printF("***************ERROR: The times of the solutions do not match! ****************\n"
	     "   times=");
      for( int ii=0; ii<numberOfFiles; ii++ )
      {
	maxTimeDiff=max(maxTimeDiff,fabs(time(ii)-time(max(ii-1,0))));
	    
	printF("%12.6e, ",time(ii));
      }
          
      printF("\n ***** Max difference in times = %8.2e ****\n",maxTimeDiff);
      printF("\n *********************************************************************************\n");

      break;
    }
  }
  // -------------- end read solutions from all files --------
  return 0;
}
	

// ==================================================================================================
/// \brief Compute the difference between the fine grid and coarse grid -- difference lives on the coarse grid.
/// \param du[i] (output) :    du[i] = Interp(uFine) - uCoarse[i] 
// ==================================================================================================
int computeDifferences( int numberOfFiles, 
                        CompositeGrid *cg, 
                        realCompositeGridFunction *u,  
                        realCompositeGridFunction *ud,
                        int interpolationWidth,
                        bool useOldWay, bool useNewWay,
                        bool interpolateFromSameDomain )
{
  const int n =numberOfFiles-1;  // finest grid
  
  realCompositeGridFunction & vn = u[n];  // Here is the fine grid solution 
  for( int i=0; i<n; i++ )
  {


    const realCompositeGridFunction & v = u[i];  // here is the coarse grid solution
    ud[i].updateToMatchGridFunction(v);
    ud[i]=0.;
        
    printF("\n >> Interpolate the fine grid solution onto the grid from file %i...\n",i);

    if( interpolateFromSameDomain && cg[i].numberOfDomains()>1 && cg[n].numberOfDomains()==cg[i].numberOfDomains() )
    {
      // --- This is a multi-domain problem and we interpolate only from the same domain ----
      //  printF("--COMP-- computeDifferences:INFO: file %i: numberOfDomains=%i\n",i,cg[i].numberOfDomains());
      printF("--COMP-- computeDiff:INFO: This is a multi-domain solution numberOfDomains=%i, \n"
             "   Solutions will be interpolated from the same domain.\n",
              cg[i].numberOfDomains());

      // APPROACH:
      //   - Build new grid functions (GFs) that only live on a given domain
      //   - Copy data from master GFs to domain GFs 
      //   - Interpolate domain solutions
      //   - copy interpolated values back to master differenecs ud[i]
      cg[n].update(CompositeGrid::THEdomain);
      cg[i].update(CompositeGrid::THEdomain);

      if( true )
      {
	printF("INFO: File %i: Fine grid  : domainNumber=[",n);
	for( int grid=0; grid<cg[n].numberOfComponentGrids(); grid++ ){ printF("%i,", cg[n].domainNumber(grid)); }
	printF("]\n");

	printF("INFO: File %i: Coarse grid: domainNumber=[",i);
	for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ ){ printF("%i,", cg[i].domainNumber(grid)); }
	printF("]\n");
      }
      
      
      // ::display(cg[n].domainNumber(),"cg[n].domainNumber() (fine)");
      // ::display(cg[i].domainNumber(),"cg[i].domainNumber() (coarse)");
      // ::display(cg[n].gridNumber(),"cg[n].gridNumber()");


      Range all, C(ud[i].getComponentBase(0),ud[i].getComponentBound(0));
      // ----------------------------------------------------------------
      // -------------- Interpolate domain by domain --------------------
      // ----------------------------------------------------------------
      for( int domain=0; domain<cg[i].numberOfDomains(); domain++ )
      {
	CompositeGrid & cgFine   = cg[n].domain[domain];
	CompositeGrid & cgCoarse = cg[i].domain[domain];
	
	// compute the master grid number for FINE grid: 
        //   cg[n][masterGrid(grid)] = cgFine[grid]
	IntegerArray masterGrid(cgFine.numberOfComponentGrids());
	int k=0;
        for( int grid=0; grid<cg[n].numberOfComponentGrids(); grid++ )
	{
	  if( cg[n].domainNumber(grid)==domain )
	  {
            masterGrid(k)=grid; k++;   // this grid in master collection is in the current domain
	  }
	}
        assert( k==cgFine.numberOfComponentGrids() );

        // printF("---domain=%i\n",domain);
	// ::display(masterGrid,"masterGrid for cg[n] (fine)");

	// ::display(cgFine.domainNumber(),"cgFine.domainNumber()");
        // ::display(cgFine.gridNumber(),"cgFine.gridNumber()");
        // ::display(cgFine.baseGridNumber(),"cgFine.baseGridNumber()");
        // ::display(cgFine.componentGridNumber(),"cgFine.componentGridNumber()");

        realCompositeGridFunction uFine(cgFine,all,all,all,C);     uFine=0.;
        realCompositeGridFunction uCoarse(cgCoarse,all,all,all,C); uCoarse=0.; 
	
        for( int grid=0; grid<cgFine.numberOfComponentGrids(); grid++ )
	{
          uFine[grid]=vn[masterGrid(grid)];  // copy fine grid solution from master to domain
	}
	

	printF("\n +++++++++++ domain=[%s] InterpolatePointsOnAGrid, interpolationWidth=%i  +++++++++++++\n\n",
	       (const char*)cg[n].getDomainName(domain),interpolationWidth );
	InterpolatePointsOnAGrid interpolator;
	interpolator.setInfoLevel( 1 );
	interpolator.setInterpolationWidth(interpolationWidth);
	// Set the number of valid ghost points that can be used when interpolating from a grid function: 
	int numGhostToUse=1;
	interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
	// Assign all points, extrapolate pts if necessary:
	interpolator.setAssignAllPoints(true);

	int numGhost=0;  // no need to interpolate ghost points

	real time0=getCPU();
	int num=interpolator.interpolateAllPoints( uFine,uCoarse,C,C,numGhost);    // interpolate uCoarse from fine
	real time=getCPU()-time0;
	time=ParallelUtility::getMaxValue(time);
	printF(" ... time to interpolate = %8.2e(s)\n",time);

	// compute the master grid number for COARSE grid: 
        //   cg[i][masterGrid(grid)] = cgCoarse[grid]
        masterGrid.redim(cgCoarse.numberOfComponentGrids());
	k=0;
        for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
	{
	  if( cg[i].domainNumber(grid)==domain )
	  {
            masterGrid(k)=grid; k++;   // this grid in master collection is in the current domain
	  }
	}
        assert( k==cgCoarse.numberOfComponentGrids() );

	// ::display(masterGrid,"masterGrid for cg[i] (coarse)");

        for( int grid=0; grid<cgCoarse.numberOfComponentGrids(); grid++ )
	{
          ud[i][masterGrid(grid)]=uCoarse[grid];  // copy interpolated values into ud[i] on master
	}

      } // end fo domain
      

    }
    // This next call can be used to call the old or new method
    else if( useOldWay )
    {
      printF("\n +++++++++++++++++++++ USE OLD INTERP ++++++++++++++++++++++\n\n");
      bool useNewWay=false;
      real time0=getCPU();
      interpolateAllPoints( vn,ud[i],useNewWay );  // interpolate ud[i] from fine grid solution vn
      real time=getCPU()-time0;
      time=ParallelUtility::getMaxValue(time);
      printF(" ... time to interpolate = %8.2e(s)\n",time);
    }
    else if( useNewWay )
    {
      // *new way*
      printF("\n +++++++++++++++++++++ USE NEW INTERP ++++++++++++++++++++++\n\n");
      InterpolatePoints interpPoints; 
      interpPoints.setInfoLevel( 1 );
      int numGhost=0;  // no need to interpolate ghost points
      Range C(ud[i].getComponentBase(0),ud[i].getComponentBound(0));
      real time0=getCPU();
      interpPoints.interpolateAllPoints( vn,ud[i],C,C,numGhost );  // interpolate ud[i] from fine grid solution vn
      real time=getCPU()-time0;
      time=ParallelUtility::getMaxValue(time);
      printF(" ... time to interpolate = %8.2e(s)\n",time);
    }
    else
    {
      // *newer way* 091126 
      printF("\n +++++++++++ USE InterpolatePointsOnAGrid, interpolationWidth=%i  +++++++++++++\n\n",
	     interpolationWidth );
      InterpolatePointsOnAGrid interpolator;
      interpolator.setInfoLevel( 1 );
      interpolator.setInterpolationWidth(interpolationWidth);
      // Set the number of valid ghost points that can be used when interpolating from a grid function: 
      int numGhostToUse=1;
      interpolator.setNumberOfValidGhostPoints( numGhostToUse );
      
      // Assign all points, extrapolate pts if necessary:
      interpolator.setAssignAllPoints(true);

      int numGhost=0;  // no need to interpolate ghost points
      Range C(ud[i].getComponentBase(0),ud[i].getComponentBound(0));

      real time0=getCPU();
      int num=interpolator.interpolateAllPoints( vn,ud[i],C,C,numGhost);    // interpolate ud from vn
      real time=getCPU()-time0;
      time=ParallelUtility::getMaxValue(time);
      printF(" ... time to interpolate = %8.2e(s)\n",time);
    }
	
	
    // ud[i] = u(coarse) - u(fine)
    // ud[i]-=v;
    for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
    {
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(ud[i][grid],uLocal);
      realSerialArray vLocal; getLocalArrayWithGhostBoundaries(    v[grid],vLocal);

      // uLocal-=vLocal;  // *wdh* 110729 - make ud = coarse - fine
      uLocal=vLocal-uLocal;
    }
	

  } // end for( int i=0; i<n; i++ )
  
  return 0;
}


// =============================================================================================
// ============================= COMP MAIN =====================================================
// =============================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();

  if( false )
  {
    // --- test computeRate
    int n=2;
    RealArray h(n+1),u(n+1),e(n+1);
    real c0=.234, s0=2.2;
    for( int i=0; i<n+1; i++ )
    {
      h(i)=1./pow(2.,double(i+1));
      u(i)=1. + c0*pow(h(i),s0);  // our solution
    }
    for( int i=0; i<n; i++ )
      e(i)=u(i)-u(n);
    
    real sigma,c;
    computeRate(n,h,e,sigma,c);
    printf(" sigma=%e (true=%e), c=%e (true=%e) \n",sigma,s0,c,c0);
    return 0;
  }

  bool plotOption=true;
  bool closeShowAfterUse=true;
  bool useOldWay=false;
  bool useNewWay=false;

  aString commandFileName="";
  if( argc > 1 )
  { // look at arguments for "-noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false;
      else if( line=="-useOld" )
        useOldWay=true;
      else if( line=="-useNew" )
        useNewWay=true;
      else if( commandFileName=="" )
      {
        commandFileName=line;    
        printf("comp: reading commands from file [%s]\n",(const char*)commandFileName);
      }
      
    }
  }

  const int maxNumberOfFiles=15;

  int numberOfFiles=0;
  aString fileName[maxNumberOfFiles];

  ShowFileReader showFileReader[maxNumberOfFiles];
  int numberOfSolutions[maxNumberOfFiles];
  int solutionNumber[maxNumberOfFiles];
  int maxSolution[maxNumberOfFiles];
  for( int i=0; i<maxNumberOfFiles; i++ )
  {
    solutionNumber[i]=-1;
    maxSolution[i]=INT_MAX;   // the largest number of solutions common to all files
  }
  
  CompositeGrid cg[maxNumberOfFiles];
  realCompositeGridFunction u[maxNumberOfFiles], ud[maxNumberOfFiles];
  RealArray h(maxNumberOfFiles);
  const int maxNumberOfComponents=20;
  RealArray time(maxNumberOfFiles);

  RealArray maxDiff(maxNumberOfFiles,maxNumberOfComponents);
  RealArray l2Diff(maxNumberOfFiles,maxNumberOfComponents);
  RealArray l1Diff(maxNumberOfFiles,maxNumberOfComponents);

  // sigmaRate(c,norm) : convergence rate for a component c and norm
  RealArray sigmaRate; 
  int interpolationWidth=3;  // width for interpolation formula

  int extra=0;  // set to -1 to not check boundary

  // PlotStuff ps(plotOption,"comp");                      // create a PlotStuff object
  GenericGraphicsInterface & ps = *Overture::getGraphicsInterface("comp",plotOption,argc,argv);

  GenericGraphicsInterface & gi = ps;
  
  PlotStuffParameters psp;           // create an object that is used to pass parameters
    
  // By default start saving the command file called "comp.cmd"
  aString logFile="comp.cmd";
  ps.saveCommandFile(logFile);
  printF("User commands are being saved in the file `%s'\n",(const char *)logFile);

  aString outputFileName="comp.log";
  FILE *outFile = NULL;
  
  aString outputShowFile = "comp.show";

  // read from a command file if given
  if( commandFileName!="" )
  {
    printF("read command file =%s\n",(const char*)commandFileName);
    ps.readCommandFile(commandFileName);
  }

  bool errorsComputed=false;
  bool timesMatch=true;
  real maxTimeDiff=0.;
  // By default we do NOT assume the fine grid holds the exact solution:
  bool assumeFineGridHoldsExactSolution=false; 

  // By default interpolate grids from the same domain only 
  bool interpolateFromSameDomain=true;
  
  // We can define components to use from each file
  IntegerArray numComponentsPerFile, componentsPerFile;

  // Keep track of which frame series (i.e. domain) to use from each show file
  IntegerArray frameSeries;

  // we can define vectors of components for computing errors such as the norm of the velocity components.
  std::vector<ComponentVector> componentVector;


  // ---------------- Build the GUI -------------------
  GUIState dialog;
  dialog.setWindowTitle("compare show files");
  dialog.setExitCommand("exit", "exit");

  aString cmds[] = {"specify files",
                    "choose a solution",
                    "choose a solution for each file",
		    "compute errors",
                    "plot solutions",
                    "plot differences",
                    "plot errors (max-norm rate)",
		    "plot errors (l1-norm rate)",
		    "plot errors (l2-norm rate)",
                    "save differences to show file",
		    "" };
  int numberOfPushButtons=0;  // number of entries in cmds
  while( cmds[numberOfPushButtons]!="" ){numberOfPushButtons++;}; // 
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( cmds, cmds, numRows ); 

  aString tbCommands[] = {"assume fine grid holds exact solution",
                          "interpolate from same domain",
                          ""};
  int tbState[10];
  tbState[0] = assumeFineGridHoldsExactSolution;
  tbState[1] = interpolateFromSameDomain;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=15;  // max number allowed
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "output file name:";  sPrintF(textStrings[nt],"%s",(const char*)outputFileName);  nt++; 
  textLabels[nt] = "interpolation width:";  sPrintF(textStrings[nt],"%i",interpolationWidth);  nt++; 
  textLabels[nt] = "output show file:";  sPrintF(textStrings[nt],"%s",(const char*)outputShowFile);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  aString answer,answer2;
  aString menu[] = {// "specify files (coarse to fine)",
                    // "choose a solution",
                    // "choose a solution for each file",
                    // "compute errors",
                    // "plot solutions",
                    // "plot differences",
                    // "plot errors (max-norm rate)",
		    // "plot errors (l1-norm rate)",
		    // "plot errors (l2-norm rate)",
                    // "save differences to show files",
                    "define a vector component",
                    "delete all vector components",
                    // "interpolation width",
                    "enter components to use per file",
                    // "output file name",
                    "choose a frame series (domain) per file",
                    // "assume fine grid holds exact solution",
                    // "do not assume fine grid holds exact solution",
                    // "do not check boundaries (toggle)",
                    // "output ascii files",
                    // "output binary files",
		    "exit",
                    "" };

  dialog.buildPopup(menu);

  dialog.addInfoLabel("See popup menu for more options.");

  gi.pushGUI(dialog);
  gi.appendToTheDefaultPrompt("comp>");


  char buff[80];
  
  for(;;)
  {
    // ps.getMenuItem(menu,answer);
    gi.getAnswer(answer,"");  

    if( answer=="specify files" ||
        answer=="specify files (coarse to fine)" )
    {
      printF("Specify names of show files (normally coarse grid to fine)\n");
      for( int i=0; i<maxNumberOfFiles; i++ )
      {
	ps.inputString(fileName[numberOfFiles],"Enter the file name (`exit' to finish)");
	if( fileName[numberOfFiles]=="" || fileName[numberOfFiles]=="exit" )
          break;
	showFileReader[numberOfFiles].open(fileName[numberOfFiles]);
	numberOfSolutions[numberOfFiles]=showFileReader[numberOfFiles].getNumberOfFrames();
	maxSolution[numberOfFiles]=min(maxSolution[numberOfFiles],numberOfSolutions[numberOfFiles]);
 
	if( closeShowAfterUse )
	  showFileReader[i].close();


	numberOfFiles++;
      }
    }
    else if( dialog.getTextValue(answer,"output file name:","%s",outputFileName) ){}  //
    else if( dialog.getTextValue(answer,"output show file:","%s",outputShowFile) ){}  //
    else if( dialog.getTextValue(answer,"interpolation width:","%i",interpolationWidth) )
    {
      printF("Setting the interpolation width to %i\n",interpolationWidth);
    }

    else if( dialog.getToggleValue(answer,"assume fine grid holds exact solution",assumeFineGridHoldsExactSolution) )
    {
      if( assumeFineGridHoldsExactSolution )
      {
	printF("Assume the fine grid holds the exact solution when computing convergence rates.\n");
      }
      else
      {
	printF("Do not assume the fine grid holds the exact solution when computing convergence rates.\n");
      }
      
    }
    
    else if( dialog.getToggleValue(answer,"interpolate from same domain",interpolateFromSameDomain) )
    {
      if( interpolateFromSameDomain )
      {
	printF("interpolateFromSameDomain=true : For multi-domain problems only interpolate from grids"
               " on the same domain.\n");
      }
      else
      {
	printF("interpolateFromSameDomain=true : For multi-domain problems allow interpolation from all domains.\n");
      }
      
    }
    

    // --- commands done the old way before dialog ---

    else if( answer=="choose a frame series (domain) per file" )
    {
      if( numberOfFiles<=0 )
      {
	printF("You should choose files first.\n");
	continue;
      }      
      aString line;
      frameSeries.redim(numberOfFiles);
      frameSeries=0;
      
      for( int i=0; i<numberOfFiles; i++ )
      {
        int numberOfFrameSeries=max(1,showFileReader[i].getNumberOfFrameSeries());
        printF(" File: %i has %i frame series (domains):\n",i,numberOfFrameSeries);
        for( int fs=0; fs<numberOfFrameSeries; fs++ )
	{
          printF(" frame series %i: %s\n",fs,(const char*)showFileReader[i].getFrameSeriesName(fs));
	}
	
	ps.inputString(line,sPrintF(buff,"Enter the number of the frame series to use, (0,...,%i).",numberOfFrameSeries-1));
        int fs=-1;
	sScanF(line,"%i",&fs);
	if( fs<0 || fs>numberOfFrameSeries )
	{
	  printF("ERROR: frame series %i is NOT valid. Will use 0\n",fs);
	  fs=0;
	}
        frameSeries(i)=fs;  // save
	showFileReader[i].setCurrentFrameSeries(fs);
      }
      
    }
    else if( answer=="enter components to use per file" )
    {
      if( numberOfFiles<=0 )
      {
	printF("You should choose files first\n");
	continue;
      }
      
      numComponentsPerFile.redim(numberOfFiles);
      componentsPerFile.redim(numberOfFiles,maxNumberOfComponents);
      componentsPerFile=-1;
      aString line;
      for( int i=0; i<numberOfFiles; i++ ) 
      {
        numComponentsPerFile(i)=0;
        int c[15];
	for( int j=0; j<15; j++ ) c[j]=-1;
	ps.inputString(line,sPrintF("Enter a list of components to use for file %i",i));
	sScanF(line,"%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
               &c[0],&c[1],&c[2],&c[3],&c[4],
               &c[5],&c[6],&c[7],&c[8],&c[9],
               &c[10],&c[11],&c[12],&c[13],&c[14]);
        for( int j=0; j<15; j++ )
	{
	  if( c[j]>=0 )
	  {
            numComponentsPerFile(i)++;
            componentsPerFile(i,j)=c[j];
	  }
	  else
	  {
	    break;
	  }
	}
      }
    }
    else if( answer=="choose a solution" ||
             answer=="choose a solution for each file" )
    {
      // In this case the user is asked to choose a solution to read in
      // Choosing a number that is too large will cause the last solution to be read 

      aString line;
      if( answer=="choose a solution" )
      {
	ps.inputString(line,sPrintF(buff,"Enter the solution number to read, in [1,%i] (-1=choose last) \n",maxSolution[0]));
	sScanF(line,"%i",&solutionNumber[0]);
        for( int i=1; i<maxNumberOfFiles; i++ ) solutionNumber[i]=solutionNumber[0];
      }
      else
      {
        for( int i=0; i<numberOfFiles; i++ ) 
          printF(" File %i has solutions [1,%i]\n",i,maxSolution[i]);
	ps.inputString(line,"Enter separate solution number of each file\n");
	assert( maxNumberOfFiles<15 );
	sScanF(line,"%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i",
               &solutionNumber[0],&solutionNumber[1],&solutionNumber[2],&solutionNumber[3],&solutionNumber[4],
               &solutionNumber[5],&solutionNumber[6],&solutionNumber[7],&solutionNumber[8],&solutionNumber[9],
               &solutionNumber[10],&solutionNumber[11],&solutionNumber[12],&solutionNumber[13],&solutionNumber[14]);
      }
      

      // -------------- read a solution from all files ---------------------
      readSolutions( numberOfFiles, cg, u, showFileReader, solutionNumber, frameSeries, fileName, time,
		     numComponentsPerFile, componentsPerFile, closeShowAfterUse, outFile, outputFileName, 
		     timesMatch, maxTimeDiff );

    }
    else if( answer=="assume fine grid holds exact solution" ||
             answer=="do not assume fine grid holds exact solution" )
    {
      assumeFineGridHoldsExactSolution=answer=="assume fine grid holds exact solution";
      printF("INFO: you should recompute the errors: assumeFineGridHoldsExactSolution=%i.\n",
             (int)assumeFineGridHoldsExactSolution);
    }
    else if( answer=="output file name" )
    {
      ps.inputString(outputFileName,"Enter the name of the output file (default is comp.log)");
      printF("Output file will be named [%s]\n",(const char*)outputFileName);
    }
    else if( answer=="interpolation width" )
    {
      ps.inputString(answer,sPrintF("Enter the interpolation width (2,3,4,...), (current=%i)",interpolationWidth));
      sScanF(answer,"%i",&interpolationWidth);
      printF("Setting interpolationWidth=%i\n",interpolationWidth);
    }
    else if( answer=="define a vector component" )
    {
      printF("Define a vector of components for which errors are estimated (e.g. the velocity vector)\n");
      ComponentVector v;
      ps.inputString(v.name,"Enter the name of the vector");
      IntegerArray c;
      int cmin=0; // minimum component number
      int nvc=ps.getValues("Enter the component numbers (enter `done' when finished)",c,cmin);
      for( int i=0; i<nvc; i++ )
      {
	v.component.push_back(c(i));
      }
      componentVector.push_back(v);
      printF(" Vector %s is defined from components ",(const char*)v.name);
      for( int i=0; i<v.component.size(); i++ ) printF("%i, ",v.component[i]);
      printF("\n");
      
    }
    else if( answer=="delete all vector components" )
    {
      componentVector.clear();
    }
    else if( answer=="output ascii files" )
    {
      aString outputFileName;
      ps.inputString(outputFileName,"Enter the output file name ");
      FILE *file;
      for( int i=0; i<numberOfFiles; i++ )
      {
        for( int solution=1; solution<=maxSolution[i]; solution++ )
	{
	  showFileReader[i].getASolution(solution,cg[i],u[i]);        // read in a grid and solution
	  HDF_DataBase & db = *(showFileReader[i].getFrame());
	  db.get(time(i),"time");   // *wdh* 091109 -- change "t" to "time"
	  

	  MappedGrid & mg = cg[i][0];
	  realMappedGridFunction & v = u[i][0];
	
	  const IntegerArray & gridIndexRange = cg[i][0].gridIndexRange();
          if( solution==1 )
	  {
  	    int n=gridIndexRange(End,axis1)-gridIndexRange(Start,axis1);
	    aString name;
	    name=sPrintF(buff,"%s%i.dat",(const char*)outputFileName,n);
	    printf("save file %s \n",(const char*)name);
	    file = fopen((const char*)name,"w");
	  }
	  DisplayParameters dp;
	  dp.set(file);
	  dp.set(DisplayParameters::labelNoIndicies);
	  Index I1,I2,I3;
	  getIndex(cg[i][0].gridIndexRange(),I1,I2,I3);
	  for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	  {
	    cout << "component Name = [" << u[i].getName(c) << "]\n";
	  
	    fprintf(file,"%s\n",(const char*)u[i].getName(c));
            fprintf(file,"%e (time)\n",time(i));
	    fprintf(file,"%i %i %i\n",I1.getBound()-I1.getBase()+1,
		    I2.getBound()-I2.getBase()+1,
		    I3.getBound()-I3.getBase()+1);
	    display(u[i][0](I1,I2,I3,c),NULL,dp);
	  }
	}
	fclose(file);
      }
    }
    else if( answer=="compute errors" )
    {
      if( solutionNumber[0]<0 )
      {
	printf(" You should `choose a solution' before computing errors\n");
	continue;
      }

      if( assumeFineGridHoldsExactSolution )
	printF("NOTE: Computing convergence rates assuming the fine grid holds the exact solution.\n");

      errorsComputed=true;
      

      Index I1,I2,I3, J1,J2,J3;
      const int n =numberOfFiles-1;  // finest grid

      // **** determine relative mesh spacings here ***
      // Assume the base grids are basically the same but with different numbers of grid points.
      // There may also be different numbers of refinement levels.
      for( int i=0; i<numberOfFiles; i++ )
      {
        h(i)=cg[i][0].gridSpacing(axis1);    // grid spacing on grid 0
	if( cg[i].numberOfRefinementLevels()>1 )
	{
	  int nl=cg[i].numberOfRefinementLevels();
	  int rf=cg[i].refinementLevel[nl-1].refinementFactor(0,0);
	  // NOTE: rf is the refinement factor to the base grid.
	  // printf(" numberOfLevels=%i refinementFactor=%i\n",nl,rf);
	  h(i)/=rf;
	}
      }
      
      for( int io=0; io<=1; io++ )
      {
	FILE *file = io==0 ? stdout : outFile;
        for( int i=0; i<numberOfFiles; i++ )
	  fPrintF(file," File %i, solution=%i, t=%9.3e, grid 0: dr = %10.3e , ratio to fine grid = %8.4f\n",
		  i,solutionNumber[i],time(i),h(i),h(i)/h(n));
      }
      fflush(outFile);

      // ----------------- Compute differences -------------
      //    du[i] = Interp(uFine) - uCoarse[i] 
      computeDifferences( numberOfFiles, cg, u, ud, interpolationWidth, useOldWay, useNewWay,interpolateFromSameDomain );
      
      for( int i=0; i<n; i++ )
      {
   
      	const realCompositeGridFunction & v = u[i];  // here is the coarse grid solution
	const int useAreaWeightedNorm=1;
        for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	{
	  l2Diff(i,c) = lpNorm(2,ud[i],c,0,0,useAreaWeightedNorm);
	  l1Diff(i,c) = lpNorm(1,ud[i],c,0,0,useAreaWeightedNorm);
	  maxDiff(i,c)=maxNorm(ud[i],c,0,0);
	}
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"h(%i)=%e, h(%i)=%e: \n",i,h(i),i+1,h(i+1));
	  for( int c=v.getComponentBase(0); c<=v.getComponentBound(0); c++ )
	    fPrintF(file,"coarse=%i : ud = coarse -fine: component=%s : maxDiff(%i)=%e, l2Diff(%i)=%e , l1Diff(%i)=%e \n",
		    i,(const char*)u[0].getName(c),c,maxDiff(i,c),c,l2Diff(i,c),c,l1Diff(i,c));
	}
	fflush(outFile);
      } // end for i 
      

      // these next arrays are used in printing the latex table.
      std::vector<aString> gridName;
      std::vector<aString> cName;

      // -- here are the components we put in the latex table ---

      int nvc = componentVector.size();  // vector components

      int ncu=u[0].getComponentBound(0)-u[0].getComponentBase(0)+1;
      // if there are vector components we do NOT save scalar components in the LaTeX file: (make an option)
      if( nvc > 0 )
	ncu =0;      
      
      const int nc = ncu+nvc;

      RealArray cErr(numberOfFiles,nc); cErr=0.;
      RealArray cSigma(nc); cSigma=0.;
      for( int i=0; i<numberOfFiles; i++ )
      {
	if( i==n && assumeFineGridHoldsExactSolution )
	  continue;  // do not include fine grid in table as the errors are zero in this case
	gridName.push_back(fileName[i]);
      }
      if( ncu>0 )
      {
	for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
	{
	  aString componentName = u[0].getName(c);
	  if( componentName=="" )
            componentName=sPrintF("component%i",c);
	  cName.push_back(componentName);
	}
      }
      for( int c=0; c<componentVector.size(); c++ ) // c = vector 
      {
	ComponentVector & v = componentVector[c];
	cName.push_back(v.name);
      }
	    

      // -- estimate convergence rates---
      if( n>=2 )
      {
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"\n Solutions at times=");
	  for( int i=0; i<numberOfFiles; i++ )
	  {
	    fPrintF(file,"%12.6e, ",time(i));
	  }
	  fPrintF(file,"\n");
	}
	
	Range C(u[0].getComponentBase(0),u[0].getComponentBound(0));
	const int numberOfNorms=3;
        sigmaRate.redim(C,numberOfNorms);
	for( int norm=0; norm<numberOfNorms; norm++ )
	{
	  for( int io=0; io<=1; io++ )
	  {
	    FILE *file = io==0 ? stdout : outFile;
	    if( norm==0 )
	      fPrintF(file,"++++++++++++++ max norm results +++++++++++++\n");
	    else if( norm==1 )
	      fPrintF(file,"++++++++++++++ l2 norm results +++++++++++++\n");
	    else 
	      fPrintF(file,"++++++++++++++ l1 norm results +++++++++++++\n");
	    fPrintF(file,"    ee = estimated error from C*h^{rate}     \n");
	  }
	
	  const RealArray & diff = norm==0 ? maxDiff : norm==1 ? l2Diff : l1Diff;
	  real sigma,cc;
	  Range R(0,n);
	  for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
	  {
	    computeRate(n,h,diff(R,c),sigma,cc, assumeFineGridHoldsExactSolution);

            sigmaRate(c,norm)=sigma;  // save for plotting errors

	    for( int io=0; io<=1; io++ )
	    {
	      FILE *file = io==0 ? stdout : outFile;

	      fPrintF(file," component=%i, %11s, rate=%5.2f, ",c,(const char*)u[0].getName(c),sigma);
	      for( int i=0; i<=n; i++ )
	      {
                // estimated errors:
                if( i==n && assumeFineGridHoldsExactSolution )
                  continue;
		fPrintF(file,"ee(%i) = %8.2e, ",i,cc*pow(h(i),sigma));
		if( i>0 ) fPrintF(file,"[r=%5.2f], ",pow(h(i-1)/h(i),sigma));

                if( ncu>0 ){ cErr(i,c)=cc*pow(h(i),sigma); cSigma(c)=sigma; }// save for latex
	      }
	      fPrintF(file,"\n");
	    }

	  }
          // --- estimate errors in the vector components ---
	  if( componentVector.size()>0 )
	  {
	    for( int c=0; c<componentVector.size(); c++ ) // c = vector 
	    {
	      ComponentVector & v = componentVector[c];


	      RealArray vdiff(n); // holds diff's for the vector 
	      vdiff=0.;
	      for( int j=0; j<v.component.size(); j++ ) // loop over components of the vector 
	      {
		const int cv = v.component[j];  
		if( cv<u[0].getComponentBase(0) || cv>u[0].getComponentBound(0) )
		{
		  printF("comp::ERROR: vector %i (%s) has an invalid component number = %i. Will ignore.\n",
                          c,(const char*)v.name,cv);
		  continue;
		}
		for( int i=0; i<n; i++ ) // i : solution number 
		{
		  if( norm==0 )
		    vdiff(i) = max( vdiff(i), maxDiff(i,cv) );
		  else if( norm==1 )
		    vdiff(i) += SQR( l2Diff(i,cv) );
		  else
		    vdiff(i) += fabs( l1Diff(i,cv) );
		}
	      }
	      if( norm==1 )
                vdiff=sqrt(vdiff); // l2 norm
	      if( norm==1 || norm==2 )
                vdiff/= v.component.size();  // average l1 or l2 norm per component in the vector 
	      
	      computeRate(n,h,vdiff,sigma,cc, assumeFineGridHoldsExactSolution);

	      for( int io=0; io<=1; io++ )
	      {
		FILE *file = io==0 ? stdout : outFile;

		fPrintF(file," Vector %s is defined from components ",(const char*)v.name);
		for( int i=0; i<v.component.size(); i++ ) fPrintF(file,"%i, ",v.component[i]);
		fPrintF(file,"\n");

		fPrintF(file," vector comp.  %11s, rate=%5.2f, ",(const char*)v.name,sigma);
		for( int i=0; i<=n; i++ )
		{ 
                  // --- estimated errors ---
		  if( i==n && assumeFineGridHoldsExactSolution )
		    continue;
		  fPrintF(file,"ee(%i) = %8.2e, ",i,cc*pow(h(i),sigma));
		  if( i>0 ) fPrintF(file,"[r=%5.2f], ",pow(h(i-1)/h(i),sigma));

		  cErr(i,c+ncu)=cc*pow(h(i),sigma); cSigma(c+ncu)=sigma; // save for latex

		}
		fPrintF(file,"\n");
	      }


	    } // end for c
	  }
	  
          // --- Now output results in the format of a LaTeX table ---
	  for( int io=0; io<=1; io++ )
	  {
	    FILE *file = io==0 ? stdout : outFile;
	    outputLatexTable( gridName, cName, cErr, cSigma, time, norm, file, assumeFineGridHoldsExactSolution );
	  }


	}  // end for norm 
	fflush(outFile);
	printF("Output written to file %s\n",(const char*)outputFileName);
	
      }
      if( !timesMatch )
      {
	for( int io=0; io<=1; io++ )
	{
	  FILE *file = io==0 ? stdout : outFile;
	  fPrintF(file,"\n***************WARNING: The times of the solutions do not match! ****************\n"
		 "   times=");
	  for( int ii=0; ii<numberOfFiles; ii++ )
	  {
	    fPrintF(file,"%12.6e, ",time(ii));
	  }
	  fPrintF(file,"\n ***** Max difference in times = %8.2e ****\n",maxTimeDiff);
	  fPrintF(file,"\n *********************************************************************************\n");
	}
      }
      
    }
    else if( answer=="plot solutions" )
    {
      for( int i=0; i<numberOfFiles; i++ )
      {
	psp.set(GI_TOP_LABEL,sPrintF(buff,"u[%i] t=%9.3e",i,time(i)));
        ps.erase();
	PlotIt::contour(ps,u[i],psp);
      }
    }
    else if( answer=="plot differences" )
    {
      if( !errorsComputed )
      {
	printF("You should compute the errors before you can plot the differences\n");
	continue;
      }

      for( int i=0; i<numberOfFiles-1; i++ )
      {
	psp.set(GI_TOP_LABEL,sPrintF(buff,"u[%i]-u[%i] t=%9.3e, %9.3e",i,numberOfFiles-1,time(i),time(numberOfFiles-1)));
        ps.erase();
	PlotIt::contour(ps,ud[i],psp);
      }
    }
    else if( answer=="plot errors (max-norm rate)" ||
             answer=="plot errors (l1-norm rate)"  ||
             answer=="plot errors (l2-norm rate)" )
    {
      // we assume: 
      //   um(i) - ue = C_i hm^p
      //   un(i) - ue = C_i hn^p 
      // This implies
      //   (um(i) - ue)/(un(i) - ue) = (hm/hn)^p == r
      //   (um(i) - ue)= r*(un(i) - ue)
      //   (r-1)*ue = r*un -um
      //         ue = (r*un-um)/(r-1)
      //   em = um(i) - ue = um - (r*un-um)/(r-1) = (um-un)*r/(r-1)
      //   en = un(i) - ue = un - (r*un-um)/(r-1) = (um-un)/(r-1)
      //      C_i = (um -ue)/hm^p 
      //          = (um-un)/(r-1)/hn^p 
      //
      if( !errorsComputed )
      {
	printF("You should compute the errors before you can plot the errors.\n");
	continue;
      }

      const int norm = answer=="plot errors (max-norm rate)" ? 0 : answer=="plot errors (l2-norm rate)" ? 1 : 2;
      aString normName = (norm==0 ? "max-norm" : norm==1 ? "l2-norm" : "l1-norm");

      printF("INFO: We assume that that error is of the form \n"
             "    um(i) - ue = C_i hm^sigma        (coarse grid)\n"
             "    un(i) - ue = C_i hn^sigma        (fine grid)\n"
             " which implies: \n"
             "    ue = (r*un-um)/(r-1),  r=(hm/hn)^sigma  \n"
             "The estimated errors are thus \n"
             " em = um(i) - ue = um - (r*un-um)/(r-1) = (um-un)*r/(r-1)    (m<n)\n"
             " en = un(i) - ue = un - (r*un-um)/(r-1) = (um-un)/(r-1)      (n=fine grid)\n\n");

      // Fix this for multiple components
      // Use which norm??

      printF("Plotting the estimated error based on the %s rates: \n",(const char*)normName);
      for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
      {
	printF(" Component %i (%s) : sigma=%8.2e\n",c,(const char*)u[0].getName(c),sigmaRate(c,norm));
      }

      // --- loop over files from coarse to fine ---
      for( int i=0; i<numberOfFiles; i++ )
      {
        const int n=numberOfFiles-1;  // fine grid 
	if( i==n && assumeFineGridHoldsExactSolution ) 
          continue;   // errors would be zero on the fine grid
	
        realCompositeGridFunction estErr;
        const int ie = i<n ? i : i-1;
        estErr.updateToMatchGridFunction(ud[ie]);
	Range all;
	for( int grid=0; grid<cg[i].numberOfComponentGrids(); grid++ )
	{
          realSerialArray udLocal;     getLocalArrayWithGhostBoundaries(ud[ie][grid],udLocal);          
          realSerialArray estErrLocal; getLocalArrayWithGhostBoundaries(estErr[grid],estErrLocal);          

	  for( int c=u[0].getComponentBase(0); c<=u[0].getComponentBound(0); c++ )
	  {
	    const real sigma = sigmaRate(c,norm);
	    const real r = pow(h(ie)/h(n),sigma);
	    real factor = i<n ? r/(r-1.) : 1./(r-1);
            if( assumeFineGridHoldsExactSolution ) 
              factor=1.;  // if fine grid holds the exact solution then the error is just the difference
	    //  estErr=ud[ie]*factor;
	    estErrLocal(all,all,all,c)=udLocal(all,all,all,c)*factor;
	  }
	}
	
	psp.set(GI_TOP_LABEL,sPrintF(buff,"estErr[%i] (%s) t=%9.3e, %9.3e",i,
				     (const char*)normName,time(i),time(numberOfFiles-1)));
	
        ps.erase();
	PlotIt::contour(ps,estErr,psp);
      }
      
    }
    
    else if( answer=="output differences to show file" )
    {
      printF("*** FINISH ME ****\n");

    }
    
    else if( answer=="save differences to show file" )  // *new way* July 2, 2016
    {
      // --- save differences over time to a show file --
      printF(" Save the difference between the fine grid and next finest grid into a show file\n");

      printF("Saving differences in the show file: [%s]\n",(const char*)outputShowFile);
      Ogshow show(outputShowFile);
      show.saveGeneralComment("Difference computed with comp");

      int nFine = numberOfFiles-1;  // fine grid 
      for( int solution=1; solution<=maxSolution[nFine]; solution++ )
      {
      
        for( int i=0; i<maxNumberOfFiles; i++ ) solutionNumber[i]=solution;

	// -------------- read a solution from all files ---------------------
	printF("Read solution %i...\n",solution);

	readSolutions( numberOfFiles, cg, u, showFileReader, solutionNumber, frameSeries, fileName, time,
		       numComponentsPerFile, componentsPerFile, closeShowAfterUse, outFile, outputFileName, 
		       timesMatch, maxTimeDiff );

	// ----------------- Compute differences -------------
	//    du[i] = Interp(uFine) - uCoarse[i] 
	computeDifferences( numberOfFiles, cg, u, ud, interpolationWidth, useOldWay, useNewWay, interpolateFromSameDomain );


        printF("Saving solution %i to show file: t=%16.10e\n",solution,time(0));

	show.startFrame();                                         // start a new frame

	char buffer[80]; 
	aString showFileTitle[5];

        nt=0;
	showFileTitle[0]=sPrintF(buffer,"(diff) t=%9.3e",time(0)); nt++;
	// showFileTitle[1]=sPrintF(buffer,"t=%4.3f, dt=%8.2e",t,dt);
	showFileTitle[nt]="";  // marks end of titles

	for( int i=0; showFileTitle[i]!=""; i++ )
	  show.saveComment(i,showFileTitle[i]);


	// show.saveComment(0,sPrintF("u[%i]-u[%i]",i,numberOfFiles-1));  
	// show.saveComment(1,sPrintF(" t=%16.10e ",time(i)));               
        int iSave=nFine-1;   // save difference between fine and next finest 
	show.saveSolution( ud[iSave] ); 

      }
      printF("...done saving to diff-show file [%s]\n",(const char*)outputShowFile);
    }
    


    else if( answer=="save differences to show files" )  // *** OLD WAY ***
    {
      if( !errorsComputed )
      {
	printF("You should compute the errors before you can save the differences\n");
	continue;
      }

      for( int i=0; i<numberOfFiles-1; i++ )
      {
        aString nameOfShowFile;
	sPrintF(nameOfShowFile,"compDiff%i.show",i);
        printF("Saving show file: [%s] with u[%i]-u[%i], t=%16.10e\n",(const char*)nameOfShowFile,i,numberOfFiles-1,time(i));
        Ogshow show(nameOfShowFile);
	show.saveGeneralComment("Difference computed with comp");
	show.startFrame();                                         // start a new frame
	show.saveComment(0,sPrintF("u[%i]-u[%i]",i,numberOfFiles-1));  
	show.saveComment(1,sPrintF(" t=%16.10e ",time(i)));               
	show.saveSolution( ud[i] ); 

      }
    }
    else if( answer=="exit" )
    {
      break;
    }
  }

  if( myid==0 )
    fclose(outFile);

  Overture::finish();          
  return 0;
}
