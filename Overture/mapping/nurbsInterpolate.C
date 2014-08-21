// define BOUNDS_CHECK
#include "NurbsMapping.h"
#include "display.h"
#include "ParallelUtility.h"

#define DGECO EXTERN_C_NAME(dgeco)
#define SGECO EXTERN_C_NAME(sgeco)
#define DGESL EXTERN_C_NAME(dgesl)
#define SGESL EXTERN_C_NAME(sgesl)

#define DGBTRF EXTERN_C_NAME(dgbtrf)
#define SGBTRF EXTERN_C_NAME(sgbtrf)

#define DGBTRS EXTERN_C_NAME(dgbtrs)
#define SGBTRS EXTERN_C_NAME(sgbtrs)

#ifdef OV_USE_DOUBLE
  #define GECO DGECO
  #define GESL DGESL
  #define GBTRF DGBTRF
  #define GBTRS DGBTRS
#else
  #define GECO SGECO
  #define GESL SGESL
  #define GBTRF SGBTRF
  #define GBTRS SGBTRS
#endif

extern "C"
{
  void SGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void DGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void SGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);

  void DGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);
  
  void SGBTRF( const int & M, const int & N, const int & KL, const int & KU, real & AB, 
               const int & LDAB, int & IPIV, int & INFO );

  void DGBTRF( const int & M, const int & N, const int & KL, const int & KU, real & AB, 
               const int & LDAB, int & IPIV, int & INFO );

  void SGBTRS( const char & TRANS, const int &N, const int &KL, const int &KU, const int &NRHS, real& AB, const int &LDAB, int& IPIV, real& B, const int &LDB, int& INFO );

  void DGBTRS( const char & TRANS, const int &N, const int &KL, const int &KU, const int &NRHS, real& AB, const int &LDAB, int& IPIV, real& B, const int &LDB, int& INFO );

}

