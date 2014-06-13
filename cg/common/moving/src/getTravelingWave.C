#include <iostream>
#include <complex>
#include <stdio.h>
using namespace std;

// abort is in here:
#include <stdlib.h>

#define OV_ABORT(label){\
printf("Error occured in file %s line %d.\n",__FILE__,__LINE__);	\
   ::abort(); }

typedef complex<double> cmplx;

// ===============================================================================================
/// \brief Evaluate the dispersion relation for the viscous fluid and elastic shell
// ===============================================================================================
cmplx 
evalDispersionViscousShell( const cmplx & omega, const double & k, const double & Ks, const double & T, 
                     const double & rhos, const double & hs,
                     const double & mu, const double & rho, const double & H )
{
  cmplx i(0,1);


  cmplx z = sqrt(k*k + (-i) * rho * omega / mu) * (sqrt(k*k + (-i) * rho * omega / mu) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - k * sinh(k * H) * cosh(sqrt(k*k + (-i) * rho * omega / mu) * H)) * rho * omega*omega + k * (2. * sqrt(k*k + (-i) * rho * omega / mu) * k * (cosh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - 1.) - (2. * k*k + (-i) * rho * omega / mu) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * sinh(k * H)) * (Ks + k*k * T - rhos * hs * omega*omega);

//  printf("eval: z=(%e,%e), omega=(%18.14e,%18.14e) k=%e, Ks=%e, T=%e, rhos=%e, hs=%e, mu=%e, rho=%e, H=%e\n",
//	 real(z),imag(z),real(omega),imag(omega),k,Ks,T,rhos,hs,mu,rho,H);

  return z;
  
}
// ===============================================================================================
/// \brief Evaluate the derivative of the dispersion relation for the viscous fluid and elastic shell
// ===============================================================================================
cmplx 
evalDispersionViscousShellDeriv( const cmplx & omega, const double & k, const double & Ks, const double & T, 
                     const double & rhos, const double & hs,
                     const double & mu, const double & rho, const double & H )
{
  cmplx i(0,1);

cmplx z = (-0.1e1 / 0.2e1*i) * pow( (k*k + (-i) * rho * omega / mu),(-0.1e1 / 0.2e1)) * (sqrt(k*k + (-i) * rho * omega / mu) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - k * sinh(k * H) * cosh(sqrt(k*k + (-i) * rho * omega / mu) * H)) * rho*rho * omega*omega / mu + sqrt(k*k + (-i) * rho * omega / mu) * ((-0.1e1 / 0.2e1*i) * pow( (k*k + (-i) * rho * omega / mu),(-0.1e1 / 0.2e1)) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) * rho / mu + (-0.1e1 / 0.2e1*i) * cosh(sqrt(k*k + (-i) * rho * omega / mu) * H) * H * rho / mu * cosh(k * H) + (0.1e1 / 0.2e1*i) * k * sinh(k * H) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * pow((k*k + (-i) * rho * omega / mu),(-0.1e1 / 0.2e1)) * H * rho / mu) * rho * omega*omega + 2. * sqrt(k*k + (-i) * rho * omega / mu) * (sqrt(k*k + (-i) * rho * omega / mu) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - k * sinh(k * H) * cosh(sqrt(k*k + (-i) * rho * omega / mu) * H)) * rho * omega + k * ((-i) * pow((k*k + (-i) * rho * omega / mu),(-0.1e1 / 0.2e1)) * k * (cosh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - 1.) * rho / mu + (-i) * k * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * H * rho / mu * cosh(k * H) + (i) * rho / mu * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * sinh(k * H) + (0.1e1 / 0.2e1*i) * (2 * k*k + (-i) * rho * omega / mu) * cosh(sqrt(k*k + (-i) * rho * omega / mu) * H) * pow((k*k + (-i) * rho * omega / mu), (-0.1e1 / 0.2e1)) * H * rho / mu * sinh(k * H)) * (Ks + k*k * T - rhos * hs * omega*omega) - 2. * k * (2. * sqrt(k*k + (-i) * rho * omega / mu) * k * (cosh(sqrt(k*k + (-i) * rho * omega / mu) * H) * cosh(k * H) - 1.) - (2. * k*k + (-i) * rho * omega / mu) * sinh(sqrt(k*k + (-i) * rho * omega / mu) * H) * sinh(k * H)) * rhos * hs * omega;

  return z;
  
}

#define dispersionShell2 dispersionshell2_
#define dispersionShell2deriv dispersionshell2deriv_
#define dispersionInviscidAcoustic dispersioninviscidacoustic_
#define dispersionInviscidAcousticDeriv dispersioninviscidacousticderiv_
#define dispersionViscousAcoustic dispersionviscousacoustic_
#define dispersionViscousAcousticDeriv dispersionviscousacousticderiv_
#define dispersionViscousElastic dispersionviscouselastic_
#define dispersionViscousElasticDeriv dispersionviscouselasticderiv_

extern "C"
{
  // fortran variables are passed by reference:
void dispersionShell2( double &wr,double &wi, double &detr, double &deti,
       const double &rho,const double &mu,const double &H,
       const double &k,const double &rhos,const double &hs,const double &Ks,const double &Ts, const double &eta1Varies );

void dispersionShell2deriv( double &wr,double &wi, double &detrp, double &detip,  
       const double &rho,const double &mu,const double &H,
       const double &k,const double &rhos,const double &hs,const double &Ks,const double &Ts, const double &eta1Varies );

void dispersionInviscidAcoustic( const double &wr, const double &wi, double &detr, double &deti,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs
       );

void dispersionInviscidAcousticDeriv( const double &wr, const double &wi, double &detrp, double &detip,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs
      );

void dispersionViscousAcoustic( const double &wr, const double &wi, double &detr, double &deti,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs,
      const double & u1Varies );

void dispersionViscousAcousticDeriv( const double &wr, const double &wi, double &detrp, double &detip,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs,
      const double & u1Varies );

void dispersionViscousElastic( const double &wr, const double &wi, double &detr, double &deti,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs,
      const double & u1Varies );

void dispersionViscousElasticDeriv( const double &wr, const double &wi, double &detrp, double &detip,  
       const double &rho,const double &mu,const double &H,
       const double &k, const double &rhos, const double &cp, const double &cs, const double &Hs,
      const double & u1Varies );

}


// ===============================================================================================
/// \brief Find a root to the dispersion relation for the viscous fluid and elastic shell
/// option
// ===============================================================================================
cmplx
findRootShellViscous( const int option,
                      const double & k, const double & Ks, const double & T, 
		      const double & rhos, const double & hs,
		      const double & mu, const double & rho, const double & H )
{

  int N = 100;  // number of continuation steps

  const double Pi = 4.*atan2(1.,1.);

  // initial guesss
  double Ks0    = 1.;
  double T0     = 1.;
  double rhos0  = 1.;
  double hs0    = 1.;
  double mu0    = 1/10.;
  double rho0   = 1.;
  double H0     = 1.;
  double k0     = 1.;
  cmplx omega0 = cmplx(.8323792222,-.2899391366);

  if( rhos>50. )
  {
    Ks0=0.;
    T0=100.;
    rhos0=100.;
    k0= 2.*Pi;
    mu0=.05;
    omega0=cmplx(6.2760362089e+00,-4.2991741480e-03);
  }
  else if( rhos>200. )
  {
    Ks0=0.;
    T0=1000.;
    rhos0=1000.;
    k0= 2.*Pi;
    mu0=.05;
    omega0=cmplx(6.2824696017e+00,-4.3086050534e-04);
  }
  

  cmplx omega = omega0;
  cmplx domega;
  cmplx ff,fp;
  double ffr,ffi,fpr,fpi;
  double eta1Varies;
  double ff0=1.;  // initial residual
  
  int maxNewtonIterations = 20; // max Newton iterations
  double tol = 1e-14;

  for( int it=1; it<N; it++ )
  {
    double alpha = it/(N-1.);
    
    double kn  = (1.-alpha)*k0 + alpha*k;
    double Ksn = (1.-alpha)*Ks0 + alpha*Ks;
    double Tn = (1.-alpha)*T0 + alpha*T;
    double rhosn = (1.-alpha)*rhos0 + alpha*rhos;
    double hsn = (1.-alpha)*hs0 + alpha*hs;
    double mun = (1.-alpha)*mu0 + alpha*mu;
    double rhon = (1.-alpha)*rho0 + alpha*rho;
    double Hn = (1.-alpha)*H0 + alpha*H;

    int nit;
    for( nit=0; nit<maxNewtonIterations; nit++ )
    {
      if( option==0 )
      {
	ff = evalDispersionViscousShell(  omega,kn,Ksn,Tn,rhosn,hsn,mun,rhon,Hn );
	fp = evalDispersionViscousShellDeriv( omega,kn,Ksn,Tn,rhosn,hsn,mun,rhon,Hn );
      }
      else
      {
        if( option==1 )
          eta1Varies=0.;
        else
          eta1Varies=1.;
        double omegar=real(omega);
        double omegai=imag(omega);
	
	dispersionShell2( omegar,omegai,ffr,ffi,  rhon,mun,Hn,kn,rhosn,hsn,Ksn,Tn, eta1Varies );
        ff=cmplx(ffr,ffi);
	dispersionShell2deriv( omegar,omegai,fpr,fpi,  rhon,mun,Hn,kn,rhosn,hsn,Ksn,Tn, eta1Varies );
        fp=cmplx(fpr,fpi);
	
      }
      ff0=max(ff0,abs(ff));
      domega = ff/fp;
      omega = omega-domega;

      // printf("Newton: nit=%i, omega=(%14.6e,%14.6e) domega=(%14.6e,%14.6e)\n",nit,real(omega),imag(omega),
      //	     real(domega),imag(domega));
      

      if( abs(domega) < tol )
	break;
    }
    if( nit>=maxNewtonIterations )
    {
      printf("findRootShellViscous::ERROR: Newton iterations did not converge\n");
    }
    
    printf("findRootShellViscous::%i Newton iterations |f(w)|/|f0|=%e, |dw|=%8.2e, k=%e, w=(%e,%e)\n",nit,abs(ff)/ff0,
	   abs(domega),kn, real(omega),imag(omega) );
  }

  return omega;
}


// ===============================================================================================
/// \brief Find a root to the dispersion relation for the invisidfluid and acoustic solid
/// option
// ===============================================================================================
cmplx
findRootInviscidAcoustic( const int option,
			 const double & k, 
			 const double & rhos, const double & Hs, const double & cp, const double & cs,
			 const double & mu, const double & rho, const double & H )
{

  int N = 100;  // number of continuation steps

  const double Pi = 4.*atan2(1.,1.);

  double Hs0    = .5;
  double cp0    = sqrt(3.);
  double cs0    = 1.;

  double mu0    = .0;
  double rho0   = 1000.;
  double H0     = 1.;
  double k0     = 2.*Pi;

  double rhos0  = 1e3;
  cmplx omega0 = cmplx(1.176997811985e+01,0.000000000000e+00);

  // here are different starting guesses:
  if( rhos>100. )
  {
    rhos0=10;
    omega0=cmplx(1.176997811985e+01,0.000000000000e+00);
  }
  else if(true || rhos>.5 )
  {
    rhos0  = 1.;
    omega0 = cmplx(1.6555576975e+01,0.0000000000e+00);
  }
//   else if( rhos>.5 )
//   {
//     rhos0=1.;
//     omega0=cmplx(7.8113842627e+00,-1.3404718121e+00);
//   }
//   else if( rhos>.2 )
//   {
//     rhos0=.1;
//     omega0=cmplx(2.3273221633e+00,-1.3810258454e+00);
//   }
//   else 
//   {
//     N=1000;
//     rhos0=.05;
//     omega0=cmplx(1.3778052143e+00,-1.2756801763e+00);
//   }
  
  cmplx omega = omega0;
  cmplx domega;
  cmplx ff,fp;
  double ffr,ffi,fpr,fpi;
  double ff0=1.;  // initial residual
  
  int maxNewtonIterations = 20; // max Newton iterations
  double tol = 1e-14;

  for( int it=1; it<N; it++ )
  {
    double alpha = it/(N-1.);
    
    double kn  = (1.-alpha)*k0 + alpha*k;
    double cpn = (1.-alpha)*cp0 + alpha*cp;
    double csn = (1.-alpha)*cs0 + alpha*cs;
    double rhosn = (1.-alpha)*rhos0 + alpha*rhos;
    double Hsn = (1.-alpha)*Hs0 + alpha*Hs;

    double mun = (1.-alpha)*mu0 + alpha*mu;
    double rhon = (1.-alpha)*rho0 + alpha*rho;
    double Hn = (1.-alpha)*H0 + alpha*H;

    int nit;
    for( nit=0; nit<maxNewtonIterations; nit++ )
    {
      double omegar=real(omega);
      double omegai=imag(omega);
	
      dispersionInviscidAcoustic( omegar,omegai,ffr,ffi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn );
      ff=cmplx(ffr,ffi);
      dispersionInviscidAcousticDeriv( omegar,omegai,fpr,fpi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn );
      fp=cmplx(fpr,fpi);

      ff0=max(ff0,abs(ff));
      domega = ff/fp;
      omega = omega-domega;

      // printf("Newton: nit=%i, omega=(%14.6e,%14.6e) domega=(%14.6e,%14.6e)\n",nit,real(omega),imag(omega),
      //	     real(domega),imag(domega));
      

      if( abs(domega) < tol )
	break;
    }
    if( nit>=maxNewtonIterations )
    {
      printf("findRootInviscidAcoustic::ERROR: Newton iterations did not converge\n");
    }
    
    printf("findRootInviscidAcoustic::%i Newton iterations |f(w)|/|f0|=%e, |dw|=%8.2e, k=%e, w=(%e,%e)\n",
       nit,abs(ff)/ff0,abs(domega),kn, real(omega),imag(omega) );
  }

  return omega;
}

