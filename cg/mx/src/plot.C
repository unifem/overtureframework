// This file automatically generated from plot.bC with bpp.
//#define BOUNDS_CHECK
//#define OV_DEBUG

#include "Maxwell.h"
#include "PlotStuff.h"
#include "GL_GraphicsInterface.h"
#include "DialogData.h"
#include "UnstructuredMapping.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "display.h"

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

static int numberOfPushButtons=0, numberOfTextBoxes=0;

//! Convert specified components of u (cellCentered of faceCentered) to vertexCentered values in v.
int
convertToVertexCentered( const realMappedGridFunction & u, const Range & Ru, 
                                                  realMappedGridFunction & v, const Range & Rv, bool plotDSIMaxVertVals = false )
{
    MappedGrid & mg = *(u.getMappedGrid());

    assert( mg.getGridType()==MappedGrid::unstructuredGrid );
    
    assert( Ru.getLength()==Rv.getLength() );

    UnstructuredMapping & map = (UnstructuredMapping &) mg.mapping().getMapping();
          	    
    const realArray & x = map.getNodes();
    const int numberOfNodes = map.getNumberOfNodes();
    const int numberOfElements = map.getNumberOfElements();
    const intArray & element = map.getElements();

    const realArray & uu = u;
    realArray & vv = v;
    
    const int numberOfComponents=Ru.getLength();
    const int cu0=Ru.getBase();
    const int cv0=Rv.getBase();
    int c;

    Range all;
    for( c=0; c<numberOfComponents; c++ )
        vv(all,0,0,c+cv0)=0.;

    UnstructuredMapping::EntityTypeEnum centering;
    if ( mg.numberOfDimensions()==2 )
        {
            centering = u.getGridFunctionType()==GridFunctionParameters::cellCentered ? UnstructuredMapping::Face : UnstructuredMapping::Edge;
        }
    else
        {
            centering = u.getGridFunctionType()==GridFunctionParameters::faceCenteredAll ? UnstructuredMapping::Face : UnstructuredMapping::Edge;
        }

    IntegerArray numPerNode(numberOfNodes);
    numPerNode = 0;

    UnstructuredMappingIterator citer, citer_end;
    UnstructuredMappingAdjacencyIterator viter, viter_end;
    
    citer_end = map.end(centering);
    real minEx=REAL_MAX, minEy=REAL_MAX, maxEx=-REAL_MAX, maxEy=-REAL_MAX;
    for ( citer=map.begin(centering); citer!=citer_end; citer++ )
    {
        int e=*citer;
        viter_end = map.adjacency_end(citer, UnstructuredMapping::Vertex);
        for ( viter=map.adjacency_begin(citer,UnstructuredMapping::Vertex);
        	  viter!=viter_end; viter++ )
        {
            int nn = *viter;
            for( c=0; c<numberOfComponents; c++ )
      	if ( plotDSIMaxVertVals )
      	{
        	  if ( fabs(uu(e,0,0,c+cu0)) > fabs(vv(nn,0,0,c+cv0)) )
          	    vv(nn,0,0,c+cv0) = uu(e,0,0,c+cu0);
      	}
      	else
        	  vv(nn,0,0,c+cv0)+=uu(e,0,0,c+cu0);
            numPerNode(nn)+=1;
        }

        if ( !citer.isGhost() )
        {
        	  
            minEx = min(minEx, uu(e,0,0,0));
            maxEx = max(maxEx, uu(e,0,0,0));
            if ( numberOfComponents==2 )
            {
      	minEy = min(minEy, uu(e,0,0,1));
      	maxEy = max(maxEy, uu(e,0,0,1));
            }

        }
    }
//   cout<<"min/max Ex "<<minEx<<"  "<<maxEx<<endl;
//   cout<<"min/max Ey "<<minEy<<"  "<<maxEy<<endl;

    int minADJ=1000;
    int maxADJ=0;
    for( int n=0; n<numberOfNodes && !plotDSIMaxVertVals; n++ )
    {
        for( c=0; c<numberOfComponents; c++ )
            if ( numPerNode(n) ) vv(n,0,0,c+cv0)/=real(numPerNode(n));
        minADJ = min(minADJ,numPerNode(n));
        maxADJ = max(maxADJ,numPerNode(n));
    }
  //    cout<<"min/max adj "<<minADJ<<"  "<<maxADJ<<endl<<"---"<<endl;

//   if( u.getGridFunctionType()==GridFunctionParameters::cellCentered )
//   {
//     // *** this computation should be done once in the calling function ****  fix this ****

//     // printF(" **** convertToVertexCentered: cell centred grid function found! ****\n");
      	
//     // grid function is cell centered!
//     // For now make a node centered values for plotting by averaging.

//     IntegerArray numElementPerNode(numberOfNodes);
//     numElementPerNode=0;
      	
// //     int e;
// //     for( e=0; e<numberOfElements; e++ )
// //     {
// //       int numNodes = map.getNumberOfNodesThisElement(e);
// //       for( int n=0; n<numNodes; n++ )
// //       {
// // 	int nn=element(e,n);
// //         for( c=0; c<numberOfComponents; c++ )
// //   	  vv(nn,0,0,c+cv0)+=uu(e,0,0,c+cu0);
// // 	numElementPerNode(nn)+=1;
// //       }
// //     }

//     UnstructuredMappingIterator eiter,eiter_end;
//     UnstructuredMappingAdjacencyIterator vert, vert_end;
//     eiter_end = map.end(UnstructuredMapping::Face);
//     for ( eiter=map.begin(UnstructuredMapping::Face); eiter!=eiter_end; eiter++ )
//       {
// 	int e = *eiter;
// 	vert_end = map.adjacency_end(eiter,UnstructuredMapping::Vertex);
// 	for ( vert=map.adjacency_begin(eiter,UnstructuredMapping::Vertex); vert!=vert_end; vert++ )
// 	  {
// 	    int nn = *vert;
// 	    for( c=0; c<numberOfComponents; c++ )
// 	      vv(nn,0,0,c+cv0)+=uu(e,0,0,c+cu0);
// 	    numElementPerNode(nn)+=1;
// 	  }
//       }

//     for( int n=0; n<numberOfNodes; n++ )
//     {
//       for( c=0; c<numberOfComponents; c++ )
//         vv(n,0,0,c+cv0)/=numElementPerNode(n);
//     }

//   }
//   else if( u.getGridFunctionType()==GridFunctionParameters::faceCenteredAll )
//   {
//     // printF(" **** convertToVertexCentered: FACE centred grid function found! ****\n");
      	
//     // grid function is cell centered!
//     // For now make a node centered values for plotting by averaging.
      	
//     IntegerArray numFacePerNode(numberOfNodes);
//     numFacePerNode=0;
      	
//     const int numberOfFaces=map.getNumberOfFaces();
//     const intArray & faces = map.getFaces();
      	
//     // **** this average will not work very well at boundaries *****

//     int f;
//     for( f=0; f<numberOfFaces; f++ )
//     {
//       int numNodes = map.getNumberOfNodesThisFace(f); // this should be 2
//       assert( numNodes==2 );
//       for( int n=0; n<numNodes; n++ )
//       {
// 	int nn=faces(f,n);
//         for( c=0; c<numberOfComponents; c++ )
//   	  vv(nn,0,0,c+cv0)+=uu(f,0,0,c+cu0);
// 	numFacePerNode(nn)+=1;
//       }
//     }
//     for( int n=0; n<numberOfNodes; n++ )
//     {
//       for( c=0; c<numberOfComponents; c++ )
// 	vv(n,0,0,c+cv0)/=numFacePerNode(n);
//     }
//   }
//   else
//   {
//     throw "error";
//   }
    
    return 0;
    
}



