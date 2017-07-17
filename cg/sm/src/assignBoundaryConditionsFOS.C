// This file automatically generated from assignBoundaryConditionsFOS.bC with bpp.
#include "Cgsm.h"
#include "SmParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "RadiationBoundaryCondition.h"
#include "ParallelUtility.h"
#include "GridMaterialProperties.h"

#define bcOptSmFOS EXTERN_C_NAME(bcoptsmfos)

extern "C"
{
    void bcOptSmFOS(const int&nd,
             	       const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
             	       const int& gridIndexRange,real& u, const int&mask,const real&rsxy, const real&xy, 
                              const int & ndMatProp, const int& matIndex, const real& matValpc, const real& matVal,
                              const real&det,
             	       const int&boundaryCondition, const int&addBoundaryForcing, const int& interfaceType, const int&dim,
                              const real & bcf00, const real & bcf10, 
                              const real & bcf01, const real & bcf11, 
                              const real & bcf02, const real & bcf12, 
                              const real & bcf0, const int64_t & bcfOffset,
             	       const int & ndpin, const int & pinbc,
             	       const int & ndpv, const real & pinValues,
                              const int&ipar, const real&rpar, const DataBase *pdb, const int&ierr );
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)


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
// ===================================================================================
//   This macro extracts the boundary data arrays
// ===================================================================================

// ===============================================================================================
// This macro determines the pointers to the variable material properties that are
// used when calling fortran routines.
// ===============================================================================================

// ************* WE SHOULD SHARE THE NEXT MACRO WITH BCOPSM *****************

// =============================================================
// Macro to apply optimized versions of BC's
// =============================================================


// =============================================================
// Macro to apply BC's for special solutions (known solutions)
// =============================================================
// end macro assignSpecialSolutionBoundaryConditions


// =========================================================================================================
/// \brief Apply boundary conditions for the First-Order-System.
///
///  \param option: 
///
// Note: uOld = u[current]
///
// =========================================================================================================
void Cgsm::
assignBoundaryConditionsFOS( int option, int grid, real t, real dt, realMappedGridFunction & u, 
                       			     realMappedGridFunction & uOld, int current )
{

    SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    if( pdeVariation==SmParameters::hemp )
    {
    // *************** for now we do not apply BC's for hemp  ********************
    // For Hemp we should fill in the boundaryData array

        return;
    }
    

    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");

    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");

    const int & u1c = parameters.dbase.get<int >("u1c");
    const int & u2c = parameters.dbase.get<int >("u2c");
    const int & u3c = parameters.dbase.get<int >("u3c");

    const int & v1c =  parameters.dbase.get<int >("v1c");
    const int & v2c =  parameters.dbase.get<int >("v2c");
    const int & v3c =  parameters.dbase.get<int >("v3c");

    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
  // bool & gridHasMaterialInterfaces = parameters.dbase.get<bool>("gridHasMaterialInterfaces");
    int & debug = parameters.dbase.get<int >("debug");

    const bool projectInterface = parameters.dbase.get<bool>("projectInterface");
    if( projectInterface )
    {
        if( debug & 2 )
            printP("*** assignBoundaryConditionsFOS: projectInterface = %i ***\n",projectInterface);
    }

    lambda = lambdaGrid(grid);
    mu = muGrid(grid);
    c1=(mu+lambda)/rho, c2= mu/rho;

  //   const real cc= c*sqrt( kx*kx+ky*ky );

    MappedGrid & mg = *u.getMappedGrid();
    MappedGridOperators & mgop = (*cgop)[grid];
    
    const int numberOfDimensions = mg.numberOfDimensions();
    
  // The RHS for BC's are saved in these next two objects:
    const RealArray & bcData = parameters.dbase.get<RealArray>("bcData");
    BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid);

  // The interfaceType(side,axis,grid) defines which faces are interfaces.
    const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");

    Range all;
    BoundaryConditionParameters bcParams;            
    BoundaryConditionParameters extrapParams;

    Range C=numberOfComponents;

    Range U=Range(uc,uc+numberOfDimensions-1);   // displacements
    Range V=Range(v1c,v1c+numberOfDimensions-1);  // velocities

    bool assignDisplacementBC=false;
    bool assignTractionBC=false;
    bool assignSlipWall=false;
    bool assignDirichletBC=true;
    bool assignSymmetryBC=true;
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        for( int side=0; side<=1; side++ )
        {
            int bc=mg.boundaryCondition(side,axis);
            switch (bc)
            {
            case 0 : break;
            case -1: break;
            case SmParameters::displacementBC:              assignDisplacementBC=true; break;
            case SmParameters::tractionBC:                  assignTractionBC=true; break;
            case SmParameters::slipWall:                    assignSlipWall=true; break;
            case SmParameters::dirichletBoundaryCondition:  assignDirichletBC=true; break;
            case SmParameters::symmetry:                    assignSymmetryBC=true; break;
            default: 
                printf("assignBCFOS: unknown boundary condition =%i on grid %i, side=%i, axis=%i\n",bc,grid,side,axis);
                OV_ABORT("error");
            break;
            }
        }
    }

    if( false && numberOfDimensions == 3 )
    {
        printF("********** assignBoundaryConditionsFOS: DO NOTHING *************\n");
        return;
    }
    

    if( forcingOption==twilightZoneForcing )
    {
    // assign exact values on dirichletBoundaryCondition boundaries for TZ
    // printF(" assignBoundaryConditionsFOS: set exact BC's on dirichletBoundaryCondition's and ghost pts...\n");
        
    // assign the boundary and 2 ghost with the exact solution
        int numGhost=max(orderOfAccuracyInSpace/2,2);
        extrapParams.extraInTangentialDirections=numGhost;
        
        for( int g=0; g<=numGhost; g++ )
        {
            extrapParams.lineToAssign=g;
            u.applyBoundaryCondition(C,BCTypes::dirichlet,SmParameters::dirichletBoundaryCondition,0.,t,extrapParams);
        }
    // reset 
        extrapParams.extraInTangentialDirections=0;
        extrapParams.lineToAssign=1;

    }

