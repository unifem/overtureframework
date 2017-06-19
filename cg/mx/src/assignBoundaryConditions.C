// This file automatically generated from assignBoundaryConditions.bC with bpp.
#include "Maxwell.h"
#include "DispersiveMaterialParameters.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "RadiationBoundaryCondition.h"
#include "ParallelUtility.h"

#define bcOptMaxwell EXTERN_C_NAME(bcoptmaxwell)
#define abcMaxwell EXTERN_C_NAME(abcmaxwell)
#define bcSymmetry EXTERN_C_NAME(bcsymmetry)
#define exmax EXTERN_C_NAME(exmax)
#define adjustForIncident EXTERN_C_NAME(adjustforincident)

extern "C"
{
            void bcOptMaxwell(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
            const int & gid, const int & dimension,
            const real&u, const real&f, const int&mask, const real&rsxy, const real&xy,
            const int&bc, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );

            void abcMaxwell(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
            const int & gid,
            const real&u, const real&un, const real&f, const int&mask, const real&rsxy, const real&xy,
            const int&bc, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );

            void bcSymmetry(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int & gid,
            const real&u, const int&mask, const real&rsxy, const real&xy,
            const int&bc, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );

            void adjustForIncident(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int & gid,
            const real&um, const real&u, const real&un, const int&mask, const real&rsxy, const real&xy,
            const real& initialConditionBoundingBox,
            const int&bc, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );

    void exmax(double&Ez,double&Bx,double&By,const int &nsources,const double&xs,const double&ys,
                      const double&tau,const double&var,const double&amp, const double&a,
                      const double&x,const double&y,const double&time);
}

#define pmlMaxwell EXTERN_C_NAME(pmlmaxwell)

extern "C"
{
  void pmlMaxwell(const int&nd,
            const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
            const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
            const int & gid, const int & dim,
            const real&um, const real&u, const real&un, 
            const int&ndra1a,const int&ndra1b,const int&ndra2a,const int&ndra2b,const int&ndra3a,const int&ndra3b,
            const real&vram, const real&vra, const real&vran, const real&wram, const real&wra, const real&wran, 
            const int&ndrb1a,const int&ndrb1b,const int&ndrb2a,const int&ndrb2b,const int&ndrb3a,const int&ndrb3b,
            const real&vrbm, const real&vrb, const real&vrbn, const real&wrbm, const real&wrb, const real&wrbn, 
            const int&ndsa1a,const int&ndsa1b,const int&ndsa2a,const int&ndsa2b,const int&ndsa3a,const int&ndsa3b,
            const real&vsam, const real&vsa, const real&vsan, const real&wsam, const real&wsa, const real&wsan, 
            const int&ndsb1a,const int&ndsb1b,const int&ndsb2a,const int&ndsb2b,const int&ndsb3a,const int&ndsb3b,
            const real&vsbm, const real&vsb, const real&vsbn, const real&wsbm, const real&wsb, const real&wsbn, 
            const int&ndta1a,const int&ndta1b,const int&ndta2a,const int&ndta2b,const int&ndta3a,const int&ndta3b,
            const real&vtam, const real&vta, const real&vtan, const real&wtam, const real&wta, const real&wtan, 
            const int&ndtb1a,const int&ndtb1b,const int&ndtb2a,const int&ndtb2b,const int&ndtb3a,const int&ndtb3b,
            const real&vtbm, const real&vtb, const real&vtbn, const real&wtbm, const real&wtb, const real&wtbn, 
            const real&f, const int&mask, const real&rsxy, const real&xy,
            const int&bc, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );
}


static FILE *localDebugFile=NULL;

#define updateghostboundaries EXTERN_C_NAME(updateghostboundaries)
#define updateghostandperiodic EXTERN_C_NAME(updateghostandperiodic)

extern "C"
{

/* This function is used to update ghost boundaries of a P++ array from fortran  */
void
updateghostboundaries(realArray *&pu )
{
  // cfprintf(localDebugFile,"**** updateGhostBoundaries called from fortran pu=%i...\n",pu);
  // const realSerialArray & uu = (*pu).getLocalArrayWithGhostBoundaries();
  // ::display(uu,"u before (fortran) update ghost boundaries -- after stage 1",localDebugFile,"%9.5f ");

    (*pu).updateGhostBoundaries();

  // Communication_Manager::Sync();
  // ::display(uu,"u after (fortran) update ghost boundaries -- after stage 1",localDebugFile,"%9.5f ");
  // Communication_Manager::Sync();
}

void
updateghostandperiodic(realMappedGridFunction *&pu )
{
  // cfprintf(localDebugFile,"**** updateGhostBoundaries called from fortran pu=%i...\n",pu);
  // const realSerialArray & uu = (*pu).getLocalArrayWithGhostBoundaries();
  // ::display(uu,"u before (fortran) update ghost boundaries -- after stage 1",localDebugFile,"%9.5f ");

    (*pu).periodicUpdate();
    (*pu).updateGhostBoundaries();

  // Communication_Manager::Sync();
  // ::display(uu,"u after (fortran) update ghost boundaries -- after stage 1",localDebugFile,"%9.5f ");
  // Communication_Manager::Sync();
} 
}

#define FOR_3D(i1,i2,i3,I1,I2,I3) int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); for(i3=I3Base; i3<=I3Bound; i3++) for(i2=I2Base; i2<=I2Bound; i2++) for(i1=I1Base; i1<=I1Bound; i1++)


// ==================================================
// ============= include forcing macros =============
// ==================================================
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

// Macros for the annulus eigenfunction exact solution
//==================================================================================================
// Evaluate the annulus eigenfunction or it's error
// 
// OPTION: solution, error
//==================================================================================================

// Macros for the plane material interface:

 // -- incident wave ---
 //  --- time derivative of incident ---
 // -- transmitted wave ---
 //  --- time derivative of transmitted wave ---

// Macros for dispersive waves
// -- dispersive plane wave solution
//        w = wr + i wi   (complex dispersion relation)
// You should define: 
//    dpwExp := exp( wi* t )
#define exDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[0]*(dpwExp))
#define eyDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y))-omegaDpwRe*(t))*(pwc[1]*(dpwExp))
#define hzDpw(x,y,t,dpwExp) sin(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

// ** FIX ME -- THIS IS WRONG
// #define extDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[0]*(dpwExp))
// #define eytDpw(x,y,t,dpwExp) (-twoPi*omegaDpwRe)*cos(twoPi*(kx*(x)+ky*(y)-omegaDpwRe*(t)))*(pwc[1]*(dpwExp))
// #define hztDpw(x,y,t,dpwExp) (-twoPi*cc)*cos(twoPi*(kx*(x)+ky*(y)-cc*(t)))*pwc[5]

// ====================================================================================
/// Macro: Return the time-dependent coefficients for a known solution
// 
// NOTE: This next section is repeated in getInitialConditions.bC,
//        getErrors.bC and assignBoundaryConditions.bC 
// ====================================================================================

// =============================================================================================================
// /Description:
//    Compute a new gridIndexRange, dimension
//             and boundaryCondition array that will be valid for the local grid on a processor.
// 
//    Set the gid to match the ends of the local array.
//    Set the bc(side,axis) to -1 (periodic) for internal boundaries between processors
//
// NOTES: In parallel we cannot assume the rsxy array is defined on all ghost points -- it will not
// be set on the extra ghost points put at the far ends of the array. -- i.e. internal boundary ghost 
// points will be set but not external
// =============================================================================================================
static void
getLocalBoundsAndBoundaryConditions( const realMappedGridFunction & a, 
                                                                          IntegerArray & gidLocal, 
                                                                          IntegerArray & dimensionLocal, 
                                                                          IntegerArray & bcLocal )
{

    MappedGrid & mg = *a.getMappedGrid();
    
    const IntegerArray & dimension = mg.dimension();
    const IntegerArray & gid = mg.gridIndexRange();
    const IntegerArray & bc = mg.boundaryCondition();
    
    gidLocal = gid;
    bcLocal = bc;
    dimensionLocal=dimension;
    
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
//      printF(" axis=%i gidLocal(0,axis)=%i a.getLocalBase(axis)=%i  dimension(0,axis)=%i\n",axis,gidLocal(0,axis),
//                        a.getLocalBase(axis),dimension(0,axis));
//      printF(" axis=%i gidLocal(1,axis)=%i a.getLocalBound(axis)=%i dimension(0,axis)=%i\n",axis,gidLocal(1,axis),
//                        a.getLocalBound(axis),dimension(1,axis));
        if( a.getLocalBase(axis) == a.getBase(axis) ) 
        {
            assert( dimension(0,axis)==a.getLocalBase(axis) );
            gidLocal(0,axis) = gid(0,axis); 
            dimensionLocal(0,axis) = dimension(0,axis); 
        }
        else
        {
            gidLocal(0,axis) = a.getLocalBase(axis)+a.getGhostBoundaryWidth(axis);
            dimensionLocal(0,axis) = a.getLocalBase(axis); 
      // for internal ghost mark as periodic since these behave in the same was as periodic
      // ** we cannot mark as "0" since the mask may be non-zero at these points and assignBC will 
      // access points out of bounds
            bcLocal(0,axis) = -1; // bc(0,axis)>=0 ? 0 : -1;
        }
        
        if( a.getLocalBound(axis) == a.getBound(axis) ) 
        {
            assert( dimension(1,axis) == a.getLocalBound(axis) );
            
            gidLocal(1,axis) = gid(1,axis); 
            dimensionLocal(1,axis) = dimension(1,axis); 
        }
        else
        {
            gidLocal(1,axis) = a.getLocalBound(axis)-a.getGhostBoundaryWidth(axis);
            dimensionLocal(1,axis) = a.getLocalBound(axis);
      // for internal ghost mark as periodic since these behave in the same was as periodic
            bcLocal(1,axis) = -1; // bc(1,axis)>=0 ? 0 : -1;
        }
        
    }
}


// ================================================================================================
//   Get the bounds of valid interior points when there are boundaries with the PML BC
//
// /extra : an additional offset (e.g. to check errors use extra=pmlErrorOffset)
// /Return value: true if this is a PML grid and the Index Iv was changed.
// ================================================================================================
bool Maxwell::
getBoundsForPML( MappedGrid & mg, Index Iv[3], int extra /* =0 */ )
{
    bool usePML = (mg.boundaryCondition(0,0)==abcPML || mg.boundaryCondition(1,0)==abcPML ||
             		 mg.boundaryCondition(0,1)==abcPML || mg.boundaryCondition(1,1)==abcPML ||
             		 mg.boundaryCondition(0,2)==abcPML || mg.boundaryCondition(1,2)==abcPML);
    
    if( !usePML ) return false;
    
  // Here is the box where we apply the interior equations when there is a PML
    Iv[2]=Range(mg.gridIndexRange(0,2),mg.gridIndexRange(1,2));
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        int na=mg.gridIndexRange(0,axis);
        if( mg.boundaryCondition(0,axis)==abcPML )
            na+=numberLinesForPML+extra;
        int nb=mg.gridIndexRange(1,axis);
        if( mg.boundaryCondition(1,axis)==abcPML )
            nb-=numberLinesForPML+extra;
        Iv[axis]=Range(na,nb);
    }

    return usePML;
}

// ================================================================================================
//   Adjust the bounds to account for the PML (i.e. do not include PML points in the bounds)
//
// /extra : an additional offset (e.g. to check errors use extra=pmlErrorOffset)
// /Return value: true if this is a PML grid and the Index Iv was changed.
// ================================================================================================
bool Maxwell::
adjustBoundsForPML( MappedGrid & mg, Index Iv[3], int extra /* =0 */ )
{
    bool usePML = (mg.boundaryCondition(0,0)==abcPML || mg.boundaryCondition(1,0)==abcPML ||
             		 mg.boundaryCondition(0,1)==abcPML || mg.boundaryCondition(1,1)==abcPML ||
             		 mg.boundaryCondition(0,2)==abcPML || mg.boundaryCondition(1,2)==abcPML);
    
    if( !usePML ) return false;

    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    {
        int na=Iv[axis].getBase();
        if( mg.boundaryCondition(0,axis)==abcPML )
            na+=numberLinesForPML+extra;
        int nb=Iv[axis].getBound();
        if( mg.boundaryCondition(1,axis)==abcPML )
            nb-=numberLinesForPML+extra;
        Iv[axis]=Range(na,nb);
    }

    return usePML;
}


// *************************************************************
// ************* PML boundary conditions ***********************
// *************************************************************


// =============================================================================================
// Macro to apply optimized versions of BC's
//
// OPTION: OPTION==field           : apply BC's to the fied variables Ex, Ey, ...
//         OPTION==timeDerivatives : apply BCs to the time-derivatives of the field (for SOSUP) 
//         OPTION==polarization    : apply BC's to the polarization vectors
// =============================================================================================

// ============================================================================
// Macro to compute the (x,y) coordinates - optimized for rectangular grids
// ============================================================================

// ============================================================================
// Macro to compute the (x,y,z) coordinates - optimized for rectangular grids
// ============================================================================


// ================================================================================================================
/// \brief Apply boundary conditions.
///
///  \param option: option=1 : apply BC's to E at t+dt/2; option=2 : apply BC's to H at t+dt, option=3 : apply all BC's
///
// ================================================================================================================
void Maxwell::
assignBoundaryConditions( int option, int grid, real t, real dt, realMappedGridFunction & u, 
                    			  realMappedGridFunction & uOld, int current )
// Note: uOld = u[current]
{
    assert( method!=yee );

    real time0=getCPU();
    const int np = max(1,Communication_Manager::numberOfProcessors());

    localDebugFile=pDebugFile;
    
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    c = cGrid(grid);
    eps = epsGrid(grid);
    mu = muGrid(grid);

    const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);

    MappedGrid & mg = *u.getMappedGrid();
    MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];
    
    const int numberOfDimensions = mg.numberOfDimensions();
    
    const int useForcing = forcingOption==twilightZoneForcing;

    const BoundaryForcingEnum & boundaryForcingOption =dbase.get<BoundaryForcingEnum>("boundaryForcingOption");

    const int & useSosupDissipation = parameters.dbase.get<int>("useSosupDissipation");
    bool addedExtraGhostLine = method==sosup || (method==nfdtd && useSosupDissipation);
    
  // Do we need the grid points: 
  // const bool centerNeeded=(useForcing || forcingOption==planeWaveBoundaryForcing ||  // **************** fix this 
  //                          initialConditionOption==gaussianPlaneWave || 
  //                          initialConditionOption==planeWaveInitialCondition ||  // for ABC + incident field fix 
  //                          initialConditionOption==planeMaterialInterfaceInitialCondition ||
  //                          initialConditionOption==annulusEigenfunctionInitialCondition  ||
  //                          method==yee || 
  //                          method==dsi );
    bool centerNeeded = vertexArrayIsNeeded( grid );

    if( centerNeeded )
    {
        if( (true || debug & 1) && t<2.*dt ) 
            printF("\n --MX-BC--  CREATE VERTEX grid=%i ---\n\n",grid);
        mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );
    }
    else
    {
        if( (true || debug & 1) && t<2.*dt ) 
            printF("\n --MX-BC--  VERTEX ARRAY NOT NEEDED grid=%i ---\n\n",grid);
    }
    

    const realArray & x = mg.center();

    Range all;
    const real dtb2=dt*.5;
    BoundaryConditionParameters bcParams;            

    const bool isRectangular = mg.isRectangular();
    real dx[3]={0.,0.,0.}; //

    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];  // NOTE: iv[0]==i1, iv[1]==i2, iv[2]==i3
    real xv[3]={0.,0.,0.};
    if( isRectangular )
    {
        mg.getRectangularGridParameters( dvx, xab );
        for( int dir=0; dir<mg.numberOfDimensions(); dir++ )
        {
            dx[dir]=dvx[dir];
            iv0[dir]=mg.gridIndexRange(0,dir);
            if( mg.isAllCellCentered() )
      	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
        }
    }
  // This macro defines the grid points for rectangular (Cartesian) grids:
    #undef XC
    #define XC(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))