// ===============================================================================================
/// \brief Find a root to the dispersion relation for the viscous fluid and acoustic solid
/// option
// ===============================================================================================
cmplx
findRootViscousAcoustic( const int option,
			 const double & k, 
			 const double & rhos, const double & Hs, const double & cp, const double & cs,
			 const double & mu, const double & rho, const double & H, const double & u1Varies )
{

  int N = 100;  // number of continuation steps

// initial guesss
//   double rhos0  = 1.;
//   double Hs0    = 4.;
//   double cp0    = 1.;

//   double mu0    = 1./5.;
//   double rho0   = 1.;
//   double H0     = 4.;
//   double k0     = 1.;
//   cmplx omega0 = cmplx( .7102319079,-0.1302413);

  const double Pi = 4.*atan2(1.,1.);

  double Hs0    = .5;
  double cp0    = sqrt(3.);
  double cs0    = 1.;

  double mu0    = .05;
  double rho0   = 1.;
  double H0     = 1.;
  double k0     = 2.*Pi;

  double rhos0  = 10.;
  cmplx omega0 = cmplx(1.1664627964e+01,-1.9887340588e-01);

  // here are different starting guesses:
  if( rhos>50. )
  {
    rhos0=500.;
    omega0=cmplx(1.2142862043e+01,-9.1400978013e-03);
  }
  else if( rhos>5. )
  {
    rhos0  = 10.;
    omega0 = cmplx(1.1664627964e+01,-1.9887340588e-01);
  }
  else if( rhos>.5 )
  {
    rhos0=1.;
    omega0=cmplx(7.8113842627e+00,-1.3404718121e+00);
  }
  else if( rhos>.2 )
  {
    rhos0=.1;
    omega0=cmplx(2.3273221633e+00,-1.3810258454e+00);
  }
  else 
  {
    N=1000;
    rhos0=.05;
    omega0=cmplx(1.3778052143e+00,-1.2756801763e+00);
  }
  
  cmplx omega = omega0;
  cmplx domega;
  cmplx ff,fp;
  double ffr,ffi,fpr,fpi;
  double ff0=1.;  // initial residual
  
  int maxNewtonIterations = 20; // max Newton iterations
  double tol = 1e-14;

  for( int it=1; it<N; it++ )
  {
    double alpha = it/(N-1.);
    
    double kn  = (1.-alpha)*k0 + alpha*k;
    double cpn = (1.-alpha)*cp0 + alpha*cp;
    double csn = (1.-alpha)*cs0 + alpha*cs;
    double rhosn = (1.-alpha)*rhos0 + alpha*rhos;
    double Hsn = (1.-alpha)*Hs0 + alpha*Hs;

    double mun = (1.-alpha)*mu0 + alpha*mu;
    double rhon = (1.-alpha)*rho0 + alpha*rho;
    double Hn = (1.-alpha)*H0 + alpha*H;

    int nit;
    for( nit=0; nit<maxNewtonIterations; nit++ )
    {
      double omegar=real(omega);
      double omegai=imag(omega);
	
      dispersionViscousAcoustic( omegar,omegai,ffr,ffi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn,u1Varies );
      ff=cmplx(ffr,ffi);
      dispersionViscousAcousticDeriv( omegar,omegai,fpr,fpi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn,u1Varies );
      fp=cmplx(fpr,fpi);

      ff0=max(ff0,abs(ff));
      domega = ff/fp;
      omega = omega-domega;

//       printf("Newton: nit=%i, omega=(%14.6e,%14.6e) domega=(%14.6e,%14.6e)\n",nit,real(omega),imag(omega),
//       	     real(domega),imag(domega));
      

      if( abs(domega) < tol )
	break;
    }
    if( nit>=maxNewtonIterations )
    {
      printf("findRootViscousAcoustic::ERROR: Newton iterations did not converge\n");
    }
    
    printf("findRootViscousAcoustic::%i Newton iterations |f(w)|/|f0|=%e, |dw|=%8.2e, k=%e, w=(%e,%e)\n",
       nit,abs(ff)/ff0,abs(domega),kn, real(omega),imag(omega) );
  }

  return omega;
}

// ===============================================================================================
/// \brief Find a root to the dispersion relation for the viscous fluid and elastic solid
/// option
// ===============================================================================================
cmplx
findRootViscousElastic( const int option,
			 const double & k, 
			 const double & rhos, const double & Hs, const double & cp, const double & cs,
			 const double & mu, const double & rho, const double & H, const double & u1Varies )
{

  int N = 100;  // number of continuation steps

  double cp0    = sqrt(3.);
  double cs0    = 1.;

  // initial guesss
//    double rhos0  = 1.;
//    double Hs0    = 4.;

//    double mu0    = 1./5.;
//    double rho0   = 1.;
//    double H0     = 4.;
//    double k0     = 1.;
//    cmplx omega0 = cmplx( .7677597130,-.1027244581);

   const double Pi = 4.*atan2(1.,1.);

   double Hs0    = .5;

   double mu0    = .05;
   double rho0   = 1.;
   double H0     = 1.;
   double k0     = 2.*Pi;

   double rhos0  = 1.;
   cmplx omega0 = cmplx(4.95937745809667e+00,-1.01615335330826e+00);

   // here are different starting guesses:
   if( rhos>75. )
   {
     rhos0  = 100.;
     omega0 = cmplx(6.713921918442e+00,-1.532282947120e-02);
   }
   else if( rhos>25. )
   {
     rhos0  = 50.;
     omega0 = cmplx(6.694455289287e+00,-3.058770280581e-02);
   }
   else if( rhos>5. )
   {
     rhos0  = 10.;
     omega0 = cmplx(6.539182737438e+00,-1.502827616814e-01);
   }
   else if( rhos>.5 )
   {
     rhos0=1.;
     omega0=cmplx(4.95937745809667e+00,-1.01615335330826e+00);
   }
   else if( rhos>.2 )
   {
     rhos0=.1;
     mu0=.01;
     omega0=cmplx(2.045599514081e+00,-4.086183655261e-01);
   }
   else if( rhos>.075 )
   {
     N=1000;
     rhos0=.1;
     mu0=.01;
     omega0=cmplx(2.045599514081e+00,-4.086183655261e-01);
   }
   else if( rhos>.04 )
   {
     // root for mu=.01, rhos=.05
     // ++findRootViscousElastic : root found: k=6.283185e+00, rhos=5.00e-02 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(1.42227198440995e+00,-3.79546093554936e-01)
     N=1000;
     rhos0=.05;
     mu0=.01;
     omega0=cmplx(1.42227198440995e+00,-3.79546093554936e-01);
   }
   else if( rhos>.01 )
   {
      // root for mu=.01, rhos=.03
       //  ++findRootViscousElastic : root found: k=6.283185e+00, rhos=3.00e-02 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(1.07420780056506e+00,-3.51490215236338e-01)

     N=1000;
     rhos0=.03;
     mu0=.01;
     omega0=cmplx(1.07420780056506e+00,-3.51490215236338e-01);
   }
   else if( rhos>.0035 )
   {
      // root for mu=.01, rhos=.005
      //      ++findRootViscousElastic : root found: k=6.283185e+00, rhos=5.00e-03 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(3.66183581424320e-01,-2.51404336187962e-01)
     N=1000;
     rhos0=.003;
     mu0=.01;
     omega0=cmplx(3.66183581424320e-01,-2.51404336187962e-01);
   }
   else if( rhos>.0025 )
   {
      // root for mu=.01, rhos=.003
      // ++findRootViscousElastic : root found: k=6.283185e+00, rhos=3.00e-03 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(2.51809768959371e-01,-2.30073629655043e-01)

     N=1000;
     rhos0=.003;
     mu0=.01;
     omega0=cmplx(2.51809768959371e-01,-2.30073629655043e-01);
   }
   else  if( rhos>.00175 )
   {
      // root for mu=.01, rhos=.002
           //   ++findRootViscousElastic : root found: k=6.283185e+00, rhos=2.00e-03 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(1.71882877079219e-01,-2.17196063300154e-01)

     N=1000;
     rhos0=.002;
     mu0=.01;
     omega0=cmplx(1.71882877079219e-01,-2.17196063300154e-01);
   }
   else 
   {
      // root for mu=.01, rhos=.0015
      //      ++findRootViscousElastic : root found: k=6.283185e+00, rhos=1.50e-03 Hs=5.00e-01 cp=1.73e+00 rho=1.00e+00 mu=1.00e-02 H=1.00e+00 omega=(1.14162140427608e-01,-2.10130189780927e-01)
     N=1000;
     rhos0=.0015;
     mu0=.01;
     omega0=cmplx(1.14162140427608e-01,-2.10130189780927e-01);
   }

  
  cmplx omega = omega0;
  cmplx domega;
  cmplx ff,fp;
  double ffr,ffi,fpr,fpi;
  double ff0=1.;  // initial residual
  
  int maxNewtonIterations = 20; // max Newton iterations
  double tol = 1e-14;

  for( int it=1; it<N; it++ )
  {
    double alpha = it/(N-1.);
    
    double kn  = (1.-alpha)*k0 + alpha*k;
    double cpn = (1.-alpha)*cp0 + alpha*cp;
    double csn = (1.-alpha)*cs0 + alpha*cs;
    double rhosn = (1.-alpha)*rhos0 + alpha*rhos;
    double Hsn = (1.-alpha)*Hs0 + alpha*Hs;

    double mun = (1.-alpha)*mu0 + alpha*mu;
    double rhon = (1.-alpha)*rho0 + alpha*rho;
    double Hn = (1.-alpha)*H0 + alpha*H;

    int nit;
    for( nit=0; nit<maxNewtonIterations; nit++ )
    {
      double omegar=real(omega);
      double omegai=imag(omega);
	
      dispersionViscousElastic( omegar,omegai,ffr,ffi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn,u1Varies );
      ff=cmplx(ffr,ffi);
      dispersionViscousElasticDeriv( omegar,omegai,fpr,fpi,  rhon,mun,Hn,kn,rhosn,cpn,csn,Hsn,u1Varies );
      fp=cmplx(fpr,fpi);

      ff0=max(ff0,abs(ff));
      domega = ff/fp;
      omega = omega-domega;

//       printf("Newton: nit=%i, |f|=%8.2e,  omega=(%14.6e,%14.6e) domega=(%14.6e,%14.6e)\n",
//              nit,abs(ff),real(omega),imag(omega),real(domega),imag(domega));
      

      if( abs(domega) < tol )
	break;
    }
    if( nit>=maxNewtonIterations )
    {
      printf("findRootViscousElastic::ERROR: Newton iterations did not converge\n");
    }
    
    printf("findRootViscousElastic::%i Newton iterations |f(w)|/|f0|=%e, |dw|=%8.2e, k=%e, w=(%18.12e,%18.12e)\n",
       nit,abs(ff)/ff0,abs(domega),kn, real(omega),imag(omega) );
  }

  return omega;
}




