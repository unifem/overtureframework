//======================================================================================
//  Solve the underdetermined least Squares problem
//    This is needed at the singularity of a coordinate system (centre of a polar
//    coordinate system) when on variable is undetermined
//======================================================================================
void ExactLocalInverse::underdetermined( RealArray & xt, RealArray & tx, int & ok )
{
  RealArray xtk(rangeDimension);
  RealArray ata(rangeDimension,rangeDimension);
  RealArray atai(rangeDimension,rangeDimension);

  IntegerArray ip(rangeDimension);
  int i,j,k;
  real det,odet;
  
  const real eps = 1.e-5;   // ******

  ok=TRUE;

  // Compute column norms of xt.
  for( j=axis1; j<rangeDimension; j++ )
  {
    xtk(j)=0.;
    for( i=axis1; i<rangeDimension; i++ )
    {
      xtk(j)=xtk(j)+xt(i,j)*xt(i,j);
      tx(i,j)=0.;
    }
    if( j==1 )
    {
      xtk1=xtk(j);
      xtk2=xtk(j);
    }
    else
    {
      xtk1=min(xtk1,xtk(j));
      xtk2=max(xtk2,xtk(j));
    }
  }

  if( xtk2==0. )
  {
    //  Rank=0.
    ok=FALSE;
  }
  else if( xtk1 < eps*xtk2 )
  {
    //  0<Rank<n.
    //  Find the non-zero columns of xt.
    irank=0; 
    for( k=axis1; k<rangeDimension; k++ ) 
    {
      if( xtk(k) > eps*xtk2 )
      {
        ip(++irank)=k;
      }
    }
    //  Skip over the zero columns of xt and
    //  form the normal matrix A = xt' * xt.
    for( k=axis1; k<irank; k++ )  
    {
      for( j=axis1; j<irank; j++ )
      {
        ata(j,k)=0.;
        for( i=axis1; i<rangeDimension; i++ )
	{
          ata(j,k)=ata(j,k)+xt(i,ip(j))*xt(i,ip(k));
	}
      }
    }
    // Invert the normal matrix A using Cramer's rule.
    if( irank==1 )
    {
      atai(1,1)=1./ata(1,1);
    }
    else
    {
      // Rank=2
      det=ata(1,1)*ata(2,2)-ata(1,2)*ata(2,1);
      if( fabs(det) < eps*xtk2 )
        ok=FALSE;   //       return
      else
      {
        odet=1./det;
        atai(1,1)= ata(2,2)*odet;
        atai(1,2)=-ata(1,2)*odet;
        atai(2,1)=-ata(2,1)*odet;
        atai(2,2)= ata(1,1)*odet;
      }
    }
    //  Solve the normal equations A * tx = xt:  tx = A \ xt.
    if( ok )
    {
      for( k=axis1; k<rangeDimension; k++ )
     {
       for( j=axis1; j<irank; j++ ) 
       {
         ij=ip(j);
         tx(ij,k)=0.;
         for( i=axis1; i<irank; i++ )
 	 {
            tx(ij,k)=tx(ij,k)+atai(j,i)*xt(k,ip(i));
  	 }
       }
     }
    }
  }
  else
  {
    // Rank=n.
    // This should never be needed, but here it is for completeness.
    //  Solve using Cramer's rule.
    if( n==1 )
    {
      tx(1,1)=1./xt(1,1);
    }
    else if( n==2 )
    {
      det=xt(1,1)*xt(2,2)-xt(2,1)*xt(1,2);
      if( fabs(det) < eps*xtk2 )
        ok=FALSE;  // return
      else
      {
        odet=1./det;
        tx(1,1)= xt(2,2)*odet;
        tx(1,2)=-xt(1,2)*odet;
        tx(2,1)=-xt(2,1)*odet;
        tx(2,2)= xt(1,1)*odet;
      }
    }
    else
    {
      //  n=3.
      det=(xt(1,2)*xt(2,3)-xt(2,2)*xt(1,3))*xt(3,1)
         +(xt(1,3)*xt(2,1)-xt(2,3)*xt(1,1))*xt(3,2)
         +(xt(1,1)*xt(2,2)-xt(2,1)*xt(1,2))*xt(3,3);
      if( fabs(det) < eps*xtk2**1.5 )
	ok=FALSE;     // return 
      else
      {
        odet=1./det;
        tx(1,1)=(xt(2,2)*xt(3,3)-xt(2,3)*xt(3,2))*odet;
        tx(2,1)=(xt(2,3)*xt(3,1)-xt(2,1)*xt(3,3))*odet;
        tx(3,1)=(xt(2,1)*xt(3,2)-xt(2,2)*xt(3,1))*odet;
        tx(1,2)=(xt(3,2)*xt(1,3)-xt(3,3)*xt(1,2))*odet;
        tx(2,2)=(xt(3,3)*xt(1,1)-xt(3,1)*xt(1,3))*odet;
        tx(3,2)=(xt(3,1)*xt(1,2)-xt(3,2)*xt(1,1))*odet;
        tx(1,3)=(xt(1,2)*xt(2,3)-xt(1,3)*xt(2,2))*odet;
        tx(2,3)=(xt(1,3)*xt(2,1)-xt(1,1)*xt(2,3))*odet;
        tx(3,3)=(xt(1,1)*xt(2,2)-xt(1,2)*xt(2,1))*odet;
      }
    }
  }
}
