! This file automatically generated from pentaDiagonal.bf with bpp.


c *** for axis==0 we put the j,k loops on the outside


c *** for axis!=0 we put the j,k loops on the inside



c ****************************************************************
c *************** Normal System **********************************
c ****************************************************************


c ****************************************************************
c *************** Extended System ********************************
c ****************************************************************



c ****************************************************************
c *************** Periodic System ********************************
c ****************************************************************






      subroutine pentaFactor(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ipar, a, b, 
     & c, d, e, s,t,u,v )
c ========================================================================
c   Factor a penta-diagonal matrix (no pivoting)
c 
c             | c[0] d[0] e[0]                     |
c             | b[1] c[1] d[1] e[1]                |
c         A = | a[2] b[2] c[2] d[2] e[2]           |
c             |      a[3] b[3] c[3] d[3] e[3]      |
c             |            .    .    .             |
c             |           a[.] b[.] c[.] d[.] e[.] |
c             |                a[.] b[.] c[.] d[.] |
c             |                     a[n] b[n] c[n] |
c 
c  Input:  a, b, c, d, e : arrays denoting the 5 diagonals.
c  Input: s,t,u,v : work-space for the periodic case (otherwise not used)
c                   These same work-spaces must be passed to the oslve function
c ========================================================================

      implicit none
