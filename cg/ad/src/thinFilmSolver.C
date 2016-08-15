#include "Cgad.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "AdamsPCData.h"
#include "AdvanceOptions.h"
#include "OGPulseFunction.h"
#include "OGTrigFunction.h"
#include "Oges.h"
#include "PlotStuff.h"
#include "Ogshow.h"
#include "ShowFileReader.h"
#include "SparseRep.h"
#include "time.h"
#include "stdio.h"
#include "gridFunctionNorms.h"

// static real S = 1.*pow(10.,-2); // *note* also defined in AdParameters.C and userDefinedInitialConditions.C
// static real G = 0.0;
#define PI 3.14159

static real dCoupleBC=0.;
static real cDecouple=1.;
//static real cDecouple=0.;
static real flux=1.;
// static real h0= 1.5;
// static real he = 0.;

static real steady=0.;
static real cpressure=-500.;


static real tabserr = pow(10.,-8); //would be nice to pass this parameter
static real trelerr = pow(10.,-10);

static Ogshow *pshow=NULL;

  

#define ForBoundary(side,axis)   for( axis=0; axis<mgNew.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


#define FOR_3(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// Use this for indexing into coefficient matrices representing systems of equations
#define CE(c,e) (stencilSize*((c)+numberOfComponentsForCoefficients*(e)))
#define M123(m1,m2,m3) (m1+halfWidth1+width*(m2+halfWidth2+width*(m3+halfWidth3)))
#define M123CE(m1,m2,m3,c,e) (M123(m1,m2,m3)+CE(c,e))


#define ForStencil(m1,m2,m3)   \
for( m3=-halfWidth3; m3<=halfWidth3; m3++) \
for( m2=-halfWidth2; m2<=halfWidth2; m2++) \
for( m1=-halfWidth1; m1<=halfWidth1; m1++)

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

// =======================================================================
// indexToEquation( n,i1,i2,i3 ) : defines the global index for each unknown in the system
//     n=component number (uc,vc,...)
//    (i1,i2,i3) = grid point
// =======================================================================
#define indexToEquation( n,i1,i2,i3 ) (n+1+ \
numberOfComponentsForCoefficients*(i1-equationNumberBase1+\
equationNumberLength1*(i2-equationNumberBase2+\
equationNumberLength2*(i3-equationNumberBase3))) + equationOffset)

// =======================================================================
// =======================================================================
#define setEquationNumber(m, ni,i1,i2,i3,  nj,j1,j2,j3 )\
equationNumber(m,i1,i2,i3)=indexToEquation( nj,j1,j2,j3)

// =======================================================================
// =======================================================================
#define setClassify(n,i1,i2,i3, type) \
classify(i1,i2,i3,n)=type

// =======================================================================
//  Macro to zero out the matrix coefficients for equations e1,e1+1,..,e2
// =======================================================================
#define zeroMatrixCoefficients( coeff,e1,e2, i1,i2,i3 )\
for( int m=CE(0,e1); m<=CE(0,e2+1)-1; m++ ) \
coeff(m,i1,i2,i3)=0.


