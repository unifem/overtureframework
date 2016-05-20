// This file automatically generated from getErrors.bC with bpp.
#include "Maxwell.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "interpPoints.h"
#include "ShowFileReader.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"

#define exmax EXTERN_C_NAME(exmax)
extern "C"
{

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

// Macros for the plane material interface:

 // -- incident wave ---
 //  --- time derivative of incident ---
 // -- transmitted wave ---
 //  --- time derivative of transmitted wave ---

//! local function to compute errors for the staggered grid DSI schemes
void
computeDSIErrors( Maxwell &mx, MappedGrid &mg, realArray &uh, realArray &uhp, realArray &ue, realArray &uep,
              		  realArray &errh, realArray &erre, 
              		  RealArray &solutionNorm, RealArray &maximumError )
{
    bool skipGhosts = true;
    ArraySimpleFixed<int,6,1,1,1> ml;
    ml = -1;

    real l2h_err,l2e_err;
    real nE, nH;
    l2h_err = l2e_err = 0;
    nE=nH=0;

    real maxDivE = 0, maxDivH = 0;
    if ( mg.getGridType()==MappedGrid::unstructuredGrid )
    {
            
        UnstructuredMapping & umap = (UnstructuredMapping &) mg.mapping().getMapping();
        int rDim = umap.getRangeDimension();

//       const realArray &verts = umap.getNodes();
//       const intArray  &edges = umap.getEntities(UnstructuredMapping::Edge);

        UnstructuredMappingIterator iter,iter_end;
        iter_end = umap.end( UnstructuredMapping::Edge, skipGhosts ); // true means skip ghosts
        for ( iter = umap.begin( UnstructuredMapping::Edge, skipGhosts );
        	  iter!=iter_end;
        	  iter++ )
        {
            assert(!umap.isGhost(UnstructuredMapping::Edge, *iter));
            int e = *iter;
      //	  if ( !umap.hasTag(UnstructuredMapping::Edge, e, "__bcnum ") )
        {
            for ( int c=0; c<ue.getLength(3); c++ )
            {
      	solutionNorm(c) = max(solutionNorm(c), fabs(ue(e,0,0,c)));
      	if ( maximumError(c)<fabs(erre(e,0,0,c)) )
        	  ml[c] = e;
      	maximumError(c) = max(maximumError(c), fabs(erre(e,0,0,c)));
      	l2e_err += erre(e,0,0,c)*erre(e,0,0,c);
            }
            nE++;
        }
    //	  cout<<.5*(verts(edges(*iter,0),0)+verts(edges(*iter,1),0))<<"  "<<.5*(verts(edges(*iter,0),1)+verts(edges(*iter,1),1))<<"  "<<erre(*iter,0,0,0)<<endl;
        }
            
        l2e_err /= real(nE);

        int off = ue.getLength(3);
        iter_end = umap.end( UnstructuredMapping::Face, skipGhosts ); // true means skip ghosts
        for ( iter = umap.begin( UnstructuredMapping::Face, skipGhosts );
        	  iter!=iter_end;
        	  iter++ )
        {
            int f = *iter;
            assert(!umap.isGhost(UnstructuredMapping::Face, *iter));
      //	  if ( !umap.hasTag(UnstructuredMapping::Face, f, "__bcnum ") )
        {
            for ( int c=0; c<uh.getLength(3); c++ )
            {
      	solutionNorm(off+c) = max(solutionNorm(off+c), fabs(uh(f,0,0,c)));
      	if ( maximumError(off+c)<fabs(errh(f,0,0,c)) )
        	  ml[off+c] = f;
      	maximumError(off+c) = max(maximumError(off+c), fabs(errh(f,0,0,c)));
      	l2h_err += errh(f,0,0,c)*errh(f,0,0,c);
            }
            nH++;
        }
        }
        l2h_err /= real(nH);
        maximumError(0) = sqrt(l2e_err);
        maximumError(off) = sqrt(l2h_err);
            
        iter_end = umap.end(UnstructuredMapping::Vertex);
        for ( iter=umap.begin(UnstructuredMapping::Vertex); iter!=iter_end; iter++ )
        {
            if ( !(iter.isGhost() || iter.isBC()) )
            {
      	real divE = 0;
      	UnstructuredMappingAdjacencyIterator aiter,aiter_end;
      	aiter_end = umap.adjacency_end(iter, UnstructuredMapping::Edge);
      	for ( aiter=umap.adjacency_begin(iter, UnstructuredMapping::Edge); aiter!=aiter_end; aiter++ )
      	{
        	  divE += aiter.orientation()*uep(*aiter,0,0);
        	  if ( aiter.isBC() || aiter.isGhost() )
        	  {
          	    divE = 0;
          	    break;
        	  }
      	}
      	maxDivE = max(maxDivE,fabs(divE));
            }
        	  
        }

        if (rDim==3)
        {
            iter_end = umap.end(UnstructuredMapping::Region);
            for ( iter=umap.begin(UnstructuredMapping::Region); iter!=iter_end; iter++ )
            {
      	if ( !(iter.isGhost() || iter.isBC()) )
      	{
        	  real divH = 0;
        	  UnstructuredMappingAdjacencyIterator aiter,aiter_end;
        	  aiter_end = umap.adjacency_end(iter, UnstructuredMapping::Face);
        	  for ( aiter=umap.adjacency_begin(iter, UnstructuredMapping::Face); aiter!=aiter_end; aiter++ )
        	  {
          	    divH += aiter.orientation()*uhp(*aiter,0,0);
          	    if ( aiter.isBC() || aiter.isGhost() )
          	    {
            	      divH = 0;
            	      break;
          	    }
        	  }
        	  maxDivH = max(maxDivH,fabs(divH));
      	}
            	      
            }
        }

    }

  //  cout<<"max error locs "<<ml<<endl;
  //  cout<<"max Div E = "<<maxDivE<<endl;
  //  cout<<"max Div H = "<<maxDivH<<endl;
    mx.divEMax = maxDivE;
    mx.gradEMax = maxDivH; // use this spot for div H in the dsi code
    cout<<"max E error location "<<ml<<endl;
    maximumError.display("maximum error");
}



//! Determine the errors.
/*!

  */
void Maxwell::
getErrors( int current, real t, real dt )
// =================================================================================================================
// =================================================================================================================
{
    if( !checkErrors )
        return;

    real time0=getCPU();
    
    const real cc= c*sqrt( kx*kx+ky*ky+kz*kz);
    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Index Iav[3], &Ia1=Iav[0], &Ia2=Iav[1], &Ia3=Iav[2]; 
    Index Ibv[3], &Ib1=Ibv[0], &Ib2=Ibv[1], &Ib3=Ibv[2]; 
    Index Icv[3], &Ic1=Icv[0], &Ic2=Icv[1], &Ic3=Icv[2]; 

    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;
    
  // printF(" >>>>>>>>getErrors: current=%i next=%i <<<<<<<<<<< \n",current,next);
    

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int numberOfDimensions = cg.numberOfDimensions();

    


    Range C(ex,hz);
    maximumError.redim(numberOfSequences); 
    if( method==nfdtd || method==yee )
    {
        solutionNorm.redim(C);  // for nfdtd
    }
    else if( method==sosup )
    {
        const int numberOfComponents= cgfields[0][0].getLength(3);
        C=numberOfComponents;
        solutionNorm.redim(numberOfComponents);  
    }
    else
    {
        if(numberOfDimensions==2)
        {
            solutionNorm.redim(3);
        }
        else
        {
            solutionNorm.redim(6);
        }
    }

    maximumError=0.;
    solutionNorm=1.;
    
    realCompositeGridFunction *uReference=NULL;
    if( compareToReferenceShowFile )
    {
    // This case is used for comparing absorbing BC's -- we compare the solution to a reference
    // solution that was computed on a bigger grid
        if( referenceShowFileReader==NULL )
        {
            referenceShowFileReader = new ShowFileReader(nameOfReferenceShowFile);
        }
          	    
        CompositeGrid cgRef;
        realCompositeGridFunction uRef;
          	    
        int solutionNumber = 1 + int( t/tPlot + .5); // fix this ******************************
        printF(" **** compareToReferenceShowFile: t=%f solutionNumber=%i\n",t,solutionNumber);
        
        referenceShowFileReader->getASolution(solutionNumber,cgRef,uRef);        // read in a grid and solution

    // This solution uReference will live on the smaller domain 
        Range all;
        uReference = new realCompositeGridFunction(cg,all,all,all,C);
        cg.update(MappedGrid::THEmask );
        cgRef.update(MappedGrid::THEmask );
        interpolateAllPoints( uRef,*uReference );  // interpolate uReference from uRef
    }

    maximumError=0.;  // max error over all grids
    solutionNorm=0.;
    
  //kkc 040310 moved this assertion outside the following loop
    assert( cgerrp!=NULL || errp!=NULL );

    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {

        if( method==yee )
        {
            assert( numberOfComponentGrids==1 );
            
            int option=1;
            int iparam[5] = { -1,-1,0,0,0 }; // 
            getValuesFDTD( option, iparam, current, t, dt, cgerrp );

            continue;
        }



        c = cGrid(grid);
        eps = epsGrid(grid);
        mu = muGrid(grid);

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
        realSerialArray uLocal;
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
                realSerialArray umLocal; getLocalArrayWithGhostBoundaries(umall,umLocal);
                realSerialArray unLocal; getLocalArrayWithGhostBoundaries(unall,unLocal);
            #else
                uLocal.reference(uall);
                realSerialArray & umLocal = umall;
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
                    reconstructDSIField(tE,EField,uep,uer);  
                    reconstructDSIField(tE-dt,EField,uepm,uerm);
                    reconstructDSIField(tH,HField,uhp,uhr);  
                    reconstructDSIField(tH-dt,HField,uhpm,uhrm);
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

        bool energyOnly = false;

        const int i0a=mg.gridIndexRange(0,0);
        const int i1a=mg.gridIndexRange(0,1);
        const int i2a=mg.gridIndexRange(0,2);

        const real xa=xab[0][0], dx0=dx[0];
        const real ya=xab[0][1], dy0=dx[1];
        const real za=xab[0][2], dz0=dx[2];

        #define X0(i0,i1,i2) (xa+dx0*(i0-i0a))
        #define X1(i0,i1,i2) (ya+dy0*(i1-i1a))
        #define X2(i0,i1,i2) (za+dz0*(i2-i2a))

        const int numberOfGhost=method==nfdtd ? orderOfAccuracyInSpace/2 : 0;
            
    // Range C(ex,hz);

        real errEx,errEy,errHz;

    // Here is the box where we evaluate the error when there is a PML
        bool usePML = getBoundsForPML( mg,Iv,pmlErrorOffset ); 
        if( usePML )
        { // do NOT include PML region in the bounds: 
            adjustBoundsForPML(mg,Iev,pmlErrorOffset ); 
            adjustBoundsForPML(mg,Ihv,pmlErrorOffset ); 
        }

        errh = 0.;
        erre = 0.;
    // if( initialConditionOption==planeWaveInitialCondition )
        if( knownSolutionOption==planeWaveKnownSolution )
        {
            if( numberOfDimensions==2 )
            {
// 	      err(I1,I2,I3,ex)=u(I1,I2,I3,ex)-exTrue(x,y,t);
// 	      err(I1,I2,I3,ey)=u(I1,I2,I3,ey)-eyTrue(x,y,t);
// 	      err(I1,I2,I3,hz)=u(I1,I2,I3,hz)-hzTrue(x,y,t);
      	
      	erre(Ie1,Ie2,Ie3,ex)  = ue(Ie1,Ie2,Ie3,ex)-exTrue(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),tE);
      	erre(Ie1,Ie2,Ie3,ey)  = ue(Ie1,Ie2,Ie3,ey)-eyTrue(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),tE);
      	errh(Ih1,Ih2,Ih3,hz)  = uh(Ih1,Ih2,Ih3,hz)-hzTrue(xh(Ih1,Ih2,Ih3),yh(Ih1,Ih2,Ih3),tH);

      	if( method==sosup )
      	{
                    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          // printF(" errLocal = [%i,%i]\n",errLocal.getBase(3),errLocal.getBound(3));

        	  errLocal(Ie1,Ie2,Ie3,ext)  = uLocal(Ie1,Ie2,Ie3,ext)-extTrue(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),tE);
        	  errLocal(Ie1,Ie2,Ie3,eyt)  = uLocal(Ie1,Ie2,Ie3,eyt)-eytTrue(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),tE);
        	  errLocal(Ih1,Ih2,Ih3,hzt)  = uLocal(Ih1,Ih2,Ih3,hzt)-hztTrue(xh(Ih1,Ih2,Ih3),yh(Ih1,Ih2,Ih3),tH);

      	}

            }
            else // 3D
            {
// 	      if( solveForElectricField )
// 	      {
// 		err(I1,I2,I3,ex)=u(I1,I2,I3,ex)-exTrue3d(x,y,z,t);
// 		err(I1,I2,I3,ey)=u(I1,I2,I3,ey)-eyTrue3d(x,y,z,t);
// 		err(I1,I2,I3,ez)=u(I1,I2,I3,ez)-ezTrue3d(x,y,z,t);
// 	      }
//               if( solveForMagneticField )
// 	      {
// 		err(I1,I2,I3,hx)=u(I1,I2,I3,hx)-hxTrue3d(x,y,z,t);
// 		err(I1,I2,I3,hy)=u(I1,I2,I3,hy)-hyTrue3d(x,y,z,t);
// 		err(I1,I2,I3,hz)=u(I1,I2,I3,hz)-hzTrue3d(x,y,z,t);
// 	      }
      	if( solveForElectricField )
      	{
        	  erre(Ie1,Ie2,Ie3,ex)=ue(Ie1,Ie2,Ie3,ex)-exTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);
        	  erre(Ie1,Ie2,Ie3,ey)=ue(Ie1,Ie2,Ie3,ey)-eyTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);
        	  erre(Ie1,Ie2,Ie3,ez)=ue(Ie1,Ie2,Ie3,ez)-ezTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);

        	  if( method==sosup )
        	  {
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
	    // printF(" errLocal = [%i,%i]\n",errLocal.getBase(3),errLocal.getBound(3));

          	    errLocal(Ie1,Ie2,Ie3,ext)  = uLocal(Ie1,Ie2,Ie3,ext)-extTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);
          	    errLocal(Ie1,Ie2,Ie3,eyt)  = uLocal(Ie1,Ie2,Ie3,eyt)-eytTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);
          	    errLocal(Ie1,Ie2,Ie3,ezt)  = uLocal(Ie1,Ie2,Ie3,ezt)-eztTrue3d(xe(Ie1,Ie2,Ie3),ye(Ie1,Ie2,Ie3),ze(Ie1,Ie2,Ie3),tE);

        	  }

      	}
      	if( solveForMagneticField )
      	{
        	  errh(Ih1,Ih2,Ih3,hx)=uh(Ih1,Ih2,Ih3,hx)-hxTrue3d(xh(Ih1,Ih2,Ih3),yh(Ih1,Ih2,Ih3),zh(Ih1,Ih2,Ih3),tH);
        	  errh(Ih1,Ih2,Ih3,hy)=uh(Ih1,Ih2,Ih3,hy)-hyTrue3d(xh(Ih1,Ih2,Ih3),yh(Ih1,Ih2,Ih3),zh(Ih1,Ih2,Ih3),tH);
        	  errh(Ih1,Ih2,Ih3,hz)=uh(Ih1,Ih2,Ih3,hz)-hzTrue3d(xh(Ih1,Ih2,Ih3),yh(Ih1,Ih2,Ih3),zh(Ih1,Ih2,Ih3),tH);
      	}
            }
          	    
        }
        else if( knownSolutionOption==twilightZoneKnownSolution )
        {
      // *****************************************************************
      // ******************* TZ FORCING **********************************
      // *****************************************************************
            assert( tz!=NULL );
            OGFunction & e = *tz;
            realArray & center = mg.center();
      // display(center,"center"); //ok
          	    
      // display(ee,"exact solution for error computation");
          	    
            Index J1,J2,J3;

            int i1,i2,i3;
            if( mg.numberOfDimensions()==2 )
            {
      	J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
      	J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
      	J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
      	FOR_3D(i1,i2,i3,J1,J2,J3)
      	{
        	  real x0 = XEP(i1,i2,i3,0);
        	  real y0 = XEP(i1,i2,i3,1);
        	  ERREX(i1,i2,i3)=UEX(i1,i2,i3)-e(x0,y0,0.,ex,tE);
        	  ERREY(i1,i2,i3)=UEY(i1,i2,i3)-e(x0,y0,0.,ey,tE);
      	}
      	if( method==sosup )
      	{
          // Compute errors in the time derivative
                    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    real x0 = XEP(i1,i2,i3,0);
          	    real y0 = XEP(i1,i2,i3,1);
          	    errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)- e(x0,y0,0.,ext,tE);
          	    errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)- e(x0,y0,0.,eyt,tE);
          	    errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)- e(x0,y0,0.,hzt,tH);
        	  }
      	}
      	

      	J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
      	J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
      	J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
      	FOR_3(i1,i2,i3,J1,J2,J3)
      	{
        	  real x0 = XHP(i1,i2,i3,0);
        	  real y0 = XHP(i1,i2,i3,1);
        	  ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-e(x0,y0,0.,hz,tH);
      	}
            }
            else // 3D
            {
      	if( solveForElectricField ) 
                {
        	  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    real x0 = XEP(i1,i2,i3,0);
          	    real y0 = XEP(i1,i2,i3,1);
          	    real z0 = XEP(i1,i2,i3,2);
          	    ERREX(i1,i2,i3)=UEX(i1,i2,i3)-e(x0,y0,z0,ex,tE);
          	    ERREY(i1,i2,i3)=UEY(i1,i2,i3)-e(x0,y0,z0,ey,tE);
          	    ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-e(x0,y0,z0,ez,tE);
        	  }
        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      real x0 = XEP(i1,i2,i3,0);
            	      real y0 = XEP(i1,i2,i3,1);
            	      real z0 = XEP(i1,i2,i3,2);
            	      errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)- e(x0,y0,z0,ext,tE);
            	      errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)- e(x0,y0,z0,eyt,tE);
            	      errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)- e(x0,y0,z0,ezt,tE);
          	    }
        	  }

      	}

      	if( solveForMagneticField ) 
                {
        	  J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
        	  J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
        	  J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    real x0 = XHP(i1,i2,i3,0);
          	    real y0 = XHP(i1,i2,i3,1);
          	    real z0 = XHP(i1,i2,i3,2);
          	    ERRHX(i1,i2,i3)=UHX(i1,i2,i3)-e(x0,y0,z0,hx,tH);
          	    ERRHY(i1,i2,i3)=UHY(i1,i2,i3)-e(x0,y0,z0,hy,tH);
          	    ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-e(x0,y0,z0,hz,tH);
        	  }
        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      real x0 = XHP(i1,i2,i3,0);
            	      real y0 = XHP(i1,i2,i3,1);
            	      real z0 = XHP(i1,i2,i3,2);
            	      errLocal(i1,i2,i3,hxt) = uLocal(i1,i2,i3,hxt)- e(x0,y0,z0,hxt,tH);
            	      errLocal(i1,i2,i3,hyt) = uLocal(i1,i2,i3,hyt)- e(x0,y0,z0,hyt,tH);
            	      errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)- e(x0,y0,z0,hzt,tH);
          	    }
        	  }
      	}

            }

            if( debug & 4 ) 
            {
                display(erre(J1,J2,J3),sPrintF("getErrors: errE on grid %i at t=%e",grid,t),pDebugFile,"%9.2e "); 
                display(errh(J1,J2,J3),sPrintF("getErrors: errH on grid %i at t=%e",grid,t),pDebugFile,"%9.2e "); 
            }

        }
        else if( knownSolutionOption==gaussianPlaneWaveKnownSolution )
        {
            realSerialArray xei(Ie1,Ie2,Ie3),xhi(Ih1,Ih2,Ih3);
      //xi=kx*(x-x0GaussianPlaneWave)+ky*(y-y0GaussianPlaneWave) -cc*t;
            xei=kx*(xe(Ie1,Ie2,Ie3)-x0GaussianPlaneWave)+ky*(ye(Ie1,Ie2,Ie3)-y0GaussianPlaneWave) -cc*tE;
            xhi=kx*(xh(Ih1,Ih2,Ih3)-x0GaussianPlaneWave)+ky*(yh(Ih1,Ih2,Ih3)-y0GaussianPlaneWave) -cc*tH;

//             err(I1,I2,I3,hz)=hzGaussianPulse(xi);  // save Hz here temporarily

//             err(I1,I2,I3,ex)=u(I1,I2,I3,ex)-err(I1,I2,I3,hz)*(-ky/(eps*cc));
//             err(I1,I2,I3,ey)=u(I1,I2,I3,ey)-err(I1,I2,I3,hz)*( kx/(eps*cc));
// 	    err(I1,I2,I3,hz)-=u(I1,I2,I3,hz);
            realSerialArray hzei(Ie1,Ie2,Ie3);
            hzei = hzGaussianPulse(xei);
            erre(Ie1,Ie2,Ie3,ex)=ue(Ie1,Ie2,Ie3,ex)-hzei(Ie1,Ie2,Ie3)*(-ky/(eps*cc));
            erre(Ie1,Ie2,Ie3,ey)=ue(Ie1,Ie2,Ie3,ey)-hzei(Ie1,Ie2,Ie3)*( kx/(eps*cc));
            errh(Ih1,Ih2,Ih3,hz)=uh(Ih1,Ih2,Ih3,hz) - hzGaussianPulse(xhi(Ih1,Ih2,Ih3));
        }
        else if( knownSolutionOption==squareEigenfunctionKnownSolution )
        {
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
            }
          	    
            int i1,i2,i3;
            real xd,yd,zd;
            if( isRectangular )
            {
//  	      const real *up = u.Array_Descriptor.Array_View_Pointer3;
//  	      real *errp = err.Array_Descriptor.Array_View_Pointer3;
//  	      const int uDim0=u.getRawDataSize(0);
//  	      const int uDim1=u.getRawDataSize(1);
//  	      const int uDim2=u.getRawDataSize(2);
//                #undef U
//                #define U(i0,i1,i2,i3) up[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]
//                #undef ERR
//                #define ERR(i0,i1,i2,i3) errp[i0+uDim0*(i1+uDim1*(i2+uDim2*(i3)))]


      	if( numberOfDimensions==2 )
      	{
        	  Index J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
        	  Index J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
        	  Index J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    xd=X0(i1,i2,i3)-x0;
          	    yd=X1(i1,i2,i3)-y0;

          	    ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3) - cos(fx*xd)*cos(fy*yd)*cos(omega*tH);
        	  }

        	  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

        	  FOR_3(i1,i2,i3,J1,J2,J3)
        	  {
          	    xd=X0(i1,i2,i3)-x0;
          	    yd=X1(i1,i2,i3)-y0;
          	    ERREX(i1,i2,i3)=UEX(i1,i2,i3) - (-fy/omega)*cos(fx*xd)*sin(fy*yd)*sin(omega*tE);  // Ex.t = Hz.y
          	    ERREY(i1,i2,i3)=UEY(i1,i2,i3) - ( fx/omega)*sin(fx*xd)*cos(fy*yd)*sin(omega*tE);  // Ey.t = - Hz.x
        	  }

        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          	    FOR_3(i1,i2,i3,J1,J2,J3)
          	    {
            	      real xde=X0(i1,i2,i3)-x0;
            	      real yde=X1(i1,i2,i3)-y0;
	      // time derivatives: 
            	      errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - (-fy)*cos(fx*xde)*sin(fy*yde)*cos(omega*tE);  // Ex.t
            	      errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - ( fx)*sin(fx*xde)*cos(fy*yde)*cos(omega*tE);  // Ey.t
            	      errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt) - (-omega)*cos(fx*xde)*cos(fy*yde)*sin(omega*tH);  // Hz.t 
          	    }
        	  }
      	} 
      	else // 3D
      	{

        	  Index J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  Index J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  Index J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

        	  FOR_3D(i1,i2,i3,I1,I2,I3)
        	  {
          	    xd=X0(i1,i2,i3)-x0;
          	    yd=X1(i1,i2,i3)-y0;
          	    zd=X2(i1,i2,i3)-z0;

          	    ERREX(i1,i2,i3)=UEX(i1,i2,i3) -  (a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*cos(omega*tE);  // 
          	    ERREY(i1,i2,i3)=UEY(i1,i2,i3) -  (a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*cos(omega*tE);  // 
          	    ERREZ(i1,i2,i3)=UEZ(i1,i2,i3) -  (a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*cos(omega*tE);  // 
        	  }

        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          	    FOR_3(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=X0(i1,i2,i3)-x0;
            	      yd=X1(i1,i2,i3)-y0;
            	      zd=X2(i1,i2,i3)-z0;
	      // time derivatives: 
            	      errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - (-omega*a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*sin(omega*tE);
            	      errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - (-omega*a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*sin(omega*tE);
            	      errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt) - (-omega*a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*sin(omega*tE);
          	    }
        	  }
      	}
            }
            else // curvilinear 
            {
	// curvilinear
      	if( numberOfDimensions==2 )
      	{
        	  Index J1 = Range(max(Ih1.getBase(),uhl.getBase(0)),min(Ih1.getBound(),uhl.getBound(0)));
        	  Index J2 = Range(max(Ih2.getBase(),uhl.getBase(1)),min(Ih2.getBound(),uhl.getBound(1)));
        	  Index J3 = Range(max(Ih3.getBase(),uhl.getBase(2)),min(Ih3.getBound(),uhl.getBound(2)));

        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    xd=XHP(i1,i2,i3,0)-x0;
          	    yd=XHP(i1,i2,i3,1)-y0;
          	    ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3) - cos(fx*xd)*cos(fy*yd)*cos(omega*tH);
        	  }

        	  J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

        	  FOR_3(i1,i2,i3,J1,J2,J3)
        	  {
          	    xd=XEP(i1,i2,i3,0)-x0;
          	    yd=XEP(i1,i2,i3,1)-y0;
          	    ERREX(i1,i2,i3)=UEX(i1,i2,i3) - (-fy/omega)*cos(fx*xd)*sin(fy*yd)*sin(omega*tE);  // Ex.t = Hz.y
          	    ERREY(i1,i2,i3)=UEY(i1,i2,i3) - ( fx/omega)*sin(fx*xd)*cos(fy*yd)*sin(omega*tE);  // Ey.t = - Hz.x
        	  }

        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);
          	    FOR_3(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=XEP(i1,i2,i3,0)-x0;
            	      yd=XEP(i1,i2,i3,1)-y0;
	      // time derivatives: 
            	      errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - (-fy)*cos(fx*xd)*sin(fy*yd)*cos(omega*tE);  // Ex.t
            	      errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - ( fx)*sin(fx*xd)*cos(fy*yd)*cos(omega*tE);  // Ey.t
            	      errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt) - (-omega)*cos(fx*xd)*cos(fy*yd)*sin(omega*tH);  // Hz.t 
          	    }
        	  }

      	} 
      	else // 3D
      	{
        	  Index J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
        	  Index J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
        	  Index J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    xd=XEP(i1,i2,i3,0)-x0;
          	    yd=XEP(i1,i2,i3,1)-y0;
          	    zd=XEP(i1,i2,i3,2)-z0;

          	    ERREX(i1,i2,i3)=UEX(i1,i2,i3) -  (a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*cos(omega*tE);  // 
          	    ERREY(i1,i2,i3)=UEY(i1,i2,i3) -  (a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*cos(omega*tE);  // 
          	    ERREZ(i1,i2,i3)=UEZ(i1,i2,i3) -  (a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*cos(omega*tE);  // 
        	  }

        	  if( method==sosup )
        	  {
            // Compute errors in the time derivative
          	    realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);

          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      xd=XEP(i1,i2,i3,0)-x0;
            	      yd=XEP(i1,i2,i3,1)-y0;
            	      zd=XEP(i1,i2,i3,2)-z0;

            	      errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - (-omega*a1/fx)*cos(fx*xd)*sin(fy*yd)*sin(fz*zd)*sin(omega*tE);
            	      errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - (-omega*a2/fy)*sin(fx*xd)*cos(fy*yd)*sin(fz*zd)*sin(omega*tE);
            	      errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt) - (-omega*a3/fz)*sin(fx*xd)*sin(fy*yd)*cos(fz*zd)*sin(omega*tE);
          	    }
        	  }

      	}

            }

        }
        else if( knownSolutionOption==annulusEigenfunctionKnownSolution )
        {
      //kkc XXX not implemented for dsi schemes
            realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);

            Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)),min(I1.getBound(),uLocal.getBound(0)));
            Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)),min(I2.getBound(),uLocal.getBound(1)));
            Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)),min(I3.getBound(),uLocal.getBound(2)));

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
                          ERRHZ(i1,i2,i3) = UHZ(i1,i2,i3) -bj*cosn*cos(omega*t);
                          ERREX(i1,i2,i3) = UEX(i1,i2,i3) - uex*sin(omega*t);  // Ex.t = Hz.y
                          ERREY(i1,i2,i3) = UEY(i1,i2,i3) - uey*sin(omega*t);  // Ey.t = - Hz.x
                          if( method==sosup )
                          {
                              errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt) + omega*bj*cosn*sint;
                              errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) - omega*uex*cost;
                              errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) - omega*uey*cost;
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
                          ERREX(i1,i2,i3) = UEX(i1,i2,i3) - uex*sinkz*cost;
                          ERREY(i1,i2,i3) = UEY(i1,i2,i3) - uey*sinkz*cost;
                          ERREZ(i1,i2,i3) = UEZ(i1,i2,i3) - bj*cosn*coskz*cost;
                          if( method==sosup )
                          {
                              sint=sin(omega*t); 
                              errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext) + omega*uex*sinkz*sint;
                              errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt) + omega*uey*sinkz*sint;
                              errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt) + omega*bj*cosn*coskz*sint;
                          }
                }
              }
                        
        }
        else if( knownSolutionOption==planeMaterialInterfaceKnownSolution )
        {
            if( method==nfdtd || method==sosup )
            { 
                realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);

	// adjust array dimensions for local arrays
      	Index J1 = Range(max(I1.getBase(),uel.getBase(0)),min(I1.getBound(),uel.getBound(0)));
      	Index J2 = Range(max(I2.getBase(),uel.getBase(1)),min(I2.getBound(),uel.getBound(1)));
      	Index J3 = Range(max(I3.getBase(),uel.getBase(2)),min(I3.getBound(),uel.getBound(2)));

      // ------------ macro for the plane material interface -------------------------
      // error: initialCondition, error, boundaryCondition
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
                          ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
                          ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
                          ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
                          if( method==sosup )
                          {
                   	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-pmct*(pmc[0]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-pmct*(pmc[2]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)-pmct*(pmc[10]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[11]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
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
                          ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
                          ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
                          ERRHZ(i1,i2,i3)=UHZ(i1,i2,i3)-u3;
                          if( method==sosup )
                          {
                   	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-(pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-(pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)-(pmct*pmc[17]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
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
                          ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
                          ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
                          ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
                          if( method==sosup )
                          {
                   	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-pmct*(pmc[0]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[1]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-pmct*(pmc[2]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[3]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)-pmct*(pmc[4]*sin(twoPi*(pmc[19]*(x-pmc[28])+pmc[20]*(y-pmc[29])+pmc[21]*(z-pmc[30])-pmc[18]*(t)))+pmc[5]*sin(twoPi*(pmc[22]*(x-pmc[28])+pmc[23]*(y-pmc[29])+pmc[24]*(z-pmc[30])-pmc[18]*(t))));
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
                          ERREX(i1,i2,i3)=UEX(i1,i2,i3)-u1;
                          ERREY(i1,i2,i3)=UEY(i1,i2,i3)-u2;
                          ERREZ(i1,i2,i3)=UEZ(i1,i2,i3)-u3;
                          if( method==sosup )
                          {
                   	 errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-(pmct*pmc[12]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-(pmct*pmc[13]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                   	 errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)-(pmct*pmc[14]*sin(twoPi*(pmc[25]*(x-pmc[28])+pmc[26]*(y-pmc[29])+pmc[27]*(z-pmc[30])-pmc[18]*(t))));
                          } 
                    }
                  }
                }
            }
            else
            {
      	printF("MX:getErrors: ERROR: initialConditionOption==planeMaterialInterfaceInitialCondition but method=%i\n",
             	       (int)method);
      	OV_ABORT("ERROR");
            }
        }
        else if( knownSolutionOption==gaussianIntegralKnownSolution )
        {
          	    
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
                double time=tE;
                int i1,i2,i3;
                FOR_3D(i1,i2,i3,J1,J2,J3)
                {
                    double x=X(i1,i2,i3,0); 
                    double y=X(i1,i2,i3,1);
                    exmax(wt,wx,wy,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,time);
                        ERREX(i1,i2,i3) = UEX(i1,i2,i3) - wy;
                        ERREY(i1,i2,i3) = UEY(i1,i2,i3) + wx;
                        ERRHZ(i1,i2,i3) = UHZ(i1,i2,i3) - wt;
                }
            }

          	    
        }
        else if( compareToReferenceShowFile )
        {
      //kkc XXX not implemented for dsi schemes

            assert( uReference!=NULL );
          	    
            realMappedGridFunction & ur = (*uReference)[grid];
          	    
#ifdef USE_PPP
            realSerialArray urLocal;  getLocalArrayWithGhostBoundaries(ur,urLocal);
#else
            const realSerialArray & urLocal  =  ur;
#endif

      //            err(I1,I2,I3,C)=fabs(u(I1,I2,I3,C)-ur(I1,I2,I3,C));
            Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
            errh(Ih1,Ih2,Ih3,Ch)=uh(I1,I2,I3,Ch)-urLocal(I1,I2,I3,Ch);
            Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);
            erre(Ie1,Ie2,Ie3,Ce)=ue(Ie1,Ie2,Ie3,Ce) - urLocal(Ie1,Ie2,Ie3,Ce);
        }

//     else if (forcingOption==planeWaveBoundaryForcing ||
// 	     initialConditionOption==planeWaveScatteredFieldInitialCondition )
        else if( knownSolutionOption==scatteringFromADiskKnownSolution ||
                          knownSolutionOption==scatteringFromADielectricDiskKnownSolution ||
                          knownSolutionOption==scatteringFromASphereKnownSolution ||
                          knownSolutionOption==scatteringFromADielectricSphereKnownSolution )
        {
      //kkc XXX not implemented for dsi schemes

            const real cc0= cGrid(0)*sqrt( kx*kx+ky*ky ); // NOTE: use grid 0 values for multi-materials

            if( knownSolution==NULL )
            {
      	initializeKnownSolution();
            }
            const realArray & ug = (*knownSolution)[grid];
            realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);

      // The analytic solution assumed incident field was Ei = exp(i*k*x-i*w*t) 
      //     This gives solution
      //           Re(E)*cos(w*t) - Im(E)*sin(w*t) for Ei=cos(w*t)
      //      or   Re(E)*cos(w*t-pi/2) - Im(E)*sin(w*t-pi/2) for Ei=cos(w*t-pi/2)               
      //      i.e. Re(E)*sin(w*t) + Im(E)*cos(w*t) for Ei=sin(w*t)
      // Ex:
            Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
            Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);

#ifdef USE_PPP
            const realSerialArray & ugLocal = ug.getLocalArrayWithGhostBoundaries();
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

	// *wdh* 050731 real cost = cos(-twoPi*cc*tH);  
	// *wdh* 050731 real sint = sin(-twoPi*cc*tH);
      	const real cost = cos(-twoPi*cc0*tH);  // *wdh* 050731 -- use cc0 
      	const real sint = sin(-twoPi*cc0*tH);
      	const real dcost =  twoPi*cc0*sint;  // d(sin(..))/dt 
      	const real dsint = -twoPi*cc0*cost;  // d(sin(..))/dt 

	// adjust array dimensions for local arrays
      	Index J1 = Range(max(Ie1.getBase(),uel.getBase(0)),min(Ie1.getBound(),uel.getBound(0)));
      	Index J2 = Range(max(Ie2.getBase(),uel.getBase(1)),min(Ie2.getBound(),uel.getBound(1)));
      	Index J3 = Range(max(Ie3.getBase(),uel.getBase(2)),min(Ie3.getBound(),uel.getBound(2)));

      	int i1,i2,i3;
      	if( numberOfDimensions==2 )
      	{
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    ERREX(i1,i2,i3) = UEX(i1,i2,i3)-(UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost);
          	    ERREY(i1,i2,i3) = UEY(i1,i2,i3)-(UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost);
          	    ERRHZ(i1,i2,i3) = UHZ(i1,i2,i3)-(UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost);
          	    if( method==sosup )
          	    { // errors in time derivatives:
                            errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-(UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost);
                            errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-(UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost);
                            errLocal(i1,i2,i3,hzt) = uLocal(i1,i2,i3,hzt)-(UG(i1,i2,i3,hz)*dsint+UG(i1,i2,i3,hz+3)*dcost);
          	    }
          	    
        	  }
      	}
      	else 
      	{
        	  if( solveForElectricField )
        	  {
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      ERREX(i1,i2,i3) = UEX(i1,i2,i3)-(UG(i1,i2,i3,ex)*sint+UG(i1,i2,i3,ex+3)*cost);
            	      ERREY(i1,i2,i3) = UEY(i1,i2,i3)-(UG(i1,i2,i3,ey)*sint+UG(i1,i2,i3,ey+3)*cost);
            	      ERREZ(i1,i2,i3) = UEZ(i1,i2,i3)-(UG(i1,i2,i3,ez)*sint+UG(i1,i2,i3,ez+3)*cost);
            	      if( method==sosup )
            	      { // errors in time derivatives:
            		errLocal(i1,i2,i3,ext) = uLocal(i1,i2,i3,ext)-(UG(i1,i2,i3,ex)*dsint+UG(i1,i2,i3,ex+3)*dcost);
            		errLocal(i1,i2,i3,eyt) = uLocal(i1,i2,i3,eyt)-(UG(i1,i2,i3,ey)*dsint+UG(i1,i2,i3,ey+3)*dcost);
            		errLocal(i1,i2,i3,ezt) = uLocal(i1,i2,i3,ezt)-(UG(i1,i2,i3,ez)*dsint+UG(i1,i2,i3,ez+3)*dcost);
            	      }
          	    }
        	  }
        	  if( solveForMagneticField )
        	  {
          	    FOR_3D(i1,i2,i3,J1,J2,J3)
          	    {
            	      ERRHX(i1,i2,i3) = UHX(i1,i2,i3)-(UG(i1,i2,i3,hx)*sint+UG(i1,i2,i3,hx+3)*cost);
            	      ERRHY(i1,i2,i3) = UHY(i1,i2,i3)-(UG(i1,i2,i3,hy)*sint+UG(i1,i2,i3,hy+3)*cost);
            	      ERRHZ(i1,i2,i3) = UHZ(i1,i2,i3)-(UG(i1,i2,i3,hz)*sint+UG(i1,i2,i3,hz+3)*cost);
          	    }
        	  }
      	}
