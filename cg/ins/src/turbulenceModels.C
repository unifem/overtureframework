#include "Cgins.h"
#include "Parameters.h"
#include "turbulenceParameters.h"
#include "ParallelUtility.h"

#define ForBoundary(side,axis)   for( int axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( int side=0; side<=1; side++ )

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


#define U(c)     u(I1,I2,I3,c)   
#define UU(c)   uu(I1,I2,I3,c)
#define UX(c)   ux(I1,I2,I3,c)
#define UY(c)   uy(I1,I2,I3,c)
#define UZ(c)   uz(I1,I2,I3,c)
#define UXX(c) uxx(I1,I2,I3,c)
#define UXY(c) uxy(I1,I2,I3,c)
#define UXZ(c) uxz(I1,I2,I3,c)
#define UYY(c) uyy(I1,I2,I3,c)
#define UYZ(c) uyz(I1,I2,I3,c)
#define UZZ(c) uzz(I1,I2,I3,c)


int Cgins::
initializeTurbulenceModels(GridFunction & cgf)
{
  realCompositeGridFunction & u = cgf.u;
  CompositeGrid & cg = *u.getCompositeGrid();
  
  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras  && !parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    const int numberOfSmooths=100;
    printF("**** Smooth initial conditions for SpalartAllmaras turbulent viscosity...\n");
    

    //  ---Use a Jacobi smoother, under-relaxed
    real omega0=.9;
    real omo=1.-omega0, ob4=omega0/4., ob6=omega0/6.;
  
    Index I1,I2,I3;
    Index N(parameters.dbase.get<int >("kc"),1);
    for( int it=0; it<numberOfSmooths; it++ )
    {
      if( debug() & 4 )
	printF(" smoothTurbulenceModel>>> iteration=%i\n",it);

      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	realArray & v = u[grid];
	getIndex(extendedGridIndexRange(cg[grid]),I1,I2,I3);
      
	if( cg.numberOfDimensions()==2 || parameters.dbase.get<int >("compare3Dto2D") )
	{
	  // where( cg[grid).mask()(I1,I2,I3) >0 ) ** add this ?
	  v(I1,I2,I3,N)=omo*v(I1,I2,I3,N)
	    +ob4*( v(I1+1,I2,I3,N)+v(I1-1,I2,I3,N)
		   +v(I1,I2+1,I3,N)+v(I1,I2-1,I3,N));
	}
	else
	{
	  v(I1,I2,I3,N)=omo*v(I1,I2,I3,N)
	    +ob6*( v(I1+1,I2,I3,N)+v(I1-1,I2,I3,N)
		   +v(I1,I2+1,I3,N)+v(I1,I2-1,I3,N)
		   +v(I1,I2,I3+1,N)+v(I1,I2,I3-1,N) );
	}


	// assign bc's 
        real t=0.;
        BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

	turbulenceModelBoundaryConditions(t,u[grid],parameters,grid,pBoundaryData);
      }

      // interpolate 
      interpolate(cgf,N);

    }
  }
  

  return 0;
}

// ************* THIS ROUTINE NOT USED ANYMORE ***********************
//! Add turbulence models to the incompressible Navier-Stokes
/*!
  /param nuT (output) : the turbulent viscosity
  /param ut (output) :
 */
