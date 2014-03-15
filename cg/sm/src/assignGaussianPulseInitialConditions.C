// This file automatically generated from assignGaussianPulseInitialConditions.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPulseFunction.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ===========================================================================================
/// \brief Assign the Gaussian Pulse initial conditions
/// \details This function assigns the solution at t=0.
/// \param gfIndex (input) : assign gf[gfIndex].u 
// ===========================================================================================
int Cgsm::
assignGaussianPulseInitialConditions(int gfIndex)
{
    real time0=getCPU();
    
    const real t = gf[gfIndex].t;

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

    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                                                      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

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

        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter ); // do this for now
        const realArray & x = mg.center();

        realMappedGridFunction & u =gf[gfIndex].u[grid];

#ifdef USE_PPP
        realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
#else
        realSerialArray & uLocal  =  u;
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

        getIndex(mg.dimension(),I1,I2,I3);

        int includeGhost=1;
        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
        if( !ok ) continue;


        lambda = lambdaGrid(grid);
        mu = muGrid(grid);
        c1=(mu+lambda)/rho, c2= mu/rho;

        uLocal=0.;
        
        SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

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
        

    // real c= sqrt( (2.*mu+lambda)/rho ); // fix this ************ why do we use this : need a pulse velocity *****
        real c= 0.;

        int pulse=0;
        const real *gpp = gaussianPulseParameters[pulse];

        const real beta    = gpp[0];
        const real scale   = gpp[1];
        const real exponent= gpp[2];
        const real x0      = gpp[3];
        const real y0      = gpp[4];
        const real z0      = gpp[5];

    // Pulse parameters:
        real alpha=exponent; // 50.; // 200.;
        real pulsePow=10.; // 20
        real a0=3.;
        real xPulse=x0; // .5;
        real yPulse=y0; // .5;
        real zPulse=z0; // .5;
#define U0(x,y,t) ( exp( - alpha*( SQR((x)-(xPulse-c*t)) + SQR((y)-yPulse) ) ) )
#define U0T(x,y,t) ( (-2.*c*alpha)*( (x)-(xPulse-c*t) )*U0(x,y,t) )
#define U0X(x,y,t) ( (  -2.*alpha)*( (x)-(xPulse-c*t) )*U0(x,y,t) )
#define U0Y(x,y,t) ( (  -2.*alpha)*( (y)-(yPulse    ) )*U0(x,y,t) )

#define U3D(x,y,z,t) exp( - alpha*( SQR((x)-(xPulse-c*t)) + SQR((y)-yPulse) + SQR((z)-zPulse) ) )
#define U3DT(x,y,z,t) ( (-2.*c*alpha)*( (x)-(xPulse-c*t) )*U3D(x,y,z,t) )
#define U3DX(x,y,z,t) ( (  -2.*alpha)*( (x)-(xPulse-c*t) )*U3D(x,y,z,t) )
#define U3DY(x,y,z,t) ( (  -2.*alpha)*( (y)-(yPulse    ) )*U3D(x,y,z,t) )
#define U3DZ(x,y,z,t) ( (  -2.*alpha)*( (z)-(zPulse    ) )*U3D(x,y,z,t) )

        printF("+++ assignGaussianPulseIC: t=%9.3e dt=%9.3e c=%9.3e\n",t,dt,c);

        if( isRectangular )
        {
      // for a rectangular grid we avoid building the array of verticies.
      // we assign the initial conditions with C-style loops
            real dx[3]={0.,0.,0.}, xab[2][3]={0.,0.,0.,0.,0.,0.};
            if( cg[grid].isRectangular() )
      	cg[grid].getRectangularGridParameters( dx, xab );

            const real xa=xab[0][0], dx0=dx[0];
            const real ya=xab[0][1], dy0=dx[1];
            const real za=xab[0][2], dz0=dx[2];

            const int i0a=cg[grid].gridIndexRange(0,0);
            const int i1a=cg[grid].gridIndexRange(0,1);
            const int i2a=cg[grid].gridIndexRange(0,2);

#define VERTEX0(i0,i1,i2) xa+dx0*(i0-i0a)
#define VERTEX1(i0,i1,i2) ya+dy0*(i1-i1a)
#define VERTEX2(i0,i1,i2) za+dz0*(i2-i2a)

            int i1,i2,i3;
            if( numberOfDimensions==2 )
            {
      	FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      	{
        	  U(i1,i2,i3,uc) =U0(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),t);
        	  U(i1,i2,i3,vc) =0.;
      	}
      	if( assignVelocities )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    U(i1,i2,i3,v1c) =U0T(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),t);
          	    U(i1,i2,i3,v2c) =0.;
        	  }
      	}
                if( assignStress )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    U(i1,i2,i3,s11c) =(lambda+2.*mu)*U0X(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),t);
          	    U(i1,i2,i3,s12c) =mu*U0Y(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),t);
          	    U(i1,i2,i3,s21c) =U(i1,i2,i3,s12c);
          	    U(i1,i2,i3,s22c) =lambda*U0X(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),t);
        	  }
      	}
            }
            if( numberOfDimensions==3 )
            {
      	FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
      	{
        	  U(i1,i2,i3,uc) =U3D(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3),t);
        	  U(i1,i2,i3,vc) =0.;
        	  U(i1,i2,i3,wc) =0.;
      	}
      	if( assignVelocities )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    U(i1,i2,i3,v1c) =U3DT(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3),t);
          	    U(i1,i2,i3,v2c) =0.;
          	    U(i1,i2,i3,v3c) =0.;
        	  }
      	}
                if( assignStress )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    U(i1,i2,i3,s11c) =(lambda+2.*mu)*U3DX(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3),t);
          	    U(i1,i2,i3,s12c) =mu*U3DY(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3),t);
          	    U(i1,i2,i3,s13c) =mu*U3DZ(VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3),t);
          	    U(i1,i2,i3,s21c) =U(i1,i2,i3,s12c);
          	    U(i1,i2,i3,s22c) =0.;
          	    U(i1,i2,i3,s23c) =0.;
          	    U(i1,i2,i3,s31c) =U(i1,i2,i3,s13c);
          	    U(i1,i2,i3,s32c) =U(i1,i2,i3,s23c);
          	    U(i1,i2,i3,s33c) =0.;
        	  }
      	}
            }