// =================================================================================================
/// \brief Interpolate an array of points -- specify which points to utilize using the xDimension 
///     and xGridIndexRange arrays
///
/// \param x(I1,I2,I3,0:r-1)  (input) array of points to interpolate (including ghost points)
/// \param domainDimension (input) : 
/// \param rangeDimension (input) : 
/// \param xDimension(2,3) (input) : range of points to interpolate
/// \param xGridIndexRange(2,3) (input) : index range of interior and boundary points
/// \param parameterizationType (input) :
/// \param xDegree[3] (input) : (optional) degree of Nurbs in each domain dimension
// =================================================================================================
void NurbsMapping::
interpolate(const RealArray & x, 
	    int domainDimension, int rangeDimension,
	    const IntegerArray & xDimension, const IntegerArray & xGridIndexRange, 
	    ParameterizationTypeEnum parameterizationType /* =parameterizeByChordLength */,
	    int *xDegree /* =NULL */ )
{

  // -- This version can handle any domainDimension and any rangeDimension
  bool useBandedSolve=true;

  setDomainDimension(domainDimension);
  setRangeDimension(rangeDimension);

  if( domainDimension<=0 || domainDimension>3 )
  {
    printF("NurbsMapping::interpolate:ERROR: the domainDimension=%i is INVALID\n",domainDimension);
    OV_ABORT("error");
  }
  if( rangeDimension<=0 || rangeDimension>3 )
  {
    printF("NurbsMapping::interpolate:ERROR: the rangeDimension=%i is INVALID\n",rangeDimension);
    OV_ABORT("error");
  }
  
  // ::display(xGridIndexRange,"xGridIndexRange");

  // -- the Nurbs is constructed to include ghost points --
  int nv[3];
  for( int axis=0; axis<3; axis++ )
  {
    nv[axis]=xDimension(1,axis)-xDimension(0,axis);  // total number of points minus one
    if( axis<domainDimension )
    {
      setGridDimensions(axis,xGridIndexRange(1,axis)-xGridIndexRange(0,axis)+1); // actual number of points 
    }
  }
  n1=nv[0], n2=nv[1], n3=nv[2];
  
  int degree[3]={3,3,3};  // 
  if( xDegree!=NULL )
  {
    for( int axis=0; axis<domainDimension; axis++ )
      degree[axis]=xDegree[axis];
  }
  

  p1=min(degree[0],n1);   // must decrease p1 if there are only a few points
  m1=n1+p1+1;
  p2=min(degree[1],n2);   // must decrease p2 if there are only a few points
  m2=n2+p2+1;
  p3=min(degree[2],n3);   // must decrease p3 if there are only a few points
  m3=n3+p3+1;

  int xBase0=xDimension(0,0); // x.getBase(0);
  int xBase1=xDimension(0,1); //x.getBase(1);
  int xBase2=xDimension(0,2); //x.getBase(2);

// construct knot vectors
  Range R(0,rangeDimension-1);
  ArraySimple<real> uBar(n1+1), vBar(n2+1), wBar(n3+1);

  // define uBar by chord-length and curvature
  int i, j, k;
  real chord;
  if( parameterizationType==parameterizeByIndex )
  {
    // use user supplied parameterization
    for (i=0; i<n1; i++)
      uBar(i)=real(i)/n1;
    uBar(n1)=1.;
    for (i=0; i<n2; i++)
      vBar(i) = real(i)/n2;
    vBar(n2)=1.;
    for (i=0; i<n3; i++)
      wBar(i) = real(i)/n3;
    wBar(n3)=1.;
  }
  else
  {
    // parameterize by arclength
    for (i=0; i<=n1; i++)
      uBar(i) = 0.;
    for (i=0; i<=n2; i++)
      vBar(i) = 0.;
    for (i=0; i<=n3; i++)
      wBar(i) = 0.;
  
    real total, d;
    ArraySimple<real> cds(max(n3,max(n1,n2))+1); // always base 0

    int num = (n2+1)*(n3+1); // number of non-degenerate rows
    for ( k=0; k<=n3; k++ )
      {
	for( j=0; j<=n2; j++ )
	  {
	    total = 0.;
	    for( i=1; i<=n1; i++ )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k+xBase2, R) - x(i-1+xBase0, j+xBase1, k+xBase2, R))) );
		if( chord==0. )
		  { // we need a non-zero chord length
		    if( i>1 )
		      chord=cds[i-1]-cds[i-2];  // use previous
		    else
		      {
			if( i<n1 )
			  {
			    chord=SQRT( sum(SQR( x(i+1+xBase0, j+xBase1, k+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
			    if( chord==0. )
			      chord=1.;
			  }
			else
			  chord=1.;
		      }
		  } // end if chord == 0
		cds[i] = chord;
		total += cds[i];
	      } // end for i=...

	    if (total == 0.) 
	      num -= 1; // this was a degenerate row
	    else
	      {
		d = 0.;
		for (i=1; i<n1; i++)
		  {
		    d += cds[i];
		    uBar(i) += d/total;
		  }
	      }
	  }
      }
    
    // end for j...
    if (num == 0)
    {
      printF("NurbsMapping::interpolate: ERROR -- all rows are degenerate! \n");
      OV_ABORT("error");
    }
    // normalize
    for (i=0; i<n1; i++)
      uBar(i) = uBar(i)/num;
    uBar(n1) = 1.;
    
    
    num = (n1+1)*(n3+1); // number of non-degenerate columns
    for ( k=0; k<=n3; k++ )
    {
      for( i=0; i<=n1; i++ )
      {
	total = 0.;
	for( j=1; j<=n2; j++ )
	{
	  chord=SQRT( sum(SQR( x(i+xBase0, j-1+xBase1, k+xBase2, R) - x(i+xBase0, j+xBase1, k+xBase2, R))) );
	  if( chord==0. )
	  { // we need a non-zero chord length
	    if( j>1 )
	      chord=cds[j-1]-cds[j-2];  // use previous
	    else
	    {
	      if( j<n2 )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j+1+xBase1, k+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
		if( chord==0. )
		  chord=1.;
	      }
	      else
		chord=1.;
	    }
	  } // end if chord == 0
	  cds[j] = chord;
	  total += cds[j];
	} // end for i=...
    
	if (total == 0.) 
	  num -= 1; // this was a degenerate row
	else
	{
	  d = 0.;
	  for (j=1; j<n2; j++)
	  {
	    d += cds[j];
	    vBar(j) += d/total;
	  }
	}
      }
    } // end for k...
    if (num == 0)
    {
      printF("NurbsMapping::interpolate:: ERROR -- all v-columns are degenerate! \n");
      OV_ABORT("error");
    }
    // normalize
    for (j=0; j<n2; j++)
      vBar(j) = vBar(j)/num;
    vBar(n2) = 1.;

    num = (n1+1)*(n2+1); // number of non-degenerate columns
    for ( int i=0; i<=n1; i++ )
    {
      for( j=0; j<=n2; j++ )
      {
	total = 0.;
	for( k=1; k<=n3; k++ )
	{
	  chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k-1+xBase2, R) - x(i+xBase0, j+xBase1, k+xBase2, R))) );
	  if( chord==0. )
	  { // we need a non-zero chord length
	    if( k>1 )
	      chord=cds[k-1]-cds[k-2];  // use previous
	    else
	    {
	      if( k<n3 )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k+1+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
		if( chord==0. )
		  chord=1.;
	      }
	      else
		chord=1.;
	    }
	  } // end if chord == 0
	  cds[k] = chord;
	  total += cds[k];
	} // end for k=...
    
	if (total == 0.) 
	  num -= 1; // this was a degenerate row
	else
	{
	  d = 0.;
	  for (k=1; k<n3; k++)
	  {
	    d += cds[k];
	    wBar(k) += d/total;
	  }
	}
      } // end for j...
    } // end for i...
    if (num == 0)
    {
      printF("NurbsMapping::interpolateVolume: ERROR -- all w-columns are degenerate! \n");
      OV_ABORT("error");
    }
    // normalize
    for (j=0; j<n3; j++)
      wBar(j) = wBar(j)/num;
    wBar(n3) = 1.;
    
  }
  

  // define the knots by averaging, this will ensure a positive definite matrix below
  uKnot.redim(m1+1);

  uKnot(Range(0,p1))=0.;
  uKnot(Range(m1-p1,m1))=1.;
  real uBarS;
  for( i=p1+1; i<m1-p1; i++ )
  {
      uBarS=0.;
      for (int q=i-p1; q<= i-1; q++)
        uBarS +=uBar(q);
    uKnot(i)=uBarS/p1;
  }
  
  // now v...
  vKnot.redim(m2+1);
  vKnot(Range(0,p2))=0.;
  vKnot(Range(m2-p2,m2))=1.;
  real vBarS;
  for( i=p2+1; i<m2-p2; i++ )
  {
    vBarS=0.;
    for (int q=i-p2; q<= i-1; q++)
      vBarS +=vBar(q);

    vKnot(i)=vBarS/p2;
  }

  // and w...
  wKnot.redim(m3+1);
  wKnot(Range(0,p3))=0.;
  wKnot(Range(m3-p3,m3))=1.;
  real wBarS;
  for( i=p3+1; i<m3-p3; i++ )
  {
    wBarS=0.;
    for (int q=i-p3; q<= i-1; q++)
      wBarS +=wBar(q);

    wKnot(i)=wBarS/p3;
  }
  
  // ----- compute control points ------

  const int n1p1=n1+1;
  const int n2p1=n2+1;
  const int n3p1=n3+1;

  // store the control polygon in cPoint
  cPoint.resize(n1p1, n2p1, n3p1, rangeDimension+1);
  Range Rn1(0,n1), Rn2(0,n2), Rn3(0,n3);
  cPoint(Rn1, Rn2, Rn3, rangeDimension)=1.;  // unit weights 

  uMin=0.;
  uMax=1.;
  vMin=0.;
  vMax=1.;
  wMin=0.;
  wMax=1.;

  // ------------------------
  // -- Setup the u-matrix --
  // ------------------------
  ArraySimple<real> work(max(n3,max(n1,n2))+1);
  ArraySimple<int> ipvt(max(n3,max(n1,n2))+1);
  ArraySimple<real> rPoint(n1+1, n2+1, n3+1, rangeDimension);
  real rcond;
  RealArray row(p1+1);
  int span;
  int job;
  int r;
  int info=0;

  if( useBandedSolve )
  {
    // ------------ BANDED SOLVE ------------

    int kl=p1-1, ku=p1-1;
    int ldab = 2*kl+ku+1;

    ArraySimple<real> matrix(ldab,n1p1);
    for (i=0; i<ldab; i++)
      for (j=0; j<=n1; j++)
	matrix(i,j)=0.;
    
    for( i=0; i<=n1; i++ )
    {
      span = findSpan(n1,p1,uBar(i),uKnot); 
      basisFuns(span,uBar(i),p1,uKnot,row);
      for( j=0; j<=p1; j++ )
      {
        int ii=i, jj=j+span-p1;
	// matrix(ii,jj)=row(j);
        // printF(" (ii,jj)=(%i,%i) row(%i)=%8.2e\n",ii,jj,j,row(j));
	int ik=kl+ku+ii-jj;
	if( row(j)!=0. ) // the first and last rows should have some zeros outside the band structure
	{
	  assert( ik>=0 && ik<ldab );
	  matrix(ik,jj)=row(j);
	}
	
      }
      
    }

    if( false )
    {
      RealArray uMatrix(ldab,n1p1);
      for (i=0; i<ldab; i++)
	for (j=0; j<=n1; j++)
	  uMatrix(i,j)=matrix(i,j);
      ::display(uMatrix,"Nurbs: interpolate: banded u-matrix","%6.3f ");
    }
    
    GBTRF( n1p1, n1p1, kl, ku, matrix(0,0), ldab, ipvt(0), info );
    if( info != 0 )
    {
      printF("NurbsMapping::interpolate:ERROR  u-matrix: banded factor: return: info=%i\n",info);
    }

    // Do (n2+1)*(n3+1) curve interpolations through the points 
    // x(0,j,k,r), x(1,j,k,r), ..., x(n1,j,k,r), j=0,1,...,n2, k=0,1,....,n3, r=0,1,2 denotes (x,y,z)
    job=0;
    for ( k=0; k<=n3; k++ )
      for( j=0; j<=n2; j++)
      {
	for( r=0; r<rangeDimension; r++ )
	{
	  for (i=0; i<=n1; i++)
	    rPoint(i,j,k,r)=x(i + xBase0, j + xBase1, k+xBase2, r);

	  if( false )
	  {
	    int nrhs=1;
	    GBTRS( 'N', n1p1, kl, ku, nrhs, matrix(0,0), ldab, ipvt(0), rPoint(0,j,k,r), n1p1,info );
	    /// GESL(matrix(0,0), n1p1, n1p1, ipvt(0), rPoint(0,j,k,r), job);
	    if( info != 0 )
	    {
	      printF("NurbsMapping::interpolate:ERROR  u-matrix: banded solve: return: info=%i\n",info);
	    }
	  }
	  
	}
      }
    
    // solve all systems in one call: 
    int nrhs=rangeDimension*n2p1*n3p1;
    GBTRS( 'N', n1p1, kl, ku, nrhs, matrix(0,0), ldab, ipvt(0), rPoint(0,0,0,0), n1p1,info );
    /// GESL(matrix(0,0), n1p1, n1p1, ipvt(0), rPoint(0,j,k,r), job);
    if( info != 0 )
    {
      printF("NurbsMapping::interpolate:ERROR  u-matrix: banded solve: return: info=%i\n",info);
    }

     //  SUBROUTINE DGBTRS( TRANS, N, KL, KU, NRHS, AB, LDAB, IPIV, B, LDB,
     // $                   INFO )

    // OV_ABORT("stop here for now");
    

  }
  else
  {
    // --- factor and solve using full matrix routines ---
    ArraySimple<real> matrix(n1+1,n1+1);
    RealArray row(p1+1);
    for (i=0; i<=n1; i++)
      for (j=0; j<=n1; j++)
	matrix(i,j)=0.;
    int span;
    for( i=0; i<=n1; i++ )
    {
      span = findSpan(n1,p1,uBar(i),uKnot); 
      basisFuns(span,uBar(i),p1,uKnot,row);
      for( j=0; j<=p1; j++ )
	matrix(i,j+span-p1)=row(j);
    }

    if( true )
    {
      RealArray uMatrix(n1p1,n1p1);
      for (i=0; i<=n1; i++)
	for (j=0; j<=n1; j++)
	  uMatrix(i,j)=matrix(i,j);
      ::display(uMatrix,"Nurbs: interpolate: u-matrix","%5.2f ");
    }
  
  
    // factor the matrix -- the semi-bandwidth is p1-1, so for p1=3 the matrix is 5 diagonal
    // AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
    GECO( matrix(0,0),n1p1,n1p1,ipvt(0),rcond,work(0) );  
    // matrix.display("Nurbs: interpolate: matrix after factor");
    if( rcond==0. )
    {
      printF("NurbsMapping::interpolate:ERROR: After factoring the u-matrix: rcond = %e \n",rcond);
      OV_ABORT("error");
    }
  
    // Do (n2+1)*(n3+1) curve interpolations through the points 
    // x(0,j,k,r), x(1,j,k,r), ..., x(n1,j,k,r), j=0,1,...,n2, k=0,1,....,n3, r=0,1,2 denotes (x,y,z)
    job=0;
    for ( k=0; k<=n3; k++ )
      for( j=0; j<=n2; j++)
      {
	for( r=0; r<rangeDimension; r++ )
	{
	  for (i=0; i<=n1; i++)
	    rPoint(i,j,k,r)=x(i + xBase0, j + xBase1, k+xBase2, r);
	  GESL(matrix(0,0), n1p1, n1p1, ipvt(0), rPoint(0,j,k,r), job);
	}
      }

  } // end non-banded solve

  if( domainDimension==1 )
  {
    for( r=0; r<rangeDimension; r++ )
      for ( k=0; k<=n3; k++ )
	for (j=0; j<=n2; j++)
	  for( i=0; i<=n1; i++)
	  {
	    cPoint(i,j,k,r)=rPoint(i,j,k,r);
	  }

    // ::display(cPoint,"interpolate: cPoint after vMatrix","%5.2f ");
  }
  else
  {
    // ------------------------
    // -- Setup the v-matrix --
    // ------------------------
    ArraySimple<real> pPoint(n2+1);
    if( useBandedSolve )
    {
      // ------------ V-MATRIX BANDED SOLVE ------------

      int kl=p2-1, ku=p2-1;
      int ldab = 2*kl+ku+1;

      ArraySimple<real> vmatrix(ldab,n2p1);
      row.redim(p2+1);
      vmatrix=0.;
      for( j=0; j<=n2; j++ )
      {
	span = findSpan(n2,p2,vBar(j),vKnot); 
	basisFuns(span,vBar(j),p2,vKnot,row);
	for( int q=0; q<=p2; q++ )
	{
	  // vmatrix(j,q+span-p2)=row(q);
	  int ii=j, jj=q+span-p2;
	  // matrix(ii,jj)=row(q);
	  int ik=kl+ku+ii-jj;
	  if( row(q)!=0. ) // the first and last rows should have some zeros outside the band structure
	  {
	    assert( ik>=0 && ik<ldab );
	    vmatrix(ik,jj)=row(q);
	  }
	}
      
      }

      //  matrix.display("Nurbs: interpolate: matrix");
  
      GBTRF( n2p1, n2p1, kl, ku, vmatrix(0,0), ldab, ipvt(0), info );
      if( info != 0 )
      {
	printF("NurbsMapping::interpolate:ERROR  v-matrix: banded factor: return: info=%i\n",info);
      }

      // Secondly, do n1+1 curve interpolations through the points
      // rPoint(i,0,r), rPoint(i,1,r), ..., rPoint(i,n2,r), i=0,1,...,n1, r=0,1,2.

      if( false )
      { // solve all systems in one call  -- doesn't make much difference in CPU
        ArraySimple<real> qPoint(n2+1, n1+1, n3+1, rangeDimension);
	for( r=0; r<rangeDimension; r++ )
	  for ( k=0; k<=n3; k++ )
	    for( i=0; i<=n1; i++)
	      for (j=0; j<=n2; j++)
		qPoint(j,i,k,r) = rPoint(i,j,k,r);

	int nrhs=n1p1*n3p1*rangeDimension;
	GBTRS( 'N', n2p1, kl, ku, nrhs, vmatrix(0,0), ldab, ipvt(0), qPoint(0,0,0,0), n2p1,info );
	if( info != 0 )
	{
	  printF("NurbsMapping::interpolate:ERROR  v-matrix: banded solve: return: info=%i\n",info);
	}

	// copy the result into the control point array
	for( r=0; r<rangeDimension; r++ )
	  for ( k=0; k<=n3; k++ )
	    for( i=0; i<=n1; i++)
	      for (j=0; j<=n2; j++)
		rPoint(i,j,k,r) = qPoint(j,i,k,r);

      }
      else
      {
	for ( k=0; k<=n3; k++ )
	  for( i=0; i<=n1; i++)
	  {
	    for( r=0; r<rangeDimension; r++ )
	    {
	      for (j=0; j<=n2; j++)
		pPoint(j) = rPoint(i,j,k,r);

	      // GESL( vmatrix(0,0),n2p1,n2p1,ipvt(0),pPoint(0),job);

	      int nrhs=1;
	      GBTRS( 'N', n2p1, kl, ku, nrhs, vmatrix(0,0), ldab, ipvt(0), pPoint(0), n2p1,info );
	      if( info != 0 )
	      {
		printF("NurbsMapping::interpolate:ERROR  v-matrix: banded solve: return: info=%i\n",info);
	      }

	      // copy the result into the control point array
	      for (j=0; j<=n2; j++)
		rPoint(i,j,k,r) = pPoint(j);
	    }
	  }
      }
      
    }
    else
    {
      // ------- V-MATRIX - FULL MATRIX SOLVE -----
      ArraySimple<real> vmatrix(n2+1,n2+1);
      row.redim(p2+1);
      vmatrix=0.;
      for( j=0; j<=n2; j++ )
      {
	span = findSpan(n2,p2,vBar(j),vKnot); 
	basisFuns(span,vBar(j),p2,vKnot,row);
	for( int q=0; q<=p2; q++ )
	  vmatrix(j,q+span-p2)=row(q);
      }

      //  matrix.display("Nurbs: interpolate: matrix");
  
      // factor the matrix -- the semi-bandwidth is p2-1, so for p2=3 the matrix is 5 diagonal
      // AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
      GECO( vmatrix(0,0),n2p1,n2p1,ipvt(0),rcond,work(0) );  
      // matrix.display("Nurbs: interpolate: matrix after factor");
      if( rcond==0. )
      {
	printF("NurbsMapping::interpolate:ERROR: After factoring the v-matrix: rcond = %e \n",rcond);
	OV_ABORT("error");
      }

      // Secondly, do n1+1 curve interpolations through the points
      // rPoint(i,0,r), rPoint(i,1,r), ..., rPoint(i,n2,r), i=0,1,...,n1, r=0,1,2.

      job=0;
      for ( k=0; k<=n3; k++ )
	for( i=0; i<=n1; i++)
	{
	  for( r=0; r<rangeDimension; r++ )
	  {
	    for (j=0; j<=n2; j++)
	      pPoint(j) = rPoint(i,j,k,r);
	    GESL( vmatrix(0,0),n2p1,n2p1,ipvt(0),pPoint(0),job);
	    // copy the result into the control point array
	    for (j=0; j<=n2; j++)
	      rPoint(i,j,k,r) = pPoint(j);
	  }
	}

    } // end full matrix solve 
  


    if( domainDimension==2 )
    {
      for( r=0; r<rangeDimension; r++ )
	for ( k=0; k<=n3; k++ )
	  for (j=0; j<=n2; j++)
	    for( i=0; i<=n1; i++)
	    {
	      cPoint(i,j,k,r)=rPoint(i,j,k,r);
	    }

      // ::display(cPoint,"interpolate: cPoint after vMatrix","%5.2f ");
    }
    else
    {

      // ------------------------
      // -- Setup the w-matrix --
      // ------------------------
      if( useBandedSolve )
      {
	// ------------ W-MATRIX BANDED SOLVE ------------

	int kl=p3-1, ku=p3-1;
	int ldab = 2*kl+ku+1;
	ArraySimple<real> wmatrix(ldab,n3p1);
	row.redim(p3+1);
	wmatrix=0.;
	for( k=0; k<=n3; k++ )
	{
	  span = findSpan(n3,p3,wBar(k),wKnot); 
	  basisFuns(span,wBar(k),p3,wKnot,row);
	  for( int q=0; q<=p3; q++ )
	  {
	  
	    // wmatrix(k,q+span-p3)=row(q);
	    int ii=k, jj=q+span-p3;
	    // matrix(ii,jj)=row(q);
	    if( row(q)!=0. ) // the first and last rows should have some zeros outside the band structure
	    {
	      int ik=kl+ku+ii-jj;
	      assert( ik>=0 && ik<ldab );
	      wmatrix(ik,jj)=row(q);
	    }
	  }
	}

	//  matrix.display("Nurbs: interpolate: matrix");
  
	GBTRF( n3p1, n3p1, kl, ku, wmatrix(0,0), ldab, ipvt(0), info );
	if( info != 0 )
	{
	  printF("NurbsMapping::interpolate:ERROR  w-matrix: banded factor: return: info=%i\n",info);
	}

	pPoint.resize(n3+1);
	for( i=0; i<=n1; i++)
	  for ( j=0; j<=n2; j++ )
	  {
	    for( r=0; r<rangeDimension; r++ )
	    {
	      for (k=0; k<=n3; k++)
		pPoint(k) = rPoint(i,j,k,r);

	      // GESL( wmatrix(0,0),n3p1,n3p1,ipvt(0),pPoint(0),job);
	      int nrhs=1;
	      GBTRS( 'N', n3p1, kl, ku, nrhs, wmatrix(0,0), ldab, ipvt(0), pPoint(0), n3p1,info );
	      if( info != 0 )
	      {
		printF("NurbsMapping::interpolate:ERROR  w-matrix: banded solve: return: info=%i\n",info);
	      }

	      // copy the result into the control point array
	      for (k=0; k<=n3; k++)
		cPoint(i,j,k,r) = pPoint(k);
	    }
	  }

      }
      else
      {
	// ------------ W-MATRIX FULL MATRIX SOLVE ------------

	ArraySimple<real> wmatrix(n3+1,n3+1);
	row.redim(p3+1);
	wmatrix=0.;
	for( k=0; k<=n3; k++ )
	{
	  span = findSpan(n3,p3,wBar(k),wKnot); 
	  basisFuns(span,wBar(k),p3,wKnot,row);
	  for( int q=0; q<=p3; q++ )
	    wmatrix(k,q+span-p3)=row(q);
	}

	//  matrix.display("Nurbs: interpolate: matrix");
  
	// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
	GECO( wmatrix(0,0),n3p1,n3p1,ipvt(0),rcond,work(0) );  
	// matrix.display("Nurbs: interpolate: matrix after factor");
	//    wKnot.display("WKNOT");
	if( rcond==0. )
	{
	  printF("NurbsMapping::interpolate:ERROR: After factoring the w-matrix: rcond = %e \n",rcond);
	  OV_ABORT("error");
	}

	pPoint.resize(n3+1);
	job=0;
	for( i=0; i<=n1; i++)
	  for ( j=0; j<=n2; j++ )
	  {
	    for( r=0; r<rangeDimension; r++ )
	    {
	      for (k=0; k<=n3; k++)
		pPoint(k) = rPoint(i,j,k,r);
	      GESL( wmatrix(0,0),n3p1,n3p1,ipvt(0),pPoint(0),job);
	      // copy the result into the control point array
	      for (k=0; k<=n3; k++)
		cPoint(i,j,k,r) = pPoint(k);
	    }
	  }


      } // end full w-matrix solve 
  
    }  // end w-matrix 

  }  // end if domainDimension>1 
  
  // -- restrict the domain to match the ghost points --
  if( true )
  {
    int numGhosta=xGridIndexRange(0,0)-xDimension(0,0);
    int numGhostb=xDimension(1,0)-xGridIndexRange(1,0);
    rStart[0]=uBar(   numGhosta);
    rEnd[0]  =uBar(n1-numGhostb);

    numGhosta=xGridIndexRange(0,1)-xDimension(0,1);
    numGhostb=xDimension(1,1)-xGridIndexRange(1,1);
    rStart[1]=vBar(   numGhosta);
    rEnd[1]  =vBar(n2-numGhostb);

    if( domainDimension==3 )
    {
      numGhosta=xGridIndexRange(0,2)-xDimension(0,2);
      numGhostb=xDimension(1,2)-xGridIndexRange(1,2);
      rStart[2]=wBar(   numGhosta);
      rEnd[2]  =wBar(n3-numGhostb);
    }
  }
  
  // rStart[0]=uBar(   numberOfGhostPoints);
  // rEnd[0]  =uBar(n1-numberOfGhostPoints);
  // rStart[1]=vBar(   numberOfGhostPoints);
  // rEnd[1]  =vBar(n2-numberOfGhostPoints);
  // rStart[2]=wBar(   numberOfGhostPoints);
  // rEnd[2]  =wBar(n3-numberOfGhostPoints);

  // ::display(cPoint,"interpolate: cPoint before reshape","%5.2f ");

  // Resize the cPoint array to match the domain dimension
  if( domainDimension==1 )
    cPoint.reshape(Rn1, rangeDimension+1);
  else if( domainDimension==2 )
    cPoint.reshape(Rn1,Rn2, rangeDimension+1);

  if( false )
  {
    ::display(uKnot,"interpolate: uKnot","%5.2f ");
    ::display(vKnot,"interpolate: vKnot","%5.2f ");
    ::display(cPoint,"interpolate: cpoint","%5.2f ");
  }
  
  initialize();



}





