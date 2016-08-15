// This file automatically generated from improveQuality.bC with bpp.
#include "Ogen.h"
#include "Overture.h"
#include "display.h"
#include "conversion.h"
#include "ParallelUtility.h"

static const int ISneededPoint = CompositeGrid::ISreservedBit2;  // from Cgsh.h
// Define a macro to index an A++ array with 3 dimensions *NOTE* a legal macro  --> #define MASK
// #define DEF_ARRAY_MACRO_3D(int,mask,MASK) //   int * mask ## p = mask.Array_Descriptor.Array_View_Pointer2;//   const int mask ## Dim0=mask.getRawDataSize(0);//   const int mask ## Dim1=mask.getRawDataSize(1);// #define MASK(i0,i1,i2) mask ## p[i0+mask ## Dim0*(i1+mask ## Dim1*(i2))]

// Macro to extract a local array with ghost boundaries
//  type = int/float/double/real
//  xd = distributed array
//  xs = serial array 
#ifdef USE_PPP
  #define GET_LOCAL(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
  #define GET_LOCAL_CONST(type,xd,xs)type ## SerialArray xs; getLocalArrayWithGhostBoundaries(xd,xs)
#else
  #define GET_LOCAL(type,xd,xs)type ## SerialArray & xs = xd
  #define GET_LOCAL_CONST(type,xd,xs)const type ## SerialArray & xs = xd
#endif

int Ogen::
determineMinimalIndexRange( CompositeGrid & cg )
// =============================================================================================================
// /Description:
//    Given a valid CompositeGrid, determine the actual range of useful points for each grid.
//
//  For now this is purely informational
// 
// =============================================================================================================
{
    if( debug & 1 )
    {
        const int numberOfBaseGrids = cg.numberOfBaseGrids();
    
        for( int grid=0; grid<numberOfBaseGrids; grid++ )
        {
            const intArray & maskd = cg[grid].mask();
            const IntegerArray & dimension = cg[grid].dimension();

            GET_LOCAL_CONST(int,maskd,mask);
            
            intSerialArray ia;
            if( mask.getLength(0)>0 )
                ia=(mask!=0).indexMap();
            Range R=ia.dimension(0);

      // displayMask(mask,"mask");

            IntegerArray indexRange(2,3);
            indexRange=0;
            if( R.getLength()>0 )
            {
      	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      	{
        	  indexRange(Start,axis)=min(ia(R,axis));
        	  indexRange(End  ,axis)=max(ia(R,axis));
      	}
      	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      	{
        	  indexRange(Start,axis)=ParallelUtility::getMinValue(indexRange(Start,axis));
        	  indexRange(End  ,axis)=ParallelUtility::getMaxValue(indexRange(End  ,axis));
      	}
            }
            printF("Minimal index range is [%2i,%5i]x[%2i,%5i]x[%2i,%5i], dimension=[%2i,%5i]x[%2i,%5i]x[%2i,%5i] for grid %s \n",
           	     indexRange(0,0),indexRange(1,0), indexRange(0,1),indexRange(1,1), indexRange(0,2),indexRange(1,2),
           	     dimension(0,0),dimension(1,0), dimension(0,1),dimension(1,1), dimension(0,2),dimension(1,2),
           	     (const char*)cg[grid].mapping().getName(Mapping::mappingName));
        }
    }
    
    return 0;
}




int 
displayMaskNeeded( const intArray & mask, 
                          const aString & label /* =nullString */ )
// =======================================================================================
// /Description:
// Display the mask array in a MappedGrid in a reasonable way
// The mask array in a MappedGrid is a bit-mapping that is difficult to look at
// if displayed in the formal way. This routine will display the mask in a more
// compact form (although some information is not printed) where each entry printed will mean:
// \begin{description}
//   \item[1] : ISdiscretizationPoint
//   \item[2] : ISghostPoint
//   \item[-1] : ISinterpolationPoint
// \end{description}   
//\end{displayInclude.tex}
// =======================================================================================
{
    
    intArray m;
    m.redim(mask);
    m=0;
    where( mask & MappedGrid::ISdiscretizationPoint )
        m=1;
    where( mask<0 && mask>-100 )
        m=mask;
    elsewhere( mask<0 )
        m=-1;
    elsewhere( mask & MappedGrid::ISghostPoint )
        m=2;

    where( mask & ISneededPoint )
    {
        m*=3;
    }

    display(m,label,"%3i");
    return 0;
}

static inline real
square( const real & x )
{
    return x*x;
}


// =====================================================================================
// Macro: getCellSize
// Parameters:
//   DIM : number of space dimensions, 2 or 3
//   GRIDTYPE: rectangular or curvilinear
//   i1,i2,i3 : lower corner of cell 
// =====================================================================================
// ================== END Macro getCellSize ======================================