//   // assign exact solution at boundaries for 3D code ... FIX ME ...
//   if( false && debug > 3 && numberOfDimensions == 3 )
//   {
//     if( forcingOption==twilightZoneForcing )
//     {
//       // Temporary: assign exact values on boundaries for TZ
//       printF(" assignBoundaryConditionsFOS: set exact BC's on all boundaries and ghost pts...\n");
        
//       // assign the boundary and 2 ghost with the exact solution
//       int numGhost=2;
//       for( int g=0; g<=numGhost; g++ )
//       {
// 	extrapParams.lineToAssign=g;
// 	u.applyBoundaryCondition(C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,extrapParams);
//       }
//       extrapParams.lineToAssign=1;
//     }
//     else
//     {

//       // -- both the displacement and velocities are given on a displacementBC
//       u.applyBoundaryCondition(U,BCTypes::dirichlet,SmParameters::displacementBC,bcData,pBoundaryData,t,
// 			       Overture::defaultBoundaryConditionParameters(),grid);

//       u.applyBoundaryCondition(V,BCTypes::dirichlet,SmParameters::displacementBC,bcData,pBoundaryData,t,
// 			       Overture::defaultBoundaryConditionParameters(),grid);

//       u.applyBoundaryCondition(C,BCTypes::extrapolate,SmParameters::displacementBC,0.,t);
         	   
                      
//       u.applyBoundaryCondition(C,BCTypes::evenSymmetry,SmParameters::symmetry,0.,t);
        