void NurbsMapping::
interpolateSurface(const RealArray & x, 
                   int degree /* = 3 */,
                   ParameterizationTypeEnum  parameterizationType /* =parameterizeByChordLength */,
                   int numberOfGhostPoints /* =0 */,
		   int degree2 /*=3*/) 
// =============================================================================================
// /Description:
//    Interpolate points that define a surface.
// 
//  /degree : build a nurbs with this degree (same degree in both parameter directions)
// /numberOfGhostPoints (input) : The array x include this many ghost points on all sides.
//=====================================================================================
{
  setDomainDimension(2);
  rangeDimension = x.getBound(2)-x.getBase(2)+1;
  if( rangeDimension<=0 || rangeDimension>3 )
  {
    cout << " NurbsMapping::interpolateSurface:ERROR: the range is out of bounds, range =" << rangeDimension << endl;
    {throw "error";}
  }
  
  n1= x.getBound(0)-x.getBase(0);
  n2= x.getBound(1)-x.getBase(1);
  setGridDimensions(axis1,n1+1-2*numberOfGhostPoints);
  setGridDimensions(axis2,n2+1-2*numberOfGhostPoints);

  p1=min(degree,n1);   // must decrease p1 if there are only a few points
  m1=n1+p1+1;
  p2=min(degree2,n2);   // must decrease p2 if there are only a few points
  m2=n2+p2+1;

  int xBase0=x.getBase(0);
  int xBase1=x.getBase(1);

// construct knot vectors
  Range R(0,rangeDimension-1);
  ArraySimple<real> uBar(n1+1), vBar(n2+1);

  // define uBar by chord-length and curvature
  int i, j;
  real chord;
  if( parameterizationType==parameterizeByIndex )
  {
    // use user supplied parameterization
    for (i=0; i<n1; i++)
      uBar(i)=real(i)/n1;
    uBar(n1)=1.;
    for (i=0; i<n2; i++)
      vBar(i) = real(i)/n2;
    vBar(n2)=1.;
  }
  else
  {
    // parameterize by arclength
    for (i=0; i<=n1; i++)
      uBar(i) = 0.;
    for (i=0; i<=n2; i++)
      vBar(i) = 0.;
  
    real total, d;
    ArraySimple<real> cds(max(n1,n2)+1); // always base 0

    int num = n2+1; // number of non-degenerate rows
    for( j=0; j<=n2; j++ )
    {
      total = 0.;
      for( i=1; i<=n1; i++ )
      {
	chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, R) - x(i-1+xBase0, j+xBase1, R))) );
	if( chord==0. )
	{ // we need a non-zero chord length
	  if( i>1 )
	    chord=cds[i-1]-cds[i-2];  // use previous
	  else
	  {
	    if( i<n1 )
	    {
	      chord=SQRT( sum(SQR( x(i+1+xBase0, j+xBase1, R)-x(i+xBase0, j+xBase1, R))) );  // use next
	      if( chord==0. )
		chord=1.;
	    }
	    else
	      chord=1.;
	  }
	} // end if chord == 0
	cds[i] = chord;
	total += cds[i];
      } // end for i=...
    
      if (total == 0.) 
	num -= 1; // this was a degenerate row
      else
      {
	d = 0.;
	for (i=1; i<n1; i++)
	{
	  d += cds[i];
	  uBar(i) += d/total;
	}
      }
    
    } // end for j...
    if (num == 0)
    {
      cout << "NurbsMapping::interpolateSurface: ERROR -- all rows are degenerate! \n";
      throw "error";
    }
