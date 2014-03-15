// =======================================================================
// This class knows how to solve problems related to the motion of
// a solid piston next to a fluid
// =======================================================================

#include "FluidPiston.h"


static real Afsr,Bfsr,Cfsr,Dfsr,gam; // global vars for functions resFSR and expansionFanFSR below:

#define zeroin EXTERN_C_NAME(zeroin)
#define fluidSolidRiemannSolution EXTERN_C_NAME(fluidsolidriemannsolution)
extern "C"
{
  // zeroin: function to compute the zero of a function on an interval -- f is assumed to change sign
  real zeroin(real & ax, real & bx, real (*f) (const real&), real & tol);

  // 
  // Evaluate the Mach number equation for the fluid-solid Riemann problem
  // (see papers/fsi/notes.pdf)
  //
  real resFSR(const real & m)
  { 
    return Afsr*m*m + Bfsr - 2.*(1.-m*m)/( (gam+1.)*m );
  }

  // 
  // Evaluate the p equation for the fluid-solid Riemann problem expansion fan case 
  // (see papers/fsi/notes.pdf)
  real expansionFanFSR(const real & p)
  { 
    return Afsr*pow(p,Bfsr) + Cfsr*p + Dfsr;
  }

  // this routine can be called from Fortran:
  void fluidSolidRiemannSolution( const real solid[], const real fluid[], real fsr[] )
  {
    FluidPiston::fluidSolidRiemannProblem(solid,fluid,fsr);
  }


}


FluidPiston::
FluidPiston()
{
}

FluidPiston::
~FluidPiston()
{
}