int Cgins::
turbulenceModels(realArray & nuT,
		 MappedGrid & mg,
		 const realArray & u, 
		 const realArray & uu, 
		 const realArray & ut, 
		 const realArray & ux, 
		 const realArray & uy, 
		 const realArray & uz, 
		 const realArray & uxx, 
		 const realArray & uyy, 
		 const realArray & uzz, 
		 const Index & I1, const Index & I2, const Index & I3, 
		 Parameters & parameters,
		 real nu,
		 const int numberOfDimensions,
		 const int grid, const real t )
{
  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::noTurbulenceModel )
  {
    return 0;
  }
  const intArray & mask = mg.mask();

  const int & uc = parameters.dbase.get<int >("uc");
  const int & vc = parameters.dbase.get<int >("vc");
  const int & wc = parameters.dbase.get<int >("wc");
  const int & pc = parameters.dbase.get<int >("pc");
  const int & kc = parameters.dbase.get<int >("kc");
  const int & epsc = parameters.dbase.get<int >("epsc");
  const int & sc = parameters.dbase.get<int >("sc");


  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon || parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kOmega )
  {

    realArray prod;
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
    {
      const real cMu=.09;
      nuT=cMu*UU(kc)*UU(kc)/UU(epsc);   // nu_T = C*k*k/eps
    }
    else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kOmega )
    {
      nuT=UU(kc)/UU(epsc);   // nu_T = k/omega
    }
    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon || parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kOmega )
    {
      if( numberOfDimensions==2 )
      {
        // prod =  nuT*( 2.*( UX(uc)*UX(uc)+UY(vc)*UY(vc) ) + SQR( UX(vc)+UY(uc) ) );
        prod =  nuT*( 4.*( UX(uc)*UX(uc) ) + SQR( UX(vc)+UY(uc) ) );
      }
      else
      {
// 	prod = nuT*
// 	  ( 2.*( ux23(i1,i2,i3,1)**2+
//                  uy23(i1,i2,i3,2)**2+
// 		 uz23(i1,i2,i3,3)**2                     )
// 	    +( ux23(i1,i2,i3,2)+uy23(i1,i2,i3,1) )**2
// 	    +( uy23(i1,i2,i3,3)+uz23(i1,i2,i3,2) )**2
// 	    +( uz23(i1,i2,i3,1)+ux23(i1,i2,i3,3) )**2  )   
      }
      
    }


    if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kEpsilon )
    {
      // here are the k-epsilon equations
      // tau_{ij} = 
      const real sigmaEps=1./1.3;
      const real cEps1=1.44, cEps2=1.92;

      ut(I1,I2,I3,kc)=   UU(uc)*UX(kc  )+UU(vc)*UY(kc  )
              +prod - UU(epsc)
              +(nu+nuT)*(UXX(kc)+UYY(kc));

      ut(I1,I2,I3,epsc)= UU(uc)*UX(epsc)+UU(vc)*UY(epsc)
            +cEps1*(UU(epsc)/UU(kc))*prod 
            -cEps2*(UU(epsc)*UU(epsc)/UU(kc))
            +(nu+sigmaEps*nuT)*(UXX(epsc)+UYY(epsc));
    }
    else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::kOmega )
    {
      // here are the k-omega equations
      const real sigma=.5, sigmas=.5, alpha=13./25.;

      real beta=(9./125.);   // more here in 3d: *(1.+70.*chi)/(1.+80.*chi);     real betas=(9./100.);  // more here
      real betas=(9./100.);  // more here

      ut(I1,I2,I3,kc)=   UU(uc)*UX(kc)+UU(vc)*UY(kc)
            + prod -betas*UU(kc)*UU(epsc)
            + (nu+sigmas*nuT)*(UXX(kc)+UYY(kc));

      ut(I1,I2,I3,epsc)= UU(uc)*UX(epsc)+UU(vc)*UY(epsc)
            +alpha*(UU(epsc)/UU(kc))*prod -beta*UU(epsc)*UU(epsc)
            +(nu+sigma*nuT)*(UXX(epsc)+UYY(epsc));
    }
    
  }
