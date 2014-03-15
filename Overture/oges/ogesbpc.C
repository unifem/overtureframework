#include "Oges.h"
#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

#define CGESPC3  cgespc3_
#define CGESPC4  cgespc4_
#define CGESPC5  cgespc5_
#define BSORT    bsort_
#define SGECO    sgeco_
#define DGECO    dgeco_
#define SGEDI    sgedi_
#define DGEDI    dgedi_
#define CGESSRA  cgessra_
#define SORTII   sortii_

extern "C"
{
  void CGESPC3( int & neqp, int & nzep, int & iep0, int & iap0, int & jap0, 
    real & ap0, int & iep, int & iap, int & jap, real & ap );
  
  void CGESPC4( int & neq, int & nze, int & ia, int & iabpc );

  void CGESPC5( int & neq, int & nze, int & ia, int & iabpc );
  
  void BSORT( const int & , int & , real & );

  void SGECO( real & b, const int & nbd, int & nb, int & ipvt,real & rcond, real & work );

  void DGECO( real & b, const int & nbd, int & nb, int & ipvt,real & rcond, real & work );

  void SGEDI( real & b, const int & nbd, int & nb, int & ipvt,real & det, real & work, 
              const int & job );

  void DGEDI( real & b, const int & nbd, int & nb, int & ipvt,real & det, real & work,
              const int & job );

  void CGESSRA( int & ie0, int & nb, int & ie, real & ap, int & neq, int & ia, int & ja, 
                real & a, int & iab, int & jab, real & ab, real & epsz );
  void SORTII( int & neqp, int & iep, int & iepi );
}

