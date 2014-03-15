// This file automatically generated from forcing.bC with bpp.
#include "Cgsm.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "interpPoints.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"

#define scatCyl EXTERN_C_NAME(scatcyl)
#define scatSphere EXTERN_C_NAME(scatsphere)
#define exmax EXTERN_C_NAME(exmax)
#define forcingOptSolidMechanics EXTERN_C_NAME(forcingoptsolidmechanics)

extern "C"
{
  void scatCyl(const int&nd ,
             	     const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,const int&nd1a,
             	     const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
             	     const real& xy, real&u, const int&ipar, const real&rpar );

  void scatSphere(const int&nd ,
             	     const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,const int&nd1a,
             	     const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,
             	     const real& xy, real&u, const int&ipar, const real&rpar );

  void exmax(double&Ez,double&Bx,double&By,const int &nsources,const double&xs,const double&ys,
                        const double&tau,const double&var,const double&amp, const double&a,
                        const double&x,const double&y,const double&time);


    void forcingOptSolidMechanics(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
            real&u, const real&f, const int&mask, const real&rx, const real&x,  
            const int&ipar, const real&rpar, int&ierr );
}

#define OGF EXTERN_C_NAME(ogf)
#define OGF2D EXTERN_C_NAME(ogf2d)
#define OGDERIV EXTERN_C_NAME(ogderiv)
#define OGDERIV3 EXTERN_C_NAME(ogderiv3)
#define OGDERIV2 EXTERN_C_NAME(ogderiv2)
#define OGF3D EXTERN_C_NAME(ogf3d)
#define EXX EXTERN_C_NAME(exx)

extern "C"
{

/* Here are functions for TZ flow that can be called from fortran */

real
OGF(OGFunction *&ep, const real &x, const real &y,const real &z, const int & c, const real & t )
{
    return (*ep)(x,y,z,c,t);
}


/* return (u,v,w) = (Ex,Ey,Hz) */
void
OGF2D(OGFunction *&ep, const real &x, const real &y, const real & t, real & u, real & v )
{
    /* assumes ex=0, ey=1, hz=2 */
    u=(*ep)(x,y,0.,0,t);
    v=(*ep)(x,y,0.,1,t);
}

/* return a general derivative */
void
OGDERIV(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
                  const real &x, const real &y, const real &z, const real & t, const int & n, real & ud )
{
    ud=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
}

/* return a general derivative for 3 components */
void
OGDERIV3(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
                  const real &x, const real &y, const real &z, const real & t, 
                  const int & n1, real & ud1, const int & n2, real & ud2, const int & n3, real & ud3 )
{
    ud1=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n1,t);
    ud2=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n2,t);
    ud3=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n3,t);
}

/* return a general derivative for 2 components */
void
OGDERIV2(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
                  const real &x, const real &y, const real &z, const real & t, 
                  const int & n1, real & ud1, const int & n2, real & ud2 )
{
    ud1=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n1,t);
    ud2=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n2,t);
}

/* return (u,v,w) */
void
OGF3D(OGFunction *&ep, const real &x, const real &y, const real &z, const real & t, real & u, real & v, real & w )
{
    /* assumes ex=0, ey=1, hz=2 */
    u=(*ep)(x,y,z,0,t);
    v=(*ep)(x,y,z,1,t);
    w=(*ep)(x,y,z,2,t);
}

real
EXX(OGFunction *&ep, const real &x, const real &y,const real &z, const int & c, const real & t )
{
    real value=(*ep).xx(x,y,z,c,t);
  // printF("exx: x=(%8.2e,%8.2e,%8.2e) c=%i t=%8.2e ...exx=%8.2e \n",x,y,z,c,t,value);
    return value;
}

}


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// Define macros for forcing functions:
// ==================================================================================
// Macro to evaluate the traveling (shock) wave solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================

// ==================================================================================
// Macro to evaluate the traveling (sine) wave solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================


// ==========================================================================
//  Define a Rayleigh surface wave: (see cgDoc/sm/notes.pdf)
//     u1 = SUM_n a_n [ exp(-b1(k_n)*y + ...  ] cos( 2*pi*k_n (x-c*t) )
//     u2 = SUM_n a_n [                       ] sin( 2*pi*k_n (x-c*t) )
//
//  Here we assume that the solid occupies the space y<= ySurf
// where ySurf is given by the user. 
// ==========================================================================

// =======================================================================================
//  The function "fg" is basically the integral appearing in the D'Alambert solution
// ======================================================================================


// ===========================================================
// Evaluate the D'Alambert function "f" and it's derivative
//  Here we assume that u(x,0)=0 and v(x,0)!=0  
// ===========================================================

// ===========================================================
// Evaluate the D'Alambert function "g"
//  Here we assume that u(x,0)=0 and v(x,0)!=0  
// ===========================================================