// ====================================================================================
/// \brief Return the FSI traveling wave solution for Stokes flow and an elastic shell
///   Reference exactSolution notes in papers/fis
/// 
///  \param wr,wi (output) : real and imaginary parts of omega 
///  \param ipar (input) : integer parameters
///  \param rpar (input) : real parameters
///  \param nf (input) : evaluate fluid solution at this many grid points in y 
///  \param yf[i] (input) : fluid grid points in y
///  \param v1r[i], v1i[i] : real and imaginary parts of v1 (fluid)
///  \param v2r[i], v2i[i] : real and imaginary parts of v2 (fluid)
///  \param  pr[i],  pi[i] : real and imaginary parts of p  (fluid)
///  \param ns (input) : evaluate solid solution at this many grid points in y
///  \param ys[i] (input) : solid grid points in y
///  \param us1r[i], us1[i] : real and imaginary parts of us1 (solid displacement)
///  \param us2r[i], us2[i] : real and imaginary parts of us2 (solid displacement)
///  \param us1yr[i], us1y[i] : real and imaginary parts of us1y (y-deriv of solid displacement)
///  \param us2yr[i], us2y[i] : real and imaginary parts of us2y (y-deriv solid displacement)
///
// This function is put in a separate file so we can use complex numbers (there are some
// conflicts with Overture names such as "real").
// ====================================================================================
int 
getTravelingWave( double & wr, double & wi , double *rpar, int *ipar, 
                  const int & nf, double *yf, double *pr, double *pi, 
                  double *v1r, double *v1i, double *v2r, double *v2i, 
                  const int & ns, double *ys, double *us1r, double *us1i,  double *us2r, double *us2i,
                  double *us1yr, double *us1yi,  double *us2yr, double *us2yi )
{
  
  int option    = ipar[0];
  int pdeOption = ipar[1];

  double k   = rpar[0];
  double H   = rpar[1];
  double rho = rpar[2];
  double mu  = rpar[3];
  double rhos= rpar[4];
  double hs  = rpar[5];
  double Ke  = rpar[6];
  double Te  = rpar[7];

  // bulk: 
  double Hs  = rpar[5];  // note -- these over-ride hs, Ke
  double cp  = rpar[6];
  double cs  = rpar[7];

  double mus=rhos*cs*cs;
  double lambdas=rhos*cp*cp - 2.*mus;
  


  cmplx I(0,1);

  cmplx w, a, Sa, Ca, A, B, C, D, ph, v2h, v1h, eta1h, eta2h;
  double Sk, Ck;
  
  // frequency: 
  const double Pi = 4.*atan2(1.,1.);
  
  cmplx omega;

  if( pdeOption==0 )
  {
    // -- Viscous Shell 
    omega=findRootShellViscous( option,k, Ke, Te, rhos, hs, mu, rho, H);
    printf("getTravelingWave: root found: k=%e, rhos=%8.2e hs=%8.2e Ke=%8.2e Te=%8.2e rho=%8.2e "
	   "mu=%8.2e omega=(%16.10e,%16.10e)\n",k,rhos,hs,Ke,Te,rho,mu,real(omega),imag(omega));
  

    wr=real(omega);
    wi=imag(omega);


//  w = cmplx(0.8323792222,-.2899391366)*2.*Pi;
//   w = cmplx(0.8323792222,-.2899391366);
    w= omega;
  
    a=sqrt(k*k-I*rho*w/mu);

    printf("getTravelingWave: k=%e, H=%e, rho=%8.2e, mu=%8.2e, w=(%g,%g), a=(%g,%g)\n",k,H,rho,mu,real(w),imag(w),
           real(a),imag(a));


    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    // --- Also see the test program codes/inses/src/dispersion.C

    cmplx G = Ke + k*k*Te - rhos*hs*w*w;

    if( option==0 )
    {
      // Solution for no tangential motion in the shell (eta1h=0)
      printf("getTravelingWave: DOUBLE CHECK: eta1 does NOT vary\n");

      A= k*(Ca-Ck);
      B= a*Sa*Sk-k*(Ca*Ck-1.);

      C= -(k/(a*Sa*Sa))*( A*( Ca*Ck-1.) + B*( Ca-Ck   ) );
      D= -(k/(a*Sa*Sa))*( A*( Ca-Ck   ) + B*( Ca*Ck-1.) );

      eta1h=0.;
      eta2h= (I/w)*( B*Sk+D*Sa );
      // -- scale: 
      // double scale = abs(w*eta2h);
      double scale = abs(eta2h);
      eta2h /=scale;
      A /= scale;
      B /= scale;
      C /= scale;
      D /= scale;

      // -- double check solution:
      double etaDiff=abs( G*eta2h - I*rho*(w/k)*( A+B*Ck ));

      printf(" |G*eta2h - I*rho*(w/k)*( A+B*Ck) |=%8.2e\n",etaDiff);
      double eq3 =abs( A*rho*w*w + B*(rho*w*w*Ck-k*G*Sk)-k*G*Sa*D );
      printf(" |eq3|=%8.2e\n",eq3);
      double eq4 =abs( A*k +B*k*Ck+a*C+a*Ca*D );
      printf(" |eq4|=%8.2e\n",eq4);

      double eq5 =abs( A*(a*Sa*Sk-k*(Ca*Ck-1.) ) + B*(-k*(Ca-Ck) ) );
      double eq6 =abs( A*(a*rho*w*w*Sa +k*k*G*(Ca-Ck) ) + B*( a*(rho*w*w*Ck-k*G*Sk)*Sa+k*k*G*(Ca*Ck-1.) ) );
      printf(" |eq5|=%8.2e, |eq6|=%8.2e\n",eq5,eq6);


      double deta = abs( (k*Sk*a*a*Sa-2*k*k*Ck*a*Ca+2*k*k*a+k*k*k*Sa*Sk)*G+(k*a*Ca*Sk-Ck*a*a*Sa)*rho*w*w );
      printf(" |det|=%8.2e, |det|/(Sk*Sa)=%8.2e\n",deta,deta/abs(Sk*Sa));

      double y=-H;
      double v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
      double v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      double v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
      cmplx py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      cmplx v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      cmplx v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      double pbc=abs( py-mu*( v2xx+v2yy ) );
      printf("CHECK: y=-H:  y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |v2y|=%9.3e, |p.y-mu*Delta(v2)|=%8.2e\n",y,v1,v2,v2y,pbc);

      y=-H/2.;
      cmplx v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      double vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
      printf("CHECK: y=-H/2:  y=%8.2e: |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n", y,vttDiff);

      y=0;
      v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
      v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
      double vDiff=abs( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) - (-I*w)*eta2h );

      v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
      printf("CHECK: y=0:  y=%8.2e: |v1|=%9.3e, |v2-eta_t|=%9.3e, |v2y|=%9.3e, |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n",
	     y,v1,vDiff,v2y,vttDiff);

      if( eq3>1.e-3 )
      {
        OV_ABORT("ERROR");
      }

    }
    else
    {
      // Solution with tangential motion in the shell
      printf("getTravelingWave: DOUBLE CHECK: eta1 varies\n");
    
      cmplx gam=rho*w*w + 2.*I*w*mu*k*k;

      cmplx a11=-Sk , a12=0.                      , a13=-Sa          , a14=0.;
      cmplx a21=k*Ck, a22=k                       , a23=a*Ca         , a24=a;
      cmplx a31=gam , a32=gam*Ck-Sk*G*k           , a33=2.*I*w*mu*k*a, a34=2.*I*w*mu*k*a*Ca-Sa*G*k;
      cmplx a41=k   , a42=k*Ck-2.*I*w*mu*k*k*Sk/G , a43=a            , a44=a*Ca-I*w*mu*Sa*(a*a+k*k)/G;


      // eliminate C, C=(-Sk/Sa)*A and eqn 1
      cmplx cc=-Sk/Sa;
      cmplx b11=a21+cc*a23, b12=a22  , b13=a24;
      cmplx b21=a31+cc*a33, b22=a32  , b23=a34;
      cmplx b31=a41+cc*a43, b32=a42  , b33=a44;
    

      // Leave D free -- choose to simplify expressions
      cmplx det=b11*b22-b12*b21;
      D=det; // scale all by det

      // solve for A,B in terms of D from [ b11 b12 ] = [-b13*D]
      //                                  [ b21 b22 ] = [-b23*D]
      A=-b22*b13+b12*b23;
      B= b21*b13-b11*b23;
      C=cc*A;
    
      eta1h=-1./G*mu*(I/k*(B*k*k*Sk+D*a*a*Sa)+I*k*(B*Sk+D*Sa));
      eta2h= 1./G*(I*rho*w/k*(A+B*Ck)-2*mu*(A*k+B*k*Ck+C*a+D*a*Ca));

      // -- double check solution:

      // -- scale: 
      // double scale = abs(w*eta2h);
      double eta1hNorm=abs(eta1h);
      double eta2hNorm=abs(eta2h);
      double scale = sqrt(eta1hNorm*eta1hNorm + eta2hNorm*eta2hNorm);

      A    /= scale;
      B    /= scale;
      C    /= scale;
      D    /= scale;
      eta1h/= scale;
      eta2h/= scale;
    

      double eq1 =abs( a11*A+a12*B+a13*C+a14*D );
      printf(" |eq1|=%8.2e\n",eq1);

      double eq2 =abs( a21*A+a22*B+a23*C+a24*D );
      printf(" |eq2|=%8.2e\n",eq2);

      double eq3 =abs( a31*A+a32*B+a33*C+a34*D );
      printf(" |eq3|=%8.2e\n",eq3);

      double eq4 =abs( a41*A+a42*B+a43*C+a44*D );
      printf(" |eq4|=%8.2e\n",eq4);

      double deta=abs( a11*a22*a33*a44-a11*a22*a34*a43-a11*a32*a23*a44+a11*a32*a24*a43+a11*a42*a23*a34-a11*a42*a24*a33-a21*a12*a33*a44+a21*a12*a34*a43+a21*a32*a13*a44-a21*a32*a14*a43-a21*a42*a13*a34+a21*a42*a14*a33+a31*a12*a23*a44-a31*a12
		       *a24*a43-a31*a22*a13*a44+a31*a22*a14*a43+a31*a42*a13*a24-a31*a42*a14*a23-a41*a12*a23*a34+a41*a12*a24*a33+a41*a22*a13*a34-a41*a22*a14*a33-a41*a32*a13*a24+a41*a32*a14*a23 );
      printf(" |det|=%8.2e, |det|/(Sk*Sa)=%8.2e\n",deta,deta/abs(Sk*Sa));

      double y=-H;
      double v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
      double v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      double v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
      cmplx py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      cmplx v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      cmplx v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      double pbc=abs( py-mu*( v2xx+v2yy ) );
      printf("CHECK: y=-H:  y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |v2y|=%9.3e, |p.y-mu*Delta(v2)|=%8.2e\n",y,v1,v2,v2y,pbc);

      y=-H/2.;
      cmplx v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      double vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
      printf("CHECK: y=-H/2:  y=%8.2e: |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n", y,vttDiff);

      y=0;
      v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
      v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
      double vDiff=abs( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) - (-I*w)*eta2h );

      v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
      py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
      v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
      v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
      vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );

      double v1Diff=abs( (I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ) -(-I*w)*eta1h );

      printf("CHECK: y=0:  y=%8.2e: |v1-eta1|=%9.3e, |v2-eta_t|=%9.3e, |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n",
	     y,v1Diff,vDiff,vttDiff);


      if( eq1>1.e-3 )
      {
        OV_ABORT("ERROR");
      }
      
    }



    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      us1r[i]=real(eta1h);
      us1i[i]=imag(eta1h);
  
      us2r[i]=real(eta2h);
      us2i[i]=imag(eta2h);
    }
  