// =========================================================================================
/// \brief compute the quaility of the interpolation
///  qForward : cell size on target grid 
///  qReverse :  cell size on donor grid 
// =========================================================================================
real Ogen::
computeInterpolationQuality(CompositeGrid & cg, const int & grid, 
                                                        const int & i1, const int & i2, const int & i3,
                      			    real & qForward, real & qReverse, 
                                                        const int qualityAlgorithm  )
{
    real quality=0.;

    const int numberOfBaseGrids = cg.numberOfBaseGrids();
    const int numberOfDimensions = cg.numberOfDimensions();

    MappedGrid & c = cg[grid];
    const IntegerArray & gid = c.gridIndexRange();
    const IntegerArray & bc = c.boundaryCondition();
    
    intArray & inverseGrid = cg.inverseGrid[grid];

    const int grid2=inverseGrid(i1,i2,i3);
    assert( grid2>=0 && grid2<cg.numberOfComponentGrids() && grid2!=grid );


    MappedGrid & g2 = cg[grid2];
    const RealArray & dr2 = g2.gridSpacing();
    const IntegerArray & gid2 = g2.gridIndexRange();
    const IntegerArray & bc2 = g2.boundaryCondition();

  // -- To save space we do not create the array of grid vertices on rectangular grids --
    real dvx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv0[3]={0,0,0}; //
    const bool isRectangular = c.isRectangular();
    if( isRectangular )
    {
        c.getRectangularGridParameters( dvx, xab );
        for( int dir=0; dir<numberOfDimensions; dir++ )
        {
            iv0[dir]=c.gridIndexRange(0,dir);
            if( c.isAllCellCentered() )
      	xab[0][dir]+=.5*dvx[dir];  // offset for cell centered
        }
    }
    real dvx2[3]={1.,1.,1.}, xab2[2][3]={{0.,0.,0.},{0.,0.,0.}};
    int iv02[3]={0,0,0}; //
    const bool isRectangular2 = g2.isRectangular();
    if( isRectangular2 )
    {
        g2.getRectangularGridParameters( dvx2, xab2 );
        for( int dir=0; dir<numberOfDimensions; dir++ )
        {
            iv02[dir]=g2.gridIndexRange(0,dir);
            if( g2.isAllCellCentered() )
      	xab2[0][dir]+=.5*dvx2[dir];  // offset for cell centered
        }
    }
  // This macro defines the grid points for rectangular grids:
#define XCV(iv,axis) (xab[0][axis]+dvx[axis]*(iv[axis]-iv0[axis]))
#define XC(i) (xab[0][0]+dvx[0]*(i-iv0[0]))
#define YC(i) (xab[0][1]+dvx[1]*(i-iv0[1]))
#define ZC(i) (xab[0][2]+dvx[2]*(i-iv0[2]))
#define DXR(m,n) xra[n][m]

#undef XCV2
#define XCV2(iv,axis) (xab2[0][axis]+dvx2[axis]*(iv[axis]-iv02[axis]))
#define XC2(i) (xab2[0][0]+dvx2[0]*(i-iv02[0]))
#define YC2(i) (xab2[0][1]+dvx2[1]*(i-iv02[1]))
#define ZC2(i) (xab2[0][2]+dvx2[2]*(i-iv02[2]))


// old way: 
#define XR(m,n) xra[n][m]
#define XR2(m,n) xra2[n][m]

    real xra[3][3];

    const bool useSizeQuality=true;

    const int iv[3]={i1,i2,i3}; // 
    int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];

    if( qualityAlgorithm==0 || qualityAlgorithm==1 )
    {
    // *** new algorithm **** July 20, 2016
        realArray & rI = cg.inverseCoordinates[grid];
        const realArray & vertex  = c.vertex();
        const realArray & vertex2 = g2.vertex();
        const real offset = c.isAllCellCentered() ? 1. : .5;

    // base the quality on the relative sizes of the cells
        real cellSize, cellSize2;
        for( int axis=0; axis<numberOfDimensions; axis++ )
        {
            jv[axis]=int( rI(i1,i2,i3,axis)/dr2(axis)+g2.indexRange(0,axis)+offset ); // closest point
            jv[axis]=max(g2.dimension(0,axis),min(g2.dimension(1,axis)-1,jv[axis]));
        }
        if( numberOfDimensions==2 )
        {
            jv[2]=g2.dimension(0,axis3);
            if( isRectangular )
            {
        // getCellSize(2,rectangular,i1,i2,i3,XC,YC,ZC,vertex,cellSize);
        //          #If "2" eq "2" 
          // -------  CELL SIZE - TWO-DIMENSIONS -------------
        //           #If "rectangular" eq "rectangular"
                        cellSize = (XC(i1+1)-XC(i1))*(YC(i2+1)-YC(i2));
            }
            else
            {
        // getCellSize(2,curvilinear,i1,i2,i3,XC,YC,ZC,vertex,cellSize);
        //          #If "2" eq "2" 
          // -------  CELL SIZE - TWO-DIMENSIONS -------------
        //           #If "curvilinear" eq "rectangular"
        //           #Elif "curvilinear" eq "curvilinear"
                      for(int axis=0; axis<numberOfDimensions; axis++ ) 
                      {
                          xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
                          xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));
                      }
                      cellSize=fabs(DXR(0,0)*DXR(1,1)-DXR(1,0)*DXR(0,1));
            }
            if( isRectangular2 )
            {
        // getCellSize(2,rectangular,j1,j2,j3,XC2,YC2,ZC2,vertex2,cellSize2);
        //          #If "2" eq "2" 
          // -------  CELL SIZE - TWO-DIMENSIONS -------------
        //           #If "rectangular" eq "rectangular"
                        cellSize2 = (XC2(j1+1)-XC2(j1))*(YC2(j2+1)-YC2(j2));
            }
            else
            {
        // getCellSize(2,curvilinear,j1,j2,j3,XC2,YC2,ZC2,vertex2,cellSize2);
        //          #If "2" eq "2" 
          // -------  CELL SIZE - TWO-DIMENSIONS -------------
        //           #If "curvilinear" eq "rectangular"
        //           #Elif "curvilinear" eq "curvilinear"
                      for(int axis=0; axis<numberOfDimensions; axis++ ) 
                      {
                          xra[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
                          xra[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
                      }
                      cellSize2=fabs(DXR(0,0)*DXR(1,1)-DXR(1,0)*DXR(0,1));
            }
            
      // real xra[2][2], xra2[2][2];
      // for(int axis=0; axis<numberOfDimensions; axis++ ) 
      // {
      // 	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
      // 	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));

      // 	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
      // 	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
      // }
            
      // cellSize=fabs(XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1));
      // cellSize2=fabs(XR2(0,0)*XR2(1,1)-XR2(1,0)*XR2(0,1));
        }
        else if( numberOfDimensions==3 )
        {
            if( isRectangular )
            {
        // getCellSize(3,rectangular,i1,i2,i3,XC,YC,ZC,vertex,cellSize);
        //          #If "3" eq "2" 
        //          #Elif "3" eq "3"
          // -------  CELL-SIZE - THREE-DIMENSIONS -------------
        //           #If "rectangular" eq "rectangular"
                      cellSize = (XC(i1+1)-XC(i1))*(YC(i2+1)-YC(i2))*(ZC(i3+1)-ZC(i3));
            }
            else
            {
        // getCellSize(3,curvilinear,i1,i2,i3,XC,YC,ZC,vertex,cellSize);
        //          #If "3" eq "2" 
        //          #Elif "3" eq "3"
          // -------  CELL-SIZE - THREE-DIMENSIONS -------------
        //           #If "curvilinear" eq "rectangular"
        //           #Elif "curvilinear" eq "curvilinear"
                      for(int axis=0; axis<numberOfDimensions; axis++ ) 
                      {
                          xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
                          xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));
                          xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis));
                      }
                      cellSize=fabs(DXR(0,0)*(DXR(1,1)*DXR(2,2)-DXR(1,2)*DXR(2,1))  +
                                                  DXR(1,0)*(DXR(2,1)*DXR(0,2)-DXR(2,2)*DXR(0,1))  +
                                                  DXR(2,0)*(DXR(0,1)*DXR(1,2)-DXR(0,2)*DXR(1,1)) );
            }
            if( isRectangular2 )
            {
        // getCellSize(3,rectangular,j1,j2,j3,XC2,YC2,ZC2,vertex2,cellSize2);
        //          #If "3" eq "2" 
        //          #Elif "3" eq "3"
          // -------  CELL-SIZE - THREE-DIMENSIONS -------------
        //           #If "rectangular" eq "rectangular"
                      cellSize2 = (XC2(j1+1)-XC2(j1))*(YC2(j2+1)-YC2(j2))*(ZC2(j3+1)-ZC2(j3));
            }
            else
            {
        // getCellSize(3,curvilinear,j1,j2,j3,XC2,YC2,ZC2,vertex2,cellSize2);
        //          #If "3" eq "2" 
        //          #Elif "3" eq "3"
          // -------  CELL-SIZE - THREE-DIMENSIONS -------------
        //           #If "curvilinear" eq "rectangular"
        //           #Elif "curvilinear" eq "curvilinear"
                      for(int axis=0; axis<numberOfDimensions; axis++ ) 
                      {
                          xra[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
                          xra[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
                          xra[2][axis]=(vertex2(j1,j2,j3+1,axis)-vertex2(j1,j2,j3,axis));
                      }
                      cellSize2=fabs(DXR(0,0)*(DXR(1,1)*DXR(2,2)-DXR(1,2)*DXR(2,1))  +
                                                  DXR(1,0)*(DXR(2,1)*DXR(0,2)-DXR(2,2)*DXR(0,1))  +
                                                  DXR(2,0)*(DXR(0,1)*DXR(1,2)-DXR(0,2)*DXR(1,1)) );
            }
      // real xra[3][3], xra2[3][3];
      // for(int axis=0; axis<numberOfDimensions; axis++ ) 
      // {
      // 	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
      // 	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));
      // 	xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis));
      // 	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
      // 	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
      // 	xra2[2][axis]=(vertex2(j1,j2,j3+1,axis)-vertex2(j1,j2,j3,axis));
      // }
      // cellSize=fabs(XR(0,0)*(XR(1,1)*XR(2,2)-XR(1,2)*XR(2,1))  +
      //               XR(1,0)*(XR(2,1)*XR(0,2)-XR(2,2)*XR(0,1))  +
      //               XR(2,0)*(XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1)) );
      // cellSize2=fabs(XR2(0,0)*(XR2(1,1)*XR2(2,2)-XR2(1,2)*XR2(2,1))  +
      //                XR2(1,0)*(XR2(2,1)*XR2(0,2)-XR2(2,2)*XR2(0,1))  +
      //                XR2(2,0)*(XR2(0,1)*XR2(1,2)-XR2(0,2)*XR2(1,1)) );
        }
        else
        {
            OV_ABORT("ERROR - not implemented in 1D");
        }
        
        qForward=cellSize2;
        qReverse=cellSize;
        
        quality=cellSize/max(cellSize2,REAL_MIN);

    // --------------------------------------------------------------
    // -- check distance to nearest (non-shared) physical boundary --
    // --------------------------------------------------------------

        if( FALSE && qualityAlgorithm==1 ) // -- distance is not used yet 
        {
            int sidec[3], sidec2[3]; // closest side in each direction
            int distc[3], distc2[3]; // distance (in grid points) to closest side in each direction
            int distMin=9999, distMin2=distMin;
            int axisMin=0, axisMin2=0;
            for( int axis=0; axis<numberOfDimensions; axis++ )
            {
                distc[axis]=9999;
      	if( bc(0,axis)>0 )
      	{
        	  sidec[axis]=0; 
        	  distc[axis]=iv[axis]-gid(0,axis);
      	}
      	if( bc(1,axis)>0 && 
                        (gid(1,axis)-iv[axis]) < distc[axis] )
      	{
        	  sidec[axis]=1;
        	  distc[axis]=gid(1,axis)-iv[axis];
      	}
                distc2[axis]=9999;
      	if( bc2(0,axis)>0 )
      	{
        	  sidec2[axis]=0; 
        	  distc2[axis]=jv[axis]-gid2(0,axis);
      	}
      	else if( bc2(1,axis)>0 && (gid2(1,axis)-jv[axis]) < distc2[axis]  )
      	{
        	  sidec2[axis]=1;
        	  distc2[axis]=gid2(1,axis)-jv[axis];
      	}
	// if( share(sidec[axis],axis) != share(sidec[axis],axis) )

      	if( distc[axis] < distMin )
      	{
        	  distMin=distc[axis]; axisMin=axis;
      	}
      	if( distc2[axis] < distMin2 )
      	{
        	  distMin2=distc2[axis]; axisMin2=axis;
      	}
            } // end for axis 
            
            int minGridLines=5; // **FIX ME ***
            if( distMin2 < minGridLines || distMin < minGridLines )
            {
      	printF("--quality-- target: grid=%i iv=[%i,%i] [axisMin,distMin]=[%i,%i] gid=[%i,%i][%i,%i]\n"
                              "             donor: grid2=%i jv=[%i,%i][axisMin2,distMin2]=[%i,%i] gid2=[%i,%i][%i,%i]\n",
             	       grid,i1,i2,axisMin,distMin, gid(0,0),gid(1,0), gid(0,1),gid(1,1),
                              grid2,j1,j2,axisMin2,distMin2, gid2(0,0),gid2(1,0), gid2(0,1),gid2(1,1));
            }
        }
        

    }
    else if( qualityAlgorithm<0 || qualityAlgorithm )
    {
        printF("computeInterpolationQuality:ERROR: unknown qualityAlgorithm=%i\n", qualityAlgorithm);
        OV_ABORT("ERROR");

    }
    else if( useSizeQuality )
    {
    // -------------------------------------------------------
    // --- compare the area of the target and donor cells ----
    // -------------------------------------------------------

        OV_ABORT("finish me");


        realArray & rI = cg.inverseCoordinates[grid];
        const realArray & vertex  = c.vertex();
        const realArray & vertex2 = g2.vertex();
        const real offset = c.isAllCellCentered() ? 1. : .5;

    // base the quality on the relative sizes of the cells
        real cellSize, cellSize2;
        int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];
        for( int axis=0; axis<numberOfDimensions; axis++ )
        {
            jv[axis]=int( rI(i1,i2,i3,axis)/dr2(axis)+g2.indexRange(0,axis)+offset ); // closest point
            jv[axis]=max(g2.dimension(0,axis),min(g2.dimension(1,axis)-1,jv[axis]));
        }
        if( numberOfDimensions==2 )
        {
            jv[2]=g2.dimension(0,axis3);
            real xra[2][2], xra2[2][2];
            for(int axis=0; axis<numberOfDimensions; axis++ ) 
            {
      	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
      	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));
      	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
      	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
            }
            cellSize=fabs(XR(0,0)*XR(1,1)-XR(1,0)*XR(0,1));
            cellSize2=fabs(XR2(0,0)*XR2(1,1)-XR2(1,0)*XR2(0,1));
        }
        else if( numberOfDimensions==3 )
        {
            real xra[3][3], xra2[3][3];
            for(int axis=0; axis<numberOfDimensions; axis++ ) 
            {
      	xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis));
      	xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis));
      	xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis));
      	xra2[0][axis]=(vertex2(j1+1,j2,j3,axis)-vertex2(j1,j2,j3,axis));
      	xra2[1][axis]=(vertex2(j1,j2+1,j3,axis)-vertex2(j1,j2,j3,axis));
      	xra2[2][axis]=(vertex2(j1,j2,j3+1,axis)-vertex2(j1,j2,j3,axis));
            }
            cellSize=fabs(XR(0,0)*(XR(1,1)*XR(2,2)-XR(1,2)*XR(2,1))  +
                                        XR(1,0)*(XR(2,1)*XR(0,2)-XR(2,2)*XR(0,1))  +
                                        XR(2,0)*(XR(0,1)*XR(1,2)-XR(0,2)*XR(1,1)) );
            cellSize2=fabs(XR2(0,0)*(XR2(1,1)*XR2(2,2)-XR2(1,2)*XR2(2,1))  +
                                          XR2(1,0)*(XR2(2,1)*XR2(0,2)-XR2(2,2)*XR2(0,1))  +
                                          XR2(2,0)*(XR2(0,1)*XR2(1,2)-XR2(0,2)*XR2(1,1)) );
        }
        else
        { // 1D
            jv[2]=g2.dimension(0,axis3);
            jv[2]=g2.dimension(0,axis3);
            cellSize=fabs((vertex(i1+1,i2,i3,axis1)-vertex(i1,i2,i3,axis1)));
            cellSize2=fabs((vertex2(j1+1,j2,j3,axis1)-vertex2(j1,j2,j3,axis1)));
        }
        
        qForward=cellSize2;
        qReverse=cellSize;
        
        quality=cellSize/max(cellSize2,REAL_MIN);
    }
    else
    {
        Mapping & map2 = g2.mapping().getMapping();
        realArray & vertex = c.vertex();

        realArray xx(1,3),rr(1,3),rx(1,3,3);
        rr=-1.;
        
        for( int axis=0; axis<numberOfDimensions; axis++ )
            xx(0,axis)=vertex(i1,i2,i3,axis);
            
        map2.inverseMap(xx,rr,rx);   // could do better here.

        real q=0., q2;
        qForward=0.; // measure quality of interpolation
        qReverse=0.; // measures quality if we interpolated in the opposite direction.
        for( int j=0; j<numberOfDimensions; j++ )
        {
            if( numberOfDimensions==2 )
            {
      	real xra[2][2];
      	for(int axis=0; axis<numberOfDimensions; axis++ ) 
      	{
        	  xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
        	  xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
      	}
      	q2=(square((rx(0,0,0)*XR(0,j)+rx(0,0,1)*XR(1,j))/dr2(0)) +
          	    square((rx(0,1,0)*XR(0,j)+rx(0,1,1)*XR(1,j))/dr2(1)) );
            }
            else
            {
      	real xra[3][3];
      	for(int axis=0; axis<numberOfDimensions; axis++ ) 
      	{
        	  xra[0][axis]=(vertex(i1+1,i2,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(0);
        	  xra[1][axis]=(vertex(i1,i2+1,i3,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(1);
        	  xra[2][axis]=(vertex(i1,i2,i3+1,axis)-vertex(i1,i2,i3,axis))/c.gridSpacing(2);
      	}
      	q2=(square((rx(0,0,0)*XR(0,j)+rx(0,0,1)*XR(1,j)+rx(0,0,2)*XR(2,j))/dr2(0)) +
          	    square((rx(0,1,0)*XR(0,j)+rx(0,1,1)*XR(1,j)+rx(0,1,2)*XR(2,j))/dr2(1)) +
          	    square((rx(0,2,0)*XR(0,j)+rx(0,2,1)*XR(1,j)+rx(0,2,2)*XR(2,j))/dr2(2)));
            }
            q2=SQRT(q2)*c.gridSpacing(j);
      // q2 = ratio of grid spacing on the interpolation grid to the interpolee grid.
            q+=q2+1./max(.001,q2);
            qReverse=max(qReverse,q2);
            qForward=max(qForward,1./q2);
        }
    //real quality=.5*q/numberOfDimensions;   // the smaller the better (1=min)
        quality=qReverse/qForward;         
    }
            
    return quality;
}
#undef XR

int Ogen::
improveQuality( CompositeGrid & cg, const int & grid, RealArray & removedPointBound )
// =============================================================================================================
// /Description:
//    Try to remove interpolation points on this grid so as to improve the quality of the interpolation.
//  This routine must be called after computing all interpolation points.
//
//  The basic algorithm is to start at the highest priority grids (which tend to lose fewer points) and
// see if it makes sense to remove some of the interpolation points (which are not required) in order
// to get better quality interpolation.
// =============================================================================================================
{
    const int numberOfBaseGrids = cg.numberOfBaseGrids();
    const int numberOfDimensions = cg.numberOfDimensions();
    const int & qualityAlgorithm = dbase.get<int>("qualityAlgorithm");

    int debugq=0; // 7; 

    if( true || debugq & 1  )
        printF("--OGEN-- improveQuality grid=%i, qualityAlgorithm=%i (0=area, 1=area+dist-to-bndry)\n",
         	   grid,qualityAlgorithm);

    MappedGrid & c = cg[grid];
    intArray & mask = c.mask();
    intArray & inverseGrid = cg.inverseGrid[grid];
//  realArray & rI = cg.inverseCoordinates[grid];

  // -- the mask is apparently not set with ISneeded anymore??
    if( debugq & 8 )
      displayMaskNeeded( mask,sPrintF("mask with needed points, grid=%i",grid));

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  // *wdh* getIndex(extendedGridIndexRange(c),I1,I2,I3);
    getIndex(c.extendedIndexRange(),I1,I2,I3);  // *wdh* 000322 : do not check far side periodic bndries

    int axis;
//   for( axis=0; axis<c.numberOfDimensions(); axis++ )
//   {
//     if( c.boundaryFlag(Start,axis)==MappedGridData::mixedPhysicalInterpolationBoundary )
//       Iv[axis]=Range(c.gridIndexRange(Start,axis)-1,Iv[axis].getBound());
//     if( c.boundaryFlag(End,axis)==MappedGridData::mixedPhysicalInterpolationBoundary )
//       Iv[axis]=Range(Iv[axis].getBase(),c.gridIndexRange(End,axis)+1);
//   }

    int iv[3], kv[3];
    int & i1 = iv[0];
    int & i2 = iv[1];
    int & i3 = iv[2];
    int jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2];

  //   int l =0;  // multigrid level
    realArray xx(1,3),rr(1,3),rx(1,3,3);
    
  // ***** first make a list of interpolation points on this grid that are
  //       on the outer boundary of interpolation points and are not needed for discretization 
    int i=0;
    intArray ia(I1.length()*I2.length()*I3.length()+1,3);
    
    int width=1;
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
        for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
        {
            for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
            {
      	if( mask(i1,i2,i3) & MappedGrid::ISinterpolationPoint  ) 
      	{
        	  const int grid2=inverseGrid(i1,i2,i3);
        	  assert( grid2>=0 && grid2<numberOfBaseGrids ); //  && grid2!=grid );

        	  if( grid2<grid ) 
        	  {
	    // int m=mask(i1,i2,i3);
          	    if( isOnInterpolationBoundary(c,iv,width) && !isNeededForDiscretization(c,iv)  ) 
          	    {

                            ia(i,0)=i1;
            	      ia(i,1)=i2;
            	      ia(i,2)=i3;
            	      i++;
          	    }
        	  }
      	}
            }
        }
    }
    int num=i;
    if( num==0 )
        return 0;

    realArray quality(I1,I2,I3);
    quality=0.;
    const real pointWasChecked = REAL_MAX;
    const real pointInList = REAL_MAX*.5;
    real qForward,qReverse;
    
    Range R(0,num-1);
    quality(ia(R,0),ia(R,1),ia(R,2))=pointInList;
    const int level=0;  // MG level
    int numberOfPointsRemoved=0;
    for( i=0; i<num; i++ )
    {
        i1=ia(i,0);
        i2=ia(i,1);
        i3=ia(i,2);

        bool okToRemove=true;
        if( mask(i1,i2,i3) & ISneededPoint ){ okToRemove=false; } // 
          if( debugq & 4 )
              printf("improveQuality: pt (%i,%i,%i) on grid %i : isNeeded=%i\n",i1,i2,i3,grid,int(!okToRemove));

    // if( quality(i1,i2,i3)!=pointWasChecked && !isNeededForDiscretization(c,iv)  ) 
        if( quality(i1,i2,i3)!=pointWasChecked && okToRemove && !isNeededForDiscretization(c,iv)  ) 
        {
            
            const int grid2=inverseGrid(i1,i2,i3);
            assert( grid2>=0 && grid2<numberOfBaseGrids && grid2!=grid );

            MappedGrid & g2 = cg[grid2];

      // compute the quaility of the interpolation
            quality(i1,i2,i3) =computeInterpolationQuality(cg,grid,i1,i2,i3,qForward,qReverse,qualityAlgorithm);

            if( debugq & 4 )
                printf("improveQuality: pt (%i,%i,%i) on grid %i has interp. quality=%6.2e (Area1=%6.2e,Area2=%6.2e) from grid %i isNeeded=%i\n",
             	       i1,i2,i3,grid,quality(i1,i2,i3),qForward,qReverse,grid2,int(!okToRemove));

            if( quality(i1,i2,i3) > qualityBound )
            {
        // remove this point if one interpolation neighbour has a better quality interpolation.
                real q=quality(i1,i2,i3);
                const int j1Min = i1 > c.extendedIndexRange(0,axis1) ? i1-1 : i1;
                const int j1Max = i1 < c.extendedIndexRange(1,axis1) ? i1+1 : i1;
                const int j2Min = i2 > c.extendedIndexRange(0,axis2) ? i2-1 : i2;
                const int j2Max = i2 < c.extendedIndexRange(1,axis2) ? i2+1 : i2;
                const int j3Min = i3 > c.extendedIndexRange(0,axis3) ? i3-1 : i3;
                const int j3Max = i3 < c.extendedIndexRange(1,axis3) ? i3+1 : i3;
                bool notDone=TRUE;
                for( j3=j3Min; j3<=j3Max && notDone; j3++ )
                for( j2=j2Min; j2<=j2Max && notDone; j2++ )
                for( j1=j1Min; j1<=j1Max; j1++ )
      	{
                    if( mask(j1,j2,j3)<0 )
        	  {
          	    if( quality(j1,j2,j3)==0. )
          	    {
	      // compute the quality for this point
            	      quality(j1,j2,j3)=computeInterpolationQuality(cg,grid,j1,j2,j3,qForward,qReverse,qualityAlgorithm);
          	    }
	    // printf("              : neighbour: quality(%i,%i,%i)=%6.2e)\n",j1,j2,j3,quality(j1,j2,j3));

            // *wdh* July 20, 2016 -- add this check, requires classify to have called markPointsReallyNeededForInterpolation

          	    if( quality(j1,j2,j3) < q )
          	    {
              // ------------------- Remove this point -------------------------

                            if( debugq & 4 )
            	      {
                  	        printf("         ***  : remove point (%i,%i,%i) on grid %i, quality=%6.2e "
                                      "(quality(%i,%i,%i)=%6.2e) okToRemove=%i\n",
                   		       i1,i2,i3,grid,quality(i1,i2,i3),j1,j2,j3,quality(j1,j2,j3),(int)okToRemove);
            	      }
                            if( debugq & 8 )
            	      {
                                int w=2; // half width of stencil to check 
                                int k3Min=numberOfDimensions==2 ? i3 : i3-w, k3Max=numberOfDimensions==2 ? i3 : i3+w;
                                int k2Min=i2-w, k2Max=i2+w;
                                int k1Min=i1-w, k1Max=i1+w;
             		 
            		printf("Nearby mask values: ( -3,3=ISneededPoint)\n");
            		for( int k3=k3Min; k3<=k3Max; k3++ )
            		{
                                    for( int k2=k2Min; k2<=k2Max; k2++ )
              		  {
                                        for( int k1=k1Min; k1<=k1Max; k1++ )
                		    {
                                            int imask=mask(k1,k2,k3);
                  		      if( imask>0 ){ imask=1; }else if( imask<0 ){ imask=-1; }

                                            if(  mask(k1,k2,k3) & ISneededPoint ){ imask*=3; }
                                            printf(" %2i",imask);   // 
                		    }
                                        printf("\n");
              		  }
            		}
		// printf("IsNeededForDiscretization:\n");
		// for( int k3=k3Min; k3<=k3Max; k3++ )
		// {
                //   for( int k2=k2Min; k2<=k2Max; k2++ )
		//   {
                //     for( int k1=k1Min; k1<=k1Max; k1++ )
		//     {
                //       kv[0]=k1; kv[1]=k2; kv[3]=k3;// 
                //       int isNeeded = isNeededForDiscretization(c,kv);
		//       printf(" %i",isNeeded);
		//     }
                //     printf("\n");
		//   }
		// }
            		
            	      } // end if debugq 
            	      

              // --- remove the point ---
            	      mask(i1,i2,i3)=0;

                            numberOfPointsRemoved++;
            	      quality(i1,i2,i3)=pointWasChecked;
            	      notDone=FALSE;
              // Keep track of the region in the unit square that encloses all removed points. This is used later.
                            real r;
                            for( axis=0; axis<numberOfDimensions; axis++ )
            	      {
                // over-estimate the discretization cell width by a factor of two. May be needed for boundaries.
                                const real width = cg.interpolationWidth(axis,grid,grid2,level)*c.gridSpacing(axis);
                                r = (iv[axis]-c.indexRange(Start,axis))*c.gridSpacing(axis);
                                removedPointBound(Start,axis,grid)=min(removedPointBound(Start,axis,grid),r-width);
                                removedPointBound(End  ,axis,grid)=max(removedPointBound(End  ,axis,grid),r+width);
            	      }
            	      break;
          	    }
        	  }
      	}
                if( !notDone )
      	{
          // if the point was removed, add any unchecked neighbouring points to the list
        	  for( int j3=j3Min; j3<=j3Max; j3++ )
          	    for( int j2=j2Min; j2<=j2Max; j2++ )
            	      for( int j1=j1Min; j1<=j1Max; j1++ )
            	      {
            		if( mask(j1,j2,j3)<0 && quality(j1,j2,j3)!=pointWasChecked && quality(j1,j2,j3)!=pointInList )
            		{
              		  const int grid3=inverseGrid(j1,j2,j3);
              		  assert( grid3>=0 && grid3<numberOfBaseGrids && grid3!=grid );
                                    if( grid3<grid )
              		  {
                		    ia(num,0)=j1; 
                		    ia(num,1)=j2; 
                		    ia(num,2)=j3;
                		    num++;
                		    quality(j1,j2,j3)=pointInList; // *** fix this ***
              		  }
            		}
            	      }
      	}
      	
            }  // end if quality < bound
            
            quality(i1,i2,i3)=pointWasChecked;  

/* ----
            real distanceToBoundary=1000.;   // distance in grid lines to the boundary of the interpolee
            for( axis=0; axis<c.numberOfDimensions(); axis++ )
            {
      	if( !g2.isPeriodic(axis) )
      	{
        	  real ri = rI(i1,i2,i3,axis)/g2.gridSpacing(axis)+g2.indexRange(Start,axis);  
        	  distanceToBoundary=min(distanceToBoundary,ri-g2.indexRange(Start,axis),g2.indexRange(End,axis)-ri);
      	}
            }

            if( distanceToBoundary<2. )
            {
                printf("improveQuality: remove point (%i,%i,%i) on grid %i, dist=%6.2e \n",i1,i2,i3,grid,distanceToBoundary);
                mask(i1,i2,i3)=0;
                inverseGrid(i1,i2,i3)=-inverseGrid(i1,i2,i3);
            }
---- */
            
        }
    }

  // ----------------------------------------------------
  // ----------- unmark isNeeded Points -----------------
  // ----------------------------------------------------
  //  July 20, 2016
    getIndex(c.dimension(),I1,I2,I3);
    for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )
    {
        for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
        {
            for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
            {
      	if( mask(i1,i2,i3) & ISneededPoint  ) 
      	{
        	  mask(i1,i2,i3) = mask(i1,i2,i3) ^ ISneededPoint;  // ^ = XOR
	  // if( mask(i1,i2,i3) & ISneededPoint  ) 
	  // {
	  //   OV_ABORT("error -- XOR did not work");
	  // }
          	    
      	}
            }
        }
    }
    
//   if(numberOfPointsRemoved>0 )
//   {
//     c.mask().periodicUpdate(); // ***** 000322
//   }
    
    if( debugq || (info & 4) )
        printF("Grid %s: Number of points removed to improve quality = %i\n",
              (const char*)c.mapping().getName(Mapping::mappingName),numberOfPointsRemoved);
    
    return 0;
}
