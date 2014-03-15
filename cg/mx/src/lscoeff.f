c      subroutine lscoeff(a,m,n,qri,iwrk,wrk,ierr)
      subroutine lscoeff(a,m,n,qri,rcond,wrk,ierr)

      implicit none

c     Consider the least squares problem
c     Au = b
c     where A \in R^{m x n}, u \in R^{n,1} and b \in R^{m,1}
c     with n \le m. 
c     This subroutine computes the matrix M such that
c        u = Mb is the solution to the least squares problem.
c
c     QR factorization is used to construct M.
c
c     First we compute Q^T such that 
c          [ R_u ]
c     Ru = [     ] u = Q^T b
c          [ 0   ] 
c     with R_u an upper triangular matrix and Q^T Q = I (Q unitary).
c     Q^T = \prod_{k=0}^{k=n} P_k 
c     with P_0 = I and P_k, k>0 the matrices diag(I_k, H_{n-k}) where
c     H_p the Householder matrix for the column vector A_{ik}, i=k..n .
c
c     R (R_u actually) is then computed by Q^T A .
c
c     Now rewrite the system as 
c      [ R_u ]     [ Q^T_1 ] { b_1 }
c      [     ] u = [       ] {     }
c      [ 0   ]     [ Q^T_2 ] { b_2 }
c     so that Q^T_1 is n x m, Q^T_2 is (m-n) x m .
c     There are now two sets of systems, one of which is
c       R_u u = Q^T_1 { b_1, b_2 }^T .  As long as rank(A)>=n
c     R_u can be inverted to give M=(R_u)^{-1} Q^T_1 .

c    NOTES: (R_u)^{-1} is temporarily stored in qri.
c
c           In fact, R is never even formed.  When a value of R is needed, 
c           is computed from the appropriate matrix-vector multiply using Q^T and A.
c
c     INPUT :  a(m,n)   - the matrix A
c              m        - row dimension of the matrix A
c              n        - column dimension of A
      integer m,n
      double precision a(m,*)

c     OUTPUT : qri(n,m) - the matrix M such that Mb = u
c              ierr     - 0 if everything was ok, 1 if rank(A)<n
      double precision qri(n,*)
      integer ierr

c     WORK : wrk(m,m+1) - Q^T is actually stored here
c            !!! NOTE the m+1 HERE, we store tmp values of the
c                Householder vectors here
c            iwrk(m) - integer work array used to store column pivots
      double precision wrk(m,*)
c      integer iwrk(*)
c     temporaries
c
c     these variables are used to compute the the Householder matrices
c           they correspond to the notation in Golub and Van Loam, 
c           Algs. 3.3-1, 3.3-2 (except that vm is "m" in the book).
      double precision vm, beta, alpha, s
      
c     
      double precision r,t,tt
      double precision nrm_r,nrm_rinv,rcond

      logical debug

c     some loop variables
      integer i,j,k,l,p

c      print *,"LSCOEFF ",m,"  ",n

      debug = .false.
      ierr = 0
      nrm_r = 0
      nrm_rinv = 0

      do j=1,m+1
         do i=1, m
            wrk(i,j) = 0
         end do
      end do

      do i=1,m
         wrk(i,i) = 1
         do j=1,n
            qri(j,i) = 0
         end do
      end do

c     compute Q^T by successivly multiplying Householder matrices
      do l=1,n
         qri(l,l) = 1

c        ALG 3.3-1
         vm = 0
         do i=l,m
c           compute what a(i,l) would be 
            s=0
            do j=1,m
               s = s + wrk(i,j)*a(j,l)
            end do

            vm = max(vm,dabs(s))
         end do

         alpha = 0
         do i=l,m
            
c           compute what a(i,l) would be 
            s = 0
            do j=1,m
               s = s + wrk(i,j)*a(j,l)
            end do

c            if ( ((1.+s).eq.(1.)) .and. (i.eq.l) ) then 
c               print *,"singular at ",l,"  ",s,"  ",m
c            end if


c           assign the normalized value for the vector (as in ALG 3.3-1)     
            wrk(i,m+1) = s/vm
            alpha = alpha + wrk(i,m+1)*wrk(i,m+1)
         end do

         alpha = sqrt(alpha)
         beta = 1./(alpha*(alpha+dabs(wrk(l,m+1))))
         wrk(l,m+1) = wrk(l,m+1) + sign(alpha,wrk(l,m+1))