// =============================================================================================
/// \brief Create a grid function that holds all the things we can plot.
/// \param t (input) : if t<0 then only fill the component names into the grid function v.
// =============================================================================================
realCompositeGridFunction& Maxwell::
getAugmentedSolution(int current, realCompositeGridFunction & v, const real t)
{
    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;
    const int numberOfDimensions = cg.numberOfDimensions();
    
  // const int numberOfComponents=mgp==NULL ? cgfields[current][0].getLength(3) : fields[current].getLength(3);
    int numberOfComponents;
    if( mgp==NULL )
    {
        if( method!=dsiMatVec )
        {
            numberOfComponents=getCGField(HField,0)[0].getLength(3);
            if ( method!=nfdtd  && method!=yee && method!=sosup )
      	numberOfComponents+=getCGField(EField,0)[0].getLength(3);
        }
        else
            numberOfComponents=cg.numberOfDimensions()==2 ? 3 : 6;
        
    }
    else
    {
        MappedGrid & mg = *mgp;
        if( mg.getGridType()==MappedGrid::structuredGrid )
        {
            numberOfComponents=fields[current].getLength(3);

            if( method!=nfdtd && method!=yee && method!=sosup )
      	numberOfComponents += fields[current+numberOfTimeLevels].getLength(3);
        }
        else
        {
      // unstructured grid
            numberOfComponents=mg.numberOfDimensions()==2 ? 3 : 6;
        }
    }
    

    const bool saveErrors = plotErrors && !(errp==NULL && cgerrp==NULL);
    const bool saveDissipation =  plotDissipation && (( (artificialDissipation>0. || artificialDissipationCurvilinear>0.) 
                                                                && (method==nfdtd)) || dissipation || cgdissipation);

    Range all;
//    if( !saveErrors && !saveDissipation && !plotDivergence && !plotScatteredField )
//    {
//      if( mgp!=NULL )
//      {
//        realMappedGridFunction & u = fields[current];
//        v.updateToMatchGrid(cg,all,all,all,numberOfComponents);
//        for( int n=0; n<numberOfComponents; n++ )
//  	v.setName(fields[current].getName(n),n);

//        v[0]=fields[current];
//        return v;
//      }
//      else
//      {
//        return cgfields[current];
//      }
//    }

    const bool saveDsiDiss= saveDissipation && method==dsiMatVec && cg.numberOfDimensions()==3;
    
  // Determine the number of components to plot and the component numbers for the errors, etc.
  //    nErr : component where the error is stored
  //    ndd  : component where the dissipation is stored
    int numberToPlot=numberOfComponents;                  // save fields
    int nErr=numberToPlot;    numberToPlot += numberOfComponents*int(saveErrors);
    int ndd=numberToPlot;     numberToPlot += numberOfComponents*int(saveDissipation);
                                                        numberToPlot += cg.numberOfDimensions()*int( saveDsiDiss ); 
    int nVarDis=numberToPlot; numberToPlot += int(useVariableDissipation);
    int nDivE=numberToPlot;   numberToPlot += int(plotDivergence); 
    int nDivH=-1;
    if( plotDivergence && (method==yee || solveForMagneticField ) && numberOfDimensions==3 )
    { // plot div(H) too
        nDivH=numberToPlot;  numberToPlot +=1;
    }
    if( method!=nfdtd && method!=yee && method!=sosup )
    {
        numberToPlot += 2;  // something for Kyle
    }
    int nEdiss=numberToPlot;  numberToPlot += (cg.numberOfDimensions()+1)*int(e_cgdissipation ? 1 : 0);
    int nRho=numberToPlot;    numberToPlot += int(plotRho); 
    int nEnergyDensity=numberToPlot; numberToPlot += int(plotEnergyDensity);
    int nIntensity=numberToPlot; numberToPlot += int(plotIntensity);

  // There are 2 components of the harmonic field, Er and Ei for each component of E. 
    int nHarmonicE=numberToPlot;  numberToPlot += 2*(cg.numberOfDimensions())*int(plotHarmonicElectricFieldComponents);

    bool plotCurlE=false;     // for testing plot curl( E_known )
    if( method==yee && false )
        plotCurlE=true;
    int nCurlE = numberToPlot; numberToPlot += 2*(1 + 2*(numberOfDimensions-2))*int(plotCurlE);
    

//   int numberToPlot=numberOfComponents*(1+int(saveErrors)+int(saveDissipation))
//     + cg.numberOfDimensions()*int(saveDissipation&&method==dsiMatVec&&cg.numberOfDimensions()==3)
//     + int(useVariableDissipation)
//     + int(plotDivergence) + 2*(method!=nfdtd) + (cg.numberOfDimensions()+1)*int(e_cgdissipation ? 1 : 0);

  // we build a grid function with more components (errors, dissipation) for plotting
    v.updateToMatchGrid(cg,all,all,all,numberToPlot);

    v=0;
    if( method==nfdtd || method==yee || method==sosup ) 
    {
        for( int n=0; n<numberOfComponents; n++ )
        {
            if( mgp!=NULL )
            {
      	MappedGrid & mg = *mgp;
      	if( mg.getGridType()==MappedGrid::structuredGrid )
      	{
        	  if ( method==nfdtd || method==sosup || ( n<fields[current].getLength(3) ) )
          	    v.setName(fields[current].getName(n),n);
        	  else if ( n<fields[current].getLength(3) ) 
          	    v.setName(fields[current+numberOfTimeLevels].getName(n-fields[current].getLength(3)),n);
              		  
        	  if( saveErrors )
          	    v.setName(errp->getName(n),n+numberOfComponents);
      	}
            }
            else
            {
	// *wdh* v.setName(cgfields[current].getName(n),n);
      	v.setName(getCGField(HField,current).getName(n),n);
      	if( saveErrors )
        	  v.setName(cgerrp->getName(n),n+numberOfComponents);
      	if( saveDissipation )
        	  v.setName(cgdissipation->getName(n),n+ndd);
            }
        	  
        }
    }
    else
    {
        if( cg.numberOfDimensions()==2 )
        {
            int i=3;
            v.setName("Hz",0);
            v.setName("Ex",1);
            v.setName("Ey",2);
            if( method==dsiMatVec )
            {
      	v.setName("E.n",3);
      	i=4;
            }
            if ( (dissipation ||cgdissipation )&& plotDissipation)
            {
      	v.setName("Hz dissp",i++);
      	v.setName("E.n dissp",i++);
      	v.setName("Ex dissp",i++);
      	v.setName("Ey dissp",i++);
            }
            if( saveErrors )
            {
      	v.setName("Hz-err",i++);
      	v.setName("Ex-err",i++);
      	v.setName("Ey-err",i);
            }
        }
        else
        {
            v.setName("Hx",hx);
            v.setName("Hy",hy);
            v.setName("Hz",hz);
            v.setName("Ex",ex+3);
            v.setName("Ey",ey+3);
            v.setName("Ez",ez+3);
            v.setName("H.n",ez+4);
            v.setName("E.n",ez+5);
        	  
            int i=8;
            if ( (dissipation ||cgdissipation) && plotDissipation)
            {
      	v.setName("H.n dissp",i++);
      	v.setName("Hx dissp",i++);
      	v.setName("Hy dissp",i++);
      	v.setName("Hz dissp",i++);
      	v.setName("E.n dissp",i++);
      	v.setName("Ex dissp",i++);
      	v.setName("Ey dissp",i++);
      	v.setName("Ez dissp",i++);
            }
        	  
            if( saveErrors )
            {
      	v.setName("Hx-err",i++);
      	v.setName("Hy-err",i++);
      	v.setName("Hz-err",i++);
            	      
      	v.setName("Ex-err",i++);
      	v.setName("Ey-err",i++);
      	v.setName("Ez-err",i);
            }
        	  
        	  
        }
            
    }

    if( plotDivergence && (method==nfdtd || method==yee || method==sosup) )
    {
        v.setName("div(E)",nDivE);
        if( nDivH>=0 )
            v.setName("div(H)",nDivH);
    }
    if( plotCurlE && (method==nfdtd || method==yee || method==sosup) )
    {
        if( numberOfDimensions==3 )
        {
            v.setName("curlExr",nCurlE  );
            v.setName("curlEyr",nCurlE+1);
            v.setName("curlEzr",nCurlE+2);
            v.setName("curlExi",nCurlE+3);
            v.setName("curlEyi",nCurlE+4);
            v.setName("curlEzi",nCurlE+5);
        }
        
    }
    
    
    if( useVariableDissipation )
        v.setName("varDis",nVarDis);
    if( plotRho )
        v.setName("rho",nRho);
    if( plotEnergyDensity )
        v.setName("energyDensity",nEnergyDensity);

    if( plotIntensity )
    {
        v.setName("intensity",nIntensity);
    }
        
    if( plotHarmonicElectricFieldComponents )
    {
        v.setName("Exr",nHarmonicE+0);
        v.setName("Exi",nHarmonicE+1);
        v.setName("Eyr",nHarmonicE+2);
        v.setName("Eyi",nHarmonicE+3);
        if( numberOfDimensions==3 )
        {
            v.setName("Ezr",nHarmonicE+4);
            v.setName("Ezi",nHarmonicE+5);
        }
    }
        
    if( t<0. )
    {
    // in this case we only assign the component names and return 
        return v;
    }


//   if( plotIntensity || plotHarmonicElectricFieldComponents )
//   {
//     if( false && intensityOption==1 )
//     {
//       // compute the intensity using current and prev values
//       int stepNumber=0;
//       real nextTimeToPlot=0.;
//       real dt=deltaT; // check this 
//       computeIntensity(current,t,dt,stepNumber,nextTimeToPlot);
//     }
//   }
  // printF(" plot: cg.numberOfComponentGrids() = %i \n",cg.numberOfComponentGrids());
    

    divEMax=0.;

    if( method==yee )
    {
    // compute node centered fields for plotting -- this will fill in v ---
        int option=3;
        int iparam[5] = { nDivE,nDivH,0,0,0 }; // 
        getValuesFDTD( option, iparam, current, t, deltaT, &v );
    // ::display(v[0],"v after getValuesFDTD","%5.2f");
        if( plotDivergence )
        {
            option=2; // compute div(E) ( and div(H) in 3D)
            getValuesFDTD( option, iparam, current, t, deltaT, &v );
        }
        if( plotCurlE )
        {
            option=4; // compute curl(E)
            iparam[0]=nCurlE;
            getValuesFDTD( option, iparam, current, t, deltaT, &v );
        }
        
    }


    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = mgp!=NULL ? *mgp : cg[grid];

        realMappedGridFunction & u = mgp!=NULL ? fields[current] : getCGField(HField,current)[grid];
        realMappedGridFunction & vg = v[grid];
    
#ifdef USE_PPP
        realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray vLocal; getLocalArrayWithGhostBoundaries(vg,vLocal);
    // const int includeGhost=1;
    // ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,includeGhost); 
#else
        const realSerialArray & uLocal = u;
        const realSerialArray & vLocal = vg;
#endif

        Range N=numberOfComponents;
        if( method==yee )
        {
      // this is done above
        }
        else if( method==nfdtd  || method==sosup )
            vLocal(all,all,all,N)=uLocal(all,all,all,N); // for now make a copy *** fix this **
        else
        {
            Range N1 = fields[current].getLength(3);
            Range N2 =fields[current+numberOfTimeLevels].getLength(3);
#ifdef USE_PPP
            realSerialArray f1Local; getLocalArrayWithGhostBoundaries(fields[current],f1Local);
            realSerialArray f2Local; getLocalArrayWithGhostBoundaries(fields[current+numberOfTimeLevels],f2Local);
#else
            const realSerialArray & f1Local = fields[current];
            const realSerialArray & f2Local = fields[current+numberOfTimeLevels];
#endif
            vLocal(all,all,all,N1) = f1Local(all,all,all,N1);
            vLocal(all,all,all,N2) = f2Local(all,all,all,N2);
        }

        if( (plotScatteredField || plotTotalField) &&
      	cg.domainNumber(grid)==0 )  //   assumes domain 0 is the exterior domain
        {
      // *** NOTE: only add plane wave to the outer domain

            const real cc= c*sqrt( kx*kx+ky*ky+kz*kz );
      	
            mg.update(MappedGrid::THEcenter | MappedGrid::THEvertex);  // *** fix for rectangular ***
      	
      // subtract off or add on the the incident field
            const real pm = plotScatteredField ? 1. : -1.;

            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            getIndex(mg.dimension(),I1,I2,I3);
            const int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 


#ifdef USE_PPP
            realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.center(),xLocal);
#else
            const realSerialArray & xLocal = mg.center();
#endif

            if( ok )
            {
      	if( mg.numberOfDimensions()==2 )
      	{
        	  const realSerialArray & x = xLocal(I1,I2,I3,0);
        	  const realSerialArray & y = xLocal(I1,I2,I3,1);

        	  vLocal(I1,I2,I3,ex)-=pm*exTrue(x,y,t);
        	  vLocal(I1,I2,I3,ey)-=pm*eyTrue(x,y,t);
        	  vLocal(I1,I2,I3,hz)-=pm*hzTrue(x,y,t);
        	  if( method==sosup )
        	  {
          	    vLocal(I1,I2,I3,ext)-=pm*extTrue(x,y,t);
          	    vLocal(I1,I2,I3,eyt)-=pm*eytTrue(x,y,t);
          	    vLocal(I1,I2,I3,hzt)-=pm*hztTrue(x,y,t);
        	  }
        	  
      	}
      	else
      	{
        	  const realSerialArray & x = xLocal(I1,I2,I3,0);
        	  const realSerialArray & y = xLocal(I1,I2,I3,1);
        	  const realSerialArray & z = xLocal(I1,I2,I3,2);

        	  if( solveForElectricField )
        	  {
          	    vLocal(I1,I2,I3,ex)-=pm*exTrue3d(x,y,z,t);
          	    vLocal(I1,I2,I3,ey)-=pm*eyTrue3d(x,y,z,t);
          	    vLocal(I1,I2,I3,ez)-=pm*ezTrue3d(x,y,z,t);
          	    if( method==sosup )
          	    {
            	      vLocal(I1,I2,I3,ext)-=pm*extTrue3d(x,y,z,t);
            	      vLocal(I1,I2,I3,eyt)-=pm*eytTrue3d(x,y,z,t);
            	      vLocal(I1,I2,I3,ezt)-=pm*eztTrue3d(x,y,z,t);
          	    }
          	    
        	  }

      	}
            } // end if ok 
      	
        } // end if( plotScatteredField || plotTotalField )
            
        if( plotEnergyDensity )
        {
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            getIndex(mg.dimension(),I1,I2,I3);
            const int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 

            if( ok )
            {
      	c = cGrid(grid);
      	eps = epsGrid(grid);
      	mu = muGrid(grid);

      	if( mg.numberOfDimensions()==2 )
      	{
	  // vLocal(I1,I2,I3,nEnergyDensity)= eps*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey)) );
        	  vLocal(I1,I2,I3,nEnergyDensity)= ( (.5*eps)*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey)) )+
                                   					     (.5*mu )*( SQR(uLocal(I1,I2,I3,hz)) ) );
      	}
      	else
      	{
                    Overture::abort("finish me -- we need H here");
        	  vLocal(I1,I2,I3,nEnergyDensity)= eps*( SQR(uLocal(I1,I2,I3,ex))+SQR(uLocal(I1,I2,I3,ey))+
                                                                                                  SQR(uLocal(I1,I2,I3,ez)) );
      	}
            } // end if ok 
      	
        } // end if( plotEnergyDensity )
            
        if( plotIntensity )
        {
            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            getIndex(mg.dimension(),I1,I2,I3);
            const int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 

            if( pIntensity!=NULL )
            {
      	realCompositeGridFunction & intensity = *pIntensity;
                #ifdef USE_PPP
                    realSerialArray intensityLocal; getLocalArrayWithGhostBoundaries(intensity[grid],intensityLocal);
                #else
                    const realSerialArray & intensityLocal = intensity[grid];
                #endif
      	if( ok )
      	{
        	  vLocal(I1,I2,I3,nIntensity)= intensityLocal(I1,I2,I3);
      	} // end if ok 
            }
            else
            {
      	vLocal(I1,I2,I3,nIntensity)=0.;
            }
        } // end if( plotIntensity )

        if( plotHarmonicElectricFieldComponents )
        {
      // plot Er and Ei assuming : E(x,t) = Er(x)*cos(w*t) + Ei(x)*sin(w*t)

            Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
            getIndex(mg.dimension(),I1,I2,I3);
            const int includeGhost=1;
            bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost); 

            Range Rx2=2*numberOfDimensions;
            if( pHarmonicElectricField!=NULL )
            {
      	realCompositeGridFunction & hef = *pHarmonicElectricField;
                #ifdef USE_PPP
                    realSerialArray hefLocal; getLocalArrayWithGhostBoundaries(hef[grid],hefLocal);
                #else
                    const realSerialArray & hefLocal = hef[grid];
                #endif
      	if( ok )
      	{
        	  vLocal(I1,I2,I3,Rx2+nHarmonicE)= hefLocal(I1,I2,I3,Rx2);
      	} // end if ok 
            }
            else
            {
      	vLocal(I1,I2,I3,Rx2+nHarmonicE)=0.;
            }
        }


        if( saveErrors )
        {
            realMappedGridFunction & err = errp!=NULL ? *errp : cgerrp!=NULL ? (*cgerrp)[grid] : u;    
#ifdef USE_PPP
            realSerialArray errLocal; getLocalArrayWithGhostBoundaries(err,errLocal);
#else
            const realSerialArray & errLocal = err;
#endif
            vLocal(all,all,all,N+numberOfComponents)=errLocal(all,all,all,N);
        }
            
        if( useVariableDissipation )
        {
#ifdef USE_PPP
            realSerialArray varDissLocal; getLocalArrayWithGhostBoundaries((*variableDissipation)[grid],varDissLocal);
#else
            const realSerialArray & varDissLocal = (*variableDissipation)[grid];
#endif
            vLocal(all,all,all,nVarDis)=varDissLocal;
        }
            
        if( saveDissipation )
        {
#ifdef USE_PPP
            realSerialArray dissLocal; getLocalArrayWithGhostBoundaries((*cgdissipation)[grid],dissLocal);
#else
            const realSerialArray & dissLocal = (*cgdissipation)[grid];
#endif
            vLocal(all,all,all,N+ndd)=dissLocal(all,all,all,N);
        }

        if( plotRho )
        {
            getChargeDensity( current,t,v,nRho );
        }

    // **New way **
        if( method==nfdtd  || method==sosup )
        {
      // printF(" $$$$ plot: call getMaxDivergence $$$$\n");

            if( plotDivergence )
            {
      	getMaxDivergence( current,t, &v,nDivE, &v,nRho);
            }
            else
            {
      	getMaxDivergence( current,t );
            }   
        }

    }