// =========================================================================
/// \brief Solve the fluid-solid Riemann problem
/// 
/// \solid[] (input) : solid state solid[0..3]=[rho,v,stress,cp]
///         cp = solid speed of sound (for p-waves)
/// \fluid[] (input) : fluid state fluid[0..4]=[rho,v,p,gamma,pOffset]
/// \fsr[0..] (output) : state next to the interface,
///            fsr[0..2] = [rhoFluid,v,pFluid, Mfsr,Sfsr]
///            Mfsr = FSR Mach number, Sfsr = reflected shock speed
/// See also papers/fsi/elasticPiston.tex for the derivation of this equation.
// =========================================================================
int FluidPiston::
fluidSolidRiemannProblem( const real solid[], const real fluid[], real fsr[] )
{
  int debug=0; // set to 1 for debug output

  real rf      = fluid[0];
  real vf      = fluid[1];
  real pf      = fluid[2];  
  gam          = fluid[3];       // global variable
  real pOffset = fluid[4];
  real af = sqrt(gam*pf/rf);

  real rs     = solid[0];
  real vs     = solid[1];
  real sigmas = solid[2];
  real as     = solid[3];
  

  //% We first solve the equation for "M" (see fsi/notes)
  //%  A*M^2 + B = G(M) = 2(1-M^2)/( (gam+1)*M )
  real zf =rf*af, zs=rs*as,  gp1=gam+1.; 
  // Afsr and Bfsr are global vars used in function resFSR
  Afsr = (2./gp1)*(zf/zs);
  Bfsr =((zf/zs)/gam)*( 1. - 2.*gam/gp1 + (sigmas - pOffset)/pf ) + (vf-vs)/af;

  // Mfsr = fzero( 'resFSR',[1e-10,10] );
  real tol=1.e-15;
  real Mmin=1.e-10, Mmax=10.;
  real Mfsr = zeroin( Mmin,Mmax,resFSR,tol );

  real K = ((gam-1.)*Mfsr*Mfsr+2.)/((gam+1.)*Mfsr*Mfsr);
  real rfsr = rf/K; 
  real pfsr=pf*( 1. + (2.*gam/(gam+1.))*( Mfsr*Mfsr-1.) );
  real afsr=sqrt(gam*pfsr/rfsr);
  real Sfsr=Mfsr*af+vf;
  real vfsr= Sfsr+ K*(vf-Sfsr);
  real vfsrf = vf + af*(2.*(Mfsr*Mfsr-1.)/( (gam+1.)*Mfsr )); 
  real efsr = pfsr/(gam-1.)+.5*rfsr*vfsr*vfsr;
  real vsfsr=vfsr;
  real sigmasfsr=-(pfsr-pOffset); 
  real sfsr = sigmas + zs*(vsfsr-vs); // this should also be sigmasfsr
  if( debug )
  {
    printf("*** FluidPiston::fluid-solid Riemann: K=%8.2e\n",K);

    real ef = pf/(gam-1.)+.5*rf*vf*vf;

    printf("fluid-solid Riemann: Mfsr = %8.2e, Sfsr=%8.2e, [r,v,p,a]=[%9.2e,%9.2e,%9.2e,%9.2e]\n",
	   Mfsr,Sfsr,rfsr,vfsr,pfsr,afsr);
    printf("fluid-solid Riemann: [vs,sigmas]=[%9.2e,%9.2e] or sigmas=%9.2e\n",vsfsr,sigmasfsr,sfsr);

    printf("fluid:IC: Sfsr=%4.2f, vfsrf=%8.2e\n",Sfsr,vfsrf);
    printf(" sigmas + (p-pOffset) = %9.2e\n",sigmasfsr+pfsr-pOffset);
    printf(" v-vs + (p-pOff+sig0)/zs = %9.2e\n",vfsr-vs +( pfsr-pOffset+sigmas)/zs);
    printf(" -(p-pOff+sig0)/zs +vs-S -K*(v0-S)= %9.2e\n",-( pfsr-pOffset+sigmas)/zs + vs-Sfsr-K*(vf-Sfsr));

    real lhs = ( pf*( 1.+(2.*gam/gp1)*( Mfsr*Mfsr-1.) ) + sigmas-pOffset )/zs+ vf-vs + Mfsr*af;
    real rhs = K*( Mfsr*af ); 
    printf("(10) : lhs=%9.2e rhs=%9.2e lhs-rhs=%9.2e\n",lhs,rhs,lhs-rhs);

    printf(" (v-S)/(v0-S) -K = %9.2e\n",(vfsr-Sfsr)/(vf-Sfsr)-K);
    printf(" (sigmas-s0)/[vs] - rs0*as = %9.2e\n",(sigmasfsr-sigmas)/(vsfsr-vs)- zs);
    printf("          [r*v]/[r] - S = %8.2e\n",(rfsr*vfsr-rf*vf)/(rfsr-rf)-Sfsr);
    printf("          [r*v^2+p]/[r*v] - S = %8.2e\n",(rfsr*vfsr*vfsr+pfsr-(rf*vf*vf+pf))/(rfsr*vfsr-rf*vf)-Sfsr);
    printf("          [E*v+p*v]/[E] - S = %8.2e\n",(efsr*vfsr+pfsr*vfsr-(ef*vf+pf*vf))/(efsr-ef)-Sfsr);
    
  }
  

  if( Mfsr<1. )
  {
    
    // there is an expansion fan in the fluid

    // z = Afsr*p^Bfsr + Cfsr*p + Dfsr;

    Afsr = (2./(gam-1.))*af*pow( pf ,(-(gam-1.)/(2.*gam))); 
    Bfsr = (gam-1.)/(2.*gam);
    Cfsr = 1./zs;
    Dfsr = -( (2./(gam-1.))*af + vs - vf - (sigmas- pOffset)/zs ); 

    // pFan = fzero( 'expansionFanFSR',[1e-10,10] );
    real pMin=1.e-10, pMax=10.;
    real pFan = zeroin( pMin,pMax,expansionFanFSR,tol );

    real rFan = rf*pow( (pFan/pf) , (1./gam) );
    real aFan = af*pow( (pFan/pf) , ( (gam-1.)/(2.*gam) ) );
    real vFan = (2./(gam-1.))*( aFan-af ) + vf; 

    if( debug )
    {
      printf("fluid:IC:FAN: [r,v,p]=[%12.6e,%12.6e,%12.6e]\n",rFan,vFan,pFan);
      printf("         shock values=[%12.6e,%12.6e,%12.6e]\n",rfsr,vfsr,pfsr);
      printf(" residual in v-vs + (p-pa+sig)/zs = %8.2e\n",vFan-vs+(pFan+sigmas-pOffset)/zs); 
      printf(" aFan-sqrt(gam*pFan/rfan)=%8.2e\n",aFan-sqrt(gam*pFan/rFan));
    }
      

    rfsr=rFan;
    vfsr=vFan;
    pfsr=pFan;
    efsr = pfsr/(gam-1.)+.5*rfsr*vfsr*vfsr; 
    afsr=aFan; 
    vsfsr=vfsr;
    sigmasfsr=-(pfsr-pOffset); 
    
  }

  fsr[0]=rfsr;
  fsr[1]=vfsr;
  fsr[2]=pfsr;
  fsr[3]=Mfsr;
  fsr[4]=Sfsr;  // reflected shock speed
  
  return 0;
  
}

