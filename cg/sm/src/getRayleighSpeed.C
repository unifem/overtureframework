#include <iostream>
#include <complex>
#include <stdio.h>
using namespace std;


// ====================================================================================
/// \brief Return the Rayleigh wave speed. See cgDoc/sm/notes.pdf
// This function is put in a separate file so we can use complex numbers (there are some
// conflicts with Overture names such as "real").
// ====================================================================================
double 
getRayleighSpeed( double rho, double mu, double lambda )
{
  
  double gamma = mu/(lambda+2.*mu);
  complex<double> R = (2./27.)*( 27. + gamma*(-90. + gamma*( 99. + gamma*(-32.))));
  complex<double> D = (4./27.)*(1.-gamma)*(1.-gamma)*( 11. + gamma*(-62. + gamma*(107. + gamma*(-64.))));
	
  complex<double> cb = 4.*(1.-gamma)/( 2.- 4.*gamma/3. + pow( R+sqrt(D), 1./3.) + pow( R-sqrt(D), 1./3.) );


  complex<double> crc = sqrt( (mu/rho)*cb );

  printf("getRayleighSpeed: mu=%e, lambda=%e, rho=%e\n",mu,lambda,rho,gamma);
  cout << "Complex Rayleigh speed: crc=" << crc << endl;
  return real(crc);
}