void Oges::setupBoundaryPC( int &  neqp, int & nzep, int & ierr )
{
  // c======================================================================
  // c
  // c    Boundary Pre-Conditioner
  // c    -------------------------
  // c
  // c Purpose
  // c   Locally invert the equations for a boundary point and fictitous
  // c   points. This should remove the problem of zero pivots so that
  // c   the Yale solver can work on a wider class of matrices. This
  // c   routine should also help the convergence of interative solvers.
  // c
  // c Method
  // c   For each point i=(i1,i2,i3) on the boundary take the equations
  // c   associated with the boundary point and neighbouring fictitious points
  // c   and solve for the boundary point and fictitious points in terms
  // c   of the other points. Change the original sparse matrix by replacing
  // c   the old equations with these new ones.
  // c
  // c   For example if (i1,i2) is on the side (kd,ks)=(1,1) and there
  // c   are 2 fictitious points, then take the equations for the points
  // c           equation 1 : (i1  ,i2)
  // c           equation 2 : (i1-1,i2)
  // c           equation 3 : (i1-2,i2)
  // c   Solve these 3 equations for the values at these 3 points in terms
  // c   of the other points. These three new equations replace the original
  // c   three equations in the sparse matrix (ia,ja,a). The second sparse
  // c   matrix (neqp,iep,iap,jap,ap) keeps track of the transformations
  // c   that need to be applied to the right-hand side vector when solving
  // c   a problem.
  // c
  // c   At a corner we solve for the corner point and the fictitious
  // c   points in each direction, for example in 2D with 2 fictitious points
  // c   solve for the following 5 points:
  // c           equation 1 : (i1  ,i2  )
  // c           equation 2 : (i1-1,i2  )
  // c           equation 3 : (i1-2,i2  )
  // c           equation 4 : (i1  ,i2-1)
  // c           equation 5 : (i1  ,i2-2)
  // c
  // c   Before solving a problem Ax=b you must first "precondition" b
  // c   with the (iap,jap,ap) matrix:
  // c           do n=1,neqp
  // c             temp=0.
  // c             do j=iap(n),iap(n+1)-1
  // c               temp=temp+ap(j)*b(jap(j))
  // c             end do
  // c             b(iep(n))=temp
  // c           end do
  // c   These operations are performed in cgesl.
  // c
  // c Output
  // c  (ia,ja,a) : new sparse matrix
  // c  (iep,iap,jap,ap) : sparse matrix used to precondition the rhs
  // c  rhsp(neqp) : space allocated for cgesl if storageFormat=1
  // c
  // c======================================================================

  if( Oges::debug & 4 ) cout << "Entering ogesbpc..." << endl;
  
  //      === ispfmt : sparse format
  if( sparseFormat==0 )
  {
    //        ---ia stored in compressed format
    iabpc.adopt(&ia(1),Range(1,ia.GET_LENGTH(axis1)+1));
  }
  else
  {
    iabpc.redim(Range(1,numberOfEquations+1+1));
    if( Oges::debug & 2 ) cout << "ogesbpc: creating compressed ia..." << endl;
    CGESPC4( numberOfEquations,numberOfNonzeros,ia(1),iabpc(1) );
  }
  

  int nfict = 1; // **** wdh **** numberOfGhostLines; //  ...number of fictitous points:

//   int nfict = discretizationOptions & 1 ? 2 : 1;   //  ...number of fictitous points:
     
  //        ...Count number of boundary points
  int nbp=0;
  int ndr1,ndr2;
  for( int grid=0; grid<numberOfGrids; grid++ )
  {
    for( int axis=axis1; axis<numberOfDimensions; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
        if( cg[grid].boundaryCondition()(side,axis) !=0 )
	{
	  //              count points on line (plane) normal to direction kd
          int axisp1 = (axis+1  )% 3 ;
	  int axisp2 = (axisp1+1)% 3 ;
          if( axisp1 < numberOfDimensions )
	  {
            // nrsab(kdp1,2,k)-nrsab(kdp1,1,k)+1;
            ndr1=cg[grid].gridIndexRange()(End,axisp1) 
	        -cg[grid].gridIndexRange()(Start,axisp1)+1;
	  }
	  else
            ndr1=1;

          if( axisp2 < numberOfDimensions )
	  {
            // nrsab(kdp2,2,k)-nrsab(kdp2,1,k)+1;
            ndr2=cg[grid].gridIndexRange()(End,axisp2) 
	        -cg[grid].gridIndexRange()(Start,axisp2)+1;
	  }
	  else
            ndr2=1;
	  
          nbp+=ndr1*ndr2;
	}
      }
    }
  }

  // 
  //      ---neqb : upper bound for number of boundary equations
  //      ---nzeb : upper bound for nonzeroes in the bound. eqns
  int neqb=nbp*(nfict+1)*numberOfComponents;
  // *wdh   nzeb=neqb*nze/neq + neq*nv + neqb*2 +1000  ! fix this
  // *wdh nzeb=neqb*nze/neq + neq*nv + neqb*nd*nfict +1000  ! fix this
  int nzeb=int(neqb*pow( (2*nfict+1), numberOfDimensions ));
  
  //      ---neqp : upper bound for number of preconditioning eqns
  //      ---nzep : upper bound for nonzeroes preconditioning equations
