#include "Oges.h"

#undef  ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )

// extend ghostlines by amounts extra1,extra2,extra3
#define getGhostIndex3( ghost,extra1,extra2,extra3,side,axis,Ig1,Ig2,Ig3 ) \
    R[0]=Range(c.indexRange()(Start,axis1)-extra1,c.indexRange()(End,axis1)+extra1); \
    R[1]=Range(c.indexRange()(Start,axis2)-extra2,c.indexRange()(End,axis2)+extra2); \
    R[2]=numberOfDimensions==2 ?  \
         Range(c.indexRange()(Start,axis3)       ,c.indexRange()(End,axis3)       ): \
         Range(c.indexRange()(Start,axis3)-extra3,c.indexRange()(End,axis3)+extra3); \
    R[axis]= side==0? \
      Range(c.indexRange()(Start,axis)-ghost,c.indexRange()(Start,axis)-ghost):  \
      c.isPeriodic()(axis) ? \
      Range(c.indexRange()(End  ,axis)+ghost+1,c.indexRange()(End  ,axis)+ghost+1): \
      Range(c.indexRange()(End  ,axis)+ghost  ,c.indexRange()(End  ,axis)+ghost  ); \
    Ig1=R[0];   \
    Ig2=R[1];   \
    Ig3=R[2];  


void Oges::
assignClassification()
{
//=================================================================================
//        Set the values in the classify array
//
//
//        classify[grid](i1,i2,i3) = interior      ( greater than zero )
//                                 = boundary      ( greater than zero )
//                                 = ghost1        ( greater than zero )
//                                 = ghost2        ( greater than zero )
//                                 = ...
//                                 = 10+i    (extra equation i=0,1,...)
//                                 = interpolation ( less than zero )
//                                 = extrapolation ( less than zero )
//                                 = periodic      ( less than zero )
//                                 = unused   (= zero )
//=================================================================================

  if( Oges::debug & 8 )
    cout << "Entering assignClassification " << endl;

  Index I1,I2,I3,Ib1,Ib2,Ib3,Ig1,Ig2,Ig3,Ie1,Ie2,Ie3,R[3];
  Index In(0,numberOfComponents);
  int side,axis;
  int n;

  
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    const IntegerArray & mask = cg[grid].mask();
    IntegerArray & cl = classify[grid];
    
    if( Oges::debug & 16 ) 
      c.mask().display("classify: here is the mask array");

    // Notes:
    //   getBoundaryIndex should use myIndexRange, defined below
    //   getGhostIndexRange should also use myGridIndex
    const IntegerArray & myIndexRange = (int)cg[grid].isAllVertexCentered() ? cg[grid].gridIndexRange() 
                                                                      : cg[grid].indexRange();
    cl=unused;  // default  -- this is inefficient ---

    // mark interpolation points   ******************* would be faster to use interpolationPoint ***
    getIndex( c.extendedIndexRange(),I1,I2,I3 );   
    cl(I1,I2,I3)=interior;
    for( n=0; n<numberOfComponents; n++ )
      where( mask(I1,I2,I3) < 0 )
	cl(I1,I2,I3,n)=interpolation;

    // mark periodic boundaries
    ForBoundary(side,axis)
    {
      if( c.boundaryCondition(side,axis)<0 )
      {
	for( int ghost=1; ghost<=numberOfGhostLines+side; ghost++ )  // for each ghost line
	{
	  getGhostIndex(c.indexRange(),side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);  
	  cl(Ig1,Ig2,Ig3,In)=periodic;
	}
      }
    }

    // Assign classify on ghost lines (including "endpoints"):
    int ghostValue,num1,num2,num3;
    ForBoundary(side,axis)
    {
      for( int ghost=1; ghost<=numberOfGhostLines; ghost++ )  // for each ghost line
      {
        if( cg[grid].boundaryCondition()(side,axis) > 0 )
        {
          // First mark extended ghostline with default value=extrapolation
          getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);
          getGhostIndex(myIndexRange,side,axis,Ie1,Ie2,Ie3,    0,numberOfGhostLines);
          where( cl(Ie1,Ie2,Ie3,In) != (int)unused 
              && cl(Ie1,Ie2,Ie3,In) != (int)periodic )
           cl(Ig1,Ig2,Ig3,In)=extrapolation;

//          where( cl(Ie1,Ie2,Ie3,In) == periodic )  // **wdh** 950717
//           cl(Ig1,Ig2,Ig3,In)=periodic;

          for( n=0; n<numberOfComponents; n++ )
	  {
            switch (ghostLineOption[grid](side,axis,ghost,n))
    	    {
	    case extrapolateGhostLine:
              getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);
              getGhostIndex(myIndexRange,side,axis,Ie1,Ie2,Ie3,    0,numberOfGhostLines);
              ghostValue=extrapolation;
              break;
	    case useGhostLine:
              getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);
              getGhostIndex(myIndexRange,side,axis,Ie1,Ie2,Ie3,    0,numberOfGhostLines);
              ghostValue=interior+ghost+1;
              break;
	    case useGhostLineExceptCorner:
              getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,0);
              getGhostIndex(myIndexRange,side,axis,Ie1,Ie2,Ie3,    0,0);

              ghostValue=interior+ghost+1;
              break;
	    case useGhostLineExceptCornerAndNeighbours:
              num1 = cg[grid].isPeriodic()(axis1) ? 0 : -1; // use neighbours if periodic
              num2 = cg[grid].isPeriodic()(axis2) ? 0 : -1; // use neighbours if periodic
              num3 = cg[grid].isPeriodic()(axis3) ? 0 : -1; // use neighbours if periodic
              getGhostIndex3(ghost,num1,num2,num3,side,axis,Ig1,Ig2,Ig3);
              getGhostIndex3(0    ,num1,num2,num3,side,axis,Ie1,Ie2,Ie3);
              ghostValue=interior+ghost+1;
              break;
            default:
	      cerr << "assignClassification:ERROR unknown ghostLineOption! " << endl;
	      break;
	    }
            where( cl(Ie1,Ie2,Ie3,n) != (int)unused 
                && cl(Ie1,Ie2,Ie3,n) != (int)interpolation
                && cl(Ie1,Ie2,Ie3,n) != (int)periodic )
	      cl(Ig1,Ig2,Ig3,n)=ghostValue;
	  }
        }
