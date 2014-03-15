#include "Chemkin.h"
#include "Reactions.h"

#define CKHMS  EXTERN_C_NAME(ckhms)
#define CKCPMS EXTERN_C_NAME(ckcpms)
#define CKWYP  EXTERN_C_NAME(ckwyp)

#ifdef OV_USE_DOUBLE
  #define GECO EXTERN_C_NAME(dgeco)
  #define GESL EXTERN_C_NAME(dgesl)
#else
  #define GECO EXTERN_C_NAME(sgeco)
  #define GESL EXTERN_C_NAME(sgesl)
#endif

extern "C"
{
  void GECO( real & b, const int & nbd, const int & nb, int & ipvt, real & rcond, real & work );

  void GEDI( real & b, const int & nbd, const int & nb, int & ipvt, real & det, real & work, 
              const int & job );

  void GESL( real & a, const int & lda,const int & n,const int & ipvt, real & b, const int & job);

}


//\begin{>>ReactionsInclude.tex}{\subsection{chemistrySource}}
void Reactions::
chemistrySource( const RealArray & pS, const RealArray & teS, const RealArray & y, 
		 const RealArray & rhoS_, const RealArray & source_ )
// ====================================================================================
// /Description:
// Compute the chemistry source terms and their Jacobian (sy)
// \begin{verbatim}
//  source =  [ 1./(rho*cv) * sum{ (R_i T - h_i ) \sigma  ]
//            [ sigma_0/rho                               ]
//            [ sigma_1/rho                               ]
//            [ sigma_2/rho                               ]
//            [   ...                                     ]
// \end{verbatim}
// /pS (input) : pressure (scaled)
// /teS (input) : temperature (scaled)
// /y (input) : species
// /rhoS\_ (output) : density (scaled)
// /source (output) : source terms 
//\end{ReactionsInclude.tex}  
// ====================================================================================
{
  RealArray & rhoS = (RealArray &) rhoS_;
  RealArray & source = (RealArray &) source_;
  

  const Range I1=pS.dimension(0);
  const Range I2=pS.dimension(1);
  const Range I3=pS.dimension(2);

  Range S0(0,numberOfSpecies_-1);
  Range S1(1,numberOfSpecies_);

  RealArray r(I1,I2,I3), hi(I1,I2,I3,S0), cpi(I1,I2,I3,S0), cp0(I1,I2,I3), sigmai(I1,I2,I3,S0);

  r=0.;
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    r+=y(I1,I2,I3,s)*(mp.R/molecularWeight(s));          // this is R:

  rhoS(I1,I2,I3)= (p0/(te0*rho0))*pS(I1,I2,I3)/(r(I1,I2,I3)*teS(I1,I2,I3));

  h(teS,hi);                // compute the enthalpy
  cp(teS,cpi);              // compute cp_i for each species
  sigmaFromRPTY(rhoS,pS,teS,y,sigmai);  // compute sigma for each species
  // sigmaFromPTY(p,te,y,sigmai);  // compute sigma for each species

  if( debug & 4 )
    printf("chemistrySource max(fabs(sigmai)) = %e \n",max(fabs(sigmai)));

  source(I1,I2,I3,0)=0.;
  cp0=0.;
  for( s=0; s<numberOfSpecies_; s++ )
  {
    real RuOverMbar = mp.R/molecularWeight(s);

    if( !pressureIsConstant )
      source(I1,I2,I3,0)+= ( (RuOverMbar*te0) * teS(I1,I2,I3) - hi(I1,I2,I3,s) )*sigmai(I1,I2,I3,s);
    else
      source(I1,I2,I3,0)-= hi(I1,I2,I3,s)*sigmai(I1,I2,I3,s);
    cp0(I1,I2,I3)+=cpi(I1,I2,I3,s)*y(I1,I2,I3,s);      // cp = sum cp_i * Y_i 
  }
  
  rhoS(I1,I2,I3)*=rho0; // scaled rho !
  if( !pressureIsConstant )
    source(I1,I2,I3,0)/=rhoS(I1,I2,I3)*(cp0-r);
  else
    source(I1,I2,I3,0)/=rhoS(I1,I2,I3)*cp0;
  for( s=0; s<numberOfSpecies_; s++ )
    source(I1,I2,I3,s+1)=sigmai(I1,I2,I3,s)/rhoS(I1,I2,I3);

  rhoS(I1,I2,I3)*=(1./rho0);
}

