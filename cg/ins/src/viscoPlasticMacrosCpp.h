// Define visco-plastic model for C++ files

//   **   NOTE: you should also change viscoPlasticMacros.h if you change this file ***

// define the viscosity and it's derivatives with respect to esr^2 (used by ins.C, insp.C)
#define defineViscoPlasticCoefficients(esr) \
 exp0 = exp(-exponentViscoPlastic*esr); \
 nuT = (etaViscoPlastic + (yieldStressViscoPlastic/esr)*(1.-exp0)); \
 nuTd = .5*( (-1./esr)*(1.-exp0) + exponentViscoPlastic*exp0  )*(yieldStressViscoPlastic/(esr*esr)); \
   /* -- fake -- */ \
 /* nuT = nu+ nuViscoPlastic*esr*esr; */ \
 /* nuTd=nuViscoPlastic; */ 


// define the viscosity and it's derivatives with respect to esr^2 (used by insp.C)
#define defineViscoPlasticCoefficientsAndTwoDerivatives(esr) \
 exp0 = exp(-exponentViscoPlastic*esr); \
 nuT = (etaViscoPlastic + (yieldStressViscoPlastic/esr)*(1.-exp0)); \
 nuTd = .5*( (-1./esr)*(1.-exp0) + exponentViscoPlastic*exp0  )*(yieldStressViscoPlastic/(esr*esr)); \
 nuTdd= .25*( 3./(esr*esr)*(1.-exp0) -3./(esr)*exponentViscoPlastic*exp0 - exponentViscoPlastic*exponentViscoPlastic*exp0 )*(yieldStressViscoPlastic/(esr*esr*esr))

   /* -- fake -- */ \
 /* nuT = nu+ nuViscoPlastic*esr*esr;  */ \
 /* nuTd=nuViscoPlastic;  */ \
 /* nuTdd=0. */



// declare and lookup visco-plastic parameters  (used by ins.C, insp.C)
#define declareViscoPlasticParameters \
    /* visco-plastic parameters */ \
    real nuViscoPlastic=1., etaViscoPlastic=1., yieldStressViscoPlastic=10., exponentViscoPlastic=10.; \
    real epsViscoPlastic=1.e-10; /* small parameter used to offset the effective strain rate */ \
    parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("nuViscoPlastic",nuViscoPlastic); \
    parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("etaViscoPlastic",etaViscoPlastic); \
    parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("yieldStressViscoPlastic",yieldStressViscoPlastic); \
    parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("exponentViscoPlastic",exponentViscoPlastic); \
    parameters.dbase.get<ListOfShowFileParameters>("pdeParameters").getParameter("epsViscoPlastic",epsViscoPlastic);

// ===============================================================
// Here is effective strain rate (plus a small value)
//         sqrt( (2/3)*eDot_ij eDot_ij ) + epsVP
// Also define the derivatives of the square of the effective strain rate
//  
// ===============================================================
#define strainRate2d(u0x,u0y,v0x,v0y) (sqrt( (2./3.)*( SQR(u0x) + SQR(v0y) + .5*SQR( u0y + v0x ) ) )+epsViscoPlastic)

#define strainRate3d(u0x,u0y,u0z, v0x,v0y,v0z, w0x,w0y,w0z) \
      (sqrt( (2./3.)*( SQR(u0x) + SQR(v0y) + SQR(w0z) + .5*( SQR( u0y + v0x ) + SQR( u0z + w0x )+ SQR( v0z + w0y )) ) )+epsViscoPlastic)

// #define strainRate2dSqx() \
//         ( (2./3.)*( 2.*u0x*u0xx + 2.*v0y*v0xy + ( u0y + v0x )*( u0xy+v0xx ) ) )
// #define strainRate2dSqy() \
//         ( (2./3.)*( 2.*u0x*u0xy + 2.*v0y*v0yy + ( u0y + v0x )*( u0yy+v0xy ) ) )
// 
// #define strainRate2dSqxx() \
//         ( (2./3.)*( 2.*(u0xx*u0xx+u0x*u0xxx) + 2.*(v0xy*v0xy+v0y*v0xxy) \
//              + ( u0xy + v0xx )*( u0xy+v0xx ) + ( u0y + v0x )*( u0xxy+v0xxx ) ) )
// 
// #define strainRate2dSqxy() \
//         ( (2./3.)*( 2.*(u0xy*u0xx+u0x*u0xxy) + 2.*(v0yy*v0xy+v0y*v0xyy) \
//              + ( u0yy + v0xy )*( u0xy+v0xx ) + ( u0y + v0x )*( u0xyy+v0xxy ) ) )
// 
// #define strainRate2dSqyy() \
//         ( (2./3.)*( 2.*(u0xy*u0xy+u0x*u0xyy) + 2.*(v0yy*v0yy+v0y*v0yyy) \
//              + ( u0yy + v0xy )*( u0yy+v0xy ) + ( u0y + v0x )*( u0yyy+v0xyy ) ) )