//   vs1r[0]=0.;
//   vs1i[0]=0.;
  
//   vs2r[0]=real((-I*w)*eta2h);
//   vs2i[0]=imag((-I*w)*eta2h);
  
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      ph = I*rho*(w/k)*( A*cosh(k*y) + B*cosh(k*(y+H)) );

      v2h = A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H));

      v1h = (I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) );  // v1_x + v2_y = 0 

      pr[i] = real( ph );
      pi[i] = imag( ph );
  
      v1r[i] = real( v1h );
      v1i[i] = imag( v1h );
  
      v2r[i] = real( v2h );
      v2i[i] = imag( v2h );
  
    }
  }
  else if( pdeOption==1 && mu==0.)
  {
    // -------------------------------
    // --- Inviscid acoustic solid ---
    // -------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    if( option==0 ) u1Varies=0.;
    omega=findRootInviscidAcoustic( optionAcoustic,k, rhos, Hs, cp, cs, mu, rho, H );
    printf("\n++findRootInviscidAcoustic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e rho=%8.2e H=%8.2e"
	   " omega=(%16.10e,%16.10e)\n",k,rhos,Hs,cp,rho,H,real(omega),imag(omega));

    wr=real(omega);
    wi=imag(omega);

    w=omega;

    // --- compute the constants
    Sk = sinh(k*H); Ck =cosh(k*H); 
    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);
  

    double rcp2=rhos*cp*cp;

    cmplx As=1.; // choose As 
    cmplx Af=I*omega*As*Sas/Sk;

    double u2Max=abs( As*sinh(as*(Hs)));
    double scale=u2Max;  // scale to |u2|=1
    Af/=scale;
    As/=scale;

    // -- CHECK: evaluate the solution 
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      cmplx SkypH=sinh(k*(y+H)), CkypH=cosh(k*(y+H));
  
      v1 =I*Af*CkypH;
      v2 =Af*SkypH;
      p=I*rho*w/k*Af*CkypH;

      u2=As*sinh(as*(y-Hs));
      cmplx u2y=As*as*cosh(as*(y-Hs));

      if( y==0. )
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2-u2_t|=%9.3e, |rcp2*u2y+p|=%8.2e\n",
	       y,abs(v1),abs(v2+I*omega*u2),abs(rcp2*u2y+p));
      else
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e\n",
	       y,abs(v1),abs(v2));
      }
    }

    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      // --- 1 DOF acoustic solid ---
      us1=0.;
      us2=As*sinh(as*(y-Hs));
      
      // -- y-derivative (for computing the stress)
      us1y=0.;
      us2y=As*as*cosh(as*(y-Hs));


      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      cmplx SkypH=sinh(k*(y+H)), CkypH=cosh(k*(y+H));
  
      v1 =I*Af*CkypH;
      v2 =Af*SkypH;
      p=I*rho*w/k*Af*CkypH;

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else if( pdeOption==1 )
  {
    // ------------------------------
    // --- Viscous acoustic solid ---
    // ------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    if( option==0 ) u1Varies=0.;
    omega=findRootViscousAcoustic( optionAcoustic,k, rhos, Hs, cp, cs, mu, rho, H, u1Varies );
    printf("\n++findRootViscousAcoustic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e cs=%8.2e rho=%8.2e "
	   "mu=%8.2e mus=%8.2e u1Varies=%f omega=(%16.10e,%16.10e)\n",k,rhos,Hs,cp,cs,rho,mu,mus,
           u1Varies,real(omega),imag(omega));

    wr=real(omega);
    wi=imag(omega);

    w=omega;

    // --- compute the constants
    cmplx Say,Cay, SaypH,CaypH;
  
    a=sqrt(k*k-I*rho*w/mu);

    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);

    cmplx bs = sqrt(k*k-w*w/(cs*cs) );
    cmplx Sbs=sinh(bs*Hs), Cbs=cosh(bs*Hs);
  

    double rcp2=rhos*cp*cp;
    double rcs2=rhos*cs*cs;

    cmplx Af,Bf, As,Bs;
    if( option==0 ) 
    {
      // -- single solid component ---
      cmplx a11=k*(k*Sa-a*Sk+(a*Ca*Sk-k*Ck*Sa)*Ck),     a12=a*(k*Sa-a*Sk+(a*Ca*Sk-k*Ck*Sa)*Ca), a13=0.;
      cmplx a21=(a*Ca*Sk-k*Ck*Sa)*Sk,                   a22=(a*Ca*Sk-k*Ck*Sa)*Sa,               a23=-I*w*Sas;
      cmplx a31= I*rho*w/k*(k*Sa+(a*Ca*Sk-k*Ck*Sa)*Ck), a32= I*rho*w/k*a*Sa,                    a33=rcp2*as*Cas;

      // solve for Af, Bf in terms of As
      //   [ a11 a12 ] = [-a13*As]
      //   [ a21 a22 ] = [-a23*As]
  

      cmplx det=a11*a22-a12*a21;
      As=1.; // choose As 
      Bs=0.;
      printf(" --> a=(%e,%e), as=(%e,%e), Adet=%e\n",real(a),imag(a),real(as),imag(as),abs(det));

      Af=(-a22*a13+a12*a23)*(As/det);
      Bf=( a21*a13-a11*a23)*(As/det);

      // -- scale so that |us|=1
      double scale=abs(As*sinh(as*(Hs)));
      Af /= scale;
      Bf /= scale;
      As /= scale;
    }
    else
    {
      // --- Acoustic solid 2 components ----

      cmplx a11,a12,a13,a14, a21,a22,a23,a24;
      cmplx a31,a32,a33,a34, a41,a42,a43,a44;

      cmplx II=I;
  
      a11 = II * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ck);
      a12 = II / k * a * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ca);
      a13 = 0;
      a14 = -II * w * Sbs;
      a21 = (a * Ca * Sk - k * Ck * Sa) * Sk;
      a22 = (a * Ca * Sk - k * Ck * Sa) * Sa;
      a23 = -II * w * Sas;
      a24 = 0;
      a31 = -2. * II * u1Varies * mu * k * (a * Ca * Sk - k * Ck * Sa) * Sk;
      a32 = -u1Varies * mu * (II / k * a*a * (a * Ca * Sk - k * Ck * 
					      Sa) * Sa + II * k * (a * Ca * Sk - k * Ck * Sa) * Sa);
      a33 = 0;
      a34 = 1 - u1Varies + u1Varies * rcs2 * bs * Cbs;
      a41 = II * rho * w / k * (k * Sa + (a * Ca * Sk - k * Ck * Sa) * Ck) 
	- 2 * mu * (k*k * Sa - k * a * Sk + (a * Ca * Sk - k * Ck * Sa) * k * Ck);
      a42 = II * rho * w / k * a * Sa - 2 * mu * (a * k * Sa - a*a * 
						  Sk + (a * Ca * Sk - k * Ck * Sa) * a * Ca);
      a43 = rcp2 * as * Cas;
      a44 = 0;
      
      // Unknowns [Af Bf As Bs ]
      // solve for Af, Bf, As in terms of Bs
      //   [ a11 a12 a13 ] = [-a14*Bs]
      //   [ a21 a22 a23 ] = [-a24*Bs]
      //   [ a31 a32 a33 ] = [-a34*Bs]

      
      cmplx det = a11*a22*a33-a11*a23*a32-a21*a12*a33+a21*a13*a32+a31*a12*a23-a31*a13*a22;
      Bs=1;
      Af = ( -(a22*a33-a23*a32)*a14+(a12*a33-a13*a32)*a24-(a12*a23-a13*a22)*a34 )*Bs/det;
      Bf = (  (a21*a33-a23*a31)*a14-(a11*a33-a13*a31)*a24+(a11*a23-a13*a21)*a34 )*Bs/det;
      As = ( -(a21*a32-a22*a31)*a14+(a11*a32-a12*a31)*a24-(a11*a22-a12*a21)*a34 )*Bs/det;
      

      // -- scale so that |us|=1
      double u1Norm = abs(-Bs*Sbs);
      double u2Norm = abs(-As*Sas);
      double scale= sqrt( u1Norm*u1Norm + u2Norm*u2Norm );

      Af /= scale;
      Bf /= scale;
      As /= scale;
      Bs /= scale;
    }

    cmplx aCSkCS=(a*Ca*Sk-k*Ck*Sa);

    // -- CHECK: evaluate the solution 
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  

      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));

      py=I*rho*w/k*(Af*(k*Sa*k*sinh(k*y)+aCSkCS*k*sinh(k*(y+H))) +Bf*a*Sa*k*sinh(k*y));

      u1=Bs*sinh(bs*(y-Hs));
      u2=As*sinh(as*(y-Hs));

// e1 := Af*k*( k*Sa -a*Sk + aCSkCS*Ck ) 
//      +Bf*a*( k*Sa -a*Sk + aCSkCS*Ca )
//      +I*w*u10:

      v1xx=-k*k*v1;
      v1y=I/k*(Af*k*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*k*sinh(k*(y+H)))+
               Bf*a*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*a*SaypH));
      v1yy=I/k*(Af*k*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*k*k*cosh(k*(y+H)))+
                Bf*a*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*a*a*CaypH));

      v2x=I*k*v2;
      v2xx=-k*k*v2;
      v2y =Af*(k*k*Sa*cosh(k*y)-k*a*Sk*Cay+aCSkCS*k*cosh(k*(y+H)))+
   	   Bf*(a*k*Sa*cosh(k*y)-a*a*Sk*Cay+aCSkCS*a*CaypH);
      v2yy=Af*(k*k*Sa*k*sinh(k*y)-k*a*a*Sk*Say+aCSkCS*k*k*sinh(k*(y+H)))+
   	   Bf*(a*k*Sa*k*sinh(k*y)-a*a*a*Sk*Say+aCSkCS*a*a*SaypH);


      u1y=Bs*bs*cosh(bs*(y-Hs));
      u2y=As*as*cosh(as*(y-Hs));

      // u1x=I*k*u1;
      // u2x=I*k*u2;
       
      if( y==0. )
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
        printf("     : traction: |rcp2**u2y -( -p+2*mu*v2y)|=%8.2e, |rcs2*u1y -mu*(v1y+v2x)|=%9.3e \n",
               abs(rhos*cp*cp*u2y - (-p+2.*mu*v2y)),
	       abs(rhos*cs*cs*u1y - mu*(v1y+v2x))
               );
      }
      else
      {
	// printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |rho*v1_t+i*k*p-mu*|, |rho*v2_t+p.y|\n",
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e |v1_momenutum|=%8.2e |v2_momenutum|=%8.2e\n",
	       y,abs(v1),abs(v2),
               abs(-I*w*rho*v1+I*k*p-mu*(v1xx+v1yy)),
               abs(-I*w*rho*v2+py   -mu*(v2xx+v2yy))
               );
      }
      
    }

    
    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      if( option==0 ) 
      {
        // --- 1 DOF acoustic solid ---
	us1=0.;
	us2=As*sinh(as*(y-Hs));
      
	// -- y-derivative (for computing the stress)
	us1y=0.;
	us2y=As*as*cosh(as*(y-Hs));
      }
      else
      { // --- 2 DOF acoustic solid ---

	us1=Bs*sinh(bs*(y-Hs));
	us2=As*sinh(as*(y-Hs));

	// -- y-derivative (for computing the stress)
	us1y=Bs*bs*cosh(bs*(y-Hs));
	us2y=As*as*cosh(as*(y-Hs));


        // -- ELASTIC: 

// 	cmplx Bs1=As;
// 	cmplx Ds1=Bs;
	
// 	cmplx As1 = -k*k*Sbs*(Bs1+Ds1)/(k*k*Cas*Sbs-as*bs*Cbs*Sas);
// 	cmplx Cs1 = as*bs*Sas*(Bs1+Ds1)/(k*k*Cas*Sbs-as*bs*Cbs*Sas);


// 	us1= As1*cosh(as*y)
// 	  +Bs1*cosh(as*(y-Hs))
// 	  +Cs1*cosh(bs*y)
// 	  +Ds1*cosh(bs*(y-Hs));

// 	us2=-I*(as*As1/k*sinh(as*y)
// 		+as*Bs1/k*sinh(as*(y-Hs))
// 		+k*Cs1/bs*sinh(bs*y)
// 		+k*Ds1/bs*sinh(bs*(y-Hs)));
      
// 	// -- y-derivative (for computing the stress)
// 	us1y =    As1*as*sinh(as*y)
// 	  +Bs1*as*sinh(as*(y-Hs))
// 	  +Cs1*bs*sinh(bs*y)
// 	  +Ds1*bs*sinh(bs*(y-Hs));

// 	us2y = -I*(as*As1/k*as*cosh(as*y)
// 		   +as*Bs1/k*as*cosh(as*(y-Hs))
// 		   +k*Cs1/bs*bs*cosh(bs*y)
// 		   +k*Ds1/bs*bs*cosh(bs*(y-Hs)));

      }
      

      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));