//\begin{>>ReactionsInclude.tex}{\subsection{chemistrySourceAndJacobian}}
void Reactions::
chemistrySourceAndJacobian(const RealArray & pS, 
			   const RealArray & teS, 
			   const RealArray & y, 
			   const RealArray & rhoS_, 
			   const RealArray & source_, 
			   const RealArray & sy_ )
// ====================================================================================
// /Description:
// Compute the chemistry source terms and their Jacobian (sy)
// \begin{verbatim}
//  source =  [ 1./(rho*cv) * sum{ (R_i T - h_i ) \sigma  ]
//            [ sigma_0/rho                               ]
//            [ sigma_1/rho                               ]
//            [ sigma_2/rho                               ]
//            [   ...                                     ]
// sy = d{source}/d{T,Y_0,Y_1,...}
// \end{verbatim}
//
// /pS (input) : pressure (scaled)
// /teS (input) : temperature (scaled)
// /y (input) : species
// /rhoS (output) : density (scaled)
// /source (output) : source terms (unscaled)
// /sy\_ (output) : jacobian (unscaled) 
//\end{ReactionsInclude.tex}  
// ====================================================================================
{
  RealArray & rhoS = (RealArray &) rhoS_;
  RealArray & source = (RealArray &) source_;
  RealArray & sy = (RealArray &) sy_;

  const int numberOfEquations=numberOfSpecies_+1;   // number of equations
  const Range I1=pS.dimension(0);
  const Range I2=pS.dimension(1);
  const Range I3=pS.dimension(2);
  Range S0(0,numberOfSpecies_-1); 
  Range R(0,numberOfEquations-1);    // unknowns are (T,Y_0,Y_1,...)

  // evaluate the source terms
  chemistrySource(pS,teS,y, rhoS,source);
  
  const real eps = pow(REAL_EPSILON,(1./3.));  // optimal eps for a first derivative
  const real epsMin=.01*eps; // REAL_EPSILON;
  RealArray delta(I1,I2,I3), tePlus(I1,I2,I3), yPlus(I1,I2,I3,S0), sourcePlus(I1,I2,I3,R);

  yPlus=y(I1,I2,I3,S0);
  // compute the Jacobian by differences
  for( int n=0; n<numberOfEquations; n++ )
  {
    if( n==0 )
    { // vary T
      delta=max(epsMin,eps*teS(I1,I2,I3)); 
      tePlus=teS(I1,I2,I3)+delta;
    }
    else
    { // vary y(n-1)
      delta=max(epsMin,eps*y(I1,I2,I3,n-1));
      yPlus(I1,I2,I3,n-1)=y(I1,I2,I3,n-1)+delta;
    }
    chemistrySource(pS,tePlus,yPlus, rhoS,sourcePlus);

    if( n==0 )
      delta=(1./te0)/delta;  // 1/(te0*delta)
    else
      delta=1./delta; // divide for efficiency
    for( int n0=0; n0<numberOfEquations; n0++ )
     sy(I1,I2,I3,n0,n)= (sourcePlus(I1,I2,I3,n0)-source(I1,I2,I3,n0))*delta;

    // reset values to unperturbed state
    if( n==0 )
      tePlus=teS(I1,I2,I3);
    else if( n<numberOfEquations )
      yPlus(I1,I2,I3,n-1)=y(I1,I2,I3,n-1);

  }
}



//\begin{>>ReactionsInclude.tex}{\subsection{solveImplicitForRTYGivenP}}
int Reactions::
solveImplicitForRTYGivenP(RealArray & u, 
			  const RealArray & rhs,
			  const int & rc, const int & pc, const int & tc, const int & sc,
			  const Index & I1, 
			  const Index & I2, 
			  const Index & I3,
			  const real & dt,
                          bool equilibriumReaction /* = FALSE */ )
