// This file automatically generated from getInitialConditions.bC with bpp.
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

#define scatCyl EXTERN_C_NAME(scatcyl)
#define scatSphere EXTERN_C_NAME(scatsphere)
#define exmax EXTERN_C_NAME(exmax)

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

// =============================================================
//  Assign initial conditions for the Gaussian pulse
//
// GRIDTYPE: curvilinear or rectangular
// ==============================================================

// Polynomial TZ functions

// ===============================================================================================
//  Macro: initialize3DPolyTW is a bpp macro that sets up the E and H polynomial tw function
// ===============================================================================================
    

// ===============================================================================================
// *** Macro: This macro defines the polynomial TZ functions 
// ===============================================================================================


// Trigonometric TZ functions
// ***********************************************************
// *** This macro defines the trigonometric TZ functions *****
// ***********************************************************

// Macros for the plane material interface:

 // -- incident wave ---
 //  --- time derivative of incident ---
 // -- transmitted wave ---
 //  --- time derivative of transmitted wave ---


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

//==================================================================================================
//==================================================================================================
int 
Maxwell::
getCenters( MappedGrid &mg, UnstructuredMapping::EntityTypeEnum cent, realArray &xe )
{
    if ( cent==UnstructuredMapping::Vertex ) 
        {
            xe.redim(0);
            xe = mg.vertex();
            return 0;
        }

    Mapping & map = mg.mapping().getMapping();
    assert( map.getClassName()=="UnstructuredMapping" );
    
    UnstructuredMapping & uns= (UnstructuredMapping &)map;
    
    UnstructuredMappingIterator citer, citer_end;
    UnstructuredMappingAdjacencyIterator viter, viter_end;
    UnstructuredMappingAdjacencyIterator fiter, fiter_end;
    int nNodes;

    xe.redim(uns.size(cent),mg.numberOfDimensions());
    xe = 0.;
    
    const realArray & verts = mg.vertex();

    citer_end = uns.end(cent);
    for ( citer=uns.begin(cent); citer!=citer_end; citer++ )
    {
        int e=*citer;
        if ( cent==UnstructuredMapping::Face || mg.numberOfDimensions()==2 || false)
        {
            nNodes = 0;
            viter_end = uns.adjacency_end(citer, UnstructuredMapping::Vertex);
            for ( viter=uns.adjacency_begin(citer, UnstructuredMapping::Vertex);
          	    viter!=viter_end;
          	    viter++ )
            {
      	int v=*viter;
      	for ( int a=0; a<mg.numberOfDimensions(); a++ )
        	  xe(e,a) += verts(v,0,0,a);
      	nNodes++;
            }
        	  
            for ( int a=0; a<mg.numberOfDimensions(); a++ )
      	xe(e,a)/=real(nNodes);
        }
        else
        {
            ArraySimpleFixed<real,3,1,1,1> xfc;
            int nFaces=0;
            fiter_end = uns.adjacency_end(citer, UnstructuredMapping::Face);
            for ( fiter=uns.adjacency_begin(citer, UnstructuredMapping::Face);
          	    fiter!=fiter_end;
          	    fiter++ )
            {
      	nNodes = 0;
      	viter_end = uns.adjacency_end(citer, UnstructuredMapping::Vertex);
      	xfc=0;
      	for ( viter=uns.adjacency_begin(citer, UnstructuredMapping::Vertex);
            	      viter!=viter_end;
            	      viter++ )
      	{
        	  int v=*viter;
        	  for ( int a=0; a<mg.numberOfDimensions(); a++ )
          	    xfc[a] += verts(v,0,0,a);
        	  nNodes++;
      	}
      	for ( int a=0; a<mg.numberOfDimensions(); a++ )
        	  xe(e,a) += xfc[a]/real(nNodes);
      	nFaces++;
            }

            for ( int a=0; a<mg.numberOfDimensions(); a++ )
      	xe(e,a) /= real(nFaces);

        }
    }
    
    return 0;
}

static int
getFaceCenters( MappedGrid & mg, realArray & xe )
{
    Mapping & map = mg.mapping().getMapping();
    assert( map.getClassName()=="UnstructuredMapping" );
    
    UnstructuredMapping & uns= (UnstructuredMapping &)map;
    
    int numberOfFaces=uns.size(UnstructuredMapping::Edge);//getNumberOfFaces();
    xe.redim(numberOfFaces,mg.numberOfDimensions());
    const realArray & x= uns.getNodes();
    const intArray & faces = uns.getFaces();
    
    for( int f=0; f<numberOfFaces; f++ )
    {
        int n0=faces(f,0), n1=faces(f,1);
        xe(f,0)=.5*( x(n0,0)+x(n1,0));
        xe(f,1)=.5*( x(n0,1)+x(n1,1));
    }
    return 0;
}




//! Return the true solution for the electric field
void Maxwell::
getField( real x, real y, real t, real *eField )
{
    const real cc= c*sqrt( kx*kx+ky*ky+kz*kz );
    eField[0]=exTrue(x,y,t);
    eField[1]=eyTrue(x,y,t);
    
}

//==========================================================================================
/// \brief Initialize the constants that define the plane material interface solution.
//==========================================================================================
int Maxwell::
initializePlaneMaterialInterface()
{
    if( initialConditionOption!=planeMaterialInterfaceInitialCondition )
    {
        return 0;
    }
    
    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    const int numberOfDimensions=cg.numberOfDimensions();

  // ------------------------------------------------------------
  // Here we compute the coefficients in the exact solution
  // 
  // E(x,y) = (a1,a2,a3)*cos( k.(x-x0)-wt) + r*(b1,b2,b3)*cos( kr.(x-x0) - w t )
  //        =                              tau*(d1,d2,d3)*cos( kappa.(x-x0) - w t )
  //
  // H(x,y) = (e1,e2,e3)*cos( k.(x-x0)-wt) + r*(f1,f2,f3)*cos( kr.(x-x0) - w t )
  //        =                              tau*(g1,g2,g3)*cos( kappa.(x-x0) - w t )
  //
  // ------------------------------------------------------------
    if( numberOfDimensions==2 && kz!=0 )
    {
        printF("Maxwell::initializePlaneMaterialInterface:ERROR: plane material interface: kz!=0 in 2D!\n");
        OV_ABORT("error");
    }
            
    const int gridLeft = 0;
    const int gridRight=cg.numberOfComponentGrids()-1;

    real c1,c2,eps1,eps2,mu1,mu2;
    if( method==yee )
    {
        eps1=epsv(gridLeft);  mu1=muv(gridLeft);    // incident 
        eps2=epsv(gridRight); mu2=muv(gridRight);   // transmitted
    }
    else
    {
        eps1=epsGrid(gridLeft);  mu1=muGrid(gridLeft); // incident
        eps2=epsGrid(gridRight); mu2=muGrid(gridRight); // transmitted

    }
    c1=1./sqrt(eps1*mu1);  // incident 
    c2=1./sqrt(eps2*mu2);  // transmitted
    
    real eta1=sqrt(mu1/eps1), eta2=sqrt(mu2/eps2);  // wave impedance 

    const real cr =c2/c1;                   // relative index of refraction

    real nv[3];
    for( int axis=0; axis<3; axis++ )
        nv[axis]=normalPlaneMaterialInterface[axis];

    const real kv[3]={kx,ky,kz};
    const real kNorm = sqrt(kx*kx+ky*ky+kz*kz);
    assert( kNorm>0. );
    const real kDotN = kx*nv[0]+ky*nv[1]+kz*nv[2];
            
  // kr : reflected wave number:
  //   kr.nv = - k.nv 
    real kr[3]={kx,ky,kz};
    for( int axis=0; axis<3; axis++ )
        kr[axis] = kr[axis] - 2.*kDotN*nv[axis];

    const real krNorm=sqrt( SQR(kr[0])+SQR(kr[1])+SQR(kr[2]) );
    assert( krNorm>0. );

    printF("PMI:3d: kv=(%8.2e,%8.2e,%8.2e) nv=(%8.2e,%8.2e,%8.2e)\n",kv[0],kv[1],kv[2],nv[0],nv[1],nv[2]);

  // kappa: transmitted wave number
  //   kappa.t = k.t 
    real kappa[3]={kx,ky,kz};     
    real kappatSq = kNorm*kNorm - kDotN*kDotN;        // tangential component of kappa = tang. comp of k (sign doesn't matter)
    real arg = (kNorm*kNorm)/(cr*cr) - kappatSq;
    printF("PMI:3d: cr=%8.2e kNorm=%8.2e kDotN=%8.2e kappatSq=%8.2e arg=%8.2e\n",cr,kNorm,kDotN,kappatSq,arg);
    
    if( arg<0. )
    {
        printF("ERROR: computing the plane material interface solution: angle of incident is too close to 90 degrees\n");
        printF("       This case is not supported.\n");
        OV_ABORT("error");
    }
    real kappan = sqrt( arg );   // normal comp. of kappa jumps 
    for( int axis=0; axis<3; axis++ )
        kappa[axis] = kappa[axis] + (kappan- kDotN)*nv[axis];  // subtract off k.n and add on kappa.n
    printF(" (kx,ky,kz)=(%8.2e,%8.2e,%8.2e) kr=(%8.2e,%8.2e,%8.2e), kappa=(%8.2e,%8.2e,%8.2e), nv=\(%8.2e,%8.2e,%8.2e)\n",
       	 kx,ky,kz,kr[0],kr[1],kr[2],kappa[0],kappa[1],kappa[2],nv[0],nv[1],nv[2]);
    printF("kappatSq=%e, kappan=%e, kDotN=%e, arg=%e\n",kappatSq,kappan,kDotN,arg);
            
    const real kappaNorm=sqrt( SQR(kappa[0])+SQR(kappa[1])+SQR(kappa[2]) );
    assert( kappaNorm>0. );
    const real cosTheta1=kDotN/kNorm;
    const real cosTheta2=kappan/kappaNorm;


  // E: (amplitude of incident=1)
    real av[3]={-ky/kNorm, kx/kNorm,0.};         // incident : we have different choices here in 3d 
    real bv[3]={-kr[1]/krNorm, kr[0]/krNorm,0.}; // reflected : this depends on av
    real dv[3]={-kappa[1]/kappaNorm, kappa[0]/kappaNorm,0.}; // transmitted : this depends on av

  // new way: use plane wave solution as the incident:
    for( int axis=0; axis<3; axis++ )
        av[axis]=pwc[axis];

    real aNorm = sqrt( SQR(av[0])+SQR(av[1])+SQR(av[2]) );
    if( numberOfDimensions==2 )
    { 
    // reflection and transmission coefficients
        const real r = (c1*cosTheta1-c2*cosTheta2)/(c1*cosTheta1+c2*cosTheta2);
        const real tau = (2.*c2*cosTheta1)/(c1*cosTheta1+c2*cosTheta2);

        printF("PMI: reflection-coeff=%8.2e, transmission-coeff=%8.2e\n",r,tau);

        bv[0]=-aNorm*r*kr[1]/krNorm; bv[1]=aNorm*r*kr[0]/krNorm; bv[2]=0;
        dv[0]=-aNorm*tau*kappa[1]/kappaNorm; dv[1]=aNorm*tau*kappa[0]/kappaNorm; dv[2]=0.;
    }
    else
    {
    // In 3d we decompose the incident field into components parallel and perpendicular to the plane of incidence 
    // These two components have different reflection and transmission coeff's etc.
        
        real qv[3],gv[3],hv[3],mv[3];
        
    // The plane of incident is defined by nv and kv
    //   qv: normal to the plane of incidence
    // qv = nv X kv 
        qv[0] = nv[1]*kv[2]-nv[2]*kv[1];
        qv[1] = nv[2]*kv[0]-nv[0]*kv[2];
        qv[2] = nv[0]*kv[1]-nv[1]*kv[0];
        real qNorm = sqrt( SQR(qv[0])+SQR(qv[1])+SQR(qv[2]) );
        if( qNorm < REAL_MIN*100. )
        {
      // nv is parallel to kv (normal incidence) -- just choose qv = nv X av 
            qv[0] = nv[1]*av[2]-nv[2]*av[1];
            qv[1] = nv[2]*av[0]-nv[0]*av[2];
            qv[2] = nv[0]*av[1]-nv[1]*av[0];
            qNorm = sqrt( SQR(qv[0])+SQR(qv[1])+SQR(qv[2]) );
            assert( qNorm > REAL_MIN*100. );
        }
        for( int axis=0; axis<3; axis++ )
            qv[axis]/=qNorm;   // normalize qv 
    // sanity check: 
        real qDotN = qv[0]*nv[0]+qv[1]*nv[1]+qv[2]*nv[2];
        real qDotK = qv[0]*kv[0]+qv[1]*kv[1]+qv[2]*kv[2];
        assert( fabs(qDotN)< 10.*REAL_EPSILON && fabs(qDotK)< 10.*REAL_EPSILON );
            
    // gv= qv X kv is in the plane of incidence and normal to kv 
        gv[0] = qv[1]*kv[2]-qv[2]*kv[1];
        gv[1] = qv[2]*kv[0]-qv[0]*kv[2];
        gv[2] = qv[0]*kv[1]-qv[1]*kv[0];    
        real gNorm = sqrt( SQR(gv[0])+SQR(gv[1])+SQR(gv[2]) );
        assert( gNorm>REAL_MIN*100. );
        for( int axis=0; axis<3; axis++ )
            gv[axis]/=gNorm;   // normalize gv 

    // Decompose the incident field:  (av.kv =0 since div(E)=0 )
    //   av = aDotq*qv + aDotg*gv 
        real aDotQ = av[0]*qv[0]+av[1]*qv[1]+av[2]*qv[2];
        real aDotG = av[0]*gv[0]+av[1]*gv[1]+av[2]*gv[2];

        printF("PMI:3d: kv=(%8.2e,%8.2e,%8.2e) av=(%8.2e,%8.2e,%8.2e) nv=(%8.2e,%8.2e,%8.2e)\n",kv[0],kv[1],kv[2],
                          av[0],av[1],av[2],nv[0],nv[1],nv[2]);
        printF("PMI:3d: qv=(%8.2e,%8.2e,%8.2e) aDotQ=%8.2e\n",qv[0],qv[1],qv[2],aDotQ);
        printF("PMI:3d: gv=(%8.2e,%8.2e,%8.2e) aDotG=%8.2e\n",gv[0],gv[1],gv[2],aDotG);
        

        const real rParallel   = (c1*cosTheta1-c2*cosTheta2)/(c1*cosTheta1+c2*cosTheta2);
        const real tauParallel = (2.*c2*cosTheta1          )/(c1*cosTheta1+c2*cosTheta2);
        
        const real rPerp   = (c2*cosTheta1-c1*cosTheta2)/(c2*cosTheta1+c1*cosTheta2);
        const real tauPerp = (2.*c2*cosTheta1          )/(c2*cosTheta1+c1*cosTheta2);

        printF("PMI:3d:  reflection-coeff=(%8.2e,%8.2e), transmission-coeff=(%8.2e,%8.2e) [(parallel,perp)]\n",rParallel,rPerp,tauParallel,tauPerp);

    // reflected: 
    // hv = qv X kr 
        hv[0] = (qv[1]*kr[2]-qv[2]*kr[1]);
        hv[1] = (qv[2]*kr[0]-qv[0]*kr[2]);
        hv[2] = (qv[0]*kr[1]-qv[1]*kr[0]);   
        real hNorm = sqrt( SQR(hv[0])+SQR(hv[1])+SQR(hv[2]) );
        assert( hNorm>REAL_MIN*100. );
        for( int axis=0; axis<3; axis++ )
            hv[axis]/=hNorm;   // normalize hv 

        for( int axis=0; axis<3; axis++ )
            bv[axis]= rPerp*aDotQ*qv[axis] + rParallel*aDotG*hv[axis];


    // transmitted:
    // mv = qv X kappa
        mv[0] = (qv[1]*kappa[2]-qv[2]*kappa[1]);
        mv[1] = (qv[2]*kappa[0]-qv[0]*kappa[2]);
        mv[2] = (qv[0]*kappa[1]-qv[1]*kappa[0]);   
        real mNorm = sqrt( SQR(mv[0])+SQR(mv[1])+SQR(mv[2]) );
        assert( mNorm>REAL_MIN*100. );
        for( int axis=0; axis<3; axis++ )
            mv[axis]/=mNorm;   // normalize mv 
        for( int axis=0; axis<3; axis++ )
            dv[axis]= tauPerp*aDotQ*qv[axis] + tauParallel*aDotG*mv[axis];


    }
    
    printF("PMI: bv=(%8.2e,%8.2e,%8.2e)\n",bv[0],bv[1],bv[2]);
    printF("PMI: dv=(%8.2e,%8.2e,%8.2e)\n",dv[0],dv[1],dv[2]);

  // H: (can be computed directly from E)
  //   mu*H_t = -curl( E )
  //  -mu*w*Hx = - [ D_y(Ez) - D_z(Ey) ]
  //  -mu*w*Hy = - [ D_z(Ex) - D_x(Ez) ]
  //  -mu*w*Hz = - [ D_x(Ey) - D_y(Ex) ]
    real ev[3]; // incident H: this depends on av
    real fv[3]; // reflected H
    real gv[3]; // transmitted H

    const real w = c1*kNorm;  // omega 
    const real w1=w*mu1, w2=w*mu2;
    ev[0] = (ky*av[2]-kz*av[1])/w1;
    ev[1] = (kz*av[0]-kx*av[2])/w1;
    ev[2] = (kx*av[1]-ky*av[0])/w1;
            
    fv[0] = (kr[1]*bv[2]-kr[2]*bv[1])/w1;
    fv[1] = (kr[2]*bv[0]-kr[0]*bv[2])/w1;
    fv[2] = (kr[0]*bv[1]-kr[1]*bv[0])/w1;

    gv[0] = (kappa[1]*dv[2]-kappa[2]*dv[1])/w2;
    gv[1] = (kappa[2]*dv[0]-kappa[0]*dv[2])/w2;
    gv[2] = (kappa[0]*dv[1]-kappa[1]*dv[0])/w2;

  // Now fill in the constants that define the solution (see planeMaterialInterface.h)
            
  // E : Incident+reflected:
    pmc[ 0]=av[0]; pmc[ 1]=bv[0];
    pmc[ 2]=av[1]; pmc[ 3]=bv[1];
    pmc[ 4]=av[2]; pmc[ 5]=bv[2];
  // E : Transmitted
    pmc[12]=dv[0]; 
    pmc[13]=dv[1]; 
    pmc[14]=dv[2]; 

  // H : Incident+reflected:
    pmc[ 6]=ev[0]; pmc[ 7]=fv[0];
    pmc[ 8]=ev[1]; pmc[ 9]=fv[1];
    pmc[10]=ev[2]; pmc[11]=fv[2];

  // H : Transmitted
    pmc[15]=gv[0]; 
    pmc[16]=gv[1]; 
    pmc[17]=gv[2]; 
            
    pmc[18]=w;  // omega 
    pmc[19]=kx; pmc[20]=ky; pmc[21]=kz;
    pmc[22]=kr[0]; pmc[23]=kr[1]; pmc[24]=kr[2];
    pmc[25]=kappa[0]; pmc[26]=kappa[1]; pmc[27]=kappa[2];
    pmc[28]=x0PlaneMaterialInterface[0]; pmc[29]=x0PlaneMaterialInterface[1]; pmc[30]=x0PlaneMaterialInterface[2];
    pmc[30]=normalPlaneMaterialInterface[0]; pmc[31]=normalPlaneMaterialInterface[1]; pmc[32]=normalPlaneMaterialInterface[2];

    
    return 0;
}

// ============================================================================
// Macro to compute the (x,y) coordinates - optimized for rectangular grids
// ============================================================================