//  int neqp0=neqp;  // save input values, this may be a refactor
//  int nzep0=nzep;

  neqp=neqb;


  nzep=int( (nbp-pow(2,numberOfDimensions))*pow((nfict+1)*numberOfComponents,2)
    +pow(2,numberOfDimensions)*pow((nfict*numberOfDimensions+1)*numberOfComponents,2)
    +500*numberOfDimensions ); // fix this

  //      --- allocate work space for temporary arrays
  //        (iab,jab,ab) : holds boundary equations
  //        (ia1,ja1,a1) : holds new sparse matrix (temporary)
  //        iep,(iap,ja,ap) : holds preconditioner for the rhs


  //  space for nonzeroes in new sparse matrix
  int nze1=numberOfNonzeros+2*numberOfEquations*nfict;

  //  integer work space
  int niwk=neqb+3*neqp+4+numberOfEquations+nzeb+nzep+nze1; 
  //  real work space
  int nrwk=nzeb+nzep+nze1;                   

  iwkbpc.redim(Range(1,niwk+1));
  rwkbpc.redim(Range(1,nrwk+1));


  iab.redim(Range(1,neqb+1+1));  
  jab.redim(Range(1,nzeb+1));    
  ia1.redim(Range(1,numberOfEquations+1+1));  
  ja1.redim(Range(1,nze1+1));   
  iep0.redim(Range(1,neqp+1+1));
  iap0.redim(Range(1,neqp+1+1));
  jap0.redim(Range(1,nzep+1));  
  iepi.redim(Range(1,neqp+1));  

  ab.redim(Range(1,nzeb+1)); 
  a1.redim(Range(1,nze1+1)); 
  ap0.redim(Range(1,nzep+1));


  setupBoundaryPC2( iabpc, nfict,neqb,nzeb,nze1,neqp,nzep,iepi );

  // *       write(*,'('' **** CGES: dskdf after cgespc2***'')')
  // *       call dskdf( id,' ',6,ierr )


  //      ---allocate space for (iep,iap,jap,ap) : rhs preconditioner
  iep.redim(Range(1,neqp+1+1)); 
  iap.redim(Range(1,neqp+1+1)); 

  if( storageFormat==1 )
  { //  ...rhsp is a temp array used when the storage format is compressed
    rhsp.redim(Range(1,neqp+1+1)); 
  }
  
    
  jap.redim(Range(1,max(1,nzep+1)));  
  ap.redim(Range(1,max(1,nzep+1)));   
  
  //      ---copy the values of (iep,iap,jap,ap) from their temporary spot
  CGESPC3( neqp,nzep,
          iep0(1),iap0(1),jap0(1),ap0(1),
	  iep(1),iap(1),jap(1),ap(1) );
  

  if( sparseFormat==1 )
  {
    if( Oges::debug & 2 )       
      cout << " ogesbpc: creating un-compressed ia..." << endl;
    CGESPC5( numberOfEquations,numberOfNonzeros,ia(1),iabpc(1) );
  }
  
  //      ---delete work spaces
  iwkbpc.redim(0);
  rwkbpc.redim(0);
  
  if( Oges::debug & 4 ) cout << "Leaving ogesbpc..." << endl;
}

extern int eqn;