#undef UG
            	      
            }
            else
            {
      	real cost = cos(-twoPi*cc*tH);  // *wdh* 040626 add "-"
      	real sint = sin(-twoPi*cc*tH);
      	errh(Ih1,Ih2,Ih3,Ch)=uh(Ih1,Ih2,Ih3,Ch) - (ugLocal(Ih1,Ih2,Ih3,Ch)*sint+ugLocal(Ih1,Ih2,Ih3,Ch+3)*cost);

      	cost = cos(-twoPi*cc*tE);  // *wdh* 040626 add "-"
      	sint = sin(-twoPi*cc*tE);  // *wdh* 040626 add "-"
      	erre(Ie1,Ie2,Ie3,Ce)=ue(Ie1,Ie2,Ie3,Ce) - (ugLocal(Ie1,Ie2,Ie3,Ce)*sint+ugLocal(Ie1,Ie2,Ie3,Ce+3)*cost);
          	    
//              // err(I1,I2,I3,C)=fabs( u(I1,I2,I3,C) - (ug(I1,I2,I3,C)*sint+ug(I1,I2,I3,C+3)*cost) );
//              // ok err(I1,I2,I3,hz)=fabs( u(I1,I2,I3,hz) - (-ug(I1,I2,I3,hz)*sint)+ug(I1,I2,I3,hz+3)*cost));

//              err(I1,I2,I3,ex)=fabs( u(I1,I2,I3,ex) + (ug(I1,I2,I3,ex)*sint+ug(I1,I2,I3,ex+3)*cost) );
//              err(I1,I2,I3,ey)=fabs( u(I1,I2,I3,ey) + (ug(I1,I2,I3,ey)*sint+ug(I1,I2,I3,ey+3)*cost) );
//              // ok err(I1,I2,I3,ey)=fabs( u(I1,I2,I3,ey) - (-ug(I1,I2,I3,ey+3)*cost) );

//              err(I1,I2,I3,hz)=fabs( u(I1,I2,I3,hz) - (ug(I1,I2,I3,hz)*sint+ug(I1,I2,I3,hz+3)*cost));
            }
        }
        else if( knownSolutionOption==userDefinedKnownSolution )
        {
            realCompositeGridFunction & cgerr = (*cgerrp);
            realSerialArray errLocal; getLocalArrayWithGhostBoundaries((*cgerrp)[grid],errLocal);      

      // save exact solution in cgrerr: 
            int numberOfTimeDerivatives=0;
            getUserDefinedKnownSolution(   t, cg,grid, cgerr[grid],I1,I2,I3,numberOfTimeDerivatives);

            errLocal(I1,I2,I3,C) = errLocal(I1,I2,I3,C) -  uLocal(I1,I2,I3,C);

        }
        else if( knownSolutionOption!=noKnownSolution )
        {
            printF("Maxwell::getErrors: unexpected value for knownSolutionOption=%i\n",(int)knownSolutionOption);
            Overture::abort("Maxwell::getErrors");
        }
        else
        {
            energyOnly = true;
        }

        getIndex(mg.gridIndexRange(),I1,I2,I3);
        RealArray errMax(C);
        errMax=0.;  // max error on this grid
      	
        if ( method==dsiMatVec && !energyOnly )
        { // punt here and use a special function that knows how to exclude uns. ghost points
            #ifdef USE_PPP
                Overture::abort("Error: finish me");
            #else
                computeDSIErrors( *this,mg, uh, uhpp, ue, uepp, errh, erre, solutionNorm, maximumError );
            #endif
        }
        else if( radiusForCheckingErrors<= 0. && (method==nfdtd || method==sosup) && !energyOnly) 
        {
            Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
            Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);
            if( true )
            {
      	const int ng=orderOfAccuracyInSpace/2;
      	const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
  
      	Index J1 = Range(max(I1.getBase(),uel.getBase(0)+ng ),min(I1.getBound(),uel.getBound(0)-ng ));
      	Index J2 = Range(max(I2.getBase(),uel.getBase(1)+ng ),min(I2.getBound(),uel.getBound(1)-ng ));
      	Index J3 = Range(max(I3.getBase(),uel.getBase(2)+ng3),min(I3.getBound(),uel.getBound(2)-ng3));

      	int i1,i2,i3;
      	FOR_3D(i1,i2,i3,J1,J2,J3)
      	{
        	  if( MASK(i1,i2,i3)!=0 )
        	  {
          	    for( int c=C.getBase(); c<=C.getBound(); c++ )
          	    {
#undef ERR
#define ERR(i0,i1,i2,i3) errep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]
#undef U
#define U(i0,i1,i2,i3) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]

            	      errMax(c)=max(errMax(c),fabs(ERR(i1,i2,i3,c)));           // this is the max err on this grid
            	      solutionNorm(c)=max(solutionNorm(c),fabs(U(i1,i2,i3,c)));
#undef ERR
#undef U	    
          	    }
        	  }
      	}
      	if( debug & 2 )
      	{
        	  fprintf(pDebugFile," *** Max errors on this processor for grid %i at t=%8.2e: ",grid,t);
        	  for( int c=C.getBase(); c<=C.getBound(); c++ )
          	    fprintf(pDebugFile,"%10.4e,",errMax(c));
                    fprintf(pDebugFile,"\n");
      	}
      	for( int c=C.getBase(); c<=C.getBound(); c++ )
      	{
// 	  errMax(c)=getMaxValue(errMax(c));
// 	  solutionNorm(c)=getMaxValue(solutionNorm(c));
            	      
        	  maximumError(c)=max(maximumError(c),errMax(c));  // max error over all grids
      	}
          	    
            }
        }
        else // check inside radius
        {
      // printF(" Check errors within the sphere of radius %10.2e\n",radiusForCheckingErrors);
        	  
      // new way
            const int ng=orderOfAccuracyInSpace/2;
            const int ng3 = mg.numberOfDimensions()==2 ? 0 : ng;
  
            Index J1 = Range(max(I1.getBase(),uLocal.getBase(0)+ng ),min(I1.getBound(),uLocal.getBound(0)-ng ));
            Index J2 = Range(max(I2.getBase(),uLocal.getBase(1)+ng ),min(I2.getBound(),uLocal.getBound(1)-ng ));
            Index J3 = Range(max(I3.getBase(),uLocal.getBase(2)+ng3),min(I3.getBound(),uLocal.getBound(2)-ng3));

            Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
            Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);
          	    
            if ( method==nfdtd )
            {
      	const real radiusForCheckingErrorsSquared=SQR(radiusForCheckingErrors);
            		
      	real radius;
      	int i1,i2,i3;
#undef ERR
#define ERR(i0,i1,i2,i3) errep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]
#undef U
#define U(i0,i1,i2,i3) uep[i0+ueDim0*(i1+ueDim1*(i2+ueDim2*(i3)))]

      	if( isRectangular )
      	{
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    radius = SQR(X0(i1,i2,i3))+SQR(X1(i1,i2,i3));
          	    if( numberOfDimensions==3 ) radius+=SQR(X2(i1,i2,i3));
                  			
          	    if( radius<radiusForCheckingErrorsSquared && MASK(i1,i2,i3)!=0 )
          	    {
            	      for( int c=C.getBase(); c<=C.getBound(); c++ )
            	      {
            		errMax(c)=max(errMax(c),fabs(ERR(i1,i2,i3,c)));           // this is the max err on this grid
            		solutionNorm(c)=max(solutionNorm(c),fabs(U(i1,i2,i3,c)));
                        				
            	      }
          	    }
          	    else
          	    {
            	      for( int c=C.getBase(); c<=C.getBound(); c++ )
            		ERR(i1,i2,i3,c)=0.;
          	    }
                  			
        	  }
      	}
      	else // curvilinear
      	{
        	  FOR_3D(i1,i2,i3,J1,J2,J3)
        	  {
          	    radius = SQR(X(i1,i2,i3,0))+SQR(X(i1,i2,i3,1));
          	    if( numberOfDimensions==3 ) radius+=SQR(X(i1,i2,i3,2));
                  			
          	    if( radius<radiusForCheckingErrorsSquared && MASK(i1,i2,i3)!=0 )
          	    {
            	      for( int c=C.getBase(); c<=C.getBound(); c++ )
            	      {
            		errMax(c)=max(errMax(c),fabs(ERR(i1,i2,i3,c)));           // this is the max err on this grid
            		solutionNorm(c)=max(solutionNorm(c),fabs(U(i1,i2,i3,c)));
                        				
            	      }
          	    }
          	    else
          	    {
            	      for( int c=C.getBase(); c<=C.getBound(); c++ )
            		ERR(i1,i2,i3,c)=0.;
          	    }
                  			
        	  }
      	}