//       v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*cosh(k*(y+H)))+
//                Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*CaypH));
  

//       v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)))+
//          Bf*(a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH);

//       p=I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H)))+Bf*a*Sa*cosh(k*y));

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else if( pdeOption==2 )
  {
    // -------------------------------------
    // --- Viscous fluid / elastic solid ---
    // ------------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    omega=findRootViscousElastic( option,k, rhos, Hs, cp, cs, mu, rho, H,u1Varies );
    printf("\n++findRootViscousElastic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e rho=%8.2e "
	   "mu=%8.2e H=%8.2e omega=(%18.14e,%18.14e)\n",k,rhos,Hs,cp,rho,mu,H,real(omega),imag(omega));


    wr=real(omega);
    wi=imag(omega);

    w=omega;

    // --- compute the constants
    cmplx Say,Cay, SaypH,CaypH;
  
    a=sqrt(k*k-I*rho*w/mu);

    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);

    cmplx bs = sqrt(k*k-w*w/(cs*cs) );
    cmplx Sbs=sinh(bs*Hs), Cbs=cosh(bs*Hs);
  

    double mus=rhos*cs*cs;
    double lambdas=rhos*cp*cp - 2.*mus;
  

    cmplx Af,Bf, As,Bs;

    cmplx a11,a12,a13,a14, a21,a22,a23,a24;
    cmplx a31,a32,a33,a34, a41,a42,a43,a44;

    cmplx II=I;
  
    a11 = II * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ck);
    a12 = II / k * a * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ca);
    a13 = II * w * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cas + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas));
    a14 = II * w * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cbs);
    a21 = (a * Ca * Sk - k * Ck * Sa) * Sk;
    a22 = (a * Ca * Sk - k * Ck * Sa) * Sa;
    a23 = -w * as / k * Sas;
    a24 = -w * k / bs * Sbs;
    a31 = -2. * II * mu * k * (a * Ca * Sk - k * Ck * Sa) * Sk;
    a32 = -mu * (II / k * a*a * (a * Ca * Sk - k * Ck * Sa) * Sa + II * k * (a * Ca * Sk - k * Ck * Sa) * Sa);
    a33 = -2 * mus * as * Sas;
    a34 = mus * (-bs * Sbs - k*k / bs * Sbs);
    a41 = II * rho * w / k * (k * Sa + (a * Ca * Sk - k * Ck * Sa) * Ck) - 2 * mu * (k*k * Sa - k * a * Sk + (a * Ca * Sk - k * Ck * Sa) * k * Ck);
    a42 = II * rho * w / k * a * Sa - 2 * mu * (a * k * Sa - a*a * Sk + (a * Ca * Sk - k * Ck * Sa) * a * Ca);
    a43 = -II * (lambdas + 2 * mus) * (-as*as * k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as*as / k * Cas + k * as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas)) + II * lambdas * k * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cas + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas));
    a44 = -II * (lambdas + 2 * mus) * (-as*as * k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + k * as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + k * Cbs) + II * lambdas * k * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cbs);
      


      
    // Unknowns [Af Bf As Bs ]
    // solve for Af, Bf, As in terms of Bs
    //   [ a11 a12 a13 ] = [-a14*Bs]
    //   [ a21 a22 a23 ] = [-a24*Bs]
    //   [ a31 a32 a33 ] = [-a34*Bs]

      
    cmplx det = a11*a22*a33-a11*a23*a32-a21*a12*a33+a21*a13*a32+a31*a12*a23-a31*a13*a22;
    Bs=1;
    Af = ( -(a22*a33-a23*a32)*a14+(a12*a33-a13*a32)*a24-(a12*a23-a13*a22)*a34 )*Bs/det;
    Bf = (  (a21*a33-a23*a31)*a14-(a11*a33-a13*a31)*a24+(a11*a23-a13*a21)*a34 )*Bs/det;
    As = ( -(a21*a32-a22*a31)*a14+(a11*a32-a12*a31)*a24-(a11*a22-a12*a21)*a34 )*Bs/det;
      

    // -- scale so that |us|=1
    double u1Norm = abs(-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)+As*Cas+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)+Bs*Cbs);
    double u2Norm=abs(-I*(-as*As/k*Sas-k*Bs/bs*Sbs));
    double scale=sqrt( u1Norm*u1Norm + u2Norm*u2Norm );

    Af /= scale;
    Bf /= scale;
    As /= scale;
    Bs /= scale;
    
    cmplx aCSkCS=(a*Ca*Sk-k*Ck*Sa);
  
    // -- evaluate the solution
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+(a*Ca*Sk-k*Ck*Sa)*cosh(k*(y+H)))+
	       Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+(a*Ca*Sk-k*Ck*Sa)*CaypH));

      v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+(a*Ca*Sk-k*Ck*Sa)*sinh(k*(y+H)))+
	Bf*(a*Sa*sinh(k*y)-a*Sk*Say+(a*Ca*Sk-k*Ck*Sa)*SaypH);

      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));

      py=I*rho*w/k*(Af*(k*Sa*k*sinh(k*y)+aCSkCS*k*sinh(k*(y+H))) +Bf*a*Sa*k*sinh(k*y));
    
      u1=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+As*cosh(as*(y-Hs))+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+Bs*cosh(bs*(y-Hs));
      u2=-I*(-as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(as*y)+as*As/k*sinh(as*(y-Hs))+k*as*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+k*Bs/bs*sinh(bs*(y-Hs)));
    
      v1xx=-k*k*v1;
      v1y=I/k*(Af*k*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*k*sinh(k*(y+H)))+
               Bf*a*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*a*SaypH));
      v1yy=I/k*(Af*k*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*k*k*cosh(k*(y+H)))+
                Bf*a*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*a*a*CaypH));

      v2x=I*k*v2;
      v2xx=-k*k*v2;
      v2y =Af*(k*k*Sa*cosh(k*y)-k*a*Sk*Cay+aCSkCS*k*cosh(k*(y+H)))+
	Bf*(a*k*Sa*cosh(k*y)-a*a*Sk*Cay+aCSkCS*a*CaypH);
      v2yy=Af*(k*k*Sa*k*sinh(k*y)-k*a*a*Sk*Say+aCSkCS*k*k*sinh(k*(y+H)))+
	Bf*(a*k*Sa*k*sinh(k*y)-a*a*a*Sk*Say+aCSkCS*a*a*SaypH);


      u1x=I*k*u1;
      u1y=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*as*sinh(as*y)+As*as*sinh(as*(y-Hs))+as*bs*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+Bs*bs*sinh(bs*(y-Hs));
      u2x=I*k*u2;
      u2y=-I*(-as*as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+as*as*As/k*cosh(as*(y-Hs))+k*as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+k*Bs*cosh(bs*(y-Hs)));


      // u1x=I*k*u1;
      // u2x=I*k*u2;
       
      if( y==0. )
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
        printf("     : traction: |s22 -( -p+2*mu*v2y)|=%8.2e, |s12 -mu*(v1y+v2x)|=%9.3e \n",
               abs((lambdas+2.*mus)*u2y +lambdas*u1x - (-p+2.*mu*v2y)),
	       abs(mus*(u1y+u2x) - mu*(v1y+v2x))
	  );
      }
      else
      {
	// printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |rho*v1_t+i*k*p-mu*|, |rho*v2_t+p.y|\n",
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e |v1_momenutum|=%8.2e |v2_momenutum|=%8.2e\n",
	       y,abs(v1),abs(v2),
               abs(-I*w*rho*v1+I*k*p-mu*(v1xx+v1yy)),
               abs(-I*w*rho*v2+py   -mu*(v2xx+v2yy))
	  );
      }


      if( y==0. )
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
      else
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e\n",
	       y,abs(v1),abs(v2));
      }
      
    }
    
    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      us1=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+As*cosh(as*(y-Hs))+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+Bs*cosh(bs*(y-Hs));
      us2=-I*(-as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(as*y)+as*As/k*sinh(as*(y-Hs))+k*as*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+k*Bs/bs*sinh(bs*(y-Hs)));
    
      us1y=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*as*sinh(as*y)+As*as*sinh(as*(y-Hs))+as*bs*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+Bs*bs*sinh(bs*(y-Hs));
      us2y=-I*(-as*as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+as*as*As/k*cosh(as*(y-Hs))+k*as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+k*Bs*cosh(bs*(y-Hs)));


      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));


//       v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*cosh(k*(y+H)))+
//                Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*CaypH));
  

//       v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)))+
//          Bf*(a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH);

//       p=I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H)))+Bf*a*Sa*cosh(k*y));

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else
  {
    printf("getTravelingWave:ERROR: unknown pdeOption=%i",pdeOption);
  }
  
  

//   vr = real( v2h*exp(I*(k*x-w*t)) );
//   v1r= real( v1h*exp(I*(k*x-w*t)) );
//   pr = real( ph*exp(I*(k*x-w*t)) );

//   etar = real( eta2h*exp(I*(k*x-w*t)) );


  return 0;
}