// normalize
    for (i=0; i<n1; i++)
      uBar(i) = uBar(i)/num;
    uBar(n1) = 1.;
 
// now do the columns to get vBar 
    num = n1+1; // number of non-degenerate columns
    for( i=0; i<=n1; i++ )
    {
      total = 0.;
      for( j=1; j<=n2; j++ )
      {
	chord=SQRT( sum(SQR( x(i+xBase0, j-1+xBase1, R) - x(i+xBase0, j+xBase1, R))) );
	if( chord==0. )
	{ // we need a non-zero chord length
	  if( j>1 )
	    chord=cds[j-1]-cds[j-2];  // use previous
	  else
	  {
	    if( j<n2 )
	    {
	      chord=SQRT( sum(SQR( x(i+xBase0, j+1+xBase1, R)-x(i+xBase0, j+xBase1, R))) );  // use next
	      if( chord==0. )
		chord=1.;
	    }
	    else
	      chord=1.;
	  }
	} // end if chord == 0
	cds[j] = chord;
	total += cds[j];
      } // end for i=...
    
      if (total == 0.) 
	num -= 1; // this was a degenerate row
      else
      {
	d = 0.;
	for (j=1; j<n2; j++)
	{
	  d += cds[j];
	  vBar(j) += d/total;
	}
      }
    
    } // end for j...
    if (num == 0)
    {
      cout << "NurbsMapping::interpolateSurface: ERROR -- all columns are degenerate! \n";
      throw "error";
    }
    // normalize
    for (j=0; j<n2; j++)
      vBar(j) = vBar(j)/num;
    vBar(n2) = 1.;
    
  }
  