// #ifndef USE_PPP  // *wdh* 090709
    if( plotDivergence && mgp==NULL ) 
    {
    // we need to interpolate the divergence to give values at the interp. pts. for plotting
        v.interpolate(Range(nDivE,nDivE));
    }
// #endif
    
    return v;
    
}

                   		       

int Maxwell::
buildRunTimeDialog()
// =============================================================================================
// =============================================================================================
{
    GenericGraphicsInterface & ps = *gip;
    if( runTimeDialog==NULL )
    {
        runTimeDialog = new GUIState;
        GUIState & dialog = *runTimeDialog;
        

        dialog.setWindowTitle("Maxwell");
        dialog.setExitCommand("finish", "finish");

        aString cmds[] = {"break","continue",
                                            "movie mode","movie and save",
                                            "contour", "E field lines",
                                            "grid", "erase",
                                            "plot options...",
                                            "parameters...",
                      // "change the grid...",
                      // "show file options...","file output...",
                      // "pde parameters...",
                                            ""};
        numberOfPushButtons=9;  // number of entries in cmds
        int numRows=(numberOfPushButtons+1)/2;
        dialog.setPushButtons( cmds, cmds, numRows ); 

    // get any extra components such as errors for tz flow or the pressure for CNS.
        realCompositeGridFunction v;
        real t=-1; // this means only fill in the component names. 
        realCompositeGridFunction & u = getAugmentedSolution(0,v,t);

        const int numberOfComponents = u.getComponentBound(0)-u.getComponentBase(0)+1;
    // create a new menu with options for choosing a component.
        aString *cmd = new aString[numberOfComponents+1];
        aString *label = new aString[numberOfComponents+1];
        for( int n=0; n<numberOfComponents; n++ )
        {
            label[n]=u.getName(n);
            cmd[n]="plot:"+u.getName(n);

        }
        cmd[numberOfComponents]="";
        label[numberOfComponents]="";
        
        dialog.addOptionMenu("plot component:", cmd,label,0);
        delete [] cmd;
        delete [] label;

//     aString tbCommands[] = {"project fields",
// 			    ""};
//     int tbState[10];
//     tbState[0] = projectFields; 
//     int numColumns=1;
//     dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

        const int numberOfTextStrings=7;
        aString textLabels[numberOfTextStrings];
        aString textStrings[numberOfTextStrings];

        int nt=0;
        textLabels[nt] = "final time";  sPrintF(textStrings[nt], "%g",tFinal);  nt++; 
        textLabels[nt] = "times to plot";  sPrintF(textStrings[nt], "%g",tPlot);  nt++; 
        textLabels[nt] = "cfl";  
        sPrintF(textStrings[nt], "%g",cfl);  nt++; 
        textLabels[nt] = "debug";  sPrintF(textStrings[nt], "%i",debug);  nt++; 
  
       // null strings terminal list
        textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
        dialog.setTextBoxes(textLabels, textLabels, textStrings);
        numberOfTextBoxes=nt;
        

    // ******************* file output *************************
//     DialogData & fileOutputDialog = dialog.getDialogSibling();

//     fileOutputDialog.setWindowTitle("File Output Parameters");
//     fileOutputDialog.setExitCommand("close file output dialog", "close");

//     aString cmdf[] = {"file output",
//                       "output periodically to a file",
//                       "close an output file",
//                       "save restart file",
//                       ""};
//     int numberOfRows=4;
//     fileOutputDialog.setPushButtons( cmdf, cmdf, numberOfRows );

//     nt=0;
//     textLabels[nt] = "output file name";  sPrintF(textStrings[nt], "%s","overBlown.out");  nt++; 
//     textLabels[nt]= "restart file name";  sPrintF(textStrings[nt], "%s",(const char*)restartFileName);nt++; 
//     textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
//     fileOutputDialog.setTextBoxes(textLabels, textLabels, textStrings);


//     // ****** pde parameters *************
//     DialogData &pdeDialog = dialog.getDialogSibling();
//     pdeDialog.setExitCommand("close pde options", "close");
//     setPdeParameters("build dialog",&pdeDialog);



        ps.pushGUI(dialog);


    }
    return 0;

}