//     }
//     u.finishBoundaryConditions();
//   }
    

  // **** now call the optimized BC routine *****


    const real dtb2=dt*.5;

    const realArray & x = mg.center();

    const bool isRectangular = mg.isRectangular();

  // const bool isRectangular=false; // ********** do this for now for Don ***************

    real dx[3]={0.,0.,0.}; //

    if( isRectangular )
    {
        mg.getDeltaX(dx);
    }
    if( !isRectangular )
    {
        mg.update( MappedGrid::THEinverseVertexDerivative | 
             	       MappedGrid::THEinverseCenterDerivative |
                              MappedGrid::THEcenterJacobian );  
    }

  // Hemp: here is where we store the initial state (mass,density,energy)
    realMappedGridFunction *pstate0 = NULL;
    if( pdeVariation == SmParameters::hemp )
    {
        assert( parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")!=NULL );
        pstate0 = &(*(parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")))[grid];
    }
    realMappedGridFunction & state0 = *pstate0;

    const aString & specialInitialConditionOption = parameters.dbase.get<aString>("specialInitialConditionOption");
    const bool applySpecialSolutionBoundaryConditions = specialInitialConditionOption != "default" ||
                                                                                                            initialConditionOption == knownSolutionInitialCondition;

    const bool centerNeeded = applySpecialSolutionBoundaryConditions;

    #ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray det;  
      if( pdeVariation == SmParameters::hemp ) 
          getLocalArrayWithGhostBoundaries(mg.centerJacobian(),det);
      realSerialArray xLocal; if( centerNeeded ) getLocalArrayWithGhostBoundaries(x,xLocal);
      realSerialArray state0Local;
      if( pdeVariation == SmParameters::hemp )
          getLocalArrayWithGhostBoundaries(state0,state0Local);

    #else
      const realSerialArray & uLocal = u;
      const realSerialArray & xLocal = x;
      const realSerialArray & det = mg.centerJacobian();
      realSerialArray & state0Local = *pstate0;
    #endif

    
    const IntegerArray & bcg = mg.boundaryCondition();
    IntegerArray gid, dim, bc;
    getLocalBoundsAndBoundaryConditions( u, gid, dim, bc );


    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];


  // ---- apply boundary conditions for special solutions ----------
    {
  //   printF("assignBCFOS: applySpecialSolutionBoundaryConditions=%i, t=%8.2e\n",
  //          (int)applySpecialSolutionBoundaryConditions,t);
        if( applySpecialSolutionBoundaryConditions )
        {
            mg.update(MappedGrid::THEvertex | MappedGrid::THEcenter );
            const realArray & x = mg.center();
            const bool isRectangular = mg.isRectangular();
            real dx[3]={0.,0.,0.}; //
            if( isRectangular )
                mg.getDeltaX(dx);
            OV_GET_SERIAL_ARRAY(real,u,uLocal);
            OV_GET_SERIAL_ARRAY_CONST(real,x,xLocal);
            const IntegerArray & bcg = mg.boundaryCondition();
            IntegerArray gid, dim, bc;
            getLocalBoundsAndBoundaryConditions( u, gid, dim, bc );
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];
      //  -- new dirichlet boundary condition : assign the exact solution for testing ---
            for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
            {
                for( int side=Start; side<=End; side++ )
                {
          	if( mg.boundaryCondition(side,axis)==SmParameters::dirichletBoundaryCondition )
          	{
            	  int numberOfGhostLines = max(2,orderOfAccuracyInSpace/2);  // for godunov we need 2 ghost lines
            	  int extra=numberOfGhostLines;
            	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,extra);
  	  // for now assign dirichlet at ghost lines too.
            	  Iv[axis] = side==0 ? Range(Iv[axis].getBase()-numberOfGhostLines,Iv[axis].getBound()) : 
              	    Range(Iv[axis].getBase(),Iv[axis].getBound()+numberOfGhostLines);
            	  if( mg.boundaryCondition(side,axis)==SmParameters::interfaceBoundaryCondition )
            	  { // do not include the boundary
              	    Iv[axis] = side==0 ? Range(Iv[axis].getBase(),Iv[axis].getBound()-1) : 
                	      Range(Iv[axis].getBase()+1,Iv[axis].getBound());
            	  }
            	  const int includeGhost=1;
            	  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            	  if( !ok ) continue;
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
            	  if( specialInitialConditionOption == "travelingWave" )
            	  {
  	    // --- traveling wave solution ---
              	    bool evalSolution = true;
  	    // macro: 
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
                                	  U(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                                	  U(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                                	  U(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                                	  U(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                              	}
                              	else
                              	{
                                	  U(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                                	  U(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                                	  }
                                        if( assignVelocities )
                              	{
                                	  U(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                                	  U(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                              	}
                              	if( assignStress )
                              	{
                                	  U(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                                	  U(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                                	  U(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                                	  U(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                                	  if( pdeVariation == SmParameters::hemp )
                                	  {
                                                real press = -(lambda+2.0*mu/3.0)*div;
                                                U(i1,i2,i3,s11c) -= press;
                                  	    U(i1,i2,i3,s22c) -= press;
                                  	    U(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
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
  	    // --- traveling sine wave solution ---
              	    bool evalSolution = true;
  	    // macro: 
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
                                	  U(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                                	  U(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                                	  U(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                                	  U(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                              	}
                              	else
                              	{
                                	  U(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                                	  U(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                                	  }
                                        if( assignVelocities )
                              	{
                                	  U(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                                	  U(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                              	}
                              	if( assignStress )
                              	{
                                	  U(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                                	  U(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                                	  U(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                                	  U(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
                                	  if( pdeVariation == SmParameters::hemp )
                                	  {
                                                real press = -(lambda+2.0*mu/3.0)*div;
                                                U(i1,i2,i3,s11c) -= press;
                                  	    U(i1,i2,i3,s22c) -= press;
                                  	    U(i1,i2,i3,pc)   = U(i1,i2,i3,pc)   - press;
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
              	    bool evalSolution = true;
  	    // macro: 
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
                              	U(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                              	U(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
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
                                	  U(i1,i2,i3,v1c) = U(i1,i2,i3,v1c) - v1;
                                	  U(i1,i2,i3,v2c) = U(i1,i2,i3,v2c) - v2;
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
                                	  U(i1,i2,i3,s11c) = U(i1,i2,i3,s11c) - p11;
                                	  U(i1,i2,i3,s12c) = U(i1,i2,i3,s12c) - p12;
                                	  U(i1,i2,i3,s21c) = U(i1,i2,i3,s21c) - p21;
                                	  U(i1,i2,i3,s22c) = U(i1,i2,i3,s22c) - p22;
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
            	  else if( specialInitialConditionOption == "RayleighWave" )
            	  {
  	    // --- Rayleigh wave solution ---
              	    bool evalSolution = true;
  	    // macro: 
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
                                	  U(i1,i2,i3,u1c) = U(i1,i2,i3,u1c) - u1;
                                	  U(i1,i2,i3,u2c) = U(i1,i2,i3,u2c) - u2;
                                	  U(i1,i2,i3,uc) = U(i1,i2,i3,uc) - x0;
                                	  U(i1,i2,i3,vc) = U(i1,i2,i3,vc) - y0;
                              	}
                              	else
                              	{
                                	  U(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                                	  U(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                                	  }
                                        if( assignVelocities )
                              	{
                                	  U(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                                	  U(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                              	}
                              	if( assignStress )
                              	{
                                	  U(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                                	  U(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                                	  U(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                                	  U(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
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
              	    bool evalSolution = true;
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
                              	U(i1,i2,i3,uc) =U(i1,i2,i3,uc) - u1;
                              	U(i1,i2,i3,vc) =U(i1,i2,i3,vc) - u2;
                                        if( assignVelocities )
                              	{
                                	  U(i1,i2,i3,v1c) =U(i1,i2,i3,v1c) - v1;
                                	  U(i1,i2,i3,v2c) =U(i1,i2,i3,v2c) - v2;
                              	}
                              	if( assignStress )
                              	{
                                	  U(i1,i2,i3,s11c) =U(i1,i2,i3,s11c) -s11;
                                	  U(i1,i2,i3,s12c) =U(i1,i2,i3,s12c) -s12;
                                	  U(i1,i2,i3,s21c) =U(i1,i2,i3,s21c) -s12;
                                	  U(i1,i2,i3,s22c) =U(i1,i2,i3,s22c) -s22;
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
                        else if( initialConditionOption == knownSolutionInitialCondition )
            	  {
              // Assign dirichlet BC from know solution *wdh* 2014/01/09 
              	    parameters.getUserDefinedKnownSolution(t,cg, grid, uLocal, I1,I2,I3 );
            	  }
            	  else
            	  {
              	    printF("assignBoundaryConditionsFOS:ERROR: unknown specialInitialConditionOption=%s\n",
                                          (const char*)specialInitialConditionOption);
              	    OV_ABORT("error");
            	  }
          	}
                }
            }
        }
    }
    
    bool useOpt=true; 
    int side,axis;


    u.periodicUpdate();

    if( specialInitialConditionOption == "translationAndRotation" ) 
    { // *************** *wdh* 090807 for now do not apply real BC's for this special IC
        return;
    }
      


  // ---------------------------------------------------------
  // ---------- Apply the (optimized) real BC's --------------
  // ---------------------------------------------------------

    getIndex(mg.gridIndexRange(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
    if( ok && useOpt )
    {
    // use optimised boundary conditions
        int ipar[30];
        real rpar[20];
        int gridType = isRectangular ? 0 : 1;
        int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
        int useForcing = forcingOption==twilightZoneForcing;
        int useWhereMask=false;
        realArray f;  // not currently used
        IntegerArray & pinBoundaryCondition = parameters.dbase.get<IntegerArray>("pinBoundaryCondition");
        int numberToPin=pinBoundaryCondition.getLength(1);
        RealArray & pinValues = parameters.dbase.get<RealArray>("pinValues");
    // fprintf(pDebugFile,"**** pu= %i, %i...\n",&u,pu);
        const bool centerNeeded=useForcing || (forcingOption==planeWaveBoundaryForcing); // **************** fix this 
    #ifdef USE_PPP 
        realSerialArray uu;    getLocalArrayWithGhostBoundaries(u,uu);
        realSerialArray uuOld; getLocalArrayWithGhostBoundaries(uOld,uuOld);
        intSerialArray  mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
        realSerialArray rx;    if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rx);
        realSerialArray xy;    if( centerNeeded ) getLocalArrayWithGhostBoundaries(mg.center(),xy);
        realSerialArray jacLocal; if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.centerJacobian(),jacLocal);
        realSerialArray ff;    getLocalArrayWithGhostBoundaries(f,ff); 
        if( debug & 16 )
        {
            fprintf(pDebugFile,"\n **** grid=%i p=%i assignBC: gid=[%i,%i][%i,%i][%i,%i] bc=[%i,%i][%i,%i][%i,%i]"
              	    " bcg=[%i,%i][%i,%i][%i,%i]******\n\n",grid,Communication_Manager::My_Process_Number,
              	    gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
              	    bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
              	    bcg(0,0),bcg(1,0),bcg(0,1),bcg(1,1),bcg(0,2),bcg(1,2));
            fprintf(pDebugFile,"\n **** uu=[%i,%i] xy=[%i,%i] rsxy=[%i,%i]\n",
              	    uu.getBase(0),uu.getBound(0),xy.getBase(0),xy.getBound(0),rx.getBase(0),rx.getBound(0));
        }
    #else
        const realSerialArray & uu    = u;
        const realSerialArray & uuOld = uOld;
        const realSerialArray & ff    = f;
        const intSerialArray  & mask  = mg.mask();
        const realSerialArray & rx = !isRectangular? mg.inverseVertexDerivative() : uu;
        const realSerialArray & xy = centerNeeded ? mg.center() : uu;
        const realSerialArray & jacLocal=mg.centerJacobian();
        const IntegerArray & gid = mg.gridIndexRange();
        const IntegerArray & dim = mg.dimension();
        const IntegerArray & bc = mg.boundaryCondition();
        if( debug & 16 )
        {
            const IntegerArray & bcg = mg.boundaryCondition();
            fprintf(pDebugFile,"\n **** grid=%i p=%i assignBC: gid=[%i,%i][%i,%i][%i,%i] bc=[%i,%i][%i,%i][%i,%i]"
              	    " bcg=[%i,%i][%i,%i][%i,%i]******\n\n",grid,Communication_Manager::My_Process_Number,
              	    gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),
              	    bc(0,0),bc(1,0),bc(0,1),bc(1,1),bc(0,2),bc(1,2),
              	    bcg(0,0),bcg(1,0),bcg(0,1),bcg(1,1),bcg(0,2),bcg(1,2));
        }
    #endif
        real *uptr   = uu.getDataPointer();
        real *fptr   = ff.getDataPointer();
        int *maskptr = mask.getDataPointer();
        real *rxptr  = rx.getDataPointer();
        real *xyptr  = centerNeeded ? xy.getDataPointer() : uptr;
        assert( xyptr!=NULL );
        real *jacptr = !isRectangular ? jacLocal.getDataPointer() : uptr;
    // Do this for now -- assumes all sides are PML
        bool usePML = (bc(0,0)==SmParameters::abcPML || bc(1,0)==SmParameters::abcPML ||
                 		 bc(0,1)==SmParameters::abcPML || bc(1,1)==SmParameters::abcPML ||
                 		 bc(0,2)==SmParameters::abcPML || bc(1,2)==SmParameters::abcPML);
    // *** need to fix gridIndex Range and bc ***********************
        if( debug & 4 )
        {
            ::display(uu,sPrintF("uu before bcOptSolidMechanics, t=%e",t),pDebugFile,"%8.1e ");
        }
        if( !isRectangular && debug & 4  ) ::display(rx,sPrintF("rx before bcOptSolidMechanics, t=%e",t),debugFile,"%9.2e ");
    // The next macro is in boundaryMacros.h
            int pdbc[2*3*2*3];
            #define dbc(s,a,side,axis) (pdbc[(s)+2*((a)+3*((side)+2*(axis)))])
            int pAddBoundaryForcing[6];
            #define addBoundaryForcing(side,axis) (pAddBoundaryForcing[(side)+2*(axis)])
            real *pbcf[2][3];
      // long int pbcfOffset[6];
      // We need an 8 byte integer so we can pass to fortran: int64_t is in stdint.h 
            int64_t pbcfOffset[6];
            #define bcfOffset(side,axis) pbcfOffset[(side)+2*(axis)]
            for( int axis=0; axis<=2; axis++ )
            {
                for( int side=0; side<=1; side++ )
                {
          // *** for now make sure the boundary data array is allocated on all sides
                    if( ( pBoundaryData[side][axis]==NULL || parameters.isAdaptiveGridProblem() ) && 
                            mg.boundaryCondition(side,axis)>0 )
                    {
              	parameters.getBoundaryData(side,axis,grid,mg);
            // RealArray & bd = *pBoundaryData[side][axis]; // this is now done in the above line *wdh* 090819
            // bd=0.;
                    }
                    if( pBoundaryData[side][axis]!=NULL )
                    {
              	if( debug & 8 )
                	  printP("+++ Cgsm: add boundary forcing to (side,axis,grid)=(%i,%i,%i) useConservative=%i\n",side,axis,grid,
                     		 (int)useConservative);
                        addBoundaryForcing(side,axis)=true;
                        RealArray & bd = *pBoundaryData[side][axis];
                        pbcf[side][axis] = bd.getDataPointer();
    	// if( debug & 8 )
            //  ::display(bd," ++++ Cgsm: Here is bd ++++","%4.2f ");
              	for( int a=0; a<=2; a++ )
              	{
                	  dbc(0,a,side,axis)=bd.getBase(a);
                	  dbc(1,a,side,axis)=bd.getBound(a);
              	}
                    }
                    else
                    {
                        addBoundaryForcing(side,axis)=false;
              	pbcf[side][axis] = fptr;  // should not be used in this case 
              	for( int a=0; a<=2; a++ )
              	{
                	  dbc(0,a,side,axis)=0;
                	  dbc(1,a,side,axis)=0;
              	}
                    }
          // for now we save the offset in a 4 byte int (double check that this is ok)
                    int64_t offset = pbcf[side][axis]- pbcf[0][0];
    //       if( offset > INT_MAX )
    //       {
    // 	printF("ERROR: offset=%li INT_MAX=%li \n",offset,(long int)INT_MAX);
    //       }
    //       assert( offset < INT_MAX );
                    bcfOffset(side,axis) = offset;
          // bcfOffset(side,axis) = pbcf[side][axis]- pbcf[0][0];
          // cout << " **** bcfOffset= " << bcfOffset(side,axis) << endl;
                }
            }
    // Macro to extract the pointers to the variable material property arrays
     // --- Variable material properies ---
          GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
          int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
          int *matIndexPtr=maskptr;  // if not used, point to mask
          real*matValPtr=uptr;       // if not used, point to u
          if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
          {
       // Material properties do vary 
              std::vector<GridMaterialProperties> & materialProperties = 
              	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");
              GridMaterialProperties & matProp = materialProperties[grid];
              materialFormat = matProp.getMaterialFormat();
              if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
              {
              	IntegerArray & matIndex = matProp.getMaterialIndexArray();
                  matIndexPtr = matIndex.getDataPointer();
              }
              RealArray & matVal = matProp.getMaterialValuesArray();
              matValPtr = matVal.getDataPointer();
              ndMatProp = matVal.getLength(0);  
       // ::display(matVal,"matVal");
          }
        ipar[0]=numberOfDimensions;
        ipar[1] = grid;
        ipar[2] = uc;
        ipar[3] = vc;
        ipar[4] = wc;
        ipar[5] = gridType;
        ipar[6] = orderOfAccuracyInSpace;
        ipar[7] = orderOfExtrapolation;
        ipar[8] = int(forcingOption==twilightZoneForcing);  // twilightZone *wdh* 090813
        ipar[9] = useWhereMask;
        ipar[10]= debug; 
        ipar[11]=parameters.dbase.get<int >("pdeTypeForGodunovMethod");
        ipar[12]=parameters.dbase.get<int>("applyInterfaceBoundaryConditions");
        ipar[13]=parameters.dbase.get<bool>("projectInterface");
        ipar[14]=numberToPin;
        ipar[15]=(int)materialFormat;
        rpar[ 0]=dx[0];
        rpar[ 1]=dx[1];
        rpar[ 2]=dx[2];
        rpar[ 3]=mg.gridSpacing(0);
        rpar[ 4]=mg.gridSpacing(1);
        rpar[ 5]=mg.gridSpacing(2);
        rpar[ 6]=t;
        OGFunction *& tz = parameters.dbase.get<OGFunction* >("exactSolution");
        rpar[ 7]=(real &)tz;  // twilight zone pointer, ep
        rpar[ 8]=dt;
        rpar[ 9]=mu;
        rpar[10]=lambda;
        rpar[11]=c1;
        rpar[12]=c2;
        int ierr=0;
        const int bc0=-1;  // do all boundaries.
        if( !usePML ) // *** fix this ***
        {
            DataBase *pdb = &parameters.dbase;
            bcOptSmFOS( numberOfDimensions, 
                		uu.getBase(0),uu.getBound(0),
                		uu.getBase(1),uu.getBound(1),
                		uu.getBase(2),uu.getBound(2),
                		gid(0,0), *uptr, *maskptr, *rxptr, *xyptr, 
                                    ndMatProp,*matIndexPtr,*matValPtr,*matValPtr, 
                                    *jacptr, *bc.getDataPointer(),
                		*pAddBoundaryForcing,*interfaceType.getDataPointer(),*pdbc, 
                		*pbcf[0][0],*pbcf[1][0], *pbcf[0][1],*pbcf[1][1], *pbcf[0][2],*pbcf[1][2],
                		*pbcf[0][0],pbcfOffset[0],
                		pinBoundaryCondition.getLength(0),*pinBoundaryCondition.getDataPointer(),
                                    pinValues.getLength(0),*pinValues.getDataPointer(),
                		ipar[0], rpar[0], pdb, ierr );
        }
        if( debug & 4  ) ::display(uu,sPrintF("uu after bcOptSolidMechanics, t=%e",t),pDebugFile,"%8.1e ");
        real *uOldptr = uuOld.getDataPointer();
    } // end use opt


   // *wdh* 090824 -- moved from above 
      if( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") &&
              parameters.dbase.get<int>("useNewExtrapInterpNeighbours") )
      {
     // *new way* 091123 -- MappedGridOperators uses new AssignInterpNeighbours class

     // -- See op/tests/testExtrapInterpNeighbours for proper way to apply --
          if( debug & 4 )
              printF("assignBC-FOS: Use new extrapolateInterpolationNeighbours at t=%g\n",t);

          extrapParams.orderOfExtrapolation=parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours");
          u.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,t,extrapParams);

          if( true )
          {
       // these are both needed:  (see op/src/fixBoundaryCorners.C)
              u.periodicUpdate();
              u.updateGhostBoundaries();
          }
          else if( false )  // *wdh* 2012/09/04 TURN THIS OFF: --  bcOptSmFOS will set 2nd ghost and corners --
          {  
       // extrap 2nd ghost line extended
              extrapParams.ghostLineToAssign=2;
              extrapParams.extraInTangentialDirections=2;

              u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);

       // reset 
              extrapParams.ghostLineToAssign=1;
              extrapParams.extraInTangentialDirections=0;

       // NOTE: We must also call finishBoundaryConditions to fix corners and update ghosts 

       // We really only want to set 2nd ghost line corner points! *********** FIX ME ************************

              u.finishBoundaryConditions();
          }
          
      }
    else if( parameters.dbase.get<int >("extrapolateInterpolationNeighbours") )
    {
    // extrapolate the 2nd ghost line and interpolation neighbours for higher-order dissipation
    // -- is this the right place to do this ? 
        extrapParams.ghostLineToAssign=2;
        extrapParams.orderOfExtrapolation=orderOfAccuracyInSpace+1;

    // extrapParams.orderOfExtrapolation=orderOfAccuracyInSpace;  // *wdh* ***********081111

        u.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,extrapParams);
        extrapParams.ghostLineToAssign=1;
        
    // printF(" extrapolateInterpolationNeighbours at t=%g\n",t);
    // *wdh* 091012 -- add extrapParams
    // extrapParams.orderOfExtrapolation=2;
        u.applyBoundaryCondition(C,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,t,extrapParams);
    }

      if( !parameters.dbase.get<int>("useNewExtrapInterpNeighbours")  )
      {
     // old way 091123
          u.periodicUpdate();
      }
      
  // u.finishBoundaryConditions();
}

