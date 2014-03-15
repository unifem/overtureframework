// This file automatically generated from assignTwilightZoneInitialConditions.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "ParallelUtility.h"

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ===========================================================================================
/// \brief Assign twilight zone initial conditions.
/// \details This function assigns the solution at t=0. It may also initialize the solution
///    at previous times.
/// \param gfIndex (input) : assign gf[gfIndex].u 
// ===========================================================================================
int Cgsm::
assignTwilightZoneInitialConditions(int gfIndex)
{
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
        

        OGFunction *& tz = parameters.dbase.get<OGFunction* >("exactSolution");
        assert( tz!=NULL );
        OGFunction & e = *tz;
          	    
        if( mg.numberOfDimensions()==2 )
        {
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	real x0 = X(i1,i2,i3,0);
      	real y0 = X(i1,i2,i3,1);

                for( int c=0; c<numberOfComponents; c++ )
      	{
        	  U(i1,i2,i3,c) =e(x0,y0,0.,c,t);
      	}
      	
            }
        }
        else
        { // ***** 3D TZ IC's ****
            FOR_3D(i1,i2,i3,I1,I2,I3)
            {
      	real x0 = X(i1,i2,i3,0);
      	real y0 = X(i1,i2,i3,1);
      	real z0 = X(i1,i2,i3,2);
                for( int c=0; c<numberOfComponents; c++ )
      	{
        	  U(i1,i2,i3,c) =e(x0,y0,z0,c,t);
      	}
            }
        }

    } // end for grid


    return 0;
}


