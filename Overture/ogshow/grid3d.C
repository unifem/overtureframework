// This file automatically generated from grid3d.bC with bpp.
#include "GL_GraphicsInterface.h"
#include "GraphicsParameters.h"
#include "CompositeGrid.h"
#include "PlotIt.h"
#include "xColours.h"
#include "ParallelUtility.h"

// This is a local version that we can set to 0 if we do not want to plot interior boundary points
static int ISinteriorBoundaryPoint=0;

// local version so that we can change it: 
static int isHiddenByRefinement=MappedGrid::IShiddenByRefinement;

int
collectInterpolationData( int srcProcessor, int destProcessor, int grid, CompositeGrid & cg,
                                                    intSerialArray & interpolationPoint, intSerialArray & interpoleeGrid );

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1,I2,I3) i1Bound=I1.getBound(); i2Bound=I2.getBound(); i3Bound=I3.getBound(); for( i3=I3.getBase(); i3<=i3Bound; i3+=m3 )  for( i2=I2.getBase(); i2<=i2Bound; i2+=m2 )  for( i1=I1.getBase(); i1<=i1Bound; i1+=m1 )


// define XR(i1,i2,i3,m,n) xr(i1,i2,i3,m+mg.numberOfDimensions()*(n))
#define XR(i1,i2,i3,m,n) xr[n][m]

// here is an unused point or an interior boundary point
#define MASK_UIB(i1,i2,i3) (MASK(i1,i2,i3)==0 || (!psp.plotInteriorBoundaryPoints && (MASK(i1,i2,i3) & ISinteriorBoundaryPoint)) )

// the following defines are taken from grid.C and are used in GL_GraphicsInterface::surfaceGrid3d
// here is a computational point but not a refined point
#define MASK_CNR(i1,i2,i3) (mask(i1,i2,i3) && !(mask(i1,i2,i3) & isHiddenByRefinement))
// here is a discretization point but not a refined point
#define MASK_DNR(i1,i2,i3) (mask(i1,i2,i3)>0 && !(mask(i1,i2,i3) & isHiddenByRefinement))
// here is a discretization point
#define MASK_D(i1,i2,i3) (mask(i1,i2,i3)>0)
// here is a discretization point that is not an interior boundary point
#define MASK_DNIB(i1,i2,i3) (mask(i1,i2,i3)>0 && !(mask(i1,i2,i3) & ISinteriorBoundaryPoint) )
// here is a discretization point or interpolation point that is not an interior boundary point
#define MASK_DINIB(i1,i2,i3) (mask(i1,i2,i3)!=0 && !(mask(i1,i2,i3) & ISinteriorBoundaryPoint) )

void 
getNormal(const realArray & x, 
        	  const int iv[3],
                    const int axis,
        	  real normal[3],
                    const int & recursion=TRUE );
void 
getNormal(const real *xp, int xDim0, int xDim1, int xDim2, int *xBase, int *xBound,
        	  const int iv[3],
                    const int axis, const int ap1, const int ap2,
        	  real normal[3],
                    const int & recursion=TRUE );

#undef XR

    
#define XSCALE(x) (psp.xScaleFactor*(x))
#define YSCALE(y) (psp.yScaleFactor*(y))
#define ZSCALE(z) (psp.zScaleFactor*(z))


// ******************************************************************************
//   ******* plotShaded *********
//
// GRIDTYPE: rectangular or curvilinear
// ******************************************************************************

void PlotIt::
plotShadedFace(GenericGraphicsInterface &gi, 
             	       const MappedGrid & c,
                              const intSerialArray & mask,
                              const realSerialArray & vertex,
             	       const Index & I1, 
             	       const Index & I2, 
             	       const Index & I3, 
             	       const int axis,
             	       const int side,
                              GraphicsParameters & psp )
// =======================================================================================================
// /Description:
//    Plot shaded faces on a grid boundary.
// 
// =======================================================================================================
{

  // -- use the vertex array if we are plotting with an adjustment for the "displacement"
    const bool plotRectangular = c.isRectangular() && !psp.adjustGridForDisplacement && !(c->computedGeometry & MappedGrid::THEvertex);

    const int & domainDimension = c.numberOfDimensions();
    const int & rangeDimension = c.numberOfDimensions();

  // (is1,is2,is3) : offset in the tangential direction
    int is1= axis==axis1 ? 0 : 1;
    int is2= axis==axis2 ? 0 : 1;
    int is3= axis==axis3 ? 0 : 1;

    int in1= axis==axis1 ? -side : 0;
    int in2= axis==axis2 ? -side : 0;
    int in3= axis==axis3 ? -side : 0;

  // these shifts indicate the points on the face
    int i1a,i2a,i3a,i1b,i2b,i3b,i1c,i2c,i3c;
    if( axis==axis1 )
    {
        i1a=0, i2a=1, i3a=0;
        i1b=0, i2b=0, i3b=1;
        i1c=0, i2c=1, i3c=1;
    }    
    else if( axis==axis2 )
    {
        i1a=1, i2a=0, i3a=0;
        i1b=0, i2b=0, i3b=1;
        i1c=1, i2c=0, i3c=1;
    }
    else
    {
        i1a=1, i2a=0, i3a=0;
        i1b=0, i2b=1, i3b=0;
        i1c=1, i2c=1, i3c=0;
    }

    
    
    Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
    I1a=Range(I1.getBase(),I1.getBound()-is1); 
    I2a=Range(I2.getBase(),I2.getBound()-is2); 
    I3a=Range(I3.getBase(),I3.getBound()-is3); 


    real normal[3];
    int iv3[3],iv[3];
    int & i1 = iv[0];
    int & i2 = iv[1];
    int & i3 = iv[2];

    int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
    const int maskDim0 = mask.getRawDataSize(0);
    const int maskDim1 = mask.getRawDataSize(1);
    const int maskDim2 = mask.getRawDataSize(2);
#define MASK(i0,i1,i2) maskp[((i0)+maskDim0*((i1)+maskDim1*((i2))))]


    if( plotRectangular )
    {
    // plotShaded(rectangular); // macro
    //     #If "rectangular" == "rectangular"
            real dx[3],xab[2][3];
            c.getRectangularGridParameters( dx, xab );
            const int i0a=c.gridIndexRange(0,0);
            const int i1a=c.gridIndexRange(0,1);
            const int i2a=c.gridIndexRange(0,2);
            const real xa=xab[0][0], dx0=dx[0];
            const real ya=xab[0][1], dy0=dx[1];
            const real za=xab[0][2], dz0=dx[2];
            normal[0]=0.; normal[1]=0.; normal[2]=0.;
            normal[axis]=1.; //  assumes right-handed
        #define VERTEX0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
        #define VERTEX1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))
        #define VERTEX2(i0,i1,i2) ZSCALE(za+dz0*(i2-i2a))
            FOR_3(i1,i2,i3,I1a,I2a,I3a)
            {  // ---plot a face that is parallel to axis
                if( (bool)c.isAllVertexCentered() )
                { // do not plot any cell that has unused points
                    if( psp.plotInterpolationCells )
                    {
              	if( MASK_UIB(i1    ,i2    ,i3    ) || MASK_UIB(i1+i1a,i2+i2a,i3+i3a)  ||
                  	    MASK_UIB(i1+i1b,i2+i2b,i3+i3b) || MASK_UIB(i1+i1c,i2+i2c,i3+i3c) )
                	  continue;  // skip this point
                    }
                    else
                    { // only plot cells that have at least 1 discretization point.
              	if( MASK(i1    ,i2    ,i3    )<=0 && MASK(i1+i1a,i2+i2a,i3+i3a)<=0  &&
                  	    MASK(i1+i1b,i2+i2b,i3+i3b)<=0 && MASK(i1+i1c,i2+i2c,i3+i3c)<=0 )
                	  continue;  // skip this point
                    }
                }
                else 
                {
                    if( psp.plotInterpolationCells )
                    {
                        if( MASK_UIB(i1+in1,i2+in2,i3+in3) )  // use this for cell centred grid
                            continue;  // skip this point
                    }
                    else 
                    {
                        if( MASK(i1+in1,i2+in2,i3+in3)<=0 )
                	  continue;
                    }
                }
                if( isHiddenByRefinement>0 )
                {
                    if( (MASK(i1    ,i2    ,i3    )&isHiddenByRefinement) || (MASK(i1+i1a,i2+i2a,i3+i3a)&isHiddenByRefinement)  ||
                	  (MASK(i1+i1b,i2+i2b,i3+i3b)&isHiddenByRefinement) || (MASK(i1+i1c,i2+i2c,i3+i3c)&isHiddenByRefinement) )
              	continue;  // skip this point
                }
                int axisp1=(axis+1) % 3;
                int axisp2=(axis+2) % 3;
        //  ....loop around the 4 vertices of the face, direction axis is fixed
                glBegin(GL_POLYGON);
                for( int i=0; i<4; i++ )
                {
                    iv3[axis  ]=iv[axis];
                    iv3[axisp1]=iv[axisp1]+ ( ((i+1)/2) % 2 );
                    iv3[axisp2]=iv[axisp2]+ ( ((i  )/2) % 2);
          //     #If "rectangular" == "curvilinear"
                    glNormal3v(normal);
                    glVertex3(VERTEX0(iv3[0],iv3[1],iv3[2]),
                    		VERTEX1(iv3[0],iv3[1],iv3[2]),
                    		VERTEX2(iv3[0],iv3[1],iv3[2]));
                }
                glEnd();  // GL_POLYGON
            }
        #undef VERTEX0
        #undef VERTEX1
        #undef VERTEX2
    }
    else
    {
    // plotShaded(curvilinear); // macro
    //     #If "curvilinear" == "rectangular"
    //     #Else    
            real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
            const int vertexDim0 = vertex.getRawDataSize(0);
            const int vertexDim1 = vertex.getRawDataSize(1);
            const int vertexDim2 = vertex.getRawDataSize(2);
            int xBase[3]={vertex.getBase(0),vertex.getBase(1),vertex.getBase(2)};  //
            int xBound[3]={vertex.getBound(0),vertex.getBound(1),vertex.getBound(2)};  //
        #define VERTEX0(i0,i1,i2) XSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((0)))))])
        #define VERTEX1(i0,i1,i2) YSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((1)))))])
        #define VERTEX2(i0,i1,i2) ZSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((2)))))])
    // define VERTEX(i0,i1,i2,i3) vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((i3)))))]
            FOR_3(i1,i2,i3,I1a,I2a,I3a)
            {  // ---plot a face that is parallel to axis
                if( (bool)c.isAllVertexCentered() )
                { // do not plot any cell that has unused points
                    if( psp.plotInterpolationCells )
                    {
              	if( MASK_UIB(i1    ,i2    ,i3    ) || MASK_UIB(i1+i1a,i2+i2a,i3+i3a)  ||
                  	    MASK_UIB(i1+i1b,i2+i2b,i3+i3b) || MASK_UIB(i1+i1c,i2+i2c,i3+i3c) )
                	  continue;  // skip this point
                    }
                    else
                    { // only plot cells that have at least 1 discretization point.
              	if( MASK(i1    ,i2    ,i3    )<=0 && MASK(i1+i1a,i2+i2a,i3+i3a)<=0  &&
                  	    MASK(i1+i1b,i2+i2b,i3+i3b)<=0 && MASK(i1+i1c,i2+i2c,i3+i3c)<=0 )
                	  continue;  // skip this point
                    }
                }
                else 
                {
                    if( psp.plotInterpolationCells )
                    {
                        if( MASK_UIB(i1+in1,i2+in2,i3+in3) )  // use this for cell centred grid
                            continue;  // skip this point
                    }
                    else 
                    {
                        if( MASK(i1+in1,i2+in2,i3+in3)<=0 )
                	  continue;
                    }
                }
                if( isHiddenByRefinement>0 )
                {
                    if( (MASK(i1    ,i2    ,i3    )&isHiddenByRefinement) || (MASK(i1+i1a,i2+i2a,i3+i3a)&isHiddenByRefinement)  ||
                	  (MASK(i1+i1b,i2+i2b,i3+i3b)&isHiddenByRefinement) || (MASK(i1+i1c,i2+i2c,i3+i3c)&isHiddenByRefinement) )
              	continue;  // skip this point
                }
                int axisp1=(axis+1) % 3;
                int axisp2=(axis+2) % 3;
        //  ....loop around the 4 vertices of the face, direction axis is fixed
                glBegin(GL_POLYGON);
                for( int i=0; i<4; i++ )
                {
                    iv3[axis  ]=iv[axis];
                    iv3[axisp1]=iv[axisp1]+ ( ((i+1)/2) % 2 );
                    iv3[axisp2]=iv[axisp2]+ ( ((i  )/2) % 2);
          //     #If "curvilinear" == "curvilinear"
                    ::getNormal(vertexp,vertexDim0,vertexDim1,vertexDim2,xBase,xBound,iv3,axis,axisp1,axisp2,normal);
                    glNormal3v(normal);
                    glVertex3(VERTEX0(iv3[0],iv3[1],iv3[2]),
                    		VERTEX1(iv3[0],iv3[1],iv3[2]),
                    		VERTEX2(iv3[0],iv3[1],iv3[2]));
                }
                glEnd();  // GL_POLYGON
            }
        #undef VERTEX0
        #undef VERTEX1
        #undef VERTEX2
    }
}


#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  
#define ForBoundary(side,axis)   for( axis=0; axis<gc.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