// ====================================================================================
/// \brief Evaluate the FSI traveling wave solution for Stokes flow and an elastic shell
///      or elastic bulk solid
///   Reference exactSolution notes in papers/fis and papers/fib
/// 
///  \param wr,wi (INPUT) : real and imaginary parts of omega 
///  \param ipar (input) : integer parameters
///  \param rpar (input) : real parameters
///  \param nf (input) : evaluate fluid solution at this many grid points in y 
///  \param yf[i] (input) : fluid grid points in y
///  \param v1r[i], v1i[i] : real and imaginary parts of v1 (fluid)
///  \param v2r[i], v2i[i] : real and imaginary parts of v2 (fluid)
///  \param  pr[i],  pi[i] : real and imaginary parts of p  (fluid)
///  \param ns (input) : evaluate solid solution at this many grid points in y
///  \param ys[i] (input) : solid grid points in y
///  \param us1r[i], us1[i] : real and imaginary parts of us1 (solid displacement)
///  \param us2r[i], us2[i] : real and imaginary parts of us2 (solid displacement)
///  \param us1yr[i], us1y[i] : real and imaginary parts of us1y (y-deriv of solid displacement)
///  \param us2yr[i], us2y[i] : real and imaginary parts of us2y (y-deriv solid displacement)
///
// This function is put in a separate file so we can use complex numbers (there are some
// conflicts with Overture names such as "real").
// ====================================================================================
int 
evalTravelingWave(const double & wr, const double & wi , double *rpar, int *ipar, 
                  const int & nf, double *yf, double *pr, double *pi, 
                  double *v1r, double *v1i, double *v2r, double *v2i, 
                  const int & ns, double *ys, double *us1r, double *us1i,  double *us2r, double *us2i,
                  double *us1yr, double *us1yi,  double *us2yr, double *us2yi )
{
  
  const int debug=0;
  

  int option    = ipar[0];
  int pdeOption = ipar[1];

  double k   = rpar[0];
  double H   = rpar[1];
  double rho = rpar[2];
  double mu  = rpar[3];
  double rhos= rpar[4];
  double hs  = rpar[5];
  double Ke  = rpar[6];
  double Te  = rpar[7];

  // bulk: 
  double Hs  = rpar[5];  // note -- these over-ride hs, Ke
  double cp  = rpar[6];
  double cs  = rpar[7];

  double mus=rhos*cs*cs;
  double lambdas=rhos*cp*cp - 2.*mus;
  


  cmplx I(0,1);

  cmplx w, a, Sa, Ca, A, B, C, D, ph, v2h, v1h, eta1h, eta2h;
  double Sk, Ck;
  
  // frequency: 
  const double Pi = 4.*atan2(1.,1.);
  
  cmplx omega = cmplx(wr,wi);

  if( pdeOption==0 )
  {
    // // -- Viscous Shell 
    // omega=findRootShellViscous( option,k, Ke, Te, rhos, hs, mu, rho, H);
    // printf("evalTravelingWave: root found: k=%e, rhos=%8.2e hs=%8.2e Ke=%8.2e Te=%8.2e rho=%8.2e "
    // 	   "mu=%8.2e omega=(%16.10e,%16.10e)\n",k,rhos,hs,Ke,Te,rho,mu,real(omega),imag(omega));
  

    // wr=real(omega);
    // wi=imag(omega);


    w= omega;
  
    a=sqrt(k*k-I*rho*w/mu);

    if( debug & 1 )
      printf("evalTravelingWave: k=%e, H=%e, rho=%8.2e, mu=%8.2e, w=(%g,%g), a=(%g,%g)\n",k,H,rho,mu,real(w),imag(w),
	     real(a),imag(a));


    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    // --- Also see the test program codes/inses/src/dispersion.C

    cmplx G = Ke + k*k*Te - rhos*hs*w*w;

    if( option==0 )
    {
      // Solution for no tangential motion in the shell (eta1h=0)
      A= k*(Ca-Ck);
      B= a*Sa*Sk-k*(Ca*Ck-1.);

      C= -(k/(a*Sa*Sa))*( A*( Ca*Ck-1.) + B*( Ca-Ck   ) );
      D= -(k/(a*Sa*Sa))*( A*( Ca-Ck   ) + B*( Ca*Ck-1.) );

      eta1h=0.;
      eta2h= (I/w)*( B*Sk+D*Sa );
      // -- scale: 
      // double scale = abs(w*eta2h);
      double scale = abs(eta2h);
      eta2h /=scale;
      A /= scale;
      B /= scale;
      C /= scale;
      D /= scale;

      if( debug & 4 )
      {
        printf("evalTravelingWave: DOUBLE CHECK: eta1 does NOT vary\n");

	// -- double check solution:
	double etaDiff=abs( G*eta2h - I*rho*(w/k)*( A+B*Ck ));

	printf(" |G*eta2h - I*rho*(w/k)*( A+B*Ck) |=%8.2e\n",etaDiff);
	double eq3 =abs( A*rho*w*w + B*(rho*w*w*Ck-k*G*Sk)-k*G*Sa*D );
	printf(" |eq3|=%8.2e\n",eq3);
	double eq4 =abs( A*k +B*k*Ck+a*C+a*Ca*D );
	printf(" |eq4|=%8.2e\n",eq4);

	double eq5 =abs( A*(a*Sa*Sk-k*(Ca*Ck-1.) ) + B*(-k*(Ca-Ck) ) );
	double eq6 =abs( A*(a*rho*w*w*Sa +k*k*G*(Ca-Ck) ) + B*( a*(rho*w*w*Ck-k*G*Sk)*Sa+k*k*G*(Ca*Ck-1.) ) );
	printf(" |eq5|=%8.2e, |eq6|=%8.2e\n",eq5,eq6);


	double deta = abs( (k*Sk*a*a*Sa-2*k*k*Ck*a*Ca+2*k*k*a+k*k*k*Sa*Sk)*G+(k*a*Ca*Sk-Ck*a*a*Sa)*rho*w*w );
	printf(" |det|=%8.2e, |det|/(Sk*Sa)=%8.2e\n",deta,deta/abs(Sk*Sa));

	double y=-H;
	double v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
	double v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	double v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
	cmplx py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	cmplx v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	cmplx v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	double pbc=abs( py-mu*( v2xx+v2yy ) );
	printf("CHECK: y=-H:  y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |v2y|=%9.3e, |p.y-mu*Delta(v2)|=%8.2e\n",y,v1,v2,v2y,pbc);

	y=-H/2.;
	cmplx v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	double vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
	printf("CHECK: y=-H/2:  y=%8.2e: |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n", y,vttDiff);

	y=0;
	v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
	v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
	double vDiff=abs( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) - (-I*w)*eta2h );

	v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
	printf("CHECK: y=0:  y=%8.2e: |v1|=%9.3e, |v2-eta_t|=%9.3e, |v2y|=%9.3e, |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n",
	       y,v1,vDiff,v2y,vttDiff);

	if( eq3>1.e-3 )
	{
	  OV_ABORT("ERROR");
	}
      }
      
    }
    else
    {
      // Solution with tangential motion in the shell
    
      cmplx gam=rho*w*w + 2.*I*w*mu*k*k;

      cmplx a11=-Sk , a12=0.                      , a13=-Sa          , a14=0.;
      cmplx a21=k*Ck, a22=k                       , a23=a*Ca         , a24=a;
      cmplx a31=gam , a32=gam*Ck-Sk*G*k           , a33=2.*I*w*mu*k*a, a34=2.*I*w*mu*k*a*Ca-Sa*G*k;
      cmplx a41=k   , a42=k*Ck-2.*I*w*mu*k*k*Sk/G , a43=a            , a44=a*Ca-I*w*mu*Sa*(a*a+k*k)/G;


      // eliminate C, C=(-Sk/Sa)*A and eqn 1
      cmplx cc=-Sk/Sa;
      cmplx b11=a21+cc*a23, b12=a22  , b13=a24;
      cmplx b21=a31+cc*a33, b22=a32  , b23=a34;
      cmplx b31=a41+cc*a43, b32=a42  , b33=a44;
    

      // Leave D free -- choose to simplify expressions
      cmplx det=b11*b22-b12*b21;
      D=det; // scale all by det

      // solve for A,B in terms of D from [ b11 b12 ] = [-b13*D]
      //                                  [ b21 b22 ] = [-b23*D]
      A=-b22*b13+b12*b23;
      B= b21*b13-b11*b23;
      C=cc*A;
    
      eta1h=-1./G*mu*(I/k*(B*k*k*Sk+D*a*a*Sa)+I*k*(B*Sk+D*Sa));
      eta2h= 1./G*(I*rho*w/k*(A+B*Ck)-2*mu*(A*k+B*k*Ck+C*a+D*a*Ca));

      // -- scale: 
      // double scale = abs(w*eta2h);
      double eta1hNorm=abs(eta1h);
      double eta2hNorm=abs(eta2h);
      double scale = sqrt(eta1hNorm*eta1hNorm + eta2hNorm*eta2hNorm);

      A    /= scale;
      B    /= scale;
      C    /= scale;
      D    /= scale;
      eta1h/= scale;
      eta2h/= scale;
    
      // -- double check solution:
      if( debug & 4 )
      {
	printf("evalTravelingWave: DOUBLE CHECK: eta1 varies\n");

	double eq1 =abs( a11*A+a12*B+a13*C+a14*D );
	printf(" |eq1|=%8.2e\n",eq1);

	double eq2 =abs( a21*A+a22*B+a23*C+a24*D );
	printf(" |eq2|=%8.2e\n",eq2);

	double eq3 =abs( a31*A+a32*B+a33*C+a34*D );
	printf(" |eq3|=%8.2e\n",eq3);

	double eq4 =abs( a41*A+a42*B+a43*C+a44*D );
	printf(" |eq4|=%8.2e\n",eq4);

	double deta=abs( a11*a22*a33*a44-a11*a22*a34*a43-a11*a32*a23*a44+a11*a32*a24*a43+a11*a42*a23*a34-a11*a42*a24*a33-a21*a12*a33*a44+a21*a12*a34*a43+a21*a32*a13*a44-a21*a32*a14*a43-a21*a42*a13*a34+a21*a42*a14*a33+a31*a12*a23*a44-a31*a12
			 *a24*a43-a31*a22*a13*a44+a31*a22*a14*a43+a31*a42*a13*a24-a31*a42*a14*a23-a41*a12*a23*a34+a41*a12*a24*a33+a41*a22*a13*a34-a41*a22*a14*a33-a41*a32*a13*a24+a41*a32*a14*a23 );
	printf(" |det|=%8.2e, |det|/(Sk*Sa)=%8.2e\n",deta,deta/abs(Sk*Sa));

	double y=-H;
	double v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
	double v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	double v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
	cmplx py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	cmplx v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	cmplx v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	double pbc=abs( py-mu*( v2xx+v2yy ) );
	printf("CHECK: y=-H:  y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |v2y|=%9.3e, |p.y-mu*Delta(v2)|=%8.2e\n",y,v1,v2,v2y,pbc);

	y=-H/2.;
	cmplx v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	double vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );
	printf("CHECK: y=-H/2:  y=%8.2e: |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n", y,vttDiff);

	y=0;
	v1=abs((I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ));
	v2=abs(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	v2y=abs(k*(A*cosh(k*y) + B*cosh(k*(y+H))) + a*(C*cosh(a*y)+ D*cosh(a*(y+H))));
	double vDiff=abs( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) - (-I*w)*eta2h );

	v2t=(-I*w)*(A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)));
	py= I*rho*(w/k)*(k)*( A*sinh(k*y) + B*sinh(k*(y+H)) );
	v2xx = -k*k*( A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H)) );
	v2yy =  k*k*( A*sinh(k*y) + B*sinh(k*(y+H))) + a*a*(C*sinh(a*y)+ D*sinh(a*(y+H)));
	vttDiff=abs( rho*v2t+py-mu*( v2xx+v2yy ) );

	double v1Diff=abs( (I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) ) -(-I*w)*eta1h );

	printf("CHECK: y=0:  y=%8.2e: |v1-eta1|=%9.3e, |v2-eta_t|=%9.3e, |rho*vt-p.y+mu*Delta(v2)|=%8.2e\n",
	       y,v1Diff,vDiff,vttDiff);


	if( eq1>1.e-3 )
	{
	  OV_ABORT("ERROR");
	}
      }
      
    }


    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      us1r[i]=real(eta1h);
      us1i[i]=imag(eta1h);
  
      us2r[i]=real(eta2h);
      us2i[i]=imag(eta2h);
    }
  
//   vs1r[0]=0.;
//   vs1i[0]=0.;
  