#define XC0(i0,i1,i2) (xab[0][0]+dvx[0]*(i0-iv0[0]))
#define XC1(i0,i1,i2) (xab[0][1]+dvx[1]*(i1-iv0[1]))
#define XC2(i0,i1,i2) (xab[0][2]+dvx[2]*(i2-iv0[2]))

    Range C(ex,hz);

    bool debugGhost=false; // ***TEMP*** June 1, 2016 -- debugging SOSUP
    
    CompositeGrid & cg = *(cgfields[next].getCompositeGrid());
    const int numberOfComponentGrids = cg.numberOfComponentGrids();

    if( mg.getGridType()==MappedGrid::structuredGrid )
    {
    // ***********************
    // *** structured grid ***
    // ***********************

        if( debugGhost && grid==1 )
        {
            fprintf(debugFile,"\n --DBG--- setting u[1](-1,-1,0,ey)=-999.\n");
            u(-1,-1,0,ey)=-999.;
        }
        
        if( debug & 4 )
        {
            ::display(u,sPrintF("u at Start of assignBC, grid=%i t=%e",grid,t),debugFile,"%8.1e ");
        }

        #ifdef USE_PPP
          realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
          realSerialArray xLocal; if( centerNeeded ) getLocalArrayWithGhostBoundaries(x,xLocal);
        #else
          const realSerialArray & uLocal = u;
          const realSerialArray & xLocal = centerNeeded ? x : uLocal;
        #endif
    
        const IntegerArray & bcg = mg.boundaryCondition();
        IntegerArray gid, dim, bc;
        getLocalBoundsAndBoundaryConditions( u, gid, dim, bc );

        if( forcingOption==magneticSinusoidalPointSource )
        { 
      // this is a "hard" source -- the solution is specified

            const IntegerArray & gid = mg.gridIndexRange();
            int i1=gid(0,0)+(gid(1,0)-gid(0,0))/2;
            int i2=gid(0,1)+(gid(1,1)-gid(0,1))/2;
            int i3=gid(0,2)+(gid(1,2)-gid(0,2))/2;
        	  
            if( i1>=uLocal.getBase(0) && i1<=uLocal.getBound(0) &&
                    i2>=uLocal.getBase(1) && i2<=uLocal.getBound(1) &&
                    i3>=uLocal.getBase(2) && i3<=uLocal.getBound(2) )
            {
      	uLocal(i1,i2,i3,hz)=sin(twoPi*frequency*(t+dt));
            }
            
        }

        Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
        Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];

    // do this for now for corners:  **** fix ****  Is this needed any more ?
        Range C(ex,hz);


        bool useOpt=true; //  && bcOption!=Maxwell::useAllDirichletBoundaryConditions;  // don't do parallel for now

        const int includeGhost=1;
        
        for( int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
        {
            for( int side=Start; side<=End; side++ )
            {
                const int bc0 = bc(side,axis);

                const int is1= axis==axis1 ? (side==0 ? 1 : -1) : 0 ;
      	const int is2= axis==axis2 ? (side==0 ? 1 : -1) : 0 ;
        // printF("applyBC (side,axis)=(%i,%i) t=%e bc=%i\n",side,axis,t,mg.boundaryCondition(side,axis));

                if( bc0<=0 )
                    continue;
      	
        // *wdh* 041018 
        // const int ng=orderOfAccuracyInSpace/2;
        // const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
                const int ng=0, ng3=0;  // *wdh* 041018 assign BC's on ghost points too

                                
      	if( method==nfdtd || method==sosup )
      	{
          // *****************************************************************
          // **************** NFDTD Method ***********************************
          // *****************************************************************

        	  if( mg.boundaryCondition(side,axis)==dirichlet ||
                            mg.boundaryCondition(side,axis)==planeWaveBoundaryCondition ) 
        	  {
	    // this is a fake BC where we give all variables equal to the true solution
	    // assign all variables, vertex centred
    
              // printF("method==nfdtd:applyBC dirichlet to (side,axis)=(%i,%i) t=%e\n",side,axis,t);
          	    
          	    int numberOfGhostLines = orderOfAccuracyInSpace/2;
                        if( addedExtraGhostLine ) numberOfGhostLines++;  // sosup uses one extra ghost line
          	    
          	    int extra=numberOfGhostLines;
          	    getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3,extra);

            // for now assign dirichlet at ghost lines too.
          	    Iv[axis] = side==0 ? Range(Iv[axis].getBase()-numberOfGhostLines,Iv[axis].getBound()) : 
            	      Range(Iv[axis].getBase(),Iv[axis].getBound()+numberOfGhostLines);
            	      
                        if( mg.boundaryCondition(side,axis)==interfaceBoundaryCondition )
          	    { // do not include the boundary
                            Iv[axis] = side==0 ? Range(Iv[axis].getBase(),Iv[axis].getBound()-1) : 
                               		                   Range(Iv[axis].getBase()+1,Iv[axis].getBound());
          	    }
          	    
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

          	    if( initialConditionOption==planeWaveInitialCondition ||
                                mg.boundaryCondition(side,axis)==planeWaveBoundaryCondition ||
                                initialConditionOption==planeMaterialInterfaceInitialCondition ||
                                initialConditionOption==gaussianIntegralInitialCondition ||
                                initialConditionOption==annulusEigenfunctionInitialCondition ||
                                knownSolutionOption==userDefinedKnownSolution )
          	    {
            	      if( debug & 16 )
            	      {
            		printF("Dirichlet:BC: (grid,side,axis)=(%i,%i,%i) assign BC: I1,I2,I3=[%i,%i][%i,%i][%i,%i] \n",
                                              grid,side,axis,I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
            	      }
            	      if( initialConditionOption==planeMaterialInterfaceInitialCondition )
            	      {
            // ------------ macro for the plane material interface -------------------------
            // boundaryCondition: initialCondition, error, boundaryCondition
            // -----------------------------------------------------------------------------
                            int i1,i2,i3;
                            real tm=t-dt,x,y,z;
                            const real pmct=pmc[18]*twoPi; // for time derivative of exact solution
                            if( numberOfDimensions==2 )
                            {
                              z=0.;
                              if( grid < numberOfComponentGrids/2 )
                              { // incident plus reflected wave.
                                FOR_3D(i1,i2,i3,I1,I2,I3)
                                {
                                    x = X(i1,i2,i3,0);
                                    y = X(i1,i2,i3,1);
                                    real u1 = (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                    real u2 = (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                    real u3 = (pmc[10]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[11]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                      U(i1,i2,i3,ex)= u1;
                                      U(i1,i2,i3,ey)= u2;
                                      U(i1,i2,i3,hz)= u3;
                                      if( method==sosup )
                                      {
                               	 uLocal(i1,i2,i3,ext) = pmct*(pmc[0]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,eyt) = pmct*(pmc[2]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,hzt) = pmct*(pmc[10]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[11]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                      }
                                }
                              }
                              else
                              {
                // transmitted wave
                                FOR_3D(i1,i2,i3,I1,I2,I3)
                                {
                                    x = X(i1,i2,i3,0);
                                    y = X(i1,i2,i3,1);
                                    real u1 = (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                    real u2 = (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                    real u3 = (pmc[17]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                      U(i1,i2,i3,ex)= u1;
                                      U(i1,i2,i3,ey)= u2;
                                      U(i1,i2,i3,hz)= u3;
                                      if( method==sosup )
                                      {
                               	 uLocal(i1,i2,i3,ext) = (pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,eyt) = (pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,hzt) = (pmct*pmc[17]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                      }
                                }
                              }
                            }
                            else // --- 3D -- 
                            {
                              if( grid < numberOfComponentGrids/2 )
                              { // incident plus reflected wave.
                                FOR_3D(i1,i2,i3,I1,I2,I3)
                                {
                                    x = X(i1,i2,i3,0);
                                    y = X(i1,i2,i3,1);
                                    z = X(i1,i2,i3,2);
                                    real u1 = (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                    real u2 = (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                    real u3 = (pmc[4]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[5]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                      U(i1,i2,i3,ex)= u1;
                                      U(i1,i2,i3,ey)= u2;
                                      U(i1,i2,i3,ez)= u3;
                                      if( method==sosup )
                                      {
                               	 uLocal(i1,i2,i3,ext) = pmct*(pmc[0]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,eyt) = pmct*(pmc[2]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,ezt) = pmct*(pmc[4]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[5]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                      }
                                }
                              }
                              else
                              {
                // transmitted wave
                                FOR_3D(i1,i2,i3,I1,I2,I3)
                                {
                                    x = X(i1,i2,i3,0);
                                    y = X(i1,i2,i3,1);
                                    z = X(i1,i2,i3,2);
                                    real u1 = (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                    real u2 = (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                    real u3 = (pmc[14]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                      U(i1,i2,i3,ex)= u1;
                                      U(i1,i2,i3,ey)= u2;
                                      U(i1,i2,i3,ez)= u3;
                                      if( method==sosup )
                                      {
                               	 uLocal(i1,i2,i3,ext) = (pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,eyt) = (pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                               	 uLocal(i1,i2,i3,ezt) = (pmct*pmc[14]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                      }
                                }
                              }
                            }
            		if( debug & 8 )
            		{
              		  ::display(u(I1,I2,I3,ey),sPrintF("After PMIBC: grid=%i,side=%i,axis=%i t=%e",grid,side,axis,t),debugFile,"%8.1e ");
            		}
            	      }
            	      else if( initialConditionOption==gaussianIntegralInitialCondition )
            	      {
                        if( initialConditionOption==gaussianIntegralInitialCondition )
                        {
                            double wt,wx,wy;
                            const int nsources=1;
                            double xs[nsources], ys[nsources], tau[nsources], var[nsources], amp[nsources];
                            xs[0]=0.;
                            ys[0]=1.e-8*1./3.;  // should not be on a grid point
                            tau[0]=-.95;
                            var[0]=30.;
                            amp[0]=1.;
                            double period= 1.;  // period in y
                            double time=t;
                            int i1,i2,i3;
                            FOR_3D(i1,i2,i3,I1,I2,I3)
                            {
                                double x=X(i1,i2,i3,0); 
                                double y=X(i1,i2,i3,1);
                                exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);
                                    U(i1,i2,i3,ex) = wy;
                                    U(i1,i2,i3,ey) =-wx;
                                    U(i1,i2,i3,hz) = wt;
                            }
                        }
            	      }
            	      else if( initialConditionOption==annulusEigenfunctionInitialCondition )
            	      {
            //   printF(" I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0)=%i %i %i %i \n",
            // 	 I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0));
            //  Index I1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
            //  Index I2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
            //  Index I3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));
                          if( numberOfDimensions==2 )
                          {
                        #include "besselPrimeZeros.h"
                              const int n = int(initialConditionParameters[0]+.5);  // angular number, n=0,1,... --> Jn(omega*r)
                              const int m = int(initialConditionParameters[1]+.5);  // radial number m=0,... 
                              assert( m<mdbpz && n<ndbpz );
                              real omega = besselPrimeZeros[n][m];  // m'th zero of Jn' (excluding r=0 for J0)
            // printF("Annulus: Bessel function solution: n=%i, m=%i, omega=%e (c=%8.2e)\n",n,m,omega,c);
                              const real eps=sqrt(REAL_EPSILON);
                              real np1Factorial=1.;
                              for( int k=2; k<=n+1; k++ )
                                  np1Factorial*=k;              //  (n+1)!
                              int i1,i2,i3;
                              real r,gr,xd,yd,zd,bj,bjp,rx,ry,theta,thetax,thetay;
                              real cosTheta,sinTheta,bjThetax,bjThetay,uex,uey,cosn,sinn;
                              FOR_3D(i1,i2,i3,I1,I2,I3)
                              {
                                  xd=X(i1,i2,i3,0);
                                  yd=X(i1,i2,i3,1);
                                  r = sqrt(xd*xd+yd*yd);
                                  theta=atan2(yd,xd);
                 // if( theta<0. ) theta+=2.*Pi;
                                  cosTheta=cos(theta);
                                  sinTheta=sin(theta);
                                  cosn=cos(n*theta);
                                  sinn=sin(n*theta);
                                  gr=omega*r;
                                  rx = cosTheta;  // x/r
                                  ry = sinTheta;  // y/r
                                  bj=jn(n,gr);  // Bessel function J of order n
                                  if( gr>eps )  // need asymptotic expansion for small gr ??
                                  {
                                      bjp = -jn(n+1,gr) + n*bj/gr;  // from the recursion relation for Jn'
                                      thetay= cosTheta/r;
                                      thetax=-sinTheta/r;
                                      uex =  (1./omega)*(omega*ry*bjp*cosn -n*bj*thetay*sinn);
                                      uey = -(1./omega)*(omega*rx*bjp*cosn -n*bj*thetax*sinn);
                                  }
                                  else
                                  {
                   // Jn(z) = (.5*z)^n *( 1 - (z*z/4)/(n+1)! + .. 
                   // At r=0 all the Jn'(0) are zero except for n=1
                   // bjp = n==1 ? 1./2. : 0.;
                                      bjp = n==0 ? 0. : pow(.5,double(n))*pow(gr,n-1.)*( 1. - (gr*gr)/(4.*np1Factorial) );
                   // bj/r = omega*bjp at r=0
                                      bjThetay= omega*bjp*cosTheta;
                                      bjThetax=-omega*bjp*sinTheta;
                                      uex =  (1./omega)*(omega*ry*bjp*cosn -n*bjThetay*sinn);  // Ex.t = Hz.y
                                      uey = -(1./omega)*(omega*rx*bjp*cosn -n*bjThetax*sinn);  // Ey.t = - Hz.x
                                  }
                                  real sint = sin(omega*t), cost = cos(omega*t);
                   // *check me*
                                      uLocal(i1,i2,i3,hz)  = bj*cosn*cost;
                                      uLocal(i1,i2,i3,ex) = uex*sint;  // Ex.t = Hz.y
                                      uLocal(i1,i2,i3,ey) = uey*sint;  // Ey.t = - Hz.x
                                      if( method==sosup )
                                      {
                                          uLocal(i1,i2,i3,hzt) = -omega*bj*cosn*sint;
                                          uLocal(i1,i2,i3,ext) = omega*uex*cost;
                                          uLocal(i1,i2,i3,eyt) = omega*uey*cost;
                                      }
                            }
                          }
                          else /* 3D */
                          {
                        #include "besselZeros.h"
                              const real cylinderLength=cylinderAxisEnd-cylinderAxisStart;
                              const int n = int(initialConditionParameters[0]+.5);  // angular number, n=0,1,... --> Jn(omega*r)
                              const int m = int(initialConditionParameters[1]+.5);  // radial number m=0,... 
                              const int k = int(initialConditionParameters[2]+.5);  // axial number k=1,2,3,...
                              assert( m<mdbz && n<ndbz );
                              real lambda = besselZeros[n][m];  // m'th zero of Jn (excluding r=0 for J0)
                              real omega = sqrt( SQR(k*Pi/cylinderLength) + lambda*lambda );
                              printF("***Cylinder: Bessel function soln: n=%i, m=%i, k=%i, lambda=%e, omega=%e (c=%8.2e) [za,zb]=[%4.2f,%4.2f]\n",
                                            n,m,k,lambda,omega,c,cylinderAxisStart,cylinderAxisEnd);
                              const real eps=sqrt(REAL_EPSILON);
                              real np1Factorial=1.;
                              for( int k=2; k<=n+1; k++ )
                                  np1Factorial*=k;              //  (n+1)!
                              int i1,i2,i3;
                              real r,gr,xd,yd,zd,bj,bjp,rx,ry,theta,thetax,thetay;
                              real cosTheta,sinTheta,bjThetax,bjThetay,uex,uey,cosn,sinn,sinkz,coskz,cost,sint;
                              FOR_3D(i1,i2,i3,I1,I2,I3)
                              {
                                  xd=X(i1,i2,i3,0);
                                  yd=X(i1,i2,i3,1);
                                  zd=(X(i1,i2,i3,2)-cylinderAxisStart)/cylinderLength; // *wdh* 040626 -- allow for any length
                                  sinkz=sin(Pi*k*zd);   
                                  coskz=cos(Pi*k*zd); 
                                  r = sqrt(xd*xd+yd*yd);
                                  theta=atan2(yd,xd);
                                  cosTheta=cos(theta);
                                  sinTheta=sin(theta);
                                  cosn=cos(n*theta);
                                  sinn=sin(n*theta);
                                  cost=cos(omega*t);
                                  gr=lambda*r;
                                  rx = cosTheta;  // x/r
                                  ry = sinTheta;  // y/r
                                  bj=jn(n,gr);  // Bessel function J of order n
                                  if( gr>eps )  // need asymptotic expansion for small gr ??
                                  {
                                      bjp = -jn(n+1,gr) + n*bj/gr;  // from the recursion relation for Jn'
                                      thetay= cosTheta/r;
                                      thetax=-sinTheta/r;
                                      uex = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*rx*bjp*cosn - n*bj*thetax*sinn );
                                      uey = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*ry*bjp*cosn - n*bj*thetay*sinn );
                                  }
                                  else
                                  {
                   // Jn(z) = (.5*z)^n *( 1 - (z*z/4)/(n+1)! + .. 
                   // At r=0 all the Jn'(0) are zero except for n=1
                   // bjp = n==1 ? 1./2. : 0.;
                                      bjp = n==0 ? 0. : pow(.5,double(n))*pow(gr,n-1.)*( 1. - (gr*gr)/(4.*np1Factorial) );
                   // bj/r = lambda*bjp at r=0
                                      bjThetay= lambda*bjp*cosTheta;
                                      bjThetax=-lambda*bjp*sinTheta;
                                      uex = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*rx*bjp*cosn -n*bjThetax*sinn);  // Ex.t = Hz.y
                                      uey = -(k*Pi/(cylinderLength*lambda*lambda))*( lambda*ry*bjp*cosn -n*bjThetay*sinn);  // Ey.t = - Hz.x
                                  }
                   // *check me*
                                      uLocal(i1,i2,i3,ex) = uex*sinkz*cost;
                                      uLocal(i1,i2,i3,ey) = uey*sinkz*cost;
                                      uLocal(i1,i2,i3,ez) = bj*cosn*coskz*cost;
                                      if( method==sosup )
                                      {
                                          sint=sin(omega*t); 
                                          uLocal(i1,i2,i3,ext) = -omega*uex*sinkz*sint;
                                          uLocal(i1,i2,i3,eyt) = -omega*uey*sinkz*sint;
                                          uLocal(i1,i2,i3,ezt) = -omega*bj*cosn*coskz*sint;
                                      }
                            }
                          }
            	      }
            	      else if( knownSolutionOption==userDefinedKnownSolution )
            	      {
                                int numberOfTimeDerivatives=0;
                                CompositeGrid & cg = *(cgfields[next].getCompositeGrid());
            		getUserDefinedKnownSolution( t, cg,grid, u,I1,I2,I3,numberOfTimeDerivatives);
  
            	      }
            	      else
            	      { //planeWaveInitialCondition or planeWaveBoundaryCondition
              		  
            		int i1,i2,i3;
            		if( numberOfDimensions==2 )
            		{
    
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                                        real x0,y0;
                                          if( isRectangular )
                                          {
                                              x0 = XC0(i1,i2,i3);
                                              y0 = XC1(i1,i2,i3);
                                          }
                                          else
                                          {
                                              x0 = X(i1,i2,i3,0);
                                              y0 = X(i1,i2,i3,1);
                                          }

                		    U(i1,i2,i3,ex)=exTrue(x0,y0,t); 
                		    U(i1,i2,i3,ey)=eyTrue(x0,y0,t);
                		    U(i1,i2,i3,hz)=hzTrue(x0,y0,t);
		    // printF("new:BC: i=%i,%i,%i x=(%6.3f,%6.3f) u=(%8.2e,%8.2e,%8.2e)\n",i1,i2,i3,X(i1,i2,i3,0),X(i1,i2,i3,1),U(i1,i2,i3,ex),U(i1,i2,i3,ey),U(i1,i2,i3,hz));
              		  }
              		  if( method==sosup )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                                            real x0,y0;
                                              if( isRectangular )
                                              {
                                                  x0 = XC0(i1,i2,i3);
                                                  y0 = XC1(i1,i2,i3);
                                              }
                                              else
                                              {
                                                  x0 = X(i1,i2,i3,0);
                                                  y0 = X(i1,i2,i3,1);
                                              }
                  		      U(i1,i2,i3,ext) =extTrue(x0,y0,t);
                  		      U(i1,i2,i3,eyt) =eytTrue(x0,y0,t);
                  		      U(i1,i2,i3,hzt) =hztTrue(x0,y0,t);
                		    }
              		  }
            		}
            		else
            		{
              		  if( solveForElectricField )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                  		      real x0,y0,z0;
                                      if( isRectangular )
                                      {
                                          x0 = XC0(i1,i2,i3);
                                          y0 = XC1(i1,i2,i3);
                                          z0 = XC2(i1,i2,i3);
                                      }
                                      else
                                      {
                                          x0 = X(i1,i2,i3,0);
                                          y0 = X(i1,i2,i3,1);
                                          z0 = X(i1,i2,i3,2);
                                      }

                  		      U(i1,i2,i3,ex)=exTrue3d(x0,y0,z0,t);
                  		      U(i1,i2,i3,ey)=eyTrue3d(x0,y0,z0,t);
                  		      U(i1,i2,i3,ez)=ezTrue3d(x0,y0,z0,t);
                		    }
                		    if( method==sosup )
                		    {
                  		      FOR_3D(i1,i2,i3,I1,I2,I3)
                  		      {
                    		        real x0,y0,z0;
                                          if( isRectangular )
                                          {
                                              x0 = XC0(i1,i2,i3);
                                              y0 = XC1(i1,i2,i3);
                                              z0 = XC2(i1,i2,i3);
                                          }
                                          else
                                          {
                                              x0 = X(i1,i2,i3,0);
                                              y0 = X(i1,i2,i3,1);
                                              z0 = X(i1,i2,i3,2);
                                          }

                  			U(i1,i2,i3,ext) =extTrue3d(x0,y0,z0,t);
                  			U(i1,i2,i3,eyt) =eytTrue3d(x0,y0,z0,t);
                  			U(i1,i2,i3,ezt) =eztTrue3d(x0,y0,z0,t);

                  		      }
                		    }
                		    
              		  }
              		  if( solveForMagneticField )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                  		      real x0,y0,z0;
                                      if( isRectangular )
                                      {
                                          x0 = XC0(i1,i2,i3);
                                          y0 = XC1(i1,i2,i3);
                                          z0 = XC2(i1,i2,i3);
                                      }
                                      else
                                      {
                                          x0 = X(i1,i2,i3,0);
                                          y0 = X(i1,i2,i3,1);
                                          z0 = X(i1,i2,i3,2);
                                      }

                  		      U(i1,i2,i3,hx)=hxTrue3d(x0,y0,z0,t);
                  		      U(i1,i2,i3,hy)=hyTrue3d(x0,y0,z0,t);
                  		      U(i1,i2,i3,hz)=hzTrue3d(x0,y0,z0,t);
                		    }
                		    
              		  }
            		}
            	      }
            	      
              // printF(" assign BC: I1,I2,I3=[%i,%i][%i,%i][%i,%i] \n",
              //            I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());

              // display(u(I1,I2,I3,C),"u(I1,I2,I3,C) after BC new:");

          	    
          	    } // end if( initialConditionOption==planeWaveInitialCondition || ...
          	    else if( forcingOption==twilightZoneForcing )
          	    {
            	      assert( tz!=NULL );
            	      OGFunction & e = *tz;
            	      Range C(ex,hz);

            	      if( true )
            	      {
            		
            		int i1,i2,i3;
            		if( mg.numberOfDimensions()==2 )
            		{
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    real x0 = X(i1,i2,i3,0);
                		    real y0 = X(i1,i2,i3,1);
                		    U(i1,i2,i3,ex) =e(x0,y0,0.,ex,t);
                		    U(i1,i2,i3,ey) =e(x0,y0,0.,ey,t);
                		    U(i1,i2,i3,hz) =e(x0,y0,0.,hz,t);
              		  }
              		  if( method==sosup )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                  		      real x0 = X(i1,i2,i3,0);
                  		      real y0 = X(i1,i2,i3,1);
                  		      U(i1,i2,i3,ext) =e(x0,y0,0.,ext,t);
                  		      U(i1,i2,i3,eyt) =e(x0,y0,0.,eyt,t);
                  		      U(i1,i2,i3,hzt) =e(x0,y0,0.,hzt,t);
                		    }
              		  }
              		  
            		}
            		else
            		{
              		  assert( !solveForMagneticField ); // this case to do..
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    real x0 = X(i1,i2,i3,0);
                		    real y0 = X(i1,i2,i3,1);
                		    real z0 = X(i1,i2,i3,2);
                		    U(i1,i2,i3,ex) =e(x0,y0,z0,ex,t);
                		    U(i1,i2,i3,ey) =e(x0,y0,z0,ey,t);
                		    U(i1,i2,i3,ez) =e(x0,y0,z0,ez,t);
              		  }
              		  if( method==sosup )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                  		      real x0 = X(i1,i2,i3,0);
                  		      real y0 = X(i1,i2,i3,1);
                  		      real z0 = X(i1,i2,i3,2);
                  		      U(i1,i2,i3,ext) =e(x0,y0,z0,ext,t);
                  		      U(i1,i2,i3,eyt) =e(x0,y0,z0,eyt,t);
                  		      U(i1,i2,i3,ezt) =e(x0,y0,z0,ezt,t);
                		    }
              		  }
              		  
            		}
            	      
            	      }
            	      else // old way -- trouble with PPP
            	      {
            		u(I1,I2,I3,C)=e(mg,I1,I2,I3,C,t);
            	      }
          	    
          	    }
          	    else if( initialConditionOption==gaussianPlaneWave )
          	    {
            	      realSerialArray xi;
            	      xi=kx*(xLocal(I1,I2,I3,0)-x0GaussianPlaneWave)+ky*(xLocal(I1,I2,I3,1)-y0GaussianPlaneWave) -cc*t;

            	      uLocal(I1,I2,I3,hz)=hzGaussianPulse(xi); 
            	      uLocal(I1,I2,I3,ex)=uLocal(I1,I2,I3,hz)*(-ky/(eps*cc));
            	      uLocal(I1,I2,I3,ey)=uLocal(I1,I2,I3,hz)*( kx/(eps*cc));
          	    }
                        else if( boundaryForcingOption==planeWaveBoundaryForcing ||
                                          initialConditionOption==planeWaveScatteredFieldInitialCondition )
          	    {
              // --- Assign the dirichlet (i.e. exact solution) BC for a plane wave or plane wave scattered field ---

            	      if( knownSolution==NULL )
            	      {
            		initializeKnownSolution();
            	      }
            	      const realArray & ug = (*knownSolution)[grid];

                	      const real cc0= cGrid(0)*sqrt( kx*kx+ky*ky ); // NOTE: use grid 0 values for multi-materials

	      // const real cost = cos(-twoPi*cc0*t);
	      // const real sint = sin(-twoPi*cc0*t);
	      // const real dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
	      // const real dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 	    

                                real cost,sint,costm,sintm,dcost,dsint;
                                real phiPc,phiPs, phiPcm,phiPsm;
                                if( dispersionModel==noDispersion )
                                {
                                    cost = cos(-twoPi*cc0*t); // *wdh* 040626 add "-"
                                    sint = sin(-twoPi*cc0*t); // *wdh* 040626 add "-"
                                    costm= cos(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
                                    sintm= sin(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
                                    dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
                                    dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 
                                }
                                else
                                {
                  // -- dispersive model -- *CHECK ME*
                  // Evaluate the dispersion relation for "s"
                                    DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
                                    const real kk = twoPi*cc0;  // Parameter in dispersion relation **check me**
                                    real reS, imS;
                                    dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );
                                    real expS = exp(reS*t), expSm=exp(reS*(t-dt));
                                    imS=-imS;  // flip sign    **** FIX ME ****
                                    printF("--IC-- scatCyl imS=%g, Im(s)/(twoPi*cc0)=%g reS=%g\n",imS,imS/twoPi*cc0,reS);
                                    cost = cos( imS*t )*expS;      // "cos(t)" for dispersive model 
                                    sint = sin( imS*t )*expS;
                                    costm = cos( imS*(t-dt) )*expS;
                                    sintm = sin( imS*(t-dt) )*expS;
                                    dcost = -imS*sint + reS*cost;  //  d/dt of "cost" 
                                    dsint =  imS*cost + reS*sint;  //  d/dt of "cost" 
                                    real alpha=reS, beta=imS;  // s= alpha + i*beta (
                                    real a,b;   // psi = a + i*b 
                  // P = Im{ psi(s)*E } = Im{ (a+i*b)*( Er + i*Ei)(cos(beta*t)+i*sin(beta*t))*exp(alpha*t) }
                                    const real gamma=dmp.gamma, omegap=dmp.omegap;
                                    const real cp = eps* omegap*omegap;
                                    const real denom = (SQR(alpha)+SQR(beta))*( SQR((alpha+gamma)) + SQR(beta) );
                                    a =  cp* (alpha*(alpha+gamma)-beta*beta)/denom;   
                                    b = -cp* beta*(2.*alpha+gamma)/denom;
                                    phiPc = a*cost-b*sint;
                                    phiPs = a*sint+b*cost;
                                    phiPcm = a*costm-b*sintm;
                                    phiPsm = a*sintm+b*costm;
                                }

              // // NOTE: This next section is repeated in getInitialConditions.bC,
              // //        getErrors.bC and assignBoundaryConditions.bC *FIX ME*
              // real cost,sint,dcost,dsint;
	      // if( dispersionModel==noDispersion )
	      // {
	      // 	cost = cos(-twoPi*cc0*t); 
	      // 	sint = sin(-twoPi*cc0*t); 
	      // 	dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
	      // 	dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 
	      // }
	      // else
	      // {
	      // 	// -- dispersive model --  *CHECK ME*

	      // 	// Evaluate the dispersion relation for "s"
	      // 	DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
	      // 	const real kk = twoPi*cc0;  // Parameter in dispersion relation **check me**
	      // 	real reS, imS;
	      // 	dmp.computeDispersionRelation( c,eps,mu,kk, reS, imS );
	      // 	real expS = exp(reS*t), expSm=exp(reS*(t-dt));
	      // 	imS=-imS;  // flip sign 
	      // 	printF("--ER-- scatCyl imS=%g, Im(s)/(twoPi*cc0)=%g reS=%g\n",imS,imS/twoPi*cc0,reS);

	      // 	cost = cos( imS*t )*expS;      // "cos(t)" for dispersive model 
	      // 	sint = sin( imS*t )*expS;

	      // 	dcost = -imS*sint + reS*cost;  //  d/dt of "cost" 
	      // 	dsint =  imS*cost + reS*sint;  //  d/dt of "cost" 
          	    
	      // }

                            if( debug & 4 ) printF("Set Dirichlet BC from known solution, grid,side,axis=%i,%i,%i\n",grid,side,axis);
            	      
              // getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);

              // *wdh* 041013: Do not use the next line -- P++ problems
              // *wdh*   u(I1,I2,I3,C)=ug(I1,I2,I3,C)*sint+ug(I1,I2,I3,C+3)*cost;

                            #ifdef USE_PPP
                  	        realSerialArray ugLocal; getLocalArrayWithGhostBoundaries(ug,ugLocal);
                            #else
                  	        const realSerialArray & ugLocal = ug; 
                            #endif
            	      real *ugp = ugLocal.Array_Descriptor.Array_View_Pointer3;
            	      const int ugDim0=ugLocal.getRawDataSize(0);
            	      const int ugDim1=ugLocal.getRawDataSize(1);
            	      const int ugDim2=ugLocal.getRawDataSize(2);
                            #define UG(i0,i1,i2,i3) ugp[i0+ugDim0*(i1+ugDim1*(i2+ugDim2*(i3)))]
            		
            	      int i1,i2,i3;
            	      if( numberOfDimensions==2 )
            	      {
            		FOR_3D(i1,i2,i3,I1,I2,I3)
            		{
              		  U(i1,i2,i3,ex)=UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
              		  U(i1,i2,i3,ey)=UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
              		  U(i1,i2,i3,hz)=UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost;
              		  if( method==sosup )
              		  { // time derivatives: 
                		    U(i1,i2,i3,ext)=UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost;
                		    U(i1,i2,i3,eyt)=UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost;
                		    U(i1,i2,i3,hzt)=UG(i1,i2,i3,hz)*dsint+UG(i1,i2,i3,hz+3)*dcost;
              		  }
            		}
		// -- dispersion model components --
            		if( dispersionModel!=noDispersion )
            		{
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    U(i1,i2,i3,pxc)  = UG(i1,i2,i3,ex)*phiPs + UG(i1,i2,i3,ex+3)*phiPc;
                		    U(i1,i2,i3,pyc)  = UG(i1,i2,i3,ey)*phiPs + UG(i1,i2,i3,ey+3)*phiPc;
              		  }
            		}
            	      }
                            else
            	      {
                                if( solveForElectricField )
            		{
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    U(i1,i2,i3,ex)=UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
                		    U(i1,i2,i3,ey)=UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
                		    U(i1,i2,i3,ez)=UG(i1,i2,i3,ez)*sint+UG(i1,i2,i3,ez+3)*cost;
                		    if( method==sosup )
                		    { // time derivatives:
                  		      U(i1,i2,i3,ext)=UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost;
                  		      U(i1,i2,i3,eyt)=UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost;
                  		      U(i1,i2,i3,ezt)=UG(i1,i2,i3,ez)*dsint+UG(i1,i2,i3,ez+3)*dcost;
                		    }
              		  
              		  }

		  // -- dispersion model components --
              		  if( dispersionModel!=noDispersion )
              		  {
                		    FOR_3D(i1,i2,i3,I1,I2,I3)
                		    {
                  		      U(i1,i2,i3,pxc)  = UG(i1,i2,i3,ex)*phiPs + UG(i1,i2,i3,ex+3)*phiPc;
                  		      U(i1,i2,i3,pyc)  = UG(i1,i2,i3,ey)*phiPs + UG(i1,i2,i3,ey+3)*phiPc;
                  		      U(i1,i2,i3,pzc)  = UG(i1,i2,i3,ez)*phiPs + UG(i1,i2,i3,ez+3)*phiPc;
                		    }
              		  }

            		}
            		if( solveForMagneticField )
            		{
              		  FOR_3D(i1,i2,i3,I1,I2,I3)
              		  {
                		    U(i1,i2,i3,hx)=UG(i1,i2,i3,hx)*sint+UG(i1,i2,i3,hx+3)*cost;
                		    U(i1,i2,i3,hy)=UG(i1,i2,i3,hy)*sint+UG(i1,i2,i3,hy+3)*cost;
                		    U(i1,i2,i3,hz)=UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost;
              		  }
            		}
            		
            	      }
            	      #undef UG

              // extrapolate the ghostline
              // getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3); // ghost line
              // u(Ig1,Ig2,Ig3,C)=2.*u(Ig1+is1,Ig2+is2,Ig3,C)-u(Ig1+2*is1,Ig2+2*is2,Ig3,C);

              // u(Ig1,Ig2,Ig3,C)=2.*( ug(I1,I2,I3,C)*sint+ug(I1,I2,I3,C+3)*cost )
              //                       -u(Ig1+2*is1,Ig2+2*is2,Ig3,C);
          	    
          	    }
                        else if( true )
          	    {
                            uLocal(I1,I2,I3,C)=0.;
          	    }
          	    else
          	    {
            	      OV_ABORT("Maxwell::assignBoundaryConditions:dirichlet unknown forcing option");
          	    }

        	  } // end if( bc(side,axis)==dirichlet || bc(side,axis)==planeWaveBoundaryCondition ) 
        	  
        	  else if( mg.boundaryCondition(side,axis)==perfectElectricalConductor )
        	  {
            // =================================================================================
            // =========== NOTE: PEC BOUNDARY CONDITIONS ARE ASSIGNED IN MXCORNERS =============
            // =================================================================================

                        assert( useOpt );
          	    
        	  }
                    else if( mg.boundaryCondition(side,axis)==symmetry )
        	  {
          	    if( FALSE ) // *WDH* June 16, 2016
          	    {
	      // THIS IS AN OLD INCORRECT SYMMETRY BC -- only did even symmetry
	      // Symmetry conditions are now performed elsewhere in bcSymmetry 

            	      if( t<=2*dt )
            	      {
            		printF("Apply symmetry BC on (side,axis)=(%i,%i) t=%8.2e is1,is2=(%i,%i)\n"
                   		       " ************ This symmetry BC should not be used any more! *********\n",
                   		       side,axis,t,is1,is2);
            	      }

	      //              Range V(ex,ey);
	      //              mgop.applyBoundaryCondition(u,V,BCTypes::vectorSymmetry,symmetry,0.,t);
	      //              Range H(hz,hz);
	      //              mgop.applyBoundaryCondition(u,H,BCTypes::evenSymmetry,symmetry,0.,t);
            	      Range C(ex,hz);
            	      mgop.applyBoundaryCondition(u,C,BCTypes::evenSymmetry,symmetry,0.,t);
            	      if( orderOfAccuracyInSpace==4 )
            	      {
            		bcParams.ghostLineToAssign=2;
            		mgop.applyBoundaryCondition(u,C,BCTypes::evenSymmetry,symmetry,0.,t,bcParams);
            	      }

            	      if( orderOfAccuracyInSpace!=2 && orderOfAccuracyInSpace!=4 )
            	      {
            		printF("cgmx: assignBC: symmetry BC : ERROR: orderOfAccuracyInSpace=%i\n",orderOfAccuracyInSpace);
            		OV_ABORT("FINISH ME");
            	      }
          	    }
          	    
        	  }
                    else if( mg.boundaryCondition(side,axis)==interfaceBoundaryCondition )
        	  {
            // do nothing here
        	  }
                    else if( (mg.boundaryCondition(side,axis)>=abcEM2 && mg.boundaryCondition(side,axis)<=abc5) ||
                                        mg.boundaryCondition(side,axis)==rbcNonLocal || mg.boundaryCondition(side,axis)==rbcLocal )
        	  {
            // do nothing here
        	  }
        	  else // bc== ?
        	  {
          	    printF("assignBoundaryConditions:ERROR: unknown boundaryCondition(%i,%i)=%i\n",
               		   side,axis,mg.boundaryCondition(side,axis));
          	    OV_ABORT("assignBoundaryConditions:ERROR");
        	  }
      	}
      	else if( method==yee )
      	{
          // *****************************************************************
          // **************** Yee Method *************************************
          // *****************************************************************
        	  if( mg.boundaryCondition(side,axis)==dirichlet )
        	  {
	    // this is a fake BC where we give all variables equal to the true solution
        	  
                        if( option==1 )
          	    {
            	      getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                            getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1); // ghost line

            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            	      if( !ok ) continue;
            	      ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

            	      realSerialArray xe(I1,I2,I3,mg.numberOfDimensions());
          	    
            	      if( axis==0 )
            	      {
		// left or right side
            		xe=.5*(xLocal(I1,I2+1,I3,all)+xLocal(I1,I2,I3,all));  // face center
            		uLocal(I1,I2,I3,ey)=eyTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);

                // extrapolate ghost line values for plotting:
                                uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);

                                if( side==1 ) // adjust for face-centredness
                                    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
            	      }
            	      else if( axis==1 )
            	      {
		// bottom or top
            		xe=.5*(xLocal(I1+1,I2,I3,all)+xLocal(I1,I2,I3,all));  // face center
            		uLocal(I1,I2,I3,ex)=exTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);

                // extrapolate ghost line values for plotting:
                                uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
                                if( side==1 )// adjust for face-centredness
                                    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
              	        uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);

            	      }
            	      else
            	      {
            		throw "error";
            	      }
          	    }
          	    else
          	    {
	      // BC for H 
              // there is no BC for H -- just extrapolate ghost line values for plotting
                            if( side==0 ) 
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1);
                            else // adjust for face-centredness
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                
            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

                            uLocal(Ig1,Ig2,Ig3,hz)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,hz)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,hz);