// ======================================================================================  
// /Description:
// This function solves the temperature eqn, the Species equations and the equation of state
//   for $(\rho,T,Y_i)$ given the pressure
//
// We solve the following nonlinear system of equations (obtained from a backward-Euler like discretization
//  of the equations for T and $Y_i$:
// \begin{align*}
//       T   - dt [ 1./(\rho cv)   \sum_i (R_i T - h_i ) \sigma ] &= RHS_(T) \\
//       Y_i - dt [ 1./(\rho)  \sigma ]                          &= RHS(Y_i)  \qquad i=0,1,...,  \\
//       p &= \rho R T   
// \end{align*}
//
//  The unknowns are $(rho,T,Y_i)$. The pressure, $p$, at the new time level is assumed to be known.
//
//  /u (input/output) : solution vector:
//              u(I1,I2,I3,rc) : guess for rho on input, the new value for rho on output
//              u(I1,I2,I3,pc) : The correct pressure
//              u(I1,I2,I3,tc) : guess for T on input, new vaule for T on output.
//              u(I1,I2,I3,sc+s)  s=0,...,numberOfSpecies-1 : Guess for species on input, new values on output.
//  /rhs (input) : right-hand-side vector:
//              rhs(I1,I2,I3,0) : The right hand side to the Temperature equation.
//              rhs(I1,I2,I3,s+1)  s=0,...,numberOfSpecies-1 : Right hand side to species equations.
// /rc,pc,tc,sc : indicate the positions of the variables in u
// /I1,I2,I3 (input) : indicate which points to compute
// /dt (input) : dt in the above equations
// /equilibriumReaction (input) : if true, then the reactions are assumed to be in equilibrium
//\end{ReactionsInclude.tex}  
// ======================================================================================  
{

  // * u(I1,I2,I3,rc)*=rho0; // compute unscaled rho
  // * u(I1,I2,I3,tc)*=te0;  // compute unscaled T

  const RealArray & rhoS = u(I1,I2,I3,rc);
  const RealArray & teS0  = u(I1,I2,I3,tc); RealArray & teS  = (RealArray &)teS0;
  RealArray pS(I1,I2,I3);   
  // * p= (pressureLevel+u(I1,I2,I3,pc))*p0;
  pS=u(I1,I2,I3,pc);

  const int numberOfEquations=numberOfSpecies_+1;   // number of equations
  
  Range R(0,numberOfEquations-1);    // unknowns are (T,Y_0,Y_1,...)
  Range S0(0,numberOfSpecies_-1);    // species, with base 0
  Range S1(1,numberOfSpecies_);      // species, with base 1
  Range S(sc,numberOfSpecies_+sc-1); // species, with base sc

  RealArray scaling(S0);
  scaling=1.;

  RealArray y(I1,I2,I3,S0), correction(I1,I2,I3,R), source(I1,I2,I3,R), sy(I1,I2,I3,R,R);
  RealArray r(I1,I2,I3);

  RealArray f(R), fx(R,R), dx(R);
  // RealArray fx2(R,R), res2(R);
  intArray ipvt(R);
  RealArray work(R);
  real rcond;
  int job=0;

  real speciesEpsilon=REAL_MIN*10.;

  y(I1,I2,I3,S0)=u(I1,I2,I3,S); // y has base zero! used later

  // rhs.display("Here is rhs");

  // ********************************
  // ***** Newton iterations ********
  // ********************************
  real newtonTolerance = max( 1.e-5, SQRT(REAL_EPSILON*10.)/10.);  // take sqrt as this is "old" error

  const real dtY =equilibriumReaction ? 1.e3 : dt;
  
//  newtonTolerance = 1.e-4;
  for( int it=0; it<10; it++ )
  {

    // Compute the chemistry source terms and their Jacobian (sy)
    //  source =  [ 1./(rho*cv) * sum{ (R_i T - h_i ) \sigma  ]
    //            [ sigma_0/rho                               ]
    //            [ sigma_1/rho                               ]
    //            [ sigma_2/rho                               ]
    //            [   ...                                     ]
    // sy = d{source}/d{T,Y_0,Y_1,...}
    chemistrySourceAndJacobian(pS,teS,y, rhoS,source,sy );

    source(I1,I2,I3,0)=(rhs(I1,I2,I3,0)-teS(I1,I2,I3))*te0 + dt*source(I1,I2,I3,0); // T equation
    int n;
    for( n=1; n<numberOfEquations; n++ )
      source(I1,I2,I3,n)=rhs(I1,I2,I3,n)-y(I1,I2,I3,n-1) +  dtY*source(I1,I2,I3,n);  // Y equations
      
//     if( FALSE )
//     {
//       source(I1,I2,I3,0)*=(1./te0);  // ***** scale for output ****
//       display(source,"source","%9.1e");
//       exit (1);
//     }
    
      
//    sigma*=l0/(rho0*u0)*sigmaFactor;
//    sy*=l0/u0*sigmaFactor;            // **** fix ****** scale above

    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
        for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
        {
          // compute the current residual
          // * f(0)=rhs(i1,i2,i3,0) - te(i1,i2,i3)*(1./te0) + dt*source(i1,i2,i3,0);        // T equation, scaled
          f(0)=source(i1,i2,i3,0);        // T equation, unscaled
	  for( n=1; n<numberOfEquations; n++ )
	    f(n)=source(i1,i2,i3,n);  // Y equations

	  // fx = I - dt*sy
	  for( int n1=0; n1<numberOfEquations; n1++ )
	  {
            real dtBE = n1==0 ? dt : dtY;
	    for( int n2=0; n2<numberOfEquations; n2++ )
	      fx(n1,n2)=(-dtBE)*sy(i1,i2,i3,n1,n2);
	    fx(n1,n1)+=1.;
	  }
          // if( debug & 4 && i1==I1.getBase() && it==0 )
          if( debug & 4 && i1==I1.getBase() && it==0 )
	  {
            // sigma.display("Here is sigma");
            fx.display("Here is fx");
            f.display("Here is f");
	  }
	  // factor matrix
          // fx2=fx; // save matrix for checking *****
	  GECO( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0));
          if( debug & 1 && i1==I1.getBase() && it>=0 )
	  {
  	    cout << "condition number = " << rcond << ", max(residual)=" << max(abs(f)) << endl;
	  }
	  // solve fx*dx = f
	  dx=f;
	  GESL( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0), dx(0), job);

/* ----
          if( debug & 1 && i1==I1.getBase() && it>=0 )
	  {
            // check if we solved: fx2*dx = f
            for( int n1=0; n1<numberOfEquations; n1++ )
	    {
              res2(n1)=f(n1);
              for( int n2=0; n2<numberOfEquations; n2++ )
                res2(n1)-=fx2(n1,n2)*dx(n2);
	    }
  	    cout << "max(Ax-b)=" << max(abs(res2)) << endl;
	  }
-------- */
	  real maxDx = max(fabs(dx(S1))/scaling);
          const real maxCorrection=.4;
	  if( maxDx> maxCorrection )
	  {
	    printf("damping Newton..\n");
	    dx(S1)=min(maxCorrection*scaling,max(-maxCorrection*scaling,dx(S1)));
	  }
	  for( n=0; n<numberOfEquations; n++ )
            correction(i1,i2,i3,n)=dx(n);
	}
      }
    }
    correction(I1,I2,I3,0)*=(1./te0);  // scale T correction
    
    teS+=correction(I1,I2,I3,0);  // *te0;
    u(I1,I2,I3,S)=y(I1,I2,I3,S0)+correction(I1,I2,I3,S1);
    y(I1,I2,I3,S0)=max(u(I1,I2,I3,S),speciesEpsilon);

    real tError= max(abs(correction(I1,I2,I3,0)));
    real error = max(abs(correction(I1,I2,I3,S1)));
    
    cout << "Newton: iteration = " << it << ", max(correction(Y)) = " << error 
         << ", max(correction(T)) = " << tError << ", tolerance=" << newtonTolerance << endl;
    if( debug & 4 )
      correction.display("here is correction");

    if( FALSE )
    {
      // Scale species to sum to one:
      r=1./sum(y(I1,I2,I3,S0),3);   // r = 1./( y(0)+y(1)+... )  (should be 1)
      for( int s=0; s<numberOfSpecies_; s++ )
	y(I1,I2,I3,s)*=r;
    }

    if( tError< newtonTolerance )
      break;
  }

  // Scale species to sum to one:
  r=1./sum(y(I1,I2,I3,S0),3);   // r = 1./( y(0)+y(1)+... )  (should be 1)
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    y(I1,I2,I3,s)*=r;

  // compute final rho??
  r=0.;
  for( s=0; s<numberOfSpecies_; s++ )
    r(I1,I2,I3)+=y(I1,I2,I3,s)*(mp.R/molecularWeight(s));          // this is R:

  rhoS(I1,I2,I3)= (p0/(te0*rho0))*pS(I1,I2,I3)/(r(I1,I2,I3)*teS(I1,I2,I3));

  // *  u(I1,I2,I3,rc)*=1./rho0;
  // * u(I1,I2,I3,tc)*=1./te0;
  u(I1,I2,I3,S)=y(I1,I2,I3,S0);
  
  return 0;
}