#undef ERR
#undef U		
      	if( debug & 2 )
      	{
        	  fprintf(pDebugFile," *** Max errors on this processor for grid %i at t=%8.2e: ",grid,t);
        	  for( int c=C.getBase(); c<=C.getBound(); c++ )
          	    fprintf(pDebugFile,"%10.4e,",errMax(c));
                    fprintf(pDebugFile,"\n");
      	}
      	
      	
        	for( int c=C.getBase(); c<=C.getBound(); c++ )
        	{
// 	  errMax(c)=getMaxValue(errMax(c));
// 	  solutionNorm(c)=getMaxValue(solutionNorm(c));
                		    
          	  maximumError(c)=max(maximumError(c),errMax(c));  // max error over all grids
        	}
            }
        } // end else if inside radius

        bool computeErrorsAtGhost=true;
#ifdef USE_PPP
        computeErrorsAtGhost=false;
#endif
        if( computeErrorsAtGhost && !usePML && (method==nfdtd || method==sosup) )
        {
            	      
      // compute error including ghost points
      // *** this is wrong ==> only check ghost points where mask on boundary !=0 
            realMappedGridFunction & err = mgp==NULL ? (*cgerrp)[grid] : *errp;
            #ifdef USE_PPP
                realSerialArray errLocal;  getLocalArrayWithGhostBoundaries(err,errLocal);
            #else
                const realSerialArray & errLocal  =  err;
            #endif	    
            
            	      
            RealArray ghostError(C,Range(1,numberOfGhost));
            ghostError=0.;
            int c,ghost;
            Index Ig1,Ig2,Ig3;
            for( int axis=0; axis<mg.numberOfDimensions(); axis++)
            {
      	for( int side=0; side<=1; side++ )
      	{
        	  getBoundaryIndex(mg.gridIndexRange(),side,axis,I1,I2,I3);
                    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3);
                    if( !ok ) continue;
        	  for( ghost=1; ghost<=numberOfGhost; ghost++ )
        	  {
          	    getGhostIndex(mg.gridIndexRange(),side,axis,Ig1,Ig2,Ig3,ghost);
          	    bool ok = ParallelUtility::getLocalArrayBounds(mask,maskLocal,Ig1,Ig2,Ig3);
          	    if( !ok ) continue;

          	    Index Ch = cg.numberOfDimensions()==2 ? Range(hz,hz) : Range(hx,hz);
          	    Index Ce = cg.numberOfDimensions()==2 ? Range(ex,ey) : Range(ex,ez);
                  			
          	    where( maskLocal(I1,I2,I3)!=0 && maskLocal(Ig1,Ig2,Ig3)!=0 )
          	    {
            	      for( c=C.getBase(); c<=C.getBound(); c++ )
            		ghostError(c,ghost)=max(ghostError(c,ghost),max(fabs(errLocal(Ig1,Ig2,Ig3,c))));
                      			    
            	      for( c=C.getBase(); c<=C.getBound(); c++ )
            	      {
            		if( max(fabs(errLocal(Ig1,Ig2,Ig3,c)))>1.e+1 )
            		{
              		  fprintf(pDebugFile," *** grid=%i side,axis=%i,%i ghost=%i c=%i ****\n",grid,side,axis,ghost,c);
              		  display(errLocal(Ig1,Ig2,Ig3,c),"ERROR on the ghost line",pDebugFile);
            		}
            	      }
                      			    
          	    }
        	  }
      	}
            }

            if( debug & 2 )
            {
      	for( ghost=1; ghost<=numberOfGhost; ghost++ )
      	{
        	  printF(" t=%9.3e: grid=%i: Errors at ghost line %i: ",t,grid,ghost);
        	  for( c=C.getBase(); c<=C.getBound(); c++ )
          	    printF("%8.2e, ",ghostError(c,ghost));
        	  printF("\n");
      	}
            }
            
        } // end if compute error at ghost

    
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
    

    for( int c=C.getBase(); c<=C.getBound(); c++ )
    {
        solutionNorm(c)=getMaxValue(solutionNorm(c));
        maximumError(c)=getMaxValue(maximumError(c));
    }

    if( debug & 2 )
    {
        fprintf(pDebugFile,"\n --> t=%10.4e dt=%7.1e Errors: ",t,dt);
        for( int c=C.getBase(); c<=C.getBound(); c++ )
            fprintf(pDebugFile,"%10.4e,",maximumError(c));
        fprintf(pDebugFile,"\n");
    }

    if( method==nfdtd || method==yee || method==sosup )
    {
        realCompositeGridFunction & cgerr = *cgerrp;
        realCompositeGridFunction & cgu = cgfields[current];

    // We print the max norm and optionally some lp norms
    // const int errorNorm = parameters.dbase.get<int >("errorNorm");
        int numberOfNormsToPrint=1;
        if( errorNorm<10000 ) numberOfNormsToPrint+=errorNorm;

        for( int norm=0; norm<numberOfNormsToPrint; norm++ )
        { // norm==0 : max-norm, otherwise Lp-norm with p=norm
            int pNorm = norm==0 ? INT_MAX : norm;

            if( norm!=0 ) // max-norm values are already computed -- we could avoid doing this above --
            {
	// compute the Lp norm
      	maximumError=0.;
      	for( int c=C.getBase(); c<=C.getBound(); c++ )
      	{
        	  const int maskOption=0;  // check points where mask != 0
                    const int checkErrorsAtGhostPoints=0;
        	  if( pNorm<10000 )
        	  {
          	    maximumError(c)=lpNorm(pNorm,cgerr,c,maskOption,checkErrorsAtGhostPoints);
          	    solutionNorm(c)=lpNorm(pNorm,cgu  ,c,maskOption,checkErrorsAtGhostPoints);
        	  }
        	  else
        	  { // assume this is the max-norm
          	    maximumError(c)=maxNorm(cgerr,c,maskOption,checkErrorsAtGhostPoints);
          	    solutionNorm(c)=maxNorm(cgu  ,c,maskOption,checkErrorsAtGhostPoints);
        	  }
      	}
            }
            
            aString normName;
            if( pNorm<1000 )
      	sPrintF(normName,"l%i",pNorm);
            else
      	normName="max";

            for( int fileio=0; fileio<2; fileio++ )
            {
      	FILE *output = fileio==0 ? logFile : stdout;

      	if( radiusForCheckingErrors>0 && radiusForCheckingErrors<10. )
        	  fPrintF(output,"                t=%8.2e dt=%7.1e %s errors(r=%3.2f):[",
                                      t,dt,radiusForCheckingErrors,(const char*)normName);
      	else
        	  fPrintF(output,">>> t=%8.2e dt=%7.1e %s errors:[",t,dt,(const char*)normName);

      	for( int c=C.getBase(); c<=C.getBound(); c++ )
        	  fPrintF(output,"%8.2e,",maximumError(c));
	// fPrintF(output,"%10.4e,",maximumError(c));

      	fPrintF(output,"], %s (u):[",(const char*)normName);

      	for( int c=C.getBase(); c<=C.getBound(); c++ )
        	  fPrintF(output,"%8.2e,",solutionNorm(c));

      	fPrintF(output,"] (%i steps)\n",numberOfStepsTaken);
            }
        } // end for norm 
    }
    
    timing(timeForGetError)+=getCPU()-time0;
}