// ==========================================================================
//  Define the pistonMotion solution (see cgDoc/mp/fluidStructure/fsm.tex)
// ==========================================================================








//   (Ex).t = (1/eps)*[  (Hz).y ]
//   (Ey).t = (1/eps)*[ -(Hz).x ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

#define exTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-ky/(eps*cc))
#define eyTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( kx/(eps*cc))
#define hzTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))

#define exLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(+ky*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define eyLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-kx*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define hzLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( -(twoPi*twoPi*(kx*kx+ky*ky) ) )

// define eyTrue(x,y,t) exp( -beta*SQR((x-x0)-c*(t)) )

// Here is a plane wave with the shape of a Gaussian
// xi = kx*(x)+ky*(y)-cc*(t)
// cc=  c*sqrt( kx*kx+ky*ky );
#define hzGaussianPulse(xi)  exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exGaussianPulse(xi)  hzGaussianPulse(xi)*(-ky/(eps*cc))
#define eyGaussianPulse(xi)  hzGaussianPulse(xi)*( kx/(eps*cc))

#define hzLaplacianGaussianPulse(xi)  ((4.*betaGaussianPlaneWave*betaGaussianPlaneWave*(kx*kx+ky*ky))*xi*xi-(2.*betaGaussianPlaneWave*(kx*kx+ky*ky)))*exp(-betaGaussianPlaneWave*((xi)*(xi)))
#define exLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*(-ky/(eps*cc))
#define eyLaplacianGaussianPulse(xi)  hzLaplacianGaussianPulse(xi,t)*( kx/(eps*cc))

// 3D
// E:
//   u.tt = (1/eps)*[ ((1/mu)*u.x).x + ((1/mu)*u.y).y + ((1/mu)*u.z).z ]
//   div(u)=0
// H
//   v.tt = (1/mu)*[ ((1/eps)*v.x).x + ((1/eps)*v.y).y + ((1/eps)*v.z).z ]
// Define macros for forcing functions


//
//   (Ex).t = (1/eps)*[ (Hz).y - (Hy).z ]
//   (Ey).t = (1/eps)*[ (Hx).z - (Hz).x ]
//   (Ez).t = (1/eps)*[ (Hy).x - (Hx).y ]
//   (Hx).t = (1/mu) *[ (Ey).z - (Ez).y ]
//   (Hy).t = (1/mu) *[ (Ez).x - (Ex).z ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

// ****************** finish this -> should `rotate' the 2d solution ****************

#define exTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-ky/(eps*cc))
#define eyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*( kx/(eps*cc))
#define ezTrue3d(x,y,z,t) 0

#define hxTrue3d(x,y,z,t) 0
#define hyTrue3d(x,y,z,t) 0
#define hzTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))

#define exLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(+ky*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define eyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-kx*(twoPi*twoPi*(kx*kx+ky*ky))/(eps*cc))
#define ezLaplacianTrue3d(x,y,z,t) 0

#define hxLaplacianTrue3d(x,y,z,t) 0
#define hyLaplacianTrue3d(x,y,z,t) 0
#define hzLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*( -(twoPi*twoPi*(kx*kx+ky*ky) ) )


// ------------ macros for the plane material interface -------------------------


// OPTION: initialCondition, error, boundaryCondition

//==================================================================================================
// Evaluate Tom Hagstom's exact solution defined as an integral of Guassian sources
// 
// OPTION: OPTION=solution or OPTION=error OPTION=bounary to compute the solution or the error or
//     the boundary condition
//
//==================================================================================================



// //! Return the true solution for the electric field
// void SolidMechanics::
// getField( real x, real y, real t, real *eField )
// {
//   const real cc= c*sqrt( kx*kx+ky*ky +kz*kz );
//   eField[0]=exTrue(x,y,t);
//   eField[1]=eyTrue(x,y,t);
    
// }