//\begin{>>ReactionsInclude.tex}{\subsection{solveImplicitForRTYGivenP}}
int Reactions::
solveImplicitForYGivenRTP(RealArray & u, 
			  const RealArray & rhs,
			  const int & rc, const int & pc, const int & tc, const int & sc,
			  const Index & I1, 
			  const Index & I2, 
			  const Index & I3,
			  const real & dt_,
                          bool equilibriumReaction /* = FALSE */  )
// ======================================================================================  
// /Description:
// This function solves species equations for $(Y_i)$ given the density, temperature and pressure.
//
// We solve the following nonlinear system of equations (obtained from a backward-Euler like discretization
//  of the equations for $Y_i$:
// \begin{align*}
//       Y_i - dt [ 1./(\rho)  \sigma ]  &= RHS(Y_i)  \qquad i=0,1,...,  \\
// \end{align*}
//
//  The unknowns are $(Y_i)$. The density $\rho$, temperature $T$ and the
//  pressure, $p$, at the new time level is assumed to be known.
//
//  /u (input/output) : solution vector:
//              u(I1,I2,I3,rc) : rho
//              u(I1,I2,I3,pc) : pressure
//              u(I1,I2,I3,tc) : temperature
//              u(I1,I2,I3,sc+s)  s=0,...,numberOfSpecies-1 : Guess for species on input, new values on output.
//  /rhs (input) : right-hand-side vector:
//              rhs(I1,I2,I3,s)  s=0,...,numberOfSpecies-1 : Right hand side to species equations.
// /rc,pc,tc,sc : indicate the positions of the variables in u
// /I1,I2,I3 (input) : indicate which points to compute
// /dt (input) : dt in the above equations
// /equilibriumReaction (input) : if true, then the reactions are assumed to be in equilibrium
//\end{ReactionsInclude.tex}  
// ======================================================================================  
{

  // * u(I1,I2,I3,rc)*=rho0; // compute unscaled rho
 // *  u(I1,I2,I3,tc)*=te0;  // compute unscaled T

  const RealArray & rho = u(I1,I2,I3,rc);
  const RealArray & te  = u(I1,I2,I3,tc);
//   RealArray p(I1,I2,I3);   
//   p= (pressureLevel+u(I1,I2,I3,pc))*p0;

  const int numberOfEquations=numberOfSpecies_;   // number of equations
  
  Range R(0,numberOfEquations-1);    // unknowns are (Y_0,Y_1,...)
  Range S0(0,numberOfSpecies_-1);    // species, with base 0
  //  Range S1(1,numberOfSpecies_);      // species, with base 1
  Range S(sc,numberOfSpecies_+sc-1); // species, with base sc

  RealArray scaling(S0);
  scaling=1.;

  RealArray y(I1,I2,I3,S0), correction(I1,I2,I3,R), source(I1,I2,I3,R), sy(I1,I2,I3,R,R);
  RealArray r(I1,I2,I3);
  r=1./rho;
  

  RealArray f(R), fx(R,R), dx(R);
  intArray ipvt(R);
  RealArray work(R);
  real rcond;
  int job=0;

  real speciesEpsilon=REAL_MIN*10.;

  y(I1,I2,I3,S0)=u(I1,I2,I3,S); // y has base zero! used later

  // rhs.display("Here is rhs");

  // ********************************
  // ***** Newton iterations ********
  // ********************************

  // * bool wasUsingScaledVariables=usingScaledVariables(); // save current value
  // * useScaledVariables(FALSE); // *** this sets rho0=1, te0=1,... ***

  const real dt=equilibriumReaction ? 1.e3 : dt_;
  const real dt0=dt/rho0;
  
  real newtonTolerance = max( 1.e-5, SQRT(REAL_EPSILON*10.)/10.);  // take sqrt as this is "old" error
  for( int it=0; it<10; it++ )
  {
    // Compute the chemistry source terms and their Jacobian (sy)
    //  source =  [ 1./(rho*cv) * sum{ (R_i T - h_i ) \sigma  ]
    //            [ sigma_0/rho                               ]
    //            [ sigma_1/rho                               ]
    //            [ sigma_2/rho                               ]
    //            [   ...                                     ]
    // sy = d{source}/d{T,Y_0,Y_1,...}
    // chemistrySourceAndJacobian(p,te,y, rho,source,sy );
    chemicalSource(rho,te,y,source,sy);

    int n;
    for( n=0; n<numberOfEquations; n++ )
      source(I1,I2,I3,n)=rhs(I1,I2,I3,n)-y(I1,I2,I3,n) +  dt0*source(I1,I2,I3,n)*r;  // Y equations
  
//     if( FALSE )
//     {
//       display(source,"source","%9.1e");
//       exit (1);
//     }

//     int n;
//     for( n=0; n<numberOfEquations; n++ )
//       source(I1,I2,I3,n)*=r;   // divide by rho ********
	  
    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
        for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
        {
          // compute the current residual
	  for( n=0; n<numberOfEquations; n++ )
	    f(n)=source(i1,i2,i3,n);

	  // fx = I - dt*sy
	  for( int n1=0; n1<numberOfEquations; n1++ )
	  {
	    for( int n2=0; n2<numberOfEquations; n2++ )
	      fx(n1,n2)=(-dt)*sy(i1,i2,i3,n1,n2);
	    fx(n1,n1)+=1.;
	  }
	  
          // if( debug & 4 && i1==I1.getBase() && it==0 )
          if( debug & 4 && i1==I1.getBase() && it==0 )
	  {
            // sigma.display("Here is sigma");
            fx.display("Here is fx");
            f.display("Here is f");
	  }
	  // factor matrix
	  GECO( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0));
          if( debug & 1 && i1==I1.getBase() && it>=0 )
	  {
  	    cout << "condition number = " << rcond << ", max(residual)=" << max(abs(f)) << endl;
	  }
	  // solve fx*dx = f
	  dx=f;
	  GESL( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0), dx(0), job);

	  real maxDx = max(fabs(dx(S0))/scaling);
          const real maxCorrection=.4;
	  if( maxDx> maxCorrection )
	  {
	    printf("damping Newton..\n");
	    dx(S0)=min(maxCorrection*scaling,max(-maxCorrection*scaling,dx(S0)));
	  }
	  for( n=0; n<numberOfEquations; n++ )
            correction(i1,i2,i3,n)=dx(n);
	}
      }
    }
    u(I1,I2,I3,S)=y(I1,I2,I3,S0)+correction(I1,I2,I3,S0);
    y(I1,I2,I3,S0)=max(u(I1,I2,I3,S),speciesEpsilon);

    real error = max(abs(correction(I1,I2,I3,S0)));
    
    cout << "Newton: iteration = " << it << ", max(correction(Y)) = " << error << endl;
    if( debug & 4 )
      correction.display("here is correction");

    if( error< newtonTolerance )
      break;
  }

  // Scale species to sum to one:
  r=1./sum(y(I1,I2,I3,S0),3);   // r = 1./( y(.,.,.,0)+y(.,.,.,1)+... )  (should be 1)
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    y(I1,I2,I3,s)*=r;

 // *  useScaledVariables(wasUsingScaledVariables);  // turn on scaling, resets rh0,te0,...

  // * u(I1,I2,I3,rc)*=1./rho0; // reset
  // * u(I1,I2,I3,tc)*=1./te0;  // reset
  u(I1,I2,I3,S)=y(I1,I2,I3,S0);
  

  return 0;
}