//    else if( parameters.dbase.get<bool >("advectPassiveScalar") )  // advect passive scalar, add artificial diffusion
//    {
//      ut(I1,I2,I3,sc)=   UU(uc)*UX(sc) + UU(vc)*UY(sc);
//      if( true )
//      {
//        // add some artificial diffusion
//        const real adc=1.;
//        if( numberOfDimensions==2 )
//  	ut(I1,I2,I3,sc)+= 
//  	  adc*(u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)+u(I1,I2+1,I3,sc)+u(I1,I2-1,I3,sc)-4.*u(I1,I2,I3,sc));
//        else if( numberOfDimensions==3 )
//  	ut(I1,I2,I3,sc)+=
//  	  adc*( u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)+u(I1,I2+1,I3,sc)+u(I1,I2-1,I3,sc)
//  		+u(I1,I2,I3+1,sc)+u(I1,I2,I3-1,sc)  -6.*u(I1,I2,I3,sc));
//        else
//  	ut(I1,I2,I3,sc)+= adc*(u(I1+1,I2,I3,sc)+u(I1-1,I2,I3,sc)-2.*u(I1,I2,I3,sc));
//      }
//     else if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && (ad21!=0. || ad22!=0.) )
//     { // --- add 2nd order artificial diffusion -- add here if explicit time stepping
//       real cd22=ad22/SQR(numberOfDimensions);
//       ut(I1,I2,I3,sc)+=AD2(sc);
//     }
//}
  else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
  {
    const int nc=kc;
    
    assert( parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")!=NULL );
    const realArray & d = (*parameters.dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];


//      real cb1=.1355, cb2=.622, cv1=7.1, sigma=2./3., sigmai=1./sigma, kappa=.41;
//      cb1*=parameters.dbase.get< >("spalartAllmarasScaleFactor");  // note: cw1 depends on cb1
//      real cw1=cb1/(kappa*kappa)+(1.+cb2)/sigma, cw2=.3, cw3=2., cw3e6=pow(cw3,6.), cv1e3=pow(cv1,3.);
      
    real cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0;
    getSpalartAllmarasParameters(cb1, cb2, cv1, sigma, sigmai, kappa, cw1, cw2, cw3, cw3e6, cv1e3, cd0, cr0);

    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      // turn off source terms for TZ flow testing
      // cb1=0.;
      // cb2=0.;
      // cw1=0.;
    }

    realArray s, r,g,fw,fnu1(I1,I2,I3),fnu2(I1,I2,I3), chi, dSq(I1,I2,I3);

    chi=U(nc)/nu;
    fnu1=chi*chi*chi/( chi*chi*chi+cv1e3);

    // fnu2 can be negative when chi=cv1 (=7.1) for example
    fnu2=1.-chi/(1.+chi*fnu1);

    if( numberOfDimensions==2 )
    {
      s= fabs(UY(uc)-UX(vc)); // turbulence source term
    }
    else
    {
      s=SQRT( SQR(UY(uc)-UX(vc)) + SQR(UZ(vc)-UY(wc)) + SQR(UX(wc)-UZ(uc)) );
    }
    
    // const real epsD=1.e-20;
    // d(I1,I2,I3)=max(d(I1,I2,I3),epsD);
    dSq=(d(I1,I2,I3)+cd0)*(d(I1,I2,I3)+cd0);
      
    s+= U(nc)*fnu2/( dSq*(kappa*kappa) );
      
    // we could assume that d is set to a nonzero value on the boundary.
    // r= U(nc)/( max( s*dSq*(kappa*kappa), cr0) ); // *WRONG* s can be negative

    // we assume that g reaches a constant for r large enough
    r = min( U(nc)/( s*dSq*(kappa*kappa) ), cr0 );
    

    // I think that r and g will always be bigger than 1 ??
    g=r+cw2*(pow(r,6.)-r);
    fw=g*pow( (1.+cw3e6)/(pow(g,6.)+cw3e6), 1./6.); // If g gets too big then this could overflow

    realArray nSqBydSq;
    nSqBydSq=cw1*fw*(U(nc)*U(nc)/dSq);

    if( numberOfDimensions==2 )
    {
      ut(I1,I2,I3,nc)= UU(uc)*UX(nc)+UU(vc)*UY(nc) + cb1*s*U(nc) + sigmai*(nu+U(nc))*(UXX(nc)+UYY(nc))
	+ ((1.+cb2)*sigmai)*(UX(nc)*UX(nc)+UY(nc)*UY(nc))
	- nSqBydSq;
    }
    else
    {
      ut(I1,I2,I3,nc)= UU(uc)*UX(nc)+UU(vc)*UY(nc)+UU(wc)*UZ(nc) 
        + cb1*s*U(nc) + sigmai*(nu+U(nc))*(UXX(nc)+UYY(nc)+UZZ(nc))
	+ ((1.+cb2)*sigmai)*(UX(nc)*UX(nc)+UY(nc)*UY(nc)+UZ(nc)*UZ(nc))
	- nSqBydSq;
    }
    
    // mg.update(MappedGrid::THEvertex );
    const realArray & x = mg.vertex();

    if( false && !parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      int numberOfTrips=1; // parameters.dbase.get<IntegerArray >("turbulenceTripPoint").getLength(1);
      for( int i=0; i<numberOfTrips; i++ )
      {
	int gridt=0; // turbulenceTripPoint(0,i);
	int i1=0; // turbulenceTripPoint(1,i);
	int i2=0; // turbulenceTripPoint(1,i);
	int i3=0;
	
	if( grid==gridt )
	{
	  real uTrip=u(i1,i2,i3,uc), vTrip=u(i1,i2,i3,vc);

	  real xTrip=0., yTrip=0.;

	  realArray deltaU; deltaU = sqrt( SQR(UU(uc)-uTrip)+SQR(UU(vc)-vTrip) );
	  realArray dTrip;  dTrip  = sqrt( SQR(x(I1,I2,I3,0)-xTrip)+SQR(x(I1,I2,I3,1)-yTrip) );
	  realArray ft1, gt;

	  real wt; // vorticity at trip
	  wt=max(REAL_MIN,fabs(uy(i1,i2,i3,uc)-ux(i1,i2,i3,vc)));
	 
	  real dxTrip = 1./20.; // max(dx,dy);  // grid spacing along the wall at the trip
	  
	  gt = min(.1, deltaU*(dxTrip/wt));
	  

	  const real ct1=1., ct2=2.;

	  ft1 = ct1*gt*exp( -(ct2*wt*wt)/(deltaU*deltaU)*( d(I1,I2,I3)*d(I1,I2,I3)+gt*gt*dTrip*dTrip ) );
	
	  ut(I1,I2,I3,nc)+=ft1*deltaU*deltaU;
	}
	
      }
    }
    
      
    real ntMax=0., nMax=0., sMin=0, sMax=0., fwMin, fwMax=0., rMin=0., rMax=0., gMin=0., gMax=0., 
         nSqBydSqMin, nSqBydSqMax, nxMax, nyMax, nuTMax,nuTMin ;
    nuT=  u(I1,I2,I3,nc)*fnu1;
    where( mask(I1,I2,I3)>0 )
    {
      sMin=min(s);
      sMax=max(s);
      rMin=min(r);
      rMax=max(r);
      gMin=min(g);
      gMax=max(g);
      fwMin=min(fw);
      fwMax=max(fw);
      nMax=max(fabs(u(I1,I2,I3,nc)));
      ntMax=max(fabs(ut(I1,I2,I3,nc)));
      nSqBydSqMin=min(nSqBydSq);
      nSqBydSqMax=max(nSqBydSq);
      nxMax=max(fabs(UX(nc)));
      nyMax=max(fabs(UY(nc)));
      nuTMax=max(nuT);
      nuTMin=min(nuT);
    }

      
    printf(" Spalart Almaras: grid=%i t=%9.3e : max du/dt =%8.2e nMax=%7.1e s=[%7.1e,%7.1e] "
           "fw=[%7.1e,%7.1e] r=[%7.1e,%7.1e]\n "
	   "           g=[%7.1e,%7.1e] nSqBydSq=[%7.1e,%7.1e] nxMax=%7.1e nyMax=%7.1e nuT=[%7.1e,%7.1e]\n",
	   grid,t,ntMax,nMax,sMin,sMax,fwMin,fwMax,rMin,rMax,gMin,gMax,nSqBydSqMin,nSqBydSqMax,
            nxMax,nyMax,nuTMin,nuTMax);
    
  }
  else
  {
    Overture::abort("turbulenceModelsINS:ERROR: unknown turbulence model");
  }
  
  return 0;
}