// define the knots by averaging, this will ensure a positive definite matrix below
  uKnot.redim(m1+1);

  uKnot(Range(0,p1))=0.;
  uKnot(Range(m1-p1,m1))=1.;
  real uBarS;
  for( i=p1+1; i<m1-p1; i++ )
  {
//    uBarS = sum(uBar(Range(i-p1,i-1)));
      uBarS=0.;
      for (int q=i-p1; q<= i-1; q++)
        uBarS +=uBar(q);
    
    uKnot(i)=uBarS/p1;
  }
  
// now v...
  vKnot.redim(m2+1);

  vKnot(Range(0,p2))=0.;
  vKnot(Range(m2-p2,m2))=1.;
  real vBarS;
  for( i=p2+1; i<m2-p2; i++ )
  {
//    vBarS = sum(vBar(Range(i-p2,i-1)));
    vBarS=0.;
    for (int q=i-p2; q<= i-1; q++)
      vBarS +=vBar(q);

    vKnot(i)=vBarS/p2;
  }
  
// compute control points
  uMin=0.;
  uMax=1.;
  vMin=0.;
  vMax=1.;

  // fill in the u-matrix
  ArraySimple<real> matrix(n1+1,n1+1);
  RealArray row(p1+1);
  for (i=0; i<=n1; i++)
    for (j=0; j<=n1; j++)
      matrix(i,j)=0.;
  int span;
  for( i=0; i<=n1; i++ )
  {
    span = findSpan(n1,p1,uBar(i),uKnot); 
    basisFuns(span,uBar(i),p1,uKnot,row);
    for( j=0; j<=p1; j++ )
      matrix(i,j+span-p1)=row(j);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
  ArraySimple<real> work(max(n1,n2)+1);
  ArraySimple<int> ipvt(max(n1,n2)+1);
  real rcond;
  
  // factor the matrix -- the semi-bandwidth is p1-1, so for p1=3 the matrix is 5 diagonal
  int n1p1=n1+1;
// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
  GECO( matrix(0,0),n1p1,n1p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring the u-matrix: rcond = %e \n",rcond);
  }
  
// Do n2+1 curve interpolations through the points 
// x(0,j,r), x(1,j,r), ..., x(n1,j,r), j=0,1,...,n2, r=0,1,2 denotes (x,y,z)
  ArraySimple<real> rPoint(n1+1, n2+1, rangeDimension);
  int job=0;
  Range Rn1(0,n1), Rn2(0,n2);
  int r;
  for( j=0; j<=n2; j++)
  {
    for( r=0; r<rangeDimension; r++ )
    {
      for (i=0; i<=n1; i++)
	rPoint(i,j,r)=x(i + xBase0, j + xBase1, r);
      GESL(matrix(0,0), n1p1, n1p1, ipvt(0), rPoint(0,j,r), job);
    }
  }

// Setup the v-matrix
  ArraySimple<real> vmatrix(n2+1,n2+1);
  row.redim(p2+1);
  vmatrix=0.;
  for( j=0; j<=n2; j++ )
  {
    span = findSpan(n2,p2,vBar(j),vKnot); 
    basisFuns(span,vBar(j),p2,vKnot,row);
    for( int q=0; q<=p2; q++ )
      vmatrix(j,q+span-p2)=row(q);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
  // factor the matrix -- the semi-bandwidth is p2-1, so for p2=3 the matrix is 5 diagonal
  int n2p1=n2+1;
// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
  GECO( vmatrix(0,0),n2p1,n2p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring the v-matrix: rcond = %e \n",rcond);
  }

// Secondly, do n1+1 curve interpolations through the points
// rPoint(i,0,r), rPoint(i,1,r), ..., rPoint(i,n2,r), i=0,1,...,n1, r=0,1,2.

// store the control polygon in cPoint
  if( &cPoint != &x )     // the update function may pass cPoint in as x
    cPoint.resize(n1p1, n2p1, rangeDimension+1);
  cPoint(Rn1, Rn2, rangeDimension)=1.;  // unit weights (this is a B-spline surface, not a general NURBS)

  ArraySimple<real> pPoint(n2+1);
  job=0;
  for( i=0; i<=n1; i++)
  {
    for( r=0; r<rangeDimension; r++ )
    {
      for (j=0; j<=n2; j++)
	pPoint(j) = rPoint(i,j,r);
      GESL( vmatrix(0,0),n2p1,n2p1,ipvt(0),pPoint(0),job);
// copy the result into the control point array
      for (j=0; j<=n2; j++)
	cPoint(i,j,r) = pPoint(j);
    }
  }
  
  if( false )
  {
    ::display(uKnot,"interpolateSurface: uKnot","%5.2f ");
    ::display(vKnot,"interpolateSurface: vKnot","%5.2f ");
    ::display(cPoint,"interpolateSurface: cPoint","%5.2f ");
  }
  
  printF("NurbsMapping::interpolateSurface: numberOfGhostPoints=%i\n",numberOfGhostPoints);
  
  if( numberOfGhostPoints>0 )
  {
    // restrict the domain to match the ghost points
    rStart[0]=uBar(   numberOfGhostPoints);
    rEnd[0]  =uBar(n1-numberOfGhostPoints);
    rStart[1]=vBar(   numberOfGhostPoints);
    rEnd[1]  =vBar(n2-numberOfGhostPoints);

    printF("NurbsMapping::interpolateSurface: restrict surface to [%8.2e,%8.2e]x[%8.2e,%8.2e]\n",rStart[0],rEnd[0],rStart[1],rEnd[1]);
  }

  initialize();
}

void NurbsMapping::
interpolateVolume(const RealArray & x, 
                  int degree /* = 3 */,
                  ParameterizationTypeEnum  parameterizationType /* =parameterizeByChordLength */,
                  int numberOfGhostPoints /* =0 */   ) 
// =============================================================================================
// /Description:
//    Interpolate points that define a volume.
// 
//  /degree : build a nurbs with this degree (same degree in all parameter directions)
//  /numberOfGhostPoints (input) : The array x include this many ghost points on all sides.
//=====================================================================================
{
  setDomainDimension(3);
  assert((x.getBound(3)-x.getBase(3)+1)==3);
  rangeDimension = 3;

  if( rangeDimension<=0 || rangeDimension>3 )
  {
    cout << " NurbsMapping::interpolateVolume:ERROR: the range is out of bounds, range =" << rangeDimension << endl;
    {throw "error";}
  }
  
  n1= x.getBound(0)-x.getBase(0);
  n2= x.getBound(1)-x.getBase(1);
  n3= x.getBound(2)-x.getBase(2);
  setGridDimensions(axis1,n1+1-2*numberOfGhostPoints);
  setGridDimensions(axis2,n2+1-2*numberOfGhostPoints);
  setGridDimensions(axis3,n3+1-2*numberOfGhostPoints);

#ifdef USE_PPP
  if( true )
  {
    int myid=max(0,Communication_Manager::My_Process_Number);
    for( int axis=0; axis<domainDimension; axis++ )
    {
      int nMin = ParallelUtility::getMinValue(getGridDimensions(axis));
      int nMax = ParallelUtility::getMaxValue(getGridDimensions(axis));
      if( nMin!=nMax )
      {
        printF("NurbsMapping::interpolateVolume:ERROR: gridDimensions don't match on different processors!\n");
	printf(" myid=%i map.getGridDimensions=[%i,%i,%i]\n",
	       myid,getGridDimensions(0),getGridDimensions(1),getGridDimensions(2));
	fflush(0);
	Overture::abort("error");
      }
    }
  }
#endif      

  p1=min(degree,n1);   // must decrease p1 if there are only a few points
  m1=n1+p1+1;
  p2=min(degree,n2);   // must decrease p2 if there are only a few points
  m2=n2+p2+1;
  p3=min(degree,n3);   // must decrease p3 if there are only a few points
  m3=n3+p3+1;

  int xBase0=x.getBase(0);
  int xBase1=x.getBase(1);
  int xBase2=x.getBase(2);

// construct knot vectors
  Range R(0,rangeDimension-1);
  ArraySimple<real> uBar(n1+1), vBar(n2+1), wBar(n3+1);

  // define uBar by chord-length and curvature
  int i, j, k;
  real chord;
  if( parameterizationType==parameterizeByIndex )
  {
    // use user supplied parameterization
    for (i=0; i<n1; i++)
      uBar(i)=real(i)/n1;
    uBar(n1)=1.;
    for (i=0; i<n2; i++)
      vBar(i) = real(i)/n2;
    vBar(n2)=1.;
    for (i=0; i<n3; i++)
      wBar(i) = real(i)/n3;
    wBar(n3)=1.;
  }
  else
  {
    // parameterize by arclength
    for (i=0; i<=n1; i++)
      uBar(i) = 0.;
    for (i=0; i<=n2; i++)
      vBar(i) = 0.;
    for (i=0; i<=n3; i++)
      wBar(i) = 0.;
  
    real total, d;
    ArraySimple<real> cds(max(n3,max(n1,n2))+1); // always base 0

    int num = (n2+1)*(n3+1); // number of non-degenerate rows
    for ( k=0; k<=n3; k++ )
      {
	for( j=0; j<=n2; j++ )
	  {
	    total = 0.;
	    for( i=1; i<=n1; i++ )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k+xBase2, R) - x(i-1+xBase0, j+xBase1, k+xBase2, R))) );
		if( chord==0. )
		  { // we need a non-zero chord length
		    if( i>1 )
		      chord=cds[i-1]-cds[i-2];  // use previous
		    else
		      {
			if( i<n1 )
			  {
			    chord=SQRT( sum(SQR( x(i+1+xBase0, j+xBase1, k+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
			    if( chord==0. )
			      chord=1.;
			  }
			else
			  chord=1.;
		      }
		  } // end if chord == 0
		cds[i] = chord;
		total += cds[i];
	      } // end for i=...

	    if (total == 0.) 
	      num -= 1; // this was a degenerate row
	    else
	      {
		d = 0.;
		for (i=1; i<n1; i++)
		  {
		    d += cds[i];
		    uBar(i) += d/total;
		  }
	      }
	  }
      }
    
    // end for j...
    if (num == 0)
      {
	cout << "NurbsMapping::interpolateVolume: ERROR -- all rows are degenerate! \n";
	throw "error";
      }
// normalize
    for (i=0; i<n1; i++)
      uBar(i) = uBar(i)/num;
    uBar(n1) = 1.;
    
    
    num = (n1+1)*(n3+1); // number of non-degenerate columns
    for ( k=0; k<=n3; k++ )
      {
	for( i=0; i<=n1; i++ )
	  {
	    total = 0.;
	    for( j=1; j<=n2; j++ )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j-1+xBase1, k+xBase2, R) - x(i+xBase0, j+xBase1, k+xBase2, R))) );
		if( chord==0. )
		  { // we need a non-zero chord length
		    if( j>1 )
		      chord=cds[j-1]-cds[j-2];  // use previous
		    else
		      {
			if( j<n2 )
			  {
			    chord=SQRT( sum(SQR( x(i+xBase0, j+1+xBase1, k+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
			    if( chord==0. )
			      chord=1.;
			  }
			else
			  chord=1.;
		      }
		  } // end if chord == 0
		cds[j] = chord;
		total += cds[j];
	      } // end for i=...
    
	    if (total == 0.) 
	      num -= 1; // this was a degenerate row
	    else
	      {
		d = 0.;
		for (j=1; j<n2; j++)
		  {
		    d += cds[j];
		    vBar(j) += d/total;
		  }
	      }
	  }
      } // end for k...
    if (num == 0)
    {
      cout << "NurbsMapping::interpolateVolume: ERROR -- all columns are degenerate! \n";
      throw "error";
    }
    // normalize
    for (j=0; j<n2; j++)
      vBar(j) = vBar(j)/num;
    vBar(n2) = 1.;

    num = (n1+1)*(n2+1); // number of non-degenerate columns
    for ( int i=0; i<=n1; i++ )
      {
	for( j=0; j<=n2; j++ )
	  {
	    total = 0.;
	    for( k=1; k<=n3; k++ )
	      {
		chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k-1+xBase2, R) - x(i+xBase0, j+xBase1, k+xBase2, R))) );
		if( chord==0. )
		  { // we need a non-zero chord length
		    if( k>1 )
		      chord=cds[k-1]-cds[k-2];  // use previous
		    else
		      {
			if( k<n3 )
			  {
			    chord=SQRT( sum(SQR( x(i+xBase0, j+xBase1, k+1+xBase2, R)-x(i+xBase0, j+xBase1, k+xBase2, R))) );  // use next
			    if( chord==0. )
			      chord=1.;
			  }
			else
			  chord=1.;
		      }
		  } // end if chord == 0
		cds[k] = chord;
		total += cds[k];
	      } // end for k=...
    
	    if (total == 0.) 
	      num -= 1; // this was a degenerate row
	    else
	      {
		d = 0.;
		for (k=1; k<n3; k++)
		  {
		    d += cds[k];
		    wBar(k) += d/total;
		  }
	      }
	  } // end for j...
      } // end for i...
    if (num == 0)
    {
      cout << "NurbsMapping::interpolateVolume: ERROR -- all columns are degenerate! \n";
      throw "error";
    }
    // normalize
    for (j=0; j<n3; j++)
      wBar(j) = wBar(j)/num;
    wBar(n3) = 1.;
    
  }
  

