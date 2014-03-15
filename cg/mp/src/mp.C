#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "CompositeGridOperators.h"
#include "Oges.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

int
solveProblem( aString & gridName,
              CompositeGrid & cg, realCompositeGridFunction & u, GenericGraphicsInterface & ps, 
	      GraphicsParameters & psp, int plotOption=0 )
// ========================================================================================================
// 
//  From refine.C
// 
// ========================================================================================================
{
  const int myid=max(0,Communication_Manager::My_Process_Number);
  const int np= max(1,Communication_Manager::numberOfProcessors());

  int debug=0;
  bool saveCheckFile=true;
  bool useTtrigTZ=true;  // **********
  
  int grid;

  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
  
  Range all;
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1);  // add 1 for interpolation equations
  realCompositeGridFunction coeff(cg,stencilSize,all,all,all); 
  coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
    
  CompositeGridOperators op(cg);                            // create some differential operators
  op.setStencilSize(stencilSize);

  coeff.setOperators(op);

  // create grid functions: 
  u.updateToMatchGrid(cg);
  realCompositeGridFunction f(cg), err(cg), ue(cg);
  f=0.; // for iterative solvers
  err=0.;

  u.setOperators(op);

  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3;
  char buff[180];
  const real bogusValue=99.;
  
  for( int degree=1; degree<=1; degree++ )
  {
    
    // create a twilight-zone function for checking the errors
    int degreeOfSpacePolynomial = degree; // problemType<2 ? 2 : 1;
    int degreeOfTimePolynomial = 0;
    int numberOfComponents = cg.numberOfDimensions();
    OGPolyFunction poly(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
					degreeOfTimePolynomial);

    real fx=1., fy=1., fz=cg.numberOfDimensions()==2 ? 0. : 1.;

    OGTrigFunction trig(fx,fy,fz);
    RealArray gx(10);
    gx=.333;
    trig.setShifts(gx,gx,gx,gx);

    OGFunction *exactPointer=&poly;

    if( useTtrigTZ ) exactPointer=&trig;  // *************
    
    
    OGFunction & exact= *exactPointer;

    // ========== Now test the elliptic equation solver. ================

    bool testOges=true;
    if( testOges )
    {
      if( FALSE )
	Oges::debug=63;
  
      // make some shorter names for readability
      BCTypes::BCNames extrapolate           = BCTypes::extrapolate,
	dirichlet             = BCTypes::dirichlet,
	allBoundaries         = BCTypes::allBoundaries; 

      coeff.updateToMatchGrid( cg , stencilSize, all, all, all);
      op.updateToMatchGrid( cg );
      op.gridCollection.updateReferences(); // **** work around to fix a bug in CG reference function
      coeff.setIsACoefficientMatrix(TRUE,stencilSize);  
      coeff.setOperators(op);
  
      coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
  
      coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,  allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);


      BoundaryConditionParameters bcParams;
      coeff.finishBoundaryConditions(bcParams);
      //  coeff.display("Here is coeff after finishBoundaryConditions");

      Oges solver( cg );                     // create a solver
      solver.setCoefficientArray( coeff );   // supply coefficients
      if( cg.numberOfDimensions()==3 )
      {
	solver.setSolverType(Oges::bcg);
	solver.setConjugateGradientPreconditioner(Oges::incompleteLU);
	solver.setConjugateGradientTolerance(REAL_EPSILON*10.);
      }    

      // assign the rhs: Laplacian(u)=f, u=exact on the boundary
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[grid];
	getIndex(mg.indexRange(),I1,I2,I3);  
	// display(mg.boundaryCondition(),"Here is bc");
	if( cg.numberOfDimensions()==1 )
	  f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0);
	else if( cg.numberOfDimensions()==2 )
	  f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0);
	else
	  f[grid](I1,I2,I3)=exact.xx(mg,I1,I2,I3,0)+exact.yy(mg,I1,I2,I3,0)+exact.zz(mg,I1,I2,I3,0);
    
	ForBoundary(side,axis)
	{
	  if( mg.boundaryCondition()(side,axis) > 0 )
	  {
	    getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	    f[grid](Ib1,Ib2,Ib3)=exact(mg,Ib1,Ib2,Ib3,0);
	  }
	}
      }
      if( Oges::debug & 16 )
	f.display("Here is the rhs f");
  
      u=0.;  // initial guess for iterative solvers
      real time0=getCPU();
      printF("solve Poisson's equation with Oges...\n");
      solver.solve( u,f );   // solve the equations
      real time=getCPU()-time0;
      cout << "time for 1st solve of the Dirichlet problem = " << time << endl;

      if( Oges::debug & 16 )
	u.display("Here is u");
      real error=0., gridError;
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
    
	getIndex(cg[grid].indexRange(),I1,I2,I3,1);  
	where( cg[grid].mask()(I1,I2,I3)!=0 )
	{
	  gridError=max(abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0))); 
	}
	error=max(error, gridError);
	if( TRUE )
	  printF("Maximum error = %e on grid %s\n",gridError,
		 (const char *)cg[grid].mapping().getName(Mapping::mappingName));

	if( FALSE || Oges::debug & 8 )
	{
	  char buff[80];
      
	  displayMask(cg[grid].mask(),sPrintF(buff,"Here is the mask on grid %i",grid));
	  realArray err(I1,I2,I3);
	  err(I1,I2,I3)=abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0));
	  where( cg[grid].mask()(I1,I2,I3)==0 )
	    err(I1,I2,I3)=0.;
	  printF(" ** max error on grid %i = %e \n",grid,max(err(I1,I2,I3)));
	  // display(err,"abs(error on indexRange +1)","%5.1e ");
	  display(err,"abs(error on indexRange +1)","%3.1f ");
	  // abs(u[grid](I1,I2,I3)-exact(cg[grid],I1,I2,I3,0)).display("abs(error)");
	}
      }
      printF("Maximum error with dirichlet bc's= %e\n",error);  

    }
    

  }  // end for degree


  return 0;


}