//! Apply boundary conditions for the turbulence models. 
/*!
 This function is called by applyBoundaryConditionsINS
 */
int Cgins::
turbulenceModelBoundaryConditions(const real & t,
				  realMappedGridFunction & u,
				  Parameters & parameters,
				  int grid,
				  RealArray *pBoundaryData[2][3] )
{
  
  const RealArray & bcData = parameters.dbase.get<RealArray >("bcData");
  // BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);
  
//    // Boundary conditions for the passive scalar.
//    if( parameters.dbase.get<bool >("advectPassiveScalar") )
//    {
//      assert( parameters.dbase.get<int >("sc")>=0 && parameters.dbase.get<int >("sc")<parameters.dbase.get<int >("numberOfComponents") );

//      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),BCTypes::dirichlet,Parameters::inflowWithVelocityGiven,bcData,pBoundaryData,t,
//                             Overture::defaultBoundaryConditionParameters(),grid);
//      // u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),neumann,allBoundaries,0.,t); *wrong for outflow*

//      u.applyBoundaryCondition(parameters.dbase.get<int >("sc"),BCTypes::extrapolate,BCTypes::allBoundaries,0.,t);
//    }


  Parameters::TurbulenceModel & turbulenceModel=parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel");


  const int kc = parameters.dbase.get<int >("kc");
  const int epsc = parameters.dbase.get<int >("epsc");
  
  if( turbulenceModel==Parameters::kEpsilon )
  {
    // Boundary conditions for k and epsilon
    assert( kc>=0 && kc<parameters.dbase.get<int >("numberOfComponents") );

    Range KE(kc,epsc);
    BoundaryConditionParameters extrapParams;
    extrapParams.orderOfExtrapolation=1;  // low order extrapolation to keep k and eps positive


    u.applyBoundaryCondition(kc  ,BCTypes::dirichlet  ,InsParameters::noSlipWall,bcData,pBoundaryData,t);
    u.applyBoundaryCondition(epsc,BCTypes::dirichlet  ,InsParameters::noSlipWall,bcData,pBoundaryData,t);
    u.applyBoundaryCondition(KE  ,BCTypes::extrapolate,InsParameters::noSlipWall,0.,t,extrapParams);

    u.applyBoundaryCondition(kc  ,BCTypes::dirichlet  ,InsParameters::inflowWithVelocityGiven,bcData,pBoundaryData,t);
    u.applyBoundaryCondition(epsc,BCTypes::dirichlet  ,InsParameters::inflowWithVelocityGiven,bcData,pBoundaryData,t);
    u.applyBoundaryCondition(KE  ,BCTypes::extrapolate,InsParameters::inflowWithVelocityGiven,0.,t,extrapParams);

    u.applyBoundaryCondition(KE,BCTypes::neumann,InsParameters::slipWall,0.,t);


    if( parameters.dbase.get<int>("outflowOption")==0 )
    {
      // extrapolate outflow 
      const int orderOfExtrapolationForOutflow = parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
      assert( orderOfExtrapolationForOutflow>0 );
      extrapParams.orderOfExtrapolation=orderOfExtrapolationForOutflow;
      u.applyBoundaryCondition(KE,BCTypes::extrapolate,InsParameters::outflow,0.,t,extrapParams);
    }
    else
    {
      // Neumann BC at outflow
      u.applyBoundaryCondition(KE,BCTypes::neumann,InsParameters::outflow,0.,t);
    }
    
  }

  if( turbulenceModel==Parameters::kOmega )
  {
    // Boundary conditions for k and omega (omega == epsc )
    assert( kc>=0 && kc<parameters.dbase.get<int >("numberOfComponents") );

    Range KE(kc,epsc);
    u.applyBoundaryCondition(KE,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t);
    u.applyBoundaryCondition(KE,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t);
  }

  if( turbulenceModel==Parameters::SpalartAllmaras )
  {
    // dirichlet on noSlip walls and inflow
    // neumann on slipWalls
    // extrapolate outflow
    const int nc=kc;
    assert( nc>=0 && nc<parameters.dbase.get<int >("numberOfComponents") );
    
    if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta &&
        t>0. )  // apply all boundary conditions at t=0
    {
      // we only need to apply a limited number of BC's for the steady state solver since most
      // have already been done.
      u.applyBoundaryCondition(nc,BCTypes::extrapolate,Parameters::noSlipWall,0.,t);
      u.applyBoundaryCondition(nc,BCTypes::extrapolate,InsParameters::inflowWithVelocityGiven,0.,t);

    }
    else
    {
      u.applyBoundaryCondition(nc,BCTypes::dirichlet,Parameters::noSlipWall,0.,t);
      u.applyBoundaryCondition(nc,BCTypes::extrapolate,Parameters::noSlipWall,0.,t);

      u.applyBoundaryCondition(nc,BCTypes::dirichlet,InsParameters::inflowWithVelocityGiven,bcData,pBoundaryData,t,
			       Overture::defaultBoundaryConditionParameters(),grid);
      u.applyBoundaryCondition(nc,BCTypes::extrapolate,InsParameters::inflowWithVelocityGiven,0.,t);
    
      u.applyBoundaryCondition(nc,BCTypes::neumann,Parameters::slipWall,0.,t);

      BoundaryConditionParameters extrapParams;
      extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForOutflow");
      u.applyBoundaryCondition(nc,BCTypes::extrapolate,InsParameters::outflow,0.,t,extrapParams);
    }
    
  }
  
  return 0;
}