// ====================================================================================================
//   This function fills in the coeff equations at the ghost lines by accessing the low-level
//   interface to coefficient matricies.
//
//  The coefficients in the matrix are defined by two arrays:
//
//    coeff(m,i1,i2,i3), m=0,1,2,...    : the coefficients associated with the equation(s) at the grid point (i1,i2,i3)
//                                        For a system of equations,
//                                                m=0,1,...,stencilDim-1             : holds the first eqn. in the system,
//                                                m=stencilDim,1,...,2*stencilDim-1  : holds the second eqn, etc.
//                                        (stencilDim is defined below)
//
//    equationNumber(m,i1,i2,i3)        : a global index that identifies the "unknown" in the system
//                                        (the global index of each unknown is defined by the indexToEquation macro)
//
// NOTES:
//   See cg/ins/src/insImp.h     : for fortran version macros
//       cg/ins/src/insImpINS.bf : for examples
// ====================================================================================================
int
fillInCoeffBoundaryConditions( realCompositeGridFunction & coeffcg, realCompositeGridFunction & newtCur ,
                               Parameters & parameters )
{
    CompositeGrid & cg = *coeffcg.getCompositeGrid();
    const int numberOfDimensions = cg.numberOfDimensions();
    int numberOfComponents = 2;
    
    const int eq1=0, eq2=1;   // equation numbers
    const int uc=0, vc=1;     // component numbers
    
    const real & S  = parameters.dbase.get<real>("inverseCapillaryNumber");
    const real & G  = parameters.dbase.get<real>("scaledStokesNumber");
    const real & h0 = parameters.dbase.get<real>("thinFilmBoundaryThickness");
    const real & he = parameters.dbase.get<real>("thinFilmLidThickness");


    Range all;
    Range c0(0,0), c1(1,1);  // c0 = first component, c1 = second component
    
    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2];
    int i1,i2,i3, j1,j2,j3, i1m,i2m,i3m, m1,m2,m3;
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];
    int side,axis;
    
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mgNew = cg[grid];
        mgNew.update(MappedGrid::THEvertex|MappedGrid::THEmask);
        realArray & vertex = mgNew.vertex();
        intArray & mask = mgNew.mask();
        realMappedGridFunction & coeff = coeffcg[grid];
        MappedGridOperators & mgop = *coeff.getOperators();
        
        assert( coeff.sparse!=NULL );
        
        SparseRepForMGF & sparse = *coeff.sparse;
        int numberOfComponentsForCoefficients = sparse.numberOfComponents;  // size of the system of equations
        int numberOfGhostLines = sparse.numberOfGhostLines;
        int stencilSize = sparse.stencilSize;
        int stencilDim=stencilSize*numberOfComponentsForCoefficients; // number of coefficients per equation
        
        
        const int equationOffset=sparse.equationOffset;
        IntegerArray & equationNumber = sparse.equationNumber;
        IntegerArray & classify = sparse.classify;
        
        const int equationNumberBase1  =equationNumber.getBase(1);
        const int equationNumberLength1=equationNumber.getLength(1);
        const int equationNumberBase2  =equationNumber.getBase(2);
        const int equationNumberLength2=equationNumber.getLength(2);
        const int equationNumberBase3  =equationNumber.getBase(3);
        
        const int orderOfAccuracy=mgop.getOrderOfAccuracy(); assert( orderOfAccuracy==2 );
        
        // stencil width's and half-width's :
        const int width = orderOfAccuracy+1;
        const int halfWidth1 = (width-1)/2;
        const int halfWidth2 = numberOfDimensions>1 ? halfWidth1 : 0;
        const int halfWidth3 = numberOfDimensions>2 ? halfWidth1 : 0;
        
        Range M0 = stencilSize;
        Range M = coeff.dimension(0);
        
        ForBoundary(side,axis)
        {
            if( mgNew.boundaryCondition(side,axis)>0 )
            {
                getBoundaryIndex(mgNew.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
                
                is1=is2=is3=0;
                isv[axis]=1-2*side;
                
                
                // --- Apply the "interior" equation as the boundary condition:
                //         eq1:       u_xx + u_yy -v =
                //         eq2:       v_xx + v_yy +u =   ** do not do this if use a Neumann BC for v **
                // NOTES:
                //     (1) the interior equation approximation is centered on the boundary point (i1,i2,i3)
                //         but is associated with the ghost-point (i1m,i2m,i3m)
                
                
                // const int eqnStart=eq1, eqnEnd=eq2;    // impose interior equation as a BC for both components
                const int eqnStart=eq1, eqnEnd=eq1;       // impose interior equation as a BC for u only (if v.n=gv is given)
                const int eqnStart1=eq2, eqnEnd1=eq2;
                
                // Evaluate the (single component) Laplace operator for points on the boundary
                realSerialArray lapCoeff(M0,Ib1,Ib2,Ib3), xCoeff(M0,Ib1,Ib2,Ib3), yCoeff(M0,Ib1,Ib2,Ib3), idCoeff(M0,Ib1,Ib2,Ib3);
                mgop.assignCoefficients(MappedGridOperators::laplacianOperator,lapCoeff,Ib1,Ib2,Ib3,0,0); //
                mgop.assignCoefficients(MappedGridOperators::xDerivative,xCoeff,Ib1,Ib2,Ib3,0,0);
                mgop.assignCoefficients(MappedGridOperators::yDerivative,yCoeff,Ib1,Ib2,Ib3,0,0);
                mgop.assignCoefficients(MappedGridOperators::identityOperator,idCoeff,Ib1,Ib2,Ib3,0,0);
                
                FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3) // loop over points on the boundary
                {
                    
                    i1m=i1-is1, i2m=i2-is2, i3m=i3-is3; //  ghost point is (i1m,i2m,i3m)
                    
                    for( int e=eqnStart; e<=eqnEnd; e++ ) // equation eq1, eq2, ...
                    {
                        int c=uc;
                        ForStencil(m1,m2,m3)
                        {
                            int m  = M123(m1,m2,m3);        // the single-component coeff-index
                            int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index
                            
                            
                            coeff(mm,i1m,i2m,i3m) = S*lapCoeff(m,i1,i2,i3);
                            
                            // Specify that the above coeff value is the coefficient of component c at the grid point (j1,j2,j3).
                            j1=i1+m1, j2=i2+m2, j3=i3+m3;   // the stencil is centred on the boundary pt (i1,i2,i3)
                            setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
                        }
                        
                        
                        c=vc;
                        ForStencil(m1,m2,m3)
                        {
                            int m  = M123(m1,m2,m3);        // the single-component coeff-index
                            int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index
                            
                            coeff(mm,i1m,i2m,i3m) = idCoeff(m,i1,i2,i3);
                            
                            // Specify that the above coeff value is the coefficient of component c at the grid point (j1,j2,j3).
                            j1=i1+m1, j2=i2+m2, j3=i3+m3;   // the stencil is centred on the boundary pt (i1,i2,i3)
                            setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
                        }
                        
                        // Specify that this a "real" equation on the first ghost line:
                        // (A "real" equation has a possible non-zero right-hand-side)
                        setClassify(e,i1m,i2m,i3m, SparseRepForMGF::ghost1);
                    }
                    
                    
                    //  Fill in boundary point
                    i1m=i1, i2m=i2, i3m=i3; //  boundary point is (i1m,i2m,i3m)
                    
                    for( int e=eqnStart1; e<=eqnEnd1; e++ ) // equation eq1, eq2, ...
                    {
                        int c=uc;
                        ForStencil(m1,m2,m3)
                        {
                            int m  = M123(m1,m2,m3);        // the single-component coeff-index
                            int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index
                            
                            
                            coeff(mm,i1m,i2m,i3m) = -0.5*newtCur[grid](i1,i2,i3,0)*(newtCur[grid].x()(i1,i2,i3,0)*newtCur[grid].x()(i1,i2,i3,1) + newtCur[grid].y()(i1,i2,i3,0)*(newtCur[grid].y()(i1,i2,i3,1)+G))*idCoeff(m,i1,i2,i3)
                            -0.25*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*newtCur[grid].x()(i1,i2,i3,1)*xCoeff(m,i1,i2,i3)
                            -0.25*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*(newtCur[grid].y()(i1,i2,i3,1)+G)*yCoeff(m,i1,i2,i3)
                            -0.25*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*newtCur[grid].laplacian()(i1,i2,i3,1)*idCoeff(m,i1,i2,i3);
                            
                            
                            //coeff(mm,i1m,i2m,i3m) = 0; //Linear PDE
                            

                            // Specify that the above coeff value is the coefficient of component c at the grid point (j1,j2,j3).
                            j1=i1+m1, j2=i2+m2, j3=i3+m3;   // the stencil is centred on the boundary pt (i1,i2,i3)
                            setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
                        }
                        
                        
                        c=vc;
                        ForStencil(m1,m2,m3)
                        {
                            int m  = M123(m1,m2,m3);        // the single-component coeff-index
                            int mm = M123CE(m1,m2,m3,c,e);  // the system coeff-index
                            
                            
                            coeff(mm,i1m,i2m,i3m) = -0.25*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*newtCur[grid].x()(i1,i2,i3,0)*xCoeff(m,i1,i2,i3)
                            -0.25*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*newtCur[grid].y()(i1,i2,i3,0)*yCoeff(m,i1,i2,i3)
                            -(1./12.)*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*newtCur[grid](i1,i2,i3,0)*lapCoeff(m,i1,i2,i3);
            
                            
                            //coeff(mm,i1m,i2m,i3m) = -lapCoeff(m,i1,i2,i3); //Linear PDE
                            

                            // Specify that the above coeff value is the coefficient of component c at the grid point (j1,j2,j3).
                            j1=i1+m1, j2=i2+m2, j3=i3+m3;   // the stencil is centred on the boundary pt (i1,i2,i3)
                            setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 );  // macro to set equationNumber
                        }
                        
                        // Specify that this a "real" equation on the first ghost line:
                        // (A "real" equation has a possible non-zero right-hand-side)
                        setClassify(e,i1m,i2m,i3m, SparseRepForMGF::boundary);
                        
                    }
                } // end FOR_3D
                
                
                // -- At corners between two physical boundaries we cannot apply the interior equation on the points
                //    marked A below since the same equation (from the corner) would appear twice in the matrix.
                //
                //                |    |    |
                //            G-  X----I----I--       I=interior point
                //                |    |    |         X=boundary point
                //                |    |    |         G=ghost point
                //            A-- X----X----X--       A=ghost point on an extended boundary 
                //                |    |    | 
                //            V   A    G    G 
                
                // INSTEAD we will extrapolate the values at the points "A"
                
                const int orderOfExtrap=3;
                real extrapCoeff[orderOfExtrap+1] = {1.,-3.,3.,-1.}; // extrapolation coefficients 
                
                const int ghost=1;  // ghost point to fill in 
                const int axisp = (axis+1) % numberOfDimensions;       // tangential direction
                
                assert( numberOfDimensions==2 );                        // -- finish me for 3D
                for( int side2=0; side2<=1; side2++ )                  // in 2D there are two adjacent boundaries, left and right 
                {
                    if( mgNew.boundaryCondition(side2,axisp)>0 ) // adjacent boundary is a physical boundary
                    {
                        Ibv[axisp] = mgNew.gridIndexRange(side2,axisp);  // set loop bounds to the "left-side" or "right-side"
                        
                        FOR_3D(i1,i2,i3,Ib1,Ib2,Ib3) // loop over points (in 2D there will just be point to assign)
                        {
                            i1m=i1-is1*(ghost); // ghost point
                            i2m=i2-is2*(ghost);
                            i3m=i3-is3*(ghost);
                            
                            for( int e=eqnStart; e<=eqnEnd; e++ ) // equation eq1, eq2, ...
                            {
                                // fill in the extrapolation equations
                                int c=e;   // extrapolate this component of the solution 
                                
                                // first zero all coefficients for this equation: 
                                zeroMatrixCoefficients( coeff,e,e, i1m,i2m,i3m ); 
                                
                                // For extrapolation we just fill in the first orderOfExtrap+1 values into the coeff matrix.
                                // We do NOT think of the values as arranged in a stencil as we did in the above case. 
                                for( int m=0; m<=orderOfExtrap; m++ )
                                {
                                    j1=i1m+is1*m; //  m-th point moving inward from the ghost point (i1,i2,i3)
                                    j2=i2m+is2*m;
                                    j3=i3m+is3*m;
                                    
                                    int mm = CE(c,e)+m;
                                    
                                    coeff(mm,i1m,i2m,i3m)=extrapCoeff[m];  //  m=0,1,2,..
                                    
                                    setEquationNumber(mm, e,i1m,i2m,i3m,  c,j1,j2,j3 ); // set the global equation number 
                                }
                                // classify this equation as extrapolation (will always have a zero right-hand-side)
                                setClassify(e,i1m,i2m,i3m, SparseRepForMGF::extrapolation);
                            }
                        }
                    }
                }
                
                
            } // end if( mg.boundaryCondition(side,axis)>0 )
        } // end ForBoundary 
        
        
    }
    
    return 0;
}