// define the knots by averaging, this will ensure a positive definite matrix below
  uKnot.redim(m1+1);

  uKnot(Range(0,p1))=0.;
  uKnot(Range(m1-p1,m1))=1.;
  real uBarS;
  for( i=p1+1; i<m1-p1; i++ )
  {
//    uBarS = sum(uBar(Range(i-p1,i-1)));
      uBarS=0.;
      for (int q=i-p1; q<= i-1; q++)
        uBarS +=uBar(q);
    
    uKnot(i)=uBarS/p1;
  }
  
// now v...
  vKnot.redim(m2+1);

  vKnot(Range(0,p2))=0.;
  vKnot(Range(m2-p2,m2))=1.;
  real vBarS;
  for( i=p2+1; i<m2-p2; i++ )
  {
//    vBarS = sum(vBar(Range(i-p2,i-1)));
    vBarS=0.;
    for (int q=i-p2; q<= i-1; q++)
      vBarS +=vBar(q);

    vKnot(i)=vBarS/p2;
  }

// and w...
  wKnot.redim(m3+1);

  wKnot(Range(0,p3))=0.;
  wKnot(Range(m3-p3,m3))=1.;
  real wBarS;
  for( i=p3+1; i<m3-p3; i++ )
  {
    wBarS=0.;
    for (int q=i-p3; q<= i-1; q++)
      wBarS +=wBar(q);

    wKnot(i)=wBarS/p3;
  }
  