static void
setSensitivity( GUIState & dialog, bool trueOrFalse )
{
    dialog.getOptionMenu(0).setSensitive(trueOrFalse);
    int n;
    for( n=1; n<numberOfPushButtons; n++ ) // leave first push button sensitive (=="break")
        dialog.setSensitive(trueOrFalse,DialogData::pushButtonWidget,n);
    
    for( n=0; n<numberOfTextBoxes; n++ )
        dialog.setSensitive(trueOrFalse,DialogData::textBoxWidget,n);
    
}

int Maxwell::
buildParametersDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//    Build the parameters dialog -- these are various parameters that can be changed
//   at run time. 
// ==========================================================================================
{

  // ************** PUSH BUTTONS *****************
    dialog.setOptionMenuColumns(1);

    aString tbCommands[] = {"project fields",
                      			  ""};
    int tbState[15];
    tbState[0] = projectFields;

    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 



  // ----- Text strings ------
    const int numberOfTextStrings=30;
    aString textCommands[numberOfTextStrings];
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    textCommands[nt] = "dissipation"; 
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%e",artificialDissipation);  nt++; 

    textCommands[nt] = "dissipation (curvilinear)"; 
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%e",artificialDissipationCurvilinear);  nt++; 

    textCommands[nt] = "projection frequency";  
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",frequencyToProjectFields); nt++; 

    textCommands[nt] = "consecutive projection steps";  
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfConsecutiveStepsToProject); nt++; 

    textCommands[nt] = "number of divergence smooths";  
    textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfDivergenceSmooths); nt++; 

  // null strings terminal list
    assert( nt<numberOfTextStrings );
    textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
    dialog.setTextBoxes(textCommands, textLabels, textStrings);

    return 0;
}