/* ---
        else if( cg[grid].boundaryCondition()(side,axis) < 0 )
        {
          getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);
          getGhostIndex(myIndexRange,side,axis,Ie1,Ie2,Ie3,    0,numberOfGhostLines);
          where( cl(Ie1,Ie2,Ie3,In) != (int)unused )
            cl(Ig1,Ig2,Ig3,In)=periodic;
	}
        else if( cg[grid].boundaryCondition()(side,axis) == 0 )
        {
          getGhostIndex(myIndexRange,side,axis,Ig1,Ig2,Ig3,ghost,numberOfGhostLines);
          where( cl(Ig1,Ig2,Ig3,In) != (int)interpolation ) // *wdh* 980408 
            cl(Ig1,Ig2,Ig3,In)=unused;
	}
--- */
      }
    }
    // mark unused points
    getIndex( c.dimension(),I1,I2,I3 );   // *wdh* 980408
    for( n=0; n<numberOfComponents; n++ )
      where( mask(I1,I2,I3)==0 )
	cl(I1,I2,I3,n)=unused;


    if( Oges::debug & 8 )
    {
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
      cout << " 1=interior, 2=bndry, 3=ghost1, 4=ghost2, -1=interp, -2=periodic, -3=extrap, 0=unused\n";
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
      classify[grid].display("***Here is the NEW classify array***");
    }
  }
  if( numberOfExtraEquations > 0 )
  {
     // assign indices for the extra equation, set classify to 10+number
    findExtraEquations();  
    cout << "Oges: oges: extraEquationNumber= " << extraEquationNumber(0) << endl;
  }
}
