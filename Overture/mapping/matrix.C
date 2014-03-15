// *** These have not been tested *****

//=======================================================================================
//       matrixInverseScaled
// 
//   Compute ai <- a^(-1)*det
//======================================================================================
void matrixInverseScaled( const RealArray & a, RealArray & ai, real & det )
{
  int base= a.getBase(axis1);   
  int bound=a.getBound(axis1);
  int aBase2= a.getBase(axis2);   int aBound2=a.getBound(axis2);
  
  int dim = bound-base+1;
  if( dim != a.getBound(axis2)-a.getBase(axis2)+1 )
  {
    cerr << "matrixInverseScaled: matrix is not square!" << endl;
    exit(1);
  }
  
  int i1=base;
  int i2=i1+1;
  int i3=i2+1;

  switch (dim)
  {
  case 1:
    det=a(i1,i1);    
    ai(i1,i1)=1.;
    break;
  case 2:
    det=a(i1,i1)*a(i2,i2)-a(i1,i2)*a(i2,i1);
    ai(i1,i1)= a(i2,i2);
    ai(i1,i2)=-a(i1,i2);
    ai(i2,i1)=-a(i2,i1);
    ai(i2,i2)= a(i1,i1);
    break;
  case 3:
    det=(a(i1,i2)*a(i2,i3)-a(i2,i2)*a(i1,i3))*a(i3,i1)
       +(a(i1,i3)*a(i2,i1)-a(i2,i3)*a(i1,i1))*a(i3,i2)
	 +(a(i1,i1)*a(i2,i2)-a(i2,i1)*a(i1,i2))*a(i3,i3);
    ai(i1,i1)=(a(i2,i2)*a(i3,i3)-a(i2,i3)*a(i3,i2));
    ai(i2,i1)=(a(i2,i3)*a(i3,i1)-a(i2,i1)*a(i3,i3));
    ai(i3,i1)=(a(i2,i1)*a(i3,i2)-a(i2,i2)*a(i3,i1));
    ai(i1,i2)=(a(i3,i2)*a(i1,i3)-a(i3,i3)*a(i1,i2));
    ai(i2,i2)=(a(i3,i3)*a(i1,i1)-a(i3,i1)*a(i1,i3));
    ai(i3,i2)=(a(i3,i1)*a(i1,i2)-a(i3,i2)*a(i1,i1));
    ai(i1,i3)=(a(i1,i2)*a(i2,i3)-a(i1,i3)*a(i2,i2));
    ai(i2,i3)=(a(i1,i3)*a(i2,i1)-a(i1,i1)*a(i2,i3));
    ai(i3,i3)=(a(i1,i1)*a(i2,i2)-a(i1,i2)*a(i2,i1));
    break;
  default:
    cerr << "matrixInverseScaled: Error - matrix dimension not equal to 1,2, or 3" << endl;
  }
  
}

//=======================================================================================
// Multiply two matrices (2D A++ arrays) together
//   c <- a*b
//   
//======================================================================================
void matrixMultiply( RealArray & a, RealArray & b, RealArray c )
{
  int aBase1= a.getBase(axis1);   int aBound1=a.getBound(axis1);
  int aBase2= a.getBase(axis2);   int aBound2=a.getBound(axis2);
  int bBase1= b.getBase(axis1);   int bBound1=b.getBound(axis1);
  int bBase2= b.getBase(axis2);   int bBound2=b.getBound(axis2);
  
  RealArray d;

  d.redim(aBound1-aBase1+1,bBound2-bBase2+1); 
  d.setBase(axis1,aBase1);  d.setBase(axis2,bBase1);
  

  if( aBound2-aBase2 != bBound1-bBase1 )
    cerr << "matrixMultiply: unable to multiply matrices! dimensions wrong" << endl;
  else
  {
    for( int i=aBase1; i<=aBound1; i++ )
      for( int j=bBase2; j<=bBound2; j++ )
      {
       real t=0;
       for( int k=aBase2; k<=aBound2; k++ )
         t=t+a(i,k)*b(k,j);
       d(i,j)=t;
     }
  }
  
  c.redim(0);
  c=d;
  
}

//======================================================================================
//     leastSquaresNewton
//
//
//=====================================================================================
void ExactLocalInverse::leastSquaresNewton( RealArray & dr, RealArray & dy, RealArray & yr, 
  RealArray & ry, int & ok  )
{
  ok=TRUE;

  Index xAxes(axis1,rangeDimension);
  RealArray l2norm(domainDimension);
  
  // Compute the l2 norms of the columns of yr
  for( int j=axis1; j<domainDimension; j++)
    l2norm(j)=sum(evaluate(yr(xAxes,j)*yr(xAxes,j)));

  real l2max=max(l2norm);
  real l2min=min(l2norm);
  real eps = 1.e-5;       // ***** fix ****
  real det;

  if( l2max==0. )
    ok=FALSE;  // rank=0
  else
  {
//    IntegerArray InZ = (l2norm > eps*l2max).indexMap(); // nonZerocolumns
//    RealArray a;
//    matrixMultiply( yr(xAxes,InZ),yr(xAxes,InZ),a );
    RealArray a(rangeDimension,rangeDimension);
    int i=axis1;
    for( j=axis1; j<domainDimension; j++)
    {
      if( l2norm(j) > eps*l2max )
        a(Axes,++i)=yr(Axes,i);
    }
    a.reshape(rangeDimension,i);
    RealArray b = transpose(a);
    matrixMultiply( b,a,a );      // form normal equations a <- a^T a
      
    matrixInverseScaled( a,ry,det );  // ry <- a^(-1)*det
    if( fabs(det)< eps*l2max )
      ok=FALSE;
    else
    {
      matrixMultiply( b,dr,dy );    // dy = a^T dr
      matrixMultiply( ry,dy,dy );   // dy = (a^Ta)^(-1) (a^t dr)      dy=dy/det;
    }
  }

}


