// This file automatically generated from assignHempInitialConditions.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "ParallelUtility.h"

#define getEng EXTERN_C_NAME(geteng)
#define getArea EXTERN_C_NAME(getarea)

extern "C"
{
    void getEng( const real &eta, const real &p, real &e, real &a, real &b, real &c, real &d );

    void getArea( const real &x1, const real &x2, const real &x3, const real &x4,
            		const real &y1, const real &y2, const real &y3, const real &y4,
            		real &Area );
}


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// ===========================================================================================
/// \brief Assign initial conditions for the Hemp solver.
/// \details This function assigns the solution at t=0. It may also initialize the solution
///    at previous times.
/// \param gfIndex (input) : assign gf[gfIndex].u 
// ===========================================================================================
int Cgsm::
assignHempInitialConditions(int gfIndex)
{
    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    assert( pdeVariation == SmParameters::hemp );

    aString & hempInitialConditionOption = parameters.dbase.get<aString>("hempInitialConditionOption");

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

    const int & pc =  parameters.dbase.get<int >("pc");
    const int & qc =  parameters.dbase.get<int >("qc");

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

  // Here is where we store the initial state (mass,density,energy)
    realCompositeGridFunction & initialState = 
                                *(parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction"));


    Range C=numberOfComponents;
    Index I1,I2,I3;
    int i1,i2,i3;
    
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
        MappedGrid & mg = cg[grid];
        const bool isRectangular = mg.isRectangular();

        mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEcenterJacobian ); // do this for now
        const realArray & x = mg.center();

        realMappedGridFunction & u =gf[gfIndex].u[grid];
        realMappedGridFunction & state0 = initialState[grid];

#ifdef USE_PPP
        realSerialArray uLocal;  getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray state0Local;  getLocalArrayWithGhostBoundaries(state0,state0Local);
        realSerialArray xLocal;  getLocalArrayWithGhostBoundaries(x,xLocal);
        const realSerialArray & det = mg.centerJacobian().getLocalArray();
#else
        realSerialArray & uLocal  =  u;
        realSerialArray & state0Local  =  state0;
        const realSerialArray & xLocal  =  x;
        const realSerialArray & det = mg.centerJacobian();
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

        real k =2.*Pi;
        real omega = k;
        real a[3]={.1,0.,0.}; 

        uLocal=0.;
        
    // here is an initial state

    //#define U0(x,y,z,n,t) (a[n-uc]*sin(k*(x))*cos(omega*(t)))
    //#define U0T(x,y,z,n,t) (-a[n-uc]*omega*sin(k*(x))*sin(omega*(t)))
#define U0(x,y,z,n,t) (0.)
#define U0T(x,y,z,n,t) (0.)
#define S110(x,y,z,t) (0.)
#define S120(x,y,z,t) (0.)
#define S220(x,y,z,t) (0.)
#define P0(x,y,z,t) (1.0e-10)
#define Q0(x,y,z,t) (0.)
      
        assert( pc>=0 && qc>=0 );
        
        if( mg.numberOfDimensions()==2 )
        {
      // 2D: solution components n=1,2,3,...,9 :
      //      x,y, v1,v2, s11,s12,s22, p, q 

            if( hempInitialConditionOption == "default" ||
                    hempInitialConditionOption == "sedov"  )
            {
      	real z0=0.;

      	FOR_3D(i1,i2,i3,I1,I2,I3)
      	{
        	  state0Local(i1,i2,i3,0) = 1.;  // mass at t=0. 
        	  state0Local(i1,i2,i3,1) = 1.;  // density at t=0. 
        	  state0Local(i1,i2,i3,2) = 1.;  // energy at t=0. 

        	  real x0 = X(i1,i2,i3,0);
        	  real y0 = X(i1,i2,i3,1);

        	  U(i1,i2,i3,uc) =x0 + U0(x0,y0,z0,uc,t);
        	  U(i1,i2,i3,vc) =y0 + U0(x0,y0,z0,vc,t);

          // X(i1,i2,i3,0) = x0;
	  // X(i1,i2,i3,1) = y0;

        	  U(i1,i2,i3,v1c) =U0T(x0,y0,z0,uc,t);
        	  U(i1,i2,i3,v2c) =U0T(x0,y0,z0,vc,t);

        	  U(i1,i2,i3,s11c) =S110(x0,y0,z0,t);
        	  U(i1,i2,i3,s12c) =S120(x0,y0,z0,t);
        	  U(i1,i2,i3,s22c) =S220(x0,y0,z0,t);

        	  U(i1,i2,i3,pc)   =P0(x0,y0,z0,t);
        	  U(i1,i2,i3,qc)   =Q0(x0,y0,z0,t);
      	}
      	int i1h,i2h,i3h;
      	i1h = (int)(0.5*(I1.getBase()+I1.getBound()));
      	i2h = (int)(0.5*(I2.getBase()+I2.getBound()));
      	i3h = I3.getBase();
      	real dx,dy;
      	dx = fabs(U(I1.getBase()+1,I2.getBase(),I3.getBase(),uc)-U(I1.getBase(),I2.getBase(),I3.getBase(),uc));
      	dy = fabs(U(I1.getBase(),I2.getBase()+1,I3.getBase(),vc)-U(I1.getBase(),I2.getBase(),I3.getBase(),vc));

      	if( hempInitialConditionOption == "sedov" )
      	{
          // -- Add a point energy source ---
        	  real p0 = 0.25*.323713580247/(dx*dy);
	  //real p0 = 1.0e-10;
        	  U(i1h,i2h,i3h,pc) = p0;
        	  U(i1h+1,i2h,i3h,pc) = p0;
        	  U(i1h,i2h+1,i3h,pc) = p0;
        	  U(i1h+1,i2h+1,i3h,pc) = p0;
      	}
      	
      	const std::vector<real> & polyEOS = parameters.dbase.get<std::vector<real> >("polyEos"); // a,b,c,d in Wilkin's EOS
      	real eta,press,eng,a,b,c,d;
      	a = polyEOS[0];
      	b = polyEOS[1];
      	c = polyEOS[2];
      	d = polyEOS[3];
      	
      	FOR_3(i1,i2,i3,I1,I2,I3)
      	{
        	  state0Local(i1,i2,i3,1) = 1.;  // density at t=0. 
        	  state0Local(i1,i2,i3,0) = dx*dy*state0Local(i1,i2,i3,1);  // mass at t=0. 

        	  press = U(i1,i2,i3,pc);
        	  eta = 1.0;
        	  getEng( eta,press,eng,a,b,c,d );
        	  state0Local(i1,i2,i3,2) = eng;  // energy at t=0. 
      	}
            }
            else if( hempInitialConditionOption == "piston" )
            {
      	real z0=0.;

      	FOR_3D(i1,i2,i3,I1,I2,I3)
      	{
        	  state0Local(i1,i2,i3,0) = -1.;  // mass at t=0. 
        	  state0Local(i1,i2,i3,1) = 1.;  // density at t=0. 
        	  state0Local(i1,i2,i3,2) = 1.;  // energy at t=0. 

        	  U(i1,i2,i3,uc) = X(i1,i2,i3,0);
        	  U(i1,i2,i3,vc) = X(i1,i2,i3,1);

        	  U(i1,i2,i3,v1c) = 0.0;
        	  U(i1,i2,i3,v2c) = 0.0;

        	  U(i1,i2,i3,s11c) = 0.0; 
        	  U(i1,i2,i3,s12c) = 0.0;
        	  U(i1,i2,i3,s22c) = 0.0;

        	  U(i1,i2,i3,pc)   = 1.e0;
        	  U(i1,i2,i3,qc)   = 0.0;
      	}
      	const std::vector<real> & polyEOS = parameters.dbase.get<std::vector<real> >("polyEos"); // a,b,c,d in Wilkin's EOS
      	real eta,press,eng,a,b,c,d;
      	a = polyEOS[0];
      	b = polyEOS[1];
      	c = polyEOS[2];
      	d = polyEOS[3];
      	real dx = fabs(U(I1.getBase()+1,I2.getBase(),I3.getBase(),uc)-U(I1.getBase(),I2.getBase(),I3.getBase(),uc));
      	real dy = fabs(U(I1.getBase(),I2.getBase()+1,I3.getBase(),vc)-U(I1.getBase(),I2.getBase(),I3.getBase(),vc));
      	
      	FOR_3(i1,i2,i3,I1,I2,I3)
      	{
        	  state0Local(i1,i2,i3,1) = 1.;  // density at t=0. 
	  //state0Local(i1,i2,i3,0) = -dx*dy*state0Local(i1,i2,i3,1);  // mass at t=0 ... needs to be fixed
	  //state0Local(i1,i2,i3,0) = 3.238579e-05;
        	  state0Local(i1,i2,i3,0) = state0Local(i1,i2,i3,1)*det(i1,i2,i3)*mg.gridSpacing(axis1)*mg.gridSpacing(axis2);

        	  press = U(i1,i2,i3,pc);
        	  eta = 1.0;
        	  getEng( eta,press,eng,a,b,c,d );
        	  state0Local(i1,i2,i3,2) = eng;  // energy at t=0. 
      	}
            }
            else
            {
                  printF("assignHempInitialConditions:ERROR: Unknown hempInitialConditionOption=[%s]\n",
                          (const char*)hempInitialConditionOption);
                  Overture::abort("error");
            }
            
        }
        else
        { // ***** 3D  ****
            printF("assignHempInitialConditions:ERROR: 3d not implemented\n");
            Overture::abort("error");
        }
    } // end for grid

#undef U0

    return 0;
}