//  cface : a face of computational points and/or interpolation points
#define CFACE(n1,n2,n3,i1,i2,i3) ( MASK(i1   ,i2   ,i3   )!=0 &&  MASK(i1+n1,i2+n2,i3   )!=0 &&  MASK(i1   ,i2+n2,i3+n3)!=0 &&  MASK(i1+n1,i2   ,i3+n3)!=0     )
//  bnd : true boundary
#define BND(n1,n2,n3,i1,i2,i3)  (  MASK(i1   ,i2   ,i3   )>0 ||  MASK(i1+n1,i2+n2,i3   )>0 ||  MASK(i1   ,i2+n2,i3+n3)>0 ||  MASK(i1+n1,i2   ,i3+n3)>0     )
//  iface : interior interpolation face
#define IFACE(m1,m2,m3,n1,n2,n3,i1,i2,i3) ( MASK(i1+m1   ,i2+m2   ,i3+m3   )==0 ||  MASK(i1+m1+n1,i2+m2+n2,i3+m3   )==0 ||  MASK(i1+m1   ,i2+m2+n2,i3+m3+n3)==0 ||  MASK(i1+m1+n1,i2+m2   ,i3+m3+n3)==0 ||  MASK(i1-m1   ,i2-m2   ,i3-m3   )==0 ||  MASK(i1-m1+n1,i2-m2+n2,i3-m3   )==0 ||  MASK(i1-m1   ,i2-m2+n2,i3-m3+n3)==0 ||  MASK(i1-m1+n1,i2-m2   ,i3-m3+n3)==0     )

// This is for vertex centered only


// ************************************************************************************
//    ********** plotStructured3d ************
// 
// GRIDTYPE: rectangular or curvilinear
// ************************************************************************************

void PlotIt:: 
grid3d(GenericGraphicsInterface &gi, 
              const GridCollection & gc, 
              GraphicsParameters & psp, 
              IntegerArray & boundaryConditionList, 
              int numberOfBoundaryConditions,
              IntegerArray & numberList, 
              int & number, int list, int lightList)
