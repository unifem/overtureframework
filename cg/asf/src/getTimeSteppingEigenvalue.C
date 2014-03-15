// ==============================================================================
//    Get the time stepping eigenvalues (that determine dt) 
//    for the ASF and related equations 
// ==============================================================================

#include "Cgasf.h"
#include "AsfParameters.h"
#include "MappedGridOperators.h"
#include "Chemkin.h"
#include "ParallelUtility.h"

#define asfdts EXTERN_C_NAME(asfdts)
extern "C"
{
 void asfdts(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask, const real& xy, const real& rx, const real& u, const real& uu, const real&gv,  
      const real & dw, const real & rL, const real & pL, const real & dtVar,
      const int&bc, const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );
}


//\begin{>>OverBlownInclude.tex}{\subsection{getTimeSteppingEigenvalueASF}} 
void Cgasf::
getTimeSteppingEigenvalue(MappedGrid & mg,
			  realMappedGridFunction & u0, 
			  realMappedGridFunction & gridVelocity,  
			  real & reLambda,
			  real & imLambda, 
			  const int & grid)
//=====================================================================================================
//
// /Description:
//   Determine the real and imaginary parts of lambda : maximum eigenvalue for the
// semi-discrete discretization. Lambda is used to determine the time step by requiring
// lambda*dt to be in the stability region of the particular time stepping method we are
// using.
//
//   Transform the system
//     r.t + u*r.x + v*r.y + r(u.x+v.y) = 0
//     u.t + u*u.x + v*u.y + (1/r)p.x = (mu/r)( (4/3)u_xx+u_yy+(1/3)v_xy )
//     v.t + u*v.x + v*v.y + (1/r)p.y = (mu/r)( v_xx+(4/3)v_yy+(1/3)u_xy )
//     p.t + u*p.x + v*p.y + gamma*p( u.x+v.y ) = ...
//     T.t + u T.x + v T.y + (gamma-1)T div(u) = (1/r)(gamma-1)K_T ( T.xx+T.yy )
// into
//     
//     u.t + a1 u.r + b1 u.s + p11 p.r + p12 p.s = nu11 u.rr + nu12 u.rs + nu22 u.ss
//     v.t + a1 v.r + b1 v.s + p21 p.r + p22 p.s = nu11 v.rr + nu12 v.rs + nu22 v.ss
//     p.t + a1 p.r + b1 p.s +gamma p( r.x u.r + s.x u.s + r.y v.r + s.y v.s ) = ...
//
//
//\end{OverBlownInclude.tex}  
// ===========================================================================================
{
  const realArray & u = u0;
  const int & rc = parameters.dbase.get<int >("rc");
  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & tc = parameters.dbase.get<int >("tc");
  const int & sc = parameters.dbase.get<int >("sc");
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & numberOfSpecies = parameters.dbase.get<int >("numberOfSpecies");

  const real & mu = parameters.dbase.get<real >("mu");
  const real & gamma = parameters.dbase.get<real >("gamma");
  const real & kThermal = parameters.dbase.get<real >("kThermal");
  // const real & Rg = parameters.dbase.get<real >("Rg");
  // const real & avr = parameters.dbase.get<real >("avr");
  const real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  // const real & nu = parameters.dbase.get<real >("nu");
  const real & anu = parameters.dbase.get<real >("anu");
  // const real & nuRho = parameters.dbase.get<real >("nuRho");
  const real & advectionCoefficient = parameters.dbase.get<real >("advectionCoefficient");

  const int & linearizeImplicitMethod = parameters.dbase.get<int >("linearizeImplicitMethod");
  const int & explicitMethod = parameters.dbase.get<int >("explicitMethod");

  real imLambdaImplicit, imLambdaExplicit;
  real reLambdaImplicit, reLambdaExplicit;

  bool useOpt=true;
  if( useOpt )
  {
    // new optimized version 

#ifdef USE_PPP
    realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0,u0Local);
#else  
    const realSerialArray & u0Local = u0;
#endif

    Index I1,I2,I3;
    // getIndex( mg.extendedIndexRange(),I1,I2,I3);

    getIndex(mg.gridIndexRange(),I1,I2,I3);  
    bool ok = ParallelUtility::getLocalArrayBounds(u0,u0Local,I1,I2,I3);  

    if( ok ) // there are pts on this processor
    {

      MappedGridOperators & op = *u0.getOperators();

      const int isRectangular=op.isRectangular(); // trouble when moving ??
      // const int isRectangular=mg.isRectangular(); 
  
      real nu = parameters.dbase.get<real >("nu");
      if( parameters.dbase.get<bool >("advectPassiveScalar") ) 
	nu=max(nu,parameters.dbase.get<real >("nuPassiveScalar"));   // could do better than this

      // only apply fourth-order AD here if it is explicit
      const bool useFourthOrderArtificialDiffusion = parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") &&
	!parameters.dbase.get<bool >("useImplicitFourthArtificialDiffusion");

      const real adcPassiveScalar=1.; // coeff or linear artificial diffusion for the passive scalar ** add to params

      const bool & gridIsMoving = parameters.gridIsMoving(grid);
      const int gridIsImplicit=parameters.getGridIsImplicit(grid);

      int useWhereMask=true;
      real dx[3]={1.,1.,1.};
      real xab[2][3]={0.,1.,0.,1.,0.,1.};
      if( isRectangular )
	mg.getRectangularGridParameters( dx, xab );
   
      const int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
      const int gridType= isRectangular ? 0 : 1;


      // For non-moving grids u==uu, otherwise uu is a temp space to hold (u-gv)
      realSerialArray uu;
      if( parameters.gridIsMoving(grid) )
      {
	// fix this : uu only needs to be dimensioned to Ru -- needed to pass uu bounds to asfdts
	// uu.redim(u0Local.dimension(0),u0Local.dimension(1),u0Local.dimension(2),parameters.dbase.get<Range >("Ru")); 
	uu.redim(u0Local.dimension(0),u0Local.dimension(1),u0Local.dimension(2),u0Local.dimension(3)); 
      }
#ifdef USE_PPP
      const real *pu = u0Local.getDataPointer();
      const real *puu = parameters.gridIsMoving(grid) ? uu.getDataPointer() : pu;
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
	((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getLocalArray().getDataPointer();
      const real *pVariableDt = pdtVar !=NULL ? (*pdtVar)[grid].getLocalArray().getDataPointer() : pu;
  
      // For now we need the center array for the axisymmetric case:
      const real *pxy = parameters.isAxisymmetric() ? mg.center().getLocalArray().getDataPointer() : pu;
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getLocalArray().getDataPointer();
      const int *pmask = mg.mask().getLocalArray().getDataPointer();
      const real *pgv = gridIsMoving ? gridVelocity.getLocalArray().getDataPointer() : pu;
#else  
      const real *pu = u0.getDataPointer();
      const real *puu = parameters.gridIsMoving(grid) ? uu.getDataPointer() : pu;
  
      const real *pdw = parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL ? pu : 
	((*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid]).getDataPointer();
  
      const real *pVariableDt = pdtVar !=NULL ? (*pdtVar)[grid].getDataPointer() : pu;
  
      // For now we need the center array for the axisymmetric case:
      if( parameters.isAxisymmetric() ) 
      {
	assert( mg.center().getLength(0)>0 );
      }
      const real *pxy = parameters.isAxisymmetric() ? mg.center().getDataPointer() : pu;
      const real *prsxy = isRectangular ? pu :  mg.inverseVertexDerivative().getDataPointer();
      const int *pmask = mg.mask().getDataPointer();
      const real *pgv = gridIsMoving ? gridVelocity.getDataPointer() : pu;
#endif

      const real *prL = linearizeImplicitMethod ? rL()[grid].getLocalArray().getDataPointer() : pu; 
      const real *ppL = linearizeImplicitMethod ? pL()[grid].getLocalArray().getDataPointer() : pu; 

      int i1a=mg.gridIndexRange(0,0);
      int i2a=mg.gridIndexRange(0,1);
      int i3a=mg.gridIndexRange(0,2);
      int viscoPlasticOption = 0; // not really used
      int ipar[] ={viscoPlasticOption,//kkc 070201 viscoplastic stuff was here (int)parameters.dbase.get<Parameters::PDEModel >("pdeModel"),
                   (int)parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),
		   grid,
		   (int)parameters.gridIsMoving(grid),
		   useWhereMask,
		   (int)gridIsImplicit, // parameters.gridIsImplicit(grid),
		   (int)parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod"),
		   (int)parameters.dbase.get<Parameters::ImplicitOption >("implicitOption"),
		   (int)parameters.isAxisymmetric(),
		   (int)parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion"),
		   (int)useFourthOrderArtificialDiffusion,
		   (int)parameters.dbase.get<bool >("advectPassiveScalar"),
		   gridType,
		   i1a,i2a,i3a };  // 


      reLambda=0.;
      imLambda=0.;

      const real yEps=sqrt(REAL_EPSILON);  // tol for axisymmetric *** fix this ***
      const real ajEps=REAL_EPSILON*100.; // for minimum value of the jacobian
      real rpar[]={reLambdaImplicit,imLambdaImplicit,
                   reLambdaExplicit,imLambdaExplicit,
                   mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),
		   dx[0],dx[1],dx[2],   
		   adcPassiveScalar,
		   xab[0][0],xab[0][1],xab[0][2],yEps,ajEps};  // 

      DataBase *pdb = &parameters.dbase;
      int ierr=0;
    
      asfdts(mg.numberOfDimensions(),
	     I1.getBase(),I1.getBound(),
	     I2.getBase(),I2.getBound(),
	     I3.getBase(),I3.getBound(),
	     u0Local.getBase(0),u0Local.getBound(0),u0Local.getBase(1),u0Local.getBound(1),
	     u0Local.getBase(2),u0Local.getBound(2),u0Local.getBase(3),u0Local.getBound(3),
	     *pmask, *pxy, *prsxy,
	     *pu, *puu, *pgv, *pdw, *prL, *ppL, *pVariableDt,
	     mg.boundaryCondition(0,0), ipar[0], rpar[0], pdb, ierr );

      reLambdaImplicit=rpar[0];
      imLambdaImplicit=rpar[1];
      reLambdaExplicit=rpar[2];
      imLambdaExplicit=rpar[3];
    }
    else
    {
      reLambdaImplicit=0.;
      imLambdaImplicit=0.;
      reLambdaExplicit=0.;
      imLambdaExplicit=0.;
    }
    
    if( true || debug() & 4 ) 
    {
      printf(">>>>>>Cgasf::asfdts: NEW: implicit-(reLambda,imLambda)=(%9.3e,%9.3e) \n"
             "                          explicit-(reLambda,imLambda)=(%9.3e,%9.3e) (p=%i) hMin=%e\n",
	     reLambdaImplicit,imLambdaImplicit,reLambdaExplicit,imLambdaExplicit,
             parameters.dbase.get<int >("myid"),hMin[grid]); 
     
//      ::display(divergenceDampingWeight(),sPrintF(" asfdts: divergenceDampingWeight() grid=%i ",grid),
// 	       parameters.dbase.get<FILE* >("debugFile"));
     
    }

  } // end if useOpt 

  if( !useOpt )
  {
    // --- old way ---

#ifdef USE_PPP
    Overture::abort("Error- fix this Bill");
#else


    // Get Index's for the interior+boundary points
    Index I1,I2,I3;

    // check for Nan's (max returns negative number when there are (all?) Nan's )
    real maxU=0.;;
    where( mg.mask()>0 )
    {
      for( int n=0; n<numberOfComponents; n++ )
	maxU= max(maxU, max(fabs(u(I1,I2,I3,n))));   
    }
    if( maxU!=maxU || maxU<0. || maxU>1.e6 )
    {
      cout << "getTimeSteppingEigenvalueASF: max(fabs(u))=" << max(fabs(u)) <<endl;
      // u.display("Cin::getLambda: ERROR: u is getting big");
      throw "error";
    }

    getIndex( mg.extendedIndexRange(),I1,I2,I3);

    gridMachNumber[grid] = maxMachNumber(u0);
    printf(" getTimeSteppingEigenvalue: grid Mach = %e, ",gridMachNumber[grid]);

    //  (4./3.) mu is an over estimate
    RealArray nu0(I1,I2,I3);
    nu0 = max((4./3.)*mu,kThermal*(gamma-1.))/u(I1,I2,I3,rc);

    RealArray diffusion;
    if( mg.numberOfDimensions()==1 && parameters.dbase.get<bool >("computeReactions") )  // *********** fix this ***********
    {
      if( true ) Overture::abort("finish this");
/* ----      
   diffusion.redim(I1,I2,I3);
   assert(parameters.dbase.get<Reactions* >("reactions")!=NULL);
   Reactions & reactions = *parameters.dbase.get<Reactions* >("reactions");
    
   RealArray & tranCoeff = transportCoefficients();        // holds viscosity and diffusion coefficients
   const RealArray & te = u(I1,I2,I3,tc);  
   const RealArray & pe = u(I1,I2,I3,pc);
      
   Range S0(0,numberOfSpecies-1);
   Range Sc(sc,sc+numberOfSpecies-1);
    
   const int & etac=0, lambdac=1;
   RealArray x(I1,I2,I3,S0);
   reactions.massFractionToMoleFraction(u(I1,I2,I3,Sc),x);  
   reactions.viscosity(te,x,tranCoeff(I1,I2,I3,etac) );  // uses mole fractions, eta is scaled

   reactions.thermalConductivity(te,x,tranCoeff(I1,I2,I3,lambdac));

   reactions.diffusion(pe,te,x,tranCoeff(I1,I2,I3,S0+2));
      
   const RealArray & eta     = tranCoeff(I1,I2,I3,etac);
   const RealArray & lambda  = tranCoeff(I1,I2,I3,lambdac);
 
   // **** need cp ****
   diffusion(I1,I2,I3)=eta/u(I1,I2,I3,rc);
   for( int s=0; s<numberOfSpecies; s++ )
   {
   diffusion(I1,I2,I3)=max(diffusion(I1,I2,I3),tranCoeff(I1,I2,I3,s+2));   // species diffusion coefficients
   }
   ----- */
    }
  
    MappedGridOperators & op = *u0.getOperators();

    // define an alias:
    mg.update(MappedGrid::THEinverseVertexDerivative);
  
    realMappedGridFunction & rx = mg.inverseVertexDerivative();
    rx.setOperators(op);

    const int nd=mg.numberOfDimensions();
#define MN(m,n) ((m)+nd*(n))
#define RX(m,n) rx(I1,I2,I3,MN(m,n))
  
  
//MappedGridSolverWorkSpace::resize(uu,I1,I2,I3,parameters.dbase.get<Range >("Ru")); // *** added 060925 ***
    realArray uu(I1,I2,I3,parameters.dbase.get<Range >("Ru"));
  
    Range N = parameters.dbase.get<Range >("Rt");


    if( parameters.isMovingGridProblem() )
    {
      uu(I1,I2,I3,uc)=advectionCoefficient*(u(I1,I2,I3,uc)-gridVelocity(I1,I2,I3,0));
      if( mg.numberOfDimensions() >1 )
	uu(I1,I2,I3,vc)=advectionCoefficient*(u(I1,I2,I3,vc)-gridVelocity(I1,I2,I3,1));
      if( mg.numberOfDimensions() > 2 )
	uu(I1,I2,I3,wc)=advectionCoefficient*(u(I1,I2,I3,wc)-gridVelocity(I1,I2,I3,2));
    }
    else
    {
      uu(I1,I2,I3,uc)=advectionCoefficient*u(I1,I2,I3,uc);
      if( mg.numberOfDimensions() >1 )
	uu(I1,I2,I3,vc)=advectionCoefficient*u(I1,I2,I3,vc);
      if( mg.numberOfDimensions() > 2 )
	uu(I1,I2,I3,wc)=advectionCoefficient*u(I1,I2,I3,wc);
    }

    if( mg.numberOfDimensions()==1 )
    {
      realArray aa1,nu11;
      // aa1 = u*r.x - nu ( r1.xx )   
      // nu11 = nu0*( r1.x*r1.x + r1.y*r1.y )
      if( !parameters.dbase.get<bool >("computeReactions") )
      {
	aa1   = uu(I1,I2,I3,uc)*rx(I1,I2,I3,0,0) - nu0*( rx.x(I1,I2,I3,0,0)(I1,I2,I3,0,0) ); 
	nu11 = nu0*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,0,0) );
      }
      else
      {
	aa1   = uu(I1,I2,I3,uc)*rx(I1,I2,I3,0,0) + diffusion(I1,I2,I3)*( rx.x(I1,I2,I3,0,0)(I1,I2,I3,0,0) );
	nu11 = diffusion(I1,I2,I3)*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,0,0) );
      }

      // Grid spacings on unit square:
      real dr1 = mg.gridSpacing()(axis1);

      // *** compute imLambda ***
      aa1 = abs(aa1)*(1./dr1);

      if( !linearizeImplicitMethod ) // ||  pL.numberOfComponentGrids()==0 )  // ***********
      {
	cout << "getLambda for NOT linearImplicitMethod\n";
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambdaImplicit=max(aa1);
      }
      else
      { // in the linearized method some of the pressure is done explicitly
	cout << "getLambda for linearImplicitMethod\n";
      
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambdaImplicit=max(
	    aa1
	    +SQRT( gamma*fabs((u(I1,I2,I3,pc)-pL()[grid](I1,I2,I3))*(1./u(I1,I2,I3,rc)-1./rL()[grid](I1,I2,I3)))*( 
		     pow( rx(I1,I2,I3,0,0), 2)*(1./SQR(dr1)) )
	      ));
      }

      // add in speed-of-sound terms for explicit time step
      where( mg.mask()(I1,I2,I3)>0 )
	imLambdaExplicit=max(
	  aa1
	  +SQRT( gamma*(u(I1,I2,I3,pc)+pressureLevel)/u(I1,I2,I3,rc)*( 
		   pow( rx(I1,I2,I3,0,0), 2)*(1./SQR(dr1)) )
	    )
	  );