int Cgsm::
getForcing(int current, int grid, realArray & u , real t, real dt, int option /* = 0 */ )
// ========================================================================================
//
//  Add the forcing:
//
// option==0 : add forcing to u
//     u+=   utt - c^2*Lap(u)
// option==1 : fill in u with the forcing
//     u =   utt - c^2*Lap(u) 
//
// kkc
// option==2 : u is h field and use += ( ==0 is efield )
// option==3 : u is h field and use == ( ==1 is efield )
// ========================================================================================
{
  // Just return if there is no forcing
    bool computeForcing = (forcingOption==twilightZoneForcing || 
                   			 forcingOption==gaussianSource ||
                                                  forcingOption==gaussianChargeSource ||
                                                  forcingOption==userDefinedForcingOption );
    
    if( !computeForcing )
        return 0;
    
    real time0=getCPU();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");

    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    const int numberOfDimensions = cg.numberOfDimensions();

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    int & debug = parameters.dbase.get<int >("debug");

    if( forcingOption==userDefinedForcingOption )
    {
        int iparam[10];
        real rparam[10];
        iparam[0]=grid;
        rparam[1]=t;
        
        realMappedGridFunction & uCurrent = gf[current].u[grid];
        if( option==0 )
        {
      // add forcing to u: 
            realArray f;
            f.redim(u);
            userDefinedForcing( f, uCurrent, iparam, rparam );

        #ifdef USE_PPP
            realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
            realSerialArray fLocal;  getLocalArrayWithGhostBoundaries(f,fLocal);
        #else
            realSerialArray & uLocal  =  u;
            const realSerialArray & fLocal  =  f;
        #endif

            uLocal += fLocal;

        }
        else if( option==1 )
        {
      // Set u to be the forcing
            userDefinedForcing( u, uCurrent, iparam, rparam );
        }
        else
        {
            OV_ABORT("getForcing: invalid option!");
        }
        

    }
    else if( forcingOption==twilightZoneForcing || 
         	   forcingOption==gaussianSource ||
         	   forcingOption==gaussianChargeSource )
    {
        
        MappedGrid & mg = cg[grid];
        const bool isRectangular = mg.isRectangular();
        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
        
    // we always need the center array for TZ forcing:
    // const realArray & x = (isRectangular && forcingOption!=twilightZoneForcing) ? u : mg.center();
        const realArray & x = mg.center();

        #ifdef USE_PPP
            realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
            realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
            const realSerialArray & uLocal  =  u;
            const realSerialArray & xLocal  =  x;
        #endif

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
        #undef U
        #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

        real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
        const int xDim0=xLocal.getRawDataSize(0);
        const int xDim1=xLocal.getRawDataSize(1);
        const int xDim2=xLocal.getRawDataSize(2);
#undef X
#define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

        int i1,i2,i3;
        Index I1,I2,I3;

        getIndex(mg.gridIndexRange(),I1,I2,I3);

        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
        if( !ok ) return 0;
      
        lambda = lambdaGrid(grid);
        mu = muGrid(grid);
        c1=(mu+lambda)/rho, c2= mu/rho;

        Range C=numberOfComponents;

        if( forcingOption==twilightZoneForcing )
        {
            OGFunction *& tz = parameters.dbase.get<OGFunction* >("exactSolution");
            assert( tz!=NULL );
            OGFunction & e = *tz;

            const int ntd=2, nxd=0, nyd=0, nzd=0; // we need the 2nd time derivative


            int i1,i2,i3;
            real x0,y0,z0=0.;
        	  
            if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0  )
            {
	// -- Variable material properties ---
      	const int rhoc = parameters.dbase.get<int>("rhoc");
      	const int muc = parameters.dbase.get<int>("muc");
      	const int lambdac = parameters.dbase.get<int>("lambdac");
      	assert( rhoc>=0 && muc>=0 && lambdac>=0 );

      	Range D(uc,uc+numberOfDimensions-1);  // displacement components
                realSerialArray   f(I1,I2,I3,D);
                realSerialArray utt(I1,I2,I3,D),  ux(I1,I2,I3,D),  uy(I1,I2,I3,D);
                realSerialArray uxx(I1,I2,I3,D), uxy(I1,I2,I3,D), uyy(I1,I2,I3,D);

                realSerialArray rho(I1,I2,I3);
                realSerialArray  mu(I1,I2,I3),  mux(I1,I2,I3),  muy(I1,I2,I3);
                realSerialArray lam(I1,I2,I3), lamx(I1,I2,I3), lamy(I1,I2,I3);
      	
      	
        // Evaluate the material parameters: 
                e.gd(rho ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,rhoc   ,t);
                e.gd(mu  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,muc    ,t);
                e.gd(lam ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,lambdac,t);

                e.gd(mux ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,muc    ,t);
                e.gd(lamx,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,lambdac,t);

                e.gd(muy ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,muc    ,t);
                e.gd(lamy,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,lambdac,t);

        // Evaluate the derivatives of the exact solution: 
                e.gd(utt,xLocal,numberOfDimensions,isRectangular,2,0,0,0,I1,I2,I3,D,t);
                e.gd(ux ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,D,t);
                e.gd(uy ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,D,t);

                e.gd(uxx,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,D,t);
                e.gd(uxy,xLocal,numberOfDimensions,isRectangular,0,1,1,0,I1,I2,I3,D,t);
                e.gd(uyy,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,D,t);

      	if( numberOfDimensions==2 )
      	{
        	  f(I1,I2,I3,uc) = utt(I1,I2,I3,uc) 
          	    -( (lam+2.*mu)*uxx(I1,I2,I3,uc) + (lamx+2.*mux)*ux(I1,I2,I3,uc)
             	       + (lam+mu)*uxy(I1,I2,I3,vc) + lamx*uy(I1,I2,I3,vc)+ muy*ux(I1,I2,I3,vc)
             	       + mu*uyy(I1,I2,I3,uc) + muy*uy(I1,I2,I3,uc) )/rho;

        	  f(I1,I2,I3,vc) = utt(I1,I2,I3,vc) 
          	    -( (lam+2.*mu)*uyy(I1,I2,I3,vc) + (lamy+2.*muy)*uy(I1,I2,I3,vc)
             	       + (lam+mu)*uxy(I1,I2,I3,uc) + lamy*ux(I1,I2,I3,uc)+ mux*uy(I1,I2,I3,uc)
             	       + mu*uxx(I1,I2,I3,vc) + mux*ux(I1,I2,I3,vc) )/rho;
      	}
      	else
      	{ // ---  3D ---
        	  realSerialArray  muz(I1,I2,I3),  lamz(I1,I2,I3);
        	  e.gd(muz ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,muc    ,t);
        	  e.gd(lamz,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,lambdac,t);

        	  realSerialArray uz(I1,I2,I3,D);
        	  realSerialArray uxz(I1,I2,I3,D), uyz(I1,I2,I3,D), uzz(I1,I2,I3,D);
        	  e.gd(uz ,xLocal,numberOfDimensions,isRectangular,0,0,0,1,I1,I2,I3,D,t);
        	  e.gd(uxz,xLocal,numberOfDimensions,isRectangular,0,1,0,1,I1,I2,I3,D,t);
        	  e.gd(uyz,xLocal,numberOfDimensions,isRectangular,0,0,1,1,I1,I2,I3,D,t);
        	  e.gd(uzz,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,D,t);


        	  f(I1,I2,I3,uc) = utt(I1,I2,I3,uc) 
          	    -( (lam+2.*mu)*uxx(I1,I2,I3,uc) + (lamx+2.*mux)*ux(I1,I2,I3,uc)
             	       + (lam+mu)*uxy(I1,I2,I3,vc) + lamx*uy(I1,I2,I3,vc)+ muy*ux(I1,I2,I3,vc)
             	       + (lam+mu)*uxz(I1,I2,I3,wc) + lamx*uz(I1,I2,I3,wc)+ muz*ux(I1,I2,I3,wc)
             	       + mu*uyy(I1,I2,I3,uc) + muy*uy(I1,I2,I3,uc) 
             	       + mu*uzz(I1,I2,I3,uc) + muz*uz(I1,I2,I3,uc) 
                                    )/rho;

        	  f(I1,I2,I3,vc) = utt(I1,I2,I3,vc) 
          	    -( (lam+2.*mu)*uyy(I1,I2,I3,vc) + (lamy+2.*muy)*uy(I1,I2,I3,vc)
             	       + (lam+mu)*uxy(I1,I2,I3,uc) + lamy*ux(I1,I2,I3,uc)+ mux*uy(I1,I2,I3,uc)
             	       + (lam+mu)*uyz(I1,I2,I3,wc) + lamy*uz(I1,I2,I3,wc)+ muz*uy(I1,I2,I3,wc)
             	       + mu*uxx(I1,I2,I3,vc) + mux*ux(I1,I2,I3,vc) 
             	       + mu*uzz(I1,I2,I3,vc) + muz*uz(I1,I2,I3,vc) 
                                    )/rho;

        	  f(I1,I2,I3,wc) = utt(I1,I2,I3,wc) 
          	    -( (lam+2.*mu)*uzz(I1,I2,I3,wc) + (lamz+2.*muz)*uz(I1,I2,I3,wc)
             	       + (lam+mu)*uxz(I1,I2,I3,uc) + lamz*ux(I1,I2,I3,uc)+ mux*uz(I1,I2,I3,uc)
             	       + (lam+mu)*uyz(I1,I2,I3,vc) + lamz*uy(I1,I2,I3,vc)+ muy*uz(I1,I2,I3,vc)
             	       + mu*uxx(I1,I2,I3,wc) + mux*ux(I1,I2,I3,wc) 
             	       + mu*uyy(I1,I2,I3,wc) + muy*uy(I1,I2,I3,wc) 
                                    )/rho;

      	}

	// ::display(xLocal,sPrintF(" TZ forcing xLocal t=%8.2e",t),"%9.2e ");
// 	::display(utt(I1,I2,I3,uc),sPrintF(" TZ forcing utt t=%8.2e",t),"%9.2e ");
      	
//         realSerialArray utta(I1,I2,I3);
// 	const int isRectangular=false;
//         e.gd(utta,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,uc,t);
// 	::display(utta,sPrintF(" TZ forcing ut: t=%8.2e",t),"%9.2e ");
//         e.gd(utta,xLocal,numberOfDimensions,isRectangular,2,0,0,0,I1,I2,I3,uc,t);
// 	::display(utta,sPrintF(" TZ forcing utt: t=%8.2e",t),"%9.2e ");

      	
//         x0=0.; y0=0.; z0=0.;
      	
//         printF(" utt = %9.2e\n",e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,uc,t));
//         printF(" vtt = %9.2e\n",e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,vc,t));

      	if( option==0 || option==2 )
      	{
                    uLocal(I1,I2,I3,D) += f(I1,I2,I3,D);
      	}
      	else
      	{
                    uLocal(I1,I2,I3,D) = f(I1,I2,I3,D);
      	}

            }
            else if( option==0 || option==2 )
            {

	// ********************** this is slow -- fix *****************


      	if( mg.numberOfDimensions()==2 )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
                        x0=X(i1,i2,i3,0);
                        y0=X(i1,i2,i3,1);
          	    
          	    U(i1,i2,i3,uc)+=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,uc,t)
                       			     - c2*(e.xx(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,uc,t))
                       			     - c1*(e.xx(x0,y0,z0,uc,t)+e.xy(x0,y0,z0,vc,t))
                                                        );
          	    U(i1,i2,i3,vc)+=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,vc,t)
                       			     - c2*(e.xx(x0,y0,z0,vc,t)+e.yy(x0,y0,z0,vc,t))
                       			     - c1*(e.xy(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,vc,t))
                                                        );
        	  }
      	}
      	else
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
                        x0=X(i1,i2,i3,0); y0=X(i1,i2,i3,1); z0=X(i1,i2,i3,2);
          	    U(i1,i2,i3,uc)+=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,uc,t)
                       			     - c2*(e.xx(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,uc,t)+e.zz(x0,y0,z0,uc,t))
                       			     - c1*(e.xx(x0,y0,z0,uc,t)+e.xy(x0,y0,z0,vc,t)+e.xz(x0,y0,z0,wc,t))
                                                        );
          	    U(i1,i2,i3,vc)+=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,vc,t)
                       			     - c2*(e.xx(x0,y0,z0,vc,t)+e.yy(x0,y0,z0,vc,t)+e.zz(x0,y0,z0,vc,t))
                       			     - c1*(e.xy(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,vc,t)+e.yz(x0,y0,z0,wc,t))
                                                        );
          	    U(i1,i2,i3,wc)+=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,wc,t)
                       			     - c2*(e.xx(x0,y0,z0,wc,t)+e.yy(x0,y0,z0,wc,t)+e.zz(x0,y0,z0,wc,t))
                       			     - c1*(e.xz(x0,y0,z0,uc,t)+e.yz(x0,y0,z0,vc,t)+e.zz(x0,y0,z0,wc,t))
                                                        );
        	  }
      	}
            }
            else
            {  // option == 1 : 
      	if( mg.numberOfDimensions()==2 )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
                        x0=X(i1,i2,i3,0);  y0=X(i1,i2,i3,1);
          	    U(i1,i2,i3,uc)=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,uc,t)
                       			     - c2*(e.xx(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,uc,t))
                       			     - c1*(e.xx(x0,y0,z0,uc,t)+e.xy(x0,y0,z0,vc,t))
                                                        );
          	    U(i1,i2,i3,vc)=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,vc,t)
                       			     - c2*(e.xx(x0,y0,z0,vc,t)+e.yy(x0,y0,z0,vc,t))
                       			     - c1*(e.xy(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,vc,t))
                                                        );
        	  }
      	}
      	else
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
                        x0=X(i1,i2,i3,0); y0=X(i1,i2,i3,1); z0=X(i1,i2,i3,2);
          	    U(i1,i2,i3,uc)=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,uc,t)
                       			     - c2*(e.xx(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,uc,t)+e.zz(x0,y0,z0,uc,t))
                       			     - c1*(e.xx(x0,y0,z0,uc,t)+e.xy(x0,y0,z0,vc,t)+e.xz(x0,y0,z0,wc,t))
                                                        );
          	    U(i1,i2,i3,vc)=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,vc,t)
                       			     - c2*(e.xx(x0,y0,z0,vc,t)+e.yy(x0,y0,z0,vc,t)+e.zz(x0,y0,z0,vc,t))
                       			     - c1*(e.xy(x0,y0,z0,uc,t)+e.yy(x0,y0,z0,vc,t)+e.yz(x0,y0,z0,wc,t))
                                                        );
          	    U(i1,i2,i3,wc)=(e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,wc,t)
                       			     - c2*(e.xx(x0,y0,z0,wc,t)+e.yy(x0,y0,z0,wc,t)+e.zz(x0,y0,z0,wc,t))
                       			     - c1*(e.xz(x0,y0,z0,uc,t)+e.yz(x0,y0,z0,vc,t)+e.zz(x0,y0,z0,wc,t))
                                                        );
        	  }
      	}

            }
            if( orderOfAccuracyInSpace>=4 )
            {// add forcing for methods that are 4th order or higher
// 	if( mg.numberOfDimensions()==2 )
// 	{
// 	  forcingLoops2D((i1,i2,i3,CC)+= (e.gd(4,0,0,0,x0,y0,z0,CC,t) // 					  - (csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)// 						       +e.gd(0,0,4,0,x0,y0,z0,CC,t)// 						       +2.*e.gd(0,2,2,0,x0,y0,z0,CC,t) ) )*dtsqby12;);
// 	}
// 	else
// 	{
// 	  forcingLoops3D((i1,i2,i3,CC)+=    (e.gd(4,0,0,0,x0,y0,z0,CC,t) // 					     - (csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)// 							  +e.gd(0,0,4,0,x0,y0,z0,CC,t)// 							  +e.gd(0,0,0,4,x0,y0,z0,CC,t)// 							  +2.*(e.gd(0,2,2,0,x0,y0,z0,CC,t)// 							       +e.gd(0,2,0,2,x0,y0,z0,CC,t)// 							       +e.gd(0,0,2,2,x0,y0,z0,CC,t)) ) )*dtsqby12;);
// 	}
            }
      	
            if( orderOfAccuracyInSpace>=6 )
            { // add forcing for methods that are 6th order or higher
      	real dt4by360=dt*dt*dt*dt/360.;

            }
            
        }
        else if( forcingOption==gaussianSource )
        {
      //  f3(x,t) = sin(2*pi*omega*t)*exp( -beta*( |x-x0|^2 ) )
            const real beta =gaussianSourceParameters[0];
            const real omega=gaussianSourceParameters[1];
            const real x0   =gaussianSourceParameters[2];
            const real y0   =gaussianSourceParameters[3];
            const real z0   =gaussianSourceParameters[4];

            const realArray & x = mg.center();

            const real t0=0.; // .25;
            const real ct=cos(2.*Pi*omega*(t-t0));
            const real st=sin(2.*Pi*omega*(t-t0));

            const bool isRectangular = mg.isRectangular();

            if( debug & 4 )
      	printf(" addForcing:gaussianSource: beta=%8.2e omega=%8.2e x0=(%8.2e,%8.2e,%8.2e) \n",
             	       beta,omega,x0,y0,z0);

//       if( !isRectangular )
//       {
            
// 	realArray f3E(Ie1,Ie2,Ie3),f3H(Ih1,Ih2,Ih3);

// 	if( mg.numberOfDimensions()==2 )
// 	{
// 	  f3H=exp( -beta*( SQR(xh(Ih1,Ih2,Ih3)-x0)+SQR(yh(Ih1,Ih2,Ih3)-y0) ) );
// 	  f3E=exp( -beta*( SQR(xe(Ie1,Ie2,Ie3)-x0)+SQR(ye(Ie1,Ie2,Ie3)-y0) ) );

// 	  if( option==0 )
// 	  {
// 	    if  ( false && method==dsiMatVec  )// kkc projection is now done by EXTRACT_GFP_END
// 	    {
// 	      const realArray &cFNorm = mg.faceNormal();
// 	      const realArray &cFArea = mg.faceArea();
// 	      u(Ie1,Ie2,Ie3,0) += 
// 		cFArea(Ie1,Ie2,Ie3)*((-2.*beta*stE)*(ye(Ie1,Ie2,Ie3)-y0)*f3E*cFNorm(Ie1,Ie2,Ie3,0)+ 
// 				     ( 2.*beta*stE)*(xe(Ie1,Ie2,Ie3)-x0)*f3E*cFNorm(Ie1,Ie2,Ie3,1));
// 	    }
// 	    else
// 	    {
// 	      u(Ih1,Ih2,Ih3,hz)+=( 2.*Pi*omega*ctH)*f3H;
// 	      u(Ie1,Ie2,Ie3,ex)+=(-2.*beta*stE)*(ye(Ie1,Ie2,Ie3)-y0)*f3E;
// 	      u(Ie1,Ie2,Ie3,ey)+=( 2.*beta*stE)*(xe(Ie1,Ie2,Ie3)-x0)*f3E;
// 	    }
// 	  }
// 	  else
// 	  {
// 	    u(Ih1,Ih2,Ih3,hz)=( 2.*Pi*omega*ctH)*f3H;
// 	    u(Ie1,Ie2,Ie3,ex)=(-2.*beta*stE)*(ye(Ie1,Ie2,Ie3)-y0)*f3E;
// 	    u(Ie1,Ie2,Ie3,ey)=( 2.*beta*stE)*(xe(Ie1,Ie2,Ie3)-x0)*f3E;
// 	  }

// 	}
// 	else // *** 3D ***
// 	{
// 	  // scale by beta*beta to make O(1)
// 	  f3E=(beta*beta*ctE)*exp( -beta*( SQR(xe(I1,I2,I3)-x0)+SQR(ye(I1,I2,I3)-y0)+SQR(ze(I1,I2,I3)-z0) ) );
// 	  if( option==0 )
// 	  {
// 	    u(I1,I2,I3,ex)+=( (xe(I1,I2,I3,2)-z0)-(xe(I1,I2,I3,1)-y0) )*f3E;
// 	    u(I1,I2,I3,ey)+=( (xe(I1,I2,I3,0)-x0)-(xe(I1,I2,I3,2)-z0) )*f3E;
// 	    u(I1,I2,I3,ez)+=( (xe(I1,I2,I3,1)-y0)-(xe(I1,I2,I3,0)-x0) )*f3E;
// 	  }
// 	  else
// 	  {
// 	    u(I1,I2,I3,ex)=( (xe(I1,I2,I3,2)-z0)-(xe(I1,I2,I3,1)-y0) )*f3E;
// 	    u(I1,I2,I3,ey)=( (xe(I1,I2,I3,0)-x0)-(xe(I1,I2,I3,2)-z0) )*f3E;
// 	    u(I1,I2,I3,ez)=( (xe(I1,I2,I3,1)-y0)-(xe(I1,I2,I3,0)-x0) )*f3E;
// 	  }
// 	}
        	  
//       }
//       else
//       {
// 	real dx[3],xab[2][3];
// 	mg.getRectangularGridParameters( dx, xab );

// 	const int i0a=mg.gridIndexRange(0,0);
// 	const int i1a=mg.gridIndexRange(0,1);
// 	const int i2a=mg.gridIndexRange(0,2);

// 	const real xa=xab[0][0], dx0=dx[0];
// 	const real ya=xab[0][1], dy0=dx[1];
// 	const real za=xab[0][2], dz0=dx[2];
      	
// #define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
// #define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
// #define X2(i0,i1,i2) (za+dz0*(i2-i2a))

// 	int i1,i2,i3;
// 	real xd,yd,zd,f3;
// 	Index J1,J2,J3;
// 	if( mg.numberOfDimensions()==2 )
// 	{
// 	  if( option==0 )
// 	  {
// 	    //XXX not fully implemented for staggered schemes yet!
// 	    J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
// 	    J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
// 	    J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
// 	    FOR_3D(i1,i2,i3,J1,J2,J3)
// 	    {
// 	      xd=X0(i1,i2,i3)-x0;
// 	      yd=X1(i1,i2,i3)-y0;
          	    
// 	      f3=exp( -beta*( xd*xd+yd*yd ) );
// 	      U(i1,i2,i3,hz)+=( 2.*Pi*omega*ctH)*f3;
// 	      U(i1,i2,i3,ex)+=(-2.*beta*stE)*yd*f3;
// 	      U(i1,i2,i3,ey)+=( 2.*beta*stE)*xd*f3;
// 	    }
// 	  }
// 	  else
// 	  {
// 	    J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
// 	    J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
// 	    J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
// 	    FOR_3D(i1,i2,i3,J1,J2,J3)
// 	    {
// 	      xd=X0(i1,i2,i3)-x0;
// 	      yd=X1(i1,i2,i3)-y0;
          	    
// 	      f3=exp( -beta*( xd*xd+yd*yd ) );
// 	      U(i1,i2,i3,hz)=( 2.*Pi*omega*ctH)*f3;
// 	      U(i1,i2,i3,ex)=(-2.*beta*stE)*yd*f3;
// 	      U(i1,i2,i3,ey)=( 2.*beta*stE)*xd*f3;
// 	    }
        	  
// 	  }
// 	}
// 	else // 3D
// 	{
// 	  // scale by beta*beta to make O(1)
// 	  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
// 	  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
// 	  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
// 	  real betaSqct=beta*beta*ctE;
// 	  if( option==0 )
// 	  {
// 	    FOR_3D(i1,i2,i3,J1,J2,J3)
// 	    {
// 	      xd=X0(i1,i2,i3)-x0;
// 	      yd=X1(i1,i2,i3)-y0;
// 	      zd=X2(i1,i2,i3)-z0;
          	    
// 	      f3=betaSqct*exp( -beta*( xd*xd+yd*yd+zd*zd ) );
// 	      U(i1,i2,i3,ex)+=( zd-yd )*f3;
// 	      U(i1,i2,i3,ey)+=( xd-zd )*f3;
// 	      U(i1,i2,i3,ez)+=( yd-xd )*f3;
// 	    }
// 	  }
// 	  else
// 	  {
// 	    FOR_3D(i1,i2,i3,J1,J2,J3)
// 	    {
// 	      xd=X0(i1,i2,i3)-x0;
// 	      yd=X1(i1,i2,i3)-y0;
// 	      zd=X2(i1,i2,i3)-z0;
          	    
// 	      f3=betaSqct*exp( -beta*( xd*xd+yd*yd+zd*zd ) );
// 	      U(i1,i2,i3,ex)=( zd-yd )*f3;
// 	      U(i1,i2,i3,ey)=( xd-zd )*f3;
// 	      U(i1,i2,i3,ez)=( yd-xd )*f3;
// 	    }
// 	  }
// 	}

//       }
            
        }
        else if( forcingOption==gaussianChargeSource )
        {

            const realArray & f = u;  // do this for now

            const intArray & mask = mg.mask();
            
            realArray & x = isRectangular ? u : mg.center();
            realArray & rx = isRectangular ? u : mg.inverseVertexDerivative();

        #ifdef USE_PPP
            realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
            realSerialArray fLocal;  getLocalArrayWithGhostBoundaries(f,fLocal);
            realSerialArray rxLocal;  getLocalArrayWithGhostBoundaries(rx,rxLocal);
            intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);

          #else
            const realSerialArray & uLocal  =  u;
            const realSerialArray & fLocal  =  f;
            const realSerialArray & rxLocal = rx; 
            const intSerialArray & maskLocal  =  mask; 
          #endif

            real *uptr = uLocal.getDataPointer();
            real *fptr = fLocal.getDataPointer();
            real *xptr = xLocal.getDataPointer();
            real *rxptr = rxLocal.getDataPointer();
            int *maskptr = maskLocal.getDataPointer();

            if( option==1 )
            {
                realSerialArray & fnc = (realSerialArray&)fLocal; // cast away const
      	fnc=0.;  // the forcing function always adds a contribution to f
            }
            
            int ngcs=0;
            real amplitude=gaussianChargeSourceParameters[ngcs][0];
            real beta     =gaussianChargeSourceParameters[ngcs][1];
            real p        =gaussianChargeSourceParameters[ngcs][2];
            real xp0      =gaussianChargeSourceParameters[ngcs][3];
            real xp1      =gaussianChargeSourceParameters[ngcs][4];
            real xp2      =gaussianChargeSourceParameters[ngcs][5];
            real vp0      =gaussianChargeSourceParameters[ngcs][6];
            real vp1      =gaussianChargeSourceParameters[ngcs][7];
            real vp2      =gaussianChargeSourceParameters[ngcs][8];