// compute control points
  uMin=0.;
  uMax=1.;
  vMin=0.;
  vMax=1.;
  wMin=0.;
  wMax=1.;

  // fill in the u-matrix
  ArraySimple<real> matrix(n1+1,n1+1);
  RealArray row(p1+1);
  for (i=0; i<=n1; i++)
    for (j=0; j<=n1; j++)
      matrix(i,j)=0.;
  int span;
  for( i=0; i<=n1; i++ )
  {
    span = findSpan(n1,p1,uBar(i),uKnot); 
    basisFuns(span,uBar(i),p1,uKnot,row);
    for( j=0; j<=p1; j++ )
      matrix(i,j+span-p1)=row(j);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
  ArraySimple<real> work(max(n3,max(n1,n2))+1);
  ArraySimple<int> ipvt(max(n3,max(n1,n2))+1);
  real rcond;
  
  // factor the matrix -- the semi-bandwidth is p1-1, so for p1=3 the matrix is 5 diagonal
  int n1p1=n1+1;
// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
  GECO( matrix(0,0),n1p1,n1p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring the u-matrix: rcond = %e \n",rcond);
  }
  
// Do (n2+1)*(n3+1) curve interpolations through the points 
// x(0,j,k,r), x(1,j,k,r), ..., x(n1,j,k,r), j=0,1,...,n2, k=0,1,....,n3, r=0,1,2 denotes (x,y,z)
  ArraySimple<real> rPoint(n1+1, n2+1, n3+1, rangeDimension);
  int job=0;
  Range Rn1(0,n1), Rn2(0,n2),Rn3(0,n3);
  int r;
  for ( k=0; k<=n3; k++ )
    for( j=0; j<=n2; j++)
      {
	for( r=0; r<rangeDimension; r++ )
	  {
	    for (i=0; i<=n1; i++)
	      rPoint(i,j,k,r)=x(i + xBase0, j + xBase1, k+xBase2, r);
	    GESL(matrix(0,0), n1p1, n1p1, ipvt(0), rPoint(0,j,k,r), job);
	  }
      }

// Setup the v-matrix
  ArraySimple<real> vmatrix(n2+1,n2+1);
  row.redim(p2+1);
  vmatrix=0.;
  for( j=0; j<=n2; j++ )
  {
    span = findSpan(n2,p2,vBar(j),vKnot); 
    basisFuns(span,vBar(j),p2,vKnot,row);
    for( int q=0; q<=p2; q++ )
      vmatrix(j,q+span-p2)=row(q);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
  // factor the matrix -- the semi-bandwidth is p2-1, so for p2=3 the matrix is 5 diagonal
  int n2p1=n2+1;
  int n3p1=n3+1;
// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
  GECO( vmatrix(0,0),n2p1,n2p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring the v-matrix: rcond = %e \n",rcond);
  }

// Secondly, do n1+1 curve interpolations through the points
// rPoint(i,0,r), rPoint(i,1,r), ..., rPoint(i,n2,r), i=0,1,...,n1, r=0,1,2.

  ArraySimple<real> pPoint(n2+1);
  job=0;
  for ( k=0; k<=n3; k++ )
    for( i=0; i<=n1; i++)
      {
	for( r=0; r<rangeDimension; r++ )
	  {
	    for (j=0; j<=n2; j++)
	      pPoint(j) = rPoint(i,j,k,r);
	    GESL( vmatrix(0,0),n2p1,n2p1,ipvt(0),pPoint(0),job);
	    // copy the result into the control point array
	    for (j=0; j<=n2; j++)
	      rPoint(i,j,k,r) = pPoint(j);
	  }
      }

// store the control polygon in cPoint
  if( &cPoint != &x )     // the update function may pass cPoint in as x
    cPoint.resize(n1p1, n2p1, n3p1, rangeDimension+1);
  cPoint(Rn1, Rn2, Rn3, rangeDimension)=1.;  // unit weights (this is a B-spline surface, not a general NURBS)

// Setup the 2-matrix
  ArraySimple<real> wmatrix(n3+1,n3+1);
  row.redim(p3+1);
  wmatrix=0.;
  for( k=0; k<=n3; k++ )
  {
    span = findSpan(n3,p3,wBar(k),wKnot); 
    basisFuns(span,wBar(k),p3,wKnot,row);
    for( int q=0; q<=p3; q++ )
      wmatrix(k,q+span-p3)=row(q);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
// AP: Note that dgeco / sgeco factors a full matrix and doesn't make use of the band structure.
  GECO( wmatrix(0,0),n3p1,n3p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  //    wKnot.display("WKNOT");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring the w-matrix: rcond = %e \n",rcond);
    cout<<n3<<", "<<wmatrix<<endl;
    wKnot.display("WKNOT");
    cout<<"WBAR"<<wBar<<endl;
    abort();
  }

  pPoint.resize(n3+1);
  job=0;
  for( i=0; i<=n1; i++)
    for ( j=0; j<=n2; j++ )
      {
	for( r=0; r<rangeDimension; r++ )
	  {
	    for (k=0; k<=n3; k++)
	      pPoint(k) = rPoint(i,j,k,r);
	    GESL( wmatrix(0,0),n3p1,n3p1,ipvt(0),pPoint(0),job);
	    // copy the result into the control point array
	    for (k=0; k<=n3; k++)
	      cPoint(i,j,k,r) = pPoint(k);
	  }
      }

  if( numberOfGhostPoints>0 )
  {
    // restrict the domain to match the ghost points
    rStart[0]=uBar(   numberOfGhostPoints);
    rEnd[0]  =uBar(n1-numberOfGhostPoints);
    rStart[1]=vBar(   numberOfGhostPoints);
    rEnd[1]  =vBar(n2-numberOfGhostPoints);
    rStart[2]=wBar(   numberOfGhostPoints);
    rEnd[2]  =wBar(n3-numberOfGhostPoints);
  }

  //  cPoint.display("CPOINT");
  initialize();
}

#ifdef USE_PPP
//   *********** obsolete method -- keep for now for backward compatibility *********
void NurbsMapping::
interpolate(const realArray & x_, 
	    const int & option    /* = 0 */,
	    realArray & parameterization_ /* =Overture::nullRealDistributedArray() */,
            int degree /* = 3 */,
            ParameterizationTypeEnum parameterizationType /* =parameterizeByChordLength */,
            int numberOfGhostPoints /* =0 */ )
{
  #ifdef USE_PPP
    const realSerialArray & x = x_.getLocalArray();
    const realSerialArray & parameterizationC = parameterization_.getLocalArray();
    realSerialArray & parameterization = (realSerialArray &)parameterizationC;
  #else
    const realSerialArray & x = x_;
    realSerialArray & parameterization = parameterization_;
  #endif
  interpolate(x,option,parameterization,degree,parameterizationType,numberOfGhostPoints);
}
#endif
     
void NurbsMapping::
interpolate(const RealArray & x, 
	    const int & option    /* = 0 */,
	    RealArray & parameterization /* =Overture::nullRealArray() */,
            int degree /* = 3 */,
            ParameterizationTypeEnum parameterizationType /* =parameterizeByChordLength */,
            int numberOfGhostPoints /* =0 */ )
// =====================================================================================
/// \brief  
///     Define a new NURBS curve that interpolates the points x(0:n1,0:r-1) OR
///  define a new NURBS surface that interpolates the points x(0:n1,0:n2,0:r-1) (NEW feature). 
///  An even more recent (051031) addition allows the interpolation of a volume defined
///  by the points x(0:n1,0:n2,0:n3,0:r-1);
///   By default the NURBS curve will be parameterized by a the chord length.
///  
/// \param option (input) : if option==0 then use the array parameterization.
///      if option==1 then return the parameterization used in the array parameterization.
/// \param parameterization_(0:n1) (input) : optionally specify the parameterization. These values
///  should start from 0, end at 1 and be increasing. If this argument is not given then
///    the parameterization will be based on chord length. If option==1 then the 
///    actual parameterization used will be returned in this array.
/// \param degree (input) : degree of approximation. Normally a value such as 1,2,3.
/// \param parameterizationType (input) : the default parameterization (if not user defined) is by chord-length.
///        One can also specify to parameterizeByIndex which means use a uniform parameterization.
/// \param numberOfGhostPoints (input) : The array x includes this many ghost points on all sides.
/// 
//=====================================================================================
{

// check for surface interpolation
  if (x.numberOfDimensions() == 3)
  {
    interpolateSurface(x, degree, parameterizationType, numberOfGhostPoints);
    return;
  }
  else if ( x.numberOfDimensions()==4 )
  {
    interpolateVolume(x,degree,parameterizationType, numberOfGhostPoints);
    return;
  }
  
  setDomainDimension(1);
  rangeDimension = x.getBound(1)-x.getBase(1)+1;
  if( rangeDimension<=0 || rangeDimension>3 )
  {
    cout << " NurbsMapping::interpolate:ERROR: the range is out of bounds, range =" << rangeDimension << endl;
    {throw "error";}
  }
  

  int xBase0=x.getBase(0);

  n1= x.getBound(0)-x.getBase(0);
  setGridDimensions(axis1,n1+1-2*numberOfGhostPoints);

  p1=min(degree,n1);   // must decrease p1 if there are only a few points
  m1=n1+p1+1;
  n2=0;
  p2=0;
  m2=0;
  vKnot.redim(0);
  
  Range R(0,rangeDimension-1);
  RealArray uBar(n1+1);
  if( option==0 && parameterization.getLength(0)>0 )
  {
    // use user specified parameterization
    uBar=parameterization(Range(0,n1));
  }
  else if( parameterizationType==parameterizeByIndex )
  {
    for( int i=0; i<n1; i++)
      uBar(i)=real(i)/n1;
    uBar(n1)=1.;
  }
  else 
  {
    // define uBar by chord-length and curvature
    real chord;
    uBar(0)=0.;
    for( int i=1; i<=n1; i++ )
    {
      chord=SQRT( sum(SQR( x(i+xBase0,R)-x(i-1+xBase0,R))) );
      if( chord==0. )
      { // we need a non-zero chord length
        if( i>1 )
          chord=uBar(i-1)-uBar(i-2);  // use previous
	else
	{
          if( i<n1 )
	  {
            chord=SQRT( sum(SQR( x(i+1+xBase0,R)-x(i+xBase0,R))) );  // use next
            if( chord==0. )
              chord=1.;
	  }
          else
	    chord=1.;
	}
      }
      uBar(i)=uBar(i-1)+chord;
    }
    if( uBar(n1)==0. )
    {
      cout << "NurbsMapping::interpolate: ERROR -- total chord length is zero ! \n";
      {throw "error";}
    }
    uBar*=(1./uBar(n1));
    uBar(n1)=1.;
    // uBar.display("interpolate: uBar");
    
  }
  
  // define the knots by averaging, this will ensure a positive definite matrix below
  uKnot.redim(m1+1);

  uKnot(Range(0,p1))=0.;
  uKnot(Range(m1-p1,m1))=1.;
  int i;
  for( i=p1+1; i<m1-p1; i++ )
    uKnot(i)=sum(uBar(Range(i-p1,i-1)))*(1./p1);
  
/* ----
  // ***** don't clamp if periodic *** this doesn't work
  for( i=p1; i<=m1-p1; i++ )
    uKnot(i)=real(i-p1+1)/(m1-2*p1+2);
  uKnot.display("interpolate: uKnot");
---- */
  
  uMin=0.;
  uMax=1.;

  // these are needed by basisFuns
  int p = max(p1,p2);
//   left.redim(p+1);
//   right.redim(p+1); 

  // fill in the matrix
  RealArray matrix(n1+1,n1+1), row(p1+1);
  matrix=0.;
  for( i=0; i<=n1; i++ )
  {
    int span = findSpan(n1,p1,uBar(i),uKnot); 
    basisFuns(span,uBar(i),p1,uKnot,row);
    for( int j=0; j<=p1; j++ )
      matrix(i,j+span-p1)=row(j);
  }

  //  matrix.display("Nurbs: interpolate: matrix");
  
  RealArray work(n1+1);
  IntegerArray ipvt(n1+1);
  real rcond;
  
  // factor the matrix -- the semi-bandwidth is p1-1, so for p1=3 the matrix is 5 diagonal
  int n1p1=n1+1;
  GECO( matrix(0,0),n1p1,n1p1,ipvt(0),rcond,work(0) );  
  // matrix.display("Nurbs: interpolate: matrix after factor");
  if( rcond==0. )
  {
    printf("NurbsMapping::interpolate:ERROR: After factoring: rcond = %e \n",rcond);
  }
  
  if( &cPoint != &x )     // the update function may pass cPoint in as x
    cPoint.resize(n1p1,rangeDimension+1);
  int job=0;
  Range Rn(0,n1);
  cPoint(Rn,rangeDimension)=1.;  // weights
  for( int r=0; r<rangeDimension; r++ )
  {
    if( &cPoint != &x )
      cPoint(Rn,r)=x(Rn+xBase0,r);
    GESL( matrix(0,0),n1p1,n1p1,ipvt(0),cPoint(0,r),job);
  }

// tmp
//      printf("first point x:(%e, %e, %e), cPoint:(%e, %e, %e)\n", x(xBase0,0), x(xBase0,1), x(xBase0,2),
//  	   cPoint(0,0), cPoint(0,1), cPoint(0,2));
//      printf("last point  x:(%e, %e, %e), cPoint:(%e, %e, %e)\n", x(xBase0+n1,0), x(xBase0+n1,1), x(xBase0+n1,2),
//  	   cPoint(n1,0), cPoint(n1,1), cPoint(n1,2));
    
//    x.display("Here is x");
//    cPoint.display("Here is cPoint");

  if( numberOfGhostPoints>0 )
  {
    // restrict the domain to match the ghost points
    rStart[0]=uBar(   numberOfGhostPoints);
    rEnd[0]  =uBar(n1-numberOfGhostPoints);
  }
  

  initialize();

  if( option==1 )
  {
    // return the parameterization that was used
    parameterization.redim(0);
    parameterization=uBar;
  }
}


void NurbsMapping::
interpolate(Mapping & map, int degree /* =3 */, 
	    ParameterizationTypeEnum parameterizationType /* =parameterizeByChordLength */,
	    int numberOfGhostPoints /* =0 */,
	    int *numPointsToInterpolate /* =NULL */  )
// =============================================================================
/// \details 
///    Construct a NURBS by interpolating another mapping.
/// \param map (input) : interpolate this Mapping.
/// \param degree (input) : degree of NURBS
/// \param parameterization (input) : 
/// \param numberOfGhostPoints (input) : include this many ghost points
/// \param numPointsToInterpolate (input) : numPointsToInterpolate[dir], dir=0,1,...,domainDimension-1, 
///        optionally specify how many points to interpolate in each direction. By default use the number 
///        of grid points from the Mapping "map". 
// =============================================================================
{
  RealArray x; 

  bool useGridFromMapping= numberOfGhostPoints==0;
  #ifdef USE_PPP
    // In parallel: if the grid is distributed then we must eval a local copy on this proc.
    // since the interpolate function does not take a distributed grid. 
    useGridFromMapping= useGridFromMapping && !map.usesDistributedInverse(); 
  #endif

  // We cannot use the existing grid in the Mapping if the user has specified a different number of points
  bool useDifferentNumberOfGridPoints =false;
  if( numPointsToInterpolate!=NULL )
  {
    for( int axis=0; axis<map.getDomainDimension(); axis++ )
    {
      if( numPointsToInterpolate[axis]!=map.getGridDimensions(axis) )
      {
	useDifferentNumberOfGridPoints=true;
	break;
      }
    }
  }
  if( useDifferentNumberOfGridPoints )
  {
    printF("NurbsMapping:INFO: will interpolate with ");
    for( int axis=0; axis<map.getDomainDimension(); axis++ )
      printF("%i, ", numPointsToInterpolate[axis]);
    printF(" grid points instead of the default\n");
  }
  
  useGridFromMapping = useGridFromMapping && !useDifferentNumberOfGridPoints;

  if( useGridFromMapping )
  {
    #ifdef USE_PPP
      if( map.usesDistributedInverse() )
      {
        printF("NurbsMapping::interpolate:ERROR:cannot interpolate a Mapping with a distributed inverse\n");
        printF(" mappingName=%s\n",(const char*)map.getName(mappingName));
        Overture::abort("error");
      }
      x = map.getGridSerial();
    #else
      x = map.getGrid();
    #endif
  }
  else
  {
    // if there are ghost points we need to compute the extra values.
    // printf("plotMapping: compute ghost values\n");
    Index Igv[3], &Ig1=Igv[0], &Ig2=Igv[1], &Ig3=Igv[2];   // include ghost points
    Ig1=0; Ig2=0; Ig3=0;
    real dr[3]={1.,1.,1.}; //
    for( int axis=0; axis<map.getDomainDimension(); axis++ )
    {
      int n;
      if( numPointsToInterpolate==NULL )
        n=map.getGridDimensions(axis);
      else
      {
	if( numPointsToInterpolate[axis]<1 )
	{
          n=map.getGridDimensions(axis);
	  printF("NurbsMapping::interpolate:ERROR: numPointsToInterpolate[%i]=%i is <1 !\n"
                 " I will use %i grid points instead\n",numPointsToInterpolate[axis],n);
	}
	else
	{
	  n=numPointsToInterpolate[axis];
	}
      }
	
      dr[axis]=1./max(1.,n-1.);
      Igv[axis]=Range(-numberOfGhostPoints,n+numberOfGhostPoints-1);
    }

    // ********************************** fix this -- only compute ghost values *************
    x.redim(Ig1,Ig2,Ig3,map.getRangeDimension());

    RealArray r(Ig1,Ig2,Ig3,map.getDomainDimension());
    int i1,i2,i3;
    for( i3=Ig3.getBase(); i3<=Ig3.getBound(); i3++ )
    {
      for( i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
	r(Ig1,i2,i3,0).seqAdd(dr[axis1]*Ig1.getBase(),dr[axis1]);

      if( map.getDomainDimension()>1 )
      {
	for( i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
	  r(i1,Ig2,i3,1).seqAdd(dr[axis2]*Ig2.getBase(),dr[axis2]);
      }
    }
    if( map.getDomainDimension()>2 )
    {
      for( i2=Ig2.getBase(); i2<=Ig2.getBound(); i2++ )
	for( i1=Ig1.getBase(); i1<=Ig1.getBound(); i1++ )
	  r(i1,i2,Ig3,2).seqAdd(dr[axis3]*Ig3.getBase(),dr[axis3]);
    }

    
    // ::display(r,"nurbsInterpolate: r","%5.2f ");
    

    #ifdef USE_PPP
      map.mapGridS(r,x,Overture::nullRealArray());
    #else
      map.mapGrid(r,x,Overture::nullRealDistributedArray());
    #endif

    // ::display(x,"nurbsInterpolate: x","%5.2f ");

  }

  if( map.getDomainDimension()==1 )
    x.reshape(x.getLength(0)*x.getLength(1)*x.getLength(2),x.getLength(3));
  else if ( map.getDomainDimension()==2 )
    x.reshape(x.getLength(0),x.getLength(1)*x.getLength(2),x.getLength(3));
  else 
    x.reshape(x.getLength(0),x.getLength(1),x.getLength(2),x.getLength(3));

  interpolate(x,0,Overture::nullRealArray(),degree,parameterizationType,numberOfGhostPoints);

  for( int dir=0; dir<map.getDomainDimension(); dir++ )
  {
    setGridDimensions( dir,map.getGridDimensions(dir) );
    for( int side=0; side<=1; side++ )
    {
      setBoundaryCondition(side,dir,map.getBoundaryCondition(side,dir));
      setShare(side,dir,map.getShare(side,dir));
    }
    setIsPeriodic(dir,map.getIsPeriodic(dir));
    nurbsIsPeriodic[dir] = map.getIsPeriodic(dir);
  }
}



