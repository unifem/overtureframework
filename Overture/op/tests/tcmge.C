//===============================================================================
//  Coefficient Matrix Example 
// 
//    o Solve a system of equations on a CompositeGrid
//    o Shows how to fill in more general coefficient matrix boundary conditions using the lower
//      level interface.
//
// Examples:
//    tcmge -grid=square10 -debug=63 
//    tcmge -grid=sise2.order2 -solver=petsc
//    tcmge -grid=cice2.order2 -plot
// 
//==============================================================================
#include "Overture.h"  
#include "Oges.h"
#include "CompositeGridOperators.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "SparseRep.h"
#include "display.h"
#include "PlotStuff.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))


#define ForStencil(m1,m2,m3)   \
    for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
    for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
    for( m1=-halfWidth1; m1<=halfWidth1; m1++) 

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

// =======================================================================
// indexToEquation( n,i1,i2,i3 ) : defines the global index for each unknown in the system
//     n=component number (uc,vc,...) 
//    (i1,i2,i3) = grid point 
// =======================================================================
#define indexToEquation( n,i1,i2,i3 ) (n+1+ \
 numberOfComponentsForCoefficients*(i1-equationNumberBase1+\
             equationNumberLength1*(i2-equationNumberBase2+\
             equationNumberLength2*(i3-equationNumberBase3))) + equationOffset)

// =======================================================================
// =======================================================================
#define setEquationNumber(m, ni,i1,i2,i3,  nj,j1,j2,j3 )\
          equationNumber(m,i1,i2,i3)=indexToEquation( nj,j1,j2,j3)

// =======================================================================
// =======================================================================
#define setClassify(n,i1,i2,i3, type) \
    classify(i1,i2,i3,n)=type

// =======================================================================
//  Macro to zero out the matrix coefficients for equations e1,e1+1,..,e2
// =======================================================================
#define zeroMatrixCoefficients( coeff,e1,e2, i1,i2,i3 )\
for( int m=CE(0,e1); m<=CE(0,e2+1)-1; m++ ) \
  coeff(m,i1,i2,i3)=0.