// ======================================================================================================
// /Description:
//   Plot a three dimensional grid.
// ======================================================================================================
{

  //  GLenum errCode;
  //  const GLubyte *errString;
    const int myid=max(0,Communication_Manager::My_Process_Number);
    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int graphicsProcessor = gi.getProcessorForGraphics();
    const bool plotOnThisProcessor = Communication_Manager::localProcessNumber()==gi.getProcessorForGraphics();


    const int numberOfGrids = psp.plotRefinementGrids ? gc.numberOfComponentGrids() : gc.numberOfBaseGrids();

    IntegerArray & gridsToPlot        = psp.gridsToPlot;
    bool & plotInterpolationPoints= psp.plotInterpolationPoints;
    bool & plotBackupInterpolationPoints= psp.plotBackupInterpolationPoints;
  // bool & labelBoundaries        = psp.labelBoundaries        ;
    bool & plotBranchCuts         = psp.plotBranchCuts;
    IntegerArray & gridOptions        = psp.gridOptions;
  // RealArray & plotBound         = psp.plotBound;
    IntegerArray & gridBoundaryConditionOptions = psp.gridBoundaryConditionOptions;

    bool plotLinesJoiningInterpolationPoints=FALSE;  // add to parameters
    int & numberOfGridCoordinatePlanes= psp.numberOfGridCoordinatePlanes; 
    IntegerArray & gridCoordinatePlane    = psp.gridCoordinatePlane;

    isHiddenByRefinement = psp.plotHiddenRefinementPoints || 
                                                  gc.numberOfRefinementLevels()<=1 ? 0 : MappedGrid::IShiddenByRefinement;

    bool ok;
    
    int side,axis;
    Index I1,I2,I3;
    real normal[3];
    int im[3],in[3],iv3[3];
    int iv[3], &i1 = iv[0], &i2 = iv[1], &i3 = iv[2];
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

  // save the state of the unstructured plotting flags (so that we can turn them on/off)
    int plotNodes = psp.plotUnsNodes;
    int plotFaces = psp.plotUnsFaces;
    int plotEdges = psp.plotUnsEdges;
    int plotBoundaryEdges = psp.plotUnsBoundaryEdges;

  // for faces that are toggled on or off
    const GraphicsParameters::GridOptions 
        faceToToggle[6]={GraphicsParameters::doNotPlotFace00,GraphicsParameters::doNotPlotFace10,
                 		     GraphicsParameters::doNotPlotFace01,GraphicsParameters::doNotPlotFace11,
                 		     GraphicsParameters::doNotPlotFace02,GraphicsParameters::doNotPlotFace12}; //

    const GraphicsParameters::GridOptions 
        toggleGridLinesOnFace[6]={
                                          GraphicsParameters::doNotPlotGridLinesOnFace00,GraphicsParameters::doNotPlotGridLinesOnFace10,
                 		     GraphicsParameters::doNotPlotGridLinesOnFace01,GraphicsParameters::doNotPlotGridLinesOnFace11,
                 		     GraphicsParameters::doNotPlotGridLinesOnFace02,GraphicsParameters::doNotPlotGridLinesOnFace12}; //

  // loop over all grids and plot with LIGHTS ON
    psp.plotUnsNodes = FALSE;
    psp.plotUnsEdges = FALSE;
    psp.plotUnsBoundaryEdges = FALSE;

    if( plotOnThisProcessor ) glNewList(lightList,GL_COMPILE);

    #ifdef USE_PPP
  // define a partition for a distributed array that only lives on the graphicsProcessor
  // No need to include parallel ghost
    Partitioning_Type partition; 
    partition.SpecifyProcessorRange(Range(graphicsProcessor,graphicsProcessor)); 
    for( int axis=0; axis<4; axis++ )
    {
        int ghost=0; // uPartition.getGhostBoundaryWidth(axis);
        if( ghost>0 )
            partition.partitionAlongAxis(axis, true , ghost);
        else
            partition.partitionAlongAxis(axis, false, 0);
    }
    #endif

    int totalNumberOfBackupInterpolationPoints=0;
    bool plotBackupInterp=false;
    
    for( int grid=0; grid<numberOfGrids; grid++ )
    {
        if( !(gridsToPlot(grid)&GraphicsParameters::toggleGrids) )
            continue;

        const MappedGrid & c = gc[grid];

    // -- use the vertex array if we are plotting with an adjustment for the "displacement"
        const bool plotRectangular = c.isRectangular() && !psp.adjustGridForDisplacement && !(c->computedGeometry & MappedGrid::THEvertex);

        if( plotOnThisProcessor ) glPushName(c.getGlobalID()); // assign a name for picking

        if( c.getGridType()==MappedGrid::unstructuredGrid )
        {
      //      printf("plot: plotting lit part of an unstructured grid...\n");
            plotUnstructured(gi, (UnstructuredMapping&)(c.mapping().getMapping()), psp);
        }
        else
        {
       // from here on, we have a structured grid

            if( psp.gridOptions(grid) & GraphicsParameters::plotInteriorBoundary )
      	ISinteriorBoundaryPoint=0;
            else
                ISinteriorBoundaryPoint=MappedGrid::ISinteriorBoundaryPoint;

      // const IntegerDistributedArray & mask = c.mask();
            const IntegerArray & gridIndexRange = c.gridIndexRange();

            getIndex(gridIndexRange,I1,I2,I3);

      //
      // Loop over boundaries and plot as appropriate
      //
      // offset grids lines from shaded surface
            if( plotOnThisProcessor ) 
            {
      	glEnable(GL_POLYGON_OFFSET_FILL); // offset lines from surfaces so we can see them
      	glPolygonOffset(1.0, psp.surfaceOffset*OFFSET_FACTOR); 
            }
            
      // printf("10:POLYGON_OFFSET_FACTOR=%f\n", psp.surfaceOffset);

      // ********************************************************
      // ************ plot shaded boundaries ********************
      // ********************************************************
            Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];


            for( int p=0; p<np; p++ ) // loop over processors
            {
      	
              #ifndef USE_PPP
      	const intSerialArray & mask = c.mask();
      	const realSerialArray & vertex = c.vertex();
              #else

      	intSerialArray mask; 
      	intArray maskd;  // holds distributed array that just lives on the graphicsProcessor
      	IndexBox pBox;
      	const int nd=4;
      	Index Jv[nd];

      	if( p==graphicsProcessor )
      	{
        	  getLocalArrayWithGhostBoundaries(c.mask(),mask);
      	}
      	else
      	{

	  // CopyArray::getLocalArrayBoxWithGhost( p, c.mask(), pBox ); // get local bounds of the array on processor p 
        	  CopyArray::getLocalArrayBox( p, c.mask(), pBox ); // get local bounds of the array on processor p 

        	  if( pBox.isEmpty() ) continue; // this is ok since pBox is the same on all processors
         	   
        	  for( int d=0; d<3; d++ )	     
        	  {
                        int ja=pBox.base(d), jb=pBox.bound(d);
            // copy an extra line on internal ghost boundaries to avoid a gap
	    // if( ja>gridIndexRange(0,d) ) ja--;
          	    if( jb<gridIndexRange(1,d) ) jb++;
          	    Jv[d]=Range(ja,jb);
        	  }
        	  
        	  Jv[3]=Range(0,0);
        	  maskd.partition(partition);
        	  maskd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
        	  ParallelUtility::copy(maskd,Jv,c.mask(),Jv,nd); // copy data from processor p to graphics processor
        	  getLocalArrayWithGhostBoundaries(maskd,mask);

      	}

      	realSerialArray vertex; 
      	realArray vertexd; // holds distributed array that just lives on the graphicsProcessor
      	if( !plotRectangular )
      	{
        	  if( p==graphicsProcessor )
        	  {
          	    getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
        	  }
        	  else
        	  {
          	    Jv[3]=Range(0,gc.numberOfDimensions()-1); // copy (x,y,z)
          	    vertexd.partition(partition);
          	    vertexd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
          	    ParallelUtility::copy(vertexd,Jv,c.vertex(),Jv,nd); // copy data from processor p to graphics processor
          	    getLocalArrayWithGhostBoundaries(vertexd,vertex);
        	  }
      	}
              #endif

      	if( !plotOnThisProcessor ) continue;

      	int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
      	const int maskDim0 = mask.getRawDataSize(0);
      	const int maskDim1 = mask.getRawDataSize(1);
      	const int maskDim2 = mask.getRawDataSize(2);
#define MASK(i0,i1,i2) maskp[((i0)+maskDim0*((i1)+maskDim1*((i2))))]

      	for( axis=0; axis<gc.numberOfDimensions(); axis++ )for( side=0; side<=1; side++ )
      	{
          // printf(" plot shaded faces: grid=%i side,axis=%i,%i gridOptions(grid)&faceToToggle[side+2*axis]=%i\n",
          //  	 grid,side,axis,int(gridOptions(grid)&faceToToggle[side+2*axis]));
        	  
        	  int bcIndex = bcNumber(c.boundaryCondition(side,axis), boundaryConditionList, numberOfBoundaryConditions);
        	  if( (gridOptions(grid) & GraphicsParameters::plotShadedSurfaces) && 
            	      (c.boundaryCondition(side,axis)>0 || 
             	       psp.plotNonPhysicalBoundaries && c.boundaryCondition(side,axis)==0) &&
            	      gridBoundaryConditionOptions(bcIndex) & 1  &&
	      !(gridOptions(grid)&faceToToggle[side+2*axis]) ) 
        	  {
                        const IntegerArray & egir = extendedGridIndexRange(c);
          	    getBoundaryIndex(egir,side,axis,I1a,I2a,I3a);

          	    for( int dir=0; dir<gc.numberOfDimensions(); dir++ )
          	    {
              // Add an extra line in the tangential direction on periodic boundaries 
            	      if( c.isPeriodic(dir)==Mapping::functionPeriodic )
            		Iva[dir]=Range(Iva[dir].getBase(),Iva[dir].getBound()+1);
          	    }
          	    Iva[axis]=c.gridIndexRange(side,axis);
      	
          	    if( psp.boundaryColourOption==GraphicsParameters::defaultColour || 
            		psp.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition )
          	    {
            	      gi.setColour(gi.getColourName(max(0,gc[grid].boundaryCondition(side,axis) %
                                    						GenericGraphicsInterface::numberOfColourNames)));
            	      if( p==0 ) numberList(++number)=c.boundaryCondition(side,axis);  // keep track of what is plotted
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByShare )
          	    {
            	      gi.setColour(gi.getColourName(max(0,gc[grid].sharedBoundaryFlag(side,axis) %
                                    						GenericGraphicsInterface::numberOfColourNames)));
            	      if( p==0 ) numberList(++number)=c.sharedBoundaryFlag(side,axis);  // keep track of what is plotted
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByGrid )
          	    {
            	      gi.setColour(gi.getColourName(grid % GenericGraphicsInterface::numberOfColourNames));
            	      if( p==0 ) numberList(++number)=grid;
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByDomain )
          	    {
            	      gi.setColour(gi.getColourName(gc.domainNumber(grid) % GenericGraphicsInterface::numberOfColourNames));
            	      if( p==0 ) numberList(++number)=gc.domainNumber(grid);
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByRefinementLevel )
          	    {
            	      gi.setColour(gi.getColourName(max(0,gc.refinementLevelNumber(grid) %
                                    						GenericGraphicsInterface::numberOfColourNames)));
            	      if( p==0 ) numberList(++number)=gc.refinementLevelNumber(grid);
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByValue )
          	    {
            	      gi.setColour(gi.getColourName(max(0,psp.boundaryColourValue %
                                    						GenericGraphicsInterface::numberOfColourNames)));
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourByIndex )
          	    {
	      // gi.setColour(getXColour(psp.gridColours(grid)));
            	      gi.setColour(getGridColour(0,side,axis,grid,gc,gi,psp));
          	    }
          	    else if( psp.boundaryColourOption==GraphicsParameters::colourBlack )
          	    {
            	      gi.setColour(GenericGraphicsInterface::textColour);
          	    }
          	    else
          	    {
            	      printf("GL_GraphicsInterface::plot(3d grid):ERROR: unknown value of psp.boundaryColourOption \n");
//	      setColour(colourNames[min(grid,numberOfColourNames-1)]);
            	      gi.setColour("");
          	    }
          	    assert( number<=numberList.getBound(0) );


            // We need to add an extra line in the tangential directions on parallel ghost boundaries
                        #ifdef USE_PPP

          	    const int includeGhost=1; // in this case (I1a,I2a,I3a) will be restricted to the bounds of the mask array
                	    bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),mask,I1a,I2a,I3a,includeGhost); 
                	    if( !ok ) continue;

          	    if( false )
          	    {
            	      printf(" G3d: myid=%i grid=%i plotShadedFace: Iva=[%i,%i][%i,%i][%i,%i] Jv=[%i,%i][%i,%i][%i,%i] mask=[%i,%i][%i,%i][%i,%i]\n",
                 		     myid,grid,
                 		     I1a.getBase(),I1a.getBound(),I2a.getBase(),I2a.getBound(),I3a.getBase(),I3a.getBound(),
                 		     Jv[0].getBase(),Jv[0].getBound(),
                 		     Jv[1].getBase(),Jv[1].getBound(),
                 		     Jv[2].getBase(),Jv[2].getBound(),
                 		     mask.getBase(0),mask.getBound(0),
                 		     mask.getBase(1),mask.getBound(1),
                 		     mask.getBase(2),mask.getBound(2));
           	     
            	      fflush(0);
          	    }
          	    
                        #endif

          	    plotShadedFace(gi, c, mask,vertex, I1a, I2a, I3a, axis, side, psp );

        	  }
      	} // end for axis, for side
            } // end loop over processors
            
            if( plotOnThisProcessor ) glDisable(GL_POLYGON_OFFSET_FILL);
    
        }
        
        
        if( plotOnThisProcessor ) glPopName();
    } // end for grid=0,1,2,...
    
    if( plotOnThisProcessor ) glEndList();  // close the list with lighting

  //
  // From here on, we plot without lighing
  //
    psp.plotUnsFaces = FALSE;
    
    psp.plotUnsNodes = plotNodes;
    psp.plotUnsEdges = plotEdges;
    psp.plotUnsBoundaryEdges = plotBoundaryEdges;


    if( plotOnThisProcessor ) glNewList(list,GL_COMPILE);

  // offset grids lines from shaded surface
    if( plotOnThisProcessor ) 
    {
        glEnable(GL_POLYGON_OFFSET_FILL); // offset lines from surfaces so we can see them
        glPolygonOffset(1.0,1.0*OFFSET_FACTOR); 
    }
    
  // loop over all grids
    for( int grid=0; grid<numberOfGrids; grid++ )
    {
        const MappedGrid & c = gc[grid];

        if( plotOnThisProcessor ) glPushName(c.getGlobalID()); // assign a name for picking

        if( c.getGridType()==MappedGrid::unstructuredGrid && (gridsToPlot(grid)&GraphicsParameters::toggleGrids) )
        {
      //      printf("plot: plotting unlit part of an unstructured grid...\n");	      
            plotUnstructured(gi, (UnstructuredMapping&)(c.mapping().getMapping()), psp);
        }
        else
        {
      // from here on, we have a structured grid

      // -- use the vertex array if we are plotting with an adjustment for the "displacement"
            const bool plotRectangular = c.isRectangular() && !psp.adjustGridForDisplacement && !(c->computedGeometry & MappedGrid::THEvertex);

            const IntegerArray & gridIndexRange = c.gridIndexRange();
            const bool isVertexCentered = (bool)c.isAllVertexCentered();

            for( int p=0; p<np; p++ ) // loop over processors
            {
      	
#ifndef USE_PPP
      	const intSerialArray & mask = c.mask();
      	const realSerialArray & vertex = c.vertex();
#else

      	intSerialArray mask; 
      	intArray maskd;  // holds distributed array that just lives on the graphicsProcessor
      	IndexBox pBox;
      	const int nd=4;
      	Index Jv[nd];

      	if( p==graphicsProcessor )
      	{
        	  getLocalArrayWithGhostBoundaries(c.mask(),mask);
      	}
      	else
      	{

	  // CopyArray::getLocalArrayBoxWithGhost( p, u, pBox ); // get local bounds of the array on processor p 
        	  CopyArray::getLocalArrayBox( p, c.mask(), pBox ); // get local bounds of the array on processor p 

        	  if( pBox.isEmpty() ) continue;
         	   
        	  for( int d=0; d<3; d++ )	     
        	  {
                        int ja=pBox.base(d), jb=pBox.bound(d);
            // copy an extra line on internal ghost boundaries to avoid a gap
	    // if( ja>gridIndexRange(0,d) ) ja--; 
          	    if( jb<gridIndexRange(1,d) ) jb++;
          	    Jv[d]=Range(ja,jb);
        	  }

        	  Jv[3]=Range(0,0);
        	  maskd.partition(partition);
        	  maskd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
        	  ParallelUtility::copy(maskd,Jv,c.mask(),Jv,nd); // copy data from processor p to graphics processor
        	  getLocalArrayWithGhostBoundaries(maskd,mask);
      	}

      	realSerialArray vertex; 
      	realArray vertexd; // holds distributed array that just lives on the graphicsProcessor
      	if( !plotRectangular )
      	{
        	  if( p==graphicsProcessor )
        	  {
          	    getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
        	  }
        	  else
        	  {
          	    Jv[3]=Range(0,gc.numberOfDimensions()-1); // copy (x,y,z)
          	    vertexd.partition(partition);
          	    vertexd.redim(Jv[0],Jv[1],Jv[2],Jv[3]);
          	    ParallelUtility::copy(vertexd,Jv,c.vertex(),Jv,nd); // copy data from processor p to graphics processor
          	    getLocalArrayWithGhostBoundaries(vertexd,vertex);
        	  }
      	}
#endif

      	int *maskp = mask.Array_Descriptor.Array_View_Pointer2;
      	const int maskDim0 = mask.getRawDataSize(0);
      	const int maskDim1 = mask.getRawDataSize(1);
      	const int maskDim2 = mask.getRawDataSize(2);

      	getIndex(gridIndexRange,I1,I2,I3);

      	Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
	// *************************************
	// ****** plot block boundaries UNLIT **
	// *************************************


      	if( plotRectangular )
      	{
        // 	  plotStructured3d(rectangular); 
        //         #If "rectangular" == "rectangular"
                    real dx[3],xab[2][3];
                    c.getRectangularGridParameters( dx, xab );
                    const int i0a=c.gridIndexRange(0,0);
                    const int i1a=c.gridIndexRange(0,1);
                    const int i2a=c.gridIndexRange(0,2);
                    const real xa=xab[0][0], dx0=dx[0];
                    const real ya=xab[0][1], dy0=dx[1];
                    const real za=xab[0][2], dz0=dx[2];
                #define VERTEX0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
                #define VERTEX1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))
                #define VERTEX2(i0,i1,i2) ZSCALE(za+dz0*(i2-i2a))
          // -----------------------------
          // --- plot Block boundaries ---
          // -----------------------------
                    if( gridOptions(grid) & GraphicsParameters::plotBlockBoundaries && plotOnThisProcessor )
                    {
                        gi.setColour(getGridColour(2,0,0,grid,gc,gi,psp));
                        if( psp.blockBoundaryColourOption==GraphicsParameters::colourByGrid )
                        {
                            if( p==0 ) numberList(++number)=grid;
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByDomain ) 
                        {
                            if( p==0 ) numberList(++number)=gc.domainNumber(grid);
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByRefinementLevel ) 
                        {
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByIndex ) 
                        {
                            if( p==0 ) numberList(++number)=psp.gridColours(grid,7);
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::defaultColour || true )
                        {
                        }
                        glLineWidth(psp.size(GraphicsParameters::lineWidth)*2.*gi.getLineWidthScaleFactor());
                        Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
                        for(axis=axis1; axis<gc.numberOfDimensions(); axis++ ) // plot lines parallel to axis
                        {
                            int axisp1 = (axis+1) % 3;
                            int axisp2 = (axis+2) % 3;
                            for( int i=0; i<=1; i++ )   // there are 4 lines parallel
                            {
                      	for( int j=0; j<=1; j++ )   // to this axis
                      	{
                                    Jv[axis]=Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
                                    Jv[axisp1]=gridIndexRange(i,axisp1);
                                    Jv[axisp2]=gridIndexRange(j,axisp2);
                        	  const int includeGhost=1;
                        	  bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),mask,J1,J2,J3,includeGhost);
                        	  if( !ok ) continue;
                  // include an extra line on parallel ghost boundaries 
        	  // ** if( Jv[axis].getBound()!=gridIndexRange(1,axis) ) Jv[axis]=Range(Jv[axis].getBase(),Jv[axis].getBound()+1);
                        	  glBegin(GL_LINE_STRIP);
                        	  FOR_3(i1,i2,i3,J1,J2,J3)
                        	  {
                          	    glVertex3( VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3) );
                        	  }
                        	  glEnd();     // GL_LINE_STRIP
                      	}
                            }
                        }
                    } // end plot block boundaries 
          // ********************************************************
          // ************ plot lines on boundaries UNLIT ************
          // ********************************************************
                    if( (gridsToPlot(grid)&GraphicsParameters::toggleGrids) && plotOnThisProcessor )
                    {
                        gi.setColour(getGridColour(1,0,0,grid,gc,gi,psp));
            // **** The first 6 times through this loop we try to plot lines on grid boundaries,
            // **** after that we plot lines on specified coordinate planes.
                        int index;
                        for( int plane=-6; plane<numberOfGridCoordinatePlanes; plane++ )
                        {
                            if( plane<0 )
                            {
                      	side=(plane+6) % 2;
                      	axis=(plane+6)/2;
                      	index=0;
                      	int bcIndex=bcNumber(c.boundaryCondition(side,axis), boundaryConditionList, numberOfBoundaryConditions);
                      	if( !( 
                            	      (gridOptions(grid) & GraphicsParameters::plotBoundaryGridLines) && 
                            	      ( ( (gridBoundaryConditionOptions(bcIndex) & 1) && !(gridOptions(grid)&toggleGridLinesOnFace[side+2*axis]) )
                            		|| (plotBranchCuts && c.boundaryCondition(side,axis)<0 ) 
                            		|| (psp.plotNonPhysicalBoundaries && c.boundaryCondition(side,axis)==0)
                            		) ) )
                      	{
                        	  continue;  // skip this side.
                      	}
                            }
                            else if( gridCoordinatePlane(0,plane)==grid && (gridOptions(grid) &GraphicsParameters::plotInteriorGridLines) )
                            {
                      	axis=gridCoordinatePlane(1,plane);
                      	index=gridCoordinatePlane(2,plane);
                      	side=Start;
                      	if( index >= c.gridIndexRange(End,axis) )
                      	{ // do this for CC grids and mask (see below)
                        	  side=End;
                        	  index-=c.gridIndexRange(End,axis);
                      	}
                      	if( axis<0 || axis>2 || index<c.gridIndexRange(Start,axis) || index > c.gridIndexRange(End,axis) )
                      	{
                        	  printf("ERROR: there are invalid values specifying a coordinate plane, grid=%i, axis=%i, index=%i \n",
                             		 grid,axis,index);
                        	  continue;
                      	}
                            }
                            else
                            {
        	// printf(" plane=%i, coordinatePlane(0,plane)=%i \n",plane,coordinatePlane(0,plane));
                      	continue;
                            }
              // *wdh* 050820: added ghost lines
                            getBoundaryIndex(extendedGridIndexRange(c),side,axis,I1a,I2a,I3a,psp.numberOfGhostLinesToPlot);
                            int dir;
                            for( dir=0; dir<gc.numberOfDimensions(); dir++ )
                            {
        	// *wdh* 061115 -- remove periodic fix, limit bounds by dimension
        // 	    if( c.isPeriodic(dir)==Mapping::functionPeriodic )
        // 	      Iva[dir]=Range(Iva[dir].getBase(),Iva[dir].getBound()+1);
                      	Iva[dir]=Range(max(Iva[dir].getBase(),c.dimension(0,dir)),min(Iva[dir].getBound(),c.dimension(1,dir)));
                            }
                            Iva[axis]=c.gridIndexRange(side,axis)+index;
              // We need to add an extra line in the tangential directions on parallel ghost boundaries
                            const int includeGhost=1;
                            bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),mask,I1a,I2a,I3a,includeGhost);
                            if( !ok ) continue;
                            glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                            glBegin(GL_LINES);
                            int in1= (isVertexCentered && axis==axis1) ? -side : 0;  // shift for mask
                            int in2= (isVertexCentered && axis==axis2) ? -side : 0;
                            int in3= (isVertexCentered && axis==axis3) ? -side : 0;
                            for( dir=1; dir<=2; dir++ )
                            {
                      	int axisp = (axis+dir) % 3;    // plot lines parallel to axis=axisp
                      	is1= axisp==axis1 ? 1 : 0;
                      	is2= axisp==axis2 ? 1 : 0;
                      	is3= axisp==axis3 ? 1 : 0;
                      	int ib1= (axis!=axis1 && axisp!=axis1) ? in1-1 : in1; 
                      	int ib2= (axis!=axis2 && axisp!=axis2) ? in2-1 : in2; 
                      	int ib3= (axis!=axis3 && axisp!=axis3) ? in3-1 : in3; 
                                Index Ivb[3], &I1b=Ivb[0], &I2b=Ivb[1], &I3b=Iva[2];
                      	I1b=Range(I1a.getBase(),I1a.getBound()-is1); 
                      	I2b=Range(I2a.getBase(),I2a.getBound()-is2); 
                      	I3b=Range(I3a.getBase(),I3a.getBound()-is3); 
                      	if( abs(gi.gridCoarseningFactor)==1 )
                      	{
                        	  FOR_3(i1,i2,i3,I1b,I2b,I3b)
                        	  {
                          	    if( isVertexCentered )
                          	    {
                            	      if( psp.plotInterpolationCells )
                            	      {
                            		if( MASK_UIB(i1,i2,i3) || MASK_UIB(i1+is1,i2+is2,i3+is3) )
                              		  continue;  // skip this line
                            	      }
                            	      else
                            	      {
                            		if( MASK(i1,i2,i3)<=0 && MASK(i1+is1,i2+is2,i3+is3)<=0 )
                              		  continue;  // skip this line
                            	      }
                          	    }
                          	    else 
                          	    {
                            	      if( psp.plotInterpolationCells )
                            	      {
                            		if( MASK_UIB(i1+in1,i2+in2,i3+in3) && MASK_UIB(i1+ib1,i2+ib2,i3+ib3) )
                              		  continue;    // skip this line
                            	      }
                            	      else
                            	      {
                            		if( MASK(i1+in1,i2+in2,i3+in3)<=0 && MASK(i1+ib1,i2+ib2,i3+ib3)<=0 )
                              		  continue;    // skip this line
                            	      }
                          	    }
                          	    if( isHiddenByRefinement>0 )
                          	    {
                            	      if( MASK(i1,i2,i3)&isHiddenByRefinement || MASK(i1+is1,i2+is2,i3+is3)&isHiddenByRefinement )
                            		continue;  // skip this point
                          	    }
                          	    glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                  		      VERTEX1(i1    ,i2    ,i3    ),
                                  		      VERTEX2(i1    ,i2    ,i3    ));
                          	    glVertex3(VERTEX0(i1+is1,i2+is2,i3+is3),
                                  		      VERTEX1(i1+is1,i2+is2,i3+is3),
                                  		      VERTEX2(i1+is1,i2+is2,i3+is3));
                        	  }
                      	}
                      	else // coarsen factor >1 
                      	{
        	  // ** new 050921 ***
        	  // strides: (m1,m2,m3)
                        	  const int m1=axisp!=axis1 ? abs(gi.gridCoarseningFactor) : 1; 
                        	  const int m2=axisp!=axis2 ? abs(gi.gridCoarseningFactor) : 1;
                        	  const int m3=axisp!=axis3 ? abs(gi.gridCoarseningFactor) : 1;
                        	  const int ms1=m1*is1;
                        	  const int ms2=m2*is2;
                        	  const int ms3=m3*is3;
                        	  const int j3b = min(i3+m3,I3b.getBound());
                        	  const int j2b = min(i2+m2,I2b.getBound());
                        	  const int j1b = min(i1+m1,I1b.getBound());
                        	  int i1Bound,i2Bound,i3Bound;
                        	  FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1b,I2b,I3b) 
                        	  {
        	    // Check that all sub-lines can be plotted, only then plot the coarser line segment
        	    // ** watch out for ends
                          	    bool ok=true && (i1+m1-1)<=i1Bound && (i2+m2-1)<=i2Bound && (i3+m3-1)<=i3Bound;
                          	    int j1,j2,j3=i3;
                          	    for( j3=i3; j3<=j3b && ok ; j3++ )
                            	      for( j2=i2; j2<=j2b && ok ; j2++ )
                            		for( j1=i1; j1<=j1b; j1++ )
                            		{
                      // 		  plotThisLineSegment(j1,j2,j3,is1,is2,is3,ok);
                                                if( psp.plotInterpolationCells )
                                                {
                                                    if( MASK_UIB(j1,j2,j3) || MASK_UIB(j1+is1,j2+is2,j3+is3) )
                                                        ok=false;  // skip this line
                                                }
                                                else
                                                {
                                                    if( MASK(j1,j2,j3)<=0 && MASK(j1+is1,j2+is2,j3+is3)<=0 )
                                                        ok=false;  // skip this line
                                                }
                                                if( isHiddenByRefinement>0 )
                                                {
                                                    if( MASK(j1,j2,j3)&isHiddenByRefinement || MASK(j1+is1,j2+is2,j3+is3)&isHiddenByRefinement )
                                                        ok=false;  // skip this line
                                                }
                              		  if( !ok )
                                		    break;
                            		}
                          	    if( ok )
                          	    {
        	      // plot longer line segment
                            	      glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                  			VERTEX1(i1    ,i2    ,i3    ),
                                  			VERTEX2(i1    ,i2    ,i3    ));
                            	      glVertex3(VERTEX0(i1+ms1,i2+ms2,i3+ms3),
                                  			VERTEX1(i1+ms1,i2+ms2,i3+ms3),
                                  			VERTEX2(i1+ms1,i2+ms2,i3+ms3));
                          	    }
                          	    else
                          	    {
        	      // plot individual line segements
                            	      const int i3b = min(i3+m3-1,i3Bound);
                            	      const int i2b = min(i2+m2-1,i2Bound);
                            	      const int i1b = min(i1+m1-1,i1Bound);
                            	      for( j3=i3; j3<=i3b; j3++ )
                            		for( j2=i2; j2<=i2b; j2++ )
                              		  for( j1=i1; j1<=i1b; j1++ )
                              		  {
                                		    ok=true;
                        // 		    plotThisLineSegment(j1,j2,j3,is1,is2,is3,ok);
                                                    if( psp.plotInterpolationCells )
                                                    {
                                                        if( MASK_UIB(j1,j2,j3) || MASK_UIB(j1+is1,j2+is2,j3+is3) )
                                                            ok=false;  // skip this line
                                                    }
                                                    else
                                                    {
                                                        if( MASK(j1,j2,j3)<=0 && MASK(j1+is1,j2+is2,j3+is3)<=0 )
                                                            ok=false;  // skip this line
                                                    }
                                                    if( isHiddenByRefinement>0 )
                                                    {
                                                        if( MASK(j1,j2,j3)&isHiddenByRefinement || MASK(j1+is1,j2+is2,j3+is3)&isHiddenByRefinement )
                                                            ok=false;  // skip this line
                                                    }
                                		    if( ok )
                                		    {
                                  		      glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                        				VERTEX1(i1    ,i2    ,i3    ),
                                        				VERTEX2(i1    ,i2    ,i3    ));
                                  		      glVertex3(VERTEX0(i1+is1,i2+is2,i3+is3),
                                        				VERTEX1(i1+is1,i2+is2,i3+is3),
                                        				VERTEX2(i1+is1,i2+is2,i3+is3));
                                		    }
                              		  }
                          	    }
                        	  }
                      	} // end coarsening factor >1 
                            } // end for dir...
                            glEnd();     // GL_LINES  ** moved
                        }
                    }   // end plot lines on boundaries 
          // ********************************************************
          // ************ plot interpolation points UNLIT ***********
          // ********************************************************
                    if( ( (gridOptions(grid) & GraphicsParameters::plotInterpolation)  ||
                                (gridOptions(grid) & GraphicsParameters::plotBackupInterpolation) )
                            && numberOfGrids > 1 && gc.getClassName()=="CompositeGrid" )
                    {
                        CompositeGrid & cg = (CompositeGrid &)gc;
            //             #If "rectangular" == "rectangular"
                          real dx[3],xab[2][3];
                          c.getRectangularGridParameters( dx, xab ); 
                          const int i0a=c.gridIndexRange(0,0);
                          const int i1a=c.gridIndexRange(0,1);
                          const int i2a=c.gridIndexRange(0,2);
                          real xa=xab[0][0], dx0=dx[0];
                          real ya=xab[0][1], dy0=dx[1];
                          real za=xab[0][2], dz0=dx[2];
                          if( !isVertexCentered )
                          {
                              xa+=dx0*.5;
                              ya+=dy0*.5;
                              za+=dz0*.5;
                          }
                          #define CENTER0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
                          #define CENTER1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))
                          #define CENTER2(i0,i1,i2) ZSCALE(za+dz0*(i2-i2a))
                        if( (plotInterpolationPoints || plotBackupInterpolationPoints ) && 
                      	(gridOptions(grid)&GraphicsParameters::plotInterpolation) &&
                      	numberOfGrids >= 1 && 
                      	gc.getClassName()=="CompositeGrid"  )
                        {
                            #ifndef USE_PPP
                              const intSerialArray & ip = cg.interpolationPoint[grid];
                              const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                            #else
               // -- parallel version --
               // copy the interpolation data from processor "p" to processor "graphicsProcessor"
               // *new* way *wdh* 110628 (from 2d version)
                              intSerialArray ip,interpoleeGrid;
                              collectInterpolationData( p,graphicsProcessor, grid,cg, ip,interpoleeGrid );
                              if( p!=graphicsProcessor )
                              {
        	 // -- for vertex centered grids we can re-use the VERTEX, otherwise we need the CENTER  ** fix me **
               //                  #If "rectangular" == "curvilinear"
                              }
                            #endif
              //               #If "rectangular" == "curvilinear"
                            if( ip.getLength(0)>0 )
                            {
                                const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
                                const int ipDim0=ip.getRawDataSize(0);
                                #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
        	// --- plot normal interpolation points ---
                      	if( plotInterpolationPoints && (gridOptions(grid) & GraphicsParameters::plotInterpolation) && 
                          	    cg.numberOfInterpolationPoints.getLength(0)>grid )
                      	{
        	  //  	  if ((errCode=glGetError()) != GL_NO_ERROR)
        	  //  	  {
        	  //  	    errString = gluErrorString(errCode);
        	  //  	    printf("grid3d: Start of plotInterpolation loop: OpenGL Error: %s\n", errString);
        	  //  	  }
                        	  gi.setColour(GenericGraphicsInterface::textColour);
                        	  glPointSize(psp.pointSize*1.67*gi.getLineWidthScaleFactor());   
                        	  glBegin(GL_POINTS);  
                        	  int oldInterpolationPointColour=-1;
        	  // for( int i=0; i<numberOfInterpolationPoints; i++ )
                        	  for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
                        	  {
                          	    if( psp.colourInterpolationPoints && interpoleeGrid(i)!=oldInterpolationPointColour )
                          	    {
        	      // colour the interpolation the same colour as the grid it interpolates from
                            	      oldInterpolationPointColour=interpoleeGrid(i);
                            	      setXColour(gi.getColourName( (oldInterpolationPointColour %
                                                  					    GenericGraphicsInterface::numberOfColourNames) ));
        	      // setXColour(gi.getColourName(min(interpoleeGrid(i),GenericGraphicsInterface::numberOfColourNames-1)));
                          	    }
                          	    glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  		      CENTER1(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  		      CENTER2(IP(i,axis1),IP(i,axis2),IP(i,axis3)));
                        	  }
                        	  glEnd();
                      	}
                      	gi.setColour(gi.getColourName(grid)); // colour by grid number
        	// if( gridOptions(grid) & GraphicsParameters::plotBackupInterpolation && 
        	// 	cg.interpolationPoint.getLength()>grid )
        	// *wdh* 110606
                      	if( plotBackupInterpolationPoints && (gridOptions(grid) & GraphicsParameters::plotInterpolation) && 
                          	    cg.numberOfInterpolationPoints.getLength(0)>grid )
                      	{
        	  // Now plot points that use back up interpolation
                        	  gi.setColour(GenericGraphicsInterface::textColour);
                        	  glPointSize(psp.pointSize*2.*gi.getLineWidthScaleFactor());   
                        	  glBegin(GL_POINTS);  
        	  // glPointSize(6.*lineWidthScaleFactor[currentWindow]);   
                        	  plotBackupInterp=true;
                        	  int numBackup=0;
        	  // for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
        	  // printF("grid3d: grid=%i ip=[%i,%i]\n",grid,ip.getBase(0),ip.getBound(0));
                                    for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
                        	  {
        	    // printF(" i=%i ip=(%i,%i,%i) mask=[%i,%i][%i,%i][%i,%i]\n",i,IP(i,0),IP(i,1),IP(i,2),
                    //        mask.getBase(0),mask.getBound(0), 
                    //        mask.getBase(1),mask.getBound(1), 
        	    // 	   mask.getBase(2),mask.getBound(2));
                          	    if( MASK(IP(i,axis1),IP(i,axis2),IP(i,axis3)) & CompositeGrid::USESbackupRules )
                          	    {
                            	      numBackup++;
                            	      glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  			CENTER1(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  			CENTER2(IP(i,axis1),IP(i,axis2),IP(i,axis3)));
                          	    }
                        	  }
                        	  totalNumberOfBackupInterpolationPoints+=numBackup;
                        	  if( numBackup>0 )
                          	    printF("plotGrid3d:There were %i backup interpolation points on grid %i (%s)\n",numBackup,grid,
                               		   (const char*)gc[grid].getName());
                        	  glEnd();
                      	}
                            }  // end if( ip.getLength(0)>0 )
                        } // end plot interp or backup interp
            // ***** Plot lines joining the interpolation points *******
                        if( plotLinesJoiningInterpolationPoints ) // **** this is FALSE for now **************
                        {
                            const IntegerArray & exGridIndexRange = extendedGridIndexRange(c);
              // getIndex(exGridIndexRange,I1a,I2a,I3a);
                            FOR_3(i1,i2,i3,I1,I2,I3)
                            {
                      	for( axis=axis1; axis<gc.numberOfDimensions(); axis++ )  // draw grid lines parallel to axis
                      	{
        	  //   ...Plot faces with iv(kd) fixed
                        	  im[0]=0; im[1]=0; im[2]=0; im[axis]=1;
                        	  in[0]=1; in[1]=1; in[2]=1; in[axis]=0;
                        	  if(iv[axis1]+in[axis1]<=exGridIndexRange(End,axis1) &&
                           	     iv[axis2]+in[axis2]<=exGridIndexRange(End,axis2) &&
                           	     iv[axis3]+in[axis3]<=exGridIndexRange(End,axis3) )
                        	  {
                          	    if( CFACE(in[axis1],in[axis2],in[axis3],i1,i2,i3) ) 
                          	    {
        	      //  ...face of computational points
        	      //  plot BC boundary, branch cut or interpolation
                            	      ok=FALSE;
                            	      if(iv[axis]==exGridIndexRange(Start,axis) || 
                             		 iv[axis]==exGridIndexRange(End  ,axis) )
                            	      {
        		//  ...On a Boundary
                            		side = iv[axis]==exGridIndexRange(Start,axis) ? Start : End;
                            		if( c.boundaryCondition()(side,axis)<0 )
                            		{
        		  // ...plot a branch cut boundary
                              		  ok=plotBranchCuts;
                            		}
                            		else if( BND(in[axis1],in[axis2],in[axis3],i1,i2,i3) )
                            		{
        		  //   ...plot a BC boundary
                              		  ok=FALSE;        // plotTrueBoundaries;
                            		}
                            		else
                            		{
        		  //   ...plot an interpolation face
                              		  ok=plotInterpolationPoints;
                            		}
                            	      }
                            	      if( !ok && plotInterpolationPoints )
                            	      {
        		// ...plot an interior interpolation face
                            		ok=IFACE(im[axis1],im[axis2],im[axis3],in[axis1],in[axis2],in[axis3],i1,i2,i3);
        		// side=Start;
                            	      }
                            	      if( ok )
                            	      {  // ---plot a face that is parallel to axis
                            		int axisp1=(axis+1) % 3;
                            		int axisp2=(axis+2) % 3;
        		//  ....loop around the 4 vertices of the face, direction axis is fixed
        		// glBegin(GL_POLYGON); // draw shaded polygon
                            		glBegin(GL_LINE_STRIP);  // draw lines only
                            		for( int i=0; i<4; i++ )
                            		{
                              		  iv3[axis  ]=iv[axis];
                              		  iv3[axisp1]=iv[axisp1]+ ( ((i+1)/2) % 2 );
                              		  iv3[axisp2]=iv[axisp2]+ ( ((i  )/2) % 2);
                      //         #If "rectangular" == "rectangular"
                              		  normal[0]=0.; normal[1]=0.; normal[2]=0.;
                              		  normal[axis]=1.;
                              		  glNormal3v(normal);
                              		  glVertex3(VERTEX0(iv3[0],iv3[1],iv3[2]),
                                      			    VERTEX1(iv3[0],iv3[1],iv3[2]),
                                      			    VERTEX2(iv3[0],iv3[1],iv3[2]));
                            		}
                            		glEnd();  
                            	      }
                          	    }
                        	  }
                      	}
                            }
                        }
                    } // end plot interpolation points
                #undef CENTER0
                #undef CENTER1
                #undef CENTER2
                #undef VERTEX0
                #undef VERTEX1
                #undef VERTEX2
      	}
      	else
      	{
        // 	  plotStructured3d(curvilinear); 
        //         #If "curvilinear" == "rectangular"
        //         #Else    
                    real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
                    const int vertexDim0 = vertex.getRawDataSize(0);
                    const int vertexDim1 = vertex.getRawDataSize(1);
                    const int vertexDim2 = vertex.getRawDataSize(2);
                    int xBase[3]={vertex.getBase(0),vertex.getBase(1),vertex.getBase(2)};  //
                    int xBound[3]={vertex.getBound(0),vertex.getBound(1),vertex.getBound(2)};  //
                #define VERTEX0(i0,i1,i2) XSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((0)))))])
                #define VERTEX1(i0,i1,i2) YSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((1)))))])
                #define VERTEX2(i0,i1,i2) ZSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((2)))))])
        // define VERTEX(i0,i1,i2,i3) vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((i3)))))]
          // -----------------------------
          // --- plot Block boundaries ---
          // -----------------------------
                    if( gridOptions(grid) & GraphicsParameters::plotBlockBoundaries && plotOnThisProcessor )
                    {
                        gi.setColour(getGridColour(2,0,0,grid,gc,gi,psp));
                        if( psp.blockBoundaryColourOption==GraphicsParameters::colourByGrid )
                        {
                            if( p==0 ) numberList(++number)=grid;
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByDomain ) 
                        {
                            if( p==0 ) numberList(++number)=gc.domainNumber(grid);
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByRefinementLevel ) 
                        {
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByIndex ) 
                        {
                            if( p==0 ) numberList(++number)=psp.gridColours(grid,7);
                        }
                        else if( psp.blockBoundaryColourOption==GraphicsParameters::defaultColour || true )
                        {
                        }
                        glLineWidth(psp.size(GraphicsParameters::lineWidth)*2.*gi.getLineWidthScaleFactor());
                        Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
                        for(axis=axis1; axis<gc.numberOfDimensions(); axis++ ) // plot lines parallel to axis
                        {
                            int axisp1 = (axis+1) % 3;
                            int axisp2 = (axis+2) % 3;
                            for( int i=0; i<=1; i++ )   // there are 4 lines parallel
                            {
                      	for( int j=0; j<=1; j++ )   // to this axis
                      	{
                                    Jv[axis]=Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
                                    Jv[axisp1]=gridIndexRange(i,axisp1);
                                    Jv[axisp2]=gridIndexRange(j,axisp2);
                        	  const int includeGhost=1;
                        	  bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),mask,J1,J2,J3,includeGhost);
                        	  if( !ok ) continue;
                  // include an extra line on parallel ghost boundaries 
        	  // ** if( Jv[axis].getBound()!=gridIndexRange(1,axis) ) Jv[axis]=Range(Jv[axis].getBase(),Jv[axis].getBound()+1);
                        	  glBegin(GL_LINE_STRIP);
                        	  FOR_3(i1,i2,i3,J1,J2,J3)
                        	  {
                          	    glVertex3( VERTEX0(i1,i2,i3),VERTEX1(i1,i2,i3),VERTEX2(i1,i2,i3) );
                        	  }
                        	  glEnd();     // GL_LINE_STRIP
                      	}
                            }
                        }
                    } // end plot block boundaries 
          // ********************************************************
          // ************ plot lines on boundaries UNLIT ************
          // ********************************************************
                    if( (gridsToPlot(grid)&GraphicsParameters::toggleGrids) && plotOnThisProcessor )
                    {
                        gi.setColour(getGridColour(1,0,0,grid,gc,gi,psp));
            // **** The first 6 times through this loop we try to plot lines on grid boundaries,
            // **** after that we plot lines on specified coordinate planes.
                        int index;
                        for( int plane=-6; plane<numberOfGridCoordinatePlanes; plane++ )
                        {
                            if( plane<0 )
                            {
                      	side=(plane+6) % 2;
                      	axis=(plane+6)/2;
                      	index=0;
                      	int bcIndex=bcNumber(c.boundaryCondition(side,axis), boundaryConditionList, numberOfBoundaryConditions);
                      	if( !( 
                            	      (gridOptions(grid) & GraphicsParameters::plotBoundaryGridLines) && 
                            	      ( ( (gridBoundaryConditionOptions(bcIndex) & 1) && !(gridOptions(grid)&toggleGridLinesOnFace[side+2*axis]) )
                            		|| (plotBranchCuts && c.boundaryCondition(side,axis)<0 ) 
                            		|| (psp.plotNonPhysicalBoundaries && c.boundaryCondition(side,axis)==0)
                            		) ) )
                      	{
                        	  continue;  // skip this side.
                      	}
                            }
                            else if( gridCoordinatePlane(0,plane)==grid && (gridOptions(grid) &GraphicsParameters::plotInteriorGridLines) )
                            {
                      	axis=gridCoordinatePlane(1,plane);
                      	index=gridCoordinatePlane(2,plane);
                      	side=Start;
                      	if( index >= c.gridIndexRange(End,axis) )
                      	{ // do this for CC grids and mask (see below)
                        	  side=End;
                        	  index-=c.gridIndexRange(End,axis);
                      	}
                      	if( axis<0 || axis>2 || index<c.gridIndexRange(Start,axis) || index > c.gridIndexRange(End,axis) )
                      	{
                        	  printf("ERROR: there are invalid values specifying a coordinate plane, grid=%i, axis=%i, index=%i \n",
                             		 grid,axis,index);
                        	  continue;
                      	}
                            }
                            else
                            {
        	// printf(" plane=%i, coordinatePlane(0,plane)=%i \n",plane,coordinatePlane(0,plane));
                      	continue;
                            }
              // *wdh* 050820: added ghost lines
                            getBoundaryIndex(extendedGridIndexRange(c),side,axis,I1a,I2a,I3a,psp.numberOfGhostLinesToPlot);
                            int dir;
                            for( dir=0; dir<gc.numberOfDimensions(); dir++ )
                            {
        	// *wdh* 061115 -- remove periodic fix, limit bounds by dimension
        // 	    if( c.isPeriodic(dir)==Mapping::functionPeriodic )
        // 	      Iva[dir]=Range(Iva[dir].getBase(),Iva[dir].getBound()+1);
                      	Iva[dir]=Range(max(Iva[dir].getBase(),c.dimension(0,dir)),min(Iva[dir].getBound(),c.dimension(1,dir)));
                            }
                            Iva[axis]=c.gridIndexRange(side,axis)+index;
              // We need to add an extra line in the tangential directions on parallel ghost boundaries
                            const int includeGhost=1;
                            bool ok = ParallelUtility::getLocalArrayBounds(c.mask(),mask,I1a,I2a,I3a,includeGhost);
                            if( !ok ) continue;
                            glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                            glBegin(GL_LINES);
                            int in1= (isVertexCentered && axis==axis1) ? -side : 0;  // shift for mask
                            int in2= (isVertexCentered && axis==axis2) ? -side : 0;
                            int in3= (isVertexCentered && axis==axis3) ? -side : 0;
                            for( dir=1; dir<=2; dir++ )
                            {
                      	int axisp = (axis+dir) % 3;    // plot lines parallel to axis=axisp
                      	is1= axisp==axis1 ? 1 : 0;
                      	is2= axisp==axis2 ? 1 : 0;
                      	is3= axisp==axis3 ? 1 : 0;
                      	int ib1= (axis!=axis1 && axisp!=axis1) ? in1-1 : in1; 
                      	int ib2= (axis!=axis2 && axisp!=axis2) ? in2-1 : in2; 
                      	int ib3= (axis!=axis3 && axisp!=axis3) ? in3-1 : in3; 
                                Index Ivb[3], &I1b=Ivb[0], &I2b=Ivb[1], &I3b=Iva[2];
                      	I1b=Range(I1a.getBase(),I1a.getBound()-is1); 
                      	I2b=Range(I2a.getBase(),I2a.getBound()-is2); 
                      	I3b=Range(I3a.getBase(),I3a.getBound()-is3); 
                      	if( abs(gi.gridCoarseningFactor)==1 )
                      	{
                        	  FOR_3(i1,i2,i3,I1b,I2b,I3b)
                        	  {
                          	    if( isVertexCentered )
                          	    {
                            	      if( psp.plotInterpolationCells )
                            	      {
                            		if( MASK_UIB(i1,i2,i3) || MASK_UIB(i1+is1,i2+is2,i3+is3) )
                              		  continue;  // skip this line
                            	      }
                            	      else
                            	      {
                            		if( MASK(i1,i2,i3)<=0 && MASK(i1+is1,i2+is2,i3+is3)<=0 )
                              		  continue;  // skip this line
                            	      }
                          	    }
                          	    else 
                          	    {
                            	      if( psp.plotInterpolationCells )
                            	      {
                            		if( MASK_UIB(i1+in1,i2+in2,i3+in3) && MASK_UIB(i1+ib1,i2+ib2,i3+ib3) )
                              		  continue;    // skip this line
                            	      }
                            	      else
                            	      {
                            		if( MASK(i1+in1,i2+in2,i3+in3)<=0 && MASK(i1+ib1,i2+ib2,i3+ib3)<=0 )
                              		  continue;    // skip this line
                            	      }
                          	    }
                          	    if( isHiddenByRefinement>0 )
                          	    {
                            	      if( MASK(i1,i2,i3)&isHiddenByRefinement || MASK(i1+is1,i2+is2,i3+is3)&isHiddenByRefinement )
                            		continue;  // skip this point
                          	    }
                          	    glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                  		      VERTEX1(i1    ,i2    ,i3    ),
                                  		      VERTEX2(i1    ,i2    ,i3    ));
                          	    glVertex3(VERTEX0(i1+is1,i2+is2,i3+is3),
                                  		      VERTEX1(i1+is1,i2+is2,i3+is3),
                                  		      VERTEX2(i1+is1,i2+is2,i3+is3));
                        	  }
                      	}
                      	else // coarsen factor >1 
                      	{
        	  // ** new 050921 ***
        	  // strides: (m1,m2,m3)
                        	  const int m1=axisp!=axis1 ? abs(gi.gridCoarseningFactor) : 1; 
                        	  const int m2=axisp!=axis2 ? abs(gi.gridCoarseningFactor) : 1;
                        	  const int m3=axisp!=axis3 ? abs(gi.gridCoarseningFactor) : 1;
                        	  const int ms1=m1*is1;
                        	  const int ms2=m2*is2;
                        	  const int ms3=m3*is3;
                        	  const int j3b = min(i3+m3,I3b.getBound());
                        	  const int j2b = min(i2+m2,I2b.getBound());
                        	  const int j1b = min(i1+m1,I1b.getBound());
                        	  int i1Bound,i2Bound,i3Bound;
                        	  FOR_3WithStride(i1,i2,i3,m1,m2,m3,I1b,I2b,I3b) 
                        	  {
        	    // Check that all sub-lines can be plotted, only then plot the coarser line segment
        	    // ** watch out for ends
                          	    bool ok=true && (i1+m1-1)<=i1Bound && (i2+m2-1)<=i2Bound && (i3+m3-1)<=i3Bound;
                          	    int j1,j2,j3=i3;
                          	    for( j3=i3; j3<=j3b && ok ; j3++ )
                            	      for( j2=i2; j2<=j2b && ok ; j2++ )
                            		for( j1=i1; j1<=j1b; j1++ )
                            		{
                      // 		  plotThisLineSegment(j1,j2,j3,is1,is2,is3,ok);
                                                if( psp.plotInterpolationCells )
                                                {
                                                    if( MASK_UIB(j1,j2,j3) || MASK_UIB(j1+is1,j2+is2,j3+is3) )
                                                        ok=false;  // skip this line
                                                }
                                                else
                                                {
                                                    if( MASK(j1,j2,j3)<=0 && MASK(j1+is1,j2+is2,j3+is3)<=0 )
                                                        ok=false;  // skip this line
                                                }
                                                if( isHiddenByRefinement>0 )
                                                {
                                                    if( MASK(j1,j2,j3)&isHiddenByRefinement || MASK(j1+is1,j2+is2,j3+is3)&isHiddenByRefinement )
                                                        ok=false;  // skip this line
                                                }
                              		  if( !ok )
                                		    break;
                            		}
                          	    if( ok )
                          	    {
        	      // plot longer line segment
                            	      glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                  			VERTEX1(i1    ,i2    ,i3    ),
                                  			VERTEX2(i1    ,i2    ,i3    ));
                            	      glVertex3(VERTEX0(i1+ms1,i2+ms2,i3+ms3),
                                  			VERTEX1(i1+ms1,i2+ms2,i3+ms3),
                                  			VERTEX2(i1+ms1,i2+ms2,i3+ms3));
                          	    }
                          	    else
                          	    {
        	      // plot individual line segements
                            	      const int i3b = min(i3+m3-1,i3Bound);
                            	      const int i2b = min(i2+m2-1,i2Bound);
                            	      const int i1b = min(i1+m1-1,i1Bound);
                            	      for( j3=i3; j3<=i3b; j3++ )
                            		for( j2=i2; j2<=i2b; j2++ )
                              		  for( j1=i1; j1<=i1b; j1++ )
                              		  {
                                		    ok=true;
                        // 		    plotThisLineSegment(j1,j2,j3,is1,is2,is3,ok);
                                                    if( psp.plotInterpolationCells )
                                                    {
                                                        if( MASK_UIB(j1,j2,j3) || MASK_UIB(j1+is1,j2+is2,j3+is3) )
                                                            ok=false;  // skip this line
                                                    }
                                                    else
                                                    {
                                                        if( MASK(j1,j2,j3)<=0 && MASK(j1+is1,j2+is2,j3+is3)<=0 )
                                                            ok=false;  // skip this line
                                                    }
                                                    if( isHiddenByRefinement>0 )
                                                    {
                                                        if( MASK(j1,j2,j3)&isHiddenByRefinement || MASK(j1+is1,j2+is2,j3+is3)&isHiddenByRefinement )
                                                            ok=false;  // skip this line
                                                    }
                                		    if( ok )
                                		    {
                                  		      glVertex3(VERTEX0(i1    ,i2    ,i3    ),
                                        				VERTEX1(i1    ,i2    ,i3    ),
                                        				VERTEX2(i1    ,i2    ,i3    ));
                                  		      glVertex3(VERTEX0(i1+is1,i2+is2,i3+is3),
                                        				VERTEX1(i1+is1,i2+is2,i3+is3),
                                        				VERTEX2(i1+is1,i2+is2,i3+is3));
                                		    }
                              		  }
                          	    }
                        	  }
                      	} // end coarsening factor >1 
                            } // end for dir...
                            glEnd();     // GL_LINES  ** moved
                        }
                    }   // end plot lines on boundaries 
          // ********************************************************
          // ************ plot interpolation points UNLIT ***********
          // ********************************************************
                    if( ( (gridOptions(grid) & GraphicsParameters::plotInterpolation)  ||
                                (gridOptions(grid) & GraphicsParameters::plotBackupInterpolation) )
                            && numberOfGrids > 1 && gc.getClassName()=="CompositeGrid" )
                    {
                        CompositeGrid & cg = (CompositeGrid &)gc;
            //             #If "curvilinear" == "rectangular"
            //             #Else    
            // const RealDistributedArray & center = c.center();
                          #ifndef USE_PPP
                              const realSerialArray & center = cg[grid].center(); 
                          #else
                              realSerialArray center; getLocalArrayWithGhostBoundaries(cg[grid].center(),center);
                          #endif
                        if( (plotInterpolationPoints || plotBackupInterpolationPoints ) && 
                      	(gridOptions(grid)&GraphicsParameters::plotInterpolation) &&
                      	numberOfGrids >= 1 && 
                      	gc.getClassName()=="CompositeGrid"  )
                        {
                            #ifndef USE_PPP
                              const intSerialArray & ip = cg.interpolationPoint[grid];
                              const intSerialArray & interpoleeGrid = cg.interpoleeGrid[grid];
                            #else
               // -- parallel version --
               // copy the interpolation data from processor "p" to processor "graphicsProcessor"
               // *new* way *wdh* 110628 (from 2d version)
                              intSerialArray ip,interpoleeGrid;
                              collectInterpolationData( p,graphicsProcessor, grid,cg, ip,interpoleeGrid );
                              if( p!=graphicsProcessor )
                              {
        	 // -- for vertex centered grids we can re-use the VERTEX, otherwise we need the CENTER  ** fix me **
               //                  #If "curvilinear" == "curvilinear"
                                  const bool cellVertex = (bool)cg[grid].isAllVertexCentered();
                       	 if( cellVertex )
                       	 { // node centered -- we can reuse the vertex array 
                         	   center.reference(vertex);
                       	 }
                       	 else
                       	 {
        	   // cell-centered : we need the center array which holds the coords of cell centers
                         	   CopyArray::copyArray( center,graphicsProcessor, center,p );
                       	 }
                       	 i2=center.getBase(1);
                       	 i3=center.getBase(2);
                              }
                            #endif
              //               #If "curvilinear" == "curvilinear"
                              const real *centerp = center.Array_Descriptor.Array_View_Pointer3;
                              const int centerDim0=center.getRawDataSize(0);
                              const int centerDim1=center.getRawDataSize(1);
                              const int centerDim2=center.getRawDataSize(2);
                              #define CENTER0(i0,i1,i2) XSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(0)))])
                              #define CENTER1(i0,i1,i2) YSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(1)))])
                              #define CENTER2(i0,i1,i2) ZSCALE(centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(2)))])
                            if( ip.getLength(0)>0 )
                            {
                                const int *ipp = ip.Array_Descriptor.Array_View_Pointer1;
                                const int ipDim0=ip.getRawDataSize(0);
                                #define IP(i0,i1) ipp[i0+ipDim0*(i1)]
        	// --- plot normal interpolation points ---
                      	if( plotInterpolationPoints && (gridOptions(grid) & GraphicsParameters::plotInterpolation) && 
                          	    cg.numberOfInterpolationPoints.getLength(0)>grid )
                      	{
        	  //  	  if ((errCode=glGetError()) != GL_NO_ERROR)
        	  //  	  {
        	  //  	    errString = gluErrorString(errCode);
        	  //  	    printf("grid3d: Start of plotInterpolation loop: OpenGL Error: %s\n", errString);
        	  //  	  }
                        	  gi.setColour(GenericGraphicsInterface::textColour);
                        	  glPointSize(psp.pointSize*1.67*gi.getLineWidthScaleFactor());   
                        	  glBegin(GL_POINTS);  
                        	  int oldInterpolationPointColour=-1;
        	  // for( int i=0; i<numberOfInterpolationPoints; i++ )
                        	  for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
                        	  {
                          	    if( psp.colourInterpolationPoints && interpoleeGrid(i)!=oldInterpolationPointColour )
                          	    {
        	      // colour the interpolation the same colour as the grid it interpolates from
                            	      oldInterpolationPointColour=interpoleeGrid(i);
                            	      setXColour(gi.getColourName( (oldInterpolationPointColour %
                                                  					    GenericGraphicsInterface::numberOfColourNames) ));
        	      // setXColour(gi.getColourName(min(interpoleeGrid(i),GenericGraphicsInterface::numberOfColourNames-1)));
                          	    }
                          	    glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  		      CENTER1(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  		      CENTER2(IP(i,axis1),IP(i,axis2),IP(i,axis3)));
                        	  }
                        	  glEnd();
                      	}
                      	gi.setColour(gi.getColourName(grid)); // colour by grid number
        	// if( gridOptions(grid) & GraphicsParameters::plotBackupInterpolation && 
        	// 	cg.interpolationPoint.getLength()>grid )
        	// *wdh* 110606
                      	if( plotBackupInterpolationPoints && (gridOptions(grid) & GraphicsParameters::plotInterpolation) && 
                          	    cg.numberOfInterpolationPoints.getLength(0)>grid )
                      	{
        	  // Now plot points that use back up interpolation
                        	  gi.setColour(GenericGraphicsInterface::textColour);
                        	  glPointSize(psp.pointSize*2.*gi.getLineWidthScaleFactor());   
                        	  glBegin(GL_POINTS);  
        	  // glPointSize(6.*lineWidthScaleFactor[currentWindow]);   
                        	  plotBackupInterp=true;
                        	  int numBackup=0;
        	  // for( int i=0; i<cg.numberOfInterpolationPoints(grid); i++ )
        	  // printF("grid3d: grid=%i ip=[%i,%i]\n",grid,ip.getBase(0),ip.getBound(0));
                                    for( int i=ip.getBase(0); i<=ip.getBound(0); i++ )
                        	  {
        	    // printF(" i=%i ip=(%i,%i,%i) mask=[%i,%i][%i,%i][%i,%i]\n",i,IP(i,0),IP(i,1),IP(i,2),
                    //        mask.getBase(0),mask.getBound(0), 
                    //        mask.getBase(1),mask.getBound(1), 
        	    // 	   mask.getBase(2),mask.getBound(2));
                          	    if( MASK(IP(i,axis1),IP(i,axis2),IP(i,axis3)) & CompositeGrid::USESbackupRules )
                          	    {
                            	      numBackup++;
                            	      glVertex3(CENTER0(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  			CENTER1(IP(i,axis1),IP(i,axis2),IP(i,axis3)),
                                  			CENTER2(IP(i,axis1),IP(i,axis2),IP(i,axis3)));
                          	    }
                        	  }
                        	  totalNumberOfBackupInterpolationPoints+=numBackup;
                        	  if( numBackup>0 )
                          	    printF("plotGrid3d:There were %i backup interpolation points on grid %i (%s)\n",numBackup,grid,
                               		   (const char*)gc[grid].getName());
                        	  glEnd();
                      	}
                            }  // end if( ip.getLength(0)>0 )
                        } // end plot interp or backup interp
            // ***** Plot lines joining the interpolation points *******
                        if( plotLinesJoiningInterpolationPoints ) // **** this is FALSE for now **************
                        {
                            const IntegerArray & exGridIndexRange = extendedGridIndexRange(c);
              // getIndex(exGridIndexRange,I1a,I2a,I3a);
                            FOR_3(i1,i2,i3,I1,I2,I3)
                            {
                      	for( axis=axis1; axis<gc.numberOfDimensions(); axis++ )  // draw grid lines parallel to axis
                      	{
        	  //   ...Plot faces with iv(kd) fixed
                        	  im[0]=0; im[1]=0; im[2]=0; im[axis]=1;
                        	  in[0]=1; in[1]=1; in[2]=1; in[axis]=0;
                        	  if(iv[axis1]+in[axis1]<=exGridIndexRange(End,axis1) &&
                           	     iv[axis2]+in[axis2]<=exGridIndexRange(End,axis2) &&
                           	     iv[axis3]+in[axis3]<=exGridIndexRange(End,axis3) )
                        	  {
                          	    if( CFACE(in[axis1],in[axis2],in[axis3],i1,i2,i3) ) 
                          	    {
        	      //  ...face of computational points
        	      //  plot BC boundary, branch cut or interpolation
                            	      ok=FALSE;
                            	      if(iv[axis]==exGridIndexRange(Start,axis) || 
                             		 iv[axis]==exGridIndexRange(End  ,axis) )
                            	      {
        		//  ...On a Boundary
                            		side = iv[axis]==exGridIndexRange(Start,axis) ? Start : End;
                            		if( c.boundaryCondition()(side,axis)<0 )
                            		{
        		  // ...plot a branch cut boundary
                              		  ok=plotBranchCuts;
                            		}
                            		else if( BND(in[axis1],in[axis2],in[axis3],i1,i2,i3) )
                            		{
        		  //   ...plot a BC boundary
                              		  ok=FALSE;        // plotTrueBoundaries;
                            		}
                            		else
                            		{
        		  //   ...plot an interpolation face
                              		  ok=plotInterpolationPoints;
                            		}
                            	      }
                            	      if( !ok && plotInterpolationPoints )
                            	      {
        		// ...plot an interior interpolation face
                            		ok=IFACE(im[axis1],im[axis2],im[axis3],in[axis1],in[axis2],in[axis3],i1,i2,i3);
        		// side=Start;
                            	      }
                            	      if( ok )
                            	      {  // ---plot a face that is parallel to axis
                            		int axisp1=(axis+1) % 3;
                            		int axisp2=(axis+2) % 3;
        		//  ....loop around the 4 vertices of the face, direction axis is fixed
        		// glBegin(GL_POLYGON); // draw shaded polygon
                            		glBegin(GL_LINE_STRIP);  // draw lines only
                            		for( int i=0; i<4; i++ )
                            		{
                              		  iv3[axis  ]=iv[axis];
                              		  iv3[axisp1]=iv[axisp1]+ ( ((i+1)/2) % 2 );
                              		  iv3[axisp2]=iv[axisp2]+ ( ((i  )/2) % 2);
                      //         #If "curvilinear" == "rectangular"
                      //         #Else
        		  // ::getNormal(vertex,iv3,axis,normal);
                              		  ::getNormal(vertexp,vertexDim0,vertexDim1,vertexDim2,xBase,xBound,iv3,axis,axisp1,axisp2,normal);
                              		  glNormal3v(normal);
                              		  glVertex3(VERTEX0(iv3[0],iv3[1],iv3[2]),
                                      			    VERTEX1(iv3[0],iv3[1],iv3[2]),
                                      			    VERTEX2(iv3[0],iv3[1],iv3[2]));
                            		}
                            		glEnd();  
                            	      }
                          	    }
                        	  }
                      	}
                            }
                        }
                    } // end plot interpolation points
                #undef CENTER0
                #undef CENTER1
                #undef CENTER2
                #undef VERTEX0
                #undef VERTEX1
                #undef VERTEX2
      	}
            } // end loop over processors

        } // end if structured grid...
        if( plotOnThisProcessor ) glPopName();
    } // end for grid=0,1,2,...

    if( plotOnThisProcessor )
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glEndList();  // close the unlit list
    }

    totalNumberOfBackupInterpolationPoints=ParallelUtility::getSum(totalNumberOfBackupInterpolationPoints);
    if( plotBackupInterpolationPoints && plotBackupInterp && totalNumberOfBackupInterpolationPoints==0 )
    {
        printF("There were no backup interpolation points.\n");
    }
    