c
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      real a(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real b(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real e(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real t(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      integer ipar(0:*)
c ...... local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,systemType
      integer nia,nib,nja,njb,njc,nka,nkb,nkc

      integer i,j,k,nx,axis,n
      real tmp1,tmp2

      integer normal,extended,periodic
      parameter( normal=0,extended=1,periodic=2 )

c.........................

      n1a        = ipar(0)
      n1b        = ipar(1)
      n1c        = ipar(2) ! stride
      n2a        = ipar(3)
      n2b        = ipar(4)
      n2c        = ipar(5)
      n3a        = ipar(6)
      n3b        = ipar(7)
      n3c        = ipar(8)
      systemType = ipar(9)
      axis       = ipar(10)

      nx=n1b
      n=n1b
      if( systemType.eq.normal )then

        if( axis.eq.0 )then
! factorOrSolveNormal(factor,0)

! initFactorOrSolve(factor,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
           do i = nia+1,nib-1
! startInnerLoop(0)
!             #If "0" != "0"
!               #If "factor" == "factor"
               tmp1 = b(i,j,k)/c(i-1,j,k)
               b(i,j,k)=tmp1  ! save pivots here
               c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
               d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
               tmp2 = a(i+1,j,k)/c(i-1,j,k)
               a(i,j,k)=tmp2
               b(i+1,j,k) = b(i+1,j,k) - tmp2 * d(i-1,j,k)
               c(i+1,j,k) = c(i+1,j,k) - tmp2 * e(i-1,j,k)
! endInnerLoop(0)
!             #If "0" != "0"
           end do

! startInnerLoop(0)
!            #If "0" != "0"
!             #If "factor" == "factor"
             tmp1 = b(n,j,k) / c(n-1,j,k)
             b(n,j,k)=tmp1
             c(n,j,k) = c(n,j,k) - tmp1 * d(n-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do


!            #If "factor" == "solve"
        else if( axis.eq.1 )then
! factorOrSolveNormal(factor,1)

! initFactorOrSolve(factor,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(1)
!            #If "1" == "0"
           do i = nia+1,nib-1
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,i,k)/c(j,i-1,k)
               b(j,i,k)=tmp1  ! save pivots here
               c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
               d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
               tmp2 = a(j,i+1,k)/c(j,i-1,k)
               a(j,i,k)=tmp2
               b(j,i+1,k) = b(j,i+1,k) - tmp2 * d(j,i-1,k)
               c(j,i+1,k) = c(j,i+1,k) - tmp2 * e(j,i-1,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do
           end do

! startInnerLoop(1)
!            #If "1" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
!             #If "factor" == "factor"
             tmp1 = b(j,n,k) / c(j,n-1,k)
             b(j,n,k)=tmp1
             c(j,n,k) = c(j,n,k) - tmp1 * d(j,n-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

! endOuterLoop(1)
!            #If "1" == "0"


!            #If "factor" == "solve"
        else
! factorOrSolveNormal(factor,2)

! initFactorOrSolve(factor,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(2)
!            #If "2" == "0"
           do i = nia+1,nib-1
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,k,i)/c(j,k,i-1)
               b(j,k,i)=tmp1  ! save pivots here
               c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
               d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
               tmp2 = a(j,k,i+1)/c(j,k,i-1)
               a(j,k,i)=tmp2
               b(j,k,i+1) = b(j,k,i+1) - tmp2 * d(j,k,i-1)
               c(j,k,i+1) = c(j,k,i+1) - tmp2 * e(j,k,i-1)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do
           end do

! startInnerLoop(2)
!            #If "2" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
!             #If "factor" == "factor"
             tmp1 = b(j,k,n) / c(j,k,n-1)
             b(j,k,n)=tmp1
             c(j,k,n) = c(j,k,n) - tmp1 * d(j,k,n-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

! endOuterLoop(2)
!            #If "2" == "0"


!            #If "factor" == "solve"
        end if

      else if( systemType.eq.extended )then
        if( axis.eq.0 )then
! factorOrSolveExtended(factor,0)

! initFactorOrSolve(factor,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(0)
!             #If "0" != "0"
            i = nia+1
!            #If "factor" == "factor"
             tmp1 = b(i,j,k)/c(i-1,j,k) ! eliminate b(1)
             b(i,j,k)=tmp1         ! save pivots in the element that we eliminate
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * a(i-1,j,k)
             a(i,j,k) = a(i,j,k) - tmp1 * b(i-1,j,k)

             tmp2 = a(i+1,j,k)/c(i-1,j,k) ! eliminate a(2)
             a(i+1,j,k)=tmp2       ! save pivots in the element that we eliminate
             b(i+1,j,k) = b(i+1,j,k) - tmp2 * d(i-1,j,k)
             c(i+1,j,k) = c(i+1,j,k) - tmp2 * e(i-1,j,k)
             d(i+1,j,k) = d(i+1,j,k) - tmp2 * a(i-1,j,k)
             e(i+1,j,k) = e(i+1,j,k) - tmp2 * b(i-1,j,k)

            i = nia+2
!            #If "factor" == "factor"
             tmp1 = b(i,j,k)/c(i-1,j,k)  ! eliminate b(2)
             b(i,j,k)=tmp1         ! save pivots in the element that we eliminate
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * a(i-1,j,k)

             tmp2 = a(i+1,j,k)/c(i-1,j,k)  ! eliminate a(3)
             a(i+1,j,k)=tmp2       ! save pivots in the element that we eliminate
             b(i+1,j,k) = b(i+1,j,k) - tmp2 * d(i-1,j,k)
             c(i+1,j,k) = c(i+1,j,k) - tmp2 * e(i-1,j,k)
             d(i+1,j,k) = d(i+1,j,k) - tmp2 * a(i-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

            do i = nia+3,nib-3
! startInnerLoop(0)
!              #If "0" != "0"
!               #If "factor" == "factor"
               tmp1 = b(i,j,k)/c(i-1,j,k)
               b(i,j,k)=tmp1                 ! save pivots in the element that we eliminate
               c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
               d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)

               tmp2 = a(i+1,j,k)/c(i-1,j,k)
               a(i+1,j,k)=tmp2               ! save pivots in the element that we eliminate
               b(i+1,j,k) = b(i+1,j,k) - tmp2 * d(i-1,j,k)
               c(i+1,j,k) = c(i+1,j,k) - tmp2 * e(i-1,j,k)
! endInnerLoop(0)
!              #If "0" != "0"
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(0)
!             #If "0" != "0"
            i=nib-2
!             #If "factor" == "factor"
             tmp1=b(i,j,k) / c(i-1,j,k)  ! eliminate b(n-2)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)

            i=nib-1
!             #If "factor" == "factor"
             tmp1=e(i,j,k) / c(i-3,j,k) ! eliminate e(n-1)
             e(i,j,k)=tmp1
             a(i,j,k) = a(i,j,k) - tmp1 * d(i-3,j,k)
             b(i,j,k) = b(i,j,k) - tmp1 * e(i-3,j,k)

             i=nib
!             #If "factor" == "factor"
             tmp1=d(i,j,k) / c(i-4,j,k)    ! eliminate d(n)
             d(i,j,k)=tmp1
             e(i,j,k) = e(i,j,k) - tmp1 * d(i-4,j,k)
             a(i,j,k) = a(i,j,k) - tmp1 * e(i-4,j,k)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "factor" == "factor"
             tmp1=a(i,j,k) / c(i-2,j,k)  ! eliminate a(n-1)
             a(i,j,k)=tmp1
             b(i,j,k) = b(i,j,k) - tmp1 * d(i-2,j,k)
             c(i,j,k) = c(i,j,k) - tmp1 * e(i-2,j,k)

             tmp1=b(i,j,k) / c(i-1,j,k)  ! eliminate b(n-1)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)

             i=nib
!             #If "factor" == "factor"
             tmp1=e(i,j,k) / c(i-3,j,k)  ! eliminate e(n)
             e(i,j,k)=tmp1
             a(i,j,k) = a(i,j,k) - tmp1 * d(i-3,j,k)
             b(i,j,k) = b(i,j,k) - tmp1 * e(i-3,j,k)

             tmp1=a(i,j,k) / c(i-2,j,k)  ! eliminate a(n)
             a(i,j,k)=tmp1
             b(i,j,k) = b(i,j,k) - tmp1 * d(i-2,j,k)
             c(i,j,k) = c(i,j,k) - tmp1 * e(i-2,j,k)

             tmp1=b(i,j,k) / c(i-1,j,k)    ! eliminate b(n)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
! endInnerLoop(0)
!             #If "0" != "0"

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do

!            #If "factor" == "solve"

        else if( axis.eq.1 )then
! factorOrSolveExtended(factor,1)

! initFactorOrSolve(factor,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(1)
!            #If "1" == "0"

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i = nia+1
!            #If "factor" == "factor"
             tmp1 = b(j,i,k)/c(j,i-1,k) ! eliminate b(1)
             b(j,i,k)=tmp1         ! save pivots in the element that we eliminate
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
             e(j,i,k) = e(j,i,k) - tmp1 * a(j,i-1,k)
             a(j,i,k) = a(j,i,k) - tmp1 * b(j,i-1,k)

             tmp2 = a(j,i+1,k)/c(j,i-1,k) ! eliminate a(2)
             a(j,i+1,k)=tmp2       ! save pivots in the element that we eliminate
             b(j,i+1,k) = b(j,i+1,k) - tmp2 * d(j,i-1,k)
             c(j,i+1,k) = c(j,i+1,k) - tmp2 * e(j,i-1,k)
             d(j,i+1,k) = d(j,i+1,k) - tmp2 * a(j,i-1,k)
             e(j,i+1,k) = e(j,i+1,k) - tmp2 * b(j,i-1,k)

            i = nia+2
!            #If "factor" == "factor"
             tmp1 = b(j,i,k)/c(j,i-1,k)  ! eliminate b(2)
             b(j,i,k)=tmp1         ! save pivots in the element that we eliminate
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
             e(j,i,k) = e(j,i,k) - tmp1 * a(j,i-1,k)

             tmp2 = a(j,i+1,k)/c(j,i-1,k)  ! eliminate a(3)
             a(j,i+1,k)=tmp2       ! save pivots in the element that we eliminate
             b(j,i+1,k) = b(j,i+1,k) - tmp2 * d(j,i-1,k)
             c(j,i+1,k) = c(j,i+1,k) - tmp2 * e(j,i-1,k)
             d(j,i+1,k) = d(j,i+1,k) - tmp2 * a(j,i-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

            do i = nia+3,nib-3
! startInnerLoop(1)
!              #If "1" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,i,k)/c(j,i-1,k)
               b(j,i,k)=tmp1                 ! save pivots in the element that we eliminate
               c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
               d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)

               tmp2 = a(j,i+1,k)/c(j,i-1,k)
               a(j,i+1,k)=tmp2               ! save pivots in the element that we eliminate
               b(j,i+1,k) = b(j,i+1,k) - tmp2 * d(j,i-1,k)
               c(j,i+1,k) = c(j,i+1,k) - tmp2 * e(j,i-1,k)
! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i=nib-2
!             #If "factor" == "factor"
             tmp1=b(j,i,k) / c(j,i-1,k)  ! eliminate b(n-2)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)

            i=nib-1
!             #If "factor" == "factor"
             tmp1=e(j,i,k) / c(j,i-3,k) ! eliminate e(n-1)
             e(j,i,k)=tmp1
             a(j,i,k) = a(j,i,k) - tmp1 * d(j,i-3,k)
             b(j,i,k) = b(j,i,k) - tmp1 * e(j,i-3,k)

             i=nib
!             #If "factor" == "factor"
             tmp1=d(j,i,k) / c(j,i-4,k)    ! eliminate d(n)
             d(j,i,k)=tmp1
             e(j,i,k) = e(j,i,k) - tmp1 * d(j,i-4,k)
             a(j,i,k) = a(j,i,k) - tmp1 * e(j,i-4,k)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "factor" == "factor"
             tmp1=a(j,i,k) / c(j,i-2,k)  ! eliminate a(n-1)
             a(j,i,k)=tmp1
             b(j,i,k) = b(j,i,k) - tmp1 * d(j,i-2,k)
             c(j,i,k) = c(j,i,k) - tmp1 * e(j,i-2,k)

             tmp1=b(j,i,k) / c(j,i-1,k)  ! eliminate b(n-1)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)

             i=nib
!             #If "factor" == "factor"
             tmp1=e(j,i,k) / c(j,i-3,k)  ! eliminate e(n)
             e(j,i,k)=tmp1
             a(j,i,k) = a(j,i,k) - tmp1 * d(j,i-3,k)
             b(j,i,k) = b(j,i,k) - tmp1 * e(j,i-3,k)

             tmp1=a(j,i,k) / c(j,i-2,k)  ! eliminate a(n)
             a(j,i,k)=tmp1
             b(j,i,k) = b(j,i,k) - tmp1 * d(j,i-2,k)
             c(j,i,k) = c(j,i,k) - tmp1 * e(j,i-2,k)

             tmp1=b(j,i,k) / c(j,i-1,k)    ! eliminate b(n)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do

! endOuterLoop(1)
!            #If "1" == "0"

!            #If "factor" == "solve"

        else
! factorOrSolveExtended(factor,2)

! initFactorOrSolve(factor,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(2)
!            #If "2" == "0"

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i = nia+1
!            #If "factor" == "factor"
             tmp1 = b(j,k,i)/c(j,k,i-1) ! eliminate b(1)
             b(j,k,i)=tmp1         ! save pivots in the element that we eliminate
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
             e(j,k,i) = e(j,k,i) - tmp1 * a(j,k,i-1)
             a(j,k,i) = a(j,k,i) - tmp1 * b(j,k,i-1)

             tmp2 = a(j,k,i+1)/c(j,k,i-1) ! eliminate a(2)
             a(j,k,i+1)=tmp2       ! save pivots in the element that we eliminate
             b(j,k,i+1) = b(j,k,i+1) - tmp2 * d(j,k,i-1)
             c(j,k,i+1) = c(j,k,i+1) - tmp2 * e(j,k,i-1)
             d(j,k,i+1) = d(j,k,i+1) - tmp2 * a(j,k,i-1)
             e(j,k,i+1) = e(j,k,i+1) - tmp2 * b(j,k,i-1)

            i = nia+2
!            #If "factor" == "factor"
             tmp1 = b(j,k,i)/c(j,k,i-1)  ! eliminate b(2)
             b(j,k,i)=tmp1         ! save pivots in the element that we eliminate
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
             e(j,k,i) = e(j,k,i) - tmp1 * a(j,k,i-1)

             tmp2 = a(j,k,i+1)/c(j,k,i-1)  ! eliminate a(3)
             a(j,k,i+1)=tmp2       ! save pivots in the element that we eliminate
             b(j,k,i+1) = b(j,k,i+1) - tmp2 * d(j,k,i-1)
             c(j,k,i+1) = c(j,k,i+1) - tmp2 * e(j,k,i-1)
             d(j,k,i+1) = d(j,k,i+1) - tmp2 * a(j,k,i-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

            do i = nia+3,nib-3
! startInnerLoop(2)
!              #If "2" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,k,i)/c(j,k,i-1)
               b(j,k,i)=tmp1                 ! save pivots in the element that we eliminate
               c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
               d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)

               tmp2 = a(j,k,i+1)/c(j,k,i-1)
               a(j,k,i+1)=tmp2               ! save pivots in the element that we eliminate
               b(j,k,i+1) = b(j,k,i+1) - tmp2 * d(j,k,i-1)
               c(j,k,i+1) = c(j,k,i+1) - tmp2 * e(j,k,i-1)
! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i=nib-2
!             #If "factor" == "factor"
             tmp1=b(j,k,i) / c(j,k,i-1)  ! eliminate b(n-2)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)

            i=nib-1
!             #If "factor" == "factor"
             tmp1=e(j,k,i) / c(j,k,i-3) ! eliminate e(n-1)
             e(j,k,i)=tmp1
             a(j,k,i) = a(j,k,i) - tmp1 * d(j,k,i-3)
             b(j,k,i) = b(j,k,i) - tmp1 * e(j,k,i-3)

             i=nib
!             #If "factor" == "factor"
             tmp1=d(j,k,i) / c(j,k,i-4)    ! eliminate d(n)
             d(j,k,i)=tmp1
             e(j,k,i) = e(j,k,i) - tmp1 * d(j,k,i-4)
             a(j,k,i) = a(j,k,i) - tmp1 * e(j,k,i-4)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "factor" == "factor"
             tmp1=a(j,k,i) / c(j,k,i-2)  ! eliminate a(n-1)
             a(j,k,i)=tmp1
             b(j,k,i) = b(j,k,i) - tmp1 * d(j,k,i-2)
             c(j,k,i) = c(j,k,i) - tmp1 * e(j,k,i-2)

             tmp1=b(j,k,i) / c(j,k,i-1)  ! eliminate b(n-1)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)

             i=nib
!             #If "factor" == "factor"
             tmp1=e(j,k,i) / c(j,k,i-3)  ! eliminate e(n)
             e(j,k,i)=tmp1
             a(j,k,i) = a(j,k,i) - tmp1 * d(j,k,i-3)
             b(j,k,i) = b(j,k,i) - tmp1 * e(j,k,i-3)

             tmp1=a(j,k,i) / c(j,k,i-2)  ! eliminate a(n)
             a(j,k,i)=tmp1
             b(j,k,i) = b(j,k,i) - tmp1 * d(j,k,i-2)
             c(j,k,i) = c(j,k,i) - tmp1 * e(j,k,i-2)

             tmp1=b(j,k,i) / c(j,k,i-1)    ! eliminate b(n)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do

! endOuterLoop(2)
!            #If "2" == "0"

!            #If "factor" == "solve"

        end if

      else if( systemType.eq.periodic )then

        ! ************************************************************
        ! ******************PERIODIC**********************************
        ! ************************************************************

        if( axis.eq.0 )then
! factorOrSolvePeriodic(factor,0)

! initFactorOrSolve(factor,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

!            #If "factor" == "factor"
! startInnerLoop(0)
!             #If "0" != "0"
             i=nia
             s(i,j,k)=a(i,j,k)    ! replace a,b,a in upper right corner
             t(i,j,k)=b(i,j,k)
             i=nia+1
             s(i,j,k)=0.
             t(i,j,k)=a(i,j,k)
             i=nia
             u(i,j,k)=e(n-1,j,k)  ! replace e,d,e in lower left corner
             u(i+1,j,k)=0.
             v(i,j,k)=d(n,j,k)
             v(i+1,j,k)=e(n,j,k)
! endInnerLoop(0)
!             #If "0" != "0"

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(0)
!              #If "0" != "0"
!               #If "factor" == "factor"
               tmp1 = b(i,j,k)/c(i-1,j,k)    ! eliminate b(i)
               b(i,j,k)=tmp1                 ! save pivots in the element that we eliminate

               c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
               d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
               s(i,j,k) = s(i,j,k) - tmp1 * s(i-1,j,k)
               t(i,j,k) = t(i,j,k) - tmp1 * t(i-1,j,k)

               tmp2 = a(i+1,j,k)/c(i-1,j,k)  ! eliminate a(i+1)
               a(i+1,j,k)=tmp2               ! save pivots in the element that we eliminate
               b(i+1,j,k) = b(i+1,j,k) - tmp2 * d(i-1,j,k)
               c(i+1,j,k) = c(i+1,j,k) - tmp2 * e(i-1,j,k)
               s(i+1,j,k) =            - tmp2 * s(i-1,j,k)
               t(i+1,j,k) =            - tmp2 * t(i-1,j,k)
! endInnerLoop(0)
!              #If "0" != "0"

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(0)
!             #If "0" != "0"
             i=n-4
!             #If "factor" == "factor"
             tmp1= b(i,j,k)/c(i-1,j,k)     ! eliminate b(n-4)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
             s(i,j,k) = s(i,j,k) - tmp1 * s(i-1,j,k)
             t(i,j,k) = t(i,j,k) - tmp1 * t(i-1,j,k)

             i=n-3
!             #If "factor" == "factor"
             tmp1= a(i,j,k)/c(i-2,j,k)     ! eliminate a(n-3)
             a(i,j,k)=tmp1
             b(i,j,k) = b(i,j,k) - tmp1 * d(i-2,j,k)
             c(i,j,k) = c(i,j,k) - tmp1 * e(i-2,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * s(i-2,j,k)
             t(i,j,k) =          - tmp1 * t(i-2,j,k)

             tmp1= b(i,j,k)/c(i-1,j,k)    ! eliminate b(n-3)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * s(i-1,j,k)
             t(i,j,k) = t(i,j,k) - tmp1 * t(i-1,j,k)

             i=n-2
!             #If "factor" == "factor"
             tmp1= a(i,j,k)/c(i-2,j,k)    ! eliminate a(n-2)
             a(i,j,k)=tmp1
             b(i,j,k) = b(i,j,k) - tmp1 * d(i-2,j,k)
             c(i,j,k) = c(i,j,k) - tmp1 * e(i-2,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * s(i-2,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * t(i-2,j,k)

             tmp1= b(i,j,k)/c(i-1,j,k)  ! eliminate b(n-2)
             b(i,j,k)=tmp1
             c(i,j,k) = c(i,j,k) - tmp1 * d(i-1,j,k)
             d(i,j,k) = d(i,j,k) - tmp1 * e(i-1,j,k)
             e(i,j,k) = e(i,j,k) - tmp1 * t(i-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(0)
!               #If "0" != "0"

!               #If "factor" == "factor"
               tmp1=u(i,j,k)/c(i,j,k)   ! eliminate u(i)
               u(i,j,k)=tmp1
               u(i+1,j,k)=u(i+1,j,k)-tmp1*d(i,j,k)
               u(i+2,j,k)=          -tmp1*e(i,j,k)
               c(n-1,j,k)=c(n-1,j,k)-tmp1*s(i,j,k)
               d(n-1,j,k)=d(n-1,j,k)-tmp1*t(i,j,k)


               tmp2=v(i,j,k)/c(i,j,k)           ! eliminate v(i)
               v(i,j,k)=tmp2
               v(i+1,j,k)=v(i+1,j,k)-tmp2*d(i,j,k)
               v(i+2,j,k)=          -tmp2*e(i,j,k)
               b(n,j,k)  =b(n,j,k)  -tmp2*s(i,j,k)
               c(n,j,k)  =c(n,j,k)  -tmp2*t(i,j,k)

! endInnerLoop(0)
!              #If "0" != "0"
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(0)
!             #If "0" != "0"
             i=n-5
!             #If "factor" == "factor"
             tmp1=u(i,j,k)/c(i,j,k)  ! eliminate u(n-5)
             u(i,j,k)=tmp1
             u(i+1,j,k)=u(i+1,j,k)-tmp1*d(i,j,k)
             a(n-1,j,k)=a(n-1,j,k)-tmp1*e(i,j,k)
             c(n-1,j,k)=c(n-1,j,k)-tmp1*s(i,j,k)
             d(n-1,j,k)=d(n-1,j,k)-tmp1*t(i,j,k)

             tmp2=v(i,j,k)/c(i,j,k)        ! eliminate v(n-5)
             v(i,j,k)=tmp2
             v(i+1,j,k)=v(i+1,j,k)-tmp2*d(i,j,k)
             v(i+2,j,k)=          -tmp2*e(i,j,k)
             b(n,j,k)  =b(n,j,k)  -tmp2*s(i,j,k)
             c(n,j,k)  =c(n,j,k)  -tmp2*t(i,j,k)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "factor" == "factor"
             tmp1=u(i,j,k)/c(i,j,k)  ! eliminate u(n-4)
             u(i,j,k)=tmp1
             a(n-1,j,k)=a(n-1,j,k)-tmp1*d(i,j,k)
             b(n-1,j,k)=b(n-1,j,k)-tmp1*e(i,j,k)
             c(n-1,j,k)=c(n-1,j,k)-tmp1*s(i,j,k)
             d(n-1,j,k)=d(n-1,j,k)-tmp1*t(i,j,k)

             tmp2=v(i,j,k)/c(i,j,k)        ! eliminate v(n-4)
             v(i,j,k)=tmp2
             v(i+1,j,k)=v(i+1,j,k)-tmp2*d(i,j,k)
             a(n,j,k)  =a(n,j,k)  -tmp2*e(i,j,k)
             b(n,j,k)  =b(n,j,k)  -tmp2*s(i,j,k)
             c(n,j,k)  =c(n,j,k)  -tmp2*t(i,j,k)

             i=n-3
!             #If "factor" == "factor"
             tmp1=a(n-1,j,k)/c(i,j,k)  ! eliminate a(n-1)
             a(n-1,j,k)=tmp1
             b(n-1,j,k)=b(n-1,j,k)-tmp1*d(i,j,k)
             c(n-1,j,k)=c(n-1,j,k)-tmp1*e(i,j,k)
             d(n-1,j,k)=d(n-1,j,k)-tmp1*t(i,j,k)

             tmp2=v(i,j,k)/c(i,j,k)        ! eliminate v(n-3)
             v(i,j,k)=tmp2
             a(n,j,k)  =a(n,j,k)  -tmp2*d(i,j,k)
             b(n,j,k)  =b(n,j,k)  -tmp2*e(i,j,k)
             c(n,j,k)  =c(n,j,k)  -tmp2*t(i,j,k)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "factor" == "factor"
             tmp1=b(n-1,j,k)/c(i,j,k)  ! eliminate b(n-1)
             b(n-1,j,k)=tmp1
             c(n-1,j,k)=c(n-1,j,k)-tmp1*d(i,j,k)
             d(n-1,j,k)=d(n-1,j,k)-tmp1*e(i,j,k)

             tmp1=a(n,j,k)/c(i,j,k)  ! eliminate a(n)
             a(n,j,k)=tmp1
             b(n,j,k)=b(n,j,k)-tmp1*d(i,j,k)
             c(n,j,k)=c(n,j,k)-tmp1*e(i,j,k)

             i=n-1
!             #If "factor" == "factor"
             tmp1=b(n,j,k)/c(i,j,k)  ! eliminate b(n)
             b(n,j,k)=tmp1
             c(n,j,k)=c(n,j,k)-tmp1*d(i,j,k)

! endInnerLoop(0)
!             #If "0" != "0"
! endOuterLoop(0)
!             #If "0" == "0"
             end do
             end do

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "factor" == "solve"

        else if( axis.eq.1 )then
! factorOrSolvePeriodic(factor,1)

! initFactorOrSolve(factor,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(1)
!            #If "1" == "0"

!            #If "factor" == "factor"
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=nia
             s(j,i,k)=a(j,i,k)    ! replace a,b,a in upper right corner
             t(j,i,k)=b(j,i,k)
             i=nia+1
             s(j,i,k)=0.
             t(j,i,k)=a(j,i,k)
             i=nia
             u(j,i,k)=e(j,n-1,k)  ! replace e,d,e in lower left corner
             u(j,i+1,k)=0.
             v(j,i,k)=d(j,n,k)
             v(j,i+1,k)=e(j,n,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(1)
!              #If "1" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,i,k)/c(j,i-1,k)    ! eliminate b(i)
               b(j,i,k)=tmp1                 ! save pivots in the element that we eliminate

               c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
               d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
               s(j,i,k) = s(j,i,k) - tmp1 * s(j,i-1,k)
               t(j,i,k) = t(j,i,k) - tmp1 * t(j,i-1,k)

               tmp2 = a(j,i+1,k)/c(j,i-1,k)  ! eliminate a(i+1)
               a(j,i+1,k)=tmp2               ! save pivots in the element that we eliminate
               b(j,i+1,k) = b(j,i+1,k) - tmp2 * d(j,i-1,k)
               c(j,i+1,k) = c(j,i+1,k) - tmp2 * e(j,i-1,k)
               s(j,i+1,k) =            - tmp2 * s(j,i-1,k)
               t(j,i+1,k) =            - tmp2 * t(j,i-1,k)
! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-4
!             #If "factor" == "factor"
             tmp1= b(j,i,k)/c(j,i-1,k)     ! eliminate b(n-4)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
             s(j,i,k) = s(j,i,k) - tmp1 * s(j,i-1,k)
             t(j,i,k) = t(j,i,k) - tmp1 * t(j,i-1,k)

             i=n-3
!             #If "factor" == "factor"
             tmp1= a(j,i,k)/c(j,i-2,k)     ! eliminate a(n-3)
             a(j,i,k)=tmp1
             b(j,i,k) = b(j,i,k) - tmp1 * d(j,i-2,k)
             c(j,i,k) = c(j,i,k) - tmp1 * e(j,i-2,k)
             e(j,i,k) = e(j,i,k) - tmp1 * s(j,i-2,k)
             t(j,i,k) =          - tmp1 * t(j,i-2,k)

             tmp1= b(j,i,k)/c(j,i-1,k)    ! eliminate b(n-3)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
             e(j,i,k) = e(j,i,k) - tmp1 * s(j,i-1,k)
             t(j,i,k) = t(j,i,k) - tmp1 * t(j,i-1,k)

             i=n-2
!             #If "factor" == "factor"
             tmp1= a(j,i,k)/c(j,i-2,k)    ! eliminate a(n-2)
             a(j,i,k)=tmp1
             b(j,i,k) = b(j,i,k) - tmp1 * d(j,i-2,k)
             c(j,i,k) = c(j,i,k) - tmp1 * e(j,i-2,k)
             d(j,i,k) = d(j,i,k) - tmp1 * s(j,i-2,k)
             e(j,i,k) = e(j,i,k) - tmp1 * t(j,i-2,k)

             tmp1= b(j,i,k)/c(j,i-1,k)  ! eliminate b(n-2)
             b(j,i,k)=tmp1
             c(j,i,k) = c(j,i,k) - tmp1 * d(j,i-1,k)
             d(j,i,k) = d(j,i,k) - tmp1 * e(j,i-1,k)
             e(j,i,k) = e(j,i,k) - tmp1 * t(j,i-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(1)
!               #If "1" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc

!               #If "factor" == "factor"
               tmp1=u(j,i,k)/c(j,i,k)   ! eliminate u(i)
               u(j,i,k)=tmp1
               u(j,i+1,k)=u(j,i+1,k)-tmp1*d(j,i,k)
               u(j,i+2,k)=          -tmp1*e(j,i,k)
               c(j,n-1,k)=c(j,n-1,k)-tmp1*s(j,i,k)
               d(j,n-1,k)=d(j,n-1,k)-tmp1*t(j,i,k)


               tmp2=v(j,i,k)/c(j,i,k)           ! eliminate v(i)
               v(j,i,k)=tmp2
               v(j,i+1,k)=v(j,i+1,k)-tmp2*d(j,i,k)
               v(j,i+2,k)=          -tmp2*e(j,i,k)
               b(j,n,k)  =b(j,n,k)  -tmp2*s(j,i,k)
               c(j,n,k)  =c(j,n,k)  -tmp2*t(j,i,k)

! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-5
!             #If "factor" == "factor"
             tmp1=u(j,i,k)/c(j,i,k)  ! eliminate u(n-5)
             u(j,i,k)=tmp1
             u(j,i+1,k)=u(j,i+1,k)-tmp1*d(j,i,k)
             a(j,n-1,k)=a(j,n-1,k)-tmp1*e(j,i,k)
             c(j,n-1,k)=c(j,n-1,k)-tmp1*s(j,i,k)
             d(j,n-1,k)=d(j,n-1,k)-tmp1*t(j,i,k)

             tmp2=v(j,i,k)/c(j,i,k)        ! eliminate v(n-5)
             v(j,i,k)=tmp2
             v(j,i+1,k)=v(j,i+1,k)-tmp2*d(j,i,k)
             v(j,i+2,k)=          -tmp2*e(j,i,k)
             b(j,n,k)  =b(j,n,k)  -tmp2*s(j,i,k)
             c(j,n,k)  =c(j,n,k)  -tmp2*t(j,i,k)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "factor" == "factor"
             tmp1=u(j,i,k)/c(j,i,k)  ! eliminate u(n-4)
             u(j,i,k)=tmp1
             a(j,n-1,k)=a(j,n-1,k)-tmp1*d(j,i,k)
             b(j,n-1,k)=b(j,n-1,k)-tmp1*e(j,i,k)
             c(j,n-1,k)=c(j,n-1,k)-tmp1*s(j,i,k)
             d(j,n-1,k)=d(j,n-1,k)-tmp1*t(j,i,k)

             tmp2=v(j,i,k)/c(j,i,k)        ! eliminate v(n-4)
             v(j,i,k)=tmp2
             v(j,i+1,k)=v(j,i+1,k)-tmp2*d(j,i,k)
             a(j,n,k)  =a(j,n,k)  -tmp2*e(j,i,k)
             b(j,n,k)  =b(j,n,k)  -tmp2*s(j,i,k)
             c(j,n,k)  =c(j,n,k)  -tmp2*t(j,i,k)

             i=n-3
!             #If "factor" == "factor"
             tmp1=a(j,n-1,k)/c(j,i,k)  ! eliminate a(n-1)
             a(j,n-1,k)=tmp1
             b(j,n-1,k)=b(j,n-1,k)-tmp1*d(j,i,k)
             c(j,n-1,k)=c(j,n-1,k)-tmp1*e(j,i,k)
             d(j,n-1,k)=d(j,n-1,k)-tmp1*t(j,i,k)

             tmp2=v(j,i,k)/c(j,i,k)        ! eliminate v(n-3)
             v(j,i,k)=tmp2
             a(j,n,k)  =a(j,n,k)  -tmp2*d(j,i,k)
             b(j,n,k)  =b(j,n,k)  -tmp2*e(j,i,k)
             c(j,n,k)  =c(j,n,k)  -tmp2*t(j,i,k)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "factor" == "factor"
             tmp1=b(j,n-1,k)/c(j,i,k)  ! eliminate b(n-1)
             b(j,n-1,k)=tmp1
             c(j,n-1,k)=c(j,n-1,k)-tmp1*d(j,i,k)
             d(j,n-1,k)=d(j,n-1,k)-tmp1*e(j,i,k)

             tmp1=a(j,n,k)/c(j,i,k)  ! eliminate a(n)
             a(j,n,k)=tmp1
             b(j,n,k)=b(j,n,k)-tmp1*d(j,i,k)
             c(j,n,k)=c(j,n,k)-tmp1*e(j,i,k)

             i=n-1
!             #If "factor" == "factor"
             tmp1=b(j,n,k)/c(j,i,k)  ! eliminate b(n)
             b(j,n,k)=tmp1
             c(j,n,k)=c(j,n,k)-tmp1*d(j,i,k)

! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do
! endOuterLoop(1)
!             #If "1" == "0"

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "factor" == "solve"

        else
! factorOrSolvePeriodic(factor,2)

! initFactorOrSolve(factor,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "factor" == "factor"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(2)
!            #If "2" == "0"

!            #If "factor" == "factor"
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=nia
             s(j,k,i)=a(j,k,i)    ! replace a,b,a in upper right corner
             t(j,k,i)=b(j,k,i)
             i=nia+1
             s(j,k,i)=0.
             t(j,k,i)=a(j,k,i)
             i=nia
             u(j,k,i)=e(j,k,n-1)  ! replace e,d,e in lower left corner
             u(j,k,i+1)=0.
             v(j,k,i)=d(j,k,n)
             v(j,k,i+1)=e(j,k,n)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(2)
!              #If "2" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "factor" == "factor"
               tmp1 = b(j,k,i)/c(j,k,i-1)    ! eliminate b(i)
               b(j,k,i)=tmp1                 ! save pivots in the element that we eliminate

               c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
               d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
               s(j,k,i) = s(j,k,i) - tmp1 * s(j,k,i-1)
               t(j,k,i) = t(j,k,i) - tmp1 * t(j,k,i-1)

               tmp2 = a(j,k,i+1)/c(j,k,i-1)  ! eliminate a(i+1)
               a(j,k,i+1)=tmp2               ! save pivots in the element that we eliminate
               b(j,k,i+1) = b(j,k,i+1) - tmp2 * d(j,k,i-1)
               c(j,k,i+1) = c(j,k,i+1) - tmp2 * e(j,k,i-1)
               s(j,k,i+1) =            - tmp2 * s(j,k,i-1)
               t(j,k,i+1) =            - tmp2 * t(j,k,i-1)
! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-4
!             #If "factor" == "factor"
             tmp1= b(j,k,i)/c(j,k,i-1)     ! eliminate b(n-4)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
             s(j,k,i) = s(j,k,i) - tmp1 * s(j,k,i-1)
             t(j,k,i) = t(j,k,i) - tmp1 * t(j,k,i-1)

             i=n-3
!             #If "factor" == "factor"
             tmp1= a(j,k,i)/c(j,k,i-2)     ! eliminate a(n-3)
             a(j,k,i)=tmp1
             b(j,k,i) = b(j,k,i) - tmp1 * d(j,k,i-2)
             c(j,k,i) = c(j,k,i) - tmp1 * e(j,k,i-2)
             e(j,k,i) = e(j,k,i) - tmp1 * s(j,k,i-2)
             t(j,k,i) =          - tmp1 * t(j,k,i-2)

             tmp1= b(j,k,i)/c(j,k,i-1)    ! eliminate b(n-3)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
             e(j,k,i) = e(j,k,i) - tmp1 * s(j,k,i-1)
             t(j,k,i) = t(j,k,i) - tmp1 * t(j,k,i-1)

             i=n-2
!             #If "factor" == "factor"
             tmp1= a(j,k,i)/c(j,k,i-2)    ! eliminate a(n-2)
             a(j,k,i)=tmp1
             b(j,k,i) = b(j,k,i) - tmp1 * d(j,k,i-2)
             c(j,k,i) = c(j,k,i) - tmp1 * e(j,k,i-2)
             d(j,k,i) = d(j,k,i) - tmp1 * s(j,k,i-2)
             e(j,k,i) = e(j,k,i) - tmp1 * t(j,k,i-2)

             tmp1= b(j,k,i)/c(j,k,i-1)  ! eliminate b(n-2)
             b(j,k,i)=tmp1
             c(j,k,i) = c(j,k,i) - tmp1 * d(j,k,i-1)
             d(j,k,i) = d(j,k,i) - tmp1 * e(j,k,i-1)
             e(j,k,i) = e(j,k,i) - tmp1 * t(j,k,i-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(2)
!               #If "2" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc

!               #If "factor" == "factor"
               tmp1=u(j,k,i)/c(j,k,i)   ! eliminate u(i)
               u(j,k,i)=tmp1
               u(j,k,i+1)=u(j,k,i+1)-tmp1*d(j,k,i)
               u(j,k,i+2)=          -tmp1*e(j,k,i)
               c(j,k,n-1)=c(j,k,n-1)-tmp1*s(j,k,i)
               d(j,k,n-1)=d(j,k,n-1)-tmp1*t(j,k,i)


               tmp2=v(j,k,i)/c(j,k,i)           ! eliminate v(i)
               v(j,k,i)=tmp2
               v(j,k,i+1)=v(j,k,i+1)-tmp2*d(j,k,i)
               v(j,k,i+2)=          -tmp2*e(j,k,i)
               b(j,k,n)  =b(j,k,n)  -tmp2*s(j,k,i)
               c(j,k,n)  =c(j,k,n)  -tmp2*t(j,k,i)

! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-5
!             #If "factor" == "factor"
             tmp1=u(j,k,i)/c(j,k,i)  ! eliminate u(n-5)
             u(j,k,i)=tmp1
             u(j,k,i+1)=u(j,k,i+1)-tmp1*d(j,k,i)
             a(j,k,n-1)=a(j,k,n-1)-tmp1*e(j,k,i)
             c(j,k,n-1)=c(j,k,n-1)-tmp1*s(j,k,i)
             d(j,k,n-1)=d(j,k,n-1)-tmp1*t(j,k,i)

             tmp2=v(j,k,i)/c(j,k,i)        ! eliminate v(n-5)
             v(j,k,i)=tmp2
             v(j,k,i+1)=v(j,k,i+1)-tmp2*d(j,k,i)
             v(j,k,i+2)=          -tmp2*e(j,k,i)
             b(j,k,n)  =b(j,k,n)  -tmp2*s(j,k,i)
             c(j,k,n)  =c(j,k,n)  -tmp2*t(j,k,i)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "factor" == "factor"
             tmp1=u(j,k,i)/c(j,k,i)  ! eliminate u(n-4)
             u(j,k,i)=tmp1
             a(j,k,n-1)=a(j,k,n-1)-tmp1*d(j,k,i)
             b(j,k,n-1)=b(j,k,n-1)-tmp1*e(j,k,i)
             c(j,k,n-1)=c(j,k,n-1)-tmp1*s(j,k,i)
             d(j,k,n-1)=d(j,k,n-1)-tmp1*t(j,k,i)

             tmp2=v(j,k,i)/c(j,k,i)        ! eliminate v(n-4)
             v(j,k,i)=tmp2
             v(j,k,i+1)=v(j,k,i+1)-tmp2*d(j,k,i)
             a(j,k,n)  =a(j,k,n)  -tmp2*e(j,k,i)
             b(j,k,n)  =b(j,k,n)  -tmp2*s(j,k,i)
             c(j,k,n)  =c(j,k,n)  -tmp2*t(j,k,i)

             i=n-3
!             #If "factor" == "factor"
             tmp1=a(j,k,n-1)/c(j,k,i)  ! eliminate a(n-1)
             a(j,k,n-1)=tmp1
             b(j,k,n-1)=b(j,k,n-1)-tmp1*d(j,k,i)
             c(j,k,n-1)=c(j,k,n-1)-tmp1*e(j,k,i)
             d(j,k,n-1)=d(j,k,n-1)-tmp1*t(j,k,i)

             tmp2=v(j,k,i)/c(j,k,i)        ! eliminate v(n-3)
             v(j,k,i)=tmp2
             a(j,k,n)  =a(j,k,n)  -tmp2*d(j,k,i)
             b(j,k,n)  =b(j,k,n)  -tmp2*e(j,k,i)
             c(j,k,n)  =c(j,k,n)  -tmp2*t(j,k,i)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "factor" == "factor"
             tmp1=b(j,k,n-1)/c(j,k,i)  ! eliminate b(n-1)
             b(j,k,n-1)=tmp1
             c(j,k,n-1)=c(j,k,n-1)-tmp1*d(j,k,i)
             d(j,k,n-1)=d(j,k,n-1)-tmp1*e(j,k,i)

             tmp1=a(j,k,n)/c(j,k,i)  ! eliminate a(n)
             a(j,k,n)=tmp1
             b(j,k,n)=b(j,k,n)-tmp1*d(j,k,i)
             c(j,k,n)=c(j,k,n)-tmp1*e(j,k,i)

             i=n-1
!             #If "factor" == "factor"
             tmp1=b(j,k,n)/c(j,k,i)  ! eliminate b(n)
             b(j,k,n)=tmp1
             c(j,k,n)=c(j,k,n)-tmp1*d(j,k,i)

! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do
! endOuterLoop(2)
!             #If "2" == "0"

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "factor" == "solve"

        end if

      else
        write(*,*) 'pentaFactor:ERROR: invalid system type=',systemType
        stop 7
      end if

      return
      end

      subroutine pentaSolve(nd1a,nd1b,nd2a,nd2b,nd3a,nd3b,ndf1a,ndf1b,
     & ndf2a,ndf2b,ndf3a,ndf3b,ipar, a, b, c, d, e, f, s,t,u,v )
c ========================================================================
c   Solve a penta-diagonal matrix
c             A x = f
c
c  Input: s,t,u,v : work-space for the periodic case (otherwise not used)
c                   These same work-spaces must be passed to the oslve function
c f : input/output: on input the RHS, on output the solution
c ========================================================================

      implicit none
c
      integer nd1a,nd1b,nd2a,nd2b,nd3a,nd3b
      real a(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real b(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real c(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real d(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real e(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real s(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real t(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real u(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)
      real v(nd1a:nd1b,nd2a:nd2b,nd3a:nd3b)

      integer ndf1a,ndf1b,ndf2a,ndf2b,ndf3a,ndf3b
      real f(ndf1a:ndf1b,ndf2a:ndf2b,ndf3a:ndf3b)

      integer ipar(0:*)

c ...... local variables
      integer n1a,n1b,n1c,n2a,n2b,n2c,n3a,n3b,n3c,systemType,axis
      integer nia,nib,nja,njb,njc,nka,nkb,nkc
      integer i,j,k,nx,n
      real tmp1,tmp2

      integer normal,extended,periodic
      parameter( normal=0,extended=1,periodic=2 )
c.........................

      n1a        = ipar(0)
      n1b        = ipar(1)
      n1c        = ipar(2) ! stride
      n2a        = ipar(3)
      n2b        = ipar(4)
      n2c        = ipar(5)
      n3a        = ipar(6)
      n3b        = ipar(7)
      n3c        = ipar(8)
      systemType = ipar(9)
      axis       = ipar(10)

      nx=n1b

      if( systemType.eq.normal )then

        if( axis.eq.0 )then
! factorOrSolveNormal(solve,0)

! initFactorOrSolve(solve,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
           do i = nia+1,nib-1
! startInnerLoop(0)
!             #If "0" != "0"
!               #If "solve" == "factor"
!               #Else
               f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
               f(i+1,j,k) = f(i+1,j,k) - a(i,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!             #If "0" != "0"
           end do

! startInnerLoop(0)
!            #If "0" != "0"
!             #If "solve" == "factor"
!             #Else
             f(n,j,k) = f(n,j,k) - b(n,j,k) * f(n-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do


!            #If "solve" == "solve"
           ! Back Substitution
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

! startInnerLoop(0)
!            #If "0" != "0"
             f(n,j,k) = f(n,j,k) / c(n,j,k)
             f(n-1,j,k) = ( f(n-1,j,k) - d(n-1,j,k)*f(n,j,k)  )/c(n-1,
     & j,k)
! endInnerLoop(0)
!            #If "0" != "0"

           do i=nib-2,nia,-1
! startInnerLoop(0)
!             #If "0" != "0"
              f(i,j,k) = ( f(i,j,k) - d(i,j,k)*f(i+1,j,k) - e(i,j,k)*f(
     & i+2,j,k) ) / c(i,j,k)
! endInnerLoop(0)
!             #If "0" != "0"
           end do

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do

        else if( axis.eq.1 )then
! factorOrSolveNormal(solve,1)

! initFactorOrSolve(solve,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(1)
!            #If "1" == "0"
           do i = nia+1,nib-1
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
               f(j,i+1,k) = f(j,i+1,k) - a(j,i,k) * f(j,i-1,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do
           end do

! startInnerLoop(1)
!            #If "1" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
!             #If "solve" == "factor"
!             #Else
             f(j,n,k) = f(j,n,k) - b(j,n,k) * f(j,n-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

! endOuterLoop(1)
!            #If "1" == "0"


!            #If "solve" == "solve"
           ! Back Substitution
! startOuterLoop(1)
!            #If "1" == "0"

! startInnerLoop(1)
!            #If "1" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
             f(j,n,k) = f(j,n,k) / c(j,n,k)
             f(j,n-1,k) = ( f(j,n-1,k) - d(j,n-1,k)*f(j,n,k)  )/c(j,n-
     & 1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

           do i=nib-2,nia,-1
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
              f(j,i,k) = ( f(j,i,k) - d(j,i,k)*f(j,i+1,k) - e(j,i,k)*f(
     & j,i+2,k) ) / c(j,i,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do
           end do

! endOuterLoop(1)
!            #If "1" == "0"

        else
! factorOrSolveNormal(solve,2)

! initFactorOrSolve(solve,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.2 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 2 points.'')')
             stop 3
           end if

           n=nib

! startOuterLoop(2)
!            #If "2" == "0"
           do i = nia+1,nib-1
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
               f(j,k,i+1) = f(j,k,i+1) - a(j,k,i) * f(j,k,i-1)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do
           end do

! startInnerLoop(2)
!            #If "2" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
!             #If "solve" == "factor"
!             #Else
             f(j,k,n) = f(j,k,n) - b(j,k,n) * f(j,k,n-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

! endOuterLoop(2)
!            #If "2" == "0"


!            #If "solve" == "solve"
           ! Back Substitution
! startOuterLoop(2)
!            #If "2" == "0"

! startInnerLoop(2)
!            #If "2" != "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc
             f(j,k,n) = f(j,k,n) / c(j,k,n)
             f(j,k,n-1) = ( f(j,k,n-1) - d(j,k,n-1)*f(j,k,n)  )/c(j,k,
     & n-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

           do i=nib-2,nia,-1
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
              f(j,k,i) = ( f(j,k,i) - d(j,k,i)*f(j,k,i+1) - e(j,k,i)*f(
     & j,k,i+2) ) / c(j,k,i)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do
           end do

! endOuterLoop(2)
!            #If "2" == "0"

        end if

      else if( systemType.eq.extended )then

        if( axis.eq.0 )then
! factorOrSolveExtended(solve,0)

! initFactorOrSolve(solve,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(0)
!             #If "0" != "0"
            i = nia+1
!            #If "solve" == "factor"
!            #Else
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
             f(i+1,j,k) = f(i+1,j,k) - a(i+1,j,k) * f(i-1,j,k)

            i = nia+2
!            #If "solve" == "factor"
!            #Else
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
             f(i+1,j,k) = f(i+1,j,k) - a(i+1,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

            do i = nia+3,nib-3
! startInnerLoop(0)
!              #If "0" != "0"
!               #If "solve" == "factor"
!               #Else
               f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
               f(i+1,j,k) = f(i+1,j,k) - a(i+1,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!              #If "0" != "0"
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(0)
!             #If "0" != "0"
            i=nib-2
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)

            i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - e(i,j,k) * f(i-3,j,k)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - d(i,j,k) * f(i-4,j,k)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - a(i,j,k) * f(i-2,j,k)
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - e(i,j,k) * f(i-3,j,k)
             f(i,j,k) = f(i,j,k) - a(i,j,k) * f(i-2,j,k)
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!             #If "0" != "0"

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do

!            #If "solve" == "solve"
           ! back substitution
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

! startInnerLoop(0)
!             #If "0" != "0"
             f(n,j,k) = f(n,j,k) / c(n,j,k)
             f(n-1,j,k) = ( f(n-1,j,k) - d(n-1,j,k)*f(n,j,k) )/c(n-1,j,
     & k)
! endInnerLoop(0)
!             #If "0" != "0"

            do i = n-2, nia+2, -1
! startInnerLoop(0)
!               #If "0" != "0"
                f(i,j,k) = ( f(i,j,k) - d(i,j,k)*f(i+1,j,k) - e(i,j,k)*
     & f(i+2,j,k) ) / c(i,j,k)
! endInnerLoop(0)
!               #If "0" != "0"
            end do
! startInnerLoop(0)
!             #If "0" != "0"
             i=nia+1
             f(i,j,k) = ( f(i,j,k) - d(i,j,k)*f(i+1,j,k) - e(i,j,k)*f(
     & i+2,j,k)- a(i,j,k)*f(i+3,j,k) ) / c(i,j,k)
             i=nia
             f(i,j,k) = ( f(i,j,k) - d(i,j,k)*f(i+1,j,k) - e(i,j,k)*f(
     & i+2,j,k)- a(i,j,k)*f(i+3,j,k) - b(i,j,k)*f(i+4,j,k) ) / c(i,j,
     & k)
! endInnerLoop(0)
!             #If "0" != "0"

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do

        else if( axis.eq.1 )then
! factorOrSolveExtended(solve,1)

! initFactorOrSolve(solve,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(1)
!            #If "1" == "0"

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i = nia+1
!            #If "solve" == "factor"
!            #Else
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
             f(j,i+1,k) = f(j,i+1,k) - a(j,i+1,k) * f(j,i-1,k)

            i = nia+2
!            #If "solve" == "factor"
!            #Else
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
             f(j,i+1,k) = f(j,i+1,k) - a(j,i+1,k) * f(j,i-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

            do i = nia+3,nib-3
! startInnerLoop(1)
!              #If "1" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
               f(j,i+1,k) = f(j,i+1,k) - a(j,i+1,k) * f(j,i-1,k)
! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i=nib-2
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)

            i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - e(j,i,k) * f(j,i-3,k)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - d(j,i,k) * f(j,i-4,k)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - a(j,i,k) * f(j,i-2,k)
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - e(j,i,k) * f(j,i-3,k)
             f(j,i,k) = f(j,i,k) - a(j,i,k) * f(j,i-2,k)
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do

! endOuterLoop(1)
!            #If "1" == "0"

!            #If "solve" == "solve"
           ! back substitution
! startOuterLoop(1)
!            #If "1" == "0"

! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             f(j,n,k) = f(j,n,k) / c(j,n,k)
             f(j,n-1,k) = ( f(j,n-1,k) - d(j,n-1,k)*f(j,n,k) )/c(j,n-1,
     & k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do

            do i = n-2, nia+2, -1
! startInnerLoop(1)
!               #If "1" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc
                f(j,i,k) = ( f(j,i,k) - d(j,i,k)*f(j,i+1,k) - e(j,i,k)*
     & f(j,i+2,k) ) / c(j,i,k)
! endInnerLoop(1)
!               #If "1" != "0"
               end do
               end do
            end do
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=nia+1
             f(j,i,k) = ( f(j,i,k) - d(j,i,k)*f(j,i+1,k) - e(j,i,k)*f(
     & j,i+2,k)- a(j,i,k)*f(j,i+3,k) ) / c(j,i,k)
             i=nia
             f(j,i,k) = ( f(j,i,k) - d(j,i,k)*f(j,i+1,k) - e(j,i,k)*f(
     & j,i+2,k)- a(j,i,k)*f(j,i+3,k) - b(j,i,k)*f(j,i+4,k) ) / c(j,i,
     & k)
! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do

! endOuterLoop(1)
!            #If "1" == "0"

        else
! factorOrSolveExtended(solve,2)

! initFactorOrSolve(solve,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib

c             | c d e a b             |  0 
c             | b c d e a             |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n  
! startOuterLoop(2)
!            #If "2" == "0"

            ! *** the first 2 iterations of the loop below are special cases
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i = nia+1
!            #If "solve" == "factor"
!            #Else
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
             f(j,k,i+1) = f(j,k,i+1) - a(j,k,i+1) * f(j,k,i-1)

            i = nia+2
!            #If "solve" == "factor"
!            #Else
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
             f(j,k,i+1) = f(j,k,i+1) - a(j,k,i+1) * f(j,k,i-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

            do i = nia+3,nib-3
! startInnerLoop(2)
!              #If "2" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
               f(j,k,i+1) = f(j,k,i+1) - a(j,k,i+1) * f(j,k,i-1)
! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do
            end do

c         Here is what the matrix looks like now:
c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 b c d e |  n-2 
c             |             e a b c d |  n-1 
c             |             d e a b c |  n

! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
            i=nib-2
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)

            i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - e(j,k,i) * f(j,k,i-3)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - d(j,k,i) * f(j,k,i-4)

c             | c d e a b             |  0 
c             | 0 c d e a             |  1
c         A = | 0 0 c d e             |  2
c             |   0 0 c d e           |  3
c             |      0 0 c d e        |
c             |       . . . . .       |
c             |           0 c d e     |  n-4
c             |           0 0 c d e   |  n-3
c             |             0 0 c d e |  n-2 
c             |             0 a b c d |  n-1 
c             |             0 e a b c |  n

             i=nib-1
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - a(j,k,i) * f(j,k,i-2)
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)

             i=nib
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - e(j,k,i) * f(j,k,i-3)
             f(j,k,i) = f(j,k,i) - a(j,k,i) * f(j,k,i-2)
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do

! endOuterLoop(2)
!            #If "2" == "0"

!            #If "solve" == "solve"
           ! back substitution
! startOuterLoop(2)
!            #If "2" == "0"

! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             f(j,k,n) = f(j,k,n) / c(j,k,n)
             f(j,k,n-1) = ( f(j,k,n-1) - d(j,k,n-1)*f(j,k,n) )/c(j,k,n-
     & 1)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do

            do i = n-2, nia+2, -1
! startInnerLoop(2)
!               #If "2" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc
                f(j,k,i) = ( f(j,k,i) - d(j,k,i)*f(j,k,i+1) - e(j,k,i)*
     & f(j,k,i+2) ) / c(j,k,i)
! endInnerLoop(2)
!               #If "2" != "0"
               end do
               end do
            end do
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=nia+1
             f(j,k,i) = ( f(j,k,i) - d(j,k,i)*f(j,k,i+1) - e(j,k,i)*f(
     & j,k,i+2)- a(j,k,i)*f(j,k,i+3) ) / c(j,k,i)
             i=nia
             f(j,k,i) = ( f(j,k,i) - d(j,k,i)*f(j,k,i+1) - e(j,k,i)*f(
     & j,k,i+2)- a(j,k,i)*f(j,k,i+3) - b(j,k,i)*f(j,k,i+4) ) / c(j,k,
     & i)
! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do

! endOuterLoop(2)
!            #If "2" == "0"

        end if

      else if( systemType.eq.periodic )then

        ! **********************************************************
        ! **************PERIODIC************************************
        ! **********************************************************

        if( axis.eq.0 )then
! factorOrSolvePeriodic(solve,0)

! initFactorOrSolve(solve,0)
!            #If "0" == "0"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n2a
            njb=n2b
            njc=n2c
            nia=n1a
            nib=n1b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

!            #If "solve" == "factor"

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(0)
!              #If "0" != "0"
!               #If "solve" == "factor"
!               #Else
               f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
               f(i+1,j,k) = f(i+1,j,k) - a(i+1,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!              #If "0" != "0"

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(0)
!             #If "0" != "0"
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - a(i,j,k) * f(i-2,j,k)
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)

             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(i,j,k) = f(i,j,k) - a(i,j,k) * f(i-2,j,k)
             f(i,j,k) = f(i,j,k) - b(i,j,k) * f(i-1,j,k)
! endInnerLoop(0)
!            #If "0" != "0"

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(0)
!               #If "0" != "0"

!               #If "solve" == "factor"
!               #Else
               f(n-1,j,k)=f(n-1,j,k)-u(i,j,k)*f(i,j,k)
               f(n,j,k)=f(n,j,k)-v(i,j,k)*f(i,j,k)

! endInnerLoop(0)
!              #If "0" != "0"
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(0)
!             #If "0" != "0"
             i=n-5
!             #If "solve" == "factor"
!             #Else
             f(n-1,j,k)=f(n-1,j,k)-u(i,j,k)*f(i,j,k)
             f(n,j,k)=f(n,j,k)  -v(i,j,k)*f(i,j,k)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(n-1,j,k)=f(n-1,j,k)-u(i,j,k)*f(i,j,k)
             f(n,j,k)=f(n,j,k)  -v(i,j,k)*f(i,j,k)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(n-1,j,k)=f(n-1,j,k)-a(n-1,j,k)*f(i,j,k)
             f(n,j,k)=f(n,j,k)  -v(i,j,k)*f(i,j,k)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(n-1,j,k)=f(n-1,j,k)-b(n-1,j,k)*f(i,j,k)
             f(n,j,k)=f(n,j,k)-a(n,j,k)*f(i,j,k)

             i=n-1
!             #If "solve" == "factor"
!             #Else
             f(n,j,k)=f(n,j,k)-b(n,j,k)*f(i,j,k)

! endInnerLoop(0)
!             #If "0" != "0"
! endOuterLoop(0)
!             #If "0" == "0"
             end do
             end do

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "solve" == "solve"
! startOuterLoop(0)
!            #If "0" == "0"
            do k = nka,nkb,nkc
            do j = nja,njb,njc

! startInnerLoop(0)
!              #If "0" != "0"
             f(n,j,k)=f(n,j,k)/c(n,j,k)
             f(n-1,j,k)=(f(n-1,j,k)-d(n-1,j,k)*f(n,j,k))/c(n-1,j,k)
             f(n-2,j,k)=(f(n-2,j,k)-d(n-2,j,k)*f(n-1,j,k)-e(n-2,j,k)*f(
     & n,j,k))/c(n-2,j,k)
             f(n-3,j,k)=(f(n-3,j,k)-d(n-3,j,k)*f(n-2,j,k)-e(n-3,j,k)*f(
     & n-1,j,k)-t(n-3,j,k)*f(n,j,k))/c(n-3,j,k)
! endInnerLoop(0)
!              #If "0" != "0"

             do i=n-4,nia,-1
! startInnerLoop(0)
!               #If "0" != "0"
                f(i,j,k)=(f(i,j,k)-d(i,j,k)*f(i+1,j,k)-e(i,j,k)*f(i+2,
     & j,k)-s(i,j,k)*f(n-1,j,k)-t(i,j,k)*f(n,j,k))/c(i,j,k)
! endInnerLoop(0)
!               #If "0" != "0"
             end do

! endOuterLoop(0)
!            #If "0" == "0"
            end do
            end do

        else if( axis.eq.1 )then
! factorOrSolvePeriodic(solve,1)

! initFactorOrSolve(solve,1)
!            #If "1" == "0"
!            #Elif "1" == "1"
            nka=n3a
            nkb=n3b
            nkc=n3c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n2a
            nib=n2b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(1)
!            #If "1" == "0"

!            #If "solve" == "factor"

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(1)
!              #If "1" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
               f(j,i+1,k) = f(j,i+1,k) - a(j,i+1,k) * f(j,i-1,k)
! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - a(j,i,k) * f(j,i-2,k)
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)

             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(j,i,k) = f(j,i,k) - a(j,i,k) * f(j,i-2,k)
             f(j,i,k) = f(j,i,k) - b(j,i,k) * f(j,i-1,k)
! endInnerLoop(1)
!            #If "1" != "0"
            end do
            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(1)
!               #If "1" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc

!               #If "solve" == "factor"
!               #Else
               f(j,n-1,k)=f(j,n-1,k)-u(j,i,k)*f(j,i,k)
               f(j,n,k)=f(j,n,k)-v(j,i,k)*f(j,i,k)

! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(1)
!             #If "1" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-5
!             #If "solve" == "factor"
!             #Else
             f(j,n-1,k)=f(j,n-1,k)-u(j,i,k)*f(j,i,k)
             f(j,n,k)=f(j,n,k)  -v(j,i,k)*f(j,i,k)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(j,n-1,k)=f(j,n-1,k)-u(j,i,k)*f(j,i,k)
             f(j,n,k)=f(j,n,k)  -v(j,i,k)*f(j,i,k)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(j,n-1,k)=f(j,n-1,k)-a(j,n-1,k)*f(j,i,k)
             f(j,n,k)=f(j,n,k)  -v(j,i,k)*f(j,i,k)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(j,n-1,k)=f(j,n-1,k)-b(j,n-1,k)*f(j,i,k)
             f(j,n,k)=f(j,n,k)-a(j,n,k)*f(j,i,k)

             i=n-1
!             #If "solve" == "factor"
!             #Else
             f(j,n,k)=f(j,n,k)-b(j,n,k)*f(j,i,k)

! endInnerLoop(1)
!             #If "1" != "0"
             end do
             end do
! endOuterLoop(1)
!             #If "1" == "0"

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "solve" == "solve"
! startOuterLoop(1)
!            #If "1" == "0"

! startInnerLoop(1)
!              #If "1" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
             f(j,n,k)=f(j,n,k)/c(j,n,k)
             f(j,n-1,k)=(f(j,n-1,k)-d(j,n-1,k)*f(j,n,k))/c(j,n-1,k)
             f(j,n-2,k)=(f(j,n-2,k)-d(j,n-2,k)*f(j,n-1,k)-e(j,n-2,k)*f(
     & j,n,k))/c(j,n-2,k)
             f(j,n-3,k)=(f(j,n-3,k)-d(j,n-3,k)*f(j,n-2,k)-e(j,n-3,k)*f(
     & j,n-1,k)-t(j,n-3,k)*f(j,n,k))/c(j,n-3,k)
! endInnerLoop(1)
!              #If "1" != "0"
              end do
              end do

             do i=n-4,nia,-1
! startInnerLoop(1)
!               #If "1" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc
                f(j,i,k)=(f(j,i,k)-d(j,i,k)*f(j,i+1,k)-e(j,i,k)*f(j,i+
     & 2,k)-s(j,i,k)*f(j,n-1,k)-t(j,i,k)*f(j,n,k))/c(j,i,k)
! endInnerLoop(1)
!               #If "1" != "0"
               end do
               end do
             end do

! endOuterLoop(1)
!            #If "1" == "0"

        else
! factorOrSolvePeriodic(solve,2)

! initFactorOrSolve(solve,2)
!            #If "2" == "0"
!            #Elif "2" == "1"
!            #Elif "2" == "2"
            nka=n2a
            nkb=n2b
            nkc=n2c
            nja=n1a
            njb=n1b
            njc=n1c
            nia=n3a
            nib=n3b

c check the macro args
!            #If "solve" == "factor"
!            #Elif "solve" == "solve"

           if( nib-nia+1.lt.6 )then
             write(*,'(''pentaFactor:ERROR too few points. Need at 
     & least 6 points.'')')
             stop 3
           end if

           n=nib


c       Here is the matrix in the periodic case:
c
c             | c d e             a b |  0 
c             | b c d e             a |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | e             a b c d |  n-1 
c             | d e             a b c |  n  
c
! startOuterLoop(2)
!            #If "2" == "0"

!            #If "solve" == "factor"

c             | c d e             s t |  0 
c             | b c d e           s t |  1
c         A = | a b c d e             |  2
c             |   a b c d e           |  3
c             |      a b c d e        |
c             |       . . . . .       |
c             |           a b c d e   |  n-3
c             |             a b c d e |  n-2 
c             | u u           a b c d |  n-1 
c             | v v             a b c |  n  

            do i = nia+1,n-5

! startInnerLoop(2)
!              #If "2" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
!               #If "solve" == "factor"
!               #Else
               f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
               f(j,k,i+1) = f(j,k,i+1) - a(j,k,i+1) * f(j,k,i-1)
! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do

            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 b c d e s t |  n-4
c             |             a b c d e   |  n-3
c             |               a b c d e |  n-2 
c             | e               a b c d |  n-1 
c             | d e               a b c |  n

             !  b(n-4), a(n-3)
             !  b(n-3), a(n-2)
             !  b(n-2)
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - a(j,k,i) * f(j,k,i-2)
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)

             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(j,k,i) = f(j,k,i) - a(j,k,i) * f(j,k,i-2)
             f(j,k,i) = f(j,k,i) - b(j,k,i) * f(j,k,i-1)
! endInnerLoop(2)
!            #If "2" != "0"
            end do
            end do

c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | u u             a b c d |  n-1 
c             | v v               a b c |  n



c         *** eliminate e(n-1), d(n) --> save pivots in u,v
             do i=nia,n-6
! startInnerLoop(2)
!               #If "2" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc

!               #If "solve" == "factor"
!               #Else
               f(j,k,n-1)=f(j,k,n-1)-u(j,k,i)*f(j,k,i)
               f(j,k,n)=f(j,k,n)-v(j,k,i)*f(j,k,i)

! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do
            end do
c         Here is what the matrix looks like now:
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 u u a b c d |  n-1 
c             | 0 0 0 ... 0 v v 0 a b c |  n
c               0 1 2 ...  n-5        n
c
! startInnerLoop(2)
!             #If "2" != "0"
             do k = nka,nkb,nkc
             do j = nja,njb,njc
             i=n-5
!             #If "solve" == "factor"
!             #Else
             f(j,k,n-1)=f(j,k,n-1)-u(j,k,i)*f(j,k,i)
             f(j,k,n)=f(j,k,n)  -v(j,k,i)*f(j,k,i)


c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 u a b c d |  n-1 
c             | 0 0 0 ... 0 0 v v a b c |  n
c               0 1 2 ...    n-4      n
c
             i=n-4
!             #If "solve" == "factor"
!             #Else
             f(j,k,n-1)=f(j,k,n-1)-u(j,k,i)*f(j,k,i)
             f(j,k,n)=f(j,k,n)  -v(j,k,i)*f(j,k,i)

             i=n-3
!             #If "solve" == "factor"
!             #Else
             f(j,k,n-1)=f(j,k,n-1)-a(j,k,n-1)*f(j,k,i)
             f(j,k,n)=f(j,k,n)  -v(j,k,i)*f(j,k,i)

c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 b c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 a b c |  n
c               0 1 2 ...        n-2  n
c
             i=n-2
!             #If "solve" == "factor"
!             #Else
             f(j,k,n-1)=f(j,k,n-1)-b(j,k,n-1)*f(j,k,i)
             f(j,k,n)=f(j,k,n)-a(j,k,n)*f(j,k,i)

             i=n-1
!             #If "solve" == "factor"
!             #Else
             f(j,k,n)=f(j,k,n)-b(j,k,n)*f(j,k,i)

! endInnerLoop(2)
!             #If "2" != "0"
             end do
             end do
! endOuterLoop(2)
!             #If "2" == "0"

           ! back substitution
c         Here is what the matrix looks at the end
c             | c d e               s t |  0 
c             | 0 c d e             s t |  1
c         A = | 0 0 c d e           s t |  2
c             |   0 0 c d e         s t |  3
c             |      0 0 c d e      s t |
c             |       . . . . .     . . |
c             |         0 c d e     s t |  n-6
c             |         0 0 c d e   s t |  n-5
c             |           0 0 c d e s t |  n-4
c             |             0 0 c d e t |  n-3
c             |               0 0 c d e |  n-2 
c             | 0 0 0 ... 0 0 0 0 0 c d |  n-1 
c             | 0 0 0 ... 0 0 0 0 0 0 c |  n
c               0 1 2 ...  n-5        n
c
!            #If "solve" == "solve"
! startOuterLoop(2)
!            #If "2" == "0"

! startInnerLoop(2)
!              #If "2" != "0"
              do k = nka,nkb,nkc
              do j = nja,njb,njc
             f(j,k,n)=f(j,k,n)/c(j,k,n)
             f(j,k,n-1)=(f(j,k,n-1)-d(j,k,n-1)*f(j,k,n))/c(j,k,n-1)
             f(j,k,n-2)=(f(j,k,n-2)-d(j,k,n-2)*f(j,k,n-1)-e(j,k,n-2)*f(
     & j,k,n))/c(j,k,n-2)
             f(j,k,n-3)=(f(j,k,n-3)-d(j,k,n-3)*f(j,k,n-2)-e(j,k,n-3)*f(
     & j,k,n-1)-t(j,k,n-3)*f(j,k,n))/c(j,k,n-3)
! endInnerLoop(2)
!              #If "2" != "0"
              end do
              end do

             do i=n-4,nia,-1
! startInnerLoop(2)
!               #If "2" != "0"
               do k = nka,nkb,nkc
               do j = nja,njb,njc
                f(j,k,i)=(f(j,k,i)-d(j,k,i)*f(j,k,i+1)-e(j,k,i)*f(j,k,
     & i+2)-s(j,k,i)*f(j,k,n-1)-t(j,k,i)*f(j,k,n))/c(j,k,i)
! endInnerLoop(2)
!               #If "2" != "0"
               end do
               end do
             end do

! endOuterLoop(2)
!            #If "2" == "0"

        end if

      else
        write(*,*) 'pentaSolve:ERROR: invalid system type=',systemType
        stop 6
      end if

      return
      end




