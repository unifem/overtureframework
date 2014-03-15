// Here we define the function that is to be called by the flow solver to implement
// the twilight zone flows. This function will take a location in space/time
// and return the associated vector right hand side forcing function.
//#include "OB_Parameters.h"
#include "OGPolyFunction.h"
#include "MappedGridOperators.h"

#define TZSOURCE EXTERN_C_NAME(tzsource)
#define GETCON   EXTERN_C_NAME(getcon)
#define GETP2D   EXTERN_C_NAME(getp2d)
#define GETCON3D EXTERN_C_NAME(getcon3d)
#define GETP3D   EXTERN_C_NAME(getp3d)
#define MVARS EXTERN_C_NAME(mvars)
#define viscosityCoefficients EXTERN_C_NAME(viscositycoefficients)

extern "C" 
{
  void TZSOURCE(int *dimensionP, int64_t *exactP,
		real *xP, real *yP, real *zP,
		real *tP, real *h, int *nsP);

  void GETCON(int & i, real  & uprim, real & ucon, int & ier); 

  void GETP2D(int & m, real & u, real & press, real & dp, int & ns,
	      real & T, int & ier);

  void GETCON3D( int & i, real  & uprim, real & ucon, int & ier); 

  void GETP3D( int & m, real & u, real & press, real & dp,
	       int & ns, real & T, int & ier );

  extern struct{ int mh,mr,me,ieos,irxn,imult,islope,ifix,ivisco;} MVARS;

  extern struct{ real amu,akappa,cmu1,cmu2,cmu3,ckap1,ckap2,ckap3;} viscosityCoefficients;

}