// ==========================================================================================
// Evaluate the thin film equations
// ==========================================================================================
int evaluateThinFilmFunction( realCompositeGridFunction & rhs,
                              GridFunction *gf, realCompositeGridFunction & newtCur,
                              int mNew, int mCur, int mOld, real t0, real dt0, Parameters & parameters )
{
  OGFunction & exact = *(parameters.dbase.get<OGFunction* >("exactSolution"));

  const real & S  = parameters.dbase.get<real>("inverseCapillaryNumber");
  const real & G  = parameters.dbase.get<real>("scaledStokesNumber");
  const real & h0 = parameters.dbase.get<real>("thinFilmBoundaryThickness");
  const real & he = parameters.dbase.get<real>("thinFilmLidThickness");

  Index I1,I2,I3;
  Index Ib1,Ib2,Ib3;
  Index Ig1,Ig2,Ig3;
  int side,axis;
  
  const real & tInitial = parameters.dbase.get<real>("tInitial");
  const bool useBDF2 = (t0-tInitial) > dt0;
  
  // printF("---- evaluateThinFilmFunction: tInitial=%12.4e\n",tInitial);

  rhs = 0.;
        
  // ------------ Calculate F ---------------------
  for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
  {

    realMappedGridFunction & uCur = gf[mCur].u[grid];
    realMappedGridFunction & uOld = gf[mOld].u[grid];
    realMappedGridFunction & gridVelocity = gf[mNew].getGridVelocity(grid);
    // gridVelocity.display("Grid velocity"); // debugging
    MappedGrid & mgNew = gf[mNew].cg[grid];

    const int numberOfDimensions = mgNew.numberOfDimensions();
         
    getIndex(mgNew.indexRange(),I1,I2,I3);
            
    OV_GET_SERIAL_ARRAY_CONST(real,newtCur[grid],uLocal);       
    Range N=2;
    RealArray ux(I1,I2,I3,N), uy(I1,I2,I3,N), uLap(I1,I2,I3,N); 
    MappedGridOperators & op = *(newtCur[grid].getOperators());  // opertaors at *new* time
    op.derivative(MappedGridOperators::xDerivative,uLocal,ux  ,I1,I2,I3,N);
    op.derivative(MappedGridOperators::yDerivative,uLocal,uy  ,I1,I2,I3,N);
    op.derivative(MappedGridOperators::laplacianOperator,uLocal,uLap  ,I1,I2,I3,N);
    //ux.display("This is ux."); //debugging
    //uy.display("This is uy."); //debugging
      
    OV_GET_SERIAL_ARRAY_CONST(real,mgNew.center(),xLocal);
    RealArray ue(I1,I2,I3,N), uet(I1,I2,I3,N), uex(I1,I2,I3,N), uey(I1,I2,I3,N), uexx(I1,I2,I3,N), ueyy(I1,I2,I3,N); // is there a way to build laplacian?
    const bool isRectangular = false; // do this for now
    const real tNew = t0+dt0;

    const bool twilightZoneFlow= parameters.dbase.get<bool >("twilightZoneFlow");
    
    if( twilightZoneFlow )
    {
      exact.gd( ue  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,N,tNew);
      exact.gd( uet ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,N,tNew);
      exact.gd( uex ,xLocal,numberOfDimensions,isRectangular,0,1,0,0,I1,I2,I3,N,tNew);
      exact.gd( uey ,xLocal,numberOfDimensions,isRectangular,0,0,1,0,I1,I2,I3,N,tNew);
      exact.gd( uexx ,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,N,tNew);
      exact.gd( ueyy ,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,N,tNew);
    }

    const bool manufacturedTearFilm = parameters.dbase.get<bool >("manufacturedTearFilm");
    if( manufacturedTearFilm )
    {
      assert( !twilightZoneFlow );
      
      // Formula : H0 * exp(-beta*(y-yl)) + Hm
      //         H0 = height at lower lid, y=yl
      //         Hm = height at the centre of the eye

      // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
      if( !parameters.dbase.get<DataBase >("modelData").has_key("CgadUserDefinedInitialConditionData") )
      {
        printF("evaluateThinFilmFunction:ERROR: sub-directory `CgadUserDefinedInitialConditionData' not found!\n");
        Overture::abort("error");
      }
      DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");

      RealArray & tearFilmParameters = db.get<RealArray>("tearFilmParameters");
      const real & H0   = tearFilmParameters(0);
      const real & beta = tearFilmParameters(1);
      const real & yl   = tearFilmParameters(2);
      const real & Hm   = tearFilmParameters(3);
      
      RealArray temp(I1,I2,I3);
      temp=H0*exp(-beta*(xLocal(I1,I2,I3,1)-yl));

      ue(I1,I2,I3,0)= temp + Hm;    
      ue(I1,I2,I3,1)= (-S*beta*beta)*temp;

      uey(I1,I2,I3,0)= (-beta)*temp;
      uey(I1,I2,I3,1)= (-beta)*ue(I1,I2,I3,1);
      
      ueyy(I1,I2,I3,0)= (beta*beta)*temp;
      ueyy(I1,I2,I3,1)= (beta*beta)*ue(I1,I2,I3,1);
      

      uet(I1,I2,I3,N)=0.;
      uex(I1,I2,I3,N)=0.;
      uexx(I1,I2,I3,N)=0.;
      

    }
    
    
      
    //We first build the right hand side where F(uNew)for backward Euler
    if(parameters.gridIsMoving(grid)){
      printF("**********************************************************************\nDEBUGGING GRID IS MOVING \n**********************************************************************\n");
      //where(mg.mask()(I1,I2,I3)>0){
          if( true )
          { // *new*
              
               //Thin Film Equation
              if(steady==1.0)
              {
                  rhs[grid](I1,I2,I3,0) =  -S*uLap(I1,I2,I3,0)-cpressure;
              }
              else
              {
                if( useBDF2 ) // BDF2
                {
                  rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0) - (4./3)*uCur(I1,I2,I3,0) + (1./3)*uOld(I1, I2, I3, 0) - (2./3)*dt0*( ux(I1,I2,I3,0)*gridVelocity(I1,I2,I3,0) + uy(I1,I2,I3,0)*gridVelocity(I1,I2,I3,1)
                                    +  (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*ux(I1,I2,I3,0)*ux(I1,I2,I3,1)
                                    + (1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uLap(I1,I2,I3,1)
                                    + ( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uy(I1,I2,I3,0)*(uy(I1,I2,I3,1) + G)
                           ) //cDecouple
                                                                                        ); //dt0
                }
                  else // backward Euler for first two time steps
                  {
                      rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)- uCur(I1,I2,I3,0) - dt0*( ux(I1,I2,I3,0)*gridVelocity(I1,I2,I3,0) + uy(I1,I2,I3,0)*gridVelocity(I1,I2,I3,1)
                                             +  (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*ux(I1,I2,I3,0)*ux(I1,I2,I3,1)
                                             + (1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uLap(I1,I2,I3,1)
                                             + ( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uy(I1,I2,I3,0)*(uy(I1,I2,I3,1) + G)
                                                                                                                                               ) //cDecouple
                                                                                                                                            ); //dt0
                  }
              }
              
              /* //Linear PDE
              rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) - dt0*(ux(I1,I2,I3,0)*gridVelocity(I1,I2,I3,0) + uy(I1,I2,I3,0)*gridVelocity(I1,I2,I3,1) + uLap(I1,I2,I3,1));
               */
              rhs[grid](I1,I2,I3,1) = newtCur[grid](I1,I2,I3,1)+S*uLap(I1,I2,I3,0);
              
              if( parameters.dbase.get<bool >("twilightZoneFlow") )
              {
                  if(steady==1.0)
                  {
                      rhs[grid](I1,I2,I3,0) -=(-S*(uexx(I1,I2,I3,0)+ueyy(I1,I2,I3,0))-cpressure);
                  }
                  else
                  {
		      if( useBDF2 ) //BDF2
                      {
                          rhs[grid](I1,I2,I3,0) -=
                          (2./3)*dt0*( uet(I1,I2,I3,0)
                               -(1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uex(I1,I2,I3,0)*uex(I1,I2,I3,1)
                               -(1./12)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*(uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1))
                               - ((1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uey(I1,I2,I3,0)*(uey(I1,I2,I3,1)+G)
                                  )//cDecouple
                               );//dt0
                      }
                      else{
                          rhs[grid](I1,I2,I3,0) -=
                          dt0*( uet(I1,I2,I3,0)
                               -(1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uex(I1,I2,I3,0)*uex(I1,I2,I3,1)
                               -(1./12)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*(uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1))
                               - ((1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uey(I1,I2,I3,0)*(uey(I1,I2,I3,1)+G)
                                  )//cDecouple
                               );//dt0
                      }
                      
                  }
                  
                  /* //Linear PDE
                  rhs[grid](I1,I2,I3,0) -= dt0*( uet(I1,I2,I3,0)-(uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1)));
                   */
                  rhs[grid](I1,I2,I3,1) -= ue(I1,I2,I3,1) + S*(uexx(I1,I2,I3,0)+ueyy(I1,I2,I3,0));
              }
          }
      
      //}//end of where

      ForBoundary(side,axis){
          if(mgNew.boundaryCondition()(side,axis)>0){
              getGhostIndex(mgNew.indexRange(),side,axis,Ib1,Ib2,Ib3,0);    //changed from gridIndexRange
              
              printF("flux = %3.5e and steady = %3.5e\n",flux,steady);
              
              if(( flux!=0) && (steady!=1.) )
              {
                  getGhostIndex(mgNew.indexRange(),side,axis,Ig1,Ig2,Ig3,1);
                  const realSerialArray & normal  = mgNew.vertexBoundaryNormal(side,axis);
                  
                  //mgNew.vertex().display();
                  
                  printF("We are applying flux boundary conditions \n");

                  rhs[grid](Ib1,Ib2,Ib3,1) = -( 0.25*(newtCur[grid](Ib1,Ib2,Ib3,0))*(newtCur[grid](Ib1,Ib2,Ib3,0))*( (ux(Ib1,Ib2,Ib3,0))*(ux(Ib1,Ib2,Ib3,1)) + (uy(Ib1,Ib2,Ib3,0))*(uy(Ib1,Ib2,Ib3,1) + G) )
                      + (1./12.)*(newtCur[grid](Ib1,Ib2,Ib3,0))*(newtCur[grid](Ib1,Ib2,Ib3,0))*(newtCur[grid](Ib1,Ib2,Ib3,0))*(uLap(Ib1,Ib2,Ib3,1))
                                                    );//end of negative
                  
                  /*
                  // Linear PDE
                  rhs[grid](Ib1,Ib2,Ib3,1) = -( (uLap(Ib1,Ib2,Ib3,1)) );//end of negative
                  
                  //rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,0) - ue(Ib1,Ib2,Ib3,0);
                  */
                  rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0);
                  
                  rhs[grid](Ig1,Ig2,Ig3,0) = newtCur[grid](Ib1,Ib2,Ib3,1) + S*(uLap(Ib1,Ib2,Ib3,0));
                  
                  //normal.display();
                  
                  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                  {
                      
                        rhs[grid](Ib1,Ib2,Ib3,0) -= ue(Ib1,Ib2,Ib3,0);
                        rhs[grid](Ig1,Ig2,Ig3,1) = normal(Ib1,Ib2,Ib3,0)*(ux(Ib1,Ib2,Ib3,1)) + normal(Ib1,Ib2,Ib3,1)*(uy(Ib1,Ib2,Ib3,1));
                      
                        rhs[grid](Ib1,Ib2,Ib3,1) -=
                                    - 0.25*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*( (uex(Ib1,Ib2,Ib3,0))*(uex(Ib1,Ib2,Ib3,1)) + (uey(Ib1,Ib2,Ib3,0))*((uey(Ib1,Ib2,Ib3,1))+G) ) - (
                                       (1./12.)*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*(uexx(Ib1,Ib2,Ib3,1)+ueyy(Ib1,Ib2,Ib3,1)));
                      
                        /*
                      
                        //Linear PDE
                      
                        rhs[grid](Ib1,Ib2,Ib3,1) -= //- uex(Ib1,Ib2,Ib3,0)*gridVelocity(Ib1,Ib2,Ib3,0) - uey(Ib1,Ib2,Ib3,0)*gridVelocity(Ib1,Ib2,Ib3,1)
                                                   - (uexx(Ib1,Ib2,Ib3,1)+ueyy(Ib1,Ib2,Ib3,1));
                      */
                        rhs[grid](Ig1,Ig2,Ig3,0) -= ue(Ib1,Ib2,Ib3,1) + S*(uexx(Ib1,Ib2,Ib3,0)+ueyy(Ib1,Ib2,Ib3,0));
                        rhs[grid](Ig1,Ig2,Ig3,1) -= ( normal(Ib1,Ib2,Ib3,0)*(uex(Ib1,Ib2,Ib3,1)) + normal(Ib1,Ib2,Ib3,1)*(uey(Ib1,Ib2,Ib3,1)) );
                      
                  }
                  else
                  {
                      rhs[grid](Ib1,Ib2,Ib3,0) -= h0;
                      rhs[grid](Ig1,Ig2,Ig3,1) = normal(Ib1,Ib2,Ib3,0)*(((h0*h0*h0)/12.)*ux(Ib1,Ib2,Ib3,1) + (h0-he)*gridVelocity(Ib1,Ib2,Ib3,0)  )
                      + normal(Ib1,Ib2,Ib3,1)*( ( (h0*h0*h0)/12.)*(uy(Ib1,Ib2,Ib3,1)+ G) + (h0-he)*gridVelocity(Ib1,Ib2,Ib3,1)); // Boundary conditions for ghost points of second equation.
                  }
              }
              else
              {
                  printF("We are applying dirichlet boundary conditions \n");
                  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                  {
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - ue(Ib1,Ib2,Ib3,0);
                      rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - ue(Ib1,Ib2,Ib3,1);
                  }
                  else{
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - h0;
                      rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - cpressure;
                  }
                  
              }
          }
      }//end of ForBoundary
                
    }else
    {
      //where(mg.mask()(I1,I2,I3)>0){
                
          if( true )
          {
              // *new* way
              
              if(steady==1.0)
              {
                  rhs[grid](I1,I2,I3,0) =  -S*uLap(I1,I2,I3,0)-cpressure;
              }
              else
              {
		  if( useBDF2 ) // BDF2
                  {
                      rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)- (4./3)*uCur(I1,I2,I3,0) + (1./3)*uOld(I1, I2, I3, 0) - (2./3)*dt0*(
                                            (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*ux(I1,I2,I3,0)*ux(I1,I2,I3,1)
                                            + (1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uLap(I1,I2,I3,1)
                                            + ( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uy(I1,I2,I3,0)*(uy(I1,I2,I3,1) + G)) //cDecouple
                                            ); //dt0
                  }
                  else // backward Euler for first two time steps
                  {
                      rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)- uCur(I1,I2,I3,0) - dt0*(
                                            (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*ux(I1,I2,I3,0)*ux(I1,I2,I3,1)
                                            + (1./12)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uLap(I1,I2,I3,1)
                                            + ( (1./4)*newtCur[grid](I1,I2,I3,0)*newtCur[grid](I1,I2,I3,0)*uy(I1,I2,I3,0)*(uy(I1,I2,I3,1) + G)) //cDecouple
                                            ); //dt0
                  }
              }
              
              /*
              //Linear PDE
              rhs[grid](I1,I2,I3,0) = newtCur[grid](I1,I2,I3,0)-uCur(I1,I2,I3,0) -  dt0*(uLap(I1,I2,I3,1));
               */
              rhs[grid](I1,I2,I3,1) = newtCur[grid](I1,I2,I3,1)+S*uLap(I1,I2,I3,0);
              
              if( parameters.dbase.get<bool >("twilightZoneFlow") )
              {
                  
                  if(steady==1.0)
                  {
                      rhs[grid](I1,I2,I3,0) -=(-S*(uexx(I1,I2,I3,0)+ueyy(I1,I2,I3,0))-cpressure);
                  }
                  else
                  {
                      if( useBDF2 ) //BDF2
                      {
                          rhs[grid](I1,I2,I3,0) -=  (2./3)*dt0*( uet(I1,I2,I3,0) -
                            (1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uex(I1,I2,I3,0)*uex(I1,I2,I3,1) - (1./12)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*(uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1)) - ( (1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uey(I1,I2,I3,0)*(uey(I1,I2,I3,1) + G )));
                      }
                      else{
                      rhs[grid](I1,I2,I3,0) -=  dt0*( uet(I1,I2,I3,0) -
					    (1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uex(I1,I2,I3,0)*uex(I1,I2,I3,1) - (1./12)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*(uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1)) - ( (1./4)*ue(I1,I2,I3,0)*ue(I1,I2,I3,0)*uey(I1,I2,I3,0)*(uey(I1,I2,I3,1) + G )));
                      }
                  }
                  /*
                  //Linear PDE
                  rhs[grid](I1,I2,I3,0) -=  dt0*( uet(I1,I2,I3,0) -   (uexx(I1,I2,I3,1) + ueyy(I1,I2,I3,1)));
                   */
                                                 
                  rhs[grid](I1,I2,I3,1) -= ue(I1,I2,I3,1) + S*(uexx(I1,I2,I3,0)+ueyy(I1,I2,I3,0));
              }
          }
      
      //}//end of where
                
      ForBoundary(side,axis){
          if(mgNew.boundaryCondition()(side,axis)>0){
              getGhostIndex(mgNew.indexRange(),side,axis,Ib1,Ib2,Ib3,0); //changed from gridIndexRange
              
              if((flux!=0)&&(steady!=1.))
              {
                  getGhostIndex(mgNew.indexRange(),side,axis,Ig1,Ig2,Ig3,1);
                  const realSerialArray & normal  = mgNew.vertexBoundaryNormal(side,axis);
                  
                  printF("We are applying flux boundary conditions \n");
                  

                  rhs[grid](Ib1,Ib2,Ib3,1) =  -( 0.25*(uLocal(Ib1,Ib2,Ib3,0))*(uLocal(Ib1,Ib2,Ib3,0))*( (ux(Ib1,Ib2,Ib3,0))*(ux(Ib1,Ib2,Ib3,1)) + (uy(Ib1,Ib2,Ib3,0))*(uy(Ib1,Ib2,Ib3,1) + G) )
                    + (1./12.)*(uLocal(Ib1,Ib2,Ib3,0))*(uLocal(Ib1,Ib2,Ib3,0))*(uLocal(Ib1,Ib2,Ib3,0))*(uLap(Ib1,Ib2,Ib3,1))
                    );//end of negative

                  
                  /*
                  //Linear PDE
                  rhs[grid](Ib1,Ib2,Ib3,1) =  -( (uLap(Ib1,Ib2,Ib3,1))
                                                );//end of negative
                  
                  //rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - ue(Ib1,Ib2,Ib3,1);
                  */
                  
                  rhs[grid](Ig1,Ig2,Ig3,0) = newtCur[grid](Ib1,Ib2,Ib3,1) + S*(uLap(Ib1,Ib2,Ib3,0));

                  
                  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                  {
                      printF("**********************************************************************\nHELLO DEBUGGING TWILIGHT ZONE\n**********************************************************************\n");
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - ue(Ib1,Ib2,Ib3,0);
                      rhs[grid](Ig1,Ig2,Ig3,1) = normal(Ib1,Ib2,Ib3,0)*(ux(Ib1,Ib2,Ib3,1)) + normal(Ib1,Ib2,Ib3,1)*(uy(Ib1,Ib2,Ib3,1));
                      
                      
                      rhs[grid](Ib1,Ib2,Ib3,1) -=
                      - 0.25*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*(uex(Ib1,Ib2,Ib3,0))*(uex(Ib1,Ib2,Ib3,1) + (uey(Ib1,Ib2,Ib3,0))*((uey(Ib1,Ib2,Ib3,1))+G)  )
                                                 - ( (1./12.)*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*(ue(Ib1,Ib2,Ib3,0))*(uexx(Ib1,Ib2,Ib3,1)+ueyy(Ib1,Ib2,Ib3,1)));
                      
                      /*
                      //Linear PDE
                      rhs[grid](Ib1,Ib2,Ib3,1) -= - (uexx(Ib1,Ib2,Ib3,1)+ueyy(Ib1,Ib2,Ib3,1));
                      */
                      
                      rhs[grid](Ig1,Ig2,Ig3,0) -= ue(Ib1,Ib2,Ib3,1) + S*(uexx(Ib1,Ib2,Ib3,0)+ueyy(Ib1,Ib2,Ib3,0));
                      rhs[grid](Ig1,Ig2,Ig3,1) -= ( normal(Ib1,Ib2,Ib3,0)*(uex(Ib1,Ib2,Ib3,1)) + normal(Ib1,Ib2,Ib3,1)*(uey(Ib1,Ib2,Ib3,1)) );
                  }
                  else
                  {
                      
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - h0;
                      rhs[grid](Ig1,Ig2,Ig3,1) = normal(Ib1,Ib2,Ib3,0)*(ux(Ib1,Ib2,Ib3,1)) + normal(Ib1,Ib2,Ib3,1)*(uy(Ib1,Ib2,Ib3,1));
                      printF("**********************************************************************\nHELLO DEBUGGING\n**********************************************************************\n");
                  }
              }
              else
              {
                 printF("We are applying dirichlet boundary conditions \n");
                  if( parameters.dbase.get<bool >("twilightZoneFlow") )
                  {
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - ue(Ib1,Ib2,Ib3,0);
                      rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - ue(Ib1,Ib2,Ib3,1);
                  }
                  else{
                      rhs[grid](Ib1,Ib2,Ib3,0) = newtCur[grid](Ib1,Ib2,Ib3,0) - h0;
                      rhs[grid](Ib1,Ib2,Ib3,1) = newtCur[grid](Ib1,Ib2,Ib3,1) - cpressure;
                  }

              }
          } //end of if(mgNew.boundaryConditions)
      }//end of ForBoundary
    }//end of else for grid moving
  }//end of for grid


  return 0;
  
}


// ===================================================================================================================
/// \brief Advance the THIN FILM EQUATIONS one time step for the BDF scheme.
///
///   **THIS VERSION IS A PLACEHOLDER FOR THE REAL EQUATIONS ***
/// 
/// \details This routine is called by the implicitTimeStep routine which is in turn called by
/// the BDF takeTimeStep routine which handles details of moving and adaptive grids.
/// 
/// \param t0 (input) : current time
/// \param dt0 (input) : current time step
///
// ===================================================================================================================
int Cgad::
thinFilmSolver(  real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions )
{
  
    
    const real & S  = parameters.dbase.get<real>("inverseCapillaryNumber");
    const real & G  = parameters.dbase.get<real>("scaledStokesNumber");
    const real & h0 = parameters.dbase.get<real>("thinFilmBoundaryThickness");
    const real & he = parameters.dbase.get<real>("thinFilmLidThickness");

    if( t0<2*dt0 ){
      printF("--AD-- thinFilmSolver S=%g, G=%g, h0=%g, he=%g, t=%9.3e\n",S,G,h0,he,t0);
    }

    /* It appears as though the parameter orderOfBDF tells cgad how many time levels the solver needs. */
    const int & orderOfBDF = parameters.dbase.get<int>("orderOfBDF");

    const real & tInitial = parameters.dbase.get<real>("tInitial");
    const bool useBDF2 = (t0-tInitial) > dt0;
    
    /* It appears as though the structure adamsData tells thinFilmSolver.C where the current time step is located in the array. */
    assert( parameters.dbase.get<int >("orderOfPredictorCorrector")==2 );  // for now we just have 2nd-order in time

    assert( parameters.dbase.get<DataBase >("modelData").has_key("AdamsPCData") );
    AdamsPCData & adamsData = parameters.dbase.get<DataBase >("modelData").get<AdamsPCData>("AdamsPCData");
  
    real & dtb=adamsData.dtb;
    int &mab0 =adamsData.mab0, &mab1=adamsData.mab1, &mab2=adamsData.mab2;
    int &nab0 =adamsData.nab0, &nab1=adamsData.nab1, &nab2=adamsData.nab2, &nab3=adamsData.nab3;
    int &ndt0=adamsData.ndt0;
    real *dtp = adamsData.dtp;
    

    /* This piece finds the index of the solution at older times */
    const int numberOfGridFunctions =  orderOfBDF+1; // movingGridProblem() ? 3 : 2;
    int & mCur = mab0;                                                              // holds u(t)
    const int mNew   = (mCur + 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t+dt)
    const int mOld   = (mCur - 1 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-dt)
    const int mOlder = (mCur - 2 + numberOfGridFunctions ) % numberOfGridFunctions; // holds u(t-2*dt)
    const int mMinus3= (mCur - 3 + numberOfGridFunctions*2) % numberOfGridFunctions; // holds u(t-3*dt)
    printF("****numberofGridFunctions = %i   mCur = %i   mNew = %i,   mOld = %i,    mOlder = %i,    mMinus3 = %i ****\n", numberOfGridFunctions, mCur, mNew, mOld, mOlder, mMinus3);

    
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Ib1,Ib2,Ib3,Ig1,Ig2,Ig3,D1,D2,D3;
    Range all;
    int side,axis;
    Range e0(0,0), e1(1,1);  // e0 = first equation, e1 = second equation
    Range c0(0,0), c1(1,1);  // c0 = first component, c1 = second component
    
    gf[mNew].cg.update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEvertexBoundaryNormal );
    
    //  -----------------------------------------------
    //  --- Build exact solution ----------------------
    //  ----------Can I pass the exact solution?-------
    //  -----------------------------------------------
    

    DataBase & db = parameters.dbase.get<DataBase >("modelData");
    
    const int numberOfComponents = parameters.dbase.get<int >("numberOfComponents")-parameters.dbase.get<int >("numberOfExtraVariables");

    assert( numberOfComponents==2 );
    
    OGFunction & exact = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    //  ----------------------------------------------------------------------------------
    //  --- Build a differential operator on the new grid --------------------------------
    //  ----- Do I have to do this?  Since I can take derivatives of uNew and uCur, ------
    //  ----- is there a differential operator create elsewhere?  How do I access it? ----
    //  ----------------------------------------------------------------------------------
    
// >> start new: 

    if( !db.has_key("coeff") )
    {
       db.put<realCompositeGridFunction>("coeff");
    }

    realCompositeGridFunction & coeff = db.get<realCompositeGridFunction>("coeff");

    int orderOfAccuracy=2;  //how do I pass this information?
    const int width = orderOfAccuracy+1;
    int stencilSize=int(pow(width,gf[mNew].cg.numberOfDimensions())+1);  // add 1 for interpolation equations
    int stencilDimension = stencilSize*SQR(numberOfComponents);

    coeff.updateToMatchGrid(gf[mNew].cg,stencilDimension,all,all,all);

    const int numberOfGhostLines=(orderOfAccuracy)/2;
    int includeGhost=numberOfGhostLines;
    coeff.setIsACoefficientMatrix(TRUE,stencilSize,numberOfGhostLines,numberOfComponents);
    coeff=0.;
// << end new 

    // *new: 
    CompositeGridOperators & op =  *gf[mNew].u.getOperators();
    // *test*
    op.updateToMatchGrid(gf[mNew].cg);

/* ----
    // old: 
    CompositeGridOperators op(gf[mNew].cg);
  ---- */

    op.setStencilSize(stencilSize);
    op.setNumberOfComponentsForCoefficients(numberOfComponents);
    op.setOrderOfAccuracy(orderOfAccuracy);
    // -----

    coeff.setOperators(op);
    
    BoundaryConditionParameters bcParams;
    // bcParams.ghostLineToAssign=2;
    
    // make some shorter names for readability
    BCTypes::BCNames dirichlet             = BCTypes::dirichlet,
    neumann               = BCTypes::neumann,
    extrapolate           = BCTypes::extrapolate,
    allBoundaries         = BCTypes::allBoundaries;
    
    //  ----------------------------------------------------------------------------------
    //  --- Build a solver on the new grid --------------------------------
    //  ----- Do I have to do this?
    //  ---------------------------------------------------------------------------------
    
    Oges solver( gf[mNew].cg );
    int solverType=OgesParameters::yale;
    solver.set(OgesParameters::THEsolverType,solverType);
    
    
    // ----------------------------------------------------------------------------------
    // --- Create composite grid functions need to complete one step of integration -----
    // --------In your ADI code, it looks like rhs is passed?----------------------------
    // ----------------------------------------------------------------------------------
    
    realCompositeGridFunction rhs(gf[mNew].cg,all,all,all,2), newtCur(gf[mNew].cg,all,all,all,2), newtDiff(gf[mNew].cg,all,all,all,2), ones(gf[mNew].cg),CompgridVelocityx(gf[mNew].cg),CompgridVelocityy(gf[mNew].cg), error(gf[mNew].cg,all,all,all,2);
    
    rhs = 0.;
    newtCur = 0.;
    newtDiff = 0.;
    CompgridVelocityx = 0.;
    CompgridVelocityy = 0.;
    ones = 1.;
    error = 0.;
    
    rhs.setOperators(op);
    newtCur.setOperators(op);
    newtDiff.setOperators(op);
    
    rhs.setName("rhs thickness",0);
    rhs.setName("rhs pressure",1);
    
    error.setName("thickness error",0);
    error.setName("pressure error",1);
    
    newtCur.setName("Thickness Newton Iteration",0);
    newtCur.setName("Pressure Newton Iteration",1);
    
    real normRHS, maxr0, maxr1, maxvalh, maxvalp, norminit;
    normRHS=0.;
    maxr0 = 0.;
    maxr1 = 0.;
    norminit = 0.;
    
    int i1,i2,i3;
    int buffer = 50;
    
    //  create show file to help debug
    //Ogshow show("TestMovingEye.show");
    
    /* Test Moving Eye*/
    if( pshow==NULL ){ pshow = new Ogshow("TestMovingEye.show"); }
    (*pshow).setFlushFrequency(1);
    (*pshow).setIsMovingGridProblem(TRUE);
    
    
    //  -----------------------------------
    //  --- Assign the right-hand-side  ---
    //  -----------------------------------

  
    printF(" gf[mCur].t = %9.3e\n",gf[mCur].t);


    for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
    {
        realMappedGridFunction & uOld = gf[mOld].u[grid];
        realMappedGridFunction & uCur = gf[mCur].u[grid];
        realMappedGridFunction & gridVelocity = gf[mNew].getGridVelocity(grid);
        MappedGrid & mgNew = gf[mNew].cg[grid];
        MappedGrid & mgCur = gf[mCur].cg[grid];
        realMappedGridFunction & cgridx =CompgridVelocityx[grid];
        realMappedGridFunction & cgridy =CompgridVelocityy[grid];
        
        //******************Remove this when I fix grid velocity issues
        getIndex(mgNew.dimension(),D1,D2,D3);
        
        if(parameters.gridIsMoving(grid)){
            cgridx(D1,D2,D3) = gridVelocity(D1,D2,D3,0);
            cgridy(D1,D2,D3) = gridVelocity(D1,D2,D3,1);
        }
        else{
            cgridx(D1,D2,D3) = 0.;
            cgridy(D1,D2,D3) = 0.;
        }
        //*********************************************************
      
        // *** COULD USE newtCur = 2*uCur - uOld   (if dt=constant)
        // newtCur[grid] = uCur;  //uNew holds the current Newton iteration approximation of u(t+dt).  To start the iteration, uNew=uCur.

        RealArray & newtCurArray = newtCur[grid];
        if(t0>dt0)
            newtCurArray = 2*uCur - uOld;
        else
            newtCurArray=uCur;

    }//end of for grid


    // NOTE: We assume that each iterate satisfies the interpolation equations
    //  since in the Newton correction we do not include a non-zero RHS to the interpolation equations
    // Further NOTE: By setting newtCur = uCur -> interpolation equations are not satisfied on a moving
    //   grid.
    newtCur.interpolate();
    //(*pshow).startFrame();
    //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
    //(*pshow).saveSolution( newtCur );

/* ---
    // --- these two steps are probably not needed
    rhs.interpolate();  //I didn't build an interpolant but this works?!
    rhs.finishBoundaryConditions(bcParams);
    --- */    

    evaluateThinFilmFunction(  rhs, gf, newtCur,  mNew, mCur, mOld, t0, dt0,  parameters );
    rhs.interpolate(); // NOTE: Not sure if necessary
    //rhs.display();
    
    
    // -- use mask here ( or use gridFunctionNorm function)

    real sum0 = 0.;
    real sum1 = 0.;
    real avg0 = 0.;
    real avg1 = 0.;
    int count = 0;
    
    int maskOption=1; // mask > 0 
    int extra=1; // include 1 ghost 
    int pNorm=1; // 1-norm 
    real lpNormH = lpNorm( pNorm,rhs,0,maskOption, extra ); // discrete L1 norm of H RHS
    real lpNormP = lpNorm( pNorm,rhs,1,maskOption, extra ); // discrete L1 norm of P RHS

    for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mgNew = gf[mNew].cg[grid];
        getIndex(mgNew.gridIndexRange(),I1,I2,I3,1);
        where(mgNew.mask()(I1,I2,I3)>0){
            realArray r0 = fabs(rhs[grid](I1,I2,I3,0));
            realArray r1 = fabs(rhs[grid](I1,I2,I3,1));
            normRHS = max(max(r1),max(max(r0),normRHS));
        }
        /* Newer approach, doesn't work (See line 1169)
        FOR_3D(i1, i2, i3, I1, I2, I3){
            where(mgNew.mask()(i1,i2,i3)>0){
                sum0 = sum0 + fabs(rhs[grid](i1, i2, i3, 0));
                sum1 = sum1 + fabs(rhs[grid](i1, i2, i3, 1));
                count= count + 1;
            }
        } */
    }
    /*
    avg0 = sum0 / count;
    avg1 = sum1 / count;
    //normRHS = ( avg0 + avg1 ) / 2;
    printF("avg0 = %e, avg1 = %e, norm = %e \n", avg0, avg1, (avg0 + avg1) / 2); */

    printF("Norm of RHS is %e \n",normRHS);
    printF("Norm of RHS: l1-Norm H=%9.2e, l1-norm P=%9.2e\n",lpNormH,lpNormP);

    printF("Size of dt is %e\n",dt0);
    
    norminit = normRHS;
    
    /* Test Moving Eye*/
    (*pshow).startFrame();
    (*pshow).saveSolution( rhs );
    
    /*
    (*pshow).startFrame();
    (*pshow).saveSolution( CompgridVelocityx );
    (*pshow).startFrame();
    (*pshow).saveSolution( CompgridVelocityy );
    */
    
    //  -----------------------------------
    //  --- Newton's Method  ---
    //  -----------------------------------
    
    int newtonCount = 1;
    //normRHS = trelerr*norminit + tabserr - 1.;
    clock_t time0, time1, timetot;
    // for tracking the time of each iteration of newtons method
    
    printF("newton convergence criteria is %e \n",trelerr*norminit + tabserr);
    
    real newtTol;
    
    newtTol = min(trelerr*norminit + tabserr,pow(10.,-8));

    while(normRHS > newtTol){
        printF("**************************************************************************************** \n");
        printF("newtons iteration number %i \n", newtonCount);
        coeff = 0.;
        time0 = 0.;
        time1 = 0.;
        timetot = 0.;
        time0 = clock();
        
        if( cDecouple!=0 )
        {
            if(steady==1.0)
            {
                coeff = -S*op.laplacianCoefficients(e0,c0)
                + S*op.laplacianCoefficients(e1,c0)
                + op.identityCoefficients(e1,c1);
            }
            else
            {
                if( useBDF2 ) //BDF2
                {
                coeff = op.identityCoefficients(e0,c0) + (2./3)*(-dt0*( multiply(CompgridVelocityx,op.xCoefficients(e0,c0)) )
                -dt0*( multiply(CompgridVelocityy,op.yCoefficients(e0,c0)) )
            //Linear PDE Comment out ------
                -dt0*(multiply( (0.5*newtCur(c0)*(newtCur.x()(c0)*newtCur.x()(c1) + newtCur.y()(c0)*(newtCur.y()(c1) + G*ones))) ,op.identityCoefficients(e0,c0)) )
                -dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c1)),op.xCoefficients(e0,c0) ) )
                - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*(newtCur.y()(c1) + G*ones)),op.yCoefficients(e0,c0) ) )
                - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.laplacian()(c1)),op.identityCoefficients(e0,c0) ) )
                - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c0)),op.xCoefficients(e0,c1) ) )
                - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.y()(c0)),op.yCoefficients(e0,c1) ) )
                - dt0*( multiply( ((1./12.)*newtCur(c0)*newtCur(c0)*newtCur(c0)),op.laplacianCoefficients(e0,c1) ) )
             // until here
                - dt0*( op.laplacianCoefficients(e0,c1) ) )
                + S*op.laplacianCoefficients(e1,c0)
                + op.identityCoefficients(e1,c1);
                }
                else{
                    coeff = op.identityCoefficients(e0,c0) - dt0*( multiply(CompgridVelocityx,op.xCoefficients(e0,c0)) )
                    -dt0*( multiply(CompgridVelocityy,op.yCoefficients(e0,c0)) )
                    //Linear PDE Comment out ------
                    -dt0*(multiply( (0.5*newtCur(c0)*(newtCur.x()(c0)*newtCur.x()(c1) + newtCur.y()(c0)*(newtCur.y()(c1) + G*ones))) ,op.identityCoefficients(e0,c0)) )
                    -dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c1)),op.xCoefficients(e0,c0) ) )
                    - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*(newtCur.y()(c1) + G*ones)),op.yCoefficients(e0,c0) ) )
                    - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.laplacian()(c1)),op.identityCoefficients(e0,c0) ) )
                    - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.x()(c0)),op.xCoefficients(e0,c1) ) )
                    - dt0*( multiply( (0.25*newtCur(c0)*newtCur(c0)*newtCur.y()(c0)),op.yCoefficients(e0,c1) ) )
                    - dt0*( multiply( ((1./12.)*newtCur(c0)*newtCur(c0)*newtCur(c0)),op.laplacianCoefficients(e0,c1) ) )
                    // until here
                    - dt0*( op.laplacianCoefficients(e0,c1) )
                    + S*op.laplacianCoefficients(e1,c0)
                    + op.identityCoefficients(e1,c1);
                }
            }
        }
        else
        {
            coeff = op.identityCoefficients(e0,c0) - dt0*( multiply(CompgridVelocityx,op.xCoefficients(e0,c0)) )
                -dt0*( multiply(CompgridVelocityy,op.yCoefficients(e0,c0)) )
                + S*op.laplacianCoefficients(e1,c0)
                + op.identityCoefficients(e1,c1);
        }
	
        // Start with dirichlet boundary conditions
        
        if((flux!=0)&&(steady!=1.0))
        {
            printF("We are applying flux boundary conditions to coeff \n");
            coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,allBoundaries);
            printF("Done with dirichlet boundary conditions. \n");
            coeff.applyBoundaryConditionCoefficients(1,1,neumann,allBoundaries);
            printF("Done with neumann boundary conditions. \n");
            fillInCoeffBoundaryConditions(coeff,newtCur,parameters);
            printF("Done with fillInCoeffBoundaryConditions. \n");
            coeff.finishBoundaryConditions();
            printF("Done with finishBoundaryConditions. \n");
        }
        else{
            printF("We are applying dirichlet boundary conditions to coeff \n");
            coeff.applyBoundaryConditionCoefficients(0,0,dirichlet,allBoundaries);
            coeff.applyBoundaryConditionCoefficients(1,1,dirichlet,allBoundaries);
            coeff.applyBoundaryConditionCoefficients(0,0,extrapolate,allBoundaries);
            coeff.applyBoundaryConditionCoefficients(1,1,extrapolate,allBoundaries);
            coeff.finishBoundaryConditions();
        }
        
        solver.setCoefficientArray( coeff );
        rhs = -rhs;
        //Oges::debug=63;
        newtDiff = 0.;
        solver.solve(newtDiff,rhs);
        
        
        // update current newton iteration
        newtCur = newtCur + newtDiff;

	// **** This may not be needed if the initial guess satisfies the interpolation equations
        newtCur.interpolate();
        rhs = 0.;
        
        // ------------ Recalculate F ---------------------
        evaluateThinFilmFunction(  rhs, gf, newtCur,  mNew, mCur, mOld, t0, dt0,  parameters );
	