// ===============================================================================================
/// \brief Compute some turbulence quantities such as y+
// ===============================================================================================
int Cgins::
computeTurbulenceQuantities( GridFunction & gf0 )
{


  CompositeGrid & cg = gf0.cg;
  realCompositeGridFunction & ucg = gf0.u;
  const int numberOfDimensions = cg.numberOfDimensions();

  int i1,i2,i3;
  Index Ib1,Ib2,Ib3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal );


    MappedGridOperators & op = *(ucg[grid].getOperators());
    const intArray & mask = mg.mask();
    OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
    OV_GET_SERIAL_ARRAY_CONST(real,ucg[grid],uLocal);
    OV_GET_SERIAL_ARRAY_CONST(real,mg.vertex(),xLocal);

    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const real & nu = parameters.dbase.get<real >("nu");
    const real rho = 1.;
    
    Range V(uc,uc+numberOfDimensions-1);
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    ForBoundary(side,axis)
    {
      is1=is2=is3=0;
      isv[axis]=1-2*side;

      if( mg.boundaryCondition(side,axis)==InsParameters::noSlipWall )
      {
        real yPlusMin=REAL_MAX, yPlusMax=-1.;
        real tauwMin=REAL_MAX,  tauwMax=-1.; 
	
        #ifdef USE_PPP
	  const RealArray & normal = mg.vertexBoundaryNormalArray(side,axis);
        #else
	  const RealArray & normal = mg.vertexBoundaryNormal(side,axis);
        #endif
	getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	int includeGhost=1;
	bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ib1,Ib2,Ib3,includeGhost);
	if( ok )
	{
	  realSerialArray ux(Ib1,Ib2,Ib3,V), uy(Ib1,Ib2,Ib3,V);
	  
          op.derivative(MappedGridOperators::xDerivative,uLocal,ux,Ib1,Ib2,Ib3,V);
          op.derivative(MappedGridOperators::yDerivative,uLocal,uy,Ib1,Ib2,Ib3,V);
	  
	  if( numberOfDimensions==2 )
	  {
	    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	    {
	      if( maskLocal(i1,i2,i3)>0 )
	      {
		real n1=normal(i1,i2,i3,0), n2=normal(i1,i2,i3,1);
		real t1=-n2, t2=n1;

		// wall shear stress = t.sigma.n 
		real tauw = fabs(nu*(t1*( n1*( 2.*ux(i1,i2,i3,uc)              ) + n2*( uy(i1,i2,i3,uc)+ux(i1,i2,i3,vc)) )+
				     t2*( n1*( uy(i1,i2,i3,uc)+ux(i1,i2,i3,vc) ) + n2*( 2.*uy(i1,i2,i3,vc)             ) ) ));

		real utau = sqrt( tauw/rho );

		// nDist = distance to first grid line
		real nDist = sqrt( SQR(xLocal(i1,i2,i3,0) - xLocal(i1+is1,i2+is2,i3,0 )) + 
				   SQR(xLocal(i1,i2,i3,1) - xLocal(i1+is1,i2+is2,i3,1 )) );
		// y^+ at the first grid line:
		real yPlus = nDist*utau/nu;
	      
		yPlusMin = min(yPlusMin,yPlus);
		yPlusMax = max(yPlusMax,yPlus);
		tauwMin=min(tauwMin,tauw);
		tauwMax=max(tauwMax,tauw);
	      }
	    }
	  }
	  else
	  {
	    // --- 3D ---
	    realSerialArray uz(Ib1,Ib2,Ib3,V);
	    op.derivative(MappedGridOperators::zDerivative,uLocal,uz,Ib1,Ib2,Ib3,V);

	    real t1,t2,t3;
	    const real eps = REAL_MIN*1000.;
	    FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3)
	    {
	      if( maskLocal(i1,i2,i3)>0 )
	      {
		real n1=normal(i1,i2,i3,0), n2=normal(i1,i2,i3,1), n3=normal(i1,i2,i3,2);

                // --- In 3D we choose the tangential direction to be in the flow direction ---
                // -- use the velocity on the first grid line:
                real u1=uLocal(i1+is1,i2+is2,i3+is3,uc), u2=uLocal(i1+is1,i2+is2,i3+is3,vc), u3=uLocal(i1+is1,i2+is2,i3+is3,wc);
		
                // subtract off the normal component of the velocity
                real nDotU = n1*u1+n2*u2+n3*u3;
		u1 = u1 - nDotU*n1;
		u2 = u2 - nDotU*n2;
		u3 = u3 - nDotU*n3;
		
                real uNorm = u1*u1 + u2*u2 + u3*u3;
		if( uNorm>eps )
		{
                  uNorm = 1./sqrt(uNorm);
		  t1=u1*uNorm; t2=u2*uNorm; t3=u3*uNorm;  // use this as the tangent
		}
		else
		{
		  // velocity is almost zero -- just pick any tangential direction
                  // First form: [v1,v2,v3] = some vector not parallel to [n1,n2,n3]
                  real v1=0., v2=0., v3=0.;
                  if( fabs(n1)>=max(fabs(n2),fabs(n3)) )
                    v2=1.;  // n is mainly in the x-direction , choose v in the y-direction
		  else
		    v1=1.;  // choose v in the x-direction
		  // form t = n X v   [a unit vector orthogonal to n]
		  t1=n2*v3-n3*v2;
		  t2=n3*v1-n1*v3;
		  t3=n1*v2-n2*v1;
		}
	     
		// wall shear stress = t.sigma.n 
                real tau11=2.*ux(i1,i2,i3,uc), tau12=uy(i1,i2,i3,uc)+ux(i1,i2,i3,vc), tau13=uz(i1,i2,i3,uc)+ux(i1,i2,i3,wc);
		real tau22=2.*uy(i1,i2,i3,vc), tau23=uz(i1,i2,i3,vc)+uy(i1,i2,i3,wc);
		real tau33=2.*uz(i1,i2,i3,wc);
		real tauw = fabs(nu*(t1*( n1*tau11 + n2*tau12 + n3*tau13 )+
				     t2*( n1*tau12 + n2*tau22 + n3*tau23 )+
				     t3*( n1*tau13 + n2*tau23 + n3*tau33 ) ));

		real utau = sqrt( tauw/rho );

		// nDist = distance to first grid line
		real nDist = sqrt( SQR(xLocal(i1,i2,i3,0) - xLocal(i1+is1,i2+is2,i3+is3,0 )) + 
				   SQR(xLocal(i1,i2,i3,1) - xLocal(i1+is1,i2+is2,i3+is3,1 )) + 
				   SQR(xLocal(i1,i2,i3,2) - xLocal(i1+is1,i2+is2,i3+is3,2 ))  );
		// y+ at the first grid line:
		real yPlus = nDist*utau/nu;
	      
		yPlusMin = min(yPlusMin,yPlus);
		yPlusMax = max(yPlusMax,yPlus);
		tauwMin=min(tauwMin,tauw);
		tauwMax=max(tauwMax,tauw);
	      }
	    }

	  }
	  
	}
	yPlusMin=ParallelUtility::getMinValue(yPlusMin);
	yPlusMax=ParallelUtility::getMaxValue(yPlusMax);
	tauwMax=ParallelUtility::getMaxValue(tauwMax);
	
	printF("t=%9.3e, grid %i (%s) (side,axis)=(%i,%i) yPlus[min,max] = [%8.2e,%8.2e], tauw[min,max]=[%8.2e,%8.2e].\n",
               gf0.t,grid,(const char*)mg.getName(),side,axis,yPlusMin,yPlusMax,tauwMin,tauwMax);

      } // end if noSlipWall

    } // end for boundary
  } // end for grid 
  

  return 0;
}
