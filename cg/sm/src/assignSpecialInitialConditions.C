// This file automatically generated from assignSpecialInitialConditions.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "ParallelUtility.h"

// ==================================================
// ============= include forcing macros =============
// ==================================================
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

// ==================================================================================
// Macro to evaluate the translation and rotation solution
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================

// ==================================================================================
// Macro to evaluate eigen modes of the sphere
// 
// evalSolution : true=eval solution, false=eval error
// ==================================================================================

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ================================================================================
//   Assign the special solution based on macros U0, U0T, U0X, and U0Y
// ================================================================================

// ===========================================================================================
/// \brief Assign "special" initial conditions.
/// \details This function assigns the solution at t=0. It may also initialize the solution
///    at previous times.
/// \param gfIndex (input) : assign gf[gfIndex].u 
/// \param evalOption (input) : computeInitialConditions or computeErrors
// ===========================================================================================
int Cgsm::
assignSpecialInitialConditions(int gfIndex, const EvaluationOptionsEnum evalOption)
{

    const real t = gf[gfIndex].t;

    assert(  evalOption==computeInitialConditions  || evalOption==computeErrors );

    if( evalOption==computeErrors )
    {
    // printF(" **** assignSpecialInitialConditions: compute the errors\n");
    }
    

    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    const int numberOfDimensions=cg.numberOfDimensions();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

    const int & u1c = parameters.dbase.get<int >("u1c");
    const int & u2c = parameters.dbase.get<int >("u2c");
    const int & u3c = parameters.dbase.get<int >("u3c");

    const int v1c = parameters.dbase.get<int >("v1c");
    const int v2c = parameters.dbase.get<int >("v2c");
    const int v3c = parameters.dbase.get<int >("v3c");

    bool assignVelocities= v1c>=0 ;
    const int s11c = parameters.dbase.get<int >("s11c");
    const int s12c = parameters.dbase.get<int >("s12c");
    const int s13c = parameters.dbase.get<int >("s13c");
    const int s21c = parameters.dbase.get<int >("s21c");
    const int s22c = parameters.dbase.get<int >("s22c");
    const int s23c = parameters.dbase.get<int >("s23c");
    const int s31c = parameters.dbase.get<int >("s31c");
    const int s32c = parameters.dbase.get<int >("s32c");
    const int s33c = parameters.dbase.get<int >("s33c");
        
    bool assignStress = s11c >=0 ;

    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                                                      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");

    const int numberOfComponentGrids = cg.numberOfComponentGrids();

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    int & debug = parameters.dbase.get<int >("debug");

    Range C=numberOfComponents;
    Index I1,I2,I3;
    int i1,i2,i3;
    
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
        MappedGrid & mg = cg[grid];
        const bool isRectangular = mg.isRectangular();

        if( pdeVariation == SmParameters::hemp )
        {
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEcenterJacobian ); // do this for now
        }
        else
        {
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // do this for now
        }
        const realArray & x = mg.center();
        realMappedGridFunction *pstate0 = NULL;
        if( pdeVariation == SmParameters::hemp )
        {
            realCompositeGridFunction *& initialState = 
      	parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction");
            if( initialState == NULL )
            {
                Range all;
      	initialState = new realCompositeGridFunction(gf[0].cg,all,all,all,3);
      	(*initialState)=1.;
            }
            else if( initialState->numberOfComponentGrids()!=numberOfComponentGrids )
            {
        // This is probably an AMR run 
                Range all;
      	initialState->updateToMatchGrid(gf[0].cg,all,all,all,3);
                (*initialState)=1.;
            }
            
            assert( parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")!=NULL );
        
            pstate0 = &(*(parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")))[grid];
        }
        realMappedGridFunction & state0 = *pstate0;

        realMappedGridFunction & u =gf[gfIndex].u[grid];

#ifdef USE_PPP
        realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
        realSerialArray det;  
        if( pdeVariation == SmParameters::hemp ) 
            getLocalArrayWithGhostBoundaries(mg.centerJacobian(),det);

        realSerialArray state0Local;
        if( pdeVariation == SmParameters::hemp )
            getLocalArrayWithGhostBoundaries(state0,state0Local);
#else
        realSerialArray & uLocal  =  u;
        const realSerialArray & xLocal  =  x;
        const realSerialArray & det = mg.centerJacobian();
        realSerialArray & state0Local = *pstate0;
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

        realMappedGridFunction & err = evalOption==computeErrors ? (*cgerrp)[grid] : u;

#ifdef USE_PPP
        realSerialArray errLocal;  getLocalArrayWithGhostBoundaries(err,errLocal);
#else
        const realSerialArray & errLocal=  err;
#endif

        real *errp = errLocal.Array_Descriptor.Array_View_Pointer3;
        const int errDim0=errLocal.getRawDataSize(0);
        const int errDim1=errLocal.getRawDataSize(1);
        const int errDim2=errLocal.getRawDataSize(2);
#undef ERR
#define ERR(i0,i1,i2,i3) errp[i0+errDim0*(i1+errDim1*(i2+errDim2*(i3)))]

        getIndex(mg.dimension(),I1,I2,I3);

        int includeGhost=1;
        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
        if( !ok ) continue;

        if( evalOption==computeInitialConditions )
            uLocal=0.;
            
        lambda = lambdaGrid(grid);
        mu = muGrid(grid);


        aString & specialInitialConditionOption = parameters.dbase.get<aString>("specialInitialConditionOption");
        if( specialInitialConditionOption == "default" ||
                specialInitialConditionOption == "eigenmode1d" )
        {
      // One-dimensional eigen mode

            real k =2.*Pi;
            real omega = sqrt( (lambda+2.*mu)/rho * k*k );

            real a[3]={1.,0.,0.}; 
#define U0(x,y,z,n,t) (a[n-uc]*sin(k*(x))*cos(omega*(t)))
#define U0T(x,y,z,n,t) (-a[n-uc]*omega*sin(k*(x))*sin(omega*(t)))
#define U0X(x,y,z,n,t) (a[n-uc]*k*cos(k*(x))*cos(omega*(t)))
#define U0Y(x,y,z,n,t) (0.)
      
              if( mg.numberOfDimensions()==2 )
              {
                  real z0=0.;
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      real x0 = X(i1,i2,i3,0);
                      real y0 = X(i1,i2,i3,1);
                      real u1 = U0(x0,y0,z0,uc,t);
                      real u2 = U0(x0,y0,z0,vc,t);
                      if( evalOption==computeInitialConditions )
                      {
                          U(i1,i2,i3,uc) =u1;
                          U(i1,i2,i3,vc) =u2;
                      }
                      else
                      {
                          ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                          ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                      }
                  }
                  printF("*** assignSpecial: assignVelocities=%i\n",assignVelocities);
                  if( assignVelocities )
                  {
                      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                      {
                          real x0 = X(i1,i2,i3,0);
                          real y0 = X(i1,i2,i3,1);
                          real v1 = U0T(x0,y0,z0,uc,t);
                          real v2 = U0T(x0,y0,z0,vc,t);
             // printF(" *** assignSpecial: v1=%e v2=%e\n",v1,v2);
                          if( evalOption==computeInitialConditions )
                          {
                   	 U(i1,i2,i3,v1c) = v1;
                   	 U(i1,i2,i3,v2c) = v2;
                          }
                          else
                          {
                   	 ERR(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1;
                   	 ERR(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2;
                          }
                      }
                  }
                  if( assignStress )
                  {
                      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                      {
                          real x0 = X(i1,i2,i3,0);
                          real y0 = X(i1,i2,i3,1);
                          real div = U0X(x0,y0,z0,uc,t) + U0Y(x0,y0,z0,vc,t);
                          real s11 = lambda*div + 2.*mu*U0X(x0,y0,z0,uc,t);
                          real s12 = mu*(U0Y(x0,y0,z0,uc,t)+U0X(x0,y0,z0,vc,t));
                          real s21 = s12;
                          real s22 = lambda*div + 2.*mu*U0Y(x0,y0,z0,vc,t);
                          if( evalOption==computeInitialConditions )
                          {
                   	 U(i1,i2,i3,s11c) =s11;
                   	 U(i1,i2,i3,s12c) =s12;
                   	 U(i1,i2,i3,s21c) =s21;
                   	 U(i1,i2,i3,s22c) =s22;
                          }
                          else
                          {
                   	 ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                   	 ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                   	 ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s21;
                   	 ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                          }
                      }
                  }
              }
              else
              { // ***** 3D  ****
                  Overture::abort("finish me for 3d");
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      real x0 = X(i1,i2,i3,0);
                      real y0 = X(i1,i2,i3,1);
                      real z0 = X(i1,i2,i3,2);
                      U(i1,i2,i3,uc) =U0(x0,y0,z0,uc,t);
                      U(i1,i2,i3,vc) =U0(x0,y0,z0,vc,t);
                      U(i1,i2,i3,wc) =U0(x0,y0,z0,wc,t);
                  }
              }
            

#undef U0
#undef U0T
#undef U0X
#undef U0Y
        }
        else if( specialInitialConditionOption == "invariant" )
        {
      // Here is an invariant of the motion  (starts out as an infinitesimal rotation)

            real a[3]={ 0.,-1.,0.};
            real b[3]={ 1., 0.,0.};
#define U0(x,y,z,n,t)  ((a[n-uc]*(x) + b[n-uc]*(y))*(t))
#define U0T(x,y,z,n,t) ((a[n-uc]*(x) + b[n-uc]*(y))    )
#define U0X(x,y,z,n,t) ((a[n-uc]                  )*(t))
#define U0Y(x,y,z,n,t) ((              b[n-uc]    )*(t))
      
              if( mg.numberOfDimensions()==2 )
              {
                  real z0=0.;
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      real x0 = X(i1,i2,i3,0);
                      real y0 = X(i1,i2,i3,1);
                      real u1 = U0(x0,y0,z0,uc,t);
                      real u2 = U0(x0,y0,z0,vc,t);
                      if( evalOption==computeInitialConditions )
                      {
                          U(i1,i2,i3,uc) =u1;
                          U(i1,i2,i3,vc) =u2;
                      }
                      else
                      {
                          ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                          ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                      }
                  }
                  printF("*** assignSpecial: assignVelocities=%i\n",assignVelocities);
                  if( assignVelocities )
                  {
                      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                      {
                          real x0 = X(i1,i2,i3,0);
                          real y0 = X(i1,i2,i3,1);
                          real v1 = U0T(x0,y0,z0,uc,t);
                          real v2 = U0T(x0,y0,z0,vc,t);
             // printF(" *** assignSpecial: v1=%e v2=%e\n",v1,v2);
                          if( evalOption==computeInitialConditions )
                          {
                   	 U(i1,i2,i3,v1c) = v1;
                   	 U(i1,i2,i3,v2c) = v2;
                          }
                          else
                          {
                   	 ERR(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1;
                   	 ERR(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2;
                          }
                      }
                  }
                  if( assignStress )
                  {
                      FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                      {
                          real x0 = X(i1,i2,i3,0);
                          real y0 = X(i1,i2,i3,1);
                          real div = U0X(x0,y0,z0,uc,t) + U0Y(x0,y0,z0,vc,t);
                          real s11 = lambda*div + 2.*mu*U0X(x0,y0,z0,uc,t);
                          real s12 = mu*(U0Y(x0,y0,z0,uc,t)+U0X(x0,y0,z0,vc,t));
                          real s21 = s12;
                          real s22 = lambda*div + 2.*mu*U0Y(x0,y0,z0,vc,t);
                          if( evalOption==computeInitialConditions )
                          {
                   	 U(i1,i2,i3,s11c) =s11;
                   	 U(i1,i2,i3,s12c) =s12;
                   	 U(i1,i2,i3,s21c) =s21;
                   	 U(i1,i2,i3,s22c) =s22;
                          }
                          else
                          {
                   	 ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                   	 ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                   	 ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s21;
                   	 ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                          }
                      }
                  }
              }
              else
              { // ***** 3D  ****
                  Overture::abort("finish me for 3d");
                  FOR_3D(i1,i2,i3,I1,I2,I3)
                  {
                      real x0 = X(i1,i2,i3,0);
                      real y0 = X(i1,i2,i3,1);
                      real z0 = X(i1,i2,i3,2);
                      U(i1,i2,i3,uc) =U0(x0,y0,z0,uc,t);
                      U(i1,i2,i3,vc) =U0(x0,y0,z0,vc,t);
                      U(i1,i2,i3,wc) =U0(x0,y0,z0,wc,t);
                  }
              }

#undef U0
#undef U0T
#undef U0X
#undef U0Y

        }
        else if( specialInitialConditionOption == "travelingWave" )
        {
      // --- traveling (shock) wave solution ---

            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines)
      //	printF("INFO: The traveling wave solutions are combinations of p and s solutions:\n"
      //               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-xa)+k2*(y-ya) - cp*t )  (p-wave)\n"
      //               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-xa)+k2*(y-ya) - cs*t )  (s-wave\n",
      //               "    where  G(xi)=0 for xi>0 and G(xi)=-1 for xi<0 \n");
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                bool assignStress = s11c >=0 ;
        // hard code some numbers for now: 
        // real k1=1., k2=0., k3=0.;
        // real ap=1., xa=.5, ya=0.;
                real cp = sqrt( (lambda+2.*mu)/rho );
                real cs = sqrt( mu/rho );
                std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
                const int np = int(twd[0]);  // number of p wave solutions
                const int ns = int(twd[1]);  // number of s wave solutions
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: travelingWave: finish me for HEMP **********\n\n");
          // OV_ABORT("error");
                }
        // printF("**** travelingWave: cp=%8.2e, t=%8.2e v0=%8.2e *********\n",cp,t,v0);
                int i1,i2,i3;
                if( mg.numberOfDimensions()==2 )
                {
                    real z0=0.;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0);
                        real y0 = X(i1,i2,i3,1);
                        real u1=0., u2=0.;  // back-ground field
                        real v1=0., v2=0.;
                        real s11=0., s12=0., s22=0.;
                        real div=0.;
            // real a1=0., b1=-1., a2=0., b2=0.;
            // u1 = a1*x0 + b1*t;  // more general back ground field
            // u2 = a2*x0 + b2*t;
                        int m=2;
                        for (int n=0; n<np; n++ ) // p-wave solutions
                        {
                  	real ap = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];
                  	real xi = k1*(x0-xa) + k2*(y0-ya) - cp*t;
                  	if( xi <= 0. )
                  	{
                    	  u1 += -ap*k1*xi;
                    	  u2 += -ap*k2*xi;
                                v1 += ap*cp*k1;
                                v2 += ap*cp*k2;
                                s11+= -ap*( lambda+2.*mu*k1*k1 );
                    	  s12+= -ap*( 2.*mu*k1*k2 );
                    	  s22+= -ap*( lambda+2.*mu*k2*k2 );
                                div+= -ap;
                  	}
                        }
                        for (int n=0; n<ns; n++ ) // s-wave solutions
                        {
                  	real as = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];
                  	real xi = k1*(x0-xa) + k2*(y0-ya) - cs*t;
                  	if( xi <= 0. )
                  	{  // (-k2,k1)
                    	  u1 += +as*k2*xi;
                    	  u2 += -as*k1*xi;
                                v1 += -as*cs*k2;
                                v2 += +as*cs*k1;
                                s11+= -as*(-2.*mu*k1*k2 );
                    	  s12+= -as*( mu*(k1*k1-k2*k2) );
                    	  s22+= -as*( 2.*mu*k1*k2 );
                  	}
                        }
            // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);
                        if( evalSolution )
                        {
                  	if( pdeVariation == SmParameters::hemp )
                  	{
                    	  U(i1,i2,i3,u1c) =u1;
                    	  U(i1,i2,i3,u2c) =u2;
                    	  U(i1,i2,i3,uc) =x0;
                    	  U(i1,i2,i3,vc) =y0;
                    	  state0Local(i1,i2,i3,1) = 1.0; // density
                    	  /*********/
                    	  state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
                    	  if( pdeVariation == SmParameters::hemp && 
                        	      i1 > I1Base && i1 < I1Bound &&
                        	      i2 > I2Base && i2 < I2Bound )
                    	  {
                      	    real area = 0.25*(det(i1,i2,i3)+det(i1+1,i2,i3)+det(i1+1,i2+1,i3)+det(i1,i2+1,i3));
                      	    state0Local(i1,i2,i3,0) = area*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
                    	  }
                  	}
                  	else
                  	{
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                  	}
                            if( assignVelocities )
                  	{
                    	  U(i1,i2,i3,v1c) = v1;
                    	  U(i1,i2,i3,v2c) = v2;
                  	}
                  	if( assignStress )
                  	{
                    	  U(i1,i2,i3,s11c) =s11;
                    	  U(i1,i2,i3,s12c) =s12;
                    	  U(i1,i2,i3,s21c) =s12;
                    	  U(i1,i2,i3,s22c) =s22;
                    	  if( pdeVariation == SmParameters::hemp )
                    	  {
                                    real press = -(lambda+2.0*mu/3.0)*div;
                                    U(i1,i2,i3,pc)   = press;
                                    U(i1,i2,i3,s11c) += press;
                                    U(i1,i2,i3,s22c) += press;
                    	  }
                  	}
                        }
                        else
                        {
                  	if( pdeVariation == SmParameters::hemp )
                  	{
                    	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                    	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                    	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                    	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                    	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                    	  }
                            if( assignVelocities )
                  	{
                    	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                  	}
                  	if( assignStress )
                  	{
                    	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                    	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                    	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                    	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                    	  if( pdeVariation == SmParameters::hemp )
                    	  {
                                    real press = -(lambda+2.0*mu/3.0)*div;
                                    ERR(i1,i2,i3,s11c) -= press;
                      	    ERR(i1,i2,i3,s22c) -= press;
                      	    ERR(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
                    	  }
                  	}
                        }
                    } // end FOR_3D
                }
                else
                {
                    OV_ABORT("Error: finish me");
                }
            }

        }
        else if( specialInitialConditionOption == "planeTravelingWave" )
        {
      // --- traveling (sine) wave solution ---

            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines)
      //	printF("INFO: The traveling wave solutions are combinations of p and s solutions:\n"
      //               "  [u1,u2] = ap* [ k1,k2] * G( k1*(x-xa)+k2*(y-ya) - cp*t )  (p-wave)\n"
      //               "  [u1,u2] = as* [-k2,k1] * G( k1*(x-xa)+k2*(y-ya) - cs*t )  (s-wave\n",
      //               "    where  G(xi)=sin(freq,xi) \n");
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                bool assignStress = s11c >=0 ;
        // hard code some numbers for now: 
        // real k1=1., k2=0., k3=0.;
        // real ap=1., xa=.5, ya=0.;
                real cp = sqrt( (lambda+2.*mu)/rho );
                real cs = sqrt( mu/rho );
                std::vector<real> & twd = parameters.dbase.get<std::vector<real> >("travelingWaveData");
                const int np = int(twd[0]);  // number of p wave solutions
                const int ns = int(twd[1]);  // number of s wave solutions
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: travelingWave: finish me for HEMP **********\n\n");
          // OV_ABORT("error");
                }
        // printF("**** planeTravelingWave: cp=%8.2e, t=%8.2e  *********\n",cp,t);
                int i1,i2,i3;
                if( mg.numberOfDimensions()==2 )
                {
                    real z0=0.;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0);
                        real y0 = X(i1,i2,i3,1);
                        real u1=0., u2=0.;  // back-ground field
                        real v1=0., v2=0.;
                        real s11=0., s12=0., s22=0.;
                        real div=0.;
            // real a1=0., b1=-1., a2=0., b2=0.;
            // u1 = a1*x0 + b1*t;  // more general back ground field
            // u2 = a2*x0 + b2*t;
                        int m=2;
                        for (int n=0; n<np; n++ ) // p-wave solutions
                        {
                  	real ap = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];
                  	real freq=twd[m++];
                  	real xi = k1*(x0-xa) + k2*(y0-ya) - cp*t;
                            real sinf = sin(freq*xi), cosf=cos(freq*xi);
                  	u1 += -ap*k1*sinf;
                  	u2 += -ap*k2*sinf;
                  	v1 += ap*cp*k1*freq*cosf;
                  	v2 += ap*cp*k2*freq*cosf;
                  	s11+= -ap*( lambda+2.*mu*k1*k1 )*freq*cosf;
                  	s12+= -ap*( 2.*mu*k1*k2        )*freq*cosf;
                  	s22+= -ap*( lambda+2.*mu*k2*k2 )*freq*cosf;
                  	div+= -ap;
                        }
                        for (int n=0; n<ns; n++ ) // s-wave solutions
                        {
                  	real as = twd[m++], k1=twd[m++], k2=twd[m++], k3=twd[m++], xa=twd[m++], ya=twd[m++], za=twd[m++];
                  	real xi = k1*(x0-xa) + k2*(y0-ya) - cs*t;
                  	real freq=twd[m++];
                  	real sinf = sin(freq*xi), cosf=cos(freq*xi);
      	// (-k2,k1)
                  	u1 += +as*k2*sinf;
                  	u2 += -as*k1*sinf;
                  	v1 += -as*cs*k2*freq*cosf;
                  	v2 += +as*cs*k1*freq*cosf;
                  	s11+= -as*(-2.*mu*k1*k2      )*freq*cosf;
                  	s12+= -as*( mu*(k1*k1-k2*k2) )*freq*cosf;
                  	s22+= -as*( 2.*mu*k1*k2      )*freq*cosf;
                        }
            // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);
                        if( evalSolution )
                        {
                  	if( pdeVariation == SmParameters::hemp )
                  	{
                    	  U(i1,i2,i3,u1c) =u1;
                    	  U(i1,i2,i3,u2c) =u2;
                    	  U(i1,i2,i3,uc) =x0;
                    	  U(i1,i2,i3,vc) =y0;
                    	  state0Local(i1,i2,i3,1) = 1.0; // density
                    	  /*********/
                    	  state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
                    	  if( pdeVariation == SmParameters::hemp && 
                        	      i1 > I1Base && i1 < I1Bound &&
                        	      i2 > I2Base && i2 < I2Bound )
                    	  {
                      	    real area = 0.25*(det(i1,i2,i3)+det(i1+1,i2,i3)+det(i1+1,i2+1,i3)+det(i1,i2+1,i3));
                      	    state0Local(i1,i2,i3,0) = area*mg.gridSpacing(axis1)*mg.gridSpacing(axis2); // mass
                    	  }
                  	}
                  	else
                  	{
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                  	}
                            if( assignVelocities )
                  	{
                    	  U(i1,i2,i3,v1c) = v1;
                    	  U(i1,i2,i3,v2c) = v2;
                  	}
                  	if( assignStress )
                  	{
                    	  U(i1,i2,i3,s11c) =s11;
                    	  U(i1,i2,i3,s12c) =s12;
                    	  U(i1,i2,i3,s21c) =s12;
                    	  U(i1,i2,i3,s22c) =s22;
                    	  if( pdeVariation == SmParameters::hemp )
                    	  {
                                    real press = -(lambda+2.0*mu/3.0)*div;
                                    U(i1,i2,i3,pc)   = press;
                                    U(i1,i2,i3,s11c) += press;
                                    U(i1,i2,i3,s22c) += press;
                    	  }
                  	}
                        }
                        else
                        {
                  	if( pdeVariation == SmParameters::hemp )
                  	{
                    	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                    	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                    	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                    	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                    	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                    	  }
                            if( assignVelocities )
                  	{
                    	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                  	}
                  	if( assignStress )
                  	{
                    	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                    	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                    	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                    	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                    	  if( pdeVariation == SmParameters::hemp )
                    	  {
                                    real press = -(lambda+2.0*mu/3.0)*div;
                                    ERR(i1,i2,i3,s11c) -= press;
                      	    ERR(i1,i2,i3,s22c) -= press;
                      	    ERR(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
                    	  }
                  	}
                        }
                    } // end FOR_3D
                }
                else
                {
                    OV_ABORT("Error: finish me");
                }
            }

        }
        else if( specialInitialConditionOption == "translationAndRotation" )
        {
      // Here is the solution for large translation and rotation 

            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines) 
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                bool assignStress = s11c >=0 ;
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: getTranslationAndRotationSolution: finish me for HEMP **********\n\n");
          // OV_ABORT("error");
                }
        // Here is the solution for large translation and rotation
        // hard code some numbers for now: 
                std::vector<real> & trd = parameters.dbase.get<std::vector<real> >("translationAndRotationSolutionData");
                real omega   = trd[0];   // rotation rate
                real xcenter = trd[1];      // center of rotation in the reference frame
                real ycenter = trd[2];      // center of rotation in the reference frame
                real zcenter = trd[3];      // center of rotation in the reference frame
                real vcenter[3]={trd[4],trd[5],trd[6]};   // velocity of center
                real rx[3]={cos(omega*t)-1.,-sin(omega*t),    0.};
                real ry[3]={sin(omega*t)   , cos(omega*t)-1., 0.};
                real rxt[3]={-omega*sin(omega*t),-omega*cos(omega*t), 0.};
                real ryt[3]={ omega*cos(omega*t),-omega*sin(omega*t), 0.};
            #define U0(x,y,z,n,t)  (vcenter[n-uc]*(t) +  rx[n-uc]*((x)-xcenter) +  ry[n-uc]*((y)-ycenter))
            #define U0T(x,y,z,n,t) (vcenter[n-uc]     + rxt[n-uc]*((x)-xcenter) + ryt[n-uc]*((y)-ycenter))
            #define U0X(x,y,z,n,t) (                     rx[n-uc]                                        )
            #define U0Y(x,y,z,n,t) (                                               ry[n-uc]              )
                if( t==0. )
                    printF("**** translationAndRotationSolution, t=%8.2e omega=%8.2e (x0,x1,x2)=(%8.2e,%8.2e,%8.2e) "
                     	   " (v0,v1,v2)=(%8.2e,%8.2e,%8.2e) *********\n",t,omega,xcenter,ycenter,zcenter,
                     	   vcenter[0],vcenter[1],vcenter[2]);
                int i1,i2,i3;
                if( mg.numberOfDimensions()==2 )
                {
                    real z0=0.;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0);
                        real y0 = X(i1,i2,i3,1);
                        real u1 = U0(x0,y0,z0,uc,t);
                        real u2 = U0(x0,y0,z0,vc,t);
                        if( evalSolution )
                        {
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                        }
                        else
                        {
                  	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                  	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                        }
                    }
                    if( assignVelocities )
                    {
                        FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                        {
                  	real x0 = X(i1,i2,i3,0);
                  	real y0 = X(i1,i2,i3,1);
                  	real v1 = U0T(x0,y0,z0,uc,t);
                  	real v2 = U0T(x0,y0,z0,vc,t);
      	// printF(" *** assignSpecial: v1=%e v2=%e\n",v1,v2);
                  	if( evalSolution )
                  	{
                    	  U(i1,i2,i3,v1c) = v1;
                    	  U(i1,i2,i3,v2c) = v2;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2;
                  	}
                        }
                    }
                    if( assignStress )
                    {
                        FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                        {
                  	real x0 = X(i1,i2,i3,0);
                  	real y0 = X(i1,i2,i3,1);
                  	real f11 = 1. + U0X(x0,y0,z0,uc,t);
                  	real f12 =      U0Y(x0,y0,z0,uc,t);
                  	real f21 =      U0X(x0,y0,z0,vc,t);
                  	real f22 = 1. + U0Y(x0,y0,z0,vc,t);
                  	real e11 = .5*(f11*f11+f21*f21-1.);     // this is E(i,j), symmetric
                  	real e12 = .5*(f11*f12+f21*f22   );
                  	real e22 = .5*(f12*f12+f22*f22-1.);
                  	real trace = e11 + e22;
                  	real s11 = lambda*trace + 2*mu*e11;     // this is S(i,j), symmetric
                  	real s12 =                2*mu*e12;
                  	real s21 = s12;
                  	real s22 = lambda*trace + 2*mu*e22;
                  	real p11 = s11*f11 + s12*f12;           // this P(i,j)
                  	real p12 = s11*f21 + s12*f22;
                  	real p21 = s21*f11 + s22*f12;
                  	real p22 = s21*f21 + s22*f22;
                  	if( evalSolution )
                  	{
                    	  U(i1,i2,i3,s11c) = p11;
                    	  U(i1,i2,i3,s12c) = p12;
                    	  U(i1,i2,i3,s21c) = p21;
                    	  U(i1,i2,i3,s22c) = p22;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,s11c) = U(i1,i2,i3,s11c) - p11;
                    	  ERR(i1,i2,i3,s12c) = U(i1,i2,i3,s12c) - p12;
                    	  ERR(i1,i2,i3,s21c) = U(i1,i2,i3,s21c) - p21;
                    	  ERR(i1,i2,i3,s22c) = U(i1,i2,i3,s22c) - p22;
                  	}
                        }
                    }
                }
                else
                { // ***** 3D  ****
                    OV_ABORT("translationAndRotationSolution: finish me for 3d");
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0);
                        real y0 = X(i1,i2,i3,1);
                        real z0 = X(i1,i2,i3,2);
                        U(i1,i2,i3,uc) =U0(x0,y0,z0,uc,t);
                        U(i1,i2,i3,vc) =U0(x0,y0,z0,vc,t);
                        U(i1,i2,i3,wc) =U0(x0,y0,z0,wc,t);
                    }
                }
            #undef U0
            #undef U0T
            #undef U0X
            #undef U0Y
            }

        }
        else if( specialInitialConditionOption=="sphereEigenmode" )
        {
            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines) 
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                const real & rho=parameters.dbase.get<real>("rho");
                const real & mu = parameters.dbase.get<real>("mu");
                const real & lambda = parameters.dbase.get<real>("lambda");
                const RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
                const RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
                bool assignStress = s11c >=0 ;
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: getSphereEigenmode: finish me for HEMP **********\n\n");
          // OV_ABORT("error");
                }
                assert( mg.numberOfDimensions()==3 );
        // parameters are stored here:
                std::vector<real> & data = parameters.dbase.get<std::vector<real> >("sphereEigenmodeData");
                const int vibrationClass = (int)data[0];
                const int n = (int)data[1];
                const int m = (int)data[2];
                const real rad   = data[3];
                const real cp =sqrt( (lambda+2.*mu)/rho );
                const real cs = sqrt( mu/rho );
        // The eigenvalues z = kappa*a satisfy (n=mode)
        //   (n-1)*psi_n(z) + z*psi_n'(z) = 0 
        // cgDoc/sm/sphere.maple: 
        //  n=1 : m=1  x/Pi = 1.83456604098843e+00
        //  n=1 : m=2  x/Pi = 2.89503202144422e+00
        // n=2 : m=1  x/Pi = 7.96135239733083e-01
        // n=2 : m=2  x/Pi = 2.27146214644857e+00
        // kappa = omega /cs
        // h = omega/cp
                real omega;
                real cPhi=1., cOmega=1.;
                if( vibrationClass==1 )
                {
          // eigenvalues are kappa*a
                    real radialEigenvalue;
                    if( n==1 )
                    {
                        if( m==1 )
                  	radialEigenvalue = 1.83456604098843*Pi;
                        else if( m==2 )
                  	radialEigenvalue = 2.89503202144422*Pi;
                        else
                        {
                  	OV_ABORT("getSphereEigenmode: invalid value for m");
                        }
                    }
                    else if( n==2 )
                    {
                        assert( m==1 );
                        if( m==1 )
                  	radialEigenvalue = 7.96135239733083e-01*Pi;
                        else if( m==2 )
                  	radialEigenvalue = 2.27146214644857*Pi;
                        else
                        {
                  	OV_ABORT("getSphereEigenmode: invalid value for m");
                        }
                    }
                    omega = (radialEigenvalue/rad)*cs;
                }
                else if( vibrationClass==2 )
                {
          // See cgDoc/sm/sphere2.maple
          // % sphere1.maple: 
          // %  Class II : n=0 : root m=1  x=4.43999821235653e+00, x/Pi = 1.41329532563144e+00, h*a/Pi=8.15966436697752e-01
          // %  Class II : n=0 : root m=2  x=1.04939244112140e+01, x/Pi = 3.34031988495483e+00, h*a/Pi=1.92853458475813e+00
          // %  Class II : n=0 : root m=3  x=1.60731909374045e+01, x/Pi = 5.11625557789555e+00, h*a/Pi=2.95387153514092e+00
          // %  Class II : n=0 : root m=4  x=2.15793450854558e+01, x/Pi = 6.86891887807218e+00, h*a/Pi=3.96577216329668e+00
                    real radialEigenvalue;
                    if( n==0 )
                    { // radial vibrations
            // h*a
                        const real eig[4] ={ // 
                  	8.15966436697752e-01,
                  	1.92853458475813e+00,
                  	2.95387153514092e+00,
                  	3.96577216329668e+00
                        };
                        if( m>=1 && m<=4 )
                        {
                            radialEigenvalue  = eig[m-1]*Pi;  // Love: m=0 : .8160
                        }
                        else 
                        {
                  	printF("sphereEigenmode: ERROR: n=%i m=%i not available\n",n,m);
                  	OV_ABORT("ERROR");
                        }
                        omega = (radialEigenvalue/rad)*cp; 
                    }
                    else if( n==2 )
                    {
            // %  n=2 : root m=1  x=2.63986927790186e+00, x/Pi = 8.40296489389027e-01, kappa*a/Pi=8.40296489389027e-01
            // %  an =-1.13478082789981e-02, cn=-3.02305786589451e-02, -an/cn=-3.75375159272393e-01
            // %  bn =-2.04041203329361e-02, dn=-5.43566078599509e-02, -bn/dn=-3.75375159272393e-01
            // % 
            // %  n=2 : root m=2  x=4.86527284993742e+00, x/Pi = 1.54866444711667e+00, kappa*a/Pi=1.54866444711667e+00
            // %  an =1.50294520720980e-02, cn=1.88125428532884e-01, -an/cn=-7.98905931500425e-02
            // %  bn =-1.22064204309416e-02, dn=-1.52789207710809e-01, -bn/dn=-7.98905931500425e-02
            // % 
            // %  n=2 : root m=3  x=8.32919545905501e+00, x/Pi = 2.65126525857435e+00, kappa*a/Pi=2.65126525857435e+00
            // %  an =4.57627801659678e-03, cn=-9.86226192484144e-02, -an/cn=4.64019111586348e-02
            // %  bn =-9.34189056911829e-04, dn=2.01325556121623e-02, -bn/dn=4.64019111586348e-02
            // % 
            // %  n=2 : root m=4  x=9.78016346034290e+00, x/Pi = 3.11312271792062e+00, kappa*a/Pi=3.11312271792062e+00
            // %  an =7.28789419129781e-04, cn=4.50066777991950e-02, -an/cn=-1.61929174684121e-02
            // %  bn =1.21547211256669e-03, dn=7.50619593373295e-02, -bn/dn=-1.61929174684121e-02
            // kappa*a
                        const real eig[4] ={ // 
                  	8.40296489389027e-01,
                  	1.54866444711667e+00,
                  	2.65126525857435e+00,
                            3.11312271792062e+00
                        };
                        const real cwp[4] ={ // 
                  	-3.75375159272393e-01,
                  	-7.98905931500425e-02,
                  	4.64019111586348e-02,
                  	-1.61929174684121e-02
                        };
            // radialEigenvalue= 2.63986927790039;
            // radialEigenvalue = .840*Pi;  // Love p286
                        if( m>=1 && m<=4 )
                        {
                  	radialEigenvalue=eig[m-1]*Pi;
                  	cPhi=cwp[m-1];
                        }
                        else 
                        {
                  	printF("sphereEigenmode: ERROR: n=%i m=%i not available\n",n,m);
                  	OV_ABORT("ERROR");
                        }
                        omega = (radialEigenvalue/rad)*cs;
                    }
                    else
                    {
                        OV_ABORT("finish me for vibrationClass==2");
                    }
                }
                else
                {
                    OV_ABORT("ERROR: vibrationClass");
                }
                real kappa,h;
        // kappa = omega/cs
                kappa = omega/cs;
        // h = omega/cp
                h = omega/cp;
                const real h2= h*h, h3=h*h*h, h4=h*h*h*h, kappa2=kappa*kappa, kappa3=kappa*kappa*kappa, kappa4=kappa*kappa*kappa*kappa;
                if( t==0. )
                    printF("**** getSphereEigenmode: t=%8.2e class=%i n=%i m=%i radius=%9.3e omega=%9.3e cp=%8.2e cs=%8.2e"
                                  " h*a/pi=%9.3e kappa*a/pi=%9.3e\n",
                                    t,vibrationClass,n,m,rad,omega,cp,cs,h*rad/Pi,kappa*rad/Pi);
        // printF("**** getSphereEigenmode: t=%8.2e class=%i evalSolution=%i \n",t,vibrationClass,evalSolution);
                real cost = cos(omega*t);
                real sint = sin(omega*t);
        // vibrationClass==1 solution: 
        //     
                if( vibrationClass==1 )
                {
                    real amp=10./rad; // amplitude
                    if( n==2 )
                        amp=100./rad;
                    const real a0xy =1.;  // shear in the x-y plane
                    const real a0yz =.8;  // shear in the y-z plane
                    const real a0zx =.6;  // shear in the z-x plane
            #define U1(x,y,z) (amp*cost*psi*( a0xy*(y)         -a0zx*(z) ))
            #define V1(x,y,z) (amp*cost*psi*(-a0xy*(x)+a0yz*(z) ))
            #define W1(x,y,z) (amp*cost*psi*(         -a0yz*(y)+a0zx*(x) ))
          // psi has an asymptotic series for small argument: 
                    real psi0, psi1= 1./(2.*(2.*n+3.));
                    if( n==1 )
                    {
                        psi0 = -1./(1.*3.);
                    }
                    else if( n==2 )
                    {
                        psi0 = 1./(1.*3.*5.);
                    }
                    else
                    {
                        OV_ABORT("finish me for n>2");
                    }
                    const real rEps = pow(REAL_EPSILON,.25); // use small r approximation when kappa*r < rEps
          // const real rEps = pow(REAL_EPSILON,.5); // use small r approximation when kappa*r < rEps
                    int i1,i2,i3;
                    real x0,y0,z0,r,kr,psi,u1,u2,u3;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        x0 = X(i1,i2,i3,0);
                        y0 = X(i1,i2,i3,1);
                        z0 = X(i1,i2,i3,2);
                        r = sqrt( x0*x0 + y0*y0 + z0*z0 );
                        kr = kappa*r;
                        if( n==1 )
                        {
      	// Here is psi_1 
                  	if( kr < rEps ) // use taylor series: (Love p279)
                    	  psi = psi0*( 1. - psi1*kr*kr );  // + O( kr^4 )
                  	else
                    	  psi = (kr*cos(kr)-sin(kr))/(kr*kr*kr);
                  	u1 = U1(x0,y0,z0);
                  	u2 = V1(x0,y0,z0);
                  	u3 = W1(x0,y0,z0);
                        }
                        else if( n==2 )
                        {
            #define U2(x,y,z) (amp*cost*psi*( a0xy*(y)*(z)              -a0zx*(z)*(y) ))
            #define V2(x,y,z) (amp*cost*psi*(-a0xy*(x)*(z)+a0yz*(z)*(x)               ))
            #define W2(x,y,z) (amp*cost*psi*(             -a0yz*(y)*(x) +a0zx*(x)*(y) ))
      	// Here is psi_2 
                  	if( kr < rEps ) // use taylor series: (Love p279)
                    	  psi = psi0*( 1. - psi1*kr*kr );  // + O( kr^4 )
                  	else
                    	  psi = ( 3.*kr*cos(kr)+ (kr*kr-3.)*sin(kr) )/(kr*kr*kr*kr*kr);
                  	u1 = U2(x0,y0,z0);
                  	u2 = V2(x0,y0,z0);
                  	u3 = W2(x0,y0,z0);
                        }
                        else
                        {
                  	OV_ABORT("un-implemented value for n");
                        }
                        if( evalSolution )
                        {
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                  	U(i1,i2,i3,wc) =u3;
                        }
                        else
                        {
                  	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                  	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                  	ERR(i1,i2,i3,wc) =U(i1,i2,i3,wc) - u3;
                        }
                    }
                    if( assignVelocities )
                    {
                        OV_ABORT("getSphereEigenmode: finish me");
                        FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                        {
                  	if( evalSolution )
                  	{
                  	}
                  	else
                  	{
                  	}
                        }
                    }
                    if( assignStress )
                    {
                        OV_ABORT("getSphereEigenmode: finish me");
                        FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
                        {
                  	if( evalSolution )
                  	{
                  	}
                  	else
                  	{
                  	}
                        }
                    }
                }
                else if( vibrationClass==2 )
                {
                    const real amp=50./rad; // amplitude -- this makes |u| about 1
                    const real rEps = pow(REAL_EPSILON,.25); // use small r approximation when kappa*r < rEps
          // const real rEps = pow(REAL_EPSILON,.5); // use small r approximation when kappa*r < rEps
          // psi has an asymptotic series for small argument: 
          // psi_n(x) = psin0*( 1. - psin1*x^2 + ... )
                    const real psi10 = -1./(1.*3.);
                    const real psi20 =  1./(1.*3.*5.);
                    const real psi30 = -1./(1.*3.*5.*7.);
                    const real psi40 =  1./(1.*3.*5.*7.*9.);
                    const real psi11 = 1./(2.*(2.*1.+3.));
                    const real psi21 = 1./(2.*(2.*2.+3.));
                    const real psi31 = 1./(2.*(2.*3.+3.));
                    const real psi41 = 1./(2.*(2.*4.+3.));
                    int m1=0;  // m value for first solid harmonic "wn"
                    int m2=0;  // m value for second solid harmonic "phi"
                    const real n2p1 = 2.*n+1.;
                    int i1,i2,i3;
                    real x0,y0,z0,r;
                    real kr,kr2,kr3,kr4,kr5,kr7,kr9;
                    real hr,hr2,hr3,hr4,hr5,hr7,hr9;
                    real u1,u2,u3;
                    real v1,v2,v3;
                    real u1x,u1y,u1z, u2x,u2y,u2z, u3x,u3y,u3z,div;
                    real s11,s12,s13,s21,s22,s23,s31,s32,s33;
                    real psi1hr, psi2hr, psi3hr, psi4hr;
                    real psi1kr, psi2kr, psi3kr, psi4kr;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        x0 = X(i1,i2,i3,0);
                        y0 = X(i1,i2,i3,1);
                        z0 = X(i1,i2,i3,2);
                        r = sqrt( x0*x0 + y0*y0 + z0*z0 );
                        kr =kappa*r;
                        kr2=kr*kr;
                        kr3=kr2*kr;
                        kr5=kr3*kr2;
                        kr7=kr5*kr2;
                        real coskr = cos(kr);
                        real sinkr = sin(kr);
                        hr = h*r;
                        hr2=hr*hr;
                        hr3=hr2*hr;
                        hr5=hr3*hr2;
                        hr7=hr5*hr2;
                        real coshr = cos(hr);
                        real sinhr = sin(hr);
                        if( n==0 )
                        {
              // radial vibrations
                  	if( hr < rEps ) // use taylor series: (Love p279)
                  	{
                    	  psi1hr = psi10*( 1. - psi11*hr2 );  // + O( hr^4 )
                  	}
                  	else
                  	{
                    	  psi1hr =   ( hr *coshr - sinhr )/hr3;
                  	}
                            u1 = amp*cost*( x0*psi1hr );
                            u2 = amp*cost*( y0*psi1hr );
                            u3 = amp*cost*( z0*psi1hr );
                  	if( assignVelocities )
                  	{
                    	  v1 = -amp*omega*sint*( x0*psi1hr );
                    	  v2 = -amp*omega*sint*( y0*psi1hr );
                    	  v3 = -amp*omega*sint*( z0*psi1hr );
                  	}
                  	if( assignStress )
                  	{
                    	  if( hr < rEps ) // use taylor series: (Love p279)
                      	    psi2hr = psi20*( 1. - psi21*hr2 );  // + O( hr^4 )
                    	  else
                      	    psi2hr = - ( hr2*sinhr + 3.*hr*coshr - 3.*sinhr)/hr5;
                // u1 = x*psi1(hr) 
                // u1x = psi1(hr) + x*h*x/r*psi1'(hr) = psi1r + (h*x)^2 psi2(hr) 
                                u1x = amp*cost*( psi1hr + h*h*x0*x0*psi2hr );
                    	  u1y = amp*cost*(          h*h*x0*y0*psi2hr );
                    	  u1z = amp*cost*(          h*h*x0*z0*psi2hr );
                                u2x = amp*cost*(          h*h*y0*x0*psi2hr );
                    	  u2y = amp*cost*( psi1hr + h*h*y0*y0*psi2hr );
                    	  u2z = amp*cost*(          h*h*y0*z0*psi2hr );
                                u3x = amp*cost*(          h*h*z0*x0*psi2hr );
                    	  u3y = amp*cost*(          h*h*z0*y0*psi2hr );
                    	  u3z = amp*cost*( psi1hr + h*h*z0*z0*psi2hr );
                  	}
                        }
                        else if( n==2 )
                        {
              // cgDoc/sm/sphericalHarmonics.maple
              // > lprint(psi1);
              //1/x^3*(cos(x)*x-sin(x))
              //> lprint(psi2);
              //-(3*cos(x)*x-3*sin(x)+sin(x)*x^2)/x^5
              //> lprint(psi3);
              //-(x^3*cos(x)-6*sin(x)*x^2-15*cos(x)*x+15*sin(x))/x^7
      	// We need 
      	//   psi2(hr), psi3(hr)
      	//   psi1(kr), psi3(kr)
                  	if( hr < rEps ) // use taylor series: (Love p279)
                  	{
                    	  psi2hr = psi20*( 1. - psi21*hr2 );  // + O( hr^4 )
                    	  psi3hr = psi30*( 1. - psi31*hr2 );  // + O( hr^4 )
                  	}
                  	else
                  	{
                    	  psi2hr = - ( hr2*sinhr + 3.*hr*coshr - 3.*sinhr)/hr5;
                    	  psi3hr = - ( hr3*coshr - 6.*hr2*sinhr - 15.*hr*coshr +15.*sinhr)/hr7;
                  	}
                  	if( kr < rEps ) // use taylor series: (Love p279)
                  	{
                    	  psi1kr = psi10*( 1. - psi11*kr2 );  // + O( kr^4 )
                    	  psi3kr = psi30*( 1. - psi31*kr2 );  // + O( kr^4 )
                  	}
                  	else
                  	{
                    	  psi1kr =   ( kr *coskr - sinkr )/kr3;
                    	  psi3kr = - ( kr3*coskr - 6.*kr2*sinkr - 15.*kr*coskr +15.*sinkr )/kr7;
                  	}
              // wn = r^2*Y_2^0 = ( 2*z^2 - x^2 - y^2 )
              // wn = r^2*Y_2^1 = ( z*(a*x+b*y) )
              // wn = r^2*Y_2^2 = ( a*(x^2-y^2) + b*x*y )
                            real w,wx,wy,wz;
                            real wxx,wxy,wxz, wyy,wyz,wzz;
                            if( m1==0 )
                  	{
                                w = 2.*z0*z0 - x0*x0 - y0*y0;
                    	  wx = -2.*x0;
                    	  wy = -2.*y0;
                    	  wz =  4.*z0;
                                wxx = -2.; wxy=0.; wxz=0.;
                                wyy = -2.; wyz=0.;
                                wzz =  4.; 
                  	}
                            else
                  	{
                                OV_ABORT("ERROR: unexpected m1");
                  	}
              // Question: do we need to have the correct leading coeff for the solid harmonics?
      	// NOTE:
              //     an*wn+cn*pn = 0 
              //     bn*wn+dn*pn = 0 
              // 
              //  p/w = -an/cn =-bn/dn 
              // sphere2.maple:
      	//  an =-1.13478082790558e-02, cn=-3.02305786593820e-02, -an/cn=-3.75375159268876e-01
              //  bn =-2.04041203329458e-02, dn=-5.43566078598750e-02, -bn/dn=-3.75375159273096e-01
      //% % ---Digits 25: 
      //%  n=2 : root m=1  x=2.63986927790186e+00, x/Pi = 8.40296489389027e-01, kappa*a/Pi=8.40296489389027e-01
      //%  an =-1.13478082789981e-02, cn=-3.02305786589451e-02, -an/cn=-3.75375159272393e-01
      //%  bn =-2.04041203329361e-02, dn=-5.43566078599509e-02, -bn/dn=-3.75375159272393e-01
      //% 
      //%  n=2 : root m=2  x=4.86527284993742e+00, x/Pi = 1.54866444711667e+00, kappa*a/Pi=1.54866444711667e+00
      //%  an =1.50294520720980e-02, cn=1.88125428532884e-01, -an/cn=-7.98905931500425e-02
      //%  bn =-1.22064204309416e-02, dn=-1.52789207710809e-01, -bn/dn=-7.98905931500425e-02
      //% 
      //%  n=2 : root m=3  x=8.32919545905501e+00, x/Pi = 2.65126525857435e+00, kappa*a/Pi=2.65126525857435e+00
      //%  an =4.57627801659678e-03, cn=-9.86226192484144e-02, -an/cn=4.64019111586348e-02
      //%  bn =-9.34189056911829e-04, dn=2.01325556121623e-02, -bn/dn=4.64019111586348e-02
      //% 
      //%  n=2 : root m=4  x=9.78016346034290e+00, x/Pi = 3.11312271792062e+00, kappa*a/Pi=3.11312271792062e+00
      //%  an =7.28789419129781e-04, cn=4.50066777991950e-02, -an/cn=-1.61929174684121e-02
      //%  bn =1.21547211256669e-03, dn=7.50619593373295e-02, -bn/dn=-1.61929174684121e-02
      //         const real cw=1.;
      //         const real cPhi=-3.75375159268876e-01;  // ********* 
                            real p,px,py,pz;
                            real pxx,pxy,pxz, pyy,pyz,pzz; 
                  	if( m2==0 )
                  	{
                                p  = cPhi*(2.*z0*z0 - x0*x0 - y0*y0);
                    	  px = cPhi*( -2.*x0 );
                    	  py = cPhi*( -2.*y0 );
                    	  pz = cPhi*(  4.*z0 );
                                pxx = cPhi*(-2. ); pxy=0.; pxz=0.;
                                pyy = cPhi*(-2. ); pyz=0.;
                                pzz = cPhi*( 4. ); 
                  	}
                  	else if( m2==1 )
                  	{
                // here is a linear combination of the real and im parts
                // p  = cPhi*( z0*( x0+ y0) );
      	  // px = cPhi*( z0 );
      	  // py = cPhi*( z0 );
      	  // pz=  cPhi*( x0+y0 );
                  	}
                            else
                  	{
                                OV_ABORT("ERROR: unexpected m2");
                  	}
            #define VB2A(xa,wxa,pxa) ( (-1./(h*h))*( psi2hr + (hr2/n2p1)*psi3hr )*(wxa)   	    + (1./n2p1)*psi3hr*( r*r*(wxa) - n2p1*(xa)*w )  + psi1kr*(pxa) - (n/(n+1.))*psi3kr*kappa*kappa*( r*r*(pxa) - n2p1*(xa)*p ) )
      // simpler: 
            #define VB2(xa,wxa,pxa) ( (-1./(h*h))*( psi2hr*(wxa) + h*h*(xa)*psi3hr*w )   + psi1kr*(pxa) - (n/(n+1.))*psi3kr*kappa*kappa*( r*r*(pxa) - n2p1*(xa)*p ) )
                            real vb2x = VB2(x0,wx,px);
                  	real vb2y = VB2(y0,wy,py);
                  	real vb2z = VB2(z0,wz,pz);
                            u1 = amp*cost*vb2x;
                            u2 = amp*cost*vb2y;
                            u3 = amp*cost*vb2z;
                  	if( assignVelocities )
                  	{
                    	  v1 = -amp*omega*sint*vb2x;
                    	  v2 = -amp*omega*sint*vb2y;
                    	  v3 = -amp*omega*sint*vb2z;
                  	}
                  	if( assignStress )
                  	{
                // we need psi4(hr) and psi4(kr)
                // > lprint(psi4);
                //  1/x^9*(10*x^3*cos(x) -45*sin(x)*x^2+ x^4*sin(x) -105*cos(x)*x+105*sin(x))
                                kr4=kr2*kr2;
                    	  kr9=kr7*kr2;
                                hr4=hr2*hr2;
                    	  hr9=hr7*hr2;
                    	  if( hr < rEps ) // use taylor series: (Love p279)
                      	    psi4hr = psi40*( 1. - psi41*hr2 );  // + O( hr^4 )
                    	  else
                      	    psi4hr = ( (hr4-45.*hr2+105.)*sinhr + (10.*hr3-105.*hr)*coshr )/hr9;
                    	  if( kr < rEps ) // use taylor series: (Love p279)
                    	  {
                      	    psi2kr = psi20*( 1. - psi21*kr2 );  // + O( kr^4 )
                      	    psi4kr = psi40*( 1. - psi41*kr2 );  // + O( kr^4 )
                    	  }
                    	  else
                    	  {
                      	    psi2kr = - ( kr2*sinkr + 3.*kr*coskr - 3.*sinkr )/kr5;
                      	    psi4kr = ( (kr4-45.*kr2+105.)*sinkr + (10.*kr3-105.*kr)*coskr )/kr9;
                    	  }
                // D( u_j )/D( x_a ) **check me **
            #define VB2X(xj,wj,pj,  xa,wa,pa, wja,pja, deltaja )( (-1./(h2))*( h2*xa*psi3hr*wj + psi2hr*wja + h2*deltaja*psi3hr*w + h4*xj*xa*psi4hr*w + h2*xj*psi3hr*wa )	    + kappa2*xa*psi2kr*pj + psi1kr*pja 	    - (n/(n+1.))*kappa4*xa*psi4kr*( r*r*pj - n2p1*xj*p )- (n/(n+1.))*kappa2*psi3kr*( 2.*xa*pj - n2p1*(deltaja)*p + r*r*pja - n2p1*xj*pa ) )
                    	  u1x = amp*cost*( VB2X(x0,wx,px, x0,wx,px, wxx,pxx, 1. ) );
                    	  u1y = amp*cost*( VB2X(x0,wx,px, y0,wy,py, wxy,pxy, 0. ) );
                    	  u1z = amp*cost*( VB2X(x0,wx,px, z0,wz,pz, wxz,pxz, 0. ) );
                    	  u2x = amp*cost*( VB2X(y0,wy,py, x0,wx,px, wxy,pxy, 0. ) );
                    	  u2y = amp*cost*( VB2X(y0,wy,py, y0,wy,py, wyy,pyy, 1. ) );
                    	  u2z = amp*cost*( VB2X(y0,wy,py, z0,wz,pz, wyz,pyz, 0. ) );
                    	  u3x = amp*cost*( VB2X(z0,wz,pz, x0,wx,px, wxz,pxz, 0. ) );
                    	  u3y = amp*cost*( VB2X(z0,wz,pz, y0,wy,py, wyz,pyz, 0. ) );
                    	  u3z = amp*cost*( VB2X(z0,wz,pz, z0,wz,pz, wzz,pzz, 1. ) );
                  	}
            #undef VB2
                        }
                        else
                        {
                            OV_ABORT("ERROR: unexpected n");
                        }
                        if( evalSolution )
                        {
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                  	U(i1,i2,i3,wc) =u3;
                        }
                        else
                        {
                  	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                  	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                  	ERR(i1,i2,i3,wc) =U(i1,i2,i3,wc) - u3;
                        }
                        if( assignVelocities )
                        {
                            if( evalSolution )
                  	{
                    	  U(i1,i2,i3,v1c) =v1;
                    	  U(i1,i2,i3,v2c) =v2;
                    	  U(i1,i2,i3,v3c) =v3;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                    	  ERR(i1,i2,i3,v3c) =U(i1,i2,i3,v3c) - v3;
                  	}
                        }
                        if( assignStress )
                        {
                  	div = u1x+u2y+u3z;
                  	s11 = lambda*div + 2.*mu*u1x;
                  	s12 = mu*( u1y+u2x );
                  	s13 = mu*( u1z+u3x );
                  	s21 = s12;
                  	s22 = lambda*div + 2.*mu*u2y;
                  	s23 = mu*( u2z + u3y );
                  	s31 = s13;
                  	s32 = s23;
                  	s33 = lambda*div + 2.*mu*u3z;
                            if( evalSolution )
                  	{
                    	  U(i1,i2,i3,s11c) =s11;
                    	  U(i1,i2,i3,s12c) =s12;
                    	  U(i1,i2,i3,s13c) =s13;
                    	  U(i1,i2,i3,s21c) =s21;
                    	  U(i1,i2,i3,s22c) =s22;
                    	  U(i1,i2,i3,s23c) =s23;
                    	  U(i1,i2,i3,s31c) =s31;
                    	  U(i1,i2,i3,s32c) =s32;
                    	  U(i1,i2,i3,s33c) =s33;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) - s11;
                    	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) - s12;
                    	  ERR(i1,i2,i3,s13c) =U(i1,i2,i3,s13c) - s13;
                    	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) - s21;
                    	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) - s22;
                    	  ERR(i1,i2,i3,s23c) =U(i1,i2,i3,s23c) - s23;
                    	  ERR(i1,i2,i3,s31c) =U(i1,i2,i3,s31c) - s31;
                    	  ERR(i1,i2,i3,s32c) =U(i1,i2,i3,s32c) - s32;
                    	  ERR(i1,i2,i3,s33c) =U(i1,i2,i3,s33c) - s33;
                  	}
                        }
                    }  // end for3d
                }
                else
                {
                    OV_ABORT("ERROR: unexpected vibrationClass");
                }
            #undef U2
            #undef V2
            #undef W2
            }

        }
        else if( specialInitialConditionOption == "RayleighWave" )
        {
      // --- Rayleigh wave solution ---
            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines)
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                bool assignStress = s11c >=0 ;
                real cp = sqrt( (lambda+2.*mu)/rho );
                real cs = sqrt( mu/rho );
                std::vector<real> & data = parameters.dbase.get<std::vector<real> >("RayleighWaveData");
                const int nk = int(data[0]);  // number of modes
                const real cr    = data[1];   // Rayleigh wave speed
                const real ySurf = data[2];   // y value on surface
                const real period= data[3];   // Rayleigh wave speed
                const real xShift= data[4];   // shift in x for wave 
                const int mStart=5;  // Fourier coeff's start at this index in the data 
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: RayelighWave: finish me for HEMP **********\n\n");
                    OV_ABORT("error");
                }
                real cb1 = sqrt(1.-SQR(cr/cp)); // b1/k : for computing b1 
                real cb2 = sqrt(1.-SQR(cr/cs)); // b2/k : for computing b2 
                real c1 = .5*SQR(cr/cs)-1.; // x/2-1 ,   x=cr^2/cs^2
                if( t==0. )
                {
                    printF("**** RayleighWave: ySurf=%8.2e, cr=%8.2e, period=%8.2e, t=%8.2e *********\n",ySurf,cr,period,t);
                    int m=mStart;
                    for( int n=0; n<nk; n++ ) 
                    {
                        real k = data[m++];  // k=wave-number
                        real a=data[m++];    // an : amplitude
                        real b=data[m++]; 
                        printF(" k%i = %e, a%i=%e, b%i=%e\n",n,k,n,a,n,b);
                    }
                }
                real scale = -1./( cb1+(c1/cb2) ); // make coeff of cos() in u2 = a at y=0
                int i1,i2,i3;
                if( mg.numberOfDimensions()==2 )
                {
                    real z0=0.;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0)-xShift;
                        real y0 = X(i1,i2,i3,1)-ySurf;
                        real u1=0., u2=0.;  
                        real v1=0., v2=0.;
                        real s11=0., s12=0., s22=0.;
                        int m=mStart;
            // --- loop over different values of k and add contributions ---
                        for( int n=0; n<nk; n++ ) 
                        {
                  	real k = twoPi*data[m++]/period;  // 2*pi*k/period , k=wave-number
              // -- note definition of a and b so that they define the Fourier coefficients
              //    of u2 on the surface
                            real b=data[m++]*scale;          
                            real a=data[m++]*scale;          
                            real b1= k*cb1, b2=k*cb2;
                            real eb1 = exp(b1*(y0)), eb2 = exp(b2*(y0));
                            real ct = cos(k*(x0-cr*t));
                  	real st = sin(k*(x0-cr*t));
                            u1 +=  ( eb1 + c1*eb2           )*( a*ct+b*st);
                  	u2 +=  ( cb1*eb1 + (c1/cb2)*eb2 )*( a*st-b*ct);
                  	if( assignVelocities )
                  	{
                    	  v1 += ( eb1 + c1*eb2           )*( k*cr*( a*st-b*ct) );
                    	  v2 +=-( cb1*eb1 + (c1/cb2)*eb2 )*( k*cr*( a*ct+b*st) );
                  	}
                  	if( assignStress )
                  	{   
                                real u1x = ( eb1 + c1*eb2       )*( k*(-a*st+b*ct) );
                                real u1y = ( b1*eb1 + b2*c1*eb2 )*(   ( a*ct+b*st) );
                                real u2x = ( cb1*eb1       + (c1/cb2)*eb2 )*( k*(a*ct+b*st) );
                                real u2y = ( b1*cb1*eb1 + b2*(c1/cb2)*eb2 )*(   (a*st-b*ct) );
                                real div=u1x+u2y;
                    	  s11 += lambda*div+2.*mu*u1x;
                    	  s12 += mu*( u1y+u2x );
                    	  s22 += lambda*div+2.*mu*u2y;
                  	}
                        }
            // printF(" (i1,i2)=(%i,%i) (x0,y0)=(%8.2e,%8.2e) xi=%8.2e (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,x0,y0,xi,u1,u2);
                        if( evalSolution )
                        {
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                            if( assignVelocities )
                  	{
                    	  U(i1,i2,i3,v1c) = v1;
                    	  U(i1,i2,i3,v2c) = v2;
                  	}
                  	if( assignStress )
                  	{
                    	  U(i1,i2,i3,s11c) =s11;
                    	  U(i1,i2,i3,s12c) =s12;
                    	  U(i1,i2,i3,s21c) =s12;
                    	  U(i1,i2,i3,s22c) =s22;
                  	}
                        }
                        else
                        {
                  	if( pdeVariation == SmParameters::hemp )
                  	{
                    	  ERR(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                    	  ERR(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                    	  ERR(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                    	  ERR(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                  	}
                  	else
                  	{
                    	  ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                    	  ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                    	  }
                            if( assignVelocities )
                  	{
                    	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                  	}
                  	if( assignStress )
                  	{
                    	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                    	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                    	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                    	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                  	}
                        }
                    } // end FOR_3D
                }
                else
                {
                    OV_ABORT("RayleighWave:ERROR: finish me for 3D");
                }
            }
        }
        else if( specialInitialConditionOption == "pistonMotion" )
        {
      // --- piston motion (for FSI) ---
            bool evalSolution = evalOption==computeInitialConditions;
      // macro: (NOTE: this macro is also called in the SOS and FOS BC routines)
            {
                const int v1c = parameters.dbase.get<int >("v1c");
                const int v2c = parameters.dbase.get<int >("v2c");
                const int v3c = parameters.dbase.get<int >("v3c");
                bool assignVelocities= v1c>=0 ;
                const int s11c = parameters.dbase.get<int >("s11c");
                const int s12c = parameters.dbase.get<int >("s12c");
                const int s13c = parameters.dbase.get<int >("s13c");
                const int s21c = parameters.dbase.get<int >("s21c");
                const int s22c = parameters.dbase.get<int >("s22c");
                const int s23c = parameters.dbase.get<int >("s23c");
                const int s31c = parameters.dbase.get<int >("s31c");
                const int s32c = parameters.dbase.get<int >("s32c");
                const int s33c = parameters.dbase.get<int >("s33c");
                const int pc = parameters.dbase.get<int >("pc");
                bool assignStress = s11c >=0 ;
                const real cp = sqrt( (lambda+2.*mu)/rho );
                const real cs = sqrt( mu/rho );
                std::vector<real> & data = parameters.dbase.get<std::vector<real> >("pistonMotionData");
                int m=0;
                const real a    =data[m++];
                const real p    =data[m++];
                const real rhog =data[m++];
                const real pg   =data[m++];
                const real gamma=data[m++];
                real angle      =data[m++];  // angle (in degrees) for a rotated piston
                const real a0 = sqrt( gamma*pg/rhog);  // speed of sound in the gas
                if( pdeVariation == SmParameters::hemp )
                {
                    printF("\n\n **************** FIX ME: getPistonMotionSolution: finish me for HEMP **********\n\n");
                    OV_ABORT("error");
                }
                if( t==0. )
                {
                    printP("**** getPistonMotion: a=%8.2e, p=%8.2e, Gas: rho=%8.2e, p=%8.2e gamma=%8.2e angle=%5.2f(degrees)*******\n",
                                  a,p,rhog,pg,gamma,angle);
                }
                angle = angle*Pi/180.;
                const real cosa = cos(angle), sina=sin(angle);
                const real cg1 = pg/(rho*cp*cp);
                const real cg2 = (-a)*(gamma-1.)/(2.*a0);
        // we assume gamma=1.4
                assert( fabs(gamma-1.4) < REAL_EPSILON*100. );
                int i1,i2,i3;
                if( mg.numberOfDimensions()==2 )
                {
                    real z0=0.;
                    FOR_3D(i1,i2,i3,I1,I2,I3)
                    {
                        real x0 = X(i1,i2,i3,0);
                        real y0 = X(i1,i2,i3,1);
                        real u1=0., u2=0.;  
                        real v1=0., v2=0.;
                        real s11=0., s12=0., s22=0.;
            // Boundary motion:
            //    F(t) = -(a/p)*t^p 
            //    F'(t) = -a*t^{p-1}
            // Solution: 
            //    u1 = f(x-cp*t) + g(x+cp*t)
            // where  
            //   f(x) = - 1/(2*cp)*( int_0^x v0(s) ds ) = -(1/2)* int_0^x G(-s/cp) ds  ,   for   x<0
            //                                          =  (cp/2)* int_0^{-x/cp} G(u) du 
            //   g(x) = + 1/(2*cp)*( int_0^x v0(s) ds ) =  (1/2)* int_0^x G(-s/cp) ds  ,   for   x<0
            //                                          = -(cp/2)* int_0^{-x/cp} G(u) du 
            //   g(x) = F(x/cp) - f(-x),                   for   x>0 
            //
            //   v0(s) = cp*G(-t/cp)   : velocity at t=0 for s<0
            //   G(t) = (pg/(rho*cp^2)) * [ 1 + (gamma-1)/(2*a0)* F'(t) ]^7 + F'(t)/cp
            //        = cg1* [ 1 + cg2*t^{p-1} ]^7 + F'(t)/cp
            //     cg1=p0/(rho*cp^2), cg2=(-a)*(gamma-1)/(2*a0)
            // 
            // where we have assumed that gamma=1.4=7/5 so that 2*gamma/(gamma-1)= 7 
            // 
            //  Int_0^t G(s) ds = cg1*[ t + (7/p)*cg2*t^{p-1}*t + (21/(2p-1))*(cg2*t^{p-1})^2*t + ) + F(t)/cp
            //                  = cg1*t*[ 1 + Z*(7)/(p) + Z^2*(21)/(2p-1) + Z^3*(35)/(3p-2) + Z^4*(35)/(4p-3) + ...
            //                              + Z^5*(21)/(5p-4) + Z^6*(7)/(6p-5) + Z^7*(1)/(7p-6) ] 
            //   Z=cg2*t^{p-1}
            // xa = distance of point (x0,y0) from the plane  (cosa,sina).(x,y)=0 
                        real xa = x0*cosa + y0*sina;
                        real xp = xa + cp*t;
                        real xm = xa - cp*t;
                        real fm, fmPrime, gp, gpPrime;
                        {
                            real xx = -xm/cp;
                            real xp1 = pow(xx,p-1);
                            real z=cg2*xp1;
                            fm = cp*(.5*(cg1*(xx)*(1.+(z)*(7./p+(z)*(21./(2.*p-1.)+(z)*(35./(3.*p-2.)+(z)*(35./(4.*p-3.)			+(z)*(21./(5.*p-4.)+(z)*(7./(6.*p-5.)+z/(7.*p-6.))))))))))  - .5*(a/p)*xp1*xx;
                            fmPrime = .5*( -cg1*pow( 1. + z , 7.) + a*xp1/cp );
                        }
                        {
                            if( xp<=0. )
                            {
                                real xx = -xp/cp;
                                real xp1 = pow(xx,p-1);
                                real z=cg2*xp1;
                                gp = cp*(.5*(cg1*(xx)*(1.+(z)*(7./p+(z)*(21./(2.*p-1.)+(z)*(35./(3.*p-2.)+(z)*(35./(4.*p-3.)			+(z)*(21./(5.*p-4.)+(z)*(7./(6.*p-5.)+z/(7.*p-6.))))))))))  + .5*(a/p)*xp1*xx ;
                                gpPrime = +.5*( -cg1*pow( 1. + z , 7.) -a*xp1/cp );
                            }
                            else
                            {
                 //   gp(xp) = F(xp/cp) - f(-xp),
                                real xx=xp/cp;
                                real xp1 = pow(xx,p-1);
                                real z=cg2*xp1;
                                gp = -(a/p)*xp1*xx - cp*(.5*(cg1*(xx)*(1.+(z)*(7./p+(z)*(21./(2.*p-1.)+(z)*(35./(3.*p-2.)+(z)*(35./(4.*p-3.)			+(z)*(21./(5.*p-4.)+(z)*(7./(6.*p-5.)+z/(7.*p-6.)))))))))) + .5*(a/p)*xp1*xx;
                                gpPrime = -a*xp1/cp + .5*( -cg1*pow( 1. + z , 7.) + a*xp1/cp );
                            }
                        }
                        real ua = fm + gp;
                        real va = cp*( - fmPrime + gpPrime );
                        real uap=fmPrime + gpPrime;
            // Piston at an angle:
            //   n = [ cos(angle) , sin(angle) ] = [ c, s ]    -- normal to the face
            //   u1 = c*ua,  u2=s*ua
            //   u1.x = c*ua.x,  u1.y = c*ua.y,   u2.x = s*ua.x, u2.y = s*ua.y
            //   
            // (1)  ua.n = c*ua.x + s*ua.y = uap
            // (2)  ua.t =-s*ua.x + c*ua.y = 0    "tangential derivative of motion"
            //
            //  (1) and (2) -->   ua.x = c*uap, ua.y=s*uap
            // Thus: u1.x = c*ua.x = c*c*uap,  u1.y = c*ua.y = c*s*uap
                        u1 = ua*cosa;
                        u2 = ua*sina;
                        v1 = va*cosa;
                        v2 = va*sina;
                        real u1x = uap*cosa*cosa;
                        real u1y = uap*cosa*sina;
                        real u2x = uap*sina*cosa;
                        real u2y = uap*sina*sina;
                        s11 = (lambda+2.*mu)*u1x + lambda*u2y;   
                        s12 = mu*( u1y+u2x );
                        s22 = lambda*u1x + (lambda+2.*mu)*u2y;
            // printF("piston (i1,i2)=(%i,%i) (u1,u2)=(%8.2e,%8.2e)\n",i1,i2,u1,u2);
                        if( evalSolution )
                        {
                  	U(i1,i2,i3,uc) =u1;
                  	U(i1,i2,i3,vc) =u2;
                            if( assignVelocities )
                  	{
                    	  U(i1,i2,i3,v1c) = v1;
                    	  U(i1,i2,i3,v2c) = v2;
                  	}
                  	if( assignStress )
                  	{
                    	  U(i1,i2,i3,s11c) =s11;
                    	  U(i1,i2,i3,s12c) =s12;
                    	  U(i1,i2,i3,s21c) =s12;
                    	  U(i1,i2,i3,s22c) =s22;
                  	}
                        }
                        else
                        {
                  	ERR(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                  	ERR(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                            if( assignVelocities )
                  	{
                    	  ERR(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                    	  ERR(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                  	}
                  	if( assignStress )
                  	{
                    	  ERR(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                    	  ERR(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                    	  ERR(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                    	  ERR(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                  	}
                        }
                    } // end FOR_3D
                }
                else
                {
                    OV_ABORT("getPistonMotion:ERROR: finish me for 3D");
                }
            }
        }
        else
        {
            printF("Cgsm:assignSpecialInitialConditions:ERROR: unknown specialInitialConditionOption=[%s]\n",
                  (const char*)specialInitialConditionOption);
            Overture::abort("error");
        }
        

    } // end for grid


    return 0;
}


