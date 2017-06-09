// This file automatically generated from forcing.bC with bpp.
#include "Maxwell.h"
#include "DispersiveMaterialParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "interpPoints.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"
#include "App.h"

#define forcingOptMaxwell EXTERN_C_NAME(forcingoptmaxwell)
#define ogf EXTERN_C_NAME(ogf)
#define ogf2d EXTERN_C_NAME(ogf2d)
#define ogf2dfo EXTERN_C_NAME(ogf2dfo)
#define ogderiv EXTERN_C_NAME(ogderiv)
#define ogderiv3 EXTERN_C_NAME(ogderiv3)
#define ogf3d EXTERN_C_NAME(ogf3d)
#define ogf3dfo EXTERN_C_NAME(ogf3dfo)
#define exx EXTERN_C_NAME(exx)

extern "C"
{
    void forcingOptMaxwell(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
            real&u, const real&f, const int&mask, const real&rx, const real&x,  
            const int&ipar, const real&rpar, int&ierr );
}



extern "C"
{

/* Here are functions for TZ flow that can be called from fortran */



real
ogf(OGFunction *&ep, const real &x, const real &y,const real &z, const int & c, const real & t )
{
    return (*ep)(x,y,z,c,t);
}


/* return (u,v,w) = (Ex,Ey,Hz) */
void
ogf2d(OGFunction *&ep, const real &x, const real &y, const real & t, real & u, real & v, real & w )
{
    /* assumes ex=0, ey=1, hz=2 */
    u=(*ep)(x,y,0.,0,t);
    v=(*ep)(x,y,0.,1,t);
    w=(*ep)(x,y,0.,2,t);
}

/* return (u,v,w) = (Ex,Ey,Hz) if fieldOption=0 or (u,v,w) = (Ex_t,Ey_t,Hz_t) if fieldOption=1 */
void
ogf2dfo(OGFunction *&ep, const int &ex, const int &ey, const int &hz, const int & fieldOption, 
                const real &x, const real &y, const real & t, real & u, real & v, real & w )
{
    if( fieldOption==0 )
    {
        u=(*ep)(x,y,0.,ex,t);
        v=(*ep)(x,y,0.,ey,t);
        w=(*ep)(x,y,0.,hz,t);
    }
    else if( fieldOption==1 )
    {
        /* assumes ex=0, ext=3 */
        u=(*ep)(x,y,0.,ex+3,t);
        v=(*ep)(x,y,0.,ex+4,t);
        w=(*ep)(x,y,0.,ex+5,t);
    }
    else
    {
        printf("ogf2dfo:ERROR - fieldOption=%i\n",fieldOption);
        OV_ABORT("error");
    }
    
}

/* return a general derivative */
void
ogderiv(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
                  const real &x, const real &y, const real &z, const real & t, const int & n, real & ud )
{
    ud=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n,t);
}

/* return a general derivative for 3 components */
void
ogderiv3(OGFunction *&ep, const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
                  const real &x, const real &y, const real &z, const real & t, 
                  const int & n1, real & ud1, const int & n2, real & ud2, const int & n3, real & ud3 )
{
    ud1=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n1,t);
    ud2=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n2,t);
    ud3=(*ep).gd(ntd,nxd,nyd,nzd,x,y,z,n3,t);
}

/* return (u,v,w) = (Ex,Ey,Ez) */
void
ogf3d(OGFunction *&ep, const real &x, const real &y, const real &z, const real & t, real & u, real & v, real & w )
{
    /* assumes ex=0, ey=1, hz=2 */
    u=(*ep)(x,y,z,0,t);
    v=(*ep)(x,y,z,1,t);
    w=(*ep)(x,y,z,2,t);
}

/* return (u,v,w) = (Ex,Ey,Ez) if fieldOption==0, return (u,v,w) = (Ex_t,Ey_t,Ez_t) if fieldOption==1 */
void
ogf3dfo(OGFunction *&ep, const int &ex, const int &ey, const int &ez, const int & fieldOption, 
                const real &x, const real &y, const real &z, const real & t, real & u, real & v, real & w )
{
    /* assumes ex=0, ey=1, ez=2 */
    if( fieldOption==0 )
    {
        u=(*ep)(x,y,z,ex,t);
        v=(*ep)(x,y,z,ey,t);
        w=(*ep)(x,y,z,ez,t);
    }
    else if( fieldOption==1 )
    {
        /* assumes ex=0, ext=3 */
        u=(*ep)(x,y,z,ex+3,t);
        v=(*ep)(x,y,z,ex+4,t);
        w=(*ep)(x,y,z,ex+5,t);
    }
    else
    {
        printf("ogf2dfo:ERROR - fieldOption=%i\n",fieldOption);
    }
}

real
exx(OGFunction *&ep, const real &x, const real &y,const real &z, const int & c, const real & t )
{
    real value=(*ep).xx(x,y,z,c,t);
  // printF("exx: x=(%8.2e,%8.2e,%8.2e) c=%i t=%8.2e ...exx=%8.2e \n",x,y,z,c,t,value);
    return value;
}

}


#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

// Define macros for forcing functions:
//   (Ex).t = (1/eps)*[  (Hz).y ]
//   (Ey).t = (1/eps)*[ -(Hz).x ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

#define exTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[0]
#define eyTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[1]
#define hzTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

#define extTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[0]
#define eytTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[1]
#define hztTrue(x,y,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

#define exLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[0])
#define eyLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[1])
#define hzLaplacianTrue(x,y,t) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky))*pwc[5])

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
//
//   (Ex).t = (1/eps)*[ (Hz).y - (Hy).z ]
//   (Ey).t = (1/eps)*[ (Hx).z - (Hz).x ]
//   (Ez).t = (1/eps)*[ (Hy).x - (Hx).y ]
//   (Hx).t = (1/mu) *[ (Ey).z - (Ez).y ]
//   (Hy).t = (1/mu) *[ (Ez).x - (Ex).z ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

// ****************** finish this -> should `rotate' the 2d solution ****************

#define exTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[0]
#define eyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[1]
#define ezTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[2]

#define extTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[0]
#define eytTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[1]
#define eztTrue3d(x,y,z,t) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[2]



#define hxTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[3]
#define hyTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[4]
#define hzTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*pwc[5]

#define exLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[0])
#define eyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[1])
#define ezLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[2])

#define hxLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[3])
#define hyLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[4])
#define hzLaplacianTrue3d(x,y,z,t) sin(twoPi*(kx*(x)+ky*(y)+kz*(z)-cc*(t)))*(-(twoPi*twoPi*(kx*kx+ky*ky+kz*kz))*pwc[5])



//==================================================================================================
// Evaluate Tom Hagstom's exact solution defined as an integral of Guassian sources
// 
// OPTION: OPTION=solution or OPTION=error OPTION=bounary to compute the solution or the error or
//     the boundary condition
//
//==================================================================================================


//==================================================================================================
// The DEFINE_GF_MACRO is a helper for EXTRACT_GFP and sets up the cpp macros for a given field
//
//==================================================================================================
//==================================================================================================

//==================================================================================================
// The EXTRACT_GFP macro extracts gridfunction pointers and array bounds.
//                 We use these to write code that works in 2/3D for both the nfdtd and
//                 dsi schemes.
//
// The macro expects the user to have the following variables defined in the enclosing scope:
//    Index I1,I2,I3 - used to get the index ranges for each grid
//    CompositeGrid &cg - the composite grid used by the fields
//    int grid      - the current grid to setup the pointers for
//    int i1,i2,i3  - grid indices into the arrays at the appropriate centering
//    MappedGrid *Maxwell:mgp - or -
//          CompositeGrid *Maxwell cgfields -or- CompositeGrid *Maxwell dsi_cgfields
//
// The macro defines:
//    cpp macros:
//               UH{XYZ}(i0,i1,i2) - access the current h field at i1,i2,i3 with the appropriate centering
//               UE{XYZ}(i0,i1,i2) - access the current e field at i1,i2,i3 with the appropriate centering        
//               UMH{XYZ}(i0,i1,i2) - the h field at the previous timestep
//               UME{XYZ}(i0,i1,i2) - the e field at the previous timestep
//               UNH{XYZ}(i0,i1,i2) - the h field at the next timestep
//               UNE{XYZ}(i0,i1,i2) - the h field at the next timestep
//
//               ERRH{XYZ}(i0,i1,i2) - acces the h field error gridfunction
//               ERRE{XYZ}(i0,i1,i2) - acces the e field error gridfunction
//
//               XEP(i0,i1,i2,i3) - coordinates of e centering
//               XHP(i0,i1,i2,i3) - coordinates of h centering
// 
//    variables:
//               MappedGrid &mg - the current mapped grid (cg[grid])
//
//               const bool isStructured - true for structured grids
//               const bool isRectangular - true for rectangular grids
//
//               realMappedGridFunction uh - view of the current h or h.n field
//               realMappedGridFunction ue - view of the current e or e.n field
//               realMappedGridFunction umh - view of the previous h or h.n field
//               realMappedGridFunction ume - view of the previous e or e.n field
//               realMappedGridFunction unh - view of the next h or h.n field
//               realMappedGridFunction une - view of the next e or e.n field
//               realMappedGridFunction errh - view of the h field error
//               realMappedGridFunction erre - view of the e field error
//               realArray xe - view of the x coordinates at the e centering
//               realArray xh - view of the x coordinates at the h centering
//               realArray ye - view of the y coordinates at the e centering
//               realArray yh - view of the y coordinates at the h centering
//               realArray ze - view of the z coordinates at the e centering
//               realArray zh - view of the z coordinates at the h centering
//               realArray xce - coordinates of the e centering
//               realArray xch - coordinates of the h centering
//               realArray emptyArray - used for setting references to things we don't need
//               real *uhp - data pointer for the current h or h.n field
//               real *uep - data pointer for the current h or e.n field
//               real *umhp - data pointer for the previous h or h.n field
//               real *umep - data pointer for the previous h or e.n field
//               real *unhp - data pointer for the next h or h.n field
//               real *unep - data pointer for the next h or e.n field
//               real *xep - data pointer for the coordinates at the e centering
//               real *xhp - data pointer for the coordinates at the h centering
//
//               real dx[3] - dx in each direction for rectangular grids  (={0,0,0} if !isRectangular)
//               real xab[2][3] - coordinate bounds for rectangular grids (={ {0,0},.. } if !isRectangular)
//
//               int uhDim0,uhDim1,uhDim2 - array dimensions for the e gridfunctions
//               int ueDim0,ueDim1,ueDim2 - array dimensions for the h gridfunctions
//               int xeDim0,xeDim1,xeDim2 - array dimensions for the e centering coordinates
//               int xhDim0,xhDim1,xhDim2 - array dimensions for the h centering coordinates
//
// KNOWN ASSUMPTIONS:  * gridFunctions for the same variable at different time levels have the same
//                                   raw data sizes
//                     * there are unrecognized and perhaps subtle assumptions being made
//         
// OPTION: 
//==================================================================================================
//==================================================================================================
//==================================================================================================