// ========================================================================================
/// \brief Construct the label that defines the time-stepping method and used in the
///    title for plots
// ========================================================================================
void Maxwell::
getTimeSteppingLabel( real dt, aString & label ) const
{
    aString buff;
    label=sPrintF(buff,"dt=%4.1e",dt);

    if( timeSteppingMethod==modifiedEquationTimeStepping )
        label+=" TS=ME";
    else if( timeSteppingMethod==stoermerTimeStepping )
        label+=" TS=ST";
    else if( timeSteppingMethod==rungeKuttaFourthOrder )
        label+=" TS=RK";
    else if( timeSteppingMethod==defaultTimeStepping )
        label+=" TS=default ";
    else
        label+="TS=??, ";

  // if( twilightZoneOption==polynomialTwilightZone )
  //   label+=sPrintF(buff," order(X,T)=(%i,%i)",
  // 		   orderOfAccuracyInSpace,orderOfAccuracyInTime);

    if( method==nfdtd )
    {
        if( artificialDissipation!=0. && artificialDissipation==artificialDissipationCurvilinear )
            label+=sPrintF(buff," ad%i=%4.2f",orderOfArtificialDissipation,artificialDissipation);
        else if( artificialDissipationCurvilinear!=0. )
            label+=sPrintF(buff," adr%i=%4.2f,adc%i=%4.2f",orderOfArtificialDissipation,artificialDissipation,
                                          orderOfArtificialDissipation,artificialDissipationCurvilinear);
            
        if( applyFilter )
            label+=sPrintF(buff,", filter%i",orderOfFilter);

        if( divergenceDamping>0. )
            label+=sPrintF(buff," dd=%5.3f",divergenceDamping);
    }
    
}