// ====================================================================================================
//   This function fills in the coeff equations at the ghost lines by accessing the low-level
//   interface to coefficient matricies. 
//
//  The coefficients in the matrix are defined by two arrays: 
//
//    coeff(m,i1,i2,i3), m=0,1,2,...    : the coefficients associated with the equation(s) at the grid point (i1,i2,i3)
//                                        For a system of equations, 
//                                                m=0,1,...,stencilDim-1             : holds the first eqn. in the system,
//                                                m=stencilDim,1,...,2*stencilDim-1  : holds the second eqn, etc. 
//                                        (stencilDim is defined below)
//                                        
//    equationNumber(m,i1,i2,i3)        : a global index that identifies the "unknown" in the system 
//                                        (the global index of each unknown is defined by the indexToEquation macro)
//
// NOTES:
//   See cg/ins/src/insImp.h     : for fortran version macros
//       cg/ins/src/insImpINS.bf : for examples 
// ====================================================================================================
int 
fillInBoundaryConditions( realCompositeGridFunction & coeffcg )
{
  CompositeGrid & cg = *coeffcg.getCompositeGrid();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  const int eq1=0, eq2=1;   // equation numbers
  const int uc=0, vc=1;     // component numbers

  Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
  int i1,i2,i3, j1,j2,j3, i1m,i2m,i3m, m1,m2,m3;
  int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    realMappedGridFunction & coeff = coeffcg[grid];
    MappedGridOperators & mgop = *coeff.getOperators();
    
    assert( coeff.sparse!=NULL );

    SparseRepForMGF & sparse = *coeff.sparse;
    int numberOfComponentsForCoefficients = sparse.numberOfComponents;  // size of the system of equations
    int numberOfGhostLines = sparse.numberOfGhostLines;
    int stencilSize = sparse.stencilSize;
    int stencilDim=stencilSize*numberOfComponentsForCoefficients; // number of coefficients per equation

  
    const int equationOffset=sparse.equationOffset;
    IntegerArray & equationNumber = sparse.equationNumber;
    IntegerArray & classify = sparse.classify;

    const int equationNumberBase1  =equationNumber.getBase(1);
    const int equationNumberLength1=equationNumber.getLength(1);
    const int equationNumberBase2  =equationNumber.getBase(2);
    const int equationNumberLength2=equationNumber.getLength(2);
    const int equationNumberBase3  =equationNumber.getBase(3);

    const int orderOfAccuracy=mgop.getOrderOfAccuracy(); assert( orderOfAccuracy==2 );
    
    // stencil width's and half-width's : 
    const int width = orderOfAccuracy+1; 
    const int halfWidth1 = (width-1)/2;
    const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
    const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;

    Range M0 = stencilSize;       
    Range M = coeff.dimension(0);
    
    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition(side,axis)>0 )
      {
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
      
	is1=is2=is3=0;
	isv[axis]=1-2*side;

	
        // --- Apply the "interior" equation as the boundary condition: 
        //         eq1:       u_xx + u_yy -v = 
        //         eq2:       v_xx + v_yy +u =   ** do not do this if use a Neumann BC for v **
        // NOTES:
        //     (1) the interior equation approximation is centered on the boundary point (i1,i2,i3)
        //         but is associated with the ghost-point (i1m,i2m,i3m)


        // const int eqnStart=eq1, eqnEnd=eq2;    // impose interior equation as a BC for both components
        const int eqnStart=eq1, eqnEnd=eq1;       // impose interior equation as a BC for u only (if v.n=gv is given)

        // Evaluate the (single component) Laplace operator for points on the boundary 
	realSerialArray lapCoeff(M0,Ib1,Ib2,Ib3);
	mgop.assignCoefficients(MappedGridOperators::laplacianOperator,lapCoeff,Ib1,Ib2,Ib3,0,0); // 

	FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3) // loop over points on the boundary 
	{
	  i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)

	  for( int e=eqnStart; e<=eqnEnd; e++ ) // equation eq1, eq2, ...
	  {
            int c=e;               
	    ForStencil(m1,m2,m3)
	    {
	      int m  = M123(m1,m2,m3);        // the single-component coeff-index
              int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index 

	      coeff(mm,i1m,i2m,i3m) = lapCoeff(m,i1,i2,i3);
	      
	      // Specify that the above coeff value is the coefficient of component c at the grid point (j1,j2,j3).
	      j1=i1+m1, j2=i2+m2, j3=i3+m3;   // the stencil is centred on the boundary pt (i1,i2,i3)
	      setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
	    }

	    if( true )
	    { // add on the "-v" term for eq1, or the "+u" for eqn2
              c= e==eq1 ? vc : uc;                // we are specifying coefficients of "v" or "u"
              real val = e==eq1 ? -1. : 1.;
              m1=m2=m3=0;          // diagonal entry in the stencil
	      int m  = M123(m1,m2,m3);        // the single-component coeff-index
              int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index 

	      coeff(mm,i1m,i2m,i3m) = val;
	      j1=i1+m1, j2=i2+m2, j3=i3+m3;
	      setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
	    }
	    

            // Specify that this a "real" equation on the first ghost line: 
            // (A "real" equation has a possible non-zero right-hand-side)
	    setClassify(e,i1m,i2m,i3m, SparseRepForMGF::ghost1);  

	  }
	} // end FOR_3D


        // -- At corners between two physical boundaries we cannot apply the interior equation on the points
        //    marked A below since the same equation (from the corner) would appear twice in the matrix. 
        // 
        //                |    |    |
        //            G-  X----I----I--       I=interior point
        //                |    |    |         X=boundary point
        //                |    |    |         G=ghost point
        //            A-- X----X----X--       A=ghost point on an extended boundary 
        //                |    |    | 
        //            V   A    G    G 

        // INSTEAD we will extrapolate the values at the points "A"

        const int orderOfExtrap=3;
        real extrapCoeff[orderOfExtrap+1] = {1.,-3.,3.,-1.}; // extrapolation coefficients 

        const int ghost=1;  // ghost point to fill in 
        const int axisp = (axis+1) % numberOfDimensions;       // tangential direction
        assert( numberOfDimensions==2 );                        // -- finish me for 3D
	for( int side2=0; side2<=1; side2++ )                  // in 2D there are two adjacent boundaries, left and right 
	{
	  if( mg.boundaryCondition(side2,axisp)>0 ) // adjacent boundary is a physical boundary 
	  {
	    Ibv[axisp] = mg.gridIndexRange(side2,axisp);  // set loop bounds to the "left-side" or "right-side"

	    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3) // loop over points (in 2D there will just be point to assign)
	    {
	      i1m=i1-is1*(ghost); // ghost point
	      i2m=i2-is2*(ghost);
	      i3m=i3-is3*(ghost);
	      for( int e=eqnStart; e<=eqnEnd; e++ ) // equation eq1, eq2, ...
	      {
		// fill in the extrapolation equations
                int c=e;   // extrapolate this component of the solution 

                // first zero all coefficients for this equation: 
                zeroMatrixCoefficients( coeff,e,e, i1m,i2m,i3m ); 

                // For extrapolation we just fill in the first orderOfExtrap+1 values into the coeff matrix.
                // We do NOT think of the values as arranged in a stencil as we did in the above case. 
                for( int m=0; m<=orderOfExtrap; m++ )
		{
		  j1=i1m+is1*m; //  m-th point moving inward from the ghost point (i1,i2,i3)
		  j2=i2m+is2*m;
		  j3=i3m+is3*m;
		  
		  int mm = CE(c,e)+m;
		  
		  coeff(mm,i1m,i2m,i3m)=extrapCoeff[m];  //  m=0,1,2,..

		  setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 ); // set the global equation number 
		}
                // classify this equation as extrapolation (will always have a zero right-hand-side)
		setClassify(e,i1m,i2m,i3m, SparseRepForMGF::extrapolation);
	      }
	    }
	  }
	}
	

      } // end if( mg.boundaryCondition(side,axis)>0 )
    } // end ForBoundary 
    

  }

  return 0;
}