//\begin{>>ReactionsInclude.tex}{\subsection{solveImplicitForRTYGivenP}}
int Reactions::
solveImplicitForPTYGivenR(RealArray & u, 
			  const RealArray & rhs,
			  const int & rc, const int & pc, const int & tc, const int & sc,
			  const Index & I1, 
			  const Index & I2, 
			  const Index & I3,
			  const real & dt,
                          bool equilibriumReaction /* = FALSE */ )
// ======================================================================================  
// /Description:
// This function solves the temperature eqn, the Species equations and the equation of state
//   for $(p,T,Y_i)$ given the pressure
//
// We solve the following nonlinear system of equations (obtained from a backward-Euler like discretization
//  of the equations for T and $Y_i$:
// \begin{align*}
//       T   - dt [ 1./(\rho cv)   \sum_i (R_i T - h_i ) \sigma ] &= RHS_(T) \\
//       Y_i - dt [ 1./(\rho)  \sigma ]                          &= RHS(Y_i)  \qquad i=0,1,...,  \\
//       p &= \rho R T   
// \end{align*}
//
//  The unknowns are $(p,T,Y_i)$. The density, $rho$, at the new time level is assumed to be known.
//
//  /u (input/output) : solution vector:
//              u(I1,I2,I3,rc) : The correct rho
//              u(I1,I2,I3,pc) : p on output
//              u(I1,I2,I3,tc) : guess for T on input, new vaule for T on output.
//              u(I1,I2,I3,sc+s)  s=0,...,numberOfSpecies-1 : Guess for species on input, new values on output.
//  /rhs (input) : right-hand-side vector:
//              rhs(I1,I2,I3,0) : The right hand side to the Temperature equation.
//              rhs(I1,I2,I3,s+1)  s=0,...,numberOfSpecies-1 : Right hand side to species equations.
// /rc,pc,tc,sc : indicate the positions of the variables in u
// /I1,I2,I3 (input) : indicate which points to compute
// /dt (input) : dt in the above equations
// /equilibriumReaction (input) : if true, then the reactions are assumed to be in equilibrium
//\end{ReactionsInclude.tex}  
// ======================================================================================  
{

  // display(u,"solveImplicitForPTYGivenR: u on input");

  if( debug & 4 )
    display(u,"solveImplicitForPTYGivenR: u on input after scaling rho,T");

  const RealArray & rhoS = u(I1,I2,I3,rc);
  const RealArray & teS0  = u(I1,I2,I3,tc); RealArray & teS  = (RealArray &)teS0;
  const RealArray & pS0   = u(I1,I2,I3,pc); RealArray & pS   = (RealArray &) pS0;

  // p= (pressureLevel+u(I1,I2,I3,pc))*p0;

  const int numberOfEquations=numberOfSpecies_+1;   // number of equations
  
  Range R(0,numberOfEquations-1);    // unknowns are (T,Y_0,Y_1,...)
  Range S0(0,numberOfSpecies_-1);    // species, with base 0
  Range S1(1,numberOfSpecies_);      // species, with base 1
  Range S(sc,numberOfSpecies_+sc-1); // species, with base sc

  RealArray scaling(S0);
  scaling=1.;

  RealArray y(I1,I2,I3,S0), correction(I1,I2,I3,R), source(I1,I2,I3,R), sy(I1,I2,I3,R,R);
  RealArray r(I1,I2,I3);

  RealArray f(R), fx(R,R), dx(R);
  intArray ipvt(R);
  RealArray work(R);
  real rcond;
  int job=0;

  real speciesEpsilon=REAL_MIN*10.;

  y(I1,I2,I3,S0)=u(I1,I2,I3,S); // y has base zero! used later

  // rhs.display("Here is rhs");

  // ********************************
  // ***** Newton iterations ********
  // ********************************
  real newtonTolerance = max( 1.e-5, SQRT(REAL_EPSILON*10.)/10.);  // take sqrt as this is "old" error

  const real dtY =equilibriumReaction ? 1.e3 : dt;

  for( int it=0; it<10; it++ )
  {

    // Compute the chemistry source terms and their Jacobian (sy)
    //  source =  [ 1./(rho*cv) * sum{ (R_i T - h_i ) \sigma  ]
    //            [ sigma_0/rho                               ]
    //            [ sigma_1/rho                               ]
    //            [ sigma_2/rho                               ]
    //            [   ...                                     ]
    // sy = d{source}/d{T,Y_0,Y_1,...}
   
    // To use chemistrySourceAndJacobian we must first compute p from p=r*R*T
    pFromRTY( rhoS,teS,y, pS);  // **** fix this **** could be more efficient

    if( debug & 4 )
      display(pS,"solveImplicitForPTYGivenR: p from p=r*R*T");

    chemistrySourceAndJacobian(pS,teS,y, rhoS,source,sy ); // ** this routine expects p on input, may change rho!
    if( debug & 4 )
    {
      display(rhoS,"solveImplicitForPTYGivenR: rho after chemistrySourceAndJacobian");
      display(source,"solveImplicitForPTYGivenR: source after chemistrySourceAndJacobian");
    }
    
    source(I1,I2,I3,0)=(rhs(I1,I2,I3,0)-teS(I1,I2,I3))*te0 + dt*source(I1,I2,I3,0); // T equation
    int n;
    for( n=1; n<numberOfEquations; n++ )
      source(I1,I2,I3,n)=rhs(I1,I2,I3,n)-y(I1,I2,I3,n-1) +  dtY*source(I1,I2,I3,n);  // Y equations
      
    if( FALSE )
    {
      source(I1,I2,I3,0)*=(1./te0);  // ***** scale for output ****
      display(source,"source","%9.1e");
      exit (1);
    }

    for( int i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
      for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
      {
        for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
        {
          // compute the current residual
          f(0)=source(i1,i2,i3,0);        // T equation
	  for( n=1; n<numberOfEquations; n++ )
	    f(n)=source(i1,i2,i3,n);  // Y equations

	  // fx = I - dt*sy
	  for( int n1=0; n1<numberOfEquations; n1++ )
	  {
            real dtBE = n1==0 ? dt : dtY;
	    for( int n2=0; n2<numberOfEquations; n2++ )
	      fx(n1,n2)=(-dtBE)*sy(i1,i2,i3,n1,n2);
	    fx(n1,n1)+=1.;
	  }
          // if( debug & 4 && i1==I1.getBase() && it==0 )
          if( debug & 4 && i1==I1.getBase() && it==0 )
	  {
            // sigma.display("Here is sigma");
            fx.display("Here is fx");
            f.display("Here is f");
	  }
	  // factor matrix
          // fx2=fx; // save matrix for checking *****
	  GECO( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0),rcond,work(0));
          if( debug & 1 && i1==I1.getBase() && it>=0 )
	  {
  	    cout << "condition number = " << rcond << ", max(residual)=" << max(abs(f)) << endl;
	  }
	  // solve fx*dx = f
	  dx=f;
	  GESL( fx(0,0), numberOfEquations, numberOfEquations, ipvt(0), dx(0), job);

/* ----
          if( debug & 1 && i1==I1.getBase() && it>=0 )
	  {
            // check if we solved: fx2*dx = f
            for( int n1=0; n1<numberOfEquations; n1++ )
	    {
              res2(n1)=f(n1);
              for( int n2=0; n2<numberOfEquations; n2++ )
                res2(n1)-=fx2(n1,n2)*dx(n2);
	    }
  	    cout << "max(Ax-b)=" << max(abs(res2)) << endl;
	  }
-------- */
	  real maxDx = max(fabs(dx(S1))/scaling);
          const real maxCorrection=.4;
	  if( maxDx> maxCorrection )
	  {
	    printf("damping Newton..\n");
	    dx(S1)=min(maxCorrection*scaling,max(-maxCorrection*scaling,dx(S1)));
	  }
	  for( n=0; n<numberOfEquations; n++ )
            correction(i1,i2,i3,n)=dx(n);
	}
      }
    }
    correction(I1,I2,I3,0)*=(1./te0);
    
    teS+=correction(I1,I2,I3,0);
    u(I1,I2,I3,S)=y(I1,I2,I3,S0)+correction(I1,I2,I3,S1);
    y(I1,I2,I3,S0)=max(u(I1,I2,I3,S),speciesEpsilon);

    real tError= max(abs(correction(I1,I2,I3,0)));
    real error = max(abs(correction(I1,I2,I3,S1)));
    
    cout << "Newton: iteration = " << it << ", max(correction(Y)) = " << error 
        << ", max(correction(T)) = " << tError << endl;
    if( debug & 4 )
      correction.display("here is correction");

    if( tError< newtonTolerance )
      break;
  }

  // Scale species to sum to one:
  r=1./sum(y(I1,I2,I3,S0),3);   // r = 1./( y(0)+y(1)+... )  (should be 1)
  int s;
  for( s=0; s<numberOfSpecies_; s++ )
    y(I1,I2,I3,s)*=r;

//  useScaledVariables(wasUsingScaledVariables);  // turn on scaling, resets rh0,te0,...

  // compute output (scaled) pressure
  // p= (pressureLevel+u(I1,I2,I3,pc))*p0;
//  u(I1,I2,I3,pc)=p*(1./p0)-pressureLevel;
  
//  u(I1,I2,I3,rc)*=1./rho0;
//  u(I1,I2,I3,tc)*=1./te0;
  u(I1,I2,I3,S)=y(I1,I2,I3,S0);
  
  return 0;
}