int 
main(int argc, char** argv)
{
  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile="innerOuter2.hdf";

  // create and read in a CompositeGrid
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);
  cg0.update(GridCollection::THEdomain);
  printF(" >>> cg0.numberOfDomains()=%i\n",cg0.numberOfDomains());

  cg0.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter);


  RealArray fx(10), fy(10), fz(10), ft(10);
  fx=1.; fy=1.; fz=1.; ft=1.;
  fx(1)=2.; fy(2)=2.;
  
  OGFunction *exactPointer = new OGTrigFunction(fx,fy,fz,ft);

  OGFunction & exact = *exactPointer;
  
  realCompositeGridFunction *ua = new realCompositeGridFunction[cg0.numberOfDomains()];
  
  Range all;
  real t=0., dt=.2;
  for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
  {
    CompositeGrid & cg = cg0.domain[domain];
    realCompositeGridFunction & u = ua[domain];
    int numberOfComponents= (domain % 5) +1;
    
    u.updateToMatchGrid(cg,all,all,all,numberOfComponents);
    for( int n=0; n<numberOfComponents; n++ )
      u.setName(sPrintF("u%i",n),n);
     
    exact.assignGridFunction(u,t+dt*domain);
  }
  
  
//   if( false )
//   {
//     for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
//     {
//       CompositeGrid & cg = cg0.domain[domain];
//       for( int g=0; g<cg.numberOfComponentGrids(); g++ )
//       {
// 	display(cg.interpoleeGrid[g],sPrintF("interpoleeGrid for g=%i, domain=%i",g,domain));
//       }
//     }
//   }
  
  
  bool openGraphicsWindow=true;
  PlotStuff ps(openGraphicsWindow,"tdomain");  // create a PlotStuff object
  PlotStuffParameters psp;                      // This object is used to change plotting parameters


  aString answer;
  aString menu[] = { 
                    "!tdomain",      
                    "contour",                  // Make some menu items
		    "stream lines",
		    "grid",
                    "solve",
		    "erase",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  for(;;)
  {

    ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
    if( answer=="contour" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
        PlotIt::contour(ps,ua[domain],psp);                        // contour/surface plots
    }
    else if( answer=="grid" )
    {
      for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
	PlotIt::plot(ps,cg0.domain[domain]);              // plot the composite grid for this domain
    }
    else if( answer=="stream lines" )
    {
      for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
        PlotIt::streamLines(ps,ua[domain]);                        // streamlines
    }
//     else if( answer=="domain" )
//     {
//       ps.inputString(answer,"Enter the domain number to make active");
//       sScanF(answer,"%i",&domain);
//       printf("Using domain=%i\n",domain);
//       u.updateToMatchGrid(cg0.domain[domain]);
//       u=1.;
//     }
    else if( answer=="solve" )
    {
      for( int domain=0; domain<cg0.numberOfDomains(); domain++ )
        solveProblem(nameOfOGFile,cg0.domain[domain],ua[domain],ps,psp);
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


  delete [] ua;
  
  Overture::finish();   

  return 0;
}