//   vs2r[0]=real((-I*w)*eta2h);
//   vs2i[0]=imag((-I*w)*eta2h);
  
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      ph = I*rho*(w/k)*( A*cosh(k*y) + B*cosh(k*(y+H)) );

      v2h = A*sinh(k*y) + B*sinh(k*(y+H)) + C*sinh(a*y)+ D*sinh(a*(y+H));

      v1h = (I/k)*( A*k*cosh(k*y) + B*k*cosh(k*(y+H)) + C*a*cosh(a*y)+ D*a*cosh(a*(y+H)) );  // v1_x + v2_y = 0 


      pr[i] = real( ph );
      pi[i] = imag( ph );
  
      v1r[i] = real( v1h );
      v1i[i] = imag( v1h );
  
      v2r[i] = real( v2h );
      v2i[i] = imag( v2h );

      // printf("evalTravelingWave: v2r[i],v2i[i]=(%g,%g)\n",v2r[i], v2i[i]);
  
    }
  }
  else if( pdeOption==1 && mu==0.)
  {
    // -------------------------------
    // --- Inviscid acoustic solid ---
    // -------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    if( option==0 ) u1Varies=0.;
    // omega=findRootInviscidAcoustic( optionAcoustic,k, rhos, Hs, cp, cs, mu, rho, H );
    // printf("\n++findRootInviscidAcoustic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e rho=%8.2e H=%8.2e"
    // 	   " omega=(%16.10e,%16.10e)\n",k,rhos,Hs,cp,rho,H,real(omega),imag(omega));

    // wr=real(omega);
    // wi=imag(omega);

    omega=cmplx(wr,wi);

    w=omega;

    // --- compute the constants
    Sk = sinh(k*H); Ck =cosh(k*H); 
    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);
  

    double rcp2=rhos*cp*cp;

    cmplx As=1.; // choose As 
    cmplx Af=I*omega*As*Sas/Sk;

    double u2Max=abs( As*sinh(as*(Hs)));
    double scale=u2Max;  // scale to |u2|=1
    Af/=scale;
    As/=scale;

    // -- CHECK: evaluate the solution 
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      cmplx SkypH=sinh(k*(y+H)), CkypH=cosh(k*(y+H));
  
      v1 =I*Af*CkypH;
      v2 =Af*SkypH;
      p=I*rho*w/k*Af*CkypH;

      u2=As*sinh(as*(y-Hs));
      cmplx u2y=As*as*cosh(as*(y-Hs));

      if( y==0. )
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2-u2_t|=%9.3e, |rcp2*u2y+p|=%8.2e\n",
	       y,abs(v1),abs(v2+I*omega*u2),abs(rcp2*u2y+p));
      else
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e\n",
	       y,abs(v1),abs(v2));
      }
    }

    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      // --- 1 DOF acoustic solid ---
      us1=0.;
      us2=As*sinh(as*(y-Hs));
      
      // -- y-derivative (for computing the stress)
      us1y=0.;
      us2y=As*as*cosh(as*(y-Hs));


      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      cmplx SkypH=sinh(k*(y+H)), CkypH=cosh(k*(y+H));
  
      v1 =I*Af*CkypH;
      v2 =Af*SkypH;
      p=I*rho*w/k*Af*CkypH;

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else if( pdeOption==1 )
  {
    // ------------------------------
    // --- Viscous acoustic solid ---
    // ------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    if( option==0 ) u1Varies=0.;
    // omega=findRootViscousAcoustic( optionAcoustic,k, rhos, Hs, cp, cs, mu, rho, H, u1Varies );
    // printf("\n++findRootViscousAcoustic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e cs=%8.2e rho=%8.2e "
    // 	   "mu=%8.2e mus=%8.2e u1Varies=%f omega=(%16.10e,%16.10e)\n",k,rhos,Hs,cp,cs,rho,mu,mus,
    //        u1Varies,real(omega),imag(omega));

    // wr=real(omega);
    // wi=imag(omega);

    omega=cmplx(wr,wi);

    w=omega;

    // --- compute the constants
    cmplx Say,Cay, SaypH,CaypH;
  
    a=sqrt(k*k-I*rho*w/mu);

    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);

    cmplx bs = sqrt(k*k-w*w/(cs*cs) );
    cmplx Sbs=sinh(bs*Hs), Cbs=cosh(bs*Hs);
  

    double rcp2=rhos*cp*cp;
    double rcs2=rhos*cs*cs;

    cmplx Af,Bf, As,Bs;
    if( option==0 ) 
    {
      // -- single solid component ---
      cmplx a11=k*(k*Sa-a*Sk+(a*Ca*Sk-k*Ck*Sa)*Ck),     a12=a*(k*Sa-a*Sk+(a*Ca*Sk-k*Ck*Sa)*Ca), a13=0.;
      cmplx a21=(a*Ca*Sk-k*Ck*Sa)*Sk,                   a22=(a*Ca*Sk-k*Ck*Sa)*Sa,               a23=-I*w*Sas;
      cmplx a31= I*rho*w/k*(k*Sa+(a*Ca*Sk-k*Ck*Sa)*Ck), a32= I*rho*w/k*a*Sa,                    a33=rcp2*as*Cas;

      // solve for Af, Bf in terms of As
      //   [ a11 a12 ] = [-a13*As]
      //   [ a21 a22 ] = [-a23*As]
  

      cmplx det=a11*a22-a12*a21;
      As=1.; // choose As 
      Bs=0.;
      printf(" --> a=(%e,%e), as=(%e,%e), Adet=%e\n",real(a),imag(a),real(as),imag(as),abs(det));

      Af=(-a22*a13+a12*a23)*(As/det);
      Bf=( a21*a13-a11*a23)*(As/det);

      // -- scale so that |us|=1
      double scale=abs(As*sinh(as*(Hs)));
      Af /= scale;
      Bf /= scale;
      As /= scale;
    }
    else
    {
      // --- Acoustic solid 2 components ----

      cmplx a11,a12,a13,a14, a21,a22,a23,a24;
      cmplx a31,a32,a33,a34, a41,a42,a43,a44;

      cmplx II=I;
  
      a11 = II * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ck);
      a12 = II / k * a * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ca);
      a13 = 0;
      a14 = -II * w * Sbs;
      a21 = (a * Ca * Sk - k * Ck * Sa) * Sk;
      a22 = (a * Ca * Sk - k * Ck * Sa) * Sa;
      a23 = -II * w * Sas;
      a24 = 0;
      a31 = -2. * II * u1Varies * mu * k * (a * Ca * Sk - k * Ck * Sa) * Sk;
      a32 = -u1Varies * mu * (II / k * a*a * (a * Ca * Sk - k * Ck * 
					      Sa) * Sa + II * k * (a * Ca * Sk - k * Ck * Sa) * Sa);
      a33 = 0;
      a34 = 1 - u1Varies + u1Varies * rcs2 * bs * Cbs;
      a41 = II * rho * w / k * (k * Sa + (a * Ca * Sk - k * Ck * Sa) * Ck) 
	- 2 * mu * (k*k * Sa - k * a * Sk + (a * Ca * Sk - k * Ck * Sa) * k * Ck);
      a42 = II * rho * w / k * a * Sa - 2 * mu * (a * k * Sa - a*a * 
						  Sk + (a * Ca * Sk - k * Ck * Sa) * a * Ca);
      a43 = rcp2 * as * Cas;
      a44 = 0;
      
      // Unknowns [Af Bf As Bs ]
      // solve for Af, Bf, As in terms of Bs
      //   [ a11 a12 a13 ] = [-a14*Bs]
      //   [ a21 a22 a23 ] = [-a24*Bs]
      //   [ a31 a32 a33 ] = [-a34*Bs]

      
      cmplx det = a11*a22*a33-a11*a23*a32-a21*a12*a33+a21*a13*a32+a31*a12*a23-a31*a13*a22;
      Bs=1;
      Af = ( -(a22*a33-a23*a32)*a14+(a12*a33-a13*a32)*a24-(a12*a23-a13*a22)*a34 )*Bs/det;
      Bf = (  (a21*a33-a23*a31)*a14-(a11*a33-a13*a31)*a24+(a11*a23-a13*a21)*a34 )*Bs/det;
      As = ( -(a21*a32-a22*a31)*a14+(a11*a32-a12*a31)*a24-(a11*a22-a12*a21)*a34 )*Bs/det;
      

      // -- scale so that |us|=1
      double u1Norm = abs(-Bs*Sbs);
      double u2Norm = abs(-As*Sas);
      double scale= sqrt( u1Norm*u1Norm + u2Norm*u2Norm );

      Af /= scale;
      Bf /= scale;
      As /= scale;
      Bs /= scale;
    }

    cmplx aCSkCS=(a*Ca*Sk-k*Ck*Sa);

    // -- CHECK: evaluate the solution 
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  

      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));

      py=I*rho*w/k*(Af*(k*Sa*k*sinh(k*y)+aCSkCS*k*sinh(k*(y+H))) +Bf*a*Sa*k*sinh(k*y));

      u1=Bs*sinh(bs*(y-Hs));
      u2=As*sinh(as*(y-Hs));

// e1 := Af*k*( k*Sa -a*Sk + aCSkCS*Ck ) 
//      +Bf*a*( k*Sa -a*Sk + aCSkCS*Ca )
//      +I*w*u10:

      v1xx=-k*k*v1;
      v1y=I/k*(Af*k*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*k*sinh(k*(y+H)))+
               Bf*a*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*a*SaypH));
      v1yy=I/k*(Af*k*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*k*k*cosh(k*(y+H)))+
                Bf*a*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*a*a*CaypH));

      v2x=I*k*v2;
      v2xx=-k*k*v2;
      v2y =Af*(k*k*Sa*cosh(k*y)-k*a*Sk*Cay+aCSkCS*k*cosh(k*(y+H)))+
   	   Bf*(a*k*Sa*cosh(k*y)-a*a*Sk*Cay+aCSkCS*a*CaypH);
      v2yy=Af*(k*k*Sa*k*sinh(k*y)-k*a*a*Sk*Say+aCSkCS*k*k*sinh(k*(y+H)))+
   	   Bf*(a*k*Sa*k*sinh(k*y)-a*a*a*Sk*Say+aCSkCS*a*a*SaypH);


      u1y=Bs*bs*cosh(bs*(y-Hs));
      u2y=As*as*cosh(as*(y-Hs));

      // u1x=I*k*u1;
      // u2x=I*k*u2;
       
      if( y==0. )
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
        printf("     : traction: |rcp2**u2y -( -p+2*mu*v2y)|=%8.2e, |rcs2*u1y -mu*(v1y+v2x)|=%9.3e \n",
               abs(rhos*cp*cp*u2y - (-p+2.*mu*v2y)),
	       abs(rhos*cs*cs*u1y - mu*(v1y+v2x))
               );
      }
      else
      {
	// printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |rho*v1_t+i*k*p-mu*|, |rho*v2_t+p.y|\n",
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e |v1_momenutum|=%8.2e |v2_momenutum|=%8.2e\n",
	       y,abs(v1),abs(v2),
               abs(-I*w*rho*v1+I*k*p-mu*(v1xx+v1yy)),
               abs(-I*w*rho*v2+py   -mu*(v2xx+v2yy))
               );
      }
      
    }

    
    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      if( option==0 ) 
      {
        // --- 1 DOF acoustic solid ---
	us1=0.;
	us2=As*sinh(as*(y-Hs));
      
	// -- y-derivative (for computing the stress)
	us1y=0.;
	us2y=As*as*cosh(as*(y-Hs));
      }
      else
      { // --- 2 DOF acoustic solid ---

	us1=Bs*sinh(bs*(y-Hs));
	us2=As*sinh(as*(y-Hs));

	// -- y-derivative (for computing the stress)
	us1y=Bs*bs*cosh(bs*(y-Hs));
	us2y=As*as*cosh(as*(y-Hs));


        // -- ELASTIC: 

// 	cmplx Bs1=As;
// 	cmplx Ds1=Bs;
	
// 	cmplx As1 = -k*k*Sbs*(Bs1+Ds1)/(k*k*Cas*Sbs-as*bs*Cbs*Sas);
// 	cmplx Cs1 = as*bs*Sas*(Bs1+Ds1)/(k*k*Cas*Sbs-as*bs*Cbs*Sas);


// 	us1= As1*cosh(as*y)
// 	  +Bs1*cosh(as*(y-Hs))
// 	  +Cs1*cosh(bs*y)
// 	  +Ds1*cosh(bs*(y-Hs));

// 	us2=-I*(as*As1/k*sinh(as*y)
// 		+as*Bs1/k*sinh(as*(y-Hs))
// 		+k*Cs1/bs*sinh(bs*y)
// 		+k*Ds1/bs*sinh(bs*(y-Hs)));
      
// 	// -- y-derivative (for computing the stress)
// 	us1y =    As1*as*sinh(as*y)
// 	  +Bs1*as*sinh(as*(y-Hs))
// 	  +Cs1*bs*sinh(bs*y)
// 	  +Ds1*bs*sinh(bs*(y-Hs));

// 	us2y = -I*(as*As1/k*as*cosh(as*y)
// 		   +as*Bs1/k*as*cosh(as*(y-Hs))
// 		   +k*Cs1/bs*bs*cosh(bs*y)
// 		   +k*Ds1/bs*bs*cosh(bs*(y-Hs)));

      }
      

      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));


//       v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*cosh(k*(y+H)))+
//                Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*CaypH));
  

//       v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)))+
//          Bf*(a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH);