// ********
      if( gridMachNumber[grid]>.5 )
	imLambdaImplicit =imLambdaExplicit;

      // save reLambda(I1,I2,I3) in aa1
      aa1= nu11 *(4./(dr1*dr1));

      if( anu>0. )
      { // artificial dissipation:
	// op.getDerivatives(u0);   // derivatives returned in ux,uxx,...
	realArray ux(I1,I2,I3,N);
	op.derivative(MappedGridOperators::xDerivative,u,ux,I1,I2,I3,N); 

	// this is not really right:
	aa1+=  4.*anu*(1.+               // *** is 4 right? ****
		       fabs(ux(I1,I2,I3,rc))
		       +fabs(ux(I1,I2,I3,uc))
		       +fabs(ux(I1,I2,I3,pc))  );
      }

      // *** compute reLambda ***
      where( mg.mask()(I1,I2,I3)>0 )
	reLambdaExplicit=max(aa1);
      reLambdaImplicit=reLambdaExplicit;

    }
    else if( mg.numberOfDimensions()==2 )
    {
      realArray aa1(I1,I2,I3),bb1(I1,I2,I3),nu11(I1,I2,I3),nu12(I1,I2,I3),nu22(I1,I2,I3);

      realArray rxx(I1,I2,I3), ryy(I1,I2,I3), sxx(I1,I2,I3), syy(I1,I2,I3);
    
      op.derivative(MappedGridOperators::xDerivative,rx,rxx,I1,I2,I3,MN(0,0));  // rxx
      op.derivative(MappedGridOperators::yDerivative,rx,ryy,I1,I2,I3,MN(0,1));  // ryy
      op.derivative(MappedGridOperators::xDerivative,rx,sxx,I1,I2,I3,MN(1,0));  // sxx
      op.derivative(MappedGridOperators::yDerivative,rx,syy,I1,I2,I3,MN(1,1));  // syy

      // aa1 = u*r.x + v*r.y - nu ( r1.xx + r1.yy )     bb1 = u*s.x + v*s.y - nu ( r2.xx + r2.yy )

      aa1   = uu(I1,I2,I3,uc)*RX(0,0)+uu(I1,I2,I3,vc)*RX(0,1) - nu0(I1,I2,I3)*( rxx  + ryy ); 
      bb1   = uu(I1,I2,I3,uc)*RX(1,0)+uu(I1,I2,I3,vc)*RX(1,1) - nu0(I1,I2,I3)*( sxx + syy );
      // nu11 = nu0*( r1.x*r1.x + r1.y*r1.y )
      // nu12 = nu0*( r1.x*r2.x + r1.y*r2.y )*2 
      // nu22 = nu0*( r2.x*r2.x + r2.y*r2.y ) 
      nu11 = nu0(I1,I2,I3)*( RX(0,0)*RX(0,0) + RX(0,1)*RX(0,1) );
      nu12 = nu0(I1,I2,I3)*( RX(0,0)*RX(1,0) + RX(0,1)*RX(1,1) )*2.;
      nu22 = nu0(I1,I2,I3)*( RX(1,0)*RX(1,0) + RX(1,1)*RX(1,1) );

      // Grid spacings on unit square:
      real dr1 = mg.gridSpacing()(axis1);
      real dr2 = mg.gridSpacing()(axis2);

      // *** compute imLambda ***
      aa1 = abs(aa1)*(1./dr1)+abs(bb1)*(1./dr2);

      if( !linearizeImplicitMethod ) // ||  pL.numberOfComponentGrids()==0 )  // ***********
      {
	cout << "getLambda for NOT linearImplicitMethod\n";
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambdaImplicit=max(aa1);
      }
      else
      { // in the linearized method some of the pressure is done explicitly
	cout << "getLambda for linearImplicitMethod\n";
      

//      display(pL()[grid](I1,I2,I3),"pL()[grid](I1,I2,I3)","%4.1f ");
//      display(rL()[grid](I1,I2,I3),"rL()[grid](I1,I2,I3)","%4.1f ");
//      display(RX(1,1),"RX(1,1)","%4.1f ");
      
	where( mg.mask()(I1,I2,I3)>0 )
	  imLambdaImplicit=max(
	    aa1
	    +SQRT( gamma*fabs((u(I1,I2,I3,pc)-pL()[grid](I1,I2,I3))*(1./u(I1,I2,I3,rc)-1./rL()[grid](I1,I2,I3)))*( 
		     pow( RX(0,0)*(1./dr1) + RX(1,0)*(1./dr2) , 2)
		     + pow( RX(0,1)*(1./dr1) + RX(1,1)*(1./dr2) , 2)
		     )
	      )
	    );

      }

      // add in speed-of-sound terms for explicit time step
//     display(aa1,"aa1","%4.1f ");
//     display(u(I1,I2,I3,pc),"u(I1,I2,I3,pc)","%4.1f ");
//     display(u(I1,I2,I3,rc),"u(I1,I2,I3,rc)","%4.1f ");
//     display(RX(1,1),"RX(1,1)","%4.1f ");
      where( mg.mask()(I1,I2,I3)>0 )
	imLambdaExplicit=max(
	  aa1
	  +SQRT( gamma*(u(I1,I2,I3,pc)+pressureLevel)/u(I1,I2,I3,rc)*( 
		   pow( RX(0,0)*(1./dr1) + RX(1,0)*(1./dr2) , 2)
		   + pow( RX(0,1)*(1./dr1) + RX(1,1)*(1./dr2) , 2)
		   )
	    )
	  );

// ********
      if( gridMachNumber[grid]>.5 )
	imLambdaImplicit =imLambdaExplicit;

      // save reLambda(I1,I2,I3) in aa1
      aa1= nu11 *(4./(dr1*dr1)) +abs(nu12)*(1./(dr1*dr2)) +nu22 *(4./(dr2*dr2));

      if( anu>0. )
      { // artificial dissipation:
	// op.getDerivatives(u0);   // derivatives returned in ux,uxx,...
	realArray ux(I1,I2,I3,N),uy(I1,I2,I3,N);
	op.derivative(MappedGridOperators::xDerivative,u,ux,I1,I2,I3,N); 
	op.derivative(MappedGridOperators::yDerivative,u,uy,I1,I2,I3,N); 

	// this is not really right:
	aa1+=  8.*anu*(1.+
		       fabs(ux(I1,I2,I3,rc))+fabs(uy(I1,I2,I3,rc))
		       +fabs(ux(I1,I2,I3,uc))+fabs(uy(I1,I2,I3,uc))
		       +fabs(ux(I1,I2,I3,vc))+fabs(uy(I1,I2,I3,vc))
		       +fabs(ux(I1,I2,I3,pc))+fabs(uy(I1,I2,I3,pc))  );
      }

      // *** compute reLambda ***
      where( mg.mask()(I1,I2,I3)>0 )
	reLambdaExplicit=max(aa1);
      reLambdaImplicit=reLambdaExplicit;
    }
    else
    {

      realArray a[3],nuA[3][3];
      // a[0] = u*r.x + v*r.y + w*r.z - nu ( r1.xx + r1.yy +r1.zz ) 
      int axis;
      for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
	a[axis]=uu(I1,I2,I3,uc)*rx(I1,I2,I3,axis,0)
	  +uu(I1,I2,I3,vc)*rx(I1,I2,I3,axis,1)
	  +uu(I1,I2,I3,wc)*rx(I1,I2,I3,axis,2)
	  -nu0*( rx.x(I1,I2,I3,axis,0)(I1,I2,I3,axis,0)  
                 +rx.y(I1,I2,I3,axis,1)(I1,I2,I3,axis,1)   
                 +rx.z(I1,I2,I3,axis,2)(I1,I2,I3,axis,2) );
      // nu11 = nu0*( r1.x*r1.x + r1.y*r1.y )
      // nu12 = nu0*( r1.x*r2.x + r1.y*r2.y )*2 
      // nu22 = nu0*( r2.x*r2.x + r2.y*r2.y ) 
      for( axis=axis1; axis<mg.numberOfDimensions(); axis++ )
      {
	for( int dir=axis1; dir<=axis; dir++ )  // only compute for dir<=axis
	{
	  nuA[axis][dir] =nu0*( rx(I1,I2,I3,axis,0)*rx(I1,I2,I3,dir,0) 
				+ rx(I1,I2,I3,axis,1)*rx(I1,I2,I3,dir,1) 
				+ rx(I1,I2,I3,axis,2)*rx(I1,I2,I3,dir,2) );
	}
      }
//    nu11 = nu0*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,0,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,0,1) );
//    nu12 = nu0*( rx(I1,I2,I3,0,0)*rx(I1,I2,I3,1,0) + rx(I1,I2,I3,0,1)*rx(I1,I2,I3,1,1) )*2.;
//    nu22 = nu0*( rx(I1,I2,I3,1,0)*rx(I1,I2,I3,1,0) + rx(I1,I2,I3,1,1)*rx(I1,I2,I3,1,1) );

      // Grid spacings on unit square:
      real dr1 = mg.gridSpacing()(axis1);
      real dr2 = mg.gridSpacing()(axis2);
      real dr3 = mg.gridSpacing()(axis3);

      // *** compute imLambda ***
      a[0]=abs(a[0])*(1./dr1)+abs(a[1])*(1./dr2)+abs(a[2])*(1./dr3);
      where( mg.mask()(I1,I2,I3)>0 )
	imLambdaImplicit=max(a[0]);

      // add in speed-of-sound terms for explicit time step
      where( mg.mask()(I1,I2,I3)>0 )
      {
	imLambdaExplicit=max(
	  a[0]
	  +SQRT( gamma*(u(I1,I2,I3,pc)+pressureLevel)/u(I1,I2,I3,rc)*( 
		   pow( rx(I1,I2,I3,0,0)*(1./dr1) + rx(I1,I2,I3,1,0)*(1./dr2) + rx(I1,I2,I3,2,0)*(1./dr3) , 2)
		   + pow( rx(I1,I2,I3,0,1)*(1./dr1) + rx(I1,I2,I3,1,1)*(1./dr2) + rx(I1,I2,I3,2,1)*(1./dr3) , 2)
		   + pow( rx(I1,I2,I3,0,2)*(1./dr1) + rx(I1,I2,I3,1,2)*(1./dr2) + rx(I1,I2,I3,2,2)*(1./dr3) , 2)
		   )
	    )
	  );
      }
      // save reLambda(I1,I2,I3) in a[0]
      a[0]=      nuA[0][0] *(4./(dr1*dr1)) 
	+nuA[1][1] *(4./(dr2*dr2))
	+nuA[2][2] *(4./(dr3*dr3))
	+abs(nuA[1][0])*(2./(dr2*dr1))   // 2 from nuA[1][0]+nuA[0][1]
	+abs(nuA[2][0])*(2./(dr3*dr1)) 
	+abs(nuA[2][1])*(2./(dr3*dr2)) ;
    

      if( anu>0. )
      { // artificial dissipation:
	// op.getDerivatives(u0);   // derivatives returned in ux,uxx,...
	realArray ux(I1,I2,I3,N),uy(I1,I2,I3,N),uz(I1,I2,I3,N);
	op.derivative(MappedGridOperators::xDerivative,u,ux,I1,I2,I3,N); 
	op.derivative(MappedGridOperators::yDerivative,u,uy,I1,I2,I3,N); 
	op.derivative(MappedGridOperators::zDerivative,u,uz,I1,I2,I3,N); 

	// this is not really right:
	a[0]+= 12.*anu*(1.+
			fabs(ux(I1,I2,I3,rc))+fabs(uy(I1,I2,I3,rc))+fabs(uz(I1,I2,I3,rc))
			+fabs(ux(I1,I2,I3,uc))+fabs(uy(I1,I2,I3,uc))+fabs(uz(I1,I2,I3,uc))
			+fabs(ux(I1,I2,I3,vc))+fabs(uy(I1,I2,I3,vc))+fabs(uz(I1,I2,I3,vc))
			+fabs(ux(I1,I2,I3,wc))+fabs(uy(I1,I2,I3,wc))+fabs(uz(I1,I2,I3,wc))
			+fabs(ux(I1,I2,I3,pc))+fabs(uy(I1,I2,I3,pc))+fabs(uz(I1,I2,I3,pc))  );
      }

      // *** compute reLambda ***
      where( mg.mask()(I1,I2,I3)>0 )
	reLambdaExplicit=max(a[0]);
      reLambdaImplicit=reLambdaExplicit;

    }
#endif
  }
  

  if( explicitMethod )
  {
    printf("================getTimeSteppingEigenvalueASF speed EXPLICT METHOD ! ======================\n");
    reLambda=reLambdaExplicit;  
    imLambda=imLambdaExplicit;
  }
  else
  {
    reLambda=reLambdaImplicit;  
    imLambda=imLambdaImplicit;
  }
  reLambda=ParallelUtility::getMaxValue(reLambda);
  imLambda=ParallelUtility::getMaxValue(imLambda);
  if( true )
  {
    printF(" ===getTimeSteppingEigenvalueASF: reLambda=%8.2e, imLambda=%8.2e ===\n",reLambda,imLambda);
  }
  
}
 