//      OLD
        maxr1=0.;
        maxr0=0.;
        normRHS=0.;

        for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mgNew = gf[mNew].cg[grid];
            getIndex(mgNew.gridIndexRange(),I1,I2,I3,1);
            

            where(mgNew.mask()(I1,I2,I3)>0){
                 realArray r0 = abs(rhs[grid](I1,I2,I3,0));
                 realArray r1 = abs(rhs[grid](I1,I2,I3,1));
                 normRHS = max(max(r1),max(max(r0),normRHS));
                 maxr0 = max(maxr0,max(r0));
                 maxr1 = max(maxr1,max(r1));
//                newsum0 = sum(abs(rhs[grid](I1, I2, I3, 0)));
//                newsum1 = sum(abs(rhs[grid](I1, I2, I3, 1)));
//                printF("Sum of grid 0 is %e \n", newsum0);
//                printF("Sum of grid 1 is %e \n", newsum1);
//                printF("This norm would be %e \n", (newsum0 + newsum1) / 2);
            }
        }
//      END OLD
/*
// NEW not implemented, doesn't change from iteration to iteration
        real newsum0 = 0.;
        real newsum1 = 0.;
        real sum0 = 0.;
        real sum1 = 0.;
        real avg0 = 0.;
        real avg1 = 0.;
        int count = 0;
        
        for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mgNew = gf[mNew].cg[grid];
            getIndex(mgNew.gridIndexRange(),I1,I2,I3,1);
            //        where(mgNew.mask()(I1,I2,I3)>0){
            //            realArray r0 = fabs(rhs[grid](I1,I2,I3,0));
            //            realArray r1 = fabs(rhs[grid](I1,I2,I3,1));
            //            normRHS = max(max(r1),max(max(r0),normRHS));
            //        }
            FOR_3D(i1, i2, i3, I1, I2, I3){
                where(mgNew.mask()(i1,i2,i3)>0){
                    sum0 = sum0 + fabs(rhs[grid](i1, i2, i3, 0));
                    sum1 = sum1 + fabs(rhs[grid](i1, i2, i3, 1));
                    count= count + 1;
                }
            }
            newsum0 = sum(abs(rhs[grid](I1, I2, I3, 0)));
            newsum1 = sum(abs(rhs[grid](I1, I2, I3, 1)));
            printF("**Sum of grid 0 is %e \n", newsum0);
            printF("Sum of grid 1 is %e \n", newsum1);
            printF("This norm would be %e** \n", (newsum0 + newsum1) / (2 * count));
        }
        avg0 = sum0 / count;
        avg1 = sum1 / count;
        //normRHS = ( avg0 + avg1 ) / 2;
        printF("sum0 = %e, sum1 = %e, norm = %e \n", sum0, sum1, (avg0 + avg1) / 2);
        printF("avg0 = %e, avg1 = %e \n", avg0, avg1);
        
// END NEW
*/
        
        time1 = clock();
        timetot = (double)(time1 - time0) / CLOCKS_PER_SEC;
        printF("time for solving of the problem = %d seconds, (iterations=%i)\n",timetot,solver.getNumberOfIterations());
        printF("Norm of RHS is %e.  The max of r0 is %e.  The max of r1 is %e. \n",normRHS,maxr0,maxr1);
        
        //rhs.display();
        //normRHS = pow(10.,-6);
        newtonCount = newtonCount + 1;
        
        rhs.interpolate();
        
        /* Test Moving Eye */
        (*pshow).startFrame();
        //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
        (*pshow).saveSolution( rhs );
        /*
        (*pshow).startFrame();
        //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
        (*pshow).saveSolution( newtCur );
         */
        

        
        //normRHS = trelerr*norminit + tabserr - 1.;
        
    }//end of while loop
    
    gf[mNew].u = newtCur;
    

    // ------------- Compute errors ----------------
    const bool manufacturedTearFilm = parameters.dbase.get<bool >("manufacturedTearFilm");
    const bool twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
    if( twilightZoneFlow || manufacturedTearFilm  )
    {
        maxvalh = 0.;
        maxvalp = 0.;
        const real tNew=t0+dt0;
	real totalErrMaxH=0.;
	real totalErrMaxP=0.;
        for( int grid=0; grid<gf[mNew].cg.numberOfComponentGrids(); grid++ )
        {
            int extra=0;  // include ghost points in error

            realMappedGridFunction & err = error[grid];
            realMappedGridFunction & uNew = gf[mNew].u[grid];
            MappedGrid & mgNew = gf[mNew].cg[grid];
            getIndex(mgNew.gridIndexRange(),I1,I2,I3,extra);

            const int numberOfDimensions = mgNew.numberOfDimensions();
            OV_GET_SERIAL_ARRAY_CONST(real,mgNew.center(),xLocal);

            Range N=2;
	    RealArray ue(I1,I2,I3,2);
	    if( twilightZoneFlow )
	    {
              const bool isRectangular = false; // do this for now
	      exact.gd( ue  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,N,tNew);
	    }
	    else if( manufacturedTearFilm )
	    {
	      assert( !twilightZoneFlow );
      
	      // Formula : H0 * exp(-beta*(y-yl)) + Hm
	      //         H0 = height at lower lid, y=yl
	      //         Hm = height at the centre of the eye

	      // Make a sub-directory in the data-base to store variables used here and in userDefinedInitialConditions
	      if( !parameters.dbase.get<DataBase >("modelData").has_key("CgadUserDefinedInitialConditionData") )
	      {
		printF("evaluateThinFilmFunction:ERROR: sub-directory `CgadUserDefinedInitialConditionData' not found!\n");
		Overture::abort("error");
	      }
	      DataBase & db = parameters.dbase.get<DataBase >("modelData").get<DataBase>("CgadUserDefinedInitialConditionData");

	      RealArray & tearFilmParameters = db.get<RealArray>("tearFilmParameters");
	      const real & H0   = tearFilmParameters(0);
	      const real & beta = tearFilmParameters(1);
	      const real & yl   = tearFilmParameters(2);
	      const real & Hm   = tearFilmParameters(3);
      
	      RealArray temp(I1,I2,I3);
	      temp=H0*exp(-beta*(xLocal(I1,I2,I3,1)-yl));

	      ue(I1,I2,I3,0)= temp + Hm;    
	      ue(I1,I2,I3,1)= (-S*beta*beta)*temp;
	    }

            err=0.;
            where( mgNew.mask()(I1,I2,I3)!=0 )
            {
               err(I1,I2,I3,0) = uNew(I1,I2,I3,0)-ue(I1,I2,I3,0);
               err(I1,I2,I3,1) = uNew(I1,I2,I3,1)-ue(I1,I2,I3,1);
               //error=max(error,max(abs(err(I1,I2,I3))));
               maxvalh = max(maxvalh,max(fabs(ue(I1,I2,I3,0))));
               maxvalp = max(maxvalp,max(fabs(ue(I1,I2,I3,1))));
               //err(I1,I2,I3) = err(I1,I2,I3)/maxval;
            }

            real errMaxH = max(fabs(err(I1,I2,I3,0)));
            real errMaxP = max(fabs(err(I1,I2,I3,1)));
	    totalErrMaxH=max(totalErrMaxH,errMaxH);
	    totalErrMaxP=max(totalErrMaxP,errMaxP);
	    

            printF(" t=%9.3e : grid=%i: err(h)=%9.3e, err(p)=%9.3e\n",tNew,grid,errMaxH,errMaxP);
        }
        //error.display("error");
        //error(c0) = error(c0)/maxvalh;
        //error(c1) = error(c1)/maxvalp;
    
        printF(" t=%9.3e : err(h)=%9.3e, err(p)=%9.3e\n",tNew,totalErrMaxH,totalErrMaxP);
        printF("max value of thickness is %e and max value of pressure is %e \n",maxvalh,maxvalp);
        
        /* Test Moving Eye
        (*pshow).startFrame();
        //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
        (*pshow).saveSolution( error );
        */
    }


    /* Test Moving Eye
    (*pshow).startFrame();
    //show.saveComment(0,sPrintF(buffer,"Here is the error at t=%e.",t0+dt0));
    (*pshow).saveSolution( rhs );
     */
    
  if( correction==0 )
  {
    // printF(" +++ ims: gf[mNew].t=%9.3e --> change to t0+dt0=%9.3e +++\n",gf[mNew].t,t0+dt0);
    gf[mNew].t=t0+dt0;  // gf[mNew] now lives at this time
  }
    
  return 0;
}