//               uLocal(Ig1,Ig2,Ig3,hz)=3.*uLocal(Ig1+is1,Ig2+is2,Ig3,hz)-3.*uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,hz)+
//                                    uLocal(Ig1+3*is1,Ig2+3*is2,Ig3,hz);
            	      
                        }
        	  }
        	  else if( mg.boundaryCondition(side,axis)==perfectElectricalConductor ) 
        	  {
            // --- YEE ---
	    // (1) tangential components of E are zero
	    // (2) normal derivative of the normal component of E is zero ??
	    // (3) normal component of magnetic field is zero

                        if( option==1 )
          	    {
            	      getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                            getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1); // ghost line
          	    
            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            	      if( !ok ) continue;
            	      ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

            	      if( axis==0 )
            	      {
		// left or right side
            		uLocal(I1,I2,I3,ey)=0.;
                // extrapolate ghost line values for plotting:
                                uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
                                if( side==1 ) // adjust for face-centredness
                                    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
            	      }
            	      else if( axis==1 )
            	      {
		// bottom or top
            		uLocal(I1,I2,I3,ex)=0.;
                // extrapolate ghost line values for plotting:
                                uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
                                if( side==1 )// adjust for face-centredness
                                    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
              	        uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
            	      }
            	      else
            	      {
            		throw "error";
            	      }
          	    }
          	    else
          	    {
	      // BC for H 
              // there is no BC for H -- just extrapolate ghost line values for plotting
                            if( side==0 ) 
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1);
                            else // adjust for face-centredness
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                
            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

                            uLocal(Ig1,Ig2,Ig3,hz)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,hz)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,hz);
                        }
        	  }
        	  else // bc
        	  {
          	    printF("applyBoundaryConditions:ERROR: unknown boundaryCondition(%i,%i)=%i\n",
               		   side,axis,mg.boundaryCondition(side,axis));
          	    Overture::abort("applyBoundaryConditions:ERROR");
        	  }
      	}  // *********************** END YEE ************************
      	
      	else if( method==dsi )
      	{
          // *****************************************************************
          // **************** DSI Method *************************************
          // *****************************************************************
        	  if( mg.boundaryCondition(side,axis)==dirichlet )
        	  {
	  // this is a fake BC where we give all variables equal to the true solution
                        getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                        getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1); // ghost line

          	    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
          	    if( !ok ) continue;
          	    ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
          	    if( !ok ) continue;

                        realSerialArray xe(I1,I2,I3,mg.numberOfDimensions());
          	    
          	    if( option==1 )
          	    {
            	      if( axis==0 )
            	      {
		// left or right side
            		xe=.5*(xLocal(I1,I2+1,I3,all)+xLocal(I1,I2,I3,all));  // face center
            		uLocal(I1,I2,I3,ex01)=exTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);
            		uLocal(I1,I2,I3,ey01)=eyTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);

	      // extrapolate ghost line values for plotting:
            		uLocal(Ig1,Ig2,Ig3,ex01)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex01)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex01);
            		uLocal(Ig1,Ig2,Ig3,ey01)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey01)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey01);

            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
            		uLocal(Ig1,Ig2,Ig3,ex10)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex10)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex10);
            		uLocal(Ig1,Ig2,Ig3,ey10)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey10)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey10);
            	      }
            	      else if( axis==1 )
            	      {
		// bottom or top
            		xe=.5*(xLocal(I1+1,I2,I3,all)+xLocal(I1,I2,I3,all));  // face center
            		uLocal(I1,I2,I3,ex10)=exTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);
            		uLocal(I1,I2,I3,ey10)=eyTrue(xe(I1,I2,I3,0),xe(I1,I2,I3,1),t+dtb2);

            		uLocal(Ig1,Ig2,Ig3,ex10)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex10)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex10);
            		uLocal(Ig1,Ig2,Ig3,ey10)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey10)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey10);
            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
            		uLocal(Ig1,Ig2,Ig3,ex01)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex01)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex01);
            		uLocal(Ig1,Ig2,Ig3,ey01)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey01)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey01);

            	      }
            	      else
            	      {
            		throw "error";
            	      }
          	    }
          	    else
          	    {
	      // BC for H 
              // there is no BC for H -- just extrapolate ghost line values for plotting
                            if( side==0 ) 
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1);
                            else // adjust for face-centredness
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                
            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

                            uLocal(Ig1,Ig2,Ig3,hz11)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,hz11)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,hz11);
          	    }
        	  }
        	  else if( mg.boundaryCondition(side,axis)==perfectElectricalConductor )
        	  {
	    // ***** DSI : Perfect Electrical Conductor *****

                        getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                        getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1); // ghost line
          	    
          	    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
          	    if( !ok ) continue;
          	    ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
          	    if( !ok ) continue;

          	    if( option==0 )
          	    {
            	      if( axis==0 )
            	      {
		// left or right side
                // tangential component is zero.
            		uLocal(I1,I2,I3,ex)=0.;  // **** finish this ****
            		uLocal(I1,I2,I3,ey)=0.;

	        // extrapolate ghost line values for plotting:
		//		uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//		uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);

            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
		//		uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//		uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
            	      }
            	      else if( axis==1 )
            	      {
		// bottom or top
            		uLocal(I1,I2,I3,ex)=0.;
            		uLocal(I1,I2,I3,ey)=0.;

		//uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
		//		uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//		uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);

            	      }
            	      else
            	      {
            		throw "error";
            	      }
          	    }
          	    else if(option==1)
          	    {
            	      if( axis==0 )
            	      {
		// left or right side
		// tangential component is zero.
            		uLocal(I1,I2,I3,ex)=0.;  // **** finish this ****
            		uLocal(I1,I2,I3,ey)=0.;
                		    
		// extrapolate ghost line values for plotting:
		//uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
                		    
            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
		//uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
            	      }
            	      else if( axis==1 )
            	      {
		// bottom or top
            		uLocal(I1,I2,I3,ex)=0.;
            		uLocal(I1,I2,I3,ey)=0.;
                		    
		//uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
            		if( side==1 ) // adjust for face-centredness
              		  getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
		//uLocal(Ig1,Ig2,Ig3,ex)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ex)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ex);
		//uLocal(Ig1,Ig2,Ig3,ey)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3,ey)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3,ey);
                		    
            	      }
            	      else
            	      {
            		throw "error";
            	      }
          	    }
          	    else
          	    {
	      // BC for H 
              // there is no BC for H -- just extrapolate ghost line values for plotting
                            if( side==0 ) 
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,1);
                            else // adjust for face-centredness
                                getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,0);
                                
            	      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,Ig1,Ig2,Ig3,includeGhost);
            	      if( !ok ) continue;

                            uLocal(Ig1,Ig2,Ig3)=2.*uLocal(Ig1+is1,Ig2+is2,Ig3)-uLocal(Ig1+2*is1,Ig2+2*is2,Ig3);
          	    }
        	  }
        	  else if( mg.boundaryCondition(side,axis)>0 )
        	  {
          	    printF("applyBoundaryConditions:ERROR: unknown boundaryCondition(%i,%i)=%i\n",
               		   side,axis,mg.boundaryCondition(side,axis));
        	  }
      	} // ************************** END DSI ************************************
      	
      	else
      	{
        	  printF("applyBoundaryConditions:ERROR: unknown boundaryCondition(%i,%i)=%i\n",
             		 side,axis,mg.boundaryCondition(side,axis));
        	  Overture::abort("applyBoundaryConditions:ERROR");
      	}
      	
            } // end side
        }  // end axis


        if( debug & 8 )
        {
            Index I1,I2,I3;
            getIndex(mg.dimension(),I1,I2,I3);      
            ::display(u(I1,I2,I3,ey),sPrintF("BC: Ey before optBC, grid=%i t=%e",grid,t),debugFile,"%8.1e ");
        }


    // *wdh* 041127 -- apply opt BC's after above dirichlet BC's ----

        if( debugGhost && grid==1 )
            fprintf(debugFile,"\n --DBG--- Before optBC: u[1](-1,-1,0,ey)=%8.2e\n",uLocal(-1,-1,0,ey));

    // *wdh* June 25, 2016 if( initialConditionOption!=planeMaterialInterfaceInitialCondition ) // *wdh* 080922
        if( true )
        {
      // *wdh* 2011/12/02 -- this next line was wrong -- side and axis are not correct here.
      // *wdh* getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
      // I1,I2,I3 are not used I don't think. We do check that there are any points on this processor
            getIndex(mg.gridIndexRange(),I1,I2,I3);
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
            if( ok && useOpt )
            {
        // use optimised boundary conditions
                int ipar[40];
                real rpar[50];
                int gridType = isRectangular ? 0 : 1;
                int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
                int useWhereMask=false;
                int bcOrderOfAccuracy=orderOfAccuracyInSpace;
                if( method==sosup && orderOfAccuracyInSpace==6 )
                {
          // NOTE: for now apply 4th order BC's for sosup order 6 on curvilinear grids:
                    if( !isRectangular )
                        bcOrderOfAccuracy=4;
                }
                realArray f;
                    if( useChargeDensity )
                    {
            // evaluate rho on the boundary to use with the div(eps*E)=rho BC
                        assert( pRho!=NULL );
                        getChargeDensity( t,(*pRho)[grid] );  // do this for now -- evaluate every where ****************
                        f.reference((*pRho)[grid]);
                    } 
                ipar[0] =0;
                ipar[1] =0;
                ipar[2] =I1.getBase();  // not used ???
                ipar[3] =I1.getBound();
                ipar[4] =I2.getBase();
                ipar[5] =I2.getBound();
                ipar[6] =I3.getBase();
                ipar[7] =I3.getBound();
                ipar[8] =gridType;
                ipar[9] =bcOrderOfAccuracy;
                ipar[10]=orderOfExtrapolation;
                ipar[11]=useForcing;
          // apply BCs to field variables
                    ipar[12]=ex;
                    ipar[13]=ey;
                    ipar[14]=ez;
                    ipar[15]=hx;
                    ipar[16]=hy;
                    ipar[17]=hz;
                ipar[18]=useWhereMask;
                ipar[19]=grid;
                #ifdef USE_PPP
                    ipar[20]= 0; // turn off debugging info in parallel -- this can cause trouble
                #else
                    ipar[20]= debug; 
                #endif
                ipar[21]=(int)forcingOption;
                ipar[22]=pmlPower;
                ipar[23]=0;  // do not have pml routine assign interior points too
                ipar[24]=(int)useChargeDensity;
                ipar[25]= adjustFarFieldBoundariesForIncidentField(grid);
        // ipar[26]=bcOpt;  // assigned below
        // int bcSymmetryOption=0;     // 0=even symmetry, 1=even-odd symmetry
                int bcSymmetryOption=1;     // This is the proper symmetry condition *wdh* Sept 6, 2016
                ipar[27]= bcSymmetryOption;
                ipar[28]=myid;
        // -- fieldOption: used for SOSUP to apply BCs to the field or its time-derivative  
                int fieldOption=0;  // apply BCs to field variables
                ipar[29]=fieldOption;
                int numberOfGhostLines = orderOfAccuracyInSpace/2;
                if( addedExtraGhostLine ) numberOfGhostLines++;  // sosup uses one extra ghost line
                ipar[30]=numberOfGhostLines;  // for symmetry BC in bcSymmetry
        // field we subtract off the incident field over this many points next to the boundary.
        // This value should take into account the width of extrapolation used at far-fields
        // For order=2: we may extrap first ghost using 1 -3 3 1 
        // For order=4: we may extrap first ghost using 1 -4 6 -4 1
                int widthForAdjustFieldsForIncident=orderOfAccuracyInSpace/2+1; 
                if( orderOfAccuracyInSpace>2 )
                    widthForAdjustFieldsForIncident+=1;  // *wdh* ABC 4th-order corners needs 1 more 
                ipar[31]=widthForAdjustFieldsForIncident;
                ipar[32]=boundaryForcingOption;
        // supply polarizationOption for dispersive models *wdh* May 29, 2017
                int polarizationOption=0;
                ipar[33]=polarizationOption;
                ipar[34]=dispersionModel;
                ipar[35]=dbase.get<int>("smoothBoundingBox"); // 1= smooth the IC at the bounding box edge
                rpar[0]=dx[0];       // for Cartesian grids          
                rpar[1]=dx[1];                
                rpar[2]=dx[2];                
                rpar[3]=mg.gridSpacing(0);
                rpar[4]=mg.gridSpacing(1);
                rpar[5]=mg.gridSpacing(2);
                rpar[6]=t;
                rpar[7]=(real &)tz;  // twilight zone pointer
                rpar[8]=dt;
                rpar[9]=c;
                rpar[10]=eps;
                rpar[11]=mu;
                rpar[12]=kx; // for plane wave scattering
                rpar[13]=ky;
                rpar[14]=kz;
                rpar[15]=slowStartInterval;
                rpar[16]=pmlLayerStrength;
                realArray *pu = &u;
                rpar[17]=(real&)(pu);  // pass pointer to u for calling updateGhostBoundaries
                rpar[20]=pwc[0];  // coeff. for plane wave solution
                rpar[21]=pwc[1];
                rpar[22]=pwc[2];
                rpar[23]=pwc[3];
                rpar[24]=pwc[4];
                rpar[25]=pwc[5];
                rpar[26]=xab[0][0];   // for Cartesian grids     
                rpar[27]=xab[0][1];
                rpar[28]=xab[0][2];
        // Chirped plane-wave parameters
                const ChirpedArrayType & cpw = dbase.get<ChirpedArrayType>("chirpedParameters");
                rpar[29]=cpw(0); // ta 
                rpar[30]=cpw(1); // tb 
                rpar[31]=cpw(2); // alpha
                rpar[32]=cpw(3); // beta
                rpar[33]=cpw(4); // amp
                rpar[34]=cpw(5); // x0
                rpar[35]=cpw(6); // y0
                rpar[36]=cpw(7); // z0
        // Dispersion parameters:
                real sr=0.,si=0.;  // Re(s), Im(s) in exp(s*t) 
                real ap=0., bp=0., cp=0.;
                if( dispersionModel !=noDispersion )
                {
                    DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
                    const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz); // true wave-number (note factor of twoPi)
                    dmp.computeDispersionRelation( c,eps,mu,kk, sr,si );
          // P equation is P_t + ap*P_t + bp*P = cp*E 
                    ap=dmp.gamma;
                    bp=0.;
                    cp=eps*SQR(dmp.omegap);
                }
                rpar[37]=sr;
                rpar[38]=si;
                rpar[39]=ap;
                rpar[40]=bp;
                rpar[41]=cp;
        // fprintf(pDebugFile,"**** pu= %i, %i...\n",&u,pu);
            #ifdef USE_PPP 
                realSerialArray uu;    getLocalArrayWithGhostBoundaries(u,uu);
                realSerialArray uuOld; getLocalArrayWithGhostBoundaries(uOld,uuOld);
                intSerialArray  mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                realSerialArray rx;    if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rx);
                realSerialArray xy;    if( centerNeeded ) getLocalArrayWithGhostBoundaries(mg.center(),xy);
                realSerialArray ff;    getLocalArrayWithGhostBoundaries(f,ff); 
                if( debug & 4 )
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
                const IntegerArray & gid = mg.gridIndexRange();
                const IntegerArray & dim = mg.dimension();
                const IntegerArray & bc = mg.boundaryCondition();
                if( debug & 1 )
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
                if( !isRectangular )
                {
          // display(mg.inverseVertexDerivative(),"inverseVertexDerivative","%7.4f ");
          // displayMask(mg.mask());
                }
        // Do this for now -- assumes all sides are PML
                bool usePML = (mg.boundaryCondition(0,0)==abcPML || mg.boundaryCondition(1,0)==abcPML ||
                         		 mg.boundaryCondition(0,1)==abcPML || mg.boundaryCondition(1,1)==abcPML ||
                         		 mg.boundaryCondition(0,2)==abcPML || mg.boundaryCondition(1,2)==abcPML);
                const int bc0=-1;  // do all boundaries.
                int ierr=0;
        // *wdh* 090509 -- symmetry BC's (like a straight PEC wall)
                int bcOption=0;     // 0=assign all faces, 1=assign corners and edges
                ipar[26]=bcOption;
                bcSymmetry( mg.numberOfDimensions(), 
                        	      uu.getBase(0),uu.getBound(0),
                        	      uu.getBase(1),uu.getBound(1),
                        	      uu.getBase(2),uu.getBound(2),
                        	      *gid.getDataPointer(),
                        	      *uptr, *maskptr,*rxptr, *xyptr,
                        	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
        // *** need to fix gridIndex Range and bc ***********************
                if( debug & 4 )
                {
                    ::display(uu,sPrintF("uu before bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                }
        // ***** NOTE: PEC boundary values are set in cornersMx routines *******
                bcOptMaxwell( mg.numberOfDimensions(), 
                        		uu.getBase(0),uu.getBound(0),
                        		uu.getBase(1),uu.getBound(1),
                        		uu.getBase(2),uu.getBound(2),
                        		ff.getBase(0),ff.getBound(0),
                        		ff.getBase(1),ff.getBound(1),
                        		ff.getBase(2),ff.getBound(2),
                        		*gid.getDataPointer(),*dim.getDataPointer(),
                        		*uptr,*fptr,*maskptr,*rxptr, *xyptr,
                        		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                if( debug & 4  ) ::display(uu,sPrintF("uu after bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                real *uOldptr = uuOld.getDataPointer();
        // Here we subtract off the incident field on points near non-reflecting boundaries
        // that also have an incoming incident field. Then the NRBC only operates on the scattered 
        // field portion of the total field.
        // Later on below we add the incident field back on 
                realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                #ifdef USE_PPP 
                    realSerialArray uum;    getLocalArrayWithGhostBoundaries(um,uum);
                #else
                    realSerialArray & uum =um;
                #endif
                const int adjustThreeLevels = usePML;
                if( true && adjustFarFieldBoundariesForIncidentField(grid) )
                {
          // printF(" ***** adjustFarFieldBoundariesForIncidentField for grid %i ********\n",grid);
                    if( debug & 4 )
                    {
                        ::display(um(all,all,all,hz),sPrintF("um (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        ::display(u (all,all,all,hz),sPrintF("un (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                    }
                    ipar[25]=-1;  // subtract the incident field
                    ipar[26]=numberLinesForPML;
                    ipar[27]=adjustThreeLevels;
          // parameters for tanh smoothing near bounding box front:
          // -- this must match the formula in getInitialConditions.bC
                    const int & side = dbase.get<int>("boundingBoxDecaySide");
                    const int & axis = dbase.get<int>("boundingBoxDecayAxis");
                    real beta=boundingBoxDecayExponent/twoPi;
                    real nv[3]={0.,0.,0.};  // normal to decay direction
                    nv[axis]=2*side-1;
          // Damp near the point xv0[] on the front
                    real xv0[3]={0.,0.,0.};  // normal to decay direction
                    for( int dir=0; dir<numberOfDimensions; dir++ )
                        xv0[dir] = .5*(initialConditionBoundingBox(1,dir)+initialConditionBoundingBox(0,dir));
                    xv0[axis]=initialConditionBoundingBox(side,axis);
                    rpar[29]=beta;
                    rpar[30]=nv[0];
                    rpar[31]=nv[1];
                    rpar[32]=nv[2];
                    rpar[33]=xv0[0];
                    rpar[34]=xv0[1];
                    rpar[35]=xv0[2];
                    adjustForIncident( mg.numberOfDimensions(),  
                        		uu.getBase(0),uu.getBound(0),
                        		uu.getBase(1),uu.getBound(1),
                        		uu.getBase(2),uu.getBound(2),
                        		*gid.getDataPointer(),
                        		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                        		*initialConditionBoundingBox.getDataPointer(),
                        		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                    ipar[25]=0;
                    if( debug & 4 )
                    {
                        ::display(um(all,all,all,hz),sPrintF("um (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        ::display(u (all,all,all,hz),sPrintF("un (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                    }
                }
        // Non-reflecting and Absorbing boundary conditions
        // ***NOTE*** symmetry corners and edges are assigned in this next routine *fix me*
                abcMaxwell( mg.numberOfDimensions(), 
                        	      uu.getBase(0),uu.getBound(0),
                        	      uu.getBase(1),uu.getBound(1),
                        	      uu.getBase(2),uu.getBound(2),
                        	      ff.getBase(0),ff.getBound(0),
                        	      ff.getBase(1),ff.getBound(1),
                        	      ff.getBase(2),ff.getBound(2),
                        	      *gid.getDataPointer(),
                        	      *uOldptr, *uptr, *fptr,*maskptr,*rxptr, *xyptr,
                        	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
        // ** we should probably assign the PML before all the other BC's since it is like an interior equation **
        //   ** but watch out for the adjustment for the incident field ***
                if( usePML )
                {
                        assert( cgp!=NULL );
                        CompositeGrid & cg= *cgp;
                        realMappedGridFunction & un = u;    // u[next];
            // realMappedGridFunction & uu = uOld; // u[current];
                        const int prev= (current-1+numberOfTimeLevels) % numberOfTimeLevels;
                        const int next = (current+1) % numberOfTimeLevels;
            // realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                        Range all;
            // ::display(um(all,all,all,hz),"um before pml BC's","%9.2e ");
            // ::display(u(all,all,all,hz) ,"u  before pml BC's","%9.2e ");
            // ::display(un(all,all,all,hz),"un before pml BC's","%9.2e ");
            // *********** In parallel we need to allocate local arrays **********
            //   *** We then need to define a ghost boundary update for these serial arrays ***
            // We should do this:  PML(n,side,axis,grid) -> time level n : vwpml(I1,I2,I3,0:1) <- store v,w in this array 
            // current way: 
            // PML(n,m,side,axis,grid)      n=time-level, m=v,w 
                        const int numberOfPMLFunctions=2;  //  v and w
                        const int numberOfComponentsPML=3; // store Ex, Ey, Hz or Ex,Ey,Ez
                        #define PML(n,m,side,axis,grid) vpml[(n)+numberOfTimeLevels*(m+numberOfPMLFunctions*(side+2*(axis+3*(grid))))]
                        #define VPML(n,side,axis,grid) PML(n,0,side,axis,grid)
                        #define WPML(n,side,axis,grid) PML(n,1,side,axis,grid)
                        if( vpml==NULL )
                        {
              // *** No need to allocate PML arrays for all grids !! ***
                            vpml= new RealArray [cg.numberOfComponentGrids()*3*2*numberOfTimeLevels*numberOfPMLFunctions];
              // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                            int *& pmlWasIntitialized =  dbase.put<int*>("pmlWasInitialized");
                            pmlWasIntitialized= new int[cg.numberOfComponentGrids()];   // who will delete this ?
                            for( int g=0; g<cg.numberOfComponentGrids(); g++ )
                                pmlWasIntitialized[g]=false;
                        }
            // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                        int *& pmlWasIntitialized =dbase.get<int*>("pmlWasInitialized");
                        if( !pmlWasIntitialized[grid] )
                        {
                            pmlWasIntitialized[grid]=true;
                            printF(" ****** assignBC: allocate vpml arrays grid=%i, numberOfTimeLevels=%i numberOfPMLFunctions=%i ***** \n",
                           	 grid,numberOfTimeLevels,numberOfPMLFunctions);
                            const int numGhost = orderOfAccuracyInSpace/2;  // we need ghost values in the PML functions *wdh* 2011/12/02
                            for( int side=0; side<=1; side++ )
                            {
                                for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
                                {
                                    if( mg.boundaryCondition(side,axis)==abcPML )
                                    {
                            	  for( int m=0; m<numberOfPMLFunctions; m++ )  // ********* FIX ********
                              	    for( int n=0; n<numberOfTimeLevels; n++ )
                              	    {
                                	      RealArray & vw = PML(n,m,side,axis,grid);
                                	      int ndr[2][3];
                                	      for( int dir=0; dir<3; dir++ )
                                	      {
                                		ndr[0][dir]=mg.dimension(0,dir);
                                		ndr[1][dir]=mg.dimension(1,dir);
                                	      }
                                	      if( side==0 )
                                	      {
                                		ndr[0][axis]=mg.dimension(side,axis);
                                		ndr[1][axis]=mg.gridIndexRange(side,axis)+numberLinesForPML-1 +numGhost;
                                	      }
                                	      else
                                	      {
                                		ndr[0][axis]=mg.gridIndexRange(side,axis)-numberLinesForPML+1 -numGhost;
                                		ndr[1][axis]=mg.dimension(side,axis);
                                	      }
          	      // RealArray a;
          	      // a.redim(Range(-2,10),Range(0,0));
                                	      vw .redim(Range(ndr[0][0],ndr[1][0]),
                                      			Range(ndr[0][1],ndr[1][1]),
                                      			Range(ndr[0][2],ndr[1][2]),numberOfComponentsPML);  // ********* FIX ********
                                	      vw=0.;
                              	    }
                          	}
                                }
                            }
                        } // end if pmlWasInitialized
                      #ifdef USE_PPP
                        realSerialArray uum; getLocalArrayWithGhostBoundaries(um,uum);
                        realSerialArray uu;  getLocalArrayWithGhostBoundaries(uOld,uu);
                        realSerialArray uun; getLocalArrayWithGhostBoundaries(un,uun);
          //   realSerialArray vram; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,0,grid),vram); 
          //   realSerialArray vrbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,0,grid),vrbm); 
          //   realSerialArray vsam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,1,grid),vsam); 
          //   realSerialArray vsbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,1,grid),vsbm); 
          //   realSerialArray vtam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,2,grid),vtam); 
          //   realSerialArray vtbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,2,grid),vtbm); 
          //   realSerialArray vra ; getLocalArrayWithGhostBoundaries(VPML(current,0,0,grid),vra ); 
          //   realSerialArray vrb ; getLocalArrayWithGhostBoundaries(VPML(current,1,0,grid),vrb ); 
          //   realSerialArray vsa ; getLocalArrayWithGhostBoundaries(VPML(current,0,1,grid),vsa ); 
          //   realSerialArray vsb ; getLocalArrayWithGhostBoundaries(VPML(current,1,1,grid),vsb ); 
          //   realSerialArray vta ; getLocalArrayWithGhostBoundaries(VPML(current,0,2,grid),vta ); 
          //   realSerialArray vtb ; getLocalArrayWithGhostBoundaries(VPML(current,1,2,grid),vtb ); 
          //   realSerialArray vran; getLocalArrayWithGhostBoundaries(VPML(next   ,0,0,grid),vran); 
          //   realSerialArray vrbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,0,grid),vrbn); 
          //   realSerialArray vsan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,1,grid),vsan); 
          //   realSerialArray vsbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,1,grid),vsbn); 
          //   realSerialArray vtan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,2,grid),vtan); 
          //   realSerialArray vtbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,2,grid),vtbn); 
          //   realSerialArray wram; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,0,grid),wram); 
          //   realSerialArray wrbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,0,grid),wrbm); 
          //   realSerialArray wsam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,1,grid),wsam); 
          //   realSerialArray wsbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,1,grid),wsbm); 
          //   realSerialArray wtam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,2,grid),wtam); 
          //   realSerialArray wtbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,2,grid),wtbm); 
          //   realSerialArray wra ; getLocalArrayWithGhostBoundaries(WPML(current,0,0,grid),wra ); 
          //   realSerialArray wrb ; getLocalArrayWithGhostBoundaries(WPML(current,1,0,grid),wrb ); 
          //   realSerialArray wsa ; getLocalArrayWithGhostBoundaries(WPML(current,0,1,grid),wsa ); 
          //   realSerialArray wsb ; getLocalArrayWithGhostBoundaries(WPML(current,1,1,grid),wsb ); 
          //   realSerialArray wta ; getLocalArrayWithGhostBoundaries(WPML(current,0,2,grid),wta ); 
          //   realSerialArray wtb ; getLocalArrayWithGhostBoundaries(WPML(current,1,2,grid),wtb ); 
          //   realSerialArray wran; getLocalArrayWithGhostBoundaries(WPML(next   ,0,0,grid),wran); 
          //   realSerialArray wrbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,0,grid),wrbn); 
          //   realSerialArray wsan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,1,grid),wsan); 
          //   realSerialArray wsbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,1,grid),wsbn); 
          //   realSerialArray wtan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,2,grid),wtan); 
          //   realSerialArray wtbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,2,grid),wtbn); 
                      #else
                        const realSerialArray & uum = um;
                        const realSerialArray & uu  = uOld;
                        const realSerialArray & uun = un;
                      #endif
                        const realSerialArray & vram = VPML(prev   ,0,0,grid); 
                        const realSerialArray & vrbm = VPML(prev   ,1,0,grid); 
                        const realSerialArray & vsam = VPML(prev   ,0,1,grid); 
                        const realSerialArray & vsbm = VPML(prev   ,1,1,grid); 
                        const realSerialArray & vtam = VPML(prev   ,0,2,grid); 
                        const realSerialArray & vtbm = VPML(prev   ,1,2,grid); 
                        const realSerialArray & vra  = VPML(current,0,0,grid); 
                        const realSerialArray & vrb  = VPML(current,1,0,grid); 
                        const realSerialArray & vsa  = VPML(current,0,1,grid); 
                        const realSerialArray & vsb  = VPML(current,1,1,grid); 
                        const realSerialArray & vta  = VPML(current,0,2,grid); 
                        const realSerialArray & vtb  = VPML(current,1,2,grid); 
                        const realSerialArray & vran = VPML(next   ,0,0,grid); 
                        const realSerialArray & vrbn = VPML(next   ,1,0,grid); 
                        const realSerialArray & vsan = VPML(next   ,0,1,grid); 
                        const realSerialArray & vsbn = VPML(next   ,1,1,grid); 
                        const realSerialArray & vtan = VPML(next   ,0,2,grid); 
                        const realSerialArray & vtbn = VPML(next   ,1,2,grid); 
                        const realSerialArray & wram = WPML(prev   ,0,0,grid); 
                        const realSerialArray & wrbm = WPML(prev   ,1,0,grid); 
                        const realSerialArray & wsam = WPML(prev   ,0,1,grid); 
                        const realSerialArray & wsbm = WPML(prev   ,1,1,grid); 
                        const realSerialArray & wtam = WPML(prev   ,0,2,grid); 
                        const realSerialArray & wtbm = WPML(prev   ,1,2,grid); 
                        const realSerialArray & wra  = WPML(current,0,0,grid); 
                        const realSerialArray & wrb  = WPML(current,1,0,grid); 
                        const realSerialArray & wsa  = WPML(current,0,1,grid); 
                        const realSerialArray & wsb  = WPML(current,1,1,grid); 
                        const realSerialArray & wta  = WPML(current,0,2,grid); 
                        const realSerialArray & wtb  = WPML(current,1,2,grid); 
                        const realSerialArray & wran = WPML(next   ,0,0,grid); 
                        const realSerialArray & wrbn = WPML(next   ,1,0,grid); 
                        const realSerialArray & wsan = WPML(next   ,0,1,grid); 
                        const realSerialArray & wsbn = WPML(next   ,1,1,grid); 
                        const realSerialArray & wtan = WPML(next   ,0,2,grid); 
                        const realSerialArray & wtbn = WPML(next   ,1,2,grid); 
                        real *umptr, *uuptr, *unptr;   
                        umptr=uum.getDataPointer();
                        uuptr= uu.getDataPointer();  
                        unptr=uun.getDataPointer();
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uu(all,all,all,hz),sPrintF("u  (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(un(all,all,all,hz),sPrintF("un (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
            // Here is the box outside of which the PML equations are applied.
                        getBoundsForPML(mg,Iv);
                        int includeGhost=0;
                        bool ok = ParallelUtility::getLocalArrayBounds(uOld,uu,I1,I2,I3,includeGhost);
                        if( ok )
                        {
                            ipar[2] =I1.getBase();
                            ipar[3] =I1.getBound();
                            ipar[4] =I2.getBase();
                            ipar[5] =I2.getBound();
                            ipar[6] =I3.getBase();
                            ipar[7] =I3.getBound();
                            assert( dx[0]>0. );
                            int bc0=-1;  // not used
              // ** for( int m=0; m<3; m++ )
                            for( int m=0; m<3; m++ )
                            {
                          	ipar[12]=ex+m; // assign this component
                          	pmlMaxwell( mg.numberOfDimensions(), 
                                    		    uu.getBase(0),uu.getBound(0),
                                    		    uu.getBase(1),uu.getBound(1),
                                    		    uu.getBase(2),uu.getBound(2),
                                    		    ff.getBase(0),ff.getBound(0),
                                    		    ff.getBase(1),ff.getBound(1),
                                    		    ff.getBase(2),ff.getBound(2),
                                    		    *gid.getDataPointer(),
                                    		    *dim.getDataPointer(),
                                    		    *umptr, *uuptr, *unptr, 
          		    // vra (left)
                                    		    vra.getBase(0),vra.getBound(0),vra.getBase(1),vra.getBound(1),vra.getBase(2),vra.getBound(2),
                                    		    *vram.getDataPointer(),*vra.getDataPointer(),*vran.getDataPointer(),
                                    		    *wram.getDataPointer(),*wra.getDataPointer(),*wran.getDataPointer(),
          		    // vrb (right)
                                    		    vrb.getBase(0),vrb.getBound(0),vrb.getBase(1),vrb.getBound(1),vrb.getBase(2),vrb.getBound(2),
                                    		    *vrbm.getDataPointer(),*vrb.getDataPointer(),*vrbn.getDataPointer(),
                                    		    *wrbm.getDataPointer(),*wrb.getDataPointer(),*wrbn.getDataPointer(),
          		    // vsa (bottom)
                                    		    vsa.getBase(0),vsa.getBound(0),vsa.getBase(1),vsa.getBound(1),vsa.getBase(2),vsa.getBound(2),
                                    		    *vsam.getDataPointer(),*vsa.getDataPointer(),*vsan.getDataPointer(),
                                    		    *wsam.getDataPointer(),*wsa.getDataPointer(),*wsan.getDataPointer(),
          		    // vsb 
                                    		    vsb.getBase(0),vsb.getBound(0),vsb.getBase(1),vsb.getBound(1),vsb.getBase(2),vsb.getBound(2),
                                    		    *vsbm.getDataPointer(),*vsb.getDataPointer(),*vsbn.getDataPointer(),
                                    		    *wsbm.getDataPointer(),*wsb.getDataPointer(),*wsbn.getDataPointer(),
          		    // vta
                                    		    vta.getBase(0),vta.getBound(0),vta.getBase(1),vta.getBound(1),vta.getBase(2),vta.getBound(2),
                                    		    *vtam.getDataPointer(),*vta.getDataPointer(),*vtan.getDataPointer(),
                                    		    *wtam.getDataPointer(),*wta.getDataPointer(),*wtan.getDataPointer(),
          		    // vtb 
                                    		    vtb.getBase(0),vtb.getBound(0),vtb.getBase(1),vtb.getBound(1),vtb.getBase(2),vtb.getBound(2),
                                    		    *vtbm.getDataPointer(),*vtb.getDataPointer(),*vtbn.getDataPointer(),
                                    		    *wtbm.getDataPointer(),*wtb.getDataPointer(),*wtbn.getDataPointer(),
                                    		    *fptr,*maskptr,*rxptr, *xyptr,
                                    		    bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                            }  // end m
                            ipar[12]=ex;
                        }
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uu(all,all,all,hz),sPrintF("u  (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(un(all,all,all,hz),sPrintF("un (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
                }
        // *wdh* 090509 -- symmetry CORNERS BC's (like a straight PEC wall)
                bcOption=1; // 1=assign corners and edges only
                ipar[26]=bcOption; 
                bcSymmetry( mg.numberOfDimensions(), 
                        	      uu.getBase(0),uu.getBound(0),
                        	      uu.getBase(1),uu.getBound(1),
                        	      uu.getBase(2),uu.getBound(2),
                        	      *gid.getDataPointer(),
                        	      *uptr, *maskptr,*rxptr, *xyptr,
                        	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
        // ::display(u,"u after pml BC's","%9.2e ");
        // assign any radiation BC's
                for( int i=0; i<2; i++ )
                {
          // ** FIX ME for SOSUP ***
                    if( radbcGrid[i]==grid )
                    {
                        RadiationBoundaryCondition::debug=debug;
                        radiationBoundaryCondition[i].tz=tz; // fix this 
                        radiationBoundaryCondition[i].assignBoundaryConditions( u,t,dt,uOld );
                    }
                }
                if( adjustFarFieldBoundariesForIncidentField(grid) )
                {
                    ipar[25]=+1;  // add back the incident field
                    ipar[26]=numberLinesForPML;
                    ipar[27]=adjustThreeLevels;
                    adjustForIncident( mg.numberOfDimensions(),  
                        		uu.getBase(0),uu.getBound(0),
                        		uu.getBase(1),uu.getBound(1),
                        		uu.getBase(2),uu.getBound(2),
                        		*gid.getDataPointer(),
                        		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                                            *initialConditionBoundingBox.getDataPointer(),
                        		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                    ipar[25]=0;
                }
            } // end ok 

            if( method==sosup )
            {
        // -- apply BCs to the time derivatives of the field --
        // *wdh* 2011/12/02 -- this next line was wrong -- side and axis are not correct here.
        // *wdh* getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
        // I1,I2,I3 are not used I don't think. We do check that there are any points on this processor
                getIndex(mg.gridIndexRange(),I1,I2,I3);
                bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
                if( ok && useOpt )
                {
          // use optimised boundary conditions
                    int ipar[40];
                    real rpar[50];
                    int gridType = isRectangular ? 0 : 1;
                    int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
                    int useWhereMask=false;
                    int bcOrderOfAccuracy=orderOfAccuracyInSpace;
                    if( method==sosup && orderOfAccuracyInSpace==6 )
                    {
            // NOTE: for now apply 4th order BC's for sosup order 6 on curvilinear grids:
                        if( !isRectangular )
                            bcOrderOfAccuracy=4;
                    }
                    realArray f;
                    ipar[0] =0;
                    ipar[1] =0;
                    ipar[2] =I1.getBase();  // not used ???
                    ipar[3] =I1.getBound();
                    ipar[4] =I2.getBase();
                    ipar[5] =I2.getBound();
                    ipar[6] =I3.getBase();
                    ipar[7] =I3.getBound();
                    ipar[8] =gridType;
                    ipar[9] =bcOrderOfAccuracy;
                    ipar[10]=orderOfExtrapolation;
                    ipar[11]=useForcing;
            // apply BCs to time-derivatives
                        ipar[12]=ext;
                        ipar[13]=eyt;
                        ipar[14]=ezt;
                        ipar[15]=hxt;
                        ipar[16]=hyt;
                        ipar[17]=hzt;
                    ipar[18]=useWhereMask;
                    ipar[19]=grid;
                    #ifdef USE_PPP
                        ipar[20]= 0; // turn off debugging info in parallel -- this can cause trouble
                    #else
                        ipar[20]= debug; 
                    #endif
                    ipar[21]=(int)forcingOption;
                    ipar[22]=pmlPower;
                    ipar[23]=0;  // do not have pml routine assign interior points too
                    ipar[24]=(int)useChargeDensity;
                    ipar[25]= adjustFarFieldBoundariesForIncidentField(grid);
          // ipar[26]=bcOpt;  // assigned below
          // int bcSymmetryOption=0;     // 0=even symmetry, 1=even-odd symmetry
                    int bcSymmetryOption=1;     // This is the proper symmetry condition *wdh* Sept 6, 2016
                    ipar[27]= bcSymmetryOption;
                    ipar[28]=myid;
          // -- fieldOption: used for SOSUP to apply BCs to the field or its time-derivative  
                    int fieldOption=0;  // apply BCs to field variables
                        fieldOption=1; // apply BCs to time-derivatives
                    ipar[29]=fieldOption;
                    int numberOfGhostLines = orderOfAccuracyInSpace/2;
                    if( addedExtraGhostLine ) numberOfGhostLines++;  // sosup uses one extra ghost line
                    ipar[30]=numberOfGhostLines;  // for symmetry BC in bcSymmetry
          // field we subtract off the incident field over this many points next to the boundary.
          // This value should take into account the width of extrapolation used at far-fields
          // For order=2: we may extrap first ghost using 1 -3 3 1 
          // For order=4: we may extrap first ghost using 1 -4 6 -4 1
                    int widthForAdjustFieldsForIncident=orderOfAccuracyInSpace/2+1; 
                    if( orderOfAccuracyInSpace>2 )
                        widthForAdjustFieldsForIncident+=1;  // *wdh* ABC 4th-order corners needs 1 more 
                    ipar[31]=widthForAdjustFieldsForIncident;
                    ipar[32]=boundaryForcingOption;
          // supply polarizationOption for dispersive models *wdh* May 29, 2017
                    int polarizationOption=0;
                    ipar[33]=polarizationOption;
                    ipar[34]=dispersionModel;
                    ipar[35]=dbase.get<int>("smoothBoundingBox"); // 1= smooth the IC at the bounding box edge
                    rpar[0]=dx[0];       // for Cartesian grids          
                    rpar[1]=dx[1];                
                    rpar[2]=dx[2];                
                    rpar[3]=mg.gridSpacing(0);
                    rpar[4]=mg.gridSpacing(1);
                    rpar[5]=mg.gridSpacing(2);
                    rpar[6]=t;
                    rpar[7]=(real &)tz;  // twilight zone pointer
                    rpar[8]=dt;
                    rpar[9]=c;
                    rpar[10]=eps;
                    rpar[11]=mu;
                    rpar[12]=kx; // for plane wave scattering
                    rpar[13]=ky;
                    rpar[14]=kz;
                    rpar[15]=slowStartInterval;
                    rpar[16]=pmlLayerStrength;
                    realArray *pu = &u;
                    rpar[17]=(real&)(pu);  // pass pointer to u for calling updateGhostBoundaries
                    rpar[20]=pwc[0];  // coeff. for plane wave solution
                    rpar[21]=pwc[1];
                    rpar[22]=pwc[2];
                    rpar[23]=pwc[3];
                    rpar[24]=pwc[4];
                    rpar[25]=pwc[5];
                    rpar[26]=xab[0][0];   // for Cartesian grids     
                    rpar[27]=xab[0][1];
                    rpar[28]=xab[0][2];
          // Chirped plane-wave parameters
                    const ChirpedArrayType & cpw = dbase.get<ChirpedArrayType>("chirpedParameters");
                    rpar[29]=cpw(0); // ta 
                    rpar[30]=cpw(1); // tb 
                    rpar[31]=cpw(2); // alpha
                    rpar[32]=cpw(3); // beta
                    rpar[33]=cpw(4); // amp
                    rpar[34]=cpw(5); // x0
                    rpar[35]=cpw(6); // y0
                    rpar[36]=cpw(7); // z0
          // Dispersion parameters:
                    real sr=0.,si=0.;  // Re(s), Im(s) in exp(s*t) 
                    real ap=0., bp=0., cp=0.;
                    if( dispersionModel !=noDispersion )
                    {
                        DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
                        const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz); // true wave-number (note factor of twoPi)
                        dmp.computeDispersionRelation( c,eps,mu,kk, sr,si );
            // P equation is P_t + ap*P_t + bp*P = cp*E 
                        ap=dmp.gamma;
                        bp=0.;
                        cp=eps*SQR(dmp.omegap);
                    }
                    rpar[37]=sr;
                    rpar[38]=si;
                    rpar[39]=ap;
                    rpar[40]=bp;
                    rpar[41]=cp;
          // fprintf(pDebugFile,"**** pu= %i, %i...\n",&u,pu);
                #ifdef USE_PPP 
                    realSerialArray uu;    getLocalArrayWithGhostBoundaries(u,uu);
                    realSerialArray uuOld; getLocalArrayWithGhostBoundaries(uOld,uuOld);
                    intSerialArray  mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                    realSerialArray rx;    if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rx);
                    realSerialArray xy;    if( centerNeeded ) getLocalArrayWithGhostBoundaries(mg.center(),xy);
                    realSerialArray ff;    getLocalArrayWithGhostBoundaries(f,ff); 
                    if( debug & 4 )
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
                    const IntegerArray & gid = mg.gridIndexRange();
                    const IntegerArray & dim = mg.dimension();
                    const IntegerArray & bc = mg.boundaryCondition();
                    if( debug & 1 )
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
                    if( !isRectangular )
                    {
            // display(mg.inverseVertexDerivative(),"inverseVertexDerivative","%7.4f ");
            // displayMask(mg.mask());
                    }
          // Do this for now -- assumes all sides are PML
                    bool usePML = (mg.boundaryCondition(0,0)==abcPML || mg.boundaryCondition(1,0)==abcPML ||
                             		 mg.boundaryCondition(0,1)==abcPML || mg.boundaryCondition(1,1)==abcPML ||
                             		 mg.boundaryCondition(0,2)==abcPML || mg.boundaryCondition(1,2)==abcPML);
                    const int bc0=-1;  // do all boundaries.
                    int ierr=0;
          // *wdh* 090509 -- symmetry BC's (like a straight PEC wall)
                    int bcOption=0;     // 0=assign all faces, 1=assign corners and edges
                    ipar[26]=bcOption;
                    bcSymmetry( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uptr, *maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // *** need to fix gridIndex Range and bc ***********************
                    if( debug & 4 )
                    {
                        ::display(uu,sPrintF("uu before bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                    }
          // ***** NOTE: PEC boundary values are set in cornersMx routines *******
                    bcOptMaxwell( mg.numberOfDimensions(), 
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		ff.getBase(0),ff.getBound(0),
                            		ff.getBase(1),ff.getBound(1),
                            		ff.getBase(2),ff.getBound(2),
                            		*gid.getDataPointer(),*dim.getDataPointer(),
                            		*uptr,*fptr,*maskptr,*rxptr, *xyptr,
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                    if( debug & 4  ) ::display(uu,sPrintF("uu after bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                    real *uOldptr = uuOld.getDataPointer();
          // Here we subtract off the incident field on points near non-reflecting boundaries
          // that also have an incoming incident field. Then the NRBC only operates on the scattered 
          // field portion of the total field.
          // Later on below we add the incident field back on 
                    realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                    #ifdef USE_PPP 
                        realSerialArray uum;    getLocalArrayWithGhostBoundaries(um,uum);
                    #else
                        realSerialArray & uum =um;
                    #endif
                    const int adjustThreeLevels = usePML;
                    if( true && adjustFarFieldBoundariesForIncidentField(grid) )
                    {
            // printF(" ***** adjustFarFieldBoundariesForIncidentField for grid %i ********\n",grid);
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(u (all,all,all,hz),sPrintF("un (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
                        ipar[25]=-1;  // subtract the incident field
                        ipar[26]=numberLinesForPML;
                        ipar[27]=adjustThreeLevels;
            // parameters for tanh smoothing near bounding box front:
            // -- this must match the formula in getInitialConditions.bC
                        const int & side = dbase.get<int>("boundingBoxDecaySide");
                        const int & axis = dbase.get<int>("boundingBoxDecayAxis");
                        real beta=boundingBoxDecayExponent/twoPi;
                        real nv[3]={0.,0.,0.};  // normal to decay direction
                        nv[axis]=2*side-1;
            // Damp near the point xv0[] on the front
                        real xv0[3]={0.,0.,0.};  // normal to decay direction
                        for( int dir=0; dir<numberOfDimensions; dir++ )
                            xv0[dir] = .5*(initialConditionBoundingBox(1,dir)+initialConditionBoundingBox(0,dir));
                        xv0[axis]=initialConditionBoundingBox(side,axis);
                        rpar[29]=beta;
                        rpar[30]=nv[0];
                        rpar[31]=nv[1];
                        rpar[32]=nv[2];
                        rpar[33]=xv0[0];
                        rpar[34]=xv0[1];
                        rpar[35]=xv0[2];
                        adjustForIncident( mg.numberOfDimensions(),  
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		*gid.getDataPointer(),
                            		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                            		*initialConditionBoundingBox.getDataPointer(),
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                        ipar[25]=0;
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(u (all,all,all,hz),sPrintF("un (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
                    }
          // Non-reflecting and Absorbing boundary conditions
          // ***NOTE*** symmetry corners and edges are assigned in this next routine *fix me*
                    abcMaxwell( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      ff.getBase(0),ff.getBound(0),
                            	      ff.getBase(1),ff.getBound(1),
                            	      ff.getBase(2),ff.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uOldptr, *uptr, *fptr,*maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // ** we should probably assign the PML before all the other BC's since it is like an interior equation **
          //   ** but watch out for the adjustment for the incident field ***
                    if( usePML )
                    {
                            assert( cgp!=NULL );
                            CompositeGrid & cg= *cgp;
                            realMappedGridFunction & un = u;    // u[next];
              // realMappedGridFunction & uu = uOld; // u[current];
                            const int prev= (current-1+numberOfTimeLevels) % numberOfTimeLevels;
                            const int next = (current+1) % numberOfTimeLevels;
              // realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                            Range all;
              // ::display(um(all,all,all,hz),"um before pml BC's","%9.2e ");
              // ::display(u(all,all,all,hz) ,"u  before pml BC's","%9.2e ");
              // ::display(un(all,all,all,hz),"un before pml BC's","%9.2e ");
              // *********** In parallel we need to allocate local arrays **********
              //   *** We then need to define a ghost boundary update for these serial arrays ***
              // We should do this:  PML(n,side,axis,grid) -> time level n : vwpml(I1,I2,I3,0:1) <- store v,w in this array 
              // current way: 
              // PML(n,m,side,axis,grid)      n=time-level, m=v,w 
                            const int numberOfPMLFunctions=2;  //  v and w
                            const int numberOfComponentsPML=3; // store Ex, Ey, Hz or Ex,Ey,Ez
                            #define PML(n,m,side,axis,grid) vpml[(n)+numberOfTimeLevels*(m+numberOfPMLFunctions*(side+2*(axis+3*(grid))))]
                            #define VPML(n,side,axis,grid) PML(n,0,side,axis,grid)
                            #define WPML(n,side,axis,grid) PML(n,1,side,axis,grid)
                            if( vpml==NULL )
                            {
                // *** No need to allocate PML arrays for all grids !! ***
                                vpml= new RealArray [cg.numberOfComponentGrids()*3*2*numberOfTimeLevels*numberOfPMLFunctions];
                // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                                int *& pmlWasIntitialized =  dbase.put<int*>("pmlWasInitialized");
                                pmlWasIntitialized= new int[cg.numberOfComponentGrids()];   // who will delete this ?
                                for( int g=0; g<cg.numberOfComponentGrids(); g++ )
                                    pmlWasIntitialized[g]=false;
                            }
              // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                            int *& pmlWasIntitialized =dbase.get<int*>("pmlWasInitialized");
                            if( !pmlWasIntitialized[grid] )
                            {
                                pmlWasIntitialized[grid]=true;
                                printF(" ****** assignBC: allocate vpml arrays grid=%i, numberOfTimeLevels=%i numberOfPMLFunctions=%i ***** \n",
                               	 grid,numberOfTimeLevels,numberOfPMLFunctions);
                                const int numGhost = orderOfAccuracyInSpace/2;  // we need ghost values in the PML functions *wdh* 2011/12/02
                                for( int side=0; side<=1; side++ )
                                {
                                    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
                                    {
                                        if( mg.boundaryCondition(side,axis)==abcPML )
                                        {
                                	  for( int m=0; m<numberOfPMLFunctions; m++ )  // ********* FIX ********
                                  	    for( int n=0; n<numberOfTimeLevels; n++ )
                                  	    {
                                    	      RealArray & vw = PML(n,m,side,axis,grid);
                                    	      int ndr[2][3];
                                    	      for( int dir=0; dir<3; dir++ )
                                    	      {
                                    		ndr[0][dir]=mg.dimension(0,dir);
                                    		ndr[1][dir]=mg.dimension(1,dir);
                                    	      }
                                    	      if( side==0 )
                                    	      {
                                    		ndr[0][axis]=mg.dimension(side,axis);
                                    		ndr[1][axis]=mg.gridIndexRange(side,axis)+numberLinesForPML-1 +numGhost;
                                    	      }
                                    	      else
                                    	      {
                                    		ndr[0][axis]=mg.gridIndexRange(side,axis)-numberLinesForPML+1 -numGhost;
                                    		ndr[1][axis]=mg.dimension(side,axis);
                                    	      }
            	      // RealArray a;
            	      // a.redim(Range(-2,10),Range(0,0));
                                    	      vw .redim(Range(ndr[0][0],ndr[1][0]),
                                          			Range(ndr[0][1],ndr[1][1]),
                                          			Range(ndr[0][2],ndr[1][2]),numberOfComponentsPML);  // ********* FIX ********
                                    	      vw=0.;
                                  	    }
                              	}
                                    }
                                }
                            } // end if pmlWasInitialized
                          #ifdef USE_PPP
                            realSerialArray uum; getLocalArrayWithGhostBoundaries(um,uum);
                            realSerialArray uu;  getLocalArrayWithGhostBoundaries(uOld,uu);
                            realSerialArray uun; getLocalArrayWithGhostBoundaries(un,uun);
            //   realSerialArray vram; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,0,grid),vram); 
            //   realSerialArray vrbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,0,grid),vrbm); 
            //   realSerialArray vsam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,1,grid),vsam); 
            //   realSerialArray vsbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,1,grid),vsbm); 
            //   realSerialArray vtam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,2,grid),vtam); 
            //   realSerialArray vtbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,2,grid),vtbm); 
            //   realSerialArray vra ; getLocalArrayWithGhostBoundaries(VPML(current,0,0,grid),vra ); 
            //   realSerialArray vrb ; getLocalArrayWithGhostBoundaries(VPML(current,1,0,grid),vrb ); 
            //   realSerialArray vsa ; getLocalArrayWithGhostBoundaries(VPML(current,0,1,grid),vsa ); 
            //   realSerialArray vsb ; getLocalArrayWithGhostBoundaries(VPML(current,1,1,grid),vsb ); 
            //   realSerialArray vta ; getLocalArrayWithGhostBoundaries(VPML(current,0,2,grid),vta ); 
            //   realSerialArray vtb ; getLocalArrayWithGhostBoundaries(VPML(current,1,2,grid),vtb ); 
            //   realSerialArray vran; getLocalArrayWithGhostBoundaries(VPML(next   ,0,0,grid),vran); 
            //   realSerialArray vrbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,0,grid),vrbn); 
            //   realSerialArray vsan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,1,grid),vsan); 
            //   realSerialArray vsbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,1,grid),vsbn); 
            //   realSerialArray vtan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,2,grid),vtan); 
            //   realSerialArray vtbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,2,grid),vtbn); 
            //   realSerialArray wram; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,0,grid),wram); 
            //   realSerialArray wrbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,0,grid),wrbm); 
            //   realSerialArray wsam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,1,grid),wsam); 
            //   realSerialArray wsbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,1,grid),wsbm); 
            //   realSerialArray wtam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,2,grid),wtam); 
            //   realSerialArray wtbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,2,grid),wtbm); 
            //   realSerialArray wra ; getLocalArrayWithGhostBoundaries(WPML(current,0,0,grid),wra ); 
            //   realSerialArray wrb ; getLocalArrayWithGhostBoundaries(WPML(current,1,0,grid),wrb ); 
            //   realSerialArray wsa ; getLocalArrayWithGhostBoundaries(WPML(current,0,1,grid),wsa ); 
            //   realSerialArray wsb ; getLocalArrayWithGhostBoundaries(WPML(current,1,1,grid),wsb ); 
            //   realSerialArray wta ; getLocalArrayWithGhostBoundaries(WPML(current,0,2,grid),wta ); 
            //   realSerialArray wtb ; getLocalArrayWithGhostBoundaries(WPML(current,1,2,grid),wtb ); 
            //   realSerialArray wran; getLocalArrayWithGhostBoundaries(WPML(next   ,0,0,grid),wran); 
            //   realSerialArray wrbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,0,grid),wrbn); 
            //   realSerialArray wsan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,1,grid),wsan); 
            //   realSerialArray wsbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,1,grid),wsbn); 
            //   realSerialArray wtan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,2,grid),wtan); 
            //   realSerialArray wtbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,2,grid),wtbn); 
                          #else
                            const realSerialArray & uum = um;
                            const realSerialArray & uu  = uOld;
                            const realSerialArray & uun = un;
                          #endif
                            const realSerialArray & vram = VPML(prev   ,0,0,grid); 
                            const realSerialArray & vrbm = VPML(prev   ,1,0,grid); 
                            const realSerialArray & vsam = VPML(prev   ,0,1,grid); 
                            const realSerialArray & vsbm = VPML(prev   ,1,1,grid); 
                            const realSerialArray & vtam = VPML(prev   ,0,2,grid); 
                            const realSerialArray & vtbm = VPML(prev   ,1,2,grid); 
                            const realSerialArray & vra  = VPML(current,0,0,grid); 
                            const realSerialArray & vrb  = VPML(current,1,0,grid); 
                            const realSerialArray & vsa  = VPML(current,0,1,grid); 
                            const realSerialArray & vsb  = VPML(current,1,1,grid); 
                            const realSerialArray & vta  = VPML(current,0,2,grid); 
                            const realSerialArray & vtb  = VPML(current,1,2,grid); 
                            const realSerialArray & vran = VPML(next   ,0,0,grid); 
                            const realSerialArray & vrbn = VPML(next   ,1,0,grid); 
                            const realSerialArray & vsan = VPML(next   ,0,1,grid); 
                            const realSerialArray & vsbn = VPML(next   ,1,1,grid); 
                            const realSerialArray & vtan = VPML(next   ,0,2,grid); 
                            const realSerialArray & vtbn = VPML(next   ,1,2,grid); 
                            const realSerialArray & wram = WPML(prev   ,0,0,grid); 
                            const realSerialArray & wrbm = WPML(prev   ,1,0,grid); 
                            const realSerialArray & wsam = WPML(prev   ,0,1,grid); 
                            const realSerialArray & wsbm = WPML(prev   ,1,1,grid); 
                            const realSerialArray & wtam = WPML(prev   ,0,2,grid); 
                            const realSerialArray & wtbm = WPML(prev   ,1,2,grid); 
                            const realSerialArray & wra  = WPML(current,0,0,grid); 
                            const realSerialArray & wrb  = WPML(current,1,0,grid); 
                            const realSerialArray & wsa  = WPML(current,0,1,grid); 
                            const realSerialArray & wsb  = WPML(current,1,1,grid); 
                            const realSerialArray & wta  = WPML(current,0,2,grid); 
                            const realSerialArray & wtb  = WPML(current,1,2,grid); 
                            const realSerialArray & wran = WPML(next   ,0,0,grid); 
                            const realSerialArray & wrbn = WPML(next   ,1,0,grid); 
                            const realSerialArray & wsan = WPML(next   ,0,1,grid); 
                            const realSerialArray & wsbn = WPML(next   ,1,1,grid); 
                            const realSerialArray & wtan = WPML(next   ,0,2,grid); 
                            const realSerialArray & wtbn = WPML(next   ,1,2,grid); 
                            real *umptr, *uuptr, *unptr;   
                            umptr=uum.getDataPointer();
                            uuptr= uu.getDataPointer();  
                            unptr=uun.getDataPointer();
                            if( debug & 4 )
                            {
                                ::display(um(all,all,all,hz),sPrintF("um (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(uu(all,all,all,hz),sPrintF("u  (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(un(all,all,all,hz),sPrintF("un (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            }
              // Here is the box outside of which the PML equations are applied.
                            getBoundsForPML(mg,Iv);
                            int includeGhost=0;
                            bool ok = ParallelUtility::getLocalArrayBounds(uOld,uu,I1,I2,I3,includeGhost);
                            if( ok )
                            {
                                ipar[2] =I1.getBase();
                                ipar[3] =I1.getBound();
                                ipar[4] =I2.getBase();
                                ipar[5] =I2.getBound();
                                ipar[6] =I3.getBase();
                                ipar[7] =I3.getBound();
                                assert( dx[0]>0. );
                                int bc0=-1;  // not used
                // ** for( int m=0; m<3; m++ )
                                for( int m=0; m<3; m++ )
                                {
                              	ipar[12]=ex+m; // assign this component
                              	pmlMaxwell( mg.numberOfDimensions(), 
                                        		    uu.getBase(0),uu.getBound(0),
                                        		    uu.getBase(1),uu.getBound(1),
                                        		    uu.getBase(2),uu.getBound(2),
                                        		    ff.getBase(0),ff.getBound(0),
                                        		    ff.getBase(1),ff.getBound(1),
                                        		    ff.getBase(2),ff.getBound(2),
                                        		    *gid.getDataPointer(),
                                        		    *dim.getDataPointer(),
                                        		    *umptr, *uuptr, *unptr, 
            		    // vra (left)
                                        		    vra.getBase(0),vra.getBound(0),vra.getBase(1),vra.getBound(1),vra.getBase(2),vra.getBound(2),
                                        		    *vram.getDataPointer(),*vra.getDataPointer(),*vran.getDataPointer(),
                                        		    *wram.getDataPointer(),*wra.getDataPointer(),*wran.getDataPointer(),
            		    // vrb (right)
                                        		    vrb.getBase(0),vrb.getBound(0),vrb.getBase(1),vrb.getBound(1),vrb.getBase(2),vrb.getBound(2),
                                        		    *vrbm.getDataPointer(),*vrb.getDataPointer(),*vrbn.getDataPointer(),
                                        		    *wrbm.getDataPointer(),*wrb.getDataPointer(),*wrbn.getDataPointer(),
            		    // vsa (bottom)
                                        		    vsa.getBase(0),vsa.getBound(0),vsa.getBase(1),vsa.getBound(1),vsa.getBase(2),vsa.getBound(2),
                                        		    *vsam.getDataPointer(),*vsa.getDataPointer(),*vsan.getDataPointer(),
                                        		    *wsam.getDataPointer(),*wsa.getDataPointer(),*wsan.getDataPointer(),
            		    // vsb 
                                        		    vsb.getBase(0),vsb.getBound(0),vsb.getBase(1),vsb.getBound(1),vsb.getBase(2),vsb.getBound(2),
                                        		    *vsbm.getDataPointer(),*vsb.getDataPointer(),*vsbn.getDataPointer(),
                                        		    *wsbm.getDataPointer(),*wsb.getDataPointer(),*wsbn.getDataPointer(),
            		    // vta
                                        		    vta.getBase(0),vta.getBound(0),vta.getBase(1),vta.getBound(1),vta.getBase(2),vta.getBound(2),
                                        		    *vtam.getDataPointer(),*vta.getDataPointer(),*vtan.getDataPointer(),
                                        		    *wtam.getDataPointer(),*wta.getDataPointer(),*wtan.getDataPointer(),
            		    // vtb 
                                        		    vtb.getBase(0),vtb.getBound(0),vtb.getBase(1),vtb.getBound(1),vtb.getBase(2),vtb.getBound(2),
                                        		    *vtbm.getDataPointer(),*vtb.getDataPointer(),*vtbn.getDataPointer(),
                                        		    *wtbm.getDataPointer(),*wtb.getDataPointer(),*wtbn.getDataPointer(),
                                        		    *fptr,*maskptr,*rxptr, *xyptr,
                                        		    bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                                }  // end m
                                ipar[12]=ex;
                            }
                            if( debug & 4 )
                            {
                                ::display(um(all,all,all,hz),sPrintF("um (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(uu(all,all,all,hz),sPrintF("u  (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(un(all,all,all,hz),sPrintF("un (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            }
                    }
          // *wdh* 090509 -- symmetry CORNERS BC's (like a straight PEC wall)
                    bcOption=1; // 1=assign corners and edges only
                    ipar[26]=bcOption; 
                    bcSymmetry( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uptr, *maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // ::display(u,"u after pml BC's","%9.2e ");
          // assign any radiation BC's
                    for( int i=0; i<2; i++ )
                    {
            // ** FIX ME for SOSUP ***
                        if( radbcGrid[i]==grid )
                        {
                            RadiationBoundaryCondition::debug=debug;
                            radiationBoundaryCondition[i].tz=tz; // fix this 
                            radiationBoundaryCondition[i].assignBoundaryConditions( u,t,dt,uOld );
                        }
                    }
                    if( adjustFarFieldBoundariesForIncidentField(grid) )
                    {
                        ipar[25]=+1;  // add back the incident field
                        ipar[26]=numberLinesForPML;
                        ipar[27]=adjustThreeLevels;
                        adjustForIncident( mg.numberOfDimensions(),  
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		*gid.getDataPointer(),
                            		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                                                *initialConditionBoundingBox.getDataPointer(),
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                        ipar[25]=0;
                    }
                } // end ok 
            }

            if( dispersionModel != noDispersion )
            {
        // -- apply BCs to the polarization vector
        // *wdh* 2011/12/02 -- this next line was wrong -- side and axis are not correct here.
        // *wdh* getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
        // I1,I2,I3 are not used I don't think. We do check that there are any points on this processor
                getIndex(mg.gridIndexRange(),I1,I2,I3);
                bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
                if( ok && useOpt )
                {
          // use optimised boundary conditions
                    int ipar[40];
                    real rpar[50];
                    int gridType = isRectangular ? 0 : 1;
                    int orderOfExtrapolation=orderOfAccuracyInSpace+1;  // not used
                    int useWhereMask=false;
                    int bcOrderOfAccuracy=orderOfAccuracyInSpace;
                    if( method==sosup && orderOfAccuracyInSpace==6 )
                    {
            // NOTE: for now apply 4th order BC's for sosup order 6 on curvilinear grids:
                        if( !isRectangular )
                            bcOrderOfAccuracy=4;
                    }
                    realArray f;
                    ipar[0] =0;
                    ipar[1] =0;
                    ipar[2] =I1.getBase();  // not used ???
                    ipar[3] =I1.getBound();
                    ipar[4] =I2.getBase();
                    ipar[5] =I2.getBound();
                    ipar[6] =I3.getBase();
                    ipar[7] =I3.getBound();
                    ipar[8] =gridType;
                    ipar[9] =bcOrderOfAccuracy;
                    ipar[10]=orderOfExtrapolation;
                    ipar[11]=useForcing;
            // apply BCs to polarization vectors
                        ipar[12]=pxc;
                        ipar[13]=pyc;
                        ipar[14]=pzc;
                        ipar[15]=qxc;
                        ipar[16]=qyc;
                        if( numberOfDimensions==2 )
                            ipar[17]=hz;  // for now apply BCs to Hz again here 
                        else
                            ipar[17]=qzc;
                    ipar[18]=useWhereMask;
                    ipar[19]=grid;
                    #ifdef USE_PPP
                        ipar[20]= 0; // turn off debugging info in parallel -- this can cause trouble
                    #else
                        ipar[20]= debug; 
                    #endif
                    ipar[21]=(int)forcingOption;
                    ipar[22]=pmlPower;
                    ipar[23]=0;  // do not have pml routine assign interior points too
                    ipar[24]=(int)useChargeDensity;
                    ipar[25]= adjustFarFieldBoundariesForIncidentField(grid);
          // ipar[26]=bcOpt;  // assigned below
          // int bcSymmetryOption=0;     // 0=even symmetry, 1=even-odd symmetry
                    int bcSymmetryOption=1;     // This is the proper symmetry condition *wdh* Sept 6, 2016
                    ipar[27]= bcSymmetryOption;
                    ipar[28]=myid;
          // -- fieldOption: used for SOSUP to apply BCs to the field or its time-derivative  
                    int fieldOption=0;  // apply BCs to field variables
                    ipar[29]=fieldOption;
                    int numberOfGhostLines = orderOfAccuracyInSpace/2;
                    if( addedExtraGhostLine ) numberOfGhostLines++;  // sosup uses one extra ghost line
                    ipar[30]=numberOfGhostLines;  // for symmetry BC in bcSymmetry
          // field we subtract off the incident field over this many points next to the boundary.
          // This value should take into account the width of extrapolation used at far-fields
          // For order=2: we may extrap first ghost using 1 -3 3 1 
          // For order=4: we may extrap first ghost using 1 -4 6 -4 1
                    int widthForAdjustFieldsForIncident=orderOfAccuracyInSpace/2+1; 
                    if( orderOfAccuracyInSpace>2 )
                        widthForAdjustFieldsForIncident+=1;  // *wdh* ABC 4th-order corners needs 1 more 
                    ipar[31]=widthForAdjustFieldsForIncident;
                    ipar[32]=boundaryForcingOption;
          // supply polarizationOption for dispersive models *wdh* May 29, 2017
                    int polarizationOption=0;
                        polarizationOption=1; // apply BCs to the polarization vector
                    ipar[33]=polarizationOption;
                    ipar[34]=dispersionModel;
                    ipar[35]=dbase.get<int>("smoothBoundingBox"); // 1= smooth the IC at the bounding box edge
                    rpar[0]=dx[0];       // for Cartesian grids          
                    rpar[1]=dx[1];                
                    rpar[2]=dx[2];                
                    rpar[3]=mg.gridSpacing(0);
                    rpar[4]=mg.gridSpacing(1);
                    rpar[5]=mg.gridSpacing(2);
                    rpar[6]=t;
                    rpar[7]=(real &)tz;  // twilight zone pointer
                    rpar[8]=dt;
                    rpar[9]=c;
                    rpar[10]=eps;
                    rpar[11]=mu;
                    rpar[12]=kx; // for plane wave scattering
                    rpar[13]=ky;
                    rpar[14]=kz;
                    rpar[15]=slowStartInterval;
                    rpar[16]=pmlLayerStrength;
                    realArray *pu = &u;
                    rpar[17]=(real&)(pu);  // pass pointer to u for calling updateGhostBoundaries
                    rpar[20]=pwc[0];  // coeff. for plane wave solution
                    rpar[21]=pwc[1];
                    rpar[22]=pwc[2];
                    rpar[23]=pwc[3];
                    rpar[24]=pwc[4];
                    rpar[25]=pwc[5];
                    rpar[26]=xab[0][0];   // for Cartesian grids     
                    rpar[27]=xab[0][1];
                    rpar[28]=xab[0][2];
          // Chirped plane-wave parameters
                    const ChirpedArrayType & cpw = dbase.get<ChirpedArrayType>("chirpedParameters");
                    rpar[29]=cpw(0); // ta 
                    rpar[30]=cpw(1); // tb 
                    rpar[31]=cpw(2); // alpha
                    rpar[32]=cpw(3); // beta
                    rpar[33]=cpw(4); // amp
                    rpar[34]=cpw(5); // x0
                    rpar[35]=cpw(6); // y0
                    rpar[36]=cpw(7); // z0
          // Dispersion parameters:
                    real sr=0.,si=0.;  // Re(s), Im(s) in exp(s*t) 
                    real ap=0., bp=0., cp=0.;
                    if( dispersionModel !=noDispersion )
                    {
                        DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);
                        const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz); // true wave-number (note factor of twoPi)
                        dmp.computeDispersionRelation( c,eps,mu,kk, sr,si );
            // P equation is P_t + ap*P_t + bp*P = cp*E 
                        ap=dmp.gamma;
                        bp=0.;
                        cp=eps*SQR(dmp.omegap);
                    }
                    rpar[37]=sr;
                    rpar[38]=si;
                    rpar[39]=ap;
                    rpar[40]=bp;
                    rpar[41]=cp;
          // fprintf(pDebugFile,"**** pu= %i, %i...\n",&u,pu);
                #ifdef USE_PPP 
                    realSerialArray uu;    getLocalArrayWithGhostBoundaries(u,uu);
                    realSerialArray uuOld; getLocalArrayWithGhostBoundaries(uOld,uuOld);
                    intSerialArray  mask;  getLocalArrayWithGhostBoundaries(mg.mask(),mask);
                    realSerialArray rx;    if( !isRectangular ) getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rx);
                    realSerialArray xy;    if( centerNeeded ) getLocalArrayWithGhostBoundaries(mg.center(),xy);
                    realSerialArray ff;    getLocalArrayWithGhostBoundaries(f,ff); 
                    if( debug & 4 )
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
                    const IntegerArray & gid = mg.gridIndexRange();
                    const IntegerArray & dim = mg.dimension();
                    const IntegerArray & bc = mg.boundaryCondition();
                    if( debug & 1 )
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
                    if( !isRectangular )
                    {
            // display(mg.inverseVertexDerivative(),"inverseVertexDerivative","%7.4f ");
            // displayMask(mg.mask());
                    }
          // Do this for now -- assumes all sides are PML
                    bool usePML = (mg.boundaryCondition(0,0)==abcPML || mg.boundaryCondition(1,0)==abcPML ||
                             		 mg.boundaryCondition(0,1)==abcPML || mg.boundaryCondition(1,1)==abcPML ||
                             		 mg.boundaryCondition(0,2)==abcPML || mg.boundaryCondition(1,2)==abcPML);
                    const int bc0=-1;  // do all boundaries.
                    int ierr=0;
          // *wdh* 090509 -- symmetry BC's (like a straight PEC wall)
                    int bcOption=0;     // 0=assign all faces, 1=assign corners and edges
                    ipar[26]=bcOption;
                    bcSymmetry( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uptr, *maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // *** need to fix gridIndex Range and bc ***********************
                    if( debug & 4 )
                    {
                        ::display(uu,sPrintF("uu before bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                    }
          // ***** NOTE: PEC boundary values are set in cornersMx routines *******
                    bcOptMaxwell( mg.numberOfDimensions(), 
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		ff.getBase(0),ff.getBound(0),
                            		ff.getBase(1),ff.getBound(1),
                            		ff.getBase(2),ff.getBound(2),
                            		*gid.getDataPointer(),*dim.getDataPointer(),
                            		*uptr,*fptr,*maskptr,*rxptr, *xyptr,
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                    if( debug & 4  ) ::display(uu,sPrintF("uu after bcOptMaxwell, grid=%i, t=%e",grid,t),pDebugFile,"%8.1e ");
                    real *uOldptr = uuOld.getDataPointer();
          // Here we subtract off the incident field on points near non-reflecting boundaries
          // that also have an incoming incident field. Then the NRBC only operates on the scattered 
          // field portion of the total field.
          // Later on below we add the incident field back on 
                    realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                    #ifdef USE_PPP 
                        realSerialArray uum;    getLocalArrayWithGhostBoundaries(um,uum);
                    #else
                        realSerialArray & uum =um;
                    #endif
                    const int adjustThreeLevels = usePML;
                    if( true && adjustFarFieldBoundariesForIncidentField(grid) )
                    {
            // printF(" ***** adjustFarFieldBoundariesForIncidentField for grid %i ********\n",grid);
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(u (all,all,all,hz),sPrintF("un (Hz) before adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
                        ipar[25]=-1;  // subtract the incident field
                        ipar[26]=numberLinesForPML;
                        ipar[27]=adjustThreeLevels;
            // parameters for tanh smoothing near bounding box front:
            // -- this must match the formula in getInitialConditions.bC
                        const int & side = dbase.get<int>("boundingBoxDecaySide");
                        const int & axis = dbase.get<int>("boundingBoxDecayAxis");
                        real beta=boundingBoxDecayExponent/twoPi;
                        real nv[3]={0.,0.,0.};  // normal to decay direction
                        nv[axis]=2*side-1;
            // Damp near the point xv0[] on the front
                        real xv0[3]={0.,0.,0.};  // normal to decay direction
                        for( int dir=0; dir<numberOfDimensions; dir++ )
                            xv0[dir] = .5*(initialConditionBoundingBox(1,dir)+initialConditionBoundingBox(0,dir));
                        xv0[axis]=initialConditionBoundingBox(side,axis);
                        rpar[29]=beta;
                        rpar[30]=nv[0];
                        rpar[31]=nv[1];
                        rpar[32]=nv[2];
                        rpar[33]=xv0[0];
                        rpar[34]=xv0[1];
                        rpar[35]=xv0[2];
                        adjustForIncident( mg.numberOfDimensions(),  
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		*gid.getDataPointer(),
                            		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                            		*initialConditionBoundingBox.getDataPointer(),
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                        ipar[25]=0;
                        if( debug & 4 )
                        {
                            ::display(um(all,all,all,hz),sPrintF("um (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(uOld(all,all,all,hz),sPrintF("u  (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            ::display(u (all,all,all,hz),sPrintF("un (Hz) after adjustForIncident grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                        }
                    }
          // Non-reflecting and Absorbing boundary conditions
          // ***NOTE*** symmetry corners and edges are assigned in this next routine *fix me*
                    abcMaxwell( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      ff.getBase(0),ff.getBound(0),
                            	      ff.getBase(1),ff.getBound(1),
                            	      ff.getBase(2),ff.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uOldptr, *uptr, *fptr,*maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // ** we should probably assign the PML before all the other BC's since it is like an interior equation **
          //   ** but watch out for the adjustment for the incident field ***
                    if( usePML )
                    {
                            assert( cgp!=NULL );
                            CompositeGrid & cg= *cgp;
                            realMappedGridFunction & un = u;    // u[next];
              // realMappedGridFunction & uu = uOld; // u[current];
                            const int prev= (current-1+numberOfTimeLevels) % numberOfTimeLevels;
                            const int next = (current+1) % numberOfTimeLevels;
              // realMappedGridFunction & um =mgp!=NULL ? fields[prev] : cgfields[prev][grid];
                            Range all;
              // ::display(um(all,all,all,hz),"um before pml BC's","%9.2e ");
              // ::display(u(all,all,all,hz) ,"u  before pml BC's","%9.2e ");
              // ::display(un(all,all,all,hz),"un before pml BC's","%9.2e ");
              // *********** In parallel we need to allocate local arrays **********
              //   *** We then need to define a ghost boundary update for these serial arrays ***
              // We should do this:  PML(n,side,axis,grid) -> time level n : vwpml(I1,I2,I3,0:1) <- store v,w in this array 
              // current way: 
              // PML(n,m,side,axis,grid)      n=time-level, m=v,w 
                            const int numberOfPMLFunctions=2;  //  v and w
                            const int numberOfComponentsPML=3; // store Ex, Ey, Hz or Ex,Ey,Ez
                            #define PML(n,m,side,axis,grid) vpml[(n)+numberOfTimeLevels*(m+numberOfPMLFunctions*(side+2*(axis+3*(grid))))]
                            #define VPML(n,side,axis,grid) PML(n,0,side,axis,grid)
                            #define WPML(n,side,axis,grid) PML(n,1,side,axis,grid)
                            if( vpml==NULL )
                            {
                // *** No need to allocate PML arrays for all grids !! ***
                                vpml= new RealArray [cg.numberOfComponentGrids()*3*2*numberOfTimeLevels*numberOfPMLFunctions];
                // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                                int *& pmlWasIntitialized =  dbase.put<int*>("pmlWasInitialized");
                                pmlWasIntitialized= new int[cg.numberOfComponentGrids()];   // who will delete this ?
                                for( int g=0; g<cg.numberOfComponentGrids(); g++ )
                                    pmlWasIntitialized[g]=false;
                            }
              // pmlWasIntitialized[grid] = true if the PML arrays were allocated for this grid
                            int *& pmlWasIntitialized =dbase.get<int*>("pmlWasInitialized");
                            if( !pmlWasIntitialized[grid] )
                            {
                                pmlWasIntitialized[grid]=true;
                                printF(" ****** assignBC: allocate vpml arrays grid=%i, numberOfTimeLevels=%i numberOfPMLFunctions=%i ***** \n",
                               	 grid,numberOfTimeLevels,numberOfPMLFunctions);
                                const int numGhost = orderOfAccuracyInSpace/2;  // we need ghost values in the PML functions *wdh* 2011/12/02
                                for( int side=0; side<=1; side++ )
                                {
                                    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
                                    {
                                        if( mg.boundaryCondition(side,axis)==abcPML )
                                        {
                                	  for( int m=0; m<numberOfPMLFunctions; m++ )  // ********* FIX ********
                                  	    for( int n=0; n<numberOfTimeLevels; n++ )
                                  	    {
                                    	      RealArray & vw = PML(n,m,side,axis,grid);
                                    	      int ndr[2][3];
                                    	      for( int dir=0; dir<3; dir++ )
                                    	      {
                                    		ndr[0][dir]=mg.dimension(0,dir);
                                    		ndr[1][dir]=mg.dimension(1,dir);
                                    	      }
                                    	      if( side==0 )
                                    	      {
                                    		ndr[0][axis]=mg.dimension(side,axis);
                                    		ndr[1][axis]=mg.gridIndexRange(side,axis)+numberLinesForPML-1 +numGhost;
                                    	      }
                                    	      else
                                    	      {
                                    		ndr[0][axis]=mg.gridIndexRange(side,axis)-numberLinesForPML+1 -numGhost;
                                    		ndr[1][axis]=mg.dimension(side,axis);
                                    	      }
            	      // RealArray a;
            	      // a.redim(Range(-2,10),Range(0,0));
                                    	      vw .redim(Range(ndr[0][0],ndr[1][0]),
                                          			Range(ndr[0][1],ndr[1][1]),
                                          			Range(ndr[0][2],ndr[1][2]),numberOfComponentsPML);  // ********* FIX ********
                                    	      vw=0.;
                                  	    }
                              	}
                                    }
                                }
                            } // end if pmlWasInitialized
                          #ifdef USE_PPP
                            realSerialArray uum; getLocalArrayWithGhostBoundaries(um,uum);
                            realSerialArray uu;  getLocalArrayWithGhostBoundaries(uOld,uu);
                            realSerialArray uun; getLocalArrayWithGhostBoundaries(un,uun);
            //   realSerialArray vram; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,0,grid),vram); 
            //   realSerialArray vrbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,0,grid),vrbm); 
            //   realSerialArray vsam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,1,grid),vsam); 
            //   realSerialArray vsbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,1,grid),vsbm); 
            //   realSerialArray vtam; getLocalArrayWithGhostBoundaries(VPML(prev   ,0,2,grid),vtam); 
            //   realSerialArray vtbm; getLocalArrayWithGhostBoundaries(VPML(prev   ,1,2,grid),vtbm); 
            //   realSerialArray vra ; getLocalArrayWithGhostBoundaries(VPML(current,0,0,grid),vra ); 
            //   realSerialArray vrb ; getLocalArrayWithGhostBoundaries(VPML(current,1,0,grid),vrb ); 
            //   realSerialArray vsa ; getLocalArrayWithGhostBoundaries(VPML(current,0,1,grid),vsa ); 
            //   realSerialArray vsb ; getLocalArrayWithGhostBoundaries(VPML(current,1,1,grid),vsb ); 
            //   realSerialArray vta ; getLocalArrayWithGhostBoundaries(VPML(current,0,2,grid),vta ); 
            //   realSerialArray vtb ; getLocalArrayWithGhostBoundaries(VPML(current,1,2,grid),vtb ); 
            //   realSerialArray vran; getLocalArrayWithGhostBoundaries(VPML(next   ,0,0,grid),vran); 
            //   realSerialArray vrbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,0,grid),vrbn); 
            //   realSerialArray vsan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,1,grid),vsan); 
            //   realSerialArray vsbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,1,grid),vsbn); 
            //   realSerialArray vtan; getLocalArrayWithGhostBoundaries(VPML(next   ,0,2,grid),vtan); 
            //   realSerialArray vtbn; getLocalArrayWithGhostBoundaries(VPML(next   ,1,2,grid),vtbn); 
            //   realSerialArray wram; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,0,grid),wram); 
            //   realSerialArray wrbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,0,grid),wrbm); 
            //   realSerialArray wsam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,1,grid),wsam); 
            //   realSerialArray wsbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,1,grid),wsbm); 
            //   realSerialArray wtam; getLocalArrayWithGhostBoundaries(WPML(prev   ,0,2,grid),wtam); 
            //   realSerialArray wtbm; getLocalArrayWithGhostBoundaries(WPML(prev   ,1,2,grid),wtbm); 
            //   realSerialArray wra ; getLocalArrayWithGhostBoundaries(WPML(current,0,0,grid),wra ); 
            //   realSerialArray wrb ; getLocalArrayWithGhostBoundaries(WPML(current,1,0,grid),wrb ); 
            //   realSerialArray wsa ; getLocalArrayWithGhostBoundaries(WPML(current,0,1,grid),wsa ); 
            //   realSerialArray wsb ; getLocalArrayWithGhostBoundaries(WPML(current,1,1,grid),wsb ); 
            //   realSerialArray wta ; getLocalArrayWithGhostBoundaries(WPML(current,0,2,grid),wta ); 
            //   realSerialArray wtb ; getLocalArrayWithGhostBoundaries(WPML(current,1,2,grid),wtb ); 
            //   realSerialArray wran; getLocalArrayWithGhostBoundaries(WPML(next   ,0,0,grid),wran); 
            //   realSerialArray wrbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,0,grid),wrbn); 
            //   realSerialArray wsan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,1,grid),wsan); 
            //   realSerialArray wsbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,1,grid),wsbn); 
            //   realSerialArray wtan; getLocalArrayWithGhostBoundaries(WPML(next   ,0,2,grid),wtan); 
            //   realSerialArray wtbn; getLocalArrayWithGhostBoundaries(WPML(next   ,1,2,grid),wtbn); 
                          #else
                            const realSerialArray & uum = um;
                            const realSerialArray & uu  = uOld;
                            const realSerialArray & uun = un;
                          #endif
                            const realSerialArray & vram = VPML(prev   ,0,0,grid); 
                            const realSerialArray & vrbm = VPML(prev   ,1,0,grid); 
                            const realSerialArray & vsam = VPML(prev   ,0,1,grid); 
                            const realSerialArray & vsbm = VPML(prev   ,1,1,grid); 
                            const realSerialArray & vtam = VPML(prev   ,0,2,grid); 
                            const realSerialArray & vtbm = VPML(prev   ,1,2,grid); 
                            const realSerialArray & vra  = VPML(current,0,0,grid); 
                            const realSerialArray & vrb  = VPML(current,1,0,grid); 
                            const realSerialArray & vsa  = VPML(current,0,1,grid); 
                            const realSerialArray & vsb  = VPML(current,1,1,grid); 
                            const realSerialArray & vta  = VPML(current,0,2,grid); 
                            const realSerialArray & vtb  = VPML(current,1,2,grid); 
                            const realSerialArray & vran = VPML(next   ,0,0,grid); 
                            const realSerialArray & vrbn = VPML(next   ,1,0,grid); 
                            const realSerialArray & vsan = VPML(next   ,0,1,grid); 
                            const realSerialArray & vsbn = VPML(next   ,1,1,grid); 
                            const realSerialArray & vtan = VPML(next   ,0,2,grid); 
                            const realSerialArray & vtbn = VPML(next   ,1,2,grid); 
                            const realSerialArray & wram = WPML(prev   ,0,0,grid); 
                            const realSerialArray & wrbm = WPML(prev   ,1,0,grid); 
                            const realSerialArray & wsam = WPML(prev   ,0,1,grid); 
                            const realSerialArray & wsbm = WPML(prev   ,1,1,grid); 
                            const realSerialArray & wtam = WPML(prev   ,0,2,grid); 
                            const realSerialArray & wtbm = WPML(prev   ,1,2,grid); 
                            const realSerialArray & wra  = WPML(current,0,0,grid); 
                            const realSerialArray & wrb  = WPML(current,1,0,grid); 
                            const realSerialArray & wsa  = WPML(current,0,1,grid); 
                            const realSerialArray & wsb  = WPML(current,1,1,grid); 
                            const realSerialArray & wta  = WPML(current,0,2,grid); 
                            const realSerialArray & wtb  = WPML(current,1,2,grid); 
                            const realSerialArray & wran = WPML(next   ,0,0,grid); 
                            const realSerialArray & wrbn = WPML(next   ,1,0,grid); 
                            const realSerialArray & wsan = WPML(next   ,0,1,grid); 
                            const realSerialArray & wsbn = WPML(next   ,1,1,grid); 
                            const realSerialArray & wtan = WPML(next   ,0,2,grid); 
                            const realSerialArray & wtbn = WPML(next   ,1,2,grid); 
                            real *umptr, *uuptr, *unptr;   
                            umptr=uum.getDataPointer();
                            uuptr= uu.getDataPointer();  
                            unptr=uun.getDataPointer();
                            if( debug & 4 )
                            {
                                ::display(um(all,all,all,hz),sPrintF("um (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(uu(all,all,all,hz),sPrintF("u  (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(un(all,all,all,hz),sPrintF("un (Hz) before pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            }
              // Here is the box outside of which the PML equations are applied.
                            getBoundsForPML(mg,Iv);
                            int includeGhost=0;
                            bool ok = ParallelUtility::getLocalArrayBounds(uOld,uu,I1,I2,I3,includeGhost);
                            if( ok )
                            {
                                ipar[2] =I1.getBase();
                                ipar[3] =I1.getBound();
                                ipar[4] =I2.getBase();
                                ipar[5] =I2.getBound();
                                ipar[6] =I3.getBase();
                                ipar[7] =I3.getBound();
                                assert( dx[0]>0. );
                                int bc0=-1;  // not used
                // ** for( int m=0; m<3; m++ )
                                for( int m=0; m<3; m++ )
                                {
                              	ipar[12]=ex+m; // assign this component
                              	pmlMaxwell( mg.numberOfDimensions(), 
                                        		    uu.getBase(0),uu.getBound(0),
                                        		    uu.getBase(1),uu.getBound(1),
                                        		    uu.getBase(2),uu.getBound(2),
                                        		    ff.getBase(0),ff.getBound(0),
                                        		    ff.getBase(1),ff.getBound(1),
                                        		    ff.getBase(2),ff.getBound(2),
                                        		    *gid.getDataPointer(),
                                        		    *dim.getDataPointer(),
                                        		    *umptr, *uuptr, *unptr, 
            		    // vra (left)
                                        		    vra.getBase(0),vra.getBound(0),vra.getBase(1),vra.getBound(1),vra.getBase(2),vra.getBound(2),
                                        		    *vram.getDataPointer(),*vra.getDataPointer(),*vran.getDataPointer(),
                                        		    *wram.getDataPointer(),*wra.getDataPointer(),*wran.getDataPointer(),
            		    // vrb (right)
                                        		    vrb.getBase(0),vrb.getBound(0),vrb.getBase(1),vrb.getBound(1),vrb.getBase(2),vrb.getBound(2),
                                        		    *vrbm.getDataPointer(),*vrb.getDataPointer(),*vrbn.getDataPointer(),
                                        		    *wrbm.getDataPointer(),*wrb.getDataPointer(),*wrbn.getDataPointer(),
            		    // vsa (bottom)
                                        		    vsa.getBase(0),vsa.getBound(0),vsa.getBase(1),vsa.getBound(1),vsa.getBase(2),vsa.getBound(2),
                                        		    *vsam.getDataPointer(),*vsa.getDataPointer(),*vsan.getDataPointer(),
                                        		    *wsam.getDataPointer(),*wsa.getDataPointer(),*wsan.getDataPointer(),
            		    // vsb 
                                        		    vsb.getBase(0),vsb.getBound(0),vsb.getBase(1),vsb.getBound(1),vsb.getBase(2),vsb.getBound(2),
                                        		    *vsbm.getDataPointer(),*vsb.getDataPointer(),*vsbn.getDataPointer(),
                                        		    *wsbm.getDataPointer(),*wsb.getDataPointer(),*wsbn.getDataPointer(),
            		    // vta
                                        		    vta.getBase(0),vta.getBound(0),vta.getBase(1),vta.getBound(1),vta.getBase(2),vta.getBound(2),
                                        		    *vtam.getDataPointer(),*vta.getDataPointer(),*vtan.getDataPointer(),
                                        		    *wtam.getDataPointer(),*wta.getDataPointer(),*wtan.getDataPointer(),
            		    // vtb 
                                        		    vtb.getBase(0),vtb.getBound(0),vtb.getBase(1),vtb.getBound(1),vtb.getBase(2),vtb.getBound(2),
                                        		    *vtbm.getDataPointer(),*vtb.getDataPointer(),*vtbn.getDataPointer(),
                                        		    *wtbm.getDataPointer(),*wtb.getDataPointer(),*wtbn.getDataPointer(),
                                        		    *fptr,*maskptr,*rxptr, *xyptr,
                                        		    bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                                }  // end m
                                ipar[12]=ex;
                            }
                            if( debug & 4 )
                            {
                                ::display(um(all,all,all,hz),sPrintF("um (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(uu(all,all,all,hz),sPrintF("u  (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                                ::display(un(all,all,all,hz),sPrintF("un (Hz) after pml BC's grid=%i t=%e",grid,t),debugFile,"%9.2e ");
                            }
                    }
          // *wdh* 090509 -- symmetry CORNERS BC's (like a straight PEC wall)
                    bcOption=1; // 1=assign corners and edges only
                    ipar[26]=bcOption; 
                    bcSymmetry( mg.numberOfDimensions(), 
                            	      uu.getBase(0),uu.getBound(0),
                            	      uu.getBase(1),uu.getBound(1),
                            	      uu.getBase(2),uu.getBound(2),
                            	      *gid.getDataPointer(),
                            	      *uptr, *maskptr,*rxptr, *xyptr,
                            	      bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
          // ::display(u,"u after pml BC's","%9.2e ");
          // assign any radiation BC's
                    for( int i=0; i<2; i++ )
                    {
            // ** FIX ME for SOSUP ***
                        if( radbcGrid[i]==grid )
                        {
                            RadiationBoundaryCondition::debug=debug;
                            radiationBoundaryCondition[i].tz=tz; // fix this 
                            radiationBoundaryCondition[i].assignBoundaryConditions( u,t,dt,uOld );
                        }
                    }
                    if( adjustFarFieldBoundariesForIncidentField(grid) )
                    {
                        ipar[25]=+1;  // add back the incident field
                        ipar[26]=numberLinesForPML;
                        ipar[27]=adjustThreeLevels;
                        adjustForIncident( mg.numberOfDimensions(),  
                            		uu.getBase(0),uu.getBound(0),
                            		uu.getBase(1),uu.getBound(1),
                            		uu.getBase(2),uu.getBound(2),
                            		*gid.getDataPointer(),
                            		*uum.getDataPointer(), *uOldptr, *uptr, *maskptr,*rxptr, *xyptr,
                                                *initialConditionBoundingBox.getDataPointer(),
                            		bc0, *bc.getDataPointer(), ipar[0], rpar[0], ierr );
                        ipar[25]=0;
                    }
                } // end ok 
            }
            
            if( debugGhost && grid==1 )
        	  fprintf(debugFile,"\n --DBG--- AFter optBC: u[1](-1,-1,0,ey)=%8.2e\n",uLocal(-1,-1,0,ey));
            
        }
    }
    else
    {
    // unstructured grid BC's
    }


    if( addedExtraGhostLine )
    {
    // Extrapolate an extra ghost line for the wider upwind stencil in SOSUP
        BoundaryConditionParameters extrapParams;

        int bcOrderOfAccuracy=orderOfAccuracyInSpace;
        if( method==sosup && orderOfAccuracyInSpace==6 )
        {
      // NOTE: for now apply 4th order BC's for sosup order 6
            bcOrderOfAccuracy=4;
        }

        const int ghostEnd = (orderOfAccuracyInSpace/2)+1;  // last ghost line for sosup stencil
    // NOTE: for now we impose at most 2 ghost lines with the 4th=order BC's 
    // first ghost line for sosup stencil: 
    //     ghostStart=2 for order=2
    //     ghostStart=3 for order>2   *fix me* when 6'th order BC's are implemented
        assert( bcOrderOfAccuracy<=4 );
        const int ghostStart= min(3,ghostEnd);              

        extrapParams.orderOfExtrapolation=orderOfAccuracyInSpace+1;  // what should this be ?

        extrapParams.extraInTangentialDirections=ghostEnd;

        Range Ca = cgfields[0][0].getLength(3); // all components

        MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];
        u.setOperators(mgop);
        
        for( int ghost=ghostStart; ghost<=ghostEnd; ghost++ )
        {
            extrapParams.ghostLineToAssign=ghost;
            if( debug & 4 )
      	printF("assignBC: sosup: extrap ghost-line %i to order %i\n",
             	       extrapParams.ghostLineToAssign,extrapParams.orderOfExtrapolation);

            for( int axis=0; axis<mg.numberOfDimensions(); axis++ )for( int side=0; side<=1; side++ )
            {
      	const int bc = mg.boundaryCondition(side,axis);
      	if( bc!=dirichlet && bc!=symmetry )
      	{
        	  u.applyBoundaryCondition(Ca,BCTypes::extrapolate,BCTypes::boundary1+side+2*(axis),0.,t,extrapParams);
      	}
            }
        }
        
    }

    

    if( debug & 8 )
    {
        Index I1,I2,I3;
        getIndex(mg.dimension(),I1,I2,I3);      
        ::display(u(I1,I2,I3,ey),sPrintF("BC: Ey after optBC, grid=%i t=%e",grid,t),debugFile,"%8.1e ");
    }

    if( debug & 4 )
    {
        ::display(u,sPrintF("u at end of assignBC, grid=%i t=%e",grid,t),debugFile,"%8.1e ");
    }

    timing(timeForBoundaryConditions)+=getCPU()-time0;
}