//       J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
//       J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
//       J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

//       int gridType = isRectangular? 0 : 1;
//       int useForcing=1;
//       int useWhereMask=1;
//       int orderOfExtrapolation=orderOfAccuracyInSpace+1;
            
//       real ep=0;   // pointer to TZ function -- not used yet
            
//       int ipar[30];
//       real rpar[30];
            
//       ipar[0] =J1.getBase(); 
//       ipar[1] =J1.getBound(); 
//       ipar[2] =J2.getBase(); 
//       ipar[3] =J2.getBound();
//       ipar[4] =J3.getBase();
//       ipar[5] =J3.getBound();
            
//       ipar[6] =gridType;
//       ipar[7] =orderOfAccuracyInSpace;
//       ipar[8] =orderOfAccuracyInTime;
//       ipar[9] =orderOfExtrapolation;
//       ipar[10]=useForcing;
//       ipar[11]=ex;
//       ipar[12]=ey;
//       ipar[13]=ez;
//       ipar[14]=hx;
//       ipar[15]=hy;
//       ipar[16]=hz;
//       ipar[17]=useWhereMask;
//       ipar[18]=grid;
//       ipar[19]=debug;
//       ipar[20]=forcingOption;

//       rpar[0] =dx[0];
//       rpar[1] =dx[1];
//       rpar[2] =dx[2];
//       rpar[3] =mg.gridSpacing(0);
//       rpar[4] =mg.gridSpacing(1);
//       rpar[5] =mg.gridSpacing(2);
//       rpar[6] =t;
//       rpar[7] =ep;
//       rpar[8] =dt;
//       rpar[9] =c;
//       rpar[10]=eps;
//       rpar[11]=mu;
//       rpar[12]=kx;
//       rpar[13]=ky;
//       rpar[14]=kz;
//       rpar[15]=slowStartInterval;
//       rpar[16]=xab[0][0];
//       rpar[17]=xab[0][1];
//       rpar[18]=xab[0][2];
//       rpar[19]=amplitude;
//       rpar[20]=beta;
//       rpar[21]=p;   
//       rpar[22]=xp0;
//       rpar[23]=xp1;
//       rpar[24]=xp2;
//       rpar[25]=vp0;
//       rpar[26]=vp1;
//       rpar[27]=vp2;


//       int ierr=0;
  
//       forcingOptSolidMechanics( mg.numberOfDimensions(),
//                          uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
// 			 uLocal.getBase(2),uLocal.getBound(2),
//                          fLocal.getBase(0),fLocal.getBound(0),fLocal.getBase(1),fLocal.getBound(1),
// 			 fLocal.getBase(2),fLocal.getBound(2),
// 			 *uptr,*fptr,*maskptr,*rxptr, *xptr,
//                          ipar[0], rpar[0], ierr );
            

//       if( debug & 8 ) display(f,"f after forcingOptSolidMechanics",debugFile,"%9.2e ");
            
        }
            
#undef X0
#undef X1
#undef X2
        
    }

    return 0;
}