void TZSOURCE (int *dimensionP, int64_t *exactP,
	       real *xP, real *yP, real *zP,
	       real *tP, real *h, int *nsP)
{
  OGFunction & ex = *((OGFunction*)(*exactP));
  real x=*xP, y=*yP, z=*zP, t=*tP;
  int dimension=*dimensionP;
  int ns=*nsP;
  int i,j, m, derivs;
  int rc, uc, vc, wc, tc, sc;
  int noDeriv, tDeriv, xDeriv, yDeriv, zDeriv, iDeriv;
  
  // first set the index variables
  noDeriv=0;
  tDeriv=1;
  xDeriv=2;
  yDeriv=3;
  zDeriv=4;
  
  if( dimension == 2 )
  {
    rc = 0;
    uc = 1;
    vc = 2;
    tc = 3;
    sc = 4;
    m=ns+4;
  }
  else
  {
    rc = 0;
    uc = 1;
    vc = 2;
    wc = 3;
    tc = 4;
    sc = 5;
    m=ns+5;
  }
  derivs=ns+2;

  // vectors hold the value followed by d/dt, d/dx, d/dy, d/dz
  // allocate space according to what we could potentially need in 3-D
  real rho[5];
  real velocity[3][5];
  real temperature[5];
  real species[5][5];
  real pressure[5];

  // get rho and derivatives
  rho[noDeriv] = ex(x,y,z,rc,t);
  rho[tDeriv] = ex.t(x,y,z,rc,t);
  rho[xDeriv] = ex.x(x,y,z,rc,t);
  rho[yDeriv] = ex.y(x,y,z,rc,t);
  if( dimension == 3 ) rho[zDeriv] = ex.z(x,y,z,rc,t);
  
  // get velocities and derivatives
  for( i=0; i<dimension; i++ )
  {
    velocity[i][noDeriv] = ex(x,y,z,i+uc,t);
    velocity[i][tDeriv] = ex.t(x,y,z,i+uc,t);
    velocity[i][xDeriv] = ex.x(x,y,z,i+uc,t);
    velocity[i][yDeriv] = ex.y(x,y,z,i+uc,t);
    if( dimension == 3) velocity[i][zDeriv] = ex.z(x,y,z,i+uc,t);
  }

  // Get T and derivatives ... note P=rho*T so T is NOT temperature
  temperature[noDeriv] = ex(x,y,z,tc,t);
  temperature[tDeriv] = ex.t(x,y,z,tc,t);
  temperature[xDeriv] = ex.x(x,y,z,tc,t);
  temperature[yDeriv] = ex.y(x,y,z,tc,t);
  if( dimension == 3 ) temperature[zDeriv] = ex.z(x,y,z,tc,t);

  // get P and derivatives
  pressure[noDeriv] = rho[noDeriv]*temperature[noDeriv];
  for( iDeriv=1; iDeriv<dimension+2; iDeriv++ )
  {
    pressure[iDeriv] = rho[iDeriv]*temperature[noDeriv]+rho[noDeriv]*temperature[iDeriv];
  }

  // get species derivatives
  for( i=0; i<ns; i++ )
  {
    species[i][noDeriv] = ex(x,y,z,i+sc,t);
    species[i][tDeriv] = ex.t(x,y,z,i+sc,t);
    species[i][xDeriv] = ex.x(x,y,z,i+sc,t);
    species[i][yDeriv] = ex.y(x,y,z,i+sc,t);
    if( dimension == 3) species[i][zDeriv] = ex.z(x,y,z,i+sc,t);
  }

  real uprim[12], ucon[12], dp[10];
  uprim[rc] = rho[noDeriv];
  for( i=0; i<dimension; i++ )
  {
    uprim[i+uc] = velocity[i][noDeriv];
  }
  uprim[tc] = pressure[noDeriv];
  for( i=0; i<ns; i++ )
  {
    uprim[i+sc] = species[i][noDeriv];
  }

  real PTemp, TTemp;
  if( dimension == 2 )
  {  
    GETCON( m, *uprim, *ucon, i );
    GETP2D( m, *ucon, PTemp, *dp, derivs, TTemp, i );
  }
  else
  {
    GETCON3D( m, *uprim, *ucon, i );
    GETP3D( m, *ucon, PTemp, *dp, derivs, TTemp, i );
  }
  
  // get derivatives of density*(internal energy)
  real rho_e[5]; // don't use rho_e[0]
  for( iDeriv=1; iDeriv<dimension+2; iDeriv++ )
  {
    rho_e[iDeriv]=pressure[iDeriv]-dp[0]*rho[iDeriv];
    for( i=0; i<ns; i++ )
    {
      rho_e[iDeriv]+=-dp[i+2]*(rho[noDeriv]*species[i][iDeriv]+rho[iDeriv]*species[i][noDeriv]);
    }
    rho_e[iDeriv]=rho_e[iDeriv]/dp[1];
  }

  // get derivatives of rho*u^2, rho*v^2, rho*w^2
  //  here we don't use rho_velSquare[0][*]
  real rho_velSquare[3][5];
  for( iDeriv=1; iDeriv<dimension+2; iDeriv++ )
  {
    for( i=0; i<dimension; i++ )
    {
      rho_velSquare[i][iDeriv]=2.0*rho[noDeriv]*velocity[i][noDeriv]*velocity[i][iDeriv]+
	                       rho[iDeriv]*velocity[i][noDeriv]*velocity[i][noDeriv];
    }
  }

  // get derivatives of total energy E=rho*e+0.5*rho*(u^2+...+w^2)
  real E[5];
  E[noDeriv]=ucon[tc];
  for( iDeriv=1; iDeriv<dimension+2; iDeriv++ )
  {
    E[iDeriv]=rho_e[iDeriv];
    for( i=0; i<dimension; i++ )
    {
      E[iDeriv]+=0.5*rho_velSquare[i][iDeriv];
    }
  }

  // begin to accumulate the source term into h ... first set h=du/dt
  h[rc]=rho[tDeriv];

  for( i=0; i<dimension; i++ )
  {
    h[i+uc]=rho[noDeriv]*velocity[i][tDeriv]+rho[tDeriv]*velocity[i][noDeriv];
  }

  h[tc]=E[tDeriv];

  for( i=0; i<ns; i++ )
  {
    h[i+sc]=rho[noDeriv]*species[i][tDeriv]+rho[tDeriv]*species[i][noDeriv];
  }

  // now accumulate the fluxes from each directions into h
  for( i=0; i<dimension; i++ )
  {
    int dir=i+2;
    h[rc]+=rho[noDeriv]*velocity[i][dir]+rho[dir]*velocity[i][noDeriv];

    //  we leave the pressure term in momentum equations until later ...
    for( j=0; j<dimension; j++ )
    {
      h[j+uc]+=rho[dir]    *velocity[i][noDeriv]*velocity[j][noDeriv]+ 
               rho[noDeriv]*velocity[i][dir]    *velocity[j][noDeriv]+
               rho[noDeriv]*velocity[i][noDeriv]*velocity[j][dir];
    }
    // now we add in the pressure terms from the momentum equations
    h[i+uc]+=pressure[dir];

    h[tc]+=velocity[i][noDeriv]*(E[dir]+pressure[dir])+
           velocity[i][dir]*(E[noDeriv]+pressure[noDeriv]);

    for( j=0; j<ns; j++ )
    {
      h[j+sc]+=rho[dir]    *velocity[i][noDeriv]*species[j][noDeriv]+ 
               rho[noDeriv]*velocity[i][dir]    *species[j][noDeriv]+
               rho[noDeriv]*velocity[i][noDeriv]*species[j][dir];
    }
  }

  //... add on viscosity terms. This is not working in 3-D yet and is ugly.
  if( MVARS.ivisco == 1 && dimension < 3 )
  {
    real uxx = ex.xx(x,y,z,uc,t);
    real uxy = ex.xy(x,y,z,uc,t);
    real uyy = ex.yy(x,y,z,uc,t);
    real vxx = ex.xx(x,y,z,vc,t);
    real vxy = ex.xy(x,y,z,vc,t);
    real vyy = ex.yy(x,y,z,vc,t);
    real Txx = ex.xx(x,y,z,tc,t);
    real Tyy = ex.yy(x,y,z,tc,t);

    real tau[2][2],q,dtau[2][2],dq;
    real amu,ak,cmu1,cmu2,cmu3,ckap1,ckap2,ckap3;
    real mu,kappa,dmu,dkappa;

    amu=viscosityCoefficients.amu;
    ak=viscosityCoefficients.akappa;
    cmu1=viscosityCoefficients.cmu1;
    cmu2=viscosityCoefficients.cmu2;
    cmu3=viscosityCoefficients.cmu3;
    ckap1=viscosityCoefficients.ckap1;
    ckap2=viscosityCoefficients.ckap2;
    ckap3=viscosityCoefficients.ckap3;
    
    real T=temperature[noDeriv];
    real Tx=temperature[xDeriv];
    real Ty=temperature[yDeriv];
    real u=velocity[0][noDeriv];
    real ux=velocity[0][xDeriv];
    real uy=velocity[0][yDeriv];
    real v=velocity[1][noDeriv];
    real vx=velocity[1][xDeriv];
    real vy=velocity[1][yDeriv];

    mu=amu*(cmu1+cmu2*pow(T,cmu3));
    kappa=ak*(ckap1+ckap2*pow(T,ckap3));

    tau[0][0]=2./3.*mu*(2.*ux-vy);
    tau[0][1]=mu*(vx+uy);
    tau[1][0]=tau[0][1];
    tau[1][1]=2./3.*mu*(2.*vy-ux);
    
    // set up x-direction viscosity
    q=-kappa*Tx;
    dmu=amu*cmu2*cmu3*pow(T,cmu3-1.)*Tx;
    dkappa=ak*ckap2*ckap3*pow(T,ckap3-1.)*Tx;
    dtau[0][0]=2./3.*(2.*(dmu*ux+mu*uxx)-(dmu*vy+mu*vxy));
    dtau[0][1]=dmu*(vx+uy)+mu*(vxx+uxy);
    dtau[1][0]=dtau[0][1];
    dtau[1][1]=2./3.*(2.*(dmu*vy+mu*vxy)-(dmu*ux-mu*uxx));
    dq=-dkappa*Tx-kappa*Txx;
    
    // add on x-direction viscosity
    h[1]-=dtau[0][0];
    h[2]-=dtau[0][1];
    h[3]-=ux*tau[0][0]+u*dtau[0][0]+vx*tau[0][1]+v*dtau[0][1]-dq;

    // set up y-direction viscosity
    q=-kappa*Ty;
    dmu=amu*cmu2*cmu3*pow(T,cmu3-1.)*Ty;
    dkappa=ak*ckap2*ckap3*pow(T,ckap3-1.)*Ty;
    dtau[0][0]=2./3.*(2.*(dmu*ux+mu*uxy)-(dmu*vy+mu*vyy));
    dtau[0][1]=dmu*(vx+uy)+mu*(vxy+uyy);
    dtau[1][0]=dtau[0][1];
    dtau[1][1]=2./3.*(2.*(dmu*vy+mu*vyy)-(dmu*ux-mu*uxy));
    dq=-dkappa*Ty-kappa*Tyy;

    // add on y-direction viscosity
    h[1]-=dtau[1][0];
    h[2]-=dtau[1][1];
    h[3]-=uy*tau[1][0]+u*dtau[1][0]+vy*tau[1][1]+v*dtau[1][1]-dq;
  }

  return;
}