void
plotResults( PlotStuff & ps, realCompositeGridFunction & u, realCompositeGridFunction & err )
// ==============================================================================================
// Plot results 
// ==============================================================================================
{
      
  GraphicsParameters psp;

  aString answer;
  aString menu[]=
  {
    "solution",
    "error",
    "grid",
    "erase",
    "exit",
    ""
  };
    
  for( ;; )
  {
    ps.getMenuItem(menu,answer,"choose an option");
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="solution" )
    {
      psp.set(GI_TOP_LABEL,"Solution"); 
      PlotIt::contour(ps,u,psp);
    }
    else if( answer=="error" )
    {
      psp.set(GI_TOP_LABEL,"error"); 
      PlotIt::contour(ps,err,psp);
    }
    else if( answer=="grid" )
    {
      psp.set(GI_TOP_LABEL,"grid"); 
      PlotIt::plot(ps,*u.getCompositeGrid(),psp);
    }
    else if( answer=="erase" )
    {
      ps.erase();
    }
      
  }

}


int 
main(int argc, char **argv)
{
  Overture::start(argc,argv);  // initialize Overture

  aString nameOfOGFile="square10.hdf";
    
  printF("Usage: `tcmge -grid=<gridName> -solver=[yale|harwell|slap|petsc] -debug=<value> -tz=[poly|trig] -plot' \n"
         "  NOTE: set debug=63 to see the full sparse matrix from Oges.\n");

  real solverTol=1.e-8; // tolerance for interative solvers 
  bool plot=false;
  int solverType=OgesParameters::yale; 
  int twilightZoneOption=0;
  int len=0;
  aString solverName="yale";  // default solver 
  for( int i=1; i<argc; i++ )
  {
    aString arg = argv[i];
    if( len=arg.matches("-grid=") )
    {
      nameOfOGFile=arg(len,arg.length()-1);
    }
    else if( len=arg.matches("-debug=") )
    {
      sScanF(arg(len,arg.length()-1),"%i",&Oges::debug);
      printf("Setting Oges::debug=%i\n",Oges::debug);
    }
    else if( arg=="-tz=poly" )
    {
      twilightZoneOption=0;
    }
    else if( arg=="-tz=trig" )
    {
      twilightZoneOption=1;
    }
    else if( arg=="-plot" )
    {
      plot=true;
    }
    else if( len=arg.matches("-solver=") )
    {
      solverName=arg(len,arg.length()-1);
      if( solverName=="yale" )
	solverType=OgesParameters::yale;
      else if( solverName=="harwell" )
	solverType=OgesParameters::harwell;
      else if( solverName=="slap" )
	solverType=OgesParameters::SLAP;
      else if( solverName=="petsc" )
	solverType=OgesParameters::PETSc;
      else
      {
	printf("Unknown solverName=%s \n",(const char*)solverName);
	throw "error";
      }
	
      printf("Setting solverType=%i\n",solverType);
    }
  }


  // make some shorter names for readability
  BCTypes::BCNames 
                   dirichlet             = BCTypes::dirichlet,
                   neumann               = BCTypes::neumann,
                   extrapolate           = BCTypes::extrapolate,
                   normalComponent       = BCTypes::normalComponent,
                   aDotU                 = BCTypes::aDotU,
                   generalizedDivergence = BCTypes::generalizedDivergence,
                   generalMixedDerivative= BCTypes::generalMixedDerivative,
                   aDotGradU             = BCTypes::aDotGradU,
                   vectorSymmetry        = BCTypes::vectorSymmetry,
                   allBoundaries         = BCTypes::allBoundaries; 


  CompositeGrid cg;
  getFromADataBase(cg,nameOfOGFile);
  cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal);

  // ----- create a twilight-zone function -------------
  int numberOfComponents = 2;

  OGFunction *exactPointer=NULL;
  if( twilightZoneOption==1 )
  {
    RealArray fx(numberOfComponents), fy(numberOfComponents), fz(numberOfComponents), ft(numberOfComponents);
    fx=1.;   fx(0)=.5; fx(1)=1.5;
    fy=.1;   fy(0)=.4; fy(1)= .5;
    ft=1.;
    exactPointer = new OGTrigFunction(fx,fy,fz,ft); 
  }
  else
  {
    int degreeOfSpacePolynomial = 2;
    int degreeOfTimePolynomial = 0;
    exactPointer = new OGPolyFunction(degreeOfSpacePolynomial,cg.numberOfDimensions(),numberOfComponents,
			 degreeOfTimePolynomial);
    RealArray c,a;
    int ndc=degreeOfSpacePolynomial+1;
    c.redim(ndc,ndc,ndc,numberOfComponents); c=0.;
    for( int m1=0; m1<ndc; m1++ )for( int m2=0; m2<ndc; m2++ )for( int m3=0; m3<ndc; m3++ )
    {
      for( int n=0; n<numberOfComponents; n++ )
      {
	if( (m1+m2+m3) <= degreeOfSpacePolynomial )
	  c(m1,m2,m3,n)=1./( .25*m1*m1 + .5*m2*m2 + 3.*m3*m3 + n+1.);
      }
    }
    int ndt=degreeOfTimePolynomial+1;
    a.redim(ndt,numberOfComponents); a=1.;
    ((OGPolyFunction*)exactPointer)->setCoefficients( c,a );
      
  }
  OGFunction & exact = *exactPointer;


  Range all;
  // make a grid function to hold the coefficients
  int stencilSize=int( pow(3,cg.numberOfDimensions())+1 );  // add 1 for interpolation equations
  int stencilDimension=stencilSize*SQR(numberOfComponents);
  realCompositeGridFunction coeff(cg,stencilDimension,all,all,all); 
  // make this grid function a coefficient matrix:
  int numberOfGhostLines=1;
  coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
  coeff=0.;
    
  // create grid functions: 
  realCompositeGridFunction u(cg,all,all,all,numberOfComponents),
    f(cg,all,all,all,numberOfComponents);

  CompositeGridOperators op(cg);                            // create some differential operators 
  op.setNumberOfComponentsForCoefficients(numberOfComponents);
  u.setOperators(op);                              // associate differential operators with u
  coeff.setOperators(op);
  
  //  Solve bi-harmonic equation as a system: 

  printF(" ***************************************************************\n"
	 "       Solving the bi-harmonic like system of equations:        \n"
	 "            u_xx + u_yy - v = f_0                               \n"
	 "            v_xx + v_yy + u = f_1                               \n"
         " Boundary conditions: u=gu, v.n=gv\n"
         " Numerical BC's: impose u_xx + u_yy - v = f_0 on the boundary\n"
	 "       Grid = %s, solver=%s              \n"
	 " ***************************************************************\n",
	 (const char*)nameOfOGFile,(const char*)solverName);

  const int eq1=0, eq2=1;   // equation numbers
  const int uc=0, vc=1;      // component numbers

  // Here are the interior equations: 
  coeff=( op.laplacianCoefficients(eq1,uc)-op.identityCoefficients(eq1,vc)+
	  op.laplacianCoefficients(eq2,vc)+op.identityCoefficients(eq2,uc)                                 );

  // Here are the boundary conditions we can impose with the high-level operators: 
  coeff.applyBoundaryConditionCoefficients(eq1,uc,dirichlet,  allBoundaries);  
  // coeff.applyBoundaryConditionCoefficients(eq2,vc,dirichlet,  allBoundaries);
  coeff.applyBoundaryConditionCoefficients(eq2,vc,neumann,  allBoundaries); // Neumann BC on v 

  // Fill in the non-standard boundary conditions: 
  fillInBoundaryConditions( coeff );

  // coeff.applyBoundaryConditionCoefficients(eq1,uc,extrapolate,allBoundaries)
  // coeff.applyBoundaryConditionCoefficients(eq2,vc,extrapolate,allBoundaries);


  coeff.finishBoundaryConditions();


  // Display the coefficient matrix here: 
  if( Oges::debug & 16 ) 
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      displayCoeff(coeff[grid],sPrintF("Here is coeff for grid %i after finishBoundaryConditions",grid));
    }
  }
  
  Oges solver( cg );                     // create a solver
  solver.setCoefficientArray( coeff );   // supply coefficients
  solver.set(OgesParameters::THEsolverType,solverType); 
  if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
  {
    solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    solver.set(OgesParameters::THEtolerance,max(solverTol,REAL_EPSILON*10.));
  }    

  // assign the rhs:  u=exact on the boundary
  Index I1,I2,I3, Ia1,Ia2,Ia3;
  int side,axis;
  Index Ib1,Ib2,Ib3, Ig1,Ig2,Ig3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.indexRange(),I1,I2,I3);  

    f[grid](I1,I2,I3,uc)=exact.xx(mg,I1,I2,I3,uc)+exact.yy(mg,I1,I2,I3,uc) - exact(mg,I1,I2,I3,vc);
    f[grid](I1,I2,I3,vc)=exact.xx(mg,I1,I2,I3,vc)+exact.yy(mg,I1,I2,I3,vc) + exact(mg,I1,I2,I3,uc);
    if( cg.numberOfDimensions()==3 )
    {
      f[grid](I1,I2,I3,uc)+=exact.zz(mg,I1,I2,I3,uc);
      f[grid](I1,I2,I3,vc)+=exact.zz(mg,I1,I2,I3,vc);
    }

    ForBoundary(side,axis)
    {
      if( mg.boundaryCondition()(side,axis) > 0 )
      {
        // NOTE: The RHS for any extrapolation equations will be set to zero by Oges 

	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1);

	f[grid](Ib1,Ib2,Ib3,uc)=exact(mg,Ib1,Ib2,Ib3,uc);
	// f[grid](Ib1,Ib2,Ib3,vc)=exact(mg,Ib1,Ib2,Ib3,vc);

	const realSerialArray & normal  = mg.vertexBoundaryNormal(side,axis);
	f[grid](Ig1,Ig2,Ig3,vc)=(normal(Ib1,Ib2,Ib3,0)*exact.x(mg,Ib1,Ib2,Ib3,vc)+
				 normal(Ib1,Ib2,Ib3,1)*exact.y(mg,Ib1,Ib2,Ib3,vc));

	// The equations for the ghost line are the interior equations centered on the boundary: 
	f[grid](Ig1,Ig2,Ig3,uc)=exact.xx(mg,Ib1,Ib2,Ib3,uc)+exact.yy(mg,Ib1,Ib2,Ib3,uc) - exact(mg,Ib1,Ib2,Ib3,vc);
	// f[grid](Ig1,Ig2,Ig3,vc)=exact.xx(mg,Ib1,Ib2,Ib3,vc)+exact.yy(mg,Ib1,Ib2,Ib3,vc) + exact(mg,Ib1,Ib2,Ib3,uc);

      }
    }
  }
  
  u=0.;  // for interative solvers.
  real time0=getCPU();
  solver.solve( u,f );   // solve the equations

  printf("residual=%8.2e, time for solve = %8.2e (iterations=%i)\n",
         solver.getMaximumResidual(),getCPU()-time0,solver.getNumberOfIterations());

  if( true )
  {
    time0=getCPU();
    solver.solve( u,f );   // solve the equations

    printf("residual=%8.2e, time for 2nd solve = %8.2e (iterations=%i)\n",
	   solver.getMaximumResidual(),getCPU()-time0,solver.getNumberOfIterations());

  }
    
  // u.display("Here is the solution to u.xx+u.yy=f");
  realCompositeGridFunction errcg(cg,all,all,all,numberOfComponents);
  errcg=0.;
  for( int n=0; n<numberOfComponents; n++ )
  {
    real error=0.;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      int extra=1;  // include ghost points in error 
      getIndex(cg[grid].indexRange(),I1,I2,I3,extra);
      realArray & err = errcg[grid];
      where( cg[grid].mask()(I1,I2,I3)!=0 )
      {
        err(I1,I2,I3,n) = u[grid](I1,I2,I3,n)-exact(cg[grid],I1,I2,I3,n);
	error=max(error,max(abs(err(I1,I2,I3,n))));
      }

      if( Oges::debug & 4 ) 
	abs(err(I1,I2,I3,n)).display("abs(error)");
    }
    printf("Maximum error in component %i = %e\n",n,error);  
  }
    

  if( plot )
  {
    u.setName("u",0);
    u.setName("v",1);
    errcg.setName("err-u",0);
    errcg.setName("err-v",1);
    
    PlotStuff ps(true,"tcmge");
    plotResults( ps, u, errcg );
  }
  
  delete exactPointer;
  Overture::finish();          
  return(0);
}

