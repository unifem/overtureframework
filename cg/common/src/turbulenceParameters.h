#ifndef TURBULENCE_PARAMETERS_H
#define TURBULENCE_PARAMETERS_H

#define getSpalartAllmarasParameters EXTERN_C_NAME(getspalartallmarasparameters)
#define getKEpsilonParameters EXTERN_C_NAME(getkepsilonparameters)
extern "C" 
{
void 
getSpalartAllmarasParameters( real & cb1, real & cb2, real & cv1, real & sigma, real & sigmai,
			      real & kappa, real & cw1, real & cw2, real & cw3,
			      real & cw3e6, real & cv1e3, real & cd0, real & cr0 );
void 
getKEpsilonParameters( real & cMu, real & cEps1, real & cEps2, real & sigmaEpsI, real & sigmaKI );

}

#endif
