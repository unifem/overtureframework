#include "GridCollection.h"
#include "GridCollectionFunction.h"
#include "SparseRep.h"
#include "conversion.h"


#define COEFF(i,n,I1,I2,I3) \
  coeffG(i+stencilDim*(n),I1,I2,I3)

#define EQUATIONNUMBER(i,n,I1,I2,I3) \
  equationNumberX(i+stencilDim*(n),I1,I2,I3)

#define ForBoundary(side,axis)   for( axis=0; axis<numberOfDimensions; axis++ ) \
                                 for( side=0; side<=1; side++ )





int 
residual(const RealGridCollectionFunction & coeff,
         const RealGridCollectionFunction & u,
         const RealGridCollectionFunction & f,
         RealGridCollectionFunction & r )
{
//=========================================================================================================
// /Description:
//    Compute the residual,  r= f - coeff*u
// \end{description}
//=========================================================================================================


  GridCollection & cg = *coeff.getGridCollection();

  if( cg.numberOfGrids()<=0 )
    return 0;
  
  assert( coeff[0].sparse!=NULL );
  
  if( !coeff.getIsACoefficientMatrix() )
  {
    printf("residual:ERROR: coeff is not a coefficient matrix!\n");
    return 1;
  }

  const int stencilLength = coeff[0].sparse->stencilSize;
  const int numberOfComponents = coeff[0].sparse->numberOfComponents;
  const int stencilDim = stencilLength*numberOfComponents;

  const int numberOfGrids = cg.numberOfGrids();

  int grid;
  IntegerArray arraySize(numberOfGrids,3), arrayDims(numberOfGrids,3);
  arraySize=1;
  arrayDims=0;
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      arraySize(grid,axis)=cg[grid].dimension(End,axis)-cg[grid].dimension(Start,axis)+1;
      arrayDims(grid,axis)=cg[grid].dimension(Start,axis);
    }
  }

  IntegerArray gridEquationBase(numberOfGrids+1);
  gridEquationBase.redim(numberOfGrids+1); 
  gridEquationBase(0)=0;
  for( grid=1; grid<=numberOfGrids; grid++ )
    gridEquationBase(grid)=gridEquationBase(grid-1)+
      numberOfComponents*((cg[grid-1].dimension(End,axis1)-cg[grid-1].dimension(Start,axis1)+1)*
			  (cg[grid-1].dimension(End,axis2)-cg[grid-1].dimension(Start,axis2)+1)*
			  (cg[grid-1].dimension(End,axis3)-cg[grid-1].dimension(Start,axis3)+1));


  r=f;
  
  for( grid=0; grid<numberOfGrids; grid++ )
  {
    realArray & resid = r[grid];
    
    const IntegerDistributedArray & equationNumber = coeff[grid].sparse->equationNumber;
    const IntegerDistributedArray & classify = coeff[grid].sparse->classify;

    const int ndra=cg[grid].dimension(Start,axis1), ndrb=cg[grid].dimension(End,axis1);
    const int ndsa=cg[grid].dimension(Start,axis2), ndsb=cg[grid].dimension(End,axis2);
    const int ndta=cg[grid].dimension(Start,axis3), ndtb=cg[grid].dimension(End,axis3);

    const realArray & coeffG = coeff[grid];
    
    
    // ------- general case ------
    int i1,i2,i3,n;
    for( i3=ndta; i3<=ndtb; i3++ )
    {
      for( i2=ndsa; i2<=ndsb; i2++ )
      {
        for( i1=ndra; i1<=ndrb; i1++ )
        {
          // printf(" i1,i2=%i,%i \n",i1,i2);
          for( n=0; n<numberOfComponents; n++)
	  {
            if( classify(i1,i2,i3,n)==SparseRepForMGF::unused )
	    {  // null equation
              resid(i1,i2,i3,n)=0.;
	    }
            else
	    {
	      for( int i=0; i<stencilDim; i++)
	      {
		int eqn=equationNumber(i+stencilDim*(n),i1,i2,i3);

		int gj=numberOfGrids-1;
		for( int grid1=1; grid1<numberOfGrids; grid1++ )
		{
		  if( eqn <= gridEquationBase(grid1) )
		  {
		    gj=grid1-1;
		    break;
		  }
		}
		int nj, j1, j2,j3;

		// coeff[gj].sparse->equationToIndex( eqn, nj, j1, j2,j3 );
		eqn-=(gridEquationBase(gj)+1);
		nj= (eqn % numberOfComponents);
		eqn/=numberOfComponents;
		j1=(eqn % arraySize(gj,axis1))+arrayDims(gj,axis1);
		eqn/=arraySize(gj,axis1);
		j2=(eqn % arraySize(gj,axis2))+arrayDims(gj,axis2);
		eqn/=arraySize(gj,axis2);
		j3=(eqn % arraySize(gj,axis3))+arrayDims(gj,axis3);

                // printf("    i=%i coeff=%5.2f (%i,%i,g=%i) \n",i,COEFF(i,n,i1,i2,i3),j1,j2,gj);
		
		// on ghost lines (i,n,i1,i2,i3) may reach into nonexistant points so skip this problem by skipping zero coeffs.
		if ( fabs(COEFF(i,n,i1,i2,i3))>0. ) resid(i1,i2,i3,n)-=COEFF(i,n,i1,i2,i3)*u[gj](j1,j2,j3,nj);

	      }
	    }
	    
	  }
	}
      }
    }
  }
  return 0;
}
