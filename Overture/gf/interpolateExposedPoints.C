#include "Overture.h"
#include "OGFunction.h"
#include "InterpolatePoints.h"
#include "display.h"

// extern IntegerArray Overture::nullIntArray();
#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_2D(i1,i2,I1,I2) \
int I1Base =I1.getBase(),   I2Base =I2.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(); \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define XY(i1,i2,i3) center(i1,i2,i3,axis1),center(i1,i2,i3,axis2)
#define XYZ(i1,i2,i3) center(i1,i2,i3,axis1),center(i1,i2,i3,axis2),center(i1,i2,i3,axis3)
//\begin{>interpolateExposedPointsInclude.tex}{}
int 
interpolateExposedPoints(CompositeGrid & cg1,  
                         CompositeGrid & cg2, 
                         realCompositeGridFunction & u1,
                         OGFunction *TZFlow /* =NULL */,
                         real t /* =0. */,
                         const bool & returnIndexValues /* =FALSE */,
                         IntegerArray & numberPerGrid0 /* = Overture::nullIntArray() */,
                         intArray & ia0 /* = Overture::nullIntArray() */,
                         int stencilWidth /* = -1 */ )
//===========================================================================
// /Purpose:
//   Assign values to exposed points in a moving grid
//
//  /cg1 (input): grid and grid function at old time
//  /cg2 (input): grid at new time
//  /u1: A grid function on grid cg1. On output, exposed points are interpolated
//  /TZFlow: If specified and non-NULL this pointer to a twilight-zone function
//           will be used to compute the error in the interpolation. This is used
//          for debugging.
//  /t: Evaluate the twilight-zone function at this time (the time corresponding to cg1). 
//  /returnIndexValues (bool) : if TRUE return points that were interpolated in the array ia:
//   \begin{verbatim} 
//     ia(i,0:2) = (i1,i2,i3) where the number of points from each grid is stored in numberPerGrid(grid) 
//      thus    ia(i,0:2) : points interpolated from grid=0 for i=0,...,numberPerGrid(0)-1
//              ia(i,0:2) : points interpolated from grid=1 for i=numberPerGrid(0),...,numberPerGrid(1)-1
//   \end{verbatim} 
//  /stencilWidth (input): interpolate enough points to support a discrete stencil of this width.
//       A second-order centered approximation, for example,  would use stencilWidth=3.
//       By default stencilWith equals the discretization width defined by the grid.
//
//  /Remarks:
//   Here is a picture of a 1D moving overlapping grid that illustrates the exposed
//   point on the old grid that requires a value so the solution can be advanced
//   to the new grid.
//   \begin{verbatim} 
//
//        +---+---+---+---I                       Old grid, point * is unused
//                 *---I---+---+---+---+          Points marked I are interpolation
//                 1   2   3   4   5
//
//        +---+---+---+---I
//         move-->   I---+---+---+---+---+        New grid, requires a value on the OLD grid
//                   1   2   3   4                at point * to compute a derivative at point 2
//   \end{verbatim} 
//
//  /NOTE:
//    This routine assumes (and checks) that the number of grid points has NOT changed!
//\end{interpolateExposedPointsInclude.tex}
//===========================================================================
{
  
  int returnValue=0;
  const int debug=0; // 1;
  if( debug & 1 ) printf("\n =================== interpolateExposedPoints ====================\n");
  
  const int numberOfDimensions = cg1.numberOfDimensions();
  
  Range N(u1.getComponentBase(0),u1.getComponentBound(0));
  const int NBase=N.getBase();
  const int NBound=N.getBound();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  IntegerArray numberPerGrid(cg1.numberOfComponentGrids());
  
  intArray ia_(1000,3);
  int *iap = ia_.Array_Descriptor.Array_View_Pointer1;
  int iaDim0=ia_.getRawDataSize(0);
#define ia(i0,i1) iap[i0+iaDim0*(i1)]

  real maxError=0.;
  
  int numberOfExposedPoints=0;
  int numberNotFound=0;
  int grid;
  for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg1[grid];
    int numPerGrid=0;
    if( cg1.numberOfInterpolationPoints(grid)>0 )
    {
      intArray & mask1_ = cg1[grid].mask();
      const intArray & mask2_ = cg2[grid].mask();
      
      const int * mask1p = mask1_.Array_Descriptor.Array_View_Pointer2;
      const int mask1Dim0=mask1_.getRawDataSize(0);
      const int mask1Dim1=mask1_.getRawDataSize(1);
#define mask1(i0,i1,i2) mask1p[i0+mask1Dim0*(i1+mask1Dim1*(i2))]
      const int * mask2p = mask2_.Array_Descriptor.Array_View_Pointer2;
      const int mask2Dim0=mask2_.getRawDataSize(0);
      const int mask2Dim1=mask2_.getRawDataSize(1);
#define mask2(i0,i1,i2) mask2p[i0+mask2Dim0*(i1+mask2Dim1*(i2))]

      if( max(abs(cg1[grid].dimension()-cg2[grid].dimension()))!=0 )
      {
	cout << "interpolateExposedPoints:ERROR: component grids must have the same number of points!\n";
        char buff[40];
        cg1[grid].dimension().display(sPrintF(buff,"Here is cg1[%i].dimension()",grid));
        cg1[grid].indexRange().display(sPrintF(buff,"Here is cg1[%i].indexRange()",grid));
        cg1[grid].numberOfGhostPoints().display(sPrintF(buff,"Here is cg1[%i].numberOfGhostPoints()",grid));
        cg2[grid].dimension().display(sPrintF(buff,"Here is cg2[%i].dimension()",grid));
        cg2[grid].indexRange().display(sPrintF(buff,"Here is cg2[%i].indexRange()",grid));
        cg2[grid].numberOfGhostPoints().display(sPrintF(buff,"Here is cg2[%i].numberOfGhostPoints()",grid));
	return 1;
      }
/* -----
     printf("bc and Mask on old grid, grid=%s \n",(const char *)cg1[grid].mapping.getName(Mapping::mappingName));
     cg1[grid].boundaryCondition().display("bc");
     display(cg1[grid].mask(),"mask",NULL,"%11i ");
     printf("bc Mask on new grid, grid=%s \n",(const char *)cg2[grid].mapping.getName(Mapping::mappingName));
     cg2[grid].boundaryCondition().display("bc");
     display(cg2[grid].mask(),"mask",NULL,"%11i ");
---- */

      //
      // Exposed points on grid1: mask1==0 && mask2 !=0 
      //
      
     int widthOfTheBorder=stencilWidth/2;
     if( stencilWidth<= 0 )
     {
       widthOfTheBorder = max(c.discretizationWidth())/2;
     }
     assert( widthOfTheBorder>=1 && widthOfTheBorder<=10 );
     if( debug & 1 ) printf(" **** interpExposed: widthOfTheBorder=%i ****\n",widthOfTheBorder );

      // choose the places to look for exposed points -- do not check ghost points outside
      // of interpolation boundaries

     // **** check this *** for AMR grids ***
     getIndex( c.gridIndexRange(),I1,I2,I3,widthOfTheBorder); 

      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
	if( c.boundaryCondition()(Start,axis)==0 && c.boundaryCondition()(End,axis)!=0 )
          Iv[axis]=Range(c.indexRange()(Start,axis),Iv[axis].getBound());
	else if( c.boundaryCondition()(Start,axis)!=0 && c.boundaryCondition()(End,axis)==0 )
          Iv[axis]=Range(Iv[axis].getBase(),c.indexRange()(End,axis));
	else if( c.boundaryCondition()(Start,axis)==0 && c.boundaryCondition()(End,axis)==0 )
          Iv[axis]=Range(c.indexRange()(Start,axis),c.indexRange()(End,axis));
      }

      int i1,i2,i3, i1m1,i2m1,i3m1, i1p1,i2p1,i3p1, i1m2,i2m2,i3m2, i1p2,i2p2,i3p2;

      const IntegerArray & dimension = c.dimension();
      const int nd1a=dimension(0,0), nd1b=dimension(1,0);
      const int nd2a=dimension(0,1), nd2b=dimension(1,1);
      const int nd3a=dimension(0,2), nd3b=dimension(1,2);
      

      const int I3Base =I3.getBase(), I3Bound=I3.getBound(); 
      for( i3=I3Base; i3<=I3Bound; i3++ )
      {
        // avoid looking at neighbours that are outside the array dimensions:
	i3m1=max(nd3a,i3-1); i3m2=max(nd3a,i3-2);
	i3p1=min(nd3b,i3+1); i3p2=min(nd3b,i3+2);
	
	// allocate space for the ia array
	if( numberOfExposedPoints+I1.length()*I2.length() > ia_.getLength(0) )
	{
	  ia_.resize(numberOfExposedPoints+I1.length()*I2.length()*numberOfDimensions ,3);
	  iap = ia_.Array_Descriptor.Array_View_Pointer1;
	  iaDim0=ia_.getRawDataSize(0);
	}
	
	if( numberOfDimensions==2 )
	{
          if( false )
	  {
	    // old way 
	    for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
		if( mask1(i1,i2,i3)==0 
		    && ( mask2(i1-1,i2-1,i3)!=0 || mask2(i1  ,i2-1,i3)!=0 || mask2(i1+1,i2-1,i3)!=0 || 
			 mask2(i1-1,i2  ,i3)!=0 || mask2(i1  ,i2  ,i3)!=0 || mask2(i1+1,i2  ,i3)!=0 || 
			 mask2(i1-1,i2+1,i3)!=0 || mask2(i1  ,i2+1,i3)!=0 || mask2(i1+1,i2+1,i3)!=0 ) )
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
	  }
	  else if( widthOfTheBorder==1 )
	  {
            FOR_2D(i1,i2,I1,I2)
	    {
	      if( mask1(i1,i2,i3)==0 )
	      {
                // avoid looking at neighbours that are outside the array dimensions:
		i1m1=max(nd1a,i1-1); i1p1=min(nd1b,i1+1); 
		i2m1=max(nd2a,i2-1); i2p1=min(nd2b,i2+1); 
		
		if( mask2(i1m1,i2m1,i3)>0 || mask2(i1  ,i2m1,i3)>0 || mask2(i1p1,i2m1,i3)>0 || 
		    mask2(i1m1,i2  ,i3)>0 || mask2(i1  ,i2  ,i3)>0 || mask2(i1p1,i2  ,i3)>0 || 
		    mask2(i1m1,i2p1,i3)>0 || mask2(i1  ,i2p1,i3)>0 || mask2(i1p1,i2p1,i3)>0 )
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
	  }
	  else if( widthOfTheBorder==2 )
	  {
	    
            FOR_2D(i1,i2,I1,I2)
	    {
              
	      if( mask1(i1,i2,i3)==0 )
	      {
                // avoid looking at neighbours that are outside the array dimensions:
		i1m1=max(nd1a,i1-1); i1m2=max(nd1a,i1-2); i2m1=max(nd2a,i2-1); i2m2=max(nd2a,i2-2);
		i1p1=min(nd1b,i1+1); i1p2=min(nd1b,i1+2); i2p1=min(nd2b,i2+1); i2p2=min(nd2b,i2+2);

#define MASK5(i2) mask2(i1m2,i2,i3)>0 || mask2(i1m1,i2,i3)>0 || mask2(i1,i2,i3)>0 || \
                  mask2(i1p1,i2,i3)>0 || mask2(i1p2,i2,i3)>0

		if( MASK5(i2m2) || MASK5(i2m1) || MASK5(i2) || MASK5(i2p1) || MASK5(i2p2) )
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
#undef MASK5
	  }
	  else
	  {
	    printf("ERROR: widthOfTheBorder=%i : not implemented\n",widthOfTheBorder);
	    Overture::abort("error");
	  }
	  
	}
	else // 3D
	{
          if( false )
	  {
	    // old
	    for( int i2=I2.getBase(); i2<=I2.getBound(); i2++ )
	    {
	      for( int i1=I1.getBase(); i1<=I1.getBound(); i1++ )
	      {
		if( mask1(i1,i2,i3)==0 
		    && ( mask2(i1-1,i2-1,i3-1)!=0 || mask2(i1  ,i2-1,i3-1)!=0 || mask2(i1+1,i2-1,i3-1)!=0 || 
			 mask2(i1-1,i2  ,i3-1)!=0 || mask2(i1  ,i2  ,i3-1)!=0 || mask2(i1+1,i2  ,i3-1)!=0 || 
			 mask2(i1-1,i2+1,i3-1)!=0 || mask2(i1  ,i2+1,i3-1)!=0 || mask2(i1+1,i2+1,i3-1)!=0 ||
			 mask2(i1-1,i2-1,i3  )!=0 || mask2(i1  ,i2-1,i3  )!=0 || mask2(i1+1,i2-1,i3  )!=0 || 
			 mask2(i1-1,i2  ,i3  )!=0 || mask2(i1  ,i2  ,i3  )!=0 || mask2(i1+1,i2  ,i3  )!=0 || 
			 mask2(i1-1,i2+1,i3  )!=0 || mask2(i1  ,i2+1,i3  )!=0 || mask2(i1+1,i2+1,i3  )!=0 ||
			 mask2(i1-1,i2-1,i3+1)!=0 || mask2(i1  ,i2-1,i3+1)!=0 || mask2(i1+1,i2-1,i3+1)!=0 || 
			 mask2(i1-1,i2  ,i3+1)!=0 || mask2(i1  ,i2  ,i3+1)!=0 || mask2(i1+1,i2  ,i3+1)!=0 || 
			 mask2(i1-1,i2+1,i3+1)!=0 || mask2(i1  ,i2+1,i3+1)!=0 || mask2(i1+1,i2+1,i3+1)!=0 ) )
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
	  }
	  else if( widthOfTheBorder==1 )
	  {
            FOR_2D(i1,i2,I1,I2)
	    {
	      if( mask1(i1,i2,i3)==0 )
	      {
                // avoid looking at neighbours that are outside the array dimensions:
		i1m1=max(nd1a,i1-1); i1p1=min(nd1b,i1+1); 
		i2m1=max(nd2a,i2-1); i2p1=min(nd2b,i2+1); 

		if( mask2(i1m1,i2m1,i3m1)>0 || mask2(i1  ,i2m1,i3m1)>0 || mask2(i1p1,i2m1,i3m1)>0 || 
		    mask2(i1m1,i2  ,i3m1)>0 || mask2(i1  ,i2  ,i3m1)>0 || mask2(i1p1,i2  ,i3m1)>0 || 
		    mask2(i1m1,i2p1,i3m1)>0 || mask2(i1  ,i2p1,i3m1)>0 || mask2(i1p1,i2p1,i3m1)>0 ||
		    mask2(i1m1,i2m1,i3  )>0 || mask2(i1  ,i2m1,i3  )>0 || mask2(i1p1,i2m1,i3  )>0 || 
		    mask2(i1m1,i2  ,i3  )>0 || mask2(i1  ,i2  ,i3  )>0 || mask2(i1p1,i2  ,i3  )>0 || 
		    mask2(i1m1,i2p1,i3  )>0 || mask2(i1  ,i2p1,i3  )>0 || mask2(i1p1,i2p1,i3  )>0 ||
		    mask2(i1m1,i2m1,i3p1)>0 || mask2(i1  ,i2m1,i3p1)>0 || mask2(i1p1,i2m1,i3p1)>0 || 
		    mask2(i1m1,i2  ,i3p1)>0 || mask2(i1  ,i2  ,i3p1)>0 || mask2(i1p1,i2  ,i3p1)>0 || 
		    mask2(i1m1,i2p1,i3p1)>0 || mask2(i1  ,i2p1,i3p1)>0 || mask2(i1p1,i2p1,i3p1)>0 ) 
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
	  }
	  else if( widthOfTheBorder==2 )
	  {
#define MASK5(i2,i3) mask2(i1m2,i2,i3)>0 || mask2(i1m1,i2,i3)>0 || mask2(i1,i2,i3)>0 || \
                     mask2(i1p1,i2,i3)>0 || mask2(i1p2,i2,i3)>0

#define MASK55(i3) MASK5(i2m2,i3) || MASK5(i2m1,i3) || MASK5(i2,i3) || MASK5(i2p1,i3) || MASK5(i2p2,i3)

            FOR_2D(i1,i2,I1,I2)
	    {
	      if( mask1(i1,i2,i3)==0 )
	      {
		i1m1=max(nd1a,i1-1); i1m2=max(nd1a,i1-2); i2m1=max(nd2a,i2-1); i2m2=max(nd2a,i2-2);
		i1p1=min(nd1b,i1+1); i1p2=min(nd1b,i1+2); i2p1=min(nd2b,i2+1); i2p2=min(nd2b,i2+2);

                if( MASK55(i3m2) || MASK55(i3m1) || MASK55(i3) || MASK55(i3p1) || MASK55(i3p2) )
		{
		  ia(numberOfExposedPoints,0)=i1;
		  ia(numberOfExposedPoints,1)=i2;
		  ia(numberOfExposedPoints,2)=i3;
		  numberOfExposedPoints++;
		  numPerGrid++;
		}
	      }
	    }
#undef MASK5
#undef MASK55

	  }
	  else
	  {
	    printf("ERROR: widthOfTheBorder=%i : not implemented\n",widthOfTheBorder);
	    Overture::abort("error");
	  }

	}

      }  // end for i3
    }  // if( cg.numberOfInterpolationPoints
    numberPerGrid(grid)=numPerGrid;
  } // end for grid
  
  if( numberOfExposedPoints>0 )
  {
    // Expose points were found

    Range R=numberOfExposedPoints;
    RealArray x_(numberOfExposedPoints,numberOfDimensions),uInterpolated_(R,N);
    real *xp = x_.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x_.getRawDataSize(0);
#define x(i0,i1) xp[i0+xDim0*(i1)]
    real *uInterpolatedp = uInterpolated_.Array_Descriptor.Array_View_Pointer1;
    const int uInterpolatedDim0=uInterpolated_.getRawDataSize(0);
#define uInterpolated(i0,i1) uInterpolatedp[i0+uInterpolatedDim0*(i1)]

    int start=0;
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {
      if( numberPerGrid(grid)>0 )
      {
        int end=start+numberPerGrid(grid);
        MappedGrid & mg1 = cg1[grid];
	if( mg1.isRectangular() )
	{
	  real dx[3],xab[2][3];
	  mg1.getRectangularGridParameters( dx, xab );

	  const int i0a=mg1.gridIndexRange(0,0);
	  const int i1a=mg1.gridIndexRange(0,1);
	  const int i2a=mg1.gridIndexRange(0,2);

	  const real xa=xab[0][0], dx0=dx[0];
	  const real ya=xab[0][1], dy0=dx[1];
	  const real za=xab[0][2], dz0=dx[2];
	
#define COORD0(i0,i1,i2) (xa+dx0*(i0-i0a))
#define COORD1(i0,i1,i2) (ya+dy0*(i1-i1a))
#define COORD2(i0,i1,i2) (za+dz0*(i2-i2a))
	  if( numberOfDimensions==2 )
	  {
	    for( int i=start; i<end; i++ )
	    {
	      x(i,0)=COORD0(ia(i,0),ia(i,1),ia(i,2));     
	      x(i,1)=COORD1(ia(i,0),ia(i,1),ia(i,2));     
	    }
	  }
	  else
	  {
	    for( int i=start; i<end; i++ )
	    {
	      x(i,0)=COORD0(ia(i,0),ia(i,1),ia(i,2));     
	      x(i,1)=COORD1(ia(i,0),ia(i,1),ia(i,2));     
	      x(i,2)=COORD2(ia(i,0),ia(i,1),ia(i,2));     
	    }
	  }
	  
	}
	else
	{
	  mg1.update(MappedGrid::THEcenter);
	  const realArray & center_ = mg1.center();
	  const real *centerp = center_.Array_Descriptor.Array_View_Pointer3;
	  const int centerDim0=center_.getRawDataSize(0);
	  const int centerDim1=center_.getRawDataSize(1);
	  const int centerDim2=center_.getRawDataSize(2);
#define center(i0,i1,i2,i3) centerp[i0+centerDim0*(i1+centerDim1*(i2+centerDim2*(i3)))]
	
	  if( numberOfDimensions==2 )
	  {
	    for( int i=start; i<end; i++ )
	    {
	      x(i,0)=center(ia(i,0),ia(i,1),ia(i,2),0);     
	      x(i,1)=center(ia(i,0),ia(i,1),ia(i,2),1);     
	    }
	  }
	  else
	  {
	    for( int i=start; i<end; i++ )
	    {
	      x(i,0)=center(ia(i,0),ia(i,1),ia(i,2),0);     
	      x(i,1)=center(ia(i,0),ia(i,1),ia(i,2),1);     
	      x(i,2)=center(ia(i,0),ia(i,1),ia(i,2),2);     
	    }
	  }
	  // if( axis==0 ) printf(" i=%i ia=%i %i %i \n",i,ia(i,0),ia(i,1),ia(i,2));
	}
#undef center	
	start=end;
      }
    }
    // interpolate all exposed points:    
    InterpolatePoints interp;
    int errCode = interp.interpolatePoints(x_,u1,uInterpolated_);
    numberNotFound+=abs(errCode);

    if( debug & 1 )
    {
      IntegerArray indexValues, interpoleeGrid;
      interp.getInterpolationInfo(cg1,indexValues,interpoleeGrid);
      for( int i=0; i<=indexValues.getBound(0); i++ )
      {
	printf(" pt: i=%i ia=(%i,%i) il=(%i,%i) interpoleeGrid=%i\n",i,ia(i,0),ia(i,1),
            indexValues(i,0),indexValues(i,1),interpoleeGrid(i));
      }
    }

    start=0;
    for( grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
    {
      if( numberPerGrid(grid)>0 )
      {
	const realArray & u1g_= u1[grid];
	real *u1gp = u1g_.Array_Descriptor.Array_View_Pointer3;
	const int u1gDim0=u1g_.getRawDataSize(0);
	const int u1gDim1=u1g_.getRawDataSize(1);
	const int u1gDim2=u1g_.getRawDataSize(2);
#define u1g(i0,i1,i2,i3) u1gp[i0+u1gDim0*(i1+u1gDim1*(i2+u1gDim2*(i3)))]

	int end=start+numberPerGrid(grid);
	for( int n=NBase; n<=NBound; n++)
	{
	  for( int i=start; i<end; i++ )
	  {
	    u1g(ia(i,0),ia(i,1),ia(i,2),n)=uInterpolated(i,n);
	  }
	}
        if( debug & 1 )
	{
	  for( int i=start; i<end; i++ )
	  {
	    printf("interpExposed:grid=%i i=%i iv=(%i,%i,%i) x=(%8.2e,%8.2e) interp: u=(",grid,
           i,ia(i,0),ia(i,1),ia(i,2),x(i,0),x(i,1));
	    for( int n=NBase; n<=NBound; n++)
	    {
              printf("%9.3e,",u1g(ia(i,0),ia(i,1),ia(i,2),n));
	    }
            printf(")\n");
	  }
	}
	
	if( TZFlow )
	{ // compute errors
	  cg1[grid].update(MappedGrid::THEcenter);
	  const realArray & center = cg1[grid].center();

	  real err;
	  if( numberOfDimensions==2 )
	  {
	    for( int i=start; i<end; i++ )
	    {
	      int i1=ia(i,0);
	      int i2=ia(i,1);
	      int i3=ia(i,2);
	      for( int n=NBase; n<=NBound; n++)
	      {
                
		err=fabs(u1g(i1,i2,i3,n)-(*TZFlow)(XY(i1,i2,i3),0.,n,t));
		maxError=max(maxError,err);
		// printf("interpolateExposedPoints: grid=%s at (i1,i2,i3,n)=(%i,%i,%i,%i), error=%e, u1=%e, "
		//       "true=%e, maskOld=%i, maskNew=%i\n",
		//       (const char *)cg1[grid].mapping.getName(Mapping::mappingName),
		//    i1,i2,i3,n,err,u1[grid](i1,i2,i3,n),TZFlow->v(XY(i1,i2,i3),0.,n,t),
		//        mask1(i1,i2,i3),cg2[grid].mask()(i1,i2,i3) );

	      }
	    }
	  }
	  else
	  {
	    for( int i=start; i<end; i++ )
	    {
	      int i1=ia(i,0);
	      int i2=ia(i,1);
	      int i3=ia(i,2);
	      for (int n=NBase; n<=NBound; n++)
	      {
		maxError=max(maxError,fabs(u1g(i1,i2,i3,n)-(*TZFlow)(XYZ(i1,i2,i3),n,t)));
		// printf("interpolateExposedPoints: at (%i,%i,%i,n) error =%e \n",i1,i2,i3,n,error);
	      }
	    }
	  }
	} // end if TZFlow
	start=end;
      }
    } // end for grid
  }
  
  if( numberNotFound>0 && cg1.numberOfComponentGrids()>1 )
    printf("**interpolateExposedPoints: numberOfExposedPoints=%i, numberNotFound=%i\n",
        	 numberOfExposedPoints,numberNotFound);

  if( (debug & 1) && TZFlow )
    printf("interpolateExposedPoints: %i exposed pts interpolated max error =%8.2e \n",numberOfExposedPoints,maxError);

  if( returnIndexValues )
  {
    ia_.resize(numberOfExposedPoints,3);
    ia0.reference(ia_);
    numberPerGrid0.reference(numberPerGrid);
  }
  
  return returnValue;
  
}
#undef XY
#undef XYZ

    
#undef MASK1
#undef MASK2