//! Assign initial conditions
//   (Ex).t = (1/eps)*[  (Hz).y ]
//   (Ey).t = (1/eps)*[ -(Hz).x ]
//   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]
void Maxwell::
assignInitialConditions(int current, real t, real dt )
// ===========================================================================================
// ===========================================================================================
{

    real time0=getCPU();
    
    if( true )
        printF("**************\n"
                      " --MX-- assignInitialConditions: t=%9.3e initialConditionOption=%i\n",t,(int)initialConditionOption);

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    const int numberOfDimensions=cg.numberOfDimensions();

    const BoundaryForcingEnum & boundaryForcingOption =dbase.get<BoundaryForcingEnum>("boundaryForcingOption");

    if( forcingOption==twilightZoneForcing )
    {
        int numberOfComponents=cgfields[0][0].getLength(3); 

    // int numberOfComponents;
    // if( numberOfDimensions==2 )
    //   numberOfComponents=3; 
    // else
    //   numberOfComponents=(int(solveForElectricField)+int(solveForMagneticField))*3;

    // if( method==sosup )
    // {
    //   numberOfComponents= cgfields[0][0].getLength(3);
    // }
        
    // *wdh* 090516 : define variable coeff's for rho, eps, mu, sigmaE and sigmaH with TZ 
        int numberOfComponentsForTZ=numberOfComponents+5;
        assert( sigmaHc==(numberOfComponentsForTZ-1) );

        delete tz;
        if( twilightZoneOption==polynomialTwilightZone )
        {
            
            tz = new OGPolyFunction(degreeSpace,numberOfDimensions,numberOfComponentsForTZ,degreeTime);
            const int ndp=max(max(5,degreeSpace+1),degreeTime+1);
            printF("\n $$$$$$$ assignInitialConditions: build OGPolyFunction: degreeSpace=%i, degreeTime=%i ndp=%i $$$$\n",
                          degreeSpace,degreeTime,ndp);
            RealArray spatialCoefficientsForTZ(ndp,ndp,ndp,numberOfComponentsForTZ);  
            spatialCoefficientsForTZ=0.;
            RealArray timeCoefficientsForTZ(ndp,numberOfComponentsForTZ);      
            timeCoefficientsForTZ=0.;
      // Default coefficients for eps, mu, sigmaE and sigmaH:
            assert( epsc>=0 && muc>=0 && sigmaEc>=0 && sigmaHc>=0 );
            printF(" *** numberOfComponentsForTZ=%i, epsc,muc,sigmaEc,sigmaHc=%i,%i,%i,%i, eps,mu=%e,%e\n",numberOfComponentsForTZ,epsc,muc,sigmaEc,sigmaHc,eps,mu);
            spatialCoefficientsForTZ(0,0,0,epsc)=eps;
            spatialCoefficientsForTZ(0,0,0,muc )=mu; 
            spatialCoefficientsForTZ(0,0,0,sigmaEc)=0.;  
            spatialCoefficientsForTZ(0,0,0,sigmaHc)=0.;
            if( numberOfDimensions==2 )
            {
                if( degreeSpace==0 )
                {
                    spatialCoefficientsForTZ(0,0,0,ex)=1.;      // u=1
                    spatialCoefficientsForTZ(0,0,0,ey)= 2.;      // v=2
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.;      // w=-1
          // -- dispersion components: 
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else if( degreeSpace==1 )
                {
                    spatialCoefficientsForTZ(0,0,0,ex)=1.;      // u=1+x+y
                    spatialCoefficientsForTZ(1,0,0,ex)=1.;
                    spatialCoefficientsForTZ(0,1,0,ex)=1.;
                    spatialCoefficientsForTZ(0,0,0,ey)= 2.;      // v=2+x-y
                    spatialCoefficientsForTZ(1,0,0,ey)= 1.;
                    spatialCoefficientsForTZ(0,1,0,ey)=-1.;
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.;      // w=-1+x + y
                    spatialCoefficientsForTZ(1,0,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,1,0,hz)= 1.;
          // eps and mu should remain positive but do this for now:
                    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x*eps*.01
                    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y*eps*.02 
                    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
          // -- dispersion components: 
          // ** FINISH ME **
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else if( degreeSpace==2 )
                {
                    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 
                    spatialCoefficientsForTZ(1,1,0,ex)=2.;
                    spatialCoefficientsForTZ(0,2,0,ex)=1.;
                    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 
                    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                    spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 -1 +.5 xy
                    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
                    spatialCoefficientsForTZ(1,1,0,hz)= .5;
          // eps and mu should remain positive 
                    spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                    spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                    spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                    spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                    spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                    spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                    spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                    spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
          // -- dispersion components: 
          // ** FINISH ME **
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(1,0,0,pxc)=.1; 
                        spatialCoefficientsForTZ(0,1,0,pxc)=.1; 
                        spatialCoefficientsForTZ(2,0,0,pxc)=.1; 
                        spatialCoefficientsForTZ(0,2,0,pxc)=.1; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                        spatialCoefficientsForTZ(1,0,0,pyc)=.1; 
                        spatialCoefficientsForTZ(0,1,0,pyc)=.2; 
                        spatialCoefficientsForTZ(2,0,0,pyc)=-.1; 
                        spatialCoefficientsForTZ(0,2,0,pyc)=.1; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else if( degreeSpace==3 )
                {
                    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .5*y^3 + .25*x^2*y + .2*x^3  - .3*x*y^2
                    spatialCoefficientsForTZ(1,1,0,ex)=2.;
                    spatialCoefficientsForTZ(0,2,0,ex)=1.;
                    spatialCoefficientsForTZ(0,3,0,ex)=.5;
                    spatialCoefficientsForTZ(2,1,0,ex)=.25;
                    spatialCoefficientsForTZ(3,0,0,0,ex)=.2;
                    spatialCoefficientsForTZ(1,2,0,0,ex)=-.3;
                    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 -.5*x^3 -.25*x*y^2  -.6*x^2*y + .1*y^3
                    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                    spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                    spatialCoefficientsForTZ(3,0,0,ey)=-.5;
                    spatialCoefficientsForTZ(1,2,0,ey)=-.25;
                    spatialCoefficientsForTZ(2,1,0,ey)=-.6;
                    spatialCoefficientsForTZ(0,3,0,ey)= .1;
                    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 -1 +.5 xy + .25*x^3 - .25*y^3
                    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
                    spatialCoefficientsForTZ(1,1,0,hz)= .5;
                    spatialCoefficientsForTZ(3,0,0,hz)= .25;
                    spatialCoefficientsForTZ(0,3,0,hz)=-.25;
          // -- dispersion components: 
          // ** FINISH ME **
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else if( degreeSpace==4 || degreeSpace==5 )
                {
                    if( degreeSpace!=4 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
                    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^4 + y^4 
                    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
                    spatialCoefficientsForTZ(1,1,0,hz)= .5;
                    spatialCoefficientsForTZ(4,0,0,hz)= 1.;     
                    spatialCoefficientsForTZ(0,4,0,hz)= 1.;     
                    spatialCoefficientsForTZ(2,2,0,hz)= -.3;
                    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
                    spatialCoefficientsForTZ(1,1,0,ex)=2.;
                    spatialCoefficientsForTZ(0,2,0,ex)=1.;
                    spatialCoefficientsForTZ(4,0,0,ex)=.2;   
                    spatialCoefficientsForTZ(0,4,0,ex)=.5;   
                    spatialCoefficientsForTZ(1,3,0,ex)=1.;   
                    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
                    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                    spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                    spatialCoefficientsForTZ(4,0,0,ey)=.125;
                    spatialCoefficientsForTZ(0,4,0,ey)=-.25;
                    spatialCoefficientsForTZ(3,1,0,ey)=-.8;
          // -- dispersion components: 
          // ** FINISH ME **
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else if( degreeSpace>=6 )
                {
                    if( degreeSpace!=6 ) printF(" ****WARNING***** using a TZ function with degree=4 in space *****\n");
                    spatialCoefficientsForTZ(1,0,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,0,0,hz)= 1.;
                    spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // p=x^2 + y^2 -1 +.5 xy + x^4 + y^4 
                    spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                    spatialCoefficientsForTZ(0,0,0,hz)=-1.; 
                    spatialCoefficientsForTZ(1,1,0,hz)= .5;
                    spatialCoefficientsForTZ(4,0,0,hz)= .2;     
                    spatialCoefficientsForTZ(0,4,0,hz)= .4;     
                    spatialCoefficientsForTZ(2,2,0,hz)= -.3;
                    spatialCoefficientsForTZ(3,2,0,hz)= .4;  
                    spatialCoefficientsForTZ(2,3,0,hz)= .8;  
                    spatialCoefficientsForTZ(3,3,0,hz)= .7;  
                    spatialCoefficientsForTZ(5,1,0,hz)= .25;  
                    spatialCoefficientsForTZ(1,5,0,hz)=-.25;  
                    spatialCoefficientsForTZ(6,0,0,hz)= .2;     
                    spatialCoefficientsForTZ(0,6,0,hz)=-.2;     
          //    spatialCoefficientsForTZ=0.; // ************************************************
          //spatialCoefficientsForTZ(2,4,0,hz)= 1.;
          //spatialCoefficientsForTZ(4,2,0,hz)= 1.;
                    spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + .2*x^4 + .5*y^4 + xy^3
                    spatialCoefficientsForTZ(1,1,0,ex)=2.;
                    spatialCoefficientsForTZ(0,2,0,ex)=1.;
                    spatialCoefficientsForTZ(4,0,0,ex)=.2;   
                    spatialCoefficientsForTZ(0,4,0,ex)=.5;   
                    spatialCoefficientsForTZ(1,3,0,ex)=1.;   
                    spatialCoefficientsForTZ(3,2,0,ex)=.1;      // .1*x^3*y^2
                    spatialCoefficientsForTZ(4,2,0,ex)=.3;      // .3 x^4 y^2 ** III
                    spatialCoefficientsForTZ(3,3,0,ex)=.4;      // .4 x^3 y^3 ** IV 
                    spatialCoefficientsForTZ(6,0,0,ex)=.1;      //  + .1*x^6 +.25*y^6 -.6*x*y^5
                    spatialCoefficientsForTZ(0,6,0,ex)=.25;
                    spatialCoefficientsForTZ(1,5,0,ex)=-.6;
                    spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 +.125*x^4 -.25*y^4 -.8*x^3 y
                    spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                    spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                    spatialCoefficientsForTZ(2,3,0,ey)=-.1;      // -.1*x^2*y^3
                    spatialCoefficientsForTZ(3,3,0,ey)=-.4;     //-.4 x^3 y^3 ** III 
                    spatialCoefficientsForTZ(2,4,0,ey)=-.3;      //-.3 x^2 y^4 ** IV
                    spatialCoefficientsForTZ(4,0,0,ey)=.125;
                    spatialCoefficientsForTZ(0,4,0,ey)=-.25;
                    spatialCoefficientsForTZ(3,1,0,ey)=-.8;
                    spatialCoefficientsForTZ(6,0,0,ey)=.3;    //   .3*x^6 +.1*y^6  + .6*x^5*y 
                    spatialCoefficientsForTZ(0,6,0,ey)=.1;
                    spatialCoefficientsForTZ(5,1,0,ey)=-.6;
          // -- dispersion components: 
          // ** FINISH ME **
                    if( pxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,pxc)=1.; 
                        spatialCoefficientsForTZ(0,0,0,pyc)=2.; 
                    }
                    if( qxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,qxc)=3.; 
                        spatialCoefficientsForTZ(0,0,0,qyc)=4.; 
                    }
                    if( rxc>=0 )
                    {
                        spatialCoefficientsForTZ(0,0,0,rxc)=5.; 
                        spatialCoefficientsForTZ(0,0,0,ryc)=6.; 
                    }
                }
                else
                {
                    printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                    Overture::abort("error");
                }
            }
      // *****************************************************************
      // ******************* Three Dimensions ****************************
      // *****************************************************************
            else if( numberOfDimensions==3 )
            {
        // ** finish me -- make the E and H poly's be different
                printF("*** initTZ functions: solveForElectricField=%i solveForMagneticField=%i\n",
                              solveForElectricField,solveForMagneticField);
                if ( solveForElectricField )
                {
            // -----------------------------------------------------------------------------
            // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
            // -----------------------------------------------------------------------------
            // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
                        if( degreeSpace >=1 )
                        {
                            spatialCoefficientsForTZ(0,0,0,ex)=1.;      // u=1 + x + y + z
                            spatialCoefficientsForTZ(1,0,0,ex)=1.;
                            spatialCoefficientsForTZ(0,1,0,ex)=1.;
                            spatialCoefficientsForTZ(0,0,1,ex)=1.;
                            spatialCoefficientsForTZ(0,0,0,ey)= 2.;      // v=2+x-2y+z
                            spatialCoefficientsForTZ(1,0,0,ey)= 1.;
                            spatialCoefficientsForTZ(0,1,0,ey)=-2.;
                            spatialCoefficientsForTZ(0,0,1,ey)= 1.;
                            spatialCoefficientsForTZ(1,0,0,ez)=-1.;      // w=-x+y+z
                            spatialCoefficientsForTZ(0,1,0,ez)= 1.;
                            spatialCoefficientsForTZ(0,0,1,ez)= 1.;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                        }
                        if( degreeSpace==2 )
                        {
                            spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
                            spatialCoefficientsForTZ(1,1,0,ex)=2.;
                            spatialCoefficientsForTZ(0,2,0,ex)=1.;
                            spatialCoefficientsForTZ(1,0,1,ex)=1.;
                            spatialCoefficientsForTZ(0,1,1,ex)=-.25;
                            spatialCoefficientsForTZ(0,0,2,ex)=-.5;
                            spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
                            spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ey)=+3.;
                            spatialCoefficientsForTZ(1,0,1,ey)=.25;
                            spatialCoefficientsForTZ(0,0,2,ey)=.5;
                            spatialCoefficientsForTZ(2,0,0,ez)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
                            spatialCoefficientsForTZ(0,2,0,ez)= 1.;
                            spatialCoefficientsForTZ(0,0,2,ez)=-2.;
                            spatialCoefficientsForTZ(1,1,0,ez)=.25;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                            spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                            spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                            spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                            spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
                            spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2
                        }
                        else if( degreeSpace==0 )
                        {
                            spatialCoefficientsForTZ(0,0,0,ex)=1.; // -1.; 
                            spatialCoefficientsForTZ(0,0,0,ey)=1.; //-.5;
                            spatialCoefficientsForTZ(0,0,0,ez)=1.; //.75; 
                        }
                        else if( degreeSpace==3 )
                        {
                            spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + xz 
                            spatialCoefficientsForTZ(1,1,0,ex)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
                            spatialCoefficientsForTZ(0,2,0,ex)=1.;
                            spatialCoefficientsForTZ(1,0,1,ex)=1.;
                            spatialCoefficientsForTZ(3,0,0,ex)=.125; 
                            spatialCoefficientsForTZ(0,3,0,ex)=.125; 
                            spatialCoefficientsForTZ(0,0,3,ex)=.125; 
                            spatialCoefficientsForTZ(1,2,0,ex)=-.75;
                            spatialCoefficientsForTZ(2,0,1,ex)=+1.; 
                            spatialCoefficientsForTZ(0,1,1,ex)=.4; 
                            spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
                            spatialCoefficientsForTZ(1,1,0,ey)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
                            spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ey)=+3.;
                            spatialCoefficientsForTZ(3,0,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,3,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,0,3,ey)=.25; 
                            spatialCoefficientsForTZ(2,1,0,ey)=-3.*.125; 
                            spatialCoefficientsForTZ(0,1,2,ey)=-3.*.125; 
                            spatialCoefficientsForTZ(2,0,0,ez)= 1.;      // w=x^2 + y^2 - 2 z^2 
                            spatialCoefficientsForTZ(0,2,0,ez)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
                            spatialCoefficientsForTZ(0,0,2,ez)=-2.;
                            spatialCoefficientsForTZ(3,0,0,ez)=.25; 
                            spatialCoefficientsForTZ(0,3,0,ez)=-.2; 
                            spatialCoefficientsForTZ(0,0,3,ez)=.125; 
                            spatialCoefficientsForTZ(1,0,2,ez)=-1.;
                            spatialCoefficientsForTZ(1,2,0,ez)=-.6;
                        }
                        else if( degreeSpace==4 )
                        {
                            spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,ex)=2.;
                            spatialCoefficientsForTZ(0,2,0,ex)=1.;
                            spatialCoefficientsForTZ(1,0,1,ex)=1.;
                            spatialCoefficientsForTZ(3,0,0,ex)=.5;      // + .5*x^3
                            spatialCoefficientsForTZ(4,0,0,ex)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,ex)=.125;    
                            spatialCoefficientsForTZ(0,0,4,ex)=.125; 
                            spatialCoefficientsForTZ(1,0,3,ex)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,ex)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,ex)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,ex)=.25; 
                            spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ey)=+3.;
                            spatialCoefficientsForTZ(2,1,0,ey)=-1.5;     // -1.5x^2*y
                            spatialCoefficientsForTZ(4,0,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,0,4,ey)=.25; 
                            spatialCoefficientsForTZ(3,1,0,ey)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,ey)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,ey)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,ey)=.25; 
                            spatialCoefficientsForTZ(2,0,0,ez)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,ez)= 1.;
                            spatialCoefficientsForTZ(0,0,2,ez)=-2.;
                            spatialCoefficientsForTZ(4,0,0,ez)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ez)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,ez)=.125; 
                            spatialCoefficientsForTZ(0,3,1,ez)=-1.;
                            spatialCoefficientsForTZ(1,3,0,ez)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,ez)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,ez)=.25; 
                        }
                        else if( degreeSpace>=5 )
                        {
                            if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
                            spatialCoefficientsForTZ(2,0,0,ex)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,ex)=2.;
                            spatialCoefficientsForTZ(0,2,0,ex)=1.;
                            spatialCoefficientsForTZ(1,0,1,ex)=1.;
                            spatialCoefficientsForTZ(4,0,0,ex)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,ex)=.125;    
                            spatialCoefficientsForTZ(0,0,4,ex)=.125; 
                            spatialCoefficientsForTZ(1,0,3,ex)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,ex)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,ex)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,ex)=.25; 
                            spatialCoefficientsForTZ(0,5,0,ex)=.125;   // y^5
                            spatialCoefficientsForTZ(2,0,0,ey)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,ey)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ey)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ey)=+3.;
                            spatialCoefficientsForTZ(4,0,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ey)=.25; 
                            spatialCoefficientsForTZ(0,0,4,ey)=.25; 
                            spatialCoefficientsForTZ(3,1,0,ey)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,ey)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,ey)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,ey)=.25; 
              // spatialCoefficientsForTZ(5,0,0,ey)=.125;  // x^5
                            spatialCoefficientsForTZ(2,0,0,ez)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,ez)= 1.;
                            spatialCoefficientsForTZ(0,0,2,ez)=-2.;
                            spatialCoefficientsForTZ(4,0,0,ez)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ez)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,ez)=.125; 
                            spatialCoefficientsForTZ(0,3,1,ez)=-1.;
                            spatialCoefficientsForTZ(1,3,0,ez)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,ez)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,ez)=.25; 
              // spatialCoefficientsForTZ(5,0,0,ez)=.125;
                        }
                        else
                        {
                            printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                            Overture::abort("error");
                        }
          // *** end the initialize3DPolyTW bpp macro 
                }
                if ( solveForMagneticField )
                {
            // -----------------------------------------------------------------------------
            // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
            // -----------------------------------------------------------------------------
            // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
                        if( degreeSpace >=1 )
                        {
                            spatialCoefficientsForTZ(0,0,0,hx)=1.;      // u=1 + x + y + z
                            spatialCoefficientsForTZ(1,0,0,hx)=1.;
                            spatialCoefficientsForTZ(0,1,0,hx)=1.;
                            spatialCoefficientsForTZ(0,0,1,hx)=1.;
                            spatialCoefficientsForTZ(0,0,0,hy)= 2.;      // v=2+x-2y+z
                            spatialCoefficientsForTZ(1,0,0,hy)= 1.;
                            spatialCoefficientsForTZ(0,1,0,hy)=-2.;
                            spatialCoefficientsForTZ(0,0,1,hy)= 1.;
                            spatialCoefficientsForTZ(1,0,0,hz)=-1.;      // w=-x+y+z
                            spatialCoefficientsForTZ(0,1,0,hz)= 1.;
                            spatialCoefficientsForTZ(0,0,1,hz)= 1.;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                        }
                        if( degreeSpace==2 )
                        {
                            spatialCoefficientsForTZ(2,0,0,hx)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
                            spatialCoefficientsForTZ(1,1,0,hx)=2.;
                            spatialCoefficientsForTZ(0,2,0,hx)=1.;
                            spatialCoefficientsForTZ(1,0,1,hx)=1.;
                            spatialCoefficientsForTZ(0,1,1,hx)=-.25;
                            spatialCoefficientsForTZ(0,0,2,hx)=-.5;
                            spatialCoefficientsForTZ(2,0,0,hy)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
                            spatialCoefficientsForTZ(1,1,0,hy)=-2.;
                            spatialCoefficientsForTZ(0,2,0,hy)=-1.;
                            spatialCoefficientsForTZ(0,1,1,hy)=+3.;
                            spatialCoefficientsForTZ(1,0,1,hy)=.25;
                            spatialCoefficientsForTZ(0,0,2,hy)=.5;
                            spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
                            spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                            spatialCoefficientsForTZ(0,0,2,hz)=-2.;
                            spatialCoefficientsForTZ(1,1,0,hz)=.25;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                            spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                            spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                            spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                            spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
                            spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2
                        }
                        else if( degreeSpace==0 )
                        {
                            spatialCoefficientsForTZ(0,0,0,hx)=1.; // -1.; 
                            spatialCoefficientsForTZ(0,0,0,hy)=1.; //-.5;
                            spatialCoefficientsForTZ(0,0,0,hz)=1.; //.75; 
                        }
                        else if( degreeSpace==3 )
                        {
                            spatialCoefficientsForTZ(2,0,0,hx)=1.;      // u=x^2 + 2xy + y^2 + xz 
                            spatialCoefficientsForTZ(1,1,0,hx)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
                            spatialCoefficientsForTZ(0,2,0,hx)=1.;
                            spatialCoefficientsForTZ(1,0,1,hx)=1.;
                            spatialCoefficientsForTZ(3,0,0,hx)=.125; 
                            spatialCoefficientsForTZ(0,3,0,hx)=.125; 
                            spatialCoefficientsForTZ(0,0,3,hx)=.125; 
                            spatialCoefficientsForTZ(1,2,0,hx)=-.75;
                            spatialCoefficientsForTZ(2,0,1,hx)=+1.; 
                            spatialCoefficientsForTZ(0,1,1,hx)=.4; 
                            spatialCoefficientsForTZ(2,0,0,hy)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
                            spatialCoefficientsForTZ(1,1,0,hy)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
                            spatialCoefficientsForTZ(0,2,0,hy)=-1.;
                            spatialCoefficientsForTZ(0,1,1,hy)=+3.;
                            spatialCoefficientsForTZ(3,0,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,3,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,0,3,hy)=.25; 
                            spatialCoefficientsForTZ(2,1,0,hy)=-3.*.125; 
                            spatialCoefficientsForTZ(0,1,2,hy)=-3.*.125; 
                            spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 - 2 z^2 
                            spatialCoefficientsForTZ(0,2,0,hz)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
                            spatialCoefficientsForTZ(0,0,2,hz)=-2.;
                            spatialCoefficientsForTZ(3,0,0,hz)=.25; 
                            spatialCoefficientsForTZ(0,3,0,hz)=-.2; 
                            spatialCoefficientsForTZ(0,0,3,hz)=.125; 
                            spatialCoefficientsForTZ(1,0,2,hz)=-1.;
                            spatialCoefficientsForTZ(1,2,0,hz)=-.6;
                        }
                        else if( degreeSpace==4 )
                        {
                            spatialCoefficientsForTZ(2,0,0,hx)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,hx)=2.;
                            spatialCoefficientsForTZ(0,2,0,hx)=1.;
                            spatialCoefficientsForTZ(1,0,1,hx)=1.;
                            spatialCoefficientsForTZ(3,0,0,hx)=.5;      // + .5*x^3
                            spatialCoefficientsForTZ(4,0,0,hx)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,hx)=.125;    
                            spatialCoefficientsForTZ(0,0,4,hx)=.125; 
                            spatialCoefficientsForTZ(1,0,3,hx)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,hx)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,hx)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,hx)=.25; 
                            spatialCoefficientsForTZ(2,0,0,hy)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,hy)=-2.;
                            spatialCoefficientsForTZ(0,2,0,hy)=-1.;
                            spatialCoefficientsForTZ(0,1,1,hy)=+3.;
                            spatialCoefficientsForTZ(2,1,0,hy)=-1.5;     // -1.5x^2*y
                            spatialCoefficientsForTZ(4,0,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,4,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,0,4,hy)=.25; 
                            spatialCoefficientsForTZ(3,1,0,hy)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,hy)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,hy)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,hy)=.25; 
                            spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                            spatialCoefficientsForTZ(0,0,2,hz)=-2.;
                            spatialCoefficientsForTZ(4,0,0,hz)=.25; 
                            spatialCoefficientsForTZ(0,4,0,hz)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,hz)=.125; 
                            spatialCoefficientsForTZ(0,3,1,hz)=-1.;
                            spatialCoefficientsForTZ(1,3,0,hz)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,hz)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,hz)=.25; 
                        }
                        else if( degreeSpace>=5 )
                        {
                            if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
                            spatialCoefficientsForTZ(2,0,0,hx)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,hx)=2.;
                            spatialCoefficientsForTZ(0,2,0,hx)=1.;
                            spatialCoefficientsForTZ(1,0,1,hx)=1.;
                            spatialCoefficientsForTZ(4,0,0,hx)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,hx)=.125;    
                            spatialCoefficientsForTZ(0,0,4,hx)=.125; 
                            spatialCoefficientsForTZ(1,0,3,hx)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,hx)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,hx)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,hx)=.25; 
                            spatialCoefficientsForTZ(0,5,0,hx)=.125;   // y^5
                            spatialCoefficientsForTZ(2,0,0,hy)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,hy)=-2.;
                            spatialCoefficientsForTZ(0,2,0,hy)=-1.;
                            spatialCoefficientsForTZ(0,1,1,hy)=+3.;
                            spatialCoefficientsForTZ(4,0,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,4,0,hy)=.25; 
                            spatialCoefficientsForTZ(0,0,4,hy)=.25; 
                            spatialCoefficientsForTZ(3,1,0,hy)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,hy)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,hy)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,hy)=.25; 
              // spatialCoefficientsForTZ(5,0,0,hy)=.125;  // x^5
                            spatialCoefficientsForTZ(2,0,0,hz)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,hz)= 1.;
                            spatialCoefficientsForTZ(0,0,2,hz)=-2.;
                            spatialCoefficientsForTZ(4,0,0,hz)=.25; 
                            spatialCoefficientsForTZ(0,4,0,hz)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,hz)=.125; 
                            spatialCoefficientsForTZ(0,3,1,hz)=-1.;
                            spatialCoefficientsForTZ(1,3,0,hz)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,hz)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,hz)=.25; 
              // spatialCoefficientsForTZ(5,0,0,hz)=.125;
                        }
                        else
                        {
                            printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                            Overture::abort("error");
                        }
          // *** end the initialize3DPolyTW bpp macro 
                }
        // -- dispersion components: 
                if( pxc>=0 ) 
                {
            // -----------------------------------------------------------------------------
            // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
            // -----------------------------------------------------------------------------
            // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
                        if( degreeSpace >=1 )
                        {
                            spatialCoefficientsForTZ(0,0,0,pxc)=1.;      // u=1 + x + y + z
                            spatialCoefficientsForTZ(1,0,0,pxc)=1.;
                            spatialCoefficientsForTZ(0,1,0,pxc)=1.;
                            spatialCoefficientsForTZ(0,0,1,pxc)=1.;
                            spatialCoefficientsForTZ(0,0,0,pyc)= 2.;      // v=2+x-2y+z
                            spatialCoefficientsForTZ(1,0,0,pyc)= 1.;
                            spatialCoefficientsForTZ(0,1,0,pyc)=-2.;
                            spatialCoefficientsForTZ(0,0,1,pyc)= 1.;
                            spatialCoefficientsForTZ(1,0,0,pzc)=-1.;      // w=-x+y+z
                            spatialCoefficientsForTZ(0,1,0,pzc)= 1.;
                            spatialCoefficientsForTZ(0,0,1,pzc)= 1.;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                        }
                        if( degreeSpace==2 )
                        {
                            spatialCoefficientsForTZ(2,0,0,pxc)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
                            spatialCoefficientsForTZ(1,1,0,pxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,pxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,pxc)=1.;
                            spatialCoefficientsForTZ(0,1,1,pxc)=-.25;
                            spatialCoefficientsForTZ(0,0,2,pxc)=-.5;
                            spatialCoefficientsForTZ(2,0,0,pyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
                            spatialCoefficientsForTZ(1,1,0,pyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,pyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,pyc)=+3.;
                            spatialCoefficientsForTZ(1,0,1,pyc)=.25;
                            spatialCoefficientsForTZ(0,0,2,pyc)=.5;
                            spatialCoefficientsForTZ(2,0,0,pzc)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
                            spatialCoefficientsForTZ(0,2,0,pzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,pzc)=-2.;
                            spatialCoefficientsForTZ(1,1,0,pzc)=.25;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                            spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                            spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                            spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                            spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
                            spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2
                        }
                        else if( degreeSpace==0 )
                        {
                            spatialCoefficientsForTZ(0,0,0,pxc)=1.; // -1.; 
                            spatialCoefficientsForTZ(0,0,0,pyc)=1.; //-.5;
                            spatialCoefficientsForTZ(0,0,0,pzc)=1.; //.75; 
                        }
                        else if( degreeSpace==3 )
                        {
                            spatialCoefficientsForTZ(2,0,0,pxc)=1.;      // u=x^2 + 2xy + y^2 + xz 
                            spatialCoefficientsForTZ(1,1,0,pxc)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
                            spatialCoefficientsForTZ(0,2,0,pxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,pxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,pxc)=.125; 
                            spatialCoefficientsForTZ(0,3,0,pxc)=.125; 
                            spatialCoefficientsForTZ(0,0,3,pxc)=.125; 
                            spatialCoefficientsForTZ(1,2,0,pxc)=-.75;
                            spatialCoefficientsForTZ(2,0,1,pxc)=+1.; 
                            spatialCoefficientsForTZ(0,1,1,pxc)=.4; 
                            spatialCoefficientsForTZ(2,0,0,pyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
                            spatialCoefficientsForTZ(1,1,0,pyc)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
                            spatialCoefficientsForTZ(0,2,0,pyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,pyc)=+3.;
                            spatialCoefficientsForTZ(3,0,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,0,3,pyc)=.25; 
                            spatialCoefficientsForTZ(2,1,0,pyc)=-3.*.125; 
                            spatialCoefficientsForTZ(0,1,2,pyc)=-3.*.125; 
                            spatialCoefficientsForTZ(2,0,0,pzc)= 1.;      // w=x^2 + y^2 - 2 z^2 
                            spatialCoefficientsForTZ(0,2,0,pzc)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
                            spatialCoefficientsForTZ(0,0,2,pzc)=-2.;
                            spatialCoefficientsForTZ(3,0,0,pzc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,pzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,3,pzc)=.125; 
                            spatialCoefficientsForTZ(1,0,2,pzc)=-1.;
                            spatialCoefficientsForTZ(1,2,0,pzc)=-.6;
                        }
                        else if( degreeSpace==4 )
                        {
                            spatialCoefficientsForTZ(2,0,0,pxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,pxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,pxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,pxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,pxc)=.5;      // + .5*x^3
                            spatialCoefficientsForTZ(4,0,0,pxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,pxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,pxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,pxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,pxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,pxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,pxc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,pyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,pyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,pyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,pyc)=+3.;
                            spatialCoefficientsForTZ(2,1,0,pyc)=-1.5;     // -1.5x^2*y
                            spatialCoefficientsForTZ(4,0,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,pyc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,pyc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,pyc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,pyc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,pyc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,pzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,pzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,pzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,pzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,pzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,pzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,pzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,pzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,pzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,pzc)=.25; 
                        }
                        else if( degreeSpace>=5 )
                        {
                            if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
                            spatialCoefficientsForTZ(2,0,0,pxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,pxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,pxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,pxc)=1.;
                            spatialCoefficientsForTZ(4,0,0,pxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,pxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,pxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,pxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,pxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,pxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,pxc)=.25; 
                            spatialCoefficientsForTZ(0,5,0,pxc)=.125;   // y^5
                            spatialCoefficientsForTZ(2,0,0,pyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,pyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,pyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,pyc)=+3.;
                            spatialCoefficientsForTZ(4,0,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,pyc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,pyc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,pyc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,pyc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,pyc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,pyc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,pyc)=.125;  // x^5
                            spatialCoefficientsForTZ(2,0,0,pzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,pzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,pzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,pzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,pzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,pzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,pzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,pzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,pzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,pzc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,pzc)=.125;
                        }
                        else
                        {
                            printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                            Overture::abort("error");
                        }
          // *** end the initialize3DPolyTW bpp macro 
                }
                if( qxc>=0 ) 
                {
            // -----------------------------------------------------------------------------
            // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
            // -----------------------------------------------------------------------------
            // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
                        if( degreeSpace >=1 )
                        {
                            spatialCoefficientsForTZ(0,0,0,qxc)=1.;      // u=1 + x + y + z
                            spatialCoefficientsForTZ(1,0,0,qxc)=1.;
                            spatialCoefficientsForTZ(0,1,0,qxc)=1.;
                            spatialCoefficientsForTZ(0,0,1,qxc)=1.;
                            spatialCoefficientsForTZ(0,0,0,qyc)= 2.;      // v=2+x-2y+z
                            spatialCoefficientsForTZ(1,0,0,qyc)= 1.;
                            spatialCoefficientsForTZ(0,1,0,qyc)=-2.;
                            spatialCoefficientsForTZ(0,0,1,qyc)= 1.;
                            spatialCoefficientsForTZ(1,0,0,qzc)=-1.;      // w=-x+y+z
                            spatialCoefficientsForTZ(0,1,0,qzc)= 1.;
                            spatialCoefficientsForTZ(0,0,1,qzc)= 1.;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                        }
                        if( degreeSpace==2 )
                        {
                            spatialCoefficientsForTZ(2,0,0,qxc)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
                            spatialCoefficientsForTZ(1,1,0,qxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,qxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,qxc)=1.;
                            spatialCoefficientsForTZ(0,1,1,qxc)=-.25;
                            spatialCoefficientsForTZ(0,0,2,qxc)=-.5;
                            spatialCoefficientsForTZ(2,0,0,qyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
                            spatialCoefficientsForTZ(1,1,0,qyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,qyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,qyc)=+3.;
                            spatialCoefficientsForTZ(1,0,1,qyc)=.25;
                            spatialCoefficientsForTZ(0,0,2,qyc)=.5;
                            spatialCoefficientsForTZ(2,0,0,qzc)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
                            spatialCoefficientsForTZ(0,2,0,qzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,qzc)=-2.;
                            spatialCoefficientsForTZ(1,1,0,qzc)=.25;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                            spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                            spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                            spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                            spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
                            spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2
                        }
                        else if( degreeSpace==0 )
                        {
                            spatialCoefficientsForTZ(0,0,0,qxc)=1.; // -1.; 
                            spatialCoefficientsForTZ(0,0,0,qyc)=1.; //-.5;
                            spatialCoefficientsForTZ(0,0,0,qzc)=1.; //.75; 
                        }
                        else if( degreeSpace==3 )
                        {
                            spatialCoefficientsForTZ(2,0,0,qxc)=1.;      // u=x^2 + 2xy + y^2 + xz 
                            spatialCoefficientsForTZ(1,1,0,qxc)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
                            spatialCoefficientsForTZ(0,2,0,qxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,qxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,qxc)=.125; 
                            spatialCoefficientsForTZ(0,3,0,qxc)=.125; 
                            spatialCoefficientsForTZ(0,0,3,qxc)=.125; 
                            spatialCoefficientsForTZ(1,2,0,qxc)=-.75;
                            spatialCoefficientsForTZ(2,0,1,qxc)=+1.; 
                            spatialCoefficientsForTZ(0,1,1,qxc)=.4; 
                            spatialCoefficientsForTZ(2,0,0,qyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
                            spatialCoefficientsForTZ(1,1,0,qyc)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
                            spatialCoefficientsForTZ(0,2,0,qyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,qyc)=+3.;
                            spatialCoefficientsForTZ(3,0,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,0,3,qyc)=.25; 
                            spatialCoefficientsForTZ(2,1,0,qyc)=-3.*.125; 
                            spatialCoefficientsForTZ(0,1,2,qyc)=-3.*.125; 
                            spatialCoefficientsForTZ(2,0,0,qzc)= 1.;      // w=x^2 + y^2 - 2 z^2 
                            spatialCoefficientsForTZ(0,2,0,qzc)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
                            spatialCoefficientsForTZ(0,0,2,qzc)=-2.;
                            spatialCoefficientsForTZ(3,0,0,qzc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,qzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,3,qzc)=.125; 
                            spatialCoefficientsForTZ(1,0,2,qzc)=-1.;
                            spatialCoefficientsForTZ(1,2,0,qzc)=-.6;
                        }
                        else if( degreeSpace==4 )
                        {
                            spatialCoefficientsForTZ(2,0,0,qxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,qxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,qxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,qxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,qxc)=.5;      // + .5*x^3
                            spatialCoefficientsForTZ(4,0,0,qxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,qxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,qxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,qxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,qxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,qxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,qxc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,qyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,qyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,qyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,qyc)=+3.;
                            spatialCoefficientsForTZ(2,1,0,qyc)=-1.5;     // -1.5x^2*y
                            spatialCoefficientsForTZ(4,0,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,qyc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,qyc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,qyc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,qyc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,qyc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,qzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,qzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,qzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,qzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,qzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,qzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,qzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,qzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,qzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,qzc)=.25; 
                        }
                        else if( degreeSpace>=5 )
                        {
                            if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
                            spatialCoefficientsForTZ(2,0,0,qxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,qxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,qxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,qxc)=1.;
                            spatialCoefficientsForTZ(4,0,0,qxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,qxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,qxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,qxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,qxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,qxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,qxc)=.25; 
                            spatialCoefficientsForTZ(0,5,0,qxc)=.125;   // y^5
                            spatialCoefficientsForTZ(2,0,0,qyc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,qyc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,qyc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,qyc)=+3.;
                            spatialCoefficientsForTZ(4,0,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,qyc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,qyc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,qyc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,qyc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,qyc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,qyc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,qyc)=.125;  // x^5
                            spatialCoefficientsForTZ(2,0,0,qzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,qzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,qzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,qzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,qzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,qzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,qzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,qzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,qzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,qzc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,qzc)=.125;
                        }
                        else
                        {
                            printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                            Overture::abort("error");
                        }
          // *** end the initialize3DPolyTW bpp macro 
                }
                if( rxc>=0 ) 
                {
            // -----------------------------------------------------------------------------
            // --------------------- DEFINE POLYNOMIAL TZ SOLUTIONS ------------------------
            // -----------------------------------------------------------------------------
            // Always include linear terms in TZ if degreSpace>=1 *wdh* Sept 18, 2016 
                        if( degreeSpace >=1 )
                        {
                            spatialCoefficientsForTZ(0,0,0,rxc)=1.;      // u=1 + x + y + z
                            spatialCoefficientsForTZ(1,0,0,rxc)=1.;
                            spatialCoefficientsForTZ(0,1,0,rxc)=1.;
                            spatialCoefficientsForTZ(0,0,1,rxc)=1.;
                            spatialCoefficientsForTZ(0,0,0,ryc)= 2.;      // v=2+x-2y+z
                            spatialCoefficientsForTZ(1,0,0,ryc)= 1.;
                            spatialCoefficientsForTZ(0,1,0,ryc)=-2.;
                            spatialCoefficientsForTZ(0,0,1,ryc)= 1.;
                            spatialCoefficientsForTZ(1,0,0,rzc)=-1.;      // w=-x+y+z
                            spatialCoefficientsForTZ(0,1,0,rzc)= 1.;
                            spatialCoefficientsForTZ(0,0,1,rzc)= 1.;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                        }
                        if( degreeSpace==2 )
                        {
                            spatialCoefficientsForTZ(2,0,0,rxc)=1.;      // u=x^2 + 2xy + y^2 + xz  - .25*yz -.5*z^2
                            spatialCoefficientsForTZ(1,1,0,rxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,rxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,rxc)=1.;
                            spatialCoefficientsForTZ(0,1,1,rxc)=-.25;
                            spatialCoefficientsForTZ(0,0,2,rxc)=-.5;
                            spatialCoefficientsForTZ(2,0,0,ryc)= 1.;      // v=x^2 -2xy - y^2 + 3yz + .25*xz +.5*z^2
                            spatialCoefficientsForTZ(1,1,0,ryc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ryc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ryc)=+3.;
                            spatialCoefficientsForTZ(1,0,1,ryc)=.25;
                            spatialCoefficientsForTZ(0,0,2,ryc)=.5;
                            spatialCoefficientsForTZ(2,0,0,rzc)= 1.;      // w=x^2 + y^2 - 2 z^2 + .25*xy 
                            spatialCoefficientsForTZ(0,2,0,rzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,rzc)=-2.;
                            spatialCoefficientsForTZ(1,1,0,rzc)=.25;
              // eps and mu should remain positive 
                            spatialCoefficientsForTZ(1,0,0,epsc)=eps*.01;  // x
                            spatialCoefficientsForTZ(0,1,0,epsc)=eps*.02;  // y
                            spatialCoefficientsForTZ(0,0,1,epsc)=eps*.12;  // z
                            spatialCoefficientsForTZ(2,0,0,epsc)=eps*.1;   // x^2
                            spatialCoefficientsForTZ(0,2,0,epsc)=eps*.15;  // y^2        
                            spatialCoefficientsForTZ(0,0,2,epsc)=eps*.11;  // z^2        
                            spatialCoefficientsForTZ(1,0,0,muc )=mu*.015;   // x
                            spatialCoefficientsForTZ(0,1,0,muc )=mu*.0125;  // y
                            spatialCoefficientsForTZ(0,0,1,muc )=mu*.095;   // z
                            spatialCoefficientsForTZ(2,0,0,muc )=mu*.125;   // x^2
                            spatialCoefficientsForTZ(0,2,0,muc )=mu*.15;    // y^2
                            spatialCoefficientsForTZ(0,0,2,muc )=mu*.13;    // z^2
                        }
                        else if( degreeSpace==0 )
                        {
                            spatialCoefficientsForTZ(0,0,0,rxc)=1.; // -1.; 
                            spatialCoefficientsForTZ(0,0,0,ryc)=1.; //-.5;
                            spatialCoefficientsForTZ(0,0,0,rzc)=1.; //.75; 
                        }
                        else if( degreeSpace==3 )
                        {
                            spatialCoefficientsForTZ(2,0,0,rxc)=1.;      // u=x^2 + 2xy + y^2 + xz 
                            spatialCoefficientsForTZ(1,1,0,rxc)=2.;    //        + .125( x^3 + y^3 + z^3 ) -.75*x*y^2 + x^2*z +.4yz
                            spatialCoefficientsForTZ(0,2,0,rxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,rxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,rxc)=.125; 
                            spatialCoefficientsForTZ(0,3,0,rxc)=.125; 
                            spatialCoefficientsForTZ(0,0,3,rxc)=.125; 
                            spatialCoefficientsForTZ(1,2,0,rxc)=-.75;
                            spatialCoefficientsForTZ(2,0,1,rxc)=+1.; 
                            spatialCoefficientsForTZ(0,1,1,rxc)=.4; 
                            spatialCoefficientsForTZ(2,0,0,ryc)= 1.;      // v=x^2 -2xy - y^2 + 3yz 
                            spatialCoefficientsForTZ(1,1,0,ryc)=-2.;      //    + .25( x^3 + y^3 + z^3 ) -.375*x^2 y  -.375*y*z^2  
                            spatialCoefficientsForTZ(0,2,0,ryc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ryc)=+3.;
                            spatialCoefficientsForTZ(3,0,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,0,3,ryc)=.25; 
                            spatialCoefficientsForTZ(2,1,0,ryc)=-3.*.125; 
                            spatialCoefficientsForTZ(0,1,2,ryc)=-3.*.125; 
                            spatialCoefficientsForTZ(2,0,0,rzc)= 1.;      // w=x^2 + y^2 - 2 z^2 
                            spatialCoefficientsForTZ(0,2,0,rzc)= 1.;      //      + .25x^3 -.2y^3 +.125 z^3 - x z^2 -.6*xy^2
                            spatialCoefficientsForTZ(0,0,2,rzc)=-2.;
                            spatialCoefficientsForTZ(3,0,0,rzc)=.25; 
                            spatialCoefficientsForTZ(0,3,0,rzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,3,rzc)=.125; 
                            spatialCoefficientsForTZ(1,0,2,rzc)=-1.;
                            spatialCoefficientsForTZ(1,2,0,rzc)=-.6;
                        }
                        else if( degreeSpace==4 )
                        {
                            spatialCoefficientsForTZ(2,0,0,rxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,rxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,rxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,rxc)=1.;
                            spatialCoefficientsForTZ(3,0,0,rxc)=.5;      // + .5*x^3
                            spatialCoefficientsForTZ(4,0,0,rxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,rxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,rxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,rxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,rxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,rxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,rxc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,ryc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,ryc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ryc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ryc)=+3.;
                            spatialCoefficientsForTZ(2,1,0,ryc)=-1.5;     // -1.5x^2*y
                            spatialCoefficientsForTZ(4,0,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,ryc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,ryc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,ryc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,ryc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,ryc)=.25; 
                            spatialCoefficientsForTZ(2,0,0,rzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,rzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,rzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,rzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,rzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,rzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,rzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,rzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,rzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,rzc)=.25; 
                        }
                        else if( degreeSpace>=5 )
                        {
                            if( true || degreeSpace!=5 ) printF(" ****WARNING***** using a TZ function with degree=5 in space *****\n");
                            spatialCoefficientsForTZ(2,0,0,rxc)=1.;      // u=x^2 + 2xy + y^2 + xz
                            spatialCoefficientsForTZ(1,1,0,rxc)=2.;
                            spatialCoefficientsForTZ(0,2,0,rxc)=1.;
                            spatialCoefficientsForTZ(1,0,1,rxc)=1.;
                            spatialCoefficientsForTZ(4,0,0,rxc)=.125;    // + .125*x^4 + .125*y^4 + .125*z^4  -.5*xz^3
                            spatialCoefficientsForTZ(0,4,0,rxc)=.125;    
                            spatialCoefficientsForTZ(0,0,4,rxc)=.125; 
                            spatialCoefficientsForTZ(1,0,3,rxc)=-.5; 
                            spatialCoefficientsForTZ(0,1,3,rxc)=.25;    // + .25*y*z^3 -.25*y^2*z^2 +.25*y^3z
                            spatialCoefficientsForTZ(0,2,2,rxc)=-.25; 
                            spatialCoefficientsForTZ(0,3,1,rxc)=.25; 
                            spatialCoefficientsForTZ(0,5,0,rxc)=.125;   // y^5
                            spatialCoefficientsForTZ(2,0,0,ryc)= 1.;      // v=x^2 -2xy - y^2 + 3yz
                            spatialCoefficientsForTZ(1,1,0,ryc)=-2.;
                            spatialCoefficientsForTZ(0,2,0,ryc)=-1.;
                            spatialCoefficientsForTZ(0,1,1,ryc)=+3.;
                            spatialCoefficientsForTZ(4,0,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,ryc)=.25; 
                            spatialCoefficientsForTZ(0,0,4,ryc)=.25; 
                            spatialCoefficientsForTZ(3,1,0,ryc)=-.5; 
                            spatialCoefficientsForTZ(1,0,3,ryc)=.25;    // + .25*x*z^3 -.25*x^2*z^2 +.25*x^3z
                            spatialCoefficientsForTZ(2,0,2,ryc)=-.25; 
                            spatialCoefficientsForTZ(3,0,1,ryc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,ryc)=.125;  // x^5
                            spatialCoefficientsForTZ(2,0,0,rzc)= 1.;      // w=x^2 + y^2 - 2 z^2
                            spatialCoefficientsForTZ(0,2,0,rzc)= 1.;
                            spatialCoefficientsForTZ(0,0,2,rzc)=-2.;
                            spatialCoefficientsForTZ(4,0,0,rzc)=.25; 
                            spatialCoefficientsForTZ(0,4,0,rzc)=-.2; 
                            spatialCoefficientsForTZ(0,0,4,rzc)=.125; 
                            spatialCoefficientsForTZ(0,3,1,rzc)=-1.;
                            spatialCoefficientsForTZ(1,3,0,rzc)=.25;    // + .25*x*y^3 -.25*x^2*y^2 +.25*x^3y
                            spatialCoefficientsForTZ(2,2,0,rzc)=-.25; 
                            spatialCoefficientsForTZ(3,1,0,rzc)=.25; 
              // spatialCoefficientsForTZ(5,0,0,rzc)=.125;
                        }
                        else
                        {
                            printF("Maxwell:: not implemented for degree in space =%i \n",degreeSpace);
                            Overture::abort("error");
                        }
          // *** end the initialize3DPolyTW bpp macro 
                }
            }
            else
            {
                OV_ABORT("ERROR:unimplemented number of dimensions");
            }
            for( int n=0; n<numberOfComponents; n++ )
            {
                for( int i=0; i<ndp; i++ )
                    timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
            }
            if( method==sosup )
            {
        // Set the TZ function for (ext,eyt,...) equal to the time derivative of (ex,ey,...)
                const int numberOfFieldComponents=3;  // 2D: (ex,ey,hz),  3D: (ex,ey,ez)
                for( int n=ex, nt=ext; n<ex+numberOfFieldComponents; n++, nt++ )
                {
                    for( int i1=0; i1<ndp; i1++ )for( int i2=0; i2<ndp; i2++ )for( int i3=0; i3<ndp; i3++ )
                    {
                        spatialCoefficientsForTZ(i1,i2,i3,nt)=spatialCoefficientsForTZ(i1,i2,i3,n);
                    }
          // E =   a0 + a1*t + a2*t^2 + ...  = [a0,a1,a2,...
          // E_t =      a1   +2*a2*t + 3*a3*t^2  = [a1,2*a2,3*a3
                    for( int i=0; i<ndp; i++ )
                        timeCoefficientsForTZ(i,nt)= i<degreeTime ? real(i+1.)/(i+2.) : 0. ;
                }
            }
      // Make eps, mu, .. constant in time : 
            timeCoefficientsForTZ(0,rc)=1.;
            timeCoefficientsForTZ(0,epsc)=1.;
            timeCoefficientsForTZ(0,muc)=1.;
            timeCoefficientsForTZ(0,sigmaEc)=1.;
            timeCoefficientsForTZ(0,sigmaHc)=1.;
      // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
            ((OGPolyFunction*)tz)->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ ); 
      // real epsEx = ((OGPolyFunction*)tz)->gd(0,0,0,0,.0,0.,0.,epsc,0.);
      // printF(" ********** epsEx = %e *********\n",epsEx);

        }
        else if( twilightZoneOption==trigonometricTwilightZone )
        {

            const int nc = numberOfComponents + int(useChargeDensity);  // include charge density in TZ
            RealArray fx(nc),fy(nc),fz(nc),ft(nc);
            RealArray gx(nc),gy(nc),gz(nc),gt(nc);
            gx=0.;
            gy=0.;
            gz=0.;
            gt=0.;
            RealArray amplitude(nc), cc(nc);
            amplitude=1.;
            cc=0.;
            fx=omega[0];
            fy = numberOfDimensions>1 ? omega[1] : 0.;
            fz = numberOfDimensions>2 ? omega[2] : 0.;
            ft = omega[3];
            if( numberOfDimensions==2  )
            {   
                const int uc=ex, vc=ey, wc=hz;
                if( !useChargeDensity )
                {
          // rho=0 : create div(E)=0 TZ function
          // u=cos(pi x) cos( pi y )* .5
          // v=sin(pi x) sin( pi y )* .5 
          // w=cos(    ) sin(      )* .5
                    assert( omega[0]==omega[1] );
                    gx(vc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
                    gy(vc)=.5/omega[1];
                    amplitude(uc)=.5;  cc(uc)=.0;
                    amplitude(vc)=.5;  cc(vc)=.0;
                    gy(wc)=.5/omega[1]; // turn off for testing symmetry
                    cc(wc)=.0;
                }
                else
                {
          // rho= cos(pi x) cos( pi y )* omega[0]*pi 
          // 
          // u=sin(pi x) cos( pi y )* .5
          // v=cos(pi x) sin( pi y )* .5 
          // w=cos(    ) sin(      )* .5
                    assert( omega[0]==omega[1] );
                    assert( rc==numberOfComponents );
                    gx(uc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
                    gy(vc)=.5/omega[1];
                    amplitude(rc)=omega[0]*Pi;  cc(rc)=.0;
                    amplitude(uc)=.5;           cc(uc)=.0;
                    amplitude(vc)=.5;           cc(vc)=.0;
                    gy(wc)=.5/omega[1]; // turn off for testing symmetry
                    cc(wc)=.0;
                }
            }
            else if( numberOfDimensions==3 )
            {
                if( solveForElectricField )
                {
                    const int uc=ex, vc=ey, wc=ez;
          // u=   cos(pi x) cos( pi y ) cos( pi z)  // **** fix ***
          // v=.5 sin(pi x) sin( pi y ) cos( pi z)
          // w=.5 sin(pi x) cos( pi y ) sin( pi z)
                    if( omega[0]==omega[1] && omega[0]==omega[2] )
                    {
                        gx(vc)=.5/omega[0];
                        gy(vc)=.5/omega[1];
                        amplitude(vc)=.5;
                        gx(wc)=.5/omega[0];
                        gz(wc)=.5/omega[2];
                        amplitude(wc)=.5;
                    }
                    else if( omega[0]==omega[2] && omega[1]==0 )
                    {
            // pseudo 2D case
                        gx(wc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
                        gz(wc)=.5/omega[2];
                        amplitude(uc)=.5;  cc(uc)=.0;
                        amplitude(wc)=.5;  cc(wc)=.0;
                    }
                    else if( omega[0]==omega[1] && omega[2]==0 )
                    {
            // pseudo 2D case
            // u=cos(pi x) cos( pi y ) cos( 0*pi z)* .5
            // v=sin(pi x) sin( pi y ) cos( 0*pi z)* .5 
            // w=cos(pi x) cos( pi y ) cos( 0*pi z)* .5
                        gx(vc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
                        gy(vc)=.5/omega[1];
                        amplitude(uc)=.5;  cc(uc)=.0;
                        amplitude(vc)=.5;  cc(vc)=.0;
                        amplitude(wc)=.5;  cc(wc)=.0;
                    }
                    else
                    {
                        printF("Cgmx: invalid values for omega: omega[0]=%9.3e, omega[1]=%9.3e, omega[2]=%9.3e\n",
                       	     omega[0],omega[1],omega[2]);
                        printF("Expecting all equal values or omega[0]==omega[2] && omega[1]==0 \n"
                                      " or omega[0]==omega[1] && omega[2]==0 (for divergence free field\n");
                        Overture::abort("Invalid values for omega[0..2]");
                    }
                }
                if( solveForMagneticField )
                    {
                        const int uc=hx, vc=hy, wc=hz;
          // u=   cos(pi x) cos( pi y ) cos( pi z)  // **** fix ***
          // v=.5 sin(pi x) sin( pi y ) cos( pi z)
          // w=.5 sin(pi x) cos( pi y ) sin( pi z)
                    if( omega[0]==omega[1] && omega[0]==omega[2] )
                    {
                        gx(vc)=.5/omega[0];
                        gy(vc)=.5/omega[1];
                        amplitude(vc)=.5;
                        gx(wc)=.5/omega[0];
                        gz(wc)=.5/omega[2];
                        amplitude(wc)=.5;
                    }
                    else if( omega[0]==omega[2] && omega[1]==0 )
                    {
            // pseudo 2D case
                        gx(wc)=.5/omega[0];   // shift by pi/2 to turn cos() into sin()
                        gz(wc)=.5/omega[2];
                        amplitude(uc)=.5;  cc(uc)=.0;
                        amplitude(wc)=.5;  cc(wc)=.0;
                    }
                    else
                    {
                        Overture::abort("Invalid values for omega[0..2]");
                    }
                }
            }
            if( method==sosup )
            {
        // Set the TZ function for (ext,eyt,...) equal to the time derivative of (ex,ey,...)
        // time dependence for time-derivatives of E:
                const int numberOfFieldComponents=3;
                for( int n=ex, nt=ext; n<ex+numberOfFieldComponents; n++,nt++ )
                {
                    fx(nt)=fx(n); fy(nt)=fy(n); fz(nt)=fz(n); ft(nt)=ft(n);
                    gx(nt)=gx(n); gy(nt)=gy(n); gz(nt)=gz(n); 
                    gt(nt)=.5/ft(n);  // shift phase by pi/2 to turn cos(Pi*ft*(t-gt)) into sin(Pi*ft*(t-gt))
                    amplitude(nt)=-amplitude(n)*ft(n)*Pi;  // amplitude
                    cc(nt)=0.; 
                }
      //   fx(eyt)=fx(ey); gx(eyt)=gx(ey); amplitude(eyt)=-amplitude(ey);  cc(eyt)=0.; ft(eyt)=ft(ey); gt(eyt)=.5/omega[3]; 
      //   fx(hzt)=fx(hz); gx(hzt)=gx(hz); amplitude(hzt)=-amplitude(hz);  cc(hzt)=0.; ft(hzt)=ft(hz); gt(hzt)=.5/omega[3]; 
            }
            tz = new OGTrigFunction(fx,fy,fz,ft);
            ((OGTrigFunction*)tz)->setShifts(gx,gy,gz,gt);
            ((OGTrigFunction*)tz)->setAmplitudes(amplitude);
            ((OGTrigFunction*)tz)->setConstants(cc);

        }
        else if( twilightZoneOption==pulseTwilightZone )
        {
            tz= new OGPulseFunction;
        }
        else
        {
            printF("assignInitialConditions:ERROR:unknown value for twilightZoneOption=%i\n",(int)twilightZoneOption);
            OV_ABORT("assignInitialConditions:ERROR");
        }

    };


    
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    const int numberOfComponentGrids = cg.numberOfComponentGrids();

  // -- moved to setup grid functions --
  // if( (method==nfdtd  || method==sosup ) && orderOfAccuracyInTime>=4 )
  // {
  //   // ** functions only needed for curvilinear grids **** fix ****
  //   numberOfFunctions=1; //  orderOfAccuracyInTime-1;

  //   if( true || cg.numberOfDimensions()==3 ) numberOfFunctions=3;  // *********************** fix *************** how many are needed?

  //   delete [] fn;
  //   fn = new realArray [numberOfFunctions*numberOfComponentGrids];
  // }


    if( initialConditionOption==planeWaveScatteredFieldInitialCondition && knownSolution==NULL )
    {
        printF("Setting initial conditions to be planeWaveScatteredFieldInitialCondition. dt=%14.6e\n",dt);
    // if ( method!=nfdtd && cg.numberOfDimensions()==3 )
    //   printF("getInitialConditions:ERROR: initialConditionOption==planeWaveScatteredFieldInitialCondition "
    //          "not implemented for staggered grids yet in 3D.\n");

            initializeKnownSolution();
    }
    

    if( initialConditionOption==userDefinedInitialConditionsOption )
    {
    // -- evaluate user defined initial conditions --

        userDefinedInitialConditions( current, t, dt );

    }
    else if( initialConditionOption==userDefinedKnownSolutionInitialCondition )
    {
        assignUserDefinedKnownSolutionInitialConditions( current, t, dt );

    }
    else
    {

#define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define X2(i0,i1,i2) (za+dz0*(i2-i2a))


        for( int grid=0; grid<numberOfComponentGrids; grid++ )
        {

            if( method==yee )
            { 
	// Compute the initial conditions for the Yee method
      	assert( numberOfComponentGrids==1 );
      	int option=0;
      	int iparm[5] = { -1,-1,0,0,0 }; // 
      	getValuesFDTD( option, iparm, current, t, dt );
      	getValuesFDTD( option, iparm, prev, t-dt, dt );
    
      	if( useTwilightZoneMaterials )
      	{
	  // define material properties from the twilight zone
        	  assert( tz!=NULL );
        	  OGFunction & e = *tz;

        	  printF(" ***** Assign the variable coefficient material properties eps(x,y,z) and mu(x,y,z)"
             		 " from the twilight zone ***\n");

        	  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    
        	  MappedGrid & mg = cg[grid];
        	  mg.update(MappedGrid::THEmask );
        	  intArray & mask = mg.mask();
#ifdef USE_PPP
        	  intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
        	  intSerialArray & maskLocal = mask; 
#endif

        	  int extra=1; // include ghost points since we average cell centered values to edges and faces
        	  getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
        	  int includeGhost=0;
        	  bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
        	  if( ok )
        	  {
	    // Each cell gets a different material
          	    media.redim(maskLocal.dimension(0),maskLocal.dimension(1),maskLocal.dimension(2));
          	    media=0;
        	  
          	    numberOfMaterialRegions=I1.getLength()*I2.getLength()*I3.getLength();
          	    epsv.resize(numberOfMaterialRegions); muv.resize(numberOfMaterialRegions); 
          	    sigmaEv.resize(numberOfMaterialRegions); sigmaHv.resize(numberOfMaterialRegions);

          	    real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
          	    mg.getRectangularGridParameters( dx, xab );
        	  
          	    const int i0a=mg.gridIndexRange(0,0);
          	    const int i1a=mg.gridIndexRange(0,1);
          	    const int i2a=mg.gridIndexRange(0,2);

          	    const real xa=xab[0][0], dx0=dx[0];
          	    const real ya=xab[0][1], dy0=dx[1];
          	    const real za=xab[0][2], dz0=dx[2];

          	    real zc=0.;
          	    int nr=0;
          	    int i1,i2,i3;
          	    FOR_3D(i1,i2,i3,I1,I2,I3)
          	    {
	      // cell-center: 
            	      real xc=X0(i1,i2,i3)+.5*dx[0];
            	      real yc=X1(i1,i2,i3)+.5*dx[1];
            	      if( numberOfDimensions==3 ) 
            		zc=X2(i1,i2,i3)+.5*dx[2];
            
	      // Here we assume the material properaties are independent of time 
            	      media(i1,i2,i3)=nr;
            	      epsv(nr)   =e(xc,yc,zc,epsc,t);
            	      muv(nr)    =e(xc,yc,zc,muc,t);
            	      sigmaEv(nr)=e(xc,yc,zc,sigmaEc,t);
            	      sigmaHv(nr)=e(xc,yc,zc,sigmaHc,t);
            	      nr++;
          	    }
          	    assert( nr==numberOfMaterialRegions );
        	  }
      	}

      	continue;
            }

            const real c = cGrid(grid);
            const real eps = epsGrid(grid);
            const real mu = muGrid(grid);

            const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);
            const real csq=c*c;

      // call the bpp macro to define the E and H field gridfunctions and cpp macros
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
            if( !ok ) continue;  // no communication allowed after this point : check this ******************************************************
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
                    uer.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,numberOfDimensions);
                    uerm.updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,numberOfDimensions);
                    if ( numberOfDimensions==2 )
                    {
                        uhr.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
                        uhrm.updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
                    }
                    else
                    {
                        uhr.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,numberOfDimensions);
                        uhrm.updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,numberOfDimensions);
                    }      
                    uer = uerm = -100;
                    uhr = uhrm = -100;
                    #ifdef USE_PPP
                        realSerialArray uerLocal; getLocalArrayWithGhostBoundaries(uer,uerLocal);
                        realSerialArray uhrLocal; getLocalArrayWithGhostBoundaries(uhr,uhrLocal);
                        realSerialArray uermLocal; getLocalArrayWithGhostBoundaries(uerm,uermLocal);
                        realSerialArray uhrmLocal; getLocalArrayWithGhostBoundaries(uhrm,uhrmLocal);
                    #else
                        realSerialArray & uerLocal=uer;
                        realSerialArray & uhrLocal=uhr;
                        realSerialArray & uermLocal=uerm;
                        realSerialArray & uhrmLocal=uhrm;
                    #endif
                    ue.reference ( uerLocal );
                    uh.reference ( uhrLocal );
                    ume.reference ( uermLocal );
                    umh.reference ( uhrmLocal );
          //      cout<<"UMH SIZE "<<umh.getLength(0)<<"  "<<umh.getLength(3)<<endl;
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

            const int i0a=mg.gridIndexRange(0,0);
            const int i1a=mg.gridIndexRange(0,1);
            const int i2a=mg.gridIndexRange(0,2);

            const real xa=xab[0][0], dx0=dx[0];
            const real ya=xab[0][1], dy0=dx[1];
            const real za=xab[0][2], dz0=dx[2];

            Index J1,J2,J3;
            int i1,i2,i3;

            const bool saveExtraForcingLevels = orderOfAccuracyInTime>=4 && timeSteppingMethod!=modifiedEquationTimeStepping;

            uh = umh = 0;
            ue = ume = 0;

            #define FN(m) fn[m+numberOfFunctions*(grid)]

            if( true /*numberOfComponents==3 || method==nfdtd*/ )
            {
// 	if( saveExtraForcingLevels || 
// 	    !isRectangular )  // ********************** fix this -- FN used in advanceStructured *****************
// 	{
// 	  // we need to save the "RHS" at some previous times.
// 	  Index D1,D2,D3;
// 	  getIndex(mg.dimension(),D1,D2,D3);
// 	  Range C(ex,hz);
// 	  for( int m=0; m<numberOfFunctions; m++ )
// 	  {
// 	    FN(m).partition(mg.getPartition());
// 	    FN(m).redim(D1,D2,D3,C);
// 	  }
// 	  currentFn=0; 
// 	}

      	Range C(ex,hz);

      	getIndex(mg.dimension(),I1,I2,I3);  // ***************** fix this -- needed for bug in OGP

      	if( forcingOption==twilightZoneForcing )
      	{
          // ==================================================
	  // ================== TZ FORCING ====================
          // ==================================================

        	  assert( tz!=NULL );
        	  OGFunction & e = *tz;
          	    
        	  if( mg.numberOfDimensions()==2 )
        	  {
            
	    // these ranges should work since we get the u*Dim* from the local raw data sizes (??!!)
          	    J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
          	    J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
          	    J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));


          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {

            	      real xe0 = XEP(i1,i2,i3,0);
            	      real ye0 = XEP(i1,i2,i3,1);

            	      UEX(i1,i2,i3) =e(xe0,ye0,0.,ex,tE);
            	      UEY(i1,i2,i3) =e(xe0,ye0,0.,ey,tE);
                            if( method==sosup )
            	      {
            		uLocal(i1,i2,i3,ext) =e(xe0,ye0,0.,ext,tE);
              	        uLocal(i1,i2,i3,eyt) =e(xe0,ye0,0.,eyt,tE);
            	      }
            	      

              // -- dispersion model components --
            	      if( pxc>=0 )
            	      {
                                uLocal(i1,i2,i3,pxc) =e(xe0,ye0,0.,pxc,tE);
                                uLocal(i1,i2,i3,pyc) =e(xe0,ye0,0.,pyc,tE);
                            }
            	      if( qxc>=0 )
            	      {
                                uLocal(i1,i2,i3,qxc) =e(xe0,ye0,0.,qxc,tE);
                                uLocal(i1,i2,i3,qyc) =e(xe0,ye0,0.,qyc,tE);
                            }
            	      if( rxc>=0 )
            	      {
                                uLocal(i1,i2,i3,rxc) =e(xe0,ye0,0.,rxc,tE);
                                uLocal(i1,i2,i3,ryc) =e(xe0,ye0,0.,ryc,tE);
                            }
            	      
            	      
          	    }
            		
          	    if ( method!=sosup )
          	    {
            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		real xe0 = XEP(i1,i2,i3,0);
            		real ye0 = XEP(i1,i2,i3,1);
            		UMEX(i1,i2,i3)=e(xe0,ye0,0.,ex,tE-dt);
            		UMEY(i1,i2,i3)=e(xe0,ye0,0.,ey,tE-dt);
                // -- dispersion model components --
              	        if( pxc>=0 )
              	        {
                                    umLocal(i1,i2,i3,pxc) =e(xe0,ye0,0.,pxc,tE-dt);
                                    umLocal(i1,i2,i3,pyc) =e(xe0,ye0,0.,pyc,tE-dt);
                                }
              	        if( qxc>=0 )
              	        {
                                    umLocal(i1,i2,i3,qxc) =e(xe0,ye0,0.,qxc,tE-dt);
                                    umLocal(i1,i2,i3,qyc) =e(xe0,ye0,0.,qyc,tE-dt);
                                }
              	        if( rxc>=0 )
              	        {
                                    umLocal(i1,i2,i3,rxc) =e(xe0,ye0,0.,rxc,tE-dt);
                                    umLocal(i1,i2,i3,ryc) =e(xe0,ye0,0.,ryc,tE-dt);
                                }
            	      }
          	    }

        	  J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
        	  J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
        	  J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));

        	  FOR_3(i1,i2,i3,J1,J2,J3)
        	  {
          	    real xh0 = XHP(i1,i2,i3,0);
          	    real yh0 = XHP(i1,i2,i3,1);
            		
          	    UHZ(i1,i2,i3) =e(xh0,yh0,0.,hz,tH);
          	    if( method==sosup )
          	    {
            	      uLocal(i1,i2,i3,hzt) =e(xh0,yh0,0.,hzt,tH);
          	    }
        	  }

        	  if ( method!=sosup )
        	  {
          	    FOR_3(i1,i2,i3,J1,J2,J3)
          	    {
            	      real xh0 = XHP(i1,i2,i3,0);
            	      real yh0 = XHP(i1,i2,i3,1);
            	      UMHZ(i1,i2,i3)=e(xh0,yh0,0.,hz,t-dt);
          	    }
        	  }


        	  }
        	  else
        	  { // ***** 3D TZ IC's ****

          	    if( solveForElectricField )
          	    {
	      // these ranges should work since we get the u*Dim* from the local raw data sizes (??!!)
            	      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
            	      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
            	      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

            	      if ( method!=sosup )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  real x0 = XEP(i1,i2,i3,0);
              		  real y0 = XEP(i1,i2,i3,1);
              		  real z0 = XEP(i1,i2,i3,2);
              		  UEX(i1,i2,i3) =e(x0,y0,z0,ex,tE);
              		  UMEX(i1,i2,i3)=e(x0,y0,z0,ex,tE-dt);

              		  UEY(i1,i2,i3) =e(x0,y0,z0,ey,tE);
              		  UMEY(i1,i2,i3)=e(x0,y0,z0,ey,tE-dt);
                  			
              		  UEZ(i1,i2,i3) =e(x0,y0,z0,ez,tE);
              		  UMEZ(i1,i2,i3)=e(x0,y0,z0,ez,tE-dt);
            		}
            	      }
            	      else
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  real x0 = XEP(i1,i2,i3,0);
              		  real y0 = XEP(i1,i2,i3,1);
              		  real z0 = XEP(i1,i2,i3,2);
                  // assign the field:
              		  UEX(i1,i2,i3) =e(x0,y0,z0,ex,tE);
              		  UEY(i1,i2,i3) =e(x0,y0,z0,ey,tE);
              		  UEZ(i1,i2,i3) =e(x0,y0,z0,ez,tE);
                  // assign time derivatives:
                                    uLocal(i1,i2,i3,ext) =e(x0,y0,z0,ext,tE);
                                    uLocal(i1,i2,i3,eyt) =e(x0,y0,z0,eyt,tE);
                                    uLocal(i1,i2,i3,ezt) =e(x0,y0,z0,ezt,tE);

            		}
            	      }
            	      if( dispersionModel != noDispersion )
            	      {
		// -- dispersion model components --
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
                                    real x0 = XEP(i1,i2,i3,0);
              		  real y0 = XEP(i1,i2,i3,1);
              		  real z0 = XEP(i1,i2,i3,2);
              		  if( pxc>=0 )
              		  {
                		    uLocal(i1,i2,i3,pxc) =e(x0,y0,z0,pxc,tE);
                		    uLocal(i1,i2,i3,pyc) =e(x0,y0,z0,pyc,tE);
                		    uLocal(i1,i2,i3,pzc) =e(x0,y0,z0,pzc,tE);
                		    if ( method!=sosup )
                		    {
                  		      umLocal(i1,i2,i3,pxc) =e(x0,y0,z0,pxc,tE-dt);
                  		      umLocal(i1,i2,i3,pyc) =e(x0,y0,z0,pyc,tE-dt);
                  		      umLocal(i1,i2,i3,pzc) =e(x0,y0,z0,pzc,tE-dt);
                		    }
                		    
              		  }
              		  if( qxc>=0 )
              		  {
                		    uLocal(i1,i2,i3,qxc) =e(x0,y0,z0,qxc,tE);
                		    uLocal(i1,i2,i3,qyc) =e(x0,y0,z0,qyc,tE);
                		    uLocal(i1,i2,i3,qzc) =e(x0,y0,z0,qzc,tE);
                		    if ( method!=sosup )
                		    {
                  		      umLocal(i1,i2,i3,qxc) =e(x0,y0,z0,qxc,tE-dt);
                  		      umLocal(i1,i2,i3,qyc) =e(x0,y0,z0,qyc,tE-dt);
                  		      umLocal(i1,i2,i3,qzc) =e(x0,y0,z0,qzc,tE-dt);
                		    }
              		  }
              		  if( rxc>=0 )
              		  {
                		    uLocal(i1,i2,i3,rxc) =e(x0,y0,z0,rxc,tE);
                		    uLocal(i1,i2,i3,ryc) =e(x0,y0,z0,ryc,tE);
                		    uLocal(i1,i2,i3,rzc) =e(x0,y0,z0,rzc,tE);
                		    if ( method!=sosup )
                		    {
                  		      umLocal(i1,i2,i3,rxc) =e(x0,y0,z0,rxc,tE-dt);
                  		      umLocal(i1,i2,i3,ryc) =e(x0,y0,z0,ryc,tE-dt);
                  		      umLocal(i1,i2,i3,rzc) =e(x0,y0,z0,rzc,tE-dt);
                		    }
              		  }
            		}
            		

            	      }
            	      

            	      
          	    }
            		
          	    if ( solveForMagneticField )
          	    {
            	      J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
            	      J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
            	      J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		real x0 = XHP(i1,i2,i3,0);
            		real y0 = XHP(i1,i2,i3,1);
            		real z0 = XHP(i1,i2,i3,2);
            		UHX(i1,i2,i3) =e(x0,y0,z0,hx,tH);
            		UMHX(i1,i2,i3)=e(x0,y0,z0,hx,tH-dt);
            		UHY(i1,i2,i3) =e(x0,y0,z0,hy,tH);
            		UMHY(i1,i2,i3)=e(x0,y0,z0,hy,tH-dt);
            		UHZ(i1,i2,i3) =e(x0,y0,z0,hz,tH);
            		UMHZ(i1,i2,i3)=e(x0,y0,z0,hz,tH-dt);
            	      }
          	    }
        	  
        	  }
            	      
            
      	
        	  if( saveExtraForcingLevels )
        	  {
	    // we need to save the "RHS" at some previous times.
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
            	      OV_ABORT("finish me for parallel");
#else
            	      realSerialArray & fnLocal = FN(m);
#endif

            	      FN(m)(I1,I2,I3,C)=csq*e.laplacian(mg,I1,I2,I3,C,t-dt*(m+1));
            	      getForcing( current, grid,FN(m),t-dt*(m+1),dt );
          	    }
        	  }
      	} // end if forcing option == twilightzone
      	else if( initialConditionOption==planeWaveInitialCondition )
      	{
	  // ::display(initialConditionBoundingBox,"initialConditionBoundingBox");

        	  printF(" *** planeWaveInitialCondition: t=%9.3e dt=%9.3e eps=%9.3e mu=%9.3e c=%9.3e ***\n",t,dt,eps,mu,c);
      	
        	  J1 = Range(max(Ie1.getBase(),ue.getBase(0)),min(Ie1.getBound(),ue.getBound(0)));
        	  J2 = Range(max(Ie2.getBase(),ue.getBase(1)),min(Ie2.getBound(),ue.getBound(1)));
        	  J3 = Range(max(Ie3.getBase(),ue.getBase(2)),min(Ie3.getBound(),ue.getBound(2)));

        	  if( numberOfDimensions==2 )
        	  {
          	    if( initialConditionBoundingBox(1,0) < initialConditionBoundingBox(0,0) )
          	    {
            	      if( method==nfdtd  ) 
            	      {
            		if( dispersionModel == noDispersion )
            		{
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
		    //real x = XEP(i1,i2,i3,0);
		    //real y = XEP(i1,i2,i3,1);
                		    real x,y;
                                  if( isRectangular )
                                  {
                                      x = X0(i1,i2,i3);
                                      y = X1(i1,i2,i3);
                                  }
                                  else
                                  {
                                      x = xe(i1,i2,i3);
                                      y = ye(i1,i2,i3);
                                  }
              		  
                		    UMEX(i1,i2,i3)=exTrue(x,y,tE-dt);
                		    UMEY(i1,i2,i3)=eyTrue(x,y,tE-dt);
                		    UMHZ(i1,i2,i3)=hzTrue(x,y,tH-dt);
            		
                		    UEX(i1,i2,i3)=exTrue(x,y,tE);
                		    UEY(i1,i2,i3)=eyTrue(x,y,tE);
                		    UHZ(i1,i2,i3)=hzTrue(x,y,tH);

              		  }
            		}
            		else
            		{
		  // --- dispersive plane wave ---
		  // Dispersive material parameters
              		  DispersiveMaterialParameters & dmp = getDispersiveMaterialParameters(grid);

                  // evaluate the dispersion relation,  exp(i(k*x-omega*t))
                  //    omega is complex 
                                    const real kk = twoPi*sqrt( kx*kx+ky*ky+kz*kz);
              		  real omegaDpwRe, omegaDpwIm;
              		  dmp.computeDispersivePlaneWaveParameters( c,eps,mu,kk, omegaDpwRe, omegaDpwIm );

              		  printF("++++ IC: dispersion relation: omegar=%g, omegai=%g\n",omegaDpwRe, omegaDpwIm );

              		  const real dpwExp =exp(omegaDpwIm*tE);
              		  const real dpwExpm=exp(omegaDpwIm*(tE-dt));
                		    
                                    OV_ABORT("Finish me -- add eval of Px and Py");
              		  
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
                		    real x,y;
                                  if( isRectangular )
                                  {
                                      x = X0(i1,i2,i3);
                                      y = X1(i1,i2,i3);
                                  }
                                  else
                                  {
                                      x = xe(i1,i2,i3);
                                      y = ye(i1,i2,i3);
                                  }
              		  
                		    UMEX(i1,i2,i3)=exDpw(x,y,tE-dt,dpwExpm);
                		    UMEY(i1,i2,i3)=eyDpw(x,y,tE-dt,dpwExpm);
                		    UMHZ(i1,i2,i3)=hzDpw(x,y,tH-dt,dpwExpm);
            		
                		    UEX(i1,i2,i3)=exDpw(x,y,tE,dpwExp);
                		    UEY(i1,i2,i3)=eyDpw(x,y,tE,dpwExp);
                		    UHZ(i1,i2,i3)=hzDpw(x,y,tH,dpwExp);
              		  }
            		} // end if noDispersion
            		
            	      }
            	      else if( method==sosup )
            	      {
                // assign both the field and it's time derivative
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  real x,y;
                              if( isRectangular )
                              {
                                  x = X0(i1,i2,i3);
                                  y = X1(i1,i2,i3);
                              }
                              else
                              {
                                  x = xe(i1,i2,i3);
                                  y = ye(i1,i2,i3);
                              }

                  // FOR NOW set old time: 
              		  UMEX(i1,i2,i3)=exTrue(x,y,tE-dt);
              		  UMEY(i1,i2,i3)=eyTrue(x,y,tE-dt);
              		  UMHZ(i1,i2,i3)=hzTrue(x,y,tH-dt);

              		  uLocal(i1,i2,i3,ex )=exTrue(x,y,tE);
              		  uLocal(i1,i2,i3,ey )=eyTrue(x,y,tE);
              		  uLocal(i1,i2,i3,hz )=hzTrue(x,y,tH);

              		  uLocal(i1,i2,i3,ext)=extTrue(x,y,tE);
              		  uLocal(i1,i2,i3,eyt)=eytTrue(x,y,tE);
              		  uLocal(i1,i2,i3,hzt)=hztTrue(x,y,tH);
            		}
            	      }
            	      else
            	      {
            		if( method==nfdtd || method==sosup ) // should be (numberOfTimeLevels>2) ??!!
            		{
              		  ume(Ie1,Ie2,Ie3,ex)=exTrue(xe,ye,t-dt);
              		  ume(Ie1,Ie2,Ie3,ey)=eyTrue(xe,ye,t-dt);
              		  umh(Ih1,Ih2,Ih3,hz)=hzTrue(xh,yh,t-dt);
            		}

            		ue(Ie1,Ie2,Ie3,ex)=exTrue(xe,ye,tE);
            		ue(Ie1,Ie2,Ie3,ey)=eyTrue(xe,ye,tE);
            		uh(Ih1,Ih2,Ih3,hz)=hzTrue(xh,yh,tH);
            	      }
          	    
          	    }
          	    else
          	    { 
              // ------------------------------------------------------------------
              // ----------- initial conditions with a BOUNDING BOX ---------------
              // ------------------------------------------------------------------

              // limit the plane wave initial condition to lie inside a bounding box: 
            	      assert( method==nfdtd  || method==sosup );
            	      int i1,i2,i3;
            	      bool clipToBoundingBox=false;
            	      
            	      if( !clipToBoundingBox )  // *new* way 
            	      {
		// In this version we smoothly damp the plane wave along the direction of the front

            		printF("--MX-- assignIC: initialConditionBoundingBox=[%9.2e,%9.2e][%9.2e,%9.2e][%9.2e,%9.2e]\n",
                   		       initialConditionBoundingBox(0,0),initialConditionBoundingBox(1,0),
                   		       initialConditionBoundingBox(0,1),initialConditionBoundingBox(1,1),
                   		       initialConditionBoundingBox(0,2),initialConditionBoundingBox(1,2));


		// Damp the initial conditions along one face of the bounding box: (*wdh* July 2, 2016)
            		const int & side = dbase.get<int>("boundingBoxDecaySide");
            		const int & axis = dbase.get<int>("boundingBoxDecayAxis");

                                
                                real nv[2]={0.,0.};  // normal to decay direction
            		nv[axis]=2*side-1;

		// Damp near the point xv0[] on the front
                                real xv0[2]={0.,0.};  // normal to decay direction
            		xv0[0] = .5*(initialConditionBoundingBox(1,0)+initialConditionBoundingBox(0,0));
            		xv0[1] = .5*(initialConditionBoundingBox(1,1)+initialConditionBoundingBox(0,1));
                                xv0[axis]=initialConditionBoundingBox(side,axis);

            		real beta=boundingBoxDecayExponent/twoPi;

		// // do this for now : 
		// real kNorm = sqrt( kx*kx+ky*ky );
		// // real beta=10./(twoPi*kNorm); // ** fix me ***
		// // *wdh* 111129 real beta=2./twoPi; // ** fix me ***

		// real beta=boundingBoxDecayExponent/twoPi;
		// real x0 = kx>=0 ? initialConditionBoundingBox(1,0) : initialConditionBoundingBox(0,0);
		// real y0 = ky>=0 ? initialConditionBoundingBox(1,1) : initialConditionBoundingBox(0,1);  


            		FOR_3D(i1,i2,i3,Ie1,Ie2,Ie3)
            		{
              		  real x,y;
                              if( isRectangular )
                              {
                                  x = X0(i1,i2,i3);
                                  y = X1(i1,i2,i3);
                              }
                              else
                              {
                                  x = xe(i1,i2,i3);
                                  y = ye(i1,i2,i3);
                              }

// NOTE: this next formula must match the one used by adjustForIncident (nrbcUtil.bf)
#define AMP2D(x,y,t) (.5*(1.-tanh(beta*twoPi*(nv[0]*((x)-xv0[0])+nv[1]*((y)-xv0[1])-cc*(t)))))

              		  real amp = AMP2D(x,y,t-dt);
              		  ume(i1,i2,i3,ex)=exTrue(x,y,t-dt)*amp;
              		  ume(i1,i2,i3,ey)=eyTrue(x,y,t-dt)*amp;
              		  umh(i1,i2,i3,hz)=hzTrue(x,y,t-dt)*amp;

              		  amp = AMP2D(x,y,t);
              		  ue(i1,i2,i3,ex)=exTrue(x,y,tE)*amp;
              		  ue(i1,i2,i3,ey)=eyTrue(x,y,tE)*amp;
              		  uh(i1,i2,i3,hz)=hzTrue(x,y,tH)*amp;

              		  if( method == sosup )
              		  {
                		    ue(i1,i2,i3,ext)=extTrue(x,y,tE)*amp;
                		    ue(i1,i2,i3,eyt)=eytTrue(x,y,tE)*amp;
                		    uh(i1,i2,i3,hzt)=hztTrue(x,y,tH)*amp;
              		  }
            		}
            	      }
            	      else
            	      {
		// old way: clip to a box 
            		FOR_3D(i1,i2,i3,Ie1,Ie2,Ie3)
            		{
              		  real x,y;
                              if( isRectangular )
                              {
                                  x = X0(i1,i2,i3);
                                  y = X1(i1,i2,i3);
                              }
                              else
                              {
                                  x = xe(i1,i2,i3);
                                  y = ye(i1,i2,i3);
                              }

              		  if( x>=initialConditionBoundingBox(0,0) && x<=initialConditionBoundingBox(1,0) &&
                  		      y>=initialConditionBoundingBox(0,1) && y<=initialConditionBoundingBox(1,1) )
              		  {

                		    ume(i1,i2,i3,ex)=exTrue(x,y,t-dt);
                		    ume(i1,i2,i3,ey)=eyTrue(x,y,t-dt);
                		    umh(i1,i2,i3,hz)=hzTrue(x,y,t-dt);

                		    ue(i1,i2,i3,ex)=exTrue(x,y,tE);
                		    ue(i1,i2,i3,ey)=eyTrue(x,y,tE);
                		    uh(i1,i2,i3,hz)=hzTrue(x,y,tH);

              		  }
            		}
            	      }
          	    
          	    }
        	  
        	  }
        	  else
        	  {  // ***** 3D ********
          	    if( solveForElectricField )
          	    {
            	      if( initialConditionBoundingBox(1,0) < initialConditionBoundingBox(0,0) )
            	      {
            		if( method==nfdtd )
            		{
		  // new way 
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
		    // real x = XEP(i1,i2,i3,0);
		    // real y = XEP(i1,i2,i3,1);
		    // real z = XEP(i1,i2,i3,2);
                		    real x,y,z;
                                  if( isRectangular )
                                  {
                                      x = X0(i1,i2,i3);
                                      y = X1(i1,i2,i3);
                                      z = X2(i1,i2,i3);
                                  }
                                  else
                                  {
                                      x = XEP(i1,i2,i3,0);
                                      y = XEP(i1,i2,i3,1);
                                      z = XEP(i1,i2,i3,2);
                                  }

                		    UMEX(i1,i2,i3)=exTrue3d(x,y,z,tE-dt);
                		    UMEY(i1,i2,i3)=eyTrue3d(x,y,z,tE-dt);
                		    UMEZ(i1,i2,i3)=ezTrue3d(x,y,z,tE-dt);
            		
                		    UEX(i1,i2,i3)=exTrue3d(x,y,z,tE);
                		    UEY(i1,i2,i3)=eyTrue3d(x,y,z,tE);
                		    UEZ(i1,i2,i3)=ezTrue3d(x,y,z,tE);
              		  }
            		}
            		else if( method==sosup )
            		{
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
		    // real x = XEP(i1,i2,i3,0);
		    // real y = XEP(i1,i2,i3,1);
		    // real z = XEP(i1,i2,i3,2);
                		    real x,y,z;
                                  if( isRectangular )
                                  {
                                      x = X0(i1,i2,i3);
                                      y = X1(i1,i2,i3);
                                      z = X2(i1,i2,i3);
                                  }
                                  else
                                  {
                                      x = XEP(i1,i2,i3,0);
                                      y = XEP(i1,i2,i3,1);
                                      z = XEP(i1,i2,i3,2);
                                  }

		    // FOR NOW set old time: 
                		    UMEX(i1,i2,i3)=exTrue3d(x,y,z,tE-dt);
                		    UMEY(i1,i2,i3)=eyTrue3d(x,y,z,tE-dt);
                		    UMEZ(i1,i2,i3)=ezTrue3d(x,y,z,tE-dt);
                		    
                		    uLocal(i1,i2,i3,ex )=exTrue3d(x,y,z,tE);
                		    uLocal(i1,i2,i3,ey )=eyTrue3d(x,y,z,tE);
                		    uLocal(i1,i2,i3,ez )=ezTrue3d(x,y,z,tE);
                		    
                		    uLocal(i1,i2,i3,ext)=extTrue3d(x,y,z,tE);
                		    uLocal(i1,i2,i3,eyt)=eytTrue3d(x,y,z,tE);
                		    uLocal(i1,i2,i3,ezt)=eztTrue3d(x,y,z,tE);
              		  }
            		}

            		else
            		{
              		  if( method==nfdtd  || method==sosup )
              		  {
                		    ume(I1,I2,I3,ex)=exTrue3d(xe,ye,ze,tE-dt);
                		    ume(I1,I2,I3,ey)=eyTrue3d(xe,ye,ze,tE-dt);
                		    ume(I1,I2,I3,ez)=ezTrue3d(xe,ye,ze,tE-dt);
              		  }

              		  ue(Ie1,Ie2,Ie3,ex)=exTrue3d(xe,ye,ze,tE);
              		  ue(Ie1,Ie2,Ie3,ey)=eyTrue3d(xe,ye,ze,tE);
              		  ue(Ie1,Ie2,Ie3,ez)=ezTrue3d(xe,ye,ze,tE);
            		}
          	    
            	      }
            	      else
            	      { // limit the plane wave initial condition to lie inside a bounding box: 
            		assert( method==nfdtd  || method==sosup );
            		assert( !solveForMagneticField );  // fix me for this case
            		int i1,i2,i3;
            		FOR_3D(i1,i2,i3,Ie1,Ie2,Ie3)
            		{
		  // real x=xe(i1,i2,i3), y=ye(i1,i2,i3), z=ze(i1,i2,i3);
              		  real x,y,z;
                              if( isRectangular )
                              {
                                  x = X0(i1,i2,i3);
                                  y = X1(i1,i2,i3);
                                  z = X2(i1,i2,i3);
                              }
                              else
                              {
                                  x = XEP(i1,i2,i3,0);
                                  y = XEP(i1,i2,i3,1);
                                  z = XEP(i1,i2,i3,2);
                              }

              		  if( x>=initialConditionBoundingBox(0,0) && x<=initialConditionBoundingBox(1,0) &&
                  		      y>=initialConditionBoundingBox(0,1) && y<=initialConditionBoundingBox(1,1) &&
                  		      z>=initialConditionBoundingBox(0,2) && z<=initialConditionBoundingBox(1,2) )
              		  {

                		    ume(i1,i2,i3,ex)=exTrue3d(x,y,z,tE-dt);
                		    ume(i1,i2,i3,ey)=eyTrue3d(x,y,z,tE-dt);
                		    ume(i1,i2,i3,ez)=ezTrue3d(x,y,z,tE-dt);

                		    ue(i1,i2,i3,ex)=exTrue3d(x,y,z,tE);
                		    ue(i1,i2,i3,ey)=eyTrue3d(x,y,z,tE);
                		    ue(i1,i2,i3,ez)=ezTrue3d(x,y,z,tE);

              		  }
            		}

            	      }
          	    }
          	    if( solveForMagneticField )
          	    {
            	      if ( method==nfdtd || method==sosup )
            	      {
            		umh(I1,I2,I3,hx)=hxTrue3d(xh,yh,zh,tH-dt);
            		umh(I1,I2,I3,hy)=hyTrue3d(xh,yh,zh,tH-dt);
            		umh(I1,I2,I3,hz)=hzTrue3d(xh,yh,zh,tH-dt);
            	      }

            	      uh(Ih1,Ih2,Ih3,hx)=hxTrue3d(xh,yh,zh,tH);
            	      uh(Ih1,Ih2,Ih3,hy)=hyTrue3d(xh,yh,zh,tH);
            	      uh(Ih1,Ih2,Ih3,hz)=hzTrue3d(xh,yh,zh,tH);
          	    }
            	      
        	  }
          	    

        	  if( saveExtraForcingLevels && timeSteppingMethod!=modifiedEquationTimeStepping )
        	  {
	    // we need to save the "RHS" at some previous times.
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
#else
            	      realSerialArray & fnLocal = FN(m);
#endif
            	      if( numberOfDimensions==2 )
            	      {
            		fnLocal(I1,I2,I3,ex)=csq*exLaplacianTrue(xe,ye,t-dt*(m+1));
            		fnLocal(I1,I2,I3,ey)=csq*eyLaplacianTrue(xe,ye,t-dt*(m+1));
            		fnLocal(I1,I2,I3,hz)=csq*hzLaplacianTrue(xh,yh,t-dt*(m+1));

            	      }
            	      else
            	      {
            		if( solveForElectricField )
            		{
              		  fnLocal(I1,I2,I3,ex)=csq*exLaplacianTrue3d(xe,ye,ze,t-dt*(m+1));
              		  fnLocal(I1,I2,I3,ey)=csq*eyLaplacianTrue3d(xe,ye,ze,t-dt*(m+1));
              		  fnLocal(I1,I2,I3,ez)=csq*ezLaplacianTrue3d(xe,ye,ze,t-dt*(m+1));
            		}
            		if( solveForMagneticField )
            		{
              		  fnLocal(I1,I2,I3,hx)=csq*hxLaplacianTrue3d(xh,yh,zh,t-dt*(m+1));
              		  fnLocal(I1,I2,I3,hy)=csq*hyLaplacianTrue3d(xh,yh,zh,t-dt*(m+1));
              		  fnLocal(I1,I2,I3,hz)=csq*hzLaplacianTrue3d(xh,yh,zh,t-dt*(m+1));
            		}
              		  
            	      }
            	      getForcing( current, grid,FN(m),t-dt*(m+1),dt );
            		
          	    }
        	  }
        	  else if( timeSteppingMethod!=modifiedEquationTimeStepping && orderOfAccuracyInTime!=2 )
        	  {
          	    OV_ABORT();
        	  }
        	  
      	}
      	else if( forcingOption==magneticSinusoidalPointSource )
      	{
        	  uh=0.;
        	  ue=0.;
        	  const IntegerArray & gid = mg.gridIndexRange();
        	  int i1=gid(0,0)+(gid(1,0)-gid(0,0))/2;
        	  int i2=gid(0,1)+(gid(1,1)-gid(0,1))/2;
        	  int i3=gid(0,2)+(gid(1,2)-gid(0,2))/2;
          	    
	  //kkc 0 for the last index takes care of faceCenteredAll H gridFunctions in 3D
        	  uh(i1,i2,i3,hz,0)=sin(twoPi*frequency*t);
      	}
      	else if( initialConditionOption==zeroInitialCondition )
      	{
        	  printF("Setting ZERO initial conditions\n");

        	  uh(Ih1,Ih2,Ih3,all,all)=0.;
        	  ue(Ie1,Ie2,Ie3,all,all)=0.;
	  //            u(I1,I2,I3,ey)=0.;

        	  if ( method==nfdtd )
        	  {
          	    umh(Ih1,Ih2,Ih3,all,all)=0.;
          	    ume(Ie1,Ie2,Ie3,all,all)=0.;
        	  }
	  //            um(I1,I2,I3,ey)=0.;

        	  if( boundaryForcingOption==planeWaveBoundaryForcing )
        	  {//kkc XXX probably not working for DSI schemes yet
          	    printF("*** Set BC's for planeWaveBoundaryForcing on initial conditions...\n");
          	    realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
          	    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];

          	    int option=0; // not used.
          	    assignBoundaryConditions( option, grid, t-dt, dt, fieldPrev, fieldPrev, prev );
          	    assignBoundaryConditions( option, grid, t   , dt, fieldCurrent, fieldCurrent, current );

        	  }
      	}
      	else if( initialConditionOption==gaussianPlaneWave )
      	{
	  // (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]
        	  printF("Setting initial condition to be a Gaussian plane wave, kx,ky,kz=%i %i %i\n",kx,ky,kz);
          	    
        	  realSerialArray xei,xhi;

        	  xei=kx*(xe-x0GaussianPlaneWave)+ky*(ye-y0GaussianPlaneWave) -cc*tE;
        	  xhi=kx*(xh-x0GaussianPlaneWave)+ky*(yh-y0GaussianPlaneWave) -cc*tH;

        	  uh(Ih1,Ih2,Ih3,hz)=hzGaussianPulse(xhi);
        	  ue(Ie1,Ie2,Ie3,ex)=exGaussianPulse(xei);//u(I1,I2,I3,hz)*(-ky/(eps*cc));
        	  ue(Ie1,Ie2,Ie3,ey)=eyGaussianPulse(xei);//u(I1,I2,I3,hz)*( kx/(eps*cc));

        	  xhi+=cc*dt;
        	  xei+=cc*dt;
        	  umh(Ih1,Ih2,Ih3,hz)=hzGaussianPulse(xhi);
        	  ume(Ie1,Ie2,Ie3,ex)=exGaussianPulse(xei);//u(I1,I2,I3,hz)*(-ky/(eps*cc));
        	  ume(Ie1,Ie2,Ie3,ey)=eyGaussianPulse(xei);//u(I1,I2,I3,hz)*( kx/(eps*cc));

        	  if( saveExtraForcingLevels )
        	  {
	    // we need to save the "RHS" at some previous times.
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
            	      OV_ABORT("finish me for parallel");
#else
            	      realSerialArray & fnLocal = FN(m);
#endif
            	      xhi=kx*(xe-x0GaussianPlaneWave)+ky*(ye-y0GaussianPlaneWave) -cc*(t-dt*(m+1));
            	      fnLocal(I1,I2,I3,hz)=hzLaplacianGaussianPulse(xhi);
            	      fnLocal(I1,I2,I3,ex)=fnLocal(I1,I2,I3,hz)*(-ky/(eps*cc));
            	      fnLocal(I1,I2,I3,ey)=fnLocal(I1,I2,I3,hz)*( kx/(eps*cc));
          	    }
        	  }
      	}
      	else if( initialConditionOption==gaussianPulseInitialCondition )
      	{
        	  ue=0.;
        	  uh=0.;
//             // (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]
        	  Index J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  Index J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  Index J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
        	  for( int pulse=0; pulse<numberOfGaussianPulses; pulse++ )
        	  {
            	      
          	    const real *gpp = gaussianPulseParameters[pulse];

          	    const real beta    = gpp[0];
          	    const real scale   = gpp[1];
          	    const real exponent= gpp[2];
          	    const real x0      = gpp[3];
          	    const real y0      = gpp[4];
          	    const real z0      = gpp[5];
          	    

          	    printF("Gaussian pulse IC's: beta=%8.2e, scale=%8.2e, exponent=%8.2e, (x0,y0,z0)=(%8.2e,%8.2e,%8.2e) \n",
               		   beta,scale,exponent,x0,y0,z0);

          	    const real c0= pulse==0 ? 0. : 1.;
          	    if( true )
          	    {
            	      int i1,i2,i3;
            	      if( isRectangular )
            	      {
                        if( mg.numberOfDimensions()==2 )
                        {
                            J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                            J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                            J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                    real xe = X0(i1,i2,i3)-x0;
                                    real ye = X1(i1,i2,i3)-y0;
                                real temp=exp( -pow(beta*(xe*xe+ye*ye),exponent) );
                                UEX(i1,i2,i3) =c0*UEX(i1,i2,i3)-ye*scale*temp;   // Ex = -C (Hz).y
                                UEY(i1,i2,i3) =c0*UEY(i1,i2,i3)+xe*scale*temp;   // Ey =  C (Hz).x
                                UMEX(i1,i2,i3)=UEX(i1,i2,i3);
                                UMEY(i1,i2,i3)=UEY(i1,i2,i3);
                            }
                            J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                            J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                            J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                            FOR_3(i1,i2,i3,J1,J2,J3)
                            {
                                    real xh = X0(i1,i2,i3)-x0;
                                    real yh = X1(i1,i2,i3)-y0;
                                real temp=exp( -pow(beta*(xh*xh+yh*yh),exponent) );
                                UHZ(i1,i2,i3) =c0*UHZ(i1,i2,i3) + exp( -pow(beta*(xh*xh+yh*yh),exponent) ); 
                                UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                        }
                        else
                        {
                            J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                            J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                            J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                    real xe = X0(i1,i2,i3)-x0;
                                    real ye = X1(i1,i2,i3)-y0;
                                    real ze = X2(i1,i2,i3)-z0;
                  // XXX NONE of the 3D part of this macro has really been implemented for staggered grids
                                real rsq = xe*xe+ye*ye+ze*ze;
                // E = curl( phi ), phi = (phix,phiy,phiz)
                // real phix = constant*exp( -pow(beta*rsq,exponent) );
                                real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
                                real dphiy = dphix;
                                real dphiz = dphix; 
                                UEX(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ye*dphiz -ze*dphiy;        //  (phiz).y - (phiy).z
                                UEY(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ze*dphix -xe*dphiz ;        // 
                                UEZ(i1,i2,i3)=c0*UEX(i1,i2,i3)+  xe*dphiy -ye*dphix ;
                                UMEX(i1,i2,i3)=UEX(i1,i2,i3);
                                UMEY(i1,i2,i3)=UEY(i1,i2,i3);
                                UMEZ(i1,i2,i3)=UEZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                            J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                            J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                            J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                            FOR_3(i1,i2,i3,J1,J2,J3)
                            {
                                    real xe = X0(i1,i2,i3)-x0;
                                    real ye = X1(i1,i2,i3)-y0;
                                    real ze = X2(i1,i2,i3)-z0;
                                    real xh = xe;// Cartesian grid stuff not implemented for staggered grids yet (Yee, DSI)
                                    real yh = ye;
                                    real zh = ze;
                  // XXX NONE of the 3D part of this macro has really been properly implemented for staggered grids
                                real rsq = xh*xh+yh*yh+zh*zh;
                // E = curl( phi ), phi = (phix,phiy,phiz)
                // real phix = constant*exp( -pow(beta*rsq,exponent) );
                                real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
                                real dphiy = dphix;
                                real dphiz = dphix; 
                                UHX(i1,i2,i3)=c0*UHX(i1,i2,i3) +  dphix;
                                UHY(i1,i2,i3)=c0*UHY(i1,i2,i3) +  dphiy;
                                UHZ(i1,i2,i3)=c0*UHZ(i1,i2,i3) +  dphiz;
                                UMHX(i1,i2,i3)=UHX(i1,i2,i3);
                                UMHY(i1,i2,i3)=UHY(i1,i2,i3);
                                UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                        }
            	      }
            	      else
            	      {
                        if( mg.numberOfDimensions()==2 )
                        {
                            J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                            J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                            J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                    real xe = XEP(i1,i2,i3,0)-x0;
                                    real ye = XEP(i1,i2,i3,1)-y0;
                                real temp=exp( -pow(beta*(xe*xe+ye*ye),exponent) );
                                UEX(i1,i2,i3) =c0*UEX(i1,i2,i3)-ye*scale*temp;   // Ex = -C (Hz).y
                                UEY(i1,i2,i3) =c0*UEY(i1,i2,i3)+xe*scale*temp;   // Ey =  C (Hz).x
                                UMEX(i1,i2,i3)=UEX(i1,i2,i3);
                                UMEY(i1,i2,i3)=UEY(i1,i2,i3);
                            }
                            J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                            J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                            J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                            FOR_3(i1,i2,i3,J1,J2,J3)
                            {
                                    real xh = XHP(i1,i2,i3,0)-x0;
                                    real yh = XHP(i1,i2,i3,1)-y0;
                                real temp=exp( -pow(beta*(xh*xh+yh*yh),exponent) );
                                UHZ(i1,i2,i3) =c0*UHZ(i1,i2,i3) + exp( -pow(beta*(xh*xh+yh*yh),exponent) ); 
                                UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                        }
                        else
                        {
                            J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
                            J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
                            J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                    real xe = XEP(i1,i2,i3,0)-x0;
                                    real ye = XEP(i1,i2,i3,1)-y0;
                                    real ze = XEP(i1,i2,i3,2)-z0;
                  // XXX NONE of the 3D part of this macro has really been implemented for staggered grids
                                real rsq = xe*xe+ye*ye+ze*ze;
                // E = curl( phi ), phi = (phix,phiy,phiz)
                // real phix = constant*exp( -pow(beta*rsq,exponent) );
                                real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
                                real dphiy = dphix;
                                real dphiz = dphix; 
                                UEX(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ye*dphiz -ze*dphiy;        //  (phiz).y - (phiy).z
                                UEY(i1,i2,i3)=c0*UEX(i1,i2,i3)+  ze*dphix -xe*dphiz ;        // 
                                UEZ(i1,i2,i3)=c0*UEX(i1,i2,i3)+  xe*dphiy -ye*dphix ;
                                UMEX(i1,i2,i3)=UEX(i1,i2,i3);
                                UMEY(i1,i2,i3)=UEY(i1,i2,i3);
                                UMEZ(i1,i2,i3)=UEZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                            J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
                            J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
                            J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
                            FOR_3(i1,i2,i3,J1,J2,J3)
                            {
                                    real xh = XHP(i1,i2,i3,0)-x0;
                                    real yh = XHP(i1,i2,i3,1)-y0;
                                    real zh = XHP(i1,i2,i3,2)-z0;
                  // XXX NONE of the 3D part of this macro has really been properly implemented for staggered grids
                                real rsq = xh*xh+yh*yh+zh*zh;
                // E = curl( phi ), phi = (phix,phiy,phiz)
                // real phix = constant*exp( -pow(beta*rsq,exponent) );
                                real dphix = scale*exp( -pow(beta*rsq,exponent) ); // exponent*pow(beta*rsq,exponent-1.);
                                real dphiy = dphix;
                                real dphiz = dphix; 
                                UHX(i1,i2,i3)=c0*UHX(i1,i2,i3) +  dphix;
                                UHY(i1,i2,i3)=c0*UHY(i1,i2,i3) +  dphiy;
                                UHZ(i1,i2,i3)=c0*UHZ(i1,i2,i3) +  dphiz;
                                UMHX(i1,i2,i3)=UHX(i1,i2,i3);
                                UMHY(i1,i2,i3)=UHY(i1,i2,i3);
                                UMHZ(i1,i2,i3)=UHZ(i1,i2,i3);   // This is wrong : should use u_t = w_y ...
                            }
                        }
            	      }
          	    }
        	  }
          	    
        	  if( saveExtraForcingLevels )
        	  {
	    // we need to save the "RHS" at some previous times.
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
            	      OV_ABORT("finish me for parallel");
#else
            	      realSerialArray & fnLocal = FN(m);
#endif
            	      fnLocal(I1,I2,I3,hz)=0.;
            	      fnLocal(I1,I2,I3,ex)=0.;
            	      fnLocal(I1,I2,I3,ey)=0.;
          	    }
        	  }
      	}
      	else if( initialConditionOption==squareEigenfunctionInitialCondition )
      	{
          // --------------------------------------------------
          // --------- Square or Box Eigenfunction ------------
          // --------------------------------------------------

        	  real fx=Pi*initialConditionParameters[0];
        	  real fy=Pi*initialConditionParameters[1];
        	  real fz=Pi*initialConditionParameters[2];
        	  real x0=initialConditionParameters[3];
        	  real y0=initialConditionParameters[4];
        	  real z0=initialConditionParameters[5];
        	  real omega;
        	  real a1=1., a2=-2., a3=1.;  // For 3d, divergence free if a1+a2+a3=0
        	  if( numberOfDimensions==2 )
        	  {
          	    omega=c*sqrt(fx*fx+fy*fy);
	    // x0=-.5, y0=-.5;   // for the square [-.5,.5]x[-.5,.5] 
        	  }
        	  else
        	  {
          	    omega=c*sqrt(fx*fx+fy*fy+fz*fz);
          	    printF(" box eigenfunction initial condition: fx=%g Pi, fy=%g Pi fz=%g Pi omega=%g Pi.\n",
               		   fx/Pi, fy/Pi, fz/Pi, omega/Pi);
        	  }
          	    
        	  Index J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  Index J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  Index J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
        	  int i1,i2,i3;
        	  real xd,yd,zd;

        	  if( isRectangular )
        	  {

          	    if( numberOfDimensions==2 )
          	    {
            	      Index J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
            	      Index J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
            	      Index J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		real xdh=X0(i1,i2,i3)-x0;
            		real ydh=X1(i1,i2,i3)-y0;
            		UHZ(i1,i2,i3) =cos(fx*xdh)*cos(fy*ydh)*cos(omega*tH);
            	      }

            	      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
            	      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
            	      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

            	      FOR_3(i1,i2,i3,J1,J2,J3)
            	      {
            		real xde=X0(i1,i2,i3)-x0;
            		real yde=X1(i1,i2,i3)-y0;
            		UEX(i1,i2,i3) =  (-fy/omega)*cos(fx*xde)*sin(fy*yde)*sin(omega*tE);  // Ex.t = Hz.y
            		UEY(i1,i2,i3) =  ( fx/omega)*sin(fx*xde)*cos(fy*yde)*sin(omega*tE);  // Ey.t = - Hz.x
            	      }

            	      if( method==nfdtd )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  real xdh=X0(i1,i2,i3)-x0;
              		  real ydh=X1(i1,i2,i3)-y0;
              		  UMHZ(i1,i2,i3)=cos(fx*xdh)*cos(fy*ydh)*cos(omega*(t-dt));
            		}
            		FOR_3(i1,i2,i3,J1,J2,J3)
            		{
              		  real xde=X0(i1,i2,i3)-x0;
              		  real yde=X1(i1,i2,i3)-y0;
              		  UMEX(i1,i2,i3) =  (-fy/omega)*cos(fx*xde)*sin(fy*yde)*sin(omega*(t-dt));  // Ex.t = Hz.y
              		  UMEY(i1,i2,i3) =  ( fx/omega)*sin(fx*xde)*cos(fy*yde)*sin(omega*(t-dt));  // Ey.t = - Hz.x
            		}
            	      }
                            if( method==sosup )
            	      {

            		FOR_3(i1,i2,i3,J1,J2,J3)
            		{
              		  real xde=X0(i1,i2,i3)-x0;
              		  real yde=X1(i1,i2,i3)-y0;
		  // time derivatives: 
              		  uLocal(i1,i2,i3,ext) = (-fy)*cos(fx*xde)*sin(fy*yde)*cos(omega*tE);  // Ex.t
              		  uLocal(i1,i2,i3,eyt) = ( fx)*sin(fx*xde)*cos(fy*yde)*cos(omega*tE);  // Ey.t

              		  uLocal(i1,i2,i3,hzt) = (-omega)*cos(fx*xde)*cos(fy*yde)*sin(omega*tH);  // Hz.t 
            		}
            		
            	      }
            	      
          	    } 
          	    else // 3D
          	    {

            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		real xde=X0(i1,i2,i3)-x0;
            		real yde=X1(i1,i2,i3)-y0;
            		real zde=X2(i1,i2,i3)-z0;

            		UEX(i1,i2,i3) =  (a1/fx)*cos(fx*xde)*sin(fy*yde)*sin(fz*zde)*cos(omega*t);  // 
            		UEY(i1,i2,i3) =  (a2/fy)*sin(fx*xde)*cos(fy*yde)*sin(fz*zde)*cos(omega*t);  // 
            		UEZ(i1,i2,i3) =  (a3/fz)*sin(fx*xde)*sin(fy*yde)*cos(fz*zde)*cos(omega*t);  // 

            		UMEX(i1,i2,i3) =  (a1/fx)*cos(fx*xde)*sin(fy*yde)*sin(fz*zde)*cos(omega*(t-dt));  // 
            		UMEY(i1,i2,i3) =  (a2/fy)*sin(fx*xde)*cos(fy*yde)*sin(fz*zde)*cos(omega*(t-dt));  // 
            		UMEZ(i1,i2,i3) =  (a3/fz)*sin(fx*xde)*sin(fy*yde)*cos(fz*zde)*cos(omega*(t-dt));  // 
            	      }
            	      if( method==sosup )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  real xde=X0(i1,i2,i3)-x0;
              		  real yde=X1(i1,i2,i3)-y0;
              		  real zde=X2(i1,i2,i3)-z0;

                  // time derivatives: 
              		  uLocal(i1,i2,i3,ext) =  (-omega*a1/fx)*cos(fx*xde)*sin(fy*yde)*sin(fz*zde)*sin(omega*t);  
              		  uLocal(i1,i2,i3,eyt) =  (-omega*a2/fy)*sin(fx*xde)*cos(fy*yde)*sin(fz*zde)*sin(omega*t);  
              		  uLocal(i1,i2,i3,ezt) =  (-omega*a3/fz)*sin(fx*xde)*sin(fy*yde)*cos(fz*zde)*sin(omega*t);  
            		}
            	      }


          	    }
        	  }
        	  else 
        	  {
	    // --- curvilinear ---

          	    if( numberOfDimensions==2 )
          	    {
            	      Index J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
            	      Index J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
            	      Index J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));

            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		xd=XHP(i1,i2,i3,0)-x0;
            		yd=XHP(i1,i2,i3,1)-y0;
            		UHZ(i1,i2,i3) =cos(fx*xd)*cos(fy*yd)*cos(omega*tH);
            	      }

            	      J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
            	      J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
            	      J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
            	      FOR_3(i1,i2,i3,J1,J2,J3)
            	      {
            		xd=XEP(i1,i2,i3,0)-x0;
            		yd=XEP(i1,i2,i3,1)-y0;
            		
            		UEX(i1,i2,i3) =  (-fy/omega)*cos(fx*xd)*sin(fy*yd)*sin(omega*tE);  // Ex.t = Hz.y
            		UEY(i1,i2,i3) =  ( fx/omega)*sin(fx*xd)*cos(fy*yd)*sin(omega*tE);  // Ey.t = - Hz.x
            	      }            

            	      if ( method==nfdtd )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  xd=XHP(i1,i2,i3,0)-x0;
              		  yd=XHP(i1,i2,i3,1)-y0;
              		  UMHZ(i1,i2,i3)=cos(fx*xd)*cos(fy*yd)*cos(omega*(t-dt));
            		}
            		FOR_3(i1,i2,i3,J1,J2,J3)
            		{
              		  xd=XEP(i1,i2,i3,0)-x0;
              		  yd=XEP(i1,i2,i3,1)-y0;
                  			
              		  UMEX(i1,i2,i3) =  (-fy/omega)*cos(fx*xd)*sin(fy*yd)*sin(omega*(t-dt));  // Ex.t = Hz.y
              		  UMEY(i1,i2,i3) =  ( fx/omega)*sin(fx*xd)*cos(fy*yd)*sin(omega*(t-dt));  // Ey.t = - Hz.x
            		}
            	      }

                            if( method==sosup )
            	      {
                // --- assign time derivatives ---
            		FOR_3(i1,i2,i3,J1,J2,J3)
            		{
              		  xd=XEP(i1,i2,i3,0)-x0;
              		  yd=XEP(i1,i2,i3,1)-y0;

		  // time derivatives: 
              		  uLocal(i1,i2,i3,ext) = (-fy)*cos(fx*xd)*sin(fy*yd)*cos(omega*tE);  // Ex.t
              		  uLocal(i1,i2,i3,eyt) = ( fx)*sin(fx*xd)*cos(fy*yd)*cos(omega*tE);  // Ey.t

              		  uLocal(i1,i2,i3,hzt) = (-omega)*cos(fx*xd)*cos(fy*yd)*sin(omega*tH);  // Hz.t 
            		}
            	      }
          	    } 
          	    else // 3D
          	    {

            	      FOR_3D(i1,i2,i3,J1,J2,J3)
            	      {
            		xd=XEP(i1,i2,i3,0)-x0;
            		yd=XEP(i1,i2,i3,1)-y0;
            		zd=XEP(i1,i2,i3,2)-z0;

            		UEX(i1,i2,i3) =  (a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*cos(omega*t);  // 
            		UEY(i1,i2,i3) =  (a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*cos(omega*t);  // 
            		UEZ(i1,i2,i3) =  (a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*cos(omega*t);  // 

            		if( method==nfdtd )
            		{
              		  UMEX(i1,i2,i3) =  (a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*cos(omega*(t-dt));  // 
              		  UMEY(i1,i2,i3) =  (a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*cos(omega*(t-dt));  // 
              		  UMEZ(i1,i2,i3) =  (a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*cos(omega*(t-dt));  // 
            		}
            		else if( method==sosup )
            		{
		  // time derivatives: 
              		  uLocal(i1,i2,i3,ext) =  (-omega*a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*sin(omega*t);  
              		  uLocal(i1,i2,i3,eyt) =  (-omega*a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*sin(omega*t);  
              		  uLocal(i1,i2,i3,ezt) =  (-omega*a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*sin(omega*t);  
            		}
            	      }
          	    }
            	      
        	  }
        	  if( saveExtraForcingLevels )
        	  { // not implemented
          	    OV_ABORT("error: not implemented");
        	  }

      	}
      	else if( initialConditionOption==annulusEigenfunctionInitialCondition )
      	{
              
        	  Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
        	  Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
        	  Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));
    
	  // This is a macro:
        //   printF(" I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0)=%i %i %i %i \n",
        // 	 I1.getBase(),uLocal.getBase(0),I1.getBound(),uLocal.getBound(0));
        //  Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
        //  Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
        //  Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));
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
                      FOR_3D(i1,i2,i3,J1,J2,J3)
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
                              UHZ(i1,i2,i3)  = bj*cosn*cost;
                              UEX(i1,i2,i3) = uex*sint;  // Ex.t = Hz.y
                              UEY(i1,i2,i3) = uey*sint;  // Ey.t = - Hz.x
                              if( method==nfdtd )
                              {
                                  UMHZ(i1,i2,i3) = bj*cosn*cos(omega*(t-dt));
                                  UMEX(i1,i2,i3) = uex*sin(omega*(t-dt)); 
                                  UMEY(i1,i2,i3) = uey*sin(omega*(t-dt)); 
                              }
                              else if( method==sosup )
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
                      FOR_3D(i1,i2,i3,J1,J2,J3)
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
                              UEX(i1,i2,i3) = uex*sinkz*cost;
                              UEY(i1,i2,i3) = uey*sinkz*cost;
                              UEZ(i1,i2,i3) = bj*cosn*coskz*cost;
                              cost=cos(omega*(t-dt)); 
                              UMEX(i1,i2,i3) = uex*sinkz*cost;
                              UMEY(i1,i2,i3) = uey*sinkz*cost;
                              UMEZ(i1,i2,i3) = bj*cosn*coskz*cost;
                              if( method==sosup )
                              {
                                  sint=sin(omega*t); 
                                  uLocal(i1,i2,i3,ext) = -omega*uex*sinkz*sint;
                                  uLocal(i1,i2,i3,eyt) = -omega*uey*sinkz*sint;
                                  uLocal(i1,i2,i3,ezt) = -omega*bj*cosn*coskz*sint;
                              }
                    }
                  }
          	    
          	    if( saveExtraForcingLevels )
          	    { // not implemented
            	      OV_ABORT("error: not implemented");
          	    }
          	    

      	}
      	else if( initialConditionOption==planeWaveScatteredFieldInitialCondition )
      	{
        	  printF("Setting initial conditions to be planeWaveScatterFieldInitialCondition. dt=%14.6e\n",dt);
        	  if( debug & 2 ) fPrintF(debugFile,
                          				  "Setting initial conditions to be planeWaveScatterFieldInitialCondition. dt=%14.6e\n",dt);

        	  if ( method!=nfdtd && method!=sosup )
          	    OV_ABORT("planeWaveScatteredFieldInitialCondition: unexpected method");

        	  const realArray & ug = (*knownSolution)[grid];

        	  const real cc0= cGrid(0)*sqrt( kx*kx+ky*ky ); // NOTE: use grid 0 values for multi-materials

	  // The analytic solution assumed incident field was Ei = exp(i*k*x-i*w*t) 
	  //     This gives solution
	  //           Re(E)*cos(w*t) - Im(E)*sin(w*t) for Ei=cos(w*t)
	  //      or   Re(E)*cos(w*t-pi/2) - Im(E)*sin(w*t-pi/2) for Ei=cos(w*t-pi/2)               
	  //      i.e. Re(E)*sin(w*t) + Im(E)*cos(w*t) for Ei=sin(w*t)
	  // Ex:
        	  const real cost = cos(-twoPi*cc0*t); // *wdh* 040626 add "-"
        	  const real sint = sin(-twoPi*cc0*t); // *wdh* 040626 add "-"

        	  const real costm= cos(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"
        	  const real sintm= sin(-twoPi*cc0*(t-dt)); // *wdh* 040626 add "-"

        	  const real dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
        	  const real dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 

	  //kkc XXX this only works for nfdtd right now (knownSolution will need to change a bit
	  //                                             to handle staggered grids)
          	    
        	  Range CE = numberOfDimensions==2 ? Range(ex,ey) : Range(ex,ez);
        	  Range CH = numberOfDimensions==2 ? Range(hz,hz) : Range(hx,hz);


#ifdef USE_PPP
        	  realSerialArray ugLocal; getLocalArrayWithGhostBoundaries(ug,ugLocal);
#else
        	  const realSerialArray & ugLocal = ug; 
#endif
        	  if( method==nfdtd || method==sosup )
        	  { // do this with scalar indexing to avoid a possible bug in P++
          	    real *ugp = ugLocal.Array_Descriptor.Array_View_Pointer3;
          	    const int ugDim0=ugLocal.getRawDataSize(0);
          	    const int ugDim1=ugLocal.getRawDataSize(1);
          	    const int ugDim2=ugLocal.getRawDataSize(2);
#undef UG
#define UG(i0,i1,i2,i3) ugp[i0+ugDim0*(i1+ugDim1*(i2+ugDim2*(i3)))]

	    // adjust array dimensions for local arrays
          	    Index J1 = Range(max(I1.getBase(),uel.getBase(0)),min(I1.getBound(),uel.getBound(0)));
          	    Index J2 = Range(max(I2.getBase(),uel.getBase(1)),min(I2.getBound(),uel.getBound(1)));
          	    Index J3 = Range(max(I3.getBase(),uel.getBase(2)),min(I3.getBound(),uel.getBound(2)));
          	    int i1,i2,i3;
          	    if( numberOfDimensions==2 )
          	    {
            	      if( method==nfdtd )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  UEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
              		  UEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
              		  UHZ(i1,i2,i3)= UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost;

              		  UMEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sintm+UG(i1,i2,i3,ex+3)*costm;
              		  UMEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sintm+UG(i1,i2,i3,ey+3)*costm;
              		  UMHZ(i1,i2,i3)= UG(i1,i2,i3,hz)*sintm+UG(i1,i2,i3,hz+3)*costm;
            		}
            	      }
            	      else if( method==sosup )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  UEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
              		  UEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
              		  UHZ(i1,i2,i3)= UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost;

		  // -- time derivatives: 
              		  uLocal(i1,i2,i3,ext)= UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost;
              		  uLocal(i1,i2,i3,eyt)= UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost;
              		  uLocal(i1,i2,i3,hzt)= UG(i1,i2,i3,hz)*dsint+UG(i1,i2,i3,hz+3)*dcost;
            		}
            	      }
            	      else
            	      {
            		OV_ABORT("planeWaveScatteredFieldInitialCondition: ERROR: unknown method");
            	      }
            	      
            	      
          	    }
          	    else 
          	    {
            	      if( solveForElectricField )
            	      {
            		if( method==nfdtd )
            		{
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
                		    UEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
                		    UEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
                		    UEZ(i1,i2,i3)= UG(i1,i2,i3,ez)*sint+UG(i1,i2,i3,ez+3)*cost;

                		    UMEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sintm+UG(i1,i2,i3,ex+3)*costm;
                		    UMEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sintm+UG(i1,i2,i3,ey+3)*costm;
                		    UMEZ(i1,i2,i3)= UG(i1,i2,i3,ez)*sintm+UG(i1,i2,i3,ez+3)*costm;
              		  }
            		}
            		else if( method==sosup )
            		{
              		  FOR_3D(i1,i2,i3,J1,J2,J3)
              		  {
                		    UEX(i1,i2,i3)= UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost;
                		    UEY(i1,i2,i3)= UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost;
                		    UEZ(i1,i2,i3)= UG(i1,i2,i3,ez)*sint+UG(i1,i2,i3,ez+3)*cost;

		    // -- time derivatives: 
                		    uLocal(i1,i2,i3,ext)= UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost;
                		    uLocal(i1,i2,i3,eyt)= UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost;
                		    uLocal(i1,i2,i3,ezt)= UG(i1,i2,i3,ez)*dsint+UG(i1,i2,i3,ez+3)*dcost;
              		  }


            		}
            		else
            		{
              		  OV_ABORT("planeWaveScatteredFieldInitialCondition: ERROR: unknown method");
            		}

            	      }
            	      if( solveForMagneticField )
            	      {
            		FOR_3D(i1,i2,i3,J1,J2,J3)
            		{
              		  UHX(i1,i2,i3)= UG(i1,i2,i3,hx)*sint+UG(i1,i2,i3,hx+3)*cost;
              		  UHY(i1,i2,i3)= UG(i1,i2,i3,hy)*sint+UG(i1,i2,i3,hy+3)*cost;
              		  UHZ(i1,i2,i3)= UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost;

              		  UMHX(i1,i2,i3)= UG(i1,i2,i3,hx)*sintm+UG(i1,i2,i3,hx+3)*costm;
              		  UMHY(i1,i2,i3)= UG(i1,i2,i3,hy)*sintm+UG(i1,i2,i3,hy+3)*costm;
              		  UMHZ(i1,i2,i3)= UG(i1,i2,i3,hz)*sintm+UG(i1,i2,i3,hz+3)*costm;
            		}
            	      }
          	    }
#undef UG

        	  }
        	  else
        	  {
          	    ue(Ie1,Ie2,Ie3,CE)= ugLocal(Ie1,Ie2,Ie3,CE)*sint+ugLocal(Ie1,Ie2,Ie3,CE+3)*cost;
          	    uh(Ih1,Ih2,Ih3,CH)= ugLocal(Ih1,Ih2,Ih3,CH)*sint+ugLocal(Ih1,Ih2,Ih3,CH+3)*cost;

	    //            ume(I1,I2,I3,C)= ug(I1,I2,I3,C)*sint+ug(I1,I2,I3,C+3)*cost;
          	    ume(I1,I2,I3,CE)= ugLocal(I1,I2,I3,CE)*sintm+ugLocal(I1,I2,I3,CE+3)*costm;
          	    umh(I1,I2,I3,CH)= ugLocal(I1,I2,I3,CH)*sintm+ugLocal(I1,I2,I3,CH+3)*costm;
        	  }
          	    
      	}
      	else if( initialConditionOption==planeMaterialInterfaceInitialCondition )
      	{
        	  if( method==nfdtd || method==sosup )
        	  { 
	    // adjust array dimensions for local arrays
          	    Index J1 = Range(max(I1.getBase(),uel.getBase(0)),min(I1.getBound(),uel.getBound(0)));
          	    Index J2 = Range(max(I2.getBase(),uel.getBase(1)),min(I2.getBound(),uel.getBound(1)));
          	    Index J3 = Range(max(I3.getBase(),uel.getBase(2)),min(I3.getBound(),uel.getBound(2)));

          // ------------ macro for the plane material interface -------------------------
          // initialCondition: initialCondition, error, boundaryCondition
          // -----------------------------------------------------------------------------
                        int i1,i2,i3;
                        real tm=t-dt,x,y,z;
                        const real pmct=pmc[18]*twoPi; // for time derivative of exact solution
                        if( numberOfDimensions==2 )
                        {
                          z=0.;
                          if( grid < numberOfComponentGrids/2 )
                          { // incident plus reflected wave.
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                x = XEP(i1,i2,i3,0);
                                y = XEP(i1,i2,i3,1);
                                real u1 = (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                real u2 = (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                real u3 = (pmc[10]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[11]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                  UEX(i1,i2,i3)= u1;
                                  UEY(i1,i2,i3)= u2;
                                  UHZ(i1,i2,i3)= u3;
                                  if( method==nfdtd )
                                  {
                                      UMEX(i1,i2,i3)= (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                                      UMEY(i1,i2,i3)= (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                                      UMHZ(i1,i2,i3)= (pmc[10]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[11]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                                  }
                                  else if( method==sosup )
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
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                x = XEP(i1,i2,i3,0);
                                y = XEP(i1,i2,i3,1);
                                real u1 = (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                real u2 = (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                real u3 = (pmc[17]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                  UEX(i1,i2,i3)= u1;
                                  UEY(i1,i2,i3)= u2;
                                  UHZ(i1,i2,i3)= u3;
                                  if( method==nfdtd )
                                  {
                           	 UMEX(i1,i2,i3)= (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMEY(i1,i2,i3)= (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMHZ(i1,i2,i3)= (pmc[17]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                                  }
                                  else if( method==sosup )
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
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                x = XEP(i1,i2,i3,0);
                                y = XEP(i1,i2,i3,1);
                                z = XEP(i1,i2,i3,2);
                                real u1 = (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                real u2 = (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                real u3 = (pmc[4]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[5]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                                  UEX(i1,i2,i3)= u1;
                                  UEY(i1,i2,i3)= u2;
                                  UEZ(i1,i2,i3)= u3;
                                  if( method==nfdtd )
                                  {
                           	 UMEX(i1,i2,i3)= (pmc[0]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[1]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMEY(i1,i2,i3)= (pmc[2]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[3]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMEZ(i1,i2,i3)= (pmc[4]*cos(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(tm)))+pmc[5]*cos(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(tm))));
                                  }
                                  else if( method==sosup )
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
                            FOR_3D(i1,i2,i3,J1,J2,J3)
                            {
                                x = XEP(i1,i2,i3,0);
                                y = XEP(i1,i2,i3,1);
                                z = XEP(i1,i2,i3,2);
                                real u1 = (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                real u2 = (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                real u3 = (pmc[14]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                  UEX(i1,i2,i3)= u1;
                                  UEY(i1,i2,i3)= u2;
                                  UEZ(i1,i2,i3)= u3;
                                  if( method==nfdtd )
                                  {
                           	 UMEX(i1,i2,i3)= (pmc[12]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMEY(i1,i2,i3)= (pmc[13]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                           	 UMEZ(i1,i2,i3)= (pmc[14]*cos(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(tm))));
                                  }
                                  else if( method==sosup )
                                  {
                           	 uLocal(i1,i2,i3,ext) = (pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                           	 uLocal(i1,i2,i3,eyt) = (pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                           	 uLocal(i1,i2,i3,ezt) = (pmct*pmc[14]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                                  }
                            }
                          }
                        }
        	  }
        	  else
        	  {
          	    printF("ERROR: initialConditionOption==planeMaterialInterfaceInitialCondition but method=%i\n",
               		   (int)method);
          	    OV_ABORT("ERROR");
        	  }
        	  
      	}

      	else if( initialConditionOption==gaussianIntegralInitialCondition )
      	{
        	  printF("Setting initial condition to be Tom's Gaussian integral solution");
          	    
	  // adjust array dimensions for local arrays
        	  Index J1 = Range(max(I1.getBase(),uel.getBase(0)),min(I1.getBound(),uel.getBound(0)));
        	  Index J2 = Range(max(I2.getBase(),uel.getBase(1)),min(I2.getBound(),uel.getBound(1)));
        	  Index J3 = Range(max(I3.getBase(),uel.getBase(2)),min(I3.getBound(),uel.getBound(2)));

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
                    FOR_3D(i1,i2,i3,J1,J2,J3)
                    {
                        double x=X(i1,i2,i3,0); 
                        double y=X(i1,i2,i3,1);
                        exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);
                            UEX(i1,i2,i3) = wy;
                            UEY(i1,i2,i3) =-wx;
                            UHZ(i1,i2,i3)= wt;
                    }
                }

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
                    double time=t-dt;
                    int i1,i2,i3;
                    FOR_3D(i1,i2,i3,J1,J2,J3)
                    {
                        double x=X(i1,i2,i3,0); 
                        double y=X(i1,i2,i3,1);
                        exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);
                            UMEX(i1,i2,i3) = wy;
                            UMEY(i1,i2,i3) =-wx;
                            UMHZ(i1,i2,i3)= wt;
                    }
                }
          	    
//             uh(Ih1,Ih2,Ih3,hz)=hzGaussianPulse(xhi);
//             ue(Ie1,Ie2,Ie3,ex)=exGaussianPulse(xei);
//             ue(Ie1,Ie2,Ie3,ey)=eyGaussianPulse(xei);

//             xhi+=cc*dt;
//             xei+=cc*dt;
//             umh(Ih1,Ih2,Ih3,hz)=hzGaussianPulse(xhi);
//             ume(Ie1,Ie2,Ie3,ex)=exGaussianPulse(xei);//u(I1,I2,I3,hz)*(-ky/(eps*cc));
//             ume(Ie1,Ie2,Ie3,ey)=eyGaussianPulse(xei);//u(I1,I2,I3,hz)*( kx/(eps*cc));

        	  if( saveExtraForcingLevels )
        	  {
	    // we need to save the "RHS" at some previous times.
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
            	      OV_ABORT("finish me for parallel");
#else
            	      realSerialArray & fnLocal = FN(m);
#endif
            	      fnLocal(I1,I2,I3,hz)=0.; 
            	      fnLocal(I1,I2,I3,ex)=0.; 
            	      fnLocal(I1,I2,I3,ey)=0.; 
          	    }
        	  }
      	}

      	else if( initialConditionOption==defaultInitialCondition )
      	{
        	  printF("Setting initial conditions to be zero. (default)\n");
          	    
	  //            u(I1,I2,I3,C)=0.;
	  //            um(I1,I2,I3,C)=0.;
        	  uh = 0;
        	  ue = 0;
        	  umh = 0.;
        	  ume = 0;
        	  if( saveExtraForcingLevels )
        	  {
          	    for( int m=0; m<numberOfFunctions; m++ )
          	    {
#ifdef USE_PPP
            	      realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(FN(m),fnLocal);
            	      OV_ABORT("finish me for parallel");
#else
            	      realSerialArray & fnLocal = FN(m);
#endif
            	      fnLocal(I1,I2,I3,C)=0.;
          	    }
        	  
        	  }
        	  if( boundaryForcingOption==planeWaveBoundaryForcing )
        	  {
          	    printF("*** Set BC's for planeWaveBoundaryForcing on initial conditions...\n");
          	    realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
          	    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];

          	    int option=0; // not used.
          	    assignBoundaryConditions( option, grid, t-dt, dt, fieldPrev, fieldPrev, prev );
          	    assignBoundaryConditions( option, grid, t   , dt, fieldCurrent, fieldCurrent, current );

        	  }

      	}
      	else if( initialConditionOption!=userDefinedInitialConditionsOption )
      	{
        	  OV_ABORT("Maxwell::unknown initialConditionOption option");
      	}

            }
#undef FN	      
//kkc XXX look inside EXTRACT_GFP_END for the replacement
//kkc XXX what should replace this??      fieldCurrent.periodicUpdate(); 
//kkc XXX what should replace this??      fieldPrev.periodicUpdate();

      // reset the cpp macros for the E and H field gridfunctions
      // should the above periodic updates be done in this macro too?
            if( method==dsiMatVec  )
            {
                #ifdef USE_PPP
                    Overture::abort("finish me for parallel");
                #else
        // XXX the following is broken for PARALLEL right now (need to use more macros...
                bool vCent = mg.isAllVertexCentered();
                int nDim = mg.numberOfDimensions();
                realMappedGridFunction &uep = (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+numberOfTimeLevels]);
                realMappedGridFunction &uhp = (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]);
                realMappedGridFunction &uepm = (mgp==NULL ? getCGField(EField,next)[grid] : fields[next+numberOfTimeLevels]);
                realMappedGridFunction &uhpm = (mgp==NULL ? getCGField(HField,next)[grid] : fields[next]);
                if ( mg.numberOfDimensions()==2 )
                {
                    for ( int e=0; e<edgeAreaNormals.getLength(0); e++ )
                    {
                        uep(e,0,0,0) = 0;
                        uepm(e,0,0,0) =  0;
                        for ( int a=0; a<mg.numberOfDimensions(); a++ )
                        {
                  	uep(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ue(e,0,0,a);
                  	uepm(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ume(e,0,0,a);
                        }
                    }
                    uhp = uh;
                    uhpm = umh;
                }
                else
                {
          //      cout<<"geom sizes "<<cFArea.getLength(0)<<"  "<<cEArea.getLength(0)<<endl;
                    for ( int f=0; f<faceAreaNormals.getLength(0); f++ )
                    {
                        uhp(f,0,0,0) = 0;
                        uhpm(f,0,0,0) =  0;
                        for ( int a=0; a<mg.numberOfDimensions(); a++ )
                        {
                  	uhp(f,0,0,0) += faceAreaNormals(f,0,0,a)*uh(f,0,0,a);
                  	uhpm(f,0,0,0) += faceAreaNormals(f,0,0,a)*umh(f,0,0,a);
                        }
                    }
                    for ( int e=0; e<edgeAreaNormals.getLength(0); e++ )
                    {
                        uep(e,0,0,0) = 0;
                        uepm(e,0,0,0) =  0;
                        for ( int a=0; a<mg.numberOfDimensions(); a++ )
                        {
                  	uep(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ue(e,0,0,a);
                  	uepm(e,0,0,0) += edgeAreaNormals(e,0,0,a)*ume(e,0,0,a);
                        }
                    }
                }
                #endif
            }
            if( method==nfdtd || method==sosup )
            {
                (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]).periodicUpdate();
                (mgp==NULL ? getCGField(HField,prev)[grid] : fields[prev]).periodicUpdate();
            }
            else
            {
                (mgp==NULL ? getCGField(HField,current)[grid] : fields[current]).periodicUpdate();
                (mgp==NULL ? getCGField(EField,current)[grid] : fields[current+2]).periodicUpdate();
            }
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
            
        } // end for grid
        
    } // end ! userDefinedIC

        
    if( debug & 4 ) 
    {	  
        for( int grid=0; grid<numberOfComponentGrids; grid++ )
        {
            realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
            realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];

            ::display(fieldPrev,sPrintF("fieldPrev after initial conditions, t=%e",t),debugFile,"%9.2e ");
            ::display(fieldCurrent,sPrintF("fieldCurrent after initial conditions, t=%e",t),debugFile,"%9.2e ");
        }  // end for grid
    } 


    const bool & useNewForcingMethod= dbase.get<bool>("useNewForcingMethod");
    if( useNewForcingMethod )
    {
    // --- Assign time history of the external forcing arrays (if needed) ---

    // numberOfForcingFunctions : assigned in setupGridFunctions

    // numberOfForcingFunctions : number of elements in forcingArray
        const int & numberOfForcingFunctions= dbase.get<int>("numberOfForcingFunctions"); 
        const int & fCurrent = dbase.get<int>("fCurrent");         // forcingArray[fCurrent] : current forcing
        realArray *& forcingArray = dbase.get<realArray*>("forcingArray");  

        if( numberOfForcingFunctions>0 && method==nfdtd && timeSteppingMethod==modifiedEquationTimeStepping )
        {
            assert( forcingIsOn() );
        
      // --- Evaluate past time forcing ---
            printF("--MX-- INFO: evaluate external forcing at past time levels...\n");

            for( int grid=0; grid<numberOfComponentGrids; grid++ )
            {
      	MappedGrid & mg = cg[grid];
      	Index I1,I2,I3;
      	getIndex(mg.dimension(),I1,I2,I3);
      	Range C(ex,hz);
      	
      	realArray & fa = forcingArray[grid];
      	
      	realArray fb;  // *************** FIX ME **************
      	fb.partition(mg.getPartition());
      	fb.redim(I1,I2,I3,C);  // could use some other array for work space ??

                OV_GET_SERIAL_ARRAY(real,fa,faLocal);
      	OV_GET_SERIAL_ARRAY(real,fb,fbLocal);
      	int includeGhost=1;
      	bool ok = ParallelUtility::getLocalArrayBounds(fb,fbLocal,I1,I2,I3,includeGhost);

      	for( int m=0; m<numberOfForcingFunctions; m++ ) 
      	{
                    int fIndex = (fCurrent - m + numberOfForcingFunctions) % numberOfForcingFunctions;
                    const int option=1;  // do not append forcing to the "f" array 
                    getForcing( current, grid,fb,t-m*dt,dt,option );

        	  if( ok )
                        faLocal(I1,I2,I3,C,fIndex)=fbLocal(I1,I2,I3,C);  // save in fa array
      	}

            }
        }
    }


    if( projectInitialConditions )
    {
        printF("--MX-- project initial conditions: t-dt=%9.3e and t=%9.3e\n",t-dt,t);
          
        project( numberOfStepsTaken-1, prev,    t-dt, dt );

        project( numberOfStepsTaken  , current, t   , dt );

        if( method!=yee )
        {
      // Apply the BC;s
            for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];
      	realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
      	realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];

      	int option=0;
        // what should we use as the "previous" solution in the next call ?
      	assignBoundaryConditions( option, grid, t-dt, dt, fieldPrev   , fieldCurrent,current ); // check this

      	assignBoundaryConditions( option, grid, t   , dt, fieldCurrent, fieldPrev   ,prev );
            }
        }
    }
    
    timing(timeForProject)=0.; // count project of IC's as part of the time for IC's
    timing(timeForInitialConditions)+=getCPU()-time0;

}

// ===========================================================
// Macro to set the known solution inside a radius
// ===========================================================

// =========================================================================================
//  Use this function to initialize a known solution -- currently used to compute
//  scattering past a cylinder or sphere or the Gaussian integral solution from Tom H.
// =========================================================================================
void Maxwell::
initializeKnownSolution()
{
    if( knownSolution!=NULL )
        return;

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;

    if( knownSolution==NULL )
    {
    // We save the Re and Im parts of the known solution
    //      u(.,.,.,0..2) = (Ex,Ey,[Hz,ez]) : Re part
    //      u(.,.,.,3..5) = (Ex,Ey,[Hz,ez]) : Im part
        Range all;
        int numberOfComponentsInKnown=6;
        if( method==yee )
        {
            numberOfComponentsInKnown=(cg.numberOfDimensions()-1)*6;  // yee = 6 in 2d, 12 in 3d
        }
        
        knownSolution=new realCompositeGridFunction(cg,all,all,all,numberOfComponentsInKnown);
    }

//    if( true )
//    {
//      *knownSolution=0.;
//      return;
//    }

  // const real a=.5;  // radius of the cylinder or sphere
    const real a = dbase.get<real>("scatteringRadius"); // radius of the cylinder or sphere *wdh* 2015/07/03
    real cr = 1.;  // c1/c2 (c2=inside)
    int computeIncident=0;  // set to 1 to compute incident wave too 
    real rpar[] = {twoPi*kx,a,cr}; //
    int option=0;  // 0=PEC cylinder, 1=di-electric
    int inOut=0;   // 0=exterior, 1=interior to the dielectric
    int staggeredGrid = method==yee ? 1 : 0;
    int ipar[] = {0,1,2,3,4,5,option,inOut,computeIncident,staggeredGrid,debug}; //

    int numberOfComponents=3;
    if( cg.numberOfDimensions()==3 && solveForMagneticField )
    {
        numberOfComponents=6;
    }
    

    Index I1,I2,I3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {

        MappedGrid & mg = cg[grid];
        mg.update(MappedGrid::THEcenter);
        
        getIndex(mg.dimension(),I1,I2,I3);
        
#ifdef USE_PPP
        const realArray & uga = (*knownSolution)[grid];
        realSerialArray ug; getLocalArrayWithGhostBoundaries(uga,ug);

        const realArray & xya = mg.center();
        realSerialArray xy;  getLocalArrayWithGhostBoundaries(xya,xy);

        const int includeGhost=1;
        bool ok = ParallelUtility::getLocalArrayBounds(uga,ug,I1,I2,I3,includeGhost); 
        if( !ok ) continue;

#else
        const realSerialArray & ug = (*knownSolution)[grid];
        const realSerialArray & xy = mg.center();
#endif

        if( method==yee &&
                (knownSolutionOption==scatteringFromADielectricDiskKnownSolution ||
       	 knownSolutionOption==scatteringFromADielectricSphereKnownSolution) )
        {
      // ----------------------------------------------------
      // ------------- Yee : staggered grid -----------------
      // ----------------------------------------------------

      // dielectric cylinder or sphere with embedded regions
            assert( numberOfMaterialRegions==2 );
            option=1;  ipar[6]=option; // 1=dielectric 

      // rpar[2] = sqrt(epsGrid(2)/epsGrid(0)); // c1/c2 (c2=inside)
            rpar[2] = sqrt( epsv(1)/epsv(0) );

            if( cg.numberOfDimensions()==2 )
            { 
	// -- dielectric cylinder --
      	rpar[1] =.4;  // radius for dielectric cyl
            }
            else
            {
	// -- dielectric sphere --
      	rpar[1] =1.;  // radius for dielectric sphere 
      	ipar[8]=1;    // compute incident field
            }      

            
      // ------------------------------------------------
      // -- Eval the solution outside and save in ug ----
      // ------------------------------------------------
            inOut=0; ipar[7]=inOut; // eval outside
            if( cg.numberOfDimensions()==2 )
            {
        // --- evaluate scattering by a cylinder ---

      	printF(" Call scatCyl for grid=%i, option=%i cr=%7.3f, inOut=%i...\n",grid,option,rpar[2],inOut);
      	scatCyl(mg.numberOfDimensions(), 
            		I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
            		ug.getBase(0),ug.getBound(0),
            		ug.getBase(1),ug.getBound(1),
            		ug.getBase(2),ug.getBound(2),
            		ug.getBase(3),ug.getBound(3),
            		*(xy.getDataPointer()),*(ug.getDataPointer()),ipar[0],rpar[0] );
            }
            else
            {
        // --- evaluate scattering by a sphere ---
                assert( ug.getLength(3)==12 );
            
      	printF(" Call scatSphere for grid=%i...\n",grid);
      	scatSphere(mg.numberOfDimensions(), 
               		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
               		   ug.getBase(0),ug.getBound(0),
               		   ug.getBase(1),ug.getBound(1),
               		   ug.getBase(2),ug.getBound(2),
               		   ug.getBase(3),ug.getBound(3),
               		   *(xy.getDataPointer()),*(ug.getDataPointer()),ipar[0],rpar[0] );
            }

      // -----------------------------------------------
      // -- Eval the solution inside and save in ui ----
      // -----------------------------------------------

            inOut=1; ipar[7]=inOut; // eval inside
            RealSerialArray ui(ug.dimension(0),ug.dimension(1),ug.dimension(2),ug.dimension(3));
            ui=0.;
            if( cg.numberOfDimensions()==2 )
            {
        // --- evaluate scattering by a cylinder ---

      	printF(" Call scatCyl for grid=%i, option=%i cr=%7.3f...\n",grid,option,rpar[2]);
      	scatCyl(mg.numberOfDimensions(), 
            		I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
            		ug.getBase(0),ug.getBound(0),
            		ug.getBase(1),ug.getBound(1),
            		ug.getBase(2),ug.getBound(2),
            		ug.getBase(3),ug.getBound(3),
            		*(xy.getDataPointer()),*(ui.getDataPointer()),ipar[0],rpar[0] );
            }
            else
            {
        // --- evaluate scattering by a sphere ---
      	printF(" Call scatSphere for grid=%i...\n",grid);
      	scatSphere(mg.numberOfDimensions(), 
               		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
               		   ug.getBase(0),ug.getBound(0),
               		   ug.getBase(1),ug.getBound(1),
               		   ug.getBase(2),ug.getBound(2),
               		   ug.getBase(3),ug.getBound(3),
               		   *(xy.getDataPointer()),*(ui.getDataPointer()),ipar[0],rpar[0] );
            }

      // Now fill in interior values in ug from ui 

            I1=Range(I1.getBase(),min(I1.getBound(),ug.getBound(0)-1));
            I2=Range(I2.getBase(),min(I2.getBound(),ug.getBound(1)-1));
            if( mg.numberOfDimensions()==3 )
      	I3=Range(I3.getBase(),min(I3.getBound(),ug.getBound(2)-1));
            
            const real radiusSquared=SQR(rpar[1]);
            real x0,y0,z0,xp,yp,zp,rad2;
            int i1,i2,i3;
            if( mg.numberOfDimensions()==2 )
            {
        // --- 2D cylinder ---
                z0=0.;
      	FOR_3D(i1,i2,i3,I1,I2,I3)
      	{
        	  x0 = xy(i1,i2,i3,0);
        	  y0 = xy(i1,i2,i3,1);
        	  xp = .5*( x0 + xy(i1+1,i2,i3,0) );
        	  yp = .5*( y0 + xy(i1,i2+1,i3,1) );

          // Set Ex,Ey,Hz to the inner solution if the location is inside the cyl
                    rad2 = SQR(xp)+SQR(y0)+SQR(z0);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,ex                   )=ui(i1,i2,i3,ex                   );
                        ug(i1,i2,i3,ex+numberOfComponents)=ui(i1,i2,i3,ex+numberOfComponents);
                    }
                    rad2 = SQR(x0)+SQR(yp)+SQR(z0);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,ey                   )=ui(i1,i2,i3,ey                   );
                        ug(i1,i2,i3,ey+numberOfComponents)=ui(i1,i2,i3,ey+numberOfComponents);
                    }
                    rad2 = SQR(xp)+SQR(yp)+SQR(z0);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,hz                   )=ui(i1,i2,i3,hz                   );
                        ug(i1,i2,i3,hz+numberOfComponents)=ui(i1,i2,i3,hz+numberOfComponents);
                    }

      	}
            }
            else
            {
        // --- 3D sphere ---
                assert( numberOfComponents==6 );
      	FOR_3D(i1,i2,i3,I1,I2,I3)
      	{
        	  x0 = xy(i1,i2,i3,0);
        	  y0 = xy(i1,i2,i3,1);
        	  z0 = xy(i1,i2,i3,2);
        	  xp = .5*( x0 + xy(i1+1,i2,i3,0) );
        	  yp = .5*( y0 + xy(i1,i2+1,i3,1) );
        	  zp = .5*( z0 + xy(i1,i2,i3+1,2) );

          // Set Ex,Ey,Ez, Hx,Hy,Hz to the inner solution if the location is inside the cyl
                    rad2 = SQR(xp)+SQR(y0)+SQR(z0);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,ex                   )=ui(i1,i2,i3,ex                   );
                        ug(i1,i2,i3,ex+numberOfComponents)=ui(i1,i2,i3,ex+numberOfComponents);
                    }
                    rad2 = SQR(x0)+SQR(yp)+SQR(z0);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,ey                   )=ui(i1,i2,i3,ey                   );
                        ug(i1,i2,i3,ey+numberOfComponents)=ui(i1,i2,i3,ey+numberOfComponents);
                    }
                    rad2 = SQR(x0)+SQR(y0)+SQR(zp);
                    if( rad2<radiusSquared )
                    { // set real and imaginary parts of the known solution:
                        ug(i1,i2,i3,ez                   )=ui(i1,i2,i3,ez                   );
                        ug(i1,i2,i3,ez+numberOfComponents)=ui(i1,i2,i3,ez+numberOfComponents);
                    }
        	  if( solveForMagneticField )
        	  {
                        rad2 = SQR(x0)+SQR(yp)+SQR(zp);
                        if( rad2<radiusSquared )
                        { // set real and imaginary parts of the known solution:
                            ug(i1,i2,i3,hx                   )=ui(i1,i2,i3,hx                   );
                            ug(i1,i2,i3,hx+numberOfComponents)=ui(i1,i2,i3,hx+numberOfComponents);
                        }
                        rad2 = SQR(xp)+SQR(y0)+SQR(zp);
                        if( rad2<radiusSquared )
                        { // set real and imaginary parts of the known solution:
                            ug(i1,i2,i3,hy                   )=ui(i1,i2,i3,hy                   );
                            ug(i1,i2,i3,hy+numberOfComponents)=ui(i1,i2,i3,hy+numberOfComponents);
                        }
                        rad2 = SQR(xp)+SQR(yp)+SQR(z0);
                        if( rad2<radiusSquared )
                        { // set real and imaginary parts of the known solution:
                            ug(i1,i2,i3,hz                   )=ui(i1,i2,i3,hz                   );
                            ug(i1,i2,i3,hz+numberOfComponents)=ui(i1,i2,i3,hz+numberOfComponents);
                        }
        	  }
        	  
      	}
            }

        }
        else if( knownSolutionOption==scatteringFromADiskKnownSolution ||
           	     knownSolutionOption==scatteringFromADielectricDiskKnownSolution ||
           	     knownSolutionOption==scatteringFromASphereKnownSolution ||
           	     knownSolutionOption==scatteringFromADielectricSphereKnownSolution )
        {
            if( gridHasMaterialInterfaces )
            {
	//assert( (cg.numberOfDimensions()==2 && cg.numberOfComponentGrids()==4 ) ||
        //        (cg.numberOfDimensions()==3 && (cg.numberOfComponentGrids()==6 || cg.numberOfComponentGrids()==8) ) );
      	option=1;
      	ipar[6]=option;
	// rpar[2] = sqrt(epsGrid(2)/epsGrid(0)); // c1/c2 (c2=inside)
	// assume grids 0 and 1 are outside and grids 2,3 are inside the cylinder
      	if( cg.numberOfDimensions()==2 )
      	{
          // -- dielectric cylinder --
            	  rpar[1] =.4;  // radius for dielectric cyl
        	  cr = cGrid(0)/cGrid(cg.numberOfComponentGrids()-1); // c1/c2 (c2=inside)
        	  rpar[2] = cr;
        	  if( cg.numberOfDomains()>1 )
                        ipar[7]= cg.domainNumber(grid);    // new way 
        	  else
              	    ipar[7]= grid<=1 ? 0 : 1;       // assume 2 grids on the outside (fix me!)
      	}
                else
      	{
          // -- dielectric sphere --
        	  rpar[1] =1.;  // radius for dielectric sphere 
        	  cr = cGrid(0)/cGrid(cg.numberOfComponentGrids()-1); // c1/c2 (c2=inside)
        	  rpar[2] = cr;
          // ipar[7]= grid<=2 ? 0 : 1;       // assume 3 grids on the outside (fix me!)
                    ipar[7]= cg.domainNumber(grid);    // new way domain=0 : outside, 1=inside 
                    ipar[8]=1;  // compute incident field
      	}
            }

            if( cg.numberOfDimensions()==2 )
            {
        // --- evaluate scattering by a cylinder ---

      	printF(" Call scatCyl for grid=%i, option=%i cr=%7.3f...\n",grid,option,cr);
      	scatCyl(mg.numberOfDimensions(), 
            		I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
            		ug.getBase(0),ug.getBound(0),
            		ug.getBase(1),ug.getBound(1),
            		ug.getBase(2),ug.getBound(2),
            		ug.getBase(3),ug.getBound(3),
            		*(xy.getDataPointer()),*(ug.getDataPointer()),ipar[0],rpar[0] );
            }
            else
            {
        // --- evaluate scattering by a sphere ---
      	printF(" Call scatSphere for grid=%i...\n",grid);
      	scatSphere(mg.numberOfDimensions(), 
               		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
               		   ug.getBase(0),ug.getBound(0),
               		   ug.getBase(1),ug.getBound(1),
               		   ug.getBase(2),ug.getBound(2),
               		   ug.getBase(3),ug.getBound(3),
               		   *(xy.getDataPointer()),*(ug.getDataPointer()),ipar[0],rpar[0] );
            }


      // Add on the incident field if we are computing the total field
      //   -- only add on incident field for the outer domain in multi-domain problems ---
            if( initialConditionOption==planeWaveInitialCondition   &&
                    (cg.numberOfDomains()==1 || cg.domainNumber(grid)==0 ) )
            {
                printF(" ** Add on incident field grid=%i...\n",grid);
                const real t=0.;
      	const real cc= c*sqrt( kx*kx+ky*ky+kz*kz );
      	if( mg.numberOfDimensions()==2 )
      	{
        	  const realSerialArray & x = xy(I1,I2,I3,0);
        	  const realSerialArray & y = xy(I1,I2,I3,1);


          // Ex.t=(Hz).y =>  -i*k*Ex = (Hz).y -> Ex = i (Hz).y/k  -> Re(Ex) = -Im(Hz.y)/k  Im(Ex) = Re(Hz.y)
          // k*Ey =-i*(Hz).x

          // the following is only valid for material interfaces I think 
        	  assert( gridHasMaterialInterfaces );

                    realSerialArray cosa,sina;
        	  cosa=cos(twoPi*(kx*(x)+ky*(y)-cc*(t)));
        	  sina=sin(twoPi*(kx*(x)+ky*(y)-cc*(t)));
        	  
        	  ug(I1,I2,I3,hz  )+= cosa;
        	  ug(I1,I2,I3,hz+3)+= sina;

        	  ug(I1,I2,I3,ex  )+= -cosa*(ky/cc); 
        	  ug(I1,I2,I3,ex+3)+= -sina*(ky/cc);

        	  ug(I1,I2,I3,ey  )+= cosa*(kx/cc); 
        	  ug(I1,I2,I3,ey+3)+= sina*(kx/cc);

      	}
      	else
      	{
          // *** fix me for material interfaces ***
        	  assert( !gridHasMaterialInterfaces );
        	  
        	  const realSerialArray & x = xy(I1,I2,I3,0);
        	  const realSerialArray & y = xy(I1,I2,I3,1);
        	  const realSerialArray & z = xy(I1,I2,I3,2);

        	  if( solveForElectricField )
        	  {
          	    ug(I1,I2,I3,ex)+=exTrue3d(x,y,z,t);
          	    ug(I1,I2,I3,ey)+=eyTrue3d(x,y,z,t);
          	    ug(I1,I2,I3,ez)+=ezTrue3d(x,y,z,t);
        	  }

      	}
            }

        }
        else
        {
            printF("initializeKnownSolution:ERROR: unexpected initialConditionOption=%i\n",
           	     initialConditionOption);
            OV_ABORT("initializeKnownSolution:ERROR");
        }



    }  // end for grid
    

}