c        ALG 3.3-2 on the submatrix starting at (l,1)
         do p=1,m
            s = 0
            do j=l,m
               s = s + wrk(j,m+1)*wrk(j,p)
            end do

            s = beta*s

            do i=l,m
               wrk(i,p) = wrk(i,p) - s*wrk(i,m+1)
            end do

         end do

      end do

c     now compute R_u^{-1} Q_1^T
      do j=1,n-1
         wrk(j,m+1) = 0
      end do
      wrk(n,m+1) = 1


c     compute R_u^{-1} and temporarily store it in qri
      do i=n,1,-1

         t = 0
         do k=1,m
            t = t + wrk(i,k)*a(k,i)
         end do
c        t = R_u(i,i)
         nrm_r = nrm_r + t*t

         tt = 100 + dabs(t)
         if ( tt.eq.(100) ) then 
            if ( debug ) then
               print *,
     .              "WARNING : the ls system may be rank deficient! ",t, 
     .              "at ",i
            end if
            ierr = i
         end if

         if ( debug .and. (dabs(t).lt.1e-14) ) then
            print *,"WARNING : small t = ",t
            print *,"        : boolean is ",tt.eq.(100)
         end if
c         wrk(i,m+1) = 1./t
c         qri(i,i) = 1./t

         do j=i-1,1,-1
            s=0
            do k=1,m
               s = s + wrk(j,k)*a(k,i)
            end do
c     s = R_u(j,i)
            nrm_r = nrm_r + s*s

            tt = 100 + dabs(t/s)
            if ( tt.eq.(100) ) then 
               if ( debug ) then
                  print *,
     .            "WARNING : the ls system may be rank deficient! ",s,t, 
     .              "at ",i
                  print *,"HEY : ",s,t,t/s
               end if
               ierr = i
            end if

            s = s/t

            do k=n,1,-1
               qri(j,k) = qri(j,k)-qri(i,k)*s
            end do
         end do
         
         do j=1,n
            qri(i,j) = qri(i,j)/t
            nrm_rinv = nrm_rinv + qri(i,j)*qri(i,j)
         end do
      end do

      if ( ierr.ne.0 ) return

c      print *,"RCOND = ",1./(nrm_r*nrm_rinv)
      rcond = 1./(nrm_r*nrm_rinv)

      if ( (100.+rcond) .eq. 100. ) then
         if ( debug ) then
            print *,
     .           "WARNING : Ill conditioned LS system : rcond = ",rcond
            print *, 
     .           "        : nrm_r = ",nrm_r," : nrm_rinv = ",nrm_rinv
            do i=1,n
               print "('[',11(e12.5,5x),$)", (qri(i,k),k=1,n)
               print *,"]"
            end do
         end if
         ierr=1
         
      end if
      
      do i=1,n

c        save the first row of R_u^{-1} in wrk(:,m+1)
         do j=1,n
            wrk(j,m+1) = qri(i,j)
         end do

         do j=1,m
            qri(i,j) = 0
            do k=1,n
               qri(i,j) = qri(i,j) + wrk(k,m+1)*wrk(k,j)
            end do
         end do
      end do

      if ( debug .and. (ierr.gt.0) ) then

       print *,"A= "
       do i=1,m
          print "('[',11(e12.5,5x),$)", (a(i,j),j=1,n)
          print *,"]"
       end do

c     compute Q^T A just for a test
C       print *,"Q^T A = "
C       do i=1,m
C          do j=1,n
C             wrk(j,m+1) = 0
C             do k=1,m
C                wrk(j,m+1) = wrk(j,m+1) + wrk(i,k)*a(k,j)
C             end do
C          end do
C          print '10(f12.8,2x)', (wrk(j,m+1),j=1,n)
C       end do

C       print *,"Q^T= "
C       do i=1,m
C          print '10(f12.8,2x)', (wrk(i,j),j=1,m)
C       end do

c       print *,"qri= "
c       do i=1,n
c          print "'[',20(f12.5,5x),$", (qri(i,j),j=1,m)
c          print *,"]"
c       end do
      endif      

      return
      end