// reset the face plotting flag
    psp.plotUnsFaces = plotFaces;

//    if ((errCode=glGetError()) != GL_NO_ERROR)
//    {
//      errString = gluErrorString(errCode);
//      printf("grid3d: End of routine: OpenGL Error: %s\n", errString);
//    }
}

void PlotIt:: 
surfaceGrid3d(GenericGraphicsInterface &gi, const GridCollection & gc, 
            	      GraphicsParameters & psp, 
            	      IntegerArray & boundaryConditionList, 
            	      int numberOfBoundaryConditions,
            	      IntegerArray & numberList, 
            	      int & number, int list, int lightList)
// ======================================================================================================
// /Description:
//   Plot a three dimensional grid.
// ======================================================================================================
{
//  GLenum errCode;
//  const GLubyte *errString;
    
    const int numberOfGrids = gc.numberOfComponentGrids();

    const int domainDimension = gc[0].domainDimension();
    const int rangeDimension = gc[0].rangeDimension();

    IntegerArray & gridsToPlot        = psp.gridsToPlot;
    bool & plotInterpolationPoints= psp.plotInterpolationPoints;

    bool & plotBranchCuts         = psp.plotBranchCuts;
    IntegerArray & gridOptions        = psp.gridOptions;
    IntegerArray & gridBoundaryConditionOptions = psp.gridBoundaryConditionOptions;

    bool plotLinesJoiningInterpolationPoints=FALSE;  // add to parameters
    int & numberOfGridCoordinatePlanes= psp.numberOfGridCoordinatePlanes; 
    IntegerArray & gridCoordinatePlane    = psp.gridCoordinatePlane;

    bool ok;
    
    int side,axis;
    Index I1,I2,I3;
    real normal[3];
    int im[3],in[3],iv3[3];
    int iv[3], &i1 = iv[0], &i2 = iv[1], &i3 = iv[2];
    int isv[3], &is1=isv[0], &is2=isv[1], &is3=isv[2];

// save the state of the unstructured plotting flags (so that we can turn them on/off)
    int plotNodes = psp.plotUnsNodes;
    int plotFaces = psp.plotUnsFaces;
    int plotEdges = psp.plotUnsEdges;
    int plotBoundaryEdges = psp.plotUnsBoundaryEdges;

  // for faces that are toggled on or off
    const GraphicsParameters::GridOptions 
        faceToToggle[6]={GraphicsParameters::doNotPlotFace00,GraphicsParameters::doNotPlotFace10,
                 		     GraphicsParameters::doNotPlotFace01,GraphicsParameters::doNotPlotFace11,
                 		     GraphicsParameters::doNotPlotFace02,GraphicsParameters::doNotPlotFace12}; //

// loop over all grids and plot with LIGHTS ON
    psp.plotUnsNodes = FALSE;
    psp.plotUnsEdges = FALSE;
    psp.plotUnsBoundaryEdges = FALSE;

    glNewList(lightList,GL_COMPILE);

    int grid;
    for( grid=0; grid<numberOfGrids; grid++ )
    {
        if( !(gridsToPlot(grid)&GraphicsParameters::toggleGrids) )
            continue;

        const MappedGrid & c = gc[grid];

        glPushName(c.getGlobalID()); // assign a name for picking

        if( c.getGridType()==MappedGrid::unstructuredGrid )
        {
      //   printf("plot: plotting lit part of an unstructured grid...\n");
            psp.set(GI_MAPPING_COLOUR,getGridColour(0,0,0,grid,gc,gi,psp));
            plotUnstructured(gi, (UnstructuredMapping&)(c.mapping().getMapping()), psp);
        }
        else
        {
      // == structured grid ==
            
            if( (gridOptions(grid) & GraphicsParameters::plotShadedSurfaces) )
            {
      	const IntegerArray & gridIndexRange = c.gridIndexRange();
        	  
	// *wdh* getIndex(extendedGridIndexRange(c),I1,I2,I3);
      	getIndex(c.gridIndexRange(),I1,I2,I3);
        	  
      	gi.setColour(gi.getColourName(min(grid,GenericGraphicsInterface::numberOfColourNames-1)));
	//
	// Loop over boundaries and plot as appropriate
	//
	// offset grids lines from shaded surface
      	glEnable(GL_POLYGON_OFFSET_FILL); // offset lines from surfaces so we can see them
      	glPolygonOffset(1.0, psp.surfaceOffset*OFFSET_FACTOR); 
        	  
                #ifdef USE_PPP
        	  intSerialArray mask;     getLocalArrayWithGhostBoundaries(c.mask(),mask);
        	  realSerialArray vertex;  getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
      	#else
                    const intSerialArray & mask = c.mask();
        	  const realSerialArray & vertex = c.vertex();
                #endif

      	plotShadedFace(gi, c, mask, vertex, I1,I2,I3,2,0,psp );
            }
        }
        glPopName();

    }
    glEndList();

    glNewList(list,GL_COMPILE);

  // unlit objects 

    psp.plotUnsNodes=plotNodes;
    psp.plotUnsFaces=plotFaces;
    psp.plotUnsEdges=plotEdges;
    psp.plotUnsBoundaryEdges=plotBoundaryEdges;

 // offset grids lines from shaded surface
    glEnable(GL_POLYGON_OFFSET_FILL); // offset lines from surfaces so we can see them
    glPolygonOffset(1.0,1.0*OFFSET_FACTOR); 

    for( grid=0; grid<numberOfGrids; grid++ )
    {
        if( !(gridsToPlot(grid)&GraphicsParameters::toggleGrids) )
            continue;

        const MappedGrid & c = gc[grid];

        glPushName(c.getGlobalID()); // assign a name for picking

        if( c.getGridType()==MappedGrid::unstructuredGrid )
        {
      //      printf("plot: plotting lit part of an unstructured grid...\n");
            psp.set(GI_MAPPING_COLOUR,getGridColour(0,0,0,grid,gc,gi,psp));
            plotUnstructured(gi, (UnstructuredMapping&)(c.mapping().getMapping()), psp);
        }
        else // structured
        {
            #ifndef USE_PPP
                const intSerialArray & mask = c.mask();
            #else
                intSerialArray mask; getLocalArrayWithGhostBoundaries(c.mask(),mask);
            #endif

            const IntegerArray & gridIndexRange = c.gridIndexRange();


      // -- use the vertex array if we are plotting with an adjustment for the "displacement"
            const bool plotRectangular = c.isRectangular() && !psp.adjustGridForDisplacement && !(c->computedGeometry & MappedGrid::THEvertex);
            if( plotRectangular )
            {
      // 	plotSurfaceGrid3d(rectangular); // macro
      //       #If "rectangular" == "rectangular"
                real dx[3],xab[2][3];
                c.getRectangularGridParameters( dx, xab );
                const int i0a=c.gridIndexRange(0,0);
                const int i1a=c.gridIndexRange(0,1);
                const int i2a=c.gridIndexRange(0,2);
                const real xa=xab[0][0], dx0=dx[0];
                const real ya=xab[0][1], dy0=dx[1];
                const real za=xab[0][2], dz0=dx[2];
            #define VERTEX0(i0,i1,i2) XSCALE(xa+dx0*(i0-i0a))
            #define VERTEX1(i0,i1,i2) YSCALE(ya+dy0*(i1-i1a))
            #define VERTEX2(i0,i1,i2) ZSCALE(za+dz0*(i2-i2a))
            // printf("10:POLYGON_OFFSET_FACTOR=%f\n", psp.surfaceOffset);
            // ********************************************************
            // ************ plot boundaries ***************************
            // ********************************************************
                        Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
                        for( axis=0; axis<domainDimension; axis++ )
                        {
                  	for( side=0; side<=1; side++ )
                  	{
                    	  int bcIndex = bcNumber(c.boundaryCondition(side,axis), boundaryConditionList, numberOfBoundaryConditions);
                    	  if( gridBoundaryConditionOptions(bcIndex) & 1 && !(gridOptions(grid)&faceToToggle[side+2*axis]) )
                    	  {
                      	    getBoundaryIndex(extendedGridIndexRange(c),side,axis,I1a,I2a,I3a);
                      	    for( int dir=0; dir<c.domainDimension(); dir++ )
                      	    {
                        	      if( c.isPeriodic(dir)==Mapping::functionPeriodic )
                        		Iva[dir]=Range(Iva[dir].getBase(),Iva[dir].getBound()+1);
                      	    }
                      	    Iva[axis]=c.gridIndexRange(side,axis);
                                    gi.setColour(getGridColour(0,side,axis,grid,gc,gi,psp));
                      	    glLineWidth(psp.size(GraphicsParameters::lineWidth)*3.*
                              			gi.getLineWidthScaleFactor());  // make lines wider so we can see them
                      	    if ( psp.boundaryColourOption==GraphicsParameters::colourBlack )
                      	    {
                        	      glLineWidth(psp.size(GraphicsParameters::lineWidth)*2.*
                                			  gi.getLineWidthScaleFactor());      // boundaries are twice as thick as other lines
                      	    } 
                      	    else if( psp.boundaryColourOption==GraphicsParameters::defaultColour || 
                             		     psp.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition )
                      	    {
                        	      numberList(++number)=c.boundaryCondition(side,axis);  // keep track of what is plotted
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByShare )
                      	    {
                        	      numberList(++number)=c.sharedBoundaryFlag(side,axis);  // keep track of what is plotted
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByGrid )
                      	    {
                        	      numberList(++number)=grid;
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByDomain )
                      	    {
                        	      numberList(++number)=gc.domainNumber(grid);
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByRefinementLevel )
                      	    {
                        	      numberList(++number)=gc.refinementLevelNumber(grid);
                      	    }
                      	    else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByIndex ) 
                      	    {
      //	      gi.setColour( getXColour(psp.gridColours(grid)) ); 
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByValue )
                      	    {
      //	      gi.setColour(gi.getColourName(min(max(0,psp.boundaryColourValue),
      //						GenericGraphicsInterface::numberOfColourNames-1)));
                      	    }
                      	    else if( psp.boundaryColourOption==psp.boundaryColourOption==GraphicsParameters::colourBlack )
                      	    {
      //	      gi.setColour(GenericGraphicsInterface::textColour);
                      	    }
                      	    else
                      	    {
                        	      printf("GL_GraphicsInterface::plot(3d grid):ERROR: unknown value of psp.boundaryColourOption \n");
      //	      setColour(colourNames[min(grid,numberOfColourNames-1)]);
      //	      gi.setColour("");
                      	    }
      	    // plot boundary lines, code taken from grid.C::plotGridBoundaries
                      	    is1 = axis==axis1 ? 0 : 1;
                      	    is2 = axis==axis2 ? 0 : 1;
                      	    glBegin(GL_LINES);
                      	    getBoundaryIndex(gc[grid].gridIndexRange(),side,axis,I1,I2,I3);
                      	    I1=Range(I1.getBase(),I1.getBound()-is1);
                      	    I2=Range(I2.getBase(),I2.getBound()-is2);
                      	    if( int(gc[grid].isAllVertexCentered()) )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( MASK_DINIB(i1,i2,i3) &&  MASK_DINIB(i1+is1,i2+is2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    else 
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( MASK_DNIB(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    glEnd();
                    	  }
                  	}
                  	glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                        }
            // 
            // plot grid lines
            // 
                        if( psp.plotGridLines % 2  )
                  	{
                    	  glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                    	  gi.setColour(getGridColour(1,0,0,grid,gc,gi,psp));
                    	  bool cellVertex = (bool)c.isAllVertexCentered();
                    	  bool cellCentre = (bool)c.isAllCellCentered();
                    	  getIndex(c.gridIndexRange(),I1,I2,I3);
                    	  for ( int axis=0; axis<c.domainDimension(); axis++ )
                    	  {
                      	    is1= axis==axis1 ? 1 : 0;
                      	    is2= axis==axis2 ? 1 : 0;
      	    //	  I1=Range(I1.getBase()+is1,I1.getBound()-is1); 
      	    //I2=Range(I2.getBase()+is2,I2.getBound()-is2); 
                      	    if( cellVertex )
                      	    {
                        	      I1=Range(max(I1.getBase(),c.dimension()(Start,0)),min(I1.getBound()-is1,c.dimension()(End,0)-is1));
                        	      I2=Range(max(I2.getBase(),c.dimension()(Start,1)),min(I2.getBound()-is2,c.dimension()(End,1)-is2));
                      	    }
                      	    else
                      	    {
                        	      I1=Range(max(I1.getBase(),c.dimension()(Start,0)),min(I1.getBound()-is1,c.dimension()(End,0)-is1));
                        	      I2=Range(max(I2.getBase(),c.dimension()(Start,1)),min(I2.getBound()-is2,c.dimension()(End,1)-is2));
      	      // include edge of extended boundaries:
                        	      if( c.boundaryCondition()(End  ,axis1)==0 )
                        		I1=Range(I1.getBase(),I1.getBound()+1);
                        	      if( c.boundaryCondition()(End  ,axis2)==0 )
                        		I2=Range(I2.getBase(),I2.getBound()+1);
                      	    }
                      	    glBegin(GL_LINES);
                      	    if(cellVertex )
                      	    {
                        	      intSerialArray cMask=mask; 
                        	      cMask(I1,I2,I3)=psp.plotInterpolationCells ? mask(I1,I2,I3)!=0 && mask(I1+is1,I2+is2,I3)!=0 
                        		: mask(I1,I2,I3)>0  && mask(I1+is1,I2+is2,I3)>0 ;
                        	      if( gc.numberOfRefinementLevels()>1 )
                        		cMask(I1,I2,I3) = cMask(I1,I2,I3) && !( mask(I1,I2,I3)&isHiddenByRefinement || 
                                                      							mask(I1+is1,I2+is2,I3)&isHiddenByRefinement);
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( cMask(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    else
                      	    {
      	      // cell centered
                        	      i3=I3.getBase();
                        	      int i2m = max(I2.getBase()-is1,c.dimension(Start,1));
                        	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
                        	      {
                        		int i1m = max(I1.getBase()-is2,c.dimension(Start,0));
                        		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
                        		{
                          		  bool plotLine = psp.plotInterpolationCells ?
                            		    MASK_CNR(i1,i2,i3) || MASK_CNR(i1m,i2m,i3) :
                            		    MASK_DNR(i1,i2,i3) && MASK_DNR(i1m,i2m,i3);
                          		  if( gc.numberOfRefinementLevels()>1 )
                            		    plotLine = plotLine && ! (mask(i1    ,i2    ,i3)&isHiddenByRefinement);
                          		  if( plotLine )
                          		  {
                            		    glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                            		    glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                          		  }
                          		  i1m=i1+1-is2;
                        		}
                        		i2m=i2+1-is1;
                        	      }
                      	    }
                      	    glEnd();
                      	    }
                  	}
            #undef VERTEX0
            #undef VERTEX1
            #undef VERTEX2
            }
            else
            {
      // 	plotSurfaceGrid3d(curvilinear); // macro
      //       #If "curvilinear" == "rectangular"
      //       #Else    
                #ifndef USE_PPP
                    const realSerialArray & vertex = c.vertex();
                #else
                    realSerialArray vertex; getLocalArrayWithGhostBoundaries(c.vertex(),vertex);
                #endif
                real *vertexp = vertex.Array_Descriptor.Array_View_Pointer3;
                const int vertexDim0 = vertex.getRawDataSize(0);
                const int vertexDim1 = vertex.getRawDataSize(1);
                const int vertexDim2 = vertex.getRawDataSize(2);
            #define VERTEX0(i0,i1,i2) XSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((0)))))])
            #define VERTEX1(i0,i1,i2) YSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((1)))))])
            #define VERTEX2(i0,i1,i2) ZSCALE(vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((2)))))])
      // define VERTEX(i0,i1,i2,i3) vertexp[((i0)+vertexDim0*((i1)+vertexDim1*((i2)+vertexDim2*((i3)))))]
            // printf("10:POLYGON_OFFSET_FACTOR=%f\n", psp.surfaceOffset);
            // ********************************************************
            // ************ plot boundaries ***************************
            // ********************************************************
                        Index Iva[3], &I1a=Iva[0], &I2a=Iva[1], &I3a=Iva[2];
                        for( axis=0; axis<domainDimension; axis++ )
                        {
                  	for( side=0; side<=1; side++ )
                  	{
                    	  int bcIndex = bcNumber(c.boundaryCondition(side,axis), boundaryConditionList, numberOfBoundaryConditions);
                    	  if( gridBoundaryConditionOptions(bcIndex) & 1 && !(gridOptions(grid)&faceToToggle[side+2*axis]) )
                    	  {
                      	    getBoundaryIndex(extendedGridIndexRange(c),side,axis,I1a,I2a,I3a);
                      	    for( int dir=0; dir<c.domainDimension(); dir++ )
                      	    {
                        	      if( c.isPeriodic(dir)==Mapping::functionPeriodic )
                        		Iva[dir]=Range(Iva[dir].getBase(),Iva[dir].getBound()+1);
                      	    }
                      	    Iva[axis]=c.gridIndexRange(side,axis);
                                    gi.setColour(getGridColour(0,side,axis,grid,gc,gi,psp));
                      	    glLineWidth(psp.size(GraphicsParameters::lineWidth)*3.*
                              			gi.getLineWidthScaleFactor());  // make lines wider so we can see them
                      	    if ( psp.boundaryColourOption==GraphicsParameters::colourBlack )
                      	    {
                        	      glLineWidth(psp.size(GraphicsParameters::lineWidth)*2.*
                                			  gi.getLineWidthScaleFactor());      // boundaries are twice as thick as other lines
                      	    } 
                      	    else if( psp.boundaryColourOption==GraphicsParameters::defaultColour || 
                             		     psp.boundaryColourOption==GraphicsParameters::colourByBoundaryCondition )
                      	    {
                        	      numberList(++number)=c.boundaryCondition(side,axis);  // keep track of what is plotted
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByShare )
                      	    {
                        	      numberList(++number)=c.sharedBoundaryFlag(side,axis);  // keep track of what is plotted
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByGrid )
                      	    {
                        	      numberList(++number)=grid;
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByDomain )
                      	    {
                        	      numberList(++number)=gc.domainNumber(grid);
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByRefinementLevel )
                      	    {
                        	      numberList(++number)=gc.refinementLevelNumber(grid);
                      	    }
                      	    else if( psp.blockBoundaryColourOption==GraphicsParameters::colourByIndex ) 
                      	    {
      //	      gi.setColour( getXColour(psp.gridColours(grid)) ); 
                      	    }
                      	    else if( psp.boundaryColourOption==GraphicsParameters::colourByValue )
                      	    {
      //	      gi.setColour(gi.getColourName(min(max(0,psp.boundaryColourValue),
      //						GenericGraphicsInterface::numberOfColourNames-1)));
                      	    }
                      	    else if( psp.boundaryColourOption==psp.boundaryColourOption==GraphicsParameters::colourBlack )
                      	    {
      //	      gi.setColour(GenericGraphicsInterface::textColour);
                      	    }
                      	    else
                      	    {
                        	      printf("GL_GraphicsInterface::plot(3d grid):ERROR: unknown value of psp.boundaryColourOption \n");
      //	      setColour(colourNames[min(grid,numberOfColourNames-1)]);
      //	      gi.setColour("");
                      	    }
      	    // plot boundary lines, code taken from grid.C::plotGridBoundaries
                      	    is1 = axis==axis1 ? 0 : 1;
                      	    is2 = axis==axis2 ? 0 : 1;
                      	    glBegin(GL_LINES);
                      	    getBoundaryIndex(gc[grid].gridIndexRange(),side,axis,I1,I2,I3);
                      	    I1=Range(I1.getBase(),I1.getBound()-is1);
                      	    I2=Range(I2.getBase(),I2.getBound()-is2);
                      	    if( int(gc[grid].isAllVertexCentered()) )
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( MASK_DINIB(i1,i2,i3) &&  MASK_DINIB(i1+is1,i2+is2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    else 
                      	    {
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( MASK_DNIB(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    glEnd();
                    	  }
                  	}
                  	glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                        }
            // 
            // plot grid lines
            // 
                        if( psp.plotGridLines % 2  )
                  	{
                    	  glLineWidth(psp.size(GraphicsParameters::lineWidth)*gi.getLineWidthScaleFactor());
                    	  gi.setColour(getGridColour(1,0,0,grid,gc,gi,psp));
                    	  bool cellVertex = (bool)c.isAllVertexCentered();
                    	  bool cellCentre = (bool)c.isAllCellCentered();
                    	  getIndex(c.gridIndexRange(),I1,I2,I3);
                    	  for ( int axis=0; axis<c.domainDimension(); axis++ )
                    	  {
                      	    is1= axis==axis1 ? 1 : 0;
                      	    is2= axis==axis2 ? 1 : 0;
      	    //	  I1=Range(I1.getBase()+is1,I1.getBound()-is1); 
      	    //I2=Range(I2.getBase()+is2,I2.getBound()-is2); 
                      	    if( cellVertex )
                      	    {
                        	      I1=Range(max(I1.getBase(),c.dimension()(Start,0)),min(I1.getBound()-is1,c.dimension()(End,0)-is1));
                        	      I2=Range(max(I2.getBase(),c.dimension()(Start,1)),min(I2.getBound()-is2,c.dimension()(End,1)-is2));
                      	    }
                      	    else
                      	    {
                        	      I1=Range(max(I1.getBase(),c.dimension()(Start,0)),min(I1.getBound()-is1,c.dimension()(End,0)-is1));
                        	      I2=Range(max(I2.getBase(),c.dimension()(Start,1)),min(I2.getBound()-is2,c.dimension()(End,1)-is2));
      	      // include edge of extended boundaries:
                        	      if( c.boundaryCondition()(End  ,axis1)==0 )
                        		I1=Range(I1.getBase(),I1.getBound()+1);
                        	      if( c.boundaryCondition()(End  ,axis2)==0 )
                        		I2=Range(I2.getBase(),I2.getBound()+1);
                      	    }
                      	    glBegin(GL_LINES);
                      	    if(cellVertex )
                      	    {
                        	      intSerialArray cMask=mask; 
                        	      cMask(I1,I2,I3)=psp.plotInterpolationCells ? mask(I1,I2,I3)!=0 && mask(I1+is1,I2+is2,I3)!=0 
                        		: mask(I1,I2,I3)>0  && mask(I1+is1,I2+is2,I3)>0 ;
                        	      if( gc.numberOfRefinementLevels()>1 )
                        		cMask(I1,I2,I3) = cMask(I1,I2,I3) && !( mask(I1,I2,I3)&isHiddenByRefinement || 
                                                      							mask(I1+is1,I2+is2,I3)&isHiddenByRefinement);
                        	      FOR_3(i1,i2,i3,I1,I2,I3)
                        	      {
                        		if( cMask(i1,i2,i3) )
                        		{
                          		  glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                          		  glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                        		}
                        	      }
                      	    }
                      	    else
                      	    {
      	      // cell centered
                        	      i3=I3.getBase();
                        	      int i2m = max(I2.getBase()-is1,c.dimension(Start,1));
                        	      for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
                        	      {
                        		int i1m = max(I1.getBase()-is2,c.dimension(Start,0));
                        		for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
                        		{
                          		  bool plotLine = psp.plotInterpolationCells ?
                            		    MASK_CNR(i1,i2,i3) || MASK_CNR(i1m,i2m,i3) :
                            		    MASK_DNR(i1,i2,i3) && MASK_DNR(i1m,i2m,i3);
                          		  if( gc.numberOfRefinementLevels()>1 )
                            		    plotLine = plotLine && ! (mask(i1    ,i2    ,i3)&isHiddenByRefinement);
                          		  if( plotLine )
                          		  {
                            		    glVertex3( VERTEX0(i1    ,i2    ,i3),VERTEX1(i1    ,i2    ,i3),VERTEX2(i1    ,i2    ,i3));
                            		    glVertex3( VERTEX0(i1+is1,i2+is2,i3),VERTEX1(i1+is1,i2+is2,i3),VERTEX2(i1+is1,i2+is2,i3));
                          		  }
                          		  i1m=i1+1-is2;
                        		}
                        		i2m=i2+1-is1;
                        	      }
                      	    }
                      	    glEnd();
                      	    }
                  	}
            #undef VERTEX0
            #undef VERTEX1
            #undef VERTEX2
            }
        }  // end structured
        glPopName();
    }

    glDisable(GL_POLYGON_OFFSET_FILL);
            
    glEndList();  // close the list with lighting


}

#undef VERTEX
#undef FOR_3
#undef MASK
#undef IP      
#undef CENTER