#undef VERTEX0
#undef VERTEX1
#undef VERTEX2
        }
        else
        {
      // --- curvilinear grid case ----

            if( numberOfDimensions==2 )
            {
      	uLocal(I1,I2,I3,uc)=U0(xLocal(I1,I2,I3,0),xLocal(I1,I2,I3,1),t);
      	uLocal(I1,I2,I3,vc)=0.;

      	if( assignVelocities )
      	{
                    uLocal(I1,I2,I3,v1c)=U0T(xLocal(I1,I2,I3,0),xLocal(I1,I2,I3,1),t);
                    uLocal(I1,I2,I3,v2c)=0.;
      	}
                if( assignStress )
      	{
          // -- assign the stress --
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    uLocal(i1,i2,i3,s11c) =(lambda+2.*mu)*U0X(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),t);
          	    uLocal(i1,i2,i3,s12c) =mu*U0Y(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),t);
          	    uLocal(i1,i2,i3,s21c) =U(i1,i2,i3,s12c);
          	    uLocal(i1,i2,i3,s22c) =lambda*U0X(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),t);
        	  }
      	}
            }	
            if( numberOfDimensions==3 )
            {
      	uLocal(I1,I2,I3,uc)=U3D(xLocal(I1,I2,I3,0),xLocal(I1,I2,I3,1),xLocal(I1,I2,I3,2),t);
      	uLocal(I1,I2,I3,vc)=0.;
      	uLocal(I1,I2,I3,wc)=0.;

      	if( assignVelocities )
      	{
                    uLocal(I1,I2,I3,v1c)=U3DT(xLocal(I1,I2,I3,0),xLocal(I1,I2,I3,1),xLocal(I1,I2,I3,2),t);
                    uLocal(I1,I2,I3,v2c)=0.;
                    uLocal(I1,I2,I3,v3c)=0.;
      	}
                if( assignStress )
      	{
        	  FOR_3D(i1,i2,i3,I1,I2,I3) // loop over all points
        	  {
          	    U(i1,i2,i3,s11c) =(lambda+2.*mu)*U3DX(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),xLocal(i1,i2,i3,2),t);
          	    U(i1,i2,i3,s12c) =mu*U3DY(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),xLocal(i1,i2,i3,2),t);
          	    U(i1,i2,i3,s13c) =mu*U3DZ(xLocal(i1,i2,i3,0),xLocal(i1,i2,i3,1),xLocal(i1,i2,i3,2),t);
          	    U(i1,i2,i3,s21c) =U(i1,i2,i3,s12c);
          	    U(i1,i2,i3,s22c) =0.;
          	    U(i1,i2,i3,s23c) =0.;
          	    U(i1,i2,i3,s31c) =U(i1,i2,i3,s13c);
          	    U(i1,i2,i3,s32c) =U(i1,i2,i3,s23c);
          	    U(i1,i2,i3,s33c) =0.;
        	  }
      	}
            }
        }
    } // end for grid

    return 0;
}

