#include "Oges.h"
#include "OGTrigFunction.h"


#define  ForExtendedIndex( indexArray,extend,i1,i2,i3 ) \
  for( i3 =(numberOfDimensions<3 ? indexArray(Start,axis3) : indexArray(Start,axis3)-extend); \
       i3<=(numberOfDimensions<3 ? indexArray(End  ,axis3) : indexArray(End  ,axis3)+extend); \
       i3++ ) \
  for( i2 =(numberOfDimensions<2 ? indexArray(Start,axis2) : indexArray(Start,axis2)-extend); \
       i2<=(numberOfDimensions<2 ? indexArray(End  ,axis2) : indexArray(End  ,axis2)+extend); \
       i2++ ) \
  for( i1=indexArray(Start,axis1)-extend; i1<=indexArray(End,axis1)+extend; i1++ )



//\begin{>OgesInclude.tex}{\subsection{determineErrors}} 
void Oges:: 
determineErrors( realCompositeGridFunction & u, OGFunction & exactSolution, int & printOptions )
//=====================================================================================
// /Purpose: Determine errors in the grid function {\ff u} assuming that it is
// supposed to equal the twilightzone function given by {\ff exactSolution}.
// /printOptions (input): A bitflag that cna be used to determine some output,
// \begin{verbatim}
//      printOptions & 1  == TRUE : Print summary of maximum errors 
//      printOptions & 8  == TRUE : Print errors at all points 
//      printOptions & 16 == TRUE : Print solution
// \end{verbatim}
// /Errors:  Some...
// /Return Values: none.
//\end{OgesInclude.tex}
//==============================================================
{
  Range R(u.getComponentBase(0),u.getComponentBound(0));
  
  IntegerArray indexOfMaximumError(3,R,numberOfGrids);
  RealArray maximumError(R,numberOfGrids);

  OGFunction & e = exactSolution;
  

  FILE *errorFile = fopen("oges2.out","w" );           // for fprintf
  if( !errorFile )
  {
    cerr << "determineErrors: error opening the errorFile! " << endl;
    exit (1);
  }
  
  RealArray xv(3); 
  real errij;

  int i1,i2,i3,axis,grid,n;

  int numberOfGhostLines=1; // ****wdh***

  for( grid=0; grid<numberOfGrids; grid++ )
  {
    MappedGrid & c = cg[grid];
    if( printOptions & 8 )
      fprintf(errorFile,"=======determineErrors======== \n"
              "  grid   i1   i2    i3  mask   n        u         ue       err \n" );

    xv(axis3)=0.;
    for( n=u[grid].getBase(axis3+1); n<=u[grid].getBound(axis3+1); n++ )
    {
      maximumError(n,grid)=0.; 
      ForExtendedIndex( c.gridIndexRange(),numberOfGhostLines,i1,i2,i3 )
      {
        if( cg[grid].mask()(i1,i2,i3) != 0 )
	{
	  for( axis=axis1; axis<numberOfDimensions; axis++ )
	  {
            xv(axis)=c.isCellCentered()(axis)? c.center()(i1,i2,i3,axis):
	                                     c.vertex()(i1,i2,i3,axis);
	  }
          errij=u[grid](i1,i2,i3,n)-e(xv(axis1),xv(axis2),xv(axis3),n);
          if( printOptions & 8 )
            fprintf(errorFile,
                    "  %4i %5i %5i %5i %4i %3i %10.3e %10.3e %8.2e x=%8.2e y=%8.2e z=%8.2e\n",
                    grid,i1,i2,i3,cg[grid].mask()(i1,i2,i3),n,u[grid](i1,i2,i3,n),
                    e(xv(axis1),xv(axis2),xv(axis3),n),errij, xv(axis1),xv(axis2),xv(axis3));
          if( fabs(errij) > maximumError(n,grid) )
	  {
            maximumError(n,grid)=fabs(errij);
            indexOfMaximumError(0,n,grid)=i1;
            indexOfMaximumError(1,n,grid)=i2;
            indexOfMaximumError(2,n,grid)=i3;
	  }
	}
      }
    }
    
    if( printOptions & 16 )
    {
      //  ...print the solution
      fprintf(errorFile,"========Solution on component grid =%i========\n",grid);
      if( numberOfComponents > 1 )
        fprintf(errorFile,"--------component n =%i--------\n",n);

      for( int n=u[grid].getBase(axis3+1); n<=u[grid].getBound(axis3+1); n++ )
      {
        for( int i3=c.dimension()(Start,axis3); i3<=c.dimension()(End,axis3); i3++ )
        {
          if( numberOfDimensions == 3 )
            fprintf(errorFile,"+++++++i3 = %i++++++++\n",i3);
          for( int i2=c.dimension()(Start,axis2); i2<=c.dimension()(End,axis2); i2++ )
          {
            for( int i1=c.dimension()(Start,axis1); i1<=c.dimension()(End,axis1); i1++ )
            {
              fprintf(errorFile," %9.2e",u[grid](i1,i2,i3,n));
	    }
	    fprintf(errorFile,"\n");
	  }
	}
      }
    }
  }

  if( printOptions & 1 )
  {
    // ...Print errors
    for( int i=0; i<numberOfExtraEquations; i++ )
    {
      int i1e,i2e,i3e,gride;
      n=0;
      equationToIndex( extraEquationNumber(i),n,i1e,i2e,i3e,gride );
      printf("Solution to extra equation number %i is u[%i](%i,%i,%i,%i)=%e\n",
         i,gride,i1e,i2e,i3e,gride,u[gride](i1e,i2e,i3e,n));
      fprintf(errorFile,"Solution to extra equation number %i is u[%i](%i,%i,%i,%i)=%e\n",
         i,gride,i1e,i2e,i3e,gride,u[gride](i1e,i2e,i3e,n));
    }

    cout << "  n  Grid  Maximum error    i1     i2    i3" << endl;
    for( n=0; n < numberOfComponents; n++)
      for( grid=0; grid < numberOfGrids; grid++ )
        printf(" %2d   %3d  %6e  %5d %5d %5d\n",n,grid,maximumError(n,grid),
               indexOfMaximumError(0,n,grid),
               indexOfMaximumError(1,n,grid),
               indexOfMaximumError(2,n,grid));
  }
  
  fclose(errorFile);
  

}