//       p=I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H)))+Bf*a*Sa*cosh(k*y));

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else if( pdeOption==2 )
  {
    // -------------------------------------
    // --- Viscous fluid / elastic solid ---
    // ------------------------------------

    int optionAcoustic=0;
    double u1Varies=1.;
    // omega=findRootViscousElastic( option,k, rhos, Hs, cp, cs, mu, rho, H,u1Varies );
    // printf("\n++findRootViscousElastic : root found: k=%e, rhos=%8.2e Hs=%8.2e cp=%8.2e rho=%8.2e "
    // 	   "mu=%8.2e H=%8.2e omega=(%18.14e,%18.14e)\n",k,rhos,Hs,cp,rho,mu,H,real(omega),imag(omega));


    // wr=real(omega);
    // wi=imag(omega);

    omega=cmplx(wr,wi);

    w=omega;

    // --- compute the constants
    cmplx Say,Cay, SaypH,CaypH;
  
    a=sqrt(k*k-I*rho*w/mu);

    Sk = sinh(k*H); Ck =cosh(k*H); Sa=sinh(a*H); Ca=cosh(a*H); 

    cmplx as = sqrt(k*k-w*w/(cp*cp) );
    cmplx Sas=sinh(as*Hs), Cas=cosh(as*Hs);

    cmplx bs = sqrt(k*k-w*w/(cs*cs) );
    cmplx Sbs=sinh(bs*Hs), Cbs=cosh(bs*Hs);
  

    double mus=rhos*cs*cs;
    double lambdas=rhos*cp*cp - 2.*mus;
  

    cmplx Af,Bf, As,Bs;

    cmplx a11,a12,a13,a14, a21,a22,a23,a24;
    cmplx a31,a32,a33,a34, a41,a42,a43,a44;

    cmplx II=I;
  
    a11 = II * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ck);
    a12 = II / k * a * (k * Sa - a * Sk + (a * Ca * Sk - k * Ck * Sa) * Ca);
    a13 = II * w * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cas + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas));
    a14 = II * w * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cbs);
    a21 = (a * Ca * Sk - k * Ck * Sa) * Sk;
    a22 = (a * Ca * Sk - k * Ck * Sa) * Sa;
    a23 = -w * as / k * Sas;
    a24 = -w * k / bs * Sbs;
    a31 = -2. * II * mu * k * (a * Ca * Sk - k * Ck * Sa) * Sk;
    a32 = -mu * (II / k * a*a * (a * Ca * Sk - k * Ck * Sa) * Sa + II * k * (a * Ca * Sk - k * Ck * Sa) * Sa);
    a33 = -2 * mus * as * Sas;
    a34 = mus * (-bs * Sbs - k*k / bs * Sbs);
    a41 = II * rho * w / k * (k * Sa + (a * Ca * Sk - k * Ck * Sa) * Ck) - 2 * mu * (k*k * Sa - k * a * Sk + (a * Ca * Sk - k * Ck * Sa) * k * Ck);
    a42 = II * rho * w / k * a * Sa - 2 * mu * (a * k * Sa - a*a * Sk + (a * Ca * Sk - k * Ck * Sa) * a * Ca);
    a43 = -II * (lambdas + 2 * mus) * (-as*as * k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as*as / k * Cas + k * as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas)) + II * lambdas * k * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cas + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas));
    a44 = -II * (lambdas + 2 * mus) * (-as*as * k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + k * as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + k * Cbs) + II * lambdas * k * (-k*k * Sbs / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + as * bs * Sas / (k*k * Cas * Sbs - as * bs * Cbs * Sas) + Cbs);
      


      
    // Unknowns [Af Bf As Bs ]
    // solve for Af, Bf, As in terms of Bs
    //   [ a11 a12 a13 ] = [-a14*Bs]
    //   [ a21 a22 a23 ] = [-a24*Bs]
    //   [ a31 a32 a33 ] = [-a34*Bs]

      
    cmplx det = a11*a22*a33-a11*a23*a32-a21*a12*a33+a21*a13*a32+a31*a12*a23-a31*a13*a22;
    Bs=1;
    Af = ( -(a22*a33-a23*a32)*a14+(a12*a33-a13*a32)*a24-(a12*a23-a13*a22)*a34 )*Bs/det;
    Bf = (  (a21*a33-a23*a31)*a14-(a11*a33-a13*a31)*a24+(a11*a23-a13*a21)*a34 )*Bs/det;
    As = ( -(a21*a32-a22*a31)*a14+(a11*a32-a12*a31)*a24-(a11*a22-a12*a21)*a34 )*Bs/det;
      

    // -- scale so that |us|=1
    double u1Norm = abs(-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)+As*Cas+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)+Bs*Cbs);
    double u2Norm=abs(-I*(-as*As/k*Sas-k*Bs/bs*Sbs));
    double scale=sqrt( u1Norm*u1Norm + u2Norm*u2Norm );

    Af /= scale;
    Bf /= scale;
    As /= scale;
    Bs /= scale;
    
    cmplx aCSkCS=(a*Ca*Sk-k*Ck*Sa);
  
    // -- evaluate the solution
    cmplx v1,v2,p,u1,u2, py,u1y,u2y, u1x,u2x, v1y,v2x,v2y, v1xx,v1yy, v2xx,v2yy;
    for( int iy=0; iy<3; iy++ )
    {
      double y= iy==0 ? -H : iy==1 ? -H*.5 : 0.;

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+(a*Ca*Sk-k*Ck*Sa)*cosh(k*(y+H)))+
	       Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+(a*Ca*Sk-k*Ck*Sa)*CaypH));

      v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+(a*Ca*Sk-k*Ck*Sa)*sinh(k*(y+H)))+
	Bf*(a*Sa*sinh(k*y)-a*Sk*Say+(a*Ca*Sk-k*Ck*Sa)*SaypH);

      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));

      py=I*rho*w/k*(Af*(k*Sa*k*sinh(k*y)+aCSkCS*k*sinh(k*(y+H))) +Bf*a*Sa*k*sinh(k*y));
    
      u1=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+As*cosh(as*(y-Hs))+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+Bs*cosh(bs*(y-Hs));
      u2=-I*(-as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(as*y)+as*As/k*sinh(as*(y-Hs))+k*as*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+k*Bs/bs*sinh(bs*(y-Hs)));
    
      v1xx=-k*k*v1;
      v1y=I/k*(Af*k*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*k*sinh(k*(y+H)))+
               Bf*a*( k*k*Sa*sinh(k*y)-a*a*Sk*Say+aCSkCS*a*SaypH));
      v1yy=I/k*(Af*k*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*k*k*cosh(k*(y+H)))+
                Bf*a*(k*k*k*Sa*cosh(k*y)-a*a*a*Sk*Cay+aCSkCS*a*a*CaypH));

      v2x=I*k*v2;
      v2xx=-k*k*v2;
      v2y =Af*(k*k*Sa*cosh(k*y)-k*a*Sk*Cay+aCSkCS*k*cosh(k*(y+H)))+
	Bf*(a*k*Sa*cosh(k*y)-a*a*Sk*Cay+aCSkCS*a*CaypH);
      v2yy=Af*(k*k*Sa*k*sinh(k*y)-k*a*a*Sk*Say+aCSkCS*k*k*sinh(k*(y+H)))+
	Bf*(a*k*Sa*k*sinh(k*y)-a*a*a*Sk*Say+aCSkCS*a*a*SaypH);


      u1x=I*k*u1;
      u1y=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*as*sinh(as*y)+As*as*sinh(as*(y-Hs))+as*bs*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+Bs*bs*sinh(bs*(y-Hs));
      u2x=I*k*u2;
      u2y=-I*(-as*as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+as*as*As/k*cosh(as*(y-Hs))+k*as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+k*Bs*cosh(bs*(y-Hs)));


      // u1x=I*k*u1;
      // u2x=I*k*u2;
       
      if( y==0. )
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
        printf("     : traction: |s22 -( -p+2*mu*v2y)|=%8.2e, |s12 -mu*(v1y+v2x)|=%9.3e \n",
               abs((lambdas+2.*mus)*u2y +lambdas*u1x - (-p+2.*mu*v2y)),
	       abs(mus*(u1y+u2x) - mu*(v1y+v2x))
	  );
      }
      else
      {
	// printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e, |rho*v1_t+i*k*p-mu*|, |rho*v2_t+p.y|\n",
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e |v1_momenutum|=%8.2e |v2_momenutum|=%8.2e\n",
	       y,abs(v1),abs(v2),
               abs(-I*w*rho*v1+I*k*p-mu*(v1xx+v1yy)),
               abs(-I*w*rho*v2+py   -mu*(v2xx+v2yy))
	  );
      }


      if( y==0. )
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |I*w*u1|=%8.2e, |v1-u1_t|=%9.3e, |v2|=%8.2e, |v2-u2_t|=%9.3e\n",
	       y,abs(v1),abs(I*w*u1),abs(v1+I*omega*u1),abs(v2),abs(v2+I*omega*u2));
      else
      {
	printf("CHECK: y=%8.2e: |v1|=%9.3e, |v2|=%9.3e\n",
	       y,abs(v1),abs(v2));
      }
      
    }
    
    // ---- evaluate the solid ----
    cmplx us1,us2,us1y,us2y;
    for( int i=0; i<ns; i++ )
    {
      double y = ys[i];  

      us1=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+As*cosh(as*(y-Hs))+as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+Bs*cosh(bs*(y-Hs));
      us2=-I*(-as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(as*y)+as*As/k*sinh(as*(y-Hs))+k*as*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+k*Bs/bs*sinh(bs*(y-Hs)));
    
      us1y=-k*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*as*sinh(as*y)+As*as*sinh(as*(y-Hs))+as*bs*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*sinh(bs*y)+Bs*bs*sinh(bs*(y-Hs));
      us2y=-I*(-as*as*k*Sbs*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(as*y)+as*as*As/k*cosh(as*(y-Hs))+k*as*bs*Sas*(As+Bs)/(k*k*Cas*Sbs-as*bs*Cbs*Sas)*cosh(bs*y)+k*Bs*cosh(bs*(y-Hs)));


      us1r[i]=real(us1);
      us1i[i]=imag(us1);
  
      us2r[i]=real(us2);
      us2i[i]=imag(us2);

      us1yr[i]=real(us1y);
      us1yi[i]=imag(us1y);
  
      us2yr[i]=real(us2y);
      us2yi[i]=imag(us2y);

    }
  
    // ---- evaluate the fluid ----
    for( int i=0; i<nf; i++ )
    {
      double y = yf[i];  

      Say=sinh(a*y), Cay=cosh(a*y);
      SaypH=sinh(a*(y+H)), CaypH=cosh(a*(y+H));
  
      v1 =I/k*( Af*k*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*cosh(k*(y+H)) )+
	        Bf*a*( k*Sa*cosh(k*y) -a*Sk*Cay +aCSkCS*CaypH) );

      v2=Af*( k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)) )+
         Bf*( a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH );
    
      p =I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H))) +Bf*a*Sa*cosh(k*y));


//       v1 =I/k*(Af*k*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*cosh(k*(y+H)))+
//                Bf*a*(k*Sa*cosh(k*y)-a*Sk*Cay+aCSkCS*CaypH));
  

//       v2=Af*(k*Sa*sinh(k*y)-k*Sk*Say+aCSkCS*sinh(k*(y+H)))+
//          Bf*(a*Sa*sinh(k*y)-a*Sk*Say+aCSkCS*SaypH);

//       p=I*rho*w/k*(Af*(k*Sa*cosh(k*y)+aCSkCS*cosh(k*(y+H)))+Bf*a*Sa*cosh(k*y));

      pr[i] = real( p );
      pi[i] = imag( p );
  
      v1r[i] = real( v1 );
      v1i[i] = imag( v1 );
  
      v2r[i] = real( v2 );
      v2i[i] = imag( v2 );
  
    }

  }
  else
  {
    printf("evalTravelingWave:ERROR: unknown pdeOption=%i",pdeOption);
  }
  
  

//   vr = real( v2h*exp(I*(k*x-w*t)) );
//   v1r= real( v1h*exp(I*(k*x-w*t)) );
//   pr = real( ph*exp(I*(k*x-w*t)) );

//   etar = real( eta2h*exp(I*(k*x-w*t)) );


  return 0;
}