//==================================================================================================
// This bpp macro undefs the cpp macros defined by EXTRACT_GFP
//               UH(i0,i1,i2) - access the current h field at i1,i2,i3 with the appropriate centering
//               UE(i0,i1,i2) - access the current e field at i1,i2,i3 with the appropriate centering        
//               UMH(i0,i1,i2) - the h field at the previous timestep
//               UME(i0,i1,i2) - the e field at the previous timestep
//               UNH(i0,i1,i2) - the h field at the next timestep
//               UNE(i0,i1,i2) - the h field at the next timestep
//               XEP(i0,i1,i2,i3) - coordinates of e centering
//               XHP(i0,i1,i2,i3) - coordinates of h centering
// OPTION: 
//==================================================================================================
//==================================================================================================
// Evaluate the annulus eigenfunction or it's error
// 
// OPTION: solution, error
//==================================================================================================

// Gaussian pulse initial conditions:
// #Include "gaussianPulse.h"






int Maxwell::
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
    bool computeForcing = forcingIsOn();
    
  // bool computeForcing = (forcingOption==twilightZoneForcing || 
  // 			 forcingOption==gaussianSource ||
  // 			 forcingOption==magneticSinusoidalPointSource ||
  //                        forcingOption==gaussianChargeSource||
  //                        forcingOption==userDefinedForcingOption );
    
    if( !computeForcing )
        return 0;
    
  // real time0=getCPU();

    if( forcingOption==userDefinedForcingOption )
    {
    // -- Compute user defined forcing ---

        int iparam[]={grid,current};  //
        real rparam[]={t,dt};  //
        if( option==1 )
            assign(u,0.); // user defined forcing only knows option==0
        userDefinedForcing( u,iparam,rparam );

        return 0;
    }


    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    const int numberOfComponentGrids = cg.numberOfComponentGrids();

    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    const int numberOfDimensions = cg.numberOfDimensions();

  // bpp macro
    #ifdef EXTGFP_SENTINEL
    ERROR : XXX : you have not closed the current EXTRACT_GFP macro before starting a new one!
    #endif
    #define EXTGFP_SENTINEL
    #ifdef EXTGFP_SENTINEL
    realArray emptyArray;
    realSerialArray emptySerialArray;
    Range all;
    Index I1,I2,I3;
    Index Iev[3], &Ie1=Iev[0], &Ie2=Iev[1], &Ie3=Iev[2];
    Index Ihv[3], &Ih1=Ihv[0], &Ih2=Ihv[1], &Ih3=Ihv[2];
    MappedGrid & mg = cg[grid];
    const bool isStructured = mg.getGridType()==MappedGrid::structuredGrid;
    const bool isRectangular = mg.isRectangular();
    assert( !(isRectangular && !isStructured) ); // just a little check on the MappedGrid's data
    real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    if( isRectangular )
        mg.getRectangularGridParameters( dx, xab );
  //realMappedGridFunction uh,ue,umh,ume,unh,une;
    realSerialArray uh,ue,umh,ume,unh,une,errh,erre;
    realSerialArray xe,xh,ye,yh,ze,zh,xce,xch;
    realSerialArray uepp, uhpp; // dsi projection arrays
    intArray & mask = mg.mask();
    #ifdef USE_PPP
        intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
    #else
        intSerialArray & maskLocal = mask;
    #endif
  // const bool buildCenter = !( isRectangular &&
  // 			    ( initialConditionOption==squareEigenfunctionInitialCondition ||
  // 			      initialConditionOption==gaussianPulseInitialCondition ||
  //                               (forcingOption==gaussianChargeSource && initialConditionOption==defaultInitialCondition)
  //                               || initialConditionOption==userDefinedKnownSolutionInitialCondition 
  //                               || initialConditionOption==userDefinedInitialConditionsOption
  //                                // || initialConditionOption==planeMaterialInterfaceInitialCondition
  // 			       // ||  initialConditionOption==annulusEigenfunctionInitialCondition
  // 			       ) 
  // 			    ); // fix this 
    const bool buildCenter = vertexArrayIsNeeded( grid );
    if( buildCenter )
    {
    // printF("assignInitialConditions:INFO:build the grid vertices, grid=%i\n",grid);
        mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex);
    }
    const realArray & center = buildCenter ? mg.center() : emptyArray;
    realSerialArray uLocal, umLocal;
    real dtb2=dt*.5;
    real tE = t, tH = t;
    getIndex(mg.dimension(),I1,I2,I3);
    int includeGhost=1;
    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
    if( !ok ) return 0;  // no communication allowed after this point : check this ******************************************************
    if( method==nfdtd || method==sosup ) 
    {
        Ie1 = Ih1 = I1;
        Ie2 = Ih2 = I2;
        Ie3 = Ih3 = I3;
    // nfdtd uses one gridFunction per time level
        realMappedGridFunction & uall = mgp==NULL ? getCGField(HField,current)[grid] : fields[current];
        realMappedGridFunction & umall = mgp==NULL ? getCGField(HField,prev)[grid] : fields[prev];
        realMappedGridFunction & unall = mgp==NULL ? getCGField(HField,next)[grid] : fields[next];
        #ifdef USE_PPP
            getLocalArrayWithGhostBoundaries(uall,uLocal);
            getLocalArrayWithGhostBoundaries(umall,umLocal);
            realSerialArray unLocal; getLocalArrayWithGhostBoundaries(unall,unLocal);
        #else
            uLocal.reference(uall);
            umLocal.reference(umall);
            realSerialArray & unLocal = unall;
        #endif
        if ( cg.numberOfDimensions()==2 ) 
        {
            ue.reference( uLocal(I1,I2,I3,Range(ex,ey)) );
            uh.reference( uLocal(I1,I2,I3,hz) );
            ume.reference( umLocal(I1,I2,I3,Range(ex,ey)) );
            umh.reference( umLocal(I1,I2,I3,hz) );
            une.reference( unLocal(I1,I2,I3,Range(ex,ey)) );
            unh.reference( unLocal(I1,I2,I3,hz) );
            if( errp )
            {
                #ifdef USE_PPP
                    realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
                    errh.reference(errLocal(I1,I2,I3,hz));
                    erre.reference(errLocal(I1,I2,I3,Range(ex,ey)));
                #else
                    errh.reference((*errp)(I1,I2,I3,hz));
                    erre.reference((*errp)(I1,I2,I3,Range(ex,ey)));
                #endif
            }
            else if ( cgerrp )
            {
                #ifdef USE_PPP
                    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
                    errh.reference(errLocal(I1,I2,I3,hz));
                    erre.reference(errLocal(I1,I2,I3,Range(ex,ey)));
                #else
                    errh.reference((*cgerrp)[grid](I1,I2,I3,hz));
                    erre.reference((*cgerrp)[grid](I1,I2,I3,Range(ex,ey)));
                #endif
            }
        }
        else
        {
            if ( solveForElectricField )
            {
                ue.reference( uLocal(I1,I2,I3,Range(ex,ez)) );
                ume.reference( umLocal(I1,I2,I3,Range(ex,ez)) );
                une.reference( unLocal(I1,I2,I3,Range(ex,ez)) );
                if ( errp )
                {
                    #ifdef USE_PPP
                        realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
                        erre.reference(errLocal(I1,I2,I3,Range(ex,ez)));
                    #else
                        erre.reference((*errp)(I1,I2,I3,Range(ex,ez)));
                    #endif
                }
                else if ( cgerrp )
                {
                    #ifdef USE_PPP
                        realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
                        erre.reference(errLocal(I1,I2,I3,Range(ex,ez)));
                    #else
                        erre.reference((*cgerrp)[grid](I1,I2,I3,Range(ex,ez)));
                    #endif
                }
            }
            if ( solveForMagneticField )
            {
                uh.reference( uLocal(I1,I2,I3,Range(hx,hz)) );
                umh.reference( umLocal(I1,I2,I3,Range(hx,hz)) );
                unh.reference( unLocal(I1,I2,I3,Range(hx,hz)) );
                if ( errp )
                {
                    #ifdef USE_PPP
                        realSerialArray errLocal; getLocalArrayWithGhostBoundaries(*errp,errLocal);
                        errh.reference(errLocal(I1,I2,I3,Range(hx,hz)));
                    #else
                        errh.reference((*errp)(I1,I2,I3,Range(hx,hz)));
                    #endif
                }
                else if ( cgerrp )
                {
                    #ifdef USE_PPP
                        realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
                        errh.reference(errLocal(I1,I2,I3,Range(hx,hz)));
                    #else
                        errh.reference((*cgerrp)[grid](I1,I2,I3,Range(hx,hz)));
                    #endif
                }
            }
        }
        #ifdef USE_PPP
            realSerialArray xLocal; if( buildCenter ) getLocalArrayWithGhostBoundaries(center,xLocal);
        #else
            const realSerialArray & xLocal = center;
        #endif
        if( buildCenter )
        {
      // *wdh* 041015 these next assignments fail in P++ if buildCenter==false -- but why make the reference?
            xe.reference( (buildCenter ? xLocal(I1,I2,I3,0) : emptySerialArray) );
            ye.reference( (buildCenter ? xLocal(I1,I2,I3,1) : emptySerialArray) );
            if ( numberOfDimensions==3 )
                ze.reference( (buildCenter ? xLocal(I1,I2,I3,2) : emptySerialArray) );
      // nfdtd uses the same centering for e and h
            xh.reference(xe);
            yh.reference(ye);
            zh.reference(ze);  // *wdh* 090628
        }
        if ( buildCenter )
        {
            xce.reference(xLocal);
            xch.reference(xce);
        }
    }
    else // dsi or yee scheme
    {
        tE -= dtb2;
        if ( method==dsiMatVec /*&& numberOfDimensions==3*/ )
        {
            realMappedGridFunction &uep = (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]);
            realMappedGridFunction &uhp = (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]);
            realMappedGridFunction &uepm = (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]);
            realMappedGridFunction &uhpm = (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]);
            #ifdef USE_PPP
                realSerialArray uepLocal; getLocalArrayWithGhostBoundaries(uep,uepLocal);
                realSerialArray uhpLocal; getLocalArrayWithGhostBoundaries(uhp,uhpLocal);
                realSerialArray uepmLocal; getLocalArrayWithGhostBoundaries(uepm,uepmLocal);
                realSerialArray uhpmLocal; getLocalArrayWithGhostBoundaries(uhpm,uhpmLocal);
            #else
                realSerialArray & uepLocal=uep;
                realSerialArray & uhpLocal=uhp;
                realSerialArray & uepmLocal=uepm;
                realSerialArray & uhpmLocal=uhpm;
            #endif
            realMappedGridFunction uer, uhr, uerm, uhrm;
            uepp.reference(uepLocal);
            uhpp.reference(uhpLocal);
            ue.reference ( uepLocal );
            uh.reference ( uhpLocal );
            ume.reference ( uepmLocal );
            umh.reference ( uhpmLocal );
        }
        else
        {
            #ifdef USE_PPP
                Overture::abort("finish me for parallel");
            #else
                ue.reference ( (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]) );
                uh.reference ( (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]) );
                ume.reference ( (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]) );
                umh.reference ( (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]) );
            #endif
        }
  // #Else
  //       ue.reference ( (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]) );
  //       uh.reference ( (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]) );
  //       ume.reference ( (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]) );
  //       umh.reference ( (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]) );
  // #End
        if ( errp )
        {
            #ifdef USE_PPP
                Overture::abort("finish me for parallel");
            #else
                erre.reference(errp[0]);
                errh.reference(errp[1]);
            #endif
        }
        else if ( cgerrp )
        {
            #ifdef USE_PPP
                Overture::abort("finish me for parallel");
            #else
                erre.reference(cgerrp[0][grid]);
                errh.reference(cgerrp[1][grid]);
            #endif
        }
        Ih1 = Range(uh.getBase(0),uh.getBound(0));
        Ih2 = Range(uh.getBase(1),uh.getBound(1));
        Ih3 = Range(uh.getBase(2),uh.getBound(2));
        Ie1 = Range(ue.getBase(0),ue.getBound(0));
        Ie2 = Range(ue.getBase(1),ue.getBound(1));
        Ie3 = Range(ue.getBase(2),ue.getBound(2));
        I1 = Ih1;
        I2 = Ih2;
        I3 = Ih3;
        if ( buildCenter )
        {
            const realArray & center = mg.isAllVertexCentered() ? mg.corner() : mg.center();
      //      cout<<"CENTER SIZE "<<center.getLength(0)<<endl;
      //      cout<<"MG VERTEX/CELL CENT "<<mg.isAllVertexCentered()<<"  "<<mg.isAllCellCentered()<<endl;
      //      getFaceCenters(mg, xce);
          #ifdef USE_PPP
              Overture::abort("finish me for parallel");
          #else    
            if ( mg.numberOfDimensions()==2 )
            {
                getCenters(mg, UnstructuredMapping::Edge, xce);
                xch.reference(center);
                xce.reshape(Range(xce.getLength(0)),1,1,Range(xce.getLength(1)));
            }
            else
            {
                getCenters(mg, UnstructuredMapping::Face, xch);
                getCenters(mg, UnstructuredMapping::Edge, xce);
                xce.reshape(Range(xce.getLength(0)),1,1,Range(xce.getLength(1)));
                xch.reshape(Range(xch.getLength(0)),1,1,Range(xch.getLength(1)));
            }
      //xh.reference(xch(all,all,all,0));
      //yh.reference(xch(all,all,all,1));
            xh.reference(xch(Ih1,Ih2,Ih3,0,all));
            yh.reference(xch(Ih1,Ih2,Ih3,1,all));
            if ( numberOfDimensions==3 )
                zh.reference(xch(Ih1,Ih2,Ih3,2,all));
            xe.reference(xce(Ie1,Ie2,Ie3,0,all));
            ye.reference(xce(Ie1,Ie2,Ie3,1,all));
            if ( numberOfDimensions==3 )
                ze.reference(xce(Ie1,Ie2,Ie3,2,all));
        #endif
        }
    }
    real *uhp=0,*uep=0,*umhp=0,*umep=0,*unhp=0,*unep=0,*xep=0,*xhp=0,*errhp=0,*errep=0;
    int uhDim0,uhDim1,uhDim2,uhDimFA;
    int ueDim0,ueDim1,ueDim2,ueDimFA;
    int xeDim0,xeDim1,xeDim2;
    int xhDim0,xhDim1,xhDim2;
    uhDim0=uhDim1=uhDim2=ueDim0=ueDim1=ueDim2=xeDim0=xeDim1=xeDim2=xhDim0=xhDim1=xhDim2=-1;
  // #ifdef USE_PPP
  // realSerialArray uel; getLocalArrayWithGhostBoundaries(ue,uel);
  // realSerialArray uhl; getLocalArrayWithGhostBoundaries(uh,uhl);
  // realSerialArray umel; if( ume.getLength(0)>0 ) ume.getLocalArrayWithGhostBoundaries(ume,umel);
  // realSerialArray umhl; if( umh.getLength(0)>0 ) umh.getLocalArrayWithGhostBoundaries(umh,umhl);
  // realSerialArray unel; getLocalArrayWithGhostBoundaries(une,unel);
  // realSerialArray unhl; getLocalArrayWithGhostBoundaries(unh,unhl);
  // realSerialArray xcel; getLocalArrayWithGhostBoundaries(xce,xcel);
  // realSerialArray xchl; getLocalArrayWithGhostBoundaries(xch,xchl);
  // realSerialArray errel; getLocalArrayWithGhostBoundaries(erre,errel);
  // realSerialArray errhl; getLocalArrayWithGhostBoundaries(errh,errhl);
  // // intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskl);
  // #else
    const realSerialArray &uel = ue;
    const realSerialArray &uhl = uh;
    const realSerialArray &umel = ume;
    const realSerialArray &umhl = umh;
    const realSerialArray &unel = une;
    const realSerialArray &unhl = unh;
    const realSerialArray &xcel = xce;
    const realSerialArray &xchl = xch;
    const realSerialArray &errel = erre;
    const realSerialArray &errhl = errh;
  // const intSerialArray & maskLocal = mask;
  // #endif
  // H field pointer and array definitions
    uhp = uhl.Array_Descriptor.Array_View_Pointer3;
    if ( umhl.getLength(0) )
    {
        umhp = umhl.Array_Descriptor.Array_View_Pointer3;
    }
    unhp = unhl.Array_Descriptor.Array_View_Pointer3;
    uhDim0 = uhl.getRawDataSize(0);
    uhDim1 = uhl.getRawDataSize(1);
    uhDim2 = uhl.getRawDataSize(2);
    uhDimFA = uhl.getRawDataSize(4);
  //DEFINE_GF_MACRO is a bpp macro
    #ifdef UHX
    ERROR : UHXalready defined!
    #else
    #define UHX(i0,i1,i2) uhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hx )))]
    #endif
    #ifdef UHY
    ERROR : UHYalready defined!
    #else
    #define UHY(i0,i1,i2) uhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hy )))]
    #endif
    #ifdef UHZ
    ERROR : UHZalready defined!
    #else
    #define UHZ(i0,i1,i2) uhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hz )))]
    #endif
    #ifdef UMHX
    ERROR : UMHXalready defined!
    #else
    #define UMHX(i0,i1,i2) umhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hx )))]
    #endif
    #ifdef UMHY
    ERROR : UMHYalready defined!
    #else
    #define UMHY(i0,i1,i2) umhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hy )))]
    #endif
    #ifdef UMHZ
    ERROR : UMHZalready defined!
    #else
    #define UMHZ(i0,i1,i2) umhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hz )))]
    #endif
    #ifdef UNHX
    ERROR : UNHXalready defined!
    #else
    #define UNHX(i0,i1,i2) unhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hx )))]
    #endif
    #ifdef UNHY
    ERROR : UNHYalready defined!
    #else
    #define UNHY(i0,i1,i2) unhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hy )))]
    #endif
    #ifdef UNHZ
    ERROR : UNHZalready defined!
    #else
    #define UNHZ(i0,i1,i2) unhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hz )))]
    #endif
    errhp = errhl.Array_Descriptor.Array_View_Pointer3;
    #ifdef ERRHX
    ERROR : ERRHXalready defined!
    #else
    #define ERRHX(i0,i1,i2) errhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hx )))]
    #endif
    #ifdef ERRHY
    ERROR : ERRHYalready defined!
    #else
    #define ERRHY(i0,i1,i2) errhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hy )))]
    #endif
    #ifdef ERRHZ
    ERROR : ERRHZalready defined!
    #else
    #define ERRHZ(i0,i1,i2) errhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*( hz )))]
    #endif
  // #ifdef UNH
  // ERROR : UNH already defined!
  // #else
  // #define UNH(i0,i1,i2,i3) unhp[i0+uhDim0*(i1+uhDim1*(i2+uhDim2*(i3)))]
  // #endif
    xhp = xchl.Array_Descriptor.Array_View_Pointer3;
    xhDim0 = xchl.getRawDataSize(0);
    xhDim1 = xchl.getRawDataSize(1);
    xhDim2 = xchl.getRawDataSize(2);
    #ifdef XHP
    ERROR : XHP already defined!
    #else
    #define XHP(i0,i1,i2,i3) xhp[i0+xhDim0*(i1+xhDim1*(i2+xhDim2*(i3)))]
    #endif
    #ifdef X
    ERROR : X already defined!
    #else
    #define X(i0,i1,i2,i3) xhp[i0+xhDim0*(i1+xhDim1*(i2+xhDim2*(i3)))]
    #endif
  // E Field pointer and array definitions
    uep = uel.Array_Descriptor.Array_View_Pointer3;
    if ( umel.getLength(0) )
    {
        umep = umel.Array_Descriptor.Array_View_Pointer3;
    }
    unep = unel.Array_Descriptor.Array_View_Pointer3;
    ueDim0 = uel.getRawDataSize(0);
    ueDim1 = uel.getRawDataSize(1);
    ueDim2 = uel.getRawDataSize(2);
    ueDimFA = uel.getRawDataSize(4);
  //DEFINE_GF_MACRO is a bpp macro
    #ifdef UEX
    ERROR : UEXalready defined!
    #else
    #define UEX(i0,i1,i2) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ex )))]
    #endif
    #ifdef UEY
    ERROR : UEYalready defined!
    #else
    #define UEY(i0,i1,i2) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ey )))]
    #endif
    #ifdef UEZ
    ERROR : UEZalready defined!
    #else
    #define UEZ(i0,i1,i2) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ez )))]
    #endif
    #ifdef UMEX
    ERROR : UMEXalready defined!
    #else
    #define UMEX(i0,i1,i2) umep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ex )))]
    #endif
    #ifdef UMEY
    ERROR : UMEYalready defined!
    #else
    #define UMEY(i0,i1,i2) umep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ey )))]
    #endif
    #ifdef UMEZ
    ERROR : UMEZalready defined!
    #else
    #define UMEZ(i0,i1,i2) umep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ez )))]
    #endif
    #ifdef UNEX
    ERROR : UNEXalready defined!
    #else
    #define UNEX(i0,i1,i2) unep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ex )))]
    #endif
    #ifdef UNEY
    ERROR : UNEYalready defined!
    #else
    #define UNEY(i0,i1,i2) unep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ey )))]
    #endif
    #ifdef UNEZ
    ERROR : UNEZalready defined!
    #else
    #define UNEZ(i0,i1,i2) unep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ez )))]
    #endif
    errep = errel.Array_Descriptor.Array_View_Pointer3;
    #ifdef ERREX
    ERROR : ERREXalready defined!
    #else
    #define ERREX(i0,i1,i2) errep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ex )))]
    #endif
    #ifdef ERREY
    ERROR : ERREYalready defined!
    #else
    #define ERREY(i0,i1,i2) errep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ey )))]
    #endif
    #ifdef ERREZ
    ERROR : ERREZalready defined!
    #else
    #define ERREZ(i0,i1,i2) errep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*( ez )))]
    #endif
    const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0=maskLocal.getRawDataSize(0);
    const int maskDim1=maskLocal.getRawDataSize(1);
    const int md1=maskDim0, md2=md1*maskDim1; 
    #define MASK(i0,i1,i2) maskp[(i0)+(i1)*md1+(i2)*md2]
  // #ifdef UE
  // ERROR : UE already defined!
  // #else
  // #define UE(i0,i1,i2,i3) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]
  // #endif
    xep = xcel.Array_Descriptor.Array_View_Pointer3;
    xeDim0 = xcel.getRawDataSize(0);
    xeDim1 = xcel.getRawDataSize(1);
    xeDim2 = xcel.getRawDataSize(2);
    #ifdef XEP
    ERROR : XEP already defined!
    #else
    #define XEP(i0,i1,i2,i3) xep[i0+xeDim0*(i1+xeDim1*(i2+xeDim2*(i3)))]
    #endif

    Index J1,J2,J3;
    int extra=0;  // evaluate forcing on this many ghost points
    if( method==sosup ) 
        extra = orderOfAccuracyInSpace/2;  // sosup needs forcing on extra ghost lines


    if( forcingOption==twilightZoneForcing || forcingOption==gaussianSource ||
            forcingOption==gaussianChargeSource )
    {
        
    // we always need the center array for TZ forcing, otherwise not needed for rectangular grids:
        realArray & x = (isRectangular && forcingOption!=twilightZoneForcing) ? u : mg.center();

        OV_GET_SERIAL_ARRAY(real,u,uLocal);
        OV_GET_SERIAL_ARRAY(real,x,xLocal);

        real *up = uLocal.Array_Descriptor.Array_View_Pointer3;
        const int uDim0=uLocal.getRawDataSize(0);
        const int uDim1=uLocal.getRawDataSize(1);
        const int uDim2=uLocal.getRawDataSize(2);
#undef U
#define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]

    //      real *xp = xLocal.Array_Descriptor.Array_View_Pointer3;
    //       const int xDim0=xLocal.getRawDataSize(0);
    //       const int xDim1=xLocal.getRawDataSize(1);
    //       const int xDim2=xLocal.getRawDataSize(2);
    //       #undef X
    //       #define X(i0,i1,i2,i3) xp[i0+xDim0*(i1+xDim1*(i2+xDim2*(i3)))]

        Index I1,I2,I3;

        getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
    // For working with local arrays (parallel):
        Index J1,J2,J3;

    // const realArray & x = mg.center();
    // Range all;
    // const bool isRectangular = mg.isRectangular();

        if( forcingOption==twilightZoneForcing )
        {
            assert( tz!=NULL );
            OGFunction & e = *tz;

            const real csq=c*c;
            const real dtsqby12=dt*dt/12.;

            Range C(ex,hz);
            const int ntd=2, nxd=0, nyd=0, nzd=0; // we need the 2nd time derivative


            bool useScalarLoops=false;
#ifdef USE_PPP
            useScalarLoops=true;
#endif 

            if( TRUE && method==nfdtd )
            {
                if( t<= 2.*dt )
        	  printF("--MX-FORCE-- add TZ forcing (*new opt way*) option=%i orderOfAccuracyInSpace=%i\n",option,
                                  orderOfAccuracyInSpace);

	// *********** NEW WAY -- JAN 2017 ***********
        // This should be more efficient
                const bool isRectangular = false; // ** do this for now ** mg.isRectangular();

      	if( option==1 )
      	{
        	  uLocal(I1,I2,I3,C)=0.;
      	}
      	Range E(ex,ex+numberOfDimensions-1); // electric field components
      	RealArray utt(I1,I2,I3,C), uxx(I1,I2,I3,C), uyy(I1,I2,I3,C);
        	  
      	e.gd( utt ,xLocal,numberOfDimensions,isRectangular,2,0,0,0,I1,I2,I3,C,t);
      	e.gd( uxx ,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,C,t);
      	e.gd( uyy ,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,C,t);

                if( numberOfDimensions==2 )
      	{
        	  uLocal(I1,I2,I3,C) += utt - csq*(uxx+uyy);
      	}
      	else
      	{
                    RealArray uzz(I1,I2,I3,C);
        	  e.gd( uzz ,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,C,t);

        	  uLocal(I1,I2,I3,C) += utt - csq*(uxx+uyy+uzz);
      	}
      	if( orderOfAccuracyInSpace>=4 )
      	{
          // --- add on forcing corrections for modified equation time-stepping ----
          //
          //       u_tt = c^2 Delta u + f
          // Modified equation:
          //    D+tD-t U = u_tt + (dt^2)/12* u_tttt + ...
          //             = c^2 Delta u + (dt^2)/12*( c^4 (Delta)^2 u ) + F 
          // where 
          //     F = f + (dt^2)/12*( c^2 Delta f + f_tt )
	  // 
          //    u_tt = c^2 Delta u + f
          //    u_tttt = (c^2 Delta u + f)_tt = c^2 Delta u_tt + f_tt
          //           = c^4 (Delta)^2 u + c^2 Delta f + f_tt
          // TZ forcing:
          //     f = w_tt - c^2 Delta w   where w=TZ solution 
          // Total forcing
          //     F = w_tt - c^2 Delta w + (dt^2/12)*[ c^2 Delta w_tt - (c^2 Delta)^2 w + w_tttt - c^2 Delta w_tt ]
          //       = w_tt - c^2 Delta w + (dt^2/12)*[ w_tttt - (c^2 Delta)^2 w ]

        	  const real csq2=csq*csq;
	  /// RealArray &utttt=utt, &uxxxx=uxx, &uyyyy=uyy, uxxyy(I1,I2,I3,C);
            	  RealArray utttt(I1,I2,I3,C), uxxxx(I1,I2,I3,C), uyyyy(I1,I2,I3,C), uxxyy(I1,I2,I3,C);
        	  
        	  e.gd( utttt ,xLocal,numberOfDimensions,isRectangular,4,0,0,0,I1,I2,I3,C,t);
        	  e.gd( uxxxx ,xLocal,numberOfDimensions,isRectangular,0,4,0,0,I1,I2,I3,C,t);
        	  e.gd( uxxyy ,xLocal,numberOfDimensions,isRectangular,0,2,2,0,I1,I2,I3,C,t);
        	  e.gd( uyyyy ,xLocal,numberOfDimensions,isRectangular,0,0,4,0,I1,I2,I3,C,t);
	  // e.gd( uyyyy ,xLocal,numberOfDimensions,isRectangular,0,0,4,0,I1,I2,I3,C,t);

                    uLocal(I1,I2,I3,C) += ( utttt - (csq2)*( uxxxx + 2.*uxxyy + uyyyy) )*dtsqby12;

          // ::display(uxxxx,"uxxxx");
          // ::display(e.gd(0,4,0,0,mg,I1,I2,I3,C,t),"e.gd(0,4,0,0,mg,I1,I2,I3,C,t)");
          // ::display(uyyyy,"uyyyy");
          // ::display(e.gd(0,0,4,0,mg,I1,I2,I3,C,t),"e.gd(0,0,4,0,mg,I1,I2,I3,C,t)");
        	  

	  // u(I1,I2,I3,C)+= (utttt
	  // 		   - (csq*csq)*(uxxxx
	  // 				+e.gd(0,0,4,0,mg,I1,I2,I3,C,t)
	  // 				+2.*uxxyy ) )*dtsqby12;

	  // u(I1,I2,I3,C)+= (e.gd(4,0,0,0,mg,I1,I2,I3,C,t) 
	  // 		   - (csq*csq)*(e.gd(0,4,0,0,mg,I1,I2,I3,C,t)
	  // 				+e.gd(0,0,4,0,mg,I1,I2,I3,C,t)
	  // 				+2.*e.gd(0,2,2,0,mg,I1,I2,I3,C,t) ) )*dtsqby12;
        	  if( numberOfDimensions==3 )
        	  {
          	    RealArray &uxxzz=utt, &uyyzz=uxx, &uzzzz=uyy;

          	    e.gd( uxxzz ,xLocal,numberOfDimensions,isRectangular,0,2,0,2,I1,I2,I3,C,t);
          	    e.gd( uyyzz ,xLocal,numberOfDimensions,isRectangular,0,0,2,2,I1,I2,I3,C,t);
          	    e.gd( uzzzz ,xLocal,numberOfDimensions,isRectangular,0,0,0,4,I1,I2,I3,C,t);

          	    uLocal(I1,I2,I3,C) += (-csq2*dtsqby12)*( 2.*(uxxzz + uyyzz) + uzzzz);
        	  }
        	  
        	  if( orderOfAccuracyInSpace>=6 ) 
        	  {
          	    const real dt4by360=dt*dt*dt*dt/360.;
                        const real csq3 = csq*csq*csq;
                    	    RealArray &utttttt=utt, &uxxxxxx=uxx, &uyyyyyy=uyy, uxxxxyy(I1,I2,I3,C), uxxyyyy(I1,I2,I3,C);
          	    
          	    e.gd( utttttt ,xLocal,numberOfDimensions,isRectangular,6,0,0,0,I1,I2,I3,C,t);
          	    e.gd( uxxxxxx ,xLocal,numberOfDimensions,isRectangular,0,6,0,0,I1,I2,I3,C,t);
          	    e.gd( uxxxxyy ,xLocal,numberOfDimensions,isRectangular,0,4,2,0,I1,I2,I3,C,t);
          	    e.gd( uxxyyyy ,xLocal,numberOfDimensions,isRectangular,0,2,4,0,I1,I2,I3,C,t);
          	    e.gd( uyyyyyy ,xLocal,numberOfDimensions,isRectangular,0,0,6,0,I1,I2,I3,C,t);

          	    uLocal(I1,I2,I3,C) += ( utttttt - csq3*( uxxxxxx + 3.*(uxxxxyy+uxxyyyy) + uyyyyyy) )*dt4by360;

          	    if( numberOfDimensions==3 )
          	    {
            	      RealArray &uxxxxzz=utt, &uyyyyzz=uxx, &uzzzzzz=uyy, &uxxzzzz=uxxxxyy, &uyyzzzz=uxxyyyy, uxxyyzz(I1,I2,I3,C);

            	      e.gd( uxxxxzz ,xLocal,numberOfDimensions,isRectangular,0,4,0,2,I1,I2,I3,C,t);
            	      e.gd( uyyyyzz ,xLocal,numberOfDimensions,isRectangular,0,0,4,2,I1,I2,I3,C,t);
            	      e.gd( uxxzzzz ,xLocal,numberOfDimensions,isRectangular,0,2,0,4,I1,I2,I3,C,t);
            	      e.gd( uyyzzzz ,xLocal,numberOfDimensions,isRectangular,0,0,2,4,I1,I2,I3,C,t);
            	      e.gd( uzzzzzz ,xLocal,numberOfDimensions,isRectangular,0,0,0,6,I1,I2,I3,C,t);
            	      e.gd( uxxyyzz ,xLocal,numberOfDimensions,isRectangular,0,2,2,2,I1,I2,I3,C,t);

            	      e.gd( uxxyyzz ,xLocal,numberOfDimensions,isRectangular,0,2,2,2,I1,I2,I3,C,t);

            	      uLocal(I1,I2,I3,C) += (-csq3*dt4by360)*( 3.*(uxxxxzz + uyyyyzz+ uxxzzzz + uyyzzzz) +6.*uxxyyzz + uzzzzzz );
          	    }

        	  }
        	  
      	}
      	
      	if( dispersionModel != noDispersion )
      	{
	  // -- Dispersion model --
                    const DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
        	  const real eps=epsGrid(grid), gamma=dmp.gamma, omegap=dmp.omegap; 

        	  Range P(pxc,pxc+numberOfDimensions-1);
          	    
        	  if( option==1 )
        	  {
          	    uLocal(I1,I2,I3,P)=0.;
        	  }

        	  RealArray ptt(I1,I2,I3,P), pt(I1,I2,I3,P);
                    RealArray &ue=utt; // reuse space 

        	  e.gd( ptt ,xLocal,numberOfDimensions,isRectangular,2,0,0,0,I1,I2,I3,P,t);  // P_tt
        	  e.gd( pt  ,xLocal,numberOfDimensions,isRectangular,1,0,0,0,I1,I2,I3,P,t);  // P_t 
        	  e.gd( ue  ,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,E,t);  // E

        	  uLocal(I1,I2,I3,E) += (1./eps)*ptt;
        	  uLocal(I1,I2,I3,P) += ptt + gamma*pt - (omegap*omegap)*ue(I1,I2,I3,E);
      	}

            }


            else if( useScalarLoops || method==dsiMatVec || method==dsi )
            {
	// ********************** this is slow -- fix *****************

      	int i1,i2,i3;
      	real x0,y0,z0=0.;
        	  
      	if( option==0 || option==2 )
      	{
        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	      forcingLoops2D(U(i1,i2,i3,CC) += e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t) //                                                - csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)););
          	    if( method==nfdtd || method==sosup )
          	    {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                          }
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                          }
          	    }
          	    else
          	    {
                        if ( (option==0 || option==1 ) )
                        {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - e.y(x0,y0,z0,hz,t)/eps;
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 + e.x(x0,y0,z0,hz,t)/eps;
                          }
                        }
                        else
                        {
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + e.x( x0,y0,z0, ey, t)/mu - e.y( x0,y0,z0, ex, t)/mu;
                          }
                        }
          	    }
        	  }
        	  else
        	  {
	    //	      forcingLoops3D(U(i1,i2,i3,CC) += e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t) 	    //			     - csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t)););
          	    if ( method==nfdtd || method==sosup )
          	    {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              z0 = XEP(i1,i2,i3,2);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ez
                              U(i1,i2,i3,CC)+=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                          }
          	    }
          	    else
          	    {
                          if ( (option==0 || option==1 ) )
                              {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              z0 = XEP(i1,i2,i3,2);
            //    #undef CC
            //    #define CC ex
            //    e1
            //    #undef CC
            //    #define CC ey
            //    e1
                                #undef CC
                                #define CC ex
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - (e.y(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hy,t))/eps;
                                #undef CC
                                #define CC ey
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 + (e.x(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hx,t))/eps;
                                #undef CC
                                #define CC ez
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - (e.x(x0,y0,z0,hy,t)-e.y(x0,y0,z0,hx,t))/eps;
                          }
                              }
                          else
                              {
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                              {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              z0 = XHP(i1,i2,i3,2);
                              #undef CC
                              #define CC hx
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + (e.y( x0,y0,z0, ez, t) - e.z( x0,y0,z0, ey, t))/mu;
                              #undef CC
                              #define CC hy
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) - (e.x( x0,y0,z0, ez, t) - e.z( x0,y0,z0, ex, t))/mu;
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)+= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + (e.x( x0,y0,z0, ey, t) - e.y( x0,y0,z0, ex, t))/mu;
                              }
                              }
          	    }
        	  }
      	}
      	else
      	{
        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	      forcingLoops2D(U(i1,i2,i3,CC) = e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t) //                                                - csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)););
          	    if ( method==nfdtd || method==sosup )
          	    {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                          }
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t));
                          }
          	    }
          	    else
          	    {
                        if ( (option==0 || option==1 ) )
                        {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - e.y(x0,y0,z0,hz,t)/eps;
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 + e.x(x0,y0,z0,hz,t)/eps;
                          }
                        }
                        else
                        {
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + e.x( x0,y0,z0, ey, t)/mu - e.y( x0,y0,z0, ex, t)/mu;
                          }
                        }
          	    }
        	  }
        	  else
        	  {
// 	      forcingLoops3D(U(i1,i2,i3,CC) = e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t) 	    //			     - csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t)););
          	    if ( method==nfdtd || method==sosup )
          	    {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              z0 = XEP(i1,i2,i3,2);
                              #undef CC
                              #define CC ex
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ey
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                              #define CC ez
                              U(i1,i2,i3,CC)=e.gd(ntd,nxd,nyd,nzd,x0,y0,z0,CC,t)			-csq*(e.xx(x0,y0,z0,CC,t)+e.yy(x0,y0,z0,CC,t)+e.zz(x0,y0,z0,CC,t));
                              #undef CC
                          }
          	    }
          	    else
          	    {
                          if ( (option==0 || option==1 ) )
                              {
                          J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                          J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                          J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                          {
                              x0 = XEP(i1,i2,i3,0);
                              y0 = XEP(i1,i2,i3,1);
                              z0 = XEP(i1,i2,i3,2);
            //    #undef CC
            //    #define CC ex
            //    e1
            //    #undef CC
            //    #define CC ey
            //    e1
                                #undef CC
                                #define CC ex
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - (e.y(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hy,t))/eps;
                                #undef CC
                                #define CC ey
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 + (e.x(x0,y0,z0,hz,t)-e.z(x0,y0,z0,hx,t))/eps;
                                #undef CC
                                #define CC ez
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) 				 - (e.x(x0,y0,z0,hy,t)-e.y(x0,y0,z0,hx,t))/eps;
                          }
                              }
                          else
                              {
                          J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                          J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                          J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                          FOR_3D(i1,i2,i3,J1,J2,J3)
                              {
                              x0 = XHP(i1,i2,i3,0);
                              y0 = XHP(i1,i2,i3,1);
                              z0 = XHP(i1,i2,i3,2);
                              #undef CC
                              #define CC hx
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + (e.y( x0,y0,z0, ez, t) - e.z( x0,y0,z0, ey, t))/mu;
                              #undef CC
                              #define CC hy
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) - (e.x( x0,y0,z0, ez, t) - e.z( x0,y0,z0, ex, t))/mu;
                              #undef CC
                              #define CC hz
                              U(i1,i2,i3,CC)= e.gd(1,nxd,nyd,nzd,x0,y0,z0,CC,t) + (e.x( x0,y0,z0, ey, t) - e.y( x0,y0,z0, ex, t))/mu;
                              }
                              }
          	    }
        	  }
      	}
      	if( orderOfAccuracyInSpace>=4 && method!=sosup )
      	{// add forcing for methods that are 4th order or higher
        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	      forcingLoops2D(U(i1,i2,i3,CC)+= (e.gd(4,0,0,0,x0,y0,z0,CC,t) // 						 - (csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)// 							      +e.gd(0,0,4,0,x0,y0,z0,CC,t)// 							      +2.*e.gd(0,2,2,0,x0,y0,z0,CC,t) ) )*dtsqby12;);
                      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                      FOR_3D(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XEP(i1,i2,i3,0);
                          y0 = XEP(i1,i2,i3,1);
                          #undef CC
                          #define CC ex
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+2.*e.gd(0,2,2,0,x0,y0,z0,CC,t)))*dtsqby12;
                          #undef CC
                          #define CC ey
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+2.*e.gd(0,2,2,0,x0,y0,z0,CC,t)))*dtsqby12;
                      }
                      J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                      J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                      J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                      FOR_3(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XHP(i1,i2,i3,0);
                          y0 = XHP(i1,i2,i3,1);
                          #undef CC
                          #define CC hz
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+2.*e.gd(0,2,2,0,x0,y0,z0,CC,t)))*dtsqby12;
                      }
        	  }
        	  else
        	  {
// 	      forcingLoops3D(U(i1,i2,i3,CC)+=    (e.gd(4,0,0,0,x0,y0,z0,CC,t) // 						    - (csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)// 								 +e.gd(0,0,4,0,x0,y0,z0,CC,t)// 								 +e.gd(0,0,0,4,x0,y0,z0,CC,t)// 								 +2.*(e.gd(0,2,2,0,x0,y0,z0,CC,t)// 								      +e.gd(0,2,0,2,x0,y0,z0,CC,t)// 								      +e.gd(0,0,2,2,x0,y0,z0,CC,t)) ) )*dtsqby12;);
                      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                      FOR_3D(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XEP(i1,i2,i3,0);
                          y0 = XEP(i1,i2,i3,1);
                          z0 = XEP(i1,i2,i3,2);
                          #undef CC
                          #define CC ex
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+e.gd(0,0,0,4,x0,y0,z0,CC,t)							+2.*(e.gd(0,2,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,0,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,2,x0,y0,z0,CC,t))))*dtsqby12;
                          #undef CC
                          #define CC ey
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+e.gd(0,0,0,4,x0,y0,z0,CC,t)							+2.*(e.gd(0,2,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,0,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,2,x0,y0,z0,CC,t))))*dtsqby12;
                          #undef CC
                          #define CC ez
                          U(i1,i2,i3,CC)+=(e.gd(4,0,0,0,x0,y0,z0,CC,t)					-(csq*csq)*(e.gd(0,4,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,4,0,x0,y0,z0,CC,t)							+e.gd(0,0,0,4,x0,y0,z0,CC,t)							+2.*(e.gd(0,2,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,0,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,2,x0,y0,z0,CC,t))))*dtsqby12;
                          #undef CC
                      }
        	  }
      	}
      	
      	if( orderOfAccuracyInSpace>=6 && method!=sosup )
      	{ // add forcing for methods that are 6th order or higher
        	  real dt4by360=dt*dt*dt*dt/360.;
        	  
        	  if( mg.numberOfDimensions()==2 )
        	  {
// 	      forcingLoops2D(U(i1,i2,i3,CC)+= (e.gd(6,0,0,0,x0,y0,z0,CC,t) // 						 - (csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)// 								  +e.gd(0,0,6,0,x0,y0,z0,CC,t)// 								  +3.*e.gd(0,2,4,0,x0,y0,z0,CC,t) // 								  +3.*e.gd(0,4,2,0,x0,y0,z0,CC,t) ) )*dt4by360;);

                      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                      FOR_3D(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XEP(i1,i2,i3,0);
                          y0 = XEP(i1,i2,i3,1);
                          #undef CC
                          #define CC ex
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,6,0,x0,y0,z0,CC,t)							+3.*e.gd(0,2,4,0,x0,y0,z0,CC,t)							+3.*e.gd(0,4,2,0,x0,y0,z0,CC,t)))*dt4by360;
                          #undef CC
                          #define CC ey
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,6,0,x0,y0,z0,CC,t)							+3.*e.gd(0,2,4,0,x0,y0,z0,CC,t)							+3.*e.gd(0,4,2,0,x0,y0,z0,CC,t)))*dt4by360;
                      }
                      J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                      J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                      J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                      FOR_3(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XHP(i1,i2,i3,0);
                          y0 = XHP(i1,i2,i3,1);
                          #undef CC
                          #define CC hz
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)							+e.gd(0,0,6,0,x0,y0,z0,CC,t)							+3.*e.gd(0,2,4,0,x0,y0,z0,CC,t)							+3.*e.gd(0,4,2,0,x0,y0,z0,CC,t)))*dt4by360;
                      }
        	  }
        	  else
        	  {
// 	      forcingLoops3D(U(i1,i2,i3,CC)+=    (e.gd(6,0,0,0,x0,y0,z0,CC,t) // 						    - (csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)// 								     +e.gd(0,0,6,0,x0,y0,z0,CC,t)// 								     +e.gd(0,0,0,6,x0,y0,z0,CC,t)// 								     +3.*( e.gd(0,4,2,0,x0,y0,z0,CC,t)// 									   +e.gd(0,2,4,0,x0,y0,z0,CC,t)// 									   +e.gd(0,4,0,2,x0,y0,z0,CC,t)// 									   +e.gd(0,2,0,4,x0,y0,z0,CC,t)// 									   +e.gd(0,0,4,2,x0,y0,z0,CC,t)// 									   +e.gd(0,0,2,4,x0,y0,z0,CC,t) )// 								     +6.*e.gd(0,2,2,2,x0,y0,z0,CC,t)// 						      ) )*dt4by360;);
                      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                      FOR_3D(i1,i2,i3,J1,J2,J3)
                      {
                          x0 = XEP(i1,i2,i3,0);
                          y0 = XEP(i1,i2,i3,1);
                          z0 = XEP(i1,i2,i3,2);
                          #undef CC
                          #define CC ex
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)								+e.gd(0,0,6,0,x0,y0,z0,CC,t)								+e.gd(0,0,0,6,x0,y0,z0,CC,t)								+3.*(e.gd(0,4,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,4,0,x0,y0,z0,CC,t)								+e.gd(0,4,0,2,x0,y0,z0,CC,t)								+e.gd(0,2,0,4,x0,y0,z0,CC,t)								+e.gd(0,0,4,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,4,x0,y0,z0,CC,t))								+6.*e.gd(0,2,2,2,x0,y0,z0,CC,t)						))*dt4by360;
                          #undef CC
                          #define CC ey
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)								+e.gd(0,0,6,0,x0,y0,z0,CC,t)								+e.gd(0,0,0,6,x0,y0,z0,CC,t)								+3.*(e.gd(0,4,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,4,0,x0,y0,z0,CC,t)								+e.gd(0,4,0,2,x0,y0,z0,CC,t)								+e.gd(0,2,0,4,x0,y0,z0,CC,t)								+e.gd(0,0,4,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,4,x0,y0,z0,CC,t))								+6.*e.gd(0,2,2,2,x0,y0,z0,CC,t)						))*dt4by360;
                          #undef CC
                          #define CC ez
                          U(i1,i2,i3,CC)+=(e.gd(6,0,0,0,x0,y0,z0,CC,t)					-(csq*csq*csq)*(e.gd(0,6,0,0,x0,y0,z0,CC,t)								+e.gd(0,0,6,0,x0,y0,z0,CC,t)								+e.gd(0,0,0,6,x0,y0,z0,CC,t)								+3.*(e.gd(0,4,2,0,x0,y0,z0,CC,t)								+e.gd(0,2,4,0,x0,y0,z0,CC,t)								+e.gd(0,4,0,2,x0,y0,z0,CC,t)								+e.gd(0,2,0,4,x0,y0,z0,CC,t)								+e.gd(0,0,4,2,x0,y0,z0,CC,t)								+e.gd(0,0,2,4,x0,y0,z0,CC,t))								+6.*e.gd(0,2,2,2,x0,y0,z0,CC,t)						))*dt4by360;
                          #undef CC
                      }
        	  }
      	}

      	
            }
            else // ** Old way **
            {
                if( t<= dt )
                  printF("--MX-FORCE-- add TZ forcing (array version)\n");
        



      	Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
      	Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);
      	
      	if( option==0 || option==2 )
      	{
                    if( solveForElectricField ) 
          	    u(I1,I2,I3,Ce)+= e.gd(ntd,nxd,nyd,nzd,mg,I1,I2,I3,Ce,tE) - csq*e.laplacian(mg,I1,I2,I3,Ce,tE);
                    if( solveForMagneticField )
              	    u(I1,I2,I3,Ch)+= e.gd(ntd,nxd,nyd,nzd,mg,I1,I2,I3,Ch,tH) - csq*e.laplacian(mg,I1,I2,I3,Ch,tH);
      	}
      	else
      	{
        	  u(I1,I2,I3,C) = e.gd(ntd,nxd,nyd,nzd,mg,I1,I2,I3,C,t) - csq*e.laplacian(mg,I1,I2,I3,C,t);
// 	      u(I1,I2,I3,Ce) = e.gd(ntd,nxd,nyd,nzd,mg,Ie1,Ie2,Ie3,Ce,tE) - csq*e.laplacian(mg,Ie1,Ie2,Ie3,Ce,tE);
// 	      u(I1,I2,I3,Ch) = e.gd(ntd,nxd,nyd,nzd,mg,Ih1,Ih2,Ih3,Ch,tH) - csq*e.laplacian(mg,Ih1,Ih2,Ih3,Ch,tH);
      	}

	// printF(" option=%i csq=%e \n",option,csq);
        	  
	// display(e.gd(ntd,nxd,nyd,nzd,mg,I1,I2,I3,C,t)," e.tt","%5.2f ");
	// display(e.laplacian(mg,I1,I2,I3,C,t),"e.laplacian(mg,I1,I2,I3,C,t)","%5.2f ");

	// display(u(I1,I2,I3,C),"getForcing: RHS after first assignment","%5.2f ");

      	if( timeSteppingMethod == modifiedEquationTimeStepping && method!=sosup )
      	{
        	  if( orderOfAccuracyInSpace>=4 )
        	  {
          	    if( mg.numberOfDimensions()==2 )
          	    {
            	      u(I1,I2,I3,C)+= (e.gd(4,0,0,0,mg,I1,I2,I3,C,t) 
                         			       - (csq*csq)*(e.gd(0,4,0,0,mg,I1,I2,I3,C,t)
                                  					    +e.gd(0,0,4,0,mg,I1,I2,I3,C,t)
                                  					    +2.*e.gd(0,2,2,0,mg,I1,I2,I3,C,t) ) )*dtsqby12;

	      // display(u(I1,I2,I3,C),"getForcing: RHS after first assignment","%5.2f ");

          	    }
          	    else
          	    {
                            Range C(ex,ez);
            	      u(I1,I2,I3,C)+=(e.gd(4,0,0,0,mg,I1,I2,I3,C,t) 
                        			      - (csq*csq)*(e.gd(0,4,0,0,mg,I1,I2,I3,C,t)
                                 					   +e.gd(0,0,4,0,mg,I1,I2,I3,C,t)
                                 					   +e.gd(0,0,0,4,mg,I1,I2,I3,C,t)
                                 					   +2.*(e.gd(0,2,2,0,mg,I1,I2,I3,C,t)
                                    						+e.gd(0,2,0,2,mg,I1,I2,I3,C,t)
                                    						+e.gd(0,0,2,2,mg,I1,I2,I3,C,t)) ) )*dtsqby12;
          	    }
        	  }
      	
        	  if( orderOfAccuracyInSpace>=6 )
        	  {
          	    real dt4by360=dt*dt*dt*dt/360.;
        	  
          	    if( mg.numberOfDimensions()==2 )
          	    {
            	      if( false )
            	      {
            		display(e.gd(6,0,0,0,mg,I1,I2,I3,hz,t)," exact tttttt",debugFile,"%9.2e ");
            		display(e.gd(0,6,0,0,mg,I1,I2,I3,hz,t)," exact xxxxxx",debugFile,"%9.2e ");
            		display(e.gd(0,2,4,0,mg,I1,I2,I3,hz,t)," exact xxyyyy",debugFile,"%9.2e ");
            		display(e.gd(0,4,2,0,mg,I1,I2,I3,hz,t)," exact xxxxyy",debugFile,"%9.2e ");
            		display(e.gd(0,0,6,0,mg,I1,I2,I3,hz,t)," exact yyyyyy",debugFile,"%9.2e ");
            	      }
            		

            	      u(I1,I2,I3,C)+= (e.gd(6,0,0,0,mg,I1,I2,I3,C,t) 
                         			       - (csq*csq*csq)*(e.gd(0,6,0,0,mg,I1,I2,I3,C,t)
                                    						+e.gd(0,0,6,0,mg,I1,I2,I3,C,t)
                                    						+3.*e.gd(0,2,4,0,mg,I1,I2,I3,C,t) 
                                    						+3.*e.gd(0,4,2,0,mg,I1,I2,I3,C,t) ) )*dt4by360;
          	    }
          	    else
          	    {
            	      u(I1,I2,I3,C)+=    (e.gd(6,0,0,0,mg,I1,I2,I3,C,t) 
                          				  - (csq*csq*csq)*(e.gd(0,6,0,0,mg,I1,I2,I3,C,t)
                                       						   +e.gd(0,0,6,0,mg,I1,I2,I3,C,t)
                                       						   +e.gd(0,0,0,6,mg,I1,I2,I3,C,t)
                                       						   +3.*( e.gd(0,4,2,0,mg,I1,I2,I3,C,t)
                                           							 +e.gd(0,2,4,0,mg,I1,I2,I3,C,t)
                                           							 +e.gd(0,4,0,2,mg,I1,I2,I3,C,t)
                                           							 +e.gd(0,2,0,4,mg,I1,I2,I3,C,t)
                                           							 +e.gd(0,0,4,2,mg,I1,I2,I3,C,t)
                                           							 +e.gd(0,0,2,4,mg,I1,I2,I3,C,t) )
                                       						   +6.*e.gd(0,2,2,2,mg,I1,I2,I3,C,t)
                            				    ) )*dt4by360;
          	    }
        	  }
      	
      	

      	}
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
            const real ctE=cos(2.*Pi*omega*(tE-t0));
            const real stE=sin(2.*Pi*omega*(tE-t0));
            const real ctH=cos(2.*Pi*omega*(tH-t0));
            const real stH=sin(2.*Pi*omega*(tH-t0));

            const bool isRectangular = mg.isRectangular();

            if( debug & 4 )
      	printF(" addForcing:gaussianSource: beta=%8.2e omega=%8.2e x0=(%8.2e,%8.2e,%8.2e) \n",
             	       beta,omega,x0,y0,z0);

            if( !isRectangular )
            {
            
      	realSerialArray f3E(Ie1,Ie2,Ie3),f3H(Ih1,Ih2,Ih3);

      	if( mg.numberOfDimensions()==2 )
      	{
        	  f3H=exp( -beta*( SQR(xh(Ih1,Ih2,Ih3)-x0)+SQR(yh(Ih1,Ih2,Ih3)-y0) ) );
        	  f3E=exp( -beta*( SQR(xe(Ie1,Ie2,Ie3)-x0)+SQR(ye(Ie1,Ie2,Ie3)-y0) ) );

        	  if( option==0 )
        	  {
          	    uLocal(Ih1,Ih2,Ih3,hz)+=( 2.*Pi*omega*ctH)*f3H;
          	    uLocal(Ie1,Ie2,Ie3,ex)+=(-2.*beta*stE)*(ye(Ie1,Ie2,Ie3)-y0)*f3E;
          	    uLocal(Ie1,Ie2,Ie3,ey)+=( 2.*beta*stE)*(xe(Ie1,Ie2,Ie3)-x0)*f3E;
        	  }
        	  else
        	  {
          	    uLocal(Ih1,Ih2,Ih3,hz)=( 2.*Pi*omega*ctH)*f3H;
          	    uLocal(Ie1,Ie2,Ie3,ex)=(-2.*beta*stE)*(ye(Ie1,Ie2,Ie3)-y0)*f3E;
          	    uLocal(Ie1,Ie2,Ie3,ey)=( 2.*beta*stE)*(xe(Ie1,Ie2,Ie3)-x0)*f3E;
        	  }
// 	    if( option==0 )
// 	    {
// 	      u(I1,I2,I3,hz)+=( 2.*Pi*omega*ct)*f3;
// 	      u(I1,I2,I3,ex)+=(-2.*beta*st)*(x(I1,I2,I3,1)-y0)*f3;
// 	      u(I1,I2,I3,ey)+=( 2.*beta*st)*(x(I1,I2,I3,0)-x0)*f3;
// 	    }
// 	    else
// 	    {
// 	      u(I1,I2,I3,hz)=( 2.*Pi*omega*ct)*f3;
// 	      u(I1,I2,I3,ex)=(-2.*beta*st)*(x(I1,I2,I3,1)-y0)*f3;
// 	      u(I1,I2,I3,ey)=( 2.*beta*st)*(x(I1,I2,I3,0)-x0)*f3;
// 	    }
      	}
      	else // *** 3D ***
      	{
	  // scale by beta*beta to make O(1)
        	  f3E(I1,I2,I3)=(beta*beta*ctE)*exp( -beta*( SQR(xe(I1,I2,I3)-x0)+SQR(ye(I1,I2,I3)-y0)+SQR(ze(I1,I2,I3)-z0) ) );
        	  if( option==0 )
        	  {
          	    uLocal(I1,I2,I3,ex)+=( (ze(I1,I2,I3)-z0)-(ye(I1,I2,I3)-y0) )*f3E(I1,I2,I3);
          	    uLocal(I1,I2,I3,ey)+=( (xe(I1,I2,I3)-x0)-(ze(I1,I2,I3)-z0) )*f3E(I1,I2,I3);
          	    uLocal(I1,I2,I3,ez)+=( (ye(I1,I2,I3)-y0)-(xe(I1,I2,I3)-x0) )*f3E(I1,I2,I3);
        	  }
        	  else
        	  {
          	    uLocal(I1,I2,I3,ex)=( (ze(I1,I2,I3)-z0)-(ye(I1,I2,I3)-y0) )*f3E(I1,I2,I3);
          	    uLocal(I1,I2,I3,ey)=( (xe(I1,I2,I3)-x0)-(ze(I1,I2,I3)-z0) )*f3E(I1,I2,I3);
          	    uLocal(I1,I2,I3,ez)=( (ye(I1,I2,I3)-y0)-(xe(I1,I2,I3)-x0) )*f3E(I1,I2,I3);
        	  }
      	}
        	  
            }
            else
            {
      	real dx[3],xab[2][3];
      	mg.getRectangularGridParameters( dx, xab );

      	const int i0a=mg.gridIndexRange(0,0);
      	const int i1a=mg.gridIndexRange(0,1);
      	const int i2a=mg.gridIndexRange(0,2);

      	const real xa=xab[0][0], dx0=dx[0];
      	const real ya=xab[0][1], dy0=dx[1];
      	const real za=xab[0][2], dz0=dx[2];
      	
#define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define X2(i0,i1,i2) (za+dz0*(i2-i2a))

      	int i1,i2,i3;
      	real xd,yd,zd,f3;
      	Index J1,J2,J3;
      	if( mg.numberOfDimensions()==2 )
      	{
        	  if( option==0 )
        	  {
	    //XXX not fully implemented for staggered schemes yet!
          	    J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
          	    J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
          	    J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=X0(i1,i2,i3)-x0;
            	      yd=X1(i1,i2,i3)-y0;
          	    
            	      f3=exp( -beta*( xd*xd+yd*yd ) );
            	      U(i1,i2,i3,hz)+=( 2.*Pi*omega*ctH)*f3;
            	      U(i1,i2,i3,ex)+=(-2.*beta*stE)*yd*f3;
            	      U(i1,i2,i3,ey)+=( 2.*beta*stE)*xd*f3;
          	    }
        	  }
        	  else
        	  {
          	    J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
          	    J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
          	    J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=X0(i1,i2,i3)-x0;
            	      yd=X1(i1,i2,i3)-y0;
          	    
            	      f3=exp( -beta*( xd*xd+yd*yd ) );
            	      U(i1,i2,i3,hz)=( 2.*Pi*omega*ctH)*f3;
            	      U(i1,i2,i3,ex)=(-2.*beta*stE)*yd*f3;
            	      U(i1,i2,i3,ey)=( 2.*beta*stE)*xd*f3;
          	    }
        	  
        	  }
      	}
      	else // 3D
      	{
	  // scale by beta*beta to make O(1)
        	  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
        	  real betaSqct=beta*beta*ctE;
        	  if( option==0 )
        	  {
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=X0(i1,i2,i3)-x0;
            	      yd=X1(i1,i2,i3)-y0;
            	      zd=X2(i1,i2,i3)-z0;
          	    
            	      f3=betaSqct*exp( -beta*( xd*xd+yd*yd+zd*zd ) );
            	      U(i1,i2,i3,ex)+=( zd-yd )*f3;
            	      U(i1,i2,i3,ey)+=( xd-zd )*f3;
            	      U(i1,i2,i3,ez)+=( yd-xd )*f3;
          	    }
        	  }
        	  else
        	  {
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=X0(i1,i2,i3)-x0;
            	      yd=X1(i1,i2,i3)-y0;
            	      zd=X2(i1,i2,i3)-z0;
          	    
            	      f3=betaSqct*exp( -beta*( xd*xd+yd*yd+zd*zd ) );
            	      U(i1,i2,i3,ex)=( zd-yd )*f3;
            	      U(i1,i2,i3,ey)=( xd-zd )*f3;
            	      U(i1,i2,i3,ez)=( yd-xd )*f3;
          	    }
        	  }
      	}

            }
            
        }
        else if( forcingOption==gaussianChargeSource )
        {

            const realArray & f = u;  // do this for now

            const intArray & mask = mg.mask();
            
            realArray & x = isRectangular ? u : mg.center();
            realArray & rx = isRectangular ? u : mg.inverseVertexDerivative();

          #ifdef USE_PPP
            const realSerialArray & uLocal  =  u.getLocalArrayWithGhostBoundaries();
            const realSerialArray & fLocal  =  f.getLocalArrayWithGhostBoundaries();
            const realSerialArray & rxLocal = rx.getLocalArrayWithGhostBoundaries();
            const intSerialArray & maskLocal  =  mask.getLocalArrayWithGhostBoundaries();
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

//       real dx[3],xab[2][3];
//       if( isRectangular )
//         mg.getRectangularGridParameters( dx, xab );


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


            J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
            J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
            J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

            int gridType = isRectangular? 0 : 1;
            int useForcing=1;
            int useWhereMask=1;
            int orderOfExtrapolation=orderOfAccuracyInSpace+1;
            
            real ep=0;   // pointer to TZ function -- not used yet
            
            int ipar[30];
            real rpar[30];
            
            ipar[0] =J1.getBase(); 
            ipar[1] =J1.getBound(); 
            ipar[2] =J2.getBase(); 
            ipar[3] =J2.getBound();
            ipar[4] =J3.getBase();
            ipar[5] =J3.getBound();
            
            ipar[6] =gridType;
            ipar[7] =orderOfAccuracyInSpace;
            ipar[8] =orderOfAccuracyInTime;
            ipar[9] =orderOfExtrapolation;
            ipar[10]=useForcing;
            ipar[11]=ex;
            ipar[12]=ey;
            ipar[13]=ez;
            ipar[14]=hx;
            ipar[15]=hy;
            ipar[16]=hz;
            ipar[17]=useWhereMask;
            ipar[18]=grid;
            ipar[19]=debug;
            ipar[20]=forcingOption;

            rpar[0] =dx[0];
            rpar[1] =dx[1];
            rpar[2] =dx[2];
            rpar[3] =mg.gridSpacing(0);
            rpar[4] =mg.gridSpacing(1);
            rpar[5] =mg.gridSpacing(2);
            rpar[6] =t;
            rpar[7] =ep;
            rpar[8] =dt;
            rpar[9] =c;
            rpar[10]=eps;
            rpar[11]=mu;
            rpar[12]=kx;
            rpar[13]=ky;
            rpar[14]=kz;
            rpar[15]=slowStartInterval;
            rpar[16]=xab[0][0];
            rpar[17]=xab[0][1];
            rpar[18]=xab[0][2];
            rpar[19]=amplitude;
            rpar[20]=beta;
            rpar[21]=p;   
            rpar[22]=xp0;
            rpar[23]=xp1;
            rpar[24]=xp2;
            rpar[25]=vp0;
            rpar[26]=vp1;
            rpar[27]=vp2;


            int ierr=0;
  
            forcingOptMaxwell( mg.numberOfDimensions(),
                                                  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
                   			 uLocal.getBase(2),uLocal.getBound(2),
                                                  fLocal.getBase(0),fLocal.getBound(0),fLocal.getBase(1),fLocal.getBound(1),
                   			 fLocal.getBase(2),fLocal.getBound(2),
                   			 *uptr,*fptr,*maskptr,*rxptr, *xptr,
                                                  ipar[0], rpar[0], ierr );
            

            if( debug & 8 ) display(f,"f after forcingOptMaxwell",debugFile,"%9.2e ");
            
        }


#undef X0
#undef X1
#undef X2
  
    }

//kkc removed because this should be in the calling scope  timing(timeForForcing)+=getCPU()-time0;
    
  #undef         UHX
  #undef         UHY
  #undef         UHZ
  #undef         UEX
  #undef         UEY
  #undef         UEZ
  #undef         UMHX
  #undef         UMHY
  #undef         UMHZ
  #undef         UMEX
  #undef         UMEY
  #undef         UMEZ
  #undef         UNHX
  #undef         UNHY
  #undef         UNHZ
  #undef         UNEX
  #undef         UNEY
  #undef         UNEZ
  #undef         XEP
  #undef         XHP
  #undef         X
  #undef         ERRHX
  #undef         ERRHY
  #undef         ERRHZ
  #undef         ERREX
  #undef         ERREY
  #undef         ERREZ
  #undef         MASK
  #endif 
  #undef EXTGFP_SENTINEL

    return 0;
}

