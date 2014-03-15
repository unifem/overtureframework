#include "Oges.h"
#include "SparseRep.h"

void Oges::
findExtraEquations()
{
  //====================================================================
  //  
  //   Purpose:
  //     Determine locations for extra equations, such as the
  //  constraint equation in the Laplace-Neumann problem or
  //  the eigenvalue in an eigen-problem
  //
  //
  //   Method
  //     Find unused points which can be used for the extra equations
  //  
  //   Output
  //     Assign : extraEquationNumber(i) i=0,...,numberOfExtraEquations-1
  //====================================================================

  if( numberOfExtraEquations <= 0 )
    return;

  extraEquationNumber.redim(numberOfExtraEquations);

  int i1,i2,i3;

  const int startingExtraEquationClassifyValue=10;
  
  int i=0;
  for( int grid=numberOfGrids-1; grid>=0; grid-- )
  {
    MappedGrid & c = cg[grid];
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;
    for( i3 =c.dimension(End  ,axis3);
         i3>=c.dimension(Start,axis3) && i<numberOfExtraEquations; i3-- )
    {
      for( i2 =c.dimension(End,  axis2);
           i2>=c.dimension(Start,axis2) && i<numberOfExtraEquations; i2-- )
      {
        for( i1 =c.dimension(End  ,axis1);
  	     i1>=c.dimension(Start,axis1) && i<numberOfExtraEquations; i1-- )
        {
          // classify already may have an extra eqn in it if, for example, initialize
          // was called twice
  	  if( (classifyX(i1,i2,i3)==SparseRepForMGF::unused 
                   || classifyX(i1,i2,i3) >= startingExtraEquationClassifyValue) && i<numberOfExtraEquations )
	  {
	    // ...This point is not used yet, use it for an extra equation
            extraEquationNumber(i)=equationNo(0,i1,i2,i3,grid);
            
            printf("----ogce: Found extra equation %i: grid=%i (i1,i2,i3)=(%i,%i,%i) classifyX(i1,i2,i3)=%i\n",
                     i,grid,i1,i2,i3,classifyX(i1,i2,i3));
	    
            // classifyX.display("classifyX");
            classifyX(i1,i2,i3)=startingExtraEquationClassifyValue+i;
            i++;
	  }
	}
      }
    }
  }
  if( i < numberOfExtraEquations )
  {
    cerr << "Oges:findExtraEquations:ERROR unable to find locations for extra equations" << endl;
    cerr << "  This application is requesting numberOfExtraEquations ="
         << numberOfExtraEquations << endl;
    cerr << "  Extra equations are placed at unused points on the grid " << endl;
    cerr << "  You could add an extra ghostline to one of the grids " << endl;
    exit(1);
  }    
}

//================================================================
//  
//   Purpose:
//     Assign the right null vector
//  
//   Input
//  
//   Output
//================================================================
void Oges::
makeRightNullVector()
{
  Index I1,I2,I3;
  for( int grid=numberOfGrids-1; grid>=0; grid-- )
  {
    MappedGrid & c = cg[grid];
    IntegerDistributedArray & classifyX = coeff[grid].sparse->classify;

    I1=Range(c.dimension()(Start,axis1),c.dimension()(End,axis1));
    I2=Range(c.dimension()(Start,axis2),c.dimension()(End,axis2));
    I3=Range(c.dimension()(Start,axis3),c.dimension()(End,axis3));
    rightNullVector[grid]=0.;
    // do not include the ghost line so that we retain u.n=0 as a BC!
    where( classifyX(I1,I2,I3)==SparseRepForMGF::interior || classifyX(I1,I2,I3)==SparseRepForMGF::boundary )
      rightNullVector[grid](I1,I2,I3)=parameters.nullVectorScaling;
  }
}