int Maxwell::
plot(int current, real t, real dt )
// ========================================================================================
// /Description:
//  plotOptions :  0 = no plotting
//            1 - plot and wait
//            2 - do not wait for response after plotting
// /Return values: 0=normal exit. 1=user has requested "finish".
// ========================================================================================
{
    if( plotOptions==0 )
        return 0;

    real cpu0=getCPU();
    int returnValue=0;

    assert( cgp!=NULL );
    CompositeGrid & cg = *cgp;

    GenericGraphicsInterface & ps = *gip;

    char buff[100];
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Maxwell %s%i%i: t=%6.2e ",(const char *)methodName,
                    orderOfAccuracyInSpace,orderOfAccuracyInTime,t));
    aString label;
    getTimeSteppingLabel( dt,label );
    
  // label=sPrintF(buff,"dt=%4.1e",dt);

  // if( timeSteppingMethod==modifiedEquationTimeStepping )
  //   label+=" TS=ME";
  // else if( timeSteppingMethod==stoermerTimeStepping )
  //   label+=" TS=ST";
  // else if( timeSteppingMethod==rungeKuttaFourthOrder )
  //   label+=" TS=RK";
  // else if( timeSteppingMethod==defaultTimeStepping )
  //   label+=" TS=default ";
  // else
  //   label+="TS=??, ";

  // if( twilightZoneOption==polynomialTwilightZone )
  //   label+=sPrintF(buff," order(X,T)=(%i,%i)",
  // 		   orderOfAccuracyInSpace,orderOfAccuracyInTime);
  // if( method==nfdtd )
  // {
  //   if( artificialDissipation!=0. && artificialDissipation==artificialDissipationCurvilinear )
  //     label+=sPrintF(buff," ad%i=%4.2f",orderOfArtificialDissipation,artificialDissipation);
  //   else if( artificialDissipationCurvilinear!=0. )
  //     label+=sPrintF(buff," adr%i=%4.2f,adc%i=%4.2f",orderOfArtificialDissipation,artificialDissipation,
  //                    orderOfArtificialDissipation,artificialDissipationCurvilinear);
            
  //   if( applyFilter )
  //     label+=sPrintF(buff,", filter%i",orderOfFilter);

  //   if( divergenceDamping>0. )
  //     label+=sPrintF(buff," dd=%5.3f",divergenceDamping);
  // }
    
    if( plotScatteredField )
        label+="(scattered field)";
        
    psp.set(GI_TOP_LABEL_SUB_1,label);

  // we need to know if the graphics is oen on any processor -- fix this in the GraphicsInterface.
    int graphicsIsOn = ps.isGraphicsWindowOpen();
    graphicsIsOn=getMaxValue(graphicsIsOn);
    int readingCommandFile = ps.readingFromCommandFile();
    readingCommandFile=getMaxValue(readingCommandFile);

  // printF(" **** t=%e, graphicsIsOn=%i readingCommandFile=%i, processor=%i\n",t,graphicsIsOn,readingCommandFile, 
  //            myid);
  // fflush(stdout);

  // // set to true for debugging:   **WARNING: this will break the check files: output called twice:
  // bool getDiv=false; // true; 
  // if(  getDiv || (!graphicsIsOn && readingCommandFile && (method==nfdtd || method==sosup) ) )
  // {
  //   // printF(" **** call getMaxDivergence t=%e, processor=%i\n",t, myid);
  //   // fflush(stdout);
    

  //   // no plotting and reading from a command file
  //   // *** get divEMax and uMin, uMax
  //   if ( method==nfdtd || method==sosup )
  //     getMaxDivergence( current,t );

  //   printF(">>> Cgmx:%s: t=%6.2e, %s |div(E)|=%8.2e, |div(E)|/|grad(E)|=%8.2e, |grad(E)|=%8.2e (%i steps)\n",
  // 	   (const char *)methodName,t,(const char*)label,
  // 	   divEMax,divEMax/max(REAL_MIN*100.,gradEMax),gradEMax,numberOfStepsTaken);
  //   if( solveForMagneticField && cg.numberOfDimensions()==3 )
  //   {
  //     printF("                                                              "
  //            "|div(H)|=%8.2e, |div(H)|/|grad(H)|=%8.2e, |grad(H)|=%8.2e (%i steps)\n",
  // 	     divHMax,divHMax/max(REAL_MIN*100.,gradHMax),gradHMax,numberOfStepsTaken);
  //   }
        
  //   outputResults(current,t,dt);
        
  //   timing(timeForPlotting)+=getCPU()-cpu0;
  //   if( !getDiv ) return returnValue;
  // }
    

    if( runTimeDialog==NULL )
    {
        buildRunTimeDialog();
    // --- Build the sibling dialog for plot options ---
        DialogData & plotOptionsDialog = runTimeDialog->getDialogSibling();
        pPlotOptionsDialog = &plotOptionsDialog;
        plotOptionsDialog.setWindowTitle("MX Plot Options");
        plotOptionsDialog.setExitCommand("close plot options", "close");
        buildPlotOptionsDialog(plotOptionsDialog);

        DialogData & parametersDialog = runTimeDialog->getDialogSibling();
        pParametersDialog = &parametersDialog;
        parametersDialog.setWindowTitle("MX Parameters");
        parametersDialog.setExitCommand("close parameters", "close");
        buildParametersDialog(parametersDialog);

    }
    DialogData &plotOptionsDialog = *pPlotOptionsDialog;
    DialogData &parametersDialog = *pParametersDialog;

    GUIState & dialog = *runTimeDialog;

    aString answer;

  // get any extra components such as errors for tz flow or the pressure for CNS.

  // MappedGrid & mg = *(fields[current].getMappedGrid());

  // **** no need to compute extra components if we are in movie mode and we are not
  //      plotting any extra component ****
    realCompositeGridFunction v;
    realCompositeGridFunction & u = getAugmentedSolution(current,v,t);  // u is either solution or v

    const int numberOfComponents = u.getComponentBound(0)-u.getComponentBase(0)+1;

    if( movieFrame>=0   )
    { // save a ppm file as part of a movie.
        psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::ppm);
        ps.outputString(sPrintF(buff,"Saving file %s%i.ppm",(const char*)movieFileName,movieFrame));
        ps.hardCopy(    sPrintF(buff,            "%s%i.ppm",(const char*)movieFileName,movieFrame),psp);
        psp.set(GI_HARD_COPY_TYPE,GraphicsParameters::postScript);
        movieFrame++;
    }

    

    ps.erase();
    if( plotOptions & 1 )
    {

    // printF(">>> Cgmx:%s: t=%6.2e, %s |div(E)|=%8.2e, |div(E)|/|grad(E)|=%8.2e, |grad(E)|=%8.2e (%i steps)\n",
    // 	   (const char *)methodName,t,(const char*)label,
    // 	   divEMax,divEMax/max(REAL_MIN*100.,gradEMax),gradEMax,numberOfStepsTaken);
    // if( solveForMagneticField && cg.numberOfDimensions()==3 )
    // {
    //   printF("                                                              "
    //          "|div(H)|=%8.2e, |div(H)|/|grad(H)|=%8.2e, |grad(H)|=%8.2e (%i steps)\n",
    // 	     divHMax,divHMax/max(REAL_MIN*100.,gradHMax),gradHMax,numberOfStepsTaken);
    // }
    // outputResults(current,t,dt);
        

    // Plot all the the things that the user has previously plotted
        psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
        if( plotChoices & 1 )
        {
            if( mgp!=NULL )
                PlotIt::plot(ps,*mgp,psp);
            else
                PlotIt::plot(ps,*cgp,psp);
        }
        if( plotChoices & 2 )
            PlotIt::contour(ps,u,psp);
        if( plotChoices & 4 )
            PlotIt::streamLines(ps,u,psp);


        bool programHalted=false;
        if( plotOptions & 2 )
        {
      // movie mode ** check here if the user has hit break ***
            if( ps.isGraphicsWindowOpen() && 
          !ps.readingFromCommandFile() )  // for now we cannot check if we are reading from a command file
            {
	// ps.outputString(sPrintF(buff,"Check for break at t=%e\n",t));
      	answer="";
      	int menuItem = ps.getAnswerNoBlock(answer,"monitor>");
      	if( answer=="break" )
      	{
        	  programHalted=true;
      	}
            }
            
        }
        
        if( ! (plotOptions & 2) || programHalted )
        {
            if( plotOptions & 1 )
            {
      	setSensitivity( dialog,true );
            }
            
            plotOptions=1; // reset movie mode if set.
            movieFrame=-1;
            
            psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

//       DialogData & fileOutputDialog = dialog.getDialogSibling(1);
//       DialogData & pdeDialog = dialog.getDialogSibling(2);

            int len;
            bool replot=false;
            for(;;)
            {
	// int menuItem = ps.getMenuItem(menu,answer,"choose an option");
                real timew=getCPU();
      	int menuItem = ps.getAnswer(answer,"");
                timing(timeForWaiting)+=getCPU()-timew;

      	if( answer=="contour" )
      	{
                    if(plotChoices & 2 )
                        ps.erase();
        	  
                    PlotIt::contour(ps,u,psp);
        	  if( psp.getObjectWasPlotted() ) 
          	    plotChoices |= 2;
                    else
                        plotChoices &= ~2;
      	}
// 	else if( menuItem > chooseAComponentMenuItem && 
//                  menuItem <= chooseAComponentMenuItem+numberOfComponents )
// 	{
//           // plot a new component
// 	  int component=menuItem-chooseAComponentMenuItem-1;
//           if( plotChoices & 2 )
// 	  {
//             ps.erase();
// 	    psp.set(GI_COMPONENT_FOR_CONTOURS,component);
// 	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

// 	    if( plotChoices & 1 )
// 	      PlotIt::plot(ps,mg,psp);

// 	    PlotIt::contour(ps,u,psp);

// 	    if( plotChoices & 4 )
// 	      PlotIt::streamLines(ps,u,psp);

// 	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
// 	  }
// 	}
                else if( answer=="grid" )
      	{
                    if( mgp!=NULL )
                        PlotIt::plot(ps,*mgp,psp);
                    else
                        PlotIt::plot(ps,*cgp,psp);

        	  if( psp.getObjectWasPlotted() ) 
          	    plotChoices |= 1;
                    else
                        plotChoices &= ~1;
      	}
      	else if( answer=="E field lines" )
      	{
                    int uc,vc;
                    if( ( mgp!=NULL && mgp->isRectangular() && method==defaultMethod) || method==yee )
        	  {
          	    uc=ex; vc=ey;
        	  }
        	  else
        	  {
          	    uc=ex10, vc=ey10;
        	  }
        	  psp.set(GI_U_COMPONENT_FOR_STREAM_LINES,uc);
        	  psp.set(GI_V_COMPONENT_FOR_STREAM_LINES,vc);
                    PlotIt::streamLines(ps,u,psp);

        	  if( psp.getObjectWasPlotted() ) 
          	    plotChoices |= 4;
                    else
                        plotChoices &= ~4;
      	}
      	else if( answer=="erase" )
      	{
                    ps.erase();
        	  plotChoices=0;
      	}
      	else if( answer=="plot options..." )
      	{
        	  plotOptionsDialog.showSibling();
      	}
      	else if( answer=="close plot options" )
      	{
        	  plotOptionsDialog.hideSibling();
      	}
      	else if( answer=="parameters..." )
      	{
        	  parametersDialog.showSibling();
      	}
      	else if( answer=="close parameters" )
      	{
        	  parametersDialog.hideSibling();
      	}
//         else if( answer=="save a restart file" )
// 	{
// 	  ps.inputFileName(answer,sPrintF(buff,"Enter the restart file name (default value=%s)",
// 					  (const char *)restartFileName));
// 	  if( answer!="" )
// 	    restartFileName=answer;

// 	  saveRestartFile(solution,restartFileName);
// 	}
//         else if( answer=="save restart file" ) // new way, do not prompt for restart file name
// 	{
// 	  saveRestartFile(solution,restartFileName);
// 	}
//         else if( answer=="output to a file" )
// 	{
// 	  FileOutput fileOutput;
// 	  fileOutput.update(u,ps);
// 	}
// 	else if( answer=="output periodically to a file" || answer=="output periodically to a file..." )
// 	{
//           if( numberOfOutputFiles>=maximumNumberOfOutputFiles )
// 	  {
// 	    printF("ERROR: too many files open\n");
// 	    continue;
// 	  }
//           fileOutputFrequency[numberOfOutputFiles]=1;
//           ps.inputString(answer,"Save to the file every how many steps? (default=1)");
//           sScanF(answer,"%i",&fileOutputFrequency[numberOfOutputFiles]);
        	  
//           FileOutput & fileOutput = * new FileOutput;
// 	  outputFile[numberOfOutputFiles] = &fileOutput;
// 	  numberOfOutputFiles++;
                    
//           fileOutput.update(u,ps);

        	  
// 	}
// 	else if( answer=="close an output file" )
// 	{
//           aString *fileMenu = new aString [numberOfOutputFiles+2];
//           int n;
// 	  for( n=0; n<numberOfOutputFiles; n++ )
// 	  {
// 	    fileMenu[n]=outputFile[n]->getFileName();
// 	  }
//           fileMenu[parameters.numberOfOutputFiles]="none";
//           fileMenu[parameters.numberOfOutputFiles+1]="";
// 	  int fileChosen = ps.getMenuItem(fileMenu,answer,"Choose a file to close");
// 	  if( fileChosen>=0 && fileChosen<parameters.numberOfOutputFiles )
// 	  {
//             printF("close file %s\n",(const char*)fileMenu[fileChosen]);
// 	    delete parameters.outputFile[fileChosen];
//             parameters.numberOfOutputFiles--;
// 	    for( n=fileChosen; n<parameters.numberOfOutputFiles; n++ )
// 	      parameters.outputFile[n]=parameters.outputFile[n+1];
// 	    parameters.outputFile[parameters.numberOfOutputFiles]=NULL;
// 	  }
// 	}
      	else if( answer=="continue" )
      	{
                    if( t >= tFinal-dt/10. )
        	  {
          	    printF("WARNING: t=tFinal. Choose `finish' if you really want to end\n");
        	  }
        	  else
                        break;
        	}
      	else if( answer=="movie mode" )
      	{
                    plotOptions=3;  // don't wait

            	  setSensitivity( dialog,false );
                    break;
        	}
                else if( answer=="movie and save" )
      	{
        	  ps.inputString(answer,"Enter basic name for the ppm files (default=plot)");
        	  if( answer !="" && answer!=" ")
          	    movieFileName=answer;
                    else
          	    movieFileName="plot";
                    ps.outputString(sPrintF(buff,"pictures will be named %s0.ppm, %s1.ppm, ...",
                        (const char*)movieFileName,(const char*)movieFileName));
        	  movieFrame=0;
                    plotOptions=3;  // don't wait

            	  setSensitivity( dialog,false );
                    break;
      	}
//         else if( answer=="show file options" || answer=="show file options..." )
// 	{
//            updateShowFile();
// 	}
      	else if( answer=="finish" )
      	{
                    tFinal=t;
                    returnValue=1;
                    break;
        	}
      	else if( plotOptionsDialog.getToggleValue(answer,"plot energy density",plotEnergyDensity) ){replot=true;}//
                else if( plotOptionsDialog.getToggleValue(answer,"plot intensity",plotIntensity) ){}//
                else if( plotOptionsDialog.getToggleValue(answer,"plot harmonic E field",plotHarmonicElectricFieldComponents) ){}//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot errors",plotErrors) ){replot=true;}//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot scattered field",plotScatteredField) ){ replot=true; }//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot total field",plotTotalField) ){ replot=true; }//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot dissipation",plotDissipation) ){replot=true;}//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot divergence",plotDivergence) ){replot=true;}//
      	else if( plotOptionsDialog.getToggleValue(answer,"check errors",checkErrors) ){replot=true;}//
                else if( plotOptionsDialog.getToggleValue(answer,"compute energy",computeEnergy) ){}//
      	else if( plotOptionsDialog.getToggleValue(answer,"plot dsi vertex max",plotDSIMaxVertVals) ){replot=true;}//
      	else if( plotOptionsDialog.getToggleValue(answer,"compare to show file",compareToReferenceShowFile) )
                  {replot=true;}//

      	else if( dialog.getTextValue(answer,"cfl","%g",cfl) ){}//
      	else if( dialog.getTextValue(answer,"final time","%g",tFinal) ){}//
      	else if( dialog.getTextValue(answer,"times to plot","%g",tPlot) ){}//
                else if( dialog.getTextValue(answer,"debug","%i",debug) ){}//
                else if( dialog.getTextValue(answer,"radius for checking errors","%f",radiusForCheckingErrors) )
                {
                    getErrors( current,t,dt );
                    replot=true;
      	}
      	else if( plotOptionsDialog.getTextValue(answer,"pml error offset","%i",pmlErrorOffset) )
      	{
                    getErrors( current,t,dt );
                    replot=true;
      	}
                else if( parametersDialog.getToggleValue(answer,"project fields",projectFields) ){}//
                else if( parametersDialog.getTextValue(answer,"projection frequency","%i",frequencyToProjectFields) ){}// 
                else if( parametersDialog.getTextValue(answer,"consecutive projection steps","%i",
                                      numberOfConsecutiveStepsToProject) ){}// 
                else if( parametersDialog.getTextValue(answer,"number of divergence smooths","%i",
                                      numberOfDivergenceSmooths) ){}// 
                else if( parametersDialog.getTextValue(answer,"dissipation (curvilinear)","%g",artificialDissipationCurvilinear) ){}//
                else if( parametersDialog.getTextValue(answer,"dissipation","%g",artificialDissipation) ){}//

      	else if( len=answer.matches("plot:") )
      	{
          // plot a new component
                    aString name = answer(len,answer.length()-1);
                    int component=-1;
        	  for( int n=0; n<numberOfComponents; n++ )
        	  {
          	    if( name==u.getName(n) )
          	    {
            	      component=n;
            	      break;
          	    }
        	  }
                    if( component==-1 )
        	  {
                        printF("ERROR: unknown component name =[%s]\n",(const char*)name);
          	    component=0;
        	  }
                    dialog.getOptionMenu(0).setCurrentChoice(component);
                    if( plotChoices & 2 )
        	  {
                        ps.erase();
          	    psp.set(GI_COMPONENT_FOR_CONTOURS,component);
          	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

          	    if( plotChoices & 1 )
          	    {
            	      if( mgp!=NULL )
              	        PlotIt::plot(ps,*mgp,psp);
                            else
              	        PlotIt::plot(ps,*cgp,psp);
          	    }
          	    PlotIt::contour(ps,u,psp);

          	    if( plotChoices & 4 )
            	      PlotIt::streamLines(ps,u,psp);

          	    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
        	  }
      	}
// 	else if( answer=="file output..." )
// 	{
//           fileOutputDialog.showSibling();
// 	}
//         else if( answer=="close file output dialog" )
// 	{
//           fileOutputDialog.hideSibling();
// 	}
// 	else if( answer=="pde parameters..." )
// 	{
// 	  pdeDialog.showSibling();
// 	}
// 	else if( answer=="close pde options" )
// 	{
// 	  pdeDialog.hideSibling();  // pop timeStepping
// 	}
// 	else if( parameters.setPdeParameters(answer,&pdeDialog)==0 )
// 	{
// 	  printF("Answer was found in setPdeParameters\n");
// 	}
                else if( answer=="break" )
      	{
      	}
                else
      	{
        	  cout << "Unknown response: " << answer << endl;
      	}
      	if( replot )
      	{
        	  replot=false;
                    getAugmentedSolution(current,v,t);
        	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
                    ps.erase();
        	  if( plotChoices & 1 )
        	  {
          	    if( mgp!=NULL )
            	      PlotIt::plot(ps,*mgp,psp);
          	    else
            	      PlotIt::plot(ps,*cgp,psp);
        	  }
        	  if( plotChoices & 2 )
          	    PlotIt::contour(ps,u,psp);
        	  if( plotChoices & 4 )
          	    PlotIt::streamLines(ps,u,psp);
    
        	  psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
                }//
            }
        }
    }
    

    if( plotOptions & 2  )
    {
        ps.redraw(TRUE);
        
    }
    
    timing(timeForPlotting)+=getCPU()-cpu0;
    return returnValue;
}