void Oges::setupBoundaryPC2( IntegerArray & ia0, int & nfict, int &  neqb, int & nzeb, 
 int & nze1, int & neqp, int & nzep, IntegerArray & iepi0 )
{
  
  // ===========================================================
  //     Boundary Pre-conditioner
  // 
  //  Purpose
  //    Locally invert the equations for a boundary point and fictitous
  //    points. This should remove the problem of zero pivots so that
  //    the Yale solver can work on a wider class of matrices. This
  //    routine should also help the convergence of interative solvers.
  // 
  // 
  //  Input
  //   nd,ng,nv,neq
  //   ia,ja,a - matrix generated by cgesg
  //   peqn(ng) - for the mapping from (n,i1,i2,i3,k) to equation number
  //              (defined in cgesg)
  //   lratio : =1 single precision, 2= double precision
  //   iab(neq+1),jab(nze),ab(nze) : work space
  //   nfict : number of fictitous points (1 for 2nd order, 2 for 4th order)
  //  Output
  // 
  // ===========================================================

  if( Oges::debug & 2 ) 
    cout << "ogesbpc:ogesbpc2: Entering ogesbpc2..." << endl;

// int Class ?
  IntegerArray kds(3,2,3,2); kds.setBase(1);
  IntegerArray ifp(3,3,2);   ifp.setBase(1);

  //       data kds/1,1,1, 1,2,2,  1,1,1, 2,1,2,  1,1,1, 2,2,1,
  //      &         2,1,1, 2,2,2,  1,2,1, 2,2,2,  1,1,2, 2,2,2/
  kds(1,1,1,1)=1;
  kds(2,1,1,1)=1;
  kds(3,1,1,1)=1;

  kds(1,2,1,1)=1;
  kds(2,2,1,1)=2;
  kds(3,2,1,1)=2;

  kds(1,1,2,1)=1;
  kds(2,1,2,1)=1;
  kds(3,1,2,1)=1;

  kds(1,2,2,1)=2;
  kds(2,2,2,1)=1;
  kds(3,2,2,1)=2;

  kds(1,1,3,1)=1;
  kds(2,1,3,1)=1;
  kds(3,1,3,1)=1;

  kds(1,2,3,1)=2;
  kds(2,2,3,1)=2;
  kds(3,2,3,1)=1;

  kds(1,1,1,2)=2;
  kds(2,1,1,2)=1;
  kds(3,1,1,2)=1;

  kds(1,2,1,2)=2;
  kds(2,2,1,2)=2;
  kds(3,2,1,2)=2;

  kds(1,1,2,2)=1;
  kds(2,1,2,2)=2;
  kds(3,1,2,2)=1;

  kds(1,2,2,2)=2;
  kds(2,2,2,2)=2;
  kds(3,2,2,2)=2;

  kds(1,1,3,2)=1;
  kds(2,1,3,2)=1;
  kds(3,1,3,2)=2;

  kds(1,2,3,2)=2;
  kds(2,2,3,2)=2;
  kds(3,2,3,2)=2;
                       
  //       data ifp/-1,0,0, 0,-1,0, 0,0,-1, 1,0,0, 0,1,0, 0,0,1/
  ifp(1,1,1)=-1;
  ifp(2,1,1)=0;
  ifp(3,1,1)=0;
  
  ifp(1,2,1)=0;
  ifp(2,2,1)=-1;
  ifp(3,2,1)=0;
  
  ifp(1,3,1)=0;
  ifp(2,3,1)=0;
  ifp(3,3,1)=-1;

  ifp(1,1,2)=1;
  ifp(2,1,2)=0;
  ifp(3,1,2)=0;
  
  ifp(1,2,2)=0;
  ifp(2,2,2)=1;
  ifp(3,2,2)=0;
  
  ifp(1,3,2)=0;
  ifp(2,3,2)=0;
  ifp(3,3,2)=1;
  
  IntegerArray iv(Range(1,4)); 
//  const int nbd=7*numberOfComponents;
  const int nbd=7*10;
  IntegerArray ipvt(Range(1,nbd+1));  
  IntegerArray ie(Range(1,nbd+1));   
  RealArray b(Range(1,nbd,nbd+1)); 
  RealArray work(Range(1,nbd+1));  

  int i,j,n;

  //      ---sort entries in a() so that column indices are increasing
  for( i=1; i<=numberOfEquations; i++ )
    BSORT( ia0(i+1)-ia0(i),ja(ia0(i)),a(ia0(i)) );
  
  //   initialize iab(1)=1
  int ie0=0;    //      ! current equation in (iab,jab,ab)
  iab(1)=1; //   ! holds temporary copy of transformed boundary equations
  iap0(1)=1; //   ! holds sparse matrix used to transform the rhs

  //      =====For each side of each grid Do ======
  int k,grid;
  int kd,axis;
  int ks,side;
  int i3a,i3b;  
  int i1n,i2n,i3n;
  int ib,jb,nb;
  
  for( grid=0, k=1; grid<numberOfGrids; grid++, k++ )
  {
    MappedGrid & c = cg[grid];
    for( axis=axis1, kd=1; axis<numberOfDimensions; axis++, kd++)
    {
      for( side=Start, ks=1; side<=End; side++, ks++ )
      {
        if( c.boundaryCondition()(side,axis) > 0 )
	{
	  //              ===boundary condition side
          //              ...loop over points on this side
          if( numberOfDimensions==2 )
	  {
	    i3a=c.dimension()(Start,axis3);
            i3b=c.dimension()(End,axis3);
	  }
	  else
	  {
	    i3a=c.indexRange()(kds(3,1,kd,ks)-1,axis3);
	    i3b=c.indexRange()(kds(3,2,kd,ks)-1,axis3);
	  }
          for( int i3=i3a; i3<=i3b; i3++ )
	  {
	    for( int i2= c.indexRange()(kds(2,1,kd,ks)-1,axis2); 
		     i2<=c.indexRange()(kds(2,2,kd,ks)-1,axis2); i2++)
	    {
  	      for( int i1= c.indexRange()(kds(1,1,kd,ks)-1,axis1); 
		       i1<=c.indexRange()(kds(1,2,kd,ks)-1,axis1); i1++)
	      {
                if( cg[grid].mask()(i1,i2,i3) > 0 )
		{
		  //                      ...get eqn numbers for this boundary point
                  //                      ie(m) m=1,...,nb : equation numbers for the
                  //                         boundary point and fictitious points
		  nb=0;
                  int n;
		  for( n=0; n<numberOfComponents; n++ )
		  {
		    nb++;
                    ie(nb)=equationNo(n,i1,i2,i3,grid); // eqn(n,i1,i2,i3,k,peqn);
		  }
                  int skip = FALSE;
		  for( i=1; i<=ie0; i++ )
		  {
		    if( ie(1)==iep0(i) )  // skip this point - we have already done it
		    {
		      skip=TRUE;
		      break;
		    }
                  }
                  if( skip )
                    continue;  // skip this point
		  
                  //  ...here are the neighbouring fictitous point(s):
                  int ifict;
                  for( ifict=1; ifict<=nfict; ifict++ )
		  {
		    i1n=i1+ifp(1,kd,ks)*ifict;
		    i2n=i2+ifp(2,kd,ks)*ifict;
                    i3n=i3+ifp(3,kd,ks)*ifict;
                    for( n=0; n<numberOfComponents; n++ )
		    {
		      nb++;
		      ie(nb)=equationNo(n,i1n,i2n,i3n,grid);  // eqn(n,i1n,i2n,i3n,k,peqn);
		    }
		  }
		  
                  // ...check for corners, add points in other directions
                  iv(1)=i1;
                  iv(2)=i2;
                  iv(3)=i3;

                  for( int kdd=0; kdd<=numberOfDimensions-2; kdd++ )
		  {
                     //   kd2=mod(kd+kdd,nd)+1     ! kd2 = other directions
                    int kd2=( (kd+kdd) % numberOfDimensions)+1;
                    for( int ks2=1; ks2<=2; ks2++ )
		    {
                      if( iv(kd2)==c.indexRange()(ks2-1,kd2-1) )
		      {
			// ...here are the neighbouring fictitous point(s):
                        //  write(*,'('' CGESBPC2: PC corner, kd,ks,kd2,ks2='',4i4)')
                        // *      & kd,ks,kd2,ks2
                        for( ifict=1; ifict<=nfict; ifict++ )
			{
			  i1n=i1+ifp(1,kd2,ks2)*ifict;
                          i2n=i2+ifp(2,kd2,ks2)*ifict;
                          i3n=i3+ifp(3,kd2,ks2)*ifict;
                          for( n=0; n<numberOfComponents; n++ )
			  {
			    nb++;
                            ie(nb)=equationNo(n,i1n,i2n,i3n,grid); // eqn(n,i1n,i2n,i3n,k,peqn);
			  }
			}
		      }
		    }
		  }
                  if( nb > nbd )
		  {
		    cerr << "ogespc2: ERROR nb > nbd " << endl;
                    exit(1);
		  }
                  // ---load the matrix with the coefficients

                  for( ib=1; ib<=nb; ib++ )
		  {
		    for( int jb=1; jb<=nb; jb++ )
                      b(ib,jb)=0.;
		    i=ie(ib);
		    for( int ii=ia0(i); ii<=ia0(i+1)-1; ii++ )
		    {
		      int jj=ja(ii);
		      for( int jb=1; jb<=nb; jb++ )
		      {
			if( jj==ie(jb) )
                          b(ib,jb)=a(ii);
		      }
		    }
		  }

                  if( Oges::debug & 16 )
		  {
		    printf(" ogespc2: k=%2i, kd = %4i, ks=%1i, i1=%6i, i2=%6i, i3=%6i \n",
                      k,kd,ks,i1,i2,i3);
		  }
                  if( Oges::debug & 32 )
		  {
                    // 9000 format(1x,' CGESPER: kd,ks,i1,i2,i3 =',2i2,3i6)
                    //      do n=1,nb
                    //        write(1,9100) n,ie(n),(ja(j),a(j),j=ia0(ie(n)),ia0(ie(n)+1)-1)
                    //      end do
                    // 9100 format(1x,' n,ie(n),(j,a) =',i2,i8,(1x,10(i6,e9.1)))
                    cout << " b : " << endl;
                    for( ib=1; ib<=nb; ib++ )
		    {
                      for( jb=1; jb<=nb; jb++ )
		      {
			printf(" %8.2e ",b(ib,jb));
		      }
		      printf("\n");
		    }
		  }

                  real rcond;
                  //                      --Factor the matrix : b(nb,nb)
                  if( realToIntegerRatio==1 )
                    SGECO( b(1,1),nbd,nb,ipvt(1),rcond,work(1) );
		  else
                    DGECO( b(1,1),nbd,nb,ipvt(1),rcond,work(1) );

                  if( rcond < parameters.matrixCutoff )
		  {
		    printf(" ogesbpc2 Warning rcond= %12.4g, i1,i2,i3,k= "
			   "%6i,%6i,%6i, %3i",rcond,i1,i2,i3,k);
		    if( rcond==0. )
		    {
                      cerr << "error: rcond=0 ! " << endl;
		      exit(1);
		    }
		  }
                  //  --Invert the matrix
                  real det;
                  if( realToIntegerRatio==1 )
                    SGEDI( b(1,1),nbd,nb,ipvt(1),det,work(1),1 );
                  else
                    DGEDI( b(1,1),nbd,nb,ipvt(1),det,work(1),1 );
		  
                  if( Oges::debug & 32  )
		  {
		    cout << "ogesp2: rcond = " << rcond << endl;
		    // write(1,'('' rcond,ipvt ='',e12.4,99i4)') rcond,(ipvt(i),i=1,nb)
                    cout << " b (inverse): " << endl;
                    for( ib=1; ib<=nb; ib++ )
		    {
                      for( jb=1; jb<=nb; jb++ )
		      {
			printf(" %8.2e ",b(ib,jb));
		      }
		      printf("\n");
		    }
		  }

                  //  ...Save the inverse in the sparse array (iap0,jap0,ap0)
                  //  ...Change the equations in (ia,ja,a) by multiplying
                  //     through by the inverse -> (iab,jab,ab)
                  for( ib=1; ib<=nb; ib++ )
		  {
		    ie0++;
		    if( ie0 > neqp )
		    {
		      cerr << "ogesbpc2: dimension error neqp too small" << endl;
                      cerr << " neqp = " << neqp << endl;
		      exit(1);
		    }
		    iep0(ie0)=ie(ib); //  ! equation number
                    iap0(ie0+1)=iap0(ie0)+nb;
		    if( iap0(ie0+1)-1 > nzep )
		    {
		      cerr << "ogesbpc2: dimension error nzep too small";
		      exit(1);
		    }
                    for( int j=iap0(ie0); j<=iap0(ie0+1)-1; j++ )
		    {
		      jb=j-iap0(ie0)+1;
                      jap0(j)=ie(jb);
                      ap0(j)=b(ib,jb);
		    }
                    // ...ab(i0,j) <- sum c(n)*a(ie(n),j)  n=1,...,ne
                    //    c(n)=ap0(iap0(ie0)+n-1)
                    CGESSRA( ie0,nb,ie(1),ap0(iap0(ie0)),
                            numberOfEquations,ia0(1),ja(1),a(1),iab(1),jab(1),ab(1),parameters.matrixCutoff );
		    if( iab(ie0+1)-1 > nzeb )
		    {
		      cerr << " ogesbpc2: nzeb too small, nzeb = " << nzeb << endl;
                      cerr << " ...increase zeroRatio" << endl;
		      exit(1);
		    }
                    // if( d(4) )then
                    //   write(1,9300) ie0,(jab(j),ab(j),j=iab(ie0),iab(ie0+1)-1)
                    // end if
		  }
		}
		//  100                continue
	      }
	    }
	  }
	}
      }
    }
  }
  
  if( Oges::debug & 4 )
  {
    cout << " CGESPC2: neqb = " << neqb << ", neqb(true) = " << ie0 << endl;
    cout << " CGESPC2: nzeb = " << nzeb << ", nzeb(true) = " << iab(ie0+1)-1 << endl;
  //      write(*,'('' CGESPC2: neqp, neqp(true) ='',2i8)') neqp,ie0
  //      write(*,'('' CGESPC2: nzep, nzep(true) ='',2i8)') nzep,
  //   &   iap0(ie0+1)-1
  }
  
  neqp=ie0; //  ! number of equations in sparse matrix for rhs
  nzep=iap0(neqp+1)-1;

  //      ---sort the iep0 array into increasing order
  for( i=1; i<=neqp; i++ )
    iepi0(i)=i;
  
  // *       if( d(1) ) write(*,'('' CGESBPC2: begin sort(2)...'')')
  // *       call bsorti( neqp,iep0,iepi0 )

  SORTII( neqp,iep0(1),iepi0(1) );

  // *       if( d(1) ) write(*,'('' CGESBPC2: end sort(2)...'')')
  // *       write(1,'('' neqp,nzep = '',2i6)') neqp,nzep
  // *       write(1,'('' Sorted iep0:'',/,(1x,20i6))') (iep0(i),i=1,neqp)
  // *       write(1,'('' Sorted iepi0:'',/,(1x,20i6))') (iepi0(i),i=1,neqp)


  //      === form new sparse matrix with new equations at boundary replacing
  //          the old equations
  int m=1;
  
  ia1(1)=1;
  iep0(neqp+1)=numberOfEquations+1; // ! special value at end
  for( n=1; n<=numberOfEquations; n++ )
  {
    if( n < iep0(m) )
    {
      ia1(n+1)=ia1(n)+ia0(n+1)-ia0(n);
      if( ia1(n+1)-1 > nze1 )
      {
	cerr << " CGES:CGESPC: error nze1 too small , nze1 = " << nze1 << endl;
	exit(1);
      }
      for( j=ia1(n); j<=ia1(n+1)-1; j++ )
      {
        ja1(j)=ja(j-ia1(n)+ia0(n));
        a1(j)=  a(j-ia1(n)+ia0(n) );
      }
    }
    else
    {
      //          ---here is a new boundary equation
      int m0=iepi0(m);
      ia1(n+1)=ia1(n)+iab(m0+1)-iab(m0);
      if( ia1(n+1)-1 > nze1 )
      {
	cerr << " CGES:CGESPC: error nze1 too small , nze1 = " << nze1 << endl;
	exit(1);
      }
      for( j=ia1(n); j<=ia1(n+1)-1; j++ )
      {
        ja1(j)=jab(j-ia1(n)+iab(m0));
        a1(j)=  ab(j-ia1(n)+iab(m0) );
      }
      m++;   //          ---increment m
    }
  }
  
  if( Oges::debug & 2 )
    cout << "CGESBPC: Old nze = " << numberOfNonzeros << "  new nze =" << ia1(numberOfEquations+1)-1 << endl;

  int nzenew=ia1(numberOfEquations+1)-1;
  if( nzenew > numberOfNonzerosBound )
  {
    cerr << " CGESPC2: old nze, new nze = " << numberOfNonzeros << ", " << nzenew << endl;
    cerr << "CGESPC2: Increase zeroRatio to >= " << (nzenew+1.)/numberOfEquations << endl;
    exit(1);
  }
  
  numberOfNonzeros=ia1(numberOfEquations+1)-1; // new number of non-zeroes

  //      ---copy back into ia,ja,a
  for( i=1; i<=numberOfEquations+1; i++ )
    ia0(i)=ia1(i);
  
  for( i=1; i<=numberOfNonzeros; i++ )
  {
    ja(i)=ja1(i);
    a(i)=a1(i);
  }
  
  // *       write(1,*) ' Matrix at end of CGESPC2...'
  // *       do n=1,neq
  // *         write(1,9700) n,(ja(j),a(j),j=ia0(n),ia0(n+1)-1)
  // *       end do
  // *  9700 format(1x,' n,(j,a) =',i8,(1x,10(i6,e9.1)))

  // ****** unsort the iep0 array - should really sort (iap0,jap0,ap0)
  for( i=1; i<=neqp; i++ )
    ia1(i)=iep0(i);
  
  for( i=1; i<=neqp; i++ )
    iep0(iepi0(i))=ia1(i);

}
